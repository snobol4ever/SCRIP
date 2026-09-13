#!/usr/bin/env bash
# test_gate_score_row_only_the_one_runner_writes.sh -- GATE (hq_B 2026-09-13, on hq_I's measurement; row
# instruments-a-suite-runner-writes-a-score-md-row-for-every-seat-while-one-runner-allows-only-the-coo).
# ⛔⭐ THE HAZARD, AND NOBODY MADE A MISTAKE MAKING IT. Two rules held at once: the FACT RULE says any suite run
# updates the ONE LEADERBOARD so the score is never an hour away, and MODE NONET's ONE RUNNER clause says every seat
# but the one runner grades with the per-group development aids and writes no SCORE.md row. util_score_row.py predates
# the lane cut and faithfully obeys the older rule, so every ladder sitting ended with a forbidden edit sitting in a
# seat's .github working tree.
# ⛔ THE MITIGATION WAS MEMORY, AND MEMORY IS WHAT FAILED -- BY BEING ROUTED AROUND, NOT BY FORGETTING. hq_I declined
# a rank raise on the stated grounds that it watched the file at every push, and within the hour a pull-with-rebase
# refused on the dirty SCORE.md, an ordinary stage-everything swept the row in, and a row only the one runner may
# write reached origin for about four minutes carrying a TRUE number with the wrong provenance -- indistinguishable
# from the runner's own, on a tree the one runner never stamped. A rule whose cue does not fire in the context you are
# actually standing in is a coin flip with good intentions.
# ⭐ SO THIS GATE GRADES A NO-OP, NOT AN ANNOUNCEMENT (hq_I's own correction): if the file is never modified there is
# nothing for a stage-everything to sweep and the class is gone, whereas a loud line leaves a dirty file waiting for
# the next rebase. The announcement is graded too, because a silent no-op and a silent success look identical.
# ⛔⭐ ARM 3 IS THE ONE THAT MATTERS IN SIX MONTHS: it names a runner who is NOT the coo in a scratch MODE file and
# requires the coo to be refused there. A hard-coded population never announces what it left out -- this root's own
# digest gate carried a literal ROOTS array and was green for months about nineteen other files, and this very lane
# cut left the queue's owner column 44 rows stale -- so an implementation that spells `coo` passes arms 1 and 2 and
# fails arm 3, which is the whole reason arm 3 exists.
# ⛔ EVERY ARM IS HERMETIC: a scratch tree, a scratch MODE file, a scratch board. It never reads the live MODE, never
# writes the real SCORE.md, and asserts BOTH directions of every door. Runs in about two seconds and builds nothing.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
U="$HERE/util_score_row.py"
GH="$(cd "$HERE/../.." && pwd)/.github"
refuse() { echo "⛔ GATE REFUSES (rc=2): $1"; exit 2; }
[ -f "$U" ] || refuse "util_score_row.py is not at $U -- the one writer moved; re-pin this gate rather than reinterpret it"
[ -f "$GH/SCORE.md" ] || refuse "no $GH/SCORE.md to copy as a fixture -- a board-write gate with no board to write cannot measure"
command -v git >/dev/null 2>&1 || refuse "no git on PATH -- the fixture's shared-board property IS a git property, so this cannot be measured"
python3 -c 'import sys' 2>/dev/null || refuse "no python3 -- the subject of this gate is a python helper"
FAIL=0; N=0
arm() { N=$((N+1)); if [ "$1" = 0 ]; then echo "  ✅ $2"; else echo "  ⛔ $2"; FAIL=$((FAIL+1)); fi; }
WORK="$(mktemp -d "${TMPDIR:-/tmp}/one_runner_writes.XXXXXX")" || refuse "could not mktemp a fixture root"
trap 'rm -rf "$WORK"' EXIT
# ⭐ THE FIXTURE IS A SHARED BOARD BY THE SAME PROPERTY THE REAL ONE HAS -- a SCORE.md inside a clone that has an
# `origin` -- because that is the property the guard tests, and a fixture that is a board "because the gate says so"
# would grade a different question than the one the runners ask. Nothing is pushed anywhere: the remote is a path
# that does not exist, which is enough to be named and never contacted.
mkboard() { # mkboard <dir> <as-repo yes|no>
    mkdir -p "$1/.github" || return 1
    cp "$GH/SCORE.md" "$1/.github/SCORE.md" || return 1
    [ "$2" = yes ] || return 0
    git -c init.defaultBranch=main init -q "$1/.github" >/dev/null 2>&1 || return 1
    git -C "$1/.github" remote add origin "$1/not-a-real-origin.git" >/dev/null 2>&1 || return 1
}
mode_with() { printf 'NONET\n# %s\n' "$1" > "$WORK/MODE"; }
# ⛔ `write` IS CALLED THE WAY A RUNNER CALLS IT, not through an internal function: the defect hq_I hit was in the
# path the runners actually take, and a gate that imports the module and calls a helper can be green while the
# command-line path is not. --column ladder + no --suite keeps SUITES.tsv out of it, so each arm grades one thing.
run_write() { # run_write <home> <measurer> [extra args...]
    local home="$1" meas="$2"; shift 2
    S4E_HOME="$home" S4E_MODE_FILE="$WORK/MODE" S4E_SEAT="$meas" S4E_DONE_WHEN_RUN="${DW:-}" \
      S4E_ONE_RUNNER_OVERRIDE="${OV:-}" python3 "$U" write --lang snocone --column ladder \
      --text "rungs 0..9 PASS 33/33 (fixture)" --measurer "$meas" "$@" 2>&1
}
sum_of() { md5sum "$1/.github/SCORE.md" | cut -d' ' -f1; }
echo "--- ARMS 1-2: a scratch MODE naming the coo -- a non-runner seat writes NOTHING and SAYS SO, the runner writes ---"
B1="$WORK/b1"; mkboard "$B1" yes || refuse "could not build the repo-backed fixture board"
mode_with 'coo PASCAL, and THE ONE RUNNER for every master and package board; hq_I SNOCONE'
before="$(sum_of "$B1")"; out="$(run_write "$B1" hq_I)"; rc=$?
[ "$rc" = 0 ]; arm $? "the refusal is NON-FATAL (rc=$rc) -- a bookkeeping rule must never be able to red a measured board"
[ "$(sum_of "$B1")" = "$before" ]; arm $? "the board file is BYTE-UNCHANGED -- nothing for a stage-everything to sweep in"
[[ "$out" == *"NOT UPDATED"* && "$out" == *coo* ]]; arm $? "and it ANNOUNCES itself, naming the one runner (a silent no-op and a silent success look identical)"
[[ "$out" == *"rungs 0..9 PASS 33/33"* ]]; arm $? "the measurement is repeated back, so the number is not lost with the row"
out="$(run_write "$B1" coo)"; rc=$?
[ "$rc" = 0 ] && [ "$(sum_of "$B1")" != "$before" ]; arm $? "THE ONE RUNNER WRITES: the row lands for the seat MODE names (rc=$rc)"
[[ "$out" == *"rewritten in place"* ]]; arm $? "and the write says so in the runner's own words"
echo "--- ARM 3: THE ROSTER IS READ, NOT SPELLED -- name a runner who is not the coo and the coo is refused ---"
B3="$WORK/b3"; mkboard "$B3" yes || refuse "could not build the second fixture board"
mode_with 'cfo SNOBOL4, and THE ONE RUNNER for every master and package board; coo PASCAL'
before="$(sum_of "$B3")"; out="$(run_write "$B3" coo)"
[ "$(sum_of "$B3")" = "$before" ] && [[ "$out" == *"NOT UPDATED"* && "$out" == *cfo* ]]
arm $? "the coo is REFUSED where the scratch MODE names the cfo -- an implementation spelling a seat name fails here"
out="$(run_write "$B3" cfo)"
[ "$(sum_of "$B3")" != "$before" ]; arm $? "and the cfo writes, for no reason other than line 2 saying so"
echo "--- ARMS 4-5: CANNOT-DETERMINE IS NOT A REFUSAL -- an unreadable roster falls back to the older FACT RULE, loudly ---"
B4="$WORK/b4"; mkboard "$B4" yes || refuse "could not build the third fixture board"
rm -f "$WORK/MODE"; before="$(sum_of "$B4")"; out="$(run_write "$B4" hq_I)"
[ "$(sum_of "$B4")" != "$before" ] && [[ "$out" == *"NOT DETERMINED"* ]]
arm $? "MODE ABSENT: the row is WRITTEN and the uncertainty is stated -- an instrument that cannot measure must not print a verdict"
B5="$WORK/b5"; mkboard "$B5" yes || refuse "could not build the fourth fixture board"
mode_with 'hq_I SNOCONE; hq_T RAKU; the lanes are cut by concern and this mode names no single board writer'
before="$(sum_of "$B5")"; out="$(run_write "$B5" hq_I)"
[ "$(sum_of "$B5")" != "$before" ] && [[ "$out" == *"NOT DETERMINED"* ]]
arm $? "A MODE THAT CUTS NO ONE RUNNER refuses nobody -- the guard follows the roster instead of outliving it"
echo "--- ARMS 6-7: BOTH SANCTIONED DOORS, LOUD AND RECORDED (lib_one_runner.sh's own, word for word) ---"
B6="$WORK/b6"; mkboard "$B6" yes || refuse "could not build the fifth fixture board"
mode_with 'coo PASCAL, and THE ONE RUNNER for every master and package board'
before="$(sum_of "$B6")"; out="$(DW=1 run_write "$B6" hq_I)"
[ "$(sum_of "$B6")" != "$before" ]; arm $? "the bus's computed \`done\` run of a DONE-WHEN writes (S4E_DONE_WHEN_RUN=1, one run per closure)"
B7="$WORK/b7"; mkboard "$B7" yes || refuse "could not build the sixth fixture board"
before="$(sum_of "$B7")"; out="$(OV='proving the seam' run_write "$B7" hq_I)"
[ "$(sum_of "$B7")" != "$before" ] && [[ "$out" == *OVERRIDE* && "$out" == *"proving the seam"* ]]
arm $? "a NAMED override writes the row and prints the reason -- the channel stays usable and stays loud"
echo "--- ARM 8: A GATE'S OWN SCRATCH COPY IS NOT THE LEADERBOARD, so this guard cannot break the gates that prove the writer ---"
B8="$WORK/b8"; mkboard "$B8" no || refuse "could not build the repo-less fixture board"
before="$(sum_of "$B8")"; out="$(run_write "$B8" hq_I)"
[ "$(sum_of "$B8")" != "$before" ] && [[ "$out" != *"NOT UPDATED"* ]]
arm $? "a SCORE.md in a plain mktemp dir (no clone, no origin) is written by any seat -- what makes a write a leaderboard write is the file it lands in"
echo "--- ARM 9: --dry-run is PREVIEWED, never refused: it writes nothing, so refusing it would only hide the preview ---"
B9="$WORK/b9"; mkboard "$B9" yes || refuse "could not build the seventh fixture board"
before="$(sum_of "$B9")"; out="$(run_write "$B9" hq_I --dry-run)"
[ "$(sum_of "$B9")" = "$before" ] && [[ "$out" != *"NOT UPDATED"* ]]
arm $? "a non-runner seat's --dry-run still previews and still writes nothing"
echo "graded $N arm(s)"
if [ "$FAIL" != 0 ]; then
    echo "⛔ GATE FAIL: $FAIL of $N arm(s) red -- ONE RUNNER, ONE BOARD is not held at the write path."
    exit 1
fi
echo "✅ GATE PASS: only the seat MODE line 2 names writes the leaderboard, the roster is read live, both doors are open, and a scratch board is nobody's leaderboard."
