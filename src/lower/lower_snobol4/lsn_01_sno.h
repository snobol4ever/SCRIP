#ifndef LOWER_LOWER_SNOBOL4_LSN_01_SNO_H
#define LOWER_LOWER_SNOBOL4_LSN_01_SNO_H
#include "lower/lower_snobol4/lsn_head.h"
#include "ir/stage2.h"
__attribute__((visibility("hidden"))) int sno_sub_val_on(void);
__attribute__((visibility("hidden"))) int sno_kw_static_slot(const char * kw);
__attribute__((visibility("hidden"))) int sno_binop_code(tree_e tt);
__attribute__((visibility("hidden"))) int sno_pe_patfn(tree_e k);
int sno_name_wrappable(const tree_t * e);
__attribute__((visibility("hidden"))) IR_t * sx_subscript_lv(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res);
__attribute__((visibility("hidden"))) IR_t * sx_subscript_lv_fused(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res, IR_t ** fuse_base, IR_t ** fuse_idx);
__attribute__((visibility("hidden"))) IR_t * sx_call_named(scx_t * cx, const char * name, const tree_t * t, int argbase, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * sno_lower_byname(scx_t * cx, const tree_t * expr, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int sno_item_argbase(const tree_t * fn);
__attribute__((visibility("hidden"))) const char * sno_capt_item_name(const tree_t * tgt);
__attribute__((visibility("hidden"))) IR_t * sx_nameval(scx_t * cx, const tree_t * inner, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int sno_capt_elem_tg(const tree_t * tg, int curs);
__attribute__((visibility("hidden"))) int sno_elem_here(const scx_t * cx, const tree_t * tg, int curs);
__attribute__((visibility("hidden"))) IR_t * sx_assign_body(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_graph_t * sno_build_call_stub(const char * entry_label, const char * fname);
#endif
