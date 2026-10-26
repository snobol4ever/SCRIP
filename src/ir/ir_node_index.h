#ifndef IR_NODE_INDEX_H
#define IR_NODE_INDEX_H
#include "IR.h"
#ifdef __cplusplus
extern "C" {
#endif
int bb_node_id(IR_t * nd);
void nidx_build(IR_t ** nodes, int n);
int nidx(IR_t ** nodes, int n, const IR_t * e);
void nidx_reset(void);
#ifdef __cplusplus
}
#endif
#endif
