#!/usr/bin/env python3
"""apply_score_grid.py <SCORE.md> <grid.txt> -- splice util_build_score_md.py's stdout into SCORE.md's STANDARDIZED DISPLAY,
replacing from the '| Language |' header through the '_tree: ..._' stamp line. Refuses if either boundary is missing or if the
new grid lacks a stamp line (a grid without provenance is the defect the generator exists to prevent)."""
import sys, re
score, grid = sys.argv[1], sys.argv[2]
s = open(score, encoding='utf-8').read(); g = open(grid, encoding='utf-8').read().strip('\n') + '\n'
assert g.startswith('| Language |') and re.search(r'^_tree: SCRIP=\S+ corpus=\S+ \.github=\S+  generated \S+_$', g, re.M), "new grid malformed or unstamped"
# ⛔⭐⭐ COLUMN-COUNT GUARD (hq_T 2026-09-03, mandate 2). A SPLICE IS A DELETION OF EVERYTHING IT DOES NOT
# REPLACE, and this one is addressed by POSITION (header line -> stamp line), so it cannot notice that the
# grid it is pasting is NARROWER than the grid it is pasting over. Measured, not reasoned: the generator's
# header is a 4-column literal (util_build_score_md.py:206) while the live board is 6 columns -- it grew
# `Vendor / package suites` and `Tree - box clock - by` and the generator was never taught them. So the
# sanctioned "regenerate it rather than hand-editing a cell" instruction, run today, DELETES the provenance
# column for every row -- the exact column SCORE.md's own text spends three paragraphs insisting on
# ("a scoreboard row whose provenance is unresolvable is not a weaker claim, it is an unfalsifiable one").
# The old guard passes it happily: a 4-column grid still starts with '| Language |' and still carries a stamp.
# ⛔ IT REFUSES RATHER THAN PADDING THE MISSING CELLS. Padding would silently publish empty provenance for
# every row, which reads as "nobody measured this" -- a false claim, where a refusal is a true one. The fix
# is to teach the generator the columns the board actually has, and until then this is the guard that keeps
# a documented workflow from quietly destroying the file it maintains.
_ncols = lambda line: len(line.strip().strip('|').split('|'))
_new_cols = _ncols(g.split('\n')[0])
_old_cols = _ncols(next(l for l in s.split('\n') if l.startswith('| Language |')))
assert _new_cols == _old_cols, (
    "REFUSING: the generated grid has %d columns but %s has %d. Splicing would DELETE the extra column(s) "
    "-- on the live board those are 'Vendor / package suites' and 'Tree - box clock - by', i.e. every row's "
    "provenance. Teach util_build_score_md.py the missing columns (it emits a 4-column literal at line 206), "
    "or update a single cell without regenerating: scripts/util_score_row.py write --lang <l> --column <c> "
    "--text '<board line>' --measurer \"$S4E_SEAT\"" % (_new_cols, score, _old_cols))
a = s.index('\n| Language |') + 1
m = re.search(r'^_tree: .*_\n', s[a:], re.M); assert m, "no stamp line after the old grid"
b = a + m.end()
open(score, 'w', encoding='utf-8').write(s[:a] + g + s[b:]); print("spliced: replaced %d old lines with %d new" % (s[a:b].count('\n'), g.count('\n')))
