/* ATTIC: dead code excised from src/runtime/runtime_eval.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t CODE_fn(DESCR_t str_d)
{
    const char *s = VARVAL_fn(str_d);
    if (!s || !*s) return FAILDESCR;
    return code(s);
}
