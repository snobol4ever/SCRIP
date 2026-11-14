#ifndef LOWER_LOWER_RAKU_LRK_07_RK_H
#define LOWER_LOWER_RAKU_LRK_07_RK_H
#include "lower/lower_raku/lrk_head.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) int rk_proc_slurpy_only(const char * name, int nref);
__attribute__((visibility("hidden"))) int rk_ns_has(const rk_ns_t * s, const char * x);
__attribute__((visibility("hidden"))) void rk_ns_add(rk_ns_t * s, const char * x);
__attribute__((visibility("hidden"))) int rk_tree_size(const tree_t * t);
__attribute__((visibility("hidden"))) void rk_cap_declared(const tree_t * t, rk_ns_t * out);
__attribute__((visibility("hidden"))) void rk_cap_file_scope(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_gw_rename(tree_t * t, const char * v, const char * to);
__attribute__((visibility("hidden"))) void rk_globalize_file_scope_writes(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_hoist_nested_multis(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_ss_collect(const tree_t * t, rk_ns_t * out);
__attribute__((visibility("hidden"))) void rk_tv_classes(const tree_t * t, rk_ns_t * out);
__attribute__((visibility("hidden"))) void rk_typed_vars(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_shadow_rename(tree_t * prog);
#endif
