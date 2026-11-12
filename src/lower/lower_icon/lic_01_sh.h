#ifndef LOWER_LOWER_ICON_LIC_01_SH_H
#define LOWER_LOWER_ICON_LIC_01_SH_H
#include "lower/lower_icon/lic_head.h"
__attribute__((visibility("hidden"))) icn_loop_fr * icn_loop_at(const icx_t * cx, int idx);
__attribute__((visibility("hidden"))) IR_t * icn_scan_at(const icx_t * cx, int k);
__attribute__((visibility("hidden"))) int icn_is_local(const icx_t * cx, const char * nm);
__attribute__((visibility("hidden"))) const char * icn_variable_lit(const icx_t * cx, const tree_t * t);
__attribute__((visibility("hidden"))) int icn_is_own_global(const icx_t * cx, const char * nm);
__attribute__((visibility("hidden"))) int icn_operand_derefs_late(const icx_t * cx, const tree_t * t);
__attribute__((visibility("hidden"))) int icn_tree_is_kw_var(const tree_t * t);
__attribute__((visibility("hidden"))) int icn_tree_is_kw_scan(const tree_t * t);
__attribute__((visibility("hidden"))) int icn_scan_window_name(const char * nm);
__attribute__((visibility("hidden"))) int icn_is_proc(const char * nm);
__attribute__((visibility("hidden"))) int icn_call_yields_a_variable(icx_t * cx, const char * nm);
__attribute__((visibility("hidden"))) void icn_collect_implicit_locals(const tree_t * n, const char ** excl, int nexcl, lc_vec * out, int unicon);
__attribute__((visibility("hidden"))) tree_t * icn_variable_lit_target(const icx_t * cx, const char * vn);
__attribute__((visibility("hidden"))) tree_t * icn_variable_case(icx_t * cx, const tree_t * t);
__attribute__((visibility("hidden"))) int icn_name_assigned(const char * nm);
__attribute__((visibility("hidden"))) int icn_proc_reassigned(const char * nm);
__attribute__((visibility("hidden"))) char * icn_unicon_row(const char * nm);
__attribute__((visibility("hidden"))) IR_t * icn_augop_build(icx_t * cx, int aop, IR_t * asn, IR_t * ω, IR_t ** op, IR_t ** ca, IR_t ** cb);
__attribute__((visibility("hidden"))) void icn_augop_operands(int aop, IR_t * op, IR_t * ca, IR_t * cb, IR_t * lval, IR_t * rval);
__attribute__((visibility("hidden"))) int is_unop_tt(tree_e tt);
__attribute__((visibility("hidden"))) int icn_tree_calls_a_generator(const tree_t * t);
__attribute__((visibility("hidden"))) int is_resumable(const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * lic_lower(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_lvalue_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res);
__attribute__((visibility("hidden"))) IR_t * lic_lower_if(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_case(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_alt(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int icn_trace_operand_has_var(const icx_t * cx, const tree_t * t);
__attribute__((visibility("hidden"))) IR_t * lower_trace_operand(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_result_operand(icx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) int icn_callee_is_name(const icx_t * cx, const tree_t * fn);
__attribute__((visibility("hidden"))) int icn_arg_stages(const icx_t * cx, const tree_t * a);
__attribute__((visibility("hidden"))) IR_t * lic_lower_call(icx_t * cx, const char * name, const tree_t * t, int argbase, int nargs, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_idx_var(icx_t * cx, const tree_t * t, IR_t * ω, IR_t ** var_res);
__attribute__((visibility("hidden"))) IR_t * lower_idx_base(icx_t * cx, const tree_t * b0, IR_t * ω, IR_t ** br);
__attribute__((visibility("hidden"))) tree_t * pm_imm_commit(void);
#endif
