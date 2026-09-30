/* ==================================================================================================================== */
function Shift(t, v, s) {
    v ? (POS(0) *whitespace) = ;
    s = tree(t, v);
    Push(s);
    OUTPUT = GT(xTrace, 3) 'Shift(' t ', ' v ')';
    if (Shift = IDENT(v) .v(s)) { nreturn; }
    Shift = DIFFER(v) .dummy;
    nreturn;
}
/* ==================================================================================================================== */
function Reduce(t, n, v, c, i, r) {
    Reduce = .dummy;
    if (IDENT(DATATYPE(t), 'EXPRESSION')) {
        if (~(t = EVAL(t))) { nreturn; }
    }
    if (IDENT(DATATYPE(n), 'EXPRESSION')) {
        if (~(n = EVAL(n))) { nreturn; }
    }
    OUTPUT = GT(xTrace, 3) 'Reduce(' t ', ' n ')';
    if (IDENT(n, 0)) {
        r = tree(t, v, 0);
        Push(r);
        nreturn;
    }
    c = ARRAY('1:' n);
    i = n + 1;
    while (i = GT(i, 1) i - 1) {
        c[i] = Pop();
    }
    r = tree(t, v, n, c);
    Push(r);
    nreturn;
}
