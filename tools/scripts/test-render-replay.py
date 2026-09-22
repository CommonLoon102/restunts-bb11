#!/usr/bin/env python3
"""Compare every serialized replay tick across renderer toggle histories."""

import os
from pathlib import Path
import subprocess
import sys
import tempfile


def main():
    executable, data_directory = map(Path, sys.argv[1:3])
    full_replays = "--full" in sys.argv[3:]
    executable = executable.resolve()
    data_directory = data_directory.resolve()
    environment = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy",
                       RESTUNTS_AUDIO_TRACE="")
    # The normal intro replay runs at 10 Hz; the short crash replay exercises
    # 20 Hz, collisions and debris. The opponent fixture covers both targets.
    fixtures = [("DEFAULT", 240)]
    for replay, limit in [("DEFCRSH", 0), ("0A0A", 240)]:
        if (data_directory / f"{replay}.RPL").exists():
            fixtures.append((replay, limit))
    if full_replays:
        fixtures = [(replay, 0) for replay, _ in fixtures]
    with tempfile.TemporaryDirectory(prefix="restunts-render-replay-") as directory:
        for replay, limit in fixtures:
            baseline = None
            for mode in range(3):
                output = Path(directory) / f"{replay}-{mode}.bin"
                subprocess.run([str(executable), "--data-dir", str(data_directory),
                                replay, str(output), str(mode), str(limit)],
                               env=environment, check=True, timeout=300 if full_replays else 120)
                actual = output.read_bytes()
                assert actual and len(actual) % 1120 == 0
                if baseline is None:
                    baseline = actual
                elif actual != baseline:
                    offset = next((index for index, pair in enumerate(zip(actual, baseline))
                                   if pair[0] != pair[1]), min(len(actual), len(baseline)))
                    raise AssertionError(f"{replay}, mode {mode}: gamestate differs at "
                                         f"tick {offset // 1120}, byte {offset % 1120}")
            print(f"{replay}: {len(baseline) // 1120} identical gamestates with "
                  "F12 off, on and repeatedly toggled", flush=True)


if __name__ == "__main__":
    main()
