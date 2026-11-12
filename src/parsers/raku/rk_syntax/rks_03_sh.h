#ifndef PARSERS_RAKU_RK_SYNTAX_RKS_03_SH_H
#define PARSERS_RAKU_RK_SYNTAX_RKS_03_SH_H
#include "parsers/raku/rk_syntax/rks_head.h"
__attribute__((visibility("hidden"))) int is_numeric_other_cp(int c);
__attribute__((visibility("hidden"))) int is_name_n(RkP *p, const char *s, int n);
int rk_is_core_type(const char *s);
__attribute__((visibility("hidden"))) void note_core_use(RkP *p, int pos, int len);
__attribute__((visibility("hidden"))) void add_mystery(RkP *p, int pos, int len);
__attribute__((visibility("hidden"))) void scope_enter(RkP *p);
__attribute__((visibility("hidden"))) void scope_leave(RkP *p);
__attribute__((visibility("hidden"))) void add_user_op(RkP *p, char cat, const char *sym, int len, int prec);
__attribute__((visibility("hidden"))) int r_block(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_blockoid(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_statementlist(RkP *p, int pos);
__attribute__((visibility("hidden"))) int line_end(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_pod(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_statement_control(RkP *p, int pos);
__attribute__((visibility("hidden"))) int rk_check_into(RkP *p, const char *src, int len, const char *path, RkP *out);
__attribute__((visibility("hidden"))) void rk_build_module(RkP *p, const char *src, int len, const char *path);
#endif
