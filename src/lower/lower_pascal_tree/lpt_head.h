#ifndef LOWER_LOWER_PASCAL_TREE_LPT_HEAD_H
#define LOWER_LOWER_PASCAL_TREE_LPT_HEAD_H
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "ir/ct_arena.h"
#include "ir/ast.h"
#include "parsers/snobol4/scrip_cc.h"
#include "runtime/builtins/gen_runtime.h"
#include "ir/bb_box.h"
#include "runtime/builtins/gen.h"
#include "lower/lower_pascal_tree.h"
#include "lower/g_lower.h"
#include "lower/lower_pascal_class.h"
#include "lower/lower_pascal_generics.h"
#include "lower/lower_pascal_types.h"
#define IS(x, k) ((x) && (x)->t == (k))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { char *c; int n, cap, sp, depth; PNodeList *lv; } pas_xrf_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct pas_blk { tree_t *b; struct pas_blk *up; };
#endif
