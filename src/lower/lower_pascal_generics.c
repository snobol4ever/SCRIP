#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "ir/ct_arena.h"
#include "ir/ast.h"
#include "lower_pascal_generics.h"
#define PG_IS(x, k) ((x) && (x)->t == (k))
#define PG_KIND(x, s) (PG_IS(x, TT_DECL) && (x)->v.sval && !strcmp((x)->v.sval, (s)))
typedef struct { const char *key; tree_t *node; } pg_inst;
typedef struct { const char *name; tree_t *params; tree_t *body; tree_t *holder; tree_t *parent; int routine; pg_inst *inst; int ninst, cinst; } pg_tmpl;
typedef struct { pg_tmpl *t; int nt, ct; int fatal; } pg_ctx;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pg_clone(const tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t);
    c->v = e->v;
    c->line = e->line;
    c->slen = e->slen;
    if ((e->t == TT_VAR || e->t == TT_QLIT) && e->v.sval) c->v.sval = ct_strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) ast_push(c, pg_clone(e->c[i]));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pg_var(const char *name) { tree_t *v = ast_node_new(TT_VAR); v->v.sval = ct_strdup(name); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pg_tmpl *pg_find(pg_ctx *x, const char *name) { for (int i = 0; name && i < x->nt; i++) if (!strcasecmp(x->t[i].name, name)) return &x->t[i]; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_add(pg_ctx *x, const char *name, tree_t *params, tree_t *body, tree_t *holder, tree_t *parent, int routine) {
    if (x->nt >= x->ct) { x->ct = x->ct ? x->ct * 2 : 16; x->t = ct_grow(x->t, (size_t)x->ct * sizeof *x->t); }
    pg_tmpl *t = &x->t[x->nt++];
    memset(t, 0, sizeof *t);
    t->name = ct_strdup(name);
    t->params = params;
    t->body = body;
    t->holder = holder;
    t->parent = parent;
    t->routine = routine;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_collect(pg_ctx *x, tree_t *p) {
    if (!p) return;
    for (int i = 0; i < p->n; i++) {
        tree_t *c = p->c[i];
        if (PG_KIND(c, "generic") && c->n >= 2 && (PG_IS(c->c[1], TT_PROCEDURE) || PG_IS(c->c[1], TT_FUNCTION)) && c->c[1]->n > 0 && c->c[1]->c[0]->v.sval) {
            pg_add(x, c->c[1]->c[0]->v.sval, c->c[0], c->c[1], c, p, 1);
            continue;
        }
        if (PG_IS(c, TT_PART) && c->v.sval && !strcmp(c->v.sval, "type")) {
            for (int j = 0; j < c->n; j++) { tree_t *d = c->c[j]; if (PG_KIND(d, "gtype") && d->n >= 3 && d->c[0]->v.sval) pg_add(x, d->c[0]->v.sval, d->c[2], d->c[1], d, c, 0); }
        }
        if (!PG_KIND(c, "generic") && !PG_KIND(c, "gtype")) pg_collect(x, c);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_key(const tree_t *e, char **buf, size_t *len, size_t *cap) {
    char tmp[64];
    const char *s;
    if (!e) return;
    if (PG_IS(e, TT_VAR) && e->v.sval) s = e->v.sval;
    else { snprintf(tmp, sizeof tmp, "t%d", (int)e->t); s = tmp; }
    size_t l = strlen(s) + 4;
    if (*len + l >= *cap) { *cap = (*cap + l) * 2; *buf = ct_grow(*buf, *cap); }
    *len += (size_t)snprintf(*buf + *len, *cap - *len, "%s%s", *len ? "__" : "", s);
    if (!PG_IS(e, TT_VAR)) for (int i = 0; i < e->n; i++) pg_key(e->c[i], buf, len, cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pg_mangle(const char *name, const tree_t *args) {
    char *buf = NULL;
    size_t len = 0, cap = 0;
    pg_key(args, &buf, &len, &cap);
    size_t n = strlen(name) + len + 8;
    char *out = ct_alloc(n);
    snprintf(out, n, "%s__%s", name, buf ? buf : "");
    for (char *q = out; *q; q++) *q = isalnum((unsigned char)*q) ? (char)tolower((unsigned char)*q) : '_';
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_subst(tree_t *e, const tree_t *params, const tree_t *args) {
    if (!e) return;
    for (int i = 0; i < e->n; i++) {
        tree_t *c = e->c[i];
        if (PG_IS(e, TT_FIELD) && i == 1) continue;
        if (PG_IS(c, TT_VAR) && c->v.sval) {
            int hit = -1;
            for (int k = 0; k < params->n && k < args->n; k++) if (params->c[k]->v.sval && !strcasecmp(params->c[k]->v.sval, c->v.sval)) hit = k;
            if (hit >= 0) { e->c[i] = pg_clone(args->c[hit]); continue; }
        }
        pg_subst(c, params, args);
    }
}
static void pg_resolve(pg_ctx *x, tree_t *e);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pg_instantiate(pg_ctx *x, tree_t *spec) {
    tree_t *nm = spec->n > 0 ? spec->c[0] : NULL, *args = spec->n > 1 ? spec->c[1] : NULL;
    pg_tmpl *t = nm && nm->v.sval ? pg_find(x, nm->v.sval) : NULL;
    if (!t || !args) {
        fprintf(stderr, "pascal: specialize of '%s': no generic of that name\n", nm && nm->v.sval ? nm->v.sval : "?");
        x->fatal = 1;
        return pg_var(nm && nm->v.sval ? nm->v.sval : "?");
    }
    pg_resolve(x, args);
    if (t->params->n != args->n) { fprintf(stderr, "pascal: specialize of '%s': %d type argument(s) given, %d expected\n", t->name, args->n, t->params->n); x->fatal = 1; return pg_var(t->name); }
    char *mn = pg_mangle(t->name, args);
    for (int i = 0; i < t->ninst; i++) if (!strcmp(t->inst[i].key, mn)) return pg_var(mn);
    if (t->ninst >= t->cinst) { t->cinst = t->cinst ? t->cinst * 2 : 4; t->inst = ct_grow(t->inst, (size_t)t->cinst * sizeof *t->inst); }
    tree_t *body = pg_clone(t->body), *node;
    pg_subst(body, t->params, args);
    if (t->routine) { body->c[0] = pg_var(mn); node = body; } else { node = ast_node_new(TT_DECL); node->v.sval = ct_strdup("type"); ast_push(node, pg_var(mn)); ast_push(node, body); }
    t->inst[t->ninst].key = mn;
    t->inst[t->ninst].node = node;
    t->ninst++;
    pg_resolve(x, node);
    return pg_var(mn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_resolve(pg_ctx *x, tree_t *e) {
    if (!e || PG_KIND(e, "generic") || PG_KIND(e, "gtype")) return;
    for (int i = 0; i < e->n; i++) { tree_t *c = e->c[i]; if (PG_KIND(c, "specialize")) { e->c[i] = pg_instantiate(x, c); continue; } pg_resolve(x, c); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_splice(pg_tmpl *t) {
    tree_t *p = t->parent;
    int at = -1, old = p->n, extra = t->ninst - 1;
    for (int i = 0; i < old; i++) if (p->c[i] == t->holder) at = i;
    if (at < 0) return;
    if (extra < 0) { for (int i = at; i + 1 < old; i++) p->c[i] = p->c[i + 1]; p->n = old - 1; return; }
    for (int k = 0; k < extra; k++) ast_push(p, NULL);
    for (int i = old - 1; i > at; i--) p->c[i + extra] = p->c[i];
    for (int k = 0; k < t->ninst; k++) p->c[at + k] = t->inst[k].node;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *pas_generics_prepass(tree_t *prog) {
    pg_ctx x;
    memset(&x, 0, sizeof x);
    pg_collect(&x, prog);
    if (!x.nt) return prog;
    pg_resolve(&x, prog);
    for (int i = 0; i < x.nt; i++) pg_splice(&x.t[i]);
    return x.fatal ? NULL : prog;
}
