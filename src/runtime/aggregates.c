#include "rt/rt_arena.h"
#include "rt/gc_heap.h"
#include "core.h"
#include "sil_macros.h"
#include <string.h>
static unsigned  _tbl_nbuck_for(int init);
static struct _TBBUCK_t **_tbl_vec_new(unsigned nb);
static long g_agg_list_ser = 1;
static long g_agg_table_ser = 1;
static long g_agg_set_ser = 1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_agg_serial_list(void) { return g_agg_list_ser++; }
long rt_agg_serial_table(void) { return g_agg_table_ser++; }
long rt_agg_serial_set(void) { return g_agg_set_ser++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ARBLK_t *array_new(int lo, int hi) {
    ARBLK_t *a = rt_ws_alloc_tag(sizeof(ARBLK_t), HB_ARR);
    a->lo   = lo;
    a->hi   = hi;
    a->ndim = 1;
    a->id   = g_agg_list_ser++;
    a->proto = (const char *)0;
    int sz  = hi - lo + 1;
    if (sz < 1) sz = 1;
    a->data = rt_ws_alloc(sz * sizeof(DESCR_t));
    for (int i = 0; i < sz; i++) a->data[i] = NULVCL;
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ARBLK_t *array_new2d(int lo1, int hi1, int lo2, int hi2) {
    ARBLK_t *a = rt_ws_alloc_tag(sizeof(ARBLK_t), HB_ARR);
    a->lo   = lo1;
    a->hi   = hi1;
    a->lo2  = lo2;
    a->hi2  = hi2;
    a->ndim = 2;
    a->id   = g_agg_list_ser++;
    a->proto = (const char *)0;
    int rows = hi1 - lo1 + 1;
    int cols = hi2 - lo2 + 1;
    if (rows < 1) rows = 1;
    if (cols < 1) cols = 1;
    a->data = rt_ws_alloc(rows * cols * sizeof(DESCR_t));
    for (int i = 0; i < rows * cols; i++) a->data[i] = NULVCL;
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t array_get(ARBLK_t *a, int i) {
    if (!a) return FAILDESCR;
    int idx = i - a->lo;
    if (idx < 0 || idx >= (a->hi - a->lo + 1)) return FAILDESCR;
    return a->data[idx];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void array_set(ARBLK_t *a, int i, DESCR_t v) {
    if (!a) return;
    int idx = i - a->lo;
    if (idx < 0 || idx >= (a->hi - a->lo + 1)) return;
    a->data[idx] = v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t array_get2(ARBLK_t *a, int i, int j) {
    if (!a) return FAILDESCR;
    int cols = a->hi2 - a->lo2 + 1;
    int row  = i - a->lo;
    int col  = j - a->lo2;
    int idx  = row * cols + col;
    int total = (a->hi - a->lo + 1) * cols;
    if (row < 0 || row >= (a->hi - a->lo + 1) || col < 0 || col >= cols || idx < 0 || idx >= total)
        return FAILDESCR;
    return a->data[idx];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void array_set2(ARBLK_t *a, int i, int j, DESCR_t v) {
    if (!a) return;
    int cols = a->hi2 - a->lo2 + 1;
    int row  = i - a->lo;
    int col  = j - a->lo2;
    int idx  = row * cols + col;
    a->data[idx] = v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static TBBUCK_t **_tbl_vec_new(unsigned nb) {
    TBBUCK_t **v = rt_gcheap_alloc(HB_AGGB, (unsigned long long)nb * sizeof(TBBUCK_t *));
    memset(v, 0, (size_t)nb * sizeof(TBBUCK_t *));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *table_new(void) {
    TBBLK_t *t = rt_agg_alloc(2, sizeof(TBBLK_t));
    t->id   = g_agg_table_ser++;
    t->size = 0;
    t->init = 11;
    t->inc  = 10;
    t->is_set = 0;
    t->nbuck = _tbl_nbuck_for(t->init);
    t->buckets = _tbl_vec_new(t->nbuck);
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *table_new_args(int init, int inc) {
    TBBLK_t *t = table_new();
    if (inc  > 0) t->inc  = inc;
    if (init > 0) {
        t->init  = init;
        unsigned nb = _tbl_nbuck_for(init);
        if (nb != t->nbuck) { t->nbuck = nb; t->buckets = _tbl_vec_new(nb); }
    }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t agg_prototype(DESCR_t v) {
    if (IS_TBL(v) && v.tbl) return INTVAL(v.tbl->init);
    if (!(IS_ARR(v) && v.arr)) { core_runtime_error(164, "prototype argument is not valid object"); return FAILDESCR; }
    ARBLK_t *a = v.arr;
    char pb[64];
    const char *p = a->proto;
    if (!p) {
        if (a->ndim > 1) snprintf(pb, sizeof pb, "%d,%d", a->hi - a->lo + 1, a->hi2 - a->lo2 + 1);
        else if (a->lo == 1) snprintf(pb, sizeof pb, "%d", a->hi);
        else snprintf(pb, sizeof pb, "%d:%d", a->lo, a->hi);
        p = pb;
    }
    int alldig = (p[0] != 0);
    for (const char *q = p; *q; q++) if (*q < '0' || *q > '9') { alldig = 0; break; }
    if (!alldig) return STRVAL(rt_ws_strdup_c(p));
    long long iv = 0;
    for (const char *q = p; *q; q++) iv = iv * 10 + (*q - '0');
    return INTVAL(iv);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *tbl_key_str(DESCR_t kd, char *buf, size_t bufn) {
    switch (kd.v) {
        case DT_SNUL: return "\001n";
        case DT_S:    return kd.s ? kd.s : "";
        case DT_I:    { char *p = buf; *p++ = '\001'; *p++ = 'i'; long long v = (long long)kd.i; unsigned long long u; if (v < 0) { *p++ = '-'; u = (unsigned long long)(-(v + 1)) + 1ull; } else u = (unsigned long long)v;
                        char t[24]; int n = 0; do { t[n++] = (char)('0' + (int)(u % 10ull)); u /= 10ull; } while (u); while (n) *p++ = t[--n]; *p = 0; (void)bufn; return buf; }
        case DT_R:    snprintf(buf, bufn, "\001r%.17g", kd.r); return buf;
        case DT_DATA: { if (!kd.u) return "\001d0"; snprintf(buf, bufn, "\001d%s#%ld", kd.u->type ? kd.u->type->name : "?", kd.u->id); return buf; }
        case DT_A:    { if (!kd.arr) return "\001l0"; if (!kd.arr->id) kd.arr->id = g_agg_list_ser++; snprintf(buf, bufn, "\001l%ld", kd.arr->id); return buf; }
        case DT_T:    { if (!kd.tbl) return "\001t0"; if (!kd.tbl->id) kd.tbl->id = g_agg_table_ser++; snprintf(buf, bufn, "\001%c%ld", kd.tbl->is_set ? 'S' : 't', kd.tbl->id); return buf; }
        default:      snprintf(buf, bufn, "\001p%p", kd.ptr); return buf;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int tbl_typed_off(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_TBL_TYPED"); v = (e && *e == '0') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned long long _tbl_rotl(unsigned long long u, int n) { return (u << n) | (u >> (64 - n)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned long long _tbl_h_snul(const DESCR_t *k) { (void)k; return 0x2F1B3D5C7E9A11ull; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned _tbl_slen(const DESCR_t *k) { return (k->slen != 0xFFFFFFFFu) ? k->slen : (k->s ? (unsigned)__builtin_strlen(k->s) : 0u); }
static inline __attribute__((always_inline)) unsigned long long _tbl_h_str(const DESCR_t *k) {
    const unsigned char *p = (const unsigned char *)(k->s ? k->s : ""); unsigned n = k->s ? _tbl_slen(k) : 0u, m = n;
    unsigned long long h = 5381ull ^ ((unsigned long long)n * 0x9E3779B97F4A7C15ull), w;
    while (m >= 8u) { h = (h ^ *(const unsigned long long *)p) * 0xFF51AFD7ED558CCDull; p += 8; m -= 8u; }
    if (m) { w = (m >= 4u) ? ((unsigned long long)*(const unsigned *)p | ((unsigned long long)*(const unsigned *)(p + m - 4u) << 32))
                : (m >= 2u) ? ((unsigned long long)*(const unsigned short *)p | ((unsigned long long)*(const unsigned short *)(p + m - 2u) << 32))
                : (unsigned long long)*p;
             h = (h ^ w) * 0xFF51AFD7ED558CCDull; }
    return ((h ^ (h >> 31)) * 0xC4CEB9FE1A85EC53ull) >> 8;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned long long _tbl_h_int(const DESCR_t *k) { return ((unsigned long long)(long long)k->i * 0x9E3779B97F4A7C15ull) >> 8; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned long long _tbl_h_real(const DESCR_t *k) {
    union { double d; unsigned long long u; } cv; cv.d = k->r;
    return ((cv.u ^ (cv.u >> 32)) * 0xBF58476D1CE4E5B9ull) >> 8;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned long long _tbl_h_arr(const DESCR_t *k) {
    if (!k->arr) return 0x51ED270B2C1A33ull;
    if (!k->arr->id) k->arr->id = g_agg_list_ser++;
    return (_tbl_rotl((unsigned long long)k->arr->id, 21) * 0xD6E8FEB86659FD93ull) >> 8;
}
static inline __attribute__((always_inline)) unsigned long long _tbl_h_tbl(const DESCR_t *k) {
    if (!k->tbl) return 0x1C69B3F74AC4Aull;
    if (!k->tbl->id) k->tbl->id = g_agg_table_ser++;
    return (_tbl_rotl((unsigned long long)k->tbl->id, 43) * 0xD6E8FEB86659FD93ull) >> 8;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned long long _tbl_h_data(const DESCR_t *k) {
    if (!k->u) return 0x3A5C17E9B24D6Full;
    return (((unsigned long long)k->u->id ^ _tbl_rotl((unsigned long long)(uintptr_t)k->u->type, 32)) * 0xFF51AFD7ED558CCDull) >> 8;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned long long _tbl_h_ptr(const DESCR_t *k) { return (((unsigned long long)(uintptr_t)k->ptr >> 4) * 0xC2B2AE3D27D4EB4Full) >> 8; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned long long _tbl_hval(const DESCR_t *k) {
    switch (k->v) {
        case DT_SNUL: return _tbl_h_snul(k);
        case DT_S:    return _tbl_h_str (k);
        case DT_I:    return _tbl_h_int (k);
        case DT_R:    return _tbl_h_real(k);
        case DT_A:    return _tbl_h_arr (k);
        case DT_T:    return _tbl_h_tbl (k);
        case DT_DATA: return _tbl_h_data(k);
        default:      return _tbl_h_ptr (k);
    }
}
static inline __attribute__((always_inline)) unsigned long long _tbl_hkey(DESCR_t k) {
    if (tbl_typed_off()) { char kb[64]; DESCR_t sk = k; sk.v = DT_S; sk.s = (char *)tbl_key_str(k, kb, sizeof kb); return ((unsigned long long)DT_S << 56) | _tbl_h_str(&sk); }
    return ((unsigned long long)k.v << 56) | (_tbl_hval(&k) & 0x00FFFFFFFFFFFFFFull);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned _tbl_nbuck_for(int init) {
    unsigned want = (init > 0) ? (unsigned)init / 2u : 4u, nb = 4u;
    while (nb < want && nb < 65536u) nb <<= 1;
    return nb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define TBL_BUCKET_OF(t_, h_) ((unsigned)(h_) & ((t_)->nbuck - 1u))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int _tbl_eq_d(const TBPAIR_t *e, DESCR_t k) {
    if (e->key_descr.v != k.v) return 0;
    switch (k.v) {
        case DT_SNUL: return 1;
        case DT_S:    { const char *a = e->key_descr.s, *b = k.s; unsigned la, lb;
                        if (a == b) return 1;
                        if (!a || !b) return 0;
                        la = _tbl_slen(&e->key_descr); lb = _tbl_slen(&k);
                        return la == lb && memcmp(a, b, (size_t)la) == 0; }
        case DT_I:    return e->key_descr.i == k.i;
        case DT_R:    { union { double d; unsigned long long u; } a, b; a.d = e->key_descr.r; b.d = k.r; return a.u == b.u; }
        case DT_A:    return e->key_descr.arr == k.arr;
        case DT_T:    return e->key_descr.tbl == k.tbl;
        case DT_DATA: return e->key_descr.u == k.u;
        default:      return e->key_descr.ptr == k.ptr;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define TBL_LINEAR_MAX 12u
#define TBL_LOAD_MAX    4u
static inline __attribute__((always_inline)) unsigned _tbl_lower(const TBPAIR_t *en, unsigned n, unsigned long long h) {
    if (n <= TBL_LINEAR_MAX) { const TBPAIR_t *p = en, *e = en + n; while (p < e && p->hkey < h) p++; return (unsigned)(p - en); }
    { unsigned lo = 0;
      while (n) { unsigned half = n >> 1; unsigned mid = lo + half; if (en[mid].hkey < h) { lo = mid + 1; n -= half + 1; } else n = half; }
      return lo; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static TBBUCK_t *_tbl_grow(TBBLK_t *tbl, TBBUCK_t *b) {
    unsigned nc;
    if (!b)                 { int hint = tbl->init / (int)tbl->nbuck; nc = 1u; while (nc < (unsigned)hint && nc < 64u) nc <<= 1; }
    else if (b->cap < 128u) nc = b->cap * 2u;
    else                    nc = b->cap + 128u;
    TBBUCK_t *nb = rt_gcheap_alloc(HB_AGGB, (unsigned long long)(sizeof(TBBUCK_t) + (size_t)nc * sizeof(TBPAIR_t)));
    if (b && b->len) { memcpy(nb->ent, b->ent, (size_t)b->len * sizeof(TBPAIR_t)); nb->len = b->len; }
    nb->cap = nc;
    return nb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *tbl_pair_key(TBPAIR_t *e) {
    if (!e) return "";
    if (!e->key) { char kb[64]; e->key = rt_ws_strdup_c(tbl_key_str(e->key_descr, kb, sizeof kb)); }
    return e->key ? e->key : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBPAIR_t *c_table_find_pair_d(TBBLK_t *tbl, DESCR_t k) {
    if (!tbl) return (TBPAIR_t *)0;
    unsigned long long h = _tbl_hkey(k);
    TBBUCK_t *b = tbl->buckets[TBL_BUCKET_OF(tbl, h)];
    if (!b) return (TBPAIR_t *)0;
    { const TBPAIR_t *p = b->ent + _tbl_lower(b->ent, b->len, h), *e = b->ent + b->len;
      for (; p < e && p->hkey == h; p++) if (_tbl_eq_d(p, k)) return (TBPAIR_t *)p; }
    return (TBPAIR_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t table_get_d(TBBLK_t *tbl, DESCR_t k) { TBPAIR_t *e = table_find_pair_d(tbl, k); return e ? e->val : NULVCL; }
DESCR_t table_get_found_d(TBBLK_t *tbl, DESCR_t k, int *found) { TBPAIR_t *e = table_find_pair_d(tbl, k); *found = e ? 1 : 0; return e ? e->val : NULVCL; }
int     table_has_d(TBBLK_t *tbl, DESCR_t k) { return table_find_pair_d(tbl, k) != (TBPAIR_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int table_delete_d(TBBLK_t *tbl, DESCR_t k) {
    if (!tbl) return 0;
    unsigned long long h = _tbl_hkey(k);
    TBBUCK_t *b = tbl->buckets[TBL_BUCKET_OF(tbl, h)];
    if (!b) return 0;
    for (unsigned i = _tbl_lower(b->ent, b->len, h); i < b->len && b->ent[i].hkey == h; i++)
        if (_tbl_eq_d(&b->ent[i], k)) { memmove(&b->ent[i], &b->ent[i + 1], (size_t)(b->len - i - 1) * sizeof(TBPAIR_t)); b->len--; tbl->size--; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _tbl_rehash(TBBLK_t *tbl) {
    unsigned old_n = tbl->nbuck, nb = old_n << 1;
    if (!nb || nb > 1u << 22) return;
    TBBUCK_t **ov = tbl->buckets, **nv = _tbl_vec_new(nb);
    tbl->buckets = nv; tbl->nbuck = nb;
    for (unsigned b = 0; b < old_n; b++) {
        TBBUCK_t *ob = ov[b];
        if (!ob) continue;
        for (unsigned i = 0; i < ob->len; i++) {
            TBPAIR_t *e = &ob->ent[i];
            unsigned  nbi = (unsigned)e->hkey & (nb - 1u);
            TBBUCK_t *nbk = nv[nbi];
            if (!nbk || nbk->len == nbk->cap) { nbk = _tbl_grow(tbl, nbk); nv[nbi] = nbk; }
            unsigned j = _tbl_lower(nbk->ent, nbk->len, e->hkey);
            if (j < nbk->len) memmove(&nbk->ent[j + 1], &nbk->ent[j], (size_t)(nbk->len - j) * sizeof(TBPAIR_t));
            nbk->ent[j] = *e; nbk->len++;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void table_set_descr_d(TBBLK_t *tbl, DESCR_t k, DESCR_t val) {
    if (!tbl) return;
    if (val.v == DT_S) rt_sxt_break_fast(val.s);
    unsigned long long h = _tbl_hkey(k);
    unsigned bi = TBL_BUCKET_OF(tbl, h);
    TBBUCK_t *b = tbl->buckets[bi];
    unsigned i = b ? _tbl_lower(b->ent, b->len, h) : 0u;
    if (b) for (; i < b->len && b->ent[i].hkey == h; i++)
        if (_tbl_eq_d(&b->ent[i], k)) { b->ent[i].val = val; b->ent[i].key_descr = k; return; }
    if (!b || b->len == b->cap) { b = _tbl_grow(tbl, b); tbl->buckets[bi] = b; }
    if (i < b->len) memmove(&b->ent[i + 1], &b->ent[i], (size_t)(b->len - i) * sizeof(TBPAIR_t));
    { TBPAIR_t *n = &b->ent[i]; n->key = (char *)0; n->key_descr = k; n->val = val; n->hkey = h; }
    b->len++; tbl->size++;
    if ((unsigned)tbl->size > tbl->nbuck * TBL_LOAD_MAX) _tbl_rehash(tbl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_copy_all(TBBLK_t *dst, TBBLK_t *src) {
    if (!dst || !src) return;
    TBPAIR_t *e; TBL_FOREACH(src, e) table_set_descr_d(dst, e->key_descr, e->key_descr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_union(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1; r->id = rt_agg_serial_set();
    set_copy_all(r, x); set_copy_all(r, y);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_diff(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1; r->id = rt_agg_serial_set();
    TBPAIR_t *e; if (x) TBL_FOREACH(x, e) if (!table_has_d(y, e->key_descr)) table_set_descr_d(r, e->key_descr, e->key_descr);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_inter(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1; r->id = rt_agg_serial_set();
    TBPAIR_t *e; if (x) TBL_FOREACH(x, e) if (table_has_d(y, e->key_descr)) table_set_descr_d(r, e->key_descr, e->key_descr);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_table_idx_get(DESCR_t base, DESCR_t key) {
    if (base.v != DT_T || !base.tbl) return NULVCL;
    return table_get_d(base.tbl, key);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_table_idx_set(DESCR_t base, DESCR_t key, DESCR_t val) {
    if (base.v != DT_T || !base.tbl) return;
    table_set_descr_d(base.tbl, key, val);
}
