#!/bin/bash
# Copy the compact part of the verification runs into the tracked results/verification/<table>/<run>/ and rebuild the tables.
#   bash verification/experiments/collect_results.sh                       raw runs in verification/results_paper/<table>/<run>/
#   FULLMODEL=<verify-fullmodel OUT dir> bash verification/experiments/collect_results.sh   also the full-model runs
# Per run: summary.csv, resolved_config.json, configs/ (the exact design JSON of every experiment), rows/, sweep_curves/ (range
# search), run.log.gz; per-trial logs (trials/*.csv.gz) stay with the raw run, except for operators/*__pm1 and catapult/ (the
# Catapult table compares them seed by seed). Full models: summary.txt + gzipped summary.json and input manifests; each group
# OUT/<group>/ replaces results/verification/fullmodel/<group>/.
set -eu
cd "$(dirname "$0")/../.."
SRC=verification/results_paper
DST=results/verification
for run in $SRC/*/*/; do
  run=${run%/}; t=$(basename "$(dirname "$run")"); name=$(basename "$run")
  case $t in operators|cosim|catapult|table7) ;; *) continue ;; esac
  rm -rf "$DST/$t/$name"; mkdir -p "$DST/$t/$name"
  for f in summary.csv resolved_config.json configs sweep_curves rows logs; do
    [ -e "$run/$f" ] && cp -r "$run/$f" "$DST/$t/$name/"
  done
  case "$t/$name" in operators/*__pm1|catapult/*) [ -d "$run/trials" ] && cp -r "$run/trials" "$DST/$t/$name/" ;; esac
  [ -f "$SRC/$t/$name.log" ] && gzip -9c "$SRC/$t/$name.log" > "$DST/$t/$name/run.log.gz"
done
if [ -n "${FULLMODEL:-}" ]; then
  for G in "$FULLMODEL"/*/; do
    FM="$DST/fullmodel/$(basename "$G")"
    rm -rf "$FM"; mkdir -p "$FM"
    for r in "$G"/*/; do
      n=$(basename "$r"); mkdir -p "$FM/$n"
      for f in summary.json summary.txt manifest.json inputs/manifest.json; do
        [ -f "$r/$f" ] || continue
        case $f in
          *.json) gzip -9nc "$r/$f" > "$FM/$n/$(echo $f | tr / _).gz" ;;   # per-tensor errors; sha256 of every parameter tensor
          *) cp "$r/$f" "$FM/$n/" ;;
        esac
      done
    done
  done
fi
python3 verification/experiments/make_tables.py
du -sh "$DST"
