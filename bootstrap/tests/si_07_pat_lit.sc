/* ==================================================================================================================== */
/* si_07_pat_lit.sc — SI-7 self-hosted test: SM_PAT_LIT + SM_PAT_DEREF + SM_EXEC_STMT.                                  */
/* Hand-builds AST for:                                                                                                  */
/*     S = 'hello'              (stmt 1)                                                                                 */
/*     S ? 'ell' = 'ELL'        (stmt 2 — pattern match with replacement)                                               */
/*     OUTPUT = S               (stmt 3)                                                                                 */
/*     END                      (stmt 4)                                                                                 */
/* ==================================================================================================================== */
function mk_slot(tag, val)        { mk_slot = tree(tag, val); return; }
function mk_slot_expr(tag, et, s) { s = tree(tag, ''); Append(s, et); mk_slot_expr = s; return; }
function mk_stmt(stno, line, slots, s, i) {
    s = tree('STMT', '');
    Append(s, mk_slot(SL_STNO, '' stno));
    Append(s, mk_slot(SL_LINE, '' line));
    i = 1; while (LE(i, n(slots))) { Append(s, c(slots)[i]); i = i + 1; }
    mk_stmt = s; return;
}
/* stmt 1: S = 'hello' */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'S')));
Append(slots1, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'hello')));
/* stmt 2: S ? 'ell' = 'ELL'  — subject=S, pattern='ell', replacement='ELL' */
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot_expr(SL_SUBJ, tree('TT_VAR',  'S')));
Append(slots2, mk_slot_expr(SL_PAT,  tree('TT_QLIT', 'ell')));
Append(slots2, mk_slot(SL_EQ, ''));
Append(slots2, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'ELL')));
/* stmt 3: OUTPUT = S */
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_EQ, ''));
Append(slots3, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots3, mk_slot_expr(SL_REPL, tree('TT_VAR', 'S')));
/* stmt 4: END */
slots4 = tree('SLOTS', ''); Append(slots4, mk_slot(SL_LBL, 'END')); Append(slots4, mk_slot(SL_END, ''));
Lower_collect(mk_stmt(1, 1, slots1));
Lower_collect(mk_stmt(2, 2, slots2));
Lower_collect(mk_stmt(3, 3, slots3));
Lower_collect(mk_stmt(4, 5, slots4));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
