/*--------------------------------------------------------------------------------------------------------------------*/
#include "branch_chain.h"
#include <stdio.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------------------------------------------------*/
int bc_is_passthrough(IR_e op) { return op == IR_SUCCEED; }
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * bc_chase(IR_t *node) {
    IR_t *seen[512]; int ns = 0; int guard = 0;
    while (node && bc_is_passthrough(node->op) && node->γ.node && guard++ < 512) {
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == node) { dup = 1; break; }
        if (dup) break;
        if (ns < 512) seen[ns++] = node;
        node = node->γ.node;
    }
    return node;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bc_redirect_to(IR_graph_t *g, IR_t *dead, IR_t *replacement) {
    int n = 0;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd || nd == dead) continue;
        if (nd->γ.node == dead) { nd->γ.node = replacement; n++; }
        if (nd->ω.node == dead) { nd->ω.node = replacement; n++; }
    }
    if (g->entry == dead) { g->entry = replacement; n++; }
    if (g->body_root == dead) { g->body_root = replacement; n++; }
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bc_run(IR_graph_t *g) {
    int total = 0; int trace = getenv("SCRIP_OPT_TRACE") ? 1 : 0;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd) continue;
        if (nd->γ.node) { IR_t *t = bc_chase(nd->γ.node); if (t != nd->γ.node) { if (trace) fprintf(stderr, "[bc] %p(op=%d).γ  %p(op=%d) -> %p(op=%d)\n", (void*)nd,(int)nd->op,(void*)nd->γ.node,(int)nd->γ.node->op,(void*)t,(int)t->op); nd->γ.node = t; total++; } }
        if (nd->ω.node) { IR_t *t = bc_chase(nd->ω.node); if (t != nd->ω.node) { if (trace) fprintf(stderr, "[bc] %p(op=%d).ω  %p(op=%d) -> %p(op=%d)\n", (void*)nd,(int)nd->op,(void*)nd->ω.node,(int)nd->ω.node->op,(void*)t,(int)t->op); nd->ω.node = t; total++; } }
    }
    if (g->entry) { IR_t *t = bc_chase(g->entry); if (t != g->entry) { g->entry = t; total++; } }
    if (g->body_root) { IR_t *t = bc_chase(g->body_root); if (t != g->body_root) { g->body_root = t; total++; } }
    return total;
}
