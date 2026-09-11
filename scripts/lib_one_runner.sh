#!/bin/bash
# lib_one_runner.sh -- ONE RUNNER, ONE BOARD (Lon 2026-09-10 16:3x CDT, in-chat to ceo, verbatim: "Quit running the boards over and over
# again. Co-ordinate with one runner and one board. Do something to alleviate the churn."; RULES.md § FACT RULES, CEO-523).
# Sourced on line 2 of every master/package board runner and called by corpus_suite_harness.py run: a board is refused (rc=2) to any
# seat but the coo, THE ONE RUNNER, who runs it once per landing batch on origin HEAD and writes the rows. Exempt: the bus's own
# computed `done`/dispatch run of a DONE-WHEN (S4E_DONE_WHEN_RUN=1, set by s4e_msg.sh, one run per closure) and a loud, named
# override (S4E_ONE_RUNNER_OVERRIDE="why", printed on the board). Identity: S4E_SEAT if set, else the ONE root-path map in
# util_score_row.derive_measurer (never a fourth copy). `bash lib_one_runner.sh --check` exits 0/2 silently for a Makefile arm to test.
# ⛔⭐ WHAT MAKES A RUN A BOARD IS THE POPULATION IT GRADES, NOT THE ENTRY POINT (ceo CEO-547 part 1, 2026-09-11, on the cfo's
# measurement). one_runner_guard <board> [suite_path]: with a suite_path OUTSIDE the corpus tree the run is NOT a board and is
# never refused -- a gate's own two-entry mktemp fixture publishes no row, writes no score, grades no corpus population and costs
# about a second. Called with no suite_path, the run is judged a board, so a caller that does not say what it grades is refused
# rather than waved through; every shell board runner above is exactly that caller. The python copy in corpus_suite_harness.py
# carries the same rule, and passes args.sno with the corpus root, because the two copies must stay word for word.
one_runner_seat() {
  if [ -n "${S4E_SEAT:-}" ]; then printf '%s\n' "$S4E_SEAT"; return 0; fi
  local here; here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  S4E_HOME="${S4E_HOME:-$(cd "$here/../.." && pwd)}" python3 -c 'import sys; sys.path.insert(0,sys.argv[1]); import util_score_row as u; print(u.derive_measurer() or "")' "$here" 2>/dev/null
}
one_runner_suite_is_a_board() {
  local suite="$1" corpus sp cr
  corpus="${S4E_CORPUS:-${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}/corpus}"
  sp="$(cd "$(dirname "$suite")" 2>/dev/null && pwd)/$(basename "$suite")" || return 0
  cr="$(cd "$corpus" 2>/dev/null && pwd)" || return 0
  [ -n "$cr" ] || return 0
  case "$sp" in "$cr"/*|"$cr") return 0;; *) return 1;; esac
}
one_runner_guard() {
  local board="${1:-${0##*/}}" suite="${2:-}" seat
  if [ -n "$suite" ] && ! one_runner_suite_is_a_board "$suite"; then return 0; fi
  seat="$(one_runner_seat)"
  if [ "$seat" = coo ]; then return 0; fi
  if [ "${S4E_DONE_WHEN_RUN:-}" = 1 ]; then printf 'ONE-RUNNER: %s runs under the bus computed done for seat %s (exempt, one run per closure)\n' "$board" "${seat:-?}"; return 0; fi
  if [ -n "${S4E_ONE_RUNNER_OVERRIDE:-}" ]; then printf '⚠ ONE-RUNNER OVERRIDE by %s on %s: %s\n' "${seat:-?}" "$board" "$S4E_ONE_RUNNER_OVERRIDE"; return 0; fi
  printf '⛔ REFUSE(2) ONE RUNNER, ONE BOARD: %s is a board and seat %s is not the coo. The coo runs every board once per landing batch on origin HEAD and writes the rows (Lon 2026-09-10 16:3x, MODE line 2, RULES.md § FACT RULES, CEO-523). Your landing verdict is your row DONE-WHEN plus the gates you touched plus make preflight. A DONE-WHEN board clause runs under s4e_msg.sh done (exempt). S4E_ONE_RUNNER_OVERRIDE="why" is loud and recorded.\n' "$board" "${seat:-?}" >&2
  return 2
}
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
  case "${1:-}" in
    --check) seat="$(one_runner_seat)"; { [ "$seat" = coo ] || [ "${S4E_DONE_WHEN_RUN:-}" = 1 ] || [ -n "${S4E_ONE_RUNNER_OVERRIDE:-}" ]; } && exit 0; exit 2;;
    --seat) one_runner_seat; exit 0;;
    *) echo "usage: lib_one_runner.sh --check | --seat  (or source it and call one_runner_guard <board> [suite_path])" >&2; exit 2;;
  esac
fi
