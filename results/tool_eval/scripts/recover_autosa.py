#!/usr/bin/env python3
"""Import selected historical AutoSA artifacts without altering their source tree."""
import argparse
import hashlib
import subprocess
from pathlib import Path

from common import EVAL, sanitize, sanitize_xml, sha256, unified_patch, write_json, write_text


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("checkout", type=Path, help="old ForgeBench AutoSA checkout")
    parser.add_argument("--full-logs", type=Path, required=True, help="external archive staging directory")
    args = parser.parse_args()
    root = args.checkout.resolve()
    commit = subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=root, text=True).strip()
    records = []
    for domain, old_domain, name, adapter, artifacts in [
        ("gemm", "gemm", "diff_dims_p1", "top_gemm_new_ijk_bias.cpp",
         ["top_gemm_new_ijk_bias_kernel.cpp", "top_gemm_new_ijk_bias_kernel.h",
          "top_gemm_new_ijk_bias_host.cpp", "run_hls_autosa.tcl"]),
        ("dnn", "conv", "conv_A", "top_conv_test_new.c",
         ["top_conv_test_kernel.cpp", "top_conv_test_kernel.h", "top_conv_test_host.cpp",
          "kernel_kernel.cpp", "kernel_kernel.h", "kernel.h", "run_hls_autosa.tcl"]),
    ]:
        old_rel = f"{old_domain}/hls_files/{name}"
        source = root / old_rel
        target = EVAL / "historical/autosa" / domain / name
        replacements = [(root, "<HISTORICAL_ROOT>"), (root.resolve(), "<HISTORICAL_ROOT>"),
                        (source, "<DESIGN>"), (source.resolve(), "<DESIGN>")]
        pristine = subprocess.check_output(["git", "show", f"{commit}:{old_rel}/top.cpp"], cwd=root, text=True)
        write_text(target / "generated/top.cpp", pristine)
        cfg = f"{old_domain}/test_case_configs/{name}.json"
        config = subprocess.check_output(["git", "show", f"{commit}:{cfg}"], cwd=root, text=True)
        write_text(EVAL / "designs/historical" / (name + ".json"), sanitize(config, replacements))
        changed = (source / adapter).read_text()
        write_text(target / "adapted" / adapter, changed)
        patch = unified_patch(pristine, changed)
        patch_path = EVAL / "patches/autosa" / (name + "-historical-rewrite.patch")
        write_text(patch_path, patch)
        for artifact in artifacts:
            write_text(target / "artifacts" / artifact, sanitize((source / artifact).read_text(), replacements))
        log_files = ["vitis_hls.log"]
        if domain == "dnn":
            log_files += ["project_1/solution1/solution1.log", "project_2/solution1/solution1.log",
                          "project_2/solution1/.autopilot/db/a.g.ld.2.m1.bc.llvm-link.err.log"]
            cmd = source / "autosa.tmp/output/src/cmd"
            write_text(target / "recovered_command.txt", cmd.read_text())
        for rel in log_files:
            log = source / rel
            cleaned = sanitize(log.read_text(errors="replace"), replacements)
            dest_name = rel.replace("/", "__")
            prefix = (f"artifact: historical AutoSA workspace; {domain}/{name}/{rel}\n"
                      f"source_sha256_before_redaction: {sha256(log)}\n"
                      "invocation: vitis_hls -f run_hls_autosa.tcl (from transcript; historical environment not recovered)\n")
            if rel != "vitis_hls.log":
                prefix = (f"artifact: historical {domain}/{name}/{rel}\n"
                          f"source_sha256_before_redaction: {sha256(log)}\n"
                          "invocation: nested Vitis synthesis; exact historical invocation not recovered\n")
            write_text(args.full_logs / "historical/autosa" / domain / dest_name, prefix + cleaned)
            lines = cleaned.splitlines()
            trimmed = cleaned if len(lines) <= 65 else "\n".join(lines[:12] + ["[middle omitted]"] + lines[-50:]) + "\n"
            write_text(EVAL / "logs/autosa" / domain / ("historical-" + dest_name), prefix + trimmed)
        report = source / "project_1/solution1/syn/report/csynth.xml"
        if report.is_file():
            write_text(EVAL / "reports/historical/autosa" / name / "csynth.xml", sanitize_xml(report.read_text(), replacements))
        records.append({
            "domain": domain, "config_name": name, "source_checkout_commit": commit,
            "generated_source_git_path": f"{old_rel}/top.cpp",
            "generated_source_sha256": hashlib.sha256(pristine.encode()).hexdigest(),
            "adapter_file": adapter, "adapter_sha256": sha256(source / adapter),
            "patch": str(patch_path.relative_to(EVAL)),
            "eligible_for_minor_edit_rating": False,
            "note": "Recovered rewrite changes arithmetic precision and removes generated interfaces; input-to-output provenance is incomplete.",
        })
    write_json(EVAL / "historical/manifest.json", records)
    print("Recovered two historical cases, rewrites, synthesis report and sanitized logs")


if __name__ == "__main__":
    main()
