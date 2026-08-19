/* rt_slab.h — RUNG TR-1: the slab pool. THE ONLY sanctioned malloc caller (TR-8 end state).
 * REGION 2 substrate: named bump arenas (rt_arena.h) draw slabs from this pool.
 * D5 (proposed default, flip-cheap): three size classes, ONE mutex. */
#ifndef RT_SLAB_H
#define RT_SLAB_H
#include <stddef.h>
#include <stdint.h>

/* TR-3 COMPENSATION (s37) — DEAD SINCE TR-4 (GC-U-4): the external-GC root registration this flag once gated is gone; #define kept at 0, gates nothing tree-wide. */
#ifndef RT_SLAB_GC_ROOTS
#define RT_SLAB_GC_ROOTS 0
#endif

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
/* TR-2: one CONTIGUOUS region of exactly >= n bytes (the big bump-heap backings —
 * ZLS/ZLS2/GC-0/ZH — need contiguity: their cursors do raw pointer arithmetic within
 * [base,end), and ZLS2 grows DOWN from hi). Lazily faulted: the pages are untouched
 * until the consumer's cursor reaches them, so a large reserve costs address space,
 * not RSS — same property MAP_NORESERVE gave the old mmaps. */
void      *rt_slab_region(size_t n);
#endif
