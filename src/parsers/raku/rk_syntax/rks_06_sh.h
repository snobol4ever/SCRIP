#ifndef PARSERS_RAKU_RK_SYNTAX_RKS_06_SH_H
#define PARSERS_RAKU_RK_SYNTAX_RKS_06_SH_H
#include "parsers/raku/rk_syntax/rks_head.h"
__attribute__((visibility("hidden"))) char *ct_strndup0(const char *s, int n);
__attribute__((visibility("hidden"))) void add_name_n(RkP *p, const char *s, int n);
__attribute__((visibility("hidden"))) void add_value_name(RkP *p, const char *s, int n);
__attribute__((visibility("hidden"))) void add_our_name(RkP *p, const char *s, int n);
__attribute__((visibility("hidden"))) void add_name_all(RkP *p, const char *s, int n);
__attribute__((visibility("hidden"))) int user_name_index(RkP *p, const char *s, int n);
__attribute__((visibility("hidden"))) const char *state_name_of(void *ctx, const char *s, int n);
__attribute__((visibility("hidden"))) int is_type_n(RkP *p, const char *s, int n);
__attribute__((visibility("hidden"))) void add_routine_name(RkP *p, const char *s, int n);
__attribute__((visibility("hidden"))) int name_part_len(RkP *p, int from, int to);
__attribute__((visibility("hidden"))) int r_EXPR(RkP *p, int pos, int preclim);
__attribute__((visibility("hidden"))) int r_longname(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_deflongname(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_nulltermish(RkP *p, int pos);
__attribute__((visibility("hidden"))) void strs_add(RkStrs *a, const char *s, int n);
__attribute__((visibility("hidden"))) void check_lexical_variable(RkP *p, int pos, int e, int twig);
__attribute__((visibility("hidden"))) int r_routine_def(RkP *p, int pos, int is_method);
__attribute__((visibility("hidden"))) int r_typename(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_trait(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_package_declarator(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_term_name(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_term_identifier(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_traits(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_param_var(RkP *p, int pos);
__attribute__((visibility("hidden"))) int r_variable_declarator(RkP *p, int pos);
__attribute__((visibility("hidden"))) int import_module_src(RkP *p, const char *src, int got, const char *path);
#endif
