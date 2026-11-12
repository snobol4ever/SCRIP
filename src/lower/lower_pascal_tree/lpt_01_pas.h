#ifndef LOWER_LOWER_PASCAL_TREE_LPT_01_PAS_H
#define LOWER_LOWER_PASCAL_TREE_LPT_01_PAS_H
#include "lower/lower_pascal_tree/lpt_head.h"
__attribute__((visibility("hidden"))) void *pas_pool_grow(void *p, int *cap, size_t esz, int init);
__attribute__((visibility("hidden"))) const PasFwd *pas_scope_fwd_of(const char *name, PNodeList *params);
__attribute__((visibility("hidden"))) const char *pas_ptrvar_target(const char *v);
__attribute__((visibility("hidden"))) void pas_assigned_add(const char *n);
__attribute__((visibility("hidden"))) int pas_assigned_get(const char *n);
__attribute__((visibility("hidden"))) const char *pas_selector_base_name(tree_t *e);
__attribute__((visibility("hidden"))) void pas_tfiletype_add(const char *name);
__attribute__((visibility("hidden"))) const char *pas_sconst_get(const char *name);
__attribute__((visibility("hidden"))) int pas_arrtype_wrap(const char *name);
__attribute__((visibility("hidden"))) unsigned long long pas_openparm_mask(const char *name);
__attribute__((visibility("hidden"))) void pas_local_add(const char *name);
__attribute__((visibility("hidden"))) const char *pas_curfunc_name(void);
__attribute__((visibility("hidden"))) unsigned long long pas_caparm_mask(const char *name);
__attribute__((visibility("hidden"))) long long pas_caparm_lo(const char *name, int pos);
__attribute__((visibility("hidden"))) tree_t *mk_proc(const char *name, PNodeList *params, tree_t *body_stmt, int is_function, int decl_level, const char **lnames, int lcount);
__attribute__((visibility("hidden"))) void pas_const_add(const char *name, long long v);
__attribute__((visibility("hidden"))) int pas_rconst_get(const char *name, double *out);
__attribute__((visibility("hidden"))) void pas_enumtype_add(const char *n, long long h);
__attribute__((visibility("hidden"))) void pas_enumnames_add(const char *tn, const char *names);
__attribute__((visibility("hidden"))) void pas_typealias_add(const char *n, const char *t);
__attribute__((visibility("hidden"))) void pas_ptrtype_add(const char *p, const char *r);
__attribute__((visibility("hidden"))) void pas_booltype_add(const char *name);
__attribute__((visibility("hidden"))) void pas_strarr_add2(const char *name, long long lo);
__attribute__((visibility("hidden"))) pval E_case_statement(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_compound_statement(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_const_decl(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_constant(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_constant_list(const tree_t *n, int from, int to);
__attribute__((visibility("hidden"))) pval E_procedure_decl_in(const tree_t *n);
#endif
