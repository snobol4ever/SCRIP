/* ================================================================================================================================ */
/* si_17b_control.sc — SI-17b: mirror of programs/snocone/corpus/sc4_control.sc using SNOBOL4-style :S/:F gotos.                    */
/* The original `if (cond) a; else b;` Snocone source lowers to roughly:                                                            */
/*   GT(x, 5) :F(L_else)                                                                                                            */
/*   OUTPUT = 'big' :(L_end)                                                                                                        */
/*   L_else: OUTPUT = 'small'                                                                                                       */
/*   L_end:                                                                                                                          */
/* We hand-build that flatter equivalent — same observable output.                                                                  */
/*                                                                                                                                  */
/* Hand-built equivalent of:                                                                                                        */
/*   x = 10                                                                                                                         */
/*   if (x > 5) OUTPUT = 'big'  else OUTPUT = 'small'                                                                              */
/*   if (x < 5) OUTPUT = 'small' else OUTPUT = 'big'                                                                              */
/*   y = 3                                                                                                                          */
/*   if (y == 3) OUTPUT = 'three'                                                                                                   */
/*   if (y == 4) OUTPUT = 'four'  else OUTPUT = 'not four'                                                                          */
/* Expected: big / big / three / not four                                                                                           */
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
function mk_call(fname, args, t, i) {
    /* TT_FNC node, v(t)='', c(t)[1]=callee with v(callee)=fname, args follow */
    t = tree('TT_FNC', '');
    Append(t, tree('TT_VAR', fname));
    i = 1; while (LE(i, n(args))) { Append(t, c(args)[i]); i = i + 1; }
    mk_call = t; return;
}
function mk_args2(a, b, t) { t = tree('ARGS', ''); Append(t, a); Append(t, b); mk_args2 = t; return; }
function mk_asgn(var_name, value_t, s) {
    s = tree('SLOTS', '');
    Append(s, mk_slot(SL_EQ, ''));
    Append(s, mk_slot_expr(SL_SUBJ, tree('TT_VAR', var_name)));
    Append(s, mk_slot_expr(SL_REPL, value_t));
    mk_asgn = s; return;
}
function mk_test_else(fname, l, r, then_label, else_label, s) {
    /* `if (fname(l,r)) ...  else ...` flattened to: GT(l,r) :F(else_label) — subject is the call,
     * goto_f is else_label.  Then we expect callers to emit then-body + goto_u skip, else: else-body. */
    s = tree('SLOTS', '');
    Append(s, mk_slot_expr(SL_SUBJ, mk_call(fname, mk_args2(l, r))));
    Append(s, mk_slot(SL_GOF, else_label));
    mk_test_else = s; return;
}
function mk_label_asgn(lbl, var_name, value_t, s) {
    /* Labelled assignment: LBL: var = value */
    s = tree('SLOTS', '');
    Append(s, mk_slot(SL_LBL, lbl));
    Append(s, mk_slot(SL_EQ, ''));
    Append(s, mk_slot_expr(SL_SUBJ, tree('TT_VAR', var_name)));
    Append(s, mk_slot_expr(SL_REPL, value_t));
    mk_label_asgn = s; return;
}
function mk_asgn_goto(var_name, value_t, goto_label, s) {
    s = tree('SLOTS', '');
    Append(s, mk_slot(SL_EQ, ''));
    Append(s, mk_slot_expr(SL_SUBJ, tree('TT_VAR', var_name)));
    Append(s, mk_slot_expr(SL_REPL, value_t));
    Append(s, mk_slot(SL_GOU, goto_label));
    mk_asgn_goto = s; return;
}
function mk_label_only(lbl, s) {
    s = tree('SLOTS', ''); Append(s, mk_slot(SL_LBL, lbl));
    mk_label_only = s; return;
}

stno = 1;
function next_stno(v) { v = stno; stno = stno + 1; next_stno = v; return; }

/* x = 10 */
Lower_collect(mk_stmt(next_stno(), 1, mk_asgn('x', tree('TT_ILIT', '10'))));

/* if (x > 5) OUTPUT = 'big'; else OUTPUT = 'small'; */
Lower_collect(mk_stmt(next_stno(), 2, mk_test_else('GT', tree('TT_VAR', 'x'), tree('TT_ILIT', '5'), '', 'else1')));
Lower_collect(mk_stmt(next_stno(), 2, mk_asgn_goto('OUTPUT', tree('TT_QLIT', 'big'), 'end1')));
Lower_collect(mk_stmt(next_stno(), 2, mk_label_asgn('else1', 'OUTPUT', tree('TT_QLIT', 'small'))));
Lower_collect(mk_stmt(next_stno(), 2, mk_label_only('end1')));

/* if (x < 5) OUTPUT = 'small'; else OUTPUT = 'big'; */
Lower_collect(mk_stmt(next_stno(), 3, mk_test_else('LT', tree('TT_VAR', 'x'), tree('TT_ILIT', '5'), '', 'else2')));
Lower_collect(mk_stmt(next_stno(), 3, mk_asgn_goto('OUTPUT', tree('TT_QLIT', 'small'), 'end2')));
Lower_collect(mk_stmt(next_stno(), 3, mk_label_asgn('else2', 'OUTPUT', tree('TT_QLIT', 'big'))));
Lower_collect(mk_stmt(next_stno(), 3, mk_label_only('end2')));

/* y = 3 */
Lower_collect(mk_stmt(next_stno(), 4, mk_asgn('y', tree('TT_ILIT', '3'))));

/* if (y == 3) OUTPUT = 'three';   (no else — just jump over on fail) */
Lower_collect(mk_stmt(next_stno(), 5, mk_test_else('EQ', tree('TT_VAR', 'y'), tree('TT_ILIT', '3'), '', 'end3')));
Lower_collect(mk_stmt(next_stno(), 5, mk_asgn('OUTPUT', tree('TT_QLIT', 'three'))));
Lower_collect(mk_stmt(next_stno(), 5, mk_label_only('end3')));

/* if (y == 4) OUTPUT = 'four'; else OUTPUT = 'not four'; */
Lower_collect(mk_stmt(next_stno(), 6, mk_test_else('EQ', tree('TT_VAR', 'y'), tree('TT_ILIT', '4'), '', 'else4')));
Lower_collect(mk_stmt(next_stno(), 6, mk_asgn_goto('OUTPUT', tree('TT_QLIT', 'four'), 'end4')));
Lower_collect(mk_stmt(next_stno(), 6, mk_label_asgn('else4', 'OUTPUT', tree('TT_QLIT', 'not four'))));
Lower_collect(mk_stmt(next_stno(), 6, mk_label_only('end4')));

slots_end = tree('SLOTS', ''); Append(slots_end, mk_slot(SL_END, ''));
Lower_collect(mk_stmt(next_stno(), 99, slots_end));
OUTPUT = '--- interp ---';
lower(g_program);
sm_interp_run();
OUTPUT = '--- done ---';
