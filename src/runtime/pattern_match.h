/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PATTERN_MATCH_H
#define PATTERN_MATCH_H
#include "ir/descr.h"
int cset_resolve(DESCR_t arg, const char **out_ptr, int *out_len);
int cset_has(const char *cv, int clen, unsigned char ch);
DESCR_t pat_lit(const char *s);
DESCR_t pat_span(const char *chars);
DESCR_t pat_break_(const char *chars);
DESCR_t pat_breakx (const char *chars);
DESCR_t pat_any_cs(const char *chars);
DESCR_t pat_notany(const char *chars);
DESCR_t pat_len(int64_t n);
DESCR_t pat_pos(int64_t n);
DESCR_t pat_rpos(int64_t n);
DESCR_t pat_tab(int64_t n);
DESCR_t pat_rtab(int64_t n);
DESCR_t pat_arb(void);
DESCR_t pat_arbno(DESCR_t inner);
DESCR_t pat_rem(void);
DESCR_t pat_fence(void);
DESCR_t pat_fence_p(DESCR_t inner);
DESCR_t pat_fail(void);
DESCR_t pat_abort(void);
DESCR_t pat_succeed(void);
DESCR_t pat_bal(void);
DESCR_t pat_epsilon(void);
DESCR_t pat_cat(DESCR_t left, DESCR_t right);
DESCR_t pat_alt(DESCR_t left, DESCR_t right);
DESCR_t pat_ref(const char *name);
DESCR_t pat_assign_imm(DESCR_t child, DESCR_t var);
DESCR_t pat_assign_cond(DESCR_t child, DESCR_t var);
DESCR_t pat_assign_callcap(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs);
DESCR_t pat_assign_callcap_named(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs, char **arg_names, int n_arg_names);
DESCR_t pat_assign_callcap_named_imm(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs, char **arg_names, int n_arg_names);
DESCR_t var_as_pattern(DESCR_t v);
DESCR_t pat_user_call(const char *name, DESCR_t *args, int nargs);
#endif
