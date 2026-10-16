#!/usr/bin/env python3
"""Run pinned AutoSA on an unchanged design directory; keep failures explicit.

The result records the failing/last attempted stage, not a claim that parsing
passed. A zero exit code is not sufficient evidence of successful codegen.
"""
import argparse
import json
import os
import re
import shlex
import subprocess
import uuid
from pathlib import Path

from common import AUTOSA_IMAGE, sanitize, sha256, write_json, write_text


def classify(returncode, output, generated):
    diagnostic = next((line.strip() for line in output.splitlines()
                       if re.search(r"fatal error:|\berror:|LLVM ERROR:|Assertion .*failed|\[AutoSA\] Error", line)), "")
    if returncode in (124, 125, 126, 127):
        return {"stage_reached": "", "rating": "", "failure_category": "", "error_head": diagnostic,
                "status": "timeout" if returncode == 124 else "environment_failure"}
    if "clang::NamedDecl::getName() const: Assertion" in output:
        return {"stage_reached": "parse", "rating": "none", "failure_category": "",
                "error_head": diagnostic, "status": "frontend_assertion"}
    if re.search(r"fatal error:|\berror:", output):
        # Categories require manual attribution to a paper feature. In
        # particular a missing header is not an unsupported LLM operator.
        return {"stage_reached": "parse", "rating": "none", "failure_category": "",
                "error_head": diagnostic, "status": "parse_failed"}
    if returncode != 0:
        return {"stage_reached": "", "rating": "", "failure_category": "",
                "error_head": diagnostic, "status": "unclassified_failure"}
    if generated:
        return {"stage_reached": "codegen", "rating": "", "failure_category": "",
                "error_head": "", "status": "codegen_requires_synthesis"}
    return {"stage_reached": "", "rating": "", "failure_category": "",
            "error_head": "", "status": "no_kernel_emitted"}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("design", type=Path)
    parser.add_argument("--domain", choices=["gemm", "dnn", "llm"], required=True)
    parser.add_argument("--design-id", required=True)
    parser.add_argument("--vitis-include", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True, help="new external work directory")
    parser.add_argument("--timeout", type=int, default=180)
    parser.add_argument("--synthesis-define", action="store_true", help="pass the normal HLS __SYNTHESIS__ macro")
    parser.add_argument("--input-patch", type=Path, help="record the patch when running an adapted diagnostic input")
    args = parser.parse_args()
    design, include, out = args.design.resolve(), args.vitis_include.resolve(), args.out.resolve()
    if not (design / "top.cpp").is_file() or not (include / "ap_fixed.h").is_file():
        raise SystemExit("Expected top.cpp and real Vitis ap_fixed.h headers")
    out.mkdir(parents=True, exist_ok=False)
    for directory in ["src", "latency_est", "resource_est", "tuning"]:
        (out / "output" / directory).mkdir(parents=True)
    # Fixed container mount names avoid shell interpolation of source paths
    # in AutoSA's own driver. Explicit include paths resolve C++/vendor headers.
    compiler = ["./autosa", "/design/top.cpp", "--config=./autosa_config/autosa_config.json",
                "--target=autosa_hls_c", "--output-dir=/work/output", "--hls",
                "--pet-autodetect", "-I/design", "-I/vitis/include",
                "-I/usr/include/c++/9", "-I/usr/include/x86_64-linux-gnu/c++/9"]
    if args.synthesis_define:
        compiler.append("-D__SYNTHESIS__")
    container = "forgebench-autosa-" + uuid.uuid4().hex[:12]
    command = ["docker", "run", "--rm", "--name", container, "--network", "none", "--user", f"{os.getuid()}:{os.getgid()}",
               "--mount", f"type=bind,src={design},dst=/design,readonly",
               "--mount", f"type=bind,src={include},dst=/vitis/include,readonly",
               "--mount", f"type=bind,src={out},dst=/work",
               "--workdir", "/usr/src/docker_autosa", "--entrypoint", "/bin/bash", AUTOSA_IMAGE,
               "-c", 'exec "$@"', "bash", *compiler]
    before = sha256(design / "top.cpp")
    try:
        run = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                             text=True, timeout=args.timeout)
        returncode, raw = run.returncode, run.stdout
    except subprocess.TimeoutExpired as exc:
        raw = exc.stdout or b""
        if isinstance(raw, bytes):
            raw = raw.decode(errors="replace")
        subprocess.run(["docker", "stop", "--time", "5", container], capture_output=True, timeout=15)
        returncode = 124
    assert before == sha256(design / "top.cpp"), "The generated source changed"
    replacements = [(design, "<DESIGN>"), (include, "<VITIS_INCLUDE>"), (out, "<WORK>"),
                    ("/usr/src/docker_autosa", "<TOOL_ROOT>"),
                    ("/design", "<DESIGN>"), ("/vitis/include", "<VITIS_INCLUDE>"),
                    ("/work", "<WORK>"), ("/usr/include", "<SYSTEM_INCLUDE>")]
    clean = sanitize(raw, replacements)
    kernels = sorted(str(p.relative_to(out)) for p in (out / "output").rglob("*_kernel.cpp"))
    result = classify(returncode, clean, kernels)
    if returncode == 124:
        result.update(stage_reached="", rating="", status="timeout")
    edits = f"diagnostic patch {args.input_patch.name}; sha256={sha256(args.input_patch)}" if args.input_patch else "none"
    result.update(design_id=args.design_id, domain=args.domain, tool="AutoSA",
                  image=AUTOSA_IMAGE, source_sha256=before, exit_code=returncode,
                  generated_kernels=kernels, input_edits=edits)
    header = (f"design_id: {args.design_id}\ndomain: {args.domain}\ninput_edits: {edits}\n"
              f"source_sha256: {before}\nimage: {AUTOSA_IMAGE}\n"
              f"command (inside pinned container): {sanitize(shlex.join(compiler), replacements)}\n"
              f"exit_code: {returncode}\nstatus: {result['status']}\n")
    write_text(out / "full.log", header + clean)
    lines = clean.splitlines()
    excerpt = clean if len(lines) <= 75 else "\n".join(lines[:20] + ["[middle omitted; see full.log]"] + lines[-50:]) + "\n"
    write_text(out / "run.log", header + excerpt)
    write_json(out / "result.json", result)
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
