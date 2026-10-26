#!/bin/bash
# C/RTL co-simulation (Table 3): the 57 Table-2 variants at +-1, 10 looped input sets, <32,10> and float, RTL vs C element by
# element. Needs Vitis HLS; run on an idle server (float: conv ~8 h, mha ~16.5 h).
#   JOBS=16 bash verification/experiments/run_cosim.sh [C_32_10 D_float]
# Raw outputs -> verification/results_paper/cosim/<config>/; then collect_results.sh.
cd "$(dirname "$0")/../.."
JOBS=${JOBS:-16}
OUT=verification/results_paper/cosim
mkdir -p $OUT
for c in ${@:-C_32_10 D_float}; do
  name=${c}__cosim
  python3 -m verification.functional_verification --config verification/experiments/cosim/$name.json --jobs $JOBS \
      --out $OUT/$name > $OUT/$name.log 2>&1 &
done
wait
echo ALL_DONE
