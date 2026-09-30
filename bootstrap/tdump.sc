/* ==================================================================================================================== */
function TValue(x, i) {
    if (TValue = IDENT(t(x), 'TT_NUL') '(TT_NUL)')                           { return; }
    if (TValue = IDENT(t(x), 'TT_CUT') '(TT_CUT)')                           { return; }
    if (TValue = IDENT(t(x), 'TT_QLIT')     '(' t(x) ' "' CQize(v(x)) '")')      { return; }
    if (TValue = IDENT(t(x), 'TT_CSET')     '(' t(x) ' "' CQize(v(x)) '")')      { return; }
    if (IDENT(t(x), 'TT_FLIT')) {
        fval = '' CONVERT(v(x), 'REAL');
        fval ('.' BREAK('0') | '.') SPAN('0') . zeros;
        while (DIFFER(zeros)) {
            fval = REPLACE(fval, zeros, '');
            zeros = '';
            fval ('.' BREAK('0') | '.') SPAN('0') . zeros;
        }
        fval SPAN('0123456789' &UCASE &LCASE '+' '-') . pre;
        if (DIFFER(pre) IDENT(SIZE(pre) + 1, SIZE(fval))) fval = pre;
        TValue = '(' t(x) ' ' fval ')';
        return;
    }
    if (TValue = IDENT(t(x), 'float')      v(x))                   { return; }
    if (TValue = IDENT(t(x), 'integer')    v(x))                   { return; }
    if (TValue = IDENT(t(x), 'bool')       v(x))                   { return; }
    if (TValue = IDENT(t(x), 'datetime')   "'" SqlSQize(v(x)) "'") { return; }
    if (TValue = IDENT(t(x), 'character')  "'" SqlSQize(v(x)) "'") { return; }
    if (TValue = IDENT(t(x), 'string')     "'" SqlSQize(v(x)) "'") { return; }
    if (TValue = IDENT(t(x), 'identifier') v(x))                   { return; }
    if (DIFFER(v(x))) {
        if (t(x) ? (POS(0) ANY(&UCASE &LCASE) (SPAN(&UCASE &LCASE '0123456789' '_') | epsilon) RPOS(0))) {
            TValue = '(' t(x) ' ' v(x) ')';
            return;
        }
    }
    TValue = t(x);
    i = 0;
    while (i = LT(i, n(x)) i + 1) {
        TValue = TValue (DIFFER(TValue) '.', '') v(c(x)[i]);
    }
    return;
}
/* ==================================================================================================================== */
function TLump(x, len, i, t, sub) {
    if (~GT(len, 0)) { freturn; }
    if (TLump = IDENT(x) '()') { return; }
    if (~(t(x) ? (POS(0) ':'))) { goto TLump_normal; }
    if (~DIFFER(n(x))) {
        if (DIFFER(v(x))) {
            TLump = t(x) ' ' v(x);
        } else {
            TLump = t(x);
        }
        if (LE(SIZE(TLump), len)) { return; }
        freturn;
    }
    if (IDENT(n(x), 1)) {
        TLump = t(x) ' ';
        sub = TLump(c(x)[1], len - SIZE(TLump));
        if (~DIFFER(sub)) { freturn; }
        TLump = TLump sub;
        if (LE(SIZE(TLump), len)) { return; }
        freturn;
    }
    TLump = t(x) ' (';
    i = 0;
    while (i = LT(i, n(x)) i + 1) {
        sub = TLump(c(x)[i], len - SIZE(TLump) - 2);
        if (~DIFFER(sub)) { freturn; }
        TLump = TLump (GT(i, 1) ' ', '') sub;
    }
    TLump = TLump ')';
    if (LE(SIZE(TLump), len)) { return; }
    freturn;
TLump_normal:
    if (DIFFER(n(x))) { goto TLump0; }
    TLump = TValue(x);
    if (IDENT(TLump, t(x))) { goto TLump0; }
    if (LE(SIZE(TLump), len)) { return; }
    freturn;
TLump0:
    TLump = '(';
    if (t(x) ? (POS(0) ANY(&UCASE &LCASE) (SPAN('0123456789' &UCASE '_' &LCASE) | '') RPOS(0))) {
        t = t(x);
    } else {
        t = '"' t(x) '"';
    }
    TLump = TLump t;
    if (DIFFER(v(x))) {
        if (IDENT(t(x), 'TT_FLIT')) {
            fval = '' v(x);
            fval SPAN('0123456789') . pre;
            if (DIFFER(pre) IDENT(SIZE(pre) + 1, SIZE(fval))) fval = pre;
            TLump = TLump ' ' fval;
        } else {
            TLump = TLump ' ' v(x);
        }
    }
    i = 0;
    while (i = LT(i, n(x)) i + 1) {
        if (~(TLump = TLump ' ' TLump(c(x)[i], len - SIZE(TLump) - 2))) { freturn; }
    }
    TLump = TLump ')';
    return;
}
/* ==================================================================================================================== */
function TDump(x, outNm, i, t) {
    outNm = IDENT(outNm) .OUTPUT;
    x = IDENT(DATATYPE(x), 'NAME') $x;
    if (Gen(TLump(x, 140 - GetLevel()) nl, outNm)) return;
    if (DIFFER(n(x))) {
        if (t(x) ? (POS(0) ':')) {
            if (IDENT(n(x), 1)) {
                Gen(t(x) nl, outNm);
                IncLevel();
                TDump(c(x)[1], outNm);
                DecLevel();
                return;
            }
            Gen(t(x) ' (' nl, outNm);
            IncLevel();
            i = 0;
            while (i = LT(i, n(x)) i + 1)
                TDump(c(x)[i], outNm);
            DecLevel();
            Gen(')' nl, outNm);
            return;
        }
        if (~(t(x) ? (POS(0) ANY(&UCASE &LCASE)
                     (SPAN(&UCASE &LCASE '0123456789' '_') | epsilon) RPOS(0))))
            t = '"' t(x) '"';
        else
            t = t(x);
        if (DIFFER(v(x))) {
            Gen('(' t ' ' v(x) nl, outNm);
        } else {
            Gen('(' t nl, outNm);
        }
        IncLevel();
        i = 0;
        while (i = LT(i, n(x)) i + 1)
            TDump(c(x)[i], outNm);
        DecLevel();
        Gen(')' nl, outNm);
        return;
    }
    Gen(TValue(x) nl, outNm);
    return;
}
/* ==================================================================================================================== */
function TreeDumpValue(x, t, v, fval, zeros, pre) {
    t = t(x); v = v(x);
    if (t ? (POS(0) ('TT_QLIT' | 'TT_CSET') RPOS(0))) { TreeDumpValue = ' "' CQize(v) '"'; return; }
    if (~DIFFER(v)) { TreeDumpValue = ; return; }
    if (IDENT(t, 'TT_FLIT')) {
        fval = '' CONVERT(v, 'REAL');
        if (fval ? (POS(0) (SPAN('0123456789+-') . pre) '.' RPOS(0))) { fval = pre; }
        TreeDumpValue = ' ' fval;
        return;
    }
    TreeDumpValue = ' ' v;
    return;
}
/* ==================================================================================================================== */
function TreeDumpSkip(x) {
    if (~IDENT(t(x), 'TT_ATTR')) freturn;
    if (v(x) ? (POS(0) (':line' | ':lline' | ':file' | ':stno' | ':src') RPOS(0))) return;
    freturn;
}
/* ==================================================================================================================== */
function TreeDumpAt(x, level, outNm, i, line, kids) {
    x = IDENT(DATATYPE(x), 'NAME') $x;
    line = DUPL(' ', 2 * level) '(' t(x) TreeDumpValue(x);
    kids = 0;
    i = 0;
    while (i = LT(i, n(x)) i + 1) kids = (TreeDumpSkip(c(x)[i]) kids, kids + 1);
    if (~GT(kids, 0)) { TreeDumpPut(line ')', outNm); return; }
    TreeDumpPut(line, outNm);
    i = 0;
    while (i = LT(i, n(x)) i + 1) {
        if (~TreeDumpSkip(c(x)[i])) TreeDumpAt(c(x)[i], level + 1, outNm);
    }
    TreeDumpPut(DUPL(' ', 2 * level) ')', outNm);
    return;
}
/* ==================================================================================================================== */
/* PARSER_TREE_HASH=1 (Lon 2026-09-29 18:5x CDT: "hashing the output tree is what I meant. In memory. Then output the one number   */
/* per-test for comparison."): each line TreeDumpAt would print, and its newline, is folded into TreeHashH instead, byte by byte, */
/* h = (h * 256 + byte) mod (2^55 - 55) -- the same fold, over the same bytes, as out/parser_<lang> (src/tools/parser_main.c).   */
/* TreeDumpEnd prints the one number for the file and resets it; unset, TreeDumpPut prints the line and TreeDumpEnd does nothing. */
function TreeDumpPut(s, outNm, i, n) {
    if (~IDENT(TreeHashOn, '1')) { $outNm = s; return; }
    n = SIZE(s);
    i = 0;
    while (i = LT(i, n) i + 1) TreeHashH = REMDR(TreeHashH * 256 + TreeHashOrd[SUBSTR(s, i, 1)], TreeHashP);
    TreeHashH = REMDR(TreeHashH * 256 + 10, TreeHashP);
    return;
}
/* ==================================================================================================================== */
function TreeDumpEnd() {
    if (~IDENT(TreeHashOn, '1')) return;
    OUTPUT = TreeHashH;
    TreeHashH = 0;
    return;
}
/* ==================================================================================================================== */
function TreeDump(x, outNm) {
    outNm = IDENT(outNm) .OUTPUT;
    TreeDumpAt(x, 0, outNm);
    return;
}
TreeHashOn  = HOST(4, 'PARSER_TREE_HASH');
TreeHashP   = 36028797018963913;
TreeHashH   = 0;
TreeHashOrd = TABLE(257);
TreeHashI   = 0;
while (LT(TreeHashI, 256)) { TreeHashOrd[SUBSTR(&ALPHABET, TreeHashI + 1, 1)] = TreeHashI; TreeHashI = TreeHashI + 1; }
