/* ==================================================================================================================== */
/* si_11_compare.sc — SI-11 self-hosted test: SM_ACOMP and SM_LCOMP.                                                    */
/* Hand-builds AST equivalent to:                                                                                        */
/*     LT(3, 5)  :S(YES_A)                  (stmt 1 — arithmetic LT, success goto)                                      */
/*     :(DONE)                               (stmt 2 — unconditional goto; not reached)                                  */
/* YES_A: OUTPUT = 'lt-ok'                  (stmt 3)                                                                     */
/*     LGT('beta', 'alpha')  :S(YES_L)      (stmt 4 — lexical LGT, success goto)                                        */
/*     :(DONE)                               (stmt 5 — not reached)                                                      */
/* YES_L: OUTPUT = 'lgt-ok'                 (stmt 6)                                                                     */
/*     END                                  (stmt 7)                                                                      */
/* Expected stdout (framed by driver):  lt-ok\nlgt-ok                                                                   */
/* Exercises: SM_ACOMP (TT_LT success), SM_JUMP_S, SM_LCOMP (TT_LGT success), SM_JUMP_F (not taken).                    */
/* ==================================================================================================================== */
function mk_slot(tag, val)        { mk_slot = tree(tag, val); return; }
function mk_slot_expr(tag, et, s) { s = tree(tag, ''); Append(s, et); mk_slot_expr = s; return; }
function mk_bin(op, l, r, t)      { t = tree(op, ''); Append(t, l); Append(t, r); mk_bin = t; return; }
function mk_stmt(stno, lineno, slots, s, i) {
    s = tree('STMT', '');
    Append(s, mk_slot(SL_STNO, '' stno));
    Append(s, mk_slot(SL_LINE, '' lineno));
    i = 1; while (LE(i, n(slots))) { Append(s, c(slots)[i]); i = i + 1; }
    mk_stmt = s; return;
}
/* stmt 1: LT(3, 5)  :S(YES_A)  — subject = TT_LT(3,5), no eq, goto_s = 'YES_A' */
lt_expr  = mk_bin('TT_LT', tree('TT_ILIT', '3'), tree('TT_ILIT', '5'));
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot_expr(SL_SUBJ, lt_expr));
Append(slots1, mk_slot('YES_A', ''));          /* SL_GOS slot: success label  */
/* NOTE: lower_stmt reads goto_s via stmt_attr_str(s, SL_GOS).  SL_GOS = ':goS'. */
/* Rebuild slots1 using the correct SL_GOS constant. */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot_expr(SL_SUBJ, lt_expr));
Append(slots1, mk_slot(SL_GOS, 'YES_A'));
/* stmt 2: :(DONE)  — unconditional goto; not reached when LT succeeds */
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot(SL_GOU, 'DONE'));
/* stmt 3: YES_A: OUTPUT = 'lt-ok' */
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_LBL,  'YES_A'));
Append(slots3, mk_slot(SL_EQ,   ''));
Append(slots3, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots3, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'lt-ok')));
/* stmt 4: LGT('beta', 'alpha')  :S(YES_L)  — subject = TT_LGT('beta','alpha'), goto_s = 'YES_L' */
lgt_expr = mk_bin('TT_LGT', tree('TT_QLIT', 'beta'), tree('TT_QLIT', 'alpha'));
slots4 = tree('SLOTS', '');
Append(slots4, mk_slot_expr(SL_SUBJ, lgt_expr));
Append(slots4, mk_slot(SL_GOS, 'YES_L'));
/* stmt 5: :(DONE)  — not reached */
slots5 = tree('SLOTS', '');
Append(slots5, mk_slot(SL_GOU, 'DONE'));
/* stmt 6: YES_L: OUTPUT = 'lgt-ok' */
slots6 = tree('SLOTS', '');
Append(slots6, mk_slot(SL_LBL,  'YES_L'));
Append(slots6, mk_slot(SL_EQ,   ''));
Append(slots6, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots6, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'lgt-ok')));
/* stmt 7: DONE: END */
slots7 = tree('SLOTS', '');
Append(slots7, mk_slot(SL_LBL, 'DONE'));
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
