/* ==================================================================================================================== */
/* si_09_pat_fns.sc — SI-9 self-hosted test: pattern function calls (LEN + SPAN) with captures.                          */
/* Hand-builds AST for:                                                                                                  */
/*     &ANCHOR = 0; &FULLSCAN = 1;                                                                                       */
/*     S = 'abc123';                                                                                                     */
/*     S ? LEN(3) . LET SPAN('0123456789') . NUM;                                                                        */
/*     OUTPUT = 'LET=' LET;                                                                                               */
/*     OUTPUT = 'NUM=' NUM;                                                                                              */
/*     END                                                                                                               */
/* ==================================================================================================================== */
function mk_slot(tag, val)        { mk_slot = tree(tag, val); return; }
function mk_slot_expr(tag, et, s) { s = tree(tag, ''); Append(s, et); mk_slot_expr = s; return; }
function mk_bin(tag, l, r, t) { t = tree(tag, ''); Append(t, l); Append(t, r); mk_bin = t; return; }
function mk_unary(tag, v, t) { t = tree(tag, ''); Append(t, v); mk_unary = t; return; }
function mk_stmt(stno, line, slots, s, i) {
    s = tree('STMT', '');
    Append(s, mk_slot(SL_STNO, '' stno));
    Append(s, mk_slot(SL_LINE, '' line));
    i = 1; while (LE(i, n(slots))) { Append(s, c(slots)[i]); i = i + 1; }
    mk_stmt = s; return;
}
/* stmt 1: &ANCHOR = 0 */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_KEYWORD', 'ANCHOR')));
Append(slots1, mk_slot_expr(SL_REPL, tree('TT_ILIT', '0')));
/* stmt 2: &FULLSCAN = 1 */
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot(SL_EQ, ''));
Append(slots2, mk_slot_expr(SL_SUBJ, tree('TT_KEYWORD', 'FULLSCAN')));
Append(slots2, mk_slot_expr(SL_REPL, tree('TT_ILIT', '1')));
/* stmt 3: S = 'abc123' */
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_EQ, ''));
Append(slots3, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'S')));
Append(slots3, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'abc123')));
/* stmt 4: S ? LEN(3) . LET SPAN('0123456789') . NUM */
len_t  = mk_unary('TT_LEN',  tree('TT_ILIT', '3'));
cap1_t = mk_bin('TT_CAPT_COND_ASGN', len_t, tree('TT_VAR', 'LET'));
span_t = mk_unary('TT_SPAN', tree('TT_QLIT', '0123456789'));
cap2_t = mk_bin('TT_CAPT_COND_ASGN', span_t, tree('TT_VAR', 'NUM'));
pat_t  = mk_bin('TT_CAT', cap1_t, cap2_t);
slots4 = tree('SLOTS', '');
Append(slots4, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'S')));
Append(slots4, mk_slot_expr(SL_PAT,  pat_t));
/* stmt 5: OUTPUT = 'LET=' LET   — concat of 'LET=' and LET */
concat1 = mk_bin('TT_CAT', tree('TT_QLIT', 'LET='), tree('TT_VAR', 'LET'));
slots5 = tree('SLOTS', '');
Append(slots5, mk_slot(SL_EQ, ''));
Append(slots5, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots5, mk_slot_expr(SL_REPL, concat1));
/* stmt 6: OUTPUT = 'NUM=' NUM */
concat2 = mk_bin('TT_CAT', tree('TT_QLIT', 'NUM='), tree('TT_VAR', 'NUM'));
slots6 = tree('SLOTS', '');
Append(slots6, mk_slot(SL_EQ, ''));
Append(slots6, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots6, mk_slot_expr(SL_REPL, concat2));
/* stmt 7: END */
slots7 = tree('SLOTS', ''); Append(slots7, mk_slot(SL_LBL, 'END')); Append(slots7, mk_slot(SL_END, ''));
Lower_collect(mk_stmt(1, 1, slots1));
Lower_collect(mk_stmt(2, 2, slots2));
Lower_collect(mk_stmt(3, 3, slots3));
Lower_collect(mk_stmt(4, 4, slots4));
Lower_collect(mk_stmt(5, 5, slots5));
Lower_collect(mk_stmt(6, 6, slots6));
Lower_collect(mk_stmt(7, 8, slots7));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
