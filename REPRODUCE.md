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
| Table 5, full model-level designs (`tab:ml_model_suite`) | `results/fullmodel/metrics_csynth.csv`, `results/verification/fullmodel/*/` | `make ml-model-suite` | `out/tables/ml_model_suite.{csv,tex}` |
| Figs 6, 7, 9 and Table 8, ML sweep (`fig:dist_csynth`, `fig:design_coverage_analysis`, `fig:dist_impl_kde`, `tab:impl_ppa`) | `results/ml_sweep/{csynth,impl}/metrics_*.csv` | `make section42` | `out/figs/{dist_csynth,combined_utilization,dist_impl}.png`, `out/tables/impl_ppa.{csv,tex}` |
| Table 10, modularization (`tab:modularization_results`) | `results/modular_designs/csynth_utilization.csv` | `make modularization` | `out/tables/modularization_{results.csv,table.tex}` |
| Tables 2-4, operator / co-sim / Catapult verification (`tab:verification-both`, `tab:verif-cosim`, `tab:verif-catapult`), and the Table 10 verification at `<32,10>` | `results/verification/{operators,cosim,catapult,table7}/` | `make verification-tables` | `out/verification/verif_*.tex`; every number in `manifest/verification/*.csv` |
| Fig 8, Catapult vs Vitis synthesis of 1,000 GEMM designs (`fig:catapult_vs_vitis`) | `results/catapult_vs_vitis/metrics.csv` | `make catapult-vs-vitis` | `out/figs/catapult_vs_vitis.{png,pdf}` |
| Table 6, sweep design space (`tab:ml_suite`) | `manifest/designs/{gemm,dnn,llm}.csv` (one row per design with its parameters); the values are the `SWEEP` dict of `forgebench/<domain>/auto_generate_json.py` | `forgebench sweep-configs <domain>` | the configs |
| Table 7, synthesis / implementation outcome per domain (`tab:synth_summary`), and Table 11, stages passed per design set (`tab:status`) | `manifest/designs/*.csv`, `manifest/ml_sweep/`, `manifest/verification/*.csv`, `results/catapult_vs_vitis/status_catapult.csv` | `make stage-tables` | `out/tables/{synth_summary,status_summary}.{csv,tex}` |
| Table 1, supported models (`tab:supported_models`), *Block* column | VGG-19 / ResNet-18 / ResNet-50 blocks: `designs/configs/examples/dnn/`; GPT and LLaMA transformer blocks: `designs/configs/modular_designs/llm/{gpt_transformer_p1,llama_transformer_p2}.json` | `forgebench generate <config>` | — |
| Fig 3 and Fig 5, GEMM JSON example and its Vitis / Catapult code (`fig:jsondesign`, `fig:fb_hls_gen`) | `designs/configs/examples/gemm/gemm_96x512x128_{vitis,catapult}.json` | `forgebench generate <config>` (`make check` compares with `designs/hls/examples/gemm/`) | `top.cpp` (`gemm_ijk`) |
| Fig 4a-b, Python generation snippets (`fig:pythonblock`, `fig:pythonmodel`) | `forgebench/fullmodel/auto_generate_json.py` (`generate_llama_decode_block`, `generate_llama_architecture`) | — | — |
| Table 9, source-to-source tool support (`tab:tool-support`) | `results/tool_eval/{summary.csv,outcomes.csv,minimal/results.csv,feature_evidence.csv}` | `python results/tool_eval/scripts/report_tables.py --write` (tables in `results/tool_eval/README.md`); re-running the tools: `results/tool_eval/README.md` §5 | `results/tool_eval/README.md` |
| Fig 4c-d, HLSFactory files (`fig:hlsfactory_opt`, `fig:hlsfactory_run`) | pending | — | — |

Table and figure numbers are those of the current manuscript; the labels in parentheses are its LaTeX labels. Identifiers recorded
with the results keep their original names: `table7` (the directory `results/verification/table7/`, `run_table7.sh`,
`manifest/verification/table7_*.csv`, the `"operators": "table7"` setting) is the modularization suite of Table 10, and `table2`
(`--table2`) the 57 operator variants of Table 2.

## 2. Designs and configs

`make check` (`python -m verification.check_designs --sweep`) regenerates and byte-compares:

- the 12,144 ML sweep configs, from `forgebench/<domain>/auto_generate_json.py` (`forgebench sweep-configs <domain>`);
- the 21 examples and the 14 full-scale models, from `designs/configs/{examples,fullmodel}` (`forgebench generate`);
- the 26 Table 10 programs at `<32,10>`, from the verified design JSONs in `results/verification/table7/C_32_10__table7/configs`
  (`verification/experiments/prepare_table7_synth.py`). The 12 modularized Table 10 designs are hand-written; their sources are
  `designs/hls/modular_designs/<domain>/*_module*`.

The sweep designs themselves are not committed; generate any of them with `forgebench generate designs/configs/ml_sweep/<domain>/<name>.json`.
Design ids (`gemm_0001`, `dnn_0001`, …) map to config names in `manifest/designs/<domain>.csv`.

## 3. Re-running the tools

The lean outputs (reports only; build trees are deleted) go anywhere you like; `_sweeps/` below is just a convention.

### ML sweep: csynth and implementation (Figs 6, 7, 9, Table 8)

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

### Full-scale models (Table 5)

Synthesis (ResNet full 18 min–29 h per design, tiled 11 min–3 h, Llama 45–90 min):

```sh
python -m forgebench.modular_designs.synth --registry manifest/designs/fullmodel.csv --cases /dev/null \
    --lean-out _sweeps/lean/fullmodel_csynth --jobs 14
forgebench collect _sweeps/lean/fullmodel_csynth/fullmodel --tool vitis --flow csynth --domain fullmodel \
    --out results/fullmodel/metrics_csynth.csv
```

C simulation of every Table 5 design against the fixed-point and FP64 PyTorch references (`verification/fullmodel/verify.py`; needs
`[verify]` and Vitis HLS). The run list is `manifest/designs/fullmodel.csv`: the 10 ResNets x seeds 42–51 and the two Llama
prefill/decode pairs x seeds 42–44 (4 prefill + 2 decode tokens), 106 runs, ~2.5 h. A Llama run needs a ~33 GB temporary parameter store.

```sh
make verify-fullmodel OUT=<new dir>                    # ONLY=<regex> for a subset, e.g. ONLY=tiled or ONLY=llama_ctx8192
FULLMODEL=<that dir> bash verification/experiments/collect_results.sh   # compact copy into results/verification/fullmodel/
make ml-model-suite
```

`verify.py` refuses sources that are not registered in `verification/fullmodel/references/contracts.json` (the SHA-256 of every
shipped `top.cpp` / `top.h`); register changed sources with `verification/fullmodel/register_sources.py`.

### Table 10 (modularization)

```sh
python -m verification.experiments.prepare_table7_synth --out _sweeps/table7_32_10
python -m forgebench.modular_designs.synth --registry _sweeps/table7_32_10/registry.csv \
    --lean-out _sweeps/lean/modular_csynth_32_10 --jobs 38
python -m forgebench.modular_designs.parse_reports --reports-root _sweeps/lean/modular_csynth_32_10   # -> results/modular_designs/csynth_utilization.csv
```

Functional verification of the 26 programs and the 12 modularized designs at `<32,10>`: `run_table7.sh` (below).

### Verification tables (operators, co-simulation, Catapult backend, Table 10)

[verification/README.md](verification/README.md) maps each verification result to its config, run script, results folder and
manifest CSV.

```sh
JOBS=14 bash verification/experiments/run_operators.sh    # operators: largest faithful range and +-1, columns A-D
JOBS=16 bash verification/experiments/run_cosim.sh        # C/RTL co-simulation, <32,10> and float
bash verification/experiments/run_catapult.sh             # Catapult backend (needs the Siemens license environment)
JOBS=6  bash verification/experiments/run_table7.sh       # Table 10 programs and modularized designs, <32,10>
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
