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
[ "$FAIL" -eq 0 ]
