/* ATTIC: dead code excised from src/driver/scrip_sm.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
void stage2_free_sm_bb(stage2_t *s2)
{
    bb_program_free(&s2->bbp);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void stage2_free_bb_after_emit(stage2_t *s2)
{
    bb_program_free(&s2->bbp);
}
