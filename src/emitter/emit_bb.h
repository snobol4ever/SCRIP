#ifndef EMIT_BB_H
#define EMIT_BB_H
#ifdef __cplusplus
extern "C" {
#endif
#include "emit_core.h"
#include "bb_pool.h"
#include "core.h"
#include "bb_box.h"
#include "IR.h"
#include <stdio.h>
bb_box_fn bb_build_flat    (IR_t * nd);
int  bb_call_route_classify(IR_t * nd);
void resolve_call_kinds_descr(IR_graph_t * g);
void resolve_call_kinds_gvar(IR_graph_t * g);
bb_box_fn descr_flat_chain_build(IR_t * entry);
int  descr_flat_chain_build_text(IR_t * entry, FILE * out, const char * prefix);
int  bb_varslot(const char * name);
int  bb_varslot_peek(const char * name);
extern int g_descr_flat_chain;
extern int g_gvar_flat_chain;
int  codegen_flat_build        (IR_t * nd, FILE * out, const char * prefix);
void lower_flat_set_intern_str(const char * (*fn)(const char *));
const char * emit_intern_str(const char * s);
void lower_flat_reset        (void);
void walk_bb_register_child_label(IR_t * nd, const char * alpha_label);
extern int g_flat_node_id;
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
void resolve_call_block_label(char *dst, size_t dsz, const char *name, int arity);
void sub_label(char *dst, size_t dsz, const char *name);
void resolve_choice_clause_label(char *dst, size_t dsz, int id, int ci, const char *suffix);
int resolve_emit_callee_block_body(const char *name, int arity, bb_label_t *bγ, bb_label_t *bω, bb_label_t *bβ);
extern IR_t *resolve_bb_entry_node(const char *name, int arity);
int  bb_kind_is_driver_owned(int t);
void bb_prepare_capture_arbno(IR_t *nd, int imm);
void bb_emit_limit_init(int limit_slot_off);
void bb_emit_repalt_clear(int off);
void bb_emit_repalt_yield(int off, int e_slot);
void bb_emit_repalt_test(int off);
const char * child_cache_get_lbl   (bb_box_fn fn);
extern void (*g_cap_fixup_cb)      (void *cap_ptr, const char *child_alpha_label);
extern char   g_flat_data_buf[];
extern size_t g_flat_data_len;
extern int    g_flat_data_any;
void data_buf_flush_pending_label(void);
void data_buf_reset(void);
static inline bb_label_t bb_label_from_name(const char *name) {
    bb_label_t lbl = { {0}, -1 };
    if (name) { strncpy(lbl.name, name, BB_LABEL_NAME_MAX - 1); lbl.name[BB_LABEL_NAME_MAX - 1] = '\0'; }
    return lbl;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define bb_build_flat_text(p,out,pfx)    codegen_flat_build(p,out,pfx)
#define bb_flat_set_intern_str(fn)        lower_flat_set_intern_str(fn)
#define bb_build_flat_text_reset()        lower_flat_reset()
#ifdef __cplusplus
}
/*--------------------------------------------------------------------------------------------------------------------*/
#endif
#endif
