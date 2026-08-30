#!/usr/bin/env bash
# test_snocone_corpus_suite.sh
#
# Grades the "corpus" family (10 entries) via execution (--run and --compile), diffed per entry
# against expected stdout. Consolidated from 10 loose file pairs 2026-08-28
# (tests-consolidate-snocone, fan-out child of corpus-suites-consolidation): sc1_literals..
# sc10_wordcount, a small language-feature coverage ladder (literals/assign/arith/control/while/
# for/procedure/strings/multiproc/wordcount). Unlike the parser-fixtures suite (graded by
# --dump-ast shape), this family's .ref files hold program OUTPUT, not AST dumps -- hence the
# explicit --modes m3,m4 rather than the LANG_CONFIGS default (ast). 4 of the 10 also needed a
# source fix before conversion (bare relop sugar deliberately removed 2026-08-24 -- same class
# already found in parser-fixtures and crosscheck/rungB09-12); once fixed, all 10 already matched
# their existing .ref content exactly, so nothing needed re-pinning.
#
# ⛔⭐ REPOINTED (seat07 2026-08-30, one-flat-suite cutover): the standalone corpus.sc/.ref pair
# this gate used to read was ABSORBED into tests/snocone/ALL.sc and DELETED once byte-equal-or-no-
# delete verified it. Reading a now-deleted path would SKIP-as-exit-0 forever -- the exact "a
# missing prerequisite must REFUSE, never a green exit" class RULES.md names -- so this gate now
# re-materializes the "corpus" family out of the master via `extract-family` (family membership
# from ALL.csv, never a name guess) and grades THAT.
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MASTER_SNO="$S4E/corpus/tests/snocone/ALL.sc"
MASTER_REF="$S4E/corpus/tests/snocone/ALL.ref"
MASTER_CSV="$S4E/corpus/tests/snocone/ALL.csv"

echo "=== Snocone corpus suite (m3+m4) ==="

if [ ! -f "$MASTER_SNO" ] || [ ! -f "$MASTER_REF" ] || [ ! -f "$MASTER_CSV" ]; then
    echo "REFUSES rc=2: master not found at $MASTER_SNO / $MASTER_REF / $MASTER_CSV -- cannot measure."
    exit 2
fi

W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
if ! python3 "$HERE/corpus_suite_harness.py" extract-family "$MASTER_SNO" "$MASTER_REF" "$MASTER_CSV" corpus "$W/corpus.sc" "$W/corpus.ref" >&2; then
    echo "REFUSES rc=2: could not extract family 'corpus' from the master -- see stderr above."
    exit 2
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$W/corpus.sc" "$W/corpus.ref" --lang snocone --modes m3,m4 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
