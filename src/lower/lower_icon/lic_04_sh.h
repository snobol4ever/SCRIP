#ifndef LOWER_LOWER_ICON_LIC_04_SH_H
#define LOWER_LOWER_ICON_LIC_04_SH_H
#include "lower/lower_icon/lic_head.h"
#include "ir/ast.h"
#include "ir/ir_graph.h"
__attribute__((visibility("hidden"))) IR_t * icn_arm_result(IR_t * rv);
__attribute__((visibility("hidden"))) int icn_gen_wiring(const IR_t * t);
__attribute__((visibility("hidden"))) void lic_γ_to(IR_t * nd, IR_t * t);
__attribute__((visibility("hidden"))) void lic_ω_to(IR_t * nd, IR_t * t);
__attribute__((visibility("hidden"))) int icn_tree_is_cursor_mover(const tree_t * a);
__attribute__((visibility("hidden"))) IR_t * lower_if_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv);
__attribute__((visibility("hidden"))) IR_t * lower_case_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv);
__attribute__((visibility("hidden"))) IR_t * lower_seq_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv);
__attribute__((visibility("hidden"))) IR_t * lower_limit_impl(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res, int lv);
__attribute__((visibility("hidden"))) IR_t * lic_lower_while(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_every(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_until(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lic_lower_repeat(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) void icn_retag_scan_body(icx_t * cx, int lo);
__attribute__((visibility("hidden"))) int icn_tree_is_literal(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * lower_runerr_111(icx_t * cx, const tree_t * other, const tree_t * bad, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_scan_impl(icx_t * cx, const tree_t * subj_t, const tree_t * lv_t, const tree_t * body_t, const tree_t * wi_t, const tree_t * wj_t, IR_t * γ, IR_t * ω, IR_t ** res, IR_t ** lv_out);
__attribute__((visibility("hidden"))) IR_t * icn_dj_α_entry(IR_graph_t * g, IR_t * dj);
__attribute__((visibility("hidden"))) int icn_tree_mentions_kw(const tree_t * t, const char * kw);
#endif
