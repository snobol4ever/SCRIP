#ifndef LOWER_LOWER_RAKU_LRK_01_SH_H
#define LOWER_LOWER_RAKU_LRK_01_SH_H
#include "lower/lower_raku/lrk_head.h"
#include "ir/ir_node.h"
__attribute__((visibility("hidden"))) int lrk_rk_seq_is_logical_and(const tree_t * t);
__attribute__((visibility("hidden"))) int rk_is_multi_name(const char * nm);
__attribute__((visibility("hidden"))) void rk_multi_name_add(const char * base);
__attribute__((visibility("hidden"))) int rk_var_byref(rcx_t * cx, const tree_t * v);
__attribute__((visibility("hidden"))) const tree_t * lrk_stmt_subj(const tree_t * s);
__attribute__((visibility("hidden"))) IR_t * lrk_build(rcx_t * cx, IR_e op, IR_t * γ, IR_t * ω);
__attribute__((visibility("hidden"))) IR_t * rk_excise_at(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int trace_wanted(void);
__attribute__((visibility("hidden"))) long trace_stmt_line(const tree_t * s);
__attribute__((visibility("hidden"))) IR_t * trace_stmt_wrap(rcx_t * cx, long line, IR_t * stmt_entry, IR_t * ω);
__attribute__((visibility("hidden"))) int rk_trace_is_block(const char * name);
__attribute__((visibility("hidden"))) const char * rk_trace_name(const char * name);
__attribute__((visibility("hidden"))) IR_t * trace_call_wrap(rcx_t * cx, const char * name, IR_t * body_entry, IR_t * ω);
__attribute__((visibility("hidden"))) IR_t * trace_value_prep(rcx_t * cx, const char * name, IR_t * γ, IR_t * ω, IR_t ** call_out);
__attribute__((visibility("hidden"))) const tree_t * rk_catch_handler(const tree_t * c);
__attribute__((visibility("hidden"))) int rk_empty_body(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * lower_rblock(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** last_res);
__attribute__((visibility("hidden"))) int rk_param_is_rw(const tree_t * pv);
__attribute__((visibility("hidden"))) IR_t * lower_rcall(rcx_t * cx, const tree_t * t, const char * nm, int from, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) tree_t * rk_case_desugar(const tree_t * t);
__attribute__((visibility("hidden"))) int lower_raku_body(const tree_t *prog, const tree_t *proc);
__attribute__((visibility("hidden"))) int rk_block_tail_is_value(const tree_t * s);
__attribute__((visibility("hidden"))) void rk_tail_ifs(tree_t * t, int in_pkg);
__attribute__((visibility("hidden"))) void rk_hoist_anon_blocks(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_rename_user_main(tree_t * prog);
__attribute__((visibility("hidden"))) tree_t * rk_top_subj(tree_t * st);
#endif
