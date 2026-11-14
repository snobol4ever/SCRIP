#ifndef RUNTIME_LIB_BN_28_SH_H
#define RUNTIME_LIB_BN_28_SH_H
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) int dop_ax_core(const char *op, DESCR_t *args, int nargs, DESCR_t *out, void **ball);
#endif
