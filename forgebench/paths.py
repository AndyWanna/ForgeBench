"""Repository layout: where generators, configs, generated designs and results live.

    forgebench/<gen>/                         generator (gen_configs, generate_code, templates); run with this dir as cwd
    designs/configs/{ml_sweep,modular_designs,examples,fullmodel}/<gen>/   input JSON configs
    designs/hls/{modular_designs,examples,fullmodel}/<gen>/<design>/       generated or hand-written HLS sources
    results/{ml_sweep/{csynth,impl},modular_designs,fullmodel,verification,catapult_vs_vitis}/

`domain` is the key used in design ids, manifests and metrics (gemm, dnn, llm) and the directory name (`GEN`).
Recorded runs from before the rename (results/verification/*, the lean archives) still say `conv`: RECORDED maps it back.
"""
import os
import shutil

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PKG = os.path.join(REPO, "forgebench")
CONFIGS = os.path.join(REPO, "designs", "configs")
HLS = os.path.join(REPO, "designs", "hls")
RESULTS = os.path.join(REPO, "results")
PAPER = os.path.join(REPO, "paper_artifacts")

DOMAINS = ("gemm", "dnn", "llm")
GEN = {"gemm": "gemm", "dnn": "dnn", "llm": "llm", "fullmodel": "fullmodel"}


RECORDED = {"dnn": "conv"}   # domain key -> the key used inside runs and archives recorded before the conv -> dnn rename


def generator_dir(domain):
    """forgebench/<gen>/ for a domain key (gemm, dnn, llm, fullmodel)."""
    return os.path.join(PKG, GEN[domain])


def sweep_configs(domain):
    """The ML sweep's generated JSON configs (auto_generate_json.py output)."""
    return os.path.join(CONFIGS, "ml_sweep", GEN[domain])


def case_config_dirs(domain):
    """Whole-design configs: the Table 7 programs, then the Table 1 examples."""
    return [os.path.join(CONFIGS, kind, GEN[domain]) for kind in ("modular_designs", "examples")]


def case_configs(domain, dirs=None):
    """Every *.json in `dirs` (default case_config_dirs(domain)), sorted by file name."""
    out = [os.path.join(d, f) for d in (dirs or case_config_dirs(domain)) if os.path.isdir(d)
           for f in os.listdir(d) if f.endswith(".json")]
    return sorted(out, key=os.path.basename)


def case_config(domain, name):
    """Path of one whole-design config by file stem, searching case_config_dirs()."""
    for d in case_config_dirs(domain):
        p = os.path.join(d, name + ".json")
        if os.path.isfile(p):
            return p
    raise FileNotFoundError(f"no config {name}.json for {domain} in {case_config_dirs(domain)}")


def modular_hls(domain):
    """Table 7 design directories (generated programs and hand-written modularized designs)."""
    return os.path.join(HLS, "modular_designs", GEN[domain])


def sweep_metrics(kind, domain):
    """results/ml_sweep/<csynth|impl>/metrics_<domain>.csv"""
    return os.path.join(RESULTS, "ml_sweep", kind, f"metrics_{domain}.csv")


def vitis_hls_root():
    """Vitis HLS install root: $XILINX_HLS (set by Vitis' settings64.sh), else the install of the vitis_hls on PATH; None if neither."""
    if os.environ.get("XILINX_HLS"):
        return os.environ["XILINX_HLS"]
    exe = shutil.which("vitis_hls")
    return os.path.dirname(os.path.dirname(os.path.realpath(exe))) if exe else None


def vitis_hls_include():
    """Vitis HLS headers (ap_fixed.h, hls_math.h): $VITIS_HLS_INCLUDE, else <vitis_hls_root()>/include; '' if not found."""
    root = vitis_hls_root()
    return os.environ.get("VITIS_HLS_INCLUDE") or (os.path.join(root, "include") if root else "")
