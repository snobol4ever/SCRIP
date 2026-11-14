#ifndef GC_HEAP_RT_GC_CALLBACK_H
#define GC_HEAP_RT_GC_CALLBACK_H
#include <stdint.h>
#include <stddef.h>
#include "runtime/rt/rt_diag.h"
#include "runtime/rt/gc_heap.h"
#ifdef __cplusplus
extern "C" {
#endif
#if RT_DIAG
#define RT_GC_CALLBACK(expr) ({ long _cb_m = rt_gc_cb_open(); char _cb_f; __typeof__(expr) _cb_r = (expr); if (_cb_m >= 0) rt_gc_cb_close(_cb_m, __FILE__, __LINE__, (void *)&_cb_f, (void *) \
    __builtin_frame_address(0)); _cb_r; })
#define RT_GC_CALLBACK_V(expr) do { long _cb_m = rt_gc_cb_open(); char _cb_f; (expr); if (_cb_m >= 0) rt_gc_cb_close(_cb_m, __FILE__, __LINE__, (void *)&_cb_f, (void *)__builtin_frame_address(0)); } \
    while (0)
#else
#define RT_GC_CALLBACK(expr) (expr)
#define RT_GC_CALLBACK_V(expr) do { (expr); } while (0)
#endif
#ifdef __cplusplus
}
#endif
#endif
