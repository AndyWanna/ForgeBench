#!/bin/bash
# Table 7 verification at <32,10>: (1) the 26 programs, whole designs end to end, C simulation, N=100 at +-1 (table7/C_32_10__table7.json);
# (2) each of the 12 modularized designs driven with its programs' inputs and compared with every program of its row. Needs Vitis HLS.
#   JOBS=6 bash verification/experiments/run_table7.sh        (conv_block_op1 dominates: ~10 h)
# Raw outputs -> verification/results_paper/table7/{C_32_10__table7,C_32_10__table7_modularized}/; then collect_results.sh.
cd "$(dirname "$0")/../.."
JOBS=${JOBS:-6}
OUT=verification/results_paper/table7
mkdir -p $OUT
python3 -m verification.functional_verification --config verification/experiments/table7/C_32_10__table7.json --jobs $JOBS \
    --out $OUT/C_32_10__table7 > $OUT/C_32_10__table7.log 2>&1 || exit 1
python3 -m verification.merged_designs --programs-run $OUT/C_32_10__table7 --out $OUT/C_32_10__table7_modularized --jobs $JOBS \
    > $OUT/C_32_10__table7_modularized.log 2>&1
echo ALL_DONE
