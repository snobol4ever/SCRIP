#ifndef RUNTIME_LIB_BN_22_RK_H
#define RUNTIME_LIB_BN_22_RK_H
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
void rk_rsprintf(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen);
void rk_rsprintf_legacy(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen);
#endif
