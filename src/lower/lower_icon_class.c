#include <string.h>
#include <stdio.h>
#include "ct_arena.h"
#include "lower.h"
#include "g_lower.h"
#include "../parsers/snobol4/scrip_cc.h"
typedef struct { const char ** v; int n, cap; } cx_sv;
typedef struct { const char * name; const char * owner; const tree_t * proc; } cx_meth;
typedef struct cx_class { const tree_t * node; const char * name; cx_sv supers, fields, anc; cx_meth * meth; int nmeth, cmeth; const tree_t * init; int built; } cx_class;
typedef struct { cx_class * cls; int ncls, ccls, tmp; cx_sv gmeth; } cx_st;
typedef struct { cx_st * st; const cx_class * cls; cx_sv shadow; } cx_ctx;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cx_push(cx_sv * s, const char * w) {
    if (s->n == s->cap) { s->cap = s->cap ? s->cap * 2 : 8; s->v = (const char **) ct_grow((void *) s->v, (size_t) s->cap * sizeof *s->v); }
    s->v[s->n++] = w;
}
static int cx_has(const cx_sv * s, const char * w) { for (int i = 0; i < s->n; i++) if (!strcmp(s->v[i], w)) return 1; return 0; }
static void cx_push_new(cx_sv * s, const char * w) { if (!cx_has(s, w)) cx_push(s, w); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * cx_var(const char * s) { tree_t * e = ast_node_new(TT_VAR); e->v.sval = ct_strdup(s); e->slen = (int) strlen(s); return e; }
static tree_t * cx_bin(tree_e k, tree_t * a, tree_t * b) { tree_t * e = ast_node_new(k); ast_push(e, a); ast_push(e, b); return e; }
static tree_t * cx_un(tree_e k, tree_t * a) { tree_t * e = ast_node_new(k); ast_push(e, a); return e; }
static tree_t * cx_fld(tree_t * o, const char * f) { return cx_bin(TT_FIELD, o, cx_var(f)); }
static char * cx_cat(const char * a, const char * mid, const char * b) {
    size_t n = strlen(a) + strlen(mid) + strlen(b) + 1;
    char * r = (char *) ct_alloc(n);
    snprintf(r, n, "%s%s%s", a, mid, b);
    return r;
}
static tree_t * cx_call(tree_t * f, int n, tree_t ** a) { tree_t * c = ast_node_new(TT_FNC); ast_push(c, f); for (int i = 0; i < n; i++) ast_push(c, a[i]); return c; }
static tree_t * cx_stmt(tree_t * top) {
    tree_t * st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":line", 0));
    ast_push(st, ast_attr_int(":stno", 0));
    ast_push(st, ast_attr_expr(":subj", top));
    return st;
}
static cx_class * cx_find(cx_st * st, const char * nm) { for (int i = 0; i < st->ncls; i++) if (!strcmp(st->cls[i].name, nm)) return &st->cls[i]; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cx_mpush(cx_class * c, const char * name, const char * owner, const tree_t * proc) {
    for (int i = 0; i < c->nmeth; i++) if (!strcmp(c->meth[i].name, name)) return;
    if (c->nmeth == c->cmeth) { c->cmeth = c->cmeth ? c->cmeth * 2 : 8; c->meth = (cx_meth *) ct_grow(c->meth, (size_t) c->cmeth * sizeof *c->meth); }
    c->meth[c->nmeth].name = name;
    c->meth[c->nmeth].owner = owner;
    c->meth[c->nmeth].proc = proc;
    c->nmeth++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cx_build(cx_st * st, cx_class * c) {
    if (c->built) return;
    c->built = 1;
    const tree_t * n = c->node;
    for (int i = 0; i < n->c[0]->n; i++) cx_push(&c->supers, n->c[0]->c[i]->v.sval);
    for (int i = 0; i < n->c[1]->n; i++) cx_push_new(&c->fields, n->c[1]->c[i]->v.sval);
    for (int i = 2; i < n->n; i++) {
        const tree_t * m = n->c[i];
        if (!strcmp(m->v.sval, "initially")) c->init = m;
        cx_mpush(c, m->v.sval, c->name, m);
        if (!cx_has(&st->gmeth, m->v.sval)) cx_push(&st->gmeth, m->v.sval);
    }
    for (int s = 0; s < c->supers.n; s++) {
        cx_class * sc = cx_find(st, c->supers.v[s]);
        if (!sc) continue;
        cx_build(st, sc);
        for (int i = 0; i < sc->fields.n; i++) cx_push_new(&c->fields, sc->fields.v[i]);
        for (int i = 0; i < sc->nmeth; i++) cx_mpush(c, sc->meth[i].name, sc->meth[i].owner, sc->meth[i].proc);
        cx_push_new(&c->anc, sc->name);
        for (int i = 0; i < sc->anc.n; i++) cx_push_new(&c->anc, sc->anc.v[i]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cx_locals(const tree_t * n, cx_sv * out) {
    if (!n) return;
    if (n->t == TT_LOCAL || n->t == TT_STATIC_DECL) { for (int i = 0; i < n->n; i++) if (n->c[i] && n->c[i]->v.sval) cx_push_new(out, n->c[i]->v.sval); return; }
    for (int i = 0; i < n->n; i++) cx_locals(n->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * cx_expr(tree_t * n, cx_ctx * x);
static void cx_kids(tree_t * n, cx_ctx * x, int from) {
    if (n->t == TT_LOCAL || n->t == TT_STATIC_DECL) return;
    for (int i = from; i < n->n; i++) { if (!n->c[i]) continue; if (n->t == TT_FIELD && i == 1) continue; n->c[i] = cx_expr(n->c[i], x); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cx_is_method(const cx_ctx * x, const char * nm) {
    if (!x->cls || cx_has(&x->shadow, nm) || cx_has(&x->cls->fields, nm)) return 0;
    for (int i = 0; i < x->cls->nmeth; i++) if (!strcmp(x->cls->meth[i].name, nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * cx_dispatch(tree_t * n, cx_ctx * x) {
    tree_t * f = n->c[0];
    const char * m = f->c[1]->v.sval;
    tree_t * obj = f->c[0];
    tree_t * mrec;
    tree_t * ref;
    if (obj->t == TT_FIELD && obj->n > 1 && obj->c[1] && obj->c[1]->t == TT_VAR && cx_find(x->st, obj->c[1]->v.sval)) {
        const char * sup = obj->c[1]->v.sval;
        tree_t * base = cx_expr(obj->c[0], x);
        tree_t * bref = base->t == TT_VAR ? cx_var(base->v.sval) : NULL;
        if (bref) {
            mrec = cx_fld(cx_fld(base, "__m"), sup);
            ref = bref;
        } else {
            char buf[24];
            snprintf(buf, sizeof buf, "%d", ++x->st->tmp);
            char * tmp = cx_cat("__o", buf, "");
            mrec = cx_fld(cx_fld(cx_bin(TT_ASSIGN, cx_var(tmp), base), "__m"), sup);
            ref = cx_var(tmp);
        }
    } else {
        tree_t * base = cx_expr(obj, x);
        if (base->t == TT_VAR) {
            ref = cx_var(base->v.sval);
            mrec = cx_fld(base, "__m");
        } else {
            char buf[24];
            snprintf(buf, sizeof buf, "%d", ++x->st->tmp);
            char * tmp = cx_cat("__o", buf, "");
            mrec = cx_fld(cx_bin(TT_ASSIGN, cx_var(tmp), base), "__m");
            ref = cx_var(tmp);
        }
    }
    tree_t * out = ast_node_new(TT_FNC);
    ast_push(out, cx_fld(mrec, m));
    ast_push(out, ref);
    for (int i = 1; i < n->n; i++) if (n->c[i]) ast_push(out, cx_expr(n->c[i], x));
    else ast_push(out, NULL);
    out->line = n->line;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * cx_expr(tree_t * n, cx_ctx * x) {
    if (!n) return n;
    if (n->t == TT_VAR && n->v.sval) {
        const char * nm = n->v.sval;
        if (nm[0] == ':' && nm[1] == ':') { tree_t * g = cx_var(nm + 2); g->line = n->line; return g; }
        if (x->cls && nm[0] != '&' && !cx_has(&x->shadow, nm) && cx_has(&x->cls->fields, nm)) { tree_t * f = cx_fld(cx_var("self"), nm); f->line = n->line; return f; }
        return n;
    }
    if (n->t == TT_FNC && n->n > 0 && n->c[0]) {
        tree_t * f = n->c[0];
        if (f->t == TT_VAR && f->v.sval && cx_is_method(x, f->v.sval)) {
            tree_t * out = ast_node_new(TT_FNC);
            ast_push(out, cx_fld(cx_fld(cx_var("self"), "__m"), f->v.sval));
            ast_push(out, cx_var("self"));
            for (int i = 1; i < n->n; i++) ast_push(out, n->c[i] ? cx_expr(n->c[i], x) : NULL);
            out->line = n->line;
            return out;
        }
        if (f->t == TT_FIELD && f->n > 1 && f->c[1] && f->c[1]->t == TT_VAR && f->c[1]->v.sval && cx_has(&x->st->gmeth, f->c[1]->v.sval)) return cx_dispatch(n, x);
    }
    cx_kids(n, x, 0);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * cx_proc(const char * name, tree_t * params, int variadic, tree_t * body) {
    tree_t * vl = ast_node_new(TT_VLIST);
    vl->v.ival = variadic;
    for (int i = 0; i < params->n; i++) ast_push(vl, cx_var(params->c[i]->v.sval));
    tree_t * p = ast_node_new(TT_PROC_DECL);
    p->v.sval = ct_strdup(name);
    p->slen = 0;
    ast_push(p, cx_var(name));
    ast_push(p, vl);
    ast_push(p, body);
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * cx_method_proc(cx_st * st, const cx_class * c, const tree_t * m) {
    tree_t * params = ast_node_new(TT_VLIST);
    ast_push(params, cx_var("self"));
    for (int i = 0; i < m->c[1]->n; i++) ast_push(params, cx_var(m->c[1]->c[i]->v.sval));
    cx_ctx x;
    memset(&x, 0, sizeof x);
    x.st = st;
    x.cls = c;
    for (int i = 0; i < params->n; i++) cx_push_new(&x.shadow, params->c[i]->v.sval);
    cx_locals(m->c[2], &x.shadow);
    tree_t * body = m->c[2];
    cx_kids(body, &x, 0);
    int ini = !strcmp(m->v.sval, "initially");
    if (ini) ast_push(body, cx_un(TT_RETURN, cx_var("self")));
    tree_t * p = cx_proc(cx_cat(c->name, "_", m->v.sval), params, (int) m->c[1]->v.ival, body);
    p->slen = m->slen;
    p->line = m->line;
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * cx_record(const char * name, const cx_sv * fl, int lead) {
    tree_t * r = ast_node_new(TT_RECORD);
    r->v.sval = ct_strdup(name);
    if (lead) { ast_push(r, cx_var("__s")); ast_push(r, cx_var("__m")); }
    for (int i = 0; i < fl->n; i++) ast_push(r, cx_var(fl->v[i]));
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cx_emit_class(cx_st * st, cx_class * c, tree_t * prog) {
    cx_sv mnames = { 0 };
    for (int i = 0; i < c->nmeth; i++) cx_push(&mnames, c->meth[i].name);
    for (int i = 0; i < c->anc.n; i++) cx_push(&mnames, c->anc.v[i]);
    ast_push(prog, cx_stmt(cx_record(cx_cat(c->name, "__state", ""), &c->fields, 1)));
    ast_push(prog, cx_stmt(cx_record(cx_cat(c->name, "__methods", ""), &mnames, 0)));
    tree_t * g = ast_node_new(TT_GLOBAL);
    ast_push(g, cx_var(cx_cat(c->name, "__oprec", "")));
    ast_push(prog, cx_stmt(g));
    tree_t * ib = ast_node_new(TT_PROGRAM);
    for (int s = 0; s < c->supers.n; s++) if (cx_find(st, c->supers.v[s])) ast_push(ib, cx_call(cx_var(cx_cat(c->supers.v[s], "__init", "")), 0, NULL));
    tree_t * ma = ast_node_new(TT_FNC);
    ast_push(ma, cx_var(cx_cat(c->name, "__methods", "")));
    for (int i = 0; i < c->nmeth; i++) ast_push(ma, c->meth[i].proc && c->meth[i].proc->n > 2 ? cx_var(cx_cat(c->meth[i].owner, "_", c->meth[i].name)) : cx_var("&null"));
    for (int i = 0; i < c->anc.n; i++) ast_push(ma, cx_var(cx_cat(c->anc.v[i], "__oprec", "")));
    ast_push(ib, cx_bin(TT_ASSIGN, cx_un(TT_NULL, cx_var(cx_cat(c->name, "__oprec", ""))), ma));
    ast_push(ib, cx_un(TT_RETURN, cx_var("&null")));
    ast_push(prog, cx_stmt(cx_proc(cx_cat(c->name, "__init", ""), ast_node_new(TT_VLIST), 0, ib)));
    const tree_t * ini = c->init;
    cx_sv cparams = { 0 };
    if (ini && ini->c[1]->n > 0) for (int i = 0; i < ini->c[1]->n; i++) cx_push(&cparams, ini->c[1]->c[i]->v.sval);
    else for (int i = 0; i < c->fields.n; i++) cx_push(&cparams, c->fields.v[i]);
    int fields_are_params = !(ini && ini->c[1]->n > 0);
    tree_t * cb = ast_node_new(TT_PROGRAM);
    ast_push(cb, cx_call(cx_var(cx_cat(c->name, "__init", "")), 0, NULL));
    tree_t * ra = ast_node_new(TT_FNC);
    ast_push(ra, cx_var(cx_cat(c->name, "__state", "")));
    ast_push(ra, cx_var("&null"));
    ast_push(ra, cx_var(cx_cat(c->name, "__oprec", "")));
    for (int i = 0; i < c->fields.n; i++) ast_push(ra, fields_are_params ? cx_var(c->fields.v[i]) : cx_var("&null"));
    ast_push(cb, cx_bin(TT_ASSIGN, cx_var("self"), ra));
    ast_push(cb, cx_bin(TT_ASSIGN, cx_fld(cx_var("self"), "__s"), cx_var("self")));
    if (ini) {
        tree_t * ic = ast_node_new(TT_FNC);
        ast_push(ic, cx_fld(cx_fld(cx_var("self"), "__m"), "initially"));
        ast_push(ic, cx_var("self"));
        for (int i = 0; i < ini->c[1]->n; i++) ast_push(ic, cx_var(ini->c[1]->c[i]->v.sval));
        ast_push(cb, cx_bin(TT_ALTERNATE, ic, ast_node_new(TT_PROC_FAIL)));
    }
    ast_push(cb, cx_un(TT_RETURN, cx_var("self")));
    tree_t * cpl = ast_node_new(TT_VLIST);
    for (int i = 0; i < cparams.n; i++) ast_push(cpl, cx_var(cparams.v[i]));
    ast_push(prog, cx_stmt(cx_proc(c->name, cpl, 0, cb)));
    for (int i = 2; i < c->node->n; i++) { const tree_t * m = c->node->c[i]; if (m->n > 2) ast_push(prog, cx_stmt(cx_method_proc(st, c, m))); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void icn_expand_classes(tree_t * prog) {
    if (!prog) return;
    cx_st st;
    memset(&st, 0, sizeof st);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t * d = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        if (!d || d->t != TT_CLASS_DECL) continue;
        if (st.ncls == st.ccls) { st.ccls = st.ccls ? st.ccls * 2 : 8; st.cls = (cx_class *) ct_grow(st.cls, (size_t) st.ccls * sizeof *st.cls); }
        memset(&st.cls[st.ncls], 0, sizeof st.cls[st.ncls]);
        st.cls[st.ncls].node = d;
        st.cls[st.ncls].name = d->v.sval;
        st.ncls++;
    }
    if (!st.ncls) return;
    for (int i = 0; i < st.ncls; i++) cx_build(&st, &st.cls[i]);
    int w = 0;
    for (int i = 0; i < prog->n; i++) {
        tree_t * s = prog->c[i];
        const tree_t * d = (s && s->t == TT_STMT) ? stmt_attr_expr(stmt_attr_find(s, ":subj")) : NULL;
        if (d && d->t == TT_CLASS_DECL) continue;
        if (d && d->t == TT_PROC_DECL && d->n > 2) { cx_ctx x; memset(&x, 0, sizeof x); x.st = &st; cx_locals(d->c[2], &x.shadow); cx_kids(d->c[2], &x, 0); }
        prog->c[w++] = prog->c[i];
    }
    prog->n = w;
    for (int i = 0; i < st.ncls; i++) cx_emit_class(&st, &st.cls[i], prog);
}
