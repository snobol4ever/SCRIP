/* ================================================================================================================================ */
/* si_15c_pat_replace.sc — SI-15 closing-gate test: pattern match + replace.                                                        */
/* Hand-built AST equivalent to:                                                                                                    */
/*   S = 'hello world'                                                                                                              */
/*   S 'world' = 'there'    (named-subject pattern match with replacement)                                                          */
/*   OUTPUT = S                                                                                                                     */
/*   END                                                                                                                            */
/* Exercises: SM_PAT_LIT, SM_EXEC_STMT (named subject + has_repl), SM_PUSH_VAR / SM_STORE_VAR — SI-7 closure on a real-shape       */
/* pattern statement.                                                                                                               */
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

/* stmt 1: S = 'hello world' */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'S')));
Append(slots1, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'hello world')));

/* stmt 2: S 'world' = 'there' — named-subject + pattern + repl */
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot(SL_EQ, ''));
Append(slots2, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'S')));
Append(slots2, mk_slot_expr(SL_PAT,  tree('TT_QLIT', 'world')));
Append(slots2, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'there')));

/* stmt 3: OUTPUT = S */
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_EQ, ''));
Append(slots3, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots3, mk_slot_expr(SL_REPL, tree('TT_VAR', 'S')));

/* stmt 4: END */
slots4 = tree('SLOTS', ''); Append(slots4, mk_slot(SL_END, ''));

&ANCHOR   = 0;
&FULLSCAN = 1;
Lower_collect(mk_stmt(1, 1, slots1));
Lower_collect(mk_stmt(2, 2, slots2));
Lower_collect(mk_stmt(3, 3, slots3));
Lower_collect(mk_stmt(4, 4, slots4));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
