#!/usr/bin/env python3
"""Publish the fresh experiment's logs, tables, and baseline reports.

This collector requires all twelve run records. Historical rewrites and
diagnostic adapters are kept outside the untouched-input summary.
"""
import argparse
import csv
import json
import shutil
import subprocess
import xml.etree.ElementTree as ET
from pathlib import Path

from common import AUTOSA_IMAGE, EVAL, sanitize, sanitize_xml, sha256, write_json, write_text

TOOLS = {"scalehls": "ScaleHLS", "hida": "HIDA", "autosa": "AutoSA", "streamhls": "StreamHLS"}


def write_csv(path, fields, rows):
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def annotate(result, transcript):
    # Narrow, manually audited attribution: the importer reports an EnumDecl
    # failure inside Vitis's fixed-point implementation. No inference about
    # dynamic loops, operators, or pragma preservation follows from it.
    result = dict(result)
    if (result.get("status") == "parse_failed"
            and "ap_fixed_base.h" in result.get("error_head", "")
            and "visiting unknonwn sub decl stmt" in transcript and "EnumDecl" in transcript):
        result.update(failure_category="F1", attribution="Importer rejects a local enum declaration in the vendor ap_fixed implementation; not a test of F2-F6")
    elif (result.get("status") == "parse_failed" and "ap_private.h" in result.get("error_head", "")
          and "'arith.ori' op requires the same type" in result["error_head"]):
        result.update(failure_category="F1", attribution="Vendor arbitrary-precision implementation lowers to invalid arith.ori operand/result types")
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True)
    args = parser.parse_args()
    work = args.work.resolve()
    summary, outcomes, run_records, baseline = [], [], [], []
    for slug, tool in TOOLS.items():
        run_folder = "autosa-final" if slug == "autosa" else slug
        for domain in ["gemm", "dnn", "llm"]:
            design_id = domain + "_0001"
            run = work / "runs" / run_folder / design_id
            result = annotate(json.loads((run / "result.json").read_text()), (run / "full.log").read_text())
            expected = sha256(EVAL / "designs" / design_id / "top.cpp")
            assert result["source_sha256"] == expected
            assert result["input_edits"] == "none"
            assert result["tool"] == tool and result["domain"] == domain
            logdir = EVAL / "logs" / slug / domain
            write_text(logdir / "run.log", sanitize((run / "run.log").read_text()))
            write_json(logdir / "result.json", result)
            if (run / "symbol-selection.log").is_file():
                write_text(logdir / "symbol-selection.log", sanitize((run / "symbol-selection.log").read_text(),
                           [("/design", "<DESIGN>"), ("/vitis/include", "<VITIS_INCLUDE>")]))
            write_text(work / "archive/fresh" / slug / domain / "full.log", sanitize((run / "full.log").read_text()))
            summary.append({"tool": tool, "domain": domain, "designs_tried": 1,
                            "stage_reached": result["stage_reached"], "rating": result["rating"],
                            "notes": f"Fresh unchanged {design_id}; {result['status']}; logs/{slug}/{domain}/run.log. Historical cohort unknown; one case is not domain-wide coverage."})
            outcomes.append({key: result[key] for key in ["design_id", "domain", "tool", "stage_reached", "failure_category", "error_head"]})
            run_records.append(result)
    for domain in ["gemm", "dnn", "llm"]:
        design_id = domain + "_0001"
        run = work / "runs/vitis" / design_id
        result = json.loads((run / "result.json").read_text())
        assert result["csynth_passed"] and result["exit_code"] == 0
        assert result["source_sha256"] == sha256(EVAL / "designs" / design_id / "top.cpp")
        xml = sanitize_xml((run / result["report"]).read_text())
        write_text(EVAL / "reports/vitis" / design_id / "csynth.xml", xml)
        write_text(EVAL / "logs/vitis" / domain / "run.log", sanitize((run / "run.log").read_text()))
        write_text(work / "archive/fresh/vitis" / domain / "full.log", sanitize((run / "full.log").read_text()))
        tree = ET.fromstring(xml)
        baseline.append({"design_id": design_id, "source_sha256": result["source_sha256"],
                         "part": tree.findtext("UserAssignments/Part"),
                         "clock_ns": tree.findtext("UserAssignments/TargetClockPeriod"),
                         "latency_cycles": tree.findtext("PerformanceEstimates/SummaryOfOverallLatency/Worst-caseLatency"),
                         **{name: tree.findtext("AreaEstimates/Resources/" + name) for name in ["BRAM_18K", "DSP", "FF", "LUT"]}})
        for folder, label in [("autosa-headers", "headers-only"), ("autosa-diagnostic", "headers-synthesis")]:
            diagnostic = work / "runs" / folder / design_id
            for filename in ["run.log", "result.json"]:
                write_text(EVAL / "logs/autosa" / domain / (label + "-" + filename), sanitize((diagnostic / filename).read_text()))
            write_text(work / "archive/diagnostic/autosa" / domain / (label + ".log"), sanitize((diagnostic / "full.log").read_text()))
    write_csv(EVAL / "summary.csv", ["tool", "domain", "designs_tried", "stage_reached", "rating", "notes"], summary)
    write_csv(EVAL / "outcomes.csv", ["design_id", "domain", "tool", "stage_reached", "failure_category", "error_head"], outcomes)
    write_csv(EVAL / "baseline_vitis.csv", list(baseline[0]), baseline)
    write_json(EVAL / "runs.json", run_records)
    revisions = {}
    for name in ["scalehls", "hida", "autosa", "streamhls", "stream-polygeist", "scalehls/polygeist",
                 "scalehls/polygeist/llvm-project", "streamhls/extern/llvm-project", "stream-polygeist/llvm-project"]:
        revisions[name] = subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=work / "tools" / name, text=True).strip()
    binaries = {}
    for build, names in [("scale", ["cgeist", "scalehls-opt", "scalehls-translate"]),
                         ("hida", ["scalehls-opt", "scalehls-translate"]),
                         ("stream-frontend", ["cgeist"]), ("streamhls", ["streamhls-opt", "streamhls-translate"])]:
        for name in names:
            binary = work / "build" / build / "bin" / name
            binaries[f"{build}/{name}"] = sha256(binary)
    image_id = subprocess.check_output(["docker", "image", "inspect", "forgebench-tool-eval-build:20261016", "--format", "{{.Id}}"], text=True).strip()
    write_json(EVAL / "versions.json", {"source_commits": revisions, "binary_sha256": binaries,
                                      "evaluation_date": "2026-10-16",
                                      "build_image_id": image_id, "autosa_image": AUTOSA_IMAGE,
                                      "source_patches": ["patches/build/hida-target-names.patch"],
                                      "stream_ir_adapter": "scripts/normalize_stream_dlti.py; lossless DLTI metadata integer-payload width conversion",
                                      "llvm_versions": {"ScaleHLS/HIDA": "16.0.0git", "StreamHLS backend": "18.1.2",
                                                        "StreamHLS Polygeist frontend": "18.0.0git"},
                                      "historical_tool_commits": "unknown; these pins describe the fresh evaluation only"})
    for filename in ["build-scale.log", "build-scale-headers.log", "build-hida.log", "build-stream.log",
                     "build-stream-native.log", "build-stream-backend.log", "build-stream-final.log", "build-stream-verify.log",
                     "build-stream-frontend.log", "build-stream-frontend-verify.log", "control-autosa.log", "control-scalehls.log",
                     "control-hida.log", "control-streamhls.log", "control-streamhls-dlti-setup.log",
                     "control-streamhls-compatible.log", "control-streamhls-compatible-all.log", "environment-probe.log"]:
        if not (work / filename).is_file():
            continue  # A clean rebuild need not reproduce failed setup attempts.
        raw = (work / filename).read_text(errors="replace")
        clean = sanitize(raw, [(work, "<WORK>"), ("/work", "<WORK>"), ("/control", "<CONTROL>")])
        write_text(work / "archive" / filename, clean)
        lines = clean.splitlines()
        write_text(EVAL / "logs/environment" / filename, "\n".join(lines[-50:]) + "\n")
    for slug in TOOLS:
        assert "CONTROL PASS" in (work / ("control-" + slug + ".log")).read_text(), slug
    print("Collected 12 tool/domain outcomes and three original-Vitis baselines")


if __name__ == "__main__":
    main()
