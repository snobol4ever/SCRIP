#!/usr/bin/env bash
# test_snocone_parser_fixtures.sh
#
# Grades the "parser" family (67 entries) via `scrip --dump-ast`, diffed per entry. Consolidated
# from 67 loose file pairs 2026-08-28 (tests-consolidate-snocone, fan-out child of
# corpus-suites-consolidation, done as the LANG_CONFIGS["snocone"] round-trip proof, on top of
# snocone-parser-fixture-ast-drift-ruling's same-day .ref re-pin -- all 67 already-corrected
# fixtures converted with zero exclusions, 0 kept loose). This script previously ran the
# pre-conversion per-file loop directly against corpus/tests/snocone/parser-fixtures/*.sc; that
# tree is now retired (git rm'd, byte-equal re-validated both directions before deletion, per
# corpus-suites-consolidation's method).
#
# ⛔⭐ REPOINTED AGAIN (seat07 2026-08-30, one-flat-suite cutover): the standalone parser.sc/.ref
# pair this gate read after the FIRST retirement was itself ABSORBED into tests/snocone/ALL.sc and
# DELETED once byte-equal-or-no-delete verified it. Reading a now-deleted path would SKIP-as-exit-0
# forever -- the exact "a missing prerequisite must REFUSE, never a green exit" class RULES.md
# names -- so this gate now re-materializes the "parser" family out of the master via
# `extract-family` (family membership from ALL.csv, never a name guess) and grades THAT.
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# ⛔⭐ THIS IS A DRIFT-DETECTOR, NOT A CORRECTNESS ORACLE (ast-dump-refs-are-self-pins-not-oracles,
# 2026-09-05): the .ref here is `scrip --dump-ast` output pinned against SCRIP's own PAST self, not
# against any external oracle -- no oracle (icont, swipl, ...) emits SCRIP's AST shape, so this can
# only ever answer "did the shape change since it was last decided", never "is the shape right". A
# non-zero exit means RE-DECIDE THE SHAPE ON SEMANTICS AND REGENERATE the .ref (does the new shape
# lose information the old one carried, or is it strictly cleaner?) -- it does NOT mean "N programs
# FAIL" and must never be read as a defect count. Never wired into a scored/watermarked board.
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MASTER_SNO="$S4E/corpus/tests/snocone/ALL.sc"
MASTER_REF="$S4E/corpus/tests/snocone/ALL.ref"
MASTER_CSV="$S4E/corpus/tests/snocone/ALL.csv"

echo "=== Snocone parser fixtures (suite) ==="

if [ ! -f "$MASTER_SNO" ] || [ ! -f "$MASTER_REF" ] || [ ! -f "$MASTER_CSV" ]; then
    echo "REFUSES rc=2: master not found at $MASTER_SNO / $MASTER_REF / $MASTER_CSV -- cannot measure."
    exit 2
fi

W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
if ! python3 "$HERE/corpus_suite_harness.py" extract-family "$MASTER_SNO" "$MASTER_REF" "$MASTER_CSV" parser "$W/parser.sc" "$W/parser.ref" >&2; then
    echo "REFUSES rc=2: could not extract family 'parser' from the master -- see stderr above."
    exit 2
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$W/parser.sc" "$W/parser.ref" --lang snocone 2>&1)
rc=$?
echo "$out"
if [ "$rc" -ne 0 ]; then
    echo "⛔ AST shape changed vs the pinned self-referential .ref -- RE-DECIDE THE SHAPE AND REGENERATE"
    echo "   (ast-dump-refs-are-self-pins-not-oracles). This is drift, not a correctness verdict."
fi
[ "$rc" -eq 0 ]
