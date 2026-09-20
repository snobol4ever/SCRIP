#!/bin/bash
# lib_one_runner.sh -- ONE RUNNER, ONE BOARD (Lon 2026-09-10 16:3x CDT, in-chat to ceo, verbatim: "Quit running the boards over and over
# again. Co-ordinate with one runner and one board. Do something to alleviate the churn."; RULES.md § FACT RULES, CEO-523).
# Sourced on line 2 of every master/package board runner and called by corpus_suite_harness.py run: a board is refused (rc=2) to any
# seat but the one MODE's LANES: line names for the board's LANGUAGE (CEO-775 below; the centralized coo runner is history). Exempt: the bus's own
# computed `done`/dispatch run of a DONE-WHEN (S4E_DONE_WHEN_RUN=1, set by s4e_msg.sh, one run per closure) and a loud, named
# override (S4E_ONE_RUNNER_OVERRIDE="why", printed on the board). ⛔⭐ THE OVERRIDE ADMITS THE RUN AND NOTHING ELSE (ceo CEO-961,
# 2026-09-20, on the coo's report; row one-runner-override-gates-the-run-and-the-leaderboard-write-needs-its-own-consent): until that
# ruling the same variable also opened util_score_row.py's LEADERBOARD write, so there was no way to say "run the arm and publish
# nothing" and a runner driven as a CONTROL ARM -- the shape RULES.md § SHARED-NODE VERDICT SCOPE requires of every landing -- published
# a row as a side effect of being used as a measurement (measured: the coo's gimpel arm, COO-115, reverted by hand). A leaderboard write
# now needs its OWN loud recorded consent, S4E_SCORE_WRITE="why", or the seat the LANES: line names for that language; a control arm
# writes NOTHING by default. Identity: S4E_SEAT if set, else the ONE root-path map in
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
# ⛔⭐ WHOSE corpus IS IT? (coo 2026-09-18, row instruments-the-progress-fingerprint-gate-appends-no-rows-arms-1-and-2-red-and-arm-4-
# vacuous). CEO-547 part 1 says a gate's own mktemp fixture is not a board because it grades no corpus population -- but the test
# written for it asked a PATH question, "is the suite under the corpus root", and the corpus root is whatever S4E_HOME/corpus
# resolves to. A hermetic fixture that builds its own two-repo world and points S4E_HOME at it therefore lands INSIDE its own corpus
# tree and is judged a board: test_gate_progress_rows_carry_the_start_fingerprint.sh was DARK on origin for a day because its
# eight-entry scratch master was refused rc=2, the harness appended nothing, and arms 1 and 2 read FAIL for a reason that was never
# about fingerprints. ⭐ THE FACT ASKED IS THE ONE CEO-547 NAMES -- is this the population every seat has? -- and it is asked of the
# remote: a real checkout carries origin snobol4ever/corpus, a `git init` scratch world carries no remote at all. ⛔ BOTH FACTS, NEVER
# EITHER: a real board that merely redirected its progress writes is still a board, and a checkout that merely lost its remote is
# still the shared population. ⛔ UNREADABLE ANSWERS BOARD: a guard that cannot tell must refuse, never wave through.
ONE_RUNNER_SHARED_CORPUS_REMOTE='snobol4ever/corpus'
ONE_RUNNER_LIVE_PROGRESS_DB='/home/resources/progress/results.tsv'
one_runner_corpus_is_the_shared_population() {
  local cr="$1" out
  out="$(git -C "$cr" remote -v 2>/dev/null)" || return 0
  case "$out" in *"$ONE_RUNNER_SHARED_CORPUS_REMOTE"*) return 0;; *) return 1;; esac
}
one_runner_writes_the_live_progress_table() {
  local db="${S4E_PROGRESS_DB:-$ONE_RUNNER_LIVE_PROGRESS_DB}"
  [ "$(readlink -f "$db" 2>/dev/null || printf '%s' "$db")" = "$(readlink -f "$ONE_RUNNER_LIVE_PROGRESS_DB" 2>/dev/null || printf '%s' "$ONE_RUNNER_LIVE_PROGRESS_DB")" ]
}
one_runner_suite_is_a_board() {
  local suite="$1" corpus sp cr
  corpus="${S4E_CORPUS:-${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}/corpus}"
  sp="$(cd "$(dirname "$suite")" 2>/dev/null && pwd)/$(basename "$suite")" || return 0
  cr="$(cd "$corpus" 2>/dev/null && pwd)" || return 0
  [ -n "$cr" ] || return 0
  case "$sp" in "$cr"/*|"$cr") ;; *) return 1;; esac
  if ! one_runner_corpus_is_the_shared_population "$cr" && ! one_runner_writes_the_live_progress_table; then
    printf 'ONE-RUNNER: FIXTURE, not a board -- %s is under %s, which is not a checkout of %s, and this run appends to a scratch progress table. No row reaches the shared record (CEO-547 part 1).\n' "$sp" "$cr" "$ONE_RUNNER_SHARED_CORPUS_REMOTE"
    return 1
  fi
  return 0
}
# ⛔⭐⭐⭐ NO CENTRAL RUNNER: EVERY LANGUAGE HQ RUNS ITS OWN LANGUAGE'S TEST AND BENCHMARK SUITES (Lon 2026-09-16 10:5x CDT, in-chat
# to ceo, verbatim: "So do not have a centralized runner at all. Let's each HQ run its language test and benchmark suites."; ceo CEO-775,
# RULES.md § FACT RULE -- ONE RUNNER PER LANGUAGE). The property Lon wanted on 09-10 -- one runner per board, no two seats grading the
# same suite -- is kept by PARTITIONING the runner by language instead of centralizing it: a board belongs to the language it grades,
# and the seat that may run it is the seat MODE's LANES: line names for that language (rebus=cfo today). Nobody is the runner of every
# board; an officer is refused every board its lane does not name, exactly as a non-coo seat was before. The law source is the LANES
# line, never a name baked in here (CEO-756 still binds). A board with no language (board_packages.sh) is admitted to any LANES seat;
# its per-language runners refuse the seats that do not own them.
one_runner_lang() {
  local board="${1:-}" suite="${2:-}" sp
  if [ -n "$suite" ]; then
    sp="$(cd "$(dirname "$suite")" 2>/dev/null && pwd)/$(basename "$suite")"
    case "$sp" in */corpus/tests/*|*/corpus/packages/*|*/corpus/benchmarks/*|*/corpus/demos/*)
      printf '%s\n' "$sp" | sed -E 's#.*/corpus/(tests|packages|benchmarks|demos)/([^/]+)/.*#\2#'; return 0;; esac
  fi
  case "$board" in
    *icon*|*jcon*|*ipl*|*arizona*) printf 'icon';;
    *prolog*|*inria*|*swi*|*gnu*|*logtalk*) printf 'prolog';;
    *pascal*|*fpc*|*pat_suite*) printf 'pascal';;
    *raku*|*roast*) printf 'raku';;
    *snocone*) printf 'snocone';;
    *rebus*) printf 'rebus';;
    *snobol4*|*snoflake*|*gimpel*|*aisnobol*|*csnobol4*|*dotnet*|*spitbol*|*testpgms*) printf 'snobol4';;
    board_packages*) printf 'all';;
    *) printf 'unknown';;
  esac
}
# ⛔⭐ AN ABSENT OR UNPARSEABLE LANES LINE REFUSES RATHER THAN RESOLVES (ceo CEO-957, 2026-09-19, on the coo's
# report; row instruments-an-absent-lanes-line-in-mode-must-refuse-not-resolve-every-language-to-the-coo).
# This function used to fall back to a BAKED SEAT NAME when MODE carried no LANES: line -- ceo under MODE CEO
# and coo under EVERY other mode, for EVERY language. That is the CENTRALIZED RUNNER that CEO-775 removed on
# Lon's word, reachable by deleting one line from MODE, and it contradicted the rule written three lines above
# it in this same file: the law source is the LANES line, never a name baked in here. It is the CEO-907 shape
# in a new place -- a flip rewrites the prose and leaves a machine-read line behind. Reproduced by both seats:
# a three-line MODE carrying QUARTET and no LANES read icon -> coo, rebus -> coo, all -> coo.
# one_runner_who now returns EMPTY, and one_runner_guard refuses rc=2 on an empty owner NAMING THE MISSING
# LANES LINE specifically -- a missing line and a line that names no owner for this language are DIFFERENT
# faults and a reader must not have to guess which one they are looking at.
one_runner_lanes_line() {
  grep -m1 '^LANES:' "${S4E_POST:-/home/resources/postoffice}/MODE" 2>/dev/null | sed 's/^LANES://'
}
one_runner_who() {
  local lang="${1:-}" lanes
  lanes="$(one_runner_lanes_line)"
  if [ -z "$lanes" ]; then printf ''; return 0; fi
  case "$lang" in
    all) printf '%s' "$lanes" | tr ' ' '\n' | sed -n 's/^[a-z0-9]*=//p' | sort -u | tr '\n' ' ' | sed 's/ $//';;
    ''|unknown) printf '';;
    *) printf '%s' "$lanes" | tr ' ' '\n' | sed -n "s/^$lang=//p" | head -1;;
  esac
}
one_runner_seat_admitted() {
  local seat="$1" who="$2" w
  for w in $who; do [ "$seat" = "$w" ] && return 0; done
  return 1
}
# ⛔⭐ THE BINARY IS STAMPED WHERE EVERY BOARD STARTS (coo 2026-09-16; hq_raku's report against themselves, ceo CEO-802; row instruments-
# a-board-does-not-refuse-when-its-own-seat-rebuilds-the-binary-under-it-the-raku-master-graded-a-tree-that-changed-mid-run): a board and a
# build share one mutable artefact, ./scrip + out/libscrip_rt.so, and a board whose seat ran `make` under it printed RakM 764/927 -- a
# measurement of nothing. one_runner_guard exports S4E_BIN_AT_START (lib_gate.sh's fingerprint shape: md5 first 12 of each file, space-
# joined); util_progress_append.py and util_score_row.py REFUSE when the binary they see differs, and corpus_suite_harness.py checks it
# before printing any board. An outer runner's stamp is kept by an inner one.
one_runner_bin_fingerprint() {
  local _root="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}" _b _r
  _b="${SCRIP:-$_root/SCRIP/scrip}"; _r="${RT_DIR:-$_root/SCRIP/out}/libscrip_rt.so"
  [ -f "$_b" ] && [ -f "$_r" ] || return 1
  md5sum "$_b" "$_r" 2>/dev/null | cut -c1-12 | tr '\n' ' '
}
one_runner_guard() {
  if [ -z "${S4E_BIN_AT_START:-}" ]; then S4E_BIN_AT_START="$(one_runner_bin_fingerprint)" && export S4E_BIN_AT_START; fi
  local board="${1:-${0##*/}}" suite="${2:-}" seat who lang
  if [ -n "$suite" ] && ! one_runner_suite_is_a_board "$suite"; then return 0; fi
  seat="$(one_runner_seat)"; lang="$(one_runner_lang "$board" "$suite")"; who="$(one_runner_who "$lang")"
  if [ -n "$seat" ] && one_runner_seat_admitted "$seat" "$who"; then return 0; fi
  if [ "${S4E_DONE_WHEN_RUN:-}" = 1 ]; then printf 'ONE-RUNNER: %s runs under the bus computed done for seat %s (exempt, one run per closure)\n' "$board" "${seat:-?}"; return 0; fi
  if [ -n "${S4E_ONE_RUNNER_OVERRIDE:-}" ]; then printf '⚠ ONE-RUNNER OVERRIDE by %s on %s: %s\n' "${seat:-?}" "$board" "$S4E_ONE_RUNNER_OVERRIDE"; return 0; fi
  local _noseat
  if [ -z "$(one_runner_lanes_line)" ]; then
    _noseat="<NO SEAT: the MODE file carries NO LANES: line at all, so no language has an owner. This REFUSES and no longer resolves to a baked name (ceo CEO-957). Fix MODE, or, in a fixture, declare a lane table the way gate_stage_picker_lane_table does>"
  else
    _noseat="<no seat -- the LANES line is present but names none for this language>"
  fi
  printf '⛔ REFUSE(2) ONE RUNNER, ONE BOARD -- ONE RUNNER PER LANGUAGE: %s is a %s board and seat %s is not %s, the seat MODE LANES: names for %s. Every language HQ runs its OWN language suites, once per landing, on origin HEAD, and writes its own rows (Lon 2026-09-16 10:5x, MODE line 2, RULES.md § ONE RUNNER PER LANGUAGE, CEO-775); another language board is an ASK to that language HQ. S4E_ONE_RUNNER_OVERRIDE="why" is loud and recorded, and it admits THE RUN ONLY -- publishing a leaderboard row needs its own consent, S4E_SCORE_WRITE="why" (ceo CEO-961).\n' "$board" "$lang" "${seat:-?}" "${who:-$_noseat}" "$lang"
  return 2
}
# ⛔⭐ THE SEAM: EVERY GUARD SHIPS A SANCTIONED WAY TO BE TRIPPED THAT DOES NOT REQUIRE DOING THE FORBIDDEN THING
# (ceo CEO-560, 2026-09-11, on hq_B's self-report and hq_U's mirror case the same hour; FINDING-2026-09-11-hq_B-a-guard-
# whose-only-proof-of-firing-is-committing-the-act-it-forbids.md). Until now the ONLY route to this guard's refusal was
# S4E_SEAT=coo -- i.e. asserting another seat's identity, the exact act the guard exists to stop -- so proving it fires
# meant committing the offence. A guard that can only be tested that way is tested that way OR NOT AT ALL, and "not at
# all" is how icn_port_trace got to 22 of 24. ⭐ THE PROBE SEAT IS INERT BY CONSTRUCTION: it is a reserved name that can
# never be a real seat, so it can only ever REMOVE privilege and never grant it -- the opposite of S4E_ONE_RUNNER_OVERRIDE,
# which proves the BYPASS works and says nothing about the refusal. Those are different assertions and a seam must not
# conflate them. The caller stays themselves throughout; the coo can run this and see a refusal without ceasing to be the coo.
ONE_RUNNER_PROBE_SEAT='__one_runner_probe_not_a_seat__'
one_runner_prove_seam() {
  local out rc fails=0 outside
  out=$(S4E_SEAT="$ONE_RUNNER_PROBE_SEAT" S4E_DONE_WHEN_RUN='' S4E_ONE_RUNNER_OVERRIDE='' one_runner_guard 'one-runner-seam-probe' "${S4E_CORPUS:-${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}/corpus}" 2>&1); rc=$?
  if [ "$rc" = 2 ] && printf '%s' "$out" | grep -q 'REFUSE(2) ONE RUNNER'; then printf '  OK   ARM 1 the guard REFUSES a board to a seat its language LANE does not name (rc=2, message asserted)
'
  else printf '  FAIL ARM 1 expected rc=2 and a REFUSE message on a corpus-rooted board, got rc=%s: %s
' "$rc" "${out:-<silent>}"; fails=$((fails+1)); fi
  outside="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  S4E_SEAT="$ONE_RUNNER_PROBE_SEAT" S4E_DONE_WHEN_RUN='' S4E_ONE_RUNNER_OVERRIDE='' one_runner_guard 'one-runner-seam-probe' "$outside" >/dev/null 2>&1; rc=$?
  if [ "$rc" = 0 ]; then printf '  OK   ARM 2 the guard ADMITS a suite path OUTSIDE the corpus tree (a gate fixture is not a board)
'
  else printf '  FAIL ARM 2 expected rc=0 for a non-corpus suite path, got rc=%s
' "$rc"; fails=$((fails+1)); fi
  [ "$fails" = 0 ] || return 1
  printf 'ONE-RUNNER SEAM: PASS -- refusal and admission both proven, no seat identity asserted
'
}
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
  case "${1:-}" in
    --prove-seam) one_runner_prove_seam; exit $?;;
    --check) seat="$(one_runner_seat)"; lang="$(one_runner_lang "${2:-}" "${3:-}")"; { { [ -n "$seat" ] && one_runner_seat_admitted "$seat" "$(one_runner_who "$lang")"; } || [ "${S4E_DONE_WHEN_RUN:-}" = 1 ] || [ -n "${S4E_ONE_RUNNER_OVERRIDE:-}" ]; } && exit 0; exit 2;;
    --who) one_runner_who "$(one_runner_lang "${2:-}" "${3:-}")"; echo; exit 0;;
    --lang) one_runner_lang "${2:-}" "${3:-}"; echo; exit 0;;
    --seat) one_runner_seat; exit 0;;
    *) echo "usage: lib_one_runner.sh --check <board> [suite] | --who <board> [suite] | --lang <board> [suite] | --seat | --prove-seam  (or source it and call one_runner_guard <board> [suite_path])" >&2; exit 2;;
  esac
fi
