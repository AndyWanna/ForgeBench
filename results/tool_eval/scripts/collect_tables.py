#!/usr/bin/env python3
"""Build summary.csv, outcomes.csv and minimal/summary_operators.csv from the run records.

Sources: minimal/results.csv (written by minimal_run.py; every whole-design and single-operator
run) and minimal/cases.json. Ratings follow the TODO's definitions: `full` = synthesis with
correct results and no source edit; `half` = synthesis with correct results after the
committed non-algorithmic patch; `none` otherwise. Output that synthesizes but computes the
wrong result is rated `none`. Nothing in these tables is entered by hand.
"""
import csv
import json
import re
import shutil
from collections import defaultdict

from common import EVAL

TOOLS = ["ScaleHLS", "HIDA", "AutoSA", "StreamHLS"]
DOMAINS = ["gemm", "dnn", "llm"]
STAGES = ["", "parse", "transform", "codegen", "synth"]
RULES = [  # (pattern in the diagnostic, category, explanation); first match wins
    (r"ap_fixed_base\.h|ap_private\.h|NamedDecl::getName|ap_int_base", "F1", "vendor ap_fixed / C++ header implementation"),
    (r"'math\.exp' op operand #0 must be floating-point-like", "F5", "hls::exp applied to ap_fixed"),
    (r"must be a (?:valid )?dimension or symbol identifier", "F2", "run-time trip count / non-affine index"),
    (r"Non-uniform dependence|Single outermost permutable band", "F2", "loop nest outside the single uniform band AutoSA requires"),
    (r"Attributes\.cpp:28", "F3", "written global buffer treated as a constant (ConvertGetGlobalToConstBuffer)"),
]


def rows():
    with (EVAL / "minimal/results.csv").open() as stream:
        return list(csv.DictReader(stream))


def category(row):
    text = row["error_head"] + " " + row["detail"]
    return next(((c, why) for pattern, c, why in RULES if re.search(pattern, text)), ("", ""))


def score(row):
    """Order attempts: correct synthesis > correct codegen > wrong output > furthest failing stage."""
    correct = row["functional_check"] == "PASS"
    # Among correct attempts an unedited one ranks first (it is the stronger claim); among failures the
    # edited one does (its error is the one left to fix).
    return (row["stage_reached"] == "synth" and correct, correct, (not row["patch"]) if correct else bool(row["patch"]),
            STAGES.index(row["stage_reached"]) if row["stage_reached"] in STAGES else 0)


def describe(row):
    if row["functional_check"] == "PASS":
        return f"{row['stage_reached']}, correct"
    if row["functional_check"] in ("FAIL", "COMPILE_ERROR", "TIMEOUT"):
        what = {"FAIL": f"wrong results (max_error {row['max_error']})", "COMPILE_ERROR": "emitted code does not compile",
                "TIMEOUT": "simulation hangs"}[row["functional_check"]]
        return f"{row['stage_reached']} reached, {what}"
    if row["status"] == "timeout":
        return "transformation did not finish within the time limit"
    if row["status"] == "synth_timeout":
        return "correct, but Vitis synthesis did not finish within the time limit"
    cat, _ = category(row)
    return f"stops at {row['stage_reached'] or row['status']}" + (f" ({cat})" if cat else "")


def main():
    data = [r for r in rows() if r["tool"] in TOOLS]
    cases = json.loads((EVAL / "minimal/cases.json").read_text())
    # ---- summary.csv: whole generated designs ----------------------------------------------------
    summary = []
    for tool in TOOLS:
        for domain in DOMAINS:
            attempts = [r for r in data if r["tool"] == tool and r["domain"] == domain and r["level"] == "design"]
            best = max(attempts, key=score)
            correct = best["functional_check"] == "PASS" and best["stage_reached"] == "synth"
            rating = ("full" if not best["patch"] else "half") if correct else "none"
            designs = sorted({r["input"].split("__")[0] for r in attempts})
            notes = "; ".join(f"{r['input'].split('__')[1]} {r['input'].split('__')[0]}"
                              + (f" [{r['tool_options']}]" if r["tool_options"] else "") + f": {describe(r)}"
                              for r in sorted(attempts, key=lambda r: r["run"]))
            summary.append({"tool": tool, "domain": domain, "designs_tried": len(designs), "stage_reached": best["stage_reached"],
                            "rating": rating, "notes": f"Whole generated designs {', '.join(designs)}. {notes}. "
                                                       "Edited inputs use minimal/patches/<design>__edited.patch; logs in minimal/logs/."})
            # One log per cell in logs/<tool>/<domain>/ for the edited whole design too.
            for r in attempts:
                if r["input"].endswith("__edited"):
                    src = EVAL / "minimal/logs" / r["run"]
                    suffix = r["run"].split("/")[-1].replace(tool.lower(), "").strip("-")
                    for name in ["run.log", "result.json", "check.json"]:
                        if (src / name).is_file():
                            dest = EVAL / "logs" / tool.lower() / domain / f"{r['input'].split('__')[0]}-edited{('-' + suffix) if suffix else ''}-{name}"
                            dest.parent.mkdir(parents=True, exist_ok=True)
                            shutil.copyfile(src / name, dest)
    with (EVAL / "summary.csv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=["tool", "domain", "designs_tried", "stage_reached", "rating", "notes"], lineterminator="\n")
        writer.writeheader(); writer.writerows(summary)
    # ---- patches/<tool>/: the edits behind every correct, synthesized result (TODO item 1) ---------
    for r in data:
        if r["functional_check"] == "PASS" and r["stage_reached"] == "synth" and r["patch"]:
            dest = EVAL / "patches" / r["tool"].lower() / (r["patch"].split("/")[-1])
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(EVAL / r["patch"], dest)
    # ---- outcomes.csv: every design x tool attempt ----------------------------------------------
    outcomes = []
    for r in data:
        cat, _ = category(r)
        head = r["error_head"]
        if r["functional_check"] == "FAIL":
            head = f"wrong results: max_error {r['max_error']} against the generated design"
        elif r["functional_check"] in ("COMPILE_ERROR", "TIMEOUT"):
            head = f"emitted code {'does not compile' if r['functional_check'] == 'COMPILE_ERROR' else 'hangs in C simulation'}: {r['detail']}"
        elif r["functional_check"] == "PASS":
            head = ""
        outcomes.append({"design_id": r["input"], "domain": r["domain"], "tool": r["tool"],
                         "stage_reached": r["stage_reached"], "failure_category": cat, "error_head": head,
                         "run": r["run"], "patch": r["patch"], "functional_check": r["functional_check"]})
    with (EVAL / "outcomes.csv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=["design_id", "domain", "tool", "stage_reached", "failure_category", "error_head",
                                                    "run", "patch", "functional_check"], lineterminator="\n")
        writer.writeheader(); writer.writerows(outcomes)
    # ---- per-operator summary -------------------------------------------------------------------
    groups = defaultdict(list)
    for r in data:
        if r["level"] == "operator":
            groups[(r["domain"], r["input"].split("__")[0], r["tool"])].append(r)
    table = []
    for (domain, op, tool), attempts in sorted(groups.items()):
        best = max(attempts, key=score)
        failing = [r for r in attempts if r is not best]
        table.append({"domain": domain, "operator": op, "tool": tool, "best_run": best["run"], "result": describe(best),
                      "patch": best["patch"], "tool_options": best["tool_options"],
                      "rating": ("half" if best["patch"] else "full") if best["functional_check"] == "PASS" and best["stage_reached"] == "synth" else "none",
                      "attempts": len(attempts)})
    with (EVAL / "minimal/summary_operators.csv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(table[0]), lineterminator="\n")
        writer.writeheader(); writer.writerows(table)
    print(f"summary.csv: {len(summary)} rows; outcomes.csv: {len(outcomes)} rows; summary_operators.csv: {len(table)} rows")


if __name__ == "__main__":
    main()
