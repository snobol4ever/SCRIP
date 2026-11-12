#ifndef LOWER_LOWER_PASCAL_TREE_LPT_05_PAS_H
#define LOWER_LOWER_PASCAL_TREE_LPT_05_PAS_H
#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
__attribute__((visibility("hidden"))) int pas_trace_enabled(void);
__attribute__((visibility("hidden"))) PNodeList *pnl_new(void);
__attribute__((visibility("hidden"))) void pnl_push(PNodeList *l, tree_t *e);
__attribute__((visibility("hidden"))) PNodeList *pnl_concat(PNodeList *a, PNodeList *b);
__attribute__((visibility("hidden"))) PasDef *pas_scope_push(PasDef **a, int *n, int *cap, const char *name);
__attribute__((visibility("hidden"))) int pas_pf_nactuals(const char *sig);
__attribute__((visibility("hidden"))) tree_t *prog_of(PNodeList *l);
__attribute__((visibility("hidden"))) tree_t *seq_of(PNodeList *l);
__attribute__((visibility("hidden"))) int pas_array_is_param(const char *name);
__attribute__((visibility("hidden"))) int pas_arrtype_isdyn(const char *name);
__attribute__((visibility("hidden"))) int pas_array_isopen(const char *name);
__attribute__((visibility("hidden"))) const char *pas_proctype_get(const char *name);
__attribute__((visibility("hidden"))) int pas_is_cafield(const tree_t *e);
__attribute__((visibility("hidden"))) long long pas_cafield_lo_get(const tree_t *e);
__attribute__((visibility("hidden"))) long long pas_cafield_hi_get(const tree_t *e);
__attribute__((visibility("hidden"))) tree_t *pas_trace_wrap_for_body(const char *var, tree_t *body);
__attribute__((visibility("hidden"))) tree_t *pas_divchk_wrap(tree_t *b);
__attribute__((visibility("hidden"))) int pas_arrtype_isbool(const char *name);
__attribute__((visibility("hidden"))) int pas_is_nafield(const tree_t *e);
__attribute__((visibility("hidden"))) void pas_flat_mark_add(tree_t *e);
__attribute__((visibility("hidden"))) tree_t *pas_rdiv(tree_t *a, tree_t *b);
__attribute__((visibility("hidden"))) char *pas_pf_cat3(const char *a, const char *b, const char *c);
__attribute__((visibility("hidden"))) const char *pas_pf_canon(const char *t);
__attribute__((visibility("hidden"))) char *pas_pf_sect(char k, int n, const char *t);
__attribute__((visibility("hidden"))) char *pas_pf_sig(char k, const char *params, const char *rtype);
__attribute__((visibility("hidden"))) const char *pas_pf_env_name(const char *formal, int k);
__attribute__((visibility("hidden"))) const char *pas_pf_heading(const char *name, PNodeList *params, char k, const char *rtype);
__attribute__((visibility("hidden"))) void pas_pf_define_routine(const char *name, const char *sig);
__attribute__((visibility("hidden"))) const char *pas_alias_resolve(const char *name);
__attribute__((visibility("hidden"))) const char *pas_shadow_name(const char *src);
__attribute__((visibility("hidden"))) void pas_pf_closure(PNodeList *out, tree_t *a, const char *fsig, const char *callee, int pos);
__attribute__((visibility("hidden"))) pval E_selector(const tree_t *n);
__attribute__((visibility("hidden"))) void pas_pf_resolve(tree_t *e);
__attribute__((visibility("hidden"))) char *tok_s(const tree_t *x);
__attribute__((visibility("hidden"))) pval E_assignment(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_call(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_call_with_args(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_parameter_decl(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_procedure_decl(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_selector(const tree_t *n);
__attribute__((visibility("hidden"))) tree_t *pas_src_call(const char *fn, tree_t *a, tree_t *b);
__attribute__((visibility("hidden"))) tree_t *pas_strhlp_prop(const tree_t *n);
__attribute__((visibility("hidden"))) pval pas_strhlp_eval(tree_t *sh);
#endif
