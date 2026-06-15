/* ATTIC: dead code excised from src/parser/pascal/pascal.lex.c (commit base 2c38d15) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/** Pushes the new state onto the stack. The new state becomes
 *  the current state. This function will allocate the stack
 *  if necessary.
 *  @param new_buffer The new state.
 *  
 */
void yypush_buffer_state (YY_BUFFER_STATE new_buffer )
{
    	if (new_buffer == NULL)
		return;

	yyensure_buffer_stack();

	/* This block is copied from yy_switch_to_buffer. */
	if ( YY_CURRENT_BUFFER )
		{
		/* Flush out information for old buffer. */
		*(yy_c_buf_p) = (yy_hold_char);
		YY_CURRENT_BUFFER_LVALUE->yy_buf_pos = (yy_c_buf_p);
		YY_CURRENT_BUFFER_LVALUE->yy_n_chars = (yy_n_chars);
		}

	/* Only push if top exists. Otherwise, replace top. */
	if (YY_CURRENT_BUFFER)
		(yy_buffer_stack_top)++;
	YY_CURRENT_BUFFER_LVALUE = new_buffer;

	/* copied from yy_switch_to_buffer. */
	yy_load_buffer_state(  );
	(yy_did_buffer_switch_on_eof) = 1;
}
/** Removes and deletes the top of the stack, if present.
 *  The next element becomes the new top.
 *  
 */
void yypop_buffer_state (void)
{
    	if (!YY_CURRENT_BUFFER)
		return;

	yy_delete_buffer(YY_CURRENT_BUFFER );
	YY_CURRENT_BUFFER_LVALUE = NULL;
	if ((yy_buffer_stack_top) > 0)
		--(yy_buffer_stack_top);

	if (YY_CURRENT_BUFFER) {
		yy_load_buffer_state(  );
		(yy_did_buffer_switch_on_eof) = 1;
	}
}
 \
	while ( 0 )

/* Accessor  methods (get/set functions) to struct members. */

/** Get the current line number.
 * 
 */
int yyget_lineno  (void)
{
    
    return yylineno;
}
/** Get the input stream.
 * 
 */
FILE *yyget_in  (void)
{
        return yyin;
}
/** Get the output stream.
 * 
 */
FILE *yyget_out  (void)
{
        return yyout;
}
/** Get the length of the current token.
 * 
 */
int yyget_leng  (void)
{
        return yyleng;
}
/** Get the current token.
 * 
 */

char *yyget_text  (void)
{
        return yytext;
}
/** Set the current line number.
 * @param _line_number line number
 * 
 */
void yyset_lineno (int  _line_number )
{
    
    yylineno = _line_number;
}
/** Set the input stream. This does not discard the current
 * input buffer.
 * @param _in_str A readable stream.
 * 
 * @see yy_switch_to_buffer
 */
void yyset_in (FILE *  _in_str )
{
        yyin = _in_str ;
}
void yyset_out (FILE *  _out_str )
{
        yyout = _out_str ;
}
int yyget_debug  (void)
{
        return yy_flex_debug;
}
void yyset_debug (int  _bdebug )
{
        yy_flex_debug = _bdebug ;
}
/* yylex_destroy is for both reentrant and non-reentrant scanners. */
int yylex_destroy  (void)
{
    
    /* Pop the buffer stack, destroying each element. */
	while(YY_CURRENT_BUFFER){
		yy_delete_buffer( YY_CURRENT_BUFFER  );
		YY_CURRENT_BUFFER_LVALUE = NULL;
		yypop_buffer_state();
	}

	/* Destroy the stack itself. */
	yyfree((yy_buffer_stack) );
	(yy_buffer_stack) = NULL;

    /* Reset the globals. This is important in a non-reentrant scanner so the next time
     * yylex() is called, initialization will occur. */
    yy_init_globals( );

    return 0;
}
