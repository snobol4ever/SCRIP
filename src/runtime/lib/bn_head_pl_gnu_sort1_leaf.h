#ifndef BN_HEAD_PL_GNU_SORT1_LEAF_H
#define BN_HEAD_PL_GNU_SORT1_LEAF_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
#define PL_GNU_SORT1_LEAF(nm, mode) PL_CX_LEAF_HEAD(nm, 1) { extern int rt_pl_sort_in_place_cell(int, void *); void *b = pl_sort_list_ball(rt_pl_deref_val(args[0]), (mode) == 2); if (b) { cx->ball = \
    b; ok = 0; } else ok = rt_pl_sort_in_place_cell((mode), &args[0]); } PL_CX_LEAF_TAIL
#ifdef __cplusplus
}
#endif
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pl_cx_leaf_head.h"
#endif
