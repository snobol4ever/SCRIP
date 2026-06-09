#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "ast.h"
#include "IR.h"
/*====================================================================================================================================================================================================*/
#define SNO_MAXSTMTS 2048
typedef struct {
    IR_graph_t * g;
    IR_t * PSUCC;  /* [0] */
    IR_t * PFAIL;  /* [1] */
    IR_t * PRET;   /* [2] :RETURN  */
    IR_t * PFRET;  /* [3] :FRETURN */
    IR_t ** labels;
    int     nlabels;
    const char ** lname;
    int           lstmt[SNO_MAXSTMTS];
    int           nlmap;
} snx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "α", 3); nd->γ.sz[3] = 0; } }
static void ω_to(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "β", 3); nd->ω.sz[3] = 0; } }
static IR_t * build(snx_t * cx, IR_e op, IR_t * γ, IR_t * ω) {
    IR_t * nd = IR_node_alloc(cx->g, op); γ_to(nd, γ); ω_to(nd, ω); return nd; }
/*====================================================================================================================================================================================================*/
/* ── AST helpers ──────────────────────────────────────────────────── */
static int sno_is_end(const tree_t * s) {
    if (!s) return 0;
    if (s->t == TT_END) return 1;
    if (s->t != TT_STMT) return 0;
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":end")) return 1;
    }
    return 0;
}
static const tree_t * sno_attr(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag))
            return (a->n > 0) ? a->c[0] : NULL;
    }
    return NULL;
}
static int sno_has_attr(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return 1;
    }
    return 0;
}
static const tree_t * sno_goto_node(const tree_t * s, tree_e kind, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i]; if (!a) continue;
        if (a->t == kind) return a;
        if (a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return a;
    }
    return NULL;
}
static const char * sno_goto_label(const tree_t * ch) {
    if (!ch || !ch->n || !ch->c[0]) return NULL; return ch->c[0]->v.sval; }
static const char * sno_stmt_label(const tree_t * s) {
    const tree_t * lbl = sno_attr(s, ":lbl"); return lbl ? lbl->v.sval : NULL; }
/*====================================================================================================================================================================================================*/
/* resolve label name → IR_t * (NULL if unknown; caller uses nxt as fallback) */
static IR_t * resolve(snx_t * cx, const char * name) {
    if (!name || !name[0]) return NULL;
    if (!strcmp(name, "END"))     return cx->PSUCC;
    if (!strcmp(name, "RETURN"))  return cx->PRET;
    if (!strcmp(name, "FRETURN")) return cx->PFRET;
    for (int i = 0; i < cx->nlmap; i++)
        if (cx->lname[i] && !strcmp(cx->lname[i], name)) return cx->labels[cx->lstmt[i]];
    return NULL;  /* unknown → caller uses nxt */
}
static IR_t * next_label(snx_t * cx, int i) {
    return (i + 1 < cx->nlabels) ? cx->labels[i + 1] : cx->PSUCC; }
/*====================================================================================================================================================================================================*/
/* ── binop helpers ─────────────────────────────────────────────────── */
static int is_sno_binop(tree_e tt) {
    switch (tt) {
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE:
    case TT_CAT: return 1;
    default: return 0; }
}
static int sno_binop_code(tree_e tt) {
    switch (tt) {
    case TT_ADD: return 0; case TT_SUB: return 1; case TT_MUL: return 2;
    case TT_DIV: return 3; case TT_MOD: return 4; case TT_POW: return 18;
    case TT_LT:  return 5; case TT_LE:  return 6; case TT_GT:  return 7;
    case TT_GE:  return 8; case TT_EQ:  return 9; case TT_NE:  return 10;
    case TT_CAT: return 11;
    case TT_LLT: return 12; case TT_LLE: return 13; case TT_LGT: return 14;
    case TT_LGE: return 15; case TT_LEQ: return 16; case TT_LNE: return 17;
    default: return 0; }
}
static int is_sno_unop(tree_e tt) {
    switch (tt) {
    case TT_MNS: case TT_PLS: case TT_NONNULL: case TT_NOT: case TT_INTERROGATE:
    case TT_NAME: case TT_INDIRECT: return 1;
    default: return 0; }
}
/*====================================================================================================================================================================================================*/
/* ── expression lowerer ─────────────────────────────────────────────── */
static IR_t * lower_expr(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt, IR_t ** res);
static IR_t * lower_expr(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, cont, nxt); *res = s; return s; }
    if (is_sno_binop(t->t)) {
        IR_t * op = build(cx, IR_BINOP, cont, nxt); IR_LIT(op).ival = sno_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL;
        IR_t * ea = lower_expr(cx, t->c[0], NULL, nxt, &lr);
        IR_t * eb = lower_expr(cx, t->c[1], op, nxt, &rr);
        γ_to(lr, eb); *res = op; return ea;
    }
    if (is_sno_unop(t->t)) {
        IR_t * op = build(cx, IR_UNOP, cont, nxt); IR_LIT(op).ival = (long long) t->t;
        IR_t * orr = NULL; IR_t * ea = lower_expr(cx, t->c[0], op, nxt, &orr);
        *res = op; return ea;
    }
    switch (t->t) {
    case TT_ILIT:    { IR_t * nd = build(cx, IR_LIT_I,   cont, nxt); IR_LIT(nd).ival  = t->v.ival; *res = nd; return nd; }
    case TT_FLIT:    { IR_t * nd = build(cx, IR_LIT_F,   cont, nxt); IR_LIT(nd).dval  = t->v.dval; *res = nd; return nd; }
    case TT_QLIT:    { IR_t * nd = build(cx, IR_LIT_S,   cont, nxt); IR_LIT(nd).sval  = t->v.sval; *res = nd; return nd; }
    case TT_NUL:     { IR_t * nd = build(cx, IR_LIT_NUL, cont, nxt);                                *res = nd; return nd; }
    case TT_VAR:     { IR_t * nd = build(cx, IR_VAR,     cont, nxt); IR_LIT(nd).sval  = t->v.sval; *res = nd; return nd; }
    case TT_KEYWORD: { IR_t * nd = build(cx, IR_KEYWORD, cont, nxt); IR_LIT(nd).sval  = t->v.sval; *res = nd; return nd; }
    case TT_FNC:     {
        const char * nm = t->v.sval ? t->v.sval : "?";
        IR_t * nd = build(cx, IR_CALL, cont, nxt); IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long) t->n;
        *res = nd; return nd;
    }
    default: { IR_t * s = build(cx, IR_SUCCEED, cont, nxt); *res = s; return s; }
    }
}
/*====================================================================================================================================================================================================*/
/* ── pattern sub-graph builder ──────────────────────────────────────── */
static IR_t * lower_pat_node(IR_graph_t * pg, const tree_t * t, IR_t * succ, IR_t * fail);
static IR_t * lower_pat_node(IR_graph_t * pg, const tree_t * t, IR_t * succ, IR_t * fail) {
    if (!t) return succ;
    switch (t->t) {
    case TT_QLIT: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_LIT); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_VAR: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ARB: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ARB); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_REM: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_REM); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_BREAK: case TT_BREAKX: {
        IR_e op = (t->t == TT_BREAK) ? IR_PAT_BREAK : IR_PAT_BREAKX;
        IR_t * nd = IR_node_alloc(pg, op); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) IR_LIT(nd).sval = t->c[0]->v.sval; return nd; }
    case TT_SPAN: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_SPAN); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) IR_LIT(nd).sval = t->c[0]->v.sval; return nd; }
    case TT_ANY: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ANY); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) IR_LIT(nd).sval = t->c[0]->v.sval; return nd; }
    case TT_NOTANY: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_NOTANY); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) IR_LIT(nd).sval = t->c[0]->v.sval; return nd; }
    case TT_POS: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_POS); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
    case TT_RPOS: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_POS); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = "r"; if (t->n > 0 && t->c[0]) IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
    case TT_LEN: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_LEN); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
    case TT_TAB: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_TAB); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
    case TT_RTAB: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_RTAB); γ_to(nd, succ); ω_to(nd, fail);
        if (t->n > 0 && t->c[0]) IR_LIT(nd).ival = t->c[0]->v.ival; return nd; }
    case TT_ARBNO: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ARBNO); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_ALT: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ALT); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_CAPT_COND_ASGN: {  /* pat . var */
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ASSIGN_COND); γ_to(nd, succ); ω_to(nd, fail);
        const char * vn = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : "";
        IR_LIT(nd).sval = (char *) vn;
        IR_t * pe = (t->n > 0) ? lower_pat_node(pg, t->c[0], nd, fail) : nd;
        return pe; }
    case TT_CAPT_IMMED_ASGN: { /* pat $ var */
        IR_t * nd = IR_node_alloc(pg, IR_PAT_ASSIGN_IMM); γ_to(nd, succ); ω_to(nd, fail);
        const char * vn = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : "";
        IR_LIT(nd).sval = (char *) vn;
        IR_t * pe = (t->n > 0) ? lower_pat_node(pg, t->c[0], nd, fail) : nd;
        return pe; }
    case TT_SEQ: {  /* pattern concatenation */
        IR_t * cat = IR_node_alloc(pg, IR_PAT_CAT); γ_to(cat, succ); ω_to(cat, fail);
        IR_t * re = lower_pat_node(pg, (t->n > 1) ? t->c[1] : NULL, cat, fail);
        IR_t * le = lower_pat_node(pg, (t->n > 0) ? t->c[0] : NULL, re,  fail);
        return le; }
    case TT_FENCE: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_FENCE); γ_to(nd, succ); ω_to(nd, fail); return nd; }
    case TT_FNC: {  /* functional pattern node: use PAT_DEFER as fallback */
        IR_t * nd = IR_node_alloc(pg, IR_PAT_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval; return nd; }
    default: {
        IR_t * nd = IR_node_alloc(pg, IR_PAT_DEFER); γ_to(nd, succ); ω_to(nd, fail);
        IR_LIT(nd).sval = "?"; return nd; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_pat_graph(const tree_t * pat) {
    IR_graph_t * pg = IR_alloc(256, IR_LANG_SNO);
    IR_t * succ = IR_node_alloc(pg, IR_SUCCEED);  /* [0] */
    IR_t * fail = IR_node_alloc(pg, IR_FAIL);      /* [1] */
    IR_t * entry = lower_pat_node(pg, pat, succ, fail);
    pg->entry = entry ? entry : succ;
    return pg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * lower_subj_graph(const char * vname) {
    IR_graph_t * sg = IR_alloc(32, IR_LANG_SNO);
    IR_t * fail = IR_node_alloc(sg, IR_FAIL);   /* [0] */
    IR_t * var  = IR_node_alloc(sg, IR_VAR);    /* [1] */
    ω_to(var, fail); IR_LIT(var).sval = (char *) vname;
    sg->entry = var;
    return sg;
}
/*====================================================================================================================================================================================================*/
/* ── assignment lowerer ──────────────────────────────────────────────── */
static IR_t * lower_assign(snx_t * cx, const char * lhs, const tree_t * rhs, IR_t * γ, IR_t * ω, int is_kw) {
    if (!rhs) {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_LIT_S;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_S, asn, ω); IR_LIT(lit).sval = "";
        return lit;
    }
    switch (rhs->t) {
    case TT_QLIT: {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_LIT_S;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_S, asn, ω); IR_LIT(lit).sval = rhs->v.sval;
        return lit; }
    case TT_ILIT: {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_LIT_I;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_I, asn, ω); IR_LIT(lit).ival = rhs->v.ival;
        return lit; }
    case TT_FLIT: {
        IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_F, asn, ω); IR_LIT(lit).dval = rhs->v.dval;
        return lit; }
    case TT_NUL: {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_LIT_S;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_NUL, asn, ω); return lit; }
    case TT_VAR: {
        IR_e op = is_kw ? IR_ASSIGN : IR_ASSIGN_VAR;
        IR_t * asn = build(cx, op, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * var = build(cx, IR_VAR, asn, ω); IR_LIT(var).sval = rhs->v.sval;
        return var; }
    case TT_FNC: {
        /* V = func(...) → ASSIGN_CALL + CALL */
        const char * nm = rhs->v.sval ? rhs->v.sval : "?";
        IR_t * asn = build(cx, IR_ASSIGN_CALL, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        IR_t * call = build(cx, IR_CALL, asn, ω); IR_LIT(call).sval = nm; IR_LIT(call).ival = (long long) rhs->n;
        return call; }
    case TT_SEQ: {
        const tree_t * leaves[64]; int nl = 0, fold = 1;
        const tree_t * stk[128]; int sp = 0;
        stk[sp++] = rhs;
        while (sp > 0 && fold) {
            const tree_t * nd = stk[--sp]; if (!nd) { fold = 0; break; }
            if (nd->t == TT_SEQ) {
                if (sp + 2 > 127) { fold = 0; break; }
                stk[sp++] = (nd->n > 1) ? nd->c[1] : NULL;
                stk[sp++] = (nd->n > 0) ? nd->c[0] : NULL;
            } else if (nd->t == TT_QLIT && nl < 63) { leaves[nl++] = nd; }
            else { fold = 0; }
        }
        IR_t * asn = build(cx, IR_ASSIGN_CONCAT, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        if (fold && nl > 0) {
            int total = 0;
            for (int i = 0; i < nl; i++) if (leaves[i]->v.sval) total += (int)strlen(leaves[i]->v.sval);
            char * buf = (char *) malloc(total + 1); buf[0] = 0;
            for (int i = 0; i < nl; i++) if (leaves[i]->v.sval) strcat(buf, leaves[i]->v.sval);
            IR_t * lit = build(cx, IR_LIT_S, asn, ω); IR_LIT(lit).sval = buf;
            return lit;
        }
        IR_t * seq = build(cx, IR_SEQ, asn, ω); IR_LIT(seq).ival = 100000000LL;
        return seq; }
    default: {
        /* TT_IDX or TT_INDIRECT as RHS: oracle emits orphan ASSIGN (no γ/ω), label chains to nxt */
        if (rhs->t == TT_IDX || rhs->t == TT_INDIRECT) {
            IR_t * asn = IR_node_alloc(cx->g, IR_ASSIGN); IR_LIT(asn).sval = (char *) lhs;
            return NULL;
        }
        IR_t * asn = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = (char *) lhs;
        return lower_expr(cx, rhs, asn, ω, NULL); }
    }
}
/*====================================================================================================================================================================================================*/
/* ── statement body lowerer ──────────────────────────────────────────── */
static IR_t * lower_stmt_body(snx_t * cx, const tree_t * s, IR_t * γ_tgt, IR_t * ω_tgt) {
    const tree_t * subj = sno_attr(s, ":subj");
    if (!subj) return NULL;
    int has_eq = sno_has_attr(s, ":eq");
    if (has_eq) {
        /* assignment:  LHS = RHS  (plain VAR or KEYWORD LHS only) */
        if (subj->t != TT_VAR && subj->t != TT_KEYWORD) return NULL;
        const tree_t * repl = sno_attr(s, ":repl");
        const char * lhs = subj->v.sval;
        int is_kw = (subj->t == TT_KEYWORD) ? 1 : 0;
        return lower_assign(cx, lhs, repl, γ_tgt, ω_tgt, is_kw);
    }
    switch (subj->t) {
    case TT_FNC: {
        const char * nm = subj->v.sval ? subj->v.sval : "?";
        /* orphan CALL when any arg is TT_IDX or TT_INDIRECT (oracle behaviour) */
        int complex_arg = 0;
        for (int ai = 0; ai < subj->n && !complex_arg; ai++) {
            const tree_t * a = subj->c[ai];
            if (a && (a->t == TT_IDX || a->t == TT_INDIRECT)) complex_arg = 1;
        }
        if (complex_arg) {
            IR_t * nd = IR_node_alloc(cx->g, IR_CALL);
            IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long) subj->n;
            return NULL;  /* orphan: no γ/ω; label chains to nxt */
        }
        IR_t * nd = build(cx, IR_CALL, γ_tgt, ω_tgt);
        IR_LIT(nd).sval = nm; IR_LIT(nd).ival = (long long) subj->n;
        return nd; }
    case TT_SCAN: {
        /* TT_SCAN children: c[0]=subject expr, c[1]=pattern expr */
        const tree_t * sv = (subj->n > 0) ? subj->c[0] : NULL;
        const tree_t * pt = (subj->n > 1) ? subj->c[1] : NULL;
        const char * vname = (sv && sv->v.sval) ? sv->v.sval : "";
        /* allocate sub-graphs */
        IR_graph_t * pg = lower_pat_graph(pt);
        IR_graph_t * sg = lower_subj_graph(vname);
        /* emit SCAN node + VAR entry in main graph */
        IR_t * scan = build(cx, IR_SCAN, γ_tgt, ω_tgt);
        IR_LIT(scan).sval = (char *) vname;
        IR_EXEC(scan).counter = (int64_t)(intptr_t) pg;
        ir_operand_push(scan, (IR_t *)(void *) sg);
        /* VAR entry: γ→scan, ω→failure_target */
        IR_t * var = build(cx, IR_VAR, scan, ω_tgt);
        IR_LIT(var).sval = (char *) vname;
        return var; }
    default:
        return lower_expr(cx, subj, γ_tgt, ω_tgt, NULL);
    }
}
/*====================================================================================================================================================================================================*/
IR_graph_t * lower_snobol4(const tree_t * prog) {
    /* 1. collect non-end stmts */
    const tree_t * stmts[SNO_MAXSTMTS]; int N = 0;
    for (int i = 0; i < prog->n && N < SNO_MAXSTMTS - 1; i++) {
        const tree_t * s = prog->c[i]; if (!s || sno_is_end(s)) continue; stmts[N++] = s; }
    /* 2. allocate graph + fixed prefix */
    IR_graph_t * g = IR_alloc(8192, IR_LANG_SNO);
    snx_t cx_s; snx_t * cx = &cx_s; memset(cx, 0, sizeof *cx); cx->g = g;
    cx->PSUCC = IR_node_alloc(g, IR_SUCCEED);
    cx->PFAIL = IR_node_alloc(g, IR_FAIL);
    cx->PRET  = IR_node_alloc(g, IR_RETURN);
    cx->PFRET = IR_node_alloc(g, IR_RETURN);
    ω_to(cx->PRET, cx->PFAIL); ω_to(cx->PFRET, cx->PFAIL);
    /* 3. label SUCCEED nodes [4..4+N-1] */
    IR_t * lbuf[SNO_MAXSTMTS];
    cx->labels = lbuf; cx->nlabels = N;
    for (int i = 0; i < N; i++) lbuf[i] = IR_node_alloc(g, IR_SUCCEED);
    /* 4. label name → stmt-index map */
    const char * lname_buf[SNO_MAXSTMTS]; cx->lname = lname_buf;
    for (int i = 0; i < N; i++) {
        const char * nm = sno_stmt_label(stmts[i]);
        if (nm && nm[0]) { cx->lname[cx->nlmap] = nm; cx->lstmt[cx->nlmap] = i; cx->nlmap++; } }
    /* 5. entry */
    g->entry = (N > 0) ? lbuf[0] : cx->PSUCC;
    /* 6. lower each stmt */
    for (int i = 0; i < N; i++) {
        const tree_t * s = stmts[i];
        const tree_t * go_s = sno_goto_node(s, TT_GOTO_S, ":goS");
        const tree_t * go_f = sno_goto_node(s, TT_GOTO_F, ":goF");
        const tree_t * go_u = sno_goto_node(s, TT_GOTO_U, ":go");
        IR_t * nxt = next_label(cx, i);
        IR_t * go_tgt_u = go_u ? resolve(cx, sno_goto_label(go_u)) : NULL;
        IR_t * go_tgt_s = go_s ? resolve(cx, sno_goto_label(go_s)) : NULL;
        IR_t * go_tgt_f = go_f ? resolve(cx, sno_goto_label(go_f)) : NULL;
        IR_t * γ_tgt, * ω_tgt;
        if (go_u) { γ_tgt = ω_tgt = go_tgt_u ? go_tgt_u : nxt; }
        else { γ_tgt = go_tgt_s ? go_tgt_s : nxt; ω_tgt = go_tgt_f ? go_tgt_f : nxt; }
        IR_t * entry = lower_stmt_body(cx, s, γ_tgt, ω_tgt);
        γ_to(lbuf[i], entry ? entry : γ_tgt);
    }
    return g;
}
