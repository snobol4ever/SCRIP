#ifndef RT_PL_TRAIL_H
#define RT_PL_TRAIL_H
#include <stdint.h>
#include "descr.h"
#define PL_TR_ARENA_LG2     25
#define PL_TR_ARENA_BYTES   ((uintptr_t)1 << PL_TR_ARENA_LG2)
#define PL_TR_HEADER_BYTES  32
#define PL_TR_ENTRY_BYTES   32
#define PL_TR_FRAME_HEADER_BYTES 64
typedef struct { DESCR_t *cell; uint64_t pad; DESCR_t old; } pl_tr_entry_t;
typedef struct pl_tr_ctx_s { char *tr; char *b; } pl_tr_ctx_t;
void *rt_pl_tr_init(void);
void  rt_pl_tr_refuse(const char *tr);
void  rt_pl_tr_gc_sync(const char *tr);
char *rt_pl_tr_unwind_to(char *tr, char *mark);
char *rt_pl_tr_unwind_sync(char *tr, char *mark);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline char *pl_tr_base_of(const char *tr) { return (char *)((uintptr_t)tr & ~(PL_TR_ARENA_BYTES - 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int pl_tr_needs_log(const pl_tr_ctx_t *cx, const DESCR_t *cell, const char *floor_) {
    if (!cx || !cx->b) return 0;
    if ((const char *)cell <= floor_) return 1;
    return (const char *)cell >= cx->b + PL_TR_FRAME_HEADER_BYTES;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_tr_push(pl_tr_ctx_t *cx, DESCR_t *cell) {
    if (((uintptr_t)cx->tr & (PL_TR_ARENA_BYTES - 1)) >= PL_TR_ARENA_BYTES - PL_TR_ENTRY_BYTES) rt_pl_tr_refuse(cx->tr);
    { pl_tr_entry_t *e = (pl_tr_entry_t *)cx->tr; e->cell = cell; e->pad = 0; e->old = *cell; cx->tr += PL_TR_ENTRY_BYTES; }
}
#endif
