#ifndef RT_PL_TRAIL_H
#define RT_PL_TRAIL_H
#include <stdint.h>
#include "descr.h"
#define PL_TR_ARENA_LG2     25
#define PL_TR_ARENA_BYTES   ((uintptr_t)1 << PL_TR_ARENA_LG2)
#define PL_TR_HEADER_BYTES  32
#define PL_TR_ENTRY_BYTES   8
#define PL_TR_VALUE_ENTRY_BYTES 24
#define PL_TR_BALL_OFF      8
#define PL_TR_FRAME_HEADER_BYTES 64
#define PL_TR_FRAME_HI_OFF 32
typedef struct { DESCR_t old; uintptr_t addr_tagged; } pl_tr_value_entry_t;
typedef struct pl_tr_ctx_s { char *tr; char *b; void *ball; } pl_tr_ctx_t;
void *rt_pl_tr_init(void);
void  rt_pl_tr_refuse(const char *tr);
void  rt_pl_tr_gc_sync(const char *tr);
char *rt_pl_tr_unwind_to(char *tr, char *mark);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline char *pl_tr_base_of(const char *tr) { return (char *)((uintptr_t)tr & ~(PL_TR_ARENA_BYTES - 1)); }
static inline void **pl_tr_ball_slot(const char *base) { return (void **)((char *)base + PL_TR_BALL_OFF); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int pl_tr_needs_log(const pl_tr_ctx_t *cx) { return cx && cx->b ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_tr_push(pl_tr_ctx_t *cx, DESCR_t *cell) {
    if (((uintptr_t)cx->tr & (PL_TR_ARENA_BYTES - 1)) >= PL_TR_ARENA_BYTES - PL_TR_VALUE_ENTRY_BYTES) rt_pl_tr_refuse(cx->tr);
    *(DESCR_t **)cx->tr = cell; cx->tr += PL_TR_ENTRY_BYTES;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_tr_push_value(pl_tr_ctx_t *cx, DESCR_t *cell) {
    if (((uintptr_t)cx->tr & (PL_TR_ARENA_BYTES - 1)) >= PL_TR_ARENA_BYTES - PL_TR_VALUE_ENTRY_BYTES) rt_pl_tr_refuse(cx->tr);
    { pl_tr_value_entry_t *e = (pl_tr_value_entry_t *)cx->tr; e->old = *cell; e->addr_tagged = (uintptr_t)cell | 1u; cx->tr += PL_TR_VALUE_ENTRY_BYTES; }
}
#endif
