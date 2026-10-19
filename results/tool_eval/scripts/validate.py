#!/usr/bin/env python3
"""Validate the tool-evaluation records: inputs, runs, logs, emitted code, the table, and privacy."""
import csv
import json
import re
import subprocess
import sys
import tarfile
from pathlib import Path

from common import EVAL, REPO, sha256
from minimal_run import FIELDS

TOOLS = {"ScaleHLS", "HIDA", "AutoSA", "StreamHLS"}
# reports/: the csynth report of the two whole designs that pass (Case Study 3, "Edits and quality of results")
REPORTS = {"reports/scalehls/gemm_0004_edited/csynth.rpt": "gemm_0004__edited/scalehls",
           "reports/hida/llm_0001_edited/csynth.rpt": "llm_0001__edited/hida-noext"}


def read_csv(path, fields=None):
    with (EVAL / path).open() as stream:
        reader = csv.DictReader(stream)
        if fields:
            assert reader.fieldnames == fields, path
        return list(reader)


def check_private(text, where):
    assert not re.search(r"/(?:usr/scratch|export/hdd/scratch|nethome)/", text), where
    assert not re.search(r"[\w.-]+\.ece\.gatech\.edu", text), where
    assert not re.search(r"(?:gh[pousr]_[A-Za-z0-9]{20,}|-----BEGIN [A-Z ]*PRIVATE KEY-----)", text), where


def main():
    # The base designs: the first design of each domain in manifest/designs/, committed as generated.
    manifest = json.loads((EVAL / "designs/manifest.json").read_text())
    for case in manifest["cases"]:
        design_id = case["design_id"]
        with (REPO / "manifest/designs" / (case["domain"] + ".csv")).open() as stream:
            assert any(row["design_id"] == design_id for row in csv.DictReader(stream)), design_id
        assert sha256(REPO / case["config_path"]) == case["config_sha256"]
        assert sha256(EVAL / "designs" / design_id / "config.json") == case["config_sha256"]
        for filename, digest in case["source_sha256"].items():
            assert sha256(EVAL / "designs" / design_id / filename) == digest

    # Every tool input is a ForgeBench config, a data type and an optional patch (tool_run_log/cases.json).
    # The whole-design inputs committed as source in designs/<id>[_float|_edited]/ must be exactly what was run;
    # their config.json is the config the source was generated from (data_type set to float for _float and _edited).
    cases = json.loads((EVAL / "tool_run_log/cases.json").read_text())
    inputs = cases["inputs"]
    for spec in inputs.values():
        assert (REPO / spec["config"]).is_file(), spec["config"]
        assert not spec["patch"] or (EVAL / spec["patch"]).is_file(), spec["patch"]
    folders = [path for path in (EVAL / "designs").iterdir() if path.is_dir()]
    for folder in folders:
        form = next((form for form in ("float", "edited") if folder.name.endswith("_" + form)), "fixed")
        spec = inputs[folder.name.removesuffix("_" + form) + "__" + form]
        assert sha256(folder / "top.cpp") == spec["sha256"], folder.name
        config = json.loads((REPO / spec["config"]).read_text())
        config["data_type"] = spec["data_type"]
        assert json.loads((folder / "config.json").read_text()) == config, folder.name

    # Every run: a known input, its short logs, and verdicts that agree with each other.
    results = read_csv("tool_run_log/results.csv", FIELDS)
    run_ids = {run["id"] for run in cases["runs"]}
    for row in results:
        assert row["run"] in run_ids or row["run"].rsplit("/", 1)[0] in run_ids, row["run"]
        logs = EVAL / "tool_run_log/logs" / row["run"]
        assert (logs / "run.log").is_file(), row["run"]
        if row["functional_check"] == "PASS" and row["tool"] in TOOLS:
            assert json.loads((logs / "check.json").read_text())["verdict"] == "PASS", row["run"]
        if row["stage_reached"] == "synth" and row["tool"] in TOOLS:
            assert row["functional_check"] == "PASS" and row["latency_cycles"], row["run"]

    # Vitis on the outputs that failed the functional check: one row per such output, read from generated_hls/.
    unchecked = read_csv("tool_run_log/synth_unchecked.csv")
    expected = {row["run"] for row in results if row["level"] != "control" and row["status"] == "codegen_requires_synthesis"}
    assert {row["run"] for row in unchecked} == expected
    for row in unchecked:
        assert sha256(EVAL / row["kernel"]) == row["kernel_sha256"], row["run"]
        assert row["synth"] in {"passed", "failed", "timeout"} and (row["synth"] == "passed") == bool(row["latency_cycles"])
        assert (EVAL / "tool_run_log/logs" / row["run"] / "synth-unchecked.log").is_file(), row["run"]

    # The tools' emitted HLS must match its index and results.csv, with no unlisted file.
    generated = EVAL / "generated_hls"
    recorded = {row["run"]: row for row in results}
    listed = {generated / "index.csv"}
    for row in read_csv("generated_hls/index.csv"):
        assert recorded[row["run"]]["stage_reached"] in {"codegen", "synth"}, row["run"]
        assert row["functional_check"] == recorded[row["run"]]["functional_check"], row["run"]
        for name, digest in zip(row["files"].split(), row["sha256"].split(), strict=True):
            assert sha256(generated / row["path"] / name) == digest, (row["path"], name)
            listed.add(generated / row["path"] / name)
    assert {path for path in generated.rglob("*") if path.is_file()} == listed

    for report, run in REPORTS.items():
        assert re.search(rf"\b{recorded[run]['latency_cycles']}\b", (EVAL / report).read_text()), report
    assert {str(path.relative_to(EVAL)) for path in (EVAL / "reports").rglob("*") if path.is_file()} == set(REPORTS)

    # The paper's table must be exactly what the records give.
    subprocess.run([sys.executable, str(EVAL / "scripts/paper_table.py"), "--check"], check=True, capture_output=True)

    # The full-log tarball matches archive.json, and nothing in it or in git is private.
    archive = json.loads((EVAL / "archive.json").read_text())
    tarball = EVAL / archive["filename"]
    assert sha256(tarball) == archive["sha256"] and tarball.stat().st_size == archive["bytes"], tarball
    with tarfile.open(tarball) as stream:
        members = [member for member in stream.getmembers() if member.isfile()]
        assert len(members) == archive["files"], tarball
        for member in members:
            check_private(stream.extractfile(member).read().decode(errors="replace"), member.name)
    size = 0
    for path in EVAL.rglob("*"):
        if not path.is_file() or "__pycache__" in path.parts or path == tarball:
            continue
        assert not path.is_symlink()
        if generated not in path.parents:
            size += path.stat().st_size
        assert path.stat().st_size < 1024 * 1024, path
        check_private(path.read_text(), path)
    assert size < 5 * 1024 * 1024, size
    print(f"PASS: {len(inputs)} inputs ({len(folders)} committed as source), {len(results)} runs, "
          f"{len(unchecked)} unchecked syntheses; {size:,} bytes outside generated_hls/ and the tarball")


if __name__ == "__main__":
    main()
