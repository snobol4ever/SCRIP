#ifndef LOWER_LOWER_PASCAL_TREE_LPT_07_SH_H
#define LOWER_LOWER_PASCAL_TREE_LPT_07_SH_H
#include "lower/lower_pascal_tree/lpt_head.h"
__attribute__((visibility("hidden"))) tree_t *pas_pf_callthrough(const char *name, PNodeList *args);
__attribute__((visibility("hidden"))) tree_t *leaf_s(tree_e k, const char *s);
__attribute__((visibility("hidden"))) tree_t *flit(double v);
__attribute__((visibility("hidden"))) tree_t *un(tree_e k, tree_t *a);
__attribute__((visibility("hidden"))) int pas_expr_is_real(tree_t *e);
__attribute__((visibility("hidden"))) const char *map_io(const char *fn);
__attribute__((visibility("hidden"))) int is_pas_io(const char *fn);
__attribute__((visibility("hidden"))) int pas_tfcomp_nonchar(const char *n);
__attribute__((visibility("hidden"))) tree_t *mk_chr_wrap(tree_t *e);
__attribute__((visibility("hidden"))) tree_t *pas_trace_prepend_tap_off(tree_t *body);
__attribute__((visibility("hidden"))) tree_t *mk_deref(tree_t *ptr);
__attribute__((visibility("hidden"))) tree_t *mk_fnc0(const char *fn);
__attribute__((visibility("hidden"))) tree_t *mk_fnc1(const char *fn, tree_t *a);
__attribute__((visibility("hidden"))) tree_t *mk_chrlit(tree_t *a);
__attribute__((visibility("hidden"))) tree_t *mk_fnc4(const char *fn, tree_t *a, tree_t *b, tree_t *c, tree_t *d);
__attribute__((visibility("hidden"))) tree_t *pas_ord_check(tree_t *v, long long lo, long long hi, const char *what);
__attribute__((visibility("hidden"))) int pas_is_unsigned_inttypename(const char *n);
__attribute__((visibility("hidden"))) tree_t *pas_split_writes(tree_t *e);
__attribute__((visibility("hidden"))) tree_t *pas_crt_route(tree_t *t);
__attribute__((visibility("hidden"))) const char *pas_pf_field_sig(const tree_t *f);
__attribute__((visibility("hidden"))) tree_t *pas_pf_field_assign(const tree_t *n);
__attribute__((visibility("hidden"))) tree_t *pas_tm_call(const char *name);
__attribute__((visibility("hidden"))) pval E_for_statement(const tree_t *n);
__attribute__((visibility("hidden"))) tree_t *pas_format_call(const tree_t *n);
__attribute__((visibility("hidden"))) pval E_expression_raw(const tree_t *n);
__attribute__((visibility("hidden"))) tree_t *pas_flatten_units(tree_t *prog);
__attribute__((visibility("hidden"))) void pas_var_inits_in(tree_t *t, struct pas_blk *chain);
#endif
