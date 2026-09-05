#!/usr/bin/env bash
# test_prolog_parser_fixtures.sh
#
# Grades the "parser" family (134 entries) via real execution (--run and --compile), diffed per
# entry against expected stdout. Consolidated from 134 loose file pairs 2026-08-27
# (suite-harness-lang-configs, fan-out child of corpus-suites-consolidation, done as the
# LANG_CONFIGS["prolog"] round-trip proof). 2 .pl files (bare non-callable top-level terms: `42.`,
# `X.`) produce empty output either way and stay loose in corpus/tests/prolog/parser/, deliberately
# not part of this gate -- see KEEP.md beside them.
#
# ⛔⭐ REPOINTED (2026-09-05, ast-dump-refs-are-self-pins-not-oracles): the standalone
# corpus/tests/prolog/parser.pl/.ref pair this gate used to read was absorbed into tests/prolog/ALL.pl
# and deleted during the one-flat-suite cutover -- this gate was never repointed after that, so it
# had been reading a deleted path and SKIPping as exit 0 (a silent vacuous pass) ever since, the exact
# "a missing prerequisite must REFUSE, never a green exit" class RULES.md names. It now re-materializes
# the "parser" family out of the master via `extract-family` (family membership from ALL.csv, never a
# name guess) and grades THAT, same pattern as its Snocone/Rebus siblings.
#
# ⛔ NO LONGER A DRIFT-DETECTOR, AND NEVER SHOULD HAVE STAYED ONE (ast-dump-refs-are-self-pins-not-
# oracles): all 134 of this family's .ref entries WERE `scrip --dump-ast` self-pins, but corpus
# 6a9f01fe4 (2026-09-02) already re-captured every one as real swipl/scrip-run output (all 134 are
# clause-only files with no top-level goal; verified per-entry against swipl, not assumed from the
# pattern -- 133 genuinely silent, 1 non-empty). That commit changed ALL.ref correctly but left
# tests/prolog/config/MODES.tsv's `parser` row declaring `ast`, so any honest re-grading of this
# family via --dump-ast FAILed all 134 by construction (a real-output ref can never equal an AST
# dump) -- exactly the self-pin-vs-oracle confusion this ruling exists to end. Fixed the same day:
# MODES.tsv now declares `parser	m3,m4`, ALL.csv reindexed to match (`--reindex`, CSV-only, ALL.pl/
# ALL.ref untouched), and this gate now grades by real execution, a genuine oracle-backed check.
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count
# drifts; a probe asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
# ⚠️ ONE KNOWN RED, NOT A SELF-PIN ARTIFACT: simple_program_87 (`plus/2`) FAILs m3 and SKIPs m4 --
# `plus` is a real SWI builtin not yet wired (rung 6 lands it per ARCH-PROLOG-BYRD-BOX-TRANSLATION.md
# sec E; Prolog rebuilds from rung 0, THERE IS NO XFAIL under FLEET-20 so this is left plainly red
# rather than marked around) -- hq_C/hq_P's lane, not this row's to cure.
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MASTER_PL="$S4E/corpus/tests/prolog/ALL.pl"
MASTER_REF="$S4E/corpus/tests/prolog/ALL.ref"
MASTER_CSV="$S4E/corpus/tests/prolog/ALL.csv"

echo "=== Prolog parser fixtures (suite) ==="

if [ ! -f "$MASTER_PL" ] || [ ! -f "$MASTER_REF" ] || [ ! -f "$MASTER_CSV" ]; then
    echo "REFUSES rc=2: master not found at $MASTER_PL / $MASTER_REF / $MASTER_CSV -- cannot measure."
    exit 2
fi

W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
if ! python3 "$HERE/corpus_suite_harness.py" extract-family "$MASTER_PL" "$MASTER_REF" "$MASTER_CSV" parser "$W/parser.pl" "$W/parser.ref" >&2; then
    echo "REFUSES rc=2: could not extract family 'parser' from the master -- see stderr above."
    exit 2
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$W/parser.pl" "$W/parser.ref" --lang prolog --modes m3,m4 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
