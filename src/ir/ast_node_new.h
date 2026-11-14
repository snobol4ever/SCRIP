#ifndef AST_NODE_NEW_H
#define AST_NODE_NEW_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir/ct_arena.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
typedef struct tree_t tree_t;
#ifdef __cplusplus
extern "C" {
#endif
static inline tree_t * ast_node_new(tree_e kind) { tree_t * e = (tree_t *)ct_zalloc(1, sizeof(tree_t)); e->t = kind; return e; }
#ifdef __cplusplus
}
#endif
#endif
