#!/bin/bash
# Catapult vs Vitis synthesis on the 1,000-design GEMM sample (paper_artifacts/catapult_vs_vitis.py), both tools at once:
# 32 Vitis csynth jobs + 32 Catapult jobs (64 cores), 1 h per design per tool; resumable (designs already ok are skipped).
#   bash forgebench/ml_sweep/scripts/run_catapult_vs_vitis.sh [vitis_jobs] [catapult_jobs]
cd "$(dirname "$0")/../../.."
OUT=${OUT:-_sweeps/catapult_vs_vitis}
VJ=${1:-32}; CJ=${2:-32}
[ -d $OUT/configs ] || python3 -m paper_artifacts.catapult_vs_vitis sample --out $OUT
# Catapult 2026.2: set MGC_HOME to its Mgc_home and the Siemens license variables (SALT_LICENSE_SERVER / MGLS_LICENSE_FILE) first.
: "${MGC_HOME:?set MGC_HOME to the Catapult 2026.2 Mgc_home (and the Siemens license environment)}"
export PATH=$PATH:$MGC_HOME/bin          # appended: Mgc_home/bin also ships a python3
command -v vitis_hls >/dev/null || { echo "vitis_hls not on PATH"; exit 1; }
python3 -m forgebench.ml_sweep.run --domain gemm --flow csynth --tool vitis --jobs $VJ --timeout 3600 \
    --configs-dir $OUT/configs --lean-out $OUT/lean_vitis ${LIMIT:+--limit $LIMIT} > $OUT/vitis.log 2>&1 &
python3 -m forgebench.ml_sweep.run --domain gemm --flow csynth --tool catapult --jobs $CJ --timeout 3600 \
    --configs-dir $OUT/configs --lean-out $OUT/lean_catapult ${LIMIT:+--limit $LIMIT} > $OUT/catapult.log 2>&1 &
wait
python3 -m paper_artifacts.catapult_vs_vitis collect --out $OUT
echo "$(date '+%F %T') ALL_DONE"
