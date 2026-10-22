#include "rt/rt_arena.h"
#include "rt_slab.h"
#include "rt/prolog_atom.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int g_atom_dot = -1;
int g_atom_nil = -1;
int g_atom_true = -1;
int g_atom_fail = -1;
int g_atom_cut = -1;
int g_functor_dot2 = -1;
#define ATOM_INIT_CAP 8
static char **g_atom_names = NULL;
static int g_atom_len = 0;
static int g_atom_cap = 0;
#define HT_INIT_SIZE 8
extern void *rt_wsb_alloc(size_t);
typedef struct { char *key; int id; } HEntry;
static HEntry *g_ht = NULL;
static int g_ht_size = 0;
static int g_ht_used = 0;
static char *g_name_pool = NULL;
static size_t g_name_pool_left = 0;
typedef struct { int name; int arity; } FEntry;
static FEntry *g_functors = NULL;
static int g_functor_len = 0;
static int g_functor_cap = 0;
static int *g_fht = NULL;
static int g_fht_size = 0;
static int g_fht_used = 0;
typedef struct { unsigned short p; unsigned char t; unsigned char u; } OpCol;
typedef struct { OpCol pre; OpCol in; OpCol post; } OpCols;
static OpCols *g_opcols = NULL;
static int g_opcols_cap = 0;
static int g_opcols_ready = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pool_dup_n(const char *s, size_t n) {
    size_t need = sizeof(uint32_t) + n + 1;
    if (need > g_name_pool_left) { size_t chunk = need > 65536 ? need : 65536; g_name_pool = (char *)rt_slab_region(chunk); g_name_pool_left = chunk; }
    {
        uint32_t len = (uint32_t)n;
        char *p = g_name_pool + sizeof(uint32_t);
        memcpy(g_name_pool, &len, sizeof len);
        if (n) memcpy(p, s, n);
        p[n] = 0;
        g_name_pool += need;
        g_name_pool_left -= need;
        return p;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t pool_len(const char *p) { uint32_t len; memcpy(&len, p - sizeof(uint32_t), sizeof len); return len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned int ht_hash(const char *s, size_t n) { unsigned int h = 2166136261u; for (size_t i = 0; i < n; i++) { h ^= (unsigned char)s[i]; h *= 16777619u; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ht_grow(int new_size) {
    HEntry *old = g_ht;
    int old_size = g_ht_size;
    g_ht = rt_wsb_alloc(new_size * sizeof(HEntry));
    memset(g_ht, 0, new_size * sizeof(HEntry));
    g_ht_size = new_size;
    g_ht_used = 0;
    for (int i = 0; i < old_size; i++) {
        if (!old[i].key) continue;
        unsigned int h = ht_hash(old[i].key, pool_len(old[i].key)) & (g_ht_size - 1);
        while (g_ht[h].key) h = (h + 1) & (g_ht_size - 1);
        g_ht[h] = old[i];
        g_ht_used++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_atom_intern_n(const char *s, size_t n) {
    if (!s) { s = ""; n = 0; }
    if (!g_ht) { g_ht_size = HT_INIT_SIZE; g_ht = rt_wsb_alloc(g_ht_size * sizeof(HEntry)); memset(g_ht, 0, g_ht_size * sizeof(HEntry)); }
    if (!g_atom_names) { g_atom_cap = ATOM_INIT_CAP; g_atom_names = rt_wsb_alloc(g_atom_cap * sizeof(char *)); memset(g_atom_names, 0, g_atom_cap * sizeof(char *)); }
    unsigned int h = ht_hash(s, n) & (g_ht_size - 1);
    while (g_ht[h].key) { if (pool_len(g_ht[h].key) == n && (!n || memcmp(g_ht[h].key, s, n) == 0)) return g_ht[h].id; h = (h + 1) & (g_ht_size - 1); }
    if (g_ht_used * 2 >= g_ht_size) { ht_grow(g_ht_size * 2); h = ht_hash(s, n) & (g_ht_size - 1); while (g_ht[h].key) h = (h + 1) & (g_ht_size - 1); }
    if (g_atom_len >= g_atom_cap) {
        int old_cap = g_atom_cap;
        g_atom_cap *= 2;
        { char **grown = (char **)rt_wsb_alloc((size_t)g_atom_cap * sizeof(char *)); memcpy(grown, g_atom_names, (size_t)old_cap * sizeof(char *)); g_atom_names = grown; }
        memset(g_atom_names + old_cap, 0, (g_atom_cap - old_cap) * sizeof(char *));
    }
    char *copy = pool_dup_n(s, n);
    int id = g_atom_len++;
    g_atom_names[id] = copy;
    g_ht[h].key = copy;
    g_ht[h].id = id;
    g_ht_used++;
    return id;
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
        char b[n + 1];
        size_t k = 0;
        for (size_t i = 0; i < n; i++) { if ((unsigned char)s[i] == 0xC0 && (unsigned char)s[i + 1] == 0x80) { b[k++] = 0; i++; } else b[k++] = s[i]; }
        return prolog_atom_intern_n(b, k);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t prolog_atom_len(int id) { return (id < 0 || id >= g_atom_len) ? 0 : pool_len(g_atom_names[id]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *prolog_atom_name(int id) { if (id < 0 || id >= g_atom_len) return NULL; return g_atom_names[id]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_atom_count(void) { return g_atom_len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned int fht_hash(int name, int arity) { unsigned int h = 2166136261u; h ^= (unsigned int)name; h *= 16777619u; h ^= (unsigned int)arity; h *= 16777619u; return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fht_grow(int new_size) {
    int *old = g_fht, old_size = g_fht_size;
    g_fht = rt_wsb_alloc((size_t)new_size * sizeof(int));
    memset(g_fht, 0, (size_t)new_size * sizeof(int));
    g_fht_size = new_size;
    g_fht_used = 0;
    for (int i = 0; i < old_size; i++) {
        if (!old[i]) continue;
        unsigned int h = fht_hash(g_functors[old[i] - 1].name, g_functors[old[i] - 1].arity) & (unsigned int)(g_fht_size - 1);
        while (g_fht[h]) h = (h + 1) & (unsigned int)(g_fht_size - 1);
        g_fht[h] = old[i];
        g_fht_used++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_intern(int name, int arity) {
    if (name < 0 || arity < 0) return -1;
    if (!g_fht) { g_fht_size = HT_INIT_SIZE; g_fht = rt_wsb_alloc((size_t)g_fht_size * sizeof(int)); memset(g_fht, 0, (size_t)g_fht_size * sizeof(int)); }
    if (!g_functors) { g_functor_cap = ATOM_INIT_CAP; g_functors = rt_wsb_alloc((size_t)g_functor_cap * sizeof(FEntry)); memset(g_functors, 0, (size_t)g_functor_cap * sizeof(FEntry)); }
    unsigned int h = fht_hash(name, arity) & (unsigned int)(g_fht_size - 1);
    while (g_fht[h]) { if (g_functors[g_fht[h] - 1].name == name && g_functors[g_fht[h] - 1].arity == arity) return g_fht[h] - 1; h = (h + 1) & (unsigned int)(g_fht_size - 1); }
    if (g_fht_used * 2 >= g_fht_size) { fht_grow(g_fht_size * 2); h = fht_hash(name, arity) & (unsigned int)(g_fht_size - 1); while (g_fht[h]) h = (h + 1) & (unsigned int)(g_fht_size - 1); }
    if (g_functor_len >= g_functor_cap) {
        int old_cap = g_functor_cap;
        g_functor_cap *= 2;
        { FEntry *grown = (FEntry *)rt_wsb_alloc((size_t)g_functor_cap * sizeof(FEntry)); memcpy(grown, g_functors, (size_t)old_cap * sizeof(FEntry)); g_functors = grown; }
        memset(g_functors + old_cap, 0, (size_t)(g_functor_cap - old_cap) * sizeof(FEntry));
    }
    int id = g_functor_len++;
    g_functors[id].name = name;
    g_functors[id].arity = arity;
    g_fht[h] = id + 1;
    g_fht_used++;
    return id;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_name(int fid) { return (fid < 0 || fid >= g_functor_len) ? -1 : g_functors[fid].name; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_arity(int fid) { return (fid < 0 || fid >= g_functor_len) ? -1 : g_functors[fid].arity; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(sizeof(FEntry) == 8 && __builtin_offsetof(FEntry, arity) == 4, "rtx_pl_unify reads a compound's arity as g_functors[fid].arity at stride 8, offset 4 (ARCH-PROLOG-C-OUT-OF-THE-BOX 6.2)")
    ;
const void *rt_pl_functor_entries(void) { return (const void *)g_functors; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_count(void) { return g_functor_len; }
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
    if (atom >= g_opcols_cap) {
        int cap = g_opcols_cap ? g_opcols_cap : ATOM_INIT_CAP;
        while (cap <= atom) cap *= 2;
        OpCols *grown = (OpCols *)rt_wsb_alloc((size_t)cap * sizeof(OpCols));
        memset(grown, 0, (size_t)cap * sizeof(OpCols));
        if (g_opcols) memcpy(grown, g_opcols, (size_t)g_opcols_cap * sizeof(OpCols));
        g_opcols = grown;
        g_opcols_cap = cap;
    }
    { OpCol *c = op_col(&g_opcols[atom], fix); c->p = (unsigned short)(prec > 0 ? prec : 0); c->t = (unsigned char)(prec > 0 ? code : 0); c->u = (unsigned char)(user == 2 ? 2 : user ? 1 : 0); }
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
    g_opcols_ready = 1;
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
    if (!g_opcols_ready) op_cols_fill();
    if (atom < 0 || atom >= g_opcols_cap || fix < 0 || fix > 2 || !op_col(&g_opcols[atom], fix)->p) return 0;
    if (op_col(&g_opcols[atom], fix)->u == 2) { extern int rt_pl_dialect_gnu(void); if (rt_pl_dialect_gnu()) return 0; }
    if (prec_out) *prec_out = op_col(&g_opcols[atom], fix)->p;
    if (type_out) *type_out = op_col(&g_opcols[atom], fix)->t;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_col_user(int atom, int fix) { return (atom >= 0 && atom < g_opcols_cap && fix >= 0 && fix <= 2) ? op_col(&g_opcols[atom], fix)->u == 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_cols_ready(void) { return g_opcols_ready; }
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
    if (g_atom_dot != 0 || g_atom_nil != 1 || g_atom_true != 2 || g_atom_fail != 3 || g_atom_cut != 4 || g_functor_dot2 != 0) {
        fprintf(stderr,
            "scrip: the compiled atom and functor tables do not match the runtime's interner -- the init atoms . [] true fail ! read ids %d %d %d %d %d (0 1 2 3 4 expected) and ./2 reads functor %d "
            "(0 expected): the tables were not emitted by this compiler's interner (ARCH-PROLOG-BB-REWRITE.md section 3)\n", g_atom_dot, g_atom_nil, g_atom_true, g_atom_fail, g_atom_cut,
            g_functor_dot2);
        exit(2);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_atom_table_install(const long *tab) {
    if (!tab) return;
    long n = tab[0];
    if (!g_ht && n > 0) { int sz = HT_INIT_SIZE; while ((long)sz < 2 * n + 2) sz *= 2; g_ht_size = sz; g_ht = rt_wsb_alloc(g_ht_size * sizeof(HEntry)); memset(g_ht, 0, g_ht_size * sizeof(HEntry)); }
    if (!g_atom_names && n > 0) {
        g_atom_cap = ATOM_INIT_CAP;
        while ((long)g_atom_cap < n + 1) g_atom_cap *= 2;
        g_atom_names = rt_wsb_alloc(g_atom_cap * sizeof(char *));
        memset(g_atom_names, 0, g_atom_cap * sizeof(char *));
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
    g_atom_dot = prolog_atom_intern(".");
    g_atom_nil = prolog_atom_intern("[]");
    g_atom_true = prolog_atom_intern("true");
    g_atom_fail = prolog_atom_intern("fail");
    g_atom_cut = prolog_atom_intern("!");
    g_functor_dot2 = prolog_functor_intern(g_atom_dot, 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_gc_roots(void) {
    extern void rt_gc_run_loc_list(const char *const *const *, long);
    static const char *const *const words[5] = { (const char *const *)&g_ht, (const char *const *)&g_atom_names, (const char *const *)&g_fht, (const char *const *)&g_functors,
        (const char *const *)&g_opcols };
    rt_gc_run_loc_list(words, 5);
}
