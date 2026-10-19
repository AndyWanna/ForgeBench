#!/usr/bin/env python3
"""Run an unchanged ForgeBench design through a pinned MLIR tool build.

Commands are executed separately, so a failed front end cannot be hidden by a
successful downstream shell pipeline. Every attempted stage has its own log.
"""
import argparse
import json
import os
import re
import shlex
import subprocess
import uuid
from pathlib import Path

from common import stop_container, sanitize, sha256, write_json, write_text
from normalize_stream_dlti import normalize
from stream_handoff import adapt as stream_handoff


def first_error(output):
    return next((line.strip() for line in output.splitlines()
                 if re.search(r"fatal error:|\berror:|LLVM ERROR|Assertion .*failed|UNREACHABLE|unsupported|Unsupported", line)), "")


def top_symbol_from_ast(output, top):
    """Clang's filtered AST is a sequence of JSON objects, not one array."""
    decoder = json.JSONDecoder()
    symbols = set()
    while output.strip():
        output = output.lstrip()
        node, end = decoder.raw_decode(output)
        output = output[end:]
        if (node.get("kind") == "FunctionDecl" and node.get("name") == top
                and any(child.get("kind") == "CompoundStmt" for child in node.get("inner", []))):
            symbols.add(node["mangledName"])
    if len(symbols) != 1:
        raise ValueError(f"Expected one definition of {top}; found {len(symbols)} symbols")
    return symbols.pop()


def has_mlir_definition(output, symbol):
    return bool(re.search(r"func\.func\s+(?:private\s+)?@" + re.escape(symbol)
                          + r"\s*\([^\n]*\)[^\n]*\{", output))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("tool", choices=["scalehls", "hida", "streamhls"])
    parser.add_argument("design", type=Path)
    parser.add_argument("--domain", choices=["gemm", "dnn", "llm"], required=True)
    parser.add_argument("--design-id", required=True)
    parser.add_argument("--work", type=Path, required=True, help="directory containing tools/ and build/")
    parser.add_argument("--vitis-include", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--image", default="forgebench-tool-eval-build:20261016")
    parser.add_argument("--timeout", type=int, default=600)
    parser.add_argument("--top", default="top")
    parser.add_argument("--input-edits", default="none", help="name of the patch applied to the generated source")
    parser.add_argument("--pipeline-options", default="",
                        help="extra options appended to the tool's pass-pipeline options, e.g. place-external-buffer=false")
    parser.add_argument("--stream-polygeist-handoff", action="store_true",
                        help="StreamHLS only: apply stream_handoff.py and bufferize function arguments")
    args = parser.parse_args()
    design, work, include, out = (p.resolve() for p in (args.design, args.work, args.vitis_include, args.out))
    if not (include / "ap_fixed.h").is_file():
        raise SystemExit("Real Vitis headers are required")
    out.mkdir(parents=True, exist_ok=False)
    family = "stream-frontend" if args.tool == "streamhls" else "scale"
    if not list((work / "build" / family / "lib/clang").glob("*/include/stddef.h")):
        raise SystemExit("Clang resource headers are missing; build the clang-resource-headers target first")
    frontend = f"/work/build/{family}/bin/cgeist"
    backend_dir = "scale" if args.tool == "scalehls" else args.tool
    backend = f"/work/build/{backend_dir}/bin"
    before = sha256(design / "top.cpp")
    image_id = subprocess.check_output(["docker", "image", "inspect", args.image, "--format", "{{.Id}}"], text=True).strip()
    # C++ names must be selected by ABI symbol; -function=top silently emits an
    # empty module. Clang is used only for name discovery, never as a substitute
    # importer or to rewrite benchmark source.
    selection = ["clang++", "-fsyntax-only", "-std=c++14", "-I/design", "-I/vitis/include",
                 "-Xclang", "-ast-dump=json", "-Xclang", f"-ast-dump-filter={args.top}", "/design/top.cpp"]
    selected = subprocess.run(["docker", "run", "--rm", "--network", "none",
        "--user", f"{os.getuid()}:{os.getgid()}",
        "--mount", f"type=bind,src={design},dst=/design,readonly",
        "--mount", f"type=bind,src={include},dst=/vitis/include,readonly", image_id, *selection],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=args.timeout)
    if selected.returncode:
        raise SystemExit("Top-symbol discovery failed: " + sanitize(selected.stderr))
    symbol = top_symbol_from_ast(selected.stdout, args.top)
    write_text(out / "symbol-selection.log", "command: " + shlex.join(selection)
               + f"\nexit_code: {selected.returncode}\nsource_top: {args.top}\nselected_symbol: {symbol}\n")
    commands = [
        ("parse", [frontend, "/design/top.cpp", f"-function={symbol}", "-S", "-memref-fullrank",
                   "-raise-scf-to-affine", "-std=c++14", "-I/design", "-I/vitis/include",
                   "-o", "/run/parsed.mlir"]),
    ]
    if args.tool == "scalehls":
        commands += [
            ("transform", [backend + "/scalehls-opt", "/run/parsed.mlir",
                           f"-scalehls-dse-pipeline=top-func={symbol} target-spec=/work/tools/scalehls/samples/polybench/config.json"
                           + (" " + args.pipeline_options if args.pipeline_options else ""),
                           "-o", "/run/transformed.mlir"]),
            ("codegen", [backend + "/scalehls-translate", "/run/transformed.mlir", "-scalehls-emit-hlscpp", "-o", "/run/emitted.cpp"]),
        ]
    elif args.tool == "hida":
        commands += [
            ("transform", [backend + "/scalehls-opt", "/run/parsed.mlir", f"-hida-cpp-pipeline=top-func={symbol}" + (" " + args.pipeline_options if args.pipeline_options else ""), "-o", "/run/transformed.mlir"]),
            ("codegen", [backend + "/scalehls-translate", "/run/transformed.mlir", "-scalehls-emit-hlscpp", "-emit-vitis-directives", "-o", "/run/emitted.cpp"]),
        ]
    else:
        commands += [
            ("transform", [backend + "/streamhls-opt", "/run/compatible.mlir",
                           f"-streamhls-kernel-pipeline=top-func={symbol} board-dsps=2520 graph-file=/run/graph report-file=/run/report optimize-schedule=false parallelize-nodes=false combined-optimization=false"
                           + (" bufferize-func-args=true" if args.stream_polygeist_handoff else "")
                           + (" " + args.pipeline_options if args.pipeline_options else ""),
                           "-o", "/run/transformed.mlir"]),
            ("codegen", [backend + "/streamhls-translate", "/run/transformed.mlir", "-emit-vivado-hls", "-allow-unregistered-dialect", f"-vitis-hls-top-func={symbol}", "-o", "/run/emitted.cpp"]),
        ]
    replacements = [(design, "<DESIGN>"), (include, "<VITIS_INCLUDE>"), (work, "<WORK>"),
                    (out, "<RUN>"), ("/design", "<DESIGN>"), ("/vitis/include", "<VITIS_INCLUDE>"),
                    ("/work", "<WORK>"), ("/run", "<RUN>"), ("/usr/include", "<SYSTEM_INCLUDE>")]
    result = {"design_id": args.design_id, "domain": args.domain,
              "tool": {"scalehls": "ScaleHLS", "hida": "HIDA", "streamhls": "StreamHLS"}[args.tool],
              "source_sha256": before, "input_edits": args.input_edits, "pipeline_options": args.pipeline_options, "image_id": image_id, "top_symbol": symbol,
              "stage_reached": "", "rating": "", "failure_category": "", "error_head": "", "stages": []}
    combined = []
    for stage, compiler in commands:
        binary = work / compiler[0].removeprefix("/work/")
        if not binary.is_file():
            result.update(status="missing_executable", error_head=f"Required executable absent: {compiler[0].removeprefix('/work/')}")
            break
        container = "forgebench-mlir-" + uuid.uuid4().hex[:12]
        command = ["docker", "run", "--rm", "--name", container, "--network", "none",
                   "--user", f"{os.getuid()}:{os.getgid()}", "--ulimit", "core=0",
                   "--mount", f"type=bind,src={work},dst=/work,readonly",
                   "--mount", f"type=bind,src={design},dst=/design,readonly",
                   "--mount", f"type=bind,src={include},dst=/vitis/include,readonly",
                   "--mount", f"type=bind,src={out},dst=/run", "--workdir", "/run", image_id, *compiler]
        timed_out = False
        try:
            run = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, timeout=args.timeout)
            code, raw = run.returncode, run.stdout
        except subprocess.TimeoutExpired as exc:
            stop_container(container)
            code, raw, timed_out = 124, exc.stdout or b"", True
            if isinstance(raw, bytes):
                raw = raw.decode(errors="replace")
        clean = sanitize(raw, replacements)
        header = f"stage: {stage}\ncommand (inside build image): {sanitize(shlex.join(compiler), replacements)}\nexit_code: {code}\n"
        write_text(out / (stage + ".log"), header + clean)
        combined.append(header + clean)
        result["stages"].append({"stage": stage, "exit_code": code})
        if timed_out or code in (125, 126, 127):
            result.update(status="timeout" if timed_out else "environment_failure", error_head=first_error(clean))
            if timed_out:
                result.update(stage_reached=stage)   # like other failures: the stage attempted
            break
        result.update(stage_reached=stage)
        if code or re.search(r"\b(?:fatal )?error:|LLVM ERROR|Assertion .*failed", clean):
            result.update(status=stage + "_failed", rating="none", error_head=first_error(clean))
            break
        expected = out / {"parse": "parsed.mlir", "transform": "transformed.mlir", "codegen": "emitted.cpp"}[stage]
        if not expected.is_file() or not expected.stat().st_size:
            result.update(status="missing_stage_output", error_head="Stage exited zero without a nonempty output")
            break
        if stage in ("parse", "transform") and not has_mlir_definition(expected.read_text(), symbol):
            result.update(status="missing_top_definition", rating="", error_head="Output has no requested top-function definition")
            break
        result.update(status=stage + "_passed")
        if stage == "parse" and args.tool == "streamhls":
            before_ir = expected.read_text()
            after_ir = normalize(before_ir)
            scope = "DLTI integer metadata payload widths only"
            if args.stream_polygeist_handoff:
                after_ir = stream_handoff(after_ir, symbol)
                scope += "; memref.alloca -> memref.alloc; uncalled helper functions dropped; bufferize-func-args=true"
            write_text(out / "compatible.mlir", after_ir)
            result["ir_compatibility_adapter"] = {"script": "normalize_stream_dlti.py"
                                                            + (" + stream_handoff.py" if args.stream_polygeist_handoff else ""),
                                                   "changed": before_ir != after_ir,
                                                   "scope": scope, "input_sha256": sha256(expected),
                                                   "output_sha256": sha256(out / "compatible.mlir")}
    assert before == sha256(design / "top.cpp"), "The generated source changed"
    # A successful emitter still requires a separate Vitis run. Never infer
    # synthesis support from a zero tool exit or mere file creation.
    if result.get("status") == "codegen_passed":
        result.update(status="codegen_requires_synthesis", rating="")
    header = f"tool: {result['tool']}\ndesign_id: {args.design_id}\nsource_sha256: {before}\ninput_edits: {args.input_edits}\nimage_id: {image_id}\nfirst_error: {result['error_head']}\n"
    full = header + "\n".join(combined)
    write_text(out / "full.log", full)
    lines = full.splitlines()
    write_text(out / "run.log", full if len(lines) <= 80 else "\n".join(lines[:22] + ["[middle omitted]"] + lines[-50:]) + "\n")
    write_json(out / "result.json", result)
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
