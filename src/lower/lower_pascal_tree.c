#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ct_arena.h"
#include "ast.h"
#include "../parsers/snobol4/scrip_cc.h"
#include "lower.h"
#include "g_lower.h"
#define IS(x, k) ((x) && (x)->t == (k))
static int pas_trace_enabled(void) { extern long g_trace_budget; return g_trace_budget != 0; }
static PNodeList *pnl_new(void) { PNodeList *l = (PNodeList *)ct_zalloc(1, sizeof *l); return l; }
static void pnl_push(PNodeList *l, tree_t *e) {
    if (!l) return;
    if (l->count >= l->cap) { l->cap = l->cap ? l->cap * 2 : 8; l->items = (tree_t **)ct_grow(l->items, (size_t)l->cap * sizeof(tree_t *)); }
    l->items[l->count++] = e;
}
static PNodeList *pnl_concat(PNodeList *a, PNodeList *b) { if (!b) return a; for (int i = 0; i < b->count; i++) pnl_push(a, b->items[i]); return a; }
static PasDef *pas_scope_push(PasDef **a, int *n, int *cap, const char *name) {
    if (*n >= *cap) { *cap = *cap ? *cap * 2 : 256; *a = (PasDef *)ct_grow(*a, (size_t)*cap * sizeof(PasDef)); }
    PasDef *d = &(*a)[(*n)++];
    d->name = name;
    d->sig = NULL;
    d->owner = NULL;
    d->depth = g_lower.pas.sem.pas_scope.depth;
    d->rid = 0;
    d->formal = 0;
    d->fld = g_lower.pas.sem.pas_recbody_depth > 0;
    d->uid = ++g_lower.pas.sem.pas_scope.nuid;
    return d;
}
static void pas_scope_defined_after_use(const char *name);
static void pas_scope_define(const char *name) {
    if (name) { pas_scope_defined_after_use(name); pas_scope_push(&g_lower.pas.sem.pas_scope.defs, &g_lower.pas.sem.pas_scope.n, &g_lower.pas.sem.pas_scope.cap, name); }
}
static int pas_scope_uid(const char *name) {
    for (int i = g_lower.pas.sem.pas_scope.n - 1; name && i >= 0; i--)
        if (g_lower.pas.sem.pas_scope.defs[i].name && !g_lower.pas.sem.pas_scope.defs[i].fld && !strcmp(g_lower.pas.sem.pas_scope.defs[i].name, name)) return g_lower.pas.sem.pas_scope.defs[i].uid;
    return 0;
}
static void pas_scope_define_list(PNodeList *ids) { for (int i = 0; ids && i < ids->count; i++) if (ids->items[i] && ids->items[i]->v.sval) pas_scope_define(ids->items[i]->v.sval); }
static void pas_scope_fwd_save(const char *name, PNodeList *params, const char *sig) {
    if (!name) return;
    if (g_lower.pas.sem.pas_scope.nfwd >= g_lower.pas.sem.pas_scope.fcap) {
        g_lower.pas.sem.pas_scope.fcap = g_lower.pas.sem.pas_scope.fcap ? g_lower.pas.sem.pas_scope.fcap * 2 : 32;
        g_lower.pas.sem.pas_scope.fwd = (PasFwd *)ct_grow(g_lower.pas.sem.pas_scope.fwd, (size_t)g_lower.pas.sem.pas_scope.fcap * sizeof(PasFwd));
    }
    g_lower.pas.sem.pas_scope.fwd[g_lower.pas.sem.pas_scope.nfwd].name = name;
    g_lower.pas.sem.pas_scope.fwd[g_lower.pas.sem.pas_scope.nfwd].sig = sig;
    g_lower.pas.sem.pas_scope.fwd[g_lower.pas.sem.pas_scope.nfwd++].params = params;
}
static const PasFwd *pas_scope_fwd_of(const char *name, PNodeList *params) {
    if ((!params || params->count == 0) && name) for (int i = g_lower.pas.sem.pas_scope.nfwd - 1; i >= 0; i--)
        if (g_lower.pas.sem.pas_scope.fwd[i].name && !strcmp(g_lower.pas.sem.pas_scope.fwd[i].name, name)) return &g_lower.pas.sem.pas_scope.fwd[i];
    return NULL;
}
static PNodeList *pas_scope_fwd_params(const char *name, PNodeList *params) { const PasFwd *f = pas_scope_fwd_of(name, params); return f ? f->params : params; }
static void pas_formal_subranges(const char *sig, PNodeList *params);
static void pas_formal_chararrs(const char *sig, PNodeList *params);
static void pas_formal_strtypes(const char *sig, PNodeList *params);
static void pas_formal_tfiles(const char *sig, PNodeList *params);
static void pas_scope_enter(const char *name, PNodeList *params) {
    if (g_lower.pas.sem.pas_scope.depth >= g_lower.pas.sem.pas_scope.mcap) {
        g_lower.pas.sem.pas_scope.mcap = g_lower.pas.sem.pas_scope.mcap ? g_lower.pas.sem.pas_scope.mcap * 2 : 16;
        g_lower.pas.sem.pas_scope.marks = (int *)ct_grow(g_lower.pas.sem.pas_scope.marks, (size_t)g_lower.pas.sem.pas_scope.mcap * sizeof(int));
    }
    g_lower.pas.sem.pas_scope.marks[g_lower.pas.sem.pas_scope.depth++] = g_lower.pas.sem.pas_scope.n;
    const PasFwd *fwd = pas_scope_fwd_of(name, params);
    if (fwd) params = fwd->params;
    int first = g_lower.pas.sem.pas_scope.n;
    pas_scope_define_list(params);
    for (int i = first; name && i < g_lower.pas.sem.pas_scope.n; i++) for (int j = g_lower.pas.sem.pas_scope.nfsig - 1; j >= 0; j--)
        if (!strcmp(g_lower.pas.sem.pas_scope.fsig[j].owner, name) && !strcmp(g_lower.pas.sem.pas_scope.fsig[j].name, g_lower.pas.sem.pas_scope.defs[i].name)) {
        g_lower.pas.sem.pas_scope.defs[i].sig = g_lower.pas.sem.pas_scope.fsig[j].sig;
        g_lower.pas.sem.pas_scope.defs[i].formal = 1;
        break;
    }
    if (first > 0 && g_lower.pas.sem.pas_scope.defs[first - 1].rid && name && !strcmp(g_lower.pas.sem.pas_scope.defs[first - 1].name, name)) {
        const char *sig = (fwd && fwd->sig) ? fwd->sig : g_lower.pas.sem.pas_scope.defs[first - 1].sig;
        pas_formal_subranges(sig, params);
        pas_formal_chararrs(sig, params);
        pas_formal_strtypes(sig, params);
        pas_formal_tfiles(sig, params);
    }
}
static int pas_pf_is_formal(const char *name);
static const PasDef *pas_pf_lookup(const char *name);
static tree_t *pas_pf_callthrough(const char *name, PNodeList *args);
static const char *pas_pf_rtype(const tree_t *e) {
    if (!e || e->t != TT_FNC || e->n < 4 || !e->c[0] || !e->c[0]->v.sval || strcmp(e->c[0]->v.sval, "__pas_pcall") || !e->c[2]->v.sval || e->c[2]->v.sval[0] != 'F') return NULL;
    const char *r = strrchr(e->c[2]->v.sval, ')');
    return (r && r[1] == ':') ? r + 2 : NULL;
}
static void pas_scope_exit(void) {
    if (g_lower.pas.sem.pas_scope.depth > 0) g_lower.pas.sem.pas_scope.n = g_lower.pas.sem.pas_scope.marks[--g_lower.pas.sem.pas_scope.depth];
    while (g_lower.pas.sem.pas_scope.napl > 0 && g_lower.pas.sem.pas_scope.apl[g_lower.pas.sem.pas_scope.napl - 1].depth > g_lower.pas.sem.pas_scope.depth) g_lower.pas.sem.pas_scope.napl--;
}
static int pas_is_int_typename(const char *t);
static int pas_is_realtypename(const char *t);
static int pas_is_settype(const char *name);
static int pas_scope_has(const char *name) {
    const char *req[] = { "true", "false", "maxint", "input", "output", "nil", "char", "boolean", "text", "string", "widechar" };
    if (!name) return 1;
    if (pas_is_int_typename(name) || pas_is_realtypename(name)) return 1;
    for (size_t i = 0; i < sizeof req / sizeof req[0]; i++) if (!strcmp(name, req[i])) return 1;
    for (int i = g_lower.pas.sem.pas_scope.n - 1; i >= 0; i--) if (g_lower.pas.sem.pas_scope.defs[i].name && !strcmp(g_lower.pas.sem.pas_scope.defs[i].name, name)) return 1;
    return 0;
}
static void pas_scope_require(const char *name);
static tree_t *leaf_s(tree_e k, const char *s) { tree_t *e = ast_node_new(k); e->v.sval = (char *)(s ? s : ""); return e; }
static tree_t *ilit(long long v) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = v; return e; }
static tree_t *flit(double v) { tree_t *e = ast_node_new(TT_FLIT); e->v.dval = v; return e; }
static tree_t *bin(tree_e k, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(k); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *un(tree_e k, tree_t *a) { tree_t *e = ast_node_new(k); ast_push(e, a); return e; }
static tree_t *mk_neg(tree_t *a) { if (a && a->t == TT_FLIT) { a->v.dval = -a->v.dval; return a; } return un(TT_MNS, a); }
static tree_t *prog_of(PNodeList *l) { tree_t *e = ast_node_new(TT_PROGRAM); if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]); return e; }
static tree_t *seq_of(PNodeList *l) { if (l && l->count == 1) return l->items[0]; tree_t *e = ast_node_new(TT_SEQ_EXPR); if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]); return e; }
static const char *map_io(const char *fn) {
    if (fn && !strcmp(fn, "writeln")) return "__pas_writeln";
    if (fn && !strcmp(fn, "write")) return "__pas_write";
    if (fn && !strcmp(fn, "sqr")) return "__pas_sqr";
    return fn;
}
static int is_pas_io(const char *fn) { return fn && (!strcmp(fn, "__pas_writeln") || !strcmp(fn, "__pas_write")); }
static const char *pas_ptrvar_target(const char *v);
static const char *pas_enumnames_by_idx(int i);
static char pas_typename_class(const char *t);
static tree_t *pas_vt_unwrap_read(tree_t *e);
static const char *pas_ptrexpr_target(tree_t *e);
static const char *pas_elem_rectype(tree_t *e);
static const char *pas_with_sel_rtype(tree_t *sel);
static const char *pas_recvar_field_rectype(const char *vn, long idx);
static int pas_rectype_nf(const char *rn);
static int pas_array_high_get(const char *name, long long *out);
static int pas_is_aggregate_designator(tree_t *e);
static tree_t *mk_rec_init_type(const char *rt);
static tree_t *pas_new_target_init(const char *rt);
static tree_t *mk_rec_init_type_d(const char *rt, int depth);
static long long pas_array_low(const char *name);
static long long pas_arrtype_high(const char *name);
static long long pas_arrtype_lo(const char *name);
static long long pas_index_bound(long long v, int want_hi);
static int pas_index_is_enum(long long v);
static int pas_array_isbool(const char *name);
static int pas_recvar_field_is_bool(const char *vn, long idx);
static int pas_rectype_field_is_bool(const char *rn, long idx);
static long long pas_array_decl_low(void);
static long long pas_array_nonparam_low(const char *name);
static int pas_subvar_get(const char *n, long long *lo, long long *hi);
static int pas_sizeof_lookup(const char *name, long long *out);
static int pas_sizeof_builtin_size(const char *n, long long *out);
static int pas_ordinal_bound_lookup(const char *name, long long *lo, long long *hi);
static int pas_var_string_kind(const char *name);
static int pas_tfcomp_range(const char *n, long long *lo, long long *hi);
static int pas_tfcomp_nonchar(const char *n);
static int pas_is_hdrfile(const char *n);
static void pas_assigned_add(const char *n);
static int pas_assigned_get(const char *n);
static int pas_array_is_param(const char *name);
static const char *pas_selector_base_name(tree_t *e);
static int pas_rectype_total_size(const char *rn, long long *out);
static int pas_rectype_field_offset(const char *rn, int idx, long long *out);
static const char *pas_typealias_get(const char *n);
static void pas_tsz_add(const char *name, long long bytes) {
    if (!name || bytes < 0) return;
    if (g_lower.pas.sem.pas_ntsz >= g_lower.pas.sem.pas_tszcap) {
        g_lower.pas.sem.pas_tszcap = g_lower.pas.sem.pas_tszcap ? g_lower.pas.sem.pas_tszcap * 2 : 64;
        g_lower.pas.sem.pas_tsz = (__typeof__(g_lower.pas.sem.pas_tsz))ct_grow(g_lower.pas.sem.pas_tsz, (size_t)g_lower.pas.sem.pas_tszcap * sizeof *g_lower.pas.sem.pas_tsz);
    }
    g_lower.pas.sem.pas_tsz[g_lower.pas.sem.pas_ntsz].name = ct_strdup(name);
    g_lower.pas.sem.pas_tsz[g_lower.pas.sem.pas_ntsz].bytes = bytes;
    g_lower.pas.sem.pas_ntsz++;
}
static int pas_tsz_get(const char *name, long long *out) {
    if (!name) return 0;
    for (int i = g_lower.pas.sem.pas_ntsz - 1; i >= 0; i--) if (!strcmp(g_lower.pas.sem.pas_tsz[i].name, name)) { *out = g_lower.pas.sem.pas_tsz[i].bytes; return 1; }
    return 0;
}
static int pas_array_is_zero_size(const char *name) { long long b; return pas_tsz_get(name, &b) && b == 0; }
#ifndef PAS_FIELD_MAX
#endif
static struct pas_vt *pas_vt_new(int state) {
    if (g_lower.pas.sem.pas_nvt >= g_lower.pas.sem.pas_vtcap) {
        g_lower.pas.sem.pas_vtcap = g_lower.pas.sem.pas_vtcap ? g_lower.pas.sem.pas_vtcap * 2 : 32;
        g_lower.pas.sem.pas_vt = (struct pas_vt **)ct_grow(g_lower.pas.sem.pas_vt, (size_t)g_lower.pas.sem.pas_vtcap * sizeof *g_lower.pas.sem.pas_vt);
    }
    struct pas_vt *v = (struct pas_vt *)ct_zalloc(1, sizeof *v);
    v->state = state;
    v->tagslot = -1;
    v->tagfi = -1;
    g_lower.pas.sem.pas_vt[g_lower.pas.sem.pas_nvt++] = v;
    return v;
}
static void pas_vt_open(void) { pas_vt_new(0); }
static struct pas_vt *pas_vt_top_open(void) { for (int i = g_lower.pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower.pas.sem.pas_vt[i]->state == 0) return g_lower.pas.sem.pas_vt[i]; return NULL; }
static int pas_vt_consts(tree_t *e, long long *v, int *n, int cap) {
    if (!e || *n >= cap) return 0;
    if (e->t == TT_ADD && e->n == 2) return pas_vt_consts(e->c[0], v, n, cap) && pas_vt_consts(e->c[1], v, n, cap);
    if (e->t == TT_EQ && e->n == 2 && e->c[1] && e->c[1]->t == TT_ILIT) { v[(*n)++] = e->c[1]->v.ival; return 1; }
    return 0;
}
static void pas_vt_arm(int lo, int hi, tree_t *consts) {
    struct pas_vt *t = pas_vt_top_open();
    if (!t) return;
    long long v[64];
    int n = 0;
    long long m = 0;
    int ok = pas_vt_consts(consts, v, &n, 64) && n > 0;
    for (int i = 0; ok && i < n; i++) { if (v[i] < 0 || v[i] > 62) ok = 0; else m |= 1LL << v[i]; }
    for (int f = lo; ok && f < hi && f < PAS_FIELD_MAX; f++) { t->fmask[f] = m; t->fhas[f] = 1; }
}
static void pas_vt_close(int tagfi) { struct pas_vt *t = pas_vt_top_open(); if (!t) return; t->tagfi = tagfi; t->state = 1; }
static void pas_vt_bind(const char *name) {
    struct pas_vt *best = NULL;
    for (int i = g_lower.pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower.pas.sem.pas_vt[i]->state == 1) {
        if (g_lower.pas.sem.pas_vt[i]->tagfi >= 0 && (!best || g_lower.pas.sem.pas_vt[i]->tagfi > best->tagfi)) best = g_lower.pas.sem.pas_vt[i];
    }
    for (int i = 0; i < g_lower.pas.sem.pas_nvt; i++) if (g_lower.pas.sem.pas_vt[i]->state == 1) g_lower.pas.sem.pas_vt[i]->state = 3;
    if (best && name) { best->state = 2; best->name = ct_strdup(name); }
}
static struct pas_vt *pas_vt_find_name(const char *nm);
static int pas_rectype_slot(const char *rn, int fi);
static tree_t *mk_assign(tree_t *sel, tree_t *rhs);
static tree_t *mk_chr_wrap(tree_t *e);
static int pas_is_charexpr(tree_t *e);
static int pas_is_charvar(const char *name);
static int pas_var_is_real(const char *name);
static int pas_is_setvar(const char *name);
static int pas_is_singlevar(const char *name);
static int pas_is_currencyexpr(tree_t *e);
static int pas_is_qword(tree_t *e);
static int pas_is_pcharvar(const char *name);
static char *pas_pchar_off_name(const char *n);
static int pas_pchar_castname(tree_t *e);
static tree_t *mk_set_bin(const char *name, tree_t *a, tree_t *b);
static void pas_tfilevar_add(const char *name);
static void pas_tfilevar_mark(void);
static void pas_tfilevar_release(void);
static int pas_is_tfilevar(const char *name);
static void pas_tfiletype_add(const char *name);
static int pas_is_tfiletype(const char *name);
static int pas_rectype_has_file(const char *rn);
static int pas_is_tfile_node(const tree_t *e);
static int pas_is_booltype(const char *name);
static void pas_boolvar_add(const char *name);
static void pas_boolvar_add_outer(const char *name);
static tree_t *pas_field_region(tree_t *base, const char *rt, int fi, tree_t *e);
static void pas_scoped_release(void);
static void pas_scoped_save(void **sv, int *svn);
static void pas_scoped_restore(void **sv, int *svn);
static void pas_charvar_add_outer(const char *name);
static int pas_is_boolvar(const char *name);
static int pas_is_boolexpr(tree_t *e);
static int pas_is_filevar(const char *name);
static int pas_is_filevar_elem(tree_t *e);
static int pas_is_stdstream(const char *name);
static int pas_is_hdrfile(const char *name);
static int pas_proc_param_is_var(const char *name, int idx);
static void pas_filevar_add(const char *name);
static int pas_is_chararr(const char *name);
static long long pas_chararr_lo(const char *name);
static int pas_is_strarr(const char *name);
static long long pas_strarr_lo(const char *name);
static tree_t *pas_alpha_wrap(tree_t *x);
static int pas_ca_is_read(const tree_t *e);
static tree_t *pas_trace_wrap_value(tree_t *val);
static const char *pas_rectype_field_enum_by_index(const char *rn, long idx);
static const char *pas_enumarr_get(const char *a);
static tree_t *pas_tree_clone(tree_t *e);
static const char *pas_scalarvartype_get(const char *vn);
static const char *pas_typealias_get(const char *n);
static int pas_var_is_boolfam(const char *name);
static const char *pas_curfunc_name(void);
static int pas_enumnames_idx(const char *tn);
static const char *pas_trace_enum_names_of_var(const char *vn) {
    const char *t = vn ? pas_scalarvartype_get(vn) : NULL;
    for (int guard = 0; t && guard < 8; guard++) {
        int ei = pas_enumnames_idx(t);
        if (ei >= 0) return pas_enumnames_by_idx(ei);
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return NULL;
}
static const char *pas_trace_store_name(const tree_t *lhs) {
    if (!lhs) return NULL;
    if (lhs->t == TT_VAR && lhs->v.sval) return lhs->v.sval;
    const char *cf = pas_curfunc_name();
    if (cf && lhs->t == TT_FNC && lhs->n >= 1 && lhs->c[0] && lhs->c[0]->t == TT_VAR && lhs->c[0]->v.sval && !strcmp(lhs->c[0]->v.sval, cf)) return cf;
    return NULL;
}
static tree_t *pas_trace_wrap_proc(const char *pname, PNodeList *params, tree_t *body, int isfunc) {
    if (!pas_trace_enabled() || !body) return body;
    tree_t *enter_call = ast_node_new(TT_FNC);
    ast_push(enter_call, leaf_s(TT_VAR, "__trace_call"));
    ast_push(enter_call, leaf_s(TT_QLIT, pname));
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *id = params->items[i];
        if (id && id->v.sval) { ast_push(enter_call, leaf_s(TT_QLIT, id->v.sval)); ast_push(enter_call, pas_trace_wrap_value(leaf_s(TT_VAR, id->v.sval))); }
    }
    tree_t *exit_call = ast_node_new(TT_FNC);
    ast_push(exit_call, leaf_s(TT_VAR, "__trace_return"));
    ast_push(exit_call, leaf_s(TT_QLIT, pname));
    if (isfunc) ast_push(exit_call, pas_trace_wrap_value(leaf_s(TT_VAR, pname)));
    tree_t *nb = ast_node_new(TT_PROGRAM);
    ast_push(nb, enter_call);
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *id = params->items[i];
        if (id && id->v.sval) {
            tree_t *bind = ast_node_new(TT_FNC);
            ast_push(bind, leaf_s(TT_VAR, "__trace_value"));
            ast_push(bind, leaf_s(TT_QLIT, id->v.sval));
            ast_push(bind, pas_trace_wrap_value(leaf_s(TT_VAR, id->v.sval)));
            ast_push(nb, bind);
        }
    }
    for (int i = 0; i < body->n; i++) ast_push(nb, body->c[i]);
    ast_push(nb, exit_call);
    return nb;
}
static tree_t *pas_trace_wrap_value(tree_t *val) {
    if (!val) return val;
    const char *_enm = (val->t == TT_IDX && val->v.ival > 0) ? pas_enumnames_by_idx((int)(val->v.ival - 1)) : NULL;
    if (!_enm && val->t == TT_VAR && val->v.sval) _enm = pas_trace_enum_names_of_var(val->v.sval);
    if (!_enm && val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[1]) {
        const char *_et = NULL, *_rt = (val->c[1]->t == TT_ILIT) ? pas_with_sel_rtype(val->c[0]) : NULL;
        if (_rt) _et = pas_rectype_field_enum_by_index(_rt, (long)val->c[1]->v.ival);
        if (!_et && val->c[0]->t == TT_VAR && val->c[0]->v.sval) _et = pas_enumarr_get(val->c[0]->v.sval);
        if (_et) _enm = pas_enumnames_by_idx(pas_enumnames_idx(_et));
    }
    if (_enm) { tree_t *_w = ast_node_new(TT_FNC); ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name")); ast_push(_w, val); ast_push(_w, leaf_s(TT_QLIT, _enm)); return _w; }
    if (pas_is_charexpr(val)) return mk_chr_wrap(val);
    if (pas_is_boolexpr(val) || (val && val->t == TT_VAR && val->v.sval && pas_var_is_boolfam(val->v.sval))) {
        tree_t *_w = ast_node_new(TT_FNC);
        ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name"));
        ast_push(_w, val->t == TT_IDX ? val : bin(TT_NE, val, ilit(0)));
        ast_push(_w, leaf_s(TT_QLIT, "false,true"));
        return _w;
    }
    if (val->t == TT_VAR && val->v.sval && pas_is_chararr(val->v.sval)) return pas_alpha_wrap(val);
    if (pas_ca_is_read(val)) return pas_alpha_wrap(val);
    if (val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[0]->t == TT_VAR && val->c[0]->v.sval && pas_is_strarr(val->c[0]->v.sval)) {
        tree_t *_w = ast_node_new(TT_FNC);
        ast_push(_w, leaf_s(TT_VAR, "__pas_alpha_str"));
        ast_push(_w, val);
        ast_push(_w, ilit(pas_strarr_lo(val->c[0]->v.sval)));
        return _w;
    }
    return val;
}
static tree_t *mk_fnc2(const char *fn, tree_t *a, tree_t *b);
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
static tree_t *pas_trace_wrap_for_body(const char *var, tree_t *body) {
    if (!pas_trace_enabled() || !var) return body;
    PNodeList *l = pnl_new();
    pnl_push(l, mk_fnc2("__trace_value", leaf_s(TT_QLIT, var), pas_trace_wrap_value(leaf_s(TT_VAR, var))));
    if (body) pnl_push(l, body);
    return seq_of(l);
}
static tree_t *pas_trace_prepend_tap_off(tree_t *body) {
    if (!pas_trace_enabled()) return body;
    tree_t *off = ast_node_new(TT_FNC);
    ast_push(off, leaf_s(TT_VAR, "__trace_tap_off"));
    tree_t *nb = ast_node_new(TT_PROGRAM);
    ast_push(nb, off);
    if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(nb, body->c[i]); } else if (body) ast_push(nb, body);
    return nb;
}
static tree_t *pas_str_to_alpha(const char *s, long long lo, long long high);
static unsigned long long pas_caparm_mask(const char *name);
static long long pas_caparm_lo(const char *name, int pos);
static void pas_caparm_add(const char *name, unsigned long long m, const long long *lo);
static int pas_is_rel(tree_t *e);
static int pas_is_proc(const char *name);
static int pas_is_func(const char *name);
static tree_t *pas_addr_of_proc(tree_t *e);
static tree_t *pas_range_wrap(tree_t *val, long long lo, long long hi, const char *clause);
static int pas_proc_param_is_real(const char *name, int idx);
static int pas_proc_param_is_string(const char *name, int idx);
static tree_t *pas_bool(tree_t *e);
static tree_t *pas_tree_clone(tree_t *e);
static tree_t *mk_deref(tree_t *ptr) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, "__pas_deref")); ast_push(e, ptr); return e; }
static tree_t *mk_fnc0(const char *fn) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); return e; }
static tree_t *mk_fnc1(const char *fn, tree_t *a) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); return e; }
static tree_t *mk_fnc2(const char *fn, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *mk_fnc3(const char *fn, tree_t *a, tree_t *b, tree_t *c) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, fn)); ast_push(e, a); ast_push(e, b); ast_push(e, c); return e; }
static int pas_ord_var_bounds(const char *name, long long *lo, long long *hi);
static tree_t *pas_ord_check(tree_t *v, long long lo, long long hi, const char *what) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_ord_check"));
    ast_push(e, v);
    ast_push(e, ilit(lo));
    ast_push(e, ilit(hi));
    ast_push(e, leaf_s(TT_QLIT, what));
    return e;
}
static int pas_is_unsigned_inttypename(const char *n) {
    if (!n) return 0;
    static const char *const U[] = {"byte","word","cardinal","longword","qword","uint8","uint16","uint32","uint64","nativeuint","pointer","ptruint","codepointer"};
    for (size_t i = 0; i < sizeof(U) / sizeof(U[0]); i++) if (!strcmp(n, U[i])) return 1;
    return 0;
}
static int pas_with_holds_pointee_of(tree_t *ptr);
static tree_t *mk_call(const char *name, PNodeList *args) {
    if (name && !strcmp(name, "ord") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        if (a && a->t == TT_FNC && a->n >= 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) return a->c[1];
        if (a && a->t == TT_IDX) a->v.ival = 0;
        if (pas_is_charexpr(a) || pas_is_boolexpr(a)) return bin(TT_ADD, a, ilit(0));
        return a;
    }
    if (name && (!strcmp(name, "pack") || !strcmp(name, "unpack")) && args && args->count >= 5) {
        int ispack = !strcmp(name, "pack");
        tree_t *a = args->items[ispack ? 0 : 2];
        tree_t *i = args->items[ispack ? 2 : 4];
        tree_t *z = args->items[ispack ? 4 : 0];
        long long zhi = -1;
        long long zlo = 1;
        if (z && z->t == TT_VAR && z->v.sval) {
            if (!pas_array_high_get(z->v.sval, &zhi)) zhi = -1;
            if (pas_is_chararr(z->v.sval)) { long long _cl = pas_chararr_lo(z->v.sval); if (_cl > 0) zlo = _cl; } else { zlo = pas_array_low(z->v.sval); }
        }
        if (a && a->t == TT_VAR && a->v.sval && z && z->t == TT_VAR && z->v.sval && zhi >= 0) {
            long long ahi = -1;
            int _ahok = pas_array_high_get(a->v.sval, &ahi);
            long long alo = pas_array_low(a->v.sval);
            long long cnt = zhi - zlo + 1;
            if (i && (pas_is_charexpr(i) || (i->t == TT_FNC && i->n >= 2 && i->c[0] && i->c[0]->v.sval && !strcmp(i->c[0]->v.sval, "__pas_chrlit")))) {
                fprintf(stderr, "pascal: ISO 7185 6.6.5.4 violation: the ordinal parameter of %s is not assignment-compatible with the index-type of the unpacked array '%s'\n", name, a->v.sval);
                g_lower.pas.sem.pas_iso_errors++;
            } else if (_ahok && cnt > 0 && i && i->t == TT_ILIT) {
                long long st = i->v.ival;
                if (st < alo || st + cnt - 1 > ahi) {
                    fprintf(stderr, "pascal: ISO 7185 6.6.5.4 violation: %s over the unpacked array '%s' declared [%lld..%lld] reads components [%lld..%lld], which is outside its index-type\n", name,
                        a->v.sval, alo, ahi, st, st + cnt - 1);
                    g_lower.pas.sem.pas_iso_errors++;
                }
            }
        }
        {
            tree_t *src = ispack ? a : z;
            if (src && src->t == TT_VAR && src->v.sval && !pas_array_is_param(src->v.sval) && !pas_assigned_get(src->v.sval) && !pas_array_is_zero_size(src->v.sval)) {
                fprintf(stderr, "pascal: ISO 7185 6.6.5.4 violation: %s reads the array '%s', whose components have never been assigned a value\n", name, src->v.sval);
                g_lower.pas.sem.pas_iso_errors++;
            }
        }
        char _cvb[24];
        snprintf(_cvb, sizeof _cvb, "__pas_pk%d", g_lower.pas.sem.pkn++);
        const char *_cv = ct_strdup(_cvb);
        tree_t *zi = ast_node_new(TT_IDX);
        ast_push(zi, pas_tree_clone(z));
        ast_push(zi, leaf_s(TT_VAR, _cv));
        tree_t *ai = ast_node_new(TT_IDX);
        ast_push(ai, pas_tree_clone(a));
        ast_push(ai, bin(TT_ADD, pas_tree_clone(i), bin(TT_SUB, leaf_s(TT_VAR, _cv), ilit(zlo))));
        tree_t *body = ispack ? mk_assign(zi, ai) : mk_assign(ai, zi);
        tree_t *f = ast_node_new(TT_FOR);
        ast_push(f, leaf_s(TT_VAR, _cv));
        ast_push(f, ilit(zlo));
        ast_push(f, ilit(zhi));
        ast_push(f, body);
        return f;
    }
    if (name && (!strcmp(name, "get") || !strcmp(name, "put")) && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval &&
        pas_is_filevar(args->items[0]->v.sval) && !pas_is_stdstream(args->items[0]->v.sval)) {
        return mk_fnc1(!strcmp(name, "get") ? "__pas_tget" : "__pas_tput", args->items[0]);
    }
    if (name && args && args->count >= 1 && pas_is_tfile_node(args->items[0])) {
        tree_t *fa = args->items[0];
        if (!strcmp(name, "readln") || !strcmp(name, "writeln")) {
            fprintf(stderr, "pascal: ISO 7185 %s violation: the file parameter of '%s' shall be a textfile, but '%s' is a file of a non-text component-type\n",
                !strcmp(name, "readln") ? "6.9.2" : "6.9.4", name, fa->v.sval ? fa->v.sval : "<file>");
            g_lower.pas.sem.pas_iso_errors++;
        }
        if (!strcmp(name, "get")) return mk_fnc1("__pas_fget", fa);
        if (!strcmp(name, "put")) return mk_fnc1("__pas_fput", fa);
        if (!strcmp(name, "eof")) return mk_fnc1("__pas_feof_t", fa);
        if (!strcmp(name, "reset")) return pas_is_hdrfile(fa->v.sval) ? mk_assign(fa, mk_set_bin("__pas_treset", pas_tree_clone(fa), leaf_s(TT_QLIT, fa->v.sval))) :
            mk_assign(fa, mk_fnc1("__pas_treset", pas_tree_clone(fa)));
        if (!strcmp(name, "rewrite")) return pas_is_hdrfile(fa->v.sval) ? mk_assign(fa, mk_set_bin("__pas_trewrite", pas_tree_clone(fa), leaf_s(TT_QLIT, fa->v.sval))) :
            mk_assign(fa, mk_fnc1("__pas_trewrite", pas_tree_clone(fa)));
        if (!strcmp(name, "read") || !strcmp(name, "write")) {
            PNodeList *stmts = pnl_new();
            for (int i = 2; i + 1 < args->count; i += 2) {
                tree_t *v = args->items[i];
                long long _rlo, _rhi;
                if (!strcmp(name, "read") && v && v->t == TT_VAR && v->v.sval && pas_subvar_get(v->v.sval, &_rlo, &_rhi)) {
                    tree_t *rc = ast_node_new(TT_FNC);
                    ast_push(rc, leaf_s(TT_VAR, "__pas_fread_range"));
                    ast_push(rc, pas_tree_clone(fa));
                    ast_push(rc, ilit(_rlo));
                    ast_push(rc, ilit(_rhi));
                    pnl_push(stmts, mk_assign(v, rc));
                } else if (!strcmp(name, "write") && fa && fa->t == TT_VAR && fa->v.sval && pas_tfcomp_range(fa->v.sval, &_rlo, &_rhi)) {
                    tree_t *wc = ast_node_new(TT_FNC);
                    ast_push(wc, leaf_s(TT_VAR, "__pas_fwrite_range"));
                    ast_push(wc, pas_tree_clone(fa));
                    ast_push(wc, v);
                    ast_push(wc, ilit(_rlo));
                    ast_push(wc, ilit(_rhi));
                    pnl_push(stmts, wc);
                } else if (!strcmp(name, "read")) pnl_push(stmts, mk_assign(v, mk_fnc1("__pas_fread", pas_tree_clone(fa))));
                else pnl_push(stmts, mk_set_bin("__pas_fwrite", pas_tree_clone(fa), v));
            }
            return seq_of(stmts);
        }
    }
    if (name && !strcmp(name, "chr") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        if (a && a->t != TT_ILIT) a = pas_ord_check(a, 0, 255, "chr(x) of a value that is the ordinal number of no char-type value");
        return mk_fnc1("__pas_chrlit", a);
    }
    if (name && (!strcmp(name, "pred") || !strcmp(name, "succ")) && args && args->count >= 1) {
        int up = !strcmp(name, "succ");
        long long lo, hi;
        tree_t *a = args->items[0];
        tree_t *v = bin(up ? TT_ADD : TT_SUB, a, ilit(1));
        if (a && a->t == TT_VAR && pas_ord_var_bounds(a->v.sval, &lo, &hi)) v = pas_ord_check(v, lo, hi, up ? "succ(x) of the largest value of its type" : "pred(x) of the smallest value of its type");
        if (a && ((a->t == TT_FNC && a->n >= 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) || pas_is_charexpr(a))) return mk_fnc1("__pas_chrlit", v);
        if (a && pas_is_boolexpr(a)) return bin(TT_NE, v, ilit(0));
        return v;
    }
    if (name && (!strcmp(name, "inc") || !strcmp(name, "dec")) && args && args->count >= 1 &&
        !(args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval && pas_is_pcharvar(args->items[0]->v.sval))) {
        tree_t *v = args->items[0];
        tree_t *delta = (args->count >= 3) ? args->items[2] : ilit(1);
        tree_e op = !strcmp(name, "inc") ? TT_ADD : TT_SUB;
        return mk_assign(v, bin(op, pas_tree_clone(v), delta));
    }
    if (name && !strcmp(name, "setlength") && args && args->count >= 4) {
        tree_t *v = args->items[0];
        tree_t *n = args->items[2];
        return mk_assign(v, mk_fnc2("__pas_setlength", pas_tree_clone(v), n));
    }
    if (name && (!strcmp(name, "low") || !strcmp(name, "high")) && args && args->count >= 1) {
        tree_t *v = args->items[0];
        long long hi;
        if (v && v->t == TT_VAR && v->v.sval && pas_array_high_get(v->v.sval, &hi)) { return ilit(!strcmp(name, "low") ? pas_array_nonparam_low(v->v.sval) : hi); }
        if (v && v->t == TT_VAR && v->v.sval) { long long lo2, hi2; if (pas_ordinal_bound_lookup(v->v.sval, &lo2, &hi2)) return ilit(!strcmp(name, "low") ? lo2 : hi2); }
        if (v && v->t == TT_VAR && v->v.sval) {
            int sk = pas_var_string_kind(v->v.sval);
            if (sk) {
                int _zb = sk == 1 && (g_lower.pas.sem.pas_zerobased_strings & 1);
                return !strcmp(name, "low") ? ilit(sk == 2 || _zb ? 0 : 1) :
                    (sk == 2 ? ilit(255) : (_zb ? bin(TT_SUB, mk_fnc1("length", pas_tree_clone(v)), ilit(1)) : mk_fnc1("length", pas_tree_clone(v))));
            }
        }
    }
    if (name && (!strcmp(name, "inc") || !strcmp(name, "dec")) && args && args->count >= 1 && args->items[0] && args->items[0]->t == TT_VAR && args->items[0]->v.sval &&
        pas_is_pcharvar(args->items[0]->v.sval)) {
        tree_t *h = leaf_s(TT_VAR, pas_pchar_off_name(args->items[0]->v.sval));
        tree_t *n = (args->count >= 3) ? args->items[2] : ilit(1);
        return bin(TT_ASSIGN, h, bin(name[0] == 'i' ? TT_ADD : TT_SUB, pas_tree_clone(h), n));
    }
    if (name && !strcmp(name, "stringtowidechar") && args && args->count >= 6) {
        tree_t *x = args->items[2];
        if (x && x->t == TT_IDX && x->n == 2 && x->c[0] && x->c[0]->t == TT_VAR) {
            tree_t *f = ast_node_new(TT_FNC);
            ast_push(f, leaf_s(TT_VAR, "__pas_s2wide"));
            ast_push(f, args->items[0]);
            ast_push(f, x->c[0]);
            ast_push(f, x->c[1]);
            ast_push(f, args->items[4]);
            return f;
        }
    }
    if (name && (!strcmp(name, "widestring") || !strcmp(name, "ansistring") || !strcmp(name, "unicodestring")) && args && args->count >= 1 && pas_pchar_castname(args->items[0]) &&
        args->items[0]->c[1] && args->items[0]->c[1]->t == TT_IDX && args->items[0]->c[1]->n == 2 && args->items[0]->c[1]->c[0] && args->items[0]->c[1]->c[0]->t == TT_VAR &&
        !pas_var_string_kind(args->items[0]->c[1]->c[0]->v.sval)) {
        tree_t *x = args->items[0]->c[1];
        tree_t *f = ast_node_new(TT_FNC);
        ast_push(f, leaf_s(TT_VAR, "__pas_wide2s"));
        ast_push(f, x->c[0]);
        ast_push(f, x->c[1]);
        return f;
    }
    if (name && !strcmp(name, "sizeof") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        if (a && a->t == TT_VAR && a->v.sval) { long long sz; if (pas_sizeof_lookup(a->v.sval, &sz)) return ilit(sz); }
    }
    if (name && !strcmp(name, "stringcodepage") && args && args->count >= 1) return ilit(g_lower.pas.sem.pas_codepage);
    if (name && !strcmp(name, "ismanagedtype") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        const char *tn = (a && a->t == TT_VAR && a->v.sval) ? pas_scalarvartype_get(a->v.sval) : NULL;
        { int _guard = 0; while (tn) { const char *_al = pas_typealias_get(tn); if (!_al || !strcmp(_al, tn) || _guard++ >= 8) break; tn = _al; } }
        int managed = tn && (!strcmp(tn, "ansistring") || !strcmp(tn, "unicodestring") || !strcmp(tn, "widestring") || !strcmp(tn, "variant"));
        return ilit(managed ? 1 : 0);
    }
    if (name && !strcmp(name, "boolean") && args && args->count >= 1) return bin(TT_NE, mk_fnc2("iand", args->items[0], ilit(255)), ilit(0));
    if (name && strcmp(name, "char") && strcmp(name, "widechar") && strcmp(name, "boolean") && args && args->count >= 1) {
        long long _tsz;
        int _isboolfam = !strcmp(name, "bytebool") || !strcmp(name, "wordbool") || !strcmp(name, "longbool") || !strcmp(name, "qwordbool");
        const char *_tcn = name;
        { const char *_al = pas_typealias_get(_tcn); int _guard = 0; while (_al && strcmp(_al, _tcn) && _guard++ < 8) { _tcn = _al; _al = pas_typealias_get(_tcn); } }
        if ((!_isboolfam || pas_is_boolexpr(args->items[0])) && pas_sizeof_builtin_size(_tcn, &_tsz)) {
            if (_isboolfam || (_tsz != 1 && _tsz != 2 && _tsz != 4)) {
                tree_t *_a0 = args->items[0];
                return (_a0 && _a0->t == TT_VAR && _a0->v.sval && pas_var_is_boolfam(_a0->v.sval)) ? bin(TT_ADD, _a0, ilit(0)) : _a0;
            }
            long long _full = 1LL << (_tsz * 8);
            tree_t *_m = mk_fnc2("iand", args->items[0], ilit(_full - 1));
            if (pas_is_unsigned_inttypename(_tcn)) return _m;
            long long _half = _full / 2;
            return bin(TT_SUB, bin(TT_MOD, bin(TT_ADD, _m, ilit(_half)), ilit(_full)), ilit(_half));
        }
    }
    if (name && !strcmp(name, "swapendian") && args && args->count >= 1) {
        tree_t *a = args->items[0];
        long long sz = 4;
        if (a && a->t == TT_VAR && a->v.sval) pas_sizeof_lookup(a->v.sval, &sz);
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_swapendian"));
        ast_push(e, a);
        ast_push(e, ilit(sz));
        return e;
    }
    if (name && !strcmp(name, "addr") && args && args->count >= 1) return pas_addr_of_proc(args->items[0]);
    if (name && !strcmp(name, "fillchar") && args && args->count >= 5) {
        tree_t *dst = args->items[0];
        tree_t *val = args->items[4];
        long long fhi = -1, flo = 0;
        tree_t *fdst = NULL;
        if (dst && dst->t == TT_VAR && dst->v.sval && pas_array_high_get(dst->v.sval, &fhi)) {
            flo = pas_array_low(dst->v.sval);
            fdst = leaf_s(TT_VAR, dst->v.sval);
        } else if (dst && dst->t == TT_FNC && dst->n >= 2 && dst->c[0] && dst->c[0]->v.sval && !strcmp(dst->c[0]->v.sval, "__pas_deref")) {
            const char *ptn = pas_ptrexpr_target(dst->c[1]);
            long long phi = ptn ? pas_arrtype_high(ptn) : -1;
            if (phi >= 0) { fhi = phi; flo = pas_arrtype_lo(ptn); fdst = mk_deref(pas_tree_clone(dst->c[1])); }
        }
        if (fdst) {
            char _fcb[24];
            snprintf(_fcb, sizeof _fcb, "__pas_fc%d", g_lower.pas.sem.fcn++);
            const char *_fcv = ct_strdup(_fcb);
            tree_t *fidx = ast_node_new(TT_IDX);
            ast_push(fidx, fdst);
            ast_push(fidx, leaf_s(TT_VAR, _fcv));
            tree_t *fbody = mk_assign(fidx, val);
            tree_t *floop = ast_node_new(TT_FOR);
            ast_push(floop, leaf_s(TT_VAR, _fcv));
            ast_push(floop, ilit(flo));
            ast_push(floop, ilit(fhi));
            ast_push(floop, fbody);
            return floop;
        }
    }
    if (name && !strcmp(name, "trunc") && args && args->count >= 1) return mk_fnc2("__pas_trunc", args->items[0], ilit(1));
    if (name && !strcmp(name, "int") && args && args->count >= 1) return mk_fnc1("__pas_trunc", args->items[0]);
    if (name && !strcmp(name, "round") && args && args->count >= 1) return mk_fnc1("__pas_round", args->items[0]);
    if (name && !strcmp(name, "halt") && (!args || args->count == 0)) return mk_fnc0("__pas_halt");
    if (name && !strcmp(name, "halt") && args && args->count >= 1) return mk_fnc1("__pas_halt", args->items[0]);
    if (name && !strcmp(name, "frac") && args && args->count >= 1) return mk_fnc1("__pas_frac", args->items[0]);
    if (name && !strcmp(name, "assert") && args && args->count >= 1) return mk_fnc1("__pas_assert", args->items[0]);
    if (name && !strcmp(name, "mkdir") && args && args->count >= 1) return mk_fnc1("__pas_mkdir", args->items[0]);
    if (name && !strcmp(name, "rmdir") && args && args->count >= 1) return mk_fnc1("__pas_rmdir", args->items[0]);
    if (name && !strcmp(name, "pos") && args && args->count >= 3) {
        tree_t *needle = args->items[0];
        if (pas_is_charexpr(needle)) needle = mk_chr_wrap(needle);
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_pos"));
        ast_push(e, needle);
        ast_push(e, args->items[2]);
        return e;
    }
    if (name && !strcmp(name, "abs") && args && args->count >= 1) return mk_fnc1("__pas_abs", args->items[0]);
    if (name && !strcmp(name, "sin") && args && args->count >= 1) return mk_fnc1("__pas_sin", args->items[0]);
    if (name && !strcmp(name, "cos") && args && args->count >= 1) return mk_fnc1("__pas_cos", args->items[0]);
    if (name && !strcmp(name, "exp") && args && args->count >= 1) return mk_fnc1("__pas_exp", args->items[0]);
    if (name && !strcmp(name, "sqrt") && args && args->count >= 1) return mk_fnc1("__pas_sqrt", args->items[0]);
    if (name && !strcmp(name, "ln") && args && args->count >= 1) return mk_fnc1("__pas_ln", args->items[0]);
    if (name && !strcmp(name, "arctan") && args && args->count >= 1) return mk_fnc1("__pas_arctan", args->items[0]);
    if (name && !strcmp(name, "odd") && args && args->count >= 1) return bin(TT_NE, bin(TT_MOD, args->items[0], ilit(2)), ilit(0));
    if (name && !strcmp(name, "eof") && (!args || args->count == 0)) return mk_fnc0("__pas_eof");
    if (name && !strcmp(name, "eoln") && (!args || args->count == 0)) return mk_fnc0("__pas_eoln");
    if (name && (!strcmp(name, "eof") || !strcmp(name, "eoln")) && args && args->count >= 1) {
        tree_t *fa = args->items[0];
        int isf = (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) || pas_is_filevar_elem(fa);
        int isstd = fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval);
        const char *base = !strcmp(name, "eof") ? "__pas_eof" : "__pas_eoln";
        if (isf && !isstd) { const char *fn2 = !strcmp(name, "eof") ? "__pas_eof_f" : "__pas_eoln_f"; return mk_fnc1(fn2, pas_tree_clone(fa)); }
        return mk_fnc0(base);
    }
    if (name && (!strcmp(name, "GetBufCh") || !strcmp(name, "getbufch")) && args && args->count >= 1) {
        tree_t *fa = args->items[0];
        int isstd = fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval);
        if (fa && fa->t == TT_VAR && fa->v.sval && !isstd) return mk_fnc1("__pas_getbufch_f", pas_tree_clone(fa));
        return mk_fnc0("__pas_getbufch");
    }
    if (name && !strcmp(name, "delete") && args && args->count >= 6) {
        tree_t *sv = args->items[0];
        tree_t *pos = args->items[2];
        tree_t *cnt = args->items[4];
        return mk_assign(sv, mk_fnc3("__pas_str_delete", pas_tree_clone(sv), pos, cnt));
    }
    if (name && !strcmp(name, "insert") && args && args->count >= 6) {
        tree_t *src = args->items[0];
        tree_t *sv = args->items[2];
        tree_t *pos = args->items[4];
        if (src && src->t == TT_FNC && src->n == 2 && src->c[0] && src->c[0]->v.sval && !strcmp(src->c[0]->v.sval, "__pas_chrlit") && src->c[1] && src->c[1]->t == TT_ILIT) {
            char _cb[2];
            _cb[0] = (char)src->c[1]->v.ival;
            _cb[1] = '\0';
            src = leaf_s(TT_QLIT, ct_strdup(_cb));
        }
        return mk_assign(sv, mk_fnc3("__pas_str_insert", src, pas_tree_clone(sv), pos));
    }
    if (name && !strcmp(name, "readstr") && args && args->count >= 4) {
        tree_t *sv = args->items[0];
        tree_t *v = args->items[2];
        tree_t *rhs = mk_fnc1("__pas_str_to_int", sv);
        if (g_lower.pas.sem.pas_range_check_on && v && v->t == TT_VAR && v->v.sval) { long long _lo, _hi; if (pas_subvar_get(v->v.sval, &_lo, &_hi)) rhs = pas_range_wrap(rhs, _lo, _hi, NULL); }
        return mk_assign(v, rhs);
    }
    if (name && !strcmp(name, "readln") && (!args || args->count == 0)) return mk_fnc0("__pas_readln");
    if (name && (!strcmp(name, "readln") || !strcmp(name, "read")) && args && args->count >= 1) {
        int isln = !strcmp(name, "readln");
        int start = 0;
        tree_t *fstream = NULL;
        tree_t *fa = args->items[0];
        if (pas_is_filevar_elem(fa)) {
            fstream = fa;
            start = 2;
        } else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) {
            if (!pas_is_stdstream(fa->v.sval)) fstream = fa;
            start = 2;
        }
        PNodeList *stmts = pnl_new();
        for (int i = start; i + 1 < args->count; i += 2) {
            tree_t *v = pas_vt_unwrap_read(args->items[i]);
            if (v && v->t == TT_VAR && v->v.sval && pas_is_chararr(v->v.sval)) {
                long long rlo = pas_array_low(v->v.sval);
                long long rhi = -1;
                pas_array_high_get(v->v.sval, &rhi);
                char _rdcb[24];
                snprintf(_rdcb, sizeof _rdcb, "__pas_rdi%d", g_lower.pas.sem.rdcn++);
                const char *_rdcv = ct_strdup(_rdcb);
                tree_t *ridx = ast_node_new(TT_IDX);
                ast_push(ridx, leaf_s(TT_VAR, v->v.sval));
                ast_push(ridx, leaf_s(TT_VAR, _rdcv));
                tree_t *rval = fstream ? mk_fnc1("__pas_read_c_f", pas_tree_clone(fstream)) : mk_fnc0("__pas_read_c");
                tree_t *rloop = ast_node_new(TT_FOR);
                ast_push(rloop, leaf_s(TT_VAR, _rdcv));
                ast_push(rloop, ilit(rlo));
                ast_push(rloop, ilit(rhi));
                ast_push(rloop, mk_assign(ridx, rval));
                pnl_push(stmts, rloop);
                continue;
            }
            int isc = v && ((v->t == TT_VAR && v->v.sval && pas_is_charvar(v->v.sval)) || pas_is_charexpr(v));
            int isr = !isc && v && v->t == TT_VAR && v->v.sval && pas_var_is_real(v->v.sval);
            tree_t *rv = isr ? (fstream ? mk_set_bin("__pas_read_i_f", pas_tree_clone(fstream), ilit(1)) : mk_fnc1("__pas_read_i", ilit(1))) : fstream ?
                mk_fnc1(isc ? "__pas_read_c_f" : "__pas_read_i_f", pas_tree_clone(fstream)) : mk_fnc0(isc ? "__pas_read_c" : "__pas_read_i");
            {
                long long _lo, _hi;
                if (!isr && g_lower.pas.sem.pas_range_check_on && v && v->t == TT_VAR && v->v.sval && !pas_is_setvar(v->v.sval) && pas_subvar_get(v->v.sval, &_lo, &_hi)) rv =
                    pas_range_wrap(rv, _lo, _hi, "6.9.1");
            }
            pnl_push(stmts, mk_assign(v, rv));
        }
        if (isln) { if (fstream) pnl_push(stmts, mk_fnc1("__pas_readln_f", pas_tree_clone(fstream))); else pnl_push(stmts, mk_fnc0("__pas_readln")); }
        return seq_of(stmts);
    }
    if (name && (!strcmp(name, "ReadInt") || !strcmp(name, "readint")) && args && args->count >= 3) {
        tree_t *fa = args->items[0];
        tree_t *fstream = NULL;
        int start = 0;
        if (pas_is_filevar_elem(fa)) {
            fstream = fa;
            start = 2;
        } else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval)) {
            if (!pas_is_stdstream(fa->v.sval)) fstream = fa;
            start = 2;
        }
        PNodeList *stmts = pnl_new();
        for (int i = start; i + 1 < args->count; i += 2) {
            tree_t *v = args->items[i];
            if (fstream) pnl_push(stmts, mk_assign(v, mk_fnc1("__pas_read_i_f", pas_tree_clone(fstream))));
            else pnl_push(stmts, mk_assign(v, mk_fnc0("__pas_read_i")));
        }
        return seq_of(stmts);
    }
    if (name && !strcmp(name, "assign") && args && args->count >= 3) { tree_t *fv = args->items[0]; tree_t *nm = args->items[2]; return mk_assign(fv, mk_fnc1("__pas_fassign", pas_alpha_wrap(nm))); }
    if (name && !strcmp(name, "rewrite") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        tree_t *dflt = (fv && fv->t == TT_VAR && fv->v.sval) ? ((g_lower.pas.sem.pas_mode_iso && pas_is_hdrfile(fv->v.sval)) ? ilit(1) : leaf_s(TT_QLIT, fv->v.sval)) : NULL;
        return mk_assign(fv, dflt ? mk_set_bin("__pas_rewrite", pas_tree_clone(fv), dflt) : mk_fnc1("__pas_rewrite", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "append") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        return mk_assign(fv, (fv && fv->t == TT_VAR && fv->v.sval) ? mk_set_bin("__pas_append", pas_tree_clone(fv), leaf_s(TT_QLIT, fv->v.sval)) : mk_fnc1("__pas_append", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "reset") && args && args->count >= 1) {
        tree_t *fv = args->items[0];
        tree_t *dflt = (fv && fv->t == TT_VAR && fv->v.sval) ? ((g_lower.pas.sem.pas_mode_iso && pas_is_hdrfile(fv->v.sval)) ? ilit(0) : leaf_s(TT_QLIT, fv->v.sval)) : NULL;
        return mk_assign(fv, dflt ? mk_set_bin("__pas_reset", pas_tree_clone(fv), dflt) : mk_fnc1("__pas_reset", pas_tree_clone(fv)));
    }
    if (name && !strcmp(name, "close") && args && args->count >= 1) { return mk_fnc1("__pas_fclose", args->items[0]); }
    if (name && !strcmp(name, "erase") && args && args->count >= 1) { return mk_fnc1("__pas_ferase", args->items[0]); }
    if (name && !strcmp(name, "new") && args && args->count >= 1) {
        tree_t *pv0 = args->items[0], *pv = pas_vt_unwrap_read(pv0);
        tree_t *pvchk = (pv != pv0 && pv0 && pv0->t == TT_SEQ_EXPR && pv0->n >= 2) ? pv0->c[0] : NULL;
        const char *rt = pas_ptrexpr_target(pv);
        tree_t *alloc = ast_node_new(TT_FNC);
        if (rt) { ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc_rec")); ast_push(alloc, pas_new_target_init(rt)); } else ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc"));
        tree_t *as = mk_assign(pv, alloc);
        if (pvchk) { tree_t *qc = ast_node_new(TT_SEQ_EXPR); ast_push(qc, pvchk); ast_push(qc, as); as = qc; }
        if (args->count >= 4 && rt) {
            struct pas_vt *vt = pas_vt_find_name(rt);
            if (vt && vt->tagfi >= 0) {
                tree_t *tg = ast_node_new(TT_IDX);
                ast_push(tg, mk_deref(pas_tree_clone(pv)));
                ast_push(tg, ilit(pas_rectype_slot(rt, vt->tagfi)));
                tree_t *q = ast_node_new(TT_SEQ_EXPR);
                ast_push(q, as);
                ast_push(q, mk_assign(tg, args->items[2]));
                ast_push(q, mk_fnc1("__pas_tagmark", pas_tree_clone(pv)));
                return q;
            }
        }
        return as;
    }
    if (name && !strcmp(name, "mark") && args && args->count >= 1) { tree_t *mk = ast_node_new(TT_FNC); ast_push(mk, leaf_s(TT_VAR, "__pas_mark")); return mk_assign(args->items[0], mk); }
    if (name && !strcmp(name, "release") && args && args->count >= 1) { return mk_fnc1("__pas_release", args->items[0]); }
    if (name && !strcmp(name, "dispose") && args && args->count >= 1) {
        if (pas_with_holds_pointee_of(args->items[0])) {
            fprintf(stderr, "pascal: ISO 7185 6.5.4 violation: dispose removes the identifying-value of a variable while a with-statement still references that variable\n");
            g_lower.pas.sem.pas_iso_errors++;
        }
        if (args->count >= 4) {
            const char *drt = pas_ptrexpr_target(args->items[0]);
            struct pas_vt *vt = drt ? pas_vt_find_name(drt) : NULL;
            tree_t *tn = ilit(-1);
            if (vt && vt->tagfi >= 0) { tn = ast_node_new(TT_IDX); ast_push(tn, mk_deref(pas_tree_clone(args->items[0]))); ast_push(tn, ilit(pas_rectype_slot(drt, vt->tagfi))); }
            tree_t *dk = ast_node_new(TT_FNC);
            ast_push(dk, leaf_s(TT_VAR, "__pas_dispose_k"));
            ast_push(dk, args->items[0]);
            ast_push(dk, args->items[2]);
            ast_push(dk, tn);
            return dk;
        }
        return mk_fnc1("__pas_dispose", args->items[0]);
    }
    tree_t *e = ast_node_new(TT_FNC);
    int _wstart = 0;
    tree_t *_wstream = NULL;
    if (is_pas_io(map_io(name)) && args && args->count >= 2) {
        tree_t *fa = args->items[0];
        if (pas_is_filevar_elem(fa) || (fa && fa->t == TT_VAR && fa->v.sval && pas_is_filevar(fa->v.sval) && !pas_is_stdstream(fa->v.sval))) {
            _wstream = fa;
            _wstart = 2;
        } else if (fa && fa->t == TT_VAR && fa->v.sval && pas_is_stdstream(fa->v.sval)) {
            _wstart = 2;
        }
    }
    ast_push(e, leaf_s(TT_VAR, map_io(name)));
    if (_wstream) { ast_push(e, pas_tree_clone(_wstream)); ast_push(e, ilit(-9)); }
    if (args) {
        if (is_pas_io(map_io(name))) {
            for (int i = _wstart; i + 1 < args->count; i += 2) {
                tree_t *val = args->items[i];
                tree_t *wid = args->items[i + 1];
                int is_char = pas_is_charexpr(val);
                const char *_enm = (val && val->t == TT_IDX && val->v.ival > 0) ? pas_enumnames_by_idx((int)(val->v.ival - 1)) : NULL;
                if (_enm) {
                    tree_t *_w = ast_node_new(TT_FNC);
                    ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name"));
                    ast_push(_w, val);
                    ast_push(_w, leaf_s(TT_QLIT, _enm));
                    val = _w;
                } else if (is_char) {
                    val = mk_chr_wrap(val);
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-2);
                } else if (pas_is_boolexpr(val) || (val && val->t == TT_VAR && val->v.sval && pas_var_is_boolfam(val->v.sval))) {
                    tree_t *_w = ast_node_new(TT_FNC);
                    ast_push(_w, leaf_s(TT_VAR, "__pas_enum_name"));
                    ast_push(_w, val->t == TT_IDX ? val : bin(TT_NE, val, ilit(0)));
                    ast_push(_w, leaf_s(TT_QLIT, "false,true"));
                    val = _w;
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(5);
                } else if (val && val->t == TT_VAR && val->v.sval && pas_is_singlevar(val->v.sval)) {
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-5);
                } else if (pas_is_currencyexpr(val)) {
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-6);
                } else if (pas_is_qword(val) && val->t == TT_VAR) {
                    if (wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-7);
                } else if (val && val->t == TT_VAR && val->v.sval && pas_is_chararr(val->v.sval)) {
                    val = pas_alpha_wrap(val);
                } else if (pas_ca_is_read(val)) {
                    val = pas_alpha_wrap(val);
                } else if (val && val->t == TT_IDX && val->n >= 2 && val->c[0] && val->c[0]->t == TT_VAR && val->c[0]->v.sval && pas_is_strarr(val->c[0]->v.sval)) {
                    tree_t *_w = ast_node_new(TT_FNC);
                    ast_push(_w, leaf_s(TT_VAR, "__pas_alpha_str"));
                    ast_push(_w, val);
                    ast_push(_w, ilit(pas_strarr_lo(val->c[0]->v.sval)));
                    val = _w;
                }
                if (g_lower.pas.sem.pas_seen_mode_directive && wid->t == TT_ILIT && wid->v.ival == -1) wid = ilit(-4);
                ast_push(e, val);
                ast_push(e, wid);
            }
        } else {
            unsigned long long _cam = pas_caparm_mask(name);
            tree_t *_vpost[args->count / 2 + 1];
            int _nvpost = 0;
            for (int i = 0; i < args->count; i += 2) {
                tree_t *val = args->items[i];
                int _pidx = i / 2;
                if (val && val->t == TT_SEQ_EXPR && val->n == 2 && val->c[0] && val->c[0]->t == TT_FNC && val->c[0]->c[0] && val->c[0]->c[0]->v.sval &&
                    !strcmp(val->c[0]->c[0]->v.sval, "__pas_tagwhole") && pas_proc_param_is_var(name, _pidx) && val->c[1] && val->c[1]->t == TT_FNC && val->c[1]->n == 2) {
                    tree_t *q = ast_node_new(TT_SEQ_EXPR);
                    ast_push(q, val->c[0]);
                    ast_push(q, val->c[1]->c[1]);
                    val->c[1]->c[1] = q;
                    val = val->c[1];
                }
                if (val && val->t == TT_SEQ_EXPR && val->n == 2 && val->c[0] && val->c[0]->t == TT_FNC && val->c[0]->n >= 1 && val->c[0]->c[0] && val->c[0]->c[0]->v.sval &&
                    !strcmp(val->c[0]->c[0]->v.sval, "__pas_vcheck") && pas_proc_param_is_var(name, _pidx)) {
                    if (pas_is_proc(name)) _vpost[_nvpost++] = pas_tree_clone(val->c[0]);
                    val = val->c[1];
                }
                if (val && val->t == TT_VAR && val->v.sval && pas_is_stdstream(val->v.sval) && !pas_ptrvar_target(val->v.sval) && pas_proc_param_is_var(name, _pidx)) val =
                    mk_fnc1("__pas_stdfile", ilit(!strcmp(val->v.sval, "input") ? 0 : 1));
                else if (val && (pas_is_proc(name) || pas_is_func(name)) && !pas_proc_param_is_var(name, _pidx) && pas_is_aggregate_designator(val)) val = mk_fnc1("__pas_arr_copy", val);
                if (val && val->t == TT_QLIT && val->v.sval && _pidx < 64 && ((_cam >> _pidx) & 1ULL)) {
                    long long _lo = pas_caparm_lo(name, _pidx);
                    val = pas_str_to_alpha(val->v.sval, _lo, _lo + (long long)strlen(val->v.sval) - 1);
                } else if (pas_ca_is_read(val) && _pidx < 64 && ((_cam >> _pidx) & 1ULL)) {
                    tree_t *_en = ast_node_new(TT_FNC);
                    ast_push(_en, leaf_s(TT_VAR, "__pas_ca_encode"));
                    ast_push(_en, val);
                    ast_push(_en, ilit(pas_caparm_lo(name, _pidx)));
                    ast_push(_en, ilit(-1));
                    val = _en;
                }
                if (val && val->t != TT_FLIT && pas_proc_param_is_real(name, _pidx)) val = bin(TT_ADD, val, flit(0.0));
                if (val && val->t == TT_FNC && val->n == 2 && val->c[0] && val->c[0]->v.sval && !strcmp(val->c[0]->v.sval, "__pas_chrlit") && val->c[1] && val->c[1]->t == TT_ILIT &&
                    pas_proc_param_is_string(name, _pidx)) {
                    char _cb[2];
                    _cb[0] = (char)val->c[1]->v.ival;
                    _cb[1] = '\0';
                    val = leaf_s(TT_QLIT, ct_strdup(_cb));
                }
                ast_push(e, val);
            }
            if (_nvpost) { tree_t *q = ast_node_new(TT_SEQ_EXPR); ast_push(q, e); for (int k = 0; k < _nvpost; k++) ast_push(q, _vpost[k]); e = q; }
        }
    }
    return e;
}
static tree_t *pas_mod(tree_t *a, tree_t *b) {
    tree_t *b1 = b;
    if (!b || b->t != TT_ILIT || b->v.ival <= 0) { b1 = pas_ord_check(b, 1, 9223372036854775807LL, "i mod j with j zero or negative"); ast_push(b1, leaf_s(TT_QLIT, "6.7.2.2")); }
    return bin(TT_MOD, bin(TT_ADD, bin(TT_MOD, a, b1), pas_tree_clone(b)), pas_tree_clone(b));
}
static tree_t *mk_in(tree_t *elem, tree_t *set) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, "__pas_in")); ast_push(e, elem); ast_push(e, set); return e; }
static tree_t *mk_set_ctor(PNodeList *elems) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_set"));
    if (elems) for (int i = 0; i < elems->count; i++) ast_push(e, elems->items[i]);
    return e;
}
static void emit_proc(PNodeList *procs, tree_t *proc) {
    tree_t *st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":line", 0));
    ast_push(st, ast_attr_int(":stno", 0));
    ast_push(st, ast_attr_expr(":subj", proc));
    pnl_push(procs, st);
}
static tree_t *pas_str_to_alpha(const char *s, long long lo, long long high);
static tree_t *mk_array_init(const char *name, long long high);
static int pas_array_high_get(const char *name, long long *out);
static tree_t *mk_proc(const char *name, PNodeList *params, tree_t *body_stmt, int is_function, int decl_level, const char **lnames, int lcount) {
    tree_t *body_prog = ast_node_new(TT_PROGRAM);
    for (int _li = 0; lnames && _li < lcount; _li++) {
        long long _hi;
        if (lnames[_li] && pas_array_high_get(lnames[_li], &_hi)) ast_push(body_prog, bin(TT_ASSIGN, leaf_s(TT_VAR, lnames[_li]), mk_array_init(lnames[_li], _hi)));
    }
    if (body_stmt && body_stmt->t == TT_PROGRAM) { for (int i = 0; i < body_stmt->n; i++) ast_push(body_prog, body_stmt->c[i]); } else if (body_stmt) { ast_push(body_prog, body_stmt); }
    tree_t *proc = ast_node_new(TT_PROC_DECL);
    proc->v.sval = (char *)name;
    ast_push(proc, leaf_s(TT_VAR, name));
    tree_t *vlist = ast_node_new(TT_VLIST);
    long long byref = 0;
    unsigned long long camask = 0;
    long long calo[64];
    for (int _z = 0; _z < 64; _z++) calo[_z] = 0;
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *pv = params->items[i];
        if (pv && pv->n > 0) { int _isvar = 0; for (int _c = 0; _c < pv->n; _c++) if (pv->c[_c] && pv->c[_c]->t == TT_SUCCEED) _isvar = 1; if (_isvar && i < 64) byref |= (1LL << i); pv->n = 0; }
        if (pv && pv->v.sval && i < 64 && pas_is_chararr(pv->v.sval)) { camask |= (1ULL << i); calo[i] = pas_chararr_lo(pv->v.sval); }
        ast_push(vlist, pv);
    }
    if (camask) pas_caparm_add(name, camask, calo);
    vlist->v.ival = byref;
    ast_push(proc, vlist);
    ast_push(proc, body_prog);
    if (is_function) ast_push(proc, leaf_s(TT_VAR, name));
    tree_t *locals = ast_node_new(TT_VLIST);
    locals->v.ival = decl_level;
    for (int i = 0; i < lcount; i++) if (lnames[i]) ast_push(locals, leaf_s(TT_VAR, lnames[i]));
    ast_push(proc, locals);
    return proc;
}
static void pas_func_add(const char *name) {
    if (g_lower.pas.sem.pas_nfunc < 256 && name) {
        g_lower.pas.sem.pas_funcs[g_lower.pas.sem.pas_nfunc].name = ct_strdup(name);
        g_lower.pas.sem.pas_funcs[g_lower.pas.sem.pas_nfunc].nvp = 0;
        g_lower.pas.sem.pas_funcs[g_lower.pas.sem.pas_nfunc].nrp = 0;
        g_lower.pas.sem.pas_funcs[g_lower.pas.sem.pas_nfunc].nsp = 0;
        g_lower.pas.sem.pas_nfunc++;
    }
}
static int pas_is_func(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nfunc; i++) if (g_lower.pas.sem.pas_funcs[i].name && !strcmp(g_lower.pas.sem.pas_funcs[i].name, name)) return 1;
    return 0;
}
static tree_t *pas_range_wrap(tree_t *val, long long lo, long long hi, const char *clause) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_range_check"));
    ast_push(e, val);
    ast_push(e, ilit(lo));
    ast_push(e, ilit(hi));
    int isset = clause && !strcmp(clause, "set");
    const char *cl = isset ? "6.8.2.2" : clause;
    if (g_lower.pas.sem.pas_range_check_on == 2 && cl) ast_push(e, leaf_s(TT_QLIT, cl));
    else if (isset) ast_push(e, leaf_s(TT_QLIT, ""));
    if (isset) ast_push(e, leaf_s(TT_QLIT, "set"));
    return e;
}
static long long pas_hash_stable(const char *nm) {
    unsigned long long h = 1469598103934665603ULL;
    for (const char *p = nm; *p; p++) { h ^= (unsigned char)*p; h *= 1099511628211ULL; }
    unsigned long long v = (1000000000ULL + (h % 1000000000ULL)) & ~0xFULL;
    return (long long)v;
}
static int pas_addr_base_offset(tree_t *e, long long *out) {
    if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) { *out = pas_hash_stable(e->v.sval); return 1; }
    if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) {
        long long base;
        if (!pas_addr_base_offset(e->c[0], &base)) return 0;
        const char *rt = pas_with_sel_rtype(e->c[0]);
        if (!rt) return 0;
        long long foff;
        if (!pas_rectype_field_offset(rt, (int)e->c[1]->v.ival, &foff)) return 0;
        *out = base + foff;
        return 1;
    }
    return 0;
}
static tree_t *pas_addr_of_proc(tree_t *e) {
    if (!e) return e;
    const char *nm = NULL;
    if (e->t == TT_VAR && e->v.sval && (pas_is_proc(e->v.sval) || pas_is_func(e->v.sval))) nm = e->v.sval;
    else if (e->t == TT_FNC && e->n == 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_func(e->c[0]->v.sval)) nm = e->c[0]->v.sval;
    if (nm) return ilit(pas_hash_stable(nm));
    { long long addr; if (pas_addr_base_offset(e, &addr)) return ilit(addr); }
    return e;
}
static void pas_proc_add(const char *name) {
    if (g_lower.pas.sem.pas_nproc < 256 && name) {
        g_lower.pas.sem.pas_procs[g_lower.pas.sem.pas_nproc].name = ct_strdup(name);
        g_lower.pas.sem.pas_procs[g_lower.pas.sem.pas_nproc].nvp = 0;
        g_lower.pas.sem.pas_procs[g_lower.pas.sem.pas_nproc].nrp = 0;
        g_lower.pas.sem.pas_procs[g_lower.pas.sem.pas_nproc].nsp = 0;
        g_lower.pas.sem.pas_nproc++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_vparam_scan(PNodeList *params, int *out) {
    int nv = 0;
    if (!params) return 0;
    for (int j = 0; j < params->count && nv < 16; j++) {
        tree_t *it = params->items[j];
        int isvar = 0;
        if (it) for (int c = 0; c < it->n; c++) if (it->c[c] && it->c[c]->t == TT_SUCCEED) { isvar = 1; break; }
        out[nv++] = isvar;
    }
    return nv;
}
static int pas_realparam_scan(PNodeList *params, int *out) {
    int nv = 0;
    if (!params) return 0;
    for (int j = 0; j < params->count && nv < 16; j++) {
        tree_t *it = params->items[j];
        int isreal = 0;
        if (it) for (int c = 0; c < it->n; c++) if (it->c[c] && it->c[c]->t == TT_FLIT) { isreal = 1; break; }
        out[nv++] = isreal;
    }
    return nv;
}
static int pas_strparam_scan(PNodeList *params, int *out) {
    int nv = 0;
    if (!params) return 0;
    for (int j = 0; j < params->count && nv < 16; j++) {
        tree_t *it = params->items[j];
        int isstr = 0;
        if (it) for (int c = 0; c < it->n; c++) if (it->c[c] && it->c[c]->t == TT_QLIT) { isstr = 1; break; }
        out[nv++] = isstr;
    }
    return nv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_proc_vparams(const char *name, PNodeList *params) {
    if (!name || !params) return;
    for (int i = g_lower.pas.sem.pas_nproc - 1; i >= 0; i--) if (g_lower.pas.sem.pas_procs[i].name && !strcmp(g_lower.pas.sem.pas_procs[i].name, name)) {
        g_lower.pas.sem.pas_procs[i].nvp = pas_vparam_scan(params, g_lower.pas.sem.pas_procs[i].vp);
        g_lower.pas.sem.pas_procs[i].nrp = pas_realparam_scan(params, g_lower.pas.sem.pas_procs[i].rp);
        g_lower.pas.sem.pas_procs[i].nsp = pas_strparam_scan(params, g_lower.pas.sem.pas_procs[i].sp);
        return;
    }
    for (int i = g_lower.pas.sem.pas_nfunc - 1; i >= 0; i--) if (g_lower.pas.sem.pas_funcs[i].name && !strcmp(g_lower.pas.sem.pas_funcs[i].name, name)) {
        g_lower.pas.sem.pas_funcs[i].nvp = pas_vparam_scan(params, g_lower.pas.sem.pas_funcs[i].vp);
        g_lower.pas.sem.pas_funcs[i].nrp = pas_realparam_scan(params, g_lower.pas.sem.pas_funcs[i].rp);
        g_lower.pas.sem.pas_funcs[i].nsp = pas_strparam_scan(params, g_lower.pas.sem.pas_funcs[i].sp);
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_required_needs_params(const char *name) {
    const char *nm[] = { "get", "put", "reset", "rewrite", "new", "dispose", "pack", "unpack", "read", "write" };
    const char *cl[] = { "6.6.5.2", "6.6.5.2", "6.6.5.2", "6.6.5.2", "6.6.5.3", "6.6.5.3", "6.6.5.4", "6.6.5.4", "6.9.1", "6.9.3" };
    for (size_t i = 0; name && i < sizeof nm / sizeof nm[0]; i++) if (!strcmp(name, nm[i])) {
        fprintf(stderr, "pascal: ISO 7185 %s violation: the required procedure '%s' is activated with no actual-parameters, and its activation takes at least one\n", cl[i], name);
        g_lower.pas.sem.pas_iso_errors++;
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_call_arity(const char *name, PNodeList *args) {
    const char *req[] = { "abs", "arctan", "chr", "cos", "dispose", "eof", "eoln", "exp", "get", "ln", "new", "odd", "ord", "pack", "page", "pred", "put", "read", "readln", "reset", "rewrite",
        "round", "sin", "sqr", "sqrt", "succ", "trunc", "unpack", "write", "writeln" };
    if (!name || !args || args->count % 2) return;
    for (size_t i = 0; i < sizeof req / sizeof req[0]; i++) if (!strcmp(name, req[i])) return;
    for (int i = 1; i < args->count; i += 2) if (!args->items[i] || args->items[i]->t != TT_ILIT || args->items[i]->v.ival != -1) return;
    int argc = args->count / 2, nent = 0, formals = -1;
    const char *clause = NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_nproc; i++) if (g_lower.pas.sem.pas_procs[i].name && !strcmp(g_lower.pas.sem.pas_procs[i].name, name)) {
        if (g_lower.pas.sem.pas_procs[i].nvp >= 16 || g_lower.pas.sem.pas_procs[i].nvp == argc) return;
        nent++;
        formals = g_lower.pas.sem.pas_procs[i].nvp;
        clause = "6.8.2.3";
    }
    for (int i = 0; i < g_lower.pas.sem.pas_nfunc; i++) if (g_lower.pas.sem.pas_funcs[i].name && !strcmp(g_lower.pas.sem.pas_funcs[i].name, name)) {
        if (g_lower.pas.sem.pas_funcs[i].nvp >= 16 || g_lower.pas.sem.pas_funcs[i].nvp == argc) return;
        nent++;
        formals = g_lower.pas.sem.pas_funcs[i].nvp;
        clause = "6.7.3";
    }
    if (!nent) return;
    fprintf(stderr, "pascal: ISO 7185 %s violation: '%s' is activated with %d actual-parameter(s), and the number of actual-parameters shall equal the number of its formal-parameters (%d)\n", clause,
        name, argc, formals);
    g_lower.pas.sem.pas_iso_errors++;
}
static int pas_proc_param_is_var(const char *name, int idx) {
    if (!name || idx < 0 || idx >= 16) return 0;
    for (int i = g_lower.pas.sem.pas_nproc - 1; i >= 0; i--) if (g_lower.pas.sem.pas_procs[i].name && !strcmp(g_lower.pas.sem.pas_procs[i].name, name)) return (idx < g_lower.pas.sem.pas_procs[i].nvp)
        ? g_lower.pas.sem.pas_procs[i].vp[idx] : 0;
    for (int i = g_lower.pas.sem.pas_nfunc - 1; i >= 0; i--) if (g_lower.pas.sem.pas_funcs[i].name && !strcmp(g_lower.pas.sem.pas_funcs[i].name, name)) return (idx < g_lower.pas.sem.pas_funcs[i].nvp)
        ? g_lower.pas.sem.pas_funcs[i].vp[idx] : 0;
    return 0;
}
static int pas_proc_param_is_string(const char *name, int idx) {
    if (!name || idx < 0 || idx >= 16) return 0;
    for (int i = g_lower.pas.sem.pas_nproc - 1; i >= 0; i--) if (g_lower.pas.sem.pas_procs[i].name && !strcmp(g_lower.pas.sem.pas_procs[i].name, name)) return (idx < g_lower.pas.sem.pas_procs[i].nsp)
        ? g_lower.pas.sem.pas_procs[i].sp[idx] : 0;
    for (int i = g_lower.pas.sem.pas_nfunc - 1; i >= 0; i--) if (g_lower.pas.sem.pas_funcs[i].name && !strcmp(g_lower.pas.sem.pas_funcs[i].name, name)) return (idx < g_lower.pas.sem.pas_funcs[i].nsp)
        ? g_lower.pas.sem.pas_funcs[i].sp[idx] : 0;
    return 0;
}
static int pas_proc_param_is_real(const char *name, int idx) {
    if (!name || idx < 0 || idx >= 16) return 0;
    for (int i = g_lower.pas.sem.pas_nproc - 1; i >= 0; i--) if (g_lower.pas.sem.pas_procs[i].name && !strcmp(g_lower.pas.sem.pas_procs[i].name, name)) return (idx < g_lower.pas.sem.pas_procs[i].nrp)
        ? g_lower.pas.sem.pas_procs[i].rp[idx] : 0;
    for (int i = g_lower.pas.sem.pas_nfunc - 1; i >= 0; i--) if (g_lower.pas.sem.pas_funcs[i].name && !strcmp(g_lower.pas.sem.pas_funcs[i].name, name)) return (idx < g_lower.pas.sem.pas_funcs[i].nrp)
        ? g_lower.pas.sem.pas_funcs[i].rp[idx] : 0;
    return 0;
}
static int pas_is_proc(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nproc; i++) if (g_lower.pas.sem.pas_procs[i].name && !strcmp(g_lower.pas.sem.pas_procs[i].name, name)) return 1;
    return 0;
}
static void pas_caparm_add(const char *name, unsigned long long m, const long long *lo) {
    if (g_lower.pas.sem.pas_ncaparm < 256 && name && m) {
        g_lower.pas.sem.pas_caparm[g_lower.pas.sem.pas_ncaparm].name = ct_strdup(name);
        g_lower.pas.sem.pas_caparm[g_lower.pas.sem.pas_ncaparm].camask = m;
        for (int i = 0; i < 64; i++) g_lower.pas.sem.pas_caparm[g_lower.pas.sem.pas_ncaparm].lo[i] = lo ? lo[i] : 0;
        g_lower.pas.sem.pas_ncaparm++;
    }
}
static unsigned long long pas_caparm_mask(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_ncaparm; i++) if (g_lower.pas.sem.pas_caparm[i].name && !strcmp(g_lower.pas.sem.pas_caparm[i].name, name)) return g_lower.pas.sem.pas_caparm[i].camask;
    return 0;
}
static long long pas_caparm_lo(const char *name, int pos) {
    if (!name || pos < 0 || pos >= 64) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_ncaparm; i++) if (g_lower.pas.sem.pas_caparm[i].name && !strcmp(g_lower.pas.sem.pas_caparm[i].name, name)) return g_lower.pas.sem.pas_caparm[i].lo[pos];
    return 0;
}
static void pas_const_add(const char *name, long long v) {
    if (g_lower.pas.sem.pas_nconst < 256 && name) {
        g_lower.pas.sem.pas_consts[g_lower.pas.sem.pas_nconst].name = ct_strdup(name);
        g_lower.pas.sem.pas_consts[g_lower.pas.sem.pas_nconst].val = v;
        g_lower.pas.sem.pas_consts[g_lower.pas.sem.pas_nconst].isenum = 0;
        g_lower.pas.sem.pas_consts[g_lower.pas.sem.pas_nconst].uid = pas_scope_uid(name);
        g_lower.pas.sem.pas_nconst++;
    }
}
static void pas_const_mark_enum(const char *name) {
    for (int i = g_lower.pas.sem.pas_nconst - 1; name && i >= 0; i--) if (g_lower.pas.sem.pas_consts[i].name && !strcmp(g_lower.pas.sem.pas_consts[i].name, name)) {
        g_lower.pas.sem.pas_consts[i].isenum = 1;
        return;
    }
}
static int pas_const_isenum(const char *name) {
    for (int i = g_lower.pas.sem.pas_nconst - 1; name && i >= 0; i--) if (g_lower.pas.sem.pas_consts[i].name && !strcmp(g_lower.pas.sem.pas_consts[i].name, name)) return g_lower.pas.sem.pas_consts[i]
        .isenum;
    return 0;
}
static int pas_const_get(const char *name, long long *out) {
    if (!name) return 0;
    int u = pas_scope_uid(name);
    for (int i = 0; i < g_lower.pas.sem.pas_nconst; i++)
        if (g_lower.pas.sem.pas_consts[i].name && !strcmp(g_lower.pas.sem.pas_consts[i].name, name) && (g_lower.pas.sem.pas_consts[i].uid == 0 || g_lower.pas.sem.pas_consts[i].uid == u)) {
        *out = g_lower.pas.sem.pas_consts[i].val;
        return 1;
    }
    return 0;
}
static void pas_rconst_add(const char *name, double v) {
    if (g_lower.pas.sem.pas_nrconst < 64 && name) {
        g_lower.pas.sem.pas_rconsts[g_lower.pas.sem.pas_nrconst].name = ct_strdup(name);
        g_lower.pas.sem.pas_rconsts[g_lower.pas.sem.pas_nrconst].val = v;
        g_lower.pas.sem.pas_nrconst++;
    }
}
static int pas_rconst_get(const char *name, double *out) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrconst; i++) if (g_lower.pas.sem.pas_rconsts[i].name && !strcmp(g_lower.pas.sem.pas_rconsts[i].name, name)) {
        *out = g_lower.pas.sem.pas_rconsts[i].val;
        return 1;
    }
    return 0;
}
static void pas_sconst_add(const char *name, const char *v) {
    if (g_lower.pas.sem.pas_nsconst < 64 && name && v) {
        g_lower.pas.sem.pas_sconsts[g_lower.pas.sem.pas_nsconst].name = ct_strdup(name);
        g_lower.pas.sem.pas_sconsts[g_lower.pas.sem.pas_nsconst].val = ct_strdup(v);
        g_lower.pas.sem.pas_nsconst++;
    }
}
static const char *pas_sconst_get(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nsconst; i++) if (g_lower.pas.sem.pas_sconsts[i].name && !strcmp(g_lower.pas.sem.pas_sconsts[i].name, name)) return g_lower.pas.sem.pas_sconsts[i].val;
    return 0;
}
static void pas_array_add(const char *name, long long high) {
    if (g_lower.pas.sem.pas_narray < 256 && name) {
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].name = ct_strdup(name);
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].high = high;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].ncols = -1;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].isbool = 0;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].is_param = 0;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].is_local = (g_lower.pas.sem.pas_level >= 2);
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].low = pas_array_decl_low();
        g_lower.pas.sem.pas_narray++;
    }
}
static void pas_array_add2d(const char *name, long long high, long long ncols) {
    if (g_lower.pas.sem.pas_narray < 256 && name) {
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].name = ct_strdup(name);
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].high = high;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].ncols = ncols;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].isbool = 0;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].is_param = 0;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].is_local = (g_lower.pas.sem.pas_level >= 2);
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].low = g_lower.pas.sem.pas_pend_sub_low;
        g_lower.pas.sem.pas_narray++;
    }
}
static void pas_array_add2d_param(const char *name, long long high, long long ncols) {
    if (g_lower.pas.sem.pas_narray < 256 && name) {
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].name = ct_strdup(name);
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].high = high;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].ncols = ncols;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].isbool = 0;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].is_param = 1;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].is_local = 0;
        g_lower.pas.sem.pas_arrays[g_lower.pas.sem.pas_narray].low = 0;
        g_lower.pas.sem.pas_narray++;
    }
}
static long long pas_array_ncols(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) return g_lower.pas.sem.pas_arrays[i].ncols;
    return -1;
}
static long long pas_array_low(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) return g_lower.pas.sem.pas_arrays[i].low;
    return 0;
}
static int pas_array_is_param(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) return g_lower.pas.sem.pas_arrays[i].is_param;
    return 0;
}
static void pas_assigned_add(const char *n) {
    if (!n) return;
    for (int i = 0; i < g_lower.pas.sem.pas_nassigned; i++) if (g_lower.pas.sem.pas_assigned[i].name && !strcmp(g_lower.pas.sem.pas_assigned[i].name, n)) return;
    if (g_lower.pas.sem.pas_nassigned < 512) { g_lower.pas.sem.pas_assigned[g_lower.pas.sem.pas_nassigned].name = ct_strdup(n); g_lower.pas.sem.pas_nassigned++; }
}
static int pas_assigned_get(const char *n) {
    if (!n) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nassigned; i++) if (g_lower.pas.sem.pas_assigned[i].name && !strcmp(g_lower.pas.sem.pas_assigned[i].name, n)) return 1;
    return 0;
}
static const char *pas_selector_base_name(tree_t *e) {
    while (e) {
        if (e->t == TT_VAR) return e->v.sval;
        if ((e->t == TT_IDX || e->t == TT_FIELD) && e->n >= 1 && e->c[0]) { e = e->c[0]; continue; }
        if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref") && e->c[1]) { e = e->c[1]; continue; }
        break;
    }
    return NULL;
}
static int pas_is_agg_local(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++)
        if (g_lower.pas.sem.pas_arrays[i].name && g_lower.pas.sem.pas_arrays[i].is_local && !g_lower.pas.sem.pas_arrays[i].is_param && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) return 1;
    return 0;
}
static int pas_array_high_get(const char *name, long long *out) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !g_lower.pas.sem.pas_arrays[i].is_param && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) {
        *out = g_lower.pas.sem.pas_arrays[i].high;
        return 1;
    }
    return 0;
}
static void pas_arrtype_add(const char *name, long long high, int ndim2, long long ncols) {
    if (g_lower.pas.sem.pas_narrtype < 64 && name) {
        g_lower.pas.sem.pas_arrtypes[g_lower.pas.sem.pas_narrtype].name = ct_strdup(name);
        g_lower.pas.sem.pas_arrtypes[g_lower.pas.sem.pas_narrtype].high = high;
        g_lower.pas.sem.pas_arrtypes[g_lower.pas.sem.pas_narrtype].ndim2 = ndim2;
        g_lower.pas.sem.pas_arrtypes[g_lower.pas.sem.pas_narrtype].ncols = ncols;
        g_lower.pas.sem.pas_arrtypes[g_lower.pas.sem.pas_narrtype].ischar = g_lower.pas.sem.pas_pend_arr_ischar;
        g_lower.pas.sem.pas_arrtypes[g_lower.pas.sem.pas_narrtype].isbool = g_lower.pas.sem.pas_pend_isbool;
        g_lower.pas.sem.pas_arrtypes[g_lower.pas.sem.pas_narrtype].lo = ndim2 ? (g_lower.pas.sem.pas_pend_sub_low > 0 ? g_lower.pas.sem.pas_pend_sub_low : 0) : g_lower.pas.sem.pas_pend_sub_low;
        g_lower.pas.sem.pas_narrtype++;
    }
}
static long long pas_arrtype_high(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_narrtype; i++) if (g_lower.pas.sem.pas_arrtypes[i].name && !strcmp(g_lower.pas.sem.pas_arrtypes[i].name, name)) return g_lower.pas.sem.pas_arrtypes[i].high;
    return -1;
}
static long long pas_arrtype_ncols(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_narrtype; i++) if (g_lower.pas.sem.pas_arrtypes[i].name && !strcmp(g_lower.pas.sem.pas_arrtypes[i].name, name)) return g_lower.pas.sem.pas_arrtypes[i]
        .ndim2 ? g_lower.pas.sem.pas_arrtypes[i].ncols : -1;
    return -1;
}
static long long pas_arrtype_lo(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narrtype; i++) if (g_lower.pas.sem.pas_arrtypes[i].name && !strcmp(g_lower.pas.sem.pas_arrtypes[i].name, name)) return g_lower.pas.sem.pas_arrtypes[i].lo;
    return 0;
}
static int pas_arrtype_isbool(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narrtype; i++) if (g_lower.pas.sem.pas_arrtypes[i].name && !strcmp(g_lower.pas.sem.pas_arrtypes[i].name, name)) return g_lower.pas.sem.pas_arrtypes[i]
        .isbool;
    return 0;
}
static void pas_array_mark_bool(const char *name) {
    for (int i = g_lower.pas.sem.pas_narray - 1; name && i >= 0; i--) if (g_lower.pas.sem.pas_arrays[i].name && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) {
        g_lower.pas.sem.pas_arrays[i].isbool = 1;
        return;
    }
}
static int pas_array_isbool(const char *name) {
    for (int i = g_lower.pas.sem.pas_narray - 1; name && i >= 0; i--) if (g_lower.pas.sem.pas_arrays[i].name && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) return g_lower.pas.sem.pas_arrays[i]
        .isbool;
    return 0;
}
static int pas_arrtype_ischar(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narrtype; i++) if (g_lower.pas.sem.pas_arrtypes[i].name && !strcmp(g_lower.pas.sem.pas_arrtypes[i].name, name)) return g_lower.pas.sem.pas_arrtypes[i]
        .ischar;
    return 0;
}
static void pas_enumtype_add(const char *n, long long h) {
    if (g_lower.pas.sem.pas_nenum < 64 && n) {
        g_lower.pas.sem.pas_enumtypes[g_lower.pas.sem.pas_nenum].name = ct_strdup(n);
        g_lower.pas.sem.pas_enumtypes[g_lower.pas.sem.pas_nenum].high = h;
        g_lower.pas.sem.pas_enumtypes[g_lower.pas.sem.pas_nenum].min_size = g_lower.pas.sem.pas_min_enum_size;
        g_lower.pas.sem.pas_nenum++;
    }
}
static long long pas_enumtype_high(const char *n) {
    if (!n) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_nenum; i++) if (g_lower.pas.sem.pas_enumtypes[i].name && !strcmp(g_lower.pas.sem.pas_enumtypes[i].name, n)) return g_lower.pas.sem.pas_enumtypes[i].high;
    return -1;
}
static int pas_enumtype_min_size(const char *n, long long *out) {
    if (!n) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nenum; i++) if (g_lower.pas.sem.pas_enumtypes[i].name && !strcmp(g_lower.pas.sem.pas_enumtypes[i].name, n)) {
        *out = g_lower.pas.sem.pas_enumtypes[i].min_size;
        return 1;
    }
    return 0;
}
static void pas_enumnames_add(const char *tn, const char *names) {
    if (g_lower.pas.sem.pas_nenumname < 64 && tn && names && names[0]) {
        g_lower.pas.sem.pas_enumnames[g_lower.pas.sem.pas_nenumname].tname = ct_strdup(tn);
        g_lower.pas.sem.pas_enumnames[g_lower.pas.sem.pas_nenumname].names = ct_strdup(names);
        g_lower.pas.sem.pas_nenumname++;
    }
}
static int pas_enumnames_idx(const char *tn) {
    if (!tn) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_nenumname; i++) if (g_lower.pas.sem.pas_enumnames[i].tname && !strcmp(g_lower.pas.sem.pas_enumnames[i].tname, tn)) return i;
    return -1;
}
static const char *pas_enumnames_by_idx(int i) { return (i >= 0 && i < g_lower.pas.sem.pas_nenumname) ? g_lower.pas.sem.pas_enumnames[i].names : NULL; }
static void pas_enumarr_add(const char *a, const char *et) {
    if (g_lower.pas.sem.pas_nenumarr < 128 && a && et) {
        g_lower.pas.sem.pas_enumarrs[g_lower.pas.sem.pas_nenumarr].aname = ct_strdup(a);
        g_lower.pas.sem.pas_enumarrs[g_lower.pas.sem.pas_nenumarr].etype = ct_strdup(et);
        g_lower.pas.sem.pas_nenumarr++;
    }
}
static const char *pas_enumarr_get(const char *a) {
    if (!a) return NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_nenumarr; i++) if (g_lower.pas.sem.pas_enumarrs[i].aname && !strcmp(g_lower.pas.sem.pas_enumarrs[i].aname, a)) return g_lower.pas.sem.pas_enumarrs[i].etype;
    return NULL;
}
static void pas_subtype_add(const char *n, long long lo, long long hi) {
    if (g_lower.pas.sem.pas_nsubtype < 64 && n) {
        g_lower.pas.sem.pas_subtypes[g_lower.pas.sem.pas_nsubtype].name = ct_strdup(n);
        g_lower.pas.sem.pas_subtypes[g_lower.pas.sem.pas_nsubtype].low = lo;
        g_lower.pas.sem.pas_subtypes[g_lower.pas.sem.pas_nsubtype].high = hi;
        g_lower.pas.sem.pas_subtypes[g_lower.pas.sem.pas_nsubtype].isenum = g_lower.pas.sem.pas_last_const_enum;
        g_lower.pas.sem.pas_nsubtype++;
    }
}
static int pas_subtype_isenum(const char *n) {
    if (!n) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nsubtype; i++) if (g_lower.pas.sem.pas_subtypes[i].name && !strcmp(g_lower.pas.sem.pas_subtypes[i].name, n)) return g_lower.pas.sem.pas_subtypes[i].isenum;
    return 0;
}
static long long pas_subtype_high(const char *n) {
    if (!n) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_nsubtype; i++) if (g_lower.pas.sem.pas_subtypes[i].name && !strcmp(g_lower.pas.sem.pas_subtypes[i].name, n)) return g_lower.pas.sem.pas_subtypes[i].high;
    return -1;
}
static long long pas_subtype_low(const char *n) {
    if (!n) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nsubtype; i++) if (g_lower.pas.sem.pas_subtypes[i].name && !strcmp(g_lower.pas.sem.pas_subtypes[i].name, n)) return g_lower.pas.sem.pas_subtypes[i].low;
    return 0;
}
static void pas_subvar_add(const char *n, long long lo, long long hi) {
    if (g_lower.pas.sem.pas_nsubvar < 256 && n) {
        g_lower.pas.sem.pas_subvars[g_lower.pas.sem.pas_nsubvar].name = ct_strdup(n);
        g_lower.pas.sem.pas_subvars[g_lower.pas.sem.pas_nsubvar].uid = pas_scope_uid(n);
        g_lower.pas.sem.pas_subvars[g_lower.pas.sem.pas_nsubvar].low = lo;
        g_lower.pas.sem.pas_subvars[g_lower.pas.sem.pas_nsubvar].high = hi;
        g_lower.pas.sem.pas_nsubvar++;
    }
}
static int pas_subvar_get(const char *n, long long *lo, long long *hi) {
    if (!n) return 0;
    int u = pas_scope_uid(n);
    for (int i = g_lower.pas.sem.pas_nsubvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_subvars[i].name && !strcmp(g_lower.pas.sem.pas_subvars[i].name, n) && g_lower.pas.sem.pas_subvars[i].uid == u) {
        *lo = g_lower.pas.sem.pas_subvars[i].low;
        *hi = g_lower.pas.sem.pas_subvars[i].high;
        return 1;
    }
    return 0;
}
static void pas_typealias_add(const char *n, const char *t) {
    if (g_lower.pas.sem.pas_ntypealias < 64 && n && t && strcmp(n, t)) {
        g_lower.pas.sem.pas_typealias[g_lower.pas.sem.pas_ntypealias].name = ct_strdup(n);
        g_lower.pas.sem.pas_typealias[g_lower.pas.sem.pas_ntypealias].target = ct_strdup(t);
        g_lower.pas.sem.pas_ntypealias++;
    }
}
static const char *pas_typealias_get(const char *n) {
    if (!n) return NULL;
    for (int i = g_lower.pas.sem.pas_ntypealias - 1; i >= 0; i--) if (g_lower.pas.sem.pas_typealias[i].name && !strcmp(g_lower.pas.sem.pas_typealias[i].name, n))
        return g_lower.pas.sem.pas_typealias[i].target;
    return NULL;
}
static void pas_scalarvartype_add(const char *vn, const char *tn) {
    if (g_lower.pas.sem.pas_nscalarvartype < 512 && vn && tn) {
        g_lower.pas.sem.pas_scalarvartype[g_lower.pas.sem.pas_nscalarvartype].vname = ct_strdup(vn);
        g_lower.pas.sem.pas_scalarvartype[g_lower.pas.sem.pas_nscalarvartype].tname = ct_strdup(tn);
        g_lower.pas.sem.pas_scalarvartype[g_lower.pas.sem.pas_nscalarvartype].is_global = (g_lower.pas.sem.pas_ldepth == 0);
        g_lower.pas.sem.pas_scalarvartype[g_lower.pas.sem.pas_nscalarvartype].uid = pas_scope_uid(vn);
        g_lower.pas.sem.pas_nscalarvartype++;
    }
}
static const char *pas_scalarvartype_get(const char *vn) {
    if (!vn) return NULL;
    for (int i = g_lower.pas.sem.pas_nscalarvartype - 1; i >= 0; i--) if (g_lower.pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower.pas.sem.pas_scalarvartype[i].vname, vn))
        return g_lower.pas.sem.pas_scalarvartype[i].tname;
    return NULL;
}
static int pas_string_type_kind(const char *t) {
    for (int guard = 0; t && guard < 8; guard++) {
        if (!strcmp(t, "shortstring") || !strcmp(t, "openstring")) return 2;
        if (!strcmp(t, "string") || !strcmp(t, "ansistring") || !strcmp(t, "widestring") || !strcmp(t, "unicodestring")) return 1;
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return 0;
}
static int pas_var_string_kind(const char *name) {
    int u = pas_scope_uid(name);
    for (int i = g_lower.pas.sem.pas_nscalarvartype - 1; name && i >= 0; i--)
        if (g_lower.pas.sem.pas_scalarvartype[i].vname && g_lower.pas.sem.pas_scalarvartype[i].uid == u && !strcmp(g_lower.pas.sem.pas_scalarvartype[i].vname, name))
        return pas_string_type_kind(g_lower.pas.sem.pas_scalarvartype[i].tname);
    return 0;
}
static void pas_tfcomp_add(const char *n, long long lo, long long hi, int ischar) {
    if (g_lower.pas.sem.pas_ntfcomp < 128 && n) {
        g_lower.pas.sem.pas_tfcomp[g_lower.pas.sem.pas_ntfcomp].name = ct_strdup(n);
        g_lower.pas.sem.pas_tfcomp[g_lower.pas.sem.pas_ntfcomp].low = lo;
        g_lower.pas.sem.pas_tfcomp[g_lower.pas.sem.pas_ntfcomp].high = hi;
        g_lower.pas.sem.pas_tfcomp[g_lower.pas.sem.pas_ntfcomp].ischar = ischar;
        g_lower.pas.sem.pas_ntfcomp++;
    }
}
static int pas_tfcomp_range(const char *n, long long *lo, long long *hi) {
    if (!n) return 0;
    for (int i = g_lower.pas.sem.pas_ntfcomp - 1; i >= 0; i--)
        if (g_lower.pas.sem.pas_tfcomp[i].name && !strcmp(g_lower.pas.sem.pas_tfcomp[i].name, n) && g_lower.pas.sem.pas_tfcomp[i].high >= g_lower.pas.sem.pas_tfcomp[i].low) {
        *lo = g_lower.pas.sem.pas_tfcomp[i].low;
        *hi = g_lower.pas.sem.pas_tfcomp[i].high;
        return 1;
    }
    return 0;
}
static int pas_tfcomp_nonchar(const char *n) {
    if (!n) return 0;
    for (int i = g_lower.pas.sem.pas_ntfcomp - 1; i >= 0; i--) if (g_lower.pas.sem.pas_tfcomp[i].name && !strcmp(g_lower.pas.sem.pas_tfcomp[i].name, n)) return !g_lower.pas.sem.pas_tfcomp[i].ischar;
    return 0;
}
#define PAS_FIELD_MAX 128
static void pas_pend_nest_save(void) {
    if (g_lower.pas.sem.pas_scope.npnest >= g_lower.pas.sem.pas_scope.cpnest) {
        g_lower.pas.sem.pas_scope.cpnest = g_lower.pas.sem.pas_scope.cpnest ? g_lower.pas.sem.pas_scope.cpnest * 2 : 4;
        g_lower.pas.sem.pas_scope.pnest = (struct pas_pend_frame *)ct_grow(g_lower.pas.sem.pas_scope.pnest, (size_t)g_lower.pas.sem.pas_scope.cpnest * sizeof *g_lower.pas.sem.pas_scope.pnest);
    }
    struct pas_pend_frame *fr = &g_lower.pas.sem.pas_scope.pnest[g_lower.pas.sem.pas_scope.npnest++];
    int n = g_lower.pas.sem.pas_pend_nf;
    fr->nf = n;
    for (int i = 0; i < n; i++) {
        fr->fields[i] = g_lower.pas.sem.pas_pend_fields[i];
        fr->fldptrto[i] = g_lower.pas.sem.pas_pend_fldptrto[i];
        fr->fldenum[i] = g_lower.pas.sem.pas_pend_fldenum[i];
        fr->fldrec[i] = g_lower.pas.sem.pas_pend_fldrec[i];
        fr->fldtypename[i] = g_lower.pas.sem.pas_pend_fldtypename[i];
        fr->fldca[i] = g_lower.pas.sem.pas_pend_fldca[i];
        fr->fldca_lo[i] = g_lower.pas.sem.pas_pend_fldca_lo[i];
        fr->fldca_hi[i] = g_lower.pas.sem.pas_pend_fldca_hi[i];
        fr->fldchar[i] = g_lower.pas.sem.pas_pend_fldchar[i];
        fr->fldfile[i] = g_lower.pas.sem.pas_pend_fldfile[i];
        fr->fldna[i] = g_lower.pas.sem.pas_pend_fldna[i];
        fr->fldna_lo[i] = g_lower.pas.sem.pas_pend_fldna_lo[i];
        fr->fldna_hi[i] = g_lower.pas.sem.pas_pend_fldna_hi[i];
        fr->fldov[i] = g_lower.pas.sem.pas_pend_fldov[i];
    }
    g_lower.pas.sem.pas_pend_nf = 0;
}
static void pas_pend_nest_restore(void) {
    if (g_lower.pas.sem.pas_scope.npnest <= 0) return;
    struct pas_pend_frame *fr = &g_lower.pas.sem.pas_scope.pnest[--g_lower.pas.sem.pas_scope.npnest];
    int n = fr->nf;
    for (int i = 0; i < n; i++) {
        g_lower.pas.sem.pas_pend_fields[i] = fr->fields[i];
        g_lower.pas.sem.pas_pend_fldptrto[i] = fr->fldptrto[i];
        g_lower.pas.sem.pas_pend_fldenum[i] = fr->fldenum[i];
        g_lower.pas.sem.pas_pend_fldrec[i] = fr->fldrec[i];
        g_lower.pas.sem.pas_pend_fldtypename[i] = fr->fldtypename[i];
        g_lower.pas.sem.pas_pend_fldca[i] = fr->fldca[i];
        g_lower.pas.sem.pas_pend_fldca_lo[i] = fr->fldca_lo[i];
        g_lower.pas.sem.pas_pend_fldca_hi[i] = fr->fldca_hi[i];
        g_lower.pas.sem.pas_pend_fldchar[i] = fr->fldchar[i];
        g_lower.pas.sem.pas_pend_fldfile[i] = fr->fldfile[i];
        g_lower.pas.sem.pas_pend_fldna[i] = fr->fldna[i];
        g_lower.pas.sem.pas_pend_fldna_lo[i] = fr->fldna_lo[i];
        g_lower.pas.sem.pas_pend_fldna_hi[i] = fr->fldna_hi[i];
        g_lower.pas.sem.pas_pend_fldov[i] = fr->fldov[i];
    }
    g_lower.pas.sem.pas_pend_nf = n;
}
static void pas_ptrtype_add(const char *p, const char *r) {
    if (g_lower.pas.sem.pas_nptrtype < PAS_REC_MAX && p && r) {
        int k = g_lower.pas.sem.pas_nptrtype++;
        g_lower.pas.sem.pas_ptrtypes[k].pname = ct_strdup(p);
        g_lower.pas.sem.pas_ptrtypes[k].rname = ct_strdup(r);
    }
}
static const char *pas_ptrtype_target(const char *p) {
    if (!p) return NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_nptrtype; i++) if (g_lower.pas.sem.pas_ptrtypes[i].pname && !strcmp(g_lower.pas.sem.pas_ptrtypes[i].pname, p)) return g_lower.pas.sem.pas_ptrtypes[i].rname;
    return NULL;
}
static void pas_arrptr_add(const char *v, const char *r) {
    if (g_lower.pas.sem.pas_narrptr < PAS_REC_MAX && v && r) {
        int k = g_lower.pas.sem.pas_narrptr++;
        g_lower.pas.sem.pas_arrptr[k].vname = ct_strdup(v);
        g_lower.pas.sem.pas_arrptr[k].rname = ct_strdup(r);
    }
}
static const char *pas_arrptr_target(const char *v) {
    if (!v) return NULL;
    for (int i = g_lower.pas.sem.pas_narrptr - 1; i >= 0; i--) if (g_lower.pas.sem.pas_arrptr[i].vname && !strcmp(g_lower.pas.sem.pas_arrptr[i].vname, v)) return g_lower.pas.sem.pas_arrptr[i].rname;
    return NULL;
}
static void pas_ptrvar_add(const char *v, const char *r) {
    if (g_lower.pas.sem.pas_nptrvar < PAS_REC_MAX && v && r) {
        int k = g_lower.pas.sem.pas_nptrvar++;
        g_lower.pas.sem.pas_ptrvars[k].vname = ct_strdup(v);
        g_lower.pas.sem.pas_ptrvars[k].rname = ct_strdup(r);
    }
}
static const char *pas_ptrvar_target(const char *v) {
    if (!v) return NULL;
    for (int i = g_lower.pas.sem.pas_nptrvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_ptrvars[i].vname && !strcmp(g_lower.pas.sem.pas_ptrvars[i].vname, v)) return g_lower.pas.sem.pas_ptrvars[i]
        .rname;
    return NULL;
}
static void pas_recvar_mark(void) { if (g_lower.pas.sem.pas_nrvmark < PAS_NEST_MAX_PV) g_lower.pas.sem.pas_rvmarks[g_lower.pas.sem.pas_nrvmark++] = g_lower.pas.sem.pas_nrecvar; }
static void pas_recvar_release(void) { if (g_lower.pas.sem.pas_nrvmark > 0) g_lower.pas.sem.pas_nrecvar = g_lower.pas.sem.pas_rvmarks[--g_lower.pas.sem.pas_nrvmark]; }
static void pas_ptrvar_mark(void) { pas_tfilevar_mark(); if (g_lower.pas.sem.pas_npvmark < PAS_NEST_MAX_PV) g_lower.pas.sem.pas_pvmarks[g_lower.pas.sem.pas_npvmark++] = g_lower.pas.sem.pas_nptrvar; }
static void pas_ptrvar_release(void) {
    pas_tfilevar_release();
    if (g_lower.pas.sem.pas_npvmark > 0) g_lower.pas.sem.pas_nptrvar = g_lower.pas.sem.pas_pvmarks[--g_lower.pas.sem.pas_npvmark];
    pas_scoped_release();
}
static void pas_fwd_save(const char *pn, PNodeList *params) {
    if (!pn || g_lower.pas.sem.pas_nfwdpv >= 64 || (g_lower.pas.sem.pas_npvmark == 0 && g_lower.pas.sem.pas_nrvmark == 0)) return;
    int k = g_lower.pas.sem.pas_nfwdpv++;
    g_lower.pas.sem.pas_fwdpv[k].pname = ct_strdup(pn);
    g_lower.pas.sem.pas_fwdpv[k].params = params;
    g_lower.pas.sem.pas_fwdpv[k].n = 0;
    g_lower.pas.sem.pas_fwdpv[k].rvn = 0;
    pas_scoped_save(g_lower.pas.sem.pas_fwdpv[k].sv, g_lower.pas.sem.pas_fwdpv[k].svn);
    if (g_lower.pas.sem.pas_npvmark > 0) for (int i = g_lower.pas.sem.pas_pvmarks[g_lower.pas.sem.pas_npvmark - 1]; i < g_lower.pas.sem.pas_nptrvar && g_lower.pas.sem.pas_fwdpv[k].n < 16; i++) {
        g_lower.pas.sem.pas_fwdpv[k].vnames[g_lower.pas.sem.pas_fwdpv[k].n] = g_lower.pas.sem.pas_ptrvars[i].vname;
        g_lower.pas.sem.pas_fwdpv[k].rnames[g_lower.pas.sem.pas_fwdpv[k].n] = g_lower.pas.sem.pas_ptrvars[i].rname;
        g_lower.pas.sem.pas_fwdpv[k].n++;
    }
    if (g_lower.pas.sem.pas_nrvmark > 0) for (int i = g_lower.pas.sem.pas_rvmarks[g_lower.pas.sem.pas_nrvmark - 1]; i < g_lower.pas.sem.pas_nrecvar && g_lower.pas.sem.pas_fwdpv[k].rvn < 16; i++)
        g_lower.pas.sem.pas_fwdpv[k].rvsave[g_lower.pas.sem.pas_fwdpv[k].rvn++] = g_lower.pas.sem.pas_recvars[i];
}
static PNodeList *pas_fwd_params(const char *pn, PNodeList *given) {
    if (given && given->count > 0) return given;
    if (pn) for (int i = 0; i < g_lower.pas.sem.pas_nfwdpv; i++) if (g_lower.pas.sem.pas_fwdpv[i].pname && !strcmp(g_lower.pas.sem.pas_fwdpv[i].pname, pn) && g_lower.pas.sem.pas_fwdpv[i].params)
        return g_lower.pas.sem.pas_fwdpv[i].params;
    return given;
}
static void pas_fwd_restore(const char *pn) {
    if (!pn) return;
    for (int i = 0; i < g_lower.pas.sem.pas_nfwdpv; i++) if (g_lower.pas.sem.pas_fwdpv[i].pname && !strcmp(g_lower.pas.sem.pas_fwdpv[i].pname, pn)) {
        for (int j = 0; j < g_lower.pas.sem.pas_fwdpv[i].n; j++) pas_ptrvar_add(g_lower.pas.sem.pas_fwdpv[i].vnames[j], g_lower.pas.sem.pas_fwdpv[i].rnames[j]);
        pas_scoped_restore(g_lower.pas.sem.pas_fwdpv[i].sv, g_lower.pas.sem.pas_fwdpv[i].svn);
        for (int j = 0; j < g_lower.pas.sem.pas_fwdpv[i].rvn && g_lower.pas.sem.pas_nrecvar < PAS_REC_MAX; j++) g_lower.pas.sem.pas_recvars[g_lower.pas.sem.pas_nrecvar++] =
            g_lower.pas.sem.pas_fwdpv[i].rvsave[j];
        return;
    }
}
static void pas_pend_reset(void) {
    g_lower.pas.sem.pas_pend_isarr = 0;
    g_lower.pas.sem.pas_pend_isbool = 0;
    g_lower.pas.sem.pas_pend_istfile = 0;
    g_lower.pas.sem.pas_pend_nf = 0;
    g_lower.pas.sem.pas_pend_ptrtarget = NULL;
    g_lower.pas.sem.pas_pend_arr_ptrto = NULL;
    g_lower.pas.sem.pas_pend_typename = NULL;
    g_lower.pas.sem.pas_pend_ischar = 0;
    g_lower.pas.sem.pas_pend_arr_ischar = 0;
    g_lower.pas.sem.pas_pend_enum_max = -1;
    g_lower.pas.sem.pas_pend_sub_low = 0;
    g_lower.pas.sem.pas_pend_sub_high = -1;
    g_lower.pas.sem.pas_pend_arr_ncols = -1;
    g_lower.pas.sem.pas_pend_arr_wrap = 0;
    g_lower.pas.sem.pas_pend_set_hi = -1;
    g_lower.pas.sem.pas_pend_esz = -1;
}
static long long pas_array_decl_low(void) {
    if (!g_lower.pas.sem.pas_pend_isarr && g_lower.pas.sem.pas_pend_typename && pas_arrtype_high(g_lower.pas.sem.pas_pend_typename) >= 0) return pas_arrtype_lo(g_lower.pas.sem.pas_pend_typename);
    return g_lower.pas.sem.pas_pend_sub_low;
}
static long long pas_array_nonparam_low(const char *name) {
    for (int i = 0; name && i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !g_lower.pas.sem.pas_arrays[i].is_param && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name))
        return g_lower.pas.sem.pas_arrays[i].low;
    return 0;
}
static void pas_vcase_push(void) {
    if (g_lower.pas.sem.pas_vcase_depth > 0 && g_lower.pas.sem.pas_vcase_depth <= 32) g_lower.pas.sem.pas_vcase_stk[g_lower.pas.sem.pas_vcase_depth - 1].nested = 1;
    if (g_lower.pas.sem.pas_vcase_depth < 32) {
        g_lower.pas.sem.pas_vcase_stk[g_lower.pas.sem.pas_vcase_depth].lo = 0;
        g_lower.pas.sem.pas_vcase_stk[g_lower.pas.sem.pas_vcase_depth].hi = 0;
        g_lower.pas.sem.pas_vcase_stk[g_lower.pas.sem.pas_vcase_depth].n = 0;
        g_lower.pas.sem.pas_vcase_stk[g_lower.pas.sem.pas_vcase_depth].first = 0;
        g_lower.pas.sem.pas_vcase_stk[g_lower.pas.sem.pas_vcase_depth].last = 0;
        g_lower.pas.sem.pas_vcase_stk[g_lower.pas.sem.pas_vcase_depth].nested = 0;
    }
    g_lower.pas.sem.pas_vcase_depth++;
}
static const char *pas_rk_base(const char *tn) { for (int guard = 0; tn && guard < 8; guard++) { const char *al = pas_typealias_get(tn); if (!al || !strcmp(al, tn)) break; tn = al; } return tn; }
static int pas_rk_scalar(const char *tn, int ischar, int isbool, long long esz, int *cls, long long *sz) {
    long long s = 0, lo = 0, hi = -1;
    const char *b;
    if (ischar) { *cls = 4; *sz = 1; return 1; }
    if (isbool) { *cls = 5; *sz = 1; return 1; }
    if (!tn) { if (esz < 1 || esz > 8) return 0; *cls = (g_lower.pas.sem.pas_pend_sub_high >= g_lower.pas.sem.pas_pend_sub_low && g_lower.pas.sem.pas_pend_sub_low >= 0) ? 2 : 1; *sz = esz; return 1; }
    b = pas_rk_base(tn);
    if (pas_is_realtypename(b)) { if (pas_sizeof_lookup(b, &s) && (s == 4 || s == 8)) { *cls = 3; *sz = s; return 1; } return 0; }
    if (pas_is_settype(b)) { if (pas_sizeof_lookup(b, &s) && s >= 1 && s <= 32) { *cls = 6; *sz = s; return 1; } return 0; }
    if (pas_is_int_typename(b) || pas_subtype_high(b) >= 0 || pas_enumtype_high(b) >= 0) {
        if (!pas_sizeof_lookup(b, &s) || s < 1 || s > 8) return 0;
        *cls = (pas_ordinal_bound_lookup(b, &lo, &hi) && lo >= 0) || pas_enumtype_high(b) >= 0 ? 2 : 1;
        *sz = s;
        return 1;
    }
    return 0;
}
static long long pas_field_rk(int nf) {
    const char *tn = g_lower.pas.sem.pas_pend_typename;
    int cls = 0, ecls = 0;
    long long sz = 0, count = 0;
    if (g_lower.pas.sem.pas_pend_fldfile[nf] || g_lower.pas.sem.pas_pend_fldrec[nf] || g_lower.pas.sem.pas_pend_fldptrto[nf]) return 0;
    if (g_lower.pas.sem.pas_pend_fldca[nf] || g_lower.pas.sem.pas_pend_fldna[nf]) {
        long long lo = g_lower.pas.sem.pas_pend_fldca[nf] ? g_lower.pas.sem.pas_pend_fldca_lo[nf] : g_lower.pas.sem.pas_pend_fldna_lo[nf],
            hi = g_lower.pas.sem.pas_pend_fldca[nf] ? g_lower.pas.sem.pas_pend_fldca_hi[nf] : g_lower.pas.sem.pas_pend_fldna_hi[nf];
        if (hi < lo || hi - lo + 1 > 60000) return 0;
        count = hi - lo + 1;
        if (g_lower.pas.sem.pas_pend_fldca[nf]) { ecls = 4; sz = 1; } else if (!pas_rk_scalar(tn, 0, 0, 0, &ecls, &sz) || ecls == 6 || ecls == 3) return 0;
        return 7 | (sz << 4) | ((long long)ecls << 12) | (count << 16);
    }
    if (!pas_rk_scalar(tn, g_lower.pas.sem.pas_pend_ischar, g_lower.pas.sem.pas_pend_isbool, g_lower.pas.sem.pas_pend_esz, &cls, &sz)) return 0;
    return cls | (sz << 4);
}
static long long pas_rk_get(long long ov, int w) { return w == 0 ? (ov >> 16) & 0xF : w == 1 ? (ov >> 20) & 0xFF : w == 2 ? (ov >> 28) & 0xF : w == 3 ? (ov >> 32) & 0xFFFF : (ov >> 48) & 0xFF; }
static int pas_arm_field(int first, int last, int arm, int j) {
    int k = 0;
    for (int i = first; i < last; i++) if (pas_rk_get(g_lower.pas.sem.pas_pend_fldov[i], 4) == arm) { if (k++ == j) return i; }
    return -1;
}
static int pas_vcase_region_do(int d) {
    if (d != 0 || d >= 32) return 0;
    int first = g_lower.pas.sem.pas_vcase_stk[d].first, last = g_lower.pas.sem.pas_vcase_stk[d].last, narms = g_lower.pas.sem.pas_vcase_stk[d].n;
    if (narms < 2 || last <= first || last > g_lower.pas.sem.pas_pend_nf || g_lower.pas.sem.pas_vcase_stk[d].nested) return 0;
    for (int i = first; i < last; i++) if (!pas_rk_get(g_lower.pas.sem.pas_pend_fldov[i], 0)) return 0;
    int legacy = 1;
    for (int a = 2; a <= narms && legacy; a++) {
        for (int j = 0; ; j++) {
            int r = pas_arm_field(first, last, 1, j), f = pas_arm_field(first, last, a, j);
            if (r < 0 && f < 0) break;
            if (r < 0 || f < 0) { legacy = 0; break; }
            long long cr = pas_rk_get(g_lower.pas.sem.pas_pend_fldov[r], 0), cf = pas_rk_get(g_lower.pas.sem.pas_pend_fldov[f], 0);
            if (cr != cf || pas_rk_get(g_lower.pas.sem.pas_pend_fldov[r], 1) != pas_rk_get(g_lower.pas.sem.pas_pend_fldov[f], 1) || !(cr == 1 || cr == 2 || cr == 4 || cr == 5)) { legacy = 0; break; }
        }
    }
    if (legacy) return 0;
    long long rsize = 0;
    for (int a = 1; a <= narms; a++) {
        long long off = 0;
        for (int j = 0; ; j++) {
            int i = pas_arm_field(first, last, a, j);
            if (i < 0) break;
            long long cls = pas_rk_get(g_lower.pas.sem.pas_pend_fldov[i], 0), es = pas_rk_get(g_lower.pas.sem.pas_pend_fldov[i], 1), cnt = pas_rk_get(g_lower.pas.sem.pas_pend_fldov[i], 3),
                sz = cls == 7 ? es * cnt : es, al = es < 1 ? 1 : (es > 8 ? 8 : es);
            off = (off + al - 1) / al * al;
            g_lower.pas.sem.pas_pend_fldov[i] = (g_lower.pas.sem.pas_pend_fldov[i] & ~(0xFFFFLL << 32)) | (off << 32);
            off += sz;
        }
        if (off > rsize) rsize = off;
    }
    if (rsize < 1 || rsize > 60000) return 0;
    for (int i = first; i < last; i++) g_lower.pas.sem.pas_pend_fldov[i] =
        (g_lower.pas.sem.pas_pend_fldov[i] & 0xFFFF0000LL) | (g_lower.pas.sem.pas_pend_fldov[i] & 0xFFFF00000000LL) | (long long)first | (rsize << 48);
    return 1;
}
static void pas_vcase_region(int d) {
    if (d < 0 || d >= 32) return;
    int first = g_lower.pas.sem.pas_vcase_stk[d].first, last = g_lower.pas.sem.pas_vcase_stk[d].last;
    if (!pas_vcase_region_do(d)) for (int i = first; i < last && i < PAS_FIELD_MAX; i++) g_lower.pas.sem.pas_pend_fldov[i] &= 0xFFFFFFFFLL;
}
static void pas_vcase_pop(void) {
    if (g_lower.pas.sem.pas_vcase_depth > 0 && g_lower.pas.sem.pas_vcase_depth <= 32) pas_vcase_region(g_lower.pas.sem.pas_vcase_depth - 1);
    if (g_lower.pas.sem.pas_vcase_depth > 0) g_lower.pas.sem.pas_vcase_depth--;
}
static int pas_field_overlayable(int a, int b) {
    if (g_lower.pas.sem.pas_pend_fldrec[a] || g_lower.pas.sem.pas_pend_fldrec[b] || g_lower.pas.sem.pas_pend_fldca[a] || g_lower.pas.sem.pas_pend_fldca[b] || g_lower.pas.sem.pas_pend_fldna[a] ||
        g_lower.pas.sem.pas_pend_fldna[b] || g_lower.pas.sem.pas_pend_fldfile[a] || g_lower.pas.sem.pas_pend_fldfile[b] || g_lower.pas.sem.pas_pend_fldptrto[a] ||
        g_lower.pas.sem.pas_pend_fldptrto[b] || (g_lower.pas.sem.pas_pend_fldchar[a] == 1) != (g_lower.pas.sem.pas_pend_fldchar[b] == 1)) return 0;
    const char *ta = g_lower.pas.sem.pas_pend_fldtypename[a], *tb = g_lower.pas.sem.pas_pend_fldtypename[b];
    if ((ta && (pas_is_realtypename(ta) || pas_is_settype(ta) || !strcmp(ta, "string"))) || (tb && (pas_is_realtypename(tb) || pas_is_settype(tb) || !strcmp(tb, "string")))) return 0;
    return 1;
}
static void pas_vcase_arm_end(int start) {
    if (g_lower.pas.sem.pas_vcase_depth <= 0 || g_lower.pas.sem.pas_vcase_depth > 32) return;
    int end = g_lower.pas.sem.pas_pend_nf;
    int d = g_lower.pas.sem.pas_vcase_depth - 1;
    for (int i = start; i < end && i < PAS_FIELD_MAX; i++) g_lower.pas.sem.pas_pend_fldov[i] =
        (g_lower.pas.sem.pas_pend_fldov[i] & ~(0xFFLL << 48)) | ((long long)(g_lower.pas.sem.pas_vcase_stk[d].n + 1) << 48);
    if (g_lower.pas.sem.pas_vcase_stk[d].n == 0) g_lower.pas.sem.pas_vcase_stk[d].first = start;
    g_lower.pas.sem.pas_vcase_stk[d].last = end;
    if (g_lower.pas.sem.pas_vcase_stk[d].n++ == 0) { g_lower.pas.sem.pas_vcase_stk[d].lo = start; g_lower.pas.sem.pas_vcase_stk[d].hi = end; return; }
    for (int j = 0; start + j < end && g_lower.pas.sem.pas_vcase_stk[d].lo + j < g_lower.pas.sem.pas_vcase_stk[d].hi; j++) {
        int a = start + j;
        int c = (int)(g_lower.pas.sem.pas_pend_fldov[g_lower.pas.sem.pas_vcase_stk[d].lo + j] & 0xFFFF);
        if (c != a && pas_field_overlayable(a, c)) g_lower.pas.sem.pas_pend_fldov[a] = (g_lower.pas.sem.pas_pend_fldov[a] & ~0xFFFFLL) | (long long)c;
    }
}
static void pas_pend_add(const char *f) {
    if (f && g_lower.pas.sem.pas_pend_nf >= PAS_FIELD_MAX) {
        if (!g_lower.pas.sem.said) {
            g_lower.pas.sem.said = 1;
            fprintf(stderr, "pascal: record has more than %d fields; field '%s' and any after it are not resolvable -- raise PAS_FIELD_MAX\n", (int)PAS_FIELD_MAX, f);
        }
    }
    if (g_lower.pas.sem.pas_pend_nf < PAS_FIELD_MAX && f) {
        g_lower.pas.sem.pas_pend_fldptrto[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_pend_ptrtarget;
        g_lower.pas.sem.pas_pend_fldenum[g_lower.pas.sem.pas_pend_nf] = (g_lower.pas.sem.pas_pend_typename && pas_enumnames_idx(g_lower.pas.sem.pas_pend_typename) >= 0) ?
            g_lower.pas.sem.pas_pend_typename : NULL;
        g_lower.pas.sem.pas_pend_fldrec[g_lower.pas.sem.pas_pend_nf] = (g_lower.pas.sem.pas_pend_typename && pas_rectype_nf(g_lower.pas.sem.pas_pend_typename) > 0) ?
            g_lower.pas.sem.pas_pend_typename : NULL;
        g_lower.pas.sem.pas_pend_fldtypename[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_pend_typename;
        {
            int _ica = g_lower.pas.sem.pas_pend_arr_ischar || (g_lower.pas.sem.pas_pend_typename && pas_arrtype_ischar(g_lower.pas.sem.pas_pend_typename));
            long long _lo = g_lower.pas.sem.pas_pend_arr_ischar ? (g_lower.pas.sem.pas_pend_sub_low > 0 ? g_lower.pas.sem.pas_pend_sub_low : 0) :
                (g_lower.pas.sem.pas_pend_typename ? pas_arrtype_lo(g_lower.pas.sem.pas_pend_typename) : 0);
            long long _hi = g_lower.pas.sem.pas_pend_arr_ischar ? g_lower.pas.sem.pas_pend_sub_high : (g_lower.pas.sem.pas_pend_typename ? pas_arrtype_high(g_lower.pas.sem.pas_pend_typename) : -1);
            g_lower.pas.sem.pas_pend_fldca[g_lower.pas.sem.pas_pend_nf] = _ica;
            g_lower.pas.sem.pas_pend_fldca_lo[g_lower.pas.sem.pas_pend_nf] = _lo;
            g_lower.pas.sem.pas_pend_fldca_hi[g_lower.pas.sem.pas_pend_nf] = _hi;
            int _iar = g_lower.pas.sem.pas_pend_isarr || (g_lower.pas.sem.pas_pend_typename && pas_arrtype_high(g_lower.pas.sem.pas_pend_typename) >= 0);
            long long _nlo = g_lower.pas.sem.pas_pend_isarr ? g_lower.pas.sem.pas_pend_sub_low : (g_lower.pas.sem.pas_pend_typename ? pas_arrtype_lo(g_lower.pas.sem.pas_pend_typename) : 0);
            long long _nhi = g_lower.pas.sem.pas_pend_isarr ? g_lower.pas.sem.pas_pend_sub_high : (g_lower.pas.sem.pas_pend_typename ? pas_arrtype_high(g_lower.pas.sem.pas_pend_typename) : -1);
            g_lower.pas.sem.pas_pend_fldna[g_lower.pas.sem.pas_pend_nf] = (_iar && !_ica) ? 1 : 0;
            g_lower.pas.sem.pas_pend_fldna_lo[g_lower.pas.sem.pas_pend_nf] = _nlo;
            g_lower.pas.sem.pas_pend_fldna_hi[g_lower.pas.sem.pas_pend_nf] = _nhi;
        }
        g_lower.pas.sem.pas_pend_fldchar[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_pend_ischar ? 1 :
            ((g_lower.pas.sem.pas_pend_isbool && !g_lower.pas.sem.pas_pend_isarr && !g_lower.pas.sem.pas_pend_ptrtarget) ? 2 : 0);
        g_lower.pas.sem.pas_pend_fldfile[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_pend_istfile ||
            (g_lower.pas.sem.pas_pend_typename && (!strcmp(g_lower.pas.sem.pas_pend_typename, "text") || pas_rectype_has_file(g_lower.pas.sem.pas_pend_typename)));
        g_lower.pas.sem.pas_pend_fldov[g_lower.pas.sem.pas_pend_nf] = (long long)g_lower.pas.sem.pas_pend_nf | (pas_field_rk(g_lower.pas.sem.pas_pend_nf) << 16);
        g_lower.pas.sem.pas_pend_fields[g_lower.pas.sem.pas_pend_nf++] = ct_strdup(f);
        g_lower.pas.sem.pas_pend_ptrtarget = NULL;
        g_lower.pas.sem.pas_pend_arr_ischar = 0;
        g_lower.pas.sem.pas_pend_ischar = 0;
        g_lower.pas.sem.pas_pend_isarr = 0;
        g_lower.pas.sem.pas_pend_istfile = 0;
    }
}
static void pas_rectype_add(const char *tn, int packed) {
    pas_vt_bind(tn);
    if (g_lower.pas.sem.pas_nrectype >= PAS_REC_MAX || !tn) return;
    int k = g_lower.pas.sem.pas_nrectype++;
    g_lower.pas.sem.pas_rectypes[k].tname = ct_strdup(tn);
    g_lower.pas.sem.pas_rectypes[k].nf = g_lower.pas.sem.pas_pend_nf;
    g_lower.pas.sem.pas_rectypes[k].packed = packed;
    g_lower.pas.sem.pas_rectypes[k].align_mac68k = g_lower.pas.sem.pas_align_mac68k;
    g_lower.pas.sem.pas_rectypes[k].pack_n = g_lower.pas.sem.pas_pack_records;
    for (int i = 0; i < g_lower.pas.sem.pas_pend_nf; i++) {
        g_lower.pas.sem.pas_rectypes[k].fldna[i] = g_lower.pas.sem.pas_pend_fldna[i];
        g_lower.pas.sem.pas_rectypes[k].fldna_lo[i] = g_lower.pas.sem.pas_pend_fldna_lo[i];
        g_lower.pas.sem.pas_rectypes[k].fldna_hi[i] = g_lower.pas.sem.pas_pend_fldna_hi[i];
        g_lower.pas.sem.pas_rectypes[k].fields[i] = g_lower.pas.sem.pas_pend_fields[i];
        g_lower.pas.sem.pas_rectypes[k].fldptrto[i] = g_lower.pas.sem.pas_pend_fldptrto[i];
        g_lower.pas.sem.pas_rectypes[k].fldenum[i] = g_lower.pas.sem.pas_pend_fldenum[i];
        g_lower.pas.sem.pas_rectypes[k].fldrec[i] = g_lower.pas.sem.pas_pend_fldrec[i];
        g_lower.pas.sem.pas_rectypes[k].fldtypename[i] = g_lower.pas.sem.pas_pend_fldtypename[i];
        g_lower.pas.sem.pas_rectypes[k].fldca[i] = g_lower.pas.sem.pas_pend_fldca[i];
        g_lower.pas.sem.pas_rectypes[k].fldca_lo[i] = g_lower.pas.sem.pas_pend_fldca_lo[i];
        g_lower.pas.sem.pas_rectypes[k].fldca_hi[i] = g_lower.pas.sem.pas_pend_fldca_hi[i];
        g_lower.pas.sem.pas_rectypes[k].fldchar[i] = g_lower.pas.sem.pas_pend_fldchar[i];
        g_lower.pas.sem.pas_rectypes[k].fldfile[i] = g_lower.pas.sem.pas_pend_fldfile[i];
        g_lower.pas.sem.pas_rectypes[k].fldov[i] = g_lower.pas.sem.pas_pend_fldov[i];
    }
}
static int pas_rectype_to_pend(const char *tn) {
    if (!tn) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, tn)) {
        pas_pend_reset();
        for (int j = 0; j < g_lower.pas.sem.pas_rectypes[i].nf; j++) {
            g_lower.pas.sem.pas_pend_fldptrto[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldptrto[j];
            g_lower.pas.sem.pas_pend_fldenum[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldenum[j];
            g_lower.pas.sem.pas_pend_fldrec[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldrec[j];
            g_lower.pas.sem.pas_pend_fldtypename[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldtypename[j];
            g_lower.pas.sem.pas_pend_fldca[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldca[j];
            g_lower.pas.sem.pas_pend_fldca_lo[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldca_lo[j];
            g_lower.pas.sem.pas_pend_fldca_hi[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldca_hi[j];
            g_lower.pas.sem.pas_pend_fldchar[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldchar[j];
            g_lower.pas.sem.pas_pend_fldfile[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldfile[j];
            g_lower.pas.sem.pas_pend_fldov[g_lower.pas.sem.pas_pend_nf] = g_lower.pas.sem.pas_rectypes[i].fldov[j];
            g_lower.pas.sem.pas_pend_fields[g_lower.pas.sem.pas_pend_nf++] = g_lower.pas.sem.pas_rectypes[i].fields[j];
        }
        return 1;
    }
    return 0;
}
static int pas_rectype_field_index(const char *rn, const char *fn) {
    if (!rn || !fn) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        for (int j = 0; j < g_lower.pas.sem.pas_rectypes[i].nf; j++) if (g_lower.pas.sem.pas_rectypes[i].fields[j] && !strcmp(g_lower.pas.sem.pas_rectypes[i].fields[j], fn)) return j;
        return -1;
    }
    return -1;
}
static int pas_rectype_has_file(const char *rn) {
    if (!rn) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        for (int j = 0; j < g_lower.pas.sem.pas_rectypes[i].nf; j++) if (g_lower.pas.sem.pas_rectypes[i].fldfile[j]) return 1;
        return 0;
    }
    return 0;
}
static int pas_rectype_nf(const char *rn) {
    if (!rn) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) return g_lower.pas.sem.pas_rectypes[i].nf;
    return 0;
}
static int pas_rectype_field_is_ca(const char *rn, long idx) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        if (idx < g_lower.pas.sem.pas_rectypes[i].nf) return g_lower.pas.sem.pas_rectypes[i].fldca[idx];
        return 0;
    }
    return 0;
}
static int pas_rectype_field_is_char(const char *rn, long idx) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        if (idx < g_lower.pas.sem.pas_rectypes[i].nf) return g_lower.pas.sem.pas_rectypes[i].fldchar[idx] == 1;
        return 0;
    }
    return 0;
}
static const char *pas_rectype_field_typename(const char *rn, long idx) {
    if (!rn || idx < 0) return NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) return idx < g_lower.pas.sem.pas_rectypes[i]
        .nf ? g_lower.pas.sem.pas_rectypes[i].fldtypename[idx] : NULL;
    return NULL;
}
static int pas_rectype_field_is_bool(const char *rn, long idx) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        if (idx < g_lower.pas.sem.pas_rectypes[i].nf) return g_lower.pas.sem.pas_rectypes[i].fldchar[idx] == 2;
        return 0;
    }
    return 0;
}
static int pas_rectype_field_is_na(const char *rn, long idx) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn))
        return (idx < g_lower.pas.sem.pas_rectypes[i].nf) ? g_lower.pas.sem.pas_rectypes[i].fldna[idx] : 0;
    return 0;
}
static long long pas_rectype_field_na_lo(const char *rn, long idx) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn))
        return (idx < g_lower.pas.sem.pas_rectypes[i].nf) ? g_lower.pas.sem.pas_rectypes[i].fldna_lo[idx] : 0;
    return 0;
}
static long long pas_rectype_field_ca_lo(const char *rn, long idx) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        if (idx < g_lower.pas.sem.pas_rectypes[i].nf) return g_lower.pas.sem.pas_rectypes[i].fldca_lo[idx];
        return 0;
    }
    return 0;
}
static long long pas_rectype_field_ca_hi(const char *rn, long idx) {
    if (!rn || idx < 0) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        if (idx < g_lower.pas.sem.pas_rectypes[i].nf) return g_lower.pas.sem.pas_rectypes[i].fldca_hi[idx];
        return -1;
    }
    return -1;
}
static const char *pas_rectype_field_ptrto_by_index(const char *rn, long idx) {
    if (!rn || idx < 0) return NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        if (idx < g_lower.pas.sem.pas_rectypes[i].nf) return g_lower.pas.sem.pas_rectypes[i].fldptrto[idx];
        return NULL;
    }
    return NULL;
}
static int pas_rectype_layout(const char *rn, long long *size_out, long long *align_out);
static int pas_rectype_field_size_align(const char *rn, int idx, long long *sz, long long *al) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) {
        if (!g_lower.pas.sem.pas_rectypes[i].tname || strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) continue;
        if (idx >= g_lower.pas.sem.pas_rectypes[i].nf) return 0;
        long long s, nat;
        if (g_lower.pas.sem.pas_rectypes[i].fldptrto[idx]) {
            s = 8;
            nat = 8;
        } else if (g_lower.pas.sem.pas_rectypes[i].fldrec[idx]) {
            if (!pas_rectype_layout(g_lower.pas.sem.pas_rectypes[i].fldrec[idx], &s, &nat)) return 0;
        } else if (g_lower.pas.sem.pas_rectypes[i].fldca[idx]) {
            long long cnt = g_lower.pas.sem.pas_rectypes[i].fldca_hi[idx] - g_lower.pas.sem.pas_rectypes[i].fldca_lo[idx] + 1;
            s = cnt > 0 ? cnt : 1;
            nat = 1;
        } else if (g_lower.pas.sem.pas_rectypes[i].fldna[idx]) {
            long long cnt = g_lower.pas.sem.pas_rectypes[i].fldna_hi[idx] - g_lower.pas.sem.pas_rectypes[i].fldna_lo[idx] + 1;
            if (cnt < 1) cnt = 1;
            long long esz = 4;
            if (g_lower.pas.sem.pas_rectypes[i].fldtypename[idx]) pas_sizeof_lookup(g_lower.pas.sem.pas_rectypes[i].fldtypename[idx], &esz);
            s = cnt * esz;
            nat = (esz < 1) ? 1 : (esz > 8 ? 8 : esz);
        } else if (g_lower.pas.sem.pas_rectypes[i].fldenum[idx]) {
            long long ms;
            if (!pas_enumtype_min_size(g_lower.pas.sem.pas_rectypes[i].fldenum[idx], &ms)) ms = 4;
            s = ms;
            nat = (ms < 1) ? 1 : (ms > 8 ? 8 : ms);
        } else if (g_lower.pas.sem.pas_rectypes[i].fldchar[idx] == 1) {
            s = 1;
            nat = 1;
        } else if (g_lower.pas.sem.pas_rectypes[i].fldtypename[idx]) {
            if (!pas_sizeof_lookup(g_lower.pas.sem.pas_rectypes[i].fldtypename[idx], &s)) return 0;
            nat = (s < 1) ? 1 : (s > 8 ? 8 : s);
        } else return 0;
        *sz = s;
        if (g_lower.pas.sem.pas_rectypes[i].packed) *al = 1;
        else if (g_lower.pas.sem.pas_rectypes[i].align_mac68k) *al = (s == 1) ? 1 : 2;
        else if (g_lower.pas.sem.pas_rectypes[i].pack_n > 0 && nat > g_lower.pas.sem.pas_rectypes[i].pack_n) *al = g_lower.pas.sem.pas_rectypes[i].pack_n;
        else *al = nat;
        return 1;
    }
    return 0;
}
static int pas_rectype_layout(const char *rn, long long *size_out, long long *align_out) {
    if (!rn) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) {
        if (!g_lower.pas.sem.pas_rectypes[i].tname || strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) continue;
        long long off = 0, maxal = 1;
        for (int j = 0; j < g_lower.pas.sem.pas_rectypes[i].nf; j++) {
            long long sz, al;
            if (!pas_rectype_field_size_align(rn, j, &sz, &al)) return 0;
            if (al > maxal) maxal = al;
            if (al > 1) off = ((off + al - 1) / al) * al;
            off += sz;
        }
        long long total;
        if (g_lower.pas.sem.pas_rectypes[i].packed) total = off;
        else if (g_lower.pas.sem.pas_rectypes[i].align_mac68k) total = ((off + 1) / 2) * 2;
        else total = ((off + maxal - 1) / maxal) * maxal;
        if (size_out) *size_out = total;
        if (align_out) *align_out = maxal;
        return 1;
    }
    return 0;
}
static int pas_rectype_total_size(const char *rn, long long *out) { return pas_rectype_layout(rn, out, NULL); }
static int pas_rectype_field_offset(const char *rn, int idx, long long *out) {
    if (!rn || idx < 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) {
        if (!g_lower.pas.sem.pas_rectypes[i].tname || strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) continue;
        if (idx >= g_lower.pas.sem.pas_rectypes[i].nf) return 0;
        long long off = 0;
        for (int j = 0; j <= idx; j++) {
            long long sz, al;
            if (!pas_rectype_field_size_align(rn, j, &sz, &al)) return 0;
            if (al > 1) off = ((off + al - 1) / al) * al;
            if (j == idx) { *out = off; return 1; }
            off += sz;
        }
        return 0;
    }
    return 0;
}
static const char *pas_rectype_field_enum_by_index(const char *rn, long idx) {
    if (!rn || idx < 0) return NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        if (idx < g_lower.pas.sem.pas_rectypes[i].nf) return g_lower.pas.sem.pas_rectypes[i].fldenum[idx];
        return NULL;
    }
    return NULL;
}
static const char *pas_rectype_field_rectype_by_index(const char *rn, long idx) {
    if (!rn || idx < 0) return NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        if (idx < g_lower.pas.sem.pas_rectypes[i].nf) return g_lower.pas.sem.pas_rectypes[i].fldrec[idx];
        return NULL;
    }
    return NULL;
}
static void pas_recvar_add(const char *vn, int packed) {
    pas_vt_bind(vn);
    if (g_lower.pas.sem.pas_nrecvar >= PAS_REC_MAX || !vn || g_lower.pas.sem.pas_pend_nf == 0) return;
    int k = g_lower.pas.sem.pas_nrecvar++;
    g_lower.pas.sem.pas_recvars[k].vname = ct_strdup(vn);
    g_lower.pas.sem.pas_recvars[k].nf = g_lower.pas.sem.pas_pend_nf;
    g_lower.pas.sem.pas_recvars[k].packed = packed;
    for (int i = 0; i < g_lower.pas.sem.pas_pend_nf; i++) {
        g_lower.pas.sem.pas_recvars[k].fields[i] = g_lower.pas.sem.pas_pend_fields[i];
        g_lower.pas.sem.pas_recvars[k].fldchar[i] = g_lower.pas.sem.pas_pend_fldchar[i];
        g_lower.pas.sem.pas_recvars[k].fldrec[i] = g_lower.pas.sem.pas_pend_fldrec[i];
        g_lower.pas.sem.pas_recvars[k].fldna[i] = g_lower.pas.sem.pas_pend_fldna[i];
        g_lower.pas.sem.pas_recvars[k].fldna_lo[i] = g_lower.pas.sem.pas_pend_fldna_lo[i];
        g_lower.pas.sem.pas_recvars[k].fldna_hi[i] = g_lower.pas.sem.pas_pend_fldna_hi[i];
        g_lower.pas.sem.pas_recvars[k].fldov[i] = g_lower.pas.sem.pas_pend_fldov[i];
    }
}
static void pas_recvar_add_from_type(const char *vn, const char *tn) {
    if (!vn || !tn) return;
    for (int ri = 0; ri < g_lower.pas.sem.pas_nrectype; ri++) {
        if (!g_lower.pas.sem.pas_rectypes[ri].tname || strcmp(g_lower.pas.sem.pas_rectypes[ri].tname, tn)) continue;
        if (g_lower.pas.sem.pas_nrecvar >= PAS_REC_MAX) return;
        int k = g_lower.pas.sem.pas_nrecvar++;
        g_lower.pas.sem.pas_recvars[k].vname = ct_strdup(vn);
        g_lower.pas.sem.pas_recvars[k].nf = g_lower.pas.sem.pas_rectypes[ri].nf;
        g_lower.pas.sem.pas_recvars[k].packed = g_lower.pas.sem.pas_rectypes[ri].packed;
        for (int j = 0; j < g_lower.pas.sem.pas_rectypes[ri].nf; j++) {
            g_lower.pas.sem.pas_recvars[k].fields[j] = g_lower.pas.sem.pas_rectypes[ri].fields[j];
            g_lower.pas.sem.pas_recvars[k].fldchar[j] = g_lower.pas.sem.pas_rectypes[ri].fldchar[j];
            g_lower.pas.sem.pas_recvars[k].fldrec[j] = g_lower.pas.sem.pas_rectypes[ri].fldrec[j];
            g_lower.pas.sem.pas_recvars[k].fldna[j] = g_lower.pas.sem.pas_rectypes[ri].fldna[j];
            g_lower.pas.sem.pas_recvars[k].fldna_lo[j] = g_lower.pas.sem.pas_rectypes[ri].fldna_lo[j];
            g_lower.pas.sem.pas_recvars[k].fldna_hi[j] = g_lower.pas.sem.pas_rectypes[ri].fldna_hi[j];
            g_lower.pas.sem.pas_recvars[k].fldov[j] = g_lower.pas.sem.pas_rectypes[ri].fldov[j];
        }
        return;
    }
}
static int pas_recvar_field_is_char(const char *vn, long idx) {
    if (!vn || idx < 0) return 0;
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, vn))
        return (idx < g_lower.pas.sem.pas_recvars[i].nf) ? g_lower.pas.sem.pas_recvars[i].fldchar[idx] == 1 : 0;
    return 0;
}
static int pas_recvar_field_is_bool(const char *vn, long idx) {
    if (!vn || idx < 0) return 0;
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, vn))
        return (idx < g_lower.pas.sem.pas_recvars[i].nf) ? g_lower.pas.sem.pas_recvars[i].fldchar[idx] == 2 : 0;
    return 0;
}
static int pas_recvar_field_index(const char *vn, const char *fn) {
    if (!vn || !fn) return -1;
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, vn)) {
        for (int j = 0; j < g_lower.pas.sem.pas_recvars[i].nf; j++) if (g_lower.pas.sem.pas_recvars[i].fields[j] && !strcmp(g_lower.pas.sem.pas_recvars[i].fields[j], fn)) return j;
        return -1;
    }
    return -1;
}
static int pas_rectype_slot(const char *rn, int fi) {
    if (!rn || fi < 0 || fi >= PAS_FIELD_MAX) return fi;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) return fi < g_lower.pas.sem.pas_rectypes[i]
        .nf ? (int)(g_lower.pas.sem.pas_rectypes[i].fldov[fi] & 0xFFFF) : fi;
    return fi;
}
static int pas_recvar_slot(const char *vn, int fi) {
    if (!vn || fi < 0 || fi >= PAS_FIELD_MAX) return fi;
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, vn)) return fi < g_lower.pas.sem.pas_recvars[i]
        .nf ? (int)(g_lower.pas.sem.pas_recvars[i].fldov[fi] & 0xFFFF) : fi;
    return fi;
}
static const char *pas_ptrexpr_target(tree_t *e);
static const char *pas_selector_rectype(tree_t *e) {
    if (!e) return NULL;
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return pas_ptrexpr_target(e->c[1]);
    return pas_elem_rectype(e);
}
static const char *pas_ptrexpr_target(tree_t *e) {
    if (!e) return NULL;
    if (e->t == TT_VAR && e->v.sval) return pas_ptrvar_target(e->v.sval);
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) { const char *at = pas_arrptr_target(e->c[0]->v.sval); if (at) return at; }
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) {
        const char *rt = pas_with_sel_rtype(e->c[0]);
        if (rt) return pas_rectype_field_ptrto_by_index(rt, e->c[1]->v.ival);
    }
    return NULL;
}
static void pas_proc_enter(void) {
    if (g_lower.pas.sem.pas_ldepth < PAS_NEST_MAX) {
        g_lower.pas.sem.pas_lstk[g_lower.pas.sem.pas_ldepth].n = 0;
        g_lower.pas.sem.pas_lstk[g_lower.pas.sem.pas_ldepth].decl_level = g_lower.pas.sem.pas_level;
        g_lower.pas.sem.pas_lstk[g_lower.pas.sem.pas_ldepth].func_name = NULL;
    }
    g_lower.pas.sem.pas_ldepth++;
    g_lower.pas.sem.pas_level++;
}
static void pas_proc_exit(void) { if (g_lower.pas.sem.pas_ldepth > 0) g_lower.pas.sem.pas_ldepth--; if (g_lower.pas.sem.pas_level > 1) g_lower.pas.sem.pas_level--; }
static void pas_local_add(const char *name) {
    if (g_lower.pas.sem.pas_level < 2 || g_lower.pas.sem.pas_ldepth == 0 || g_lower.pas.sem.pas_ldepth > PAS_NEST_MAX || !name) return;
    int d = g_lower.pas.sem.pas_ldepth - 1;
    if (g_lower.pas.sem.pas_lstk[d].n < PAS_LOCAL_MAX) g_lower.pas.sem.pas_lstk[d].names[g_lower.pas.sem.pas_lstk[d].n++] = ct_strdup(name);
}
static void pas_curfunc_push(const char *fname) {
    if (g_lower.pas.sem.pas_ldepth > 0 && g_lower.pas.sem.pas_ldepth <= PAS_NEST_MAX) g_lower.pas.sem.pas_lstk[g_lower.pas.sem.pas_ldepth - 1].func_name = fname;
}
static const char *pas_curfunc_name(void) {
    return (g_lower.pas.sem.pas_ldepth > 0 && g_lower.pas.sem.pas_ldepth <= PAS_NEST_MAX) ? g_lower.pas.sem.pas_lstk[g_lower.pas.sem.pas_ldepth - 1].func_name : NULL;
}
static int pas_is_declared_local_here(const char *name) {
    if (!name || g_lower.pas.sem.pas_ldepth <= 0) return 0;
    int d = g_lower.pas.sem.pas_ldepth - 1;
    for (int i = 0; i < g_lower.pas.sem.pas_lstk[d].n; i++) if (g_lower.pas.sem.pas_lstk[d].names[i] && !strcmp(g_lower.pas.sem.pas_lstk[d].names[i], name)) return 1;
    return 0;
}
static void pas_settype_add(const char *name, long long hi) {
    if (g_lower.pas.sem.pas_nsettype < 64 && name) {
        g_lower.pas.sem.pas_settypes[g_lower.pas.sem.pas_nsettype].name = ct_strdup(name);
        g_lower.pas.sem.pas_settypes[g_lower.pas.sem.pas_nsettype].hi = hi;
        g_lower.pas.sem.pas_settypes[g_lower.pas.sem.pas_nsettype].pack = g_lower.pas.sem.pas_pack_set_size;
        g_lower.pas.sem.pas_nsettype++;
    }
}
static int pas_is_settype(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nsettype; i++) if (g_lower.pas.sem.pas_settypes[i].name && !strcmp(g_lower.pas.sem.pas_settypes[i].name, name)) return 1;
    return 0;
}
static long long pas_settype_hi(const char *name) {
    if (!name) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_nsettype; i++) if (g_lower.pas.sem.pas_settypes[i].name && !strcmp(g_lower.pas.sem.pas_settypes[i].name, name)) return g_lower.pas.sem.pas_settypes[i].hi;
    return -1;
}
static long long pas_settype_pack(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nsettype; i++) if (g_lower.pas.sem.pas_settypes[i].name && !strcmp(g_lower.pas.sem.pas_settypes[i].name, name)) return g_lower.pas.sem.pas_settypes[i].pack;
    return 0;
}
static void pas_setvar_add(const char *name) {
    if (g_lower.pas.sem.pas_nsetvar < 256 && name) {
        g_lower.pas.sem.pas_setvars[g_lower.pas.sem.pas_nsetvar].name = ct_strdup(name);
        g_lower.pas.sem.pas_setvars[g_lower.pas.sem.pas_nsetvar++].depth = g_lower.pas.sem.pas_npvmark;
    }
}
static int pas_is_setvar(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nsetvar; i++) if (g_lower.pas.sem.pas_setvars[i].name && !strcmp(g_lower.pas.sem.pas_setvars[i].name, name)) return 1;
    return 0;
}
static void pas_charvar_add(const char *name) {
    if (g_lower.pas.sem.pas_ncharvar < 256 && name) {
        g_lower.pas.sem.pas_charvars[g_lower.pas.sem.pas_ncharvar].name = ct_strdup(name);
        g_lower.pas.sem.pas_charvars[g_lower.pas.sem.pas_ncharvar++].depth = g_lower.pas.sem.pas_npvmark;
    }
}
static void pas_charvar_add_outer(const char *name) {
    if (g_lower.pas.sem.pas_ncharvar < 256 && name) {
        g_lower.pas.sem.pas_charvars[g_lower.pas.sem.pas_ncharvar].name = ct_strdup(name);
        g_lower.pas.sem.pas_charvars[g_lower.pas.sem.pas_ncharvar++].depth = g_lower.pas.sem.pas_npvmark > 0 ? g_lower.pas.sem.pas_npvmark - 1 : 0;
    }
}
static void pas_singlevar_add(const char *name) {
    if (g_lower.pas.sem.pas_nsinglevar < 256 && name) {
        g_lower.pas.sem.pas_singlevars[g_lower.pas.sem.pas_nsinglevar].name = ct_strdup(name);
        g_lower.pas.sem.pas_singlevars[g_lower.pas.sem.pas_nsinglevar++].depth = g_lower.pas.sem.pas_npvmark;
    }
}
static int pas_is_singlevar(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nsinglevar; i++) if (g_lower.pas.sem.pas_singlevars[i].name && !strcmp(g_lower.pas.sem.pas_singlevars[i].name, name)) return 1;
    return 0;
}
static int pas_is_currencyvar(const char *name) {
    int u = pas_scope_uid(name);
    for (int i = g_lower.pas.sem.pas_nscalarvartype - 1; name && i >= 0; i--)
        if (g_lower.pas.sem.pas_scalarvartype[i].vname && g_lower.pas.sem.pas_scalarvartype[i].uid == u && !strcmp(g_lower.pas.sem.pas_scalarvartype[i].vname, name))
        return !strcmp(g_lower.pas.sem.pas_scalarvartype[i].tname, "currency");
    return 0;
}
static int pas_is_currencyexpr(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_VAR) return e->v.sval && pas_is_currencyvar(e->v.sval);
    if (e->t == TT_ADD || e->t == TT_SUB || e->t == TT_MUL || e->t == TT_DIV || e->t == TT_MNS) { for (int i = 0; i < e->n; i++) if (pas_is_currencyexpr(e->c[i])) return 1; }
    return 0;
}
static void pas_pcharvar_add(const char *name) {
    if (g_lower.pas.sem.pas_npcharvar < 256 && name) {
        g_lower.pas.sem.pas_pcharvars[g_lower.pas.sem.pas_npcharvar].name = ct_strdup(name);
        g_lower.pas.sem.pas_pcharvars[g_lower.pas.sem.pas_npcharvar++].depth = g_lower.pas.sem.pas_npvmark;
    }
}
static int pas_is_pcharvar(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_npcharvar; i++) if (g_lower.pas.sem.pas_pcharvars[i].name && !strcmp(g_lower.pas.sem.pas_pcharvars[i].name, name)) return 1;
    return 0;
}
static void pas_tfilevar_add(const char *name) { if (g_lower.pas.sem.pas_ntfilevar < 128 && name) g_lower.pas.sem.pas_tfilevars[g_lower.pas.sem.pas_ntfilevar++].name = ct_strdup(name); }
static void pas_tfilevar_mark(void) { if (g_lower.pas.sem.pas_ntfvmark < PAS_NEST_MAX_PV) g_lower.pas.sem.pas_tfvmarks[g_lower.pas.sem.pas_ntfvmark++] = g_lower.pas.sem.pas_ntfilevar; }
static void pas_tfilevar_release(void) { if (g_lower.pas.sem.pas_ntfvmark > 0) g_lower.pas.sem.pas_ntfilevar = g_lower.pas.sem.pas_tfvmarks[--g_lower.pas.sem.pas_ntfvmark]; }
static int pas_is_tfilevar(const char *name) {
    if (!name) return 0;
    for (int i = g_lower.pas.sem.pas_ntfilevar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_tfilevars[i].name && !strcmp(g_lower.pas.sem.pas_tfilevars[i].name, name)) return 1;
    return 0;
}
static void pas_tfiletype_add(const char *name) { if (g_lower.pas.sem.pas_ntfiletype < 64 && name) g_lower.pas.sem.pas_tfiletypes[g_lower.pas.sem.pas_ntfiletype++].name = ct_strdup(name); }
static int pas_is_tfiletype(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_ntfiletype; i++) if (g_lower.pas.sem.pas_tfiletypes[i].name && !strcmp(g_lower.pas.sem.pas_tfiletypes[i].name, name)) return 1;
    return 0;
}
static int pas_is_tfile_node(const tree_t *e) { return e && e->t == TT_VAR && e->v.sval && pas_is_tfilevar(e->v.sval); }
static void pas_boolvar_add(const char *name) {
    if (g_lower.pas.sem.pas_nboolvar < 512 && name) {
        g_lower.pas.sem.pas_boolvars[g_lower.pas.sem.pas_nboolvar].name = ct_strdup(name);
        g_lower.pas.sem.pas_boolvars[g_lower.pas.sem.pas_nboolvar++].depth = g_lower.pas.sem.pas_npvmark;
    }
}
static void pas_boolvar_add_outer(const char *name) {
    if (g_lower.pas.sem.pas_nboolvar < 512 && name) {
        g_lower.pas.sem.pas_boolvars[g_lower.pas.sem.pas_nboolvar].name = ct_strdup(name);
        g_lower.pas.sem.pas_boolvars[g_lower.pas.sem.pas_nboolvar++].depth = g_lower.pas.sem.pas_npvmark > 0 ? g_lower.pas.sem.pas_npvmark - 1 : 0;
    }
}
static int pas_is_boolvar(const char *name) {
    if (!name) return 0;
    for (int i = g_lower.pas.sem.pas_nboolvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_boolvars[i].name && !strcmp(g_lower.pas.sem.pas_boolvars[i].name, name)) return 1;
    return 0;
}
static void pas_booltype_add(const char *name) { if (g_lower.pas.sem.pas_nbooltype < 64 && name) g_lower.pas.sem.pas_booltypes[g_lower.pas.sem.pas_nbooltype++].name = ct_strdup(name); }
static int pas_is_booltype(const char *name) {
    if (!name) return 0;
    if (!strcmp(name, "boolean")) return 1;
    for (int i = 0; i < g_lower.pas.sem.pas_nbooltype; i++) if (g_lower.pas.sem.pas_booltypes[i].name && !strcmp(g_lower.pas.sem.pas_booltypes[i].name, name)) return 1;
    return 0;
}
static int pas_is_boolexpr(tree_t *e) {
    if (!e) return 0;
    if (pas_pf_rtype(e)) return pas_is_booltype(pas_pf_rtype(e));
    switch (e->t) {
        case TT_LT:
        case TT_LE:
        case TT_GT:
        case TT_GE:
        case TT_EQ:
        case TT_NE:
        case TT_NOT:
        return 1;
        case TT_CONJ:
        case TT_ALT:
        return 1;
        case TT_MUL:
        case TT_ADD:
        return e->n == 2 && pas_is_boolexpr(e->c[0]) && pas_is_boolexpr(e->c[1]);
        case TT_VAR:
        return pas_is_boolvar(e->v.sval);
        case TT_IDX:
        if (e->n < 2 || !e->c[0] || !e->c[1]) return 0;
        if (e->c[0]->t == TT_VAR && e->c[0]->v.sval) { if (pas_array_isbool(e->c[0]->v.sval)) return 1; return e->c[1]->t == TT_ILIT && pas_recvar_field_is_bool(e->c[0]->v.sval, e->c[1]->v.ival); }
        if (e->c[1]->t == TT_ILIT) { const char *rt = pas_with_sel_rtype(e->c[0]); return rt && pas_rectype_field_is_bool(rt, e->c[1]->v.ival); }
        return 0;
        case TT_FNC:
        {
            const char *fn = (e->n >= 1 && e->c[0]) ? e->c[0]->v.sval : NULL;
            if (!fn) return 0;
            if (!strcmp(fn, "__pas_deref") && e->n == 2 && pas_typename_class(pas_ptrexpr_target(e->c[1])) == 'b') return 1;
            if (!strcmp(fn, "__pas_rdecode") && e->n == 5 && e->c[3] && e->c[3]->t == TT_ILIT && (e->c[3]->v.ival & 0xFF) == 5) return 1;
            if (!strcmp(fn, "__pas_in") || !strcmp(fn, "__pas_eof") || !strcmp(fn, "__pas_eoln") || !strcmp(fn, "__pas_eof_f") || !strcmp(fn, "__pas_eoln_f") || !strcmp(fn, "__pas_feof_t") ||
                !strcmp(fn, "__pas_seteq") || !strcmp(fn, "__pas_setne") || !strcmp(fn, "__pas_subset") || !strcmp(fn, "__pas_super")) return 1;
            return pas_is_boolvar(fn);
        }
        default:
        return 0;
    }
}
static int pas_is_charvar(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_ncharvar; i++) if (g_lower.pas.sem.pas_charvars[i].name && !strcmp(g_lower.pas.sem.pas_charvars[i].name, name)) return 1;
    return 0;
}
static void pas_filevar_add(const char *name) {
    if (g_lower.pas.sem.pas_nfilevar < 256 && name) {
        for (int i = 0; i < g_lower.pas.sem.pas_nfilevar; i++) if (g_lower.pas.sem.pas_filevars[i].name && !strcmp(g_lower.pas.sem.pas_filevars[i].name, name)) return;
        g_lower.pas.sem.pas_filevars[g_lower.pas.sem.pas_nfilevar].name = ct_strdup(name);
        g_lower.pas.sem.pas_filevars[g_lower.pas.sem.pas_nfilevar++].depth = g_lower.pas.sem.pas_npvmark;
    }
}
static int pas_is_filevar(const char *name) {
    if (!name) return 0;
    if (!strcmp(name, "input") || !strcmp(name, "output")) return 1;
    for (int i = 0; i < g_lower.pas.sem.pas_nfilevar; i++) if (g_lower.pas.sem.pas_filevars[i].name && !strcmp(g_lower.pas.sem.pas_filevars[i].name, name)) return 1;
    return 0;
}
#define PAS_SCOPED_FNS(T, ARR, CNT) static void T##_release(void) { int w = 0; for (int i = 0; i < CNT; i++) if (ARR[i].depth <= g_lower.pas.sem.pas_npvmark) ARR[w++] = ARR[i]; CNT = w; } static int \
    T##_save(void **buf) { int n = 0; if (g_lower.pas.sem.pas_npvmark <= 0) return 0; for (int i = 0; i < CNT; i++) if (ARR[i].depth == g_lower.pas.sem.pas_npvmark) n++; if (!n) return 0; __typeof__ \
    (ARR[0]) *b = ct_zalloc((size_t)n, sizeof ARR[0]); int k = 0; for (int i = 0; i < CNT; i++) if (ARR[i].depth == g_lower.pas.sem.pas_npvmark) b[k++] = ARR[i]; *buf = b; return n; } static void T \
    ##_restore(void *buf, int n) { __typeof__(ARR[0]) *b = buf; for (int i = 0; i < n && CNT < (int)(sizeof ARR / sizeof ARR[0]); i++) { ARR[CNT] = b[i]; ARR[CNT++].depth = g_lower.pas.sem. \
    pas_npvmark; } }
PAS_SCOPED_FNS(pas_boolvar, g_lower.pas.sem.pas_boolvars, g_lower.pas.sem.pas_nboolvar) PAS_SCOPED_FNS(pas_setvar, g_lower.pas.sem.pas_setvars, g_lower.pas.sem.pas_nsetvar)
    PAS_SCOPED_FNS(pas_charvar, g_lower.pas.sem.pas_charvars, g_lower.pas.sem.pas_ncharvar) PAS_SCOPED_FNS(pas_singlevar, g_lower.pas.sem.pas_singlevars, g_lower.pas.sem.pas_nsinglevar)
    PAS_SCOPED_FNS(pas_pcharvar, g_lower.pas.sem.pas_pcharvars, g_lower.pas.sem.pas_npcharvar) PAS_SCOPED_FNS(pas_filevar, g_lower.pas.sem.pas_filevars, g_lower.pas.sem.pas_nfilevar)
    static void pas_scoped_release(void) {
    pas_boolvar_release();
    pas_setvar_release();
    pas_charvar_release();
    pas_singlevar_release();
    pas_pcharvar_release();
    pas_filevar_release();
}
static void pas_scoped_save(void **sv, int *svn) {
    svn[0] = pas_boolvar_save(&sv[0]);
    svn[1] = pas_setvar_save(&sv[1]);
    svn[2] = pas_charvar_save(&sv[2]);
    svn[3] = pas_singlevar_save(&sv[3]);
    svn[4] = pas_pcharvar_save(&sv[4]);
    svn[5] = pas_filevar_save(&sv[5]);
}
static void pas_scoped_restore(void **sv, int *svn) {
    pas_boolvar_restore(sv[0], svn[0]);
    pas_setvar_restore(sv[1], svn[1]);
    pas_charvar_restore(sv[2], svn[2]);
    pas_singlevar_restore(sv[3], svn[3]);
    pas_pcharvar_restore(sv[4], svn[4]);
    pas_filevar_restore(sv[5], svn[5]);
}
static int pas_is_filevar_elem(tree_t *e) {
    long long _ah;
    return e && e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_filevar(e->c[0]->v.sval) && pas_array_high_get(e->c[0]->v.sval, &_ah);
}
static int pas_is_stdstream(const char *name) { return name && (!strcmp(name, "input") || !strcmp(name, "output")); }
static int pas_is_hdrfile(const char *n) {
    if (!n) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nhdrfile; i++) if (g_lower.pas.sem.pas_hdrfiles[i] && !strcmp(g_lower.pas.sem.pas_hdrfiles[i], n)) return 1;
    return 0;
}
static void pas_chararr_add2(const char *name, long long lo) {
    if (g_lower.pas.sem.pas_nchararr < 256 && name) {
        g_lower.pas.sem.pas_chararrs[g_lower.pas.sem.pas_nchararr].name = ct_strdup(name);
        g_lower.pas.sem.pas_chararrs[g_lower.pas.sem.pas_nchararr].lo = lo;
        g_lower.pas.sem.pas_chararrs[g_lower.pas.sem.pas_nchararr].uid = pas_scope_uid(name);
        g_lower.pas.sem.pas_chararrs[g_lower.pas.sem.pas_nchararr].nostr = 0;
        g_lower.pas.sem.pas_chararrs[g_lower.pas.sem.pas_nchararr].n = -1;
        g_lower.pas.sem.pas_nchararr++;
    }
}
static void pas_chararr_add(const char *name) { pas_chararr_add2(name, 0); }
static int pas_chararr_row(const char *name) {
    if (!name) return -1;
    int u = pas_scope_uid(name);
    for (int i = g_lower.pas.sem.pas_nchararr - 1; i >= 0; i--)
        if (g_lower.pas.sem.pas_chararrs[i].name && !strcmp(g_lower.pas.sem.pas_chararrs[i].name, name) && g_lower.pas.sem.pas_chararrs[i].uid == u) return i;
    return -1;
}
static long long pas_chararr_lo(const char *name) { int i = pas_chararr_row(name); return i < 0 ? 0 : g_lower.pas.sem.pas_chararrs[i].lo; }
static int pas_is_chararr(const char *name) { return pas_chararr_row(name) >= 0; }
static void pas_chararr_shape(const char *name, int nostr, long long n) {
    int i = pas_chararr_row(name);
    if (i >= 0) { g_lower.pas.sem.pas_chararrs[i].nostr = nostr; g_lower.pas.sem.pas_chararrs[i].n = n; }
}
static int pas_arrtype_nostr(const char *name) { return pas_arrtype_ischar(name) & PAS_CA_NOSTR; }
static void pas_cafield_mark_add(tree_t *e, long long lo, long long hi) {
    if (g_lower.pas.sem.pas_ncafield < 2048 && e) {
        g_lower.pas.sem.pas_cafield_marks[g_lower.pas.sem.pas_ncafield] = e;
        g_lower.pas.sem.pas_cafield_lo[g_lower.pas.sem.pas_ncafield] = lo;
        g_lower.pas.sem.pas_cafield_hi[g_lower.pas.sem.pas_ncafield] = hi;
        g_lower.pas.sem.pas_ncafield++;
    }
}
static int pas_is_cafield(const tree_t *e) { for (int i = 0; i < g_lower.pas.sem.pas_ncafield; i++) if (g_lower.pas.sem.pas_cafield_marks[i] == (tree_t *)e) return 1; return 0; }
static long long pas_cafield_lo_get(const tree_t *e) {
    for (int i = 0; i < g_lower.pas.sem.pas_ncafield; i++) if (g_lower.pas.sem.pas_cafield_marks[i] == (tree_t *)e) return g_lower.pas.sem.pas_cafield_lo[i];
    return 0;
}
static long long pas_cafield_hi_get(const tree_t *e) {
    for (int i = 0; i < g_lower.pas.sem.pas_ncafield; i++) if (g_lower.pas.sem.pas_cafield_marks[i] == (tree_t *)e) return g_lower.pas.sem.pas_cafield_hi[i];
    return -1;
}
static void pas_nafield_mark_add(tree_t *e, long long lo) {
    if (g_lower.pas.sem.pas_nnafield < 2048 && e) {
        g_lower.pas.sem.pas_nafield_marks[g_lower.pas.sem.pas_nnafield] = e;
        g_lower.pas.sem.pas_nafield_lo[g_lower.pas.sem.pas_nnafield] = lo;
        g_lower.pas.sem.pas_nnafield++;
    }
}
static int pas_is_nafield(const tree_t *e) { for (int i = 0; i < g_lower.pas.sem.pas_nnafield; i++) if (g_lower.pas.sem.pas_nafield_marks[i] == (tree_t *)e) return 1; return 0; }
static long long pas_nafield_lo_get(const tree_t *e) {
    for (int i = 0; i < g_lower.pas.sem.pas_nnafield; i++) if (g_lower.pas.sem.pas_nafield_marks[i] == (tree_t *)e) return g_lower.pas.sem.pas_nafield_lo[i];
    return 0;
}
static void pas_cvfield_mark_add(tree_t *e) { if (g_lower.pas.sem.pas_ncvfield < 2048 && e) g_lower.pas.sem.pas_cvfield_marks[g_lower.pas.sem.pas_ncvfield++] = e; }
static int pas_is_cvfield(const tree_t *e) { for (int i = 0; i < g_lower.pas.sem.pas_ncvfield; i++) if (g_lower.pas.sem.pas_cvfield_marks[i] == (tree_t *)e) return 1; return 0; }
static int pas_ca_is_read(const tree_t *e) { return e && e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_ca_unpack"); }
static void pas_strarr_add2(const char *name, long long lo) {
    if (g_lower.pas.sem.pas_nstrarr < 128 && name) {
        g_lower.pas.sem.pas_strarrs[g_lower.pas.sem.pas_nstrarr].name = ct_strdup(name);
        g_lower.pas.sem.pas_strarrs[g_lower.pas.sem.pas_nstrarr].lo = lo;
        g_lower.pas.sem.pas_nstrarr++;
    }
}
static void pas_strarr_add(const char *name) { pas_strarr_add2(name, 1); }
static int pas_is_strarr(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nstrarr; i++) if (g_lower.pas.sem.pas_strarrs[i].name && !strcmp(g_lower.pas.sem.pas_strarrs[i].name, name)) return 1;
    return 0;
}
static long long pas_strarr_lo(const char *name) {
    if (!name) return 1;
    for (int i = 0; i < g_lower.pas.sem.pas_nstrarr; i++) if (g_lower.pas.sem.pas_strarrs[i].name && !strcmp(g_lower.pas.sem.pas_strarrs[i].name, name)) return g_lower.pas.sem.pas_strarrs[i].lo;
    return 1;
}
static void pas_arrrec_add(const char *a, const char *r, int nf) {
    if (g_lower.pas.sem.pas_narrrec < 128 && a && nf > 0) {
        g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].aname = ct_strdup(a);
        g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].rname = r ? ct_strdup(r) : NULL;
        g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].nf = nf;
        for (int _i = 0; _i < nf && _i < 32; _i++) {
            g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].fields[_i] = g_lower.pas.sem.pas_pend_fields[_i] ? ct_strdup(g_lower.pas.sem.pas_pend_fields[_i]) : NULL;
            g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].fldenum[_i] = g_lower.pas.sem.pas_pend_fldenum[_i] ? ct_strdup(g_lower.pas.sem.pas_pend_fldenum[_i]) : NULL;
            g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].fldchar[_i] = g_lower.pas.sem.pas_pend_fldchar[_i];
            g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].fldtypename[_i] = g_lower.pas.sem.pas_pend_fldtypename[_i] ? ct_strdup(g_lower.pas.sem.pas_pend_fldtypename[_i]) : NULL;
            g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].fldrec[_i] = g_lower.pas.sem.pas_pend_fldrec[_i];
            g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].fldna[_i] = g_lower.pas.sem.pas_pend_fldna[_i];
            g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].fldna_lo[_i] = g_lower.pas.sem.pas_pend_fldna_lo[_i];
            g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].fldna_hi[_i] = g_lower.pas.sem.pas_pend_fldna_hi[_i];
            g_lower.pas.sem.pas_arrrecs[g_lower.pas.sem.pas_narrrec].fldov[_i] = g_lower.pas.sem.pas_pend_fldov[_i];
        }
        g_lower.pas.sem.pas_narrrec++;
    }
}
static void pas_arrrec_add_from_type(const char *a, const char *tn) {
    if (!a || !tn) return;
    pas_pend_nest_save();
    if (pas_rectype_to_pend(tn)) pas_arrrec_add(a, tn, g_lower.pas.sem.pas_pend_nf);
    pas_pend_nest_restore();
}
static const char *pas_arrrec_field_enum(const char *a, long idx) {
    if (!a || idx < 0 || idx >= 32) return NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_narrrec; i++) if (g_lower.pas.sem.pas_arrrecs[i].aname && !strcmp(g_lower.pas.sem.pas_arrrecs[i].aname, a)) {
        if (idx < g_lower.pas.sem.pas_arrrecs[i].nf) return g_lower.pas.sem.pas_arrrecs[i].fldenum[idx];
        return NULL;
    }
    return NULL;
}
static int pas_arrrec_field_is_char(const char *a, long idx) {
    if (!a || idx < 0 || idx >= 32) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narrrec; i++) if (g_lower.pas.sem.pas_arrrecs[i].aname && !strcmp(g_lower.pas.sem.pas_arrrecs[i].aname, a)) {
        if (idx < g_lower.pas.sem.pas_arrrecs[i].nf) return g_lower.pas.sem.pas_arrrecs[i].fldchar[idx] == 1;
        return 0;
    }
    return 0;
}
static int pas_arrrec_find(const char *a, const char **rn) {
    if (!a) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narrrec; i++) if (g_lower.pas.sem.pas_arrrecs[i].aname && !strcmp(g_lower.pas.sem.pas_arrrecs[i].aname, a)) {
        if (rn) *rn = g_lower.pas.sem.pas_arrrecs[i].rname;
        return g_lower.pas.sem.pas_arrrecs[i].nf;
    }
    return 0;
}
static int pas_arrrec_field_index(const char *a, const char *fn) {
    if (!a || !fn) return -1;
    for (int i = 0; i < g_lower.pas.sem.pas_narrrec; i++) if (g_lower.pas.sem.pas_arrrecs[i].aname && !strcmp(g_lower.pas.sem.pas_arrrecs[i].aname, a)) {
        for (int j = 0; j < g_lower.pas.sem.pas_arrrecs[i].nf && j < 32; j++) if (g_lower.pas.sem.pas_arrrecs[i].fields[j] && !strcmp(g_lower.pas.sem.pas_arrrecs[i].fields[j], fn)) return j;
    }
    return -1;
}
static int pas_arrrec_slot(const char *a, long fi) {
    if (!a || fi < 0 || fi >= 32) return (int)fi;
    for (int i = 0; i < g_lower.pas.sem.pas_narrrec; i++) if (g_lower.pas.sem.pas_arrrecs[i].aname && !strcmp(g_lower.pas.sem.pas_arrrecs[i].aname, a)) return fi < g_lower.pas.sem.pas_arrrecs[i].nf ?
        (int)(g_lower.pas.sem.pas_arrrecs[i].fldov[fi] & 0xFFFF) : (int)fi;
    return (int)fi;
}
static tree_t *pas_arrrec_flatten(tree_t *idxsel, long long fi) {
    tree_t *e = ast_node_new(TT_IDX);
    ast_push(e, idxsel);
    ast_push(e, ilit((idxsel && idxsel->t == TT_IDX && idxsel->n == 2 && idxsel->c[0] && idxsel->c[0]->t == TT_VAR && idxsel->c[0]->v.sval) ? pas_arrrec_slot(idxsel->c[0]->v.sval, fi) : fi));
    e->slen = PAS_FIELD_IDX_MARK;
    return e;
}
static tree_t *mk_chr_wrap(tree_t *e) { tree_t *r = ast_node_new(TT_FNC); ast_push(r, leaf_s(TT_VAR, "__pas_chr")); ast_push(r, e); return r; }
static int pas_buf_is_char(tree_t *e) { return !(!strcmp(e->c[0]->v.sval, "__pas_fbuf_get") && e->n >= 2 && e->c[1] && e->c[1]->t == TT_VAR && pas_tfcomp_nonchar(e->c[1]->v.sval)); }
static tree_t *pas_vt_unwrap_read(tree_t *e) {
    if (e && e->t == TT_SEQ_EXPR && e->n >= 2 && e->c[0] && e->c[0]->t == TT_FNC && e->c[0]->n >= 1 && e->c[0]->c[0] && e->c[0]->c[0]->v.sval &&
        (!strcmp(e->c[0]->c[0]->v.sval, "__pas_vcheck") || !strcmp(e->c[0]->c[0]->v.sval, "__pas_tagwhole"))) return e->c[e->n - 1];
    return e;
}
static int pas_is_charexpr(tree_t *e) {
    e = pas_vt_unwrap_read(e);
    if (!e) return 0;
    if (e->t == TT_FNC && e->n == 5 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_rdecode") && e->c[3] && e->c[3]->t == TT_ILIT && (e->c[3]->v.ival & 0xFF) == 4) return 1;
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref") && pas_typename_class(pas_ptrexpr_target(e->c[1])) == 'c') return 1;
    if (pas_pf_rtype(e)) return !strcmp(pas_pf_rtype(e), "char");
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->v.sval &&
        (!strcmp(e->c[0]->v.sval, "__pas_fbuf_get") || !strcmp(e->c[0]->v.sval, "__pas_tbuf_get") || !strcmp(e->c[0]->v.sval, "__pas_pchar_deref"))) return pas_buf_is_char(e);
    if (e->t == TT_FNC && e->n == 1 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_getbufch")) return 1;
    if (e->t == TT_VAR && e->v.sval && pas_is_charvar(e->v.sval)) return 1;
    if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && e->c[1] && e->c[1]->t == TT_ILIT && pas_recvar_field_is_char(e->c[0]->v.sval, e->c[1]->v.ival)) return 1;
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "__pas_chr") || !strcmp(e->c[0]->v.sval, "__pas_chrlit"))) return 1;
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_charvar(e->c[0]->v.sval)) return 1;
    if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_chararr(e->c[0]->v.sval)) return 1;
    if (e->t == TT_IDX && e->n == 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && (pas_var_string_kind(e->c[0]->v.sval) || pas_is_pcharvar(e->c[0]->v.sval))) return 1;
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && pas_is_cafield(e->c[0])) return 1;
    if (pas_is_cvfield(e)) return 1;
    return 0;
}
static int pas_is_strtyped(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_QLIT) return 1;
    if (pas_ca_is_read(e)) return 1;
    if (e->t == TT_VAR && e->v.sval && pas_is_chararr(e->v.sval)) return 1;
    if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && (pas_is_chararr(e->c[0]->v.sval) || pas_is_strarr(e->c[0]->v.sval))) return 1;
    return 0;
}
static tree_t *pas_alpha_wrap(tree_t *x) {
    if (x && x->t == TT_VAR && x->v.sval && pas_is_chararr(x->v.sval)) {
        tree_t *f = ast_node_new(TT_FNC);
        ast_push(f, leaf_s(TT_VAR, "__pas_alpha_str"));
        ast_push(f, x);
        ast_push(f, ilit(pas_chararr_lo(x->v.sval)));
        return f;
    }
    if (x && x->t == TT_IDX && x->n >= 2 && x->c[0] && x->c[0]->t == TT_VAR && x->c[0]->v.sval && pas_is_strarr(x->c[0]->v.sval)) {
        tree_t *f = ast_node_new(TT_FNC);
        ast_push(f, leaf_s(TT_VAR, "__pas_alpha_str"));
        ast_push(f, x);
        ast_push(f, ilit(pas_strarr_lo(x->c[0]->v.sval)));
        return f;
    }
    return x;
}
static int pas_func_returns_stringish(const char *fname) {
    const char *t = fname ? pas_scalarvartype_get(fname) : NULL;
    for (int guard = 0; t && guard < 8; guard++) {
        if (!strcmp(t, "string") || !strcmp(t, "ansistring") || !strcmp(t, "shortstring") || !strcmp(t, "widestring") || !strcmp(t, "unicodestring")) return 1;
        const char *al = pas_typealias_get(t);
        if (!al || !strcmp(al, t)) break;
        t = al;
    }
    return 0;
}
static int pas_is_strval(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_QLIT || e->t == TT_CAT) return 1;
    if (pas_ca_is_read(e)) return 1;
    if (e->t == TT_VAR && e->v.sval && (pas_is_chararr(e->v.sval) || pas_is_strarr(e->v.sval))) return 1;
    if (e->t == TT_IDX && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_is_strarr(e->c[0]->v.sval)) return 1;
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_func_returns_stringish(e->c[0]->v.sval)) return 1;
    return 0;
}
static tree_t *mk_set_bin(const char *fn, tree_t *a, tree_t *b);
static tree_t *pas_rel(tree_e tt, tree_t *a, tree_t *b) {
    int lex = pas_is_strval(a) || pas_is_strval(b);
    if (lex || pas_is_strtyped(a) || pas_is_strtyped(b)) { a = pas_alpha_wrap(a); b = pas_alpha_wrap(b); }
    return lex ? bin(tt, mk_set_bin("__pas_strcmp", a, b), ilit(0)) : bin(tt, a, b);
}
static int pas_is_setexpr(tree_t *e);
static tree_t *pas_rel_or_set(tree_e tt, const char *setfn, tree_t *a, tree_t *b) { return (pas_is_setexpr(a) || pas_is_setexpr(b)) ? mk_set_bin(setfn, a, b) : pas_rel(tt, a, b); }
static void pas_case_push(void) {
    if (g_lower.pas.sem.pas_case_depth < 8) snprintf(g_lower.pas.sem.pas_case_tmp[g_lower.pas.sem.pas_case_depth], sizeof g_lower.pas.sem.pas_case_tmp[0], "__pct%d", g_lower.pas.sem.pas_case_ctr++);
    g_lower.pas.sem.pas_case_depth++;
}
static const char *pas_case_cur(void) { int d = g_lower.pas.sem.pas_case_depth - 1; if (d < 0) d = 0; if (d > 7) d = 7; return ct_strdup(g_lower.pas.sem.pas_case_tmp[d]); }
static void pas_case_pop(void) { if (g_lower.pas.sem.pas_case_depth > 0) g_lower.pas.sem.pas_case_depth--; }
static int pas_tree_same(tree_t *a, tree_t *b) {
    if (!a || !b) return a == b;
    if (a->t != b->t || a->n != b->n) return 0;
    if (a->t == TT_VAR || a->t == TT_QLIT) { if (!a->v.sval || !b->v.sval || strcmp(a->v.sval, b->v.sval)) return 0; } else if (a->t == TT_ILIT) { if (a->v.ival != b->v.ival) return 0; }
    for (int i = 0; i < a->n; i++) if (!pas_tree_same(a->c[i], b->c[i])) return 0;
    return 1;
}
static int pas_with_holds_pointee_of(tree_t *ptr) {
    for (int i = 0; ptr && i < g_lower.pas.sem.with_depth; i++) {
        tree_t *w = g_lower.pas.sem.with_stk[i].orig;
        if (w && w->t == TT_FNC && w->n == 2 && w->c[0] && w->c[0]->v.sval && !strcmp(w->c[0]->v.sval, "__pas_deref") && pas_tree_same(w->c[1], ptr)) return 1;
    }
    return 0;
}
static tree_t *pas_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t);
    c->v = e->v;
    if (e->t == TT_IDX && e->slen == PAS_FIELD_IDX_MARK) c->slen = e->slen;
    if ((e->t == TT_VAR || e->t == TT_QLIT) && e->v.sval) c->v.sval = ct_strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) ast_push(c, pas_tree_clone(e->c[i]));
    return c;
}
static const char *pas_with_sel_rtype(tree_t *sel) {
    if (!sel) return NULL;
    { const char *_er = pas_elem_rectype(sel); if (_er) return _er; }
    if (sel->t == TT_VAR && sel->v.sval) {
        for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, sel->v.sval)) {
            const char *rt = NULL;
            for (int j = 0; j < g_lower.pas.sem.pas_nrectype; j++) {
                int match = 1;
                if (!g_lower.pas.sem.pas_rectypes[j].tname) continue;
                if (g_lower.pas.sem.pas_rectypes[j].nf != g_lower.pas.sem.pas_recvars[i].nf) continue;
                for (int k = 0; k < g_lower.pas.sem.pas_recvars[i].nf; k++)
                    if (!g_lower.pas.sem.pas_recvars[i].fields[k] || !g_lower.pas.sem.pas_rectypes[j].fields[k] ||
                    strcmp(g_lower.pas.sem.pas_recvars[i].fields[k], g_lower.pas.sem.pas_rectypes[j].fields[k])) {
                    match = 0;
                    break;
                }
                if (match) { rt = g_lower.pas.sem.pas_rectypes[j].tname; break; }
            }
            return rt;
        }
    }
    if (sel->t == TT_FNC && sel->n >= 2 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_deref")) { const char *ptn = pas_ptrexpr_target(sel->c[1]); return ptn; }
    if (sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval) {
        const char *_arn = NULL;
        if (pas_arrrec_find(sel->c[0]->v.sval, &_arn) > 0 && _arn) return _arn;
    }
    if (sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval && sel->c[1] && sel->c[1]->t == TT_ILIT) {
        const char *_vfr = pas_recvar_field_rectype(sel->c[0]->v.sval, (long)sel->c[1]->v.ival);
        if (_vfr) return _vfr;
    }
    if (sel->t == TT_IDX && sel->n >= 2 && sel->c[1] && sel->c[1]->t == TT_ILIT) {
        const char *bt = pas_with_sel_rtype(sel->c[0]);
        if (bt) { const char *_fr = pas_rectype_field_rectype_by_index(bt, sel->c[1]->v.ival); if (_fr) return _fr; return pas_rectype_field_ptrto_by_index(bt, sel->c[1]->v.ival); }
    }
    return NULL;
}
static int pas_with_field_index(const char *rtype, const char *fname) { return pas_rectype_field_index(rtype, fname); }
static int pas_with_recvar_field(const char *vname, const char *fname) { return pas_recvar_field_index(vname, fname); }
static void pas_with_push(tree_t *sel) {
    if (g_lower.pas.sem.with_depth >= PAS_WITH_MAX || !sel) return;
    const char *rt = pas_with_sel_rtype(sel);
    g_lower.pas.sem.with_stk[g_lower.pas.sem.with_depth].sel = sel;
    g_lower.pas.sem.with_stk[g_lower.pas.sem.with_depth].rtype = rt;
    g_lower.pas.sem.with_stk[g_lower.pas.sem.with_depth].orig = sel;
    g_lower.pas.sem.with_stk[g_lower.pas.sem.with_depth].pre = NULL;
    g_lower.pas.sem.with_depth++;
}
static void pas_with_pop(void) { if (g_lower.pas.sem.with_depth > 0) g_lower.pas.sem.with_depth--; }
static tree_t *pas_with_tmp(tree_t *val, tree_t **pre) {
    char _b[24];
    snprintf(_b, sizeof _b, "__pas_wp%d", g_lower.pas.sem.wn++);
    const char *_nm = ct_strdup(_b);
    pas_scope_define(_nm);
    pas_local_add(_nm);
    if (!*pre) *pre = ast_node_new(TT_SEQ_EXPR);
    ast_push(*pre, mk_assign(leaf_s(TT_VAR, _nm), val));
    return leaf_s(TT_VAR, _nm);
}
static tree_t *pas_with_capture(tree_t *sel, tree_t **pre) {
    if (!sel) return sel;
    if (sel->t == TT_FNC && sel->n == 2 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_deref") && sel->c[1]) {
        const char *_tg = pas_ptrexpr_target(sel->c[1]);
        tree_t *_in = pas_with_capture(sel->c[1], pre);
        tree_t *_t = pas_with_tmp(_in, pre);
        if (_tg && _t->v.sval) pas_ptrvar_add(_t->v.sval, _tg);
        tree_t *_d = ast_node_new(sel->t);
        _d->v = sel->v;
        _d->slen = sel->slen;
        ast_push(_d, pas_tree_clone(sel->c[0]));
        ast_push(_d, _t);
        return _d;
    }
    if (sel->t == TT_IDX && sel->n == 2 && sel->c[0] && sel->c[1]) {
        tree_t *_b = pas_with_capture(sel->c[0], pre);
        tree_t *_i = sel->c[1]->t == TT_ILIT ? sel->c[1] : pas_with_tmp(sel->c[1], pre);
        tree_t *_d = ast_node_new(sel->t);
        _d->v = sel->v;
        _d->slen = sel->slen;
        ast_push(_d, _b);
        ast_push(_d, _i);
        return _d;
    }
    return sel;
}
static void pas_with_push_once(tree_t *sel) {
    tree_t *_pre = NULL;
    tree_t *_cs = pas_with_capture(sel, &_pre);
    int _d0 = g_lower.pas.sem.with_depth;
    pas_with_push(_cs);
    if (g_lower.pas.sem.with_depth > _d0) { g_lower.pas.sem.with_stk[_d0].orig = sel; g_lower.pas.sem.with_stk[_d0].pre = _pre; }
}
static tree_t *pas_with_finish(long long n, tree_t *body) {
    tree_t *q = NULL;
    int _lo = g_lower.pas.sem.with_depth - (int)n;
    if (_lo < 0) _lo = 0;
    for (int i = _lo; i < g_lower.pas.sem.with_depth; i++) if (g_lower.pas.sem.with_stk[i].pre) {
        if (!q) q = ast_node_new(TT_SEQ_EXPR);
        for (int k = 0; k < g_lower.pas.sem.with_stk[i].pre->n; k++) ast_push(q, g_lower.pas.sem.with_stk[i].pre->c[k]);
    }
    for (long long i = 0; i < n; i++) pas_with_pop();
    if (!q) return body;
    if (body) ast_push(q, body);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_elem_rectype(tree_t *e) {
    if (!e || e->t != TT_IDX || e->n != 2 || e->slen == PAS_FIELD_IDX_MARK || !e->c[0]) return NULL;
    tree_t *a = e->c[0];
    if (a->t == TT_FNC && a->n >= 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_deref")) {
        const char *pt = pas_ptrexpr_target(a->c[1]);
        return (pt && pas_arrtype_high(pt) >= 0 && pas_rectype_nf(pt) > 0) ? pt : NULL;
    }
    if (a->t != TT_IDX || a->n != 2 || !a->c[0] || !a->c[1] || a->c[1]->t != TT_ILIT) return NULL;
    long fi = (long)a->c[1]->v.ival;
    tree_t *b = a->c[0];
    if (b->t == TT_VAR && b->v.sval) {
        for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, b->v.sval))
            return (fi >= 0 && fi < g_lower.pas.sem.pas_recvars[i].nf && g_lower.pas.sem.pas_recvars[i].fldna[fi]) ? g_lower.pas.sem.pas_recvars[i].fldrec[fi] : NULL;
        return NULL;
    }
    { const char *bt = pas_with_sel_rtype(b); return (bt && pas_rectype_field_is_na(bt, fi)) ? pas_rectype_field_rectype_by_index(bt, fi) : NULL; }
}
static int pas_rectype_is_packed(const char *rn) {
    if (!rn) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) return g_lower.pas.sem.pas_rectypes[i]
        .packed;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_recvar_is_packed(const char *vn) {
    if (!vn) return 0;
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, vn)) return g_lower.pas.sem.pas_recvars[i]
        .packed;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_recvar_field_rectype(const char *vn, long idx) {
    if (!vn || idx < 0) return NULL;
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, vn))
        return (idx < g_lower.pas.sem.pas_recvars[i].nf) ? g_lower.pas.sem.pas_recvars[i].fldrec[idx] : NULL;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_designator_rectype(tree_t *e) {
    if (!e) return NULL;
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) {
        long idx = (long)e->c[1]->v.ival;
        tree_t *b = e->c[0];
        if (b->t == TT_VAR && b->v.sval) return pas_recvar_field_rectype(b->v.sval, idx);
        const char *bt = pas_designator_rectype(b);
        return bt ? pas_rectype_field_rectype_by_index(bt, idx) : NULL;
    }
    if (e->t == TT_FIELD && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_VAR && e->c[1]->v.sval) {
        tree_t *b = e->c[0];
        const char *fn = e->c[1]->v.sval;
        if (b->t == TT_VAR && b->v.sval) { int fi = pas_recvar_field_index(b->v.sval, fn); return (fi >= 0) ? pas_recvar_field_rectype(b->v.sval, fi) : NULL; }
        const char *bt = pas_designator_rectype(b);
        if (!bt) return NULL;
        int fi = pas_rectype_field_index(bt, fn);
        return (fi >= 0) ? pas_rectype_field_rectype_by_index(bt, fi) : NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_is_aggregate_designator(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) { long long _ah; return pas_array_high_get(e->v.sval, &_ah); }
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return pas_rectype_nf(pas_ptrexpr_target(e->c[1])) > 0;
    if (e->t == TT_IDX && e->n == 2 && e->slen != PAS_FIELD_IDX_MARK && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_arrrec_find(e->c[0]->v.sval, NULL) > 0) return 1;
    if (pas_elem_rectype(e)) return 1;
    if (e->t != TT_IDX || e->n != 2 || !e->c[0] || !e->c[1] || e->c[1]->t != TT_ILIT) return 0;
    if (pas_designator_rectype(e)) return 1;
    {
        long idx = (long)e->c[1]->v.ival;
        tree_t *b = e->c[0];
        if (b->t == TT_VAR && b->v.sval) {
            for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, b->v.sval))
                return (idx >= 0 && idx < g_lower.pas.sem.pas_recvars[i].nf) ? (g_lower.pas.sem.pas_recvars[i].fldna[idx] || g_lower.pas.sem.pas_recvars[i].fldrec[idx] != NULL) : 0;
            return 0;
        }
        { const char *bt = pas_with_sel_rtype(b); return bt ? (pas_rectype_field_is_na(bt, idx) || pas_rectype_field_rectype_by_index(bt, idx) != NULL) : 0; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_assigns_result(tree_t *t, const char *f) {
    if (!t) return 0;
    if (t->t == TT_ASSIGN && t->n >= 1 && t->c[0]) { tree_t *l = t->c[0]->t == TT_FNC && t->c[0]->n >= 1 ? t->c[0]->c[0] : t->c[0]; return l && l->t == TT_VAR && l->v.sval && !strcmp(l->v.sval, f); }
    if (t->t == TT_FNC && t->n >= 1 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__pas_rterr")) return 1;
    if (t->t == TT_IF) return t->n == 3 && pas_assigns_result(t->c[1], f) && pas_assigns_result(t->c[2], f);
    if (t->t == TT_PROGRAM || t->t == TT_SEQ_EXPR) { for (int i = 0; i < t->n; i++) if (pas_assigns_result(t->c[i], f)) return 1; }
    return 0;
}
static tree_t *pas_result_check(const char *fname, tree_t *body) {
    if (g_lower.pas.sem.pas_seen_mode_directive || !fname || !body || body->t != TT_PROGRAM || pas_func_returns_stringish(fname) || pas_assigns_result(body, fname)) return body;
    size_t ml = strlen(fname) + 80;
    char *msg = (char *)ct_zalloc(ml, 1);
    snprintf(msg, ml, "the result of the function '%s' is undefined upon completion of its algorithm", fname);
    tree_t *chain = ast_node_new(TT_FNC);
    ast_push(chain, leaf_s(TT_VAR, "__pas_rterr"));
    ast_push(chain, leaf_s(TT_QLIT, "6.6.2"));
    ast_push(chain, leaf_s(TT_QLIT, msg));
    ast_push(body, bin(TT_IF, bin(TT_EQ, mk_fnc1("__pas_resundef", leaf_s(TT_VAR, fname)), ilit(1)), chain));
    return body;
}
static int pas_actual_is_tagfield(tree_t *a) {
    for (int i = g_lower.pas.sem.pas_nvt - 1; a && i >= 0; i--)
        if ((g_lower.pas.sem.pas_vt[i]->state == 7 || g_lower.pas.sem.pas_vt[i]->state == 5 || g_lower.pas.sem.pas_vt[i]->state == 8) && g_lower.pas.sem.pas_vt[i]->node == a) return 1;
    return 0;
}
static int pas_actual_is_packed_component(tree_t *a) {
    if (!a || a->n < 2 || !a->c[0] || !a->c[1]) return 0;
    if (a->t == TT_IDX && a->c[0]->t == TT_IDX && a->c[0]->n >= 2 && a->c[0]->c[0] && a->c[0]->c[1] && a->c[0]->c[1]->t == TT_ILIT && a->c[1]->t != TT_ILIT) {
        tree_t *b = a->c[0]->c[0];
        long i = (long)a->c[0]->c[1]->v.ival;
        const char *rt = NULL;
        if (b->t == TT_VAR && b->v.sval) { if (pas_recvar_is_packed(b->v.sval)) return 1; rt = pas_recvar_field_rectype(b->v.sval, i); }
        if (!rt) { const char *bt = pas_designator_rectype(b); rt = bt ? pas_rectype_field_rectype_by_index(bt, i) : NULL; }
        return (rt && pas_rectype_is_packed(rt)) ? 1 : 0;
    }
    if (!((a->t == TT_IDX && a->c[1]->t == TT_ILIT) || (a->t == TT_FIELD && a->c[1]->t == TT_VAR))) return 0;
    tree_t *base = a->c[0];
    if (base->t == TT_VAR && base->v.sval) return pas_recvar_is_packed(base->v.sval);
    const char *rt = pas_designator_rectype(base);
    return (rt && pas_rectype_is_packed(rt)) ? 1 : 0;
}
static const char *pas_field_typename(tree_t *e) {
    if (!e || e->t != TT_IDX || e->n != 2 || !e->c[0] || !e->c[1]) return NULL;
    const char *rt = NULL;
    long long fi = -1;
    if (e->slen == PAS_FIELD_IDX_MARK && e->c[0]->t == TT_IDX && e->c[0]->n == 2 && e->c[0]->c[0] && e->c[0]->c[0]->t == TT_VAR && e->c[1]->t == TT_ILIT) {
        fi = e->c[1]->v.ival;
        for (int i = 0; i < g_lower.pas.sem.pas_narrrec; i++) if (g_lower.pas.sem.pas_arrrecs[i].aname && !strcmp(g_lower.pas.sem.pas_arrrecs[i].aname, e->c[0]->c[0]->v.sval))
            return (fi >= 0 && fi < 32) ? g_lower.pas.sem.pas_arrrecs[i].fldtypename[fi] : NULL;
        return NULL;
    }
    if (e->c[1]->t == TT_ILIT) { rt = pas_selector_rectype(e->c[0]); if (!rt) rt = pas_with_sel_rtype(e->c[0]); fi = e->c[1]->v.ival; }
    if (!rt || fi < 0) return NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rt))
        return (fi < g_lower.pas.sem.pas_rectypes[i].nf) ? g_lower.pas.sem.pas_rectypes[i].fldtypename[fi] : NULL;
    return NULL;
}
static int pas_is_setexpr(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) return pas_is_setvar(e->v.sval);
    if (e->t == TT_IDX && pas_is_settype(pas_field_typename(e))) return 1;
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->v.sval) {
        const char *f = e->c[0]->v.sval;
        if (!strcmp(f, "__pas_rdecode") && e->n == 5 && e->c[3] && e->c[3]->t == TT_ILIT && (e->c[3]->v.ival & 0xFF) == 6) return 1;
        return !strcmp(f, "__pas_set") || !strcmp(f, "__pas_setrange") || !strcmp(f, "__pas_setuni") || !strcmp(f, "__pas_setint") || !strcmp(f, "__pas_setdif");
    }
    return 0;
}
static tree_t *mk_set_bin(const char *name, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, name)); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *pas_rdiv(tree_t *a, tree_t *b) { return bin(TT_DIV, bin(TT_MUL, a, flit(1.0)), b); }
static int pas_var_unsigned(const char *name) {
    if (!name) return 0;
    const char *tn = pas_scalarvartype_get(name);
    long long lo, hi;
    if (tn && (pas_is_unsigned_inttypename(tn) || (pas_subtype_high(tn) >= 0 && pas_subtype_low(tn) >= 0))) return 1;
    return pas_subvar_get(name, &lo, &hi) && lo >= 0;
}
static int pas_unsigned_sum(tree_t *e, int *nv) {
    if (!e) return 0;
    if (e->t == TT_ILIT) return e->v.ival >= 0;
    if (e->t == TT_VAR) { if (!pas_var_unsigned(e->v.sval)) return 0; (*nv)++; return 1; }
    return e->t == TT_ADD && e->n == 2 && pas_unsigned_sum(e->c[0], nv) && pas_unsigned_sum(e->c[1], nv);
}
static int pas_is_qword(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_ILIT) return e->v.ival < 0;
    if (e->t != TT_VAR || !e->v.sval) return 0;
    const char *tn = pas_scalarvartype_get(e->v.sval);
    for (int g = 0; tn && g < 8; g++) { if (!strcmp(tn, "qword") || !strcmp(tn, "uint64")) return 1; const char *al = pas_typealias_get(tn); if (!al || !strcmp(al, tn)) break; tn = al; }
    return 0;
}
static int pas_is_stringexpr(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_CAT) return 1;
    if (e->t == TT_QLIT) return !(e->v.sval && strlen(e->v.sval) == 1);
    if (e->t == TT_VAR && e->v.sval) return pas_var_string_kind(e->v.sval) != 0;
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) {
        const char *rt = pas_with_sel_rtype(e->c[0]);
        const char *tn = rt ? pas_rectype_field_typename(rt, e->c[1]->v.ival) : NULL;
        return tn && pas_string_type_kind(tn);
    }
    { const char *rt = pas_pf_rtype(e); if (rt && pas_string_type_kind(rt)) return 1; }
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->v.sval && pas_func_returns_stringish(e->c[0]->v.sval)) return 1;
    return 0;
}
static tree_t *pas_concat_operand(tree_t *e) {
    if (!e || pas_is_stringexpr(e) || !pas_is_charexpr(e)) return e;
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_chrlit") && e->c[1] && e->c[1]->t == TT_ILIT) {
        char *cb = (char *)ct_zalloc(2, 1);
        cb[0] = (char)e->c[1]->v.ival;
        tree_t *q = ast_node_new(TT_QLIT);
        q->v.sval = cb;
        return q;
    }
    return mk_chr_wrap(e);
}
static tree_t *pas_arith_or_set(tree_e ak, const char *setfn, tree_t *a, tree_t *b) {
    if (pas_is_setexpr(a) || pas_is_setexpr(b)) return mk_set_bin(setfn, a, b);
    if (ak == TT_ADD && (pas_is_stringexpr(a) || pas_is_stringexpr(b) || (g_lower.pas.sem.pas_seen_mode_directive && pas_is_charexpr(a) && pas_is_charexpr(b))))
        return bin(TT_CAT, pas_concat_operand(a), pas_concat_operand(b));
    if (ak == TT_SUB && (g_lower.pas.sem.pas_zerobased_strings & 2) && a && a->t == TT_ADD) { int nv = 0; if (pas_unsigned_sum(a, &nv) && nv >= 2) return mk_fnc1("__pas_qchk", bin(ak, a, b)); }
    return bin(ak, a, b);
}
static struct pas_vt *pas_vt_find_name(const char *nm) {
    if (!nm) return NULL;
    for (int i = g_lower.pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower.pas.sem.pas_vt[i]->state == 2 && g_lower.pas.sem.pas_vt[i]->name && !strcmp(g_lower.pas.sem.pas_vt[i]->name, nm))
        return g_lower.pas.sem.pas_vt[i];
    return NULL;
}
static void pas_vt_mark(tree_t *e, tree_t *base, const char *rt, int fi) {
    if (!e || !base || fi < 0 || fi >= PAS_FIELD_MAX) return;
    const char *nm = rt ? rt : pas_with_sel_rtype(base);
    struct pas_vt *t = pas_vt_find_name(nm);
    if (!t && base->t == TT_VAR && base->v.sval) t = pas_vt_find_name(base->v.sval);
    if (t && t->tagfi >= 0 && fi == t->tagfi && base->t == TT_FNC && base->n == 2 && base->c[0] && base->c[0]->v.sval && !strcmp(base->c[0]->v.sval, "__pas_deref")) {
        struct pas_vt *m = pas_vt_new(5);
        m->node = e;
        m->tagexpr = pas_tree_clone(base->c[1]);
        return;
    }
    if (t && t->tagfi >= 0 && fi == t->tagfi && base->t == TT_VAR && base->v.sval) { struct pas_vt *m = pas_vt_new(7); m->node = e; m->name = ct_strdup(base->v.sval); return; }
    if (!t || !t->fhas[fi] || t->tagfi < 0) return;
    if (base->t != TT_VAR || !base->v.sval) return;
    {
        int asg = 0;
        for (int i = g_lower.pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower.pas.sem.pas_vt[i]->state == 6 && g_lower.pas.sem.pas_vt[i]->name && !strcmp(g_lower.pas.sem.pas_vt[i]->name, base->v.sval)) {
            asg = 1;
            break;
        }
        if (!asg) return;
    }
    int slot = (rt || (nm && pas_vt_find_name(nm))) ? pas_rectype_slot(nm, t->tagfi) : pas_recvar_slot(base->v.sval, t->tagfi);
    tree_t *te = ast_node_new(TT_IDX);
    ast_push(te, pas_tree_clone(base));
    ast_push(te, ilit(slot));
    struct pas_vt *m = pas_vt_new(4);
    m->node = e;
    m->tagexpr = te;
    m->mask = t->fmask[fi];
}
static void pas_vt_rekey(tree_t *from, tree_t *to) { for (int i = g_lower.pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower.pas.sem.pas_vt[i]->node == from) g_lower.pas.sem.pas_vt[i]->node = to; }
static tree_t *pas_region_slot_of(tree_t *sel) {
    if (!sel) return NULL;
    if (sel->t == TT_IDX) return sel;
    if (sel->t == TT_FNC && sel->n >= 2 && sel->c[0] && sel->c[0]->v.sval && (!strcmp(sel->c[0]->v.sval, "__pas_rdecode") || !strcmp(sel->c[0]->v.sval, "__pas_rarr")) && sel->c[1] &&
        sel->c[1]->t == TT_IDX) return sel->c[1];
    return NULL;
}
static tree_t *pas_vt_check_of(tree_t *sel) {
    if (!sel) return NULL;
    for (int i = g_lower.pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower.pas.sem.pas_vt[i]->state == 4 && g_lower.pas.sem.pas_vt[i]->node == sel) {
        tree_t *c = ast_node_new(TT_FNC);
        ast_push(c, leaf_s(TT_VAR, "__pas_vcheck"));
        ast_push(c, pas_tree_clone(g_lower.pas.sem.pas_vt[i]->tagexpr));
        ast_push(c, ilit(g_lower.pas.sem.pas_vt[i]->mask));
        return c;
    }
    if (sel->t == TT_FNC && sel->n == 2 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_deref")) {
        const char *wt = pas_ptrexpr_target(sel->c[1]);
        struct pas_vt *vt = wt ? pas_vt_find_name(wt) : NULL;
        if (vt && vt->tagfi >= 0) { tree_t *c = ast_node_new(TT_FNC); ast_push(c, leaf_s(TT_VAR, "__pas_tagwhole")); ast_push(c, pas_tree_clone(sel->c[1])); return c; }
    }
    return NULL;
}
static tree_t *pas_vt_wrap_read(tree_t *sel) {
    tree_t *c = pas_vt_check_of(sel);
    if (!c) return sel;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    ast_push(q, c);
    tree_t *_sl = pas_region_slot_of(sel);
    if (!g_lower.pas.sem.pas_seen_mode_directive && _sl && c->c[0]->v.sval && !strcmp(c->c[0]->v.sval, "__pas_vcheck")) {
        tree_t *er = ast_node_new(TT_FNC);
        ast_push(er, leaf_s(TT_VAR, "__pas_rterr"));
        ast_push(er, leaf_s(TT_QLIT, "6.5.3.3"));
        ast_push(er, leaf_s(TT_QLIT, "a variant of a variant-part is undefined after the tag-field changes value until it is assigned"));
        ast_push(q, bin(TT_IF, bin(TT_EQ, mk_fnc1("__pas_resundef", pas_tree_clone(_sl)), ilit(1)), er));
    }
    ast_push(q, sel);
    return q;
}
static int pas_tree_pure(const tree_t *t) {
    if (!t) return 0;
    switch (t->t) {
        case TT_ILIT:
        case TT_QLIT:
        case TT_VAR:
        return 1;
        case TT_EQ:
        case TT_NE:
        case TT_LT:
        case TT_LE:
        case TT_GT:
        case TT_GE:
        case TT_NOT:
        case TT_ADD:
        case TT_SUB:
        case TT_MUL:
        for (int i = 0; i < t->n; i++) if (!pas_tree_pure(t->c[i])) return 0;
        return 1;
        default:
        return 0;
    }
}
static tree_t *pas_field_init_tree(const char *rn, const char *vn, int fi) {
    char *const *fr = NULL;
    const int *fn = NULL;
    const long long *lo = NULL, *hi = NULL;
    int nf = 0;
    for (int i = 0; rn && i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) {
        fr = g_lower.pas.sem.pas_rectypes[i].fldrec;
        fn = g_lower.pas.sem.pas_rectypes[i].fldna;
        lo = g_lower.pas.sem.pas_rectypes[i].fldna_lo;
        hi = g_lower.pas.sem.pas_rectypes[i].fldna_hi;
        nf = g_lower.pas.sem.pas_rectypes[i].nf;
        break;
    }
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; !fr && vn && i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, vn)) {
        fr = g_lower.pas.sem.pas_recvars[i].fldrec;
        fn = g_lower.pas.sem.pas_recvars[i].fldna;
        lo = g_lower.pas.sem.pas_recvars[i].fldna_lo;
        hi = g_lower.pas.sem.pas_recvars[i].fldna_hi;
        nf = g_lower.pas.sem.pas_recvars[i].nf;
        break;
    }
    if (!fr || fi < 0 || fi >= nf) return NULL;
    if (fn[fi] && hi[fi] >= lo[fi] && fr[fi]) return mk_fnc3("__pas_arr_of", ilit(lo[fi]), ilit(hi[fi]), mk_rec_init_type_d(fr[fi], 1));
    if (fn[fi] && hi[fi] >= lo[fi]) return mk_fnc2("arr_make", ilit(lo[fi]), ilit(hi[fi]));
    if (!fn[fi] && fr[fi]) return mk_rec_init_type_d(fr[fi], 1);
    return NULL;
}
static tree_t *pas_vt_undefine_variants(tree_t *sel, tree_t *rhs, tree_t *st) {
    if (g_lower.pas.sem.pas_seen_mode_directive || !sel || sel->t != TT_IDX || sel->n < 2 || !sel->c[0] || sel->c[0]->t != TT_VAR || !sel->c[0]->v.sval || !pas_tree_pure(rhs)) return st;
    const char *vn = sel->c[0]->v.sval, *rn = pas_with_sel_rtype(sel->c[0]);
    struct pas_vt *t = rn ? pas_vt_find_name(rn) : NULL;
    if (!t) t = pas_vt_find_name(vn);
    if (!t || t->tagfi < 0) return st;
    tree_t *nul = ast_node_new(TT_SEQ_EXPR);
    int ns = 0;
    for (int fi = 0; fi < PAS_FIELD_MAX; fi++) {
        if (!t->fhas[fi] || fi == t->tagfi) continue;
        int sl = pas_recvar_slot(vn, fi);
        int dup = 0;
        for (int fj = 0; fj < fi; fj++) if (t->fhas[fj] && fj != t->tagfi && pas_recvar_slot(vn, fj) == sl) dup = 1;
        if (dup) continue;
        ns++;
        tree_t *ix = ast_node_new(TT_IDX);
        ast_push(ix, leaf_s(TT_VAR, vn));
        ast_push(ix, ilit(sl));
        { tree_t *_iv = pas_field_init_tree(rn, vn, fi); ast_push(nul, mk_assign(ix, _iv ? _iv : leaf_s(TT_VAR, "__pas_undefined_value"))); }
    }
    if (!ns) return st;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    ast_push(q, bin(TT_IF, bin(TT_NE, pas_tree_clone(sel), pas_tree_clone(rhs)), nul));
    ast_push(q, st);
    return q;
}
static tree_t *pas_vt_strip_undef(tree_t *a) {
    if (!a || a->t != TT_SEQ_EXPR || a->n != 3 || !a->c[1] || a->c[1]->t != TT_IF || a->c[1]->n != 2 || !a->c[1]->c[1] || a->c[1]->c[1]->t != TT_FNC || a->c[1]->c[1]->n < 2 || !a->c[1]->c[1]->c[1] ||
        !a->c[1]->c[1]->c[1]->v.sval || strcmp(a->c[1]->c[1]->c[1]->v.sval, "6.5.3.3")) return a;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    ast_push(q, a->c[0]);
    ast_push(q, a->c[2]);
    return q;
}
static tree_t *pas_vt_wrap_stmt(tree_t *sel, tree_t *rhs, tree_t *st) {
    for (int i = g_lower.pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower.pas.sem.pas_vt[i]->state == 7 && g_lower.pas.sem.pas_vt[i]->node == sel) {
        struct pas_vt *a = pas_vt_new(6);
        a->name = ct_strdup(g_lower.pas.sem.pas_vt[i]->name);
        st = pas_vt_undefine_variants(sel, rhs, st);
        break;
    }
    for (int i = g_lower.pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower.pas.sem.pas_vt[i]->state == 5 && g_lower.pas.sem.pas_vt[i]->node == sel && rhs) {
        tree_t *c = ast_node_new(TT_FNC);
        ast_push(c, leaf_s(TT_VAR, "__pas_tagset_chk"));
        ast_push(c, pas_tree_clone(g_lower.pas.sem.pas_vt[i]->tagexpr));
        ast_push(c, pas_tree_clone(sel));
        ast_push(c, pas_tree_clone(rhs));
        tree_t *q = ast_node_new(TT_SEQ_EXPR);
        ast_push(q, c);
        ast_push(q, st);
        return q;
    }
    tree_t *c = pas_vt_check_of(sel);
    if (!c) return st;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    ast_push(q, c);
    ast_push(q, st);
    return q;
}
static tree_t *pas_nested_field_resolve(tree_t *base, const char *fld) {
    const char *_brt = pas_with_sel_rtype(base);
    if (_brt) {
        int _nfi = pas_rectype_field_index(_brt, fld);
        if (_nfi >= 0) {
            tree_t *e = ast_node_new(TT_IDX);
            ast_push(e, base);
            ast_push(e, ilit(pas_rectype_slot(_brt, _nfi)));
            const char *_fe = pas_rectype_field_enum_by_index(_brt, _nfi);
            if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); }
            if (pas_rectype_field_is_ca(_brt, _nfi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_brt, _nfi), pas_rectype_field_ca_hi(_brt, _nfi));
            if (pas_rectype_field_is_na(_brt, _nfi)) pas_nafield_mark_add(e, pas_rectype_field_na_lo(_brt, _nfi));
            if (pas_rectype_field_is_char(_brt, _nfi)) pas_cvfield_mark_add(e);
            return pas_field_region(base, _brt, _nfi, e);
        }
    }
    return bin(TT_FIELD, base, leaf_s(TT_VAR, fld));
}
static int pas_is_realtypename(const char *t) { return t && (!strcmp(t, "real") || !strcmp(t, "single") || !strcmp(t, "double") || !strcmp(t, "extended") || !strcmp(t, "comp")); }
static int pas_is_int_typename(const char *t) {
    return t &&
        (!strcmp(t, "integer") || !strcmp(t, "longint") || !strcmp(t, "shortint") || !strcmp(t, "byte") || !strcmp(t, "word") || !strcmp(t, "cardinal") || !strcmp(t, "int64") || !strcmp(t, "qword") ||
        !strcmp(t, "smallint") || !strcmp(t, "longword"));
}
static int pas_var_typename_is(const char *name, int (*pred)(const char *)) {
    const char *t = name ? pas_scalarvartype_get(name) : NULL;
    for (int guard = 0; t && guard < 8; guard++) { if (pred(t)) return 1; const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
    return 0;
}
static int pas_var_is_real(const char *name) { return pas_var_typename_is(name, pas_is_realtypename); }
static int pas_var_every_decl_real(const char *name) {
    int n = 0;
    for (int i = 0; name && i < g_lower.pas.sem.pas_nscalarvartype; i++) if (g_lower.pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower.pas.sem.pas_scalarvartype[i].vname, name)) {
        const char *t = g_lower.pas.sem.pas_scalarvartype[i].tname;
        for (int guard = 0; t && guard < 8 && !pas_is_realtypename(t); guard++) { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
        if (!t || !pas_is_realtypename(t)) return 0;
        n++;
    }
    return n > 0;
}
static void pas_ordinal_fn_arg(const char *name, PNodeList *args) {
    if (!name || !args || args->count < 1 || pas_is_func(name) || pas_is_proc(name)) return;
    int is_chr = !strcmp(name, "chr");
    if (!is_chr && strcmp(name, "ord") && strcmp(name, "succ") && strcmp(name, "pred")) return;
    tree_t *a = args->items[0];
    if (!a || !(a->t == TT_FLIT || (a->t == TT_VAR && a->v.sval && pas_var_every_decl_real(a->v.sval)))) return;
    if (a->t == TT_FLIT)
        fprintf(stderr, "pascal: ISO 7185 6.6.6.4 violation: the argument of %s is the real constant %g, and %s requires an expression of %s\n", name, a->v.dval, name,
        is_chr ? "integer-type" : "an ordinal-type");
    else fprintf(stderr, "pascal: ISO 7185 6.6.6.4 violation: the argument of %s is the variable '%s' of type real, and %s requires an expression of %s\n", name, a->v.sval, name,
        is_chr ? "integer-type" : "an ordinal-type");
    g_lower.pas.sem.pas_iso_errors++;
}
static char pas_expr_lit_class(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_FLIT) return 'r';
    if (e->t == TT_QLIT) return (e->v.sval && strlen(e->v.sval) == 1) ? 'c' : 's';
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_chrlit")) return 'c';
    if (e->t == TT_EQ && e->n == 2 && e->c[0] && e->c[1] && e->c[0]->t == TT_ILIT && e->c[1]->t == TT_ILIT) return 'b';
    return 0;
}
static char pas_typename_class(const char *t) {
    char c = 0;
    for (int guard = 0; t && guard < 8 && !c; guard++) {
        if (pas_is_int_typename(t)) c = 'i';
        else if (pas_is_realtypename(t)) c = 'r';
        else if (!strcmp(t, "char")) c = 'c';
        else if (pas_is_booltype(t)) c = 'b';
        else if (pas_enumtype_high(t) >= 0) c = 'e';
        else { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
    }
    return c;
}
static char pas_var_decl_class(const char *name) {
    char k = 0;
    if (!name || pas_is_chararr(name) || pas_is_strarr(name)) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) return 0;
    for (int i = 0; name && i < g_lower.pas.sem.pas_nscalarvartype; i++) if (g_lower.pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower.pas.sem.pas_scalarvartype[i].vname, name)) {
        char c = pas_typename_class(g_lower.pas.sem.pas_scalarvartype[i].tname);
        if (!c || (k && k != c)) return 0;
        k = c;
    }
    return k;
}
static int pas_ord_var_bounds(const char *name, long long *lo, long long *hi) {
    int n = 0;
    long long l = 0, h = -1;
    if (!name || pas_is_chararr(name) || pas_is_strarr(name)) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nscalarvartype; i++) if (g_lower.pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower.pas.sem.pas_scalarvartype[i].vname, name)) {
        const char *t = g_lower.pas.sem.pas_scalarvartype[i].tname;
        long long cl = 0, ch = -1;
        int ok = 0;
        for (int guard = 0; t && guard < 8 && !ok; guard++) {
            if (!strcmp(t, "integer")) {
                cl = -2147483647LL;
                ch = 2147483647LL;
                ok = 1;
            } else if (!strcmp(t, "char")) {
                cl = 0;
                ch = 255;
                ok = 1;
            } else if (pas_is_booltype(t)) {
                cl = 0;
                ch = 1;
                ok = 1;
            } else if (pas_enumtype_high(t) >= 0 && pas_subtype_high(t) < 0) {
                cl = 0;
                ch = pas_enumtype_high(t);
                ok = 1;
            } else {
                const char *al = pas_typealias_get(t);
                if (!al || !strcmp(al, t)) break;
                t = al;
            }
        }
        if (!ok || (n && (cl != l || ch != h))) return 0;
        l = cl;
        h = ch;
        n++;
    }
    if (!n) return 0;
    *lo = l;
    *hi = h;
    return 1;
}
static const char *pas_class_name(char k) {
    switch (k) {
        case 'i':
        return "integer-type";
        case 'r':
        return "real-type";
        case 'c':
        return "char-type";
        case 'b':
        return "Boolean-type";
        case 'e':
        return "an enumerated-type";
        case 's':
        return "a string-type";
        default:
        return "?";
    }
}
static void pas_sign_operand(tree_t *e, char op) {
    char k = pas_expr_lit_class(e);
    if (!k && e && e->t == TT_VAR && e->v.sval) k = pas_var_decl_class(e->v.sval);
    if (!k || k == 'i' || k == 'r') return;
    fprintf(stderr, "pascal: ISO 7185 6.7.2.2 violation: the monadic %c is applied to an operand of %s, and it takes only integer-type or real-type (table 4)\n", op, pas_class_name(k));
    g_lower.pas.sem.pas_iso_errors++;
}
static void pas_set_member_ordinal(tree_t *e) {
    char k = pas_expr_lit_class(e);
    if (!k && e && e->t == TT_VAR && e->v.sval) k = pas_var_decl_class(e->v.sval);
    if (k != 'r' && k != 's') return;
    fprintf(stderr, "pascal: ISO 7185 6.7.1 violation: a member-designator of a set-constructor is of %s, and the type of its expressions shall be an ordinal-type\n", pas_class_name(k));
    g_lower.pas.sem.pas_iso_errors++;
}
static void pas_variant_values(tree_t *e, long long *v, int *n, int cap) {
    if (!e) return;
    if (e->t == TT_ADD && e->n == 2) { pas_variant_values(e->c[0], v, n, cap); pas_variant_values(e->c[1], v, n, cap); return; }
    if (e->t == TT_EQ && e->n == 2 && e->c[1] && e->c[1]->t == TT_ILIT && *n < cap) v[(*n)++] = e->c[1]->v.ival;
}
static void pas_variant_constants_in_tag_type(const char *tag_type, PNodeList *arms) {
    const char *t = tag_type;
    long long lo = 0, hi = -1;
    for (int guard = 0; t && guard < 8 && hi < lo; guard++) {
        if (pas_is_booltype(t)) {
            lo = 0;
            hi = 1;
        } else if (pas_subtype_high(t) >= 0) {
            lo = pas_subtype_low(t);
            hi = pas_subtype_high(t);
        } else if (pas_enumtype_high(t) >= 0) {
            lo = 0;
            hi = pas_enumtype_high(t);
        } else {
            const char *al = pas_typealias_get(t);
            if (!al || !strcmp(al, t)) break;
            t = al;
        }
    }
    if (hi < lo || hi - lo >= 4096 || !arms) return;
    long long v[4096];
    int n = 0;
    unsigned char seen[4096];
    memset(seen, 0, sizeof seen);
    for (int i = 0; i < arms->count; i++) pas_variant_values(arms->items[i], v, &n, 4096);
    for (int i = 0; i < n; i++) {
        if (v[i] < lo || v[i] > hi) {
            fprintf(stderr, "pascal: ISO 7185 6.4.3.3 violation: a case-constant of the variant-part denotes %lld, which is not a value of its tag-type %s (%lld..%lld)\n", v[i], tag_type, lo, hi);
            g_lower.pas.sem.pas_iso_errors++;
            return;
        }
        if (seen[v[i] - lo]++) {
            fprintf(stderr, "pascal: ISO 7185 6.4.3.3 violation: the value %lld of the tag-type %s is denoted by more than one case-constant of the variant-part\n", v[i], tag_type);
            g_lower.pas.sem.pas_iso_errors++;
            return;
        }
    }
    for (long long x = lo; x <= hi; x++) if (!seen[x - lo]) {
        fprintf(stderr, "pascal: ISO 7185 6.4.3.3 violation: the case-constants of the variant-part do not denote the value %lld of its tag-type %s, and they shall denote every value of it\n", x,
            tag_type);
        g_lower.pas.sem.pas_iso_errors++;
        return;
    }
}
static void pas_type_not_self_applied(const char *name) {
    int self = name && g_lower.pas.sem.pas_pend_typename && !strcmp(g_lower.pas.sem.pas_pend_typename, name) && !g_lower.pas.sem.pas_pend_ptrtarget;
    for (int i = 0; name && !self && i < g_lower.pas.sem.pas_pend_nf && i < PAS_FIELD_MAX; i++)
        if (g_lower.pas.sem.pas_pend_fldtypename[i] && !strcmp(g_lower.pas.sem.pas_pend_fldtypename[i], name) && !g_lower.pas.sem.pas_pend_fldptrto[i]) self = 1;
    if (!self) return;
    fprintf(stderr, "pascal: ISO 7185 6.4.1 violation: the type-denoter of the type-definition of '%s' contains an applied occurrence of '%s' outside the domain-type of a pointer-type\n", name, name);
    g_lower.pas.sem.pas_iso_errors++;
}
static int pas_typealias_is_char(const char *t) {
    for (int guard = 0; t && guard < 8; guard++) { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) return 0; if (!strcmp(al, "char")) return 1; t = al; }
    return 0;
}
static int pas_class_compatible(char to, char from) { return to == from || (to == 'r' && from == 'i'); }
static void pas_value_compat(const char *vn, tree_t *rhs, const char *clause, const char *what) {
    char to = pas_var_decl_class(vn), from = pas_expr_lit_class(rhs);
    if (!to || !from || pas_class_compatible(to, from)) return;
    fprintf(stderr, "pascal: ISO 7185 %s violation: %s '%s' is given a value of %s, which is not assignment-compatible with its %s (6.4.6)\n", clause, what, vn, pas_class_name(from),
        pas_class_name(to));
    g_lower.pas.sem.pas_iso_errors++;
}
static void pas_deref_assign_compat(tree_t *lhs, tree_t *rhs) {
    if (!lhs || lhs->t != TT_FNC || lhs->n != 2 || !lhs->c[0] || !lhs->c[0]->v.sval || strcmp(lhs->c[0]->v.sval, "__pas_deref")) return;
    const char *tn = pas_ptrexpr_target(lhs->c[1]);
    char to = pas_typename_class(tn), from = pas_expr_lit_class(rhs);
    if (!from && to == 'c' && rhs && rhs->t == TT_ILIT) from = 'i';
    if (!to || !from || pas_class_compatible(to, from)) return;
    fprintf(stderr,
        "pascal: ISO 7185 6.8.2.2 violation: the variable referenced through a pointer of the domain type '%s' is given a value of %s, which is not assignment-compatible with its %s (6.4.6)\n", tn,
        pas_class_name(from), pas_class_name(to));
    g_lower.pas.sem.pas_iso_errors++;
}
static void pas_string_assign_check(const char *vn, tree_t *rhs) {
    if (g_lower.pas.sem.pas_seen_mode_directive || !vn || !pas_is_chararr(vn)) return;
    char k = pas_expr_lit_class(rhs);
    int i = pas_chararr_row(vn);
    if (i < 0 || (k != 'c' && k != 's')) return;
    if (k == 'c') {
        fprintf(stderr,
            "pascal: ISO 7185 6.8.2.2 violation: the variable '%s' is an array and is given a char value, which is not assignment-compatible with it; a character-string of one character is of the ch"
            "ar-type (6.1.7, 6.4.6)\n", vn);
        g_lower.pas.sem.pas_iso_errors++;
        return;
    }
    long long len = (long long)strlen(rhs->v.sval);
    if (g_lower.pas.sem.pas_chararrs[i].nostr) {
        fprintf(stderr,
            "pascal: ISO 7185 6.4.3.2 violation: the variable '%s' is given a character-string of %lld characters but is not of a string-type, which is a packed array [1..n] of char with n > 1; a ch"
            "aracter-string is assignment-compatible only with a string-type (6.4.6, 6.8.2.2)\n", vn, len);
        g_lower.pas.sem.pas_iso_errors++;
    } else if (g_lower.pas.sem.pas_chararrs[i].n >= 0 && len != g_lower.pas.sem.pas_chararrs[i].n) {
        fprintf(stderr,
            "pascal: ISO 7185 6.4.6 violation: the variable '%s' is a string-type of %lld components and is given a character-string of %lld characters; assignment-compatible string-types have the s"
            "ame number of components (6.8.2.2)\n", vn, g_lower.pas.sem.pas_chararrs[i].n, len);
        g_lower.pas.sem.pas_iso_errors++;
    }
}
static int pas_boolfam_width(const char *tn0) {
    const char *tn = tn0;
    int guard = 0;
    while (tn) {
        if (!strcmp(tn, "bytebool")) return 1;
        if (!strcmp(tn, "wordbool")) return 2;
        if (!strcmp(tn, "longbool")) return 4;
        if (!strcmp(tn, "qwordbool")) return 8;
        const char *al = pas_typealias_get(tn);
        if (!al || !strcmp(al, tn) || guard++ >= 8) break;
        tn = al;
    }
    return 0;
}
static int pas_var_is_boolfam(const char *name) { return name ? pas_boolfam_width(pas_scalarvartype_get(name)) > 0 : 0; }
static char *pas_pchar_off_name(const char *n) { size_t l = strlen(n) + 12; char *h = (char *)ct_zalloc(l, 1); snprintf(h, l, "__pas_po_%s", n); return h; }
static int pas_pchar_castname(tree_t *e) {
    if (!e || e->t != TT_FNC || e->n != 2 || !e->c[0] || !e->c[0]->v.sval) return 0;
    const char *f = e->c[0]->v.sval;
    return !strcmp(f, "pchar") || !strcmp(f, "pwidechar") || !strcmp(f, "pansichar");
}
static tree_t *pas_pchar_assign(tree_t *sel, tree_t *rhs) {
    if (!sel || sel->t != TT_VAR || !sel->v.sval || !pas_is_pcharvar(sel->v.sval) || !rhs) return NULL;
    tree_t *in = pas_pchar_castname(rhs) ? rhs->c[1] : rhs;
    tree_t *base = NULL, *off = NULL;
    if (in->t == TT_IDX && in->n == 2 && in->c[0] && in->c[0]->t == TT_VAR && in->c[0]->v.sval && pas_var_string_kind(in->c[0]->v.sval)) {
        base = in->c[0];
        off = bin(TT_SUB, in->c[1], ilit(1));
    } else if (in->t == TT_VAR && in->v.sval && pas_is_pcharvar(in->v.sval)) {
        base = in;
        off = leaf_s(TT_VAR, pas_pchar_off_name(in->v.sval));
    } else if (in->t == TT_VAR && in->v.sval && pas_var_string_kind(in->v.sval)) {
        base = in;
        off = ilit(0);
    } else if (in->t == TT_QLIT) {
        base = in;
        off = ilit(0);
    }
    if (!base) return NULL;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    ast_push(q, bin(TT_ASSIGN, sel, pas_tree_clone(base)));
    ast_push(q, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_pchar_off_name(sel->v.sval)), off));
    return q;
}
static tree_t *pas_pchar_deref(tree_t *p) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, p); ast_push(e, bin(TT_ADD, leaf_s(TT_VAR, pas_pchar_off_name(p->v.sval)), ilit(1))); return e; }
static long long pas_rectype_field_ov(const char *rn, int fi) {
    if (!rn || fi < 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rn)) return fi < g_lower.pas.sem.pas_rectypes[i]
        .nf ? g_lower.pas.sem.pas_rectypes[i].fldov[fi] : 0;
    return 0;
}
static long long pas_recvar_field_ov(const char *vn, int fi) {
    if (!vn || fi < 0) return 0;
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, vn)) return fi < g_lower.pas.sem.pas_recvars[i]
        .nf ? g_lower.pas.sem.pas_recvars[i].fldov[fi] : 0;
    return 0;
}
static long long pas_recvar_field_na_lo(const char *vn, int fi) {
    if (!vn || fi < 0) return 0;
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, vn)) return fi < g_lower.pas.sem.pas_recvars[i]
        .nf ? g_lower.pas.sem.pas_recvars[i].fldna_lo[fi] : 0;
    return 0;
}
static tree_t *pas_region_wrap(tree_t *e, long long ov, long long lo) {
    int cls = (int)pas_rk_get(ov, 0), es = (int)pas_rk_get(ov, 1), ecls = (int)pas_rk_get(ov, 2);
    long long off = pas_rk_get(ov, 3), rsz = (ov >> 48) & 0xFFFF;
    tree_t *f = ast_node_new(TT_FNC);
    ast_push(f, leaf_s(TT_VAR, cls == 7 ? "__pas_rarr" : "__pas_rdecode"));
    ast_push(f, e);
    ast_push(f, ilit(off));
    ast_push(f, ilit(cls == 7 ? (long long)(ecls | (es << 8)) : (long long)(cls | (es << 8))));
    if (cls == 7) ast_push(f, ilit(lo));
    ast_push(f, ilit(rsz));
    return f;
}
static tree_t *pas_field_region(tree_t *base, const char *rt, int fi, tree_t *e) {
    const char *rt2 = rt ? rt : pas_with_sel_rtype(base);
    long long ov = 0, lo = 0;
    if (rt2) ov = pas_rectype_field_ov(rt2, fi);
    else if (base && base->t == TT_VAR && base->v.sval) ov = pas_recvar_field_ov(base->v.sval, fi);
    if (!(((ov >> 48) & 0xFFFF) != 0 && pas_rk_get(ov, 0) != 0)) return e;
    if (pas_rk_get(ov, 0) == 7) lo = rt2 ? (pas_rectype_field_is_ca(rt2, fi) ? pas_rectype_field_ca_lo(rt2, fi) : pas_rectype_field_na_lo(rt2, fi)) :
        pas_recvar_field_na_lo(base && base->t == TT_VAR ? base->v.sval : NULL, fi);
    return pas_region_wrap(e, ov, lo);
}
static tree_t *pas_arrrec_region(tree_t *e, const char *an, int fi) {
    const char *rn = NULL;
    long long ov = 0, lo = 0;
    if (!e || !an || fi < 0 || fi >= 32 || pas_arrrec_find(an, &rn) <= 0) return e;
    for (int i = 0; i < g_lower.pas.sem.pas_narrrec; i++) if (g_lower.pas.sem.pas_arrrecs[i].aname && !strcmp(g_lower.pas.sem.pas_arrrecs[i].aname, an)) {
        ov = fi < g_lower.pas.sem.pas_arrrecs[i].nf ? g_lower.pas.sem.pas_arrrecs[i].fldov[fi] : 0;
        lo = g_lower.pas.sem.pas_arrrecs[i].fldna_lo[fi];
        break;
    }
    if (rn && pas_rectype_nf(rn) > fi) { long long ov2 = pas_rectype_field_ov(rn, fi); if (ov2) ov = ov2; }
    if (!(((ov >> 48) & 0xFFFF) != 0 && pas_rk_get(ov, 0) != 0)) return e;
    if (pas_rk_get(ov, 0) == 7 && rn) lo = pas_rectype_field_is_ca(rn, fi) ? pas_rectype_field_ca_lo(rn, fi) : pas_rectype_field_na_lo(rn, fi);
    return pas_region_wrap(e, ov, lo);
}
static tree_t *pas_region_elem(tree_t *f, tree_t *idx) {
    long long ek = f->c[3]->v.ival, es = (ek >> 8) & 0xFF, off = f->c[2]->v.ival, lo = f->c[4]->v.ival, rsz = f->c[5]->v.ival;
    tree_t *d = ast_node_new(TT_FNC);
    ast_push(d, leaf_s(TT_VAR, "__pas_rdecode"));
    ast_push(d, f->c[1]);
    ast_push(d, bin(TT_ADD, ilit(off), bin(TT_MUL, bin(TT_SUB, idx, ilit(lo)), ilit(es))));
    ast_push(d, ilit(ek));
    ast_push(d, ilit(rsz));
    return d;
}
static tree_t *mk_assign(tree_t *sel, tree_t *rhs) {
    if (sel && sel->t == TT_FNC && sel->n == 5 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_rdecode") && sel->c[1]) {
        tree_t *e0 = sel->c[1];
        tree_t *pt = ast_node_new(TT_FNC);
        ast_push(pt, leaf_s(TT_VAR, "__pas_rpatch"));
        ast_push(pt, pas_tree_clone(e0));
        ast_push(pt, sel->c[2]);
        ast_push(pt, sel->c[3]);
        ast_push(pt, sel->c[4]);
        ast_push(pt, rhs);
        return mk_assign(e0, pt);
    }
    { tree_t *_pc = pas_pchar_assign(sel, rhs); if (_pc) return _pc; }
    { const char *_abn = pas_selector_base_name(sel); if (_abn) pas_assigned_add(_abn); }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[1] && sel->c[1]->t == TT_ILIT && rhs && rhs->t != TT_QLIT && pas_is_charexpr(rhs)) {
        const char *_frt = pas_with_sel_rtype(sel->c[0]);
        const char *_ftn = _frt ? pas_rectype_field_typename(_frt, sel->c[1]->v.ival) : NULL;
        if (_ftn && pas_string_type_kind(_ftn)) {
            if (rhs->t == TT_FNC && rhs->n == 2 && rhs->c[0] && rhs->c[0]->v.sval && !strcmp(rhs->c[0]->v.sval, "__pas_chrlit") && rhs->c[1] && rhs->c[1]->t == TT_ILIT) {
                char *_cb = (char *)ct_zalloc(2, 1);
                _cb[0] = (char)rhs->c[1]->v.ival;
                tree_t *_q = ast_node_new(TT_QLIT);
                _q->v.sval = _cb;
                rhs = _q;
            } else rhs = mk_chr_wrap(rhs);
        }
    }
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs && rhs->t != TT_QLIT && pas_var_string_kind(sel->v.sval) && pas_is_charexpr(rhs)) rhs = mk_chr_wrap(rhs);
    if (sel && sel->t == TT_FNC && sel->n == 1 && sel->c[0] && sel->c[0]->t == TT_VAR && sel->c[0]->v.sval && rhs && rhs->t != TT_QLIT && pas_func_returns_stringish(sel->c[0]->v.sval) &&
        pas_is_charexpr(rhs)) rhs = pas_concat_operand(rhs);
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs && rhs->t != TT_FLIT && pas_var_is_real(sel->v.sval)) rhs = bin(TT_ADD, rhs, flit(0.0));
    if (sel && sel->t == TT_VAR && sel->v.sval && rhs) {
        int _dbf = pas_var_is_boolfam(sel->v.sval);
        int _sbf = (rhs->t == TT_VAR && rhs->v.sval) ? pas_var_is_boolfam(rhs->v.sval) : 0;
        if (_dbf && !_sbf && pas_is_boolexpr(rhs)) rhs = bin(TT_SUB, ilit(0), rhs);
        else if (!_dbf && _sbf) rhs = bin(TT_NE, rhs, ilit(0));
    }
    if (pas_is_aggregate_designator(rhs)) rhs = mk_fnc1("__pas_arr_copy", rhs);
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && pas_is_cafield(sel->c[0]) && sel->c[0]->t == TT_IDX && sel->c[0]->n >= 2 && sel->c[0]->c[0] && sel->c[0]->c[0]->t == TT_FNC &&
        sel->c[0]->c[0]->n >= 2 && sel->c[0]->c[0]->c[0] && sel->c[0]->c[0]->c[0]->v.sval && !strcmp(sel->c[0]->c[0]->c[0]->v.sval, "__pas_deref")) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_field_idx_set"));
        ast_push(e, sel->c[0]->c[0]->c[1]);
        ast_push(e, sel->c[0]->c[1]);
        ast_push(e, sel->c[1]);
        ast_push(e, rhs);
        return e;
    }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && pas_is_cafield(sel->c[0]) && sel->c[0]->t == TT_IDX && sel->c[0]->n >= 2 && sel->c[0]->c[0] && sel->c[0]->c[0]->t == TT_VAR) {
        tree_t *base = sel->c[0]->c[0];
        tree_t *upd = ast_node_new(TT_FNC);
        ast_push(upd, leaf_s(TT_VAR, "__pas_field_idx_update"));
        ast_push(upd, pas_tree_clone(base));
        ast_push(upd, pas_tree_clone(sel->c[0]->c[1]));
        ast_push(upd, sel->c[1]);
        ast_push(upd, rhs);
        return bin(TT_ASSIGN, base, upd);
    }
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_FNC && sel->c[0]->n >= 2 && sel->c[0]->c[0] && sel->c[0]->c[0]->v.sval &&
        !strcmp(sel->c[0]->c[0]->v.sval, "__pas_deref")) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_field_set"));
        ast_push(e, sel->c[0]->c[1]);
        ast_push(e, sel->c[1]);
        ast_push(e, rhs);
        return e;
    }
    return bin(TT_ASSIGN, sel, rhs);
}
static tree_t *mk_ident(const char *name) {
    if (pas_pf_is_formal(name) && pas_pf_lookup(name)->sig[0] == 'F') return pas_pf_callthrough(name, NULL);
    if (name && !strcmp(name, "result") && !pas_is_declared_local_here(name)) { const char *cf = pas_curfunc_name(); if (cf) return leaf_s(TT_VAR, cf); }
    if (name && !strcmp(name, "true")) return bin(TT_EQ, ilit(1), ilit(1));
    if (name && !strcmp(name, "false")) return bin(TT_EQ, ilit(0), ilit(1));
    if (name && !strcmp(name, "nil")) return ilit(0);
    if (name && !strcmp(name, "hinstance")) return ilit(0);
    if (name && !strcmp(name, "eof")) return mk_fnc0("__pas_eof");
    if (name && !strcmp(name, "eoln")) return mk_fnc0("__pas_eoln");
    long long cv;
    if (pas_const_get(name, &cv)) return pas_is_charvar(name) ? mk_fnc1("__pas_chrlit", ilit(cv)) : ilit(cv);
    if (name && !strcmp(name, "maxint") && !pas_scalarvartype_get(name) && !pas_is_func(name)) return ilit(2147483647);
    double rv;
    if (pas_rconst_get(name, &rv)) return flit(rv);
    const char *sv = pas_sconst_get(name);
    if (sv) return leaf_s(TT_QLIT, sv);
    if (pas_is_func(name)) return mk_call(name, NULL);
    for (int wi = g_lower.pas.sem.with_depth - 1; wi >= 0; wi--) {
        tree_t *wsel = g_lower.pas.sem.with_stk[wi].sel;
        const char *rt = g_lower.pas.sem.with_stk[wi].rtype;
        int fi = -1;
        if (rt) fi = pas_with_field_index(rt, name);
        if (fi < 0 && wsel && wsel->t == TT_VAR && wsel->v.sval) fi = pas_with_recvar_field(wsel->v.sval, name);
        if (fi < 0 && wsel && wsel->t == TT_IDX && wsel->n == 2 && wsel->c[0] && wsel->c[0]->t == TT_VAR && wsel->c[0]->v.sval) fi = pas_arrrec_field_index(wsel->c[0]->v.sval, name);
        if (fi >= 0) {
            if (wsel && wsel->t == TT_IDX && wsel->n == 2 && wsel->c[0] && wsel->c[0]->t == TT_VAR && pas_arrrec_find(wsel->c[0]->v.sval, NULL) > 0) {
                tree_t *_af = pas_arrrec_flatten(pas_tree_clone(wsel), fi);
                if (pas_arrrec_field_is_char(wsel->c[0]->v.sval, fi)) pas_cvfield_mark_add(_af);
                return pas_arrrec_region(_af, wsel->c[0]->v.sval, fi);
            }
            tree_t *e = ast_node_new(TT_IDX);
            ast_push(e, pas_tree_clone(wsel));
            ast_push(e, ilit(rt ? pas_rectype_slot(rt, fi) : (wsel && wsel->t == TT_VAR && wsel->v.sval) ? pas_recvar_slot(wsel->v.sval, fi) : fi));
            {
                const char *_crt = rt ? rt : pas_with_sel_rtype(wsel);
                if (_crt && pas_rectype_field_is_ca(_crt, fi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_crt, fi), pas_rectype_field_ca_hi(_crt, fi));
                if (_crt && pas_rectype_field_is_char(_crt, fi)) pas_cvfield_mark_add(e);
            }
            {
                struct pas_vt *_vt = pas_vt_find_name(rt ? rt : pas_with_sel_rtype(wsel));
                if (!_vt && wsel && wsel->t == TT_VAR && wsel->v.sval) _vt = pas_vt_find_name(wsel->v.sval);
                if (_vt && _vt->tagfi >= 0 && fi == _vt->tagfi) pas_vt_new(8)->node = e;
            }
            return pas_field_region(wsel, rt, fi, e);
        }
    }
    pas_scope_require(name);
    return leaf_s(TT_VAR, name);
}
static void pas_scope_applied(const char *name) {
    for (int i = g_lower.pas.sem.pas_scope.n - 1; name && i >= 0; i--) if (g_lower.pas.sem.pas_scope.defs[i].name && !strcmp(g_lower.pas.sem.pas_scope.defs[i].name, name)) {
        if (g_lower.pas.sem.pas_scope.defs[i].depth < g_lower.pas.sem.pas_scope.depth)
            pas_scope_push(&g_lower.pas.sem.pas_scope.apl, &g_lower.pas.sem.pas_scope.napl, &g_lower.pas.sem.pas_scope.capl, name);
        return;
    }
}
static void pas_scope_defined_after_use(const char *name) {
    if (g_lower.pas.sem.pas_recbody_depth > 0) return;
    for (int i = g_lower.pas.sem.pas_scope.napl - 1; i >= 0 && g_lower.pas.sem.pas_scope.apl[i].depth == g_lower.pas.sem.pas_scope.depth; i--) if (!strcmp(g_lower.pas.sem.pas_scope.apl[i].name, name))
        {
        fprintf(stderr, "pascal: ISO 7185 6.2.2.9 violation: '%s' is used in this block before its defining-point in this block, which shall precede every applied occurrence\n", name);
        g_lower.pas.sem.pas_iso_errors++;
        return;
    }
}
static void pas_scope_require(const char *name) {
    pas_scope_applied(name);
    if (pas_scope_has(name)) return;
    fprintf(stderr, "pascal: ISO 7185 6.2.2.1 violation: the identifier '%s' has no defining-point -- it is declared nowhere in a region enclosing this use\n", name);
    g_lower.pas.sem.pas_iso_errors++;
}
static int pas_is_rel(tree_t *e) { if (!e) return 0; switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; } }
static void pas_not_a_word_symbol(const char *n) {
    const char *ws[] = { "and", "array", "begin", "case", "const", "div", "do", "downto", "else", "end", "file", "for", "function", "goto", "if", "in", "label", "mod", "nil", "not", "of", "or",
        "packed", "procedure", "program", "record", "repeat", "set", "then", "to", "type", "until", "var", "while", "with" };
    for (size_t i = 0; n && i < sizeof ws / sizeof ws[0]; i++) if (!strcmp(n, ws[i])) {
        fprintf(stderr, "pascal: ISO 7185 6.1.2 violation: '%s' is a word-symbol and cannot be defined as an identifier\n", n);
        g_lower.pas.sem.pas_iso_errors++;
        return;
    }
}
static void pas_label_in_range(long long v) {
    if (v < 0 || v > 9999) { fprintf(stderr, "pascal: ISO 7185 6.1.6 violation: the label %lld is not in the closed interval 0 to 9999\n", v); g_lower.pas.sem.pas_iso_errors++; }
}
static void pas_label_declare(long long v) {
    char b[24];
    snprintf(b, sizeof b, "%lld", v);
    pas_scope_push(&g_lower.pas.sem.pas_scope.lab, &g_lower.pas.sem.pas_scope.nlab, &g_lower.pas.sem.pas_scope.clab, ct_strdup(b));
}
static PasDef *pas_label_find(long long v) {
    char b[24];
    snprintf(b, sizeof b, "%lld", v);
    for (int i = g_lower.pas.sem.pas_scope.nlab - 1; i >= 0; i--) if (!strcmp(g_lower.pas.sem.pas_scope.lab[i].name, b)) return &g_lower.pas.sem.pas_scope.lab[i];
    fprintf(stderr, "pascal: ISO 7185 6.2.2.1 violation: the label %lld has no defining-point -- no label-declaration-part of this block or of an enclosing block declares it\n", v);
    g_lower.pas.sem.pas_iso_errors++;
    return NULL;
}
static void pas_label_prefix(long long v) { PasDef *d = pas_label_find(v); if (d && d->depth == g_lower.pas.sem.pas_scope.depth) d->formal++; }
static void pas_labels_close(void) {
    while (g_lower.pas.sem.pas_scope.nlab > 0 && g_lower.pas.sem.pas_scope.lab[g_lower.pas.sem.pas_scope.nlab - 1].depth >= g_lower.pas.sem.pas_scope.depth) {
        PasDef *d = &g_lower.pas.sem.pas_scope.lab[--g_lower.pas.sem.pas_scope.nlab];
        if (d->formal == 1) continue;
        fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: the label %s prefixes %d statements of the block that declares it, which shall closest-contain exactly one\n", d->name, d->formal);
        g_lower.pas.sem.pas_iso_errors++;
    }
}
static void pas_real_is_not_ordinal(double v) {
    if (g_lower.pas.sem.pas_case_depth > 0) fprintf(stderr, "pascal: ISO 7185 6.8.3.5 violation: the case-constant %g is of type real, which is not an ordinal-type\n", v);
    else fprintf(stderr, "pascal: ISO 7185 6.4.2.4 violation: the constant %g is of type real, and a subrange bound or a variant's case-constant (6.4.3.3) shall be of an ordinal-type\n", v);
    g_lower.pas.sem.pas_iso_errors++;
}
static void pas_program_params_distinct(PNodeList *ids) {
    for (int i = 0; ids && i < ids->count; i++) for (int j = 0; j < i; j++)
        if (ids->items[i] && ids->items[j] && ids->items[i]->v.sval && ids->items[j]->v.sval && !strcmp(ids->items[i]->v.sval, ids->items[j]->v.sval)) {
        fprintf(stderr, "pascal: ISO 7185 6.10 violation: the program-parameter '%s' appears more than once in the program-heading\n", ids->items[i]->v.sval);
        g_lower.pas.sem.pas_iso_errors++;
        break;
    }
}
static PNodeList *pas_var_names_add(PNodeList *seen, PNodeList *ids) {
    if (ids) for (int i = 0; i < ids->count; i++) {
        const char *n = ids->items[i] ? ids->items[i]->v.sval : NULL;
        if (!n) continue;
        for (int j = 0; j < seen->count; j++) if (seen->items[j] && seen->items[j]->v.sval && !strcmp(seen->items[j]->v.sval, n)) {
            fprintf(stderr, "pascal: ISO 7185 6.2.2.7 violation: the identifier '%s' has two defining-points in one variable-declaration-part\n", n);
            g_lower.pas.sem.pas_iso_errors++;
            break;
        }
        pnl_push(seen, ids->items[i]);
    }
    return seen;
}
static tree_t *pas_for_once(tree_t *e) {
    if (!e || e->t != TT_FOR || e->n < 4 || !e->c[1] || !e->c[2]) return e;
    int kfrom = e->c[1]->t == TT_ILIT || e->c[1]->t == TT_QLIT, kto = e->c[2]->t == TT_ILIT || e->c[2]->t == TT_QLIT;
    if (kto) return e;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    if (!kfrom) {
        char _b[24];
        snprintf(_b, sizeof _b, "__pas_fl%d", g_lower.pas.sem.fln++);
        const char *_n = ct_strdup(_b);
        pas_scope_define(_n);
        pas_local_add(_n);
        ast_push(q, mk_assign(leaf_s(TT_VAR, _n), e->c[1]));
        e->c[1] = leaf_s(TT_VAR, _n);
    }
    char _b2[24];
    snprintf(_b2, sizeof _b2, "__pas_fl%d", g_lower.pas.sem.fln++);
    const char *_n2 = ct_strdup(_b2);
    pas_scope_define(_n2);
    pas_local_add(_n2);
    ast_push(q, mk_assign(leaf_s(TT_VAR, _n2), e->c[2]));
    e->c[2] = leaf_s(TT_VAR, _n2);
    ast_push(q, e);
    return q;
}
static void pas_for_const_bounds(const char *cv, tree_t *from, tree_t *to, int down) {
    if (!cv || !from || !to || from->t != TT_ILIT || to->t != TT_ILIT) return;
    long long lo = 0, hi = -1, a = from->v.ival, b = to->v.ival;
    int n = 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nsubvar; i++) if (g_lower.pas.sem.pas_subvars[i].name && !strcmp(g_lower.pas.sem.pas_subvars[i].name, cv)) {
        if (n && (g_lower.pas.sem.pas_subvars[i].low != lo || g_lower.pas.sem.pas_subvars[i].high != hi)) return;
        lo = g_lower.pas.sem.pas_subvars[i].low;
        hi = g_lower.pas.sem.pas_subvars[i].high;
        n++;
    }
    if (!n) return;
    for (int i = 0; i < g_lower.pas.sem.pas_nscalarvartype; i++)
        if (g_lower.pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower.pas.sem.pas_scalarvartype[i].vname, cv) && pas_subtype_high(g_lower.pas.sem.pas_scalarvartype[i].tname) < 0) return;
    if (down ? a < b : a > b) return;
    if (a >= lo && a <= hi && b >= lo && b <= hi) return;
    fprintf(stderr,
        "pascal: ISO 7185 6.8.3.9 violation: the for-statement over '%s' (of type %lld..%lld) runs from %lld to %lld, and a value outside the"
        " control-variable's type is not assignment-compatible with it\n", cv, lo, hi, a, b);
    g_lower.pas.sem.pas_iso_errors++;
}
static tree_t *pas_cond(tree_t *e) { return (pas_is_rel(e) || (e && (e->t == TT_CONJ || e->t == TT_ALT || e->t == TT_NOT))) ? e : bin(TT_NE, e, ilit(0)); }
static const char *pas_cond_var_nonbool_type(tree_t *e) {
    if (!e || e->t != TT_VAR || !e->v.sval || pas_is_boolvar(e->v.sval)) return NULL;
    const char *found = NULL;
    for (int i = 0; i < g_lower.pas.sem.pas_nscalarvartype; i++) if (g_lower.pas.sem.pas_scalarvartype[i].vname && !strcmp(g_lower.pas.sem.pas_scalarvartype[i].vname, e->v.sval)) {
        const char *t = g_lower.pas.sem.pas_scalarvartype[i].tname;
        for (int guard = 0; t && guard < 8; guard++) { const char *al = pas_typealias_get(t); if (!al || !strcmp(al, t)) break; t = al; }
        if (!t || pas_is_booltype(t) || !(pas_enumtype_high(t) >= 0 || pas_is_int_typename(t) || pas_is_realtypename(t) || !strcmp(t, "char"))) return NULL;
        found = g_lower.pas.sem.pas_scalarvartype[i].tname;
    }
    return found;
}
static tree_t *pas_cond_bool(tree_t *e, const char *stmt, const char *clause) {
    const char *t = pas_cond_var_nonbool_type(e);
    if (t) {
        fprintf(stderr, "pascal: ISO 7185 %s violation: the Boolean-expression of %s is the variable '%s' of type %s, which is not the required-type Boolean\n", clause, stmt, e->v.sval, t);
        g_lower.pas.sem.pas_iso_errors++;
    }
    return pas_cond(e);
}
static tree_t *pas_bool(tree_t *e) { return e; }
static tree_t *pas_flip_rel(tree_t *e) {
    switch (e->t) {
        case TT_LT:
        e->t = TT_GE;
        break;
        case TT_GE:
        e->t = TT_LT;
        break;
        case TT_LE:
        e->t = TT_GT;
        break;
        case TT_GT:
        e->t = TT_LE;
        break;
        case TT_EQ:
        e->t = TT_NE;
        break;
        case TT_NE:
        e->t = TT_EQ;
        break;
        case TT_CONJ:
        case TT_ALT:
        { tree_t *n = ast_node_new(TT_NOT); ast_push(n, e); return n; }
        default:
        break;
    }
    return e;
}
static tree_t *pas_str_to_alpha(const char *s, long long lo, long long high) {
    tree_t *q = ast_node_new(TT_QLIT);
    q->v.sval = ct_strdup(s ? s : "");
    return mk_fnc3("__pas_ca_encode", q, ilit(lo < 0 ? 0 : lo), ilit(high));
}
static tree_t *pas_str_padded(const char *s, long long n) {
    size_t sl = s ? strlen(s) : 0;
    size_t len = (n > (long long) sl) ? (size_t) n : sl;
    char *buf = (char *) ct_alloc(len + 1);
    for (size_t k = 0; k < len; k++) buf[k] = (k < sl) ? s[k] : ' ';
    buf[len] = '\0';
    tree_t *q = ast_node_new(TT_QLIT);
    q->v.sval = buf;
    return q;
}
static int pas_array_is_pure_num(const char *name) {
    if (!name) return 0;
    if (pas_is_chararr(name)) return 0;
    if (pas_arrrec_find(name, NULL) > 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrecvar; i++) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, name)) return 0;
    return 1;
}
static int pas_array_is_real(const char *name) {
    if (pas_array_is_pure_num(name)) return 1;
    if (!name || pas_is_chararr(name) || pas_enumarr_get(name) || pas_arrrec_find(name, NULL) <= 0) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrecvar; i++) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, name)) return 0;
    return 1;
}
static long long pas_range_fit_size(long long lo, long long hi) {
    if (lo >= 0) { if (hi <= 255) return 1; if (hi <= 65535) return 2; return 4; }
    if (lo >= -128 && hi <= 127) return 1;
    if (lo >= -32768 && hi <= 32767) return 2;
    return 4;
}
static int pas_sizeof_builtin_size(const char *n, long long *out) {
    if (!n) return 0;
    static const struct {
        const char *n;
        long long sz;
    } T[] = { {"integer",4},{"longint",4},{"shortint",1},{"byte",1},{"word",2},{"smallint",2}, {"cardinal",4},{"longword",4},{"int64",8},{"qword",8},{"single",4},{"real",8}, {"double",8},{"extended",
        10},{"comp",8},{"char",1},{"widechar",1}, {"boolean",1},{"bytebool",1},{"wordbool",2},{"longbool",4},{"qwordbool",8},{"pointer",8}, {"ptruint",8},{"ptrint",8},{"int8",1},{"int16",2},{"int32",
        4},{"uint8",1}, {"uint16",2},{"uint32",4},{"uint64",8},{"nativeint",8},{"nativeuint",8}, {"codepointer",8}, };
    for (size_t i = 0; i < sizeof(T) / sizeof(T[0]); i++) if (!strcmp(n, T[i].n)) { *out = T[i].sz; return 1; }
    return 0;
}
static int pas_sizeof_lookup(const char *name, long long *out) {
    if (!name) return 0;
    if (pas_sizeof_builtin_size(name, out)) return 1;
    if (pas_arrtype_high(name) >= 0 && pas_tsz_get(name, out)) return 1;
    { const char *al = pas_typealias_get(name); if (al && strcmp(al, name)) return pas_sizeof_lookup(al, out); }
    {
        long long eh = pas_enumtype_high(name);
        if (eh >= 0) { long long ms; if (!pas_enumtype_min_size(name, &ms)) ms = 4; long long fit = pas_range_fit_size(0, eh); *out = fit > ms ? fit : ms; return 1; }
    }
    if (pas_is_settype(name)) { long long pk = pas_settype_pack(name); if (pk > 0) { *out = pk; return 1; } long long sh = pas_settype_hi(name); *out = (sh >= 0 && sh <= 31) ? 4 : 32; return 1; }
    { long long sh = pas_subtype_high(name); if (sh >= 0) { *out = pas_range_fit_size(pas_subtype_low(name), sh); return 1; } }
    { long long lo, hi; if (pas_subvar_get(name, &lo, &hi)) { *out = pas_range_fit_size(lo, hi); return 1; } }
    if (pas_rectype_nf(name) > 0 && pas_rectype_total_size(name, out)) return 1;
    for (int i = 0; i < g_lower.pas.sem.pas_nrecvar; i++) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, name)) {
        const char *rt = pas_with_sel_rtype(leaf_s(TT_VAR, name));
        return (rt && pas_rectype_total_size(rt, out)) ? 1 : 0;
    }
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) return pas_tsz_get(name, out);
    if (pas_is_charvar(name)) { *out = 1; return 1; }
    if (pas_is_boolvar(name)) { *out = 1; return 1; }
    if (pas_ptrvar_target(name)) { *out = 8; return 1; }
    { const char *tn = pas_scalarvartype_get(name); if (tn && strcmp(tn, name)) return pas_sizeof_lookup(tn, out); }
    return pas_tsz_get(name, out);
}
static int pas_ordinal_builtin_bound(const char *n, long long *lo, long long *hi) {
    if (!n) return 0;
    static const struct {
        const char *n;
        long long lo;
        long long hi;
    } T[] = { {"byte",0,255},{"shortint",-128,127},{"word",0,65535},{"smallint",-32768,32767}, {"longword",0,4294967295LL},{"cardinal",0,4294967295LL},{"uint32",0,4294967295LL}, {"longint",
        -2147483648LL,2147483647LL},{"integer",-2147483648LL,2147483647LL}, {"int32",-2147483648LL,2147483647LL},{"int8",-128,127},{"uint8",0,255}, {"int16",-32768,32767},{"uint16",0,65535}, {"int64",
        (long long)0x8000000000000000ULL,0x7FFFFFFFFFFFFFFFLL}, {"qword",0,-1},{"uint64",0,-1},{"nativeint",(long long)0x8000000000000000ULL,0x7FFFFFFFFFFFFFFFLL}, {"nativeuint",0,-1},{"char",0,255},
        {"widechar",0,255},{"boolean",0,1},{"bytebool",0,255}, };
    for (size_t i = 0; i < sizeof(T) / sizeof(T[0]); i++) if (!strcmp(n, T[i].n)) { *lo = T[i].lo; *hi = T[i].hi; return 1; }
    return 0;
}
static int pas_ordinal_bound_lookup(const char *name, long long *lo, long long *hi) {
    if (!name) return 0;
    if (pas_ordinal_builtin_bound(name, lo, hi)) return 1;
    { const char *al = pas_typealias_get(name); if (al && strcmp(al, name)) return pas_ordinal_bound_lookup(al, lo, hi); }
    { long long sh = pas_subtype_high(name); if (sh >= 0) { *lo = pas_subtype_low(name); *hi = sh; return 1; } }
    if (pas_subvar_get(name, lo, hi)) return 1;
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) return 0;
    for (int i = 0; i < g_lower.pas.sem.pas_nrecvar; i++) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, name)) return 0;
    if (pas_is_charvar(name)) { *lo = 0; *hi = 255; return 1; }
    if (pas_is_boolvar(name)) { *lo = 0; *hi = 1; return 1; }
    { const char *tn = pas_scalarvartype_get(name); if (tn && strcmp(tn, name)) return pas_ordinal_bound_lookup(tn, lo, hi); }
    return 0;
}
static tree_t *mk_rec_block(int nf, char *const *fldrec, const int *fldna, const long long *na_lo, const long long *na_hi, int depth) {
    tree_t *e = mk_fnc1("arr_make", ilit(nf > 0 ? nf - 1 : 0));
    for (int j = 0; j < nf && depth < 16; j++) {
        tree_t *sub = NULL;
        if (fldna[j] && na_hi[j] >= na_lo[j] && fldrec[j]) sub = mk_fnc3("__pas_arr_of", ilit(na_lo[j]), ilit(na_hi[j]), mk_rec_init_type_d(fldrec[j], depth + 1));
        else if (fldna[j] && na_hi[j] >= na_lo[j]) sub = mk_fnc2("arr_make", ilit(na_lo[j]), ilit(na_hi[j]));
        else if (!fldna[j] && fldrec[j]) sub = mk_rec_init_type_d(fldrec[j], depth + 1);
        if (sub) e = mk_fnc3("arr_set_pure", e, ilit(j), sub);
    }
    return e;
}
static tree_t *mk_rec_init_type_d(const char *rt, int depth) {
    for (int i = 0; rt && i < g_lower.pas.sem.pas_nrectype; i++) if (g_lower.pas.sem.pas_rectypes[i].tname && !strcmp(g_lower.pas.sem.pas_rectypes[i].tname, rt))
        return mk_rec_block(g_lower.pas.sem.pas_rectypes[i].nf, g_lower.pas.sem.pas_rectypes[i].fldrec, g_lower.pas.sem.pas_rectypes[i].fldna, g_lower.pas.sem.pas_rectypes[i].fldna_lo,
        g_lower.pas.sem.pas_rectypes[i].fldna_hi, depth);
    return mk_fnc1("arr_make", ilit(0));
}
static tree_t *mk_rec_init_type(const char *rt) { return mk_rec_init_type_d(rt, 0); }
static tree_t *pas_new_target_init(const char *rt) {
    long long ah = pas_arrtype_high(rt);
    if (ah >= 0 && !pas_arrtype_ischar(rt) && pas_arrtype_ncols(rt) < 0) return pas_rectype_nf(rt) > 0 ? mk_fnc3("__pas_arr_of", ilit(pas_arrtype_lo(rt)), ilit(ah), mk_rec_init_type(rt)) :
        mk_fnc2("arr_make", ilit(pas_arrtype_lo(rt)), ilit(ah));
    return pas_rectype_nf(rt) > 0 ? mk_rec_init_type(rt) : ilit(0);
}
static tree_t *mk_array_init(const char *name, long long high) {
    for (int i = g_lower.pas.sem.pas_nrecvar - 1; name && i >= 0; i--) if (g_lower.pas.sem.pas_recvars[i].vname && !strcmp(g_lower.pas.sem.pas_recvars[i].vname, name))
        return mk_rec_block(g_lower.pas.sem.pas_recvars[i].nf, g_lower.pas.sem.pas_recvars[i].fldrec, g_lower.pas.sem.pas_recvars[i].fldna, g_lower.pas.sem.pas_recvars[i].fldna_lo,
        g_lower.pas.sem.pas_recvars[i].fldna_hi, 0);
    for (int i = 0; name && i < g_lower.pas.sem.pas_narrrec; i++) if (g_lower.pas.sem.pas_arrrecs[i].aname && !strcmp(g_lower.pas.sem.pas_arrrecs[i].aname, name)) {
        tree_t *proto = (g_lower.pas.sem.pas_arrrecs[i].rname && pas_rectype_nf(g_lower.pas.sem.pas_arrrecs[i].rname) == g_lower.pas.sem.pas_arrrecs[i].nf) ?
            mk_rec_init_type(g_lower.pas.sem.pas_arrrecs[i].rname) :
            mk_rec_block(g_lower.pas.sem.pas_arrrecs[i].nf, g_lower.pas.sem.pas_arrrecs[i].fldrec, g_lower.pas.sem.pas_arrrecs[i].fldna, g_lower.pas.sem.pas_arrrecs[i].fldna_lo,
            g_lower.pas.sem.pas_arrrecs[i].fldna_hi, 0);
        return mk_fnc3("__pas_arr_of", ilit(pas_array_nonparam_low(name)), ilit(high), proto);
    }
    if (!pas_array_is_real(name)) return mk_fnc2("arr_make", ilit(pas_is_chararr(name) ? pas_chararr_lo(name) : 0), ilit(high));
    for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (g_lower.pas.sem.pas_arrays[i].name && !g_lower.pas.sem.pas_arrays[i].is_param && !strcmp(g_lower.pas.sem.pas_arrays[i].name, name)) {
        if (g_lower.pas.sem.pas_arrays[i].ncols < 0 && g_lower.pas.sem.pas_arrays[i].low != 0 && g_lower.pas.sem.pas_arrays[i].low <= high)
            return mk_fnc2("arr_make", ilit(g_lower.pas.sem.pas_arrays[i].low), ilit(high));
        break;
    }
    return mk_fnc1("arr_make", ilit(high));
}
static int pas_index_is_enum(long long v) {
    if (g_lower.pas.sem.pas_pend_sub_high >= 0 && g_lower.pas.sem.pas_pend_sub_high == v) return g_lower.pas.sem.pas_last_const_enum;
    if (g_lower.pas.sem.pas_pend_typename && pas_subtype_high(g_lower.pas.sem.pas_pend_typename) >= 0 && pas_subtype_high(g_lower.pas.sem.pas_pend_typename) == v)
        return pas_subtype_isenum(g_lower.pas.sem.pas_pend_typename);
    return 0;
}
static long long pas_index_bound(long long v, int want_hi) {
    long long lo = 0, hi = -1;
    if (g_lower.pas.sem.pas_pend_sub_high >= 0 && g_lower.pas.sem.pas_pend_sub_high == v) {
        lo = g_lower.pas.sem.pas_pend_sub_low;
        hi = v;
    } else if (g_lower.pas.sem.pas_pend_typename && pas_subtype_high(g_lower.pas.sem.pas_pend_typename) >= 0 && pas_subtype_high(g_lower.pas.sem.pas_pend_typename) == v) {
        lo = pas_subtype_low(g_lower.pas.sem.pas_pend_typename);
        hi = v;
    } else if (v >= 0) {
        lo = 0;
        hi = v;
    } else if (g_lower.pas.sem.pas_pend_typename) {
        long long bl, bh;
        if (pas_ordinal_bound_lookup(g_lower.pas.sem.pas_pend_typename, &bl, &bh) && bh >= bl && bh - bl < 65536) { lo = bl; hi = bh; }
    }
    return want_hi ? hi : lo;
}
static long long pas_decl_part_order(long long prev, long long cur) {
    const char *nm[] = { "", "label-declaration-part", "constant-definition-part", "type-definition-part", "variable-declaration-part", "procedure-and-function-declaration-part" };
    if (cur < prev) {
        fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: a block's %s shall precede its %s\n", nm[cur], nm[prev]);
        g_lower.pas.sem.pas_iso_errors++;
    } else if (cur == prev && cur < 5) {
        fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: a block has more than one %s\n", nm[cur]);
        g_lower.pas.sem.pas_iso_errors++;
    }
    return cur > prev ? cur : prev;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_sig_value_type(const char *sig, int idx, int var_too) {
    if (!sig || strlen(sig) < 3) return NULL;
    const char *p = sig + 2;
    int at = 0;
    while (*p && *p != ')') {
        const char *q = p;
        int dep = 0;
        while (*q && !(dep == 0 && (*q == ';' || *q == ')'))) { if (*q == '(') dep++; else if (*q == ')') dep--; q++; }
        char k = *p;
        int cnt = (k == 'v' || k == 'r') ? atoi(p + 1) : 1;
        if (idx < at + cnt) {
            const char *c = (k == 'v' || (var_too && k == 'r')) ? memchr(p, ':', (size_t)(q - p)) : NULL;
            if (!c) return NULL;
            char *t = (char *)ct_zalloc(1, (size_t)(q - c));
            memcpy(t, c + 1, (size_t)(q - c - 1));
            return t;
        }
        at += cnt;
        p = (*q == ';') ? q + 1 : q;
    }
    return NULL;
}
static void pas_formal_subranges(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) {
        tree_t *id = params->items[i];
        if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 1);
        if (t && pas_subtype_high(t) >= 0) pas_subvar_add(id->v.sval, pas_subtype_low(t), pas_subtype_high(t));
    }
}
static void pas_formal_chararrs(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) {
        tree_t *id = params->items[i];
        if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 0);
        if (t && pas_arrtype_ischar(t)) { pas_chararr_add2(id->v.sval, pas_arrtype_lo(t)); pas_chararr_shape(id->v.sval, pas_arrtype_nostr(t), pas_arrtype_high(t)); }
    }
}
static void pas_formal_tfiles(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) {
        tree_t *id = params->items[i];
        if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 1);
        if (t && pas_is_tfiletype(t)) { pas_tfilevar_add(id->v.sval); pas_tfcomp_add(id->v.sval, 0, -1, 0); }
    }
}
static void pas_formal_strtypes(const char *sig, PNodeList *params) {
    for (int i = 0, fi = 0; params && i < params->count; i++) {
        tree_t *id = params->items[i];
        if (!id || !id->v.sval || !strncmp(id->v.sval, "__pas_pe", 8)) continue;
        const char *t = pas_sig_value_type(sig, fi++, 1);
        if (t && (pas_string_type_kind(t) || !strcmp(t, "currency"))) pas_scalarvartype_add(id->v.sval, t);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_value_actuals_check(const char *callee, const PasDef *cd, PNodeList *args) {
    for (int i = 0; i + 1 < args->count; i += 2) {
        const char *st = pas_sig_value_type(cd->sig, i / 2, 1);
        if (st && pas_string_type_kind(st) && args->items[i] && args->items[i]->t != TT_QLIT && pas_is_charexpr(args->items[i])) args->items[i] = pas_concat_operand(args->items[i]);
    }
    for (int i = 0; i + 1 < args->count; i += 2) {
        const char *t = pas_sig_value_type(cd->sig, i / 2, 0);
        tree_t *a = args->items[i];
        const char *at = NULL;
        const char *rt = t ? NULL : pas_sig_value_type(cd->sig, i / 2, 1);
        long long _lo, _hi;
        if (rt && pas_subtype_high(rt) < 0 && strcmp(rt, "char") && !pas_is_booltype(rt) && a && a->t == TT_VAR && a->v.sval && !pas_is_setvar(a->v.sval) && pas_subvar_get(a->v.sval, &_lo, &_hi)) {
            fprintf(stderr,
                "pascal: ISO 7185 6.6.3.3 violation: the actual-parameter %d of '%s' is a variable of the subrange type %lld..%lld, which is not the type %s of its variable formal-parameter\n",
                i / 2 + 1, callee, _lo, _hi, rt);
            g_lower.pas.sem.pas_iso_errors++;
            continue;
        }
        if (!t || !a || (strcmp(t, "integer") && strcmp(t, "real") && strcmp(t, "char") && strcmp(t, "boolean"))) continue;
        if (a->t == TT_FNC && a->n == 2 && a->c[0] && a->c[0]->v.sval && !strcmp(a->c[0]->v.sval, "__pas_chrlit")) at = "char";
        else if (a->t == TT_FLIT) at = "real";
        else if (a->t == TT_QLIT && a->v.sval && strlen(a->v.sval) > 1) at = "string";
        else if (a->t == TT_ILIT && !strcmp(t, "char")) at = "non-char ordinal";
        if (!at || !strcmp(at, t)) continue;
        fprintf(stderr, "pascal: ISO 7185 6.6.3.2 violation: the actual-parameter %d of '%s' is a %s value, which is not assignment-compatible with the type %s of its value formal-parameter\n",
            i / 2 + 1, callee, at, t);
        g_lower.pas.sem.pas_iso_errors++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_value_formal_file_check(PNodeList *ids, const char *t) {
    if (!t || !(!strcmp(t, "text") || pas_is_tfiletype(t) || pas_rectype_has_file(t))) return;
    for (int i = 0; ids && i < ids->count; i++) if (ids->items[i] && ids->items[i]->v.sval) {
        fprintf(stderr,
            "pascal: ISO 7185 6.6.3.2 violation: the value formal-parameter '%s' has the type %s, which is or contains a file-type, so no actual-parameter is assignment-compatible with it\n",
            ids->items[i]->v.sval, t);
        g_lower.pas.sem.pas_iso_errors++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pas_pf_cat3(const char *a, const char *b, const char *c) {
    size_t la = a ? strlen(a) : 0, lb = b ? strlen(b) : 0, lc = c ? strlen(c) : 0;
    char *s = (char *)ct_zalloc(1, la + lb + lc + 1);
    if (la) memcpy(s, a, la);
    if (lb) memcpy(s + la, b, lb);
    if (lc) memcpy(s + la + lb, c, lc);
    return s;
}
static const char *pas_pf_canon(const char *t) { for (int g = 0; t && g < 16 && pas_subtype_high(t) < 0; g++) { const char *a = pas_typealias_get(t); if (!a) break; t = a; } return t ? t : ""; }
static char *pas_pf_sect(char k, int n, const char *t) { char nb[24]; snprintf(nb, sizeof nb, "%c%d:", k, n); return pas_pf_cat3(nb, pas_pf_canon(t), NULL); }
static char *pas_pf_sig(char k, const char *params, const char *rtype) { char *s = pas_pf_cat3(k == 'F' ? "F(" : "P(", params, ")"); return k == 'F' ? pas_pf_cat3(s, ":", pas_pf_canon(rtype)) : s; }
static const char *pas_pf_env_name(const char *formal, int k) { char b[16]; snprintf(b, sizeof b, "__pas_pe%d_", k); return pas_pf_cat3(b, formal, NULL); }
static void pas_pf_pend(const char *formal, const char *sig) { pas_scope_push(&g_lower.pas.sem.pas_scope.pend, &g_lower.pas.sem.pas_scope.npend, &g_lower.pas.sem.pas_scope.cpend, formal)->sig = sig; }
static void pas_pf_section(char k, PNodeList *ids, const char *t) { pas_pf_pend(NULL, pas_pf_sect(k, ids ? ids->count : 0, t)); }
static PNodeList *pas_pf_formal(const char *name, const char *sig) {
    pas_not_a_word_symbol(name);
    pas_pf_pend(NULL, sig);
    pas_pf_pend(name, sig);
    PNodeList *l = pnl_new();
    pnl_push(l, leaf_s(TT_VAR, name));
    for (int k = 1; k <= 3; k++) pnl_push(l, leaf_s(TT_VAR, pas_pf_env_name(name, k)));
    return l;
}
static const char *pas_pf_heading(const char *name, PNodeList *params, char k, const char *rtype) {
    if ((!params || params->count == 0) && name) for (int i = g_lower.pas.sem.pas_scope.nfwd - 1; i >= 0; i--)
        if (g_lower.pas.sem.pas_scope.fwd[i].name && g_lower.pas.sem.pas_scope.fwd[i].sig && !strcmp(g_lower.pas.sem.pas_scope.fwd[i].name, name)) {
        g_lower.pas.sem.pas_scope.npend = 0;
        return g_lower.pas.sem.pas_scope.fwd[i].sig;
    }
    const char *ps = "";
    for (int i = 0; i < g_lower.pas.sem.pas_scope.npend; i++) {
        PasDef *d = &g_lower.pas.sem.pas_scope.pend[i];
        if (!d->name) ps = pas_pf_cat3(ps, *ps ? ";" : "", d->sig);
        else if (name) { PasDef *f = pas_scope_push(&g_lower.pas.sem.pas_scope.fsig, &g_lower.pas.sem.pas_scope.nfsig, &g_lower.pas.sem.pas_scope.cfsig, d->name); f->sig = d->sig; f->owner = name; }
    }
    g_lower.pas.sem.pas_scope.npend = 0;
    return pas_pf_sig(k, ps, rtype);
}
static void pas_pf_define_routine(const char *name, const char *sig) {
    if (name) {
        PasDef *d = pas_scope_push(&g_lower.pas.sem.pas_scope.defs, &g_lower.pas.sem.pas_scope.n, &g_lower.pas.sem.pas_scope.cap, name);
        d->sig = sig;
        d->rid = ++g_lower.pas.sem.pas_scope.nrid;
    }
}
static const PasDef *pas_pf_lookup(const char *name) {
    for (int i = g_lower.pas.sem.pas_scope.n - 1; name && i >= 0; i--) if (g_lower.pas.sem.pas_scope.defs[i].name && !strcmp(g_lower.pas.sem.pas_scope.defs[i].name, name))
        return g_lower.pas.sem.pas_scope.defs[i].sig ? &g_lower.pas.sem.pas_scope.defs[i] : NULL;
    return NULL;
}
static int pas_pf_is_formal(const char *name) { const PasDef *d = pas_pf_lookup(name); return d && d->formal; }
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
static int pas_pf_nactuals(const char *sig) { int n = 0; const char *f; char k; for (int i = 0; (k = pas_pf_param(sig, i, &f)) != 0; i++) n += (k == 'P' || k == 'F') ? 4 : 1; return n; }
static void pas_pf_candidate(const PasDef *d) {
    for (int i = 0; i < g_lower.pas.sem.pas_scope.ncand; i++) if (g_lower.pas.sem.pas_scope.cand[i].rid == d->rid) return;
    PasDef *c = pas_scope_push(&g_lower.pas.sem.pas_scope.cand, &g_lower.pas.sem.pas_scope.ncand, &g_lower.pas.sem.pas_scope.ccand, d->name);
    c->sig = d->sig;
    c->depth = d->depth;
    c->rid = d->rid;
}
static void pas_pf_closure(PNodeList *out, tree_t *a, const char *fsig, const char *callee, int pos) {
    const char *nm = NULL;
    if (a && a->t == TT_VAR) nm = a->v.sval;
    else if (a && a->t == TT_FNC && a->n >= 1 && a->c[0] && a->c[0]->t == TT_VAR && a->c[0]->v.sval) nm = (!strcmp(a->c[0]->v.sval, "__pas_pcall") && a->n == 4) ? a->c[1]->v.sval :
        (a->n == 1 ? a->c[0]->v.sval : NULL);
    const PasDef *d = pas_pf_lookup(nm);
    if (!d) {
        fprintf(stderr, "pascal: ISO 7185 6.6.3.4 violation: the actual-parameter %d of '%s' shall be a %s-identifier\n", pos, callee, *fsig == 'F' ? "function" : "procedure");
        g_lower.pas.sem.pas_iso_errors++;
    } else if (strcmp(d->sig, fsig)) {
        fprintf(stderr, "pascal: ISO 7185 6.6.3.6 violation: the actual-parameter %d of '%s' is '%s', whose heading %s is not congruous with the formal-parameter's %s\n", pos, callee, nm, d->sig,
            fsig);
        g_lower.pas.sem.pas_iso_errors++;
    }
    if (d && d->formal) {
        pnl_push(out, leaf_s(TT_VAR, nm));
        pnl_push(out, ilit(-1));
        for (int k = 1; k <= 3; k++) { pnl_push(out, leaf_s(TT_VAR, pas_pf_env_name(nm, k))); pnl_push(out, ilit(-1)); }
        return;
    }
    int lvl = d ? d->depth + 1 : 1;
    if (lvl > 4) {
        fprintf(stderr, "pascal: '%s' is declared at lexical level %d; passing a routine nested deeper than level 4 as an actual-parameter is not implemented\n", nm, lvl);
        g_lower.pas.sem.pas_iso_errors++;
    }
    if (d) pas_pf_candidate(d);
    pnl_push(out, ilit(d ? d->rid : 0));
    pnl_push(out, ilit(-1));
    for (int k = 1; k <= 3; k++) { pnl_push(out, k < lvl ? mk_fnc1("__pas_display_get", ilit(k)) : ilit(0)); pnl_push(out, ilit(-1)); }
}
static PNodeList *pas_pf_actuals(const char *callee, PNodeList *args) {
    const PasDef *cd = pas_pf_lookup(callee);
    const char *fs;
    if (cd && args) pas_value_actuals_check(callee, cd, args);
    if (!cd || !args || !strpbrk(cd->sig + 1, "PF")) return args;
    PNodeList *out = pnl_new();
    for (int i = 0; i + 1 < args->count; i += 2) {
        char k = pas_pf_param(cd->sig, i / 2, &fs);
        if (k == 'P' || k == 'F') pas_pf_closure(out, args->items[i], fs, callee, i / 2 + 1);
        else { pnl_push(out, args->items[i]); pnl_push(out, args->items[i + 1]); }
    }
    return out;
}
static tree_t *pas_pf_callthrough(const char *name, PNodeList *args) {
    const PasDef *d = pas_pf_lookup(name);
    int na = args ? args->count / 2 : 0, nf = pas_pf_nactuals(d->sig);
    if (na != nf) {
        fprintf(stderr, "pascal: ISO 7185 %s violation: '%s' is activated with the wrong number of actual-parameters for its heading %s\n", d->sig[0] == 'F' ? "6.7.3" : "6.8.2.3", name, d->sig);
        g_lower.pas.sem.pas_iso_errors++;
    }
    tree_t *e = mk_fnc1("__pas_pcall", leaf_s(TT_VAR, name));
    ast_push(e, leaf_s(TT_QLIT, d->sig));
    ast_push(e, ilit(0));
    for (int i = 0; args && i + 1 < args->count; i += 2) ast_push(e, args->items[i]);
    return e;
}
static tree_t *pas_pf_envcall(tree_t *call, const char *pp, int lvl, int n) {
    tree_t *e = mk_fnc1("__pas_envcall", ilit(lvl));
    for (int k = 1; k <= 3; k++) ast_push(e, leaf_s(TT_VAR, pas_pf_env_name(pp, k)));
    for (int k = 1; k <= 3; k++) { char b[48]; snprintf(b, sizeof b, "__pas_vptmp_pfs%d_%d", n, k); ast_push(e, leaf_s(TT_VAR, ct_strdup(b))); }
    for (int i = 0; i < call->n; i++) ast_push(e, call->c[i]);
    return e;
}
static void pas_pf_resolve(tree_t *e) {
    if (!e) return;
    for (int i = 0; i < e->n; i++) pas_pf_resolve(e->c[i]);
    if (e->t != TT_FNC || e->n < 4 || !e->c[0] || !e->c[0]->v.sval || strcmp(e->c[0]->v.sval, "__pas_pcall")) return;
    const char *pp = e->c[1]->v.sval, *fs = e->c[2]->v.sval;
    int isf = fs[0] == 'F', n = g_lower.pas.sem.pas_scope.npf++;
    char tb[40];
    snprintf(tb, sizeof tb, "__pas_vptmp_pf%d", n);
    const char *tmp = ct_strdup(tb);
    tree_t *chain = mk_fnc1("__pas_rterr", leaf_s(TT_QLIT, "6.6.3.4"));
    ast_push(chain, leaf_s(TT_QLIT, "a procedural or functional parameter denotes no routine"));
    for (int ci = g_lower.pas.sem.pas_scope.ncand - 1; ci >= 0; ci--) {
        const PasDef *c = &g_lower.pas.sem.pas_scope.cand[ci];
        if (strcmp(c->sig, fs)) continue;
        PNodeList *al = pnl_new();
        for (int i = 4; i < e->n; i++) { pnl_push(al, pas_tree_clone(e->c[i])); pnl_push(al, ilit(-1)); }
        tree_t *call = mk_call(c->name, al);
        if (c->depth >= 1 && call && call->t == TT_FNC) call = pas_pf_envcall(call, pp, c->depth + 1, n);
        tree_t *iff = ast_node_new(TT_IF);
        ast_push(iff, bin(TT_EQ, leaf_s(TT_VAR, pp), ilit(c->rid)));
        ast_push(iff, isf ? mk_assign(leaf_s(TT_VAR, tmp), call) : call);
        ast_push(iff, chain);
        chain = iff;
    }
    if (isf) { tree_t *sq = ast_node_new(TT_SEQ_EXPR); ast_push(sq, chain); ast_push(sq, leaf_s(TT_VAR, tmp)); chain = sq; }
    *e = *chain;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t *pas_sem_proc_at(const tree_t *root, int k) {
    if (!root || k < 0 || k >= root->n) return NULL;
    const tree_t *w = root->c[k];
    if (!w) return NULL;
    if (w->t == TT_PROC_DECL) return w;
    for (int i = 0; i < w->n; i++) {
        const tree_t *a = w->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":subj") && a->n > 0 && a->c[0] && a->c[0]->t == TT_PROC_DECL) return a->c[0];
    }
    return NULL;
}
static const char *pas_sem_name(const tree_t *p) { if (!p) return NULL; if (p->v.sval) return p->v.sval; return (p->n > 0 && p->c[0]) ? p->c[0]->v.sval : NULL; }
static const tree_t *pas_sem_locals(const tree_t *p) { return (p && p->n > 0) ? p->c[p->n - 1] : NULL; }
static const tree_t *pas_sem_params(const tree_t *p) { return (p && p->n > 1) ? p->c[1] : NULL; }
static const tree_t *pas_sem_body(const tree_t *p) { return (p && p->n > 2) ? p->c[2] : NULL; }
static int pas_sem_level(const tree_t *p) { const tree_t *l = pas_sem_locals(p); return l ? (int)l->v.ival : 0; }
static int pas_sem_vlist_has(const tree_t *vl, const char *name) {
    if (!vl || !name) return 0;
    for (int i = 0; i < vl->n; i++) if (vl->c[i] && vl->c[i]->v.sval && !strcmp(vl->c[i]->v.sval, name)) return 1;
    return 0;
}
static int pas_sem_shadows(const tree_t *p, const char *name) { return pas_sem_vlist_has(pas_sem_params(p), name) || pas_sem_vlist_has(pas_sem_locals(p), name); }
static const tree_t *pas_sem_find_proc(const tree_t *root, const char *name) {
    if (!root || !name) return NULL;
    for (int i = 0; i < root->n; i++) { const tree_t *p = pas_sem_proc_at(root, i); if (p) { const char *pn = pas_sem_name(p); if (pn && !strcmp(pn, name)) return p; } }
    return NULL;
}
static int pas_sem_threatens(const tree_t *root, const tree_t *node, const char *cv, const tree_t *self_for) {
    if (!node || !cv) return 0;
    if (node->t == TT_ASSIGN && node->n > 0 && node->c[0] && node->c[0]->t == TT_VAR && node->c[0]->v.sval && !strcmp(node->c[0]->v.sval, cv)) return node->line ? node->line : 1;
    if (node->t == TT_FOR && node != self_for && node->n > 0 && node->c[0] && node->c[0]->v.sval && !strcmp(node->c[0]->v.sval, cv)) return node->line ? node->line : 1;
    if (node->t == TT_FNC && node->n > 1 && node->c[0] && node->c[0]->v.sval) {
        const tree_t *callee = pas_sem_find_proc(root, node->c[0]->v.sval);
        if (callee) {
            const tree_t *pl = pas_sem_params(callee);
            long long mask = pl ? pl->v.ival : 0;
            for (int a = 1; a < node->n; a++) {
                int idx = a - 1;
                if (idx < 64 && (mask & (1LL << idx)) && node->c[a] && node->c[a]->t == TT_VAR && node->c[a]->v.sval && !strcmp(node->c[a]->v.sval, cv)) return node->line ? node->line : 1;
            }
        }
    }
    for (int i = 0; i < node->n; i++) { int r = pas_sem_threatens(root, node->c[i], cv, self_for); if (r) return r; }
    return 0;
}
static int pas_sem_subtree_start(const tree_t *root, int k) {
    int L = pas_sem_level(pas_sem_proc_at(root, k));
    int j = k - 1;
    while (j >= 0 && pas_sem_proc_at(root, j) && pas_sem_level(pas_sem_proc_at(root, j)) > L) j--;
    return j + 1;
}
static int pas_sem_threat_nested(const tree_t *root, int k, const char *cv) {
    int start = pas_sem_subtree_start(root, k);
    int j = k - 1;
    while (j >= start) {
        const tree_t *p = pas_sem_proc_at(root, j);
        int sub = pas_sem_subtree_start(root, j);
        if (p && pas_sem_level(p) <= pas_sem_level(pas_sem_proc_at(root, k))) break;
        if (p && pas_sem_shadows(p, cv)) { j = sub - 1; continue; }
        if (p) { int r = pas_sem_threatens(root, pas_sem_body(p), cv, NULL); if (r) return r; }
        j--;
    }
    return 0;
}
static void pas_sem_walk(const tree_t *root, int k, const tree_t *node, const char *fname, int *nerr) {
    if (!node) return;
    if (node->t == TT_FOR && node->n > 0 && node->c[0] && node->c[0]->v.sval && strncmp(node->c[0]->v.sval, "__pas_", 6) != 0) {
        const char *cv = node->c[0]->v.sval;
        const tree_t *b = pas_sem_proc_at(root, k);
        int lvl = pas_sem_level(b);
        if (lvl >= 1 && !pas_sem_vlist_has(pas_sem_locals(b), cv) && !pas_sem_vlist_has(pas_sem_params(b), cv)) {
            fprintf(stderr,
                "pascal: ISO 7185 6.8.3.9 violation in %s line %d: for-statement control-variable '%s' is not declared in the"
                " variable-declaration-part of the block closest-containing the for-statement\n", fname, node->line, cv);
            (*nerr)++;
        }
        int t = (node->n > 3) ? pas_sem_threatens(root, node->c[3], cv, node) : 0;
        if (!t) t = pas_sem_threat_nested(root, k, cv);
        if (t) {
            const tree_t *_bp = pas_sem_proc_at(root, k);
            const char *_pn = (_bp && _bp->v.sval) ? _bp->v.sval : "(main)";
            fprintf(stderr,
                "pascal: ISO 7185 6.8.3.9 violation in %s line %d: for-statement control-variable '%s' is threatened at line %d by the"
                " block closest-containing the for-statement (assigned to, passed as a variable parameter, or reused as a control-variable) -- in procedure %s\n", fname, node->line, cv, t, _pn);
            (*nerr)++;
        }
    }
    for (int i = 0; i < node->n; i++) pas_sem_walk(root, k, node->c[i], fname, nerr);
}
static int pas_sem_is_seq(const tree_t *n) { return n->t == TT_PROGRAM || n->t == TT_SEQ_EXPR || n->t == TT_LABEL_DEF; }
static int pas_sem_label_at(const tree_t *node, const char *lab, const pas_sem_step_t *gp, int glen, int len, int m, int root_only) {
    if (!node) return 0;
    if (node->t == TT_LABEL_DEF && node->v.sval && !strcmp(node->v.sval, lab)) return (root_only ? len == 0 : len == m) ? 1 : 2;
    for (int i = 0; i < node->n; i++) {
        int r;
        if (pas_sem_is_seq(node)) r = pas_sem_label_at(node->c[i], lab, gp, glen, len, m, root_only);
        else if (m == len && len < glen && gp[len].n == node && gp[len].i == i) r = pas_sem_label_at(node->c[i], lab, gp, glen, len + 1, len + 1, root_only);
        else r = pas_sem_label_at(node->c[i], lab, gp, glen, len + 1, m, root_only);
        if (r) return r;
    }
    return 0;
}
static int pas_sem_parent(const tree_t *root, int j) {
    int L = pas_sem_level(pas_sem_proc_at(root, j));
    for (int k = j + 1; k < root->n; k++) { const tree_t *p = pas_sem_proc_at(root, k); if (p && pas_sem_level(p) < L) return k; }
    return -1;
}
static void pas_sem_goto_check(const tree_t *root, int k, const tree_t *g, const pas_sem_step_t *gp, int glen, const char *fname, int *nerr) {
    const char *lab = g->v.sval;
    const char *pn = pas_sem_name(pas_sem_proc_at(root, k));
    int r = pas_sem_label_at(pas_sem_body(pas_sem_proc_at(root, k)), lab, gp, glen, 0, 0, 0);
    if (r == 2) {
        fprintf(stderr,
            "pascal: ISO 7185 6.8.1 violation in %s line %d: goto %s enters a structured statement -- label %s prefixes a statement outside every"
            " statement-sequence that contains the goto (in %s)\n", fname, g->line, lab, lab, pn ? pn : "main");
        (*nerr)++;
        return;
    }
    if (r == 1) return;
    for (int a = pas_sem_parent(root, k); a >= 0; a = pas_sem_parent(root, a)) {
        r = pas_sem_label_at(pas_sem_body(pas_sem_proc_at(root, a)), lab, gp, glen, 0, 0, 1);
        if (r == 1) return;
        if (r == 2) {
            const char *an = pas_sem_name(pas_sem_proc_at(root, a));
            fprintf(stderr,
                "pascal: ISO 7185 6.8.1 violation in %s line %d: goto %s leaves %s for label %s of the enclosing block %s, which does not prefix a statement"
                " of that block's outermost statement-sequence\n", fname, g->line, lab, pn ? pn : "main", lab, an ? an : "main");
            (*nerr)++;
            return;
        }
    }
    fprintf(stderr, "pascal: ISO 7185 6.8.1 violation in %s line %d: goto %s names a label that prefixes no statement of its block or of any enclosing block (in %s)\n", fname, g->line, lab,
        pn ? pn : "main");
    (*nerr)++;
}
static void pas_sem_goto_walk(const tree_t *root, int k, const tree_t *node, pas_sem_step_t *gp, int len, const char *fname, int *nerr) {
    if (!node || len >= PAS_SEM_PATH_MAX) return;
    if (node->t == TT_GOTO_U && node->v.sval) { pas_sem_goto_check(root, k, node, gp, len, fname, nerr); return; }
    for (int i = 0; i < node->n; i++) {
        if (pas_sem_is_seq(node)) { pas_sem_goto_walk(root, k, node->c[i], gp, len, fname, nerr); continue; }
        gp[len].n = node;
        gp[len].i = i;
        pas_sem_goto_walk(root, k, node->c[i], gp, len + 1, fname, nerr);
    }
}
static int pascal_sem_check(const tree_t *root, const char *filename) {
    if (!root) return 0;
    int nerr = 0;
    const char *fname = filename ? filename : "<stdin>";
    for (int k = 0; k < root->n; k++) { const tree_t *p = pas_sem_proc_at(root, k); if (p) pas_sem_walk(root, k, pas_sem_body(p), fname, &nerr); }
    for (int k = 0; k < root->n; k++) { const tree_t *p = pas_sem_proc_at(root, k); pas_sem_step_t gp[PAS_SEM_PATH_MAX]; if (p) pas_sem_goto_walk(root, k, pas_sem_body(p), gp, 0, fname, &nerr); }
    return nerr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void elab_bad(const char *nt, const tree_t *n) { fprintf(stderr, "pascal: internal error: no production of '%s' fits tree node kind %d\n", nt, n ? (int)n->t : -1); abort(); }
static void pas_apply_event(const char *ev) {
    if (!strcmp(ev, "iso_error")) g_lower.pas.sem.pas_iso_errors++;
    else if (!strcmp(ev, "mode_iso")) g_lower.pas.sem.pas_mode_iso = 1;
    else if (!strcmp(ev, "seen_mode")) g_lower.pas.sem.pas_seen_mode_directive = 1;
    else if (!strncmp(ev, "minenum ", 8)) g_lower.pas.sem.pas_min_enum_size = atoi(ev + 8);
    else if (!strncmp(ev, "packset ", 8)) g_lower.pas.sem.pas_pack_set_size = atoi(ev + 8);
    else if (!strncmp(ev, "packrec ", 8)) g_lower.pas.sem.pas_pack_records = atoi(ev + 8);
    else if (!strncmp(ev, "range ", 6)) g_lower.pas.sem.pas_range_check_on = atoi(ev + 6);
    else if (!strncmp(ev, "zerobased ", 10)) {
        if (ev[10] == '1') g_lower.pas.sem.pas_zerobased_strings |= 1;
        else g_lower.pas.sem.pas_zerobased_strings &= ~1;
    } else if (!strncmp(ev, "overflow ", 9)) {
        if (ev[9] == '1') g_lower.pas.sem.pas_zerobased_strings |= 2;
        else g_lower.pas.sem.pas_zerobased_strings &= ~2;
    } else if (!strncmp(ev, "align ", 6)) g_lower.pas.sem.pas_align_mac68k = (ev[6] == '1');
    else if (!strncmp(ev, "codepage ", 9)) g_lower.pas.sem.pas_codepage = atoi(ev + 9);
    else if (!strcmp(ev, "push")) {
        if (g_lower.pas.sem.pas_min_enum_stack_n < 64) {
            g_lower.pas.sem.pas_stk_enum[g_lower.pas.sem.pas_min_enum_stack_n] = g_lower.pas.sem.pas_min_enum_size;
            g_lower.pas.sem.pas_stk_set[g_lower.pas.sem.pas_min_enum_stack_n] = g_lower.pas.sem.pas_pack_set_size;
            g_lower.pas.sem.pas_stk_rec[g_lower.pas.sem.pas_min_enum_stack_n] = g_lower.pas.sem.pas_pack_records;
            g_lower.pas.sem.pas_stk_zb[g_lower.pas.sem.pas_min_enum_stack_n] = g_lower.pas.sem.pas_zerobased_strings;
            g_lower.pas.sem.pas_min_enum_stack_n++;
        }
    } else if (!strcmp(ev, "pop")) {
        if (g_lower.pas.sem.pas_min_enum_stack_n > 0) {
            g_lower.pas.sem.pas_min_enum_stack_n--;
            g_lower.pas.sem.pas_min_enum_size = g_lower.pas.sem.pas_stk_enum[g_lower.pas.sem.pas_min_enum_stack_n];
            g_lower.pas.sem.pas_pack_set_size = g_lower.pas.sem.pas_stk_set[g_lower.pas.sem.pas_min_enum_stack_n];
            g_lower.pas.sem.pas_pack_records = g_lower.pas.sem.pas_stk_rec[g_lower.pas.sem.pas_min_enum_stack_n];
            g_lower.pas.sem.pas_zerobased_strings = g_lower.pas.sem.pas_stk_zb[g_lower.pas.sem.pas_min_enum_stack_n];
        }
    }
}
static void tok_events(const tree_t *x) {
    if (!x) return;
    for (int i = 0; i < x->n; i++) {
        tree_t *a = x->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":pragma") && a->slen == 0 && a->n > 0 && a->c[0] && a->c[0]->v.sval) { a->slen = 1; pas_apply_event(a->c[0]->v.sval); }
    }
}
static char *tok_s(const tree_t *x) { tok_events(x); return x->v.sval; }
static long long tok_i(const tree_t *x) { tok_events(x); return x->v.ival; }
static double tok_d(const tree_t *x) { tok_events(x); return x->v.dval; }
static pval E_decl_part(const tree_t *n);
static pval E_body(const tree_t *n);
static pval E_block(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    v[1].ival = 0;
    for (int i = 0; i < n->n - 1; i++) { v[2] = E_decl_part(n->c[i]); v[1].ival = pas_decl_part_order(v[1].ival, v[2].ival); }
    v[2] = E_body(n->c[n->n - 1]);
    pas_labels_close();
    out.node = v[2].node;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pval E_argument(const tree_t *n);
static pval E_argument_list(const tree_t *n, int from, int to);
static pval E_assignment(const tree_t *n);
static pval E_body(const tree_t *n);
static pval E_body_stmt(const tree_t *n);
static pval E_call(const tree_t *n);
static pval E_call_with_args(const tree_t *n);
static pval E_case_arm_mark(const tree_t *n);
static pval E_case_elem(const tree_t *n);
static pval E_case_list(const tree_t *n, int from, int to);
static pval E_case_open(const tree_t *n);
static pval E_case_statement(const tree_t *n);
static pval E_compound_statement(const tree_t *n);
static pval E_const_decl(const tree_t *n);
static pval E_const_decl_list(const tree_t *n, int from, int to);
static pval E_constant(const tree_t *n);
static pval E_constant_list(const tree_t *n, int from, int to);
static pval E_decl_part(const tree_t *n);
static pval E_expression(const tree_t *n);
static pval E_expression_list(const tree_t *n, int from, int to);
static pval E_file_id_list_opt(const tree_t *n);
static pval E_for_statement(const tree_t *n);
static pval E_goto_statement(const tree_t *n);
static pval E_id_list(const tree_t *n, int from, int to);
static pval E_if_statement(const tree_t *n);
static pval E_label_list(const tree_t *n, int from, int to);
static pval E_packed_opt(const tree_t *n);
static pval E_parameter_decl(const tree_t *n);
static pval E_parameter_decl_list(const tree_t *n, int from, int to);
static pval E_parameter_list_opt(const tree_t *n);
static pval E_pf_params(const tree_t *n);
static pval E_pf_section(const tree_t *n);
static pval E_pf_sections(const tree_t *n, int from, int to);
static pval E_procedure_decl(const tree_t *n);
static pval E_program(const tree_t *n);
static pval E_pv_mark(const tree_t *n);
static pval E_record_body(const tree_t *n);
static pval E_record_case_arm(const tree_t *n);
static pval E_record_case_list(const tree_t *n, int from, int to);
static pval E_record_case_opt(const tree_t *n);
static pval E_record_field(const tree_t *n);
static pval E_record_field_list(const tree_t *n, int from, int to);
static pval E_repeat_statement(const tree_t *n);
static pval E_scalar_constant(const tree_t *n);
static pval E_selector(const tree_t *n);
static pval E_set_member(const tree_t *n);
static pval E_set_member_list(const tree_t *n, int from, int to);
static pval E_simple_type(const tree_t *n);
static pval E_statement(const tree_t *n);
static pval E_statement_list(const tree_t *n, int from, int to);
static pval E_statement_no_label(const tree_t *n);
static pval E_type(const tree_t *n);
static pval E_type_decl(const tree_t *n);
static pval E_type_decl_list(const tree_t *n, int from, int to);
static pval E_var_decl(const tree_t *n);
static pval E_var_decl_list(const tree_t *n, int from, int to);
static pval E_while_statement(const tree_t *n);
static pval E_with_open(const tree_t *n, int from, int to);
static pval E_with_statement(const tree_t *n);
static pval E_argument(const tree_t *n) {
    pval v[8];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_FMT) && n->n == 2) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool(v[1].node)); pnl_push(_al, v[3].node); out.list = _al; }
        return out;
    }
    if (IS(n, TT_FMT)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        v[5] = E_expression(n->c[2]);
        { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool(v[1].node)); pnl_push(_al, ilit(-3)); pnl_push(_al, v[3].node); pnl_push(_al, v[5].node); out.list = _al; }
        return out;
    }
    if (n != NULL) { v[1] = E_expression(n); { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool(v[1].node)); pnl_push(_al, ilit(-1)); out.list = _al; } return out; }
    elab_bad("argument", n);
    return out;
}
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
static pval E_assignment(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_ASSIGN)) {
        v[1] = E_selector(n->c[0]);
        v[3] = E_expression(n->c[1]);
        {
            if (v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) {
                pas_value_compat(v[1].node->v.sval, v[3].node, "6.8.2.2", "the variable");
                pas_string_assign_check(v[1].node->v.sval, v[3].node);
            }
            pas_deref_assign_compat(v[1].node, v[3].node);
            tree_t *_tl = pas_trace_lhs(v[1].node);
            if (v[1].node && v[1].node->t == TT_FNC && v[1].node->n == 2 && v[1].node->c[0] && v[1].node->c[0]->v.sval &&
                (!strcmp(v[1].node->c[0]->v.sval, "__pas_fbuf_get") || !strcmp(v[1].node->c[0]->v.sval, "__pas_tbuf_get"))) {
                int _isqlit = v[3].node &&
                    (v[3].node->t == TT_QLIT || (v[3].node->t == TT_FNC && v[3].node->n >= 1 && v[3].node->c[0] && v[3].node->c[0]->v.sval && !strcmp(v[3].node->c[0]->v.sval, "__pas_chrlit")));
                if (!strcmp(v[1].node->c[0]->v.sval, "__pas_fbuf_get") && v[1].node->c[1] && v[1].node->c[1]->t == TT_VAR && v[1].node->c[1]->v.sval && _isqlit &&
                    pas_tfcomp_nonchar(v[1].node->c[1]->v.sval)) {
                    fprintf(stderr, "pascal: ISO 7185 6.6.5.2 violation: the value assigned to the buffer-variable of '%s' is not assignment-compatible with its component-type\n",
                        v[1].node->c[1]->v.sval);
                    g_lower.pas.sem.pas_iso_errors++;
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
                if (g_lower.pas.sem.pas_range_check_on && v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) {
                    long long _rlo, _rhi;
                    if (pas_subvar_get(v[1].node->v.sval, &_rlo, &_rhi)) _rhs0 = pas_range_wrap(_rhs0, _rlo, _rhi, pas_is_setvar(v[1].node->v.sval) ? "set" : "6.8.2.2");
                }
                out.node = pas_trace_assigned(_tl, mk_assign(v[1].node, _rhs0));
            }
            out.node = pas_vt_wrap_stmt(v[1].node, v[3].node, out.node);
        }
        return out;
    }
    elab_bad("assignment", n);
    return out;
}
static pval E_body(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_SEQ_EXPR)) { v[2] = E_statement_list(n, 0, n->n); { out.node = prog_of(v[2].list); } return out; }
    elab_bad("body", n);
    return out;
}
static pval E_body_stmt(const tree_t *n) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n != NULL) { { } v[2] = E_statement(n); { int _ln = n->line; if (v[2].node && !v[2].node->line) v[2].node->line = _ln; out.node = v[2].node; } return out; }
    elab_bad("body_stmt", n);
    return out;
}
static pval E_call(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_FNC) && n->n == 1) {
        v[1].str = tok_s(n->c[0]);
        {
            if (pas_pf_is_formal(v[1].str)) out.node = pas_pf_callthrough(v[1].str, NULL);
            else if (pas_is_proc(v[1].str)) {
                tree_t *e = ast_node_new(TT_FNC);
                ast_push(e, leaf_s(TT_VAR, v[1].str));
                out.node = e;
            } else {
                pas_required_needs_params(v[1].str);
                out.node = mk_call(v[1].str, NULL);
            }
        }
        return out;
    }
    if (n != NULL) { v[1] = E_call_with_args(n); { out.node = v[1].node; } return out; }
    elab_bad("call", n);
    return out;
}
static pval E_call_with_args(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_FNC)) {
        v[1].str = tok_s(n->c[0]);
        v[3] = E_argument_list(n, 1, n->n);
        {
            v[3].list = pas_pf_actuals(v[1].str, v[3].list);
            if (v[3].list) for (int i = 0; i < v[3].list->count; i++) if (pas_proc_param_is_var(v[1].str, i) && pas_actual_is_packed_component(v[3].list->items[i])) {
                fprintf(stderr, "pascal: ISO 7185 6.6.3.3 violation: a component of a packed structure is passed as the variable parameter %d of '%s'\n", i + 1, v[1].str);
                g_lower.pas.sem.pas_iso_errors++;
            }
            if (v[3].list) for (int i = 0; i < v[3].list->count; i++) if (pas_proc_param_is_var(v[1].str, i)) v[3].list->items[i] = pas_vt_strip_undef(v[3].list->items[i]);
            if (v[3].list) for (int i = 0; i < v[3].list->count; i++) if (pas_proc_param_is_var(v[1].str, i) && pas_actual_is_tagfield(v[3].list->items[i])) {
                fprintf(stderr, "pascal: ISO 7185 6.6.3.3 violation: a tag-field of a variant-part is passed as the variable parameter %d of '%s'\n", i + 1, v[1].str);
                g_lower.pas.sem.pas_iso_errors++;
            }
            pas_call_arity(v[1].str, v[3].list);
            pas_ordinal_fn_arg(v[1].str, v[3].list);
            out.node = pas_pf_is_formal(v[1].str) ? pas_pf_callthrough(v[1].str, v[3].list) : mk_call(v[1].str, v[3].list);
        }
        return out;
    }
    elab_bad("call_with_args", n);
    return out;
}
static pval E_case_arm_mark(const tree_t *n) {
    pval v[3];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n == NULL) { { out.ival = g_lower.pas.sem.pas_pend_nf; } return out; }
    elab_bad("case_arm_mark", n);
    return out;
}
static pval E_case_elem(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_ARM)) { v[1] = E_constant_list(n->c[0], 0, n->c[0]->n); v[3] = E_body_stmt(n->c[1]); { out.node = bin(TT_IF, pas_cond(v[1].node), v[3].node); } return out; }
    elab_bad("case_elem", n);
    return out;
}
static pval E_case_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_case_elem(ce);
            { PNodeList *l = pnl_new(); if (v[1].node) pnl_push(l, v[1].node); out.list = l; }
        } else {
            v[1] = out;
            v[3] = E_case_elem(ce);
            { if (v[3].node) pnl_push(v[1].list, v[3].node); out.list = v[1].list; }
        }
    }
    return out;
}
static pval E_case_open(const tree_t *n) {
    pval v[3];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n == NULL) { { pas_vcase_push(); pas_vt_open(); } return out; }
    elab_bad("case_open", n);
    return out;
}
static pval E_case_statement(const tree_t *n) {
    pval v[9];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_CASE)) {
        v[2] = E_expression(n->c[0]);
        { pas_case_push(); }
        v[5] = E_case_list(n, 1, n->n);
        {
            tree_t *seq = ast_node_new(TT_SEQ_EXPR);
            ast_push(seq, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_case_cur()), v[2].node));
            tree_t *chain = ast_node_new(TT_FNC);
            ast_push(chain, leaf_s(TT_VAR, "__pas_rterr"));
            ast_push(chain, leaf_s(TT_QLIT, "6.8.3.5"));
            ast_push(chain, leaf_s(TT_QLIT, "no case-constant of the case-statement is equal to the value of its case-index"));
            if (v[5].list) for (int i = v[5].list->count - 1; i >= 0; i--) { tree_t *e = v[5].list->items[i]; if (!e) continue; ast_push(e, chain); chain = e; }
            ast_push(seq, chain);
            pas_case_pop();
            out.node = seq;
        }
        return out;
    }
    elab_bad("case_statement", n);
    return out;
}
static pval E_compound_statement(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_SEQ_EXPR)) { v[2] = E_statement_list(n, 0, n->n); { out.node = seq_of(v[2].list); } return out; }
    elab_bad("compound_statement", n);
    return out;
}
static pval E_const_decl(const tree_t *n) {
    pval v[8];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_FLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[3].dval = tok_d(n->c[1]);
        { pas_scope_define(v[1].str); pas_rconst_add(v[1].str, v[3].dval); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_PLS) && IS(n->c[1]->c[0], TT_FLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[4].dval = tok_d(n->c[1]->c[0]);
        { pas_scope_define(v[1].str); pas_rconst_add(v[1].str, v[4].dval); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_MNS) && IS(n->c[1]->c[0], TT_FLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[4].dval = tok_d(n->c[1]->c[0]);
        { pas_scope_define(v[1].str); pas_rconst_add(v[1].str, -v[4].dval); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_QLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[3].str = tok_s(n->c[1]);
        {
            pas_scope_define(v[1].str);
            if (v[3].str && strlen(v[3].str)==1) { pas_const_add(v[1].str,(long long)(unsigned char)v[3].str[0]); pas_charvar_add(v[1].str); } else pas_sconst_add(v[1].str,v[3].str);
        }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_CHRLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[3].ival = tok_i(n->c[1]);
        { pas_scope_define(v[1].str); pas_const_add(v[1].str, v[3].ival); pas_charvar_add(v[1].str); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const")) { v[1].str = tok_s(n->c[0]); v[3] = E_constant(n->c[1]); { pas_scope_define(v[1].str); pas_const_add(v[1].str, v[3].ival); } return out; }
    elab_bad("const_decl", n);
    return out;
}
static pval E_const_decl_list(const tree_t *n, int from, int to) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) { const tree_t *ce = n->c[i]; if (i == from) { v[1] = E_const_decl(ce); } else { v[1] = out; v[2] = E_const_decl(ce); } }
    return out;
}
static pval E_constant(const tree_t *n) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_PLS)) { v[2] = E_scalar_constant(n->c[0]); { out.ival = v[2].ival; } return out; }
    if (IS(n, TT_MNS)) { v[2] = E_scalar_constant(n->c[0]); { out.ival = -v[2].ival; } return out; }
    if (n != NULL) { v[1] = E_scalar_constant(n); { out.ival = v[1].ival; } return out; }
    elab_bad("constant", n);
    return out;
}
static pval E_constant_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_constant(ce);
            { out.node = bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit(v[1].ival)); }
        } else {
            v[1] = out;
            v[3] = E_constant(ce);
            { out.node = bin(TT_ADD, v[1].node, bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit(v[3].ival))); }
        }
    }
    return out;
}
static pval E_decl_part(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_PART) && !strcmp(n->v.sval, "label")) { v[2] = E_label_list(n, 0, n->n); { out.ival = 1; } return out; }
    if (IS(n, TT_PART) && !strcmp(n->v.sval, "const")) { v[2] = E_const_decl_list(n, 0, n->n); { out.ival = 2; } return out; }
    if (IS(n, TT_PART) && !strcmp(n->v.sval, "type")) { v[2] = E_type_decl_list(n, 0, n->n); { out.ival = 3; } return out; }
    if (IS(n, TT_PART) && !strcmp(n->v.sval, "var")) { v[2] = E_var_decl_list(n, 0, n->n); { out.ival = 4; } return out; }
    if (IS(n, TT_PROCEDURE) || IS(n, TT_FUNCTION)) { v[1] = E_procedure_decl(n); { out.ival = 5; } return out; }
    elab_bad("decl_part", n);
    return out;
}
static pval E_expression(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_IN)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = mk_in(v[1].node, v[3].node); } return out; }
    if (IS(n, TT_LT)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_rel(TT_LT, v[1].node, v[3].node); } return out; }
    if (IS(n, TT_LE)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_rel_or_set(TT_LE, "__pas_subset", v[1].node, v[3].node); } return out; }
    if (IS(n, TT_GT)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_rel(TT_GT, v[1].node, v[3].node); } return out; }
    if (IS(n, TT_GE)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_rel_or_set(TT_GE, "__pas_super", v[1].node, v[3].node); } return out; }
    if (IS(n, TT_NE)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_rel_or_set(TT_NE, "__pas_setne", v[1].node, v[3].node); } return out; }
    if (IS(n, TT_EQ)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_rel_or_set(TT_EQ, "__pas_seteq", v[1].node, v[3].node); } return out; }
    if (IS(n, TT_PLS)) { v[2] = E_expression(n->c[0]); { pas_sign_operand(v[2].node, '+'); out.node = v[2].node; } return out; }
    if (IS(n, TT_MNS)) { v[2] = E_expression(n->c[0]); { pas_sign_operand(v[2].node, '-'); out.node = mk_neg(v[2].node); } return out; }
    if (IS(n, TT_ADD)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_arith_or_set(TT_ADD, "__pas_setuni", v[1].node, v[3].node); } return out; }
    if (IS(n, TT_SUB)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_arith_or_set(TT_SUB, "__pas_setdif", v[1].node, v[3].node); } return out; }
    if (IS(n, TT_ALT)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = (pas_is_boolexpr(v[1].node) && pas_is_boolexpr(v[3].node)) ? bin(TT_ALT, v[1].node, v[3].node) : mk_fnc2("ior", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_MUL)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_arith_or_set(TT_MUL, "__pas_setint", v[1].node, v[3].node); } return out; }
    if (IS(n, TT_DIV)) { v[1] = E_expression(n->c[0]); v[3] = E_expression(n->c[1]); { out.node = pas_rdiv(v[1].node, v[3].node); } return out; }
    if (IS(n, TT_IDIV)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_is_qword(v[1].node) ? mk_fnc2("__pas_udiv", v[1].node, v[3].node) : bin(TT_DIV, v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_MOD)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = pas_is_qword(v[1].node) ? mk_fnc2("__pas_umod", v[1].node, v[3].node) : pas_mod(v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_CONJ)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { out.node = (pas_is_boolexpr(v[1].node) && pas_is_boolexpr(v[3].node)) ? bin(TT_CONJ, v[1].node, v[3].node) : mk_fnc2("iand", v[1].node, v[3].node); }
        return out;
    }
    if (IS(n, TT_VAR) || IS(n, TT_IDX) || IS(n, TT_FIELD) || IS(n, TT_DEREF)) {
        v[1] = E_selector(n);
        {
            if (pas_is_cafield(v[1].node)) {
                tree_t *u = ast_node_new(TT_FNC);
                ast_push(u, leaf_s(TT_VAR, "__pas_ca_unpack"));
                ast_push(u, v[1].node);
                ast_push(u, ilit(pas_cafield_lo_get(v[1].node)));
                out.node = u;
            } else out.node = pas_vt_wrap_read(v[1].node);
        }
        return out;
    }
    if (IS(n, TT_FNC)) { v[1] = E_call_with_args(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_ILIT)) { v[1].ival = tok_i(n); { out.node = ilit(v[1].ival); } return out; }
    if (IS(n, TT_FLIT)) { v[1].dval = tok_d(n); { out.node = flit(v[1].dval); } return out; }
    if (IS(n, TT_QLIT)) {
        v[1].str = tok_s(n);
        {
            if (v[1].str && strlen(v[1].str) == 1) {
                tree_t *_cl = ast_node_new(TT_FNC);
                ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit"));
                ast_push(_cl, ilit((long long)(unsigned char)v[1].str[0]));
                out.node = _cl;
            } else out.node = leaf_s(TT_QLIT, v[1].str);
        }
        return out;
    }
    if (IS(n, TT_CHRLIT)) { v[1].ival = tok_i(n); { tree_t *_cl = ast_node_new(TT_FNC); ast_push(_cl, leaf_s(TT_VAR, "__pas_chrlit")); ast_push(_cl, ilit(v[1].ival)); out.node = _cl; } return out; }
    if (IS(n, TT_NOT)) { v[2] = E_expression(n->c[0]); { out.node = pas_flip_rel(pas_cond(v[2].node)); } return out; }
    if (IS(n, TT_ADDR)) { v[2] = E_expression(n->c[0]); { out.node = pas_addr_of_proc(v[2].node); } return out; }
    if (IS(n, TT_SET) && n->n == 0) { { out.node = mk_set_ctor(NULL); } return out; }
    if (IS(n, TT_SET)) { v[2] = E_set_member_list(n, 0, n->n); { out.node = v[2].node; } return out; }
    elab_bad("expression", n);
    return out;
}
static pval E_expression_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_expression(ce);
            { PNodeList *l = pnl_new(); pnl_push(l, v[1].node); out.list = l; }
        } else {
            v[1] = out;
            v[3] = E_expression(ce);
            { pnl_push(v[1].list, v[3].node); out.list = v[1].list; }
        }
    }
    return out;
}
static pval E_file_id_list_opt(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n == NULL) { return out; }
    if (n != NULL) {
        v[2] = E_id_list(n, 0, n->n);
        {
            pas_scope_define_list(v[2].list);
            pas_program_params_distinct(v[2].list);
            if (v[2].list) for (int i = 0; i < v[2].list->count; i++) {
                tree_t *id = v[2].list->items[i];
                if (id && id->v.sval && strcmp(id->v.sval, "input") && strcmp(id->v.sval, "output")) {
                    pas_filevar_add(id->v.sval);
                    if (g_lower.pas.sem.pas_nhdrfile < 32) g_lower.pas.sem.pas_hdrfiles[g_lower.pas.sem.pas_nhdrfile++] = ct_strdup(id->v.sval);
                }
            }
        }
        return out;
    }
    elab_bad("file_id_list_opt", n);
    return out;
}
static pval E_for_statement(const tree_t *n) {
    pval v[11];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_FOR) && !strcmp(n->v.sval, "to")) {
        v[2].str = tok_s(n->c[0]);
        v[4] = E_expression(n->c[1]);
        v[6] = E_expression(n->c[2]);
        v[8] = E_body_stmt(n->c[3]);
        {
            pas_scope_require(v[2].str);
            pas_value_compat(v[2].str, v[4].node, "6.8.3.9", "the control-variable");
            pas_value_compat(v[2].str, v[6].node, "6.8.3.9", "the control-variable");
            if (pas_var_is_real(v[2].str)) {
                fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation: the control-variable '%s' of a for-statement has type real, which is not an ordinal-type\n", v[2].str);
                g_lower.pas.sem.pas_iso_errors++;
            }
            pas_for_const_bounds(v[2].str, v[4].node, v[6].node, 0);
            tree_t *e = ast_node_new(TT_FOR);
            ast_push(e, leaf_s(TT_VAR, v[2].str));
            ast_push(e, v[4].node);
            ast_push(e, v[6].node);
            ast_push(e, pas_trace_wrap_for_body(v[2].str, v[8].node));
            if (!g_lower.pas.sem.pas_seen_mode_directive) e->v.ival |= 2;
            out.node = pas_for_once(e);
        }
        return out;
    }
    if (IS(n, TT_FOR)) {
        v[2].str = tok_s(n->c[0]);
        v[4] = E_expression(n->c[1]);
        v[6] = E_expression(n->c[2]);
        v[8] = E_body_stmt(n->c[3]);
        {
            pas_scope_require(v[2].str);
            pas_value_compat(v[2].str, v[4].node, "6.8.3.9", "the control-variable");
            pas_value_compat(v[2].str, v[6].node, "6.8.3.9", "the control-variable");
            if (pas_var_is_real(v[2].str)) {
                fprintf(stderr, "pascal: ISO 7185 6.8.3.9 violation: the control-variable '%s' of a for-statement has type real, which is not an ordinal-type\n", v[2].str);
                g_lower.pas.sem.pas_iso_errors++;
            }
            pas_for_const_bounds(v[2].str, v[4].node, v[6].node, 1);
            tree_t *e = ast_node_new(TT_FOR);
            ast_push(e, leaf_s(TT_VAR, v[2].str));
            ast_push(e, v[4].node);
            ast_push(e, v[6].node);
            ast_push(e, pas_trace_wrap_for_body(v[2].str, v[8].node));
            e->v.ival = 1;
            if (!g_lower.pas.sem.pas_seen_mode_directive) e->v.ival |= 2;
            out.node = pas_for_once(e);
        }
        return out;
    }
    elab_bad("for_statement", n);
    return out;
}
static pval E_goto_statement(const tree_t *n) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_GOTO_U)) {
        v[2].ival = tok_i(n->c[0]);
        {
            pas_label_find(v[2].ival);
            char _gb[24];
            snprintf(_gb, sizeof _gb, "%lld", (long long)v[2].ival);
            tree_t *G = ast_node_new(TT_GOTO_U);
            G->v.sval = ct_strdup(_gb);
            G->line = (n->line);
            out.node = G;
        }
        return out;
    }
    elab_bad("goto_statement", n);
    return out;
}
static pval E_id_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1].str = tok_s(ce);
            { pas_not_a_word_symbol(v[1].str); PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, v[1].str)); out.list = l; }
        } else {
            v[1] = out;
            v[3].str = tok_s(ce);
            { pas_not_a_word_symbol(v[3].str); pnl_push(v[1].list, leaf_s(TT_VAR, v[3].str)); out.list = v[1].list; }
        }
    }
    return out;
}
static pval E_if_statement(const tree_t *n) {
    pval v[9];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_IF) && n->n == 2) {
        v[2] = E_expression(n->c[0]);
        v[4] = E_body_stmt(n->c[1]);
        { out.node = bin(TT_IF, pas_cond_bool(v[2].node, "an if-statement", "6.8.3.4"), v[4].node); }
        return out;
    }
    if (IS(n, TT_IF)) {
        v[2] = E_expression(n->c[0]);
        v[4] = E_body_stmt(n->c[1]);
        v[6] = E_body_stmt(n->c[2]);
        { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond_bool(v[2].node, "an if-statement", "6.8.3.4")); ast_push(e, v[4].node); ast_push(e, v[6].node); out.node = e; }
        return out;
    }
    elab_bad("if_statement", n);
    return out;
}
static pval E_label_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1].ival = tok_i(ce);
            { pas_label_in_range(v[1].ival); pas_label_declare(v[1].ival); }
        } else {
            v[1] = out;
            v[3].ival = tok_i(ce);
            { pas_label_in_range(v[3].ival); pas_label_declare(v[3].ival); }
        }
    }
    return out;
}
static pval E_packed_opt(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n == NULL) { { out.ival = 0; } return out; }
    if (n != NULL) { { out.ival = 1; } return out; }
    elab_bad("packed_opt", n);
    return out;
}
static pval E_parameter_decl(const tree_t *n) {
    pval v[8];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int hasp = n->n > 1 && n->c[1] && n->c[1]->t == TT_PARAMS;
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "procedure")) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pf_params(hasp ? n->c[1] : NULL);
        { out.list = pas_pf_formal(v[2].str, pas_pf_sig('P', v[3].str, NULL)); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "function")) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pf_params(hasp ? n->c[1] : NULL);
        v[5].str = tok_s(n->c[n->n - 1]);
        { out.list = pas_pf_formal(v[2].str, pas_pf_sig('F', v[3].str, v[5].str)); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "var")) {
        v[2] = E_id_list(n->c[0], 0, n->c[0]->n);
        v[4].str = tok_s(n->c[1]);
        {
            pas_pf_section('r', v[2].list, v[4].str);
            if (!strcmp(v[4].str, "text")) for (int i = 0; i < v[2].list->count; i++) if (v[2].list->items[i] && v[2].list->items[i]->v.sval) pas_filevar_add(v[2].list->items[i]->v.sval);
            if (pas_is_booltype(v[4].str)) for (int i = 0; i < v[2].list->count; i++) if (v[2].list->items[i] && v[2].list->items[i]->v.sval) pas_boolvar_add(v[2].list->items[i]->v.sval);
            if (pas_is_settype(v[4].str)) for (int i = 0; i < v[2].list->count; i++) if (v[2].list->items[i] && v[2].list->items[i]->v.sval) pas_setvar_add(v[2].list->items[i]->v.sval);
            const char *_pt = pas_ptrtype_target(v[4].str);
            if (_pt) for (int i = 0; i < v[2].list->count; i++) if (v[2].list->items[i] && v[2].list->items[i]->v.sval) pas_ptrvar_add(v[2].list->items[i]->v.sval, _pt);
            {
                long long _ah = pas_arrtype_high(v[4].str);
                long long _nc = pas_arrtype_ncols(v[4].str);
                int _nf = pas_rectype_nf(v[4].str);
                for (int i = 0; i < v[2].list->count; i++) if (v[2].list->items[i] && v[2].list->items[i]->v.sval) {
                    if (_nf > 0 && _ah >= 0) {
                        pas_array_add2d_param(v[2].list->items[i]->v.sval, _ah, -1);
                        pas_arrrec_add_from_type(v[2].list->items[i]->v.sval, v[4].str);
                    } else if (_nf > 0) {
                        pas_recvar_add_from_type(v[2].list->items[i]->v.sval, v[4].str);
                        pas_array_add2d_param(v[2].list->items[i]->v.sval, (long long)(_nf - 1), -1);
                    } else if (_ah >= 0) {
                        if (_nc >= 0) pas_array_add2d_param(v[2].list->items[i]->v.sval, _ah, _nc);
                        else pas_array_add2d_param(v[2].list->items[i]->v.sval, _ah, -1);
                    }
                }
            }
            for (int i = 0; i < v[2].list->count; i++) if (v[2].list->items[i]) ast_push(v[2].list->items[i], ast_node_new(TT_SUCCEED));
            out.list = v[2].list;
        }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "value")) {
        v[1] = E_id_list(n->c[0], 0, n->c[0]->n);
        v[3].str = tok_s(n->c[1]);
        {
            pas_value_formal_file_check(v[1].list, v[3].str);
            pas_pf_section('v', v[1].list, v[3].str);
            if (pas_is_booltype(v[3].str)) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) pas_boolvar_add(v[1].list->items[i]->v.sval);
            if (pas_is_settype(v[3].str)) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) pas_setvar_add(v[1].list->items[i]->v.sval);
            const char *_pt = pas_ptrtype_target(v[3].str);
            if (_pt) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) pas_ptrvar_add(v[1].list->items[i]->v.sval, _pt);
            if (!strcmp(v[3].str, "char")) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) pas_charvar_add(v[1].list->items[i]->v.sval);
            if (!strcmp(v[3].str, "single")) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) pas_singlevar_add(v[1].list->items[i]->v.sval);
            if (pas_is_realtypename(v[3].str)) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i]) ast_push(v[1].list->items[i], ast_node_new(TT_FLIT));
            if (!strcmp(v[3].str, "pchar")) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i]) {
                ast_push(v[1].list->items[i], ast_node_new(TT_QLIT));
                if (v[1].list->items[i]->v.sval) pas_pcharvar_add(v[1].list->items[i]->v.sval);
            }
            {
                long long _ah = pas_arrtype_high(v[3].str);
                long long _nc = pas_arrtype_ncols(v[3].str);
                int _nf = pas_rectype_nf(v[3].str);
                for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) {
                    if (_nf > 0 && _ah >= 0) {
                        pas_array_add2d_param(v[1].list->items[i]->v.sval, _ah, -1);
                        pas_arrrec_add_from_type(v[1].list->items[i]->v.sval, v[3].str);
                    } else if (_nf > 0) {
                        pas_recvar_add_from_type(v[1].list->items[i]->v.sval, v[3].str);
                        pas_array_add2d_param(v[1].list->items[i]->v.sval, (long long)(_nf - 1), -1);
                    } else if (_ah >= 0) {
                        if (_nc >= 0) pas_array_add2d_param(v[1].list->items[i]->v.sval, _ah, _nc);
                        else pas_array_add2d_param(v[1].list->items[i]->v.sval, _ah, -1);
                    }
                }
            }
            out.list = v[1].list;
        }
        return out;
    }
    elab_bad("parameter_decl", n);
    return out;
}
static pval E_parameter_decl_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) { v[1] = E_parameter_decl(ce); { out.list = v[1].list; } } else { v[1] = out; v[3] = E_parameter_decl(ce); { out.list = pnl_concat(v[1].list, v[3].list); } }
    }
    return out;
}
static pval E_parameter_list_opt(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_PARAMS)) { v[2] = E_parameter_decl_list(n, 0, n->n); { out.list = v[2].list; } return out; }
    if (n == NULL) { { out.list = pnl_new(); } return out; }
    elab_bad("parameter_list_opt", n);
    return out;
}
static pval E_pf_params(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_PARAMS)) { v[2] = E_pf_sections(n, 0, n->n); { out.str = v[2].str; } return out; }
    if (n == NULL) { { out.str = ""; } return out; }
    elab_bad("pf_params", n);
    return out;
}
static pval E_pf_section(const tree_t *n) {
    pval v[8];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int hasp = n->n > 1 && n->c[1] && n->c[1]->t == TT_PARAMS;
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "value")) { v[1] = E_id_list(n->c[0], 0, n->c[0]->n); v[3].str = tok_s(n->c[1]); { out.str = pas_pf_sect('v', v[1].list->count, v[3].str); } return out; }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "var")) { v[2] = E_id_list(n->c[0], 0, n->c[0]->n); v[4].str = tok_s(n->c[1]); { out.str = pas_pf_sect('r', v[2].list->count, v[4].str); } return out; }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "procedure")) { v[2].str = tok_s(n->c[0]); v[3] = E_pf_params(hasp ? n->c[1] : NULL); { out.str = pas_pf_sig('P', v[3].str, NULL); } return out; }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "function")) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pf_params(hasp ? n->c[1] : NULL);
        v[5].str = tok_s(n->c[n->n - 1]);
        { out.str = pas_pf_sig('F', v[3].str, v[5].str); }
        return out;
    }
    elab_bad("pf_section", n);
    return out;
}
static pval E_pf_sections(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) { v[1] = E_pf_section(ce); { out.str = v[1].str; } } else { v[1] = out; v[3] = E_pf_section(ce); { out.str = pas_pf_cat3(v[1].str, ";", v[3].str); } }
    }
    return out;
}
static pval E_procedure_decl(const tree_t *n) {
    pval v[13];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int hasp = n->n > 1 && n->c[1] && n->c[1]->t == TT_PARAMS;
    int hasr = n->t == TT_FUNCTION && n->n > (hasp ? 2 : 1) && n->c[hasp ? 2 : 1] && n->c[hasp ? 2 : 1]->t == TT_VAR;
    tree_t *tail = n->c[n->n - 1];
    if (IS(n, TT_PROCEDURE) && IS(tail, TT_KEYWORD)) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pv_mark(NULL);
        v[4] = E_parameter_list_opt(hasp ? n->c[1] : NULL);
        {
            const char *_sg = pas_pf_heading(v[2].str, v[4].list, 'P', NULL);
            pas_pf_define_routine(v[2].str, _sg);
            pas_scope_fwd_save(v[2].str, v[4].list, _sg);
            pas_proc_add(v[2].str);
            pas_proc_vparams(v[2].str, v[4].list);
            pas_fwd_save(v[2].str, v[4].list);
            pas_ptrvar_release();
            pas_recvar_release();
        }
        return out;
    }
    if (IS(n, TT_FUNCTION) && hasr && IS(tail, TT_KEYWORD)) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pv_mark(NULL);
        v[4] = E_parameter_list_opt(hasp ? n->c[1] : NULL);
        v[6].str = tok_s(n->c[hasp ? 2 : 1]);
        {
            const char *_sg = pas_pf_heading(v[2].str, v[4].list, 'F', v[6].str);
            pas_pf_define_routine(v[2].str, _sg);
            pas_scope_fwd_save(v[2].str, v[4].list, _sg);
            pas_func_add(v[2].str);
            pas_proc_vparams(v[2].str, v[4].list);
            if (v[6].str && !strcmp(v[6].str, "char")) pas_charvar_add_outer(v[2].str);
            if (pas_is_booltype(v[6].str)) pas_boolvar_add_outer(v[2].str);
            if (v[6].str) pas_scalarvartype_add(v[2].str, v[6].str);
            pas_fwd_save(v[2].str, v[4].list);
            pas_ptrvar_release();
            pas_recvar_release();
        }
        return out;
    }
    if (IS(n, TT_PROCEDURE)) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pv_mark(NULL);
        v[4] = E_parameter_list_opt(hasp ? n->c[1] : NULL);
        {
            pas_pf_define_routine(v[2].str, pas_pf_heading(v[2].str, v[4].list, 'P', NULL));
            pas_scope_enter(v[2].str, v[4].list);
            pas_proc_add(v[2].str);
            pas_proc_vparams(v[2].str, pas_scope_fwd_params(v[2].str, v[4].list));
            pas_proc_enter();
            pas_fwd_restore(v[2].str);
        }
        v[7] = E_block(tail);
        {
            int d = g_lower.pas.sem.pas_ldepth - 1;
            int d_ok = (d >= 0 && d < PAS_NEST_MAX);
            int dl = d_ok ? g_lower.pas.sem.pas_lstk[d].decl_level : 1;
            const char **ln = d_ok ? g_lower.pas.sem.pas_lstk[d].names : NULL;
            int lc = d_ok ? g_lower.pas.sem.pas_lstk[d].n : 0;
            tree_t *p = mk_proc(v[2].str, pas_fwd_params(v[2].str, v[4].list), pas_trace_wrap_proc(v[2].str, v[4].list, v[7].node, 0), 0, dl, ln, lc);
            pas_proc_exit();
            pas_scope_exit();
            pas_ptrvar_release();
            pas_recvar_release();
            emit_proc(&g_lower.pas.sem.pascal_procs, p);
        }
        return out;
    }
    if (IS(n, TT_FUNCTION) && hasr) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pv_mark(NULL);
        v[4] = E_parameter_list_opt(hasp ? n->c[1] : NULL);
        v[6].str = tok_s(n->c[hasp ? 2 : 1]);
        {
            pas_pf_define_routine(v[2].str, pas_pf_heading(v[2].str, v[4].list, 'F', v[6].str));
            pas_scope_enter(v[2].str, v[4].list);
            pas_func_add(v[2].str);
            pas_proc_vparams(v[2].str, pas_scope_fwd_params(v[2].str, v[4].list));
            if (v[6].str && !strcmp(v[6].str, "char")) pas_charvar_add_outer(v[2].str);
            if (pas_is_booltype(v[6].str)) pas_boolvar_add_outer(v[2].str);
            if (v[6].str) pas_scalarvartype_add(v[2].str, v[6].str);
            pas_proc_enter();
            pas_curfunc_push(v[2].str);
            pas_fwd_restore(v[2].str);
        }
        v[9] = E_block(tail);
        {
            int d = g_lower.pas.sem.pas_ldepth - 1;
            int d_ok = (d >= 0 && d < PAS_NEST_MAX);
            int dl = d_ok ? g_lower.pas.sem.pas_lstk[d].decl_level : 1;
            const char **ln = d_ok ? g_lower.pas.sem.pas_lstk[d].names : NULL;
            int lc = d_ok ? g_lower.pas.sem.pas_lstk[d].n : 0;
            tree_t *p = mk_proc(v[2].str, pas_fwd_params(v[2].str, v[4].list), pas_trace_wrap_proc(v[2].str, v[4].list, pas_result_check(v[2].str, v[9].node), 1), 1, dl, ln, lc);
            pas_proc_exit();
            pas_scope_exit();
            pas_ptrvar_release();
            pas_recvar_release();
            emit_proc(&g_lower.pas.sem.pascal_procs, p);
        }
        return out;
    }
    if (IS(n, TT_FUNCTION)) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pv_mark(NULL);
        {
            pas_pf_define_routine(v[2].str, pas_pf_heading(v[2].str, NULL, 'F', NULL));
            pas_scope_enter(v[2].str, NULL);
            pas_func_add(v[2].str);
            pas_proc_vparams(v[2].str, pas_scope_fwd_params(v[2].str, NULL));
            pas_proc_enter();
            pas_curfunc_push(v[2].str);
            pas_fwd_restore(v[2].str);
        }
        v[6] = E_block(tail);
        {
            int d = g_lower.pas.sem.pas_ldepth - 1;
            int d_ok = (d >= 0 && d < PAS_NEST_MAX);
            int dl = d_ok ? g_lower.pas.sem.pas_lstk[d].decl_level : 1;
            const char **ln = d_ok ? g_lower.pas.sem.pas_lstk[d].names : NULL;
            int lc = d_ok ? g_lower.pas.sem.pas_lstk[d].n : 0;
            tree_t *p = mk_proc(v[2].str, pas_fwd_params(v[2].str, pnl_new()), pas_trace_wrap_proc(v[2].str, NULL, pas_result_check(v[2].str, v[6].node), 1), 1, dl, ln, lc);
            pas_proc_exit();
            pas_scope_exit();
            pas_ptrvar_release();
            pas_recvar_release();
            emit_proc(&g_lower.pas.sem.pascal_procs, p);
        }
        return out;
    }
    elab_bad("procedure_decl", n);
    return out;
}
static pval E_program(const tree_t *n) {
    pval v[9];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int bi = n->n - 1;
    while (bi > 0 && n->c[bi] && n->c[bi]->t == TT_ATTR) bi--;
    int hasf = bi > 1 && n->c[1] && n->c[1]->t == TT_VLIST;
    if (IS(n, TT_PROGRAM)) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_file_id_list_opt(hasf ? n->c[1] : NULL);
        v[5] = E_block((tok_events(n), n->c[bi]));
        {
            tree_t *body = v[5].node;
            if (g_lower.pas.sem.pas_narray > 0 || g_lower.pas.sem.pas_nhdrfile > 0 || g_lower.pas.sem.pas_nscalarvartype > 0) {
                tree_t *combined = ast_node_new(TT_PROGRAM);
                for (int i = 0; i < g_lower.pas.sem.pas_narray; i++) if (!g_lower.pas.sem.pas_arrays[i].is_param && !g_lower.pas.sem.pas_arrays[i].is_local)
                    ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, g_lower.pas.sem.pas_arrays[i].name), mk_array_init(g_lower.pas.sem.pas_arrays[i].name, g_lower.pas.sem.pas_arrays[i].high)));
                for (int i = 0; i < g_lower.pas.sem.pas_nscalarvartype; i++) {
                    const char *vn = g_lower.pas.sem.pas_scalarvartype[i].vname;
                    long long _ah;
                    if (!vn || !g_lower.pas.sem.pas_scalarvartype[i].is_global || pas_is_func(vn) || pas_is_proc(vn) || pas_array_high_get(vn, &_ah)) continue;
                    if (pas_var_typename_is(vn, pas_is_int_typename)) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, vn), ilit(0)));
                    else if (pas_var_typename_is(vn, pas_is_realtypename)) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, vn), flit(0.0)));
                }
                if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(combined, body->c[i]); } else if (body) ast_push(combined, body);
                body = combined;
            }
            body = pas_trace_prepend_tap_off(body);
            tree_t *mainp = mk_proc("main", NULL, body, 0, 0, NULL, 0);
            emit_proc(&g_lower.pas.sem.pascal_procs, mainp);
            for (int i = 0; i < g_lower.pas.sem.pascal_procs.count; i++) pas_pf_resolve(g_lower.pas.sem.pascal_procs.items[i]);
            tree_t *root = ast_stmt_new(TT_PROGRAM);
            for (int i = 0; i < g_lower.pas.sem.pascal_procs.count; i++) ast_push(root, g_lower.pas.sem.pascal_procs.items[i]);
            out.node = root;
        }
        return out;
    }
    elab_bad("program", n);
    return out;
}
static pval E_pv_mark(const tree_t *n) {
    pval v[3];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n == NULL) { { pas_ptrvar_mark(); pas_recvar_mark(); } return out; }
    elab_bad("pv_mark", n);
    return out;
}
static pval E_record_body(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int hasv = n->n > 0 && n->c[n->n - 1] && n->c[n->n - 1]->t == TT_CASE;
    if (IS(n, TT_FIELDS)) { v[1] = E_record_field_list(n, 0, hasv ? n->n - 1 : n->n); v[2] = E_record_case_opt(hasv ? n->c[n->n - 1] : NULL); return out; }
    elab_bad("record_body", n);
    return out;
}
static pval E_record_case_arm(const tree_t *n) {
    pval v[9];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_ARM)) {
        v[1] = E_constant_list(n->c[0], 0, n->c[0]->n);
        v[4] = E_case_arm_mark(NULL);
        v[5] = E_record_body(n->c[1]);
        { pas_vcase_arm_end((int)v[4].ival); pas_vt_arm((int)v[4].ival, g_lower.pas.sem.pas_pend_nf, v[1].node); out.node = v[1].node; }
        return out;
    }
    elab_bad("record_case_arm", n);
    return out;
}
static pval E_record_case_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_record_case_arm(ce);
            { PNodeList *l = pnl_new(); if (v[1].node) pnl_push(l, v[1].node); out.list = l; }
        } else {
            v[1] = out;
            v[3] = E_record_case_arm(ce);
            { if (v[3].node) pnl_push(v[1].list, v[3].node); out.list = v[1].list; }
        }
    }
    return out;
}
static pval E_record_case_opt(const tree_t *n) {
    pval v[10];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_CASE) && n->n >= 2 && n->c[1] && n->c[1]->t == TT_VAR) {
        v[2].str = tok_s(n->c[0]);
        v[4].str = tok_s(n->c[1]);
        v[6] = E_case_open(NULL);
        v[7] = E_record_case_list(n, 2, n->n);
        {
            pas_vcase_pop();
            pas_scope_define(v[2].str);
            pas_variant_constants_in_tag_type(v[4].str, v[7].list);
            if (v[2].str) {
                g_lower.pas.sem.pas_pend_typename = ct_strdup(v[4].str);
                { int _sb = g_lower.pas.sem.pas_pend_isbool; g_lower.pas.sem.pas_pend_isbool = pas_is_booltype(v[4].str); pas_pend_add(v[2].str); g_lower.pas.sem.pas_pend_isbool = _sb; }
                pas_vt_close(g_lower.pas.sem.pas_pend_nf - 1);
            } else pas_vt_close(-1);
        }
        return out;
    }
    if (IS(n, TT_CASE)) {
        v[2].str = tok_s(n->c[0]);
        v[4] = E_case_open(NULL);
        v[5] = E_record_case_list(n, 1, n->n);
        { pas_vcase_pop(); pas_variant_constants_in_tag_type(v[2].str, v[5].list); if (v[2].str) pas_pend_add(v[2].str); pas_vt_close(-1); }
        return out;
    }
    if (n == NULL) { return out; }
    elab_bad("record_case_opt", n);
    return out;
}
static pval E_record_field(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "field")) {
        v[1] = E_id_list(n->c[0], 0, n->c[0]->n);
        { v[3].str = g_lower.pas.sem.pas_pend_typename; g_lower.pas.sem.pas_pend_typename = NULL; g_lower.pas.sem.pas_pend_isbool = 0; }
        v[4] = E_type(n->c[1]);
        {
            pas_scope_define_list(v[1].list);
            int _syn = 0;
            if (v[4].ival == -2 && !g_lower.pas.sem.pas_pend_typename) {
                char _snm[32];
                snprintf(_snm, sizeof _snm, "$anonset%d", ++g_lower.pas.sem.pas_scope.anonseq);
                pas_settype_add(_snm, g_lower.pas.sem.pas_pend_set_hi);
                g_lower.pas.sem.pas_pend_typename = ct_strdup(_snm);
                _syn = 1;
            }
            if (v[1].list) {
                char *_svp = g_lower.pas.sem.pas_pend_ptrtarget;
                int _svc = g_lower.pas.sem.pas_pend_ischar;
                int _sva = g_lower.pas.sem.pas_pend_arr_ischar;
                int _svr = g_lower.pas.sem.pas_pend_isarr;
                for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) {
                    g_lower.pas.sem.pas_pend_ptrtarget = _svp;
                    g_lower.pas.sem.pas_pend_ischar = _svc;
                    g_lower.pas.sem.pas_pend_arr_ischar = _sva;
                    g_lower.pas.sem.pas_pend_isarr = _svr;
                    pas_pend_add(v[1].list->items[i]->v.sval);
                }
            }
            if (!g_lower.pas.sem.pas_pend_typename || _syn) g_lower.pas.sem.pas_pend_typename = v[3].str;
        }
        return out;
    }
    elab_bad("record_field", n);
    return out;
}
static pval E_record_field_list(const tree_t *n, int from, int to) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) { const tree_t *ce = n->c[i]; if (i == from) { v[1] = E_record_field(ce); } else { v[1] = out; v[3] = E_record_field(ce); } }
    return out;
}
static pval E_repeat_statement(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_REPEAT)) {
        v[2] = E_statement_list(n->c[0], 0, n->c[0]->n);
        v[4] = E_expression(n->c[1]);
        { out.node = bin(TT_REPEAT, seq_of(v[2].list), pas_cond_bool(v[4].node, "a repeat-statement", "6.8.3.7")); }
        return out;
    }
    elab_bad("repeat_statement", n);
    return out;
}
static pval E_scalar_constant(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_VAR)) {
        v[1].str = tok_s(n);
        {
            pas_scope_applied(v[1].str);
            long long cv = 0;
            if (v[1].str && !strcmp(v[1].str, "true")) cv = 1;
            else if (v[1].str && !strcmp(v[1].str, "false")) cv = 0;
            else if (!pas_const_get(v[1].str, &cv) && v[1].str && !strcmp(v[1].str, "maxint")) cv = 2147483647;
            g_lower.pas.sem.pas_last_const_enum = pas_const_isenum(v[1].str);
            out.ival = cv;
        }
        return out;
    }
    if (IS(n, TT_ILIT)) { v[1].ival = tok_i(n); { g_lower.pas.sem.pas_last_const_enum = 0; out.ival = v[1].ival; } return out; }
    if (IS(n, TT_FLIT)) { v[1].dval = tok_d(n); { g_lower.pas.sem.pas_last_const_enum = 0; pas_real_is_not_ordinal(v[1].dval); out.ival = (long long)v[1].dval; } return out; }
    if (IS(n, TT_QLIT)) { v[1].str = tok_s(n); { g_lower.pas.sem.pas_last_const_enum = 0; out.ival = (v[1].str && strlen(v[1].str) == 1) ? (long long)(unsigned char)v[1].str[0] : 0; } return out; }
    if (IS(n, TT_CHRLIT)) { v[1].ival = tok_i(n); { g_lower.pas.sem.pas_last_const_enum = 0; out.ival = v[1].ival; } return out; }
    elab_bad("scalar_constant", n);
    return out;
}
static pval E_selector(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_IDX)) {
        v[1] = E_selector(n->c[0]);
        v[3] = E_expression_list(n, 1, n->n);
        {
            tree_t *_rg =
                (v[1].node && v[1].node->t == TT_FNC && v[1].node->n == 6 && v[1].node->c[0] && v[1].node->c[0]->v.sval && !strcmp(v[1].node->c[0]->v.sval, "__pas_rarr") && v[3].list &&
                v[3].list->count == 1) ? pas_region_elem(v[1].node, v[3].list->items[0]) : NULL;
            tree_t *e = NULL;
            if (v[3].list && v[3].list->count == 2 && v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) {
                long long _nc = pas_array_ncols(v[1].node->v.sval);
                if (_nc > 0) { tree_t *flat = bin(TT_ADD, bin(TT_MUL, v[3].list->items[0], ilit(_nc)), v[3].list->items[1]); e = ast_node_new(TT_IDX); ast_push(e, v[1].node); ast_push(e, flat); }
            }
            if (!e && v[3].list && v[3].list->count == 1 && v[1].node && v[1].node->t == TT_IDX && v[1].node->n == 2 && v[1].node->c[0] && v[1].node->c[0]->t == TT_VAR && v[1].node->c[0]->v.sval &&
                !pas_is_nafield(v[1].node)) {
                long long _nc2 = pas_array_ncols(v[1].node->c[0]->v.sval);
                if (_nc2 > 0) {
                    tree_t *flat2 = bin(TT_ADD, bin(TT_MUL, v[1].node->c[1], ilit(_nc2)), v[3].list->items[0]);
                    e = ast_node_new(TT_IDX);
                    ast_push(e, v[1].node->c[0]);
                    ast_push(e, flat2);
                }
            }
            if (!e) {
                e = ast_node_new(TT_IDX);
                ast_push(e, v[1].node);
                if (v[3].list) for (int i = 0; i < v[3].list->count; i++)
                    ast_push(e,
                    (v[3].list->count == 1 && v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && pas_is_pcharvar(v[1].node->v.sval)) ?
                    bin(TT_ADD, bin(TT_ADD, v[3].list->items[i], leaf_s(TT_VAR, pas_pchar_off_name(v[1].node->v.sval))), ilit(1)) :
                    (v[3].list->count == 1 && (g_lower.pas.sem.pas_zerobased_strings & 1) && v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && pas_var_string_kind(v[1].node->v.sval) == 1) ?
                    bin(TT_ADD, v[3].list->items[i], ilit(1)) : v[3].list->items[i]);
            }
            if (e && v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) {
                const char *_et = pas_enumarr_get(v[1].node->v.sval);
                if (_et) { int _ei = pas_enumnames_idx(_et); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); }
            }
            out.node = _rg ? _rg : e;
        }
        return out;
    }
    if (IS(n, TT_FIELD)) {
        v[1] = E_selector(n->c[0]);
        v[3].str = tok_s(n->c[1]);
        {
            int _fi = -1;
            const char *_rt = pas_selector_rectype(v[1].node);
            if (_rt) _fi = pas_rectype_field_index(_rt, v[3].str);
            else if (v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) _fi = pas_recvar_field_index(v[1].node->v.sval, v[3].str);
            if (_fi < 0 && v[1].node && v[1].node->t == TT_IDX && v[1].node->n == 2 && v[1].node->c[0] && v[1].node->c[0]->t == TT_VAR) {
                const char *_arn = NULL;
                int _anf = pas_arrrec_find(v[1].node->c[0]->v.sval, &_arn);
                if (_anf > 0) {
                    int _afi = _arn ? pas_rectype_field_index(_arn, v[3].str) : -1;
                    if (_afi < 0) { _afi = pas_arrrec_field_index(v[1].node->c[0]->v.sval, v[3].str); }
                    if (_afi < 0) {
                        for (int _ri = 0; _ri < g_lower.pas.sem.pas_nrectype; _ri++) {
                            int _t = pas_rectype_field_index(g_lower.pas.sem.pas_rectypes[_ri].tname, v[3].str);
                            if (_t >= 0 && g_lower.pas.sem.pas_rectypes[_ri].nf == _anf) { _afi = _t; break; }
                        }
                    }
                    if (_afi >= 0) {
                        out.node = pas_arrrec_flatten(v[1].node, _afi);
                        if (pas_arrrec_field_is_char(v[1].node->c[0]->v.sval, _afi) || (_arn && pas_rectype_field_is_char(_arn, _afi))) pas_cvfield_mark_add(out.node);
                        const char *_fe = pas_arrrec_field_enum(v[1].node->c[0]->v.sval, _afi);
                        if (!_fe && _arn) _fe = pas_rectype_field_enum_by_index(_arn, _afi);
                        if (_fe && out.node) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) out.node->v.ival = (long long)(_ei + 1); }
                        out.node = pas_arrrec_region(out.node, v[1].node->c[0]->v.sval, _afi);
                        if (_arn && out.node && pas_rectype_field_is_ca(_arn, _afi)) pas_cafield_mark_add(out.node, pas_rectype_field_ca_lo(_arn, _afi), pas_rectype_field_ca_hi(_arn, _afi));
                    } else {
                        out.node = bin(TT_FIELD, v[1].node, leaf_s(TT_VAR, v[3].str));
                    }
                } else {
                    out.node = pas_nested_field_resolve(v[1].node, v[3].str);
                }
            } else if (_fi >= 0) {
                tree_t *e = ast_node_new(TT_IDX);
                ast_push(e, v[1].node);
                ast_push(e, ilit(_rt ? pas_rectype_slot(_rt, _fi) : (v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) ? pas_recvar_slot(v[1].node->v.sval, _fi) : _fi));
                if (_rt) { const char *_fe = pas_rectype_field_enum_by_index(_rt, _fi); if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } }
                {
                    const char *_mrt = _rt ? _rt : pas_with_sel_rtype(v[1].node);
                    if (_mrt && pas_rectype_field_is_ca(_mrt, _fi)) pas_cafield_mark_add(e, pas_rectype_field_ca_lo(_mrt, _fi), pas_rectype_field_ca_hi(_mrt, _fi));
                    if (_mrt && pas_rectype_field_is_char(_mrt, _fi)) pas_cvfield_mark_add(e);
                    if (_mrt && pas_rectype_field_is_na(_mrt, _fi)) pas_nafield_mark_add(e, pas_rectype_field_na_lo(_mrt, _fi));
                }
                pas_vt_mark(e, v[1].node, _rt, _fi);
                { tree_t *_w = pas_field_region(v[1].node, _rt, _fi, e); if (_w != e) pas_vt_rekey(e, _w); out.node = _w; }
            } else {
                out.node = pas_nested_field_resolve(v[1].node, v[3].str);
            }
        }
        return out;
    }
    if (IS(n, TT_DEREF)) {
        v[1] = E_selector(n->c[0]);
        {
            out.node = pas_is_tfile_node(v[1].node) ? mk_fnc1("__pas_fbuf_get", v[1].node) :
                ((v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && pas_is_filevar(v[1].node->v.sval) && !pas_is_stdstream(v[1].node->v.sval)) ? mk_fnc1("__pas_tbuf_get", v[1].node) :
                ((v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && pas_is_pcharvar(v[1].node->v.sval)) ? pas_pchar_deref(v[1].node) :
                ((v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && !strcmp(v[1].node->v.sval, "input") && !pas_ptrvar_target(v[1].node->v.sval)) ? mk_fnc0("__pas_getbufch") :
                mk_deref(v[1].node))));
        }
        return out;
    }
    if (IS(n, TT_VAR)) { v[1].str = tok_s(n); { out.node = mk_ident(v[1].str); } return out; }
    elab_bad("selector", n);
    return out;
}
static pval E_set_member(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_SUBRANGE)) {
        v[1] = E_expression(n->c[0]);
        v[3] = E_expression(n->c[1]);
        { pas_set_member_ordinal(v[1].node); pas_set_member_ordinal(v[3].node); out.node = mk_set_bin("__pas_setrange", v[1].node, v[3].node); }
        return out;
    }
    if (n != NULL) { v[1] = E_expression(n); { pas_set_member_ordinal(v[1].node); PNodeList *_l = pnl_new(); pnl_push(_l, v[1].node); out.node = mk_set_ctor(_l); } return out; }
    elab_bad("set_member", n);
    return out;
}
static pval E_set_member_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) { v[1] = E_set_member(ce); { out.node = v[1].node; } } else { v[1] = out; v[3] = E_set_member(ce); { out.node = mk_set_bin("__pas_setuni", v[1].node, v[3].node); } }
    }
    return out;
}
static pval E_simple_type(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_VAR)) {
        v[1].str = tok_s(n);
        {
            pas_scope_applied(v[1].str);
            long long _esz;
            if (!pas_sizeof_lookup(v[1].str, &_esz)) _esz = -1;
            g_lower.pas.sem.pas_pend_typename = ct_strdup(v[1].str);
            g_lower.pas.sem.pas_pend_isbool = pas_is_booltype(v[1].str);
            g_lower.pas.sem.pas_pend_istfile = pas_is_tfiletype(v[1].str);
            g_lower.pas.sem.pas_pend_ischar = !strcmp(v[1].str, "char") || !strcmp(v[1].str, "widechar") || pas_typealias_is_char(v[1].str);
            const char *_pt = pas_ptrtype_target(v[1].str);
            if (_pt) {
                g_lower.pas.sem.pas_pend_ptrtarget = ct_strdup(_pt);
                out.ival = -3;
            } else {
                if (!strcmp(v[1].str, "char") || !strcmp(v[1].str, "widechar")) {
                    out.ival = 255;
                } else if (pas_is_settype(v[1].str)) {
                    out.ival = -2;
                } else {
                    long long _eh = pas_enumtype_high(v[1].str);
                    long long _sh = pas_subtype_high(v[1].str);
                    long long _ah = pas_arrtype_high(v[1].str);
                    if (_eh >= 0) {
                        out.ival = _eh;
                    } else if (_sh >= 0) {
                        out.ival = _sh;
                    } else if (_ah >= 0) {
                        if (g_lower.pas.sem.pas_recbody_depth == 0 && pas_rectype_nf(v[1].str) > 0) { pas_rectype_to_pend(v[1].str); g_lower.pas.sem.pas_pend_typename = ct_strdup(v[1].str); }
                        out.ival = _ah;
                    } else {
                        if (g_lower.pas.sem.pas_recbody_depth == 0) pas_rectype_to_pend(v[1].str);
                        g_lower.pas.sem.pas_pend_typename = ct_strdup(v[1].str);
                        out.ival = -1;
                    }
                }
            }
            g_lower.pas.sem.pas_pend_esz = _esz;
        }
        return out;
    }
    if (IS(n, TT_ENUM_TYPE)) {
        v[2] = E_id_list(n, 0, n->n);
        {
            pas_scope_define_list(v[2].list);
            int _eo = 0;
            g_lower.pas.sem.pas_pend_enum_names[0] = '\0';
            if (v[2].list) for (int i = 0; i < v[2].list->count; i++) {
                tree_t *_id = v[2].list->items[i];
                if (_id && _id->v.sval) {
                    if (_eo > 0) strncat(g_lower.pas.sem.pas_pend_enum_names, ",", sizeof g_lower.pas.sem.pas_pend_enum_names - strlen(g_lower.pas.sem.pas_pend_enum_names) - 1);
                    strncat(g_lower.pas.sem.pas_pend_enum_names, _id->v.sval, sizeof g_lower.pas.sem.pas_pend_enum_names - strlen(g_lower.pas.sem.pas_pend_enum_names) - 1);
                    pas_const_add(_id->v.sval, (long long)(_eo));
                    pas_const_mark_enum(_id->v.sval);
                    _eo++;
                }
            }
            g_lower.pas.sem.pas_pend_enum_max = (long long)(_eo - 1);
            g_lower.pas.sem.pas_pend_esz = -1;
            out.ival = _eo > 0 ? (long long)(_eo - 1) : -1;
        }
        return out;
    }
    if (IS(n, TT_SUBRANGE) && IS(n->c[0], TT_QLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[3] = E_constant(n->c[1]);
        {
            long long _l = (v[1].str && strlen(v[1].str) == 1) ? (long long)(unsigned char)v[1].str[0] : 0;
            g_lower.pas.sem.pas_pend_sub_low = _l;
            g_lower.pas.sem.pas_pend_sub_high = v[3].ival;
            g_lower.pas.sem.pas_pend_ischar = 1;
            g_lower.pas.sem.pas_pend_esz = 1;
            out.ival = v[3].ival;
        }
        return out;
    }
    if (IS(n, TT_SUBRANGE) && IS(n->c[0], TT_CHRLIT)) {
        v[1].ival = tok_i(n->c[0]);
        v[3] = E_constant(n->c[1]);
        { g_lower.pas.sem.pas_pend_sub_low = v[1].ival; g_lower.pas.sem.pas_pend_sub_high = v[3].ival; g_lower.pas.sem.pas_pend_ischar = 1; g_lower.pas.sem.pas_pend_esz = 1; out.ival = v[3].ival; }
        return out;
    }
    if (IS(n, TT_SUBRANGE)) {
        v[1] = E_constant(n->c[0]);
        v[3] = E_constant(n->c[1]);
        {
            g_lower.pas.sem.pas_pend_sub_low = v[1].ival;
            g_lower.pas.sem.pas_pend_sub_high = v[3].ival;
            g_lower.pas.sem.pas_pend_ischar = 0;
            g_lower.pas.sem.pas_pend_esz = (v[3].ival >= v[1].ival) ? pas_range_fit_size(v[1].ival, v[3].ival) : -1;
            out.ival = v[3].ival;
        }
        return out;
    }
    elab_bad("simple_type", n);
    return out;
}
static pval E_statement(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_LABEL_DEF)) {
        v[1].ival = tok_i(n->c[0]);
        v[3] = E_statement_no_label(n->c[1]);
        {
            pas_label_in_range(v[1].ival);
            pas_label_prefix(v[1].ival);
            char _lb[24];
            snprintf(_lb, sizeof _lb, "%lld", (long long)v[1].ival);
            tree_t *L = ast_node_new(TT_LABEL_DEF);
            L->v.sval = ct_strdup(_lb);
            ast_push(L, v[3].node);
            out.node = L;
        }
        return out;
    }
    if (n != NULL) { v[1] = E_statement_no_label(n); { out.node = v[1].node; } return out; }
    elab_bad("statement", n);
    return out;
}
static pval E_statement_list(const tree_t *n, int from, int to) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            { }
            v[2] = E_statement(ce);
            {
                int _ln = ce->line;
                PNodeList *l = pnl_new();
                if (v[2].node && !v[2].node->line) v[2].node->line = _ln;
                if (v[2].node) { if (pas_trace_enabled() && v[2].node->t != TT_SUCCEED) pnl_push(l, mk_fnc1("__trace_stmt", ilit(_ln))); pnl_push(l, v[2].node); }
                out.list = l;
            }
        } else {
            v[1] = out;
            { }
            v[4] = E_statement(ce);
            {
                int _ln = ce->line;
                if (v[4].node && !v[4].node->line) v[4].node->line = _ln;
                if (v[4].node) { if (pas_trace_enabled() && v[4].node->t != TT_SUCCEED) pnl_push(v[1].list, mk_fnc1("__trace_stmt", ilit(_ln))); pnl_push(v[1].list, v[4].node); }
                out.list = v[1].list;
            }
        }
    }
    return out;
}
static pval E_statement_no_label(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_ASSIGN)) { v[1] = E_assignment(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_FNC)) { v[1] = E_call(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_SEQ_EXPR)) { v[1] = E_compound_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_GOTO_U)) { v[1] = E_goto_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_IF)) { v[1] = E_if_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_CASE)) { v[1] = E_case_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_WHILE)) { v[1] = E_while_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_REPEAT)) { v[1] = E_repeat_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_FOR)) { v[1] = E_for_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_WITH)) { v[1] = E_with_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_SUCCEED)) { { out.node = ast_node_new(TT_SUCCEED); } return out; }
    elab_bad("statement_no_label", n);
    return out;
}
static pval E_type(const tree_t *n) {
    pval v[12];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int off = (n->n > 0 && n->c[0] && n->c[0]->t == TT_KEYWORD) ? 1 : 0;
    if (IS(n, TT_PTR_TYPE)) { v[2].str = tok_s(n->c[0]); { g_lower.pas.sem.pas_pend_esz = -1; g_lower.pas.sem.pas_pend_ptrtarget = ct_strdup(v[2].str); out.ival = -3; } return out; }
    if (IS(n, TT_ARRAY_TYPE) && n->n - ((n->n > 0 && n->c[0] && n->c[0]->t == TT_KEYWORD) ? 1 : 0) == 2) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        v[4] = E_simple_type(n->c[off]);
        { v[7].ival = pas_index_bound(v[4].ival, 0); g_lower.pas.sem.pas_last_const_enum = pas_index_is_enum(v[4].ival); }
        { v[8].ival = pas_index_bound(v[4].ival, 1); g_lower.pas.sem.pas_pend_esz = -1; }
        v[9] = E_type(n->c[off + 1]);
        {
            g_lower.pas.sem.pas_pend_esz = (g_lower.pas.sem.pas_pend_esz >= 0 && v[8].ival >= v[7].ival) ? g_lower.pas.sem.pas_pend_esz * (v[8].ival - v[7].ival + 1) : -1;
            int _eic = g_lower.pas.sem.pas_pend_ischar;
            if (_eic &&
                (!v[1].ival || !g_lower.pas.sem.pas_pend_typename || strcmp(g_lower.pas.sem.pas_pend_typename, "char") || v[7].ival != 1 || v[8].ival < 2 || g_lower.pas.sem.pas_last_const_enum))
                _eic |= PAS_CA_NOSTR;
            int _wr = g_lower.pas.sem.pas_pend_arr_ischar || (g_lower.pas.sem.pas_pend_typename && pas_arrtype_ischar(g_lower.pas.sem.pas_pend_typename));
            g_lower.pas.sem.pas_pend_arr_wrap = _wr;
            g_lower.pas.sem.pas_pend_arr_ptrto = g_lower.pas.sem.pas_pend_ptrtarget ? g_lower.pas.sem.pas_pend_ptrtarget :
                (g_lower.pas.sem.pas_pend_typename ? (char *)pas_ptrtype_target(g_lower.pas.sem.pas_pend_typename) : NULL);
            g_lower.pas.sem.pas_pend_ptrtarget = NULL;
            g_lower.pas.sem.pas_pend_arr_ncols = -1;
            g_lower.pas.sem.pas_pend_arr_ischar = _eic;
            g_lower.pas.sem.pas_pend_isarr = 1;
            if (v[8].ival >= v[7].ival) { g_lower.pas.sem.pas_pend_sub_low = v[7].ival; g_lower.pas.sem.pas_pend_sub_high = v[8].ival; }
            out.ival = (v[4].ival >= 0 || v[8].ival < v[7].ival) ? v[4].ival : v[8].ival;
        }
        return out;
    }
    if (IS(n, TT_ARRAY_TYPE)) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        v[4] = E_simple_type(n->c[off]);
        v[6] = E_simple_type(n->c[off + 1]);
        v[9] = E_type(n->c[off + 2]);
        {
            g_lower.pas.sem.pas_pend_esz = -1;
            int _eic = g_lower.pas.sem.pas_pend_ischar;
            g_lower.pas.sem.pas_pend_ptrtarget = NULL;
            long long r = v[4].ival;
            long long c = v[6].ival;
            g_lower.pas.sem.pas_pend_arr_ncols = c + 1;
            g_lower.pas.sem.pas_pend_arr_ischar = _eic;
            g_lower.pas.sem.pas_pend_isarr = 1;
            out.ival = (r + 1) * (c + 1) - 1;
        }
        return out;
    }
    if (IS(n, TT_RECORD)) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        { g_lower.pas.sem.pas_recbody_depth++; if (g_lower.pas.sem.pas_recbody_depth > 1) pas_pend_nest_save(); }
        v[4] = E_record_body(n->c[off]);
        {
            if (g_lower.pas.sem.pas_recbody_depth > 1) {
                char _anm[32];
                snprintf(_anm, sizeof _anm, "$anonrec%d", ++g_lower.pas.sem.pas_scope.anonseq);
                pas_rectype_add(_anm, v[1].ival ? 1 : 0);
                pas_pend_nest_restore();
                g_lower.pas.sem.pas_pend_typename = ct_strdup(_anm);
            }
            g_lower.pas.sem.pas_recbody_depth--;
            g_lower.pas.sem.pas_pend_ptrtarget = NULL;
            g_lower.pas.sem.pas_pend_sub_low = 0;
            g_lower.pas.sem.pas_pend_sub_high = -1;
            g_lower.pas.sem.pas_pend_enum_max = -1;
            g_lower.pas.sem.pas_pend_arr_ncols = -1;
            g_lower.pas.sem.pas_pend_ischar = 0;
            g_lower.pas.sem.pas_pend_arr_ischar = 0;
            g_lower.pas.sem.pas_pend_esz = (g_lower.pas.sem.pas_recbody_depth == 0 && g_lower.pas.sem.pas_pend_nf == 0) ? 0 : -1;
            out.ival = v[1].ival ? -4 : -1;
        }
        return out;
    }
    if (IS(n, TT_SET_TYPE)) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        v[4] = E_simple_type(n->c[off]);
        { g_lower.pas.sem.pas_pend_esz = -1; g_lower.pas.sem.pas_pend_ptrtarget = NULL; g_lower.pas.sem.pas_pend_set_hi = v[4].ival; out.ival = -2; }
        return out;
    }
    if (IS(n, TT_FILE_TYPE) && n->n - ((n->n > 0 && n->c[0] && n->c[0]->t == TT_KEYWORD) ? 1 : 0) == 0) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        { g_lower.pas.sem.pas_pend_esz = -1; g_lower.pas.sem.pas_pend_ptrtarget = NULL; out.ival = -1; }
        return out;
    }
    if (IS(n, TT_FILE_TYPE)) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        v[4] = E_type(n->c[off]);
        {
            int _cf = g_lower.pas.sem.pas_pend_istfile ||
                (g_lower.pas.sem.pas_pend_typename && (!strcmp(g_lower.pas.sem.pas_pend_typename, "text") || pas_rectype_has_file(g_lower.pas.sem.pas_pend_typename)));
            for (int _i = 0; !_cf && _i < g_lower.pas.sem.pas_pend_nf; _i++) if (g_lower.pas.sem.pas_pend_fldfile[_i]) _cf = 1;
            if (_cf) {
                fprintf(stderr,
                    "pascal: ISO 7185 6.4.3.5 violation: the component-type of a file-type shall not be a file-type nor a structured-type having a file-type as a component -- 'file of %s'\n",
                    g_lower.pas.sem.pas_pend_typename ? g_lower.pas.sem.pas_pend_typename : "<file>");
                g_lower.pas.sem.pas_iso_errors++;
            }
            if (g_lower.pas.sem.pas_pend_sub_high >= 0) {
                g_lower.pas.sem.pas_pend_frng_lo = g_lower.pas.sem.pas_pend_sub_low;
                g_lower.pas.sem.pas_pend_frng_hi = g_lower.pas.sem.pas_pend_sub_high;
            } else if (g_lower.pas.sem.pas_pend_typename && pas_subtype_high(g_lower.pas.sem.pas_pend_typename) >= 0) {
                g_lower.pas.sem.pas_pend_frng_lo = pas_subtype_low(g_lower.pas.sem.pas_pend_typename);
                g_lower.pas.sem.pas_pend_frng_hi = pas_subtype_high(g_lower.pas.sem.pas_pend_typename);
            }
            g_lower.pas.sem.pas_pend_frng_ischar = g_lower.pas.sem.pas_pend_ischar;
            pas_pend_reset();
            g_lower.pas.sem.pas_pend_esz = -1;
            g_lower.pas.sem.pas_pend_istfile = 1;
            out.ival = -1;
        }
        return out;
    }
    if (n != NULL) {
        v[1] = E_simple_type(n);
        {
            if (g_lower.pas.sem.pas_pend_ptrtarget) {
                out.ival = -3;
            } else if (v[1].ival == -2) {
                out.ival = -2;
            } else if (v[1].ival >= 0 && g_lower.pas.sem.pas_pend_typename && pas_arrtype_high(g_lower.pas.sem.pas_pend_typename) >= 0) {
                long long _tnc = pas_arrtype_ncols(g_lower.pas.sem.pas_pend_typename);
                if (_tnc >= 0) g_lower.pas.sem.pas_pend_arr_ncols = _tnc;
                out.ival = v[1].ival;
            } else {
                out.ival = -1;
            }
        }
        return out;
    }
    elab_bad("type", n);
    return out;
}
static pval E_type_decl(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "type")) {
        v[1].str = tok_s(n->c[0]);
        v[3] = E_type(n->c[1]);
        {
            pas_scope_define(v[1].str);
            pas_type_not_self_applied(v[1].str);
            pas_tsz_add(v[1].str, g_lower.pas.sem.pas_pend_esz);
            if (g_lower.pas.sem.pas_pend_ischar && g_lower.pas.sem.pas_pend_sub_high >= 0 && !g_lower.pas.sem.pas_pend_isarr && g_lower.pas.sem.pas_pend_nf == 0) pas_typealias_add(v[1].str, "char");
            long long _ty3 = (v[3].ival == -4) ? -1 : v[3].ival;
            int _pk3 = (v[3].ival == -4) || (g_lower.pas.sem.pas_pend_typename && pas_rectype_is_packed(g_lower.pas.sem.pas_pend_typename));
            if (_ty3 < 0 && g_lower.pas.sem.pas_pend_istfile) pas_tfiletype_add(v[1].str);
            if (_ty3 < 0 && _ty3 != -2 && _ty3 != -3 && g_lower.pas.sem.pas_pend_isbool) pas_booltype_add(v[1].str);
            if (_ty3 == -2) pas_settype_add(v[1].str, g_lower.pas.sem.pas_pend_set_hi);
            if (g_lower.pas.sem.pas_pend_ptrtarget) pas_ptrtype_add(v[1].str, g_lower.pas.sem.pas_pend_ptrtarget);
            else if (g_lower.pas.sem.pas_pend_nf > 0) pas_rectype_add(v[1].str, _pk3);
            if (g_lower.pas.sem.pas_pend_enum_max >= 0) { pas_enumtype_add(v[1].str, g_lower.pas.sem.pas_pend_enum_max); pas_enumnames_add(v[1].str, g_lower.pas.sem.pas_pend_enum_names); }
            if (g_lower.pas.sem.pas_pend_sub_high >= 0 && _ty3 < 0 && g_lower.pas.sem.pas_pend_arr_ncols < 0)
                pas_subtype_add(v[1].str, g_lower.pas.sem.pas_pend_sub_low, g_lower.pas.sem.pas_pend_sub_high);
            if (_ty3 >= 0 && !g_lower.pas.sem.pas_pend_ptrtarget) { pas_arrtype_add(v[1].str, _ty3, g_lower.pas.sem.pas_pend_arr_ncols >= 0 ? 1 : 0, g_lower.pas.sem.pas_pend_arr_ncols); }
            if (_ty3 == -1 && !g_lower.pas.sem.pas_pend_ptrtarget && g_lower.pas.sem.pas_pend_nf == 0 && g_lower.pas.sem.pas_pend_enum_max < 0 && g_lower.pas.sem.pas_pend_sub_high < 0 &&
                g_lower.pas.sem.pas_pend_typename) pas_typealias_add(v[1].str, g_lower.pas.sem.pas_pend_typename);
            pas_pend_reset();
        }
        return out;
    }
    elab_bad("type_decl", n);
    return out;
}
static pval E_type_decl_list(const tree_t *n, int from, int to) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) { const tree_t *ce = n->c[i]; if (i == from) { v[1] = E_type_decl(ce); } else { v[1] = out; v[2] = E_type_decl(ce); } }
    return out;
}
static pval E_var_decl(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "var")) {
        v[1] = E_id_list(n->c[0], 0, n->c[0]->n);
        v[3] = E_type(n->c[1]);
        {
            pas_scope_define_list(v[1].list);
            if (v[1].list) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) pas_tsz_add(v[1].list->items[i]->v.sval, g_lower.pas.sem.pas_pend_esz);
            long long _ty3 = (v[3].ival == -4) ? -1 : v[3].ival;
            int _pk3 = (v[3].ival == -4) || (g_lower.pas.sem.pas_pend_typename && pas_rectype_is_packed(g_lower.pas.sem.pas_pend_typename));
            if (v[1].list) for (int i = 0; i < v[1].list->count; i++) {
                tree_t *id = v[1].list->items[i];
                if (id && id->v.sval) {
                    if (_ty3 == -3) {
                        if (g_lower.pas.sem.pas_pend_ptrtarget) pas_ptrvar_add(id->v.sval, g_lower.pas.sem.pas_pend_ptrtarget);
                    } else {
                        if (_ty3 >= 0 && g_lower.pas.sem.pas_pend_nf > 0) {
                            if (g_lower.pas.sem.pas_pend_arr_ncols >= 0) pas_array_add2d(id->v.sval, _ty3, g_lower.pas.sem.pas_pend_arr_ncols);
                            else pas_array_add(id->v.sval, _ty3);
                            pas_arrrec_add(id->v.sval, g_lower.pas.sem.pas_pend_typename, g_lower.pas.sem.pas_pend_nf);
                        } else if (_ty3 >= 0) {
                            long long _varnc = (g_lower.pas.sem.pas_pend_arr_ncols >= 0) ? g_lower.pas.sem.pas_pend_arr_ncols : pas_arrtype_ncols(g_lower.pas.sem.pas_pend_typename);
                            if (g_lower.pas.sem.pas_pend_ischar && !g_lower.pas.sem.pas_pend_arr_ischar && _varnc < 0 && g_lower.pas.sem.pas_pend_nf == 0) {
                                pas_charvar_add(id->v.sval);
                            } else if (_varnc >= 0) {
                                pas_array_add2d(id->v.sval, _ty3, _varnc);
                                if ((g_lower.pas.sem.pas_pend_isbool && !g_lower.pas.sem.pas_pend_ischar) || pas_arrtype_isbool(g_lower.pas.sem.pas_pend_typename)) pas_array_mark_bool(id->v.sval);
                            } else {
                                pas_array_add(id->v.sval, _ty3);
                                if ((g_lower.pas.sem.pas_pend_isbool && !g_lower.pas.sem.pas_pend_ischar) || pas_arrtype_isbool(g_lower.pas.sem.pas_pend_typename)) pas_array_mark_bool(id->v.sval);
                                if (g_lower.pas.sem.pas_pend_arr_ptrto) pas_arrptr_add(id->v.sval, g_lower.pas.sem.pas_pend_arr_ptrto);
                                int _aic = g_lower.pas.sem.pas_pend_arr_ischar || (g_lower.pas.sem.pas_pend_typename && pas_arrtype_ischar(g_lower.pas.sem.pas_pend_typename));
                                if (_aic && g_lower.pas.sem.pas_pend_arr_wrap)
                                    pas_strarr_add2(id->v.sval,
                                    (g_lower.pas.sem.pas_pend_typename && pas_arrtype_lo(g_lower.pas.sem.pas_pend_typename) > 0) ? pas_arrtype_lo(g_lower.pas.sem.pas_pend_typename) : 1);
                                else if (_aic) {
                                    pas_chararr_add2(id->v.sval,
                                        g_lower.pas.sem.pas_pend_arr_ischar ? (g_lower.pas.sem.pas_pend_sub_low > 0 ? g_lower.pas.sem.pas_pend_sub_low : 0) :
                                        pas_arrtype_lo(g_lower.pas.sem.pas_pend_typename));
                                    pas_chararr_shape(id->v.sval,
                                        (g_lower.pas.sem.pas_pend_arr_ischar & PAS_CA_NOSTR) || (g_lower.pas.sem.pas_pend_typename && pas_arrtype_nostr(g_lower.pas.sem.pas_pend_typename)),
                                        g_lower.pas.sem.pas_pend_arr_ischar ? g_lower.pas.sem.pas_pend_sub_high : pas_arrtype_high(g_lower.pas.sem.pas_pend_typename));
                                } else if (g_lower.pas.sem.pas_pend_typename && pas_enumnames_idx(g_lower.pas.sem.pas_pend_typename) >= 0)
                                    pas_enumarr_add(id->v.sval, g_lower.pas.sem.pas_pend_typename);
                            }
                        }
                        if (_ty3 == -2) pas_setvar_add(id->v.sval);
                        if (_ty3 < 0 && g_lower.pas.sem.pas_pend_ischar) pas_charvar_add(id->v.sval);
                        if (_ty3 < 0 && _ty3 != -2 && _ty3 != -3 && g_lower.pas.sem.pas_pend_isbool) pas_boolvar_add(id->v.sval);
                        if (_ty3 < 0 && g_lower.pas.sem.pas_pend_istfile) {
                            pas_tfilevar_add(id->v.sval);
                            pas_tfcomp_add(id->v.sval, g_lower.pas.sem.pas_pend_frng_lo, g_lower.pas.sem.pas_pend_frng_hi, g_lower.pas.sem.pas_pend_frng_ischar);
                        }
                        if (_ty3 < 0 && !g_lower.pas.sem.pas_pend_istfile && g_lower.pas.sem.pas_pend_nf == 0 && !g_lower.pas.sem.pas_pend_isarr && g_lower.pas.sem.pas_pend_sub_high >= 0) {
                            pas_subvar_add(id->v.sval, g_lower.pas.sem.pas_pend_sub_low, g_lower.pas.sem.pas_pend_sub_high);
                        }
                            else if (_ty3 < 0 && !g_lower.pas.sem.pas_pend_istfile && g_lower.pas.sem.pas_pend_nf == 0 && !g_lower.pas.sem.pas_pend_isarr && g_lower.pas.sem.pas_pend_typename &&
                            pas_subtype_high(g_lower.pas.sem.pas_pend_typename) >= 0) {
                            pas_subvar_add(id->v.sval, pas_subtype_low(g_lower.pas.sem.pas_pend_typename), pas_subtype_high(g_lower.pas.sem.pas_pend_typename));
                        }
                        if (_ty3 < 0 && g_lower.pas.sem.pas_pend_nf > 0) { pas_recvar_add(id->v.sval, _pk3); pas_array_add(id->v.sval, (long long)(g_lower.pas.sem.pas_pend_nf - 1)); }
                        if (g_lower.pas.sem.pas_pend_typename && !strcmp(g_lower.pas.sem.pas_pend_typename, "text")) pas_filevar_add(id->v.sval);
                        if (g_lower.pas.sem.pas_pend_typename && !strcmp(g_lower.pas.sem.pas_pend_typename, "pchar")) pas_pcharvar_add(id->v.sval);
                        if (g_lower.pas.sem.pas_pend_typename) pas_scalarvartype_add(id->v.sval, g_lower.pas.sem.pas_pend_typename);
                    }
                    pas_local_add(id->v.sval);
                }
            }
            if (g_lower.pas.sem.pas_pend_typename && !strcmp(g_lower.pas.sem.pas_pend_typename, "pchar") && v[1].list) {
                int _n0 = v[1].list->count;
                for (int _hi = 0; _hi < _n0; _hi++) {
                    tree_t *_id = v[1].list->items[_hi];
                    if (_id && _id->v.sval) { char *_hn = pas_pchar_off_name(_id->v.sval); pas_scope_define(_hn); pas_local_add(_hn); pnl_push(v[1].list, leaf_s(TT_VAR, _hn)); }
                }
            }
            g_lower.pas.sem.pas_pend_frng_lo = 0;
            g_lower.pas.sem.pas_pend_frng_hi = -1;
            g_lower.pas.sem.pas_pend_frng_ischar = 0;
            pas_pend_reset();
            out.list = v[1].list;
        }
        return out;
    }
    elab_bad("var_decl", n);
    return out;
}
static pval E_var_decl_list(const tree_t *n, int from, int to) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_var_decl(ce);
            { out.list = pas_var_names_add(pnl_new(), v[1].list); }
        } else {
            v[1] = out;
            v[2] = E_var_decl(ce);
            { out.list = pas_var_names_add(v[1].list, v[2].list); }
        }
    }
    return out;
}
static pval E_while_statement(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_WHILE)) { v[2] = E_expression(n->c[0]); v[4] = E_body_stmt(n->c[1]); { out.node = bin(TT_WHILE, pas_cond_bool(v[2].node, "a while-statement", "6.8.3.8"), v[4].node); } return out; }
    elab_bad("while_statement", n);
    return out;
}
static pval E_with_open(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_selector(ce);
            { pas_with_push_once(v[1].node); out.ival = 1; }
        } else {
            v[1] = out;
            v[3] = E_selector(ce);
            { pas_with_push_once(v[3].node); out.ival = v[1].ival + 1; }
        }
    }
    return out;
}
static pval E_with_statement(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_WITH)) { v[2] = E_with_open(n, 0, n->n - 1); v[4] = E_body_stmt(n->c[n->n - 1]); { out.node = pas_with_finish(v[2].ival, v[4].node); } return out; }
    elab_bad("with_statement", n);
    return out;
}
tree_t *lower_pascal_tree(tree_t *pruned, const char *filename) {
    if (!pruned) return NULL;
    if (!filename) filename = "<stdin>";
    memset(&g_lower.pas.sem, 0, sizeof g_lower.pas.sem);
    g_lower.pas.sem.pas_min_enum_size = 4;
    g_lower.pas.sem.pas_range_check_on = 2;
    g_lower.pas.sem.pas_pend_esz = -1;
    g_lower.pas.sem.pas_level = 1;
    g_lower.pas.sem.pas_pend_set_hi = -1;
    g_lower.pas.sem.pas_pend_frng_hi = -1;
    g_lower.pas.sem.pas_ncafield = 0;
    g_lower.pas.sem.pas_nnafield = 0;
    g_lower.pas.sem.pas_ncvfield = 0;
    g_lower.pas.sem.pas_pend_arr_ncols = -1;
    g_lower.pas.sem.pas_pend_enum_max = -1;
    g_lower.pas.sem.pas_pend_sub_high = -1;
    tree_t *root = E_program(pruned).node;
    int nsem = pascal_sem_check(root, filename) + g_lower.pas.sem.pas_iso_errors;
    if (nsem > 0) { fprintf(stderr, "pascal: %d ISO 7185 violation(s) in %s -- no code generated\n", nsem, filename); return NULL; }
    return root;
}
