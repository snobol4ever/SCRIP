#ifndef PARSERS_RAKU_RK_TREE_RKT_01_SH_H
#define PARSERS_RAKU_RK_TREE_RKT_01_SH_H
#include "parsers/raku/rk_tree/rkt_head.h"
__attribute__((visibility("hidden"))) char *fmt(const char *f, ...);
__attribute__((visibility("hidden"))) char *spn(RkB *b, int from, int to);
__attribute__((visibility("hidden"))) char *trimdup(const char *s, int n);
__attribute__((visibility("hidden"))) tree_t *leaf_sval(tree_e k, const char *s);
__attribute__((visibility("hidden"))) tree_t *var_node(RkB *b, const char *name);
__attribute__((visibility("hidden"))) tree_t *make_call(const char *name);
__attribute__((visibility("hidden"))) tree_t *call2(const char *fn, tree_t *l, tree_t *r);
__attribute__((visibility("hidden"))) tree_t *rk_ilit(long long v);
__attribute__((visibility("hidden"))) tree_t *mkbool_lit(int v);
__attribute__((visibility("hidden"))) tree_t *rk_phaser_mark(const char *name, tree_t *body);
__attribute__((visibility("hidden"))) tree_t *rk_quiet_store(tree_t *target, tree_t *rhs);
__attribute__((visibility("hidden"))) tree_t *rk_container_bind(tree_t *target, tree_t *rhs);
__attribute__((visibility("hidden"))) tree_t *rk_incdec(RkB *b, const char *var, int add);
__attribute__((visibility("hidden"))) tree_t *rk_post_incdec(RkB *b, const char *var, int add);
__attribute__((visibility("hidden"))) tree_t *rk_destructure(RkB *b, TL *targets, tree_t *rhs_arr);
__attribute__((visibility("hidden"))) tree_t *rk_for_multi(RkB *b, TL *vars, TL *acc, tree_t *list, tree_t *body, int destr);
__attribute__((visibility("hidden"))) void rk_range_fl_arg(tree_t *call, const tree_t *to);
__attribute__((visibility("hidden"))) tree_t *rk_arr_index(RkB *b, const char *arr, tree_t *idx);
__attribute__((visibility("hidden"))) tree_t *rk_dec(tree_t *hi);
__attribute__((visibility("hidden"))) tree_t *rk_arr_end_index(RkB *b, const char *arr, tree_t *off, tree_e op);
__attribute__((visibility("hidden"))) tree_t *rk_arr_all(RkB *b, const char *arr);
__attribute__((visibility("hidden"))) tree_t *rx_var(RkB *b, int sig, const char *name);
__attribute__((visibility("hidden"))) tree_t *rx_wrap(const char *fn, tree_t *x);
__attribute__((visibility("hidden"))) tree_t *b_dq(RkB *b, RkClosure *cl, int ncl, int from, int to);
__attribute__((visibility("hidden"))) tree_t *closure_expr(RkB *b, RkClosure *c);
__attribute__((visibility("hidden"))) tree_t *b_words(RkB *b, const char *s, int n, int bs);
__attribute__((visibility("hidden"))) tree_t *build_decl(RkB *b, RkDecl *d, RkList *outer, int ofirst);
__attribute__((visibility("hidden"))) int arglist(RkB *b, RkList *L, int ctx, TL *pos, TL *named);
__attribute__((visibility("hidden"))) tree_t *rk_adv_pair(const char *name);
__attribute__((visibility("hidden"))) tree_t *method_call(RkB *b, tree_t *inv, RkPf *pf);
#endif
