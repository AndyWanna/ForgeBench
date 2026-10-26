# Functional verification

Every verification result in the paper, where its configuration and results live, and how to re-run it. Each experiment is one JSON
config run by the engine `verification/functional_verification.py`; the per-design numbers of every table are in
`manifest/verification/<table>.csv`, and `make verification-tables` rebuilds the LaTeX tables from `results/verification/`.

| Paper result | Designs | Config (`verification/experiments/`) | Run | Results (`results/verification/`) | Manifest (`manifest/verification/`) |
|---|---|---|---|---|---|
| Operators, largest faithful range | 57 Table-2 operator variants (`operators/<op>/variants/`) | `operators/{A,B,C,D}_*__max_range.json` | `run_operators.sh` | `operators/*__max_range/` | `operators.csv` |
| Operators, inputs in [-1, 1] | same | `operators/{A,B,C,D}_*__pm1.json` | `run_operators.sh` | `operators/*__pm1/` | `operators.csv` |
| C/RTL co-simulation | same, `<32,10>` and float | `cosim/{C_32_10,D_float}__cosim.json` | `run_cosim.sh` | `cosim/` | `cosim.csv` |
| Catapult backend | the 8 matrix-product variants (GEMM, vmm, mmv, dot, without/with bias), `<32,10>` and float | `catapult/*__catapult_pm1.json` | `run_catapult.sh` | `catapult/` | `catapult.csv` |
| Table 10 programs | the 26 programs (`designs/{configs,hls}/modular_designs/`), `<32,10>` | `table7/C_32_10__table7.json` | `run_table7.sh` | `table7/C_32_10__table7/` | `table7_programs.csv` |
| Table 10 modularized designs | the 12 modularized designs, each against every program of its row | (the programs' inputs) | `run_table7.sh` | `table7/C_32_10__table7_modularized/` | `table7_modularized.csv` |
| Table 5 C-sim error | the 14 full-scale models (`designs/hls/fullmodel/`) | `manifest/designs/fullmodel.csv` | `run_fullmodel.sh` | `fullmodel/<group>/` | `fullmodel.csv` |

Every run folder in `results/verification/` has `summary.csv` (one row per design x format), `resolved_config.json` (the settings
used), `configs/` (the exact design JSON of every reported experiment: data type, input ranges, seeds), `rows/` and `run.log.gz`.
Per-trial logs are in the data release, except for the `operators/*__pm1` and `catapult/` trials, which the Catapult table compares
seed by seed. Full models: see [fullmodel/README.md](fullmodel/README.md).

## Re-running

Needs Vitis HLS 2024.1 on `PATH` (Catapult 2026.2 and its license for `run_catapult.sh`). Raw outputs go to
`verification/results_paper/<table>/<run>/` (not tracked); `collect_results.sh` copies the compact part into
`results/verification/<table>/<run>/` and rebuilds the tables and manifest CSVs.

```bash
JOBS=14 bash verification/experiments/run_operators.sh    # ~1.5 h on 128 cores
JOBS=16 bash verification/experiments/run_cosim.sh        # idle server; float conv ~8 h, mha ~16.5 h
bash verification/experiments/run_catapult.sh
JOBS=6  bash verification/experiments/run_table7.sh       # conv_block_op1 dominates: ~10 h
make verify-fullmodel OUT=<new dir>                        # ~2.5 h; then FULLMODEL=<dir> for collect_results.sh
bash verification/experiments/collect_results.sh
```

One design and format: `python -m verification.functional_verification --config verification/experiments/operators/C_32_10__pm1.json
--operators ops/gemm/gemm__bias1 --out <dir>` (`--n`, `--range`, `--dtypes`, `--sim` override the config).

Without Vitis: `make selfcheck` compiles every whole-design config with g++ (`shim/` stands in for the Vitis headers) and checks it
against the float golden.

## Method

**Formats (columns).** A `<16,5>` throughout; B "mixed": `<16,5>` at the interface, `<24,8>` operator storage and a `<32,10>`
round-to-nearest accumulator; C `<32,10>` throughout; D IEEE float. `fixed<W,I>` is truncate + wrap unless written
`fixed<W,I,rnd,sat>` (see [docs/DATA_TYPES.md](../docs/DATA_TYPES.md)).

**Golden.** NumPy float64 on the *quantized* inputs (what the design receives after its `(data_t)` conversion), so the error measured
is the design's arithmetic, not input rounding. The golden (`golden_ref.py`, `domains/`, `activations.py`) implements each operator's
textbook math and does not read the generated C.

**Error bound.** Fixed point: c * LSB * (L + stages) + a fraction of the output's peak, with L the longest accumulation and `stages`
the dependent rounding stages (+ #ops - 1 for whole designs). Float: rtol 1e-3, atol 1e-5.

**Trials.** N=100 input sets, uniform in [-r, r] (non-negative inputs: [0, r]); trial k uses seed `seed_base + report_seed_offset + k`.
Input constraints: operator weights (matmul, mha, swa, conv) +-0.1, batchnorm parameters [0.25, 1]; Table 10 weights
+-sqrt(3/fan_in) (gemm: the second operand), conv bias +-0.1, norm parameters [0.25, 1]; conv_block_op1 uses N=10 (up to 60 min per
trial). Full models: [fullmodel/README.md](fullmodel/README.md).

**Largest faithful range** (`range_search.py`). Scan a geometric grid of input magnitudes (factor sqrt 2; fixed point 0.001 to 2x the
format's integer range, float 0.001-1000), 20 samples per point; a point passes if every output of every sample is within the bound
and nothing crashed. Take the largest contiguous passing window, bisect its top edge (4 steps), confirm it with N=100 samples (stepping
down by 1/1.19 until all pass) and back off one more step. Then report N=100 fresh samples at that range. A window need not start at 0
(e.g. `<16,5>` softmax over 32 entries is faithful only from +-1.45: near-equal inputs make the exponential sum ~32, past +-16); the
tables mark such rows. `range_search.caps` caps the search for tanh (Vitis `hls::tanh` defect at large |x|).

**Co-simulation.** Vitis C/RTL co-simulation (Verilog) of the +-1 design and inputs, 10 input sets in one looped testbench; RTL
outputs compared element by element with the C simulation.

**Catapult backend.** The Catapult backend (`forgebench/backends/catapult.py`, `"tool": "catapult"`) generates the design; Catapult
SCVerify's original-C++ simulation runs it on exactly the inputs, seeds, golden and bound of the Vitis `operators/*__pm1` run, so
every trial is compared with the Vitis trial of the same seed.

**Table 10 modularized designs** (`merged_designs.py`). Each modularized design is driven with the inputs of every program of its row
(the program run's design JSONs and seeds) and each output is checked against that program's golden and bound.

## Code

| File | Role |
|---|---|
| `functional_verification.py` | the engine: generate, build once with Vitis C sim, range search, N trials, optional co-sim |
| `range_search.py`, `fixed_runner.py`, `csim_runner.py`, `cosim.py`, `prepare_designs.py` | its parts: range search, fixed-point bounds and output reading, C-sim / Catapult runs, co-sim, design generation |
| `golden_ref.py`, `domains/`, `activations.py`, `fp.py`, `fixedpoint.py`, `io.py`, `layout.py` | golden references, number formats, file layout |
| `merged_designs.py` | Table 10 modularized designs against their programs |
| `fullmodel/` | full-scale model verifier (PyTorch fixed-point + FP64 references) |
| `experiments/` | the paper configs and run scripts above; `make_tables.py` (tables + manifest CSVs), `collect_results.sh` |
| `operators/<op>/variants/<domain>/` | the 57 Table-2 operator designs |
| `check_designs.py`, `regression_check.py`, `gcc_selfcheck.py`, `shim/` | `make check`, generator regression check, `make selfcheck` |
| `tests/` | unit tests (`make test`) |
