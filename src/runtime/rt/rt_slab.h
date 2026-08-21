/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RT_SLAB_H
#define RT_SLAB_H
#include <stddef.h>
#include <stdint.h>
#ifndef RT_SLAB_GC_ROOTS
#define RT_SLAB_GC_ROOTS 0
#endif
typedef struct rt_slab {
    struct rt_slab *next;
    size_t          cap;
    uint32_t        klass;
    uint32_t        magic;
} rt_slab_t;
#define RT_SLAB_MAGIC 0x534C4142u
#define RT_SLAB_BASE(s) ((uint8_t *)(s) + 32)
rt_slab_t *rt_slab_get(size_t min_bytes);
void       rt_slab_put(rt_slab_t *s);
size_t     rt_slab_pool_bytes(void);
void      *rt_slab_region(size_t n);
#endif
