#ifndef EMIT_DRIVE_H
#define EMIT_DRIVE_H
#include "IR.h"
#include "emit.h"
IR_t * bb_child0(const IR_t *n);
IR_t * bb_child1(const IR_t *n);
int    binop_slot_kind(IR_t *nd);
int    bb_call_write_route(IR_t *nd);
int    descr_binop_opnd_slot(IR_t *o);
int    binop_is_num_real(IR_graph_t *g, IR_t *nd);
void   bb_fill_alpha(IR_t *nd);
void   bb_flat_cursor_reserve(int upto);
int    bb_flat_cursor(void);
void   emit_drive(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
void   emit_jvm_drive(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
void   emit_net_drive(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
void   emit_js_drive(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
void   emit_wasm_drive(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
#endif
