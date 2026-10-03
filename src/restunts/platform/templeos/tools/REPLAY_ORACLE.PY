#!/usr/bin/env python3
"""Generate the HolyC regression header from an independently recorded DOS dump."""

import argparse
import hashlib
from pathlib import Path


HEADER_BYTES = 2
STATE_BYTES = 1120
DEFAULT_TEST_FRAMES = 120
HASHES_PER_LINE = 8
FNV_OFFSET = 2166136261
FNV_PRIME = 16777619
U32_MASK = 0xFFFFFFFF


def fingerprint(data):
    value = FNV_OFFSET
    for byte in data:
        value = ((value ^ byte) * FNV_PRIME) & U32_MASK
    return value


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dump", type=Path, required=True)
    parser.add_argument("--replay", type=Path, required=True)
    parser.add_argument("--executable", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--frames", type=int, default=DEFAULT_TEST_FRAMES)
    args = parser.parse_args()
    dump = args.dump.read_bytes()
    replay = args.replay.read_bytes()
    executable = args.executable.read_bytes()
    count = int.from_bytes(dump[:HEADER_BYTES], "little")
    if len(dump) != HEADER_BYTES + count * STATE_BYTES:
        parser.error("DOS dump does not contain its declared canonical state count")
    if args.frames < 1 or count < args.frames:
        parser.error("DOS dump has fewer states than the requested positive frame count")
    lines = [
        "// Archived Borland REPLDUMO.EXE, DEFAULT.RPL, DOSBox dynamic/max.",
        "// Oracle executable SHA256 " + hashlib.sha256(executable).hexdigest(),
        "// Replay SHA256 " + hashlib.sha256(replay).hexdigest(),
        f"// Full {count}-frame dump SHA256 " + hashlib.sha256(dump).hexdigest(),
        f"#define TEMPLE_REPLAY_TEST_FRAMES {args.frames}",
        f"#define TEMPLE_REPLAY_STATE_BYTES {STATE_BYTES}",
        f"#define TEMPLE_REPLAY_INPUT_BYTES {len(replay)}",
        f"#define TEMPLE_REPLAY_INPUT_HASH 0x{fingerprint(replay):08X}",
        "legacy_u32 temple_replay_expected[TEMPLE_REPLAY_TEST_FRAMES] = {",
    ]
    hashes = []
    for frame in range(args.frames):
        offset = HEADER_BYTES + frame * STATE_BYTES
        hashes.append(f"0x{fingerprint(dump[offset:offset + STATE_BYTES]):08X}")
    for start in range(0, len(hashes), HASHES_PER_LINE):
        suffix = "," if start + HASHES_PER_LINE < len(hashes) else "};"
        lines.append("\t" + ", ".join(hashes[start:start + HASHES_PER_LINE]) + suffix)
    lines.append("")
    args.output.write_bytes("\r\n".join(lines).encode("ascii"))


if __name__ == "__main__":
    main()
