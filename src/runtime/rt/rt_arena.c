#include "rt_arena.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define AL16(n) (((n) + 15u) & ~(size_t)15u)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_arena_init(rt_arena_t *a, arena_flavor_t f) {
    memset(a, 0, sizeof *a);
    a->flavor = f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void arena_new_slab(rt_arena_t *a, size_t need) {
    rt_slab_t *s = rt_slab_get(need);
    s->next = a->head;
    a->head = s;
    a->cur  = RT_SLAB_BASE(s);
    a->end  = a->cur + s->cap;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_arena_alloc(rt_arena_t *a, size_t n) {
    size_t hdr = 0;
#if RT_ARENA_TITLE
    if (a->flavor == A_PROG) hdr = 16;
#endif
    size_t need = AL16(n) + hdr;
    if (!a->head || (size_t)(a->end - a->cur) < need) arena_new_slab(a, need);
    uint8_t *p = a->cur;
    a->cur += need;
#if RT_ARENA_ZERO
    memset(p, 0, need);
#endif
    if (hdr) { *(uint64_t *)p = ((uint64_t)n << 8) | (uint64_t)a->flavor; p += hdr; }
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
arena_mark_t rt_arena_mark(rt_arena_t *a) {
    arena_mark_t m = { a->head, a->cur };
    return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_arena_release(rt_arena_t *a, arena_mark_t m) {
    if (a->flavor == A_PROG) { fprintf(stderr, "rt_arena_release: A_PROG is immortal\n"); abort(); }
    while (a->head && a->head != m.slab) {
        rt_slab_t *s = a->head;
        a->head = s->next;
        rt_slab_put(s);
    }
    if (a->head) {
        a->cur = m.cur;
        a->end = RT_SLAB_BASE(a->head) + a->head->cap;
    } else {
        a->cur = a->end = NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_arena_destroy(rt_arena_t *a) {
    while (a->head) { rt_slab_t *s = a->head; a->head = s->next; rt_slab_put(s); }
    a->cur = a->end = NULL; a->reuse = NULL; a->block_sz = 0;
}
typedef struct zblk { struct zblk *next; } zblk_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_arena_zblock_get(rt_arena_t *a, size_t block_sz) {
    if (a->block_sz == 0) a->block_sz = AL16(block_sz);
    if (AL16(block_sz) != a->block_sz) {
        fprintf(stderr, "rt_arena_zblock_get: mixed block sizes (%zu vs %zu)\n",
                AL16(block_sz), a->block_sz);
        abort();
    }
    if (a->reuse) {
        zblk_t *b = (zblk_t *)a->reuse;
        a->reuse = b->next;
#if RT_ARENA_ZERO
        memset(b, 0, a->block_sz);
#endif
        return b;
    }
    return rt_arena_alloc(a, a->block_sz);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_arena_zblock_put(rt_arena_t *a, void *blk) {
    if (!blk) return;
    zblk_t *b = (zblk_t *)blk;
    b->next = (zblk_t *)a->reuse;
    a->reuse = b;
}
