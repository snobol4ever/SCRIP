#include "lower/lower_prolog/lpl_head.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "lower/lower_common.h"
#include "runtime/rt/prolog_atom.h"
#include "lower/pl_arith_names.h"
#include "lower/lower_prolog/lpl_01_pl.h"
#include "lower/lower_prolog/lpl_02_pl.h"
#include "lower/lower_prolog/lpl_03_pl.h"
#include "lower/lower_prolog/lpl_04_pl.h"
#include "lower/lower_prolog/lpl_05_pl.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lpl_build(lcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
int pl_tree_is_big(const tree_t * t) { return t && t->t == TT_FNC && t->n == 1 && t->v.sval && !strcmp(t->v.sval, "$pl_big") && t->c[0] && t->c[0]->t == TT_QLIT && t->c[0]->v.sval; }
static int pl_tree_is_engine(const tree_t * t) { return t && t->t == TT_FNC && t->n == 1 && t->v.sval && !strcmp(t->v.sval, "$pl_engine") && t->c[0] && t->c[0]->t == TT_ILIT; }
static const char * pl_list_fname(const char * nm, int ar) { return (g_stage2.pl_dialect == PL_DIALECT_SWI && ar == 2 && nm && !strcmp(nm, "[|]")) ? "." : nm; }
static int pl_tree_is_c0(const tree_t * t) { return t && t->t == TT_FNC && t->n == 0 && t->v.sval && strcmp(t->v.sval, "{}") != 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * mkc_node(lcx_t * cx, const char * fname, int nkids, IR_t ** kids, IR_t ** kid_entries, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, NULL, cx->tω);
    IR_LIT(nd).sval = "$mkc";
    extern int prolog_atom_intern(const char *);
    extern int prolog_functor_intern(int, int);
    IR_t * fn = lpl_build(cx, IR_LIT_INTEGER, NULL, cx->tω);
    IR_LIT(fn).ival = (int64_t)prolog_functor_intern(prolog_atom_intern(pl_list_fname(fname, nkids)), nkids);
    ir_operand_push(nd, fn);
    IR_t * prev = fn;
    IR_t * first = fn;
    for (int i = 0; i < nkids; i++) {
        IR_t * ke = kid_entries[i] ? kid_entries[i] : kids[i];
        lc_γ_to(prev, ke);
        if (cx->tω) lc_ω_to(kids[i], cx->tω);
        prev = kids[i];
        ir_operand_push(nd, kids[i]);
    }
    lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_atom_lit_sval(const tree_t * t) {
    if (!t->v.sval || t->slen <= 0 || !memchr(t->v.sval, 0, (size_t)t->slen)) return t->v.sval;
    char * o = (char *)ct_alloc(2 * (size_t)t->slen + 1);
    size_t k = 0;
    for (int i = 0; i < t->slen; i++) { if (t->v.sval[i]) o[k++] = t->v.sval[i]; else { o[k++] = (char)0xC0; o[k++] = (char)0x80; } }
    o[k] = 0;
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_is_cmp(const tree_t * t) { return t && ((t->t == TT_MAKELIST && t->n > 0) || (t->t == TT_FNC && t->n > 0 && t->v.sval && !pl_tree_is_big(t) && !pl_tree_is_engine(t))); }
static int pl_tree_ground(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_ILIT || t->t == TT_FLIT || (t->t == TT_MAKELIST && t->n == 0)) return 1;
    if (t->t == TT_QLIT) return t->v.sval != NULL;
    if (!pl_tree_is_cmp(t)) return 0;
    for (int i = 0; i < t->n; i++) if (!pl_tree_ground(t->c[i])) return 0;
    return 1;
}
static int pl_tree_ground_nested(const tree_t * t) {
    if (!pl_tree_is_cmp(t) || !pl_tree_ground(t)) return 0;
    if (t->t == TT_MAKELIST && (t->v.ival == 1 ? t->n - 1 : t->n) >= 2) return 1;
    for (int i = 0; i < t->n; i++) if (pl_tree_is_cmp(t->c[i])) return 1;
    return 0;
}
static int pl_ground_reserve(int n) {
    int base = g_stage2.pl_ground_n;
    if (base + n > g_stage2.pl_ground_cap) {
        int cap = g_stage2.pl_ground_cap ? g_stage2.pl_ground_cap : 256;
        while (cap < base + n) cap *= 2;
        g_stage2.pl_ground_words = (long long *)(g_stage2.pl_ground_words ? ct_grow(g_stage2.pl_ground_words, (size_t)cap * 16) : ct_alloc((size_t)cap * 16));
        g_stage2.pl_ground_cap = cap;
    }
    g_stage2.pl_ground_n = base + n;
    return base;
}
static void pl_ground_put(int at, uint64_t w0, uint64_t w1) { g_stage2.pl_ground_words[2 * at] = (long long)w0; g_stage2.pl_ground_words[2 * at + 1] = (long long)w1; }
static uint64_t pl_ground_fid(const char * nm, int n) {
    extern int prolog_atom_intern(const char *);
    extern int prolog_functor_intern(int, int);
    return (uint64_t)(uint32_t)prolog_functor_intern(prolog_atom_intern(pl_list_fname(nm, n)), n);
}
static void pl_ground_fill(const tree_t * t, int at, int base) {
    extern int prolog_atom_intern_lit(const char *);
    for (;;) {
        if (t->t == TT_ILIT) { pl_ground_put(at, (uint64_t)DT_I, (uint64_t)t->v.ival); return; }
        if (t->t == TT_FLIT) { union { double d; uint64_t u; } b; b.d = t->v.dval; pl_ground_put(at, (uint64_t)DT_R, b.u); return; }
        if (!pl_tree_is_cmp(t)) { pl_ground_put(at, (uint64_t)DT_PLATOM, (uint64_t)(uint32_t)prolog_atom_intern_lit(t->t == TT_QLIT ? pl_atom_lit_sval(t) : "[]")); return; }
        if (t->t == TT_MAKELIST) {
            int bar = (t->v.ival == 1 && t->n > 0), ne = bar ? t->n - 1 : t->n;
            for (int i = 0; i < ne; i++) {
                int a = pl_ground_reserve(2);
                pl_ground_put(at, (uint64_t)DT_PLREF | (pl_ground_fid(".", 2) << 32), (2ull << 32) | (uint64_t)(a - base));
                pl_ground_fill(t->c[i], a, base);
                at = a + 1;
            }
            if (!bar) { pl_ground_put(at, (uint64_t)DT_PLATOM, (uint64_t)(uint32_t)prolog_atom_intern_lit("[]")); return; }
            t = t->c[t->n - 1];
        } else {
            int n = t->n, a = pl_ground_reserve(n);
            pl_ground_put(at, (uint64_t)DT_PLREF | (pl_ground_fid(t->v.sval, n) << 32), ((uint64_t)(uint32_t)n << 32) | (uint64_t)(a - base));
            for (int i = 0; i + 1 < n; i++) pl_ground_fill(t->c[i], a + i, base);
            at = a + n - 1;
            t = t->c[n - 1];
        }
    }
}
static IR_t * mkground_node(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    int base = pl_ground_reserve(1);
    IR_t * nd = lpl_build(cx, IR_CALL, NULL, cx->tω);
    IR_t * id = lpl_build(cx, IR_LIT_INTEGER, NULL, cx->tω);
    IR_LIT(nd).sval = "$mkground";
    pl_ground_fill(t, base, base);
    IR_LIT(id).ival = ((int64_t)(g_stage2.pl_ground_n - base) << 32) | (int64_t)base;
    ir_operand_push(nd, id);
    lc_γ_to(id, nd);
    if (entry_out) *entry_out = id;
    return nd;
}
int pl_ground_words(long long id, long long ** out) {
    int base = (int)(id & 0xFFFFFFFFll), n = (int)(id >> 32);
    *out = g_stage2.pl_ground_words ? g_stage2.pl_ground_words + 2 * base : (long long *)0;
    return *out ? 2 * n : 0;
}
static int pl_mkground_functor_is(const IR_t * f, const char * nm, int ar) {
    extern int prolog_functor_name(int);
    extern const char * prolog_atom_name(int);
    long long * w = 0;
    if (!f || f->op != IR_LIT_INTEGER || pl_ground_words((long long)IR_LIT(f).ival, &w) < 2) return 0;
    { const char * s = prolog_atom_name(prolog_functor_name((int)((uint64_t)w[0] >> 32))); return (int)((uint64_t)w[1] >> 32) == ar && s && !strcmp(s, nm); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * term_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!t) return NULL;
    switch (t->t) {
        case TT_QLIT:
        { IR_t * nd = lpl_build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(nd).sval = pl_atom_lit_sval(t); return nd; }
        case TT_ILIT:
        { IR_t * nd = lpl_build(cx, IR_LIT_INTEGER, NULL, cx->tω); IR_LIT(nd).ival = t->v.ival; return nd; }
        case TT_DQLIT:
        {
            IR_t * nd = lpl_build(cx, IR_LIT_STRING, NULL, cx->tω);
            IR_LIT(nd).sval = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : "";
            if (t->n > 0 && t->c[0] && t->c[0]->slen > 0) { IR_t * ln = lpl_build(cx, IR_LIT_INTEGER, NULL, NULL); IR_LIT(ln).ival = (int64_t) t->c[0]->slen; ir_operand_push(nd, ln); }
            return nd;
        }
        case TT_FLIT:
        { IR_t * nd = lpl_build(cx, IR_LIT_REAL, NULL, cx->tω); IR_LIT(nd).dval = t->v.dval; return nd; }
        case TT_VAR:
        { IR_t * nd = lpl_build(cx, IR_VAR, NULL, cx->tω); IR_LIT(nd).sval = pl_vname(cx, (int) t->v.ival); return nd; }
        case TT_MAKELIST:
        {
            int bar = (t->v.ival == 1 && t->n > 0);
            if (pl_tree_ground_nested(t)) return mkground_node(cx, t, entry_out);
            IR_t * prev;
            IR_t * prev_e = NULL;
            if (bar) prev = term_lval_e(cx, t->c[t->n - 1], &prev_e);
            else { prev = lpl_build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(prev).sval = "[]"; }
            for (int i = (bar ? t->n - 2 : t->n - 1); i >= 0; i--) {
                IR_t * ee = NULL;
                IR_t * e = term_lval_e(cx, t->c[i], &ee);
                IR_t * kids[2] = { e, prev };
                IR_t * kes[2] = { ee, prev_e };
                prev = mkc_node(cx, ".", 2, kids, kes, &prev_e);
            }
            if (entry_out) *entry_out = prev_e;
            return prev;
        }
        case TT_FNC:
        {
            int nk = t->n;
            if (pl_tree_ground_nested(t)) return mkground_node(cx, t, entry_out);
            if (pl_tree_is_big(t)) {
                IR_t * nd = lpl_build(cx, IR_CALL, NULL, cx->tω);
                IR_LIT(nd).sval = "$pl_big";
                IR_t * dg = lpl_build(cx, IR_LIT_STRING, NULL, cx->tω);
                IR_LIT(dg).sval = t->c[0]->v.sval;
                ir_operand_push(nd, dg);
                lc_γ_to(dg, nd);
                if (entry_out) *entry_out = dg;
                return nd;
            }
            if (pl_tree_is_engine(t)) {
                IR_t * nd = lpl_build(cx, IR_CALL, NULL, cx->tω);
                IR_LIT(nd).sval = "$pl_engine";
                IR_t * dg = lpl_build(cx, IR_LIT_STRING, NULL, cx->tω);
                IR_LIT(dg).sval = ct_fmt("%ld", (long) t->c[0]->v.ival);
                ir_operand_push(nd, dg);
                lc_γ_to(dg, nd);
                if (entry_out) *entry_out = dg;
                return nd;
            }
            if (nk == 0 && pl_tree_is_c0(t)) return mkc_node(cx, t->v.sval, 0, (IR_t **) 0, (IR_t **) 0, entry_out);
            if (nk == 0) { IR_t * nd = lpl_build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : "?"; return nd; }
            IR_t ** kids = (IR_t **) ct_zalloc((size_t)(nk > 0 ? nk : 1), sizeof(IR_t *));
            IR_t ** kes = (IR_t **) ct_zalloc((size_t)(nk > 0 ? nk : 1), sizeof(IR_t *));
            for (int i = 0; i < nk; i++) kids[i] = term_lval_e(cx, t->c[i], &kes[i]);
            IR_t * nd = mkc_node(cx, t->v.sval ? t->v.sval : "?", nk, kids, kes, entry_out);
            ct_drop(kids);
            ct_drop(kes);
            return nd;
        }
        case TT_CUT:
        { IR_t * nd = lpl_build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(nd).sval = "!"; return nd; }
        case TT_UNIFY:
        if (t->n == 2) { IR_t * kids[2], * kes[2]; for (int i = 0; i < 2; i++) kids[i] = term_lval_e(cx, t->c[i], &kes[i]); return mkc_node(cx, "=", 2, kids, kes, entry_out); }
        { IR_t * nd = lpl_build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(nd).sval = "?"; return nd; }
        default:
        { IR_t * nd = lpl_build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(nd).sval = "?"; return nd; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_same_functor(const tree_t * a, const tree_t * b) {
    return a && b && a->t == TT_FNC && b->t == TT_FNC && a->n == b->n && a->n > 0 && a->v.sval && b->v.sval && !strcmp(a->v.sval, b->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_const_lit(lcx_t * cx, const tree_t * t) {
    IR_t * nd = (IR_t *)0;
    if (!t) return nd;
    if (t->t == TT_QLIT || (t->t == TT_FNC && t->n == 0 && !pl_tree_is_c0(t))) {
        nd = lpl_build(cx, IR_LIT_ATOM, NULL, NULL);
        IR_LIT(nd).sval = t->v.sval ? pl_atom_lit_sval(t) : "?";
    } else if (t->t == TT_MAKELIST && t->n == 0) {
        nd = lpl_build(cx, IR_LIT_ATOM, NULL, NULL);
        IR_LIT(nd).sval = "[]";
    } else if (t->t == TT_CUT) {
        nd = lpl_build(cx, IR_LIT_ATOM, NULL, NULL);
        IR_LIT(nd).sval = "!";
    } else if (t->t == TT_ILIT) {
        nd = lpl_build(cx, IR_LIT_INTEGER, NULL, NULL);
        IR_LIT(nd).ival = t->v.ival;
    }
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_unify_node3(lcx_t * cx, IR_e op, IR_t * src, long idx, IR_t * payload, IR_t * γ, IR_t * ω) {
    IR_t * u = lpl_build(cx, op, γ, ω);
    IR_t * ix = lpl_build(cx, IR_LIT_INTEGER, NULL, NULL);
    IR_LIT(ix).ival = (int64_t) idx;
    ir_operand_push(u, src);
    ir_operand_push(u, ix);
    if (payload) ir_operand_push(u, payload);
    return u;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_unify_const_node(lcx_t * cx, const char * vname, IR_t * klit, IR_t * γ, IR_t * ω) {
    IR_t * v = lpl_build(cx, IR_VAR_REF, NULL, NULL);
    IR_LIT(v).sval = vname;
    return pl_unify_node3(cx, IR_UNIFY_CONST, v, 0, klit, γ, ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_head_first_walk(const tree_t * t, const tree_t * occ, int slot, int * found) {
    if (!t || *found) return 0;
    if (t->t == TT_VAR) { if ((int) t->v.ival == slot) { *found = 1; return t == occ; } return 0; }
    for (int i = 0; i < t->n; i++) { int r = pl_head_first_walk(t->c[i], occ, slot, found); if (*found) return r; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_head_is_first(const tree_t * cl, int ar, const tree_t * occ, int slot) {
    int found = 0;
    for (int i = 0; i < ar; i++) { int r = pl_head_first_walk(cl->c[i], occ, slot, &found); if (found) return r; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_head_boxable(const tree_t * t) {
    if (!t) return 0;
    switch (t->t) {
        case TT_VAR:
        case TT_QLIT:
        case TT_ILIT:
        case TT_CUT:
        return 1;
        case TT_MAKELIST:
        for (int i = 0; i < t->n; i++) if (!pl_head_boxable(t->c[i])) return 0;
        return 1;
        case TT_FNC:
        if (pl_tree_is_big(t) || pl_tree_is_engine(t)) return 0;
        for (int i = 0; i < t->n; i++) if (!pl_head_boxable(t->c[i])) return 0;
        return 1;
        default:
        return 0;
    }
}
static IR_t * pl_head_list(lcx_t * cx, const tree_t * cl, int ar, IR_t * src, long idx, const tree_t * t, int i, IR_t * next, IR_t * step) {
    int bar = (t->v.ival == 1 && t->n > 0);
    int last = bar ? t->n - 1 : t->n;
    if (i >= last) {
        if (bar) return pl_head_term(cx, cl, ar, src, idx, t->c[t->n - 1], next, step);
        { IR_t * nil = lpl_build(cx, IR_LIT_ATOM, NULL, NULL); IR_LIT(nil).sval = "[]"; return pl_unify_node3(cx, IR_UNIFY_CONST, src, idx, nil, next, step); }
    }
    {
        extern int prolog_atom_intern(const char *);
        extern int prolog_functor_intern(int, int);
        IR_t * st = lpl_build(cx, IR_UNIFY_STRUCT, NULL, step);
        IR_t * nx = pl_head_list(cx, cl, ar, st, 1, t, i + 1, next, step);
        nx = pl_head_term(cx, cl, ar, st, 0, t->c[i], nx, step);
        lc_γ_to(st, nx);
        {
            IR_t * f = lpl_build(cx, IR_LIT_INTEGER, NULL, NULL);
            IR_LIT(f).ival = (int64_t) prolog_functor_intern(prolog_atom_intern("."), 2);
            IR_t * ix = lpl_build(cx, IR_LIT_INTEGER, NULL, NULL);
            IR_LIT(ix).ival = (int64_t) idx;
            ir_operand_push(st, src);
            ir_operand_push(st, ix);
            ir_operand_push(st, f);
        }
        return st;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_head_term(lcx_t * cx, const tree_t * cl, int ar, IR_t * src, long idx, const tree_t * t, IR_t * next, IR_t * step) {
    if (t->t == TT_VAR) {
        int sl = (int) t->v.ival;
        if (sl < 0) return next;
        { int n = 0; for (int i = 0; i < cl->n; i++) n += pl_var_count(cl->c[i], sl); if (n <= 1) return next; }
        { IR_t * u = pl_unify_node3(cx, pl_head_is_first(cl, ar, t, sl) ? IR_UNIFY_FIRST : IR_UNIFY_VALUE, src, idx, NULL, next, step); IR_LIT(u).sval = pl_vname(cx, sl); return u; }
    }
    { IR_t * klit = pl_const_lit(cx, t); if (klit) return pl_unify_node3(cx, IR_UNIFY_CONST, src, idx, klit, next, step); }
    if (t->t == TT_MAKELIST) return pl_head_list(cx, cl, ar, src, idx, t, 0, next, step);
    if (t->t == TT_FNC && (t->n > 0 || pl_tree_is_c0(t))) {
        extern int prolog_atom_intern(const char *);
        extern int prolog_functor_intern(int, int);
        IR_t * st = lpl_build(cx, IR_UNIFY_STRUCT, NULL, step);
        IR_t * nx = next;
        for (int j = t->n - 1; j >= 0; j--) nx = pl_head_term(cx, cl, ar, st, j, t->c[j], nx, step);
        lc_γ_to(st, nx);
        {
            IR_t * f = lpl_build(cx, IR_LIT_INTEGER, NULL, NULL);
            IR_LIT(f).ival = (int64_t) prolog_functor_intern(prolog_atom_intern(pl_list_fname(t->v.sval ? t->v.sval : "?", t->n)), t->n);
            IR_t * ix = lpl_build(cx, IR_LIT_INTEGER, NULL, NULL);
            IR_LIT(ix).ival = (int64_t) idx;
            ir_operand_push(st, src);
            ir_operand_push(st, ix);
            ir_operand_push(st, f);
        }
        return st;
    }
    pl_refuse("head term shape the boxes do not cover in", t->v.sval ? t->v.sval : "?", 2);
    return next;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * unify_pair(lcx_t * cx, const tree_t * lt, const tree_t * rt, IR_t * γ, IR_t * ω, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    {
        const tree_t * vt = (lt && lt->t == TT_VAR) ? lt : (rt && rt->t == TT_VAR) ? rt : (const tree_t *)0;
        const tree_t * kt = (vt == lt) ? rt : lt;
        if (vt && (int) vt->v.ival >= 0) { IR_t * klit = pl_const_lit(cx, kt); if (klit) return pl_unify_const_node(cx, pl_vname(cx, (int) vt->v.ival), klit, γ, ω); }
    }
    if (pl_same_functor(lt, rt)) {
        IR_t * next = γ;
        IR_t * first_entry = γ;
        IR_t * head = NULL;
        for (int i = lt->n - 1; i >= 0; i--) { IR_t * e = NULL; IR_t * u = unify_pair(cx, lt->c[i], rt->c[i], next, ω, &e); next = e ? e : u; head = u; if (i == 0) first_entry = next; }
        if (entry_out) *entry_out = first_entry;
        return head;
    }
    IR_t * nd = lpl_build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = "$unify";
    IR_t * e0 = NULL;
    IR_t * e1 = NULL;
    IR_t * a0 = term_lval_e(cx, lt, &e0);
    IR_t * a1 = term_lval_e(cx, rt, &e1);
    lc_γ_to(a0, e1 ? e1 : a1);
    lc_ω_to(a0, ω);
    lc_γ_to(a1, nd);
    lc_ω_to(a1, ω);
    ir_operand_push(nd, a0);
    ir_operand_push(nd, a1);
    if (entry_out) *entry_out = e0 ? e0 : a0;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_cmp_op_suffix(const char * s) {
    if (!s) return NULL;
    if (!strcmp(s, "<")) return "lt";
    if (!strcmp(s, ">")) return "gt";
    if (!strcmp(s, "=<")) return "le";
    if (!strcmp(s, ">=")) return "ge";
    if (!strcmp(s, "=:=")) return "eq";
    if (!strcmp(s, "=\\=")) return "ne";
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_ax_suffix(const char * s, int ar) {
    if (g_stage2.pl_dialect == PL_DIALECT_XSB && ar == 2 && !strcmp(s, "><")) return "xor";
    if (g_stage2.pl_dialect == PL_DIALECT_XSB && ar == 1 && (!strcmp(s, "asin") || !strcmp(s, "acos") || !strcmp(s, "tan") || !strcmp(s, "log10"))) return pl_ax_suffix_ext(s, ar);
    return pl_ax_suffix_of(s, ar);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ax_divides(const char * sfx) { if (!sfx) return 0; return !strcmp(sfx, "div") || !strcmp(sfx, "idiv") || !strcmp(sfx, "divf") || !strcmp(sfx, "mod") || !strcmp(sfx, "rem"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_arith_val(lcx_t * cx, const tree_t * t, IR_t * ωfail, IR_t ** entry_out) {
    if (pl_tree_is_big(t)) {
        IR_t * nd = lpl_build(cx, IR_CALL, NULL, ωfail);
        IR_LIT(nd).sval = "$pl_big";
        IR_t * dg = lpl_build(cx, IR_LIT_STRING, NULL, ωfail);
        IR_LIT(dg).sval = t->c[0]->v.sval;
        ir_operand_push(nd, dg);
        lc_γ_to(dg, nd);
        lc_ω_to(dg, ωfail);
        if (entry_out) *entry_out = dg;
        return nd;
    }
    if (t && (t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval && pl_ax_suffix(t->v.sval, 0)) {
        char nb[fmt_len("$ax_%s", pl_ax_suffix(t->v.sval, 0))];
        snprintf(nb, sizeof nb, "$ax_%s", pl_ax_suffix(t->v.sval, 0));
        IR_t * nd = lpl_build(cx, IR_CALL, NULL, ωfail);
        IR_LIT(nd).sval = ct_strdup(nb);
        if (entry_out) *entry_out = nd;
        return nd;
    }
    if (t && t->t == TT_FNC && t->v.sval && (t->n == 1 || t->n == 2) && pl_ax_suffix(t->v.sval, t->n)) {
        char nb[fmt_len("$ax_%s", pl_ax_suffix(t->v.sval, t->n))];
        snprintf(nb, sizeof nb, "$ax_%s", pl_ax_suffix(t->v.sval, t->n));
        IR_t * nd = lpl_build(cx, IR_CALL, NULL, ωfail);
        IR_LIT(nd).sval = ct_strdup(nb);
        IR_t * prev = NULL;
        IR_t * first = NULL;
        IR_t * dvsr = NULL;
        for (int i = 0; i < t->n; i++) {
            IR_t * ke = NULL;
            IR_t * k = lower_arith_val(cx, t->c[i], ωfail, &ke);
            IR_t * en = ke ? ke : k;
            if (prev) lc_γ_to(prev, en);
            else first = en;
            lc_ω_to(k, ωfail);
            prev = k;
            ir_operand_push(nd, k);
            if (i == 1) dvsr = k;
        }
        if (dvsr && t->n == 2 && pl_ax_divides(pl_ax_suffix(t->v.sval, 2))) {
            IR_t * zg = lpl_build(cx, IR_CALL, nd, ωfail);
            IR_LIT(zg).sval = "$ax_zguard";
            IR_t * opn = lpl_build(cx, IR_LIT_STRING, zg, ωfail);
            IR_LIT(opn).sval = ct_strdup(t->v.sval);
            ir_operand_push(zg, dvsr);
            ir_operand_push(zg, opn);
            if (prev) lc_γ_to(prev, opn);
            prev = zg;
        }
        if (prev) lc_γ_to(prev, nd);
        if (entry_out) *entry_out = first ? first : nd;
        return nd;
    }
    return term_e(cx, t, entry_out);
}
IR_t * pl_cut_redo(lcx_t * cx) { return lpl_build(cx, IR_GOTO, cx->cutω, cx->cutω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_leaf_guarded1(lcx_t * cx, const char * sym, const char * guard_sym, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * ve = NULL;
    IR_t * v = term_lval_e(cx, t->c[0], &ve);
    lc_ω_to(v, ωfail);
    IR_t * chk = lpl_build(cx, IR_CALL, nd, ωfail);
    IR_LIT(chk).sval = guard_sym;
    ir_operand_push(chk, v);
    ir_operand_push(nd, v);
    lc_γ_to(v, chk);
    if (entry_out) *entry_out = ve ? ve : v;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_leaf_guarded1_dir(lcx_t * cx, const char * sym, const char * guard_sym, const char * dir, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * ve = NULL;
    IR_t * v = term_lval_e(cx, t->c[0], &ve);
    lc_ω_to(v, ωfail);
    IR_t * chk = lpl_build(cx, IR_CALL, nd, ωfail);
    IR_LIT(chk).sval = guard_sym;
    IR_t * dl = lpl_build(cx, IR_LIT_STRING, chk, ωfail);
    IR_LIT(dl).sval = dir;
    ir_operand_push(chk, v);
    ir_operand_push(chk, dl);
    ir_operand_push(nd, v);
    lc_γ_to(v, dl);
    if (entry_out) *entry_out = ve ? ve : v;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_dyn_rt_declare(const char * name, int arity) {
    extern int rt_pl_db_decl(void *, const char *, int64_t, int64_t);
    if (g_stage2.rt_lowering && g_stage2.rt_root) (void) rt_pl_db_decl(g_stage2.rt_root, name, arity, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_db_leaf1(lcx_t * cx, const char * sym, int k, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = (char *) sym;
    IR_t * kn = lpl_build(cx, IR_LIT_INTEGER, NULL, ωfail);
    IR_LIT(kn).ival = k;
    lc_γ_to(kn, nd);
    lc_ω_to(kn, ωfail);
    ir_operand_push(nd, kn);
    if (entry_out) *entry_out = kn;
    return nd;
}
static const struct {
    const char * nm;
    int ar;
} pl_iso_pis[] = { { "!", 0 }, { ",", 2 }, { "->", 2 }, { ";", 2 }, { "<", 2 }, { "=", 2 }, { "=..", 2 }, { "=:=", 2 }, { "=<", 2 }, { "==", 2 }, { "=\\=", 2 }, { ">", 2 }, { ">=", 2 }, { "@<", 2 },
    { "@=<", 2 }, { "@>", 2 }, { "@>=", 2 }, { "\\+", 1 }, { "\\=", 2 }, { "\\==", 2 }, { "abolish", 1 }, { "acyclic_term", 1 }, { "arg", 3 }, { "asserta", 1 }, { "assertz", 1 }, { "at_end_of_stream",
    0 }, { "at_end_of_stream", 1 }, { "atom", 1 }, { "atom_chars", 2 }, { "atom_codes", 2 }, { "atom_concat", 3 }, { "atom_length", 2 }, { "atomic", 1 }, { "bagof", 3 }, { "call", 1 }, { "call", 2 },
    { "call", 3 }, { "call", 4 }, { "call", 5 }, { "call", 6 }, { "call", 7 }, { "call", 8 }, { "callable", 1 }, { "catch", 3 }, { "char_code", 2 }, { "char_conversion", 2 }, { "clause", 2 },
    { "close", 1 }, { "close", 2 }, { "compare", 3 }, { "compound", 1 }, { "copy_term", 2 }, { "current_char_conversion", 2 }, { "current_input", 1 }, { "current_op", 3 }, { "current_output", 1 },
    { "current_predicate", 1 }, { "current_prolog_flag", 2 }, { "discontiguous", 1 }, { "dynamic", 1 }, { "fail", 0 }, { "false", 0 }, { "findall", 3 }, { "float", 1 }, { "flush_output", 0 },
    { "flush_output", 1 }, { "functor", 3 }, { "get_byte", 1 }, { "get_byte", 2 }, { "get_char", 1 }, { "get_char", 2 }, { "get_code", 1 }, { "get_code", 2 }, { "ground", 1 }, { "halt", 0 }, { "halt",
    1 }, { "initialization", 1 }, { "integer", 1 }, { "is", 2 }, { "keysort", 2 }, { "length", 2 }, { "message_queue_create", 2 }, { "message_queue_destroy", 1 }, { "message_queue_property", 2 },
    { "multifile", 1 }, { "mutex_create", 2 }, { "mutex_destroy", 1 }, { "mutex_lock", 1 }, { "mutex_property", 2 }, { "mutex_trylock", 1 }, { "mutex_unlock", 1 }, { "nl", 0 }, { "nl", 1 },
    { "nonvar", 1 }, { "number", 1 }, { "number_chars", 2 }, { "number_codes", 2 }, { "numbervars", 3 }, { "once", 1 }, { "op", 3 }, { "open", 3 }, { "open", 4 }, { "peek_byte", 1 }, { "peek_byte",
    2 }, { "peek_char", 1 }, { "peek_char", 2 }, { "peek_code", 1 }, { "peek_code", 2 }, { "phrase", 2 }, { "phrase", 3 }, { "predicate_property", 2 }, { "put_byte", 1 }, { "put_byte", 2 },
    { "put_char", 1 }, { "put_char", 2 }, { "put_code", 1 }, { "put_code", 2 }, { "read", 1 }, { "read", 2 }, { "read_term", 2 }, { "read_term", 3 }, { "repeat", 0 }, { "retract", 1 }, { "retractall",
    1 }, { "set_input", 1 }, { "set_output", 1 }, { "set_prolog_flag", 2 }, { "set_stream_position", 2 }, { "setof", 3 }, { "sort", 2 }, { "stream_property", 2 }, { "sub_atom", 5 }, { "subsumes_term",
    2 }, { "term_variables", 2 }, { "thread_create", 3 }, { "thread_detach", 1 }, { "thread_get_message", 1 }, { "thread_get_message", 2 }, { "thread_get_message", 3 }, { "thread_peek_message", 1 },
    { "thread_peek_message", 2 }, { "thread_property", 2 }, { "thread_self", 1 }, { "thread_send_message", 2 }, { "thread_signal", 2 }, { "throw", 1 }, { "true", 0 }, { "unify_with_occurs_check", 2 },
    { "var", 1 }, { "with_mutex", 2 }, { "write", 1 }, { "write", 2 }, { "write_canonical", 1 }, { "write_canonical", 2 }, { "write_term", 2 }, { "write_term", 3 }, { "writeq", 1 }, { "writeq", 2 },
    };
int pl_pi_is_iso(const char * pn, int ar) {
    for (size_t i = 0; pn && i < sizeof pl_iso_pis / sizeof pl_iso_pis[0]; i++) if (pl_iso_pis[i].ar == ar && !strcmp(pl_iso_pis[i].nm, pn)) return 1;
    return 0;
}
const tree_t * pl_tree_number(const tree_t * t) { return (t && (t->t == TT_ILIT || t->t == TT_FLIT || pl_tree_is_big(t))) ? t : NULL; }
const tree_t * pl_body_ill_typed(const tree_t * b) {
    if (!b) return NULL;
    if (pl_tree_number(b)) return b;
    if (b->t == TT_FNC && b->v.sval && b->n == 2 && (!strcmp(b->v.sval, ",") || !strcmp(b->v.sval, ";") || !strcmp(b->v.sval, "->") || !strcmp(b->v.sval, "*->") || !strcmp(b->v.sval, "|")))
        return pl_body_ill_typed(b->c[0]) ? b : (pl_body_ill_typed(b->c[1]) ? b : NULL);
    if (b->t == TT_FNC && b->v.sval && b->n == 1 && !strcmp(b->v.sval, "\\+")) return pl_body_ill_typed(b->c[0]) ? b : NULL;
    return NULL;
}
const tree_t * pl_clause_ill_typed(const tree_t * cl) {
    if (!cl) return NULL;
    if (cl->t == TT_FNC && cl->v.sval && !strcmp(cl->v.sval, ":-") && cl->n == 2) { if (pl_tree_number(cl->c[0])) return cl->c[0]; return pl_body_ill_typed(cl->c[1]); }
    return pl_tree_number(cl);
}
const char * pl_spec_key(const tree_t * s, int * ar_out) {
    if (!s || s->t != TT_FNC || !s->v.sval || strcmp(s->v.sval, "/") || s->n != 2) return NULL;
    if (!s->c[0] || !(s->c[0]->t == TT_QLIT || s->c[0]->t == TT_NAME) || !s->c[0]->v.sval) return NULL;
    if (!s->c[1] || s->c[1]->t != TT_ILIT) return NULL;
    if (ar_out) *ar_out = (int) s->c[1]->v.ival;
    return s->c[0]->v.sval;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_db_leaf2_tree(lcx_t * cx, const char * sym, const tree_t * a0, const tree_t * a1, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = (char *) sym;
    IR_t * e0 = NULL;
    IR_t * v0 = term_e(cx, a0, &e0);
    IR_t * e1 = NULL;
    IR_t * v1 = term_e(cx, a1, &e1);
    lc_γ_to(v0, e1 ? e1 : v1);
    lc_ω_to(v0, ωfail);
    lc_γ_to(v1, nd);
    lc_ω_to(v1, ωfail);
    ir_operand_push(nd, v0);
    ir_operand_push(nd, v1);
    if (entry_out) *entry_out = e0 ? e0 : v0;
    return nd;
}
IR_t * pl_db_leaf2(lcx_t * cx, const char * sym, int k, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = (char *) sym;
    IR_t * kn = lpl_build(cx, IR_LIT_INTEGER, NULL, ωfail);
    IR_LIT(kn).ival = k;
    IR_t * te = NULL;
    IR_t * tv = term_e(cx, arg, &te);
    lc_γ_to(kn, te ? te : tv);
    lc_ω_to(kn, ωfail);
    lc_γ_to(tv, nd);
    lc_ω_to(tv, ωfail);
    ir_operand_push(nd, kn);
    ir_operand_push(nd, tv);
    if (entry_out) *entry_out = kn;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_db_enum(lcx_t * cx, int k, const tree_t * target, int erase, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * er = NULL;
    if (erase) { er = lpl_build(cx, IR_CALL, γnext, ωfail); IR_LIT(er).sval = "$db_erase"; }
    IR_t * uni = lpl_build(cx, IR_CALL, er ? er : γnext, ωfail);
    IR_LIT(uni).sval = "$clause_unify";
    IR_t * cp = lpl_build(cx, IR_CALL, uni, ωfail);
    IR_LIT(cp).sval = "$db_copy";
    IR_t * to = lpl_build(cx, IR_TO, cp, ωfail);
    IR_LIT(to).sval = (char *) "db:trl";
    IR_t * z = lpl_build(cx, IR_LIT_INTEGER, to, ωfail);
    IR_LIT(z).ival = 0;
    IR_t * te = NULL;
    IR_t * tv = term_e(cx, target, &te);
    IR_t * kn;
    if (k < 0) {
        kn = lpl_build(cx, IR_CALL, z, ωfail);
        IR_LIT(kn).sval = "$db_store_k";
        ir_operand_push(kn, tv);
        lc_γ_to(tv, kn);
        lc_ω_to(tv, ωfail);
    } else {
        kn = lpl_build(cx, IR_LIT_INTEGER, NULL, ωfail);
        IR_LIT(kn).ival = k;
        lc_γ_to(kn, te ? te : tv);
        lc_ω_to(kn, ωfail);
        lc_γ_to(tv, z);
        lc_ω_to(tv, ωfail);
    }
    ir_operand_push(to, kn);
    ir_operand_push(to, z);
    ir_operand_push(to, tv);
    ir_operand_push(cp, kn);
    ir_operand_push(cp, to);
    ir_operand_push(uni, cp);
    ir_operand_push(uni, tv);
    if (er) { ir_operand_push(er, kn); ir_operand_push(er, to); lc_ω_to_β(er, to); }
    lc_ω_to_β(cp, to);
    lc_ω_to_β(uni, to);
    if (entry_out) *entry_out = (k < 0) ? (te ? te : tv) : kn;
    return to;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_db_data(lcx_t * cx, int k, const tree_t * target, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * uni = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(uni).sval = "$db_data_unify";
    IR_t * to = lpl_build(cx, IR_TO, uni, ωfail);
    IR_LIT(to).sval = (char *) "db:trl:det";
    IR_t * z = lpl_build(cx, IR_LIT_INTEGER, to, ωfail);
    IR_LIT(z).ival = 0;
    IR_t * te = NULL;
    IR_t * tv = term_e(cx, target, &te);
    IR_t * kn = lpl_build(cx, IR_LIT_INTEGER, NULL, ωfail);
    IR_LIT(kn).ival = k;
    lc_γ_to(kn, te ? te : tv);
    lc_ω_to(kn, ωfail);
    lc_γ_to(tv, z);
    lc_ω_to(tv, ωfail);
    ir_operand_push(to, kn);
    ir_operand_push(to, z);
    ir_operand_push(to, tv);
    ir_operand_push(uni, kn);
    ir_operand_push(uni, to);
    ir_operand_push(uni, tv);
    lc_ω_to_β(uni, to);
    if (entry_out) *entry_out = kn;
    return to;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_db_enum_ref(lcx_t * cx, const tree_t * target, const tree_t * refterm, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * un2 = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(un2).sval = "$unify";
    IR_t * rat = lpl_build(cx, IR_CALL, un2, ωfail);
    IR_LIT(rat).sval = "$db_ref_r";
    IR_t * uni = lpl_build(cx, IR_CALL, rat, ωfail);
    IR_LIT(uni).sval = "$clause_unify";
    IR_t * at = lpl_build(cx, IR_CALL, uni, ωfail);
    IR_LIT(at).sval = "$db_at_r";
    IR_t * to = lpl_build(cx, IR_TO, at, ωfail);
    IR_LIT(to).sval = (char *) "ag:trl";
    IR_t * cnt = lpl_build(cx, IR_CALL, to, ωfail);
    IR_LIT(cnt).sval = "$db_n_r";
    IR_t * lo = lpl_build(cx, IR_LIT_INTEGER, cnt, ωfail);
    IR_LIT(lo).ival = 0;
    IR_t * re = NULL;
    IR_t * rv = term_lval_e(cx, refterm, &re);
    IR_t * te = NULL;
    IR_t * tv = term_e(cx, target, &te);
    lc_γ_to(tv, re ? re : rv);
    lc_ω_to(tv, ωfail);
    lc_γ_to(rv, lo);
    lc_ω_to(rv, ωfail);
    ir_operand_push(cnt, tv);
    ir_operand_push(cnt, rv);
    ir_operand_push(to, lo);
    ir_operand_push(to, cnt);
    ir_operand_push(at, tv);
    ir_operand_push(at, rv);
    ir_operand_push(at, to);
    ir_operand_push(uni, at);
    ir_operand_push(uni, tv);
    ir_operand_push(rat, tv);
    ir_operand_push(rat, rv);
    ir_operand_push(rat, to);
    ir_operand_push(un2, rat);
    ir_operand_push(un2, rv);
    lc_ω_to_β(at, to);
    lc_ω_to_β(uni, to);
    lc_ω_to_β(rat, to);
    lc_ω_to_β(un2, to);
    if (entry_out) *entry_out = te ? te : tv;
    return to;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_nb_key(const tree_t * t) { if (!t || !(t->t == TT_QLIT || t->t == TT_NAME) || !t->v.sval) return NULL; return t->v.sval; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_nb_leaf_lv(lcx_t * cx, const char * sym, int k, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = (char *) sym;
    IR_t * kn = lpl_build(cx, IR_LIT_INTEGER, NULL, ωfail);
    IR_LIT(kn).ival = k;
    IR_t * te = NULL;
    IR_t * tv = term_lval_e(cx, arg, &te);
    lc_γ_to(kn, te ? te : tv);
    lc_ω_to(kn, ωfail);
    lc_γ_to(tv, nd);
    lc_ω_to(tv, ωfail);
    ir_operand_push(nd, kn);
    ir_operand_push(nd, tv);
    if (entry_out) *entry_out = kn;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_nb_leaf_lv_tree(lcx_t * cx, const char * sym, const tree_t * keyt, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = (char *) sym;
    IR_t * ke = NULL;
    IR_t * kn = term_e(cx, keyt, &ke);
    IR_t * te = NULL;
    IR_t * tv = term_lval_e(cx, arg, &te);
    lc_γ_to(kn, te ? te : tv);
    lc_ω_to(kn, ωfail);
    lc_γ_to(tv, nd);
    lc_ω_to(tv, ωfail);
    ir_operand_push(nd, kn);
    ir_operand_push(nd, tv);
    if (entry_out) *entry_out = ke ? ke : kn;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_nil_term(void) { return ast_node_new(TT_MAKELIST); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_meta_call_dyn(lcx_t * cx, const tree_t * g, const tree_t * const * extra, int nextra, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL_VALUE, γnext, ωfail);
    IR_LIT(nd).sval = "goal";
    IR_t * ge = NULL;
    IR_t * gv = term_e(cx, g, &ge);
    lc_ω_to(gv, ωfail);
    ir_operand_push(nd, gv);
    IR_t * prev = gv;
    IR_t * first = ge ? ge : gv;
    for (int i = 0; i < nextra; i++) { IR_t * ae = NULL; IR_t * a = term_lval_e(cx, extra[i], &ae); lc_γ_to(prev, ae ? ae : a); lc_ω_to(a, ωfail); prev = a; ir_operand_push(nd, a); }
    lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first;
    return nd;
}
int pl_mc_graph_fell_back(const IR_graph_t * g, const char * nm, int ar) {
    if (!g) return 1;
    for (int i = 0; i < g->n; i++) {
        const IR_t * nd = g->all[i];
        const IR_t * gv;
        if (!nd || nd->op != IR_CALL_VALUE || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval, "goal") || nd->n_operands < 1) continue;
        gv = nd->operands[0];
        if (!gv) continue;
        if (ar == 0 && (gv->op == IR_LIT_STRING || gv->op == IR_LIT_ATOM) && IR_LIT(gv).sval && !strcmp(IR_LIT(gv).sval, nm)) return 1;
        if (ar > 0 && ir_kind0(gv) == IR_CALL && IR_LIT(gv).sval && !strcmp(IR_LIT(gv).sval, "$mkc") && gv->n_operands == ar + 1 && gv->operands[0] && pl_mkc_functor_is(gv->operands[0], nm)) return 1;
        if (ar > 0 && ir_kind0(gv) == IR_CALL && IR_LIT(gv).sval && !strcmp(IR_LIT(gv).sval, "$mkground") && gv->n_operands == 1 && pl_mkground_functor_is(gv->operands[0], nm, ar)) return 1;
    }
    return 0;
}
