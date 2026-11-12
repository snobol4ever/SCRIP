#ifndef LOWER_LOWER_RAKU_LRK_05_RK_H
#define LOWER_LOWER_RAKU_LRK_05_RK_H
#include "lower/lower_raku/lrk_head.h"
__attribute__((visibility("hidden"))) int rk_destr_count(const char * pn);
__attribute__((visibility("hidden"))) int rk_destr_plan(const char * pn, const tree_t ** plan, int nplan);
__attribute__((visibility("hidden"))) int rk_subsig_count(const tree_t * pv);
__attribute__((visibility("hidden"))) int rk_subsig_plan(const tree_t * pv, const char * src, const tree_t ** plan, int nplan);
__attribute__((visibility("hidden"))) const char * rk_coerce_base(const char * ty);
__attribute__((visibility("hidden"))) int rk_is_loop_node(const tree_t * t);
__attribute__((visibility("hidden"))) int rk_is_multi_for(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * rk_loop_ctl(rcx_t * cx, const tree_t * t, const char * nm, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int rk_is_str_subform(const char * nm);
__attribute__((visibility("hidden"))) int rk_rx_core(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * rk_lower_smatch_boxes(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int rk_rx_boxes_on(void);
__attribute__((visibility("hidden"))) void rk_refuse(const tree_t * t, const char * what);
__attribute__((visibility("hidden"))) void rk_listops_to_methcalls(tree_t * t, int is_iter_src);
__attribute__((visibility("hidden"))) void rk_attr_aliases(tree_t * t);
__attribute__((visibility("hidden"))) void rk_name_anon_subsig(tree_t * t, int * ctr);
__attribute__((visibility("hidden"))) void rk_shd_subsig(const tree_t * pv, rk_ns_t * out);
__attribute__((visibility("hidden"))) void rk_subsig_blocks(tree_t * t, int * ctr);
__attribute__((visibility("hidden"))) void rk_pl_destructure(tree_t * t, int * uid);
#endif
