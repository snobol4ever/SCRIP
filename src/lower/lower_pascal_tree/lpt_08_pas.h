#ifndef LOWER_LOWER_PASCAL_TREE_LPT_08_PAS_H
#define LOWER_LOWER_PASCAL_TREE_LPT_08_PAS_H
#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
__attribute__((visibility("hidden"))) int pas_is_currencyexpr(tree_t *e);
__attribute__((visibility("hidden"))) int pas_pchar_castname(tree_t *e);
__attribute__((visibility("hidden"))) int pas_proc_param_is_var(const char *name, int idx);
__attribute__((visibility("hidden"))) int pas_xr_low(tree_t *e);
__attribute__((visibility("hidden"))) tree_t *pas_xr_wrap(const tree_t *src, tree_t *e);
__attribute__((visibility("hidden"))) int pas_var_is_boolfam(const char *name);
__attribute__((visibility("hidden"))) int pas_is_proc(const char *name);
__attribute__((visibility("hidden"))) int pas_is_func(const char *name);
__attribute__((visibility("hidden"))) tree_t *pas_addr_of_proc(tree_t *e);
__attribute__((visibility("hidden"))) tree_t *pas_range_wrap(tree_t *val, long long lo, long long hi, const char *clause);
__attribute__((visibility("hidden"))) int pas_proc_param_is_real(const char *name, int idx);
__attribute__((visibility("hidden"))) int pas_proc_param_is_string(const char *name, int idx);
__attribute__((visibility("hidden"))) int pas_setcons_elems(tree_t *v, tree_t *out);
__attribute__((visibility("hidden"))) int pas_has_user_call(const tree_t *x);
__attribute__((visibility("hidden"))) void pas_proc_add(const char *name);
__attribute__((visibility("hidden"))) void pas_proc_vparams(const char *name, PNodeList *params);
__attribute__((visibility("hidden"))) void pas_required_needs_params(const char *name);
__attribute__((visibility("hidden"))) void pas_call_arity(const char *name, PNodeList *args);
__attribute__((visibility("hidden"))) void pas_ordinal_fn_arg(const char *name, PNodeList *args);
__attribute__((visibility("hidden"))) void pas_string_assign_check(const char *vn, tree_t *rhs);
__attribute__((visibility("hidden"))) tree_t *pas_pchar_assign(tree_t *sel, tree_t *rhs);
__attribute__((visibility("hidden"))) const char *pas_ptrcast_target(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_argument(const tree_t *n);
__attribute__((visibility("hidden"))) int pas_xr_elab(const tree_t *n, pval *out);
__attribute__((visibility("hidden"))) int pas_xr_call(const tree_t *n, pval *out);
__attribute__((visibility("hidden"))) int pas_routine_named(const tree_t *t, const char *nm);
__attribute__((visibility("hidden"))) int pas_takes_address(const tree_t *root, const tree_t *t);
__attribute__((visibility("hidden"))) int pas_user_ioflow(const tree_t *t);
__attribute__((visibility("hidden"))) int pas_has_unit(const tree_t *prog, const char *name);
#endif
