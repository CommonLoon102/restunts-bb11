#!/usr/bin/env python3
"""Check that the guest runner rejects compiler errors and warnings."""
from pathlib import Path
import runpy
import unittest

VM = runpy.run_path(str(Path(__file__).with_name("VM.PY")))


class VmDiagnosticTests(unittest.TestCase):
    def test_doldoc_warning_is_rejected_after_returning_normally(self):
        diagnostic = (
            "RESTUNTS_VM_PASS\n"
            "$WW+H,1$OptPass3 COCCompile PrsFun PrsGlblVarLst "
            "$FG,4$$BK,1$WARNING:$FG$$BK,0$ Using 64-bit reg var.\n"
            "RESTUNTS_VM_DONE\n"
        )
        self.assertEqual(VM["compiler_diagnostic_level"](diagnostic), "warning")

    def test_error_is_rejected_without_an_exception(self):
        diagnostic = "RESTUNTS_VM_PASS\n$FG,4$ERROR:$FG$ File not found.\nRESTUNTS_VM_DONE\n"
        self.assertEqual(VM["compiler_diagnostic_level"](diagnostic), "error")

    def test_plain_diagnostics_are_case_insensitive(self):
        for level in ("Error", "Warning"):
            with self.subTest(level=level):
                self.assertEqual(VM["compiler_diagnostic_level"](level + " : message"), level.lower())

    def test_ordinary_output_and_identifiers_are_accepted(self):
        diagnostic = (
            "RESTUNTS_VM_PASS\n"
            "COMPILER_WARNING: 0\nERROR_COUNT: 0\n"
            "Warnings: 0\nError reporting checks passed.\n"
            "RESTUNTS_VM_DONE\n"
        )
        self.assertIsNone(VM["compiler_diagnostic_level"](diagnostic))


if __name__ == "__main__":
    unittest.main()
