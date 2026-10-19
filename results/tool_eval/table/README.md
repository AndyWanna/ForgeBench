# The paper's tool-support table, cell by cell

This folder ties every cell of the revised `tab:tool-support` to the runs behind it. Nothing
here is typed by hand except [`constructs.json`](constructs.json), the list of runs behind each
F1–F6 mark, and `paper_table.py` checks every entry of it against the records.

| File | Content |
|---|---|
| [`tool_support.tex`](tool_support.tex) | The table, as generated from the records |
| [`cells.csv`](cells.csv) | One row per cell: tool, domain, column, value, and the runs it rests on |
| [`runs.csv`](runs.csv) | One row per run named in `cells.csv`: its input, verdicts, emitted code and logs |
| [`constructs.json`](constructs.json) | The runs with and without each F1–F6 construct, with the expected status of each |

`python scripts/paper_table.py` rebuilds the three generated files from
[`../minimal/results.csv`](../minimal/results.csv),
[`../minimal/synth_unchecked.csv`](../minimal/synth_unchecked.csv),
[`../minimal/cases.json`](../minimal/cases.json) and `constructs.json`.
`scripts/validate.py` fails if they are out of date.

## How a cell is computed

* **Design:** the base design of the domain, `gemm_0001`, `dnn_0001` or `llm_0001`. Every input the
  tool was given for it counts: as generated in fixed point, with the `float` option, and with
  the `float` option plus the committed patch. The cell takes the best of those runs:
  * ● when one synthesizes and passes the functional check;
  * ◐ when one's emitted code synthesizes;
  * ○ otherwise.
* **Ops:** the single-operator designs the tool was given. An operator counts under Synth. when
  the emitted code of at least one of its inputs synthesizes, and under FV when one of them also
  passes the functional check. Both counts are bold when every evaluated operator passes FV.
* **F1–F6:** the mark in `constructs.json`, with the runs that contain the construct as generated
  (`with`) and the runs that differ only by removing it (`without`).

"Synthesizes" is Vitis HLS 2024.1, `csynth_design` on xczu9eg-ffvb1156-2-e at 10 ns. The functional
check compares the emitted C++ with the unmodified generated `float` design on five random input
sets, with a relative error below 1e-4 on every output (`scripts/minimal_check.py`).

Synthesis comes from two records:
* `scripts/minimal_run.py` synthesizes only output that passes the check. These runs are in
  `minimal/results.csv`.
* `scripts/synth_unchecked.py` synthesizes every other emitted output, 72 in all, with the same
  flow. Their results are in `minimal/synth_unchecked.csv`, and 44 of them synthesize. It reads
  the kernels from `generated_hls/`, which is byte-identical to what the tools wrote.

## Tracing a cell to its inputs, outputs and logs

1. Find the cell in `cells.csv`. Its `runs` column lists run ids, such as `gemm_0001__edited/scalehls`.
2. Look up each run in `runs.csv`:
   * **Input:** `input_config`, `data_type` and `patch`. `scripts/minimal_inputs.py` regenerates
     the input from these and checks `input_top_cpp_sha256`. Whole-design inputs are also
     committed as source in `designs/` (see the main README §2.1).
   * **Emitted code:** `emitted_code`, in `generated_hls/`.
   * **Short logs:** `logs`, in `minimal/logs/<run>/`. These are `run.log`, `result.json`, and
     `check.json` when the check ran. When `synth_unchecked.py` synthesized the output, there is
     also `synth-unchecked.log`.
   * **Full logs:** `full_logs`, in `tool_eval_logs.tar.gz`. These are `full.log` (the complete
     transcript), `csynth.xml` when the output synthesized, and `synth-unchecked.log` with
     `synth-unchecked-csynth.xml` for the outputs that failed the check.

## The cells

### Design

| Tool | GEMM | DNN | LLM |
|---|---|---|---|
| ScaleHLS | ◐ `gemm_0001__edited/scalehls`: synthesizes (750 cycles), max relative error 2.5 | ◐ `dnn_0001__edited/scalehls`: synthesizes (144,717 cycles), max relative error 1.02 | ○ `llm_0001__edited/scalehls`: DSE times out after 1,800 s, no code |
| HIDA | ◐ `gemm_0001__edited/hida`: synthesizes (8,226 cycles); the one-element output port is passed by value, so the original `top()` call does not compile (F3) | ◐ `dnn_0001__edited/hida`: synthesizes (861,222 cycles); unit dimensions dropped from the interface. With `place-external-buffer=false`: synthesizes, max relative error 0.664 | ● `llm_0001__edited/hida-noext`: correct (3.6e-7), 164,804 cycles |
| AutoSA | ○ fixed and float stop in the Clang front end | ○ same | ○ same |
| StreamHLS | ○ edited design stops at transform | ○ edited design crashes at transform | ○ edited design crashes at transform |

* The `ji` loop-order sibling `gemm_0004` is ● for ScaleHLS: correct (1.1e-6), 750 cycles. It is
  not the base design, so the table does not use it.
* AutoSA was not given an edited whole design. It requires the computation in one
  `#pragma scop` region with a single uniform loop band, which a design of several operators
  does not have. Its Design cells reflect the inputs it was given.

### Ops

| Tool | GEMM Synth. / FV | DNN Synth. / FV | LLM Synth. / FV |
|---|---|---|---|
| ScaleHLS | 6/6 / 2/6. All orders synthesize; only `ikj` and `kij` are correct | 3/3 / 3/3 | 4/4 / 2/4. `mha` and `mha_rope` synthesize but are wrong |
| HIDA | 6/6 / 6/6 | 3/3 / 3/3 | 4/4 / 4/4. `mha`, `mha_rope`, `rmsnorm` and `softmax` need `place-external-buffer=false` and the patches in `minimal/patches/` |
| AutoSA | 2/2 / 0/2. Two orders run; st3 synthesizes in both but is wrong | 2/3 / 0/3. `conv_k3` yields no kernel | 2/4 / 0/4. `rmsnorm` and `softmax` st0 synthesize but hang in C simulation; `mha` and `mha_rope` yield no kernel |
| StreamHLS | 2/6 / 2/6. The other four orders crash in loop permutation | 3/3 / 2/3. Padded `conv_k3` synthesizes but is wrong | 0/4 / 0/4. Every form crashes at transform |

### F1–F6

`cells.csv` gives each mark's claim and runs, and `constructs.json` gives each run's expected
status. In short:

| Mark | With the construct | Without it |
|---|---|---|
| F1 vendor fixed-point types | `*__fixed`: every tool stops at parse in the vendor `ap_fixed` code. AutoSA also stops on `*__float`, which still includes `ap_fixed.h` | `*__float` parses (ScaleHLS, HIDA, StreamHLS). For AutoSA, the headers are removed in `*__autosa` |
| F2 file-scope buffers | `*__float`: ScaleHLS and HIDA crash on constant-buffer conversion; StreamHLS asserts in its constant matcher | `*__local`, with the buffers declared in `top()`, gets past it. AutoSA ✓: its inputs keep the buffers at file scope |
| F3 top-level interface | HIDA: `gemm_0001__edited`, `dnn_0001__edited` and `mha__attn` change the ports | ScaleHLS, StreamHLS and AutoSA output is checked through the original `top()` |
| F4 arbitrary loop order | ScaleHLS: 4 orders wrong. StreamHLS: 4 orders crash. AutoSA: both orders tried fail | `ikj` and `kij` are correct for ScaleHLS and StreamHLS. HIDA ✓ on all six |
| F5 conditional accesses | `conv_k3__local`: ScaleHLS and HIDA crash. StreamHLS is wrong even with the guard sunk. AutoSA: non-uniform dependence | `conv_k3__sunkguard` is correct for ScaleHLS and HIDA. Unguarded `conv_k3p0__noguard_reorder` is correct for StreamHLS |
| F6 scalar reductions | `rmsnorm`/`softmax__local`: HIDA output does not compile, in g++ or in Vitis. StreamHLS crashes in every form. AutoSA declares no FIFO, or its kernel hangs | `*__rowacc_split` with `place-external-buffer=false` is correct for HIDA. ScaleHLS ✓ as generated |

## Where the evidence does not fully support a mark

* **AutoSA F4 ✗:** AutoSA was given two GEMM loop orders, and both fail. The two kernels are
  byte-identical, though, so loop order is not what makes AutoSA fail. Under the caption's
  definition ("fails on the construct"), the evidence fits a ✓, with the failure explained
  elsewhere. Alternatively, keep the ✗ with a footnote. `cells.csv` marks this cell "NOT ISOLATED".
* **AutoSA GEMM Ops:** the denominator is 2, not 6, because only two loop orders were run (footnote a).
  The kernels for the two orders are byte-identical, so the other four are expected to behave
  the same, but they were not run.
