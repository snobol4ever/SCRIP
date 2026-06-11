/*====================================================================================================================*/
/*====================================================================================================================*/
#define BB_DEFINE_NAMES
#include "IR.h"
#include "ast.h"
#include "stage2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
stage2_t g_stage2;
/*--------------------------------------------------------------------------------------------------------------------*/
int is_global(const char * name) { (void) name; return 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
extern IR_t * lower_value_entry(IR_graph_t * bbg, const tree_t * e, IR_t * g, IR_t * w, IR_t ** a, IR_t ** b);
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t * lit(long long v) { tree_t * n = ast_node_new(TT_ILIT); n->v.ival = v; return n; }
static tree_t * bin(tree_e op, tree_t * a, tree_t * b) { tree_t * n = ast_node_new(op); ast_push(n, a); ast_push(n, b); return n; }
static tree_t * un(tree_e op, tree_t * a) { tree_t * n = ast_node_new(op); ast_push(n, a); return n; }
static tree_t * tri(tree_e op, tree_t * a, tree_t * b, tree_t * c) { tree_t * n = ast_node_new(op); ast_push(n, a); ast_push(n, b); ast_push(n, c); return n; }
static tree_t * slit(const char * s) { tree_t * n = ast_node_new(TT_QLIT); n->v.sval = (char *) s; return n; }
static tree_t * var(const char * s) { tree_t * n = ast_node_new(TT_VAR); n->v.sval = (char *) s; return n; }
static tree_t * kw(const char * s) { tree_t * n = ast_node_new(TT_KEYWORD); n->v.sval = (char *) s; return n; }
static tree_t * jct(const char * flav, tree_t * m0, tree_t * m1, tree_t * m2) {
    tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) flav;
    ast_push(n, var(flav));
    if (m0) ast_push(n, m0); if (m1) ast_push(n, m1); if (m2) ast_push(n, m2);
    return n;
}
static tree_t * rkfnc(const char * name, tree_t * a0, tree_t * a1, tree_t * a2) {
    tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) name;
    ast_push(n, var(name));
    if (a0) ast_push(n, a0); if (a1) ast_push(n, a1); if (a2) ast_push(n, a2);
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int idx_of(IR_graph_t * g, IR_t * n) { if (!n) return -1; for (int i = 0; i < g->n; i++) if (g->all[i] == n) return i; return -2; }
static const char * kname(IR_e t) {
    switch (t) {
    case IR_LIT_I: return "LIT_I"; case IR_BINOP: return "BINOP"; case IR_TO: return "TO"; case IR_TO_BY: return "TO_BY"; case IR_VAR: return "VAR";
    case IR_UNOP: return "UNOP"; case IR_IF: return "IF"; case IR_SUCCEED: return "PSUCC"; case IR_FAIL: return "PFAIL";
    case IR_CONJ: return "CONJ"; case IR_ALT: return "ALT"; case IR_EVERY: return "EVERY"; case IR_WHILE: return "WHILE";
    case IR_UNTIL: return "UNTIL"; case IR_REPEAT: return "REPEAT"; case IR_NOT: return "NOT";
    case IR_ASSIGN: return "ASGN"; case IR_CALL: return "CALL"; case IR_LIT_S: return "LIT_S";
    case IR_ASSIGN_LIT_S: return "ASGN_LS"; case IR_ASSIGN_LIT_I: return "ASGN_LI";
    case IR_ASSIGN_VAR: return "ASGN_V"; case IR_ASSIGN_CONCAT: return "ASGN_C"; case IR_ASSIGN_CALL: return "ASGN_K";
    case IR_PAT_LIT: return "PLIT"; case IR_PAT_REM: return "PREM"; case IR_PAT_ARB: return "PARB";
    case IR_PAT_SPAN: return "PSPAN"; case IR_PAT_SPAN_VAR: return "PSPANV"; case IR_PAT_ANY: return "PANY"; case IR_PAT_NOTANY: return "PNANY"; case IR_PAT_BREAK: return "PBRK"; case IR_PAT_BREAKX: return "PBRKX";
    case IR_PAT_CAT: return "PCAT"; case IR_PAT_ALT: return "PALT";
    case IR_PAT_LEN: return "PLEN"; case IR_PAT_POS: return "PPOS"; case IR_PAT_TAB: return "PTAB"; case IR_PAT_RTAB: return "PRTAB";
    case IR_PAT_FENCE: return "PFNC"; case IR_PAT_ABORT: return "PABT"; case IR_PAT_ARBNO: return "PARBN";
    case IR_PAT_ASSIGN_COND: return "PCAP"; case IR_PAT_ASSIGN_IMM: return "PCAPI"; case IR_PAT_ATP: return "PATP";
    case IR_PAT_DEFER: return "PDEF"; case IR_PAT_BAL: return "PBAL";
    case IR_GCONJ: return "GCONJ"; case IR_DISJ: return "DISJ"; case IR_UNIFY: return "UNIFY"; case IR_ARITH: return "ARITH"; case IR_CUT: return "CUT";
    case IR_BUILTIN: return "BLTIN"; case IR_ATOM: return "ATOM"; case IR_STRUCT: return "STRCT"; case IR_LOGICVAR: return "LVAR";
    case IR_ITE: return "ITE";
    case IR_GATHER: return "GTHR";
    case IR_SUBJECT: return "SUBJ";
    case IR_REF_INVARIANT: return "REFINV";
    case IR_PAT_MATCH: return "PATMAT";
    default: return "?";
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void dump(const char * title, tree_t * ast, int expect_nodes) {
    IR_graph_t * g = IR_alloc(64, 0);
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t * a = NULL, * b = NULL;
    IR_t * top = lower_value_entry(g, ast, PSUCC, PFAIL, &a, &b);
    printf("=== %s ===\n", title);
    printf("principal idx=%d  α(start)=%d  β(resume)=%d  node_count=%d  (2 sentinels PSUCC=0 PFAIL=1)\n", idx_of(g, top), idx_of(g, a), idx_of(g, b), g->n);
    printf("idx  kind    α    β    γ    ω      ival  dval\n");
    for (int i = 0; i < g->n; i++) {
        IR_t * n = g->all[i];
        printf("%3d  %-6s %3d  %3d  %3d  %3d  %8lld  %.1f\n", i, kname(n->op), idx_of(g, ((IR_t*)0)), idx_of(g, ((IR_t*)0)), idx_of(g, n->γ.node), idx_of(g, n->ω.node), (long long) IR_LIT(n).ival, IR_LIT(n).dval);
    }
    int real = g->n - 2;
    printf("real(non-sentinel) IR nodes = %d ; expected = %d ; %s\n\n", real, expect_nodes, real == expect_nodes ? "PASS" : "FAIL");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void dump_raku_value(const char * title, tree_t * ast, int expect_nodes) {
    IR_graph_t * g = IR_alloc(64, IR_LANG_RKU);
    IR_t * PSUCC = IR_node_alloc(g, IR_SUCCEED);
    IR_t * PFAIL = IR_node_alloc(g, IR_FAIL);
    IR_t * a = NULL, * b = NULL;
    IR_t * top = lower_value_entry(g, ast, PSUCC, PFAIL, &a, &b);
    printf("=== %s ===\n", title);
    printf("principal idx=%d  α(start)=%d  β(resume)=%d  node_count=%d  (2 sentinels PSUCC=0 PFAIL=1)\n", idx_of(g, top), idx_of(g, a), idx_of(g, b), g->n);
    printf("idx  kind    α    β    γ    ω      ival  dval\n");
    for (int i = 0; i < g->n; i++) {
        IR_t * n = g->all[i];
        printf("%3d  %-6s %3d  %3d  %3d  %3d  %8lld  %.1f\n", i, kname(n->op), idx_of(g, ((IR_t*)0)), idx_of(g, ((IR_t*)0)), idx_of(g, n->γ.node), idx_of(g, n->ω.node), (long long) IR_LIT(n).ival, IR_LIT(n).dval);
    }
    int real = g->n - 2;
    printf("real(non-sentinel) IR nodes = %d ; expected = %d ; %s\n\n", real, expect_nodes, real == expect_nodes ? "PASS" : "FAIL");
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
    dump("(1 to 2) & (3 to 4)   [ir_conjunction: c0.gamma->c1.alpha, c1.omega->c0.beta, resume=c1.beta]",
         bin(TT_SEQ, bin(TT_TO, lit(1), lit(2)), bin(TT_TO, lit(3), lit(4))), 7);
    dump("1 | 2 | 3   [ir_a_Alt: arm.gamma->alt, fail-chain arm[i].omega->arm[i+1].alpha, last->omega]",
         tri(TT_ALTERNATE, lit(1), lit(2), lit(3)), 4);
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
    dump_raku_value("RAKU:     say('hello world')   [RK-LOWER-0 wire_det_builtin1: arg.gamma->CALL, CALL.sval=write (say=newline), det; arg IR_LIT_S]",
         un(TT_SAY, slit("hello world")), 2);
    dump_raku_value("RAKU:     print('hi')   [RK-LOWER-0 wire_det_builtin1: CALL.sval=writes (print=no newline), det; arg IR_LIT_S]",
         un(TT_PRINT, slit("hi")), 2);
    dump_raku_value("RAKU:     for 1..3 { say $_ }   [RK-LOWER-1 v_raku_for: gen.gamma->ASGN(_), bind.gamma->body, body.gamma->gen.beta (re-pump), gen.omega->done]",
         bin(TT_EVERY, un(TT_ITERATE, bin(TT_TO, lit(1), lit(3))), un(TT_SAY, var("_"))), 6);
    {
        tree_t * sq = ast_node_new(TT_SEQ_EXPR);
        ast_push(sq, un(TT_SUSPEND, lit(10)));
        ast_push(sq, un(TT_SUSPEND, lit(20)));
        ast_push(sq, un(TT_SUSPEND, lit(30)));
        tree_t * it = ast_node_new(TT_ITERATE); it->v.sval = "v"; ast_push(it, un(TT_GATHER, sq));
        dump_raku_value("RAKU:     for gather { take 10; take 20; take 30 } -> $v { say $v }   [RK-LOWER-2 v_raku_gather: IR_GATHER β=self (own resume), gen.gamma->ASGN(v), gen.omega->done; takes in sub-graphs]",
             bin(TT_EVERY, it, un(TT_SAY, var("v"))), 4);
    }
    {
        tree_t * mp = ast_node_new(TT_MAP);
        ast_push(mp, bin(TT_MUL, var("_"), lit(2)));
        ast_push(mp, bin(TT_TO, lit(1), lit(3)));
        tree_t * it = ast_node_new(TT_ITERATE); it->v.sval = "v"; ast_push(it, mp);
        dump_raku_value("RAKU:     for map { $_ * 2 } 1..3 -> $v { say $v }   [RK-LOWER-3 v_raku_map_grep: IR_MAP β=self (own resume), gen.gamma->ASGN(v), gen.omega->done; source+closure in sub-graphs]",
             bin(TT_EVERY, it, un(TT_SAY, var("v"))), 4);
    }
    {
        tree_t * gp = ast_node_new(TT_GREP);
        ast_push(gp, bin(TT_GT, var("_"), lit(2)));
        ast_push(gp, bin(TT_TO, lit(1), lit(5)));
        tree_t * it = ast_node_new(TT_ITERATE); it->v.sval = "v"; ast_push(it, gp);
        dump_raku_value("RAKU:     for grep { $_ > 2 } 1..5 -> $v { say $v }   [RK-LOWER-3 v_raku_map_grep: IR_GREP β=self (own resume), keep-on-truthy filter, gen.omega->done; pred+source in sub-graphs]",
             bin(TT_EVERY, it, un(TT_SAY, var("v"))), 4);
    }
    dump_raku_value("RAKU:     any(1, 2, 3)   [RK-LOWER-4: IR_CALL __rk_jct_any dval=2.0, det (β=ω); 3 members in isolated value sub-graphs]",
         jct("any", lit(1), lit(2), lit(3)), 1);
    dump_raku_value("RAKU:     any(1, all(5, 5))   [RK-LOWER-4 nested: outer IR_CALL __rk_jct_any (1 principal); inner all(5,5) is one opaque member sub-graph]",
         jct("any", lit(1), jct("all", lit(5), lit(5), NULL), NULL), 1);
    dump_raku_value("RAKU:     (1,2,3) => __rk_arr(1,2,3)   [RK-LOWER-5a: IR_CALL __rk_arr dval=2.0, det; 3 elems in isolated value sub-graphs]",
         rkfnc("__rk_arr", lit(1), lit(2), lit(3)), 1);
    dump_raku_value("RAKU:     @a[1]   [RK-LOWER-5a TT_ARR_GET -> IR_CALL arr_get dval=2.0, det; var+idx in sub-graphs]",
         bin(TT_ARR_GET, var("a"), lit(1)), 1);
    dump_raku_value("RAKU:     sort(@a)   [RK-LOWER-5a TT_SORT -> IR_CALL array_sort dval=2.0, det; @a in sub-graph]",
         un(TT_SORT, var("a")), 1);
    dump_raku_value("RAKU:     elems(@a)   [RK-LOWER-5a TT_FNC whitelist -> IR_CALL elems dval=2.0, det; @a in sub-graph]",
         rkfnc("elems", var("a"), NULL, NULL), 1);
    return 0;
}
