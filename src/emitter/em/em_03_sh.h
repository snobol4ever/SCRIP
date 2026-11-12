#ifndef EMITTER_EM_EM_03_SH_H
#define EMITTER_EM_EM_03_SH_H
#include "emitter/em/em_head.h"
extern "C" { extern g_emitter_t g_emitter; }
extern "C" { extern bb_emit_mode_t bb_emit_mode; }
extern "C" { extern FILE *bb_emit_out; }
extern "C" { extern bb_medium_t g_medium; }
extern "C" { extern sm_emit_t g_emit; }
extern "C" { __attribute__((visibility("hidden"))) void emit_stno_mark(int32_t stno); }
extern "C" { __attribute__((visibility("hidden"))) const char * flat_label_kind(IR_e op); }
extern "C" { __attribute__((visibility("hidden"))) const char * flat_label_kind_of(IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) void ef_text_jmp(const char *mnem, const char *target); }
extern "C" { void emit_label_define_bb(bb_label_t *lbl); }
extern "C" { void emit_label_initf(bb_label_t *lbl, const char *fmt, ...); }
extern "C" { __attribute__((visibility("hidden"))) void emit_stno_row(int32_t stno, int32_t line, const char * file); }
extern "C" { int bb_slot_get(IR_t *nd); }
extern "C" { __attribute__((visibility("hidden"))) void emit_ops_reserve(int n); }
extern "C" { __attribute__((visibility("hidden"))) void bb_slotmap_push(IR_t *nd, int off); }
extern "C" { __attribute__((visibility("hidden"))) int emit_fb_stmt_scan(IR_graph_t * g); }
extern "C" { int emit_binop_opnd_slot(IR_t *o, int own, int vs); }
extern "C" { __attribute__((visibility("hidden"))) void call_box_emit(std::string (*box)(void)); }
extern "C" { int walk_bb_node(IR_t * nd, FILE * out); }
extern "C" { __attribute__((visibility("hidden"))) void emit_sep_rule(char ch); }
extern "C" { extern int bb_slot_get(IR_t *nd); }
extern "C" { extern IR_graph_t * g_emit_cfg; }
extern "C" { __attribute__((visibility("hidden"))) void flat_drive_repalt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **ra_y, bb_label_t **ra_t, bb_label_t *node_γ, bb_label_t *node_ω); }
extern "C" { __attribute__((visibility("hidden"))) std::string pl_clause_zero(int ak); }
extern "C" { __attribute__((visibility("hidden"))) std::string pl_step_reseed_locals(void); }
extern "C" { __attribute__((visibility("hidden"))) void drive_guard_refused(IR_t *nd, int line); }
extern "C" { __attribute__((visibility("hidden"))) int emit_graph_has_deep_arrival(IR_graph_t *g); }
extern "C" { __attribute__((visibility("hidden"))) int resume_sealed_defer_on_root(IR_t ** nodes, int n, IR_t * br); }
extern "C" { __attribute__((visibility("hidden"))) int resume_carrier_ok(const IR_t * nd, int sealed_defer); }
extern "C" { int blob_carve_bytes(void); }
extern "C" { __attribute__((visibility("hidden"))) void blob_layout_build(void); }
extern "C" { __attribute__((visibility("hidden"))) void icn_register_local_offsets(const char * pname); }
extern "C" { __attribute__((visibility("hidden"))) const char * gc_map_entry_fam(const char * fam); }
std::string emit_gc_map_register(int value_bytes, int frame_bytes, int header_bytes, unsigned flags, int frame_rel, uint64_t link);
extern "C" { __attribute__((visibility("hidden"))) void emit_gc_map_data(const char * fam); }
extern "C" { extern int g_flat_dc_np; }
extern "C" { void emit_set_sink(FILE * out); }
extern "C" { long emit_text_count(void); }
extern "C" { void emit_text_n(const char * s, size_t n); }
extern "C" { void emit_textf(const char * fmt, ...); }
extern "C" { void kw_snobol4_prepare(void); }
extern "C" { void unify_prepare(IR_t *nd); }
__attribute__((visibility("hidden"))) void call_marg_plan(int i, IR_t * lf, int aoff, IR_t * owner);
#endif
