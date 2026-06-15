/* ATTIC: dead code excised from src/lower/lower_raku.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*====================================================================================================================================================================================================*/
int lower_raku_enum(const tree_t * prog, const tree_t ** out, int max) {
    int n = 0;
    if (!prog) return 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (d && d->t == TT_SUB_DECL) { if (out && n < max) out[n] = d; n++; }
    }
    return n;
}
/*====================================================================================================================================================================================================*/
IR_graph_t * lower_raku(const tree_t * prog) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_RKU);
    rcx_t cx; cx.g = g;
    IR_t * top = IR_node_alloc(g, IR_PROG);
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        ir_operand_push(top, lower_decl(&cx, d));
    }
    g->entry = top;
    return g;
}
