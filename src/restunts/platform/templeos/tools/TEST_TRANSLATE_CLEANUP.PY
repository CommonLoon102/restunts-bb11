#!/usr/bin/env python3
"""Check generated constant dependencies and native function replacements."""
import importlib.machinery
import importlib.util
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

HERE = Path(__file__).resolve().parent
LOADER = importlib.machinery.SourceFileLoader("temple_translate_cleanup", str(HERE / "TRANSLATE.PY"))
SPEC = importlib.util.spec_from_loader(LOADER.name, LOADER)
TRANSLATE = importlib.util.module_from_spec(SPEC)
LOADER.exec_module(TRANSLATE)


class CleanupTests(unittest.TestCase):
    def test_constants_follow_transitive_dependencies(self):
        definitions = {"BASE": "2", "NEXT": "BASE + 1", "LIMIT": "NEXT + 1",
                       "UNUSED": "BASE + 9"}
        self.assertEqual(TRANSLATE.referenced_defines(definitions, ["values[LIMIT];"]),
                         {"BASE", "NEXT", "LIMIT"})

    def test_native_only_constants_and_cycles(self):
        definitions = {"NATIVE": "FIRST", "FIRST": "SECOND", "SECOND": "FIRST",
                       "UNUSED": "0"}
        self.assertEqual(TRANSLATE.referenced_defines(definitions,
                         ["legacy_s64 generated;", "Check(NATIVE);"]),
                         {"NATIVE", "FIRST", "SECOND"})

    def test_comments_strings_and_unused_dependencies_do_not_keep_constants(self):
        definitions = {"COMMENT": "0", "STRING": "0", "UNUSED": "DEPENDENCY",
                       "DEPENDENCY": "0"}
        self.assertEqual(TRANSLATE.referenced_defines(definitions,
                         ['// COMMENT\n/* COMMENT */ Print("STRING");']), set())
        self.assertEqual(TRANSLATE.referenced_defines(definitions, []), set())

    def test_function_forwards_follow_first_use(self):
        source = """
            legacy_void First() { Later(); }
            legacy_void Later() { First(); Later(); }
            legacy_void Last() { Later(); }
        """
        self.assertEqual(TRANSLATE.required_forwards({"First", "Later", "Last"}, source),
                         {"Later"})

    def test_function_addresses_and_global_initializers_need_forwards(self):
        source = """
            legacy_u8 *callback = &Handler;
            legacy_s64 *pointer = &value;
            legacy_void Handler() {}
            legacy_s64 value;
        """
        self.assertEqual(TRANSLATE.required_forwards({"callback", "Handler", "pointer", "value"},
                         source), {"Handler", "value"})

    def test_native_extern_prevents_a_duplicate_generated_header(self):
        source = """
            extern legacy_void Later();
            legacy_void First() { Later(); }
            legacy_void Later() {}
        """
        self.assertEqual(TRANSLATE.required_forwards({"First", "Later"}, source), set())

    def test_class_cycles_and_self_pointers(self):
        source = """
            class CFirst { CFirst *self; CSecond *next; };
            class CSecond { CFirst *previous; };
        """
        self.assertEqual(TRANSLATE.required_forwards({"CFirst", "CSecond"}, source), {"CSecond"})

    def test_local_shadows_and_member_names_are_not_global_references(self):
        source = """
            class CNode { legacy_s64 value; };
            legacy_void First(legacy_s64 value) {
                legacy_s64 noreg local, noreg other;
                CNode *node;
                local = value + node->value + other;
            }
            legacy_s64 value, local, other;
        """
        self.assertEqual(TRANSLATE.required_forwards({"value", "local", "other"}, source), set())

    def test_prior_global_use_is_not_hidden_by_a_later_local(self):
        source = """
            legacy_void First() { value = 1; legacy_s64 value; value = 2; }
            legacy_s64 value;
        """
        self.assertEqual(TRANSLATE.required_forwards({"value"}, source), {"value"})

    def test_nested_local_remains_in_holyc_function_scope(self):
        source = """
            legacy_void First() { { legacy_s64 value; value = 1; } value = 2; }
            legacy_s64 value;
        """
        self.assertEqual(TRANSLATE.required_forwards({"value"}, source), set())

    def test_function_pointer_declarators_and_casts(self):
        source = """
            legacy_void (*callback)() = Handler;
            legacy_void Handler() {}
            legacy_void First(legacy_void (*value)()) {
                callback();
                value();
                callback(legacy_u8 *);
            }
            legacy_u8 *value;
        """
        self.assertEqual(TRANSLATE.required_forwards({"callback", "Handler", "First", "value"},
                         source), {"Handler"})

    def test_include_order_uses_buffered_root_relative_generated_files(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            sources = {root / "LOAD.HC": '#include "generated/ALL.HC"\nAfter();',
                       root / "generated/ALL.HC": '#include "generated/FUN.HC"\nMiddle();',
                       root / "generated/FUN.HC": 'Before();'}
            with patch.object(TRANSLATE, "HERE", root):
                self.assertEqual(TRANSLATE.compilation_source(root / "LOAD.HC", sources),
                                 'Before();\nMiddle();\nAfter();')

    def test_raster_replacement_does_not_generate_discarded_locals(self):
        tree = TRANSLATE.c_parser.CParser().parse("""
            typedef unsigned char legacy_u8;
            typedef unsigned short legacy_u16;
            typedef short legacy_s16;
            typedef void legacy_void;
            legacy_void shape2d_write_raster(legacy_u8 *bitmap, legacy_u16 destination,
                legacy_u8 value, legacy_s16 operation, legacy_u8 planar)
            {
                legacy_u8 unused = planar ? value : bitmap[destination];
                bitmap[destination] = unused;
            }
        """)
        model = TRANSLATE.Model()
        model.collect(tree, "fixture")
        TRANSLATE.build_name_map(model)
        generator = TRANSLATE.Generator(model)
        body = generator.function(tree.ext[-1], "shape2d_write_raster")
        self.assertEqual(generator.local_decls, [])
        self.assertIn("\tplanar;", body)
        self.assertIn("\tvideo_pages_raster_span(bitmap, destination, &value, 0, 1, operation, 0);", body)


if __name__ == "__main__":
    unittest.main()
