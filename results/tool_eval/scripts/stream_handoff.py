#!/usr/bin/env python3
"""Bring Polygeist's MLIR into the form StreamHLS's kernel pipeline expects from torch-mlir.

StreamHLS is upstream-driven from torch-mlir, whose bufferized IR has heap
buffers (memref.alloc) and a single function. Polygeist instead emits stack
buffers (memref.alloca) and keeps the definitions of helper functions it has
already inlined. ConvertMemRefsToFIFOs matches only memref.alloc, so no FIFOs,
dataflow graph or tasks are formed, and CreateDataflowFromAffine then crashes.
The kept helpers each get their own "node0", a duplicate symbol.

Both rewrites are lossless for a function-scope buffer and for functions with
no remaining call sites. No loop, operation, shape or C++ input is changed.
"""
import re
import sys
from pathlib import Path


def stack_to_heap(text):
    return text.replace("memref.alloca()", "memref.alloc()")


def keep_only(text, symbol):
    """Drop every top-level func.func definition except `symbol`; refuse if a dropped one is still called.

    Body-less declarations (external functions such as hls::sqrt) are kept.
    """
    lines, kept, dropped, keep = text.splitlines(True), [], [], True
    for line in lines:
        match = re.match(r"  func\.func (?:private )?@([\w$.]+)\(", line)
        if match and line.rstrip().endswith("{"):
            keep = match.group(1) == symbol
            if not keep:
                dropped.append(match.group(1))
        if keep:
            kept.append(line)
        elif line.startswith("  }"):
            keep = True
    result = "".join(kept)
    for name in dropped:
        if re.search(r"call @" + re.escape(name) + r"\b", result):
            raise ValueError(f"{name} is still called; refusing to drop it")
    return result


def adapt(text, symbol):
    return keep_only(stack_to_heap(text), symbol)


if __name__ == "__main__":
    source, dest, top = sys.argv[1:]
    Path(dest).write_text(adapt(Path(source).read_text(), top))
