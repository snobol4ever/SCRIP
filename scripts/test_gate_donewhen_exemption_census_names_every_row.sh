#!/usr/bin/env bash
# test_gate_donewhen_exemption_census_names_every_row.sh -- THE DONE-WHEN EXEMPTION CENSUS, PINNED BOTH WAYS (ceo CEO-967, on the coo's
# COO-116 report; row instruments-the-donewhen-exemption-census-names-every-criterion-that-measures-its-one-runner-exemption). The bus
# runs every computed DONE-WHEN under S4E_DONE_WHEN_RUN=1, a ONE-RUNNER exemption, so a criterion that runs the guard or a guarded
# runner can be measuring the exemption rather than its subject. util_donewhen_exemption_census.py names each such baton with its owner,
# its mechanism and its class (GUARD, BOARD) and counts the ones that clear the exemption (CLEAR).
# ARMS on a hermetic fixture tasks tree and QUEUE: (1) a live baton running a guarded board is named BOARD with its owner, and one reading
# the guard is named GUARD, rc 1; (2) a baton that clears the exemption first is counted CLEAR and never named; (3) a DONE baton is not
# named by default and is with --all; (4) the other direction: once both exposed criteria clear the exemption the census reads rc 0 with
# 0 exposed; (5) an unreadable tasks tree and an unreadable QUEUE each REFUSE rc 2, never a zero; (6) on the live postoffice it measures
# (rc 0 or 1) and prints its population beside the rc.
# ⛔ FAILED ONCE: before this landing there was no census, and the row's DONE-WHEN read "No such file".
# EXIT 0 every arm holds; 1 an arm is red; 2 REFUSED.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; C="$HERE/util_donewhen_exemption_census.py"
[ -f "$C" ] || { echo "REFUSED(2): $C missing"; exit 2; }
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
fails=0; checks=0
ck() { checks=$((checks+1)); if [ "$1" = ok ]; then printf '  ok    %s\n' "$2"; else printf '  FAIL  %s\n' "$2"; fails=$((fails+1)); fi; }
mk() { mkdir -p "$T/tasks"; printf '# TASK %s\nGOAL: fixture\nDONE-WHEN: %s\n## NEXT\nx\n' "$1" "$2" > "$T/tasks/$1.task.md"; }
mk exposed-board 'cd "$S4E_HOME/SCRIP" && bash scripts/test_icon_jcon_suite.sh | grep -q "m3_pass=91"'
mk exposed-guard 'cd "$S4E_HOME/SCRIP" && bash scripts/lib_one_runner.sh --check test_icon_jcon_suite.sh'
mk clear-board 'cd "$S4E_HOME/SCRIP" && env -u S4E_DONE_WHEN_RUN bash scripts/test_icon_jcon_suite.sh'
mk plain-gate 'cd "$S4E_HOME/SCRIP" && bash scripts/test_gate_our_files_are_lf.sh'
mk done-board 'cd "$S4E_HOME/SCRIP" && bash scripts/test_icon_arizona_suite.sh'
printf '# fixture\n0\texposed-board\thq_icon\tFREE\n0\texposed-guard\tcoo\tASSIGNED:coo\n1\tclear-board\thq_icon\tFREE\n1\tplain-gate\tcoo\tFREE\n2\tdone-board\thq_icon\tDONE\n' > "$T/QUEUE.tsv"
run() { S4E_TASKS="$T/tasks" S4E_QUEUE="$T/QUEUE.tsv" python3 "$C" "$@"; }
out="$(run 2>&1)"; rc=$?
[ "$rc" = 1 ] && grep -qE '^  BOARD +hq_icon .*exposed-board +<- test_icon_jcon_suite\.sh' <<<"$out" && grep -qE '^  GUARD +coo .*exposed-guard +<- lib_one_runner\.sh' <<<"$out" \
  && grep -q 'exposed and LIVE: 2$' <<<"$out" && ck ok "(1) a live board criterion is named BOARD with its owner and a guard criterion GUARD, rc 1, 2 exposed" || ck no "(1) rc=$rc :: $(tr '\n' '|' <<<"$out" | cut -c1-300)"
! grep -q 'clear-board\|plain-gate' <<<"$out" && grep -q 'CLEAR 1;' <<<"$out" && ck ok "(2) a criterion that clears the exemption first is counted CLEAR and never named; a plain gate is not counted" || ck no "(2) $(tr '\n' '|' <<<"$out" | cut -c1-300)"
all="$(run --all 2>&1)"
! grep -q 'done-board' <<<"$out" && grep -qE 'BOARD +hq_icon +DONE .*done-board' <<<"$all" && ck ok "(3) a DONE baton is not named by default and is with --all" || ck no "(3) default/all: $(grep -c done-board <<<"$out")/$(grep -c done-board <<<"$all")"
mk exposed-board 'cd "$S4E_HOME/SCRIP" && env -u S4E_DONE_WHEN_RUN bash scripts/test_icon_jcon_suite.sh | grep -q "m3_pass=91"'
mk exposed-guard 'cd "$S4E_HOME/SCRIP" && S4E_DONE_WHEN_RUN= bash scripts/lib_one_runner.sh --check test_icon_jcon_suite.sh'
out4="$(run 2>&1)"; rc4=$?
[ "$rc4" = 0 ] && grep -q 'exposed and LIVE: 0$' <<<"$out4" && grep -q 'CLEAR 3;' <<<"$out4" && ck ok "(4) the other direction: once both criteria clear the exemption the census reads rc 0 with 0 exposed (CLEAR 3)" || ck no "(4) rc=$rc4 :: $(tail -1 <<<"$out4" | cut -c1-200)"
S4E_TASKS=/nonexistent-tasks-dir python3 "$C" --json > /dev/null 2>&1; r5a=$?
S4E_TASKS="$T/tasks" S4E_QUEUE=/nonexistent-queue.tsv python3 "$C" > /dev/null 2>&1; r5b=$?
[ "$r5a" = 2 ] && [ "$r5b" = 2 ] && ck ok "(5) an unreadable tasks tree and an unreadable QUEUE each REFUSE rc 2 -- never a dark zero" || ck no "(5) unreadable tasks rc=$r5a, unreadable QUEUE rc=$r5b"
live="$(python3 "$C" 2>&1)"; r6=$?
if [ "$r6" = 2 ]; then echo "⛔ REFUSED(2): the live postoffice could not be read: $(tail -1 <<<"$live" | cut -c1-160)"; exit 2; fi
{ [ "$r6" = 0 ] || [ "$r6" = 1 ]; } && grep -qE '^population: [1-9][0-9]* baton' <<<"$live" && ck ok "(6) on the live postoffice: $(tail -1 <<<"$live" | cut -c13-200) (rc $r6)" || ck no "(6) the live census: rc=$r6 $(tail -1 <<<"$live" | cut -c1-160)"
echo "population: $checks arm(s) -- a 5-baton fixture both ways, two refusal shapes, and the live tasks tree"
if [ "$fails" = 0 ]; then echo "GATE PASS(0) [donewhen_exemption_census_names_every_row]: $checks of $checks arms hold"; exit 0; fi
echo "⛔ GATE FAIL(1) [donewhen_exemption_census_names_every_row]: $fails of $checks arms red"; exit 1
