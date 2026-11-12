#ifndef LOWER_LOWER_RAKU_LRK_02_RK_H
#define LOWER_LOWER_RAKU_LRK_02_RK_H
#include "lower/lower_raku/lrk_head.h"
#include "ir/stage2.h"
__attribute__((visibility("hidden"))) int rk_is_grammar_name(const char * nm);
__attribute__((visibility("hidden"))) const char * rk_qualified_type_gist(const char * nm);
__attribute__((visibility("hidden"))) int rk_is_version_lit(const char * s);
__attribute__((visibility("hidden"))) int rk_is_modeled_type(const char * ty);
__attribute__((visibility("hidden"))) int rk_method_is_stub(const tree_t * m);
__attribute__((visibility("hidden"))) const char * rk_fld_bare(const char * s);
__attribute__((visibility("hidden"))) void lrk_γ_to(IR_t * nd, IR_t * t);
__attribute__((visibility("hidden"))) void lrk_ω_to(IR_t * nd, IR_t * t);
__attribute__((visibility("hidden"))) void rk_loop_open(rcx_t * cx, rk_loopfr_t * lf, IR_t * exit, IR_t * next);
__attribute__((visibility("hidden"))) void rk_loop_close(rcx_t * cx, rk_loopfr_t * lf, IR_t * body);
__attribute__((visibility("hidden"))) int rk_is_binop(tree_e tt);
__attribute__((visibility("hidden"))) int64_t rk_binop_code(tree_e tt);
__attribute__((visibility("hidden"))) void rk_relop_code(IR_t * op, tree_e tt);
__attribute__((visibility("hidden"))) int rk_io_lower_name(const char * nm);
__attribute__((visibility("hidden"))) int rk_proc_known(const char * name);
__attribute__((visibility("hidden"))) int rk_yields_list(const tree_t * t);
__attribute__((visibility("hidden"))) int rk_yields_array(const tree_t * t);
__attribute__((visibility("hidden"))) int rk_is_relop(tree_e tt);
__attribute__((visibility("hidden"))) int rk_proc_known(const char * name);
__attribute__((visibility("hidden"))) int rk_proc_rest_kind(const char * name);
__attribute__((visibility("hidden"))) IR_t * lower_cond(rcx_t * cx, const tree_t * c, IR_t * on_true, IR_t * on_false);
__attribute__((visibility("hidden"))) IR_t * rk_lower_logical(rcx_t * cx, const tree_t * t, int is_or, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * lower_field_set(rcx_t * cx, const tree_t * lhs, const tree_t * rhs, IR_t * γ, IR_t * ω, IR_t ** res);
__attribute__((visibility("hidden"))) IR_t * rk_xf_body(rcx_t * cx, const tree_t * xf, int xfk, const char * vname, IR_t * valprod, const tree_t * body, IR_t * pump, IR_t * ω);
__attribute__((visibility("hidden"))) void rk_reclassify_calls(void);
__attribute__((visibility("hidden"))) void rk_discover_grammars(const tree_t * prog);
__attribute__((visibility("hidden"))) void rk_register_classes(const tree_t * prog);
__attribute__((visibility("hidden"))) void rk_discover_procs(const tree_t * prog);
#endif
