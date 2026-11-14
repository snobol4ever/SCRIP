#ifndef LOWER_LOWER_PROLOG_LPL_06_PL_H
#define LOWER_LOWER_PROLOG_LPL_06_PL_H
#include "lower/lower_prolog/lpl_head.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) int pl_tree_is_nil(const tree_t * t);
extern __attribute__((visibility("hidden"))) const char * const pl_decl_directives[14];
__attribute__((visibility("hidden"))) int pl_table_opt_kind(const tree_t * o);
__attribute__((visibility("hidden"))) int pl_attv_on(void);
__attribute__((visibility("hidden"))) int pl_attv_body(const tree_t * cl, int ar, const tree_t ** out, int cap);
__attribute__((visibility("hidden"))) int pl_swi_library_leaf(const char * nm, int ar);
__attribute__((visibility("hidden"))) int pl_swi_absent_leaf(const char * nm, int ar);
__attribute__((visibility("hidden"))) void pl_dir_number_vars(tree_t * t, const char ** names, int * n, int scope0);
__attribute__((visibility("hidden"))) int pl_table_specs(const tree_t * a, const char ** keys, const tree_t ** specs, int n, int cap);
__attribute__((visibility("hidden"))) const tree_t * pl_table_dir(const tree_t * s);
__attribute__((visibility("hidden"))) tree_t * pl_table_goal(const char * f, int ar);
#endif
