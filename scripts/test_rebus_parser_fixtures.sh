#!/usr/bin/env bash
# test_rebus_parser_fixtures.sh
#
# Grades the "parser" family (15 entries) via `scrip --dump-ast`, diffed per entry. Consolidated
# from 15 loose file pairs 2026-08-27/28 (tests-consolidate-rebus, fan-out child of
# corpus-suites-consolidation); 33 known-bad stems (AST-dump shape has drifted from the committed
# .ref -- same systematic-drift class already found in Snocone parser-fixtures 59/67 and Icon
# parser/ 153/153, a compiler-shape correctness question, not a per-fixture bug) absorbed
# separately into the master as their own single-entry families, and 48 .reb files with no
# committed .ref stay loose (config/parser_KEEP.md), deliberately not part of this gate.
#
# ⛔⭐ REPOINTED (seat07 2026-08-30, one-flat-suite cutover): the standalone tests/rebus/parser.reb
# this gate used to read was ABSORBED into tests/rebus/ALL.reb and DELETED once byte-equal-or-no-
# delete verified it (corpus-suites-consolidation.task.md, THE ONE-FLAT-SUITE RULING). Reading a
# now-deleted path would SKIP-as-exit-0 forever -- the exact "a missing prerequisite must REFUSE,
# never a green exit" class RULES.md names -- so this gate now re-materializes the "parser" family
# out of the master via `extract-family` (family membership from ALL.csv, never a name guess) and
# grades THAT, reproducing this gate's original scope exactly (verified: extracted pair grades
# total=15 ast_pass=15 ast_fail=0, matching the family's original conversion receipt).
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count (see test_snocone_parser_fixtures.sh fix, 2026-08-27)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MASTER_SNO="$S4E/corpus/tests/rebus/ALL.reb"
MASTER_REF="$S4E/corpus/tests/rebus/ALL.ref"
MASTER_CSV="$S4E/corpus/tests/rebus/ALL.csv"

echo "=== Rebus parser fixtures (suite) ==="

if [ ! -f "$MASTER_SNO" ] || [ ! -f "$MASTER_REF" ] || [ ! -f "$MASTER_CSV" ]; then
    echo "REFUSES rc=2: master not found at $MASTER_SNO / $MASTER_REF / $MASTER_CSV -- cannot measure."
    exit 2
fi

W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
if ! python3 "$HERE/corpus_suite_harness.py" extract-family "$MASTER_SNO" "$MASTER_REF" "$MASTER_CSV" parser "$W/parser.reb" "$W/parser.ref" >&2; then
    echo "REFUSES rc=2: could not extract family 'parser' from the master -- see stderr above."
    exit 2
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$W/parser.reb" "$W/parser.ref" --lang rebus 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
