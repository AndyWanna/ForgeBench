#!/usr/bin/env python3
"""Build the non-sweep design registries: manifest/designs/{ops,modular,modular_cases}.csv.

Scope rule: only designs the paper references are registered.
  ops      Table 2 (per-operator verification: 17 operators)         -> ops.csv
  modular  Sec. 4.4 / Table 7 (modularization test cases)            -> modular.csv, modular_cases.csv
  (sweeps: Table 4/Figs 6-8/Table 5 -> build_manifest.py; full models: Table 3 -> fullmodel.csv, once the
   configs are recovered; tool evaluation: the tool-support table, Case Study 3 -> results/tool_eval/)
Whole-design configs in designs/configs/examples/<gen>/ that the paper does not reference (e.g. ResNet/VGG blocks,
attention_op_p*, testing_*) and the two unused modular designs (gemm/mlp, gemm/diff_dims_module_large) are not
registered; they remain in the repo and in git history.

design_id = "<suite>/<domain>/<name>" (e.g. ops/gemm/dot_product__bias1, modular/gemm/diff_dims_p1).

    python manifest/build_registry.py

Sources of truth (nothing is hand-typed here except the Table 2 operator labels):
  ops      verification/operators/<operator>/variants/<domain>/*.json (via verification.layout), the generator dispatch in
           <domain>/generate_code.py and the golden dispatch in verification/domains/<domain>.py
  modular  designs/hls/modular_designs/<gen>/*  +  forgebench/modular_designs/parse_reports.py:ROWS
           (ROWS defines each test case: its programs, its modularized design and its shared functions)
"""
import ast
import csv
import hashlib
import importlib.util
import json
import os
import glob
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, REPO)
from forgebench import paths  # noqa: E402
OUT = os.path.join(REPO, "manifest", "designs")
DOMAINS = ["gemm", "dnn", "llm"]
CATEGORY = {"GEMM": "gemm", "DNN": "dnn", "LLM": "llm"}


# Operator -> row label in the paper's Table 2 (17 rows).
TABLE2_ROW = {
    "gemm": "GEMM", "vmm": "Vec-Mtx Mult.", "mmv": "Mtx-Vec Mult.", "dot_product": "Dot Product",
    "conv": "Convolution", "batchnorm": "BatchNorm", "mha": "Multi-head Attention", "swa": "Sliding-window Attn.",
    "matmul": "MatMul", "layernorm": "LayerNorm", "rmsnorm": "RMSNorm", "activation": "Activation",
    "matrix_add": "Matrix add", "elementwise_mult": "Element-wise Mult.", "dropout": "Dropout",
    "maxpool": "Max pool", "adaptive_avgpool": "Avg pool",
}
# Which domain's variants count toward Table 2 (the paper counts each operator once; matrix_add and the activations exist in
# several domains). Activations are counted for the rank-2 template (gemm domain) and the rank-3 template (conv domain).
TABLE2_DOMAINS = {
    "gemm": {"gemm", "vmm", "mmv", "dot_product", "activation"},
    "dnn": {"conv", "batchnorm", "maxpool", "adaptive_avgpool", "matrix_add", "activation"},
    "llm": {"mha", "swa", "matmul", "layernorm", "rmsnorm", "dropout", "elementwise_mult"},
}
REUSE = {"dagger": "tiling", "ddagger": "functional", "ast": "arithmetic"}      # Table 7 footnote marks


def sha256_file(path):
    with open(path, "rb") as f:
        return hashlib.sha256(f.read()).hexdigest()


def rel(path):
    return os.path.relpath(path, REPO).replace(os.sep, "/")


def compact(obj):
    return json.dumps(obj, separators=(",", ":"), sort_keys=False)


def write_csv(name, cols, rows):
    os.makedirs(OUT, exist_ok=True)
    path = os.path.join(OUT, name)
    with open(path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=cols, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
    print(f"{name}: {len(rows)} rows")


def generator_dispatch(domain):
    """func_name -> generator function name, read from forgebench/<gen>/generate_code.py:generate_func_def."""
    src = open(os.path.join(paths.generator_dir(domain), "generate_code.py")).read()
    return dict(re.findall(r"op_info\['func_name'\] == '(\w+)':\s*\n\s*code_line, full_func_name = (\w+)\(", src))


def golden_dispatch(domain):
    """func_name -> golden function name, read from the _DISPATCH dict in verification/domains/<domain>.py."""
    tree = ast.parse(open(os.path.join(REPO, "verification", "domains", f"{domain}.py")).read())
    for node in tree.body:
        if isinstance(node, ast.Assign) and any(getattr(t, "id", "") == "_DISPATCH" for t in node.targets):
            return {k.value: v.id for k, v in zip(node.value.keys, node.value.values)}
    return {}


def template_of(op):
    fi = op.get("func_info") or []
    return fi[0] if fi and isinstance(fi[0], str) and fi[0].endswith(".cpp") else ""


def under_test(cfg):
    """The op(s) other than load/store (operator variants contain exactly one)."""
    return [(n, o) for n, o in cfg["ops"].items() if o["func_name"] not in ("load", "store")]


# ------------------------------------------------------------------ ops
def build_ops():
    sys.path.insert(0, REPO)
    from verification import layout
    rows = []
    for domain in DOMAINS:
        gen, gold = generator_dispatch(domain), golden_dispatch(domain)
        for v in layout.variants(domain):
            stem, path = v["stem"], v["path"]
            cfg = json.load(open(path))
            ops = under_test(cfg)
            assert len(ops) == 1, (stem, [o[1]["func_name"] for o in ops])
            op = ops[0][1]
            fn = op["func_name"]
            rows.append({
                "design_id": f"ops/{domain}/{stem}", "domain": domain,
                "operator": v["operator"], "paper_table2_row": TABLE2_ROW[v["operator"]],
                "in_table2": "YES" if v["operator"] in TABLE2_DOMAINS[domain] else "NO", "variant": "" if v["variant"] == "default" else v["variant"],
                "op_func": fn, "op_dims": compact(op.get("dims", [])), "op_func_info": compact(op.get("func_info", [])),
                "template": template_of(op),
                "generator_function": f"{rel(paths.generator_dir(domain))}/generate_code.py:{gen.get(fn, '')}",
                "golden_function": f"verification/domains/{domain}.py:{gold.get(fn, '')}",
                "input_range": compact(cfg.get("input_range", "")), "config_path": rel(path),
            })
    cols = ["design_id", "domain", "operator", "paper_table2_row", "in_table2", "variant", "op_func", "op_dims", "op_func_info", "template",
            "generator_function", "golden_function", "input_range", "config_path"]
    return cols, rows


# ------------------------------------------------------------------ modular
def load_modular_rows():
    spec = importlib.util.spec_from_file_location("_modular_parse", os.path.join(paths.PKG, "modular_designs", "parse_reports.py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod.ROWS


def slug(text):
    t = re.sub(r"\$\\\w+\$", "", text)               # latex footnote marks
    return re.sub(r"[^a-z0-9]+", "_", t.lower()).strip("_")


MODULAR_ARCHIVE = "reports/csynth_modular_lean.tar.gz"
MODULAR_ARCHIVE_LOCAL = os.path.join("checkpoints", "r2", "csynth_modular_lean.tar.gz")


def modular_reports():
    """{design_id: csynth.xml member path} from the modular lean archive (empty if not packaged yet)."""
    import tarfile
    path = os.path.join(os.environ.get("FORGEBENCH_ARCHIVES", REPO), MODULAR_ARCHIVE_LOCAL)
    out = {}
    if os.path.isfile(path):
        with tarfile.open(path, "r:gz") as tf:
            for m in tf.getnames():
                if m.endswith("project_1/solution1/syn/report/csynth.xml"):
                    parts = m.split("/")                      # modular/<domain>/<name>/project_1/...
                    dom = {v: k for k, v in paths.RECORDED.items()}.get(parts[1], parts[1])   # archive says conv
                    out[f"modular/{dom}/{parts[2]}"] = m
    return out


def modular_synthesized():
    """design ids with a Table 7 csynth result (results/modular_designs/csynth_utilization.csv, the <32,10> synthesis)."""
    with open(os.path.join(REPO, "results", "modular_designs", "csynth_utilization.csv"), newline="") as f:
        return {f"modular/{r['category']}/{r['project']}" for r in csv.DictReader(f) if not r["function"]}


def build_modular():
    reports, synthesized = modular_reports(), modular_synthesized()
    cases = []
    in_cases = {}                                     # (category, design dir) -> [case_id]
    role = {}
    for r in load_modular_rows():
        cat = r["category"]
        case_id = f"modular_case/{cat}/{slug(r['case'])}"
        progs = [p for p in r["programs"] if p]
        for p in progs:
            in_cases.setdefault((cat, p), []).append(case_id)
            role[(cat, p)] = "program"
        in_cases.setdefault((cat, r["module"]), []).append(case_id)
        role[(cat, r["module"])] = "modularized_design"
        cases.append({
            "case_id": case_id, "suite_name": r["suite"], "domain": cat,
            "case_name": re.sub(r"\s+", " ", re.sub(r"\$\\\w+\$", "", r["case"])).strip(),
            "reuse_types": ";".join(REUSE[m] for m in re.findall(r"\\(\w+)\$", r["case"]) if m in REUSE),
            "programs": ";".join(f"modular/{cat}/{p}" for p in progs),
            "modularized_design": f"modular/{cat}/{r['module']}", "shared_functions": ";".join(r["shared"]),
        })
    designs = []
    for cat in DOMAINS:
        base = paths.modular_hls(cat)
        cfg_dir = os.path.join(paths.CONFIGS, "modular_designs", paths.GEN[cat])
        for name in sorted(os.listdir(base)):
            ddir = os.path.join(base, name)
            if not os.path.isdir(ddir):
                continue
            files = set(os.listdir(ddir))
            cfg = os.path.join(cfg_dir, name + ".json")
            has_cfg = os.path.isfile(cfg)
            r_ = role.get((cat, name))
            if r_ is None:
                continue                                  # in no Table 7 test case: not registered
            if has_cfg:
                construction = "generated"
            elif r_ == "modularized_design":
                construction = "manual"
            else:
                construction = "generated (config not in repo)"
            did = f"modular/{cat}/{name}"
            ok = did in synthesized
            designs.append({
                "design_id": did, "domain": cat, "role": r_,
                "test_cases": ";".join(in_cases.get((cat, name), [])),
                "construction": construction,
                "config_path": rel(cfg) if has_cfg else "", "design_path": rel(ddir),
                "has_testbench": "tb_top.cpp" in files,
                "has_dram_inputs": any(f.startswith(("DRAM_", "BRAM_")) and f.endswith(".txt") for f in files),
                "generated": "YES",
                "csynth": "YES" if ok else "NO",
                "csynth_report": f"reports/modular/{cat}/{name}/csynth.xml" if ok and did in reports else "",
                "fail_reason": "",
            })
    dcols = ["design_id", "domain", "role", "test_cases", "construction", "config_path", "design_path", "has_testbench",
             "has_dram_inputs", "generated", "csynth", "csynth_report", "fail_reason"]
    ccols = ["case_id", "suite_name", "domain", "case_name", "reuse_types", "programs", "modularized_design", "shared_functions"]
    return dcols, designs, ccols, cases



# ------------------------------------------------------------------ full models (Table 3)
FULLMODEL_RUNS = {"full": "resnet_full_32_10", "tiled": "resnet_tiled_16_5_trn"}   # results/verification/fullmodel/<run>/


def build_fullmodel():
    """One row per Table 3 project: sources, config, csynth metrics and the C-sim runs behind its row. Also the registry input of
    forgebench.modular_designs.synth (design_id, domain, name, hls_dir) for re-synthesizing the full models."""
    with open(os.path.join(REPO, "results", "fullmodel", "metrics_csynth.csv"), newline="") as f:
        synth = {r["design"]: r for r in csv.DictReader(f)}
    rows = []
    for d in sorted(glob.glob(os.path.join(paths.HLS, "fullmodel", "*")), key=lambda p: (not os.path.basename(p).startswith("RESNET"),
                                                                                          "_TILED" in p, len(p), p)):
        name = os.path.basename(d)
        m = re.match(r"RESNET(\d+)", name)
        if m:
            model, variant = f"ResNet-{m[1]}", "tiled" if "_TILED" in name else "full"
            runs = os.path.join("results", "verification", "fullmodel", FULLMODEL_RUNS[variant])
            seeds = len(glob.glob(os.path.join(REPO, runs, name + "seed*")))
        else:
            ctx = re.search(r"ctx(\d+)", name)[1]
            model, variant = f"Llama-3-8B (ctx {ctx})", "prefill" if "PREFILL" in name else "decode"
            runs = os.path.join("results", "verification", "fullmodel", f"llama_ctx{ctx}_32_10")
            seeds = len(glob.glob(os.path.join(REPO, runs, f"LLAMA3_8B_ctx{ctx}_32_10_seed*")))
        fmt = "fixed<16,5> (TRN/WRAP)" if "_16_5_" in name else "fixed<32,10,rnd,sat>"
        rows.append({"design_id": f"fullmodel/{name}", "domain": "fullmodel", "name": name, "model": model, "variant": variant,
                     "format": fmt, "config_path": rel(os.path.join(paths.CONFIGS, "fullmodel", name + ".json")), "hls_dir": rel(d),
                     "csynth": "YES" if name in synth else "NO", "csim_runs": runs, "csim_seeds": seeds})
    cols = ["design_id", "domain", "name", "model", "variant", "format", "config_path", "hls_dir", "csynth", "csim_runs", "csim_seeds"]
    return cols, rows


def main():
    cols, rows = build_ops()
    n57 = sum(1 for r in rows if r["in_table2"] == "YES")
    assert n57 == 57, f"Table 2 selection has {n57} variants, expected 57"
    write_csv("ops.csv", cols, rows)
    dcols, designs, ccols, cases = build_modular()
    write_csv("modular.csv", dcols, designs)
    write_csv("modular_cases.csv", ccols, cases)
    write_csv("fullmodel.csv", *build_fullmodel())
    # consistency checks
    ids = [r["design_id"] for f in ("ops", "modular", "fullmodel") for r in csv.DictReader(open(os.path.join(OUT, f + ".csv")))]
    assert len(ids) == len(set(ids)), "duplicate design_id across registries"
    print(f"total registered non-sweep designs: {len(ids)}")


if __name__ == "__main__":
    main()
