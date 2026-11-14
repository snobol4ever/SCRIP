#ifndef GC_HEAP_HBLK_BYTES_H
#define GC_HEAP_HBLK_BYTES_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define HBLK_BYTES(h) ((uint64_t)(h)->gran << 4)
#define HBLK_SET_BYTES(h, n) ((h)->gran = (uint32_t)((uint64_t)(n) >> 4))
#ifdef __cplusplus
}
#endif
#endif
