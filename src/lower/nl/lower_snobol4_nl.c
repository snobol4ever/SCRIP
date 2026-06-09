#include <string.h>
#include "ast.h"
#include "IR.h"
/*====================================================================================================================================================================================================*/
#define SNO_MAXSTMTS 2048
typedef struct {
    IR_graph_t * g;
    /* fixed terminal nodes */
    IR_t * PSUCC;   /* [0] */
    IR_t * PFAIL;   /* [1] */
    IR_t * PRET;    /* [2] RETURN  (target of :RETURN goto) */
    IR_t * PFRET;   /* [3] RETURN  (target of :FRETURN goto) */
    /* per-stmt label SUCCEED nodes */
    IR_t ** labels;
    int     nlabels;
    /* label name → stmt-index map */
    const char ** lname;
    int           lstmt[SNO_MAXSTMTS];
    int           nlmap;
} snx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { if (nd) { nd->γ.node = t; memcpy(nd->γ.sz, "α", 3); nd->γ.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { if (nd) { nd->ω.node = t; memcpy(nd->ω.sz, "β", 3); nd->ω.sz[3] = 0; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(snx_t * cx, IR_e op, IR_t * γ, IR_t * ω) {
    IR_t * nd = IR_node_alloc(cx->g, op); γ_to(nd, γ); ω_to(nd, ω); return nd; }
/*====================================================================================================================================================================================================*/
/* ── AST helpers ─────────────────────────────────────────────────── */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* extract first child of a named TT_ATTR inside TT_STMT */
static const tree_t * sno_attr(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag))
            return (a->n > 0) ? a->c[0] : NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_has_attr(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* extract goto child (TT_GOTO_S/F/U or TT_ATTR ":goS"/":goF"/":go") */
static const tree_t * sno_goto_node(const tree_t * s, tree_e kind, const char * tag) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (!a) continue;
        if (a->t == kind) return a;
        if (a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return a;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* get label name from TT_GOTO_* or ":goX" TT_ATTR child */
static const char * sno_goto_label(const tree_t * ch) {
    if (!ch || !ch->n || !ch->c[0]) return NULL;
    return ch->c[0]->v.sval;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* get stmt's own label name (if any) */
static const char * sno_stmt_label(const tree_t * s) {
    const tree_t * lbl = sno_attr(s, ":lbl");
    return lbl ? lbl->v.sval : NULL;
}
/*====================================================================================================================================================================================================*/
/* resolve goto label name → IR_t * SUCCEED/RETURN node */
static IR_t * resolve(snx_t * cx, const char * name) {
    if (!name || !name[0]) return cx->PSUCC;
    if (!strcmp(name, "END"))     return cx->PSUCC;
    if (!strcmp(name, "RETURN"))  return cx->PRET;
    if (!strcmp(name, "FRETURN")) return cx->PFRET;
    for (int i = 0; i < cx->nlmap; i++)
        if (cx->lname[i] && !strcmp(cx->lname[i], name)) return cx->labels[cx->lstmt[i]];
    return cx->PSUCC; /* unknown → terminal */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* "next" label for stmt at index i (i+1 or PSUCC if last) */
static IR_t * next_label(snx_t * cx, int i) {
    return (i + 1 < cx->nlabels) ? cx->labels[i + 1] : cx->PSUCC;
}
/*====================================================================================================================================================================================================*/
/* ── expression lowerer (for RHS of assignments and sub-exprs) ──── */
static int is_sno_binop(tree_e tt) {
    switch (tt) {
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE:
    case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: case TT_LEQ: case TT_LNE:
    case TT_CAT: case TT_SEQ: return 1;
    default: return 0;
    }
}
static int is_sno_unop(tree_e tt) {
    switch (tt) {
    case TT_MNS: case TT_PLS: case TT_NONNULL: case TT_NOT: case TT_INTERROGATE:
    case TT_NAME: case TT_INDIRECT: return 1;
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_expr(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt, IR_t ** res);
static IR_t * lower_expr(snx_t * cx, const tree_t * t, IR_t * cont, IR_t * nxt, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, cont, nxt); *res = s; return s; }
    if (is_sno_binop(t->t)) {
        IR_t * op = build(cx, IR_BINOP, cont, nxt);
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
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, cont, nxt); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, cont, nxt); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_S, cont, nxt); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_NUL:  { IR_t * nd = build(cx, IR_LIT_NUL, cont, nxt); *res = nd; return nd; }
    case TT_VAR:  { IR_t * nd = build(cx, IR_VAR, cont, nxt); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_KEYWORD: { IR_t * nd = build(cx, IR_KEYWORD, cont, nxt); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_FNC: {
        const char * nm = (t->n > 0 && t->c[0]) ? t->c[0]->v.sval : "?";
        IR_t * nd = build(cx, IR_CALL, cont, nxt); IR_LIT(nd).sval = nm; IR_LIT(nd).ival = t->n - 1;
        *res = nd; return nd;
    }
    default: { IR_t * s = build(cx, IR_SUCCEED, cont, nxt); *res = s; return s; }
    }
}
/*====================================================================================================================================================================================================*/
/* ── assignment lowerer ─────────────────────────────────────────── */
/* lower V = EXPR; γ=ω=nxt for the result node (assignment always succeeds in SNOBOL4) */
static IR_t * lower_assign(snx_t * cx, const char * lhs, const tree_t * rhs, IR_t * nxt) {
    if (!rhs) {
        /* empty assignment  V =  (SNOBOL4 null replacement) */
        IR_t * asn = build(cx, IR_ASSIGN_LIT_S, nxt, nxt); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_S, asn, nxt); IR_LIT(lit).sval = "";
        return lit;
    }
    switch (rhs->t) {
    case TT_QLIT: {
        IR_t * asn = build(cx, IR_ASSIGN_LIT_S, nxt, nxt); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_S, asn, nxt); IR_LIT(lit).sval = rhs->v.sval;
        return lit;
    }
    case TT_ILIT: {
        IR_t * asn = build(cx, IR_ASSIGN_LIT_I, nxt, nxt); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_I, asn, nxt); IR_LIT(lit).ival = rhs->v.ival;
        return lit;
    }
    case TT_FLIT: {
        IR_t * asn = build(cx, IR_ASSIGN_LIT_I, nxt, nxt); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_F, asn, nxt); IR_LIT(lit).dval = rhs->v.dval;
        return lit;
    }
    case TT_NUL: {
        IR_t * asn = build(cx, IR_ASSIGN_LIT_S, nxt, nxt); IR_LIT(asn).sval = (char *) lhs;
        IR_t * lit = build(cx, IR_LIT_NUL, asn, nxt);
        return lit;
    }
    case TT_VAR: {
        IR_t * asn = build(cx, IR_ASSIGN_VAR, nxt, nxt); IR_LIT(asn).sval = (char *) lhs;
        IR_t * var = build(cx, IR_VAR, asn, nxt); IR_LIT(var).sval = rhs->v.sval;
        return var;
    }
    default: {
        /* complex expr: ASSIGN + expr chain */
        IR_t * asn = build(cx, IR_ASSIGN, nxt, nxt); IR_LIT(asn).sval = (char *) lhs;
        return lower_expr(cx, rhs, asn, nxt, NULL);
    }
    }
}
/*====================================================================================================================================================================================================*/
/* ── statement body lowerer ──────────────────────────────────────── */
/* Returns entry node for this stmt body (or NULL if no body / pure goto). */
static IR_t * lower_stmt_body(snx_t * cx, const tree_t * s, IR_t * γ_tgt, IR_t * ω_tgt) {
    const tree_t * subj = sno_attr(s, ":subj");
    if (!subj) return NULL; /* empty stmt or pure-goto: no body */
    int has_eq = sno_has_attr(s, ":eq");
    if (has_eq) {
        /* assignment:  LHS = RHS */
        const tree_t * repl = sno_attr(s, ":repl");
        const char * lhs = (subj->t == TT_VAR) ? subj->v.sval : "?";
        /* for assignment, both γ_tgt and ω_tgt are the same (next); use γ_tgt */
        return lower_assign(cx, lhs, repl, γ_tgt);
    }
    /* expression statement (call, scan, etc.) */
    switch (subj->t) {
    case TT_FNC: {
        const char * nm = (subj->n > 0 && subj->c[0]) ? subj->c[0]->v.sval : "?";
        IR_t * nd = build(cx, IR_CALL, γ_tgt, ω_tgt);
        IR_LIT(nd).sval = nm; IR_LIT(nd).ival = subj->n - 1;
        return nd;
    }
    case TT_SCAN: {
        /* SCAN: stub – emit IR_SCAN with subgraph; pattern graph not built here (LAD-3c) */
        const tree_t * scan_var = (subj->n > 0) ? subj->c[0] : NULL;
        IR_t * nd = build(cx, IR_SCAN, γ_tgt, ω_tgt);
        IR_LIT(nd).sval = (scan_var && scan_var->v.sval) ? scan_var->v.sval : "";
        return nd;
    }
    default:
        return lower_expr(cx, subj, γ_tgt, ω_tgt, NULL);
    }
}
/*====================================================================================================================================================================================================*/
IR_graph_t * lower_snobol4(const tree_t * prog) {
    /* ── 1. collect non-end stmts ─────────────────────────────────── */
    const tree_t * stmts[SNO_MAXSTMTS];
    int N = 0;
    for (int i = 0; i < prog->n && N < SNO_MAXSTMTS - 1; i++) {
        const tree_t * s = prog->c[i];
        if (!s || sno_is_end(s)) continue;
        stmts[N++] = s;
    }
    /* ── 2. allocate graph + fixed prefix nodes ────────────────────── */
    IR_graph_t * g = IR_alloc(8192, IR_LANG_SNO);
    snx_t cx_s; snx_t * cx = &cx_s; memset(cx, 0, sizeof *cx); cx->g = g;
    cx->PSUCC = IR_node_alloc(g, IR_SUCCEED);   /* [0] terminal success */
    cx->PFAIL = IR_node_alloc(g, IR_FAIL);      /* [1] terminal failure */
    cx->PRET  = IR_node_alloc(g, IR_RETURN);    /* [2] :RETURN target   */
    cx->PFRET = IR_node_alloc(g, IR_RETURN);    /* [3] :FRETURN target  */
    ω_to(cx->PRET,  cx->PFAIL);
    ω_to(cx->PFRET, cx->PFAIL);
    /* ── 3. label SUCCEED nodes [4..4+N-1] ───────────────────────── */
    IR_t * lbuf[SNO_MAXSTMTS];
    cx->labels  = lbuf;
    cx->nlabels = N;
    for (int i = 0; i < N; i++) lbuf[i] = IR_node_alloc(g, IR_SUCCEED);
    /* ── 4. build label name → stmt-index map ─────────────────────── */
    const char * lname_buf[SNO_MAXSTMTS];
    cx->lname = lname_buf;
    for (int i = 0; i < N; i++) {
        const char * nm = sno_stmt_label(stmts[i]);
        if (nm && nm[0]) {
            cx->lname[cx->nlmap] = nm;
            cx->lstmt[cx->nlmap] = i;
            cx->nlmap++;
        }
    }
    /* ── 5. entry ───────────────────────────────────────────────────── */
    g->entry = (N > 0) ? lbuf[0] : cx->PSUCC;
    /* ── 6. lower each stmt ─────────────────────────────────────────── */
    for (int i = 0; i < N; i++) {
        const tree_t * s = stmts[i];
        /* ── determine goto targets ─── */
        const tree_t * go_s = sno_goto_node(s, TT_GOTO_S, ":goS");
        const tree_t * go_f = sno_goto_node(s, TT_GOTO_F, ":goF");
        const tree_t * go_u = sno_goto_node(s, TT_GOTO_U, ":go");
        IR_t * nxt = next_label(cx, i);
        IR_t * γ_tgt, * ω_tgt;
        if (go_u) {
            γ_tgt = ω_tgt = resolve(cx, sno_goto_label(go_u));
        } else {
            γ_tgt = go_s ? resolve(cx, sno_goto_label(go_s)) : nxt;
            ω_tgt = go_f ? resolve(cx, sno_goto_label(go_f)) : nxt;
        }
        /* ── lower stmt body ─────── */
        IR_t * entry = lower_stmt_body(cx, s, γ_tgt, ω_tgt);
        if (entry) {
            γ_to(lbuf[i], entry);
        } else {
            /* no body: label SUCCEED points directly to goto target */
            γ_to(lbuf[i], γ_tgt);
        }
    }
    return g;
}
