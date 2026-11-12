#ifndef PARSERS_RAKU_RK_TREE_RKT_02_SH_H
#define PARSERS_RAKU_RK_TREE_RKT_02_SH_H
#include "parsers/raku/rk_tree/rkt_head.h"
__attribute__((visibility("hidden"))) void sb_c(SB *s, char c);
__attribute__((visibility("hidden"))) char *sb_str(SB *s);
__attribute__((visibility("hidden"))) tree_t *rk_adhoc_new(const char *cname, TL *named, TL *pos);
__attribute__((visibility("hidden"))) tree_t *rk_rq_term(RkB *b, const RkRq *q, RkClosure *cl, int ncl);
__attribute__((visibility("hidden"))) tree_t *b_dq_fl(RkB *b, RkClosure *cl, int ncl, int from, int to, int fl);
__attribute__((visibility("hidden"))) tree_t *b_sq(RkB *b, int from, int to);
__attribute__((visibility("hidden"))) tree_t *rkb_smartmatch_term_raw(RkB *b, tree_t *l, RkTerm *x);
__attribute__((visibility("hidden"))) int b_quote_fl(RkB *b, RkTerm *it, RkClosure *cl, int ncl, int bf, int bt, int qf);
#endif
