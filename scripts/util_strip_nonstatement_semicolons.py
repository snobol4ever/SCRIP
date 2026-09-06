#!/usr/bin/env python3
"""Census (--fix to repair) Icon sources where our semicolon pass put a `;` where NO Icon accepts one.

MEASURED 2026-09-06 (hq_B), all four shapes, both directions, against icont/iconx and SCRIP:
    $define N 3;   record pt(x,y);   global g;   invocable all;
icont REJECTS every one ("`;`: invalid declaration"); SCRIP accepts them WITH or WITHOUT the semicolon.
So the semicolon our conversion added to preprocessor directives and declarations bought SCRIP nothing
and cost the file its gradability against its own oracle -- hq_I found arizona's prepro.icn and ilib.icn
scored as FAILs that no compiler change could ever cure, because the oracle cannot compile our copy.

TWO RULES, BOTH MEASURED THE SAME WAY. (1) A line whose FIRST token is `$<directive>`, `record`,
`global`, `link` or `invocable` and whose last non-comment character is `;`. (2) A line ending in `;`
whose next non-blank, non-comment line begins with `else` -- our pass put a semicolon after the THEN
branch, which CLOSES THE IF for icont ("`else`: invalid expression") while SCRIP tolerates it. Both
shapes were checked both ways: SCRIP runs identically with and without, icont refuses only ours.

⛔ SCOPE, DELIBERATELY NARROW. A statement-ending semicolon is
REQUIRED by SCRIP's frontend (RULES.md, ICON SEMICOLON-REQUIRED) and is never touched. Continuation
lines are left alone: a `record` spanning two lines has its `;` on a line this rule does not match, and
leaving it is the safe direction -- this tool exists to remove a semicolon no Icon accepts, not to
normalise style.

⭐ A PROGRAM THAT PRINTS ITS OWN SOURCE has its .ref moved by any edit here (arizona's fncs1 and io are
the known pair). This tool does not re-cut refs; it names every file it changed so the caller can grade
the suite that owns them.
"""
import os, re, sys
DECL = re.compile(r'^\s*(\$\s*[A-Za-z_]+|record|global|link|invocable)\b')
def strip_line(l):
    body, comment = l, ""
    h = body.find('#')
    if h >= 0:
        body, comment = body[:h], body[h:]
    stripped = body.rstrip()
    if not stripped.endswith(';'):
        return None
    return stripped[:-1].rstrip() + (" " + comment if comment else "")
def main():
    args = sys.argv[1:]
    fix = "--fix" in args
    roots = [a for a in args if not a.startswith("--")] or ["../corpus"]
    files = []
    for r in roots:
        if not os.path.isdir(r):
            print("⛔ REFUSES (rc=2): %s is not a directory -- cannot measure" % r); return 2
        for dp, _dn, fn in os.walk(r):
            files += [os.path.join(dp, n) for n in fn if n.endswith(".icn")]
    if not files:
        print("⛔ REFUSES (rc=2): no .icn files under %s -- a census that cannot see its population must not print 0" % ", ".join(roots)); return 2
    def next_code(ls, i):
        for j in range(i + 1, len(ls)):
            t = ls[j].strip()
            if t and not t.startswith("#"):
                return t
        return ""
    touched = lines_changed = 0
    for p in sorted(files):
        try:
            src = open(p, encoding="utf-8", errors="surrogateescape").read()
        except OSError as e:
            print("⛔ REFUSES (rc=2): cannot read %s (%s)" % (p, e)); return 2
        srclines = src.split("\n")
        out, n = [], 0
        for i, l in enumerate(srclines):
            if DECL.match(l):
                t = strip_line(l)
                if t is not None:
                    out.append(t); n += 1; continue
            if re.match(r'^\s*else\b', next_code(srclines, i)):
                t = strip_line(l)
                if t is not None:
                    out.append(t); n += 1; continue
            out.append(l)
        if n:
            touched += 1; lines_changed += n
            print("  %-6s %s (%d line(s))" % ("FIXED" if fix else "would", p, n))
            if fix:
                open(p, "w", encoding="utf-8", errors="surrogateescape").write("\n".join(out))
    print("icn files scanned=%d | %s=%d files, %d line(s)"
          % (len(files), "REPAIRED" if fix else "would repair", touched, lines_changed))
    if not fix and touched:
        print("   run with --fix to strip them; then re-grade the suites that own those files")
    return 0
if __name__ == "__main__":
    sys.exit(main())
