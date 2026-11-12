#ifndef PARSERS_RAKU_RK_SYNTAX_RKS_07_SH_H
#define PARSERS_RAKU_RK_SYNTAX_RKS_07_SH_H
#include "parsers/raku/rk_syntax/rks_head.h"
__attribute__((visibility("hidden"))) void register_user_op(RkP *p, int from, int to);
__attribute__((visibility("hidden"))) int r_semilist(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_dottyop(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_infix_plain(RkP *p, int pos, OpInfo *o);
__attribute__((visibility("hidden"))) int r_infixish(RkP *p, int pos, OpInfo *o, int in_meta);
__attribute__((visibility("hidden"))) int r_prefixish(RkP *p, int pos, int *prec);
__attribute__((visibility("hidden"))) int r_dotty(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_postfixish(RkP *p, int pos);
__attribute__((visibility("hidden"))) int bracket_ending_at(RkP *p, int end);
__attribute__((visibility("hidden"))) int r_reduce(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_user_term(RkP *p, int pos);
__attribute__((visibility("hidden"))) void enum_names_from(RkP *p, int from, int to, const char *en, int enl);
#endif
