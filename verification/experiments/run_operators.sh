#!/bin/bash
# Operator verification (Table 2): the 57 Table-2 variants, C simulation, N=100, formats A <16,5>, B mixed, C <32,10>,
# D float, each at its largest faithful input range (operators/*__max_range.json) and at +-1 (operators/*__pm1.json). Needs Vitis HLS.
#   JOBS=14 bash verification/experiments/run_operators.sh [A B C D]      (~1.5 h on 128 cores)
# Raw outputs -> verification/results_paper/operators/<config>/; then collect_results.sh.
cd "$(dirname "$0")/../.."
JOBS=${JOBS:-14}
OUT=verification/results_paper/operators
mkdir -p $OUT
for col in ${*:-A B C D}; do
  for cfg in verification/experiments/operators/${col}_*.json; do
    name=$(basename "$cfg" .json)
    python3 -m verification.functional_verification --config "$cfg" --jobs $JOBS --out $OUT/$name > $OUT/$name.log 2>&1 &
  done
done
wait
echo ALL_DONE
