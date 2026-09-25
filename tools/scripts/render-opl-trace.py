"""Render an SDL3 OPL register trace with the vendored Nuked-OPL2-Lite core.

Only Python's standard library and a host C compiler are required. The WAV
contains generated PCM: SDL queue-clear markers do not remove samples from it.
Recorded immediate and buffered writes retain their original timing semantics.
"""

import argparse
from dataclasses import dataclass
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import wave


ROOT = Path(__file__).resolve().parents[2]
OPL_CLOCK_HZ = 3579545
MIN_SAMPLE_RATE = 8000
MAX_SAMPLE_RATE = 384000
DEFAULT_MAX_SECONDS = 3600
PCM_SAMPLE_BYTES = 2
PCM_CHANNELS = 1
WAV_HEADER_OVERHEAD = 36
MAX_RIFF_SIZE = 0xFFFFFFFF
PCM_COPY_BUFFER_BYTES = 65536
TRACE_HEADER_FIELDS = 5
TRACE_WRITE_FIELDS = 5
MAX_WAV_FRAMES = (MAX_RIFF_SIZE - WAV_HEADER_OVERHEAD) // PCM_SAMPLE_BYTES
MAX_LINE_LENGTH = 4096


@dataclass
class TraceInfo:
    clock: int
    sample_rate: int
    frames: int = 0
    writes: int = 0
    buffered_writes: int = 0
    queue_clears: int = 0


def number(token, base, description):
    pattern = r"[0-9]+" if base == 10 else r"[0-9a-fA-F]{1,2}"
    if not re.fullmatch(pattern, token):
        raise ValueError(f"invalid {description}: {token!r}")
    return int(token, base)


def normalize_trace(source, destination, max_seconds=DEFAULT_MAX_SECONDS):
    """Validate incrementally and write a bounded, decimal event stream."""
    if max_seconds <= 0:
        raise ValueError("maximum duration must be positive")
    info = None
    ended = False
    previous_frame = 0
    lines = iter(lambda: source.readline(MAX_LINE_LENGTH + 1), "")
    for line_number, raw in enumerate(lines, 1):
        if len(raw) > MAX_LINE_LENGTH:
            raise ValueError(f"line {line_number}: line is too long")
        fields = raw.split()
        if not fields or fields[0].startswith("#"):
            continue
        try:
            if info is None:
                if len(fields) != TRACE_HEADER_FIELDS or fields[:2] != ["RESTUNTS_OPL_TRACE", "1"]:
                    raise ValueError("expected RESTUNTS_OPL_TRACE 1 backend clock sample_rate")
                if fields[2] != "nuked":
                    raise ValueError(f"unsupported recorded emulator: {fields[2]}")
                clock = number(fields[3], 10, "clock")
                sample_rate = number(fields[4], 10, "sample rate")
                if clock != OPL_CLOCK_HZ:
                    raise ValueError("the vendored Nuked core supports only the 3579545 Hz clock")
                if not MIN_SAMPLE_RATE <= sample_rate <= MAX_SAMPLE_RATE:
                    raise ValueError("sample rate must be between 8000 and 384000 Hz")
                info = TraceInfo(clock, sample_rate)
                continue
            if ended:
                raise ValueError("event follows the end marker")
            if len(fields) < 2:
                raise ValueError("incomplete event")
            frame = number(fields[0], 10, "sample frame")
            if frame < previous_frame:
                raise ValueError("sample frames must not decrease")
            if frame > min(MAX_WAV_FRAMES, info.sample_rate * max_seconds):
                raise ValueError("sample frame exceeds the duration or WAV size limit")
            kind = fields[1]
            if kind == "W" and len(fields) == TRACE_WRITE_FIELDS:
                register = number(fields[2], 16, "register")
                value = number(fields[3], 16, "register value")
                if fields[4] not in ("0", "1"):
                    raise ValueError("buffered flag must be 0 or 1")
                buffered = int(fields[4])
                destination.write(f"{frame} W {register} {value} {buffered}\n")
                info.writes += 1
                info.buffered_writes += buffered
            elif kind == "C" and len(fields) == 2:
                destination.write(f"{frame} C\n")
                info.queue_clears += 1
            elif kind == "E" and len(fields) == 2:
                destination.write(f"{frame} E\n")
                info.frames = frame
                ended = True
            else:
                raise ValueError("expected a W, C, or E event with the correct fields")
            previous_frame = frame
        except ValueError as error:
            raise ValueError(f"line {line_number}: {error}") from error
    if info is None:
        raise ValueError("trace has no header")
    if not ended:
        raise ValueError("trace has no final E marker; finish the capture before rendering")
    return info


HELPER = r'''
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "opl2.h"
#include "src/restunts/c/legacy.h"
#define SAMPLE_BUFFER_FRAMES 4096U
static opl2_chip *chip;
static legacy_s32 initialize(uint32_t clock, uint32_t rate)
{
    if (clock != OPL_CLOCK_HZ) { return 0; }
    chip = calloc(1, sizeof(*chip));
    if (!chip) { return 0; }
    OPL2_Reset(chip, rate);
    return 1;
}
static int16_t next_sample(void)
{
    int16_t sample;
    OPL2_GenerateResampled(chip, &sample);
    return sample;
}
static void write_register(legacy_uint reg, legacy_uint value, legacy_uint buffered)
{
    if (buffered) {
        OPL2_WriteRegBuffered(chip, (uint8_t)reg, (uint8_t)value);
    } else {
        OPL2_WriteReg(chip, (uint8_t)reg, (uint8_t)value);
    }
}
static void destroy(void) { free(chip); }

static legacy_s32 generate(FILE *output, uint64_t count)
{
    legacy_u8 bytes[SAMPLE_BUFFER_FRAMES * PCM_SAMPLE_BYTES];
    while (count != 0) {
        size_t frames = count > SAMPLE_BUFFER_FRAMES ? SAMPLE_BUFFER_FRAMES : (size_t)count;
        for (size_t index = 0; index < frames; ++index) {
            uint16_t sample = (uint16_t)next_sample();
            bytes[index * PCM_SAMPLE_BYTES] = (legacy_u8)sample;
            bytes[index * PCM_SAMPLE_BYTES + 1] = (legacy_u8)(sample >> CHAR_BIT);
        }
        if (fwrite(bytes, PCM_SAMPLE_BYTES, frames, output) != frames) {
            return 0;
        }
        count -= frames;
    }
    return 1;
}

legacy_int main(legacy_int argc, legacy_char **argv)
{
    if (argc != 5) {
        return 2;
    }
    FILE *input = fopen(argv[3], "r");
    FILE *output = fopen(argv[4], "wb");
    if (!input || !output || !initialize((uint32_t)strtoul(argv[1], NULL, 10),
                                        (uint32_t)strtoul(argv[2], NULL, 10))) {
        fputs("Cannot initialize trace renderer\n", stderr);
        return 2;
    }
    uint64_t current = 0;
    uint64_t frame;
    legacy_char event;
    legacy_s32 ended = 0;
    while (fscanf(input, "%" SCNu64 " %c", &frame, &event) == 2) {
        if (frame < current || !generate(output, frame - current)) {
            break;
        }
        current = frame;
        if (event == 'W') {
            legacy_uint reg, value, buffered;
            if (fscanf(input, "%u %u %u", &reg, &value, &buffered) != 3 ||
                reg > UINT8_MAX || value > UINT8_MAX || buffered > 1) {
                break;
            }
            write_register(reg, value, buffered);
        } else if (event == 'E') {
            ended = 1;
            break;
        } else if (event != 'C') {
            break;
        }
    }
    legacy_s32 failed = !ended || ferror(input) || ferror(output);
    failed |= fclose(input) != 0;
    failed |= fclose(output) != 0;
    destroy();
    if (failed) {
        fputs("Cannot finish trace rendering\n", stderr);
    }
    return failed ? 2 : 0;
}
'''


def compile_helper(directory, cc="cc"):
    library = ROOT / "third_party/nuked-opl2-lite"
    source = directory / "render.c"
    executable = directory / ("render.exe" if os.name == "nt" else "render")
    source.write_text(f"#define OPL_CLOCK_HZ {OPL_CLOCK_HZ}U\n"
                      f"#define PCM_SAMPLE_BYTES {PCM_SAMPLE_BYTES}U\n" + HELPER, encoding="utf-8")
    command = [cc, "-std=c99", "-O2", "-I", str(library), "-I", str(ROOT), str(source),
               str(library / "opl2.c"), "-o", str(executable)]
    subprocess.run(command, check=True)
    return executable


def synthesize(executable, events, raw_pcm, info):
    subprocess.run([str(executable), str(info.clock), str(info.sample_rate),
                    str(events), str(raw_pcm)], check=True)
    if raw_pcm.stat().st_size != info.frames * PCM_SAMPLE_BYTES:
        raise ValueError("renderer returned the wrong sample count")


def write_wave(raw_pcm, output, info):
    """Replace the requested output only after the complete WAV is written."""
    descriptor, temporary = tempfile.mkstemp(prefix=f".{output.name}.",
                                            suffix=".tmp", dir=output.parent)
    os.close(descriptor)
    try:
        with wave.open(temporary, "wb") as destination, raw_pcm.open("rb") as source:
            destination.setparams((PCM_CHANNELS, PCM_SAMPLE_BYTES, info.sample_rate, info.frames, "NONE", "not compressed"))
            while block := source.read(PCM_COPY_BUFFER_BYTES):
                destination.writeframesraw(block)
        os.replace(temporary, output)
    finally:
        if os.path.exists(temporary):
            os.unlink(temporary)


def render(trace, output, cc="cc", max_seconds=DEFAULT_MAX_SECONDS):
    trace = Path(trace)
    output = Path(output)
    if trace.resolve() == output.resolve():
        raise ValueError("output must not overwrite the input trace")
    with tempfile.TemporaryDirectory(prefix="restunts-opl-render-") as temporary:
        directory = Path(temporary)
        events = directory / "events.txt"
        with trace.open("r", encoding="ascii") as source, events.open("w", encoding="ascii") as normalized:
            info = normalize_trace(source, normalized, max_seconds)
        executable = compile_helper(directory, cc)
        raw_pcm = directory / "render.pcm"
        synthesize(executable, events, raw_pcm, info)
        write_wave(raw_pcm, output, info)
    return info


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("trace", type=Path, help="completed RESTUNTS_OPL_TRACE file")
    parser.add_argument("output", type=Path, help="output mono 16-bit WAV")
    parser.add_argument("--cc", default="cc", help="host C compiler executable (default: cc)")
    parser.add_argument("--max-seconds", type=int, default=DEFAULT_MAX_SECONDS,
                        help="maximum trace duration to render (default: 3600 seconds)")
    arguments = parser.parse_args()
    try:
        info = render(arguments.trace, arguments.output, arguments.cc, arguments.max_seconds)
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        print(f"Cannot render OPL trace: {error}", file=sys.stderr)
        return 1
    print(f"Rendered {info.frames} frames ({info.frames / info.sample_rate:.3f}s) "
          f"at {info.sample_rate} Hz with Nuked-OPL2-Lite: {arguments.output}")
    if info.queue_clears:
        print(f"Note: retained samples across {info.queue_clears} SDL queue-clear markers; "
              "this WAV contains synthesized PCM, not reconstructed device playback.", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
