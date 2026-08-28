#!/usr/bin/env bash
# test_snocone_corpus_suite.sh
#
# Grades corpus/tests/snocone/corpus.sc / .ref (corpus_suite_harness.py's format-B suite pair,
# --lang snocone --modes m3,m4) via execution (--run and --compile), diffed per entry against
# expected stdout. Consolidated from 10 loose file pairs 2026-08-28 (tests-consolidate-snocone,
# fan-out child of corpus-suites-consolidation): sc1_literals..sc10_wordcount, a small
# language-feature coverage ladder (literals/assign/arith/control/while/for/procedure/strings/
# multiproc/wordcount). Unlike the parser-fixtures suite (graded by --dump-ast shape), this
# family's .ref files hold program OUTPUT, not AST dumps -- hence the explicit --modes m3,m4
# rather than the LANG_CONFIGS default (ast). 4 of the 10 also needed a source fix before
# conversion (bare relop sugar deliberately removed 2026-08-24 -- same class already found in
# parser-fixtures and crosscheck/rungB09-12); once fixed, all 10 already matched their existing
# .ref content exactly, so nothing needed re-pinning.
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SNO="$S4E/corpus/tests/snocone/corpus.sc"
REF="$S4E/corpus/tests/snocone/corpus.ref"

echo "=== Snocone corpus suite (m3+m4) ==="

if [ ! -f "$SNO" ] || [ ! -f "$REF" ]; then
    echo "SKIP suite not found at $SNO / $REF"
    exit 0
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang snocone --modes m3,m4 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
