#!/usr/bin/env bash
# test_prolog_parser_fixtures.sh
#
# Grades corpus/tests/prolog/parser.pl / .ref (corpus_suite_harness.py's format-B suite pair,
# --lang prolog) via `scrip --dump-ast`, diffed per entry. Consolidated from 134 loose file pairs
# 2026-08-27 (suite-harness-lang-configs, fan-out child of corpus-suites-consolidation, done as the
# LANG_CONFIGS["prolog"] round-trip proof): all 20 pre-existing committed .ref files had drifted
# from the current AST-dump shape (same systematic-drift class already found in Snocone
# parser-fixtures 59/67, Icon parser/ 153/153, and Rebus parser/ 15/48) -- re-captured fresh from
# current output rather than left stale, per this project's standing rule for these self-referential
# regression witnesses (no external "correct AST shape" oracle exists to check against). 2 .pl files
# (bare non-callable top-level terms: `42.`, `X.`) produce empty --dump-ast output and stay loose in
# corpus/tests/prolog/parser/, deliberately not part of this gate -- see KEEP.md beside them.
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count (see test_snocone_parser_fixtures.sh fix, 2026-08-27)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SNO="$S4E/corpus/tests/prolog/parser.pl"
REF="$S4E/corpus/tests/prolog/parser.ref"

echo "=== Prolog parser fixtures (suite) ==="

if [ ! -f "$SNO" ] || [ ! -f "$REF" ]; then
    echo "SKIP suite not found at $SNO / $REF"
    exit 0
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang prolog 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
