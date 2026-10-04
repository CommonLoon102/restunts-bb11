#!/usr/bin/env python3
"""Check that HolyC spacing and identifier changes preserve language tokens."""
from pathlib import Path
import runpy
import unittest

STYLE = runpy.run_path(str(Path(__file__).with_name('STYLE.PY')))


class StyleTests(unittest.TestCase):
    def test_literals_and_comments_keep_their_contents(self):
        source = 'old_name("old_name\\\"",\'old_name\'); // old_name\n/* old_name */'
        expected = 'NewName("old_name\\\"",\'old_name\'); // old_name\n/* old_name */'
        self.assertEqual(STYLE['rename_identifiers'](source, {'old_name': 'NewName'}), expected)

    def test_compaction_does_not_create_operators_or_comments(self):
        source = 'a + +b; a - --b; a / *ptr; a / / b; a < < b; a & & b;'
        result = STYLE['compact_spacing'](source)
        tokens = lambda text: [token[0] for token in STYLE['TOKEN'].finditer(text)]
        self.assertEqual(tokens(source), tokens(result))
        self.assertIn('/ *', result)
        self.assertIn('/ /', result)

    def test_compact_arguments_and_pointer_declarations(self):
        source = 'legacy_u8 *ptr = Fun(a + b, c);\n'
        self.assertEqual(STYLE['compact_spacing'](source), 'legacy_u8 *ptr=Fun(a+b,c);\n')

    def test_macro_replacement_does_not_become_a_function_macro(self):
        source = '#define VALUE (first + second)\n'
        self.assertEqual(STYLE['compact_spacing'](source), '#define VALUE (first+second)\n')
        source = '#define NEGATIVE -1\n#define POINTER *value\n'
        self.assertEqual(STYLE['compact_spacing'](source), source)

    def test_continuation_indentation_preserves_literal_and_comment_whitespace(self):
        source = '  (value=Fun(\n       other));\n  "first\n   second";\n/* first\n * second */'
        expected = '  (value=Fun(\n      other));\n  "first\n   second";\n/* first\n * second */'
        self.assertEqual(STYLE['compact_spacing'](source), expected)

    def test_function_names_do_not_shadow_templeos_builtins(self):
        self.assertEqual(STYLE['function_name']('free'), 'RsFree')
        self.assertEqual(STYLE['function_name']('rs_timing_timer_reset'), 'RsTimingTimerReset')
        self.assertEqual(STYLE['function_name']('TempleTimerPump'), 'TempleTimerPump')
        self.assertNotEqual(STYLE['function_name']('_memcpy'), STYLE['function_name']('memcpy'))

    def test_classes_and_variables(self):
        self.assertEqual(STYLE['class_name']('CARSTATE'), 'CCarState')
        self.assertEqual(STYLE['class_name']('POINT2D'), 'CPoint2D')
        self.assertEqual(STYLE['class_name']('TempleRasterSurface'), 'CTempleRasterSurface')
        self.assertEqual(STYLE['class_name']('CTempleRasterSurface'), 'CTempleRasterSurface')
        self.assertEqual(STYLE['snake_case']('followOpponentFlag'), 'follow_opponent_flag')


if __name__ == '__main__':
    unittest.main()
