#!/usr/bin/env bash
# test_prolog_rung14.sh — retract: all, basic, mixed, nonexistent, unify
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIPBIN="${HERE}/../scrip"
CORPUS="$S4E/corpus/tests/prolog"
FAMILY=rung14_retract
SNO="$CORPUS/$FAMILY.pl"
REF="$CORPUS/$FAMILY.ref"
PASS=0; FAIL=0
[ -d "$CORPUS" ] || { echo "⛔ REFUSED-TO-GRADE: $CORPUS missing"; exit 2; }

echo "=== rung14: retract (all/basic/mixed/nonexistent/unify) ==="

# consolidated 2026-08-28 (tests-consolidate-prolog): 2 entries (nonexistent, unify --
# deterministic, no backtrack) converted to the suite pair below -- delegate to `run`, which
# extracts and runs each entry alone (a suite file is never run whole, corpus_suite_harness.py's
# own docstring). The other 3 loose siblings (retract_all's retract_loop recursion,
# retract_basic/retract_mixed's X, write(X), nl, fail. multi-solution shape) independently
# signal-11 crash -- PZ-4 (prolog-multiclause-uninit-lexprep-frame). They stay loose ON
# PURPOSE, not KEEP.md (not a permanent design choice, see task file
# tests-consolidate-prolog), and are DELIBERATELY KEPT in this script's own board below rather
# than dropped, so the crash stays visible instead of silently disappearing once PZ-4 lands.
if [ -f "$SNO" ] && [ -f "$REF" ]; then
    out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang prolog --modes m3 2>&1)
    echo "$out" | grep -v '^SUITE_BOARD'
    board=$(echo "$out" | grep '^SUITE_BOARD')
    p=$(echo "$board" | grep -oP 'm3_pass=\K[0-9]+')
    f=$(echo "$board" | grep -oP 'm3_fail=\K[0-9]+')
    c=$(echo "$board" | grep -oP 'm3_crash=\K[0-9]+')
    h=$(echo "$board" | grep -oP 'm3_hang=\K[0-9]+')
    u=$(echo "$board" | grep -oP 'm3_unproven=\K[0-9]+')
    PASS=$((PASS+p)); FAIL=$((FAIL+f+c+h+u))
fi

shopt -s nullglob
for f in "$CORPUS"/rung14_retract_*.pl; do
    ref="${f%.pl}.expected"
    [ -f "$ref" ] || continue
    actual=$(timeout 8 "$SCRIPBIN" --run "$f" < /dev/null 2>/dev/null)
    rc=$?
    expected=$(cat "$ref")
    # ⛔ rc IS PART OF THE VERDICT, NOT JUST STDOUT -- retract_all is the MEASURED witness:
    # it prints the full correct "empty" line and THEN segfaults (rc=139) during cleanup, so a
    # stdout-only compare reads a real, reproducible (5/5) crash as PASS. Same FACT RULE as
    # RULES.md's "a suite that ignores exit status cannot see a wrong exit status."
    if [ "$rc" -eq 0 ] && [ "$actual" = "$expected" ]; then
        echo "  PASS $(basename "$f")"; PASS=$((PASS+1))
    else
        echo "  FAIL $(basename "$f") (loose -- known PZ-4 prolog-multiclause-uninit-lexprep-frame, rc=$rc)"; FAIL=$((FAIL+1))
    fi
done

echo ""
echo "PASS=$PASS FAIL=$FAIL"
# ⛔⭐ ZERO-GRADED IS A REFUSAL, NOT A PASS (hq_B 2026-08-30). BOTH witness arms above are
# conditional -- the consolidated suite pair on [ -f "$SNO" ] && [ -f "$REF" ], the loose
# siblings on a `shopt -s nullglob` loop -- so a corpus deletion empties BOTH silently and the
# bare `[ "$FAIL" -eq 0 ]` below then reported GREEN having measured nothing at all.
# ⛔ MEASURED, not theoretical: corpus fdbe8ff8 ("delete 226 confirmed-redundant source files")
# removed all five rung13_assertz witnesses; test_prolog_rung13.sh then exited rc=0 with
# PASS=0 FAIL=0 -- and it is the FIRST conjunct of the DONE-WHEN of rank-0 keystone row
# prolog-pz4-gamma-retain-activation-frames, so that row's acceptance criterion had a
# vacuously-true term for anyone who ran it. The other rung graders (rung16+) already refuse
# via their own suite-not-found guard; this family only checked [ -d "$CORPUS" ], which is the
# DIRECTORY, never a witness. RULES.md: a test that cannot measure REFUSES with rc=2 --
# never skip-as-success. The guard asks the question that actually matters (did anything get
# graded), not whether one particular file exists, because this family has TWO witness sources.
if [ $((PASS+FAIL)) -eq 0 ]; then
    echo "REFUSE (rc=2): $FAMILY graded ZERO witnesses -- looked for suite pair $SNO / $REF and loose $CORPUS/${FAMILY}_*.pl, found neither. Cannot measure, not a pass."
    exit 2
fi
[ "$FAIL" -eq 0 ]
