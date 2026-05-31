#!/usr/bin/env python3
"""util_swi_match.py — compare actual vs expected PASS/FAIL/EMPTY lines for one SWI test file.
Usage: util_swi_match.py <ref_file> <actual_text_file>
Prints matched count on stdout.

SWI-5 (2026-05-28): EMPTY added as a third verdict — printed by pj_suite_verdict
when zero test bodies executed for a suite (TC =:= 0). Pre-SWI-5 the verdict was
binary (PASS for SF=:=0, FAIL otherwise); EMPTY now distinguishes "no tests ran"
from "all tests passed", killing the SF=:=0 false-positive PASS.
"""
import sys

ref_path = sys.argv[1]
actual_path = sys.argv[2]

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

matched = sum(1 for e in expected if e in actual_set)
print(matched)
