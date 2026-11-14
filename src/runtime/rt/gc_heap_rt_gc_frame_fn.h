#ifndef GC_HEAP_RT_GC_FRAME_FN_H
#define GC_HEAP_RT_GC_FRAME_FN_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef void (*rt_gc_frame_fn)(const void *map, const char *base, void *a);
#ifdef __cplusplus
}
#endif
#endif
