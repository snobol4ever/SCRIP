/* ================================================================================================================================ */
/* si_15a_literals.sc — SI-15 closing-gate test: mirrors corpus/programs/snocone/corpus/sc1_literals.sc.                            */
/* Hand-built AST equivalent to:                                                                                                    */
/*     OUTPUT = 'hello'                                                                                                             */
/*     OUTPUT = 'world'                                                                                                             */
/*     OUTPUT = 42                                                                                                                  */
/*     END                                                                                                                          */
/* The real .sc source can't yet flow through the self-hosted parser→lower→interp pipeline (depends on SL-13d).                     */
/* This test exercises the exact SM opcode sequence a real program would emit, hand-built to bypass the parser.                     */
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
Lower_collect(mk_stmt(1, 1, mk_asgn('OUTPUT', tree('TT_QLIT', 'hello'))));
Lower_collect(mk_stmt(2, 2, mk_asgn('OUTPUT', tree('TT_QLIT', 'world'))));
Lower_collect(mk_stmt(3, 3, mk_asgn('OUTPUT', tree('TT_ILIT', '42'))));
slots_end = tree('SLOTS', ''); Append(slots_end, mk_slot(SL_END, ''));
Lower_collect(mk_stmt(4, 4, slots_end));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
