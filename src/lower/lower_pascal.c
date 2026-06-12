#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "IR.h"
/*====================================================================================================================================================================================================*/
#define PAS_MAX_SCOPE 64
typedef struct pas_scope_s {
    const char *        names[PAS_MAX_SCOPE];
    int                 n;
    int                 nparams;  /* number of param slots (0..nparams-1 are params) */
    long long           byref;
    int                 has_children;
    struct pas_scope_s * outer;
} pas_scope_t;
typedef struct {
    IR_graph_t * g;
    pas_scope_t  sc;
    IR_t       * labels[128];
    const char * lnames[128];
    int          nlabels;
    int          npbt;
} pcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int bb_operand_aux_set(IR_graph_t * bbg, IR_t * bb, IR_t * const * src, int n);
static IR_t * lower(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
static IR_graph_t * pas_arg_block(pcx_t * cx, const tree_t * a) {
    IR_graph_t * saved = cx->g;
    IR_graph_t * g2 = IR_alloc(256, IR_LANG_PAS); cx->g = g2;
    IR_t * F = IR_node_alloc(g2, IR_FAIL);
    IR_t * e = lower(cx, a, NULL, F);
    g2->entry = e;
    cx->g = saved;
    return g2;
}
static void pas_call_blocks(pcx_t * cx, IR_t * call, double dv, const tree_t * const * args, int nargs) {
    IR_LIT(call).dval = dv;
    if (nargs <= 0) return;
    IR_graph_t ** blks = (IR_graph_t **) calloc((size_t) nargs, sizeof(IR_graph_t *));
    if (!blks) return;
    for (int k = 0; k < nargs; k++) blks[k] = pas_arg_block(cx, args[k]);
    IR_EXEC(call).counter = (int64_t)(intptr_t) blks;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void γ_to(IR_t * nd, IR_t * t) { lc_γ_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ω_to(IR_t * nd, IR_t * t) { lc_ω_to(nd, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(pcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int binop_code(tree_e tt) {
    switch (tt) {
    case TT_ADD: return 0; case TT_SUB: return 1; case TT_MUL: return 2; case TT_DIV: return 3; case TT_MOD: return 4;
    case TT_LT:  return 5; case TT_LE:  return 6; case TT_GT:  return 7; case TT_GE:  return 8;
    case TT_EQ:  return 9; case TT_NE:  return 10; case TT_POW: return 18; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scope_slot(const pas_scope_t * sc, const char * name) {
    if (!name) return -1;
    for (int i = 0; i < sc->n; i++) if (sc->names[i] && !strcmp(sc->names[i], name)) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scope_slot_chain(const pas_scope_t * sc, const char * name, long long * byref_out) {
    if (!name) return -1;
    for (const pas_scope_t * s = sc; s; s = s->outer) {
        int slot = scope_slot(s, name);
        if (slot >= 0) { if (byref_out) *byref_out = s->byref; return slot; }
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * label_find(pcx_t * cx, const char * name) {
    if (!name) return NULL;
    for (int i = 0; i < cx->nlabels; i++) if (cx->lnames[i] && !strcmp(cx->lnames[i], name)) return cx->labels[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_var(pcx_t * cx, const char * name, IR_t * γ, IR_t * ω) {
    long long byref = 0;
    const pas_scope_t * found_sc = NULL;
    int slot = -1;
    for (const pas_scope_t * s = &cx->sc; s; s = s->outer) {
        int sl = scope_slot(s, name);
        if (sl >= 0) { slot = sl; byref = s->byref; found_sc = s; break; }
    }
    if (slot < 0) {
        IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = name; return nd;
    }
    int isref     = (int)((byref >> slot) & 1LL);
    int is_own    = (found_sc == &cx->sc);
    int use_frame = isref || !is_own
                    || (cx->sc.outer != NULL || cx->sc.byref != 0 || cx->sc.has_children);
    if (isref) {
        IR_t * nd = build(cx, IR_VAR_FRAME_REF, γ, ω);
        IR_LIT(nd).sval = name; IR_LIT(nd).ival = slot; return nd;
    }
    if (!use_frame) {
        IR_t * nd = build(cx, IR_VAR, γ, ω); IR_LIT(nd).sval = name; return nd;
    }
    IR_t * nd = build(cx, IR_VAR_FRAME, γ, ω);
    IR_LIT(nd).sval = name; IR_LIT(nd).ival = slot; return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_assign_var(pcx_t * cx, const char * name, IR_t * γ, IR_t * ω) {
    long long byref = 0;
    const pas_scope_t * found_sc = NULL;
    int slot = -1;
    for (const pas_scope_t * s = &cx->sc; s; s = s->outer) {
        int sl = scope_slot(s, name);
        if (sl >= 0) { slot = sl; byref = s->byref; found_sc = s; break; }
    }
    if (slot < 0) {
        IR_t * nd = build(cx, IR_ASSIGN, γ, ω); IR_LIT(nd).sval = name; return nd;
    }
    int isref     = (int)((byref >> slot) & 1LL);
    int is_own    = (found_sc == &cx->sc);
    int use_frame = isref || !is_own
                    || (cx->sc.outer != NULL || cx->sc.byref != 0 || cx->sc.has_children);
    if (isref) {
        IR_t * nd = build(cx, IR_ASSIGN_FRAME_REF, γ, ω);
        IR_LIT(nd).sval = name; IR_LIT(nd).ival = slot; return nd;
    }
    if (!use_frame) {
        IR_t * nd = build(cx, IR_ASSIGN, γ, ω); IR_LIT(nd).sval = name; return nd;
    }
    IR_t * nd = build(cx, IR_ASSIGN_FRAME, γ, ω);
    IR_LIT(nd).sval = name; IR_LIT(nd).ival = slot; return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_relop(tree_e tt) {
    switch (tt) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pas_mat(pcx_t * cx, const tree_t * e, IR_t * ω, IR_t ** v_out, IR_t ** at_out, IR_t ** af_out) {
    char * nm = (char *) malloc(16);
    snprintf(nm, 16, "__pbt%d", cx->npbt++);
    IR_t * n1 = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(n1).ival = 1;
    IR_t * n0 = build(cx, IR_LIT_I, NULL, NULL); IR_LIT(n0).ival = 0;
    IR_t * at = build(cx, IR_ASSIGN, NULL, ω); IR_LIT(at).sval = nm;
    IR_t * af = build(cx, IR_ASSIGN, NULL, ω); IR_LIT(af).sval = nm;
    γ_to(n1, at); γ_to(n0, af);
    IR_t * v = build(cx, IR_VAR, NULL, NULL); IR_LIT(v).sval = nm;
    IR_t * eE = lower(cx, e, n1, n0);
    *v_out = v; *at_out = at; *af_out = af;
    return eE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_binop(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * lt = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * rt = (t->n > 1) ? t->c[1] : NULL;
    int lm = lt && is_relop(lt->t) && !is_relop(t->t);
    int rm = rt && is_relop(rt->t) && !is_relop(t->t);
    IR_t * op = build(cx, IR_BINOP, γ, ω);
    IR_LIT(op).ival = binop_code(t->t);
    if (!lm && !rm) {
        int lmark = cx->g->n;
        IR_t * le = lower(cx, lt, NULL, ω);
        int rmark = cx->g->n;
        IR_t * re = lower(cx, rt, op,  ω);
        IR_t * lres = (cx->g->n > lmark) ? cx->g->all[lmark] : le;
        IR_t * rres = (cx->g->n > rmark) ? cx->g->all[rmark] : re;
        γ_to(lres, re);
        { IR_t * ax[2]; ax[0] = lres; ax[1] = rres; bb_operand_aux_set(cx->g, op, ax, 2); }
        return le;
    }
    IR_t * vL = NULL, * atL = NULL, * afL = NULL, * vR = NULL, * atR = NULL, * afR = NULL;
    IR_t * eL = NULL, * eR = NULL, * lres = NULL, * rres = NULL, * entry = NULL;
    if (lm) eL = pas_mat(cx, lt, ω, &vL, &atL, &afL);
    if (rm) eR = pas_mat(cx, rt, ω, &vR, &atR, &afR);
    IR_t * pL = NULL, * pR = NULL;
    if (!lm) { int m = cx->g->n; pL = lower(cx, lt, NULL, ω); lres = (cx->g->n > m) ? cx->g->all[m] : pL; }
    if (!rm) { int m = cx->g->n; pR = lower(cx, rt, op, ω); rres = (cx->g->n > m) ? cx->g->all[m] : pR; }
    if (lm) lres = vL;
    if (rm) rres = vR;
    if (lm && rm)      { entry = eL; γ_to(atL, eR);    γ_to(afL, eR);    γ_to(atR, vL); γ_to(afR, vL); γ_to(vL, vR); γ_to(vR, op); }
    else if (lm)       { entry = eL; γ_to(atL, vL);    γ_to(afL, vL);    γ_to(vL, pR); }
    else               { entry = eR; γ_to(atR, pL);    γ_to(afR, pL);    γ_to(lres, vR); γ_to(vR, op); }
    { IR_t * ax[2]; ax[0] = lres; ax[1] = rres; bb_operand_aux_set(cx->g, op, ax, 2); }
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_unop(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    IR_t * op = build(cx, IR_UNOP, γ, ω);
    IR_LIT(op).ival = (long long) t->t;
    IR_t * child = lower(cx, (t->n > 0) ? t->c[0] : NULL, op, ω);
    return child ? child : op;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_call(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * c0 = (t->n > 0) ? t->c[0] : NULL;
    IR_t * nd = build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = (c0 && c0->v.sval) ? c0->v.sval : NULL;
    IR_LIT(nd).ival = (t->n > 0) ? t->n - 1 : 0;
    pas_call_blocks(cx, nd, 3.0, (const tree_t * const *) (t->n > 1 ? &t->c[1] : NULL), (t->n > 0) ? t->n - 1 : 0);
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_assign(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * lhs = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * rhs = (t->n > 1) ? t->c[1] : NULL;
    const char * vname = NULL;
    if (lhs && lhs->t == TT_IDX) {
        const tree_t * base = (lhs->n > 0) ? lhs->c[0] : NULL;
        const char * bname = (base && base->t == TT_VAR) ? base->v.sval : NULL;
        IR_t * asn = lower_assign_var(cx, bname, γ, ω);
        IR_t * call = build(cx, IR_CALL, asn, ω); IR_LIT(call).sval = "arr_set_pure"; IR_LIT(call).ival = lhs->n + 1;
        { const tree_t * av[34]; int an = 0; for (int k = 0; k < lhs->n && an < 33; k++) av[an++] = lhs->c[k]; av[an++] = rhs; pas_call_blocks(cx, call, 2.0, av, an); }
        return call;
    }
    if (lhs && lhs->t == TT_FNC && lhs->n > 0 && lhs->c[0] && lhs->c[0]->v.sval && !strcmp(lhs->c[0]->v.sval, "__pas_deref")) {
        IR_t * call = build(cx, IR_CALL, γ, ω); IR_LIT(call).sval = "__pas_deref_set"; IR_LIT(call).ival = 2;
        { const tree_t * av[2]; av[0] = (lhs->n > 1) ? lhs->c[1] : NULL; av[1] = rhs; pas_call_blocks(cx, call, 2.0, av, 2); }
        return call;
    }
    if (lhs && lhs->t == TT_VAR) vname = lhs->v.sval;
    else if (lhs && lhs->t == TT_FNC && lhs->n > 0 && lhs->c[0]) vname = lhs->c[0]->v.sval;
    if (rhs && is_relop(rhs->t)) {
        IR_t * nd = build(cx, IR_IF, γ, ω);
        IR_t * th = lower_assign_var(cx, vname, γ, ω);
        IR_t * tlit = build(cx, IR_LIT_I, th, ω); IR_LIT(tlit).ival = 1; γ_to(th, tlit);
        IR_t * el = lower_assign_var(cx, vname, γ, ω);
        IR_t * elit = build(cx, IR_LIT_I, el, ω); IR_LIT(elit).ival = 0; γ_to(el, elit);
        IR_t * cond_entry = lower(cx, rhs, tlit, el);
        if (cond_entry) ir_operand_push(nd, cond_entry);
        return nd;
    }
    IR_t * nd = lower_assign_var(cx, vname, γ, ω);
    IR_t * rentry = lower(cx, rhs, nd, ω);
    return rentry ? rentry : nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_if(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * cond   = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * then_t = (t->n > 1) ? t->c[1] : NULL;
    const tree_t * els_t  = (t->n > 2) ? t->c[2] : NULL;
    IR_t * nd = build(cx, IR_IF, γ, ω);
    IR_t * then_entry = lower(cx, then_t, γ, ω);
    IR_t * else_entry = els_t ? lower(cx, els_t, γ, ω) : build(cx, IR_SUCCEED, γ, ω);
    IR_t * te = then_entry ? then_entry : γ;
    IR_t * ee = else_entry ? else_entry : ω;
    IR_t * cond_entry;
    if (cond && is_relop(cond->t)) {
        cond_entry = lower(cx, cond, te, ee);
    } else {
        IR_t * ne = build(cx, IR_BINOP, te, ee); IR_LIT(ne).ival = 10;
        IR_t * expr = lower(cx, cond, ne, ee);
        IR_t * lit0 = build(cx, IR_LIT_I, ne, ee); IR_LIT(lit0).ival = 0;
        γ_to(expr, lit0);
        cond_entry = expr;
    }
    if (cond_entry) ir_operand_push(nd, cond_entry);
    return cond_entry ? cond_entry : nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_while(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * cond = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * body = (t->n > 1) ? t->c[1] : NULL;
    /* WHILE node: γ=after-loop, ω=outer-fail */
    IR_t * wnd = build(cx, IR_WHILE, γ, ω);
    /* Unwrap TT_NOT: oracle strips NOT and uses NE(child, 0) */
    const tree_t * cond_inner = (cond && cond->t == TT_NOT && cond->n > 0) ? cond->c[0] : cond;
    /* Condition nodes allocated FIRST (before CONJ and body) per oracle order.
       body_entry wired after body is lowered. */
    IR_t * cond_entry;
    IR_t * cond_res = NULL;
    IR_t * ne = NULL;
    if (cond_inner && is_relop(cond_inner->t)) {
        /* Relop condition: allocate binop first (body_entry wired later) */
        int cmark = cx->g->n;
        cond_entry = lower(cx, cond_inner, NULL, wnd);
        cond_res = (cx->g->n > cmark) ? cx->g->all[cmark] : cond_entry;
    } else {
        /* NE-wrap: BINOP first, then expr, then LIT */
        ne = build(cx, IR_BINOP, NULL, wnd); IR_LIT(ne).ival = 10;
        IR_t * expr = lower(cx, cond_inner, ne, wnd);
        IR_t * lit0 = build(cx, IR_LIT_I, ne, wnd); IR_LIT(lit0).ival = 0;
        γ_to(expr, lit0);
        cond_entry = expr;
    }
    /* Body loops back to the condition entry (γ=ω=cond_entry); a multi-statement
       begin..end body supplies the single CONJ join via lower_seq, mirroring lower_for. */
    IR_t * body_entry = lower(cx, body, cond_entry, cond_entry);
    if (!body_entry) body_entry = cond_entry;
    /* Wire condition success to body */
    if (cond_inner && is_relop(cond_inner->t)) {
        /* For relop, the binop result node needs γ=body_entry */
        γ_to(cond_res, body_entry);
    } else {
        γ_to(ne, body_entry);
    }
    ir_operand_push(wnd, cond_entry);
    return cond_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_for(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * var  = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * from = (t->n > 1) ? t->c[1] : NULL;
    const tree_t * to   = (t->n > 2) ? t->c[2] : NULL;
    const tree_t * body = (t->n > 3) ? t->c[3] : NULL;
    const char * vname  = (var && var->t == TT_VAR) ? var->v.sval : NULL;
    int is_downto = (t->v.ival == 1);
    int cmp_op  = is_downto ? 8 : 6;
    int inc_op  = is_downto ? 1 : 0;
    /* 1. Limit BINOP: LE for to, GE for downto; ω=γ is the loop-exit continuation */
    IR_t * lim_cmp  = build(cx, IR_BINOP, NULL, γ); IR_LIT(lim_cmp).ival = cmp_op;
    /* 2. VAR(i) read that feeds the limit BINOP on every iteration */
    IR_t * lim_var  = lower_var(cx, vname, lim_cmp, γ);
    /* 3. to expression evaluated ONCE before the loop; lim_cmp reads its cached value via aux so a complex limit is not re-walked per iteration */
    int to_mark = cx->g->n;
    IR_t * to_entry = lower(cx, to, lim_var, ω);
    IR_t * to_res   = (cx->g->n > to_mark) ? cx->g->all[to_mark] : to_entry;
    if (!to_entry) { to_entry = lim_var; to_res = lim_var; }
    { IR_t * ax[2]; ax[0] = lim_var; ax[1] = to_res; bb_operand_aux_set(cx->g, lim_cmp, ax, 2); }
    /* 4. Increment/decrement loops back to lim_var, not to_entry, so to is evaluated once */
    IR_t * inc_assign = lower_assign_var(cx, vname, lim_var, ω);
    IR_t * inc_var    = lower_var(cx, vname, NULL, ω);
    IR_t * inc_lit1   = build(cx, IR_LIT_I, NULL, ω); IR_LIT(inc_lit1).ival = 1;
    IR_t * inc_binop  = build(cx, IR_BINOP, inc_assign, ω); IR_LIT(inc_binop).ival = inc_op;
    γ_to(inc_var, inc_lit1); γ_to(inc_lit1, inc_binop);
    /* 5. Body: γ=ω=inc_var */
    IR_t * body_entry = lower(cx, body, inc_var, inc_var);
    if (!body_entry) body_entry = inc_var;
    γ_to(lim_cmp, body_entry);
    /* 6. Init: i := from → to_entry (compute limit once) → lim_var → lim_cmp */
    IR_t * init_assign = lower_assign_var(cx, vname, to_entry, ω);
    IR_t * from_entry  = lower(cx, from, init_assign, ω);
    return from_entry ? from_entry : init_assign;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_repeat(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * body = (t->n > 0) ? t->c[0] : NULL;
    const tree_t * cond = (t->n > 1) ? t->c[1] : NULL;
    IR_t * cond_entry; IR_t * cond_res = NULL;
    int cmark = cx->g->n;
    if (cond && is_relop(cond->t)) {
        cond_entry = lower(cx, cond, γ, NULL);
        cond_res = (cx->g->n > cmark) ? cx->g->all[cmark] : cond_entry;
    } else {
        IR_t * ne = build(cx, IR_BINOP, γ, NULL); IR_LIT(ne).ival = 10;
        IR_t * expr = lower(cx, cond, ne, NULL);
        IR_t * lit0 = build(cx, IR_LIT_I, ne, NULL); IR_LIT(lit0).ival = 0;
        γ_to(expr, lit0); cond_entry = expr; cond_res = ne;
    }
    IR_t * body_entry = lower(cx, body, cond_entry, ω);
    if (!body_entry) body_entry = cond_entry;
    ω_to(cond_res ? cond_res : cond_entry, body_entry);
    return body_entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_resumable(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_STMT) { for (int j = 0; j < t->n; j++) { const tree_t * a = t->c[j]; if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) { t = (a->n > 0) ? a->c[0] : NULL; break; } } }
    if (!t) return 0;
    switch (t->t) { case TT_IF: case TT_UNLESS: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void seq_flatten(const tree_t * t, const tree_t ** out, int * k) {
    for (int i = 0; i < t->n && *k < 512; i++) {
        const tree_t * s = t->c[i];
        if (s && s->t == TT_SEQ_EXPR) seq_flatten(s, out, k);
        else if (s) out[(*k)++] = s;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_seq(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    IR_t * conj = build(cx, IR_CONJ, γ, ω);
    const tree_t * st[512]; int k = 0;
    seq_flatten(t, st, &k);
    if (k == 0) return conj;
    IR_t * succ = conj; IR_t * entry = NULL;
    IR_t * anchor[512];
    for (int i = k - 1; i >= 0; i--) {
        int m = cx->g->n;
        IR_t * e = lower(cx, st[i], succ, ω);
        anchor[i] = (cx->g->n > m) ? cx->g->all[m] : e;
        if (e) { entry = e; succ = e; }
    }
    int lr = -1;
    for (int i = 0; i < k; i++) { if (lr >= 0 && anchor[i]) ω_to(anchor[i], anchor[lr]); if (pas_resumable(st[i])) lr = i; }
    return entry ? entry : conj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_block(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    const tree_t * stmts[512]; int k = 0;
    for (int i = 0; i < t->n && k < 512; i++) {
        const tree_t * s = t->c[i];
        if (!s) continue;
        if (s->t == TT_PROC_DECL) continue;
        if (s->t == TT_STMT) {
            const tree_t * sub = NULL;
            for (int j = 0; j < s->n; j++) {
                const tree_t * a = s->c[j];
                if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj")) { sub = (a->n > 0) ? a->c[0] : NULL; break; }
            }
            if (!sub) continue;
            s = sub;
        }
        stmts[k++] = s;
    }
    if (k == 0) return build(cx, IR_SUCCEED, γ, ω);
    IR_t * succ = γ; IR_t * entry = NULL;
    for (int i = k - 1; i >= 0; i--) { entry = lower(cx, stmts[i], succ, ω); if (entry) succ = entry; }
    return entry ? entry : build(cx, IR_SUCCEED, γ, ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scan_labels(pcx_t * cx, const tree_t * t, IR_t * fail) {
    if (!t) return;
    if (t->t == TT_LABEL_DEF && t->v.sval && cx->nlabels < 128) {
        IR_t * lnd = IR_node_alloc(cx->g, IR_SUCCEED);
        ω_to(lnd, fail);
        cx->lnames[cx->nlabels] = t->v.sval;
        cx->labels[cx->nlabels] = lnd;
        cx->nlabels++;
    }
    if (t->t == TT_STMT) {
        for (int i = 0; i < t->n; i++) {
            const tree_t * a = t->c[i];
            if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":lbl")) {
                for (int j = 0; j < a->n && cx->nlabels < 128; j++) {
                    const tree_t * ln = a->c[j];
                    if (ln && ln->v.sval) {
                        IR_t * lnd = IR_node_alloc(cx->g, IR_SUCCEED);
                        ω_to(lnd, fail);
                        cx->lnames[cx->nlabels] = ln->v.sval;
                        cx->labels[cx->nlabels] = lnd;
                        cx->nlabels++;
                    }
                }
            }
        }
    }
    for (int i = 0; i < t->n; i++) scan_labels(cx, t->c[i], fail);
}
/*====================================================================================================================================================================================================*/
static IR_t * lower(pcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω) {
    if (!t) return build(cx, IR_SUCCEED, γ, ω);
    switch (t->t) {
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_I, γ, ω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_F, γ, ω); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_S, γ, ω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_VAR:  return lower_var(cx, t->v.sval, γ, ω);
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW:
    case TT_LT:  case TT_LE:  case TT_GT:  case TT_GE:  case TT_EQ:  case TT_NE:
        return lower_binop(cx, t, γ, ω);
    case TT_MNS: case TT_PLS: case TT_NOT: case TT_SIZE:
        return lower_unop(cx, t, γ, ω);
    case TT_ASSIGN: return lower_assign(cx, t, γ, ω);
    case TT_IDX: { IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "arr_get"; IR_LIT(nd).ival = t->n; pas_call_blocks(cx, nd, 2.0, (const tree_t * const *) t->c, t->n); return nd; }
    case TT_FNC:    return lower_call(cx, t, γ, ω);
    case TT_IF: case TT_UNLESS: return lower_if(cx, t, γ, ω);
    case TT_WHILE:  return lower_while(cx, t, γ, ω);
    case TT_FOR:    return lower_for(cx, t, γ, ω);
    case TT_REPEAT: return lower_repeat(cx, t, γ, ω);
    case TT_SEQ: case TT_SEQ_EXPR: return lower_seq(cx, t, γ, ω);
    case TT_PROGRAM: return lower_block(cx, t, γ, ω);
    case TT_STMT: {
        const tree_t * sub = NULL;
        IR_t * goto_s = NULL; IR_t * goto_f = NULL; IR_t * goto_u = NULL;
        const char * lbl = NULL;
        for (int i = 0; i < t->n; i++) {
            const tree_t * a = t->c[i];
            if (!a || a->t != TT_ATTR) continue;
            if (!a->v.sval) continue;
            if (!strcmp(a->v.sval, ":subj")) sub = (a->n > 0) ? a->c[0] : NULL;
            else if (!strcmp(a->v.sval, ":lbl") && a->n > 0 && a->c[0]) lbl = a->c[0]->v.sval;
            else if (!strcmp(a->v.sval, ":goS") && a->n > 0 && a->c[0]) { IR_t * tgt = label_find(cx, a->c[0]->v.sval); goto_s = tgt; }
            else if (!strcmp(a->v.sval, ":goF") && a->n > 0 && a->c[0]) { IR_t * tgt = label_find(cx, a->c[0]->v.sval); goto_f = tgt; }
            else if (!strcmp(a->v.sval, ":go")  && a->n > 0 && a->c[0]) { IR_t * tgt = label_find(cx, a->c[0]->v.sval); goto_u = tgt; }
        }
        IR_t * lbl_node = lbl ? label_find(cx, lbl) : NULL;
        if (goto_u && !sub) { if (lbl_node) γ_to(lbl_node, goto_u); return lbl_node ? lbl_node : goto_u; }
        IR_t * s_γ = goto_s ? goto_s : γ;
        IR_t * f_ω = goto_f ? goto_f : ω;
        IR_t * result = sub ? lower(cx, sub, s_γ, f_ω) : build(cx, IR_SUCCEED, s_γ, f_ω);
        if (lbl_node) γ_to(lbl_node, result ? result : s_γ);
        return result;
    }
    case TT_LABEL_DEF: {
        IR_t * lnd = label_find(cx, t->v.sval);
        const tree_t * child = (t->n > 0) ? t->c[0] : NULL;
        IR_t * entry = child ? lower(cx, child, γ, ω) : NULL;
        if (lnd) { γ_to(lnd, entry ? entry : γ); return lnd; }
        return entry ? entry : build(cx, IR_SUCCEED, γ, ω);
    }
    case TT_GOTO_U: {
        const char * name = t->v.sval ? t->v.sval : (t->n > 0 && t->c[0] ? t->c[0]->v.sval : NULL);
        IR_t * tgt = label_find(cx, name);
        if (tgt) { IR_t * jmp = build(cx, IR_SUCCEED, tgt, ω); return jmp; }
        return build(cx, IR_SUCCEED, γ, ω);
    }
    default: return build(cx, IR_SUCCEED, γ, ω);
    }
}
/*====================================================================================================================================================================================================*/
#define PAS_MAX_PROCS 256
static const tree_t * g_pas_proc_list[PAS_MAX_PROCS];
static const tree_t * g_pas_proc_parent[PAS_MAX_PROCS];
static int             g_pas_nprocs;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void build_scope(pas_scope_t * sc, const tree_t * pd, pas_scope_t * outer) {
    memset(sc, 0, sizeof *sc);
    sc->outer = outer;
    if (!pd || pd->t != TT_PROC_DECL) return;
    const tree_t * params = (pd->n > 1) ? pd->c[1] : NULL;
    const tree_t * locals = (pd->n > 0) ? pd->c[pd->n - 1] : NULL;
    if (locals && locals->t != TT_VLIST) locals = NULL;
    sc->byref = (params && params->t == TT_VLIST) ? params->v.ival : 0;
    if (params && params->t == TT_VLIST) for (int i = 0; i < params->n && sc->n < PAS_MAX_SCOPE; i++) { if (params->c[i] && params->c[i]->v.sval) sc->names[sc->n++] = params->c[i]->v.sval; }
    sc->nparams = sc->n;
    if (locals) for (int i = 0; i < locals->n && sc->n < PAS_MAX_SCOPE; i++) { if (locals->c[i] && locals->c[i]->v.sval) sc->names[sc->n++] = locals->c[i]->v.sval; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_procs(const tree_t * body, const tree_t * parent) {
    if (!body) return;
    for (int i = 0; i < body->n; i++) {
        const tree_t * s = body->c[i];
        if (!s) continue;
        const tree_t * pd = NULL;
        if (s->t == TT_PROC_DECL) pd = s;
        else if (s->t == TT_STMT) {
            for (int j = 0; j < s->n; j++) {
                const tree_t * a = s->c[j];
                if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj") && a->n > 0 && a->c[0] && a->c[0]->t == TT_PROC_DECL) { pd = a->c[0]; break; }
            }
        }
        if (pd && g_pas_nprocs < PAS_MAX_PROCS) {
            g_pas_proc_list[g_pas_nprocs] = pd;
            g_pas_proc_parent[g_pas_nprocs] = parent;
            g_pas_nprocs++;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int proc_decl_level(const tree_t * pd) {
    if (!pd || pd->t != TT_PROC_DECL || pd->n < 1) return 1;
    const tree_t * locals = pd->c[pd->n - 1];
    if (locals && locals->t == TT_VLIST) return (int) locals->v.ival;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void assign_parents(void) {
    for (int i = 0; i < g_pas_nprocs; i++) {
        int my_level = proc_decl_level(g_pas_proc_list[i]);
        g_pas_proc_parent[i] = NULL;
        if (my_level <= 1) continue;
        for (int j = i + 1; j < g_pas_nprocs; j++) {
            if (proc_decl_level(g_pas_proc_list[j]) == my_level - 1) { g_pas_proc_parent[i] = g_pas_proc_list[j]; break; }
        }
        if (!g_pas_proc_parent[i]) {
            for (int j = i - 1; j >= 0; j--) {
                if (proc_decl_level(g_pas_proc_list[j]) == my_level - 1) { g_pas_proc_parent[i] = g_pas_proc_list[j]; break; }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_pascal_enum(const tree_t * prog, const tree_t ** out, int max) {
    g_pas_nprocs = 0;
    if (!prog) return 0;
    collect_procs(prog, NULL);
    assign_parents();
    if (!out) return g_pas_nprocs;
    int n = (g_pas_nprocs < max) ? g_pas_nprocs : max;
    for (int i = 0; i < n; i++) out[i] = g_pas_proc_list[i];
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pas_scope_t * build_scope_chain(const tree_t * pd) {
    if (!pd) return NULL;
    const tree_t * parent_pd = NULL;
    for (int i = 0; i < g_pas_nprocs; i++) {
        if (g_pas_proc_list[i] == pd) { parent_pd = g_pas_proc_parent[i]; break; }
    }
    pas_scope_t * outer = parent_pd ? build_scope_chain(parent_pd) : NULL;
    pas_scope_t * sc = (pas_scope_t *) calloc(1, sizeof(pas_scope_t));
    build_scope(sc, pd, outer);
    for (int i = 0; i < g_pas_nprocs; i++) if (g_pas_proc_parent[i] == pd) { sc->has_children = 1; break; }
    return sc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_pascal_proc(const tree_t * prog, const tree_t * pd) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_PAS); pcx_t cx; memset(&cx, 0, sizeof cx); cx.g = g;
    scan_labels(&cx, pd, NULL);
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED); IR_t * fail = IR_node_alloc(g, IR_FAIL);
    for (int li = 0; li < cx.nlabels; li++) ω_to(cx.labels[li], fail);
    pas_scope_t * sc = build_scope_chain(pd);
    if (sc) cx.sc = *sc;
    const tree_t * body = (pd->n > 2) ? pd->c[2] : NULL;
    int is_func = (pd->n > 3) && pd->c[3] && (pd->c[3]->t == TT_VAR);
    IR_t * top = succ;
    if (is_func) { IR_t * ret = build(&cx, IR_RETURN, succ, succ); IR_t * rv = build(&cx, IR_VAR, NULL, NULL); IR_LIT(rv).sval = pd->v.sval; ir_operand_push(ret, rv); top = ret; }
    IR_t * entry = lower(&cx, body, top, fail);
    g->entry = entry ? entry : succ; return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_pascal(const tree_t * prog) {
    lower_pascal_enum(prog, NULL, 0);
    IR_graph_t * g = IR_alloc(8192, IR_LANG_PAS); pcx_t cx; memset(&cx, 0, sizeof cx); cx.g = g;
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED); IR_t * fail = IR_node_alloc(g, IR_FAIL);
    IR_t * entry = lower(&cx, prog, succ, fail);
    g->entry = entry ? entry : succ; return g;
}

/*====================================================================================================================*/
/* stage2 entry — relocated from lower_program.c (lower_common rung)                                                  */
/*====================================================================================================================*/
#include "stage2.h"
#include "bb_program.h"
extern int scope_get(Scope *sc, const char *name);
extern const char *lp_strdup(const char *s);
/*--------------------------------------------------------------------------------------------------------------------*/
static int pas_scope_chain(int pi, Scope **scs, int *dls, int *pis, int maxd) {
    int n = 0;
    int dl = g_stage2.proc_table[pi].decl_level;
    scs[n] = &g_stage2.proc_table[pi].lower_sc; dls[n] = dl; pis[n] = pi; n++;
    int want = dl - 1, at = pi;
    while (want >= 1 && n < maxd) {
        int found = -1;
        for (int j = at + 1; j < g_stage2.proc_count; j++) {
            const tree_t *pj = (const tree_t *) g_stage2.proc_table[j].proc;
            if (!pj || pj->t != TT_PROC_DECL || !g_stage2.proc_table[j].name || !strcmp(g_stage2.proc_table[j].name, "main")) continue;
            if (g_stage2.proc_table[j].decl_level == want) { found = j; break; }
        }
        if (found < 0) break;
        scs[n] = &g_stage2.proc_table[found].lower_sc; dls[n] = want; pis[n] = found; n++;
        at = found; want--;
    }
    return n;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int lower_pascal_body(const tree_t *prog, const tree_t *proc) {
    IR_graph_t * ng = lower_pascal_proc(prog, proc);
    if (!ng || !ng->entry) return -1;
    return bb_program_add(&g_stage2.bbp, ng);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void lower_pascal_stage2(const tree_t *prog) {
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (!proc || proc->t != TT_PROC_DECL) continue;
        if (g_stage2.proc_table[pi].bb_idx >= 0) continue;
        int bb_idx = lower_pascal_body(prog, proc);
        if (bb_idx >= 0) {
            g_stage2.proc_table[pi].bb_idx = bb_idx;
            const tree_t *plist = (proc->n >= 2) ? proc->c[1] : NULL;
            g_stage2.proc_table[pi].nparams = plist ? plist->n : 0;
            int np = g_stage2.proc_table[pi].nparams;
            Scope *sc = &g_stage2.proc_table[pi].lower_sc;
            sc->n = 0;
            for (int k = 0; k < np && plist && k < plist->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                const tree_t *pv = plist->c[k];
                if (!pv || !pv->v.sval) continue;
                sc->e[sc->n].name = lp_strdup(pv->v.sval);
                sc->e[sc->n].slot = sc->n;
                sc->n++;
            }
            const tree_t *locals = (proc->n >= 1) ? proc->c[proc->n - 1] : NULL;
            if (locals && locals->t == TT_VLIST) {
                g_stage2.proc_table[pi].decl_level = (int) locals->v.ival;
                for (int k = 0; k < locals->n && sc->n < STAGE2_FRAME_SLOT_MAX; k++) {
                    const tree_t *lv = locals->c[k];
                    if (!lv || !lv->v.sval) continue;
                    if (scope_get(sc, lv->v.sval) >= 0) continue;
                    sc->e[sc->n].name = lp_strdup(lv->v.sval);
                    sc->e[sc->n].slot = sc->n;
                    sc->n++;
                }
            }
        }
    }
    int pas_has_nesting = 0;
    for (int pi = 0; pi < g_stage2.proc_count; pi++) {
        const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
        if (proc && proc->t == TT_PROC_DECL && (g_stage2.proc_table[pi].decl_level > 1 || g_stage2.proc_table[pi].byref_mask)) { pas_has_nesting = 1; break; }
    }
    if (pas_has_nesting) {
        for (int pi = 0; pi < g_stage2.proc_count; pi++) {
            const tree_t *proc = (const tree_t *) g_stage2.proc_table[pi].proc;
            if (!proc || proc->t != TT_PROC_DECL) continue;
            if (!g_stage2.proc_table[pi].name || !strcmp(g_stage2.proc_table[pi].name, "main")) continue;
            int idx = g_stage2.proc_table[pi].bb_idx;
            if (idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx]) continue;
            Scope *scs[16]; int dls[16]; int pis[16];
            int nch = pas_scope_chain(pi, scs, dls, pis, 16);
            (void) nch;
            g_stage2.bbp.table[idx]->nslots = g_stage2.proc_table[pi].lower_sc.n + 1;
        }
    }
}
