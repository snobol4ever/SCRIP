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
int pf_run(IR_graph_t * g) {
    if (!g || g->n <= 0) return 0;
    int * in = (int *)calloc((size_t)g->n, sizeof(int));
    if (!in) return 0;
    int total = 0;
    for (int pass = 0; pass < 8; pass++) {
        int changed = 0;
        pf_count_in(g, in);
        for (int i = 0; i < g->n; i++) {
            IR_t * seq = g->all[i];
            if (!seq || seq->op != IR_MATCH_SEQUENCE || !pf_seq_paired(seq)) continue;
            if ((long)IR_LIT(seq).ival * 2 != (long)seq->n_operands) continue;
            for (int k = 0; k + 2 < seq->n_operands; k += 2) {
                IR_t * A = seq->operands[k], * B = seq->operands[k + 2];
                if (A == B || !pf_lit_internal(A, seq, g, in) || !pf_lit_internal(B, seq, g, in)) continue;
                const char * as = IR_LIT(A).sval ? IR_LIT(A).sval : "", * bs = IR_LIT(B).sval ? IR_LIT(B).sval : "";
                size_t al = strlen(as), bl = strlen(bs);
                char * jn = (char *)malloc(al + bl + 1);
                if (!jn) continue;
                memcpy(jn, as, al); memcpy(jn + al, bs, bl + 1);
                IR_LIT(A).sval = jn;
                for (int m = k + 2; m + 2 < seq->n_operands; m++) seq->operands[m] = seq->operands[m + 2];
                seq->n_operands -= 2;
                IR_LIT(seq).ival -= 1;
                changed++; total++;
                break;
            }
        }
        if (!changed) break;
    }
    free(in);
    return total;
}
