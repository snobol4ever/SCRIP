#include "copy_prop.h"
#include "gen.h"
#include "ir_query.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* THE NODE WHOSE VALUE THIS NODE MERELY COPIES.  This is Proebsting's copy propagation ("Simple Translation of Goal-Directed
   Evaluation", §5: the naive four-port translation "suffers from generating many simple copies … propagating copies and
   eliminating branches to branches optimizes the code well").  His per-operator temporary IS our ζ-spine slot, so a copy here
   is a slot-to-slot move -- two stores and two loads of pure hand-off.  Widening this function is how a box stops existing:
   cp_run redirects the consumer's edge, the copy node falls out of the reference set, becomes IR_SUCCEED, and bc_chase()
   walks γ straight through it.  ⛔ ONLY pure value-identities belong here: a node listed as a copy still EXECUTES for its
   control flow (a predicate's ω still recedes), but nothing may read its slot afterwards. */
static IR_t * cp_source(IR_t * c) {
    if (!c || c->n_operands < 1 || !c->operands[0]) return (IR_t *)0;
    if (c->op == IR_COERCE_STRING && c->operands[0]->op == IR_LIT_STRING) return c->operands[0];
    if (c->op == IR_COERCE_INTEGER && c->operands[0]->op == IR_LIT_INTEGER) { int neg_err = (int)((IR_LIT(c).ival >> 16) & 0xFFFF); if (IR_LIT(c->operands[0]).ival >= 0 || neg_err == 0) return c->operands[0]; }
    if (c->op == IR_BINOP && c->n_operands == 2 && c->operands[1] && (long)IR_LIT(c).ival == BINOP_CONCAT) {
        if (ir_value_is_null_string(c->operands[0])) return c->operands[1];
        if (ir_value_is_null_string(c->operands[1])) return c->operands[0]; }
    return (IR_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cp_index_of(IR_graph_t * g, IR_t * p) { for (int i = 0; i < g->n; i++) if (g->all[i] == p) return i; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cp_run(IR_graph_t * g) {
    if (!g || g->n <= 0) return 0;
    int total = 0;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        for (int k = 0; k < nd->n_operands; k++) { int guard = 0; IR_t * src; while ((src = cp_source(nd->operands[k])) != (IR_t *)0 && guard++ < 32) { nd->operands[k] = src; total++; } }
    }
    if (!total) return 0;
    char * ref = (char *)calloc((size_t)g->n, 1);
    if (!ref) return total;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        for (int k = 0; k < nd->n_operands; k++) { int j = cp_index_of(g, nd->operands[k]); if (j >= 0) ref[j] = 1; }
    }
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && cp_source(nd) != (IR_t *)0 && !ref[i]) { nd->op = IR_SUCCEED; nd->n_operands = 0; total++; } }
    free(ref);
    return total;
}
