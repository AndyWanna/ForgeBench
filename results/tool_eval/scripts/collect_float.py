#!/usr/bin/env python3
"""Collect the user-requested float cohort separately from fixed-point results."""
import argparse
import json
import re
import subprocess
import xml.etree.ElementTree as ET
from pathlib import Path

from common import EVAL, sanitize, sanitize_xml, sha256, write_json, write_text
from collect import TOOLS, write_csv


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True)
    args = parser.parse_args()
    work = args.work.resolve()
    summary, outcomes, records, baselines = [], [], [], []
    for slug, tool in TOOLS.items():
        for domain in ["gemm", "dnn", "llm"]:
            design_id = domain + "_0001_float"
            run = work / "runs-float" / slug / design_id
            result = json.loads((run / "result.json").read_text())
            assert result["source_sha256"] == sha256(EVAL / "designs" / design_id / "top.cpp")
            assert result["input_edits"] == "none"
            logdir = EVAL / "logs" / slug / domain
            if slug in ("scalehls", "hida") and result["status"] == "transform_failed":
                log = (run / "transform.log").read_text()
                offsets = re.findall(r"scalehls-opt\(\+(0x[0-9a-f]+)\)", log)
                binary = work / "build" / ("scale" if slug == "scalehls" else "hida") / "bin/scalehls-opt"
                trace = subprocess.check_output(["addr2line", "-Cf", "-e", str(binary), *offsets], text=True)
                write_text(logdir / "float-symbolized.log", "command: addr2line -Cf -e <BUILD>/bin/scalehls-opt "
                           + " ".join(offsets) + "\nexit_code: 0\n" + sanitize(trace))
                if "ConvertGetGlobalToConstBuffer" in trace and "memref.global" in (run / "parsed.mlir").read_text():
                    result.update(failure_category="F3", attribution="Global BRAM memref without an ElementsAttr initializer is passed to hls.const_buffer construction")
            write_json(logdir / "float-result.json", result)
            write_text(logdir / "float-run.log", sanitize((run / "run.log").read_text()))
            if (run / "symbol-selection.log").exists():
                write_text(logdir / "float-symbol-selection.log", sanitize((run / "symbol-selection.log").read_text(),
                           [("/design", "<DESIGN>"), ("/vitis/include", "<VITIS_INCLUDE>")]))
            write_text(work / "archive/float" / slug / domain / "full.log", sanitize((run / "full.log").read_text()))
            for name in ["parsed.mlir", "compatible.mlir", "transformed.mlir", "emitted.cpp", "synth.tcl"]:
                if (run / name).exists():
                    write_text(work / "archive/float" / slug / domain / name, sanitize((run / name).read_text()))
            if result["stage_reached"] == "synth":
                write_text(EVAL / "reports" / slug / design_id / "csynth.xml", (run / "csynth.xml").read_text())
            summary.append({"tool": tool, "domain": domain, "designs_tried": 1, "stage_reached": result["stage_reached"],
                            "rating": result["rating"], "notes": f"Float regeneration {design_id}; {result['status']}; logs/{slug}/{domain}/float-run.log. Same configuration except data_type; no precision-equivalence claim."})
            outcomes.append({key: result[key] for key in ["design_id", "domain", "tool", "stage_reached", "failure_category", "error_head"]})
            records.append(result)
    for domain in ["gemm", "dnn", "llm"]:
        design_id = domain + "_0001_float"
        run = work / "runs-float/vitis" / design_id
        result = json.loads((run / "result.json").read_text())
        assert result["csynth_passed"] and result["exit_code"] == 0
        xml = sanitize_xml((run / result["report"]).read_text())
        write_text(EVAL / "reports/vitis" / design_id / "csynth.xml", xml)
        write_text(EVAL / "logs/vitis" / domain / "float-run.log", sanitize((run / "run.log").read_text()))
        write_text(work / "archive/float/vitis" / domain / "full.log", sanitize((run / "full.log").read_text()))
        tree = ET.fromstring(xml)
        baselines.append({"design_id": design_id, "source_sha256": result["source_sha256"],
                          "part": tree.findtext("UserAssignments/Part"), "clock_ns": tree.findtext("UserAssignments/TargetClockPeriod"),
                          "latency_cycles": tree.findtext("PerformanceEstimates/SummaryOfOverallLatency/Worst-caseLatency"),
                          **{key: tree.findtext("AreaEstimates/Resources/" + key) for key in ["BRAM_18K", "DSP", "FF", "LUT"]}})
    write_csv(EVAL / "float_summary.csv", list(summary[0]), summary)
    write_csv(EVAL / "float_outcomes.csv", list(outcomes[0]), outcomes)
    write_json(EVAL / "float_runs.json", records)
    write_csv(EVAL / "float_baseline_vitis.csv", list(baselines[0]), baselines)
    print("Collected 12 float outcomes and three float Vitis baselines")


if __name__ == "__main__":
    main()
