/* ATTIC: dead code excised from src/runtime/rt/pat_pool.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
void pat_pool_reset(void) {
    g_pat_pool_cur = g_pat_pool_base;
}
