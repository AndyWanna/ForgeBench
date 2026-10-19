# ForgeBench

ForgeBench is a generator for HLS benchmarks of ML accelerators. You describe an accelerator as a JSON dataflow config (buffers,
operators, data type), and ForgeBench writes a synthesizable Vitis HLS project for it: the top function (`top.cpp` / `top.h`), a
self-checking testbench (`tb_top.cpp`), input data and the `run_hls.tcl` script. Four generators cover GEMM, DNN (convolution),
LLM (attention / transformer) and full-scale models (ResNet-18…152, Llama-3-8B). The GEMM generator also has a Catapult HLS backend.

The benchmark suites in the paper are all produced by these generators. Their configs, the generated sources and the collected
results are in this repository, including the verification results (`results/verification/`). The synthesis and implementation
reports are in the data release (see [Data release](#data-release)).

## Quick start

```sh
git clone <this repo> && cd ForgeBench
pip install -e .                       # forgebench CLI + analysis dependencies (Python >= 3.12)
pip install -e '.[verify]'             # optional: torch + pytest, for the full-model verifier and the tests

# generate an HLS project from a config (the generator is picked from the path or the ops; --domain to override)
forgebench generate designs/configs/examples/dnn/vgg19_block1.json -o out
cd out/vgg19_block1 && vitis_hls -f run_hls.tcl          # needs Vitis HLS

# change the data type or the tasks in run_hls.tcl
forgebench generate designs/configs/examples/gemm/mlp.json -o out --data-type "fixed<32,10,rnd,sat>" --task csim csynth

# regenerate the ML sweep configs of one domain (compare with designs/configs/ml_sweep/<domain>)
forgebench sweep-configs llm -o out/llm_configs

# rebuild every paper table and figure from results/ (no EDA tools needed) -> paper_artifacts/out/
make paper
make check                             # every committed design and sweep config regenerates byte-identically
```

`make help` lists all targets. [REPRODUCE.md](REPRODUCE.md) maps each table and figure of the paper to its inputs and commands.

## Repository layout

```
forgebench/                 the package (code only)
  gemm/ dnn/ llm/ fullmodel/   generators: auto_generate_json.py (sweep configs), gen_configs.py + generate_code.py + *_template.cpp
  backends/                    Vitis and Catapult backends (types, pragmas, math library binding)
  ml_sweep/                    run.py: csynth/impl sweeps with bounded disk use; select_impl.py: the seeded impl sample
  modular_designs/             synth.py: csynth of saved designs; parse_reports.py: Table 10
  analysis/                    report extractors (Vitis, Catapult) -> metrics.csv
  cli.py, paths.py             the `forgebench` command; the repository layout
designs/
  configs/                     every config the paper uses
    ml_sweep/{gemm,dnn,llm}/     the 12,144 sweep configs (3,072 / 5,184 / 3,888)
    modular_designs/<domain>/    the 26 Table 10 programs
    examples/<domain>/           example designs (VGG-19 / ResNet blocks, MLP, GEMM for Vitis and Catapult)
    fullmodel/                   the 14 Table 5 full-scale models
  hls/                         the generated (or hand-written) HLS sources, mirroring configs/
results/                     the collected results every paper table and figure is built from (inputs only; `make paper` builds the outputs)
  ml_sweep/{csynth,impl}/      metrics per design (12,144 csynth, 2,961 implementation)
  modular_designs/             Table 10 csynth utilization
  fullmodel/                   Table 5 csynth metrics
  verification/                one folder per verification result: operators/ cosim/ catapult/ table7/ fullmodel/
  catapult_vs_vitis/           Catapult vs Vitis synthesis of 1,000 GEMM designs
verification/                functional verification: engine, golden references, the 57 operator designs, full-model verifier;
                             experiments/ holds the paper configs and one run script per result (see verification/README.md)
paper_artifacts/             scripts that turn results/ into the paper's tables and figures (-> paper_artifacts/out/)
manifest/                    what every result is made of: designs/ (every design), verification/ (every verified number),
                             ml_sweep/ (design ids, implementation samples and failures)
release/                     scripts that assemble the data release
docs/                        data types, tool versions
```

## Config format

A config lists on-chip buffers (`brams`), off-chip arrays (`drams`, with their AXI bundle) and an ordered set of operators (`ops`).
Each operator names a generator function (`func_name`: `load`, `store`, `gemm`, `conv`, `batchnorm`, `mha`, `rmsnorm`, …), its
dimensions and its buffer arguments. Optional fields set the data type (`data_type`, see [docs/DATA_TYPES.md](docs/DATA_TYPES.md)),
the HLS tasks (`task`), accumulator types (`acc_type`, …) and the tool (`tool`).

```json
{
  "brams": [{"name": "BRAM_attn_input", "dims": [8, 256]}, ...],
  "drams": [{"name": "DRAM_attn_input", "dims": [8, 256], "bundle": "mem1"}, ...],
  "ops": {
    "load_1": {"func_name": "load", "dims": [8, 256], "args": ["DRAM_attn_input", "BRAM_attn_input"]},
    ...
  },
  "output_dram_names": ["DRAM_output"]
}
```

The configs under `designs/configs/` are working examples of every operator and composition the generators support.

## What is in the benchmark

| Suite | Designs | Configs | Sources | Results |
|---|---|---|---|---|
| ML sweep (GEMM / DNN / LLM) | 3,072 / 5,184 / 3,888; implementation sample 1,000 per domain (999 / 1,000 / 962 completed) | `designs/configs/ml_sweep/` | regenerated (`forgebench generate`), full set in the data release | `results/ml_sweep/` |
| Operators (Table 2) | 57 variants of 17 operators | `verification/operators/*/variants/` | generated by the verification harness | `results/verification/{operators,cosim,catapult}/` |
| Modularization (Table 10) | 12 test cases: 26 generated programs + 12 hand-written modularized designs | `designs/configs/modular_designs/` | `designs/hls/modular_designs/` | `results/modular_designs/`, `results/verification/table7/` |
| Full-scale models (Table 5) | ResNet-18…152 full `<32,10>` and tiled `<16,5>`, Llama-3-8B prefill / decode at ctx 2048 and 8192 (14) | `designs/configs/fullmodel/` | `designs/hls/fullmodel/` | `results/fullmodel/`, `results/verification/fullmodel/` |
| Examples | 21: VGG-19 and ResNet-18/50 blocks, an MLP, the GEMM of the JSON example figure (Vitis and Catapult) | `designs/configs/examples/` | `designs/hls/examples/` | — |

Every design has an id and a status row in `manifest/designs/` (`gemm.csv`, `dnn.csv`, `llm.csv`, `ops.csv`, `modular.csv`,
`fullmodel.csv`); see [manifest/README.md](manifest/README.md).

## Data release

The raw tool reports are archived on Zenodo: [doi:10.5281/zenodo.23425257](https://doi.org/10.5281/zenodo.23425257).

| File | Contents |
|---|---|
| `configs_{gemm,dnn,llm}.tar.gz` | the 12,144 sweep configs under their design ids (`configs/<domain>/<design_id>.json`) |
| `reports_{gemm,dnn,llm}.tar.gz` | lean csynth (`csynth.xml`) and implementation (`export_impl.xml`, `power_routed.rpt`) reports of every sweep design |
| `reports_modular.tar.gz` | csynth reports of the 38 modularization designs, with one `<function>_csynth.xml` per function |
| `reports_fullmodel.tar.gz` | `csynth.xml` / `csynth.rpt` of the 14 full model-level designs |
| `reports_catapult_vs_vitis.tar.gz` | Catapult `rtl.rpt` and Vitis `csynth.xml` of the 1,000 GEMM designs of the Catapult comparison |
| `tool_eval_logs.tar.gz` | full logs of the source-to-source tool evaluation (Table 9), also committed as `results/tool_eval/tool_eval_logs.tar.gz` next to the results, inputs, emitted HLS and short logs |
| `SHA256SUMS` | checksums of the files above (`sha256sum -c SHA256SUMS`); each tarball also contains a SHA256SUMS of its members |

Extract the tarballs next to each other and every report path in `manifest/designs/*.csv` resolves. The machine name in the tool
reports is replaced by `<host>`; nothing else is changed. `release/bundle_files.csv` lists every file with its size and SHA-256, and
`make bundle` rebuilds the bundle (maintainers; it needs the raw lean archives). The verification results are in git
(`results/verification/`). Nothing in `make paper` or `make check` needs the data release.

## Tools

Vitis HLS / Vivado 2024.1.2 (target ZCU102, `xczu9eg-ffvb1156-2-e`, 10 ns), Catapult 2026.2 for the Catapult backend; exact
versions in [docs/TOOL_VERSIONS.md](docs/TOOL_VERSIONS.md). Generating designs and rebuilding the paper's tables needs only Python.

## License

MIT, see [LICENSE](LICENSE). ForgeBench was first developed in [hchen799/ForgeBench](https://github.com/hchen799/ForgeBench); this
repository continues it for the journal version of the paper.
