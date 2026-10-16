#!/usr/bin/env python3
"""Test removing unused host I/O includes; retain the exact patch and arithmetic.

These supplemental runs do not replace the untouched-input evaluation.
"""
import argparse
import shutil
import subprocess
import sys
from pathlib import Path

from common import EVAL, unified_patch, write_text


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True)
    parser.add_argument("--vitis-include", type=Path, required=True)
    args = parser.parse_args()
    for domain in ["gemm", "dnn", "llm"]:
        design_id = domain + "_0001"
        original = EVAL / "designs" / design_id
        adapted = args.work / "diagnostic-inputs" / design_id
        shutil.copytree(original, adapted)
        before = (original / "top.cpp").read_text()
        after = "".join(line for line in before.splitlines(True)
                        if line.strip() not in {"#include <iostream>", "#include <fstream>", "#include <cstdlib>"})
        write_text(adapted / "top.cpp", after)
        patch = EVAL / "patches/autosa" / (design_id + "-unused-host-headers.patch")
        write_text(patch, unified_patch(before, after))
        for label, flags in [("autosa-headers", []), ("autosa-diagnostic", ["--synthesis-define"])]:
            subprocess.run([sys.executable, str(EVAL / "scripts/run_autosa.py"), str(adapted),
                            "--domain", domain, "--design-id", design_id, *flags,
                            "--input-patch", str(patch), "--vitis-include", str(args.vitis_include),
                            "--out", str(args.work / "runs" / label / design_id)], check=True)


if __name__ == "__main__":
    main()
