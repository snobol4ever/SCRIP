#ifndef LOWER_LOWER_RAKU_LRK_12_RK_H
#define LOWER_LOWER_RAKU_LRK_12_RK_H
#include "lower/lower_raku/lrk_head.h"
__attribute__((visibility("hidden"))) int lrk_rk_reads_var(const tree_t * t, const char * nm);
__attribute__((visibility("hidden"))) void rk_ss_hoist(tree_t * list);
__attribute__((visibility("hidden"))) void rk_eval_host_classes(const tree_t * t);
__attribute__((visibility("hidden"))) void rk_module_names(const tree_t * t, rk_ns_t * out);
__attribute__((visibility("hidden"))) int lrk_rk_reads_var(const tree_t * t, const char * nm);
__attribute__((visibility("hidden"))) void rk_invocant_params(tree_t * t, const tree_t * pkg);
__attribute__((visibility("hidden"))) void rk_sub_topic(tree_t * t, int in_pkg);
#endif
