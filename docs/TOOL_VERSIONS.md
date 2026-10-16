# Tool versions

Versions of the tools behind the R2 results, as reported by the tools themselves on the run server (chao-srv2.ece.gatech.edu).

## Vitis (all sweeps, operator / Table 7 verification)

| Tool | Version | Source |
|---|---|---|
| Vitis HLS | v2024.1.2 (64-bit), SW Build 5096458 (Sep 5 2024) | `vitis_hls -version` |
| Vivado | 2024.1.2 | implementation runs |

## Catapult backend check (verification/experiments/run_catapult.sh)

| Tool | Version | Source |
|---|---|---|
| Catapult | Catapult Prime Synthesis 2026.2/1292347 (Production Release, May 14 2026) | session banner of every run |
| `ac_fixed` / `ac_std_float` / `ac_math` headers | shipped with Catapult 2026.2 (`Mgc_home/shared/include`) | header banner |
| C simulation | SCVerify `Verify_orig_cxx_osci.mk`, Catapult-bundled g++ (Calypto) 13.4.0 (`Mgc_home/pkgs/dcs_gcc`) | Catapult transcript |
| RTL simulation | Questa Sim-64 2026.1_2, SCVerify `Verify_rtl_v_msim.mk` | `vsim -version` |
| Target library | `mgc_Xilinx-ZYNQ-uplus-2_beh` (Vivado flow, part `xczu9eg-ffvb1156-2-e`, the Vitis part) + `Xilinx_RAMS`; clock 10 ns. The earlier C-sim runs and a first synthesis pass used `nangate-45nm_beh` (C-sim results do not depend on the library) | `forgebench/backends/catapult.py` (`DEFAULT_LIBS = ZU9EG_LIBS`) |

Catapult and Questa need a Siemens license (`SALT_LICENSE_SERVER` / `MGLS_LICENSE_FILE`); the Catapult scripts take the install from `MGC_HOME`, Questa from `QUESTA_BIN` or `vsim` on `PATH`. Vitis HLS is taken from `vitis_hls` on `PATH` (or `XILINX_HLS`).
`run_catapult.sh` exports those two variables; with the `MGLS_LICENSE_FILE` set in `~/.bashrc` alone, Catapult 2026.2 waits indefinitely
after its banner.
