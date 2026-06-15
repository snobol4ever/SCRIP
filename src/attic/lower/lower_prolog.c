/* ATTIC: dead code excised from src/lower/lower_prolog.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_prolog(const tree_t * prog) {
    IR_graph_t * g = IR_alloc(8192, IR_LANG_PL);
    lcx_t cx; cx.g = g; cx.tω = NULL;
    const tree_t * clause = NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * d = prog->c[i];
        if (d->t == TT_STMT) { const tree_t * sub = stmt_subj(d); if (!sub) continue; d = sub; }
        if (d->t == TT_CHOICE && d->v.sval && !strcmp(d->v.sval, "main/0") && d->n > 0) { clause = d->c[0]; break; }
    }
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * gconj   = build(&cx, IR_GCONJ, succeed, fail);
    if (!clause) { IR_LIT(gconj).ival = (long long)(intptr_t) calloc(1, sizeof(bb_conj_state_t)); g->entry = gconj; g->body_root = gconj; g->nslots = 0; return g; }
    int arity = (int) clause->v.dval;
    IR_t * entry = NULL;
    thread_goals(&cx, clause, arity, clause->n, gconj, fail, &entry, gconj);
    g->entry = entry ? entry : gconj;
    g->body_root = gconj;
    g->nslots = max_var_slot(clause, -1) + 1;
    return g;
}
int pl_rt_assertz(Term *clause_term, int prepend) {
    int fid = -1, arity = 0;
    tree_t *clause = pl_assert_term(clause_term, &fid, &arity);
    if (!clause || clause->t != TT_CLAUSE) return 0;
    const char *fname = prolog_atom_name(fid);
    if (!fname) return 0;
    char key[256]; snprintf(key, sizeof key, "%s/%d", fname, arity);
    int body_idx = lower_pl_clause_graph(clause);
    if (body_idx < 0) return 0;
    IR_graph_t *body = g_stage2.bbp.table[body_idx];
    if (!body) return 0;
    Resolve_PredEntry_BB *entry = resolve_bb_lookup(key, arity);
    IR_graph_t *pred_cfg = entry ? bb_graph_of_pred(entry) : NULL;
    if (!pred_cfg || !pred_cfg->entry || pred_cfg->entry->op != IR_CHOICE) {
        IR_graph_t *prior = (pred_cfg && pred_cfg->entry) ? pred_cfg : NULL;
        IR_graph_t *cg = IR_alloc(8, IR_LANG_PL);
        if (!cg) return 0;
        IR_t *PSUCC = IR_node_alloc(cg, IR_SUCCEED);
        IR_t *PFAIL = IR_node_alloc(cg, IR_FAIL);
        IR_t *nd = IR_node_alloc(cg, IR_CHOICE);
        if (!nd) return 0;
        bb_choice_state_t *zc0 = (bb_choice_state_t *)GC_MALLOC(sizeof *zc0);
        if (!zc0) return 0;
        memset(zc0, 0, sizeof *zc0);
        if (prior) {
            IR_graph_t **pb = (IR_graph_t **)GC_MALLOC(sizeof(IR_graph_t *));
            if (!pb) return 0;
            pb[0] = prior;
            zc0->bodies = pb; zc0->nbodies = 1;
        } else {
            zc0->bodies = NULL; zc0->nbodies = 0;
        }
        zc0->idx_ok = 0; zc0->idx_key = NULL;
        IR_LIT(nd).ival = (int64_t)(intptr_t)zc0;
        nd->γ.node = PSUCC; memcpy(nd->γ.sz, "α", 3); nd->ω.node = PFAIL; memcpy(nd->ω.sz, "α", 3);
        (void)PSUCC; (void)PFAIL;
        cg->entry = nd;
        int cg_idx = bb_program_add(&g_stage2.bbp, cg);
        if (cg_idx < 0) return 0;
        resolve_bb_register(key, arity, cg_idx);
        pred_cfg = g_stage2.bbp.table[cg_idx];
    }
    bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)IR_LIT(pred_cfg->entry).ival;
    if (!zc) return 0;
    int n = zc->nbodies;
    IR_graph_t **nb = (IR_graph_t **)GC_MALLOC((size_t)(n + 1) * sizeof(IR_graph_t *));
    if (!nb) return 0;
    if (prepend) {
        nb[0] = body;
        for (int i = 0; i < n; i++) nb[i + 1] = zc->bodies[i];
    } else {
        for (int i = 0; i < n; i++) nb[i] = zc->bodies[i];
        nb[n] = body;
    }
    zc->bodies = nb;
    zc->nbodies = n + 1;
    zc->idx_ok = 0; zc->idx_key = NULL;
    return 1;
}
