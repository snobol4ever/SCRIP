#ifndef EMITTER_EM_EM_01_SH_H
#define EMITTER_EM_EM_01_SH_H
#include "emitter/em/em_head.h"
extern "C" { __attribute__((visibility("hidden"))) bb_label_t * emit_floater_label(int k); }
extern "C" { bb_label_t *emit_label_alloc(const char *fmt, ...); }
extern "C" { bb_label_t *emit_label_intern(const char *name); }
extern "C" { void bb_emit_begin(bb_buf_t buf, int size); }
extern "C" { bb_label_t *bb_label_fold(bb_label_t *l); }
extern "C" { void bb_emit_patch_rel32(bb_label_t *lbl); }
extern "C" { void bb_emit_u32(uint32_t v); }
extern "C" { void bb_emit_u64(uint64_t v); }
extern "C" { extern int g_is_text; }
extern "C" { extern int g_emit_pos; }
extern "C" { int emitter_end(void); }
extern "C" { __attribute__((visibility("hidden"))) void ef_b1 (uint8_t a); }
extern "C" { __attribute__((visibility("hidden"))) void ef_b2 (uint8_t a, uint8_t b); }
extern "C" { __attribute__((visibility("hidden"))) void ef_b3 (uint8_t a, uint8_t b, uint8_t c); }
extern "C" { void emit_jmp_label(bb_label_t *target, jmp_kind_t kind); }
extern "C" { void bb_label_define(bb_label_t *lbl); }
extern "C" { __attribute__((visibility("hidden"))) const char * icn_trace_intern(const char * s); }
extern "C" { void xa_dispatch(XA_op_t op); }
extern "C" { void strtab_label(char *buf, size_t bufsz, const char *s); }
extern "C" { __attribute__((visibility("hidden"))) long flat_buf_max_v(); }
extern "C" { extern int g_flat_node_id; }
extern "C" { void emit_fb_divergence_check(void); }
extern "C" { extern __attribute__((visibility("hidden"))) bb_label_t *g_create_body_entry; }
extern "C" { void data_buf_reset(void); }
extern "C" { void data_buf_flush_pending_label(void); }
extern "C" { int blob_cont_copy(void); }
extern "C" { __attribute__((visibility("hidden"))) int blob_omega_ret(void); }
extern "C" { __attribute__((visibility("hidden"))) std::string blob_zero_fill(int bfb); }
extern "C" { const char * blob_alt_first(const IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) void blob_first_guard(void); }
extern "C" { __attribute__((visibility("hidden"))) std::string flat_zero_fill(int lo, int len); }
extern "C" { __attribute__((visibility("hidden"))) std::string icn_entry_gva(void); }
extern "C" { __attribute__((visibility("hidden"))) std::string icn_trace_tap(const char * pname, int kind, int np, int aoff = 16); }
#endif
