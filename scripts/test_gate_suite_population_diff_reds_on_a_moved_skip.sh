#!/usr/bin/env bash
# test_gate_suite_population_diff_reds_on_a_moved_skip.sh -- A SKIP THAT MOVES IS A RED, IN EITHER DIRECTION
# (coo 2026-09-17, CEO-824; row instruments-a-skip-count-change-between-batches-is-a-red-not-a-silent-population-change,
# minted on the cfo's escalation).
#
# THE DEFECT, MEASURED, AND WHY A GATE RATHER THAN A HABIT.  The SNOBOL4 master read m4_pass=1970 m4_skip=0 at SCRIP
# ad0f85fae and m4_pass=1967 m4_skip=3 at b12714737, and that landing's own commit said "equal to base program for
# program" while its board wrote SCORE's SnoM row on that tree; the three were cured at 0567ad829 and read skip=0 at
# a861648ab.  BOTH the appearance and the disappearance passed unremarked, because every reader compared PASS and a
# SKIP fires no verdict in the ladder -- it is the DENOMINATOR moving, which lowers the pass count exactly like a
# regression and raises it exactly like a cure.
#
# WHAT THIS GRADES: the instrument, not a suite.  It is hermetic and needs no build -- it plants readings and asserts
# the verdicts, so it cannot go quiet when a runner is unavailable.
#   (a) util_suite_population_diff.py --selftest passes, and its arms are COUNTED (>= 13)
#   (b) THE RECORDED PAIR, re-derived here rather than trusted: 0 -> 3 is rc=1, 3 -> 0 is rc=1, identical is rc=0
#   (c) a pass/fail move with the population FIXED is rc=0 -- an instrument that reds on everything measures nothing
#   (d) two families and a shard-vs-full reading each REFUSE rc=2, never a number
# FAIL_ONCE=1 blanks the selftest's proof line before arm (a) asserts, to prove the arm trips.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; cd "$HERE/.." || exit 2
D="$HERE/util_suite_population_diff.py"
refuse(){ echo "⛔ REFUSED-TO-GRADE (rc=2): $*"; exit 2; }
[ -f "$D" ] || refuse "no instrument at $D"
fails=0; checks=0
ck(){ checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
echo "=== gate: a SKIP or graded-denominator move between two readings of one suite is a red, in either direction ==="

st="$(timeout 60s python3 "$D" --selftest 2>&1)"; strc=$?
[ "$strc" = 124 ] && refuse "the instrument's selftest timed out"
[ -n "${FAIL_ONCE:-}" ] && st="$(printf '%s\n' "$st" | grep -v '^SELFTEST PASS')"
arms="$(printf '%s\n' "$st" | sed -n 's/^population: \([0-9]*\) selftest arm(s).*/\1/p')"
if printf '%s\n' "$st" | grep -q '^SELFTEST PASS' && [ "$strc" = 0 ] && [ "${arms:-0}" -ge 13 ]; then
  ck ok "(a) the instrument trips on every planted arm -- $arms arms, 0 FAIL"
else
  ck no "(a) the selftest did not pass (rc=$strc, arms=${arms:-none}): $(printf '%s\n' "$st" | grep -m2 '  FAIL  ' | tr '\n' ';')"
fi

W="$(mktemp -d)" || refuse "no temp dir"; trap 'rm -rf "$W"' EXIT
B="SUITE_BOARD family=ALL total=1982 shipped=1982 outside=8 m3_n=1974 m3_pass=1970 m3_fail=3 m3_skip=0 m4_n=1974"
printf '%s m4_pass=1970 m4_fail=3 m4_skip=0 all_pass=1970 all_n=1982\n' "$B" > "$W/base"
printf '%s m4_pass=1967 m4_fail=3 m4_skip=3 all_pass=1967 all_n=1982\n' "$B" > "$W/skipped"
printf '%s m4_pass=1967 m4_fail=6 m4_skip=0 all_pass=1967 all_n=1982\n' "$B" > "$W/regressed"
sed 's/family=ALL/family=OTHER/' "$W/base" > "$W/other"
sed 's/$/ shard=1\/4/' "$W/base" > "$W/shard"
r(){ python3 "$D" "$1" "$2" >"$W/out" 2>&1; echo $?; }
[ "$(r "$W/base" "$W/skipped")" = 1 ] && ck ok "(b1) the recorded ad0f85fae -> b12714737 pair (m4_skip 0 -> 3, pass 1970 -> 1967) reds" \
  || ck no "(b1) the recorded 0 -> 3 pair did not red: $(head -2 "$W/out" | tr '\n' ';')"
[ "$(r "$W/skipped" "$W/base")" = 1 ] && ck ok "(b2) the DISAPPEARANCE (3 -> 0, the 0567ad829 cure) reds too -- a skip that vanishes is as much a population change as one that appears" \
  || ck no "(b2) the 3 -> 0 direction did not red: $(head -2 "$W/out" | tr '\n' ';')"
[ "$(r "$W/base" "$W/base")" = 0 ] && ck ok "(b3) two identical readings are green" \
  || ck no "(b3) two identical readings did not read green: $(head -2 "$W/out" | tr '\n' ';')"
[ "$(r "$W/base" "$W/regressed")" = 0 ] && ck ok "(c) three PASSes becoming three FAILs with the population FIXED is green here and printed as information -- that red is the board's, not this instrument's" \
  || ck no "(c) a pure pass/fail move was convicted as a population change: $(head -2 "$W/out" | tr '\n' ';')"
[ "$(r "$W/base" "$W/other")" = 2 ] && ck ok "(d1) two different families REFUSE rc=2 -- comparing two populations is not a comparison" \
  || ck no "(d1) two families did not refuse: $(head -2 "$W/out" | tr '\n' ';')"
[ "$(r "$W/base" "$W/shard")" = 2 ] && ck ok "(d2) a shard reading against a full board REFUSES rc=2" \
  || ck no "(d2) shard-vs-full did not refuse: $(head -2 "$W/out" | tr '\n' ';')"

echo "population: $checks arm(s) graded, $fails FAIL; the instrument's own selftest carried ${arms:-0} more"
if [ "$fails" -eq 0 ]; then
  echo "GATE PASS [suite_population_diff_reds_on_a_moved_skip]: $checks of $checks arms hold"; exit 0
fi
echo "⛔ GATE RED [suite_population_diff_reds_on_a_moved_skip]: $fails of $checks arms FAIL"; exit 1
