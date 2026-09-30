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
/* ==================================================================================================================== */
/* A value a node will carry, held from the token that names it to the one Reduce that builds the node -- the pattern  */
/* recognises a call's name before its arguments, whose own actions run in between (the tree is built once, Lon 2026-09-30). */
struct link_val { next, value }
function PushVal(v) {
    $'#V' = link_val($'#V', v);
    PushVal = .dummy;
    nreturn;
}
function PopVal() {
    PopVal = value($'#V');
    $'#V' = next($'#V');
    return;
}
