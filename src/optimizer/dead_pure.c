#include "dead_pure.h"
#include "ir_index.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dp_pure_lit(IR_e op) { return op == IR_LIT_INTEGER || op == IR_LIT_REAL || op == IR_LIT_STRING; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dp_run(IR_graph_t * g) {
    if (!g || g->n <= 0) return 0;
    char * ref = (char *)calloc((size_t)g->n, 1);
    if (!ref) return 0;
    ir_index_t ix; ir_index_build(&ix, g);
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        for (int k = 0; k < nd->n_operands; k++) { int j = ir_index_of(&ix, nd->operands[k]); if (j >= 0) ref[j] = 1; }
    }
    ir_index_free(&ix);
    int total = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && dp_pure_lit(nd->op) && !ref[i]) { nd->op = IR_SUCCEED; nd->n_operands = 0; total++; } }
    free(ref);
    return total;
}
