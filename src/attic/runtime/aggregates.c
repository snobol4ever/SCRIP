/* ATTIC: dead code excised from src/runtime/aggregates.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t *array_ptr(ARBLK_t *a, int i) {
    if (!a) return NULL;
    int idx = i - a->lo;
    if (idx < 0 || idx >= (a->hi - a->lo + 1)) return NULL;
    return &a->data[idx];
}
/*--------------------------------------------------------------------------------------------------------------------*/
void table_set(TBBLK_t *tbl, const char *key, DESCR_t val) {
    if (!tbl || !key) return;
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next) {
        if (strcmp(e->key, key) == 0) { e->val = val; return; }
    }
    TBPAIR_t *e = GC_malloc(sizeof(TBPAIR_t));
    e->key       = GC_strdup(key);
    e->key_descr = STRVAL(e->key);
    e->val  = val;
    e->next = tbl->buckets[h];
    tbl->buckets[h] = e;
    tbl->size++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t *table_ptr(TBBLK_t *tbl, DESCR_t key_d) {
    if (!tbl) return NULL;
    const char *key = VARVAL_fn(key_d);
    if (!key) key = "";
    unsigned h = _tbl_hash(key);
    for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return &e->val;
    TBPAIR_t *e = GC_malloc(sizeof(TBPAIR_t));
    e->key       = GC_strdup(key);
    e->key_descr = key_d;
    e->val       = NULVCL;
    e->next      = tbl->buckets[h];
    tbl->buckets[h] = e;
    tbl->size++;
    return &e->val;
}
