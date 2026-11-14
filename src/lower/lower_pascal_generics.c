#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "ir/ct_arena.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
#include "lower_pascal_generics.h"
#define PG_IS(x, k) ((x) && (x)->t == (k))
#define PG_KIND(x, s) (PG_IS(x, TT_DECL) && (x)->v.sval && !strcmp((x)->v.sval, (s)))
#define PG_TYPE 0
#define PG_ROUTINE 1
#define PG_METHOD 2
typedef struct { const char *key; tree_t *node; int seq; } pg_inst;
typedef struct { const char *name; tree_t *params; tree_t *body; tree_t *holder; tree_t *parent; int kind; int owner; pg_inst *inst; int ninst, cinst; } pg_tmpl;
typedef struct { pg_tmpl *t; int nt, ct; int fatal; int seq; } pg_ctx;
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
static int pg_find(pg_ctx *x, const char *name) { for (int i = 0; name && i < x->nt; i++) if (x->t[i].kind != PG_METHOD && !strcasecmp(x->t[i].name, name)) return i; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pg_tmpl *pg_add(pg_ctx *x, const char *name, tree_t *params, tree_t *body, tree_t *holder, tree_t *parent, int kind, int owner) {
    if (x->nt >= x->ct) { x->ct = x->ct ? x->ct * 2 : 16; x->t = ct_grow(x->t, (size_t)x->ct * sizeof *x->t); }
    pg_tmpl *t = &x->t[x->nt++];
    memset(t, 0, sizeof *t);
    t->name = ct_strdup(name);
    t->params = params;
    t->body = body;
    t->holder = holder;
    t->parent = parent;
    t->kind = kind;
    t->owner = owner;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pg_is_routine(const tree_t *c) { return PG_IS(c, TT_PROCEDURE) || PG_IS(c, TT_FUNCTION); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_collect(pg_ctx *x, tree_t *p) {
    if (!p) return;
    for (int i = 0; i < p->n; i++) {
        tree_t *c = p->c[i];
        if (PG_KIND(c, "generic") && c->n >= 2 && pg_is_routine(c->c[1]) && c->c[1]->n > 0 && c->c[1]->c[0]->v.sval) {
            pg_add(x, c->c[1]->c[0]->v.sval, c->c[0], c->c[1], c, p, PG_ROUTINE, -1);
            continue;
        }
        if (PG_IS(c, TT_PART) && c->v.sval && !strcmp(c->v.sval, "type")) {
            for (int j = 0; j < c->n; j++) { tree_t *d = c->c[j]; if (PG_KIND(d, "gtype") && d->n >= 3 && d->c[0]->v.sval) pg_add(x, d->c[0]->v.sval, d->c[2], d->c[1], d, c, PG_TYPE, -1); }
        }
        if (!PG_KIND(c, "generic") && !PG_KIND(c, "gtype")) pg_collect(x, c);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_collect_methods(pg_ctx *x, tree_t *p) {
    if (!p) return;
    for (int i = 0; i < p->n; i++) {
        tree_t *c = p->c[i];
        if (PG_KIND(c, "generic") || PG_KIND(c, "gtype")) continue;
        if (pg_is_routine(c) && c->n > 0 && PG_IS(c->c[0], TT_FIELD) && c->c[0]->n == 2 && PG_IS(c->c[0]->c[0], TT_VAR) && c->c[0]->c[0]->v.sval) {
            int o = pg_find(x, c->c[0]->c[0]->v.sval);
            if (o >= 0 && x->t[o].kind == PG_TYPE) { pg_add(x, c->c[0]->c[0]->v.sval, x->t[o].params, c, c, p, PG_METHOD, o); continue; }
        }
        pg_collect_methods(x, c);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pg_is_template_node(pg_ctx *x, const tree_t *e) { for (int i = 0; i < x->nt; i++) if (x->t[i].holder == e) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_key(const tree_t *e, char **buf, size_t *len, size_t *cap) {
    char tmp[64];
    const char *s;
    if (!e) return;
    if (PG_IS(e, TT_VAR) && e->v.sval) s = e->v.sval;
    else { snprintf(tmp, sizeof tmp, "t%d", (int)e->t); s = tmp; }
    size_t l = strlen(s) + 4;
    if (*len + l >= *cap) { *cap = (*cap + l) * 2; *buf = ct_grow(*buf, *cap); }
    *len += (size_t)snprintf(*buf + *len, *cap - *len, "%s%s", *len ? "_" : "", s);
    if (!PG_IS(e, TT_VAR)) for (int i = 0; i < e->n; i++) pg_key(e->c[i], buf, len, cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pg_mangle(const char *name, const tree_t *args) {
    char *buf = NULL;
    size_t len = 0, cap = 0;
    for (int i = 0; i < args->n; i++) pg_key(args->c[i], &buf, &len, &cap);
    size_t n = strlen(name) + len + 8;
    char *out = ct_alloc(n);
    snprintf(out, n, "%s_g_%s", name, buf ? buf : "");
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
static void pg_sh_rename(tree_t *e, const char *sfx) {
    if (!e) return;
    if (PG_IS(e, TT_VAR) && e->v.sval && !strncmp(e->v.sval, "__sh_", 5)) {
        size_t n = strlen(e->v.sval) + strlen(sfx) + 2;
        char *r = ct_alloc(n);
        snprintf(r, n, "%s_%s", e->v.sval, sfx);
        e->v.sval = r;
    }
    for (int i = 0; i < e->n; i++) pg_sh_rename(e->c[i], sfx);
}
static void pg_self(tree_t *e, const char *name, const char *mn) {
    if (!e || PG_KIND(e, "specialize")) return;
    for (int i = 0; i < e->n; i++) {
        tree_t *c = e->c[i];
        if (PG_IS(e, TT_FIELD) && i == 1) continue;
        if (PG_IS(c, TT_VAR) && c->v.sval && !strcasecmp(c->v.sval, name)) { e->c[i] = pg_var(mn); continue; }
        pg_self(c, name, mn);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_resolve(pg_ctx *x, tree_t *e);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pg_seen(const pg_tmpl *t, const char *mn) { for (int i = 0; i < t->ninst; i++) if (!strcmp(t->inst[i].key, mn)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pg_record(pg_tmpl *t, const char *mn, tree_t *node) {
    if (t->ninst >= t->cinst) { t->cinst = t->cinst ? t->cinst * 2 : 4; t->inst = ct_grow(t->inst, (size_t)t->cinst * sizeof *t->inst); }
    t->inst[t->ninst].key = mn;
    t->inst[t->ninst].node = node;
    t->inst[t->ninst].seq = -1;
    t->ninst++;
    return node;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pg_instantiate(pg_ctx *x, tree_t *spec) {
    tree_t *nm = spec->n > 0 ? spec->c[0] : NULL, *args = spec->n > 1 ? spec->c[1] : NULL;
    int ti = nm && nm->v.sval ? pg_find(x, nm->v.sval) : -1;
    if (ti < 0 || !args) {
        fprintf(stderr, "pascal: specialize of '%s': no generic of that name\n", nm && nm->v.sval ? nm->v.sval : "?");
        x->fatal = 1;
        return pg_var(nm && nm->v.sval ? nm->v.sval : "?");
    }
    pg_resolve(x, args);
    pg_tmpl *t = &x->t[ti];
    if (t->params->n != args->n) { fprintf(stderr, "pascal: specialize of '%s': %d type argument(s) given, %d expected\n", t->name, args->n, t->params->n); x->fatal = 1; return pg_var(t->name); }
    char *mn = pg_mangle(t->name, args);
    if (pg_seen(t, mn)) return pg_var(mn);
    tree_t *body = pg_clone(t->body), *node;
    pg_subst(body, t->params, args);
    if (t->kind == PG_TYPE) pg_self(body, t->name, mn);
    pg_sh_rename(body, mn);
    if (t->kind == PG_ROUTINE) { body->c[0] = pg_var(mn); node = body; } else { node = ast_node_new(TT_DECL); node->v.sval = ct_strdup("type"); ast_push(node, pg_var(mn)); ast_push(node, body); }
    pg_record(t, mn, node);
    pg_resolve(x, node);
    t->inst[t->ninst - 1].seq = ++x->seq;
    for (int i = 0; t->kind == PG_TYPE && i < x->nt; i++) {
        pg_tmpl *m = &x->t[i];
        if (m->kind != PG_METHOD || m->owner != ti) continue;
        tree_t *mb = pg_clone(m->body);
        mb->c[0]->c[0] = pg_var(mn);
        pg_subst(mb, t->params, args);
        pg_self(mb, t->name, mn);
        pg_sh_rename(mb, mn);
        pg_record(m, mn, mb);
        pg_resolve(x, mb);
    }
    return pg_var(mn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_resolve(pg_ctx *x, tree_t *e) {
    if (!e || PG_KIND(e, "generic") || PG_KIND(e, "gtype")) return;
    for (int i = 0; i < e->n; i++) { tree_t *c = e->c[i]; if (PG_KIND(c, "specialize")) { e->c[i] = pg_instantiate(x, c); continue; } if (pg_is_template_node(x, c)) continue; pg_resolve(x, c); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_splice(pg_tmpl *t) {
    tree_t *p = t->parent;
    int at = -1, old = p->n, extra = t->kind == PG_TYPE ? -1 : t->ninst - 1;
    for (int i = 0; i < old; i++) if (p->c[i] == t->holder) at = i;
    if (at < 0) return;
    if (extra < 0) { for (int i = at; i + 1 < old; i++) p->c[i] = p->c[i + 1]; p->n = old - 1; return; }
    for (int k = 0; k < extra; k++) ast_push(p, NULL);
    for (int i = old - 1; i > at; i--) p->c[i + extra] = p->c[i];
    for (int k = 0; k < t->ninst; k++) p->c[at + k] = t->inst[k].node;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pg_program_block(tree_t *prog) {
    if (PG_IS(prog, TT_BLOCK)) return prog;
    for (int i = 0; PG_IS(prog, TT_PROGRAM) && i < prog->n; i++) if (PG_IS(prog->c[i], TT_BLOCK)) return prog->c[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pg_has_name(const tree_t *e, const char *name) {
    if (!e || !name) return 0;
    if (PG_IS(e, TT_VAR) && e->v.sval && !strcasecmp(e->v.sval, name)) return 1;
    for (int i = 0; i < e->n; i++) if (pg_has_name(e->c[i], name)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pg_mentions(const pg_ctx *x, const tree_t *e) {
    if (!e) return 0;
    if (PG_IS(e, TT_VAR) && e->v.sval) for (int i = 0; i < x->nt; i++) if (x->t[i].kind == PG_TYPE) for (int k = 0; k < x->t[i].ninst; k++) if (!strcmp(x->t[i].inst[k].key, e->v.sval)) return 1;
    for (int i = 0; i < e->n; i++) if (pg_mentions(x, e->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_insert_decl(tree_t *part, tree_t *node, const char *key) {
    int first = -1, dep = -1;
    for (int i = 0; i < part->n; i++) {
        tree_t *d = part->c[i];
        const tree_t *dn = PG_IS(d, TT_DECL) && d->n >= 2 ? d->c[0] : NULL;
        if (first < 0 && pg_has_name(d, key)) first = i;
        if (dn && dn->v.sval && !PG_IS(d->c[1], TT_CLASS_DECL) && pg_has_name(node->c[1], dn->v.sval)) dep = i;
    }
    int pos = first >= 0 ? first : part->n;
    if (dep >= pos) pos = dep + 1;
    ast_push(part, NULL);
    for (int i = part->n - 1; i > pos; i--) part->c[i] = part->c[i - 1];
    part->c[pos] = node;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pg_place_types(pg_ctx *x, tree_t *prog) {
    tree_t *blk = pg_program_block(prog), *part = NULL;
    int total = 0, at = 0;
    if (!blk) return;
    for (int s = 1; s <= x->seq; s++) for (int i = 0; i < x->nt; i++) for (int k = 0; x->t[i].kind == PG_TYPE && k < x->t[i].ninst; k++) if (x->t[i].inst[k].seq == s) total++;
    if (!total) return;
    for (int i = 0; i < blk->n; i++) if (PG_IS(blk->c[i], TT_PART) && blk->c[i]->v.sval && !strcmp(blk->c[i]->v.sval, "type")) part = blk->c[i];
    if (!part) {
        part = ast_node_new(TT_PART);
        part->v.sval = ct_strdup("type");
        for (int i = 0; i < blk->n; i++) if (PG_IS(blk->c[i], TT_PART) && blk->c[i]->v.sval && (!strcmp(blk->c[i]->v.sval, "const") || !strcmp(blk->c[i]->v.sval, "label"))) at = i + 1;
        for (int i = 0; i < at; i++) if (pg_mentions(x, blk->c[i])) { at = i; break; }
        for (int i = at; i < blk->n; i++) if (pg_mentions(x, blk->c[i])) { at = at < i ? at : i; break; }
        ast_push(blk, NULL);
        for (int i = blk->n - 1; i > at; i--) blk->c[i] = blk->c[i - 1];
        blk->c[at] = part;
    }
    for (int s = 1; s <= x->seq; s++) for (int i = 0; i < x->nt; i++) for (int k = 0; x->t[i].kind == PG_TYPE && k < x->t[i].ninst; k++) if (x->t[i].inst[k].seq == s)
        pg_insert_decl(part, x->t[i].inst[k].node, x->t[i].inst[k].key);
}
static int pg_is_method_inst(const pg_ctx *x, const tree_t *n) {
    for (int i = 0; i < x->nt; i++) for (int k = 0; x->t[i].kind == PG_METHOD && k < x->t[i].ninst; k++) if (x->t[i].inst[k].node == n) return 1;
    return 0;
}
static void pg_move_methods(pg_ctx *x, tree_t *prog) {
    tree_t *blk = pg_program_block(prog), **tmp;
    int last = -1, w = 0, m = 0;
    if (!blk) return;
    for (int i = 0; i < blk->n; i++) if (PG_IS(blk->c[i], TT_PART) && blk->c[i]->v.sval && !strcmp(blk->c[i]->v.sval, "type")) last = i;
    if (last < 0) return;
    tmp = ct_zalloc(last + 1, sizeof *tmp);
    for (int j = 0; j < last; j++) { if (pg_is_method_inst(x, blk->c[j])) tmp[m++] = blk->c[j]; else blk->c[w++] = blk->c[j]; }
    blk->c[w++] = blk->c[last];
    for (int j = 0; j < m; j++) blk->c[w++] = tmp[j];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *pas_generics_prepass(tree_t *prog) {
    pg_ctx x;
    memset(&x, 0, sizeof x);
    pg_collect(&x, prog);
    if (!x.nt) return prog;
    pg_collect_methods(&x, prog);
    pg_resolve(&x, prog);
    for (int i = 0; i < x.nt; i++) pg_splice(&x.t[i]);
    pg_place_types(&x, prog);
    pg_move_methods(&x, prog);
    return x.fatal ? NULL : prog;
}
