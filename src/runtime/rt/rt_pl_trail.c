#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include "rt_pl_trail.h"
#define PL_TR_VA_TOP ((uintptr_t)1 << 47)
extern void rt_gc_root_range_add_topword(const char *lo);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_tr_init(void) {
    char *base = (char *)0;
    for (uintptr_t a = PL_TR_ARENA_BYTES; !base && a + PL_TR_ARENA_BYTES <= PL_TR_VA_TOP; a += PL_TR_ARENA_BYTES) {
        void *m = mmap((void *)a, PL_TR_WINDOW_BYTES, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_FIXED_NOREPLACE, -1, 0);
        if (m == (void *)a) base = (char *)m;
        else if (m != MAP_FAILED) munmap(m, PL_TR_WINDOW_BYTES);
    }
    if (!base) {
        fprintf(stderr, "scrip: prolog: no %lu GB-aligned block of address space below 2^47 is free for the trail -- REFUSE rc=2, not a wrong answer\n",
                (unsigned long)(PL_TR_ARENA_BYTES >> 30));
        exit(2);
    }
    char *tr = base + PL_TR_HEADER_BYTES;
    *(char **)base = tr;
    *pl_tr_ball_slot(base) = (void *)0;
    *(char **)(base + PL_TR_END_OFF) = base + PL_TR_WINDOW_BYTES;
    rt_gc_root_range_add_topword(base);
    return (void *)tr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(PL_TR_HEADER_BYTES >= PL_TR_END_OFF + 8, "the trail's mapped-end word must fit inside the trail header, below the first entry");
_Static_assert(PL_TR_END_OFF >= PL_TR_BALL_OFF + 8, "the mapped-end word sits above the pending-ball slot");
_Static_assert(PL_TR_BALL_OFF >= 8, "the trail top word owns offset 0 of the header");
_Static_assert((uintptr_t)PL_TR_ARENA_BYTES == (uintptr_t)1099511627776, "rtx_plunify.s and x86_asm.h spell this block size as the literal mask -1099511627776");
_Static_assert(PL_TR_BALL_OFF == 8, "rtx_plunify.s spells this offset as the literal 8");
_Static_assert(PL_TR_WINDOW_BYTES % 4096 == 0 && PL_TR_WINDOW_BYTES <= PL_TR_ARENA_BYTES, "the first window is whole pages inside the aligned block");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_tr_gc_root_ball(const char *base)
{
    extern void rt_gc_visit_raw(const char **loc);
    void **slot = pl_tr_ball_slot(base);
    if (*slot) rt_gc_visit_raw((const char **)slot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_tr_grow(const char *tr) {
    char *base = pl_tr_base_of(tr), *end = *(char **)(base + PL_TR_END_OFF);
    size_t have = (size_t)(end - base), add = have;
    unsigned long used = (unsigned long)((size_t)(tr - base - PL_TR_HEADER_BYTES) / PL_TR_ENTRY_BYTES);
    if (add > PL_TR_ARENA_BYTES - have) add = PL_TR_ARENA_BYTES - have;
    if (add >= PL_TR_ENTRY_BYTES) {
        void *m = mmap((void *)end, add, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_FIXED_NOREPLACE, -1, 0);
        if (m == (void *)end) { *(char **)(base + PL_TR_END_OFF) = end + add; return; }
        { int e = errno; if (m != MAP_FAILED) munmap(m, add);
          fprintf(stderr, "scrip: prolog: the trail holds %lu conditional bindings in %lu MB and its next %lu MB could not be mapped (%s) -- REFUSE rc=2, not a wrong answer\n",
                  used, (unsigned long)(have >> 20), (unsigned long)(add >> 20), m == MAP_FAILED ? strerror(e) : "the address space past the trail is taken");
          exit(2); }
    }
    fprintf(stderr, "scrip: prolog: the trail holds %lu conditional bindings and fills its %lu GB block -- REFUSE rc=2, not a wrong answer\n",
            used, (unsigned long)(PL_TR_ARENA_BYTES >> 30));
    exit(2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_tr_gc_sync(const char *tr) { *(const char **)pl_tr_base_of(tr) = tr; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_pl_tr_unwind_to(char *tr, char *mark) {
    while (tr > mark) { tr -= PL_TR_ENTRY_BYTES; { pl_tr_entry_t *e = (pl_tr_entry_t *)tr; *e->cell = e->old; } }
    return tr;
}
