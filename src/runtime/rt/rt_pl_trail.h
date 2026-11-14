#ifndef RT_PL_TRAIL_H
#define RT_PL_TRAIL_H
#include <stdint.h>
#include "ir/descr.h"
#include "runtime/rt/rt_pl_trail_pl_tr_ctx_s.h"
#ifdef __cplusplus
extern "C" {
#endif
void *rt_pl_tr_init(void);
void *rt_pl_tr_new_arena(void **slab_out);
void rt_pl_tr_free_arena(const char *tr, void *slab);
void rt_pl_tr_refuse(const char *tr);
void rt_pl_tr_gc_sync(const char *tr);
char *rt_pl_tr_unwind_to(char *tr, char *mark);
void rt_pl_tr_freeze(void);
char *pl_tr_base_of(const char *tr);
void **pl_tr_ball_slot(const char *base);
void **pl_tr_wslot_slot(const char *base);
uint64_t *pl_tr_ocheck_slot(const char *base);
char **pl_tr_cleanup_slot(const char *base);
uint64_t *pl_tr_probe_b_slot(const char *base);
uint64_t *pl_tr_wake_slot(const char *base);
uint64_t *pl_tr_residue_slot(const char *base);
char **pl_tr_floor_slot(const char *base);
int pl_attv_is(const DESCR_t *c);
uint64_t pl_var_word0(const DESCR_t *c);
uint64_t pl_var_serial(const DESCR_t *c);
uint64_t pl_serial_draw(const pl_tr_ctx_t *cx);
void pl_var_make(DESCR_t *c, const pl_tr_ctx_t *cx, int attv);
void pl_var_move(DESCR_t *j, const DESCR_t *old, const pl_tr_ctx_t *cx);
uint64_t pl_tr_ocheck_mode(const pl_tr_ctx_t *cx);
uintptr_t pl_tr_frame_hi(const char *b);
void pl_tr_frame_hi_set(char *b, const void *hi);
int pl_tr_needs_log(const pl_tr_ctx_t *cx, const DESCR_t *cell, const char *floor_);
const char *pl_tr_hb_of(const pl_tr_ctx_t *cx);
int pl_tr_needs_log_bind(const pl_tr_ctx_t *cx, const DESCR_t *cell, const char *floor_);
void pl_tr_push(pl_tr_ctx_t *cx, DESCR_t *cell);
int pl_tr_residue_on(const pl_tr_ctx_t *cx);
void pl_tr_push_residue(pl_tr_ctx_t *cx, DESCR_t *cell);
void pl_tr_push_keep(pl_tr_ctx_t *cx, DESCR_t *cell);
void pl_tr_push_attv(pl_tr_ctx_t *cx, DESCR_t *cell);
#ifdef __cplusplus
}
#endif
#endif
