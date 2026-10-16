#!/usr/bin/env bash
# Assemble the ForgeBench data release OUTSIDE git, then write SHA256SUMS and rewrite release/bundle_files.csv.
# The human uploads every file of release/_bundle/ to the Zenodo dataset record (DOI 10.5281/zenodo.23425257).
#
# Usage: bash release/build_release.sh [BUNDLE_DIR]      (default: release/_bundle)
# Zenodo records have no folders, so the bundle is flat: one tarball per suite, each with an inner SHA256SUMS, plus a top-level
# SHA256SUMS of the tarballs (bare file names, so `sha256sum -c SHA256SUMS` works in the download directory).
# Inputs (maintainers only, not in git): the raw lean archives in checkpoints/ of this checkout, or of FORGEBENCH_ARCHIVES.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
OUT="${1:-release/_bundle}"
PY="${PY:-python3}"

rm -rf "$OUT"; mkdir -p "$OUT"

# configs_<domain>.tar.gz, reports_<domain>.tar.gz (sweeps), reports_modular.tar.gz (tab:modularization_results),
# reports_fullmodel.tar.gz (tab:ml_model_suite), reports_catapult_vs_vitis.tar.gz (fig:catapult_vs_vitis):
# the trees the manifest paths describe, regenerated configs + lean reports, machine names scrubbed.
"$PY" release/assemble_reports.py --out "$OUT"
rm -rf "$OUT/_tree"

# Tool-evaluation results (Case Study 3, tab:tool-support), once they are in results/tool_eval/.
if [ -n "$(ls -A results/tool_eval 2>/dev/null)" ]; then
  tar -czf "$OUT/results_tool_eval.tar.gz" results/tool_eval
else
  echo "note: results/tool_eval/ is empty, not bundled"
fi

( cd "$OUT" && find . -maxdepth 1 -type f ! -name SHA256SUMS -printf '%f\n' | sort | xargs sha256sum > SHA256SUMS )
rm -f release/bundle_files.csv
"$PY" release/make_manifest.py --out release/bundle_files.csv --category bundle --root "$OUT" "$OUT"

echo; echo "Bundle: $OUT  ($(du -sh "$OUT" | cut -f1), $(find "$OUT" -type f | wc -l) files)"
