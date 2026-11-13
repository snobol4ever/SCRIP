#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include "ir/ast.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_08_pas.h"
#include "lower/lower_pascal_tree/lpt_11_sh.h"
static const char *pas_elem_rectype(tree_t *e);
static const char *pas_recvar_field_rectype(const char *vn, long idx);
static int pas_recvar_field_is_bool(const char *vn, long idx);
static int pas_rectype_field_is_bool(const char *rn, long idx);
static int pas_rtf(const char *rn, long idx, struct pty_fld *f) { const pty_t *t = pty_rec_by_name(rn); return t && idx >= 0 && pty_rec_fld(t, (int)idx, f); }
const pty_t *pas_rvt(const char *vn) { const pty_t *t = vn ? pty_name_type(vn) : NULL; return t && t->k == PTK_RECORD && t->lay ? t : NULL; }
int pas_rvf(const char *vn, long idx, struct pty_fld *f) { const pty_t *t = pas_rvt(vn); return t && idx >= 0 && pty_rec_fld(t, (int)idx, f); }
const char *pas_fld_rectype(const struct pty_fld *f) {
    if (!f->rec || !f->ft) return NULL;
    const pty_t *x = f->ft;
    if (x->k == PTK_ARRAY) { if (x->name && pty_type_by_name(x->name) == x) return x->name; while (x && x->k == PTK_ARRAY) x = x->base; }
    return pty_name_of(x);
}
struct pas_vt *pas_vt_new(int state) {
    if (g_lower_pas.sem.pas_nvt >= g_lower_pas.sem.pas_vtcap) {
        g_lower_pas.sem.pas_vtcap = g_lower_pas.sem.pas_vtcap ? g_lower_pas.sem.pas_vtcap * 2 : 32;
        g_lower_pas.sem.pas_vt = (struct pas_vt **)ct_grow(g_lower_pas.sem.pas_vt, (size_t)g_lower_pas.sem.pas_vtcap * sizeof *g_lower_pas.sem.pas_vt);
    }
    struct pas_vt *v = (struct pas_vt *)ct_zalloc(1, sizeof *v);
    v->state = state;
    g_lower_pas.sem.pas_vt[g_lower_pas.sem.pas_nvt++] = v;
    return v;
}
const char *pas_trace_enum_names_of_var(const char *vn) {
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
const char *pas_trace_store_name(const tree_t *lhs) {
    if (!lhs) return NULL;
    if (lhs->t == TT_VAR && lhs->v.sval) return lhs->v.sval;
    const char *cf = pas_curfunc_name();
    if (cf && lhs->t == TT_FNC && lhs->n >= 1 && lhs->c[0] && lhs->c[0]->t == TT_VAR && lhs->c[0]->v.sval && !strcmp(lhs->c[0]->v.sval, cf)) return cf;
    return NULL;
}
int pas_ty_dyn_depth(tree_t *v, const pty_t **leaf) { const pty_t *t = pty_of_low(v); int d = 0; while (t && t->k == PTK_ARRAY && t->dyn) { d++; t = t->base; } if (leaf) *leaf = t; return d; }
tree_t *pas_ty_dyn_proto(const pty_t *q) {
    const char *qn = q ? pty_name_of(q) : NULL;
    if (!q) return ilit(0);
    if (q->k == PTK_REAL) return flit(0.0);
    if (q->k == PTK_STRING) return leaf_s(TT_QLIT, "");
    if (q->k == PTK_CHAR) return mk_chrlit(ilit(0));
    if (q->k == PTK_RECORD && qn && pas_rectype_nf(qn) > 0) return mk_rec_init_type(qn);
    return ilit(0);
}
int pas_enumnames_idx(const char *tn) {
    if (!tn) return -1;
    for (int i = 0; i < g_lower_pas.sem.pas_nenumname; i++) if (g_lower_pas.sem.pas_enumnames[i].tname && !strcmp(g_lower_pas.sem.pas_enumnames[i].tname, tn)) return i;
    return -1;
}
static int pas_rectype_field_index(const char *rn, const char *fn) {
    const pty_t *t = pty_rec_by_name(rn);
    struct pty_fld g;
    for (int j = 0; fn && t && j < pty_rec_nf(t); j++) if (pty_rec_fld(t, j, &g) && g.name && !strcmp(g.name, fn)) return j;
    return -1;
}
int pas_rectype_has_file(const char *rn) {
    const pty_t *t = pty_rec_by_name(rn);
    struct pty_fld g;
    for (int j = 0; t && j < pty_rec_nf(t); j++) if (pty_rec_fld(t, j, &g) && g.file) return 1;
    return 0;
}
int pas_rectype_nf(const char *rn) { return pty_rec_nf(pty_rec_by_name(rn)); }
int pas_pend_is_rectype(const char *rn) {
    const pty_t *t = rn ? pty_rec_by_name(rn) : NULL;
    if (!t || pty_rec_nf(t) <= 0 || pty_rec_nf(t) != g_lower_pas.sem.pas_pend_nf) return 0;
    for (int j = 0; j < g_lower_pas.sem.pas_pend_nf; j++) {
        struct pty_fld f;
        const char *b = g_lower_pas.sem.pas_pend_fields[j];
        if (!pty_rec_fld(t, j, &f) || (f.name != b && (!f.name || !b || strcmp(f.name, b)))) return 0;
    }
    return 1;
}
static int pas_rectype_field_is_ca(const char *rn, long idx) { struct pty_fld f; return pas_rtf(rn, idx, &f) ? f.ca != 0 : 0; }
static int pas_rectype_field_is_char(const char *rn, long idx) { struct pty_fld f; return pas_rtf(rn, idx, &f) ? f.ch == 1 : 0; }
const char *pas_rectype_field_typename(const char *rn, long idx) { const pty_t *t = rn && idx >= 0 ? pty_rec_by_name(rn) : NULL; return t ? pty_fld_typename(t, (int)idx) : NULL; }
static int pas_rectype_field_is_bool(const char *rn, long idx) { struct pty_fld f; return pas_rtf(rn, idx, &f) ? f.ch == 2 : 0; }
static int pas_rectype_field_is_na(const char *rn, long idx) { struct pty_fld f; return pas_rtf(rn, idx, &f) ? f.na != 0 : 0; }
static long long pas_rectype_field_na_lo(const char *rn, long idx) { struct pty_fld f; return pas_rtf(rn, idx, &f) ? f.nalo : 0; }
static long long pas_rectype_field_ca_lo(const char *rn, long idx) { struct pty_fld f; return pas_rtf(rn, idx, &f) ? f.calo : 0; }
static long long pas_rectype_field_ca_hi(const char *rn, long idx) { struct pty_fld f; return pas_rtf(rn, idx, &f) ? f.cahi : -1; }
static const char *pas_rectype_field_ptrto_by_index(const char *rn, long idx) {
    struct pty_fld f;
    if (!pas_rtf(rn, idx, &f)) return NULL;
    return f.ptr && f.ft ? pty_name_of(f.ft->base ? f.ft->base : f.ft) : NULL;
}
static int pas_rectype_field_size_align(const char *rn, int idx, long long *sz, long long *al) {
    const pty_t *t = rn && idx >= 0 ? pty_rec_by_name(rn) : NULL;
    return t ? pty_rec_fld_sa(t, idx, sz, al) : 0;
}
static int pas_rectype_layout(const char *rn, long long *size_out, long long *align_out) {
    long long ls = 0, la = 0;
    const pty_t *t = rn ? pty_rec_by_name(rn) : NULL;
    int r = t ? pty_rec_layout(t, &ls, &la) : 0;
    if (size_out) *size_out = ls;
    if (align_out) *align_out = la;
    return r;
}
int pas_rectype_total_size(const char *rn, long long *out) { return pas_rectype_layout(rn, out, NULL); }
int pas_rectype_field_offset(const char *rn, int idx, long long *out) {
    const pty_t *t = rn && idx >= 0 ? pty_rec_by_name(rn) : NULL;
    if (!t || idx >= pty_rec_nf(t)) return 0;
    long long off = 0;
    for (int j = 0; j <= idx; j++) { long long sz, al; if (!pty_rec_fld_sa(t, j, &sz, &al)) return 0; if (al > 1) off = ((off + al - 1) / al) * al; if (j == idx) { *out = off; return 1; } off += sz; }
    return 0;
}
const char *pas_rectype_field_enum_by_index(const char *rn, long idx) { struct pty_fld f; if (!pas_rtf(rn, idx, &f)) return NULL; return f.en && f.ft ? pty_name_of(f.ft) : NULL; }
static const char *pas_rectype_field_rectype_by_index(const char *rn, long idx) { struct pty_fld f; return pas_rtf(rn, idx, &f) ? pas_fld_rectype(&f) : NULL; }
static int pas_recvar_field_is_char(const char *vn, long idx) { struct pty_fld f; return pas_rvf(vn, idx, &f) ? f.ch == 1 : 0; }
static int pas_recvar_field_is_bool(const char *vn, long idx) { struct pty_fld f; return pas_rvf(vn, idx, &f) ? f.ch == 2 : 0; }
static int pas_recvar_field_index(const char *vn, const char *fn) {
    const pty_t *t = pas_rvt(vn);
    struct pty_fld g;
    for (int j = 0; fn && t && j < pty_rec_nf(t); j++) if (pty_rec_fld(t, j, &g) && g.name && !strcmp(g.name, fn)) return j;
    return -1;
}
int pas_rectype_slot(const char *rn, int fi) { struct pty_fld f; return pas_rtf(rn, fi, &f) ? (int)(f.ov & 0xFFFF) : fi; }
int pas_recvar_slot(const char *vn, int fi) { struct pty_fld f; return pas_rvf(vn, fi, &f) ? (int)(f.ov & 0xFFFF) : fi; }
static const char *pas_selector_rectype(tree_t *e) {
    if (!e) return NULL;
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return pas_ptrexpr_target(e->c[1]);
    return pas_elem_rectype(e);
}
const char *pas_ptrexpr_target(tree_t *e) {
    if (!e) return NULL;
    if (e->t == TT_VAR && e->v.sval) return pas_ptrvar_target(e->v.sval);
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval) { const char *at = pas_arrptr_target(e->c[0]->v.sval); if (at) return at; }
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) {
        const char *rt = pas_with_sel_rtype(e->c[0]);
        if (rt) return pas_rectype_field_ptrto_by_index(rt, e->c[1]->v.ival);
    }
    return NULL;
}
static void pas_dense_lows(const char *name, long long *rlo, long long *clo) {
    const pty_t *t = name ? pty_name_type(name) : NULL;
    *rlo = 0;
    *clo = 0;
    if (t && t->k == PTK_ARRAY && !t->dyn && !t->open && t->nix == 2 && t->ix && t->ix[0] && t->ix[1] && t->ix[0]->bounded && t->ix[1]->bounded) { *rlo = t->ix[0]->lo; *clo = t->ix[1]->lo; }
}
static int pas_dense_type(const pty_t *t, long long *rl, long long *cl, long long *nc) {
    if (!t || t->k != PTK_ARRAY || t->dyn || t->open || t->nix != 2 || !t->ix || !t->ix[0] || !t->ix[1] || !t->ix[0]->bounded || !t->ix[1]->bounded || !t->base || t->base->k == PTK_CHAR) return 0;
    *rl = t->ix[0]->lo;
    *cl = t->ix[1]->lo;
    *nc = t->ix[1]->hi - t->ix[1]->lo + 1;
    return 1;
}
static int pas_dense_src(const tree_t *b) { long long rl, cl, nc; return IS(b, TT_DEREF) && pas_dense_type(pty_of_src(b), &rl, &cl, &nc); }
static tree_t *pas_dense_flat(tree_t *r, tree_t *c, const pty_t *t) {
    long long rl, cl, nc;
    pas_dense_type(t, &rl, &cl, &nc);
    return bin(TT_ADD, bin(TT_MUL, pas_dense_sub(r, rl), ilit(nc)), pas_dense_sub(c, cl));
}
tree_t *pas_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t);
    c->v = e->v;
    if (e->t == TT_IDX && e->slen == PAS_FIELD_IDX_MARK) c->slen = e->slen;
    if ((e->t == TT_VAR || e->t == TT_QLIT) && e->v.sval) c->v.sval = ct_strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) ast_push(c, pas_tree_clone(e->c[i]));
    return c;
}
const char *pas_with_sel_rtype(tree_t *sel) { return sel ? pty_sel_rtype(sel) : NULL; }
int pas_with_field_index(const char *rtype, const char *fname) { return pas_rectype_field_index(rtype, fname); }
int pas_with_recvar_field(const char *vname, const char *fname) { return pas_recvar_field_index(vname, fname); }
static void pas_with_push(tree_t *sel) {
    if (!sel) return;
    if (g_lower_pas.sem.with_depth >= g_lower_pas.sem.with_cap) g_lower_pas.sem.with_stk = pas_pool_grow(g_lower_pas.sem.with_stk, &g_lower_pas.sem.with_cap, sizeof *g_lower_pas.sem.with_stk, 8);
    const char *rt = pas_with_sel_rtype(sel);
    g_lower_pas.sem.with_stk[g_lower_pas.sem.with_depth].sel = sel;
    g_lower_pas.sem.with_stk[g_lower_pas.sem.with_depth].rtype = rt;
    g_lower_pas.sem.with_stk[g_lower_pas.sem.with_depth].orig = sel;
    g_lower_pas.sem.with_stk[g_lower_pas.sem.with_depth].pre = NULL;
    g_lower_pas.sem.with_depth++;
}
static void pas_with_pop(void) { if (g_lower_pas.sem.with_depth > 0) g_lower_pas.sem.with_depth--; }
static tree_t *pas_with_tmp(tree_t *val, tree_t **pre) {
    char _b[24];
    snprintf(_b, sizeof _b, "__pas_wp%d", g_lower_pas.sem.wn++);
    const char *_nm = ct_strdup(_b);
    pas_scope_define(_nm);
    pas_local_add(_nm);
    pty_note_temp(_nm, val);
    if (!*pre) *pre = ast_node_new(TT_SEQ_EXPR);
    ast_push(*pre, mk_assign(leaf_s(TT_VAR, _nm), val));
    return leaf_s(TT_VAR, _nm);
}
static tree_t *pas_with_capture(tree_t *sel, tree_t **pre) {
    if (!sel) return sel;
    if (sel->t == TT_FNC && sel->n == 2 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_deref") && sel->c[1]) {
        tree_t *_in = pas_with_capture(sel->c[1], pre);
        tree_t *_t = pas_with_tmp(_in, pre);
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
void pas_with_push_once(tree_t *sel) {
    tree_t *_pre = NULL;
    tree_t *_cs = pas_with_capture(sel, &_pre);
    int _d0 = g_lower_pas.sem.with_depth;
    pas_with_push(_cs);
    if (g_lower_pas.sem.with_depth > _d0) { g_lower_pas.sem.with_stk[_d0].orig = sel; g_lower_pas.sem.with_stk[_d0].pre = _pre; }
}
tree_t *pas_with_finish(long long n, tree_t *body) {
    tree_t *q = NULL;
    int _lo = g_lower_pas.sem.with_depth - (int)n;
    if (_lo < 0) _lo = 0;
    for (int i = _lo; i < g_lower_pas.sem.with_depth; i++) if (g_lower_pas.sem.with_stk[i].pre) {
        if (!q) q = ast_node_new(TT_SEQ_EXPR);
        for (int k = 0; k < g_lower_pas.sem.with_stk[i].pre->n; k++) ast_push(q, g_lower_pas.sem.with_stk[i].pre->c[k]);
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
    if (b->t == TT_VAR && b->v.sval) { struct pty_fld f; return (fi >= 0 && pas_rvf(b->v.sval, fi, &f) && f.na) ? pas_fld_rectype(&f) : NULL; }
    { const char *bt = pas_with_sel_rtype(b); return (bt && pas_rectype_field_is_na(bt, fi)) ? pas_rectype_field_rectype_by_index(bt, fi) : NULL; }
}
int pas_rectype_is_packed(const char *rn) { const pty_t *t = pty_rec_by_name(rn); return t ? t->packed != 0 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_recvar_is_packed(const char *vn) { const pty_t *t = pas_rvt(vn); return t ? t->packed != 0 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_recvar_field_rectype(const char *vn, long idx) { struct pty_fld f; return pas_rvf(vn, idx, &f) ? pas_fld_rectype(&f) : NULL; }
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
int pas_is_aggregate_designator(tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_IDX) { const pty_t *dt = pty_of_low(e); if (dt && dt->k == PTK_ARRAY && dt->dyn) return 0; }
    if (e->t == TT_VAR && e->v.sval) { long long _ah; return !pas_array_isdyn(e->v.sval) && (pas_array_high_get(e->v.sval, &_ah) || pas_array_is_param(e->v.sval)); }
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return pas_rectype_nf(pas_ptrexpr_target(e->c[1])) > 0;
    if (e->t == TT_IDX && e->n == 2 && e->slen != PAS_FIELD_IDX_MARK && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_arrrec_find(e->c[0]->v.sval, NULL) > 0) return 1;
    if (pas_elem_rectype(e)) return 1;
    if (e->t == TT_IDX && e->n == 2 && e->slen != PAS_FIELD_IDX_MARK && e->c[0] && e->c[0]->t == TT_VAR && e->c[0]->v.sval && pas_rows_get(e->c[0]->v.sval, 0, NULL, NULL)) return 1;
    if (e->t != TT_IDX || e->n != 2 || !e->c[0] || !e->c[1] || e->c[1]->t != TT_ILIT) return 0;
    if (pas_designator_rectype(e)) return 1;
    {
        long idx = (long)e->c[1]->v.ival;
        tree_t *b = e->c[0];
        if (b->t == TT_VAR && b->v.sval) { struct pty_fld f; return pas_rvf(b->v.sval, idx, &f) ? (f.na != 0 || pas_fld_rectype(&f) != NULL) : 0; }
        { const char *bt = pas_with_sel_rtype(b); return bt ? (pas_rectype_field_is_na(bt, idx) || pas_rectype_field_rectype_by_index(bt, idx) != NULL) : 0; }
    }
}
int pas_actual_is_packed_component(tree_t *a) {
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
static void pas_vt_mark(tree_t *e, tree_t *base, const char *rt, int fi) {
    if (!e || !base || fi < 0) return;
    const char *nm = rt ? rt : pas_with_sel_rtype(base);
    const pty_vt_t *t = pas_vt_find_name(nm);
    if (!t && base->t == TT_VAR && base->v.sval) t = pas_vt_find_name(base->v.sval);
    if (t && t->tagfi >= 0 && fi == t->tagfi && base->t == TT_FNC && base->n == 2 && base->c[0] && base->c[0]->v.sval && !strcmp(base->c[0]->v.sval, "__pas_deref")) {
        struct pas_vt *m = pas_vt_new(5);
        m->node = e;
        m->tagexpr = pas_tree_clone(base->c[1]);
        return;
    }
    if (t && t->tagfi >= 0 && fi == t->tagfi && base->t == TT_VAR && base->v.sval) { struct pas_vt *m = pas_vt_new(7); m->node = e; m->name = ct_strdup(base->v.sval); return; }
    if (!t || fi >= t->fcap || !t->fhas[fi] || t->tagfi < 0) return;
    if (base->t != TT_VAR || !base->v.sval) return;
    {
        int asg = 0;
        for (int i = g_lower_pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower_pas.sem.pas_vt[i]->state == 6 && g_lower_pas.sem.pas_vt[i]->name && !strcmp(g_lower_pas.sem.pas_vt[i]->name, base->v.sval)) {
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
static void pas_vt_rekey(tree_t *from, tree_t *to) { for (int i = g_lower_pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower_pas.sem.pas_vt[i]->node == from) g_lower_pas.sem.pas_vt[i]->node = to; }
static tree_t *pas_region_slot_of(tree_t *sel) {
    if (!sel) return NULL;
    if (sel->t == TT_IDX) return sel;
    if (sel->t == TT_FNC && sel->n >= 2 && sel->c[0] && sel->c[0]->v.sval && (!strcmp(sel->c[0]->v.sval, "__pas_rdecode") || !strcmp(sel->c[0]->v.sval, "__pas_rarr")) && sel->c[1] &&
        sel->c[1]->t == TT_IDX) return sel->c[1];
    return NULL;
}
static tree_t *pas_vt_check_of(tree_t *sel) {
    if (!sel) return NULL;
    for (int i = g_lower_pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower_pas.sem.pas_vt[i]->state == 4 && g_lower_pas.sem.pas_vt[i]->node == sel) {
        tree_t *c = ast_node_new(TT_FNC);
        ast_push(c, leaf_s(TT_VAR, "__pas_vcheck"));
        ast_push(c, pas_tree_clone(g_lower_pas.sem.pas_vt[i]->tagexpr));
        ast_push(c, ilit(g_lower_pas.sem.pas_vt[i]->mask));
        return c;
    }
    if (sel->t == TT_FNC && sel->n == 2 && sel->c[0] && sel->c[0]->v.sval && !strcmp(sel->c[0]->v.sval, "__pas_deref")) {
        const char *wt = pas_ptrexpr_target(sel->c[1]);
        const pty_vt_t *vt = wt ? pas_vt_find_name(wt) : NULL;
        if (vt && vt->tagfi >= 0) { tree_t *c = ast_node_new(TT_FNC); ast_push(c, leaf_s(TT_VAR, "__pas_tagwhole")); ast_push(c, pas_tree_clone(sel->c[1])); return c; }
    }
    return NULL;
}
tree_t *pas_vt_wrap_read(tree_t *sel) {
    tree_t *c = pas_vt_check_of(sel);
    if (!c) return sel;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    ast_push(q, c);
    tree_t *_sl = pas_region_slot_of(sel);
    if (!g_lower_pas.sem.pas_seen_mode_directive && _sl && c->c[0]->v.sval && !strcmp(c->c[0]->v.sval, "__pas_vcheck")) {
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
static tree_t *pas_vt_undefine_variants(tree_t *sel, tree_t *rhs, tree_t *st) {
    if (g_lower_pas.sem.pas_seen_mode_directive || !sel || sel->t != TT_IDX || sel->n < 2 || !sel->c[0] || sel->c[0]->t != TT_VAR || !sel->c[0]->v.sval || !pas_tree_pure(rhs)) return st;
    const char *vn = sel->c[0]->v.sval, *rn = pas_with_sel_rtype(sel->c[0]);
    const pty_vt_t *t = rn ? pas_vt_find_name(rn) : NULL;
    if (!t) t = pas_vt_find_name(vn);
    if (!t || t->tagfi < 0) return st;
    tree_t *nul = ast_node_new(TT_SEQ_EXPR);
    int ns = 0;
    for (int fi = 0; fi < t->fcap; fi++) {
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
tree_t *pas_vt_strip_undef(tree_t *a) {
    if (!a || a->t != TT_SEQ_EXPR || a->n != 3 || !a->c[1] || a->c[1]->t != TT_IF || a->c[1]->n != 2 || !a->c[1]->c[1] || a->c[1]->c[1]->t != TT_FNC || a->c[1]->c[1]->n < 2 || !a->c[1]->c[1]->c[1] ||
        !a->c[1]->c[1]->c[1]->v.sval || strcmp(a->c[1]->c[1]->c[1]->v.sval, "6.5.3.3")) return a;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    ast_push(q, a->c[0]);
    ast_push(q, a->c[2]);
    return q;
}
tree_t *pas_vt_wrap_stmt(tree_t *sel, tree_t *rhs, tree_t *st) {
    for (int i = g_lower_pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower_pas.sem.pas_vt[i]->state == 7 && g_lower_pas.sem.pas_vt[i]->node == sel) {
        struct pas_vt *a = pas_vt_new(6);
        a->name = ct_strdup(g_lower_pas.sem.pas_vt[i]->name);
        st = pas_vt_undefine_variants(sel, rhs, st);
        break;
    }
    for (int i = g_lower_pas.sem.pas_nvt - 1; i >= 0; i--) if (g_lower_pas.sem.pas_vt[i]->state == 5 && g_lower_pas.sem.pas_vt[i]->node == sel && rhs) {
        tree_t *c = ast_node_new(TT_FNC);
        ast_push(c, leaf_s(TT_VAR, "__pas_tagset_chk"));
        ast_push(c, pas_tree_clone(g_lower_pas.sem.pas_vt[i]->tagexpr));
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
            return pas_field_region(base, _brt, _nfi, e);
        }
    }
    return bin(TT_FIELD, base, leaf_s(TT_VAR, fld));
}
static long long pas_rectype_field_ov(const char *rn, int fi) { struct pty_fld f; return pas_rtf(rn, fi, &f) ? f.ov : 0; }
static long long pas_recvar_field_ov(const char *vn, int fi) { struct pty_fld f; return pas_rvf(vn, fi, &f) ? f.ov : 0; }
static long long pas_recvar_field_na_lo(const char *vn, int fi) { struct pty_fld f; return pas_rvf(vn, fi, &f) ? f.nalo : 0; }
tree_t *pas_field_region(tree_t *base, const char *rt, int fi, tree_t *e) {
    const char *rt2 = rt ? rt : pas_with_sel_rtype(base);
    long long ov = 0, lo = 0;
    if (rt2) ov = pas_rectype_field_ov(rt2, fi);
    else if (base && base->t == TT_VAR && base->v.sval) ov = pas_recvar_field_ov(base->v.sval, fi);
    if (!(((ov >> 48) & 0xFFFF) != 0 && pas_rk_get(ov, 0) != 0)) return e;
    if (pas_rk_get(ov, 0) == 7) lo = rt2 ? (pas_rectype_field_is_ca(rt2, fi) ? pas_rectype_field_ca_lo(rt2, fi) : pas_rectype_field_na_lo(rt2, fi)) :
        pas_recvar_field_na_lo(base && base->t == TT_VAR ? base->v.sval : NULL, fi);
    return pas_region_wrap(e, ov, lo);
}
const char *pas_store_tn(tree_t *sel) {
    if (!sel) return NULL;
    if (sel->t == TT_VAR && sel->v.sval) return pas_scalarvartype_get(sel->v.sval);
    if (sel->t == TT_FIELD && sel->n == 2 && sel->c[1] && sel->c[1]->t == TT_VAR && sel->c[1]->v.sval) {
        const char *fn = sel->c[1]->v.sval;
        const pty_t *r = sel->c[0] ? pty_of_src(sel->c[0]) : NULL;
        if (r && r->k == PTK_POINTER) r = pty_pointee(r);
        for (int i = 0; r && r->k == PTK_RECORD && i < pty_rec_nf(r); i++) { struct pty_fld o; if (pty_rec_fld(r, i, &o) && o.name && !strcasecmp(o.name, fn)) return o.ft ? pty_name_of(o.ft) : NULL; }
        return NULL;
    }
    if (sel->t == TT_IDX && sel->n == 2 && sel->c[0] && sel->c[1]) {
        tree_t *b = sel->c[0];
        struct pty_fld o;
        if (sel->c[1]->t == TT_ILIT && b->t == TT_IDX && b->n == 2 && b->c[0] && b->c[0]->t == TT_VAR && b->c[0]->v.sval) {
            const pty_t *_at = pty_name_type(b->c[0]->v.sval);
            if (_at && _at->k == PTK_ARRAY && _at->base && _at->base->k == PTK_RECORD && pty_rec_fld(_at->base, (int) sel->c[1]->v.ival, &o)) return o.ft ? pty_name_of(o.ft) : NULL;
        }
        if (sel->c[1]->t == TT_ILIT && b->t == TT_FNC && b->n == 2 && b->c[0] && b->c[0]->v.sval && !strcmp(b->c[0]->v.sval, "__pas_deref") && b->c[1] && b->c[1]->t == TT_VAR && b->c[1]->v.sval) {
            const pty_t *_pt = pty_name_type(b->c[1]->v.sval);
            if (_pt && _pt->k == PTK_POINTER && _pt->base && _pt->base->k == PTK_RECORD && pty_rec_fld(_pt->base, (int) sel->c[1]->v.ival, &o)) return o.ft ? pty_name_of(o.ft) : NULL;
        }
        if (sel->c[1]->t == TT_ILIT) {
            const char *rn = b->t == TT_VAR && b->v.sval ? NULL : pas_designator_rectype(b);
            if (b->t == TT_VAR && b->v.sval) {
                const pty_t *rt = pas_rvt(b->v.sval);
                if (rt && pty_rec_fld(rt, (int) sel->c[1]->v.ival, &o)) return o.ft ? pty_name_of(o.ft) : NULL;
            } else if (rn && pas_rtf(rn, (long) sel->c[1]->v.ival, &o)) return o.ft ? pty_name_of(o.ft) : NULL;
        }
        if (b->t == TT_VAR && b->v.sval) return pas_scalarvartype_get(b->v.sval);
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pas_value_formal_file_check(PNodeList *ids, const char *t) {
    if (!t || !(!strcmp(t, "text") || pas_is_tfiletype(t) || pas_rectype_has_file(t))) return;
    for (int i = 0; ids && i < ids->count; i++) if (ids->items[i] && ids->items[i]->v.sval) {
        fprintf(stderr,
            "pascal: ISO 7185 6.6.3.2 violation: the value formal-parameter '%s' has the type %s, which is or contains a file-type, so no actual-parameter is assignment-compatible with it\n",
            ids->items[i]->v.sval, t);
        g_lower_pas.sem.pas_iso_errors++;
    }
}
static tree_t *pas_typer_field(tree_t *base, const char *fname) {
    int fi = pty_field_slot(base, fname);
    if (fi < 0) return NULL;
    tree_t *e = ast_node_new(TT_IDX);
    ast_push(e, base);
    ast_push(e, ilit(fi));
    return e;
}
pval E_selector_raw(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_FIELD) && n->n == 2) { tree_t *_sh = pas_strhlp_prop(n); if (_sh) return pas_strhlp_eval(_sh); }
    if (IS(n, TT_IDX) && n->n >= 3 && !(n->n == 3 && IS(n->c[0], TT_VAR) && pas_array_ncols(tok_s(n->c[0])) > 0) && !(n->n == 3 && pas_dense_src(n->c[0]))) {
        tree_t *inner = ast_node_new(TT_IDX);
        ast_push(inner, n->c[0]);
        ast_push(inner, n->c[1]);
        tree_t *outer = ast_node_new(TT_IDX);
        ast_push(outer, inner);
        for (int i = 2; i < n->n; i++) ast_push(outer, n->c[i]);
        return E_selector(outer);
    }
    if (IS(n, TT_IDX)) {
        v[1] = E_selector(n->c[0]);
        v[3] = E_expression_list(n, 1, n->n);
        if (v[3].list && v[3].list->count == 1 && v[1].node && v[1].node->t == TT_FNC && v[1].node->n == 2 && v[1].node->c[0] && v[1].node->c[0]->v.sval &&
            !strcmp(v[1].node->c[0]->v.sval, "__pas_deref") && pas_ptr_typed(v[1].node->c[1])) {
            long long _lo, _hi;
            if (pty_array_bounds(pty_pointee(pty_of_low(v[1].node->c[1])), &_lo, &_hi)) {
                tree_t *_dx = ast_node_new(TT_FNC);
                ast_push(_dx, leaf_s(TT_VAR, "__pas_pdx"));
                ast_push(_dx, v[1].node->c[1]);
                ast_push(_dx, v[3].list->items[0]);
                ast_push(_dx, ilit(_lo));
                ast_push(_dx, ilit(_hi));
                pty_note_ptr_to(_dx, n);
                out.node = mk_deref(_dx);
                return out;
            }
        }
        if (v[3].list && v[3].list->count == 1 && pas_ptr_typed(v[1].node)) { out.node = mk_deref(mk_fnc2("__pas_ptr_inc", v[1].node, v[3].list->items[0])); return out; }
        if (v[1].node &&
            ((v[1].node->t == TT_VAR && v[1].node->v.sval && (pas_var_string_kind(v[1].node->v.sval) || pas_sconst_get(v[1].node->v.sval))) || (v[1].node->t == TT_QLIT && v[1].node->v.sval)) &&
            !(g_lower_pas.sem.pas_zerobased_strings & 1) && v[3].list && v[3].list->count == 1 && v[3].list->items[0] && v[3].list->items[0]->t == TT_ILIT && v[3].list->items[0]->v.ival == 0) {
            out.node = mk_chrlit(mk_fnc1("length", pas_tree_clone(v[1].node)));
            return out;
        }
        {
            tree_t *_rg =
                (v[1].node && v[1].node->t == TT_FNC && v[1].node->n == 6 && v[1].node->c[0] && v[1].node->c[0]->v.sval && !strcmp(v[1].node->c[0]->v.sval, "__pas_rarr") && v[3].list &&
                v[3].list->count == 1) ? pas_region_elem(v[1].node, v[3].list->items[0]) : NULL;
            tree_t *e = NULL;
            if (v[3].list && v[3].list->count == 2 && v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) {
                long long _nc = pas_array_ncols(v[1].node->v.sval);
                if (_nc > 0) {
                    long long _rl, _cl;
                    pas_dense_lows(v[1].node->v.sval, &_rl, &_cl);
                    tree_t *flat = bin(TT_ADD, bin(TT_MUL, pas_dense_sub(v[3].list->items[0], _rl), ilit(_nc)), pas_dense_sub(v[3].list->items[1], _cl));
                    e = ast_node_new(TT_IDX);
                    ast_push(e, v[1].node);
                    ast_push(e, flat);
                    pas_flat_mark_add(e);
                }
            }
            if (!e && v[3].list && v[3].list->count == 2 && v[1].node && pas_dense_src(n->c[0])) {
                e = ast_node_new(TT_IDX);
                ast_push(e, v[1].node);
                ast_push(e, pas_dense_flat(v[3].list->items[0], v[3].list->items[1], pty_of_src(n->c[0])));
                pas_flat_mark_add(e);
            }
            if (!e && v[3].list && v[3].list->count == 1 && v[1].node && v[1].node->t == TT_IDX && v[1].node->n == 2 && IS(n->c[0], TT_IDX) && n->c[0]->n == 2 && pas_dense_src(n->c[0]->c[0]) &&
                pas_is_deref_call(v[1].node->c[0]) && !pas_flat_marked(v[1].node)) {
                e = ast_node_new(TT_IDX);
                ast_push(e, v[1].node->c[0]);
                ast_push(e, pas_dense_flat(v[1].node->c[1], v[3].list->items[0], pty_of_src(n->c[0]->c[0])));
                pas_flat_mark_add(e);
            }
            if (!e && v[3].list && v[3].list->count == 1 && v[1].node && v[1].node->t == TT_IDX && v[1].node->n == 2 && v[1].node->c[0] && v[1].node->c[0]->t == TT_VAR && v[1].node->c[0]->v.sval &&
                !pas_is_nafield(v[1].node) && !pas_flat_marked(v[1].node)) {
                long long _nc2 = pas_array_ncols(v[1].node->c[0]->v.sval);
                if (_nc2 > 0) {
                    long long _rl2, _cl2;
                    pas_dense_lows(v[1].node->c[0]->v.sval, &_rl2, &_cl2);
                    tree_t *flat2 = bin(TT_ADD, bin(TT_MUL, pas_dense_sub(v[1].node->c[1], _rl2), ilit(_nc2)), pas_dense_sub(v[3].list->items[0], _cl2));
                    e = ast_node_new(TT_IDX);
                    ast_push(e, v[1].node->c[0]);
                    ast_push(e, flat2);
                    pas_flat_mark_add(e);
                }
            }
            if (!e) {
                e = ast_node_new(TT_IDX);
                ast_push(e, v[1].node);
                if (v[3].list) for (int i = 0; i < v[3].list->count; i++)
                    ast_push(e,
                    (v[3].list->count == 1 && (g_lower_pas.sem.pas_zerobased_strings & 1) && v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && pas_var_string_kind(v[1].node->v.sval) == 1) ?
                    bin(TT_ADD, v[3].list->items[i], ilit(1)) : v[3].list->items[i]);
            }
            if (e && v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) {
                const char *_et = pas_enumarr_of(v[1].node);
                if (_et) { int _ei = pas_enumnames_idx(_et); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); }
            }
            out.node = _rg ? _rg : e;
        }
        return out;
    }
    if (IS(n, TT_FIELD)) {
        if (IS(n->c[0], TT_VAR) && n->c[0]->v.sval && IS(n->c[1], TT_VAR) && n->c[1]->v.sval && pas_enumtype_high(n->c[0]->v.sval) >= 0) {
            long long _ev;
            if (pas_const_get(n->c[1]->v.sval, &_ev)) { out.node = ilit(_ev); return out; }
        }
        v[1] = E_selector(n->c[0]);
        v[3].str = tok_s(n->c[1]);
        {
            int _fi = -1;
            const char *_rt = IS(n->c[0], TT_DEREF) && pas_ptrcast_target(n->c[0]->c[0]) ? pas_ptrcast_target(n->c[0]->c[0]) : pas_selector_rectype(v[1].node);
            if (_rt) _fi = pas_rectype_field_index(_rt, v[3].str);
            else if (v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) _fi = pas_recvar_field_index(v[1].node->v.sval, v[3].str);
            if (_fi < 0 && v[1].node && v[1].node->t == TT_IDX && v[1].node->n == 2 && v[1].node->c[0] && v[1].node->c[0]->t == TT_VAR) {
                const char *_arn = NULL;
                int _anf = pas_arrrec_find(v[1].node->c[0]->v.sval, &_arn);
                if (_anf > 0) {
                    int _afi = _arn ? pas_rectype_field_index(_arn, v[3].str) : -1;
                    if (_afi < 0) { _afi = pas_arrrec_field_index(v[1].node->c[0]->v.sval, v[3].str); }
                    if (_afi >= 0) {
                        out.node = pas_arrrec_flatten(v[1].node, _afi);
                        const char *_fe = pas_arrrec_field_enum(v[1].node->c[0]->v.sval, _afi);
                        if (!_fe && _arn) _fe = pas_rectype_field_enum_by_index(_arn, _afi);
                        if (_fe && out.node) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) out.node->v.ival = (long long)(_ei + 1); }
                        out.node = pas_arrrec_region(out.node, v[1].node->c[0]->v.sval, _afi);
                    } else {
                        out.node = bin(TT_FIELD, v[1].node, leaf_s(TT_VAR, v[3].str));
                    }
                } else {
                    out.node = pas_typer_field(v[1].node, v[3].str);
                    if (!out.node) out.node = pas_nested_field_resolve(v[1].node, v[3].str);
                }
            } else if (_fi >= 0) {
                tree_t *e = ast_node_new(TT_IDX);
                ast_push(e, v[1].node);
                ast_push(e, ilit(_rt ? pas_rectype_slot(_rt, _fi) : (v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval) ? pas_recvar_slot(v[1].node->v.sval, _fi) : _fi));
                if (_rt) { const char *_fe = pas_rectype_field_enum_by_index(_rt, _fi); if (_fe) { int _ei = pas_enumnames_idx(_fe); if (_ei >= 0) e->v.ival = (long long)(_ei + 1); } }
                { const char *_mrt = _rt ? _rt : pas_with_sel_rtype(v[1].node); }
                pas_vt_mark(e, v[1].node, _rt, _fi);
                { tree_t *_w = pas_field_region(v[1].node, _rt, _fi, e); if (_w != e) pas_vt_rekey(e, _w); out.node = _w; }
            } else {
                out.node = pas_typer_field(v[1].node, v[3].str);
                if (!out.node) out.node = pas_nested_field_resolve(v[1].node, v[3].str);
            }
        }
        return out;
    }
    if (IS(n, TT_DEREF) && IS(n->c[0], TT_FNC) && n->c[0]->n == 2 && IS(n->c[0]->c[0], TT_VAR) && n->c[0]->c[0]->v.sval && !strcmp(pas_alias_resolve(n->c[0]->c[0]->v.sval), "pointer") &&
        !pas_is_func("pointer") && IS(n->c[0]->c[1], TT_VAR) && n->c[0]->c[1]->v.sval && pas_array_isdyn(n->c[0]->c[1]->v.sval)) {
        tree_t *ix = ast_node_new(TT_IDX);
        ast_push(ix, n->c[0]->c[1]);
        ast_push(ix, ilit(0));
        return E_selector(ix);
    }
    if (IS(n, TT_DEREF)) {
        v[1] = (IS(n->c[0], TT_VAR) || IS(n->c[0], TT_IDX) || IS(n->c[0], TT_FIELD) || IS(n->c[0], TT_DEREF) || IS(n->c[0], TT_FNC)) ? E_selector(n->c[0]) : E_expression(n->c[0]);
        {
            out.node = pas_is_tfile_node(v[1].node) ? mk_fnc1("__pas_fbuf_get", v[1].node) :
                ((v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && pas_is_filevar(v[1].node->v.sval) && !pas_is_stdstream(v[1].node->v.sval)) ? mk_fnc1("__pas_tbuf_get", v[1].node) :
                ((v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && !strcmp(v[1].node->v.sval, "input") && !pas_ptrvar_target(v[1].node->v.sval)) ? mk_fnc0("__pas_getbufch") :
                ((v[1].node && v[1].node->t == TT_VAR && v[1].node->v.sval && !strcmp(v[1].node->v.sval, "output") && pas_is_stdstream("output") && !pas_ptrvar_target("output")) ?
                leaf_s(TT_VAR, "__pas_outbuf") : mk_deref(v[1].node))));
        }
        return out;
    }
    if (IS(n, TT_VAR)) { v[1].str = pas_alias_resolve(tok_s(n)); { out.node = mk_ident(v[1].str); } return out; }
    if (IS(n, TT_FNC)) return E_call_with_args(n);
    elab_bad("selector", n);
    return out;
}
