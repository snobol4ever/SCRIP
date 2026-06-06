#!/usr/bin/env bash
# test_gate_bb_one_box.sh — PL-HY-FENCE gate (GOAL-PROLOG-BB.md).
#
# ONE BOX PER PROLOG TEMPLATE FILE. Each Prolog-OWNED box file in BB_templates/ must have
# EXACTLY ONE extern "C" void bb_*(…) entry (comments stripped). The bb_*.cpp helper
# files are _str-only (no box entry) and are EXEMPT (count == 0 is correct for them).
#
# Prolog box files: bb_arith, bb_atom, bb_resolve (the router), bb_catch, bb_choice,
#   bb_conj, bb_cut, bb_disj, bb_fail, bb_goal, bb_ite, bb_logicvar, bb_unify.
# Helper files (exempt): bb_aggregate_nb, bb_atom_string, bb_findall,
#   bb_io, bb_is_cmp, bb_list, bb_retract_throw,
#   bb_succ_plus, bb_term_inspect, bb_term_io, bb_type_test.
#
# Usage: bash scripts/test_gate_bb_one_box.sh
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
cd "$ROOT"
fail=0
PL_BOX_FILES="
src/emitter/BB_templates/bb_arith.cpp
src/emitter/BB_templates/bb_atom.cpp
src/emitter/BB_templates/bb_resolve.cpp
src/emitter/BB_templates/bb_catch.cpp
src/emitter/BB_templates/bb_choice.cpp
src/emitter/BB_templates/bb_conj.cpp
src/emitter/BB_templates/bb_cut.cpp
src/emitter/BB_templates/bb_disj.cpp
src/emitter/BB_templates/bb_fail.cpp
src/emitter/BB_templates/bb_goal.cpp
src/emitter/BB_templates/bb_ite.cpp
src/emitter/BB_templates/bb_logicvar.cpp
src/emitter/BB_templates/bb_unify.cpp
"
PL_HELPER_FILES="
src/emitter/BB_templates/bb_aggregate_nb.cpp
src/emitter/BB_templates/bb_atom_string.cpp
src/emitter/BB_templates/bb_findall.cpp
src/emitter/BB_templates/bb_io.cpp
src/emitter/BB_templates/bb_is_cmp.cpp
src/emitter/BB_templates/bb_list.cpp
src/emitter/BB_templates/bb_retract_throw.cpp
src/emitter/BB_templates/bb_succ_plus.cpp
src/emitter/BB_templates/bb_term_inspect.cpp
src/emitter/BB_templates/bb_term_io.cpp
src/emitter/BB_templates/bb_type_test.cpp
"
strip_comments() { perl -0777 -pe 's{/\*.*?\*/}{}gs; s{//[^\n]*}{}g' "$1"; }
for f in $PL_BOX_FILES; do
    [ -f "$f" ] || { echo "FAIL: expected box file missing: $f"; fail=1; continue; }
    n=$(strip_comments "$f" | grep -cE 'extern "C" void bb_[a-z_]+[[:space:]]*\(')
    if [ "$n" -ne 1 ]; then
        echo "FAIL: $f has $n extern \"C\" void bb_* entries (expected exactly 1)"
        fail=1
    fi
done
for f in $PL_HELPER_FILES; do
    [ -f "$f" ] || { echo "FAIL: expected helper file missing: $f"; fail=1; continue; }
    n=$(strip_comments "$f" | grep -cE 'extern "C" void bb_[a-z_]+[[:space:]]*\(')
    if [ "$n" -ne 0 ]; then
        echo "FAIL: helper file $f has $n extern \"C\" void bb_* entries (expected 0; _str helper, not a box)"
        fail=1
    fi
done
if [ "$fail" -eq 0 ]; then
    echo "PASS: every Prolog box file has exactly 1 extern \"C\" void bb_* entry; all helpers have 0."
fi
exit $fail
