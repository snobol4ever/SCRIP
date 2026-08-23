/* ================================================================================================================================ */
/* si_14_computed_goto.sc — SI-14 self-hosted test: SM_JUMP_INDIR (computed goto via label table).                                  */
/* Hand-builds AST equivalent to:                                                                                                   */
/*   L = 'TARGET'                                                                                                                   */
/*   :($L)                  (computed goto via SM_JUMP_INDIR)                                                                       */
/*   OUTPUT = 'miss'        (skipped)                                                                                               */
/*   TARGET: OUTPUT = 'hit' (reached)                                                                                               */
/*   END                                                                                                                            */
/* Expected stdout:                                                                                                                  */
/*   --- interp ---                                                                                                                  */
/*   hit                                                                                                                            */
/*   --- done ---                                                                                                                    */
/* SM_JUMP_INDIR pops the label name from TOS and sets pc = g_labtab[name].                                                         */
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

/* stmt 1: L = 'TARGET' */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'L')));
Append(slots1, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'TARGET')));

/* stmt 2: :($L) — computed goto.  SL_GOU as expr child (not a string) triggers SM_JUMP_INDIR. */
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot_expr(SL_GOU, tree('TT_VAR', 'L')));

/* stmt 3: OUTPUT = 'miss' — should NOT execute */
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_EQ, ''));
Append(slots3, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots3, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'miss')));

/* stmt 4: TARGET: OUTPUT = 'hit' */
slots4 = tree('SLOTS', '');
Append(slots4, mk_slot(SL_LBL, 'TARGET'));
Append(slots4, mk_slot(SL_EQ, ''));
Append(slots4, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots4, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'hit')));

/* stmt 5: END */
slots5 = tree('SLOTS', ''); Append(slots5, mk_slot(SL_END, ''));

Lower_collect(mk_stmt(1, 1, slots1));
Lower_collect(mk_stmt(2, 2, slots2));
Lower_collect(mk_stmt(3, 3, slots3));
Lower_collect(mk_stmt(4, 4, slots4));
Lower_collect(mk_stmt(5, 5, slots5));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
