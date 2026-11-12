#ifndef LOWER_LOWER_PASCAL_TREE_LPT_11_SH_H
#define LOWER_LOWER_PASCAL_TREE_LPT_11_SH_H
#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
__attribute__((visibility("hidden"))) void pas_scope_defined_after_use(const char *name);
__attribute__((visibility("hidden"))) void pas_scope_require(const char *name);
__attribute__((visibility("hidden"))) int pas_is_rel(tree_t *e);
__attribute__((visibility("hidden"))) int pas_const_isenum(const char *name);
__attribute__((visibility("hidden"))) int pas_const_get(const char *name, long long *out);
__attribute__((visibility("hidden"))) void pas_not_a_word_symbol(const char *n);
__attribute__((visibility("hidden"))) long long tok_i(const tree_t *n);
__attribute__((visibility("hidden"))) tree_t *pas_for_once(tree_t *e);
__attribute__((visibility("hidden"))) void tok_events(const tree_t *x);
__attribute__((visibility("hidden"))) double tok_d(const tree_t *x);
__attribute__((visibility("hidden"))) pval E_block(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_body_stmt(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_expression_list(const tree_t *n, int from, int to);
__attribute__((visibility("hidden"))) pval E_file_id_list_opt(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_id_list(const tree_t *n, int from, int to);
__attribute__((visibility("hidden"))) pval E_parameter_list_opt(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_pf_params(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_scalar_constant(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_simple_type(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_statement_list(const tree_t *n, int from, int to);
#endif
