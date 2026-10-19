#!/usr/bin/env python3
"""Compare a tool's emitted C++ with ForgeBench's unmodified generated float design.

The comparison harness is generated from the design itself: the top-level signature is read
from the generated top.h and the outputs from the config's output_dram_names. Every input array
gets seeded uniform random values (optionally a per-array range), both implementations run on
the same inputs, and every output element is compared. The error metric is
|ref - got| / max(1, |ref|); a reference that produces NaN invalidates the trial instead of
passing silently. Prints one JSON line: verdict PASS / FAIL / INVALID / COMPILE_ERROR / TIMEOUT.
"""
import argparse
import json
import re
import subprocess
import tempfile
from pathlib import Path

from common import sanitize

TOLERANCE = 1e-4


def signature(top_h):
    """[(name, dims)] of the generated top function, in order."""
    text = Path(top_h).read_text()
    params = re.search(r"void\s+top\s*\(([^)]*)\)\s*;", text).group(1)
    result = []
    for param in params.split(","):
        match = re.fullmatch(r"\s*data_t\s+(\w+)((?:\[\d+\])+)\s*", param)
        if not match:
            raise ValueError(f"unexpected top parameter {param!r}")
        result.append((match.group(1), [int(d) for d in re.findall(r"\d+", match.group(2))]))
    return result


def harness(params, outputs, ranges, trials, seed):
    decl = ", ".join(f"float {n}{''.join(f'[{d}]' for d in dims)}" for n, dims in params)
    lines = ["#include <cmath>", "#include <cstdio>", "#include <cstdlib>",
             f"void top({decl});", f"void candidate({decl});"]
    for name, dims in params:
        shape = "".join(f"[{d}]" for d in dims)
        lines.append(f"static float {name}{shape};")
        if name in outputs:
            lines.append(f"static float {name}_ref{shape};")
    lines += ["static float uniform(float lo, float hi) { return lo + (hi - lo) * ((float)rand() / RAND_MAX); }",
              "int main() {", f"  srand({seed});", "  double worst = 0; int invalid = 0;",
              f"  for (int t = 0; t < {trials}; t++) {{"]
    for name, dims in params:
        flat = f"(&{name}{'[0]' * len(dims)})"
        size = 1
        for d in dims:
            size *= d
        if name in outputs:
            lines.append(f"    for (int k = 0; k < {size}; k++) {{ {flat}[k] = -12345.0f; (&{name}_ref{'[0]' * len(dims)})[k] = 12345.0f; }}")
        else:
            lo, hi = ranges.get(name, (-1.0, 1.0))
            lines.append(f"    for (int k = 0; k < {size}; k++) {flat}[k] = uniform({lo}f, {hi}f);")
    ref_args = ", ".join(n + "_ref" if n in outputs else n for n, _ in params)
    lines += [f"    top({ref_args});", f"    candidate({', '.join(n for n, _ in params)});"]
    for name, dims in params:
        if name not in outputs:
            continue
        size = 1
        for d in dims:
            size *= d
        got, ref = f"(&{name}{'[0]' * len(dims)})", f"(&{name}_ref{'[0]' * len(dims)})"
        lines += [f"    for (int k = 0; k < {size}; k++) {{",
                  f"      if (std::isnan({ref}[k])) {{ invalid = 1; continue; }}",
                  f"      double e = std::isnan({got}[k]) ? 1e30 : std::fabs({ref}[k] - {got}[k]) / std::fmax(1.0, std::fabs({ref}[k]));",
                  "      if (e > worst) worst = e;", "    }"]
    lines += ["  }", f'  printf("%s %.3g\\n", invalid ? "INVALID" : (worst < {TOLERANCE} ? "PASS" : "FAIL"), worst);',
              "  return 0;", "}"]
    return "\n".join(lines) + "\n"


def compile_and_run(reference, candidate, function, extras, call, config, ranges, include, trials=5, seed=1, timeout=300):
    """-> dict(verdict, max_error, detail)."""
    params = signature(Path(reference).parent / "top.h")
    outputs = set(json.loads(Path(config).read_text())["output_dram_names"])
    lib = (Path(include) / "../lnx64/lib/csim").resolve()
    fpo = (Path(include) / "../lnx64/tools/fpo_v7_1").resolve()
    with tempfile.TemporaryDirectory(prefix="tool-eval-check-") as tmp:
        tmp = Path(tmp)
        names = [n for n, _ in params]
        decl = ", ".join(f"float {n}{''.join(f'[{d}]' for d in dims)}" for n, dims in params)
        body = (call or "{fn}(" + ", ".join("{" + n + "}" for n in names) + ")").format(fn=function, **{n: n for n in names})
        (tmp / "cand.cpp").write_text(Path(candidate).read_text() + f"\nvoid candidate({decl}) {{ {body}; }}\n")
        (tmp / "main.cpp").write_text(harness(params, outputs, ranges, trials, seed))
        cxx = ["g++", "-std=c++14", "-O1", "-w", f"-I{include}"]
        steps = [cxx + [f"-I{Path(reference).parent}", "-c", str(reference), "-o", str(tmp / "ref.o")],
                 cxx + [f"-I{Path(candidate).parent}", "-c", str(tmp / "cand.cpp"), "-o", str(tmp / "cand0.o")]]
        objects = [str(tmp / "cand0.o")]
        for n, extra in enumerate(extras):
            steps.append(cxx + [f"-I{Path(extra).parent}", "-c", str(extra), "-o", str(tmp / f"extra{n}.o")])
            objects.append(str(tmp / f"extra{n}.o"))
        steps.append(["ld", "-r", *objects, "-o", str(tmp / "cand.o")])
        for step in steps:
            run = subprocess.run(step, capture_output=True, text=True)
            if run.returncode:
                error = next((l for l in run.stderr.splitlines() if "error" in l), run.stderr.strip()[-300:])
                return {"verdict": "COMPILE_ERROR", "max_error": "", "detail": sanitize(error, [(tmp, "<TMP>")])}
        # The candidate may define the same functions and globals as the reference.
        symbols = subprocess.run(["nm", "--defined-only", "-g", str(tmp / "cand.o")], capture_output=True, text=True).stdout
        renames = [f"{s[2]} cand_{s[2]}" for s in (l.split() for l in symbols.splitlines())
                   if len(s) == 3 and not s[2].startswith("_Z9candidate") and s[1] not in "VWu"]
        (tmp / "syms").write_text("\n".join(renames) + "\n")
        subprocess.run(["objcopy", f"--redefine-syms={tmp / 'syms'}", str(tmp / "cand.o")], check=True)
        link = ["g++", str(tmp / "main.cpp"), str(tmp / "ref.o"), str(tmp / "cand.o"), f"-L{lib}", f"-L{fpo}",
                f"-Wl,-rpath,{lib}", f"-Wl,-rpath,{fpo}", "-lhlsmc++-GCC46", "-lhlsm-GCC46",
                "-lIp_floating_point_v7_1_bitacc_cmodel", "-lgmp", "-lmpfr", "-o", str(tmp / "a.out")]
        run = subprocess.run(link, capture_output=True, text=True)
        if run.returncode:
            error = next((l for l in run.stderr.splitlines() if "error" in l or "undefined" in l), run.stderr.strip()[-300:])
            return {"verdict": "COMPILE_ERROR", "max_error": "", "detail": sanitize(error, [(tmp, "<TMP>")])}
        try:
            run = subprocess.run([str(tmp / "a.out")], capture_output=True, text=True, timeout=timeout)
        except subprocess.TimeoutExpired:
            return {"verdict": "TIMEOUT", "max_error": "", "detail": f"simulation did not finish in {timeout} s"}
        last = next((l for l in reversed(run.stdout.splitlines()) if l.split()[:1] in (["PASS"], ["FAIL"], ["INVALID"])), "")
        if not last:
            return {"verdict": "FAIL", "max_error": "", "detail": f"no result line; exit code {run.returncode}"}
        verdict, error = last.split()
        return {"verdict": verdict, "max_error": error, "detail": ""}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, required=True, help="unmodified generated float top.cpp")
    parser.add_argument("--config", type=Path, required=True, help="the config the reference was generated from")
    parser.add_argument("--candidate", type=Path, required=True)
    parser.add_argument("--function", required=True, help="the candidate's top function")
    parser.add_argument("--extra", type=Path, action="append", default=[], help="further candidate sources (AutoSA kernel)")
    parser.add_argument("--call", help="call expression when the tool changed the interface, e.g. '{fn}({a}, {a}, {b})'")
    parser.add_argument("--range", action="append", default=[], help="NAME=LO,HI input range (default -1,1)")
    parser.add_argument("--vitis-include", type=Path, required=True)
    args = parser.parse_args()
    ranges = {r.split("=")[0]: tuple(float(x) for x in r.split("=")[1].split(",")) for r in args.range}
    print(json.dumps(compile_and_run(args.reference, args.candidate, args.function, args.extra, args.call,
                                     args.config, ranges, args.vitis_include)))


if __name__ == "__main__":
    main()
