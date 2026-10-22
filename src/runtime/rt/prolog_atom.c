#include "rt/rt_arena.h"
#include "rt_slab.h"
#include "rt/prolog_atom.h"
#include "g_runtime.h"
typedef g_atom_hentry_t HEntry;
typedef g_atom_fentry_t FEntry;
typedef g_atom_opcol_t OpCol;
typedef g_atom_opcols_t OpCols;
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define ATOM_INIT_CAP 8
#define HT_INIT_SIZE 8
extern void *rt_wsb_alloc(size_t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pool_carve(size_t n) {
    size_t need = sizeof(uint32_t) + n + 1;
    char *p;
    if (n >= (size_t)UINT32_MAX) return (char *)0;
    if (need > g_runtime.atoms.name_pool_left) {
        size_t chunk = need > 65536 ? need : 65536;
        char *c = (char *)rt_slab_try_region(chunk);
        if (!c) return (char *)0;
        g_runtime.atoms.name_pool = c;
        g_runtime.atoms.name_pool_left = chunk;
    }
    p = g_runtime.atoms.name_pool + sizeof(uint32_t);
    g_runtime.atoms.name_pool += need;
    g_runtime.atoms.name_pool_left -= need;
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pool_seal(char *p, size_t n, size_t k) {
    uint32_t len = (uint32_t)k;
    memcpy(p - sizeof(uint32_t), &len, sizeof len);
    p[k] = 0;
    if (g_runtime.atoms.name_pool == p + n + 1) { g_runtime.atoms.name_pool = p + k + 1; g_runtime.atoms.name_pool_left += n - k; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t pool_len(const char *p) { uint32_t len; memcpy(&len, p - sizeof(uint32_t), sizeof len); return len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned int ht_hash(const char *s, size_t n) { unsigned int h = 2166136261u; for (size_t i = 0; i < n; i++) { h ^= (unsigned char)s[i]; h *= 16777619u; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ht_grow(int new_size) {
    HEntry *old = g_runtime.atoms.ht;
    int old_size = g_runtime.atoms.ht_size;
    g_runtime.atoms.ht = rt_wsb_alloc(new_size * sizeof(HEntry));
    memset(g_runtime.atoms.ht, 0, new_size * sizeof(HEntry));
    g_runtime.atoms.ht_size = new_size;
    g_runtime.atoms.ht_used = 0;
    for (int i = 0; i < old_size; i++) {
        if (!old[i].key) continue;
        unsigned int h = ht_hash(old[i].key, pool_len(old[i].key)) & (g_runtime.atoms.ht_size - 1);
        while (g_runtime.atoms.ht[h].key) h = (h + 1) & (g_runtime.atoms.ht_size - 1);
        g_runtime.atoms.ht[h] = old[i];
        g_runtime.atoms.ht_used++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int atom_find(const char *s, size_t n, unsigned int *hp) {
    unsigned int h;
    if (!g_runtime.atoms.ht) {
        g_runtime.atoms.ht_size = HT_INIT_SIZE;
        g_runtime.atoms.ht = rt_wsb_alloc(g_runtime.atoms.ht_size * sizeof(HEntry));
        memset(g_runtime.atoms.ht, 0, g_runtime.atoms.ht_size * sizeof(HEntry));
    }
    if (!g_runtime.atoms.atom_names) {
        g_runtime.atoms.atom_cap = ATOM_INIT_CAP;
        g_runtime.atoms.atom_names = rt_wsb_alloc(g_runtime.atoms.atom_cap * sizeof(char *));
        memset(g_runtime.atoms.atom_names, 0, g_runtime.atoms.atom_cap * sizeof(char *));
    }
    h = ht_hash(s, n) & (g_runtime.atoms.ht_size - 1);
    while (g_runtime.atoms.ht[h].key) {
        if (pool_len(g_runtime.atoms.ht[h].key) == n && (!n || memcmp(g_runtime.atoms.ht[h].key, s, n) == 0)) return g_runtime.atoms.ht[h].id;
        h = (h + 1) & (g_runtime.atoms.ht_size - 1);
    }
    *hp = h;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int atom_add(char *copy, size_t n, unsigned int h) {
    if (g_runtime.atoms.ht_used * 2 >= g_runtime.atoms.ht_size) {
        ht_grow(g_runtime.atoms.ht_size * 2);
        h = ht_hash(copy, n) & (g_runtime.atoms.ht_size - 1);
        while (g_runtime.atoms.ht[h].key) h = (h + 1) & (g_runtime.atoms.ht_size - 1);
    }
    if (g_runtime.atoms.atom_len >= g_runtime.atoms.atom_cap) {
        int old_cap = g_runtime.atoms.atom_cap;
        g_runtime.atoms.atom_cap *= 2;
        {
            char **grown = (char **)rt_wsb_alloc((size_t)g_runtime.atoms.atom_cap * sizeof(char *));
            memcpy(grown, g_runtime.atoms.atom_names, (size_t)old_cap * sizeof(char *));
            g_runtime.atoms.atom_names = grown;
        }
        memset(g_runtime.atoms.atom_names + old_cap, 0, (g_runtime.atoms.atom_cap - old_cap) * sizeof(char *));
    }
    { int id = g_runtime.atoms.atom_len++; g_runtime.atoms.atom_names[id] = copy; g_runtime.atoms.ht[h].key = copy; g_runtime.atoms.ht[h].id = id; g_runtime.atoms.ht_used++; return id; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_atom_intern_n(const char *s, size_t n) {
    unsigned int h = 0;
    int id;
    char *p;
    if (!s) { s = ""; n = 0; }
    id = atom_find(s, n, &h);
    if (id >= 0) return id;
    p = pool_carve(n);
    if (!p) { fprintf(stderr, "scrip: the atom pool cannot hold an atom of %zu bytes\n", n); exit(2); }
    if (n) memcpy(p, s, n);
    pool_seal(p, n, n);
    return atom_add(p, n, h);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *prolog_atom_carve(size_t n) { return pool_carve(n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_atom_uncarve(char *p, size_t n) {
    if (p && g_runtime.atoms.name_pool == p + n + 1) { g_runtime.atoms.name_pool = p - sizeof(uint32_t); g_runtime.atoms.name_pool_left += sizeof(uint32_t) + n + 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_atom_carved(char *p, size_t n, size_t k) {
    unsigned int h = 0;
    int id;
    if (k > n) k = n;
    pool_seal(p, n, k);
    id = atom_find(p, k, &h);
    if (id >= 0) { prolog_atom_uncarve(p, k); return id; }
    return atom_add(p, k, h);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_atom_intern(const char *name) { if (!name) name = ""; return prolog_atom_intern_n(name, strlen(name)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_atom_intern_lit(const char *s) {
    size_t n;
    if (!s) s = "";
    n = strlen(s);
    if (!memchr(s, 0xC0, n)) return prolog_atom_intern_n(s, n);
    {
        char *b = pool_carve(n);
        size_t k = 0;
        if (!b) { fprintf(stderr, "scrip: the atom pool cannot hold an atom of %zu bytes\n", n); exit(2); }
        for (size_t i = 0; i < n; i++) { if ((unsigned char)s[i] == 0xC0 && (unsigned char)s[i + 1] == 0x80) { b[k++] = 0; i++; } else b[k++] = s[i]; }
        return prolog_atom_carved(b, n, k);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t prolog_atom_len(int id) { return (id < 0 || id >= g_runtime.atoms.atom_len) ? 0 : pool_len(g_runtime.atoms.atom_names[id]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *prolog_atom_name(int id) { if (id < 0 || id >= g_runtime.atoms.atom_len) return NULL; return g_runtime.atoms.atom_names[id]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_atom_count(void) { return g_runtime.atoms.atom_len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned int fht_hash(int name, int arity) { unsigned int h = 2166136261u; h ^= (unsigned int)name; h *= 16777619u; h ^= (unsigned int)arity; h *= 16777619u; return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fht_grow(int new_size) {
    int *old = g_runtime.atoms.fht, old_size = g_runtime.atoms.fht_size;
    g_runtime.atoms.fht = rt_wsb_alloc((size_t)new_size * sizeof(int));
    memset(g_runtime.atoms.fht, 0, (size_t)new_size * sizeof(int));
    g_runtime.atoms.fht_size = new_size;
    g_runtime.atoms.fht_used = 0;
    for (int i = 0; i < old_size; i++) {
        if (!old[i]) continue;
        unsigned int h = fht_hash(g_runtime.atoms.functors[old[i] - 1].name, g_runtime.atoms.functors[old[i] - 1].arity) & (unsigned int)(g_runtime.atoms.fht_size - 1);
        while (g_runtime.atoms.fht[h]) h = (h + 1) & (unsigned int)(g_runtime.atoms.fht_size - 1);
        g_runtime.atoms.fht[h] = old[i];
        g_runtime.atoms.fht_used++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_intern(int name, int arity) {
    if (name < 0 || arity < 0) return -1;
    if (!g_runtime.atoms.fht) {
        g_runtime.atoms.fht_size = HT_INIT_SIZE;
        g_runtime.atoms.fht = rt_wsb_alloc((size_t)g_runtime.atoms.fht_size * sizeof(int));
        memset(g_runtime.atoms.fht, 0, (size_t)g_runtime.atoms.fht_size * sizeof(int));
    }
    if (!g_runtime.atoms.functors) {
        g_runtime.atoms.functor_cap = ATOM_INIT_CAP;
        g_runtime.atoms.functors = rt_wsb_alloc((size_t)g_runtime.atoms.functor_cap * sizeof(FEntry));
        memset(g_runtime.atoms.functors, 0, (size_t)g_runtime.atoms.functor_cap * sizeof(FEntry));
    }
    unsigned int h = fht_hash(name, arity) & (unsigned int)(g_runtime.atoms.fht_size - 1);
    while (g_runtime.atoms.fht[h]) {
        if (g_runtime.atoms.functors[g_runtime.atoms.fht[h] - 1].name == name && g_runtime.atoms.functors[g_runtime.atoms.fht[h] - 1].arity == arity) return g_runtime.atoms.fht[h] - 1;
        h = (h + 1) & (unsigned int)(g_runtime.atoms.fht_size - 1);
    }
    if (g_runtime.atoms.fht_used * 2 >= g_runtime.atoms.fht_size) {
        fht_grow(g_runtime.atoms.fht_size * 2);
        h = fht_hash(name, arity) & (unsigned int)(g_runtime.atoms.fht_size - 1);
        while (g_runtime.atoms.fht[h]) h = (h + 1) & (unsigned int)(g_runtime.atoms.fht_size - 1);
    }
    if (g_runtime.atoms.functor_len >= g_runtime.atoms.functor_cap) {
        int old_cap = g_runtime.atoms.functor_cap;
        g_runtime.atoms.functor_cap *= 2;
        {
            FEntry *grown = (FEntry *)rt_wsb_alloc((size_t)g_runtime.atoms.functor_cap * sizeof(FEntry));
            memcpy(grown, g_runtime.atoms.functors, (size_t)old_cap * sizeof(FEntry));
            g_runtime.atoms.functors = grown;
        }
        memset(g_runtime.atoms.functors + old_cap, 0, (size_t)(g_runtime.atoms.functor_cap - old_cap) * sizeof(FEntry));
    }
    int id = g_runtime.atoms.functor_len++;
    g_runtime.atoms.functors[id].name = name;
    g_runtime.atoms.functors[id].arity = arity;
    g_runtime.atoms.fht[h] = id + 1;
    g_runtime.atoms.fht_used++;
    return id;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_name(int fid) { return (fid < 0 || fid >= g_runtime.atoms.functor_len) ? -1 : g_runtime.atoms.functors[fid].name; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_arity(int fid) { return (fid < 0 || fid >= g_runtime.atoms.functor_len) ? -1 : g_runtime.atoms.functors[fid].arity; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(sizeof(FEntry) == 8 && __builtin_offsetof(FEntry, arity) == 4,
    "rtx_pl_unify reads a compound's arity as g_runtime.atoms.functors[fid].arity at stride 8, offset 4 (ARCH-PROLOG-C-OUT-OF-THE-BOX 6.2)") ;
const void *rt_pl_functor_entries(void) { return (const void *)g_runtime.atoms.functors; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_count(void) { return g_runtime.atoms.functor_len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int op_type_code(const char *ty) {
    static const char names[][4] = { "xfx", "xfy", "yfx", "fy", "fx", "yf", "xf" };
    for (int k = 0; k < 7; k++) if (ty && !strcmp(ty, names[k])) return k + 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *prolog_op_type_name(int code) { static const char *const names[] = { "", "xfx", "xfy", "yfx", "fy", "fx", "yf", "xf" }; return (code >= 1 && code <= 7) ? names[code] : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static OpCol *op_col(OpCols *c, int fix) { return fix == 1 ? &c->in : fix == 2 ? &c->post : &c->pre; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_op_col_set_u(int atom, int prec, const char *ty, int user) {
    int code = op_type_code(ty), fix = code == 0 ? -1 : code <= 3 ? 1 : code <= 5 ? 0 : 2;
    if (atom < 0 || fix < 0) return;
    if (atom >= g_runtime.atoms.opcols_cap) {
        int cap = g_runtime.atoms.opcols_cap ? g_runtime.atoms.opcols_cap : ATOM_INIT_CAP;
        while (cap <= atom) cap *= 2;
        OpCols *grown = (OpCols *)rt_wsb_alloc((size_t)cap * sizeof(OpCols));
        memset(grown, 0, (size_t)cap * sizeof(OpCols));
        if (g_runtime.atoms.opcols) memcpy(grown, g_runtime.atoms.opcols, (size_t)g_runtime.atoms.opcols_cap * sizeof(OpCols));
        g_runtime.atoms.opcols = grown;
        g_runtime.atoms.opcols_cap = cap;
    }
    {
        OpCol *c = op_col(&g_runtime.atoms.opcols[atom], fix);
        c->p = (unsigned short)(prec > 0 ? prec : 0);
        c->t = (unsigned char)(prec > 0 ? code : 0);
        c->u = (unsigned char)(user == 2 ? 2 : user ? 1 : 0);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_op_col_set(int atom, int prec, const char *ty) { prolog_op_col_set_u(atom, prec, ty, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void op_cols_fill(void) {
    extern int prolog_op_table_count(void);
    extern int prolog_op_table_get(int, const char **, int *, const char **);
    static const struct {
        const char *n;
        const char *ty;
        int p;
    } pre[] = { { "dynamic", "fx", 1150 }, { "discontiguous", "fx", 1150 }, { "initialization", "fx", 1150 }, { "multifile", "fx", 1150 }, { "public", "fx", 1150 }, { "meta_predicate", "fx", 1150 },
        { "table", "fx", 1150 }, { 0, 0, 0 } };
    g_runtime.atoms.opcols_ready = 1;
    extern int prolog_op_user_count(void);
    extern int prolog_op_user_get(int, const char **, int *, const char **);
    for (int i = 0; pre[i].n; i++) prolog_op_col_set_u(prolog_atom_intern(pre[i].n), pre[i].p, pre[i].ty, 2);
    for (int i = 0, n = prolog_op_table_count(); i < n; i++) {
        const char *nm = 0, *ty = 0;
        int pr = 0;
        if (prolog_op_table_get(i, &nm, &pr, &ty) && nm && ty) prolog_op_col_set_u(prolog_atom_intern(nm), pr, ty, 0);
    }
    for (int i = 0, n = prolog_op_user_count(); i < n; i++) {
        const char *nm = 0, *ty = 0;
        int pr = 0;
        if (prolog_op_user_get(i, &nm, &pr, &ty) && nm && ty) prolog_op_col_set_u(prolog_atom_intern(nm), pr, ty, 1);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_col_get(int atom, int fix, int *prec_out, int *type_out) {
    if (!g_runtime.atoms.opcols_ready) op_cols_fill();
    if (atom < 0 || atom >= g_runtime.atoms.opcols_cap || fix < 0 || fix > 2 || !op_col(&g_runtime.atoms.opcols[atom], fix)->p) return 0;
    if (op_col(&g_runtime.atoms.opcols[atom], fix)->u == 2) { extern int rt_pl_dialect_gnu(void); if (rt_pl_dialect_gnu()) return 0; }
    if (prec_out) *prec_out = op_col(&g_runtime.atoms.opcols[atom], fix)->p;
    if (type_out) *type_out = op_col(&g_runtime.atoms.opcols[atom], fix)->t;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_col_user(int atom, int fix) { return (atom >= 0 && atom < g_runtime.atoms.opcols_cap && fix >= 0 && fix <= 2) ? op_col(&g_runtime.atoms.opcols[atom], fix)->u == 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_cols_ready(void) { return g_runtime.atoms.opcols_ready; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_functor_table_install(const long *tab) {
    if (!tab) return;
    long n = tab[0];
    const long *rows = tab + 1;
    for (long i = 0; i < n; i++) {
        int id = prolog_functor_intern((int)rows[2 * i], (int)rows[2 * i + 1]);
        if (id != (int)i) {
            fprintf(stderr,
                "scrip: the compiled functor table does not match the runtime's interner -- functor %ld (atom %ld/%ld) took id %d (a functor was interned before the table was installed; ARCH-PROLOG-"
                "BB-REWRITE.md section 3)\n", i, rows[2 * i], rows[2 * i + 1], id);
            exit(2);
        }
    }
    prolog_atom_init();
    if (g_runtime.atoms.atom_dot != 0 || g_runtime.atoms.atom_nil != 1 || g_runtime.atoms.atom_true != 2 || g_runtime.atoms.atom_fail != 3 || g_runtime.atoms.atom_cut != 4 ||
        g_runtime.atoms.functor_dot2 != 0) {
        fprintf(stderr,
            "scrip: the compiled atom and functor tables do not match the runtime's interner -- the init atoms . [] true fail ! read ids %d %d %d %d %d (0 1 2 3 4 expected) and ./2 reads functor %d "
            "(0 expected): the tables were not emitted by this compiler's interner (ARCH-PROLOG-BB-REWRITE.md section 3)\n", g_runtime.atoms.atom_dot, g_runtime.atoms.atom_nil,
            g_runtime.atoms.atom_true, g_runtime.atoms.atom_fail, g_runtime.atoms.atom_cut, g_runtime.atoms.functor_dot2);
        exit(2);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_atom_table_install(const long *tab) {
    if (!tab) return;
    long n = tab[0];
    if (!g_runtime.atoms.ht && n > 0) {
        int sz = HT_INIT_SIZE;
        while ((long)sz < 2 * n + 2) sz *= 2;
        g_runtime.atoms.ht_size = sz;
        g_runtime.atoms.ht = rt_wsb_alloc(g_runtime.atoms.ht_size * sizeof(HEntry));
        memset(g_runtime.atoms.ht, 0, g_runtime.atoms.ht_size * sizeof(HEntry));
    }
    if (!g_runtime.atoms.atom_names && n > 0) {
        g_runtime.atoms.atom_cap = ATOM_INIT_CAP;
        while ((long)g_runtime.atoms.atom_cap < n + 1) g_runtime.atoms.atom_cap *= 2;
        g_runtime.atoms.atom_names = rt_wsb_alloc(g_runtime.atoms.atom_cap * sizeof(char *));
        memset(g_runtime.atoms.atom_names, 0, g_runtime.atoms.atom_cap * sizeof(char *));
    }
    const long *rows = tab + 1;
    for (long i = 0; i < n; i++) {
        const char *nm = (const char *)rows[2 * i];
        int id = nm ? prolog_atom_intern_n(nm, (size_t)rows[2 * i + 1]) : prolog_atom_intern_n("", 0);
        if (id != (int)i) {
            fprintf(stderr,
                "scrip: the compiled atom table does not match the runtime's interner -- atom %ld '%s' took id %d (an atom was interned before the table was installed; ARCH-PROLOG-BB-REWRITE.md sect"
                "ion 3)\n", i, nm ? nm : "", id);
            exit(2);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_atom_init(void) {
    g_runtime.atoms.atom_dot = prolog_atom_intern(".");
    g_runtime.atoms.atom_nil = prolog_atom_intern("[]");
    g_runtime.atoms.atom_true = prolog_atom_intern("true");
    g_runtime.atoms.atom_fail = prolog_atom_intern("fail");
    g_runtime.atoms.atom_cut = prolog_atom_intern("!");
    g_runtime.atoms.functor_dot2 = prolog_functor_intern(g_runtime.atoms.atom_dot, 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_gc_roots(void) {
    extern void rt_gc_run_loc_list(const char *const *const *, long);
    static const char *const *const words[5] = { (const char *const *)&g_runtime.atoms.ht, (const char *const *)&g_runtime.atoms.atom_names, (const char *const *)&g_runtime.atoms.fht,
        (const char *const *)&g_runtime.atoms.functors, (const char *const *)&g_runtime.atoms.opcols };
    rt_gc_run_loc_list(words, 5);
}
