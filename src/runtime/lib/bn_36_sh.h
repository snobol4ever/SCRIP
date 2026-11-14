#ifndef RUNTIME_LIB_BN_36_SH_H
#define RUNTIME_LIB_BN_36_SH_H
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) int script_try_call_builtin_by_name_rq(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, long *rq);
#endif
