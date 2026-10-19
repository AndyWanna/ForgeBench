#!/usr/bin/env python3
"""Publish the HLS C++ each tool emitted in the minimal-edit study into generated_hls/.

Every row of minimal/results.csv that reached code generation gets one directory,
generated_hls/<tool>/<input>[__noext][/stN]/, holding what the tool wrote: emitted.cpp for
ScaleHLS, HIDA and StreamHLS; the kernel, its header and the host code for each AutoSA
space-time candidate. Files are redacted with common.sanitize. generated_hls/index.csv links
each directory to its results.csv row, so the verdict next to the code is the recorded one.
"""
import argparse
import csv
import shutil
from pathlib import Path

from common import EVAL, sanitize, sha256, write_text

OUT = EVAL / "generated_hls"
FIELDS = ["path", "run", "level", "domain", "input", "data_type", "patch", "tool", "tool_options", "stage_reached",
          "status", "functional_check", "max_error", "latency_cycles", "files", "sha256"]
AUTOSA_FILES = ["top_kernel.cpp", "top_kernel.h", "top_host.cpp"]


def sources(runs, row):
    """The emitted files of one results.csv row, and where they go under generated_hls/."""
    parts = row["run"].split("/")
    if row["tool"] == "AutoSA" and row["level"] == "control":
        src = runs / f"{parts[0]}__autosa/control/src"
        return Path("autosa/control_mm"), sorted(src.glob("kernel_*"))
    if row["tool"] == "AutoSA":
        src = runs / f"{parts[0]}__autosa/{parts[2]}/output/src"
        return Path("autosa", parts[0], parts[2]), [src / name for name in AUTOSA_FILES if (src / name).is_file()]
    tool = row["tool"].lower()
    suffix = "__noext" if parts[1] == "hida-noext" else ""
    emitted = runs / f"{parts[0]}__{parts[1]}" / tool / "emitted.cpp"
    return Path(tool, parts[0] + suffix), [emitted] if emitted.is_file() else []


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True, help="the --work directory of minimal_run.py")
    args = parser.parse_args()
    runs = args.work.resolve() / "runs"
    if OUT.exists():
        shutil.rmtree(OUT)
    index = []
    with (EVAL / "minimal/results.csv").open() as stream:
        for row in csv.DictReader(stream):
            if row["tool"] == "Vitis" or row["stage_reached"] not in {"codegen", "synth"}:
                continue
            target, files = sources(runs, row)
            if not files:   # codegen was attempted but wrote nothing
                continue
            for path in files:
                write_text(OUT / target / path.name, sanitize(path.read_text(errors="replace"), [(path.parent, "<SRC>")]))
            names = [path.name for path in files]
            index.append({**{key: row[key] for key in FIELDS if key in row}, "path": target.as_posix(),
                          "files": " ".join(names), "sha256": " ".join(sha256(OUT / target / name) for name in names)})
    with (OUT / "index.csv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=FIELDS, lineterminator="\n")
        writer.writeheader()
        writer.writerows(index)
    print(f"{len(index)} runs, {sum(len(row['files'].split()) for row in index)} files in {OUT.relative_to(EVAL)}/")


if __name__ == "__main__":
    main()
