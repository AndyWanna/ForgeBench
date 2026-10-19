#!/bin/bash
# Full-scale model verification for every Table 5 design (Vitis HLS C simulation of the generated accelerators) with
# verification/fullmodel/verify.py: every run compares the accelerator against (a) the integer-code fixed-point
# PyTorch reference (bit-exact: mismatching codes) and (b) the FP64 PyTorch graph (relative L2 <= 5% at every observed tensor).
# One seed = one synthetic parameter set + input (DRAM_input uniform +-1; weights +-sqrt(3/fan_in); BN gamma/var [0.9,1.1], beta/mean
# +-0.05; Llama: embedding +-0.5, norm weights [0.9,1.1], 4 prefill + 2 decode tokens; prefill and decode projects run together).
#
#   OUT=<new results dir> [SOURCES=designs/hls/fullmodel] [ONLY=<regex>] [SEEDS="42 ..."] [PAR=3] [PY=python3] \
#       bash verification/experiments/run_fullmodel.sh            (or: make verify-fullmodel OUT=<dir>)
#
# The run list is manifest/designs/fullmodel.csv: the 10 ResNet projects (full <32,10> and tiled <16,5>) x 10 seeds (42-51) and the
# two Llama-3-8B prefill/decode pairs (ctx 2048, 8192) x 3 seeds (42-44). Results land in the groups Table 5 reads:
#   OUT/resnet_full_32_10/<project>seed<n>/   OUT/resnet_tiled_16_5_trn/...   OUT/llama_ctx{2048,8192}_32_10/LLAMA3_8B_ctx<c>_32_10_seed<n>/
# ONLY filters on "<group>/<run>" (e.g. ONLY=tiled, ONLY=RESNET50, ONLY=llama_ctx8192); SEEDS overrides the seed list of every design.
# Time per seed: ResNet 0.4-4 min, Llama ~16 min plus a ~33 GB temporary parameter store (deleted after the run; its manifest and
# sha256s remain). ResNets run PAR at a time (8 threads each), Llama one at a time (16 threads): ~2.5 h for all 106 runs at PAR=3.
# Then: FULLMODEL=<OUT> bash verification/experiments/collect_results.sh   (compact copy into results/verification/fullmodel/)
# Do not edit verification/fullmodel/references/contracts.json while this runs (verify.py checks it is unchanged).
set -u
REPO=$(cd "$(dirname "$0")/../.." && pwd)
PY=${PY:-python3}
PAR=${PAR:-3}
SOURCES=${SOURCES:-$REPO/designs/hls/fullmodel}
ONLY=${ONLY:-}
SEEDS=${SEEDS:-}
: "${OUT:?set OUT to a new results directory}"
mkdir -p "$OUT"
OUT=$(cd "$OUT" && pwd)
LOG="$OUT/fullmodel_runs.txt"
cd "$REPO"

# one line per run: group run family project decode_project seed
jobs=$("$PY" - "$SEEDS" <<'EOF'
import csv, os, sys
seeds = sys.argv[1].split()
rows = list(csv.DictReader(open("manifest/designs/fullmodel.csv")))
for r in rows:
    group = os.path.basename(r["csim_runs"])
    ss = seeds or [str(42 + i) for i in range(int(r["csim_seeds"]))]
    if r["model"].startswith("ResNet"):
        fam = "resnet" + r["model"].split("-")[1]
        for s in ss:
            print(group, f"{r['name']}seed{s}", fam, r["name"], "-", s)
    elif r["variant"] == "prefill":
        dec = r["name"].replace("PREFILL", "DECODE")
        ctx = r["name"].split("_")[3]
        for s in ss:
            print(group, f"LLAMA3_8B_{ctx}_32_10_seed{s}", "llama3", r["name"], dec, s)
EOF
) || exit 1
[ -n "$ONLY" ] && jobs=$(echo "$jobs" | awk -v re="$ONLY" '($1 "/" $2) ~ re')
[ -n "$jobs" ] || { echo "no runs match ONLY=$ONLY"; exit 1; }
echo "$(echo "$jobs" | wc -l) runs -> $OUT"

run() {   # group run family project decode_project seed
  local g=$1 n=$2 f=$3 p=$4 d=$5 s=$6 o
  mkdir -p "$OUT/$g"; o="$OUT/$g/$n"
  rm -rf "$o"
  if [ "$f" = llama3 ]; then
    "$PY" verification/fullmodel/verify.py --family llama3 --project "$SOURCES/$p" --decode-project "$SOURCES/$d" \
        --prefill 4 --decode 2 --seed "$s" --timeout 14400 --threads 16 --output "$o" > "$o.log" 2>&1
    rm -rf "$o/inputs"/*.bin
  else
    "$PY" verification/fullmodel/verify.py --family "$f" --project "$SOURCES/$p" --seed "$s" --threads 8 --timeout 7200 \
        --output "$o" > "$o.log" 2>&1
  fi
  echo "$g/$n $(head -n 1 "$o/summary.txt" 2>/dev/null || echo NO_SUMMARY) $(grep -o 'mismatches=[0-9]* .*relative_l2=[0-9.e-]*' "$o/summary.txt" 2>/dev/null | head -n 1)" >> "$LOG"
}
export -f run
export PY SOURCES OUT LOG

echo "$jobs" | awk '$3 != "llama3"' | xargs -r -P "$PAR" -n 6 bash -c 'run "$@"' _
echo "$jobs" | awk '$3 == "llama3"' | xargs -r -P 1 -n 6 bash -c 'run "$@"' _
echo ALL_DONE >> "$LOG"
grep -c '^[^ ]* PASS' "$LOG" | xargs -I{} echo "{} of $(echo "$jobs" | wc -l) runs PASS (log: $LOG)"
