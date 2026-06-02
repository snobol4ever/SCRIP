#pragma once
#ifndef EMIT_IR_H
#define EMIT_IR_H
#include <stdio.h>
#include "IR.h"
#include "ast.h"
int  bb_node_id(IR_t * nd);
int  bb_is_generator(IR_e k);
void bb_walk(IR_graph_t * cfg, void (*visit)(IR_t * nd, void * ctx), void * ctx);
#endif
