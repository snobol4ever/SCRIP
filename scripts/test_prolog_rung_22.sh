#!/usr/bin/env bash
# test_prolog_rung_22.sh — write_canonical/writeq builtins (PL-8)
# Gate: PASS=5 FAIL=0
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
CORPUS=$S4E/corpus/programs/prolog/rung22
PASS=0; FAIL=0
echo "=== rung22: write_canonical / writeq ==="
for f in "$CORPUS"/*.pl; do
    ref="${f%.pl}.ref"
    [ -f "$ref" ] || continue
    actual=$(timeout 8 "$SCRIP" --run "$f" 2>/dev/null)
    expected=$(cat "$ref")
    if [ "$actual" = "$expected" ]; then
        echo "  PASS $(basename "$f")"; PASS=$((PASS+1))
    else
        echo "  FAIL $(basename "$f")"
        echo "    expected: $(echo "$expected" | head -2)"
        echo "    actual:   $(echo "$actual"   | head -2)"
        FAIL=$((FAIL+1))
    fi
done
echo ""
echo "PASS=$PASS FAIL=$FAIL"; [ "$FAIL" -eq 0 ]
