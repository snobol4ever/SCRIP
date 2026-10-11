#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "ct_arena.h"
#include "ast.h"
#include "lower_pascal_class.h"
#define PC_IS(x, k) ((x) && (x)->t == (k))
typedef struct pc_class pc_class;
typedef struct { const char *name; const char *kind; int isfunc; tree_t *params; tree_t *rtype; int virt, over, abst, has_impl, used; pc_class *owner; } pc_meth;
typedef struct { const char *name; tree_t *type; tree_t *ip; const char *rd; const char *wr; int dflt; } pc_prop;
typedef struct { const char *name; tree_t *type; } pc_field;
struct pc_class { const char *name; const char *raw; const char *pname; pc_class *parent; int id; pc_field *f; int nf, cf; pc_meth *m; int nm, cm; pc_prop *p; int np, cp; };
typedef struct { const char *name; int kind; tree_t *type; pc_class *cls; int depth; } pc_sym;
typedef struct { const char *from; const char *to; } pc_ren;
typedef struct { pc_class *cls; const char *tmp; tree_t *rtype; } pc_with;
typedef struct { pc_class *c; pc_meth *m; } pc_made;
typedef struct {
    pc_class **cls;
    int ncls, ccls;
    pc_sym *sym;
    int nsym, csym, depth;
    pc_class *cur;
    pc_meth *curm;
    const char *cur_impl;
    int mdepth;
    pc_ren *ren;
    int nren, cren;
    pc_with *with;
    int nwith, cwith;
    tree_t *vars;
    int ntmp;
    pc_made *made;
    int nmade, cmade;
    int exc;
    const char *xv;
    pc_class *root;
} pc_ctx;
typedef struct { tree_t *t; pc_class *cls; int ref; } pc_ty;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_leaf(tree_e k, const char *s) { tree_t *e = ast_node_new(k); e->v.sval = (char *)s; return e; }
static tree_t *pc_var(const char *s) { return pc_leaf(TT_VAR, s); }
static tree_t *pc_int(long long v) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = v; return e; }
static tree_t *pc_n1(tree_e k, tree_t *a) { tree_t *e = ast_node_new(k); if (a) ast_push(e, a); return e; }
static tree_t *pc_n2(tree_e k, tree_t *a, tree_t *b) { tree_t *e = pc_n1(k, a); if (b) ast_push(e, b); return e; }
static tree_t *pc_fld(tree_t *base, const char *f) { return pc_n2(TT_FIELD, base, pc_var(f)); }
static tree_t *pc_ofld(tree_t *obj, const char *f) { return pc_fld(pc_n1(TT_DEREF, obj), f); }
static tree_t *pc_asg(tree_t *l, tree_t *r) { return pc_n2(TT_ASSIGN, l, r); }
static tree_t *pc_seq(void) { return ast_node_new(TT_SEQ_EXPR); }
static tree_t *pc_if(tree_t *c, tree_t *t, tree_t *e) { tree_t *n = pc_n2(TT_IF, c, t); if (e) ast_push(n, e); return n; }
static tree_t *pc_fnc(const char *fn) { return pc_n1(TT_FNC, pc_var(fn)); }
static const char *pc_sv(const tree_t *t) { return t ? t->v.sval : NULL; }
static int pc_eq(const char *a, const char *b) { return a && b && !strcasecmp(a, b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_clone(const tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t);
    c->v = e->v;
    c->line = e->line;
    c->slen = e->slen;
    for (int i = 0; i < e->n; i++) ast_push(c, pc_clone(e->c[i]));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pc_name(const char *a, const char *b, const char *c) {
    size_t n = strlen(a) + (b ? strlen(b) : 0) + (c ? strlen(c) : 0) + 1;
    char *s = ct_alloc(n);
    snprintf(s, n, "%s%s%s", a, b ? b : "", c ? c : "");
    for (char *p = s; *p; p++) if (*p == '.') *p = '_';
    return s;
}
static const char *pc_impl_name(const pc_class *c, const char *m) { return pc_name(pc_name("__pc_", c->name, "__"), m, NULL); }
static const char *pc_tmp(pc_ctx *x, const char *tag) { int k = ++x->ntmp; char b[snprintf(NULL, 0, "%d", k) + 1]; snprintf(b, sizeof b, "%d", k); return pc_name("__pc_", tag, b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pc_sym_add(pc_ctx *x, const char *name, int kind, tree_t *type, pc_class *cls) {
    if (!name) return;
    if (x->nsym >= x->csym) { x->csym = x->csym ? x->csym * 2 : 64; x->sym = ct_grow(x->sym, (size_t)x->csym * sizeof *x->sym); }
    pc_sym *s = &x->sym[x->nsym++];
    s->name = name;
    s->kind = kind;
    s->type = type;
    s->cls = cls;
    s->depth = x->depth;
}
static pc_sym *pc_sym_find(pc_ctx *x, const char *name) { for (int i = x->nsym - 1; name && i >= 0; i--) if (pc_eq(x->sym[i].name, name)) return &x->sym[i]; return NULL; }
static pc_class *pc_class_named(pc_ctx *x, const char *n) { for (int i = 0; n && i < x->ncls; i++) if (pc_eq(x->cls[i]->name, n)) return x->cls[i]; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pc_class *pc_class_new(pc_ctx *x, const char *name, const char *raw) {
    pc_class *c = ct_zalloc(1, sizeof *c);
    c->name = name;
    c->raw = raw;
    if (x->ncls >= x->ccls) { x->ccls = x->ccls ? x->ccls * 2 : 16; x->cls = ct_grow(x->cls, (size_t)x->ccls * sizeof *x->cls); }
    x->cls[x->ncls++] = c;
    c->id = x->ncls;
    return c;
}
static pc_meth *pc_meth_add(pc_class *c, const char *name, const char *kind, int isfunc, tree_t *params, tree_t *rtype) {
    if (c->nm >= c->cm) { c->cm = c->cm ? c->cm * 2 : 8; c->m = ct_grow(c->m, (size_t)c->cm * sizeof *c->m); }
    pc_meth *m = &c->m[c->nm++];
    memset(m, 0, sizeof *m);
    m->name = name;
    m->kind = kind;
    m->isfunc = isfunc;
    m->params = params;
    m->rtype = rtype;
    m->owner = c;
    return m;
}
static void pc_field_add(pc_class *c, const char *name, tree_t *type) {
    if (c->nf >= c->cf) { c->cf = c->cf ? c->cf * 2 : 8; c->f = ct_grow(c->f, (size_t)c->cf * sizeof *c->f); }
    c->f[c->nf].name = name;
    c->f[c->nf++].type = type;
}
static pc_prop *pc_prop_add(pc_class *c, const char *name, tree_t *type) {
    if (c->np >= c->cp) { c->cp = c->cp ? c->cp * 2 : 8; c->p = ct_grow(c->p, (size_t)c->cp * sizeof *c->p); }
    pc_prop *p = &c->p[c->np++];
    memset(p, 0, sizeof *p);
    p->name = name;
    p->type = type;
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pc_meth *pc_find_meth(pc_class *c, const char *n) { for (; c; c = c->parent) for (int i = 0; i < c->nm; i++) if (pc_eq(c->m[i].name, n)) return &c->m[i]; return NULL; }
static pc_field *pc_find_field(pc_class *c, const char *n) { for (; c; c = c->parent) for (int i = 0; i < c->nf; i++) if (pc_eq(c->f[i].name, n)) return &c->f[i]; return NULL; }
static pc_prop *pc_find_prop(pc_class *c, const char *n) { for (; c; c = c->parent) for (int i = 0; i < c->np; i++) if (pc_eq(c->p[i].name, n)) return &c->p[i]; return NULL; }
static pc_prop *pc_find_default(pc_class *c) { for (; c; c = c->parent) for (int i = 0; i < c->np; i++) if (c->p[i].dflt) return &c->p[i]; return NULL; }
static int pc_is_member(pc_class *c, const char *n) { return pc_find_meth(c, n) || pc_find_field(c, n) || pc_find_prop(c, n); }
static int pc_descends(const pc_class *d, const pc_class *a) { for (; d; d = d->parent) if (d == a) return 1; return 0; }
static pc_meth *pc_meth_root(pc_meth *m) {
    if (!m || !(m->virt || m->over)) return NULL;
    pc_meth *r = m;
    for (pc_class *c = m->owner->parent; c; c = c->parent) for (int i = 0; i < c->nm; i++) if (pc_eq(c->m[i].name, m->name) && (c->m[i].virt || c->m[i].over)) r = &c->m[i];
    return r;
}
static pc_meth *pc_impl_for(pc_class *d, pc_meth *root) {
    for (pc_class *c = d; c; c = c->parent) {
        for (int i = 0; i < c->nm; i++) if (pc_eq(c->m[i].name, root->name) && (c->m[i].over || &c->m[i] == root) && c->m[i].has_impl) return &c->m[i];
        if (c == root->owner) break;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pc_word_in(const char *w, const char *const *list) { for (int i = 0; list[i]; i++) if (pc_eq(w, list[i])) return 1; return 0; }
static void pc_class_members(pc_class *c, tree_t *members) {
    static const char *const vis[] = { "private", "public", "protected", "published", "strict", "automated", 0 };
    pc_meth *lm = NULL;
    pc_prop *lp = NULL;
    for (int i = 0; members && i < members->n; i++) {
        tree_t *mb = members->c[i];
        if (PC_IS(mb, TT_KEYWORD)) {
            const char *w = pc_sv(mb);
            if (pc_word_in(w, vis)) continue;
            if (lm && (pc_eq(w, "virtual") || pc_eq(w, "dynamic"))) lm->virt = 1;
            else if (lm && pc_eq(w, "override")) lm->over = 1;
            else if (lm && pc_eq(w, "abstract")) { lm->abst = 1; if (!lm->over) lm->virt = 1; } else if (lp && pc_eq(w, "default")) lp->dflt = 1;
            continue;
        }
        if (PC_IS(mb, TT_DECL) && pc_eq(pc_sv(mb), "field") && mb->n >= 2) { for (int k = 0; k < mb->c[0]->n; k++) pc_field_add(c, pc_sv(mb->c[0]->c[k]), mb->c[1]); lm = NULL; lp = NULL; continue; }
        if ((PC_IS(mb, TT_PROCEDURE) || PC_IS(mb, TT_FUNCTION)) && mb->n >= 1) {
            tree_t *ps = NULL, *rt = NULL;
            for (int k = 1; k < mb->n; k++) { if (PC_IS(mb->c[k], TT_PARAMS)) ps = mb->c[k]; else if (PC_IS(mb->c[k], TT_VAR)) rt = mb->c[k]; }
            lm = pc_meth_add(c, pc_sv(mb->c[0]), pc_sv(mb), PC_IS(mb, TT_FUNCTION), ps, rt);
            lp = NULL;
            continue;
        }
        if (PC_IS(mb, TT_DECL) && pc_eq(pc_sv(mb), "property") && mb->n >= 2) {
            lp = pc_prop_add(c, pc_sv(mb->c[0]), mb->c[1]);
            lm = NULL;
            for (int k = 2; k < mb->n; k++) {
                tree_t *s = mb->c[k];
                if (PC_IS(s, TT_PARAMS)) { lp->ip = s; continue; }
                for (int j = 0; PC_IS(s, TT_VLIST) && j + 1 < s->n; j += 2) {
                    if (pc_eq(pc_sv(s->c[j]), "read")) lp->rd = pc_sv(s->c[j + 1]);
                    else if (pc_eq(pc_sv(s->c[j]), "write")) lp->wr = pc_sv(s->c[j + 1]);
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pc_collect(pc_ctx *x, tree_t *t) {
    if (!t) return;
    if (PC_IS(t, TT_DECL) && pc_eq(pc_sv(t), "type") && t->n >= 2 && PC_IS(t->c[1], TT_CLASS_DECL) && !pc_eq(pc_sv(t->c[1]), "forward")) {
        tree_t *cd = t->c[1];
        pc_class *c = pc_class_new(x, pc_sv(t->c[0]), (cd->n > 0 && pc_sv(cd->c[0])) ? pc_sv(cd->c[0]) : pc_sv(t->c[0]));
        for (int i = 1; i < cd->n; i++) { if (PC_IS(cd->c[i], TT_ARGS) && cd->c[i]->n > 0) c->pname = pc_sv(cd->c[i]->c[0]); else if (PC_IS(cd->c[i], TT_VLIST)) pc_class_members(c, cd->c[i]); }
        return;
    }
    for (int i = 0; i < t->n; i++) pc_collect(x, t->c[i]);
}
static void pc_scan_impls(pc_ctx *x, tree_t *t) {
    if (!t) return;
    if ((PC_IS(t, TT_PROCEDURE) || PC_IS(t, TT_FUNCTION)) && t->n > 0 && PC_IS(t->c[0], TT_FIELD) && t->c[0]->n == 2) {
        pc_class *c = pc_class_named(x, pc_sv(t->c[0]->c[0]));
        for (int i = 0; c && i < c->nm; i++) if (pc_eq(c->m[i].name, pc_sv(t->c[0]->c[1]))) c->m[i].has_impl = 1;
    }
    for (int i = 0; i < t->n; i++) pc_scan_impls(x, t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pc_root_class(pc_ctx *x) {
    pc_class *r = pc_class_new(x, "tobject", "TObject");
    x->root = r;
    pc_meth_add(r, "create", "constructor", 0, NULL, NULL);
    pc_meth *d = pc_meth_add(r, "destroy", "destructor", 0, NULL, NULL);
    d->virt = 1;
    d->has_impl = 1;
    pc_meth_add(r, "free", NULL, 0, NULL, NULL)->has_impl = 1;
    pc_meth_add(r, "classname", NULL, 1, NULL, pc_var("string"))->has_impl = 1;
    pc_meth *ts = pc_meth_add(r, "tostring", NULL, 1, NULL, pc_var("string"));
    ts->virt = 1;
    ts->has_impl = 1;
}
static void pc_link(pc_ctx *x) {
    for (int i = 0; i < x->ncls; i++) {
        pc_class *c = x->cls[i];
        if (c == x->root) continue;
        c->parent = c->pname ? pc_class_named(x, c->pname) : NULL;
        if (!c->parent || pc_descends(c->parent, c)) c->parent = x->root;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_rec_type(pc_ctx *x, pc_class *c) {
    tree_t *fl = ast_node_new(TT_FIELDS);
    ast_push(fl, pc_n2(TT_DECL, pc_n1(TT_VLIST, pc_var("__cls")), pc_var("integer")));
    fl->c[0]->v.sval = "field";
    pc_class *chain[x->ncls + 1];
    int n = 0;
    for (pc_class *a = c; a && n <= x->ncls; a = a->parent) chain[n++] = a;
    for (int k = n - 1; k >= 0; k--) for (int i = 0; i < chain[k]->nf; i++) {
        tree_t *d = pc_n2(TT_DECL, pc_n1(TT_VLIST, pc_var(chain[k]->f[i].name)), pc_clone(chain[k]->f[i].type));
        d->v.sval = "field";
        ast_push(fl, d);
    }
    return pc_n1(TT_RECORD, fl);
}
static tree_t *pc_tdecl(const char *name, tree_t *type) { tree_t *d = pc_n2(TT_DECL, pc_var(name), type); d->v.sval = "type"; return d; }
static tree_t *pc_vdecl(const char *name, tree_t *type) { tree_t *d = pc_n2(TT_DECL, pc_n1(TT_VLIST, pc_var(name)), type); d->v.sval = "var"; return d; }
static void pc_type_part(pc_ctx *x, tree_t *part, int with_root) {
    tree_t *out = ast_node_new(TT_PART);
    out->v.sval = part->v.sval;
    out->line = part->line;
    if (with_root) { ast_push(out, pc_tdecl("tobject", pc_n1(TT_PTR_TYPE, pc_var("tobject__rec")))); ast_push(out, pc_tdecl("tobject__rec", pc_rec_type(x, x->root))); }
    for (int i = 0; i < part->n; i++) {
        tree_t *d = part->c[i];
        if (PC_IS(d, TT_DECL) && d->n >= 2 && PC_IS(d->c[1], TT_CLASS_DECL)) {
            pc_class *c = pc_class_named(x, pc_sv(d->c[0]));
            if (c) ast_push(out, pc_tdecl(c->name, pc_n1(TT_PTR_TYPE, pc_var(pc_name(c->name, "__rec", NULL)))));
        }
    }
    for (int i = 0; i < part->n; i++) {
        tree_t *d = part->c[i];
        if (!(PC_IS(d, TT_DECL) && d->n >= 2 && PC_IS(d->c[1], TT_CLASS_DECL))) { ast_push(out, d); continue; }
        if (pc_eq(pc_sv(d->c[1]), "forward")) continue;
        pc_class *c = pc_class_named(x, pc_sv(d->c[0]));
        if (c) ast_push(out, pc_tdecl(pc_name(c->name, "__rec", NULL), pc_rec_type(x, c)));
    }
    part->n = 0;
    for (int i = 0; i < out->n; i++) ast_push(part, out->c[i]);
}
static int pc_part_has_class(const tree_t *p) {
    for (int i = 0; PC_IS(p, TT_PART) && pc_eq(pc_sv(p), "type") && i < p->n; i++) if (PC_IS(p->c[i], TT_DECL) && p->c[i]->n >= 2 && PC_IS(p->c[i]->c[1], TT_CLASS_DECL)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pc_ty pc_resolve(pc_ctx *x, tree_t *t) {
    pc_ty r = { NULL, NULL, 0 };
    for (int guard = 0; t && guard < 16; guard++) {
        if (!PC_IS(t, TT_VAR)) { r.t = t; return r; }
        pc_sym *s = pc_sym_find(x, pc_sv(t));
        if (s && s->kind == 'k') { r.cls = s->cls; return r; }
        if (!s || s->kind != 't' || !s->type) { r.t = t; return r; }
        t = s->type;
    }
    r.t = t;
    return r;
}
static pc_ty pc_member_type(pc_ctx *x, pc_class *c, const char *n) {
    pc_ty r = { NULL, NULL, 0 };
    pc_field *f = pc_find_field(c, n);
    if (f) return pc_resolve(x, f->type);
    pc_prop *p = pc_find_prop(c, n);
    if (p) return pc_resolve(x, p->type);
    pc_meth *m = pc_find_meth(c, n);
    if (m && m->isfunc) return pc_resolve(x, m->rtype);
    return r;
}
static pc_ty pc_type(pc_ctx *x, tree_t *e);
static pc_ty pc_ident_type(pc_ctx *x, const char *n) {
    pc_ty r = { NULL, NULL, 0 };
    for (int i = x->nren - 1; i >= 0; i--) if (pc_eq(x->ren[i].from, n)) { n = x->ren[i].to; break; }
    for (int i = x->nwith - 1; i >= 0; i--) if (x->with[i].cls && pc_is_member(x->with[i].cls, n)) return pc_member_type(x, x->with[i].cls, n);
    pc_sym *s = pc_sym_find(x, n);
    if (x->cur && (!s || s->depth < x->mdepth) && pc_is_member(x->cur, n)) return pc_member_type(x, x->cur, n);
    if (!s) return r;
    if (s->kind == 'k') { r.cls = s->cls; r.ref = 1; return r; }
    if (s->kind == 'v' || s->kind == 'f') return pc_resolve(x, s->type);
    return r;
}
static pc_ty pc_record_field(pc_ctx *x, tree_t *rt, const char *n) {
    pc_ty r = { NULL, NULL, 0 };
    for (int i = 0; PC_IS(rt, TT_RECORD) && i < rt->n; i++) {
        tree_t *fl = rt->c[i];
        for (int j = 0; PC_IS(fl, TT_FIELDS) && j < fl->n; j++) {
            tree_t *d = fl->c[j];
            if (!PC_IS(d, TT_DECL) || d->n < 2 || !PC_IS(d->c[0], TT_VLIST)) continue;
            for (int k = 0; k < d->c[0]->n; k++) if (pc_eq(pc_sv(d->c[0]->c[k]), n)) return pc_resolve(x, d->c[1]);
        }
    }
    return r;
}
static pc_ty pc_type(pc_ctx *x, tree_t *e) {
    pc_ty r = { NULL, NULL, 0 };
    if (!e) return r;
    if (PC_IS(e, TT_VAR)) return pc_ident_type(x, pc_sv(e));
    if (PC_IS(e, TT_FIELD) && e->n == 2) {
        pc_ty b = pc_type(x, e->c[0]);
        if (b.cls) { pc_meth *m = pc_find_meth(b.cls, pc_sv(e->c[1])); if (m && pc_eq(m->kind, "constructor")) { r.cls = b.cls; return r; } return pc_member_type(x, b.cls, pc_sv(e->c[1])); }
        return pc_record_field(x, b.t, pc_sv(e->c[1]));
    }
    if (PC_IS(e, TT_DEREF) && e->n == 1) { pc_ty b = pc_type(x, e->c[0]); if (PC_IS(b.t, TT_PTR_TYPE) && b.t->n > 0) return pc_resolve(x, b.t->c[0]); return r; }
    if (PC_IS(e, TT_IDX) && e->n >= 2) {
        pc_ty b = pc_type(x, e->c[0]);
        if (b.cls) { pc_prop *p = pc_find_default(b.cls); if (p) return pc_resolve(x, p->type); return r; }
        if (PC_IS(e->c[0], TT_FIELD) && e->c[0]->n == 2) {
            pc_ty bb = pc_type(x, e->c[0]->c[0]);
            pc_prop *p = bb.cls ? pc_find_prop(bb.cls, pc_sv(e->c[0]->c[1])) : NULL;
            if (p && p->ip) return pc_resolve(x, p->type);
        }
        int left = e->n - 1;
        tree_t *t = b.t;
        while (left > 0 && PC_IS(t, TT_ARRAY_TYPE) && t->n > 0) {
            int dims = t->n - 1 - ((t->n > 0 && PC_IS(t->c[0], TT_KEYWORD)) ? 1 : 0);
            if (dims < 1) dims = 1;
            left -= dims;
            pc_ty el = pc_resolve(x, t->c[t->n - 1]);
            if (left <= 0) return el;
            t = el.t;
        }
        return r;
    }
    if (PC_IS(e, TT_FNC) && e->n >= 1) {
        tree_t *h = e->c[0];
        if (PC_IS(h, TT_VAR)) {
            if (pc_eq(pc_sv(h), "as") && e->n == 3) return pc_ident_type(x, pc_sv(e->c[2])).cls ? (pc_ty){ NULL, pc_ident_type(x, pc_sv(e->c[2])).cls, 0 } : r;
            pc_ty h_t = pc_ident_type(x, pc_sv(h));
            if (h_t.ref && e->n == 2) { r.cls = h_t.cls; return r; }
            pc_sym *s = pc_sym_find(x, pc_sv(h));
            if (s && s->kind == 't' && e->n == 2) return pc_resolve(x, pc_var(pc_sv(h)));
            return h_t;
        }
        if (PC_IS(h, TT_FIELD)) return pc_type(x, h);
    }
    if (PC_IS(e, TT_KEYWORD) && pc_eq(pc_sv(e), "inherited") && e->n == 1 && x->cur && x->cur->parent && PC_IS(e->c[0], TT_FNC)) {
        pc_meth *m = pc_find_meth(x->cur->parent, pc_sv(e->c[0]->c[0]));
        if (m && m->isfunc) return pc_resolve(x, m->rtype);
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pc_add_var(pc_ctx *x, const char *name, tree_t *type) {
    if (!x->vars) { x->vars = ast_node_new(TT_PART); x->vars->v.sval = "var"; }
    ast_push(x->vars, pc_vdecl(name, type));
    pc_sym_add(x, name, 'v', type, NULL);
}
static void pc_ren_push(pc_ctx *x, const char *from, const char *to) {
    if (x->nren >= x->cren) { x->cren = x->cren ? x->cren * 2 : 8; x->ren = ct_grow(x->ren, (size_t)x->cren * sizeof *x->ren); }
    x->ren[x->nren].from = from;
    x->ren[x->nren++].to = to;
}
static void pc_with_push(pc_ctx *x, pc_class *c, const char *tmp, tree_t *rt) {
    if (x->nwith >= x->cwith) { x->cwith = x->cwith ? x->cwith * 2 : 8; x->with = ct_grow(x->with, (size_t)x->cwith * sizeof *x->with); }
    x->with[x->nwith].cls = c;
    x->with[x->nwith].tmp = tmp;
    x->with[x->nwith++].rtype = rt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pc_new_fn(pc_ctx *x, pc_class *c, pc_meth *m) {
    for (int i = 0; i < x->nmade; i++) if (x->made[i].c == c && x->made[i].m == m) return pc_name(pc_impl_name(c, m->name), "__new", NULL);
    if (x->nmade >= x->cmade) { x->cmade = x->cmade ? x->cmade * 2 : 8; x->made = ct_grow(x->made, (size_t)x->cmade * sizeof *x->made); }
    x->made[x->nmade].c = c;
    x->made[x->nmade++].m = m;
    return pc_name(pc_impl_name(c, m->name), "__new", NULL);
}
static tree_t *pc_expr(pc_ctx *x, tree_t *e);
static tree_t *pc_stmt(pc_ctx *x, tree_t *s);
static tree_t *pc_mcall(pc_ctx *x, tree_t *obj, pc_meth *m, tree_t *args, int from, int direct) {
    tree_t *call;
    if (pc_eq(m->kind, "class")) call = pc_fnc(pc_impl_name(m->owner, m->name));
    else if (pc_eq(m->kind, "constructor") && !m->has_impl) return ast_node_new(TT_SUCCEED);
    else {
        pc_meth *root = direct ? NULL : pc_meth_root(m);
        if (root) { root->used = 1; call = pc_fnc(pc_name(pc_impl_name(root->owner, root->name), "__vd", NULL)); } else if (m->has_impl) call = pc_fnc(pc_impl_name(m->owner, m->name));
        else return ast_node_new(TT_SUCCEED);
        ast_push(call, obj);
    }
    for (int i = from; args && i < args->n; i++) ast_push(call, pc_expr(x, args->c[i]));
    return call;
}
static tree_t *pc_self(pc_ctx *x, const char *member) {
    for (int i = x->nwith - 1; i >= 0; i--) if (x->with[i].cls && pc_is_member(x->with[i].cls, member)) return pc_var(x->with[i].tmp);
    return pc_var("self");
}
static pc_class *pc_member_owner(pc_ctx *x, const char *member) {
    for (int i = x->nwith - 1; i >= 0; i--) if (x->with[i].cls && pc_is_member(x->with[i].cls, member)) return x->with[i].cls;
    return x->cur;
}
static int pc_is_with_record_field(pc_ctx *x, const char *n) {
    for (int i = x->nwith - 1; i >= 0; i--) if (!x->with[i].cls && x->with[i].rtype && pc_record_field(x, x->with[i].rtype, n).t) return 1;
    return 0;
}
static int pc_ident_is_member(pc_ctx *x, const char *n) {
    for (int i = x->nren - 1; i >= 0; i--) if (pc_eq(x->ren[i].from, n)) return 0;
    if (pc_is_with_record_field(x, n)) return 0;
    for (int i = x->nwith - 1; i >= 0; i--) if (x->with[i].cls && pc_is_member(x->with[i].cls, n)) return 1;
    if (!x->cur) return 0;
    pc_sym *s = pc_sym_find(x, n);
    return (!s || s->depth < x->mdepth) && pc_is_member(x->cur, n);
}
static tree_t *pc_read_member(pc_ctx *x, tree_t *obj, pc_class *c, const char *n, tree_t *idx) {
    pc_field *f = pc_find_field(c, n);
    if (f) return pc_ofld(obj, n);
    pc_prop *p = pc_find_prop(c, n);
    if (p && p->rd) { if (pc_find_field(c, p->rd)) return pc_ofld(obj, p->rd); pc_meth *g = pc_find_meth(c, p->rd); if (g) return pc_mcall(x, obj, g, idx, 1, 0); }
    pc_meth *m = pc_find_meth(c, n);
    if (m) return pc_mcall(x, obj, m, idx, 1, 0);
    return pc_fld(obj, n);
}
static tree_t *pc_write_member(pc_ctx *x, tree_t *obj, pc_class *c, const char *n, tree_t *idx, tree_t *rhs) {
    pc_prop *p = pc_find_field(c, n) ? NULL : pc_find_prop(c, n);
    const char *w = p ? p->wr : n;
    if (w && pc_find_field(c, w)) return pc_asg(pc_ofld(obj, w), rhs);
    pc_meth *s = w ? pc_find_meth(c, w) : NULL;
    if (!s) return pc_asg(pc_ofld(obj, n), rhs);
    tree_t *args = ast_node_new(TT_VLIST);
    ast_push(args, NULL);
    for (int i = 1; idx && i < idx->n; i++) ast_push(args, idx->c[i]);
    tree_t *call = pc_mcall(x, obj, s, args, 1, 0);
    if (PC_IS(call, TT_FNC)) ast_push(call, rhs);
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_inherited(pc_ctx *x, tree_t *k) {
    if (!x->cur || !x->curm || !x->cur->parent) return ast_node_new(TT_SUCCEED);
    tree_t *call = (k->n > 0 && PC_IS(k->c[0], TT_FNC)) ? k->c[0] : NULL;
    const char *mn = call ? pc_sv(call->c[0]) : x->curm->name;
    pc_meth *m = pc_find_meth(x->cur->parent, mn);
    if (!m) return ast_node_new(TT_SUCCEED);
    if (call) return pc_mcall(x, pc_var("self"), m, call, 1, 1);
    tree_t *args = ast_node_new(TT_VLIST);
    ast_push(args, NULL);
    for (int i = 0; x->curm->params && i < x->curm->params->n; i++) {
        tree_t *d = x->curm->params->c[i];
        if (PC_IS(d, TT_DECL) && d->n > 0 && PC_IS(d->c[0], TT_VLIST)) for (int j = 0; j < d->c[0]->n; j++) ast_push(args, pc_var(pc_sv(d->c[0]->c[j])));
    }
    return pc_mcall(x, pc_var("self"), m, args, 1, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_expr(pc_ctx *x, tree_t *e) {
    if (!e) return e;
    if (PC_IS(e, TT_VAR)) {
        const char *n = pc_sv(e);
        for (int i = x->nren - 1; i >= 0; i--) if (pc_eq(x->ren[i].from, n)) return pc_var(x->ren[i].to);
        if (pc_ident_is_member(x, n)) return pc_read_member(x, pc_self(x, n), pc_member_owner(x, n), n, NULL);
        pc_ty t = pc_ident_type(x, n);
        if (t.ref && t.cls) return pc_int(t.cls->id);
        return e;
    }
    if (PC_IS(e, TT_FIELD) && e->n == 2) {
        pc_ty b = pc_type(x, e->c[0]);
        const char *n = pc_sv(e->c[1]);
        if (b.cls && b.ref) {
            pc_meth *m = pc_find_meth(b.cls, n);
            if (m && pc_eq(m->kind, "constructor")) return pc_fnc(pc_new_fn(x, b.cls, m));
            if (pc_eq(n, "classname")) return pc_leaf(TT_QLIT, b.cls->raw);
            if (m && pc_eq(m->kind, "class")) return pc_mcall(x, NULL, m, NULL, 0, 0);
            return e;
        }
        if (b.cls) return pc_read_member(x, pc_expr(x, e->c[0]), b.cls, n, NULL);
        e->c[0] = pc_expr(x, e->c[0]);
        return e;
    }
    if (PC_IS(e, TT_IDX) && e->n >= 2) {
        pc_ty b = pc_type(x, e->c[0]);
        if (b.cls && !b.ref) { pc_prop *p = pc_find_default(b.cls); if (p) return pc_read_member(x, pc_expr(x, e->c[0]), b.cls, p->name, e); }
        if (PC_IS(e->c[0], TT_FIELD) && e->c[0]->n == 2) {
            pc_ty bb = pc_type(x, e->c[0]->c[0]);
            pc_prop *p = bb.cls && !bb.ref ? pc_find_prop(bb.cls, pc_sv(e->c[0]->c[1])) : NULL;
            if (p && p->ip) return pc_read_member(x, pc_expr(x, e->c[0]->c[0]), bb.cls, p->name, e);
        }
        if (PC_IS(e->c[0], TT_VAR) && pc_ident_is_member(x, pc_sv(e->c[0]))) {
            pc_class *c = pc_member_owner(x, pc_sv(e->c[0]));
            pc_prop *p = pc_find_prop(c, pc_sv(e->c[0]));
            if (p && p->ip) return pc_read_member(x, pc_self(x, pc_sv(e->c[0])), c, p->name, e);
        }
        for (int i = 0; i < e->n; i++) e->c[i] = pc_expr(x, e->c[i]);
        return e;
    }
    if (PC_IS(e, TT_KEYWORD) && pc_eq(pc_sv(e), "inherited")) return pc_inherited(x, e);
    if (PC_IS(e, TT_FNC) && e->n >= 1) {
        tree_t *h = e->c[0];
        if (PC_IS(h, TT_VAR)) {
            const char *n = pc_sv(h);
            pc_sym *s = pc_sym_find(x, n);
            if (pc_eq(n, "is") && e->n == 3 && !s) {
                pc_ty t = pc_ident_type(x, pc_sv(e->c[2]));
                tree_t *c = pc_fnc("__pc_isa");
                ast_push(c, pc_expr(x, e->c[1]));
                ast_push(c, pc_int(t.cls ? t.cls->id : 0));
                return c;
            }
            if (pc_eq(n, "as") && e->n == 3 && !s) return pc_expr(x, e->c[1]);
            if (e->n == 2 && s && (s->kind == 'k' || (s->kind == 't' && PC_IS(pc_resolve(x, h).t, TT_PTR_TYPE)))) return pc_expr(x, e->c[1]);
            if (pc_ident_is_member(x, n)) { pc_class *c = pc_member_owner(x, n); pc_meth *m = pc_find_meth(c, n); if (m) return pc_mcall(x, pc_self(x, n), m, e, 1, 0); }
            for (int i = 1; i < e->n; i++) e->c[i] = pc_expr(x, e->c[i]);
            return e;
        }
        if (PC_IS(h, TT_FIELD) && h->n == 2) {
            pc_ty b = pc_type(x, h->c[0]);
            const char *n = pc_sv(h->c[1]);
            pc_meth *m = b.cls ? pc_find_meth(b.cls, n) : NULL;
            if (m && b.ref) {
                if (pc_eq(m->kind, "constructor")) { tree_t *c = pc_fnc(pc_new_fn(x, b.cls, m)); for (int i = 1; i < e->n; i++) ast_push(c, pc_expr(x, e->c[i])); return c; }
                return pc_mcall(x, NULL, m, e, 1, 0);
            }
            if (m) return pc_mcall(x, pc_expr(x, h->c[0]), m, e, 1, 0);
            if (b.cls && !b.ref) return pc_read_member(x, pc_expr(x, h->c[0]), b.cls, n, e);
        }
        for (int i = 0; i < e->n; i++) e->c[i] = pc_expr(x, e->c[i]);
        return e;
    }
    for (int i = 0; i < e->n; i++) e->c[i] = pc_expr(x, e->c[i]);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_assign(pc_ctx *x, tree_t *s) {
    tree_t *l = s->c[0];
    tree_t *rhs = s->n > 1 ? s->c[1] : NULL;
    int op = s->n > 2;
    if (PC_IS(l, TT_VAR)) {
        const char *n = pc_sv(l);
        pc_sym *sy = pc_sym_find(x, n);
        if (x->curm && x->curm->isfunc && pc_eq(n, x->curm->name) && (!sy || sy->depth < x->mdepth)) { s->c[0] = pc_var(x->cur_impl); s->c[1] = pc_expr(x, rhs); return s; }
        if (!op && pc_ident_is_member(x, n)) return pc_write_member(x, pc_self(x, n), pc_member_owner(x, n), n, NULL, pc_expr(x, rhs));
    }
    if (!op && PC_IS(l, TT_FIELD) && l->n == 2) { pc_ty b = pc_type(x, l->c[0]); if (b.cls && !b.ref) return pc_write_member(x, pc_expr(x, l->c[0]), b.cls, pc_sv(l->c[1]), NULL, pc_expr(x, rhs)); }
    if (!op && PC_IS(l, TT_IDX) && l->n >= 2) {
        pc_ty b = pc_type(x, l->c[0]);
        pc_prop *p = (b.cls && !b.ref) ? pc_find_default(b.cls) : NULL;
        if (p) return pc_write_member(x, pc_expr(x, l->c[0]), b.cls, p->name, l, pc_expr(x, rhs));
        if (PC_IS(l->c[0], TT_FIELD) && l->c[0]->n == 2) {
            pc_ty bb = pc_type(x, l->c[0]->c[0]);
            pc_prop *q = bb.cls && !bb.ref ? pc_find_prop(bb.cls, pc_sv(l->c[0]->c[1])) : NULL;
            if (q && q->ip) return pc_write_member(x, pc_expr(x, l->c[0]->c[0]), bb.cls, q->name, l, pc_expr(x, rhs));
        }
        if (PC_IS(l->c[0], TT_VAR) && pc_ident_is_member(x, pc_sv(l->c[0]))) {
            pc_class *c = pc_member_owner(x, pc_sv(l->c[0]));
            pc_prop *q = pc_find_prop(c, pc_sv(l->c[0]));
            if (q && q->ip) return pc_write_member(x, pc_self(x, pc_sv(l->c[0])), c, q->name, l, pc_expr(x, rhs));
        }
    }
    s->c[0] = pc_expr(x, l);
    if (rhs) s->c[1] = pc_expr(x, rhs);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_raise_tail(pc_ctx *x, tree_t *obj) { tree_t *q = pc_seq(); x->exc = 1; ast_push(q, pc_asg(pc_var("__pc_exc"), obj)); ast_push(q, ast_node_new(TT_DIE)); return q; }
static tree_t *pc_try(pc_ctx *x, tree_t *s) {
    x->exc = 1;
    tree_t *body = pc_stmt(x, s->c[0]);
    if (pc_eq(pc_sv(s), "finally")) {
        const char *t = pc_tmp(x, "f");
        pc_add_var(x, t, pc_var("integer"));
        tree_t *r = pc_n2(TT_TRY, body, pc_stmt(x, s->c[1]));
        ast_push(r, pc_var(t));
        r->v.sval = "finally";
        return r;
    }
    const char *xv = pc_tmp(x, "x");
    pc_add_var(x, xv, pc_var("tobject"));
    const char *saved = x->xv;
    x->xv = xv;
    tree_t *h = pc_seq();
    ast_push(h, pc_asg(pc_var(xv), pc_var("__pc_exc")));
    tree_t *hs = s->c[1];
    if (PC_IS(hs, TT_VLIST)) {
        tree_t *chain = (s->n > 2 && PC_IS(s->c[2], TT_PART)) ? pc_stmt(x, pc_n1(TT_SEQ_EXPR, NULL)) : pc_raise_tail(x, pc_var(xv));
        if (s->n > 2 && PC_IS(s->c[2], TT_PART)) { chain = pc_seq(); for (int i = 0; i < s->c[2]->n; i++) ast_push(chain, pc_stmt(x, s->c[2]->c[i])); }
        for (int i = hs->n - 1; i >= 0; i--) {
            tree_t *c = hs->c[i];
            if (!PC_IS(c, TT_CATCH) || c->n < 2) continue;
            tree_t *ty = c->n == 3 ? c->c[1] : c->c[0];
            pc_ty t = pc_ident_type(x, pc_sv(ty));
            tree_t *test = pc_fnc("__pc_isa");
            ast_push(test, pc_var(xv));
            ast_push(test, pc_int(t.cls ? t.cls->id : 0));
            tree_t *arm = pc_seq();
            int pushed = 0;
            if (c->n == 3) { const char *ev = pc_tmp(x, "e"); pc_add_var(x, ev, pc_var(pc_sv(ty))); ast_push(arm, pc_asg(pc_var(ev), pc_var(xv))); pc_ren_push(x, pc_sv(c->c[0]), ev); pushed = 1; }
            ast_push(arm, pc_stmt(x, c->c[c->n - 1]));
            if (pushed) x->nren--;
            chain = pc_if(test, arm, chain);
        }
        ast_push(h, chain);
    } else ast_push(h, pc_stmt(x, hs));
    x->xv = saved;
    tree_t *r = pc_n2(TT_TRY, body, h);
    r->v.sval = "except";
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_with_stmt(pc_ctx *x, tree_t *s) {
    tree_t *pre = pc_seq();
    tree_t *keep = ast_node_new(TT_WITH);
    int base = x->nwith;
    for (int i = 0; i + 1 < s->n; i++) {
        pc_ty t = pc_type(x, s->c[i]);
        if (t.cls && !t.ref) {
            const char *tmp = pc_tmp(x, "w");
            pc_add_var(x, tmp, pc_var(t.cls->name));
            ast_push(pre, pc_asg(pc_var(tmp), pc_expr(x, s->c[i])));
            pc_with_push(x, t.cls, tmp, NULL);
        } else {
            ast_push(keep, pc_expr(x, s->c[i]));
            pc_with_push(x, NULL, NULL, t.t);
        }
    }
    tree_t *body = pc_stmt(x, s->c[s->n - 1]);
    x->nwith = base;
    if (keep->n > 0) { ast_push(keep, body); body = keep; }
    if (pre->n == 0) return body;
    ast_push(pre, body);
    return pre;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_stmt(pc_ctx *x, tree_t *s) {
    if (!s) return s;
    switch (s->t) {
        case TT_ASSIGN:
        return pc_assign(x, s);
        case TT_FNC:
        if (s->n == 2 && PC_IS(s->c[0], TT_VAR) && pc_eq(pc_sv(s->c[0]), "freeandnil") && !pc_sym_find(x, "freeandnil")) {
            tree_t *q = pc_seq();
            tree_t *fr = pc_fnc("__pc_tobject__free");
            ast_push(fr, pc_expr(x, pc_clone(s->c[1])));
            ast_push(q, fr);
            ast_push(q, pc_asg(pc_expr(x, s->c[1]), pc_var("nil")));
            return q;
        }
        if (s->n == 1 && PC_IS(s->c[0], TT_VAR) && pc_ident_is_member(x, pc_sv(s->c[0]))) {
            pc_class *c = pc_member_owner(x, pc_sv(s->c[0]));
            pc_meth *m = pc_find_meth(c, pc_sv(s->c[0]));
            if (m) return pc_mcall(x, pc_self(x, pc_sv(s->c[0])), m, NULL, 0, 0);
        }
        return pc_expr(x, s);
        case TT_KEYWORD:
        return pc_eq(pc_sv(s), "inherited") ? pc_inherited(x, s) : s;
        case TT_DIE:
        if (s->n > 0) return pc_raise_tail(x, pc_expr(x, s->c[0]));
        if (x->xv) return pc_raise_tail(x, pc_var(x->xv));
        x->exc = 1;
        return s;
        case TT_TRY:
        return pc_try(x, s);
        case TT_WITH:
        return pc_with_stmt(x, s);
        case TT_SEQ_EXPR:
        for (int i = 0; i < s->n; i++) s->c[i] = pc_stmt(x, s->c[i]);
        return s;
        case TT_LABEL_DEF:
        if (s->n > 1) s->c[1] = pc_stmt(x, s->c[1]);
        return s;
        case TT_IF:
        s->c[0] = pc_expr(x, s->c[0]);
        for (int i = 1; i < s->n; i++) s->c[i] = pc_stmt(x, s->c[i]);
        return s;
        case TT_WHILE:
        s->c[0] = pc_expr(x, s->c[0]);
        s->c[1] = pc_stmt(x, s->c[1]);
        return s;
        case TT_REPEAT:
        s->c[0] = pc_stmt(x, s->c[0]);
        s->c[1] = pc_expr(x, s->c[1]);
        return s;
        case TT_FOR:
        for (int i = 1; i + 1 < s->n; i++) s->c[i] = pc_expr(x, s->c[i]);
        s->c[s->n - 1] = pc_stmt(x, s->c[s->n - 1]);
        return s;
        case TT_CASE:
        s->c[0] = pc_expr(x, s->c[0]);
        for (int i = 1; i < s->n; i++) {
            tree_t *a = s->c[i];
            if (PC_IS(a, TT_ARM) && a->n > 1) a->c[1] = pc_stmt(x, a->c[1]);
            else if (PC_IS(a, TT_PART)) for (int j = 0; j < a->n; j++) a->c[j] = pc_stmt(x, a->c[j]);
        }
        return s;
        default:
        return s;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pc_param_syms(pc_ctx *x, tree_t *ps) {
    for (int i = 0; PC_IS(ps, TT_PARAMS) && i < ps->n; i++) {
        tree_t *d = ps->c[i];
        if (!PC_IS(d, TT_DECL) || d->n < 1) continue;
        if (PC_IS(d->c[0], TT_VLIST)) { for (int k = 0; k < d->c[0]->n; k++) pc_sym_add(x, pc_sv(d->c[0]->c[k]), 'v', d->n > 1 ? d->c[1] : NULL, NULL); continue; }
        pc_sym_add(x, pc_sv(d->c[0]), pc_eq(pc_sv(d), "function") ? 'f' : 'p', pc_eq(pc_sv(d), "function") ? d->c[d->n - 1] : NULL, NULL);
    }
}
static void pc_block_syms(pc_ctx *x, tree_t *b) {
    for (int i = 0; b && i < b->n; i++) {
        tree_t *p = b->c[i];
        if (PC_IS(p, TT_PROCEDURE) || PC_IS(p, TT_FUNCTION)) {
            if (!PC_IS(p->c[0], TT_VAR)) continue;
            tree_t *rt = NULL;
            for (int k = 1; k < p->n; k++) if (PC_IS(p->c[k], TT_VAR)) rt = p->c[k];
            pc_sym_add(x, pc_sv(p->c[0]), PC_IS(p, TT_FUNCTION) ? 'f' : 'p', rt, NULL);
            continue;
        }
        if (!PC_IS(p, TT_PART)) continue;
        for (int j = 0; j < p->n; j++) {
            tree_t *d = p->c[j];
            if (!PC_IS(d, TT_DECL) || d->n < 1) continue;
            if (pc_eq(pc_sv(p), "type") && d->n >= 2) {
                pc_class *c = PC_IS(d->c[1], TT_CLASS_DECL) ? pc_class_named(x, pc_sv(d->c[0])) : NULL;
                if (c || PC_IS(d->c[1], TT_CLASS_DECL)) { pc_sym_add(x, pc_sv(d->c[0]), 'k', NULL, c); continue; }
                pc_sym_add(x, pc_sv(d->c[0]), 't', d->c[1], NULL);
            } else if (pc_eq(pc_sv(p), "var") && PC_IS(d->c[0], TT_VLIST)) {
                for (int k = 0; k < d->c[0]->n; k++) pc_sym_add(x, pc_sv(d->c[0]->c[k]), 'v', d->n > 1 ? d->c[1] : NULL, NULL);
            } else if (pc_eq(pc_sv(p), "const")) {
                if (pc_eq(pc_sv(d), "tconst") && PC_IS(d->c[0], TT_VLIST) && d->c[0]->n > 0) pc_sym_add(x, pc_sv(d->c[0]->c[0]), 'v', d->n > 1 ? d->c[1] : NULL, NULL);
                else pc_sym_add(x, pc_sv(d->c[0]), 'c', NULL, NULL);
            }
        }
    }
}
static void pc_insert_vars(tree_t *b, tree_t *vars) {
    if (!vars || !b) return;
    for (int i = 0; i < b->n; i++) if (PC_IS(b->c[i], TT_PART) && pc_eq(pc_sv(b->c[i]), "var")) { for (int j = 0; j < vars->n; j++) ast_push(b->c[i], vars->c[j]); return; }
    int at = 0;
    for (int i = 0; i < b->n; i++) if (PC_IS(b->c[i], TT_PART) && (pc_eq(pc_sv(b->c[i]), "label") || pc_eq(pc_sv(b->c[i]), "const") || pc_eq(pc_sv(b->c[i]), "type") || pc_eq(pc_sv(b->c[i]), "var")))
        at = i + 1;
    ast_push(b, NULL);
    for (int i = b->n - 1; i > at; i--) b->c[i] = b->c[i - 1];
    b->c[at] = vars;
}
static void pc_block(pc_ctx *x, tree_t *b);
static void pc_routine(pc_ctx *x, tree_t *r) {
    tree_t *blk = NULL, *ps = NULL, *rt = NULL;
    for (int k = 1; k < r->n; k++) { if (PC_IS(r->c[k], TT_BLOCK)) blk = r->c[k]; else if (PC_IS(r->c[k], TT_PARAMS)) ps = r->c[k]; else if (PC_IS(r->c[k], TT_VAR)) rt = r->c[k]; }
    pc_class *sc = x->cur;
    pc_meth *sm = x->curm;
    const char *si = x->cur_impl;
    int smd = x->mdepth;
    int nsym = x->nsym;
    x->depth++;
    if (PC_IS(r->c[0], TT_FIELD) && r->c[0]->n == 2) {
        pc_class *c = pc_class_named(x, pc_sv(r->c[0]->c[0]));
        pc_meth *m = c ? pc_find_meth(c, pc_sv(r->c[0]->c[1])) : NULL;
        if (c && m && m->owner == c) {
            const char *impl = pc_impl_name(c, m->name);
            r->c[0] = pc_var(impl);
            r->v.sval = NULL;
            int cls_m = pc_eq(m->kind, "class");
            if (!cls_m) {
                tree_t *np = ast_node_new(TT_PARAMS);
                tree_t *sd = pc_n2(TT_DECL, pc_n1(TT_VLIST, pc_var("self")), pc_var(c->name));
                sd->v.sval = "value";
                ast_push(np, sd);
                for (int i = 0; ps && i < ps->n; i++) ast_push(np, ps->c[i]);
                if (ps) { for (int k = 1; k < r->n; k++) if (r->c[k] == ps) r->c[k] = np; } else { ast_push(r, NULL); for (int k = r->n - 1; k > 1; k--) r->c[k] = r->c[k - 1]; r->c[1] = np; }
                ps = np;
            }
            x->cur = c;
            x->curm = m;
            x->cur_impl = impl;
            x->mdepth = x->depth;
        }
    } else if (!PC_IS(r->c[0], TT_FIELD)) {
        x->cur = sc;
        x->curm = NULL;
    }
    pc_param_syms(x, ps);
    if (PC_IS(r, TT_FUNCTION) && rt) pc_sym_add(x, "result", 'v', rt, NULL);
    if (blk) pc_block(x, blk);
    x->nsym = nsym;
    x->depth--;
    x->cur = sc;
    x->curm = sm;
    x->cur_impl = si;
    x->mdepth = smd;
}
static void pc_block(pc_ctx *x, tree_t *b) {
    tree_t *sv = x->vars;
    x->vars = NULL;
    int nsym = x->nsym;
    x->depth++;
    pc_block_syms(x, b);
    for (int i = 0; i < b->n; i++) {
        tree_t *p = b->c[i];
        if (PC_IS(p, TT_PROCEDURE) || PC_IS(p, TT_FUNCTION)) pc_routine(x, p);
        else if (PC_IS(p, TT_PART) && pc_eq(pc_sv(p), "type") && pc_part_has_class(p)) pc_type_part(x, p, 0);
    }
    if (b->n > 0 && PC_IS(b->c[b->n - 1], TT_SEQ_EXPR)) b->c[b->n - 1] = pc_stmt(x, b->c[b->n - 1]);
    pc_insert_vars(b, x->vars);
    x->nsym = nsym;
    x->depth--;
    x->vars = sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_self_params(pc_class *c, tree_t *ps) {
    tree_t *np = ast_node_new(TT_PARAMS);
    if (c) { tree_t *sd = pc_n2(TT_DECL, pc_n1(TT_VLIST, pc_var("self")), pc_var(c->name)); sd->v.sval = "value"; ast_push(np, sd); }
    for (int i = 0; PC_IS(ps, TT_PARAMS) && i < ps->n; i++) ast_push(np, pc_clone(ps->c[i]));
    return np->n ? np : NULL;
}
static void pc_param_args(tree_t *call, tree_t *ps) {
    for (int i = 0; PC_IS(ps, TT_PARAMS) && i < ps->n; i++) {
        tree_t *d = ps->c[i];
        if (PC_IS(d, TT_DECL) && d->n > 0 && PC_IS(d->c[0], TT_VLIST)) for (int j = 0; j < d->c[0]->n; j++) ast_push(call, pc_var(pc_sv(d->c[0]->c[j])));
        else if (PC_IS(d, TT_DECL) && d->n > 0) ast_push(call, pc_var(pc_sv(d->c[0])));
    }
}
static tree_t *pc_routine_node(int isfunc, const char *name, tree_t *ps, tree_t *rt, tree_t *tail) {
    tree_t *r = ast_node_new(isfunc ? TT_FUNCTION : TT_PROCEDURE);
    ast_push(r, pc_var(name));
    if (ps) ast_push(r, ps);
    if (isfunc) ast_push(r, pc_clone(rt));
    ast_push(r, tail);
    return r;
}
static tree_t *pc_blk(tree_t *vars, tree_t *body) { tree_t *b = ast_node_new(TT_BLOCK); if (vars) ast_push(b, vars); ast_push(b, body); return b; }
static tree_t *pc_vpart(const char *name, tree_t *type) { tree_t *p = ast_node_new(TT_PART); p->v.sval = "var"; ast_push(p, pc_vdecl(name, type)); return p; }
static void pc_emit(tree_t *fwd, tree_t *gen, int isfunc, const char *name, tree_t *ps, tree_t *rt, tree_t *blk) {
    ast_push(fwd, pc_routine_node(isfunc, name, pc_clone(ps), rt, pc_leaf(TT_KEYWORD, "forward")));
    if (blk) ast_push(gen, pc_routine_node(isfunc, name, ps, rt, blk));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pc_init_field(pc_ctx *x, tree_t *lhs, tree_t *type) {
    static const char *const strs[] = { "string", "ansistring", "shortstring", "widestring", "unicodestring", "rawbytestring", "utf8string", 0 };
    static const char *const nums[] = { "integer", "longint", "int64", "byte", "word", "cardinal", "shortint", "smallint", "longword", "qword", "nativeint", "nativeuint", "sizeint", "ptrint", "real",
        "double", "single", "extended", "currency", "comp", 0 };
    pc_ty t = pc_resolve(x, type);
    if (t.cls) return pc_asg(lhs, pc_var("nil"));
    if (PC_IS(t.t, TT_PTR_TYPE)) return pc_asg(lhs, pc_var("nil"));
    if (PC_IS(t.t, TT_VAR) && pc_word_in(pc_sv(t.t), strs)) return pc_asg(lhs, pc_leaf(TT_QLIT, ""));
    if (PC_IS(t.t, TT_VAR) && pc_word_in(pc_sv(t.t), nums)) return pc_asg(lhs, pc_int(0));
    if (PC_IS(t.t, TT_VAR) && pc_eq(pc_sv(t.t), "boolean")) return pc_asg(lhs, pc_var("false"));
    if (PC_IS(t.t, TT_VAR) && pc_eq(pc_sv(t.t), "char")) { tree_t *c = ast_node_new(TT_CHRLIT); c->v.ival = 0; return pc_asg(lhs, c); }
    if (PC_IS(t.t, TT_ARRAY_TYPE) && t.t->n - ((t.t->n > 0 && PC_IS(t.t->c[0], TT_KEYWORD)) ? 1 : 0) == 1) { tree_t *c = pc_fnc("setlength"); ast_push(c, lhs); ast_push(c, pc_int(0)); return c; }
    return NULL;
}
static void pc_gen_new(pc_ctx *x, tree_t *fwd, tree_t *gen, pc_class *c, pc_meth *m) {
    const char *nm = pc_name(pc_impl_name(c, m->name), "__new", NULL);
    tree_t *body = pc_seq();
    ast_push(body, pc_n2(TT_FNC, pc_var("new"), pc_var("self")));
    ast_push(body, pc_asg(pc_ofld(pc_var("self"), "__cls"), pc_int(c->id)));
    for (pc_class *a = c; a; a = a->parent) for (int i = 0; i < a->nf; i++) { tree_t *s = pc_init_field(x, pc_ofld(pc_var("self"), a->f[i].name), a->f[i].type); if (s) ast_push(body, s); }
    if (m->has_impl) { tree_t *call = pc_fnc(pc_impl_name(m->owner, m->name)); ast_push(call, pc_var("self")); pc_param_args(call, m->params); ast_push(body, call); }
    ast_push(body, pc_asg(pc_var(nm), pc_var("self")));
    pc_emit(fwd, gen, 1, nm, pc_self_params(NULL, m->params), pc_var(c->name), pc_blk(pc_vpart("self", pc_var(c->name)), body));
}
static void pc_gen_dispatch(pc_ctx *x, tree_t *fwd, tree_t *gen, pc_meth *root) {
    const char *nm = pc_name(pc_impl_name(root->owner, root->name), "__vd", NULL);
    tree_t *cs = pc_n1(TT_CASE, pc_ofld(pc_var("self"), "__cls"));
    for (int i = 0; i < x->ncls; i++) {
        pc_class *d = x->cls[i];
        if (!pc_descends(d, root->owner)) continue;
        pc_meth *im = pc_impl_for(d, root);
        if (!im) continue;
        tree_t *call = pc_fnc(pc_impl_name(im->owner, im->name));
        ast_push(call, pc_var("self"));
        pc_param_args(call, root->params);
        ast_push(cs, pc_n2(TT_ARM, pc_n1(TT_VLIST, pc_int(d->id)), root->isfunc ? pc_asg(pc_var(nm), call) : call));
    }
    if (cs->n == 1) return;
    pc_emit(fwd, gen, root->isfunc, nm, pc_self_params(root->owner, root->params), root->rtype, pc_blk(NULL, pc_n1(TT_SEQ_EXPR, cs)));
}
static void pc_gen_root(pc_ctx *x, tree_t *fwd, tree_t *gen) {
    tree_t *ps = pc_self_params(x->root, NULL);
    pc_emit(fwd, gen, 0, "__pc_tobject__destroy", pc_clone(ps), NULL, pc_blk(NULL, pc_n1(TT_SEQ_EXPR, ast_node_new(TT_SUCCEED))));
    tree_t *dcall = pc_fnc(x->root->m[1].used ? "__pc_tobject__destroy__vd" : "__pc_tobject__destroy");
    ast_push(dcall, pc_var("self"));
    x->root->m[1].used = 1;
    pc_emit(fwd, gen, 0, "__pc_tobject__free", pc_clone(ps), NULL, pc_blk(NULL, pc_n1(TT_SEQ_EXPR, pc_if(pc_n2(TT_NE, pc_var("self"), pc_var("nil")), dcall, NULL))));
    tree_t *cn = pc_fnc("__pc_classname");
    ast_push(cn, pc_ofld(pc_var("self"), "__cls"));
    pc_emit(fwd, gen, 1, "__pc_tobject__classname", pc_clone(ps), pc_var("string"), pc_blk(NULL, pc_n1(TT_SEQ_EXPR, pc_asg(pc_var("__pc_tobject__classname"), cn))));
    tree_t *cn2 = pc_fnc("__pc_tobject__classname");
    ast_push(cn2, pc_var("self"));
    pc_emit(fwd, gen, 1, "__pc_tobject__tostring", pc_clone(ps), pc_var("string"), pc_blk(NULL, pc_n1(TT_SEQ_EXPR, pc_asg(pc_var("__pc_tobject__tostring"), cn2))));
    tree_t *nps = ast_node_new(TT_PARAMS);
    tree_t *cd = pc_n2(TT_DECL, pc_n1(TT_VLIST, pc_var("c")), pc_var("integer"));
    cd->v.sval = "value";
    ast_push(nps, cd);
    tree_t *cs = pc_n1(TT_CASE, pc_var("c"));
    for (int i = 0; i < x->ncls; i++) ast_push(cs, pc_n2(TT_ARM, pc_n1(TT_VLIST, pc_int(x->cls[i]->id)), pc_asg(pc_var("__pc_classname"), pc_leaf(TT_QLIT, x->cls[i]->raw))));
    pc_emit(fwd, gen, 1, "__pc_classname", nps, pc_var("string"), pc_blk(NULL, pc_n1(TT_SEQ_EXPR, cs)));
    tree_t *ips = ast_node_new(TT_PARAMS);
    tree_t *od = pc_n2(TT_DECL, pc_n1(TT_VLIST, pc_var("o")), pc_var("tobject"));
    od->v.sval = "value";
    ast_push(ips, od);
    tree_t *td = pc_n2(TT_DECL, pc_n1(TT_VLIST, pc_var("t")), pc_var("integer"));
    td->v.sval = "value";
    ast_push(ips, td);
    tree_t *pcs = pc_n1(TT_CASE, pc_var("c"));
    for (int i = 0; i < x->ncls; i++) ast_push(pcs, pc_n2(TT_ARM, pc_n1(TT_VLIST, pc_int(x->cls[i]->id)), pc_asg(pc_var("c"), pc_int(x->cls[i]->parent ? x->cls[i]->parent->id : 0))));
    tree_t *hit = pc_seq();
    ast_push(hit, pc_asg(pc_var("__pc_isa"), pc_var("true")));
    ast_push(hit, pc_asg(pc_var("c"), pc_int(0)));
    tree_t *loop = pc_n2(TT_WHILE, pc_n2(TT_NE, pc_var("c"), pc_int(0)), pc_if(pc_n2(TT_EQ, pc_var("c"), pc_var("t")), hit, pcs));
    tree_t *walk = pc_seq();
    ast_push(walk, pc_asg(pc_var("c"), pc_ofld(pc_var("o"), "__cls")));
    ast_push(walk, loop);
    tree_t *ib = pc_seq();
    ast_push(ib, pc_asg(pc_var("__pc_isa"), pc_var("false")));
    ast_push(ib, pc_if(pc_n2(TT_NE, pc_var("o"), pc_var("nil")), walk, NULL));
    pc_emit(fwd, gen, 1, "__pc_isa", ips, pc_var("boolean"), pc_blk(pc_vpart("c", pc_var("integer")), ib));
}
static void pc_gen_unhandled(pc_ctx *x, tree_t *fwd, tree_t *gen) {
    pc_class *ex = pc_class_named(x, "exception");
    pc_prop *mp = ex ? pc_find_prop(ex, "message") : NULL;
    const char *mf = mp && mp->rd && pc_find_field(ex, mp->rd) ? mp->rd : NULL;
    tree_t *body = pc_seq();
    ast_push(body, pc_asg(pc_var("m"), pc_leaf(TT_QLIT, "")));
    tree_t *vars = pc_vpart("m", pc_var("string"));
    if (mf) {
        ast_push(vars, pc_vdecl("e", pc_var("exception")));
        tree_t *test = pc_fnc("__pc_isa");
        ast_push(test, pc_var("__pc_exc"));
        ast_push(test, pc_int(ex->id));
        tree_t *get = pc_seq();
        ast_push(get, pc_asg(pc_var("e"), pc_var("__pc_exc")));
        ast_push(get, pc_asg(pc_var("m"), pc_ofld(pc_var("e"), mf)));
        ast_push(body, pc_if(test, get, NULL));
    }
    tree_t *cn = pc_fnc("__pc_classname");
    ast_push(cn, pc_ofld(pc_var("__pc_exc"), "__cls"));
    tree_t *rep = pc_fnc("__pas_unhandled");
    ast_push(rep, cn);
    ast_push(rep, pc_var("m"));
    ast_push(body, rep);
    pc_emit(fwd, gen, 0, "__pc_unhandled", NULL, NULL, pc_blk(vars, body));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pc_has_class(const tree_t *t) { if (!t) return 0; if (PC_IS(t, TT_CLASS_DECL)) return 1; for (int i = 0; i < t->n; i++) if (pc_has_class(t->c[i])) return 1; return 0; }
static int pc_has_exc(const tree_t *t) { if (!t) return 0; if (PC_IS(t, TT_TRY) || PC_IS(t, TT_DIE)) return 1; for (int i = 0; i < t->n; i++) if (pc_has_exc(t->c[i])) return 1; return 0; }
static void pc_impl_fwds(pc_ctx *x, tree_t *b, tree_t *fwd) {
    for (int i = 0; i < b->n; i++) {
        tree_t *r = b->c[i];
        if (!(PC_IS(r, TT_PROCEDURE) || PC_IS(r, TT_FUNCTION)) || !PC_IS(r->c[0], TT_VAR) || strncmp(pc_sv(r->c[0]), "__pc_", 5)) continue;
        tree_t *ps = NULL, *rt = NULL;
        for (int k = 1; k < r->n; k++) { if (PC_IS(r->c[k], TT_PARAMS)) ps = r->c[k]; else if (PC_IS(r->c[k], TT_VAR)) rt = r->c[k]; }
        ast_push(fwd, pc_routine_node(PC_IS(r, TT_FUNCTION), pc_sv(r->c[0]), pc_clone(ps), rt, pc_leaf(TT_KEYWORD, "forward")));
    }
    (void)x;
}
tree_t *pas_classes_prepass(tree_t *prog) {
    if (!PC_IS(prog, TT_PROGRAM)) return prog;
    tree_t *b = NULL;
    for (int i = 0; i < prog->n; i++) if (PC_IS(prog->c[i], TT_BLOCK)) b = prog->c[i];
    if (!b || !(pc_has_class(b) || pc_has_exc(b))) return prog;
    pc_ctx cx;
    memset(&cx, 0, sizeof cx);
    pc_ctx *x = &cx;
    pc_root_class(x);
    pc_collect(x, b);
    pc_link(x);
    pc_scan_impls(x, b);
    pc_sym_add(x, "tobject", 'k', NULL, x->root);
    pc_sym_add(x, "__pc_exc", 'v', pc_var("tobject"), NULL);
    int tp = -1;
    for (int i = 0; i < b->n; i++) if (PC_IS(b->c[i], TT_PART) && pc_eq(pc_sv(b->c[i]), "type")) { tp = i; break; }
    if (tp < 0) {
        tree_t *p = ast_node_new(TT_PART);
        p->v.sval = "type";
        int at = 0;
        for (int i = 0; i < b->n; i++) if (PC_IS(b->c[i], TT_PART) && (pc_eq(pc_sv(b->c[i]), "label") || pc_eq(pc_sv(b->c[i]), "const"))) at = i + 1;
        ast_push(b, NULL);
        for (int i = b->n - 1; i > at; i--) b->c[i] = b->c[i - 1];
        b->c[at] = p;
        tp = at;
    }
    int has_cls_tp = pc_part_has_class(b->c[tp]);
    pc_block(x, b);
    if (!has_cls_tp) pc_type_part(x, b->c[tp], 1);
    else {
        tree_t *p = b->c[tp];
        ast_push(p, NULL);
        ast_push(p, NULL);
        for (int i = p->n - 1; i > 1; i--) p->c[i] = p->c[i - 2];
        p->c[0] = pc_tdecl("tobject", pc_n1(TT_PTR_TYPE, pc_var("tobject__rec")));
        p->c[1] = pc_tdecl("tobject__rec", pc_rec_type(x, x->root));
    }
    tree_t *fwd = ast_node_new(TT_VLIST), *gen = ast_node_new(TT_VLIST);
    pc_impl_fwds(x, b, fwd);
    if (x->exc) {
        tree_t *main = b->c[b->n - 1];
        tree_t *h = pc_seq();
        ast_push(h, pc_fnc("__pc_unhandled"));
        tree_t *tr = pc_n2(TT_TRY, main, h);
        tr->v.sval = "except";
        b->c[b->n - 1] = pc_n1(TT_SEQ_EXPR, tr);
        pc_gen_unhandled(x, fwd, gen);
        ast_push(b, NULL);
        for (int i = b->n - 1; i > tp + 1; i--) b->c[i] = b->c[i - 1];
        b->c[tp + 1] = pc_vpart("__pc_exc", pc_var("tobject"));
    }
    for (int i = 0; i < x->nmade; i++) pc_gen_new(x, fwd, gen, x->made[i].c, x->made[i].m);
    pc_gen_root(x, fwd, gen);
    for (int i = 0; i < x->ncls; i++) for (int j = 0; j < x->cls[i]->nm; j++) { pc_meth *m = &x->cls[i]->m[j]; if (m->virt && !m->over && m->used) pc_gen_dispatch(x, fwd, gen, m); }
    int at = b->n - 1;
    for (int i = 0; i < b->n - 1; i++) if (PC_IS(b->c[i], TT_PROCEDURE) || PC_IS(b->c[i], TT_FUNCTION)) { at = i; break; }
    tree_t *nb = ast_node_new(TT_BLOCK);
    nb->line = b->line;
    for (int i = 0; i < at; i++) ast_push(nb, b->c[i]);
    for (int i = 0; i < fwd->n; i++) ast_push(nb, fwd->c[i]);
    for (int i = at; i < b->n - 1; i++) ast_push(nb, b->c[i]);
    for (int i = 0; i < gen->n; i++) ast_push(nb, gen->c[i]);
    ast_push(nb, b->c[b->n - 1]);
    for (int i = 0; i < prog->n; i++) if (prog->c[i] == b) prog->c[i] = nb;
    return prog;
}
