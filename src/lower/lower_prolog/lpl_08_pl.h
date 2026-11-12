#ifndef LOWER_LOWER_PROLOG_LPL_08_PL_H
#define LOWER_LOWER_PROLOG_LPL_08_PL_H
#include "lower/lower_prolog/lpl_head.h"
__attribute__((visibility("hidden"))) void collect_conj(const tree_t * t, lc_vec * out);
__attribute__((visibility("hidden"))) IR_t * pl_lower_conj(lcx_t * cx, const tree_t * const * gl, int ng, IR_t * γtail, IR_t * ωbase, IR_t ** entry_out, IR_t ** redo_out, IR_t ** tail_out);
__attribute__((visibility("hidden"))) const tree_t * pl_caret_body(const tree_t * t);
__attribute__((visibility("hidden"))) int pl_tree_is_control(const tree_t * t);
__attribute__((visibility("hidden"))) int pl_goal_holds_cut(const tree_t * g);
__attribute__((visibility("hidden"))) int pl_goal_arg_wants_guard(const tree_t * t);
__attribute__((visibility("hidden"))) int pl_tree_control_var_leaf(const tree_t * t);
__attribute__((visibility("hidden"))) int pl_is_ite(const tree_t * t);
__attribute__((visibility("hidden"))) int pl_is_scite(const tree_t * t);
__attribute__((visibility("hidden"))) void pl_pop_vet(lcx_t * cx, int from);
__attribute__((visibility("hidden"))) IR_t * pl_lower_disj(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_probe_before(lcx_t * cx, IR_t * target, IR_t * mark, int needs_mark);
__attribute__((visibility("hidden"))) IR_t * pl_lower_ite(lcx_t * cx, const tree_t * C, const tree_t * T, const tree_t * E, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_lower_softcut(lcx_t * cx, const tree_t * C, const tree_t * T, const tree_t * E, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_cleanup_node(lcx_t * cx, int kind, IR_t * open, IR_t * γ, IR_t * ω);
__attribute__((visibility("hidden"))) int pl_dollar_user(int ar);
__attribute__((visibility("hidden"))) tree_t * pl_det_unqual_goal(const tree_t * G);
__attribute__((visibility("hidden"))) IR_t * pl_lower_catch(lcx_t * cx, const tree_t * G, const tree_t * C, const tree_t * R, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
#endif
