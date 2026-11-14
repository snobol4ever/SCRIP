#ifndef LOWER_LOWER_PROLOG_LPL_09_SH_H
#define LOWER_LOWER_PROLOG_LPL_09_SH_H
#include "lower/lower_prolog/lpl_head.h"
#include "ir/ast.h"
__attribute__((visibility("hidden"))) IR_t * goal_inner(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
#endif
