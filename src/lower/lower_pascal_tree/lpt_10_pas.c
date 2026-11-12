#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include "ir/ast.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_10_pas.h"
static int pas_view_kind(const pty_t *t, long long *w) {
    if (!t) return -1;
    *w = pty_sizeof_flat(t);
    if (*w != 1 && *w != 2 && *w != 4 && *w != 8) return -1;
    if (t->k == PTK_INT || t->k == PTK_ENUM) return t->lo < 0 ? 4 : 0;
    if (t->k == PTK_CHAR) return 1;
    if (t->k == PTK_BOOL) return t->wbool ? 4 : 2;
    if (t->k == PTK_REAL) return *w == 4 || *w == 8 ? 3 : -1;
    return -1;
}
tree_t *pas_pun_view(tree_t *sel, tree_t *rhs) {
    if (!sel || !rhs || rhs->t != TT_FNC || rhs->n < 2 || !rhs->c[0] || !rhs->c[0]->v.sval || !rhs->c[1]) return rhs;
    int av = !strcmp(rhs->c[0]->v.sval, "__pas_addr_var"), ae = !strcmp(rhs->c[0]->v.sval, "__pas_addr");
    const pty_t *pt = (av || ae) ? pty_of_low(sel) : NULL;
    if (!pt || pt->k != PTK_POINTER || !pt->base || pty_is_pchar(pt)) return rhs;
    const pty_t *bt = pty_of_low(rhs->c[1]);
    if (!bt && rhs->c[1]->t == TT_VAR && rhs->c[1]->v.sval) bt = pty_name_type(rhs->c[1]->v.sval);
    if (!bt || bt->k == PTK_UNKNOWN) return rhs;
    const pty_t *P = pt->base, *e = bt;
    tree_t *addr = rhs;
    if (bt->k == PTK_ARRAY) {
        if (P == bt || P->k == PTK_ARRAY || bt->dyn || bt->open || bt->nix != 1 || !bt->ix || !bt->ix[0] || !bt->ix[0]->bounded || !bt->base) return rhs;
        e = bt->base;
        if (av) addr = mk_fnc2("__pas_addr", rhs->c[1], ilit(bt->ix[0]->lo));
    } else if (ae) return rhs;
    if (P == e) return addr;
    long long w = 0, W = 0;
    int ek = pas_view_kind(e, &w), pk = pas_view_kind(P, &W);
    if (ek < 0 || pk < 0) {
        if (P->k == PTK_UNKNOWN || e->k == PTK_UNKNOWN || (P->name && e->name && !strcmp(P->name, e->name))) return addr;
        fprintf(stderr, "pascal: a pointer to %s cannot view the storage of %s: only flat ordinal storage takes a pointer of another width (ARCH-PASCAL-REDESIGN R5 amended (5))\n",
            pty_name_of(P) ? pty_name_of(P) : pty_kind_name(P->k), pty_name_of(e) ? pty_name_of(e) : pty_kind_name(e->k));
        g_lower_pas.sem.pas_iso_errors++;
        return rhs;
    }
    if (w == W && ek == pk) return addr;
    return mk_fnc2("__pas_pview", addr, ilit(w | (W << 8) | ((long long)((ek & 3) | ((ek >> 2) << 2) | ((pk & 3) << 3) | ((pk >> 2) << 5)) << 16)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_castname(const tree_t *t, const char *n) {
    static const char *const B[] = { "pointer", "integer", "longint", "int64", "byte", "word", "cardinal", "longword", "shortint", "smallint", "char", "boolean", "qword", "dword", "ptruint", "ptrint",
        "nativeint", "nativeuint", "pchar", 0 };
    for (int i = 0; n && B[i]; i++) if (!strcmp(n, B[i])) return 1;
    for (int i = 0; t && i < t->n; i++) {
        const tree_t *c = t->c[i];
        if (IS(c, TT_DECL) && c->v.sval && !strcmp(c->v.sval, "type") && c->n >= 1 && IS(c->c[0], TT_VAR) && c->c[0]->v.sval && n && !strcmp(c->c[0]->v.sval, n)) return 1;
        if (c && c->n && pas_castname(c, n)) return 1;
    }
    return 0;
}
static int pas_declared_type(const tree_t *t, const char *n) {
    for (int i = 0; t && n && i < t->n; i++) {
        const tree_t *c = t->c[i];
        if (IS(c, TT_DECL) && c->v.sval && !strcmp(c->v.sval, "type") && c->n >= 1 && IS(c->c[0], TT_VAR) && c->c[0]->v.sval && !strcmp(c->c[0]->v.sval, n)) return 1;
        if (c && c->n && pas_declared_type(c, n)) return 1;
    }
    return 0;
}
static int pas_var_of_type(const tree_t *t, const char *vn, const char *tn) {
    for (int i = 0; t && vn && i < t->n; i++) {
        const tree_t *c = t->c[i];
        if (IS(c, TT_DECL) && c->v.sval && !strcmp(c->v.sval, "var") && c->n >= 2 && IS(c->c[1], TT_VAR) && c->c[1]->v.sval && !strcasecmp(c->c[1]->v.sval, tn)) {
            const tree_t *ids = c->c[0];
            if (IS(ids, TT_VLIST)) {
                for (int q = 0; q < ids->n; q++) if (IS(ids->c[q], TT_VAR) && ids->c[q]->v.sval && !strcmp(ids->c[q]->v.sval, vn)) return 1;
            } else if (IS(ids, TT_VAR) && ids->v.sval && !strcmp(ids->v.sval, vn)) return 1;
        }
        if (c && c->n && pas_var_of_type(c, vn, tn)) return 1;
    }
    return 0;
}
static void pas_add_hidden_var(tree_t *blk, const char *name) {
    tree_t *part = NULL;
    for (int i = 0; blk && i < blk->n - 1 && !part; i++) if (IS(blk->c[i], TT_PART) && blk->c[i]->v.sval && !strcmp(blk->c[i]->v.sval, "var")) part = blk->c[i];
    tree_t *d = ast_node_new(TT_DECL), *ids = ast_node_new(TT_VLIST);
    d->v.sval = (char *)"var";
    ast_push(ids, leaf_s(TT_VAR, name));
    ast_push(d, ids);
    ast_push(d, leaf_s(TT_VAR, "integer"));
    if (part) { ast_push(part, d); return; }
    part = ast_node_new(TT_PART);
    part->v.sval = (char *)"var";
    ast_push(part, d);
    int k = blk->n - 1;
    for (int i = 0; i < blk->n - 1; i++) if (IS(blk->c[i], TT_PROCEDURE) || IS(blk->c[i], TT_FUNCTION)) { k = i; break; }
    ast_push(blk, blk->c[blk->n - 1]);
    for (int i = blk->n - 2; i > k; i--) blk->c[i] = blk->c[i - 1];
    blk->c[k] = part;
}
void pas_forin_types(tree_t *root, tree_t *t, tree_t *blk) {
    for (int i = 0; t && i < t->n; i++) pas_forin_types(root, t->c[i], IS(t, TT_BLOCK) ? t : blk);
    if (!IS(t, TT_FOR) || !t->v.sval || strcmp(t->v.sval, "in") || t->n != 3 || !IS(t->c[0], TT_VAR) || !IS(t->c[1], TT_VAR) || !t->c[1]->v.sval) return;
    tree_t *x = t->c[0], *coll = t->c[1], *body = t->c[2];
    if (pas_declared_type(root, coll->v.sval)) {
        tree_t *lo = ast_node_new(TT_FNC), *hi = ast_node_new(TT_FNC);
        ast_push(lo, leaf_s(TT_VAR, "low"));
        ast_push(lo, leaf_s(TT_VAR, coll->v.sval));
        ast_push(lo, ilit(-1));
        ast_push(hi, leaf_s(TT_VAR, "high"));
        ast_push(hi, leaf_s(TT_VAR, coll->v.sval));
        ast_push(hi, ilit(-1));
        t->v.sval = (char *)"to";
        t->c[1] = lo;
        t->c[2] = hi;
        ast_push(t, body);
    } else if (blk && pas_var_of_type(root, coll->v.sval, "tstringlist")) {
        char nm[32];
        snprintf(nm, sizeof nm, "__pas_fi_%d", ++g_lower_pas.sem.pas_catmp_seq);
        char *iv = ct_strdup(nm);
        pas_add_hidden_var(blk, iv);
        tree_t *cnt = ast_node_new(TT_FIELD), *hi = ast_node_new(TT_SUB), *ix = ast_node_new(TT_IDX), *seq = ast_node_new(TT_SEQ_EXPR), *as = ast_node_new(TT_ASSIGN);
        ast_push(cnt, leaf_s(TT_VAR, coll->v.sval));
        ast_push(cnt, leaf_s(TT_VAR, "count"));
        ast_push(hi, cnt);
        ast_push(hi, ilit(1));
        ast_push(ix, leaf_s(TT_VAR, coll->v.sval));
        ast_push(ix, leaf_s(TT_VAR, iv));
        ast_push(as, x);
        ast_push(as, ix);
        ast_push(seq, as);
        ast_push(seq, body);
        t->v.sval = (char *)"to";
        t->c[0] = leaf_s(TT_VAR, iv);
        t->c[1] = ilit(0);
        t->c[2] = hi;
        ast_push(t, seq);
    }
}
void pas_lvalue_casts(tree_t *root, tree_t *t) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t *c = t->c[i];
        if (IS(t, TT_ASSIGN) && i == 0 && IS(c, TT_FNC) && c->n == 2 && IS(c->c[0], TT_VAR) && pas_castname(root, c->c[0]->v.sval)) { t->c[0] = c->c[1]; c = t->c[0]; }
        pas_lvalue_casts(root, c);
    }
}
void pas_call_casts(tree_t *root, tree_t *t) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t *c = t->c[i];
        if (IS(t, TT_FNC) && i == 0 && IS(c, TT_FNC) && c->n == 2 && IS(c->c[0], TT_VAR) && pas_castname(root, c->c[0]->v.sval)) { t->c[0] = c->c[1]; c = t->c[0]; }
        pas_call_casts(root, c);
    }
}
static void pas_rename_var(tree_t *t, const char *from, const char *to) {
    for (int i = 0; t && i < t->n; i++) { tree_t *c = t->c[i]; if (IS(c, TT_VAR) && c->v.sval && !strcmp(c->v.sval, from)) c->v.sval = (char *)to; pas_rename_var(c, from, to); }
}
static int pas_abs_flat_name(const char *n) {
    if (!n) return 0;
    if (!strcmp(n, "byte") || !strcmp(n, "shortint") || !strcmp(n, "char") || !strcmp(n, "boolean") || !strcmp(n, "word") || !strcmp(n, "smallint")) return 1;
    return !strcmp(n, "integer") || !strcmp(n, "longword") || !strcmp(n, "cardinal") || !strcmp(n, "longint") || !strcmp(n, "int64") || !strcmp(n, "qword");
}
static const tree_t *pas_abs_decl_of(const tree_t *blk, const char *part_kind, const char *name) {
    for (int k = 0; blk && name && k < blk->n; k++) {
        const tree_t *part = blk->c[k];
        if (!IS(part, TT_PART) || !part->v.sval || strcmp(part->v.sval, part_kind)) continue;
        for (int j = 0; j < part->n; j++) {
            const tree_t *d = part->c[j], *ids = IS(d, TT_DECL) && d->n >= 2 ? d->c[0] : NULL;
            if (ids && IS(ids, TT_VAR) && ids->v.sval && !strcmp(ids->v.sval, name)) return d->c[1];
            for (int q = 0; ids && IS(ids, TT_VLIST) && q < ids->n; q++) if (IS(ids->c[q], TT_VAR) && ids->c[q]->v.sval && !strcmp(ids->c[q]->v.sval, name)) return d->c[1];
        }
    }
    return NULL;
}
static int pas_abs_flat_type(const tree_t *blk, const tree_t *ty, int left) {
    if (!ty || left < 0) return 0;
    if (IS(ty, TT_SUBRANGE)) return 1;
    if (IS(ty, TT_ARRAY_TYPE)) return ty->n == 2 && IS(ty->c[0], TT_SUBRANGE) && pas_abs_flat_type(blk, ty->c[1], left - 1) == 1 ? 2 : 0;
    if (!IS(ty, TT_VAR) || !ty->v.sval) return 0;
    if (pas_abs_flat_name(ty->v.sval)) return 1;
    return pas_abs_flat_type(blk, pas_abs_decl_of(blk, "type", ty->v.sval), left - 1);
}
static void pas_abs_deref_uses(tree_t *t, const char *name, const tree_t *skip) {
    for (int i = 0; t && i < t->n; i++) {
        tree_t *c = t->c[i];
        if (c == skip) continue;
        if (IS(c, TT_VAR) && c->v.sval && !strcmp(c->v.sval, name)) t->c[i] = un(TT_DEREF, c);
        else pas_abs_deref_uses(c, name, skip);
    }
}
static int pas_abs_overlay(tree_t *t, tree_t *d, const tree_t *ab) {
    tree_t *ids = d->c[0], *id = IS(ids, TT_VLIST) && ids->n == 1 ? ids->c[0] : IS(ids, TT_VAR) ? ids : NULL, *ty = d->c[1], *body = t->n ? t->c[t->n - 1] : NULL;
    const char *tk = ab->c[0]->v.sval;
    if (!id || !IS(id, TT_VAR) || !id->v.sval || !IS(ty, TT_VAR) || !pas_abs_flat_name(ty->v.sval) || !body || IS(body, TT_PART)) return 0;
    const tree_t *kt = pas_abs_decl_of(t, "var", tk);
    if (!kt || (IS(kt, TT_VAR) && kt->v.sval && !strcmp(kt->v.sval, ty->v.sval)) || !pas_abs_flat_type(t, kt, t->n)) return 0;
    pas_abs_deref_uses(t, id->v.sval, d);
    d->c[1] = un(TT_PTR_TYPE, ty);
    d->n = 2;
    tree_t *init = bin(TT_ASSIGN, leaf_s(TT_VAR, id->v.sval), un(TT_ADDR, leaf_s(TT_VAR, tk)));
    if (!IS(body, TT_SEQ_EXPR)) { tree_t *s = ast_node_new(TT_SEQ_EXPR); ast_push(s, body); t->c[t->n - 1] = body = s; }
    ast_push(body, init);
    for (int q = body->n - 1; q > 0; q--) body->c[q] = body->c[q - 1];
    body->c[0] = init;
    return 1;
}
void pas_absolute_vars(tree_t *t) {
    for (int i = 0; t && i < t->n; i++) pas_absolute_vars(t->c[i]);
    if (!IS(t, TT_BLOCK)) return;
    { int w = 0; for (int k = 0; k < t->n; k++) { tree_t *pt = t->c[k]; if (IS(pt, TT_PART) && pt->v.sval && !strcmp(pt->v.sval, "exports")) continue; t->c[w++] = pt; } t->n = w; }
    for (int k = 0; k < t->n; k++) {
        tree_t *part = t->c[k];
        if (!IS(part, TT_PART) || !part->v.sval || strcmp(part->v.sval, "var")) continue;
        int w = 0;
        for (int j = 0; j < part->n; j++) {
            tree_t *d = part->c[j];
            tree_t *ab = (IS(d, TT_DECL) && d->n >= 3 && IS(d->c[2], TT_DECL) && d->c[2]->v.sval && !strcmp(d->c[2]->v.sval, "absolute") && d->c[2]->n >= 1) ? d->c[2] : NULL;
            if (!ab || !IS(ab->c[0], TT_VAR) || !ab->c[0]->v.sval || pas_abs_overlay(t, d, ab)) { part->c[w++] = d; continue; }
            const tree_t *ids = d->c[0];
            if (IS(ids, TT_VLIST)) {
                for (int q = 0; q < ids->n; q++) if (IS(ids->c[q], TT_VAR) && ids->c[q]->v.sval) pas_rename_var(t, ids->c[q]->v.sval, ab->c[0]->v.sval);
            } else if (IS(ids, TT_VAR) && ids->v.sval) pas_rename_var(t, ids->v.sval, ab->c[0]->v.sval);
        }
        part->n = w;
    }
}
