#!/usr/bin/env bash
# test_prolog_rung28.sh — PL-10 throw/1, catch/3
# D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CORPUS=$S4E/corpus/tests/prolog
FAMILY=rung28
SNO="$CORPUS/$FAMILY.pl"
REF="$CORPUS/$FAMILY.ref"

echo "=== rung28: throw/catch exceptions ==="

# consolidated 2026-08-29 (tests-consolidate-prolog, hq_P): the loose $CORPUS/$FAMILY/*.pl
# directory this script used to glob was replaced by one suite pair. ⛔ THE OLD FORM WENT
# FALSE-GREEN THE MOMENT THE DIRECTORY EMPTIED -- an unmatched glob left PASS=0 FAIL=0 and
# the trailing [ "$FAIL" -eq 0 ] exited 0, reporting success having run nothing (measured on
# rung27/rung28/rung39_atom_iso at conversion time). A suite file is NEVER run whole
# (corpus_suite_harness.py's own docstring) -- delegate to `run`, which extracts and runs
# each entry alone in its own temp dir, and REFUSE rc=2 when the suite is absent.
if [ ! -f "$SNO" ] || [ ! -f "$REF" ]; then
    echo "REFUSE (rc=2): $FAMILY suite not found (expected $SNO / $REF) -- cannot measure, not a pass"; exit 2
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang prolog --modes m3,m4 2>&1)
echo "$out" | grep -v '^SUITE_BOARD'
board=$(echo "$out" | grep '^SUITE_BOARD')
[ -n "$board" ] || { echo "REFUSE (rc=2): harness produced no SUITE_BOARD line -- cannot measure, not a pass"; exit 2; }
BAD=0; PASSN=0
for m in m3 m4; do
  for k in fail crash hang unproven; do BAD=$((BAD + $(echo "$board" | grep -oP "${m}_${k}=\K[0-9]+"))); done
  PASSN=$((PASSN + $(echo "$board" | grep -oP "${m}_pass=\K[0-9]+")))
done
echo ""
echo "PASS=$PASSN FAIL=$BAD"
[ "$BAD" -eq 0 ]
