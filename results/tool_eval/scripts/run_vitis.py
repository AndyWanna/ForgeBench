#!/usr/bin/env python3
"""Synthesize a generated project in a fresh directory and preserve its evidence."""
import argparse
import json
import os
import shutil
import signal
import subprocess
from pathlib import Path

from common import sanitize, sha256, write_json, write_text


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("design", type=Path, help="full generated project, including DRAM text inputs")
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--timeout", type=int, default=900)
    args = parser.parse_args()
    source, out = args.design.resolve(), args.out.resolve()
    shutil.copytree(source, out)  # Refuses to overwrite another run.
    if (out / "project_1").exists():
        raise SystemExit("Input is not a fresh generated project")
    command = ["vitis_hls", "-f", "run_hls.tcl"]
    run = subprocess.Popen(command, cwd=out, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                           text=True, start_new_session=True)
    try:
        output, _ = run.communicate(timeout=args.timeout)
    except subprocess.TimeoutExpired:
        os.killpg(run.pid, signal.SIGTERM)
        try:
            output, _ = run.communicate(timeout=10)
        except subprocess.TimeoutExpired:
            os.killpg(run.pid, signal.SIGKILL)
            output, _ = run.communicate()
    cleaned = sanitize(output, [(out, "<DESIGN>"), (out.resolve(), "<DESIGN>")])
    report = out / "project_1/solution1/syn/report/csynth.xml"
    passed = run.returncode == 0 and report.is_file()
    write_text(out / "full.log", "command: vitis_hls -f run_hls.tcl\n" + cleaned)
    write_text(out / "run.log", f"command: vitis_hls -f run_hls.tcl\nexit_code: {run.returncode}\n"
               + "\n".join(cleaned.splitlines()[:12] + ["[middle omitted]"] + cleaned.splitlines()[-50:]) + "\n")
    result = {"source_sha256": sha256(source / "top.cpp"), "csynth_passed": passed,
              "exit_code": run.returncode, "report": "project_1/solution1/syn/report/csynth.xml" if passed else ""}
    write_json(out / "result.json", result)
    print(json.dumps(result))


if __name__ == "__main__":
    main()
