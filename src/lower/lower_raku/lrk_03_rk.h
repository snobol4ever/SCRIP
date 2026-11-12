#ifndef LOWER_LOWER_RAKU_LRK_03_RK_H
#define LOWER_LOWER_RAKU_LRK_03_RK_H
#include "lower/lower_raku/lrk_head.h"
#include "ir/stage2.h"
__attribute__((visibility("hidden"))) int rk_is_class_name(const char * nm);
__attribute__((visibility("hidden"))) tree_t * leaf_sval2(tree_e kind, const char * s);
__attribute__((visibility("hidden"))) tree_t * rk_coerce_val(tree_t * val, const char * base);
__attribute__((visibility("hidden"))) int rk_user_proc_exists(const char * nm);
extern __attribute__((visibility("hidden"))) const char * const rk_shadowable_terms[8];
__attribute__((visibility("hidden"))) int rk_certainly_undeclared(const char * nm);
__attribute__((visibility("hidden"))) int rk_predeclared(const char * nm, int bare);
__attribute__((visibility("hidden"))) tree_t * rk_fnc2(const char * nm, const tree_t * a, const tree_t * b);
__attribute__((visibility("hidden"))) tree_t * rk_typeobj_tree(const char * ty);
__attribute__((visibility("hidden"))) const char * rk_decl_typeobj_name(const char * ty);
__attribute__((visibility("hidden"))) const tree_t * rk_assign_rhs(const char * vn, const tree_t * rhs);
__attribute__((visibility("hidden"))) tree_t * rk_fnc2(const char * nm, const tree_t * a, const tree_t * b);
__attribute__((visibility("hidden"))) const tree_t * rk_block_decl(const tree_t * prog, const char * nm);
__attribute__((visibility("hidden"))) const tree_t * rk_numctx(tree_e k, const tree_t * x, const tree_t * o);
__attribute__((visibility("hidden"))) int rk_xx_value_is_pure(const tree_t * e);
__attribute__((visibility("hidden"))) void rk_tv_walk(tree_t * t, rk_tvenv_t * e);
__attribute__((visibility("hidden"))) tree_t * rk_ss_arg(int j);
__attribute__((visibility("hidden"))) const char * rk_prologue_target(const tree_t * s);
__attribute__((visibility("hidden"))) stage2_t *rk_stage2_core(const tree_t *prog, int reset_multi, int want_main);
#endif
