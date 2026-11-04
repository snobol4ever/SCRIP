#ifndef IR_NODE_INDEX_H
#define IR_NODE_INDEX_H
#include <stddef.h>
#include "ir/ir_node.h"
#ifdef __cplusplus
extern "C" {
#endif
int bb_node_id(IR_t * nd);
void nidx_build(IR_t ** nodes, int n);
int nidx(IR_t ** nodes, int n, const IR_t * e);
void nidx_reset(void);
typedef struct { IR_t ** nodes; int n; IR_t ** keys; int * vals; size_t cap; int own; } nidx_scope_t;
void nidx_scope_open(nidx_scope_t * s, IR_t ** nodes, int n);
void nidx_scope_close(const nidx_scope_t * s);
#ifdef __cplusplus
}
#endif
#endif
