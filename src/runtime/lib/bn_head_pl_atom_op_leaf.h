#ifndef BN_HEAD_PL_ATOM_OP_LEAF_H
#define BN_HEAD_PL_ATOM_OP_LEAF_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
#define PL_ATOM_OP_LEAF(nm, ar) PL_CX_LEAF_HEAD(nm, ar) ok = rt_pl_atom_op_cell(#nm, &args[0], ar > 1 ? (void *)&args[1] : (void *)0, ar > 2 ? (void *)&args[2] : (void *)0, cx); PL_CX_LEAF_TAIL
#ifdef __cplusplus
}
#endif
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pl_cx_leaf_head.h"
#endif
