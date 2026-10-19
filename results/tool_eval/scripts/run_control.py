#!/usr/bin/env python3
"""Run an upstream positive control in a fresh external directory."""
import argparse
import os
import shlex
import subprocess
import uuid
from pathlib import Path

from common import AUTOSA_IMAGE, EVAL, sanitize, write_json, write_text


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("tool", choices=["scalehls", "hida", "autosa", "streamhls"])
    parser.add_argument("--work", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    work, out = args.work.resolve(), args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    image = AUTOSA_IMAGE if args.tool == "autosa" else "forgebench-tool-eval-build:20261016"
    script = "control_autosa.sh" if args.tool == "autosa" else "control_mlir.sh"
    container = "forgebench-control-" + uuid.uuid4().hex[:12]
    command = ["docker", "run", "--rm", "--name", container, "--network", "none",
               "--user", f"{os.getuid()}:{os.getgid()}", "--ulimit", "core=0",
               "--mount", f"type=bind,src={work},dst=/work,readonly",
               "--mount", f"type=bind,src={out},dst=/control",
               "--mount", f"type=bind,src={EVAL / 'scripts'},dst=/scripts,readonly",
               "--workdir", "/control", "--entrypoint", "bash", image,
               "/scripts/" + script, args.tool]
    try:
        run = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, timeout=600)
        code, output = run.returncode, run.stdout
    except subprocess.TimeoutExpired as exc:
        subprocess.run(["docker", "stop", "--time", "5", container], capture_output=True, timeout=15)
        code, output = 124, exc.stdout or b""
        if isinstance(output, bytes):
            output = output.decode(errors="replace")
    passed = code == 0 and "CONTROL PASS" in output
    replacements = [(work, "<WORK>"), (out, "<CONTROL>"), (EVAL, "<EVAL>"),
                    ("/work", "<WORK>"), ("/control", "<CONTROL>")]
    log = sanitize(f"command: {shlex.join(command)}\nexit_code: {code}\n" + output, replacements)
    write_text(out / "run.log", log)
    write_json(out / "result.json", {"tool": args.tool, "exit_code": code, "passed": passed,
                                    "scope": "Upstream control, not a ForgeBench evaluation input"})
    print(log)
    raise SystemExit(0 if passed else 1)


if __name__ == "__main__":
    main()
