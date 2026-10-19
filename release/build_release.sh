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

# Tool-evaluation logs (Case Study 3, tab:tool-support): the full transcripts behind results/tool_eval/, committed there
# by results/tool_eval/scripts/archive_logs.py; name, size and SHA-256 are pinned in results/tool_eval/archive.json.
LOGS=results/tool_eval/tool_eval_logs.tar.gz
"$PY" - "$LOGS" results/tool_eval/archive.json <<'PYEOF'
import hashlib, json, os, sys
path, meta = sys.argv[1], json.load(open(sys.argv[2]))
if not os.path.isfile(path):
    sys.exit(f"missing {path} (tool-evaluation logs, see results/tool_eval/archive.json)")
h = hashlib.sha256(open(path, "rb").read()).hexdigest()
if (h, os.path.getsize(path)) != (meta["sha256"], meta["bytes"]):
    sys.exit(f"{path}: sha256 {h} / {os.path.getsize(path)} bytes, archive.json expects {meta['sha256']} / {meta['bytes']}")
PYEOF

# configs_<domain>.tar.gz, reports_<domain>.tar.gz (sweeps), reports_modular.tar.gz (tab:modularization_results),
# reports_fullmodel.tar.gz (tab:ml_model_suite), reports_catapult_vs_vitis.tar.gz (fig:catapult_vs_vitis):
# the trees the manifest paths describe, regenerated configs + lean reports, machine names scrubbed.
"$PY" release/assemble_reports.py --out "$OUT"
rm -rf "$OUT/_tree"

# the tool-evaluation logs, checked above
cp "$LOGS" "$OUT/tool_eval_logs.tar.gz"

( cd "$OUT" && find . -maxdepth 1 -type f ! -name SHA256SUMS -printf '%f\n' | sort | xargs sha256sum > SHA256SUMS )
rm -f release/bundle_files.csv
"$PY" release/make_manifest.py --out release/bundle_files.csv --category bundle --root "$OUT" "$OUT"

echo; echo "Bundle: $OUT  ($(du -sh "$OUT" | cut -f1), $(find "$OUT" -type f | wc -l) files)"
