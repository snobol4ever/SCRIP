#!/usr/bin/env bash
# test_prolog_rung36_arith_edge.sh
# D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CORPUS=$S4E/corpus/tests/prolog
FAMILY=rung36_arith_edge
SNO="$CORPUS/$FAMILY.pl"
REF="$CORPUS/$FAMILY.ref"

echo "=== rung36_arith_edge: ISO §8 arithmetic edge cases (PR-13 driver) ==="

# consolidated 2026-08-27 (tests-consolidate-prolog): the loose $CORPUS/$FAMILY/*.pl
# directory this script used to glob was replaced by one suite pair. A suite file is
# NEVER run whole (corpus_suite_harness.py's own docstring) -- delegate to `run`,
# which extracts and runs each entry alone in its own temp dir.
if [ ! -f "$SNO" ] || [ ! -f "$REF" ]; then
    echo "REFUSE (rc=2): $FAMILY suite not found (expected $SNO / $REF) -- cannot measure, not a pass"; exit 2
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang prolog --modes m3 2>&1)
echo "$out" | grep -v '^SUITE_BOARD'
board=$(echo "$out" | grep '^SUITE_BOARD')
pass=$(echo "$board" | grep -oP 'm3_pass=\K[0-9]+')
fail=$(echo "$board" | grep -oP 'm3_fail=\K[0-9]+')
crash=$(echo "$board" | grep -oP 'm3_crash=\K[0-9]+')
hang=$(echo "$board" | grep -oP 'm3_hang=\K[0-9]+')
unproven=$(echo "$board" | grep -oP 'm3_unproven=\K[0-9]+')
BAD=$((fail+crash+hang+unproven))
echo ""
echo "PASS=$pass FAIL=$BAD"
[ "$BAD" -eq 0 ]
