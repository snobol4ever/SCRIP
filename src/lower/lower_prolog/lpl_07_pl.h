#ifndef LOWER_LOWER_PROLOG_LPL_07_PL_H
#define LOWER_LOWER_PROLOG_LPL_07_PL_H
#include "lower/lower_prolog/lpl_head.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) int pl_hook_dyn_is(const char * name, int arity);
__attribute__((visibility("hidden"))) void pl_hook_dyn_record(const tree_t * spec);
__attribute__((visibility("hidden"))) tree_t * pl_det_guard_callee(const char * own_key, const tree_t * const * gl, int ng, int k);
__attribute__((visibility("hidden"))) const tree_t * pl_clauseq_rewrite(const tree_t * t);
__attribute__((visibility("hidden"))) void pl_rs_dyn(const tree_t * spec);
__attribute__((visibility("hidden"))) int pl_rs_clause(tree_t * ch, int ci, const char * key, const char ** tk, int ntk, tree_t ** out, int nout);
__attribute__((visibility("hidden"))) const tree_t * pl_auto_table_rewrite(const tree_t * prog);
__attribute__((visibility("hidden"))) const tree_t * pl_det_rewrite(const tree_t * prog);
#endif
