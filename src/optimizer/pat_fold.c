#include "pat_fold.h"
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pf_index_of(IR_graph_t * g, IR_t * p) { for (int i = 0; i < g->n; i++) if (g->all[i] == p) return i; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pf_count_in(IR_graph_t * g, int * in) {
    for (int i = 0; i < g->n; i++) in[i] = 0;
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i];
        if (!nd) continue;
        if (nd->γ.node) { int j = pf_index_of(g, nd->γ.node); if (j >= 0) in[j]++; }
        if (nd->ω.node) { int j = pf_index_of(g, nd->ω.node); if (j >= 0) in[j]++; }
        for (int k = 0; k < nd->n_operands; k++) { int j = pf_index_of(g, nd->operands[k]); if (j >= 0) in[j]++; }
    }
    if (g->entry)     { int j = pf_index_of(g, g->entry);     if (j >= 0) in[j]++; }
    if (g->body_root) { int j = pf_index_of(g, g->body_root); if (j >= 0) in[j]++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pf_seq_paired(const IR_t * seq) {
    if (seq->n_operands < 4 || (seq->n_operands & 1)) return 0;
    for (int k = 0; k < seq->n_operands; k += 2) if (!seq->operands[k] || seq->operands[k] != seq->operands[k + 1]) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pf_lit_internal(IR_t * el, IR_t * seq, IR_graph_t * g, const int * in) {
    if (!el || el->op != IR_MATCH_LIT || el->n_operands != 0) return 0;
    if (el->γ.node != seq || el->ω.node != seq) return 0;
    int j = pf_index_of(g, el);
    return j >= 0 && in[j] == 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pf_run(IR_graph_t * g) { (void)g; return 0; }   /* SEQ-ERAD SE-5/SE-6: IR_MATCH_SEQUENCE deleted; literal-adjacency folding was its only customer */
