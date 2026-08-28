#!/usr/bin/env bash
# test_prolog_rung33_bridge_callN.sh — PR-19c driver tests for call/1 and call/N
# with goal-as-variable. Extends the v3 Term→EXPR bridge to call/N dispatch.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
CORPUS=$S4E/corpus/tests/prolog/rung33_bridge_callN
PASS=0; FAIL=0
echo "=== rung33_bridge_callN: call/1 and call/N with goal-as-variable (PR-19c driver) ==="
if [ ! -d "$CORPUS" ]; then
    echo "SKIP: corpus dir not present at $CORPUS"
    exit 0
fi
for f in "$CORPUS"/*.pl; do
    ref="${f%.pl}.ref"; [ -f "$ref" ] || continue
    actual=$(timeout 8 "$SCRIP" --run "$f" < /dev/null 2>/dev/null); rc=$?
    expected=$(cat "$ref")
    if [ "$actual" = "$expected" ] && [ "$rc" -eq 0 ]; then
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
