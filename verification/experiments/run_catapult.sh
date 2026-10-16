#!/bin/bash
# Catapult backend check (the Catapult table): the Table-2 matrix-product variants (GEMM, vmm, mmv, dot; each without and with bias)
# generated with the Catapult backend, Catapult SCVerify C simulation, N=100 at +-1, <32,10> and float: same inputs, seeds, golden and
# bound as the Vitis operators/{C_32_10,D_float}__pm1 runs, so every trial is compared seed by seed. Needs Catapult 2026.2 + license.
#   bash verification/experiments/run_catapult.sh
# Raw outputs -> verification/results_paper/catapult/<config>/; then collect_results.sh.
cd "$(dirname "$0")/../.."
# Catapult 2026.2: set MGC_HOME to its Mgc_home and the Siemens license variables (SALT_LICENSE_SERVER / MGLS_LICENSE_FILE) first.
: "${MGC_HOME:?set MGC_HOME to the Catapult 2026.2 Mgc_home (and the Siemens license environment)}"
export PATH=$PATH:$MGC_HOME/bin          # appended: Mgc_home/bin also ships a python3
[ "$(dirname "$(readlink -f "$(command -v catapult)")")" = "$(readlink -f $MGC_HOME/bin)" ] || { echo "catapult on PATH is not 2026.2"; exit 1; }
OUT=verification/results_paper/catapult
mkdir -p $OUT
for c in C_32_10 D_float; do
  name=${c}__catapult_pm1
  python3 -m verification.functional_verification --config verification/experiments/catapult/$name.json --jobs 2 \
      --out $OUT/$name > $OUT/$name.log 2>&1 &
done
wait
echo ALL_DONE
