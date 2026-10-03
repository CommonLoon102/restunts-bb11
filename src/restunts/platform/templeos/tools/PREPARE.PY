#!/usr/bin/env python3
"""Prepare HolyC sources, optionally with private uppercase game data."""

import argparse
from pathlib import Path
import shutil


PORT = Path(__file__).resolve().parent.parent
SOURCE_EXTENSIONS = {".HC", ".HH"}
DATA_EXTENSIONS = {
    ".RES", ".PRE", ".PVS", ".VSH", ".PES", ".ESH", ".3SH", ".P3S",
    ".TRK", ".RPL", ".HIG", ".CFG", ".DAT", ".DRV", ".VCE", ".SFX",
    ".KMS", ".FNT", ".PLB",
}


def prepare(data, destination):
    data = data.resolve(strict=True) if data is not None else None
    destination = destination.resolve()
    if data is not None and not data.is_dir():
        raise ValueError("--data must name the original game directory")
    if destination.exists():
        raise ValueError("Choose a new output directory; existing packages are never overwritten")
    if data is not None and (destination == data or data in destination.parents):
        raise ValueError("The output must be outside the original game directory")
    assets = {}
    for source in sorted(data.iterdir()) if data is not None else ():
        if source.is_file() and source.suffix.upper() in DATA_EXTENSIONS:
            name = source.name.upper()
            if name in assets:
                if source.read_bytes() != assets[name].read_bytes():
                    raise ValueError("Different files have the same uppercase name: " + name)
            else:
                assets[name] = source
    if data is not None and not any(name in assets for name in ("MISC.RES", "MISC.PRE")):
        raise ValueError("The data directory has no MISC.RES or MISC.PRE")
    code = [source for source in PORT.rglob("*")
            if source.is_file() and source.suffix in SOURCE_EXTENSIONS
            and not {"tools", "package"}.intersection(source.relative_to(PORT).parts)
            and source.name != "REPLAY_EXPECTED.HH"
            and not source.name.startswith("TEST_")]
    if not code:
        raise ValueError("No HolyC sources found; run TRANSLATE.PY first")
    (destination / "PORT").mkdir(parents=True)
    (destination / "DATA").mkdir()
    for source in code:
        target = destination / "PORT" / source.relative_to(PORT)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
    shutil.copyfile(PORT / "README.TXT", destination / "PORT" / "README.TXT")
    for name, source in assets.items():
        shutil.copyfile(source, destination / "DATA" / name)
    print(f"Prepared {len(code)} HolyC files and {len(assets)} game resources in {destination}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", type=Path,
                        help="Optional original game directory for a private local copy")
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    try:
        prepare(args.data, args.output)
    except (OSError, ValueError) as error:
        parser.exit(1, str(error) + "\n")


if __name__ == "__main__":
    main()
