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
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    printf("prove_lower: 0 cases — DEAD GATE pending NL-shaped prove-case authoring (IR-REDESIGN ratify; old lower_value_entry cases deleted with lower.c, 662f249 mirror)\n");
    return 0;
}
