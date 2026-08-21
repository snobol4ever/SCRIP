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
#define RT_PL_CTERM_ISLAND_BYTES ((size_t)16u << 20)
#define RT_PL_CTERM_POISON 0xDE
static uint8_t *g_pl_cterm_base = 0;
static uint8_t *g_pl_cterm_cur  = 0;
static uint8_t *g_pl_cterm_end  = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_cterm_poison(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_PL_CTERM_POISON"); p = e ? (atoi(e) != 0) : 0; } return p; }
int rt_pl_ctr_on(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_NO_CTR"); p = e ? (atoi(e) == 0) : 1; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_pl_cterm_lazy_init(void) {
    extern void *rt_slab_region(size_t);
    if (g_pl_cterm_base) return;
    g_pl_cterm_base = (uint8_t *)rt_slab_region(RT_PL_CTERM_ISLAND_BYTES);
    if (!g_pl_cterm_base) { fprintf(stderr, "rt_pl_cterm: island reserve failed\n"); abort(); }
    g_pl_cterm_cur = g_pl_cterm_base;
    g_pl_cterm_end = g_pl_cterm_base + RT_PL_CTERM_ISLAND_BYTES;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_cterm_alloc(size_t n) {
    if (!g_pl_cterm_base) rt_pl_cterm_lazy_init();
    size_t need = AL16(n);
    if ((size_t)(g_pl_cterm_end - g_pl_cterm_cur) < need) { fprintf(stderr, "rt_pl_cterm: island exhausted (%zu used)\n", (size_t)(g_pl_cterm_cur - g_pl_cterm_base)); abort(); }
    uint8_t *p = g_pl_cterm_cur;
    g_pl_cterm_cur += need;
#if RT_ARENA_ZERO
    memset(p, 0, need);
#endif
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
arena_mark_t rt_pl_cterm_mark(void) {
    if (!g_pl_cterm_base) rt_pl_cterm_lazy_init();
    arena_mark_t m = { (rt_slab_t *)0, g_pl_cterm_cur };
    return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cterm_release(arena_mark_t m) {
    if (!g_pl_cterm_base || !m.cur) return;
    if (m.cur < g_pl_cterm_base || m.cur > g_pl_cterm_cur) { fprintf(stderr, "rt_pl_cterm: release out of range (non-LIFO)\n"); abort(); }
    if (rt_pl_cterm_poison() && g_pl_cterm_cur > m.cur) memset(m.cur, RT_PL_CTERM_POISON, (size_t)(g_pl_cterm_cur - m.cur));
    g_pl_cterm_cur = m.cur;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cterm_roots(void **base, size_t *bytes) { if (base) *base = (void *)g_pl_cterm_base; if (bytes) *bytes = (size_t)(g_pl_cterm_cur - g_pl_cterm_base); }
#define RT_PL_CELLWS_ISLAND_BYTES ((size_t)64u << 20)
static uint8_t *g_pl_cellws_base = 0;
static uint8_t *g_pl_cellws_cur  = 0;
static uint8_t *g_pl_cellws_end  = 0;
int g_plw_cellws_on = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_cellws_on(void) { if (g_plw_cellws_on < 0) { const char *e = getenv("SCRIP_PL_WS_RECLAIM"); g_plw_cellws_on = e ? (atoi(e) != 0) : 0; } return g_plw_cellws_on; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_pl_cellws_lazy_init(void) {
    extern void *rt_slab_region(size_t);
    if (g_pl_cellws_base) return;
    g_pl_cellws_base = (uint8_t *)rt_slab_region(RT_PL_CELLWS_ISLAND_BYTES);
    if (!g_pl_cellws_base) { fprintf(stderr, "rt_pl_cellws: island reserve failed\n"); abort(); }
    g_pl_cellws_cur = g_pl_cellws_base;
    g_pl_cellws_end = g_pl_cellws_base + RT_PL_CELLWS_ISLAND_BYTES;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_cellws_alloc(size_t n) {
    if (!g_pl_cellws_base) rt_pl_cellws_lazy_init();
    size_t need = AL16(n);
    if ((size_t)(g_pl_cellws_end - g_pl_cellws_cur) < need) { fprintf(stderr, "rt_pl_cellws: island exhausted (%zu used)\n", (size_t)(g_pl_cellws_cur - g_pl_cellws_base)); abort(); }
    uint8_t *p = g_pl_cellws_cur;
    g_pl_cellws_cur += need;
    memset(p, 0, need);
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
arena_mark_t rt_pl_cellws_mark(void) {
    if (!g_pl_cellws_base) rt_pl_cellws_lazy_init();
    arena_mark_t m = { (rt_slab_t *)0, g_pl_cellws_cur };
    return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cellws_release(arena_mark_t m) {
    if (!g_pl_cellws_base || !m.cur) return;
    if (m.cur < g_pl_cellws_base || m.cur > g_pl_cellws_cur) return;
    g_pl_cellws_cur = m.cur;
}
