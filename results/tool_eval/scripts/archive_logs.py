#!/usr/bin/env python3
"""Make a redacted log-only tarball outside the public repository."""
import argparse
import csv
import gzip
import json
import tarfile
from pathlib import Path

from common import EVAL, REPO, sanitize, sanitize_xml, sha256, write_json, write_text

PACKED_DIRS = ("minimal", "setup-tests")
PACKED_FILES = ("README.txt", "build-", "control-", "environment-probe.log")   # top-level files


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--minimal-work", type=Path, help="the --work directory of minimal_run.py, to include its transcripts")
    args = parser.parse_args()
    work, out = args.work.resolve(), args.out.resolve()
    if out == REPO or REPO in out.parents:
        raise SystemExit("The log tarball must be outside git")
    if out.exists():
        raise SystemExit("Refusing to overwrite an existing archive")
    archive = work / "archive"
    if args.minimal_work:
        source = args.minimal_work.resolve() / "archive/minimal"
        for log in sorted(source.rglob("*")):
            if log.is_file():
                write_text(archive / "minimal" / log.relative_to(source), log.read_text())
        # The csynth report of every minimal-study run that synthesized: tool output that passed the
        # functional check (synth_emitted.py) and the Vitis reference runs. synth_unchecked.py
        # already archived the reports of the outputs that failed the check.
        runs = args.minimal_work.resolve() / "runs"
        with (EVAL / "minimal/results.csv").open() as stream:
            for row in csv.DictReader(stream):
                if row["status"] != "synth_passed":
                    continue
                directory, tool = row["run"].rsplit("/", 1)
                if row["tool"] == "Vitis":
                    report = runs / f"{directory}__vitis/vitis/project_1/solution1/syn/report/csynth.xml"
                else:
                    report = runs / f"{directory}__{tool}" / tool.split("-")[0] / "csynth.xml"
                write_text(archive / "minimal" / row["run"] / "csynth.xml",
                           sanitize_xml(report.read_text(), [(report.parent, "<SYNTH_RUN>"), (runs, "<RUNS>")]))
    # Test runs of the control wrappers while they were written, and the regeneration check.
    for name in ["control-autosa-wrapper-test.log", "control-hida-wrapper-test.log", "control-wrapper-test.log",
                 "control-streamhls-array-only.log", "control-streamhls-directional.log", "control-streamhls-final.log",
                 "regeneration-check.log"]:
        if (work / name).is_file():
            write_text(archive / "setup-tests" / name, sanitize((work / name).read_text(), [(work, "<WORK>")]))
    write_text(archive / "README.txt", "Full redacted transcripts of the tool evaluation (results/tool_eval/).\n"
               "minimal/: the study behind the paper's table (minimal/cases.json, minimal/results.csv).\n"
               "  minimal/<run>/full.log: the complete transcript of one row of minimal/results.csv.\n"
               "  minimal/<run>/csynth.xml: its csynth report, when it synthesized.\n"
               "  minimal/<run>/synth-unchecked.log and synth-unchecked-csynth.xml: Vitis on output that failed the\n"
               "    functional check (minimal/synth_unchecked.csv).\n"
               "build-*.log: tool builds, including failed setup attempts as well as verified builds.\n"
               "control-*.log: each tool on its own upstream example (control-autosa.log is the row\n"
               "  autosa_control_mm/autosa of minimal/results.csv).\n"
               "setup-tests/: test runs of the control wrappers while they were written, and the regeneration check.\n"
               "environment-probe.log: the machine and tool environment.\n"
               "No compiler build trees, vendor headers, licenses, credentials, or raw private logs are included.\n")
    # Only the study behind the table and the setup are packed. The earlier primary, float, diagnostic,
    # historical and excluded-setup transcripts stay in the staging directory and in the previous
    # tarball (git history); their excerpts are committed under logs/.
    packed = []
    for path in sorted(archive.rglob("*")):
        parts = path.relative_to(archive).parts
        if path.is_file() and (parts[0] in PACKED_DIRS if len(parts) > 1 else parts[0].startswith(PACKED_FILES)):
            packed.append(path)
    members = []
    for path in packed:
        assert not path.is_symlink()
        content = path.read_text()
        assert sanitize(content) == content, f"Unredacted content: {path.name}"
        members.append({"path": str(path.relative_to(archive)), "sha256": sha256(path), "bytes": path.stat().st_size})
    write_json(archive / "manifest.json", members)
    out.parent.mkdir(parents=True, exist_ok=True)
    # A fixed gzip timestamp keeps the archive byte-identical across rebuilds.
    with gzip.GzipFile(out, "wb", mtime=0) as compressed, tarfile.open(fileobj=compressed, mode="w") as tar:
        for path in packed + [archive / "manifest.json"]:
            if path.is_file():
                info = tar.gettarinfo(str(path), arcname="tool-eval-logs/" + str(path.relative_to(archive)))
                info.uid = info.gid = 0
                info.uname = info.gname = ""
                info.mtime = 0
                with path.open("rb") as stream:
                    tar.addfile(info, stream)
    write_json(EVAL / "archive.json", {"filename": out.name, "sha256": sha256(out), "bytes": out.stat().st_size,
               "files": len(members) + 1, "contents": "Full redacted logs of the minimal-edit study behind the paper's table, plus tool builds, controls and setup tests",
               "distribution": "Committed next to archive.json (built outside the repository, then copied in); also intended for the Zenodo bundle. No public DOI assigned yet."})
    print(json.dumps({"filename": out.name, "sha256": sha256(out), "bytes": out.stat().st_size}))


if __name__ == "__main__":
    main()
