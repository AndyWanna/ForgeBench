#!/usr/bin/env python3
"""Regenerate the same configurations in float; never rewrite fixed-point inputs."""
import argparse
import json
import subprocess
import sys
from pathlib import Path

from common import EVAL, REPO, sha256, unified_patch, write_json, write_text

sys.path.insert(0, str(REPO))
from forgebench.cli import generate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True)
    args = parser.parse_args()
    work = args.work.resolve()
    work.mkdir(parents=True, exist_ok=False)
    cases = []
    for domain in ["gemm", "dnn", "llm"]:
        parent_id = domain + "_0001"
        design_id = parent_id + "_float"
        original = EVAL / "designs" / parent_id
        config = json.loads((original / "config.json").read_text())
        assert config["data_type"] == "ap_fixed<16,5>"
        config["data_type"] = "float"
        dest = EVAL / "designs" / design_id
        write_json(dest / "config.json", config)
        generated = Path(generate(str(dest / "config.json"), str(work / design_id), domain=domain))
        hashes = {}
        for source in sorted(generated.iterdir()):
            if source.suffix in {".cpp", ".h", ".tcl"}:
                write_text(dest / source.name, source.read_text())
                hashes[source.name] = sha256(source)
        # Generated identifier/math changes are all visible in this patch.
        write_text(EVAL / "patches/float" / (design_id + ".patch"),
                   unified_patch((original / "top.cpp").read_text(), (dest / "top.cpp").read_text()))
        cases.append({"design_id": design_id, "domain": domain, "parent_design_id": parent_id,
                      "configuration_change": {"data_type": ["ap_fixed<16,5>", "float"]},
                      "config_sha256": sha256(dest / "config.json"), "source_sha256": hashes,
                      "input_edits": "none after generation", "precision_equivalence_claimed": False,
                      "generated_directory": str(generated.relative_to(work))})
        print(f"Generated {design_id}; source sha256={hashes['top.cpp']}")
    write_json(EVAL / "designs/float_manifest.json", {
        "generator_commit": subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=REPO, text=True).strip(),
        "selection": "User-requested float regeneration of the same three configurations; separate from original fixed-point cohort",
        "cases": cases})


if __name__ == "__main__":
    main()
