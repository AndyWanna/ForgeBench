#!/usr/bin/env python3
"""Render the README's result tables (Markdown) from summary.csv and tool_run_log/results.csv.

With --write, each table replaces the block between `<!-- BEGIN GENERATED: name -->` and
`<!-- END GENERATED: name -->` in README.md, so the report's numbers always come from the records.
"""
import argparse
import csv
import json
import re
import sys
from collections import OrderedDict

from collect_tables import category
from common import EVAL

TOOLS = ["ScaleHLS", "HIDA", "StreamHLS", "AutoSA"]
STAGES = ["", "parse", "transform", "codegen", "synth"]


def read(name):
    with (EVAL / name).open() as stream:
        return list(csv.DictReader(stream))


def reason(row):
    if row["functional_check"] == "PASS":
        return ""
    if row["functional_check"] == "FAIL":
        return f"wrong results (max_error {row['max_error']})"
    if row["functional_check"] == "COMPILE_ERROR":
        return "emitted C++ unusable: `" + row["detail"].replace("<TMP>/cand.cpp", "emitted.cpp").replace("|", "\\|")[:110] + "`"
    if row["functional_check"] == "TIMEOUT":
        return "emitted kernel hangs in C simulation"
    if row["status"] == "timeout":
        return "transformation (DSE) did not finish within the run's time limit"
    cat, why = category(row)
    head = row["error_head"].replace("|", "\\|")
    if len(head) > 110:
        head = head[:107] + "..."
    if head:
        return (f"{cat}: {why}; " if cat else "") + f"`{head}`"
    return f"{cat}: {why}" if cat else row["status"]


def result(row):
    if row["functional_check"] == "PASS":
        if row["stage_reached"] == "synth":
            return "✅ synthesized, correct"
        return "✅ correct; Vitis synthesis did not finish" if row["status"] == "synth_timeout" else "✅ correct (not synthesized)"
    if row["functional_check"] in ("FAIL", "COMPILE_ERROR", "TIMEOUT"):
        return f"❌ {row['stage_reached']} reached, output wrong/unusable"
    return f"❌ stops at {row['stage_reached'] or 'setup'}"


def edits(row, cases):
    spec = cases["inputs"].get(row["input"], {})
    label = "generated " + ("`ap_fixed<16,5>`" if "fixed" in spec.get("data_type", "") else "`float`")
    if spec.get("patch"):
        label += f" + [`{spec['patch'].split('/')[-1]}`]({spec['patch']})"
    if row["tool_options"]:
        label += f" + `{row['tool_options']}`"
    return label


def order(row):
    # Best attempt: correct synthesis > correct > (unedited if correct, edited if failing) > furthest stage.
    correct = row["functional_check"] == "PASS"
    return (correct and row["stage_reached"] == "synth", correct,
            (not row["patch"]) if correct else bool(row["patch"]),
            STAGES.index(row["stage_reached"]) if row["stage_reached"] in STAGES else 0)


def whole(rows, cases):
    lines = ["| Design | Input | Tool | Stage reached | Result | Why it failed |", "|---|---|---|---|---|---|"]
    for r in rows:
        if r["level"] == "design" and r["tool"] in TOOLS:
            lines.append(f"| `{r['input'].split('__')[0]}` | {edits(r, cases)} | {r['tool']} | {r['stage_reached'] or '—'} | "
                         f"{result(r)} | {reason(r)} |")
    return lines


def operators(rows, cases):
    lines = ["| Domain | Operator | Tool | Best result | Input of that attempt | Why it failed |", "|---|---|---|---|---|---|"]
    groups = OrderedDict()
    for r in rows:
        if r["level"] == "operator" and r["tool"] in TOOLS:
            groups.setdefault((r["domain"], r["input"].split("__")[0], r["tool"]), []).append(r)
    for (domain, op, tool), attempts in groups.items():
        best = max(attempts, key=order)
        lines.append(f"| {domain} | `{op}` | {tool} | {result(best)} | {edits(best, cases)} | {reason(best)} |")
    return lines


def summary(rows, cases):
    cells = {(r["tool"], r["domain"]): r for r in read("summary.csv")}
    symbol = {"full": "● full", "half": "◐ half", "none": "○ none"}
    lines = ["| Tool | GEMM | DNN | LLM |", "|---|---|---|---|"]
    for tool in ["ScaleHLS", "HIDA", "AutoSA", "StreamHLS"]:
        lines.append(f"| {tool} | " + " | ".join(f"{symbol[cells[(tool, d)]['rating']]} ({cells[(tool, d)]['stage_reached']})"
                                                for d in ["gemm", "dnn", "llm"]) + " |")
    return lines


def qor(rows, cases):
    baseline = {r["input"].split("__")[0]: r for r in rows if r["tool"] == "Vitis" and r["data_type"] == "float"}
    lines = ["| Input | Tool | Latency (cycles) | Speed-up vs. Vitis | Est. clock (ns) | BRAM | DSP | FF | LUT |",
             "|---|---|---|---|---|---|---|---|---|"]
    for r in rows:
        if r["tool"] in TOOLS and r["stage_reached"] == "synth" and r["functional_check"] == "PASS":
            base = baseline.get(r["input"].split("__")[0])
            ratio = f"{int(base['latency_cycles']) / int(r['latency_cycles']):.2f}×" if base and r["latency_cycles"] else ""
            tool = r["tool"] + (f" (`{r['tool_options']}`)" if r["tool_options"] else "")
            lines.append(f"| `{r['input']}` | {tool} | {r['latency_cycles']} | {ratio} | {r['est_clock_ns']} | "
                         f"{r['BRAM_18K']} | {r['DSP']} | {r['FF']} | {r['LUT']} |")
    lines += ["", "Vitis on the unmodified generated designs:", "",
              "| Design | Type | Latency (cycles) | Est. clock (ns) | BRAM | DSP | FF | LUT |", "|---|---|---|---|---|---|---|---|"]
    for r in rows:
        if r["tool"] == "Vitis":
            lines.append(f"| `{r['input'].split('__')[0]}` | `{r['data_type']}` | {r['latency_cycles']} | {r['est_clock_ns']} | "
                         f"{r['BRAM_18K']} | {r['DSP']} | {r['FF']} | {r['LUT']} |")
    return lines


def short(row):
    """One-phrase reason for a failed attempt (overview table)."""
    if row["functional_check"] == "FAIL":
        return "wrong results"
    if row["functional_check"] == "COMPILE_ERROR":
        return "emitted code unusable"
    if row["functional_check"] == "TIMEOUT":
        return "kernel hangs"
    if row["status"] == "timeout":
        return "DSE timeout"
    cat, why = category(row)
    return f"{cat} ({why})" if cat else f"crash in {row['stage_reached'] or 'setup'}"


def overview(rows, cases):
    lines = ["| Tool | GEMM whole design | GEMM single op | DNN whole design | DNN single ops | LLM whole design | LLM single ops |",
             "|---|---|---|---|---|---|---|"]
    for tool in TOOLS:
        cells = []
        for domain in ["gemm", "dnn", "llm"]:
            designs = [r for r in rows if r["tool"] == tool and r["domain"] == domain and r["level"] == "design"]
            best = max(designs, key=order)
            if best["functional_check"] == "PASS":
                cells.append(f"✅ `{best['input'].split('__')[0]}` with edits")
            else:
                cells.append(f"❌ {short(best)}")
            ops = OrderedDict()
            for r in rows:
                if r["tool"] == tool and r["domain"] == domain and r["level"] == "operator":
                    ops.setdefault(r["input"].split("__")[0], []).append(r)
            good = [op for op, attempts in ops.items() if any(a["functional_check"] == "PASS" for a in attempts)]
            bad = {op: short(max(attempts, key=order)) for op, attempts in ops.items() if op not in good}
            text = (f"✅ {len(good)}/{len(ops)}: " + ", ".join(good)) if good else f"❌ 0/{len(ops)}"
            if bad:
                text += "; ❌ " + ", ".join(f"{op} ({why})" for op, why in bad.items())
            cells.append(text)
        lines.append(f"| {tool} | " + " | ".join(cells) + " |")
    return lines


def features(rows, cases):
    lines = ["| Tool | Mark | Paper | Evidence | Agrees (tick = supports) | Example | Diagnostic |", "|---|---|---|---|---|---|---|"]
    for r in read("feature_evidence.csv"):
        mark = {"tick": "✓", "cross": "✗"}[r["paper_mark"]]
        diag = r["diagnostic"].replace("|", "\\|")
        diag = (diag[:90] + "...") if len(diag) > 90 else diag
        lines.append(f"| {r['tool']} | {r['feature']} ({r['meaning']}) | {mark} | {r['evidence_status'].replace('_', ' ')} | "
                     f"{r['agrees_with_paper'] or '—'} | {('`' + r['example_design'] + '`') if r['example_design'] else ''} | "
                     f"{('`' + diag + '`') if diag else ''} |")
    return lines


def tables():
    rows = read("tool_run_log/results.csv")
    cases = json.loads((EVAL / "tool_run_log/cases.json").read_text())
    return {name: "\n".join(fn(rows, cases)) + "\n"
            for name, fn in [("overview", overview), ("whole", whole), ("operators", operators), ("summary", summary),
                             ("features", features), ("qor", qor)]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--write", action="store_true", help="update the generated blocks in README.md")
    args = parser.parse_args()
    out = tables()
    if not args.write:
        for name, text in out.items():
            print(f"<!-- {name} -->\n{text}")
        return
    readme = EVAL / "README.md"
    text = readme.read_text()
    for name, table in out.items():
        pattern = re.compile(rf"(<!-- BEGIN GENERATED: {name} -->\n).*?(<!-- END GENERATED: {name} -->)", re.S)
        if not pattern.search(text):
            sys.exit(f"README.md has no generated block {name!r}")
        text = pattern.sub(lambda m: m.group(1) + table + m.group(2), text)
    readme.write_text(text)
    print("Updated generated tables in README.md: " + ", ".join(out))


if __name__ == "__main__":
    main()
