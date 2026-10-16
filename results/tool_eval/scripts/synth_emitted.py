#!/usr/bin/env python3
"""Synthesize a tool-emitted kernel; preserve the pre-synthesis run record."""
import argparse
import json
import os
import re
import shutil
import signal
import subprocess
from pathlib import Path

from common import sanitize, sanitize_xml, sha256, write_json, write_text
from run_mlir import first_error


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("run", type=Path, help="completed MLIR runner output directory")
    parser.add_argument("--out", type=Path, required=True, help="new external synthesis directory")
    parser.add_argument("--timeout", type=int, default=900)
    args = parser.parse_args()
    run, out = args.run.resolve(), args.out.resolve()
    result = json.loads((run / "result.json").read_text())
    assert result["status"] == "codegen_requires_synthesis"
    symbol = result["top_symbol"]
    assert re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", symbol)
    out.mkdir(parents=True, exist_ok=False)
    shutil.copy2(run / "emitted.cpp", out / "emitted.cpp")
    write_text(out / "run_hls.tcl", "open_project project_1\n" + f"set_top {symbol}\n"
               "add_files emitted.cpp\nopen_solution solution1 -flow_target vivado\n"
               "set_part {xczu9eg-ffvb1156-2-e}\ncreate_clock -period 10 -name default\ncsynth_design\nexit\n")
    proc = subprocess.Popen(["vitis_hls", "-f", "run_hls.tcl"], cwd=out,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, start_new_session=True)
    timeout = False
    try:
        output, _ = proc.communicate(timeout=args.timeout)
    except subprocess.TimeoutExpired:
        timeout = True
        os.killpg(proc.pid, signal.SIGTERM)
        try:
            output, _ = proc.communicate(timeout=10)
        except subprocess.TimeoutExpired:
            os.killpg(proc.pid, signal.SIGKILL)
            output, _ = proc.communicate()
    log = f"command: vitis_hls -f run_hls.tcl\nexit_code: {proc.returncode}\n" + sanitize(output, [(out, "<SYNTH_RUN>")])
    write_text(out / "full.log", log)
    report = out / "project_1/solution1/syn/report/csynth.xml"
    passed = proc.returncode == 0 and report.is_file() and report.stat().st_size > 0
    write_json(run / "pre-synthesis-result.json", result)
    result["stages"].append({"stage": "synth", "exit_code": proc.returncode})
    result.update(status="synth_passed" if passed else ("synth_timeout" if timeout else "synth_failed"),
                  stage_reached="synth" if passed else "codegen", rating="full" if passed else ("" if timeout else "none"),
                  error_head="" if passed else first_error(log), emitted_source_sha256=sha256(run / "emitted.cpp"))
    write_json(run / "result.json", result)
    write_text(run / "full.log", (run / "full.log").read_text() + "\n" + log)
    write_text(run / "run.log", (run / "run.log").read_text() + "\n" + f"first_synthesis_error: {result['error_head']}\n"
               + "\n".join(log.splitlines()[:8] + ["[middle omitted]"] + log.splitlines()[-50:]) + "\n")
    if passed:
        write_text(run / "csynth.xml", sanitize_xml(report.read_text(), [(out, "<SYNTH_RUN>")]))
    shutil.copy2(out / "run_hls.tcl", run / "synth.tcl")
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
