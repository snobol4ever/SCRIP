#!/bin/bash
# test_gate_one_runner_one_board.sh -- ONE RUNNER, ONE BOARD (Lon 2026-09-10 16:3x CDT; RULES.md § FACT RULES; CEO-523).
# Hermetic, no build, ~1s. ARMS: (1) DETECTOR: a non-coo seat is REFUSED rc=2 with the ruling named; (2) the coo passes rc=0;
# (3) the bus computed done (S4E_DONE_WHEN_RUN=1) passes and says so; (4) a loud override passes and prints its reason; (5) --check
# exits 2 for a non-coo seat and 0 for the coo; (6) CENSUS: every runner named in scripts/one_runner_boards.txt sources the guard on
# line 2 and corpus_suite_harness.py guards cmd_run with the suite it holds; (7) a non-coo run of a CORPUS suite is refused;
# (8/10) CEO-547 part 1 DETECTOR: a suite outside the corpus tree -- a gate own mktemp fixture -- is NOT a board and is graded,
# not refused; (9/11/12) CONTROL: a corpus suite is still refused, both copies carry the narrowing, and a caller that does not say
# what it grades is refused rather than waved through. Run it as a script, never pasted (CEO-480).
set -u
H="$(cd "$(dirname "$0")" && pwd)"; L="$H/lib_one_runner.sh"; G=one_runner_one_board; fail=0; examined=0
CORPUS="${S4E_CORPUS:-${S4E_HOME:-$(cd "$H/../.." && pwd)}/corpus}"
[ -r "$CORPUS/tests/icon/ALL.icn" ] || { echo "REFUSE(2) [$G]: no corpus master at $CORPUS/tests/icon/ALL.icn -- the board/not-a-board arms cannot be measured"; exit 2; }
[ -r "$L" ] || { echo "REFUSE(2) [$G]: $L missing -- cannot measure"; exit 2; }
arm() { examined=$((examined+1)); if eval "$2"; then echo "  ok   $1"; else echo "  FAIL $1"; fail=$((fail+1)); fi; }
arm "1 detector: seat hq_B refused rc=2 naming the ruling" 'out=$(S4E_SEAT=hq_B bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); rc=$?; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out" && grep -q "test_x_suite.sh" <<<"$out"'
arm "2 the coo passes rc=0 silently" 'out=$(S4E_SEAT=coo bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "3 the bus computed done is exempt and says so" 'out=$(S4E_SEAT=hq_B S4E_DONE_WHEN_RUN=1 bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); [ $? -eq 0 ] && grep -q "computed done" <<<"$out"'
arm "4 a loud override passes and prints its reason" 'out=$(S4E_SEAT=cto S4E_ONE_RUNNER_OVERRIDE="ceo audit CEO-999" bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); [ $? -eq 0 ] && grep -q "OVERRIDE by cto" <<<"$out" && grep -q "CEO-999" <<<"$out"'
arm "5 --check: 2 for hq_B, 0 for coo" 'S4E_SEAT=hq_B bash "$L" --check; a=$?; S4E_SEAT=coo bash "$L" --check; b=$?; [ $a -eq 2 ] && [ $b -eq 0 ]'
arm "6a census: every listed board runner sources the guard on line 2" 'miss=""; while read -r b; do [ -n "$b" ] || continue; sed -n 2p "$H/$b" | grep -q "one_runner_guard" || miss="$miss $b"; done < "$H/one_runner_boards.txt"; [ -z "$miss" ] || { echo "     missing:$miss"; false; }'
arm "6b census: the master harness guards cmd_run before it grades, with the suite it holds" 'grep -A2 "^def cmd_run(args):" "$H/corpus_suite_harness.py" | grep -q "_one_runner_guard(args.sno, paths\\[.corpus.\\])"'
arm "6c the bus done run exports S4E_DONE_WHEN_RUN=1" 'grep -q "S4E_DONE_WHEN_RUN=1 timeout" "$H/s4e_msg.sh"'
arm "7 harness detector: a non-coo run of a CORPUS suite is refused rc=2 before any grading" 'out=$(cd "$H/.." && S4E_SEAT=hq_B python3 scripts/corpus_suite_harness.py run "$CORPUS/tests/icon/ALL.icn" "$CORPUS/tests/icon/ALL.ref" --lang icon --by-modes-column 2>&1); rc=$?; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'

# ⛔⭐ CEO-547 PART 1 -- WHAT MAKES A RUN A BOARD IS THE POPULATION IT GRADES, NOT THE ENTRY POINT. The guard used to fire at the
# top of cmd_run before it knew which it was holding, so a gate feeding the harness its own two-entry mktemp fixture was refused
# rc=2 exactly like a 763-entry corpus master -- and make test, THE blocking set, was red on a clean origin tree for twelve of
# thirteen seats. Arms 8 and 10 are the DETECTOR half (they FAIL on the un-narrowed guard) and 9 and 11 are the CONTROL half
# (they FAIL on a guard narrowed too far, which is the way this cure breaks): a real board must still be refused.
arm "8 a suite OUTSIDE the corpus tree is not a board and is not refused" 'out=$(S4E_SEAT=hq_B bash -c "source $L; one_runner_guard test_x_suite.sh /tmp/fixture_not_a_board/ALL.icn" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "9 control: a suite UNDER the corpus tree is still refused rc=2" 'out=$(S4E_SEAT=hq_B bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/icon/ALL.icn" 2>&1); [ $? -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
arm "10 harness: a gate own mktemp fixture is graded, not refused" 'T=$(mktemp -d) || exit 1; printf "* one\nline\n" > "$T/f.sno"; printf "* one\nline\n" > "$T/f.ref"; out=$(cd "$H/.." && S4E_SEAT=hq_B python3 scripts/corpus_suite_harness.py run "$T/f.sno" "$T/f.ref" --lang snobol4 2>&1); rc=$?; rm -rf "$T"; [ $rc -ne 2 ] || ! grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
arm "11 census: BOTH copies carry the narrowing, word for word is the promise in the header" 'grep -q "one_runner_suite_is_a_board" "$L" && grep -q "_suite_is_a_board" "$H/corpus_suite_harness.py"'
arm "12 a caller that does not say what it grades is still refused, never waved through" 'out=$(S4E_SEAT=hq_B bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); [ $? -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
echo "$G: examined=$examined fail=$fail"
[ $examined -ge 14 ] || { echo "REFUSE(2) [$G]: examined=$examined below the 14 declared arms"; exit 2; }
[ $fail -eq 0 ] && { echo "GATE PASS(0) [$G]: $examined/$examined -- one runner (the coo), one board"; exit 0; }
echo "GATE FAIL(1) [$G]: $fail of $examined arms red"; exit 1
