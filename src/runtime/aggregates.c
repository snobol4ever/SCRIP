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
    for (int i = 0; i < TABLE_BUCKETS; i++) for (TBPAIR_t *e = src->buckets[i]; e; e = e->next) table_set_descr(dst, e->key, e->key_descr, e->key_descr);
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
    if (x) for (int i = 0; i < TABLE_BUCKETS; i++) for (TBPAIR_t *e = x->buckets[i]; e; e = e->next) if (!table_has(y, e->key)) table_set_descr(r, e->key, e->key_descr, e->key_descr);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *set_inter(TBBLK_t *x, TBBLK_t *y) {
    TBBLK_t *r = table_new(); r->is_set = 1;
    if (x) for (int i = 0; i < TABLE_BUCKETS; i++) for (TBPAIR_t *e = x->buckets[i]; e; e = e->next) if (table_has(y, e->key)) table_set_descr(r, e->key, e->key_descr, e->key_descr);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_table_idx_get(DESCR_t base, DESCR_t key) {
    if (base.v != DT_T || !base.tbl) return NULVCL;
    char kb[64];
    return table_get(base.tbl, tbl_key_str(key, kb, sizeof kb));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_table_idx_set(DESCR_t base, DESCR_t key, DESCR_t val) {
    if (base.v != DT_T || !base.tbl) return;
    char kb[64];
    table_set_descr(base.tbl, tbl_key_str(key, kb, sizeof kb), key, val);
}
