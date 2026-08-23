/* ================================================================================================================================ */
/* si_17a_assign.sc — SI-17a: mirror of programs/snocone/corpus/sc2_assign.sc.                                                      */
/* Hand-built AST equivalent to:                                                                                                    */
/*     x = 'hello'                                                                                                                  */
/*     y = 42                                                                                                                       */
/*     OUTPUT = x                                                                                                                   */
/*     OUTPUT = y                                                                                                                   */
/*     z = x                                                                                                                        */
/*     OUTPUT = z                                                                                                                   */
/*     END                                                                                                                          */
/* Exercises SM_PUSH_LIT_S, SM_PUSH_LIT_I, SM_STORE_VAR (user vars), SM_PUSH_VAR — Phase 1 only.                                    */
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
function mk_asgn(var_name, value_t, s) {
    s = tree('SLOTS', '');
    Append(s, mk_slot(SL_EQ, ''));
    Append(s, mk_slot_expr(SL_SUBJ, tree('TT_VAR', var_name)));
    Append(s, mk_slot_expr(SL_REPL, value_t));
    mk_asgn = s; return;
}
Lower_collect(mk_stmt(1, 1, mk_asgn('x',      tree('TT_QLIT', 'hello'))));
Lower_collect(mk_stmt(2, 2, mk_asgn('y',      tree('TT_ILIT', '42'))));
Lower_collect(mk_stmt(3, 3, mk_asgn('OUTPUT', tree('TT_VAR', 'x'))));
Lower_collect(mk_stmt(4, 4, mk_asgn('OUTPUT', tree('TT_VAR', 'y'))));
Lower_collect(mk_stmt(5, 5, mk_asgn('z',      tree('TT_VAR', 'x'))));
Lower_collect(mk_stmt(6, 6, mk_asgn('OUTPUT', tree('TT_VAR', 'z'))));
slots_end = tree('SLOTS', ''); Append(slots_end, mk_slot(SL_END, ''));
Lower_collect(mk_stmt(7, 7, slots_end));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
