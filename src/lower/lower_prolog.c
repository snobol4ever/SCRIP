#include <stdio.h>
#include "ct_arena.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
#include "emit.h"
#include "stage2.h"
#include "../parsers/snobol4/scrip_cc.h"
#include "bb_program.h"
#include "ir_query.h"
#include "pl_arith_names.h"
#include "pl_control_names.h"
#include "ct_vec.h"
typedef struct { const char * name; int arity; int bb_idx; } pl_bb_ent_t;
static cv_t pl_bb_cv;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_SEAL_TAIL 1
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned resolve_pred_hash(const char *s) { unsigned h = 5381; while (*s) h = h * 33 ^ (unsigned char)*s++; return h % STAGE2_PL_PRED_TABLE_SIZE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice) {
    unsigned h = resolve_pred_hash(key);
    Resolve_PredEntry *e = (Resolve_PredEntry *) ct_alloc(sizeof(Resolve_PredEntry));
    e->key = key;
    e->choice = choice;
    e->entry_pc = -1;
    e->next = pt->buckets[h];
    pt->buckets[h] = e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key) {
    for (Resolve_PredEntry *e = pt->buckets[resolve_pred_hash(key)]; e; e = e->next) if (strcmp(e->key, key) == 0) return e->choice;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_bb_ent_t * pl_bb_lookup(const char * name, int arity) {
    if (!name) return NULL;
    for (uint32_t i = 0; i < pl_bb_cv.len; i++) { pl_bb_ent_t * e = &CV_AT(pl_bb_cv, pl_bb_ent_t, i); if (e->arity == arity && e->name && strcmp(e->name, name) == 0) return e; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_bb_ent_t * pl_bb_register(const char * name, int arity, int bb_idx) {
    if (!name) return NULL;
    pl_bb_ent_t * existing = pl_bb_lookup(name, arity);
    if (existing) { existing->bb_idx = bb_idx; return existing; }
    pl_bb_ent_t * e = &CV_PUSH(pl_bb_cv, pl_bb_ent_t);
    e->name = ct_strdup(name);
    e->arity = arity;
    e->bb_idx = bb_idx;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_pi_is_static_builtin(const char * pn, int ar);
void pl_dyn_mark(const char *name, int arity) {
    if (!name) return;
    if (pl_pi_is_static_builtin(name, arity)) return;
    for (int i = 0; i < g_stage2.pl_dyn_n; i++) if (g_stage2.pl_dyn_name[i] && !strcmp(g_stage2.pl_dyn_name[i], name) && g_stage2.pl_dyn_arity[i] == arity) return;
    if (g_stage2.pl_dyn_n >= g_stage2.pl_dyn_cap) {
        int nc = g_stage2.pl_dyn_cap > 0 ? g_stage2.pl_dyn_cap * 2 : 64;
        g_stage2.pl_dyn_name = (const char **) ct_grow((void *) g_stage2.pl_dyn_name, (size_t) nc * sizeof(const char *));
        g_stage2.pl_dyn_arity = (int *) ct_grow((void *) g_stage2.pl_dyn_arity, (size_t) nc * sizeof(int));
        g_stage2.pl_dyn_cap = nc;
    }
    g_stage2.pl_dyn_name[g_stage2.pl_dyn_n] = name;
    g_stage2.pl_dyn_arity[g_stage2.pl_dyn_n] = arity;
    g_stage2.pl_dyn_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_dyn_is_marked(const char *name, int arity) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.pl_dyn_n; i++) if (g_stage2.pl_dyn_name[i] && !strcmp(g_stage2.pl_dyn_name[i], name) && g_stage2.pl_dyn_arity[i] == arity) return 1;
    return 0;
}
static const char ** g_pl_decl_dyn_name;
static int * g_pl_decl_dyn_arity;
static int g_pl_decl_dyn_n = 0;
static int g_pl_decl_dyn_cap = 0;
static void pl_decl_dyn_mark(const char * name, int arity) {
    if (!name) return;
    for (int i = 0; i < g_pl_decl_dyn_n; i++) if (g_pl_decl_dyn_name[i] && !strcmp(g_pl_decl_dyn_name[i], name) && g_pl_decl_dyn_arity[i] == arity) return;
    if (g_pl_decl_dyn_n >= g_pl_decl_dyn_cap) {
        int nc = g_pl_decl_dyn_cap > 0 ? g_pl_decl_dyn_cap * 2 : 64;
        g_pl_decl_dyn_name = (const char **) ct_grow((void *) g_pl_decl_dyn_name, (size_t) nc * sizeof(const char *));
        g_pl_decl_dyn_arity = (int *) ct_grow((void *) g_pl_decl_dyn_arity, (size_t) nc * sizeof(int));
        g_pl_decl_dyn_cap = nc;
    }
    g_pl_decl_dyn_name[g_pl_decl_dyn_n] = ct_strdup(name);
    g_pl_decl_dyn_arity[g_pl_decl_dyn_n] = arity;
    g_pl_decl_dyn_n++;
}
static int pl_decl_dyn_is(const char * name, int arity) {
    if (!name) return 0;
    for (int i = 0; i < g_pl_decl_dyn_n; i++) if (g_pl_decl_dyn_name[i] && !strcmp(g_pl_decl_dyn_name[i], name) && g_pl_decl_dyn_arity[i] == arity) return 1;
    return 0;
}
static const char ** g_pl_decl_other_name;
static int * g_pl_decl_other_arity;
static int g_pl_decl_other_n = 0;
static int g_pl_decl_other_cap = 0;
static void pl_decl_other_record(const tree_t * spec, int mf) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_decl_other_record(spec->c[0], mf); pl_decl_other_record(spec->c[1], mf); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_decl_other_record(spec->c[i], mf); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "/") && spec->n == 2 && spec->c[0] && (spec->c[0]->t == TT_QLIT || spec->c[0]->t == TT_NAME) && spec->c[0]->v.sval && spec->c[1] &&
        spec->c[1]->t == TT_ILIT) {
        if (mf) CV_PUSH(g_stage2.pl_decl_multifile, const tree_t *) = spec;
        if (g_pl_decl_other_n >= g_pl_decl_other_cap) {
            int nc = g_pl_decl_other_cap > 0 ? g_pl_decl_other_cap * 2 : 256;
            g_pl_decl_other_name = (const char **) ct_grow((void *) g_pl_decl_other_name, (size_t) nc * sizeof(const char *));
            g_pl_decl_other_arity = (int *) ct_grow((void *) g_pl_decl_other_arity, (size_t) nc * sizeof(int));
            g_pl_decl_other_cap = nc;
        }
        g_pl_decl_other_name[g_pl_decl_other_n] = spec->c[0]->v.sval;
        g_pl_decl_other_arity[g_pl_decl_other_n] = (int) spec->c[1]->v.ival;
        g_pl_decl_other_n++;
    }
}
static int pl_decl_mf_is(const char * name, int arity) {
    for (uint32_t i = 0; name && i < g_stage2.pl_decl_multifile.len; i++) {
        const tree_t * m = CV_AT(g_stage2.pl_decl_multifile, const tree_t *, i);
        if (m->c[1]->v.ival == arity && !strcmp(m->c[0]->v.sval, name)) return 1;
    }
    return 0;
}
static void pl_decl_meta_record(const tree_t * spec) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_decl_meta_record(spec->c[0]); pl_decl_meta_record(spec->c[1]); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_decl_meta_record(spec->c[i]); return; }
    if (spec->t == TT_FNC && spec->v.sval && spec->n > 0) CV_PUSH(g_stage2.pl_decl_meta, const tree_t *) = spec;
}
static const tree_t * pl_decl_meta_of(const char * name, int arity) {
    for (uint32_t i = 0; name && i < g_stage2.pl_decl_meta.len; i++) { const tree_t * m = CV_AT(g_stage2.pl_decl_meta, const tree_t *, i); if (m->n == arity && !strcmp(m->v.sval, name)) return m; }
    return (const tree_t *) 0;
}
typedef struct {
    IR_graph_t * g;
    IR_t * tω;
    IR_t * cutω;
    IR_t * clause_cutω;
    int cut_scope;
    int scope_seq;
    IR_t * meta_redo;
    int meta_redo_set;
    int stmt_depth;
    int * valias;
    int nvalias;
    const char * cur_key;
} lcx_t;
static const char * pl_vname(const lcx_t * cx, int slot);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(lcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_refuse(const char * what, const char * detail, int rung) {
    fprintf(stderr, "scrip: prolog: %s%s%s is not on the ladder yet -- rung %d lands it (ARCH-PROLOG-BYRD-BOX-TRANSLATION.md sec E; rung 0 is hello world)\n", what, detail ? " " : "",
        detail ? detail : "", rung);
    exit(2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_var_name(int slot) {
    static char * cache[1024];
    static char buf[24];
    if (slot >= 0 && slot < 1024) { if (!cache[slot]) { snprintf(buf, sizeof buf, "G%d", slot); cache[slot] = ct_strdup(buf); } return cache[slot]; }
    snprintf(buf, sizeof buf, "G%d", slot);
    return ct_strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_pi_name(const char * nm, int ar) { if (!nm) return ct_strdup("?"); return ct_fmt("%s/%d", nm, ar); }
static IR_t * term_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
static int pl_tree_is_big(const tree_t * t) { return t && t->t == TT_FNC && t->n == 1 && t->v.sval && !strcmp(t->v.sval, "$pl_big") && t->c[0] && t->c[0]->t == TT_QLIT && t->c[0]->v.sval; }
static IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * mkc_node(lcx_t * cx, const char * fname, int nkids, IR_t ** kids, IR_t ** kid_entries, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, NULL, cx->tω);
    IR_LIT(nd).sval = "$mkc";
    extern int prolog_atom_intern(const char *);
    extern int prolog_functor_intern(int, int);
    IR_t * fn = build(cx, IR_LIT_INTEGER, NULL, cx->tω);
    IR_LIT(fn).ival = (int64_t)prolog_functor_intern(prolog_atom_intern(fname), nkids);
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
static IR_t * term_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!t) return NULL;
    switch (t->t) {
        case TT_QLIT:
        { IR_t * nd = build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval; return nd; }
        case TT_ILIT:
        { IR_t * nd = build(cx, IR_LIT_INTEGER, NULL, cx->tω); IR_LIT(nd).ival = t->v.ival; return nd; }
        case TT_DQLIT:
        { IR_t * nd = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(nd).sval = (t->n > 0 && t->c[0] && t->c[0]->v.sval) ? t->c[0]->v.sval : ""; return nd; }
        case TT_FLIT:
        { IR_t * nd = build(cx, IR_LIT_REAL, NULL, cx->tω); IR_LIT(nd).dval = t->v.dval; return nd; }
        case TT_VAR:
        { IR_t * nd = build(cx, IR_VAR, NULL, cx->tω); IR_LIT(nd).sval = pl_vname(cx, (int) t->v.ival); return nd; }
        case TT_MAKELIST:
        {
            int bar = (t->v.ival == 1 && t->n > 0);
            IR_t * prev;
            IR_t * prev_e = NULL;
            if (bar) prev = term_lval_e(cx, t->c[t->n - 1], &prev_e);
            else { prev = build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(prev).sval = "[]"; }
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
            if (pl_tree_is_big(t)) {
                IR_t * nd = build(cx, IR_CALL, NULL, cx->tω);
                IR_LIT(nd).sval = "$pl_big";
                IR_t * dg = build(cx, IR_LIT_STRING, NULL, cx->tω);
                IR_LIT(dg).sval = t->c[0]->v.sval;
                ir_operand_push(nd, dg);
                lc_γ_to(dg, nd);
                if (entry_out) *entry_out = dg;
                return nd;
            }
            if (nk == 0) { IR_t * nd = build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : "?"; return nd; }
            IR_t ** kids = (IR_t **) ct_zalloc((size_t)(nk > 0 ? nk : 1), sizeof(IR_t *));
            IR_t ** kes = (IR_t **) ct_zalloc((size_t)(nk > 0 ? nk : 1), sizeof(IR_t *));
            for (int i = 0; i < nk; i++) kids[i] = term_lval_e(cx, t->c[i], &kes[i]);
            IR_t * nd = mkc_node(cx, t->v.sval ? t->v.sval : "?", nk, kids, kes, entry_out);
            ct_drop(kids);
            ct_drop(kes);
            return nd;
        }
        case TT_CUT:
        { IR_t * nd = build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(nd).sval = "!"; return nd; }
        default:
        { IR_t * nd = build(cx, IR_LIT_ATOM, NULL, cx->tω); IR_LIT(nd).sval = "?"; return nd; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (t && t->t == TT_VAR) { IR_t * nd = build(cx, IR_VAR_REF, NULL, cx->tω); IR_LIT(nd).sval = pl_vname(cx, (int) t->v.ival); return nd; }
    return term_e(cx, t, entry_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_param_name(int i) {
    static cv_t cache;
    if (i < 0) return ct_fmt("A%d", i);
    cv_reserve(&cache, (uint32_t) sizeof(char *), (uint64_t) i + 1, "pl_param_name");
    if (!CV_AT(cache, char *, i)) CV_AT(cache, char *, i) = ct_fmt("A%d", i);
    return CV_AT(cache, char *, i);
}
static const char * pl_vname(const lcx_t * cx, int slot) { if (cx && slot >= 0 && slot < cx->nvalias && cx->valias[slot]) return pl_param_name(cx->valias[slot] - 1); return pl_var_name(slot); }
static int pl_var_nodes(const tree_t * t) { int n; if (!t) return 0; n = (t->t == TT_VAR); for (int i = 0; i < t->n; i++) n += pl_var_nodes(t->c[i]); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int max_var_slot(const tree_t * t, int mx) {
    if (!t) return mx;
    if (t->t == TT_VAR && (int) t->v.ival > mx) mx = (int) t->v.ival;
    for (int i = 0; i < t->n; i++) mx = max_var_slot(t->c[i], mx);
    return mx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_collect_vars(const tree_t * t, int * slots, int * n, int cap) {
    if (!t || *n >= cap) return;
    if (t->t == TT_VAR) { int sl = (int) t->v.ival; for (int i = 0; i < *n; i++) if (slots[i] == sl) return; if (*n < cap) slots[(*n)++] = sl; return; }
    for (int i = 0; i < t->n; i++) pl_collect_vars(t->c[i], slots, n, cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_free_vars(const tree_t * tmpl, const tree_t * goal, int * out, int cap) {
    int nvn = pl_var_nodes(tmpl) + pl_var_nodes(goal) + 1;
    int bound[nvn], nb = 0, gv[nvn], ng = 0, n = 0;
    const tree_t * g = goal;
    pl_collect_vars(tmpl, bound, &nb, nvn);
    while (g && g->t == TT_FNC && g->v.sval && !strcmp(g->v.sval, "^") && g->n == 2) { pl_collect_vars(g->c[0], bound, &nb, nvn); g = g->c[1]; }
    pl_collect_vars(g, gv, &ng, nvn);
    for (int i = 0; i < ng && n < cap; i++) { int f = 0; for (int j = 0; j < nb; j++) if (bound[j] == gv[i]) { f = 1; break; } if (!f) out[n++] = gv[i]; }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_same_functor(const tree_t * a, const tree_t * b) {
    return a && b && a->t == TT_FNC && b->t == TT_FNC && a->n == b->n && a->n > 0 && a->v.sval && b->v.sval && !strcmp(a->v.sval, b->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_const_lit(lcx_t * cx, const tree_t * t) {
    IR_t * nd = (IR_t *)0;
    if (!t) return nd;
    if (t->t == TT_QLIT || (t->t == TT_FNC && t->n == 0)) {
        nd = build(cx, IR_LIT_ATOM, NULL, NULL);
        IR_LIT(nd).sval = t->v.sval ? t->v.sval : "?";
    } else if (t->t == TT_MAKELIST && t->n == 0) {
        nd = build(cx, IR_LIT_ATOM, NULL, NULL);
        IR_LIT(nd).sval = "[]";
    } else if (t->t == TT_CUT) {
        nd = build(cx, IR_LIT_ATOM, NULL, NULL);
        IR_LIT(nd).sval = "!";
    } else if (t->t == TT_ILIT) {
        nd = build(cx, IR_LIT_INTEGER, NULL, NULL);
        IR_LIT(nd).ival = t->v.ival;
    }
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_unify_node3(lcx_t * cx, IR_e op, IR_t * src, long idx, IR_t * payload, IR_t * γ, IR_t * ω) {
    IR_t * u = build(cx, op, γ, ω);
    IR_t * ix = build(cx, IR_LIT_INTEGER, NULL, NULL);
    IR_LIT(ix).ival = (int64_t) idx;
    ir_operand_push(u, src);
    ir_operand_push(u, ix);
    if (payload) ir_operand_push(u, payload);
    return u;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_unify_const_node(lcx_t * cx, const char * vname, IR_t * klit, IR_t * γ, IR_t * ω) {
    IR_t * v = build(cx, IR_VAR_REF, NULL, NULL);
    IR_LIT(v).sval = vname;
    return pl_unify_node3(cx, IR_UNIFY_CONST, v, 0, klit, γ, ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_var_count(const tree_t * t, int slot) {
    if (!t) return 0;
    if (t->t == TT_VAR) return (int) t->v.ival == slot;
    { int n = 0; for (int i = 0; i < t->n; i++) n += pl_var_count(t->c[i], slot); return n; }
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
static int pl_head_boxable(const tree_t * t) {
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
        if (pl_tree_is_big(t)) return 0;
        for (int i = 0; i < t->n; i++) if (!pl_head_boxable(t->c[i])) return 0;
        return 1;
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_head_term(lcx_t * cx, const tree_t * cl, int ar, IR_t * src, long idx, const tree_t * t, IR_t * next, IR_t * step);
static IR_t * pl_head_list(lcx_t * cx, const tree_t * cl, int ar, IR_t * src, long idx, const tree_t * t, int i, IR_t * next, IR_t * step) {
    int bar = (t->v.ival == 1 && t->n > 0);
    int last = bar ? t->n - 1 : t->n;
    if (i >= last) {
        if (bar) return pl_head_term(cx, cl, ar, src, idx, t->c[t->n - 1], next, step);
        { IR_t * nil = build(cx, IR_LIT_ATOM, NULL, NULL); IR_LIT(nil).sval = "[]"; return pl_unify_node3(cx, IR_UNIFY_CONST, src, idx, nil, next, step); }
    }
    {
        extern int prolog_atom_intern(const char *);
        extern int prolog_functor_intern(int, int);
        IR_t * st = build(cx, IR_UNIFY_STRUCT, NULL, step);
        IR_t * nx = pl_head_list(cx, cl, ar, st, 1, t, i + 1, next, step);
        nx = pl_head_term(cx, cl, ar, st, 0, t->c[i], nx, step);
        lc_γ_to(st, nx);
        {
            IR_t * f = build(cx, IR_LIT_INTEGER, NULL, NULL);
            IR_LIT(f).ival = (int64_t) prolog_functor_intern(prolog_atom_intern("."), 2);
            IR_t * ix = build(cx, IR_LIT_INTEGER, NULL, NULL);
            IR_LIT(ix).ival = (int64_t) idx;
            ir_operand_push(st, src);
            ir_operand_push(st, ix);
            ir_operand_push(st, f);
        }
        return st;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_head_term(lcx_t * cx, const tree_t * cl, int ar, IR_t * src, long idx, const tree_t * t, IR_t * next, IR_t * step) {
    if (t->t == TT_VAR) {
        int sl = (int) t->v.ival;
        if (sl < 0) return next;
        { int n = 0; for (int i = 0; i < cl->n; i++) n += pl_var_count(cl->c[i], sl); if (n <= 1) return next; }
        { IR_t * u = pl_unify_node3(cx, pl_head_is_first(cl, ar, t, sl) ? IR_UNIFY_FIRST : IR_UNIFY_VALUE, src, idx, NULL, next, step); IR_LIT(u).sval = pl_vname(cx, sl); return u; }
    }
    { IR_t * klit = pl_const_lit(cx, t); if (klit) return pl_unify_node3(cx, IR_UNIFY_CONST, src, idx, klit, next, step); }
    if (t->t == TT_MAKELIST) return pl_head_list(cx, cl, ar, src, idx, t, 0, next, step);
    if (t->t == TT_FNC && t->n > 0) {
        extern int prolog_atom_intern(const char *);
        extern int prolog_functor_intern(int, int);
        IR_t * st = build(cx, IR_UNIFY_STRUCT, NULL, step);
        IR_t * nx = next;
        for (int j = t->n - 1; j >= 0; j--) nx = pl_head_term(cx, cl, ar, st, j, t->c[j], nx, step);
        lc_γ_to(st, nx);
        {
            IR_t * f = build(cx, IR_LIT_INTEGER, NULL, NULL);
            IR_LIT(f).ival = (int64_t) prolog_functor_intern(prolog_atom_intern(t->v.sval ? t->v.sval : "?"), t->n);
            IR_t * ix = build(cx, IR_LIT_INTEGER, NULL, NULL);
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
static IR_t * unify_pair(lcx_t * cx, const tree_t * lt, const tree_t * rt, IR_t * γ, IR_t * ω, IR_t ** entry_out) {
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
    IR_t * nd = build(cx, IR_CALL, γ, ω);
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
static const char * pl_rung6_builtins[] = { "=..", "==", "@<", "@=<", "@>", "@>=", "\\==", "acyclic_term", "arg", "atom", "atom_chars", "atom_codes", "atom_concat", "atom_length", "atom_number",
    "atom_string", "atomic", "atomic_concat", "atomic_list_concat", "callable", "char_type", "compound", "concat_atom", "copy_term", "downcase_atom", "float", "format", "functor", "ground", "integer",
    "is_list", "msort", "name", "nonvar", "number", "number_chars", "number_codes", "number_string", "numbervars", "plus", "print", "sort", "string", "string_chars", "string_codes", "string_concat",
    "string_length", "string_lower", "string_to_atom", "string_upper", "succ", "tab", "term_string", "term_to_atom", "term_variables", "upcase_atom", "var", "write_canonical", "writeln", "writeq",
    "put_char", "halt", "flush_output", "read", "read_term", "get_char", "peek_char", "nl", "write", NULL };
static const char * pl_rung7_builtins[] = { "between", "repeat", "sub_atom", "for", "current_op", "current_predicate", "predicate_property", "current_prolog_flag", "current_stream", "stream_property",
    NULL };
static const char * pl_rung8_builtins[] = { "findall", "bagof", "setof", "aggregate_all", NULL };
static const char * pl_rung9_builtins[] = { NULL };
static const char * pl_rung10_builtins[] = { "call", "assert", "asserta", "assertz", "retractall", "abolish", "clause", "retract", "dynamic", "b_setval", "b_getval", "phrase", "with_output_to",
    "setup_call_cleanup", "use_module", "ensure_loaded", "module", "set_prolog_flag", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_name_in(const char * nm, const char * const * lst) { if (!nm) return 0; for (int i = 0; lst[i]; i++) if (!strcmp(nm, lst[i])) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_rung_of(const char * nm) {
    if (pl_name_in(nm, pl_rung10_builtins)) return 10;
    if (pl_name_in(nm, pl_rung9_builtins)) return 9;
    if (pl_name_in(nm, pl_rung8_builtins)) return 8;
    if (pl_name_in(nm, pl_rung7_builtins)) return 7;
    if (pl_name_in(nm, pl_rung6_builtins)) return 6;
    return 0;
}
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
static int pl_tree_is_nil(const tree_t * t) { if (!t) return 0; if (t->t == TT_MAKELIST) return t->n == 0; return (t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval && !strcmp(t->v.sval, "[]"); }
static const char * pl_decl_directives[] = { "multifile", "discontiguous", "ensure_loaded", "use_module", "module", "meta_predicate", "dynamic", "table", NULL };
static void pl_decl_dynamic_record(stage2_t * s2, tree_t * spec, tree_t * marker) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_decl_dynamic_record(s2, spec->c[0], marker); pl_decl_dynamic_record(s2, spec->c[1], marker); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_decl_dynamic_record(s2, spec->c[i], marker); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "/") && spec->n == 2 && spec->c[0] && (spec->c[0]->t == TT_QLIT || spec->c[0]->t == TT_NAME) && spec->c[0]->v.sval && spec->c[1] &&
        spec->c[1]->t == TT_ILIT) {
        char key[fmt_len("%s/%d", spec->c[0]->v.sval, (int) spec->c[1]->v.ival)];
        snprintf(key, sizeof key, "%s/%d", spec->c[0]->v.sval, (int) spec->c[1]->v.ival);
        pl_decl_dyn_mark(spec->c[0]->v.sval, (int) spec->c[1]->v.ival);
        if (!resolve_pred_table_lookup(&s2->resolve_pred_table, key)) resolve_pred_table_insert(&s2->resolve_pred_table, ct_strdup(key), marker);
        return;
    }
    pl_refuse("dynamic declaration shape", spec->v.sval ? spec->v.sval : "?", 10);
}
static int pl_flag_directive_is_advisory(const tree_t * subj) {
    static const char * const advisory[] = { "optimise", "last_call_optimisation", "generate_debug_info", "agc_margin", "stack_limit", "xref", "verbose", "report_error", "autoload", NULL };
    const tree_t * f = subj->c[0];
    if (!f || !(f->t == TT_QLIT || f->t == TT_NAME) || !f->v.sval) return 0;
    for (int i = 0; advisory[i]; i++) if (!strcmp(f->v.sval, advisory[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_cmp_op_suffix(const char * s) {
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
static const char * pl_ax_suffix(const char * s, int ar) { return pl_ax_suffix_of(s, ar); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ax_divides(const char * sfx) { if (!sfx) return 0; return !strcmp(sfx, "div") || !strcmp(sfx, "idiv") || !strcmp(sfx, "divf") || !strcmp(sfx, "mod") || !strcmp(sfx, "rem"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_arith_val(lcx_t * cx, const tree_t * t, IR_t * ωfail, IR_t ** entry_out) {
    if (pl_tree_is_big(t)) {
        IR_t * nd = build(cx, IR_CALL, NULL, ωfail);
        IR_LIT(nd).sval = "$pl_big";
        IR_t * dg = build(cx, IR_LIT_STRING, NULL, ωfail);
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
        IR_t * nd = build(cx, IR_CALL, NULL, ωfail);
        IR_LIT(nd).sval = ct_strdup(nb);
        if (entry_out) *entry_out = nd;
        return nd;
    }
    if (t && t->t == TT_FNC && t->v.sval && (t->n == 1 || t->n == 2) && pl_ax_suffix(t->v.sval, t->n)) {
        char nb[fmt_len("$ax_%s", pl_ax_suffix(t->v.sval, t->n))];
        snprintf(nb, sizeof nb, "$ax_%s", pl_ax_suffix(t->v.sval, t->n));
        IR_t * nd = build(cx, IR_CALL, NULL, ωfail);
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
            IR_t * zg = build(cx, IR_CALL, nd, ωfail);
            IR_LIT(zg).sval = "$ax_zguard";
            IR_t * opn = build(cx, IR_LIT_STRING, zg, ωfail);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_conj(const tree_t * t, lc_vec * out) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ",")) { for (int i = 0; i < t->n; i++) collect_conj(t->c[i], out); return; }
    if (t->t == TT_PROGRAM) { for (int i = 0; i < t->n; i++) collect_conj(t->c[i], out); return; }
    lc_vec_push(out, &t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_lower_conj(lcx_t * cx, const tree_t * const * gl, int ng, IR_t * γtail, IR_t * ωbase, IR_t ** entry_out, IR_t ** redo_out, IR_t ** tail_out) {
    IR_t ** gn = (IR_t **) ct_zalloc((size_t)(ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t ** en = (IR_t **) ct_zalloc((size_t)(ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t ** rd = (IR_t **) ct_zalloc((size_t)(ng > 0 ? ng : 1), sizeof(IR_t *));
    int * rds = (int *) ct_zalloc((size_t)(ng > 0 ? ng : 1), sizeof(int));
    IR_t * next = γtail;
    for (int i = ng - 1; i >= 0; i--) {
        IR_t * e = NULL;
        cx->meta_redo_set = 0;
        IR_t * nd = goal(cx, gl[i], next, ωbase, &e);
        rd[i] = cx->meta_redo_set ? cx->meta_redo : NULL;
        rds[i] = cx->meta_redo_set;
        cx->meta_redo_set = 0;
        gn[i] = nd;
        en[i] = e ? e : nd;
        next = en[i];
    }
    IR_t * last_res = ωbase;
    int last_res_beta = 0;
    for (int i = 0; i < ng; i++) {
        if (gn[i] && gn[i]->op == IR_CUT && IR_LIT(gn[i]).ival == cx->cut_scope) { lc_ω_to(gn[i], cx->cutω); last_res = cx->cutω; last_res_beta = 0; continue; }
        if (gn[i] && gn[i]->op == IR_GOTO) {
            if (last_res_beta) lc_γ_to_β(gn[i], last_res);
            else lc_γ_to(gn[i], last_res);
            if (rds[i] && rd[i]) { last_res = rd[i]; last_res_beta = 1; }
        } else if (last_res_beta) lc_ω_to_β(gn[i], last_res);
        else lc_ω_to(gn[i], last_res);
        if (gn[i] && (gn[i]->op == IR_CALL_PROC_STAGED || gn[i]->op == IR_DISJUNCTION || gn[i]->op == IR_GATE || ir_is_generator_kind(gn[i]->op))) { last_res = gn[i]; last_res_beta = 1; }
    }
    if (redo_out) *redo_out = last_res_beta ? last_res : NULL;
    if (entry_out) *entry_out = (ng > 0) ? en[0] : γtail;
    if (tail_out) *tail_out = (ng > 0) ? gn[ng - 1] : NULL;
    IR_t * first = (ng > 0) ? gn[0] : NULL;
    ct_drop(gn);
    ct_drop(en);
    ct_drop(rd);
    ct_drop(rds);
    return first;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_has_caret(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "^")) return 1;
    for (int i = 0; i < t->n; i++) if (pl_tree_has_caret(t->c[i])) return 1;
    return 0;
}
static const tree_t * pl_caret_body(const tree_t * t) { int guard = 0; while (t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "^") && t->n == 2 && guard++ < 256) t = t->c[1]; return t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_is_control(const tree_t * t) {
    return t && t->t == TT_FNC && t->v.sval && t->n == 2 && (!strcmp(t->v.sval, ",") || !strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|") || !strcmp(t->v.sval, "->") || !strcmp(t->v.sval, "*->"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_noncallable_goal(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_ILIT || t->t == TT_FLIT) return 1;
    if (pl_tree_is_control(t)) { for (int i = 0; i < t->n; i++) if (pl_tree_noncallable_goal(t->c[i])) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_goal_arg_wants_guard(const tree_t * t) { return t && (t->t == TT_VAR || pl_tree_noncallable_goal(t)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_control_var_leaf(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_VAR) return 1;
    if (pl_tree_is_control(t)) { for (int i = 0; i < t->n; i++) if (pl_tree_control_var_leaf(t->c[i])) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_ite(const tree_t * t) {
    return t && t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|")) && t->n == 2 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval &&
        !strcmp(t->c[0]->v.sval, "->");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_softcut(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "*->") && t->n == 2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_scite(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|")) && t->n == 2 && pl_is_softcut(t->c[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_disj(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|")) && t->n == 2 && !pl_is_ite(t) && !pl_is_scite(t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_disj(const tree_t * t, lc_vec * out) { if (pl_is_disj(t)) { collect_disj(t->c[0], out); collect_disj(t->c[1], out); return; } lc_vec_push(out, &t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_disj_entry(lcx_t * cx, IR_t * e, IR_t * dj) {
    int guard = 0;
    while (e && e->op == IR_SUCCEED && e->γ.node && e->γ.node != dj && guard++ < 4096) e = e->γ.node;
    if (e && e->op != IR_SUCCEED) return e;
    { IR_t * g = build(cx, IR_GOTO, dj, dj); memcpy(g->γ.sz, "σ", 3); g->γ.sz[3] = 0; memcpy(g->ω.sz, "φ", 3); g->ω.sz[3] = 0; return g; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_conj_top_cut(const lc_vec * v) {
    for (int i = 0; i < v->n; i++) {
        const tree_t * g = ((const tree_t * const *) v->data)[i];
        if (g && (g->t == TT_CUT || ((g->t == TT_QLIT || g->t == TT_NAME) && g->v.sval && !strcmp(g->v.sval, "!")))) return 1;
    }
    return 0;
}
static IR_t * pl_cut_redo(lcx_t * cx) { return build(cx, IR_GOTO, cx->cutω, cx->cutω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_lower_disj(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    lc_vec bv;
    lc_vec_init(&bv, (int) sizeof(const tree_t *));
    collect_disj(t, &bv);
    const tree_t * const * br = (const tree_t * const *) bv.data;
    int nb = bv.n;
    if (nb > 32) pl_refuse("disjunction wider than 32 branches", ";", 3);
    IR_t * dj = build(cx, IR_DISJUNCTION, γnext, ωfail);
    for (int j = 0; j < nb; j++) {
        int before = cx->g->n;
        lc_vec glv;
        lc_vec_init(&glv, (int) sizeof(const tree_t *));
        collect_conj(br[j], &glv);
        IR_t * bentry = NULL;
        IR_t * redo = NULL;
        IR_t * first = pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, dj, dj, &bentry, &redo, NULL);
        for (int k = before; k < cx->g->n; k++) {
            IR_t * x = cx->g->all[k];
            if (!x) continue;
            if (x->γ.node == dj) { if (x->op == IR_GOTO && x->ω.node == dj) memcpy(x->γ.sz, "φ", 3); else memcpy(x->γ.sz, "σ", 3); x->γ.sz[3] = 0; }
            if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
        }
        ir_operand_push(dj, pl_disj_entry(cx, bentry ? bentry : (first ? first : dj), dj));
        ir_operand_push(dj, redo ? redo : (pl_conj_top_cut(&glv) ? pl_cut_redo(cx) : dj));
    }
    for (int j = 0; j < nb; j++) ir_operand_push(dj, NULL);
    IR_LIT(dj).ival = (long) nb;
    if (entry_out) *entry_out = dj;
    return dj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_mark_into(lcx_t * cx, int before, IR_t * tgt, const char * gsz, const char * osz) {
    for (int k = before; k < cx->g->n; k++) {
        IR_t * x = cx->g->all[k];
        if (!x) continue;
        if (gsz && x->γ.node == tgt) { memcpy(x->γ.sz, gsz, strlen(gsz) + 1); }
        if (osz && x->ω.node == tgt) { memcpy(x->ω.sz, osz, strlen(osz) + 1); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_atom_goal(const char * nm) { tree_t * n = ast_node_new(TT_QLIT); n->v.sval = (char *) nm; return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_cc_fnc1(const char * f, tree_t * a0) { tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) f; ast_push(n, a0); return n; }
static tree_t * pl_cc_fnc2(const char * f, tree_t * a0, tree_t * a1) { tree_t * n = ast_node_new(TT_FNC); n->v.sval = (char *) f; ast_push(n, a0); ast_push(n, a1); return n; }
static tree_t * pl_meta_var(const char * nm) { tree_t * v = ast_node_new(TT_VAR); v->v.sval = (char *) nm; return v; }
static tree_t * pl_cc_ilit(long long v) { tree_t * n = ast_node_new(TT_ILIT); n->v.ival = v; return n; }
static tree_t * pl_cc_ite(tree_t * c, tree_t * th, tree_t * el) { return pl_cc_fnc2(";", pl_cc_fnc2("->", c, th), el); }
static tree_t * pl_cc_scite(tree_t * c, tree_t * th, tree_t * el) { return pl_cc_fnc2(";", pl_cc_fnc2("*->", c, th), el); }
static tree_t * pl_cc_pi(const char * nm) { return pl_cc_fnc2("/", (tree_t *) pl_atom_goal(nm), pl_cc_ilit(2)); }
static tree_t * pl_cc_throw(tree_t * formal, const char * ctx_nm) { return pl_cc_fnc1("throw", pl_cc_fnc2("error", formal, pl_cc_pi(ctx_nm))); }
static tree_t * pl_cc_fnc3(const char * f, tree_t * a0, tree_t * a1, tree_t * a2) {
    tree_t * n = ast_node_new(TT_FNC);
    n->v.sval = (char *) f;
    ast_push(n, a0);
    ast_push(n, a1);
    ast_push(n, a2);
    return n;
}
static tree_t * pl_cc_pi_ar(const char * nm, int ar) { return pl_cc_fnc2("/", (tree_t *) pl_atom_goal(nm), pl_cc_ilit(ar)); }
static tree_t * pl_cc_perm_static(const char * nm, int ar) {
    return pl_cc_fnc1("throw",
        pl_cc_fnc2("error", pl_cc_fnc3("permission_error", (tree_t *) pl_atom_goal("modify"), (tree_t *) pl_atom_goal("static_procedure"), pl_cc_pi_ar(nm, ar)), pl_cc_pi_ar(nm, ar)));
}
static tree_t * pl_cc_ischar(tree_t * x) { return pl_cc_fnc2(",", pl_cc_fnc1("atom", x), pl_cc_fnc2("atom_length", x, pl_cc_ilit(1))); }
static tree_t * pl_cc_fnc4(const char * f, tree_t * a0, tree_t * a1, tree_t * a2, tree_t * a3) {
    tree_t * n = ast_node_new(TT_FNC);
    n->v.sval = (char *) f;
    ast_push(n, a0);
    ast_push(n, a1);
    ast_push(n, a2);
    ast_push(n, a3);
    return n;
}
static int g_pl_fresh_next = 900000;
static tree_t * pl_cc_freshvar(void) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = g_pl_fresh_next++; return v; }
static tree_t * pl_cc_gen2(const char * count_leaf, const char * nth_leaf, tree_t * a0, tree_t * a1, tree_t * a2) {
    tree_t * cv = pl_cc_freshvar();
    tree_t * cv2 = pl_cc_freshvar();
    tree_t * iv = pl_cc_freshvar();
    tree_t * iv2 = pl_cc_freshvar();
    cv2->v.ival = cv->v.ival;
    iv2->v.ival = iv->v.ival;
    tree_t * gen = pl_cc_fnc2(",", pl_cc_fnc1(count_leaf, cv), pl_cc_fnc3("between", pl_cc_ilit(1), cv2, iv));
    tree_t * pick = a2 ? pl_cc_fnc4(nth_leaf, iv2, a0, a1, a2) : pl_cc_fnc3(nth_leaf, iv2, a0, a1);
    return pl_cc_fnc2(",", gen, pick);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_cc_print_via_format(tree_t * stream, tree_t * term) {
    tree_t * l = ast_node_new(TT_MAKELIST);
    l->v.ival = 0;
    ast_push(l, term);
    return stream ? pl_cc_fnc3("format", stream, (tree_t *) pl_atom_goal("~p"), l) : pl_cc_fnc2("format", (tree_t *) pl_atom_goal("~p"), l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_opts_are_portray_only(const tree_t * l) {
    int portray = 0;
    if (!l || l->t != TT_MAKELIST || l->v.ival == 1) return 0;
    for (int i = 0; i < l->n; i++) {
        const tree_t * o = l->c[i];
        const tree_t * a = (o && o->t == TT_FNC && o->n == 1) ? o->c[0] : (const tree_t *) 0;
        if (!a || (a->t != TT_QLIT && a->t != TT_NAME) || !a->v.sval || strcmp(a->v.sval, "true")) return 0;
        if (!strcmp(o->v.sval, "portray") || !strcmp(o->v.sval, "portrayed")) portray = 1;
        else if (strcmp(o->v.sval, "numbervars")) return 0;
    }
    return portray;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_cc_gen2_desc(const char * count_leaf, const char * nth_leaf, tree_t * a0, tree_t * a1, tree_t * a2) {
    tree_t * cv = pl_cc_freshvar();
    tree_t * cv2 = pl_cc_freshvar();
    tree_t * cv3 = pl_cc_freshvar();
    tree_t * jv = pl_cc_freshvar();
    tree_t * jv2 = pl_cc_freshvar();
    tree_t * iv = pl_cc_freshvar();
    tree_t * iv2 = pl_cc_freshvar();
    cv2->v.ival = cv->v.ival;
    cv3->v.ival = cv->v.ival;
    jv2->v.ival = jv->v.ival;
    iv2->v.ival = iv->v.ival;
    tree_t * down = pl_cc_fnc2("is", iv, pl_cc_fnc2("-", pl_cc_fnc2("+", cv3, pl_cc_ilit(1)), jv2));
    tree_t * gen = pl_cc_fnc2(",", pl_cc_fnc1(count_leaf, cv), pl_cc_fnc2(",", pl_cc_fnc3("between", pl_cc_ilit(1), cv2, jv), down));
    return pl_cc_fnc2(",", gen, pl_cc_fnc4(nth_leaf, iv2, a0, a1, a2));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_pl_fence_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PL_FENCE"); v = (e && *e == (char) 48) ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_fence_on(void) { return emit_pl_fence_on(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_lower_ite(lcx_t * cx, const tree_t * C, const tree_t * T, const tree_t * E, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * ig = build(cx, IR_GATE, γnext, ωfail);
    IR_t * mark = build(cx, IR_BOUND, NULL, ig);
    IR_t * unmk_c = build(cx, IR_UNMARK, ig, ωfail);
    ir_operand_push(unmk_c, mark);
    IR_LIT(unmk_c).ival = pl_fence_on() ? 5 : 1;
    IR_t * unmk_f = build(cx, IR_UNMARK, ig, ωfail);
    ir_operand_push(unmk_f, mark);
    IR_LIT(unmk_f).ival = pl_fence_on() ? 5 : 1;
    IR_t * arm_entry[2] = { NULL, NULL };
    const tree_t * arms[2];
    int nb = 0;
    arms[nb++] = T;
    if (E) arms[nb++] = E;
    for (int j = 0; j < nb; j++) {
        IR_t * ml = build(cx, IR_GATE_ARM, NULL, ωfail);
        lc_vec av;
        lc_vec_init(&av, (int) sizeof(const tree_t *));
        collect_conj(arms[j], &av);
        IR_t * ae = NULL;
        IR_t * redo = NULL;
        IR_t * first = pl_lower_conj(cx, (const tree_t * const *) av.data, av.n, ml, unmk_c, &ae, &redo, NULL);
        IR_t * rt = redo ? redo : (pl_conj_top_cut(&av) ? pl_cut_redo(cx) : ig);
        ir_operand_push(ml, rt);
        ir_operand_push(ml, ig);
        ir_operand_push(ig, ml);
        IR_LIT(ig).ival = ig->n_operands;
        ml->seal = (rt != ig) ? 1 : 0;
        IR_LIT(ml).ival = ig->n_operands - 1;
        arm_entry[j] = ae ? ae : (first ? first : ml);
    }
    {
        int before = cx->g->n;
        lc_vec cv;
        lc_vec_init(&cv, (int) sizeof(const tree_t *));
        collect_conj(C, &cv);
        IR_t * ce = NULL;
        lc_γ_to(unmk_f, (nb > 1) ? arm_entry[1] : ig);
        IR_t * saveω = cx->cutω;
        IR_t * ccut = build(cx, IR_GOTO, unmk_f, unmk_f);
        cx->cutω = ccut;
        IR_t * savecutω = cx->cutω;
        cx->cutω = ccut;
        int save_scope = cx->cut_scope;
        cx->cut_scope = ++cx->scope_seq;
        IR_t * fence = pl_fence_on() ? build(cx, IR_UNMARK, arm_entry[0], unmk_c) : (IR_t *) 0;
        if (fence) { ir_operand_push(fence, mark); IR_LIT(fence).ival = 2; }
        IR_t * cfirst = pl_lower_conj(cx, (const tree_t * const *) cv.data, cv.n, fence ? fence : arm_entry[0], unmk_f, &ce, NULL, NULL);
        cx->cutω = savecutω;
        cx->cut_scope = save_scope;
        cx->cutω = saveω;
        lc_γ_to(mark, ce ? ce : (cfirst ? cfirst : arm_entry[0]));
        if (entry_out) *entry_out = mark;
    }
    return ig;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_sc_entry(IR_t * e) { int guard = 0; while (e && e->op == IR_SUCCEED && e->γ.node && guard++ < 4096) e = e->γ.node; return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_lower_softcut(lcx_t * cx, const tree_t * C, const tree_t * T, const tree_t * E, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * ig = build(cx, IR_GATE, γnext, ωfail);
    IR_t * eg = build(cx, IR_GATE, γnext, ig);
    IR_t * mark = build(cx, IR_BOUND, NULL, ig);
    IR_t * unmk_f = build(cx, IR_UNMARK, NULL, ig);
    ir_operand_push(unmk_f, mark);
    IR_LIT(unmk_f).ival = 1;
    IR_t * unmk_c = build(cx, IR_UNMARK, ig, ig);
    ir_operand_push(unmk_c, mark);
    IR_LIT(unmk_c).ival = 1;
    lc_γ_to_β(unmk_f, eg);
    IR_t * ml_e = build(cx, IR_GATE_ARM, NULL, ig);
    IR_t * ml_c = build(cx, IR_GATE_ARM, NULL, ig);
    IR_t * ml_t = build(cx, IR_GATE_ARM, NULL, ig);
    IR_t * credo = NULL;
    {
        lc_vec cv;
        lc_vec_init(&cv, (int) sizeof(const tree_t *));
        collect_conj(C, &cv);
        IR_t * ce = NULL;
        IR_t * savecutω = cx->cutω;
        cx->cutω = build(cx, IR_GOTO, unmk_f, unmk_f);
        int save_scope = cx->cut_scope;
        cx->cut_scope = ++cx->scope_seq;
        IR_t * cfirst = pl_lower_conj(cx, (const tree_t * const *) cv.data, cv.n, ml_c, unmk_f, &ce, &credo, NULL);
        cx->cutω = savecutω;
        cx->cut_scope = save_scope;
        lc_γ_to(ml_e, pl_sc_entry(ce ? ce : (cfirst ? cfirst : ml_c)));
    }
    IR_t * tredo = NULL;
    int sc_cut_in_then = 0;
    int sc_cut_in_else = 0;
    IR_t * sc_then_cut = NULL;
    IR_t * sc_else_cut = NULL;
    {
        lc_vec tv;
        lc_vec_init(&tv, (int) sizeof(const tree_t *));
        collect_conj(T, &tv);
        int before = cx->g->n;
        IR_t * te = NULL;
        IR_t * tfirst = pl_lower_conj(cx, (const tree_t * const *) tv.data, tv.n, ml_t, credo ? credo : unmk_c, &te, &tredo, NULL);
        sc_cut_in_then = !tredo && pl_conj_top_cut(&tv);
        if (sc_cut_in_then) sc_then_cut = pl_cut_redo(cx);
        if (credo) pl_mark_into(cx, before, credo, "β", "β");
        lc_γ_to(ml_c, pl_sc_entry(te ? te : (tfirst ? tfirst : ml_t)));
    }
    IR_t * ee = NULL;
    if (E) {
        IR_t * ml_ee = build(cx, IR_GATE_ARM, NULL, ig);
        lc_vec ev;
        lc_vec_init(&ev, (int) sizeof(const tree_t *));
        collect_conj(E, &ev);
        IR_t * eredo = NULL;
        IR_t * efirst = pl_lower_conj(cx, (const tree_t * const *) ev.data, ev.n, ml_ee, unmk_c, &ee, &eredo, NULL);
        sc_cut_in_else = !eredo && pl_conj_top_cut(&ev);
        if (sc_cut_in_else) sc_else_cut = pl_cut_redo(cx);
        ir_operand_push(ml_ee, sc_cut_in_else ? sc_else_cut : (eredo ? eredo : ig));
        ir_operand_push(ml_ee, ig);
        ir_operand_push(ig, ml_ee);
        IR_LIT(ig).ival = ig->n_operands;
        ml_ee->seal = (eredo || sc_cut_in_else) ? 1 : 0;
        IR_LIT(ml_ee).ival = ig->n_operands - 1;
        ee = pl_sc_entry(ee ? ee : (efirst ? efirst : ml_ee));
        if (!ee) ee = ml_ee;
    }
    ir_operand_push(ml_e, E ? ee : eg);
    ir_operand_push(ml_e, eg);
    ir_operand_push(eg, ml_e);
    IR_LIT(eg).ival = eg->n_operands;
    ml_e->seal = 0;
    IR_LIT(ml_e).ival = eg->n_operands - 1;
    ir_operand_push(ml_c, eg);
    ir_operand_push(ml_c, eg);
    ir_operand_push(eg, ml_c);
    IR_LIT(eg).ival = eg->n_operands;
    ml_c->seal = 0;
    IR_LIT(ml_c).ival = eg->n_operands - 1;
    ir_operand_push(ml_t, sc_cut_in_then ? sc_then_cut : (tredo ? tredo : (credo ? credo : ig)));
    ir_operand_push(ml_t, ig);
    ir_operand_push(ig, ml_t);
    IR_LIT(ig).ival = ig->n_operands;
    ml_t->seal = (tredo || credo || sc_cut_in_then) ? 1 : 0;
    IR_LIT(ml_t).ival = ig->n_operands - 1;
    lc_γ_to(mark, ml_e);
    if (entry_out) *entry_out = mark;
    return ig;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf_lv(lcx_t * cx, const char * sym, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
static const tree_t * pl_tree_number(const tree_t * t);
static tree_t * pl_cc_throw_ar(tree_t * formal, const char * nm, int ar);
static tree_t * pl_cc_type_error(const char * type, const tree_t * culprit, const char * nm, int ar);
static IR_t * pl_lower_scc(lcx_t * cx, const tree_t * S, const tree_t * G, const tree_t * C, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    tree_t * ball = pl_meta_var("$SccBall");
    tree_t * quiet = pl_cc_fnc2(",", pl_cc_fnc3("catch", pl_cc_fnc1("once", (tree_t *) C), pl_meta_var("$SccIgn"), (tree_t *) pl_atom_goal("true")), pl_cc_fnc1("throw", ball));
    tree_t * ran =
        pl_cc_ite(pl_cc_fnc3("catch", (tree_t *) G, ball, quiet), pl_cc_ite(pl_cc_fnc1("once", (tree_t *) C), (tree_t *) pl_atom_goal("true"), (tree_t *) pl_atom_goal("true")),
        pl_cc_fnc2(",", pl_cc_fnc1("once", (tree_t *) C), (tree_t *) pl_atom_goal("fail")));
    if (pl_tree_number(C)) return goal(cx, pl_cc_type_error("callable", C, "setup_call_cleanup", 3), γnext, ωfail, entry_out);
    if (C && C->t == TT_VAR) ran =
        pl_cc_fnc2(",", pl_cc_ite(pl_cc_fnc1("var", (tree_t *) C), pl_cc_throw_ar((tree_t *) pl_atom_goal("instantiation_error"), "setup_call_cleanup", 3), (tree_t *) pl_atom_goal("true")), ran);
    return goal(cx, pl_cc_fnc2(",", pl_cc_fnc1("once", (tree_t *) S), ran), γnext, ωfail, entry_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_lower_catch(lcx_t * cx, const tree_t * G, const tree_t * C, const tree_t * R, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * ig = build(cx, IR_GATE, γnext, ωfail);
    IR_t * mark = build(cx, IR_BOUND, NULL, ig);
    IR_t * unmk = build(cx, IR_UNMARK, NULL, ig);
    ir_operand_push(unmk, mark);
    tree_t * ct = ast_node_new(TT_FNC);
    ct->v.sval = (char *) "$catch_handle";
    ast_push(ct, (tree_t *) C);
    IR_t * he = NULL;
    IR_t * hc = pl_leaf_lv(cx, "$catch_handle", ct, 1, NULL, ig, &he);
    lc_γ_to(unmk, he ? he : hc);
    IR_t * arm_entry[2] = { NULL, NULL };
    const tree_t * arms[2] = { G, R };
    for (int j = 0; j < 2; j++) {
        IR_t * ml = build(cx, IR_GATE_ARM, NULL, ig);
        IR_t * saveω = cx->cutω;
        int save_scope = cx->cut_scope;
        cx->cutω = build(cx, IR_GOTO, ig, ig);
        cx->cut_scope = ++cx->scope_seq;
        lc_vec av;
        lc_vec_init(&av, (int) sizeof(const tree_t *));
        collect_conj(arms[j], &av);
        IR_t * ae = NULL;
        IR_t * redo = NULL;
        IR_t * first = pl_lower_conj(cx, (const tree_t * const *) av.data, av.n, ml, j ? ig : unmk, &ae, &redo, NULL);
        cx->cutω = saveω;
        cx->cut_scope = save_scope;
        ir_operand_push(ml, redo ? redo : ig);
        ir_operand_push(ml, ig);
        ir_operand_push(ig, ml);
        IR_LIT(ig).ival = ig->n_operands;
        ml->seal = redo ? 1 : 0;
        IR_LIT(ml).ival = ig->n_operands - 1;
        arm_entry[j] = ae ? ae : (first ? first : ml);
    }
    lc_γ_to(hc, arm_entry[1]);
    lc_γ_to(mark, arm_entry[0]);
    if (entry_out) *entry_out = mark;
    return ig;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_trace_wanted(void) { extern long g_trace_budget; return g_trace_budget != 0; }
static long g_pl_trace_stmtno = 0;
static int pl_trace_is_control(const tree_t * t, int n, const char * f) { return t && t->t == TT_FNC && t->n == n && t->v.sval && !strcmp(t->v.sval, f); }
static void pl_trace_number_goals(const tree_t * t) {
    if (!t) return;
    if (pl_trace_is_control(t, 2, ",") || pl_trace_is_control(t, 2, ";") || pl_trace_is_control(t, 2, "->") || pl_trace_is_control(t, 2, "*->")) {
        pl_trace_number_goals(t->c[0]);
        pl_trace_number_goals(t->c[1]);
        return;
    }
    if (pl_trace_is_control(t, 1, "\\+") || pl_trace_is_control(t, 1, "call") || pl_trace_is_control(t, 1, "once")) { pl_trace_number_goals(t->c[0]); return; }
    if ((t->t == TT_FNC || t->t == TT_QLIT || t->t == TT_NAME || t->t == TT_CUT) && t->line <= 0) ((tree_t *) t)->line = (int) ++g_pl_trace_stmtno;
}
static IR_t * pl_trace_stmt_wrap(lcx_t * cx, long line, IR_t * entry, IR_t * ωfail) {
    if (line <= 0 || !pl_trace_wanted()) return entry;
    IR_t * call = build(cx, IR_CALL, entry, ωfail);
    IR_LIT(call).sval = "__trace_stmt";
    IR_t * lit = build(cx, IR_LIT_INTEGER, call, ωfail);
    IR_LIT(lit).ival = line;
    ir_operand_push(call, lit);
    return lit;
}
static IR_t * pl_trace_named_wrap(lcx_t * cx, const char * hook, const char * name, IR_t * entry, IR_t * ωfail) {
    if (!name || !*name || !pl_trace_wanted()) return entry;
    IR_t * call = build(cx, IR_CALL, entry, ωfail);
    IR_LIT(call).sval = (char *) hook;
    IR_t * nm = build(cx, IR_LIT_STRING, call, ωfail);
    IR_LIT(nm).sval = (char *) name;
    ir_operand_push(call, nm);
    return nm;
}
static IR_t * pl_leaf(lcx_t * cx, const char * sym, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * prev = NULL;
    IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL;
        IR_t * a = term_e(cx, t->c[i], &ae);
        IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en);
        else first = en;
        lc_ω_to(a, ωfail);
        prev = a;
        ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    { IR_t * fe = first ? first : nd; if (entry_out) *entry_out = fe; }
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char * nm; int ar; const char * sym; } pl_det_leaf_t;
static const pl_det_leaf_t pl_det_leaves[] = { { "var", 1, "$var" }, { "nonvar", 1, "$nonvar" }, { "atom", 1, "$atom" }, { "number", 1, "$number" }, { "integer", 1, "$integer" }, { "float", 1,
    "$float" }, { "atomic", 1, "$atomic" }, { "string", 1, "$string" }, { "compound", 1, "$compound" }, { "callable", 1, "$callable" }, { "ground", 1, "$ground" }, { "is_list", 1, "$is_list" },
    { "acyclic_term", 1, "$acyclic_term" }, { "==", 2, "$atop_eq" }, { "\\==", 2, "$atop_ne" }, { "@<", 2, "$atop_lt" }, { "@=<", 2, "$atop_le" }, { "@>", 2, "$atop_gt" }, { "@>=", 2, "$atop_ge" },
    { "compare", 3, "$compare" }, { "functor", 3, "$functor" }, { "arg", 3, "$arg" }, { "=..", 2, "$univ" }, { "copy_term", 2, "$copy_term" }, { "term_variables", 2, "$term_variables" },
    { "numbervars", 3, "$numbervars3" }, { "numbervars", 1, "$numbervars1" }, { "succ", 2, "$succ" }, { "$skip_list", 3, "$skip_list" }, { "$argv", 1, "$argv" }, { "plus", 3, "$plus" }, { "sort", 2,
    "$sort" }, { "msort", 2, "$msort" }, { "char_type", 2, "$char_type" }, { "term_string", 2, "$term_string" }, { "term_to_atom", 2, "$term_to_atom" }, { "atom_length", 2, "$atom_length" },
    { "atom_concat", 3, "$atom_concat" }, { "atom_chars", 2, "$atom_chars" }, { "atom_codes", 2, "$atom_codes" }, { "atom_number", 2, "$atom_number" }, { "atom_string", 2, "$atom_string" },
    { "upcase_atom", 2, "$upcase_atom" }, { "downcase_atom", 2, "$downcase_atom" }, { "string_concat", 3, "$string_concat" }, { "string_length", 2, "$string_length" }, { "string_lower", 2,
    "$string_lower" }, { "string_upper", 2, "$string_upper" }, { "string_to_atom", 2, "$string_to_atom" }, { "number_string", 2, "$number_string" }, { "string_chars", 2, "$string_chars" },
    { "string_codes", 2, "$string_codes" }, { "atomic_concat", 3, "$atomic_concat" }, { "atomic_list_concat", 2, "$atomic_list_concat" }, { "atomic_list_concat", 3, "$atomic_list_concat" },
    { "concat_atom", 2, "$concat_atom" }, { "concat_atom", 3, "$concat_atom" }, { "char_code", 2, "$char_code" }, { "number_codes", 2, "$number_codes" }, { "number_chars", 2, "$number_chars" },
    { "name", 2, "$name" }, { "get_char", 1, "$get_char" }, { "peek_char", 1, "$peek_char" }, { "get_code", 1, "$get_code" }, { "peek_code", 1, "$peek_code" }, { "get_byte", 1, "$get_byte" },
    { "peek_byte", 1, "$peek_byte" }, { "put_code", 1, "$put_code" }, { "put_byte", 1, "$put_byte" }, { "unget_char", 1, "$unget_char" }, { "at_end_of_stream", 0, "$at_end_of_stream" },
    { "current_prolog_flag", 2, "$current_prolog_flag" }, { "set_prolog_flag", 2, "$set_prolog_flag" }, { "telling", 1, "$telling" }, { "seeing", 1, "$seeing" }, { "tell", 1, "$tell" }, { "append", 1,
    "$append1" }, { "see", 1, "$see" }, { "told", 0, "$told" }, { "seen", 0, "$seen" }, { "at_end_of_stream", 1, "$at_end_of_stream_s" }, { "put", 1, "$put_code" }, { "get0", 1, "$get_code" },
    { "get", 1, "$get_edin" }, { "skip", 1, "$skip" }, { "unget_code", 1, "$unget_code" }, { "unget_byte", 1, "$unget_byte" }, { "get_code", 2, "$get_code_s" }, { "peek_code", 2, "$peek_code_s" },
    { "get_byte", 2, "$get_byte_s" }, { "peek_byte", 2, "$peek_byte_s" }, { "put_code", 2, "$put_code_s" }, { "put_byte", 2, "$put_byte_s" }, { "unget_char", 2, "$unget_char_s" }, { "unget_code", 2,
    "$unget_code_s" }, { "unget_byte", 2, "$unget_byte_s" }, { "read", 1, "$read" }, { "atom_to_term", 3, "$atom_to_term" }, { "read_term_from_atom", 3, "$read_term_from_atom" },
    { "read_term_from_chars", 3, "$read_term_from_chars" }, { "read_term_from_codes", 3, "$read_term_from_codes" }, { "writeq", 1, "$writeq" }, { "print", 1, "$print" }, { "write_term", 2,
    "$write_term" }, { "write_term", 3, "$write_term_s" }, { "write_canonical", 1, "$write_canonical" }, { "writeln", 1, "$writeln" }, { "display", 1, "$display" }, { "display", 2, "$display_s" },
    { "unify_with_occurs_check", 2, "$unify_oc" }, { "$aggregate_reduce", 3, "$aggregate_reduce" }, { "$wot_open", 3, "$wot_open" }, { "$wot_capture", 4, "$wot_capture" }, { "$wot_discard", 3,
    "$wot_discard" }, { "$set_prolog_flag_declare", 2, "$set_prolog_flag_declare" }, { "put_char", 1, "$put_char" }, { "$db_t_guard", 2, "$db_t_guard" }, { "$db_assertz_t", 1, "$db_assertz_t" },
    { "$db_asserta_t", 1, "$db_asserta_t" }, { "$db_abolish_t", 1, "$db_abolish_t" }, { "$db_retractall_t", 1, "$db_retractall_t" }, { "$db_seed_once", 3, "$db_seed_once" }, { "$db_asserta_r", 2,
    "$db_asserta_r" }, { "$db_assertz_r", 2, "$db_assertz_r" }, { "$db_erase_ref", 1, "$db_erase_ref" }, { "$db_n_r", 2, "$db_n_r" }, { "$db_at_r", 3, "$db_at_r" }, { "$db_ref_r", 3, "$db_ref_r" },
    { "$pl_declared", 2, "$pl_declared" }, { "$pl_dynamic", 1, "$pl_dynamic" }, { "$pl_list_guard", 1, "$pl_list_guard" }, { "$pl_op_check", 3, "$pl_op_check" }, { "$cutcall", 2, "$cutcall" },
    { "$pl_ioarg", 2, "$pl_ioarg" }, { "$put_code", 1, "$put_code" }, { "$put_code_s", 2, "$put_code_s" }, { "$put_char", 1, "$put_char" }, { "$put_char_c_s", 2, "$put_char_c_s" }, { "$get_code", 1,
    "$get_code" }, { "$get_code_s", 2, "$get_code_s" }, { "$peek_code", 1, "$peek_code" }, { "$peek_code_s", 2, "$peek_code_s" }, { "$get_char", 1, "$get_char" }, { "$get_char_s", 2, "$get_char_s" },
    { "$peek_char", 1, "$peek_char" }, { "$peek_char_s", 2, "$peek_char_s" }, { "$get_byte", 1, "$get_byte" }, { "$get_byte_s", 2, "$get_byte_s" }, { "$peek_byte", 1, "$peek_byte" }, { "$peek_byte_s",
    2, "$peek_byte_s" }, { "$get_edin", 1, "$get_edin" }, { "$skip", 1, "$skip" }, { "$current_prolog_flag", 2, "$current_prolog_flag" }, { "$pl_sp_check", 2, "$pl_sp_check" }, { "$pl_goal_guard", 1,
    "$pl_goal_guard" }, { "$pl_cp_count", 1, "$pl_cp_count" }, { "$pl_cp_nth", 3, "$pl_cp_nth" }, { "$pl_pp_guard", 1, "$pl_pp_guard" }, { "$pl_pp_count", 2, "$pl_pp_count" }, { "$pl_pp_nth", 3,
    "$pl_pp_nth" }, { "$pl_cp_guard", 1, "$pl_cp_guard" }, { "halt", 0, "$halt" }, { "halt", 1, "$halt" }, { "flush_output", 0, "$flush_output" }, { "format", 1, "$format" }, { "format", 2,
    "$format" }, { "write", 2, "$write_s" }, { "writeq", 2, "$writeq_s" }, { "print", 2, "$print_s" }, { "write_canonical", 2, "$write_canonical_s" }, { "writeln", 2, "$writeln_s" }, { "nl", 1,
    "$nl_s" }, { "put_char", 2, "$put_char_c_s" }, { "flush_output", 1, "$flush_output_s" }, { "format", 3, "$format3" }, { "read", 2, "$read_s" }, { "get_char", 2, "$get_char_s" }, { "peek_char", 2,
    "$peek_char_s" }, { "open", 3, "$open" }, { "open", 4, "$open4" }, { "close", 1, "$close" }, { "close", 2, "$close" }, { "current_output", 1, "$current_output" }, { "current_input", 1,
    "$current_input" }, { "set_output", 1, "$set_output" }, { "set_input", 1, "$set_input" }, { "keysort", 2, "$keysort" }, { "set_stream_position", 2, "$set_stream_position" }, { "op", 3, "$op" },
    { "$pl_op_count", 1, "$pl_op_count" }, { "$pl_op_nth", 4, "$pl_op_nth" }, { "$pl_sp_count", 1, "$pl_sp_count" }, { "$pl_sp_nth", 3, "$pl_sp_nth" }, { "$pl_cs_count", 1, "$pl_cs_count" },
    { "$pl_cs_nth", 4, "$pl_cs_nth" }, { "wall_us", 1, "$wall_us" }, { "wall_ms", 1, "$wall_ms" }, { "sort", 1, "$gnu_sort1" }, { "msort", 1, "$gnu_msort1" }, { "keysort", 1, "$gnu_keysort1" },
    { "line_count", 2, "$gnu_line_count" }, { "line_position", 2, "$gnu_line_position" }, { "character_count", 2, "$gnu_character_count" }, { "stream_line_column", 3, "$gnu_stream_line_column" },
    { "last_read_start_line_column", 2, "$gnu_last_read_start" }, { "absolute_file_name", 2, "$gnu_absolute_file_name" }, { "prolog_file_name", 2, "$gnu_prolog_file_name" }, { "$gnu_builtin", 2,
    "$gnu_builtin" }, { "working_directory", 1, "$gnu_working_directory" }, { "change_directory", 1, "$gnu_change_directory" }, { "make_directory", 1, "$gnu_make_directory" }, { "delete_file", 1,
    "$gnu_delete_file" }, { "file_exists", 1, "$gnu_file_exists" }, { "directory_files", 2, "$gnu_directory_files" }, { "term_hash", 2, "$gnu_term_hash" }, { "prolog_pid", 1, "$gnu_prolog_pid" },
    { "$gnu_environ_list", 1, "$gnu_environ_list" }, { "$gnu_file_props", 2, "$gnu_file_props" }, { 0, 0, 0 } };
static int pl_det_leaf_name_wired(const char * nm) { for (int i = 0; pl_det_leaves[i].nm; i++) if (!strcmp(nm, pl_det_leaves[i].nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_det_leaf_sym(const char * nm, int ar) {
    for (int i = 0; pl_det_leaves[i].nm; i++) if (pl_det_leaves[i].ar == ar && !strcmp(nm, pl_det_leaves[i].nm)) return pl_det_leaves[i].sym;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const struct {
    const char * nm;
    int ar;
    const char * gsym;
} pl_anum_guards[] = { { "atom_length", 2, "$pl_anum_guard2" }, { "atom_chars", 2, "$pl_anum_guard2" }, { "atom_codes", 2, "$pl_anum_guard2" }, { "char_code", 2, "$pl_anum_guard2" }, { "number_chars",
    2, "$pl_anum_guard2" }, { "number_codes", 2, "$pl_anum_guard2" }, { "number_string", 2, "$pl_anum_guard2" }, { "atom_concat", 3, "$pl_anum_guard3" }, { "atomic_concat", 3, "$pl_anum_guard3" },
    { "atomic_list_concat", 2, "$pl_anum_guard2" }, { "atomic_list_concat", 3, "$pl_anum_guard3" }, { "arg", 3, "$pl_anum_guard3" }, { "functor", 3, "$pl_anum_guard3" }, { 0, 0, 0 } };
static const char * pl_anum_guard_sym(const char * nm, int ar) {
    for (int i = 0; pl_anum_guards[i].nm; i++) if (pl_anum_guards[i].ar == ar && !strcmp(nm, pl_anum_guards[i].nm)) return pl_anum_guards[i].gsym;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf_lv_guarded(lcx_t * cx, const char * sym, const char * gsym, const char * gname, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * chk = build(cx, IR_CALL, nd, ωfail);
    IR_LIT(chk).sval = gsym;
    IR_t * nl = build(cx, IR_LIT_STRING, chk, ωfail);
    IR_LIT(nl).sval = (char *) gname;
    ir_operand_push(chk, nl);
    IR_t * prev = NULL;
    IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL;
        IR_t * a = term_lval_e(cx, t->c[i], &ae);
        IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en);
        else first = en;
        lc_ω_to(a, ωfail);
        prev = a;
        ir_operand_push(nd, a);
        ir_operand_push(chk, a);
    }
    if (prev) lc_γ_to(prev, nl);
    if (entry_out) *entry_out = first ? first : nl;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf_lv(lcx_t * cx, const char * sym, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * prev = NULL;
    IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL;
        IR_t * a = term_lval_e(cx, t->c[i], &ae);
        IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en);
        else first = en;
        lc_ω_to(a, ωfail);
        prev = a;
        ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first ? first : nd;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf_guarded1(lcx_t * cx, const char * sym, const char * guard_sym, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * ve = NULL;
    IR_t * v = term_lval_e(cx, t->c[0], &ve);
    lc_ω_to(v, ωfail);
    IR_t * chk = build(cx, IR_CALL, nd, ωfail);
    IR_LIT(chk).sval = guard_sym;
    ir_operand_push(chk, v);
    ir_operand_push(nd, v);
    lc_γ_to(v, chk);
    if (entry_out) *entry_out = ve ? ve : v;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf_guarded1_dir(lcx_t * cx, const char * sym, const char * guard_sym, const char * dir, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * ve = NULL;
    IR_t * v = term_lval_e(cx, t->c[0], &ve);
    lc_ω_to(v, ωfail);
    IR_t * chk = build(cx, IR_CALL, nd, ωfail);
    IR_LIT(chk).sval = guard_sym;
    IR_t * dl = build(cx, IR_LIT_STRING, chk, ωfail);
    IR_LIT(dl).sval = dir;
    ir_operand_push(chk, v);
    ir_operand_push(chk, dl);
    ir_operand_push(nd, v);
    lc_γ_to(v, dl);
    if (entry_out) *entry_out = ve ? ve : v;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dyn_index(const char * name, int arity) {
    if (!name) return -1;
    for (int i = 0; i < g_stage2.pl_dyn_n; i++) if (g_stage2.pl_dyn_name[i] && !strcmp(g_stage2.pl_dyn_name[i], name) && g_stage2.pl_dyn_arity[i] == arity) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_rt_is_dynamic(const char * nm, int ar) {
    extern int rt_pl_db_key_is_dynamic_na(void *, const char *, int);
    extern int rt_pl_db_key_cell_na(void *, const char *, int);
    return g_stage2.rt_lowering && g_stage2.rt_root && (rt_pl_db_key_cell_na(g_stage2.rt_root, nm, ar) >= 0 || rt_pl_db_key_is_dynamic_na(g_stage2.rt_root, nm, ar));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dyn_index_or_add(const char * name, int arity) {
    if (g_stage2.rt_lowering) {
        extern int rt_pl_db_cell_for(void *, const char *, int);
        int rk;
        if (!g_stage2.rt_root)
            pl_refuse("a clause compiled at run time names a root cell but its compile carries no root -- the registry road needs the standing frame (ARCH-PROLOG-C-OUT-OF-THE-BOX 5) --", name, 10);
        rk = rt_pl_db_cell_for(g_stage2.rt_root, name, arity);
        if (rk < 0) pl_refuse("a clause compiled at run time names a root cell but the root's registry could not grow its overflow vector --", name, 10);
        return rk;
    }
    { int k = pl_dyn_index(name, arity); if (k >= 0) return k; pl_dyn_mark(ct_strdup(name), arity); return pl_dyn_index(name, arity); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_db_leaf1(lcx_t * cx, const char * sym, int k, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = (char *) sym;
    IR_t * kn = build(cx, IR_LIT_INTEGER, NULL, ωfail);
    IR_LIT(kn).ival = k;
    lc_γ_to(kn, nd);
    lc_ω_to(kn, ωfail);
    ir_operand_push(nd, kn);
    if (entry_out) *entry_out = kn;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_file_defines(const char * nm, int ar) {
    char key[fmt_len("%s/%d", nm, ar)];
    snprintf(key, sizeof key, "%s/%d", nm, ar);
    const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
    return (ch && ch->t == TT_CHOICE && ch->n > 0) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_owned(const char * nm, int ar) {
    if (pl_rt_is_dynamic(nm, ar)) return 1;
    if (pl_decl_dyn_is(nm, ar)) return 1;
    if (pl_dyn_index(nm, ar) < 0) return 0;
    return pl_file_defines(nm, ar) ? 0 : 1;
}
static tree_t * pl_static_clause_term(const tree_t * cl, const char * pn, int ar);
static IR_t * pl_db_leaf_seed(lcx_t * cx, int k, int i, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
static IR_t * pl_db_seed_file(lcx_t * cx, const char * pn, int ar, IR_t * next, IR_t * ωfail, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!pl_file_defines(pn, ar)) return next;
    {
        char key[fmt_len("%s/%d", pn, ar)];
        snprintf(key, sizeof key, "%s/%d", pn, ar);
        const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
        int k = pl_dyn_index_or_add(pn, ar);
        if (k < 0 || !ch) pl_refuse("a dynamic predicate with file clauses needs a root cell and the 64 compile-time root cells are exhausted --", pn, 10);
        {
            IR_t * first = NULL;
            int n = (ch->t == TT_CHOICE) ? ch->n : 1;
            for (int i = n - 1; i >= 0; i--) {
                const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[i] : ch;
                IR_t * se = NULL;
                pl_db_leaf_seed(cx, k, i, pl_static_clause_term(cl, pn, ar), next, ωfail, &se);
                next = se;
                first = se;
            }
            if (entry_out) *entry_out = first;
            return next;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_head_key(const tree_t * a, int * ar_out) {
    const tree_t * h = a;
    if (!h) return NULL;
    if (h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":-") && h->n == 2) h = h->c[0];
    if (!h) return NULL;
    if (h->t == TT_FNC && h->v.sval) { if (ar_out) *ar_out = h->n; return h->v.sval; }
    if ((h->t == TT_QLIT || h->t == TT_NAME) && h->v.sval) { if (ar_out) *ar_out = 0; return h->v.sval; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_pi_is_static_builtin(const char * pn, int ar) {
    if (!pn) return 0;
    if (pl_decl_dyn_is(pn, ar)) return 0;
    if (pl_pi_is_control(pn, ar)) return 1;
    if (pl_det_leaf_sym(pn, ar)) return 1;
    if (pl_rung_of(pn) && strcmp(pn, "for")) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_pi_is_builtin(const char * pn, int ar) { return pl_pi_is_static_builtin(pn, ar); }
static const tree_t * pl_tree_number(const tree_t * t) { return (t && (t->t == TT_ILIT || t->t == TT_FLIT || pl_tree_is_big(t))) ? t : NULL; }
static const tree_t * pl_body_ill_typed(const tree_t * b) {
    if (!b) return NULL;
    if (pl_tree_number(b)) return b;
    if (b->t == TT_FNC && b->v.sval && b->n == 2 && (!strcmp(b->v.sval, ",") || !strcmp(b->v.sval, ";") || !strcmp(b->v.sval, "->") || !strcmp(b->v.sval, "*->") || !strcmp(b->v.sval, "|")))
        return pl_body_ill_typed(b->c[0]) ? b : (pl_body_ill_typed(b->c[1]) ? b : NULL);
    if (b->t == TT_FNC && b->v.sval && b->n == 1 && !strcmp(b->v.sval, "\\+")) return pl_body_ill_typed(b->c[0]) ? b : NULL;
    return NULL;
}
static const tree_t * pl_clause_ill_typed(const tree_t * cl) {
    if (!cl) return NULL;
    if (cl->t == TT_FNC && cl->v.sval && !strcmp(cl->v.sval, ":-") && cl->n == 2) { if (pl_tree_number(cl->c[0])) return cl->c[0]; return pl_body_ill_typed(cl->c[1]); }
    return pl_tree_number(cl);
}
static const char * pl_pred_props[] = { "built_in", "dynamic", "static", "defined", "multifile", "discontiguous", "meta_predicate", "exported", "imported_from", "foreign", "iso", "number_of_clauses",
    "public", "private", "native", "tabled", "volatile", "transparent", "nodebug", "visible", "template", "alias", "declared_in", "defined_in", "scope", "synchronized", "file", "line_count",
    "notrace", "thread_local", "non_terminal", "ssu", "quasi_quotation_syntax", NULL };
static const char * pl_meta_template(const char * pn, int ar) {
    static const struct {
        const char * n;
        int a;
        const char * t;
    } m[] = { { ",", 2, "00" }, { ";", 2, "00" }, { "->", 2, "00" }, { "*->", 2, "00" }, { "\\+", 1, "0" }, { "call", 1, "0" }, { "call", 2, "1?" }, { "call", 3, "2??" }, { "call", 4, "3???" },
        { "call", 5, "4????" }, { "call", 6, "5?????" }, { "call", 7, "6??????" }, { "call", 8, "7???????" }, { "once", 1, "0" }, { "ignore", 1, "0" }, { "not", 1, "0" }, { "catch", 3, "0?0" },
        { "findall", 3, "?0?" }, { "findall", 4, "?0??" }, { "bagof", 3, "?^?" }, { "setof", 3, "?^?" }, { "forall", 2, "00" }, { "aggregate_all", 3, "?0?" }, { "setup_call_cleanup", 3, "000" },
        { "call_cleanup", 2, "00" }, { "call_nth", 2, "0?" }, { "if", 3, "000" }, { NULL, 0, NULL } };
    for (int i = 0; m[i].n; i++) if (m[i].a == ar && !strcmp(m[i].n, pn)) return m[i].t;
    return (const char *) 0;
}
static tree_t * pl_cc_throw_ar(tree_t * formal, const char * nm, int ar) { return pl_cc_fnc1("throw", pl_cc_fnc2("error", formal, pl_cc_pi_ar(nm, ar))); }
static tree_t * pl_cc_type_error(const char * type, const tree_t * culprit, const char * nm, int ar) {
    return pl_cc_throw_ar(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal(type), (tree_t *) culprit), nm, ar);
}
static tree_t * pl_cc_perm_access(const char * pn, int ar) {
    return pl_cc_fnc1("throw",
        pl_cc_fnc2("error", pl_cc_fnc3("permission_error", (tree_t *) pl_atom_goal("access"), (tree_t *) pl_atom_goal("private_procedure"), pl_cc_pi_ar(pn, ar)), pl_cc_pi_ar("clause", 2)));
}
static tree_t * pl_cc_spec_ill_typed(const tree_t * s, const char * nm) {
    if (!s || s->t == TT_VAR) return NULL;
    if (!(s->t == TT_FNC && s->v.sval && !strcmp(s->v.sval, "/") && s->n == 2)) return pl_cc_type_error("predicate_indicator", s, nm, 1);
    {
        const tree_t * n = s->c[0];
        const tree_t * a = s->c[1];
        if (n && n->t != TT_VAR && !(n->t == TT_QLIT || n->t == TT_NAME)) return strcmp(nm, "abolish") ? pl_cc_type_error("predicate_indicator", s, nm, 1) : pl_cc_type_error("atom", n, nm, 1);
        if (a && a->t != TT_VAR && a->t != TT_ILIT) return strcmp(nm, "abolish") ? pl_cc_type_error("predicate_indicator", s, nm, 1) : pl_cc_type_error("integer", a, nm, 1);
        if (a && a->t == TT_ILIT && a->v.ival < 0 && !strcmp(nm, "abolish")) return pl_cc_throw_ar(pl_cc_fnc2("domain_error", (tree_t *) pl_atom_goal("not_less_than_zero"), (tree_t *) a), nm, 1);
        if (a && a->t == TT_ILIT && a->v.ival > 1024 && !strcmp(nm, "abolish")) return pl_cc_throw_ar(pl_cc_fnc1("representation_error", (tree_t *) pl_atom_goal("max_arity")), nm, 1);
        return NULL;
    }
}
static const char * pl_spec_key(const tree_t * s, int * ar_out) {
    if (!s || s->t != TT_FNC || !s->v.sval || strcmp(s->v.sval, "/") || s->n != 2) return NULL;
    if (!s->c[0] || !(s->c[0]->t == TT_QLIT || s->c[0]->t == TT_NAME) || !s->c[0]->v.sval) return NULL;
    if (!s->c[1] || s->c[1]->t != TT_ILIT) return NULL;
    if (ar_out) *ar_out = (int) s->c[1]->v.ival;
    return s->c[0]->v.sval;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_db_leaf2_tree(lcx_t * cx, const char * sym, const tree_t * a0, const tree_t * a1, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
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
static IR_t * pl_db_leaf2(lcx_t * cx, const char * sym, int k, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = (char *) sym;
    IR_t * kn = build(cx, IR_LIT_INTEGER, NULL, ωfail);
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
static IR_t * pl_db_enum(lcx_t * cx, int k, const tree_t * target, int erase, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * er = NULL;
    if (erase) { er = build(cx, IR_CALL, γnext, ωfail); IR_LIT(er).sval = "$db_erase"; }
    IR_t * uni = build(cx, IR_CALL, er ? er : γnext, ωfail);
    IR_LIT(uni).sval = erase ? "$unify" : "$clause_unify";
    IR_t * cp = build(cx, IR_CALL, uni, ωfail);
    IR_LIT(cp).sval = "$db_copy";
    IR_t * to = build(cx, IR_TO, cp, ωfail);
    IR_LIT(to).sval = (char *) "db";
    IR_t * z = build(cx, IR_LIT_INTEGER, to, ωfail);
    IR_LIT(z).ival = 0;
    IR_t * te = NULL;
    IR_t * tv = term_e(cx, target, &te);
    IR_t * kn;
    if (k < 0) {
        kn = build(cx, IR_CALL, z, ωfail);
        IR_LIT(kn).sval = "$db_store_k";
        ir_operand_push(kn, tv);
        lc_γ_to(tv, kn);
        lc_ω_to(tv, ωfail);
    } else {
        kn = build(cx, IR_LIT_INTEGER, NULL, ωfail);
        IR_LIT(kn).ival = k;
        lc_γ_to(kn, te ? te : tv);
        lc_ω_to(kn, ωfail);
        lc_γ_to(tv, z);
        lc_ω_to(tv, ωfail);
    }
    ir_operand_push(to, kn);
    ir_operand_push(to, z);
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
static IR_t * pl_db_enum_ref(lcx_t * cx, const tree_t * target, const tree_t * refterm, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * un2 = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(un2).sval = "$unify";
    IR_t * rat = build(cx, IR_CALL, un2, ωfail);
    IR_LIT(rat).sval = "$db_ref_r";
    IR_t * uni = build(cx, IR_CALL, rat, ωfail);
    IR_LIT(uni).sval = "$clause_unify";
    IR_t * at = build(cx, IR_CALL, uni, ωfail);
    IR_LIT(at).sval = "$db_at_r";
    IR_t * to = build(cx, IR_TO, at, ωfail);
    IR_LIT(to).sval = (char *) "ag";
    IR_t * cnt = build(cx, IR_CALL, to, ωfail);
    IR_LIT(cnt).sval = "$db_n_r";
    IR_t * lo = build(cx, IR_LIT_INTEGER, cnt, ωfail);
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
static tree_t * pl_clause_target(const tree_t * c, const tree_t * body) {
    if (!body && c && c->t == TT_FNC && c->v.sval && !strcmp(c->v.sval, ":-") && c->n == 2) return (tree_t *) c;
    {
        tree_t * tg = ast_node_new(TT_FNC);
        tg->v.sval = (char *) ":-";
        ast_push(tg, (tree_t *) c);
        if (body) ast_push(tg, (tree_t *) body);
        else { tree_t * tr = ast_node_new(TT_QLIT); tr->v.sval = (char *) "true"; ast_push(tg, tr); }
        return tg;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_tree_copy(const tree_t * t) {
    if (!t) return NULL;
    { tree_t * c = ast_node_new(t->t); c->v = t->v; c->line = t->line; for (int i = 0; i < t->n; i++) ast_push(c, pl_tree_copy(t->c[i])); return c; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int g_pl_seed_var_base = 4096;
static int pl_tree_var_occ(const tree_t * t) { int c = 0; if (!t) return 0; if (t->t == TT_VAR) return 1; for (int i = 0; i < t->n; i++) c += pl_tree_var_occ(t->c[i]); return c; }
static void pl_tree_renumber_vars(tree_t * t, long long * slots, int * n, int cap, int base) {
    if (!t) return;
    if (t->t == TT_VAR) {
        int slot = -1;
        long long key = t->v.ival;
        for (int i = 0; i < *n; i++) if (slots[i] == key) { slot = i; break; }
        if (slot < 0 && *n < cap) { slots[*n] = key; slot = *n; (*n)++; }
        t->v.ival = base + (slot < 0 ? cap - 1 : slot);
        return;
    }
    for (int i = 0; i < t->n; i++) pl_tree_renumber_vars(t->c[i], slots, n, cap, base);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_body_term(const tree_t * b) {
    if (!b) return (tree_t *) pl_atom_goal("true");
    if (b->t == TT_VAR) { tree_t * c = ast_node_new(TT_FNC); c->v.sval = (char *) "call"; ast_push(c, pl_tree_copy(b)); return c; }
    if (b->t == TT_IF) {
        tree_t * ar = ast_node_new(TT_FNC);
        ar->v.sval = (char *) "->";
        ast_push(ar, pl_body_term(b->n > 0 ? b->c[0] : NULL));
        ast_push(ar, pl_body_term(b->n > 1 ? b->c[1] : NULL));
        if (b->v.ival) return ar;
        { tree_t * d = ast_node_new(TT_FNC); d->v.sval = (char *) ";"; ast_push(d, ar); ast_push(d, (b->n > 2) ? pl_body_term(b->c[2]) : (tree_t *) pl_atom_goal("fail")); return d; }
    }
    if (b->t == TT_PROGRAM) {
        tree_t * acc = NULL;
        for (int i = b->n - 1; i >= 0; i--) {
            tree_t * g = pl_body_term(b->c[i]);
            if (!acc) acc = g;
            else { tree_t * c = ast_node_new(TT_FNC); c->v.sval = (char *) ","; ast_push(c, g); ast_push(c, acc); acc = c; }
        }
        return acc ? acc : (tree_t *) pl_atom_goal("true");
    }
    if (b->t == TT_FNC && b->v.sval && b->n >= 2 && (!strcmp(b->v.sval, ",") || !strcmp(b->v.sval, ";") || !strcmp(b->v.sval, "->"))) {
        tree_t * c = ast_node_new(TT_FNC);
        c->v.sval = b->v.sval;
        for (int i = 0; i < b->n; i++) ast_push(c, pl_body_term(b->c[i]));
        return c;
    }
    return pl_tree_copy(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_static_clause_term(const tree_t * cl, const char * pn, int ar) {
    tree_t * h;
    tree_t * bt = NULL;
    int nc = (cl && cl->t == TT_CLAUSE) ? cl->n : 0;
    if (ar > 0) {
        h = ast_node_new(TT_FNC);
        h->v.sval = ct_strdup(pn);
        for (int i = 0; i < ar; i++) ast_push(h, (i < nc) ? pl_tree_copy(cl->c[i]) : pl_meta_var("_"));
    } else {
        h = ast_node_new(TT_QLIT);
        h->v.sval = ct_strdup(pn);
    }
    for (int i = nc - 1; i >= ar; i--) {
        tree_t * g = pl_body_term(cl->c[i]);
        if (!bt) bt = g;
        else { tree_t * c = ast_node_new(TT_FNC); c->v.sval = (char *) ","; ast_push(c, g); ast_push(c, bt); bt = c; }
    }
    if (!bt) { bt = ast_node_new(TT_QLIT); bt->v.sval = (char *) "true"; }
    {
        tree_t * tg = pl_clause_target(h, bt);
        int occ = pl_tree_var_occ(tg) + 1;
        long long slots[occ];
        int n = 0;
        pl_tree_renumber_vars(tg, slots, &n, occ, g_pl_seed_var_base);
        g_pl_seed_var_base += (n > 0 ? n : 1);
        return tg;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_db_leaf_seed(lcx_t * cx, int k, int i, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = "$db_seed_once";
    IR_t * kn = build(cx, IR_LIT_INTEGER, NULL, ωfail);
    IR_LIT(kn).ival = k;
    IR_t * in = build(cx, IR_LIT_INTEGER, NULL, ωfail);
    IR_LIT(in).ival = i;
    IR_t * te = NULL;
    IR_t * tv = term_e(cx, arg, &te);
    lc_γ_to(kn, in);
    lc_ω_to(kn, ωfail);
    lc_γ_to(in, te ? te : tv);
    lc_ω_to(in, ωfail);
    lc_γ_to(tv, nd);
    lc_ω_to(tv, ωfail);
    ir_operand_push(nd, kn);
    ir_operand_push(nd, in);
    ir_operand_push(nd, tv);
    if (entry_out) *entry_out = kn;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_nb_key(const tree_t * t) { if (!t || !(t->t == TT_QLIT || t->t == TT_NAME) || !t->v.sval) return NULL; return t->v.sval; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_nb_leaf_lv(lcx_t * cx, const char * sym, int k, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = (char *) sym;
    IR_t * kn = build(cx, IR_LIT_INTEGER, NULL, ωfail);
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
static IR_t * pl_nb_leaf_lv_tree(lcx_t * cx, const char * sym, const tree_t * keyt, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
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
static IR_t * pl_meta_call_dyn(lcx_t * cx, const tree_t * g, const tree_t * const * extra, int nextra, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
static int pl_tree_has_var(const tree_t * t) { if (!t) return 0; if (t->t == TT_VAR) return 1; for (int i = 0; i < t->n; i++) if (pl_tree_has_var(t->c[i])) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_CALL_STATIC (1 << 30)
static int pl_callee_static(lcx_t * cx, const char * nm, int ar, const char * key, int db_live, int defined) {
    if (g_stage2.rt_lowering || db_live || pl_decl_dyn_is(nm, ar)) return 0;
    if (defined) return 1;
    return (cx && cx->cur_key && strcmp(cx->cur_key, key) == 0) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_user_call(lcx_t * cx, const char * nm, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    int pl_db_live = pl_db_owned(nm, nargs);
    int st = 0;
    {
        char key[fmt_len("%s/%d", nm, nargs)];
        snprintf(key, sizeof key, "%s/%d", nm, nargs);
        const tree_t * ch = pl_db_live ? (const tree_t *) 0 : resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
        int file_def = ch && ch->t == TT_CHOICE && ch->n > 0;
        int bbhit = (pl_db_live || file_def) ? 0 : (pl_bb_lookup(key, nargs) != 0);
        if (!ch && !pl_db_live && !bbhit) return pl_meta_call_dyn(cx, t, NULL, 0, γnext, ωfail, entry_out);
        if (ch && ch->t == TT_FNC && !g_stage2.rt_lowering) { IR_t * nd = build(cx, IR_GOTO, ωfail, ωfail); if (entry_out) *entry_out = nd; return nd; }
        st = pl_callee_static(cx, nm, nargs, key, pl_db_live, file_def || bbhit);
    }
    IR_t * nd = build(cx, IR_CALL_PROC_STAGED, γnext, ωfail);
    IR_LIT(nd).sval = pl_pi_name(nm, nargs);
    nd->strict = st ? PL_CALL_STATIC : 0;
    IR_t * prev = NULL;
    IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL;
        IR_t * a = term_lval_e(cx, t->c[i], &ae);
        IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en);
        else first = en;
        lc_ω_to(a, ωfail);
        prev = a;
        ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    IR_t * body_entry = first ? first : nd;
    if (pl_db_live) {
        tree_t * pit = ast_node_new(TT_QLIT);
        pit->v.sval = (char *) pl_pi_name(nm, nargs);
        IR_t * ne = NULL;
        pl_db_leaf1(cx, "$db_nonempty", pl_dyn_index_or_add(nm, nargs), body_entry, ωfail, &ne);
        IR_t * guard_entry = NULL;
        pl_db_leaf2(cx, "$db_alive", pl_dyn_index_or_add(nm, nargs), pit, ne, ωfail, &guard_entry);
        if (g_stage2.rt_lowering) {
            if (entry_out) *entry_out = guard_entry;
        } else {
            IR_t * se = NULL;
            pl_db_seed_file(cx, nm, nargs, guard_entry, ωfail, &se);
            if (entry_out) *entry_out = se ? se : guard_entry;
        }
        return nd;
    }
    if (entry_out) *entry_out = body_entry;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_nil_term(void) { return ast_node_new(TT_MAKELIST); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_meta_call_dyn(lcx_t * cx, const tree_t * g, const tree_t * const * extra, int nextra, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL_VALUE, γnext, ωfail);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_is_callable(const tree_t * g) {
    if (!g) return 0;
    switch (g->t) { case TT_FNC: case TT_QLIT: case TT_NAME: case TT_CUT: case TT_UNIFY: case TT_IF: case TT_PROGRAM: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_phrase_plain_nt(const tree_t * g) {
    static const char * const ctl[] = { ",", ";", "->", "*->", "\\+", "{}", ".", "[]", "[|]", "!", "|", "call", 0 };
    if (!g || (g->t != TT_FNC && g->t != TT_NAME && g->t != TT_QLIT) || !g->v.sval) return 0;
    for (int i = 0; ctl[i]; i++) if (!strcmp(g->v.sval, ctl[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_meta_goal(const tree_t * g, const tree_t * const * extra, int nextra) {
    if (!g) return NULL;
    if (nextra <= 0) return pl_tree_is_callable(g) ? g : NULL;
    if (g->t != TT_FNC && g->t != TT_NAME && g->t != TT_QLIT) return NULL;
    tree_t * e = ast_node_new(TT_FNC);
    e->v.sval = g->v.sval;
    e->line = g->line;
    for (int i = 0; i < g->n; i++) ast_push(e, g->c[i]);
    for (int i = 0; i < nextra; i++) ast_push(e, (tree_t *) extra[i]);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * goal_inner(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
static int pl_trace_stmt_control(const tree_t * t) {
    if (!t) return 1;
    if (t->t == TT_PROGRAM || t->t == TT_MAKELIST || t->t == TT_IF) return 1;
    if (t->t != TT_FNC || t->n != 2 || !t->v.sval) return 0;
    return !strcmp(t->v.sval, ",") || !strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|") || !strcmp(t->v.sval, "->") || !strcmp(t->v.sval, "*->");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    if (!t || !pl_trace_wanted() || cx->stmt_depth > 0 || pl_trace_stmt_control(t)) return goal_inner(cx, t, γnext, ωfail, entry_out);
    IR_t * e = NULL;
    cx->stmt_depth++;
    IR_t * nd = goal_inner(cx, t, γnext, ωfail, &e);
    cx->stmt_depth--;
    IR_t * te = pl_trace_stmt_wrap(cx, (t->line > 0) ? (long) t->line : ++g_pl_trace_stmtno, e ? e : nd, ωfail);
    if (entry_out) *entry_out = te;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_curout_writer(const char * nm, int n) {
    static const struct {
        const char * n;
        int a;
    } w[] = { { "write", 1 }, { "writeq", 1 }, { "print", 1 }, { "write_canonical", 1 }, { "write_term", 2 }, { "writeln", 1 }, { "format", 1 }, { "format", 2 }, { NULL, 0 } };
    for (int i = 0; w[i].n; i++) if (w[i].a == n && !strcmp(w[i].n, nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_curout_text_guard(lcx_t * cx, IR_t * nd, IR_t * ne, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * ge = NULL;
    IR_t * g = goal(cx, pl_cc_fnc2("$pl_ioarg", (tree_t *) pl_atom_goal("put_text1"), (tree_t *) pl_atom_goal("[]")), ne ? ne : nd, ωfail, &ge);
    if (entry_out) *entry_out = ge ? ge : g;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_pp_runtime(const tree_t * h, const tree_t * pp) {
    tree_t * cv = pl_cc_freshvar();
    tree_t * cv2 = pl_cc_freshvar();
    tree_t * iv = pl_cc_freshvar();
    tree_t * iv2 = pl_cc_freshvar();
    cv2->v.ival = cv->v.ival;
    iv2->v.ival = iv->v.ival;
    return pl_cc_fnc2(",", pl_cc_fnc1("$pl_pp_guard", (tree_t *) h),
        pl_cc_fnc2(",", pl_cc_fnc2("$pl_pp_count", (tree_t *) h, cv), pl_cc_fnc2(",", pl_cc_fnc3("between", pl_cc_ilit(1), cv2, iv), pl_cc_fnc3("$pl_pp_nth", iv2, (tree_t *) h, (tree_t *) pp))));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * goal_inner(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!t) return build(cx, IR_SUCCEED, γnext, ωfail);
    if (pl_tree_number(t)) return goal(cx, pl_cc_type_error("callable", t, "call", 1), γnext, ωfail, entry_out);
    if (t->t == TT_MAKELIST && t->n > 0) {
        tree_t * g = pl_cc_fnc1("consult", (tree_t *) t->c[0]);
        for (int i = 1; i < t->n; i++) g = pl_cc_fnc2(",", g, pl_cc_fnc1("consult", (tree_t *) t->c[i]));
        return goal(cx, g, γnext, ωfail, entry_out);
    }
    switch (t->t) {
        case TT_FNC:
        {
            const char * nm = t->v.sval ? t->v.sval : "?";
            if (!strcmp(nm, ",")) {
                lc_vec glv;
                lc_vec_init(&glv, (int) sizeof(const tree_t *));
                collect_conj(t, &glv);
                {
                    IR_t * cω = build(cx, IR_GOTO, ωfail, ωfail);
                    IR_t * ientry = NULL;
                    IR_t * iredo = NULL;
                    pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, γnext, cω, &ientry, &iredo, NULL);
                    cx->meta_redo = iredo;
                    cx->meta_redo_set = 1;
                    if (entry_out) *entry_out = ientry;
                    return cω;
                }
            }
            if (!strcmp(nm, "write") && t->n == 1) { IR_t * ne = NULL; IR_t * nd = pl_leaf(cx, "$write", t, 1, γnext, ωfail, &ne); return pl_curout_text_guard(cx, nd, ne, ωfail, entry_out); }
            if (!strcmp(nm, ";") || !strcmp(nm, "|")) {
                if (pl_is_ite(t)) return pl_lower_ite(cx, t->c[0]->c[0], t->c[0]->c[1], t->c[1], γnext, ωfail, entry_out);
                if (pl_is_scite(t)) return pl_lower_softcut(cx, t->c[0]->c[0], t->c[0]->c[1], t->c[1], γnext, ωfail, entry_out);
                return pl_lower_disj(cx, t, γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "->") && t->n == 2) return pl_lower_ite(cx, t->c[0], t->c[1], NULL, γnext, ωfail, entry_out);
            if (!strcmp(nm, "*->") && t->n == 2) return pl_lower_softcut(cx, t->c[0], t->c[1], NULL, γnext, ωfail, entry_out);
            if (!strcmp(nm, "if") && t->n == 3 && !pl_db_owned(nm, 3) && !pl_file_defines(nm, 3) && !pl_bb_lookup("if/3", 3))
                return pl_lower_softcut(cx, pl_cc_fnc1("call", (tree_t *) t->c[0]), pl_cc_fnc1("call", (tree_t *) t->c[1]), pl_cc_fnc1("call", (tree_t *) t->c[2]), γnext, ωfail, entry_out);
            if ((!strcmp(nm, "\\+") || !strcmp(nm, "not")) && t->n == 1) return pl_lower_ite(cx, t->c[0], pl_atom_goal("fail"), pl_atom_goal("true"), γnext, ωfail, entry_out);
            if (!strcmp(nm, "once") && t->n == 1) return pl_lower_ite(cx, t->c[0], pl_atom_goal("true"), pl_atom_goal("fail"), γnext, ωfail, entry_out);
            if (!strcmp(nm, "ignore") && t->n == 1) return pl_lower_ite(cx, t->c[0], pl_atom_goal("true"), pl_atom_goal("true"), γnext, ωfail, entry_out);
            if (!strcmp(nm, "forall") && t->n == 2) {
                tree_t * inner = ast_node_new(TT_FNC);
                inner->v.sval = (char *) "\\+";
                ast_push(inner, (tree_t *) t->c[1]);
                tree_t * conj = ast_node_new(TT_FNC);
                conj->v.sval = (char *) ",";
                ast_push(conj, (tree_t *) t->c[0]);
                ast_push(conj, inner);
                return pl_lower_ite(cx, conj, pl_atom_goal("fail"), pl_atom_goal("true"), γnext, ωfail, entry_out);
            }
            if ((!strcmp(nm, "call") && t->n >= 1) || (!strcmp(nm, "phrase") && (t->n == 2 || t->n == 3))) {
                const tree_t * xs[2];
                const tree_t * const * extra;
                int nextra;
                if (!strcmp(nm, "phrase")) {
                    xs[0] = t->c[1];
                    xs[1] = (t->n == 3) ? t->c[2] : pl_nil_term();
                    extra = xs;
                    nextra = 2;
                    if (!pl_phrase_plain_nt(t->c[0])) return goal(cx, pl_cc_fnc3("$phrase", (tree_t *) t->c[0], (tree_t *) xs[0], (tree_t *) xs[1]), γnext, ωfail, entry_out);
                } else {
                    extra = (const tree_t * const *) &t->c[1];
                    nextra = t->n - 1;
                }
                if (t->c[0] && t->c[0]->t == TT_VAR) return pl_meta_call_dyn(cx, t->c[0], extra, nextra, γnext, ωfail, entry_out);
                const tree_t * ext = pl_meta_goal(t->c[0], extra, nextra);
                if (ext && pl_body_ill_typed(ext)) return goal(cx, pl_cc_type_error("callable", ext, nm, t->n), γnext, ωfail, entry_out);
                if (!ext)
                    return goal(cx, pl_cc_fnc1("throw", pl_cc_fnc2("error", pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("callable"), (tree_t *) t->c[0]), pl_cc_pi_ar(nm, t->n))), γnext, ωfail,
                    entry_out);
                {
                    IR_t * callω = build(cx, IR_GOTO, ωfail, ωfail);
                    IR_t * saveω = cx->cutω;
                    cx->cutω = callω;
                    int save_scope = cx->cut_scope;
                    cx->cut_scope = ++cx->scope_seq;
                    lc_vec glv;
                    lc_vec_init(&glv, (int) sizeof(const tree_t *));
                    collect_conj(ext, &glv);
                    IR_t * ientry = NULL;
                    IR_t * iredo = NULL;
                    pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, γnext, callω, &ientry, &iredo, NULL);
                    cx->cutω = saveω;
                    cx->cut_scope = save_scope;
                    cx->meta_redo = iredo;
                    cx->meta_redo_set = 1;
                    if (pl_tree_is_control(ext) && pl_tree_control_var_leaf(ext)) {
                        IR_t * gge = NULL;
                        IR_t * gg = goal(cx, pl_cc_fnc1("$pl_goal_guard", (tree_t *) ext), ientry ? ientry : γnext, ωfail, &gge);
                        ientry = gge ? gge : gg;
                    }
                    if (entry_out) *entry_out = ientry;
                    return callω;
                }
            }
            if (!strcmp(nm, "throw") && t->n == 1) return pl_leaf(cx, "$throw", t, 1, ωfail, ωfail, entry_out);
            if (!strcmp(nm, "catch") && t->n == 3) return pl_lower_catch(cx, t->c[0], t->c[1], t->c[2], γnext, ωfail, entry_out);
            if (!strcmp(nm, "setup_call_cleanup") && t->n == 3) return pl_lower_scc(cx, t->c[0], t->c[1], t->c[2], γnext, ωfail, entry_out);
            if (!strcmp(nm, "call_cleanup") && t->n == 2 && !pl_file_defines(nm, 2)) return pl_lower_scc(cx, pl_atom_goal("true"), t->c[0], t->c[1], γnext, ωfail, entry_out);
            if (!strcmp(nm, "=") && t->n == 2) { IR_t * e = NULL; IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e); if (entry_out) *entry_out = e ? e : nd; return nd; }
            if (!strcmp(nm, "\\=") && t->n == 2) {
                tree_t * u = ast_node_new(TT_UNIFY);
                ast_push(u, (tree_t *) t->c[0]);
                ast_push(u, (tree_t *) t->c[1]);
                return pl_lower_ite(cx, u, pl_atom_goal("fail"), pl_atom_goal("true"), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "is") && t->n == 2) {
                IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$is_v";
                IR_t * xe = NULL;
                IR_t * xl = term_lval_e(cx, t->c[0], &xe);
                IR_t * ve = NULL;
                IR_t * v = lower_arith_val(cx, t->c[1], ωfail, &ve);
                lc_γ_to(xl, ve ? ve : v);
                lc_ω_to(xl, ωfail);
                lc_γ_to(v, nd);
                lc_ω_to(v, ωfail);
                ir_operand_push(nd, xl);
                ir_operand_push(nd, v);
                if (entry_out) *entry_out = xe ? xe : xl;
                return nd;
            }
            {
                const char * csuf = (t->n == 2) ? pl_cmp_op_suffix(nm) : NULL;
                if (csuf) {
                    char nb[fmt_len("$cmp_%s", csuf)];
                    snprintf(nb, sizeof nb, "$cmp_%s", csuf);
                    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                    IR_LIT(nd).sval = ct_strdup(nb);
                    IR_t * ea = NULL;
                    IR_t * eb = NULL;
                    IR_t * a = lower_arith_val(cx, t->c[0], ωfail, &ea);
                    IR_t * b = lower_arith_val(cx, t->c[1], ωfail, &eb);
                    lc_γ_to(a, eb ? eb : b);
                    lc_ω_to(a, ωfail);
                    lc_γ_to(b, nd);
                    lc_ω_to(b, ωfail);
                    ir_operand_push(nd, a);
                    ir_operand_push(nd, b);
                    if (entry_out) *entry_out = ea ? ea : a;
                    return nd;
                }
            }
            if (!strcmp(nm, "tab") && t->n == 1 && !(t->c[0] && t->c[0]->t == TT_ILIT) && !pl_file_defines(nm, 1)) {
                tree_t * fv = pl_cc_freshvar();
                tree_t * f2 = pl_cc_freshvar();
                tree_t * f3 = pl_cc_freshvar();
                f2->v.ival = fv->v.ival;
                f3->v.ival = fv->v.ival;
                IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$tab";
                IR_t * ve = NULL;
                IR_t * v = term_e(cx, f3, &ve);
                lc_γ_to(v, nd);
                lc_ω_to(v, ωfail);
                ir_operand_push(nd, v);
                {
                    IR_t * ge = NULL;
                    IR_t * g =
                        goal(cx,
                        pl_cc_fnc2(",", pl_cc_fnc2("is", fv, (tree_t *) t->c[0]),
                        pl_cc_ite(pl_cc_fnc1("integer", f2), (tree_t *) pl_atom_goal("true"), pl_cc_throw(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("integer"), f2), "tab"))), ve ? ve : v,
                        ωfail, &ge);
                    if (entry_out) *entry_out = ge ? ge : g;
                }
                return nd;
            }
            if (!strcmp(nm, "tab") && t->n == 1) {
                IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$tab";
                IR_t * ve = NULL;
                IR_t * v = lower_arith_val(cx, t->c[0], ωfail, &ve);
                lc_γ_to(v, nd);
                lc_ω_to(v, ωfail);
                ir_operand_push(nd, v);
                if (entry_out) *entry_out = ve ? ve : v;
                return nd;
            }
            if (!strcmp(nm, "tab") && t->n == 2) {
                IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$tab_s";
                IR_t * se = NULL;
                IR_t * sv = term_lval_e(cx, t->c[0], &se);
                IR_t * ve = NULL;
                IR_t * v = lower_arith_val(cx, t->c[1], ωfail, &ve);
                lc_γ_to(sv, ve ? ve : v);
                lc_ω_to(sv, ωfail);
                lc_γ_to(v, nd);
                lc_ω_to(v, ωfail);
                ir_operand_push(nd, sv);
                ir_operand_push(nd, v);
                if (entry_out) *entry_out = se ? se : sv;
                return nd;
            }
            if (!strcmp(nm, "atom_concat") && t->n == 3) {
                IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$atom_concat_at";
                IR_t * to = build(cx, IR_TO, nd, ωfail);
                IR_LIT(to).sval = (char *) "ag";
                IR_t * cnt = build(cx, IR_CALL, to, ωfail);
                IR_LIT(cnt).sval = "$atom_concat_n";
                IR_t * lo = build(cx, IR_LIT_INTEGER, NULL, ωfail);
                IR_LIT(lo).ival = 0;
                IR_t * ale = NULL, * ble = NULL, * cle = NULL;
                IR_t * al = term_lval_e(cx, t->c[0], &ale);
                IR_t * bl = term_lval_e(cx, t->c[1], &ble);
                IR_t * cl = term_lval_e(cx, t->c[2], &cle);
                IR_t * a2e = NULL, * b2e = NULL, * c2e = NULL;
                IR_t * a2 = term_lval_e(cx, t->c[0], &a2e);
                IR_t * b2 = term_lval_e(cx, t->c[1], &b2e);
                IR_t * c2 = term_lval_e(cx, t->c[2], &c2e);
                IR_t * gnl = build(cx, IR_LIT_STRING, NULL, ωfail);
                IR_LIT(gnl).sval = (char *) "atom_concat";
                IR_t * gchk = build(cx, IR_CALL, lo, ωfail);
                IR_LIT(gchk).sval = "$pl_anum_guard3";
                lc_γ_to(gnl, gchk);
                lc_ω_to(gnl, ωfail);
                ir_operand_push(gchk, gnl);
                ir_operand_push(gchk, al);
                ir_operand_push(gchk, bl);
                ir_operand_push(gchk, cl);
                lc_γ_to(al, ble ? ble : bl);
                lc_ω_to(al, ωfail);
                lc_γ_to(bl, cle ? cle : cl);
                lc_ω_to(bl, ωfail);
                lc_γ_to(cl, gnl);
                lc_ω_to(cl, ωfail);
                lc_γ_to(lo, a2e ? a2e : a2);
                lc_ω_to(lo, ωfail);
                lc_γ_to(a2, b2e ? b2e : b2);
                lc_ω_to(a2, ωfail);
                lc_γ_to(b2, c2e ? c2e : c2);
                lc_ω_to(b2, ωfail);
                lc_γ_to(c2, cnt);
                lc_ω_to(c2, ωfail);
                ir_operand_push(cnt, a2);
                ir_operand_push(cnt, b2);
                ir_operand_push(cnt, c2);
                ir_operand_push(to, lo);
                ir_operand_push(to, cnt);
                ir_operand_push(nd, to);
                ir_operand_push(nd, al);
                ir_operand_push(nd, bl);
                ir_operand_push(nd, cl);
                lc_ω_to_β(nd, to);
                if (entry_out) *entry_out = ale ? ale : al;
                return to;
            }
            if (!strcmp(nm, "sub_atom") && t->n == 5) {
                IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$sub_atom_at";
                IR_t * to = build(cx, IR_TO, nd, ωfail);
                IR_LIT(to).sval = (char *) "ag";
                IR_t * ae = NULL;
                IR_t * av = term_e(cx, t->c[0], &ae);
                IR_t * cnt = build(cx, IR_CALL, to, ωfail);
                IR_LIT(cnt).sval = "$sub_atom_n";
                IR_t * av2e = NULL;
                IR_t * av2 = term_e(cx, t->c[0], &av2e);
                IR_t * lo = build(cx, IR_LIT_INTEGER, NULL, ωfail);
                IR_LIT(lo).ival = 0;
                IR_t * bl = NULL, * ll = NULL, * al = NULL, * sl = NULL;
                IR_t * be = NULL, * le = NULL, * ale = NULL, * se = NULL;
                bl = term_lval_e(cx, t->c[1], &be);
                ll = term_lval_e(cx, t->c[2], &le);
                al = term_lval_e(cx, t->c[3], &ale);
                sl = term_lval_e(cx, t->c[4], &se);
                IR_t * gnl = build(cx, IR_LIT_STRING, NULL, ωfail);
                IR_LIT(gnl).sval = (char *) "sub_atom";
                IR_t * gchk = build(cx, IR_CALL, lo, ωfail);
                IR_LIT(gchk).sval = "$pl_anum_guard5";
                lc_γ_to(gnl, gchk);
                lc_ω_to(gnl, ωfail);
                ir_operand_push(gchk, gnl);
                ir_operand_push(gchk, av);
                ir_operand_push(gchk, bl);
                ir_operand_push(gchk, ll);
                ir_operand_push(gchk, al);
                ir_operand_push(gchk, sl);
                lc_γ_to(bl, le ? le : ll);
                lc_ω_to(bl, ωfail);
                lc_γ_to(ll, ale ? ale : al);
                lc_ω_to(ll, ωfail);
                lc_γ_to(al, se ? se : sl);
                lc_ω_to(al, ωfail);
                lc_γ_to(sl, ae ? ae : av);
                lc_ω_to(sl, ωfail);
                lc_γ_to(av, gnl);
                lc_ω_to(av, ωfail);
                lc_γ_to(lo, av2e ? av2e : av2);
                lc_ω_to(lo, ωfail);
                lc_γ_to(av2, cnt);
                lc_ω_to(av2, ωfail);
                ir_operand_push(cnt, av2);
                ir_operand_push(cnt, bl);
                ir_operand_push(cnt, ll);
                ir_operand_push(cnt, al);
                ir_operand_push(cnt, sl);
                ir_operand_push(to, lo);
                ir_operand_push(to, cnt);
                ir_operand_push(nd, av);
                ir_operand_push(nd, to);
                ir_operand_push(nd, bl);
                ir_operand_push(nd, ll);
                ir_operand_push(nd, al);
                ir_operand_push(nd, sl);
                lc_ω_to_β(nd, to);
                if (entry_out) *entry_out = be ? be : bl;
                return to;
            }
            if (!strcmp(nm, "write_to_atom") && t->n == 2 && !pl_file_defines(nm, 2))
                return goal(cx, pl_cc_fnc2("with_output_to", pl_cc_fnc1("atom", (tree_t *) t->c[0]), pl_cc_fnc1("write", (tree_t *) t->c[1])), γnext, ωfail, entry_out);
            if (!strcmp(nm, "format_to_atom") && t->n == 3 && !pl_file_defines(nm, 3))
                return goal(cx, pl_cc_fnc2("with_output_to", pl_cc_fnc1("atom", (tree_t *) t->c[0]), pl_cc_fnc2("format", (tree_t *) t->c[1], (tree_t *) t->c[2])), γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_from_atom") && t->n == 2 && !pl_file_defines(nm, 2))
                return goal(cx, pl_cc_fnc3("atom_to_term", (tree_t *) t->c[0], (tree_t *) t->c[1], pl_cc_freshvar()), γnext, ωfail, entry_out);
            if (!strcmp(nm, "with_output_to") && t->n == 2 && !pl_file_defines(nm, 2)) {
                tree_t * ti = pl_cc_freshvar();
                tree_t * to = pl_cc_freshvar();
                tree_t * th = pl_cc_freshvar();
                tree_t * op = pl_cc_fnc3("$wot_open", ti, to, th);
                tree_t * so = pl_cc_fnc1("set_output", ti);
                tree_t * ev = pl_cc_freshvar();
                tree_t * ev2 = pl_cc_freshvar();
                ev2->v.ival = ev->v.ival;
                tree_t * dis = pl_cc_fnc3("$wot_discard", ti, to, th);
                tree_t * cg = pl_cc_fnc3("catch", pl_cc_fnc1("call", (tree_t *) t->c[1]), ev, pl_cc_fnc2(",", dis, pl_cc_fnc1("throw", ev2)));
                tree_t * cap = pl_cc_fnc4("$wot_capture", ti, to, th, (tree_t *) t->c[0]);
                tree_t * fb = pl_cc_fnc2(",", dis, pl_atom_goal("fail"));
                tree_t * ite = pl_cc_fnc2(";", pl_cc_fnc2("->", cg, cap), fb);
                tree_t * body = pl_cc_fnc2(",", op, pl_cc_fnc2(",", so, ite));
                return goal(cx, body, γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "aggregate_all") && t->n == 3 && !pl_file_defines(nm, 3)) {
                const tree_t * spec = t->c[0];
                const char * kind = NULL;
                const tree_t * tmpl = NULL;
                if (spec && spec->t == TT_FNC && spec->n == 1 && spec->v.sval) {
                    if (!strcmp(spec->v.sval, "count")) {
                        kind = "count";
                        tmpl = spec->c[0];
                    } else if (!strcmp(spec->v.sval, "max")) {
                        kind = "max";
                        tmpl = spec->c[0];
                    } else if (!strcmp(spec->v.sval, "min")) {
                        kind = "min";
                        tmpl = spec->c[0];
                    } else if (!strcmp(spec->v.sval, "sum")) {
                        kind = "sum";
                        tmpl = spec->c[0];
                    } else if (!strcmp(spec->v.sval, "bag")) {
                        kind = "bag";
                        tmpl = spec->c[0];
                    } else if (!strcmp(spec->v.sval, "set")) {
                        kind = "set";
                        tmpl = spec->c[0];
                    }
                } else if (spec && (spec->t == TT_NAME || spec->t == TT_QLIT) && spec->v.sval && !strcmp(spec->v.sval, "count")) {
                    kind = "count";
                    tmpl = pl_cc_ilit(0);
                }
                if (kind && !strcmp(kind, "set")) {
                    tree_t * lv = pl_cc_freshvar();
                    tree_t * fa = pl_cc_fnc3("findall", (tree_t *) tmpl, (tree_t *) t->c[1], lv);
                    tree_t * so = pl_cc_fnc2("sort", lv, (tree_t *) t->c[2]);
                    return goal(cx, pl_cc_fnc2(",", fa, so), γnext, ωfail, entry_out);
                }
                if (kind) {
                    tree_t * lv = pl_cc_freshvar();
                    tree_t * fa = pl_cc_fnc3("findall", (tree_t *) tmpl, (tree_t *) t->c[1], lv);
                    tree_t * rd = pl_cc_fnc3("$aggregate_reduce", (tree_t *) pl_atom_goal(kind), lv, (tree_t *) t->c[2]);
                    return goal(cx, pl_cc_fnc2(",", fa, rd), γnext, ωfail, entry_out);
                }
            }
            if (((!strcmp(nm, "findall") || !strcmp(nm, "bagof") || !strcmp(nm, "setof")) && t->n == 3) || (!strcmp(nm, "findall") && t->n == 4)) {
                int pl_find4 = t->n == 4;
                const char * fin = !strcmp(nm, "findall") ? (pl_find4 ? "$findall_result4" : "$findall_result") : (!strcmp(nm, "bagof") ? "$bagof_result" : "$setof_result");
                const tree_t * gt = pl_caret_body(t->c[1]);
                int wantg = pl_goal_arg_wants_guard(gt);
                int pl_isfind = !strcmp(nm, "findall");
                if (!pl_isfind && gt && gt->t == TT_VAR)
                    return goal(cx, pl_cc_fnc3(!strcmp(nm, "bagof") ? "$bagof_var" : "$setof_var", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2]), γnext, ωfail, entry_out);
                int pl_fvn = pl_isfind ? 1 : pl_var_nodes(t->c[1]) + 1;
                int pl_fv[pl_fvn];
                int pl_nfv = pl_isfind ? 0 : pl_free_vars(t->c[0], t->c[1], pl_fv, pl_fvn);
                if (pl_nfv > 0) {
                    const char * gat = !strcmp(nm, "bagof") ? "$bagof_group_at" : "$setof_group_at";
                    IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                    IR_LIT(nd).sval = (char *) gat;
                    IR_t * to = build(cx, IR_TO, nd, ωfail);
                    IR_LIT(to).sval = (char *) "ag";
                    IR_t * cnt = build(cx, IR_CALL, to, ωfail);
                    IR_LIT(cnt).sval = "$bagof_group_n";
                    IR_t * lo = build(cx, IR_LIT_INTEGER, cnt, ωfail);
                    IR_LIT(lo).ival = 0;
                    IR_t * acc = build(cx, IR_CALL, NULL, ωfail);
                    IR_LIT(acc).sval = "$findall_new";
                    IR_t * add = build(cx, IR_CALL, NULL, ωfail);
                    IR_LIT(add).sval = "$findall_add";
                    IR_t * gentry = NULL;
                    IR_t * gredo = NULL;
                    lc_vec glv2;
                    lc_vec_init(&glv2, (int) sizeof(const tree_t *));
                    collect_conj(gt, &glv2);
                    IR_t * te2 = NULL;
                    IR_t * tv2 = term_e(cx, t->c[0], &te2);
                    IR_t ** wk = (IR_t **) ct_zalloc((size_t) pl_nfv, sizeof(IR_t *));
                    IR_t ** we = (IR_t **) ct_zalloc((size_t) pl_nfv, sizeof(IR_t *));
                    for (int i = 0; i < pl_nfv; i++) { wk[i] = build(cx, IR_VAR, NULL, ωfail); IR_LIT(wk[i]).sval = pl_vname(cx, pl_fv[i]); we[i] = NULL; }
                    IR_t * wce = NULL;
                    IR_t * wc = mkc_node(cx, "$w", pl_nfv, wk, we, &wce);
                    IR_t * pkids[2];
                    IR_t * pkes[2];
                    pkids[0] = wc;
                    pkes[0] = wce;
                    pkids[1] = tv2;
                    pkes[1] = te2;
                    IR_t * pe = NULL;
                    IR_t * pr = mkc_node(cx, "-", 2, pkids, pkes, &pe);
                    IR_t * mark2 = build(cx, IR_BOUND, NULL, ωfail);
                    IR_t * unmk2 = build(cx, IR_UNMARK, lo, ωfail);
                    ir_operand_push(unmk2, mark2);
                    IR_LIT(unmk2).ival = pl_fence_on() ? 5 : 1;
                    IR_t * bc2 = build(cx, IR_CALL, unmk2, ωfail);
                    IR_LIT(bc2).sval = "$ball_pending";
                    IR_t * gcutω2 = build(cx, IR_GOTO, unmk2, unmk2);
                    IR_t * gsaveω2 = cx->cutω;
                    cx->cutω = gcutω2;
                    int gsave_scope2 = cx->cut_scope;
                    cx->cut_scope = ++cx->scope_seq;
                    IR_t * first2 = pl_lower_conj(cx, (const tree_t * const *) glv2.data, glv2.n, pe ? pe : pr, bc2, &gentry, &gredo, NULL);
                    cx->cutω = gsaveω2;
                    cx->cut_scope = gsave_scope2;
                    lc_γ_to(acc, mark2);
                    lc_γ_to(mark2, gentry ? gentry : (first2 ? first2 : unmk2));
                    lc_γ_to(pr, add);
                    lc_ω_to(pr, lo);
                    ir_operand_push(add, acc);
                    ir_operand_push(add, pr);
                    if (gredo) lc_γ_to_β(add, gredo);
                    else lc_γ_to(add, lo);
                    lc_ω_to(add, lo);
                    IR_t ** rk = (IR_t **) ct_zalloc((size_t) pl_nfv, sizeof(IR_t *));
                    IR_t ** rke = (IR_t **) ct_zalloc((size_t) pl_nfv, sizeof(IR_t *));
                    for (int i = 0; i < pl_nfv; i++) { rk[i] = build(cx, IR_VAR_REF, NULL, ωfail); IR_LIT(rk[i]).sval = pl_vname(cx, pl_fv[i]); rke[i] = NULL; }
                    IR_t * wre = NULL;
                    IR_t * wr = mkc_node(cx, "$w", pl_nfv, rk, rke, &wre);
                    IR_t * re2 = NULL;
                    IR_t * rl2 = term_lval_e(cx, t->c[2], &re2);
                    lc_γ_to(rl2, wre ? wre : wr);
                    lc_ω_to(rl2, ωfail);
                    lc_γ_to(wr, acc);
                    lc_ω_to(wr, ωfail);
                    ir_operand_push(cnt, acc);
                    ir_operand_push(to, lo);
                    ir_operand_push(to, cnt);
                    ir_operand_push(nd, acc);
                    ir_operand_push(nd, to);
                    ir_operand_push(nd, wr);
                    ir_operand_push(nd, rl2);
                    lc_ω_to_β(nd, to);
                    ct_drop(wk);
                    ct_drop(we);
                    ct_drop(rk);
                    ct_drop(rke);
                    { IR_t * lge = NULL; IR_t * lg = goal(cx, pl_cc_fnc1("$pl_list_guard", (tree_t *) t->c[2]), re2 ? re2 : rl2, ωfail, &lge); if (entry_out) *entry_out = lge ? lge : lg; }
                    return to;
                }
                IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = (char *) fin;
                IR_t * acc = build(cx, IR_CALL, NULL, ωfail);
                IR_LIT(acc).sval = "$findall_new";
                IR_t * add = build(cx, IR_CALL, NULL, ωfail);
                IR_LIT(add).sval = "$findall_add";
                IR_t * mark = build(cx, IR_BOUND, NULL, ωfail);
                IR_t * unmk = build(cx, IR_UNMARK, nd, ωfail);
                ir_operand_push(unmk, mark);
                IR_LIT(unmk).ival = pl_fence_on() ? 5 : 1;
                IR_t * gentry = NULL;
                IR_t * gredo = NULL;
                lc_vec glv;
                lc_vec_init(&glv, (int) sizeof(const tree_t *));
                collect_conj(pl_isfind ? (wantg ? gt : t->c[1]) : gt, &glv);
                IR_t * te = NULL;
                IR_t * tv = term_e(cx, t->c[0], &te);
                IR_t * bc = build(cx, IR_CALL, unmk, ωfail);
                IR_LIT(bc).sval = "$ball_pending";
                IR_t * gcutω = build(cx, IR_GOTO, unmk, unmk);
                IR_t * gsaveω = cx->cutω;
                cx->cutω = gcutω;
                int gsave_scope = cx->cut_scope;
                cx->cut_scope = ++cx->scope_seq;
                IR_t * first = pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, te ? te : tv, bc, &gentry, &gredo, NULL);
                cx->cutω = gsaveω;
                cx->cut_scope = gsave_scope;
                lc_γ_to(acc, mark);
                lc_γ_to(mark, gentry ? gentry : (first ? first : unmk));
                lc_γ_to(tv, add);
                lc_ω_to(tv, unmk);
                ir_operand_push(add, acc);
                ir_operand_push(add, tv);
                if (gredo) lc_γ_to_β(add, gredo);
                else lc_γ_to(add, unmk);
                lc_ω_to(add, unmk);
                IR_t * re = NULL;
                IR_t * rl = term_lval_e(cx, t->c[2], &re);
                lc_γ_to(rl, nd);
                lc_ω_to(rl, ωfail);
                ir_operand_push(nd, acc);
                ir_operand_push(nd, rl);
                lc_γ_to(rl, acc);
                if (pl_find4) { IR_t * t4e = NULL; IR_t * t4 = term_lval_e(cx, t->c[3], &t4e); lc_γ_to(t4, acc); lc_ω_to(t4, ωfail); lc_γ_to(rl, t4e ? t4e : t4); ir_operand_push(nd, t4); }
                if (wantg) {
                    IR_t * ge = NULL;
                    IR_t * gv = term_lval_e(cx, gt, &ge);
                    IR_t * gchk = build(cx, IR_CALL, re ? re : rl, ωfail);
                    IR_LIT(gchk).sval = "$pl_goal_guard";
                    ir_operand_push(gchk, gv);
                    lc_γ_to(gv, gchk);
                    lc_ω_to(gv, ωfail);
                    { IR_t * lge = NULL; IR_t * lg = goal(cx, pl_cc_fnc1("$pl_list_guard", (tree_t *) t->c[2]), ge ? ge : gv, ωfail, &lge); if (entry_out) *entry_out = lge ? lge : lg; }
                } else {
                    IR_t * lge = NULL;
                    IR_t * lg = goal(cx, pl_cc_fnc1("$pl_list_guard", (tree_t *) t->c[2]), re ? re : rl, ωfail, &lge);
                    if (entry_out) *entry_out = lge ? lge : lg;
                }
                if (pl_find4) {
                    IR_t * hd = entry_out ? *entry_out : nd;
                    IR_t * t4ge = NULL;
                    IR_t * t4g = goal(cx, pl_cc_fnc1("$pl_list_guard", (tree_t *) t->c[3]), hd, ωfail, &t4ge);
                    if (entry_out) *entry_out = t4ge ? t4ge : t4g;
                }
                return nd;
            }
            if ((!strcmp(nm, "between") || !strcmp(nm, "for")) && t->n == 3) {
                int forarg = !strcmp(nm, "for");
                const tree_t * lo_t = forarg ? t->c[1] : t->c[0];
                const tree_t * hi_t = forarg ? t->c[2] : t->c[1];
                const tree_t * vr_t = forarg ? t->c[0] : t->c[2];
                IR_t * nd = build(cx, IR_CALL, γnext, ωfail);
                IR_LIT(nd).sval = "$is_v";
                IR_t * to = build(cx, IR_TO, nd, ωfail);
                IR_LIT(to).sval = (char *) "ag";
                IR_t * loe = NULL;
                IR_t * lo = lower_arith_val(cx, lo_t, ωfail, &loe);
                IR_t * hie = NULL;
                IR_t * hi = lower_arith_val(cx, hi_t, ωfail, &hie);
                IR_t * xe = NULL;
                IR_t * xl = term_lval_e(cx, vr_t, &xe);
                lc_γ_to(xl, loe ? loe : lo);
                lc_ω_to(xl, ωfail);
                lc_γ_to(lo, hie ? hie : hi);
                lc_ω_to(lo, ωfail);
                {
                    IR_t * chk = build(cx, IR_CALL, to, ωfail);
                    IR_LIT(chk).sval = "$pl_between_guard";
                    ir_operand_push(chk, lo);
                    ir_operand_push(chk, hi);
                    ir_operand_push(chk, xl);
                    lc_γ_to(hi, chk);
                    lc_ω_to(hi, ωfail);
                }
                ir_operand_push(to, lo);
                ir_operand_push(to, hi);
                ir_operand_push(nd, xl);
                ir_operand_push(nd, to);
                lc_ω_to_β(nd, to);
                if (entry_out) *entry_out = xe ? xe : xl;
                return to;
            }
            if (!strcmp(nm, "current_output") && t->n == 1) return pl_leaf_guarded1(cx, "$current_output", "$pl_curstream_guard", t, γnext, ωfail, entry_out);
            if (!strcmp(nm, "current_input") && t->n == 1) return pl_leaf_guarded1(cx, "$current_input", "$pl_curstream_guard", t, γnext, ωfail, entry_out);
            if (!strcmp(nm, "set_output") && t->n == 1) return pl_leaf_guarded1_dir(cx, "$set_output", "$pl_stream_guard", "output", t, γnext, ωfail, entry_out);
            if (!strcmp(nm, "set_input") && t->n == 1) return pl_leaf_guarded1_dir(cx, "$set_input", "$pl_stream_guard", "input", t, γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_term") && t->n == 2 && pl_tree_is_nil(t->c[1])) return pl_leaf_lv(cx, "$read", t, 1, γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_term") && t->n == 2) return pl_leaf_lv(cx, "$read_term_opts", t, 2, γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_term") && t->n == 3 && pl_tree_is_nil(t->c[2])) return pl_leaf_lv(cx, "$read_s", t, 2, γnext, ωfail, entry_out);
            if (!strcmp(nm, "read_term") && t->n == 3) return pl_leaf_lv(cx, "$read_term_opts_s", t, 3, γnext, ωfail, entry_out);
            if (!strcmp(nm, "$db_decls") && t->n >= 1) return pl_leaf_lv(cx, "$db_decls", t, t->n, γnext, ωfail, entry_out);
            if ((!strcmp(nm, "assertz") || !strcmp(nm, "assert") || !strcmp(nm, "asserta")) && t->n == 1) {
                int ar = 0;
                const char * pn;
                { const tree_t * bad = pl_clause_ill_typed(t->c[0]); if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 1), γnext, ωfail, entry_out); }
                pn = pl_head_key(t->c[0], &ar);
                if (!pn)
                    return goal(cx,
                    pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal("assert")),
                    pl_cc_fnc1(!strcmp(nm, "asserta") ? "$db_asserta_t" : "$db_assertz_t", (tree_t *) t->c[0])), γnext, ωfail, entry_out);
                if (pl_pi_is_static_builtin(pn, ar) || !pl_db_owned(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_leaf2(cx, !strcmp(nm, "asserta") ? "$db_asserta" : "$db_assertz", pl_dyn_index_or_add(pn, ar), t->c[0], γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    {
                        IR_t * first = se ? se : le;
                        if (pl_tree_has_var(t->c[0])) {
                            int hb = t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, ":-") && t->c[0]->n == 2 && pl_tree_has_var(t->c[0]->c[1]);
                            IR_t * ge = NULL;
                            pl_db_leaf2_tree(cx, "$db_t_guard", t->c[0], pl_atom_goal(hb ? "assert" : "assert_cyc"), first, ωfail, &ge);
                            if (ge) first = ge;
                        }
                        if (entry_out) *entry_out = first;
                    }
                    return nd;
                }
            }
            if ((!strcmp(nm, "asserta") || !strcmp(nm, "assertz") || !strcmp(nm, "assert")) && t->n == 2) {
                { const tree_t * bad = pl_clause_ill_typed(t->c[0]); if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 2), γnext, ωfail, entry_out); }
                return goal(cx,
                    pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[1], (tree_t *) pl_atom_goal("clref_out")),
                    pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal("assert")),
                    pl_cc_fnc2(!strcmp(nm, "asserta") ? "$db_asserta_r" : "$db_assertz_r", (tree_t *) t->c[0], (tree_t *) t->c[1]))), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "clause") && t->n == 3) {
                {
                    const tree_t * bad = pl_tree_number(t->c[0]);
                    if (!bad) bad = pl_tree_number(t->c[1]);
                    if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 3), γnext, ωfail, entry_out);
                }
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum_ref(cx, pl_clause_target(t->c[0], t->c[1]), t->c[2], γnext, ωfail, &le);
                    IR_t * ge = NULL;
                    pl_db_leaf2_tree(cx, "$db_t_guard", t->c[2], pl_atom_goal("clref_opt"), le, ωfail, &ge);
                    if (entry_out) *entry_out = ge ? ge : le;
                    return nd;
                }
            }
            if (!strcmp(nm, "erase") && t->n == 1) {
                return goal(cx, pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal("clref_in")), pl_cc_fnc1("$db_erase_ref", (tree_t *) t->c[0])), γnext, ωfail,
                    entry_out);
            }
            if (!strcmp(nm, "retract") && t->n == 1) {
                int ar = 0;
                const char * pn;
                { const tree_t * bad = pl_clause_ill_typed(t->c[0]); if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 1), γnext, ωfail, entry_out); }
                pn = pl_head_key(t->c[0], &ar);
                if (!pn) {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum(cx, -1, pl_clause_target(t->c[0], NULL), 1, γnext, ωfail, &le);
                    IR_t * ge = NULL;
                    pl_db_leaf2_tree(cx, "$db_t_guard", t->c[0], pl_atom_goal("retract"), le, ωfail, &ge);
                    if (entry_out) *entry_out = ge ? ge : le;
                    return nd;
                }
                if (pl_pi_is_static_builtin(pn, ar) || !pl_db_owned(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum(cx, pl_dyn_index_or_add(pn, ar), pl_clause_target(t->c[0], NULL), 1, γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    if (entry_out) *entry_out = se ? se : le;
                    return nd;
                }
            }
            if (!strcmp(nm, "retractall") && t->n == 1) {
                int ar = 0;
                const char * pn;
                { const tree_t * bad = pl_tree_number(t->c[0]); if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 1), γnext, ωfail, entry_out); }
                pn = pl_head_key(t->c[0], &ar);
                if (!pn)
                    return goal(cx, pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal("retractall")), pl_cc_fnc1("$db_retractall_t", (tree_t *) t->c[0])), γnext,
                    ωfail, entry_out);
                if (pl_pi_is_static_builtin(pn, ar) || !pl_db_owned(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_leaf2(cx, "$db_retractall", pl_dyn_index_or_add(pn, ar), t->c[0], γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    if (entry_out) *entry_out = se ? se : le;
                    return nd;
                }
            }
            if (!strcmp(nm, "abolish") && t->n == 1) {
                int ar = 0;
                const char * pn;
                { tree_t * bad = pl_cc_spec_ill_typed(t->c[0], nm); if (bad) return goal(cx, bad, γnext, ωfail, entry_out); }
                pn = pl_spec_key(t->c[0], &ar);
                if (!pn)
                    return goal(cx, pl_cc_fnc2(",", pl_cc_fnc2("$db_t_guard", (tree_t *) t->c[0], (tree_t *) pl_atom_goal("abolish")), pl_cc_fnc1("$db_abolish_t", (tree_t *) t->c[0])), γnext, ωfail,
                    entry_out);
                if (pl_pi_is_static_builtin(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                if (!pl_db_owned(pn, ar)) return goal(cx, pl_cc_perm_static(pn, ar), γnext, ωfail, entry_out);
                {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_leaf1(cx, "$db_abolish", pl_dyn_index_or_add(pn, ar), γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    if (entry_out) *entry_out = se ? se : le;
                    return nd;
                }
            }
            if ((!strcmp(nm, "nb_setval") || !strcmp(nm, "nb_getval") || !strcmp(nm, "b_setval") || !strcmp(nm, "b_getval")) && t->n == 2) {
                const char * kk = pl_nb_key(t->c[0]);
                if (!kk) {
                    if (!strcmp(nm, "nb_setval")) return pl_db_leaf2_tree(cx, "$nb_setval", t->c[0], t->c[1], γnext, ωfail, entry_out);
                    if (!strcmp(nm, "b_setval")) return pl_db_leaf2_tree(cx, "$b_setval", t->c[0], t->c[1], γnext, ωfail, entry_out);
                    {
                        IR_t * body_entry = NULL;
                        IR_t * nd = pl_nb_leaf_lv_tree(cx, "$nb_getval", t->c[0], t->c[1], γnext, ωfail, &body_entry);
                        IR_t * guard_entry = NULL;
                        pl_db_leaf2_tree(cx, "$pl_nb_getval_guard", t->c[0], t->c[0], body_entry, ωfail, &guard_entry);
                        if (entry_out) *entry_out = guard_entry;
                        return nd;
                    }
                }
                {
                    int k = pl_dyn_index_or_add(kk, -1);
                    if (k < 0) pl_refuse("global variable needs a root cell but the 64 compile-time root cells are exhausted --", kk, 10);
                    if (!strcmp(nm, "nb_setval")) return pl_db_leaf2(cx, "$nb_setval", k, t->c[1], γnext, ωfail, entry_out);
                    if (!strcmp(nm, "b_setval")) return pl_db_leaf2(cx, "$b_setval", k, t->c[1], γnext, ωfail, entry_out);
                    {
                        IR_t * body_entry = NULL;
                        IR_t * nd = pl_nb_leaf_lv(cx, "$nb_getval", k, t->c[1], γnext, ωfail, &body_entry);
                        tree_t * kt = ast_node_new(TT_QLIT);
                        kt->v.sval = (char *) kk;
                        IR_t * guard_entry = NULL;
                        pl_db_leaf2(cx, "$pl_nb_getval_guard", k, kt, body_entry, ωfail, &guard_entry);
                        if (entry_out) *entry_out = guard_entry;
                        return nd;
                    }
                }
            }
            if (!strcmp(nm, "clause") && t->n == 2) {
                int ar = 0;
                const char * pn;
                {
                    const tree_t * bad = pl_tree_number(t->c[0]);
                    if (!bad) bad = pl_tree_number(t->c[1]);
                    if (bad) return goal(cx, pl_cc_type_error("callable", bad, nm, 2), γnext, ωfail, entry_out);
                }
                pn = pl_head_key(t->c[0], &ar);
                if (!pn) {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum(cx, -1, pl_clause_target(t->c[0], t->c[1]), 0, γnext, ωfail, &le);
                    IR_t * ge = NULL;
                    pl_db_leaf2_tree(cx, "$db_t_guard", pl_clause_target(t->c[0], t->c[1]), pl_atom_goal("clause"), le, ωfail, &ge);
                    if (entry_out) *entry_out = ge ? ge : le;
                    return nd;
                }
                if (pl_pi_is_static_builtin(pn, ar)) return goal(cx, pl_cc_perm_access(pn, ar), γnext, ωfail, entry_out);
                if (pl_db_owned(pn, ar)) {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum(cx, pl_dyn_index_or_add(pn, ar), pl_clause_target(t->c[0], t->c[1]), 0, γnext, ωfail, &le);
                    IR_t * se = NULL;
                    pl_db_seed_file(cx, pn, ar, le, ωfail, &se);
                    if (entry_out) *entry_out = se ? se : le;
                    return nd;
                }
                if (!pl_file_defines(pn, ar)) {
                    IR_t * le = NULL;
                    IR_t * nd = pl_db_enum(cx, -1, pl_clause_target(t->c[0], t->c[1]), 0, γnext, ωfail, &le);
                    IR_t * ge = NULL;
                    pl_db_leaf2_tree(cx, "$db_t_guard", pl_clause_target(t->c[0], t->c[1]), pl_atom_goal("clause"), le, ωfail, &ge);
                    if (entry_out) *entry_out = ge ? ge : le;
                    return nd;
                }
                {
                    char key[fmt_len("%s/%d", pn, ar)];
                    snprintf(key, sizeof key, "%s/%d", pn, ar);
                    const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
                    int k = pl_dyn_index_or_add(pn, ar);
                    if (k < 0 || !ch) pl_refuse("clause/2 on a file-defined predicate needs a root cell and the 64 compile-time root cells are exhausted --", pn, 10);
                    {
                        IR_t * enum_entry = NULL;
                        IR_t * to = pl_db_enum(cx, k, pl_clause_target(t->c[0], t->c[1]), 0, γnext, ωfail, &enum_entry);
                        IR_t * next = enum_entry;
                        IR_t * first = NULL;
                        int n = (ch->t == TT_CHOICE) ? ch->n : 1;
                        for (int i = n - 1; i >= 0; i--) {
                            const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[i] : ch;
                            IR_t * se = NULL;
                            pl_db_leaf_seed(cx, k, i, pl_static_clause_term(cl, pn, ar), next, ωfail, &se);
                            next = se;
                            first = se;
                        }
                        {
                            IR_t * body_first = first ? first : enum_entry;
                            IR_t * ge = NULL;
                            pl_db_leaf2_tree(cx, "$db_t_guard", pl_clause_target(t->c[0], t->c[1]), pl_atom_goal("clause"), body_first, ωfail, &ge);
                            if (entry_out) *entry_out = ge ? ge : body_first;
                            return to;
                        }
                    }
                }
            }
            {
                static const struct {
                    const char * n;
                    int a;
                    const char * k;
                    const char * leaf;
                } io[] = { { "put_code", 1, "put_code1", "$put_code" }, { "put_code", 2, "put_code2", "$put_code_s" }, { "put_char", 1, "put_char1", "$put_char" }, { "put_char", 2, "put_char2",
                    "$put_char_c_s" }, { "get_code", 1, "in_code1", "$get_code" }, { "get_code", 2, "in_code2", "$get_code_s" }, { "peek_code", 1, "in_code1", "$peek_code" }, { "peek_code", 2,
                    "in_code2", "$peek_code_s" }, { "get_char", 1, "in_char1", "$get_char" }, { "get_char", 2, "in_char2", "$get_char_s" }, { "peek_char", 1, "in_char1", "$peek_char" }, { "peek_char",
                    2, "in_char2", "$peek_char_s" }, { "get_byte", 1, "in_byte1", "$get_byte" }, { "get_byte", 2, "in_byte2", "$get_byte_s" }, { "peek_byte", 1, "in_byte1", "$peek_byte" },
                    { "peek_byte", 2, "in_byte2", "$peek_byte_s" }, { "put", 1, "put_code1", "$put_code" }, { "get", 1, "in_code1", "$get_edin" }, { "get0", 1, "in_code1", "$get_code" }, { "skip", 1,
                    "put_code1", "$skip" }, { NULL, 0, NULL, NULL } };
                for (int i = 0; io[i].n; i++) if (t->n == io[i].a && !strcmp(nm, io[i].n) && !pl_file_defines(nm, t->n)) {
                    tree_t * raw = ast_node_new(TT_FNC);
                    raw->v.sval = (char *) io[i].leaf;
                    for (int k = 0; k < t->n; k++) ast_push(raw, (tree_t *) t->c[k]);
                    return goal(cx, pl_cc_fnc2(",", pl_cc_fnc2("$pl_ioarg", (tree_t *) pl_atom_goal(io[i].k), (tree_t *) t->c[t->n - 1]), raw), γnext, ωfail, entry_out);
                }
            }
            if (!strcmp(nm, "current_prolog_flag") && t->n == 2 && t->c[0] && t->c[0]->t == TT_VAR && !pl_file_defines(nm, 2)) {
                extern const char *rt_pl_flag_name(int);
                tree_t * alt = (tree_t *) 0;
                for (int i = 63; i >= 0; i--) {
                    const char * fnm = rt_pl_flag_name(i);
                    if (!fnm) continue;
                    tree_t * u = pl_cc_fnc2("=", (tree_t *) t->c[0], (tree_t *) pl_atom_goal(fnm));
                    alt = alt ? pl_cc_fnc2(";", u, alt) : u;
                }
                if (alt)
                    return goal(cx,
                    pl_cc_fnc2(",", pl_cc_ite(pl_cc_fnc1("var", (tree_t *) t->c[0]), alt, (tree_t *) pl_atom_goal("true")), pl_cc_fnc2("$current_prolog_flag", (tree_t *) t->c[0], (tree_t *) t->c[1])),
                    γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "current_op") && t->n == 3 && !pl_file_defines(nm, 3))
                return goal(cx,
                pl_cc_fnc2(",", pl_cc_fnc3("$pl_op_check", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2]),
                pl_cc_gen2_desc("$pl_op_count", "$pl_op_nth", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2])), γnext, ωfail, entry_out);
            if (!strcmp(nm, "current_stream") && t->n == 3 && !pl_file_defines(nm, 3))
                return goal(cx, pl_cc_gen2("$pl_cs_count", "$pl_cs_nth", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) t->c[2]), γnext, ωfail, entry_out);
            if (!strcmp(nm, "stream_property") && t->n == 2 && !pl_file_defines(nm, 2))
                return goal(cx,
                pl_cc_fnc2(",", pl_cc_fnc2("$pl_sp_check", (tree_t *) t->c[0], (tree_t *) t->c[1]), pl_cc_gen2("$pl_sp_count", "$pl_sp_nth", (tree_t *) t->c[0], (tree_t *) t->c[1], (tree_t *) 0)),
                γnext, ωfail, entry_out);
            if (!strcmp(nm, "predicate_property") && t->n == 2 && !pl_file_defines(nm, 2)) {
                const tree_t * h = t->c[0];
                const char * pn = (h && (h->t == TT_FNC || h->t == TT_QLIT || h->t == TT_NAME)) ? h->v.sval : (const char *) 0;
                int ar = (h && h->t == TT_FNC) ? h->n : 0;
                const tree_t * pp = t->c[1];
                if (pl_tree_number(h)) return goal(cx, pl_cc_type_error("callable", h, nm, 2), γnext, ωfail, entry_out);
                if (pp && (pp->t == TT_QLIT || pp->t == TT_NAME || pp->t == TT_FNC) && pp->v.sval && !pl_name_in(pp->v.sval, pl_pred_props))
                    return goal(cx, pl_cc_throw_ar(pl_cc_fnc2("domain_error", (tree_t *) pl_atom_goal("predicate_property"), (tree_t *) pp), nm, 2), γnext, ωfail, entry_out);
                if (!pn) return goal(cx, pl_pp_runtime(h, pp), γnext, ωfail, entry_out);
                {
                    int dyn = (pl_dyn_index(pn, ar) >= 0) || pl_decl_dyn_is(pn, ar) || pl_rt_is_dynamic(pn, ar);
                    int def = dyn || pl_file_defines(pn, ar);
                    const char * meta = def ? (const char *) 0 : pl_meta_template(pn, ar);
                    int bi = !def && (meta || pl_det_leaf_name_wired(pn) || pl_rung_of(pn) != 0);
                    const tree_t * props[7];
                    int np = 0;
                    const tree_t * um = def ? pl_decl_meta_of(pn, ar) : (const tree_t *) 0;
                    int portray1 = !strcmp(pn, "portray") && ar == 1;
                    const tree_t * props_pt[2];
                    int npt = 0;
                    if (portray1) { if (!dyn) props_pt[npt++] = pl_atom_goal("dynamic"); props_pt[npt++] = pl_atom_goal("multifile"); }
                    if (dyn) props[np++] = pl_atom_goal("dynamic");
                    else if (def && !portray1) props[np++] = pl_atom_goal("static");
                    if (def) props[np++] = pl_atom_goal("defined");
                    if (bi) { props[np++] = pl_atom_goal("built_in"); props[np++] = pl_atom_goal("defined"); }
                    if (!portray1 && pl_decl_mf_is(pn, ar)) props[np++] = pl_atom_goal("multifile");
                    if (meta) {
                        tree_t * tm = ast_node_new(TT_FNC);
                        tm->v.sval = (char *) pn;
                        for (int i = 0; i < ar; i++) ast_push(tm, meta[i] >= '0' && meta[i] <= '9' ? pl_cc_ilit(meta[i] - '0') : (tree_t *) pl_atom_goal(meta[i] == '^' ? "^" : "?"));
                        props[np++] = pl_cc_fnc1("meta_predicate", tm);
                    }
                    if (um) {
                        tree_t * tm = ast_node_new(TT_FNC);
                        tm->v.sval = (char *) pn;
                        for (int i = 0; i < ar; i++) {
                            const tree_t * a = um->c[i];
                            ast_push(tm, (a && (a->t == TT_NAME || a->t == TT_QLIT) && a->v.sval && !strcmp(a->v.sval, "*")) ? (tree_t *) pl_atom_goal("?") : (tree_t *) a);
                        }
                        props[np++] = pl_cc_fnc1("meta_predicate", tm);
                    }
                    for (int i = 0; i < npt && np < 7; i++) props[np++] = props_pt[i];
                    if (!np) return goal(cx, pl_pp_runtime(h, pp), γnext, ωfail, entry_out);
                    {
                        tree_t * alt = pl_cc_fnc2("=", (tree_t *) t->c[1], (tree_t *) props[np - 1]);
                        for (int i = np - 2; i >= 0; i--) alt = pl_cc_fnc2(";", pl_cc_fnc2("=", (tree_t *) t->c[1], (tree_t *) props[i]), alt);
                        return goal(cx, alt, γnext, ωfail, entry_out);
                    }
                }
            }
            if (!strcmp(nm, "current_predicate") && t->n == 1 && !pl_file_defines(nm, 1)) {
                tree_t * s = (tree_t *) t->c[0];
                tree_t * n;
                tree_t * a;
                tree_t * body;
                { tree_t * bad = pl_cc_spec_ill_typed(s, nm); if (bad) return goal(cx, bad, γnext, ωfail, entry_out); }
                if (s && s->t == TT_FNC && s->v.sval && !strcmp(s->v.sval, "/") && s->n == 2) {
                    n = (tree_t *) s->c[0];
                    a = (tree_t *) s->c[1];
                    body = pl_cc_gen2("$pl_cp_count", "$pl_cp_nth", n, a, (tree_t *) 0);
                } else {
                    tree_t * n2;
                    tree_t * a2;
                    n = pl_cc_freshvar();
                    a = pl_cc_freshvar();
                    n2 = pl_cc_freshvar();
                    a2 = pl_cc_freshvar();
                    n2->v.ival = n->v.ival;
                    a2->v.ival = a->v.ival;
                    body = pl_cc_fnc2(",", pl_cc_fnc2("=", s, pl_cc_fnc2("/", n2, a2)), pl_cc_gen2("$pl_cp_count", "$pl_cp_nth", n, a, (tree_t *) 0));
                }
                return goal(cx, pl_cc_fnc2(",", pl_cc_fnc1("$pl_cp_guard", s), body), γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "dynamic") && t->n >= 1) {
                tree_t * g = pl_cc_fnc1("$pl_dynamic", (tree_t *) t->c[0]);
                for (int i = 1; i < t->n; i++) g = pl_cc_fnc2(",", g, pl_cc_fnc1("$pl_dynamic", (tree_t *) t->c[i]));
                return goal(cx, g, γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "char_conversion") && t->n == 2) {
                (void) pl_dyn_index_or_add("$pl_cconv", 2);
                tree_t * in = (tree_t *) t->c[0];
                tree_t * out = (tree_t *) t->c[1];
                tree_t * store = pl_cc_fnc1("asserta", pl_cc_fnc2("$pl_cconv", in, out));
                tree_t * out_ck = pl_cc_ite(pl_cc_ischar(out), store, pl_cc_throw(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("character"), out), "char_conversion"));
                tree_t * out_gate = pl_cc_ite(pl_cc_fnc1("var", out), pl_cc_throw((tree_t *) pl_atom_goal("instantiation_error"), "char_conversion"), out_ck);
                tree_t * in_ck = pl_cc_ite(pl_cc_ischar(in), out_gate, pl_cc_throw(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("character"), in), "char_conversion"));
                tree_t * rewrite = pl_cc_ite(pl_cc_fnc1("var", in), pl_cc_throw((tree_t *) pl_atom_goal("instantiation_error"), "char_conversion"), in_ck);
                return goal(cx, rewrite, γnext, ωfail, entry_out);
            }
            if (!strcmp(nm, "current_char_conversion") && t->n == 2) {
                (void) pl_dyn_index_or_add("$pl_cconv", 2);
                tree_t * in = (tree_t *) t->c[0];
                tree_t * out = (tree_t *) t->c[1];
                tree_t * lookup = pl_cc_fnc2("$pl_cconv", in, out);
                tree_t * bound_path =
                    pl_cc_ite(pl_cc_ischar(in), pl_cc_ite(lookup, (tree_t *) pl_atom_goal("true"), pl_cc_fnc2("=", out, in)),
                    pl_cc_throw(pl_cc_fnc2("type_error", (tree_t *) pl_atom_goal("character"), in), "current_char_conversion"));
                tree_t * rewrite = pl_cc_ite(pl_cc_fnc1("var", in), lookup, bound_path);
                return goal(cx, rewrite, γnext, ωfail, entry_out);
            }
            {
                const char * ls = pl_det_leaf_sym(nm, t->n);
                if (ls && !strcmp(nm, "print") && t->n == 1 && (pl_file_defines("portray", 1) || pl_db_owned("portray", 1))) ls = (const char *) 0;
                if (ls && (pl_file_defines("portray", 1) || pl_db_owned("portray", 1))) {
                    if (!strcmp(nm, "print") && t->n == 2) return goal(cx, pl_cc_print_via_format((tree_t *) t->c[0], (tree_t *) t->c[1]), γnext, ωfail, entry_out);
                    if (!strcmp(nm, "write_term") && t->n == 2 && pl_opts_are_portray_only(t->c[1]))
                        return goal(cx, pl_cc_print_via_format((tree_t *) 0, (tree_t *) t->c[0]), γnext, ωfail, entry_out);
                    if (!strcmp(nm, "write_term") && t->n == 3 && pl_opts_are_portray_only(t->c[2]))
                        return goal(cx, pl_cc_print_via_format((tree_t *) t->c[0], (tree_t *) t->c[1]), γnext, ωfail, entry_out);
                }
                if (ls) {
                    const char * gs = pl_anum_guard_sym(nm, t->n);
                    IR_t * ne = NULL;
                    IR_t * nd = gs ? pl_leaf_lv_guarded(cx, ls, gs, nm, t, t->n, γnext, ωfail, &ne) : pl_leaf_lv(cx, ls, t, t->n, γnext, ωfail, &ne);
                    if (pl_curout_writer(nm, t->n)) return pl_curout_text_guard(cx, nd, ne, ωfail, entry_out);
                    if (entry_out) *entry_out = ne;
                    return nd;
                }
            }
            { extern int g_rt_fragment_emit; int r = pl_rung_of(nm); if (r && !g_rt_fragment_emit && !pl_file_defines(nm, t->n) && !pl_det_leaf_name_wired(nm)) pl_refuse("builtin", nm, r); }
            return pl_user_call(cx, nm, t, t->n, γnext, ωfail, entry_out);
        }
        case TT_QLIT:
        case TT_NAME:
        {
            const char * nm = t->v.sval ? t->v.sval : "?";
            if (!strcmp(nm, "true")) return build(cx, IR_SUCCEED, γnext, ωfail);
            if (!strcmp(nm, "fail") || !strcmp(nm, "false")) return build(cx, IR_GOTO, ωfail, ωfail);
            if (!strcmp(nm, "nl")) {
                IR_t * ne = NULL;
                IR_t * nd = pl_leaf(cx, "$nl", t, 0, γnext, ωfail, &ne);
                IR_t * ge = NULL;
                IR_t * g = goal(cx, pl_cc_fnc2("$pl_ioarg", (tree_t *) pl_atom_goal("put_nl1"), (tree_t *) pl_atom_goal("[]")), ne ? ne : nd, ωfail, &ge);
                if (entry_out) *entry_out = ge ? ge : g;
                return nd;
            }
            if (!strcmp(nm, "repeat") && !pl_file_defines(nm, 0)) return goal(cx, pl_cc_fnc3("between", pl_cc_ilit(1), pl_cc_ilit(9223372036854775807LL), pl_cc_freshvar()), γnext, ωfail, entry_out);
            if (!strcmp(nm, "!")) { IR_t * cn = build(cx, IR_CUT, γnext, cx->cutω); if (cx->cutω != cx->clause_cutω) IR_LIT(cn).ival = 1; return cn; }
            { const char * ls = pl_det_leaf_sym(nm, 0); if (ls) return pl_leaf_lv(cx, ls, t, 0, γnext, ωfail, entry_out); }
            { extern int g_rt_fragment_emit; int r = pl_rung_of(nm); if (r && !g_rt_fragment_emit && !pl_file_defines(nm, 0) && !pl_det_leaf_name_wired(nm)) pl_refuse("builtin", nm, r); }
            return pl_user_call(cx, nm, t, 0, γnext, ωfail, entry_out);
        }
        case TT_CUT:
        { IR_t * cn = build(cx, IR_CUT, γnext, cx->cutω); IR_LIT(cn).ival = cx->cut_scope; return cn; }
        case TT_UNIFY:
        { IR_t * e = NULL; IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e); if (entry_out) *entry_out = e ? e : nd; return nd; }
        case TT_IF:
        return pl_lower_ite(cx, t->c[0], (t->n > 1) ? t->c[1] : NULL, (t->n > 2) ? t->c[2] : NULL, γnext, ωfail, entry_out);
        case TT_PROGRAM:
        return pl_lower_conj(cx, (const tree_t * const *) t->c, t->n, γnext, ωfail, entry_out, NULL, NULL);
        case TT_VAR:
        return pl_meta_call_dyn(cx, t, NULL, 0, γnext, ωfail, entry_out);
        default:
        return build(cx, IR_SUCCEED, γnext, ωfail);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_new_proc(const char * name, int nparams, int bb_idx) {
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name = ct_strdup(name);
    g_stage2.proc_table[pi].proc = NULL;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].bb_idx = bb_idx;
    g_stage2.proc_table[pi].nparams = nparams;
    g_stage2.proc_table[pi].is_generator = 1;
    {
        int mi = g_stage2.module_registry.nmod - 1;
        if (mi >= 0) { g_stage2.module_registry.mods[mi].nprocs++; if (!strcmp(name, "main") && g_stage2.module_registry.main_mod < 0) g_stage2.module_registry.main_mod = mi; }
    }
    return pi;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_graph_stamp(IR_graph_t * g, int arity, int maxlocal, const unsigned char * aliased, int naliased) {
    g->body_root = NULL;
    {
        extern int dop_direct_leaf_known(const char *, int);
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (nd && nd->op == IR_CALL && nd->sval && nd->sval[0] == '$' && dop_direct_leaf_known(nd->sval, nd->n_operands)) nd->seal = IR_SEAL_CALL_DET_LEAF;
        }
    }
    g->nparams = arity;
    if (arity > 0) { g->pnames = (const char **) ct_zalloc((size_t) arity, sizeof(const char *)); for (int i = 0; i < arity; i++) g->pnames[i] = pl_param_name(i); }
    int nl = 0;
    if (maxlocal >= 0) {
        g->lnames = (const char **) ct_zalloc((size_t)(maxlocal + 1), sizeof(const char *));
        for (int k = 0; k <= maxlocal; k++) {
            const char * nm = pl_var_name(k);
            int used = !(aliased && k < naliased && aliased[k]);
            for (int i = 0; i < g->n && !used; i++) {
                const IR_t * nd = g->all[i];
                if (nd && (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_UNIFY_FIRST || nd->op == IR_UNIFY_VALUE) && IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, nm)) used = 1;
            }
            if (used) g->lnames[nl++] = nm;
        }
        g->nlocals = nl;
    }
    g->nslots = arity + nl + 8;
    g->resumable_callable = 1;
    g->deterministic = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_alt_alloc(IR_graph_t * g, int nc) {
    g->alt_entry = (IR_t **) ct_zalloc((size_t) nc, sizeof(IR_t *));
    g->alt_ret = (IR_t **) ct_zalloc((size_t) nc, sizeof(IR_t *));
    g->alt_redo = (IR_t **) ct_zalloc((size_t) nc, sizeof(IR_t *));
    g->n_alts = nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * pl_body_graph(const tree_t * const * gl, int ng) {
    IR_graph_t * g = IR_alloc(4096);
    lcx_t cx;
    memset(&cx, 0, sizeof cx);
    cx.g = g;
    cx.tω = NULL;
    cx.cutω = NULL;
    cx.clause_cutω = NULL;
    cx.cut_scope = 0;
    cx.scope_seq = 0;
    cx.meta_redo = NULL;
    cx.meta_redo_set = 0;
    cx.stmt_depth = 1;
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * step = build(&cx, IR_FAIL, NULL, NULL);
    cx.cutω = fail;
    cx.clause_cutω = fail;
    IR_t * entry = NULL;
    IR_t * redo = NULL;
    IR_t * tnode = NULL;
    int maxlocal = -1;
    for (int i = 0; i < ng; i++) maxlocal = max_var_slot(gl[i], maxlocal);
    int fresh_saved = g_pl_fresh_next;
    g_pl_fresh_next = maxlocal + 1;
    if (pl_trace_wanted()) for (int i = 0; i < ng; i++) pl_trace_number_goals(gl[i]);
    IR_t * first = pl_lower_conj(&cx, gl, ng, succeed, step, &entry, &redo, &tnode);
    if (tnode && tnode->op == IR_CALL_PROC_STAGED && !pl_trace_wanted()) tnode->seal = PL_SEAL_TAIL;
    maxlocal = g_pl_fresh_next - 1;
    g_pl_fresh_next = fresh_saved;
    g->entry = entry ? entry : (first ? first : succeed);
    pl_alt_alloc(g, 1);
    g->alt_entry[0] = g->entry;
    g->alt_ret[0] = succeed;
    g->alt_redo[0] = redo;
    g->alt_fail = step;
    pl_graph_stamp(g, 0, maxlocal, (const unsigned char *)0, 0);
    (void) fail;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * pl_pred_graph(const tree_t * ch, const char * key) {
    int nc = (ch->t == TT_CHOICE) ? ch->n : 1;
    if (nc < 1) nc = 1;
    IR_graph_t * g = IR_alloc(1024 + 1024 * nc);
    lcx_t cx;
    memset(&cx, 0, sizeof cx);
    cx.g = g;
    cx.cur_key = key;
    cx.tω = NULL;
    cx.cutω = NULL;
    cx.clause_cutω = NULL;
    cx.cut_scope = 0;
    cx.scope_seq = 0;
    cx.meta_redo = NULL;
    cx.meta_redo_set = 0;
    const char * trace_key = key ? ct_strdup(key) : NULL;
    {
        extern int pl_prelude_defines(const char *, int);
        const char * slash = key ? strrchr(key, '/') : NULL;
        if (slash) {
            size_t nl = (size_t)(slash - key);
            char nmbuf[nl + 1];
            memcpy(nmbuf, key, nl);
            nmbuf[nl] = 0;
            {
                int ar = atoi(slash + 1);
                if (pl_prelude_defines(nmbuf, ar) || pl_db_owned(nmbuf, ar) || (pl_pi_is_static_builtin(nmbuf, ar) && !pl_file_defines(nmbuf, ar))) { cx.stmt_depth = 1; trace_key = NULL; }
            }
        }
    }
    IR_t * step = build(&cx, IR_FAIL, NULL, NULL);
    cx.cutω = build(&cx, IR_FAIL, NULL, NULL);
    cx.clause_cutω = cx.cutω;
    pl_alt_alloc(g, nc);
    g->alt_fail = step;
    int arity = -1;
    int maxlocal = -1;
    int fresh_saved = g_pl_fresh_next;
    for (int k = 0; k < nc; k++) { const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[k] : ch; if (cl) { int ml = max_var_slot(cl, -1); if (ml > maxlocal) maxlocal = ml; } }
    int nva = maxlocal + 1 > 0 ? maxlocal + 1 : 1;
    int valias[nva];
    unsigned char aliased[nva];
    memset(aliased, 0, sizeof aliased);
    cx.valias = valias;
    cx.nvalias = nva;
    g_pl_fresh_next = maxlocal + 1;
    for (int k = 0; k < nc; k++) {
        const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[k] : ch;
        if (!cl || cl->t != TT_CLAUSE) pl_refuse("clause shape in", key, 2);
        int ar = (int) cl->v.dval;
        if (ar < 0) ar = 0;
        if (ar > cl->n) ar = cl->n;
        if (arity < 0) arity = ar;
        if (ar != arity) pl_refuse("clauses of differing arity in", key, 2);
        memset(valias, 0, sizeof valias);
        for (int i = 0; i < ar; i++) {
            const tree_t * a = cl->c[i];
            if (!a || a->t != TT_VAR) continue;
            int sl = (int) a->v.ival;
            if (sl < 0 || sl >= nva) continue;
            int seen = 0;
            for (int j = 0; j < i && !seen; j++) {
                int nvs = pl_var_nodes(cl->c[j]) + 1;
                int vs[nvs], nv = 0;
                pl_collect_vars(cl->c[j], vs, &nv, nvs);
                for (int q = 0; q < nv; q++) if (vs[q] == sl) seen = 1;
            }
            if (!seen) { valias[sl] = i + 1; aliased[sl] = 1; }
        }
        IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
        IR_t * tret = pl_trace_named_wrap(&cx, "__trace_return", trace_key, succeed, step);
        IR_t * bentry = NULL;
        IR_t * redo = NULL;
        IR_t * tnode = NULL;
        if (pl_trace_wanted()) for (int i = ar; i < cl->n; i++) pl_trace_number_goals(cl->c[i]);
        IR_t * first = pl_lower_conj(&cx, (const tree_t * const *)(cl->c + ar), cl->n - ar, tret, step, &bentry, &redo, &tnode);
        if (tnode && tnode->op == IR_CALL_PROC_STAGED && !pl_trace_wanted()) tnode->seal = PL_SEAL_TAIL;
        IR_t * next = bentry ? bentry : (first ? first : succeed);
        for (int i = ar - 1; i >= 0; i--) {
            if (cl->c[i] && cl->c[i]->t == TT_VAR && (int) cl->c[i]->v.ival >= 0 && (int) cl->c[i]->v.ival < nva && valias[(int) cl->c[i]->v.ival] == i + 1) continue;
            { IR_t * klit = pl_const_lit(&cx, cl->c[i]); if (klit) { next = pl_unify_const_node(&cx, pl_param_name(i), klit, next, step); continue; } }
            if (pl_head_boxable(cl->c[i])) {
                IR_t * srcv = build(&cx, IR_VAR_REF, NULL, NULL);
                IR_LIT(srcv).sval = pl_param_name(i);
                next = pl_head_term(&cx, cl, ar, srcv, 0, cl->c[i], next, step);
                continue;
            }
            IR_t * u = build(&cx, IR_CALL, next, step);
            IR_LIT(u).sval = "$unify";
            IR_t * lhs = build(&cx, IR_VAR_REF, NULL, NULL);
            IR_LIT(lhs).sval = pl_param_name(i);
            IR_t * he = NULL;
            IR_t * rhs = term_lval_e(&cx, cl->c[i], &he);
            lc_γ_to(lhs, he ? he : rhs);
            lc_ω_to(lhs, step);
            lc_γ_to(rhs, u);
            lc_ω_to(rhs, step);
            ir_operand_push(u, lhs);
            ir_operand_push(u, rhs);
            next = lhs;
        }
        g->alt_entry[k] = next;
        g->alt_ret[k] = succeed;
        g->alt_redo[k] = redo;
    }
    maxlocal = g_pl_fresh_next - 1;
    g_pl_fresh_next = fresh_saved;
    if (arity < 0) arity = 0;
    g->entry = pl_trace_named_wrap(&cx, "__trace_call", trace_key, g->alt_entry[0], step);
    g->alt_entry[0] = g->entry;
    pl_graph_stamp(g, arity, maxlocal, aliased, nva);
    g->deterministic = (nc == 1);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_pl_pred_graph(const char * key, const tree_t * ch) {
    if (ch->t == TT_CHOICE) { if (ch->n < 1) return -1; if (!ch->c[0] || ch->c[0]->t != TT_CLAUSE) return -1; } else if (ch->t != TT_CLAUSE) return -1;
    { IR_graph_t * g = pl_pred_graph(ch, key); return bb_program_add(&g_stage2.bbp, g); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_runtime_define_pred_x(const char * key, const tree_t * choice, int arity, IR_graph_t ** gout, int pkt_cell, int pkt_slot, int * chain_off_out, void * rt_root) {
    extern IR_graph_t * g_emit_cfg;
    extern int g_frame_active;
    extern int g_rt_fragment_emit;
    extern int g_gen_proc_active;
    extern int emit_jmp_entry_for_proc(const char *, int, int, IR_graph_t *);
    extern void emit_jmp_entry_clear(void);
    extern void zls_graph_name(const IR_graph_t *, const char *);
    extern void rt_proc_set_fn(const char *, bb_box_fn);
    extern void rt_proc_set_frame_bytes(const char *, int);
    extern void rt_proc_set_dyn_scope(const char *, int);
    extern void emit_patzeta_register(const char *, int, int, int);
    extern void rt_proc_set_generator(const char *, int);
    extern void rt_proc_set_jmpentry(const char *, int);
    extern void rt_proc_set_zstatic(const char *, int);
    extern void bb_ab_seal_entry_cells(const char *, void *, int);
    extern int g_last_flat_frame_bytes;
    extern int g_last_flat_fp;
    extern int g_last_flat_uniform;
    extern int g_last_flat_zstatic;
    int idx;
    IR_graph_t * g;
    bb_box_fn fn;
    IR_graph_t * cfg_sv;
    int fa;
    int rfe_sv;
    int gpa_sv;
    if (!key || !choice) return (void *)0;
    {
        extern int rt_proc_is_registered(const char *);
        extern void rt_proc_register(const char *, const char **, int);
        if (!rt_proc_is_registered(key)) rt_proc_register(ct_strdup(key), (const char **) 0, arity);
    }
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void zls_reset(void); zls_reset(); }
    {
        void * rsv = g_stage2.rt_root;
        int lsv = g_stage2.rt_lowering;
        g_stage2.rt_root = rt_root;
        g_stage2.rt_lowering = 1;
        idx = lower_pl_pred_graph(key, choice);
        g_stage2.rt_root = rsv;
        g_stage2.rt_lowering = lsv;
    }
    if (idx < 0) return (void *)0;
    g = g_stage2.bbp.table[idx];
    if (!g) return (void *)0;
    if (gout) *gout = g;
    g->resumable_callable = 1;
    if (pkt_slot >= 0) { g->pkt_fragment = 1; g->pkt_cell = pkt_cell; g->pkt_slot = pkt_slot; }
    { extern void zls_forget_graph_nodes(const IR_graph_t *); zls_forget_graph_nodes(g); }
    { extern void fl_derive_tier(IR_graph_t *); fl_derive_tier(g); }
    { extern void ir_drive_slot_assign(IR_graph_t *); ir_drive_slot_assign(g); }
    cfg_sv = g_emit_cfg;
    g_emit_cfg = g;
    fa = g_frame_active;
    g_frame_active = 1;
    rfe_sv = g_rt_fragment_emit;
    g_rt_fragment_emit = 1;
    rt_proc_set_generator(key, 1);
    rt_proc_set_jmpentry(key, 1);
    rt_proc_set_dyn_scope(key, 0);
    { extern void rt_proc_set_pinned(const char *, int); rt_proc_set_pinned(key, (g->zframe_pinned_base && g->zframe_graph && !g->icn_cells_graph) ? 1 : 0); }
    gpa_sv = g_gen_proc_active;
    g_gen_proc_active = 1;
    {
        extern int g_flat_frame_floor;
        extern int zls_g_region(const IR_graph_t *);
        g_flat_frame_floor = 0;
        if (g->entry && ((g->entry->op == IR_DEFINE && IR_LIT(g->entry).ival == 3) || g->entry->op == IR_GOTO_DEFERRED)) {
            for (int _mi = 0; _mi < g_stage2.proc_count; _mi++) if (g_stage2.proc_table[_mi].name && !strcmp(g_stage2.proc_table[_mi].name, "main")) {
                int _mx = g_stage2.proc_table[_mi].bb_idx;
                if (_mx >= 0 && _mx < g_stage2.bbp.count && g_stage2.bbp.table[_mx]) g_flat_frame_floor = zls_g_region(g_stage2.bbp.table[_mx]);
                break;
            }
            if (g_flat_frame_floor <= 0) g_flat_frame_floor = zls_g_region(g);
        }
    }
    emit_jmp_entry_for_proc(key, 0, 1, g);
    { extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int); g_flat_dc_np = (pkt_slot < 0 && rt_pl_dc_ok(key, g->nparams)) ? g->nparams : -1; }
    zls_graph_name(g, key);
    {
        char pfx[fmt_len("proc_%s", key)];
        snprintf(pfx, sizeof pfx, "proc_%s", key);
        if (getenv("SCRIP_PL_RTASM")) {
            extern int zls_g_region(const IR_graph_t *);
            extern int zls_off(const IR_t *);
            int _mx = -1;
            for (int _i = 0; _i < g->n; _i++) if (g->all[_i]) { int _o = zls_off(g->all[_i]); if (_o > _mx) _mx = _o; }
            fprintf(stderr, "[RTASM] %s: nodes=%d nparams=%d zls_region=%d max_granted_cell=%d %s\n", key, g->n, g->nparams, zls_g_region(g), _mx,
                (_mx >= 0 && _mx + 16 > zls_g_region(g)) ? "*** CELL OUTSIDE REGION ***" : "(in region)");
            {
                extern int zls_scope_of(const IR_t *);
                extern int zls_g_first_scope(const IR_graph_t *);
                int _rs = zls_g_first_scope(g);
                for (int _i = 0; _i < g->n; _i++) if (g->all[_i]) {
                    int _o = zls_off(g->all[_i]);
                    int _sc = zls_scope_of(g->all[_i]);
                    if (_o < 0) continue;
                    fprintf(stderr, "[RTASM]   node[%d] op=%d off=%d scope=%d root_scope=%d %s\n", _i, (int)g->all[_i]->op, _o, _sc, _rs,
                        (_sc >= 0 && _sc != _rs) ? "*** STALE ENTRY FROM ANOTHER GRAPH ***" : "");
                }
            }
            fprintf(stderr, "[RTASM] ---- runtime fragment for %s ----\n", key);
            emit_chain(g->entry, stderr, pfx);
            fprintf(stderr, "[RTASM] ---- end %s ----\n", key);
        }
        fn = emit_chain(g->entry, (FILE *)0, pfx);
    }
    { extern void emit_gc_tables_register(const void *); emit_gc_tables_register((const void *) fn); }
    emit_jmp_entry_clear();
    g_gen_proc_active = gpa_sv;
    g_rt_fragment_emit = rfe_sv;
    g_frame_active = fa;
    g_emit_cfg = cfg_sv;
    if (!fn) return (void *)0;
    if (chain_off_out) { extern long emit_last_pkt_chain_off(void); *chain_off_out = (int)emit_last_pkt_chain_off(); }
    rt_proc_set_frame_bytes(key, g_last_flat_frame_bytes);
    rt_proc_set_fn(key, fn);
    bb_ab_seal_entry_cells(key, (void *)fn, 1);
    rt_proc_set_zstatic(key, g_last_flat_zstatic);
    emit_patzeta_register(key, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform);
    { extern long g_last_dc_off; extern void rt_proc_set_dcfn(const char *, void *); rt_proc_set_dcfn(key, (g_last_dc_off >= 0) ? (void *)((char *)fn + g_last_dc_off) : (void *)0); }
    if (!gout) {
        extern void zls_reset(void);
        extern void IR_free(IR_graph_t *);
        zls_reset();
        g_stage2.bbp.table[idx] = (IR_graph_t *)0;
        if (idx == g_stage2.bbp.count - 1) g_stage2.bbp.count--;
        IR_free(g);
    }
    return (void *)fn;
}
static void * pl_runtime_define_pred_g(const char * key, const tree_t * choice, int arity, IR_graph_t ** gout) {
    return pl_runtime_define_pred_x(key, choice, arity, gout, -1, -1, (int *)0, (void *)0);
}
void * pl_runtime_define_pred(const char * key, const tree_t * choice, int arity, void * rt_root) { return pl_runtime_define_pred_x(key, choice, arity, NULL, -1, -1, (int *)0, rt_root); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_count(const tree_t * t) { int n; if (!t) return 0; n = 1; for (int i = 0; i < t->n; i++) n += pl_tree_count(t->c[i]); return n; }
static void pl_tree_gather(tree_t * t, tree_t ** v, int * k) { if (!t) return; v[(*k)++] = t; for (int i = 0; i < t->n; i++) pl_tree_gather(t->c[i], v, k); }
static int pl_tree_ptr_cmp(const void * a, const void * b) { uintptr_t x = (uintptr_t) *(tree_t * const *) a, y = (uintptr_t) *(tree_t * const *) b; return (x > y) - (x < y); }
static void pl_trees_drop(tree_t * a, tree_t * b) {
    int n = pl_tree_count(a) + pl_tree_count(b);
    int k = 0;
    tree_t ** v;
    if (n <= 0) return;
    v = (tree_t **) ct_alloc((size_t) n * sizeof(tree_t *));
    pl_tree_gather(a, v, &k);
    pl_tree_gather(b, v, &k);
    qsort(v, (size_t) k, sizeof(tree_t *), pl_tree_ptr_cmp);
    for (int i = 0; i < k; i++) if (!i || v[i] != v[i - 1]) { if (v[i]->c) ct_drop((char *) v[i]->c - sizeof(size_t)); ct_drop(v[i]); }
    ct_drop(v);
}
void * pl_runtime_define_fragment(const char * fkey, void * clause_cell, int arity, int cell_k, int slot, int * ridx_out, int * chain_off_out, void * rt_root) {
    extern void * rt_pl_clause_tree(void *);
    extern tree_t * pl_runtime_clause_tree(tree_t *);
    extern void * rt_pl_choice_new(const char *);
    extern void rt_pl_choice_add(void *, void *);
    extern int rt_proc_index_of(const char *);
    void * raw = clause_cell ? rt_pl_clause_tree(clause_cell) : (void *)0;
    void * cl = raw ? (void *) pl_runtime_clause_tree((tree_t *) raw) : (void *)0;
    void * ch;
    void * fn;
    int off = -1;
    if (!fkey || !cl) return (void *)0;
    ch = rt_pl_choice_new(fkey);
    rt_pl_choice_add(ch, cl);
    fn = pl_runtime_define_pred_x(ct_strdup(fkey), (const tree_t *) ch, arity, NULL, cell_k, slot, &off, rt_root);
    pl_trees_drop((tree_t *) raw, (tree_t *) ch);
    if (!fn || off < 0) return (void *)0;
    if (ridx_out) *ridx_out = rt_proc_index_of(fkey);
    if (chain_off_out) *chain_off_out = off;
    return fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_runtime_install_packet_entry(const char * key, int arity, const char * head_fkey) {
    extern int rt_proc_is_registered(const char *);
    extern void rt_proc_register(const char *, const char **, int);
    extern void * rt_proc_fn(const char *);
    extern void rt_proc_set_generator(const char *, int);
    extern void rt_proc_set_jmpentry(const char *, int);
    extern void rt_proc_set_dyn_scope(const char *, int);
    extern void rt_proc_set_pinned(const char *, int);
    extern void rt_proc_set_fn(const char *, bb_box_fn);
    extern void bb_ab_seal_entry_cells(const char *, void *, int);
    void * fn = head_fkey ? rt_proc_fn(head_fkey) : (void *)0;
    if (!key || !fn) return 0;
    if (!rt_proc_is_registered(key)) rt_proc_register(ct_strdup(key), (const char **) 0, arity);
    rt_proc_set_generator(key, 1);
    rt_proc_set_jmpentry(key, 1);
    rt_proc_set_dyn_scope(key, 0);
    rt_proc_set_pinned(key, 1);
    rt_proc_set_fn(key, (bb_box_fn) fn);
    bb_ab_seal_entry_cells(key, fn, 1);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_mkc_functor_is(const IR_t * f, const char * nm) {
    extern int prolog_functor_name(int);
    extern const char * prolog_atom_name(int);
    if (!f || !nm) return 0;
    if (f->op == IR_LIT_INTEGER) { const char * s = prolog_atom_name(prolog_functor_name((int)IR_LIT(f).ival)); return s && !strcmp(s, nm); }
    return (f->op == IR_LIT_STRING || f->op == IR_LIT_ATOM) && IR_LIT(f).sval && !strcmp(IR_LIT(f).sval, nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_mc_wrapper_term(const char * fn, int ar) {
    if (ar <= 0) { tree_t * a = ast_node_new(TT_QLIT); a->v.sval = ct_strdup(fn); return a; }
    {
        tree_t * t = ast_node_new(TT_FNC);
        t->v.sval = ct_strdup(fn);
        for (int i = 0; i < ar; i++) { char b[24]; tree_t * v = ast_node_new(TT_VAR); snprintf(b, sizeof b, "_A%d", i); v->v.sval = ct_strdup(b); ast_push(t, v); }
        return t;
    }
}
static int pl_mc_graph_fell_back(const IR_graph_t * g, const char * nm, int ar) {
    if (!g) return 1;
    for (int i = 0; i < g->n; i++) {
        const IR_t * nd = g->all[i];
        const IR_t * gv;
        if (!nd || nd->op != IR_CALL_VALUE || !IR_LIT(nd).sval || strcmp(IR_LIT(nd).sval, "goal") || nd->n_operands < 1) continue;
        gv = nd->operands[0];
        if (!gv) continue;
        if (ar == 0 && (gv->op == IR_LIT_STRING || gv->op == IR_LIT_ATOM) && IR_LIT(gv).sval && !strcmp(IR_LIT(gv).sval, nm)) return 1;
        if (ar > 0 && gv->op == IR_CALL && IR_LIT(gv).sval && !strcmp(IR_LIT(gv).sval, "$mkc") && gv->n_operands == ar + 1 && gv->operands[0] && pl_mkc_functor_is(gv->operands[0], nm)) return 1;
    }
    return 0;
}
static void * pl_mc_define(const char * wkey, const char * wname, int ar, tree_t * body, IR_graph_t ** gout) {
    extern tree_t * pl_runtime_clause_tree(tree_t *);
    tree_t * pair = pl_cc_fnc2(":-", pl_mc_wrapper_term(wname, ar), body);
    tree_t * cl = pl_runtime_clause_tree(pair);
    tree_t * ch;
    if (!cl) return (void *)0;
    ch = ast_node_new(TT_CHOICE);
    ch->v.sval = ct_strdup(wkey);
    ast_push(ch, cl);
    return pl_runtime_define_pred_g(wkey, ch, ar, gout);
}
void * pl_runtime_define_goal_wrapper(const char * wkey, const char * nm, int ar) {
    extern int rt_pl_unknown_suppress(const char *);
    IR_graph_t * g = NULL;
    void * fn;
    if (!wkey || !nm || ar < 0) return (void *)0;
    nm = ct_strdup(nm);
    char wname[fmt_len("$mc:%s", nm)];
    snprintf(wname, sizeof wname, "$mc:%s", nm);
    fn = pl_mc_define(wkey, wname, ar, pl_mc_wrapper_term(nm, ar), &g);
    if (!fn) return (void *)0;
    if (!pl_mc_graph_fell_back(g, nm, ar)) return fn;
    {
        char key[fmt_len("%s/%d", nm, ar)];
        tree_t * body;
        snprintf(key, sizeof key, "%s/%d", nm, ar);
        if (rt_pl_unknown_suppress(key)) body = (tree_t *) pl_atom_goal("fail");
        else body =
            pl_cc_fnc2(";", pl_cc_fnc2("->", pl_cc_fnc2("$pl_declared", (tree_t *) pl_atom_goal(nm), pl_cc_ilit(ar)), (tree_t *) pl_atom_goal("fail")),
            pl_cc_fnc1("throw", pl_cc_fnc2("error", pl_cc_fnc2("existence_error", (tree_t *) pl_atom_goal("procedure"), pl_cc_pi_ar(nm, ar)), pl_cc_pi_ar(nm, ar))));
        return pl_mc_define(wkey, wname, ar, body, NULL);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    int mod_idx = -1;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        tree_t * sub = lp_s_expr(s, ":subj");
        if (!sub) continue;
        if ((sub->t == TT_CHOICE || sub->t == TT_CLAUSE) && sub->v.sval) {
            resolve_pred_table_insert(&s2->resolve_pred_table, sub->v.sval, sub);
            if (strcmp(sub->v.sval, "main/0") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_dir_number_vars(tree_t * t, const char ** names, int * n, int scope0) {
    if (!t) return;
    if (t->t == TT_VAR) {
        const char * nm = t->v.sval;
        int slot = *n;
        if (nm && strcmp(nm, "_") != 0) { for (int i = scope0; i < *n; i++) if (names[i] && !strcmp(names[i], nm)) { slot = i; break; } }
        if (slot == *n) { names[*n] = nm; (*n)++; }
        t->v.ival = slot;
        return;
    }
    for (int i = 0; i < t->n; i++) pl_dir_number_vars(t->c[i], names, n, scope0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_dir_catch_wrap(tree_t * dirgoal, const char ** dvn, int * dvc) {
    int dv_scope0 = *dvc;
    tree_t * eb = ast_node_new(TT_VAR);
    eb->v.sval = (char *) "$DirBall";
    tree_t * eb2 = ast_node_new(TT_VAR);
    eb2->v.sval = (char *) "$DirBall";
    tree_t * fargs = ast_node_new(TT_MAKELIST);
    fargs->v.ival = 0;
    ast_push(fargs, eb2);
    tree_t * rep = ast_node_new(TT_FNC);
    rep->v.sval = (char *) "format";
    ast_push(rep, (tree_t *) pl_atom_goal("user_error"));
    ast_push(rep, (tree_t *) pl_atom_goal("Warning: directive raised: ~q~n"));
    ast_push(rep, fargs);
    tree_t * cat = ast_node_new(TT_FNC);
    cat->v.sval = (char *) "catch";
    ast_push(cat, dirgoal);
    ast_push(cat, eb);
    ast_push(cat, rep);
    tree_t * ig = pl_cc_fnc1("ignore", cat);
    pl_dir_number_vars(ig, dvn, dvc, dv_scope0);
    return ig;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { PL_SEED_CHUNK = 24, PL_SEED_PROCS_MAX = 900 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * pl_pi_text(const char * nm, int ar, int tail) {
    int n = snprintf(NULL, 0, tail ? "%s/%d" : "%s%d", nm, ar);
    char * r = (char *) ct_alloc((size_t) n + 1);
    snprintf(r, (size_t) n + 1, tail ? "%s/%d" : "%s%d", nm, ar);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_clause_term_key(const tree_t * h, cv_t * keys, int * all) {
    char * key;
    int ar = 0;
    const char * pn;
    while (h && h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2) h = h->c[1];
    if (!h) return;
    pn = pl_head_key(h, &ar);
    if (!pn) { if (!pl_tree_number(h)) *all = 1; return; }
    key = pl_pi_text(pn, ar, 1);
    for (uint32_t i = 0; i < keys->len; i++) if (!strcmp(CV_AT(*keys, const char *, i), key)) { ct_drop(key); return; }
    CV_PUSH(*keys, const char *) = key;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_clause_term_walk(const tree_t * t, cv_t * keys, int * all) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "clause") && (t->n == 2 || t->n == 3) && t->c[0]) pl_clause_term_key(t->c[0], keys, all);
    for (int i = 0; i < t->n; i++) pl_clause_term_walk(t->c[i], keys, all);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_static_seed_pred(const char * key, const tree_t * ch, cv_t * procs) {
    const char * slash = strrchr(key, '/');
    char * nm;
    size_t nl;
    int ar, n;
    const char * kn;
    if (!slash || key[0] == '$' || !ch || ch->t != TT_CHOICE || ch->n < 1) return;
    ar = atoi(slash + 1);
    nl = (size_t) (slash - key);
    nm = (char *) ct_alloc(nl + 1);
    memcpy(nm, key, nl);
    nm[nl] = 0;
    if (pl_dyn_index(nm, ar) >= 0 || pl_decl_dyn_is(nm, ar)) { ct_drop(nm); return; }
    n = ch->n;
    kn = ct_strdup(key);
    for (int lo = 0; lo < n && procs->len < PL_SEED_PROCS_MAX; lo += PL_SEED_CHUNK) {
        int hi = lo + PL_SEED_CHUNK < n ? lo + PL_SEED_CHUNK : n, m = 0, save_base = g_pl_seed_var_base;
        const tree_t ** sg = (const tree_t **) ct_zalloc((size_t) (hi - lo), sizeof(const tree_t *));
        g_pl_seed_var_base = 0;
        for (int i = lo; i < hi; i++) sg[m++] = pl_cc_fnc3("$db_seed_once", (tree_t *) pl_atom_goal(kn), pl_cc_ilit(i), pl_static_clause_term(ch->c[i], nm, ar));
        g_pl_seed_var_base = save_base;
        {
            IR_graph_t * g = pl_body_graph(sg, m);
            int bb_idx = bb_program_add(&g_stage2.bbp, g);
            if (bb_idx >= 0) {
                char * pn = pl_pi_text("$db_seedS", (int) procs->len, 0);
                char * rk = pl_pi_text(pn, 0, 1);
                pl_bb_register(rk, 0, bb_idx);
                pl_new_proc(rk, 0, bb_idx);
                CV_PUSH(*procs, const char *) = pn;
            }
        }
        ct_drop(sg);
    }
    ct_drop(nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_name_is_scrip_own(const char * nm, int ar) {
    extern int pl_prelude_defines(const char *, int);
    if (!nm) return 1;
    if (!strcmp(nm, "$pl_cconv") && ar == 2) return 1;
    return ar >= 0 && pl_prelude_defines(nm, ar);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_decls_capture(void) {
    extern int prolog_atom_intern(const char *);
    extern void * ct_grow(void *, size_t);
    long long * w = (long long *) 0;
    int n = 0, cap = 0;
#define PL_DW(v) do { if (n >= cap) { cap = cap ? cap * 2 : 64; w = (long long *) ct_grow((void *) w, (size_t) cap * sizeof(long long)); } w[n++] = (long long) (v); } while (0)
    PL_DW(0);
    PL_DW(g_stage2.pl_dyn_n > 0 ? g_stage2.pl_dyn_n : 1);
    for (int di = 0; di < g_stage2.pl_dyn_n; di++) {
        const char * dn = g_stage2.pl_dyn_name[di];
        int da = g_stage2.pl_dyn_arity[di];
        int kind = 0;
        if (!dn || pl_name_is_scrip_own(dn, da)) continue;
        if (da >= 0) { if (pl_decl_dyn_is(dn, da)) kind = 2; else if (pl_file_defines(dn, da)) kind = 3; }
        PL_DW(prolog_atom_intern(dn));
        PL_DW(da);
        PL_DW(kind);
        PL_DW(di);
    }
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) for (Resolve_PredEntry * pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
        const char * slash;
        int ar;
        size_t nl;
        if (!pe->key || !pe->choice || pe->choice->t != TT_CHOICE || pe->choice->n < 1) continue;
        slash = strrchr(pe->key, '/');
        if (!slash) continue;
        ar = atoi(slash + 1);
        nl = (size_t) (slash - pe->key);
        {
            char nm[nl + 1];
            memcpy(nm, pe->key, nl);
            nm[nl] = 0;
            if (pl_dyn_index(nm, ar) >= 0 || pl_decl_dyn_is(nm, ar) || pl_name_is_scrip_own(nm, ar)) continue;
            PL_DW(prolog_atom_intern(nm));
            PL_DW(ar);
            PL_DW(1);
            PL_DW(-1);
        }
    }
    for (int i = 0; i < g_pl_decl_other_n; i++) if (!pl_name_is_scrip_own(g_pl_decl_other_name[i], g_pl_decl_other_arity[i])) {
        PL_DW(prolog_atom_intern(g_pl_decl_other_name[i]));
        PL_DW(g_pl_decl_other_arity[i]);
        PL_DW(2);
        PL_DW(-1);
    }
#undef PL_DW
    w[0] = n - 2;
    g_stage2.db_decls_words = w;
    g_stage2.db_decls_n = n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_db_decls_words(long long ** out) { *out = g_stage2.db_decls_words; return g_stage2.db_decls_words ? g_stage2.db_decls_n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_table_specs(const tree_t * a, const char ** keys, int n, int cap) {
    if (!a || a->t != TT_FNC || !a->v.sval || a->n != 2) return n;
    if (!strcmp(a->v.sval, ",")) return pl_table_specs(a->c[1], keys, pl_table_specs(a->c[0], keys, n, cap), cap);
    if (!strcmp(a->v.sval, "as")) return pl_table_specs(a->c[0], keys, n, cap);
    if (strcmp(a->v.sval, "/") || !a->c[0] || (a->c[0]->t != TT_QLIT && a->c[0]->t != TT_NAME) || !a->c[0]->v.sval || !a->c[1] || a->c[1]->t != TT_ILIT) return n;
    if (keys && n < cap) { char b[strlen(a->c[0]->v.sval) + 24]; snprintf(b, sizeof b, "%s/%lld", a->c[0]->v.sval, a->c[1]->v.ival); keys[n] = lp_strdup(b); }
    return n + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_table_dir(const tree_t * s) {
    const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
    return (subj && subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "table") && subj->n >= 1) ? subj->c[0] : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_table_stmt(const tree_t * s, tree_t * subj) {
    tree_t * st = ast_node_new(TT_STMT);
    const tree_t * old = stmt_attr_find(s, ":subj");
    for (int i = 0; i < s->n; i++) ast_push(st, s->c[i] == old ? ast_attr_expr(":subj", subj) : s->c[i]);
    st->line = s->line;
    return st;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_table_goal(const char * f, int ar) {
    tree_t * g = ast_node_new(ar > 0 ? TT_FNC : TT_QLIT);
    g->v.sval = (char *) f;
    for (int i = 0; i < ar; i++) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = i; ast_push(g, v); }
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * pl_table_wrapper(const char * key) {
    const char * sl = strrchr(key, '/');
    int ar = atoi(sl + 1);
    char nm[sl - key + 1], tn[sl - key + 6];
    memcpy(nm, key, (size_t)(sl - key));
    nm[sl - key] = 0;
    snprintf(tn, sizeof tn, "$tbl %s", nm);
    tree_t * cl = ast_node_new(TT_CLAUSE);
    cl->v.dval = ar;
    for (int i = 0; i < ar; i++) { tree_t * v = ast_node_new(TT_VAR); v->v.ival = i; ast_push(cl, v); }
    tree_t * call = ast_node_new(TT_FNC);
    call->v.sval = (char *) "$tbl_call";
    ast_push(call, pl_table_goal(lp_strdup(nm), ar));
    ast_push(call, pl_table_goal(lp_strdup(tn), ar));
    ast_push(cl, call);
    tree_t * ch = ast_node_new(TT_CHOICE);
    ch->v.sval = (char *) lp_strdup(key);
    ast_push(ch, cl);
    return ch;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_table_rewrite(const tree_t * prog) {
    int nk = 0;
    for (int i = 0; i < prog->n; i++) nk = pl_table_specs(pl_table_dir(prog->c[i]), NULL, nk, 0);
    if (!nk) return prog;
    const char * keys[nk];
    unsigned char done[nk];
    int nf = 0;
    memset(done, 0, sizeof done);
    for (int i = 0; i < prog->n; i++) nf = pl_table_specs(pl_table_dir(prog->c[i]), keys, nf, nk);
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        int k = -1;
        if (subj && (subj->t == TT_CHOICE || subj->t == TT_CLAUSE) && subj->v.sval) for (int j = 0; j < nf; j++) if (!strcmp(keys[j], subj->v.sval)) k = j;
        if (k < 0) { ast_push(np, (tree_t *) s); continue; }
        tree_t * rn = pl_tree_copy(subj);
        char b[strlen(keys[k]) + 6];
        snprintf(b, sizeof b, "$tbl %s", keys[k]);
        rn->v.sval = (char *) lp_strdup(b);
        ast_push(np, pl_table_stmt(s, rn));
        if (!done[k]) { done[k] = 1; ast_push(np, pl_table_stmt(s, pl_table_wrapper(keys[k]))); }
    }
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_pl_stage2(const tree_t *prog) {
    prog = pl_table_rewrite(prog);
    int _pl_bb0 = g_stage2.bbp.count;
    pl_register_program(&g_stage2, prog);
    int ndirs = 1, ndvn = 1;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        if (subj && subj->t != TT_CHOICE && subj->t != TT_CLAUSE) { ndirs++; ndvn += pl_var_nodes(subj) + 2; }
    }
    const tree_t * init_goals[ndirs];
    int ninit = 0;
    const tree_t * dir_goals[ndirs];
    int ndir = 0;
    const char * dvn[ndvn];
    int dvc = 0;
    g_pl_decl_dyn_n = 0;
    g_pl_decl_other_n = 0;
    g_stage2.pl_decl_multifile.len = 0;
    g_stage2.pl_decl_meta.len = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t *subj = lp_s_expr(s, ":subj");
        if (!subj || subj->t == TT_CHOICE || subj->t == TT_CLAUSE) continue;
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "initialization") && subj->n >= 1) {
            const tree_t *gt = subj->c[0];
            if (gt && ((gt->t == TT_QLIT || gt->t == TT_NAME || gt->t == TT_FNC) && gt->v.sval)) {
                int iv_scope0 = dvc;
                pl_dir_number_vars((tree_t *) gt, dvn, &dvc, iv_scope0);
                init_goals[ninit++] = gt;
                continue;
            }
        }
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "dynamic") && subj->n >= 1) {
            for (int k = 0; k < subj->n; k++) pl_decl_dynamic_record(&g_stage2, subj->c[k], (tree_t *) subj);
            continue;
        }
        if (subj->t == TT_FNC && subj->v.sval && (!strcmp(subj->v.sval, "multifile") || !strcmp(subj->v.sval, "discontiguous")) && subj->n >= 1) {
            for (int k = 0; k < subj->n; k++) pl_decl_other_record(subj->c[k], !strcmp(subj->v.sval, "multifile"));
            continue;
        }
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "meta_predicate") && subj->n >= 1) { for (int k = 0; k < subj->n; k++) pl_decl_meta_record(subj->c[k]); continue; }
        if (subj->t == TT_FNC && subj->v.sval && pl_name_in(subj->v.sval, pl_decl_directives)) continue;
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "op") && subj->n == 3) { init_goals[ninit++] = pl_dir_catch_wrap((tree_t *) subj, dvn, &dvc); continue; }
        {
            tree_t * dirgoal = (tree_t *) subj;
            if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "set_prolog_flag") && subj->n == 2) {
                if (pl_flag_directive_is_advisory(subj)) continue;
                dirgoal = pl_cc_fnc2("$set_prolog_flag_declare", subj->c[0], subj->c[1]);
            }
            dir_goals[ndir++] = pl_dir_catch_wrap(dirgoal, dvn, &dvc);
            continue;
        }
    }
    IR_graph_t * top;
    {
        const tree_t * all_goals[ndir + ninit + 3];
        int nall = 0;
        int nseedv = 1;
        { tree_t * dt = ast_node_new(TT_FNC); dt->v.sval = (char *) "$db_decls"; ast_push(dt, pl_cc_ilit(0)); all_goals[nall++] = dt; }
        for (int di = 0; di < g_stage2.pl_dyn_n; di++) {
            const char * dn = g_stage2.pl_dyn_name[di];
            int da = g_stage2.pl_dyn_arity[di];
            if (!dn || da < 0 || pl_name_is_scrip_own(dn, da) || !pl_file_defines(dn, da)) continue;
            {
                char key[fmt_len("%s/%d", dn, da)];
                snprintf(key, sizeof key, "%s/%d", dn, da);
                const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
                nseedv += ch ? ((ch->t == TT_CHOICE) ? ch->n : 1) : 0;
            }
        }
        {
            const tree_t * seeds[nseedv];
            int nseed = 0;
            int save_base = g_pl_seed_var_base;
            g_pl_seed_var_base = 0;
            for (int di = 0; di < g_stage2.pl_dyn_n; di++) {
                const char * dn = g_stage2.pl_dyn_name[di];
                int da = g_stage2.pl_dyn_arity[di];
                if (!dn || da < 0 || pl_name_is_scrip_own(dn, da)) continue;
                if (pl_file_defines(dn, da)) {
                    char key[fmt_len("%s/%d", dn, da)];
                    snprintf(key, sizeof key, "%s/%d", dn, da);
                    const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
                    int n = ch ? ((ch->t == TT_CHOICE) ? ch->n : 1) : 0;
                    for (int i = 0; i < n; i++) {
                        const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[i] : ch;
                        seeds[nseed++] = pl_cc_fnc3("$db_seed_once", pl_cc_ilit(di), pl_cc_ilit(i), pl_static_clause_term(cl, dn, da));
                    }
                }
            }
            g_pl_seed_var_base = save_base;
            if (nseed > 0) {
                IR_graph_t * sg = pl_body_graph(seeds, nseed);
                int bb_idx = bb_program_add(&g_stage2.bbp, sg);
                if (bb_idx >= 0) { pl_bb_register("$db_seed/0", 0, bb_idx); pl_new_proc("$db_seed/0", 0, bb_idx); all_goals[nall++] = pl_atom_goal("$db_seed"); }
            }
        }
        {
            cv_t sk = { 0 };
            cv_t sp = { 0 };
            int sall = 0;
            for (int i = 0; i < prog->n; i++) { const tree_t * st = prog->c[i]; if (st && st->t == TT_STMT) pl_clause_term_walk(lp_s_expr(st, ":subj"), &sk, &sall); }
            if (sall) {
                for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) for (Resolve_PredEntry * pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) if (pe->key)
                    pl_static_seed_pred(pe->key, pe->choice, &sp);
            } else for (uint32_t i = 0; i < sk.len; i++) {
                const char * kk = CV_AT(sk, const char *, i);
                pl_static_seed_pred(kk, resolve_pred_table_lookup(&g_stage2.resolve_pred_table, kk), &sp);
            }
            if (sp.len > 0) {
                const tree_t ** dg = (const tree_t **) ct_zalloc(sp.len, sizeof(const tree_t *));
                for (uint32_t i = 0; i < sp.len; i++) dg[i] = pl_atom_goal(CV_AT(sp, const char *, i));
                {
                    IR_graph_t * g = pl_body_graph(dg, (int) sp.len);
                    int bb_idx = bb_program_add(&g_stage2.bbp, g);
                    if (bb_idx >= 0) { pl_bb_register("$db_seedS/0", 0, bb_idx); pl_new_proc("$db_seedS/0", 0, bb_idx); all_goals[nall++] = pl_atom_goal("$db_seedS"); }
                }
                ct_drop(dg);
            }
            ct_drop(sk.p);
            ct_drop(sp.p);
        }
        for (int i = 0; i < ndir; i++) all_goals[nall++] = dir_goals[i];
        for (int i = 0; i < ninit; i++) all_goals[nall++] = init_goals[i];
        top = pl_body_graph(all_goals, nall);
    }
    top->root_graph = 1;
    int top_idx = bb_program_add(&g_stage2.bbp, top);
    pl_new_proc("main", 0, top_idx);
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) {
        for (Resolve_PredEntry *pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
            if (!pe->key || !pe->choice) continue;
            const char *slash = strrchr(pe->key, '/');
            int ar = slash ? atoi(slash + 1) : 0;
            if (pl_bb_lookup(pe->key, ar)) continue;
            int bb_idx = lower_pl_pred_graph(pe->key, pe->choice);
            if (bb_idx < 0) continue;
            pl_bb_register(pe->key, ar, bb_idx);
            pl_new_proc(pe->key, ar, bb_idx);
        }
    }
    {
        extern tree_t * pl_runtime_clause_tree(tree_t *);
        {
            const char * fk = "$fc/3";
            if (!pl_bb_lookup(fk, 3) && !resolve_pred_table_lookup(&g_stage2.resolve_pred_table, fk)) {
                tree_t * ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(fk);
                int nb = 0;
                pl_bb_register(fk, 3, -1);
                for (int bi = 0; bi < 4; bi++) {
                    tree_t * hd = ast_node_new(TT_FNC);
                    tree_t * body;
                    tree_t * raw;
                    tree_t * cl;
                    hd->v.sval = ct_strdup("$fc");
                    if (bi == 0) {
                        ast_push(hd, pl_meta_var("G"));
                        ast_push(hd, pl_meta_var("_"));
                        ast_push(hd, pl_meta_var("_"));
                        body = pl_cc_fnc2(",", pl_cc_fnc2(",", pl_cc_fnc1("var", pl_meta_var("G")), (tree_t *) pl_atom_goal("!")), (tree_t *) pl_atom_goal("fail"));
                    } else if (bi == 1) {
                        ast_push(hd, pl_meta_var("G"));
                        ast_push(hd, (tree_t *) pl_atom_goal("true"));
                        ast_push(hd, (tree_t *) pl_atom_goal("true"));
                        body = pl_cc_fnc2(",", pl_cc_fnc2("==", pl_meta_var("G"), (tree_t *) pl_atom_goal("!")), (tree_t *) pl_atom_goal("!"));
                    } else if (bi == 2) {
                        ast_push(hd, pl_cc_fnc2(",", pl_meta_var("A"), pl_meta_var("B")));
                        ast_push(hd, pl_meta_var("Pre"));
                        ast_push(hd, pl_meta_var("Post"));
                        {
                            tree_t * s1 = pl_cc_fnc2(",", pl_cc_fnc3("$fc", pl_meta_var("A"), pl_meta_var("PA"), pl_meta_var("QA")), (tree_t *) pl_atom_goal("!"));
                            tree_t * s2 = pl_cc_fnc2(",", s1, pl_cc_fnc2("=", pl_meta_var("Pre"), pl_meta_var("PA")));
                            body = pl_cc_fnc2(",", s2, pl_cc_fnc2("=", pl_meta_var("Post"), pl_cc_fnc2(",", pl_meta_var("QA"), pl_meta_var("B"))));
                        }
                    } else {
                        ast_push(hd, pl_cc_fnc2(",", pl_meta_var("A"), pl_meta_var("B")));
                        ast_push(hd, pl_meta_var("Pre"));
                        ast_push(hd, pl_meta_var("Post"));
                        {
                            tree_t * s1 = pl_cc_fnc2(",", pl_cc_fnc3("$fc", pl_meta_var("B"), pl_meta_var("PB"), pl_meta_var("QB")), (tree_t *) pl_atom_goal("!"));
                            tree_t * s2 = pl_cc_fnc2(",", s1, pl_cc_fnc2("=", pl_meta_var("Pre"), pl_cc_fnc2(",", pl_meta_var("A"), pl_meta_var("PB"))));
                            body = pl_cc_fnc2(",", s2, pl_cc_fnc2("=", pl_meta_var("Post"), pl_meta_var("QB")));
                        }
                    }
                    raw = ast_node_new(TT_FNC);
                    raw->v.sval = (char *) ":-";
                    ast_push(raw, hd);
                    ast_push(raw, body);
                    cl = pl_runtime_clause_tree(raw);
                    if (!cl) continue;
                    ast_push(ch, cl);
                    nb++;
                }
                if (nb) { int bb_idx = lower_pl_pred_graph(fk, ch); if (bb_idx >= 0) { pl_bb_register(fk, 3, bb_idx); pl_new_proc(fk, 3, bb_idx); } }
            }
        }
        static const char * const pl_meta_ctrl[] = { ",", ";", "->", "*->" };
        for (size_t wi = 0; wi < sizeof pl_meta_ctrl / sizeof pl_meta_ctrl[0]; wi++) {
            const char * wn = pl_meta_ctrl[wi];
            char key[fmt_len("%s/2", wn)];
            int nclause;
            snprintf(key, sizeof key, "%s/2", wn);
            if (pl_bb_lookup(key, 2)) continue;
            if (resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key)) continue;
            nclause = !strcmp(wn, ";") ? 6 : !strcmp(wn, ",") ? 2 : 1;
            {
                tree_t * ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(key);
                int nb = 0;
                int cc = !strcmp(wn, ",") ? 1 : 0;
                for (int bk = 0; bk <= nclause; bk++) {
                    tree_t * hd = ast_node_new(TT_FNC);
                    tree_t * body = (tree_t *) 0;
                    tree_t * raw;
                    tree_t * cl;
                    int bi = bk - (bk > cc);
                    hd->v.sval = ct_strdup(wn);
                    ast_push(hd, pl_meta_var("A"));
                    ast_push(hd, pl_meta_var("B"));
                    if (bk == cc) body =
                        pl_cc_fnc2(",", pl_cc_fnc2(",", pl_cc_fnc2("$cutcall", pl_cc_fnc2(wn, pl_meta_var("A"), pl_meta_var("B")), pl_meta_var("NG")), (tree_t *) pl_atom_goal("!")),
                        pl_cc_fnc1("call", pl_meta_var("NG")));
                    else if (!strcmp(wn, ",")) {
                        tree_t * f1 = pl_cc_fnc3("$fc", pl_cc_fnc2(",", pl_meta_var("A"), pl_meta_var("B")), pl_meta_var("Pre"), pl_meta_var("Post"));
                        tree_t * f2 = pl_cc_fnc2(",", pl_cc_fnc2(",", f1, (tree_t *) pl_atom_goal("!")), pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("Pre")), (tree_t *) pl_atom_goal("!")));
                        body = bi == 0 ? pl_cc_fnc2(",", f2, pl_cc_fnc1("call", pl_meta_var("Post"))) : pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("A")), pl_cc_fnc1("call", pl_meta_var("B")));
                    } else if (!strcmp(wn, "->")) body = pl_cc_fnc2(",", pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("A")), (tree_t *) pl_atom_goal("!")), pl_cc_fnc1("call", pl_meta_var("B")));
                    else if (!strcmp(wn, "*->")) body = pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("A")), pl_cc_fnc1("call", pl_meta_var("B")));
                    else if (bi == 0) body =
                        pl_cc_fnc2(",", pl_cc_fnc2("=", pl_meta_var("A"), pl_cc_fnc2("->", pl_meta_var("C"), pl_meta_var("T"))),
                        pl_cc_fnc2(",", (tree_t *) pl_atom_goal("!"), pl_cc_ite(pl_cc_fnc1("call", pl_meta_var("C")), pl_cc_fnc1("call", pl_meta_var("T")), pl_cc_fnc1("call", pl_meta_var("B")))));
                    else if (bi == 1) body =
                        pl_cc_fnc2(",", pl_cc_fnc2("=", pl_meta_var("A"), pl_cc_fnc2("*->", pl_meta_var("C"), pl_meta_var("T"))),
                        pl_cc_fnc2(",", (tree_t *) pl_atom_goal("!"), pl_cc_scite(pl_cc_fnc1("call", pl_meta_var("C")), pl_cc_fnc1("call", pl_meta_var("T")), pl_cc_fnc1("call", pl_meta_var("B")))));
                    else if (bi == 2) {
                        tree_t * g1 = pl_cc_fnc2(",", pl_cc_fnc3("$fc", pl_meta_var("A"), pl_meta_var("Pre"), pl_meta_var("Post")), pl_cc_fnc1("call", pl_meta_var("Pre")));
                        body = pl_cc_fnc2(",", pl_cc_fnc2(",", g1, (tree_t *) pl_atom_goal("!")), pl_cc_fnc1("call", pl_meta_var("Post")));
                    } else if (bi == 3) body = pl_cc_fnc1("call", pl_meta_var("A"));
                    else if (bi == 4) {
                        tree_t * g1 = pl_cc_fnc2(",", pl_cc_fnc3("$fc", pl_meta_var("B"), pl_meta_var("Pre"), pl_meta_var("Post")), (tree_t *) pl_atom_goal("!"));
                        body = pl_cc_fnc2(",", pl_cc_fnc2(",", g1, pl_cc_fnc2(",", pl_cc_fnc1("call", pl_meta_var("Pre")), (tree_t *) pl_atom_goal("!"))), pl_cc_fnc1("call", pl_meta_var("Post")));
                    } else body = pl_cc_fnc1("call", pl_meta_var("B"));
                    raw = ast_node_new(TT_FNC);
                    raw->v.sval = (char *) ":-";
                    ast_push(raw, hd);
                    ast_push(raw, body);
                    cl = pl_runtime_clause_tree(raw);
                    if (!cl) continue;
                    ast_push(ch, cl);
                    nb++;
                }
                if (!nb) continue;
                { int bb_idx = lower_pl_pred_graph(key, ch); if (bb_idx < 0) continue; pl_bb_register(key, 2, bb_idx); pl_new_proc(key, 2, bb_idx); }
            }
        }
        {
            const char * pk = "print/1";
            if ((pl_file_defines("portray", 1) || pl_db_owned("portray", 1)) && !pl_bb_lookup(pk, 1) && !resolve_pred_table_lookup(&g_stage2.resolve_pred_table, pk) && !pl_decl_dyn_is("print", 1)) {
                tree_t * ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(pk);
                tree_t * hd = ast_node_new(TT_FNC);
                hd->v.sval = ct_strdup("print");
                ast_push(hd, pl_meta_var("A"));
                tree_t * body = pl_cc_print_via_format((tree_t *) 0, pl_meta_var("A"));
                tree_t * raw = ast_node_new(TT_FNC);
                raw->v.sval = (char *) ":-";
                ast_push(raw, hd);
                ast_push(raw, body);
                tree_t * cl = pl_runtime_clause_tree(raw);
                if (cl) { ast_push(ch, cl); { int bb_idx = lower_pl_pred_graph(pk, ch); if (bb_idx >= 0) { pl_bb_register(pk, 1, bb_idx); pl_new_proc(pk, 1, bb_idx); } } }
            }
        }
        {
            const char * ik = "if/3";
            if (!pl_bb_lookup(ik, 3) && !resolve_pred_table_lookup(&g_stage2.resolve_pred_table, ik) && !pl_decl_dyn_is("if", 3)) {
                tree_t * ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(ik);
                tree_t * hd = ast_node_new(TT_FNC);
                hd->v.sval = ct_strdup("if");
                ast_push(hd, pl_meta_var("C"));
                ast_push(hd, pl_meta_var("T"));
                ast_push(hd, pl_meta_var("E"));
                tree_t * body = pl_cc_scite(pl_cc_fnc1("call", pl_meta_var("C")), pl_cc_fnc1("call", pl_meta_var("T")), pl_cc_fnc1("call", pl_meta_var("E")));
                tree_t * raw = ast_node_new(TT_FNC);
                raw->v.sval = (char *) ":-";
                ast_push(raw, hd);
                ast_push(raw, body);
                tree_t * cl = pl_runtime_clause_tree(raw);
                if (cl) { ast_push(ch, cl); { int bb_idx = lower_pl_pred_graph(ik, ch); if (bb_idx >= 0) { pl_bb_register(ik, 3, bb_idx); pl_new_proc(ik, 3, bb_idx); } } }
            }
        }
    }
    {
        extern tree_t * pl_runtime_clause_tree(tree_t *);
        static const pl_det_leaf_t pl_meta_early[] = { { "write", 1, "$write" }, { "nl", 0, "$nl" }, { "true", 0, "$true" }, { "!", 0, "$true" }, { "fail", 0, "$fail" }, { "false", 0, "$fail" },
            { "throw", 1, "$throw" }, { "=", 2, "$unify" }, { "is", 2, "$is_v" }, { ">", 2, "$cmp_gt" }, { "assert", 1, "$db_assertz_t" }, { "asserta", 1, "$db_asserta_t" }, { "assertz", 1,
            "$db_assertz_t" }, { "retract", 1, "$db_erase_t" }, { "retractall", 1, "$db_retractall_t" }, { "abolish", 1, "$db_abolish_t" }, { "clause", 2, "$db_at_t" }, { 0, 0, 0 } };
        for (int tbl = 0; tbl < 2; tbl++) for (int li = 0; (tbl ? pl_meta_early[li].nm : pl_det_leaves[li].nm); li++) {
            const char * bn = tbl ? pl_meta_early[li].nm : pl_det_leaves[li].nm;
            int ba = tbl ? pl_meta_early[li].ar : pl_det_leaves[li].ar;
            if (!strcmp(bn, "halt") || bn[0] == '$') continue;
            char key[fmt_len("%s/%d", bn, ba)];
            snprintf(key, sizeof key, "%s/%d", bn, ba);
            if (pl_bb_lookup(key, ba)) continue;
            if (resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key)) continue;
            {
                tree_t * hd;
                tree_t * body;
                tree_t * raw;
                tree_t * cl;
                tree_t * ch;
                static const char * const an[] = { "A", "B", "C", "D", "E" };
                if (ba > 5) continue;
                if (ba > 0) {
                    hd = ast_node_new(TT_FNC);
                    hd->v.sval = ct_strdup(bn);
                    body = ast_node_new(TT_FNC);
                    body->v.sval = ct_strdup(bn);
                    for (int k = 0; k < ba; k++) { ast_push(hd, pl_meta_var(an[k])); ast_push(body, pl_meta_var(an[k])); }
                } else {
                    hd = ast_node_new(TT_QLIT);
                    hd->v.sval = ct_strdup(bn);
                    body = ast_node_new(TT_QLIT);
                    body->v.sval = ct_strdup(bn);
                }
                raw = ast_node_new(TT_FNC);
                raw->v.sval = (char *) ":-";
                ast_push(raw, hd);
                ast_push(raw, body);
                cl = pl_runtime_clause_tree(raw);
                if (!cl) continue;
                ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(key);
                ast_push(ch, cl);
                { int bb_idx = lower_pl_pred_graph(key, ch); if (bb_idx < 0) continue; pl_bb_register(key, ba, bb_idx); pl_new_proc(key, ba, bb_idx); }
            }
        }
    }
    for (int di = 0; di < g_stage2.pl_dyn_n; di++) {
        const char * dn = g_stage2.pl_dyn_name[di];
        int da = g_stage2.pl_dyn_arity[di];
        if (!dn || da < 0 || pl_name_is_scrip_own(dn, da)) continue;
        {
            char key[fmt_len("%s/%d", dn, da)];
            snprintf(key, sizeof key, "%s/%d", dn, da);
            if (pl_bb_lookup(key, da)) continue;
            {
                extern tree_t * pl_runtime_clause_tree(tree_t *);
                tree_t * hd;
                tree_t * raw;
                tree_t * cl;
                tree_t * ch;
                if (da > 0) {
                    hd = ast_node_new(TT_FNC);
                    hd->v.sval = ct_strdup(dn);
                    for (int i = 0; i < da; i++) { tree_t * v = ast_node_new(TT_VAR); v->v.sval = (char *) "_"; ast_push(hd, v); }
                } else {
                    hd = ast_node_new(TT_QLIT);
                    hd->v.sval = ct_strdup(dn);
                }
                { tree_t * fb = ast_node_new(TT_QLIT); fb->v.sval = (char *) "fail"; raw = ast_node_new(TT_FNC); raw->v.sval = (char *) ":-"; ast_push(raw, hd); ast_push(raw, fb); }
                cl = pl_runtime_clause_tree(raw);
                if (!cl) continue;
                ch = ast_node_new(TT_CHOICE);
                ch->v.sval = ct_strdup(key);
                ast_push(ch, cl);
                { int bb_idx = lower_pl_pred_graph(key, ch); if (bb_idx < 0) continue; pl_bb_register(key, da, bb_idx); pl_new_proc(key, da, bb_idx); }
            }
        }
    }
    for (int _gi = _pl_bb0; _gi < g_stage2.bbp.count; _gi++) if (g_stage2.bbp.table[_gi]) g_stage2.bbp.table[_gi]->resumable_callable = 1;
    { extern int rt_pl_db_frame_cells(void); top->standing_cells = rt_pl_db_frame_cells(); }
    pl_db_decls_capture();
    return &g_stage2;
}
