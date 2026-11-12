#ifndef LOWER_LOWER_PROLOG_LPL_05_PL_H
#define LOWER_LOWER_PROLOG_LPL_05_PL_H
#include "lower/lower_prolog/lpl_head.h"
#include "runtime/core/g_runtime.h"
__attribute__((visibility("hidden"))) int pl_var_nodes(const tree_t * t);
__attribute__((visibility("hidden"))) int max_var_slot(const tree_t * t, int mx);
__attribute__((visibility("hidden"))) void pl_collect_vars(const tree_t * t, int * slots, int * n, int cap);
__attribute__((visibility("hidden"))) int pl_free_vars(const tree_t * tmpl, const tree_t * goal, int * out, int cap);
__attribute__((visibility("hidden"))) int pl_var_count(const tree_t * t, int slot);
__attribute__((visibility("hidden"))) int pl_name_in(const char * nm, const char * const * lst);
__attribute__((visibility("hidden"))) int pl_rung_of(const char * nm);
__attribute__((visibility("hidden"))) void pl_kill_co_goal(const tree_t * const * ts, int nt, int gi, int * co, int nv);
__attribute__((visibility("hidden"))) void pl_kill_scan(const tree_t * t, int gi, int * first, int * last, int nv);
__attribute__((visibility("hidden"))) int pl_tree_calls(const tree_t * t, const char * f, int n);
__attribute__((visibility("hidden"))) tree_t * pl_cutto_subj(const tree_t * subj, int ar, int head_ok);
__attribute__((visibility("hidden"))) tree_t * pl_table_stmt(const tree_t * s, tree_t * subj);
__attribute__((visibility("hidden"))) const tree_t * pl_cutto_rewrite(const tree_t * prog);
__attribute__((visibility("hidden"))) const tree_t * pl_scc_rewrite(const tree_t * prog);
__attribute__((visibility("hidden"))) const tree_t * pl_mq_rewrite(const tree_t * prog);
__attribute__((visibility("hidden"))) const tree_t * pl_meta_rt_rewrite(const tree_t * prog);
__attribute__((visibility("hidden"))) int pl_prog_defines(const tree_t * prog, const char * key);
__attribute__((visibility("hidden"))) const tree_t * pl_src_file_rewrite(const tree_t * prog);
__attribute__((visibility("hidden"))) int pl_load_hook_line(const tree_t * prog, const char * file);
__attribute__((visibility("hidden"))) int pl_load_capacity(const tree_t * prog);
__attribute__((visibility("hidden"))) const tree_t * pl_load_defer(const tree_t * prog, pl_load_item_t * it, int * nit);
__attribute__((visibility("hidden"))) void pl_load_procs(const tree_t * prog, const pl_load_item_t * it, int nit, cv_t * procs);
#endif
