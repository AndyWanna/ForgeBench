#!/usr/bin/env python3
"""Regenerate every input of the minimal-edit study from minimal/cases.json.

Each input is a ForgeBench config generated with an optional data-type override, plus an
optional committed patch against that generated source. The unmodified generations are kept
under <work>/base/<key>/ (they are the functional reference and the Vitis baseline); the edited
copies go to <work>/inputs/<input id>/. With --record, the resulting top.cpp hashes are written
back to cases.json; otherwise they must match the recorded ones.
"""
import argparse
import json
import shutil
import subprocess
import sys
from pathlib import Path

from common import EVAL, REPO, sha256

sys.path.insert(0, str(REPO))
from forgebench.cli import generate  # noqa: E402

CASES = EVAL / "minimal/cases.json"


def base_key(spec):
    # The parent directory disambiguates sweep configs, which are all called config.json.
    config = Path(spec["config"])
    return (config.parent.name + "__" + config.stem + "__"
            + spec["data_type"].replace("<", "_").replace(">", "_").replace(",", "_"))


def generate_base(spec, work):
    """Generate (once) the unmodified design for this config and data type; return its directory."""
    target = work / "base" / base_key(spec)
    if not (target / "top.cpp").is_file():
        tmp = work / "base" / (base_key(spec) + ".tmp")
        shutil.rmtree(tmp, ignore_errors=True)
        generated = Path(generate(str(REPO / spec["config"]), str(tmp), domain=spec["domain"], data_type=spec["data_type"]))
        shutil.rmtree(target, ignore_errors=True)
        shutil.move(str(generated), str(target))
        shutil.rmtree(tmp, ignore_errors=True)
    return target


def build(input_id, spec, work):
    base = generate_base(spec, work)
    dest = work / "inputs" / input_id
    shutil.rmtree(dest, ignore_errors=True)
    shutil.copytree(base, dest)
    if spec.get("patch"):
        subprocess.run(["git", "apply", str(EVAL / spec["patch"])], cwd=dest, check=True)
    return dest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True, help="external scratch directory")
    parser.add_argument("--record", action="store_true", help="write the top.cpp hashes into cases.json")
    args = parser.parse_args()
    cases = json.loads(CASES.read_text())
    work = args.work.resolve()
    mismatches = []
    for input_id, spec in cases["inputs"].items():
        digest = sha256(build(input_id, spec, work) / "top.cpp")
        if args.record:
            spec["sha256"] = digest
        elif spec.get("sha256") != digest:
            mismatches.append(input_id)
    if args.record:
        CASES.write_text(json.dumps(cases, indent=1) + "\n")
    if mismatches:
        raise SystemExit("top.cpp hash differs from cases.json for: " + ", ".join(mismatches))
    print(f"Built {len(cases['inputs'])} inputs under {work / 'inputs'}")


if __name__ == "__main__":
    main()
