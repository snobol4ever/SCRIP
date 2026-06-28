#include "branchopt.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static double bopt_litdval(const IR_t * n) {
    if (!n) return 0.0;
    return n->dval;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int bopt_is_forwarder(const IR_t * n) {
    if (!n || !n->γ.node) return 0;
    if (n->op == IR_SUCCEED) return 1;
    if ((n->op == IR_SEQ || n->op == IR_SEQ_EXPR) && bopt_litdval(n) == 1.0) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * bopt_resolve(IR_t * n) {
    int guard = 0;
    while (bopt_is_forwarder(n) && guard++ < 4096) n = n->γ.node;
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bopt_chain(IR_graph_t * g) {
    if (!g || !g->all) return 0;
    int rewired = 0;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        if (nd->γ.node) { IR_t * r = bopt_resolve(nd->γ.node); if (r != nd->γ.node) { nd->γ.node = r; rewired++; } }
        if (nd->ω.node) { IR_t * r = bopt_resolve(nd->ω.node); if (r != nd->ω.node) { nd->ω.node = r; rewired++; } }
    }
    return rewired;
}
