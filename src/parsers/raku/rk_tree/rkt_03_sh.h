#ifndef PARSERS_RAKU_RK_TREE_RKT_03_SH_H
#define PARSERS_RAKU_RK_TREE_RKT_03_SH_H
#include "parsers/raku/rk_tree/rkt_head.h"
__attribute__((visibility("hidden"))) void tl_add(TL *l, tree_t *t);
__attribute__((visibility("hidden"))) tree_t *stmt_plain(RkB *b, RkList *L);
__attribute__((visibility("hidden"))) tree_t *rk_tree_clone(tree_t *e);
__attribute__((visibility("hidden"))) tree_t *rk_scalar_rhs(tree_t *rhs);
__attribute__((visibility("hidden"))) tree_t *el_tree(RkB *b, RkEl *e);
__attribute__((visibility("hidden"))) tree_t *el_rest(RkB *b, RkEl *e);
__attribute__((visibility("hidden"))) tree_t *el_t0(RkB *b, RkEl *e);
__attribute__((visibility("hidden"))) int plain(RkTerm *t, int cls);
int rkb_op_index(int lv, const char *op);
int rkb_op_index_rev(int lv, const char *op, int *rev);
__attribute__((visibility("hidden"))) tree_t *rkb_binop_raw(RkB *b, int lv, int k, tree_t *l, tree_t *r);
__attribute__((visibility("hidden"))) tree_t *rkb_prefix_raw(RkB *b, const char *op, tree_t *x);
tree_t *rkb_ternary(RkB *b, tree_t *l, tree_t *mid, tree_t *r);
tree_t *rkb_smartmatch(RkB *b, tree_t *l, tree_t *r);
__attribute__((visibility("hidden"))) void flatten_paren_args(RkList *L, TL *pos);
__attribute__((visibility("hidden"))) tree_t *rs_list(RkB *b, RkEl **rs, int nrs);
__attribute__((visibility("hidden"))) tree_t *assign_forms(RkB *b, RkList *L, int tail, int bk);
__attribute__((visibility("hidden"))) tree_t *stmt_tail(RkB *b, RkList *L, int bk);
#endif
