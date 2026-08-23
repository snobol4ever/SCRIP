/* ==================================================================================================================== */
/* si_12_call_builtin.sc — SI-12 self-hosted test: SM_CALL_FN via host APPLY().                                         */
/* Hand-builds AST equivalent to:                                                                                        */
/*     OUTPUT = SIZE('hello')          stmt 1 — 1-arg builtin                                                           */
/*     OUTPUT = SUBSTR('hello', 2, 3)  stmt 2 — 3-arg builtin                                                           */
/*     OUTPUT = TRIM('  hi  ')         stmt 3 — 1-arg builtin (string result)                                           */
/*     END                             stmt 4                                                                            */
/* Expected stdout (framed by driver):                                                                                   */
/*     --- interp ---                                                                                                    */
/*     5                                                                                                                 */
/*     ell                                                                                                               */
/*       hi                                                                                                              */
/*     --- done ---                                                                                                      */
/* Exercises: SM_CALL_FN with nargs=1, nargs=3 via host APPLY().                                                         */
/* ==================================================================================================================== */
function mk_slot(tag, val)        { mk_slot = tree(tag, val); return; }
function mk_slot_expr(tag, et, s) { s = tree(tag, ''); Append(s, et); mk_slot_expr = s; return; }
function mk_stmt(stno, lineno, slots, s, i) {
    s = tree('STMT', '');
    Append(s, mk_slot(SL_STNO, '' stno));
    Append(s, mk_slot(SL_LINE, '' lineno));
    i = 1; while (LE(i, n(slots))) { Append(s, c(slots)[i]); i = i + 1; }
    mk_stmt = s; return;
}
function mk_call(fname, nargs_ary, t, i) {
    /* Build TT_FNC('', TT_VAR(fname), arg1, ..., argN) from a SLOTS-style tree of arg exprs. */
    t = tree('TT_FNC', '');
    Append(t, tree('TT_VAR', fname));
    i = 1; while (LE(i, n(nargs_ary))) { Append(t, c(nargs_ary)[i]); i = i + 1; }
    mk_call = t; return;
}
/* stmt 1: OUTPUT = SIZE('hello')  — 1-arg */
args1 = tree('ARGS', ''); Append(args1, tree('TT_QLIT', 'hello'));
fnc1 = mk_call('SIZE', args1);
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots1, mk_slot_expr(SL_REPL, fnc1));
/* stmt 2: OUTPUT = SUBSTR('hello', 2, 3)  — 3-arg */
args2 = tree('ARGS', '');
Append(args2, tree('TT_QLIT', 'hello'));
Append(args2, tree('TT_ILIT', '2'));
Append(args2, tree('TT_ILIT', '3'));
fnc2 = mk_call('SUBSTR', args2);
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot(SL_EQ, ''));
Append(slots2, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots2, mk_slot_expr(SL_REPL, fnc2));
/* stmt 3: OUTPUT = TRIM('  hi  ')  — 1-arg */
args3 = tree('ARGS', ''); Append(args3, tree('TT_QLIT', '  hi  '));
fnc3 = mk_call('TRIM', args3);
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_EQ, ''));
Append(slots3, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots3, mk_slot_expr(SL_REPL, fnc3));
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
