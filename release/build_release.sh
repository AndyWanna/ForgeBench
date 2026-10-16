#!/usr/bin/env bash
# Assemble the ForgeBench data release OUTSIDE git, then write SHA256SUMS and refresh
# release/bundle_files.csv. The human uploads release/_bundle/ to Zenodo + a GitHub Release.
#
# Usage: bash release/build_release.sh [BUNDLE_DIR]      (default: release/_bundle)
# Sources that do not exist are skipped with a warning (listed at the end), so the script
# can be re-run as more results (Catapult, HLSFactory, tool eval, full-model) appear.
# Inputs (maintainers only, not in git): the raw lean archives in checkpoints/ of this checkout, or of FORGEBENCH_ARCHIVES.
set -uo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
OUT="${1:-release/_bundle}"
ARCH="${FORGEBENCH_ARCHIVES:-.}/checkpoints"
CKPT_DIR="${CKPT_DIR:-$ARCH/20260720}"
MISSING=()

rm -rf "$OUT"; mkdir -p "$OUT"/{full_model,logs}

have() { [ -e "$1" ] || { MISSING+=("$1"); return 1; }; }

# (i)-(iii) configs/ and reports/ trees as the manifest paths describe them (regenerated configs + lean csynth/impl/modular
# reports from the July and R2 archives named in manifest/_common.py), one tarball per domain, each with an inner SHA256SUMS.
python3 release/assemble_reports.py --out "$OUT" && rm -rf "$OUT/_tree"

# (iv) full-model reports (Table 3). Configs and sources are in git (designs/{configs,hls}/fullmodel).
# scale_csynth_r1_lean: July csynth reports of ResNet-18/34 (plain+tiled), saved before the build trees were deleted.
have "$CKPT_DIR/scale_csynth_r1_lean.tar.gz" && cp "$CKPT_DIR/scale_csynth_r1_lean.tar.gz" "$OUT/full_model/"
# Hanqiu's single-seed full-model runs on the pre-R2 sources (24 variants, incl. the only record that Llama-3-8B fails at <16,5>):
# provenance only, not used by any paper claim. Not in this repository; archived from the pre-release history (commit e9eae9a5).
have "$ARCH/r2_cleanup/fullmodel_single_seed_pre_r2.tar.gz" && cp "$ARCH/r2_cleanup/fullmodel_single_seed_pre_r2.tar.gz" "$OUT/full_model/"

# Not in the paper, removed from the tree in the release cleanup: verification runs of other formats and reruns (Table 7 at <16,5>,
# mixed and float, R1 Table 7 runs, fix re-runs), the R1 C-sim development runs, the reduced-unroll Catapult synthesis, the impl run
# status logs. Archived from the pre-release history (commit 82bbdd50).
have "$ARCH/r2_cleanup/removed_from_repo_r2_cleanup.tar.gz" && cp "$ARCH/r2_cleanup/removed_from_repo_r2_cleanup.tar.gz" "$OUT/logs/"

# (v)+(vi) verification / CO-SIM / Catapult / HLSFactory / tool-eval logs
for d in verification/e2e results/catapult_vs_vitis results/hlsfactory/expected_output results/tool_eval; do
  if have "$d"; then tar -czf "$OUT/logs/$(echo "$d" | tr / _).tar.gz" "$d"; fi
done
have _run_logs && tar -czf "$OUT/logs/run_logs_r1_july.tar.gz" _run_logs
have verification/_csim_ops_n1000 && tar -czf "$OUT/logs/verification_csim_ops_n1000.tar.gz" verification/_csim_ops_n1000

( cd "$OUT" && find . -type f ! -name SHA256SUMS | sort | xargs sha256sum > SHA256SUMS )
python3 release/make_manifest.py --out "${MANIFEST_OUT:-release/bundle_files.csv}" --category bundle "$OUT"

echo; echo "Bundle: $OUT  ($(du -sh "$OUT" | cut -f1))"
if [ ${#MISSING[@]} -gt 0 ]; then
  echo "Skipped (not present yet):"; printf '  %s\n' "${MISSING[@]}"
fi
