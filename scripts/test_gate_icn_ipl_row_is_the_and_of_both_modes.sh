#!/usr/bin/env bash
# test_gate_icn_ipl_row_is_the_and_of_both_modes.sh -- the IPL SCORE row is the AND of both modes per program (ceo CEO-627).
#
# MEASURED 2026-09-12 on 146d027e7: test_icon_ipl_suite.sh published --suite-pass "$M3_RUN_PASS" alone, so a run that read
# m3 162/162 · m4 161/162 and exited rc=1 on that mode-4 red still wrote "162/162 ✅ done" to SCORE.md and SUITES.tsv. The
# sibling runners (arizona, jcon) publish gate_and_per_program over both modes (CEO-545): a program is green only if BOTH
# modes are. Arms: (1) the IPL write passes $AND_PASS with --modes m3,m4 and never a single mode's count; (2) AND_PASS is
# derived by gate_and_per_program from both modes' FAIL, CRASH and HANG names; (3) the helper itself lowers the row for a red
# in one mode only, counts a program red in both modes once, and publishes the full population when nothing is red.
# Fail-once: IPL_RUNNER=<a pre-cure copy of the runner> bash scripts/test_gate_icn_ipl_row_is_the_and_of_both_modes.sh -> rc=1.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_gate.sh" 2>/dev/null || { echo "⛔ GATE REFUSES (rc=2): lib_gate.sh unloadable"; exit 2; }
RUNNER="${IPL_RUNNER:-$HERE/test_icon_ipl_suite.sh}"
[ -s "$RUNNER" ] || { echo "⛔ GATE REFUSES (rc=2): no runner at $RUNNER -- graded nothing"; exit 2; }
command -v gate_and_per_program >/dev/null || { echo "⛔ GATE REFUSES (rc=2): lib_gate.sh no longer defines gate_and_per_program"; exit 2; }
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = 0 ]; then echo "  OK   $2"; else echo "  FAIL $2"; fails=$((fails+1)); fi; }
write="$(awk '/util_score_row.py" write --lang icon --column vendor --suite IPL/{p=1} p{print} p&&/--text/{exit}' "$RUNNER")"
[ -n "$write" ] || { echo "⛔ GATE REFUSES (rc=2): the IPL score-row write is not in $RUNNER -- the writer moved, this gate grades nothing"; exit 2; }
printf '%s' "$write" | grep -qF -- '--suite-pass "${AND_PASS'; ck $? "ARM 1a the IPL row's --suite-pass is AND_PASS"
printf '%s' "$write" | grep -qF -- '--modes m3,m4'; ck $? "ARM 1b the IPL row declares --modes m3,m4"
printf '%s' "$write" | grep -qE -- '--suite-pass "\$M[34]_RUN_PASS"'; [ $? = 1 ]; ck $? "ARM 1c no single mode's count is published as the row"
grep -qF 'gate_and_per_program "$RUN_GRADED" "$m3_RED_NAMES" "$m4_RED_NAMES"' "$RUNNER"; ck $? "ARM 2a AND_PASS comes from gate_and_per_program over both modes"
for v in M3_RUN_FAIL_NAMES M3_RUN_CRASH_NAMES M3_RUN_HANG_NAMES; do grep -q "^m3_RED_NAMES=.*$v" "$RUNNER"; ck $? "ARM 2b the m3 reds include $v"; done
for v in M4_RUN_FAIL_NAMES M4_RUN_CRASH_NAMES M4_RUN_HANG_NAMES; do grep -q "^m4_RED_NAMES=.*$v" "$RUNNER"; ck $? "ARM 2c the m4 reds include $v"; done
read -r ap ar an <<<"$(gate_and_per_program 162 "" "ilump")"; [ "${ap:-}" = 161 ] && [ "${ar:-}" = 1 ] && [ "${an:-}" = ilump ]; ck $? "ARM 3a one mode-4 red lowers 162 to 161 (got ${ap:-} ${ar:-} ${an:-})"
read -r ap ar an <<<"$(gate_and_per_program 162 "ilump" "ilump")"; [ "${ap:-}" = 161 ]; ck $? "ARM 3b a program red in both modes counts once (got ${ap:-})"
read -r ap ar an <<<"$(gate_and_per_program 162 "" "")"; [ "${ap:-}" = 162 ]; ck $? "ARM 3c no reds publishes the full population (got ${ap:-})"
echo "IPL_ROW_AND_GATE checks=$checks fails=$fails runner=$(basename "$RUNNER")"
[ "$fails" = 0 ] || exit 1
