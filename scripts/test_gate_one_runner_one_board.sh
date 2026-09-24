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
unset S4E_DONE_WHEN_RUN S4E_ONE_RUNNER_OVERRIDE S4E_ONE_RUNNER_FIXTURE S4E_SEAT
H="$(cd "$(dirname "$0")" && pwd)"; L="$H/lib_one_runner.sh"; G=one_runner_one_board; fail=0; examined=0
CORPUS="${S4E_CORPUS:-${S4E_HOME:-$(cd "$H/../.." && pwd)}/corpus}"
[ -r "$CORPUS/tests/icon/ALL.icn" ] || { echo "REFUSE(2) [$G]: no corpus master at $CORPUS/tests/icon/ALL.icn -- the board/not-a-board arms cannot be measured"; exit 2; }
[ -r "$L" ] || { echo "REFUSE(2) [$G]: $L missing -- cannot measure"; exit 2; }
arm() { examined=$((examined+1)); if eval "$2"; then echo "  ok   $1"; else echo "  FAIL $1"; fail=$((fail+1)); fi; }
# ⛔⭐ THE OWNERS ARE READ FROM THE SUBJECT'S OWN AUTHORITY, NOT NAMED HERE (COO-80, 2026-09-19, row
# instrument-the-nineteen-non-gc-blocking-arms). Arms 1c, 2a, 2b and 5 used to spell the owners out --
# icon=hq_icon, rebus=cfo -- and they were true when they were typed. lib_one_runner.sh reads the owner from
# MODE's LANES: line (one_runner_who), the LANES line moved to a single owner for every language under the
# consolidated modes, and those four arms went RED WITH THE GUARD BEHAVING EXACTLY AS RULED. That is the most
# expensive kind of red there is, and this gate's sister states the principle best: a fixture that restates
# its subject's facts does not fail when the subject is wrong, it fails when the subject is RIGHT and has
# moved -- it points at the cure and calls it the bug. ⭐ SO THE ARMS ASK one_runner_who, exactly as the
# runners do, and derive a NON-owner seat for every refusal arm rather than assuming one. The gate still
# grades what it always graded -- the named owner passes silently, everyone else is refused rc=2 with the
# ruling named -- and it now survives every future lane cut without an edit.
_who() { bash -c "source \"$L\"; one_runner_who \"$1\"" 2>/dev/null; }
OWN_ICON="$(_who icon)"; OWN_REBUS="$(_who rebus)"
[ -n "$OWN_ICON" ]  || { echo "REFUSE(2) [$G]: MODE LANES: names no owner for icon, so the owner-passes arms cannot be built. That is a finding about the LANES line, not a broken fixture -- report it rather than lowering the bar."; exit 2; }
[ -n "$OWN_REBUS" ] || { echo "REFUSE(2) [$G]: MODE LANES: names no owner for rebus, so the second-language arm cannot be built. That is a finding about the LANES line, not a broken fixture."; exit 2; }
# A refusal arm needs a seat the lane does NOT name. Derive one instead of trusting that any particular name
# is still a non-owner: under a one-owner table most names are, but WHICH ones depends on the live line.
_not_owner_of() { local _own="$1" _c; for _c in hq_pascal hq_prolog hq_icon hq_B cto cfo coo; do [ "$_c" = "$_own" ] || { printf '%s' "$_c"; return 0; }; done; return 1; }
NOT_ICON="$(_not_owner_of "$OWN_ICON")"   || { echo "REFUSE(2) [$G]: every candidate seat owns icon -- no refusal arm can be built"; exit 2; }
NOT_REBUS="$(_not_owner_of "$OWN_REBUS")" || { echo "REFUSE(2) [$G]: every candidate seat owns rebus -- no refusal arm can be built"; exit 2; }
# arm 5 drives two refused seats and the owner; both refused seats must really not own icon.
REF_A=hq_B; [ "$REF_A" = "$OWN_ICON" ] && REF_A="$NOT_ICON"
REF_B=coo;  [ "$REF_B" = "$OWN_ICON" ] && REF_B=cto
echo "    lanes (read from MODE LANES: via one_runner_who): icon -> $OWN_ICON, rebus -> $OWN_REBUS; refusal seats: $NOT_ICON (icon), $NOT_REBUS (rebus), $REF_A + $REF_B (--check)"
arm "1a detector: legacy seat hq_B refused rc=2 on an icon board, naming the ruling" 'out=$(S4E_SEAT=hq_B bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); rc=$?; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out" && grep -q "test_icon_x_suite.sh" <<<"$out"'
arm "1b detector: the coo is refused rc=2 on an icon board -- there is no central runner" 'out=$(S4E_SEAT=coo bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 2 ] && grep -q "ONE RUNNER PER LANGUAGE" <<<"$out"'
arm "1c detector: $NOT_ICON is refused rc=2 on an icon board -- a seat the icon lane does not name" 'out=$(S4E_SEAT=$NOT_ICON bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 2 ] && grep -q "$OWN_ICON" <<<"$out"'
arm "2a the lane owner the LANES line names for icon ($OWN_ICON) passes rc=0 silently on an icon board" 'out=$(S4E_SEAT=$OWN_ICON bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "2b the owner is read from the LANES line, not baked in: the rebus owner $OWN_REBUS passes on a rebus master path" 'out=$(S4E_SEAT=$OWN_REBUS bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/rebus/ALL.reb" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "2c control: $NOT_REBUS is refused rc=2 on that same rebus master path" 'out=$(S4E_SEAT=$NOT_REBUS bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/rebus/ALL.reb" 2>&1); [ $? -eq 2 ]'
arm "2d a BARE language directory is a suite path: the rebus owner $OWN_REBUS passes on $CORPUS/benchmarks/rebus with nothing below it (it printed the whole path as the language and refused the owner too, coo 2026-09-23)" 'out=$(S4E_SEAT=$OWN_REBUS bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/benchmarks/rebus" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "2e control: $NOT_REBUS is refused rc=2 on that bare directory, naming the rebus owner, and no sed error reaches the reader" 'out=$(S4E_SEAT=$NOT_REBUS bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/benchmarks/rebus" 2>&1); [ $? -eq 2 ] && printf "%s" "$out" | grep -q "is not $OWN_REBUS" && ! printf "%s" "$out" | grep -q "unknown option"'
arm "3 the bus computed done is exempt and says so" 'out=$(S4E_SEAT=hq_B S4E_DONE_WHEN_RUN=1 bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 0 ] && grep -q "computed done" <<<"$out"'
arm "4 a loud override passes and prints its reason" 'out=$(S4E_SEAT=cto S4E_ONE_RUNNER_OVERRIDE="ceo audit CEO-999" bash -c "source $L; one_runner_guard test_icon_x_suite.sh" 2>&1); [ $? -eq 0 ] && grep -q "OVERRIDE by cto" <<<"$out" && grep -q "CEO-999" <<<"$out"'
arm "5 --check <board>: 2 for $REF_A and for $REF_B on an icon board, 0 for the lane owner $OWN_ICON" 'S4E_SEAT=$REF_A bash "$L" --check test_icon_x_suite.sh; a=$?; S4E_SEAT=$REF_B bash "$L" --check test_icon_x_suite.sh; c=$?; S4E_SEAT=$OWN_ICON bash "$L" --check test_icon_x_suite.sh; b=$?; [ $a -eq 2 ] && [ $c -eq 2 ] && [ $b -eq 0 ]'
arm "6a census: every listed board runner sources the guard on line 2 and calls it (raku_roast_scoreboard calls it after argument parsing, hq_T 09-14)" 'miss=""; while read -r b; do [ -n "$b" ] || continue; { sed -n 2p "$H/$b" | grep -q "lib_one_runner.sh" && grep -q "one_runner_guard" "$H/$b"; } || miss="$miss $b"; done < "$H/one_runner_boards.txt"; [ -z "$miss" ] || { echo "     missing:$miss"; false; }'
arm "6b census: the master harness guards cmd_run before it grades, with the suite it holds" 'grep -A2 "^def cmd_run(args):" "$H/corpus_suite_harness.py" | grep -q "_one_runner_guard(args.sno, paths\\[.corpus.\\], getattr(args, .lang., None))"'
arm "6c the bus done run exports S4E_DONE_WHEN_RUN=1" 'grep -q "S4E_DONE_WHEN_RUN=1 timeout" "$H/s4e_msg.sh"'
arm "7 harness detector: a wrong-language run of a CORPUS suite is refused rc=2 before any grading" 'out=$(cd "$H/.." && S4E_SEAT=hq_pascal python3 scripts/corpus_suite_harness.py run "$CORPUS/tests/icon/ALL.icn" "$CORPUS/tests/icon/ALL.ref" --lang icon 2>&1); rc=$?; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'

# ⛔⭐ CEO-547 PART 1 -- WHAT MAKES A RUN A BOARD IS THE POPULATION IT GRADES, NOT THE ENTRY POINT. The guard used to fire at the
# top of cmd_run before it knew which it was holding, so a gate feeding the harness its own two-entry mktemp fixture was refused
# rc=2 exactly like a 763-entry corpus master -- and make test, THE blocking set, was red on a clean origin tree for twelve of
# thirteen seats. Arms 8 and 10 are the DETECTOR half (they FAIL on the un-narrowed guard) and 9 and 11 are the CONTROL half
# (they FAIL on a guard narrowed too far, which is the way this cure breaks): a real board must still be refused.
arm "8 a suite OUTSIDE the corpus tree is not a board and is not refused" 'out=$(S4E_SEAT=hq_pascal bash -c "source $L; one_runner_guard test_x_suite.sh /tmp/fixture_not_a_board/ALL.icn" 2>&1); [ $? -eq 0 ] && [ -z "$out" ]'
arm "9 control: a suite UNDER the corpus tree is still refused rc=2 to a seat its lane does not name" 'out=$(S4E_SEAT=hq_pascal bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/icon/ALL.icn" 2>&1); [ $? -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
arm "10 harness: a gate own mktemp fixture is graded, not refused" 'T=$(mktemp -d) || exit 1; printf "* one\nline\n" > "$T/f.sno"; printf "* one\nline\n" > "$T/f.ref"; out=$(cd "$H/.." && S4E_SEAT=hq_B python3 scripts/corpus_suite_harness.py run "$T/f.sno" "$T/f.ref" --lang snobol4 2>&1); rc=$?; rm -rf "$T"; [ $rc -ne 2 ] || ! grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
arm "11 census: BOTH copies carry the narrowing, word for word is the promise in the header" 'grep -q "one_runner_suite_is_a_board" "$L" && grep -q "_suite_is_a_board" "$H/corpus_suite_harness.py" && grep -q "one_runner_in_a_shared_checkout" "$L" && grep -q "def _in_a_shared_checkout" "$H/corpus_suite_harness.py"'
arm "12 a caller that does not say what it grades (no language in its name, no suite path) is still refused, never waved through" 'out=$(S4E_SEAT=hq_icon bash -c "source $L; one_runner_guard test_x_suite.sh" 2>&1); [ $? -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
arm "13 this gate pins its own environment, so a computed done cannot flip its verdict" 'grep -q "^unset S4E_DONE_WHEN_RUN S4E_ONE_RUNNER_OVERRIDE S4E_ONE_RUNNER_FIXTURE S4E_SEAT$" "$H/test_gate_one_runner_one_board.sh"'
# ⛔⭐ OUTSIDE THE CONFIGURED ROOT IS NOT OUTSIDE THE SHARED CORPUS (coo 2026-09-23, CEO-1229's runner wiring). The root is whatever
# S4E_CORPUS or S4E_HOME say, so pointing either at an EXISTING scratch directory made a real corpus suite read "outside the corpus
# tree, not a board": measured on origin 2352905c5, hq_pascal was admitted rc=0 to tests/icon/ALL.icn under S4E_CORPUS=<scratch>, and
# the harness under S4E_HOME=<scratch> went on to grade with no refusal. Both arms point at the suite's own checkout; the harness arm
# names a suite that does not exist, so on a guard without the fix it stops on the missing file -- it never grades a real board.
arm "14 a suite inside the shared checkout is a board even when S4E_CORPUS names an existing scratch root" 'E=$(mktemp -d) || exit 1; out=$(S4E_SEAT=hq_pascal S4E_CORPUS="$E" bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/icon/ALL.icn" 2>&1); rc=$?; rm -rf "$E"; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
arm "15 harness: the same under S4E_HOME=<existing scratch> -- refused rc=2 by the guard, not by a missing file" 'E=$(mktemp -d) || exit 1; out=$(cd "$H/.." && S4E_SEAT=hq_pascal S4E_HOME="$E" SCRIP="$H/../scrip" RT_DIR="$H/../out" timeout 60 python3 scripts/corpus_suite_harness.py run "$CORPUS/tests/icon/NO_SUCH_SUITE.icn" "$CORPUS/tests/icon/NO_SUCH_SUITE.ref" --lang icon 2>&1); rc=$?; rm -rf "$E"; [ $rc -eq 2 ] && grep -q "ONE RUNNER, ONE BOARD" <<<"$out"'
# ⛔⭐ ONE SEAT, ONE LANGUAGE (Lon 2026-09-24, verbatim: "Just have each seat run only their own test suites." / "We can not have all
# 5 HQ's deciding to run all seven test suites simultaneously. Fix that."): a language HQ on another language's board is refused
# EVEN with the override and EVEN under a computed done -- both copies of the guard. Arms 4 and 3 above keep the officer override and
# the done exemption alive for seats that are not language HQs (cto, the legacy hq_B).
arm "16 a language HQ ($OWN_ICON) is refused rc=2 on a rebus board even with the override AND a computed done set" 'out=$(S4E_SEAT=$OWN_ICON S4E_ONE_RUNNER_OVERRIDE="probe" S4E_DONE_WHEN_RUN=1 bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/rebus/ALL.reb" 2>&1); [ $? -eq 2 ] && grep -q "ONE SEAT, ONE LANGUAGE" <<<"$out"'
arm "17 harness: the same language HQ with the override is refused rc=2 by the guard before any grading" 'out=$(cd "$H/.." && S4E_SEAT=$OWN_ICON S4E_ONE_RUNNER_OVERRIDE="probe" python3 scripts/corpus_suite_harness.py run "$CORPUS/tests/rebus/NO_SUCH_SUITE.reb" "$CORPUS/tests/rebus/NO_SUCH_SUITE.ref" --lang rebus --modes m3 2>&1); [ $? -eq 2 ] && grep -q "ONE SEAT, ONE LANGUAGE" <<<"$out"'
arm "18 a GATE's instrument fixture (S4E_ONE_RUNNER_FIXTURE) still admits a language HQ -- the blocking set's slices are not a seat's board run" 'out=$(S4E_SEAT=$OWN_ICON S4E_ONE_RUNNER_FIXTURE="probe fixture" bash -c "source $L; one_runner_guard test_x_suite.sh $CORPUS/tests/rebus/ALL.reb" 2>&1); [ $? -eq 0 ] && grep -q "ONE-RUNNER FIXTURE" <<<"$out"'
echo "$G: examined=$examined fail=$fail"
[ $examined -ge 26 ] || { echo "REFUSE(2) [$G]: examined=$examined below the 26 declared arms"; exit 2; }
[ $fail -eq 0 ] && { echo "GATE PASS(0) [$G]: $examined/$examined -- one runner PER LANGUAGE, one board"; exit 0; }
echo "GATE FAIL(1) [$G]: $fail of $examined arms red"; exit 1
