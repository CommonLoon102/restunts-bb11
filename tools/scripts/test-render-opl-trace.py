"""Validate trace rejection, frame timing, and real Nuked OPL synthesis."""

import importlib.util
import io
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
import wave


SCRIPT = Path(__file__).with_name("render-opl-trace.py")
SPEC = importlib.util.spec_from_file_location("opl_trace_renderer", SCRIPT)
renderer = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = renderer
sys.dont_write_bytecode = True
SPEC.loader.exec_module(renderer)

HEADER = "RESTUNTS_OPL_TRACE 1 nuked 3579545 44100\n"


def sine_trace(start=0, end=48510, retrigger=None):
    registers = [(0x01, 0x20), (0x20, 0x21), (0x23, 0x21),
                 (0x40, 0x3F), (0x43, 0x00), (0x60, 0x00), (0x63, 0xF0),
                 (0x80, 0x00), (0x83, 0x0F), (0xC0, 0x00),
                 (0xA0, 0x44), (0xB0, 0x32)]
    lines = [HEADER.rstrip()]
    lines += [f"{start} W {reg:02X} {value:02X} 0" for reg, value in registers]
    if retrigger is not None:
        lines += [f"800 W B0 12 {retrigger}", f"800 W B0 32 {retrigger}"]
    lines.append(f"{end} E")
    return "\n".join(lines) + "\n"


class TraceParsingTests(unittest.TestCase):
    def normalize(self, text, max_seconds=3600):
        output = io.StringIO()
        info = renderer.normalize_trace(io.StringIO(text), output, max_seconds)
        return info, output.getvalue()

    def test_same_frame_order_buffer_flags_and_queue_clear_are_preserved(self):
        info, output = self.normalize(HEADER + "0 W B0 12 0\n0 W b0 32 1\n"
                                      "44 C\n441 E\n")
        self.assertEqual("0 W 176 18 0\n0 W 176 50 1\n44 C\n441 E\n", output)
        self.assertEqual((441, 2, 1, 1),
                         (info.frames, info.writes, info.buffered_writes, info.queue_clears))

    def test_malformed_and_incomplete_captures_are_rejected(self):
        invalid = ["", HEADER, HEADER + "0 W A0 10 0\n",
                   HEADER + "10 C\n9 E\n", HEADER + "0 E\n0 W 00 00 0\n",
                   HEADER + "0 W 100 00 0\n0 E\n", HEADER + "0 W 20 FF 2\n0 E\n",
                   HEADER + "0 W -1 00 0\n0 E\n", HEADER + "0 W 20 00\n0 E\n",
                   HEADER + "0 Q\n0 E\n", HEADER + "0 E extra\n",
                   HEADER.replace(" 1 ", " 2 ", 1) + "0 E\n",
                   HEADER.replace("nuked", "unknown") + "0 E\n",
                   HEADER.replace("44100", "0") + "0 E\n",
                   HEADER.replace("3579545", "0") + "0 E\n",
                   HEADER + "9" * 4097 + " E\n"]
        for text in invalid:
            with self.subTest(trace=text[:100]), self.assertRaises(ValueError):
                self.normalize(text)

    def test_duration_and_riff_limits_prevent_unbounded_rendering(self):
        with self.assertRaisesRegex(ValueError, "duration or WAV size"):
            self.normalize(HEADER + "44101 E\n", max_seconds=1)
        with self.assertRaisesRegex(ValueError, "duration or WAV size"):
            self.normalize(HEADER + f"{renderer.MAX_WAV_FRAMES + 1} E\n", max_seconds=10**9)
        with self.assertRaisesRegex(ValueError, "must be positive"):
            self.normalize(HEADER + "0 E\n", max_seconds=0)

    def test_invalid_cli_input_preserves_existing_output(self):
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            trace = directory / "invalid.trace"
            output = directory / "existing.wav"
            trace.write_text(HEADER + "0 W 20 20 0\n", encoding="ascii")
            output.write_bytes(b"keep this file")
            result = subprocess.run([sys.executable, str(SCRIPT), str(trace), str(output)],
                                    capture_output=True, text=True)
            self.assertNotEqual(0, result.returncode)
            self.assertIn("final E marker", result.stderr)
            self.assertEqual(b"keep this file", output.read_bytes())

    def test_nuked_clock_and_input_overwrite_are_rejected_before_compilation(self):
        with tempfile.TemporaryDirectory() as temporary:
            trace = Path(temporary) / "input.trace"
            trace.write_text(HEADER.replace("3579545", "4000000") + "0 E\n", encoding="ascii")
            with self.assertRaisesRegex(ValueError, "3579545"):
                renderer.render(trace, trace.with_suffix(".wav"), cc="unused")
            with self.assertRaisesRegex(ValueError, "overwrite"):
                renderer.render(trace, trace, cc="unused")


class SynthesizerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = os.environ.get("CC", "cc")
        if not shutil.which(compiler):
            raise unittest.SkipTest("real OPL synthesis tests require a host C compiler")
        cls.temporary = tempfile.TemporaryDirectory(prefix="restunts-trace-tests-")
        cls.addClassCleanup(cls.temporary.cleanup)
        cls.directory = Path(cls.temporary.name)
        cls.helper = renderer.compile_helper(cls.directory, compiler)

    def synthesize(self, text):
        with tempfile.TemporaryDirectory(dir=self.directory) as temporary:
            directory = Path(temporary)
            events = directory / "events.txt"
            with events.open("w", encoding="ascii") as output:
                info = renderer.normalize_trace(io.StringIO(text), output)
            raw_pcm = directory / "samples.pcm"
            renderer.synthesize(self.helper, events, raw_pcm, info)
            wave_path = directory / "samples.wav"
            renderer.write_wave(raw_pcm, wave_path, info)
            with wave.open(str(wave_path), "rb") as recorded:
                self.assertEqual((1, 2, 44100, info.frames),
                                 (recorded.getnchannels(), recorded.getsampwidth(),
                                  recorded.getframerate(), recorded.getnframes()))
                pcm = recorded.readframes(info.frames)
            self.assertEqual(raw_pcm.read_bytes(), pcm)
            return pcm

    def test_nuked_produces_audible_440hz_at_the_recorded_rate(self):
        pcm = self.synthesize(sine_trace())
        samples = [value[0] for value in struct.iter_unpack("<h", pcm)][4410:]
        crossings = sum(first <= 0 < second for first, second in zip(samples, samples[1:]))
        self.assertGreater(sum(value * value for value in samples), 1000000)
        self.assertGreaterEqual(crossings, 438)
        self.assertLessEqual(crossings, 442)

    def test_register_events_happen_at_the_exact_recorded_output_frame(self):
        pcm = self.synthesize(sine_trace(start=4410, end=8820))
        self.assertEqual(bytes(4410 * 2), pcm[:4410 * 2])
        self.assertNotEqual(bytes(4410 * 2), pcm[4410 * 2:])

    def test_nuked_buffer_flag_preserves_same_frame_note_retrigger(self):
        uninterrupted = self.synthesize(sine_trace(end=3000))
        immediate = self.synthesize(sine_trace(end=3000, retrigger=0))
        buffered = self.synthesize(sine_trace(end=3000, retrigger=1))
        self.assertEqual(uninterrupted, immediate)
        self.assertEqual(uninterrupted[:800 * 2], buffered[:800 * 2])
        self.assertNotEqual(uninterrupted[800 * 2:], buffered[800 * 2:])

    def test_queue_clear_does_not_remove_generated_samples(self):
        original = sine_trace(end=3000)
        with_clear = original.replace("3000 E", "1500 C\n3000 E")
        self.assertEqual(self.synthesize(original),
                         self.synthesize(with_clear))


if __name__ == "__main__":
    unittest.main()
