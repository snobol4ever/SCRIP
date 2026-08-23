/* ================================================================================================================================ */
/* si_13_proc.sc — SI-13 self-hosted test: SM_CALL_EXPRESSION + SM_RETURN + SM_FRETURN.                                            */
/* Hand-builds AST equivalent to:                                                                                                   */
/*   X = 'hello'                                                                                                                    */
/*   OUTPUT = EVAL(*X)          (thunk SM_CALL_EXPRESSION → SM_RETURN with TOS='hello')                                            */
/*   OUTPUT = EVAL(*(3 + 4))    (arithmetic thunk → SM_RETURN with TOS=7)                                                          */
/*   EVAL(*FAIL_THUNK) :F(FRET_FAILED)    (FRETURN thunk → last_ok=0)                                                              */
/*   :(SKIP)                                                                                                                        */
/*   FRET_FAILED: OUTPUT = 'fret-ok'                                                                                                */
/*   SKIP: END                                                                                                                      */
/* Expected stdout:                                                                                                                  */
/*   --- interp ---                                                                                                                  */
/*   hello                                                                                                                          */
/*   7                                                                                                                              */
/*   fret-ok                                                                                                                        */
/*   --- done ---                                                                                                                    */
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
function mk_eval_defer(expr_t, d, e) {
    /* Build TT_FNC(v='EVAL', child=TT_DEFER(expr)) — triggers SM_CALL_EXPRESSION path in lower_fnc. */
    d = tree('TT_DEFER', '');   Append(d, expr_t);
    e = tree('TT_FNC', 'EVAL'); Append(e, d);
    mk_eval_defer = e; return;
}

/* stmt 1: X = 'hello' */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'X')));
Append(slots1, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'hello')));

/* stmt 2: OUTPUT = EVAL(*X) */
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot(SL_EQ, ''));
Append(slots2, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots2, mk_slot_expr(SL_REPL, mk_eval_defer(tree('TT_VAR', 'X'))));

/* stmt 3: OUTPUT = EVAL(*(3 + 4)) */
add_t = tree('TT_ADD', '');
Append(add_t, tree('TT_ILIT', '3'));
Append(add_t, tree('TT_ILIT', '4'));
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_EQ, ''));
Append(slots3, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots3, mk_slot_expr(SL_REPL, mk_eval_defer(add_t)));

/* stmt 4: EVAL(*PROC_FAIL) — TT_PROC_FAIL lowers to SM_PUSH_NULL + SM_FRETURN; wrapped in EVAL(*) gives SM_CALL_EXPRESSION */
slots4 = tree('SLOTS', '');
Append(slots4, mk_slot_expr(SL_SUBJ, mk_eval_defer(tree('TT_PROC_FAIL', ''))));
Append(slots4, mk_slot(SL_GOF, 'FRET_FAILED'));

/* stmt 5: :(SKIP) — not reached when FRETURN fires */
slots5 = tree('SLOTS', ''); Append(slots5, mk_slot(SL_GOU, 'SKIP'));

/* stmt 6: FRET_FAILED: OUTPUT = 'fret-ok' */
slots6 = tree('SLOTS', '');
Append(slots6, mk_slot(SL_LBL,  'FRET_FAILED'));
Append(slots6, mk_slot(SL_EQ,   ''));
Append(slots6, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots6, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'fret-ok')));

/* stmt 7: SKIP: END */
slots7 = tree('SLOTS', '');
Append(slots7, mk_slot(SL_LBL, 'SKIP'));
Append(slots7, mk_slot(SL_END, ''));

Lower_collect(mk_stmt(1, 1, slots1));
Lower_collect(mk_stmt(2, 2, slots2));
Lower_collect(mk_stmt(3, 3, slots3));
Lower_collect(mk_stmt(4, 4, slots4));
Lower_collect(mk_stmt(5, 5, slots5));
Lower_collect(mk_stmt(6, 6, slots6));
Lower_collect(mk_stmt(7, 7, slots7));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
