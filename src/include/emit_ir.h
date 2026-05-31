#pragma once
#ifndef EMIT_IR_H
#define EMIT_IR_H
#include <stdio.h>
#include "BB.h"
#include "../ast/ast.h"
int  bb_node_id(BB_t * nd);
int  bb_is_generator(BB_op_t k);
void bb_walk(IR_graph_t * cfg, void (*visit)(BB_t * nd, void * ctx), void * ctx);
#endif
