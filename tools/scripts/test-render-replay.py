#!/usr/bin/env python3
"""Verify interpolated rendering preserves every replay tick and RNG seed."""

import argparse
import os
from pathlib import Path
import subprocess
import tempfile


GAMESTATE_SIZE = 1120
RANDOM_SEED_SIZE = 6
RECORD_SIZE = GAMESTATE_SIZE + RANDOM_SEED_SIZE
REPLAY_TIMEOUT_SECONDS = 120
FULL_REPLAY_TIMEOUT_SECONDS = 300
BACKEND_TOGGLE_MODE = 4
TOGGLE_DEFAULT_LIMIT = 60
TOGGLE_HARDLAND_LIMIT = 330


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", type=Path)
    parser.add_argument("data_directory", type=Path)
    scope = parser.add_mutually_exclusive_group()
    scope.add_argument("--full", action="store_true")
    scope.add_argument("--toggle-backends-only", action="store_true",
                       help="compare CPU classic with repeated F10 changes on two bounded replays")
    parser.add_argument("--backend", choices=("cpu", "vulkan"), default="cpu",
                        help="enhanced backend to require; classic baseline always uses CPU")
    options = parser.parse_args()
    if options.toggle_backends_only and options.backend != "vulkan":
        parser.error("--toggle-backends-only requires --backend vulkan")
    executable, data_directory = options.executable, options.data_directory
    full_replays = options.full
    default_timeout = FULL_REPLAY_TIMEOUT_SECONDS if full_replays else REPLAY_TIMEOUT_SECONDS
    timeout_seconds = int(os.environ.get("RESTUNTS_RENDER_REPLAY_TIMEOUT_SECONDS", default_timeout))
    if timeout_seconds <= 0:
        raise ValueError("RESTUNTS_RENDER_REPLAY_TIMEOUT_SECONDS must be a positive integer")
    executable = executable.resolve()
    data_directory = data_directory.resolve()
    video_driver = os.environ.get("SDL_VIDEODRIVER",
                                  "offscreen" if options.backend == "vulkan" else "dummy")
    environment = dict(os.environ, SDL_VIDEODRIVER=video_driver, SDL_AUDIODRIVER="dummy",
                       RESTUNTS_AUDIO_TRACE="")
    available = {path.stem.upper(): path.stem for path in data_directory.iterdir()
                 if path.suffix.upper() == ".RPL"}
    # Cover 10 Hz, 20 Hz collisions/debris, both cars, and the hard landing at
    # 16.00-16.50 seconds, plus the loop exit after 40 seconds in SHAKING.
    # Check bounded interpolation through the HARDLAND landing and SHAKING loop exit.
    fixtures = [(available["DEFAULT"], 240, 0, 0)]
    for replay, limit, first, last in [("DEFCRSH", 0, 0, 0), ("0A0A", 240, 0, 0),
                                      ("HARDLAND", 0, 320, 330), ("SHAKING", 0, 0, 0)]:
        if replay in available:
            fixtures.append((available[replay], limit, first, last))
    if full_replays:
        fixtures = [(replay, 0, first, last) for replay, _, first, last in fixtures]
    if options.toggle_backends_only:
        fixtures = [(available["DEFAULT"], TOGGLE_DEFAULT_LIMIT, 0, 0)]
        if "HARDLAND" in available:
            fixtures.append((available["HARDLAND"], TOGGLE_HARDLAND_LIMIT, 320, 330))
    modes = (0, BACKEND_TOGGLE_MODE) if options.toggle_backends_only else range(4)
    with tempfile.TemporaryDirectory(prefix="restunts-render-replay-") as directory:
        for replay, limit, first, last in fixtures:
            settling = ("800", "900") if replay.upper() == "SHAKING" else ("0", "0")
            baseline = None
            for mode in modes:
                output = Path(directory) / f"{replay}-{mode}.bin"
                subprocess.run([str(executable), "--data-dir", str(data_directory),
                                replay, str(output), str(mode), str(limit),
                                str(first), str(last), *settling,
                                "--backend", "cpu" if mode == 0 else options.backend],
                               env=environment, check=True,
                               timeout=timeout_seconds)
                actual = output.read_bytes()
                assert actual and len(actual) % RECORD_SIZE == 0
                if baseline is None:
                    baseline = actual
                elif actual != baseline:
                    offset = next((index for index, pair in enumerate(zip(actual, baseline))
                                   if pair[0] != pair[1]), min(len(actual), len(baseline)))
                    field_offset = offset % RECORD_SIZE
                    field = "gamestate" if field_offset < GAMESTATE_SIZE else "RNG seed"
                    if field == "RNG seed":
                        field_offset -= GAMESTATE_SIZE
                    raise AssertionError(f"{replay}, mode {mode}: {field} differs at "
                                         f"tick {offset // RECORD_SIZE}, byte {field_offset}")
            detail = ("with classic rendering and repeated F10 CPU/Vulkan transitions"
                      if options.toggle_backends_only else
                      f"with rendering off, {options.backend} on, toggled and adaptive quality swept")
            print(f"{replay}: {len(baseline) // RECORD_SIZE} identical gamestates and RNG seeds "
                  f"{detail}", flush=True)


if __name__ == "__main__":
    main()
