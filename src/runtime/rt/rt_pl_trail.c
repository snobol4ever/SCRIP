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
    rt_gc_root_range_add_topword(base);
    return (void *)tr;
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
