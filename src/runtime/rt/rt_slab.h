/* rt_slab.h — RUNG TR-1: the slab pool. THE ONLY sanctioned malloc caller (TR-8 end state).
 * REGION 2 substrate: named bump arenas (rt_arena.h) draw slabs from this pool.
 * D5 (proposed default, flip-cheap): three size classes, ONE mutex. */
#ifndef RT_SLAB_H
#define RT_SLAB_H
#include <stddef.h>
#include <stdint.h>

typedef struct rt_slab {
    struct rt_slab *next;   /* free-list / arena chain link */
    size_t          cap;    /* usable bytes at base */
    uint32_t        klass;  /* size-class index */
    uint32_t        magic;  /* RT_SLAB_MAGIC */
    /* payload follows, 16-aligned */
} rt_slab_t;

#define RT_SLAB_MAGIC 0x534C4142u /* 'SLAB' */
#define RT_SLAB_BASE(s) ((uint8_t *)(s) + 32) /* header padded to 32 => payload 16-aligned */

rt_slab_t *rt_slab_get(size_t min_bytes); /* >= min_bytes usable; from free list or malloc */
void       rt_slab_put(rt_slab_t *s);     /* return to free list (never freed to libc)     */
size_t     rt_slab_pool_bytes(void);      /* total bytes ever drawn from malloc (ledger)   */
#endif
