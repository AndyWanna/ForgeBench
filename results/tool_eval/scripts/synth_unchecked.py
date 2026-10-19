#!/usr/bin/env python3
"""Synthesize every emitted kernel that minimal_run.py left unsynthesized; write tool_run_log/synth_unchecked.csv.

minimal_run.py synthesizes only code that passes the functional check. The paper's table also
counts code that synthesizes but fails the check (wrong results, a hang, or an interface the
check harness cannot call), so this script runs the same Vitis HLS flow as synth_emitted.py on
every remaining row of tool_run_log/results.csv that reached code generation. It reads the kernels
from generated_hls/, the byte-identical copy of the tools' output in this repository:
emitted.cpp with the run's top symbol for ScaleHLS, HIDA and StreamHLS, and top_kernel.cpp with
kernel0 for AutoSA. Build trees stay in <work>/synth_unchecked/; the redacted full log and
csynth.xml of each run go to <work>/archive/minimal/<run>/ for archive_logs.py.
"""
import argparse
import csv
import json
import shutil
import subprocess
import xml.etree.ElementTree as ET
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from common import EVAL, REPO, sanitize, sanitize_xml, sha256, write_json, write_text

FIELDS = ["run", "tool", "input", "functional_check", "kernel", "kernel_sha256", "top", "synth",
          "latency_cycles", "est_clock_ns", "BRAM_18K", "DSP", "FF", "LUT", "first_error"]
TCL = ("open_project project_1\nset_top {top}\nadd_files {kernel}\nopen_solution solution1 -flow_target vivado\n"
       "set_part {{xczu9eg-ffvb1156-2-e}}\ncreate_clock -period 10 -name default\ncsynth_design\nexit\n")


def pending():
    """Rows that emitted code but were not synthesized, with their kernel in generated_hls/."""
    with (EVAL / "generated_hls/index.csv").open() as stream:
        kernels = {row["run"]: row for row in csv.DictReader(stream)}
    with (EVAL / "tool_run_log/results.csv").open() as stream:
        for row in csv.DictReader(stream):
            if row["level"] == "control" or row["status"] != "codegen_requires_synthesis":
                continue
            entry = kernels[row["run"]]
            if row["tool"] == "AutoSA":
                kernel, top = "top_kernel.cpp", "kernel0"
            else:
                kernel = "emitted.cpp"
                top = json.loads((EVAL / "tool_run_log/logs" / row["run"] / "result.json").read_text())["top_symbol"]
            yield row, EVAL / "generated_hls" / entry["path"], kernel, top


def qor(report):
    root = ET.parse(report).getroot()
    performance, resources = root.find("PerformanceEstimates"), root.find("AreaEstimates/Resources")
    return {"latency_cycles": performance.findtext("SummaryOfOverallLatency/Worst-caseLatency"),
            "est_clock_ns": performance.findtext("SummaryOfTimingAnalysis/EstimatedClockPeriod"),
            **{name: resources.findtext(name) for name in ["BRAM_18K", "DSP", "FF", "LUT"]}}


def synth(job, work, timeout):
    row, source, kernel, top = job
    out = work / "synth_unchecked" / row["run"].replace("/", "__")
    record = out / "record.json"
    if record.is_file():
        return json.loads(record.read_text())
    shutil.rmtree(out, ignore_errors=True)
    out.mkdir(parents=True)
    for path in source.iterdir():     # the kernel and, for AutoSA, the header it includes
        shutil.copy2(path, out / path.name)
    write_text(out / "run_hls.tcl", TCL.format(top=top, kernel=kernel))
    proc = subprocess.run(["timeout", "-s", "KILL", str(timeout), "vitis_hls", "-f", "run_hls.tcl"],
                          cwd=out, capture_output=True, text=True)
    log = sanitize(f"command: vitis_hls -f run_hls.tcl\nexit_code: {proc.returncode}\n" + proc.stdout + proc.stderr,
                   [(out, "<SYNTH_RUN>")])
    report = out / "project_1/solution1/syn/report/csynth.xml"
    passed = proc.returncode == 0 and report.is_file()
    result = {"run": row["run"], "tool": row["tool"], "input": row["input"], "functional_check": row["functional_check"],
              "kernel": f"generated_hls/{source.relative_to(EVAL / 'generated_hls').as_posix()}/{kernel}",
              "kernel_sha256": sha256(source / kernel), "top": top,
              "synth": "passed" if passed else ("timeout" if proc.returncode in (124, 137, -9) else "failed")}
    archive = work / "archive/minimal" / row["run"]
    write_text(archive / "synth-unchecked.log", log)
    if passed:
        result.update(qor(report))
        write_text(archive / "synth-unchecked-csynth.xml", sanitize_xml(report.read_text(), [(out, "<SYNTH_RUN>")]))
    else:
        result["first_error"] = next((line for line in log.splitlines() if "ERROR" in line), log.splitlines()[-1])[:300]
    # A short excerpt in the repository, next to the run's other logs; the full log is archived.
    lines = log.splitlines()
    write_text(EVAL / "tool_run_log/logs" / row["run"] / "synth-unchecked.log",
               "\n".join(lines[:2] + [line for line in lines if "ERROR" in line][:5]
                         + ["[middle omitted; full log in tool_eval_logs.tar.gz]"] + lines[-12:]) + "\n")
    write_json(record, result)
    print(json.dumps({key: result.get(key, "") for key in ["run", "synth", "latency_cycles"]}), flush=True)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True, help="the --work directory of minimal_run.py")
    parser.add_argument("--jobs", type=int, default=16)
    parser.add_argument("--timeout", type=int, default=3600)
    args = parser.parse_args()
    work = args.work.resolve()
    if work == REPO or REPO in work.parents:
        raise SystemExit("--work must be outside the repository")
    jobs = list(pending())
    with ThreadPoolExecutor(args.jobs) as pool:
        results = list(pool.map(lambda job: synth(job, work, args.timeout), jobs))
    with (EVAL / "tool_run_log/synth_unchecked.csv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=FIELDS, lineterminator="\n")
        writer.writeheader()
        writer.writerows({key: result.get(key, "") for key in FIELDS} for result in results)
    print(f"{len(results)} kernels: {sum(r['synth'] == 'passed' for r in results)} synthesized")


if __name__ == "__main__":
    main()
