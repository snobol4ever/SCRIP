/* rt_arena.c — RUNG TR-1 kernel; TR-3 (s37) began flipping consumers — rt_ws_alloc is the Region-2 workspace entry. */
#include "rt_arena.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define AL16(n) (((n) + 15u) & ~(size_t)15u)

void rt_arena_init(rt_arena_t *a, arena_flavor_t f) {
    memset(a, 0, sizeof *a);
    a->flavor = f;
}

static void arena_new_slab(rt_arena_t *a, size_t need) {
    rt_slab_t *s = rt_slab_get(need);
    s->next = a->head;          /* chain newest-first */
    a->head = s;
    a->cur  = RT_SLAB_BASE(s);
    a->end  = a->cur + s->cap;
}

void *rt_arena_alloc(rt_arena_t *a, size_t n) {
    size_t hdr = 0;
#if RT_ARENA_TITLE
    if (a->flavor == A_PROG) hdr = 16; /* 8B title word (size<<8|flavor) + 8B pad, keeps 16-align */
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

arena_mark_t rt_arena_mark(rt_arena_t *a) {
    arena_mark_t m = { a->head, a->cur };
    return m;
}

void rt_arena_release(rt_arena_t *a, arena_mark_t m) {
    if (a->flavor == A_PROG) { fprintf(stderr, "rt_arena_release: A_PROG is immortal\n"); abort(); }
    while (a->head && a->head != m.slab) {   /* return slabs newer than the mark */
        rt_slab_t *s = a->head;
        a->head = s->next;
        rt_slab_put(s);
    }
    if (a->head) {
        a->cur = m.cur;
        a->end = RT_SLAB_BASE(a->head) + a->head->cap;
    } else {                                  /* mark predates every live slab (or {NULL,NULL}) */
        a->cur = a->end = NULL;
    }
}

void rt_arena_destroy(rt_arena_t *a) {
    while (a->head) { rt_slab_t *s = a->head; a->head = s->next; rt_slab_put(s); }
    a->cur = a->end = NULL; a->reuse = NULL; a->block_sz = 0;
}

/* --- A_COEXPR reusable ζ blocks (Lon s36 ruling): fixed size, free-list recycled --- */
typedef struct zblk { struct zblk *next; } zblk_t;

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

void rt_arena_zblock_put(rt_arena_t *a, void *blk) {
    if (!blk) return;
    zblk_t *b = (zblk_t *)blk;
    b->next = (zblk_t *)a->reuse;
    a->reuse = b;
}

/* --- THE WORKSPACE (RUNG TR-3, s37 three-region): the ONE Region-2 instance families flip onto; grow-only A_PROG until GC-W-2 collects it --- */
static rt_arena_t g_ws;
static int g_ws_up = 0;

void *rt_ws_alloc(size_t n) {
    if (!g_ws_up) { rt_arena_init(&g_ws, A_PROG); g_ws_up = 1; }
    return rt_arena_alloc(&g_ws, n);
}

/* --- grow-only realloc: the D6 title word (size<<8|flavor at p-16) IS the old size; no free until GC-W-2 --- */
void *rt_ws_realloc(void *p, size_t n) {
    if (!p) return rt_ws_alloc(n);
    size_t old = (size_t)(*(uint64_t *)((uint8_t *)p - 16) >> 8);
    if (n <= old) return p;
    void *q = rt_ws_alloc(n);
    memcpy(q, p, old);
    return q;
}

char *rt_ws_strdup(const char *s) {
    if (!s) return (char *)0;
    size_t n = strlen(s);
    char *q = (char *)rt_ws_alloc(n + 1);
    memcpy(q, s, n + 1);
    return q;
}
