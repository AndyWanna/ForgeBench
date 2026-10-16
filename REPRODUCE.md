# Reproducing the paper

Three levels, from cheapest to most expensive:

1. **Tables and figures from the committed results** (Python only, minutes): `make paper`.
2. **Regenerate every design and config** and check it against the repository (Python only, < 1 min): `make check`.
3. **Re-run the tools**: synthesis, implementation and functional verification (Vitis HLS / Vivado 2024.1.2, Catapult 2026.2,
   torch; hours to days). The commands below are the ones that produced `results/`.

## Environment

| Need | For |
|---|---|
| Python >= 3.12, `pip install -e .` | everything below |
| `pip install -e '.[verify]'` (torch, pytest) | full-model verification, `make test` |
| g++ | `make selfcheck` (Vitis-free C simulation of the generated designs) |
| Vitis HLS + Vivado 2024.1.2 on `PATH` | synthesis, implementation, C simulation / co-simulation |
| Catapult 2026.2 + its license (`verification/experiments/run_catapult.sh`) | Catapult backend results |

Exact tool builds: [docs/TOOL_VERSIONS.md](docs/TOOL_VERSIONS.md). Target: ZCU102 (`xczu9eg-ffvb1156-2-e`), 10 ns clock.

## 1. Tables and figures from `results/`

`make paper` runs everything in this table; outputs go to `paper_artifacts/out/`.

| Paper item | Inputs (in git) | Command | Output |
|---|---|---|---|
| Table 3, full-scale models | `results/fullmodel/metrics_csynth.csv`, `results/verification/fullmodel/*/` | `make table3` | `out/tables/table3_fullmodel.{csv,tex}` |
| Figs 6–8, Table 5 (ML sweep) | `results/ml_sweep/{csynth,impl}/metrics_*.csv` | `make section42` | `out/figs/fig{6,7,8}_*.png`, `out/tables/table5_impl.{csv,tex}` |
| Table 7, modularization | `results/modular_designs/csynth_utilization.csv` | `make table7` | `out/tables/modularization_{results.csv,table.tex}` |
| Verification tables (operators, co-sim, Catapult, Table 7 at `<32,10>`) | `results/verification/{operators,cosim,catapult,table7}/` | `make verification-tables` | `out/verification/verif_*.tex`; every number in `manifest/verification/*.csv` |
| Catapult vs Vitis synthesis (1,000 GEMM designs) | `results/catapult_vs_vitis/metrics.csv` | `make catapult-vs-vitis` | `out/figs/catapult_vs_vitis.{png,pdf}` |
| Table 4, sweep design space | `manifest/designs/{gemm,dnn,llm}.csv` (one row per design with its parameters) | — | — |

## 2. Designs and configs

`make check` (`python -m verification.check_designs --sweep`) regenerates and byte-compares:

- the 12,144 ML sweep configs, from `forgebench/<domain>/auto_generate_json.py` (`forgebench sweep-configs <domain>`);
- the 19 examples and the 14 full-scale models, from `designs/configs/{examples,fullmodel}` (`forgebench generate`);
- the 26 Table 7 programs at `<32,10>`, from the verified design JSONs in `results/verification/table7/C_32_10__table7/configs`
  (`verification/experiments/prepare_table7_synth.py`). The 12 modularized Table 7 designs are hand-written; their sources are
  `designs/hls/modular_designs/<domain>/*_module*`.

The sweep designs themselves are not committed; generate any of them with `forgebench generate designs/configs/ml_sweep/<domain>/<name>.json`.
Design ids (`gemm_0001`, `dnn_0001`, …) map to config names in `manifest/designs/<domain>.csv`.

## 3. Re-running the tools

The lean outputs (reports only; build trees are deleted) go anywhere you like; `_sweeps/` below is just a convention.

### ML sweep: csynth and implementation (Figs 6–8, Table 5)

```sh
for d in gemm dnn llm; do
  forgebench ml-sweep --domain $d --flow csynth --jobs 48 --lean-out _sweeps/lean/${d}_csynth
  forgebench ml-sweep --domain $d --flow impl --jobs 32 --timeout 14400 \
      --select manifest/ml_sweep/${d}_impl_selection.csv --lean-out _sweeps/lean/${d}_impl
  forgebench collect _sweeps/lean/${d}_csynth --tool vitis --flow csynth --domain $d --out results/ml_sweep/csynth/metrics_$d.csv
  forgebench collect _sweeps/lean/${d}_impl   --tool vitis --flow impl   --domain $d --out results/ml_sweep/impl/metrics_$d.csv
done
python manifest/rekey_results.py          # generator names -> design ids in results/ml_sweep
make section42
```

The implementation samples are seeded: `forgebench select-impl <domain> --seed <s> --out manifest/ml_sweep/<domain>_impl_selection.csv`
with seeds GEMM 20261002, DNN 20261014, LLM 20261001 reproduces the committed selections. Runs are resumable (designs already `ok`
in `<lean-out>/status.csv` are skipped).

### Full-scale models (Table 3)

Synthesis (ResNet full 18 min–29 h per design, tiled 11 min–3 h, Llama 45–90 min):

```sh
python -m forgebench.modular_designs.synth --registry manifest/designs/fullmodel.csv --cases /dev/null \
    --lean-out _sweeps/lean/fullmodel_csynth --jobs 14
forgebench collect _sweeps/lean/fullmodel_csynth/fullmodel --tool vitis --flow csynth --domain fullmodel \
    --out results/fullmodel/metrics_csynth.csv
```

C simulation of every Table 3 design against the fixed-point and FP64 PyTorch references (`verification/fullmodel/verify.py`; needs
`[verify]` and Vitis HLS). The run list is `manifest/designs/fullmodel.csv`: the 10 ResNets x seeds 42–51 and the two Llama
prefill/decode pairs x seeds 42–44 (4 prefill + 2 decode tokens), 106 runs, ~2.5 h. A Llama run needs a ~33 GB temporary parameter store.

```sh
make verify-fullmodel OUT=<new dir>                    # ONLY=<regex> for a subset, e.g. ONLY=tiled or ONLY=llama_ctx8192
FULLMODEL=<that dir> bash verification/experiments/collect_results.sh   # compact copy into results/verification/fullmodel/
make table3
```

`verify.py` refuses sources that are not registered in `verification/fullmodel/references/contracts.json` (the SHA-256 of every
shipped `top.cpp` / `top.h`); register changed sources with `verification/fullmodel/register_sources.py`.

### Table 7 (modularization)

```sh
python -m verification.experiments.prepare_table7_synth --out _sweeps/table7_32_10
python -m forgebench.modular_designs.synth --registry _sweeps/table7_32_10/registry.csv \
    --lean-out _sweeps/lean/modular_csynth_32_10 --jobs 38
python -m forgebench.modular_designs.parse_reports --reports-root _sweeps/lean/modular_csynth_32_10   # -> results/modular_designs/csynth_utilization.csv
```

Functional verification of the 26 programs and the 12 modularized designs at `<32,10>`: `run_table7.sh` (below).

### Verification tables (operators, co-simulation, Catapult backend, Table 7)

[verification/README.md](verification/README.md) maps each verification result to its config, run script, results folder and
manifest CSV.

```sh
JOBS=14 bash verification/experiments/run_operators.sh    # operators: largest faithful range and +-1, columns A-D
JOBS=16 bash verification/experiments/run_cosim.sh        # C/RTL co-simulation, <32,10> and float
bash verification/experiments/run_catapult.sh             # Catapult backend (needs the Siemens license environment)
JOBS=6  bash verification/experiments/run_table7.sh       # Table 7 programs and modularized designs, <32,10>
bash verification/experiments/collect_results.sh          # compact copy into results/verification/, tables, manifest/verification/
```

### Catapult vs Vitis synthesis

```sh
python -m paper_artifacts.catapult_vs_vitis sample --out _sweeps/catapult_vs_vitis
bash forgebench/ml_sweep/scripts/run_catapult_vs_vitis.sh
python -m paper_artifacts.catapult_vs_vitis collect --out _sweeps/catapult_vs_vitis
python -m paper_artifacts.catapult_vs_vitis plot --out _sweeps/catapult_vs_vitis
```

Results: `results/catapult_vs_vitis/`.

## Changing a generator

`make regression-save` before and `make regression-compare` after a generator change byte-compares a seeded sample of 252 designs
(sweep, operator variants, whole designs); `make check` covers every committed design.
