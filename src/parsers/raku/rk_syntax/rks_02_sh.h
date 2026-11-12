#ifndef PARSERS_RAKU_RK_SYNTAX_RKS_02_SH_H
#define PARSERS_RAKU_RK_SYNTAX_RKS_02_SH_H
#include "parsers/raku/rk_syntax/rks_head.h"
__attribute__((visibility("hidden"))) int is_alpha_cp(int c);
__attribute__((visibility("hidden"))) int kw(RkP *p, int pos, const char *w);
__attribute__((visibility("hidden"))) int at_bol(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_colonpair(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_circumfix(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_variable(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_quote_escape(RkP *p, int pos, RkLang *L);
__attribute__((visibility("hidden"))) int r_term(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_pblock(RkP *p, int pos, int implicit);
__attribute__((visibility("hidden"))) int r_signature(RkP *p, int pos, int allow_invocant);
__attribute__((visibility("hidden"))) int user_routine_n(RkP *p, const char *s, int n);
__attribute__((visibility("hidden"))) int qok(RkP *p, int pos, const char *word);
__attribute__((visibility("hidden"))) int r_type_declarator(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_post_constraint(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_sublongname(RkP *p, int pos);
#endif
