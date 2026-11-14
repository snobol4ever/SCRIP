#ifndef RUNTIME_LIB_BN_41_SH_H
#define RUNTIME_LIB_BN_41_SH_H
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) int try_call_builtin_by_name_bl_s_rq(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen, int strict, long *rq);
#endif
