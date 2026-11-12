#ifndef PARSERS_RAKU_RK_SYNTAX_RKS_09_SH_H
#define PARSERS_RAKU_RK_SYNTAX_RKS_09_SH_H
#include "parsers/raku/rk_syntax/rks_head.h"
__attribute__((visibility("hidden"))) int r_statement(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_statement_in_regex(RkP *p, int pos);
__attribute__((visibility("hidden"))) int compile_time_var_known(const char *s, int n);
__attribute__((visibility("hidden"))) int r_keyword_term(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_xblock(RkP *p, int pos, int implicit);
__attribute__((visibility("hidden"))) int r_initializer(RkP *p, int pos);
__attribute__((visibility("hidden"))) void stmt_end_info(RkP *p, int e, int *semi, int *last);
#endif
