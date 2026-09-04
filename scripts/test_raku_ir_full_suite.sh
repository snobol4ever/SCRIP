#!/usr/bin/env bash
# scripts/test_raku_ir_full_suite.sh — Raku master-suite AST+run parity sweep via corpus_suite_harness.py.
# Re-pointed off the retired test/raku rung tree (row test-raku-ir-full-suite-skips-rc-0-when-its-population-
# directory-is-absent): the old TRACK-4 population never existed under the one-flat-suite corpus reorg, so this
# always printed "SKIP ... rc=0" -- a never-ran reading as green (GOAL-CEO.md CEO-20, FINDING-2026-08-30-hq_B).
# Population is the master pair corpus/tests/raku/ALL.raku + ALL.ref. An absent population REFUSES rc=2 -- it
# is never silently skipped.
# ⛔ MUST pass --by-modes-column --modes m3,m4: ALL.csv's modes column is 762 "m3,m4" (run-graded) vs 97 "ast"
# (ast-dump-graded) of 859 total. A bare `--lang raku` collapses both into a single ast-only grade and diffs
# every run-oracle entry against --dump-ast output it was never meant to match -- a full, plausible, entirely
# false board (measured 2026-09-04: bare invocation read FAIL=620; the true board is FAIL=2, one entry in each
# of m3/m4). See corpus_suite_harness.py's own "MIRROR TRAP" comments -- the identical collapse independently
# hit Pascal/Snocone/Icon the same day; this file has one shared root cause, not four.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SNO="$S4E/corpus/tests/raku/ALL.raku"
REF="$S4E/corpus/tests/raku/ALL.ref"

if [ ! -f "$SNO" ] || [ ! -f "$REF" ]; then
    echo "⛔ REFUSING (rc=2): raku master suite population absent -- expected $SNO + $REF" >&2
    exit 2
fi

if out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang raku --modes m3,m4 --by-modes-column 2>&1); then rc=0; else rc=$?; fi
echo "$out"
ast=$(printf '%s\n' "$out" | grep '^SUITE_BOARD_AST ' || true)
run=$(printf '%s\n' "$out" | grep '^SUITE_BOARD ' || true)
ap=$(printf '%s\n' "$ast" | grep -oE 'ast_pass=[0-9]+' | head -1 | cut -d= -f2 || true)
af=$(printf '%s\n' "$ast" | grep -oE 'ast_fail=[0-9]+' | head -1 | cut -d= -f2 || true)
m3p=$(printf '%s\n' "$run" | grep -oE 'm3_pass=[0-9]+' | head -1 | cut -d= -f2 || true)
m3f=$(printf '%s\n' "$run" | grep -oE 'm3_fail=[0-9]+' | head -1 | cut -d= -f2 || true)
m4p=$(printf '%s\n' "$run" | grep -oE 'm4_pass=[0-9]+' | head -1 | cut -d= -f2 || true)
m4f=$(printf '%s\n' "$run" | grep -oE 'm4_fail=[0-9]+' | head -1 | cut -d= -f2 || true)
pass=$(( ${ap:-0} + ${m3p:-0} + ${m4p:-0} ))
fail=$(( ${af:-0} + ${m3f:-0} + ${m4f:-0} ))
echo "PASS=${pass} FAIL=${fail}"
exit "$rc"
