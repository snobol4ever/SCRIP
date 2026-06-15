/* ATTIC: dead code excised from src/runtime/runtime_eval.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t CODE_fn(DESCR_t str_d)
{
    const char *s = VARVAL_fn(str_d);
    if (!s || !*s) return FAILDESCR;
    return code(s);
}
/*====================================================================================================================*/
/* ATTIC: exec_code excised from src/runtime/runtime_eval.c (commit base cf50707; DT_C-CODE rung, 2026-06-15).        */
/* REASON: the AST-walking CODE interpreter. It iterated prog->c[] and called eval_node() on every subj/pat/repl —    */
/* eval_node is a [B0b] BOMB (the mode-1 AST evaluator was deleted), so this path aborted at runtime. Replaced by the */
/* DT_E-drill emit pipeline: code() now parses the whole program → lower_snobol4 → gvar_flat_chain_build → bb_box_fn, */
/* and EXPVAL_fn's DT_C branch runs the emitted chain via rt_eval_run (the twin of the DT_E slen==3 branch). This is  */
/* both AST-walking-at-runtime (forbidden in modes 2/3/4) and interpreter residue (DE-INTERP-aligned). Provenance     */
/* only; not compiled.                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
const char *exec_code(DESCR_t code_block)
{
    if (code_block.v != DT_C || !code_block.ptr) return NULL;
    const tree_t *prog = (const tree_t *)code_block.ptr;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t *s = prog->c[_ci];
        if (!s) continue;
        if (s->t == TT_END) return "";
        int has_eq = stmt_attr_find(s, ":eq") != NULL;
        tree_t *subject     = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        tree_t *pattern     = stmt_attr_expr(stmt_attr_find(s, ":pat"));
        tree_t *replacement = stmt_attr_expr(stmt_attr_find(s, ":repl"));
        if (!pattern && subject && subject->t == TT_SCAN && subject->n == 2) {
            pattern = subject->c[1];
            subject = subject->c[0];
        }
        if (!pattern && subject && subject->t == TT_SEQ && subject->n >= 2) {
            tree_t *first = subject->c[0];
            if (first->t == TT_VAR || first->t == TT_KEYWORD || first->t == TT_QLIT || first->t == TT_INDIRECT) {
                int nc = subject->n - 1;
                tree_t *rest;
                if (nc == 1) { rest = subject->c[1]; }
                else { rest = ast_node_new(TT_SEQ); for (int i = 1; i < subject->n; i++) expr_add_child(rest, subject->c[i]); }
                pattern = rest;
                subject = first;
            }
        }
        const char *goto_u = goto_node_str(stmt_goto_find(s, TT_GOTO_U));
        const char *goto_s = goto_node_str(stmt_goto_find(s, TT_GOTO_S));
        const char *goto_f = goto_node_str(stmt_goto_find(s, TT_GOTO_F));
        DESCR_t subj_val = NULVCL;
        const char *subj_name = NULL;
        if (subject) {
            if (subject->t == TT_VAR && subject->v.sval) {
                subj_name = subject->v.sval;
                subj_val  = NV_GET_fn(subj_name);
            } else {
                subj_val = eval_node(subject);
            }
        }
        int succeeded = 1;
        if (pattern) {
            DESCR_t pat_d = eval_node(pattern);
            if (IS_FAIL_fn(pat_d)) {
                succeeded = 0;
            } else {
                DESCR_t repl_val;
                int has_repl = 0;
                if (has_eq && replacement) {
                    repl_val = eval_node(replacement);
                    has_repl = !IS_FAIL_fn(repl_val);
                }
                succeeded = exec_stmt(
                    subj_name,
                    subj_name ? NULL : &subj_val,
                    pat_d,
                    has_repl ? &repl_val : NULL,
                    has_repl);
            }
        } else if (has_eq && replacement && subj_name) {
            DESCR_t repl_val = eval_node(replacement);
            if (IS_FAIL_fn(repl_val)) {
                succeeded = 0;
            } else {
                NV_SET_fn(subj_name, repl_val);
                succeeded = 1;
            }
        } else if (subject && !pattern && !has_eq) {
            if (IS_FAIL_fn(subj_val)) succeeded = 0;
        }
        if (goto_u || goto_s || goto_f) {
            if (goto_u && *goto_u) return goto_u;
            if (succeeded && goto_s && *goto_s) return goto_s;
            if (!succeeded && goto_f && *goto_f) return goto_f;
        }
    }
    return "";
}
