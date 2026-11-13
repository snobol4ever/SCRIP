#ifndef LOWER_LOWER_RAKU_LRK_06_RK_H
#define LOWER_LOWER_RAKU_LRK_06_RK_H
#include "lower/lower_raku/lrk_head.h"
__attribute__((visibility("hidden"))) int rk_meth_is_bool(const char * m);
__attribute__((visibility("hidden"))) IR_t * lower_rcall_bool(rcx_t * cx, const tree_t * t, const char * nm, int from, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_rcall1(rcx_t * cx, const tree_t * recv, const char * nm, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * rk_lower_str_operand(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_rcall_skip1(rcx_t * cx, const tree_t * t, const char * nm, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) tree_t * rk_divis_desugar(const tree_t * t);
__attribute__((visibility("hidden"))) const tree_t * rk_closure_block(const tree_t * t);
__attribute__((visibility("hidden"))) tree_t * rk_code_call1(const tree_t * prog, const tree_t * code, const tree_t * topic, int * ok);
__attribute__((visibility("hidden"))) tree_t * rk_case_match(const tree_t * subj, const tree_t * cond);
__attribute__((visibility("hidden"))) int rk_user_meth_named(const char * m);
__attribute__((visibility("hidden"))) tree_t * rk_throw_desugar(rcx_t * cx, const tree_t * e);
__attribute__((visibility("hidden"))) IR_t * rk_lower_iter_meth(rcx_t * cx, const tree_t * t, const char * mname, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) void rk_scan_implicit_params(const tree_t * t, int * topic, const char ** ph, int * nph, int max);
__attribute__((visibility("hidden"))) const char * rk_ph_id(const char * v);
__attribute__((visibility("hidden"))) int rk_ph_cmp(const char * a, const char * b);
__attribute__((visibility("hidden"))) void rk_topic_default(tree_t * seq);
__attribute__((visibility("hidden"))) void rk_sub_placeholders(tree_t * t);
__attribute__((visibility("hidden"))) void rk_ph_alias(tree_t * t);
__attribute__((visibility("hidden"))) void rk_subst_code_walk(const tree_t * prog, tree_t * t, int * seq);
__attribute__((visibility("hidden"))) void rk_make_walk(tree_t * t, const char * slash);
__attribute__((visibility("hidden"))) void rk_nested_sub_names(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_amp_methcalls(const tree_t * prog, tree_t * t);
__attribute__((visibility("hidden"))) void rk_bool_regexes(tree_t * t);
__attribute__((visibility("hidden"))) void rk_relaxed_collect(const tree_t * t, rk_ns_t * out);
__attribute__((visibility("hidden"))) void rk_relaxed_walk(tree_t * t, const rk_ns_t * set);
#endif
