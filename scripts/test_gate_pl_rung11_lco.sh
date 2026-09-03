#!/usr/bin/env bash
# test_gate_pl_rung11_lco.sh -- RUNG 11 LANDING GATE (LCO / last-call optimisation, ARCH-PROLOG-BYRD-BOX-TRANSLATION.md
# sec B.18; task prolog-rung-11-last-call-optimisation-lco). This is the task's own DONE-WHEN, minted as a script rather
# than a one-line bash -c: the sec B.18 depth criterion needs two witness programs and an RSS comparison, which does not
# fit legibly on one line and this project's OWN convention for a multi-part criterion is a dedicated script (see
# test_gate_pl_quad_regs.sh, test_prolog_ladder.sh), not a fragile nested-quoting one-liner.
# THREE PARTS, ALL REQUIRED:
#   (1) bash scripts/test_prolog_ladder.sh --only 11   -- PASS 2/2 (m3 + m4) on the ladder's own rung-11 witness.
#   (2) bash scripts/test_gate_pl_port_trace.sh --only 11 -- the port-trace instrument (refs owed at landing, ceo 2026-09-03).
#   (3) THE DEPTH CRITERION -- count(N) to 10^6 under `ulimit -s 512` (vs the default 8MB, so the ceiling this proves is
#       tighter than the ladder witness's own 100000 by construction), and RSS must be FLAT across N, not merely
#       non-crashing: measured at N=1000 and N=1000000 (a 1000x range), the high-N run's max RSS must stay under 2x the
#       low-N run's -- a generous bound (true O(1) LCO measures near-identical, ~9.7MB both ends on this seat's run) that
#       still catches an O(N) regression (a 1000x N range would blow way past 2x if frames were still accumulating).
# EXIT: 0 all three parts pass · 1 any part fails · 2 UNPROVEN (cannot measure: no scrip binary, a witness would not write).
# AUTHORSHIP: drafted by seat05 while it held this row, saved outside the root and never pushed when the ceo released
# the row (a fleet seat never touches a rung); adopted here unchanged in substance by hq_P, which owns the row, after
# proving it FAILS ONCE -- part 3's RSS bound was checked to reject an O(N) stack by running the high-N witness with a
# deliberately un-flat control. A DONE-WHEN nobody has seen fail is not an acceptance test.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; cd "$ROOT"
SCRIP="${SCRIP:-$ROOT/scrip}"
[ -x "$SCRIP" ] || { echo "GATE UNPROVEN(2) [pl_rung11_lco]: no scrip binary at $SCRIP"; exit 2; }

echo "--- part 1: ladder --only 11 ---"
LOUT="$(bash "$HERE/test_prolog_ladder.sh" --only 11 2>&1)"; LRC=$?
printf '%s\n' "$LOUT"
echo "$LOUT" | grep -q '^rung 11 summary: PASS=2 FAIL=0 '
P1=$?
[ "$LRC" -ne 2 ] || { echo "GATE UNPROVEN(2) [pl_rung11_lco]: part 1 (ladder) could not measure"; exit 2; }

echo "--- part 2: port-trace --only 11 ---"
bash "$HERE/test_gate_pl_port_trace.sh" --only 11
P2=$?
[ "$P2" -ne 2 ] || { echo "GATE UNPROVEN(2) [pl_rung11_lco]: part 2 (port-trace) could not measure"; exit 2; }

echo "--- part 3: sec B.18 depth criterion (10^6 under ulimit -s 512, RSS flat) ---"
command -v /usr/bin/time >/dev/null 2>&1 || { echo "GATE UNPROVEN(2) [pl_rung11_lco]: /usr/bin/time not found, cannot measure RSS"; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
mk_witness() { printf ':- initialization(main).\ncount(N,N) :- !.\ncount(I,N) :- I < N, J is I+1, count(J,N).\nmain :- count(0,%s), write(done), nl.\n' "$1"; }
mk_witness 1000    > "$W/lo.pl"
mk_witness 1000000 > "$W/hi.pl"
LO_OUT="$(/usr/bin/time -v "$SCRIP" "$W/lo.pl" </dev/null 2>"$W/lo.time")"; LO_RC=$?
LO_RSS="$(grep 'Maximum resident' "$W/lo.time" | grep -oE '[0-9]+')"
HI_OUT="$(bash -c "ulimit -s 512; exec /usr/bin/time -v \"$SCRIP\" \"$W/hi.pl\" </dev/null" 2>"$W/hi.time")"; HI_RC=$?
HI_RSS="$(grep 'Maximum resident' "$W/hi.time" | grep -oE '[0-9]+')"
echo "N=1000:    rc=$LO_RC out=$LO_OUT rss=${LO_RSS:-?}KB"
echo "N=1000000: rc=$HI_RC out=$HI_OUT rss=${HI_RSS:-?}KB (ulimit -s 512)"
[ -n "$LO_RSS" ] && [ -n "$HI_RSS" ] || { echo "GATE UNPROVEN(2) [pl_rung11_lco]: could not read Maximum resident set size from /usr/bin/time -v output"; exit 2; }
P3=1
[ "$LO_RC" -eq 0 ] && [ "$HI_RC" -eq 0 ] && [ "$LO_OUT" = "done" ] && [ "$HI_OUT" = "done" ] && [ "$HI_RSS" -lt "$((LO_RSS * 2))" ] && P3=0

echo "--- summary ---"
echo "part1(ladder)=$([ $P1 -eq 0 ] && echo PASS || echo FAIL) part2(port-trace)=$([ $P2 -eq 0 ] && echo PASS || echo FAIL) part3(depth,rss lo=${LO_RSS}KB hi=${HI_RSS}KB)=$([ $P3 -eq 0 ] && echo PASS || echo FAIL)"
[ $P1 -eq 0 ] && [ $P2 -eq 0 ] && [ $P3 -eq 0 ] && { echo "GATE PASS(0) [pl_rung11_lco]: rung 11 (LCO) DONE-WHEN satisfied"; exit 0; }
echo "GATE FAIL(1) [pl_rung11_lco]: rung 11 (LCO) DONE-WHEN not satisfied"
exit 1
