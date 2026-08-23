/* ==================================================================================================================== */
/* sm_lower_test.sc — SL-9 test driver for GOAL-SNOCONE-SM-LOWER.                                                       */
/*                                                                                                                      */
/* Builds a small TT_PROGRAM by hand with three statements:                                                              */
/*   stmt 1 — assignment:     X = 'hello'                                                                                */
/*   stmt 2 — pattern match:  X 'hi' = 'bye'                                                                             */
/*   stmt 3 — END:             label END, no body                                                                        */
/* Calls Lower_run() to lower the program and dump the SM_Program text.                                                  */
/* Output is byte-compared against sm_lower_test.ref by the SL-9 gate.                                                   */
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
    s = tree('TT_STMT', '');
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
/* stmt 1: X = 'hello'                                                                                                  */
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR',  'X')));
Append(slots1, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'hello')));
stmt1 = mk_stmt(1, 1, slots1);
/* ==================================================================================================================== */
/* stmt 2: X 'hi' = 'bye'                                                                                                */
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot(SL_EQ, ''));
Append(slots2, mk_slot_expr(SL_SUBJ, tree('TT_VAR',  'X')));
Append(slots2, mk_slot_expr(SL_PAT,  tree('TT_QLIT', 'hi')));
Append(slots2, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'bye')));
stmt2 = mk_stmt(2, 2, slots2);
/* ==================================================================================================================== */
/* stmt 3: END label, no body                                                                                            */
slots3 = tree('SLOTS', '');
Append(slots3, mk_slot(SL_LBL, 'END'));
Append(slots3, mk_slot(SL_END, ''));
stmt3 = mk_stmt(3, 5, slots3);
/* ==================================================================================================================== */
Lower_collect(stmt1);
Lower_collect(stmt2);
Lower_collect(stmt3);
OUTPUT = '--- SM ---';
Lower_run();
