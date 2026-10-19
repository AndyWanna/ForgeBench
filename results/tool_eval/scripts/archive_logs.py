#!/usr/bin/env python3
"""Make a redacted log-only tarball outside the public repository."""
import argparse
import csv
import gzip
import json
import tarfile
from pathlib import Path

from common import EVAL, REPO, sanitize, sanitize_xml, sha256, write_json, write_text


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
    # Keep excluded setup attempts explicitly separate from benchmark results.
    for name in ["scalehls-empty-selector", "scalehls-missing-resources"]:
        for log in (work / "runs" / name).glob("*/*.log"):
            write_text(archive / "excluded-setup" / name / log.parent.name / log.name,
                       sanitize(log.read_text(), [(work, "<WORK>"), ("/work", "<WORK>"),
                                                 ("/design", "<DESIGN>"), ("/vitis/include", "<VITIS_INCLUDE>")]))
    write_text(archive / "README.txt", "Full redacted transcripts for the fresh tool-evaluation evidence package.\n"
               "fresh/: unchanged benchmark runs and original Vitis baselines.\n"
               "historical/: recovered older AutoSA transcripts, not fresh measurements.\n"
               "diagnostic/: explicitly edited/header-mode experiments.\n"
               "excluded-setup/: missing-header or wrong-function-selector attempts; NOT benchmark outcomes.\n"
               "minimal/: the minimal-edit study (whole designs and single operators; see minimal/cases.json).\n"
               "  minimal/<run>/full.log: the complete transcript of one row of minimal/results.csv.\n"
               "  minimal/<run>/csynth.xml: its csynth report, when it synthesized.\n"
               "  minimal/<run>/synth-unchecked.log and synth-unchecked-csynth.xml: Vitis on output that failed the\n"
               "    functional check (minimal/synth_unchecked.csv).\n"
               "setup-tests/: test runs of the control wrappers while they were written, and the regeneration check.\n"
               "build*.log: includes failed setup attempts as well as verified builds.\n"
               "No compiler build trees, vendor headers, licenses, credentials, or raw private logs are included.\n")
    members = []
    for path in sorted(archive.rglob("*")):
        if not path.is_file() or path == archive / "manifest.json":
            continue  # A manifest left by an earlier run must not list itself.
        assert not path.is_symlink()
        content = path.read_text()
        assert sanitize(content) == content, f"Unredacted content: {path.name}"
        members.append({"path": str(path.relative_to(archive)), "sha256": sha256(path), "bytes": path.stat().st_size})
    write_json(archive / "manifest.json", members)
    out.parent.mkdir(parents=True, exist_ok=True)
    # A fixed gzip timestamp keeps the archive byte-identical across rebuilds.
    with gzip.GzipFile(out, "wb", mtime=0) as compressed, tarfile.open(fileobj=compressed, mode="w") as tar:
        for path in sorted(archive.rglob("*")):
            if path.is_file():
                info = tar.gettarinfo(str(path), arcname="tool-eval-logs/" + str(path.relative_to(archive)))
                info.uid = info.gid = 0
                info.uname = info.gname = ""
                info.mtime = 0
                with path.open("rb") as stream:
                    tar.addfile(info, stream)
    write_json(EVAL / "archive.json", {"filename": out.name, "sha256": sha256(out), "bytes": out.stat().st_size,
               "files": len(members) + 1, "contents": "Full redacted run, control, diagnostic, historical and build logs",
               "distribution": "Committed next to archive.json (built outside the repository, then copied in); also intended for the Zenodo bundle. No public DOI assigned yet."})
    print(json.dumps({"filename": out.name, "sha256": sha256(out), "bytes": out.stat().st_size}))


if __name__ == "__main__":
    main()
