#include "dead_goto.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dg_mon(void) { static int m = -1; if (m < 0) m = (getenv("MONITOR_BIN") && getenv("MONITOR_GOTO_TAP")) ? 1 : 0; return m; }
static int dg_index_of(IR_graph_t * g, IR_t * p) { for (int i = 0; i < g->n; i++) if (g->all[i] == p) return i; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dg_run(IR_graph_t * g) {
    if (!g || g->n <= 0) return 0;
    char * ref = (char *)calloc((size_t)g->n, 1);
    if (!ref) return 0;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        for (int k = 0; k < nd->n_operands; k++) { int j = dg_index_of(g, nd->operands[k]); if (j >= 0) ref[j] = 1; }
        if (nd->γ.node) { int j = dg_index_of(g, nd->γ.node); if (j >= 0) ref[j] = 1; }
        if (nd->ω.node) { int j = dg_index_of(g, nd->ω.node); if (j >= 0) ref[j] = 1; }
    }
    if (g->entry) { int j = dg_index_of(g, g->entry); if (j >= 0) ref[j] = 1; }
    if (g->body_root) { int j = dg_index_of(g, g->body_root); if (j >= 0) ref[j] = 1; }
    int total = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && nd->op == IR_GOTO && !ref[i] && !(dg_mon() && IR_LIT(nd).ival > 0)) { g->all[i] = NULL; total++; } }
    free(ref);
    return total;
}
