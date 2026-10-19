# Source-to-source tool evaluation (Case Study 3, Table 9)

This directory holds the evaluation of ScaleHLS, HIDA, AutoSA and StreamHLS on ForgeBench-generated
GEMM, DNN and LLM designs behind Table 9 (`tab:tool-support`) and Case Study 3 of the paper. Every
cell of the table is derived from the run records by `scripts/paper_table.py`; [`table/`](table/README.md)
traces each cell to its runs, and each run to its input, emitted code and logs.

## What was run

* **Whole designs:** the base design of each domain in the ML test suite, the first design of
  each domain in `manifest/designs/`:
  * `gemm_0001`: x·A·B·y with 64-element vectors and 64×64 matrices (two vector-matrix products
    and a dot product, loop order `ij`), and its `ji` loop-order version `gemm_0004`;
  * `dnn_0001`: a 1×1 convolution, 16→16 channels on 56×56, with padding 1, batch norm and ReLU;
  * `llm_0001`: attention over 8 tokens of width 128 (8 heads of 16) with RoPE and LayerNorm.
* **Single operators** ([`tool_run_log/configs/`](tool_run_log/configs/)), 13 in all: GEMM
  (64×64×64) in all six loop orders; 1×1, unpadded 3×3 and padded 3×3 convolutions; RMSNorm,
  softmax, and multi-head attention with and without RoPE.

Each design is given to each tool as generated (`ap_fixed<16,5>`), with the generator's `float`
option, and with `float` plus a minor edit committed as a patch in
[`tool_run_log/patches/`](tool_run_log/patches/). Minor edits leave the algorithm, loop bounds,
pragmas and top-level interface unchanged; documented tool options are also allowed. Every input
is listed in [`tool_run_log/cases.json`](tool_run_log/cases.json) as a ForgeBench config, a data type
and an optional patch, with the hash of the resulting `top.cpp`; `scripts/minimal_inputs.py`
regenerates them with ForgeBench's own generator and refuses any hash mismatch. The whole-design
inputs are also committed as source in [`designs/`](designs/) (`<id>/` as generated, `<id>_float/`,
and `<id>_edited/` for the two that pass).

**Support criterion.** A run passes when the tool's flow completes, the emitted C++ synthesizes
with Vitis HLS 2024.1 (xczu9eg-ffvb1156-2-e, 10 ns), and its outputs match the generated `float`
design on five random input sets, with max |ref − out| / max(1, |ref|) < 1e-4 over every output
element (`scripts/minimal_check.py`). Output that synthesizes but computes wrong results is never
counted as correct.

## Results

Table 9, as generated in [`table/tool_support.tex`](table/tool_support.tex) (● passes, ◐ synthesizes
but fails the functional check, ○ fails before synthesis; Ops are Synth. / FV out of those evaluated):

| Tool | GEMM Design | GEMM Ops | DNN Design | DNN Ops | LLM Design | LLM Ops |
|---|---|---|---|---|---|---|
| ScaleHLS | ●‡ | 6/6, 2/6 | ◐ | 3/3, 3/3 | ○ | 4/4, 2/4 |
| HIDA | ◐ | 6/6, 6/6 | ◐ | 3/3, 3/3 | ● | 4/4, 4/4 |
| AutoSA | ○ | 2/2, 0/2 † | ○ | 2/3, 0/3 | ○ | 2/4, 0/4 |
| StreamHLS | ○ | 2/6, 2/6 | ○ | 3/3, 2/3 | ○ | 0/4, 0/4 |

‡ with the `ji` loop order (`gemm_0004`); the base design `gemm_0001` synthesizes but computes wrong
results. † AutoSA was run on two of the six GEMM loop orders (reduction loop innermost and not
innermost); it reschedules the loop nest itself and emitted byte-identical kernels for both.

* As generated in fixed point, every tool stops while parsing the vendor `ap_fixed` headers (F1).
  With `float`, ScaleHLS, HIDA and StreamHLS fail during transformation and AutoSA in its front end.
* **GEMM:** ScaleHLS passes `gemm_0004` with the buffers moved into `top()` and the dot product's
  initialization moved into its loop. HIDA's emitted top level passes the one-element output port
  by value, so the original `top()` call does not compile (F3). On the operators, HIDA is correct
  for all six loop orders; ScaleHLS and StreamHLS only for the two with the reduction loop
  innermost (`ikj`, `kij`; in the generated operator `j` is the reduction loop).
* **DNN:** no tool passes the whole design. ScaleHLS and HIDA pass all three convolutions once the
  padding guard is moved into the innermost loop; StreamHLS passes the two unpadded ones.
* **LLM:** HIDA passes `llm_0001` with five edits (below) and `place-external-buffer=false`; ScaleHLS's
  DSE does not finish within 30 minutes. HIDA passes all four LLM operators, ScaleHLS RMSNorm and
  softmax.
* **AutoSA** produces no correct kernel at any level. Its own example, `autosa_tests/mm`, also fails
  its built-in golden check in the pinned image ("Failed with 4096 errors!";
  [`tool_run_log/logs/autosa_control_mm/`](tool_run_log/logs/autosa_control_mm/autosa/run.log)).

The construct behind each F1–F6 mark, with the runs with and without it, is in
[`table/README.md`](table/README.md#f1f6). Every run, with its stage reached, verdict, error and QoR,
is a row of [`tool_run_log/results.csv`](tool_run_log/results.csv) (Vitis on the outputs that failed the
check: [`tool_run_log/synth_unchecked.csv`](tool_run_log/synth_unchecked.csv)).

### The edits behind the two passing designs

Both patches are against the source generated with the `float` option, and leave `top.h`,
`tb_top.cpp`, `run_hls.tcl`, the operators, the loop bounds, the pragmas and the top-level
interface unchanged. Each edited source computes the same result as the generated design.

**ScaleHLS, `gemm_0004`** ([`gemm_0004__edited.patch`](tool_run_log/patches/gemm_0004__edited.patch);
before and after: [`designs/gemm_0004_float/`](designs/gemm_0004_float/),
[`designs/gemm_0004_edited/`](designs/gemm_0004_edited/)):
1. The ten file-scope `BRAM_*` buffers move into `top()`. ScaleHLS and HIDA convert every global
   array into a constant buffer, and a written global with no initializer then asserts (F2).
2. The dot product's `BRAM_result[0] = 0;` moves into the first iteration of its loop
   (`if (i == 0) BRAM_result[0] = 0;`); ScaleHLS's DSE crashes on a store outside any loop that a
   later loop reads.

**HIDA, `llm_0001`** ([`llm_0001__edited.patch`](tool_run_log/patches/llm_0001__edited.patch), with
`place-external-buffer=false`; before and after: [`designs/llm_0001_float/`](designs/llm_0001_float/),
[`designs/llm_0001_edited/`](designs/llm_0001_edited/)):
1. The eight file-scope `BRAM_*` buffers move into `top()` (F2).
2. The run-time `groups` argument, always 1 at the call site, is specialized to 1, so the head index
   is affine.
3. The `= 0` initializations of the Q/K/V projections and the attention scores move into loop nests
   of their own ahead of the accumulation.
4. Scalar accumulators become per-row array entries (`sum_exp[8]`, `max_score[8]`, LayerNorm's
   `sum[8]` and `var_sum[8]`; the context product accumulates into `output` directly). HIDA's C++
   emitter cannot print a loop-carried scalar (F6).
5. The softmax maximum is written as a select.

With the default `place-external-buffer=true`, HIDA moves buffers of 1024 or more elements to
external ports and fails in code generation; the documented option keeps them on chip.

### Quality of results

| Design | Tool | Latency (cycles) | Vitis HLS on the same `float` design | Speed-up |
|---|---|---|---|---|
| `gemm_0004` | ScaleHLS | 750 | 29,005 | 38.7× |
| `llm_0001` | HIDA | 164,804 | 607,028 | 3.7× |

Both are `csynth_design` estimates with the settings above; the reports are in
[`reports/`](reports/). The latency and resources of every synthesized run are in
`tool_run_log/results.csv` and `tool_run_log/synth_unchecked.csv`. ScaleHLS's DSE uses its own
resource specification and StreamHLS runs without its AMPL/Gurobi schedule solver, so these are
not matched-resource comparisons.

## Contents

| Path | Content |
|---|---|
| [`table/`](table/README.md) | Table 9 as generated, each cell with its runs, and the evidence behind each F1–F6 mark |
| [`tool_run_log/`](tool_run_log/) | `cases.json` (inputs and runs), `configs/` (operator configs), `patches/` (the edits), `results.csv` (every run), `synth_unchecked.csv`, and `logs/<run>/` (trimmed logs: the command, the first error and the last ~50 lines) |
| [`designs/`](designs/) | The whole-design inputs as source; `manifest.json` records the base designs' configs and hashes |
| [`generated_hls/`](generated_hls/) | The HLS C++ each tool emitted, for the 110 runs that reached code generation, whatever the verdict; `index.csv` gives each directory's run, verdict and file hashes |
| [`reports/`](reports/) | csynth reports of the two passing whole designs |
| [`tool_eval_logs.tar.gz`](tool_eval_logs.tar.gz) | Full redacted transcripts of every run (`tool_run_log/<run>/full.log`, `csynth.xml`), the tool builds, each tool on its own example, and the environment; name, SHA-256 and size in [`archive.json`](archive.json). Also in the Zenodo release |
| [`versions.json`](versions.json) | Tool commits, binary hashes and images |
| [`patches/build/`](patches/build/) | The one source patch applied to build HIDA |
| [`scripts/`](scripts/) | Everything needed to rebuild the tools and re-run the evaluation |

Logs and emitted code are redacted of local paths and host names (`<PATH>`, `<WORK>`,
`<VITIS_INCLUDE>`, …).

## Tool versions

| Tool | Repository | Commit / dependency |
|---|---|---|
| ScaleHLS | [UIUC-ChenLab/scalehls](https://github.com/UIUC-ChenLab/scalehls) | `b69f334c9c122fad3759fcf388d3f039689b6fc0`; Polygeist `737290e9`, LLVM/MLIR 16.0.0git `cf0ff67a` |
| HIDA | [UIUC-ChenLab/ScaleHLS-HIDA](https://github.com/UIUC-ChenLab/ScaleHLS-HIDA) | `fd79f6c946593c290d6d285c2a2efcab72fbe9c6`; same Polygeist/LLVM as ScaleHLS |
| AutoSA | [UCLA-VAST/AutoSA](https://github.com/UCLA-VAST/AutoSA) | `b61a1b4132d631600696feba59eb606acb34d304`, image `whbldhwj/autosa@sha256:b9318c50…`; Clang 9.0.1, PET 0.11.3, ISL 0.22 |
| StreamHLS | [UCLA-VAST/Stream-HLS](https://github.com/UCLA-VAST/Stream-HLS) | `00c6219a82fc710492f4eb44e4f34836dfb19e08`; LLVM/MLIR 18.1.2 `26a1d660`; C front end Polygeist `77c04bb2` with LLVM 18.0.0git `26eb4285` |
| Vitis HLS | AMD | 2024.1, part `xczu9eg-ffvb1156-2-e`, 10 ns |

## Re-running

**Environment:** Docker; the tools are built from the pinned commits by `scripts/checkout_tools.sh`
and `scripts/build_mlir.sh` inside `scripts/Dockerfile.build` (Ubuntu 22.04, Clang/LLD 14, CMake,
Ninja), and AutoSA runs in its pinned upstream image. Vitis HLS 2024.1 must be on `PATH`, with
`VITIS_HLS_INCLUDE` set to its `include` directory, and Python ≥ 3.9 with NumPy (for the generator).
The tool builds need about 40 GB outside git.

```bash
tool_eval_work=$(mktemp -d)
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
  --vitis-include "$VITIS_HLS_INCLUDE" --jobs 12           # every run in tool_run_log/cases.json; resumable
python synth_unchecked.py --work "$tool_eval_work/minimal" --jobs 16  # Vitis on output that failed the check
python collect_generated.py --work "$tool_eval_work/minimal"  # the emitted HLS into generated_hls/
python paper_table.py                                        # Table 9, cell by cell, into table/
python -m unittest discover -s . -p test_evidence.py && python validate.py
```

Without the tools, `python paper_table.py --check` and `python validate.py` check the committed
records: every input hash, log, verdict and emitted file, that `table/` is what the records give,
and that the tarball matches `archive.json`.

| Script | Role |
|---|---|
| `minimal_inputs.py` | Regenerates every input from its config, data type and patch, and checks its hash |
| `minimal_run.py` | Runs the matrix in `cases.json` (`--only <regex>` for a subset) and writes `results.csv` |
| `run_mlir.py`, `run_autosa.py`, `run_vitis.py`, `run_control.py` | One tool on one design (or on its own example); record the stage reached, the first error and the input hash |
| `minimal_check.py` | The functional comparison against the generated `float` design |
| `synth_emitted.py`, `synth_unchecked.py` | Vitis HLS on emitted code that passed, and that failed, the check |
| `collect_generated.py` | Copies each tool's emitted HLS into `generated_hls/` |
| `paper_table.py` | Derives Table 9 into `table/`; `--check` compares with the committed files |
| `archive_logs.py` | Builds `tool_eval_logs.tar.gz` and `archive.json` |
| `validate.py`, `test_evidence.py` | The record checks and the unit tests of the runners |
