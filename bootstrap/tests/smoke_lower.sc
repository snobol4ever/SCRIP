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
slots1 = tree('SLOTS', '');
Append(slots1, mk_slot(SL_EQ, ''));
Append(slots1, mk_slot_expr(SL_SUBJ, tree('TT_VAR',  'OUTPUT')));
Append(slots1, mk_slot_expr(SL_REPL, tree('TT_QLIT', 'hello')));
stmt1 = mk_stmt(1, 1, slots1);
slots2 = tree('SLOTS', '');
Append(slots2, mk_slot(SL_LBL, 'END'));
Append(slots2, mk_slot(SL_END, ''));
stmt2 = mk_stmt(2, 3, slots2);
Lower_collect(stmt1);
Lower_collect(stmt2);
OUTPUT = '--- SM ---';
Lower_run();
