#!/usr/bin/env bash
# test_snocone_parser_fixtures.sh
#
# Grades corpus/tests/snocone/parser.sc / .ref (corpus_suite_harness.py's format-B suite pair,
# --lang snocone) via `scrip --dump-ast`, diffed per entry. Consolidated from 67 loose file pairs
# 2026-08-28 (tests-consolidate-snocone, fan-out child of corpus-suites-consolidation, done as the
# LANG_CONFIGS["snocone"] round-trip proof, on top of snocone-parser-fixture-ast-drift-ruling's
# same-day .ref re-pin -- all 67 already-corrected fixtures converted with zero exclusions, 0 kept
# loose). This script previously ran the pre-conversion per-file loop directly against
# corpus/tests/snocone/parser-fixtures/*.sc; that tree is now retired (git rm'd, byte-equal
# re-validated both directions before deletion, per corpus-suites-consolidation's method).
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SNO="$S4E/corpus/tests/snocone/parser.sc"
REF="$S4E/corpus/tests/snocone/parser.ref"

echo "=== Snocone parser fixtures (suite) ==="

if [ ! -f "$SNO" ] || [ ! -f "$REF" ]; then
    echo "SKIP suite not found at $SNO / $REF"
    exit 0
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang snocone 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
