/* ==================================================================================================================== */
/* si_06_exp.sc — SI-6 self-hosted test: SM_EXP (exponentiation).                                                       */
/* Hand-builds AST for: OUTPUT = 2 ^ 10  (expects 1024)                                                                 */
/* ==================================================================================================================== */
function mk_slot(tag, val)        { mk_slot = tree(tag, val); return; }
function mk_slot_expr(tag, et, s) { s = tree(tag, ''); Append(s, et); mk_slot_expr = s; return; }
function mk_bin(op, l, r, t)      { t = tree(op, ''); Append(t, l); Append(t, r); mk_bin = t; return; }
function mk_stmt(stno, line, slots, s, i) {
    s = tree('STMT', '');
    Append(s, mk_slot(SL_STNO, '' stno));
    Append(s, mk_slot(SL_LINE, '' line));
    i = 1; while (LE(i, n(slots))) { Append(s, c(slots)[i]); i = i + 1; }
    mk_stmt = s; return;
}
/* OUTPUT = 2 ^ 10 */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots1, mk_slot_expr(SL_REPL, mk_bin('TT_POW', tree('TT_ILIT', '2'), tree('TT_ILIT', '10'))));
slots2 = tree('SLOTS', ''); Append(slots2, mk_slot(SL_LBL, 'END')); Append(slots2, mk_slot(SL_END, ''));
Lower_collect(mk_stmt(1, 1, slots1));
Lower_collect(mk_stmt(2, 3, slots2));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
