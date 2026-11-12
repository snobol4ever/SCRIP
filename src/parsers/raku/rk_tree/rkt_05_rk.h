#ifndef PARSERS_RAKU_RK_TREE_RKT_05_RK_H
#define PARSERS_RAKU_RK_TREE_RKT_05_RK_H
#include "parsers/raku/rk_tree/rkt_head.h"
__attribute__((visibility("hidden"))) int rk_is_slice_key(const tree_t *k);
__attribute__((visibility("hidden"))) tree_t *rk_arr_rhs(tree_t *rhs);
__attribute__((visibility("hidden"))) tree_t *nctx(RkB *b, tree_t *e);
__attribute__((visibility("hidden"))) tree_t *b_mul(RkB *b, int k, tree_t *l, tree_t *r);
__attribute__((visibility("hidden"))) tree_t *b_addsub(RkB *b, int k, tree_t *l, tree_t *r);
__attribute__((visibility("hidden"))) tree_t *b_cmp(RkB *b, int k, tree_t *l, tree_t *r);
__attribute__((visibility("hidden"))) char *rk_op_name_canon(char *nm);
int rkb_user_op_find(RkB *b, char cat, const char *sym);
__attribute__((visibility("hidden"))) const char *rk_user_name(RkB *b, int idx, const char *cat);
__attribute__((visibility("hidden"))) int rk_user_canon_known(RkB *b, const char *cn);
__attribute__((visibility("hidden"))) tree_t *rk_user_call1(RkB *b, int idx, const char *cat, tree_t *x);
int rkb_cross_inner(const char *op, int *lv, int *k, int *rev);
__attribute__((visibility("hidden"))) tree_t *rk_hyper_elem(int i);
__attribute__((visibility("hidden"))) tree_t *rk_loose_topical(RkB *b, tree_t *r, const char *tmp);
tree_t *rkb_hyper_prefix(RkB *b, const char *op, tree_t *x);
__attribute__((visibility("hidden"))) tree_t *rk_list_operand(RkB *b, RkList *L);
__attribute__((visibility("hidden"))) tree_t *rk_opref(RkB *b, const char *name);
__attribute__((visibility("hidden"))) char *rk_op_name_canon(char *nm);
__attribute__((visibility("hidden"))) tree_t *rk_hash_key(RkB *b, RkPf *pf);
__attribute__((visibility("hidden"))) int rk_slash_param(const TL *params);
__attribute__((visibility("hidden"))) void rk_slash_rename(tree_t *t);
#endif
