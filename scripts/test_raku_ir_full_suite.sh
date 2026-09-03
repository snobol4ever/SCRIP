#!/usr/bin/env bash
# scripts/test_raku_ir_full_suite.sh — Raku master-suite AST-parity sweep via corpus_suite_harness.py.
# Re-pointed off the retired test/raku rung tree (row test-raku-ir-full-suite-skips-rc-0-when-its-population-
# directory-is-absent): the old TRACK-4 population never existed under the one-flat-suite corpus reorg, so this
# always printed "SKIP ... rc=0" -- a never-ran reading as green (GOAL-CEO.md CEO-20, FINDING-2026-08-30-hq_B).
# Population is now the master pair corpus/tests/raku/ALL.raku + ALL.ref, graded in "ast" mode
# (LANG_CONFIGS["raku"]["modes"]). An absent population REFUSES rc=2 -- it is never silently skipped.
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"
SNO="$S4E/corpus/tests/raku/ALL.raku"
REF="$S4E/corpus/tests/raku/ALL.ref"

if [ ! -f "$SNO" ] || [ ! -f "$REF" ]; then
    echo "⛔ REFUSING (rc=2): raku master suite population absent -- expected $SNO + $REF" >&2
    exit 2
fi

if out=$(python3 "$HERE/corpus_suite_harness.py" run "$SNO" "$REF" --lang raku 2>&1); then rc=0; else rc=$?; fi
echo "$out"
board=$(printf '%s\n' "$out" | grep '^SUITE_BOARD ' || true)
pass=$(printf '%s\n' "$board" | grep -oE 'ast_pass=[0-9]+' | head -1 | cut -d= -f2 || true)
fail=$(printf '%s\n' "$board" | grep -oE 'ast_fail=[0-9]+' | head -1 | cut -d= -f2 || true)
echo "PASS=${pass:-0} FAIL=${fail:-0}"
exit "$rc"
