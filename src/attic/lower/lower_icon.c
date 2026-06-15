/* ATTIC: dead code excised from src/lower/lower_icon.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_user_proc(icx_t * cx, const char * nm) { if (!nm) return 0; for (int i = 0; i < cx->npn; i++) if (cx->pn[i] && !strcmp(cx->pn[i], nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_icon_enum(const tree_t * prog, const tree_t ** out, int max) { return collect_procs(prog, out, max, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_icon(const tree_t * prog) {
    lc_vec ps; lc_vec_init(&ps, (int) sizeof(const tree_t *)); collect_procs_vec(prog, &ps);
    if (ps.n > 0) return lower_icon_proc(prog, LC_AT(&ps, const tree_t *, 0));
    IR_graph_t * g = IR_alloc(64, IR_LANG_ICN); icx_t cx; memset(&cx, 0, sizeof cx); cx.g = g; IR_t * s = build(&cx, IR_SUCCEED, 0, 0); g->entry = s; return g;
}
IR_graph_t *lower_proc_gen(struct GeneratorState *gs) {
    if (!gs) return NULL;
    IR_graph_t *bbg = IR_alloc(4, IR_LANG_ICN);
    if (!bbg) return NULL;
    IR_t *bb = IR_node_alloc(bbg, IR_PROC_GEN);
    if (!bb) return NULL;
    IR_EXEC(bb).counter = (int64_t)(uintptr_t)gs;
    bb->γ.node = NULL;
    bb->ω.node = NULL;
    bbg->entry = bb;
    return bbg;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* ATTIC: dead-code sweep batch 4 (origin src/lower/lower_icon.c, base 4c9b6bd) — collect_procs: self-recursive only, never called from any reachable fn in this TU (collect_procs_vec is the live variant). GC-oracle dead. Provenance only; not compiled. */
static int collect_procs(const tree_t * t, const tree_t ** out, int max, int n) {
    if (!t || n >= max) return n;
    if (t->t == TT_STMT) return collect_procs(stmt_subj(t), out, max, n);
    if (t->t == TT_PROC_DECL) { out[n++] = t; return n; }
    for (int i = 0; i < t->n; i++) n = collect_procs(t->c[i], out, max, n);
    return n;
}
