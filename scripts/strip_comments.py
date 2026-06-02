#!/usr/bin/env python3
"""strip_comments.py — DN-CLEAN-0 (GOAL-DENAME-EMITTER-RUNTIME).
Remove C/C++ comments from SCRIP source, string/char-literal aware.
  - Strips /* ... */ and // ... comments.
  - NEVER touches text inside "..." or '...' literals (incl. escapes).
  - Excludes the 12 generated flex/bison files (stripping breaks flex scaffolding).
  - SEPARATOR POLICY (Lon fork): KEEP_SEPARATORS=True keeps the sanctioned 200-char
    /*---*/ (minor) and /*===*/ (major) separators that RULES.md mandates; set False
    for literal "zero comments" (which also requires editing RULES.md).
Usage:
  strip_comments.py --dry-run        # report counts only, write nothing
  strip_comments.py --apply          # rewrite files in place
"""
import os, re, sys

KEEP_SEPARATORS = True   # Option A (default). Set False for Option B (literal zero).
SRC_ROOT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "src")
EXTS = (".c", ".h", ".cpp", ".y", ".l")
GENERATED = re.compile(r"(\.lex\.c|\.tab\.c|\.tab\.h|(^|/)lex\.[^/]+\.c)$")
SEP = re.compile(r"^\s*/\*[-=]+\*/\s*$")

def strip_one(text):
    """Return text with comments removed, preserving string/char literals."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        # string / char literal — copy verbatim through the closing quote
        if c == '"' or c == "'":
            q = c
            out.append(c); i += 1
            while i < n:
                d = text[i]
                out.append(d); i += 1
                if d == "\\" and i < n:        # escape: copy next char raw
                    out.append(text[i]); i += 1
                elif d == q:
                    break
            continue
        # block comment
        if c == "/" and i + 1 < n and text[i+1] == "*":
            j = text.find("*/", i + 2)
            j = (j + 2) if j != -1 else n
            # KEEP the line-break separator, NORMALIZED to the canonical 120-char form
            # (Lon 2026-06-02): /* + 116 dashes + */  (or '=' for a major/larger-file separator).
            blk = text[i:j]
            if KEEP_SEPARATORS and SEP.match(blk):
                ch = "=" if "=" in blk else "-"
                out.append("/*" + ch * 116 + "*/")
            else:
                out.append(" ")          # C semantics: a comment becomes one space (no token fusion)
            i = j
            continue
        # line comment
        if c == "/" and i + 1 < n and text[i+1] == "/":
            j = text.find("\n", i)
            i = j if j != -1 else n
            continue
        out.append(c); i += 1
    return "".join(out)

def clean_blank_runs(text):
    # collapse comment-removal-induced blank lines; RULES.md = zero blank lines.
    lines = [ln.rstrip() for ln in text.split("\n")]
    return "\n".join(ln for ln in lines if ln.strip() != "") + "\n"

def main():
    mode = sys.argv[1] if len(sys.argv) > 1 else "--dry-run"
    files = []
    for root, _, names in os.walk(SRC_ROOT):
        for nm in names:
            if nm.endswith(EXTS):
                p = os.path.join(root, nm)
                if GENERATED.search(p):
                    continue
                files.append(p)
    files.sort()
    tot_before = tot_after = changed = 0
    for p in files:
        with open(p, "r", errors="surrogateescape") as f:
            src = f.read()
        before = src.count("\n")
        stripped = clean_blank_runs(strip_one(src))
        after = stripped.count("\n")
        tot_before += before; tot_after += after
        if stripped != src:
            changed += 1
            if mode == "--apply":
                with open(p, "w", errors="surrogateescape") as f:
                    f.write(stripped)
    print("files scanned (generated excluded): %d" % len(files))
    print("files that would change          : %d" % changed)
    print("total lines  before -> after     : %d -> %d  (%+d)" % (tot_before, tot_after, tot_after - tot_before))
    print("KEEP_SEPARATORS                  : %s" % KEEP_SEPARATORS)
    print("mode                             : %s" % mode)

if __name__ == "__main__":
    main()
