#!/usr/bin/env python3
"""apply_score_grid.py <SCORE.md> <grid.txt> -- splice util_build_score_md.py's stdout into SCORE.md's STANDARDIZED DISPLAY,
replacing from the '| Language |' header through the '_tree: ..._' stamp line. Refuses if either boundary is missing or if the
new grid lacks a stamp line (a grid without provenance is the defect the generator exists to prevent)."""
import sys, re
score, grid = sys.argv[1], sys.argv[2]
s = open(score, encoding='utf-8').read(); g = open(grid, encoding='utf-8').read().strip('\n') + '\n'
assert g.startswith('| Language |') and re.search(r'^_tree: SCRIP=\S+ corpus=\S+ \.github=\S+  generated \S+_$', g, re.M), "new grid malformed or unstamped"
a = s.index('\n| Language |') + 1
m = re.search(r'^_tree: .*_\n', s[a:], re.M); assert m, "no stamp line after the old grid"
b = a + m.end()
open(score, 'w', encoding='utf-8').write(s[:a] + g + s[b:]); print("spliced: replaced %d old lines with %d new" % (s[a:b].count('\n'), g.count('\n')))
