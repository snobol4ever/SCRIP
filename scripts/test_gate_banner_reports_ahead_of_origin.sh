#!/usr/bin/env bash
# test_gate_banner_reports_ahead_of_origin.sh -- the banner NAMES a repo whose HEAD is not on origin.
#
# ⛔ THE GAP THIS GUARDS (ceo -> hq_B 2026-09-08, row instruments-the-banner-reports-a-seat-that-is-ahead-of-
# origin-because-finished-invisible-work-is-reported-by-nothing): the fleet reports a seat that STALLS and a seat
# that FAILS, and reported NOTHING about a seat whose work is finished, correct and unpushed. There is no red
# anywhere in that state -- the seat's own banner said ✅ SUCCESS, because "safe to /clear" is a different question
# from "can anyone else see this". It cost 45 minutes on the cto's END-statement cure with the coo waiting on it,
# and 20 minutes on hq_B's digest-gate commit, both on 2026-09-08.
#
# ⛔⭐ THIS GATE PROVES RED AS WELL AS GREEN, WHICH IS THE ONLY REASON TO TRUST ITS GREEN. A reporting line is the
# easiest thing in this repo to break invisibly: delete it and every banner still prints, still says SUCCESS, and
# no suite goes red. So the fixture below is a REAL git pair -- a bare origin and a clone with a deliberate
# unpushed commit -- and the gate fails if the ahead arm does not name that repo AND its count.
# ⭐ AND IT ASSERTS THE NEGATIVE HALF TOO: a clone whose HEAD is on origin must print the none-line, because a
# line that fires unconditionally is worth nothing and would be muted inside a day.
#
# ⛔ THE VERDICT MUST NOT MOVE. Being ahead is not a failure; the ceo's row says so outright ("must never print
# FAILURE -- the seat has done nothing wrong except not finish"). So the gate also asserts the word FAILURE does
# NOT appear on the ahead line, and that the banner's own rc is unchanged between the ahead and clean arms.
#
# Exit: 0 CLEAN / 1 VIOLATION / 2 UNPROVEN (cannot build the fixture -- never a silent skip).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"
# ⛔ THE VERDICT GOES THROUGH lib_gate.sh's OWN API, NEVER A PRIVATE gate_pass/gate_fail PAIR. The first draft of
# this file invented `gate_fail`/`gate_pass`/`gate_unproven` -- names that do not exist in lib_gate.sh -- and every
# assertion below therefore ran, matched nothing, and fell through to `gate_pass: command not found` at rc=127.
# ⭐ THAT IS THIS GATE'S OWN LESSON HAPPENING TO ITSELF: rc=127 is not 0, so it was caught here -- but a gate whose
# FAILURE path is a typo'd function name would have passed silently had the last line been the only real call. The
# violation counter below is a plain integer and the verdict is gate_verdict's, so there is one exit path.
VIOL=0; WHY=""
gate_fail() { VIOL=$((VIOL+1)); WHY="${WHY:+$WHY; }$1"; echo "GATE HIT [$GATE_NAME]: $1"; }
gate_unproven() { echo "GATE UNPROVEN(2) [$GATE_NAME]: $1"; echo "    This is NOT a pass. The gate could not build its fixture, so it cannot certify anything."; gate_stamp; exit 2; }

command -v git >/dev/null 2>&1 || gate_unproven "git is not on PATH -- cannot build the fixture clone, so this gate cannot measure"
W="$(mktemp -d)" || gate_unproven "could not mktemp a fixture root"
trap 'rm -rf "$W"' EXIT
export GIT_AUTHOR_NAME=LCherryholmes GIT_AUTHOR_EMAIL=lcherryh@yahoo.com
export GIT_COMMITTER_NAME=LCherryholmes GIT_COMMITTER_EMAIL=lcherryh@yahoo.com

# --- the fixture: a bare origin, and a clone of it under a seat-root-shaped directory --------------------------
git init -q --bare "$W/origin.git" 2>/dev/null || gate_unproven "git init --bare failed -- cannot build the fixture"
git -c init.defaultBranch=main clone -q "$W/origin.git" "$W/root/FIXTUREREPO" 2>/dev/null || gate_unproven "git clone failed -- cannot build the fixture"
cd "$W/root/FIXTUREREPO" || gate_unproven "cannot enter the fixture clone"
git checkout -q -b main 2>/dev/null || true
echo one > f.txt && git add f.txt && git commit -q -m "fixture base" 2>/dev/null || gate_unproven "could not commit the fixture base"
git push -q -u origin main 2>/dev/null || gate_unproven "could not push the fixture base to its bare origin"

# ⛔⭐ THE WHOLE BANNER IS CAPTURED, AND THE GREP READS THE CAPTURE (coo 2026-09-10, ceo CEO-524 (2)).
# THE REPORT THIS ANSWERS (hq_B): this gate read 6/6 arms EMPTY inside `make test` twice and passed standalone
# -- a flake whose failure mode is "the banner printed nothing". The old body piped the banner straight into
# `grep -m1` and kept ONLY the matched line, so on the empty reading EVERY BYTE OF EVIDENCE WAS DISCARDED: no
# rc, no stderr, no way to tell a timeout from an early exit from a banner that ran fine and simply omitted the
# arm. Three candidate causes were ruled out by measurement rather than by reading -- the Makefile exports no
# S4E_HOME (so the seat cannot mis-resolve under make), util_score_row.py takes no lock on SCORE.md (so a
# concurrent board cannot block it), and the gate passed twice at load 1.8 with a package board grading beside
# it. ⭐ SO THE CAUSE IS STILL UNNAMED, AND CEO-524 (2) SAYS NAME IT BEFORE CURING. This is the instrument that
# names it: the capture costs nothing on the happy path and, on the next empty reading, prints the banner's rc
# (124 IS THE TIMEOUT FIRING AND SAYS SO), the resolved identity the banner computed, and the first lines it
# actually produced. ⛔ A gate that reports "printed nothing" and knows nothing about the nothing cannot be
# debugged from its own log, which is why this flake survived two sightings.
BANNER_RAW=""; BANNER_RC=0
banner_ahead_line() {  # runs the banner arm against the fixture root and echoes its AHEAD line
    BANNER_RAW="$(S4E_AHEAD_HOME="$W/root" S4E_BANNER_NO_BOARD=1 timeout 300 bash "$HERE/s4e_msg.sh" banner 2>&1)"; BANNER_RC=$?
    printf '%s\n' "$BANNER_RAW" | grep -m1 'AHEAD OF ORIGIN'
}
banner_evidence() {  # called ONLY on an empty reading -- the flake's own post-mortem, printed where the log is
    echo "    ⛔ EVIDENCE FOR THE EMPTY READING (ceo CEO-524 (2), hq_B's flake):"
    if [ "$BANNER_RC" -eq 124 ]; then
        echo "       banner rc=124 -- THE 300s TIMEOUT FIRED. That is a duration, not a defect of the arm: the"
        echo "       banner did not finish, so the line it would have printed was never reached."
    else
        echo "       banner rc=$BANNER_RC (124 would mean the 300s timeout fired; 3 is the bus refusing an unknown"
        echo "       mailbox; 0 means it ran to completion and simply did not print the arm)"
    fi
    echo "       bytes captured: $(printf '%s' "$BANNER_RAW" | wc -c)   lines: $(printf '%s\n' "$BANNER_RAW" | grep -c .)"
    echo "       S4E_HOME=${S4E_HOME:-<unset>}  S4E_SEAT=${S4E_SEAT:-<unset>}  S4E_POST=${S4E_POST:-<unset>}  PWD=$PWD"
    echo "       first lines the banner actually produced:"
    printf '%s\n' "$BANNER_RAW" | head -25 | sed 's/^/         | /'
}

# --- ARM 1: CLEAN -- HEAD is on origin, the line must say so and must not claim an ahead count ------------------
clean_line="$(banner_ahead_line)"; clean_rc=$?
[ -n "$clean_line" ] || gate_fail "the banner printed NO 'AHEAD OF ORIGIN' line at all against a clean fixture -- the arm is absent or unreachable, which is exactly the invisible-reporting defect this gate exists for"
banner_evidence
case "$clean_line" in
    *"AHEAD OF ORIGIN: none"*) ;;
    *) gate_fail "clean fixture (HEAD == origin) but the banner did not print the none-line; it printed: $clean_line" ;;
esac

# --- ARM 2: AHEAD -- one deliberate unpushed commit, the line must NAME the repo and the count -------------------
echo two >> f.txt && git commit -q -am "fixture unpushed commit" 2>/dev/null || gate_unproven "could not create the deliberate unpushed commit"
ahead_line="$(banner_ahead_line)"; ahead_rc=$?
[ -n "$ahead_line" ] || gate_fail "one unpushed commit exists and the banner printed NO 'AHEAD OF ORIGIN' line"
banner_evidence
case "$ahead_line" in
    *"AHEAD OF ORIGIN: none"*) gate_fail "one unpushed commit exists and the banner still reported 'none' -- the count is not being read live from git: $ahead_line" ;;
esac
case "$ahead_line" in
    *FIXTUREREPO*) ;;
    *) gate_fail "the ahead line does not NAME the repo that is ahead (the ceo's row requires naming each repo and by how much): $ahead_line" ;;
esac
case "$ahead_line" in
    *"FIXTUREREPO+1"*) ;;
    *) gate_fail "the ahead line names the repo but not its count as +1: $ahead_line" ;;
esac
case "$ahead_line" in
    *FAILURE*) gate_fail "the ahead line contains the word FAILURE; being ahead of origin is NOT a failure (ceo 2026-09-08, verbatim: 'being ahead is NOT a failure and must never print FAILURE'): $ahead_line" ;;
esac
[ "$clean_rc" = "$ahead_rc" ] || gate_fail "the banner's exit status changed between the clean arm ($clean_rc) and the ahead arm ($ahead_rc) -- the ahead report must never move the verdict"

# --- ARM 3: THE AGE ARM SAYS IT IN WORDS ------------------------------------------------------------------------
# ⭐ Ahead-1-for-thirty-seconds is a seat mid-push; ahead-1-for-forty-five-minutes is a cure nobody has. A bare
# count cannot tell those apart. Forcing the threshold to 0 makes the just-made commit read as stale, which is the
# only way to exercise the escalated wording without waiting ten minutes inside a gate.
stale_line="$(S4E_AHEAD_STALE_MIN=0 S4E_AHEAD_HOME="$W/root" S4E_BANNER_NO_BOARD=1 timeout 300 bash "$HERE/s4e_msg.sh" banner 2>&1 | grep -m1 'AHEAD OF ORIGIN')"
case "$stale_line" in
    *"AHEAD OF ORIGIN FOR "*m*) ;;
    *) gate_fail "with the staleness threshold at 0 the banner did not print the age-in-words arm: $stale_line" ;;
esac
case "$stale_line" in
    *FAILURE*) gate_fail "the stale-ahead line contains the word FAILURE, which the row forbids: $stale_line" ;;
esac

gate_verdict "$VIOL" "defect(s) in the banner's ahead-of-origin report (fixture: a bare origin plus a clone with one deliberate unpushed commit; arms: clean says none, ahead names repo+count, age escalates in words, the word FAILURE never appears, verdict rc unchanged)${WHY:+ -- $WHY}"
