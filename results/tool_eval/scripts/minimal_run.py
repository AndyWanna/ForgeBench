#!/usr/bin/env python3
"""Run the minimal-edit study described in minimal/cases.json and write minimal/results.csv.

Inputs come from minimal_inputs.py (run it first with the same --work). For each run, the tool's
pinned runner is invoked (run_mlir.py, run_autosa.py, run_vitis.py). Code that reaches code
generation is compared with the unmodified generated float design (minimal_check.py). Only
output that computes the right result is synthesized (synth_emitted.py). Every run leaves
<work>/runs/<run>/done.json, so an interrupted study resumes where it stopped (--force reruns).
The results table is rebuilt from those records; nothing in it is entered by hand.
"""
import argparse
import csv
import json
import re
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from common import AUTOSA_IMAGE, EVAL, sanitize, write_json, write_text
from minimal_check import compile_and_run
from minimal_inputs import generate_base

SCRIPTS = Path(__file__).resolve().parent
CASES = EVAL / "minimal/cases.json"
PY = sys.executable
FIELDS = ["run", "level", "domain", "input", "data_type", "patch", "tool", "tool_options", "stage_reached", "status",
          "functional_check", "max_error", "latency_cycles", "est_clock_ns", "BRAM_18K", "DSP", "FF", "LUT",
          "error_head", "detail"]
# AutoSA's manual tuning stages: the largest divisor of each tilable loop bound up to this limit.
AUTOSA_LIMIT = {"array_part": 16, "array_part_L2": 16, "latency": 4, "simd": 1}


def qor(report):
    if not report or not Path(report).is_file():
        return {}
    tree = ET.parse(report)
    return {"latency_cycles": tree.findtext("PerformanceEstimates/SummaryOfOverallLatency/Worst-caseLatency"),
            "est_clock_ns": tree.findtext("PerformanceEstimates/SummaryOfTimingAnalysis/EstimatedClockPeriod"),
            **{k: tree.findtext("AreaEstimates/Resources/" + k) for k in ["BRAM_18K", "DSP", "FF", "LUT"]}}


class Study:
    def __init__(self, args):
        self.args = args
        self.cases = json.loads(CASES.read_text())
        self.work = args.work.resolve()
        self.include = args.vitis_include.resolve()

    def reference(self, spec):
        """The unmodified generated float design for this input's config."""
        return generate_base({**spec, "data_type": "float"}, self.work) / "top.cpp"

    def check(self, spec, candidate, function, extras=(), call=None, timeout=300):
        ranges = {k: tuple(v) for k, v in spec.get("ranges", {}).items()}
        return compile_and_run(self.reference(spec), candidate, function, list(extras), call,
                               EVAL.parent.parent / spec["config"], ranges, self.include, timeout=timeout)

    def runner(self, command, log):
        run = subprocess.run(command, capture_output=True, text=True, errors="replace", cwd=SCRIPTS)
        write_text(log, run.stdout + run.stderr)
        return run.returncode

    # ---- one run ------------------------------------------------------------------------------
    def execute(self, run):
        out = self.work / "runs" / run["id"].replace("/", "__")
        if (out / "done.json").is_file() and not self.args.force:
            return
        shutil.rmtree(out, ignore_errors=True)
        out.mkdir(parents=True)
        spec = self.cases["inputs"].get(run["input"]) if run["input"] else None
        rows = {"vitis": self.vitis, "autosa": self.autosa, "autosa_control": self.autosa_control}.get(run["tool"], self.mlir)(run, spec, out)
        for row in rows:
            row.setdefault("run", run["id"])
            if spec:
                row.update(level=spec["level"], domain=spec["domain"], input=run["input"], data_type=spec["data_type"],
                           patch=spec["patch"] or "")
        write_json(out / "done.json", rows)
        print(f"{run['id']}: " + "; ".join(f"{r.get('status')} {r.get('functional_check', '')}".strip() for r in rows), flush=True)

    def vitis(self, run, spec, out):
        self.runner([PY, "run_vitis.py", str(self.work / "inputs" / run["input"]), "--out", str(out / "vitis")], out / "runner.log")
        result = json.loads((out / "vitis/result.json").read_text())
        report = out / "vitis" / result["report"] if result.get("csynth_passed") else None
        self.publish(run, out / "vitis", ["run.log", "result.json"])
        return [{"tool": "Vitis", "stage_reached": "synth" if report else "", "status": "synth_passed" if report else "synth_failed",
                 "functional_check": "reference" if report else "", **qor(report)}]

    def mlir(self, run, spec, out):
        tool = run["tool"]
        options = []
        command = [PY, "run_mlir.py", tool, str(self.work / "inputs" / run["input"]), "--domain", spec["domain"],
                   "--design-id", run["input"], "--work", str(self.args.tools_work), "--vitis-include", str(self.include),
                   "--out", str(out / tool), "--input-edits", Path(spec["patch"]).name if spec["patch"] else "none"]
        if run.get("pipeline_options"):
            command += ["--pipeline-options", run["pipeline_options"]]
            options.append(run["pipeline_options"])
        if run.get("handoff"):
            command.append("--stream-polygeist-handoff")
            options.append("--stream-polygeist-handoff")
        if run.get("timeout"):
            command += ["--timeout", str(run["timeout"])]
        self.runner(command, out / "runner.log")
        if not (out / tool / "result.json").is_file():
            return [{"tool": tool, "tool_options": " ".join(options), "status": "runner_error",
                     "error_head": sanitize((out / "runner.log").read_text().strip().splitlines()[-1])}]
        result = json.loads((out / tool / "result.json").read_text())
        if result["status"] == "timeout" and result["stages"]:
            result["stage_reached"] = result["stages"][-1]["stage"]   # the stage attempted, as for other failures
        row = {"tool": result["tool"], "tool_options": " ".join(options), "stage_reached": result["stage_reached"],
               "status": result["status"], "error_head": result["error_head"]}
        if result["status"] == "codegen_requires_synthesis":
            verdict = self.check(spec, out / tool / "emitted.cpp", result["top_symbol"], call=run.get("call"))
            write_json(out / tool / "check.json", verdict)
            row.update(functional_check=verdict["verdict"], max_error=verdict["max_error"], detail=verdict["detail"])
            if run.get("call"):
                row["detail"] = ("interface changed by the tool; checked through " + run["call"] + ". " + row["detail"]).strip()
            if verdict["verdict"] == "PASS":
                self.runner([PY, "synth_emitted.py", str(out / tool), "--out", str(out / "synth"),
                             "--timeout", str(run.get("synth_timeout", 3600))],
                            out / "synth.log")
                result = json.loads((out / tool / "result.json").read_text())
                row.update(stage_reached=result["stage_reached"], status=result["status"], error_head=result["error_head"],
                           **qor(out / tool / "csynth.xml"))
        self.publish(run, out / tool, ["run.log", "result.json", "check.json"])
        return [row]

    # ---- AutoSA ---------------------------------------------------------------------------------
    def autosa_once(self, run, spec, out, sizes=None):
        command = [PY, "run_autosa.py", str(self.work / "inputs" / run["input"]), "--domain", spec["domain"],
                   "--design-id", run["input"], "--vitis-include", str(self.include), "--out", str(out)]
        if spec["patch"]:
            command += ["--input-patch", str(EVAL / spec["patch"])]
        if sizes is not None:
            sizes_text = "{" + ";".join(f"kernel[]->{k}[{v}]" for k, v in sizes.items()) + "}"
            command += ["--autosa-arg=--no-pet-autodetect", f"--autosa-arg=--sa-sizes={sizes_text}", "--stdin=" + "n\\n" * 6]
        shutil.rmtree(out, ignore_errors=True)
        self.runner(command, out.parent / (out.name + ".runner.log"))
        return json.loads((out / "result.json").read_text())

    def autosa(self, run, spec, out):
        if not run.get("sweep"):
            result = self.autosa_once(run, spec, out / "autosa")
            self.publish(run, out / "autosa", ["run.log", "result.json"])
            return [{"tool": "AutoSA", "stage_reached": result["stage_reached"], "status": result["status"],
                     "error_head": result["error_head"]}]
        # Enumerate the space-time candidates, then answer each manual tuning stage per candidate.
        first = self.autosa_once(run, spec, out / "enumerate", sizes={})
        tuning = out / "enumerate/output/tuning.json"
        if not tuning.is_file():
            self.publish(run, out / "enumerate", ["run.log", "result.json"])
            return [{"tool": "AutoSA", "stage_reached": first["stage_reached"], "status": first["status"],
                     "error_head": first["error_head"], "detail": "no space-time candidate"}]
        count = json.loads(tuning.read_text())["space_time"]["n_kernel"]
        rows = []
        for st in range(count):
            sizes, cand = {"space_time": str(st)}, out / f"st{st}"
            for _ in range(6):
                result = self.autosa_once(run, spec, cand, sizes)
                stage_file = cand / "output/tuning.json"
                if result["status"] != "no_kernel_emitted" or not stage_file.is_file():
                    break
                stage, info = next(iter(json.loads(stage_file.read_text()).items()))
                if stage in sizes or stage not in AUTOSA_LIMIT:
                    break
                sizes[stage] = ",".join(str(max(d for d in range(1, min(b, AUTOSA_LIMIT[stage]) + 1) if b % d == 0))
                                        for b in info["tilable_loops"])
            row = {"run": f"{run['id']}/st{st}", "tool": "AutoSA", "stage_reached": result["stage_reached"],
                   "status": result["status"], "error_head": result["error_head"],
                   "tool_options": "sa-sizes " + ";".join(f"{k}[{v}]" for k, v in sizes.items())}
            src = cand / "output/src"
            if (src / "top_kernel.cpp").is_file():
                syntax = subprocess.run(["g++", "-std=c++14", "-fsyntax-only", "-w", f"-I{self.include}", f"-I{src}",
                                         str(src / "top_kernel.cpp")], capture_output=True, text=True)
                if syntax.returncode:
                    row.update(functional_check="COMPILE_ERROR",
                               detail=sanitize(next((l for l in syntax.stderr.splitlines() if "error" in l), ""), [(src, "<SRC>")]))
                else:
                    verdict = self.check(spec, src / "top_host.cpp", "top", extras=[src / "top_kernel.cpp"])
                    row.update(functional_check=verdict["verdict"], max_error=verdict["max_error"], detail=verdict["detail"])
                write_json(cand / "check.json", {k: row.get(k, "") for k in ["functional_check", "max_error", "detail"]})
            self.publish({"id": row["run"]}, cand, ["run.log", "result.json", "check.json"])
            rows.append(row)
        return rows

    def autosa_control(self, run, spec, out):
        """AutoSA's own matrix multiplication with its documented command, run against its golden check."""
        (out / "control").mkdir(parents=True)
        docker = ["docker", "run", "--rm", "--network", "none", "--user", f"{subprocess.check_output(['id', '-u'], text=True).strip()}:"
                  f"{subprocess.check_output(['id', '-g'], text=True).strip()}",
                  "--mount", f"type=bind,src={out / 'control'},dst=/control",
                  "--mount", f"type=bind,src={SCRIPTS},dst=/scripts,readonly",
                  "--entrypoint", "/bin/bash", AUTOSA_IMAGE, "/scripts/control_autosa.sh"]
        code = self.runner(docker, out / "control.log")
        src = out / "control/src"
        build = subprocess.run(["g++", "-std=c++14", "-O1", "-w", f"-I{self.include}", f"-I{src}", str(src / "kernel_host.cpp"),
                                str(src / "kernel_kernel.cpp"), "-o", str(out / "control.bin")], capture_output=True, text=True)
        verdict = ""
        if code == 0 and build.returncode == 0:
            verdict = next((l.strip() for l in subprocess.run([str(out / "control.bin")], capture_output=True, text=True,
                                                               timeout=600).stdout.splitlines() if "Passed" in l or "Failed" in l), "")
        log = (f"command: control_autosa.sh (upstream autosa_tests/mm, documented --sa-sizes, --host-serialize)\n"
               f"autosa exit_code: {code}\ng++ exit_code: {build.returncode}\nbuilt-in golden check: {verdict}\n")
        write_text(EVAL / "minimal/logs" / run["id"] / "run.log", sanitize(log))
        return [{"tool": "AutoSA", "level": "control", "domain": "gemm", "input": "autosa_tests/mm (upstream example)",
                 "stage_reached": "codegen" if code == 0 else "", "status": "codegen_requires_synthesis" if code == 0 else "failed",
                 "functional_check": "PASS" if verdict.startswith("Passed") else "FAIL", "detail": verdict}]

    # ---- outputs --------------------------------------------------------------------------------
    def publish(self, run, directory, names):
        """Redacted excerpts into the repository; full transcripts into the external archive."""
        target = EVAL / "minimal/logs" / run["id"]
        for name in names:
            if (directory / name).is_file():
                write_text(target / name, sanitize((directory / name).read_text(errors="replace")))
        if (directory / "full.log").is_file():
            write_text(self.work / "archive/minimal" / run["id"] / "full.log", sanitize((directory / "full.log").read_text(errors="replace")))

    def collect(self):
        rows = []
        for run in self.cases["runs"]:
            done = self.work / "runs" / run["id"].replace("/", "__") / "done.json"
            if done.is_file():
                for row in json.loads(done.read_text()):
                    # Records made before the timeout fix name the last passed stage; use the attempted one.
                    if row.get("status") == "timeout" and run["tool"] in ("scalehls", "hida", "streamhls"):
                        record = done.parent / run["tool"] / "result.json"
                        stages = json.loads(record.read_text()).get("stages", []) if record.is_file() else []
                        if stages:
                            row["stage_reached"] = stages[-1]["stage"]
                    rows.append(row)
        with (EVAL / "minimal/results.csv").open("w", newline="") as stream:
            writer = csv.DictWriter(stream, fieldnames=FIELDS, lineterminator="\n", extrasaction="ignore")
            writer.writeheader()
            writer.writerows({k: str(row.get(k, "")) for k in FIELDS} for row in rows)
        print(f"{len(rows)} rows in minimal/results.csv")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True, help="the --work directory used with minimal_inputs.py")
    parser.add_argument("--tools-work", type=Path, required=True, help="directory containing tools/ and build/ (see README)")
    parser.add_argument("--vitis-include", type=Path, required=True)
    parser.add_argument("--only", default="", help="regular expression selecting run ids")
    parser.add_argument("--jobs", type=int, default=4)
    parser.add_argument("--force", action="store_true", help="rerun runs that already have a record")
    parser.add_argument("--collect-only", action="store_true")
    args = parser.parse_args()
    study = Study(args)
    if not args.collect_only:
        selected = [r for r in study.cases["runs"] if re.search(args.only, r["id"])]
        # Generate the shared bases serially; parallel runs then only read them.
        for r in selected:
            if r["input"]:
                study.reference(study.cases["inputs"][r["input"]])
        with ThreadPoolExecutor(args.jobs) as pool:
            for future in [pool.submit(study.execute, r) for r in selected]:
                future.result()
    study.collect()


if __name__ == "__main__":
    main()
