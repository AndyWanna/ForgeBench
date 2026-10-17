"""Tests for errors that could otherwise turn missing evidence into a result."""
import unittest
from pathlib import Path

import xml.etree.ElementTree as ET

from common import sanitize, sanitize_xml, unified_patch
from collect import annotate
from normalize_stream_dlti import normalize
from run_autosa import classify
from run_mlir import first_error, has_mlir_definition, top_symbol_from_ast
from stream_handoff import adapt as stream_handoff
from collect_tables import category
from minimal_check import harness, signature
from minimal_inputs import base_key


class EvidenceTests(unittest.TestCase):
    def test_dlti_bridge_preserves_values_and_operation_types(self):
        text = '#dlti.dl_entry<!llvm.ptr<270>, dense<32> : vector<4xi32>>, #dlti.dl_entry<i64, dense<64> : vector<2xi32>>'
        self.assertEqual(normalize(text), text.replace("xi32>", "xi64>"))
        self.assertEqual(normalize("%x = arith.constant 32 : i32"), "%x = arith.constant 32 : i32")

    def test_check_signature_and_harness(self):
        import tempfile
        with tempfile.TemporaryDirectory() as tmp:
            header = Path(tmp) / "top.h"
            header.write_text("typedef float data_t;\nvoid top(data_t DRAM_A[64][64], data_t DRAM_y[1]);\n")
            params = signature(header)
        self.assertEqual(params, [("DRAM_A", [64, 64]), ("DRAM_y", [1])])
        main = harness(params, {"DRAM_y"}, {}, 2, 1)
        # A NaN reference invalidates the trial instead of passing silently.
        self.assertIn("isnan", main)
        self.assertIn("INVALID", main)

    def test_sweep_configs_get_distinct_bases(self):
        a = {"config": "results/tool_eval/designs/gemm_0001/config.json", "data_type": "float"}
        b = {"config": "results/tool_eval/designs/dnn_0001/config.json", "data_type": "float"}
        self.assertNotEqual(base_key(a), base_key(b))

    def test_failure_categories_are_rule_based(self):
        row = {"error_head": "loc(\"<VITIS_INCLUDE>/etc/ap_fixed_base.h\":828:9): error", "detail": ""}
        self.assertEqual(category(row)[0], "F1")
        row = {"error_head": "'math.exp' op operand #0 must be floating-point-like, but got '!llvm.struct<...>'", "detail": ""}
        self.assertEqual(category(row)[0], "F5")
        self.assertEqual(category({"error_head": "segfault in some pass", "detail": ""})[0], "")

    def test_stream_handoff_drops_only_uncalled_helpers(self):
        module = ("module {\n  func.func @top() {\n    %0 = memref.alloca() : memref<4xf32>\n    return\n  }\n"
                  "  func.func @helper() {\n    return\n  }\n}\n")
        adapted = stream_handoff(module, "top")
        self.assertIn("memref.alloc() : memref<4xf32>", adapted)
        self.assertNotIn("@helper", adapted)
        with self.assertRaises(ValueError):
            stream_handoff(module.replace("    return\n  }\n  func", "    call @helper() : () -> ()\n    return\n  }\n  func", 1), "top")
        declared = module.replace("}\n", "}\n  func.func private @ext(f32) -> f32\n", 1)
        self.assertIn("func.func private @ext(f32) -> f32", stream_handoff(declared, "top"))

    def test_feature_attribution_requires_vendor_enum_evidence(self):
        result = {"status": "parse_failed", "error_head": "ap_fixed_base.h: error", "failure_category": ""}
        self.assertEqual(annotate(result, "visiting unknonwn sub decl stmt EnumDecl")["failure_category"], "F1")
        self.assertEqual(annotate(result, "unrelated failure")["failure_category"], "")
        self.assertEqual(result["failure_category"], "")

    def test_empty_mlir_module_is_not_a_parse_pass(self):
        self.assertFalse(has_mlir_definition("module {\n}\n", "_Z3topv"))
        self.assertTrue(has_mlir_definition("func.func @_Z3topv() {\n return\n}", "_Z3topv"))

    def test_cpp_symbol_is_selected_from_definition(self):
        ast = '{"kind":"VarDecl","name":"top"}\n' + '{"kind":"FunctionDecl","name":"top","mangledName":"_Z3topv","inner":[{"kind":"CompoundStmt"}]}'
        self.assertEqual(top_symbol_from_ast(ast, "top"), "_Z3topv")
        with self.assertRaises(ValueError):
            top_symbol_from_ast('{"kind":"FunctionDecl","name":"top"}', "top")

    def test_empty_success_is_not_codegen(self):
        result = classify(0, "", [])
        self.assertEqual(result["status"], "no_kernel_emitted")
        self.assertEqual(result["stage_reached"], "")
        self.assertEqual(result["rating"], "")

    def test_codegen_is_not_synthesis(self):
        result = classify(0, "generated a file", ["output/src/kernel_kernel.cpp"])
        self.assertEqual(result["stage_reached"], "codegen")
        self.assertEqual(result["rating"], "")

    def test_diagnostic_overrules_exit_zero(self):
        result = classify(0, "top.cpp:1: error: unknown type", ["output/src/kernel_kernel.cpp"])
        self.assertEqual(result["status"], "parse_failed")
        self.assertEqual(result["rating"], "none")

    def test_frontend_assertion_is_not_a_later_operator_failure(self):
        error = 'clang::NamedDecl::getName() const: Assertion `Name.isIdentifier()' + "' failed."
        result = classify(250, error, [])
        self.assertEqual(result["stage_reached"], "parse")
        self.assertEqual(result["failure_category"], "")
        self.assertEqual(result["error_head"], error)

    def test_environment_error_is_unclassified(self):
        result = classify(125, "Cannot connect to the Docker daemon", [])
        self.assertEqual(result["rating"], "")
        self.assertEqual(result["stage_reached"], "")
        self.assertEqual(classify(125, "error: Docker setup failed", ["kernel.cpp"])["rating"], "")

    def test_first_error_is_not_last_stack_line(self):
        self.assertEqual(first_error("warning: noise\ntop.cpp:7: error: bad type\nerror: cascading"),
                         "top.cpp:7: error: bad type")

    def test_private_paths_and_identity_are_redacted(self):
        path = "/" + "/".join(["export", "scratch", "person", "design", "top.cpp"])
        text = f"For user 'person' on host 'private.example' {path}:17: error: failure"
        cleaned = sanitize(text)
        self.assertNotIn("person", cleaned)
        self.assertNotIn("private.example", cleaned)
        self.assertIn("<PATH>/top.cpp:17", cleaned)

    def test_redacted_xml_stays_well_formed(self):
        path = "/" + "/".join(["tools", "vendor", "include", "hls_sqrt_apfixed.h"])
        xml = f'<r><SourceLocation>{path}:18</SourceLocation><Op SOURCE="{path}:255"/><DSP>1</DSP></r>'
        tree = ET.fromstring(sanitize_xml(xml, [("/run", "<RUN>")]))
        self.assertEqual(tree.findtext("SourceLocation"), "<PATH>/hls_sqrt_apfixed.h:18")
        self.assertEqual(tree.find("Op").get("SOURCE"), "<PATH>/hls_sqrt_apfixed.h:255")
        self.assertEqual(tree.findtext("DSP"), "1")

    def test_public_urls_survive(self):
        url = "https://github.com/UCLA-VAST/AutoSA"
        self.assertEqual(sanitize(url), url)

    def test_patch_preserves_missing_final_newline(self):
        patch = unified_patch("first\nlast", "first\nchanged")
        self.assertIn("-last\n\\ No newline at end of file\n", patch)
        self.assertIn("+changed\n\\ No newline at end of file\n", patch)


if __name__ == "__main__":
    unittest.main()
