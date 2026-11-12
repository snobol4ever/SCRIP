#ifndef LOWER_LOWER_PROLOG_LPL_03_PL_H
#define LOWER_LOWER_PROLOG_LPL_03_PL_H
#include "lower/lower_prolog/lpl_head.h"
__attribute__((visibility("hidden"))) IR_t * lpl_build(lcx_t * cx, IR_e op, IR_t * γ, IR_t * ω);
__attribute__((visibility("hidden"))) IR_t * term_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
__attribute__((visibility("hidden"))) int pl_tree_is_big(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * mkc_node(lcx_t * cx, const char * fname, int nkids, IR_t ** kids, IR_t ** kid_entries, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_const_lit(lcx_t * cx, const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * pl_unify_const_node(lcx_t * cx, const char * vname, IR_t * klit, IR_t * γ, IR_t * ω);
__attribute__((visibility("hidden"))) int pl_head_boxable(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * pl_head_term(lcx_t * cx, const tree_t * cl, int ar, IR_t * src, long idx, const tree_t * t, IR_t * next, IR_t * step);
__attribute__((visibility("hidden"))) IR_t * unify_pair(lcx_t * cx, const tree_t * lt, const tree_t * rt, IR_t * γ, IR_t * ω, IR_t ** entry_out);
__attribute__((visibility("hidden"))) const char * pl_cmp_op_suffix(const char * s);
__attribute__((visibility("hidden"))) IR_t * lower_arith_val(lcx_t * cx, const tree_t * t, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_cut_redo(lcx_t * cx);
__attribute__((visibility("hidden"))) const tree_t * pl_tree_number(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * pl_leaf_guarded1(lcx_t * cx, const char * sym, const char * guard_sym, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_leaf_guarded1_dir(lcx_t * cx, const char * sym, const char * guard_sym, const char * dir, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) void pl_dyn_rt_declare(const char * name, int arity);
__attribute__((visibility("hidden"))) IR_t * pl_db_leaf1(lcx_t * cx, const char * sym, int k, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
int pl_pi_is_iso(const char * pn, int ar);
__attribute__((visibility("hidden"))) const tree_t * pl_body_ill_typed(const tree_t * b);
__attribute__((visibility("hidden"))) const tree_t * pl_clause_ill_typed(const tree_t * cl);
__attribute__((visibility("hidden"))) const char * pl_spec_key(const tree_t * s, int * ar_out);
__attribute__((visibility("hidden"))) IR_t * pl_db_leaf2_tree(lcx_t * cx, const char * sym, const tree_t * a0, const tree_t * a1, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_db_leaf2(lcx_t * cx, const char * sym, int k, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_db_enum(lcx_t * cx, int k, const tree_t * target, int erase, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_db_data(lcx_t * cx, int k, const tree_t * target, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_db_enum_ref(lcx_t * cx, const tree_t * target, const tree_t * refterm, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) const char * pl_nb_key(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * pl_nb_leaf_lv(lcx_t * cx, const char * sym, int k, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_nb_leaf_lv_tree(lcx_t * cx, const char * sym, const tree_t * keyt, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * pl_meta_call_dyn(lcx_t * cx, const tree_t * g, const tree_t * const * extra, int nextra, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) const tree_t * pl_nil_term(void);
__attribute__((visibility("hidden"))) int pl_mc_graph_fell_back(const IR_graph_t * g, const char * nm, int ar);
#endif
