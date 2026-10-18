#include "rt/rt_arena.h"
#include "rt_slab.h"
#include "rt/prolog_atom.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int ATOM_DOT = -1;
int ATOM_NIL = -1;
int ATOM_TRUE = -1;
int ATOM_FAIL = -1;
int ATOM_CUT = -1;
int FUNCTOR_DOT2 = -1;
#define ATOM_INIT_CAP 8
static char **atom_names = NULL;
static int atom_len = 0;
static int atom_cap = 0;
#define HT_INIT_SIZE 8
extern void *rt_wsb_alloc(size_t);
typedef struct { char *key; int id; } HEntry;
static HEntry *ht = NULL;
static int ht_size = 0;
static int ht_used = 0;
static char *name_pool = NULL;
static size_t name_pool_left = 0;
typedef struct { int name; int arity; } FEntry;
static FEntry *functors = NULL;
static int functor_len = 0;
static int functor_cap = 0;
static int *fht = NULL;
static int fht_size = 0;
static int fht_used = 0;
typedef struct { unsigned short p; unsigned char t; unsigned char u; } OpCol;
typedef struct { OpCol pre; OpCol in; OpCol post; } OpCols;
static OpCols *opcols = NULL;
static int opcols_cap = 0;
static int opcols_ready = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pool_dup_n(const char *s, size_t n) {
    size_t need = sizeof(uint32_t) + n + 1;
    if (need > name_pool_left) { size_t chunk = need > 65536 ? need : 65536; name_pool = (char *)rt_slab_region(chunk); name_pool_left = chunk; }
    { uint32_t len = (uint32_t)n; char *p = name_pool + sizeof(uint32_t); memcpy(name_pool, &len, sizeof len); if (n) memcpy(p, s, n); p[n] = 0; name_pool += need; name_pool_left -= need; return p; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t pool_len(const char *p) { uint32_t len; memcpy(&len, p - sizeof(uint32_t), sizeof len); return len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned int ht_hash(const char *s, size_t n) { unsigned int h = 2166136261u; for (size_t i = 0; i < n; i++) { h ^= (unsigned char)s[i]; h *= 16777619u; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ht_grow(int new_size) {
    HEntry *old = ht;
    int old_size = ht_size;
    ht = rt_wsb_alloc(new_size * sizeof(HEntry));
    memset(ht, 0, new_size * sizeof(HEntry));
    ht_size = new_size;
    ht_used = 0;
    for (int i = 0; i < old_size; i++) {
        if (!old[i].key) continue;
        unsigned int h = ht_hash(old[i].key, pool_len(old[i].key)) & (ht_size - 1);
        while (ht[h].key) h = (h + 1) & (ht_size - 1);
        ht[h] = old[i];
        ht_used++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_atom_intern_n(const char *s, size_t n) {
    if (!s) { s = ""; n = 0; }
    if (!ht) { ht_size = HT_INIT_SIZE; ht = rt_wsb_alloc(ht_size * sizeof(HEntry)); memset(ht, 0, ht_size * sizeof(HEntry)); }
    if (!atom_names) { atom_cap = ATOM_INIT_CAP; atom_names = rt_wsb_alloc(atom_cap * sizeof(char *)); memset(atom_names, 0, atom_cap * sizeof(char *)); }
    unsigned int h = ht_hash(s, n) & (ht_size - 1);
    while (ht[h].key) { if (pool_len(ht[h].key) == n && (!n || memcmp(ht[h].key, s, n) == 0)) return ht[h].id; h = (h + 1) & (ht_size - 1); }
    if (ht_used * 2 >= ht_size) { ht_grow(ht_size * 2); h = ht_hash(s, n) & (ht_size - 1); while (ht[h].key) h = (h + 1) & (ht_size - 1); }
    if (atom_len >= atom_cap) {
        int old_cap = atom_cap;
        atom_cap *= 2;
        { char **grown = (char **)rt_wsb_alloc((size_t)atom_cap * sizeof(char *)); memcpy(grown, atom_names, (size_t)old_cap * sizeof(char *)); atom_names = grown; }
        memset(atom_names + old_cap, 0, (atom_cap - old_cap) * sizeof(char *));
    }
    char *copy = pool_dup_n(s, n);
    int id = atom_len++;
    atom_names[id] = copy;
    ht[h].key = copy;
    ht[h].id = id;
    ht_used++;
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
size_t prolog_atom_len(int id) { return (id < 0 || id >= atom_len) ? 0 : pool_len(atom_names[id]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *prolog_atom_name(int id) { if (id < 0 || id >= atom_len) return NULL; return atom_names[id]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_atom_count(void) { return atom_len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned int fht_hash(int name, int arity) { unsigned int h = 2166136261u; h ^= (unsigned int)name; h *= 16777619u; h ^= (unsigned int)arity; h *= 16777619u; return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void fht_grow(int new_size) {
    int *old = fht, old_size = fht_size;
    fht = rt_wsb_alloc((size_t)new_size * sizeof(int));
    memset(fht, 0, (size_t)new_size * sizeof(int));
    fht_size = new_size;
    fht_used = 0;
    for (int i = 0; i < old_size; i++) {
        if (!old[i]) continue;
        unsigned int h = fht_hash(functors[old[i] - 1].name, functors[old[i] - 1].arity) & (unsigned int)(fht_size - 1);
        while (fht[h]) h = (h + 1) & (unsigned int)(fht_size - 1);
        fht[h] = old[i];
        fht_used++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_intern(int name, int arity) {
    if (name < 0 || arity < 0) return -1;
    if (!fht) { fht_size = HT_INIT_SIZE; fht = rt_wsb_alloc((size_t)fht_size * sizeof(int)); memset(fht, 0, (size_t)fht_size * sizeof(int)); }
    if (!functors) { functor_cap = ATOM_INIT_CAP; functors = rt_wsb_alloc((size_t)functor_cap * sizeof(FEntry)); memset(functors, 0, (size_t)functor_cap * sizeof(FEntry)); }
    unsigned int h = fht_hash(name, arity) & (unsigned int)(fht_size - 1);
    while (fht[h]) { if (functors[fht[h] - 1].name == name && functors[fht[h] - 1].arity == arity) return fht[h] - 1; h = (h + 1) & (unsigned int)(fht_size - 1); }
    if (fht_used * 2 >= fht_size) { fht_grow(fht_size * 2); h = fht_hash(name, arity) & (unsigned int)(fht_size - 1); while (fht[h]) h = (h + 1) & (unsigned int)(fht_size - 1); }
    if (functor_len >= functor_cap) {
        int old_cap = functor_cap;
        functor_cap *= 2;
        { FEntry *grown = (FEntry *)rt_wsb_alloc((size_t)functor_cap * sizeof(FEntry)); memcpy(grown, functors, (size_t)old_cap * sizeof(FEntry)); functors = grown; }
        memset(functors + old_cap, 0, (size_t)(functor_cap - old_cap) * sizeof(FEntry));
    }
    int id = functor_len++;
    functors[id].name = name;
    functors[id].arity = arity;
    fht[h] = id + 1;
    fht_used++;
    return id;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_name(int fid) { return (fid < 0 || fid >= functor_len) ? -1 : functors[fid].name; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_arity(int fid) { return (fid < 0 || fid >= functor_len) ? -1 : functors[fid].arity; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(sizeof(FEntry) == 8 && __builtin_offsetof(FEntry, arity) == 4, "rtx_pl_unify reads a compound's arity as functors[fid].arity at stride 8, offset 4 (ARCH-PROLOG-C-OUT-OF-THE-BOX 6.2)");
const void *rt_pl_functor_entries(void) { return (const void *)functors; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_functor_count(void) { return functor_len; }
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
    if (atom >= opcols_cap) {
        int cap = opcols_cap ? opcols_cap : ATOM_INIT_CAP;
        while (cap <= atom) cap *= 2;
        OpCols *grown = (OpCols *)rt_wsb_alloc((size_t)cap * sizeof(OpCols));
        memset(grown, 0, (size_t)cap * sizeof(OpCols));
        if (opcols) memcpy(grown, opcols, (size_t)opcols_cap * sizeof(OpCols));
        opcols = grown;
        opcols_cap = cap;
    }
    { OpCol *c = op_col(&opcols[atom], fix); c->p = (unsigned short)(prec > 0 ? prec : 0); c->t = (unsigned char)(prec > 0 ? code : 0); c->u = (unsigned char)(user == 2 ? 2 : user ? 1 : 0); }
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
    opcols_ready = 1;
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
    if (!opcols_ready) op_cols_fill();
    if (atom < 0 || atom >= opcols_cap || fix < 0 || fix > 2 || !op_col(&opcols[atom], fix)->p) return 0;
    if (op_col(&opcols[atom], fix)->u == 2) { extern int rt_pl_dialect_gnu(void); if (rt_pl_dialect_gnu()) return 0; }
    if (prec_out) *prec_out = op_col(&opcols[atom], fix)->p;
    if (type_out) *type_out = op_col(&opcols[atom], fix)->t;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_col_user(int atom, int fix) { return (atom >= 0 && atom < opcols_cap && fix >= 0 && fix <= 2) ? op_col(&opcols[atom], fix)->u == 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_cols_ready(void) { return opcols_ready; }
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
    if (ATOM_DOT != 0 || ATOM_NIL != 1 || ATOM_TRUE != 2 || ATOM_FAIL != 3 || ATOM_CUT != 4 || FUNCTOR_DOT2 != 0) {
        fprintf(stderr,
            "scrip: the compiled atom and functor tables do not match the runtime's interner -- the init atoms . [] true fail ! read ids %d %d %d %d %d (0 1 2 3 4 expected) and ./2 reads functor %d "
            "(0 expected): the tables were not emitted by this compiler's interner (ARCH-PROLOG-BB-REWRITE.md section 3)\n", ATOM_DOT, ATOM_NIL, ATOM_TRUE, ATOM_FAIL, ATOM_CUT, FUNCTOR_DOT2);
        exit(2);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_atom_table_install(const long *tab) {
    if (!tab) return;
    long n = tab[0];
    if (!ht && n > 0) { int sz = HT_INIT_SIZE; while ((long)sz < 2 * n + 2) sz *= 2; ht_size = sz; ht = rt_wsb_alloc(ht_size * sizeof(HEntry)); memset(ht, 0, ht_size * sizeof(HEntry)); }
    if (!atom_names && n > 0) {
        atom_cap = ATOM_INIT_CAP;
        while ((long)atom_cap < n + 1) atom_cap *= 2;
        atom_names = rt_wsb_alloc(atom_cap * sizeof(char *));
        memset(atom_names, 0, atom_cap * sizeof(char *));
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
    ATOM_DOT = prolog_atom_intern(".");
    ATOM_NIL = prolog_atom_intern("[]");
    ATOM_TRUE = prolog_atom_intern("true");
    ATOM_FAIL = prolog_atom_intern("fail");
    ATOM_CUT = prolog_atom_intern("!");
    FUNCTOR_DOT2 = prolog_functor_intern(ATOM_DOT, 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_gc_roots(void) {
    extern void rt_gc_run_loc_list(const char *const *const *, long);
    static const char *const *const words[5] = { (const char *const *)&ht, (const char *const *)&atom_names, (const char *const *)&fht, (const char *const *)&functors, (const char *const *)&opcols };
    rt_gc_run_loc_list(words, 5);
}
