#!/usr/bin/env bash
# test_gate_suite_set_does_not_move_another_suites_row.sh
# (coo 2026-09-08, on hq_P's measured report; cures the coo's own 2026-09-06 change)
#
# ⛔ THE DEFECT, MEASURED BY hq_P TWICE IN ONE EVENING BEFORE ANYONE LOOKED FOR IT.
# util_suite_banner.py --set writes one row of SUITES.tsv and then RE-RENDERS SCORE.md's suite table
# from that TSV. The re-render was UNSCOPED, so a seat that measured ONE suite rewrote the display
# cells of EVERY OTHER suite -- with whatever its own local SUITES.tsv happened to hold. When that
# local TSV was behind origin, the write was a SILENT REVERT of another seat's newer number, and it
# landed inside a commit whose message truthfully described something else entirely.
# hq_P's case: their tree held `Budne 64/93 (0bd961e07)` while origin already held
# `Budne 66/93 (251693227)`. Only a rebase conflict caught it; `git add -A` after a board would have
# pushed the revert silently, and the pusher would have believed they committed a FINDING.
#
# ⛔ WHY A GATE AND NOT JUST THE FIX: the trigger is ordinary (run any board, commit normally), the
# blast radius is the whole table, and the damage is INVISIBLE in the diff of the commit that does it
# unless a human reads rows they never measured. Nothing else on the board can see this.
#
# HERMETIC: every arm runs against scratch copies under mktemp via S4E_SUITES_TSV / S4E_SCORE_MD.
# The real .github/SUITES.tsv and SCORE.md are never opened for writing -- asserted at the end by
# comparing their hashes from before and after.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GH="$HERE/../../.github"
BANNER="$GH/scripts/util_suite_banner.py"
for f in "$BANNER" "$GH/SUITES.tsv" "$GH/SCORE.md"; do
    [ -f "$f" ] || { echo "⛔ REFUSE(rc=2): missing $f -- cannot measure, which is never green"; exit 2; }
done
REAL_BEFORE="$(cat "$GH/SUITES.tsv" "$GH/SCORE.md" | cksum)"

W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
RED=0; ARMS=0
# VICTIM is the suite whose row must NOT move; SUBJECT is the one being --set.
VICTIM=gimpel; SUBJECT=snoflake
reset_fixture() {
    cp "$GH/SUITES.tsv" "$W/SUITES.tsv"; cp "$GH/SCORE.md" "$W/SCORE.md"
    # Make the LOCAL tsv deliberately stale for the VICTIM, standing in for "origin moved ahead of me".
    python3 - "$W/SUITES.tsv" "$VICTIM" <<'PY'
import sys
p,victim=sys.argv[1],sys.argv[2]
L=open(p,encoding='utf-8').read().split('\n')
for i,l in enumerate(L):
    if l.startswith(victim+'\t'):
        f=l.split('\t'); f[8]='1'; f[9]='999'; L[i]='\t'.join(f)
open(p,'w',encoding='utf-8').write('\n'.join(L))
PY
}
victim_row() { grep -oE "^\| [^|]*\($VICTIM\) \|[^|]*\|[^|]*\|[^|]*" "$W/SCORE.md" | head -1; }
subject_row() { grep -oE "^\| [^|]*\($SUBJECT\) \|[^|]*\|[^|]*\|[^|]*" "$W/SCORE.md" | head -1; }

echo "== test_gate_suite_set_does_not_move_another_suites_row =="
echo "   victim=$VICTIM (local TSV deliberately staled to 1/999) · subject=$SUBJECT · hermetic scratch under mktemp"

# ARM 1 — THE INVARIANT: --set on SUBJECT leaves VICTIM's rendered row byte-identical.
reset_fixture; before="$(victim_row)"
S4E_SUITES_TSV="$W/SUITES.tsv" S4E_SCORE_MD="$W/SCORE.md" \
  python3 "$BANNER" --set "$SUBJECT" 107 124 2026-09-08 gate0test1 >/dev/null 2>&1
after="$(victim_row)"; ARMS=$((ARMS+1))
if [ "$before" = "$after" ]; then echo "  ✓ arm 1: --set $SUBJECT left $VICTIM's row byte-identical"
else echo "  ⛔ arm 1: --set $SUBJECT REWROTE $VICTIM's row from a stale local TSV"; echo "      before: $before"; echo "      after : $after"; RED=$((RED+1)); fi

# ARM 2 — THE POSITIVE CONTROL, and it is load-bearing: an implementation that writes NOTHING would
# pass arm 1 perfectly. The subject's own row must actually carry the new numbers.
ARMS=$((ARMS+1))
if subject_row | grep -q '107/124'; then echo "  ✓ arm 2: $SUBJECT's own row carries the numbers it was set to (107/124)"
else echo "  ⛔ arm 2: $SUBJECT's row does NOT carry 107/124 -- the write was scoped away entirely"; echo "      row: $(subject_row)"; RED=$((RED+1)); fi

# ARM 3 — FAIL-ONCE / DETECTOR PROOF: the UNSCOPED path (--render, which is what --set used to do)
# must still move the victim on this same fixture. If it does not, the fixture stopped reproducing the
# defect and arm 1 is passing over nothing -- the empty-denominator shape.
reset_fixture; before="$(victim_row)"
S4E_SUITES_TSV="$W/SUITES.tsv" S4E_SCORE_MD="$W/SCORE.md" python3 "$BANNER" --render >/dev/null 2>&1
after="$(victim_row)"; ARMS=$((ARMS+1))
if [ "$before" != "$after" ]; then echo "  ✓ arm 3: the unscoped --render still moves $VICTIM, so arm 1's fixture really does reproduce the defect"
else echo "  ⛔ arm 3: the unscoped --render did NOT move $VICTIM -- this fixture no longer reproduces the defect, so arm 1 proves nothing"; RED=$((RED+1)); fi

# ARM 4 — HERMETIC: the real records were never written.
ARMS=$((ARMS+1))
if [ "$(cat "$GH/SUITES.tsv" "$GH/SCORE.md" | cksum)" = "$REAL_BEFORE" ]; then echo "  ✓ arm 4: the real SUITES.tsv and SCORE.md are byte-identical after this run"
else echo "  ⛔ arm 4: THIS GATE WROTE THE REAL BOARD -- a gate that mutates live state is unrunnable"; RED=$((RED+1)); fi

echo "-- population: 1 stale-TSV fixture × $ARMS arms (invariant, positive control, detector proof, hermeticity), $RED red"
[ "$RED" -eq 0 ] || { echo "⛔ GATE RED: $RED of $ARMS arms"; exit 1; }
echo "✅ GATE OK: $ARMS/$ARMS arms -- a suite --set moves its own row and no other"
