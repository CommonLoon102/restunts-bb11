#!/usr/bin/env python3
"""Load the complete DIA3/CSIL race in DOS and draw with SuperSight off/on/off.

Build with `make -C src/restunts test-race-memory`. This test links the full
normal game, including its menus, and uses actual dashboard, opponent, track,
and two full 320x200 VGA pages without a conventional-memory framebuffer. Run with --dosbox dosbox and
--dosbox dosbox-x to check both emulators. Its small test entry adds code,
so its memory budget is slightly stricter than the game.
"""

import argparse
import math
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import time


def main():
    repository = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--executable",
        type=Path,
        default=repository / "src/restunts/tests/build/watcom/release/RACEMEM.EXE",
    )
    parser.add_argument("--game-directory", type=Path, default=repository / "stunts")
    parser.add_argument("--car-directory", type=Path, default=repository / "tools/scripts/cars")
    parser.add_argument("--dosbox", default="dosbox")
    parser.add_argument("--timeout", type=float, default=45)
    args = parser.parse_args()
    if not args.executable.is_file():
        parser.error(f"DOS test executable is missing: {args.executable}")
    if not args.game_directory.is_dir() or not args.car_directory.is_dir():
        parser.error("Game and car directories must exist")
    if not math.isfinite(args.timeout) or args.timeout <= 0:
        parser.error("--timeout must be finite and positive")

    game_files = {path.name.upper(): path for path in args.game_directory.iterdir() if path.is_file()}
    if "DEFAULT.TRK" not in game_files:
        parser.error("The game directory must contain DEFAULT.TRK")

    required_cars = {
        f"{prefix}{car}.{suffix}"
        for car in ("DIA3", "CSIL")
        for prefix, suffix in (("CAR", "RES"), ("ST", "3SH"), ("STDA", "VSH"), ("STDB", "VSH"))
    }
    car_files = {path.name.upper(): path for path in args.car_directory.iterdir() if path.is_file()}
    missing = required_cars - car_files.keys()
    if missing:
        parser.error(f"Missing test car resources: {', '.join(sorted(missing))}")

    with tempfile.TemporaryDirectory(prefix="restunts-race-memory-") as temporary:
        directory = Path(temporary)
        # Copy writable files as well as resources: never alter the user's game configuration.
        runner_files = {"RESULT.TXT", "STATUS.TXT", "DONE.TXT", "DOSBOX.LOG"}
        for name, source in game_files.items():
            if name not in runner_files and source.suffix.lower() not in (".exe", ".com", ".bat"):
                shutil.copyfile(source, directory / name)
        for name in required_cars:
            shutil.copyfile(car_files[name], directory / name)
        shutil.copyfile(args.executable, directory / "RACEMEM.EXE")
        environment = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
        command = [
            args.dosbox,
            "-silent",
            "-conf",
            str(repository / "tools/scripts/dosbox.proc.conf"),
            "-c",
            f'mount c "{directory}"',
            "-c",
            "c:",
            "-c",
            "RACEMEM.EXE /nointro > RESULT.TXT",
            "-c",
            "if errorlevel 1 echo FAILED > STATUS.TXT",
            "-c",
            "echo DONE > DONE.TXT",
        ]
        with (directory / "dosbox.log").open("w") as log:
            process = subprocess.Popen(
                command, cwd=directory, env=environment, stdout=log, stderr=subprocess.STDOUT
            )
            try:
                deadline = time.monotonic() + args.timeout
                while not (directory / "DONE.TXT").exists():
                    if process.poll() is not None or time.monotonic() >= deadline:
                        break
                    time.sleep(0.1)
            finally:
                if process.poll() is None:
                    process.kill()
                process.wait()

        result_path = directory / "RESULT.TXT"
        result = result_path.read_text(errors="replace") if result_path.exists() else ""
        status_path = directory / "STATUS.TXT"
        status = status_path.read_text(errors="replace") if status_path.exists() else ""
        success = (
            (directory / "DONE.TXT").exists()
            and not status.strip()
            and result.strip().endswith("DIA3 / CSIL race memory check passed")
        )
        print(result.strip() or "Race memory test did not finish before DOSBox stopped.")
        if not success:
            print((directory / "dosbox.log").read_text(errors="replace"))
            return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
