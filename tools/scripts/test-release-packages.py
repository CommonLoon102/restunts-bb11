#!/usr/bin/env python3
"""Exercise archive integrity failures before release publication."""

import argparse
import contextlib
import importlib.util
import io
import os
from pathlib import Path
import stat
import subprocess
import tarfile
import tempfile
import unittest
from unittest import mock
import zipfile


SCRIPT = Path(__file__).with_name("release-packages.py")
SPEC = importlib.util.spec_from_file_location("release_packages", SCRIPT)
PACKAGES = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PACKAGES)
COMMIT = "a" * 40
OTHER_COMMIT = "b" * 40
RUN_ID = "12345"
RUN_ATTEMPT = "2"
FILE_DATA = b"release fixture\n"


class ReleasePackageTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.output = self.root / "archives"

    def create(self, target):
        runtime = self.root / target
        for name in PACKAGES.required_files(target) - {PACKAGES.README}:
            path = runtime / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(FILE_DATA)
            if name in {"bin/restunts", "bin/repldump", "bin/pixldump", "run-restunts.sh"}:
                path.chmod(PACKAGES.EXECUTABLE_MODE)
        arguments = argparse.Namespace(target=target, runtime=runtime,
                                       directory=self.output, commit=COMMIT)
        environment = {"GITHUB_RUN_ID": RUN_ID, "GITHUB_RUN_ATTEMPT": RUN_ATTEMPT}
        with mock.patch.object(subprocess, "check_output", return_value=COMMIT + "\n"), \
                mock.patch.dict(os.environ, environment), contextlib.redirect_stdout(io.StringIO()):
            PACKAGES.create_package(arguments)
        return self.output / PACKAGES.archive_name(target)

    def checksum(self, archive):
        archive.with_name(archive.name + ".sha256").write_text(
            f"{PACKAGES.sha256(archive.read_bytes())}  {archive.name}\n", encoding="ascii")

    def test_complete_matrix_and_executable_permissions_survive_archiving(self):
        for target in PACKAGES.TARGETS:
            self.create(target)
        arguments = argparse.Namespace(directory=self.output, commit=COMMIT, all=True,
                                       run_id=RUN_ID, run_attempt=RUN_ATTEMPT)
        with contextlib.redirect_stdout(io.StringIO()):
            PACKAGES.verify_directory(arguments)
        files, modes = PACKAGES.archive_contents(self.output / PACKAGES.archive_name("linux-x64"))
        self.assertTrue(modes["bin/restunts"] & stat.S_IXUSR)
        self.assertEqual(files["bin/restunts"], FILE_DATA)

    def test_missing_target_and_unexpected_release_asset_are_rejected(self):
        self.create("dos16")
        arguments = argparse.Namespace(directory=self.output, commit=COMMIT, all=True,
                                       run_id=RUN_ID, run_attempt=RUN_ATTEMPT)
        with self.assertRaisesRegex(ValueError, "Release file set differs"):
            PACKAGES.verify_directory(arguments)
        (self.output / "injected.exe").write_bytes(FILE_DATA)
        with self.assertRaisesRegex(ValueError, "injected.exe"):
            PACKAGES.verify_directory(arguments)

    def test_modified_archive_fails_checksum(self):
        archive = self.create("dos16")
        with archive.open("ab") as output:
            output.write(b"modified after upload")
        with self.assertRaisesRegex(ValueError, "SHA256 mismatch"):
            PACKAGES.verify_package(archive, "dos16", COMMIT)

    def test_repackaged_binary_fails_file_manifest_even_with_new_archive_checksum(self):
        archive = self.create("linux-x64")
        files, modes = PACKAGES.archive_contents(archive)
        files["bin/restunts"] = b"substituted executable"
        with tarfile.open(archive, "w:gz") as output:
            for name, data in files.items():
                info = tarfile.TarInfo(name)
                info.size = len(data)
                info.mode = modes[name]
                output.addfile(info, io.BytesIO(data))
        self.checksum(archive)
        with self.assertRaisesRegex(ValueError, "File differs from manifest"):
            PACKAGES.verify_package(archive, "linux-x64", COMMIT)

    def test_wrong_source_run_and_attempt_are_rejected(self):
        archive = self.create("dos16")
        with self.assertRaisesRegex(ValueError, "Wrong source commit"):
            PACKAGES.verify_package(archive, "dos16", OTHER_COMMIT)
        with self.assertRaisesRegex(ValueError, "Wrong workflow run"):
            PACKAGES.verify_package(archive, "dos16", COMMIT, "unrelated-run", RUN_ATTEMPT)
        with self.assertRaisesRegex(ValueError, "Wrong run attempt"):
            PACKAGES.verify_package(archive, "dos16", COMMIT, RUN_ID, "1")

    def test_original_data_unknown_suffixes_and_other_extra_files_are_rejected(self):
        files = {name: FILE_DATA for name in PACKAGES.required_files("dos32")}
        modes = {name: PACKAGES.FILE_MODE for name in files}
        for extra in ("bin/MISC.RES", "bin/DEFAULT.P3S", "assets.zip", "unrelated.exe"):
            with self.subTest(extra=extra), self.assertRaises(ValueError):
                PACKAGES.validate_contents("dos32", dict(files, **{extra: FILE_DATA}), modes)

    def test_missing_artwork_library_source_and_license_are_rejected(self):
        files = {name: FILE_DATA for name in PACKAGES.required_files("windows-arm64")}
        modes = {name: PACKAGES.FILE_MODE for name in files}
        for missing in ("bin/opponents/game/opp6.png", "bin/skyboxes/sky4-3.png",
                        "bin/nuked-opl2.dll", "share/restunts/nuked-opl2-lite/opl2.c",
                        "share/licenses/restunts/Nuked-OPL2-LICENSE"):
            incomplete = files.copy()
            incomplete.pop(missing)
            with self.subTest(missing=missing), self.assertRaisesRegex(ValueError, "missing packaged"):
                PACKAGES.validate_contents("windows-arm64", incomplete, modes)

    def test_dos_runtime_filenames_do_not_require_long_filename_support(self):
        files = {name: FILE_DATA for name in PACKAGES.required_files("dos32")}
        modes = {name: PACKAGES.FILE_MODE for name in files}
        PACKAGES.validate_contents("dos32", files, modes)
        for extra in ("bin/opponents/game/opp1.png", "bin/skyboxes/image.long", "bin/with space/a.png"):
            with self.subTest(extra=extra), self.assertRaisesRegex(ValueError, "8.3 names"):
                PACKAGES.validate_contents("dos32", dict(files, **{extra: FILE_DATA}), modes)

    def test_unsafe_paths_and_links_are_rejected_without_extraction(self):
        for path in ("../outside", "/absolute", "C:/windows.exe", "file:stream",
                     "folder\\file", "a/./b", ".", "bad\nname"):
            with self.subTest(path=path), self.assertRaisesRegex(ValueError, "Unsafe"):
                PACKAGES.checked_path(path)
        archive = self.root / "link.zip"
        with zipfile.ZipFile(archive, "w") as output:
            entry = zipfile.ZipInfo("symlink")
            entry.external_attr = (stat.S_IFLNK | PACKAGES.EXECUTABLE_MODE) << 16
            output.writestr(entry, "outside")
        with self.assertRaisesRegex(ValueError, "Unsupported ZIP member"):
            PACKAGES.archive_contents(archive)

    def test_browser_requires_relink_objects_and_dos_omits_nuked(self):
        files = {name: FILE_DATA for name in PACKAGES.required_files("browser")}
        files.pop("share/restunts/wasm-relink/objects/main.c.o")
        with self.assertRaisesRegex(ValueError, "missing packaged"):
            PACKAGES.validate_contents("browser", files, {})
        for target in ("dos16", "dos32"):
            self.assertFalse(any("nuked" in name for name in PACKAGES.required_files(target)))


if __name__ == "__main__":
    unittest.main()
