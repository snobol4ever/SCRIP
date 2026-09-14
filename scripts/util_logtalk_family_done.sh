#!/usr/bin/env bash
# util_logtalk_family_done.sh -- the DONE-WHEN body of one Logtalk FAMILY row (ceo CEO-650): grades each named group
# with util_logtalk_grade.py --group (a development aid, never a board -- the coo's test_prolog_logtalk_suite.sh is the
# board) in BOTH modes and is green only when every group's cases ALL PASS IN BOTH MODES over a non-zero population.
#
# ⛔⭐ THE VERDICT IS both-modes-pass == population, NOT FAIL=0, AND THE DIFFERENCE IS A FALSE GREEN THAT
# FIRED (hq_C, 2026-09-13). FAIL is not the only way to not pass: the runner's own doctrine is that a case
# it cannot set up is UNGRADED, named with its reason, "never dropped from the population and never counted
# as passing" -- but a FAIL=0 verdict counts it as passing anyway. Measured that day: a harness change
# turned the 15 cases blocked on stream aliases from FAIL into UNGRADED, and this script immediately
# reported `escape_sequences PASS cases=12 both-modes=8` -- a green group with a THIRD of its cases
# unmeasured, printed on the same line as the 8 that were. ⭐ Nothing about the change was wrong: the
# reclassification was correct and made the board MORE honest. The defect was that this verdict read one of
# the six buckets and treated the other five as empty, so becoming more honest upstream read as becoming
# greener here -- a criterion that improves when the work becomes less measurable can never close its row.
# Usage: bash scripts/util_logtalk_family_done.sh <group> [<group>...]      rc 0 green · 1 red · 2 could not measure
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP_BIN:-$ROOT/scrip}"; [ -x "$SCRIP" ] || { echo "⛔ REFUSE(2): no scrip at $SCRIP"; exit 2; }
SUITE="$ROOT/../corpus/packages/prolog/logtalk_iso"; [ -d "$SUITE" ] || { echo "⛔ REFUSE(2): no logtalk_iso at $SUITE"; exit 2; }
[ $# -ge 1 ] || { echo "⛔ REFUSE(2): name at least one group (a directory basename under $SUITE)"; exit 2; }
RC=0; POP=0; BOTH=0; F3=0; F4=0; OUT=0; N=$#
for G in "$@"; do
  L=$(cd "$ROOT" && timeout 900 python3 scripts/util_logtalk_grade.py --suite "$SUITE" --scrip "$SCRIP" --modes m3,m4 --group "$G" 2>&1 | grep -m1 '^BOARD_FOR_SHELL ') || true
  out=$(printf '%s' "$L" | grep -oE 'outside=[0-9]+' | head -1); out=${out#outside=}
  set -- $L; [ "${1:-}" = BOARD_FOR_SHELL ] && [ "${2:-0}" -gt 0 ] || { echo "⛔ REFUSE(2): group $G graded nothing ($L)"; exit 2; }
  pop=$2; both=$3; p3=$4; f3=$5; p4=$6; f4=$7
  # ⛔ THE GRADED DENOMINATOR IS THE POPULATION MINUS WHAT THE ORACLE CANNOT RUN, AND IT IS NAMED EVERY TIME
  # IT IS USED (cto ruling 2026-09-13; RULES.md FACT RULE CEO-542, CEO-391: outside-the-baseline is named
  # beside the suite, out of the denominator, NEVER HIDDEN). ⭐ An absent outside= field means an OLDER grader
  # that cannot assign the bucket -- it reads 0 and the verdict falls back to the full population, which is
  # the strict direction. A missing measurement must never be the lenient one.
  [ -n "$out" ] || out=0
  gpop=$((pop-out)); OUT=$((OUT+out))
  POP=$((POP+pop)); BOTH=$((BOTH+both)); F3=$((F3+f3)); F4=$((F4+f4))
  [ "$gpop" -gt 0 ] || { echo "⛔ REFUSE(2): group $G has $pop case(s) and ALL of them are outside the baseline -- a group with an empty graded denominator cannot be PASSED or RED, and calling it green would be a verdict on nothing"; exit 2; }
  if [ "$both" = "$gpop" ]; then echo "  $G  PASS  graded=$gpop both-modes=$both$([ "$out" -gt 0 ] && echo "  (+$out OUTSIDE the baseline, of $pop)")"
  else echo "  $G  RED   graded=$gpop both-modes=$both m3_fail=$f3 m4_fail=$f4 not-passing=$((gpop-both))$([ "$out" -gt 0 ] && echo "  (+$out OUTSIDE the baseline, of $pop)")"; RC=1; fi
done
echo "LOGTALK_FAMILY groups=$N cases=$POP outside=$OUT graded=$((POP-OUT)) both_modes_pass=$BOTH m3_fail=$F3 m4_fail=$F4 verdict=$([ $RC = 0 ] && echo GREEN || echo RED)  tree: SCRIP=$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null)$(git -C "$ROOT" diff --quiet 2>/dev/null || echo -DIRTY)  measured $(date -u +%Y-%m-%dT%H:%MZ)"
exit $RC
