/* parser_raku.sc -- Raku as Snocone patterns, cut from Rakudo 2026.05's src/Raku/Grammar.nqp (the RakuAST front end; the
   default front end src/Perl6/Grammar.nqp has the same comp_unit, statementlist, statement, statement_control, termish and
   infixish structure): the scope/routine/package declarators, termish, and the 33-level operator precedence table (letters
   b= .. y=, RakuAST::OperatorProperties) as a precedence climb, tightest level first.  One tree, built in order by Shift and
   Reduce at the point of recognition, in the TT vocabulary the C parser dumps (src/parsers/raku/rk_tree.c), so lower and the
   tree gate stand.
   Lon 2026-09-30: "A Compiland grammar with proper precedence, and a tree built directly in-order, using simple shift reduce."
   Runtime chain, as every parser_*.sc: global case assign match counter stack tree ShiftReduce tdump gen qize semantic omega trace. */
&FULLSCAN = 1;
E_Parse = "'Parse'";
wordchars = &UCASE &LCASE '_' digits X1xxxxxxx;
alpha     = ANY(&UCASE &LCASE '_' X1xxxxxxx);
/* ==================================================================================================================== */
/* lookahead predicates, immediate: they read the subject through Src (an interpolated block swaps Src for its own text) */
/* ==================================================================================================================== */
function NoWordAt(q, c)    { c = SUBSTR(Src, q + 1, 1); if (IDENT(c)) { return; } if (c ? ANY(wordchars)) { freturn; } return; }
function NoneAt(q, set, c) { c = SUBSTR(Src, q + 1, 1); if (IDENT(c)) { return; } if (set ? c) { freturn; } return; }
function IsAt(q, set, c)   { c = SUBSTR(Src, q + 1, 1); if (IDENT(c)) { freturn; } if (set ? c) { return; } freturn; }
function AtBol(q, c)       { if (EQ(q, 0)) { return; } c = SUBSTR(Src, q, 1); if (IDENT(c, nl)) { return; } freturn; }
function Kw(w)             { Kw = w @kw_q *NoWordAt(kw_q); return; }
function Strip(s, u)       { while (s ? '_' = '') { ; } Strip = s; return; }
/* ==================================================================================================================== */
/* token ws: blanks, comments (# and #`( )), pod (=begin/=end, =finish, =word ... blank line), unspace                    */
/* ==================================================================================================================== */
pod_end  = BREAK(nl) nl ARBNO(NOTANY(nl) BREAK(nl) nl | nl) '=end' (BREAK(nl) nl | REM);
pod      = @pod_q *AtBol(pod_q) FENCE(SPAN(' ' tab) | epsilon) '=' ( 'finish' REM
                                                                 | 'begin' *pod_end
                                                                 | *alpha (BREAK(nl) | REM) FENCE(ARBNO(nl NOTANY(nl) BREAK(nl)) | epsilon) );
white    = ( SPAN(' ' tab nl cr) | '#`(' BREAK(')') ')' | '#`[' BREAK(']') ']' | '#' (BREAK(nl) | REM) | *pod | bSlash SPAN(' ' tab nl cr) );
White    = *white FENCE(*White | epsilon);
Gray     = FENCE(*White | epsilon);
$' '     = Gray;
$'  '    = White;
/* ==================================================================================================================== */
/* names and variables: identifier, name (::), sigil twigil desigilname; a name reduces to TT_VAR, a field to TT_TWIGIL_FIELD */
/* ==================================================================================================================== */
ident    = *alpha FENCE(SPAN(wordchars) | epsilon) FENCE(*ident_more | epsilon);
ident_more = ('-' | "'") *alpha FENCE(SPAN(wordchars) | epsilon) FENCE(*ident_more | epsilon);
name     = *ident FENCE(*name_more | epsilon);
name_more = '::' *ident FENCE(*name_more | epsilon);
sigil    = ANY('$@%&');
twigil   = ANY('.!*?^:');
function VarNode(s, sg, tw, nm) {
    s ? (POS(0) *sigil . sg FENCE(*twigil . tw | epsilon) REM . nm);
    if (IDENT(sg, '$') (IDENT(tw, '.'), IDENT(tw, '!'))) { Sh('TT_TWIGIL_FIELD', nm); VarNode = .dummy; nreturn; }
    if (IDENT(sg, '$')) { Sh('TT_VAR', tw nm); VarNode = .dummy; nreturn; }
    if (IDENT(sg, '&')) { Sh('TT_VAR', nm); VarNode = .dummy; nreturn; }
    Sh('TT_VAR', sg tw nm);
    VarNode = .dummy; nreturn;
}
var_tok  = ( bSlash *name . sl_nm . *Sh('TT_VAR', sl_nm)
           | ('$' '<' BREAK('>') . cap_nm '>') . *Sh('TT_QLIT', cap_nm) . *Reduce('TT_NAMED_CAPTURE', 1)
           | ('$' SPAN(digits) . cap_ix @cap_q *NoWordAt(cap_q)) . *Sh('TT_ILIT', cap_ix) . *Reduce('TT_CAPTURE', 1)
           | (*sigil FENCE(*twigil | epsilon) (*name | '_' | '/' | '!')) $ dv_tx . var_tx . *VarNode(var_tx)
           );
/* ==================================================================================================================== */
/* values: numbers and strings (token value:sym<number>, quote:sym<apos>, quote:sym<dblq>, quote:sym<< < > >>)          */
/* ==================================================================================================================== */
hexval = TABLE(); hx_i = 0;
while (LE(hx_i, 15)) { hexval[SUBSTR('0123456789abcdef', hx_i + 1, 1)] = hx_i; hexval[SUBSTR('0123456789ABCDEF', hx_i + 1, 1)] = hx_i; hx_i = hx_i + 1; }
function IntVal(s, v, r, i, d) {
    v = Strip(s);
    if (v ? (POS(0) '0' ANY('xXbBoO') . r REM . s)) {
        r = (IDENT(lwr(r), 'x') 16, IDENT(lwr(r), 'b') 2, 8);
        i = 0; v = 0;
        while (LE(i = i + 1, SIZE(s))) { d = hexval[SUBSTR(s, i, 1)]; v = v * r + d; }
        IntVal = '' v; return;
    }
    IntVal = v; return;
}
number   = ( ('0' ANY('xX') SPAN(hex_digits '_') | '0' ANY('bB') SPAN('01_') | '0' ANY('oO') SPAN('01234567_')) . num_tx . *Sh('TT_ILIT', IntVal(num_tx))
           | (SPAN(digits '_') '.' SPAN(digits) FENCE(ANY('eE') FENCE(ANY('+-') | epsilon) SPAN(digits) | epsilon)
             | SPAN(digits '_') ANY('eE') FENCE(ANY('+-') | epsilon) SPAN(digits)) . num_tx . *Sh('TT_FLIT', Strip(num_tx))
           | SPAN(digits '_') . num_tx . *Sh('TT_ILIT', IntVal(num_tx))
           );
esc_tbl = TABLE();
esc_tbl['n'] = nl; esc_tbl['t'] = tab; esc_tbl['r'] = cr; esc_tbl['0'] = nul; esc_tbl['a'] = CHAR(7); esc_tbl['b'] = bs; esc_tbl['e'] = CHAR(27); esc_tbl['f'] = ff;
esc_tbl[bSlash] = bSlash; esc_tbl['"'] = '"'; esc_tbl['$'] = '$'; esc_tbl['@'] = '@'; esc_tbl['%'] = '%'; esc_tbl['&'] = '&'; esc_tbl['{'] = '{'; esc_tbl['}'] = '}'; esc_tbl["'"] = "'";
function Utf8(cp) {
    if (LT(cp, 128))   { Utf8 = CHAR(cp); return; }
    if (LT(cp, 2048))  { Utf8 = CHAR(192 + cp / 64) CHAR(128 + REMDR(cp, 64)); return; }
    if (LT(cp, 65536)) { Utf8 = CHAR(224 + cp / 4096) CHAR(128 + REMDR(cp / 64, 64)) CHAR(128 + REMDR(cp, 64)); return; }
    Utf8 = CHAR(240 + cp / 262144) CHAR(128 + REMDR(cp / 4096, 64)) CHAR(128 + REMDR(cp / 64, 64)) CHAR(128 + REMDR(cp, 64)); return;
}
function HexCp(h, v, i) { v = 0; i = 0; while (LE(i = i + 1, SIZE(h))) { v = v * 16 + hexval[SUBSTR(h, i, 1)]; } HexCp = v; return; }
function HexList(s, out, h) { out = ''; while (s ? (POS(0) SPAN(hex_digits) . h FENCE(',' | epsilon) REM . s)) { out = out Utf8(HexCp(h)); } HexList = out; return; }
function DecodeSq(s, out, c) {
    out = '';
    while (s ? (POS(0) ( bSlash ANY(bSlash "'") . c | LEN(1) . c ) REM . s)) { out = out c; }
    DecodeSq = out; return;
}
/* a double-quoted body: literal runs and escapes are shifted as TT_QLIT, $var @var[] %var{} and { code } as their trees, each  */
/* piece after the first joined by TT_CAT as it is recognised (the C parser's left-nested CAT chain)                           */
function DqFlush() {
    if (~EQ(dq_lit, 1)) { return; }
    Sh('TT_QLIT', dq_buf); dq_buf = ''; dq_lit = 0;
    if (EQ(dq_n, 0)) { dq_n = 1; return; }
    Reduce('TT_CAT', 2); return;
}
function DqPiece(kind, v) {
    if (IDENT(kind, 'TT_QLIT')) { dq_buf = dq_buf v; dq_lit = 1; return; }
    DqFlush();
    if (EQ(dq_n, 0)) { dq_n = 1; return; }
    Reduce('TT_CAT', 2); return;
}
function DqCode(code, save) { save = Src; Src = code; if (~(code ? (POS(0) *$' ' *item *$' ' RPOS(0)))) { Src = save; freturn; } Src = save; return; }
function Interp(s, c, h, code, save_n, save_l, save_b) {
    save_n = dq_n; save_l = dq_lit; save_b = dq_buf; dq_n = 0; dq_lit = 0; dq_buf = '';
    while (GT(SIZE(s), 0)) {
        if (s ? (POS(0) bSlash 'x[' BREAK(']') . h ']' REM . s))            { DqPiece('TT_QLIT', HexList(h)); continue; }
        if (s ? (POS(0) bSlash 'x' SPAN(hex_digits) . h REM . s))           { DqPiece('TT_QLIT', Utf8(HexCp(h))); continue; }
        if (s ? (POS(0) bSlash LEN(1) . c REM . s))                          { DqPiece('TT_QLIT', (DIFFER(esc_tbl[c]) esc_tbl[c], c)); continue; }
        if (s ? (POS(0) '{' BREAK('}') . code '}' REM . s))                  { DqFlush(); if (DqCode(code)) { DqPiece('TT_X'); } else { DqPiece('TT_QLIT', '{' code '}'); } continue; }
        if (s ? (POS(0) '$' ANY(wordchars '.!^*_<') ))                       { DqFlush(); if (s ? (POS(0) (*var_tok *dq_post) REM . s)) { DqPiece('TT_X'); continue; } }
        if (s ? (POS(0) ANY('@%') *alpha))                                   { if (s ? (POS(0) (*var_tok *dq_sub))) { DqFlush(); if (s ? (POS(0) (*var_tok *dq_sub) REM . s)) { DqPiece('TT_X'); continue; } } }
        if (s ? (POS(0) SPAN(' ' tab nl cr) . c REM . s))                    { DqPiece('TT_QLIT', c); continue; }
        if (s ? (POS(0) BREAK(bSlash '$@%{' ' ' tab nl cr) . c REM . s))      { if (DIFFER(c)) { DqPiece('TT_QLIT', c); continue; } }
        if (s ? (POS(0) LEN(1) . c REM . s))                                 { DqPiece('TT_QLIT', c); continue; }
    }
    DqFlush();
    if (EQ(dq_n, 0)) { Sh('TT_QLIT', ''); }
    dq_n = save_n; dq_lit = save_l; dq_buf = save_b;
    Interp = .dummy; nreturn;
}
dq_sub   = ( '[' *$' ' *item *$' ' ']' . *Reduce('TT_ARR_GET', 2) *dq_post
           | '<' BREAK('>') . hk_tx '>' . *Sh('TT_QLIT', hk_tx) . *Reduce('TT_HASH_GET', 2) *dq_post
           | '{' *$' ' *item *$' ' '}' . *Reduce('TT_HASH_GET', 2) *dq_post );
dq_post  = FENCE( *dq_sub
                | '.' *ident . mth_tx @mq *NoneAt(mq, '(') . *Sh('TT_QLIT', mth_tx) . *Reduce('TT_METHCALL', 2) *dq_post
                | epsilon );
dq_body  = ARBNO( NOTANY('"' bSlash) | bSlash LEN(1) );
sq_body  = ARBNO( NOTANY("'" bSlash) | bSlash LEN(1) );
word     = (NOTANY(' ' tab nl '>') FENCE(BREAK(' ' tab nl '>') | REM)) . w_tx . *Sh('TT_QLIT', w_tx) . *IncCounter();
words    = FENCE(SPAN(' ' tab nl) | epsilon) FENCE(*word FENCE(SPAN(' ' tab nl) | epsilon) *words | epsilon);
string   = ( "'" *sq_body . str_tx "'"                                 . *Sh('TT_QLIT', DecodeSq(str_tx))
           | '"' *dq_body . str_tx '"'                                 . *Interp(str_tx)
           | 'qq' '[' BREAK(']') . str_tx ']'                          . *Interp(str_tx)
           | 'q' FENCE(':w' | epsilon) '[' BREAK(']') . str_tx ']'    . *Sh('TT_QLIT', str_tx)
           | 'Q' '[' BREAK(']') . str_tx ']'                           . *Sh('TT_QLIT', str_tx)
           );
qwords   = '<' @qw_q *NoneAt(qw_q, '=<-') . *PushCounter() . *Sh('TT_VAR', '__rk_arr') *words '>' . *Reduce('TT_FNC', nTop() + 1, '__rk_arr') . *PopCounter();
function RxTrans(s, out, c, k) {
    out = '';
    while (GT(SIZE(s), 0)) {
        if (s ? (POS(0) '<-[' BREAK(']') . k ']>' REM . s)) { while (k ? '..' = '-') { ; } out = out '[^' k ']'; continue; }
        if (s ? (POS(0) '<[' BREAK(']') . k ']>' REM . s))  { while (k ? '..' = '-') { ; } out = out '[' k ']'; continue; }
        if (s ? (POS(0) '$<' BREAK('>') . k '>=' REM . s))  { out = out '<' k '>'; continue; }
        if (s ? (POS(0) "'" BREAK("'") . k "'" REM . s))   { out = out k; continue; }
        if (s ? (POS(0) SPAN(' ' tab nl) REM . s))          { continue; }
        if (s ? (POS(0) bSlash LEN(1) . c REM . s))         { out = out bSlash c; continue; }
        if (s ? (POS(0) LEN(1) . c REM . s))                { out = out c; continue; }
    }
    RxTrans = out; return;
}
function RxNode(tx) { Sh('TT_QLIT', RxTrans(tx)); rx_node[Top()] = 1; RxNode = .dummy; nreturn; }
rx_node  = TABLE();
regex    = ( '/' ARBNO(NOTANY('/' bSlash) | bSlash LEN(1)) . rx_tx '/'   . *RxNode(rx_tx)
           | ('rx' | 'm') FENCE(':i' | epsilon) '/' ARBNO(NOTANY('/' bSlash) | bSlash LEN(1)) . rx_tx '/' . *RxNode(rx_tx)
           );
/* ==================================================================================================================== */
/* node builders that read the stack: the shapes the C parser prints for calls, methods, assignments and comparisons      */
/* ==================================================================================================================== */
function mk(t, v)      { mk = tree(t, v, 0); return; }
function Sh(t, v, s)   { s = tree(t, v, 0); Push(s); Sh = .dummy; nreturn; }
function SetCount(k)   { value($'#N') = k; SetCount = .dummy; nreturn; }
call_kind = TABLE();
call_kind['say'] = 'TT_SAY'; call_kind['print'] = 'TT_PRINT'; call_kind['die'] = 'TT_DIE'; call_kind['return'] = 'TT_RETURN'; call_kind['take'] = 'TT_SUSPEND';
call_kind['reverse'] = 'TT_REVERSE'; call_kind['sort'] = 'TT_SORT'; call_kind['map'] = 'TT_MAP'; call_kind['grep'] = 'TT_GREP'; call_kind['next'] = 'TT_LOOP_NEXT'; call_kind['last'] = 'TT_LOOP_BREAK';
listop_tbl = TABLE();
listop_tbl['say'] = 1; listop_tbl['print'] = 1; listop_tbl['die'] = 1; listop_tbl['return'] = 1; listop_tbl['take'] = 1; listop_tbl['next'] = 1; listop_tbl['last'] = 1; listop_tbl['exit'] = 1; listop_tbl['redo'] = 1;
opword_tbl = TABLE();
opword_tbl['x'] = 1; opword_tbl['xx'] = 1; opword_tbl['eq'] = 1; opword_tbl['ne'] = 1; opword_tbl['lt'] = 1; opword_tbl['gt'] = 1; opword_tbl['le'] = 1; opword_tbl['ge'] = 1; opword_tbl['div'] = 1;
opword_tbl['mod'] = 1; opword_tbl['and'] = 1; opword_tbl['or'] = 1; opword_tbl['not'] = 1; opword_tbl['so'] = 1; opword_tbl['cmp'] = 1; opword_tbl['leg'] = 1; opword_tbl['eqv'] = 1; opword_tbl['gcd'] = 1;
opword_tbl['lcm'] = 1; opword_tbl['min'] = 1; opword_tbl['max'] = 1; opword_tbl['is'] = 1; opword_tbl['does'] = 1; opword_tbl['xor'] = 1; opword_tbl['andthen'] = 1; opword_tbl['orelse'] = 1;
opword_tbl['await'] = 1; opword_tbl['start'] = 1; opword_tbl['X'] = 1; opword_tbl['Z'] = 1; opword_tbl['with'] = 1; opword_tbl['without'] = 1; opword_tbl['if'] = 1; opword_tbl['unless'] = 1; opword_tbl['while'] = 1; opword_tbl['until'] = 1; opword_tbl['for'] = 1; opword_tbl['given'] = 1; opword_tbl['when'] = 1; opword_tbl['else'] = 1; opword_tbl['elsif'] = 1;
function PairAhead(q, c) { q = q + 1; while (IDENT(SUBSTR(Src, q, 1), ' ')) { q = q + 1; } if (IDENT(SUBSTR(Src, q, 2), '=>')) { return; } freturn; }
function NoInfixAhead(q, c1, c2) { c1 = SUBSTR(Src, q + 1, 1); c2 = SUBSTR(Src, q + 2, 1); if (('+-*/~<>=!?&|^%.' ? c1) (IDENT(c2, ' '), IDENT(c2, tab))) { freturn; } if (IDENT(SUBSTR(Src, q + 1, 2), '..')) { freturn; } return; }
function NotOpWord(w) { if (DIFFER(opword_tbl[w])) { freturn; } return; }
function EndAhead(q, c) { q = q + 1; while ((IDENT(SUBSTR(Src, q, 1), ' '), IDENT(SUBSTR(Src, q, 1), tab))) { q = q + 1; } c = SUBSTR(Src, q, 1); if (IDENT(c)) { return; } if (';}),' ? c) { return; } if (IDENT(c, nl)) { return; } freturn; }
function IsListop(w)  { if (DIFFER(listop_tbl[w])) { return; } freturn; }
function Call(nm, n, k, i, c) {
    nm = (DIFFER(rename_tbl[nm]) rename_tbl[nm], nm);
    if (IDENT(nm, 'fail')) { i = n; while (GT(i, 0)) { Pop(); i = i - 1; } Reduce('TT_RETURN', 0); Call = .dummy; nreturn; }
    k = call_kind[nm];
    if (DIFFER(k) EQ(n, 1)) { c = ARRAY('1:1'); c[1] = Top(); if (DIFFER(paren_list[c[1]])) { c[1] = Pop(); i = 1; while (LT(i, n(c[1]))) { i = i + 1; Push(c(c[1])[i]); } n = n(c[1]) - 1; } }
    if (DIFFER(k)) {
        if ((IDENT(k, 'TT_MAP'), IDENT(k, 'TT_GREP')) GT(n, 0)) { c = ARRAY('1:' n); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; }
            if (IDENT(t(c[1]), 'TT_ANON_BLOCK') EQ(n(c(c[1])[1]), 1)) { c[1] = c(c(c[1])[1])[1]; } i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } }
        Reduce(k, n); Call = .dummy; nreturn; }
    c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; }
    if (GT(nFlat(n, c), n)) { Push(mk('TT_VAR', '__rk_named_call')); Push(mk('TT_QLIT', nm)); Push(mk('TT_ILIT', 2 * n - nFlat(n, c)));
        i = 0; while (LT(i, n)) { i = i + 1; if (~(IDENT(t(c[i]), 'TT_FNC') IDENT(v(c[i]), '__rk_pair'))) { Push(c[i]); } }
        i = 0; while (LT(i, n)) { i = i + 1; if (IDENT(t(c[i]), 'TT_FNC') IDENT(v(c[i]), '__rk_pair')) { Push(c(c[i])[2]); Push(c(c[i])[3]); } }
        Reduce('TT_FNC', nFlat(n, c) + 3, '__rk_named_call'); Call = .dummy; nreturn; }
    Push(mk('TT_VAR', nm)); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); }
    Reduce('TT_FNC', n + 1, nm); Call = .dummy; nreturn;
}
bare_nm = TABLE();
function Bare(nm) { if ((IDENT(nm, 'True'), IDENT(nm, 'False'))) { Push(mk('TT_VAR', '__rk_mkbool')); Push(mk('TT_ILIT', (IDENT(nm, 'True') 1, 0))); Reduce('TT_FNC', 2, '__rk_mkbool'); Bare = .dummy; nreturn; } Sh('TT_VAR', nm); bare_nm[Top()] = 1; Bare = .dummy; nreturn; }
rename_tbl = TABLE();
rename_tbl['plan'] = '__rk_test_plan'; rename_tbl['ok'] = '__rk_test_ok'; rename_tbl['is-approx'] = '__rk_test_is_approx'; rename_tbl['exit'] = '__rk_exit'; rename_tbl['flat'] = '__rk_arr';
function nFlat(n, c, i, k, m) { m = 0; i = 0; while (LT(i, n)) { i = i + 1; k = c[i]; m = m + (IDENT(t(k), 'TT_FNC') IDENT(v(k), '__rk_pair') 2, 1); } nFlat = m; return; }
function PushFlat(n, c, i, k) { i = 0; while (LT(i, n)) { i = i + 1; k = c[i]; if (IDENT(t(k), 'TT_FNC') IDENT(v(k), '__rk_pair')) { Push(c(k)[2]); Push(c(k)[3]); } else { Push(k); } } return; }
function MethCall(n, k, i, c, inv, nm) {
    c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; }
    nm = Pop(); inv = Pop();
    if (IDENT(v(nm), 'bless')) { Push(inv); Push(nm); PushFlat(n, c); Reduce('TT_METHCALL', nFlat(n, c) + 2); MethCall = .dummy; nreturn; }
    if (IDENT(v(nm), 'new') IDENT(t(inv), 'TT_VAR') DIFFER(bare_nm[inv])) { Push(mk('TT_QLIT', v(inv))); PushFlat(n, c); Reduce('TT_NEW', nFlat(n, c) + 1); MethCall = .dummy; nreturn; }
    Push(inv); Push(nm); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); }
    Reduce('TT_METHCALL', n + 2); MethCall = .dummy; nreturn;
}
function Assign(r, l) {
    r = Pop(); l = Pop();
    if (IDENT(t(l), 'TT_ARR_GET'))  { Push(c(l)[1]); Push(c(l)[2]); Push(r); Reduce('TT_ARR_SET', 3); Assign = .dummy; nreturn; }
    if (IDENT(t(l), 'TT_HASH_GET')) { Push(c(l)[1]); Push(c(l)[2]); Push(r); Reduce('TT_HASH_SET', 3); Assign = .dummy; nreturn; }
    if (IDENT(t(l), 'TT_METHCALL') EQ(n(l), 2)) { Push(c(l)[1]); Reduce('TT_FIELD', 1, v(c(l)[2])); Push(r); Reduce('TT_ASSIGN', 2); Assign = .dummy; nreturn; }
    Push(l); ListRhs(r, l); Reduce('TT_ASSIGN', 2); Assign = .dummy; nreturn;
}
function DotAssign(n, i, c, nm, l) { c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } nm = Pop(); l = Pop(); Push(l); Push(l); Push(nm); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_METHCALL', n + 2); Reduce('TT_ASSIGN', 2); DotAssign = .dummy; nreturn; }
function AssignOp(k, r, l) { r = Pop(); l = Pop(); Push(l); Push(l); Push(r); if (IDENT(k, '__rk_dor')) { Call2('__rk_dor'); } else { Reduce(k, 2); } Reduce('TT_ASSIGN', 2); AssignOp = .dummy; nreturn; }
function IncDec(k, l)      { l = Pop(); Push(l); Push(l); Push(mk('TT_ILIT', 1)); Reduce(k, 2); Reduce('TT_ASSIGN', 2); IncDec = .dummy; nreturn; }
function Call2(nm, r, l)   { r = Pop(); l = Pop(); Push(mk('TT_VAR', nm)); Push(l); Push(r); Reduce('TT_FNC', 3, nm); Call2 = .dummy; nreturn; }
cmp_kind = TABLE();
cmp_kind['TT_EQ'] = 1; cmp_kind['TT_NE'] = 1; cmp_kind['TT_LT'] = 1; cmp_kind['TT_GT'] = 1; cmp_kind['TT_LE'] = 1; cmp_kind['TT_GE'] = 1;
function ChainCmp(k, r, l, last) {
    r = Pop(); l = Pop();
    if (DIFFER(cmp_kind[t(l)])) { last = c(l)[2]; Push(l); Push(last); Push(r); Reduce(k, 2); Reduce('TT_SEQ', 2); ChainCmp = .dummy; nreturn; }
    if (IDENT(t(l), 'TT_SEQ') DIFFER(cmp_kind[t(c(l)[n(l)])])) { last = c(c(l)[n(l)])[2]; Push(l); Push(last); Push(r); Reduce(k, 2); Reduce('TT_SEQ', 2); ChainCmp = .dummy; nreturn; }
    Push(l); Push(r); Reduce(k, 2); ChainCmp = .dummy; nreturn;
}
function Junct(fl, r, l, i) {
    r = Pop(); l = Pop();
    Push(mk('TT_VAR', fl));
    if (IDENT(t(l), 'TT_FNC') IDENT(v(l), fl)) { i = 1; while (LT(i, n(l))) { i = i + 1; Push(c(l)[i]); } Push(r); Reduce('TT_FNC', n(l) + 1, fl); }
    else { Push(l); Push(r); Reduce('TT_FNC', 3, fl); }
    Junct = .dummy; nreturn;
}
caret_to = TABLE();
function CaretRange(r) { r = Pop(); Push(mk('TT_ILIT', 0)); if (IDENT(t(r), 'TT_ILIT')) { Push(mk('TT_ILIT', v(r) - 1)); } else { Push(r); Push(mk('TT_ILIT', 1)); Reduce('TT_SUB', 2); } Reduce('TT_TO', 2); caret_to[Top()] = 1; CaretRange = .dummy; nreturn; }
function RangeEx(r, l) {
    r = Pop(); l = Pop(); Push(l);
    if (IDENT(t(r), 'TT_ILIT')) { Push(mk('TT_ILIT', v(r) - 1)); Reduce('TT_TO', 2); RangeEx = .dummy; nreturn; }
    if (IDENT(t(r), 'TT_VAR') (v(r) ? (POS(0) '@'))) { Push(r); Push(mk('TT_QLIT', 'elems')); Reduce('TT_METHCALL', 2); Push(mk('TT_ILIT', 1)); Reduce('TT_SUB', 2); Reduce('TT_TO', 2); RangeEx = .dummy; nreturn; }
    Push(r); Push(mk('TT_ILIT', 1)); Reduce('TT_SUB', 2); Reduce('TT_TO', 2); RangeEx = .dummy; nreturn;
}
function MkBool(b) { Push(mk('TT_VAR', '__rk_mkbool')); Push(mk('TT_ILIT', b)); Reduce('TT_FNC', 2, '__rk_mkbool'); MkBool = .dummy; nreturn; }
function Pair(r, l)        { r = Pop(); l = Pop(); if (IDENT(t(l), 'TT_VAR') DIFFER(bare_nm[l])) { l = mk('TT_QLIT', v(l)); } Push(mk('TT_VAR', '__rk_pair')); Push(l); Push(r); Reduce('TT_FNC', 3, '__rk_pair'); Pair = .dummy; nreturn; }
/* a parenthesised or bracketed list of n items: one item is itself, pairs make __rk_hash with keys and values flattened, else __rk_arr */
function ListNode(n, force, lit, c, i) {
    if (EQ(n, 1) IDENT(force)) { ListNode = .dummy; nreturn; }
    if (IDENT(lit, 1)) { c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } Push(mk('TT_VAR', '__rk_arr_lit')); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_FNC', n + 1, '__rk_arr_lit'); ListNode = .dummy; nreturn; }
    c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; }
    if (GT(n, 0) IDENT(t(c[1]), 'TT_FNC') IDENT(v(c[1]), '__rk_pair')) { Push(mk('TT_VAR', '__rk_hash')); PushFlat(n, c); Reduce('TT_FNC', nFlat(n, c) + 1, '__rk_hash'); ListNode = .dummy; nreturn; }
    Push(mk('TT_VAR', '__rk_arr')); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_FNC', n + 1, '__rk_arr'); ListNode = .dummy; nreturn;
}
paren_list = TABLE();
function MarkParen(x) { x = Top(); if (IDENT(t(x), 'TT_FNC') IDENT(v(x), '__rk_arr')) { paren_list[x] = 1; } MarkParen = .dummy; nreturn; }
listctor = TABLE();
listctor['__rk_arr'] = 1; listctor['__rk_arr_lit'] = 1; listctor['__rk_hash'] = 1; listctor['__rk_arr_xx'] = 1; listctor['__rk_range_arr'] = 1; listctor['__rk_arr_slice'] = 1; listctor['__rk_undef'] = 1; listctor['__rk_arr_pick'] = 1;
function ListRhs(r, l) {
    if (IDENT(t(r), 'TT_XREP')) { Push(mk('TT_VAR', '__rk_rep')); Push(c(r)[1]); Push(c(r)[2]); Reduce('TT_FNC', 3, '__rk_rep'); return; }
    if (~(IDENT(t(l), 'TT_VAR') (v(l) ? (POS(0) ANY('@%'))))) { Push(r); return; }
    if (IDENT(t(r), 'TT_TO')) { Push(mk('TT_VAR', '__rk_range_arr')); Push(c(r)[1]); Push(c(r)[2]); Reduce('TT_FNC', 3, '__rk_range_arr'); return; }
    if (IDENT(t(r), 'TT_FNC') IDENT(v(r), '__rk_pair')) { Push(r); ListNode(1, 1); return; }
    if ((IDENT(t(r), 'TT_REVERSE'), IDENT(t(r), 'TT_SORT'), IDENT(t(r), 'TT_MAP'), IDENT(t(r), 'TT_GREP'), IDENT(t(r), 'TT_METHCALL'), (IDENT(t(r), 'TT_VAR') (v(r) ? (POS(0) ANY('@%')))))) { Push(r); return; }
    if (IDENT(t(r), 'TT_FNC') DIFFER(listctor[v(r)])) { Push(r); return; }
    Push(r); ListNode(1, 1); return;
}
function DeclInit(r, l) { r = Pop(); l = Pop(); Push(l); ListRhs(r, l); Reduce('TT_DECL', 3); DeclInit = .dummy; nreturn; }
function ArrInit(r, l) { r = Pop(); l = Pop(); Push(l); ListRhs(r, l); Reduce('TT_ASSIGN', 2); ArrInit = .dummy; nreturn; }
function IsListVar() { if (dv_tx ? (POS(0) ANY('@%'))) { return; } freturn; }
stmt_kind = TABLE();
stmt_kind['TT_RETURN'] = 1; stmt_kind['TT_SAY'] = 1; stmt_kind['TT_PRINT'] = 1; stmt_kind['TT_IF'] = 1; stmt_kind['TT_UNLESS'] = 1; stmt_kind['TT_WHILE'] = 1; stmt_kind['TT_UNTIL'] = 1;
stmt_kind['TT_REPEAT'] = 1; stmt_kind['TT_CLOOP'] = 1; stmt_kind['TT_EVERY'] = 1; stmt_kind['TT_FOR_RANGE'] = 1; stmt_kind['TT_CASE'] = 1; stmt_kind['TT_LOOP_BREAK'] = 1; stmt_kind['TT_LOOP_NEXT'] = 1;
stmt_kind['TT_SUB_DECL'] = 1; stmt_kind['TT_CLASS_DECL'] = 1; stmt_kind['TT_DIE'] = 1; stmt_kind['TT_TRY'] = 1; stmt_kind['TT_CATCH'] = 1; stmt_kind['TT_YADA'] = 1; stmt_kind['TT_SEQ'] = 1;
stmt_kind['TT_GATHER'] = 1; stmt_kind['TT_ROLE_DECL'] = 1; stmt_kind['TT_GRAMMAR_DECL'] = 1; stmt_kind['TT_SEQ_EXPR'] = 1; stmt_kind['TT_STMT'] = 1; stmt_kind['TT_PROGRAM'] = 1;
/* the value of a routine body is its last statement: at the closing brace the top node, if it is an expression, is the return */
function SetMulti(f) { is_multi = f; SetMulti = .dummy; nreturn; }
function RoutineEnd(nb, np, x, c, i, nm) {
    nb = TopCounter();
    if (GT(nb, 0)) { x = Top(); if (IDENT(stmt_kind[t(x)])) { Reduce('TT_RETURN', 1); } }
    PopCounter(); np = PopVal();
    if (EQ(is_multi, 1)) { c = ARRAY('1:' (GT(np + nb, 0) np + nb, 1)); i = np + nb; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } nm = Pop(); v(nm) = v(nm) '$' np; i = 0; while (LT(i, np)) { i = i + 1; v(nm) = v(nm) '$' (DIFFER(ptype[c[i]]) ptype[c[i]], 'Any'); } Push(nm); i = 0; while (LT(i, np + nb)) { i = i + 1; Push(c[i]); } }
    Reduce('TT_SUB_DECL', np + nb + 1); RoutineEnd = .dummy; nreturn;
}
function ForStmt(body, var, lst) {
    body = Pop(); var = Pop(); lst = Pop();
    if (IDENT(t(lst), 'TT_TO') IDENT(caret_to[lst]) DIFFER(v(var))) { Push(var); Push(c(lst)[1]); Push(c(lst)[2]); Push(body); Push(mk('TT_ILIT', 0)); Reduce('TT_FOR_RANGE', 5); ForStmt = .dummy; nreturn; }
    Push(lst); Reduce('TT_ITERATE', 1, v(var)); Push(body); Reduce('TT_EVERY', 2); ForStmt = .dummy; nreturn;
}
function SetArms(k) { has_arms = k; SetArms = .dummy; nreturn; }
function CatchEnd(n, i, c, topic) { if (EQ(has_arms, 1)) { Given(n); Reduce('TT_SEQ_EXPR', 1); CatchEnd = .dummy; nreturn; } c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } topic = Pop(); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_SEQ_EXPR', n); CatchEnd = .dummy; nreturn; }
function ForStmtN(np, body, i, k, c, lst, a, ix) {
    if (EQ(np, 1)) { ForStmt(); ForStmtN = .dummy; nreturn; }
    body = Pop(); c = ARRAY('1:' (GT(np, 0) np, 1)); i = np; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } lst = Pop();
    a = '__fm_a_' fm_n; ix = '__fm_i_' fm_n; fm_n = fm_n + 1;
    Push(mk('TT_VAR', a)); Push(lst); Reduce('TT_ASSIGN', 2);
    Push(mk('TT_VAR', ix)); Push(mk('TT_ILIT', 0)); Reduce('TT_ASSIGN', 2);
    Push(mk('TT_VAR', ix)); Push(mk('TT_VAR', 'elems')); Push(mk('TT_VAR', a)); Reduce('TT_FNC', 2, 'elems'); Reduce('TT_LT', 2);
    Push(mk('TT_VAR', ix)); Push(mk('TT_VAR', ix)); Push(mk('TT_ILIT', np)); Reduce('TT_ADD', 2); Reduce('TT_ASSIGN', 2);
    i = 0; while (LT(i, np)) { i = i + 1; Push(c[i]); Push(mk('TT_VAR', '__rk_arr_at')); Push(mk('TT_VAR', a)); if (EQ(i, 1)) { Push(mk('TT_VAR', ix)); } else { Push(mk('TT_VAR', ix)); Push(mk('TT_ILIT', i - 1)); Reduce('TT_ADD', 2); } Reduce('TT_FNC', 3, '__rk_arr_at'); Reduce('TT_ASSIGN', 2); }
    Push(body); Reduce('TT_SEQ', np + 1); Reduce('TT_CLOOP', 4); Reduce('TT_SEQ', 2); ForStmtN = .dummy; nreturn;
}
function DoFor(body, lst, e) { body = Pop(); lst = Pop(); e = (IDENT(t(body), 'TT_SEQ_EXPR') EQ(n(body), 1) c(body)[1], body); Push(e); Push(lst); Reduce('TT_MAP', 2); DoFor = .dummy; nreturn; }
function Given(n, topic, i, c) { c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } topic = Pop(); Push(topic); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_CASE', n + 1); Given = .dummy; nreturn; }
function ReduceBlock(np, i, c, body) { body = Pop(); c = ARRAY('1:' (GT(np, 0) np, 1)); i = np; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } Push(body); i = 0; while (LT(i, np)) { i = i + 1; Push(c[i]); } Reduce('TT_ANON_BLOCK', np + 1); ReduceBlock = .dummy; nreturn; }
function ArrGet(ix, a, i) { ix = Pop(); a = Pop(); if (IDENT(t(ix), 'TT_FNC') IDENT(v(ix), '__rk_arr')) { Push(mk('TT_VAR', '__rk_arr_pick')); Push(a); i = 1; while (LT(i, n(ix))) { i = i + 1; Push(c(ix)[i]); } Reduce('TT_FNC', n(ix) + 1, '__rk_arr_pick'); ArrGet = .dummy; nreturn; } if (IDENT(t(ix), 'TT_TO')) { Push(mk('TT_VAR', '__rk_arr_slice')); Push(a); Push(c(ix)[1]); Push(c(ix)[2]); Reduce('TT_FNC', 4, '__rk_arr_slice'); ArrGet = .dummy; nreturn; } Push(a); Push(ix); Reduce('TT_ARR_GET', 2); ArrGet = .dummy; nreturn; }
function ZenSlice(a) { a = Pop(); Push(mk('TT_VAR', '__rk_arr_slice')); Push(a); Push(mk('TT_ILIT', 0)); Push(a); Push(mk('TT_QLIT', 'elems')); Reduce('TT_METHCALL', 2); Push(mk('TT_ILIT', 1)); Reduce('TT_SUB', 2); Reduce('TT_FNC', 4, '__rk_arr_slice'); ZenSlice = .dummy; nreturn; }
function WhateverIdx(r, a) { r = Pop(); a = Top(); Push(a); Push(mk('TT_QLIT', 'elems')); Reduce('TT_METHCALL', 2); Push(r); Reduce('TT_SUB', 2); WhateverIdx = .dummy; nreturn; }
function Adverb(k, g) { g = Pop(); Push(c(g)[1]); Push(c(g)[2]); Reduce(k, 2); Adverb = .dummy; nreturn; }
function Smatch(r, l) { r = Pop(); l = Pop(); Push(l); Push(r); if (IDENT(t(r), 'TT_QLIT') DIFFER(rx_node[r])) { Push(mk('TT_QLIT', 'match')); Reduce('TT_SMATCH', 3); Smatch = .dummy; nreturn; } Reduce('TT_SMATCH', 2); Smatch = .dummy; nreturn; }
function Mod(k, cnd, x)   { cnd = Pop(); x = Pop(); Push(cnd); Push(x); Reduce('TT_SEQ_EXPR', 1); Reduce(k, 2); Mod = .dummy; nreturn; }
function ModFor(lst, x)   { lst = Pop(); x = Pop(); Push(lst); Reduce('TT_ITERATE', 1, '_'); Push(x); Reduce('TT_SEQ_EXPR', 1); Reduce('TT_EVERY', 2); ModFor = .dummy; nreturn; }
function ModWith(k, cnd, x) { cnd = Pop(); x = Pop(); Push(mk('TT_VAR', '_')); Push(cnd); Reduce('TT_ASSIGN', 2); Push(mk('TT_VAR', '__rk_defined')); Push(mk('TT_VAR', '_')); Reduce('TT_FNC', 2, '__rk_defined'); Push(x); Reduce('TT_SEQ_EXPR', 1); Reduce(k, 2); Reduce('TT_SEQ_EXPR', 2); ModWith = .dummy; nreturn; }
function ModGiven(tp, x)  { tp = Pop(); x = Pop(); Push(mk('TT_VAR', '_')); Push(tp); Reduce('TT_ASSIGN', 2); Push(x); Reduce('TT_SEQ_EXPR', 2); ModGiven = .dummy; nreturn; }
function LoopForever(b)   { b = Pop(); Push(mk('TT_ILIT', 1)); Push(b); Reduce('TT_WHILE', 2); LoopForever = .dummy; nreturn; }
function ForSwap(v, b)    { v = Pop(); b = Pop(); Push(v); Push(b); ForSwap = .dummy; nreturn; }
function UndefInit(l)     { l = Top(); if (v(l) ? (POS(0) ANY('@%'))) { Push(mk('TT_VAR', '__rk_undef')); Reduce('TT_FNC', 1, '__rk_undef'); Reduce('TT_ASSIGN', 2); UndefInit = .dummy; nreturn; } Push(mk('TT_NUL', )); Reduce('TT_ASSIGN', 2); UndefInit = .dummy; nreturn; }
function HasNode(sg, nm)  { if (IDENT(sg, '@')) { Sh('TT_ARR_DECL', nm); HasNode = .dummy; nreturn; } if (IDENT(sg, '%')) { Sh('TT_HASH_DECL', nm); HasNode = .dummy; nreturn; } Sh('TT_VAR', nm); HasNode = .dummy; nreturn; }
function SetRed(s)  { red_nm = s; SetRed = .dummy; nreturn; }
function SetPk(k)   { pk_kind = k; pk_val = ''; SetPk = .dummy; nreturn; }
function PkTrait(w, nm) { pk_val = pk_val (DIFFER(pk_val) CHAR(1), '') (IDENT(w, 'does') 'd', 'i') nm; PkTrait = .dummy; nreturn; }
function SetForce() { lst_force = 1; SetForce = .dummy; nreturn; }
function ClearForce() { lst_force = ; ClearForce = .dummy; nreturn; }
/* ==================================================================================================================== */
/* operator tokens, longest spelling first, each ruling out the spellings it prefixes; word operators need word ends      */
/* ==================================================================================================================== */
op_pow   = *$' ' '**' @op_q *NoneAt(op_q, '=') *$' ';
op_mul   = *$' ' '*' @op_q *NoneAt(op_q, '*=') *$' ';        op_div = *$' ' '/' @op_q *NoneAt(op_q, '/=') *$' ';
op_mod   = *$' ' '%' @op_q *NoneAt(op_q, '%=') *$' ';        op_divis = *$' ' '%%' @op_q *NoneAt(op_q, '=') *$' ';
op_add   = *$' ' '+' @op_q *NoneAt(op_q, '+=&|^') *$' ';     op_sub = *$' ' '-' @op_q *NoneAt(op_q, '->=') *$' ';
op_cat   = *$' ' '~' @op_q *NoneAt(op_q, '~=') *$' ';
op_jand  = *$' ' '&' @op_q *NoneAt(op_q, '&=') *$' ';        op_jor = *$' ' '|' @op_q *NoneAt(op_q, '|=') *$' ';    op_jxor = *$' ' '^' @op_q *NoneAt(op_q, '^=.') *$' ';
op_eq    = *$' ' '==' @op_q *NoneAt(op_q, '=') *$' ';        op_ne  = *$' ' '!=' @op_q *NoneAt(op_q, '=') *$' ';
op_lt    = *$' ' '<' @op_q *NoneAt(op_q, '=<') *$' ';        op_gt  = *$' ' '>' @op_q *NoneAt(op_q, '=>') *$' ';
op_le    = *$' ' '<=' @op_q *NoneAt(op_q, '>') *$' ';        op_ge  = *$' ' '>=' *$' ';
op_id    = *$' ' '===' *$' ';                                op_cmp3 = *$' ' '<=>' *$' ';
op_smatch = *$' ' '~~' *$' ';                                op_nsmatch = *$' ' '!~~' *$' ';
op_tand  = *$' ' '&&' *$' ';                                 op_tor = *$' ' '||' *$' ';    op_dor = *$' ' '//' @op_q *NoneAt(op_q, '=') *$' ';   op_txor = *$' ' '^^' *$' ';
op_assign = *$' ' '=' @op_q *NoneAt(op_q, '=>:~') *$' ';
op_bind  = *$' ' ':=' *$' ';                                 op_pair = *$' ' '=>' *$' ';
op_range = *$' ' '..' @op_q *NoneAt(op_q, '.^') *$' ';       op_rangex = *$' ' '..^' *$' ';
op_tern1 = *$' ' '??' *$' ';                                 op_tern2 = *$' ' '!!' *$' ';
op_xrep  = *$'  ' 'x' @op_q *NoWordAt(op_q) *$'  ';          op_xx  = *$'  ' 'xx' @op_q *NoWordAt(op_q) *$'  ';
op_leq   = *$'  ' 'eq' @op_q *NoWordAt(op_q) *$'  ';         op_lne = *$'  ' 'ne' @op_q *NoWordAt(op_q) *$'  ';
op_llt   = *$'  ' 'lt' @op_q *NoWordAt(op_q) *$'  ';         op_lgt = *$'  ' 'gt' @op_q *NoWordAt(op_q) *$'  ';
op_lle   = *$'  ' 'le' @op_q *NoWordAt(op_q) *$'  ';         op_lge = *$'  ' 'ge' @op_q *NoWordAt(op_q) *$'  ';
op_eqv   = *$'  ' 'eqv' @op_q *NoWordAt(op_q) *$'  ';        op_cmp = *$'  ' 'cmp' @op_q *NoWordAt(op_q) *$'  ';   op_leg = *$'  ' 'leg' @op_q *NoWordAt(op_q) *$'  ';
op_min   = *$'  ' 'min' @op_q *NoWordAt(op_q) *$'  ';        op_max = *$'  ' 'max' @op_q *NoWordAt(op_q) *$'  ';
op_land  = *$'  ' 'and' @op_q *NoWordAt(op_q) *$'  ';        op_lor = *$'  ' 'or' @op_q *NoWordAt(op_q) *$'  ';    op_lxor = *$'  ' 'xor' @op_q *NoWordAt(op_q) *$'  ';
op_div_i = *$'  ' 'div' @op_q *NoWordAt(op_q) *$'  ';        op_mod_w = *$'  ' 'mod' @op_q *NoWordAt(op_q) *$'  ';
op_gcd   = *$'  ' 'gcd' @op_q *NoWordAt(op_q) *$'  ';        op_lcm = *$'  ' 'lcm' @op_q *NoWordAt(op_q) *$'  ';
kw_if = Kw('if');  kw_unless = Kw('unless');  kw_while = Kw('while');  kw_until = Kw('until');  kw_for = Kw('for');  kw_loop = Kw('loop');  kw_repeat = Kw('repeat');
kw_given = Kw('given');  kw_when = Kw('when');  kw_default = Kw('default');  kw_else = Kw('else');  kw_elsif = Kw('elsif');  kw_CATCH = Kw('CATCH');  kw_use = Kw('use');
kw_my = Kw('my');  kw_our = Kw('our');  kw_state = Kw('state');  kw_has = Kw('has');  kw_constant = Kw('constant');  kw_multi = Kw('multi');  kw_proto = Kw('proto');
kw_sub = Kw('sub');  kw_method = Kw('method');  kw_submethod = Kw('submethod');  kw_class = Kw('class');  kw_role = Kw('role');  kw_grammar = Kw('grammar');  kw_module = Kw('module');
kw_token = Kw('token');  kw_rule = Kw('rule');  kw_regex = Kw('regex');  kw_is = Kw('is');  kw_does = Kw('does');  kw_handles = Kw('handles');  kw_returns = Kw('returns');
kw_gather = Kw('gather');  kw_try = Kw('try');  kw_do = Kw('do');  kw_not = Kw('not');  kw_so = Kw('so');  kw_self = Kw('self');  kw_with = Kw('with');  kw_without = Kw('without');  kw_await = Kw('await');  kw_start = Kw('start');  kw_X = *$'  ' 'X' @kw_q *NoWordAt(kw_q) *$'  ';  kw_Z = *$'  ' 'Z' @kw_q *NoWordAt(kw_q) *$'  ';
/* ==================================================================================================================== */
/* term (token termish, term:sym<...>): variables, values, calls, names, circumfixes, blocks, regexes, reductions           */
/* ==================================================================================================================== */
list_items = ( *item . *IncCounter() FENCE(*$' ' ',' *$' ' FENCE(*list_items | epsilon . *SetForce()) | epsilon) );
args_p   = ( '(' *$' ' FENCE(*list_items | epsilon) *$' ' ')' );
call_args = ( epsilon . *PushCounter() *args_p . *Call(PopVal(), nTop()) . *PopCounter() );
listop_args = ( epsilon . *PushCounter() *$'  ' @lo_q *NoneAt(lo_q, ';}),=|&') *NoInfixAhead(lo_q) *list_items . *Call(PopVal(), nTop()) . *PopCounter() );
listop_none = ( epsilon . *Call(PopVal(), 0) );
block_term = ( *$' ' '->' . *PushCounter() FENCE(*$' ' *params | epsilon) *$' ' *block . *ReduceBlock(nTop()) . *PopCounter()
             | *$' ' *block . *Reduce('TT_ANON_BLOCK', 1) );
paren    = ( '(' *$' ' . *PushCounter() . *ClearForce() FENCE(*list_items | epsilon) *$' ' FENCE(',' *$' ' . *SetForce() | epsilon) ')' . *ListNode(nTop(), lst_force) . *MarkParen() . *PopCounter() );
bracket  = ( '[' *$' ' . *PushCounter() FENCE(*list_items | epsilon) *$' ' FENCE(',' *$' ' | epsilon) ']' . *ListNode(nTop(), 1, 1) . *PopCounter() );
reduce_op = ( '[' ( ('+' . *SetRed('add')) | ('-' . *SetRed('sub')) | ('*' . *SetRed('mul')) | ('~' . *SetRed('cat')) | ('min' . *SetRed('min')) | ('max' . *SetRed('max')) ) ']' *$' '
              . *Sh('TT_VAR', '__rk_reduce_' red_nm) *cat_expr . *Reduce('TT_FNC', 2, '__rk_reduce_' red_nm) );
key_term = ( *kw_do *$'  ' *kw_for *$' ' . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() *$' ' *block . *DoFor()
           | *kw_await *$'  ' *item
           | *kw_start *$' ' *block
           | *kw_sub *$' ' . *PushCounter() FENCE('(' *$' ' FENCE(*params | epsilon) *$' ' ')' *$' ' | epsilon) *block . *ReduceBlock(nTop()) . *PopCounter()
           | *kw_gather *$' ' *block . *Reduce('TT_GATHER', 1)
           | *kw_try *$' ' *block . *Reduce('TT_TRY', 1)
           | *kw_do *$' ' *block
           | *kw_not *$' ' *item . *Reduce('TT_NOT', 1)
           | *kw_so *$' ' . *PushVal('so') . *PushCounter() *item . *IncCounter() . *Call(PopVal(), nTop()) . *PopCounter()
           | *kw_self . *Sh('TT_VAR', 'self')
           );
term     = ( ':' *ident . cp_nm . *Sh('TT_QLIT', cp_nm) ( '(' *$' ' *item *$' ' ')' | '<' BREAK('>') . cp_v '>' . *Sh('TT_QLIT', cp_v) | epsilon . *MkBool(1) ) . *Pair()
           | ':!' *ident . cp_nm . *Sh('TT_QLIT', cp_nm) . *MkBool(0) . *Pair()
           | '^' *$' ' *pow_expr . *CaretRange()
           | '.' (FENCE('^' | epsilon) *ident) . mth_tx . *Sh('TT_VAR', '_') . *Sh('TT_QLIT', mth_tx) . *PushCounter() FENCE(*args_p | epsilon) . *MethCall(nTop()) . *PopCounter()
           | *var_tok
           | *number
           | *string
           | *qwords
           | *reduce_op
           | *regex
           | *paren
           | *bracket
           | *block_term
           | *key_term
           | *ident $ cl_i . cl_nm @pk_q *PairAhead(pk_q) . *Bare(cl_nm)
           | *ident $ cl_i . cl_nm *NotOpWord(cl_i) @cl_q *IsAt(cl_q, '(') . *PushVal(cl_nm) *call_args
           | *ident $ cl_i . cl_nm *NotOpWord(cl_i) *IsListop(cl_i) @cl_q *EndAhead(cl_q) . *PushVal(cl_nm) *listop_none
           | *ident $ cl_i . cl_nm *NotOpWord(cl_i) @cl_q *IsAt(cl_q, ' ' tab) . *PushVal(cl_nm) *listop_args
           | *ident $ cl_i . cl_nm *NotOpWord(cl_i) *IsListop(cl_i) . *PushVal(cl_nm) *listop_none
           | *name $ cl_i . cl_nm *NotOpWord(cl_i) . *Bare(cl_nm)
           );
/* postfixes: method call, subscripts, call parens, ++ --, :exists :delete -- adjacent to the term (Grammar.nqp: no ws before them) */
postfix  = FENCE( '.' (FENCE('^' | epsilon) *ident) . mth_tx . *Sh('TT_QLIT', mth_tx) . *PushCounter() FENCE(*args_p | ':' *$' ' *list_items | epsilon) . *MethCall(nTop()) . *PopCounter() *postfix
                | '[' *$' ' '*' *$' ' ']' . *ZenSlice() *postfix
                | '[' *$' ' ( '*' *$' ' '-' *$' ' *item . *WhateverIdx() | epsilon . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() ) *$' ' ']' . *ArrGet() *postfix
                | '<' BREAK('>') . hk_tx '>' . *Sh('TT_QLIT', hk_tx) . *Reduce('TT_HASH_GET', 2) *postfix
                | '{' *$' ' *item *$' ' '}' . *Reduce('TT_HASH_GET', 2) *postfix
                | '(' *$' ' . *PushCounter() FENCE(*list_items | epsilon) *$' ' ')' . *Reduce('TT_INVOKE', nTop() + 1) . *PopCounter() *postfix
                | '++' . *IncDec('TT_ADD') *postfix
                | '--' . *IncDec('TT_SUB') *postfix
                | ':exists' . *Adverb('TT_HASH_EXISTS') *postfix
                | ':delete' . *Adverb('TT_HASH_DELETE') *postfix
                | epsilon );
postfix_expr = *term *postfix;
/* ==================================================================================================================== */
/* the precedence climb, tightest first: ** | symbolic unary | * / % | + - | x xx | ~ | & | | ^ | .. | cmp | chaining | && | || // | ?? !! | = op= | and | or */
/* ==================================================================================================================== */
pow_expr   = *postfix_expr FENCE( *op_pow *unary_expr . *Reduce('TT_POW', 2) | epsilon );
unary_expr = ( *$' ' '-' @un_q *NoneAt(un_q, '-=>') *$' ' *unary_expr . *Reduce('TT_MNS', 1)
             | *$' ' '+' @un_q *NoneAt(un_q, '+=') *$' ' *unary_expr
             | *$' ' '!' @un_q *NoneAt(un_q, '!=~') *$' ' *unary_expr . *Reduce('TT_NOT', 1)
             | *$' ' '?' @un_q *NoneAt(un_q, '?') *$' ' . *PushVal('so') . *PushCounter() *unary_expr . *IncCounter() . *Call(PopVal(), nTop()) . *PopCounter()
             | *$' ' '++' *unary_expr . *IncDec('TT_ADD')
             | *$' ' '--' *unary_expr . *IncDec('TT_SUB')
             | *pow_expr );
mul_expr = *unary_expr *mul_tail;
mul_tail = FENCE( ( *op_divis *unary_expr . *Reduce('TT_DIVIS', 2)
                  | *op_mul   *unary_expr . *Reduce('TT_MUL', 2)
                  | *op_div   *unary_expr . *Reduce('TT_DIV', 2)
                  | *op_mod   *unary_expr . *Reduce('TT_MOD', 2)
                  | *op_div_i *unary_expr . *Call2('__rk_intdiv')
                  | *op_mod_w *unary_expr . *Call2('__rk_mod')
                  | *op_gcd   *unary_expr . *Call2('__rk_gcd')
                  | *op_lcm   *unary_expr . *Call2('__rk_lcm') ) *mul_tail | epsilon );
add_expr = *mul_expr *add_tail;
add_tail = FENCE( ( *op_add *mul_expr . *Reduce('TT_ADD', 2) | *op_sub *mul_expr . *Reduce('TT_SUB', 2) ) *add_tail | epsilon );
rep_expr = *add_expr *rep_tail;
rep_tail = FENCE( ( *op_xx *add_expr . *Call2('__rk_arr_xx') | *op_xrep *add_expr . *Reduce('TT_XREP', 2) ) *rep_tail | epsilon );
cat_expr = *rep_expr *cat_tail;
cat_tail = FENCE( *op_cat *rep_expr . *Reduce('TT_CAT', 2) *cat_tail | epsilon );
jand_expr = *cat_expr *jand_tail;
jand_tail = FENCE( *op_jand *cat_expr . *Junct('all') *jand_tail | epsilon );
jor_expr = *jand_expr *jor_tail;
jor_tail = FENCE( ( *op_jor *jand_expr . *Junct('any') | *op_jxor *jand_expr . *Junct('one') ) *jor_tail | epsilon );
op_seq   = *$' ' '...' *$' ';
rng_expr = *jor_expr FENCE( *op_seq *jor_expr . *Reduce('TT_TO', 2) | *op_rangex *jor_expr . *RangeEx() | *op_range *jor_expr . *Reduce('TT_TO', 2) | epsilon );
str_expr = *rng_expr FENCE( *op_cmp3 *rng_expr . *Call2('__rk_cmp3') | *op_cmp *rng_expr . *Call2('__rk_cmpg') | *op_leg *rng_expr . *Call2('__rk_leg') | epsilon );
chn_expr = *str_expr *chn_tail;
chn_tail = FENCE( ( *op_id  *str_expr . *Call2('__rk_ident')  | *op_eqv *str_expr . *Call2('__rk_eqv')
                  | *op_nsmatch *str_expr . *Call2('__rk_not_smartmatch')
                  | *op_smatch  *str_expr . *Smatch()
                  | *op_eq *str_expr . *ChainCmp('TT_EQ') | *op_ne *str_expr . *ChainCmp('TT_NE')
                  | *op_le *str_expr . *ChainCmp('TT_LE') | *op_ge *str_expr . *ChainCmp('TT_GE')
                  | *op_lt *str_expr . *ChainCmp('TT_LT') | *op_gt *str_expr . *ChainCmp('TT_GT')
                  | *op_leq *str_expr . *Reduce('TT_LEQ', 2) | *op_lne *str_expr . *Reduce('TT_LNE', 2)
                  | *op_llt *str_expr . *Reduce('TT_LLT', 2) | *op_lgt *str_expr . *Reduce('TT_LGT', 2)
                  | *op_lle *str_expr . *Reduce('TT_LLE', 2) | *op_lge *str_expr . *Reduce('TT_LGE', 2) ) *chn_tail | epsilon );
tand_expr = *chn_expr *tand_tail;
tand_tail = FENCE( *op_tand *chn_expr . *Reduce('TT_SEQ', 2) *tand_tail | epsilon );
tor_expr = *tand_expr *tor_tail;
tor_tail = FENCE( ( *op_tor *tand_expr . *Reduce('TT_ALT', 2) | *op_dor *tand_expr . *Call2('__rk_dor') | *op_txor *tand_expr . *Call2('__rk_xor')
                  | *op_min *tand_expr . *Call2('__rk_min') | *op_max *tand_expr . *Call2('__rk_max') ) *tor_tail | epsilon );
cond_expr = *tor_expr FENCE( *op_tern1 *cond_expr *op_tern2 *cond_expr . *Reduce('TT_TERNARY', 3) | epsilon );
lst_expr = *cond_expr FENCE( ( *kw_X *cond_expr . *ListOp2('__rk_cross') | *kw_Z *cond_expr . *ListOp2('__rk_zip') ) | epsilon );
function RangeArr(x) { if (IDENT(t(x), 'TT_TO')) { Push(mk('TT_VAR', '__rk_range_arr')); Push(c(x)[1]); Push(c(x)[2]); Reduce('TT_FNC', 3, '__rk_range_arr'); return; } Push(x); return; }
function ListOp2(nm, r, l) { r = Pop(); l = Pop(); Push(mk('TT_VAR', nm)); RangeArr(l); RangeArr(r); Reduce('TT_FNC', 3, nm); ListOp2 = .dummy; nreturn; }
item     = ( *lst_expr FENCE( *op_pair *item . *Pair()
                             | *op_assign *decl_init . *Assign()
                             | *$' ' '.=' *$' ' *ident . mth_tx . *Sh('TT_QLIT', mth_tx) . *PushCounter() FENCE(*args_p | epsilon) . *DotAssign(nTop()) . *PopCounter()
                             | *op_bind *item . *Reduce('TT_ASSIGN', 2)
                             | *$' ' '+=' *$' ' *item . *AssignOp('TT_ADD') | *$' ' '-=' *$' ' *item . *AssignOp('TT_SUB')
                             | *$' ' '*=' *$' ' *item . *AssignOp('TT_MUL') | *$' ' '/=' *$' ' *item . *AssignOp('TT_DIV')
                             | *$' ' '~=' *$' ' *item . *AssignOp('TT_CAT') | *$' ' '%=' *$' ' *item . *AssignOp('TT_MOD')
                             | *$' ' '//=' *$' ' *item . *AssignOp('__rk_dor')
                             | epsilon ) );
land_expr = *item *land_tail;
land_tail = FENCE( *op_land *item . *Reduce('TT_SEQ', 2) *land_tail | epsilon );
lor_expr = *land_expr *lor_tail;
lor_tail = FENCE( ( *op_lor *land_expr . *Reduce('TT_ALT', 2) | *op_lxor *land_expr . *Call2('__rk_xor') ) *lor_tail | epsilon );
expr     = *lor_expr;
/* ==================================================================================================================== */
/* blocks and statements (token block, rule statementlist, token statement, statement_mod_cond, statement_mod_loop)      */
/* ==================================================================================================================== */
block    = ( '{' *$' ' . *PushCounter() *stmts *$' ' '}' . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter() );
stmts    = ARBNO( FENCE(*$' ' *statement . *IncCounter()) );
stmt_end = *$' ' FENCE( ';' . *SetTerm(1) | @se_q *IsAt(se_q, '}') . *SetTerm(0) | RPOS(0) . *SetTerm(0) );
function SetTerm(k) { last_term = k; SetTerm = .dummy; nreturn; }
function EndSlot() { if (EQ(last_term, 1)) { Reduce('TT_SEQ_EXPR', 0); Reduce('TT_ATTR', 1, ':subj'); Reduce('TT_STMT', 1); IncCounter(); } EndSlot = .dummy; nreturn; }
blk_end  = *$' ' . *SetTerm(1);
stmt_mod = FENCE( *kw_if *$'  ' *expr . *Mod('TT_IF')
                | *kw_unless *$'  ' *expr . *Mod('TT_UNLESS')
                | *kw_while *$'  ' *expr . *Mod('TT_WHILE')
                | *kw_until *$'  ' *expr . *Mod('TT_UNTIL')
                | *kw_for *$'  ' *expr . *ModFor()
                | *kw_given *$'  ' *expr . *ModGiven()
                | *kw_with *$'  ' *expr . *ModWith('TT_IF')
                | *kw_without *$'  ' *expr . *ModWith('TT_UNLESS')
                | epsilon );
statement = ( *control
            | *declaration
            | *phaser
            | '{' *$' ' . *PushCounter() *stmts *$' ' '}' . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter() *blk_end
            | ('...' | '!!!' | '???') . *Reduce('TT_YADA', 0) *stmt_end
            | *expr *$' ' *stmt_mod *stmt_end
            | ';' . *Reduce('TT_SEQ_EXPR', 0) . *SetTerm(1)
            );
/* statement_control: if elsif else, unless, while until, repeat, loop, for, given when default, CATCH, use            */
cond     = ( *$' ' *expr *$' ' );
else_part = FENCE( *$' ' *kw_elsif *cond *block *else_part . *Reduce('TT_IF', nElse()) . *SetCount(1)
                 | *$' ' *kw_else *$' ' *block . *IncCounter()
                 | epsilon );
function nElse() { nElse = 2 + TopCounter(); return; }
control  = ( *kw_if *cond *block . *PushCounter() *else_part . *Reduce('TT_IF', nElse()) . *PopCounter() *blk_end
           | *kw_unless *cond *block . *Reduce('TT_UNLESS', 2) *blk_end
           | *kw_while *cond *block . *Reduce('TT_WHILE', 2) *blk_end
           | *kw_until *cond *block . *Reduce('TT_UNTIL', 2) *blk_end
           | *kw_repeat *$' ' *block *$' ' ( *kw_until *cond . *Reduce('TT_REPEAT', 2) | *kw_while *cond . *Reduce('TT_REPEAT', 2) ) *stmt_end
           | *kw_loop *$' ' '(' *$' ' *loop_part ';' *$' ' *loop_part ';' *$' ' *loop_part ')' *$' ' *block . *Reduce('TT_CLOOP', 4) *blk_end
           | *kw_loop *$' ' *block . *LoopForever() *blk_end
           | *kw_for *$' ' . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() *$' ' ( '->' *$' ' . *PushCounter() *params . *PushVal(nTop()) . *PopCounter() *$' ' *block . *ForStmtN(PopVal()) | *block . *Sh('TT_VAR', '') . *ForSwap() . *ForStmt() ) *blk_end
           | *kw_given *cond '{' *$' ' . *PushCounter() *when_arms *$' ' '}' . *Given(nTop()) . *PopCounter() *blk_end
           | *kw_CATCH *$' ' '{' *$' ' . *Sh('TT_VAR', '_') . *PushCounter() . *SetArms(0) *when_arms *$' ' '}' . *CatchEnd(nTop()) . *Reduce('TT_CATCH', 1) . *PopCounter() *blk_end
           | *kw_try *$' ' *block . *Reduce('TT_TRY', 1) *blk_end
           | *kw_gather *$' ' *block . *Reduce('TT_GATHER', 1) *blk_end
           | *kw_do *$' ' *block *blk_end
           | *kw_use *$'  ' ( 'v6' . *PushCounter() ARBNO('.' *ident . uv_tx . *UseChain(uv_tx)) . *Reduce('TT_USE_DECL', nTop(), 'v6') . *PopCounter() | *name . use_nm . *Reduce('TT_USE_DECL', 0, use_nm) ) *stmt_end
           );
function UseChain(s) { if (EQ(TopCounter(), 0)) { Push(mk('TT_VAR', '_')); IncCounter(); } Push(mk('TT_QLIT', s)); Reduce('TT_METHCALL', 2); UseChain = .dummy; nreturn; }
loop_part = FENCE( *scope_decl *$' ' | *expr *$' ' | epsilon . *Reduce('TT_NUL', 0) );
when_arms = ARBNO( FENCE( *$' ' *kw_when *cond *block . *IncCounter() . *IncCounter() . *SetArms(1)
                        | *$' ' *kw_default *$' ' . *Reduce('TT_NUL', 0) *block . *IncCounter() . *IncCounter() . *SetArms(1)
                        | *$' ' *statement . *IncCounter() ) );
/* phasers and statement prefixes keep their place in the statement list (the placement is the lowerer's)                */
phaser   = ( ( 'BEGIN' | 'END' | 'INIT' | 'CHECK' | 'FIRST' | 'LAST' | 'NEXT' | 'ENTER' | 'LEAVE' | 'KEEP' | 'UNDO' | 'PRE' | 'POST' | 'CONTROL' | 'QUIT' | 'once' | 'quietly' | 'start' | 'lazy' | 'eager' | 'sink' | 'react' | 'TEMP' ) @kw_q *NoWordAt(kw_q)
             *$' ' *block *blk_end );
/* declarations: scope_declarator (my our state has constant), routine_declarator (sub method), package_declarator, regex_declarator */
type_nm  = ( *name . ty_tx @ty_q *IsAt(ty_q, ' ' tab) *$'  ' @ty_v *IsAt(ty_v, '$@%&') );
decl_init = ( *IsListVar() . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() | *item );
scope_decl = ( ( *kw_my | *kw_our | *kw_state ) *$'  '
                ( *type_nm . *Sh('TT_VAR', ty_tx) *var_tok *$' ' ( (*op_assign | *op_bind) *decl_init . *DeclInit() | epsilon . *Reduce('TT_NUL', 0) . *Reduce('TT_DECL', 3) )
                | '(' *$' ' . *PushCounter() *var_tok . *IncCounter() ARBNO(*$' ' ',' *$' ' *var_tok . *IncCounter()) *$' ' ')' *$' ' . *PushVal(nTop()) . *PopCounter() *op_assign . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() . *Destr(PopVal())
                | *var_tok *$' ' ( (*op_assign | *op_bind) *decl_init . *ArrInit() | epsilon . *UndefInit() ) ) );
function Destr(n, r, i, c, tmp) { r = Pop(); c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } tmp = '__destr_' destr_n; destr_n = destr_n + 1; Push(mk('TT_VAR', tmp)); if (IDENT(t(r), 'TT_FNC') DIFFER(listctor[v(r)])) { Push(r); } else { Push(r); ListNode(1, 1); } Reduce('TT_ASSIGN', 2);
    i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); Push(mk('TT_VAR', '__rk_arr_at')); Push(mk('TT_VAR', tmp)); Push(mk('TT_ILIT', i - 1)); Reduce('TT_FNC', 3, '__rk_arr_at'); Reduce('TT_ASSIGN', 2); } Reduce('TT_SEQ_EXPR', n + 1); Destr = .dummy; nreturn; }
declaration = ( FENCE((*kw_my | *kw_our) *$'  ' | epsilon) *kw_constant *$'  ' ( *name . cn_tx . *Sh('TT_VAR', cn_tx) | *var_tok ) *$' ' *op_assign *item . *Reduce('TT_ASSIGN', 2) *stmt_end
              | *scope_decl *stmt_mod *stmt_end
              | *kw_has *$'  ' *has_decl *stmt_end
              | FENCE((*kw_my | *kw_our) *$'  ' | epsilon) FENCE(*kw_multi *$'  ' . *SetMulti(1) | *kw_proto *$'  ' . *SetMulti(0) | epsilon . *SetMulti(0)) ( *kw_sub | *kw_method | *kw_submethod ) *$'  ' *routine *blk_end
              | *kw_multi *$'  ' . *SetMulti(1) *routine *blk_end
              | ( *kw_class . *SetPk('TT_CLASS_DECL') | *kw_role . *SetPk('TT_ROLE_DECL') | *kw_grammar . *SetPk('TT_GRAMMAR_DECL') | *kw_module . *SetPk('TT_CLASS_DECL') ) *$'  ' *package *blk_end
              | ( *kw_token | *kw_rule | *kw_regex ) *$'  ' *name . rg_nm . *Sh('TT_VAR', rg_nm) *$' ' FENCE('(' BREAK(')') ')' *$' ' | epsilon) '{' BREAK('}') . rg_tx '}' . *Sh('TT_QLIT', rg_tx) . *Reduce('TT_REGEX_DECL', 2) *blk_end
              );
has_decl = ( FENCE(*type_nm | epsilon) *sigil . hs_sg *twigil . hs_tw *ident . hs_nm *$' '
             ( *kw_is *$'  ' 'rw' . *Sh('TT_RW_DECL', hs_tw hs_nm)
             | *kw_is *$'  ' 'required' . *Sh('TT_HAS_DECL', hs_tw hs_nm)
             | *kw_handles *$'  ' '<' BREAK('>') . hd_tx '>' . *Sh('TT_QLIT', hd_tx) . *Reduce('TT_HANDLES_DECL', 1, hs_tw hs_nm)
             | epsilon . *HasNode(hs_sg, hs_tw hs_nm) )
             FENCE( *$' ' *kw_is *$'  ' *ident | epsilon ) *$' ' FENCE( *op_assign *item . *HasDefault() | epsilon ) );
function HasDefault(d, h) { d = Pop(); h = Pop(); if (IDENT(t(h), 'TT_VAR')) { t(h) = 'TT_HAS_DECL'; } Append(h, d); Push(h); HasDefault = .dummy; nreturn; }
params   = ( *param . *IncCounter() FENCE(*$' ' ',' *$' ' *params | epsilon) );
param    = ( ( (*name FENCE(':' ANY('DU_') | epsilon)) . pty_tx *$'  ' | epsilon . *NoPty() ) ( ('**' | '*' | '+') . ppf_tx . *SetPfx(ppf_tx) | ':' . *SetPfx('') | epsilon . *SetPfx('') ) *var_tok . *ParamType(pty_tx, ppf_tx) FENCE(ANY('?!') | epsilon) *$' ' FENCE( *op_assign *cond_expr . *ParamDefault() | epsilon ) FENCE( *$' ' *kw_is *$'  ' *ident | epsilon ) );
function NoPty() { pty_tx = ''; NoPty = .dummy; nreturn; }
function SetPfx(s) { ppf_tx = s; SetPfx = .dummy; nreturn; }
function ParamType(ty, pf, v) { v = Pop(); if (DIFFER(pf)) { Append(v, mk('TT_QLIT', pf SUBSTR(var_tx, 1, 1))); ptype[v] = 'Slurpy'; } else if (DIFFER(ty)) { Append(v, mk('TT_QLIT', ty)); ptype[v] = ty; } Push(v); ParamType = .dummy; nreturn; }
ptype = TABLE();
pdef_v = ARRAY('1:64'); pdef_d = ARRAY('1:64'); pdef_n = 0;
function ParamDefault(d, v) { d = Pop(); v = Top(); pdef_n = pdef_n + 1; pdef_v[pdef_n] = v; pdef_d[pdef_n] = d; ParamDefault = .dummy; nreturn; }
function ResetDefaults() { pdef_n = 0; ResetDefaults = .dummy; nreturn; }
function PushDefaults(i) { i = 0; while (LT(i, pdef_n)) { i = i + 1; Push(pdef_v[i]); Push(mk('TT_QLIT', 'defined')); Reduce('TT_METHCALL', 2); Push(pdef_v[i]); Push(pdef_d[i]); Reduce('TT_ASSIGN', 2); Reduce('TT_SEQ_EXPR', 1); Reduce('TT_UNLESS', 2); IncCounter(); } pdef_n = 0; PushDefaults = .dummy; nreturn; }
signature = FENCE( '(' *$' ' FENCE(*params | epsilon) *$' ' FENCE('-->' *$' ' *name *$' ' | epsilon) ')' *$' ' | epsilon );
hex2 = TABLE(257); hx_i = 0;
while (LT(hx_i, 256)) { hex2[SUBSTR(&ALPHABET, hx_i + 1, 1)] = SUBSTR('0123456789abcdef', hx_i / 16 + 1, 1) SUBSTR('0123456789abcdef', REMDR(hx_i, 16) + 1, 1); hx_i = hx_i + 1; }
function OpName(kind, op, out, i) { out = 'R' kind '_'; i = 0; while (LT(i, SIZE(op))) { i = i + 1; out = out hex2[SUBSTR(op, i, 1)]; } rt_nm = out; OpName = .dummy; nreturn; }
op_routine_nm = ( ('infix' | 'prefix' | 'postfix' | 'circumfix') . rk_tx ':' ( '<' BREAK('>') . rop_tx '>' | '«' BREAK('»') . rop_tx '»' ) . *OpName(rk_tx, rop_tx) );
routine  = ( FENCE(*op_routine_nm | *name . rt_nm | epsilon . *NoName()) . *ResetDefaults() . *Sh('TT_VAR', rt_nm) *$' ' . *PushCounter() *signature . *PushVal(nTop()) . *PopCounter()
             FENCE(*kw_is *$'  ' *ident *$' ' | epsilon) FENCE(*kw_returns *$'  ' *name *$' ' | epsilon)
             '{' *$' ' . *PushCounter() . *PushDefaults() *stmts *$' ' '}' . *RoutineEnd() );
function NoName() { rt_nm = ''; NoName = .dummy; nreturn; }
package  = ( *name . pk_nm . *Sh('TT_VAR', pk_nm) *$' ' ARBNO( ( 'is' . pk_w | 'does' . pk_w ) *$'  ' *name . pk_tn . *PkTrait(pk_w, pk_tn) *$' ' )
             ( '{' *$' ' . *PushCounter() *members *$' ' '}' . *Reduce(pk_kind, nTop() + 1, pk_val) . *PopCounter() | ';' . *Reduce(pk_kind, 1, pk_val) ) );
members  = ARBNO( FENCE(*$' ' ( ';' | *statement . *IncCounter() )) );
/* ==================================================================================================================== */
/* comp_unit: every top-level statement is TT_STMT(TT_ATTR :subj X); the END slot closes the program as the C parser prints it */
/* ==================================================================================================================== */
top_stmt = ( *statement . *Reduce('TT_ATTR', 1, ':subj') . *Reduce('TT_STMT', 1) . *IncCounter() );
Compiland = epsilon . *PushCounter()
            POS(0) ARBNO( FENCE(*$' ' *top_stmt) FLUSH ) *$' ' RPOS(0)
            . *EndSlot()
            . *Reduce('Parse', nTop())
            . *PopCounter();
function ParseOne(ptree, i, n_kids) {
    if (GT(SIZE(Src), 0)) Src = SUBSTR(Src, 1, SIZE(Src) - 1);
    pf_a = TIME();
    InitCounter();
    InitStack();
    dq_n = 0; dq_lit = 0; dq_buf = ''; lst_force = ; last_term = 0; destr_n = 0; fm_n = 0; pdef_n = 0;
    if (Src ? *Compiland) {
        ptree = Pop();
        pf_parse = pf_parse + (TIME() - pf_a);
        if (DIFFER(ptree)) { i = 1; n_kids = n(ptree); while (LE(i, n_kids)) { TreeDump(c(ptree)[i]); i = i + 1; } }
        TreeDumpEnd();
    } else { pf_parse = pf_parse + (TIME() - pf_a); OUTPUT = 'Parse Error'; }
    return;
}
pf_list = HOST(4, 'PARSER_FILES');
if (IDENT(pf_list)) {
    INPUT(.INPUT, 9, '[-f0 -r16777215]');
    Src = INPUT;
    ParseOne();
} else {
    pf_n = 0; pf_bytes = 0; pf_parse = 0; pf_parse1 = 0;
    INPUT(.pf_names, 8, pf_list);
    while (pf_name = pf_names) {
        INPUT(.INPUT, 9, pf_name '[-r16777215]');
        Src = INPUT;
        ENDFILE(9);
        pf_bytes = pf_bytes + SIZE(Src);
        OUTPUT = '== ' pf_name;
        ParseOne();
        pf_n = pf_n + 1;
        if (EQ(pf_n, 1)) { pf_parse1 = pf_parse; }
    }
    TERMINAL = 'PARSER-METRICS files=' pf_n ' bytes=' pf_bytes ' parse_first_us=' pf_parse1 / 1000 ' parse_us=' pf_parse / 1000;
}
