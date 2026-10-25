#ifndef RT_PL_TRAIL_H
#define RT_PL_TRAIL_H
#include <stdint.h>
#include "g_runtime.h"
#include "descr.h"
#ifdef __cplusplus
extern "C" {
#endif
#define PL_TR_ARENA_LG2 29
#define PL_TR_ARENA_BYTES ((uintptr_t)1 << PL_TR_ARENA_LG2)
#define PL_TR_HEADER_BYTES 128
#define PL_TR_ENTRY_BYTES 32
#define PL_TR_BALL_OFF 8
#define PL_TR_WSLOT_OFF 16
#define PL_TR_OCHECK_OFF 24
#define PL_TR_CLEANUP_OFF 32
#define PL_TR_PROBE_B_OFF 40
#define PL_TR_WAKE_OFF 48
#define PL_TR_RESIDUE_OFF 56
#define PL_TR_FLOOR_OFF 64
#define PL_TR_PAD_RESIDUE 0x45445352u
#define PL_TR_PAD_KEEP 0x5045454Bu
#define PL_ATTV_SLEN 0x56545441u
#define PL_ATTV_SEEN 0x4E454553u
#define PL_TR_FRAME_HEADER_BYTES 64
#define CLEANUP_DONE 0
#define CLEANUP_ACTIVE 1
#define CLEANUP_PENDING 2
#define CLEANUP_REC_LINK 0
#define CLEANUP_REC_BS 8
#define CLEANUP_REC_STATE 16
#define CLEANUP_REC_C 32
#define CLEANUP_REC_BYTES 48
#define PL_TR_FRAME_HI_OFF 32
#define PL_TBH_CELLS 10
typedef struct { DESCR_t *cell; uint64_t pad; DESCR_t old; } pl_tr_entry_t;
typedef struct pl_tr_ctx_s { char *tr; char *b; void *ball; uint64_t rbx; uint64_t r14; uint64_t r15; } pl_tr_ctx_t;
void *rt_pl_tr_init(void);
void *rt_pl_tr_new_arena(void **slab_out);
void rt_pl_tr_free_arena(const char *tr, void *slab);
void rt_pl_tr_refuse(const char *tr);
void rt_pl_tr_gc_sync(const char *tr);
char *rt_pl_tr_unwind_to(char *tr, char *mark);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline char *pl_tr_base_of(const char *tr) { return (char *)((uintptr_t)tr & ~(PL_TR_ARENA_BYTES - 1)); }
static inline void **pl_tr_ball_slot(const char *base) { return (void **)((char *)base + PL_TR_BALL_OFF); }
static inline void **pl_tr_wslot_slot(const char *base) { return (void **)((char *)base + PL_TR_WSLOT_OFF); }
static inline uint64_t *pl_tr_ocheck_slot(const char *base) { return (uint64_t *)((char *)base + PL_TR_OCHECK_OFF); }
static inline char **pl_tr_cleanup_slot(const char *base) { return (char **)((char *)base + PL_TR_CLEANUP_OFF); }
static inline uint64_t *pl_tr_probe_b_slot(const char *base) { return (uint64_t *)((char *)base + PL_TR_PROBE_B_OFF); }
static inline uint64_t *pl_tr_wake_slot(const char *base) { return (uint64_t *)((char *)base + PL_TR_WAKE_OFF); }
static inline uint64_t *pl_tr_residue_slot(const char *base) { return (uint64_t *)((char *)base + PL_TR_RESIDUE_OFF); }
static inline char **pl_tr_floor_slot(const char *base) { return (char **)((char *)base + PL_TR_FLOOR_OFF); }
static inline int pl_attv_is(const DESCR_t *c) { return c->v == (DTYPE_t)DT_PLVAR && c->p == (const void *)c && c->slen == PL_ATTV_SLEN; }
static inline uint64_t pl_tr_ocheck_mode(const pl_tr_ctx_t *cx) { return (cx && cx->tr) ? *pl_tr_ocheck_slot(pl_tr_base_of(cx->tr)) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline uintptr_t pl_tr_frame_hi(const char *b) { return *(const uintptr_t *)(b + PL_TR_FRAME_HI_OFF) >> 8; }
static inline void pl_tr_frame_hi_set(char *b, const void *hi) { *(uintptr_t *)(b + PL_TR_FRAME_HI_OFF) = ((uintptr_t)hi << 8) | DT_RAW; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int pl_tr_needs_log(const pl_tr_ctx_t *cx, const DESCR_t *cell, const char *floor_) {
    if (!cx || !cx->b) return 0;
    if ((const char *)cell <= floor_) return 1;
    return (uintptr_t)cell >= pl_tr_frame_hi(cx->b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char *pl_tr_hb_of(const pl_tr_ctx_t *cx) {
    const char *mark = *(const char * const *)cx->b;
    if (mark < pl_tr_base_of(cx->tr) + PL_TR_HEADER_BYTES + PL_TR_ENTRY_BYTES || mark > cx->tr) return 0;
    { const pl_tr_entry_t *e = (const pl_tr_entry_t *)(mark - PL_TR_ENTRY_BYTES); return e->cell ? 0 : (const char *)(uintptr_t)e->pad; }
}
static inline int pl_tr_needs_log_bind(const pl_tr_ctx_t *cx, const DESCR_t *cell, const char *floor_) {
    if (!cx || !cx->b) return 0;
    if ((const char *)cell > floor_) return (uintptr_t)cell >= pl_tr_frame_hi(cx->b);
    { const char *hb = pl_tr_hb_of(cx); return !hb || (const char *)cell < hb || (const char *)cell >= *(char * const *)(const void *)&g_runtime.abi.hp_fr; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_tr_push(pl_tr_ctx_t *cx, DESCR_t *cell) {
    if (((uintptr_t)cx->tr & (PL_TR_ARENA_BYTES - 1)) >= PL_TR_ARENA_BYTES - PL_TR_ENTRY_BYTES) rt_pl_tr_refuse(cx->tr);
    { pl_tr_entry_t *e = (pl_tr_entry_t *)cx->tr; e->cell = cell; e->pad = 0; e->old = *cell; cx->tr += PL_TR_ENTRY_BYTES; *(char **)pl_tr_base_of(cx->tr) = cx->tr; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int pl_tr_residue_on(const pl_tr_ctx_t *cx) { return cx && cx->tr && *pl_tr_residue_slot(pl_tr_base_of(cx->tr)) != 0; }
static inline void pl_tr_push_residue(pl_tr_ctx_t *cx, DESCR_t *cell) { pl_tr_push(cx, cell); ((pl_tr_entry_t *)(cx->tr - PL_TR_ENTRY_BYTES))->pad = PL_TR_PAD_RESIDUE; }
static inline void pl_tr_push_keep(pl_tr_ctx_t *cx, DESCR_t *cell) { pl_tr_push(cx, cell); ((pl_tr_entry_t *)(cx->tr - PL_TR_ENTRY_BYTES))->pad = PL_TR_PAD_KEEP; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void pl_tr_push_attv(pl_tr_ctx_t *cx, DESCR_t *cell) {
    char *base = pl_tr_base_of(cx->tr);
    uint64_t off = (uint64_t)(cx->tr - base);
    uint64_t *w = pl_tr_wake_slot(base);
    if (!*w || *w > off) *w = off;
    pl_tr_push(cx, cell);
}
#ifdef __cplusplus
}
#endif
#endif
