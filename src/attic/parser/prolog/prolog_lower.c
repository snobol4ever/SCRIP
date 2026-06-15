/* ATTIC: dead code excised from src/parser/prolog/prolog_lower.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *pl_make_clause(tree_t *head_tr, tree_t *body_tr) {
    tree_t *cl = ast_node_new(TT_CLAUSE);
    if (head_tr) {
        ast_push(cl, head_tr);
    } else {
        ast_push(cl, ast_node_new(TT_NUL));
    }
    tree_t *prog = ast_node_new(TT_PROGRAM);
    if (body_tr) pl_flatten_conj(body_tr, prog);
    ast_push(cl, prog);
    return cl;
}
/*--------------------------------------------------------------------------------------------------------------------*/
tree_t *pl_assert_term(Term *t, int *functor_out, int *arity_out) {
    if (!t) return NULL;
    t = term_deref(t);
    if (!t) return NULL;
    Term *head = NULL;
    Term *body = NULL;
    if (t->tag == TERM_COMPOUND && t->compound.arity == 2) {
        const char *fn = prolog_atom_name(t->compound.functor);
        if (fn && strcmp(fn, ":-") == 0) {
            head = term_deref(t->compound.args[0]);
            body = term_deref(t->compound.args[1]);
        }
    }
    if (!head) head = t;
    PredKey k = key_of_head(head);
    if (k.functor < 0) return NULL;
    if (functor_out) *functor_out = k.functor;
    if (arity_out)   *arity_out   = k.arity;
    PlClause cl_buf;
    memset(&cl_buf, 0, sizeof cl_buf);
    cl_buf.head   = head;
    cl_buf.lineno = 0;
    Term *body_goals[256];
    int nbody = 0;
    if (body) {
        Term *cur = body;
        while (cur && cur->tag == TERM_COMPOUND && cur->compound.arity == 2) {
            const char *cfn = prolog_atom_name(cur->compound.functor);
            if (!cfn || strcmp(cfn, ",") != 0) break;
            if (nbody < 256) body_goals[nbody++] = term_deref(cur->compound.args[0]);
            cur = term_deref(cur->compound.args[1]);
        }
        if (nbody < 256) body_goals[nbody++] = cur;
    }
    cl_buf.body  = (nbody > 0) ? body_goals : NULL;
    cl_buf.nbody = nbody;
    return lower_clause(&cl_buf, k);
}
