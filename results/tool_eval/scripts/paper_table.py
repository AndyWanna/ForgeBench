#!/usr/bin/env python3
"""Derive every cell of the paper's tool-support table from the run records; write table/.

Inputs: tool_run_log/results.csv (every tool run), tool_run_log/synth_unchecked.csv (Vitis on the
outputs that failed the functional check), tool_run_log/cases.json (what each input is), and
table/constructs.json (the runs behind each F1-F6 mark, checked here against the records).

Rules, as in the caption:
* Design: the base design of each domain (gemm_0001, dnn_0001, llm_0001), over every input the
  tool was given for it. Full when a run synthesizes and passes the functional check, half
  when a run's output synthesizes, empty otherwise.
* Ops: the single-operator designs the tool was given. An operator counts under Synth. when
  the output of at least one of its inputs synthesizes, and under FV when one of them also
  passes the functional check. Both counts are bold when every evaluated operator passes FV.

Outputs: table/tool_support.tex (the table), table/cells.csv (each cell and the runs behind
it), table/runs.csv (each of those runs: input, emitted code, logs, verdicts). --check
compares them with the committed files instead of writing.
"""
import argparse
import csv
import io
import json
import sys

from common import EVAL

TOOLS = [("ScaleHLS", "yeScaleHLSScalableHighlevel2022a"), ("HIDA", "yeHIDAHierarchicalDataflow2024a"),
         ("AutoSA", "wangAutoSAPolyhedralCompiler2021a"), ("StreamHLS", "basalamaStreamHLSAutomaticDataflow2025a")]
DOMAINS = {"gemm": ("gemm_0001", ["gemm_ijk", "gemm_ikj", "gemm_jik", "gemm_jki", "gemm_kij", "gemm_kji"]),
           "dnn": ("dnn_0001", ["conv_k1", "conv_k3p0", "conv_k3"]),
           "llm": ("llm_0001", ["rmsnorm", "softmax", "mha", "mha_rope"])}
STAGES = ["", "parse", "transform", "codegen", "synth"]
CIRCLE = {"full": r"\fullcirc{}", "half": r"\halfcirc{}", "none": r"\emptycirc{}"}
FOOTNOTES = {("AutoSA", "gemm"): "a"}
FOOTNOTE_TEXT = {"a": "AutoSA was run on two of the six GEMM loop orders (reduction loop innermost and not "
                      "innermost); it reschedules the loop nest itself and emitted identical kernels for both."}
TEMPLATE = r"""\begin{table*}[t]
\centering
\caption{\new{m4}{1}{Support of source-to-source HLS tools for \thiswork{} designs, using the \texttt{float} option with minor edits. \emph{Design}: base design of the ML test suite ($\S$.~\ref{sec:ml_testsuite}); \fullcirc{} passes functional verification, \halfcirc{} synthesizes but fails functional verification, \emptycirc{} fails before synthesis. \emph{Ops}: single-operator designs that synthesize (\emph{Synth.}) and that pass functional verification (\emph{FV}), out of those evaluated; bold when all pass. F1--F6: whether the tool handles (\cmark) or fails on (\xmark) each construct.}}
\label{tab:tool-support}
\resizebox{\textwidth}{!}{%
\todofloat{
\begin{tabular}{l ccc ccc ccc | cccccc}
\toprule
\multirow{3}{*}{\textbf{Tool}} & \multicolumn{3}{c}{\textbf{GEMM}} & \multicolumn{3}{c}{\textbf{DNN}} & \multicolumn{3}{c}{\textbf{LLM}} & \multicolumn{6}{c}{\textbf{Constructs}} \\
\cmidrule(lr){2-4}\cmidrule(lr){5-7}\cmidrule(lr){8-10}\cmidrule(lr){11-16}
& \multirow{2}{*}{Design} & \multicolumn{2}{c}{Ops} & \multirow{2}{*}{Design} & \multicolumn{2}{c}{Ops} & \multirow{2}{*}{Design} & \multicolumn{2}{c}{Ops} & \multirow{2}{*}{F1} & \multirow{2}{*}{F2} & \multirow{2}{*}{F3} & \multirow{2}{*}{F4} & \multirow{2}{*}{F5} & \multirow{2}{*}{F6} \\
\cmidrule(lr){3-4}\cmidrule(lr){6-7}\cmidrule(lr){9-10}
& & Synth. & FV & & Synth. & FV & & Synth. & FV & & & & & & \\
\midrule
%ROWS%\bottomrule
\end{tabular}
}
}
\vspace{2pt}
{\centering\footnotesize
F1: vendor fixed-point types; F2: file-scope buffers; F3: top-level interface; \\ F4: arbitrary loop order; F5: conditional accesses; F6: scalar reductions. \\
%FOOTNOTES%
}
\end{table*}
"""


def load():
    with (EVAL / "tool_run_log/results.csv").open() as stream:
        runs = {row["run"]: row for row in csv.DictReader(stream) if row["tool"] != "Vitis" and row["level"] != "control"}
    with (EVAL / "tool_run_log/synth_unchecked.csv").open() as stream:
        extra = {row["run"]: row for row in csv.DictReader(stream)}
    with (EVAL / "generated_hls/index.csv").open() as stream:
        emitted = {row["run"]: row["path"] for row in csv.DictReader(stream)}
    for run, row in runs.items():
        if row["status"] == "synth_passed":
            row["synth"], row["synth_source"] = "passed", "tool_run_log/results.csv"
        elif run in extra:
            row["synth"], row["synth_source"] = extra[run]["synth"], "tool_run_log/synth_unchecked.csv"
            row["latency_cycles"] = row["latency_cycles"] or extra[run]["latency_cycles"]
        else:
            row["synth"], row["synth_source"] = "", ""   # no code was emitted
        row["fv"] = row["functional_check"] == "PASS" and row["status"] == "synth_passed"
        row["emitted"] = emitted.get(run, "")
    return runs


def rank(row):
    """How far a run got: correct, synthesized, emitted code, then by stage."""
    return (row["fv"], row["synth"] == "passed", bool(row["emitted"]), STAGES.index(row["stage_reached"]))


def design_cell(runs, tool, base):
    tried = [row for row in runs.values() if row["tool"] == tool and row["input"].startswith(base + "__")]
    best = max(tried, key=rank)
    value = "full" if best["fv"] else "half" if best["synth"] == "passed" else "none"
    return value, [best["run"]]


def ops_cells(runs, tool, operators):
    tried = [op for op in operators if any(r["tool"] == tool and r["input"].split("__")[0] == op for r in runs.values())]
    best = {op: max((r for r in runs.values() if r["tool"] == tool and r["input"].split("__")[0] == op), key=rank)
            for op in tried}
    synth = [op for op in tried if best[op]["synth"] == "passed"]
    fv = [op for op in tried if best[op]["fv"]]
    return (len(synth), len(fv), len(tried)), [best[op]["run"] for op in tried]


def check_constructs(runs):
    constructs = json.loads((EVAL / "table/constructs.json").read_text())
    marks = {}
    for entry in constructs["marks"]:
        for run, status, check, error in entry["with"] + entry["without"]:
            row = runs[run]
            assert row["status"] == status and row["functional_check"] == check, (run, row["status"], row["functional_check"])
            assert error in row["error_head"] + row["detail"], (run, error)
        marks[entry["tool"], entry["feature"]] = entry
    assert len(marks) == 24
    return marks


def fraction(count, total, bold, note=""):
    text = f"{count}/{total}"
    return (rf"\textbf{{{text}}}" if bold else text) + (rf"\textsuperscript{{{note}}}" if note else "")


def build():
    runs = load()
    marks = check_constructs(runs)
    cells, rows, used = [], [], set()
    for tool, cite in TOOLS:
        tex = [f"{tool}~\\cite{{{cite}}}"]
        for domain, (base, operators) in DOMAINS.items():
            value, evidence = design_cell(runs, tool, base)
            cells.append({"tool": tool, "domain": domain, "column": "Design", "value": value, "runs": ";".join(evidence)})
            (synth, fv, total), op_runs = ops_cells(runs, tool, operators)
            note = FOOTNOTES.get((tool, domain), "")
            cells.append({"tool": tool, "domain": domain, "column": "Ops Synth.", "value": f"{synth}/{total}", "runs": ";".join(op_runs)})
            cells.append({"tool": tool, "domain": domain, "column": "Ops FV", "value": f"{fv}/{total}", "runs": ";".join(op_runs)})
            # "Bold when all pass": both counts are bold when every evaluated operator passes FV.
            tex += [CIRCLE[value], fraction(synth, total, fv == total, note), fraction(fv, total, fv == total, note)]
            used.update(evidence + op_runs)
        for feature in ["F1", "F2", "F3", "F4", "F5", "F6"]:
            entry = marks[tool, feature]
            evidence = [item[0] for item in entry["with"] + entry["without"]]
            cells.append({"tool": tool, "domain": "", "column": feature, "value": entry["mark"], "runs": ";".join(evidence),
                          "note": entry["claim"] + (" NOT ISOLATED: " + entry["not_isolated"] if "not_isolated" in entry else "")})
            tex.append(r"\cmark{}" if entry["mark"] == "check" else r"\xmark{}")
            used.update(evidence)
        rows.append(" & ".join(tex) + r" \\" + "\n")
    footnotes = "\n".join(rf"\textsuperscript{{{key}}}{text}" for key, text in FOOTNOTE_TEXT.items())
    table = TEMPLATE.replace("%ROWS%", "".join(rows)).replace("%FOOTNOTES%", footnotes)

    cases = json.loads((EVAL / "tool_run_log/cases.json").read_text())["inputs"]
    run_rows = []
    for run in sorted(used):
        row, spec = runs[run], cases[runs[run]["input"]]
        archived = f"tool_eval_logs.tar.gz: tool-eval-logs/tool_run_log/{run}/"
        run_rows.append({
            "run": run, "tool": row["tool"], "input": row["input"],
            "input_config": spec["config"], "data_type": spec["data_type"], "patch": spec["patch"],
            "input_top_cpp_sha256": spec["sha256"], "tool_options": row["tool_options"],
            "stage_reached": row["stage_reached"], "status": row["status"], "functional_check": row["functional_check"],
            "synth": row["synth"], "synth_record": row["synth_source"], "latency_cycles": row["latency_cycles"],
            "emitted_code": f"generated_hls/{row['emitted']}/" if row["emitted"] else "",
            "logs": f"tool_run_log/logs/{run}/", "full_logs": archived})
    return table, cells, run_rows


def to_csv(rows):
    fields = list(dict.fromkeys(key for row in rows for key in row))
    stream = io.StringIO()
    writer = csv.DictWriter(stream, fieldnames=fields, lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    return stream.getvalue()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="compare with the committed table/ files")
    args = parser.parse_args()
    table, cells, run_rows = build()
    outputs = {"tool_support.tex": table, "cells.csv": to_csv(cells), "runs.csv": to_csv(run_rows)}
    for name, text in outputs.items():
        path = EVAL / "table" / name
        if args.check:
            if not path.is_file() or path.read_text() != text:
                sys.exit(f"table/{name} is out of date; run scripts/paper_table.py")
        else:
            path.write_text(text)
    print(f"{len(cells)} cells from {len(run_rows)} runs" + (" match" if args.check else " written"))


if __name__ == "__main__":
    main()
