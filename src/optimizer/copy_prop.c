/*--------------------------------------------------------------------------------------------------------------------*/
#include "copy_prop.h"
#include "branch_chain.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static void redirect_operand(IR_graph_t *g, IR_t *from, IR_t *to) {
    for (int i = 0; i < g->n; i++) {
        IR_t *nd = g->all[i];
        if (!nd || nd == from) continue;
        for (int k = 0; k < nd->n_operands; k++) if (nd->operands[k] == from) nd->operands[k] = to;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int cp_run(IR_graph_t *g) {
    int total = 0, changed;
    do {
        changed = 0;
        for (int i = 0; i < g->n; i++) {
            IR_t *m = g->all[i];
            if (!m || m->op != IR_MOVE || m->n_operands != 1 || !m->operands[0] || m->operands[0] == m || !m->γ.node) continue;
            IR_t *src = m->operands[0];
            redirect_operand(g, m, src);
            bc_redirect_to(g, m, bc_chase(m->γ.node));
            total++; changed = 1;
        }
    } while (changed);
    return total;
}
