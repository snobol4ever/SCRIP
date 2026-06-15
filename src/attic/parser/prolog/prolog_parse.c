/* ATTIC: dead code excised from src/parser/prolog/prolog_parse.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
void prolog_program_free(PlProgram *prog) {
    PlClause *cl = prog->head;
    while (cl) {
        PlClause *next = cl->next;
        free(cl->body);
        free(cl);
        cl = next;
    }
    free(prog);
}
