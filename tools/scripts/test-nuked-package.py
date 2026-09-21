"""Verify relocated Linux packages and replacement of their shared Nuked library.

Requires an existing native CMake build with tests enabled, the game test assets,
a C compiler, CMake, and readelf. Everything installed or rebuilt by this test
lives in a temporary directory; the existing build and its binaries are retained.
"""

import argparse
import hashlib
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile


ROOT = Path(__file__).resolve().parents[2]
LIBRARY_NAME = "libnuked-opl2.so"
SOURCE_DIRECTORY = Path("share/restunts/nuked-opl2-lite")
MARKER = "RESTUNTS_REPLACEMENT_NUKED_LIBRARY_LOADED"
SOURCE_FILES = ("opl2.c", "opl2.h", "CMakeLists.txt", "LICENSE", "README.md")


def require(condition, message):
    if not condition:
        raise ValueError(message)


def command(arguments, *, cwd=None, env=None, timeout=120):
    result = subprocess.run([str(argument) for argument in arguments], cwd=cwd, env=env,
                            capture_output=True, text=True, timeout=timeout)
    if result.returncode:
        raise ValueError(f"command failed ({result.returncode}): {arguments}\n"
                         f"{result.stdout}{result.stderr}")
    return result.stdout + result.stderr


def cache_values(build):
    values = {}
    for line in (build / "CMakeCache.txt").read_text(encoding="utf-8").splitlines():
        if line.startswith(("#", "//")) or "=" not in line or ":" not in line:
            continue
        name, value = line.split("=", 1)
        values[name.split(":", 1)[0]] = value
    return values


def matching_file(installed, expected):
    require(installed.is_file(), f"missing packaged file: {installed}")
    require(installed.read_bytes() == expected.read_bytes(),
            f"packaged file differs from the build source: {installed}")


def verify_contents(prefix, component, source, build, bundled_sdl):
    library = prefix / "lib" / LIBRARY_NAME
    require(library.is_file(), f"missing replaceable shared library: {library}")
    vendored = source / "third_party/nuked-opl2-lite"
    for name in SOURCE_FILES:
        matching_file(prefix / SOURCE_DIRECTORY / name, vendored / name)
    matching_file(prefix / SOURCE_DIRECTORY / "nuked-build-info.txt",
                  build / "nuked-build-info.txt")
    context_files = ("CMakeLists.txt", "cmake/nuked-build-info.txt.in",
                     "cmake/toolchains/linux-x86.cmake", "cmake/toolchains/mingw-x86.cmake",
                     "cmake/toolchains/mingw-x64.cmake")
    for name in context_files:
        matching_file(prefix / SOURCE_DIRECTORY / "build-context" / name, source / name)
    license_file = prefix / "share/licenses/restunts/Nuked-OPL2-LICENSE"
    matching_file(license_file, vendored / "LICENSE")
    license_text = license_file.read_text(encoding="utf-8")
    require("GNU LESSER GENERAL PUBLIC LICENSE" in license_text and
            "Version 2.1, February 1999" in license_text and
            "END OF TERMS AND CONDITIONS" in license_text and len(license_text) > 20000,
            f"package must include the full LGPL 2.1 license: {license_file}")
    notices = prefix / "THIRD-PARTY-NOTICES.txt"
    matching_file(notices, source / "THIRD-PARTY-NOTICES.txt")
    notice_text = notices.read_text(encoding="utf-8")
    require("Nuked" in notice_text and "LGPL" in notice_text,
            f"package notices must identify Nuked and its LGPL license: {notices}")
    if bundled_sdl:
        require((prefix / "share/licenses/restunts/SDL-LICENSE.txt").is_file(),
                f"missing bundled SDL license in {component} package")
    if component == "Runtime":
        for name in ("restunts", "repldump", "pixldump"):
            require((prefix / "bin" / name).is_file(), f"missing runtime executable: {name}")
        matching_file(prefix / "share/docs/restunts/sdl3.md", source / "docs/sdl3.md")
    else:
        require((prefix / "bin/test-sdl3-audio").is_file(),
                "Tests package is missing its audio regression executable")


def verify_dynamic_linkage(binary, *, needs_nuked):
    output = command(["readelf", "--dynamic", binary], env=dict(os.environ, LC_ALL="C"))
    needed = re.findall(r"\(NEEDED\).*?\[([^]]+)\]", output)
    require((LIBRARY_NAME in needed) == needs_nuked,
            f"unexpected Nuked dynamic dependency for {binary}: {needed}")
    search_paths = re.findall(r"\((?:RPATH|RUNPATH)\).*?\[([^]]*)\]", output)
    for entry in search_paths:
        for path in entry.split(":"):
            require(path.startswith(("$ORIGIN/", "${ORIGIN}/")),
                    f"non-relocatable library search path in {binary}: {path!r}")
    if needs_nuked:
        require(any("$ORIGIN/../lib" in entry.split(":") or
                    "${ORIGIN}/../lib" in entry.split(":") for entry in search_paths),
                f"missing package-relative library search path in {binary}")


def fixture_environment():
    environment = os.environ.copy()
    for name in list(environment):
        if name.startswith("LD_") or name == "RESTUNTS_AUDIO_TRACE":
            environment.pop(name)
    environment.update(SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
    return environment


def run_audio_fixture(binary, asset, working_directory):
    output = command([binary, asset], cwd=working_directory,
                     env=fixture_environment(), timeout=60)
    require("SDL3 AdLib:" in output and "passed" in output,
            "packaged audio fixture did not report successful completion")
    return output


def instrument_replacement(source):
    implementation = source / "opl2.c"
    text = implementation.read_bytes().decode("utf-8")
    newline = "\r\n" if "\r\n" in text else "\n"
    pattern = r"(void\s+OPL2_Reset\s*\([^)]*\)\s*\{)"
    text, count = re.subn(pattern,
                         lambda match: match.group(1) + newline +
                         f'    fputs("{MARKER}\\n", stderr);', text)
    require(count == 1, "cannot identify the shipped OPL2_Reset function for replacement test")
    text = ("/* Modified only for the temporary library replacement regression. */" + newline +
            "#include <stdio.h>" + newline + text)
    implementation.write_bytes(text.encode("utf-8"))


def test_package(build, cmake):
    require(sys.platform.startswith("linux"), "this package regression requires native Linux")
    build = build.resolve()
    cache = cache_values(build)
    source = Path(cache.get("CMAKE_HOME_DIRECTORY", str(ROOT))).resolve()
    require(cache.get("RESTUNTS_BUILD_TESTS") == "ON", "configure with RESTUNTS_BUILD_TESTS=ON")
    require(shutil.which("readelf") is not None, "readelf is required to inspect ELF dependencies")
    asset = source / "stunts/ADSKIDMS.VCE"
    require(asset.is_file(), f"missing audio fixture asset: {asset}")
    compiler = cache.get("CMAKE_C_COMPILER")
    require(compiler is not None, "existing build does not identify its C compiler")
    bundled_sdl = cache.get("RESTUNTS_SYSTEM_SDL", "OFF") == "OFF"
    with tempfile.TemporaryDirectory(prefix="restunts-nuked-package-") as temporary:
        directory = Path(temporary)
        relocated = directory / "relocated packages"
        relocated.mkdir()
        packages = {}
        for component in ("Runtime", "Tests"):
            original = directory / ("install-" + component.lower())
            command([cmake, "--install", build, "--prefix", original, "--component", component])
            prefix = relocated / component.lower()
            shutil.move(str(original), prefix)
            require(not original.exists(), "original installation path survived relocation")
            verify_contents(prefix, component, source, build, bundled_sdl)
            packages[component] = prefix
        runtime = packages["Runtime"]
        tests = packages["Tests"]
        for name in ("restunts", "pixldump"):
            verify_dynamic_linkage(runtime / "bin" / name, needs_nuked=True)
        verify_dynamic_linkage(runtime / "bin/repldump", needs_nuked=False)
        for prefix in (runtime, tests):
            verify_dynamic_linkage(prefix / "lib" / LIBRARY_NAME, needs_nuked=False)
        fixture = tests / "bin/test-sdl3-audio"
        verify_dynamic_linkage(fixture, needs_nuked=True)
        executable_hash = hashlib.sha256(fixture.read_bytes()).digest()
        working_directory = directory / "fixture"
        working_directory.mkdir()
        output = run_audio_fixture(fixture, asset, working_directory)
        require(MARKER not in output, "baseline package unexpectedly contains the replacement marker")
        print("Relocated Runtime and Tests packages contain matching source, licenses, and ELF linkage.",
              flush=True)
        print("Relocated audio fixture passes without library-path environment overrides.", flush=True)

        replacement_source = directory / "replacement-source"
        shutil.copytree(tests / SOURCE_DIRECTORY, replacement_source)
        instrument_replacement(replacement_source)
        replacement_build = directory / "replacement-build"
        build_type = cache.get("CMAKE_BUILD_TYPE") or "Release"
        build_configuration = build_type.upper()
        command([cmake, "-S", replacement_source, "-B", replacement_build,
                 f"-DCMAKE_BUILD_TYPE={build_type}", f"-DCMAKE_C_COMPILER={compiler}",
                 f"-DCMAKE_C_FLAGS={cache.get('CMAKE_C_FLAGS', '')}",
                 f"-DCMAKE_C_FLAGS_{build_configuration}=" +
                 cache.get(f"CMAKE_C_FLAGS_{build_configuration}", ""),
                 f"-DCMAKE_SHARED_LINKER_FLAGS={cache.get('CMAKE_SHARED_LINKER_FLAGS', '')}",
                 f"-DNUKED_OPL2_SSE2={cache.get('RESTUNTS_SSE2', 'ON')}"])
        command([cmake, "--build", replacement_build, "--parallel", "2"])
        replacement = replacement_build / LIBRARY_NAME
        require(replacement.is_file(), "shipped library sources did not build the replacement library")
        shutil.copy2(replacement, tests / "lib" / LIBRARY_NAME)
        output = run_audio_fixture(fixture, asset, working_directory)
        require(MARKER in output, "packaged executable did not load the user-rebuilt library")
        require(hashlib.sha256(fixture.read_bytes()).digest() == executable_hash,
                "audio executable changed during the shared-library replacement test")
        print("Replacement built from shipped sources loaded successfully without relinking the fixture.",
              flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-directory", type=Path, required=True,
                        help="existing native Linux CMake build with tests enabled")
    parser.add_argument("--cmake", default="cmake", help="CMake executable (default: cmake)")
    arguments = parser.parse_args()
    try:
        test_package(arguments.build_directory, arguments.cmake)
    except (OSError, ValueError, subprocess.TimeoutExpired) as error:
        print(f"Nuked package regression failed: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
