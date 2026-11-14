#ifndef GC_HEAP_RT_HBLK_H
#define GC_HEAP_RT_HBLK_H
#include <stdint.h>
#include <stddef.h>
#include "runtime/core/g_runtime.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct rt_hblk_t { uint64_t fwd; uint32_t gran; uint16_t type; uint16_t flags; } rt_hblk_t;
#ifdef __cplusplus
}
#endif
#endif
