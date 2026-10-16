"""Regenerate every committed design and byte-compare it with designs/hls/ (no EDA tools needed).

    python -m verification.check_designs [--sweep]

  examples         designs/configs/examples/<gen>/*.json      -> designs/hls/examples/<gen>/<name>/
  fullmodel        designs/configs/fullmodel/*.json           -> designs/hls/fullmodel/<name>/
  modular designs  the Table 7 programs, generated from the verified design JSONs (verification.experiments.prepare_table7_synth)
                   -> designs/hls/modular_designs/<gen>/<program>/
  --sweep          also regenerate the 12,144 ML sweep configs and compare with designs/configs/ml_sweep/<gen>/

Compares top.cpp, top.h, tb_top.cpp and run_hls.tcl. Exit status 1 on any difference.
"""
import argparse
import csv
import filecmp
import glob
import os
import sys
import tempfile

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, REPO)
from forgebench import cli, paths  # noqa: E402

FILES = ("top.cpp", "top.h", "tb_top.cpp", "run_hls.tcl")


def same(a, b):
    return [f for f in FILES if os.path.isfile(os.path.join(b, f)) and
            not (os.path.isfile(os.path.join(a, f)) and filecmp.cmp(os.path.join(a, f), os.path.join(b, f), shallow=False))]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--sweep", action="store_true", help="also regenerate the ML sweep configs (a few minutes)")
    a = ap.parse_args()
    bad, n = [], 0
    with tempfile.TemporaryDirectory(prefix="forgebench_check_") as tmp:
        jobs = [(c, os.path.join(paths.HLS, "examples", g, os.path.basename(c)[:-5]), g)
                for g in ("gemm", "dnn", "llm") for c in sorted(glob.glob(os.path.join(paths.CONFIGS, "examples", g, "*.json")))]
        jobs += [(c, os.path.join(paths.HLS, "fullmodel", os.path.basename(c)[:-5]), "fullmodel")
                 for c in sorted(glob.glob(os.path.join(paths.CONFIGS, "fullmodel", "*.json")))]
        for cfg, ref, dom in jobs:
            out = cli.generate(cfg, os.path.join(tmp, dom), dom)
            diff = same(out, ref)
            n += 1
            if diff:
                bad.append(f"{os.path.relpath(ref, REPO)}: {', '.join(diff)}")
        print(f"examples + full models: {n} designs regenerated", flush=True)

        from verification.experiments import prepare_table7_synth
        t7 = os.path.join(tmp, "table7")
        prepare_table7_synth.main(["--out", t7])
        with open(os.path.join(REPO, "manifest", "designs", "modular.csv"), newline="") as f:
            progs = [r for r in csv.DictReader(f) if r["role"] == "program"]
        for r in progs:
            diff = same(os.path.join(t7, r["domain"], os.path.basename(r["design_path"])), os.path.join(REPO, r["design_path"]))
            if diff:
                bad.append(f"{r['design_path']}: {', '.join(diff)}")
        print(f"Table 7 programs: {len(progs)} regenerated", flush=True)

        if a.sweep:
            for g in ("gemm", "dnn", "llm"):
                out = os.path.join(tmp, "sweep", g)
                k = cli.sweep_configs(g, out)
                d = filecmp.dircmp(out, paths.sweep_configs(g))
                mism = d.left_only + d.right_only + filecmp.cmpfiles(out, paths.sweep_configs(g), d.common_files, shallow=False)[1]
                print(f"sweep configs {g}: {k} regenerated, {len(mism)} differ", flush=True)
                bad += [f"designs/configs/ml_sweep/{g}/{x}" for x in mism]
    for b in bad:
        print("DIFFERS", b)
    print("OK: every committed design regenerates byte-identically" if not bad else f"{len(bad)} differences")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
