#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include <stdlib.h>
#include <ctype.h>
#include "ir/ast.h"
#include "lower/lower_pascal_class.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_08_pas.h"
#include "lower/lower_pascal_tree/lpt_09_sh.h"
#include "lower/lower_pascal_tree/lpt_10_pas.h"
#include "lower/lower_pascal_tree/lpt_11_sh.h"
#include "lower/lower_pascal_tree/lpt_12_pas.h"
static pval E_argument_list(const tree_t *n, int from, int to);
int pas_trace_enabled(void) { return g_runtime.abi.trace_budget != 0; }
PNodeList *pnl_new(void) { PNodeList *l = (PNodeList *)ct_zalloc(1, sizeof *l); return l; }
void pnl_push(PNodeList *l, tree_t *e) {
    if (!l) return;
    if (l->count >= l->cap) { l->cap = l->cap ? l->cap * 2 : 8; l->items = (tree_t **)ct_grow(l->items, (size_t)l->cap * sizeof(tree_t *)); }
    l->items[l->count++] = e;
}
PNodeList *pnl_concat(PNodeList *a, PNodeList *b) { if (!b) return a; for (int i = 0; i < b->count; i++) pnl_push(a, b->items[i]); return a; }
PasDef *pas_scope_push(PasDef **a, int *n, int *cap, const char *name) {
    if (*n >= *cap) { *cap = *cap ? *cap * 2 : 256; *a = (PasDef *)ct_grow(*a, (size_t)*cap * sizeof(PasDef)); }
    PasDef *d = &(*a)[(*n)++];
    d->name = name;
    d->sig = NULL;
    d->owner = NULL;
    d->alias = NULL;
    d->ov = NULL;
    d->depth = g_lower_pas.sem.pas_scope.depth;
    d->rid = 0;
    d->formal = 0;
    d->fld = g_lower_pas.sem.pas_recbody_depth > 0;
    d->uid = ++g_lower_pas.sem.pas_scope.nuid;
    return d;
}
tree_t *prog_of(PNodeList *l) { tree_t *e = ast_node_new(TT_PROGRAM); if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]); return e; }
tree_t *seq_of(PNodeList *l) { if (l && l->count == 1) return l->items[0]; tree_t *e = ast_node_new(TT_SEQ_EXPR); if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]); return e; }
static tree_t *pas_trace_lhs(tree_t *lhs) { return (pas_trace_enabled() && lhs && lhs->t != TT_VAR) ? pas_tree_clone(lhs) : lhs; }
static tree_t *pas_trace_assigned(tree_t *lhs, tree_t *asn) {
    if (!pas_trace_enabled() || !lhs) return asn;
    const char *nm = pas_trace_store_name(lhs);
    tree_t *tv;
    if (nm) tv = mk_fnc2("__trace_value", leaf_s(TT_QLIT, nm), pas_trace_wrap_value(leaf_s(TT_VAR, nm)));
    else tv = mk_fnc2("__trace_value", leaf_s(TT_QLIT, "<lval>"), pas_trace_wrap_value(lhs));
    PNodeList *sl = pnl_new();
    pnl_push(sl, asn);
    pnl_push(sl, tv);
    return seq_of(sl);
}
tree_t *pas_trace_wrap_for_body(const char *var, tree_t *body) {
    if (!pas_trace_enabled() || !var) return body;
    PNodeList *l = pnl_new();
    pnl_push(l, mk_fnc2("__trace_value", leaf_s(TT_QLIT, var), pas_trace_wrap_value(leaf_s(TT_VAR, var))));
    if (body) pnl_push(l, body);
    return seq_of(l);
}
tree_t *pas_divchk_wrap(tree_t *b) {
    if (!g_lower_pas.sem.pas_divchk || !b || (b->t == TT_ILIT && b->v.ival != 0)) return b;
    PNodeList *l = pnl_new();
    pnl_push(l, b);
    pnl_push(l, ilit(-1));
    return mk_call_raw("__sh_divchk", l);
}
int pas_array_is_param(const char *name) {
    const pty_t *t = name ? pty_name_type(name) : NULL;
    int k = pty_name_kind(name);
    return t && (k == 'p' || k == 'r') && (t->k == PTK_ARRAY || t->k == PTK_RECORD);
}
const char *pas_proctype_get(const char *name) {
    for (int i = g_lower_pas.sem.pas_nproctype - 1; name && i >= 0; i--) if (!strcmp(g_lower_pas.sem.pas_proctypes[i].name, name)) return g_lower_pas.sem.pas_proctypes[i].sig;
    return NULL;
}
int pas_array_isopen(const char *name) { const pty_t *t = name ? pty_name_type(name) : NULL; return t && t->k == PTK_ARRAY && t->open; }
int pas_arrtype_isdyn(const char *name) { return pty_tn_dyn(name); }
int pas_arrtype_isbool(const char *name) { return pty_tn_isbool(name); }
static int pas_field_arr_t(const tree_t *e, int charel) {
    const pty_t *t = pty_of_low(e);
    if (!e || !t || !pty_is_field(e) || t->k != PTK_ARRAY || t->dyn || !t->base) return 0;
    return charel ? t->base->k == PTK_CHAR : t->base->k != PTK_CHAR;
}
static long long pas_fld_lo(const tree_t *e) { const pty_t *t = pty_of_low(e); return t && t->k == PTK_ARRAY && t->ix && t->ix[0] ? t->ix[0]->lo : 0; }
static long long pas_fld_hi(const tree_t *e) { const pty_t *t = pty_of_low(e); return t && t->k == PTK_ARRAY && t->ix && t->ix[0] ? t->ix[0]->hi : -1; }
int pas_is_cafield(const tree_t *e) { return pas_field_arr_t(e, 1); }
long long pas_cafield_lo_get(const tree_t *e) { return pas_fld_lo(e); }
long long pas_cafield_hi_get(const tree_t *e) { return pas_fld_hi(e); }
int pas_is_nafield(const tree_t *e) { return pas_field_arr_t(e, 0); }
static long long pas_nafield_lo_get(const tree_t *e) { return pas_fld_lo(e); }
void pas_flat_mark_add(tree_t *e) { if (e) pnl_push(&g_lower_pas.sem.pas_flat_marks, e); }
tree_t *pas_rdiv(tree_t *a, tree_t *b) {
    if (a && b && (a->t == TT_ILIT || a->t == TT_FLIT) && (b->t == TT_ILIT ? b->v.ival == 0 : b->t == TT_FLIT && b->v.dval == 0)) return flit((a->t == TT_ILIT ? (double) a->v.ival : a->v.dval) / 0.0);
    tree_t *q = bin(TT_MUL, a, flit(1.0));
    if (!g_lower_pas.sem.pas_divchk || (b && ((b->t == TT_ILIT && b->v.ival != 0) || (b->t == TT_FLIT && b->v.dval != 0)))) return bin(TT_DIV, q, b);
    PNodeList *l = pnl_new();
    pnl_push(l, q);
    pnl_push(l, ilit(-1));
    pnl_push(l, b);
    pnl_push(l, ilit(-1));
    return mk_call_raw("__sh_rdiv", l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *pas_pf_cat3(const char *a, const char *b, const char *c) {
    size_t la = a ? strlen(a) : 0, lb = b ? strlen(b) : 0, lc = c ? strlen(c) : 0;
    char *s = (char *)ct_zalloc(1, la + lb + lc + 1);
    if (la) memcpy(s, a, la);
    if (lb) memcpy(s + la, b, lb);
    if (lc) memcpy(s + la + lb, c, lc);
    return s;
}
const char *pas_pf_canon(const char *t) { for (int g = 0; t && g < 16 && pas_subtype_high(t) < 0; g++) { const char *a = pas_typealias_get(t); if (!a) break; t = a; } return t ? t : ""; }
char *pas_pf_sect(char k, int n, const char *t) { char nb[24]; snprintf(nb, sizeof nb, "%c%d:", k, n); return pas_pf_cat3(nb, pas_pf_canon(t), NULL); }
char *pas_pf_sig(char k, const char *params, const char *rtype) { char *s = pas_pf_cat3(k == 'F' ? "F(" : "P(", params, ")"); return k == 'F' ? pas_pf_cat3(s, ":", pas_pf_canon(rtype)) : s; }
const char *pas_pf_env_name(const char *formal, int k) { char b[16]; snprintf(b, sizeof b, "__pas_pe%d_", k); return pas_pf_cat3(b, formal, NULL); }
static void pas_pf_pend(const char *formal, const char *sig) { pas_scope_push(&g_lower_pas.sem.pas_scope.pend, &g_lower_pas.sem.pas_scope.npend, &g_lower_pas.sem.pas_scope.cpend, formal)->sig = sig; }
static void pas_pf_section(char k, PNodeList *ids, const char *t) { pas_pf_pend(NULL, pas_pf_sect(k, ids ? ids->count : 0, t)); }
static PNodeList *pas_pf_formal(const char *name, const char *sig, int named) {
    pas_not_a_word_symbol(name);
    pas_pf_pend(NULL, sig);
    pas_pf_pend(name, sig);
    g_lower_pas.sem.pas_scope.pend[g_lower_pas.sem.pas_scope.npend - 1].depth = named;
    PNodeList *l = pnl_new();
    pnl_push(l, leaf_s(TT_VAR, name));
    for (int k = 1; k <= 3; k++) pnl_push(l, leaf_s(TT_VAR, pas_pf_env_name(name, k)));
    return l;
}
static const char *pas_pf_no_self(const char *sig) {
    const char *p, *q, *rest;
    char *s;
    int dep = 0;
    if (!sig || !sig[0] || sig[1] != '(') return NULL;
    p = q = sig + 2;
    while (*q && !(dep == 0 && (*q == ';' || *q == ')'))) { if (*q == '(') dep++; else if (*q == ')') dep--; q++; }
    if (q == p) return NULL;
    rest = *q == ';' ? q + 1 : q;
    s = (char *)ct_zalloc(1, strlen(rest) + 3);
    s[0] = sig[0];
    s[1] = '(';
    memcpy(s + 2, rest, strlen(rest));
    return s;
}
const char *pas_pf_heading(const char *name, PNodeList *params, char k, const char *rtype) {
    if ((!params || params->count == 0) && name) for (int i = g_lower_pas.sem.pas_scope.nfwd - 1; i >= 0; i--)
        if (g_lower_pas.sem.pas_scope.fwd[i].name && g_lower_pas.sem.pas_scope.fwd[i].sig && !strcmp(g_lower_pas.sem.pas_scope.fwd[i].name, name)) {
        g_lower_pas.sem.pas_scope.npend = 0;
        return g_lower_pas.sem.pas_scope.fwd[i].sig;
    }
    const char *ps = "";
    for (int i = 0; i < g_lower_pas.sem.pas_scope.npend; i++) {
        PasDef *d = &g_lower_pas.sem.pas_scope.pend[i];
        if (!d->name) ps = pas_pf_cat3(ps, *ps ? ";" : "", d->sig);
        else if (name) {
            PasDef *f = pas_scope_push(&g_lower_pas.sem.pas_scope.fsig, &g_lower_pas.sem.pas_scope.nfsig, &g_lower_pas.sem.pas_scope.cfsig, d->name);
            f->sig = d->sig;
            f->owner = name;
            f->depth = d->depth;
        }
    }
    g_lower_pas.sem.pas_scope.npend = 0;
    return pas_pf_sig(k, ps, rtype);
}
void pas_pf_define_routine(const char *name, const char *sig) {
    if (name) {
        PasDef *d = pas_scope_push(&g_lower_pas.sem.pas_scope.defs, &g_lower_pas.sem.pas_scope.n, &g_lower_pas.sem.pas_scope.cap, name);
        d->sig = sig;
        d->rid = ++g_lower_pas.sem.pas_scope.nrid;
    }
}
static const PasDef *pas_innermost_def(const char *name) {
    for (int i = g_lower_pas.sem.pas_scope.n - 1; name && i >= 0; i--) { const PasDef *d = &g_lower_pas.sem.pas_scope.defs[i]; if (d->name && !d->fld && !strcmp(d->name, name)) return d; }
    return NULL;
}
const char *pas_alias_resolve(const char *name) { const PasDef *d = pas_innermost_def(name); return (d && d->alias) ? d->alias : name; }
int pas_name_is_data(const char *name) { const PasDef *d = pas_innermost_def(name); return d && !d->rid; }
const char *pas_shadow_name(const char *src) {
    const PasDef *d = pas_innermost_def(src);
    if (!d || (!d->rid && !d->alias)) return src;
    if (d->depth >= g_lower_pas.sem.pas_scope.depth) return d->alias ? d->alias : src;
    size_t n = strlen(src) + 24;
    char buf[n];
    snprintf(buf, n, "%s$%d", src, ++g_lower_pas.sem.pas_routine_alias_n);
    const char *uniq = ct_strdup(buf);
    PasDef *a = pas_scope_push(&g_lower_pas.sem.pas_scope.defs, &g_lower_pas.sem.pas_scope.n, &g_lower_pas.sem.pas_scope.cap, src);
    a->alias = uniq;
    a->rid = 1;
    return uniq;
}
static char pas_pf_param(const char *sig, int idx, const char **fsig) {
    *fsig = NULL;
    if (!sig || strlen(sig) < 3) return 0;
    const char *p = sig + 2;
    int at = 0;
    while (*p && *p != ')') {
        const char *q = p;
        int dep = 0;
        while (*q && !(dep == 0 && (*q == ';' || *q == ')'))) { if (*q == '(') dep++; else if (*q == ')') dep--; q++; }
        char k = *p;
        int cnt = (k == 'v' || k == 'r') ? atoi(p + 1) : 1;
        if (idx < at + cnt) { if (k == 'P' || k == 'F') { char *f = (char *)ct_zalloc(1, (size_t)(q - p) + 1); memcpy(f, p, (size_t)(q - p)); *fsig = f; } return k; }
        at += cnt;
        p = (*q == ';') ? q + 1 : q;
    }
    return 0;
}
static const char *pas_pf_varproc(const char *sig, int idx) { const char *f, *t; return pas_pf_param(sig, idx, &f) == 'r' && (t = pas_sig_value_type(sig, idx, 1)) ? pas_proctype_get(t) : NULL; }
int pas_pf_nactuals(const char *sig) {
    int n = 0;
    const char *f;
    char k;
    for (int i = 0; (k = pas_pf_param(sig, i, &f)) != 0; i++) n += (k == 'P' || k == 'F' || pas_pf_varproc(sig, i)) ? 4 : 1;
    return n;
}
int pas_pf_decl_index(const char *callee, int xi) {
    const PasDef *cd = pas_pf_lookup(callee);
    const char *f;
    char k;
    if (!cd || !cd->sig || !strpbrk(cd->sig + 1, "PFr")) return xi;
    for (int d = 0, at = 0; (k = pas_pf_param(cd->sig, d, &f)) != 0; d++) { int w = (k == 'P' || k == 'F' || pas_pf_varproc(cd->sig, d)) ? 4 : 1; if (xi < at + w) return xi == at ? d : -1; at += w; }
    return xi;
}
static void pas_pf_candidate(const PasDef *d) {
    for (int i = 0; i < g_lower_pas.sem.pas_scope.ncand; i++) if (g_lower_pas.sem.pas_scope.cand[i].rid == d->rid && !strcmp(g_lower_pas.sem.pas_scope.cand[i].sig, d->sig)) return;
    PasDef *c = pas_scope_push(&g_lower_pas.sem.pas_scope.cand, &g_lower_pas.sem.pas_scope.ncand, &g_lower_pas.sem.pas_scope.ccand, d->name);
    c->sig = d->sig;
    c->depth = d->depth;
    c->rid = d->rid;
}
static const char *pas_pf_named_formal(const char *callee, int idx) {
    const PasDef *cd = pas_pf_lookup(callee);
    const char *f;
    int j = 0;
    for (int i = 0; cd && i < idx; i++) { char k = pas_pf_param(cd->sig, i, &f); if (k == 'P' || k == 'F') j++; }
    for (int i = 0; cd && i < g_lower_pas.sem.pas_scope.nfsig; i++) {
        const PasDef *e = &g_lower_pas.sem.pas_scope.fsig[i];
        if (e->owner && !strcmp(e->owner, callee) && j-- == 0) return e->depth ? e->name : NULL;
    }
    return NULL;
}
void pas_pf_closure(PNodeList *out, tree_t *a, const char *fsig, const char *callee, int pos) {
    const char *nm = NULL;
    tree_t *obj = NULL;
    int cast = 0;
    if (g_lower_pas.sem.pas_dialect_fpc && a && a->t == TT_FNC && a->n == 2 && a->c[0] && a->c[0]->t == TT_VAR && a->c[0]->v.sval && a->c[1] && !pas_is_func(a->c[0]->v.sval) &&
        !pas_is_proc(a->c[0]->v.sval)) {
        const pty_t *ct = pty_type_by_name(a->c[0]->v.sval);
        if (ct && ct->k == PTK_PROC) { a = a->c[1]; cast = 1; }
    }
    if (a && a->t == TT_FNC && a->n == 3 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_mptr")) { nm = a->c[1]->v.sval; obj = a->c[2]; } else if (a && a->t == TT_VAR) nm = a->v.sval;
    else if (a && a->t == TT_FNC && a->n >= 1 && a->c[0] && a->c[0]->t == TT_VAR && a->c[0]->v.sval) nm = (!strcmp(a->c[0]->v.sval, "__pas_pcall") && a->n == 4) ? a->c[1]->v.sval :
        (a->n == 1 ? a->c[0]->v.sval : NULL);
    if (!nm && a && a->t == TT_ILIT) {
        for (int i = 0; !nm && i < g_lower_pas.sem.pas_nproc; i++) if (g_lower_pas.sem.pas_procs[i].name && pas_hash_stable(g_lower_pas.sem.pas_procs[i].name) == a->v.ival) nm =
            g_lower_pas.sem.pas_procs[i].name;
        for (int i = 0; !nm && i < g_lower_pas.sem.pas_nfunc; i++) if (g_lower_pas.sem.pas_funcs[i].name && pas_hash_stable(g_lower_pas.sem.pas_funcs[i].name) == a->v.ival) nm =
            g_lower_pas.sem.pas_funcs[i].name;
    }
    const PasDef *d = pas_pf_lookup(nm);
    if (!d) {
        fprintf(stderr, "pascal: ISO 7185 6.6.3.4 violation: the actual-parameter %d of '%s' shall be a %s-identifier\n", pos, callee, *fsig == 'F' ? "function" : "procedure");
        g_lower_pas.sem.pas_iso_errors++;
    } else if (!cast && strcmp(d->sig, fsig) && !(obj && pas_pf_no_self(d->sig) && !strcmp(pas_pf_no_self(d->sig), fsig))) {
        fprintf(stderr, "pascal: ISO 7185 6.6.3.6 violation: the actual-parameter %d of '%s' is '%s', whose heading %s is not congruous with the formal-parameter's %s\n", pos, callee, nm, d->sig,
            fsig);
        g_lower_pas.sem.pas_iso_errors++;
    }
    if (d && d->formal) {
        pnl_push(out, leaf_s(TT_VAR, nm));
        pnl_push(out, ilit(-1));
        for (int k = 1; k <= 3; k++) { pnl_push(out, leaf_s(TT_VAR, pas_pf_env_name(nm, k))); pnl_push(out, ilit(-1)); }
        return;
    }
    int lvl = d ? d->depth + 1 : 1;
    const char *pn = d && d->depth >= 1 ? pas_pf_named_formal(callee, pos - 1) : NULL;
    if (pn) {
        fprintf(stderr, "pascal: the nested routine '%s' cannot be passed to the parameter '%s' of '%s': the parameter has a procedural type name, which takes no nested routine (fpc: is nested)\n",
            nm, pn, callee);
        g_lower_pas.sem.pas_iso_errors++;
    }
    if (lvl > 4) {
        fprintf(stderr, "pascal: '%s' is declared at lexical level %d; passing a routine nested deeper than level 4 as an actual-parameter is not implemented\n", nm, lvl);
        g_lower_pas.sem.pas_iso_errors++;
    }
    if (d && cast) { PasDef dc = *d; dc.sig = fsig; pas_pf_candidate(&dc); } else if (d) pas_pf_candidate(d);
    pnl_push(out, ilit(d ? d->rid : 0));
    pnl_push(out, ilit(-1));
    for (int k = 1; k <= 3; k++) { pnl_push(out, (k == 1 && obj) ? obj : (k < lvl ? mk_fnc1("__pas_display_get", ilit(k)) : ilit(0))); pnl_push(out, ilit(-1)); }
}
static PNodeList *pas_pf_actuals(const char *callee, PNodeList *args) {
    const PasDef *cd = pas_pf_lookup(callee);
    const char *fs;
    if (cd && args) pas_value_actuals_check(callee, cd, args);
    if (!cd || !args || !strpbrk(cd->sig + 1, "PFr")) return args;
    PNodeList *out = pnl_new();
    for (int i = 0; i + 1 < args->count; i += 2) {
        char k = pas_pf_param(cd->sig, i / 2, &fs);
        tree_t *a = args->items[i];
        const char *vs = k == 'r' ? pas_pf_varproc(cd->sig, i / 2) : NULL;
        if (k == 'P' || k == 'F') pas_pf_closure(out, a, fs, callee, i / 2 + 1);
        else if (vs && a && a->t == TT_VAR && a->v.sval && pas_pf_is_formal(a->v.sval)) {
            if (strcmp(pas_pf_lookup(a->v.sval)->sig, vs)) {
                fprintf(stderr, "pascal: ISO 7185 6.6.3.3 violation: the actual-parameter %d of '%s' is not of the type of its variable-parameter\n", i / 2 + 1, callee);
                g_lower_pas.sem.pas_iso_errors++;
            }
            for (int w = 0; w <= 3; w++) { pnl_push(out, leaf_s(TT_VAR, w ? pas_pf_env_name(a->v.sval, w) : a->v.sval)); pnl_push(out, args->items[i + 1]); }
        } else if (vs) {
            fprintf(stderr, "pascal: the actual-parameter %d of '%s' passes a procedural variable-parameter that is not a whole variable; that is not implemented\n", i / 2 + 1, callee);
            g_lower_pas.sem.pas_iso_errors++;
        } else {
            pnl_push(out, a);
            pnl_push(out, args->items[i + 1]);
        }
    }
    return out;
}
static tree_t *pas_pf_envcall(tree_t *call, const char *pp, int lvl, int n) {
    tree_t *e = mk_fnc1("__pas_envcall", ilit(lvl));
    for (int k = 1; k <= 3; k++) ast_push(e, leaf_s(TT_VAR, pas_pf_env_name(pp, k)));
    for (int k = 1; k <= 3; k++) { char b[48]; snprintf(b, sizeof b, "__pas_vptmp_pfs%d_%d", n, k); ast_push(e, leaf_s(TT_VAR, ct_strdup(b))); }
    for (int i = 0; i < call->n; i++) ast_push(e, call->c[i]);
    return e;
}
static int pas_tm_fits(const PasDef *c, const char *fs) { const char *ns = pas_pf_no_self(c->sig); return !strcmp(c->sig, fs) || (ns && !strcmp(ns, fs)); }
static void pas_tm_candidates(const char *fs) {
    for (int i = 0; i < g_lower_pas.sem.pas_scope.n; i++) {
        const PasDef *d = &g_lower_pas.sem.pas_scope.defs[i];
        if (!d->name || d->fld || !d->sig || strncmp(d->name, "__pc_", 5)) continue;
        const char *ns = pas_pf_no_self(d->sig);
        if (ns && !strcmp(ns, fs)) pas_pf_candidate(d);
    }
    const PasDef *ca = g_lower_pas.sem.pas_scope.cand;
    for (int i = 0; i < g_lower_pas.sem.pas_scope.ncand; i++) for (int j = i + 1; j < g_lower_pas.sem.pas_scope.ncand; j++) {
        if (strncmp(ca[i].name, "__pc_", 5) || strncmp(ca[j].name, "__pc_", 5) || !pas_tm_fits(&ca[i], fs) || !pas_tm_fits(&ca[j], fs)) continue;
        if (strcmp(ca[i].name, ca[j].name) == 0 || pas_hash_stable(ca[i].name) != pas_hash_stable(ca[j].name)) continue;
        fprintf(stderr, "pascal: the methods '%s' and '%s' have the same code value in one TMethod call; the call cannot tell them apart\n", ca[i].name, ca[j].name);
        g_lower_pas.sem.pas_iso_errors++;
    }
}
void pas_pf_resolve(tree_t *e) {
    if (!e) return;
    for (int i = 0; i < e->n; i++) pas_pf_resolve(e->c[i]);
    if (e->t != TT_FNC || e->n < 4 || !e->c[0] || !e->c[0]->v.sval || strcmp(e->c[0]->v.sval, "__pas_pcall")) return;
    int ex = e->c[1]->t != TT_VAR;
    const char *pp = ex ? NULL : e->c[1]->v.sval, *fs = e->c[2]->v.sval;
    int isf = fs[0] == 'F', n = g_lower_pas.sem.pas_scope.npf++;
    char tb[40];
    snprintf(tb, sizeof tb, "__pas_vptmp_pf%d", n);
    const char *tmp = ct_strdup(tb);
    tree_t *chain = mk_fnc1("__pas_rterr", leaf_s(TT_QLIT, "6.6.3.4"));
    ast_push(chain, leaf_s(TT_QLIT, "a procedural or functional parameter denotes no routine"));
    int hm = e->c[3] && e->c[3]->t == TT_ILIT && e->c[3]->v.ival == 1;
    if (hm) pas_tm_candidates(fs);
    for (int ci = g_lower_pas.sem.pas_scope.ncand - 1; ci >= 0; ci--) {
        const PasDef *c = &g_lower_pas.sem.pas_scope.cand[ci];
        const char *ns = (strcmp(c->sig, fs) && !strncmp(c->name, "__pc_", 5)) ? pas_pf_no_self(c->sig) : NULL;
        int meth = ns && !strcmp(ns, fs);
        if (strcmp(c->sig, fs) && !meth) continue;
        if (ex && (meth || c->depth >= 1)) continue;
        PNodeList *al = pnl_new();
        if (meth) { pnl_push(al, leaf_s(TT_VAR, pas_pf_env_name(pp, 1))); pnl_push(al, ilit(-1)); }
        for (int i = 4; i < e->n; i++) { pnl_push(al, pas_tree_clone(e->c[i])); pnl_push(al, ilit(-1)); }
        tree_t *call = lpt_mk_call(c->name, al);
        if (!ex && c->depth >= 1 && call && call->t == TT_FNC) call = pas_pf_envcall(call, pp, c->depth + 1, n);
        tree_t *iff = ast_node_new(TT_IF);
        ast_push(iff, bin(TT_EQ, ex ? pas_tree_clone(e->c[1]) : leaf_s(TT_VAR, pp), ilit(hm ? pas_hash_stable(c->name) : c->rid)));
        ast_push(iff, isf ? mk_assign(leaf_s(TT_VAR, tmp), call) : call);
        ast_push(iff, chain);
        chain = iff;
    }
    if (isf) { tree_t *sq = ast_node_new(TT_SEQ_EXPR); ast_push(sq, chain); ast_push(sq, leaf_s(TT_VAR, tmp)); chain = sq; }
    *e = *chain;
}
char *tok_s(const tree_t *x) { tok_events(x); return x->v.sval; }
static pval E_argument_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) { v[1] = E_argument(ce); { out.list = v[1].list; } } else { v[1] = out; v[3] = E_argument(ce); { out.list = pnl_concat(v[1].list, v[3].list); } }
    }
    return out;
}
static int pas_set_has_range(const tree_t *set) { for (int i = 0; i < set->n; i++) if (IS(set->c[i], TT_SUBRANGE)) return 1; return 0; }
tree_t *pas_src_call(const char *fn, tree_t *a, tree_t *b) { tree_t *f = ast_node_new(TT_FNC); ast_push(f, leaf_s(TT_VAR, fn)); ast_push(f, a); if (b) ast_push(f, b); return f; }
static tree_t *pas_dynarr_append_src(const tree_t *lhs, const tree_t *set) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR), *a = (tree_t *)lhs;
    ast_push(seq, pas_src_call("setlength", a, bin(TT_ADD, pas_src_call("length", a, NULL), ilit(set->n))));
    for (int i = 0; i < set->n; i++) ast_push(seq, bin(TT_ASSIGN, bin(TT_IDX, a, bin(TT_SUB, pas_src_call("length", a, NULL), ilit(set->n - i))), set->c[i]));
    return seq;
}
static tree_t *pas_dynarr_assign_src(const tree_t *lhs, const tree_t *set) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR), *a = (tree_t *)lhs;
    ast_push(seq, pas_src_call("setlength", a, ilit(set->n)));
    for (int i = 0; i < set->n; i++) ast_push(seq, bin(TT_ASSIGN, bin(TT_IDX, a, ilit(i)), set->c[i]));
    return seq;
}
static tree_t *pas_pf_assign(const tree_t *n) {
    const char *nm = n->c[0]->v.sval;
    const PasDef *d = pas_pf_lookup(nm);
    PNodeList *c = pnl_new();
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    if (IS(n->c[1], TT_VAR) && n->c[1]->v.sval && !strcasecmp(n->c[1]->v.sval, "nil")) for (int k = 0; k < 4; k++) {
        pnl_push(c, ilit(0));
        pnl_push(c, ilit(-1));
    } else pas_pf_closure(c, E_expression(n->c[1]).node, d->sig, nm, 1);
    ast_push(seq, mk_assign(leaf_s(TT_VAR, nm), c->items[0]));
    for (int k = 1; k <= 3; k++) ast_push(seq, mk_assign(leaf_s(TT_VAR, pas_pf_env_name(nm, k)), c->items[2 * k]));
    return seq;
}
pval E_assignment(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_ASSIGN)) {
        if (n->n == 2 && IS(n->c[0], TT_VAR) && n->c[0]->v.sval && pas_pf_is_formal(n->c[0]->v.sval)) { out.node = pas_pf_assign(n); return out; }
        if (n->n == 2 && IS(n->c[0], TT_FIELD) && pas_pf_field_sig(n->c[0])) { out.node = pas_pf_field_assign(n); return out; }
        if (n->n == 2 && IS(n->c[0], TT_IDX) && n->c[0]->n == 2 && IS(n->c[0]->c[1], TT_ILIT) && n->c[0]->c[1]->v.ival == 0) {
            pval _b = E_selector(n->c[0]->c[0]);
            int _n0 = pty_shortmax(_b.node);
            if (_n0 > 0) {
                tree_t *_s0 = ast_node_new(TT_FNC);
                ast_push(_s0, leaf_s(TT_VAR, "__pas_sstr_store"));
                ast_push(_s0, pas_tree_clone(_b.node));
                ast_push(_s0, ilit(0));
                ast_push(_s0, pas_bool(E_expression(n->c[1]).node));
                ast_push(_s0, ilit(_n0));
                out.node = pas_trace_assigned(pas_trace_lhs(_b.node), mk_assign(_b.node, _s0));
                return out;
            }
        }
        v[1] = E_selector(n->c[0]);
        if (n->n == 3 && IS(n->c[2], TT_KEYWORD) && n->c[2]->v.sval && !strcmp(n->c[2]->v.sval, "+=") && IS(n->c[1], TT_SET) && pas_dyn_depth(v[1].node, NULL) > 0 && !pas_set_has_range(n->c[1]))
            return E_compound_statement(pas_dynarr_append_src(n->c[0], n->c[1]));
        if (n->n == 2 && IS(n->c[1], TT_SET) && n->c[1]->n > 0 && pas_dyn_depth(v[1].node, NULL) > 0 && !pas_set_has_range(n->c[1]))
            return E_compound_statement(pas_dynarr_assign_src(n->c[0], n->c[1]));
        v[3] = E_expression(n->c[1]);
        if (n->n == 2 && !IS(n->c[1], TT_SET)) {
            const pty_t *_lt = pty_of_src(n->c[0]), *_rt = pty_of_src(n->c[1]);
            int _tr = pty_real_rank(_lt), _rr = pty_real_rank(_rt);
            if (_tr == 2 && (_rr != 2 || IS(n->c[1], TT_FLIT)) && (_rr >= 0 || (_rt && _rt->k == PTK_INT))) v[3].node = pas_xr_wrap(n->c[1], v[3].node);
            else if (_tr >= 0 && _tr < 2 && _rr == 2) v[3].node = pas_xr_cvtn(_tr == 0 ? 5 : 4, v[3].node);
        }
        if (n->n == 3 && n->c[2] && n->c[2]->t == TT_KEYWORD && n->c[2]->v.sval) {
            const char *op = n->c[2]->v.sval;
            tree_t *cur = E_expression(n->c[0]).node;
            const pty_t *_clt = pty_of_src(n->c[0]), *_crt = pty_of_src(n->c[1]);
            int _ctr = pty_real_rank(_clt), _crr = pty_real_rank(_crt);
            if (_ctr == 2 && (_crr != 2 || IS(n->c[1], TT_FLIT)) && (_crr >= 0 || (_crt && _crt->k == PTK_INT))) v[3].node = pas_xr_wrap(n->c[1], v[3].node);
            v[3].node = op[0] == '+' ? pas_arith_or_set(TT_ADD, "__pas_setuni", cur, v[3].node) : op[0] == '-' ? pas_arith_or_set(TT_SUB, "__pas_setdif", cur, v[3].node) : op[0] == '*' ?
                pas_arith_or_set(TT_MUL, "__pas_setint", cur, v[3].node) : pas_rdiv(cur, v[3].node);
            if (_ctr >= 0 && _ctr < 2 && _crr == 2) v[3].node = pas_xr_cvtn(_ctr == 0 ? 5 : 4, v[3].node);
        }
        {
            if (v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) {
                pas_value_compat(v[1].node->v.sval, v[3].node, "6.8.2.2", "the variable");
                pas_string_assign_check(v[1].node->v.sval, v[3].node);
            }
            pas_deref_assign_compat(v[1].node, v[3].node);
            tree_t *_tl = pas_trace_lhs(v[1].node);
            if (v[1].node && v[1].node->t == TT_IDX && v[1].node->n == 2 && pty_shortmax(v[1].node->c[0]) > 0) {
                tree_t *_ss = ast_node_new(TT_FNC);
                ast_push(_ss, leaf_s(TT_VAR, "__pas_sstr_store"));
                ast_push(_ss, pas_tree_clone(v[1].node->c[0]));
                ast_push(_ss, v[1].node->c[1]);
                ast_push(_ss, pas_bool(v[3].node));
                ast_push(_ss, ilit(pty_shortmax(v[1].node->c[0])));
                out.node = pas_trace_assigned(_tl, mk_assign(v[1].node->c[0], _ss));
            }
                else if (v[1].node && v[1].node->t == TT_FNC && v[1].node->n == 2 && v[1].node->c[0] && v[1].node->c[0]->v.sval &&
                (!strcmp(v[1].node->c[0]->v.sval, "__pas_fbuf_get") || !strcmp(v[1].node->c[0]->v.sval, "__pas_tbuf_get"))) {
                int _isqlit = v[3].node &&
                    (v[3].node->t == TT_QLIT || (v[3].node->t == TT_FNC && v[3].node->n >= 1 && v[3].node->c[0] && v[3].node->c[0]->v.sval && !strcmp(v[3].node->c[0]->v.sval, "__pas_chrlit")));
                if (!strcmp(v[1].node->c[0]->v.sval, "__pas_fbuf_get") && v[1].node->c[1] && v[1].node->c[1]->t == TT_VAR && v[1].node->c[1]->v.sval && _isqlit &&
                    pas_tfcomp_nonchar(v[1].node->c[1]->v.sval)) {
                    fprintf(stderr, "pascal: ISO 7185 6.6.5.2 violation: the value assigned to the buffer-variable of '%s' is not assignment-compatible with its component-type\n",
                        v[1].node->c[1]->v.sval);
                    g_lower_pas.sem.pas_iso_errors++;
                }
                out.node = pas_trace_assigned(_tl, mk_set_bin(!strcmp(v[1].node->c[0]->v.sval, "__pas_tbuf_get") ? "__pas_tbuf_set" : "__pas_fbuf_set", v[1].node->c[1], pas_bool(v[3].node)));
            } else if (v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && pas_is_chararr(v[1].node->v.sval) && v[3].node && v[3].node->t == TT_QLIT && v[3].node->v.sval) {
                long long _cah;
                if (!pas_array_high_get(v[1].node->v.sval, &_cah)) _cah = (long long)strlen(v[3].node->v.sval);
                out.node = pas_trace_assigned(_tl, mk_assign(v[1].node, pas_str_to_alpha(v[3].node->v.sval, pas_chararr_lo(v[1].node->v.sval), _cah)));
            }
                else if (v[1].node && v[1].node->t == TT_IDX && v[1].node->n == 2 && v[1].node->c[0] && v[1].node->c[0]->t == TT_VAR && v[1].node->c[0]->v.sval &&
                pas_is_strarr(v[1].node->c[0]->v.sval) && v[3].node && v[3].node->t == TT_QLIT && v[3].node->v.sval) {
                long long _slo = pas_strarr_lo(v[1].node->c[0]->v.sval);
                long long _shi = _slo + (long long)strlen(v[3].node->v.sval) - 1;
                out.node = pas_trace_assigned(_tl, mk_assign(v[1].node, pas_str_to_alpha(v[3].node->v.sval, _slo, _shi)));
            }
                else if (v[1].node && v[1].node->t == TT_IDX && v[1].node->n == 2 && v[1].node->c[0] && v[1].node->c[0]->t == TT_VAR && v[1].node->c[0]->v.sval &&
                pas_is_strarr(v[1].node->c[0]->v.sval) && pas_ca_is_read(v[3].node)) {
                tree_t *_en = ast_node_new(TT_FNC);
                ast_push(_en, leaf_s(TT_VAR, "__pas_ca_encode"));
                ast_push(_en, v[3].node);
                ast_push(_en, ilit(pas_strarr_lo(v[1].node->c[0]->v.sval)));
                ast_push(_en, ilit(-1));
                out.node = pas_trace_assigned(_tl, mk_assign(v[1].node, _en));
            } else if (v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && pas_is_chararr(v[1].node->v.sval) && pas_ca_is_read(v[3].node)) {
                long long _ceh;
                if (!pas_array_high_get(v[1].node->v.sval, &_ceh)) _ceh = -1;
                tree_t *_en = ast_node_new(TT_FNC);
                ast_push(_en, leaf_s(TT_VAR, "__pas_ca_encode"));
                ast_push(_en, v[3].node);
                ast_push(_en, ilit(pas_chararr_lo(v[1].node->v.sval)));
                ast_push(_en, ilit(_ceh));
                out.node = pas_trace_assigned(_tl, mk_assign(v[1].node, _en));
            } else if (pas_is_cafield(v[1].node)) {
                tree_t *_rhs;
                long long _flo = pas_cafield_lo_get(v[1].node);
                if (v[3].node && v[3].node->t == TT_QLIT && v[3].node->v.sval) {
                    long long _fhi = pas_cafield_hi_get(v[1].node);
                    if (_fhi < _flo) _fhi = _flo + (long long)strlen(v[3].node->v.sval) - 1;
                    _rhs = pas_str_padded(v[3].node->v.sval, _fhi - _flo + 1);
                } else _rhs = pas_bool(v[3].node);
                tree_t *_pk = _rhs;
                if (!pas_ca_is_read(_rhs) && _rhs->t != TT_QLIT) { _pk = ast_node_new(TT_FNC); ast_push(_pk, leaf_s(TT_VAR, "__pas_ca_pack")); ast_push(_pk, _rhs); ast_push(_pk, ilit(_flo)); }
                out.node = pas_trace_assigned(_tl, mk_assign(v[1].node, _pk));
            } else {
                tree_t *_rhs0 = pas_bool(v[3].node);
                if (g_lower_pas.sem.pas_range_check_on && v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) {
                    long long _rlo, _rhi;
                    if (pas_subvar_get(v[1].node->v.sval, &_rlo, &_rhi)) _rhs0 = pas_range_wrap(_rhs0, _rlo, _rhi, pas_is_setvar(v[1].node->v.sval) ? "set" : "6.8.2.2");
                }
                int _sm = pty_shortmax(v[1].node);
                int _scat = n->n == 3 && IS(n->c[2], TT_KEYWORD) && n->c[2]->v.sval && n->c[2]->v.sval[0] == '+';
                if (_sm > 0 && (_scat || !pty_string_within(v[3].node, _sm))) _rhs0 = mk_fnc2("__pas_sstr_fit", _rhs0, ilit(_sm));
                out.node = pas_trace_assigned(_tl, mk_assign(v[1].node, _rhs0));
            }
            out.node = pas_vt_wrap_stmt(v[1].node, v[3].node, out.node);
        }
        return out;
    }
    elab_bad("assignment", n);
    return out;
}
pval E_call(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_FNC) && n->n == 1 && IS(n->c[0], TT_FIELD) && n->c[0]->n == 2) { tree_t *_sh = pas_strhlp_prop(n->c[0]); if (_sh) return pas_strhlp_eval(_sh); }
    if (IS(n, TT_FNC) && n->n == 1) {
        const char *src0 = tok_s(n->c[0]);
        v[1].str = pas_alias_resolve(src0);
        { int onp = 0; const char *ov = pas_overload_pick(src0, NULL, &onp); if (ov) v[1].str = ov; }
        {
            if (pty_name_type(v[1].str) && pty_name_type(v[1].str) == pty_type_by_name("tmethod")) out.node = pas_tm_call(v[1].str);
            else if (pas_pf_is_formal(v[1].str)) out.node = pas_pf_callthrough(v[1].str, NULL);
            else if (pas_default_get(v[1].str, 0)) {
                PNodeList *dl = pas_defaults_extend(v[1].str, NULL);
                pas_call_arity(v[1].str, dl);
                out.node = lpt_mk_call(v[1].str, dl);
            } else if (pas_is_proc(v[1].str)) {
                pas_call_arity(v[1].str, NULL);
                tree_t *e = ast_node_new(TT_FNC);
                ast_push(e, leaf_s(TT_VAR, v[1].str));
                out.node = e;
            } else {
                pas_required_needs_params(v[1].str);
                pas_call_arity(v[1].str, NULL);
                out.node = lpt_mk_call(v[1].str, NULL);
            }
        }
        return out;
    }
    if (n != NULL) { v[1] = E_call_with_args(n); { out.node = v[1].node; } return out; }
    elab_bad("call", n);
    return out;
}
static int pas_split_opt_arg(const tree_t *a) {
    const pty_t *t = pty_of_src(a);
    const tree_t *m = IS(a, TT_FIELD) && a->n == 2 ? a->c[1] : a;
    if (t) return t->k == PTK_ENUM;
    return IS(m, TT_VAR) && m->v.sval && (!strcasecmp(m->v.sval, "none") || !strcasecmp(m->v.sval, "excludeempty") || !strcasecmp(m->v.sval, "excludelastempty"));
}
static int pas_strhlp_type_recv(const tree_t *r) {
    static const char *const T[] = { "string", "ansistring", "unicodestring", 0 };
    for (int i = 0; IS(r, TT_VAR) && r->v.sval && T[i]; i++) if (!strcasecmp(r->v.sval, T[i])) return 1;
    return 0;
}
static tree_t *pas_strhlp_call(const tree_t *n) {
    const tree_t *r = n->c[0]->c[0], *mt = n->c[0]->c[1];
    const char *m = IS(mt, TT_VAR) ? mt->v.sval : NULL;
    const pty_t *rt = pty_of_src(r);
    int na = n->n - 1;
    tree_t *c = NULL;
    if (!m || !r) return NULL;
    if (!strcasecmp(m, "join") && pas_strhlp_type_recv(r) && (na == 2 || na == 4)) {
        c = ast_node_new(TT_FNC);
        ast_push(c, leaf_s(TT_VAR, na == 2 ? "__strhlp_join" : "__strhlp_join_r"));
        for (int i = 1; i <= na; i++) ast_push(c, n->c[i]);
    } else if (!strcasecmp(m, "split") && rt && rt->k == PTK_STRING && na >= 1 && na <= 3) {
        const pty_t *st = pty_of_src(n->c[1]);
        tree_t *seps = n->c[1], *cnt = NULL, *opt = NULL;
        if (!IS(seps, TT_SET) && !(st && st->k == PTK_ARRAY)) { seps = ast_node_new(TT_SET); ast_push(seps, n->c[1]); }
        if (na == 2) { if (pas_split_opt_arg(n->c[2])) opt = n->c[2]; else cnt = n->c[2]; }
        if (na == 3) { cnt = n->c[2]; opt = n->c[3]; }
        c = ast_node_new(TT_FNC);
        ast_push(c, leaf_s(TT_VAR, "__strhlp_split"));
        ast_push(c, (tree_t *)r);
        ast_push(c, seps);
        ast_push(c, cnt ? cnt : ilit(0));
        ast_push(c, opt ? opt : ilit(0));
    } else if (rt && rt->k == PTK_STRING) {
        static const char *const A1[] = { "contains", "startswith", "endswith", "indexof", 0 };
        int hit = 0;
        for (int i = 0; A1[i]; i++) if (na == 1 && !strcasecmp(m, A1[i])) hit = 1;
        if (hit || (!strcasecmp(m, "replace") && (na == 2 || na == 3)) || (!strcasecmp(m, "substring") && (na == 1 || na == 2))) {
            const char *fn = !strcasecmp(m, "substring") ? (na == 1 ? "__strhlp_substring" : "__strhlp_substring_r") : NULL;
            char *nm = NULL;
            if (!fn) { size_t ln = strlen(m) + 10; nm = (char *)ct_alloc(ln); snprintf(nm, ln, "__strhlp_%s", m); for (char *q = nm + 9; *q; q++) *q = (char)tolower((unsigned char)*q); fn = nm; }
            c = ast_node_new(TT_FNC);
            ast_push(c, leaf_s(TT_VAR, fn));
            ast_push(c, (tree_t *)r);
            for (int i = 1; i <= na; i++) ast_push(c, n->c[i]);
            if (!strcasecmp(m, "replace") && na == 2) { tree_t *fl = ast_node_new(TT_SET); ast_push(fl, leaf_s(TT_VAR, "rfreplaceall")); ast_push(c, fl); }
        }
    }
    if (c) c->line = n->line;
    return c;
}
tree_t *pas_strhlp_prop(const tree_t *n) {
    static const char *const Z[] = { "trim", "trimleft", "trimright", "toupper", "tolower", "length", 0 };
    const tree_t *r = n->c[0], *mt = n->c[1];
    const char *m = IS(mt, TT_VAR) ? mt->v.sval : NULL;
    const pty_t *rt = r ? pty_of_src(r) : NULL;
    if (!m || !rt || rt->k != PTK_STRING) return NULL;
    for (int i = 0; Z[i]; i++) if (!strcasecmp(m, Z[i])) {
        size_t ln = strlen(m) + 10;
        char *nm = (char *)ct_alloc(ln);
        snprintf(nm, ln, "__strhlp_%s", m);
        for (char *q = nm + 9; *q; q++) *q = (char)tolower((unsigned char)*q);
        tree_t *c = ast_node_new(TT_FNC);
        ast_push(c, leaf_s(TT_VAR, nm));
        ast_push(c, (tree_t *)r);
        c->line = n->line;
        return c;
    }
    return NULL;
}
pval pas_strhlp_eval(tree_t *sh) {
    pval o = E_call_with_args(sh);
    const char *fn = IS(sh->c[0], TT_VAR) ? sh->c[0]->v.sval : NULL;
    if (o.node && fn && (!strcmp(fn, "__strhlp_contains") || !strcmp(fn, "__strhlp_startswith") || !strcmp(fn, "__strhlp_endswith"))) pty_note_kind(o.node, PTK_BOOL);
    return o;
}
pval E_call_with_args(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (pas_xr_call(n, &out)) return out;
    if (IS(n, TT_FNC) && n->n == 1 && IS(n->c[0], TT_FIELD) && n->c[0]->n == 2) { tree_t *_sh = pas_strhlp_prop(n->c[0]); if (_sh) return pas_strhlp_eval(_sh); }
    if (IS(n, TT_FNC)) {
        if (n->n == 2 && IS(n->c[0], TT_VAR) && n->c[0]->v.sval && !strcmp(n->c[0]->v.sval, "exit") && IS(n->c[1], TT_SET) && n->c[1]->n > 0 && !pas_set_has_range(n->c[1]) && pas_curfunc_name() &&
            pas_dyn_depth(E_selector(leaf_s(TT_VAR, "result")).node, NULL) > 0) {
            tree_t *_seq = pas_dynarr_assign_src(leaf_s(TT_VAR, "result"), n->c[1]);
            ast_push(_seq, mk_fnc0("exit"));
            return E_compound_statement(_seq);
        }
        if (n->n == 2 && IS(n->c[0], TT_VAR) && n->c[0]->v.sval && !strcmp(n->c[0]->v.sval, "__pas_untyped_actual")) {
            tree_t *_ua = (IS(n->c[1], TT_VAR) || IS(n->c[1], TT_IDX) || IS(n->c[1], TT_FIELD) || IS(n->c[1], TT_DEREF)) ? pas_addr_of(E_selector(n->c[1]).node) : NULL;
            out.node = _ua ? pas_untyped_view(_ua) : E_expression(n->c[1]).node;
            return out;
        }
        if (n->n == 3 && IS(n->c[0], TT_VAR) && n->c[0]->v.sval && !strcmp(n->c[0]->v.sval, "__pas_mptr")) {
            out.node = mk_fnc2("__pas_mptr", leaf_s(TT_QLIT, n->c[1]->v.sval), E_expression(n->c[2]).node);
            return out;
        }
        if (n->n >= 2 && IS(n->c[0], TT_FIELD) && n->c[0]->n == 2) { tree_t *sh = pas_strhlp_call(n); if (sh) return pas_strhlp_eval(sh); }
        if (n->n >= 1 && IS(n->c[0], TT_FIELD) && n->c[0]->n == 2 && pas_pf_field_sig(n->c[0])) {
            tree_t *pc = mk_fnc1("__pas_pcall", E_selector(n->c[0]).node);
            ast_push(pc, leaf_s(TT_QLIT, pas_pf_field_sig(n->c[0])));
            ast_push(pc, ilit(0));
            PNodeList *al = E_argument_list(n, 1, n->n).list;
            for (int i = 0; al && i + 1 < al->count; i += 2) ast_push(pc, al->items[i]);
            out.node = pc;
            return out;
        }
        const char *src0 = tok_s(n->c[0]);
        const char *dflt = NULL;
        int onp = 0;
        v[1].str = pas_alias_resolve(src0);
        if (g_lower_pas.sem.pas_ptrcell && n->n == 2 && src0 && (!strcmp(src0, "pchar") || !strcmp(src0, "pansichar") || !strcmp(src0, "pwidechar"))) {
            v[3] = E_expression(n->c[1]);
            out.node = pas_ptr_typed(v[3].node) ? v[3].node : pas_pchar_of_strelem(v[3].node);
            if (!out.node) out.node = mk_fnc1("__pas_pchar_of", v[3].node);
            return out;
        }
        if (n->n == 2 && v[1].str && !pas_is_func(v[1].str) && !pas_is_proc(v[1].str) && (!strcmp(v[1].str, "ansistring") || !strcmp(v[1].str, "string") || !strcmp(v[1].str, "unicodestring")) &&
            pty_of_src(n->c[1]) && pty_of_src(n->c[1])->k == PTK_POINTER) return E_expression(n->c[1]);
        {
            const char *_pct = pas_ptrcast_target(n);
            if (_pct) { const pty_t *_pp = pty_named_type(_pct); pval _pv = E_expression(n->c[1]); if (_pp) _pv.node = pas_pun_view_to(_pp, _pv.node); return _pv; }
        }
        if (v[1].str && !strcmp(v[1].str, "length") && !pas_is_func(v[1].str) && !pas_is_proc(v[1].str) && n->n == 2 && IS(n->c[1], TT_IDX) && n->c[1]->n == 2 && IS(n->c[1]->c[0], TT_VAR)) {
            tree_t *b = E_selector(n->c[1]->c[0]).node;
            long long d2[4];
            if (b && b->t == TT_VAR && pas_array_dims(b->v.sval, d2)) { out.node = ilit(d2[3] - d2[2] + 1); return out; }
        }
        if (v[1].str && !strcmp(v[1].str, "format") && n->n == 3 && IS(n->c[2], TT_SET) && !pas_is_func(v[1].str) && !pas_is_proc(v[1].str) && (out.node = pas_format_call(n))) return out;
        if (v[1].str && !strcmp(v[1].str, "format") && n->n == 3 && !IS(n->c[2], TT_SET) && !pas_is_func(v[1].str) && !pas_is_proc(v[1].str)) {
            out.node = mk_fnc2("__pas_format", E_expression(n->c[1]).node, E_expression(n->c[2]).node);
            return out;
        }
        if (pas_openparm_mask(v[1].str) != 0 || pas_pf_lookup(v[1].str)) {
            for (int i = 1; i < n->n; i++) {
                int _ac = IS(n->c[i], TT_SET) && ((i - 1 < 64 && ((pas_openparm_mask(v[1].str) >> (i - 1)) & 1ULL)) || pas_formal_is_dynarr(v[1].str, i - 1));
                g_lower_pas.sem.pas_arrctor = _ac && pas_formal_is_aoc(v[1].str, i - 1) ? 2 : _ac;
                pval _a = E_argument(n->c[i]);
                g_lower_pas.sem.pas_arrctor = 0;
                v[3].list = i == 1 ? _a.list : pnl_concat(v[3].list, _a.list);
            }
        } else {
            g_lower_pas.sem.pas_addr_int = n->n == 2 && IS(n->c[1], TT_ADDR) && pty_int_typename(src0);
            v[3] = E_argument_list(n, 1, n->n);
            g_lower_pas.sem.pas_addr_int = 0;
        }
        { const char *ov = pas_overload_pick(src0, v[3].list, &onp); if (ov) { v[1].str = ov; dflt = src0; } }
        if (v[1].str && (!strcmp(v[1].str, "max") || !strcmp(v[1].str, "min")) && v[3].list && v[3].list->count >= 4 && pas_is_func("fpcmaxdouble") &&
            (pas_expr_is_real(v[3].list->items[0]) || pas_expr_is_real(v[3].list->items[2]))) v[1].str = v[1].str[1] == 'a' ? "fpcmaxdouble" : "fpcmindouble";
        v[3].list = pas_defaults_extend(dflt ? dflt : v[1].str, v[3].list);
        if (dflt && v[3].list && v[3].list->count > 2 * onp) v[3].list->count = 2 * onp;
        {
            v[3].list = pas_pf_actuals(v[1].str, v[3].list);
            if (v[3].list) for (int i = 0; i + 1 < v[3].list->count; i += 2)
                if (pas_proc_param_is_var(v[1].str, i / 2) && IS(v[3].list->items[i], TT_FNC) && v[3].list->items[i]->n > 0 && IS(v[3].list->items[i]->c[0], TT_VAR) &&
                !strcmp(v[3].list->items[i]->c[0]->v.sval, "arr_slice")) {
                fprintf(stderr, "pascal: a slice a[lo..hi] is a copy and cannot be passed as the variable parameter %d of '%s'\n", i / 2 + 1, v[1].str);
                g_lower_pas.sem.pas_iso_errors++;
            }
            if (v[3].list) for (int i = 0; i < v[3].list->count; i++) if (pas_proc_param_is_var(v[1].str, i) && pas_actual_is_packed_component(v[3].list->items[i])) {
                fprintf(stderr, "pascal: ISO 7185 6.6.3.3 violation: a component of a packed structure is passed as the variable parameter %d of '%s'\n", i + 1, v[1].str);
                g_lower_pas.sem.pas_iso_errors++;
            }
            if (v[3].list) for (int i = 0; i < v[3].list->count; i++) if (pas_proc_param_is_var(v[1].str, i)) v[3].list->items[i] = pas_vt_strip_undef(v[3].list->items[i]);
            if (v[3].list) for (int i = 0; i < v[3].list->count; i++) if (pas_proc_param_is_var(v[1].str, i) && pas_actual_is_tagfield(v[3].list->items[i])) {
                fprintf(stderr, "pascal: ISO 7185 6.6.3.3 violation: a tag-field of a variant-part is passed as the variable parameter %d of '%s'\n", i + 1, v[1].str);
                g_lower_pas.sem.pas_iso_errors++;
            }
            pas_call_arity(v[1].str, v[3].list);
            pas_ordinal_fn_arg(v[1].str, v[3].list);
            if (pty_name_type(v[1].str) && pty_name_type(v[1].str) == pty_type_by_name("tmethod") && v[3].list && v[3].list->count > 0) {
                fprintf(stderr, "pascal: a call through the TMethod record '%s' with arguments is not implemented; only a call without arguments is\n", v[1].str);
                g_lower_pas.sem.pas_iso_errors++;
            }
            out.node = pas_pf_is_formal(v[1].str) ? pas_pf_callthrough(v[1].str, v[3].list) : lpt_mk_call(v[1].str, v[3].list);
        }
        return out;
    }
    elab_bad("call_with_args", n);
    return out;
}
pval E_parameter_decl(const tree_t *n) {
    pval v[8];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int hasp = n->n > 1 && n->c[1] && n->c[1]->t == TT_PARAMS;
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "procedure")) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pf_params(hasp ? n->c[1] : NULL);
        { out.list = pas_pf_formal(v[2].str, pas_pf_sig('P', v[3].str, NULL), 0); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "function")) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pf_params(hasp ? n->c[1] : NULL);
        v[5].str = tok_s(n->c[n->n - 1]);
        { out.list = pas_pf_formal(v[2].str, pas_pf_sig('F', v[3].str, v[5].str), 0); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "var") && pas_proctype_get(tok_s(n->c[1]))) {
        v[2] = E_id_list(n->c[0], 0, n->c[0]->n);
        pas_pf_section('r', v[2].list, tok_s(n->c[1]));
        PNodeList *all = pnl_new();
        for (int i = 0; v[2].list && i < v[2].list->count; i++) if (v[2].list->items[i] && v[2].list->items[i]->v.sval) {
            const char *nm = v[2].list->items[i]->v.sval;
            pas_not_a_word_symbol(nm);
            pas_pf_pend(nm, pas_proctype_get(tok_s(n->c[1])));
            g_lower_pas.sem.pas_scope.pend[g_lower_pas.sem.pas_scope.npend - 1].depth = 1;
            for (int k = 0; k <= 3; k++) { tree_t *lf = leaf_s(TT_VAR, k ? pas_pf_env_name(nm, k) : nm); ast_push(lf, ast_node_new(TT_SUCCEED)); pnl_push(all, lf); }
        }
        out.list = all;
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "var")) {
        v[2] = E_id_list(n->c[0], 0, n->c[0]->n);
        v[4].str = tok_s(n->c[1]);
        { pas_pf_section('r', v[2].list, v[4].str); for (int i = 0; i < v[2].list->count; i++) if (v[2].list->items[i]) ast_push(v[2].list->items[i], ast_node_new(TT_SUCCEED)); out.list = v[2].list; }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "value") && pas_proctype_get(tok_s(n->c[1]))) {
        const char *sig = pas_proctype_get(tok_s(n->c[1]));
        v[1] = E_id_list(n->c[0], 0, n->c[0]->n);
        PNodeList *all = pnl_new();
        for (int i = 0; v[1].list && i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) {
            PNodeList *one = pas_pf_formal(v[1].list->items[i]->v.sval, sig, 1);
            for (int k = 0; k < one->count; k++) pnl_push(all, one->items[k]);
        }
        out.list = all;
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "value")) {
        v[1] = E_id_list(n->c[0], 0, n->c[0]->n);
        v[3].str = tok_s(n->c[1]);
        {
            pas_value_formal_file_check(v[1].list, v[3].str);
            pas_pf_section('v', v[1].list, v[3].str);
            if (pas_is_realtypename(v[3].str)) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i]) ast_push(v[1].list->items[i], ast_node_new(TT_FLIT));
            out.list = v[1].list;
        }
        return out;
    }
    elab_bad("parameter_decl", n);
    return out;
}
pval E_procedure_decl(const tree_t *n) {
    const void *pc = pty_enter(n);
    const char *nm = n->n > 0 && n->c[0] ? tok_s(n->c[0]) : NULL;
    int sv = g_lower_pas.sem.pas_divchk, svr = g_lower_pas.sem.pas_crt_raw;
    if (nm && !strncmp(nm, "__sh_", 5)) g_lower_pas.sem.pas_divchk = 0;
    if (nm && !strncasecmp(nm, "crt.", 4)) g_lower_pas.sem.pas_crt_raw = 1;
    pval o = E_procedure_decl_in(n);
    g_lower_pas.sem.pas_divchk = sv;
    g_lower_pas.sem.pas_crt_raw = svr;
    pty_leave(pc);
    return o;
}
pval E_selector(const tree_t *n) { pval o = E_selector_raw(n); pty_note_low(o.node, n); return o; }
