#!/usr/bin/env python3
"""Bridge LLVM 18.0 -> 18.1 DLTI integer payload widths, preserving their values.

Polygeist's pinned LLVM emits data-layout vectors as i32; StreamHLS's pinned
LLVM requires i64. Only those metadata payload types (and stack-alignment's
payload type) change. No function, operation, shape, layout value, or C++ input
is rewritten. This adapter is part of the explicitly recorded fresh setup.
"""
import re
import sys
from pathlib import Path


def normalize(text):
    text = re.sub(r"(#dlti\.dl_entry<[^,\n]+, dense<[^>]+> : vector<\d+x)i32(>>)",
                  r"\g<1>i64\2", text)
    return re.sub(r'(#dlti\.dl_entry<"dlti\.stack_alignment", \d+ : )i32(>)', r"\g<1>i64\2", text)


if __name__ == "__main__":
    source, dest = map(Path, sys.argv[1:])
    dest.write_text(normalize(source.read_text()))
