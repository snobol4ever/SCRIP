#ifndef GC_HEAP_RT_SXT_BREAK_FAST_H
#define GC_HEAP_RT_SXT_BREAK_FAST_H
#include <stdint.h>
#include <stddef.h>
#include "runtime/rt/gc_heap_g_sxt_owner.h"
#ifdef __cplusplus
extern "C" {
#endif
static inline __attribute__((always_inline)) void rt_sxt_break_fast(const char *s) { if (s && s == g_sxt_owner) g_sxt_owner = (char *)0; }
#ifdef __cplusplus
}
#endif
#endif
