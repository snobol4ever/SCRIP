#include "dead_goto.h"
#include "ir_index.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dg_mon(void) { static int m = -1; if (m < 0) m = (getenv("MONITOR_BIN") && getenv("MONITOR_GOTO_TAP")) ? 1 : 0; return m; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dg_run(IR_graph_t * g) {
    if (!g || g->n <= 0) return 0;
    char * ref = (char *)calloc((size_t)g->n, 1);
    if (!ref) return 0;
    ir_index_t ix; ir_index_build(&ix, g);
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        for (int k = 0; k < nd->n_operands; k++) { int j = ir_index_of(&ix, nd->operands[k]); if (j >= 0) ref[j] = 1; }
        if (nd->γ.node) { int j = ir_index_of(&ix, nd->γ.node); if (j >= 0) ref[j] = 1; }
        if (nd->ω.node) { int j = ir_index_of(&ix, nd->ω.node); if (j >= 0) ref[j] = 1; }
    }
    if (g->entry) { int j = ir_index_of(&ix, g->entry); if (j >= 0) ref[j] = 1; }
    if (g->body_root) { int j = ir_index_of(&ix, g->body_root); if (j >= 0) ref[j] = 1; }
    ir_index_free(&ix);
    int total = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && nd->op == IR_GOTO && !ref[i] && !(dg_mon() && IR_LIT(nd).ival > 0)) { g->all[i] = NULL; total++; } }
    free(ref);
    return total;
}
