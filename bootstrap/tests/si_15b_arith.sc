/* ================================================================================================================================ */
/* si_15b_arith.sc — SI-15 closing-gate test: mirrors corpus/programs/snocone/corpus/sc3_arith.sc.                                  */
/* Hand-built AST equivalent to:                                                                                                    */
/*     OUTPUT = 3 + 4                                                                                                               */
/*     OUTPUT = 10 - 3                                                                                                              */
/*     OUTPUT = 6 * 7                                                                                                               */
/*     OUTPUT = 20 / 4                                                                                                              */
/*     OUTPUT = 2 ^ 8                                                                                                               */
/*     x = 5                                                                                                                        */
/*     OUTPUT = x + x                                                                                                               */
/*     OUTPUT = x * 3                                                                                                               */
/*     END                                                                                                                          */
/* Exercises: SM_ADD/SUB/MUL/DIV/EXP, SM_PUSH_LIT_I, SM_PUSH_VAR, SM_STORE_VAR — the SI-2..SI-6 opcodes.                            */
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
function mk_bin(op_tag, l, r, t)  { t = tree(op_tag, ''); Append(t, l); Append(t, r); mk_bin = t; return; }
function mk_asgn(var_name, value_t, s) {
    s = tree('SLOTS', '');
    Append(s, mk_slot(SL_EQ, ''));
    Append(s, mk_slot_expr(SL_SUBJ, tree('TT_VAR', var_name)));
    Append(s, mk_slot_expr(SL_REPL, value_t));
    mk_asgn = s; return;
}
Lower_collect(mk_stmt(1, 1, mk_asgn('OUTPUT', mk_bin('TT_ADD', tree('TT_ILIT', '3'),  tree('TT_ILIT', '4')))));
Lower_collect(mk_stmt(2, 2, mk_asgn('OUTPUT', mk_bin('TT_SUB', tree('TT_ILIT', '10'), tree('TT_ILIT', '3')))));
Lower_collect(mk_stmt(3, 3, mk_asgn('OUTPUT', mk_bin('TT_MUL', tree('TT_ILIT', '6'),  tree('TT_ILIT', '7')))));
Lower_collect(mk_stmt(4, 4, mk_asgn('OUTPUT', mk_bin('TT_DIV', tree('TT_ILIT', '20'), tree('TT_ILIT', '4')))));
Lower_collect(mk_stmt(5, 5, mk_asgn('OUTPUT', mk_bin('TT_POW', tree('TT_ILIT', '2'),  tree('TT_ILIT', '8')))));
Lower_collect(mk_stmt(6, 6, mk_asgn('x',      tree('TT_ILIT', '5'))));
Lower_collect(mk_stmt(7, 7, mk_asgn('OUTPUT', mk_bin('TT_ADD', tree('TT_VAR', 'x'),   tree('TT_VAR', 'x')))));
Lower_collect(mk_stmt(8, 8, mk_asgn('OUTPUT', mk_bin('TT_MUL', tree('TT_VAR', 'x'),   tree('TT_ILIT', '3')))));
slots_end = tree('SLOTS', ''); Append(slots_end, mk_slot(SL_END, ''));
Lower_collect(mk_stmt(9, 9, slots_end));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
