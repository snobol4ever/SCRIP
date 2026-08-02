#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IR.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rr_idx_of(IR_graph_t * g, IR_t * t) { for (int i = 0; i < g->n; i++) if (g->all[i] == t) return i; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rr_is_beta(const char * sz) { return sz && sz[0] == (char)0xce && (unsigned char)sz[1] == 0xb2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void region_report(IR_graph_t * g) {
    if (!g || g->n <= 0) return;
    int * rid = (int *)calloc((size_t)g->n, sizeof(int)); if (!rid) return;
    int next = 0, cur = 0;
    for (int i = 0; i < g->n; i++) { IR_t * nd = g->all[i]; if (nd && nd->op == IR_MATCH_BEGIN) cur = ++next; rid[i] = cur; if (nd && nd->op == IR_MATCH_END) cur = ++next; }
    int nr = next + 1;
    int * bin = (int *)calloc((size_t)nr, sizeof(int)); int * bout = (int *)calloc((size_t)nr, sizeof(int)); int * bxin = (int *)calloc((size_t)nr, sizeof(int));
    int * lo = (int *)calloc((size_t)nr, sizeof(int)); int * hi = (int *)calloc((size_t)nr, sizeof(int)); int * cnt = (int *)calloc((size_t)nr, sizeof(int)); int * pat = (int *)calloc((size_t)nr, sizeof(int));
    if (!bin || !bout || !bxin || !lo || !hi || !cnt || !pat) { free(rid); free(bin); free(bout); free(bxin); free(lo); free(hi); free(cnt); free(pat); return; }
    for (int r = 0; r < nr; r++) lo[r] = -1;
    for (int i = 0; i < g->n; i++) { int r = rid[i]; if (lo[r] < 0) lo[r] = i; hi[r] = i; cnt[r]++; IR_t * nd = g->all[i]; if (nd && (nd->op == IR_MATCH_BEGIN || (nd->op >= IR_MATCH_LIT && nd->op <= IR_MATCH_ASSIGN_SAVE))) pat[r] = 1; }
    for (int i = 0; i < g->n; i++) {
        IR_t * nd = g->all[i]; if (!nd) continue;
        IR_t * tg[2] = { nd->γ.node, nd->ω.node }; const char * sg[2] = { nd->γ.sz, nd->ω.sz };
        for (int e = 0; e < 2; e++) { if (!tg[e]) continue; int j = rr_idx_of(g, tg[e]); if (j < 0) continue; if (rid[j] != rid[i]) { bout[rid[i]]++; bin[rid[j]]++; if (rr_is_beta(sg[e])) bxin[rid[j]]++; } }
    }
    fprintf(stderr, "[REGION-REPORT] graph=%p nodes=%d regions=%d (partition v0: MATCH_BEGIN..MATCH_END pattern intervals; branch-in/out over gamma+omega edges; beta-sigil external landings tracked)\n", (void *)g, g->n, nr);
    for (int r = 0; r < nr; r++) { if (cnt[r] == 0) continue;
        fprintf(stderr, "[REGION %2d] span=[%3d..%3d] n=%3d %s in=%d out=%d beta-in=%d  %s\n", r, lo[r], hi[r], cnt[r], pat[r] ? "PATTERN" : "plain  ", bin[r], bout[r], bxin[r], bxin[r] == 0 ? "GAMMA-FREE-SAFE (no external backtrack re-entry: frame dead past this region's gamma)" : "HELD (external beta lands here: frame must survive its gamma)"); }
    free(rid); free(bin); free(bout); free(bxin); free(lo); free(hi); free(cnt); free(pat);
}
