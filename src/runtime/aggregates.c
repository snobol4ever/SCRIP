#include "core.h"
#include "sil_macros.h"
#include <string.h>
/*====================================================================================================================*/
ARBLK_t *array_new(int lo, int hi) {
    ARBLK_t *a = GC_malloc(sizeof(ARBLK_t));
    a->lo   = lo;
    a->hi   = hi;
    a->ndim = 1;
    int sz  = hi - lo + 1;
    if (sz < 1) sz = 1;
    a->data = GC_malloc(sz * sizeof(DESCR_t));
    for (int i = 0; i < sz; i++) a->data[i] = NULVCL;
    return a;
}
/*--------------------------------------------------------------------------------------------------------------------*/
ARBLK_t *array_new2d(int lo1, int hi1, int lo2, int hi2) {
    ARBLK_t *a = GC_malloc(sizeof(ARBLK_t));
    a->lo   = lo1;
    a->hi   = hi1;
    a->lo2  = lo2;
    a->hi2  = hi2;
    a->ndim = 2;
    int rows = hi1 - lo1 + 1;
    int cols = hi2 - lo2 + 1;
    if (rows < 1) rows = 1;
    if (cols < 1) cols = 1;
    a->data = GC_malloc(rows * cols * sizeof(DESCR_t));
    for (int i = 0; i < rows * cols; i++) a->data[i] = NULVCL;
    return a;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t array_get(ARBLK_t *a, int i) {
    if (!a) return FAILDESCR;
    int idx = i - a->lo;
    if (idx < 0 || idx >= (a->hi - a->lo + 1)) return FAILDESCR;
    return a->data[idx];
}
/*--------------------------------------------------------------------------------------------------------------------*/
void array_set(ARBLK_t *a, int i, DESCR_t v) {
    if (!a) return;
    int idx = i - a->lo;
    if (idx < 0 || idx >= (a->hi - a->lo + 1)) return;
    a->data[idx] = v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void array_set2(ARBLK_t *a, int i, int j, DESCR_t v) {
    if (!a) return;
    int cols = a->hi2 - a->lo2 + 1;
    int row  = i - a->lo;
    int col  = j - a->lo2;
    int idx  = row * cols + col;
    a->data[idx] = v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static unsigned _tbl_hash(const char *key) {
    unsigned h = 5381;
    while (*key) h = h * 33 ^ (unsigned char)*key++;
    return h % TABLE_BUCKETS;
}
/*--------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *table_new(void) {
    TBBLK_t *t = GC_malloc(sizeof(TBBLK_t));
    memset(t->buckets, 0, sizeof(t->buckets));
    t->size = 0;
    t->init = 10;
    t->inc  = 10;
    return t;
}
/*--------------------------------------------------------------------------------------------------------------------*/
TBBLK_t *table_new_args(int init, int inc) {
    TBBLK_t *t = table_new();
    t->init = (init > 0) ? init : 10;
    t->inc  = (inc  > 0) ? inc  : 10;
    return t;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t table_get(TBBLK_t *tbl, const char *key) {
    if (!tbl || !key) return NULVCL;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return e->val;
    return NULVCL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t table_get_found(TBBLK_t *tbl, const char *key, int *found) {
    *found = 0;
    if (!tbl || !key) return NULVCL;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next)
        if (strcmp(e->key, key) == 0) { *found = 1; return e->val; }
    return NULVCL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void table_set_descr(TBBLK_t *tbl, const char *key, DESCR_t key_d, DESCR_t val) {
    if (!tbl || !key) return;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next) {
        if (strcmp(e->key, key) == 0) { e->val = val; e->key_descr = key_d; return; }
    }
    TBPAIR_t *e = GC_malloc(sizeof(TBPAIR_t));
    e->key       = GC_strdup(key);
    e->key_descr = key_d;
    e->val  = val;
    e->next = tbl->buckets[h];
    tbl->buckets[h] = e;
    tbl->size++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int table_has(TBBLK_t *tbl, const char *key) {
    if (!tbl || !key) return 0;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_table_idx_get(DESCR_t base, DESCR_t key) {
    if (base.v != DT_T || !base.tbl) return NULVCL;
    const char *ks = VARVAL_fn(key);
    return table_get(base.tbl, ks ? ks : "");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_table_idx_set(DESCR_t base, DESCR_t key, DESCR_t val) {
    if (base.v != DT_T || !base.tbl) return;
    const char *ks = VARVAL_fn(key);
    table_set_descr(base.tbl, ks ? ks : "", key, val);
}
