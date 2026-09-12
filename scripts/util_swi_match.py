#!/usr/bin/env python3
"""util_swi_match.py — compare actual vs expected PASS/FAIL/EMPTY lines for one SWI test file.
Usage: util_swi_match.py <ref_file> <actual_text_file>
Prints matched count on stdout.

  --per-line   instead of the count, print one `<unit>\t<MATCH|MISS>` row per EXPECTED line, in ref order.

⛔ WHY --per-line EXISTS AND WHY THE COUNT WAS NOT ENOUGH (hq_C 2026-09-12, CEO-601 / COO-60).  The suite's
denominator is SUITE LINES -- one plunit unit each -- while its progress rows were written one per FILE, so
the coverage report read 20 rows against a 118-line board and could not say WHICH unit moved.  A count answers
"how many agreed"; a board row has to answer "did THIS one agree", and the two are not recoverable from each
other.  The default output is unchanged, because other callers grade on it.

SWI-5 (2026-05-28): EMPTY added as a third verdict — printed by pj_suite_verdict
when zero test bodies executed for a suite (TC =:= 0). Pre-SWI-5 the verdict was
binary (PASS for SF=:=0, FAIL otherwise); EMPTY now distinguishes "no tests ran"
from "all tests passed", killing the SF=:=0 false-positive PASS.
"""
import sys

_args = [a for a in sys.argv[1:] if not a.startswith("--")]
if len(_args) != 2:
    sys.stderr.write("usage: util_swi_match.py [--per-line] <ref_file> <actual_text_file>\n"); sys.exit(2)
ref_path, actual_path = _args

expected = open(ref_path).read().strip().splitlines()
actual_raw = open(actual_path).read().strip().splitlines()

# Deduplicate actual (first occurrence wins — eliminates double-run artefacts)
seen = set()
actual_set = set()
for line in actual_raw:
    line = line.strip()
    if line.startswith(('PASS ', 'FAIL ', 'EMPTY ')) and line not in seen:
        seen.add(line)
        actual_set.add(line)

per_line = "--per-line" in sys.argv[1:]
if per_line:
    for e in expected:
        unit = e.split(None, 1)[1].strip() if len(e.split(None, 1)) > 1 else e.strip()
        print(f"{unit}\t{'MATCH' if e in actual_set else 'MISS'}")
else:
    print(sum(1 for e in expected if e in actual_set))
