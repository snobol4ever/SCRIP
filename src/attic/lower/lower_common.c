/* ATTIC: dead code excised from src/lower/lower_common.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * bb_label_landing(const char * name) {
    if (!name) return NULL;
    for (int i = 0; i < g_bb_labels.n; i++)
        if (LC_AT(&g_bb_labels, bb_label_entry_t, i).name && !strcmp(LC_AT(&g_bb_labels, bb_label_entry_t, i).name, name)) return LC_AT(&g_bb_labels, bb_label_entry_t, i).landing;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void * lc_vec_at(const lc_vec * v, int i) { return (char *) v->data + (size_t) i * (size_t) v->esz; }
