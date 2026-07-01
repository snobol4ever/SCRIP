/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef BRANCH_CHAIN_H
#define BRANCH_CHAIN_H
#include "IR.h"
int bc_is_passthrough(IR_e op);
IR_t * bc_chase(IR_t *node);
int bc_redirect_to(IR_graph_t *g, IR_t *dead, IR_t *replacement);
int bc_run(IR_graph_t *g);
#endif
