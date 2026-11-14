#ifndef LOWER_LOWER_RAKU_LRK_08_RK_H
#define LOWER_LOWER_RAKU_LRK_08_RK_H
#include "lower/lower_raku/lrk_head.h"
#include "ir/stage2.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) IR_t * rk_tern_branch(rcx_t * cx, const tree_t * br, IR_t * at, IR_t * ω);
__attribute__((visibility("hidden"))) tree_t * rk_state_desugar(const tree_t * t);
__attribute__((visibility("hidden"))) int rk_diverges(const tree_t * t);
__attribute__((visibility("hidden"))) void rk_file_scope_reads_are_globals(void);
__attribute__((visibility("hidden"))) void rk_unassigned_dynamics_are_globals(void);
__attribute__((visibility("hidden"))) void rk_unfound_dynamics(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_eval_main_globals(void);
void lower_raku_eval_reads_are_globals(int pc0);
__attribute__((visibility("hidden"))) void raku_register_program(stage2_t * s2, const tree_t * prog);
__attribute__((visibility("hidden"))) void rk_pkg_scope(tree_t * t, const char * path);
__attribute__((visibility("hidden"))) void rk_pkg_refs(tree_t * t);
__attribute__((visibility("hidden"))) void rk_class_stubs(const tree_t * prog, tree_t * t);
__attribute__((visibility("hidden"))) void rk_class_body(tree_t * t, int * seq);
__attribute__((visibility("hidden"))) void rk_qualified_sub_refs(const tree_t * prog, tree_t * t);
__attribute__((visibility("hidden"))) void rk_trans_flags_walk(tree_t * t);
__attribute__((visibility("hidden"))) void rk_subst_meth_walk(tree_t * t);
__attribute__((visibility("hidden"))) void rk_hoist_nested_types(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_role_specialize(tree_t * prog);
#endif
