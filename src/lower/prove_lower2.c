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
static tree_t * kw(const char * s) { tree_t * n = ast_node_new(TT_KEYWORD); n->v.sval = (char *) s; return n; }
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
    case IR_PAT_LEN: return "PLEN"; case IR_PAT_POS: return "PPOS"; case IR_PAT_TAB: return "PTAB";
    case IR_PAT_FENCE: return "PFNC"; case IR_PAT_ABORT: return "PABT"; case IR_PAT_ARBNO: return "PARBN";
    case IR_PAT_ASSIGN_COND: return "PCAP"; case IR_PAT_ASSIGN_IMM: return "PCAPI"; case IR_PAT_ATP: return "PATP";
    case IR_PAT_DEFER: return "PDEF"; case IR_PAT_BAL: return "PBAL";
    case IR_GCONJ: return "GCONJ"; case IR_DISJ: return "DISJ"; case IR_UNIFY: return "UNIFY"; case IR_ARITH: return "ARITH"; case IR_CUT: return "CUT";
    case IR_BUILTIN: return "BLTIN"; case IR_ATOM: return "ATOM"; case IR_STRUCT: return "STRCT"; case IR_LOGICVAR: return "LVAR";
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
/* dump_sno_value — VALUE role under IR_LANG_SNO (lang seeded from bbg->lang). For SNOBOL4-gated value arms
 * (cx.lang==IR_LANG_SNO), e.g. keyword-assign `&NAME = expr`, which the shared lang-0 dump would route loud. */
static void dump_sno_value(const char * title, tree_t * ast, int expect_nodes) {
    IR_graph_t * g = IR_alloc(64, IR_LANG_SNO);
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
    dump_sno_value("SNOBOL4:  &ANCHOR = 1   [v_assign keyword lhs: ASGN.sval=ANCHOR (& stripped), routes NV_SET_fn kw dispatch; SPITBOL ch.16]",
         bin(TT_ASSIGN, kw("ANCHOR"), lit(1)), 2);
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
    /* L2-P leaves — each case was wired 2026-05-31; proven here via node-count + port dump */
    dump_pat("SNOBOL4:  LEN(3)   [IR_PAT_LEN: generator(resumable) β=self, single-choice match 3 chars]",
         un(TT_LEN, lit(3)), 1);
    dump_pat("SNOBOL4:  POS(2)   [IR_PAT_POS: bounded β=omega, cursor-check from left (SPITBOL ch.19)]",
         un(TT_POS, lit(2)), 1);
    dump_pat("SNOBOL4:  RPOS(1)  [IR_PAT_POS sval=r: bounded, cursor check from right (RPOS(1)=cursor==N-1)]",
         un(TT_RPOS, lit(1)), 1);
    dump_pat("SNOBOL4:  TAB(5)   [IR_PAT_TAB: generator, match chars up to cursor 5]",
         un(TT_TAB, lit(5)), 1);
    dump_pat("SNOBOL4:  RTAB(2)  [IR_PAT_TAB sval=r: generator, match to N-2 from end]",
         un(TT_RTAB, lit(2)), 1);
    dump_pat("SNOBOL4:  FENCE(bare)  [IR_PAT_FENCE: bounded; commits match, backtrack => fail (SPITBOL ch.9)]",
         ast_node_new(TT_FENCE), 1);
    /* FENCE(inner): inner lit is lowered first, FENCE is its successor; 2 real nodes */
    { tree_t * fence_with = un(TT_FENCE, slit("if")); dump_pat("SNOBOL4:  FENCE('if')  [IR_PAT_LIT + IR_PAT_FENCE: inner.gamma->FENCE, FENCE bounded successor]", fence_with, 2); }
    dump_pat("SNOBOL4:  ABORT    [IR_PAT_ABORT: bounded, immediately fails entire match (SPITBOL ch.9)]",
         ast_node_new(TT_ABORT), 1);
    /* SUCCEED in pattern context: nalloc(IR_SUCCEED) + emit_leaf — same IR kind as the sentinel but a
       distinct newly-allocated node; resumable (kind_is_resumable returns 0 for IR_SUCCEED — NOT in the list)
       → bounded path: β=ω_in. So 1 real node, like any other bounded leaf. */
    dump_pat("SNOBOL4:  SUCCEED  [IR_SUCCEED: always succeeds; β=ω_in (not in kind_is_resumable)]",
         ast_node_new(TT_SUCCEED), 1);
    dump_pat("SNOBOL4:  FAIL(pat) [IR_FAIL: bounded, forces backtrack to seek alternatives (SPITBOL ch.9)]",
         ast_node_new(TT_FAIL), 1);
    /* ARBNO('ab'): outer graph has 1 IR_PAT_ARBNO; inner graph is a separate IR_alloc not counted here */
    dump_pat("SNOBOL4:  ARBNO('ab')  [IR_PAT_ARBNO: shy generator; inner sub-graph in separate IR_alloc]",
         un(TT_ARBNO, slit("ab")), 1);
    /* Conditional capture 'abc' . X: inner LIT + ASSIGN_COND capture node = 2 real nodes */
    { tree_t * cc = ast_node_new(TT_CAPT_COND_ASGN); ast_push(cc, slit("abc")); ast_push(cc, var("X"));
      dump_pat("SNOBOL4:  'abc' . X  [IR_PAT_LIT + IR_PAT_ASSIGN_COND: inner.gamma->CAP, CAP.sval=X]", cc, 2); }
    /* @P cursor capture: 1 IR_PAT_ATP node, resumable, sval=varname */
    { tree_t * atp = ast_node_new(TT_CAPT_CURSOR); ast_push(atp, var("P"));
      dump_pat("SNOBOL4:  @P  [IR_PAT_ATP: resumable cursor-capture; sval='P' (SPITBOL ch.9 @var)]", atp, 1); }
    /* *var deferred pattern (TT_DEFER, ival=1): 1 IR_PAT_DEFER, resumable */
    { tree_t * df = un(TT_DEFER, var("pat"));
      dump_pat("SNOBOL4:  *pat  [IR_PAT_DEFER ival=1: resumable; var holds pattern resolved at match time]", df, 1); }
    /* bare var in pattern context (TT_VAR, ival=0): 1 IR_PAT_DEFER */
    dump_pat("SNOBOL4:  VAR(token)  [IR_PAT_DEFER ival=0: bare var ref, resolved as string match at runtime]",
         var("token"), 1);
    dump_pat("SNOBOL4:  BAL  [IR_PAT_BAL: generator(resumable) β=self; shortest paren-balanced non-null (SPITBOL ch.18)]",
         ast_node_new(TT_BAL), 1);
    /* ===== END SNOBOL4 SECTION ===== */

    /* ===== ICON SECTION — APPEND ICON (VALUE-role) CASES BELOW THIS LINE ===== */
    /* (Icon's foundation/L2-A/B cases live above in the Proebsting block; new Icon proofs go here.) */
    /* ===== END ICON SECTION ===== */

    /* ===== PROLOG SECTION — APPEND PROLOG (GOAL-role) CASES BELOW THIS LINE ===== */
    dump_goal("Prolog:   write('hi')   [PLG-1 g_builtin: IR_BUILTIN(sval=write,ival=1) + IR_ATOM arg on alpha; pl_write, no auto-nl]",
         fnc1("write", slit("hi")), 2);
    dump_goal("Prolog:   nl   [PLG-1 g_builtin: bare IR_BUILTIN(sval=nl,ival=0) leaf; EXEC = putchar('\\n')]",
         slit("nl"), 1);
    dump_goal("Prolog:   (write(a) , write(b))   [GOAL conj = wire_seq(IR_GCONJ): same shape as Icon &/SNOBOL CAT]",
         gfnc2(",", fnc1("write", slit("a")), fnc1("write", slit("b"))), 5);
    dump_goal("Prolog:   (write(a) ; write(b))   [GOAL disj = wire_alt(IR_DISJ): same fail-chain as SNOBOL ALT]",
         gfnc2(";", fnc1("write", slit("a")), fnc1("write", slit("b"))), 5);
    dump_goal("Prolog:   X = Y   [g_unify: lhs.gamma->rhs.alpha->UNIFY, semidet resume->fail]",
         gfnc2("=", var("X"), var("Y")), 3);
    dump_goal("Prolog:   X < 5   [g_compare: IR_BUILTIN(sval=\"<\") bb->alpha=LOGICVAR(X) bb->beta=LIT_I(5); exec resolve_arith_eval both sides]",
         gfnc2("<", var("X"), lit(5)), 3);
    dump_goal("Prolog:   X is 5   [g_is: IR_BUILTIN(sval=is) bb->alpha=LOGICVAR(X) bb->beta=LIT_I(5); exec eval(rhs)->unify lhs]",
         gfnc2("is", var("X"), lit(5)), 3);
    dump_goal("Prolog:   X is 2+3   [g_is: IR_BUILTIN bb->alpha=LOGICVAR bb->beta=IR_BINOP(ADD,LIT_I,LIT_I); 5 nodes]",
         gfnc2("is", var("X"), bin(TT_ADD, lit(2), lit(3))), 5);
    /* ===== END PROLOG SECTION ===== */
    return 0;
}
