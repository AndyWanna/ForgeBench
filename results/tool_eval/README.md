# Source-to-source tool evaluation

This directory contains a **fresh, explicitly identified evaluation**, plus
recovered historical AutoSA artifacts. It is not a reconstruction of the July
evaluation. The original ScaleHLS, HIDA, and StreamHLS installations and run
artifacts were deleted, and their historical commits and input cohorts are
unknown. New runs must not be presented as those original measurements.

The paper under review is `Forgebench_FPT_Journal_2026_R2.pdf`, Case Study 3,
Table 9 (`tab:tool-support`); the corresponding manuscript files are
`sec/results.tex` and `table/testcases_table.tex`. No manuscript files were
modified. Corrections and unresolved claims are recorded here for review.

## Result at a glance

**The fresh evaluation does not reproduce any ● or ◐ cell of Table 9.** On the
unchanged generated designs, every tool stops in its C/C++ front end, in all
three domains. Changing only `data_type` to `float` lets ScaleHLS, HIDA and
StreamHLS import the designs, but each then fails in its transformation
pipeline. No tool reaches code generation or synthesis on any input.

| Tool | Paper GEMM / DNN / LLM | Fresh `ap_fixed<16,5>` (stage, rating) | Fresh `float` (stage, rating) |
|---|---|---|---|
| ScaleHLS | ● / ○ / ○ | parse ○ / parse ○ / parse ○ | transform ○ / transform ○ / transform ○ |
| HIDA | ● / ○ / ○ | parse ○ / parse ○ / parse ○ | transform ○ / transform ○ / transform ○ |
| AutoSA | ◐ / ◐ / ○ | parse ○ / parse ○ / parse ○ | parse ○ / parse ○ / parse ○ |
| StreamHLS | ◐ / ○ / ○ | parse ○ / parse ○ / parse ○ | transform ○ / transform ○ / transform ○ |

`stage_reached` is the stage that was attempted and failed (see stage semantics
below). The paper's prose claims that differ from these results are: "On GEMM,
all four tools reach synthesis"; "ScaleHLS and HIDA advance only to the
transform level" on DNN (fresh: parse for the fixed-point input; transform only
after the float change); "StreamHLS generates code but the output is not
synthesizable" (fresh: no code is generated); and "AutoSA synthesizes only a
subset of designs" (fresh: 0 of 1). The LLM row ("every tool halts at parsing")
agrees with the fixed-point results. The fresh cohort is one design per domain
and the tool revisions are current upstream, so these results do not show that
the July results were wrong. They do show that the July results cannot be
reproduced from the generated sources and that their inputs are unknown (see
[Recovered historical evidence](#recovered-historical-evidence-and-discrepancies)).
Each upstream tool passes its own example (`logs/environment/control-*.log`),
so these failures are not caused by broken builds.

## Fresh experiment

The fixed cohort is the first design in each released sweep manifest:

| Domain | Design ID | Input |
|---|---|---|
| GEMM | `gemm_0001` | Option-1 composition of vector/matrix products and a dot product, dimensions 64 |
| DNN | `dnn_0001` | Convolution with batch normalization and ReLU; 16 input/output channels, 56-by-56 input, kernel size 1 |
| LLM | `llm_0001` | Grouped multi-head attention with RoPE and LayerNorm; sequence 8, dimension 128, 8 heads, head dimension 16 |

These are whole generated sweep designs using `ap_fixed<16,5>`, including
their generated load/store functions, memory interfaces, and pragmas. There is
one input per domain per tool, not an exhaustive sweep and not a full ResNet or
32-layer LLaMA model. In particular, GEMM's first manifest case is a composed
linear-algebra workload, not an isolated three-loop matrix multiplication.

[`designs/manifest.json`](designs/manifest.json) records the generator commit,
original config paths, config hashes, and hashes of every retained generator
output. The exact configs and unmodified sources are in `designs/<design_id>/`.
`scripts/prepare.py` regenerates them using ForgeBench's own generator in an
external working directory. No verification shim, handwritten replacement
kernel, type substitution, or operator removal is used in the primary runs.

[`summary.csv`](summary.csv) has the requested twelve tool/domain rows;
[`outcomes.csv`](outcomes.csv) has twelve design/tool rows. The richer
[`runs.json`](runs.json) and per-cell `result.json` files retain status, exit
codes, source hashes, and image identity. Every cell links to a command and
transcript under `logs/<tool>/<domain>/`.

**Stage semantics:** `parse` includes importing C/C++ into MLIR or AutoSA's
front-end representation. On failure it denotes the stage attempted, not a
successful parse. `transform` and `codegen` likewise identify an attempted
stage; the JSON `status` distinguishes passing from failing it. `synth` is
reserved for completed Vitis C synthesis with a nonempty report. A zero
frontend exit with no output, a timeout, or a missing executable is not evidence
of tool support. `full` requires synthesis without source edits; `half` requires
synthesis after documented non-algorithmic edits. No QoR improvement or
functional-equivalence claim follows from synthesis alone.

All three unmodified inputs passed direct Vitis HLS 2024.1.2 C synthesis on
`xczu9eg-ffvb1156-2-e`, clock period 10 ns. Their reports are in
`reports/vitis/<design_id>/csynth.xml` and measurements in
[`baseline_vitis.csv`](baseline_vitis.csv). These baseline passes do not count
as source-to-source tool passes.

## User-requested float comparison

The same three configurations were regenerated with only `data_type` changed
to `float`. Dimensions, operations, memory hierarchy, and scheduling settings
in the configs are unchanged. The float sources are unedited generator output
under `designs/<parent_id>_float/`; their provenance is in
[`designs/float_manifest.json`](designs/float_manifest.json). Exact generated
source differences are retained under `patches/float/`. This precision change
is **not** a minor non-algorithmic adapter or a numerical-equivalence claim.

The separate [`float_summary.csv`](float_summary.csv),
[`float_outcomes.csv`](float_outcomes.csv), and
[`float_baseline_vitis.csv`](float_baseline_vitis.csv) must not be substituted
for the fixed-point experiment. All three float originals also pass direct
Vitis C synthesis.

ScaleHLS and HIDA import all three float inputs, but fail during transformation.
The symbolized backtraces identify `ConvertGetGlobalToConstBuffer`, which passes
`global.getConstantInitValue()` to `hls.const_buffer` without checking whether
an initializer exists. The imported BRAM globals have `= uninitialized` in
MLIR, producing the assertion “expected valid attribute name and value”. This
is an observed global-memory conversion limitation (F3), not a failure to
represent `float`. See `logs/<tool>/<domain>/float-symbolized.log` and the pinned
[ScaleHLS conversion source](https://github.com/UIUC-ChenLab/scalehls/blob/b69f334c9c122fad3759fcf388d3f039689b6fc0/lib/Transforms/Dataflow/BufferizeDataflow.cpp#L128).
The generated C++ globals have normal C++ static-storage semantics; the
`uninitialized` wording describes the imported MLIR, not a claim that the C++
generator uses uninitialized global storage.

AutoSA still asserts while visiting C++ headers. The float generator retains
its standard includes, including the unused `ap_fixed.h`; changing the scalar
type alone does not remove that header compatibility problem. This unchanged
float experiment does not remove headers or replace HLS math calls.

**What data types are supported?** Type representation and whole-source
compatibility are different questions. The pinned ScaleHLS/HIDA emitters
explicitly handle `f32`/`float`, `f64`/`double`, index, and integer types; see
their `lib/Translation/EmitHLSCpp.cpp`. The StreamHLS Vivado emitter likewise
handles floating-point and integer scalar types in
`lib/Translation/EmitVivadoHLS.cpp`. All four upstream controls use floating-point
kernels. None of this guarantees that all C++ library implementations or all
operations on those types can be imported, transformed, and synthesized.
In particular, emitting `ap_int` for an integer IR value does not establish
the ability to import an arbitrary vendor `ap_fixed` C++ class implementation.

To regenerate and run the float comparison:

```bash
python results/tool_eval/scripts/prepare_float.py --work "$tool_eval_work/generated-float"
python results/tool_eval/scripts/run_mlir.py scalehls \
  results/tool_eval/designs/gemm_0001_float --domain gemm --design-id gemm_0001_float \
  --work "$tool_eval_work" --vitis-include "$VITIS_HLS_INCLUDE" \
  --out "$tool_eval_work/runs-float/scalehls/gemm_0001_float"
```

Repeat for each tool/domain, using `run_autosa.py` for AutoSA. Run the float
Vitis baselines with `run_vitis.py`, then `collect_float.py --work ...`.
ScaleHLS uses its upstream PolyBench DSE configuration (220-DSP resource limit)
in these attempts; Vitis uses the common ZCU102/10 ns target. No matched-budget
QoR improvement is claimed.

## Recovered historical evidence and discrepancies

The old AutoSA workspace contains `diff_dims_p1` (GEMM) and `conv_A` (DNN).
Their pristine generated `top.cpp` files were recovered from ForgeBench commit
`d2a26d3dd9c351678fa7b9a04775464325639e1f`, not from the modified working copies.
Configs are under `designs/historical/`; provenance is in
[`historical/manifest.json`](historical/manifest.json).

* **GEMM:** the recovered Vitis run completed synthesis of
  `top_gemm_new_ijk_bias_kernel.cpp` as `kernel0`, then failed co-simulation with
  `malloc(): unsorted double linked list corrupted` and `COSIM 212-360`. Its
  `csynth.xml` is retained under `reports/historical/autosa/diff_dims_p1/`.
  The corresponding hand-adapted input uses `float` instead of `ap_fixed`,
  removes generated memory interfaces and load/store functions, changes the
  loop nesting, and supplies a standalone host harness. The recovered rewrite
  cannot substantiate the paper's claim of only minor non-algorithmic changes.
  The exact historical AutoSA command producing this output was not recovered.
* **DNN:** no successful `csynth.xml` was recovered. Two synthesis projects
  record `LLVM ERROR: IO failure on output stream: No space left on device`.
  That is an infrastructure failure, not evidence that the DNN is unsupported.
  The last top-level transcript terminates during source analysis with
  `command 'ap_source' returned error code`; its nested compiler log is empty.
  These are distinct historical attempts and are retained separately.
* **DNN provenance:** the recovered `autosa.tmp/output/src/cmd` invokes the
  upstream `autosa_tests/large/cnn/kernel.c`, whose recovered output header
  defines 640 output channels, 512 input channels, and a 56-by-56 output.
  Those dimensions do not match ForgeBench `conv_A` (64 channels, 14-by-14).
  Other local input variants also change layout, input shape, padding behavior,
  and precision. Directory co-location is insufficient to count these as
  successful transformations of `conv_A`.

`patches/autosa/*-historical-rewrite.patch` makes the recovered changes
reviewable against the pristine generated sources. These are **evidence of
rewrites**, not approved minor adapters or claimed equivalent implementations.
The DNN patch names the recovered variant explicitly; it is not asserted to be
the input that produced either of the recovered generated kernels. There is
no recovered StreamHLS GEMM patch. Inventing one would not repair the historical
provenance gap.

## Answers to the five table questions

1. **Meaning of F1–F6 ticks:** the active table header says “HLS Support”, so
   the natural reading is “supports the feature”; the prose instead calls the
   columns limitation categories and never defines the tick polarity. The
   intended historical convention cannot be established from the available
   artifacts. Only the support reading is internally consistent: AutoSA, the
   one polyhedral tool, is the only tool with a cross in F2 (dynamic loop
   nests), and that is a support gap, not an absent limitation. Under that
   reading, however, the F1 ticks for all four tools are contradicted by the
   fresh runs: every MLIR-based tool fails importing Vitis's `ap_fixed`
   implementation (F1-attributed in `outcomes.csv`). Use an explicit support
   convention in a revised caption:
   “A tick denotes a demonstrated feature probe pass; a cross denotes a
   demonstrated failure; a dash denotes untested or blocked by an earlier
   failure.” Do not mechanically invert the current marks.
2. **AutoSA DNN subset size:** the historical numerator and denominator are
   unknown. There are zero demonstrated successful full generated DNN inputs
   in the recovered artifacts. In the fresh cohort AutoSA synthesizes 0 of 1
   DNN designs (`dnn_0001` and its float variant both fail in the front end).
   A historical subset count must not be assigned from the presence of
   unrelated generated CNN code.
3. **StreamHLS DNN Vitis error:** no historical StreamHLS output or Vitis log
   survived. In the fresh runs StreamHLS generates no code for the DNN design:
   the fixed-point input fails in its Polygeist front end
   (`ValueCategory::store` assertion `toStore.isReference`), and the float
   input fails in `streamhls-opt` (`ilist_iterator` sentinel assertion). So no
   Vitis error exists to report, and the claim that StreamHLS generates
   unsynthesizable code is not backed.
4. **LLM parsing failures:** the exact first diagnostic for each tool is in
   `outcomes.csv` and its per-cell log. The AutoSA raw-input failure is
   `clang::NamedDecl::getName()` / “Name is not a simple identifier” while
   visiting a C++ header. It is not an observed failure of RoPE, attention,
   LayerNorm, or another LLM operator. ScaleHLS and HIDA reject an `EnumDecl`
   in `ap_fixed_base.h:1639` while importing the LLM input; the GEMM and DNN
   cases hit other local enums in the same header. StreamHLS's Polygeist
   front end aborts with `ValueCategory::store` assertion `toStore.isReference`
   (DNN fails the same way; GEMM instead fails earlier on an invalid
   `arith.ori` lowered from `ap_private.h`). The last expressions it reports
   before the abort are `ap_private`/`ap_int_base` operator calls in the vendor
   headers, but the assertion does not name the construct, so
   `failure_category` stays blank. These are failures in
   importing vendor fixed-point C++ implementation code, not demonstrated
   failures of the named LLM operations. With `float`, the LLM design gets past
   import in ScaleHLS, HIDA and StreamHLS and fails in transformation instead,
   so in the float variant "halts at parsing" holds only for AutoSA.
5. **Evidence for every F mark:** broad marks cannot be established by a
   shared early failure. F2–F6 require independent probes that actually reach
   dynamic-loop handling, hierarchy, LLM operators, fixed-point intrinsics,
   and pragma preservation. Their presence in the source is not a test of
   support. Leave these marks unverified where no isolated probe exists;
   `failure_category` is blank when the diagnostic cannot establish one.
   [`feature_evidence.csv`](feature_evidence.csv) inventories all 24 existing
   marks, links example constructs and blocking logs, and distinguishes the
   observed F1 vendor-header failures from unverified features. F1 attribution
   is narrow: it does not mean every HLS-specific construct is unsupported.

The fresh unchanged-input results and the recovered rewrites do not by
themselves establish the original GEMM synthesis, AutoSA DNN subset, or
StreamHLS DNN codegen claims. Any revised statement must name the evaluated
cohort, precision, revisions, and tested input path. The original cohort and
its precise tool versions remain missing.

## Tool versions and build environment

[`versions.json`](versions.json) pins the fresh tool sources and executable
hashes. The relevant sources are:

| Tool | Repository | Fresh commit / dependency |
|---|---|---|
| ScaleHLS | [UIUC-ChenLab/scalehls](https://github.com/UIUC-ChenLab/scalehls) | `b69f334c9c122fad3759fcf388d3f039689b6fc0` |
| HIDA | [UIUC-ChenLab/ScaleHLS-HIDA](https://github.com/UIUC-ChenLab/ScaleHLS-HIDA) | `fd79f6c946593c290d6d285c2a2efcab72fbe9c6` |
| AutoSA | [UCLA-VAST/AutoSA](https://github.com/UCLA-VAST/AutoSA) | `b61a1b4132d631600696feba59eb606acb34d304` |
| StreamHLS | [UCLA-VAST/Stream-HLS](https://github.com/UCLA-VAST/Stream-HLS) | `00c6219a82fc710492f4eb44e4f34836dfb19e08` |

ScaleHLS and HIDA both pin Polygeist
`737290e9c8fc87473a101472ea66b2e0e33634d2` and its LLVM/MLIR 16.0.0git fork
`cf0ff67a7b051ed44b7d689eec73b12990676013`; they share that dependency build.
StreamHLS pins LLVM/MLIR 18.1.2 at
`26a1d6601d727a96f4301d0d8647b5a42760ae0c`. Its optional C/C++ front end is
Polygeist `77c04bb2a7a2406ca9480bcc9e729b07d2c8d077`, compiled against its own
LLVM/MLIR 18.0.0git dependency `26eb4285b56edd8c897642078d91f16ff0fd3472`.
Building this Polygeist against 18.1.2 failed because of incompatible LLVM/MLIR
APIs; that setup attempt is retained in the archive, not counted as a benchmark
failure. This front-end selection is an explicit fresh-run choice; StreamHLS
does not pin Polygeist itself. The frontend/backend handoff is checked using
the upstream control.

AutoSA runs in the upstream documented image pinned by content digest:
`whbldhwj/autosa@sha256:b9318c507d8e9876e363ad2cb3a7913ef743a95eb02517a538ea7f8ec0225b72`.
Its reported front-end versions are Clang `9.0.1-12`, PET
`pet-0.11.3-4-gf21882c`, and ISL `isl-0.22-GMP`. Its source tree has two
pre-existing differences from the named commit: whitespace-only changes in
`src/autosa_codegen.cpp`, and changed dimensions in an upstream CNN example.
The image digest, rather than a claim of a completely clean source checkout,
identifies the executable environment. The upstream matrix-multiplication
control successfully emits HLS C++.

The MLIR build image is Ubuntu 22.04 with Clang/LLD 14, CMake 3.22, Ninja,
GCC's C++ headers, Python, GMP, Boost, and zlib. The script uses optimized LLVM
builds with assertions enabled, X86 only, and no Python bindings. All compiler
build trees stay outside git. The unified builds disable an explicit Ninja
link pool because the pinned external projects otherwise register that pool
twice. StreamHLS's CMake forces Debug; its standalone compilation uses
`-O1 -g0` to avoid large debug objects while retaining assertions.
HIDA's two CMake executable targets are renamed to avoid collisions with
imported ScaleHLS targets; the executable filenames and compiler algorithms
are unchanged. The exact build-only patch is
[`patches/build/hida-target-names.patch`](patches/build/hida-target-names.patch).

The primary MLIR front-end invocation uses normal C++ mode and real Vitis
headers. `__SYNTHESIS__` is not defined: a standard Clang syntax-only check
accepts the unmodified GEMM source in normal mode, whereas defining the macro
requires Vitis-specific built-ins and introduces unrelated header errors.
No vendor headers or license material are redistributed.

The runners discover the top function's mangled C++ name using a Clang AST
query. This changes only the command-line selector, not the input. They also
require Clang's resource headers, reject compiler errors even with exit code
zero, and check for the requested function definition in MLIR. Initial setup
attempts with missing resource headers or a plain `-function=top` selector
emitted empty modules; those attempts are excluded, not counted as codegen
passes. Regression tests cover these false-positive risks.

AutoSA supplemental diagnostics remove only unused `iostream`, `fstream`,
and `cstdlib` includes, with the exact patches in `patches/autosa/`. Both normal
and synthesis-macro header modes are tested. The latter errors on `__fp16`
declarations and vendor built-ins; this mode is a compatibility diagnostic,
not the primary result or an LLM-operator test. The patches do not earn a
`half` rating without a successful full flow.

StreamHLS's configured post-import pipeline uses the default disabled solver
optimizations; AMPL/Gurobi are not supplied by the build image. A front-end
failure occurs before those options matter. Optimization quality and the
licensed solver flow are not evaluated here.

## Re-running

Run from the ForgeBench repository root. Use a new external working directory
on a filesystem with enough space for LLVM builds, and a licensed Vitis HLS
installation on `PATH`. Set `VITIS_HLS_INCLUDE` to its `include` directory.
The evidence scripts require Python 3.9 or newer; the generator also needs
NumPy. The recorded generation used
Python 3.9.13 and NumPy 1.24.4. Follow the repository's normal installation
requirements when installing ForgeBench as a package.

```bash
tool_eval_repo=$PWD
tool_eval_work=$(mktemp -d)
bash results/tool_eval/scripts/checkout_tools.sh "$tool_eval_work"
docker build -t forgebench-tool-eval-build:20261016 \
  -f results/tool_eval/scripts/Dockerfile.build results/tool_eval/scripts

# Invoke with 'scale' (also builds HIDA), 'stream', and 'stream-frontend'.
# Independent builds may run concurrently if resources permit.
for mode in scale stream stream-frontend; do
docker run --rm --network none --user "$(id -u):$(id -g)" \
  --mount "type=bind,src=$tool_eval_work,dst=/work" \
  --mount "type=bind,src=$tool_eval_repo/results/tool_eval/scripts,dst=/scripts,readonly" \
  -e JOBS=24 forgebench-tool-eval-build:20261016 bash /scripts/build_mlir.sh "$mode"
done

python results/tool_eval/scripts/prepare.py --work "$tool_eval_work/generated"

# For each of scalehls, hida, streamhls and each domain's design directory:
python results/tool_eval/scripts/run_mlir.py scalehls \
  results/tool_eval/designs/gemm_0001 --domain gemm --design-id gemm_0001 \
  --work "$tool_eval_work" --vitis-include "$VITIS_HLS_INCLUDE" \
  --out "$tool_eval_work/runs/scalehls/gemm_0001"

python results/tool_eval/scripts/run_autosa.py results/tool_eval/designs/gemm_0001 \
  --domain gemm --design-id gemm_0001 --vitis-include "$VITIS_HLS_INCLUDE" \
  --out "$tool_eval_work/runs/autosa-final/gemm_0001"
```

Repeat the tool commands for `dnn_0001` and `llm_0001`. The runners refuse to
overwrite existing run directories and preserve unchanged inputs. Inspect
`result.json`; a script finishing is not itself evidence that the tool passed.
If code generation succeeds, the MLIR runner explicitly reports
`codegen_requires_synthesis`; it does not silently award synthesis credit.

For byte-identical regeneration, use the generator commit recorded in
`designs/manifest.json`. The retained input hashes are the authoritative link
between the generated code and the runs. Randomized testbench data is kept
outside git; no C simulation or equivalence claim is made for this evaluation.

`run_vitis.py` takes the full generated design directory (including its DRAM
text files) and a fresh `--out` directory. `diagnose_autosa.py --work ...
--vitis-include ...` runs the supplemental header diagnostics. Upstream controls
are provided as `control_autosa.sh` and `control_mlir.sh`; their inputs are not
part of the ForgeBench result cohort. `recover_autosa.py` optionally reimports
the old workspace if it is available; the public historical snapshots do not
require that workspace to inspect.

For example, run an upstream control with:

```bash
python results/tool_eval/scripts/run_control.py scalehls \
  --work "$tool_eval_work" --out "$tool_eval_work/controls/scalehls"
```

Repeat for the other three tools. A control validates import/transformation/
emission, not FPGA synthesis, functional equivalence, or performance.

After all twelve runs and the three baselines are available, `collect.py
--work ...` generates the tables and redacted public excerpts (`collect_float.py
--work ...` does the same for the float runs). Then run:

```bash
python -m unittest discover -s results/tool_eval/scripts -p test_evidence.py -v
python results/tool_eval/scripts/feature_evidence.py
python results/tool_eval/scripts/validate.py
```

## Remaining evidence and archive

The original twelve-cell historical cohort, original versions of the three
deleted tools, historical StreamHLS Vitis error, successful AutoSA DNN
numerator/denominator, and a validated StreamHLS GEMM minor-edit patch remain
unrecoverable from the supplied material. Independent F1–F6 pass/fail probes
are also required before retaining all 24 feature marks. These gaps must remain
visible even after the fresh twelve-cell evaluation is complete.

The historical GEMM synthesis report is not a matched QoR comparison: its
floating-point rewrite differs from the generated fixed-point design. The
fresh Vitis reports establish input synthesizability and baseline resource
numbers, not a speedup for any source-to-source tool.

Full redacted transcripts and build logs are staged in the external working
directory's `archive/` and packaged outside git by `scripts/archive_logs.py
--work ... --out <outside-git>/tool_eval_logs.tar.gz`. The tarball's name,
SHA-256 and size are in [`archive.json`](archive.json); it is not in git and
must be added to the Zenodo bundle separately. Only small excerpts, configs,
sources, patches, and XML reports belong here. The release bundle already
includes `results/tool_eval/`; compiler trees and raw private machine logs must
not be added to it.

The repository's top-level `.gitignore` ignores `*.log`; the
`.gitignore` in this directory re-includes it so the per-cell transcripts under
`logs/` are tracked.

### Status against the TODO

| TODO item | Status |
|---|---|
| 1. Designs fed to each tool | Done for the fresh cohort (`designs/manifest.json`); historical cohort unknown |
| 1. Tool versions | Done for the fresh runs (`versions.json`); historical versions unknown |
| 1. `summary.csv`, one log per cell | Done (12 rows, `logs/<tool>/<domain>/`) |
| 1. Minor-edit patches for ◐ cells | No fresh cell earned ◐. Historical AutoSA rewrites are in `patches/autosa/` and are not minor; no StreamHLS GEMM patch exists |
| 2. Open questions | Answered above, with the gaps stated |
| 3. `outcomes.csv` | Done (12 rows; float variant in `float_outcomes.csv`) |
| 4. Scripts and environment | Done (`scripts/`, build image `scripts/Dockerfile.build`, AutoSA image digest) |
| 5. csynth reports | Original-Vitis reports for all six inputs; no tool output reached synthesis, so there are no tool reports to compare against |
| 5. Full logs tarball | Built outside git (`archive.json`) |
