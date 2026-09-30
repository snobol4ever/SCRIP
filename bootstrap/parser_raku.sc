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
white    = ( SPAN(' ' tab nl cr) | '#`(' BREAK(')') ')' | '#`[' BREAK(']') ']' | '#' (BREAK(nl) | REM) | *pod | bSlash SPAN(' ' tab nl cr)
           | @ns_q *AtBol(ns_q) ( 'no' | 'need' | 'require' | 'import' ) @kw_q *NoWordAt(kw_q) SPAN(' ' tab) *name FENCE(BREAK(';' nl) | epsilon) ';' . *SetTerm(1) );
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
fh_tbl = TABLE(); fh_tbl['STDOUT'] = 1; fh_tbl['OUT'] = 1; fh_tbl['STDERR'] = 2; fh_tbl['ERR'] = 2; fh_tbl['STDIN'] = 0; fh_tbl['IN'] = 0;
function VarNode(s, sg, tw, nm) {
    s ? (POS(0) *sigil . sg FENCE(*twigil . tw | epsilon) REM . nm);
    if (IDENT(sg, '$') IDENT(tw, '*') DIFFER(fh_tbl[nm])) { Push(mk('TT_ILIT', fh_tbl[nm])); Reduce('TT_FH_CAPTURE', 1); VarNode = .dummy; nreturn; }
    if (IDENT(sg, '$') (IDENT(tw, '.'), IDENT(tw, '!'))) { Sh('TT_TWIGIL_FIELD', nm); VarNode = .dummy; nreturn; }
    if (IDENT(sg, '$')) { Sh('TT_VAR', tw nm); VarNode = .dummy; nreturn; }
    if ((IDENT(tw, '.'), IDENT(tw, '!'))) { Sh('TT_TWIGIL_FIELD', nm); VarNode = .dummy; nreturn; }
    Sh('TT_VAR', sg tw nm);
    VarNode = .dummy; nreturn;
}
var_tok  = ( bSlash *name $ sl_now . sl_nm *NoteNow(sl_now) . *Sh('TT_VAR', sl_nm)
           | ('$' '[' BREAK(']') ']') . it_tx . *Sh('TT_VAR', SUBSTR(it_tx, 2))
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
esc_tbl['n'] = nl; esc_tbl['t'] = tab; esc_tbl['r'] = cr; esc_tbl['a'] = CHAR(7); esc_tbl['b'] = bs; esc_tbl['e'] = CHAR(27); esc_tbl['f'] = ff;
esc_tbl[bSlash] = bSlash; esc_tbl['"'] = '"'; esc_tbl['{'] = '{'; esc_tbl['}'] = '}'; esc_tbl["'"] = "'";
function Utf8(cp) {
    if (LT(cp, 128))   { Utf8 = CHAR(cp); return; }
    if (LT(cp, 2048))  { Utf8 = CHAR(192 + cp / 64) CHAR(128 + REMDR(cp, 64)); return; }
    if (LT(cp, 65536)) { Utf8 = CHAR(224 + cp / 4096) CHAR(128 + REMDR(cp / 64, 64)) CHAR(128 + REMDR(cp, 64)); return; }
    Utf8 = CHAR(240 + cp / 262144) CHAR(128 + REMDR(cp / 4096, 64)) CHAR(128 + REMDR(cp / 64, 64)) CHAR(128 + REMDR(cp, 64)); return;
}
function HexCp(h, v, i) { v = 0; i = 0; while (LE(i = i + 1, SIZE(h))) { v = v * 16 + hexval[SUBSTR(h, i, 1)]; } HexCp = v; return; }
function HexList(s, out, h) { out = ''; while (s ? (POS(0) SPAN(hex_digits) . h FENCE(',' | epsilon) REM . s)) { out = out Utf8(HexCp(h)); } HexList = out; return; }
function DecList(s, out, d) { out = ''; while (s ? (POS(0) SPAN(digits) . d FENCE(',' | epsilon) REM . s)) { out = out Utf8(d); } DecList = out; return; }
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
    if (IDENT(kind, 'TT_QLIT')) { dq_buf = dq_buf v; dq_lit = 1; dq_code = 0; return; }
    DqFlush();
    if (EQ(dq_n, 0)) { dq_n = 1; return; }
    Reduce('TT_CAT', 2); return;
}
function DqCode(code, save) { save = Src; Src = code; if (~(code ? (POS(0) *$' ' *item *$' ' RPOS(0)))) { Src = save; freturn; } Src = save; return; }
function Interp(s, c, h, code, save_n, save_l, save_b, save_c) {
    save_n = dq_n; save_l = dq_lit; save_b = dq_buf; save_c = dq_code; dq_n = 0; dq_lit = 0; dq_buf = ''; dq_code = 0;
    while (GT(SIZE(s), 0)) {
        if (s ? (POS(0) bSlash 'x[' BREAK(']') . h ']' REM . s))            { DqPiece('TT_QLIT', HexList(h)); continue; }
        if (s ? (POS(0) bSlash 'x' SPAN(hex_digits) . h REM . s))           { DqPiece('TT_QLIT', Utf8(HexCp(h))); continue; }
        if (s ? (POS(0) bSlash 'c[' BREAK(']') . h ']' REM . s))            { DqPiece('TT_QLIT', DecList(h)); continue; }
        if (s ? (POS(0) bSlash LEN(1) . c REM . s))                          { DqPiece('TT_QLIT', (DIFFER(esc_tbl[c]) esc_tbl[c], bSlash c)); continue; }
        if (s ? (POS(0) '{' BREAK('}') . code '}' REM . s))                  { dq_lit = 1; DqFlush(); if (DqCode(code)) { DqPiece('TT_X'); dq_code = 1; } else { DqPiece('TT_QLIT', '{' code '}'); } continue; }
        if (s ? (POS(0) '$' ANY(wordchars '.!^*_<') ))                       { DqFlush(); if (s ? (POS(0) (*var_tok *dq_post) REM . s)) { DqPiece('TT_X'); continue; } }
        if (s ? (POS(0) '@' *alpha))                                         { DqFlush(); if (s ? (POS(0) ('@' *ident) . var_tx . *VarNode(var_tx) *dq_asub REM . s)) { DqPiece('TT_X'); continue; } }
        if (s ? (POS(0) SPAN(' ' tab nl cr) . c REM . s))                    { DqPiece('TT_QLIT', c); continue; }
        if (s ? (POS(0) BREAK(bSlash '$@{' ' ' tab nl cr) . c REM . s))       { if (DIFFER(c)) { DqPiece('TT_QLIT', c); continue; } }
        if (s ? (POS(0) LEN(1) . c REM . s))                                 { DqPiece('TT_QLIT', c); continue; }
    }
    if (EQ(dq_code, 1) EQ(dq_lit, 0)) { dq_lit = 1; }
    DqFlush();
    if (EQ(dq_n, 0)) { Sh('TT_QLIT', ''); }
    dq_n = save_n; dq_lit = save_l; dq_buf = save_b; dq_code = save_c;
    Interp = .dummy; nreturn;
}
dq_asub  = FENCE( '[' *$' ' *item *$' ' ']' . *ArrGet() | epsilon );
dq_sub   = ( '[' *$' ' *item *$' ' ']' . *Reduce('TT_ARR_GET', 2) *dq_post
           | '<' BREAK('>') . hk_tx '>' . *Sh('TT_QLIT', hk_tx) . *Reduce('TT_HASH_GET', 2) *dq_post
           | '{' *$' ' *item *$' ' '}' . *Reduce('TT_HASH_GET', 2) *dq_post );
dq_post  = FENCE( *dq_sub
                | '.' *ident . mth_tx @mq *NoneAt(mq, '(') . *Sh('TT_QLIT', mth_tx) . *Reduce('TT_METHCALL', 2) *dq_post
                | epsilon );
dq_body  = ARBNO( NOTANY('"' bSlash) | bSlash LEN(1) );
sq_body  = ARBNO( NOTANY("'" bSlash) | bSlash LEN(1) );
word     = (NOTANY(' ' tab nl '>') FENCE(BREAK(' ' tab nl '>') | REM)) . w_tx . *Sh('TT_QLIT', DecodeSq(w_tx)) . *IncCounter();
words    = FENCE(SPAN(' ' tab nl) | epsilon) FENCE(*word FENCE(SPAN(' ' tab nl) | epsilon) *words | epsilon);
string   = ( "'" *sq_body . str_tx "'"                                 . *Sh('TT_QLIT', DecodeSq(str_tx))
           | '"' *dq_body . str_tx '"'                                 . *Interp(str_tx)
           | 'qq' '[' BREAK(']') . str_tx ']'                          . *Interp(str_tx)
           | 'q' FENCE(':w' | epsilon) '[' BREAK(']') . str_tx ']'    . *Sh('TT_QLIT', str_tx)
           | 'Q' '[' BREAK(']') . str_tx ']'                           . *Sh('TT_QLIT', str_tx)
           );
qwords   = '<' @qw_q *NoneAt(qw_q, '=<-') . *PushCounter() . *Sh('TT_VAR', '__rk_arr') *words '>' . *Reduce('TT_FNC', nTop() + 1, '__rk_arr') . *PopCounter();
rx_cls = TABLE();
rx_cls['alpha'] = '[A-Za-z]'; rx_cls['digit'] = '[0-9]'; rx_cls['alnum'] = '[A-Za-z0-9]'; rx_cls['upper'] = '[A-Z]'; rx_cls['lower'] = '[a-z]'; rx_cls['space'] = bSlash 's';
rx_cls['xdigit'] = '[0-9A-Fa-f]'; rx_cls['ws'] = bSlash 's*'; rx_cls['punct'] = '[!-/:-@' bSlash '[-`{-~]';
rx_meta = bSlash '()[]{}<>|*+?.^$';
function RxClass(k, out, c) {
    out = '';
    while (GT(SIZE(k), 0)) {
        if (k ? (POS(0) SPAN(' ' tab nl) REM . k))        { continue; }
        if (k ? (POS(0) bSlash LEN(1) . c REM . k))       { out = out bSlash c; continue; }
        if (k ? (POS(0) '..' REM . k))                    { out = out '-'; continue; }
        if (k ? (POS(0) ANY('-^]') . c REM . k))          { out = out bSlash c; continue; }
        if (k ? (POS(0) LEN(1) . c REM . k))              { out = out c; continue; }
    }
    RxClass = out; return;
}
function RxQuote(k, out, c) { out = ''; while (k ? (POS(0) FENCE(bSlash | epsilon) LEN(1) . c REM . k)) { out = out (rx_meta ? c bSlash, '') c; } RxQuote = out; return; }
function RxTrans(s, out, c, k, q) {
    out = '';
    while (GT(SIZE(s), 0)) {
        if (s ? (POS(0) SPAN(' ' tab nl cr) REM . s))                        { continue; }
        if (s ? (POS(0) '#' (BREAK(nl) | REM) REM . s))                      { continue; }
        if (s ? (POS(0) ANY('"' "'") $ q ARBNO(NOTANY(*q bSlash) | bSlash LEN(1)) . k (*q | RPOS(0)) REM . s)) { out = out RxQuote(k); continue; }
        if (s ? (POS(0) bSlash LEN(1) . c REM . s))                          { out = out bSlash c; continue; }
        if (s ? (POS(0) '||' REM . s))                                       { out = out '|'; continue; }
        if (s ? (POS(0) '$<' SPAN(wordchars '-') . k '>=(' REM . s))         { out = out '<' k '>('; continue; }
        if (s ? (POS(0) '<' FENCE('-' | epsilon) . c '[' BREAK(']') . k ']' ARBNO('+[' BREAK(']') . k2 ']' . *RxMore()) FENCE('>' | epsilon) REM . s)) { out = out '[' (IDENT(c, '-') '^', '') RxClass(k) rx_more ']'; rx_more = ''; continue; }
        if (s ? (POS(0) '<' SPAN(wordchars '-') . k '>')) { if (DIFFER(rx_cls[k])) { s = SUBSTR(s, SIZE(k) + 3); out = out rx_cls[k]; continue; } }
        if (s ? (POS(0) '[' REM . s))                                        { out = out '(?:'; continue; }
        if (s ? (POS(0) ']' REM . s))                                        { out = out ')'; continue; }
        if (s ? (POS(0) LEN(1) . c REM . s))                                 { out = out c; continue; }
    }
    RxTrans = out; return;
}
rx_more = '';
function RxMore() { rx_more = rx_more RxClass(k2); RxMore = .dummy; nreturn; }
rx_kind = TABLE();
function RxNodeK(tx, k) { Push(mk('TT_VAR', '__rk_regex')); Sh('TT_QLIT', (IDENT(k, 'subst') tx, RxTrans(tx))); Reduce('TT_FNC', 2, '__rk_regex'); rx_node[Top()] = 1; rx_kind[Top()] = k; RxNodeK = .dummy; nreturn; }
function RxNode(tx) { Push(mk('TT_VAR', '__rk_regex')); Sh('TT_QLIT', RxTrans(tx)); Reduce('TT_FNC', 2, '__rk_regex'); rx_node[Top()] = 1; RxNode = .dummy; nreturn; }
rx_node  = TABLE();
regex    = ( '/' ARBNO(NOTANY('/' bSlash) | bSlash LEN(1)) . rx_tx '/'   . *RxNode(rx_tx)
           | ('rx' | 'm') (FENCE(':i' | epsilon)) . rxa_tx ':g' '/' ARBNO(NOTANY('/' bSlash) | bSlash LEN(1)) . rx_tx '/' . *RxNodeK(rxa_tx rx_tx, 'match_global')
           | ('rx' | 'm') (FENCE(':i' | epsilon)) . rxa_tx '/' ARBNO(NOTANY('/' bSlash) | bSlash LEN(1)) . rx_tx '/' . *RxNode(rxa_tx rx_tx)
           | 's' '/' ARBNO(NOTANY('/' bSlash) | bSlash LEN(1)) . rx_tx '/' ARBNO(NOTANY('/' bSlash) | bSlash LEN(1)) . rxr_tx '/' . *RxNodeK(RxTrans(rx_tx) CHAR(1) rxr_tx CHAR(1) '-', 'subst')
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
opword_tbl['enum'] = 1; opword_tbl['await'] = 1; opword_tbl['start'] = 1; opword_tbl['X'] = 1; opword_tbl['Z'] = 1; opword_tbl['with'] = 1; opword_tbl['without'] = 1; opword_tbl['if'] = 1; opword_tbl['unless'] = 1; opword_tbl['while'] = 1; opword_tbl['until'] = 1; opword_tbl['for'] = 1; opword_tbl['given'] = 1; opword_tbl['when'] = 1; opword_tbl['else'] = 1; opword_tbl['elsif'] = 1;
function PairAhead(q, c) { q = q + 1; while (IDENT(SUBSTR(Src, q, 1), ' ')) { q = q + 1; } if (IDENT(SUBSTR(Src, q, 2), '=>')) { return; } freturn; }
function NoInfixAhead(q, c1, c2) { c1 = SUBSTR(Src, q + 1, 1); c2 = SUBSTR(Src, q + 2, 1); if (IDENT(c1, '-') IDENT(c2, '>')) { freturn; } if (('*/~<>=!?&|^%.' ? c1) (IDENT(c2, ' '), IDENT(c2, tab))) { freturn; } if (IDENT(SUBSTR(Src, q + 1, 2), '..')) { freturn; } return; }
function WordArgAhead(q, c) { c = SUBSTR(Src, q + 1, 1); if (IDENT(c)) { return; } if ('),;' ? c) { return; } freturn; }
const_tbl = TABLE(); const_tbl['pi'] = 1; const_tbl['e'] = 1; const_tbl['tau'] = 1; const_tbl['i'] = 1; const_tbl['Inf'] = 1; const_tbl['NaN'] = 1; const_tbl['Nil'] = 1; const_tbl['Empty'] = 1; const_tbl['True'] = 1; const_tbl['False'] = 1;
function NotOpWord(w) { if (DIFFER(opword_tbl[w])) { freturn; } return; }
function NotCall(w) { if (DIFFER(opword_tbl[w])) { freturn; } if (DIFFER(const_tbl[w])) { freturn; } return; }
function NoteConst(w) { const_tbl[w] = 1; NoteConst = .dummy; nreturn; }
function NoteNow(w) { const_tbl[w] = 1; NoteNow = ''; return; }
function EndAhead(q, c) { q = q + 1; while ((IDENT(SUBSTR(Src, q, 1), ' '), IDENT(SUBSTR(Src, q, 1), tab))) { q = q + 1; } c = SUBSTR(Src, q, 1); if (IDENT(c)) { return; } if (';}),' ? c) { return; } if (IDENT(c, nl)) { return; } freturn; }
function IsListop(w)  { if (DIFFER(listop_tbl[w])) { return; } freturn; }
function Call(nm, n, k, i, c) {
    nm = (DIFFER(rename_tbl[nm]) rename_tbl[nm], nm);
    if (IDENT(nm, 'fail')) { i = n; while (GT(i, 0)) { Pop(); i = i - 1; } Reduce('TT_RETURN', 0); Call = .dummy; nreturn; }
    if (IDENT(nm, 'reverse') GT(n, 1)) { c = ARRAY('1:' n); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } Push(mk('TT_VAR', '__rk_arr')); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_FNC', n + 1, '__rk_arr'); Reduce('TT_REVERSE', 1); Call = .dummy; nreturn; }
    if (IDENT(nm, 'take') GT(n, 1)) { c = ARRAY('1:' n); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } Push(mk('TT_VAR', '__rk_arr')); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_FNC', n + 1, '__rk_arr'); Reduce('TT_SUSPEND', 1); Call = .dummy; nreturn; }
    if ((IDENT(nm, 'return'), IDENT(nm, 'die'), IDENT(nm, '__rk_exit')) GT(n, 1)) { c = ARRAY('1:' n); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } Push(c[1]); n = 1; }
    if ((IDENT(nm, 'exists'), IDENT(nm, 'delete')) EQ(n, 1) IDENT(t(Top()), 'TT_HASH_GET')) { c = ARRAY('1:1'); c[1] = Pop(); Push(c(c[1])[1]); Push(c(c[1])[2]); Reduce((IDENT(nm, 'exists') 'TT_HASH_EXISTS', 'TT_HASH_DELETE'), 2); Call = .dummy; nreturn; }
    k = call_kind[nm];
    if (DIFFER(k) EQ(n, 1)) { c = ARRAY('1:1'); c[1] = Top(); if (DIFFER(paren_list[c[1]])) { c[1] = Pop(); i = 1; while (LT(i, n(c[1]))) { i = i + 1; Push(c(c[1])[i]); } n = n(c[1]) - 1; } }
    if (DIFFER(k)) {
        if ((IDENT(k, 'TT_MAP'), IDENT(k, 'TT_GREP')) GT(n, 0)) { c = ARRAY('1:' n); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; }
            if (IDENT(t(c[1]), 'TT_ANON_BLOCK') EQ(n(c(c[1])[1]), 1)) { c[1] = c(c(c[1])[1])[1]; } i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } }
        Reduce(k, n); Call = .dummy; nreturn; }
    c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; }
    if (IDENT(nm, 'await')) { if (EQ(n, 1)) { Push(c[1]); Call = .dummy; nreturn; } Push(mk('TT_VAR', '__rk_arr')); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_FNC', n + 1, '__rk_arr'); Call = .dummy; nreturn; }
    if (IDENT(nm, 'defined') EQ(n, 1)) { Push(c[1]); Push(mk('TT_QLIT', 'defined')); Reduce('TT_METHCALL', 2); Call = .dummy; nreturn; }
    if (GT(nFlat(n, c), n)) { Push(mk('TT_VAR', '__rk_named_call')); Push(mk('TT_QLIT', nm)); Push(mk('TT_ILIT', 2 * n - nFlat(n, c)));
        i = 0; while (LT(i, n)) { i = i + 1; if (~IsNamed(c[i])) { Push(ArgVal(c[i])); } }
        i = 0; while (LT(i, n)) { i = i + 1; if (IsNamed(c[i])) { Push(c(c[i])[2]); Push(c(c[i])[3]); } }
        Reduce('TT_FNC', nFlat(n, c) + 3, '__rk_named_call'); Call = .dummy; nreturn; }
    Push(mk('TT_VAR', nm)); i = 0; while (LT(i, n)) { i = i + 1; Push(ArgVal(c[i])); }
    Reduce('TT_FNC', n + 1, nm); Call = .dummy; nreturn;
}
core_tbl = TABLE(2048); core_tbl['AST'] = 1; core_tbl['Allomorph'] = 1; core_tbl['Any'] = 1; core_tbl['Array'] = 1; core_tbl['Array::Element'] = 1; core_tbl['Array::Element::Access'] = 1;
core_tbl['Array::Shaped'] = 1; core_tbl['Array::Shaped1'] = 1; core_tbl['Array::Shaped2'] = 1; core_tbl['Array::Shaped3'] = 1; core_tbl['Array::Slice'] = 1;
core_tbl['Array::Slice::Access'] = 1; core_tbl['Array::Slice::Assign'] = 1; core_tbl['Array::Slice::Bind'] = 1; core_tbl['Array::Typed'] = 1; core_tbl['Associative'] = 1;
core_tbl['Attribute'] = 1; core_tbl['Awaitable'] = 1; core_tbl['Awaitable::Handle'] = 1; core_tbl['Awaiter'] = 1; core_tbl['Awaiter::Blocking'] = 1; core_tbl['Backtrace'] = 1;
core_tbl['Backtrace::Frame'] = 1; core_tbl['Bag'] = 1; core_tbl['BagHash'] = 1; core_tbl['Baggy'] = 1; core_tbl['BigEndian'] = 1; core_tbl['Blob'] = 1; core_tbl['Block'] = 1;
core_tbl['Bool'] = 1; core_tbl['Bool::False'] = 1; core_tbl['Bool::True'] = 1; core_tbl['Broken'] = 1; core_tbl['Buf'] = 1; core_tbl['CORE-SETTING-REV'] = 1; core_tbl['CX'] = 1;
core_tbl['CX::Done'] = 1; core_tbl['CX::Emit'] = 1; core_tbl['CX::Last'] = 1; core_tbl['CX::Next'] = 1; core_tbl['CX::Proceed'] = 1; core_tbl['CX::Redo'] = 1; core_tbl['CX::Return'] = 1;
core_tbl['CX::Succeed'] = 1; core_tbl['CX::Take'] = 1; core_tbl['CX::Warn'] = 1; core_tbl['CallFrame'] = 1; core_tbl['Callable'] = 1; core_tbl['Cancellation'] = 1; core_tbl['Capture'] = 1;
core_tbl['Channel'] = 1; core_tbl['Code'] = 1; core_tbl['Collation'] = 1; core_tbl['CompUnit'] = 1; core_tbl['CompUnit::DependencySpecification'] = 1; core_tbl['CompUnit::Handle'] = 1;
core_tbl['CompUnit::Loader'] = 1; core_tbl['CompUnit::PrecompilationDependency'] = 1; core_tbl['CompUnit::PrecompilationDependency::File'] = 1; core_tbl['CompUnit::PrecompilationId'] = 1;
core_tbl['CompUnit::PrecompilationRepository'] = 1; core_tbl['CompUnit::PrecompilationRepository::Default'] = 1; core_tbl['CompUnit::PrecompilationRepository::None'] = 1;
core_tbl['CompUnit::PrecompilationStore'] = 1; core_tbl['CompUnit::PrecompilationStore::File'] = 1; core_tbl['CompUnit::PrecompilationStore::FileSystem'] = 1;
core_tbl['CompUnit::PrecompilationUnit'] = 1; core_tbl['CompUnit::PrecompilationUnit::File'] = 1; core_tbl['CompUnit::Repository'] = 1; core_tbl['CompUnit::Repository::AbsolutePath'] = 1;
core_tbl['CompUnit::Repository::Distribution'] = 1; core_tbl['CompUnit::Repository::FileSystem'] = 1; core_tbl['CompUnit::Repository::Installable'] = 1;
core_tbl['CompUnit::Repository::Installation'] = 1; core_tbl['CompUnit::Repository::Locally'] = 1; core_tbl['CompUnit::Repository::NQP'] = 1; core_tbl['CompUnit::Repository::Perl5'] = 1;
core_tbl['CompUnit::Repository::Spec'] = 1; core_tbl['CompUnit::Repository::Unknown'] = 1; core_tbl['CompUnit::RepositoryRegistry'] = 1; core_tbl['Compiler'] = 1; core_tbl['Complex'] = 1;
core_tbl['ComplexStr'] = 1; core_tbl['ContainerDescriptor'] = 1; core_tbl['Cool'] = 1; core_tbl['CurrentThreadScheduler'] = 1; core_tbl['Cursor'] = 1; core_tbl['Date'] = 1;
core_tbl['DateTime'] = 1; core_tbl['Dateish'] = 1; core_tbl['Deprecation'] = 1; core_tbl['Distribution'] = 1; core_tbl['Distribution::Hash'] = 1; core_tbl['Distribution::Locally'] = 1;
core_tbl['Distribution::Path'] = 1; core_tbl['Distribution::Resource'] = 1; core_tbl['Distribution::Resources'] = 1; core_tbl['Distro'] = 1; core_tbl['Duration'] = 1; core_tbl['Empty'] = 1;
core_tbl['Encoding'] = 1; core_tbl['Encoding::Builtin'] = 1; core_tbl['Encoding::Decoder'] = 1; core_tbl['Encoding::Decoder::Builtin'] = 1; core_tbl['Encoding::Encoder'] = 1;
core_tbl['Encoding::Encoder::Builtin'] = 1; core_tbl['Encoding::Encoder::TranslateNewlineWrapper'] = 1; core_tbl['Encoding::Registry'] = 1; core_tbl['Endian'] = 1;
core_tbl['Endian::BigEndian'] = 1; core_tbl['Endian::LittleEndian'] = 1; core_tbl['Endian::NativeEndian'] = 1; core_tbl['Enumeration'] = 1; core_tbl['Exception'] = 1;
core_tbl['Exceptions'] = 1; core_tbl['Exceptions::JSON'] = 1; core_tbl['Failure'] = 1; core_tbl['False'] = 1; core_tbl['FatRat'] = 1; core_tbl['FileChangeEvent'] = 1;
core_tbl['FileChangeEvent::FileChanged'] = 1; core_tbl['FileChangeEvent::FileRenamed'] = 1; core_tbl['FileChanged'] = 1; core_tbl['FileRenamed'] = 1; core_tbl['ForeignCode'] = 1;
core_tbl['Grammar'] = 1; core_tbl['HardRoutine'] = 1; core_tbl['Hash'] = 1; core_tbl['Hash::Object'] = 1; core_tbl['Hash::Typed'] = 1; core_tbl['Hyper'] = 1;
core_tbl['HyperConfiguration'] = 1; core_tbl['HyperSeq'] = 1; core_tbl['HyperWhatever'] = 1; core_tbl['IO'] = 1; core_tbl['IO::ArgFiles'] = 1; core_tbl['IO::CatHandle'] = 1;
core_tbl['IO::Handle'] = 1; core_tbl['IO::Notification'] = 1; core_tbl['IO::Notification::Change'] = 1; core_tbl['IO::Path'] = 1; core_tbl['IO::Path::Cygwin'] = 1;
core_tbl['IO::Path::Parts'] = 1; core_tbl['IO::Path::QNX'] = 1; core_tbl['IO::Path::Spec'] = 1; core_tbl['IO::Path::Unix'] = 1; core_tbl['IO::Path::Win32'] = 1;
core_tbl['IO::Path::slurp-size'] = 1; core_tbl['IO::Pipe'] = 1; core_tbl['IO::Socket'] = 1; core_tbl['IO::Socket::Async'] = 1; core_tbl['IO::Socket::INET'] = 1; core_tbl['IO::Spec'] = 1;
core_tbl['IO::Spec::Cygwin'] = 1; core_tbl['IO::Spec::QNX'] = 1; core_tbl['IO::Spec::Unix'] = 1; core_tbl['IO::Spec::Win32'] = 1; core_tbl['IO::Special'] = 1; core_tbl['Inf'] = 1;
core_tbl['Instant'] = 1; core_tbl['Int'] = 1; core_tbl['IntAttrRef'] = 1; core_tbl['IntLexRef'] = 1; core_tbl['IntPosRef'] = 1; core_tbl['IntStr'] = 1; core_tbl['Iterable'] = 1;
core_tbl['IterationBuffer'] = 1; core_tbl['IterationEnd'] = 1; core_tbl['Iterator'] = 1; core_tbl['JSONException'] = 1; core_tbl['Junction'] = 1; core_tbl['Kept'] = 1;
core_tbl['Kernel'] = 1; core_tbl['Label'] = 1; core_tbl['Less'] = 1; core_tbl['List'] = 1; core_tbl['List::Reifier'] = 1; core_tbl['LittleEndian'] = 1; core_tbl['Lock'] = 1;
core_tbl['Lock::Async'] = 1; core_tbl['Lock::ConditionVariable'] = 1; core_tbl['Lock::Soft'] = 1; core_tbl['Macro'] = 1; core_tbl['Map'] = 1; core_tbl['Match'] = 1;
core_tbl['Metamodel'] = 1; core_tbl['Metamodel::Archetypes'] = 1; core_tbl['Metamodel::ArrayType'] = 1; core_tbl['Metamodel::AttributeContainer'] = 1; core_tbl['Metamodel::BUILDPLAN'] = 1;
core_tbl['Metamodel::BaseType'] = 1; core_tbl['Metamodel::BoolificationProtocol'] = 1; core_tbl['Metamodel::C3MRO'] = 1; core_tbl['Metamodel::ClassHOW'] = 1;
core_tbl['Metamodel::CoercionHOW'] = 1; core_tbl['Metamodel::ConcreteRoleHOW'] = 1; core_tbl['Metamodel::Concretization'] = 1; core_tbl['Metamodel::ConcretizationCache'] = 1;
core_tbl['Metamodel::Configuration'] = 1; core_tbl['Metamodel::ContainerSpecProtocol'] = 1; core_tbl['Metamodel::CurriedRoleHOW'] = 1; core_tbl['Metamodel::DefaultParent'] = 1;
core_tbl['Metamodel::DefiniteHOW'] = 1; core_tbl['Metamodel::Documenting'] = 1; core_tbl['Metamodel::EnumHOW'] = 1; core_tbl['Metamodel::Explaining'] = 1;
core_tbl['Metamodel::Finalization'] = 1; core_tbl['Metamodel::GenericHOW'] = 1; core_tbl['Metamodel::GrammarHOW'] = 1; core_tbl['Metamodel::InvocationProtocol'] = 1;
core_tbl['Metamodel::LanguageRevision'] = 1; core_tbl['Metamodel::MROBasedMethodDispatch'] = 1; core_tbl['Metamodel::MROBasedTypeChecking'] = 1;
core_tbl['Metamodel::MetaMethodContainer'] = 1; core_tbl['Metamodel::MethodContainer'] = 1; core_tbl['Metamodel::MethodDelegation'] = 1; core_tbl['Metamodel::Mixins'] = 1;
core_tbl['Metamodel::ModuleHOW'] = 1; core_tbl['Metamodel::MultiMethodContainer'] = 1; core_tbl['Metamodel::MultipleInheritance'] = 1; core_tbl['Metamodel::Naming'] = 1;
core_tbl['Metamodel::NativeHOW'] = 1; core_tbl['Metamodel::NativeRefHOW'] = 1; core_tbl['Metamodel::Nominalizable'] = 1; core_tbl['Metamodel::PackageHOW'] = 1;
core_tbl['Metamodel::ParametricRoleGroupHOW'] = 1; core_tbl['Metamodel::ParametricRoleHOW'] = 1; core_tbl['Metamodel::Primitives'] = 1; core_tbl['Metamodel::PrivateMethodContainer'] = 1;
core_tbl['Metamodel::REPRComposeProtocol'] = 1; core_tbl['Metamodel::RoleContainer'] = 1; core_tbl['Metamodel::RolePunning'] = 1; core_tbl['Metamodel::Stashing'] = 1;
core_tbl['Metamodel::SubsetHOW'] = 1; core_tbl['Metamodel::Trusting'] = 1; core_tbl['Metamodel::TypePretense'] = 1; core_tbl['Metamodel::Versioning'] = 1; core_tbl['Method'] = 1;
core_tbl['MethodDispatcher'] = 1; core_tbl['Mix'] = 1; core_tbl['MixHash'] = 1; core_tbl['Mixy'] = 1; core_tbl['More'] = 1; core_tbl['Mu'] = 1; core_tbl['MultiDispatcher'] = 1;
core_tbl['NFC'] = 1; core_tbl['NFD'] = 1; core_tbl['NFKC'] = 1; core_tbl['NFKD'] = 1; core_tbl['NQPMatchRole'] = 1; core_tbl['NQPdidMATCH'] = 1; core_tbl['NaN'] = 1;
core_tbl['NativeEndian'] = 1; core_tbl['Nil'] = 1; core_tbl['Num'] = 1; core_tbl['NumAttrRef'] = 1; core_tbl['NumLexRef'] = 1; core_tbl['NumPosRef'] = 1; core_tbl['NumStr'] = 1;
core_tbl['Numeric'] = 1; core_tbl['NumericEnumeration'] = 1; core_tbl['NumericStringyEnumeration'] = 1; core_tbl['ObjAt'] = 1; core_tbl['Order'] = 1; core_tbl['Order::Less'] = 1;
core_tbl['Order::More'] = 1; core_tbl['Order::Same'] = 1; core_tbl['PF_INET'] = 1; core_tbl['PF_INET6'] = 1; core_tbl['PF_LOCAL'] = 1; core_tbl['PF_MAX'] = 1; core_tbl['PF_UNIX'] = 1;
core_tbl['PF_UNSPEC'] = 1; core_tbl['PROTO_TCP'] = 1; core_tbl['PROTO_UDP'] = 1; core_tbl['Pair'] = 1; core_tbl['ParallelSequence'] = 1; core_tbl['Parameter'] = 1; core_tbl['Perl'] = 1;
core_tbl['Planned'] = 1; core_tbl['Pod'] = 1; core_tbl['Pod::Block'] = 1; core_tbl['Pod::Block::Code'] = 1; core_tbl['Pod::Block::Comment'] = 1; core_tbl['Pod::Block::Declarator'] = 1;
core_tbl['Pod::Block::Named'] = 1; core_tbl['Pod::Block::Para'] = 1; core_tbl['Pod::Block::Table'] = 1; core_tbl['Pod::Config'] = 1; core_tbl['Pod::Defn'] = 1;
core_tbl['Pod::FormattingCode'] = 1; core_tbl['Pod::Heading'] = 1; core_tbl['Pod::Item'] = 1; core_tbl['Pod::Raw'] = 1; core_tbl['Positional'] = 1; core_tbl['PositionalBindFailover'] = 1;
core_tbl['PredictiveIterator'] = 1; core_tbl['Proc'] = 1; core_tbl['Proc::Async'] = 1; core_tbl['Proc::Async::Pipe'] = 1; core_tbl['Promise'] = 1; core_tbl['PromiseStatus'] = 1;
core_tbl['PromiseStatus::Broken'] = 1; core_tbl['PromiseStatus::Kept'] = 1; core_tbl['PromiseStatus::Planned'] = 1; core_tbl['ProtocolFamily'] = 1; core_tbl['ProtocolFamily::PF_INET'] = 1;
core_tbl['ProtocolFamily::PF_INET6'] = 1; core_tbl['ProtocolFamily::PF_LOCAL'] = 1; core_tbl['ProtocolFamily::PF_MAX'] = 1; core_tbl['ProtocolFamily::PF_UNIX'] = 1;
core_tbl['ProtocolFamily::PF_UNSPEC'] = 1; core_tbl['ProtocolType'] = 1; core_tbl['ProtocolType::PROTO_TCP'] = 1; core_tbl['ProtocolType::PROTO_UDP'] = 1; core_tbl['Proxy'] = 1;
core_tbl['PseudoStash'] = 1; core_tbl['QuantHash'] = 1; core_tbl['REPL'] = 1; core_tbl['RaceSeq'] = 1; core_tbl['Raku'] = 1; core_tbl['Rakudo'] = 1; core_tbl['Rakudo::Deprecations'] = 1;
core_tbl['Rakudo::Internals'] = 1; core_tbl['Rakudo::Internals::CompilerServices'] = 1; core_tbl['Rakudo::Internals::EvalIdSource'] = 1; core_tbl['Rakudo::Internals::HyperBatcher'] = 1;
core_tbl['Rakudo::Internals::HyperIteratorBatcher'] = 1; core_tbl['Rakudo::Internals::HyperJoiner'] = 1; core_tbl['Rakudo::Internals::HyperPipeline'] = 1;
core_tbl['Rakudo::Internals::HyperProcessor'] = 1; core_tbl['Rakudo::Internals::HyperRaceSharedImpl'] = 1; core_tbl['Rakudo::Internals::HyperRebatcher'] = 1;
core_tbl['Rakudo::Internals::HyperToIterator'] = 1; core_tbl['Rakudo::Internals::HyperWorkBatch'] = 1; core_tbl['Rakudo::Internals::HyperWorkStage'] = 1;
core_tbl['Rakudo::Internals::ImplementationDetail'] = 1; core_tbl['Rakudo::Internals::IterationSet'] = 1; core_tbl['Rakudo::Internals::JSON'] = 1;
core_tbl['Rakudo::Internals::LoweredAwayLexical'] = 1; core_tbl['Rakudo::Internals::RaceToIterator'] = 1; core_tbl['Rakudo::Internals::ReactAwaitHandle'] = 1;
core_tbl['Rakudo::Internals::ReactAwaitable'] = 1; core_tbl['Rakudo::Internals::ReactOneWheneverAwaitHandle'] = 1; core_tbl['Rakudo::Internals::RegexBoolification6cMarker'] = 1;
core_tbl['Rakudo::Internals::ShapedArrayCommon'] = 1; core_tbl['Rakudo::Internals::SprintfHandler'] = 1; core_tbl['Rakudo::Internals::SupplySequencer'] = 1; core_tbl['Rakudo::Iterator'] = 1;
core_tbl['Rakudo::Iterator::Blobby'] = 1; core_tbl['Rakudo::Iterator::Dir'] = 1; core_tbl['Rakudo::Iterator::DirTest'] = 1; core_tbl['Rakudo::Iterator::Mappy'] = 1;
core_tbl['Rakudo::Iterator::Mappy-kv-from-pairs'] = 1; core_tbl['Rakudo::Iterator::ShapeBranch'] = 1; core_tbl['Rakudo::Iterator::ShapeLeaf'] = 1; core_tbl['Rakudo::Metaops'] = 1;
core_tbl['Rakudo::QuantHash'] = 1; core_tbl['Rakudo::QuantHash::Pairs'] = 1; core_tbl['Rakudo::QuantHash::Quanty-kv'] = 1; core_tbl['Rakudo::SlippyIterator'] = 1;
core_tbl['Rakudo::Sorting'] = 1; core_tbl['Rakudo::Supply'] = 1; core_tbl['Rakudo::Supply::BlockAddWheneverAwaiter'] = 1; core_tbl['Rakudo::Supply::BlockState'] = 1;
core_tbl['Rakudo::Supply::BlockTappable'] = 1; core_tbl['Rakudo::Supply::CachedAwaitHandle'] = 1; core_tbl['Rakudo::Supply::OneEmitTappable'] = 1;
core_tbl['Rakudo::Supply::OneWheneverState'] = 1; core_tbl['Rakudo::Supply::OneWheneverTappable'] = 1; core_tbl['Rakudo::Unicodey'] = 1; core_tbl['Range'] = 1; core_tbl['Rat'] = 1;
core_tbl['RatStr'] = 1; core_tbl['Rational'] = 1; core_tbl['Real'] = 1; core_tbl['Regex'] = 1; core_tbl['Routine'] = 1; core_tbl['SIGABRT'] = 1; core_tbl['SIGALRM'] = 1;
core_tbl['SIGBREAK'] = 1; core_tbl['SIGBUS'] = 1; core_tbl['SIGCHLD'] = 1; core_tbl['SIGCONT'] = 1; core_tbl['SIGEMT'] = 1; core_tbl['SIGFPE'] = 1; core_tbl['SIGHUP'] = 1;
core_tbl['SIGILL'] = 1; core_tbl['SIGINFO'] = 1; core_tbl['SIGINT'] = 1; core_tbl['SIGIO'] = 1; core_tbl['SIGKILL'] = 1; core_tbl['SIGPIPE'] = 1; core_tbl['SIGPROF'] = 1;
core_tbl['SIGPWR'] = 1; core_tbl['SIGQUIT'] = 1; core_tbl['SIGSEGV'] = 1; core_tbl['SIGSTKFLT'] = 1; core_tbl['SIGSTOP'] = 1; core_tbl['SIGSYS'] = 1; core_tbl['SIGTERM'] = 1;
core_tbl['SIGTHR'] = 1; core_tbl['SIGTRAP'] = 1; core_tbl['SIGTSTP'] = 1; core_tbl['SIGTTIN'] = 1; core_tbl['SIGTTOU'] = 1; core_tbl['SIGURG'] = 1; core_tbl['SIGUSR1'] = 1;
core_tbl['SIGUSR2'] = 1; core_tbl['SIGVTALRM'] = 1; core_tbl['SIGWINCH'] = 1; core_tbl['SIGXCPU'] = 1; core_tbl['SIGXFSZ'] = 1; core_tbl['SOCK_DGRAM'] = 1; core_tbl['SOCK_MAX'] = 1;
core_tbl['SOCK_PACKET'] = 1; core_tbl['SOCK_RAW'] = 1; core_tbl['SOCK_RDM'] = 1; core_tbl['SOCK_SEQPACKET'] = 1; core_tbl['SOCK_STREAM'] = 1; core_tbl['Same'] = 1; core_tbl['Scalar'] = 1;
core_tbl['ScalarVAR'] = 1; core_tbl['Scheduler'] = 1; core_tbl['SeekFromBeginning'] = 1; core_tbl['SeekFromCurrent'] = 1; core_tbl['SeekFromEnd'] = 1; core_tbl['SeekType'] = 1;
core_tbl['SeekType::SeekFromBeginning'] = 1; core_tbl['SeekType::SeekFromCurrent'] = 1; core_tbl['SeekType::SeekFromEnd'] = 1; core_tbl['Semaphore'] = 1; core_tbl['Seq'] = 1;
core_tbl['Sequence'] = 1; core_tbl['Set'] = 1; core_tbl['SetHash'] = 1; core_tbl['Setty'] = 1; core_tbl['Signal'] = 1; core_tbl['Signal::SIGABRT'] = 1; core_tbl['Signal::SIGALRM'] = 1;
core_tbl['Signal::SIGBREAK'] = 1; core_tbl['Signal::SIGBUS'] = 1; core_tbl['Signal::SIGCHLD'] = 1; core_tbl['Signal::SIGCONT'] = 1; core_tbl['Signal::SIGEMT'] = 1;
core_tbl['Signal::SIGFPE'] = 1; core_tbl['Signal::SIGHUP'] = 1; core_tbl['Signal::SIGILL'] = 1; core_tbl['Signal::SIGINFO'] = 1; core_tbl['Signal::SIGINT'] = 1;
core_tbl['Signal::SIGIO'] = 1; core_tbl['Signal::SIGKILL'] = 1; core_tbl['Signal::SIGPIPE'] = 1; core_tbl['Signal::SIGPROF'] = 1; core_tbl['Signal::SIGPWR'] = 1;
core_tbl['Signal::SIGQUIT'] = 1; core_tbl['Signal::SIGSEGV'] = 1; core_tbl['Signal::SIGSTKFLT'] = 1; core_tbl['Signal::SIGSTOP'] = 1; core_tbl['Signal::SIGSYS'] = 1;
core_tbl['Signal::SIGTERM'] = 1; core_tbl['Signal::SIGTHR'] = 1; core_tbl['Signal::SIGTRAP'] = 1; core_tbl['Signal::SIGTSTP'] = 1; core_tbl['Signal::SIGTTIN'] = 1;
core_tbl['Signal::SIGTTOU'] = 1; core_tbl['Signal::SIGURG'] = 1; core_tbl['Signal::SIGUSR1'] = 1; core_tbl['Signal::SIGUSR2'] = 1; core_tbl['Signal::SIGVTALRM'] = 1;
core_tbl['Signal::SIGWINCH'] = 1; core_tbl['Signal::SIGXCPU'] = 1; core_tbl['Signal::SIGXFSZ'] = 1; core_tbl['Signature'] = 1; core_tbl['SignedBlob'] = 1; core_tbl['Slang'] = 1;
core_tbl['Slip'] = 1; core_tbl['SocketType'] = 1; core_tbl['SocketType::SOCK_DGRAM'] = 1; core_tbl['SocketType::SOCK_MAX'] = 1; core_tbl['SocketType::SOCK_PACKET'] = 1;
core_tbl['SocketType::SOCK_RAW'] = 1; core_tbl['SocketType::SOCK_RDM'] = 1; core_tbl['SocketType::SOCK_SEQPACKET'] = 1; core_tbl['SocketType::SOCK_STREAM'] = 1; core_tbl['SoftRoutine'] = 1;
core_tbl['Stash'] = 1; core_tbl['Str'] = 1; core_tbl['StrAttrRef'] = 1; core_tbl['StrDistance'] = 1; core_tbl['StrLexRef'] = 1; core_tbl['StrPosRef'] = 1; core_tbl['Stringy'] = 1;
core_tbl['StringyEnumeration'] = 1; core_tbl['Sub'] = 1; core_tbl['Submethod'] = 1; core_tbl['Supplier'] = 1; core_tbl['Supplier::Preserving'] = 1; core_tbl['Supply'] = 1;
core_tbl['Systemic'] = 1; core_tbl['Tap'] = 1; core_tbl['Tappable'] = 1; core_tbl['Thread'] = 1; core_tbl['Thread::THREAD_ERROR'] = 1; core_tbl['ThreadPoolScheduler'] = 1;
core_tbl['ThreadPoolScheduler::ThreadPoolAwaiter'] = 1; core_tbl['True'] = 1; core_tbl['UINT64_UPPER'] = 1; core_tbl['UInt'] = 1; core_tbl['UIntAttrRef'] = 1; core_tbl['UIntLexRef'] = 1;
core_tbl['UIntPosRef'] = 1; core_tbl['Uni'] = 1; core_tbl['UnsignedBlob'] = 1; core_tbl['VM'] = 1; core_tbl['ValueObjAt'] = 1; core_tbl['Variable'] = 1; core_tbl['Version'] = 1;
core_tbl['WalkList'] = 1; core_tbl['Whatever'] = 1; core_tbl['WhateverCode'] = 1; core_tbl['WrapDispatcher'] = 1; core_tbl['X'] = 1; core_tbl['X::AdHoc'] = 1; core_tbl['X::Adverb'] = 1;
core_tbl['X::Anon'] = 1; core_tbl['X::Anon::Augment'] = 1; core_tbl['X::Anon::Multi'] = 1; core_tbl['X::ArrayShapeMismatch'] = 1; core_tbl['X::Assignment'] = 1;
core_tbl['X::Assignment::ArrayShapeMismatch'] = 1; core_tbl['X::Assignment::RO'] = 1; core_tbl['X::Assignment::RO::Comp'] = 1; core_tbl['X::Assignment::ToShaped'] = 1;
core_tbl['X::Attribute'] = 1; core_tbl['X::Attribute::NoPackage'] = 1; core_tbl['X::Attribute::Package'] = 1; core_tbl['X::Attribute::Regex'] = 1; core_tbl['X::Attribute::Required'] = 1;
core_tbl['X::Attribute::Scope'] = 1; core_tbl['X::Attribute::Scope::Package'] = 1; core_tbl['X::Attribute::Undeclared'] = 1; core_tbl['X::Augment'] = 1;
core_tbl['X::Augment::NoSuchType'] = 1; core_tbl['X::Await'] = 1; core_tbl['X::Await::Died'] = 1; core_tbl['X::Backslash'] = 1; core_tbl['X::Backslash::NonVariableDollar'] = 1;
core_tbl['X::Backslash::UnrecognizedSequence'] = 1; core_tbl['X::BadType'] = 1; core_tbl['X::Bind'] = 1; core_tbl['X::Bind::NativeType'] = 1; core_tbl['X::Bind::Rebind'] = 1;
core_tbl['X::Bind::Slice'] = 1; core_tbl['X::Bind::ZenSlice'] = 1; core_tbl['X::Buf'] = 1; core_tbl['X::Buf::AsStr'] = 1; core_tbl['X::Buf::Pack'] = 1;
core_tbl['X::Buf::Pack::NonASCII'] = 1; core_tbl['X::Caller'] = 1; core_tbl['X::Caller::NotDynamic'] = 1; core_tbl['X::Cannot'] = 1; core_tbl['X::Cannot::Capture'] = 1;
core_tbl['X::Cannot::Empty'] = 1; core_tbl['X::Cannot::Lazy'] = 1; core_tbl['X::Cannot::Map'] = 1; core_tbl['X::Cannot::New'] = 1; core_tbl['X::Channel'] = 1;
core_tbl['X::Channel::ReceiveOnClosed'] = 1; core_tbl['X::Channel::SendOnClosed'] = 1; core_tbl['X::Coerce'] = 1; core_tbl['X::Coerce::Impossible'] = 1; core_tbl['X::Comp'] = 1;
core_tbl['X::Comp::AdHoc'] = 1; core_tbl['X::Comp::BeginTime'] = 1; core_tbl['X::Comp::FailGoal'] = 1; core_tbl['X::Comp::Group'] = 1; core_tbl['X::Comp::NYI'] = 1;
core_tbl['X::Comp::Trait'] = 1; core_tbl['X::Comp::Trait::Invalid'] = 1; core_tbl['X::Comp::Trait::NotOnNative'] = 1; core_tbl['X::Comp::Trait::Scope'] = 1;
core_tbl['X::Comp::Trait::Unknown'] = 1; core_tbl['X::Comp::WheneverOutOfScope'] = 1; core_tbl['X::CompUnit'] = 1; core_tbl['X::CompUnit::UnsatisfiedDependency'] = 1;
core_tbl['X::Composition'] = 1; core_tbl['X::Composition::NotComposable'] = 1; core_tbl['X::Constructor'] = 1; core_tbl['X::Constructor::BadType'] = 1;
core_tbl['X::Constructor::Positional'] = 1; core_tbl['X::Control'] = 1; core_tbl['X::ControlFlow'] = 1; core_tbl['X::ControlFlow::Return'] = 1; core_tbl['X::DateTime'] = 1;
core_tbl['X::DateTime::InvalidDeltaUnit'] = 1; core_tbl['X::DateTime::TimezoneClash'] = 1; core_tbl['X::Declaration'] = 1; core_tbl['X::Declaration::OurScopeInRole'] = 1;
core_tbl['X::Declaration::Scope'] = 1; core_tbl['X::Declaration::Scope::Multi'] = 1; core_tbl['X::Delete'] = 1; core_tbl['X::Does'] = 1; core_tbl['X::Does::TypeObject'] = 1;
core_tbl['X::Dynamic'] = 1; core_tbl['X::Dynamic::NotFound'] = 1; core_tbl['X::Dynamic::Package'] = 1; core_tbl['X::Dynamic::Postdeclaration'] = 1; core_tbl['X::Encoding'] = 1;
core_tbl['X::Encoding::AlreadyRegistered'] = 1; core_tbl['X::Encoding::Unknown'] = 1; core_tbl['X::Enum'] = 1; core_tbl['X::Enum::NoValue'] = 1; core_tbl['X::Eval'] = 1;
core_tbl['X::Eval::NoSuchLang'] = 1; core_tbl['X::Exhausted'] = 1; core_tbl['X::Experimental'] = 1; core_tbl['X::Export'] = 1; core_tbl['X::Export::NameClash'] = 1; core_tbl['X::Hash'] = 1;
core_tbl['X::Hash::Store'] = 1; core_tbl['X::Hash::Store::OddNumber'] = 1; core_tbl['X::HyperOp'] = 1; core_tbl['X::HyperOp::Infinite'] = 1; core_tbl['X::HyperOp::NonDWIM'] = 1;
core_tbl['X::HyperRace'] = 1; core_tbl['X::HyperRace::Died'] = 1; core_tbl['X::HyperWhatever'] = 1; core_tbl['X::HyperWhatever::Multiple'] = 1; core_tbl['X::IO'] = 1;
core_tbl['X::IO::BinaryAndEncoding'] = 1; core_tbl['X::IO::BinaryMode'] = 1; core_tbl['X::IO::Chdir'] = 1; core_tbl['X::IO::Chmod'] = 1; core_tbl['X::IO::Chown'] = 1;
core_tbl['X::IO::Closed'] = 1; core_tbl['X::IO::Copy'] = 1; core_tbl['X::IO::Cwd'] = 1; core_tbl['X::IO::Dir'] = 1; core_tbl['X::IO::Directory'] = 1; core_tbl['X::IO::DoesNotExist'] = 1;
core_tbl['X::IO::Flush'] = 1; core_tbl['X::IO::Link'] = 1; core_tbl['X::IO::Lock'] = 1; core_tbl['X::IO::Mkdir'] = 1; core_tbl['X::IO::Move'] = 1; core_tbl['X::IO::NotAChild'] = 1;
core_tbl['X::IO::NotAFile'] = 1; core_tbl['X::IO::Null'] = 1; core_tbl['X::IO::Rename'] = 1; core_tbl['X::IO::Resolve'] = 1; core_tbl['X::IO::Rmdir'] = 1; core_tbl['X::IO::Symlink'] = 1;
core_tbl['X::IO::Unknown'] = 1; core_tbl['X::IO::Unlink'] = 1; core_tbl['X::IllegalDimensionInShape'] = 1; core_tbl['X::IllegalOnFixedDimensionArray'] = 1; core_tbl['X::Immutable'] = 1;
core_tbl['X::Import'] = 1; core_tbl['X::Import::MissingSymbols'] = 1; core_tbl['X::Import::NoSuchTag'] = 1; core_tbl['X::Import::OnlystarProto'] = 1; core_tbl['X::Import::Positional'] = 1;
core_tbl['X::Import::Redeclaration'] = 1; core_tbl['X::Inheritance'] = 1; core_tbl['X::Inheritance::NotComposed'] = 1; core_tbl['X::Inheritance::SelfInherit'] = 1;
core_tbl['X::Inheritance::UnknownParent'] = 1; core_tbl['X::Inheritance::Unsupported'] = 1; core_tbl['X::Invalid'] = 1; core_tbl['X::Invalid::ComputedValue'] = 1;
core_tbl['X::Invalid::Value'] = 1; core_tbl['X::InvalidCodepoint'] = 1; core_tbl['X::InvalidType'] = 1; core_tbl['X::InvalidTypeSmiley'] = 1; core_tbl['X::Item'] = 1;
core_tbl['X::Language'] = 1; core_tbl['X::Language::IncompatRevisions'] = 1; core_tbl['X::Language::ModRequired'] = 1; core_tbl['X::Language::TooLate'] = 1;
core_tbl['X::Language::Unsupported'] = 1; core_tbl['X::LibEmpty'] = 1; core_tbl['X::LibNone'] = 1; core_tbl['X::Localizer'] = 1; core_tbl['X::Localizer::NoContainer'] = 1;
core_tbl['X::Lock'] = 1; core_tbl['X::Lock::Async'] = 1; core_tbl['X::Lock::Async::NotLocked'] = 1; core_tbl['X::Lock::ConditionVariable'] = 1;
core_tbl['X::Lock::ConditionVariable::Duplicate'] = 1; core_tbl['X::Lock::ConditionVariable::New'] = 1; core_tbl['X::Lock::ConditionVariable::NoMutex'] = 1;
core_tbl['X::Lock::ConditionVariable::WrongThread'] = 1; core_tbl['X::Lock::Unlock'] = 1; core_tbl['X::Lock::Unlock::NoMutex'] = 1; core_tbl['X::Lock::Unlock::WrongThread'] = 1;
core_tbl['X::MOP'] = 1; core_tbl['X::Make'] = 1; core_tbl['X::Make::MatchRequired'] = 1; core_tbl['X::Match'] = 1; core_tbl['X::Match::Bool'] = 1; core_tbl['X::Method'] = 1;
core_tbl['X::Method::Duplicate'] = 1; core_tbl['X::Method::InvalidQualifier'] = 1; core_tbl['X::Method::NotFound'] = 1; core_tbl['X::Method::Private'] = 1;
core_tbl['X::Method::Private::Permission'] = 1; core_tbl['X::Method::Private::Unqualified'] = 1; core_tbl['X::Mixin'] = 1; core_tbl['X::Mixin::NotComposable'] = 1; core_tbl['X::Multi'] = 1;
core_tbl['X::Multi::Ambiguous'] = 1; core_tbl['X::Multi::NoMatch'] = 1; core_tbl['X::MultipleTypeSmiley'] = 1; core_tbl['X::MustBeParametric'] = 1; core_tbl['X::NQP'] = 1;
core_tbl['X::NQP::NotFound'] = 1; core_tbl['X::NYI'] = 1; core_tbl['X::NYI::Available'] = 1; core_tbl['X::NYI::BigInt'] = 1; core_tbl['X::NoCoreRevision'] = 1;
core_tbl['X::NoDispatcher'] = 1; core_tbl['X::NoSuchSymbol'] = 1; core_tbl['X::Nominalizable'] = 1; core_tbl['X::Nominalizable::NoKind'] = 1; core_tbl['X::Nominalizable::NoWrappee'] = 1;
core_tbl['X::NotEnoughDimensions'] = 1; core_tbl['X::NotFoundInRepository'] = 1; core_tbl['X::NotParametric'] = 1; core_tbl['X::Numeric'] = 1; core_tbl['X::Numeric::CannotConvert'] = 1;
core_tbl['X::Numeric::Confused'] = 1; core_tbl['X::Numeric::DivideByZero'] = 1; core_tbl['X::Numeric::Overflow'] = 1; core_tbl['X::Numeric::Real'] = 1; core_tbl['X::Numeric::Underflow'] = 1;
core_tbl['X::Numeric::Uninitialized'] = 1; core_tbl['X::OS'] = 1; core_tbl['X::Obsolete'] = 1; core_tbl['X::OutOfRange'] = 1; core_tbl['X::Package'] = 1; core_tbl['X::Package::Stubbed'] = 1;
core_tbl['X::Package::UseLib'] = 1; core_tbl['X::Pairup'] = 1; core_tbl['X::Pairup::OddNumber'] = 1; core_tbl['X::Parameter'] = 1; core_tbl['X::Parameter::AfterDefault'] = 1;
core_tbl['X::Parameter::BadType'] = 1; core_tbl['X::Parameter::Default'] = 1; core_tbl['X::Parameter::Default::TypeCheck'] = 1; core_tbl['X::Parameter::InvalidConcreteness'] = 1;
core_tbl['X::Parameter::InvalidType'] = 1; core_tbl['X::Parameter::MultipleTypeConstraints'] = 1; core_tbl['X::Parameter::Placeholder'] = 1; core_tbl['X::Parameter::RW'] = 1;
core_tbl['X::Parameter::Twigil'] = 1; core_tbl['X::Parameter::TypedSlurpy'] = 1; core_tbl['X::Parameter::WrongOrder'] = 1; core_tbl['X::ParametricConstant'] = 1; core_tbl['X::Phaser'] = 1;
core_tbl['X::Phaser::Multiple'] = 1; core_tbl['X::Phaser::PrePost'] = 1; core_tbl['X::PhaserExceptions'] = 1; core_tbl['X::Placeholder'] = 1; core_tbl['X::Placeholder::Attribute'] = 1;
core_tbl['X::Placeholder::Block'] = 1; core_tbl['X::Placeholder::Mainline'] = 1; core_tbl['X::Placeholder::NonPlaceholder'] = 1; core_tbl['X::Pod'] = 1; core_tbl['X::PoisonedAlias'] = 1;
core_tbl['X::Pragma'] = 1; core_tbl['X::Pragma::CannotPrecomp'] = 1; core_tbl['X::Pragma::CannotWhat'] = 1; core_tbl['X::Pragma::MustOneOf'] = 1; core_tbl['X::Pragma::NoArgs'] = 1;
core_tbl['X::Pragma::OnlyOne'] = 1; core_tbl['X::Pragma::UnknownArg'] = 1; core_tbl['X::Proc'] = 1; core_tbl['X::Proc::Async'] = 1; core_tbl['X::Proc::Async::AlreadyStarted'] = 1;
core_tbl['X::Proc::Async::BindOrUse'] = 1; core_tbl['X::Proc::Async::CharsOrBytes'] = 1; core_tbl['X::Proc::Async::MustBeStarted'] = 1; core_tbl['X::Proc::Async::OpenForWriting'] = 1;
core_tbl['X::Proc::Async::SupplyOrStd'] = 1; core_tbl['X::Proc::Async::TapBeforeSpawn'] = 1; core_tbl['X::Proc::Unsuccessful'] = 1; core_tbl['X::Promise'] = 1;
core_tbl['X::Promise::Broken'] = 1; core_tbl['X::Promise::CauseOnlyValidOnBroken'] = 1; core_tbl['X::Promise::Combinator'] = 1; core_tbl['X::Promise::Resolved'] = 1;
core_tbl['X::Promise::Vowed'] = 1; core_tbl['X::PseudoPackage'] = 1; core_tbl['X::PseudoPackage::InDeclaration'] = 1; core_tbl['X::Range'] = 1; core_tbl['X::Range::Incomparable'] = 1;
core_tbl['X::Range::InvalidArg'] = 1; core_tbl['X::React'] = 1; core_tbl['X::React::Died'] = 1; core_tbl['X::Redeclaration'] = 1; core_tbl['X::Redeclaration::Outer'] = 1;
core_tbl['X::Role'] = 1; core_tbl['X::Role::Attribute'] = 1; core_tbl['X::Role::Attribute::Conflicts'] = 1; core_tbl['X::Role::Attribute::Exists'] = 1; core_tbl['X::Role::Group'] = 1;
core_tbl['X::Role::Group::Documenting'] = 1; core_tbl['X::Role::Initialization'] = 1; core_tbl['X::Role::Parametric'] = 1; core_tbl['X::Role::Parametric::NoSuchCandidate'] = 1;
core_tbl['X::Role::Unimplemented'] = 1; core_tbl['X::Role::Unimplemented::Multi'] = 1; core_tbl['X::Role::Unresolved'] = 1; core_tbl['X::Role::Unresolved::Method'] = 1;
core_tbl['X::Role::Unresolved::Multi'] = 1; core_tbl['X::Role::Unresolved::Private'] = 1; core_tbl['X::RoleApplier'] = 1; core_tbl['X::RoleApplier::Method'] = 1; core_tbl['X::Routine'] = 1;
core_tbl['X::Routine::Unwrap'] = 1; core_tbl['X::Scheduler'] = 1; core_tbl['X::Scheduler::CueInNaNSeconds'] = 1; core_tbl['X::SecurityPolicy'] = 1; core_tbl['X::SecurityPolicy::Eval'] = 1;
core_tbl['X::Seq'] = 1; core_tbl['X::Seq::Consumed'] = 1; core_tbl['X::Seq::NotIndexable'] = 1; core_tbl['X::Sequence'] = 1; core_tbl['X::Sequence::Deduction'] = 1;
core_tbl['X::Sequence::Endpoint'] = 1; core_tbl['X::Set'] = 1; core_tbl['X::Set::Coerce'] = 1; core_tbl['X::Signature'] = 1; core_tbl['X::Signature::NameClash'] = 1;
core_tbl['X::Signature::Placeholder'] = 1; core_tbl['X::Str'] = 1; core_tbl['X::Str::InvalidCharName'] = 1; core_tbl['X::Str::Match'] = 1; core_tbl['X::Str::Match::x'] = 1;
core_tbl['X::Str::Numeric'] = 1; core_tbl['X::Str::Sprintf'] = 1; core_tbl['X::Str::Sprintf::Directives'] = 1; core_tbl['X::Str::Sprintf::Directives::BadType'] = 1;
core_tbl['X::Str::Sprintf::Directives::Count'] = 1; core_tbl['X::Str::Sprintf::Directives::Unsupported'] = 1; core_tbl['X::Str::Subst'] = 1; core_tbl['X::Str::Subst::Adverb'] = 1;
core_tbl['X::Str::Trans'] = 1; core_tbl['X::Str::Trans::IllegalKey'] = 1; core_tbl['X::Str::Trans::InvalidArg'] = 1; core_tbl['X::StubCode'] = 1; core_tbl['X::Subscript'] = 1;
core_tbl['X::Subscript::Negative'] = 1; core_tbl['X::Supply'] = 1; core_tbl['X::Supply::Combinator'] = 1; core_tbl['X::Supply::Migrate'] = 1; core_tbl['X::Supply::Migrate::Needs'] = 1;
core_tbl['X::Supply::New'] = 1; core_tbl['X::Syntax'] = 1; core_tbl['X::Syntax::AddCategorical'] = 1; core_tbl['X::Syntax::AddCategorical::TooFewParts'] = 1;
core_tbl['X::Syntax::AddCategorical::TooManyParts'] = 1; core_tbl['X::Syntax::Adverb'] = 1; core_tbl['X::Syntax::Argument'] = 1; core_tbl['X::Syntax::Argument::MOPMacro'] = 1;
core_tbl['X::Syntax::Augment'] = 1; core_tbl['X::Syntax::Augment::Adverb'] = 1; core_tbl['X::Syntax::Augment::Illegal'] = 1; core_tbl['X::Syntax::Augment::WithoutMonkeyTyping'] = 1;
core_tbl['X::Syntax::BlockGobbled'] = 1; core_tbl['X::Syntax::CannotMeta'] = 1; core_tbl['X::Syntax::Coercer'] = 1; core_tbl['X::Syntax::Coercer::TooComplex'] = 1;
core_tbl['X::Syntax::Comment'] = 1; core_tbl['X::Syntax::Comment::Embedded'] = 1; core_tbl['X::Syntax::ConditionalOperator'] = 1;
core_tbl['X::Syntax::ConditionalOperator::PrecedenceTooLoose'] = 1; core_tbl['X::Syntax::ConditionalOperator::SecondPartGobbled'] = 1;
core_tbl['X::Syntax::ConditionalOperator::SecondPartInvalid'] = 1; core_tbl['X::Syntax::Confused'] = 1; core_tbl['X::Syntax::DuplicatedPrefix'] = 1; core_tbl['X::Syntax::Extension'] = 1;
core_tbl['X::Syntax::Extension::Category'] = 1; core_tbl['X::Syntax::Extension::Null'] = 1; core_tbl['X::Syntax::Extension::SpecialForm'] = 1;
core_tbl['X::Syntax::Extension::TooComplex'] = 1; core_tbl['X::Syntax::InfixInTermPosition'] = 1; core_tbl['X::Syntax::KeywordAsFunction'] = 1; core_tbl['X::Syntax::Malformed'] = 1;
core_tbl['X::Syntax::Malformed::Elsif'] = 1; core_tbl['X::Syntax::Missing'] = 1; core_tbl['X::Syntax::Name'] = 1; core_tbl['X::Syntax::Name::Null'] = 1;
core_tbl['X::Syntax::NegatedPair'] = 1; core_tbl['X::Syntax::NoSelf'] = 1; core_tbl['X::Syntax::NonAssociative'] = 1; core_tbl['X::Syntax::NonListAssociative'] = 1;
core_tbl['X::Syntax::Number'] = 1; core_tbl['X::Syntax::Number::IllegalDecimal'] = 1; core_tbl['X::Syntax::Number::LiteralType'] = 1; core_tbl['X::Syntax::Number::RadixOutOfRange'] = 1;
core_tbl['X::Syntax::P5'] = 1; core_tbl['X::Syntax::ParentAsHash'] = 1; core_tbl['X::Syntax::Perl5Var'] = 1; core_tbl['X::Syntax::Pod'] = 1; core_tbl['X::Syntax::Pod::BeginWithoutEnd'] = 1;
core_tbl['X::Syntax::Pod::BeginWithoutIdentifier'] = 1; core_tbl['X::Syntax::Pod::DeclaratorLeading'] = 1; core_tbl['X::Syntax::Pod::DeclaratorTrailing'] = 1;
core_tbl['X::Syntax::Regex'] = 1; core_tbl['X::Syntax::Regex::Adverb'] = 1; core_tbl['X::Syntax::Regex::Alias'] = 1; core_tbl['X::Syntax::Regex::Alias::LongName'] = 1;
core_tbl['X::Syntax::Regex::MalformedRange'] = 1; core_tbl['X::Syntax::Regex::NonQuantifiable'] = 1; core_tbl['X::Syntax::Regex::NullRegex'] = 1;
core_tbl['X::Syntax::Regex::QuantifierValue'] = 1; core_tbl['X::Syntax::Regex::SolitaryBacktrackControl'] = 1; core_tbl['X::Syntax::Regex::SolitaryQuantifier'] = 1;
core_tbl['X::Syntax::Regex::SpacesInBareRange'] = 1; core_tbl['X::Syntax::Regex::UnrecognizedMetachar'] = 1; core_tbl['X::Syntax::Regex::UnrecognizedModifier'] = 1;
core_tbl['X::Syntax::Regex::Unspace'] = 1; core_tbl['X::Syntax::Regex::Unterminated'] = 1; core_tbl['X::Syntax::Reserved'] = 1; core_tbl['X::Syntax::Self'] = 1;
core_tbl['X::Syntax::Self::WithoutObject'] = 1; core_tbl['X::Syntax::Signature'] = 1; core_tbl['X::Syntax::Signature::InvocantMarker'] = 1;
core_tbl['X::Syntax::Signature::InvocantNotAllowed'] = 1; core_tbl['X::Syntax::\124erm'] = 1; core_tbl['X::Syntax::\124erm::MissingInitializer'] = 1; core_tbl['X::Syntax::Type'] = 1;
core_tbl['X::Syntax::Type::Adverb'] = 1; core_tbl['X::Syntax::UnlessElse'] = 1; core_tbl['X::Syntax::Variable'] = 1; core_tbl['X::Syntax::Variable::BadType'] = 1;
core_tbl['X::Syntax::Variable::ConflictingTypes'] = 1; core_tbl['X::Syntax::Variable::IndirectDeclaration'] = 1; core_tbl['X::Syntax::Variable::Initializer'] = 1;
core_tbl['X::Syntax::Variable::Match'] = 1; core_tbl['X::Syntax::Variable::MissingInitializer'] = 1; core_tbl['X::Syntax::Variable::Numeric'] = 1;
core_tbl['X::Syntax::Variable::SignatureAssignment'] = 1; core_tbl['X::Syntax::Variable::SignatureWithoutInitializer'] = 1; core_tbl['X::Syntax::Variable::Twigil'] = 1;
core_tbl['X::Syntax::VirtualCall'] = 1; core_tbl['X::Syntax::WithoutElse'] = 1; core_tbl['X::Temporal'] = 1; core_tbl['X::Temporal::InvalidFormat'] = 1; core_tbl['X::TooLateForREPR'] = 1;
core_tbl['X::TooManyDimensions'] = 1; core_tbl['X::Trait'] = 1; core_tbl['X::Trait::Invalid'] = 1; core_tbl['X::Trait::NotOnNative'] = 1; core_tbl['X::Trait::Scope'] = 1;
core_tbl['X::Trait::Unknown'] = 1; core_tbl['X::TypeCheck'] = 1; core_tbl['X::TypeCheck::Argument'] = 1; core_tbl['X::TypeCheck::Assignment'] = 1; core_tbl['X::TypeCheck::Attribute'] = 1;
core_tbl['X::TypeCheck::Attribute::Default'] = 1; core_tbl['X::TypeCheck::Binding'] = 1; core_tbl['X::TypeCheck::Binding::Parameter'] = 1; core_tbl['X::TypeCheck::Return'] = 1;
core_tbl['X::TypeCheck::Splice'] = 1; core_tbl['X::Undeclared'] = 1; core_tbl['X::Undeclared::Symbols'] = 1; core_tbl['X::UnitScope'] = 1; core_tbl['X::UnitScope::Invalid'] = 1;
core_tbl['X::UnitScope::TooLate'] = 1; core_tbl['X::Value'] = 1; core_tbl['X::Value::Dynamic'] = 1; core_tbl['X::WheneverOutOfScope'] = 1; core_tbl['X::Worry'] = 1;
core_tbl['X::Worry::P5'] = 1; core_tbl['X::Worry::P5::BackReference'] = 1; core_tbl['X::Worry::P5::LeadingZero'] = 1; core_tbl['X::Worry::P5::Reference'] = 1;
core_tbl['X::Worry::Precedence'] = 1; core_tbl['X::Worry::Precedence::Range'] = 1; core_tbl['array'] = 1; core_tbl['array::intarray'] = 1; core_tbl['array::numarray'] = 1;
core_tbl['array::shaped1intarray'] = 1; core_tbl['array::shaped1numarray'] = 1; core_tbl['array::shaped1strarray'] = 1; core_tbl['array::shaped1uintarray'] = 1;
core_tbl['array::shaped2intarray'] = 1; core_tbl['array::shaped2numarray'] = 1; core_tbl['array::shaped2strarray'] = 1; core_tbl['array::shaped2uintarray'] = 1;
core_tbl['array::shaped3intarray'] = 1; core_tbl['array::shaped3numarray'] = 1; core_tbl['array::shaped3strarray'] = 1; core_tbl['array::shaped3uintarray'] = 1;
core_tbl['array::shapedarray'] = 1; core_tbl['array::shapedintarray'] = 1; core_tbl['array::shapednumarray'] = 1; core_tbl['array::shapedstrarray'] = 1;
core_tbl['array::shapeduintarray'] = 1; core_tbl['array::strarray'] = 1; core_tbl['array::typedim2role'] = 1; core_tbl['array::uintarray'] = 1; core_tbl['atomicint'] = 1;
core_tbl['blob16'] = 1; core_tbl['blob32'] = 1; core_tbl['blob64'] = 1; core_tbl['blob8'] = 1; core_tbl['buf16'] = 1; core_tbl['buf32'] = 1; core_tbl['buf64'] = 1; core_tbl['buf8'] = 1;
core_tbl['byte'] = 1; core_tbl['e'] = 1; core_tbl['i'] = 1; core_tbl['int'] = 1; core_tbl['int16'] = 1; core_tbl['int32'] = 1; core_tbl['int64'] = 1; core_tbl['int8'] = 1;
core_tbl['num'] = 1; core_tbl['num32'] = 1; core_tbl['num64'] = 1; core_tbl['pi'] = 1; core_tbl['str'] = 1; core_tbl['tau'] = 1; core_tbl['uint'] = 1; core_tbl['uint16'] = 1;
core_tbl['uint32'] = 1; core_tbl['uint64'] = 1; core_tbl['uint8'] = 1; core_tbl['utf16'] = 1; core_tbl['utf32'] = 1; core_tbl['utf8'] = 1; core_tbl['π'] = 1; core_tbl['τ'] = 1;
core_tbl['𝑒'] = 1; core_tbl['EVAL'] = 1; core_tbl['EVALFILE'] = 1; core_tbl['HOW'] = 1; core_tbl['NYI'] = 1; core_tbl['RUN-MAIN'] = 1; core_tbl['VAR'] = 1; core_tbl['WHAT'] = 1;
core_tbl['abs'] = 1; core_tbl['acos'] = 1; core_tbl['acosec'] = 1; core_tbl['acosech'] = 1; core_tbl['acosh'] = 1; core_tbl['acotan'] = 1; core_tbl['acotanh'] = 1; core_tbl['all'] = 1;
core_tbl['any'] = 1; core_tbl['append'] = 1; core_tbl['asec'] = 1; core_tbl['asech'] = 1; core_tbl['asin'] = 1; core_tbl['asinh'] = 1; core_tbl['atan'] = 1; core_tbl['atan2'] = 1;
core_tbl['atanh'] = 1; core_tbl['atomic-add-fetch'] = 1; core_tbl['atomic-assign'] = 1; core_tbl['atomic-dec-fetch'] = 1; core_tbl['atomic-fetch'] = 1; core_tbl['atomic-fetch-add'] = 1;
core_tbl['atomic-fetch-dec'] = 1; core_tbl['atomic-fetch-inc'] = 1; core_tbl['atomic-fetch-sub'] = 1; core_tbl['atomic-inc-fetch'] = 1; core_tbl['atomic-sub-fetch'] = 1;
core_tbl['await'] = 1; core_tbl['bag'] = 1; core_tbl['cache'] = 1; core_tbl['callframe'] = 1; core_tbl['callsame'] = 1; core_tbl['callwith'] = 1; core_tbl['cas'] = 1;
core_tbl['categorize'] = 1; core_tbl['ceiling'] = 1; core_tbl['chars'] = 1; core_tbl['chdir'] = 1; core_tbl['chmod'] = 1; core_tbl['chomp'] = 1; core_tbl['chop'] = 1; core_tbl['chown'] = 1;
core_tbl['chr'] = 1; core_tbl['chrs'] = 1; core_tbl['circumfix:<:{ }>'] = 1; core_tbl['circumfix:<[ ]>'] = 1; core_tbl['circumfix:<{ }>'] = 1; core_tbl['cis'] = 1; core_tbl['classify'] = 1;
core_tbl['close'] = 1; core_tbl['comb'] = 1; core_tbl['combinations'] = 1; core_tbl['copy'] = 1; core_tbl['cos'] = 1; core_tbl['cosec'] = 1; core_tbl['cosech'] = 1; core_tbl['cosh'] = 1;
core_tbl['cotan'] = 1; core_tbl['cotanh'] = 1; core_tbl['cross'] = 1; core_tbl['deepmap'] = 1; core_tbl['defined'] = 1; core_tbl['die'] = 1; core_tbl['dir'] = 1; core_tbl['done'] = 1;
core_tbl['duckmap'] = 1; core_tbl['elems'] = 1; core_tbl['emit'] = 1; core_tbl['end'] = 1; core_tbl['exit'] = 1; core_tbl['exp'] = 1; core_tbl['expmod'] = 1; core_tbl['fail'] = 1;
core_tbl['fc'] = 1; core_tbl['first'] = 1; core_tbl['flat'] = 1; core_tbl['flip'] = 1; core_tbl['floor'] = 1; core_tbl['full-barrier'] = 1; core_tbl['get'] = 1; core_tbl['getc'] = 1;
core_tbl['gist'] = 1; core_tbl['goto'] = 1; core_tbl['grep'] = 1; core_tbl['hash'] = 1; core_tbl['head'] = 1; core_tbl['index'] = 1; core_tbl['indices'] = 1; core_tbl['indir'] = 1;
core_tbl['infix:<!=>'] = 1; core_tbl['infix:<!~~>'] = 1; core_tbl['infix:<%%>'] = 1; core_tbl['infix:<%>'] = 1; core_tbl['infix:<&&>'] = 1; core_tbl['infix:<&>'] = 1;
core_tbl['infix:<(&)>'] = 1; core_tbl['infix:<(+)>'] = 1; core_tbl['infix:<(-)>'] = 1; core_tbl['infix:<(.)>'] = 1; core_tbl['infix:<(==)>'] = 1; core_tbl['infix:<(^)>'] = 1;
core_tbl['infix:<(cont)>'] = 1; core_tbl['infix:<(elem)>'] = 1; core_tbl['infix:<(|)>'] = 1; core_tbl['infix:<**>'] = 1; core_tbl['infix:<*>'] = 1; core_tbl['infix:<+&>'] = 1;
core_tbl['infix:<+>'] = 1; core_tbl['infix:<+^>'] = 1; core_tbl['infix:<+|>'] = 1; core_tbl['infix:<,>'] = 1; core_tbl['infix:<->'] = 1; core_tbl['infix:<...>'] = 1;
core_tbl['infix:<...^>'] = 1; core_tbl['infix:<..>'] = 1; core_tbl['infix:<..^>'] = 1; core_tbl['infix:<//>'] = 1; core_tbl['infix:</>'] = 1; core_tbl['infix:<=:=>'] = 1;
core_tbl['infix:<===>'] = 1; core_tbl['infix:<==>'] = 1; core_tbl['infix:<=>'] = 1; core_tbl['infix:<=~=>'] = 1; core_tbl['infix:<=~>'] = 1; core_tbl['infix:<?&>'] = 1;
core_tbl['infix:<?^>'] = 1; core_tbl['infix:<?|>'] = 1; core_tbl['infix:<X>'] = 1; core_tbl['infix:<Z>'] = 1; core_tbl['infix:<^...>'] = 1; core_tbl['infix:<^...^>'] = 1;
core_tbl['infix:<^..>'] = 1; core_tbl['infix:<^..^>'] = 1; core_tbl['infix:<^>'] = 1; core_tbl['infix:<^^>'] = 1; core_tbl['infix:<^…>'] = 1; core_tbl['infix:<^…^>'] = 1;
core_tbl['infix:<after>'] = 1; core_tbl['infix:<and>'] = 1; core_tbl['infix:<andthen>'] = 1; core_tbl['infix:<before>'] = 1; core_tbl['infix:<but>'] = 1; core_tbl['infix:<cmp>'] = 1;
core_tbl['infix:<coll>'] = 1; core_tbl['infix:<div>'] = 1; core_tbl['infix:<does>'] = 1; core_tbl['infix:<eq>'] = 1; core_tbl['infix:<eqv>'] = 1; core_tbl['infix:<gcd>'] = 1;
core_tbl['infix:<ge>'] = 1; core_tbl['infix:<gt>'] = 1; core_tbl['infix:<lcm>'] = 1; core_tbl['infix:<le>'] = 1; core_tbl['infix:<leg>'] = 1; core_tbl['infix:<lt>'] = 1;
core_tbl['infix:<max>'] = 1; core_tbl['infix:<min>'] = 1; core_tbl['infix:<minmax>'] = 1; core_tbl['infix:<mod>'] = 1; core_tbl['infix:<ne>'] = 1; core_tbl['infix:<notandthen>'] = 1;
core_tbl['infix:<o>'] = 1; core_tbl['infix:<or>'] = 1; core_tbl['infix:<orelse>'] = 1; core_tbl['infix:<unicmp>'] = 1; core_tbl['infix:<x>'] = 1; core_tbl['infix:<xor>'] = 1;
core_tbl['infix:<xx>'] = 1; core_tbl['infix:<|>'] = 1; core_tbl['infix:<||>'] = 1; core_tbl['infix:<~&>'] = 1; core_tbl['infix:<~>'] = 1; core_tbl['infix:<~^>'] = 1;
core_tbl['infix:<~|>'] = 1; core_tbl['infix:<~~>'] = 1; core_tbl['infix:<×>'] = 1; core_tbl['infix:<÷>'] = 1; core_tbl['infix:<…>'] = 1; core_tbl['infix:<…^>'] = 1;
core_tbl['infix:<∈>'] = 1; core_tbl['infix:<∉>'] = 1; core_tbl['infix:<∊>'] = 1; core_tbl['infix:<∋>'] = 1; core_tbl['infix:<∌>'] = 1; core_tbl['infix:<∍>'] = 1; core_tbl['infix:<−>'] = 1;
core_tbl['infix:<∖>'] = 1; core_tbl['infix:<∘>'] = 1; core_tbl['infix:<∩>'] = 1; core_tbl['infix:<∪>'] = 1; core_tbl['infix:<≅>'] = 1; core_tbl['infix:<≠>'] = 1; core_tbl['infix:<≡>'] = 1;
core_tbl['infix:<≢>'] = 1; core_tbl['infix:<≤>'] = 1; core_tbl['infix:<≥>'] = 1; core_tbl['infix:<≼>'] = 1; core_tbl['infix:<≽>'] = 1; core_tbl['infix:<⊂>'] = 1; core_tbl['infix:<⊃>'] = 1;
core_tbl['infix:<⊄>'] = 1; core_tbl['infix:<⊅>'] = 1; core_tbl['infix:<⊆>'] = 1; core_tbl['infix:<⊇>'] = 1; core_tbl['infix:<⊈>'] = 1; core_tbl['infix:<⊉>'] = 1; core_tbl['infix:<⊍>'] = 1;
core_tbl['infix:<⊎>'] = 1; core_tbl['infix:<⊖>'] = 1; core_tbl['infix:<⚛+=>'] = 1; core_tbl['infix:<⚛-=>'] = 1; core_tbl['infix:<⚛=>'] = 1; core_tbl['infix:<⚛−=>'] = 1;
core_tbl['infix:<⩵>'] = 1; core_tbl['infix:<⩶>'] = 1; core_tbl['infix:«(<)»'] = 1; core_tbl['infix:«(<+)»'] = 1; core_tbl['infix:«(<=)»'] = 1; core_tbl['infix:«(>)»'] = 1;
core_tbl['infix:«(>+)»'] = 1; core_tbl['infix:«(>=)»'] = 1; core_tbl['infix:«+<»'] = 1; core_tbl['infix:«+>»'] = 1; core_tbl['infix:«<=>»'] = 1; core_tbl['infix:«<=»'] = 1;
core_tbl['infix:«<»'] = 1; core_tbl['infix:«=>»'] = 1; core_tbl['infix:«>=»'] = 1; core_tbl['infix:«>»'] = 1; core_tbl['infix:«~<»'] = 1; core_tbl['infix:«~>»'] = 1;
core_tbl['is-prime'] = 1; core_tbl['item'] = 1; core_tbl['join'] = 1; core_tbl['keys'] = 1; core_tbl['kv'] = 1; core_tbl['last'] = 1; core_tbl['lastcall'] = 1; core_tbl['lc'] = 1;
core_tbl['leave'] = 1; core_tbl['lines'] = 1; core_tbl['link'] = 1; core_tbl['list'] = 1; core_tbl['log'] = 1; core_tbl['log10'] = 1; core_tbl['log2'] = 1; core_tbl['lsb'] = 1;
core_tbl['make'] = 1; core_tbl['map'] = 1; core_tbl['max'] = 1; core_tbl['min'] = 1; core_tbl['minmax'] = 1; core_tbl['mix'] = 1; core_tbl['mkdir'] = 1; core_tbl['move'] = 1;
core_tbl['msb'] = 1; core_tbl['next'] = 1; core_tbl['nextcallee'] = 1; core_tbl['nextsame'] = 1; core_tbl['nextwith'] = 1; core_tbl['nodemap'] = 1; core_tbl['none'] = 1; core_tbl['not'] = 1;
core_tbl['note'] = 1; core_tbl['one'] = 1; core_tbl['open'] = 1; core_tbl['ord'] = 1; core_tbl['ords'] = 1; core_tbl['pair'] = 1; core_tbl['pairs'] = 1; core_tbl['parse-base'] = 1;
core_tbl['parse-names'] = 1; core_tbl['permutations'] = 1; core_tbl['pick'] = 1; core_tbl['pop'] = 1; core_tbl['postcircumfix:<[ ]>'] = 1; core_tbl['postcircumfix:<[; ]>'] = 1;
core_tbl['postcircumfix:<{ }>'] = 1; core_tbl['postcircumfix:<{; }>'] = 1; core_tbl['postfix:<++>'] = 1; core_tbl['postfix:<-->'] = 1; core_tbl['postfix:<i>'] = 1;
core_tbl['postfix:<ⁿ>'] = 1; core_tbl['postfix:<⚛++>'] = 1; core_tbl['postfix:<⚛-->'] = 1; core_tbl['prefix:<!>'] = 1; core_tbl['prefix:<++>'] = 1; core_tbl['prefix:<++⚛>'] = 1;
core_tbl['prefix:<+>'] = 1; core_tbl['prefix:<+^>'] = 1; core_tbl['prefix:<-->'] = 1; core_tbl['prefix:<--⚛>'] = 1; core_tbl['prefix:<->'] = 1; core_tbl['prefix:<?>'] = 1;
core_tbl['prefix:<?^>'] = 1; core_tbl['prefix:<^>'] = 1; core_tbl['prefix:<let>'] = 1; core_tbl['prefix:<not>'] = 1; core_tbl['prefix:<so>'] = 1; core_tbl['prefix:<temp>'] = 1;
core_tbl['prefix:<|>'] = 1; core_tbl['prefix:<~>'] = 1; core_tbl['prefix:<~^>'] = 1; core_tbl['prefix:<−>'] = 1; core_tbl['prefix:<⚛>'] = 1; core_tbl['prepend'] = 1; core_tbl['print'] = 1;
core_tbl['printf'] = 1; core_tbl['proceed'] = 1; core_tbl['produce'] = 1; core_tbl['prompt'] = 1; core_tbl['push'] = 1; core_tbl['put'] = 1; core_tbl['rand'] = 1; core_tbl['redo'] = 1;
core_tbl['reduce'] = 1; core_tbl['rename'] = 1; core_tbl['repeated'] = 1; core_tbl['repl'] = 1; core_tbl['return'] = 1; core_tbl['return-rw'] = 1; core_tbl['reverse'] = 1;
core_tbl['rindex'] = 1; core_tbl['rmdir'] = 1; core_tbl['roll'] = 1; core_tbl['roots'] = 1; core_tbl['rotate'] = 1; core_tbl['round'] = 1; core_tbl['roundrobin'] = 1; core_tbl['run'] = 1;
core_tbl['samecase'] = 1; core_tbl['samemark'] = 1; core_tbl['samewith'] = 1; core_tbl['say'] = 1; core_tbl['sec'] = 1; core_tbl['sech'] = 1; core_tbl['set'] = 1; core_tbl['shell'] = 1;
core_tbl['shift'] = 1; core_tbl['sign'] = 1; core_tbl['signal'] = 1; core_tbl['sin'] = 1; core_tbl['sinh'] = 1; core_tbl['skip'] = 1; core_tbl['sleep'] = 1; core_tbl['sleep-timer'] = 1;
core_tbl['sleep-until'] = 1; core_tbl['slip'] = 1; core_tbl['slurp'] = 1; core_tbl['so'] = 1; core_tbl['sort'] = 1; core_tbl['splice'] = 1; core_tbl['split'] = 1; core_tbl['sprintf'] = 1;
core_tbl['spurt'] = 1; core_tbl['sqrt'] = 1; core_tbl['squish'] = 1; core_tbl['srand'] = 1; core_tbl['subbuf-rw'] = 1; core_tbl['substr'] = 1; core_tbl['substr-rw'] = 1;
core_tbl['succeed'] = 1; core_tbl['sum'] = 1; core_tbl['symlink'] = 1; core_tbl['tail'] = 1; core_tbl['take'] = 1; core_tbl['take-rw'] = 1; core_tbl['tan'] = 1; core_tbl['tanh'] = 1;
core_tbl['tc'] = 1; core_tbl['tclc'] = 1; core_tbl['term:<now>'] = 1; core_tbl['term:<time>'] = 1; core_tbl['trait_mod:<does>'] = 1; core_tbl['trait_mod:<handles>'] = 1;
core_tbl['trait_mod:<hides>'] = 1; core_tbl['trait_mod:<is>'] = 1; core_tbl['trait_mod:<of>'] = 1; core_tbl['trait_mod:<returns>'] = 1; core_tbl['trait_mod:<trusts>'] = 1;
core_tbl['trait_mod:<will>'] = 1; core_tbl['trim'] = 1; core_tbl['trim-leading'] = 1; core_tbl['trim-trailing'] = 1; core_tbl['truncate'] = 1; core_tbl['uc'] = 1; core_tbl['undefine'] = 1;
core_tbl['unimatch'] = 1; core_tbl['uniname'] = 1; core_tbl['uninames'] = 1; core_tbl['uniparse'] = 1; core_tbl['uniprop'] = 1; core_tbl['uniprops'] = 1; core_tbl['unique'] = 1;
core_tbl['unival'] = 1; core_tbl['univals'] = 1; core_tbl['unlink'] = 1; core_tbl['unpolar'] = 1; core_tbl['unshift'] = 1; core_tbl['val'] = 1; core_tbl['values'] = 1; core_tbl['warn'] = 1;
core_tbl['wordcase'] = 1; core_tbl['words'] = 1; core_tbl['zip'] = 1; core_tbl['prefix:<//>'] = 1; core_tbl['rotor'] = 1; core_tbl['snip'] = 1; core_tbl['snitch'] = 1;
core_tbl['term:<nano>'] = 1; core_tbl['MONKEY-SEE-NO-EVAL'] = 1; core_tbl['bail-out'] = 1; core_tbl['can-ok'] = 1; core_tbl['cmp-ok'] = 1; core_tbl['diag'] = 1; core_tbl['dies-ok'] = 1;
core_tbl['does-ok'] = 1; core_tbl['done-testing'] = 1; core_tbl['eval-dies-ok'] = 1; core_tbl['eval-lives-ok'] = 1; core_tbl['fails-like'] = 1; core_tbl['flunk'] = 1; core_tbl['is'] = 1;
core_tbl['is-approx'] = 1; core_tbl['is-deeply'] = 1; core_tbl['is_approx'] = 1; core_tbl['isa-ok'] = 1; core_tbl['isnt'] = 1; core_tbl['like'] = 1; core_tbl['lives-ok'] = 1;
core_tbl['nok'] = 1; core_tbl['ok'] = 1; core_tbl['pass'] = 1; core_tbl['plan'] = 1; core_tbl['skip'] = 1; core_tbl['skip-rest'] = 1; core_tbl['subtest'] = 1; core_tbl['throws-like'] = 1;
core_tbl['todo'] = 1; core_tbl['trait_mod:<is>'] = 1; core_tbl['unlike'] = 1; core_tbl['use-ok'] = 1;
typeobj = TABLE();
bare_nm = TABLE();
notcore_tbl = TABLE(); notcore_tbl['True'] = 1; notcore_tbl['False'] = 1; notcore_tbl['Less'] = 1; notcore_tbl['Same'] = 1; notcore_tbl['More'] = 1; notcore_tbl['Inf'] = 1; notcore_tbl['NaN'] = 1; notcore_tbl['Empty'] = 1; notcore_tbl['Nil'] = 1;
function IsCore(nm) { if (IDENT(core_tbl[nm])) { freturn; } if (DIFFER(notcore_tbl[nm])) { freturn; } if (~(SUBSTR(nm, 1, 1) ? ANY(&UCASE))) { freturn; } if (nm ? ':') { freturn; } return; }
function BareKey(nm) { Sh('TT_VAR', nm); bare_nm[Top()] = 1; BareKey = .dummy; nreturn; }
function SetBare() { no_sub = 1; SetBare = ''; return; }
function ClearBare() { no_sub = 0; ClearBare = ''; return; }
function MaySub() { if (EQ(no_sub, 1)) { freturn; } return; }
function Bare(nm) { if ((IDENT(nm, 'True'), IDENT(nm, 'False'))) { MkBool((IDENT(nm, 'True') 1, 0)); Bare = .dummy; nreturn; } if (IsCore(nm)) { Push(mk('TT_VAR', '__rk_typeobj')); Push(mk('TT_QLIT', nm)); Reduce('TT_FNC', 2, '__rk_typeobj'); typeobj[Top()] = nm; Bare = .dummy; nreturn; } if ((IDENT(nm, 'True'), IDENT(nm, 'False'))) { Push(mk('TT_VAR', '__rk_mkbool')); Push(mk('TT_ILIT', (IDENT(nm, 'True') 1, 0))); Reduce('TT_FNC', 2, '__rk_mkbool'); Bare = .dummy; nreturn; } Sh('TT_VAR', nm); bare_nm[Top()] = 1; Bare = .dummy; nreturn; }
rename_tbl = TABLE();
rename_tbl['plan'] = '__rk_test_plan'; rename_tbl['ok'] = '__rk_test_ok'; rename_tbl['is-approx'] = '__rk_test_is_approx'; rename_tbl['exit'] = '__rk_exit'; rename_tbl['flat'] = '__rk_arr';
function nFlatP(n, c, i, k, m) { m = 0; i = 0; while (LT(i, n)) { i = i + 1; k = c[i]; m = m + (IsPair(k) 2, 1); } nFlatP = m; return; }
function PushFlatP(n, c, i, k) { i = 0; while (LT(i, n)) { i = i + 1; k = c[i]; if (IsPair(k)) { Push(c(k)[2]); Push(c(k)[3]); } else { Push(k); } } return; }
function nFlat(n, c, i, k, m) { m = 0; i = 0; while (LT(i, n)) { i = i + 1; k = c[i]; m = m + (IsNamed(k) 2, 1); } nFlat = m; return; }
function PushFlat(n, c, i, k) { i = 0; while (LT(i, n)) { i = i + 1; k = c[i]; if (IsNamed(k)) { Push(c(k)[2]); Push(c(k)[3]); } else { Push(ArgVal(k)); } } return; }
function MethCall(n, k, i, c, inv, nm) {
    c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; }
    nm = Pop(); inv = Pop();
    if (IDENT(v(nm), 'bless')) { Push(inv); Push(nm); PushNamed(n, c); Reduce('TT_METHCALL', nFlat0(n, c) + 2); MethCall = .dummy; nreturn; }
    if (IDENT(v(nm), 'new') IDENT(t(inv), 'TT_VAR') IDENT(v(inv), 'X::AdHoc')) { AdHoc(n, c); MethCall = .dummy; nreturn; }
    if (IDENT(v(nm), 'new') DIFFER(typeobj[inv])) { Push(mk('TT_QLIT', typeobj[inv])); PushNamed(n, c); Reduce('TT_NEW', nFlat0(n, c) + 1); MethCall = .dummy; nreturn; }
    if (IDENT(v(nm), 'new') IDENT(t(inv), 'TT_VAR') DIFFER(bare_nm[inv])) { Push(mk('TT_QLIT', v(inv))); PushNamed(n, c); Reduce('TT_NEW', nFlat0(n, c) + 1); MethCall = .dummy; nreturn; }
    Push(inv); Push(nm); PushNamed(n, c); Reduce('TT_METHCALL', nFlat0(n, c) + 2); MethCall = .dummy; nreturn;
}
function IsPair(k) { if (IDENT(t(k), 'TT_FNC') IDENT(v(k), '__rk_pair')) { return; } freturn; }
function IsNamed(k) { if (DIFFER(named_pair[k])) { return; } freturn; }
function ArgVal(k) { if (DIFFER(adverb_val[k])) { ArgVal = c(k)[3]; return; } ArgVal = k; return; }
function nFlat0(n, c, i, m, run) { m = 0; run = 1; i = 0; while (LT(i, n)) { i = i + 1; if (IsNamed(c[i]) EQ(run, 1)) { m = m + 2; } else { run = 0; m = m + 1; } } nFlat0 = m; return; }
function PushNamed(n, c, i, run) { run = 1; i = 0; while (LT(i, n)) { i = i + 1; if (IsNamed(c[i]) EQ(run, 1)) { ; } else { run = 0; Push(ArgVal(c[i])); } } run = 1; i = 0; while (LT(i, n)) { i = i + 1; if (IsNamed(c[i]) EQ(run, 1)) { Push(c(c[i])[2]); Push(c(c[i])[3]); } else { run = 0; } } return; }
function AdHoc(n, c, i, k) { i = 0; while (LT(i, n)) { i = i + 1; k = c[i]; if (IsPair(k) (IDENT(v(c(k)[2]), 'payload'), IDENT(v(c(k)[2]), 'message'))) { Push(c(k)[3]); return; } } i = 0; while (LT(i, n)) { i = i + 1; if (~IsPair(c[i])) { Push(c[i]); return; } } Push(mk('TT_QLIT', '')); return; }
function Assign(r, l) {
    ValPost(); r = Pop(); l = Pop();
    if (IDENT(t(l), 'TT_ARR_GET'))  { Push(c(l)[1]); Push(c(l)[2]); Push(r); Reduce('TT_ARR_SET', 3); Assign = .dummy; nreturn; }
    if (IDENT(t(l), 'TT_HASH_GET')) { Push(c(l)[1]); Push(c(l)[2]); Push(r); Reduce('TT_HASH_SET', 3); Assign = .dummy; nreturn; }
    if (IDENT(t(l), 'TT_METHCALL') EQ(n(l), 2)) { Push(c(l)[1]); Reduce('TT_FIELD', 1, v(c(l)[2])); Push(r); Reduce('TT_ASSIGN', 2); Assign = .dummy; nreturn; }
    Push(l); ListRhs(r, l); Reduce('TT_ASSIGN', 2); Assign = .dummy; nreturn;
}
function DotAssign(n, i, c, nm, l) { c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } nm = Pop(); l = Pop(); Push(l); Push(l); Push(nm); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_METHCALL', n + 2); Reduce('TT_ASSIGN', 2); DotAssign = .dummy; nreturn; }
function AssignOp(k, r, l, st) { r = Pop(); l = Pop(); st = (IDENT(t(l), 'TT_ARR_GET') 'TT_ARR_SET', IDENT(t(l), 'TT_HASH_GET') 'TT_HASH_SET', ''); if (DIFFER(st)) { Push(c(l)[1]); Push(c(l)[2]); } else { Push(l); } Push(l); Push(r); if ((k ? (POS(0) '__rk_'), IDENT(k, 'iand'), IDENT(k, 'ishift'))) { Call2(k); } else { Reduce(k, 2); } if (DIFFER(st)) { Reduce(st, 3); } else { Reduce('TT_ASSIGN', 2); } AssignOp = .dummy; nreturn; }
post_inc = TABLE(); post_seq = TABLE(); post_n = 0; twpost_n = 0;
function IncDec(k, post, l, st) { l = Pop(); st = (IDENT(t(l), 'TT_ARR_GET') 'TT_ARR_SET', IDENT(t(l), 'TT_HASH_GET') 'TT_HASH_SET', ''); if (DIFFER(st)) { Push(c(l)[1]); Push(c(l)[2]); Push(l); Push(mk('TT_ILIT', 1)); Reduce(k, 2); Reduce(st, 3); if (IDENT(post, 'p')) { PostSeq(); } IncDec = .dummy; nreturn; } Push(l); Push(l); Push(mk('TT_ILIT', 1)); Reduce(k, 2); Reduce('TT_ASSIGN', 2); if (IDENT(post, 'p')) { PostSeq(); } IncDec = .dummy; nreturn; }
function PostTail(x) { x = Top(); if (~(EQ(last_term, 0) IDENT(post_inc[x], 'p'))) { return; } PostSeq(); return; }
function ValPost(x) { x = Top(); if (IDENT(post_inc[x], 'p') IDENT(t(x), 'TT_ASSIGN')) { PostSeq(); } ValPost = .dummy; nreturn; }
function PostSeq(x, l, k, tmp, e, st) { x = Pop(); st = t(x);
    if ((IDENT(st, 'TT_ARR_SET'), IDENT(st, 'TT_HASH_SET'))) { l = mk((IDENT(st, 'TT_ARR_SET') 'TT_ARR_GET', 'TT_HASH_GET'), ''); Append(l, c(x)[1]); Append(l, c(x)[2]); e = c(x)[3]; } else { l = c(x)[1]; e = c(x)[2]; } k = t(e);
    if (IDENT(t(l), 'TT_TWIGIL_FIELD')) { tmp = '__twpost_' twpost_n; twpost_n = twpost_n + 1; } else { tmp = '__post_' post_n; post_n = post_n + 1; }
    Push(mk('TT_VAR', tmp)); Push(l); Reduce('TT_ASSIGN', 2);
    if ((IDENT(st, 'TT_ARR_SET'), IDENT(st, 'TT_HASH_SET'))) { Push(c(x)[1]); Push(c(x)[2]); Push(mk('TT_VAR', tmp)); Push(mk('TT_ILIT', 1)); Reduce(k, 2); Reduce(st, 3); } else { Push(l); Push(l); Push(mk('TT_ILIT', 1)); Reduce(k, 2); Reduce('TT_ASSIGN', 2); }
    Push(mk('TT_VAR', tmp)); Reduce('TT_SEQ_EXPR', 3); post_seq[Top()] = 1; return; }
function Call1(nm, x)      { x = Pop(); Push(mk('TT_VAR', nm)); Push(x); Reduce('TT_FNC', 2, nm); Call1 = .dummy; nreturn; }
function NumCtx(x)         { x = Pop(); if (IDENT(t(x), 'TT_VAR') (v(x) ? (POS(0) '@'))) { Push(x); Push(mk('TT_QLIT', 'elems')); Reduce('TT_METHCALL', 2); NumCtx = .dummy; nreturn; } if ((IDENT(t(x), 'TT_ILIT'), IDENT(t(x), 'TT_FLIT'))) { Push(x); NumCtx = .dummy; nreturn; } Push(x); Push(mk('TT_ILIT', 0)); Reduce('TT_ADD', 2); NumCtx = .dummy; nreturn; }
function Elems(x) { if (IDENT(t(x), 'TT_VAR') DIFFER(arr_tbl[v(x)])) { Push(x); Push(mk('TT_QLIT', 'elems')); Reduce('TT_METHCALL', 2); return; } Push(x); return; }
function Arith(k, r, l) { r = Pop(); l = Pop(); Elems(l); Elems(r); Reduce(k, 2); Arith = .dummy; nreturn; }
function Mns(x) { x = Pop(); Elems(x); Reduce('TT_MNS', 1); Mns = .dummy; nreturn; }
function Arith2(nm, r, l)  { r = Pop(); l = Pop(); Push(mk('TT_VAR', nm)); Elems(l); Elems(r); Reduce('TT_FNC', 3, nm); Arith2 = .dummy; nreturn; }
function Call2(nm, r, l)   { r = Pop(); l = Pop(); Push(mk('TT_VAR', nm)); Push(l); Push(r); Reduce('TT_FNC', 3, nm); Call2 = .dummy; nreturn; }
cmp_kind = TABLE();
cmp_kind['TT_EQ'] = 1; cmp_kind['TT_NE'] = 1; cmp_kind['TT_LT'] = 1; cmp_kind['TT_GT'] = 1; cmp_kind['TT_LE'] = 1; cmp_kind['TT_GE'] = 1;
function ChainCmp(k, r, l, last) {
    r = Pop(); l = Pop(); Elems(l); l = Pop(); Elems(r); r = Pop();
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
function CaretRange(r) { r = Pop(); Push(mk('TT_ILIT', 0)); if (IDENT(t(r), 'TT_ILIT')) { Push(mk('TT_ILIT', v(r) - 1)); } else { Elems(r); Push(mk('TT_ILIT', 1)); Reduce('TT_SUB', 2); } Reduce('TT_TO', 2); caret_to[Top()] = 1; CaretRange = .dummy; nreturn; }
function RangeEx(r, l) {
    r = Pop(); l = Pop(); Push(l);
    if (IDENT(t(r), 'TT_ILIT')) { Push(mk('TT_ILIT', v(r) - 1)); Reduce('TT_TO', 2); RangeEx = .dummy; nreturn; }
    if (IDENT(t(r), 'TT_VAR') (v(r) ? (POS(0) '@'))) { Push(r); Push(mk('TT_QLIT', 'elems')); Reduce('TT_METHCALL', 2); Push(mk('TT_ILIT', 1)); Reduce('TT_SUB', 2); Reduce('TT_TO', 2); RangeEx = .dummy; nreturn; }
    Push(r); Push(mk('TT_ILIT', 1)); Reduce('TT_SUB', 2); Reduce('TT_TO', 2); RangeEx = .dummy; nreturn;
}
function MkBool(b) { Push(mk('TT_VAR', '__rk_mkbool')); Push(mk('TT_ILIT', b)); Reduce('TT_FNC', 2, '__rk_mkbool'); MkBool = .dummy; nreturn; }
named_pair = TABLE(); adverb_val = TABLE();
function Pair(r, l, nmd)   { r = Pop(); l = Pop(); nmd = 0; if (IDENT(t(l), 'TT_VAR') DIFFER(bare_nm[l])) { l = mk('TT_QLIT', v(l)); nmd = 1; } Push(mk('TT_VAR', '__rk_pair')); Push(l); Push(r); Reduce('TT_FNC', 3, '__rk_pair'); if (EQ(nmd, 1)) { named_pair[Top()] = 1; } Pair = .dummy; nreturn; }
function CPair(kind, r, l) { r = Pop(); l = Pop(); Push(mk('TT_VAR', '__rk_pair')); Push(l); Push(r); Reduce('TT_FNC', 3, '__rk_pair'); if (IDENT(kind, 'n')) { named_pair[Top()] = 1; } else { adverb_val[Top()] = 1; } CPair = .dummy; nreturn; }
/* a parenthesised or bracketed list of n items: one item is itself, pairs make __rk_hash with keys and values flattened, else __rk_arr */
function ListNode(n, force, lit, c, i) {
    if (EQ(n, 1) IDENT(force)) { ListNode = .dummy; nreturn; }
    if (IDENT(lit, 1)) { c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } Push(mk('TT_VAR', '__rk_arr_lit')); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_FNC', n + 1, '__rk_arr_lit'); ListNode = .dummy; nreturn; }
    c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; }
    if (GT(n, 0) IsPair(c[1])) { Push(mk('TT_VAR', '__rk_hash')); PushFlatP(n, c); Reduce('TT_FNC', nFlatP(n, c) + 1, '__rk_hash'); ListNode = .dummy; nreturn; }
    Push(mk('TT_VAR', '__rk_arr')); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_FNC', n + 1, '__rk_arr'); ListNode = .dummy; nreturn;
}
paren_list = TABLE();
function MarkParen(x) { x = Top(); if (IDENT(t(x), 'TT_FNC') IDENT(v(x), '__rk_arr')) { paren_list[x] = 1; } MarkParen = .dummy; nreturn; }
listctor = TABLE();
listctor['__rk_arr'] = 1; listctor['__rk_arr_lit'] = 1; listctor['__rk_hash'] = 1; listctor['__rk_arr_xx'] = 1; listctor['__rk_range_arr'] = 1; listctor['__rk_arr_slice'] = 1; listctor['__rk_undef'] = 1; listctor['__rk_arr_pick'] = 1;
one_scalar = TABLE();
one_scalar['TT_QLIT'] = 1; one_scalar['TT_ILIT'] = 1; one_scalar['TT_FLIT'] = 1; one_scalar['TT_CAT'] = 1; one_scalar['TT_XREP'] = 1; one_scalar['TT_ADD'] = 1; one_scalar['TT_SUB'] = 1;
one_scalar['TT_MUL'] = 1; one_scalar['TT_DIV'] = 1; one_scalar['TT_MOD'] = 1; one_scalar['TT_POW'] = 1; one_scalar['TT_MNS'] = 1;
function ListRhs(r, l) {
    if (~(IDENT(t(l), 'TT_VAR') (v(l) ? (POS(0) ANY('@%'))))) { if (IDENT(t(r), 'TT_XREP')) { Push(mk('TT_VAR', '__rk_rep')); Push(c(r)[1]); Push(c(r)[2]); Reduce('TT_FNC', 3, '__rk_rep'); return; } Push(r); return; }
    if (v(l) ? (POS(0) '%')) { if (IsPair(r)) { Push(r); ListNode(1, 1); return; } Push(r); return; }
    if (IDENT(t(r), 'TT_TO')) { Push(mk('TT_VAR', '__rk_range_arr')); Push(c(r)[1]); Push(c(r)[2]); Reduce('TT_FNC', 3, '__rk_range_arr'); return; }
    if (IsPair(r)) { Push(r); ListNode(1, 1); return; }
    if ((DIFFER(one_scalar[t(r)]), (IDENT(t(r), 'TT_VAR') IDENT(bare_nm[r]) ~(v(r) ? (POS(0) ANY('@%')))))) { Push(r); ListNode(1, 1); return; }
    Push(r); return;
}
function DeclInit(r, l) { ValPost(); r = Pop(); l = Pop(); Push(l); ListRhs(r, l); Reduce('TT_DECL', 3); DeclInit = .dummy; nreturn; }
function ArrInit(r, l) { ValPost(); r = Pop(); l = Pop(); Push(l); ListRhs(r, l); Reduce('TT_ASSIGN', 2); ArrInit = .dummy; nreturn; }
function IsListVar() { if (dv_tx ? (POS(0) ANY('@%'))) { return; } freturn; }
stmt_kind = TABLE();
stmt_kind['TT_RETURN'] = 1; stmt_kind['TT_SAY'] = 1; stmt_kind['TT_PRINT'] = 1; stmt_kind['TT_IF'] = 1; stmt_kind['TT_UNLESS'] = 1; stmt_kind['TT_WHILE'] = 1; stmt_kind['TT_UNTIL'] = 1;
stmt_kind['TT_REPEAT'] = 1; stmt_kind['TT_CLOOP'] = 1; stmt_kind['TT_EVERY'] = 1; stmt_kind['TT_CASE'] = 1; stmt_kind['TT_LOOP_BREAK'] = 1; stmt_kind['TT_LOOP_NEXT'] = 1;
stmt_kind['TT_SUB_DECL'] = 1; stmt_kind['TT_CLASS_DECL'] = 1; stmt_kind['TT_DIE'] = 1; stmt_kind['TT_TRY'] = 1; stmt_kind['TT_CATCH'] = 1; stmt_kind['TT_YADA'] = 1; stmt_kind['TT_SEQ'] = 1;
stmt_kind['TT_GATHER'] = 1; stmt_kind['TT_ROLE_DECL'] = 1; stmt_kind['TT_GRAMMAR_DECL'] = 1; stmt_kind['TT_SEQ_EXPR'] = 1; stmt_kind['TT_STMT'] = 1; stmt_kind['TT_PROGRAM'] = 1;
/* the value of a routine body is its last statement: at the closing brace the top node, if it is an expression, is the return */
function SetMulti(f) { is_multi = f; SetMulti = .dummy; nreturn; }
function RoutineEnd(nb, np, x, c, i, nm) {
    nb = TopCounter();
    if (GT(nb, 0)) { x = Top(); if ((IDENT(stmt_kind[t(x)]), DIFFER(post_seq[x]))) { Reduce('TT_RETURN', 1); } }
    PopCounter(); np = PopVal();
    if (EQ(is_multi, 1)) { c = ARRAY('1:' (GT(np + nb, 0) np + nb, 1)); i = np + nb; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } nm = Pop(); v(nm) = v(nm) '$' np; i = 0; while (LT(i, np)) { i = i + 1; v(nm) = v(nm) '$' Mangle(ptype[c[i]]); } Push(nm); i = 0; while (LT(i, np + nb)) { i = i + 1; Push(c[i]); } }
    Reduce('TT_SUB_DECL', np + nb + 1); RoutineEnd = .dummy; nreturn;
}
function Mangle(s) { s = (DIFFER(s) s, 'Any'); if (IDENT(s, '%')) { s = 'Any'; } while (s ? ':' = '_') { ; } Mangle = s; return; }
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
    Push(mk('TT_VAR', a)); RangeArr(lst); Reduce('TT_ASSIGN', 2);
    Push(mk('TT_VAR', ix)); Push(mk('TT_ILIT', 0)); Reduce('TT_ASSIGN', 2);
    Push(mk('TT_VAR', ix)); Push(mk('TT_VAR', 'elems')); Push(mk('TT_VAR', a)); Reduce('TT_FNC', 2, 'elems'); Reduce('TT_LT', 2);
    Push(mk('TT_VAR', ix)); Push(mk('TT_VAR', ix)); Push(mk('TT_ILIT', np)); Reduce('TT_ADD', 2); Reduce('TT_ASSIGN', 2);
    i = 0; while (LT(i, np)) { i = i + 1; Push(c[i]); Push(mk('TT_VAR', '__rk_arr_at')); Push(mk('TT_VAR', a)); if (EQ(i, 1)) { Push(mk('TT_VAR', ix)); } else { Push(mk('TT_VAR', ix)); Push(mk('TT_ILIT', i - 1)); Reduce('TT_ADD', 2); } Reduce('TT_FNC', 3, '__rk_arr_at'); Reduce('TT_ASSIGN', 2); }
    Push(body); Reduce('TT_SEQ', np + 1); Reduce('TT_CLOOP', 4); Reduce('TT_SEQ', 2); ForStmtN = .dummy; nreturn;
}
function DoFor(body, lst, e) { body = Pop(); lst = Pop(); e = body; while (IDENT(t(e), 'TT_SEQ_EXPR') EQ(n(e), 1)) { e = c(e)[1]; } Push(e); Push(lst); Reduce('TT_MAP', 2); DoFor = .dummy; nreturn; }
function EnumDecl(l, i, k) { l = Pop(); i = 1; while (LT(i, n(l))) { i = i + 1; k = c(l)[i]; const_tbl[v(k)] = 1; Push(mk('TT_VAR', v(k))); Push(mk('TT_ILIT', i - 2)); Reduce('TT_ASSIGN', 2); } Reduce('TT_SEQ_EXPR', n(l) - 1); EnumDecl = .dummy; nreturn; }
function Given(n, topic, i, c) { c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } topic = Pop(); Push(topic); i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); } Reduce('TT_CASE', n + 1); Given = .dummy; nreturn; }
function ReduceBlock(np, i, c, body) { body = Pop(); c = ARRAY('1:' (GT(np, 0) np, 1)); i = np; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } Push(body); i = 0; while (LT(i, np)) { i = i + 1; Push(c[i]); } Reduce('TT_ANON_BLOCK', np + 1); ReduceBlock = .dummy; nreturn; }
function ArrGet(ix, a, i) { ix = Pop(); a = Pop(); if (IDENT(t(ix), 'TT_FNC') IDENT(v(ix), '__rk_arr')) { Push(mk('TT_VAR', '__rk_arr_pick')); Push(a); i = 1; while (LT(i, n(ix))) { i = i + 1; Push(c(ix)[i]); } Reduce('TT_FNC', n(ix) + 1, '__rk_arr_pick'); ArrGet = .dummy; nreturn; } if (IDENT(t(ix), 'TT_TO')) { Push(mk('TT_VAR', '__rk_arr_slice')); Push(a); Push(c(ix)[1]); Push(c(ix)[2]); Reduce('TT_FNC', 4, '__rk_arr_slice'); ArrGet = .dummy; nreturn; } Push(a); Push(ix); Reduce('TT_ARR_GET', 2); ArrGet = .dummy; nreturn; }
function HyperMeth(nm, x) { nm = Pop(); x = Pop(); Push(mk('TT_VAR', '__rk_hyper_meth')); Push(x); Push(nm); Reduce('TT_FNC', 3, '__rk_hyper_meth'); HyperMeth = .dummy; nreturn; }
function ZenSlice(a) { a = Pop(); Push(mk('TT_VAR', '__rk_arr_slice')); Push(a); Push(mk('TT_ILIT', 0)); Push(a); Push(mk('TT_QLIT', 'elems')); Reduce('TT_METHCALL', 2); Push(mk('TT_ILIT', 1)); Reduce('TT_SUB', 2); Reduce('TT_FNC', 4, '__rk_arr_slice'); ZenSlice = .dummy; nreturn; }
function WhateverIdx(r, a) { r = Pop(); a = Top(); Push(a); Push(mk('TT_QLIT', 'elems')); Reduce('TT_METHCALL', 2); Push(r); Reduce('TT_SUB', 2); WhateverIdx = .dummy; nreturn; }
function Adverb(k, g) { g = Pop(); Push(c(g)[1]); Push(c(g)[2]); Reduce(k, 2); Adverb = .dummy; nreturn; }
function Smatch(r, l) { r = Pop(); l = Pop(); if (DIFFER(rx_node[r])) { Push(l); Push(c(r)[2]); Push(mk('TT_QLIT', (DIFFER(rx_kind[r]) rx_kind[r], 'match'))); Reduce('TT_SMATCH', 3); Smatch = .dummy; nreturn; } if (DIFFER(typeobj[r])) { Push(l); Push(mk('TT_QLIT', 'does')); Push(mk('TT_QLIT', typeobj[r])); Reduce('TT_METHCALL', 3); Smatch = .dummy; nreturn; } if (IDENT(t(r), 'TT_VAR') DIFFER(bare_nm[r])) { Push(l); Push(mk('TT_QLIT', 'does')); Push(mk('TT_QLIT', v(r))); Reduce('TT_METHCALL', 3); Smatch = .dummy; nreturn; } Push(mk('TT_VAR', '__rk_smartmatch')); Push(l); Push(r); Reduce('TT_FNC', 3, '__rk_smartmatch'); Smatch = .dummy; nreturn; }
function NSmatch(r, l) { r = Pop(); l = Pop(); Push(mk('TT_VAR', '__rk_not_smartmatch')); Push(l); if (IDENT(t(r), 'TT_VAR') DIFFER(bare_nm[r])) { Push(mk('TT_VAR', '__rk_typeobj')); Push(mk('TT_QLIT', v(r))); Reduce('TT_FNC', 2, '__rk_typeobj'); } else { Push(r); } Reduce('TT_FNC', 3, '__rk_not_smartmatch'); NSmatch = .dummy; nreturn; }
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
op_band  = *$' ' '+&' @op_q *NoneAt(op_q, '=') *$' ';        op_shl = *$' ' '+<' @op_q *NoneAt(op_q, '=') *$' ';   op_sband = *$' ' '~&' @op_q *NoneAt(op_q, '=') *$' ';
op_bor   = *$' ' '+|' @op_q *NoneAt(op_q, '=') *$' ';        op_sbor = *$' ' '~|' @op_q *NoneAt(op_q, '=') *$' ';
kw_if = Kw('if');  kw_unless = Kw('unless');  kw_while = Kw('while');  kw_until = Kw('until');  kw_for = Kw('for');  kw_loop = Kw('loop');  kw_repeat = Kw('repeat');
kw_given = Kw('given');  kw_when = Kw('when');  kw_default = Kw('default');  kw_else = Kw('else');  kw_elsif = Kw('elsif');  kw_CATCH = Kw('CATCH');  kw_use = Kw('use');
kw_my = Kw('my');  kw_our = Kw('our');  kw_state = Kw('state');  kw_has = Kw('has');  kw_constant = Kw('constant');  kw_multi = Kw('multi');  kw_proto = Kw('proto');
kw_sub = Kw('sub');  kw_method = Kw('method');  kw_submethod = Kw('submethod');  kw_class = Kw('class');  kw_role = Kw('role');  kw_grammar = Kw('grammar');  kw_module = Kw('module');
kw_token = Kw('token');  kw_rule = Kw('rule');  kw_regex = Kw('regex');  kw_is = Kw('is');  kw_does = Kw('does');  kw_handles = Kw('handles');  kw_returns = Kw('returns');
kw_gather = Kw('gather');  kw_try = Kw('try');  kw_do = Kw('do');  kw_not = Kw('not');  kw_so = Kw('so');  kw_self = Kw('self');  kw_with = Kw('with');  kw_without = Kw('without');  kw_await = Kw('await');  kw_enum = Kw('enum');  kw_start = Kw('start');  kw_X = *$'  ' 'X' @kw_q *NoWordAt(kw_q) *$'  ';  kw_Z = *$'  ' 'Z' @kw_q *NoWordAt(kw_q) *$'  ';
/* ==================================================================================================================== */
/* term (token termish, term:sym<...>): variables, values, calls, names, circumfixes, blocks, regexes, reductions           */
/* ==================================================================================================================== */
list_items = ( *item . *ValPost() . *IncCounter() FENCE(*$' ' ',' *$' ' FENCE(*list_items | epsilon . *SetForce()) | epsilon) );
args_p   = ( '(' *$' ' FENCE(*list_items *list_infix | epsilon) *$' ' ')' );
call_args = ( epsilon . *PushCounter() *args_p . *Call(PopVal(), nTop()) . *PopCounter() );
listop_args = ( epsilon . *PushCounter() *$'  ' @lo_q *NoneAt(lo_q, ';}),=|&') *NoInfixAhead(lo_q) *list_items *list_infix . *Call(PopVal(), nTop()) . *PopCounter() );
listop_none = ( epsilon . *Call(PopVal(), 0) );
block_term = ( *$' ' '->' . *PushCounter() FENCE(*$' ' *params | epsilon) *$' ' *block . *ReduceBlock(nTop()) . *PopCounter()
             | *$' ' *block . *BlockOrHash() );
function BlockOrHash(b, s, i, n2) { b = Top(); if (EQ(n(b), 1)) { s = c(b)[1]; if (IsPair(s)) { Pop(); Push(s); ListNode(1, 1); BlockOrHash = .dummy; nreturn; } if (IDENT(t(s), 'TT_FNC') IDENT(v(s), '__rk_hash')) { Pop(); Push(s); BlockOrHash = .dummy; nreturn; } } Reduce('TT_ANON_BLOCK', 1); BlockOrHash = .dummy; nreturn; }
list_infix = FENCE( ( *kw_X . *ListNode(nTop(), '') . *SetCount(1) . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() . *ListOp2('__rk_cross')
                    | *kw_Z . *ListNode(nTop(), '') . *SetCount(1) . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() . *ListOp2('__rk_zip') ) *list_infix | epsilon );
paren    = ( '(' *$' ' . *PushCounter() . *ClearForce() FENCE(*list_items *list_infix | epsilon) *$' ' FENCE(',' *$' ' . *SetForce() | epsilon) ')' . *ListNode(nTop(), lst_force) . *MarkParen() . *PopCounter() );
bracket  = ( '[' *$' ' . *PushCounter() FENCE(*list_items | epsilon) *$' ' FENCE(',' *$' ' | epsilon) ']' . *ListNode(nTop(), 1, 1) . *PopCounter() );
reduce_op = ( '[' ( ('+' . *SetRed('add')) | ('-' . *SetRed('sub')) | ('*' . *SetRed('mul')) | ('~' . *SetRed('cat')) | ('min' . *SetRed('min')) | ('max' . *SetRed('max')) ) ']' *$' '
              . *Sh('TT_VAR', '__rk_reduce_' red_nm) *rng_expr . *RangeArrTop() . *Reduce('TT_FNC', 2, '__rk_reduce_' red_nm) );
function RangeArrTop(x) { x = Pop(); RangeArr(x); RangeArrTop = .dummy; nreturn; }
key_term = ( *kw_do *$'  ' *kw_for *$' ' . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() *$' ' *block . *DoFor()
           | *kw_await *$' ' *item
           | *kw_start *$' ' *block
           | *kw_sub *$' ' . *PushCounter() FENCE('(' *$' ' FENCE(*params | epsilon) *$' ' ')' *$' ' | epsilon) *block . *ReduceBlock(nTop()) . *PopCounter()
           | *kw_gather *$' ' *block . *Reduce('TT_GATHER', 1)
           | *kw_gather *$'  ' *control . *Reduce('TT_GATHER', 1)
           | *kw_try *$' ' *block . *Reduce('TT_TRY', 1)
           | *kw_do *$' ' . *Sh('TT_VAR', '__rk_phaser_do') *block . *Reduce('TT_FNC', 2, '__rk_phaser_do')
           | ( 'once' | 'quietly' | 'react' ) . ph_w @kw_q *NoWordAt(kw_q) *$' ' . *Sh('TT_VAR', '__rk_phaser_' ph_w) *block . *Reduce('TT_FNC', 2, '__rk_phaser_' ph_w)
           | *kw_not *$' ' *item . *Reduce('TT_NOT', 1)
           | *kw_so *$' ' . *PushVal('so') . *PushCounter() *item . *IncCounter() . *Call(PopVal(), nTop()) . *PopCounter()
           | *kw_self . *Sh('TT_VAR', 'self')
           | *kw_my *$'  ' *var_tok . *NoteArr() *$' ' *op_assign *decl_init . *ArrInit()
           | ':' *var_tok . *ColonVar()
           );
function ColonVar(x) { x = Pop(); MkBool(1); ColonVar = .dummy; nreturn; }
term     = *ClearBare() ( ':' *ident . cp_nm . *Sh('TT_QLIT', cp_nm) ( '(' *$' ' *item *$' ' ')' . *CPair('n') | '<' BREAK('>') . cp_v '>' . *Sh('TT_QLIT', cp_v) . *CPair('v') | epsilon . *MkBool(1) . *CPair('n') )
           | ':!' *ident . cp_nm . *Sh('TT_QLIT', cp_nm) . *MkBool(0) . *CPair('v')
           | '^' *$' ' *pow_expr . *CaretRange()
           | '.' (FENCE('^' | epsilon) *ident) . mth_tx . *Sh('TT_VAR', '_') . *Sh('TT_QLIT', mth_tx) . *PushCounter() FENCE(*args_p | epsilon) . *MethCall(nTop()) . *PopCounter()
           | '.' '[' *$' ' . *Sh('TT_VAR', '_') *item *$' ' ']' . *Reduce('TT_ARR_GET', 2)
           | '.' '<' . *Sh('TT_VAR', '_') BREAK('>') . tk_tx '>' . *Sh('TT_QLIT', tk_tx) . *Reduce('TT_HASH_GET', 2)
           | '.' '{' *$' ' . *Sh('TT_VAR', '_') *item *$' ' '}' . *Reduce('TT_HASH_GET', 2)
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
           | *ident $ cl_i . cl_nm @pk_q *PairAhead(pk_q) *SetBare() . *BareKey(cl_nm)
           | *ident $ cl_i . cl_nm @pk_q *WordArgAhead(pk_q) *SetBare() . *Bare(cl_nm)
           | ('int' | 'num' | 'str' | 'uint') . cl_nm @cl_q *IsAt(cl_q, '(') . *Sh('TT_VAR', cl_nm)
           | *name $ cl_i . cl_nm *NotOpWord(cl_i) @cl_q *IsAt(cl_q, '(') . *PushVal(cl_nm) *call_args
           | *ident $ cl_i . cl_nm *NotCall(cl_i) *IsListop(cl_i) @cl_q *EndAhead(cl_q) . *PushVal(cl_nm) *listop_none
           | *ident $ cl_i . cl_nm *NotCall(cl_i) @cl_q *IsAt(cl_q, ' ' tab) . *PushVal(cl_nm) *listop_args
           | *ident $ cl_i . cl_nm *NotCall(cl_i) *IsListop(cl_i) . *PushVal(cl_nm) *listop_none
           | *name $ cl_i . cl_nm *NotOpWord(cl_i) FENCE(':' ANY('DU_') @sm_q *NoWordAt(sm_q) | epsilon) *SetBare() . *Bare(cl_nm)
           );
/* postfixes: method call, subscripts, call parens, ++ --, :exists :delete -- adjacent to the term (Grammar.nqp: no ws before them) */
postfix  = FENCE( ('>>.' | '».') *ident . mth_tx . *Sh('TT_QLIT', mth_tx) . *HyperMeth() *postfix
                | ANY('.!') (FENCE('^' | epsilon) *ident) . mth_tx . *Sh('TT_QLIT', mth_tx) . *PushCounter() FENCE(*args_p | ':' *$' ' *list_items | epsilon) *ClearBare() . *MethCall(nTop()) . *PopCounter() *postfix
                | '[' *$' ' '*' *$' ' ']' . *ZenSlice() *postfix
                | '[' *$' ' ']' . *Reduce('TT_NUL', 0) . *Reduce('TT_ARR_GET', 2) *postfix
                | '[' *$' ' ( '*' *$' ' '-' *$' ' *item . *WhateverIdx() | epsilon . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() ) *$' ' ']' *ClearBare() . *ArrGet() *postfix
                | *MaySub() '<' BREAK('>') . hk_tx '>' . *Sh('TT_QLIT', hk_tx) . *Reduce('TT_HASH_GET', 2) *ClearBare() *postfix
                | *MaySub() '{' *$' ' *item *$' ' '}' *ClearBare() . *Reduce('TT_HASH_GET', 2) *postfix
                | '(' *$' ' . *PushCounter() FENCE(*list_items | epsilon) *$' ' ')' *ClearBare() . *Reduce('TT_INVOKE', nTop() + 1) . *PopCounter() *postfix
                | '++' . *IncDec('TT_ADD', 'p') *postfix
                | '--' . *IncDec('TT_SUB', 'p') *postfix
                | ':exists' . *Adverb('TT_HASH_EXISTS') *postfix
                | ':delete' . *Adverb('TT_HASH_DELETE') *postfix
                | epsilon );
postfix_expr = *term *postfix;
/* ==================================================================================================================== */
/* the precedence climb, tightest first: ** | symbolic unary | * / % | + - | x xx | ~ | & | | ^ | .. | cmp | chaining | && | || // | ?? !! | = op= | and | or */
/* ==================================================================================================================== */
pow_expr   = *postfix_expr FENCE( *op_pow *unary_expr . *Arith('TT_POW') | epsilon );
unary_expr = ( *$' ' ANY('+-') '«' *$' ' *unary_expr
             | *$' ' '-' @un_q *NoneAt(un_q, '-=>') *$' ' *unary_expr . *Mns()
             | *$' ' '+' @un_q *NoneAt(un_q, '+=') *$' ' *unary_expr . *NumCtx()
             | *$' ' '~' @un_q *NoneAt(un_q, '~=') *$' ' *unary_expr . *Call1('__rk_str')
             | *$' ' '!' @un_q *NoneAt(un_q, '!=~') *$' ' *unary_expr . *Reduce('TT_NOT', 1)
             | *$' ' '?' @un_q *NoneAt(un_q, '?') *$' ' *unary_expr . *Call1('__rk_mkbool')
             | *$' ' '++' *unary_expr . *IncDec('TT_ADD')
             | *$' ' '--' *unary_expr . *IncDec('TT_SUB')
             | *pow_expr );
mul_expr = *unary_expr *mul_tail;
mul_tail = FENCE( ( *op_divis *unary_expr . *Reduce('TT_DIVIS', 2)
                  | *op_mul   *unary_expr . *Arith('TT_MUL')
                  | *op_div   *unary_expr . *Arith('TT_DIV')
                  | *op_mod   *unary_expr . *Arith('TT_MOD')
                  | *op_div_i *unary_expr . *Arith2('__rk_intdiv')
                  | *op_mod_w *unary_expr . *Call2('__rk_mod')
                  | *op_gcd   *unary_expr . *Call2('__rk_gcd')
                  | *op_lcm   *unary_expr . *Call2('__rk_lcm')
                  | *op_band  *unary_expr . *Call2('iand') | *op_shl *unary_expr . *Call2('ishift') | *op_sband *unary_expr . *Call2('__rk_sband') ) *mul_tail | epsilon );
add_expr = *mul_expr *add_tail;
add_tail = FENCE( ( *op_add *mul_expr . *Arith('TT_ADD') | *op_sub *mul_expr . *Arith('TT_SUB') | *op_bor *mul_expr . *Call2('__rk_bor') | *op_sbor *mul_expr . *Call2('__rk_sbor') ) *add_tail | epsilon );
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
                  | *op_nsmatch *str_expr . *NSmatch()
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
lst_expr = *cond_expr;
function RangeArr(x) { if (IDENT(t(x), 'TT_TO')) { Push(mk('TT_VAR', '__rk_range_arr')); Push(c(x)[1]); Push(c(x)[2]); Reduce('TT_FNC', 3, '__rk_range_arr'); return; } Push(x); return; }
function ListOp2(nm, r, l) { r = Pop(); l = Pop(); Push(mk('TT_VAR', nm)); ListArg(l); ListArg(r); Reduce('TT_FNC', 3, nm); ListOp2 = .dummy; nreturn; }
function ListArg(x) { if (IDENT(t(x), 'TT_TO')) { RangeArr(x); return; } if ((DIFFER(one_scalar[t(x)]), (IDENT(t(x), 'TT_VAR') IDENT(bare_nm[x]) (v(x) ? (POS(0) '$'))))) { Push(mk('TT_VAR', '__rk_arr')); Push(x); Reduce('TT_FNC', 2, '__rk_arr'); return; } Push(x); return; }
item     = ( *lst_expr FENCE( *op_pair *item . *Pair()
                             | *op_assign *decl_init . *Assign()
                             | *$' ' '.=' *$' ' *ident . mth_tx . *Sh('TT_QLIT', mth_tx) . *PushCounter() FENCE(*args_p | epsilon) . *DotAssign(nTop()) . *PopCounter()
                             | *op_bind *item . *Assign()
                             | *$' ' '+=' *$' ' *item . *AssignOp('TT_ADD') | *$' ' '-=' *$' ' *item . *AssignOp('TT_SUB')
                             | *$' ' '*=' *$' ' *item . *AssignOp('TT_MUL') | *$' ' '/=' *$' ' *item . *AssignOp('TT_DIV')
                             | *$' ' '~=' *$' ' *item . *AssignOp('TT_CAT') | *$' ' '%=' *$' ' *item . *AssignOp('TT_MOD')
                             | *$' ' '//=' *$' ' *item . *AssignOp('__rk_dor')
                             | *$' ' '||=' *$' ' *item . *AssignOp('TT_ALT') | *$' ' '&&=' *$' ' *item . *AssignOp('TT_SEQ')
                             | *$' ' '**=' *$' ' *item . *AssignOp('TT_POW')
                             | *$'  ' 'x=' *$' ' *item . *AssignOp('__rk_rep') | *$'  ' 'xx=' *$' ' *item . *AssignOp('__rk_arr_xx')
                             | *$'  ' 'div=' *$' ' *item . *AssignOp('__rk_intdiv') | *$'  ' 'mod=' *$' ' *item . *AssignOp('__rk_mod')
                             | *$'  ' 'min=' *$' ' *item . *AssignOp('__rk_min') | *$'  ' 'max=' *$' ' *item . *AssignOp('__rk_max')
                             | *$'  ' 'gcd=' *$' ' *item . *AssignOp('__rk_gcd') | *$'  ' 'lcm=' *$' ' *item . *AssignOp('__rk_lcm')
                             | *$' ' '+|=' *$' ' *item . *AssignOp('__rk_bor') | *$' ' '+&=' *$' ' *item . *AssignOp('iand') | *$' ' '+<=' *$' ' *item . *AssignOp('ishift')
                             | *$' ' '%%=' *$' ' *item . *AssignOp('TT_DIVIS')
                             | epsilon ) );
land_expr = *item *land_tail;
land_tail = FENCE( *op_land *item . *Reduce('TT_SEQ', 2) *land_tail | epsilon );
lor_expr = *land_expr *lor_tail;
lor_tail = FENCE( ( *op_lor *land_expr . *Reduce('TT_ALT', 2) | *op_lxor *land_expr . *Call2('__rk_xor') ) *lor_tail | epsilon );
expr     = *lor_expr;
/* ==================================================================================================================== */
/* blocks and statements (token block, rule statementlist, token statement, statement_mod_cond, statement_mod_loop)      */
/* ==================================================================================================================== */
block    = ( '{' *$' ' . *PushCounter() *stmts *$' ' '}' . *BlockTail() . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter() );
function BlockTail() { if (GT(TopCounter(), 0)) { PostTail(); } BlockTail = .dummy; nreturn; }
stmts    = ARBNO( FENCE(*$' ' *statement . *IncCounter()) );
stmt_end = *$' ' FENCE( ';' . *SetTerm(1) | @se_q *IsAt(se_q, '}') . *SetTerm(0) | RPOS(0) . *SetTerm(0) );
function SetTerm(k) { last_term = k; SetTerm = .dummy; nreturn; }
function EndSlot() { if (EQ(last_term, 1)) { Reduce('TT_SEQ_EXPR', 0); Reduce('TT_ATTR', 1, ':subj'); Reduce('TT_STMT', 1); IncCounter(); } EndSlot = .dummy; nreturn; }
blk_end  = FENCE(SPAN(' ' tab) | epsilon) @be_q *AtEnd(be_q) . *SetTerm(1);
function AtEnd(q, c) { c = SUBSTR(Src, q + 1, 1); if (IDENT(c)) { return; } if ((IDENT(c, nl), IDENT(c, cr), IDENT(c, ';'), IDENT(c, '}'), IDENT(c, '#'))) { return; } freturn; }
stmt_mods = FENCE( *stmt_mod *$' ' *stmt_mods | epsilon );
stmt_mod = ( *kw_if *$'  ' *expr . *Mod('TT_IF')
                | *kw_unless *$'  ' *expr . *Mod('TT_UNLESS')
                | *kw_while *$'  ' *expr . *Mod('TT_WHILE')
                | *kw_until *$'  ' *expr . *Mod('TT_UNTIL')
                | *kw_for *$'  ' *expr . *ModFor()
                | *kw_given *$'  ' *expr . *ModGiven()
                | *kw_with *$'  ' *expr . *ModWith('TT_IF')
                | *kw_without *$'  ' *expr . *ModWith('TT_UNLESS') );
statement = ( *control
            | *declaration
            | *phaser
            | '{' *$' ' . *PushCounter() *stmts *$' ' '}' . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter() *blk_end
            | ('...' | '!!!' | '???') . *Reduce('TT_YADA', 0) *stmt_end
            | '(' *$' ' . *PushCounter() *var_tok . *IncCounter() ARBNO(*$' ' ',' *$' ' *var_tok . *IncCounter()) *$' ' ')' *$' ' . *PushVal(nTop()) . *PopCounter() *op_assign . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() . *Destr(PopVal()) *stmt_mods *stmt_end
            | *expr *$' ' *stmt_mods *stmt_end
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
           | *kw_with *cond *block . *Reduce('TT_IF', 2) *blk_end
           | *kw_without *cond *block . *Reduce('TT_UNLESS', 2) *blk_end
           | ( 'QUIT' | 'CONTROL' ) @kw_q *NoWordAt(kw_q) *$' ' *block . *Reduce('TT_CATCH', 1) *blk_end
           | *kw_while *cond *block . *Reduce('TT_WHILE', 2) *blk_end
           | *kw_until *cond *block . *Reduce('TT_UNTIL', 2) *blk_end
           | *kw_repeat *$' ' *block *$' ' ( *kw_until *cond . *Reduce('TT_REPEAT', 2) | *kw_while *cond . *Reduce('TT_REPEAT', 2) ) *stmt_end
           | *kw_loop *$' ' '(' *$' ' *loop_part ';' *$' ' *loop_part ';' *$' ' *loop_part ')' *$' ' *block . *Reduce('TT_CLOOP', 4) *blk_end
           | *kw_loop *$' ' *block . *LoopForever() *blk_end
           | *kw_for *$' ' . *PushCounter() *list_items *list_infix . *ListNode(nTop(), '') . *PopCounter() *$' ' ( '->' *$' ' . *PushCounter() ( '(' *$' ' *params *$' ' ')' | *params ) . *PushVal(nTop()) . *PopCounter() *$' ' *block . *ForStmtN(PopVal()) | *block . *Sh('TT_VAR', '') . *ForSwap() . *ForStmt() ) *blk_end
           | *kw_given *cond '{' *$' ' . *PushCounter() *when_arms *$' ' '}' . *Given(nTop()) . *PopCounter() *blk_end
           | *kw_CATCH *$' ' '{' *$' ' . *Sh('TT_VAR', '_') . *PushCounter() . *SetArms(0) *when_arms *$' ' '}' . *CatchEnd(nTop()) . *Reduce('TT_CATCH', 1) . *PopCounter() *blk_end
           | *kw_try *$' ' *block . *Reduce('TT_TRY', 1) *blk_end
           | *kw_gather *$' ' *block . *Reduce('TT_GATHER', 1) *blk_end
           | *kw_use *$'  ' ( 'v6' . *PushCounter() ARBNO('.' *ident . uv_tx . *UseChain(uv_tx)) . *Reduce('TT_USE_DECL', nTop(), 'v6') . *PopCounter() | *name . use_nm . *Reduce('TT_USE_DECL', 0, use_nm) ) *stmt_end
           );
function UseChain(s) { if (EQ(TopCounter(), 0)) { Push(mk('TT_VAR', '_')); IncCounter(); } Push(mk('TT_QLIT', s)); Reduce('TT_METHCALL', 2); UseChain = .dummy; nreturn; }
loop_part = FENCE( *scope_decl . *LoopDecl() *$' ' | *expr . *ValPost() *$' ' | epsilon . *Reduce('TT_NUL', 0) );
function LoopDecl(d) { d = Top(); if (IDENT(t(d), 'TT_DECL')) { d = Pop(); Push(c(d)[2]); Push(c(d)[3]); Reduce('TT_ASSIGN', 2); } LoopDecl = .dummy; nreturn; }
function WhenCond(x) { x = Top(); if (IDENT(t(x), 'TT_VAR') IDENT(v(x), 'X::AdHoc')) { Pop(); Push(mk('TT_NUL', '')); } WhenCond = .dummy; nreturn; }
when_arms = ARBNO( FENCE( *$' ' *kw_when *cond . *WhenCond() *block . *IncCounter() . *IncCounter() . *SetArms(1)
                        | *$' ' *kw_default *$' ' . *Reduce('TT_NUL', 0) *block . *IncCounter() . *IncCounter() . *SetArms(1)
                        | *$' ' *statement . *IncCounter() ) );
/* phasers and statement prefixes keep their place in the statement list (the placement is the lowerer's)                */
phaser   = ( ( 'BEGIN' | 'END' | 'INIT' | 'CHECK' | 'FIRST' | 'LAST' | 'NEXT' | 'ENTER' | 'LEAVE' | 'KEEP' | 'UNDO' | 'PRE' | 'POST' | 'TEMP' | 'once' | 'quietly' | 'react' | 'do' ) . ph_w @kw_q *NoWordAt(kw_q) *$' ' . *Sh('TT_VAR', '__rk_phaser_' ph_w) *block . *Reduce('TT_FNC', 2, '__rk_phaser_' ph_w) *$' ' FENCE(';' . *SetTerm(1) | epsilon . *SetTerm(0))
           | ( 'CLOSE' | 'supply' | 'start' ) @kw_q *NoWordAt(kw_q) *$' ' *block *stmt_end
           | ( 'lazy' | 'eager' | 'sink' | 'hyper' | 'race' | 'start' ) @kw_q *NoWordAt(kw_q) *$'  ' *expr *stmt_end );
/* declarations: scope_declarator (my our state has constant), routine_declarator (sub method), package_declarator, regex_declarator */
type_nm  = ( *name . ty_tx @ty_q *IsAt(ty_q, ' ' tab) *$'  ' @ty_v *IsAt(ty_v, '$@%&') );
native_nm = ( ('int' | 'num' | 'str' | 'uint' | 'int8' | 'int16' | 'int32' | 'int64' | 'num32' | 'num64') @ty_q *IsAt(ty_q, ' ' tab) *$'  ' @ty_v *IsAt(ty_v, '$@%&') );
decl_init = ( *IsListVar() . *PushCounter() *list_items *list_infix . *ListNode(nTop(), '') . *PopCounter() | *item );
scope_decl = ( ( *kw_my | *kw_our | *kw_state ) *$'  '
                ( *type_nm . *Sh('TT_VAR', ty_tx) *var_tok . *NoteArr() *$' ' ( (*op_assign | *op_bind) *decl_init . *DeclInit() | epsilon . *Reduce('TT_DECL', 2) )
                | '(' *$' ' . *PushCounter() *var_tok . *SigilChild() . *IncCounter() ARBNO(*$' ' ',' *$' ' *var_tok . *SigilChild() . *IncCounter()) *$' ' ')' *$' ' . *PushVal(nTop()) . *PopCounter() ( *op_assign . *PushCounter() *list_items . *ListNode(nTop(), '') . *PopCounter() | epsilon . *ListNode(0, '') ) . *Destr(PopVal())
                | *var_tok . *NoteArr() *$' ' ( (*op_assign | *op_bind) *decl_init . *ArrInit() | epsilon . *UndefInit() ) ) );
arr_tbl = TABLE();
function NoteArr() { if (var_tx ? (POS(0) '@')) { arr_tbl[var_tx] = 1; } NoteArr = .dummy; nreturn; }
function SigilChild(v) { if (var_tx ? (POS(0) ANY('@%'))) { v = Pop(); Append(v, mk('TT_QLIT', SUBSTR(var_tx, 1, 1))); Push(v); } SigilChild = .dummy; nreturn; }
function Destr(n, r, i, c, tmp) { r = Pop(); c = ARRAY('1:' (GT(n, 0) n, 1)); i = n; while (GT(i, 0)) { c[i] = Pop(); i = i - 1; } tmp = '__destr_' destr_n; destr_n = destr_n + 1; Push(mk('TT_VAR', tmp)); Push(r); Reduce('TT_ASSIGN', 2);
    i = 0; while (LT(i, n)) { i = i + 1; Push(c[i]); Push(mk('TT_VAR', '__rk_arr_at')); Push(mk('TT_VAR', tmp)); Push(mk('TT_ILIT', i - 1)); Reduce('TT_FNC', 3, '__rk_arr_at'); Reduce('TT_ASSIGN', 2); } Reduce('TT_SEQ_EXPR', n + 1); Destr = .dummy; nreturn; }
declaration = ( FENCE((*kw_my | *kw_our) *$'  ' | epsilon) *kw_constant *$'  ' ( *name $ cn_now . cn_tx *NoteNow(cn_now) . *Sh('TT_VAR', cn_tx) | *var_tok ) *$' ' *op_assign *item . *Reduce('TT_ASSIGN', 2) *stmt_end
              | *scope_decl *stmt_mods *stmt_end
              | *kw_has *$'  ' *has_decl *stmt_end
              | FENCE((*kw_my | *kw_our) *$'  ' | epsilon) FENCE(*kw_multi *$'  ' . *SetMulti(1) | *kw_proto *$'  ' . *SetMulti(0) | epsilon . *SetMulti(0)) ( *kw_sub | *kw_method | *kw_submethod ) *$'  ' *routine *blk_end
              | *kw_multi *$'  ' . *SetMulti(1) *routine *blk_end
              | ( *kw_class . *SetPk('TT_CLASS_DECL') | *kw_role . *SetPk('TT_ROLE_DECL') | *kw_grammar . *SetPk('TT_GRAMMAR_DECL') | *kw_module . *SetPk('TT_CLASS_DECL') ) *$'  ' *package *blk_end
              | *kw_enum *$'  ' *name *$' ' ( *qwords | *paren ) . *EnumDecl() *stmt_end
              | FENCE(*kw_proto *$'  ' | epsilon) ( *kw_token | *kw_rule | *kw_regex ) *$'  ' (*name FENCE(':sym<' BREAK('>') '>' | epsilon)) . rg_nm . *Sh('TT_VAR', rg_nm) *$' ' FENCE('(' BREAK(')') ')' *$' ' | epsilon) '{' *rg_body . rg_tx '}' . *Sh('TT_QLIT', rg_tx) . *Reduce('TT_REGEX_DECL', 2) *blk_end
              );
rg_body  = ARBNO( NOTANY('{}') | '{' *rg_body '}' );
has_decl = ( FENCE(*name *$'  ' @ty_v *IsAt(ty_v, '$@%&') | epsilon) *sigil . hs_sg *twigil . hs_tw *ident . hs_nm *$' '
             ( *kw_is *$'  ' 'rw' . *Sh('TT_RW_DECL', hs_tw hs_nm)
             | *kw_is *$'  ' 'required' . *Sh('TT_HAS_DECL', hs_tw hs_nm)
             | *kw_is *$'  ' *ident FENCE('(' BREAK(')') ')' | epsilon) . *HasNode(hs_sg, hs_tw hs_nm)
             | *kw_handles *$'  ' '<' BREAK('>') . hd_tx '>' . *Sh('TT_QLIT', hd_tx) . *Reduce('TT_HANDLES_DECL', 1, hs_tw hs_nm)
             | epsilon . *HasNode(hs_sg, hs_tw hs_nm) )
             FENCE( *$' ' *kw_is *$'  ' *ident FENCE('(' BREAK(')') ')' | epsilon) | epsilon ) *$' ' FENCE( *op_assign *item . *HasDefault() | epsilon ) );
function HasDefault(d, h) { d = Pop(); h = Pop(); if (IDENT(t(h), 'TT_RW_DECL')) { Push(h); Sh('TT_HAS_DECL', v(h)); Append(Top(), d); IncCounter(); HasDefault = .dummy; nreturn; } if (IDENT(t(h), 'TT_VAR')) { t(h) = 'TT_HAS_DECL'; } Append(h, d); Push(h); HasDefault = .dummy; nreturn; }
params   = ( *param . *IncCounter() FENCE(*$' ' ',' *$' ' *params | epsilon) );
param    = ( bSlash @bp_q *IsAt(bp_q, ')') . *Sh('TT_VAR', '')
           | bSlash *ident $ sl_now . sl_nm *NoteNow(sl_now) . *Sh('TT_VAR', sl_nm) *$' ' FENCE( *op_assign *cond_expr . *ParamDefault() | epsilon )
           | ( (*name FENCE(':' ANY('DU_') | epsilon)) . pty_tx *$'  ' | epsilon . *NoPty() ) ( ('**' | '*' | '+') . ppf_tx . *SetPfx(ppf_tx) | ':' . *SetPfx('') | epsilon . *SetPfx('') ) *var_tok . *ParamType(pty_tx, ppf_tx) FENCE(ANY('?!') | epsilon) *$' ' FENCE( *op_assign *cond_expr . *ParamDefault() | epsilon ) FENCE( *$' ' *kw_is *$'  ' *ident | epsilon ) );
function NoPty() { pty_tx = ''; NoPty = .dummy; nreturn; }
function SetPfx(s) { ppf_tx = s; SetPfx = .dummy; nreturn; }
function ParamType(ty, pf, v) { v = Pop(); if (var_tx ? (POS(0) '@')) { arr_tbl[var_tx] = 1; } if (DIFFER(pf)) { Append(v, mk('TT_QLIT', pf SUBSTR(var_tx, 1, 1))); ptype[v] = 'Slurpy'; } else if (DIFFER(ty)) { Append(v, mk('TT_QLIT', ty)); ptype[v] = ty; } else if (var_tx ? (POS(0) '@')) { Append(v, mk('TT_QLIT', '@')); ptype[v] = '@'; } Push(v); ParamType = .dummy; nreturn; }
ptype = TABLE();
pdef_v = ARRAY('1:64'); pdef_d = ARRAY('1:64'); pdef_n = 0;
function ParamDefault(d, v) { d = Pop(); v = Top(); pdef_n = pdef_n + 1; pdef_v[pdef_n] = v; pdef_d[pdef_n] = d; ParamDefault = .dummy; nreturn; }
function ResetDefaults() { pdef_n = 0; ResetDefaults = .dummy; nreturn; }
function PushDefaults(i) { i = 0; while (LT(i, pdef_n)) { i = i + 1; Push(pdef_v[i]); Push(mk('TT_QLIT', 'defined')); Reduce('TT_METHCALL', 2); Push(pdef_v[i]); Push(pdef_d[i]); Reduce('TT_ASSIGN', 2); Reduce('TT_SEQ_EXPR', 1); Reduce('TT_UNLESS', 2); IncCounter(); } pdef_n = 0; PushDefaults = .dummy; nreturn; }
signature = FENCE( '(' *$' ' FENCE(*params | epsilon) *$' ' FENCE('-->' *$' ' *name *$' ' | epsilon) ')' *$' ' | epsilon );
hex2 = TABLE(257); hx_i = 0;
while (LT(hx_i, 256)) { hex2[SUBSTR(&ALPHABET, hx_i + 1, 1)] = SUBSTR('0123456789abcdef', hx_i / 16 + 1, 1) SUBSTR('0123456789abcdef', REMDR(hx_i, 16) + 1, 1); hx_i = hx_i + 1; }
function OpName(kind, op, out, i) { out = 'R' kind '_'; i = 0; while (LT(i, SIZE(op))) { i = i + 1; out = out hex2[SUBSTR(op, i, 1)]; } rt_nm = out; OpName = .dummy; nreturn; }
op_routine_nm = ( ('infix' | 'prefix' | 'postfix' | 'circumfix') . rk_tx ':' '<' BREAK('>') . rop_tx '>' . *OpName(rk_tx, rop_tx)
                | (('infix' | 'prefix' | 'postfix' | 'circumfix') ':' '«' BREAK('»') '»') . rt_nm );
routine  = ( FENCE(*op_routine_nm | FENCE('!' | epsilon) *name . rt_nm | epsilon . *NoName()) . *ResetDefaults() . *Sh('TT_VAR', rt_nm) *$' ' . *PushCounter() *signature . *PushVal(nTop()) . *PopCounter()
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
    if (GT(SIZE(Src), 0) IDENT(SUBSTR(Src, SIZE(Src), 1), nl)) Src = SUBSTR(Src, 1, SIZE(Src) - 1);
    pf_a = TIME();
    InitCounter();
    InitStack();
    dq_n = 0; dq_lit = 0; dq_buf = ''; lst_force = ; last_term = 0; destr_n = 0; fm_n = 0; pdef_n = 0; post_n = 0; twpost_n = 0;
    rx_node = TABLE(); rx_kind = TABLE(); bare_nm = TABLE(); paren_list = TABLE(); post_inc = TABLE(); named_pair = TABLE(); adverb_val = TABLE(); arr_tbl = TABLE(); ptype = TABLE(); caret_to = TABLE(); typeobj = TABLE();
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
