#include "dead_pure.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dp_index_of(IR_graph_t * g, IR_t * p) { for (int i = 0; i < g->n; i++) if (g->all[i] == p) return i; return -1; }
static int dp_pure_lit(IR_e op) { return op == IR_LIT_INTEGER || op == IR_LIT_REAL || op == IR_LIT_STRING; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dp_run(IR_graph_t * g) {
    if (!g || g->n <= 0) return 0;
    char * ref = (char *)calloc((size_t)g->n, 1);
    if (!ref) return 0;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        for (int k = 0; k < nd->n_operands; k++) { int j = dp_index_of(g, nd->operands[k]); if (j >= 0) ref[j] = 1; }
    }
    int total = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && dp_pure_lit(nd->op) && !ref[i]) { nd->op = IR_SUCCEED; nd->n_operands = 0; total++; } }
    free(ref);
    return total;
}
