#!/usr/bin/env python3
"""Validate committed evidence links, hashes, coverage, and privacy constraints."""
import csv
import json
import re
import shutil
import subprocess
import tarfile
import tempfile
import xml.etree.ElementTree as ET
from pathlib import Path

from common import EVAL, REPO, sha256
from minimal_run import FIELDS


def read_csv(filename, fields):
    with (EVAL / filename).open() as stream:
        reader = csv.DictReader(stream)
        assert reader.fieldnames == fields, filename
        return list(reader)


def main():
    manifest = json.loads((EVAL / "designs/manifest.json").read_text())
    cases = {row["design_id"]: row for row in manifest["cases"]}
    for design_id, case in cases.items():
        with (REPO / "manifest/designs" / (case["domain"] + ".csv")).open() as stream:
            assert any(row["design_id"] == design_id for row in csv.DictReader(stream)), design_id
        assert sha256(REPO / case["config_path"]) == case["config_sha256"]
        assert sha256(EVAL / "designs" / design_id / "config.json") == case["config_sha256"]
        for filename, digest in case["source_sha256"].items():
            assert sha256(EVAL / "designs" / design_id / filename) == digest
    summary = read_csv("summary.csv", ["tool", "domain", "designs_tried", "stage_reached", "rating", "notes"])
    outcomes = read_csv("outcomes.csv", ["design_id", "domain", "tool", "stage_reached", "failure_category", "error_head",
                                         "run", "patch", "functional_check"])
    results = read_csv("minimal/results.csv", FIELDS)
    by_run = {row["run"]: row for row in results}
    tools = {"ScaleHLS": "scalehls", "HIDA": "hida", "AutoSA": "autosa", "StreamHLS": "streamhls"}
    expected = {(tool, domain) for tool in tools for domain in ["gemm", "dnn", "llm"]}
    assert len(summary) == 12 and {(row["tool"], row["domain"]) for row in summary} == expected
    for row in summary:
        # The primary unmodified run of each cell keeps its log in logs/<tool>/<domain>/.
        logdir = EVAL / "logs" / tools[row["tool"]] / row["domain"]
        result = json.loads((logdir / "result.json").read_text())
        assert result["source_sha256"] == cases[row["domain"] + "_0001"]["source_sha256"]["top.cpp"]
        assert "command" in (logdir / "run.log").read_text()
        assert row["stage_reached"] in {"parse", "transform", "codegen", "synth"}
        assert row["rating"] in {"full", "half", "none"}
        if row["rating"] != "none":
            # A rating above none needs a synthesized, functionally correct whole-design run, and
            # `half` exactly when that run used a committed patch.
            backing = [r for r in results if r["tool"] == row["tool"] and r["domain"] == row["domain"] and r["level"] == "design"
                       and r["stage_reached"] == "synth" and r["functional_check"] == "PASS"]
            assert backing and any(bool(r["patch"]) == (row["rating"] == "half") for r in backing), row
    for row in outcomes:
        assert row["run"] in by_run, row["run"]
        assert row["failure_category"] in {"", "F1", "F2", "F3", "F4", "F5", "F6"}
    minimal = json.loads((EVAL / "minimal/cases.json").read_text())
    for input_id, spec in minimal["inputs"].items():
        assert (REPO / spec["config"]).is_file(), spec["config"]
        if spec["patch"]:
            assert (EVAL / spec["patch"]).is_file(), spec["patch"]
        # Inputs committed as source (gemm_0004's three variants, llm_0001's edited one) must be exactly what was run.
        if input_id.startswith("gemm_0004__") or input_id == "llm_0001__edited":
            variant = input_id.split("__")[0] + {"fixed": "", "float": "_float", "edited": "_edited"}[input_id.split("__")[1]]
            assert sha256(EVAL / "designs" / variant / "top.cpp") == spec["sha256"], variant
            assert sha256(EVAL / "designs" / variant / "config.json") == sha256(REPO / spec["config"]), variant
    run_ids = {run["id"] for run in minimal["runs"]}
    for row in results:
        assert row["run"] in run_ids or row["run"].rsplit("/", 1)[0] in run_ids, row["run"]
        logs = EVAL / "minimal/logs" / row["run"]
        assert (logs / "run.log").is_file(), row["run"]
        if row["functional_check"] == "PASS" and row["tool"] in tools:
            assert json.loads((logs / "check.json").read_text())["verdict"] == "PASS", row["run"]
        if row["stage_reached"] == "synth" and row["tool"] in tools:
            assert row["functional_check"] == "PASS" and row["latency_cycles"], row["run"]
    for patch in (EVAL / "patches").glob("*/*.patch"):
        if patch.parent.name in tools.values() and patch.name in {p.name for p in (EVAL / "minimal/patches").iterdir()}:
            assert patch.read_bytes() == (EVAL / "minimal/patches" / patch.name).read_bytes(), patch
    for design_id in cases:
        report = EVAL / "reports/vitis" / design_id / "csynth.xml"
        tree = ET.parse(report)
        assert tree.findtext("UserAssignments/TopModelName") == "top"
        assert tree.findtext("UserAssignments/Part") == "xczu9eg-ffvb1156-2-e"
    with (EVAL / "feature_evidence.csv").open() as stream:
        features = list(csv.DictReader(stream))
    assert len(features) == 24 and len({(r["tool"], r["feature"]) for r in features}) == 24
    for row in features:
        assert row["evidence_status"] in {"unverified", "limitation_observed", "operation_handled_after_edits"}
        if row["evidence_status"] != "unverified":
            assert row["example_run"] in by_run, row["example_run"]
    # Apply each claimed input patch in an isolated directory and compare the
    # actual resulting bytes, not just whether the patch's syntax is valid.
    historical = json.loads((EVAL / "historical/manifest.json").read_text())
    for record in historical:
        root = EVAL / "historical/autosa" / record["domain"] / record["config_name"]
        with tempfile.TemporaryDirectory(prefix="tool-eval-patch-") as temporary:
            shutil.copy2(root / "generated/top.cpp", Path(temporary) / "top.cpp")
            subprocess.run(["git", "apply", str(EVAL / record["patch"])], cwd=temporary, check=True)
            assert sha256(Path(temporary) / "top.cpp") == sha256(root / "adapted" / record["adapter_file"])
    for design_id in cases:
        original = EVAL / "designs" / design_id / "top.cpp"
        patch = EVAL / "patches/autosa" / (design_id + "-unused-host-headers.patch")
        with tempfile.TemporaryDirectory(prefix="tool-eval-patch-") as temporary:
            shutil.copy2(original, Path(temporary) / "top.cpp")
            subprocess.run(["git", "apply", str(patch)], cwd=temporary, check=True)
            expected_source = "".join(line for line in original.read_text().splitlines(True)
                                      if line.strip() not in {"#include <iostream>", "#include <fstream>", "#include <cstdlib>"})
            assert (Path(temporary) / "top.cpp").read_text() == expected_source
    float_manifest = json.loads((EVAL / "designs/float_manifest.json").read_text())
    float_cases = {case["design_id"]: case for case in float_manifest["cases"]}
    for design_id, case in float_cases.items():
        original = EVAL / "designs" / case["parent_design_id"]
        changed = EVAL / "designs" / design_id
        config = json.loads((original / "config.json").read_text())
        config["data_type"] = "float"
        assert json.loads((changed / "config.json").read_text()) == config
        assert sha256(changed / "config.json") == case["config_sha256"]
        for filename, digest in case["source_sha256"].items():
            assert sha256(changed / filename) == digest
        with tempfile.TemporaryDirectory(prefix="tool-eval-float-patch-") as temporary:
            shutil.copy2(original / "top.cpp", Path(temporary) / "top.cpp")
            subprocess.run(["git", "apply", str(EVAL / "patches/float" / (design_id + ".patch"))], cwd=temporary, check=True)
            assert sha256(Path(temporary) / "top.cpp") == case["source_sha256"]["top.cpp"]
        assert ET.parse(EVAL / "reports/vitis" / design_id / "csynth.xml").findtext("UserAssignments/TopModelName") == "top"
    float_summary = read_csv("float_summary.csv", ["tool", "domain", "designs_tried", "stage_reached", "rating", "notes"])
    float_outcomes = read_csv("float_outcomes.csv", ["design_id", "domain", "tool", "stage_reached", "failure_category", "error_head"])
    assert len(float_summary) == 12 and {(r["tool"], r["domain"]) for r in float_summary} == expected
    assert len(float_outcomes) == 12
    for row in float_summary:
        result = json.loads((EVAL / "logs" / tools[row["tool"]] / row["domain"] / "float-result.json").read_text())
        assert result["source_sha256"] == float_cases[result["design_id"]]["source_sha256"]["top.cpp"]
        assert row["rating"] in {"full", "half", "none"} and row["rating"] == result["rating"]
        assert row["stage_reached"] == result["stage_reached"]
        if row["rating"] in {"full", "half"}:
            assert result["stage_reached"] == "synth"
            assert (EVAL / "reports" / tools[row["tool"]] / result["design_id"] / "csynth.xml").is_file()
    def check_private(text, where):
        assert not re.search(r"/(?:usr/scratch|export/hdd/scratch|nethome)/", text), where
        assert not re.search(r"[\w.-]+\.ece\.gatech\.edu", text), where
        assert not re.search(r"(?:gh[pousr]_[A-Za-z0-9]{20,}|-----BEGIN [A-Z ]*PRIVATE KEY-----)", text), where

    # The full-log tarball is committed alongside the evidence but kept out of the size budget.
    archive = json.loads((EVAL / "archive.json").read_text())
    tarball = EVAL / archive["filename"]
    assert sha256(tarball) == archive["sha256"] and tarball.stat().st_size == archive["bytes"], tarball
    with tarfile.open(tarball) as stream:
        members = [member for member in stream.getmembers() if member.isfile()]
        assert len(members) == archive["files"], tarball
        for member in members:
            check_private(stream.extractfile(member).read().decode(errors="replace"), member.name)
    # The tools' emitted HLS is kept out of the size budget too, but must match its index and results.csv.
    generated = EVAL / "generated_hls"
    with (generated / "index.csv").open() as stream:
        index = list(csv.DictReader(stream))
    recorded = {row["run"]: row for row in results}
    listed = {generated / "index.csv"}
    for row in index:
        assert recorded[row["run"]]["stage_reached"] in {"codegen", "synth"}, row["run"]
        assert row["functional_check"] == recorded[row["run"]]["functional_check"], row["run"]
        for name, digest in zip(row["files"].split(), row["sha256"].split(), strict=True):
            assert sha256(generated / row["path"] / name) == digest, (row["path"], name)
            listed.add(generated / row["path"] / name)
    assert {path for path in generated.rglob("*") if path.is_file()} == listed
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
    print(f"PASS: {len(summary)} summary cells, {len(outcomes)} outcomes, {len(results)} minimal-study rows, "
          f"{len(minimal['inputs'])} inputs; {size:,} public bytes")


if __name__ == "__main__":
    main()
