/* ATTIC: dead code excised from src/lower/lower_pascal.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int scope_slot_chain(const pas_scope_t * sc, const char * name, long long * byref_out) {
    if (!name) return -1;
    for (const pas_scope_t * s = sc; s; s = s->outer) {
        int slot = scope_slot(s, name);
        if (slot >= 0) { if (byref_out) *byref_out = s->byref; return slot; }
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_pascal(const tree_t * prog) {
    lower_pascal_enum(prog, NULL, 0);
    IR_graph_t * g = IR_alloc(8192, IR_LANG_PAS); pcx_t cx; memset(&cx, 0, sizeof cx); cx.g = g; lc_vec_init(&cx.labels, (int) sizeof(pas_label_t));
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED); IR_t * fail = IR_node_alloc(g, IR_FAIL);
    IR_t * entry = lower(&cx, prog, succ, fail);
    g->entry = entry ? entry : succ; return g;
}
