#!/usr/bin/env bash
# test_prolog_rung_25.sh — PL-9 string/IO builtins
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
CORPUS=$S4E/corpus/programs/prolog/rung25
PASS=0; FAIL=0
echo "=== rung25 ==="
for f in "$CORPUS"/*.pl; do
    ref="${f%.pl}.ref"; [ -f "$ref" ] || continue
    actual=$(timeout 8 "$SCRIP" --run "$f" 2>/dev/null)
    expected=$(cat "$ref")
    if [ "$actual" = "$expected" ]; then echo "  PASS $(basename "$f")"; PASS=$((PASS+1))
    else echo "  FAIL $(basename "$f")"; FAIL=$((FAIL+1)); fi
done
echo ""; echo "PASS=$PASS FAIL=$FAIL"; [ "$FAIL" -eq 0 ]
