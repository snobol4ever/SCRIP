#!/usr/bin/env python3
"""descr8_scan.py — inventory every raw DESCR_t field access in src/.

DESCR_t union/struct members: v, slen, i, r, s, ptr, p, arr, tbl, u.
We classify each .FIELD touch as WRITE (lhs of =), SENTINEL (cset/name discriminators),
or READ.  We exclude the macro layer itself (descr.h, sil_macros.h) since that is
where access is *defined*.  We also try to suppress obvious non-DESCR false positives
by requiring the receiver to be a plausible DESCR variable — but we err toward
INCLUDING (a human reviews); the .slen/.ptr/.arr/.tbl/.u fields are DESCR-unique and
near-zero false positive, while .v/.i/.r/.s need light heuristics.
"""
import os, re, sys, json

SRC = os.path.join(os.path.dirname(__file__), "..", "src")
SRC = os.path.abspath(SRC)
MACRO_LAYER = {"descr.h", "sil_macros.h"}

# DESCR-unique fields: extremely low false positive
UNIQUE = ("slen", "ptr", "arr", "tbl", "u", "p")
# Shared fields: need the receiver to look like a DESCR var, else huge FP
SHARED = ("v", "i", "r", "s")

# cset sentinel and name discriminators baked into slen
SENTINEL_PAT = re.compile(r"0xFFFFFFFF|slen\s*==\s*[01]\b|slen\s*=\s*[01]\b")

field_re = {f: re.compile(r"\.%s\b" % f) for f in UNIQUE + SHARED}
write_re = {f: re.compile(r"\.%s\s*=(?!=)" % f) for f in UNIQUE + SHARED}

def classify(line, field):
    if SENTINEL_PAT.search(line):
        return "SENTINEL"
    if write_re[field].search(line):
        return "WRITE"
    return "READ"

rows = []
counts = {"READ":0, "WRITE":0, "SENTINEL":0}
per_file = {}
for root, _, files in os.walk(SRC):
    for fn in files:
        if not fn.endswith((".c", ".h", ".cpp")):
            continue
        if fn in MACRO_LAYER:
            continue
        path = os.path.join(root, fn)
        rel = os.path.relpath(path, SRC)
        try:
            txt = open(path, encoding="utf-8", errors="replace").read().splitlines()
        except Exception:
            continue
        for ln, line in enumerate(txt, 1):
            s = line.split("//")[0]  # drop trailing line comments
            for f in UNIQUE:
                if field_re[f].search(s):
                    cls = classify(s, f)
                    rows.append((rel, ln, f, cls, line.strip()[:120]))
                    counts[cls]+=1
                    per_file[rel] = per_file.get(rel,0)+1
print("=== DESCR-UNIQUE field accesses (slen/ptr/arr/tbl/u/p) outside macro layer ===")
print("READ=%d  WRITE=%d  SENTINEL=%d  TOTAL=%d  across %d files"
      % (counts["READ"], counts["WRITE"], counts["SENTINEL"],
         sum(counts.values()), len(per_file)))
print()
print("=== per-file (sorted) ===")
for f,c in sorted(per_file.items(), key=lambda x:-x[1]):
    print("  %4d  %s" % (c, f))
# dump full row list to json for the funnel pass
json.dump(rows, open("/tmp/desc8/sites_unique.json","w"))
print("\nfull site list -> /tmp/desc8/sites_unique.json (%d rows)" % len(rows))
