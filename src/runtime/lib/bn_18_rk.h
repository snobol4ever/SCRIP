#ifndef RUNTIME_LIB_BN_18_RK_H
#define RUNTIME_LIB_BN_18_RK_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) int rk_type_isa(const char *have, const char *want);
__attribute__((visibility("hidden"))) char *rk_str_succ_buf(const char *s, char *b);
__attribute__((visibility("hidden"))) const char *rt_mc_type_name(DESCR_t d);
__attribute__((visibility("hidden"))) int rt_mc_accepts(const char *ptype, DESCR_t arg);
__attribute__((visibility("hidden"))) int rt_multi_meth_dispatch(const char *cname, const char *mname, DESCR_t *args, int nargs, DESCR_t *out, long *rq);
void rk_sprintf_core(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen);
__attribute__((visibility("hidden"))) int rk_fast_rat_builtin(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) const char *rk_multi_named_winner(const char *base, const DESCR_t *pos, int npos, const DESCR_t *kv, int nkv, int *none);
#endif
