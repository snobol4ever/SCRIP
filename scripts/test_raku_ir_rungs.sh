#!/bin/bash
# scripts/test_raku_ir_rungs.sh — Raku --run test harness
# Self-contained. Run from anywhere with no env vars required.
# Mirrors test_icon_all_rungs.sh structure.
#
# Runs all test/raku/*.raku files against scrip --run and compares
# output to the corresponding .expected file.
#
# Gate: PASS=13 FAIL=0
#
# roast reference: github.com/Raku/roast — official Raku test suite.
# Our hand-written corpus covers the Tiny-Raku subset only (RK-9).
#
# Authors: LCherryholmes · Claude Sonnet 4.6

set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO="$(cd "$HERE/.." && pwd)"
SCRIP="${SCRIP:-$REPO/scrip}"
TESTDIR="$REPO/test/raku"

if [ ! -x "$SCRIP" ]; then
    echo "SKIP scrip binary not found at $SCRIP" >&2
    exit 0
fi
if [ ! -d "$TESTDIR" ]; then
    echo "⛔ REFUSING (rc=2): $TESTDIR not found -- test/raku is retired, this script cannot measure" >&2
    exit 2
fi

PASS=0; FAIL=0

run_one() {
    local raku="$1"
    local exp="${raku%.raku}.expected"
    [ -f "$exp" ] || { echo "SKIP $(basename "$raku") (no .expected)"; return; }
    local name got want
    name=$(basename "$raku" .raku)
    got=$(timeout 8 "$SCRIP" --run "$raku" < /dev/null 2>/dev/null) || true
    want=$(cat "$exp")
    if [ "$got" = "$want" ]; then
        echo "PASS $name"
        PASS=$((PASS+1))
    else
        echo "FAIL $name"
        echo "  want: $(echo "$want" | tr '\n' '|')"
        echo "  got:  $(echo "$got"  | tr '\n' '|')"
        FAIL=$((FAIL+1))
    fi
}

for raku in "$TESTDIR"/*.raku; do
    [ -f "$raku" ] || continue
    run_one "$raku"
done

echo ""
echo "--- Raku --run: PASS=$PASS FAIL=$FAIL TOTAL=$((PASS+FAIL)) ---"
[ "$FAIL" -eq 0 ]
