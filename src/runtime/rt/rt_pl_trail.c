#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "pl_trail_layout.h"
#include "rt_pl_trail.h"
#include "runtime/core/g_runtime.h"
#include "runtime/core/g_runtime_entry.h"
#include "rt_slab.h"
extern void rt_gc_root_range_add_topword(const char *lo);
extern void rt_gcheap_cap_road(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pl_tr_arena_open(char *raw) {
    char *base = (char *)(((uintptr_t)raw + PL_TR_ARENA_BYTES - 1) & ~(PL_TR_ARENA_BYTES - 1));
    char *tr = base + PL_TR_HEADER_BYTES;
    *(char **)base = tr;
    *pl_tr_ball_slot(base) = (void *)0;
    *pl_tr_wslot_slot(base) = (void *)0;
    *pl_tr_ocheck_slot(base) = 0;
    *pl_tr_cleanup_slot(base) = (char *)0;
    *pl_tr_probe_b_slot(base) = 0;
    *pl_tr_wake_slot(base) = 0;
    *pl_tr_residue_slot(base) = 0;
    *pl_tr_floor_slot(base) = (char *)0;
    memset(base + PL_TR_UNDO_LIST_OFF, 0, 2 * sizeof(DESCR_t));
    *(char **)(base + PL_TR_CATCH_OFF) = (char *)0;
    *(void **)(base + PL_TR_OVF_BALL_OFF) = (void *)0;
    memset(base + PL_TR_ALARM_DUE_OFF, 0, PL_TR_HEADER_BYTES - PL_TR_ALARM_DUE_OFF);
    rt_gc_root_range_add_topword(base);
    return tr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_tr_ovf_ball_make(char *tr) {
    extern void *rt_pl_cap_error(void);
    extern void prolog_atom_init(void);
    if (g_runtime.atoms.atom_dot < 0) prolog_atom_init();
    *(void **)(pl_tr_base_of(tr) + PL_TR_OVF_BALL_OFF) = rt_pl_cap_error();
}
void *rt_pl_tr_init(void) { char *tr = pl_tr_arena_open((char *)rt_slab_region((size_t)PL_TR_ARENA_BYTES * 2u)); rt_gcheap_cap_road(); pl_tr_ovf_ball_make(tr); return (void *)tr; }
void *rt_pl_tr_new_arena(void **slab_out) {
    rt_slab_t *s = rt_slab_try_get((size_t)PL_TR_ARENA_BYTES * 2u);
    if (!s) return (void *)0;
    *slab_out = (void *)s;
    { char *tr = pl_tr_arena_open((char *)RT_SLAB_BASE(s)); pl_tr_ovf_ball_make(tr); return (void *)tr; }
}
void rt_pl_tr_free_arena(const char *tr, void *slab) {
    extern void rt_gc_root_range_del(const char *lo);
    if (!tr || !slab) return;
    if (*(int64_t *)(pl_tr_base_of(tr) + PL_TR_ALARM_MADE_OFF)) timer_delete(*(timer_t *)(pl_tr_base_of(tr) + PL_TR_ALARM_TIMER_OFF));
    rt_gc_root_range_del(pl_tr_base_of(tr));
    rt_slab_put((rt_slab_t *)slab);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(PL_TR_HEADER_BYTES >= PL_TR_BALL_OFF + 8, "the pending-ball slot must fit inside the trail header, below the first entry");
_Static_assert(PL_TR_WAKE_OFF == 48 && PL_TR_HEADER_BYTES >= PL_TR_WAKE_OFF + 8,
    "the attvar wake word (the lowest trail offset of an unwoken attributed-variable binding, 0 none): bb_cleanup.cpp, bb_pl_cell.h and rtx_plunify.s read offset 48");
_Static_assert(PL_VAR_ATTV == 0x100 && PL_SERIAL_SHIFT == 9 && PL_SERIAL_STEP == 512 && PL_SERIAL_CELL == 255 && __builtin_offsetof(DESCR_t, src_node0) == 1,
    "an unbound variable's word 0 is (serial << 9) | (attv << 8) | DT_PLVAR: the attributed bit is bit 0 of src_node0, rtx_plunify.s and bb_pl_cell.h spell it as the byte at +1 and the immediate 0x1"
    "48, and the serial counter is " "standing cell 255 of the root record, [r14 - 24 - 8 * 255] = [r14 - 2064]");
_Static_assert(PL_TR_RESIDUE_OFF == PL_TR_WAKE_OFF + 8 && PL_TR_HEADER_BYTES >= PL_TR_RESIDUE_OFF + 8,
    "the call_residue_vars/2 depth word, how many of its goals are running (0 none): while it is non-zero every attribute block made or changed is pushed on the trail with "
    "the pad PL_TR_PAD_RESIDUE and $crv_collect reads those entries from the call's mark; the prelude raises and lowers it on every exit of the goal, so it is never trailed");
_Static_assert(PL_TR_BALL_OFF >= 8, "the trail top word owns offset 0 of the header");
_Static_assert((uintptr_t)PL_TR_ARENA_BYTES == (uintptr_t)536870912, "rtx_plunify.s and x86_asm.h spell this arena size as the literal mask -536870912");
_Static_assert(PL_TR_BALL_OFF == 8, "rtx_plunify.s spells this offset as the literal 8");
_Static_assert(PL_TR_OCHECK_OFF == 24 && PL_TR_HEADER_BYTES >= PL_TR_OCHECK_OFF + 8, "the occurs_check mode word (0 false, 1 true, 2 error): rtx_plunify.s and bb_unify_value.cpp read offset 24");
_Static_assert(PL_TR_HEADER_BYTES == 160 && PL_TR_FLOOR_OFF == 64 && PL_TR_FLOOR_OFF == PL_TR_RESIDUE_OFF + 8 && PL_TR_CLEANUP_OFF == 32 && PL_TR_PROBE_B_OFF == 40 &&
    PL_TR_HEADER_BYTES >= PL_TR_PROBE_B_OFF + 8,
    "the cleanup record cell (a stack address, never a heap pointer) and the cut probe's B word: bb_cleanup.cpp, xa_flat.cpp and gc_heap.c's trail scan read these offsets");
_Static_assert(PL_TR_ALARM_DUE_OFF == 128 && PL_TR_ALARM_DEADLINE_OFF == 136 && PL_TR_ALARM_TIMER_OFF == 144 && PL_TR_ALARM_MADE_OFF == 152 && sizeof(timer_t) <= 8 &&
    PL_TR_HEADER_BYTES == PL_TR_ALARM_MADE_OFF + 8,
    "the goal interrupt words (ARCH-PROLOG-DATABASE 19.2): DUE int32, DEADLINE monotonic microseconds, the timer, the timer-made word; the SIGALRM handler stores DUE through the timer value");
_Static_assert(PL_TR_UNDO_LIST_OFF == 80 && PL_TR_UNDO_PENDING_OFF == PL_TR_UNDO_LIST_OFF + 16 && sizeof(DESCR_t) == 16 && PL_TR_UNDO_PENDING_OFF + 16 <= PL_TR_CATCH_OFF,
    "undo/1 (CEO-2035 section 10.3): UNDO_LIST, the goals recorded and trailed as a binding of this header cell, and UNDO_PENDING, the list as the first unwind since the last probe found it; x86_cal"
    "l.cpp spells both offsets");
_Static_assert(PL_TR_CATCH_OFF == 112 && PL_TR_OVF_BALL_OFF == 120 && PL_TR_FLOOR_OFF + 8 <= PL_TR_CATCH_OFF && PL_TR_OVF_BALL_OFF + 8 <= PL_TR_ALARM_DUE_OFF,
    "the catch word (the innermost catch record, a frame address, never a heap pointer; CTO-508) and the prebuilt overflow ball (a heap pointer the trail root visits): bb_bound.cpp, bb_gate.cpp and "
    "rt_stack_overflow.c read them");
_Static_assert(PL_TR_WSLOT_OFF != PL_TR_BALL_OFF && PL_TR_WSLOT_OFF >= 8 && PL_TR_HEADER_BYTES >= PL_TR_WSLOT_OFF + 8,
    "the writer's slot-vector word must fit inside the trail header, apart from the top word and the ball (the cfo's condition c)");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_tr_gc_root_ball(const char *base) {
    extern void rt_gc_visit_raw(const char **loc);
    void **slot = pl_tr_ball_slot(base);
    if (*slot) rt_gc_visit_raw((const char **)slot);
    { void **ws = pl_tr_wslot_slot(base); if (*ws) rt_gc_visit_raw((const char **)ws); }
    { void **ob = (void **)((char *)base + PL_TR_OVF_BALL_OFF); if (*ob) rt_gc_visit_raw((const char **)ob); }
    { extern void rt_gc_visit_descr(DESCR_t *); rt_gc_visit_descr((DESCR_t *)((char *)base + PL_TR_UNDO_LIST_OFF)); rt_gc_visit_descr((DESCR_t *)((char *)base + PL_TR_UNDO_PENDING_OFF)); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_tr_refuse, leaf, 0);
void rt_pl_tr_refuse(const char *tr) {
    unsigned long used = (unsigned long)((uintptr_t)tr & (PL_TR_ARENA_BYTES - 1)) / PL_TR_ENTRY_BYTES;
    fprintf(stderr, "scrip: prolog: trail arena exhausted after %lu conditional bindings (PL_TR_ARENA_LG2=%d, %lu MB) -- REFUSE rc=2, not a wrong answer\n", used, (int)PL_TR_ARENA_LG2,
        (unsigned long)(PL_TR_ARENA_BYTES >> 20));
    exit(2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_tr_gc_sync(const char *tr) { *(const char **)pl_tr_base_of(tr) = tr; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_tr_freeze(void) {
    const char *top = g_runtime.abi.hp_fr.top;
    for (long i = 0; i < g_runtime.gc.gc_rrng_n; i++) {
        const struct gc_rng_t *r = &g_runtime.gc.gc_rrng[i];
        const char *etop;
        if (r->hi || !r->lo) continue;
        etop = *(const char * const *)r->lo;
        for (const char *p = r->lo + PL_TR_HEADER_BYTES; p + PL_TR_ENTRY_BYTES <= etop; p += PL_TR_ENTRY_BYTES) {
            pl_tr_entry_t *e = (pl_tr_entry_t *)(uintptr_t)p;
            if (!e->cell && e->pad && (const char *)(uintptr_t)e->pad < top) e->pad = (uint64_t)(uintptr_t)top;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_tr_undo_prestep(const char *tr) {
    char *base = pl_tr_base_of(tr);
    DESCR_t *pend = (DESCR_t *)(base + PL_TR_UNDO_PENDING_OFF);
    if (!*(uint64_t *)pend) *pend = *(DESCR_t *)(base + PL_TR_UNDO_LIST_OFF);
}
char *rt_pl_tr_unwind_to(char *tr, char *mark) {
    if (tr > mark) pl_tr_undo_prestep(tr);
    while (tr > mark) { tr -= PL_TR_ENTRY_BYTES; { pl_tr_entry_t *e = (pl_tr_entry_t *)tr; if (e->cell) *e->cell = e->old; } }
    return tr;
}
char *pl_tr_base_of(const char *tr) { return (char *)((uintptr_t)tr & ~(PL_TR_ARENA_BYTES - 1)); }
void **pl_tr_ball_slot(const char *base) { return (void **)((char *)base + PL_TR_BALL_OFF); }
void **pl_tr_wslot_slot(const char *base) { return (void **)((char *)base + PL_TR_WSLOT_OFF); }
uint64_t *pl_tr_ocheck_slot(const char *base) { return (uint64_t *)((char *)base + PL_TR_OCHECK_OFF); }
char **pl_tr_cleanup_slot(const char *base) { return (char **)((char *)base + PL_TR_CLEANUP_OFF); }
uint64_t *pl_tr_probe_b_slot(const char *base) { return (uint64_t *)((char *)base + PL_TR_PROBE_B_OFF); }
uint64_t *pl_tr_wake_slot(const char *base) { return (uint64_t *)((char *)base + PL_TR_WAKE_OFF); }
uint64_t *pl_tr_residue_slot(const char *base) { return (uint64_t *)((char *)base + PL_TR_RESIDUE_OFF); }
char **pl_tr_floor_slot(const char *base) { return (char **)((char *)base + PL_TR_FLOOR_OFF); }
int pl_attv_is(const DESCR_t *c) { return c->v == (DTYPE_t)DT_PLVAR && c->p == (const void *)c && (c->src_node0 & 1); }
uint64_t pl_var_word0(const DESCR_t *c) { uint64_t w; __builtin_memcpy(&w, c, 8); return w; }
uint64_t pl_var_serial(const DESCR_t *c) { return (int)c->v == DT_PLVAR ? pl_var_word0(c) >> PL_SERIAL_SHIFT : 0; }
uint64_t pl_serial_draw(const pl_tr_ctx_t *cx) {
    uint64_t *k;
    if (!cx || !cx->r14) return (uint64_t)DT_PLVAR;
    k = (uint64_t *)(uintptr_t)(cx->r14 - PL_SERIAL_OFF);
    { uint64_t w = *k; if ((w & 255) != (uint64_t)DT_PLVAR) return (uint64_t)DT_PLVAR; *k = w + PL_SERIAL_STEP; return w; }
}
void pl_var_make(DESCR_t *c, const pl_tr_ctx_t *cx, int attv) { uint64_t w = pl_serial_draw(cx) | (attv ? PL_VAR_ATTV : 0); __builtin_memcpy(c, &w, 8); c->p = (void *)c; }
void pl_var_move(DESCR_t *j, const DESCR_t *old, const pl_tr_ctx_t *cx) {
    if ((int)old->v != DT_PLVAR) { pl_var_make(j, cx, 0); return; }
    { uint64_t w = pl_var_word0(old) & ~(uint64_t)PL_VAR_ATTV; __builtin_memcpy(j, &w, 8); j->p = (void *)j; }
}
uint64_t pl_tr_ocheck_mode(const pl_tr_ctx_t *cx) { return (cx && cx->tr) ? *pl_tr_ocheck_slot(pl_tr_base_of(cx->tr)) : 0; }
uintptr_t pl_tr_frame_hi(const char *b) { return *(const uintptr_t *)(b + PL_TR_FRAME_HI_OFF) >> 8; }
void pl_tr_frame_hi_set(char *b, const void *hi) { *(uintptr_t *)(b + PL_TR_FRAME_HI_OFF) = ((uintptr_t)hi << 8) | DT_RAW; }
int pl_tr_needs_log(const pl_tr_ctx_t *cx, const DESCR_t *cell, const char *floor_) {
    if (!cx || !cx->b) return 0;
    if ((const char *)cell <= floor_) return 1;
    return (uintptr_t)cell >= pl_tr_frame_hi(cx->b);
}
const char *pl_tr_hb_of(const pl_tr_ctx_t *cx) {
    const char *mark = *(const char * const *)cx->b;
    if (mark < pl_tr_base_of(cx->tr) + PL_TR_HEADER_BYTES + PL_TR_ENTRY_BYTES || mark > cx->tr) return 0;
    { const pl_tr_entry_t *e = (const pl_tr_entry_t *)(mark - PL_TR_ENTRY_BYTES); return e->cell ? 0 : (const char *)(uintptr_t)e->pad; }
}
int pl_tr_needs_log_bind(const pl_tr_ctx_t *cx, const DESCR_t *cell, const char *floor_) {
    if (!cx || !cx->b) return 0;
    if ((const char *)cell > floor_) return (uintptr_t)cell >= pl_tr_frame_hi(cx->b);
    { const char *hb = pl_tr_hb_of(cx); return !hb || (const char *)cell < hb || (const char *)cell >= *(char * const *)(const void *)&g_runtime.abi.hp_fr; }
}
void pl_tr_push(pl_tr_ctx_t *cx, DESCR_t *cell) {
    if (((uintptr_t)cx->tr & (PL_TR_ARENA_BYTES - 1)) >= PL_TR_ARENA_BYTES - PL_TR_ENTRY_BYTES) rt_pl_tr_refuse(cx->tr);
    { pl_tr_entry_t *e = (pl_tr_entry_t *)cx->tr; e->cell = cell; e->pad = 0; e->old = *cell; cx->tr += PL_TR_ENTRY_BYTES; *(char **)pl_tr_base_of(cx->tr) = cx->tr; }
}
int pl_tr_residue_on(const pl_tr_ctx_t *cx) { return cx && cx->tr && *pl_tr_residue_slot(pl_tr_base_of(cx->tr)) != 0; }
void pl_tr_push_residue(pl_tr_ctx_t *cx, DESCR_t *cell) { pl_tr_push(cx, cell); ((pl_tr_entry_t *)(cx->tr - PL_TR_ENTRY_BYTES))->pad = PL_TR_PAD_RESIDUE; }
void pl_tr_push_keep(pl_tr_ctx_t *cx, DESCR_t *cell) { pl_tr_push(cx, cell); ((pl_tr_entry_t *)(cx->tr - PL_TR_ENTRY_BYTES))->pad = PL_TR_PAD_KEEP; }
void pl_tr_push_attv(pl_tr_ctx_t *cx, DESCR_t *cell) {
    char *base = pl_tr_base_of(cx->tr);
    uint64_t off = (uint64_t)(cx->tr - base);
    uint64_t *w = pl_tr_wake_slot(base);
    if (!*w || *w > off) *w = off;
    pl_tr_push(cx, cell);
}
