#ifndef RUNTIME_LIB_BN_20_PL_H
#define RUNTIME_LIB_BN_20_PL_H
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_rk_redisp.h"
#include "runtime/lib/bn_head_pl_flag.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) int rk_is_rakudo_method(const char *m);
__attribute__((visibility("hidden"))) int bnd_handles_need(const char *cls, const char *meth);
__attribute__((visibility("hidden"))) const char *resolve_method_chain(const char *cls, const char *mname, int *pfound);
__attribute__((visibility("hidden"))) int meth_is_user_proc(const char *procname);
extern __attribute__((visibility("hidden"))) rk_redisp_t *g_redisp_cur;
__attribute__((visibility("hidden"))) int rk_class_user_meth(DESCR_t d, const char *m);
__attribute__((visibility("hidden"))) int rd_mro_len(const char *cname);
__attribute__((visibility("hidden"))) DESCR_t invoke_method_proc_rq(const char *procname, DESCR_t *callargs, int total, long *rq);
__attribute__((visibility("hidden"))) DESCR_t invoke_method_proc(const char *procname, DESCR_t *callargs, int total);
__attribute__((visibility("hidden"))) int pl_round_mode(const char *v);
__attribute__((visibility("hidden"))) int pl_edin_revert(int is_out);
__attribute__((visibility("hidden"))) const char *pl_dia_flag(const char *nm);
__attribute__((visibility("hidden"))) void pl_flags_ready(void);
__attribute__((visibility("hidden"))) pl_flag_t * pl_flag_find_or_create(const char *nm);
__attribute__((visibility("hidden"))) int pl_restraint_kind(const char *nm);
#endif
