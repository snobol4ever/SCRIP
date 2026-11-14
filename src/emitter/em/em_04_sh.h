#ifndef EMITTER_EM_EM_04_SH_H
#define EMITTER_EM_EM_04_SH_H
#include "emitter/em/em_head.h"
#include "emitter/emit_medium.h"
#include "emitter/emit_regime.h"
#include "ir/ir_graph.h"
extern "C" {
#include "planner/zeta_plan.h"
}
extern "C" { FILE *emit_outf(void); }
extern "C" { extern bb_buf_t bb_emit_buf; }
extern "C" { extern int bb_emit_pos; }
extern "C" { extern int bb_patch_count; }
extern "C" { __attribute__((visibility("hidden"))) int zrow_of(const IR_t * nd); }
extern "C" { const znode_t * zrow(const IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) int zrow_opoff(const IR_t * nd, int k); }
extern "C" { __attribute__((visibility("hidden"))) int zrow_opvs(const IR_t * nd, int k); }
extern "C" { extern int g_gen_proc_active; }
extern "C" { extern int g_resumable_callable_active; }
extern "C" { IR_t * bb_child0(const IR_t *n); }
extern "C" { IR_t * bb_child1(const IR_t *n); }
extern "C" { int blob_frame_scope(void); }
extern "C" { int binop_slot_kind(IR_t *nd); }
extern "C" { int binop_is_num_real(IR_graph_t *g, IR_t *nd); }
extern "C" { __attribute__((visibility("hidden"))) int walk_bb_node_inner(IR_t * nd, FILE * out); }
extern "C" { __attribute__((visibility("hidden"))) int drive_value_slot(IR_t *nd); }
extern "C" { extern void bb_slot_register(IR_t *nd, int off); }
extern "C" { extern int g_gva_active; }
extern "C" { __attribute__((visibility("hidden"))) void flat_drive_gate(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t *node_γ, bb_label_t *node_ω, const unsigned char *zon, const int *zout, const int *xb); }
extern "C" { __attribute__((visibility("hidden"))) void flat_drive_match_alt(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t **na_s, bb_label_t **na_f, bb_label_t **na_fo, bb_label_t ***fc_sig, bb_label_t *node_γ, bb_label_t *node_ω, bb_label_t *chain_ω); }
extern "C" { int blob_frame_scope(void); }
extern "C" { int sn4_alt_carrier(void); }
extern "C" { void sn4_blob_choice_scan(int * n_choice, int * leaf_ok, int * has_fence); }
extern "C" { int sn4_blob_casmark(void); }
extern "C" { int sn4_defer_resume(void); }
extern "C" { __attribute__((visibility("hidden"))) int blob_carve_pad(void); }
extern "C" { int blob_frame_bytes(void); }
extern "C" { __attribute__((visibility("hidden"))) void emit_chain_prepare(IR_t * entry); }
int bcfn_result_slot(IR_t * nd);
#endif
