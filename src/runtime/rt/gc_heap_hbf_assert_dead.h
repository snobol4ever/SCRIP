#ifndef GC_HEAP_HBF_ASSERT_DEAD_H
#define GC_HEAP_HBF_ASSERT_DEAD_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define HBF_TTL 0x0001
#define HBF_MARK 0x0002
#define HBF_ASSERT_DEAD 0x0004
#define HBF_ASSERT_INST 0x0008
#define HBF_VIS 0x0010
#ifdef __cplusplus
}
#endif
#endif
