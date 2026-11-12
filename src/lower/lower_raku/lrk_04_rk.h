#ifndef LOWER_LOWER_RAKU_LRK_04_RK_H
#define LOWER_LOWER_RAKU_LRK_04_RK_H
#include "lower/lower_raku/lrk_head.h"
__attribute__((visibility("hidden"))) const tree_t * rk_find_type_decl(const tree_t * prog, const char * name);
__attribute__((visibility("hidden"))) tree_t * rk_ti(long long v);
__attribute__((visibility("hidden"))) tree_t * rk_ti(long long v);
__attribute__((visibility("hidden"))) tree_t * rk_ti(long long v);
__attribute__((visibility("hidden"))) tree_t * rk_t2(tree_e k, tree_t * a, tree_t * b);
__attribute__((visibility("hidden"))) tree_t * rk_gath_asg(tree_t * lhs, tree_t * rhs);
__attribute__((visibility("hidden"))) void rk_return_outside_routine(tree_t * t, int in_routine);
__attribute__((visibility("hidden"))) void rk_seq_desugar(tree_t * t);
__attribute__((visibility("hidden"))) void rk_lazy_stages(tree_t * t);
__attribute__((visibility("hidden"))) void rk_gather_consumers(tree_t * t, tree_t * parent, int idx);
__attribute__((visibility("hidden"))) tree_t * rk_tv(const char * nm);
__attribute__((visibility("hidden"))) tree_t * rk_tset(const char * v, tree_t * e);
__attribute__((visibility("hidden"))) tree_t * rk_sort_helper(const char * hname, const char * cmp, int nref);
__attribute__((visibility("hidden"))) void rk_new_build_walk(tree_t * prog, tree_t * t, int * seq);
__attribute__((visibility("hidden"))) void rk_smartmatch_topic_walk(tree_t * t);
__attribute__((visibility("hidden"))) void rk_parse_actions_walk(tree_t * t, int * seq);
__attribute__((visibility("hidden"))) void rk_hyper_meth_walk(tree_t * t, int * seq);
__attribute__((visibility("hidden"))) void rk_user_terms(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_infinite_ranges(tree_t * t, int * seq, int insub);
__attribute__((visibility("hidden"))) void rk_xx_thunks(tree_t * t);
__attribute__((visibility("hidden"))) void rk_array_single_arg(tree_t * t);
__attribute__((visibility("hidden"))) void rk_wrap_bodies(tree_t * t);
__attribute__((visibility("hidden"))) void rk_pl_enum(tree_t * t);
__attribute__((visibility("hidden"))) void rk_pl_modifier(tree_t * t, int kind);
#endif
