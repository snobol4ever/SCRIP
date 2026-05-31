/*====================================================================================================================================================================================================*/
/* prove_lower2.c — TOPOLOGY PROOF HARNESS for lower2.c.
 * Builds Proebsting's Figure-1 AST by hand, lowers it through lower2_value_entry, and dumps every IR
 * node's four ports (α/β/γ/ω) as graph indices so the wiring can be diffed against Figures 1 & 2.
 * Links lower2.o + scrip_ir.o ONLY; supplies local kind_is_resumable + cset_try_fold so the production
 * lower.c is NOT pulled in. Built + run by scripts/prove_lower2.sh. Cold-build without the script:
 *   gcc -O0 -g -I src -I src/include -I src/lower -I src/processor -I src/emitter -I src/runtime/core \
 *       -I src/runtime src/lower/lower2.c src/lower/scrip_ir.c src/lower/prove_lower2.c -lgc -lm -o /tmp/prove
 * Requires the lower2_value_entry shim in lower2.c (kept there on purpose).                              */
/*====================================================================================================================================================================================================*/
#define BB_DEFINE_NAMES
#include "IR.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* kind_is_resumable + cset_try_fold now come from lower.o (the production lowerer defines them); the harness no longer supplies local copies, which would otherwise be duplicate symbols at link. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern IR_t * lower2_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * g, IR_t * w, IR_t ** a, IR_t ** b);
extern IR_t * lower2_goal_entry(IR_graph_t * bbg, const tree_t * e, IR_t * g, IR_t * w, IR_t ** a, IR_t ** b);
extern IR_t * lower2_pattern_entry(IR_graph_t * bbg, const tree_t * e, IR_t * g, IR_t * w, IR_t ** a, IR_t ** b);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * lit(long long v) { tree_t * n = ast_node_new(TT_ILIT); n->v.ival = v; return n; }
static tree_t * bin(tree_e op, tree_t * a, tree_t * b) { tree_t * n = ast_node_new(op); ast_push(n, a); ast_push(n, b); return n; }
static tree_t * un(tree_e op, tree_t * a) { tree_t * n = ast_node_new(op); ast_push(n, a); return n; }
static tree_t * tri(tree_e op, tree_t * a, tree_t * b, tree_t * c) { tree_t * n = ast_node_new(op); ast_push(n, a); ast_push(n, b); ast_push(n, c); return n; }
static tree_t * slit(const char * s) { tree_t * n = ast_node_new(TT_QLIT); n->v.sval = (char *) s; return n; }
static tree_t * var(const char * s) { tree_t * n = ast_node_new(TT_VAR); n->v.sval = (char *) s; return n; }
static tree_t * fnc1(const char * name, tree_t * a) { tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) name; ast_push(n, a); return n; }
static tree_t * gfnc2(const char * name, tree_t * a, tree_t * b) { tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) name; ast_push(n, a); ast_push(n, b); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int idx_of(IR_graph_t * g, IR_t * n) { if (!n) return -1; for (int i = 0; i < g->n; i++) if (g->all[i] == n) return i; return -2; }
static const char * kname(IR_e t) {
    switch (t) {
    case IR_LIT_I: return "LIT_I"; case IR_BINOP: return "BINOP"; case IR_TO: return "TO"; case IR_TO_BY: return "TO_BY"; case IR_VAR: return "VAR";
    case IR_UNOP: return "UNOP"; case IR_IF: return "IF"; case IR_SUCCEED: return "PSUCC"; case IR_FAIL: return "PFAIL";
    case IR_CONJ: return "CONJ"; case IR_ALT: return "ALT"; case IR_EVERY: return "EVERY"; case IR_WHILE: return "WHILE";
    case IR_UNTIL: return "UNTIL"; case IR_REPEAT: return "REPEAT"; case IR_NOT: return "NOT";
    case IR_ASSIGN: return "ASGN"; case IR_CALL: return "CALL"; case IR_LIT_S: return "LIT_S";
    case IR_PAT_LIT: return "PLIT"; case IR_PAT_REM: return "PREM"; case IR_PAT_ARB: return "PARB";
    case IR_PAT_SPAN: return "PSPAN"; case IR_PAT_ANY: return "PANY"; case IR_PAT_NOTANY: return "PNANY"; case IR_PAT_BREAK: return "PBRK";
    case IR_PAT_CAT: return "PCAT"; case IR_PAT_ALT: return "PALT";
    case IR_GCONJ: return "GCONJ"; case IR_DISJ: return "DISJ"; case IR_UNIFY: return "UNIFY"; case IR_ARITH: return "ARITH"; case IR_CUT: return "CUT";
    default: return "?";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dump(const char * title, tree_t * ast, int expect_nodes) {
    IR_graph_t * g = IR_alloc(64, 0);
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t * a = NULL, * b = NULL;
    IR_t * top = lower2_value_entry(g, ast, PSUCC, PFAIL, &a, &b);
    printf("=== %s ===\n", title);
    printf("principal idx=%d  α(start)=%d  β(resume)=%d  node_count=%d  (2 sentinels PSUCC=0 PFAIL=1)\n", idx_of(g, top), idx_of(g, a), idx_of(g, b), g->n);
    printf("idx  kind    α    β    γ    ω      ival  dval\n");
    for (int i = 0; i < g->n; i++) {
        IR_t * n = g->all[i];
        printf("%3d  %-6s %3d  %3d  %3d  %3d  %8lld  %.1f\n", i, kname(n->t), idx_of(g, n->α), idx_of(g, n->β), idx_of(g, n->γ), idx_of(g, n->ω), (long long) n->ival, n->dval);
    }
    int real = g->n - 2;
    printf("real(non-sentinel) IR nodes = %d ; expected = %d ; %s\n\n", real, expect_nodes, real == expect_nodes ? "PASS" : "FAIL");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dump_goal(const char * title, tree_t * ast, int expect_nodes) {
    IR_graph_t * g = IR_alloc(64, 0);
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t * a = NULL, * b = NULL;
    IR_t * top = lower2_goal_entry(g, ast, PSUCC, PFAIL, &a, &b);
    printf("=== %s ===\n", title);
    printf("principal idx=%d  α(start)=%d  β(resume)=%d  node_count=%d  (2 sentinels PSUCC=0 PFAIL=1)\n", idx_of(g, top), idx_of(g, a), idx_of(g, b), g->n);
    printf("idx  kind    α    β    γ    ω      ival  dval\n");
    for (int i = 0; i < g->n; i++) {
        IR_t * n = g->all[i];
        printf("%3d  %-6s %3d  %3d  %3d  %3d  %8lld  %.1f\n", i, kname(n->t), idx_of(g, n->α), idx_of(g, n->β), idx_of(g, n->γ), idx_of(g, n->ω), (long long) n->ival, n->dval);
    }
    int real = g->n - 2;
    printf("real(non-sentinel) IR nodes = %d ; expected = %d ; %s\n\n", real, expect_nodes, real == expect_nodes ? "PASS" : "FAIL");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dump_pat(const char * title, tree_t * ast, int expect_nodes) {
    IR_graph_t * g = IR_alloc(64, 0);
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t * a = NULL, * b = NULL;
    IR_t * top = lower2_pattern_entry(g, ast, PSUCC, PFAIL, &a, &b);
    printf("=== %s ===\n", title);
    printf("principal idx=%d  α(start)=%d  β(resume)=%d  node_count=%d  (2 sentinels PSUCC=0 PFAIL=1)\n", idx_of(g, top), idx_of(g, a), idx_of(g, b), g->n);
    printf("idx  kind    α    β    γ    ω      ival  dval\n");
    for (int i = 0; i < g->n; i++) {
        IR_t * n = g->all[i];
        printf("%3d  %-6s %3d  %3d  %3d  %3d  %8lld  %.1f\n", i, kname(n->t), idx_of(g, n->α), idx_of(g, n->β), idx_of(g, n->γ), idx_of(g, n->ω), (long long) n->ival, n->dval);
    }
    int real = g->n - 2;
    printf("real(non-sentinel) IR nodes = %d ; expected = %d ; %s\n\n", real, expect_nodes, real == expect_nodes ? "PASS" : "FAIL");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    tree_t * a_to1 = bin(TT_TO, lit(1), lit(2));
    tree_t * a_to2 = bin(TT_TO, lit(3), lit(4));
    tree_t * a_mul = bin(TT_MUL, a_to1, a_to2);
    tree_t * a_gt  = bin(TT_GT,  lit(5), a_mul);
    dump("5 > ((1 to 2) * (3 to 4))   [Proebsting Fig 1: nine expanded templates]", a_gt, 9);

    tree_t * b_to1 = bin(TT_TO, lit(1), lit(2));
    tree_t * b_to2 = bin(TT_TO, lit(3), lit(4));
    tree_t * b_nest = bin(TT_TO, b_to1, b_to2);
    dump("(1 to 2) to (3 to 4)   [paper sec.2: outer-to initiated four times; to2.fail -> to1]", b_nest, 7);

    /* L2-A combinators */
    dump("(1 to 2) & (3 to 4)   [ir_conjunction: c0.gamma->c1.alpha, c1.omega->c0.beta, resume=c1.beta]",
         bin(TT_SEQ, bin(TT_TO, lit(1), lit(2)), bin(TT_TO, lit(3), lit(4))), 7);
    dump("1 | 2 | 3   [ir_a_Alt: arm.gamma->alt, fail-chain arm[i].omega->arm[i+1].alpha, last->omega]",
         tri(TT_ALTERNATE, lit(1), lit(2), lit(3)), 4);

    /* L2-B loops (core) */
    dump("every (1 to 3)   [ir_a_Every: E1.gamma->E1.resume (no body, drain); E1.omega->every.fail]",
         un(TT_EVERY, bin(TT_TO, lit(1), lit(3))), 4);
    dump("while (1 to 3)   [ir_a_While: cond bounded; E1.gamma->loop; E1.omega->while.fail]",
         un(TT_WHILE, bin(TT_TO, lit(1), lit(3))), 4);
    dump("until (1 to 3)   [ir_a_Until: E1.gamma->until.fail; E1.omega->body/loop]",
         un(TT_UNTIL, bin(TT_TO, lit(1), lit(3))), 4);
    dump("repeat (1 to 3)   [ir_a_Repeat: E.gamma=E.omega=repeat.start]",
         un(TT_REPEAT, bin(TT_TO, lit(1), lit(3))), 4);
    dump("not (1 to 3)   [ir_a_Not: E.gamma->not.fail; E.omega->not(null,succeed)]",
         un(TT_NOT, bin(TT_TO, lit(1), lit(3))), 4);

    /* SHARED TABLE — cross-language hello-world arms (lowering proven; EXEC is the next session) */
    dump("SNOBOL4:  OUTPUT = \"hello world\"   [v_assign: rhs.gamma->ASGN, ASGN.sval=OUTPUT, bounded]",
         bin(TT_ASSIGN, var("OUTPUT"), slit("hello world")), 2);
    dump_goal("Prolog:   write('hello world')   [g_det_builtin1: arg.gamma->CALL, CALL.sval=write, det]",
         fnc1("write", slit("hello world")), 2);

    /* ============================================================================================ */
    /* SHARED COMBINATORS + PER-LANGUAGE PROOF CASES. The three concurrent sessions APPEND their own    */
    /* cases to their OWN section below (between the BEGIN/END markers) so the inserts land in different  */
    /* hunks and git auto-merges — never add a case outside your language's section. (FACT RULE: the      */
    /* proof gate is the shared green signal; cases are additive.)                                        */
    /* ============================================================================================ */

    /* ===== SNOBOL4 SECTION — APPEND SNOBOL4 (PATTERN-role) CASES BELOW THIS LINE ===== */
    dump_pat("SNOBOL4:  'WIN' REM   [PATTERN CAT = wire_seq(IR_PAT_CAT): subsequent, P1.gamma->P2.alpha]",
         bin(TT_CAT, slit("WIN"), ast_node_new(TT_REM)), 3);
    dump_pat("SNOBOL4:  'A' | 'B' | 'C'   [PATTERN ALT = wire_alt(IR_PAT_ALT): fail-chain]",
         tri(TT_ALT, slit("A"), slit("B"), slit("C")), 4);
    /* ===== END SNOBOL4 SECTION ===== */

    /* ===== ICON SECTION — APPEND ICON (VALUE-role) CASES BELOW THIS LINE ===== */
    /* (Icon's foundation/L2-A/B cases live above in the Proebsting block; new Icon proofs go here.) */
    /* ===== END ICON SECTION ===== */

    /* ===== PROLOG SECTION — APPEND PROLOG (GOAL-role) CASES BELOW THIS LINE ===== */
    dump_goal("Prolog:   (write(a) , write(b))   [GOAL conj = wire_seq(IR_GCONJ): same shape as Icon &/SNOBOL CAT]",
         gfnc2(",", fnc1("write", slit("a")), fnc1("write", slit("b"))), 5);
    dump_goal("Prolog:   (write(a) ; write(b))   [GOAL disj = wire_alt(IR_DISJ): same fail-chain as SNOBOL ALT]",
         gfnc2(";", fnc1("write", slit("a")), fnc1("write", slit("b"))), 5);
    dump_goal("Prolog:   X = Y   [g_unify: lhs.gamma->rhs.alpha->UNIFY, semidet resume->fail]",
         gfnc2("=", var("X"), var("Y")), 3);
    dump_goal("Prolog:   X < 5   [g_compare: ARITH ival=BINOP_LT, lhs.gamma->rhs.alpha->ARITH]",
         gfnc2("<", var("X"), lit(5)), 3);
    /* ===== END PROLOG SECTION ===== */
    return 0;
}
