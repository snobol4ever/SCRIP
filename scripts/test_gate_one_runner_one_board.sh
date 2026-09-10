#!/bin/bash
# test_gate_one_runner_one_board.sh -- ONE RUNNER, ONE BOARD (Lon 2026-09-10 16:3x CDT; RULES.md § FACT RULES; CEO-523).
# Hermetic, no build, ~1s. ARMS: (1) DETECTOR: a non-coo seat is REFUSED rc=2 with the ruling named; (2) the coo passes rc=0;
# (3) the bus computed done (S4E_DONE_WHEN_RUN=1) passes and says so; (4) a loud override passes and prints its reason; (5) --check
# exits 2 for a non-coo seat and 0 for the coo; (6) CENSUS: every runner named in scripts/one_runner_boards.txt sources the guard on
# line 2 and corpus_suite_harness.py calls _one_runner_guard() first in cmd_run. Run it as a script, never pasted (CEO-480).
set -u
H="$(cd "$(dirname "$0")" && pwd)"; L="$H/lib_one_runner.sh"; G=one_runner_one_board; fail=0; examined=0
[ -r "$L" ] || { echo "REFUSE(2) [$G]: $L missing -- cannot measure"; exit 2; }
arm() { examined=$((examined+1)); if eval "$2"; then echo "  ok   $1"; else echo "  FAIL $1"; fail=$((fail+1)); fi; }
arm "1 detector: seat hq_B refused rc=2 naming the ruling" 'out=$(S4E_SEAT=hq_B bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); rc=$?; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out" && grep -q "test_x_suite.sh" <<<"$out"'
arm "2 the coo passes rc=0 silently" 'out=$(S4E_SEAT=coo bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "3 the bus computed done is exempt and says so" 'out=$(S4E_SEAT=hq_B S4E_DONE_WHEN_RUN=1 bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); [ $? -eq 0 ] && grep -q "computed done" <<<"$out"'
arm "4 a loud override passes and prints its reason" 'out=$(S4E_SEAT=cto S4E_ONE_RUNNER_OVERRIDE="ceo audit CEO-999" bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); [ $? -eq 0 ] && grep -q "OVERRIDE by cto" <<<"$out" && grep -q "CEO-999" <<<"$out"'
arm "5 --check: 2 for hq_B, 0 for coo" 'S4E_SEAT=hq_B bash "$L" --check; a=$?; S4E_SEAT=coo bash "$L" --check; b=$?; [ $a -eq 2 ] && [ $b -eq 0 ]'
arm "6a census: every listed board runner sources the guard on line 2" 'miss=""; while read -r b; do [ -n "$b" ] || continue; sed -n 2p "$H/$b" | grep -q "one_runner_guard" || miss="$miss $b"; done < "$H/one_runner_boards.txt"; [ -z "$miss" ] || { echo "     missing:$miss"; false; }'
arm "6b census: the master harness guards cmd_run first" 'grep -A1 "^def cmd_run(args):" "$H/corpus_suite_harness.py" | grep -q "_one_runner_guard()"'
arm "6c the bus done run exports S4E_DONE_WHEN_RUN=1" 'grep -q "S4E_DONE_WHEN_RUN=1 timeout" "$H/s4e_msg.sh"'
arm "7 harness detector: a non-coo master run is refused rc=2 before any grading" 'out=$(cd "$H/.." && S4E_SEAT=hq_B python3 scripts/corpus_suite_harness.py run /nonexistent.icn /nonexistent.ref --lang icon --by-modes-column 2>&1); rc=$?; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
echo "$G: examined=$examined fail=$fail"
[ $examined -ge 9 ] || { echo "REFUSE(2) [$G]: examined=$examined below the 9 declared arms"; exit 2; }
[ $fail -eq 0 ] && { echo "GATE PASS(0) [$G]: $examined/$examined -- one runner (the coo), one board"; exit 0; }
echo "GATE FAIL(1) [$G]: $fail of $examined arms red"; exit 1
