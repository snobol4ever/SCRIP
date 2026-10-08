/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef BRANCH_CHAIN_H
#define BRANCH_CHAIN_H
#include "IR.h"
#include "ir_index.h"
#include "ct_vec.h"
typedef struct { int *gen; int *pos; int cur; cv_t list; } bc_walk_t; int bc_is_passthrough(IR_e op); IR_t * bc_chase(const char *prot, const ir_index_t *ix, bc_walk_t *w, IR_t *node, char sz[4]);
int bc_run(IR_graph_t *g);
#endif
