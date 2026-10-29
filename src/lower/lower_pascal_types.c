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
typedef struct { const void *key; const pty_t *t; int line; } pty_ent;
typedef struct { pty_ent *e; long n, cap; } pty_map;
struct pty_state {
    pty_map src, low, sstr;
    pscope_t *cur;
    int line, fpc;
    pty_t **pend;
    int npend, cpend;
    long *stats;
    pty_t *tint, *tchar, *tbool, *treal, *tstr, *tshort, *ttext, *tnil, *tvoid, *tptr, *tqword, *tfile, *tpchar;
};
static struct pty_state *pt_st(void) { return g_lower.pas.sem.pty; }
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
    static const char *const reals[] = { "real", "single", "double", "extended", "comp", "currency", "real48", "valreal", 0 };
    static const char *const strs[] = { "string", "ansistring", "unicodestring", "widestring", "rawbytestring", "utf8string", 0 };
    if (!n) return NULL;
    if (pt_in(n, ints)) return s->tint;
    if (pt_in(n, uints)) return pt_ord(PTK_INT, n, 0, 4294967295LL);
    if (!strcmp(n, "byte") || !strcmp(n, "uint8")) return pt_ord(PTK_INT, n, 0, 255);
    if (!strcmp(n, "shortint") || !strcmp(n, "int8")) return pt_ord(PTK_INT, n, -128, 127);
    if (!strcmp(n, "word") || !strcmp(n, "uint16")) return pt_ord(PTK_INT, n, 0, 65535);
    if (!strcmp(n, "smallint") || !strcmp(n, "int16")) return pt_ord(PTK_INT, n, -32768, 32767);
    if (!strcmp(n, "qword") || !strcmp(n, "uint64")) return s->tqword;
    if (pt_in(n, chars)) return s->tchar;
    if (pt_in(n, bools)) return s->tbool;
    if (pt_in(n, wbools)) { pty_t *t = pt_ord(PTK_BOOL, n, 0, 1); t->wbool = 1; return t; }
    if (pt_in(n, reals)) return s->treal;
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
            if (!strcmp(n->v.sval, "true")) { *v = 1; return 1; }
            if (!strcmp(n->v.sval, "false")) { *v = 0; return 1; }
            if (!strcmp(n->v.sval, "maxint")) { *v = 2147483647; return 1; }
            psym_t *p = ps_find(s, n->v.sval);
            if (p && (p->k == 'c' || p->k == 'e') && p->hv) { *v = p->iv; return 1; }
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
static void pt_rec_fields(struct pty_state *s, pty_t *r, const tree_t *body) {
    for (int i = 0; body && i < body->n; i++) {
        const tree_t *c = body->c[i];
        if (PT_IS(c, TT_DECL) && c->v.sval && !strcmp(c->v.sval, "field") && c->n >= 2) {
            pty_t *ft = pt_type(s, c->c[1]);
            const tree_t *ids = c->c[0];
            if (PT_IS(ids, TT_VLIST)) { for (int j = 0; j < ids->n; j++) if (ids->c[j]) pt_add_field(r, ids->c[j]->v.sval, ft); } else if (PT_IS(ids, TT_VAR)) pt_add_field(r, ids->v.sval, ft);
        } else if (PT_IS(c, TT_CASE)) {
            int k = 0;
            if (c->n >= 2 && PT_IS(c->c[0], TT_VAR) && PT_IS(c->c[1], TT_VAR)) { pt_add_field(r, c->c[0]->v.sval, pt_leaf(s, c->c[1])); k = 2; } else if (c->n >= 1 && PT_IS(c->c[0], TT_VAR)) k = 1;
            for (int j = k; j < c->n; j++) if (PT_IS(c->c[j], TT_ARM) && c->c[j]->n >= 2) pt_rec_fields(s, r, c->c[j]->c[1]);
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
            for (int i = 0; i < n->n; i++) { if (!n->c[i] || !n->c[i]->v.sval) continue; psym_t *p = ps_def(s, n->c[i]->v.sval, 'e', t); p->iv = i; p->hv = 1; }
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
            if (PT_IS(tg, TT_VAR) && tg->v.sval) { t->base = pt_leaf(s, tg); if (!t->base) { t->fwd = tg->v.sval; pt_pend(s, t); } } else t->base = pt_type(s, tg);
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
        { pty_t *t = pt_new(PTK_RECORD, NULL); t->packed = off; for (int i = off; i < n->n; i++) pt_rec_fields(s, t, n->c[i]); return t; }
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
    if (PT_IS(cal, TT_FIELD)) { pt_expr(s, cal->c[0]); return NULL; }
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
static pty_t *pt_catbound(struct pty_state *s, const pty_t *a, const pty_t *b) {
    long long la = pt_strlen_max(a), lb = pt_strlen_max(b);
    if (la <= 0 || lb <= 0) return s->tstr;
    pty_t *t = pt_strbound(la + lb > 255 ? 255 : la + lb);
    if (la + lb > 255 && s->fpc) t->cap = 1;
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
static pty_t *pt_expr(struct pty_state *s, const tree_t *n) { if (!n) return NULL; pty_t *t = pt_expr1(s, n); if (t) pt_map_put(&s->src, n, t, s->line); return t; }
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
void pty_run(tree_t *prog, int fpc) {
    struct pty_state *s = pt_mk();
    s->fpc = fpc;
    s->stats = (long *)ct_zalloc(12, sizeof(long));
    s->tint = pt_ord(PTK_INT, "integer", -2147483648LL, 2147483647LL);
    s->tqword = pt_ord(PTK_INT, "qword", 0, 0);
    s->tqword->uns = 1;
    s->tqword->bounded = 0;
    s->tchar = pt_ord(PTK_CHAR, "char", 0, 255);
    s->tbool = pt_ord(PTK_BOOL, "boolean", 0, 1);
    s->treal = pt_new(PTK_REAL, "real");
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
    for (int i = 0; prog && i < prog->n; i++) {
        const tree_t *c = prog->c[i];
        if (PT_IS(c, TT_VLIST)) for (int j = 0; j < c->n; j++) if (c->c[j] && c->c[j]->v.sval)
            ps_def(s, c->c[j]->v.sval, 'v', (!strcmp(c->c[j]->v.sval, "input") || !strcmp(c->c[j]->v.sval, "output")) ? s->ttext : NULL);
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
    if (t) pt_map_put(&s->low, low, t, e->line);
}
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
void pty_census(const char *pred, const tree_t *low, int legacy, int typer) {
    struct pty_state *s = pt_st();
    if (!s || !g_lower.pas.dump_types) return;
    int id = !strcmp(pred, "char") ? 0 : !strcmp(pred, "string") ? 1 : !strcmp(pred, "bool") ? 2 : 3;
    s->stats[id]++;
    if (typer < 0) return;
    s->stats[4 + id]++;
    if (typer == legacy) { s->stats[8 + id]++; return; }
    const pty_ent *x = pt_map_get(&s->low, low);
    const char *what = pt_what(low);
    fprintf(stderr, "PTY %s legacy=%d typer=%d line=%d kind=%s node=%d %s\n", pred, legacy, typer, x ? x->line : 0, x && x->t ? pty_kind_name(x->t->k) : "-", low ? (int)low->t : -1, what ? what : "");
}
void pty_census_report(void) {
    struct pty_state *s = pt_st();
    if (!s || !g_lower.pas.dump_types) return;
    static const char *const nm[] = { "char", "string", "bool", "set" };
    for (int i = 0; i < 4; i++) fprintf(stderr, "PTY-STATS %s asked=%ld answered=%ld agreed=%ld\n", nm[i], s->stats[i], s->stats[4 + i], s->stats[8 + i]);
}
void pty_note_shortstring(const tree_t *leaf, const tree_t *bound) {
    struct pty_state *s = pt_mk();
    if (!leaf || !bound) return;
    pty_t *t = pt_new(PTK_STRING, "shortstring");
    t->bnd = bound;
    pt_map_put(&s->sstr, leaf, t, 0);
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
