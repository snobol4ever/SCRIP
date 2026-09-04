#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
typedef struct { IR_graph_t * g; IR_t * try_catch; IR_t * loop_exit; IR_t * loop_next; const tree_t * cur_proc; uint64_t cur_byref_mask; int cur_nparams; } rcx_t;
#define RK_GRAM_MAX 64
static const char * g_rk_gram_names[RK_GRAM_MAX];
static int          g_rk_gram_n = 0;
static const char * g_rk_class_names[RK_GRAM_MAX];
static int          g_rk_class_n = 0;
static char         g_rk_multi_names[RK_GRAM_MAX][128];
static int          g_rk_multi_n = 0;
extern int rk_is_arrlit_scalar(const char * nm);
extern int rk_is_array_name(const char * nm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_multi_name(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_rk_multi_n; i++) if (!strcmp(g_rk_multi_names[i], nm)) return 1; return 0; }
static void rk_multi_name_add(const char * base) { if (!base || rk_is_multi_name(base) || g_rk_multi_n >= RK_GRAM_MAX) return; snprintf(g_rk_multi_names[g_rk_multi_n++], 128, "%s", base); }
static int rk_is_grammar_name(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_rk_gram_n; i++) if (!strcmp(g_rk_gram_names[i], nm)) return 1; return 0; }
static int rk_is_class_name(const char * nm) { if (!nm) return 0; for (int i = 0; i < g_rk_class_n; i++) if (!strcmp(g_rk_class_names[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_modeled_type(const char * ty) {
    if (!ty) return 0; static const char * k[] = { "Int", "Num", "Rat", "Str", "Numeric", "Real", "Cool", "Bool", 0 }; for (int i = 0; k[i]; i++) if (!strcmp(ty, k[i])) return 1;
    return rk_is_class_name(ty);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t rk_param_byref_mask(const tree_t * proc, int nparams) {
    uint64_t brm = 0; if (!proc) return 0;
    for (int k = 0; k < nparams && k < 64 && (1 + k) < proc->n; k++) {
        const tree_t * pv = proc->c[1 + k];
        if (pv && pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT && pv->c[0]->v.sval && !strcmp(pv->c[0]->v.sval, "@")) brm |= (1ULL << k);
    }
    return brm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_name_is_byref(rcx_t * cx, const char * name) {
    if (!cx || !cx->cur_proc || !cx->cur_byref_mask || !name) return 0;
    for (int k = 0; k < cx->cur_nparams && k < 64 && (1 + k) < cx->cur_proc->n; k++) {
        if (!((cx->cur_byref_mask >> k) & 1ULL)) continue;
        const tree_t * pv = cx->cur_proc->c[1 + k];
        if (pv && pv->v.sval && !strcmp(pv->v.sval, name)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t rk_callee_byref_mask(const char * nm) {
    if (!nm) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, nm)) return g_stage2.proc_table[i].byref_mask;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_method_is_stub(const tree_t * m) {
    if (!m || m->t != TT_SUB_DECL) return 0; int bs = (int) m->v.ival; if (bs < 1) bs = 1; if (m->n - bs != 1) return 0; const tree_t * b = m->c[bs]; return b && b->t == TT_YADA;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_fld_bare(const char * s) { return (s && (s[0] == '.' || s[0] == '!')) ? s + 1 : s; }
static int rk_fld_priv(const char * s) { return (s && s[0] == '!') ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * rk_find_type_decl(const tree_t * prog, const char * name) {
    if (!prog || !name) return NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || (d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL)) continue;
        const char * tn = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (tn && !strcmp(tn, name)) return d;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_type_provides_real_method(const tree_t * decl, const char * mname) {
    if (!decl || !mname) return 0;
    for (int j = 1; j < decl->n; j++) {
        const tree_t * ch = decl->c[j];
        if (!ch || ch->t != TT_SUB_DECL || rk_method_is_stub(ch)) continue;
        const char * nm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
        if (nm && !strcmp(nm, mname)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_γ_to_β(nd, t); else lc_γ_to(nd, t); }
static void ω_to(IR_t * nd, IR_t * t) { if (t && ir_is_generator_kind(t->op)) lc_ω_to_β(nd, t); else lc_ω_to(nd, t); }
static tree_t * leaf_sval2(tree_e kind, const char * s) { tree_t * n = ast_node_new(kind); n->v.sval = (char *)s; return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(rcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) {
    IR_t * nd = lc_build(cx->g, op, γ, ω);
    if (γ && ir_is_generator_kind(γ->op)) lc_γ_to_β(nd, γ);
    if (ω && ir_is_generator_kind(ω->op)) lc_ω_to_β(nd, ω);
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_binop(tree_e tt) {
    switch (tt) { case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW: case TT_CAT: case TT_XREP: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_relop(tree_e tt) {
    switch (tt) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: case TT_LEQ: case TT_LNE: case TT_LLT: case TT_LLE: case TT_LGT: case TT_LGE: return 1; default: return 0; }
}
static IR_t * lower_rv(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res);
static int rk_proc_known(const char * name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_excise(rcx_t * cx, IR_t * γ, IR_t * ω, IR_t ** res) { IR_t * nd = build(cx, IR_EXCISED, γ, ω); if (res) *res = nd; return nd; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rblock(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    if (!t) return build(cx, IR_SUCCEED, γ, ω);
    if (t->t != TT_SEQ && t->t != TT_PROGRAM && t->t != TT_SEQ_EXPR) { IR_t * r = NULL; return lower_rv(cx, t, γ, ω, &r); }
    if (t->n == 0) return build(cx, IR_SUCCEED, γ, ω);
    IR_t * succ = γ; IR_t * entry = γ;
    for (int i = t->n - 1; i >= 0; i--) {
        const tree_t * s = t->c[i];
        if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
        if (s && s->t == TT_CATCH) continue;
        if (s && (s->t == TT_SEQ || s->t == TT_SEQ_EXPR || s->t == TT_PROGRAM) && s->n == 0) continue;
        IR_t * gs = succ, * gw = ω;
        if (cx->try_catch) {
            IR_t * pγ = build(cx, IR_CALL, cx->try_catch, succ); IR_LIT(pγ).sval = "exc_check";
            IR_t * pω = build(cx, IR_CALL, cx->try_catch, ω); IR_LIT(pω).sval = "exc_check";
            gs = pγ; gw = pω;
        }
        IR_t * r = NULL; IR_t * e = lower_rv(cx, s, gs, gw, &r);
        if (e && ir_is_generator_kind(e->op)) { IR_t * tramp = build(cx, IR_GOTO, NULL, NULL); lc_γ_to(tramp, e); lc_ω_to(tramp, e); e = tramp; }
        if (e) { entry = e; succ = e; }
    }
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rcall(rcx_t * cx, const tree_t * t, const char * nm, int from, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = nm;
    int nargs = t->n - from; IR_t * prev = NULL; IR_t * entry = nd;
    uint64_t brm = rk_callee_byref_mask(nm);
    for (int k = 0; k < nargs; k++) {
        const tree_t * argt = t->c[from + k];
        IR_t * ar = NULL; IR_t * ae;
        if (((brm >> k) & 1ULL) && argt && argt->t == TT_VAR && argt->v.sval && !rk_name_is_byref(cx, argt->v.sval)) {
            IR_t * vr = build(cx, IR_VAR_REF, (k == nargs - 1) ? nd : NULL, ω); IR_LIT(vr).sval = argt->v.sval;
            ae = vr; ar = vr;
        } else if (((brm >> k) & 1ULL) && argt && argt->t == TT_VAR && argt->v.sval) {
            IR_t * vr = build(cx, IR_VAR, (k == nargs - 1) ? nd : NULL, ω); IR_LIT(vr).sval = argt->v.sval;
            ae = vr; ar = vr;
        } else {
            ae = lower_rv(cx, argt, (k == nargs - 1) ? nd : NULL, ω, &ar);
        }
        if (k == 0) entry = ae;
        if (prev && ae) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(nd, ar);
    }
    if (res) *res = nd; return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rcall1(rcx_t * cx, const tree_t * recv, const char * nm, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = nm;
    IR_t * ar = NULL; IR_t * ae = lower_rv(cx, recv, nd, ω, &ar);
    if (ar) ir_operand_push(nd, ar);
    if (res) *res = nd; return ae;
}
static IR_t * lower_rcall_skip1(rcx_t * cx, const tree_t * t, const char * nm, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = nm;
    int total = t->n - 1; IR_t * prev = NULL; IR_t * entry = nd; int k = 0;
    for (int ci = 0; ci < t->n; ci++) {
        if (ci == 1) continue;
        IR_t * ar = NULL; IR_t * ae = lower_rv(cx, t->c[ci], (k == total - 1) ? nd : NULL, ω, &ar);
        if (k == 0) entry = ae;
        if (prev && ae) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(nd, ar);
        k++;
    }
    if (res) *res = nd; return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_divis_desugar(const tree_t * t) {
    tree_t * md = ast_node_new(TT_MOD); ast_push(md, (tree_t *) t->c[0]); ast_push(md, (tree_t *) t->c[1]);
    tree_t * z = ast_node_new(TT_ILIT); z->v.ival = 0;
    tree_t * eq = ast_node_new(TT_EQ); ast_push(eq, md); ast_push(eq, z); return eq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_cond(rcx_t * cx, const tree_t * c, IR_t * on_true, IR_t * on_false) {
    if (c && c->t == TT_STMT) { const tree_t * sub = stmt_subj(c); if (sub) c = sub; }
    if (!c) return on_true;
    if (c->t == TT_DIVIS && c->n > 1) return lower_cond(cx, rk_divis_desugar(c), on_true, on_false);
    if (rk_is_relop(c->t) && c->n > 1) {
        IR_t * op = build(cx, IR_BINOP_TEST, on_true, on_false); IR_LIT(op).ival = lc_binop_code(c->t);
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower_rv(cx, c->c[0], NULL, on_false, &lr);
        IR_t * eb = lower_rv(cx, c->c[1], op, on_false, &rr);
        γ_to(lr, eb); ir_operand_push(op, lr); ir_operand_push(op, rr); return ea;
    }
    if (c->t == TT_NOT && c->n > 0) return lower_cond(cx, c->c[0], on_false, on_true);
    if (c->t == TT_SEQ && c->n > 1) { IR_t * rhs = lower_cond(cx, c->c[1], on_true, on_false); return lower_cond(cx, c->c[0], rhs, on_false); }
    IR_t * bk = build(cx, IR_CALL, on_true, on_false); IR_LIT(bk).sval = "__rk_bool";
    IR_t * r = NULL; IR_t * e = lower_rv(cx, c, bk, on_false, &r);
    if (r) ir_operand_push(bk, r);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_field_set(rcx_t * cx, const tree_t * lhs, const tree_t * rhs, IR_t * γ, IR_t * ω, IR_t ** res) {
    const char * fname = (lhs->t == TT_TWIGIL_FIELD) ? lhs->v.sval : ((lhs->n > 1 && lhs->c[1]) ? lhs->c[1]->v.sval : lhs->v.sval);
    IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = (lhs->t == TT_TWIGIL_FIELD) ? "field_set" : "field_set_pub";
    IR_t * nl = build(cx, IR_LIT_STRING, NULL, ω); IR_LIT(nl).sval = fname;
    IR_t * or_ = NULL; IR_t * entry;
    if (lhs->t == TT_TWIGIL_FIELD) { IR_t * sv = build(cx, IR_VAR, nl, ω); IR_LIT(sv).sval = "self"; or_ = sv; entry = sv; }
    else { entry = lower_rv(cx, lhs->c[0], nl, ω, &or_); }
    IR_t * vr = NULL; IR_t * ev = lower_rv(cx, rhs, nd, ω, &vr);
    γ_to(nl, ev);
    if (or_) ir_operand_push(nd, or_);
    ir_operand_push(nd, nl);
    if (vr) ir_operand_push(nd, vr);
    if (res) *res = nd; return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_take_list(const tree_t * g, const tree_t ** out, int max) {
    const tree_t * gb = (g && g->n > 0) ? g->c[0] : NULL; int n = 0;
    if (gb && gb->t == TT_SUSPEND) { if (gb->n < 1 || !gb->c[0]) return -1; out[n++] = gb->c[0]; return n; }
    if (!gb || gb->t != TT_SEQ_EXPR) return -1;
    for (int i = 0; i < gb->n; i++) { const tree_t * s = gb->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (sub) s = sub; }
        if (!s || s->t != TT_SUSPEND || s->n < 1 || !s->c[0] || n >= max) return -1; out[n++] = s->c[0]; }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_xf_body(rcx_t * cx, const tree_t * xf, int xfk, const char * vname, IR_t * valprod, const tree_t * body, IR_t * pump, IR_t * ω) {
    IR_t * bentry = lower_rblock(cx, body, pump, ω);
    IR_t * av = build(cx, IR_ASSIGN, bentry, ω); IR_LIT(av).sval = vname;
    if (xfk == 1) {
        IR_t * fr = NULL; IR_t * fe = lower_rv(cx, xf, av, ω, &fr); if (fr) ir_operand_push(av, fr);
        IR_t * au = build(cx, IR_ASSIGN, fe, ω); IR_LIT(au).sval = "_"; ir_operand_push(au, valprod);
        return au; }
    if (xfk == 2) {
        IR_t * vr = build(cx, IR_VAR, av, ω); IR_LIT(vr).sval = "_"; ir_operand_push(av, vr);
        IR_t * centry = lower_cond(cx, xf, vr, pump);
        IR_t * au = build(cx, IR_ASSIGN, centry, ω); IR_LIT(au).sval = "_"; ir_operand_push(au, valprod);
        return au; }
    ir_operand_push(av, valprod);
    return av;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_rv(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * dummy = NULL; if (!res) res = &dummy;
    if (!t) { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    if (t->t == TT_DIVIS && t->n > 1) { return lower_rv(cx, rk_divis_desugar(t), γ, ω, res); }
    if (rk_is_relop(t->t)) {
        if (t->n < 2) return rk_excise(cx, γ, ω, res);
        IR_t * op = build(cx, IR_BINOP_RELOP_VAL, γ, ω); IR_LIT(op).ival = lc_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower_rv(cx, t->c[0], NULL, ω, &lr); IR_t * eb = lower_rv(cx, t->c[1], op, ω, &rr);
        γ_to(lr, eb); ir_operand_push(op, lr); ir_operand_push(op, rr); *res = op; return ea; }
    if (t->t == TT_DIV && t->n > 1) { return lower_rcall(cx, t, "__rk_div", 0, γ, ω, res); }
    if (rk_is_binop(t->t)) {
        IR_t * op = build(cx, IR_BINOP, γ, ω); IR_LIT(op).ival = lc_binop_code(t->t);
        IR_t * lr = NULL, * rr = NULL; IR_t * ea = lower_rv(cx, t->c[0], NULL, ω, &lr); IR_t * eb = lower_rv(cx, t->c[1], op, ω, &rr);
        γ_to(lr, eb); ir_operand_push(op, lr); ir_operand_push(op, rr); *res = op; return ea; }
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, γ, ω); IR_LIT(nd).ival = t->v.ival; *res = nd; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, γ, ω); IR_LIT(nd).dval = t->v.dval; *res = nd; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd; }
    case TT_NUL: { IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "__rk_undef"; *res = nd; return nd; }
    case TT_MNS: { IR_t * nd = build(cx, IR_UNOP, γ, ω); IR_LIT(nd).ival = (long long) TT_MNS;
        IR_t * r = NULL; IR_t * e = lower_rv(cx, t->c[0], nd, ω, &r); ir_operand_push(nd, r); *res = nd; return e; }
    case TT_VAR: {
        if (rk_is_grammar_name(t->v.sval) || rk_is_class_name(t->v.sval)) {
            IR_t * nd = build(cx, IR_LIT_STRING, γ, ω); IR_LIT(nd).sval = t->v.sval; *res = nd; return nd;
        }
        if (rk_name_is_byref(cx, t->v.sval)) {
            IR_t * dr = build(cx, IR_DEREF, γ, ω);
            IR_t * v = build(cx, IR_VAR, dr, ω); IR_LIT(v).sval = t->v.sval;
            ir_operand_push(dr, v); *res = dr; return v;
        }
        IR_t * nd = build(cx, IR_VAR, γ, ω);
        IR_LIT(nd).sval = t->v.sval; *res = nd; return nd;
    }
    case TT_ASSIGN: if (t->n > 1 && t->c[0] && (t->c[0]->t == TT_FIELD || t->c[0]->t == TT_TWIGIL_FIELD)) {
        return lower_field_set(cx, t->c[0], t->c[1], γ, ω, res); }
        else if (t->n > 1 && t->c[0] && t->c[0]->t == TT_VAR) {
        const tree_t * rhs = t->c[1];
        if (rhs && rhs->t == TT_FNC && rhs->n > 1 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "pop") && rhs->c[1] && rhs->c[1]->t == TT_VAR) {
            IR_t * asA = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asA).sval = rhs->c[1]->v.sval;
            IR_t * r2 = NULL; IR_t * einit = lower_rcall(cx, rhs, "arr_init", 1, asA, ω, &r2); if (r2) ir_operand_push(asA, r2);
            IR_t * asP = build(cx, IR_ASSIGN, einit, ω); IR_LIT(asP).sval = t->c[0]->v.sval;
            IR_t * r3 = NULL; IR_t * elast = lower_rcall(cx, rhs, "arr_last", 1, asP, ω, &r3); if (r3) ir_operand_push(asP, r3); *res = asA; return elast; }
        if (rhs && rk_is_relop(rhs->t)) {
            IR_t * nd = build(cx, IR_ASSIGN, γ, ω); IR_LIT(nd).sval = t->c[0]->v.sval;
            IR_t * op = build(cx, IR_BINOP_RELOP_VAL, nd, ω); IR_LIT(op).ival = lc_binop_code(rhs->t);
            IR_t * lr = NULL, * rr = NULL;
            IR_t * ea = lower_rv(cx, rhs->c[0], NULL, ω, &lr);
            IR_t * eb = lower_rv(cx, rhs->c[1], op, ω, &rr);
            γ_to(lr, eb); ir_operand_push(op, lr); ir_operand_push(op, rr);
            ir_operand_push(nd, op); *res = nd; return ea; }
        IR_t * nd = build(cx, IR_ASSIGN, γ, ω); IR_LIT(nd).sval = t->c[0]->v.sval;
        IR_t * rr = NULL; IR_t * e = lower_rv(cx, t->c[1], nd, ω, &rr); if (rr) ir_operand_push(nd, rr); *res = nd; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_DECL: if (t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) {
        IR_t * nd = build(cx, IR_ASSIGN, γ, ω); IR_LIT(nd).sval = t->c[1]->v.sval;
        IR_t * rr = NULL; IR_t * e; if (t->n > 2 && t->c[2]) { e = lower_rv(cx, t->c[2], nd, ω, &rr); }
        else { IR_t * u = build(cx, IR_CALL, nd, ω); IR_LIT(u).sval = "__rk_undef"; rr = u; e = u; }
        if (rr) ir_operand_push(nd, rr); *res = nd; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_ARR_SET: if (t->n > 2 && t->c[0] && t->c[0]->t == TT_VAR) {
        const char * vn = t->c[0]->v.sval;
        if (rk_name_is_byref(cx, vn)) {
            IR_t * asn = build(cx, IR_ASSIGN_VAR, γ, ω);
            IR_t * vr = build(cx, IR_VAR, NULL, ω); IR_LIT(vr).sval = vn;
            IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "arr_set_pure", 0, asn, ω, &r2);
            γ_to(vr, e ? e : asn);
            ir_operand_push(asn, vr); if (r2) ir_operand_push(asn, r2);
            *res = asn; return vr; }
        IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = vn;
        IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "arr_set_pure", 0, as, ω, &r2); if (r2) ir_operand_push(as, r2); *res = as; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_USE_DECL: { IR_t * nd = build(cx, IR_SUCCEED, γ, ω); *res = nd; return nd; }
    case TT_SAY: case TT_SAY_FH:
        if (t->n == 1 && t->c[0] && t->c[0]->t == TT_VAR &&
            (rk_is_arrlit_scalar(t->c[0]->v.sval) || rk_is_array_name(t->c[0]->v.sval)))
            return lower_rcall(cx, t, "rk_write_arr", 0, γ, ω, res);
        return lower_rcall(cx, t, "rk_write", 0, γ, ω, res);
    case TT_PRINT: case TT_PRINT_FH: return lower_rcall(cx, t, "rk_writes", 0, γ, ω, res);
    case TT_DIE: return lower_rcall(cx, t, "die", 0, γ, ω, res);
    case TT_TRY: {
        const tree_t * body = (t->n > 0) ? t->c[0] : NULL; const tree_t * handler = (t->n > 1) ? t->c[1] : NULL;
        if (!handler && body && (body->t == TT_SEQ || body->t == TT_SEQ_EXPR || body->t == TT_PROGRAM))
            for (int i = 0; i < body->n; i++) { const tree_t * s = body->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (sub) s = sub; }
                if (s && s->t == TT_CATCH && s->n > 0) { handler = s->c[0]; break; } }
        IR_t * kexit = build(cx, IR_CALL, γ, ω); IR_LIT(kexit).sval = "try_exit";
        IR_t * save = cx->try_catch; cx->try_catch = NULL;
        IR_t * centry;
        if (handler) {
            IR_t * hentry = lower_rblock(cx, handler, γ, ω);
            IR_t * clr = build(cx, IR_CALL, hentry, ω); IR_LIT(clr).sval = "exc_clear";
            IR_t * asg = build(cx, IR_ASSIGN, clr, ω); IR_LIT(asg).sval = "_";
            IR_t * eg = build(cx, IR_CALL, asg, ω); IR_LIT(eg).sval = "exc_get"; ir_operand_push(asg, eg);
            IR_t * tx = build(cx, IR_CALL, eg, ω); IR_LIT(tx).sval = "try_exit"; centry = tx;
        } else {
            IR_t * clr = build(cx, IR_CALL, γ, ω); IR_LIT(clr).sval = "exc_clear";
            IR_t * tx = build(cx, IR_CALL, clr, ω); IR_LIT(tx).sval = "try_exit"; centry = tx;
        }
        cx->try_catch = centry;
        IR_t * bentry = lower_rblock(cx, body, kexit, ω);
        cx->try_catch = save;
        IR_t * te = build(cx, IR_CALL, bentry, ω); IR_LIT(te).sval = "try_enter";
        *res = kexit; return te; }
    case TT_ANON_BLOCK: { const char * bn = t->v.sval ? t->v.sval : "?";
        tree_t * mc = ast_node_new(TT_FNC); mc->v.sval = (char *)"__blk_ref";
        tree_t * nmv = ast_node_new(TT_VAR); nmv->v.sval = (char *)"__blk_ref"; ast_push(mc, nmv);
        tree_t * bq = ast_node_new(TT_QLIT); bq->v.sval = (char *)bn; ast_push(mc, bq);
        return lower_rcall(cx, mc, "__blk_ref", 1, γ, ω, res); }
    case TT_INVOKE: {
        tree_t * mc = ast_node_new(TT_FNC); mc->v.sval = (char *)"__blk_invoke";
        tree_t * nmv = ast_node_new(TT_VAR); nmv->v.sval = (char *)"__blk_invoke"; ast_push(mc, nmv);
        for (int i = 0; i < t->n; i++) ast_push(mc, t->c[i]);
        return lower_rcall(cx, mc, "__blk_invoke", 1, γ, ω, res); }
    case TT_FNC: { const char * nm = (t->n > 0 && t->c[0]) ? t->c[0]->v.sval : "?";
        if (nm && rk_is_multi_name(nm)) {
            tree_t * mc = ast_node_new(TT_FNC); mc->v.sval = (char *)"__multi_call";
            tree_t * nmv = ast_node_new(TT_VAR); nmv->v.sval = (char *)"__multi_call"; ast_push(mc, nmv);
            tree_t * basq = ast_node_new(TT_QLIT); basq->v.sval = (char *)nm; ast_push(mc, basq);
            for (int i = 1; i < t->n; i++) ast_push(mc, t->c[i]);
            return lower_rcall(cx, mc, "__multi_call", 1, γ, ω, res); }
        if (nm && !strcmp(nm, "any")) nm = "__rk_jct_any"; else if (nm && !strcmp(nm, "all")) nm = "__rk_jct_all";
        else if (nm && !strcmp(nm, "one")) nm = "__rk_jct_one"; else if (nm && !strcmp(nm, "none")) nm = "__rk_jct_none";
        if (nm && !strcmp(nm, "push") && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) {
            IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = t->c[1]->v.sval;
            IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "push_pure", 1, as, ω, &r2); if (r2) ir_operand_push(as, r2); *res = as; return e; }
        if (nm && !strcmp(nm, "hash_set") && t->n > 2 && t->c[1] && (t->c[1]->t == TT_VAR || t->c[1]->t == TT_TWIGIL_FIELD)) {
            const char * vn = t->c[1]->v.sval;
            IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = vn;
            IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "hash_set_pure", 1, as, ω, &r2); if (r2) ir_operand_push(as, r2); *res = as; return e; }
        if (nm && !strcmp(nm, "hash_delete") && t->n > 1 && t->c[1] && (t->c[1]->t == TT_VAR || t->c[1]->t == TT_TWIGIL_FIELD)) {
            const char * vn = t->c[1]->v.sval;
            IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = vn;
            IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "hash_delete_pure", 1, as, ω, &r2); if (r2) ir_operand_push(as, r2); *res = as; return e; }
        return lower_rcall(cx, t, nm, 1, γ, ω, res); }
    case TT_STMT: { const tree_t * sub = stmt_subj(t); return sub ? lower_rv(cx, sub, γ, ω, res) : (build(cx, IR_SUCCEED, γ, ω)); }
    case TT_IF: case TT_UNLESS: {
        IR_t * tentry = (t->n > 1 && t->c[1]) ? lower_rblock(cx, t->c[1], γ, ω) : γ;
        IR_t * eentry = (t->n > 2 && t->c[2]) ? lower_rblock(cx, t->c[2], γ, ω) : γ;
        IR_t * e = (t->t == TT_UNLESS) ? lower_cond(cx, t->c[0], eentry, tentry) : lower_cond(cx, t->c[0], tentry, eentry);
        *res = e; return e; }
    case TT_WHILE: {
        IR_t * LOOP = build(cx, IR_GOTO, NULL, ω);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = LOOP;
        IR_t * bentry = (t->n > 1) ? lower_rblock(cx, t->c[1], LOOP, ω) : LOOP;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        IR_t * centry = lower_cond(cx, t->c[0], bentry, γ);
        bb_src_note(centry, "rk_while_cond", 0);
        γ_to(LOOP, centry); ω_to(LOOP, centry);
        *res = LOOP; return centry; }
    case TT_CLOOP: {
        IR_t * LOOP = build(cx, IR_GOTO, NULL, ω);
        IR_t * incr_entry = lower_rblock(cx, t->c[2], LOOP, ω);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = incr_entry;
        IR_t * bentry = lower_rblock(cx, t->c[3], incr_entry, ω);
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        IR_t * centry = lower_cond(cx, t->c[1], bentry, γ);
        bb_src_note(centry, "rk_cloop_cond", 0);
        if (incr_entry && incr_entry != LOOP) bb_src_note(incr_entry, "rk_cloop_incr", 0);
        γ_to(LOOP, centry); ω_to(LOOP, centry);
        IR_t * ientry = lower_rblock(cx, t->c[0], LOOP, ω);
        *res = LOOP; return ientry; }
    case TT_UNTIL: {
        IR_t * LOOP = build(cx, IR_GOTO, NULL, ω);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = LOOP;
        IR_t * bentry = (t->n > 1) ? lower_rblock(cx, t->c[1], LOOP, ω) : LOOP;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        IR_t * centry = lower_cond(cx, t->c[0], γ, bentry);
        bb_src_note(centry, "rk_until_cond", 0);
        γ_to(LOOP, centry); ω_to(LOOP, centry);
        *res = LOOP; return centry; }
    case TT_REPEAT: if (t->v.ival != 0 && t->n > 1) {
        IR_t * BACK = build(cx, IR_GOTO, NULL, ω);
        IR_t * centry = (t->v.ival == 1) ? lower_cond(cx, t->c[1], BACK, γ) : lower_cond(cx, t->c[1], γ, BACK);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = γ; cx->loop_next = centry;
        IR_t * bentry = lower_rblock(cx, t->c[0], centry, ω);
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        bb_src_note(bentry, "rk_repeat_body", 0);
        bb_src_note(centry, "rk_repeat_cond", 0);
        γ_to(BACK, bentry); ω_to(BACK, bentry);
        *res = BACK; return bentry; }
    else {
        IR_t * LOOP = build(cx, IR_GOTO, NULL, ω);
        IR_t * sv_exit = cx->loop_exit; IR_t * sv_next = cx->loop_next;
        cx->loop_exit = ω; cx->loop_next = LOOP;
        IR_t * bentry = (t->n > 0) ? lower_rblock(cx, t->c[0], LOOP, ω) : LOOP;
        cx->loop_exit = sv_exit; cx->loop_next = sv_next;
        if (bentry && bentry != LOOP) bb_src_note(bentry, "rk_loop_body", 0);
        γ_to(LOOP, bentry); ω_to(LOOP, bentry);
        *res = LOOP; return bentry; }
    case TT_LOOP_BREAK: case TT_LOOP_NEXT: {
        IR_t * tgt = (t->t == TT_LOOP_BREAK) ? cx->loop_exit : cx->loop_next;
        if (!tgt) tgt = ω;
        IR_t * j = build(cx, IR_GOTO, tgt, tgt);
        *res = NULL; return j; }
    case TT_EVERY: if (t->n > 1 && t->c[0] && t->c[0]->t == TT_ITERATE && t->c[0]->n > 0) {
        const char * vname = t->c[0]->v.sval ? t->c[0]->v.sval : "_"; const tree_t * src = t->c[0]->c[0]; const tree_t * body = t->c[1];
        const tree_t * xf = NULL; int xfk = 0;
        if (src && (src->t == TT_MAP || src->t == TT_GREP) && src->n > 1) { xfk = (src->t == TT_MAP) ? 1 : 2; xf = src->c[0]; src = src->c[1]; }
        if (src && src->t == TT_TO && src->n > 1) {
            IR_t * to = build(cx, IR_TO, NULL, γ); IR_LIT(to).sval = "ag";
            IR_t * rlo = NULL, * rhi = NULL;
            IR_t * elo = lower_rv(cx, src->c[0], NULL, ω, &rlo);
            IR_t * ehi = lower_rv(cx, src->c[1], to, ω, &rhi);
            γ_to(rlo, ehi); ir_operand_push(to, rlo); ir_operand_push(to, rhi); if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
            IR_t * inner = rk_xf_body(cx, xf, xfk, vname, to, body, to, ω);
            γ_to(to, inner);
            *res = to; return elo; }
        if (src && src->t == TT_GATHER) {
            const tree_t * takes[64]; int ntk = rk_take_list(src, takes, 64);
            if (ntk > 0) {
                IR_t * succ = γ;
                for (int k = ntk - 1; k >= 0; k--) {
                    IR_t * r = NULL; IR_t * ek = lower_rv(cx, takes[k], NULL, ω, &r);
                    IR_t * inner = rk_xf_body(cx, xf, xfk, vname, r, body, succ, ω);
                    γ_to(r, inner); succ = ek; }
                *res = succ; return succ; } }
        if (src && xfk == 0) {
            static int g_forlist_ctr = 0; int fid = ++g_forlist_ctr;
            char iname[48]; snprintf(iname, sizeof iname, "__foridx_%d", fid); const char * in = intern(iname);
            const char * ln; int need_matr = 0;
            if (src->t == TT_VAR && src->v.sval) { ln = src->v.sval; }
            else { char lname[48]; snprintf(lname, sizeof lname, "__forlist_%d", fid); ln = intern(lname); need_matr = 1; }
            tree_t * elc = ast_node_new(TT_FNC); elc->v.sval = (char *)"elems";
            ast_push(elc, leaf_sval2(TT_VAR, "elems")); ast_push(elc, leaf_sval2(TT_VAR, ln));
            tree_t * one = ast_node_new(TT_ILIT); one->v.ival = 1;
            tree_t * hi = ast_node_new(TT_SUB); ast_push(hi, elc); ast_push(hi, one);
            tree_t * lo = ast_node_new(TT_ILIT); lo->v.ival = 0;
            tree_t * bind = ast_node_new(TT_DECL); ast_push(bind, leaf_sval2(TT_VAR, "__decl"));
            ast_push(bind, leaf_sval2(TT_VAR, vname));
            tree_t * ag = ast_node_new(TT_ARR_GET); ast_push(ag, leaf_sval2(TT_VAR, ln)); ast_push(ag, leaf_sval2(TT_VAR, in));
            ast_push(bind, ag);
            tree_t * nb = ast_node_new(TT_SEQ_EXPR); ast_push(nb, bind);
            if (body) { if (body->t == TT_SEQ || body->t == TT_SEQ_EXPR || body->t == TT_PROGRAM) { for (int k = 0; k < body->n; k++) ast_push(nb, body->c[k]); } else ast_push(nb, (tree_t *)body); }
            tree_t * fr = ast_node_new(TT_FOR_RANGE);
            ast_push(fr, leaf_sval2(TT_VAR, in)); ast_push(fr, lo); ast_push(fr, hi); ast_push(fr, nb);
            tree_t * ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(fr, ex);
            if (need_matr) { IR_t * as = build(cx, IR_ASSIGN, NULL, ω); IR_LIT(as).sval = ln;
                IR_t * rr = NULL; IR_t * einit = lower_rv(cx, src, as, ω, &rr); if (rr) ir_operand_push(as, rr);
                IR_t * fre = lower_rv(cx, fr, γ, ω, res); lc_γ_to(as, fre); return einit; }
            return lower_rv(cx, fr, γ, ω, res); }
        return rk_excise(cx, γ, ω, res); }
        return rk_excise(cx, γ, ω, res);
    case TT_CASE: return rk_excise(cx, γ, ω, res);
    case TT_SMATCH: if (t->n > 2 && t->c[2] && t->c[2]->v.sval && strcmp(t->c[2]->v.sval, "subst")) {
        IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = strcmp(t->c[2]->v.sval, "match_global") ? "re_match" : "re_match_global";
        IR_t * sr = NULL, * pr = NULL; IR_t * es = lower_rv(cx, t->c[0], NULL, ω, &sr); IR_t * ep = lower_rv(cx, t->c[1], nd, ω, &pr);
        γ_to(sr, ep); ir_operand_push(nd, sr); ir_operand_push(nd, pr); *res = nd; return es; }
        return rk_excise(cx, γ, ω, res);
    case TT_GATHER: return rk_excise(cx, γ, ω, res);
    case TT_MAP: return rk_excise(cx, γ, ω, res);
    case TT_GREP: return rk_excise(cx, γ, ω, res);
    case TT_REVERSE: return lower_rcall(cx, t, "array_reverse", 0, γ, ω, res);
    case TT_FOR_RANGE: if (t->n > 3 && t->c[0] && t->c[0]->t == TT_VAR) {
        IR_t * va = build(cx, IR_ASSIGN, NULL, ω); IR_LIT(va).sval = t->c[0]->v.sval;
        IR_t * to = build(cx, IR_TO, va, γ); IR_LIT(to).sval = "ag";
        IR_t * rlo = NULL, * rhi = NULL;
        IR_t * elo = lower_rv(cx, t->c[1], NULL, ω, &rlo);
        IR_t * ehi = lower_rv(cx, t->c[2], to, ω, &rhi);
        γ_to(rlo, ehi); ir_operand_push(to, rlo); ir_operand_push(to, rhi); if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
        ir_operand_push(va, to);
        IR_t * bentry = lower_rblock(cx, t->c[3], to, ω);
        γ_to(va, bentry);
        *res = to; return elo; }
        return rk_excise(cx, γ, ω, res);
    case TT_ARR_GET: return lower_rcall(cx, t, "__rk_arr_at", 0, γ, ω, res);
    case TT_HASH_GET: return lower_rcall(cx, t, "hash_get", 0, γ, ω, res);
    case TT_HASH_SET: if (t->n > 2 && t->c[0] && (t->c[0]->t == TT_VAR || t->c[0]->t == TT_TWIGIL_FIELD)) {
        const char * vn = t->c[0]->t == TT_TWIGIL_FIELD ? t->c[0]->v.sval : (t->c[0]->n > 0 && t->c[0]->c[0] ? t->c[0]->c[0]->v.sval : t->c[0]->v.sval);
        IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = vn;
        IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "hash_set_pure", 0, as, ω, &r2); if (r2) ir_operand_push(as, r2); *res = as; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_HASH_EXISTS: return lower_rcall(cx, t, "hash_exists", 0, γ, ω, res);
    case TT_HASH_DELETE: if (t->n > 0 && t->c[0] && (t->c[0]->t == TT_VAR || t->c[0]->t == TT_TWIGIL_FIELD)) {
        const char * vn = t->c[0]->t == TT_TWIGIL_FIELD ? t->c[0]->v.sval : (t->c[0]->n > 0 && t->c[0]->c[0] ? t->c[0]->c[0]->v.sval : t->c[0]->v.sval);
        IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = vn;
        IR_t * r2 = NULL; IR_t * e = lower_rcall(cx, t, "hash_delete_pure", 0, as, ω, &r2); if (r2) ir_operand_push(as, r2); *res = as; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_TO: if (t->n > 1) { IR_t * to = build(cx, IR_TO, γ, ω); IR_LIT(to).sval = "ag";
        IR_t * rlo = NULL, * rhi = NULL;
        IR_t * elo = lower_rv(cx, t->c[0], NULL, ω, &rlo);
        IR_t * ehi = lower_rv(cx, t->c[1], to, ω, &rhi);
        γ_to(rlo, ehi); ir_operand_push(to, rlo); ir_operand_push(to, rhi); if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
        *res = to; return elo; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_SORT: return lower_rcall(cx, t, "array_sort", 0, γ, ω, res);
    case TT_CAPTURE: return lower_rcall(cx, t, "re_capture", 0, γ, ω, res);
    case TT_NAMED_CAPTURE: return lower_rcall(cx, t, "re_named_capture", 0, γ, ω, res);
    case TT_SEQ: case TT_PROGRAM: case TT_SEQ_EXPR: { IR_t * b = lower_rblock(cx, t, γ, ω); *res = b; return b; }
    case TT_METHCALL: {
        const char * mname = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : NULL;
        if (mname && t->c[0] && t->c[0]->t == TT_VAR) {
            if (!strcmp(mname, "push") || !strcmp(mname, "unshift")) {
                const char * fn = !strcmp(mname, "push") ? "push_pure" : "unshift_pure";
                IR_t * as = build(cx, IR_ASSIGN, γ, ω); IR_LIT(as).sval = t->c[0]->v.sval;
                IR_t * r2 = NULL; IR_t * e = lower_rcall_skip1(cx, t, fn, as, ω, &r2); if (r2) ir_operand_push(as, r2);
                *res = as; return e;
            }
            if (!strcmp(mname, "pop") || !strcmp(mname, "shift")) {
                const char * mut_fn = !strcmp(mname, "pop") ? "arr_init" : "arr_tail";
                const char * val_fn = !strcmp(mname, "pop") ? "arr_last" : "__rk_arr_first";
                IR_t * asA = build(cx, IR_ASSIGN, γ, ω); IR_LIT(asA).sval = t->c[0]->v.sval;
                IR_t * rmut = NULL; IR_t * emut = lower_rcall1(cx, t->c[0], mut_fn, asA, ω, &rmut); if (rmut) ir_operand_push(asA, rmut);
                IR_t * rval = NULL; IR_t * eval_ = lower_rcall1(cx, t->c[0], val_fn, emut, ω, &rval);
                *res = rval; return eval_;
            }
        }
        return lower_rcall(cx, t, "meth_call", 0, γ, ω, res);
    }
    case TT_NEW: return lower_rcall(cx, t, "obj_new", 0, γ, ω, res);
    case TT_TWIGIL_FIELD: {
        IR_t * nd = build(cx, IR_FIELD_GET, γ, ω); IR_LIT(nd).sval = t->v.sval; IR_t * sv = build(cx, IR_VAR, nd, ω); IR_LIT(sv).sval = "self"; ir_operand_push(nd, sv); *res = nd; return sv;
    }
    case TT_FIELD: { const char * fname = (t->n > 1 && t->c[1]) ? t->c[1]->v.sval : t->v.sval;
        IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "field_get_pub";
        IR_t * nl = build(cx, IR_LIT_STRING, nd, ω); IR_LIT(nl).sval = fname;
        IR_t * or_ = NULL; IR_t * eo = lower_rv(cx, t->c[0], nl, ω, &or_);
        if (or_) ir_operand_push(nd, or_);
        ir_operand_push(nd, nl);
        *res = nd; return eo; }
    case TT_SUSPEND: return rk_excise(cx, γ, ω, res);
    case TT_TERNARY: if (t->n > 2) {
        static int tern_n = 0; char tn[32]; snprintf(tn, sizeof tn, "$?tern%d", tern_n++); const char * tname = lp_strdup(tn);
        IR_t * jv = build(cx, IR_VAR, γ, ω); IR_LIT(jv).sval = tname;
        IR_t * at = build(cx, IR_ASSIGN, jv, ω); IR_LIT(at).sval = tname;
        IR_t * af = build(cx, IR_ASSIGN, jv, ω); IR_LIT(af).sval = tname;
        IR_t * rt_ = NULL, * rf_ = NULL;
        IR_t * et = lower_rv(cx, t->c[1], at, ω, &rt_); if (rt_) ir_operand_push(at, rt_);
        IR_t * ef = lower_rv(cx, t->c[2], af, ω, &rf_); if (rf_) ir_operand_push(af, rf_);
        IR_t * e = lower_cond(cx, t->c[0], et, ef);
        *res = jv; return e; }
        { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    case TT_RETURN: {
        if (t->n > 0 && t->c[0]) {
            IR_t * nd = build(cx, IR_RETURN, γ, ω); IR_t * r = NULL; IR_t * e = lower_rv(cx, t->c[0], nd, ω, &r); ir_operand_push(nd, r ? r : e); *res = nd; return e;
        } IR_t * nd = build(cx, IR_RETURN, γ, ω);
        *res = nd; return nd;
    }
    default: { IR_t * s = build(cx, IR_SUCCEED, γ, ω); *res = s; return s; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_reclassify_calls(void) {
    extern int rt_builtin_is_known(const char *);
    for (int gi = 0; gi < g_stage2.bbp.count; gi++) {
        IR_graph_t * g = g_stage2.bbp.table[gi]; if (!g) continue;
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (!nd || nd->op != IR_CALL) continue;
            const char * fn = IR_LIT(nd).sval; if (!fn || !fn[0]) continue;
            if (!strcmp(fn, "__rk_bool")) continue;
            if (rk_proc_known(fn)) nd->op = IR_CALL_PROC_STAGED;
            else if (rt_builtin_is_known(fn)) nd->op = IR_CALL_BUILTIN;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_register_proc(const tree_t * proc, const char * name, int nparams) {
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name     = lp_strdup(name);
    g_stage2.proc_table[pi].proc     = (tree_t *)(intptr_t) proc;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].bb_idx   = -1;
    g_stage2.proc_table[pi].nparams  = nparams;
    g_stage2.proc_table[pi].byref_mask = rk_param_byref_mask(proc, nparams);
    { const tree_t * lp = (proc && nparams > 0 && nparams < proc->n) ? proc->c[nparams] : (const tree_t *)0;
      if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "*@")) {
          g_stage2.proc_table[pi].is_variadic = 1; g_stage2.proc_table[pi].rest_kind = 1; }
      else if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "**@")) {
          g_stage2.proc_table[pi].is_variadic = 1; g_stage2.proc_table[pi].rest_kind = 2; }
      else if (lp && lp->n > 0 && lp->c[0] && lp->c[0]->t == TT_QLIT && lp->c[0]->v.sval && !strcmp(lp->c[0]->v.sval, "*%")) {
          g_stage2.proc_table[pi].named_rest = nparams; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_proc_known(const char * name) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++)
        if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_grammars(const tree_t * prog) {
    extern void rt_grammar_register(const char *qname, const char *body, int flavor);
    g_rk_gram_n = 0;
    if (!prog) return;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_GRAMMAR_DECL) continue;
        const char * gname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!gname || !*gname) continue;
        if (g_rk_gram_n < RK_GRAM_MAX && !rk_is_grammar_name(gname)) g_rk_gram_names[g_rk_gram_n++] = gname;
        for (int j = 1; j < d->n; j++) {
            const tree_t * rd = d->c[j];
            if (!rd || rd->t != TT_REGEX_DECL) continue;
            const char * rname = (rd->n > 0 && rd->c[0] && rd->c[0]->v.sval) ? rd->c[0]->v.sval : NULL;
            const char * body  = (rd->n > 1 && rd->c[1] && rd->c[1]->v.sval) ? rd->c[1]->v.sval : NULL;
            if (!rname || !body) continue;
            char qn[256]; snprintf(qn, sizeof qn, "%s::%s", gname, rname);
            rt_grammar_register(qn, body, (int) rd->v.ival);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_register_classes(const tree_t * prog) {
    extern void record_register(const char *spec);
    if (!prog) return;
    g_rk_class_n = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || (d->t != TT_CLASS_DECL && d->t != TT_ROLE_DECL)) continue;
        const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!cname || !*cname) continue;
        if (g_rk_class_n < RK_GRAM_MAX && !rk_is_class_name(cname)) g_rk_class_names[g_rk_class_n++] = cname;
        char spec[512]; int pos = 0;
        pos += snprintf(spec + pos, sizeof(spec) - pos, "%s(", cname);
        int first_field = 1;
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t == TT_SUB_DECL) continue;
            if (!first_field) { if (pos < (int)sizeof(spec) - 2) spec[pos++] = ','; }
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : "");
            pos += snprintf(spec + pos, sizeof(spec) - pos, "%s", fn);
            first_field = 0;
        }
        if (pos < (int)sizeof(spec) - 1) spec[pos++] = ')';
        spec[pos] = '\0';
        record_register(spec);
        extern void dat_add_method(const char *type, const char *mname);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_SUB_DECL || rk_method_is_stub(ch)) continue;
            const char * mname = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
            if (!mname) continue;
            const char * dollar = strchr(mname, '$');
            if (dollar) { char base[128]; int bl = (int)(dollar - mname); if (bl > 127) bl = 127; memcpy(base, mname, bl); base[bl] = '\0'; dat_add_method(cname, base); }
            else dat_add_method(cname, mname);
            if (!strcmp(mname, "BUILD")) {
                extern void dat_set_build_key(const char *cls, const char *key);
                int bs = (int)ch->v.ival; if (bs < 1) bs = 1; dat_set_build_key(cname, NULL);
                for (int p = 1; p < bs; p++) { const tree_t * pp = ch->c[p]; const char * pk = (pp && pp->v.sval) ? pp->v.sval : NULL; if (pk && *pk) dat_set_build_key(cname, pk); }
            }
        }
        extern void dat_set_field_default_i(const char *cls, const char *field, int64_t v);
        extern void dat_set_field_default_s(const char *cls, const char *field, const char *v);
        extern void dat_set_field_default_r(const char *cls, const char *field, double v);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_HAS_DECL || ch->n < 1) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); const tree_t * dv = ch->c[0]; if (!dv) continue;
            if (dv->t == TT_ILIT) dat_set_field_default_i(cname, fn, dv->v.ival);
            else if (dv->t == TT_QLIT) dat_set_field_default_s(cname, fn, dv->v.sval);
            else if (dv->t == TT_FLIT) dat_set_field_default_r(cname, fn, dv->v.dval);
        }
        extern void dat_set_field_required(const char *cls, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_HAS_DECL || ch->n != 0) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (*fn) dat_set_field_required(cname, fn);
        }
        extern void dat_set_field_rw(const char *cls, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_RW_DECL) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (*fn) dat_set_field_rw(cname, fn);
        }
        extern void dat_add_handles(const char *cls, const char *meth, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t != TT_HANDLES_DECL) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (!*fn) continue;
            const char * words = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : "";
            char wbuf[256]; snprintf(wbuf, sizeof wbuf, "%s", words); char * sp = wbuf; char * tok;
            while ((tok = strtok(sp, " \t")) != (char *)0) { sp = (char *)0; if (*tok) dat_add_handles(cname, tok, fn); }
        }
        extern void dat_set_field_sigil(const char *cls, const char *field, int sig);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || (ch->t != TT_ARR_DECL && ch->t != TT_HASH_DECL)) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (*fn) dat_set_field_sigil(cname, fn, ch->t == TT_ARR_DECL ? '@' : '%');
        }
        extern void dat_set_field_priv(const char *cls, const char *field);
        for (int j = 1; j < d->n; j++) {
            const tree_t * ch = d->c[j];
            if (!ch || ch->t == TT_SUB_DECL) continue;
            if (!rk_fld_priv(ch->v.sval)) continue;
            const char * fn = rk_fld_bare(ch->v.sval ? ch->v.sval : ""); if (*fn) dat_set_field_priv(cname, fn);
        }
    }
    extern void class_inherit_multi(const char *child, const char **parents, int nparents);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_CLASS_DECL) continue;
        const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        const char * pname = d->v.sval;
        if (cname && pname && *pname) {
            char pbuf[8][64]; const char * pl[8]; int np = 0; char rbuf[8][64]; int nr = 0; const char * s = pname;
            while (*s) { const char * nx = strchr(s, '\x01'); size_t L = nx ? (size_t)(nx - s) : strlen(s);
                if (L >= 1) { char tag = s[0]; const char * nm = s + 1; size_t NL = L - 1; if (NL > 63) NL = 63;
                    if (tag == 'i' && np < 8) { memcpy(pbuf[np], nm, NL); pbuf[np][NL] = '\0'; pl[np] = pbuf[np]; np++; }
                    else if (tag == 'd' && nr < 8) { memcpy(rbuf[nr], nm, NL); rbuf[nr][NL] = '\0'; nr++; } }
                if (!nx) break; s = nx + 1; }
            if (np > 0) class_inherit_multi(cname, pl, np);
            extern void class_compose_role(const char *child, const char *role);
            for (int ri = 0; ri < nr; ri++) class_compose_role(cname, rbuf[ri]);
            extern void rt_script_die_surface(const char *msg);
            const tree_t * cdecl = rk_find_type_decl(prog, cname);
            for (int ri = 0; ri < nr; ri++) {
                const tree_t * rdecl = rk_find_type_decl(prog, rbuf[ri]); if (!rdecl) continue;
                for (int j = 1; j < rdecl->n; j++) {
                    const tree_t * ch = rdecl->c[j];
                    if (!ch || ch->t != TT_SUB_DECL || !rk_method_is_stub(ch)) continue;
                    const char * rm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
                    if (!rm) continue;
                    int sat = rk_type_provides_real_method(cdecl, rm);
                    for (int rj = 0; rj < nr && !sat; rj++) { const tree_t * od = rk_find_type_decl(prog, rbuf[rj]); if (rk_type_provides_real_method(od, rm)) sat = 1; }
                    if (!sat) {
                        char _m[256]; snprintf(_m, sizeof _m, "Method '%s' must be implemented by class %s because it is required by role %s", rm, cname, rbuf[ri]); rt_script_die_surface(_m);
                    }
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_nested_subs(const tree_t * d) {
    if (!d || d->t != TT_SUB_DECL) return;
    int np = (int) d->v.ival; if (np < 0) np = 0;
    for (int k = np + 1; k < d->n; k++) {
        const tree_t * ch = d->c[k];
        if (ch && ch->t == TT_STMT) { const tree_t * sub = stmt_subj(ch); if (!sub) continue; ch = sub; }
        if (!ch || ch->t != TT_SUB_DECL) continue;
        const char * nm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
        if (!nm || !*nm) continue;
        if (!rk_proc_known(nm)) rk_register_proc(ch, nm, (int) ch->v.ival);
        rk_discover_nested_subs(ch);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_discover_procs(const tree_t * prog) {
    if (!prog) return;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d) continue;
        if (d->t == TT_SUB_DECL) {
            const char * nm = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
            if (!nm) continue;
            int np = (int) d->v.ival;
            rk_register_proc(d, nm, np);
        } else if (d->t == TT_CLASS_DECL || d->t == TT_ROLE_DECL) {
            const char * cname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
            if (!cname || !*cname) continue;
            for (int j = 1; j < d->n; j++) {
                const tree_t * ch = d->c[j];
                if (!ch || ch->t != TT_SUB_DECL || rk_method_is_stub(ch)) continue;
                const char * mname = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
                if (!mname) continue;
                char qname[256]; snprintf(qname, sizeof qname, "%s__%s", cname, mname);
                int np = (int) ch->v.ival;
                rk_register_proc(ch, qname, np);
            }
        } else if (d->t == TT_MODULE_DECL) {
            for (int j = 1; j < d->n; j++) {
                const tree_t * ch = d->c[j];
                if (ch && ch->t == TT_STMT) { const tree_t * sub = stmt_subj(ch); if (!sub) continue; ch = sub; }
                if (!ch || ch->t != TT_SUB_DECL) continue;
                const char * nm = (ch->n > 0 && ch->c[0] && ch->c[0]->v.sval) ? ch->c[0]->v.sval : NULL;
                if (!nm) continue;
                rk_register_proc(ch, nm, (int) ch->v.ival);
            }
        }
    }
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (d && d->t == TT_SUB_DECL) rk_discover_nested_subs(d);
        else if (d && d->t == TT_MODULE_DECL) {
            for (int j = 1; j < d->n; j++) {
                const tree_t * ch = d->c[j];
                if (ch && ch->t == TT_STMT) { const tree_t * sub = stmt_subj(ch); if (!sub) continue; ch = sub; }
                if (ch && ch->t == TT_SUB_DECL) rk_discover_nested_subs(ch);
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_prologue_target(const tree_t * s) {
    if (!s || s->t != TT_UNLESS || s->n < 1 || !s->c[0]) return NULL;
    const tree_t * mc = s->c[0];
    if (mc->t != TT_METHCALL || mc->n < 2 || !mc->c[0] || !mc->c[1] || !mc->c[1]->v.sval || strcmp(mc->c[1]->v.sval, "defined")) return NULL;
    return (mc->c[0]->t == TT_VAR) ? mc->c[0]->v.sval : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_raku_proc(const tree_t * prog, const tree_t * pd) {
    IR_graph_t * g = IR_alloc(8192); rcx_t cx; cx.g = g; cx.try_catch = NULL; cx.loop_exit = NULL; cx.loop_next = NULL;
    cx.cur_proc = pd; cx.cur_byref_mask = 0; cx.cur_nparams = 0;
    for (int _pbi = 0; pd && _pbi < g_stage2.proc_count; _pbi++) if (g_stage2.proc_table[_pbi].proc == pd) {
        cx.cur_byref_mask = g_stage2.proc_table[_pbi].byref_mask; cx.cur_nparams = g_stage2.proc_table[_pbi].nparams; break; }
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED); IR_t * fail = IR_node_alloc(g, IR_FAIL);
    IR_t * sentry = succ; IR_t * entry = succ;
    int is_multi = (pd && pd->n > 0 && pd->c[0] && pd->c[0]->v.sval) && strchr(pd->c[0]->v.sval, '$');
    int rk_np = 0; if (pd && !is_multi) for (int k = 1; k < pd->n && pd->c[k] && pd->c[k]->t == TT_VAR; k++) rk_np++;
    int rk_bstart = rk_np + 1, rk_bn = (pd && pd->n > rk_bstart) ? pd->n - rk_bstart : 0;
    const tree_t ** rk_plan = (rk_np > 0) ? (const tree_t **) calloc((size_t) (rk_bn + rk_np), sizeof(const tree_t *)) : NULL;
    if (rk_plan) {
        int nplan = 0, bi = 0;
        for (int k = 1; k <= rk_np; k++) {
            const tree_t * pv = pd->c[k]; const char * pn = (pv && pv->t == TT_VAR) ? pv->v.sval : NULL;
            const char * tgt = (bi < rk_bn) ? rk_prologue_target(pd->c[rk_bstart + bi]) : NULL;
            if (pn && tgt && !strcmp(tgt, pn)) rk_plan[nplan++] = pd->c[rk_bstart + bi++];
            if (pn && pv->n >= 1 && pv->c[0] && pv->c[0]->v.sval) {
                const char * ty = pv->c[0]->v.sval;
                if (strstr(ty, ":D") || strstr(ty, ":U") || rk_is_modeled_type(ty)) {
                    tree_t * mc = ast_node_new(TT_FNC); mc->v.sval = (char *)"__param_check";
                    tree_t * nmv = ast_node_new(TT_VAR); nmv->v.sval = (char *)"__param_check"; ast_push(mc, nmv);
                    tree_t * tyq = ast_node_new(TT_QLIT); tyq->v.sval = (char *)ty; ast_push(mc, tyq);
                    tree_t * pvr = ast_node_new(TT_VAR); pvr->v.sval = (char *)pn; ast_push(mc, pvr);
                    tree_t * pnq = ast_node_new(TT_QLIT); pnq->v.sval = (char *)pn; ast_push(mc, pnq);
                    rk_plan[nplan++] = mc;
                }
            }
        }
        for (; bi < rk_bn; bi++) rk_plan[nplan++] = pd->c[rk_bstart + bi];
        for (int i = nplan - 1; i >= 0; i--) {
            const tree_t * s = rk_plan[i]; if (!s) continue;
            if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
            if (s->t == TT_VAR) continue;
            IR_t * r = NULL; IR_t * e = lower_rv(&cx, s, sentry, fail, &r);
            if (e) { entry = e; sentry = e; }
        }
        free(rk_plan); g->entry = entry; return g;
    }
    for (int i = (pd ? pd->n : 0) - 1; i >= 1; i--) {
        const tree_t * s = pd->c[i];
        if (!s) continue;
        if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
        if (s->t == TT_VAR) continue;
        IR_t * r = NULL; IR_t * e = lower_rv(&cx, s, sentry, fail, &r);
        if (e) { entry = e; sentry = e; }
    }
    if (pd && !is_multi) {
        for (int k = pd->n - 1; k >= 1; k--) {
            const tree_t * pv = pd->c[k];
            if (!pv || pv->t != TT_VAR || !pv->v.sval) continue;
            if (pv->n < 1 || !pv->c[0] || !pv->c[0]->v.sval) continue;
            const char * ty = pv->c[0]->v.sval;
            if (!strstr(ty, ":D") && !strstr(ty, ":U") && !rk_is_modeled_type(ty)) continue;
            const char * pn = pv->v.sval;
            tree_t * mc = ast_node_new(TT_FNC); mc->v.sval = (char *) "__param_check";
            tree_t * nmv = ast_node_new(TT_VAR); nmv->v.sval = (char *) "__param_check"; ast_push(mc, nmv);
            tree_t * tyq = ast_node_new(TT_QLIT); tyq->v.sval = (char *) ty; ast_push(mc, tyq);
            tree_t * pvr = ast_node_new(TT_VAR); pvr->v.sval = (char *) pn; ast_push(mc, pvr);
            tree_t * pnq = ast_node_new(TT_QLIT); pnq->v.sval = (char *) pn; ast_push(mc, pnq);
            IR_t * r = NULL; IR_t * e = lower_rcall(&cx, mc, "__param_check", 1, sentry, fail, &r);
            if (e) { entry = e; sentry = e; }
        }
    }
    g->entry = entry; return g;
}
#include "stage2.h"
#include "bb_program.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_gram_class_members(const char * nm) {
    if (!nm) return NULL;
    if (!strcmp(nm, "digit"))  return "0123456789";
    if (!strcmp(nm, "alpha"))  return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (!strcmp(nm, "alnum"))  return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (!strcmp(nm, "upper"))  return "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (!strcmp(nm, "lower"))  return "abcdefghijklmnopqrstuvwxyz";
    if (!strcmp(nm, "space"))  return " \t\n\r";
    if (!strcmp(nm, "xdigit")) return "0123456789abcdefABCDEF";
    return NULL;
}
typedef struct { int is_lit; char s[256]; } rk_gleaf_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gram_seq_leaves(const char * body, rk_gleaf_t * out, int maxlv) {
    if (!body || !out || maxlv < 1) return 0;
    int n = (int) strlen(body); int i = 0; int nlv = 0;
    while (i < n) {
        while (i < n && (body[i]==' '||body[i]=='\t'||body[i]=='\n'||body[i]=='\r')) i++;
        if (i >= n) break;
        if (nlv >= maxlv) return 0;
        if (body[i] == '"' || body[i] == '\'') {
            char q = body[i++]; int op = 0;
            while (i < n && body[i] != q) { if (body[i] == '\\' || op >= (int) sizeof out[nlv].s - 1) return 0; out[nlv].s[op++] = body[i++]; }
            if (i >= n || op == 0) return 0;
            i++; out[nlv].s[op] = '\0'; out[nlv].is_lit = 1; nlv++;
        } else if (body[i] == '<') {
            i++; if (i < n && body[i] == '.') i++;
            int s = i; char nm[64]; int nl = 0;
            while (i < n && (isalnum((unsigned char)body[i]) || body[i] == '_') && nl < 63) nm[nl++] = body[i++];
            nm[nl] = '\0';
            if (i == s || i >= n || body[i] != '>') return 0;
            i++;
            const char * cs = rk_gram_class_members(nm);
            if (!cs || (int) strlen(cs) >= (int) sizeof out[nlv].s) return 0;
            strcpy(out[nlv].s, cs); out[nlv].is_lit = 0; nlv++;
        } else return 0;
    }
    return nlv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gram_split_alt(const char * body, char * lbuf, int lsz, char * rbuf, int rsz) {
    if (!body || !lbuf || !rbuf) return 0;
    int n = (int) strlen(body); int depth = 0;
    for (int i = 0; i < n; i++) {
        char c = body[i];
        if (c == '"' || c == '\'') { char q = c; i++; while (i < n && body[i] != q) i++; continue; }
        if (c == '<') { depth++; continue; }
        if (c == '>') { if (depth > 0) depth--; continue; }
        if (c == '|' && depth == 0) {
            int ll = i; while (ll > 0 && (body[ll-1]==' '||body[ll-1]=='\t')) ll--;
            if (ll <= 0 || ll >= lsz) return 0;
            memcpy(lbuf, body, (size_t)ll); lbuf[ll] = '\0';
            int rs = i + 1; while (rs < n && (body[rs]==' '||body[rs]=='\t')) rs++;
            int rl = n - rs; if (rl <= 0 || rl >= rsz) return 0;
            memcpy(rbuf, body + rs, (size_t)rl); rbuf[rl] = '\0';
            return 1;
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * rk_gram_build_leaf_chain(IR_graph_t * gg, rk_gleaf_t * lv, int nlv, IR_t * fail_tgt, int beta_tag) {
    IR_t * next = NULL;
    for (int e = nlv - 1; e >= 0; e--) {
        IR_t * nd = lc_build(gg, lv[e].is_lit ? IR_GLIT : IR_GCC, next, NULL);
        IR_LIT(nd).sval = lp_strdup(lv[e].s);
        if (beta_tag) lc_ω_to_β(nd, fail_tgt); else lc_ω_to(nd, fail_tgt);
        next = nd;
    }
    return next;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_lower_grammar_boxes(const tree_t * prog) {
    extern int g_opt_dump_bb; static int nat = -1; if (nat < 0) { const char *e = getenv("RK_GRAM_NATIVE"); nat = (e && e[0] == '0') ? 0 : 1; }
    if (!g_opt_dump_bb && !nat) return;
    if (!prog) return;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_GRAMMAR_DECL) continue;
        const char * gname = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!gname || !*gname) continue;
        for (int j = 1; j < d->n; j++) {
            const tree_t * rd = d->c[j];
            if (!rd || rd->t != TT_REGEX_DECL) continue;
            const char * rname = (rd->n > 0 && rd->c[0] && rd->c[0]->v.sval) ? rd->c[0]->v.sval : NULL;
            const char * body  = (rd->n > 1 && rd->c[1] && rd->c[1]->v.sval) ? rd->c[1]->v.sval : NULL;
            if (!rname || !body) continue;
            char pn[320]; snprintf(pn, sizeof pn, "gram__%s__%s", gname, rname);
            IR_graph_t * gg = IR_alloc(64);
            IR_t * entry = NULL;
            char lbody[512]; char rbody[512];
            if (rk_gram_split_alt(body, lbody, sizeof lbody, rbody, sizeof rbody)) {
                rk_gleaf_t lv1[32]; int nlv1 = rk_gram_seq_leaves(lbody, lv1, 32);
                rk_gleaf_t lv2[32]; int nlv2 = rk_gram_seq_leaves(rbody, lv2, 32);
                if (nlv1 <= 0 || nlv2 <= 0) continue;
                IR_t * galt = lc_build(gg, IR_GALT, NULL , NULL );
                IR_t * arm2_root = rk_gram_build_leaf_chain(gg, lv2, nlv2, NULL, 0);
                ir_operand_push(galt, arm2_root);
                IR_t * arm1_root = rk_gram_build_leaf_chain(gg, lv1, nlv1, galt, 1 );
                ir_operand_push(galt, arm1_root);
                entry = galt;
            } else {
                rk_gleaf_t lv[64]; int nlv = rk_gram_seq_leaves(body, lv, 64);
                if (nlv <= 0) continue;
                IR_t * next = NULL;
                for (int e = nlv - 1; e >= 0; e--) {
                    IR_t * nd = lc_build(gg, lv[e].is_lit ? IR_GLIT : IR_GCC, next, NULL);
                    IR_LIT(nd).sval = lp_strdup(lv[e].s);
                    next = nd;
                }
                entry = next;
            }
            gg->entry = entry;
            int gidx = bb_program_add(&g_stage2.bbp, gg);
            int pidx = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[pidx].name = lp_strdup(pn); g_stage2.proc_table[pidx].proc = (tree_t *) rd;
            g_stage2.proc_table[pidx].entry_pc = -1; g_stage2.proc_table[pidx].bb_idx = gidx; g_stage2.proc_table[pidx].nparams = 0;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_raku_body(const tree_t *prog, const tree_t *proc) {
    IR_graph_t * ng = lower_raku_proc(prog, proc);
    if (!ng || !ng->entry) return -1;
    return bb_program_add(&g_stage2.bbp, ng);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void raku_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    extern void record_register(const char * spec);
    int mod_idx = -1;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        tree_t * proc = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!proc) continue;
        if (proc->t == TT_GLOBAL) {
            for (int _gi = 0; _gi < proc->n; _gi++)
                if (proc->c[_gi] && proc->c[_gi]->v.sval)
                    global_register(proc->c[_gi]->v.sval);
        }
        if (proc->t == TT_RECORD && proc->v.sval && *proc->v.sval) {
            char spec[256]; int pos = 0;
            pos += snprintf(spec+pos, sizeof(spec)-pos, "%s(", proc->v.sval);
            for (int _ri = 0; _ri < proc->n && pos < (int)sizeof(spec)-2; _ri++) {
                if (_ri > 0) spec[pos++] = ',';
                const char *fn2 = (proc->c[_ri] && proc->c[_ri]->v.sval) ? proc->c[_ri]->v.sval : "";
                pos += snprintf(spec+pos, sizeof(spec)-pos, "%s", fn2);
            }
            if (pos < (int)sizeof(spec)-1) spec[pos++] = ')';
            spec[pos] = '\0';
            record_register(spec);
        }
        if (proc->t == TT_PROC_DECL) {
            const char *name = NULL;
            if (proc->t == TT_SUB_DECL) {
                if (proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) name = proc->c[0]->v.sval;
            } else {
                name = (proc->v.sval && *proc->v.sval) ? proc->v.sval : ((proc->n > 0 && proc->c[0] && proc->c[0]->t == TT_VAR && proc->c[0]->v.sval && *proc->c[0]->v.sval) ? proc->c[0]->v.sval : NULL);
            }
            if (name) {
                int _pi = stage2_proc_grow(s2);
                s2->proc_table[_pi].name     = name;
                s2->proc_table[_pi].proc     = proc;
                s2->proc_table[_pi].entry_pc = -1;
                s2->proc_table[_pi].bb_idx   = -1;
                s2->proc_table[_pi].nparams  = (int)proc->v.ival;
                s2->proc_table[_pi].byref_mask = 0;
                if (mod_idx >= 0) s2->module_registry.mods[mod_idx].nprocs++;
                if (strcmp(name, "main") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_collect_blocks(const tree_t * t, tree_t ** out, int * n, int max) {
    if (!t || *n >= max) return;
    if (t->t == TT_ANON_BLOCK && !t->v.sval) out[(*n)++] = (tree_t *) t;
    for (int i = 0; i < t->n; i++) rk_collect_blocks(t->c[i], out, n, max);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_hoist_anon_blocks(tree_t * prog) {
    if (!prog) return;
    static tree_t * blks[512]; int nb = 0; rk_collect_blocks(prog, blks, &nb, 512);
    static int g_blk_ctr = 0;
    for (int i = 0; i < nb; i++) {
        tree_t * blk = blks[i]; char nm[64]; snprintf(nm, sizeof nm, "__blk_%d", ++g_blk_ctr);
        char * pn = lp_strdup(nm); blk->v.sval = pn;
        tree_t * sd = ast_node_new(TT_SUB_DECL); sd->v.ival = 0;
        tree_t * nn = ast_node_new(TT_VAR); nn->v.sval = pn; ast_push(sd, nn);
        const tree_t * body = (blk->n > 0) ? blk->c[0] : NULL;
        if (body) { for (int k = 0; k < body->n; k++) ast_push(sd, body->c[k]); ((tree_t *) body)->n = 0; }
        blk->n = 0;
        ast_push(prog, sd);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_raku_stage2(const tree_t *prog) {
    rk_hoist_anon_blocks((tree_t *) prog);
    raku_register_program(&g_stage2, prog);
    rk_discover_grammars(prog);
    rk_lower_grammar_boxes(prog);
    rk_register_classes(prog);
    g_rk_multi_n = 0;
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (!d || d->t != TT_SUB_DECL) continue;
        const char * nm = (d->n > 0 && d->c[0] && d->c[0]->v.sval) ? d->c[0]->v.sval : NULL;
        if (!nm) continue;
        const char * soh = strchr(nm, '$'); if (!soh) continue;
        char base[128]; int bl = (int)(soh - nm); if (bl > 127) bl = 127; memcpy(base, nm, bl); base[bl] = 0;
        rk_multi_name_add(base);
    }
    rk_discover_procs(prog);
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_SUB_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_raku_body(prog, proc);
        if (bb_idx >= 0) {
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            const char * pname = g_stage2.proc_table[pi].name;
            int is_method = (pname && strchr(pname, '_') && strchr(pname, '_')[1] == '_');
            int param_start = is_method ? 0 : 1;
            int np = g_stage2.proc_table[pi].nparams;
            Scope *sc = &g_stage2.proc_table[pi].lower_sc;
            sc->n = 0;
            if (is_method) {
                sc->e[sc->n].name = lp_strdup("self");
                sc->e[sc->n].slot = sc->n; sc->n++;
                param_start = 1;
            }
            for (int k = 0; k < np && (k + param_start) < proc->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                const tree_t *pv = proc->c[k + param_start];
                if (!pv || !pv->v.sval) continue;
                sc->e[sc->n].name = lp_strdup(pv->v.sval);
                sc->e[sc->n].slot = sc->n; sc->n++;
            }
            g_stage2.bbp.table[bb_idx]->nparams = sc->n;
            if (sc->n > 0) {
                const char ** _pn = (const char **) calloc((size_t) sc->n, sizeof(const char *));
                if (_pn) { for (int k = 0; k < sc->n; k++) _pn[k] = sc->e[k].name; g_stage2.bbp.table[bb_idx]->pnames = _pn; }
            }
        }
    }
    int has_main = 0;
    for (int pi = 0; pi < g_stage2.proc_count; pi++)
        if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, "main") == 0) { has_main = 1; break; }
    if (!has_main) {
        IR_graph_t * tg = IR_alloc(8192); rcx_t tcx; tcx.g = tg; tcx.try_catch = NULL; tcx.loop_exit = NULL; tcx.loop_next = NULL;
        IR_t * succ = IR_node_alloc(tg, IR_SUCCEED); IR_t * fail = IR_node_alloc(tg, IR_FAIL);
        IR_t * sentry = succ; IR_t * entry = succ;
        int has_rk_MAIN = 0;
        for (int pi = 0; pi < g_stage2.proc_count; pi++)
            if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, "MAIN") == 0) { has_rk_MAIN = 1; break; }
        if (has_rk_MAIN) {
            tree_t * mc = ast_node_new(TT_FNC); mc->v.sval = (char *)"MAIN";
            tree_t * nmv = ast_node_new(TT_VAR); nmv->v.sval = (char *)"MAIN"; ast_push(mc, nmv);
            IR_t * r = NULL; IR_t * e = lower_rv(&tcx, mc, sentry, sentry, &r);
            if (e) { entry = e; sentry = e; }
        }
        for (int i = prog->n - 1; i >= 0; i--) {
            const tree_t * s = prog->c[i];
            if (!s) continue;
            if (s->t == TT_STMT) { const tree_t * sub = stmt_subj(s); if (!sub) continue; s = sub; }
            if (s->t == TT_SUB_DECL || s->t == TT_CLASS_DECL || s->t == TT_ROLE_DECL || s->t == TT_GRAMMAR_DECL) continue;
            if (s->t == TT_MODULE_DECL) {
                for (int j = s->n - 1; j >= 1; j--) {
                    const tree_t * ch = s->c[j];
                    if (ch && ch->t == TT_STMT) { const tree_t * sub = stmt_subj(ch); if (!sub) continue; ch = sub; }
                    if (!ch || ch->t == TT_SUB_DECL) continue;
                    IR_t * r = NULL; IR_t * e = lower_rv(&tcx, ch, sentry, sentry, &r);
                    if (e) { entry = e; sentry = e; }
                }
                continue;
            }
            if ((s->t == TT_SEQ || s->t == TT_SEQ_EXPR || s->t == TT_PROGRAM) && s->n == 0) continue;
            IR_t * r = NULL; IR_t * e = lower_rv(&tcx, s, sentry, sentry, &r);
            if (e) { entry = e; sentry = e; }
        }
        tg->entry = entry;
        int bb_idx = bb_program_add(&g_stage2.bbp, tg);
        if (bb_idx >= 0) {
            int pi = stage2_proc_grow(&g_stage2);
            g_stage2.proc_table[pi].name     = lp_strdup("main");
            g_stage2.proc_table[pi].proc     = NULL;
            g_stage2.proc_table[pi].entry_pc = -1;
            g_stage2.proc_table[pi].bb_idx   = bb_idx;
            g_stage2.proc_table[pi].nparams  = 0;
        }
    }
    rk_reclassify_calls();
    { static int _zf = -1; if (_zf < 0) { const char *_e = getenv("SCRIP_RK_ZFRAME"); _zf = (_e && *_e == '0') ? 0 : 1; }
      if (_zf) for (int _gi = 0; _gi < g_stage2.bbp.count; _gi++) if (g_stage2.bbp.table[_gi]) g_stage2.bbp.table[_gi]->zframe_graph = 1; }
    return &g_stage2;
}
