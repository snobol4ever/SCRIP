#pragma once
#ifndef IR_INTERP_H
#define IR_INTERP_H
#include "IR.h"
typedef int (*bb_body_fn)(DESCR_t value, void * ctx);
DESCR_t IR_interp_once(IR_graph_t * bbg);
DESCR_t IR_interp_resume(IR_graph_t * bbg);
int IR_interp_pump(IR_graph_t * bbg, bb_body_fn body_fn, void * ctx);
IR_t * IR_interp_node(IR_t * bb);
int IR_interp_pat(IR_graph_t *bbg,
                const char *subj_name,
                DESCR_t    *subj_var,
                DESCR_t    *repl,
                int         has_repl);
#endif
