#include "rt/rt_arena.h"
#include "core.h"
#include "sil_macros.h"
#include <string.h>
static long g_agg_list_ser = 1;
static long g_agg_table_ser = 1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_agg_serial_list(void) { return g_agg_list_ser++; }
long rt_agg_serial_table(void) { return g_agg_table_ser++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ARBLK_t *array_new(int lo, int hi) {
    ARBLK_t *a = rt_ws_alloc(sizeof(ARBLK_t));
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
    ARBLK_t *a = rt_ws_alloc(sizeof(ARBLK_t));
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
static unsigned _tbl_hash(const char *key) {
    unsigned h = 5381;
    while (*key) h = h * 33 ^ (unsigned char)*key++;
    return h % TABLE_BUCKETS;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *table_new(void) {
    TBBLK_t *t = rt_agg_alloc(2, sizeof(TBBLK_t));
    memset(t->buckets, 0, sizeof(t->buckets));
    t->id   = g_agg_table_ser++;
    t->size = 0;
    t->init = 11;
    t->inc  = 10;
    t->is_set = 0;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *table_new_args(int init, int inc) {
    TBBLK_t *t = table_new();
    if (init > 0) t->init = init;
    if (inc  > 0) t->inc  = inc;
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
/*⭐⭐⭐ ONE HASH FUNCTION PER DATATYPE, THEN BY VALUE (Lon, 2026-08-23 s262).  DIAGNOSED s258, FILED AS A QUEUE ROW LOCKED BY A SEAT THAT NEVER EXISTED, AND THEREFORE NEVER CURED FOR A DAY.
  ⛔⛔ MY FIRST ATTEMPT MEASURED WORSE (+0.24%, then +6.13% on table_access) AND THE REASON WAS A CONSTRAINT I INVENTED, NOT A LIMIT OF HASHING.  I made the typed hash reproduce the STRING hash bucket-for-bucket so that string-keyed and descriptor-keyed callers could share one table while I converted callers gradually.  To reproduce djb2("\001i123") you must walk the digits '1','2','3' -- so the "typed" hash still stringified in all but name, bought only the strcmp, and paid a fatter hash function for it.  ⭐ THE CURE IS TO STOP HEDGING: hash an integer AS AN INTEGER (a multiply and a shift), and convert EVERY caller in the same change so there is no mixed-hash hazard to hedge against.  A table where some callers hash by string and some by value silently misses; that risk is what this file now removes by construction.
  ⛔ DT_S HASHING IS DELIBERATELY UNCHANGED -- still djb2 over the bytes -- for two reasons: a string key IS its own value so there is nothing to gain, and rtx_icnsub.S RTX-26 INLINES this exact djb2 loop to walk the chain itself for DT_S subscripts.  Changing it would send the assembly to the wrong bucket.  That arm handles DT_S only, so integer keys never reach it.
  ⛔ THE STRUCT DOES NOT MOVE: sizeof(TBPAIR_t)==48, key@0, val@24, next@40, pinned by a _Static_assert in rtx_init.c.
  ⛔ TBPAIR_t.key IS STILL POPULATED on insert -- iteration, sorting, CONVERT and the set operators read it -- but it is built ONCE per distinct key instead of on every lookup, which is where the 36.6% was.
  KILLSWITCH SCRIP_TBL_TYPED=0 restores string hashing and string compare on the same binary. */
static inline __attribute__((always_inline)) int tbl_typed_off(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_TBL_TYPED"); v = (e && *e == '0') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) unsigned _tbl_mix64(unsigned long long u) { u *= 0x9E3779B97F4A7C15ull; u ^= u >> 29; u *= 0xBF58476D1CE4E5B9ull; u ^= u >> 32; return (unsigned)(u & (unsigned long long)(TABLE_BUCKETS - 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned _tbl_hash_d(DESCR_t k) {
    if (tbl_typed_off()) { char kb[64]; return _tbl_hash(tbl_key_str(k, kb, sizeof kb)); }
    switch (k.v) {
        case DT_S:    { unsigned h = 5381; const char *p = k.s ? k.s : ""; while (*p) h = h * 33 ^ (unsigned char)*p++; return h % TABLE_BUCKETS; }   /* unchanged -- rtx_icnsub.S inlines this loop */
        case DT_SNUL: return _tbl_mix64(0x6E756C6Cull);
        case DT_I:    return _tbl_mix64((unsigned long long)(long long)k.i ^ 0x01000000ull);
        case DT_R:    { union { double d; unsigned long long u; } cv; cv.d = k.r; return _tbl_mix64(cv.u ^ 0x02000000ull); }
        case DT_A:    { if (!k.arr) return _tbl_mix64(0x03000000ull); if (!k.arr->id) k.arr->id = g_agg_list_ser++; return _tbl_mix64((unsigned long long)k.arr->id ^ 0x03000000ull); }
        case DT_T:    { if (!k.tbl) return _tbl_mix64(0x04000000ull); if (!k.tbl->id) k.tbl->id = g_agg_table_ser++; return _tbl_mix64((unsigned long long)k.tbl->id ^ (k.tbl->is_set ? 0x05000000ull : 0x04000000ull)); }
        default:      { char kb[64]; return _tbl_hash(tbl_key_str(k, kb, sizeof kb)); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Datatype first, then value.  Distinct types are never equal -- tbl_key_str's encoding, which still backs e->key, is injective across types, so this matches what the string compare always did. */
static inline __attribute__((always_inline)) int _tbl_eq_d(const TBPAIR_t *e, DESCR_t k) {
    if (e->key_descr.v != k.v) return 0;
    switch (k.v) {
        case DT_I:    return e->key_descr.i == k.i;
        case DT_SNUL: return 1;
        case DT_S:    return e->key && k.s && strcmp(e->key, k.s) == 0;
        case DT_R:    return e->key_descr.r == k.r;
        case DT_A:    return e->key_descr.arr == k.arr;
        case DT_T:    return e->key_descr.tbl == k.tbl;
        default:      { char kb[64]; const char *ks = tbl_key_str(k, kb, sizeof kb); return e->key && strcmp(e->key, ks) == 0; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBPAIR_t *table_find_pair_d(TBBLK_t *tbl, DESCR_t k) {
    if (!tbl) return (TBPAIR_t *)0;
    unsigned h = _tbl_hash_d(k);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next) if (_tbl_eq_d(e, k)) return e;
    return (TBPAIR_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t table_get_d(TBBLK_t *tbl, DESCR_t k) { TBPAIR_t *e = table_find_pair_d(tbl, k); return e ? e->val : NULVCL; }
DESCR_t table_get_found_d(TBBLK_t *tbl, DESCR_t k, int *found) { TBPAIR_t *e = table_find_pair_d(tbl, k); *found = e ? 1 : 0; return e ? e->val : NULVCL; }
int     table_has_d(TBBLK_t *tbl, DESCR_t k) { return table_find_pair_d(tbl, k) != (TBPAIR_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int table_delete_d(TBBLK_t *tbl, DESCR_t k) {
    if (!tbl) return 0;
    unsigned h = _tbl_hash_d(k);
    for (TBPAIR_t **pp = &tbl->buckets[h]; *pp; pp = &(*pp)->next) if (_tbl_eq_d(*pp, k)) { *pp = (*pp)->next; tbl->size--; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void table_set_descr_d(TBBLK_t *tbl, DESCR_t k, DESCR_t val) {
    if (!tbl) return;
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    unsigned h = _tbl_hash_d(k);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next) if (_tbl_eq_d(e, k)) { e->val = val; e->key_descr = k; return; }
    char kb[64]; const char *ks = tbl_key_str(k, kb, sizeof kb);   /* ONCE per distinct key, on insert only */
    TBPAIR_t *e = rt_agg_alloc(1, sizeof(TBPAIR_t));
    e->key = rt_ws_strdup_c(ks); e->key_descr = k; e->val = val;
    e->next = tbl->buckets[h]; tbl->buckets[h] = e; tbl->size++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBPAIR_t *table_find_pair(TBBLK_t *tbl, const char *key) {
    if (!tbl || !key) return (TBPAIR_t *)0;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return e;
    return (TBPAIR_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t table_get(TBBLK_t *tbl, const char *key) {
    if (!tbl || !key) return NULVCL;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return e->val;
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t table_get_found(TBBLK_t *tbl, const char *key, int *found) {
    *found = 0;
    if (!tbl || !key) return NULVCL;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next)
        if (strcmp(e->key, key) == 0) { *found = 1; return e->val; }
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void table_set_descr(TBBLK_t *tbl, const char *key, DESCR_t key_d, DESCR_t val) {
    if (!tbl || !key) return;
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next) {
        if (strcmp(e->key, key) == 0) { e->val = val; e->key_descr = key_d; return; }
    }
    TBPAIR_t *e = rt_agg_alloc(1, sizeof(TBPAIR_t));
    e->key       = rt_ws_strdup_c(key);
    e->key_descr = key_d;
    e->val  = val;
    e->next = tbl->buckets[h];
    tbl->buckets[h] = e;
    tbl->size++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void table_set_descr_keyown(TBBLK_t *tbl, const char *key, DESCR_t key_d, DESCR_t val) {
    if (!tbl || !key) return;
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next) {
        if (strcmp(e->key, key) == 0) { e->val = val; e->key_descr = key_d; return; }
    }
    TBPAIR_t *e = rt_agg_alloc(1, sizeof(TBPAIR_t));
    e->key       = (char *)key;
    e->key_descr = key_d;
    e->val  = val;
    e->next = tbl->buckets[h];
    tbl->buckets[h] = e;
    tbl->size++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int table_delete(TBBLK_t *tbl, const char *key) {
    if (!tbl || !key) return 0;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t **pp = &tbl->buckets[h]; *pp; pp = &(*pp)->next)
        if (strcmp((*pp)->key, key) == 0) { *pp = (*pp)->next; tbl->size--; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int table_has(TBBLK_t *tbl, const char *key) {
    if (!tbl || !key) return 0;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_copy_all(TBBLK_t *dst, TBBLK_t *src) {
    if (!dst || !src) return;
    for (int i = 0; i < TABLE_BUCKETS; i++) for (TBPAIR_t *e = src->buckets[i]; e; e = e->next) table_set_descr_d(dst, e->key_descr, e->key_descr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_union(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1;
    set_copy_all(r, x); set_copy_all(r, y);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_diff(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1;
    if (x) for (int i = 0; i < TABLE_BUCKETS; i++) for (TBPAIR_t *e = x->buckets[i]; e; e = e->next) if (!table_has_d(y, e->key_descr)) table_set_descr_d(r, e->key_descr, e->key_descr);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_inter(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1;
    if (x) for (int i = 0; i < TABLE_BUCKETS; i++) for (TBPAIR_t *e = x->buckets[i]; e; e = e->next) if (table_has_d(y, e->key_descr)) table_set_descr_d(r, e->key_descr, e->key_descr);
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
