#ifndef PARSERS_RAKU_RK_SYNTAX_RKS_08_SH_H
#define PARSERS_RAKU_RK_SYNTAX_RKS_08_SH_H
#include "parsers/raku/rk_syntax/rks_head.h"
__attribute__((visibility("hidden"))) int r_baseint(RkP *p, int pos, int base);
__attribute__((visibility("hidden"))) int qq_backslash(RkP *p, int pos, RkLang *L);
__attribute__((visibility("hidden"))) int r_regex_nibbler(RkP *p, int pos, RkLang *L);
__attribute__((visibility("hidden"))) int rx_metachar(RkP *p, int pos, RkLang *L);
#endif
