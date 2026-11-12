#ifndef PARSERS_RAKU_RK_SYNTAX_RKS_04_SH_H
#define PARSERS_RAKU_RK_SYNTAX_RKS_04_SH_H
#include "parsers/raku/rk_syntax/rks_head.h"
__attribute__((visibility("hidden"))) int line_of(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_nibble_until(RkP *p, int pos, RkLang *L, int *endpos);
__attribute__((visibility("hidden"))) int r_quote(RkP *p, int pos);
__attribute__((visibility("hidden"))) void lang_q(RkLang *L);
__attribute__((visibility("hidden"))) int quote_body(RkP *p, int pos, RkLang *L, const char *what);
__attribute__((visibility("hidden"))) int r_postcircumfix(RkP *p, int pos);
__attribute__((visibility("hidden"))) int expect_close(RkP *p, int pos, int c, const char *what, int open_pos);
__attribute__((visibility("hidden"))) int words_quote(RkP *p, int pos, int open, int nrep, int qq);
__attribute__((visibility("hidden"))) int strs_has(const RkStrs *a, const char *s, int n);
__attribute__((visibility("hidden"))) void package_close(RkP *p);
__attribute__((visibility("hidden"))) int p5_regex_body(RkP *p, int pos, RkLang *L);
#endif
