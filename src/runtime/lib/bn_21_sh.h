#ifndef RUNTIME_LIB_BN_21_SH_H
#define RUNTIME_LIB_BN_21_SH_H
#include "driver/driver_data.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
extern __attribute__((visibility("hidden"))) const char *const g_rk_uni_types[6];
__attribute__((visibility("hidden"))) int rk_uni_method(const char *m, DESCR_t self, int nmargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int rk_str_norm_method(const char *m, DESCR_t recv, int nmargs, DESCR_t *out);
DESCR_t rk_attr_defaults(DatType *dt, DESCR_t inst);
#endif
