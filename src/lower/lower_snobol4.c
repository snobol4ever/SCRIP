#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lower.h"
#include "bb_program.h"
#include "parser/icon/icon_lex.h"
extern void global_register(const char * name);
extern int stage2_proc_grow(stage2_t * s2);
typedef struct { IR_graph_t * g; IR_t * loop_exit; IR_t * loop_next; const char * result_name; IR_t * pat_fail; } scx_t;
#define SNO_DEF_MAX 128
#define SNO_DEF_NAMES_MAX 64
typedef struct { const char * fname; const char * entry; const char * result_name; const char * names[SNO_DEF_NAMES_MAX]; int nnames; } sno_def_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_fatal(const char * what, const char * detail) {
    fprintf(stderr, "FATAL lower_snobol4 (GZ#5 subset): %s%s%s. Pattern matching, EVAL and CODE are outside the landed subset (IR_MATCH_* family pending); see GOAL-SNOBOL4-BB.md.\n",
            what, detail ? ": " : "", detail ? detail : "");
    exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sfind(const tree_t * s, const char * tag) {
    for (int i = 0; i < s->n; i++) { const tree_t * a = s->c[i]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, tag)) return a; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sfind_str(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0 && a->c[0]) ? a->c[0]->v.sval : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * sfind_expr(const tree_t * s, const char * tag) { const tree_t * a = sfind(s, tag); return (a && a->n > 0) ? a->c[0] : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_reg_var(const char * nm) { if (nm && nm[0] && nm[0] != '&') global_register(lp_strdup(nm)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_binop_code(tree_e tt) {
    switch (tt) {
    case TT_ADD: return 0; case TT_SUB: return 1; case TT_MUL: return 2; case TT_DIV: return 3; case TT_POW: return 18; case TT_SEQ: return 11; case TT_CAT: return 11;
    default: return -1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_lower(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sco_branch(scx_t * cx, const tree_t * pg, IR_t * γ, IR_t * ω) {
    if (!pg) return γ;
    if (pg->t != TT_PROGRAM) { IR_t * r = NULL; return sx_lower(cx, pg, γ, ω, &r); }
    IR_t * entry = γ;
    for (int i = pg->n - 1; i >= 0; i--) {
        const tree_t * s = pg->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t * subj = lc_stmt_subj(s);
        if (!subj) continue;
        IR_t * r = NULL;
        entry = sx_lower(cx, subj, entry, entry, &r);
    }
    return entry;
}
static IR_t * sx_subscript_lv(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_binop(scx_t * cx, const tree_t * t, int code, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * op = lc_build(cx->g, IR_BINOP, γ, ω); IR_LIT(op).ival = code;
    IR_t * lr = NULL; IR_t * rr = NULL;
    IR_t * ea = sx_lower(cx, t->c[0], NULL, ω, &lr);
    IR_t * eb = sx_lower(cx, t->c[1], op, ω, &rr);
    lc_γ_to(lr, eb);
    ir_operand_push(op, lr); ir_operand_push(op, rr);
    if (res) *res = op; return ea;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_call_named(scx_t * cx, const char * name, const tree_t * t, int argbase, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * call = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(call).sval = (char *) lp_strdup(name);
    int nargs = t ? (t->n - argbase) : 0;
    IR_t * prev = NULL; IR_t * entry = call;
    for (int k = 0; k < nargs; k++) {
        IR_t * ar = NULL; IR_t * ae = sx_lower(cx, t->c[argbase + k], (k == nargs - 1) ? call : NULL, ω, &ar);
        if (k == 0) entry = ae;
        if (prev) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(call, ar);
    }
    if (res) *res = call; return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_nameval(scx_t * cx, const tree_t * inner, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$NAME";
    IR_t * nr = NULL; IR_t * ne = sx_lower(cx, inner, mk, ω, &nr);
    if (nr) ir_operand_push(mk, nr);
    if (res) *res = mk; return ne;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_lower(scx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (!t) { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = lc_build(cx->g, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; if (res) *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = lc_build(cx->g, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; if (res) *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; if (res) *res = nd; return nd; }
    case TT_NUL:  { IR_t * nd = lc_build(cx->g, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = (char *) ""; if (res) *res = nd; return nd; }
    case TT_VAR:  { sno_reg_var(t->v.sval); IR_t * nd = lc_build(cx->g, IR_VAR, γ, ω); IR_LIT(nd).sval = t->v.sval; if (res) *res = nd; return nd; }
    case TT_KEYWORD: { IR_t * nd = lc_build(cx->g, IR_KEYWORD_SNOBOL4, γ, ω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) ""; if (res) *res = nd; return nd; }
    case TT_NAME: {
        if (t->n < 1 || !t->c[0]) sno_fatal("name operator with no operand", NULL);
        if (t->c[0]->t == TT_VAR && t->c[0]->v.sval) {
            sno_reg_var(t->c[0]->v.sval);
            IR_t * mk = lc_build(cx->g, IR_CALL, γ, ω); IR_LIT(mk).sval = (char *) "SNO$NAME";
            IR_t * nl = lc_build(cx->g, IR_LIT_STRING, mk, ω); IR_LIT(nl).sval = t->c[0]->v.sval;
            ir_operand_push(mk, nl);
            if (res) *res = mk; return nl;
        }
        if (t->c[0]->t == TT_IDX && t->c[0]->n >= 2) {
            const tree_t * ix = t->c[0];
            IR_t * vr = NULL; IR_t * entry = sx_subscript_lv(cx, ix->c[0], (const tree_t * const *) &ix->c[1], ix->n - 1, ω, &vr);
            if (γ) lc_γ_to(vr, γ);
            if (res) *res = vr; return entry;
        }
        sno_fatal("name operator over this form is outside the landed subset", NULL);
    }
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_POW: case TT_SEQ: case TT_CAT:
        if (t->n < 2) sno_fatal("binary operator with missing operand", NULL);
        return sx_binop(cx, t, sno_binop_code(t->t), γ, ω, res);
    case TT_MNS: case TT_PLS: {
        if (t->n < 1) sno_fatal("unary operator with missing operand", NULL);
        IR_t * op = lc_build(cx->g, IR_UNOP, γ, ω); IR_LIT(op).ival = (long long) t->t;
        IR_t * ar = NULL; IR_t * ea = sx_lower(cx, t->c[0], op, ω, &ar);
        ir_operand_push(op, ar);
        if (res) *res = op; return ea;
    }
    case TT_INDIRECT: {
        if (t->n < 1) sno_fatal("indirect reference with no operand", NULL);
        if (t->c[0] && t->c[0]->t == TT_NAME) {
            IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
            IR_t * nr = NULL; IR_t * ne = sx_lower(cx, t->c[0], NULL, ω, &nr);
            lc_γ_to(nr, dr);
            ir_operand_push(dr, nr);
            if (res) *res = dr; return ne;
        }
        IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
        IR_t * vr = NULL; IR_t * ve = sx_nameval(cx, t->c[0], dr, ω, &vr);
        ir_operand_push(dr, vr);
        if (res) *res = dr; return ve;
    }
    case TT_IDX: {
        if (t->n < 2) sno_fatal("subscript with no index", NULL);
        IR_t * br = NULL; IR_t * entry = sx_lower(cx, t->c[0], NULL, ω, &br);
        IR_t * cur = br;
        for (int k = 1; k < t->n; k++) {
            IR_t * ir = NULL; IR_t * ie = sx_lower(cx, t->c[k], NULL, ω, &ir);
            lc_γ_to(cur, ie);
            IR_t * sub = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
            lc_γ_to(ir, sub);
            ir_operand_push(sub, cur); ir_operand_push(sub, ir);
            cur = sub;
        }
        IR_t * dr = lc_build(cx->g, IR_DEREF, γ, ω);
        lc_γ_to(cur, dr);
        ir_operand_push(dr, cur);
        if (res) *res = dr; return entry;
    }
    case TT_FNC: {
        const char * name = t->v.sval; int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        if (!name) sno_fatal("call with no resolvable name", NULL);
        if (!strcmp(name, "DEFINE")) sno_fatal("DEFINE in expression position is outside the landed subset (statement-level literal DEFINE only)", NULL);
        if (!strcmp(name, "EVAL") || !strcmp(name, "CODE")) sno_fatal("outside subset", name);
        return sx_call_named(cx, name, t, argbase, γ, ω, res);
    }
    case TT_WHILE: case TT_UNTIL: {
        const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * B = (t->n > 1) ? t->c[1] : NULL;
        if (!C) sno_fatal("loop with no condition", NULL);
        int is_until = (t->t == TT_UNTIL);
        IR_t * cr = NULL;
        IR_t * ce = is_until ? sx_lower(cx, C, γ, NULL, &cr) : sx_lower(cx, C, NULL, γ, &cr);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = ce;
        IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        if (cr) { if (is_until) { if (!cr->ω.node) lc_ω_to(cr, be); } else { if (!cr->γ.node) lc_γ_to(cr, be); } }
        if (res) *res = NULL;
        return ce;
    }
    case TT_DO_WHILE: {
        const tree_t * B = (t->n > 0) ? t->c[0] : NULL; const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
        if (!C) sno_fatal("do-while without condition outside the landed subset", NULL);
        IR_t * cr = NULL;
        IR_t * ce = sx_lower(cx, C, NULL, γ, &cr);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = ce;
        IR_t * be = B ? sco_branch(cx, B, ce, ω) : ce;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        if (cr) { if (!cr->γ.node) lc_γ_to(cr, be); }
        if (res) *res = NULL;
        return be;
    }
    case TT_FOR: {
        const tree_t * INIT = (t->n > 0) ? t->c[0] : NULL; const tree_t * C = (t->n > 1) ? t->c[1] : NULL;
        const tree_t * STEP = (t->n > 2) ? t->c[2] : NULL; const tree_t * B = (t->n > 3) ? t->c[3] : NULL;
        if (!C) sno_fatal("for-loop without condition outside the landed subset", NULL);
        IR_t * cr = NULL;
        IR_t * ce = sx_lower(cx, C, NULL, γ, &cr);
        IR_t * se = STEP ? sx_lower(cx, STEP, ce, ω, NULL) : ce;
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = se;
        IR_t * be = B ? sco_branch(cx, B, se, ω) : se;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        if (cr) { if (!cr->γ.node) lc_γ_to(cr, be); }
        IR_t * ie = INIT ? sx_lower(cx, INIT, ce, ω, NULL) : ce;
        if (res) *res = NULL;
        return ie;
    }
    case TT_LOOP_BREAK: case TT_LOOP_NEXT: {
        if (t->n > 0 && t->c[0]) sno_fatal("labeled break/next outside the SCO-CF-3 subset", NULL);
        IR_t * tgt = (t->t == TT_LOOP_BREAK) ? cx->loop_exit : cx->loop_next;
        if (!tgt) sno_fatal("break/next outside any loop", NULL);
        IR_t * j = lc_build(cx->g, IR_GOTO, tgt, NULL);
        if (res) *res = NULL;
        return j;
    }
    case TT_ASSIGN: {
        const tree_t * L = (t->n > 0) ? t->c[0] : NULL; const tree_t * R = (t->n > 1) ? t->c[1] : NULL;
        if (!L || L->t != TT_VAR || !L->v.sval) sno_fatal("TT_ASSIGN lhs form outside the SCO-CF-2 subset (TT_VAR only)", NULL);
        if (!R) sno_fatal("TT_ASSIGN with no rhs", NULL);
        sno_reg_var(L->v.sval);
        IR_t * asn = lc_build(cx->g, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = L->v.sval;
        IR_t * vr = NULL; IR_t * e = sx_lower(cx, R, asn, ω, &vr);
        ir_operand_push(asn, vr);
        if (res) *res = asn;
        return e;
    }
    case TT_IF: {
        const tree_t * C = (t->n > 0) ? t->c[0] : NULL; const tree_t * TH = (t->n > 1) ? t->c[1] : NULL; const tree_t * EL = (t->n > 2) ? t->c[2] : NULL;
        if (!C) sno_fatal("TT_IF with no condition", NULL);
        IR_t * th_entry = TH ? sco_branch(cx, TH, γ, ω) : γ;
        IR_t * el_entry = EL ? sco_branch(cx, EL, γ, ω) : γ;
        IR_t * cr = NULL; IR_t * ce = sx_lower(cx, C, th_entry, el_entry, &cr);
        if (res) *res = NULL;
        return ce;
    }
    case TT_NOT: {
        const tree_t * inner = (t->n > 0) ? t->c[0] : NULL;
        if (!inner) sno_fatal("TT_NOT with no operand", NULL);
        IR_t * gate = lc_build(cx->g, IR_GOTO, γ, NULL);
        IR_t * r = NULL;
        IR_t * e = sx_lower(cx, inner, ω, gate, &r);
        if (res) *res = gate;
        return e;
    }
    case TT_CASE: {
        if (t->n < 1 || !t->c[0]) sno_fatal("TT_CASE with no subject", NULL);
        const tree_t * subj = t->c[0];
        const tree_t * def_body = NULL;
        for (int i = 1; i + 1 < t->n; i += 2) if (t->c[i] && t->c[i]->t == TT_NUL) def_body = t->c[i + 1];
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        IR_t * else_tgt;
        if (def_body) { cx->loop_exit = γ; IR_t * db = sco_branch(cx, def_body, γ, ω); cx->loop_exit = sv_exit; else_tgt = db; }
        else else_tgt = γ;
        IR_t * chain = else_tgt;
        for (int i = t->n - 2; i >= 1; i -= 2) {
            const tree_t * v = t->c[i]; const tree_t * b = t->c[i + 1];
            if (v && v->t == TT_NUL) continue;
            cx->loop_exit = γ;
            IR_t * body_entry = sco_branch(cx, b, γ, ω);
            cx->loop_exit = sv_exit;
            tree_t * idc = ast_node_new(TT_FNC); idc->v.sval = (char *) "IDENT"; ast_push(idc, (tree_t *) subj); ast_push(idc, (tree_t *) v);
            IR_t * ir = NULL; IR_t * te = sx_lower(cx, idc, body_entry, chain, &ir);
            chain = te;
        }
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        if (res) *res = NULL;
        return chain;
    }
    case TT_AUGOP: {
        const tree_t * L = (t->n > 0) ? t->c[0] : NULL;
        if (!L || L->t != TT_VAR || !L->v.sval || t->n < 2) sno_fatal("TT_AUGOP outside the landed subset (simple-variable lhs only)", NULL);
        int code;
        switch ((int) t->v.ival) {
            case TK_AUGPLUS:  code = 0;  break;
            case TK_AUGMINUS: code = 1;  break;
            case TK_AUGSTAR:  code = 2;  break;
            case TK_AUGSLASH: code = 3;  break;
            case TK_AUGPOW:   code = 18; break;
            default: sno_fatal("TT_AUGOP operator outside the landed subset", NULL); code = 0;
        }
        sno_reg_var(L->v.sval);
        IR_t * asn = lc_build(cx->g, IR_ASSIGN, γ, ω); IR_LIT(asn).sval = L->v.sval;
        IR_t * vr = NULL; IR_t * e = sx_binop(cx, t, code, asn, ω, &vr);
        ir_operand_push(asn, vr);
        if (res) *res = asn;
        return e;
    }
    case TT_RETURN: case TT_NRETURN: case TT_PROC_FAIL: {
        const char * rl = (t->t == TT_PROC_FAIL) ? "FRETURN" : (t->t == TT_NRETURN) ? "NRETURN" : "RETURN";
        IR_t * j = lc_build(cx->g, IR_GOTO, bb_label_landing(rl), NULL);
        if (res) *res = NULL;
        if (t->t == TT_RETURN && t->n > 0 && t->c[0] && cx->result_name) {
            sno_reg_var(cx->result_name);
            IR_t * asn = lc_build(cx->g, IR_ASSIGN, j, j); IR_LIT(asn).sval = (char *) cx->result_name;
            IR_t * vr = NULL; IR_t * e = sx_lower(cx, t->c[0], asn, j, &vr);
            ir_operand_push(asn, vr);
            return e;
        }
        return j;
    }
    default: {
        char buf[64]; snprintf(buf, sizeof buf, "tree kind %d", (int) t->t);
        sno_fatal("expression form not in the landed subset", buf);
        return NULL;
    }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sgoto(const tree_t * s, tree_e kind) {
    for (int i = 0; i < s->n; i++) {
        const tree_t * a = s->c[i];
        if (!a || a->t != kind) continue;
        if (a->n > 0 && a->c[0] && a->c[0]->t == TT_QLIT && a->c[0]->v.sval) return a->c[0]->v.sval;
        sno_fatal("indirect/computed goto not in the landed subset", NULL);
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_resolve_label(const char * nm) {
    IR_t * l = nm ? bb_label_landing(nm) : NULL;
    if (!l) sno_fatal("goto to unknown label", nm ? nm : "?");
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * sno_stmt_define(const tree_t * s, int * out_argbase) {
    const tree_t * subj = lc_stmt_subj(s);
    if (!subj || subj->t != TT_FNC) return NULL;
    const char * name = subj->v.sval; int argbase = 0;
    if (!name && subj->n > 0 && subj->c[0] && subj->c[0]->t == TT_VAR) { name = subj->c[0]->v.sval; argbase = 1; }
    if (!name || strcmp(name, "DEFINE")) return NULL;
    if (sfind(s, ":eq") || sfind_expr(s, ":pat")) sno_fatal("DEFINE with a pattern or replacement field is outside the landed subset", NULL);
    if (subj->n <= argbase || !subj->c[argbase] || subj->c[argbase]->t != TT_QLIT || !subj->c[argbase]->v.sval)
        sno_fatal("DEFINE with a non-literal prototype string is outside the landed subset (runtime DEFINE pending)", NULL);
    if (out_argbase) *out_argbase = argbase;
    return subj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_parse_define(const char * spec, const char * entry_opt, sno_def_t * d) {
    char buf[512]; int bn = 0;
    for (const char * p = spec; *p && bn < (int) sizeof buf - 1; p++) if (*p != ' ' && *p != '\t') buf[bn++] = *p;
    buf[bn] = 0;
    char * par = strchr(buf, '(');
    char * cls = par ? strchr(par, ')') : NULL;
    if (!par || !cls) sno_fatal("DEFINE prototype missing parameter parentheses", spec);
    *par = 0; *cls = 0;
    if (!buf[0]) sno_fatal("DEFINE prototype missing function name", spec);
    d->fname = lp_strdup(buf);
    d->entry = (entry_opt && entry_opt[0]) ? lp_strdup(entry_opt) : d->fname;
    d->result_name = NULL;
    d->nnames = 0;
    for (char * seg = par + 1; seg && *seg; ) {
        char * cm = strchr(seg, ','); if (cm) *cm = 0;
        if (*seg && d->nnames < SNO_DEF_NAMES_MAX) d->names[d->nnames++] = lp_strdup(seg);
        seg = cm ? cm + 1 : NULL;
    }
    for (char * seg = cls + 1; seg && *seg; ) {
        char * cm = strchr(seg, ','); if (cm) *cm = 0;
        if (*seg && d->nnames < SNO_DEF_NAMES_MAX) d->names[d->nnames++] = lp_strdup(seg);
        seg = cm ? cm + 1 : NULL;
    }
    sno_reg_var(d->fname);
    for (int k = 0; k < d->nnames; k++) sno_reg_var(d->names[k]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sx_subscript_lv(scx_t * cx, const tree_t * base, const tree_t * const * idxs, int nidx, IR_t * ω, IR_t ** var_res) {
    IR_t * br = NULL; IR_t * entry = sx_lower(cx, base, NULL, ω, &br);
    IR_t * cur = br;
    for (int k = 0; k < nidx; k++) {
        IR_t * ir = NULL; IR_t * ie = sx_lower(cx, idxs[k], NULL, ω, &ir);
        lc_γ_to(cur, ie);
        IR_t * sub = lc_build(cx->g, IR_SUBSCRIPT, NULL, ω);
        lc_γ_to(ir, sub);
        ir_operand_push(sub, cur); ir_operand_push(sub, ir);
        cur = sub;
    }
    if (var_res) *var_res = cur;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int ir_is_generator_kind(IR_e t);
static void sno_ω_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_resume_ω_to(IR_t * nd, IR_t * t) {
    /* SN4-PAT-CAPTURE-STACK: re-point nd's exhaust-ω at a left generator t — but a capture COND's ω is
     * ALREADY the capture's inward resume edge (inner generator's β, or SAVE's pop); clobbering it would
     * sever the capture's own chain.  The capture's OUTWARD exhaust is its SAVE's ω (β pops, then ω), so
     * chain through operands[1] instead.  Everything else re-points directly (the pre-stack behaviour). */
    if (nd && nd->op == IR_MATCH_ASSIGN_COND && nd->n_operands > 1 && nd->operands[1]) nd = nd->operands[1];
    sno_ω_to(nd, t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_is_fence(const tree_t * t) { return t && ((t->t == TT_FENCE) || (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "FENCE"))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_seq_has_fence(const tree_t * t) { if (!t) return 0; if (sno_is_fence(t)) return 1; if (t->t == TT_SEQ) return sno_seq_has_fence((t->n > 0) ? t->c[0] : NULL) || sno_seq_has_fence((t->n > 1) ? t->c[1] : NULL); return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_deterministic(const tree_t * t) {
    /* ZB-5 v1 gate: a body element that can yield MORE THAN ONE way (a generator, or an alternation whose
     * re-choice a completed iteration would need) makes ARBNO backtrack non-total — that needs the
     * per-iteration COLLECTION (v2).  Deterministic subtrees exhaust totally, so no iteration state is kept. */
    if (!t) return 1;
    if (t->t == TT_ALT || t->t == TT_ARB || t->t == TT_ARBNO || sno_is_fence(t)) return 0;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "ARB")) return 0;
    for (int i = 0; i < t->n; i++) if (!sno_pat_deterministic(t->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_pat_node(scx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail) {
    IR_graph_t * g = cx->g;
    if (!t) return succ;
    switch (t->t) {
    case TT_QLIT: {
        IR_t * nd = lc_build(g, IR_MATCH_LIT, succ, NULL);
        sno_ω_to(nd, fail);
        IR_LIT(nd).sval = t->v.sval ? t->v.sval : (char *) "";
        return nd;
    }
    case TT_ANY: case TT_NOTANY: {
        IR_t * nd = lc_build(g, (t->t == TT_ANY) ? IR_MATCH_ANY : IR_MATCH_NOTANY, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_QLIT) IR_LIT(nd).sval = t->c[0]->v.sval ? t->c[0]->v.sval : (char *) "";
        else sno_fatal("ANY/NOTANY with a non-literal charset is outside the SN4-PAT subset", NULL);
        return nd;
    }
    case TT_SPAN: {
        IR_t * nd = lc_build(g, IR_MATCH_SPAN, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_QLIT) IR_LIT(nd).sval = t->c[0]->v.sval ? t->c[0]->v.sval : (char *) "";
        else sno_fatal("SPAN with a non-literal charset is outside the SN4-PAT subset", NULL);
        return nd;
    }
    case TT_BREAK: case TT_BREAKX: {
        IR_t * nd = lc_build(g, (t->t == TT_BREAK) ? IR_MATCH_BREAK : IR_MATCH_BREAKX, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_QLIT) IR_LIT(nd).sval = t->c[0]->v.sval ? t->c[0]->v.sval : (char *) "";
        else sno_fatal("BREAK/BREAKX with a non-literal charset is outside the SN4-PAT subset", NULL);
        return nd;
    }
    case TT_TAB: case TT_RTAB: {
        IR_t * nd = lc_build(g, (t->t == TT_TAB) ? IR_MATCH_TAB : IR_MATCH_RTAB, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->n <= 0 || !t->c[0]) sno_fatal("TAB/RTAB requires a count argument", NULL);
        IR_t * argval = NULL;
        IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval);
        ir_operand_push(nd, argval);
        return arg_entry;
    }
    case TT_POS: case TT_RPOS: {
        IR_t * nd = lc_build(g, IR_MATCH_POS, succ, NULL);
        sno_ω_to(nd, fail);
        if (t->t == TT_RPOS) IR_LIT(nd).sval = (char *) "r";
        if (t->n <= 0 || !t->c[0]) sno_fatal("POS/RPOS requires a position argument", NULL);
        IR_t * argval = NULL;
        IR_t * arg_entry = sx_lower(cx, t->c[0], nd, fail, &argval);
        ir_operand_push(nd, argval);
        return arg_entry;
    }
    case TT_FENCE:
        return (t->n > 0 && t->c[0]) ? sno_pat_node(cx, t->c[0], succ, fail) : succ;
    case TT_VAR: {
        const char * nm = t->v.sval;
        if (nm && !strcmp(nm, "REM")) { IR_t * nd = lc_build(g, IR_MATCH_REM, succ, NULL); sno_ω_to(nd, fail); return nd; }
        if (nm && !strcmp(nm, "ARB")) { IR_t * nd = lc_build(g, IR_MATCH_ARB, succ, NULL); sno_ω_to(nd, fail); return nd; }
        if (nm && !strcmp(nm, "FENCE")) return succ;
        sno_fatal("bare-identifier pattern outside the SN4-PAT subset (REM, ARB, FENCE only; ABORT/BAL/deferred-var pending)", NULL);
        return succ;
    }
    case TT_REM: {
        IR_t * nd = lc_build(g, IR_MATCH_REM, succ, NULL);
        sno_ω_to(nd, fail);
        return nd;
    }
    case TT_ARB: {
        IR_t * nd = lc_build(g, IR_MATCH_ARB, succ, NULL);
        sno_ω_to(nd, fail);
        return nd;
    }
    case TT_ARBNO: {
        /* ZB-5 SN4-PAT ARBNO v1 (deterministic body).  Three phases share IR_MATCH_ARBNO, IR_LIT.ival = phase.
         * G (0, generator, no operands; MUST be first-allocated so TT_SEQ's tail rule finds the β surface):
         *   α saves entry+yield cursors and jmps γ (null yield — SPITBOL shortest-first); β restores δ=yield,
         *   records cur_before, jmps ω — G's ω is REPURPOSED as the body-entry edge (the β-continuation); the
         *   construct's real fail exit lives on F.  K (1, operand[0]=G): body-success landing — null-progress
         *   guard (δ==cur_before → ω=F, the 4/28 zero-advance rule), else yield=δ, jmp γ (yield one more).
         * F (2, operand[0]=G): exhaust — δ=entry, jmp ω (outer fail); its template defines a β alias because
         *   body leaves stamp their fail edges via sno_ω_to and IR_MATCH_ARBNO is generator-kind.
         * Deterministic bodies exhaust totally (a completed iteration cannot re-choose), so NO per-iteration
         * COLLECTION is needed — that is the generator-body v2 requirement (ALT/ARB/ARBNO/FENCE inside). */
        if (!(t->n > 0) || !t->c[0]) sno_fatal("ARBNO requires a pattern argument", NULL);
        if (!sno_pat_deterministic(t->c[0])) sno_fatal("ARBNO with a generator body (ALT/ARB/ARBNO/FENCE inside) needs per-iteration COLLECTION state — ZB-5 v2, not yet implemented", NULL);
        IR_t * G = lc_build(g, IR_MATCH_ARBNO, succ, NULL); IR_LIT(G).ival = 0;
        IR_t * F = lc_build(g, IR_MATCH_ARBNO, NULL, NULL); IR_LIT(F).ival = 2; ir_operand_push(F, G); sno_ω_to(F, fail);
        IR_t * K = lc_build(g, IR_MATCH_ARBNO, succ, F);    IR_LIT(K).ival = 1; ir_operand_push(K, G);
        IR_t * be = sno_pat_node(cx, t->c[0], K, F);
        lc_ω_to(G, be);
        return G;
    }
    case TT_LEN: {
        IR_t * nd = lc_build(g, IR_MATCH_LEN, succ, NULL);
        sno_ω_to(nd, fail);
        long long n = 0;
        if (t->n > 0 && t->c[0] && t->c[0]->t == TT_ILIT) n = t->c[0]->v.ival;
        else sno_fatal("LEN with a non-literal count is outside the SN4-PAT-2 subset", NULL);
        IR_LIT(nd).ival = n;
        return nd;
    }
    case TT_CAPT_COND_ASGN: {
        const char * vn = (t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) ? t->c[1]->v.sval : NULL;
        if (!vn || !(t->n > 0 && t->c[0])) sno_fatal("conditional capture target is not a simple variable (SN4-PAT-2 subset)", NULL);
        sno_reg_var(vn);
        /* SN4-PAT-CAPTURE-STACK (Lon directive 2026-07-05): capture spans [start-of-inner, current) on a
         * per-box STACK — SAVE.α pushes the open cursor, SAVE.β pops it, the COND at every inner yield
         * assigns from the top-of-stack frame — so the β-resume chain survives a generator between the
         * capture's open and close.  COND is allocated FIRST (first-allocated = the capture's tail for
         * TT_SEQ's re-point rule; both ops are generator-kind so re-points land β-wards).  The inner is
         * lowered with fail = SAVE: an inner exhaust lands on SAVE.β (pop, then ω → the capture's fail,
         * β-aware so a generator further left still resumes).  COND.ω is the backtrack-IN edge: a failing
         * right neighbour lands on COND.β (jmp ω) and resumes the inner generator's β — or, for a
         * deterministic inner, goes straight to SAVE.β (pop + fail leftward, the pre-stack destination). */
        IR_t * nd = lc_build(g, IR_MATCH_ASSIGN_COND, succ, NULL);  /* phase-1 COND, γ → succ */
        IR_LIT(nd).sval = (char *) vn;
        IR_t * save = lc_build(g, IR_MATCH_ASSIGN_SAVE, NULL, NULL); /* phase-0 SAVE: α push, β pop */
        IR_LIT(save).sval = (char *) vn;                           /* template's op_sval[0] guard */
        sno_ω_to(save, fail);                                      /* pop path exits to the capture's fail */
        int before_i = g->n;
        IR_t * pe = sno_pat_node(cx, t->c[0], nd, save);           /* inner pattern, γ → COND, fail → SAVE.β */
        IR_t * itail = (before_i < g->n) ? g->all[before_i] : pe;  /* inner rightmost leaf (first allocated) */
        lc_γ_to(save, pe);                                         /* SAVE.γ → inner entry */
        sno_ω_to(nd, ir_is_generator_kind(itail->op) ? itail : save); /* COND backtrack-in: resume or pop */
        ir_operand_push(nd, pe);                                   /* [0] inner entry */
        ir_operand_push(nd, save);                                 /* [1] SAVE → COND.op_off = save's slot */
        return save;                                               /* capture entry is the SAVE node */
    }
    case TT_SEQ: {
        /* SN4-PAT-3h CAT: pattern concatenation A B — node-free in the live single-HEAD
         * design (the parked IR_MATCH_SEQUENCE was a subgraph success-sink; here success threads
         * straight to `succ`).  Wire A.γ → B.α → succ by lowering right-first, then left with
         * succ = right's entry.  Failure: a deterministic element's ω already points at `fail`
         * ( = head = retry-position, correct SNOBOL4 for SPAN/BREAK/LEN/… which never back off).
         * The ONLY resumable leaf today is ARB (a generator): if the left element is a
         * generator, the right element's failure must resume it (β) rather than advance the
         * whole attempt, so re-point right's tail-ω at the left tail via sno_ω_to (β-aware). */
        /* SN4-PAT FENCE: a fence in the spine seals — every element to its RIGHT fails to the
         * statement-level cx->pat_fail (no HEAD retry, no left-generator resume) instead of `fail`
         * (= HEAD).  Fence is node-free and transparent forward.  Fence-free sequences keep the
         * untouched 2-way path (zero behavioural change for every landed matcher). */
        if (!sno_seq_has_fence(t)) {
            const tree_t * lc = (t->n > 0) ? t->c[0] : NULL;
            const tree_t * rc = (t->n > 1) ? t->c[1] : NULL;
            if (!lc) return sno_pat_node(cx, rc, succ, fail);
            if (!rc) return sno_pat_node(cx, lc, succ, fail);
            int before_r = g->n;
            IR_t * re = sno_pat_node(cx, rc, succ, fail);
            IR_t * re_tail = (before_r < g->n) ? g->all[before_r] : re;  /* rc rightmost leaf (first allocated) */
            int before_l = g->n;
            IR_t * le = sno_pat_node(cx, lc, re, fail);
            IR_t * le_tail = (before_l < g->n) ? g->all[before_l] : le;  /* lc rightmost leaf */
            if (re_tail && le_tail && ir_is_generator_kind(le_tail->op)) sno_resume_ω_to(re_tail, le_tail);
            return le;
        }
        const tree_t * elems[128]; int ne = 0; const tree_t * rstack[128]; int nr = 0; const tree_t * cur = t;
        while (cur && cur->t == TT_SEQ) { if (nr >= 128) sno_fatal("pattern sequence too long (SN4-PAT cap 128)", NULL); rstack[nr++] = (cur->n > 1) ? cur->c[1] : NULL; cur = (cur->n > 0) ? cur->c[0] : NULL; }
        elems[ne++] = cur;
        for (int i = nr - 1; i >= 0; i--) elems[ne++] = rstack[i];
        int first_fence = ne;
        for (int i = 0; i < ne; i++) if (sno_is_fence(elems[i])) { first_fence = i; break; }
        IR_t * cur_succ = succ; IR_t * right_tail = NULL; int right_sealed = 0;
        for (int i = ne - 1; i >= 0; i--) {
            if (sno_is_fence(elems[i])) {                                           /* seals everything to its right; the element to its left cannot resume into it */
                right_sealed = 1;
                const tree_t * inner = (elems[i]->t == TT_FENCE && elems[i]->n > 0) ? elems[i]->c[0] : NULL;
                if (inner) {                                                        /* FENCE(P): lower P with the pre-seal fail so P retries normally on forward-fail; the seal blocks re-entry after success */
                    IR_t * fail_p = (i > first_fence) ? cx->pat_fail : fail;
                    int before_p = g->n;
                    IR_t * pe = sno_pat_node(cx, inner, cur_succ, fail_p);
                    IR_t * p_tail = (before_p < g->n) ? g->all[before_p] : pe;
                    cur_succ = pe; right_tail = p_tail;
                }
                continue;
            }
            IR_t * fail_i = (i > first_fence) ? cx->pat_fail : fail;                 /* right of the fence: cut to statement-fail, never HEAD */
            int before_e = g->n;
            IR_t * ee = sno_pat_node(cx, elems[i], cur_succ, fail_i);
            IR_t * e_tail = (before_e < g->n) ? g->all[before_e] : ee;
            if (right_tail && !right_sealed && before_e < g->n && ir_is_generator_kind(e_tail->op)) sno_resume_ω_to(right_tail, e_tail);
            cur_succ = ee; right_tail = e_tail; right_sealed = 0;
        }
        return cur_succ;
    }
    case TT_ALT: {
        /* SN4-PAT-3h ALTERNATE (A | B | C).  Flatten the left-associative TT_ALT spine into a
         * left-to-right list, then wire: a phase-0 SAVE records the entry cursor; alternative i
         * fails to a phase-1 RESTORE that reloads the cursor and jumps to alternative i+1; the last
         * alternative fails to the outer `fail`; every alternative succeeds to `succ`. */
        const tree_t * alts[64]; int na = 0;
        const tree_t * rstack[64]; int nr = 0;
        const tree_t * cur = t;
        while (cur && cur->t == TT_ALT) {
            if (nr >= 64) sno_fatal("alternation with too many branches (SN4-PAT-3h cap 64)", NULL);
            rstack[nr++] = (cur->n > 1) ? cur->c[1] : NULL;
            cur = (cur->n > 0) ? cur->c[0] : NULL;
        }
        alts[na++] = cur;                                   /* leftmost */
        for (int i = nr - 1; i >= 0; i--) alts[na++] = rstack[i];
        if (na == 1) return sno_pat_node(cx, alts[0], succ, fail);
        IR_t * save = lc_build(g, IR_MATCH_ALTERNATE, NULL, NULL);   /* phase-0 (n_operands==0): save cursor */
        IR_t * fail_target = fail;                                    /* last alternative fails to outer fail */
        for (int i = na - 1; i >= 1; i--) {
            IR_t * ei = sno_pat_node(cx, alts[i], succ, fail_target); /* alternative i, γ→succ, ω→fail_target */
            IR_t * ri = lc_build(g, IR_MATCH_ALTERNATE, ei, NULL);    /* phase-1 RESTORE_i, γ→alt i entry */
            ir_operand_push(ri, save);                                /* reads save's slot */
            fail_target = ri;                                         /* alternative i-1 fails to RESTORE_i */
        }
        IR_t * e0 = sno_pat_node(cx, alts[0], succ, fail_target);     /* leftmost: fail→RESTORE_1, no restore before it */
        lc_γ_to(save, e0);                                            /* SAVE proceeds to the first alternative */
        return save;
    }
    default:
        sno_fatal("pattern element not in the SN4-PAT subset (LEN, literal, ANY, NOTANY, SPAN, BREAK, BREAKX, TAB, RTAB, POS, RPOS, REM, ARB; SEQ+ALT landed SN4-PAT-3h)", NULL);
    }
    return succ;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sno_pat_supported(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FENCE) return t->n == 0 || sno_pat_supported(t->c[0]);
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "FENCE")) return 1;
    if (t->t == TT_QLIT) return 1;
    if (t->t == TT_ANY || t->t == TT_NOTANY) return t->n > 0 && t->c[0] && t->c[0]->t == TT_QLIT;
    if (t->t == TT_SPAN) return t->n > 0 && t->c[0] && t->c[0]->t == TT_QLIT;
    if (t->t == TT_BREAK || t->t == TT_BREAKX) return t->n > 0 && t->c[0] && t->c[0]->t == TT_QLIT;
    if (t->t == TT_TAB || t->t == TT_RTAB) return t->n > 0 && t->c[0] != NULL;
    if (t->t == TT_POS || t->t == TT_RPOS) return t->n > 0 && t->c[0] != NULL;
    if (t->t == TT_REM || t->t == TT_ARB) return 1;
    if (t->t == TT_ARBNO) return t->n > 0 && t->c[0] && sno_pat_supported(t->c[0]) && sno_pat_deterministic(t->c[0]);
    if (t->t == TT_VAR) return t->v.sval && (!strcmp(t->v.sval, "REM") || !strcmp(t->v.sval, "ARB"));
    if (t->t == TT_LEN) return t->n > 0 && t->c[0] && t->c[0]->t == TT_ILIT;
    if (t->t == TT_CAPT_COND_ASGN) return t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR && sno_pat_supported(t->c[0]);
    if (t->t == TT_SEQ) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    if (t->t == TT_ALT) return sno_pat_supported((t->n > 0) ? t->c[0] : NULL) && sno_pat_supported((t->n > 1) ? t->c[1] : NULL);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * sno_lower_match(scx_t * cx, const tree_t * subj, IR_t * sJ, IR_t * fJ) {
    IR_graph_t * g = cx->g;
    cx->pat_fail = fJ;
    const tree_t * svt = (subj->n > 0) ? subj->c[0] : NULL;
    const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
    IR_t * head = lc_build(g, IR_MATCH_HEAD, NULL, fJ);
    IR_t * pat_entry = sno_pat_node(cx, ptt, sJ, head);
    lc_γ_to(head, pat_entry);
    IR_t * subjval = NULL;
    IR_t * subj_entry = sx_lower(cx, svt, head, fJ, &subjval);
    ir_operand_push(head, subjval);
    return subj_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * sno_build_graph(const tree_t ** st, int nst, int entry_idx, const int * is_def, const char * result_name) {
    IR_graph_t * g = IR_alloc(nst * 16 + 256);
    scx_t cx; cx.g = g; cx.loop_exit = NULL; cx.loop_next = NULL; cx.result_name = result_name; cx.pat_fail = NULL;
    IR_t * exitnd = lc_build(g, IR_SUCCEED, NULL, NULL);
    IR_t * failnd = lc_build(g, IR_FAIL, NULL, NULL);
    IR_t ** anchor = (IR_t **) calloc((size_t) nst, sizeof(IR_t *));
    bb_label_registry_reset();
    for (int i = 0; i < nst; i++) {
        anchor[i] = lc_build(g, IR_GOTO, NULL, NULL);
        const char * lbl = sfind_str(st[i], ":lbl");
        if (lbl && lbl[0]) bb_label_registry_add(lp_strdup(lbl), anchor[i]);
    }
    bb_label_registry_add(lp_strdup("END"), exitnd);
    if (!bb_label_landing("RETURN"))  bb_label_registry_add(lp_strdup("RETURN"),  exitnd);
    if (!bb_label_landing("FRETURN")) bb_label_registry_add(lp_strdup("FRETURN"), failnd);
    if (!bb_label_landing("NRETURN")) bb_label_registry_add(lp_strdup("NRETURN"), exitnd);
    g->entry = (nst > 0) ? anchor[entry_idx] : exitnd;
    for (int i = 0; i < nst; i++) {
        const tree_t * s = st[i];
        { extern void zls_group_mark(const IR_graph_t *, const char *); const char * mlbl = sfind_str(s, ":lbl"); if (mlbl && mlbl[0]) zls_group_mark(g, lp_strdup(mlbl)); }
        IR_t * next = (i + 1 < nst) ? anchor[i + 1] : exitnd;
        if (sfind(s, ":end")) { lc_γ_to(anchor[i], exitnd); continue; }
        const char * goU = sgoto(s, TT_GOTO_U);
        const char * goS = sgoto(s, TT_GOTO_S);
        const char * goF = sgoto(s, TT_GOTO_F);
        IR_t * sT = goS ? sno_resolve_label(goS) : (goU ? sno_resolve_label(goU) : next);
        IR_t * fT = goF ? sno_resolve_label(goF) : (goU ? sno_resolve_label(goU) : next);
        IR_t * sJ = lc_build(g, IR_GOTO, sT, NULL);
        IR_t * fJ = lc_build(g, IR_GOTO, fT, NULL);
        if (is_def && is_def[i]) { lc_γ_to(anchor[i], sJ); continue; }
        const tree_t * subj = lc_stmt_subj(s);
        const tree_t * pat  = sfind_expr(s, ":pat");
        int has_eq = sfind(s, ":eq") != NULL;
        if (pat) sno_fatal("statement has a separate :pat field (stored-pattern form) — SN4-PAT-2 handles TT_SCAN match subjects only", NULL);
        if (subj && subj->t == TT_SCAN) {
            if (has_eq) sno_fatal("SUBJECT PATTERN = REPL splice is outside the SN4-PAT-2 subset (match-only for now)", NULL);
            const tree_t * ptt = (subj->n > 1) ? subj->c[1] : NULL;
            if (!sno_pat_supported(ptt)) sno_fatal("pattern shape outside the SN4-PAT subset (LEN, literal, ANY, NOTANY, SPAN, BREAK, BREAKX, TAB, RTAB, POS, RPOS, REM, ARB)", NULL);
            IR_t * e = sno_lower_match(&cx, subj, sJ, fJ);
            lc_γ_to(anchor[i], e);
            continue;
        }
        if (!subj) { lc_γ_to(anchor[i], sJ); continue; }
        if (!has_eq) {
            IR_t * r = NULL; IR_t * e = sx_lower(&cx, subj, sJ, fJ, &r);
            lc_γ_to(anchor[i], e);
            continue;
        }
        tree_t * repl = sfind_expr(s, ":repl");
        if (subj->t == TT_VAR) {
            sno_reg_var(subj->v.sval);
            IR_t * asn = lc_build(g, IR_ASSIGN, sJ, fJ); IR_LIT(asn).sval = subj->v.sval;
            IR_t * vr = NULL; IR_t * e = sx_lower(&cx, repl, asn, fJ, &vr);
            ir_operand_push(asn, vr);
            lc_γ_to(anchor[i], e);
            continue;
        }
        if (subj->t == TT_INDIRECT && subj->n > 0) {
            IR_t * nv = NULL; IR_t * e1 = sx_nameval(&cx, subj->c[0], NULL, fJ, &nv);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
            lc_γ_to(nv, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fJ);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, nv); ir_operand_push(asn, vv);
            lc_γ_to(anchor[i], e1);
            continue;
        }
        if (subj->t == TT_IDX && subj->n >= 2) {
            IR_t * vr = NULL; IR_t * e1 = sx_subscript_lv(&cx, subj->c[0], (const tree_t * const *) &subj->c[1], subj->n - 1, fJ, &vr);
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
            lc_γ_to(vr, e2);
            IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fJ);
            lc_γ_to(vv, asn);
            ir_operand_push(asn, vr); ir_operand_push(asn, vv);
            lc_γ_to(anchor[i], e1);
            continue;
        }
        if (subj->t == TT_FNC) {
            const char * fname = subj->v.sval; int argbase = 0;
            if (!fname && subj->n > 0 && subj->c[0] && subj->c[0]->t == TT_VAR) { fname = subj->c[0]->v.sval; argbase = 1; }
            int fnargs = subj->n - argbase;
            if (fname && !strcmp(fname, "ITEM") && fnargs >= 2) {
                IR_t * vr = NULL; IR_t * e1 = sx_subscript_lv(&cx, subj->c[argbase], (const tree_t * const *) &subj->c[argbase + 1], fnargs - 1, fJ, &vr);
                IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
                lc_γ_to(vr, e2);
                IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fJ);
                lc_γ_to(vv, asn);
                ir_operand_push(asn, vr); ir_operand_push(asn, vv);
                lc_γ_to(anchor[i], e1);
                continue;
            }
            { extern int rt_dat_field_of_any(const char *);
              if (fname && fnargs == 1 && rt_dat_field_of_any(fname)) {
                  IR_t * br = NULL; IR_t * e1 = sx_lower(&cx, subj->c[argbase], NULL, fJ, &br);
                  IR_t * fv = lc_build(g, IR_FIELD_VAR, NULL, fJ); IR_LIT(fv).sval = (char *) lp_strdup(fname);
                  lc_γ_to(br, fv);
                  ir_operand_push(fv, br);
                  IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
                  lc_γ_to(fv, e2);
                  IR_t * asn = lc_build(g, IR_ASSIGN_VAR, sJ, fJ);
                  lc_γ_to(vv, asn);
                  ir_operand_push(asn, fv); ir_operand_push(asn, vv);
                  lc_γ_to(anchor[i], e1);
                  continue;
              } }
        }
        if (subj->t == TT_KEYWORD && subj->v.sval) {
            IR_t * mk = lc_build(g, IR_CALL, sJ, fJ); IR_LIT(mk).sval = (char *) "SNO$KWSET";
            IR_t * nl = lc_build(g, IR_LIT_STRING, NULL, fJ); IR_LIT(nl).sval = subj->v.sval;
            IR_t * vv = NULL; IR_t * e2 = sx_lower(&cx, repl, NULL, fJ, &vv);
            lc_γ_to(nl, e2);
            lc_γ_to(vv, mk);
            ir_operand_push(mk, nl); ir_operand_push(mk, vv);
            lc_γ_to(anchor[i], nl);
            continue;
        }
        sno_fatal("assignment subject form not in the landed subset", NULL);
    }
    free(anchor);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    int mod_idx = -1;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        const tree_t * subj = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!subj) continue;
        const char * lbl = stmt_attr_str(stmt_attr_find(s, ":lbl"));
        if (mod_idx >= 0 && lbl && *lbl) s2->module_registry.mods[mod_idx].core_label_count++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * sno_litname(const tree_t * a) {
    if (!a) return NULL;
    if (a->t == TT_QLIT && a->v.sval) return a->v.sval;
    if (a->t == TT_NAME && a->n > 0 && a->c[0] && a->c[0]->t == TT_VAR && a->c[0]->v.sval) return a->c[0]->v.sval;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sno_prescan_expr(const tree_t * t, sno_def_t * defs, int * ndefs) {
    if (!t) return;
    if (t->t == TT_FNC) {
        const char * name = t->v.sval; int argbase = 0;
        if (!name && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR) { name = t->c[0]->v.sval; argbase = 1; }
        if (name && !strcmp(name, "DATA") && t->n > argbase && t->c[argbase] && t->c[argbase]->t == TT_QLIT && t->c[argbase]->v.sval) {
            extern void * dat_register(const char * spec); extern void * dat_find_type(const char * name);
            const char * sp = t->c[argbase]->v.sval;
            char nb[128]; int k = 0; for (; sp[k] && sp[k] != '(' && k < 127; k++) nb[k] = sp[k]; nb[k] = 0;
            if (nb[0] && !dat_find_type(nb)) dat_register(sp);
        }
        if (name && !strcmp(name, "OPSYN") && t->n - argbase == 2) {
            const char * an = sno_litname(t->c[argbase]);
            const char * on = sno_litname(t->c[argbase + 1]);
            if (an && on) {
                int fo = -1;
                for (int k = 0; k < *ndefs; k++) if (!strcmp(defs[k].fname, on)) { fo = k; break; }
                if (fo >= 0 && *ndefs < SNO_DEF_MAX) { sno_def_t d = defs[fo]; d.result_name = d.result_name ? d.result_name : d.fname; d.fname = lp_strdup(an); sno_reg_var(d.fname); defs[(*ndefs)++] = d; }
                else { extern void rt_builtin_synonym_add(const char *, const char *); rt_builtin_synonym_add(lp_strdup(an), lp_strdup(on)); }
            }
        }
    }
    for (int i = 0; i < t->n; i++) sno_prescan_expr(t->c[i], defs, ndefs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t * lower_sno_stage2(const tree_t * prog) {
    if (!prog || prog->t != TT_PROGRAM) return NULL;
    sno_register_program(&g_stage2, prog);
    int nst = 0;
    for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) nst++;
    if (nst == 0) return &g_stage2;
    const tree_t ** st = (const tree_t **) calloc((size_t) nst, sizeof(tree_t *));
    { int k = 0; for (int i = 0; i < prog->n; i++) if (prog->c[i] && prog->c[i]->t == TT_STMT) st[k++] = prog->c[i]; }
    sno_def_t defs[SNO_DEF_MAX]; int ndefs = 0;
    const tree_t * def_body[SNO_DEF_MAX]; for (int _k = 0; _k < SNO_DEF_MAX; _k++) def_body[_k] = NULL;
    int * is_def = (int *) calloc((size_t) nst, sizeof(int));
    for (int i = 0; i < nst; i++) {
        sno_prescan_expr(lc_stmt_subj(st[i]), defs, &ndefs);
        const tree_t * dfn = lc_stmt_subj(st[i]);
        if (dfn && dfn->t == TT_DEFINE) {
            is_def[i] = 1;
            const tree_t * pnode = (dfn->n > 1) ? dfn->c[1] : NULL;
            if (!pnode || pnode->t != TT_QLIT || !pnode->v.sval) sno_fatal("TT_DEFINE missing literal prototype string", NULL);
            sno_def_t d; sno_parse_define(pnode->v.sval, NULL, &d);
            const tree_t * body = (dfn->n > 2) ? dfn->c[2] : NULL;
            int found = -1;
            for (int k = 0; k < ndefs; k++) if (!strcmp(defs[k].fname, d.fname)) { found = k; break; }
            if (found >= 0) { defs[found] = d; def_body[found] = body; }
            else if (ndefs < SNO_DEF_MAX) { def_body[ndefs] = body; defs[ndefs++] = d; }
            else sno_fatal("too many DEFINEs in one program", d.fname);
            continue;
        }
        int argbase = 0;
        const tree_t * dsub = sno_stmt_define(st[i], &argbase);
        if (!dsub) continue;
        is_def[i] = 1;
        const char * entry_opt = NULL;
        if (dsub->n > argbase + 1 && dsub->c[argbase + 1]) {
            const tree_t * ea = dsub->c[argbase + 1];
            if (ea->t == TT_QLIT && ea->v.sval) entry_opt = ea->v.sval;
            else if (ea->t == TT_NAME && ea->n > 0 && ea->c[0] && ea->c[0]->t == TT_VAR && ea->c[0]->v.sval) entry_opt = ea->c[0]->v.sval;
        }
        sno_def_t d; sno_parse_define(dsub->c[argbase]->v.sval, entry_opt, &d);
        int found = -1;
        for (int k = 0; k < ndefs; k++) if (!strcmp(defs[k].fname, d.fname)) { found = k; break; }
        if (found >= 0) defs[found] = d;
        else if (ndefs < SNO_DEF_MAX) defs[ndefs++] = d;
        else sno_fatal("too many DEFINEs in one program", d.fname);
    }
    IR_graph_t * g = sno_build_graph(st, nst, 0, is_def, NULL);
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name = "main";
    g_stage2.proc_table[pi].proc = NULL;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].nparams = 0;
    g_stage2.proc_table[pi].is_generator = 0;
    g_stage2.proc_table[pi].dyn_scope = 0;
    g_stage2.proc_table[pi].result_name = NULL;
    g_stage2.proc_table[pi].bb_idx = bb_program_add(&g_stage2.bbp, g);
    for (int di = 0; di < ndefs; di++) {
        IR_graph_t * gf;
        const char * rn = defs[di].result_name ? defs[di].result_name : defs[di].fname;
        if (def_body[di]) {
            const tree_t * bp = def_body[di];
            int bn = 0;
            for (int i = 0; i < bp->n; i++) if (bp->c[i] && bp->c[i]->t == TT_STMT) bn++;
            const tree_t ** bst = (const tree_t **) calloc((size_t)(bn > 0 ? bn : 1), sizeof(tree_t *));
            int bk = 0;
            for (int i = 0; i < bp->n; i++) if (bp->c[i] && bp->c[i]->t == TT_STMT) bst[bk++] = bp->c[i];
            int * bis = (int *) calloc((size_t)(bn > 0 ? bn : 1), sizeof(int));
            gf = sno_build_graph(bst, bn, 0, bis, rn);
            free((void *) bst); free(bis);
        } else {
            int eidx = -1;
            for (int i = 0; i < nst; i++) { const char * lbl = sfind_str(st[i], ":lbl"); if (lbl && !strcmp(lbl, defs[di].entry)) { eidx = i; break; } }
            if (eidx < 0) sno_fatal("DEFINE entry label not found among statement labels", defs[di].entry);
            gf = sno_build_graph(st, nst, eidx, is_def, rn);
        }
        int fpi = stage2_proc_grow(&g_stage2);
        g_stage2.proc_table[fpi].name = defs[di].fname;
        g_stage2.proc_table[fpi].proc = NULL;
        g_stage2.proc_table[fpi].entry_pc = -1;
        g_stage2.proc_table[fpi].nparams = defs[di].nnames;
        for (int k = 0; k < defs[di].nnames && k < STAGE2_FRAME_SLOT_MAX; k++) g_stage2.proc_table[fpi].lower_sc.e[k].name = defs[di].names[k];
        g_stage2.proc_table[fpi].lower_sc.n = defs[di].nnames < STAGE2_FRAME_SLOT_MAX ? defs[di].nnames : STAGE2_FRAME_SLOT_MAX;
        g_stage2.proc_table[fpi].is_generator = 0;
        g_stage2.proc_table[fpi].dyn_scope = 1;
        g_stage2.proc_table[fpi].result_name = defs[di].result_name;
        g_stage2.proc_table[fpi].bb_idx = bb_program_add(&g_stage2.bbp, gf);
    }
    free((void *) st); free(is_def);
    return &g_stage2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_snobol4(const tree_t * prog) {
    (void) prog;
    fprintf(stderr, "FATAL lower_snobol4 (GZ#5 subset): the EVAL/CODE runtime-lowering entry is outside the landed subset (per directive: EVAL and CODE are not remotely possible yet).\n");
    abort();
    return NULL;
}
