"""Small helpers for public, provenance-bearing evaluation artifacts."""
import hashlib
import difflib
import json
import re
import subprocess
import sys
from pathlib import Path

if sys.version_info < (3, 9):
    raise SystemExit("The evidence scripts require Python 3.9 or newer")

EVAL = Path(__file__).resolve().parents[1]
REPO = EVAL.parents[1]
AUTOSA_IMAGE = "whbldhwj/autosa@sha256:b9318c507d8e9876e363ad2cb3a7913ef743a95eb02517a538ea7f8ec0225b72"


def unified_patch(before, after):
    lines = difflib.unified_diff(before.splitlines(True), after.splitlines(True),
                                fromfile="a/top.cpp", tofile="b/top.cpp")
    return "".join(line if line.endswith("\n") else line + "\n\\ No newline at end of file\n"
                   for line in lines)


def sha256(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def sanitize(text, replacements=()):
    for old, new in sorted(replacements, key=lambda pair: -len(str(pair[0]))):
        text = text.replace(str(old), new)
    text = re.sub(r"For user '[^']*' on host '[^']*'", "For user '<USER>' on host '<HOST>'", text)
    # Preserve public URLs, but remove machine-specific absolute paths from any machine.
    text = re.sub(r"(?<![:/\w])/(?:export|usr|home|nethome|tools|opt|tmp|scratch|mnt|root|nfs)(?:/[^\s'\"<>:,;)]+)+",
                  lambda match: "<PATH>/" + match.group().rsplit("/", 1)[-1], text)
    text = re.sub(r"\b[\w.-]+\.(?:ece\.gatech\.edu|gatech\.edu)\b", "<HOST>", text)
    return text


def sanitize_xml(text, replacements=()):
    # Placeholders such as <PATH> would otherwise become unbalanced XML tags.
    text = sanitize(text, [(old, new.replace("<", "&lt;").replace(">", "&gt;")) for old, new in replacements])
    return re.sub(r"<(PATH|HOST|USER)>", r"&lt;\1&gt;", text)


def stop_container(name):
    """Stop a timed-out tool container; kill it if a loaded machine makes `docker stop` slow."""
    try:
        subprocess.run(["docker", "stop", "--time", "5", name], capture_output=True, timeout=60)
    except subprocess.TimeoutExpired:
        subprocess.run(["docker", "kill", name], capture_output=True, timeout=60)


def write_json(path, value):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + "\n")


def write_text(path, value):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(value)
