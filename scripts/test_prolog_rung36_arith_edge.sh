#!/usr/bin/env bash
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
CORPUS=$S4E/corpus/tests/prolog
RUNG="$CORPUS/rung36_arith_edge"

echo "=== rung36_arith_edge: ISO §8 arithmetic edge cases (PR-13 driver) ==="

if [ ! -d "$RUNG" ]; then echo "SKIP  rung36 not found"; exit 0; fi
if [ ! -f "$SCRIP" ]; then echo "SKIP  scrip not found"; exit 0; fi

PASS=0; FAIL=0
for f in "$RUNG"/*.pl; do
    ref="${f%.pl}.ref"
    [ -f "$ref" ] || continue
    actual=$(timeout 8 "$SCRIP" --run "$f" < /dev/null 2>/dev/null)
    expected=$(cat "$ref")
    if [ "$actual" = "$expected" ]; then
        echo "  PASS $(basename $f)"; PASS=$((PASS+1))
    else
        echo "  FAIL $(basename $f)"
        echo "    expected: $(echo "$expected" | head -3)"
        echo "    actual:   $(echo "$actual"   | head -3)"
        FAIL=$((FAIL+1))
    fi
done
echo ""
echo "PASS=$PASS FAIL=$FAIL"
[ "$FAIL" -eq 0 ]
