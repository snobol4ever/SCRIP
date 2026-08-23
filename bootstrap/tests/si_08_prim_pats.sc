/* ==================================================================================================================== */
/* si_08_prim_pats.sc — SI-8 self-hosted test: primitive patterns (ARB capture) + CAT.                                  */
/* Hand-builds AST for:                                                                                                  */
/*     &ANCHOR = 0; &FULLSCAN = 1;                                                                                       */
/*     S = 'abc';                                                                                                        */
/*     S ? ARB . X 'c';                                                                                                  */
/*     OUTPUT = X;                                                                                                       */
/*     END                                                                                                               */
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
function mk_bin(tag, l, r, t) { t = tree(tag, ''); Append(t, l); Append(t, r); mk_bin = t; return; }
/* stmt 1: &ANCHOR = 0  — use TT_KEYWORD for &ANCHOR */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_KEYWORD', 'ANCHOR')));
Append(slots1, mk_slot_expr(SL_REPL, tree('TT_ILIT', '0')));
/* stmt 2: &FULLSCAN = 1 */
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot(SL_EQ, ''));
Append(slots2, mk_slot_expr(SL_SUBJ, tree('TT_KEYWORD', 'FULLSCAN')));
Append(slots2, mk_slot_expr(SL_REPL, tree('TT_ILIT', '1')));
/* stmt 3: S = 'abc' */
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_EQ, ''));
Append(slots3, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'S')));
Append(slots3, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'abc')));
/* stmt 4: S ? ARB . X 'c'   — pattern = TT_CAT(TT_CAPT_COND_ASGN(TT_ARB, TT_VAR 'X'), TT_QLIT 'c') */
cap_t = mk_bin('TT_CAPT_COND_ASGN', tree('TT_ARB', ''), tree('TT_VAR', 'X'));
pat_t = mk_bin('TT_CAT', cap_t, tree('TT_QLIT', 'c'));
slots4 = tree('SLOTS', '');
Append(slots4, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'S')));
Append(slots4, mk_slot_expr(SL_PAT,  pat_t));
/* stmt 5: OUTPUT = X */
slots5 = tree('SLOTS', '');
Append(slots5, mk_slot(SL_EQ, ''));
Append(slots5, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots5, mk_slot_expr(SL_REPL, tree('TT_VAR', 'X')));
/* stmt 6: END */
slots6 = tree('SLOTS', ''); Append(slots6, mk_slot(SL_LBL, 'END')); Append(slots6, mk_slot(SL_END, ''));
Lower_collect(mk_stmt(1, 1, slots1));
Lower_collect(mk_stmt(2, 2, slots2));
Lower_collect(mk_stmt(3, 3, slots3));
Lower_collect(mk_stmt(4, 4, slots4));
Lower_collect(mk_stmt(5, 5, slots5));
Lower_collect(mk_stmt(6, 7, slots6));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
