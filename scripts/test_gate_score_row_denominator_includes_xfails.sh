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
import re,sys
rows=[l for l in open(sys.argv[1],encoding='utf-8').read().splitlines() if re.search(r'\(\w[\w-]*-master\)',l)]
if not rows:
    print("⛔ GATE REFUSES (rc=2): no '(<lang>-master)' rows found in SCORE.md -- the table's shape changed and this gate is grading nothing."); sys.exit(2)
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
    xf=re.search(r'(\d+)\s*xfail',l)
    if not xf: continue          # a row declaring no xfail count is not gradable here, and says so by silence
    p,t=cur; x=int(xf.group(1)); graded+=1
    if t < p+x:
        bad.append(f"  {name}: reads {p}/{t} while declaring {x} xfail -- needs total >= {p+x}, short by {p+x-t}"
                   + ("  [N/N: every known red dropped from BOTH sides]" if p==t else ""))
if graded==0:
    print("⛔ GATE REFUSES (rc=2): found master rows but none declared an xfail count -- graded zero, so this prints no verdict."); sys.exit(2)
if bad:
    print(f"⛔ FAIL: {len(bad)} of {graded} master row(s) exclude their own xfails from the denominator (CEO-416: an xfail counts as a FAIL -- denominator, not numerator):")
    print("\n".join(bad))
    print("  cure: the row's writer must put the xfails in the total. A runner that sums pass+fail+skip only will re-flip this on every run.")
    sys.exit(1)
print(f"✅ GATE OK: {graded}/{graded} master row(s) carry their declared xfails inside the denominator.")
PY
