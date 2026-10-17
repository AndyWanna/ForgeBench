#!/usr/bin/env python3
"""Record the evidence behind each of the paper's 24 feature marks (4 tools x F1-F6).

Evidence comes from outcomes.csv (rule-based failure categories, see collect_tables.py) and,
for F4, from whether the tool computed an LLM operator correctly in the minimal-edit study.
A mark without a direct observation stays `unverified`; nothing is inferred from a construct
merely appearing in the source. `agrees_with_paper` reads a tick as "the tool supports the
feature", the only reading under which the paper's AutoSA F2 cross is consistent.
"""
import csv

from common import EVAL

FEATURES = {
    "F1": "HLS-tool-specific constructs",
    "F2": "Dynamic loop nests",
    "F3": "Memory and function hierarchy",
    "F4": "LLM operations",
    "F5": "Fixed-point math intrinsics",
    "F6": "Complete pragma support",
}
LLM_OPERATORS = ("mha", "mha_rope", "rmsnorm", "softmax", "llm_0001")


def paper_mark(tool, feature):
    return "tick" if feature in ("F1", "F2") and not (tool == "AutoSA" and feature == "F2") else "cross"


def main():
    with (EVAL / "outcomes.csv").open() as stream:
        outcomes = list(csv.DictReader(stream))
    rows = []
    for tool in ["ScaleHLS", "HIDA", "AutoSA", "StreamHLS"]:
        mine = [r for r in outcomes if r["tool"] == tool]
        for feature, meaning in FEATURES.items():
            failures = [r for r in mine if r["failure_category"] == feature]
            status, example = "unverified", None
            if failures:
                status, example = "limitation_observed", failures[0]
            elif feature == "F4":
                passes = [r for r in mine if r["functional_check"] == "PASS" and r["design_id"].split("__")[0] in LLM_OPERATORS]
                if passes:
                    status, example = "operation_handled_after_edits", max(passes, key=lambda r: r["design_id"].startswith("mha_rope"))
            mark = paper_mark(tool, feature)
            agrees = ""
            if status == "limitation_observed":
                agrees = "yes" if mark == "cross" else "no"
            elif status == "operation_handled_after_edits":
                agrees = "yes" if mark == "tick" else "no"
            rows.append({"tool": tool, "feature": feature, "meaning": meaning, "paper_mark": mark, "evidence_status": status,
                         "agrees_with_paper": agrees, "example_design": example["design_id"] if example else "",
                         "example_run": example["run"] if example else "",
                         "diagnostic": example["error_head"] if example else "",
                         "observations": len(failures)})
    with (EVAL / "feature_evidence.csv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    print(f"Recorded {len(rows)} feature marks: "
          + ", ".join(f"{s}={sum(r['evidence_status'] == s for r in rows)}" for s in sorted({r['evidence_status'] for r in rows})))


if __name__ == "__main__":
    main()
