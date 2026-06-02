#pragma once
#ifndef BB_EXEC_H
#define BB_EXEC_H
#include "IR.h"
typedef int (*bb_body_fn)(DESCR_t value, void * ctx);
DESCR_t bb_exec_once(IR_graph_t * bbg);
DESCR_t bb_exec_resume(IR_graph_t * bbg);
int bb_exec_pump(IR_graph_t * bbg, bb_body_fn body_fn, void * ctx);
IR_t * bb_exec_node(IR_t * bb);
int bb_exec_pat(IR_graph_t *bbg,
                const char *subj_name,
                DESCR_t    *subj_var,
                DESCR_t    *repl,
                int         has_repl);
#endif
