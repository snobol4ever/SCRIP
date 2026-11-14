#ifndef GC_HEAP_GV_AT_H
#define GC_HEAP_GV_AT_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define GV_AT(v, T, i) (((T *)(v).p)[i])
#ifdef __cplusplus
}
#endif
#endif
