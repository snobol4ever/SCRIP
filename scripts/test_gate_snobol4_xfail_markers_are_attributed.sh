#!/usr/bin/env bash
# test_gate_snobol4_xfail_markers_are_attributed.sh — every surviving XFAIL marker in the SNOBOL4
# master must NAME A LIVE ROW, so that "expected to fail" always carries a route to a cure.
#
# ⛔ WHY THIS GATE EXISTS (hq_P 2026-09-06, row snobol4-every-xfail-fixed-as-a-faulty-test-or-cured-
# as-a-defect; CEO-328 (4) asked the question in prose and nobody could run it). Measured at the time
# of writing: of 32 markers, 6 named a live row, 3 named a row that was never minted, and 23 named NO
# ROW AT ALL -- they cite a FINDING and the words "Known, still open". ⭐ A FINDING IS A RECORD, NOT A
# ROUTE: under Lon's THERE IS NO XFAIL ruling, a marker whose text names no row leaves its reader with
# no way to reach the work, and it is invisible to every board because it is, by construction, expected.
# ⛔ THAT IS A STATEMENT ABOUT THE MARKER'S TEXT, NOT ABOUT WHETHER ANYONE OWNS THE DEFECT -- see the
# double-banner note below before you act on a single number from this gate.
#
# ⛔ REPORTED, NOT BLOCKING, ON PURPOSE: it is RED today (23 unresolvable by name). It is wired into `make test`
# with a leading `-` exactly like test_gate_no_xfail_survives.sh, and the ramp to blocking is a
# ceo/Lon date decision, not a script author's.
#
# ⛔⛔ READ THE OUTPUT AS "UNRESOLVABLE BY NAME", NEVER AS "UNOWNED" -- THE AUTHOR OF THIS GATE MADE
# EXACTLY THAT MISREADING AND WAS ONE COMMAND FROM MINTING THREE DUPLICATE ROWS INTO ANOTHER HQ'S LANE.
# The 23 ARE owned: six snobol4-xfail-class-* rows cover this population, and each re-derives its own
# membership with its own bespoke grep over the marker prose (hq_T's fuzz row closes on
# `grep -ciE "fz_|fuzz"`). No marker names its row and no row lists its entries, so the join is prose
# and is invisible from both ends -- that is the defect, not an absence of rows. ⭐ A gate that answers
# "does this string appear in QUEUE.tsv" cannot answer "is this defect owned", and it has no way to
# say so. See FINDING-2026-09-06-hq_P-23-of-32-snobol4-xfail-markers-name-no-live-row-... (the filename
# is the claim that turned out to be wrong; it is kept because the correction is the finding).
#
# ⛔ WHAT THIS GATE DOES NOT CLAIM: that an attributed marker is CORRECT, or that its row is the right
# row. It grades ROUTING ONLY. A marker pointing at a live row it has nothing to do with passes here
# and is still wrong -- that is a different question and needs a different instrument.
set -u
here="$(cd "$(dirname "$0")" && pwd)"; root="$(cd "$here/../.." && pwd)"
MASTER_XFAIL="$root/corpus/tests/snobol4/ALL.xfail"
QUEUE="${S4E_QUEUE:-/home/resources/postoffice/QUEUE.tsv}"
# ⛔ REFUSE rc=2 rather than skip-as-success: an instrument that cannot measure must say so. A missing
# ALL.xfail is NOT "zero unattributed markers" -- it is a gate that graded nothing and must not print
# the same string as one that passed.
[ -f "$MASTER_XFAIL" ] || { echo "⛔ GATE REFUSES: no $MASTER_XFAIL -- cannot grade marker attribution"; exit 2; }
[ -f "$QUEUE" ]        || { echo "⛔ GATE REFUSES: no QUEUE.tsv at $QUEUE -- cannot tell a live row from a dead one"; exit 2; }
command -v python3 >/dev/null 2>&1 || { echo "⛔ GATE REFUSES: no python3"; exit 2; }
python3 - "$MASTER_XFAIL" "$QUEUE" <<'PY'
import re, sys, collections
BANNER = re.compile(r"^\*-+ (?P<seq>\d+) (?P<name>\S+)(?P<xfail> XFAIL)?$")
xf, q = sys.argv[1], sys.argv[2]
rows = {}
for ln in open(q, encoding="utf-8", errors="replace"):
    if ln.startswith("#"):
        continue
    f = ln.rstrip("\n").split("\t")
    if len(f) >= 4:
        rows[f[1]] = f[3]
if not rows:
    print("⛔ GATE REFUSES: QUEUE.tsv parsed as zero rows -- the format moved under this gate")
    sys.exit(2)
marks, cur = collections.OrderedDict(), None
for ln in open(xf, encoding="utf-8", errors="replace"):
    m = BANNER.match(ln.rstrip("\n"))
    if m:
        cur = m.group("name"); marks[cur] = ""
    elif cur and ln.strip():
        marks[cur] += ln
if not marks:
    print("⛔ GATE REFUSES: %s parsed as zero markers -- banner format moved, or the file is a stub" % xf)
    sys.exit(2)
live, closed, unminted, none = [], [], [], []
for e, txt in marks.items():
    cands = set(re.findall(r"`([a-z0-9][a-z0-9-]{6,})`", txt))
    cands |= set(re.findall(r"(?:class )?row `?([a-z0-9][a-z0-9-]{6,})`?", txt))
    known = {c for c in cands if c in rows}
    if known:
        (live if any(not rows[c].startswith(("DONE", "SUPERSEDED")) for c in known) else closed).append((e, sorted(known)))
    elif cands:
        unminted.append((e, sorted(cands)))
    else:
        none.append((e, []))
print("XFAIL MARKER ATTRIBUTION (snobol4): markers=%d  live_row=%d  row_closed=%d  named_but_never_minted=%d  NO_ROW=%d"
      % (len(marks), len(live), len(closed), len(unminted), len(none)))
bad = closed + unminted + none
for label, group in (("ROW CLOSED", closed), ("NAMED BUT NEVER MINTED", unminted), ("NAMES NO ROW", none)):
    for e, c in group:
        print("  %-24s %-46s %s" % (label, e, ",".join(c) if c else "-"))
if bad:
    print("⛔ %d of %d markers do not NAME their owning row -- unresolvable by name." % (len(bad), len(marks)))
    print("   ⛔ This is NOT a count of unowned defects: six snobol4-xfail-class-* rows cover this")
    print("      population and each re-derives membership with its own grep over this prose. The join")
    print("      is prose and invisible from both ends -- that is the defect this gate reports.")
    sys.exit(1)
print("✅ every surviving XFAIL marker names a live row")
PY
