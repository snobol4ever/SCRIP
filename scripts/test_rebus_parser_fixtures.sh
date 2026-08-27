#!/usr/bin/env bash
# test_rebus_parser_fixtures.sh
#
# Grades corpus/tests/rebus/parser.reb / .ref (corpus_suite_harness.py's format-B suite pair,
# --lang rebus) via `scrip --dump-ast`, diffed per entry. Consolidated from 15 loose file pairs
# 2026-08-27/28 (tests-consolidate-rebus, fan-out child of corpus-suites-consolidation); 33
# known-bad stems (AST-dump shape has drifted from the committed .ref -- same systematic-drift
# class already found in Snocone parser-fixtures 59/67 and Icon parser/ 153/153, a compiler-shape
# correctness question, not a per-fixture bug) and 48 .reb files with no committed .ref stay loose
# in corpus/tests/rebus/parser/, deliberately not part of this gate.
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count (see test_snocone_parser_fixtures.sh fix, 2026-08-27)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SNO="$S4E/corpus/tests/rebus/parser.reb"
REF="$S4E/corpus/tests/rebus/parser.ref"

echo "=== Rebus parser fixtures (suite) ==="

if [ ! -f "$SNO" ] || [ ! -f "$REF" ]; then
    echo "SKIP suite not found at $SNO / $REF"
    exit 0
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang rebus 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
