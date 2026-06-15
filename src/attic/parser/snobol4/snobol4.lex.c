/* ATTIC: dead code excised from src/parser/snobol4/snobol4.lex.c (commit base 1308f79) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Token mktok(int k, const char *sv, long iv, double dv) {
    Token t; t.kind=k; t.sval=sv; t.ival=iv; t.dval=dv; t.lineno=lineno;
    return t;
}
    static void yy_pop_state  (yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
	if ( --yyg->yy_start_stack_ptr < 0 )
		YY_FATAL_ERROR( "start-condition stack underflow" );

	BEGIN(yyg->yy_start_stack[yyg->yy_start_stack_ptr]);
}
    static int yy_top_state  (yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
	return yyg->yy_start_stack[yyg->yy_start_stack_ptr - 1];
}
 \
	while ( 0 )

/* Accessor  methods (get/set functions) to struct members. */

/** Get the user-defined data for this scanner.
 * @param yyscanner The scanner object.
 */
YY_EXTRA_TYPE yyget_extra  (yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    return yyextra;
}
/** Get the current column number.
 * @param yyscanner The scanner object.
 */
int yyget_column  (yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

        if (! YY_CURRENT_BUFFER)
            return 0;
    
    return yycolumn;
}
/** Set the user-defined data. This data is never touched by the scanner.
 * @param user_defined The data to be associated with this scanner.
 * @param yyscanner The scanner object.
 */
void yyset_extra (YY_EXTRA_TYPE  user_defined , yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    yyextra = user_defined ;
}
/** Set the current column.
 * @param _column_no column number
 * @param yyscanner The scanner object.
 */
void yyset_column (int  _column_no , yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

        /* column is only valid if an input buffer exists. */
        if (! YY_CURRENT_BUFFER )
           YY_FATAL_ERROR( "yyset_column called with no buffer" );
    
    yycolumn = _column_no;
}
/* yylex_init_extra has the same functionality as yylex_init, but follows the
 * convention of taking the scanner as the last argument. Note however, that
 * this is a *pointer* to a scanner, as it will be allocated by this call (and
 * is the reason, too, why this function also must handle its own declaration).
 * The user defined value in the first argument will be available to yyalloc in
 * the yyextra field.
 */
int yylex_init_extra( YY_EXTRA_TYPE yy_user_defined, yyscan_t* ptr_yy_globals )
{
    struct yyguts_t dummy_yyguts;

    yyset_extra (yy_user_defined, &dummy_yyguts);

    if (ptr_yy_globals == NULL){
        errno = EINVAL;
        return 1;
    }

    *ptr_yy_globals = (yyscan_t) yyalloc ( sizeof( struct yyguts_t ), &dummy_yyguts );

    if (*ptr_yy_globals == NULL){
        errno = ENOMEM;
        return 1;
    }

    /* By setting to 0xAA, we expose bugs in
    yy_init_globals. Leave at 0x00 for releases. */
    memset(*ptr_yy_globals,0x00,sizeof(struct yyguts_t));

    yyset_extra (yy_user_defined, *ptr_yy_globals);

    return yy_init_globals ( *ptr_yy_globals );
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Token lex_peek(Lex *lx) {
    if (!lx->peeked) { lx->peek=lex_next(lx); lx->peeked=1; }
    return lx->peek;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int  lex_at_end(Lex *lx) { return lex_peek(lx).kind==T_EOF; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void lex_destroy(Lex *lx) { flex_lex_destroy(lx); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
CODE_t *sno_parse(FILE *f, const char *fname) {
    Lex lx; memset(&lx,0,sizeof lx);
    flex_lex_open(&lx,f,fname);
    CODE_t *prog = parse_program_tokens(&lx);
    flex_lex_destroy(&lx);
    return prog;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sno_reset(void) { sno_nerrors = 0; n_inc = 0; }
