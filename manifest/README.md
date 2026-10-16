# ForgeBench manifest

Quick lookup of what the paper's results are made of: every design behind a table or figure, what was run on it, where its
reports are, and every verification number behind a table.

| Folder | What |
|---|---|
| `designs/` | one row per design: the ML sweeps, the Table-2 operators, Table 7, Table 3 |
| `verification/` | one row per verified design x format, per verification table (built by `make verification-tables`) |
| `ml_sweep/` | the frozen design-id map, the seeded implementation samples, and why the 39 unfinished implementation runs failed |

## Sweep designs: `designs/{gemm,dnn,llm}.csv`

One row per design (Table 4; Figs 6-8, Table 5). Columns, in order:

| Column | Meaning |
|---|---|
| `design_id` | `gemm_0001`, `dnn_0001`, `llm_0001`, ... Opaque and frozen for the release. |
| *sweep parameters* | One column per parameter that varies in that sweep (see the `SWEEP` dict in `forgebench/<domain>/auto_generate_json.py`). Parameters with a single value are in `designs/fixed_parameters.csv`. |
| `generated` | `YES` if the design's HLS source was generated (it is regenerable from `config_path`). |
| `csynth` | `YES` (C-synthesis report present), `NO` (not run), `FAIL`. |
| `impl` | `YES` (place-and-route report present), `NO` (not in the implemented subset / not run), `FAIL`. |
| `config_path` | The JSON config that generates the design: `configs/<domain>/<design_id>.json`. |
| `csynth_report`, `impl_report`, `impl_power_report` | `reports/<domain>/<design_id>/{csynth.xml, export_impl.xml, power_routed.rpt}`; empty unless that stage is `YES`. |
| `over_capacity` | `YES` if the csynth estimate exceeds any ZCU102 limit (BRAM_18K 1,824; DSP 2,520; LUT 274,080; FF 548,160). The report shows which resource. Empty if csynth was not run. |
| `fail_reason` | Only for `FAIL` rows, prefixed with the stage, e.g. `impl: Vivado 2024.1.2 segfault in opt_design ...`. |

Paths are relative to the extracted release bundle: extract every `configs_*.tar.gz` and `reports_*.tar.gz` next to
each other and every path in these files resolves. (The archives exist only because Zenodo limits files per record.)
Metrics (LUT, DSP, cycles, power, Fmax, ...) are not repeated here: they are in
`results/ml_sweep/{csynth,impl}/metrics_<domain>.csv`, keyed by `design_id`, and are re-derivable from the reports
(`forgebench collect`).

### Which designs were implemented (`impl`)
- dnn (R2): 1,000 = all 576 designs with no unrolling plus 53 chosen at random from each of the 8 unrolled
  (unroll_cin, unroll_cout) combinations; seed 20261014 (`forgebench/ml_sweep/select_impl.py`, list in `ml_sweep/dnn_impl_selection.csv`). Replaces
  the July set of 896, whose sampler was not preserved.
- gemm (R2): 1,000 = all 384 designs with no unrolling plus 88 chosen at random from each of the 7 unrolled
  (unroll_M, unroll_K, unroll_N) combinations; seed 20261002 (`forgebench/ml_sweep/select_impl.py`, list in `ml_sweep/gemm_impl_selection.csv`).
- llm (R2): 1,000, 250 at random for each `hd_unroll` value; seed 20261001 (`ml_sweep/llm_impl_selection.csv`).

## Other suites

| File | Rows | What |
|---|---|---|
| `designs/ops.csv` | 57 | The Table-2 operator variants verified in `verification/` (17 operators). `paper_table2_row` maps each to its Table 2 row; `generator_function` and `golden_function` point at the source and the NumPy reference. A bare `<op>.json` is skipped when `<op>__*.json` variants exist. |
| `designs/modular.csv` | 38 | Design directories behind Table 7: `program` (P1-P3) and `modularized_design` (the paper's "Overall"; contains the shared function(s)). `construction`: generated (the 26 programs, from `designs/configs/modular_designs/`) / manual (the 12 modularized designs; their testbench is built by `verification/merged_designs.py` from their programs' verified inputs). `design_path` is the directory in this repo. Reports: `reports/modular/<domain>/<name>/` holds `csynth.xml` plus one `<function>_csynth.xml` per function (Table 7's "Shared" column sums these). |
| `designs/modular_cases.csv` | 12 | The 12 rows of Table 7: `reuse_types` (the paper's tiling/functional/arithmetic marks), programs, modularized design, shared functions. |
| `designs/fullmodel.csv` | 14 | The Table 3 full-scale models: `model`, `variant` (full / tiled / prefill / decode), `format`, `config_path` (designs/configs/fullmodel), `hls_dir` (designs/hls/fullmodel), `csynth`, and the C-simulation runs behind the row (`csim_runs`, `csim_seeds`). Also the registry input of `forgebench.modular_designs.synth` for re-synthesizing them. |

## Verification: `verification/*.csv`

One CSV per verification result in the paper ([verification/README.md](../verification/README.md) maps each to its config, run
script and results folder): `operators.csv` (57 variants x 4 formats x {largest range, +-1}), `cosim.csv`, `catapult.csv`,
`table7_programs.csv` (26 programs), `table7_modularized.csv` (12 modularized designs x their programs), `fullmodel.csv` (one row
per Table 3 C-simulation run: 106).

Only designs the paper references are registered. Whole-design configs in `designs/configs/examples/` that are not part
of Table 7 are shipped as examples (Table 1), not as manifest entries.

## Regenerating

```
python manifest/rekey_results.py          # after collecting a run: map run names to design ids in results/ml_sweep/*
python manifest/build_manifest.py         # designs/{gemm,dnn,llm}.csv (add --check-disk to verify configs on disk)
python manifest/build_registry.py         # designs/{ops,modular,modular_cases,fullmodel}.csv
make verification-tables                  # verification/*.csv
python release/assemble_reports.py        # build configs/ + reports/ trees and tarballs; checks every manifest path exists
```
`ml_sweep/id_map.csv` maps each design id to the generator's parameter-encoded name (and the R1 name where the design
existed in R1). It freezes the numbering; `build_manifest.py` refuses to run if the generator order no longer matches it.
`ml_sweep/impl_failures.csv` gives the reason behind every `impl: FAIL` row.
