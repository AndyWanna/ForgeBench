# Source-to-source tool evaluation (Case Study 3, Table 9)

This directory backs Table 9 (`tab:tool-support`) of `Forgebench_FPT_Journal_2026_R2.pdf`
(Case Study 3; `sec/results.tex`, `table/testcases_table.tex`). It covers ScaleHLS, HIDA,
AutoSA and StreamHLS on ForgeBench-generated GEMM, DNN and LLM designs. It follows
`todo_tool_eval.md` item by item. No manuscript file was modified; where the evidence differs
from the paper, the difference is stated here, with a suggested revision in
[§7](#7-suggestions-for-revising-the-paper).

Every result below comes from a committed command, input and log. All tables inside
`GENERATED` blocks are produced by [`scripts/report_tables.py`](scripts/report_tables.py) from
[`minimal/results.csv`](minimal/results.csv) and [`summary.csv`](summary.csv); none is typed by
hand.

## 1. What was verified, and the results

Two levels were tested, in all three domains, with all four tools:

* **Whole generated designs** (end to end): the sweep designs `gemm_0001`, `gemm_0004`,
  `dnn_0001` and `llm_0001`, each in three forms:
  * as generated (`ap_fixed<16,5>`);
  * generated with the generator's `float` option;
  * `float` plus a committed patch.
* **Single operators**: the smallest designs the generator can produce for one operator:
  * GEMM in all six loop orders;
  * convolution 1×1, 3×3 without padding, and 3×3 with padding;
  * RMSNorm, softmax, multi-head attention (MHA), and MHA with RoPE.

  Each is tested as generated, in `float`, and with patches.

"Correct" means the tool's emitted C++ matches the unmodified generated `float` design on 5
random input sets, with max |ref − out| / max(1, |ref|) < 1e-4 over every output element
(`scripts/minimal_check.py`). Only correct output is synthesized. A tool output that synthesizes
but computes the wrong result is counted as a failure.

<!-- BEGIN GENERATED: overview -->
| Tool | GEMM whole design | GEMM single op | DNN whole design | DNN single ops | LLM whole design | LLM single ops |
|---|---|---|---|---|---|---|
| ScaleHLS | ✅ `gemm_0004` with edits | ✅ 2/6: gemm_ikj, gemm_kij; ❌ gemm_ijk (wrong results), gemm_jik (wrong results), gemm_jki (wrong results), gemm_kji (wrong results) | ❌ wrong results | ✅ 3/3: conv_k1, conv_k3p0, conv_k3 | ❌ DSE timeout | ✅ 2/4: rmsnorm, softmax; ❌ mha (wrong results), mha_rope (wrong results) |
| HIDA | ❌ emitted code unusable | ✅ 6/6: gemm_ijk, gemm_ikj, gemm_jik, gemm_jki, gemm_kij, gemm_kji | ❌ emitted code unusable | ✅ 3/3: conv_k1, conv_k3p0, conv_k3 | ✅ `llm_0001` with edits | ✅ 4/4: rmsnorm, softmax, mha, mha_rope |
| StreamHLS | ❌ crash in transform | ✅ 2/6: gemm_ikj, gemm_kij; ❌ gemm_ijk (crash in transform), gemm_jik (crash in transform), gemm_jki (crash in transform), gemm_kji (crash in transform) | ❌ crash in transform | ✅ 2/3: conv_k1, conv_k3p0; ❌ conv_k3 (wrong results) | ❌ crash in transform | ❌ 0/4; ❌ rmsnorm (crash in transform), softmax (crash in transform), mha (crash in transform), mha_rope (crash in transform) |
| AutoSA | ❌ F1 (vendor ap_fixed / C++ header implementation) | ❌ 0/2; ❌ gemm_ijk (emitted code unusable), gemm_ikj (emitted code unusable) | ❌ F1 (vendor ap_fixed / C++ header implementation) | ❌ 0/3; ❌ conv_k1 (wrong results), conv_k3p0 (emitted code unusable), conv_k3 (F2 (loop nest outside the single uniform band AutoSA requires)) | ❌ F1 (vendor ap_fixed / C++ header implementation) | ❌ 0/4; ❌ rmsnorm (kernel hangs), softmax (kernel hangs), mha (crash in parse), mha_rope (F1 (vendor ap_fixed / C++ header implementation)) |
<!-- END GENERATED: overview -->

**Whole designs versus single operators:**

* **DNN:** no tool passes the whole generated design end to end, either as generated or with
  edits. Single convolution operators do pass:
  * HIDA and ScaleHLS on all three convolutions, including the padded 3×3;
  * StreamHLS on the two unpadded ones.
* **LLM:** no tool passes the whole generated design as generated (fixed point or float). With
  edits, HIDA passes the whole `llm_0001` (attention + RoPE + LayerNorm) end to end; ScaleHLS's
  DSE did not finish within 30 minutes; StreamHLS and AutoSA fail. Per operator:
  * HIDA passes all four LLM operators;
  * ScaleHLS passes RMSNorm and softmax;
  * StreamHLS and AutoSA pass none.
* **GEMM:** no tool passes the whole generated design as generated. With edits, ScaleHLS passes
  the whole `gemm_0004` (the `vm_order = ji` sibling of `gemm_0001`) end to end. HIDA's
  emitted top level turns the one-element output port `DRAM_result[1]` into a by-value scalar,
  so the result never reaches the caller. Per operator:
  * HIDA passes all six loop orders;
  * ScaleHLS and StreamHLS pass the two orders with the reduction loop innermost;
  * AutoSA passes none.
* **AutoSA** produces no correct kernel at any level. Its own documented example,
  `autosa_tests/mm`, also fails its built-in golden check in the pinned image ("Failed with
  4096 errors!"; [`minimal/logs/autosa_control_mm/`](minimal/logs/autosa_control_mm/autosa/run.log)).
  So no AutoSA result can be validated in this environment.

Detailed results follow. Every row links its exact input and patch; logs are in
`minimal/logs/<input>/<tool>/`.

<!-- BEGIN GENERATED: whole -->
| Design | Input | Tool | Stage reached | Result | Why it failed |
|---|---|---|---|---|---|
| `gemm_0001` | generated `ap_fixed<16,5>` | ScaleHLS | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":828:9): error:  + visiting unknonwn sub decl stmt` |
| `gemm_0001` | generated `ap_fixed<16,5>` | HIDA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":828:9): error:  + visiting unknonwn sub decl stmt` |
| `gemm_0001` | generated `ap_fixed<16,5>` + `--stream-polygeist-handoff` | StreamHLS | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc(callsite("<VITIS_INCLUDE>/etc/ap_private.h":2210:9 at "<VITIS_INCLUDE>/etc/ap_private.h":6766:28)): err...` |
| `gemm_0001` | generated `ap_fixed<16,5>` | AutoSA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier(...` |
| `gemm_0001` | generated `float` | ScaleHLS | transform | ❌ stops at transform | F3: written global buffer treated as a constant (ConvertGetGlobalToConstBuffer); `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::StringAttr, mlir::...` |
| `gemm_0001` | generated `float` | HIDA | transform | ❌ stops at transform | F3: written global buffer treated as a constant (ConvertGetGlobalToConstBuffer); `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::StringAttr, mlir::...` |
| `gemm_0001` | generated `float` + `--stream-polygeist-handoff` | StreamHLS | transform | ❌ stops at transform | transform_failed |
| `gemm_0001` | generated `float` | AutoSA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier(...` |
| `gemm_0001` | generated `float` + [`gemm_0001__edited.patch`](minimal/patches/gemm_0001__edited.patch) | ScaleHLS | codegen | ❌ codegen reached, output wrong/unusable | wrong results (max_error 2.5) |
| `gemm_0001` | generated `float` + [`gemm_0001__edited.patch`](minimal/patches/gemm_0001__edited.patch) | HIDA | codegen | ❌ codegen reached, output wrong/unusable | emitted C++ unusable: `emitted.cpp:216:196: error: cannot convert ‘float*’ to ‘float’` |
| `gemm_0001` | generated `float` + [`gemm_0001__edited.patch`](minimal/patches/gemm_0001__edited.patch) + `--stream-polygeist-handoff` | StreamHLS | transform | ❌ stops at transform | transform_failed |
| `gemm_0004` | generated `ap_fixed<16,5>` | ScaleHLS | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":828:9): error:  + visiting unknonwn sub decl stmt` |
| `gemm_0004` | generated `ap_fixed<16,5>` | HIDA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":828:9): error:  + visiting unknonwn sub decl stmt` |
| `gemm_0004` | generated `ap_fixed<16,5>` + `--stream-polygeist-handoff` | StreamHLS | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc(callsite("<VITIS_INCLUDE>/etc/ap_private.h":2210:9 at "<VITIS_INCLUDE>/etc/ap_private.h":6766:28)): err...` |
| `gemm_0004` | generated `ap_fixed<16,5>` | AutoSA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier(...` |
| `gemm_0004` | generated `float` | ScaleHLS | transform | ❌ stops at transform | F3: written global buffer treated as a constant (ConvertGetGlobalToConstBuffer); `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::StringAttr, mlir::...` |
| `gemm_0004` | generated `float` | HIDA | transform | ❌ stops at transform | F3: written global buffer treated as a constant (ConvertGetGlobalToConstBuffer); `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::StringAttr, mlir::...` |
| `gemm_0004` | generated `float` + `--stream-polygeist-handoff` | StreamHLS | transform | ❌ stops at transform | transform_failed |
| `gemm_0004` | generated `float` | AutoSA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier(...` |
| `gemm_0004` | generated `float` + [`gemm_0004__edited.patch`](minimal/patches/gemm_0004__edited.patch) | ScaleHLS | synth | ✅ synthesized, correct |  |
| `gemm_0004` | generated `float` + [`gemm_0004__edited.patch`](minimal/patches/gemm_0004__edited.patch) | HIDA | codegen | ❌ codegen reached, output wrong/unusable | emitted C++ unusable: `emitted.cpp:208:196: error: cannot convert ‘float*’ to ‘float’` |
| `gemm_0004` | generated `float` + [`gemm_0004__edited.patch`](minimal/patches/gemm_0004__edited.patch) + `--stream-polygeist-handoff` | StreamHLS | transform | ❌ stops at transform | transform_failed |
| `dnn_0001` | generated `ap_fixed<16,5>` | ScaleHLS | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":1288:5): error:  + visiting unknonwn sub decl stmt` |
| `dnn_0001` | generated `ap_fixed<16,5>` | HIDA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":1288:5): error:  + visiting unknonwn sub decl stmt` |
| `dnn_0001` | generated `ap_fixed<16,5>` + `--stream-polygeist-handoff` | StreamHLS | parse | ❌ stops at parse | `cgeist: <WORK><PATH>/ValueCategory.cc:154: void ValueCategory::store(mlir::Location, mlir::OpBuilder &, Val...` |
| `dnn_0001` | generated `ap_fixed<16,5>` | AutoSA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier(...` |
| `dnn_0001` | generated `float` | ScaleHLS | transform | ❌ stops at transform | F3: written global buffer treated as a constant (ConvertGetGlobalToConstBuffer); `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::StringAttr, mlir::...` |
| `dnn_0001` | generated `float` | HIDA | transform | ❌ stops at transform | F3: written global buffer treated as a constant (ConvertGetGlobalToConstBuffer); `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::StringAttr, mlir::...` |
| `dnn_0001` | generated `float` + `--stream-polygeist-handoff` | StreamHLS | transform | ❌ stops at transform | `streamhls-opt: <WORK><PATH>/ilist_iterator.h:138: llvm::ilist_iterator::reference llvm::ilist_iterator<llvm...` |
| `dnn_0001` | generated `float` | AutoSA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier(...` |
| `dnn_0001` | generated `float` + [`dnn_0001__edited.patch`](minimal/patches/dnn_0001__edited.patch) | ScaleHLS | codegen | ❌ codegen reached, output wrong/unusable | wrong results (max_error 1.02) |
| `dnn_0001` | generated `float` + [`dnn_0001__edited.patch`](minimal/patches/dnn_0001__edited.patch) | HIDA | codegen | ❌ codegen reached, output wrong/unusable | emitted C++ unusable: `emitted.cpp:407:258: error: cannot convert ‘float (*)[16][1][1]’ to ‘float (*)[16]’` |
| `dnn_0001` | generated `float` + [`dnn_0001__edited.patch`](minimal/patches/dnn_0001__edited.patch) + `--stream-polygeist-handoff` | StreamHLS | transform | ❌ stops at transform | `streamhls-opt: <WORK><PATH>/ilist_iterator.h:138: llvm::ilist_iterator::reference llvm::ilist_iterator<llvm...` |
| `llm_0001` | generated `ap_fixed<16,5>` | ScaleHLS | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":1639:5): error:  + visiting unknonwn sub decl stmt` |
| `llm_0001` | generated `ap_fixed<16,5>` | HIDA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":1639:5): error:  + visiting unknonwn sub decl stmt` |
| `llm_0001` | generated `ap_fixed<16,5>` + `--stream-polygeist-handoff` | StreamHLS | parse | ❌ stops at parse | `cgeist: <WORK><PATH>/ValueCategory.cc:154: void ValueCategory::store(mlir::Location, mlir::OpBuilder &, Val...` |
| `llm_0001` | generated `ap_fixed<16,5>` | AutoSA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier(...` |
| `llm_0001` | generated `float` | ScaleHLS | transform | ❌ stops at transform | F3: written global buffer treated as a constant (ConvertGetGlobalToConstBuffer); `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::StringAttr, mlir::...` |
| `llm_0001` | generated `float` | HIDA | transform | ❌ stops at transform | F3: written global buffer treated as a constant (ConvertGetGlobalToConstBuffer); `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::StringAttr, mlir::...` |
| `llm_0001` | generated `float` + `--stream-polygeist-handoff` | StreamHLS | transform | ❌ stops at transform | `streamhls-opt: <WORK><PATH>/ilist_iterator.h:138: llvm::ilist_iterator::reference llvm::ilist_iterator<llvm...` |
| `llm_0001` | generated `float` | AutoSA | parse | ❌ stops at parse | F1: vendor ap_fixed / C++ header implementation; `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier(...` |
| `llm_0001` | generated `float` + [`llm_0001__edited.patch`](minimal/patches/llm_0001__edited.patch) | ScaleHLS | transform | ❌ stops at transform | transformation (DSE) did not finish within the run's time limit |
| `llm_0001` | generated `float` + [`llm_0001__edited.patch`](minimal/patches/llm_0001__edited.patch) | HIDA | codegen | ❌ stops at codegen | `<RUN>/transformed.mlir:622:10: error: 'hls.dataflow.buffer' op only support depth of 1` |
| `llm_0001` | generated `float` + [`llm_0001__edited.patch`](minimal/patches/llm_0001__edited.patch) + `place-external-buffer=false` | HIDA | synth | ✅ synthesized, correct |  |
| `llm_0001` | generated `float` + [`llm_0001__edited.patch`](minimal/patches/llm_0001__edited.patch) + `--stream-polygeist-handoff` | StreamHLS | transform | ❌ stops at transform | `streamhls-opt: <WORK><PATH>/ilist_iterator.h:138: llvm::ilist_iterator::reference llvm::ilist_iterator<llvm...` |
| `gemm_0001` | generated `float` + [`gemm_0001__edited.patch`](minimal/patches/gemm_0001__edited.patch) + `place-external-buffer=false` | HIDA | codegen | ❌ codegen reached, output wrong/unusable | emitted C++ unusable: `emitted.cpp:216:196: error: cannot convert ‘float*’ to ‘float’` |
| `gemm_0004` | generated `float` + [`gemm_0004__edited.patch`](minimal/patches/gemm_0004__edited.patch) + `place-external-buffer=false` | HIDA | codegen | ❌ codegen reached, output wrong/unusable | emitted C++ unusable: `emitted.cpp:208:196: error: cannot convert ‘float*’ to ‘float’` |
| `dnn_0001` | generated `float` + [`dnn_0001__edited.patch`](minimal/patches/dnn_0001__edited.patch) + `place-external-buffer=false` | HIDA | codegen | ❌ codegen reached, output wrong/unusable | wrong results (max_error 0.664) |
<!-- END GENERATED: whole -->

<!-- BEGIN GENERATED: operators -->
| Domain | Operator | Tool | Best result | Input of that attempt | Why it failed |
|---|---|---|---|---|---|
| gemm | `gemm_ijk` | ScaleHLS | ❌ codegen reached, output wrong/unusable | generated `float` + [`gemm_ijk__local.patch`](minimal/patches/gemm_ijk__local.patch) | wrong results (max_error 10.8) |
| gemm | `gemm_ijk` | HIDA | ✅ synthesized, correct | generated `float` + [`gemm_ijk__local.patch`](minimal/patches/gemm_ijk__local.patch) |  |
| gemm | `gemm_ijk` | StreamHLS | ❌ stops at transform | generated `float` + [`gemm_ijk__local.patch`](minimal/patches/gemm_ijk__local.patch) + `--stream-polygeist-handoff` | `streamhls-opt: <WORK><PATH>/LoopUtils.cpp:1438: unsigned int mlir::affine::permuteLoops(MutableArrayRef<mli...` |
| gemm | `gemm_ijk` | AutoSA | ❌ codegen reached, output wrong/unusable | generated `float` + [`gemm_ijk__autosa.patch`](minimal/patches/gemm_ijk__autosa.patch) + `sa-sizes space_time[0];array_part[16,16,16];latency[4,4]` | emitted C++ unusable: `<SRC>/top_kernel.cpp:465:28: error: ‘fifo_BRAM_B_in’ was not declared in this scope` |
| gemm | `gemm_ikj` | ScaleHLS | ✅ synthesized, correct | generated `float` + [`gemm_ikj__local.patch`](minimal/patches/gemm_ikj__local.patch) |  |
| gemm | `gemm_ikj` | HIDA | ✅ synthesized, correct | generated `float` + [`gemm_ikj__local.patch`](minimal/patches/gemm_ikj__local.patch) |  |
| gemm | `gemm_ikj` | StreamHLS | ✅ synthesized, correct | generated `float` + [`gemm_ikj__local.patch`](minimal/patches/gemm_ikj__local.patch) + `--stream-polygeist-handoff` |  |
| gemm | `gemm_jik` | ScaleHLS | ❌ codegen reached, output wrong/unusable | generated `float` + [`gemm_jik__local.patch`](minimal/patches/gemm_jik__local.patch) | wrong results (max_error 10.5) |
| gemm | `gemm_jik` | HIDA | ✅ synthesized, correct | generated `float` + [`gemm_jik__local.patch`](minimal/patches/gemm_jik__local.patch) |  |
| gemm | `gemm_jik` | StreamHLS | ❌ stops at transform | generated `float` + [`gemm_jik__local.patch`](minimal/patches/gemm_jik__local.patch) + `--stream-polygeist-handoff` | `streamhls-opt: <WORK><PATH>/LoopUtils.cpp:1438: unsigned int mlir::affine::permuteLoops(MutableArrayRef<mli...` |
| gemm | `gemm_jki` | ScaleHLS | ❌ codegen reached, output wrong/unusable | generated `float` + [`gemm_jki__local.patch`](minimal/patches/gemm_jki__local.patch) | wrong results (max_error 10.3) |
| gemm | `gemm_jki` | HIDA | ✅ synthesized, correct | generated `float` + [`gemm_jki__local.patch`](minimal/patches/gemm_jki__local.patch) |  |
| gemm | `gemm_jki` | StreamHLS | ❌ stops at transform | generated `float` + [`gemm_jki__local.patch`](minimal/patches/gemm_jki__local.patch) + `--stream-polygeist-handoff` | `streamhls-opt: <WORK><PATH>/LoopUtils.cpp:1438: unsigned int mlir::affine::permuteLoops(MutableArrayRef<mli...` |
| gemm | `gemm_kij` | ScaleHLS | ✅ synthesized, correct | generated `float` + [`gemm_kij__local.patch`](minimal/patches/gemm_kij__local.patch) |  |
| gemm | `gemm_kij` | HIDA | ✅ synthesized, correct | generated `float` + [`gemm_kij__local.patch`](minimal/patches/gemm_kij__local.patch) |  |
| gemm | `gemm_kij` | StreamHLS | ✅ synthesized, correct | generated `float` + [`gemm_kij__local.patch`](minimal/patches/gemm_kij__local.patch) + `--stream-polygeist-handoff` |  |
| gemm | `gemm_kji` | ScaleHLS | ❌ codegen reached, output wrong/unusable | generated `float` + [`gemm_kji__local.patch`](minimal/patches/gemm_kji__local.patch) | wrong results (max_error 10.3) |
| gemm | `gemm_kji` | HIDA | ✅ synthesized, correct | generated `float` + [`gemm_kji__local.patch`](minimal/patches/gemm_kji__local.patch) |  |
| gemm | `gemm_kji` | StreamHLS | ❌ stops at transform | generated `float` + [`gemm_kji__local.patch`](minimal/patches/gemm_kji__local.patch) + `--stream-polygeist-handoff` | `streamhls-opt: <WORK><PATH>/LoopUtils.cpp:1438: unsigned int mlir::affine::permuteLoops(MutableArrayRef<mli...` |
| gemm | `gemm_ikj` | AutoSA | ❌ codegen reached, output wrong/unusable | generated `float` + [`gemm_ikj__autosa.patch`](minimal/patches/gemm_ikj__autosa.patch) + `sa-sizes space_time[0];array_part[16,16,16];latency[4,4]` | emitted C++ unusable: `<SRC>/top_kernel.cpp:465:28: error: ‘fifo_BRAM_B_in’ was not declared in this scope` |
| dnn | `conv_k1` | ScaleHLS | ✅ synthesized, correct | generated `float` + [`conv_k1__sunkguard.patch`](minimal/patches/conv_k1__sunkguard.patch) |  |
| dnn | `conv_k1` | HIDA | ✅ synthesized, correct | generated `float` + [`conv_k1__sunkguard.patch`](minimal/patches/conv_k1__sunkguard.patch) |  |
| dnn | `conv_k1` | StreamHLS | ✅ synthesized, correct | generated `float` + [`conv_k1__sunkguard.patch`](minimal/patches/conv_k1__sunkguard.patch) + `--stream-polygeist-handoff` |  |
| dnn | `conv_k1` | AutoSA | ❌ codegen reached, output wrong/unusable | generated `float` + [`conv_k1__autosa.patch`](minimal/patches/conv_k1__autosa.patch) + `sa-sizes space_time[0];array_part[14,14,16,16];latency[2,2,4]` | wrong results (max_error 1.82e+35) |
| dnn | `conv_k3p0` | ScaleHLS | ✅ synthesized, correct | generated `float` + [`conv_k3p0__sunkguard.patch`](minimal/patches/conv_k3p0__sunkguard.patch) |  |
| dnn | `conv_k3p0` | HIDA | ✅ synthesized, correct | generated `float` + [`conv_k3p0__sunkguard.patch`](minimal/patches/conv_k3p0__sunkguard.patch) |  |
| dnn | `conv_k3p0` | StreamHLS | ✅ synthesized, correct | generated `float` + [`conv_k3p0__noguard_reorder.patch`](minimal/patches/conv_k3p0__noguard_reorder.patch) + `--stream-polygeist-handoff` |  |
| dnn | `conv_k3p0` | AutoSA | ❌ codegen reached, output wrong/unusable | generated `float` + [`conv_k3p0__autosa.patch`](minimal/patches/conv_k3p0__autosa.patch) + `sa-sizes space_time[0];array_part[12,12,16,3];latency[4,4,4]` | emitted C++ unusable: `<SRC>/top_kernel.cpp:398:22: error: ‘fifo_kernel_in’ was not declared in this scope` |
| dnn | `conv_k3` | ScaleHLS | ✅ synthesized, correct | generated `float` + [`conv_k3__sunkguard.patch`](minimal/patches/conv_k3__sunkguard.patch) |  |
| dnn | `conv_k3` | HIDA | ✅ synthesized, correct | generated `float` + [`conv_k3__sunkguard.patch`](minimal/patches/conv_k3__sunkguard.patch) |  |
| dnn | `conv_k3` | StreamHLS | ❌ codegen reached, output wrong/unusable | generated `float` + [`conv_k3__sunkguard.patch`](minimal/patches/conv_k3__sunkguard.patch) + `--stream-polygeist-handoff` | wrong results (max_error 5.14) |
| dnn | `conv_k3` | AutoSA | ❌ stops at setup | generated `float` + [`conv_k3__autosa_guard.patch`](minimal/patches/conv_k3__autosa_guard.patch) | F2: loop nest outside the single uniform band AutoSA requires; `what():  [AutoSA] Error: Non-uniform dependence detected.` |
| llm | `rmsnorm` | ScaleHLS | ✅ synthesized, correct | generated `float` + [`rmsnorm__local.patch`](minimal/patches/rmsnorm__local.patch) |  |
| llm | `rmsnorm` | HIDA | ✅ synthesized, correct | generated `float` + [`rmsnorm__rowacc_split.patch`](minimal/patches/rmsnorm__rowacc_split.patch) + `place-external-buffer=false` |  |
| llm | `rmsnorm` | StreamHLS | ❌ stops at transform | generated `float` + [`rmsnorm__local.patch`](minimal/patches/rmsnorm__local.patch) + `--stream-polygeist-handoff` | `streamhls-opt: <WORK><PATH>/ilist_iterator.h:138: llvm::ilist_iterator::reference llvm::ilist_iterator<llvm...` |
| llm | `rmsnorm` | AutoSA | ❌ codegen reached, output wrong/unusable | generated `float` + [`rmsnorm__autosa.patch`](minimal/patches/rmsnorm__autosa.patch) + `sa-sizes space_time[0];array_part[8,16];latency[4]` | emitted kernel hangs in C simulation |
| llm | `softmax` | ScaleHLS | ✅ synthesized, correct | generated `float` + [`softmax__local.patch`](minimal/patches/softmax__local.patch) |  |
| llm | `softmax` | HIDA | ✅ synthesized, correct | generated `float` + [`softmax__rowacc_split.patch`](minimal/patches/softmax__rowacc_split.patch) + `place-external-buffer=false` |  |
| llm | `softmax` | StreamHLS | ❌ stops at transform | generated `float` + [`softmax__local.patch`](minimal/patches/softmax__local.patch) + `--stream-polygeist-handoff` | `streamhls-opt: <WORK><PATH>/AffineMemAccess.cpp:1293: mlir::AffineMap mlir::streamhls::getMinimalAccessPatt...` |
| llm | `softmax` | AutoSA | ❌ codegen reached, output wrong/unusable | generated `float` + [`softmax__autosa.patch`](minimal/patches/softmax__autosa.patch) + `sa-sizes space_time[0];array_part[8,16];latency[4]` | emitted kernel hangs in C simulation |
| llm | `mha` | ScaleHLS | ❌ codegen reached, output wrong/unusable | generated `float` + [`mha__attn.patch`](minimal/patches/mha__attn.patch) | wrong results (max_error 0.0914) |
| llm | `mha` | HIDA | ✅ synthesized, correct | generated `float` + [`mha__attn.patch`](minimal/patches/mha__attn.patch) + `place-external-buffer=false` |  |
| llm | `mha` | StreamHLS | ❌ stops at transform | generated `float` + [`mha__local.patch`](minimal/patches/mha__local.patch) + `--stream-polygeist-handoff` | `streamhls-opt: <WORK><PATH>/LoopUtils.cpp:1438: unsigned int mlir::affine::permuteLoops(MutableArrayRef<mli...` |
| llm | `mha` | AutoSA | ❌ stops at parse | generated `float` + [`mha__autosa_scores.patch`](minimal/patches/mha__autosa_scores.patch) | `collect2: error: ld returned 1 exit status` |
| llm | `mha_rope` | ScaleHLS | ❌ codegen reached, output wrong/unusable | generated `float` + [`mha_rope__attn.patch`](minimal/patches/mha_rope__attn.patch) | wrong results (max_error 0.0991) |
| llm | `mha_rope` | HIDA | ✅ synthesized, correct | generated `float` + [`mha_rope__attn.patch`](minimal/patches/mha_rope__attn.patch) + `place-external-buffer=false` |  |
| llm | `mha_rope` | StreamHLS | ❌ stops at transform | generated `float` + [`mha_rope__local.patch`](minimal/patches/mha_rope__local.patch) + `--stream-polygeist-handoff` | `streamhls-opt: <WORK><PATH>/LoopUtils.cpp:1438: unsigned int mlir::affine::permuteLoops(MutableArrayRef<mli...` |
| llm | `mha_rope` | AutoSA | ❌ stops at parse | generated `ap_fixed<16,5>` | F1: vendor ap_fixed / C++ header implementation; `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier(...` |
<!-- END GENERATED: operators -->

## 2. TODO item 1: one piece of evidence per table cell

### 2.1 Designs fed to each tool

| Level | Inputs | Where |
|---|---|---|
| Whole designs (Table 9's cohort) | `gemm_0001`, `dnn_0001`, `llm_0001`: the first design of each manifest (`manifest/designs/{gemm,dnn,llm}.csv`) | configs and unmodified generated sources in [`designs/<id>/`](designs/); hashes in [`designs/manifest.json`](designs/manifest.json) |
| Whole designs, `float` option | the same three, `data_type = float` | [`designs/<id>_float/`](designs/), [`designs/float_manifest.json`](designs/float_manifest.json) |
| Whole design, loop-order sibling | `gemm_0004` (`vm_order` = `ji`/`ji`; otherwise identical to `gemm_0001`) | sweep config `designs/configs/ml_sweep/gemm/GEMM_config_M64_K64_N64_UM1_UK1_UN1_GORDna_VM1ji_VM2ji_...json` |
| Single operators | the 13 configs in [`minimal/configs/`](minimal/configs/) | generated by `forgebench generate` |

Every tool input, including each edited one, is listed in [`minimal/cases.json`](minimal/cases.json)
as a ForgeBench config, a data type, and an optional patch against the generated source. Its
`top.cpp` hash is recorded there too. `scripts/minimal_inputs.py` regenerates all 67 inputs
with ForgeBench's own generator and refuses any hash mismatch. No input is a hand-written kernel.

### 2.2 Tool versions

[`versions.json`](versions.json) pins the sources and executable hashes:

| Tool | Repository | Commit / dependency |
|---|---|---|
| ScaleHLS | [UIUC-ChenLab/scalehls](https://github.com/UIUC-ChenLab/scalehls) | `b69f334c9c122fad3759fcf388d3f039689b6fc0`; Polygeist `737290e9`, LLVM/MLIR 16.0.0git `cf0ff67a` |
| HIDA | [UIUC-ChenLab/ScaleHLS-HIDA](https://github.com/UIUC-ChenLab/ScaleHLS-HIDA) | `fd79f6c946593c290d6d285c2a2efcab72fbe9c6`; same Polygeist/LLVM as ScaleHLS |
| AutoSA | [UCLA-VAST/AutoSA](https://github.com/UCLA-VAST/AutoSA) | `b61a1b4132d631600696feba59eb606acb34d304` (upstream HEAD), image `whbldhwj/autosa@sha256:b9318c50…`; Clang 9.0.1, PET 0.11.3, ISL 0.22 |
| StreamHLS | [UCLA-VAST/Stream-HLS](https://github.com/UCLA-VAST/Stream-HLS) | `00c6219a82fc710492f4eb44e4f34836dfb19e08`; LLVM/MLIR 18.1.2 `26a1d660`; C front end Polygeist `77c04bb2` with LLVM 18.0.0git `26eb4285` |
| Vitis HLS | AMD | 2024.1, part `xczu9eg-ffvb1156-2-e`, 10 ns |

The tools that were run for the paper's original table, and their versions, are unknown; see
[§8](#8-what-is-missing-and-discrepancies).

### 2.3 `summary.csv`

[`summary.csv`](summary.csv) has one row per tool × domain, with the TODO's columns. It rates the
**whole generated designs**:
* `full` = synthesizes with correct results and no source edit;
* `half` = the same after the committed non-algorithmic patch;
* `none` = otherwise.

<!-- BEGIN GENERATED: summary -->
| Tool | GEMM | DNN | LLM |
|---|---|---|---|
| ScaleHLS | ◐ half (synth) | ○ none (codegen) | ○ none (transform) |
| HIDA | ○ none (codegen) | ○ none (codegen) | ◐ half (synth) |
| AutoSA | ○ none (parse) | ○ none (parse) | ○ none (parse) |
| StreamHLS | ○ none (transform) | ○ none (transform) | ○ none (transform) |
<!-- END GENERATED: summary -->

The per-operator ratings are in [`minimal/summary_operators.csv`](minimal/summary_operators.csv).
`float_summary.csv`, `float_outcomes.csv` and `float_runs.json` record the first, separate run of
the three unmodified `float` designs (`scripts/collect_float.py`). They agree with the `float`
rows of §1.

### 2.4 One log per cell

* `logs/<tool>/<domain>/` holds the primary run on each unmodified generated design
  (`run.log`, `result.json`), its float run (`float-*`), and the edited whole-design runs
  (`<design>-edited-*`).
* Every other run, at both levels, has `run.log`, `result.json` and, where code was emitted,
  `check.json` in `minimal/logs/<input>/<tool>/`.

Logs are trimmed to the command, the first error, and the last ~50 lines. Full transcripts are
in the tarball (§6.2).

### 2.5 The edits behind each ◐ result

Every patch is against the source ForgeBench generates with the `float` option. Each edited
source was checked to compute exactly the same result as the generated design before any tool
saw it. `patches/<tool>/` holds the patch of every correct, synthesized result of that tool
(copied by `scripts/collect_tables.py`; `validate.py` checks they are identical to
`minimal/patches/`).

**Whole designs:**

* **ScaleHLS, GEMM:** [`patches/scalehls/gemm_0004__edited.patch`](patches/scalehls/gemm_0004__edited.patch), on `gemm_0004`.
  1. The ten `BRAM_*` buffers move from file scope into `top()`. ScaleHLS and HIDA convert every
     global array into a constant buffer (`ConvertGetGlobalToConstBuffer`), and a written global
     with no initializer then asserts.
  2. The dot product's `BRAM_result[0] = 0;` moves into the first iteration of its loop
     (`if (i == 0) …`). ScaleHLS's DSE crashes on a store outside any loop that a later loop
     reads.

  `gemm_0004` is used rather than `gemm_0001` because ScaleHLS miscompiles the `ij` order
  (point 4 of §3.5).
* **HIDA, LLM:** [`patches/hida/llm_0001__edited.patch`](patches/hida/llm_0001__edited.patch), plus the HIDA option
  `place-external-buffer=false`.
  1. Global buffers move into `top()`.
  2. The attention's scalar accumulators (`context`, `sum_exp`, `max_score`) and LayerNorm's
     (`sum`, `var_sum`) become one slot per row.
  3. Each zero-initialization moves into its own loop ahead of its reduction.
  4. The run-time `groups` argument is specialized to the constant 1 the call site passes.
  5. The softmax maximum is written as a value select.

  HIDA's emitter cannot print loop-carried scalars (`float v21 = float v22 + v20;`), and the
  run-time `groups` makes the head index non-affine.

**The paper's own ◐ cells** (AutoSA GEMM, AutoSA DNN, StreamHLS GEMM):

* **AutoSA:** no edit set produces a correct kernel; see §3.2. The recovered historical AutoSA
  inputs are rewrites (`float`, removed interfaces, changed loop nests), not minor edits:
  [`patches/autosa/*-historical-rewrite.patch`](patches/autosa/).
* **StreamHLS GEMM:** the whole design fails. The single-operator GEMM passes with
  [`patches/streamhls/gemm_ikj__local.patch`](patches/streamhls/gemm_ikj__local.patch) (buffers into
  `top()`), the `ikj`/`kij` loop order, and the lossless IR handoff
  [`scripts/stream_handoff.py`](scripts/stream_handoff.py). The handoff changes `memref.alloca` to
  `memref.alloc`, drops uncalled helper functions, and enables `bufferize-func-args`. It is needed
  because StreamHLS's kernel pipeline expects torch-mlir's IR, not Polygeist's.

## 3. TODO item 2: answers to the table questions

### 3.1 Meaning of the F1–F6 ticks

The prose calls F1–F6 limitation categories, but the header says "HLS Support". Only the
**support** reading is consistent with the table itself. AutoSA, the only polyhedral tool, is
also the only one with a cross in F2 (dynamic loop nests): that is a missing capability, not an
absent limitation. **Use "✓ = the tool supports this feature" and fix the caption**; mark
untested entries "–".

Under that reading, the measured evidence is below. "Agrees" compares each observation with the
paper's mark.

<!-- BEGIN GENERATED: features -->
| Tool | Mark | Paper | Evidence | Agrees (tick = supports) | Example | Diagnostic |
|---|---|---|---|---|---|---|
| ScaleHLS | F1 (HLS-tool-specific constructs) | ✓ | limitation observed | no | `gemm_0001__fixed` | `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":828:9): error:  + visiting unknonwn sub decl stm...` |
| ScaleHLS | F2 (Dynamic loop nests) | ✓ | unverified | — |  |  |
| ScaleHLS | F3 (Memory and function hierarchy) | ✗ | limitation observed | yes | `gemm_0001__float` | `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::S...` |
| ScaleHLS | F4 (LLM operations) | ✗ | operation handled after edits | no | `rmsnorm__local` |  |
| ScaleHLS | F5 (Fixed-point math intrinsics) | ✗ | unverified | — |  |  |
| ScaleHLS | F6 (Complete pragma support) | ✗ | unverified | — |  |  |
| HIDA | F1 (HLS-tool-specific constructs) | ✓ | limitation observed | no | `gemm_0001__fixed` | `loc("<VITIS_INCLUDE>/etc/ap_fixed_base.h":828:9): error:  + visiting unknonwn sub decl stm...` |
| HIDA | F2 (Dynamic loop nests) | ✓ | limitation observed | no | `mha__noscalar_split` | `<RUN>/parsed.mlir:195:20: error: 'affine.load' op index must be a dimension or symbol iden...` |
| HIDA | F3 (Memory and function hierarchy) | ✗ | limitation observed | yes | `gemm_0001__float` | `scalehls-opt: <WORK><PATH>/Attributes.cpp:28: mlir::NamedAttribute::NamedAttribute(mlir::S...` |
| HIDA | F4 (LLM operations) | ✗ | operation handled after edits | no | `mha_rope__attn` |  |
| HIDA | F5 (Fixed-point math intrinsics) | ✗ | unverified | — |  |  |
| HIDA | F6 (Complete pragma support) | ✗ | unverified | — |  |  |
| AutoSA | F1 (HLS-tool-specific constructs) | ✓ | limitation observed | no | `gemm_0001__fixed` | `autosa: <PATH>/Decl.h:276: llvm::StringRef clang::NamedDecl::getName() const: Assertion `N...` |
| AutoSA | F2 (Dynamic loop nests) | ✗ | limitation observed | yes | `conv_k3__autosa_guard` | `what():  [AutoSA] Error: Non-uniform dependence detected.` |
| AutoSA | F3 (Memory and function hierarchy) | ✗ | unverified | — |  |  |
| AutoSA | F4 (LLM operations) | ✗ | unverified | — |  |  |
| AutoSA | F5 (Fixed-point math intrinsics) | ✗ | unverified | — |  |  |
| AutoSA | F6 (Complete pragma support) | ✗ | unverified | — |  |  |
| StreamHLS | F1 (HLS-tool-specific constructs) | ✓ | limitation observed | no | `gemm_0001__fixed` | `loc(callsite("<VITIS_INCLUDE>/etc/ap_private.h":2210:9 at "<VITIS_INCLUDE>/etc/ap_private....` |
| StreamHLS | F2 (Dynamic loop nests) | ✓ | unverified | — |  |  |
| StreamHLS | F3 (Memory and function hierarchy) | ✗ | unverified | — |  |  |
| StreamHLS | F4 (LLM operations) | ✗ | unverified | — |  |  |
| StreamHLS | F5 (Fixed-point math intrinsics) | ✗ | limitation observed | yes | `softmax__fixed` | `loc("<DESIGN>/top.cpp":45:28): error: 'math.exp' op operand #0 must be floating-point-like...` |
| StreamHLS | F6 (Complete pragma support) | ✗ | unverified | — |  |  |
<!-- END GENERATED: features -->

Notes on the table:
* The F1 ticks are contradicted for every tool: all four fail on the vendor `ap_fixed`/C++
  implementation.
* The F4 crosses are contradicted for HIDA and ScaleHLS: once the C-level constructs are removed,
  both compute LLM operators correctly (HIDA all four, ScaleHLS the norms).
* Marks with no direct observation stay "unverified". A construct merely appearing in the source
  is not a test.

### 3.2 AutoSA on DNN: how many designs out of how many?

* **Whole designs: 0 of 1.** `dnn_0001` fails while parsing the C++ headers, in fixed point and
  in float.
* **Single convolutions: 0 of 3.** On the two AutoSA accepts (1×1 and unpadded 3×3), with the
  edits that remove each blocker found (headers, `#pragma scop`, `+=` spelled out, inlined
  index temporaries, guard removed), **0 of 14 space-time candidates** produce a correct
  kernel. They compute garbage, emit code that does not compile, hang in C simulation, or crash
  AutoSA.
* **Padded 3×3:** AutoSA rejects it ("Non-uniform dependence detected").
* **AutoSA's own example** fails its golden check in the same image.
* **Historical:** the recovered DNN attempt did not use the ForgeBench design (§8).

The "subset" in the paper cannot be reproduced.

### 3.3 StreamHLS on DNN: what is the Vitis error?

There is none to report:
* **Whole design:** StreamHLS generates no code for `dnn_0001` at all. Its kernel pipeline
  crashes in transformation (`ilist_iterator` sentinel assertion), with or without edits.
* **Single convolutions:** it emits correct, synthesizable code for the two unpadded ones. For
  the padded 3×3, the code it emits compiles but computes wrong results (max_error 5.14; almost
  every output differs). The padding guard puts the input read under an `affine.if`, and its
  buffer-to-FIFO conversion mishandles that.

So "generates code, but the output is not synthesizable" is not what was observed: the observed
failure is "does not generate code" (whole design) or "generates wrong code" (padded operator).

### 3.4 LLM: what does each tool stop on when parsing?

On the generated fixed-point `llm_0001`, every tool stops in its C/C++ front end, as the paper
says, but on the vendor fixed-point implementation, not on an LLM operator:

| Tool | First error |
|---|---|
| ScaleHLS, HIDA | Polygeist rejects a local `enum` inside Vitis's `ap_fixed_base.h:1639` ("visiting unknown sub decl stmt") |
| StreamHLS | its Polygeist asserts in `ValueCategory::store` while lowering `ap_private`/`ap_int_base` operators; on fixed-point softmax it gets further and fails on `'math.exp' op operand #0 must be floating-point-like, but got '!llvm.struct<...>'`, i.e. `hls::exp` on an `ap_fixed` value (F5) |
| AutoSA | Clang 9 asserts in `NamedDecl::getName()` while visiting the C++ headers |

With the generator's `float` option, all tools except AutoSA get past parsing on every LLM
design. They fail later, for the reasons in §1, and HIDA passes with edits.

### 3.5 The construct behind each mark (one example each)

| Mark | Construct (example input) | Error |
|---|---|---|
| F1 | `typedef ap_fixed<16,5> data_t;` (every fixed-point design) | ScaleHLS/HIDA: `ap_fixed_base.h:828: visiting unknown sub decl stmt`; StreamHLS: invalid `arith.ori` lowered from `ap_private.h`; AutoSA: `NamedDecl::getName()` assertion |
| F2 | `int groups` run-time parameter → `heads_per_group = num_heads / groups` (`mha__noscalar_split`) | HIDA: `'affine.load' op index must be a dimension or symbol identifier` |
| F2 | padding guard `if (in_row >= 0 && …)` (`conv_k3__autosa_guard`) | AutoSA: `Non-uniform dependence detected` |
| F3 | file-scope `data_t BRAM_*[…];` written by `top()` (every generated design, float) | ScaleHLS/HIDA: `Attributes.cpp:28 … expected valid attribute name and value` (`ConvertGetGlobalToConstBuffer`) |
| F3 | one-element output port `DRAM_result[1]` (`gemm_0001__edited`) | HIDA emits it as a by-value `float`; the result is lost |
| F5 | `hls::exp` on `ap_fixed` (`softmax__fixed`) | StreamHLS: `'math.exp' op operand #0 must be floating-point-like` |

Beyond the F-categories, three tool defects decide most cells:

1. **ScaleHLS silently miscompiles** any loop nest where a loop-invariant operand is hoisted
   above the innermost loop. The output synthesizes but is wrong (GEMM `ijk`/`jik`/`jki`/`kji`,
   `gemm_0001`, `dnn_0001`, attention). It is correct whenever the reduction loop is innermost.
   The verdicts match the earlier exploratory runs, although its DSE picks a different tiling
   each time.
2. **HIDA's C++ emitter** cannot print loop-carried scalars (`iter_args`). Polygeist creates them
   from scalar accumulators and from array elements zeroed right before their reduction loop.
3. **StreamHLS's kernel pipeline** expects torch-mlir's linalg-derived IR:
   * perfect loop bands;
   * `memref.alloc`;
   * one function;
   * no guarded reads;
   * no row reductions in its performance model.

   Most C-derived IR violates one of these.

## 4. TODO item 3: per-design outcomes

[`outcomes.csv`](outcomes.csv) has one row per design × tool attempt, with the TODO's
columns `design_id,domain,tool,stage_reached,failure_category,error_head`, plus `run`, `patch` and
`functional_check`. `failure_category` is assigned by explicit rules in
`scripts/collect_tables.py` (vendor headers → F1, non-affine/run-time bounds → F2, global buffers
→ F3, `math.exp` on `ap_fixed` → F5). It is blank when the diagnostic is a tool crash that no
category explains. `error_head` is the first error line, or "wrong results: max_error …" for
output that computes the wrong result.

## 5. TODO item 4: re-runnable scripts and environment

**Environment:**
* Tools are built from the pinned commits by `scripts/checkout_tools.sh` and
  `scripts/build_mlir.sh`, inside `scripts/Dockerfile.build` (Ubuntu 22.04, Clang/LLD 14,
  CMake, Ninja; image `forgebench-tool-eval-build:20261016`).
* AutoSA runs in the pinned upstream image.
* Vitis HLS 2024.1 must be on `PATH`, with `VITIS_HLS_INCLUDE` set to its `include` directory.
* Python ≥ 3.9 with NumPy (for the generator).

```bash
tool_eval_work=$(mktemp -d)                                 # outside git, ~40 GB for the tool builds
bash results/tool_eval/scripts/checkout_tools.sh "$tool_eval_work"
docker build -t forgebench-tool-eval-build:20261016 -f results/tool_eval/scripts/Dockerfile.build results/tool_eval/scripts
for mode in scale stream stream-frontend; do                # 'scale' also builds HIDA
  docker run --rm --network none --user "$(id -u):$(id -g)" \
    --mount "type=bind,src=$tool_eval_work,dst=/work" \
    --mount "type=bind,src=$PWD/results/tool_eval/scripts,dst=/scripts,readonly" \
    -e JOBS=24 forgebench-tool-eval-build:20261016 bash /scripts/build_mlir.sh "$mode"
done
cd results/tool_eval/scripts
python minimal_inputs.py --work "$tool_eval_work/minimal"   # regenerate and patch all 67 inputs; checks hashes
python minimal_run.py --work "$tool_eval_work/minimal" --tools-work "$tool_eval_work" \
  --vitis-include "$VITIS_HLS_INCLUDE" --jobs 12           # every run in minimal/cases.json; resumable
python collect_tables.py && python feature_evidence.py && python report_tables.py --write
python -m unittest discover -s . -p test_evidence.py && python validate.py
```

**Scripts:**

| Script | Role |
|---|---|
| `run_mlir.py`, `run_autosa.py`, `run_vitis.py` | Single runs: one tool on one design directory. They record the stage reached, the first error, and the input hash, and refuse a zero exit with empty output as a pass. |
| `minimal_run.py --only <regex>` | Re-runs any subset of the matrix. |
| `minimal_check.py` | The functional comparison. It reads the generated `top.h`, so it works for any ForgeBench design. |

The primary unmodified runs of §2.4 were made with the same runners (`prepare.py`,
`collect.py`).

## 6. TODO item 5: QoR and full logs

### 6.1 Synthesis results

Correct tool output, synthesized with Vitis HLS 2024.1 for ZCU102 at 10 ns, compared with Vitis
on the same unmodified generated `float` design:

<!-- BEGIN GENERATED: qor -->
| Input | Tool | Latency (cycles) | Speed-up vs. Vitis | Est. clock (ns) | BRAM | DSP | FF | LUT |
|---|---|---|---|---|---|---|---|---|
| `gemm_0004__edited` | ScaleHLS | 750 | 38.67× | 7.016 | 32 | 118 | 29381 | 21201 |
| `llm_0001__edited` | HIDA (`place-external-buffer=false`) | 164804 | 3.68× | 7.240 | 17 | 78 | 10201 | 17893 |
| `gemm_ijk__local` | HIDA | 4460564 | 0.06× | 13.737 | 8 | 5 | 3476 | 3842 |
| `gemm_ikj__local` | ScaleHLS | 6227 |  | 7.016 | 342 | 215 | 38686 | 53362 |
| `gemm_ikj__local` | HIDA | 4460561 |  | 13.737 | 8 | 5 | 3412 | 3940 |
| `gemm_ikj__local` | StreamHLS (`--stream-polygeist-handoff`) | 790541 |  | 7.313 | 56 | 5 | 1302 | 1798 |
| `gemm_jik__local` | HIDA | 4460561 |  | 13.737 | 8 | 5 | 3444 | 3972 |
| `gemm_jki__local` | HIDA | 4460567 |  | 13.737 | 8 | 5 | 3438 | 4135 |
| `gemm_kij__local` | ScaleHLS | 6227 |  | 7.016 | 342 | 215 | 38686 | 53374 |
| `gemm_kij__local` | HIDA | 4460567 |  | 13.737 | 8 | 5 | 3374 | 4103 |
| `gemm_kij__local` | StreamHLS (`--stream-polygeist-handoff`) | 794640 |  | 7.313 | 64 | 5 | 1241 | 1752 |
| `gemm_kji__local` | HIDA | 4460560 |  | 13.737 | 8 | 5 | 3465 | 4187 |
| `conv_k1__sunkguard` | ScaleHLS | 3215 | 17.65× | 12.874 | 2912 | 80 | 155965 | 202559 |
| `conv_k1__sunkguard` | HIDA | 53732 | 1.06× | 7.300 | 6 | 7 | 2957 | 3318 |
| `conv_k1__sunkguard` | StreamHLS (`--stream-polygeist-handoff`) | 56464 | 1.01× | 7.016 | 49 | 5 | 1137 | 2117 |
| `conv_k3p0__sunkguard` | ScaleHLS | 62437 | 5.44× | 15.774 | 256 | 50 | 35797 | 58855 |
| `conv_k3p0__sunkguard` | HIDA | 337714 | 1.01× | 7.300 | 8 | 5 | 3885 | 4498 |
| `conv_k3__sunkguard` | ScaleHLS | 38908 | 12.41× | 9.169 | 448 | 68 | 29906 | 73238 |
| `conv_k3__sunkguard` | HIDA | 458357 | 1.05× | 7.300 | 8 | 7 | 3986 | 4596 |
| `conv_k1__noguard` | HIDA | 103528 | 0.55× | 7.300 | 6 | 8 | 3059 | 3466 |
| `conv_k1__noguard_reorder` | ScaleHLS | 2037 | 27.86× | 9.018 | 3136 | 226 | 408013 | 383772 |
| `conv_k1__noguard_reorder` | HIDA | 53732 | 1.06× | 7.300 | 6 | 7 | 2957 | 3318 |
| `conv_k1__noguard_reorder` | StreamHLS (`--stream-polygeist-handoff`) | 156816 | 0.36× | 7.313 | 51 | 5 | 1236 | 2203 |
| `conv_k3p0__noguard` | HIDA | 665893 | 0.51× | 7.300 | 8 | 6 | 3914 | 4715 |
| `conv_k3p0__noguard_reorder` | ScaleHLS | 10424 | 32.58× | 7.016 | 384 | 160 | 35949 | 54089 |
| `conv_k3p0__noguard_reorder` | HIDA | 337714 | 1.01× | 7.300 | 8 | 5 | 3885 | 4498 |
| `conv_k3p0__noguard_reorder` | StreamHLS (`--stream-polygeist-handoff`) | 1000784 | 0.34× | 7.313 | 68 | 5 | 1399 | 2964 |
| `conv_k3__sunkguard_reorder` | ScaleHLS | 35954 | 13.43× | 7.272 | 1568 | 76 | 135257 | 266020 |
| `conv_k3__sunkguard_reorder` | HIDA | 458357 | 1.05× | 7.300 | 8 | 7 | 3986 | 4596 |
| `rmsnorm__local` | ScaleHLS | 1564 | 4.24× | 7.057 | 64 | 28 | 3965 | 5218 |
| `softmax__local` | ScaleHLS | 1198 | 6.26× | 7.894 | 0 | 53 | 142068 | 142500 |
| `rmsnorm__rowacc_split` | ScaleHLS | 1729 | 3.84× | 7.057 | 68 | 26 | 3754 | 5250 |
| `rmsnorm__rowacc_split` | HIDA (`place-external-buffer=false`) | 4444 | 1.49× | 7.057 | 1 | 5 | 916 | 1009 |
| `softmax__rowacc_split` | ScaleHLS | 1596 | 4.70× | 7.350 | 12 | 44 | 159383 | 143358 |
| `softmax__rowacc_split` | HIDA (`place-external-buffer=false`) | 5448 | 1.38× | 7.057 | 3 | 9 | 1283 | 2291 |
| `mha__attn` | HIDA (`place-external-buffer=false`) | 155538 | 3.92× | 7.057 | 10 | 22 | 3862 | 5167 |
| `mha_rope__attn` | HIDA (`place-external-buffer=false`) | 157112 | 4.03× | 7.240 | 13 | 78 | 9031 | 16693 |

Vitis on the unmodified generated designs:

| Design | Type | Latency (cycles) | Est. clock (ns) | BRAM | DSP | FF | LUT |
|---|---|---|---|---|---|---|---|
| `gemm_0001` | `ap_fixed<16,5>` | 12485 | 7.300 | 14 | 3 | 2717 | 4177 |
| `gemm_0001` | `float` | 12625 | 7.300 | 26 | 5 | 3470 | 4449 |
| `gemm_0004` | `ap_fixed<16,5>` | 12485 | 7.300 | 14 | 3 | 3139 | 4473 |
| `gemm_0004` | `float` | 29005 | 7.300 | 26 | 5 | 3654 | 4729 |
| `dnn_0001` | `ap_fixed<16,5>` | 1154734 | 7.300 | 164 | 7 | 5265 | 8850 |
| `dnn_0001` | `float` | 1167518 | 7.300 | 299 | 14 | 4715 | 6297 |
| `llm_0001` | `ap_fixed<16,5>` | 180348 | 7.300 | 75 | 58 | 28087 | 33529 |
| `llm_0001` | `float` | 607028 | 7.300 | 125 | 145 | 24010 | 33867 |
| `gemm_ijk` | `ap_fixed<16,5>` | 274475 | 7.300 | 14 | 1 | 2135 | 3431 |
| `gemm_ijk` | `float` | 274480 | 7.300 | 28 | 5 | 2759 | 3650 |
| `conv_k1` | `ap_fixed<16,5>` | 56747 | 7.300 | 11 | 1 | 2151 | 3903 |
| `conv_k1` | `float` | 56752 | 7.300 | 21 | 5 | 2775 | 4122 |
| `conv_k3p0` | `ap_fixed<16,5>` | 339564 | 7.300 | 14 | 1 | 2214 | 4423 |
| `conv_k3p0` | `float` | 339569 | 7.300 | 28 | 5 | 2838 | 4642 |
| `conv_k3` | `ap_fixed<16,5>` | 474142 | 7.300 | 14 | 1 | 2248 | 4215 |
| `conv_k3` | `float` | 482962 | 7.300 | 28 | 5 | 2872 | 4434 |
| `rmsnorm` | `ap_fixed<16,5>` | 4750 | 7.300 | 5 | 2 | 6506 | 8176 |
| `rmsnorm` | `float` | 6638 | 7.300 | 9 | 5 | 2871 | 3383 |
| `softmax` | `ap_fixed<16,5>` | 5505 | 7.300 | 4 | 3 | 4146 | 4675 |
| `softmax` | `float` | 7505 | 7.300 | 8 | 9 | 3006 | 4134 |
| `mha` | `ap_fixed<16,5>` | 192087 | 7.300 | 62 | 36 | 19611 | 20319 |
| `mha` | `float` | 610355 | 7.300 | 110 | 112 | 16361 | 20386 |
| `mha_rope` | `ap_fixed<16,5>` | 210548 | 7.300 | 67 | 56 | 21796 | 25304 |
| `mha_rope` | `float` | 633052 | 7.300 | 111 | 145 | 19273 | 29004 |
<!-- END GENERATED: qor -->

* **Settings are not matched:** ScaleHLS's DSE uses its own PolyBench resource specification,
  and StreamHLS runs without its AMPL/Gurobi schedule solver. These are therefore not
  matched-resource comparisons.
* **Where the reports are:**
  * `csynth.xml` of the unmodified sweep designs: `reports/vitis/<id>/`.
  * `csynth.xml` of every tool run: in the tarball, since the repository is kept under 5 MB.

### 6.2 Full logs

The full redacted transcripts (primary runs, controls, builds, every minimal-study run) are in
`tool_eval_logs.tar.gz`, built outside git by `scripts/archive_logs.py`. Its name, SHA-256 and
size are in [`archive.json`](archive.json), and it is intended for the Zenodo bundle. Nothing in
git contains local paths, host names, licenses or build trees (checked by `validate.py`).

## 7. Suggestions for revising the paper

1. **Table 9 cells: rate what the tools actually do, and say at which level.** With the paper's
   own legend:
   * **Whole generated designs, as generated:** ○ for every tool and domain.
   * **Whole generated designs with minor non-algorithmic edits, float:**
     * GEMM: ◐ ScaleHLS; ○ HIDA, StreamHLS, AutoSA.
     * DNN: ○ for all.
     * LLM: ◐ HIDA; ○ ScaleHLS, StreamHLS, AutoSA.
   * **Single operators with minor edits** (optional sub-row; strengthens the point):
     * HIDA ◐ on every operator tried;
     * ScaleHLS ◐ on GEMM (reduction-innermost orders), convolutions and norms;
     * StreamHLS ◐ on GEMM (reduction-innermost orders) and the unpadded convolutions;
     * AutoSA ○.

   The current ● for ScaleHLS/HIDA GEMM is not supported: no tool synthesizes an unmodified
   generated design.
2. **Prose of Case Study 3:**
   * "On GEMM, all four tools reach synthesis" → only after edits, in float, and for three tools.
     ScaleHLS and StreamHLS also need the reduction loop innermost. AutoSA produces no correct
     kernel.
   * "ScaleHLS and HIDA advance only to the transform level" on DNN → true for the generated
     design. After small edits both compute every single convolution correctly, but neither
     passes the whole conv + BN + ReLU design.
   * "StreamHLS generates code but the output is not synthesizable" → it generates no code for
     the DNN design. For a padded convolution it generates code that computes wrong results.
   * "AutoSA synthesizes only a subset" → not reproducible (0 of 1 design, 0 of 14 kernels);
     AutoSA's own example fails its check in the evaluated image.
   * "On LLM designs, every tool halts at parsing" → true for the generated fixed-point design;
     the parse failure is in the vendor `ap_fixed` headers, not an LLM operator. In float, HIDA
     compiles the whole attention + LayerNorm design correctly after edits.
3. **Caption and setup:**
   * define the tick polarity ("✓ = supported");
   * state the tool commits (§2.2), the data type used for each result, and that results were
     checked for functional equivalence;
   * define ◐ concretely: the edits are documented, non-algorithmic, and listed in the artifact.
4. **F-marks:** keep only marks that have an observation (§3.1, §3.5), and add the example
   construct for each.
   * F1: change to ✗ for all tools.
   * F2: add HIDA ✗ (observed).
   * F3: ✗ is supported for ScaleHLS/HIDA.
   * F5: ✗ is observed for StreamHLS.
   * F4: ✗ is contradicted for HIDA and ScaleHLS.
   * Mark the rest "–" (untested), or drop them.
5. **Add the stronger finding:** silent miscompilation. ScaleHLS emits synthesizable C++ that
   computes the wrong result for common loop orders (GEMM `ijk`, attention). StreamHLS does the
   same for padded convolutions. Without a functional check these would have been counted as
   successes. That is a concrete argument for ForgeBench's generated designs *with* golden
   references, more convincing than parse failures.
6. **Say what blocks the tools:** most failures are vendor C++ (`ap_fixed`), file-scope
   buffers, loop-carried scalars, and run-time trip counts. These are C/HLS idioms, not the
   operators. This supports the paper's message that the tools cannot be applied out of the box,
   and is more precise than "deeper hierarchies and modern operators".

## 8. What is missing, and discrepancies

* **The original evaluation cannot be reconstructed.** Its cohort, tool versions and
  run artifacts were deleted. The results here are a fresh evaluation and must not be presented
  as the July measurements.
* **The recovered AutoSA artifacts (historical) are rewrites:**
  * GEMM `diff_dims_p1`: synthesized as `kernel0` but failed co-simulation (`malloc(): unsorted
    double linked list corrupted`). Its input is a `float`, interface-free, re-nested rewrite;
    see [`patches/autosa/diff_dims_p1-historical-rewrite.patch`](patches/autosa/diff_dims_p1-historical-rewrite.patch)
    and `reports/historical/`.
  * DNN `conv_A`: no successful synthesis. Two attempts ran out of disk; the recovered command
    used AutoSA's own `autosa_tests/large/cnn/kernel.c`, whose dimensions do not match
    `conv_A`. Provenance: [`historical/manifest.json`](historical/manifest.json).
* **Two upstream controls were weaker than they looked:**
  * AutoSA's control only checked that a file was emitted; its golden check fails.
  * StreamHLS's control used the base pipeline; its dataflow kernel pipeline needs the IR
    handoff.

  The earlier version of this report relied on both controls; this version supersedes it.
* **Coverage limits:**
  * one sweep design per domain (plus `gemm_0004`);
  * batch norm, activations other than ReLU, LayerNorm alone, sliding-window attention and the
    full ResNet/LLaMA models were not tested separately;
  * all passing results are in `float`, because no tool accepts the generated `ap_fixed` code;
  * re-typing emitted float code to `ap_fixed` would not reproduce ForgeBench's arithmetic, so
    no fixed-point result is claimed.
* **ScaleHLS's DSE is not deterministic.** Its tiling differs between runs, and so do the
  numbers in §6.1. The pass/fail verdicts were stable across reruns. Some ScaleHLS runs needed
  30-minute DSE or 4-hour synthesis limits (`minimal/cases.json`).
* **Note for the generator:** a conv config with `with_bias = false` must list three `args`. A
  fourth is accepted silently and produces a call that does not compile.
