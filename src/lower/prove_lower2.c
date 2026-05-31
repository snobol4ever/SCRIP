/* prove_lower2.c — lower the paper's Figure-1 example through lower2() and dump port topology.
   Links: lower2.c + scrip_ir.c only. Provides local kind_is_resumable + cset_try_fold so the old
   lowerer is NOT linked. AST built by hand. Output: one line per IR node: idx kind α β γ ω.        */
/* BUILD (cold session — links lower2.c + scrip_ir.c ONLY, NOT the old lowerer):
 *   gcc -std=c99 -include string.h -I src -I src/include -I src/lower -I src/processor \
 *       -I src/runtime/interp -I src/runtime/core -I src/runtime/rt -I src/runtime \
 *       -I src/frontend/snobol4 -I src/driver -I src/emitter \
 *       src/lower/lower2.c src/lower/scrip_ir.c src/lower/prove_lower2.c -lm -o /tmp/prove
 *   /tmp/prove          # dumps each IR node idx + α/β/γ/ω to diff against Proebsting Fig 1/2.
 * Swap the test expression in main() (lit/bin helpers) to prove other shapes.
 * Requires the lower2_value_entry shim in lower2.c (kept there on purpose).            */

#define BB_DEFINE_NAMES
#include "IR.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int kind_is_resumable(IR_e t) {
    return t == IR_TO || t == IR_TO_BY || t == IR_UPTO || t == IR_ALT ||
           t == IR_BINOP_GEN || t == IR_ITERATE || t == IR_LIMIT || t == IR_PROC_GEN ||
           t == IR_EVERY || t == IR_REPEAT || t == IR_SUSPEND || t == IR_SCAN ||
           t == IR_LIST_BANG || t == IR_KEY_GEN || t == IR_FIND_GEN || t == IR_SEQ_GEN ||
           t == IR_GEN_SCAN || t == IR_CONJ;
}
char * cset_try_fold(const tree_t * t) { (void) t; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* the real entry we are proving (static in lower2.c; re-declared via a thin shim compiled WITH it) */
extern IR_t * lower2_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * g, IR_t * w, IR_t ** a, IR_t ** b);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * lit(long long v)            { tree_t * n = ast_node_new(TT_ILIT); n->v.ival = v; return n; }
static tree_t * bin(tree_e op, tree_t * a, tree_t * b) { tree_t * n = ast_node_new(op); ast_push(n, a); ast_push(n, b); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int idx_of(IR_graph_t * g, IR_t * n) {
    if (!n) return -1;
    for (int i = 0; i < g->n; i++) if (g->all[i] == n) return i;
    return -2;
}
static const char * kname(IR_e t) {
    switch (t) {
    case IR_LIT_I: return "LIT_I"; case IR_BINOP: return "BINOP"; case IR_TO: return "TO";
    case IR_TO_BY: return "TO_BY"; case IR_VAR: return "VAR"; case IR_UNOP: return "UNOP";
    case IR_IF: return "IF"; case IR_SUCCEED: return "PSUCC"; case IR_FAIL: return "PFAIL"; default: return "?";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    /* 5 > ((1 to 2) * (3 to 4)) */
    tree_t * to1 = bin(TT_TO, lit(1), lit(2));
    tree_t * to2 = bin(TT_TO, lit(3), lit(4));
    tree_t * gt  = bin(TT_TO, to1, to2);   /* (1 to 2) to (3 to 4) — nested-generator bounds */

    IR_graph_t * g = IR_alloc(64, 0 /*lang*/);
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED); /* sentinel program-success */
    IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);    /* sentinel program-fail */
    IR_t * a = NULL, * b = NULL;
    IR_t * top = lower2_value_entry(g, gt, PSUCC, PFAIL, &a, &b);

    printf("=== lower2((1 to 2) to (3 to 4)) ===\n");
    printf("returned principal idx=%d ; α(start)=%d ; β(resume)=%d\n",
           idx_of(g, top), idx_of(g, a), idx_of(g, b));
    printf("graph node count=%d\n", g->n);
    printf("idx  kind    α    β    γ    ω\n");
    for (int i = 0; i < g->n; i++) {
        IR_t * n = g->all[i];
        printf("%3d  %-6s %3d  %3d  %3d  %3d   (ival=%lld dval=%.1f)\n",
               i, kname(n->t), idx_of(g, n->α), idx_of(g, n->β), idx_of(g, n->γ), idx_of(g, n->ω),
               (long long) n->ival, n->dval);
    }
    return 0;
}
