#!/usr/bin/env bash
# test_snocone_parser_fixtures.sh — SI-7 gate
#
# Runs `scrip --dump-ast` on every .sc in corpus/programs/snocone/parser-fixtures/
# and diffs the output against the corresponding .ref oracle.
#
# Gate: PASS=67 FAIL=0  (all fixtures, byte-identical AST dump)
#
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${HERE}/../scrip"
FIXTURES="$S4E/corpus/programs/snocone/parser-fixtures"
PASS=0; FAIL=0; SKIP=0

echo "=== Snocone parser fixtures ==="

if [ ! -d "$FIXTURES" ]; then
    echo "SKIP corpus not found at $FIXTURES"
    exit 0
fi

for sc in "$FIXTURES"/*.sc; do
    ref="${sc%.sc}.ref"
    name="$(basename "${sc%.sc}")"
    if [ ! -f "$ref" ]; then
        echo "  SKIP $name (no .ref)"
        SKIP=$((SKIP+1))
        continue
    fi
    actual=$(timeout 8 "$SCRIP" --dump-ast "$sc" 2>/dev/null)
    expected=$(cat "$ref")
    if [ "$actual" = "$expected" ]; then
        echo "  PASS $name"
        PASS=$((PASS+1))
    else
        echo "  FAIL $name"
        diff <(printf '%s\n' "$expected") <(printf '%s\n' "$actual") | head -12
        FAIL=$((FAIL+1))
    fi
done

echo ""
echo "PASS=$PASS FAIL=$FAIL SKIP=$SKIP"
[ "$FAIL" -eq 0 ]
