#!/bin/bash
# test_gate_one_runner_one_board.sh -- ONE RUNNER PER LANGUAGE, ONE BOARD (Lon 2026-09-10 CEO-523, partitioned by language on Lon's
# word 2026-09-16 10:5x "do not have a centralized runner at all", ceo CEO-775). Hermetic, no build, ~1s. ARMS: (1) DETECTOR: a seat
# the board's LANE does not name is REFUSED rc=2 with the ruling named (a legacy HQ, the coo, and a WRONG-language HQ, three arms);
# (2) the language HQ the LANES line names passes rc=0 silently, and the cfo on a rebus board; (3) the bus computed done
# (S4E_DONE_WHEN_RUN=1) passes and says so; (4) a loud override passes and prints its reason; (5) --check <board> exits 2 for a
# wrong seat and 0 for the lane owner; (6) CENSUS: every runner named in scripts/one_runner_boards.txt sources the guard on
# line 2 and corpus_suite_harness.py guards cmd_run with the suite it holds; (7) a non-coo run of a CORPUS suite is refused;
# (8/10) CEO-547 part 1 DETECTOR: a suite outside the corpus tree -- a gate own mktemp fixture -- is NOT a board and is graded,
# not refused; (9/11/12) CONTROL: a corpus suite is still refused, both copies carry the narrowing, and a caller that does not say
# what it grades is refused rather than waved through. Run it as a script, never pasted (CEO-480).
set -u
# ⛔⭐ THE GATE PINS ITS OWN ENVIRONMENT, AND THIS IS THE INSTRUMENT LAW NOT A TIDY-UP (found by running this gate under
# `s4e_msg.sh done`, cto 2026-09-11). The bus exports S4E_DONE_WHEN_RUN=1 around a computed DONE-WHEN, and that is an EXEMPTION
# from the very guard six of these arms exist to measure -- so the gate read 6 of 14 red from inside a `done` and 0 of 14 red from
# a bare shell, ON THE SAME TREE. A verdict that depends on who called it measures the caller, not the subject. Every arm that
# asserts a REFUSAL is one of the arms that flipped, so the failure ran in the flattering direction for the seat invoking it.
# Arms 3 and 4 set the exemption they test, explicitly, one arm at a time.
unset S4E_DONE_WHEN_RUN S4E_ONE_RUNNER_OVERRIDE S4E_SEAT
H="$(cd "$(dirname "$0")" && pwd)"; L="$H/lib_one_runner.sh"; G=one_runner_one_board; fail=0; examined=0
CORPUS="${S4E_CORPUS:-${S4E_HOME:-$(cd "$H/../.." && pwd)}/corpus}"
[ -r "$CORPUS/tests/icon/ALL.icn" ] || { echo "REFUSE(2) [$G]: no corpus master at $CORPUS/tests/icon/ALL.icn -- the board/not-a-board arms cannot be measured"; exit 2; }
[ -r "$L" ] || { echo "REFUSE(2) [$G]: $L missing -- cannot measure"; exit 2; }
arm() { examined=$((examined+1)); if eval "$2"; then echo "  ok   $1"; else echo "  FAIL $1"; fail=$((fail+1)); fi; }
arm "1a detector: legacy seat hq_B refused rc=2 on an icon board, naming the ruling" 'out=$(S4E_SEAT=hq_B bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); rc=$?; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out" && grep -q "test_icon_x_suite.sh" <<<"$out"'
arm "1b detector: the coo is refused rc=2 on an icon board -- there is no central runner" 'out=$(S4E_SEAT=coo bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 2 ] && grep -q "ONE RUNNER PER LANGUAGE" <<<"$out"'
arm "1c detector: hq_pascal is refused rc=2 on an icon board -- a wrong-language HQ" 'out=$(S4E_SEAT=hq_pascal bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 2 ] && grep -q "hq_icon" <<<"$out"'
arm "2a the lane owner (LANES: icon=hq_icon) passes rc=0 silently on an icon board" 'out=$(S4E_SEAT=hq_icon bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "2b the owner is read from the LANES line, not baked in: cfo passes on a rebus master path" 'out=$(S4E_SEAT=cfo bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/rebus/ALL.reb" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "2c control: hq_icon is refused rc=2 on that same rebus master path" 'out=$(S4E_SEAT=hq_icon bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/rebus/ALL.reb" 2>&1); [ $? -eq 2 ]'
arm "3 the bus computed done is exempt and says so" 'out=$(S4E_SEAT=hq_B S4E_DONE_WHEN_RUN=1 bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 0 ] && grep -q "computed done" <<<"$out"'
arm "4 a loud override passes and prints its reason" 'out=$(S4E_SEAT=cto S4E_ONE_RUNNER_OVERRIDE="ceo audit CEO-999" bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 0 ] && grep -q "OVERRIDE by cto" <<<"$out" && grep -q "CEO-999" <<<"$out"'
arm "5 --check <board>: 2 for hq_B and for coo on an icon board, 0 for hq_icon" 'S4E_SEAT=hq_B bash "$L" --check test_icon_x_suite.sh; a=$?; S4E_SEAT=coo bash "$L" --check test_icon_x_suite.sh; c=$?; S4E_SEAT=hq_icon bash "$L" --check test_icon_x_suite.sh; b=$?; [ $a -eq 2 ] && [ $c -eq 2 ] && [ $b -eq 0 ]'
arm "6a census: every listed board runner sources the guard on line 2 and calls it (raku_roast_scoreboard calls it after argument parsing, hq_T 09-14)" 'miss=""; while read -r b; do [ -n "$b" ] || continue; { sed -n 2p "$H/$b" | grep -q "lib_one_runner.sh" && grep -q "one_runner_guard" "$H/$b"; } || miss="$miss $b"; done < "$H/one_runner_boards.txt"; [ -z "$miss" ] || { echo "     missing:$miss"; false; }'
arm "6b census: the master harness guards cmd_run before it grades, with the suite it holds" 'grep -A2 "^def cmd_run(args):" "$H/corpus_suite_harness.py" | grep -q "_one_runner_guard(args.sno, paths\\[.corpus.\\], getattr(args, .lang., None))"'
arm "6c the bus done run exports S4E_DONE_WHEN_RUN=1" 'grep -q "S4E_DONE_WHEN_RUN=1 timeout" "$H/s4e_msg.sh"'
arm "7 harness detector: a wrong-language run of a CORPUS suite is refused rc=2 before any grading" 'out=$(cd "$H/.." && S4E_SEAT=hq_pascal python3 scripts/corpus_suite_harness.py run "$CORPUS/tests/icon/ALL.icn" "$CORPUS/tests/icon/ALL.ref" --lang icon --by-modes-column 2>&1); rc=$?; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'

# ⛔⭐ CEO-547 PART 1 -- WHAT MAKES A RUN A BOARD IS THE POPULATION IT GRADES, NOT THE ENTRY POINT. The guard used to fire at the
# top of cmd_run before it knew which it was holding, so a gate feeding the harness its own two-entry mktemp fixture was refused
# rc=2 exactly like a 763-entry corpus master -- and make test, THE blocking set, was red on a clean origin tree for twelve of
# thirteen seats. Arms 8 and 10 are the DETECTOR half (they FAIL on the un-narrowed guard) and 9 and 11 are the CONTROL half
# (they FAIL on a guard narrowed too far, which is the way this cure breaks): a real board must still be refused.
arm "8 a suite OUTSIDE the corpus tree is not a board and is not refused" 'out=$(S4E_SEAT=hq_pascal bash -c "source $L; one_runner_guard test_x_suite.sh /tmp/fixture_not_a_board/ALL.icn" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "9 control: a suite UNDER the corpus tree is still refused rc=2 to a seat its lane does not name" 'out=$(S4E_SEAT=hq_pascal bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/icon/ALL.icn" 2>&1); [ $? -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
arm "10 harness: a gate own mktemp fixture is graded, not refused" 'T=$(mktemp -d) || exit 1; printf "* one\nline\n" > "$T/f.sno"; printf "* one\nline\n" > "$T/f.ref"; out=$(cd "$H/.." && S4E_SEAT=hq_B python3 scripts/corpus_suite_harness.py run "$T/f.sno" "$T/f.ref" --lang snobol4 2>&1); rc=$?; rm -rf "$T"; [ $rc -ne 2 ] || ! grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
arm "11 census: BOTH copies carry the narrowing, word for word is the promise in the header" 'grep -q "one_runner_suite_is_a_board" "$L" && grep -q "_suite_is_a_board" "$H/corpus_suite_harness.py"'
arm "12 a caller that does not say what it grades (no language in its name, no suite path) is still refused, never waved through" 'out=$(S4E_SEAT=hq_icon bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); [ $? -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
arm "13 this gate pins its own environment, so a computed done cannot flip its verdict" 'grep -q "^unset S4E_DONE_WHEN_RUN S4E_ONE_RUNNER_OVERRIDE S4E_SEAT$" "$H/test_gate_one_runner_one_board.sh"'
echo "$G: examined=$examined fail=$fail"
[ $examined -ge 19 ] || { echo "REFUSE(2) [$G]: examined=$examined below the 19 declared arms"; exit 2; }
[ $fail -eq 0 ] && { echo "GATE PASS(0) [$G]: $examined/$examined -- one runner PER LANGUAGE, one board"; exit 0; }
echo "GATE FAIL(1) [$G]: $fail of $examined arms red"; exit 1
