#ifndef LOWER_LOWER_RAKU_LRK_10_RK_H
#define LOWER_LOWER_RAKU_LRK_10_RK_H
#include "lower/lower_raku/lrk_head.h"
__attribute__((visibility("hidden"))) int rk_proc_is_generator(const char * name);
__attribute__((visibility("hidden"))) const char * rk_ph_of(const tree_t * n);
__attribute__((visibility("hidden"))) void rk_place_phasers(tree_t * prog);
__attribute__((visibility("hidden"))) void rk_mark_generators(void);
__attribute__((visibility("hidden"))) void rk_gather_blockify(tree_t * t);
__attribute__((visibility("hidden"))) void rk_supply_nodes(tree_t * t, int user_emit);
#endif
