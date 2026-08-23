/* ==================================================================================================================== */
/* sm_interp_test.sc — SI-3 + SI-4 self-hosted test.                                                                     */
/* Hand-builds an AST equivalent to:                                                                                     */
/*     X = 2 + 3                                                                                                         */
/*     OUTPUT = X * 4                                                                                                    */
/*     END                                                                                                               */
/* Lowers it via lower(), executes via sm_interp_run().  Expected stdout: '20'.                                           */
/* Exercises: SM_PUSH_LIT_I, SM_ADD, SM_STORE_VAR (no-print target), SM_PUSH_VAR, SM_MUL, SM_STORE_VAR (OUTPUT print).    */
/* ==================================================================================================================== */
function mk_slot(tag, val, slot) {
    slot = tree(tag, val);
    mk_slot = slot;
    return;
}
/* ==================================================================================================================== */
function mk_slot_expr(tag, expr_tree, slot) {
    slot = tree(tag, '');
    Append(slot, expr_tree);
    mk_slot_expr = slot;
    return;
}
/* ==================================================================================================================== */
function mk_stmt(stno, line, slots_seq, s, i) {
    s = tree('STMT', '');
    Append(s, mk_slot(SL_STNO, '' stno));
    Append(s, mk_slot(SL_LINE, '' line));
    i = 1;
    while (LE(i, n(slots_seq))) {
        Append(s, c(slots_seq)[i]);
        i = i + 1;
    }
    mk_stmt = s;
    return;
}
/* ==================================================================================================================== */
function mk_bin(op_tag, lhs, rhs, t) {
    t = tree(op_tag, '');
    Append(t, lhs);
    Append(t, rhs);
    mk_bin = t;
    return;
}
/* ==================================================================================================================== */
/* stmt 1: X = 2 + 3                                                                                                     */
add_expr = mk_bin('TT_ADD', tree('TT_ILIT', '2'), tree('TT_ILIT', '3'));
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'X')));
Append(slots1, mk_slot_expr(SL_REPL, add_expr));
stmt1 = mk_stmt(1, 1, slots1);
/* ==================================================================================================================== */
/* stmt 2: OUTPUT = X * 4                                                                                                */
mul_expr = mk_bin('TT_MUL', tree('TT_VAR', 'X'), tree('TT_ILIT', '4'));
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot(SL_EQ, ''));
Append(slots2, mk_slot_expr(SL_SUBJ, tree('TT_VAR', 'OUTPUT')));
Append(slots2, mk_slot_expr(SL_REPL, mul_expr));
stmt2 = mk_stmt(2, 2, slots2);
/* ==================================================================================================================== */
/* stmt 3: END                                                                                                           */
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_LBL, 'END'));
Append(slots3, mk_slot(SL_END, ''));
stmt3 = mk_stmt(3, 5, slots3);
Lower_collect(stmt1);
Lower_collect(stmt2);
Lower_collect(stmt3);
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
