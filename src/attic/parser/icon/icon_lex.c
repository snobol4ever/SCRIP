/* ATTIC: dead code excised from src/parser/icon/icon_lex.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
IcnToken icn_lex_peek(IcnLexer *lx) {
    size_t saved_pos  = lx->pos;
    int    saved_line = lx->line;
    int    saved_col  = lx->col;
    IcnToken t = lex_one(lx);
    lx->pos  = saved_pos;
    lx->line = saved_line;
    lx->col  = saved_col;
    return t;
}
