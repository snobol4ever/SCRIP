#ifndef LOWER_LOWER_PROLOG_LPL_01_PL_H
#define LOWER_LOWER_PROLOG_LPL_01_PL_H
#include "lower/lower_prolog/lpl_head.h"
unsigned resolve_pred_hash(const char *s);
Resolve_PredEntry *resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice);
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key);
__attribute__((visibility("hidden"))) pl_bb_ent_t * pl_bb_lookup(const char * name, int arity);
__attribute__((visibility("hidden"))) pl_bb_ent_t * pl_bb_register(const char * name, int arity, int bb_idx);
__attribute__((visibility("hidden"))) int pl_pi_is_static_builtin(const char * pn, int ar);
void pl_dyn_mark(const char *name, int arity);
__attribute__((visibility("hidden"))) void pl_decl_dyn_mark(const char * name, int arity);
__attribute__((visibility("hidden"))) int pl_decl_dyn_is(const char * name, int arity);
__attribute__((visibility("hidden"))) void pl_decl_other_record(const tree_t * spec, int mf);
__attribute__((visibility("hidden"))) const char * pl_vname(const lcx_t * cx, int slot);
__attribute__((visibility("hidden"))) IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
__attribute__((visibility("hidden"))) IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) int pl_seal_branch_tail(int tail, int pop, IR_t * nd);
__attribute__((visibility("hidden"))) int pl_file_defines(const char * nm, int ar);
__attribute__((visibility("hidden"))) int pl_scc_armed(void);
__attribute__((visibility("hidden"))) IR_t * pl_leaf(lcx_t * cx, const char * sym, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
extern __attribute__((visibility("hidden"))) const pl_det_leaf_t pl_det_leaves[334];
__attribute__((visibility("hidden"))) int pl_det_leaf_name_wired(const char * nm);
__attribute__((visibility("hidden"))) const char * pl_det_leaf_sym(const char * nm, int ar);
__attribute__((visibility("hidden"))) const char * pl_anum_guard_sym(const char * nm, int ar);
__attribute__((visibility("hidden"))) IR_t * pl_leaf_lv_guarded(lcx_t * cx, const char * sym, const char * gsym, const char * gname, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) const char * pl_pred_file(const char * nm, int ar);
__attribute__((visibility("hidden"))) int pl_rt_registered_static(const char * nm, int ar);
__attribute__((visibility("hidden"))) int pl_user_file_defines(const char * nm, int ar);
__attribute__((visibility("hidden"))) int pl_db_owned(const char * nm, int ar);
__attribute__((visibility("hidden"))) int pl_db_writable(const char * nm, int ar);
__attribute__((visibility("hidden"))) int pl_tree_has_var(const tree_t * t);
__attribute__((visibility("hidden"))) int pl_phrase_rt_checked(void);
__attribute__((visibility("hidden"))) IR_t * pl_user_call(lcx_t * cx, const char * nm, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
__attribute__((visibility("hidden"))) int pl_dl_prelude_key(const lcx_t * cx);
__attribute__((visibility("hidden"))) int pl_dl_ctl_leaf_scope(const lcx_t * cx);
__attribute__((visibility("hidden"))) int pl_new_proc(const char * name, int nparams, int bb_idx);
__attribute__((visibility("hidden"))) IR_graph_t * pl_body_graph(const tree_t * const * gl, int ng);
__attribute__((visibility("hidden"))) int lower_pl_pred_graph(const char * key, const tree_t * ch);
__attribute__((visibility("hidden"))) void pl_register_program(stage2_t * s2, const tree_t * prog);
__attribute__((visibility("hidden"))) int pl_attv_user(const char * nm, int ar);
#endif
