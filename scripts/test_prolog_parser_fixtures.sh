#!/usr/bin/env bash
# test_prolog_parser_fixtures.sh
#
# Grades the "parser" family (134 entries) of the prolog master via `scrip --dump-ast`, diffed per entry.
# Consolidated from 134 loose file pairs 2026-08-27 (suite-harness-lang-configs, fan-out child of
# corpus-suites-consolidation, done as the LANG_CONFIGS["prolog"] round-trip proof): all 20 pre-existing
# committed .ref files had drifted from the current AST-dump shape (the same systematic-drift class found in
# Snocone parser-fixtures 59/67, Icon parser/ 153/153, Rebus parser/ 15/48) -- re-captured fresh from current
# output rather than left stale, per this project's standing rule for these self-referential regression
# witnesses (no external oracle emits SCRIP's AST, so a fixture ref is a SELF-PIN: it proves the parse has not
# MOVED, never that it is RIGHT). 2 .pl files (bare non-callable top-level terms: `42.`, `X.`) produce empty
# --dump-ast output and stay loose in corpus/tests/prolog/parser/, deliberately not part of this gate --
# see KEEP.md beside them.
#
# ⛔⭐⭐ REPOINTED (hq_T 2026-09-05) — AND THE OLD BODY IS THE MEASURED REASON THIS MATTERS. This gate still read
# the standalone tests/prolog/parser.{pl,ref}, which the one-flat-suite absorption DELETED. Its own words for
# that case were `echo "SKIP suite not found"; exit 0` -- so from the day its subject moved, the gate that owned
# 134 fixtures reported SUCCESS while grading nothing, which is the "a missing prerequisite must REFUSE, never
# a green exit" class RULES.md names. The other three fixture gates (rebus, snocone, raku) were repointed at
# their masters on 2026-08-30 and refuse rc=2; prolog was the one the campaign missed.
# ⭐ WHAT THE DARKNESS HID: the absorption carried the 134 SOURCES into the master and left their REFS behind.
# 133 refs were EMPTY and the 134th held `starting` -- the RUNTIME output of an entry whose own family says it
# is never run. Graded by --dump-ast against nothing, all 134 failed by construction: ast_fail=134/134 on every
# prolog board, for over a week, with this gate green beside it. seat07 measured the 134/134 and correctly
# called it pre-existing; nobody could see the cause, because the instrument that would have named it was
# reporting success. Refs cut 2026-09-05 (134/134 dumps rc=0, deterministic over two runs, every one `(STMT`);
# test_gate_ast_declared_refs_are_ast_dumps.sh now holds the invariant so an empty ast ref cannot return.
#
# Gate: FAIL=0 over the printed total (do not pin a specific total here -- the fixture count drifts; a probe
# asserts FAIL=0/SKIP=0 over its own denominator, never a copied number -- RULES.md).
#
# Commit identity: LCherryholmes / lcherryh@yahoo.com  (RULES.md)
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME

set -uo pipefail   # deliberately NOT -e: this script's own diagnostic must never mask a fail count (see test_snocone_parser_fixtures.sh fix, 2026-08-27)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MASTER_SNO="$S4E/corpus/tests/prolog/ALL.pl"
MASTER_REF="$S4E/corpus/tests/prolog/ALL.ref"
MASTER_CSV="$S4E/corpus/tests/prolog/ALL.csv"

echo "=== Prolog parser fixtures (suite) ==="

if [ ! -f "$MASTER_SNO" ] || [ ! -f "$MASTER_REF" ] || [ ! -f "$MASTER_CSV" ]; then
    echo "REFUSES rc=2: master not found at $MASTER_SNO / $MASTER_REF / $MASTER_CSV -- cannot measure."
    exit 2
fi

W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
if ! python3 "$HERE/corpus_suite_harness.py" extract-family "$MASTER_SNO" "$MASTER_REF" "$MASTER_CSV" parser "$W/parser.pl" "$W/parser.ref" >&2; then
    echo "REFUSES rc=2: could not extract family 'parser' from the master -- see stderr above."
    exit 2
fi

out=$(python3 "$HERE/corpus_suite_harness.py" run "$W/parser.pl" "$W/parser.ref" --lang prolog 2>&1)
rc=$?
echo "$out"
[ "$rc" -eq 0 ]
