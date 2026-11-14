#ifndef EMITTER_EMIT_H
#define EMITTER_EMIT_H
#include <stdio.h>
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "ir/bb_box_fn.h"
#include "emitter/bb_label.h"
#ifdef __cplusplus
extern "C" {
#endif
#define ZD_NOPS_MIN 64
extern void (*g_emit_chain_posthook)(void);
extern int g_use_sm_macros;
extern int g_use_bb_macros;
void drive_arg_slots_reserve (int n);
int walk_bb_node(struct IR_t * nd, FILE * out);
void kw_snobol4_prepare(void);
const char * kw_bare(const char * s);
typedef struct {
    void * graph_key;
    int base;
    int arity;
    int nlocals;
    int mark_slot;
    IR_t * body_head;
    IR_t * frame_node;
    void * lblA;
    void * lblB;
    int nchild;
    int nclauses;
    IR_t ** clause_head;
    int body_emitted;
} pl_gz_callee_t;
typedef struct { pl_gz_callee_t ** v; int n; int cap; } pl_gz_callee_vec_t;
int bb_call_route_classify(IR_t * nd);
struct zeta_plan_t;
bb_box_fn emit_chain(struct zeta_plan_t * rec, FILE * out, const char * prefix);
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
int bb_kind_is_driver_owned(int t);
void bb_prepare_capture_arbno(IR_t *nd, int imm);
void bb_emit_limit_init(int limit_slot_off);
const char * child_cache_get_lbl (bb_box_fn fn);
extern void (*g_cap_fixup_cb) (void *cap_ptr, const char *child_alpha_label);
IR_t * bb_child0(const IR_t *n);
IR_t * bb_child1(const IR_t *n);
int binop_slot_kind(IR_t *nd);
int emit_binop_opnd_slot(IR_t *o, int own, int vs);
int binop_is_num_real(IR_graph_t *g, IR_t *nd);
void bb_fill_alpha(IR_t *nd);
void emit_drive(IR_t *nd, bb_label_t *lbl_α, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
int emit_graph_has_suspend(IR_graph_t * g);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
void defer_prepare(void);
#endif
#endif
