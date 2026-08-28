#!/usr/bin/env bash
# test_prolog_rung39_atom_iso.sh — PR-16 driver: ISO atom builtins
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
CORPUS=$S4E/corpus/tests/prolog/rung39_atom_iso
PASS=0; FAIL=0
echo "=== rung39_atom_iso: ISO §7.8 atom builtins (PR-16 driver) ==="
for f in "$CORPUS"/*.pl; do
    ref="${f%.pl}.ref"
    [ -f "$ref" ] || continue
    actual=$(timeout 8 "$SCRIP" --run "$f" < /dev/null 2>/dev/null); rc=$?
    expected=$(cat "$ref")
    if [ "$actual" = "$expected" ] && [ "$rc" -eq 0 ]; then
        echo "  PASS $(basename $f)"; PASS=$((PASS+1))
    else
        echo "  FAIL $(basename $f)"
        echo "    expected: $(cat $ref)"
        echo "    actual:   $actual"
        FAIL=$((FAIL+1))
    fi
done
echo ""
echo "PASS=$PASS FAIL=$FAIL"; [ "$FAIL" -eq 0 ]
