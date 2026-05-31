#!/usr/bin/env python3
"""descr8_rewrite.py FILE — rewrite raw DESCR-unique field access to GET_/SET_.

CONSERVATIVE: only transforms unambiguous single-field touches on a simple
identifier receiver.  Leaves compound initializers (DESCR_t x; x.v=..;x.slen=..)
and designated-initializer literals for hand edits — the scanner flags those.

Transforms (receiver = a C identifier, optionally with -> or . chains kept intact):
  WRITE   <recv>.FIELD = RHS ;     ->  SET_FIELD(<recv>, RHS);
  READ    <recv>.FIELD             ->  GET_FIELD(<recv>)

FIELD in {slen, ptr, arr, tbl, u, p}.  We do reads last and skip lines that
still contain a designated initializer ('.FIELD =' inside a {...} or a
multi-assignment compound) — those are reported, not rewritten.
"""
import re, sys

FIELDS = ["slen", "ptr", "arr", "tbl", "u", "p"]
GET = {f: "GET_" + f.upper() for f in FIELDS}
SET = {f: "SET_" + f.upper() for f in FIELDS}

# a "simple receiver" — identifier possibly with [] index, but NOT a (DESCR_t){...} literal
RECV = r"([A-Za-z_]\w*(?:\[[^\]\n]*\])?)"

path = sys.argv[1]
src = open(path).read()
lines = src.splitlines(keepends=True)
out = []
skipped = []

for i, line in enumerate(lines, 1):
    orig = line
    code = line
    # skip designated-initializer / compound-build lines entirely (hand-edit)
    if "{" in code and re.search(r"\.\s*(slen|ptr|arr|tbl|u|p|v|i|r|s)\s*=", code):
        if re.search(r"\.\s*\w+\s*=", code.split("{",1)[1] if "{" in code else ""):
            skipped.append((i, "designated-init", orig.rstrip()))
            out.append(orig); continue
    # multi-field compound on one line: 'x.v = ..; x.slen = ..' -> hand edit
    if code.count(".slen =") + code.count(".ptr =") + code.count(".tbl =") + \
       code.count(".arr =") + code.count(".u =") + code.count(".p =") > 1:
        skipped.append((i, "multi-assign", orig.rstrip()))
        out.append(orig); continue

    new = code
    for f in FIELDS:
        # WRITE: recv.FIELD = RHS   (not ==)   -> SET_FIELD(recv, RHS)
        wpat = re.compile(RECV + r"\.\s*" + f + r"\s*=(?!=)\s*(.*?);")
        def wrepl(m):
            return f"{SET[f]}({m.group(1)}, {m.group(2)});"
        new2 = wpat.sub(wrepl, new)
        new = new2
    for f in FIELDS:
        # READ: recv.FIELD  (field not followed by '=' already consumed; avoid '(' which would be a fn)
        rpat = re.compile(RECV + r"\.\s*" + f + r"\b(?!\s*\()")
        def rrepl(m):
            return f"{GET[f]}({m.group(1)})"
        new = rpat.sub(rrepl, new)
    out.append(new)

open(path, "w").write("".join(out))
print(f"rewrote {path}")
if skipped:
    print(f"  {len(skipped)} line(s) left for hand-edit:")
    for ln, why, txt in skipped:
        print(f"    {ln:5d} [{why}] {txt[:100]}")
