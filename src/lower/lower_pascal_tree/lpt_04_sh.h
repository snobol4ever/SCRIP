#ifndef LOWER_LOWER_PASCAL_TREE_LPT_04_SH_H
#define LOWER_LOWER_PASCAL_TREE_LPT_04_SH_H
#include "lower/lower_pascal_tree/lpt_head.h"
__attribute__((visibility("hidden"))) void pas_scope_define(const char *name);
__attribute__((visibility("hidden"))) int pas_scope_uid(const char *name);
__attribute__((visibility("hidden"))) void pas_scope_define_list(PNodeList *ids);
__attribute__((visibility("hidden"))) long long pas_array_low(const char *name);
__attribute__((visibility("hidden"))) long long pas_array_nonparam_low(const char *name);
__attribute__((visibility("hidden"))) int pas_tsz_get(const char *name, long long *out);
__attribute__((visibility("hidden"))) int pas_array_is_zero_size(const char *name);
__attribute__((visibility("hidden"))) int pas_with_holds_pointee_of(tree_t *ptr);
__attribute__((visibility("hidden"))) void pas_setconst_add(const char *name, tree_t *val);
__attribute__((visibility("hidden"))) int pas_rows_get(const char *name, int istype, long long *lo, long long *hi);
__attribute__((visibility("hidden"))) tree_t *pas_setconst_get(const char *name);
__attribute__((visibility("hidden"))) const char *pas_ptrtype_target(const char *p);
__attribute__((visibility("hidden"))) const char *pas_arrptr_target(const char *v);
__attribute__((visibility("hidden"))) long long pas_rk_get(long long ov, int w);
__attribute__((visibility("hidden"))) int pas_rectype_to_pend(const char *tn);
__attribute__((visibility("hidden"))) int pas_is_declared_local_here(const char *name);
__attribute__((visibility("hidden"))) tree_t *pas_region_wrap(tree_t *e, long long ov, long long lo);
__attribute__((visibility("hidden"))) tree_t *pas_arrrec_region(tree_t *e, const char *an, int fi);
__attribute__((visibility("hidden"))) void elab_bad(const char *nt, const tree_t *n);
__attribute__((visibility("hidden"))) pval E_case_list(const tree_t *n, int from, int to);
__attribute__((visibility("hidden"))) pval E_pv_mark(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_type_decl_list(const tree_t *n, int from, int to);
__attribute__((visibility("hidden"))) pval E_var_decl(const tree_t *n);
#endif
