#!/usr/bin/env bash
# scripts/test_gate_orphaned_witnesses_do_not_grow.sh — ceo CEO-414 (minted hq_P 2026-09-08).
#
# WHAT IT GATES.  An ORPHANED WITNESS is a committed `<name><ext>` + `<name>.ref` pair under
# corpus/tests/<lang>/ whose name appears in NO `ALL.csv`, `ALL.xfail` or `ALL.excluded.txt`.
# It is not passing, not expected-failing and not excluded: NO DENOMINATOR CONTAINS IT, so no
# board has ever run it.  Of the 39 SNOBOL4 orphans measured at mint, FOUR ARE RED and one of
# those SEGFAULTS in both modes (nested_arbno_rpos, rc=139) -- while the SNOBOL4 master read
# 1894/1894 FAIL=0 on the same tree.  This count may FALL, never RISE.
#
# ⭐ WHY A RATCHET IS LEGITIMATE HERE — the question test_gate_term_wordref_ratchet.sh's header
# forces on anyone copying its shape is CAN THIS COUNT REACH ZERO BY DESIGN?  Here it can and is
# meant to: every witness pair we commit is supposed to end up in its language's master, so a
# remaining orphan is debt and 0 is the intended terminal value.  It never fires on legitimate
# work, because ABSORBING an orphan lowers the count and writing a NEW witness straight into the
# master never raises it.
#
# ⛔ THE CURE IS TO ABSORB, NEVER TO DELETE.  Deleting an orphan also makes this gate green and is
# the one move that would satisfy the ratchet while destroying the evidence.  Say so here because
# a ratchet cannot tell the two apart.
#
# ⛔ MEMBERSHIP IS SUBSTRING, NOT TOKEN — AND THAT IS A CORRECTION, NOT A PREFERENCE.  Absorbed
# files are RENAMED into the master and keep their original filename only inside a longer token:
# icon's `alt_arith` lives in ALL.csv as `parser_alt_arith__alt_arith`.  The first census of this
# defect used token matching, could not see that, and MANUFACTURED ~153 FALSE ICON ORPHANS (it
# reported icon 172 / total 213; the truth is icon 19 / total 59).  ⭐ It was control-armed in both
# directions -- ON SNOBOL4, whose naming happens not to use that form, so the control arm passed
# AND COULD NOT HAVE FAILED.  A control arm proves the instrument on the population you ran it on.
# Substring matching is deliberately GENEROUS: it UNDER-reports rather than inventing orphans,
# which is the only safe direction for a ratchet.
set -u
cd "$(dirname "$0")/.." || exit 2
CORPUS="${CORPUS:-$(cd .. && pwd)/corpus}"
CENSUS="$CORPUS/tests/ORPHANED-WITNESSES.tsv"
[ -d "$CORPUS/tests" ] || { echo "⛔ GATE REFUSES(2): no corpus tests tree at $CORPUS/tests"; exit 2; }
[ -f scripts/util_orphaned_witness_census.py ] || { echo "⛔ GATE REFUSES(2): census generator missing"; exit 2; }
# ---- the floor, per language.  Lower a number ONLY in the commit that absorbs the witnesses.
FLOOR_icon=19 FLOOR_pascal=0 FLOOR_prolog=0 FLOOR_raku=0 FLOOR_rebus=0 FLOOR_snobol4=39 FLOOR_snocone=1
out="$(python3 scripts/util_orphaned_witness_census.py "$CORPUS" --counts 2>&1)" || { echo "⛔ GATE REFUSES(2): census generator failed:"; echo "$out"; exit 2; }
printf %s "$out" | grep -q . || { echo "⛔ GATE REFUSES(2): census produced NO output -- an empty census is not a green board"; exit 2; }
rc=0 tot=0 seen=0
while IFS=$'\t' read -r lang n; do
  [ -n "${lang:-}" ] || continue
  seen=$((seen+1)); tot=$((tot+n))
  eval "floor=\${FLOOR_$lang-}"
  if [ -z "$floor" ]; then echo "  ⛔ $lang: $n orphan(s) — NO FLOOR PINNED for this language; add FLOOR_$lang to this gate"; rc=1; continue; fi
  if [ "$n" -gt "$floor" ]; then echo "  ⛔ $lang: $n orphan(s), floor $floor — GREW by $((n-floor))"; rc=1
  elif [ "$n" -lt "$floor" ]; then echo "  ⭐ $lang: $n orphan(s), floor $floor — FELL by $((floor-n)); lower FLOOR_$lang to $n in this commit"; rc=1
  else echo "  ok    $lang: $n orphan(s) at the floor"; fi
done <<< "$out"
[ "$seen" -gt 0 ] || { echo "⛔ GATE REFUSES(2): census listed no languages at all"; exit 2; }
if [ ! -f "$CENSUS" ]; then echo "  ⛔ the census file $CENSUS is MISSING — regenerate it"; rc=1
else
  fresh="$(python3 scripts/util_orphaned_witness_census.py "$CORPUS")"
  if ! printf '%s\n' "$fresh" | diff -q - "$CENSUS" >/dev/null 2>&1; then
    echo "  ⛔ $CENSUS is STALE — it does not match the tree; regenerate:"
    echo "       python3 SCRIP/scripts/util_orphaned_witness_census.py corpus > corpus/tests/ORPHANED-WITNESSES.tsv"; rc=1; fi
fi
echo "------------------------------------------------------------"
[ $rc -eq 0 ] && echo "✅ GATE PASS [orphaned_witnesses_do_not_grow]: $tot orphaned witness pair(s), none above its floor" \
              || echo "⛔ GATE FAIL [orphaned_witnesses_do_not_grow]: $tot orphaned witness pair(s) — see above"
exit $rc
