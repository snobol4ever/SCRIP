#!/usr/bin/env bash
# test_raku_parser_fixtures.sh
#
# Grades corpus/tests/raku/parser.raku / .ref (corpus_suite_harness.py's format-B suite pair,
# --lang raku) via `scrip --dump-ast`, diffed per entry. Consolidated from 83 loose file pairs
# 2026-08-27 (corpus-suites-consolidation); 14 known-bad stems (class/OOP + gather/take -- not
# yet supported by the compiler) and 50 .raku files with no committed .ref stay loose in
# corpus/tests/raku/parser/, deliberately not part of this gate.
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count (see test_snocone_parser_fixtures.sh fix, 2026-08-27)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SNO="$S4E/corpus/tests/raku/parser.raku"
REF="$S4E/corpus/tests/raku/parser.ref"

echo "=== Raku parser fixtures (suite) ==="

if [ ! -f "$SNO" ] || [ ! -f "$REF" ]; then
    echo "SKIP suite not found at $SNO / $REF"
    exit 0
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang raku 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
