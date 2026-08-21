/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RT_ARENA_H
#define RT_ARENA_H
#include <stddef.h>
#include <stdint.h>
#include "rt_slab.h"
#ifndef RT_ARENA_ZERO
#define RT_ARENA_ZERO 1
#endif
#ifndef RT_ARENA_TITLE
#define RT_ARENA_TITLE 1
#endif
typedef enum { A_PROG = 0, A_TRANS = 1, A_SUSP = 2, A_COEXPR = 3 } arena_flavor_t;
typedef struct rt_arena {
    rt_slab_t     *head;
    uint8_t       *cur, *end;
    arena_flavor_t flavor;
    void          *reuse;
    size_t         block_sz;
} rt_arena_t;
typedef struct { rt_slab_t *slab; uint8_t *cur; } arena_mark_t;
void         rt_arena_init(rt_arena_t *a, arena_flavor_t f);
void        *rt_arena_alloc(rt_arena_t *a, size_t n);
arena_mark_t rt_arena_mark(rt_arena_t *a);
void         rt_arena_release(rt_arena_t *a, arena_mark_t m);
void         rt_arena_destroy(rt_arena_t *a);
void        *rt_arena_zblock_get(rt_arena_t *a, size_t block_sz);
void         rt_arena_zblock_put(rt_arena_t *a, void *blk);
void        *rt_ws_alloc(size_t n);
void        *rt_ws_realloc(void *p, size_t n);
char        *rt_ws_strdup(const char *s);
void        *rt_ws_alloc_c(size_t n);
char        *rt_ws_strdup_c(const char *s);
void        *rt_agg_alloc(int kind, size_t n);
void        *rt_pl_cterm_alloc(size_t n);
arena_mark_t rt_pl_cterm_mark(void);
void         rt_pl_cterm_release(arena_mark_t m);
int          rt_pl_ctr_on(void);
int          rt_pl_cellws_on(void);
void        *rt_pl_cellws_alloc(size_t n);
arena_mark_t rt_pl_cellws_mark(void);
void         rt_pl_cellws_release(arena_mark_t m);
void         rt_pl_cterm_roots(void **base, size_t *bytes);
#endif
