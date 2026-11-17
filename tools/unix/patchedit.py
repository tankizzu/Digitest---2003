"""Edit a game source as the port patches it, then write the patch back.

    python3 tools/unix/patchedit.py show src/menus/title/menu.c          prints the patched source
    python3 tools/unix/patchedit.py apply src/menus/title/menu.c EDITS   EDITS: a Python file defining
                                                                         EDITS = [(old, new), ...]

Each old text must occur exactly once in the patched source. The patch goes to
patches/<path>.patch, against ../dw3_decomp/<path>.
"""
import difflib
import runpy
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DECOMP = ROOT.parent / "dw3_decomp"


def patched(rel):
    orig = DECOMP / rel
    patch = ROOT / "patches" / f"{rel}.patch"
    if not patch.exists():
        return orig.read_text(newline="")
    with tempfile.TemporaryDirectory() as d:
        out = Path(d) / "out"
        r = subprocess.run(["patch", "-s", "-o", str(out), str(orig), str(patch)], capture_output=True, text=True)
        if r.returncode != 0:
            sys.exit(r.stdout + r.stderr)
        return out.read_text(newline="")


def main():
    cmd, rel = sys.argv[1], sys.argv[2]
    text = patched(rel)
    if cmd == "show":
        sys.stdout.write(text)
        return
    for old, new in runpy.run_path(sys.argv[3])["EDITS"]:
        if text.count(old) != 1:
            sys.exit(f"{text.count(old)} matches for:\n{old}")
        text = text.replace(old, new)
    orig = (DECOMP / rel).read_text(newline="").splitlines(keepends=True)
    diff = difflib.unified_diff(orig, text.splitlines(keepends=True), f"a/{rel}", f"b/{rel}")
    out = ROOT / "patches" / f"{rel}.patch"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("".join(diff), newline="")
    print(f"wrote {out.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
