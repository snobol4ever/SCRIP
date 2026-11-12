#ifndef EMITTER_EMIT_REGIME_H
#define EMITTER_EMIT_REGIME_H
#include <stdint.h>
#include "ir/bb_box.h"
#include "emitter/emit_state.h"
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int emit_jmp_pin_legacy(void) { return g_emit.flat_deep_arrival || g_emit.flat_gen || g_emit.flat_lcl_proc || g_emit.zframe_graph; }
extern int g_gen_proc_active;
extern int g_resumable_callable_active;
static inline int emit_heap_fb_adopt(void) { return g_gen_proc_active || g_resumable_callable_active; }
static inline int emit_rec_pin(void) { return emit_jmp_pin_legacy() || emit_heap_fb_adopt(); }
static inline const char * emit_pl_fb(void) { return "rbp"; }
int cap_fail_retreat(void);
int emit_resumable_frame(void);
#define TEMPLATE_FN_ADDR(sym) ((uint64_t)(uintptr_t)(void *)(sym))
#define TEMPLATE_ADDR_SIGMA ((uint64_t)(uintptr_t)&Σ)
#define TEMPLATE_ADDR_SIGLEN ((uint64_t)(uintptr_t)&Σlen)
#define MATCH_CTX_CELL_BYTES 32
#define MATCH_CTX_CELL_OFF(extra) (-56 - (extra))
int emit_zrow_zchk(const IR_t * nd);
int emit_zrow_xabs(const IR_t * nd);
int emit_port_exit_label_promotes(const char * label);
void emit_zsm_frame_addr(const IR_graph_t * g, const IR_t * nd, int off, int cls, int rsp_form);
int emit_zrow_release(int below, int kind, const int ** offs);
int emit_diag_regs_suppress(void);
long emit_zsm_word(const IR_t * nd, int op_zres, int op_subj_cell, long owner);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
const char * emit_rec_or_empty(void * r);
void emit_diag_arbno_arm(const char * arm);
#endif
#endif
