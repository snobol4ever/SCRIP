#pragma once
#ifndef LOWER_PAT_DCG_H
#define LOWER_PAT_DCG_H
#include "BB.h"
#include "../ast/ast.h"
struct _PATND_t;
IR_graph_t * IR_lower_pat(const tree_t * pat_tree);
IR_graph_t * patnd_to_bb_graph(struct _PATND_t * pp);
IR_graph_t * patnd_to_bb_tree(struct _PATND_t * pp);
#endif
