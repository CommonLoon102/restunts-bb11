#!/usr/bin/env python3
"""Exercise archive integrity failures before release publication."""

import argparse
import contextlib
import importlib.util
import io
import os
from pathlib import Path
import stat
import struct
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
ELF_TARGETS = {
    "freebsd-x64": "FreeBSD x86-64",
    "haiku-x64": "Haiku x86-64",
    "haiku-x86": "Haiku 32-bit x86 with SSE2",
    "openbsd-x64": "OpenBSD x86-64",
    "netbsd-x64": "NetBSD x86-64",
    "netbsd-x86": "NetBSD 32-bit x86 with SSE2",
    "netbsd-x86-no-sse2": "NetBSD 32-bit x86; SSE2 instructions are disabled",
}


class ReleasePackageTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.output = self.root / "archives"

    def runtime_data(self, target, name):
        if target not in ELF_TARGETS or name not in PACKAGES.ELF_FILES:
            return FILE_DATA
        elf_class, machine = PACKAGES.ELF_ARCHITECTURES[target.split("-")[1]]
        header = bytearray(PACKAGES.ELF_HEADER_SIZES[elf_class])
        header[:len(PACKAGES.ELF_MAGIC)] = PACKAGES.ELF_MAGIC
        header[PACKAGES.ELF_CLASS_OFFSET] = elf_class
        header[PACKAGES.ELF_DATA_OFFSET] = PACKAGES.ELF_DATA_LSB
        struct.pack_into("<H", header, PACKAGES.ELF_MACHINE_OFFSET, machine)
        return bytes(header) + FILE_DATA

    def create(self, target, asset_data=None):
        runtime = self.root / target
        for name in PACKAGES.required_files(target) - {PACKAGES.README}:
            path = runtime / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(self.runtime_data(target, name))
            if name in {"bin/restunts", "bin/repldump", "bin/pixldump", "run-restunts.sh"}:
                path.chmod(PACKAGES.EXECUTABLE_MODE)
        for name, content in (asset_data or {}).items():
            path = runtime / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(content)
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

    def test_elf_archives_include_native_runtime_and_platform_instructions(self):
        for target, platform in ELF_TARGETS.items():
            with self.subTest(target=target):
                archive = self.create(target)
                self.assertEqual(archive.name, f"restunts-{target}.tar.gz")
                files, modes = PACKAGES.archive_contents(archive)
                for executable in PACKAGES.NATIVE_EXECUTABLES:
                    name = f"bin/{executable}"
                    self.assertEqual(files[name], self.runtime_data(target, name))
                    self.assertTrue(modes[name] & stat.S_IXUSR)
                for required in ("lib/libnuked-opl2.so", "share/restunts/nuked-opl2-lite/opl2.c",
                                 "share/licenses/restunts/Nuked-OPL2-LICENSE",
                                 "bin/opponents/game/opp6.png", "bin/skyboxes/sky4-3.png",
                                 "bin/menus/main.png", "bin/menus/showroom.png",
                                 "bin/menus/opponent.png"):
                    self.assertIn(required, files)
                self.assertIn(platform, files[PACKAGES.README].decode("utf-8"))
                if target.startswith("haiku-"):
                    self.assertIn("share/docs/restunts/haiku.md", files)
                    self.assertIn("share/docs/restunts/haiku.md", files[PACKAGES.README].decode("utf-8"))

    def test_elf_executables_must_have_execute_permission(self):
        for target in ELF_TARGETS:
            files = {name: self.runtime_data(target, name) for name in PACKAGES.required_files(target)}
            modes = {name: PACKAGES.FILE_MODE for name in files}
            for executable in PACKAGES.NATIVE_EXECUTABLES:
                modes[f"bin/{executable}"] = PACKAGES.EXECUTABLE_MODE
            PACKAGES.validate_contents(target, files, modes)
            for executable in PACKAGES.NATIVE_EXECUTABLES:
                name = f"bin/{executable}"
                with self.subTest(target=target, executable=executable), \
                        self.assertRaisesRegex(ValueError, f"Missing executable permission: {name}"):
                    PACKAGES.validate_contents(target, files, {**modes, name: PACKAGES.FILE_MODE})

    def test_elf_archives_reject_wrong_or_missing_elf_architecture(self):
        for target in ELF_TARGETS:
            files = {name: self.runtime_data(target, name) for name in PACKAGES.required_files(target)}
            modes = {name: PACKAGES.EXECUTABLE_MODE for name in files}
            for name in PACKAGES.ELF_FILES:
                wrong_class = bytearray(files[name])
                wrong_class[PACKAGES.ELF_CLASS_OFFSET] = (
                    PACKAGES.ELF_CLASS_32 if target.endswith("-x64") else PACKAGES.ELF_CLASS_64)
                wrong_machine = bytearray(files[name])
                machine = PACKAGES.ELF_MACHINE_X86 if target.endswith("-x64") else PACKAGES.ELF_MACHINE_X64
                struct.pack_into("<H", wrong_machine, PACKAGES.ELF_MACHINE_OFFSET, machine)
                for content in (FILE_DATA, files[name][:PACKAGES.ELF_MACHINE_OFFSET],
                                wrong_class, wrong_machine):
                    with self.subTest(target=target, name=name, content=content), \
                            self.assertRaisesRegex(ValueError, "ELF"):
                        PACKAGES.validate_contents(target, {**files, name: content}, modes)

    def test_complete_release_requires_every_elf_archive(self):
        for target in PACKAGES.TARGETS:
            self.create(target)
        arguments = argparse.Namespace(directory=self.output, commit=COMMIT, all=True,
                                       run_id=RUN_ID, run_attempt=RUN_ATTEMPT)
        for target in ELF_TARGETS:
            archive = self.output / PACKAGES.archive_name(target)
            saved = self.root / archive.name
            archive.rename(saved)
            try:
                with self.subTest(target=target), \
                        self.assertRaisesRegex(ValueError, f"missing=.*{archive.name}"):
                    PACKAGES.verify_directory(arguments)
            finally:
                saved.rename(archive)

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
                        "bin/menus/main.png", "bin/menus/showroom.png", "bin/menus/opponent.png",
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
        for missing in ("share/restunts/wasm-relink/objects/main.c.o",
                        "share/restunts/wasm-relink/data/assets/menus/main.png",
                        "share/restunts/wasm-relink/data/assets/menus/showroom.png",
                        "share/restunts/wasm-relink/data/assets/menus/opponent.png"):
            incomplete = files.copy()
            incomplete.pop(missing)
            with self.subTest(missing=missing), self.assertRaisesRegex(ValueError, "missing packaged"):
                PACKAGES.validate_contents("browser", incomplete, {})
        for target in ("dos16", "dos32"):
            self.assertFalse(any("nuked" in name for name in PACKAGES.required_files(target)))

    def test_source_music_is_required_and_survives_archiving(self):
        source = self.root / "source"
        tracks = ("titl", "slct", "over", "vict")
        targets = {"linux-x64": "bin/music", "dos32": "bin/music",
                   "browser": "share/restunts/wasm-relink/data/assets/music"}
        with mock.patch.object(PACKAGES, "ROOT", source):
            baseline = {target: PACKAGES.required_files(target) for target in targets}
            for track in tracks:
                path = source / "assets/music" / f"{track}.ogg"
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(b"OggS archive payload " + track.encode("ascii"))
            for target, directory in targets.items():
                audio = {f"{directory}/{track}.ogg":
                         (source / "assets/music" / f"{track}.ogg").read_bytes() for track in tracks}
                files = {name: FILE_DATA for name in baseline[target]} | audio
                modes = {name: PACKAGES.EXECUTABLE_MODE for name in files}
                with self.subTest(target=target):
                    PACKAGES.validate_contents(target, files, modes)
                    for missing in audio:
                        incomplete = files.copy()
                        incomplete.pop(missing)
                        with self.assertRaisesRegex(ValueError, "missing packaged files"):
                            PACKAGES.validate_contents(target, incomplete, modes)
                    for wrong in ("menu.ogg", "SLCT.ogg", "slct.wav"):
                        with self.assertRaisesRegex(ValueError, "Unexpected package file"):
                            PACKAGES.validate_contents(
                                target, files | {f"{directory}/{wrong}": FILE_DATA}, modes)
                    archive = self.create(target, audio)
                    archived, _ = PACKAGES.archive_contents(archive)
                    for name, content in audio.items():
                        self.assertEqual(archived[name], content)
            self.assertFalse(any("/music/" in name for name in PACKAGES.required_files("dos16")))

    def test_absent_music_is_optional(self):
        with mock.patch.object(PACKAGES, "ROOT", self.root / "empty-source"):
            for target in ("linux-x64", "dos32", "browser"):
                with self.subTest(target=target):
                    self.assertEqual(PACKAGES.optional_music_files(target), set())
                    files = {name: FILE_DATA for name in PACKAGES.required_files(target)}
                    modes = {name: PACKAGES.EXECUTABLE_MODE for name in files}
                    PACKAGES.validate_contents(target, files, modes)

    def test_decoder_files_follow_install_paths(self):
        decoder_files = {"THIRD-PARTY-NOTICES.txt", "share/licenses/restunts/stb-LICENSE",
                         "share/licenses/restunts/libvpx-LICENSE",
                         "share/licenses/restunts/libvpx-PATENTS",
                         "share/licenses/restunts/nestegg-LICENSE"}
        for target in ("linux-x64", "dos32", "browser"):
            expected = decoder_files.copy()
            if target == "browser":
                expected.add("share/restunts/wasm-relink/lib/librestunts_webm.a")
            files = {name: FILE_DATA for name in PACKAGES.required_files(target)}
            modes = {name: PACKAGES.EXECUTABLE_MODE for name in files}
            with self.subTest(target=target):
                self.assertTrue(expected <= files.keys())
                PACKAGES.validate_contents(target, files, modes)
                for missing in expected:
                    incomplete = files.copy()
                    incomplete.pop(missing)
                    with self.assertRaisesRegex(ValueError, "missing packaged files"):
                        PACKAGES.validate_contents(target, incomplete, modes)


if __name__ == "__main__":
    unittest.main()
