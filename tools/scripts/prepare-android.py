#!/usr/bin/env python3
"""Prepare Android's Java/native SDL glue from the same verified source as CMake."""
import hashlib
import io
from pathlib import Path
import re
import shutil
import tarfile
import urllib.request


def dependency(root, name, configuration, directory):
    cmake = (root / configuration).read_text()
    pin = re.search(r"FetchContent_Declare\(" + name + r"\s+URL (\S+)\s+URL_HASH SHA256=(\w+)", cmake)
    if pin is None:
        raise RuntimeError("Cannot locate the pinned " + name + " source and checksum")
    url, checksum = pin.groups()
    destination = root / "out/android-deps" / directory
    marker = destination / ".restunts-checksum"
    if not marker.exists() or marker.read_text().strip() != checksum:
        with urllib.request.urlopen(url, timeout=120) as response:
            data = response.read()
        if hashlib.sha256(data).hexdigest() != checksum:
            raise RuntimeError(name + " source checksum mismatch")
        temporary = destination.with_name(directory + "-importing")
        if temporary.exists():
            shutil.rmtree(temporary)
        temporary.mkdir(parents=True)
        with tarfile.open(fileobj=io.BytesIO(data), mode="r:gz") as archive:
            archive.extractall(temporary, filter="data")
        extracted, = temporary.iterdir()
        if destination.exists():
            shutil.rmtree(destination)
        extracted.rename(destination)
        temporary.rmdir()
        marker.write_text(checksum + "\n")
    return destination


def prepare(root):
    destination = dependency(root, "SDL3", "CMakeLists.txt", "sdl")
    vpx = dependency(root, "restunts_vpx", "cmake/webm.cmake", "vpx")
    nestegg = dependency(root, "restunts_nestegg", "cmake/webm.cmake", "nestegg")
    # Upstream wrapper belongs with upstream Java glue; keep generated files in out/.
    wrapper = root / "out/android-gradle"
    wrapper.mkdir(parents=True, exist_ok=True)
    upstream = destination / "android-project"
    shutil.copy2(upstream / "gradlew", wrapper / "gradlew")
    (wrapper / "gradlew").chmod(0o755)
    shutil.copytree(upstream / "gradle", wrapper / "gradle", dirs_exist_ok=True)
    notices = root / "android/generated-assets/licenses"
    notices.mkdir(parents=True, exist_ok=True)
    sources = [root / "THIRD-PARTY-NOTICES.txt", destination / "LICENSE.txt",
               root / "third_party/nuked-opl2-lite/LICENSE", root / "third_party/stb/LICENSE"]
    for source, name in zip(sources, ["THIRD-PARTY-NOTICES.txt", "SDL-LICENSE.txt",
                                   "Nuked-OPL2-LICENSE.txt", "stb-LICENSE.txt"]):
        shutil.copy2(source, notices / name)
    for source, name in [(vpx / "LICENSE", "libvpx-LICENSE.txt"),
                         (vpx / "PATENTS", "libvpx-PATENTS.txt"),
                         (nestegg / "LICENSE", "nestegg-LICENSE.txt")]:
        shutil.copy2(source, notices / name)


if __name__ == "__main__":
    prepare(Path(__file__).resolve().parents[2])
