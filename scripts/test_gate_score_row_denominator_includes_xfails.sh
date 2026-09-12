#!/usr/bin/env bash
# test_gate_score_row_denominator_includes_xfails.sh -- ROW-LEVEL GATE (hq_T, on coo's ask 2026-09-08).
# ⛔⭐ ASSERTS THE PROPERTY THE RULING ESTABLISHED, NOT THE NUMBERS IT ESTABLISHED THEM WITH (Lon's FACT RULE via
# ceo CEO-416: an xfail counts as a FAIL, so it sits in the DENOMINATOR and out of the numerator).  A row is red
# here when its own declared xfail count cannot fit in the gap between its pass and its total -- that is, when
# total < pass + xfail.  The N/N shape is the extreme case of it.
# ⭐ WHY AT THE ROW AND NOT IN A RUNNER: the defect that prompted this was one runner's arithmetic dropping m4x
# from TOTAL, and curing that runner would leave the NEXT runner free to make the same mistake.  A row-level
# assertion catches every writer of every row, including hands, and survives whichever way the numbers are later
# ruled -- the ruling can move 1871/1898 anywhere it likes and this gate keeps meaning the same thing.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCORE="${S4E_SCORE_MD:-$HERE/../../.github/SCORE.md}"
[ -r "$SCORE" ] || { echo "⛔ GATE REFUSES (rc=2): cannot read SCORE.md at $SCORE -- a gate that cannot measure never prints the success shape."; exit 2; }
python3 - "$SCORE" <<'PY'
import re,sys,os
_lines=open(sys.argv[1],encoding='utf-8').read().splitlines()
# rows name a suite by NICKNAME only (Lon 2026-09-12: one name); the master rows are the nicknames whose SUITES.tsv key ends in -master
_nicks=set()
try:
    for _l in open(os.path.join(os.path.dirname(sys.argv[1]),'SUITES.tsv'),encoding='utf-8'):
        if _l.startswith('#') or not _l.strip(): continue
        _c=_l.rstrip('\n').split('\t')
        if len(_c)>1 and _c[0].endswith('-master'): _nicks.add(_c[1])
except OSError: pass
rows=[l for l in _lines if re.match(r'^\|\s*([^|]+?)\s*\|',l) and re.match(r'^\|\s*([^|]+?)\s*\|',l).group(1) in _nicks]
# ⛔⭐⭐ THE DECLARED XFAIL IS READ FROM THE RUNNER-PUBLISHED CELL, NEVER FROM PROSE ANYWHERE ON THE ROW.
# This gate used to take the first `<n> xfail` it found on the grid line, and on IcnM that was a HAND-WRITTEN
# note, "20 xfail=fail", counted from 20 per-entry .xfail markers in corpus/tests/icon. hq_I then measured what
# those 20 actually are: entries that ARE NOT IN ALL.csv AT ALL (and ALL.csv carries zero rows with xfail=1). So
# the note and the fraction describe TWO DISJOINT SETS, and this gate was comparing across that seam -- reporting
# a denominator defect on a row whose denominator was never wrong. ⭐ The general form, which is why the cure is a
# reader change and not a number change: an arithmetic check is only as sound as its premise that both operands
# describe the SAME POPULATION, and nothing about `704/704` and `20 xfail` on one line says whether they do. The
# only xfail count that provably shares the fraction's population is the one the RUNNER published from the same
# board, which since SCRIP 845b25e70 sits in the Master board cell as `xfail=<n>`.
def _published_xfail(lang):
    """the runner's own xfail count for this language, out of the display table's Master board cell."""
    hdr=col=None
    for n,l in enumerate(_lines):
        if not l.startswith('|'): continue
        cs=[c.strip() for c in l.strip().strip('|').split('|')]
        for i,c in enumerate(cs):
            if c.lower().startswith('master board'): hdr,col=n,i; break
        if hdr is not None: break
    if hdr is None: return None
    for l in _lines[hdr+1:]:
        if not l.startswith('|'): continue
        cs=[c.strip() for c in l.strip().strip('|').split('|')]
        if len(cs)<=col: continue
        if cs[0].strip('* ').lower()!=lang: continue
        # ⛔⭐ ONLY THE CURRENT READING. util_score_row.py folds the PREVIOUS cell text forward behind a
        # SUPERSEDES marker, verbatim, as provenance -- so a cell physically contains every xfail count this
        # row has ever published. Scanning the whole cell takes the MAX over history, and this gate convicted
        # its own freshly-published row on a number from the run before it (1870/1894 graded against an
        # xfail=25 that a superseded clause was still carrying). ⭐ Same family as the row it was written for:
        # two numbers in one cell describing two different measurements, and adjacency saying nothing about
        # which. The sentinel is util_score_row.py's own (SUPERSEDE_MARKER), matched on its stable prefix.
        cur=cs[col].split('SUPERSEDES the reading below')[0]
        ms=[int(m) for m in re.findall(r'xfail=(\d+)',cur)]
        # the worst mode is the binding one: a count that holds for m3 and not m4 has not been checked.
        return max(ms) if ms else None
    return None
if not rows:
    print("⛔ GATE REFUSES (rc=2): no master rows (by nickname, SUITES.tsv keys ending in -master) found in SCORE.md -- the table's shape changed and this gate is grading nothing."); sys.exit(2)
bad=[];graded=0
for l in rows:
    cells=[c.strip() for c in l.strip().strip('|').split('|')]
    name=re.sub(r'\s+',' ',cells[0])
    # the CURRENT reading is the first cell carrying `P/T (date, `hash`)` -- the dated-with-tree form.
    cur=None
    for c in cells[1:]:
        m=re.match(r'^(\d+)/(\d+)\s*\(\d\d-\d\d,',c)
        if m: cur=(int(m.group(1)),int(m.group(2))); break
    if cur is None: continue
    _lang=None
    for c in cells[1:]:
        if re.fullmatch(r'[a-z][a-z0-9_]*',c): _lang=c; break
    x=_published_xfail(_lang) if _lang else None
    if x is None: continue       # no runner-published xfail for this language: not gradable here, and silent by design
    p,t=cur; graded+=1
    if t < p+x:
        bad.append(f"  {name}: reads {p}/{t} while its runner published {x} xfail -- needs total >= {p+x}, short by {p+x-t}"
                   + ("  [N/N: every known red dropped from BOTH sides]" if p==t else ""))
if graded==0:
    print("⛔ GATE REFUSES (rc=2): found master rows but none carries a RUNNER-PUBLISHED xfail= count in its Master board cell -- graded zero, so this prints no verdict. (Prose on the grid row is deliberately not read: see the header.)"); sys.exit(2)
if bad:
    print(f"⛔ FAIL: {len(bad)} of {graded} master row(s) exclude their own xfails from the denominator (CEO-416: an xfail counts as a FAIL -- denominator, not numerator):")
    print("\n".join(bad))
    print("  cure: the row's writer must put the xfails in the total. A runner that sums pass+fail+skip only will re-flip this on every run.")
    sys.exit(1)
print(f"✅ GATE OK: {graded}/{graded} master row(s) carry their RUNNER-PUBLISHED xfails inside the denominator.")
PY
