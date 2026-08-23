/* ================================================================================================================================ */
/* si_16_cond_return.sc — exercises SM_RETURN_S (success-conditional return).                                                       */
/* Hand-builds AST equivalent to:                                                                                                   */
/*   X = 'hello'                                                                                                                    */
/*   EVAL(*X) :S(RETURN)    — thunk runs, last_ok=1, RETURN_S fires → top-level halt                                                */
/*   OUTPUT = 'should-not-print'                                                                                                    */
/*   END                                                                                                                            */
/* Expected: --- interp --- / --- done ---  (the 'should-not-print' line never appears because RETURN_S halts at top-level).        */
/* ================================================================================================================================ */
function mk_slot(tag, val)        { mk_slot = tree(tag, val); return; }
function mk_slot_expr(tag, et, s) { s = tree(tag, ''); Append(s, et); mk_slot_expr = s; return; }
function mk_stmt(stno, lineno, slots, s, i) {
    s = tree('STMT', '');
    Append(s, mk_slot(SL_STNO, '' stno));
    Append(s, mk_slot(SL_LINE, '' lineno));
    i = 1; while (LE(i, n(slots))) { Append(s, c(slots)[i]); i = i + 1; }
    mk_stmt = s; return;
}

/* stmt 1: X = 'hello' */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'X')));
Append(slots1, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'hello')));

/* stmt 2: EVAL(*X) :S(RETURN) — thunk succeeds, SM_RETURN_S fires at top-level → halt */
defer_x = tree('TT_DEFER', '');     Append(defer_x, tree('TT_VAR', 'X'));
eval_t  = tree('TT_FNC', 'EVAL');   Append(eval_t, defer_x);
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot_expr(SL_SUBJ, eval_t));
Append(slots2, mk_slot(SL_GOS, 'RETURN'));

/* stmt 3: OUTPUT = 'should-not-print' (never reached) */
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_EQ, ''));
Append(slots3, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots3, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'should-not-print')));

/* stmt 4: END */
slots4 = tree('SLOTS', ''); Append(slots4, mk_slot(SL_END, ''));

Lower_collect(mk_stmt(1, 1, slots1));
Lower_collect(mk_stmt(2, 2, slots2));
Lower_collect(mk_stmt(3, 3, slots3));
Lower_collect(mk_stmt(4, 4, slots4));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
