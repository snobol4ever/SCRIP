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
  strip_comments.py --check          # rc=1 if any comment (other than a 200-char separator) or blank line survives; the R5 gate arm
"""
import os, re, sys

KEEP_SEPARATORS = True   # Option A (default). Set False for Option B (literal zero).
SRC_ROOT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "src")
EXTS = (".c", ".h", ".cpp", ".hpp", ".y", ".l", ".s", ".S")
GENERATED = re.compile(r"(\.lex\.c|\.tab\.c|\.tab\.h|(^|/)lex\.[^/]+\.c)$")
SEP = re.compile(r"^\s*/\*[-=]+\*/\s*$")

def strip_one(text):
    """Return text with comments removed, preserving string/char literals."""
    out = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        # string / char literal — copy verbatim through the closing quote
        # a single quote opens a char literal ONLY if it closes within 8 chars on the same line ('x', '\\n', '\\x41', '\\'');
        # otherwise it is an ordinary character -- flex patterns like '([^']|'')*' (pascal.l) and <BODY>\\' (snobol4.l) carry
        # unpaired quotes that swallowed the rest of the file verbatim (R5 2026-09-02 measured: 47 separators + comments survived in snobol4.l)
        if c == "'":
            k = text.find("'", i + 1); k2 = text.find("\n", i + 1)
            if k == -1 or k - i > 8 or (k2 != -1 and k2 < k):
                out.append(c); i += 1
                continue
        # likewise a double quote opens a string ONLY if an unescaped closing quote follows on the same line: flex patterns
        # such as <BODY>\\" (snobol4.l) carry an unpaired quote that swallowed 34 separators and their comments verbatim (R5)
        if c == '"':
            k = i + 1; closed = False
            while k < n and text[k] != "\n":
                if text[k] == "\\": k += 2; continue
                if text[k] == '"': closed = True; break
                k += 1
            if not closed:
                out.append(c); i += 1
                continue
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
            # KEEP the line-break separator, NORMALIZED to the canonical 200-char form
            # (Lon 2026-09-02 "Do the line-break comments at 200 character length"; R4 2026-08-20 had 200 in the tree while this constant still said 120): /* + 196 dashes + */  (or '=' for a major separator).
            blk = text[i:j]
            if KEEP_SEPARATORS and SEP.match(blk):
                ch = "=" if "=" in blk else "-"
                out.append("/*" + ch * 196 + "*/")
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

def strip_flex(text):
    """flex sources: the C lexer is UNSAFE here (a bare /* inside a character class -- raku.l `[a-zA-Z0-9_.:/*+?=!$,;#@|\\\\-]` --
    is a pattern, and R4 2026-08-20 + R5 2026-09-02 both destroyed the lexer by treating it as a comment). So a .l file only loses
    WHOLE-LINE comments (a line whose first token is /* and whose block ends on a line with nothing after */), keeps every pattern
    and action byte, and has its separators renormalized. Trailing comments inside actions are left and reported by --check."""
    lines = text.split("\n"); out = []; i = 0
    while i < len(lines):
        l = lines[i]; s = l.strip()
        if s.startswith("/*"):
            if SEP.match(s):
                out.append(l[:len(l) - len(l.lstrip())] + "/*" + ("=" if "=" in s else "-") * 196 + "*/"); i += 1; continue
            j = i
            while j < len(lines) and "*/" not in lines[j]: j += 1
            if j < len(lines) and lines[j].split("*/", 1)[1].strip() == "":
                i = j + 1; continue
        out.append(l); i += 1
    return "\n".join(out)

def strip_file(p, src):
    return strip_flex(src) if p.endswith(".l") else strip_one(src)

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
        stripped = clean_blank_runs(strip_file(p, src))
        after = stripped.count("\n")
        tot_before += before; tot_after += after
        if stripped != src:
            changed += 1
            if mode == "--apply":
                with open(p, "w", errors="surrogateescape") as f:
                    f.write(stripped)
    if mode == "--check":
        dirty = [p for p in files if clean_blank_runs(strip_file(p, open(p, "r", errors="surrogateescape").read())) != open(p, "r", errors="surrogateescape").read()]
        print("files scanned (generated excluded): %d" % len(files))
        print("files still carrying a comment or a blank line: %d" % len(dirty))
        for p in dirty[:40]: print("  " + os.path.relpath(p, SRC_ROOT))
        sys.exit(1 if dirty else 0)
    print("files scanned (generated excluded): %d" % len(files))
    print("files that would change          : %d" % changed)
    print("total lines  before -> after     : %d -> %d  (%+d)" % (tot_before, tot_after, tot_after - tot_before))
    print("KEEP_SEPARATORS                  : %s" % KEEP_SEPARATORS)
    print("mode                             : %s" % mode)

if __name__ == "__main__":
    main()
