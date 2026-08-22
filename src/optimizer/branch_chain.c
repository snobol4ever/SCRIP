#include "branch_chain.h"
#include "ir_index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bc_is_passthrough(IR_e op) { return op == IR_SUCCEED || op == IR_GOTO; }
static int bc_mon(void) { static int m = -1; if (m < 0) m = (getenv("MONITOR_BIN") && getenv("MONITOR_GOTO_TAP")) ? 1 : 0; return m; }
static int bc_stamped(const IR_t *nd) { return bc_mon() && nd->op == IR_GOTO && IR_LIT(nd).ival > 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * bc_build_protect(IR_graph_t *g, const ir_index_t *ix) {
    char *prot = (char *)calloc((size_t)(g->n > 0 ? g->n : 1), 1);
    if (!prot) return prot;
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd) continue;
        for (int k = 0; k < nd->n_operands; k++) { int j = ir_index_of(ix, nd->operands[k]); if (j >= 0) prot[j] = 1; }
    }
    return prot;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * bc_chase(const char *prot, const ir_index_t *ix, IR_t *node, char sz[4]) {
    IR_t *seen[512]; int ns = 0; int guard = 0;
    while (node && bc_is_passthrough(node->op) && !bc_stamped(node) && node->γ.node && guard++ < 512) {
        int j = ir_index_of(ix, node);
        if (j >= 0 && prot[j]) break;
        int dup = 0; for (int i = 0; i < ns; i++) if (seen[i] == node) { dup = 1; break; }
        if (dup) break;
        if (ns < 512) seen[ns++] = node;
        memcpy(sz, node->γ.sz, 4);
        node = node->γ.node;
    }
    return node;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bc_run(IR_graph_t *g) {
    int total = 0; int trace = getenv("SCRIP_OPT_TRACE") ? 1 : 0;
    ir_index_t ix; ir_index_build(&ix, g);
    char *prot = bc_build_protect(g, &ix);
    if (!prot) { ir_index_free(&ix); return 0; }
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd) continue;
        if (nd->γ.node) {
            char sz[4]; memcpy(sz, nd->γ.sz, 4); IR_t *t = bc_chase(prot, &ix,nd->γ.node, sz);
            if (t != nd->γ.node) {
                if (trace) fprintf(stderr, "[bc] %p(op=%d).γ %p(op=%d) -> %p(op=%d) sz=%s\n", (void *)nd, (int)nd->op, (void *)nd->γ.node, (int)nd->γ.node->op, (void *)t, (int)t->op, sz);
                nd->γ.node = t; memcpy(nd->γ.sz, sz, 4); total++;
            }
        }
        if (nd->ω.node) {
            char sz[4]; memcpy(sz, nd->ω.sz, 4); IR_t *t = bc_chase(prot, &ix,nd->ω.node, sz);
            if (t != nd->ω.node) {
                if (trace) fprintf(stderr, "[bc] %p(op=%d).ω %p(op=%d) -> %p(op=%d) sz=%s\n", (void *)nd, (int)nd->op, (void *)nd->ω.node, (int)nd->ω.node->op, (void *)t, (int)t->op, sz);
                nd->ω.node = t; memcpy(nd->ω.sz, sz, 4); total++;
            }
        }
    }
    if (g->entry) { char sz[4] = { 0 }; IR_t *t = bc_chase(prot, &ix,g->entry, sz); if (t != g->entry) { g->entry = t; total++; } }
    if (g->body_root) { char sz[4] = { 0 }; IR_t *t = bc_chase(prot, &ix,g->body_root, sz); if (t != g->body_root) { g->body_root = t; total++; } }
    free(prot);
    ir_index_free(&ix);
    return total;
}
