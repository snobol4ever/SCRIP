#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ct_arena.h"
#include "ast.h"
#include "lower.h"
#include "g_lower.h"
#include "lower_pascal_types.h"
#define PT_IS(x, k) ((x) && (x)->t == (k))
typedef struct psym psym_t;
struct psym { const char *name; int k; pty_t *t; long long iv; int hv; const tree_t *rt; psym_t *next; };
typedef struct pscope pscope_t;
struct pscope { pscope_t *up; psym_t *s; };
typedef struct { const void *key; const pty_t *t; int line, fld; } pty_ent;
typedef struct { pty_ent *e; long n, cap; } pty_map;
struct pty_state {
    pty_map src, low, sstr, scp, prag;
    struct { const char *name; const pty_t *t; } *anon;
    int nanon, canon;
    pscope_t *cur, *ecur;
    int line, fpc;
    pty_t **pend;
    int npend, cpend;
    long *stats;
    pty_t *tint, *tchar, *tbool, *treal, *tsingle, *tstr, *tshort, *ttext, *tnil, *tvoid, *tptr, *tqword, *tfile, *tpchar;
};
static struct pty_state *pt_st(void) { return g_lower.pas.sem.pty; }
static void pt_prag_apply(struct pty_state *s, pty_t *t, const tree_t *n);
static struct pty_state *pt_mk(void) { if (!g_lower.pas.sem.pty) g_lower.pas.sem.pty = (struct pty_state *)ct_zalloc(1, sizeof(struct pty_state)); return g_lower.pas.sem.pty; }
static unsigned long pt_hash(const void *p) { uint64_t x = (uint64_t)(uintptr_t)p; x ^= x >> 17; x *= 0x9E3779B97F4A7C15ULL; return (unsigned long)(x >> 24); }
static void pt_map_put(pty_map *m, const void *k, const pty_t *t, int line) {
    if (!k) return;
    if ((m->n + 1) * 2 > m->cap) {
        long nc = m->cap ? m->cap * 2 : 1024;
        pty_ent *ne = (pty_ent *)ct_zalloc((size_t)nc, sizeof *ne);
        for (long i = 0; i < m->cap; i++) {
            if (!m->e[i].key) continue;
            unsigned long h = pt_hash(m->e[i].key) & (unsigned long)(nc - 1);
            while (ne[h].key) h = (h + 1) & (unsigned long)(nc - 1);
            ne[h] = m->e[i];
        }
        if (m->e) ct_drop(m->e);
        m->e = ne;
        m->cap = nc;
    }
    unsigned long h = pt_hash(k) & (unsigned long)(m->cap - 1);
    while (m->e[h].key && m->e[h].key != k) h = (h + 1) & (unsigned long)(m->cap - 1);
    if (!m->e[h].key) m->n++;
    m->e[h].key = k;
    m->e[h].t = t;
    m->e[h].line = line;
}
static const pty_ent *pt_map_get(const pty_map *m, const void *k) {
    if (!k || !m->cap) return NULL;
    unsigned long h = pt_hash(k) & (unsigned long)(m->cap - 1);
    while (m->e[h].key) { if (m->e[h].key == k) return &m->e[h]; h = (h + 1) & (unsigned long)(m->cap - 1); }
    return NULL;
}
static pty_t *pt_new(pty_kind k, const char *name) { pty_t *t = (pty_t *)ct_zalloc(1, sizeof *t); t->k = k; t->name = name; return t; }
static pty_t *pt_ord(pty_kind k, const char *name, long long lo, long long hi) { pty_t *t = pt_new(k, name); t->lo = lo; t->hi = hi; t->bounded = 1; return t; }
static void pt_add_field(pty_t *r, const char *name, pty_t *t) {
    if (!r || !name) return;
    if (r->nf >= r->cf) { r->cf = r->cf ? r->cf * 2 : 8; r->f = (pty_field_t *)ct_grow(r->f, (size_t)r->cf * sizeof *r->f); }
    r->f[r->nf].name = name;
    r->f[r->nf++].type = t;
}
static void pt_pend(struct pty_state *s, pty_t *t) {
    if (s->npend >= s->cpend) { s->cpend = s->cpend ? s->cpend * 2 : 16; s->pend = (pty_t **)ct_grow(s->pend, (size_t)s->cpend * sizeof *s->pend); }
    s->pend[s->npend++] = t;
}
static psym_t *ps_def(struct pty_state *s, const char *name, int k, pty_t *t) {
    psym_t *p = (psym_t *)ct_zalloc(1, sizeof *p);
    p->name = name;
    p->k = k;
    p->t = t;
    p->next = s->cur->s;
    s->cur->s = p;
    return p;
}
static psym_t *ps_find(struct pty_state *s, const char *name) {
    for (pscope_t *c = s->cur; name && c; c = c->up) for (psym_t *p = c->s; p; p = p->next) if (p->name && !strcmp(p->name, name)) return p;
    return NULL;
}
static psym_t *ps_find_kind(struct pty_state *s, const char *name, int k) {
    for (pscope_t *c = s->cur; name && c; c = c->up) for (psym_t *p = c->s; p; p = p->next) if (p->k == k && p->name && !strcmp(p->name, name)) return p;
    return NULL;
}
static void ps_push(struct pty_state *s) { pscope_t *c = (pscope_t *)ct_zalloc(1, sizeof *c); c->up = s->cur; s->cur = c; }
static void ps_pop(struct pty_state *s) { if (s->cur && s->cur->up) s->cur = s->cur->up; }
static int pt_in(const char *n, const char *const *list) { for (int i = 0; n && list[i]; i++) if (!strcmp(n, list[i])) return 1; return 0; }
static pty_t *pt_builtin(struct pty_state *s, const char *n) {
    static const char *const ints[] = { "integer", "longint", "int64", "nativeint", "ptrint", "sizeint", "int32", "valsint", "hresult", "thandle", "codepointer", 0 };
    static const char *const uints[] = { "cardinal", "longword", "dword", "uint32", "nativeuint", "ptruint", "sizeuint", "valuint", 0 };
    static const char *const chars[] = { "char", "ansichar", "widechar", "unicodechar", 0 };
    static const char *const bools[] = { "boolean", "boolean8", "boolean16", "boolean32", "boolean64", 0 };
    static const char *const wbools[] = { "bytebool", "wordbool", "longbool", 0 };
    static const char *const reals[] = { "real", "double", "extended", "comp", "currency", "real48", "valreal", 0 };
    static const char *const strs[] = { "string", "ansistring", "unicodestring", "widestring", "rawbytestring", "utf8string", "openstring", 0 };
    if (!n) return NULL;
    if (!strcmp(n, "integer")) return s->tint;
    if (pt_in(n, ints)) return pt_ord(PTK_INT, n, -2147483648LL, 2147483647LL);
    if (pt_in(n, uints)) return pt_ord(PTK_INT, n, 0, 4294967295LL);
    if (!strcmp(n, "byte") || !strcmp(n, "uint8")) return pt_ord(PTK_INT, n, 0, 255);
    if (!strcmp(n, "shortint") || !strcmp(n, "int8")) return pt_ord(PTK_INT, n, -128, 127);
    if (!strcmp(n, "word") || !strcmp(n, "uint16")) return pt_ord(PTK_INT, n, 0, 65535);
    if (!strcmp(n, "smallint") || !strcmp(n, "int16")) return pt_ord(PTK_INT, n, -32768, 32767);
    if (!strcmp(n, "qword") || !strcmp(n, "uint64")) return s->tqword;
    if (pt_in(n, chars)) return s->tchar;
    if (pt_in(n, bools)) return s->tbool;
    if (pt_in(n, wbools)) { pty_t *t = pt_ord(PTK_BOOL, n, 0, 1); t->wbool = 1; return t; }
    if (!strcmp(n, "single")) return s->tsingle;
    if (!strcmp(n, "real")) return s->treal;
    if (pt_in(n, reals)) return pt_new(PTK_REAL, n);
    if (pt_in(n, strs)) return s->tstr;
    if (!strcmp(n, "shortstring")) return s->tshort;
    if (!strcmp(n, "text") || !strcmp(n, "textfile")) return s->ttext;
    if (!strcmp(n, "file")) return s->tfile;
    if (!strcmp(n, "pointer")) return s->tptr;
    if (!strcmp(n, "pchar") || !strcmp(n, "pansichar") || !strcmp(n, "pwidechar")) return s->tpchar;
    return NULL;
}
static pty_t *pt_named(struct pty_state *s, const char *n) { psym_t *p = ps_find_kind(s, n, 't'); return p ? p->t : pt_builtin(s, n); }
static pty_t *pt_expr(struct pty_state *s, const tree_t *n);
static const pty_t *pt_of_src_mut(struct pty_state *s, const tree_t *n);
static int pt_const(struct pty_state *s, const tree_t *n, long long *v) {
    long long a, b;
    if (!n) return 0;
    switch (n->t) {
        case TT_ILIT:
        case TT_CHRLIT:
        *v = n->v.ival;
        return 1;
        case TT_QLIT:
        if (n->v.sval && strlen(n->v.sval) == 1) { *v = (unsigned char)n->v.sval[0]; return 1; }
        return 0;
        case TT_VAR:
        {
            if (!n->v.sval) return 0;
            psym_t *p = ps_find(s, n->v.sval);
            if (p && (p->k == 'c' || p->k == 'e') && p->hv) { *v = p->iv; return 1; }
            if (p) return 0;
            if (!strcmp(n->v.sval, "true")) { *v = 1; return 1; }
            if (!strcmp(n->v.sval, "false")) { *v = 0; return 1; }
            if (!strcmp(n->v.sval, "maxint")) { *v = 2147483647; return 1; }
            return 0;
        }
        case TT_PLS:
        return n->n == 1 && pt_const(s, n->c[0], v);
        case TT_MNS:
        if (n->n == 1 && pt_const(s, n->c[0], &a)) { *v = -a; return 1; }
        return 0;
        case TT_ADD:
        case TT_SUB:
        case TT_MUL:
        case TT_IDIV:
        case TT_MOD:
        if (n->n != 2 || !pt_const(s, n->c[0], &a) || !pt_const(s, n->c[1], &b)) return 0;
        if (n->t == TT_ADD) *v = a + b;
        else if (n->t == TT_SUB) *v = a - b;
        else if (n->t == TT_MUL) *v = a * b;
        else if (!b) return 0;
        else *v = n->t == TT_IDIV ? a / b : a % b;
        return 1;
        case TT_FNC:
        {
            const char *f = (n->n == 2 && PT_IS(n->c[0], TT_VAR)) ? n->c[0]->v.sval : NULL;
            if (!f || !pt_const(s, n->c[1], &a)) return 0;
            if (!strcmp(f, "ord") || !strcmp(f, "chr")) { *v = a; return 1; }
            if (!strcmp(f, "succ")) { *v = a + 1; return 1; }
            if (!strcmp(f, "pred")) { *v = a - 1; return 1; }
            return 0;
        }
        default:
        return 0;
    }
}
static pty_t *pt_type(struct pty_state *s, const tree_t *n);
static const char *pt_spelling(const tree_t *x) {
    for (int i = 0; x && i < x->n; i++) if (PT_IS(x->c[i], TT_ATTR) && x->c[i]->v.sval && !strcmp(x->c[i]->v.sval, ":spelling") && x->c[i]->n > 0 && x->c[i]->c[0]->v.sval) return x->c[i]->c[0]
        ->v.sval;
    return x ? x->v.sval : NULL;
}
static pty_t *pt_strbound(long long len);
static int pt_leaf_ansi(const tree_t *n) {
    for (int i = 0; i < n->n; i++) if (PT_IS(n->c[i], TT_ATTR) && n->c[i]->n && PT_IS(n->c[i]->c[0], TT_QLIT) && n->c[i]->c[0]->v.sval && !strcmp(n->c[i]->c[0]->v.sval, "ansistr")) return 1;
    return 0;
}
static pty_t *pt_leaf(struct pty_state *s, const tree_t *n) {
    if (!n || !n->v.sval) return NULL;
    const pty_ent *ss = pt_map_get(&s->sstr, n);
    long long sn;
    if (ss && ss->t && pt_const(s, (const tree_t *)ss->t->bnd, &sn) && sn >= 1 && sn <= 255) { pty_t *t = pt_new(PTK_STRING, "shortstring"); t->strmax = (int)sn; return t; }
    if (!ss && s->fpc && !strcmp(n->v.sval, "string") && !pt_leaf_ansi(n) && !ps_find_kind(s, "string", 't')) return s->tshort;
    return pt_named(s, n->v.sval);
}
struct pty_lay { int nf, cap; const char **name; const pty_t **ft; long long *ov; unsigned char *named; int nev, evcap; int *ev; };
static void lb_ev(struct pty_lay *L, int code, int arg) {
    if (!L) return;
    if (L->nev >= L->evcap) { L->evcap = L->evcap ? L->evcap * 2 : 16; L->ev = (int *)ct_grow(L->ev, (size_t)L->evcap * sizeof *L->ev); }
    L->ev[L->nev++] = code | (arg << 3);
}
static void lb_compute_ov(struct pty_lay *L);
static void lb_add(struct pty_lay *L, const char *name, const pty_t *ft, int named) {
    if (!L || !name) return;
    if (L->nf >= L->cap) {
        int nc = L->cap ? L->cap * 2 : 16;
        L->name = (const char **)ct_grow(L->name, (size_t)nc * sizeof *L->name);
        L->ft = (const pty_t **)ct_grow(L->ft, (size_t)nc * sizeof *L->ft);
        L->ov = (long long *)ct_grow(L->ov, (size_t)nc * sizeof *L->ov);
        L->named = (unsigned char *)ct_grow(L->named, (size_t)nc);
        L->cap = nc;
    }
    L->name[L->nf] = name;
    L->ft[L->nf] = ft;
    L->ov[L->nf] = (long long)L->nf;
    L->named[L->nf] = (unsigned char)named;
    L->nf++;
    lb_ev(L, 1, 0);
}
static void pt_rec_fields(struct pty_state *s, pty_t *r, const tree_t *body) {
    for (int i = 0; body && i < body->n; i++) {
        const tree_t *c = body->c[i];
        if (PT_IS(c, TT_DECL) && c->v.sval && !strcmp(c->v.sval, "field") && c->n >= 2) {
            pty_t *ft = pt_type(s, c->c[1]);
            const tree_t *ids = c->c[0];
            if (PT_IS(ids, TT_VLIST)) {
                for (int j = 0; j < ids->n; j++) if (ids->c[j]) { pt_add_field(r, ids->c[j]->v.sval, ft); lb_add(r->lay, ids->c[j]->v.sval, ft, PT_IS(c->c[1], TT_VAR)); }
            } else if (PT_IS(ids, TT_VAR)) {
                pt_add_field(r, ids->v.sval, ft);
                lb_add(r->lay, ids->v.sval, ft, PT_IS(c->c[1], TT_VAR));
            }
        } else if (PT_IS(c, TT_CASE)) {
            int k = 0;
            const char *tag = NULL;
            pty_t *tagt = NULL;
            r->var = 1;
            if (c->n >= 2 && PT_IS(c->c[0], TT_VAR) && PT_IS(c->c[1], TT_VAR)) {
                tag = c->c[0]->v.sval;
                tagt = pt_leaf(s, c->c[1]);
                pt_add_field(r, tag, tagt);
                k = 2;
            } else if (c->n >= 1 && PT_IS(c->c[0], TT_VAR)) {
                k = 1;
                tag = c->c[0]->v.sval;
                tagt = pt_leaf(s, c->c[0]);
            }
            lb_ev(r->lay, 2, 0);
            for (int j = k; j < c->n; j++) if (PT_IS(c->c[j], TT_ARM) && c->c[j]->n >= 2) { int st = r->lay->nf; pt_rec_fields(s, r, c->c[j]->c[1]); lb_ev(r->lay, 3, st); }
            lb_ev(r->lay, 4, 0);
            if (tag) lb_add(r->lay, tag, tagt, k == 2 ? 3 : 5);
        } else if (PT_IS(c, TT_FIELDS) || PT_IS(c, TT_VLIST)) pt_rec_fields(s, r, c);
    }
}
static pty_t *pt_type(struct pty_state *s, const tree_t *n) {
    if (!n) return NULL;
    int off = (n->n > 0 && PT_IS(n->c[0], TT_KEYWORD)) ? 1 : 0;
    switch (n->t) {
        case TT_VAR:
        {
            const char *nm = n->v.sval;
            if (!nm) return NULL;
            if (!strncmp(nm, "$open$", 6)) { pty_t *t = pt_new(PTK_ARRAY, NULL); t->open = 1; t->dyn = 1; t->base = pt_named(s, nm + 6); return t; }
            return pt_leaf(s, n);
        }
        case TT_IDX:
        {
            long long v = 255;
            if (n->n != 2 || !PT_IS(n->c[0], TT_VAR) || !n->c[0]->v.sval || strcmp(n->c[0]->v.sval, "string")) return NULL;
            pty_t *t = pt_new(PTK_STRING, "shortstring");
            t->strmax = pt_const(s, n->c[1], &v) ? (int)v : 255;
            return t;
        }
        case TT_ENUM_TYPE:
        {
            pty_t *t = pt_ord(PTK_ENUM, NULL, 0, n->n - 1);
            pt_prag_apply(s, t, n);
            for (int i = 0; i < n->n; i++) {
                if (!n->c[i] || !n->c[i]->v.sval) continue;
                psym_t *p = ps_def(s, n->c[i]->v.sval, 'e', t);
                p->iv = i;
                p->hv = 1;
                pt_add_field(t, pt_spelling(n->c[i]), NULL);
            }
            return t;
        }
        case TT_SUBRANGE:
        {
            long long lo = 0, hi = 0;
            pty_t *lt = n->n >= 1 ? pt_expr(s, n->c[0]) : NULL;
            if (n->n >= 2) pt_expr(s, n->c[1]);
            int ok = n->n >= 2 && pt_const(s, n->c[0], &lo) && pt_const(s, n->c[1], &hi);
            pty_t *t = pt_new(lt ? lt->k : PTK_INT, NULL);
            if (ok) { t->lo = lo; t->hi = hi; t->bounded = 1; }
            if (lt && lt->k == PTK_ENUM) t->base = lt;
            return t;
        }
        case TT_PTR_TYPE:
        {
            pty_t *t = pt_new(PTK_POINTER, NULL);
            const tree_t *tg = n->n ? n->c[0] : NULL;
            if (PT_IS(tg, TT_VAR) && tg->v.sval) {
                int own = 0;
                for (psym_t *q = s->cur ? s->cur->s : NULL; q; q = q->next) if (q->k == 't' && q->name && !strcmp(q->name, tg->v.sval)) own = 1;
                t->base = !own && ps_find_kind(s, tg->v.sval, 't') ? NULL : pt_leaf(s, tg);
                if (!t->base) { t->fwd = tg->v.sval; pt_pend(s, t); }
            } else t->base = pt_type(s, tg);
            return t;
        }
        case TT_ARRAY_TYPE:
        {
            int ni = n->n - off - 1;
            pty_t *t = pt_new(PTK_ARRAY, NULL);
            t->packed = off;
            if (ni <= 0) t->dyn = 1;
            else { t->nix = ni; t->ix = (pty_t **)ct_zalloc((size_t)ni, sizeof *t->ix); for (int i = 0; i < ni; i++) t->ix[i] = pt_type(s, n->c[off + i]); }
            t->base = n->n > off ? pt_type(s, n->c[n->n - 1]) : NULL;
            return t;
        }
        case TT_RECORD:
        {
            pty_t *t = pt_new(PTK_RECORD, NULL);
            pt_prag_apply(s, t, n);
            t->packed = off;
            t->lay = (struct pty_lay *)ct_zalloc(1, sizeof *t->lay);
            for (int i = off; i < n->n; i++) pt_rec_fields(s, t, n->c[i]);
            lb_compute_ov(t->lay);
            pt_map_put(&s->src, n, t, s->line);
            return t;
        }
        case TT_SET_TYPE:
        { pty_t *t = pt_new(PTK_SET, NULL); t->packed = off; t->base = n->n > off ? pt_type(s, n->c[n->n - 1]) : NULL; return t; }
        case TT_FILE_TYPE:
        { pty_t *t = pt_new(PTK_FILE, NULL); t->packed = off; t->base = n->n > off ? pt_type(s, n->c[n->n - 1]) : NULL; return t; }
        case TT_DECL:
        {
            if (!n->v.sval || (strcmp(n->v.sval, "proctype") && strcmp(n->v.sval, "functype"))) return NULL;
            pty_t *t = pt_new(PTK_PROC, NULL);
            t->isfunc = !strcmp(n->v.sval, "functype");
            if (t->isfunc && n->n >= 1 && PT_IS(n->c[n->n - 1], TT_VAR)) t->base = pt_leaf(s, n->c[n->n - 1]);
            return t;
        }
        default:
        return NULL;
    }
}
static void pt_resolve_pending(struct pty_state *s) {
    int w = 0;
    for (int i = 0; i < s->npend; i++) { pty_t *t = s->pend[i]; if (!t->base && t->fwd) t->base = pt_named(s, t->fwd); if (!t->base) s->pend[w++] = t; }
    s->npend = w;
}
static const pty_t *pt_deref(struct pty_state *s, const pty_t *t) { if (t && t->k == PTK_POINTER && !t->base && t->fwd) ((pty_t *)t)->base = pt_named(s, t->fwd); return t ? t->base : NULL; }
static pty_t *pt_field_of(const pty_t *r, const char *name) {
    for (int i = 0; r && name && r->k == PTK_RECORD && i < r->nf; i++) if (r->f[i].name && !strcmp(r->f[i].name, name)) return r->f[i].type;
    return NULL;
}
static pty_t *pt_ident(struct pty_state *s, const char *nm) {
    if (!nm) return NULL;
    psym_t *p = ps_find(s, nm);
    if (p) { if (p->k == 't') return NULL; if (p->k == 'f') return p->t ? p->t : s->tvoid; return p->t; }
    if (!strcmp(nm, "true") || !strcmp(nm, "false")) return s->tbool;
    if (!strcmp(nm, "nil")) return s->tnil;
    if (!strcmp(nm, "maxint") || !strcmp(nm, "maxlongint") || !strcmp(nm, "maxsmallint") || !strcmp(nm, "paramcount")) return s->tint;
    if (!strcmp(nm, "pi")) return s->treal;
    if (!strcmp(nm, "input") || !strcmp(nm, "output") || !strcmp(nm, "stdout") || !strcmp(nm, "stderr") || !strcmp(nm, "erroutput")) return s->ttext;
    if (!strcmp(nm, "eof") || !strcmp(nm, "eoln")) return s->tbool;
    if (!strcmp(nm, "random")) return s->treal;
    return NULL;
}
static pty_t *pt_index(struct pty_state *s, const tree_t *n) {
    const pty_t *bt = pt_expr(s, n->c[0]);
    for (int k = 1; k < n->n; k++) pt_expr(s, n->c[k]);
    int k = 1;
    while (k < n->n && bt) {
        if (bt->k == PTK_ARRAY) {
            if (bt->dyn || bt->nix <= 1) { bt = bt->base; k++; continue; }
            int left = n->n - k;
            if (left >= bt->nix) { k += bt->nix; bt = bt->base; continue; }
            pty_t *row = pt_new(PTK_ARRAY, NULL);
            row->nix = bt->nix - left;
            row->ix = bt->ix + left;
            row->base = bt->base;
            return row;
        }
        if (bt->k == PTK_STRING) { bt = s->tchar; k++; continue; }
        if (bt->k == PTK_POINTER) { bt = pt_deref(s, bt); k++; continue; }
        return NULL;
    }
    return (pty_t *)bt;
}
static pty_t *pt_field(struct pty_state *s, const tree_t *n) {
    const pty_t *bt = pt_expr(s, n->c[0]);
    const char *fn = (n->n >= 2 && PT_IS(n->c[1], TT_VAR)) ? n->c[1]->v.sval : NULL;
    if (bt && bt->k == PTK_POINTER) bt = pt_deref(s, bt);
    return pt_field_of(bt, fn);
}
static int pt_is_ordinal(const pty_t *t) { return t && (t->k == PTK_INT || t->k == PTK_CHAR || t->k == PTK_BOOL || t->k == PTK_ENUM); }
static pty_t *pt_routine_result(struct pty_state *s, const char *nm, int *found) {
    *found = 0;
    for (pscope_t *c = s->cur; nm && c; c = c->up) {
        pty_t *r = NULL;
        int seen = 0, mixed = 0;
        for (psym_t *p = c->s; p; p = p->next) {
            if (!p->name || strcmp(p->name, nm)) continue;
            if (p->k == 'R') continue;
            if (p->k != 'f') { if (!seen) return NULL; continue; }
            if (seen && ((r == NULL) != (p->t == NULL) || (r && p->t && r->k != p->t->k))) mixed = 1;
            if (!seen) r = p->t;
            seen = 1;
        }
        if (seen) { *found = 1; return mixed ? NULL : (r ? r : s->tvoid); }
    }
    return NULL;
}
static pty_t *pt_builtin_call(struct pty_state *s, const char *f, const tree_t *n) {
    static const char *const toint[] = { "ord", "length", "pos", "round", "trunc", "sizeof", "ioresult", "paramcount", "strtoint", "bitsizeof", "ord64", 0 };
    static const char *const toreal[] = { "sqrt", "sin", "cos", "arctan", "exp", "ln", "frac", "int", "pi", "tan", "arcsin", "arccos", "arctan2", "hypot", "power", "log10", "log2", "logn", 0 };
    static const char *const tostr[] = { "copy", "concat", "stringofchar", "format", "paramstr", "space", "dupestring", "inttostr", "trim", "trimleft", "trimright", "reversestring", 0 };
    static const char *const tobool[] = { "odd", "eof", "eoln", "assigned", "seekeof", "seekeoln", 0 };
    static const char *const same[] = { "abs", "sqr", "succ", "pred", "min", "max", 0 };
    static const char *const tovoid[] = { "write", "writeln", "read", "readln", "inc", "dec", "new", "dispose", "halt", "exit", "setlength", "fillchar", "move", "val", "str", "reset", "rewrite",
        "assign", "close", "append", "include", "exclude", "insert", "delete", "randomize", "break", "continue", "get", "put", "page", "pack", "unpack", "flush", "assignfile", "closefile", 0 };
    const pty_t *a1 = n->n >= 2 ? pt_of_src_mut(s, n->c[1]) : NULL;
    int na = n->n - 1;
    if (pt_in(f, toint)) return s->tint;
    if (pt_in(f, toreal)) return s->treal;
    if (pt_in(f, tostr)) return s->tstr;
    if (pt_in(f, tobool)) return s->tbool;
    if (pt_in(f, tovoid)) return s->tvoid;
    if (!strcmp(f, "chr")) return s->tchar;
    if (!strcmp(f, "random")) return na >= 1 ? s->tint : s->treal;
    if (pt_in(f, same)) return a1 && (a1->k == PTK_ENUM || a1->k == PTK_CHAR || a1->k == PTK_BOOL || a1->k == PTK_INT || a1->k == PTK_REAL) ? (pty_t *)a1 : NULL;
    if (!strcmp(f, "upcase") || !strcmp(f, "lowercase") || !strcmp(f, "uppercase") || !strcmp(f, "lowcase")) return a1 && a1->k == PTK_CHAR ? s->tchar : (a1 ? s->tstr : NULL);
    if (!strcmp(f, "high") || !strcmp(f, "low")) {
        if (a1 && a1->k == PTK_ARRAY && !a1->dyn && a1->nix >= 1) return a1->ix[0];
        if (a1 && (a1->k == PTK_ARRAY || a1->k == PTK_STRING)) return s->tint;
        if (n->n >= 2 && PT_IS(n->c[1], TT_VAR)) { pty_t *tt = pt_named(s, n->c[1]->v.sval); if (pt_is_ordinal(tt)) return tt; }
        return pt_is_ordinal(a1) ? (pty_t *)a1 : NULL;
    }
    return NULL;
}
static pty_t *pt_call(struct pty_state *s, const tree_t *n) {
    const tree_t *cal = n->n ? n->c[0] : NULL;
    for (int i = 1; i < n->n; i++) pt_expr(s, n->c[i]);
    if (PT_IS(cal, TT_FIELD)) {
        const char *m = (cal->n >= 2 && PT_IS(cal->c[1], TT_VAR)) ? cal->c[1]->v.sval : NULL;
        int tn = PT_IS(cal->c[0], TT_VAR) && cal->c[0]->v.sval && !strcasecmp(cal->c[0]->v.sval, "string");
        const pty_t *rt = pt_expr(s, cal->c[0]);
        if (m && tn && !strcasecmp(m, "join")) return s->tstr;
        if (m && !tn && rt && rt->k == PTK_STRING && !strcasecmp(m, "split")) { pty_t *a = pt_new(PTK_ARRAY, NULL); a->dyn = 1; a->base = s->tstr; return a; }
        return NULL;
    }
    if (!PT_IS(cal, TT_VAR) || !cal->v.sval) return NULL;
    const char *f = cal->v.sval;
    if (!strcmp(f, "xor") && n->n == 3) {
        const pty_t *a = pt_of_src_mut(s, n->c[1]), *b = pt_of_src_mut(s, n->c[2]);
        if (a && b && a->k == PTK_BOOL && b->k == PTK_BOOL) return s->tbool;
        return (a && b && a->k == PTK_INT && b->k == PTK_INT) ? s->tint : NULL;
    }
    if ((!strcmp(f, "shl") || !strcmp(f, "shr")) && n->n == 3) return s->tint;
    if (!strcmp(f, "is") && n->n == 3) return s->tbool;
    int found = 0;
    pty_t *r = pt_routine_result(s, f, &found);
    if (found) return r;
    psym_t *p = ps_find(s, f);
    if (p && p->k == 't') return p->t;
    if (p && p->t && p->t->k == PTK_PROC) return p->t->isfunc ? p->t->base : s->tvoid;
    if (p) return NULL;
    pty_t *bt = pt_builtin(s, f);
    if (bt && n->n == 2) return (s->fpc || bt->k != PTK_CHAR) ? bt : NULL;
    return pt_builtin_call(s, f, n);
}
static pty_t *pt_strbound(long long len) { pty_t *t = pt_new(PTK_STRING, "string"); t->strmax = (int)(len < 1 ? 1 : len > 1000000 ? 1000000 : len); return t; }
static long long pt_strlen_max(const pty_t *t) { return t->k == PTK_CHAR ? 1 : (t->k == PTK_STRING ? t->strmax : 0); }
static int pt_isshort(const pty_t *t) { return t->k == PTK_STRING && (t->cap || (t->name && !strcmp(t->name, "shortstring"))); }
static pty_t *pt_catbound(struct pty_state *s, const pty_t *a, const pty_t *b) {
    long long la = pt_strlen_max(a), lb = pt_strlen_max(b);
    if (la <= 0 || lb <= 0) return s->tstr;
    int cap = s->fpc && (pt_isshort(a) || pt_isshort(b)) && la + lb > 255;
    pty_t *t = pt_strbound(cap ? 255 : la + lb);
    t->cap = cap;
    return t;
}
static pty_t *pt_arith(struct pty_state *s, const tree_t *n) {
    const pty_t *a = pt_expr(s, n->c[0]), *b = pt_expr(s, n->c[1]);
    if (!a || !b) return NULL;
    if (a->k == PTK_SET || b->k == PTK_SET) return (pty_t *)(a->k == PTK_SET ? a : b);
    if (n->t == TT_ADD && (a->k == PTK_STRING || b->k == PTK_STRING)) return pt_catbound(s, a, b);
    if (n->t == TT_ADD && a->k == PTK_CHAR && b->k == PTK_CHAR) return s->fpc ? pt_strbound(2) : NULL;
    if (a->k == PTK_POINTER && b->k == PTK_INT) return (pty_t *)a;
    if (a->k == PTK_REAL || b->k == PTK_REAL) return (a->k == PTK_REAL || a->k == PTK_INT) && (b->k == PTK_REAL || b->k == PTK_INT) ? s->treal : NULL;
    if (a->k == PTK_INT && b->k == PTK_INT) return (a->uns || b->uns) ? s->tqword : s->tint;
    return NULL;
}
static pty_t *pt_expr1(struct pty_state *s, const tree_t *n) {
    switch (n->t) {
        case TT_ILIT:
        return s->tint;
        case TT_FLIT:
        return s->treal;
        case TT_QLIT:
        return (n->v.sval && strlen(n->v.sval) == 1) ? s->tchar : pt_strbound(n->v.sval ? (long long)strlen(n->v.sval) : 0);
        case TT_CHRLIT:
        return s->tchar;
        case TT_VAR:
        return pt_ident(s, n->v.sval);
        case TT_IDX:
        return n->n >= 1 ? pt_index(s, n) : NULL;
        case TT_FIELD:
        return n->n >= 1 ? pt_field(s, n) : NULL;
        case TT_DEREF:
        {
            const pty_t *t = n->n ? pt_expr(s, n->c[0]) : NULL;
            if (t && t->k == PTK_POINTER) return (pty_t *)pt_deref(s, t);
            if (t && t->k == PTK_TEXT) return s->tchar;
            if (t && t->k == PTK_FILE) return t->base;
            return NULL;
        }
        case TT_FNC:
        return pt_call(s, n);
        case TT_ADD:
        case TT_SUB:
        case TT_MUL:
        return n->n == 2 ? pt_arith(s, n) : NULL;
        case TT_DIV:
        if (n->n == 2) { pt_expr(s, n->c[0]); pt_expr(s, n->c[1]); }
        return s->treal;
        case TT_IDIV:
        case TT_MOD:
        { const pty_t *a = n->n == 2 ? pt_expr(s, n->c[0]) : NULL; if (n->n == 2) pt_expr(s, n->c[1]); return (a && a->uns) ? s->tqword : s->tint; }
        case TT_POW:
        { const pty_t *a = n->n == 2 ? pt_expr(s, n->c[0]) : NULL, *b = n->n == 2 ? pt_expr(s, n->c[1]) : NULL; return (a && b && a->k == PTK_INT && b->k == PTK_INT) ? s->tint : s->treal; }
        case TT_PLS:
        case TT_MNS:
        return n->n ? pt_expr(s, n->c[0]) : NULL;
        case TT_NOT:
        { const pty_t *a = n->n ? pt_expr(s, n->c[0]) : NULL; if (a && a->k == PTK_BOOL) return s->tbool; return (a && a->k == PTK_INT) ? s->tint : NULL; }
        case TT_CONJ:
        case TT_ALT:
        {
            const pty_t *a = n->n == 2 ? pt_expr(s, n->c[0]) : NULL, *b = n->n == 2 ? pt_expr(s, n->c[1]) : NULL;
            if (a && b && a->k == PTK_BOOL && b->k == PTK_BOOL) return s->tbool;
            return (a && b && a->k == PTK_INT && b->k == PTK_INT) ? s->tint : NULL;
        }
        case TT_LT:
        case TT_LE:
        case TT_GT:
        case TT_GE:
        case TT_EQ:
        case TT_NE:
        case TT_IN:
        for (int i = 0; i < n->n; i++) pt_expr(s, n->c[i]);
        return s->tbool;
        case TT_SET:
        { pty_t *t = pt_new(PTK_SET, NULL); for (int i = 0; i < n->n; i++) { const pty_t *m = pt_expr(s, n->c[i]); if (!t->base && m) t->base = (pty_t *)m; } return t; }
        case TT_SUBRANGE:
        { const pty_t *a = n->n ? pt_expr(s, n->c[0]) : NULL; for (int i = 1; i < n->n; i++) pt_expr(s, n->c[i]); return (pty_t *)a; }
        case TT_ADDR:
        { const pty_t *a = n->n ? pt_expr(s, n->c[0]) : NULL; pty_t *t = pt_new(PTK_POINTER, NULL); t->base = (pty_t *)a; return t; }
        case TT_FMT:
        { const pty_t *a = n->n ? pt_expr(s, n->c[0]) : NULL; for (int i = 1; i < n->n; i++) pt_expr(s, n->c[i]); return (pty_t *)a; }
        case TT_KEYWORD:
        return (n->n == 1 && PT_IS(n->c[0], TT_FNC)) ? pt_expr(s, n->c[0]) : NULL;
        default:
        return NULL;
    }
}
static pty_t *pt_expr(struct pty_state *s, const tree_t *n) {
    if (!n) return NULL;
    pty_t *t = pt_expr1(s, n);
    if (!t) return t;
    pt_map_put(&s->src, n, t, s->line);
    if (PT_IS(n, TT_FIELD) || (PT_IS(n, TT_VAR) && n->v.sval && ps_find(s, n->v.sval) && ps_find(s, n->v.sval)->k == 'd')) ((pty_ent *)pt_map_get(&s->src, n))->fld = 1;
    return t;
}
static const pty_t *pt_of_src_mut(struct pty_state *s, const tree_t *n) { const pty_ent *e = pt_map_get(&s->src, n); return e ? e->t : NULL; }
static void pt_block(struct pty_state *s, const tree_t *b);
static void pt_stmt(struct pty_state *s, const tree_t *n);
static void pt_stmts(struct pty_state *s, const tree_t *n) { for (int i = 0; n && i < n->n; i++) pt_stmt(s, n->c[i]); }
static void pt_with(struct pty_state *s, const tree_t *n) {
    int ns = n->n - 1;
    ps_push(s);
    for (int i = 0; i < ns; i++) {
        const pty_t *t = pt_expr(s, n->c[i]);
        if (t && t->k == PTK_POINTER) t = pt_deref(s, t);
        for (int j = 0; t && t->k == PTK_RECORD && j < t->nf; j++) ps_def(s, t->f[j].name, 'd', t->f[j].type);
    }
    if (ns >= 0) pt_stmt(s, n->c[ns]);
    ps_pop(s);
}
static void pt_stmt(struct pty_state *s, const tree_t *n) {
    if (!n) return;
    if (n->line) s->line = n->line;
    switch (n->t) {
        case TT_SEQ_EXPR:
        pt_stmts(s, n);
        return;
        case TT_ASSIGN:
        for (int i = 0; i < n->n && i < 2; i++) pt_expr(s, n->c[i]);
        return;
        case TT_FNC:
        pt_expr(s, n);
        return;
        case TT_IF:
        if (n->n >= 1) pt_expr(s, n->c[0]);
        for (int i = 1; i < n->n; i++) pt_stmt(s, n->c[i]);
        return;
        case TT_WHILE:
        if (n->n >= 1) pt_expr(s, n->c[0]);
        if (n->n >= 2) pt_stmt(s, n->c[1]);
        return;
        case TT_REPEAT:
        if (n->n >= 1) pt_stmt(s, n->c[0]);
        if (n->n >= 2) pt_expr(s, n->c[1]);
        return;
        case TT_FOR:
        for (int i = 0; i < n->n - 1; i++) pt_expr(s, n->c[i]);
        if (n->n >= 1) pt_stmt(s, n->c[n->n - 1]);
        return;
        case TT_CASE:
        if (n->n >= 1) pt_expr(s, n->c[0]);
        for (int i = 1; i < n->n; i++) {
            const tree_t *a = n->c[i];
            if (PT_IS(a, TT_ARM)) {
                if (a->n >= 1 && a->c[0]) for (int j = 0; j < a->c[0]->n; j++) pt_expr(s, a->c[0]->c[j]);
                if (a->n >= 2) pt_stmt(s, a->c[1]);
            } else if (PT_IS(a, TT_PART)) pt_stmts(s, a);
            else pt_stmt(s, a);
        }
        return;
        case TT_WITH:
        pt_with(s, n);
        return;
        case TT_LABEL_DEF:
        if (n->n >= 2) pt_stmt(s, n->c[1]);
        return;
        case TT_TRY:
        for (int i = 0; i < n->n; i++) {
            const tree_t *c = n->c[i];
            if (PT_IS(c, TT_VLIST)) {
                for (int j = 0; j < c->n; j++) {
                    const tree_t *h = c->c[j];
                    if (!PT_IS(h, TT_CATCH) || h->n < 2) continue;
                    ps_push(s);
                    if (h->n >= 3 && PT_IS(h->c[0], TT_VAR) && PT_IS(h->c[1], TT_VAR)) ps_def(s, h->c[0]->v.sval, 'v', pt_named(s, h->c[1]->v.sval));
                    pt_stmt(s, h->c[h->n - 1]);
                    ps_pop(s);
                }
            } else if (PT_IS(c, TT_PART)) pt_stmts(s, c);
            else pt_stmt(s, c);
        }
        return;
        case TT_DIE:
        if (n->n >= 1) pt_expr(s, n->c[0]);
        return;
        case TT_KEYWORD:
        if (n->n == 1) pt_expr(s, n->c[0]);
        return;
        default:
        return;
    }
}
static void pt_params(struct pty_state *s, const tree_t *ps) {
    for (int i = 0; ps && i < ps->n; i++) {
        const tree_t *d = ps->c[i];
        if (!PT_IS(d, TT_DECL) || !d->v.sval || d->n < 1) continue;
        if (!strcmp(d->v.sval, "procedure") || !strcmp(d->v.sval, "function")) {
            pty_t *t = pt_new(PTK_PROC, NULL);
            t->isfunc = !strcmp(d->v.sval, "function");
            if (t->isfunc && PT_IS(d->c[d->n - 1], TT_VAR) && d->n >= 2) t->base = pt_leaf(s, d->c[d->n - 1]);
            if (PT_IS(d->c[0], TT_VAR)) ps_def(s, d->c[0]->v.sval, 'p', t);
            continue;
        }
        pty_t *t = d->n >= 2 ? pt_type(s, d->c[1]) : NULL;
        int k = !strcmp(d->v.sval, "var") ? 'r' : 'p';
        const tree_t *ids = d->c[0];
        if (PT_IS(ids, TT_VLIST)) { for (int j = 0; j < ids->n; j++) if (ids->c[j]) ps_def(s, ids->c[j]->v.sval, k, t); } else if (PT_IS(ids, TT_VAR)) ps_def(s, ids->v.sval, k, t);
        if (d->n >= 3) pt_expr(s, d->c[2]);
    }
}
static void pt_routine(struct pty_state *s, const tree_t *r) {
    const tree_t *nm = r->n ? r->c[0] : NULL, *ps = NULL, *rt = NULL, *body = NULL;
    const char *name = PT_IS(nm, TT_VAR) ? nm->v.sval : NULL;
    for (int i = 1; i < r->n; i++) { const tree_t *c = r->c[i]; if (PT_IS(c, TT_PARAMS)) ps = c; else if (PT_IS(c, TT_BLOCK)) body = c; else if (PT_IS(c, TT_VAR) && !body) rt = c; }
    int isfunc = r->t == TT_FUNCTION;
    pty_t *res = isfunc && rt ? pt_leaf(s, rt) : NULL;
    if (isfunc && !res && !rt) { psym_t *fw = ps_find_kind(s, name, 'f'); if (fw) res = fw->t; }
    if (name) { psym_t *p = ps_def(s, name, 'f', res); p->rt = r; }
    if (!body) return;
    ps_push(s);
    pt_map_put(&s->scp, r, (const pty_t *)s->cur, 0);
    if (!ps && name) for (pscope_t *c = s->cur->up; c && !ps; c = c->up) for (psym_t *p = c->s; p && !ps; p = p->next) if (p->k == 'f' && p->rt && p->rt != r && p->name && !strcmp(p->name, name)) {
        for (int i = 1; i < p->rt->n; i++) if (PT_IS(p->rt->c[i], TT_PARAMS)) ps = p->rt->c[i];
    }
    if (isfunc && res) ps_def(s, "result", 'R', res);
    pt_params(s, ps);
    if (isfunc && name) ps_def(s, name, 'R', res);
    pt_block(s, body);
    ps_pop(s);
}
static void pt_decl_part(struct pty_state *s, const tree_t *p) {
    const char *w = p->v.sval;
    if (!w) return;
    for (int i = 0; i < p->n; i++) {
        const tree_t *d = p->c[i];
        if (!PT_IS(d, TT_DECL) || !d->v.sval) continue;
        if (!strcmp(d->v.sval, "const") && d->n >= 2 && PT_IS(d->c[0], TT_VAR)) {
            pty_t *t = pt_expr(s, d->c[1]);
            psym_t *sym = ps_def(s, d->c[0]->v.sval, 'c', t);
            sym->hv = pt_const(s, d->c[1], &sym->iv);
        } else if (!strcmp(d->v.sval, "type") && d->n >= 2 && PT_IS(d->c[0], TT_VAR)) {
            pty_t *t = pt_type(s, d->c[1]);
            if (t && !t->name) t->name = d->c[0]->v.sval;
            ps_def(s, d->c[0]->v.sval, 't', t);
        } else if ((!strcmp(d->v.sval, "var") || !strcmp(d->v.sval, "tconst")) && d->n >= 2) {
            pty_t *t = pt_type(s, d->c[1]);
            const tree_t *ids = d->c[0];
            if (PT_IS(ids, TT_VLIST)) { for (int j = 0; j < ids->n; j++) if (ids->c[j]) ps_def(s, ids->c[j]->v.sval, 'v', t); } else if (PT_IS(ids, TT_VAR)) ps_def(s, ids->v.sval, 'v', t);
            if (d->n >= 3) pt_expr(s, d->c[2]);
        }
    }
    if (!strcmp(w, "type")) pt_resolve_pending(s);
}
static void pt_block(struct pty_state *s, const tree_t *b) {
    for (int i = 0; b && i < b->n; i++) {
        const tree_t *c = b->c[i];
        if (PT_IS(c, TT_PART)) pt_decl_part(s, c);
        else if (PT_IS(c, TT_PROCEDURE) || PT_IS(c, TT_FUNCTION)) pt_routine(s, c);
        else if (PT_IS(c, TT_SEQ_EXPR) || PT_IS(c, TT_PROGRAM)) { pt_resolve_pending(s); pt_stmts(s, c); }
    }
}
struct pt_prag { int men, set, rec, al; };
struct pt_pw { struct pt_prag cur, *stk; int sp, cap; };
static void pt_prag_event(struct pt_pw *w, const char *ev) {
    if (!strncmp(ev, "minenum ", 8)) w->cur.men = atoi(ev + 8);
    else if (!strncmp(ev, "packset ", 8)) w->cur.set = atoi(ev + 8);
    else if (!strncmp(ev, "packrec ", 8)) w->cur.rec = atoi(ev + 8);
    else if (!strncmp(ev, "align ", 6)) w->cur.al = ev[6] == '1';
    else if (!strcmp(ev, "push")) {
        if (w->sp >= w->cap) { w->cap = w->cap ? w->cap * 2 : 16; w->stk = (struct pt_prag *)ct_grow(w->stk, (size_t)w->cap * sizeof *w->stk); }
        w->stk[w->sp++] = w->cur;
    } else if (!strcmp(ev, "pop") && w->sp > 0) {
        w->sp--;
        w->cur.men = w->stk[w->sp].men;
        w->cur.set = w->stk[w->sp].set;
        w->cur.rec = w->stk[w->sp].rec;
    }
}
static void pt_prag_walk(struct pty_state *s, struct pt_pw *w, const tree_t *n) {
    if (!n) return;
    for (int i = 0; i < n->n; i++) {
        const tree_t *a = n->c[i];
        if (PT_IS(a, TT_ATTR) && a->v.sval && !strcmp(a->v.sval, ":pragma") && a->n > 0 && a->c[0] && a->c[0]->v.sval) pt_prag_event(w, a->c[0]->v.sval);
    }
    for (int i = 0; i < n->n; i++) if (!PT_IS(n->c[i], TT_ATTR)) pt_prag_walk(s, w, n->c[i]);
    if (PT_IS(n, TT_RECORD) || PT_IS(n, TT_ENUM_TYPE)) pt_map_put(&s->prag, n, NULL, w->cur.men | (w->cur.rec << 8) | (w->cur.al << 16));
}
static void pt_prag_apply(struct pty_state *s, pty_t *t, const tree_t *n) {
    const pty_ent *e = pt_map_get(&s->prag, n);
    t->pmen = e ? (e->line & 0xff) : 4;
    t->prec = e ? ((e->line >> 8) & 0xff) : 0;
    t->pal = e ? ((e->line >> 16) & 1) : 0;
}
void pty_run(tree_t *prog, int fpc) {
    struct pty_state *s = pt_mk();
    s->fpc = fpc;
    s->stats = (long *)ct_zalloc(80, sizeof(long));
    s->tint = pt_ord(PTK_INT, "integer", -2147483648LL, 2147483647LL);
    s->tqword = pt_ord(PTK_INT, "qword", 0, 0);
    s->tqword->uns = 1;
    s->tqword->bounded = 0;
    s->tchar = pt_ord(PTK_CHAR, "char", 0, 255);
    s->tbool = pt_ord(PTK_BOOL, "boolean", 0, 1);
    s->treal = pt_new(PTK_REAL, "real");
    s->tsingle = pt_new(PTK_REAL, "single");
    s->tstr = pt_new(PTK_STRING, "string");
    s->tshort = pt_new(PTK_STRING, "shortstring");
    s->tshort->strmax = 255;
    s->ttext = pt_new(PTK_TEXT, "text");
    s->tfile = pt_new(PTK_FILE, "file");
    s->tnil = pt_new(PTK_NIL, "nil");
    s->tvoid = pt_new(PTK_VOID, NULL);
    s->tptr = pt_new(PTK_POINTER, "pointer");
    s->tpchar = pt_new(PTK_POINTER, "pchar");
    s->tpchar->base = s->tchar;
    ps_push(s);
    { struct pt_pw w = { { 4, 0, 0, 0 }, NULL, 0, 0 }; pt_prag_walk(s, &w, prog); if (w.stk) ct_drop(w.stk); }
    pt_map_put(&s->scp, prog, (const pty_t *)s->cur, 0);
    ps_def(s, "input", 'v', s->ttext);
    ps_def(s, "output", 'v', s->ttext);
    if (fpc) ps_def(s, "returnnilifgrowheapfails", 'v', s->tbool);
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t *c = prog->c[i];
        if (PT_IS(c, TT_VLIST)) for (int j = 0; j < c->n; j++) if (c->c[j] && c->c[j]->v.sval) ps_def(s, c->c[j]->v.sval, 'v', s->ttext);
    }
    for (int i = 0; prog && i < prog->n; i++) if (PT_IS(prog->c[i], TT_BLOCK)) pt_block(s, prog->c[i]);
}
const pty_t *pty_of_src(const tree_t *n) { struct pty_state *s = pt_st(); if (!s) return NULL; const pty_ent *e = pt_map_get(&s->src, n); return e ? e->t : NULL; }
const pty_t *pty_of_low(const tree_t *e) { struct pty_state *s = pt_st(); if (!s) return NULL; const pty_ent *x = pt_map_get(&s->low, e); return x ? x->t : NULL; }
void pty_note_low(const tree_t *low, const tree_t *src) {
    struct pty_state *s = pt_st();
    if (!s || !low || !src) return;
    const pty_ent *e = pt_map_get(&s->src, src);
    if (!e || !e->t) return;
    const pty_t *t = e->t;
    if (t->k == PTK_PROC && low->t == TT_FNC && low->n > 0 && low->c[0] && low->c[0]->v.sval && !strcmp(low->c[0]->v.sval, "__pas_pcall")) t = t->isfunc ? t->base : NULL;
    if (t) { pt_map_put(&s->low, low, t, e->line); ((pty_ent *)pt_map_get(&s->low, low))->fld = e->fld; }
}
int pty_is_field(const tree_t *low) { struct pty_state *s = pt_st(); const pty_ent *x = s ? pt_map_get(&s->low, low) : NULL; return x ? x->fld : 0; }
const void *pty_enter(const tree_t *src) {
    struct pty_state *s = pt_st();
    if (!s) return NULL;
    const void *prev = s->ecur;
    const pty_ent *e = pt_map_get(&s->scp, src);
    if (e) s->ecur = (pscope_t *)e->t;
    return prev;
}
void pty_leave(const void *prev) { struct pty_state *s = pt_st(); if (s) s->ecur = (pscope_t *)prev; }
static psym_t *pe_find(struct pty_state *s, const char *name) {
    for (pscope_t *c = s->ecur; name && c; c = c->up) for (psym_t *p = c->s; p; p = p->next)
        if (p->name && !strcmp(p->name, name) && (p->k == 'v' || p->k == 'r' || p->k == 'p' || p->k == 'R' || p->k == 'c')) return p;
    return NULL;
}
int pty_is_pchar(const pty_t *t) { struct pty_state *s = pt_st(); return s && t && t == s->tpchar; }
const pty_t *pty_name_type(const char *name) { struct pty_state *s = pt_st(); if (!s || !s->ecur) return NULL; psym_t *p = pe_find(s, name); return p ? p->t : NULL; }
int pty_name_is(const char *name, pty_kind want) {
    struct pty_state *s = pt_st();
    if (!s || !s->ecur) return -1;
    psym_t *p = pe_find(s, name);
    if (!p) return -1;
    if (!p->t || p->t->k == PTK_UNKNOWN) return -1;
    if (p->t->k == PTK_BOOL && p->t->wbool) return want == PTK_INT;
    return p->t->k == want;
}
void pty_note_kind(const tree_t *low, pty_kind k) { struct pty_state *s = pt_mk(); if (low) pt_map_put(&s->low, low, pt_new(k, NULL), 0); }
int pty_answer(const tree_t *low, pty_kind want) {
    const pty_t *t = pty_of_low(low);
    if (!t || t->k == PTK_UNKNOWN) return -1;
    if (t->k == PTK_BOOL && t->wbool) return want == PTK_INT;
    return t->k == want;
}
const char *pty_kind_name(pty_kind k) {
    static const char *const nm[] = { "unknown", "int", "char", "bool", "enum", "real", "string", "array", "record", "set", "pointer", "file", "text", "proc", "nil", "void" };
    return (k >= PTK_UNKNOWN && k <= PTK_VOID) ? nm[k] : "?";
}
static const char *pt_what(const tree_t *h) {
    while (h && (h->t == TT_IDX || h->t == TT_FIELD) && h->n > 0) h = h->c[0];
    if (h && h->t == TT_FNC && h->n > 0 && h->c[0] && h->c[0]->t == TT_VAR) return h->c[0]->v.sval;
    if (h && h->t == TT_VAR) return h->v.sval;
    return "";
}
void pty_census(const char *pred, const tree_t *low, int typer) {
    struct pty_state *s = pt_st();
    if (!s || !g_lower.pas.dump_types) return;
    int id = !strcmp(pred, "char") ? 0 : !strcmp(pred, "string") ? 1 : !strcmp(pred, "bool") ? 2 : 3;
    s->stats[id]++;
    if (typer < 0) { fprintf(stderr, "PTY-UNK %s node=%d %s\n", pred, low ? (int)low->t : -1, pt_what(low)); return; }
    s->stats[4 + id]++;
    s->stats[8 + id]++;
}
static int lf_has_file(const pty_t *t) {
    if (!t) return 0;
    if (t->k == PTK_FILE || t->k == PTK_TEXT) return 1;
    if (t->k == PTK_RECORD) for (int i = 0; i < t->nf; i++) if (lf_has_file(t->f[i].type)) return 1;
    return 0;
}
static int lf_char(const pty_t *t) { return !t ? 0 : t->k == PTK_CHAR ? 1 : t->k == PTK_BOOL ? 2 : 0; }
static int lf_rec(const pty_t *t) { while (t && t->k == PTK_ARRAY) t = t->base; return t && t->k == PTK_RECORD && t->lay && t->lay->nf > 0; }
static int lf_eff_char(const struct pty_lay *L, int i);
static int lf_leak_bool(const struct pty_lay *L) { for (int i = L->nf - 1; i >= 0; i--) { if (L->named[i] & 2) continue; return lf_eff_char(L, i) == 2; } return 0; }
static int lf_eff_char(const struct pty_lay *L, int i) {
    const pty_t *ft = L->ft[i];
    if (L->named[i] & 4) return i > 0 && lf_eff_char(L, i - 1) == 2 ? 2 : 0;
    if (ft && ft->k == PTK_RECORD && ft->lay && ft->lay->nf > 0 && lf_leak_bool(ft->lay)) return 2;
    return lf_char(ft);
}
static int lf_arr(const pty_t *t, int *ischar, long long *lo, long long *hi) {
    if (!t || t->k != PTK_ARRAY) return 0;
    const pty_t *e = t->base;
    while (e && e->k == PTK_ARRAY) e = e->base;
    *ischar = e && e->k == PTK_CHAR;
    if (t->dyn || t->open) { *lo = 0; *hi = *ischar ? -1 : 0; return 1; }
    if (t->nix < 1 || !t->ix || !t->ix[0]) return 0;
    *lo = t->ix[0]->lo;
    *hi = t->ix[0]->hi;
    if (t->nix == 2 && t->ix[1]) { *lo = 0; *hi = (t->ix[0]->hi + 1) * (t->ix[1]->hi + 1) - 1; }
    return 1;
}
struct lfx { int ch, ca, na, file, ptr, rec, en, isreal, isset, isstr, ph, el; long long calo, cahi, nalo, nahi; const pty_t *ft; int nm; };
static void lf_of(const struct pty_lay *L, int i, struct lfx *f) {
    memset(f, 0, sizeof *f);
    const pty_t *ft = L->ft[i];
    int ph = (L->named[i] & 4) != 0, nm = L->named[i] & 1;
    if (ph) { int j = i - 1; while (j >= 0 && (L->named[j] & 4)) j--; if (j >= 0) { ft = L->ft[j]; nm = L->named[j] & 1; } }
    int ischar = 0;
    long long lo = 0, hi = -1;
    int ar = lf_arr(ft, &ischar, &lo, &hi);
    const pty_t *eft = ft;
    if (ph && !nm) while (eft && eft->k == PTK_ARRAY) eft = eft->base;
    if (ar && ft->nix == 2 && !ft->dyn && !nm && ft->ix[1]) { lo = ft->ix[1]->lo; hi = ft->ix[1]->hi; }
    f->ph = ph;
    f->nm = nm;
    f->ft = ph ? eft : ft;
    f->ca = ar && ischar;
    f->na = ar && !ischar;
    if (ph && !nm) { f->ca = 0; f->na = 0; }
    f->calo = lo > 0 ? lo : 0;
    f->cahi = hi;
    f->nalo = lo;
    f->nahi = hi;
    f->ch = lf_eff_char(L, i);
    if (f->ca && !nm && !ph) f->ch = 1;
    f->file = lf_has_file(ft);
    f->ptr = !ph && ft && ft->k == PTK_POINTER && (ft->base || ft->fwd);
    f->rec = lf_rec(ft);
    f->en = ft && ft->k == PTK_ENUM && ft->name && ft->nf > 0;
    f->isreal = ft && ft->k == PTK_REAL;
    f->isset = ft && ft->k == PTK_SET;
    f->isstr = ft && ft->k == PTK_STRING;
}
static int lk_bsize(const char *n, long long *out) {
    static const struct {
        const char *n;
        long long sz;
    } T[] = { {"integer",4},{"longint",4},{"shortint",1},{"byte",1},{"word",2},{"smallint",2},{"cardinal",4},{"longword",4},{"int64",8},{"qword",8},{"single",4}, {"real",8},{"double",8},{"extended",
        10},{"comp",8},{"char",1},{"widechar",1},{"boolean",1},{"bytebool",1},{"wordbool",2},{"longbool",4},{"qwordbool",8},{"pointer",8},{"ptruint",8},{"ptrint",8}, {"int8",1},{"int16",2},{"int32",
        4},{"uint8",1},{"uint16",2},{"uint32",4},{"uint64",8},{"nativeint",8},{"nativeuint",8},{"codepointer",8} };
    for (size_t i = 0; n && i < sizeof T / sizeof T[0]; i++) if (!strcmp(n, T[i].n)) { *out = T[i].sz; return 1; }
    return 0;
}
static int lk_isint(const char *t) {
    static const char *const L[] = { "integer","longint","shortint","byte","word","cardinal","int64","qword","smallint","longword","uint8","uint16","uint32","uint64","int8","int16","int32",
        "nativeint","nativeuint","sizeint","sizeuint","ptrint","ptruint", 0 };
    return pt_in(t, L);
}
static long long lk_fit(long long lo, long long hi) {
    if (lo >= 0) return hi <= 255 ? 1 : hi <= 65535 ? 2 : 4;
    if (lo >= -128 && hi <= 127) return 1;
    if (lo >= -32768 && hi <= 32767) return 2;
    return 4;
}
static int lk_bbound(const char *n, long long *lo) {
    static const struct {
        const char *n;
        long long lo;
    } T[] = { {"byte",0},{"shortint",-128},{"word",0},{"smallint",-32768},{"longword",0},{"cardinal",0},{"uint32",0},{"longint",-1},{"integer",-1},{"int32",-1},{"int8",-1},{"uint8",0}, {"int16",-1},
        {"uint16",0},{"int64",-1},{"qword",0},{"uint64",0},{"nativeint",-1},{"nativeuint",0} };
    for (size_t i = 0; n && i < sizeof T / sizeof T[0]; i++) if (!strcmp(n, T[i].n)) { *lo = T[i].lo; return 1; }
    return 0;
}
static int lk_scalar(const struct lfx *f, int *cls, long long *sz) {
    const pty_t *t = f->ft;
    long long s = 0, lo = 0;
    if (!t) return 0;
    if (f->ch == 1 && !f->ph) { *cls = 4; *sz = 1; return 1; }
    if (f->ch == 2) { *cls = 5; *sz = 1; return 1; }
    if (t->k == PTK_CHAR && !f->ph && !f->el) { *cls = 4; *sz = 1; return 1; }
    if (t->k == PTK_REAL) { if (!lk_bsize(t->name, &s) || (s != 4 && s != 8)) return 0; *cls = 3; *sz = s; return 1; }
    if (t->k == PTK_SET) {
        long long h = t->base ? (t->base->k == PTK_ENUM && t->base->nf > 0 ? t->base->nf - 1 : t->base->bounded ? t->base->hi : 255) : 255;
        *sz = (h >= 0 && h <= 31) ? 4 : 32;
        *cls = 6;
        return 1;
    }
    if (t->k == PTK_ENUM && t->nf > 0) { long long fit = lk_fit(0, t->hi), ms = 4; *sz = fit > ms ? fit : ms; if (*sz > 8) return 0; *cls = 2; return 1; }
    if (t->k == PTK_INT || t->k == PTK_ENUM || t->k == PTK_CHAR || t->k == PTK_BOOL) {
        if (t->name && lk_bsize(t->name, &s) && lk_isint(t->name)) { *sz = s; *cls = (lk_bbound(t->name, &lo) && lo >= 0) ? 2 : 1; return 1; }
        if (t->bounded && !(t->name && lk_bsize(t->name, &s))) { *sz = lk_fit(t->lo, t->hi); *cls = t->lo >= 0 ? 2 : 1; return 1; }
        return 0;
    }
    return 0;
}
static long long lk_rk(const struct pty_lay *L, int i) {
    struct lfx f;
    lf_of(L, i, &f);
    if (f.file || f.rec || f.ptr) return 0;
    if (f.ca || f.na) {
        long long lo = f.ca ? f.calo : f.nalo, hi = f.ca ? f.cahi : f.nahi, sz = 0, count;
        int ecls = 0;
        if (hi < lo || hi - lo + 1 > 60000) return 0;
        count = hi - lo + 1;
        if (f.ca) {
            ecls = 4;
            sz = 1;
        } else {
            struct lfx e;
            memset(&e, 0, sizeof e);
            e.ft = f.ft && f.ft->k == PTK_ARRAY ? f.ft->base : NULL;
            if ((L->named[i] & 1) || !e.ft) return 0;
            e.el = 1;
            if (!lk_scalar(&e, &ecls, &sz) || ecls == 6 || ecls == 3) return 0;
        }
        return 7 | (sz << 4) | ((long long)ecls << 12) | (count << 16);
    }
    { int cls = 0; long long sz = 0; return lk_scalar(&f, &cls, &sz) ? (cls | (sz << 4)) : 0; }
}
static long long lk_get(long long ov, int w) { return w == 0 ? (ov >> 16) & 0xF : w == 1 ? (ov >> 20) & 0xFF : w == 2 ? (ov >> 28) & 0xF : w == 3 ? (ov >> 32) & 0xFFFF : (ov >> 48) & 0xFF; }
struct lk_vs { int lo, hi, n, first, last, nested; };
static int lk_arm_field(const long long *ov, int first, int last, int arm, int j) { int k = 0; for (int i = first; i < last; i++) if (lk_get(ov[i], 4) == arm) { if (k++ == j) return i; } return -1; }
static int lk_region_do(long long *ov, int nfnow, const struct lk_vs *v, int d) {
    if (d != 0) return 0;
    int first = v->first, last = v->last, narms = v->n;
    if (narms < 2 || last <= first || last > nfnow || v->nested) return 0;
    for (int i = first; i < last; i++) if (!lk_get(ov[i], 0)) return 0;
    int legacy = 1;
    for (int a = 2; a <= narms && legacy; a++) {
        for (int j = 0; ; j++) {
            int r = lk_arm_field(ov, first, last, 1, j), f = lk_arm_field(ov, first, last, a, j);
            if (r < 0 && f < 0) break;
            if (r < 0 || f < 0) { legacy = 0; break; }
            long long cr = lk_get(ov[r], 0), cf = lk_get(ov[f], 0);
            if (cr != cf || lk_get(ov[r], 1) != lk_get(ov[f], 1) || !(cr == 1 || cr == 2 || cr == 4 || cr == 5)) { legacy = 0; break; }
        }
    }
    if (legacy) return 0;
    long long rsize = 0;
    for (int a = 1; a <= narms; a++) {
        long long off = 0;
        for (int j = 0; ; j++) {
            int i = lk_arm_field(ov, first, last, a, j);
            if (i < 0) break;
            long long cls = lk_get(ov[i], 0), es = lk_get(ov[i], 1), cnt = lk_get(ov[i], 3), sz = cls == 7 ? es * cnt : es, al = es < 1 ? 1 : (es > 8 ? 8 : es);
            off = (off + al - 1) / al * al;
            ov[i] = (ov[i] & ~(0xFFFFLL << 32)) | (off << 32);
            off += sz;
        }
        if (off > rsize) rsize = off;
    }
    if (rsize < 1 || rsize > 60000) return 0;
    for (int i = first; i < last; i++) ov[i] = (ov[i] & 0xFFFF0000LL) | (ov[i] & 0xFFFF00000000LL) | (long long)first | (rsize << 48);
    return 1;
}
static int lk_overlayable(const struct pty_lay *L, int a, int b) {
    struct lfx fa, fb;
    lf_of(L, a, &fa);
    lf_of(L, b, &fb);
    if (fa.rec || fb.rec || fa.ca || fb.ca || fa.na || fb.na || fa.file || fb.file || fa.ptr || fb.ptr || (fa.ch == 1) != (fb.ch == 1)) return 0;
    if (fa.isreal || fa.isset || fa.isstr || fb.isreal || fb.isset || fb.isstr) return 0;
    return 1;
}
static void lb_compute_ov(struct pty_lay *L) {
    struct lk_vs vs[64];
    int depth = 0, nfnow = 0;
    for (int e = 0; e < L->nev; e++) {
        int code = L->ev[e] & 7, arg = L->ev[e] >> 3;
        if (code == 1) {
            L->ov[nfnow] = (long long)nfnow | (lk_rk(L, nfnow) << 16);
            nfnow++;
        } else if (code == 2) {
            if (depth > 0 && depth <= 64) vs[depth - 1].nested = 1;
            if (depth < 64) memset(&vs[depth], 0, sizeof vs[depth]);
            depth++;
        } else if (code == 3) {
            if (depth <= 0 || depth > 64) continue;
            int start = arg, end = nfnow, d = depth - 1;
            for (int i = start; i < end; i++) L->ov[i] = (L->ov[i] & ~(0xFFLL << 48)) | ((long long)(vs[d].n + 1) << 48);
            if (vs[d].n == 0) vs[d].first = start;
            vs[d].last = end;
            if (vs[d].n++ == 0) { vs[d].lo = start; vs[d].hi = end; continue; }
            for (int j = 0; start + j < end && vs[d].lo + j < vs[d].hi; j++) {
                int a = start + j, c = (int)(L->ov[vs[d].lo + j] & 0xFFFF);
                if (c != a && lk_overlayable(L, a, c)) L->ov[a] = (L->ov[a] & ~0xFFFFLL) | (long long)c;
            }
        } else if (code == 4) {
            if (depth > 0 && depth <= 64) { int d = depth - 1; if (!lk_region_do(L->ov, nfnow, &vs[d], d)) for (int i = vs[d].first; i < vs[d].last; i++) L->ov[i] &= 0xFFFFFFFFLL; }
            if (depth > 0) depth--;
        }
    }
}
static int ty_rec_layout(const pty_t *r, long long *size_out, long long *align_out, int depth);
static int ty_sizeof(const pty_t *t, long long *out, int depth) {
    long long s = 0;
    if (!t || depth > 8) return 0;
    if (t->name && lk_bsize(t->name, &s) && t->k != PTK_ARRAY && t->k != PTK_RECORD) { *out = s; return 1; }
    switch (t->k) {
        case PTK_ENUM:
        if (t->nf > 0) { long long fit = lk_fit(0, t->hi), ms = t->pmen ? t->pmen : 4; *out = fit > ms ? fit : ms; return 1; }
        if (t->bounded) { *out = lk_fit(t->lo, t->hi); return 1; }
        return 0;
        case PTK_INT:
        case PTK_CHAR:
        case PTK_BOOL:
        if (t->bounded) { *out = lk_fit(t->lo, t->hi); return 1; }
        return 0;
        case PTK_SET:
        { long long h = t->base ? (t->base->k == PTK_ENUM && t->base->nf > 0 ? t->base->nf - 1 : t->base->bounded ? t->base->hi : 255) : 255; *out = (h >= 0 && h <= 31) ? 4 : 32; return 1; }
        case PTK_POINTER:
        *out = 8;
        return 1;
        case PTK_RECORD:
        return ty_rec_layout(t, out, NULL, depth + 1);
        case PTK_ARRAY:
        {
            long long e = 0;
            if (t->dyn || t->open || t->nix < 1 || !t->ix || !t->ix[0] || !t->ix[0]->bounded || !ty_sizeof(t->base, &e, depth + 1)) return 0;
            long long cnt = 1;
            for (int i = 0; i < t->nix; i++) { if (!t->ix[i] || !t->ix[i]->bounded) return 0; cnt *= (t->ix[i]->hi - t->ix[i]->lo + 1); }
            *out = cnt * e;
            return 1;
        }
        case PTK_STRING:
        if (t->name && !strcmp(t->name, "shortstring") && t->strmax > 0) { *out = (long long)t->strmax + 1; return 1; }
        return 0;
        default:
        return 0;
    }
}
static int ty_rec_fld_sa(const pty_t *r, int idx, long long *sz, long long *al, int depth) {
    struct pty_fld f;
    if (!pty_rec_fld(r, idx, &f)) return 0;
    long long s, nat;
    if (f.ptr) {
        s = 8;
        nat = 8;
    } else if (f.rec) {
        const pty_t *x = f.ft;
        while (x && x->k == PTK_ARRAY) x = x->base;
        if (!ty_rec_layout(x, &s, &nat, depth + 1)) return 0;
    } else if (f.ca) {
        long long cnt = f.cahi - f.calo + 1;
        s = cnt > 0 ? cnt : 1;
        nat = 1;
    } else if (f.na) {
        long long cnt = f.nahi - f.nalo + 1, esz = 4;
        if (cnt < 1) cnt = 1;
        const pty_t *et = f.enm ? f.eft : (f.eft && f.eft->k == PTK_ARRAY ? f.eft->base : f.eft);
        if (f.eft && f.eft->k == PTK_ARRAY && ((!f.enm && f.eft->dyn) || (f.enm && f.eft->nix > 1))) et = NULL;
        if (et) ty_sizeof(et, &esz, depth + 1);
        s = cnt * esz;
        nat = esz < 1 ? 1 : (esz > 8 ? 8 : esz);
    } else if (f.en) {
        s = f.eft && f.eft->pmen ? f.eft->pmen : 4;
        nat = s < 1 ? 1 : (s > 8 ? 8 : s);
    } else if (f.ch == 1) {
        s = 1;
        nat = 1;
    } else {
        if (!(f.ph ? f.eft && f.eft->name : f.enm) || (f.eft && f.eft->k == PTK_STRING) || !ty_sizeof(f.eft, &s, depth + 1)) return 0;
        nat = s < 1 ? 1 : (s > 8 ? 8 : s);
    }
    *sz = s;
    if (r->packed) *al = 1;
    else if (r->pal) *al = s == 1 ? 1 : 2;
    else if (r->prec > 0 && nat > r->prec) *al = r->prec;
    else *al = nat;
    return 1;
}
static int ty_rec_layout(const pty_t *r, long long *size_out, long long *align_out, int depth) {
    if (!r || !r->lay || depth > 8) return 0;
    long long off = 0, maxal = 1;
    for (int j = 0; j < r->lay->nf; j++) { long long sz, al; if (!ty_rec_fld_sa(r, j, &sz, &al, depth)) return 0; if (al > maxal) maxal = al; if (al > 1) off = ((off + al - 1) / al) * al; off += sz; }
    long long total = r->packed ? off : r->pal ? ((off + 1) / 2) * 2 : ((off + maxal - 1) / maxal) * maxal;
    if (size_out) *size_out = total;
    if (align_out) *align_out = maxal;
    return 1;
}
int pty_rec_layout(const pty_t *r, long long *sz, long long *al) { return ty_rec_layout(r, sz, al, 0); }
int pty_rec_fld_sa(const pty_t *r, int idx, long long *sz, long long *al) { return ty_rec_fld_sa(r, idx, sz, al, 0); }
const pty_t *pty_type_by_name(const char *name) {
    struct pty_state *s = pt_st();
    if (!s || !s->ecur) return NULL;
    for (pscope_t *c = s->ecur; name && c; c = c->up) for (psym_t *p = c->s; p; p = p->next) if (p->k == 't' && p->name && !strcmp(p->name, name)) return p->t;
    return NULL;
}
const pty_t *pty_rec_by_name(const char *name) {
    struct pty_state *s = pt_st();
    if (!s || !name) return NULL;
    for (int i = s->nanon - 1; i >= 0; i--) if (!strcmp(s->anon[i].name, name)) return s->anon[i].t;
    const pty_t *t = pty_type_by_name(name);
    while (t && t->k == PTK_ARRAY) t = t->base;
    return t && t->k == PTK_RECORD && t->lay ? t : NULL;
}
void pty_note_recname(const char *name, const tree_t *recnode) {
    struct pty_state *s = pt_st();
    const pty_ent *e = s && recnode ? pt_map_get(&s->src, recnode) : NULL;
    if (!e || !e->t || e->t->k != PTK_RECORD || !name) return;
    if (s->nanon >= s->canon) { s->canon = s->canon ? s->canon * 2 : 32; s->anon = ct_grow(s->anon, (size_t)s->canon * sizeof *s->anon); }
    s->anon[s->nanon].name = name;
    s->anon[s->nanon++].t = e->t;
}
const pty_t *pty_pointee(const pty_t *t) { return t && t->k == PTK_POINTER ? pt_deref(pt_st(), t) : NULL; }
const char *pty_name_of(const pty_t *t) {
    struct pty_state *s = pt_st();
    if (!s || !t) return NULL;
    for (int i = s->nanon - 1; i >= 0; i--) if (s->anon[i].t == t) return s->anon[i].name;
    if (t->name && pty_type_by_name(t->name) == t) return t->name;
    if (t->k != PTK_RECORD) return t->name;
    char buf[32];
    snprintf(buf, sizeof buf, "$rec%d", s->nanon + 1);
    char *nm = (char *)ct_zalloc(strlen(buf) + 1, 1);
    memcpy(nm, buf, strlen(buf));
    if (s->nanon >= s->canon) { s->canon = s->canon ? s->canon * 2 : 32; s->anon = ct_grow(s->anon, (size_t)s->canon * sizeof *s->anon); }
    s->anon[s->nanon].name = nm;
    s->anon[s->nanon++].t = t;
    return nm;
}
static const pty_t *pt_sel_type(const tree_t *low, int depth) {
    const pty_t *t = pty_of_low(low);
    if (t || !low || depth > 16) return t;
    if (PT_IS(low, TT_VAR) && low->v.sval) return pty_name_type(low->v.sval);
    if (PT_IS(low, TT_FNC) && low->n >= 2 && low->c[0] && low->c[0]->v.sval && !strcmp(low->c[0]->v.sval, "__pas_deref")) {
        const pty_t *p = pt_sel_type(low->c[1], depth + 1);
        return p && p->k == PTK_POINTER ? pt_deref(pt_st(), p) : NULL;
    }
    if (PT_IS(low, TT_IDX) && low->n >= 2) {
        const pty_t *b = pt_sel_type(low->c[0], depth + 1);
        if (b && b->k == PTK_POINTER) b = pt_deref(pt_st(), b);
        if (!b) return NULL;
        if (b->k == PTK_ARRAY) return b->base;
        struct pty_fld f;
        if (b->k == PTK_RECORD && PT_IS(low->c[1], TT_ILIT) && pty_rec_fld(b, (int)low->c[1]->v.ival, &f)) return f.ft;
    }
    return NULL;
}
void pty_note_temp(const char *name, const tree_t *like) {
    struct pty_state *s = pt_st();
    const pty_t *t = s && s->ecur && name ? pt_sel_type(like, 0) : NULL;
    if (!t) return;
    psym_t *p = (psym_t *)ct_zalloc(1, sizeof *p);
    p->name = name;
    p->k = 'v';
    p->t = (pty_t *)t;
    p->next = s->ecur->s;
    s->ecur->s = p;
}
const char *pty_sel_rtype(const tree_t *low) {
    const pty_t *t = pt_sel_type(low, 0);
    if (t && t->k == PTK_POINTER) t = pt_deref(pt_st(), t);
    return t && t->k == PTK_RECORD && t->lay ? pty_name_of(t) : NULL;
}
int pty_rec_nf(const pty_t *r) { return r && r->lay ? r->lay->nf : 0; }
int pty_rec_fld(const pty_t *r, int i, struct pty_fld *o) {
    if (!r || !r->lay || i < 0 || i >= r->lay->nf) return 0;
    struct lfx f;
    lf_of(r->lay, i, &f);
    memset(o, 0, sizeof *o);
    o->ch = f.ch;
    o->ca = f.ca;
    o->na = f.na;
    o->file = f.file;
    o->ptr = f.ptr;
    o->rec = f.rec;
    o->en = f.en;
    o->calo = f.calo;
    o->cahi = f.cahi;
    o->nalo = f.nalo;
    o->nahi = f.nahi;
    o->ov = r->lay->ov[i];
    o->name = r->lay->name[i];
    o->ft = r->lay->ft[i];
    o->named = r->lay->named[i] & 1;
    o->eft = f.ft;
    o->enm = f.nm;
    o->ph = (r->lay->named[i] & 4) != 0;
    o->tag = (r->lay->named[i] & 2) != 0;
    return 1;
}
const char *pty_fld_typename(const pty_t *r, int i) {
    struct pty_fld f;
    if (!pty_rec_fld(r, i, &f) || !f.eft) return NULL;
    const pty_t *t = f.eft;
    int nm = f.ph ? t->name != NULL : f.enm, d = 0;
    if (!nm) while (t && t->k == PTK_ARRAY && t->dyn) { d++; t = t->base; }
    if (!nm && !d) while (t && t->k == PTK_ARRAY) t = t->base;
    const char *n = t && t->name ? t->name : NULL;
    if (!d || !n) return n;
    size_t len = strlen(n) + 16;
    char *m = (char *)ct_zalloc(len, 1);
    snprintf(m, len, "$dyn%d$%s", d, n);
    return m;
}
void pty_lay_check(const tree_t *rec, int nf, char **fields, int *fldchar, int *fldca, long long *ca_lo, long long *ca_hi, int *fldna, long long *na_lo, long long *na_hi, int *fldfile,
    char **fldptrto, char **fldrec, char **fldenum, long long *fldov) {
    struct pty_state *s = pt_st();
    if (!s || !g_lower.pas.dump_types) return;
    const pty_ent *e = pt_map_get(&s->src, rec);
    const pty_t *t = e ? e->t : NULL;
    s->stats[64 + 0]++;
    if (!t || t->k != PTK_RECORD || !t->lay) { fprintf(stderr, "PTYL-UNK line=%d nf=%d\n", rec ? rec->line : 0, nf); return; }
    const struct pty_lay *L = t->lay;
    int bad = 0;
    if (L->nf != nf) {
        fprintf(stderr, "PTYL nf legacy=%d typer=%d legacy-names:", nf, L->nf);
        for (int i = 0; i < nf; i++) fprintf(stderr, " %s", fields[i] ? fields[i] : "?");
        fprintf(stderr, " | typer-names:");
        for (int i = 0; i < L->nf; i++) fprintf(stderr, " %s", L->name[i]);
        fprintf(stderr, "\n");
        return;
    }
    for (int i = 0; i < nf; i++) {
        const pty_t *ft = L->ft[i];
        int ph = (L->named[i] & 4) != 0, nm = L->named[i] & 1;
        if (ph && i > 0) { ft = L->ft[i - 1]; nm = L->named[i - 1] & 1; }
        int ischar = 0, c = ph ? 0 : lf_eff_char(L, i);
        long long lo = 0, hi = -1;
        int ar = lf_arr(ft, &ischar, &lo, &hi);
        if (ar && ft->nix == 2 && !ft->dyn && !nm && ft->ix[1]) { lo = ft->ix[1]->lo; hi = ft->ix[1]->hi; }
        int ca = ar && ischar, na = ar && !ischar;
        if (ph && !nm) { ca = 0; na = 0; }
        if (ca && !nm && !ph) c = 1;
        if (ph) c = lf_eff_char(L, i);
        int file = lf_has_file(ft), ptr = !ph && ft && ft->k == PTK_POINTER && (ft->base || ft->fwd), rc = lf_rec(ft), en = ft && ft->k == PTK_ENUM && ft->name && ft->nf > 0;
        const char *what = NULL;
        if (!fields[i] || !L->name[i] || strcmp(fields[i], L->name[i])) what = "name";
        else if (fldchar[i] != c) what = "char";
        else if ((fldca[i] != 0) != ca) what = "ca";
        else if (ca && (ca_lo[i] != (lo > 0 ? lo : 0) || ca_hi[i] != hi)) what = "ca-bounds";
        else if ((fldna[i] != 0) != na) what = "na";
        else if (na && (na_lo[i] != lo || na_hi[i] != hi)) what = "na-bounds";
        else if ((fldfile[i] != 0) != file) what = "file";
        else if ((fldptrto[i] != NULL) != ptr) what = "ptr";
        else if ((fldrec[i] != NULL) != rc) what = "rec";
        else if ((fldenum[i] != NULL) != en) what = "enum";
        else if (fldov[i] != L->ov[i]) what = "ov";
        if (what && !strcmp(what, "ov")) {
            bad++;
            fprintf(stderr, "PTYL ov field=%s i=%d legacy=%llx typer=%llx kind=%s named=%d tname=%s lo=%lld hi=%lld\n", fields[i] ? fields[i] : "?", i, fldov[i], L->ov[i],
                ft ? pty_kind_name(ft->k) : "-", (int)L->named[i], ft && ft->name ? ft->name : "-", ft ? ft->lo : 0, ft ? ft->hi : 0);
        } else if (what) {
            bad++;
            fprintf(stderr,
                "PTYL %s line=%d field=%s legacy[char=%d ca=%d na=%d file=%d ptr=%d rec=%d enum=%d bounds=%lld..%lld/%lld..%lld] typer[kind=%s char=%d arr=%d ischar=%d %lld..%lld named=%d]\n", what,
                rec->line, fields[i] ? fields[i] : "?", fldchar[i], fldca[i], fldna[i], fldfile[i], fldptrto[i] != NULL, fldrec[i] != NULL, fldenum[i] != NULL, ca_lo[i], ca_hi[i], na_lo[i], na_hi[i],
                ft ? pty_kind_name(ft->k) : "-", c, ar, ischar, lo, hi, (int)L->named[i]);
        }
    }
    s->stats[64 + 1] += bad ? 0 : 1;
}
void pty_census_report(void) {
    struct pty_state *s = pt_st();
    if (!s || !g_lower.pas.dump_types) return;
    static const char *const nm[] = { "char", "string", "bool", "set" };
    for (int i = 0; i < 4; i++) fprintf(stderr, "PTY-STATS %s asked=%ld answered=%ld agreed=%ld\n", nm[i], s->stats[i], s->stats[4 + i], s->stats[8 + i]);
    fprintf(stderr, "PTYL-STATS checked=%ld clean=%ld\n", s->stats[64], s->stats[65]);
}
void pty_note_shortstring(const tree_t *leaf, const tree_t *bound) {
    struct pty_state *s = pt_mk();
    if (!leaf || !bound) return;
    pty_t *t = pt_new(PTK_STRING, "shortstring");
    t->bnd = bound;
    pt_map_put(&s->sstr, leaf, t, 0);
}
int pty_field_slot(const tree_t *low, const char *name) {
    const pty_t *t = pty_of_low(low);
    if (!t || t->k != PTK_RECORD || t->var || !name) return -1;
    for (int i = 0; i < t->nf; i++) if (t->f[i].type && t->f[i].type->k == PTK_ARRAY && (t->f[i].type->packed || (t->f[i].type->base && t->f[i].type->base->k == PTK_CHAR))) return -1;
    for (int i = 0; i < t->nf; i++) if (t->f[i].name && !strcmp(t->f[i].name, name)) return i;
    return -1;
}
const char *pty_enum_names(const tree_t *low) {
    const pty_t *t = pty_of_low(low);
    size_t l = 1;
    char *r, *w;
    for (int g = 0; t && (t->k != PTK_ENUM || t->nf <= 0) && t->base && g < 4; g++) t = t->base;
    if (!t || t->k != PTK_ENUM || t->nf <= 0) return NULL;
    for (int i = 0; i < t->nf; i++) l += strlen(t->f[i].name) + 1;
    r = w = (char *)ct_zalloc(1, l);
    for (int i = 0; i < t->nf; i++) { size_t n = strlen(t->f[i].name); if (i) *w++ = ','; memcpy(w, t->f[i].name, n); w += n; }
    return r;
}
int pty_capped(const tree_t *src) { const pty_t *t = pty_of_src(src); return (t && t->k == PTK_STRING && t->cap) ? t->strmax : 0; }
int pty_shortmax(const tree_t *low) {
    const pty_t *t = pty_of_low(low);
    return (t && t->k == PTK_STRING && t->strmax > 0 && t->strmax <= 255 && !strcmp(t->name ? t->name : "", "shortstring")) ? t->strmax : 0;
}
int pty_string_within(const tree_t *low, long long n) {
    const pty_t *t = pty_of_low(low);
    if (!t || (t->k != PTK_STRING && t->k != PTK_CHAR)) return 1;
    return t->k == PTK_CHAR || (t->strmax > 0 && t->strmax <= n);
}
static long long pt_size(const pty_t *t) {
    if (!t) return -1;
    if (t->k == PTK_STRING) return (t->name && !strcmp(t->name, "shortstring") && t->strmax > 0) ? (long long)t->strmax + 1 : -1;
    if (t->k != PTK_ARRAY || t->dyn || t->open || t->nix != 1 || !t->ix || !t->ix[0] || !t->ix[0]->bounded) return -1;
    long long e = pt_size(t->base);
    return e > 0 ? (t->ix[0]->hi - t->ix[0]->lo + 1) * e : -1;
}
long long pty_sizeof_low(const tree_t *low) { return pt_size(pty_of_low(low)); }
void lower_pascal_dump_types_on(void) { g_lower.pas.dump_types = 1; }
