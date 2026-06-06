#!/usr/bin/env bash
# test_gate_bb_one_box.sh — PL-HY-FENCE + ICN-HY-FENCE gate (GOAL-PROLOG-BB.md, GOAL-ICON-BB.md).
#
# ONE BOX PER TEMPLATE FILE. Each language-OWNED box file in BB_templates/ must have
# EXACTLY ONE extern "C" void bb_*(…) entry (comments stripped). The bb_*.cpp helper
# files are _str-only (no box entry) and are EXEMPT (count == 0 is correct for them).
#
# PROLOG box files: bb_arith, bb_atom, bb_resolve (the router), bb_catch, bb_choice,
#   bb_conj, bb_cut, bb_disj, bb_fail, bb_goal, bb_ite, bb_logicvar, bb_unify.
# PROLOG helper files (exempt): bb_aggregate_nb, bb_atom_string, bb_findall,
#   bb_io, bb_is_cmp, bb_list, bb_retract_throw,
#   bb_succ_plus, bb_term_inspect, bb_term_io, bb_type_test.
#
# ICON box files: bb_every, bb_to, bb_alt, bb_iterate (IR_LIST_BANG),
#   bb_scan_pos, bb_scan_any, bb_scan_match, bb_scan_many, bb_scan_tab, bb_scan_move,
#   bb_scan_upto, bb_scan_find, bb_scan_bal, bb_lit_scalar, bb_var, bb_var_global,
#   bb_var_frame, bb_var_frame_ref, bb_assign_frame, bb_assign_frame_ref, bb_assign_local,
#   bb_unop, bb_succeed, bb_keyword, bb_return, bb_call,
#   bb_binop_arith, bb_binop_relop, bb_binop_gvar_arith, bb_binop_gvar_relop,
#   bb_binop_gvar_arith_slot, bb_binop_concat_slot.
# ICON helper files (exempt): bb_call_fn, bb_call_proc_staged, bb_call_userproc, bb_call_write_slot.
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
ICN_BOX_FILES="
src/emitter/BB_templates/bb_every.cpp
src/emitter/BB_templates/bb_to.cpp
src/emitter/BB_templates/bb_alt.cpp
src/emitter/BB_templates/bb_iterate.cpp
src/emitter/BB_templates/bb_scan_pos.cpp
src/emitter/BB_templates/bb_scan_any.cpp
src/emitter/BB_templates/bb_scan_match.cpp
src/emitter/BB_templates/bb_scan_many.cpp
src/emitter/BB_templates/bb_scan_tab.cpp
src/emitter/BB_templates/bb_scan_move.cpp
src/emitter/BB_templates/bb_scan_upto.cpp
src/emitter/BB_templates/bb_scan_find.cpp
src/emitter/BB_templates/bb_scan_bal.cpp
src/emitter/BB_templates/bb_lit_scalar.cpp
src/emitter/BB_templates/bb_var.cpp
src/emitter/BB_templates/bb_var_global.cpp
src/emitter/BB_templates/bb_var_frame.cpp
src/emitter/BB_templates/bb_var_frame_ref.cpp
src/emitter/BB_templates/bb_assign_frame.cpp
src/emitter/BB_templates/bb_assign_frame_ref.cpp
src/emitter/BB_templates/bb_assign_local.cpp
src/emitter/BB_templates/bb_unop.cpp
src/emitter/BB_templates/bb_succeed.cpp
src/emitter/BB_templates/bb_keyword.cpp
src/emitter/BB_templates/bb_return.cpp
src/emitter/BB_templates/bb_call.cpp
src/emitter/BB_templates/bb_binop_arith.cpp
src/emitter/BB_templates/bb_binop_relop.cpp
src/emitter/BB_templates/bb_binop_gvar_arith.cpp
src/emitter/BB_templates/bb_binop_gvar_relop.cpp
src/emitter/BB_templates/bb_binop_gvar_arith_slot.cpp
src/emitter/BB_templates/bb_binop_concat_slot.cpp
"
ICN_HELPER_FILES="
src/emitter/BB_templates/bb_call_fn.cpp
src/emitter/BB_templates/bb_call_proc_staged.cpp
src/emitter/BB_templates/bb_call_userproc.cpp
src/emitter/BB_templates/bb_call_write_slot.cpp
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
for f in $ICN_BOX_FILES; do
    [ -f "$f" ] || { echo "FAIL: expected Icon box file missing: $f"; fail=1; continue; }
    n=$(strip_comments "$f" | grep -cE 'extern "C" void bb_[a-z_]+[[:space:]]*\(')
    if [ "$n" -ne 1 ]; then
        echo "FAIL: $f has $n extern \"C\" void bb_* entries (expected exactly 1)"
        fail=1
    fi
done
for f in $ICN_HELPER_FILES; do
    [ -f "$f" ] || { echo "FAIL: expected Icon helper file missing: $f"; fail=1; continue; }
    n=$(strip_comments "$f" | grep -cE 'extern "C" void bb_[a-z_]+[[:space:]]*\(')
    if [ "$n" -ne 0 ]; then
        echo "FAIL: Icon helper file $f has $n extern \"C\" void bb_* entries (expected 0; _str helper, not a box)"
        fail=1
    fi
done
if [ "$fail" -eq 0 ]; then
    echo "PASS: every Prolog+Icon box file has exactly 1 extern \"C\" void bb_* entry; all helpers have 0."
fi
exit $fail
