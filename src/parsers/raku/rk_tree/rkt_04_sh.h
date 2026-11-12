#ifndef PARSERS_RAKU_RK_TREE_RKT_04_SH_H
#define PARSERS_RAKU_RK_TREE_RKT_04_SH_H
#include "parsers/raku/rk_tree/rkt_head.h"
__attribute__((visibility("hidden"))) int line_at(RkB *b, int pos);
__attribute__((visibility("hidden"))) const char *strip_sigil(const char *s);
__attribute__((visibility("hidden"))) const char *rk_tw_bare(const char *s);
__attribute__((visibility("hidden"))) int ns_has(const NS *t, const char *s);
__attribute__((visibility("hidden"))) void mark_arr(RkB *b, const char *bare);
__attribute__((visibility("hidden"))) int is_arr(RkB *b, const char *bare);
__attribute__((visibility("hidden"))) void mark_arrlit(RkB *b, const char *bare, const tree_t *rhs);
__attribute__((visibility("hidden"))) const char *var_ident(const char *s);
void rkb_codevar(RkB *b, const char *name);
__attribute__((visibility("hidden"))) tree_t *rk_logical_and(tree_t *a, tree_t *c);
__attribute__((visibility("hidden"))) tree_t *rk_bind(tree_t *target, tree_t *rhs);
__attribute__((visibility("hidden"))) tree_t *rk_cstyle_loop(tree_t *init, tree_t *cond, tree_t *incr, tree_t *body);
__attribute__((visibility("hidden"))) tree_t *rk_tw_field(const char *name);
__attribute__((visibility("hidden"))) tree_t *rk_tw_post_incdec(RkB *b, const char *var, int add);
__attribute__((visibility("hidden"))) tree_t *rk_defaults_prologue(TL *params, tree_t *body);
tree_t *rkb_expr(RkB *b, RkList *L);
tree_t *rkb_paren(RkB *b, RkList *L, int nstmts);
__attribute__((visibility("hidden"))) tree_t *closure_of(RkTerm *t);
__attribute__((visibility("hidden"))) int rkt_rk_reads_var(const tree_t *t, const char *name);
__attribute__((visibility("hidden"))) void rk_pcv_release(RkB *b, tree_t *sig);
__attribute__((visibility("hidden"))) tree_t *hash_pairs(RkB *b, RkEl **rs, int nrs);
__attribute__((visibility("hidden"))) tree_t *rk_attr_prologue(TL *params, tree_t *body, int kind);
#endif
