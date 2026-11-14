#ifndef RUNTIME_LIB_BN_02_SH_H
#define RUNTIME_LIB_BN_02_SH_H
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
__attribute__((visibility("hidden"))) int icn_open_spec_is_icon(const char *spec);
__attribute__((visibility("hidden"))) int icn_open_untranslated(const char *spec);
__attribute__((visibility("hidden"))) void icn_open_cmode(const char *spec, char *out, int *is_pipe);
__attribute__((visibility("hidden"))) DESCR_t rk_failure_new(DESCR_t ex);
#endif
