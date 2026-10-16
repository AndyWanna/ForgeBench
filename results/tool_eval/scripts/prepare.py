#!/usr/bin/env python3
"""Regenerate three explicitly selected sweep designs without editing sources."""
import argparse
import csv
import json
import subprocess
import sys
from pathlib import Path

from common import EVAL, REPO, sha256, write_json, write_text

sys.path.insert(0, str(REPO))
from forgebench.cli import generate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True, help="empty external scratch directory")
    args = parser.parse_args()
    work = args.work.resolve()
    work.mkdir(parents=True, exist_ok=True)
    selected = {"gemm_0001", "dnn_0001", "llm_0001"}
    with (REPO / "manifest/ml_sweep/id_map.csv").open() as stream:
        cases = [row for row in csv.DictReader(stream) if row["design_id"] in selected]
    assert len(cases) == 3
    records = []
    for row in cases:
        domain, design_id, name = row["domain"], row["design_id"], row["generator_name"]
        if (work / name).exists():
            raise SystemExit(f"Refusing to overwrite an existing generated design: {design_id}")
        config = REPO / "designs/configs/ml_sweep" / domain / (name + ".json")
        generated = Path(generate(str(config), str(work), domain=domain))
        dest = EVAL / "designs" / design_id
        dest.mkdir(parents=True, exist_ok=True)
        # Copy deterministic generator outputs only; randomized test data and
        # build trees stay in scratch. No replacement of types or operators.
        files = {}
        for source in sorted(generated.iterdir()):
            if source.suffix not in {".cpp", ".h", ".tcl"}:
                continue
            write_text(dest / source.name, source.read_text())
            files[source.name] = sha256(source)
        write_text(dest / "config.json", config.read_text())
        records.append({
            "design_id": design_id, "domain": domain,
            "config_path": str(config.relative_to(REPO)),
            "config_sha256": sha256(config), "source_sha256": files,
            "input_edits": "none",
        })
        print(f"Generated {design_id}; source sha256={files['top.cpp']}")
    write_json(EVAL / "designs/manifest.json", {
        "generator_commit": subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=REPO, text=True).strip(),
        "selection": "First manifest design in each domain; a three-design smoke test, not the historical evaluation cohort.",
        "cases": records,
    })


if __name__ == "__main__":
    main()
