#include <stdio.h>
#include <stdlib.h>
#include "rt_pl_trail.h"
#include "rt_slab.h"
extern void rt_gc_root_range_add_topword(const char *lo);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_tr_init(void) {
    char *raw = (char *)rt_slab_region((size_t)PL_TR_ARENA_BYTES * 2u);
    char *base = (char *)(((uintptr_t)raw + PL_TR_ARENA_BYTES - 1) & ~(PL_TR_ARENA_BYTES - 1));
    char *tr = base + PL_TR_HEADER_BYTES;
    *(char **)base = tr;
    *pl_tr_ball_slot(base) = (void *)0;
    rt_gc_root_range_add_topword(base);
    return (void *)tr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(PL_TR_HEADER_BYTES >= PL_TR_BALL_OFF + 8, "the pending-ball slot must fit inside the trail header, below the first entry");
_Static_assert(PL_TR_BALL_OFF >= 8, "the trail top word owns offset 0 of the header");
_Static_assert((uintptr_t)PL_TR_ARENA_BYTES == (uintptr_t)33554432, "rtx_plunify.s spells this arena size as the literal mask -33554432");
_Static_assert(PL_TR_BALL_OFF == 8, "rtx_plunify.s spells this offset as the literal 8");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_tr_gc_root_ball(const char *base)
{
    extern void rt_gc_visit_raw(const char **loc);
    void **slot = pl_tr_ball_slot(base);
    if (*slot) rt_gc_visit_raw((const char **)slot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_tr_refuse(const char *tr) {
    unsigned long used = (unsigned long)((uintptr_t)tr & (PL_TR_ARENA_BYTES - 1)) / PL_TR_ENTRY_BYTES;
    fprintf(stderr, "scrip: prolog: trail arena exhausted after %lu conditional bindings (PL_TR_ARENA_LG2=%d, %lu MB) -- REFUSE rc=2, not a wrong answer\n",
            used, (int)PL_TR_ARENA_LG2, (unsigned long)(PL_TR_ARENA_BYTES >> 20));
    exit(2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_tr_gc_sync(const char *tr) { *(const char **)pl_tr_base_of(tr) = tr; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_pl_tr_unwind_to(char *tr, char *mark) {
    while (tr > mark) { tr -= PL_TR_ENTRY_BYTES; { pl_tr_entry_t *e = (pl_tr_entry_t *)tr; *e->cell = e->old; } }
    return tr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_pl_tr_unwind_sync(char *tr, char *mark) {
    tr = rt_pl_tr_unwind_to(tr, mark);
    rt_pl_tr_gc_sync(tr);
    return tr;
}
