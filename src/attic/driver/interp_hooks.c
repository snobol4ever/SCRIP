/* ATTIC: dead code excised from src/driver/interp_hooks.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
int _expr_is_pat(tree_t *e) {
    if (!e) return 0;
    switch (e->t) {
        case TT_ARB: case TT_ARBNO: case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN: case TT_CAPT_CURSOR: case TT_DEFER:
            return 1;
        default: break;
    }
    if (e->t == TT_FNC && _is_pat_fnc_name(e->v.sval)) return 1;
    if (e->t == TT_VAR && _is_pat_fnc_name(e->v.sval)) return 1;
    for (int i = 0; i < e->n; i++)
        if (_expr_is_pat(e->c[i])) return 1;
    return 0;
}
