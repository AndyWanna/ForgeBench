"""Table 10 synthesis sources: every program and modularized design at <32,10> (truncate/wrap), the format they were verified in,
built the same way verification built them:

  programs (26)             gen_configs.run_hls_flow on the verified design JSON (results/verification/table7/C_32_10__table7/configs), data_type fixed<32,10>, task csynth
                            (verification.prepare_designs.generate_design)
  modularized designs (12)  designs/hls/modular_designs/<gen>/<name> with data_t/acc_t/math_t rebound plus every spelled-out
                            ap_fixed<16,5> (copied generator code) replaced, own run_hls.tcl; the same sources
                            verification.merged_designs checks against every program of the row (table7/C_32_10__table7_modularized)

Writes <out>/<domain>/<name>/{top.cpp,top.h,tb_top.cpp,run_hls.tcl} and <out>/registry.csv (manifest/designs/modular.csv rows with hls_dir
pointing at <out>), the input of forgebench.modular_designs.synth:

  python3 -m verification.experiments.prepare_table7_synth --out _sweeps/table7_32_10
  python3 -m forgebench.modular_designs.synth --registry _sweeps/table7_32_10/registry.csv --lean-out _sweeps/lean/modular_csynth_32_10 --jobs 38
"""
import argparse
import csv
import glob
import os
import re
import shutil
import sys

from verification import typedefs as M
from forgebench import paths
from verification.prepare_designs import generate_design

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
DTYPE = "fixed<32,10>"
# verified program runs; a later run overrides an earlier one (vec_mtx_p2, conv_block_op3, llama_transformer_p2 re-verified after fixes)
RUNS = ("C_32_10__table7",)
GEN_CONFIGS = []
KEEP = ("top.cpp", "top.h", "tb_top.cpp", "run_hls.tcl")
OLD_TYPE = re.compile(r"ap_fixed\s*<\s*16\s*,\s*5\s*>")


def rebind(src, dst):
    """Copy a hand-written design with its typedefs rebound to DTYPE (verification.typedefs), keeping its own tcl/testbench."""
    os.makedirs(dst, exist_ok=True)
    tys = M.types(DTYPE)
    for fn in os.listdir(src):
        p = os.path.join(src, fn)
        if not os.path.isfile(p) or fn.endswith((".log", ".jou")):
            continue
        if fn in ("top.cpp", "top.h"):
            text = open(p).read()
            for name, c in tys.items():
                text = M.typedef_re(name).sub(f"typedef {c} {name};", text)
            # merged designs copied generated functions with the type spelled out (ap_fixed<16, 5> in signatures/locals); function
            # names (..._ap_fixed_16_5_) are left alone: Table 10's Shared column looks the per-function reports up by name
            text = OLD_TYPE.sub(tys["data_t"], text)
            open(os.path.join(dst, fn), "w").write(text)
        else:
            shutil.copy2(p, dst)


def main(argv=None):
    global DTYPE
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", required=True)
    ap.add_argument("--dtype", default=DTYPE, help="data type of the designs, e.g. fixed<16,5> (default %(default)s)")
    ap.add_argument("--runs", nargs="+", default=RUNS, help="verification runs (results/verification/table7/<run>) holding the program configs; later wins")
    a = ap.parse_args(argv)
    DTYPE = a.dtype
    GEN_CONFIGS[:] = [os.path.join(REPO, "results", "verification", "table7", r, "configs") for r in a.runs]
    tag = DTYPE.replace("<", "_").replace(",", "_").replace(">", "").replace(" ", "")
    out = os.path.abspath(a.out)
    shutil.rmtree(out, ignore_errors=True)
    os.makedirs(out)
    rows, how = [], {}
    with open(os.path.join(REPO, "manifest", "designs", "modular.csv"), newline="") as f:
        reg = list(csv.DictReader(f))
    for r in reg:
        dom, name = r["domain"], os.path.basename(r["design_path"])
        dst = os.path.join(out, dom, name)
        key = f"{dom}/{name}"
        if r["role"] == "program":
            rec = paths.RECORDED.get(dom, dom)   # the verification runs were recorded before conv -> dnn
            cands = [os.path.join(d, f"designs__{x}__{name}__{tag}_trn_wrap.json") for d in GEN_CONFIGS for x in dict.fromkeys((dom, rec))]
            cands = [c for c in cands if os.path.isfile(c)]
            if not cands:
                sys.exit(f"no verified config for {key} in {GEN_CONFIGS}")
            cfg = cands[-1]
            work = os.path.join(out, "_gen", dom)
            os.makedirs(work, exist_ok=True)
            run_dir, _ = generate_design(dom, cfg, work, ["csynth"], data_type=DTYPE)
            os.makedirs(dst, exist_ok=True)
            for fn in KEEP:
                if os.path.isfile(os.path.join(run_dir, fn)):
                    shutil.copy2(os.path.join(run_dir, fn), dst)
            how[key] = "generated from the verified design JSON"
        else:
            rebind(os.path.join(REPO, r["design_path"]), dst)
            how[key] = "modularized design, typedefs rebound (verified against its programs: verification.merged_designs)"
        rows.append({**r, "hls_dir": os.path.relpath(dst, REPO), "name": name, "source": how[key]})
    shutil.rmtree(os.path.join(out, "_gen"), ignore_errors=True)
    with open(os.path.join(out, "registry.csv"), "w", newline="") as f:
        w = csv.DictWriter(f, list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    for k in sorted(how):
        print(f"{k:<40} {how[k]}")
    print(f"{len(rows)} designs -> {out}/registry.csv")


if __name__ == "__main__":
    main()
