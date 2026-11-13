#ifndef LOWER_LOWER_RAKU_LRK_11_RK_H
#define LOWER_LOWER_RAKU_LRK_11_RK_H
#include "lower/lower_raku/lrk_head.h"
__attribute__((visibility("hidden"))) int rk_is_scalar_bind(const tree_t * t);
__attribute__((visibility("hidden"))) tree_t * rk_clone_tree(const tree_t * t);
__attribute__((visibility("hidden"))) void rk_bind_unbox(tree_t * t, const rk_ns_t * Bx);
__attribute__((visibility("hidden"))) void rk_class_default_tweaks(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_nested_elem_sets(tree_t * t);
__attribute__((visibility("hidden"))) tree_t * rk_ss_call(const char * nm, tree_t * arg);
__attribute__((visibility("hidden"))) void rk_elem_store_values(tree_t * t);
__attribute__((visibility("hidden"))) void rk_bind_refs(tree_t * t, int in_blk);
__attribute__((visibility("hidden"))) void rk_rw_loops(tree_t * t, int * seq);
__attribute__((visibility("hidden"))) void rk_rw_subs(const tree_t * t, rk_rwmap_t * m);
__attribute__((visibility("hidden"))) void rk_rw_calls(tree_t * t, const rk_rwmap_t * m, int in_blk);
__attribute__((visibility("hidden"))) void rk_rw_method_deref(tree_t * t, const rk_rwmap_t * m);
__attribute__((visibility("hidden"))) void rk_rw_method_setters(tree_t * prog);
__attribute__((visibility("hidden"))) int rk_user_sub_named(const tree_t * prog, const char * nm);
__attribute__((visibility("hidden"))) void rk_minmax_fns(tree_t * t, const tree_t * prog, int umax, int umin);
__attribute__((visibility("hidden"))) void rk_ss_rewrite(tree_t * t, const rk_ns_t * ss, int in_pkg);
__attribute__((visibility("hidden"))) void rk_ss_multi(tree_t * t, const rk_ns_t * ss, const rk_ns_t * classes, tree_t * root, int * ctr);
#endif
