#!/usr/bin/env python3
"""Record evidence status for each existing paper feature mark, without guessing."""
import csv
import json

from common import EVAL

FEATURES = {
    "F1": ("HLS-tool-specific constructs", "gemm_0001", "top.cpp", "typedef ap_fixed<16,5> data_t;"),
    "F2": ("Dynamic loop nests", "llm_0001", "top.cpp", "heads_per_group"),
    "F3": ("Memory and function hierarchy", "gemm_0001", "top.cpp", "void load_"),
    "F4": ("LLM operations", "llm_0001", "top.cpp", "void grouped_multihead_attention_"),
    "F5": ("Fixed-point math intrinsics", "llm_0001", "top.cpp", "hls::exp"),
    "F6": ("Complete pragma support", "gemm_0001", "top.cpp", "#pragma HLS INTERFACE"),
}


def main():
    runs = json.loads((EVAL / "runs.json").read_text())
    rows = []
    for tool in ["ScaleHLS", "HIDA", "AutoSA", "StreamHLS"]:
        slug = tool.lower()
        for feature, (meaning, design, filename, token) in FEATURES.items():
            source = EVAL / "designs" / design / filename
            numbered = list(enumerate(source.read_text().splitlines(), 1))
            found = next(((i, line.strip()) for i, line in numbered if token.lower() in line.lower()), None)
            assert found, (feature, token)
            run = next(row for row in runs if row["tool"] == tool and row["design_id"] == design)
            observed = feature == "F1" and run["failure_category"] == "F1"
            rows.append({"tool": tool, "feature": feature, "meaning": meaning,
                         "paper_mark": "tick" if feature in ("F1", "F2") and not (tool == "AutoSA" and feature == "F2") else "cross",
                         "evidence_status": "failure_observed_in_vendor_header" if observed else "unverified_preempted_by_frontend",
                         "example_design": design, "source": f"designs/{design}/{filename}:{found[0]}",
                         "construct": found[1], "diagnostic": run["error_head"] if observed else "",
                         "blocking_log": f"logs/{slug}/{run['domain']}/run.log",
                         "scope": run.get("attribution", "") if observed else "Source occurrence is not an isolated feature probe; no pass/fail inferred"})
    with (EVAL / "feature_evidence.csv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    print("Recorded all 24 feature marks with explicit evidence status")


if __name__ == "__main__":
    main()
