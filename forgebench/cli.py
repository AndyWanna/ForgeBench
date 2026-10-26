"""`forgebench` command line.

    forgebench generate CONFIG.json [-o OUT] [--domain D] [--data-type T] [--task csynth ...] [--tool vitis|catapult]
    forgebench sweep-configs {gemm,dnn,llm} [-o DIR]       regenerate the ML sweep configs (default: a fresh dir, compare with
                                                           designs/configs/ml_sweep/<gen>)
    forgebench ml-sweep ...                                run a csynth/impl sweep (forgebench.ml_sweep.run; needs Vitis HLS)
    forgebench select-impl ...                             the seeded implementation sample (forgebench.ml_sweep.select_impl)
    forgebench collect ...                                 tool reports -> metrics.csv (forgebench.analysis.collect)
    forgebench paper {ml-model-suite,modularization,section42,verification,catapult-vs-vitis,all}   rebuild paper tables/figures from results/

Generators are run in a scratch copy of forgebench/<gen>/ (they read templates and write DRAM_*.txt in their cwd), so the
checkout is never written to except at OUT.
"""
import argparse
import glob
import json
import os
import shutil
import subprocess
import sys
import tempfile

from forgebench import paths

# ops that only one generator emits: used to pick the generator when --domain is not given and the path does not tell
_MARKERS = {
    "gemm": {"gemm", "vmm", "mmv", "dot_product"},
    "dnn": {"conv", "batchnorm", "maxpool", "adaptive_avgpool"},
    "llm": {"matmul", "mha", "swa", "layernorm", "rmsnorm", "elementwise_mult", "dropout"},
}


def infer_domain(config_path, cfg):
    parts = os.path.abspath(config_path).split(os.sep)
    if "configs" in parts:                                  # designs/configs/<kind>/<gen>/<name>.json, designs/configs/fullmodel/<name>.json
        i = parts.index("configs")
        tail = parts[i + 1:i + 3]
        if tail and tail[0] == "fullmodel":
            return "fullmodel"
        if len(tail) == 2 and tail[1] in ("gemm", "dnn", "llm"):
            return tail[1]
    if any(op.get("func_name") == "conv_tile" for op in cfg.get("ops", {}).values()):
        return "fullmodel"
    funcs = {op.get("func_name") for op in cfg.get("ops", {}).values()}
    hits = [d for d, m in _MARKERS.items() if funcs & m]
    if len(hits) == 1:
        return hits[0]
    sys.exit(f"cannot tell which generator {config_path} is for (ops {sorted(f for f in funcs if f)}); pass --domain")


def generate(config_path, out, domain=None, data_type=None, task=None, tool=None):
    """Generate one design from a JSON config into out/<name>/. Returns that directory."""
    with open(config_path) as f:
        cfg = json.load(f)
    domain = domain or infer_domain(config_path, cfg)
    if data_type:
        cfg["data_type"] = data_type
    if task:
        cfg["task"] = list(task)
    if tool:
        if domain != "gemm" and tool != "vitis":
            sys.exit(f"--tool {tool}: only the gemm generator has a non-Vitis backend")
        cfg["tool"] = tool
    name = os.path.splitext(os.path.basename(config_path))[0]
    out = os.path.abspath(out)
    with tempfile.TemporaryDirectory(prefix="forgebench_gen_") as tmp:
        work = os.path.join(tmp, "gen")
        shutil.copytree(paths.generator_dir(domain), work, ignore=shutil.ignore_patterns("__pycache__", "auto_generated_configs",
                                                                                        "large_hls_files", "hls_files"))
        with open(os.path.join(work, name + ".json"), "w") as f:
            json.dump(cfg, f, indent=4)
        call = f"import gen_configs; gen_configs.run_hls_flow({name + '.json'!r}, base_dir='out'" + \
               (f", task={cfg['task']!r})" if "task" in cfg else ")")
        r = subprocess.run([sys.executable, "-c", call], cwd=work, capture_output=True, text=True,
                           env=dict(os.environ, PYTHONPATH=paths.REPO + os.pathsep + os.environ.get("PYTHONPATH", "")))
        if r.returncode:
            sys.exit(f"generation failed for {config_path}:\n{r.stderr[-2000:]}")
        dst = os.path.join(out, name)
        shutil.rmtree(dst, ignore_errors=True)
        os.makedirs(out, exist_ok=True)
        shutil.copytree(os.path.join(work, "out", name), dst)
    return dst


def sweep_configs(domain, out):
    """Run forgebench/<gen>/auto_generate_json.py; its auto_generated_configs/ becomes `out`."""
    out = os.path.abspath(out)
    with tempfile.TemporaryDirectory(prefix="forgebench_sweep_") as tmp:
        work = os.path.join(tmp, "gen")
        shutil.copytree(paths.generator_dir(domain), work, ignore=shutil.ignore_patterns("__pycache__", "auto_generated_configs"))
        r = subprocess.run([sys.executable, "auto_generate_json.py"], cwd=work, capture_output=True, text=True,
                           env=dict(os.environ, PYTHONPATH=paths.REPO + os.pathsep + os.environ.get("PYTHONPATH", "")))
        if r.returncode:
            sys.exit(f"auto_generate_json.py failed:\n{r.stderr[-2000:]}")
        shutil.rmtree(out, ignore_errors=True)
        shutil.move(os.path.join(work, "auto_generated_configs"), out)
    return len(glob.glob(os.path.join(out, "*.json")))


def _forward(module, argv):
    """Run another module's argparse main() with argv."""
    import importlib
    mod = importlib.import_module(module)
    sys.argv = [module] + list(argv)
    return mod.main()


PAPER = {   # target -> (module, argv)
    "ml-model-suite": [("paper_artifacts.ml_model_suite", [])],
    "section42": [("paper_artifacts.section42", [])],
    "modularization": [("forgebench.modular_designs.parse_reports", ["--out", os.path.join(paths.PAPER, "out", "tables")])],
    "verification": [("verification.experiments.make_tables", [])],
    "catapult-vs-vitis": [("paper_artifacts.catapult_vs_vitis", ["plot", "--out", os.path.join(paths.REPO, "results", "catapult_vs_vitis"),
                                                                 "--fig", os.path.join(paths.PAPER, "out", "figs", "catapult_vs_vitis.png")])],
}


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    if argv and argv[0] in ("ml-sweep", "select-impl", "collect"):
        mod = {"ml-sweep": "forgebench.ml_sweep.run", "select-impl": "forgebench.ml_sweep.select_impl",
               "collect": "forgebench.analysis.collect"}[argv[0]]
        return _forward(mod, argv[1:])
    ap = argparse.ArgumentParser(prog="forgebench", description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    g = sub.add_parser("generate", help="generate an HLS project from a JSON config")
    g.add_argument("configs", nargs="+")
    g.add_argument("-o", "--out", default="forgebench_out")
    g.add_argument("--domain", choices=["gemm", "dnn", "llm", "fullmodel"], help="generator (default: from the path or the ops)")
    g.add_argument("--data-type", help='e.g. "ap_fixed<16,5>", "fixed<32,10,rnd,sat>", "float" (default: the config\'s)')
    g.add_argument("--task", nargs="+", choices=["csim", "csynth", "cosim", "export", "impl"], help="tasks in run_hls.tcl")
    g.add_argument("--tool", choices=["vitis", "catapult"])
    s = sub.add_parser("sweep-configs", help="regenerate the ML sweep configs of one domain")
    s.add_argument("domain", choices=["gemm", "dnn", "llm"])
    s.add_argument("-o", "--out", help="output dir (default: forgebench_out/sweep_configs/<gen>)")
    for name in ("ml-sweep", "select-impl", "collect"):
        sub.add_parser(name, help=f"see `forgebench {name} --help`", add_help=False)
    p = sub.add_parser("paper", help="rebuild paper tables/figures from results/ (outputs in paper_artifacts/out/)")
    p.add_argument("targets", nargs="+", choices=sorted(PAPER) + ["all"])
    a = ap.parse_args(argv)
    if a.cmd == "generate":
        for c in a.configs:
            print(generate(c, a.out, a.domain, a.data_type, a.task, a.tool))
    elif a.cmd == "sweep-configs":
        out = a.out or os.path.join("forgebench_out", "sweep_configs", paths.GEN[a.domain])
        print(f"{sweep_configs(a.domain, out)} configs -> {out}")
    elif a.cmd == "paper":
        for t in (sorted(PAPER) if "all" in a.targets else a.targets):
            for mod, args in PAPER[t]:
                _forward(mod, args)


if __name__ == "__main__":
    main()
