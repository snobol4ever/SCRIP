/* PST-ICN-SC ✅ 2026-05-19 — Expr11 pure shift/reduce; zero violations. */
/* ==================================================================================================================== */
/* a $directive ($define, $include, $ifdef ...) is a whole line and only at a line's start: $( $) $< $> are brackets */
white        =   (  SPAN(' ' CHAR(9))
                 |  CHAR(10) FENCE(SPAN(' ' CHAR(9)) | epsilon) FENCE('$' FENCE(SPAN(' ' CHAR(9)) | epsilon) ANY(&LCASE &UCASE) BREAK(CHAR(10)) | epsilon)
                 |  POS(0) FENCE(SPAN(' ' CHAR(9)) | epsilon) '$' FENCE(SPAN(' ' CHAR(9)) | epsilon) ANY(&LCASE &UCASE) BREAK(CHAR(10))
                 |  '#' BREAK(CHAR(10))
                 );
White        =   *white FENCE(*White | epsilon);
Gray         =   FENCE(*White | epsilon);
$' '         =   Gray;
$'  '        =   White;
Id           = ANY(&UCASE &LCASE '_') FENCE(SPAN('0123456789' &UCASE &LCASE '_') | epsilon);
reserved     = POS(0) ('break' | 'by' | 'case' | 'create' | 'default' | 'do' | 'else' | 'end' | 'every' | 'fail'
                     | 'global' | 'if' | 'initial' | 'invocable' | 'link' | 'local' | 'next' | 'not' | 'of'
                     | 'procedure' | 'record' | 'repeat' | 'return' | 'static' | 'suspend' | 'then' | 'to'
                     | 'until' | 'while') RPOS(0);
ResT         = TABLE(29);
ResT['break'] = 1; ResT['by'] = 1; ResT['case'] = 1; ResT['create'] = 1; ResT['default'] = 1; ResT['do'] = 1; ResT['else'] = 1; ResT['end'] = 1;
ResT['every'] = 1; ResT['fail'] = 1; ResT['global'] = 1; ResT['if'] = 1; ResT['initial'] = 1; ResT['invocable'] = 1; ResT['link'] = 1;
ResT['local'] = 1; ResT['next'] = 1; ResT['not'] = 1; ResT['of'] = 1; ResT['procedure'] = 1; ResT['record'] = 1; ResT['repeat'] = 1;
ResT['return'] = 1; ResT['static'] = 1; ResT['suspend'] = 1; ResT['then'] = 1; ResT['to'] = 1; ResT['until'] = 1; ResT['while'] = 1;
id_pat       = *Id $ tx *IDENT(ResT[tx]);
int_pat      = SPAN('0123456789') FENCE(ANY('rR') SPAN('0123456789' &UCASE &LCASE) | epsilon);
exp_part     = (('e' | 'E') ('+' | '-' | '') SPAN('0123456789'));
real_pat     = (( SPAN('0123456789') '.' (SPAN('0123456789') | '') | '.' SPAN('0123456789') ) (*exp_part | '')
               | SPAN('0123456789') *exp_part
               );
str_pat      = ('"' BREAK('"') . strbody '"');
cset_pat     = ("'" BREAK("'") . csetbody "'");
strchars     = ARBNO(NOTANY('"\_') | '\^' LEN(1) | '\' LEN(1) | '_' CHAR(10) FENCE(SPAN(' ' CHAR(9)) | epsilon) | '_');
csetchars    = ARBNO(NOTANY("'\_") | '\^' LEN(1) | '\' LEN(1) | '_' CHAR(10) FENCE(SPAN(' ' CHAR(9)) | epsilon) | '_');
/* a literal's value is its DECODED text, as the C lexer stores it (icon_lex.c scan_string/scan_cset, icn_esc_simple):   */
/* \x up to 2 hex digits, \^c control, up to 3 octal digits, the simple letters either case, any other char itself;   */
/* in a string (not a cset) an underscore ending the line continues it, the next line's leading blanks dropped.       */
IcnEscT = TABLE(32);
IcnEscT['b'] = bs;  IcnEscT['B'] = bs;  IcnEscT['d'] = CHAR(127); IcnEscT['D'] = CHAR(127); IcnEscT['e'] = CHAR(27); IcnEscT['E'] = CHAR(27);
IcnEscT['f'] = ff;  IcnEscT['F'] = ff;  IcnEscT['l'] = nl; IcnEscT['L'] = nl; IcnEscT['n'] = nl; IcnEscT['N'] = nl;
IcnEscT['r'] = cr;  IcnEscT['R'] = cr;  IcnEscT['t'] = tab; IcnEscT['T'] = tab; IcnEscT['v'] = vt; IcnEscT['V'] = vt;
IcnEscT['8'] = bs;  IcnEscT['9'] = tab;
function IcnDigits(d, base, v, c) {
    v = 0;
    while (d ? (POS(0) LEN(1) . c) = ) {
        hex_digits ? (BREAK(c) . c);
        v = v * base + (GT(SIZE(c), 15) SIZE(c) - 6, SIZE(c));
    }
    IcnDigits = v;
    return;
}
/* an integer literal's value as the C lexer computes it (icon_lex.c scan_number): decimal, or <radix>r<digits> with   */
/* 0-9 a-z A-Z as digit values; past 9223372036854775807 it is a large integer, TT_FNC (TT_VAR integer) (TT_QLIT text). */
IcnDigV = TABLE(64);
IcnDigI = 0;
while (IcnDigI = LT(IcnDigI, 36) IcnDigI + 1) {
    IcnDigV[SUBSTR('0123456789abcdefghijklmnopqrstuvwxyz', IcnDigI, 1)] = IcnDigI - 1;
    IcnDigV[SUBSTR('0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', IcnDigI, 1)] = IcnDigI - 1;
}
function IcnIntLit(x, radix, ds, txt, val, big, c, d) {
    IcnIntLit = .dummy;
    radix = 10; ds = x; txt = x;
    if (x ? (POS(0) SPAN(digits) . radix ANY('rR') REM . ds)) { radix = radix + 0; txt = radix 'r' ds; }
    val = 0; big = 0;
    while (ds ? (POS(0) LEN(1) . c) = ) {
        d = IcnDigV[c];
        if (EQ(big, 0)) {
            if (GT(val, (9223372036854775807 - d) / radix)) { big = 1; }
            else { val = val * radix + d; }
        }
    }
    if (EQ(big, 0)) { Shift('TT_ILIT', '' val); nreturn; }
    Shift('TT_VAR', 'integer');
    Shift('TT_QLIT', txt);
    Reduce('TT_FNC', 2);
    nreturn;
}
function IcnUnesc(s, cset, out, part, e, d, c) {
    if (~(s ? BREAK(bSlash '_'))) { IcnUnesc = s; return; }
    out = '';
    while (s ? (POS(0) BREAK(bSlash '_') . part LEN(1) . e) = ) {
        out = out part;
        if (IDENT(e, '_')) {
            if (DIFFER(cset)) { out = out '_'; }
            else if (s ? (POS(0) (cr | '') nl (SPAN(' ' tab) | '')) = ) { out = out; }
            else { out = out '_'; }
        } else if (s ? (POS(0) 'x' ((ANY(hex_digits) (ANY(hex_digits) | '')) | '') . d) = ) {
            out = out CHAR(IcnDigits(d, 16));
        } else if (s ? (POS(0) '^' LEN(1) . c) = ) {
            &ALPHABET ? (BREAK(c) . d);
            out = out CHAR(REMDR(SIZE(d), 32));
        } else if (s ? (POS(0) (ANY(oct_digits) (ANY(oct_digits) (ANY(oct_digits) | '') | '')) . d) = ) {
            out = out CHAR(REMDR(IcnDigits(d, 8), 256));
        } else if (s ? (POS(0) LEN(1) . c) = ) {
            d = IcnEscT[c];
            out = out (DIFFER(d) d, c);
        }
    }
    IcnUnesc = out s;
    return;
}
semi_opt     = FENCE(';' | epsilon);
$'if'        =  *$' ' *Id $ tx *IDENT(tx, 'if')       ;
$'then'      =  *$' ' *Id $ tx *IDENT(tx, 'then')     ;
$'else'      =  *$' ' *Id $ tx *IDENT(tx, 'else')     ;
$'while'     =  *$' ' *Id $ tx *IDENT(tx, 'while')    ;
$'do'        =  *$' ' *Id $ tx *IDENT(tx, 'do')       ;
$'every'     =  *$' ' *Id $ tx *IDENT(tx, 'every')    ;
$'return'    =  *$' ' *Id $ tx *IDENT(tx, 'return')   ;
$'end'       =  *$' ' *Id $ tx *IDENT(tx, 'end')      ;
$'procedure' =  *$' ' *Id $ tx *IDENT(tx, 'procedure');
$'until'     =  *$' ' *Id $ tx *IDENT(tx, 'until')    ;
$'repeat'    =  *$' ' *Id $ tx *IDENT(tx, 'repeat')   ;
$'break'     =  *$' ' *Id $ tx *IDENT(tx, 'break')    ;
$'next'      =  *$' ' *Id $ tx *IDENT(tx, 'next')     ;
$'case'      =  *$' ' *Id $ tx *IDENT(tx, 'case')     ;
$'of'        =  *$' ' *Id $ tx *IDENT(tx, 'of')       ;
$'default'   =  *$' ' *Id $ tx *IDENT(tx, 'default')  ;
$'to'        =  *$' ' *Id $ tx *IDENT(tx, 'to')       ;
$'by'        =  *$' ' *Id $ tx *IDENT(tx, 'by')       ;
$'global'    =  *$' ' *Id $ tx *IDENT(tx, 'global')   ;
$'local'     =  *$' ' *Id $ tx *IDENT(tx, 'local')    ;
$'static'    =  *$' ' *Id $ tx *IDENT(tx, 'static')   ;
$'record'    =  *$' ' *Id $ tx *IDENT(tx, 'record')   ;
$'initial'   =  *$' ' *Id $ tx *IDENT(tx, 'initial')  ;
$'suspend'   =  *$' ' *Id $ tx *IDENT(tx, 'suspend')  ;
$'fail'      =  *$' ' *Id $ tx *IDENT(tx, 'fail')     ;
$'not'       =  *$' ' *Id $ tx *IDENT(tx, 'not')      ;
$'create'    =  *$' ' *Id $ tx *IDENT(tx, 'create')   ;
$'link'      =  *$' ' *Id $ tx *IDENT(tx, 'link')     ;
$'invocable' =  *$' ' *Id $ tx *IDENT(tx, 'invocable');
$'('        =   *$' ' '(' *$' ';
$'['        =   *$' ' ('[' | '$<') *$' ';
$'{'        =   *$' ' ('{' | '$(') *$' ';
$')'        =   *$' ' ')';
$']'        =   *$' ' (']' | '$>');
$'}'        =   *$' ' ('}' | '$)');
$','        =   *$' ' ','   *$' ';
$';'        =   *$' ' ';'   *$' ';
$':'        =   *$' ' ':'   *$' ';
$'.'        =   *$' ' '.'   *$' ';
$'|||'      =   *$' ' '|||'   *$' ';
$'||'       =   *$' ' '||'    *$' ';
$'|'        =   *$' ' '|'     *$' ';
$'++'       =   *$' ' '++'    *$' ';
$'--'       =   *$' ' '--'    *$' ';
$'**'       =   *$' ' '**'    *$' ';
$'+'        =   *$' ' '+'     *$' ';
$'-'        =   *$' ' '-'     *$' ';
$'*'        =   *$' ' '*'     *$' ';
$'/'        =   *$' ' '/'     *$' ';
$'%'        =   *$' ' '%'     *$' ';
$'^'        =   *$' ' '^'     *$' ';
$'?'        =   *$' ' '?'     *$' ';
$'~'        =   *$' ' '~'     *$' ';
$'!'        =   *$' ' '!'     *$' ';
$'@'        =   *$' ' '@'     *$' ';
$'&'        =   *$' ' '&'     *$' ';
$'\\'      =   *$' ' '\'     *$' ';
$'~==='     =   *$' ' '~==='  *$' ';
$'~=='      =   *$' ' '~=='   *$' ';
$'~='       =   *$' ' '~='    *$' ';
$'==='      =   *$' ' '==='   *$' ';
$'=='       =   *$' ' '=='    *$' ';
$'='        =   *$' ' '='     *$' ';
$'<='       =   *$' ' '<='    *$' ';
$'>='       =   *$' ' '>='    *$' ';
$'<<='      =   *$' ' '<<='   *$' ';
$'<<'       =   *$' ' '<<'    *$' ';
$'>>='      =   *$' ' '>>='   *$' ';
$'>>'       =   *$' ' '>>'    *$' ';
$'<'        =   *$' ' '<' @lt_a FENCE(ANY('-=<') | epsilon) @lt_b *EQ(lt_a, lt_b) *$' ';
$'>'        =   *$' ' '>' @gt_a FENCE(ANY('=>')  | epsilon) @gt_b *EQ(gt_a, gt_b)  *$' ';
$':=:'      =   *$' ' ':=:'   *$' ';
$':='       =   *$' ' ':='    *$' ';
$'+:'       =   *$' ' '+:'    *$' ';
$'-:'       =   *$' ' '-:'    *$' ';
$'<->'      =   *$' ' '<->'   *$' ';
$'<-'       =   *$' ' '<-'    *$' ';
$'~==:='    =   *$' ' '~==:=' *$' ';
$'~=:='     =   *$' ' '~=:='  *$' ';
$'<<=:='    =   *$' ' '<<=:=' *$' ';
$'<<:='     =   *$' ' '<<:='  *$' ';
$'>>=:='    =   *$' ' '>>=:=' *$' ';
$'>>:='     =   *$' ' '>>:='  *$' ';
$'==:='     =   *$' ' '==:='  *$' ';
$'<=:='     =   *$' ' '<=:='  *$' ';
$'>=:='     =   *$' ' '>=:='  *$' ';
$'<:='      =   *$' ' '<:='   *$' ';
$'>:='      =   *$' ' '>:='   *$' ';
$'+:='      =   *$' ' '+:='   *$' ';
$'-:='      =   *$' ' '-:='   *$' ';
$'*:='      =   *$' ' '*:='   *$' ';
$'/:='      =   *$' ' '/:='   *$' ';
$'%:='      =   *$' ' '%:='   *$' ';
$'^:='      =   *$' ' '^:='   *$' ';
$'||:='     =   *$' ' '||:='  *$' ';
$'++:='     =   *$' ' '++:='  *$' ';
$'--:='     =   *$' ' '--:='  *$' ';
$'**:='     =   *$' ' '**:='  *$' ';
$'?:='      =   *$' ' '?:='   *$' ';
$'=:='      =   *$' ' '=:='   *$' ';
$'@:='      =   *$' ' '@:='   *$' ';
$'&:='      =   *$' ' '&:='   *$' ';
$'|||:='    =   *$' ' '|||:=' *$' ';
$'~===:='   =   *$' ' '~===:=' *$' ';
$'===:='    =   *$' ' '===:=' *$' ';
/* ==================================================================================================================== */
/* Leaf-push helpers: allowed by PST rules — set v.sval/v.dval from token capture, no child inspection. */
/* ==================================================================================================================== */
If     = ( *$'if' *If_rest );
If_rest          = (      *$'  ' *Expr  *$'then' *$' ' *Expr
           (  *$'else' *$' ' *Expr  . *Reduce('TT_IF', 3)
           |  epsilon . *Reduce('TT_IF', 2)
           )
         );
While  = ( *$'while' *While_rest );
While_rest       = (   *$'  ' *Expr
           (  *$'do' *$' ' *Expr  . *Reduce('TT_WHILE', 2)
           |  epsilon . *Reduce('TT_WHILE', 1)
           )
         );
Until  = ( *$'until' *Until_rest );
Until_rest       = (   *$'  ' *Expr
           (  *$'do' *$' ' *Expr  . *Reduce('TT_UNTIL', 2)
           |  epsilon . *Reduce('TT_UNTIL', 1)
           )
         );
Every  = ( *$'every' *Every_rest );
Every_rest       = (   *$' ' *Expr
           (  *$'do' *$' ' *Expr  . *Reduce('TT_EVERY', 2)
           |  epsilon . *Reduce('TT_EVERY', 1)
           )
         );
Repeat = ( *$'repeat' *Repeat_rest );
Repeat_rest      = (  *$' ' *Expr  . *Reduce('TT_REPEAT', 1) );
Create = ( *$'create' *Create_rest );
Create_rest      = (  *$' ' *Expr  . *Reduce('TT_CREATE', 1) );
ArgFirst  = ( *$' ' *Expr  . *IncCounter() );
/* an omitted argument f(a, , b) is &null, the C frontend's own leaf                                */
ArgRest   = ( *$',' (*Expr | epsilon . *Shift('TT_VAR', '&null')) . *IncCounter() );
NullFirst = ( *$' ' . *Shift('TT_VAR', '&null') . *IncCounter() *ArgRest );
CallArgs  = ( *ArgFirst ARBNO(*ArgRest) | *NullFirst ARBNO(*ArgRest) | epsilon );
Call      = ( epsilon . *PushCounter()
              *$' ' (*id_pat) . thx . *Shift('TT_VAR', thx)  . *IncCounter()
              *$'(' *CallArgs *$')'
              . *Reduce('TT_FNC', nTop())
              . *PopCounter()
            );
ConjRest  = ( *$',' (*Expr | epsilon . *Reduce('TT_NUL', 0)) . *IncCounter() );
/* (e1, e2) is mutual evaluation, TT_CONJ; a ';' inside parentheses is refused, as icont refuses it. */
Paren     = ( epsilon . *PushCounter()
              ( *$' ' *$'(' *Expr  . *IncCounter()
                ( *ConjRest ARBNO(*ConjRest) . *Reduce('TT_CONJ', nTop())
                | epsilon
                )
                *$')'
              | *$' ' *$'(' *$')' . *Reduce('TT_SEQ_EXPR', 0)
              | *$' ' *$'(' . *Reduce('TT_NUL', 0) . *IncCounter() *ConjRest ARBNO(*ConjRest) . *Reduce('TT_CONJ', nTop()) *$')'
              )
              . *PopCounter()
            );
/* { s1; s2 } is C's parse_block_or_expr: each statement may take one trailing ';', a bare ';' is the leaf &null, and */
/* when the token before '}' is a ';' one more &null closes the block; {} is an empty TT_SEQ_EXPR, one item stands alone. */
/* The ';' is tracked by the deferred actions themselves (IcnSemi), which run in match order, inner blocks first.         */
function IcnSemi(f) { icnSemiFlag = f; IcnSemi = .dummy; nreturn; }
function IcnBlockEnd() {
    IcnBlockEnd = .dummy;
    if (IDENT(icnSemiFlag, 1)) { Shift('TT_VAR', '&null'); IncCounter(); }
    nreturn;
}
CompoundItem  = FENCE( *$' ' *$';' . *Shift('TT_VAR', '&null') . *IncCounter() . *IcnSemi(1)
                     | *$' ' *Expr . *IncCounter() *$' ' FENCE( *$';' . *IcnSemi(1) | epsilon . *IcnSemi(0) ) );
CompoundItems = FENCE( *CompoundItem *CompoundItems | epsilon );
Compound      = ( epsilon . *PushCounter()
                  *$'{'
                  ( *$' ' *$'}' | epsilon . *IcnSemi(0) *CompoundItems *$' ' *$'}' . *IcnBlockEnd() )
                  . *Reduce('TT_SEQ_EXPR', *(DIFFER(nTop(), 1) nTop()))
                  . *PopCounter()
                );
ListFirst = ( *$' ' *Expr  . *IncCounter() );
ListRest  = ( *$',' (*Expr | epsilon . *Shift('TT_NUL', '')) . *IncCounter() );
NullListFirst = ( *$' ' . *Shift('TT_NUL', '') . *IncCounter() *ListRest );
ListCtor  = ( epsilon . *PushCounter()
              *$' ' *$'['
              ( *ListFirst ARBNO(*ListRest) | *NullListFirst ARBNO(*ListRest) | epsilon )
              *$']'
              . *Reduce('TT_MAKELIST', nTop())
              . *PopCounter()
            );
/* FieldTail: shift field name as TT_VAR (source order: object already on stack below),
   then reduce('TT_FIELD', 2) gives children [object, TT_VAR(name)] in source order. */
FieldTail   = ( *$'.' (*id_pat) . thx . *Shift('TT_VAR', thx) . *Reduce('TT_FIELD', 2) );
/* any primary may be invoked: (!p)(), 1(a, b), f(x) -- the callee is already on the stack.    */
IdxStar     = FENCE( *$',' *Expr . *IncCounter() *IdxStar | epsilon );
/* f{e1, e2} invokes f with a list of co-expressions: TT_FNC(f, TT_MAKELIST(TT_CREATE e1, ...))      */
CoArg       = ( *$' ' *Expr . *Reduce('TT_CREATE', 1) . *IncCounter() );
Expr11tail  = ( epsilon . *PushCounter() *$'(' *CallArgs *$')' . *Reduce('TT_FNC', nTop() + 1) . *PopCounter()
              | epsilon . *PushCounter() *$'{' ( *CoArg ARBNO(*$',' *CoArg) | epsilon ) *$'}' . *Reduce('TT_MAKELIST', nTop()) . *Reduce('TT_FNC', 2) . *PopCounter()
              | epsilon . *PushCounter() . *IncCounter() *$'['
                ( *Expr . *IncCounter()
                  FENCE( *$' ' ('+:' *$' ' *Expr *$']' . *Reduce('TT_SECTION_PLUS', 3)
                       | '-:' *$' ' *Expr *$']' . *Reduce('TT_SECTION_MINUS', 3)
                       | ':' *$' '  *Expr *$']' . *Reduce('TT_SECTION', 3)
                       ) | *IdxStar *$']'   . *Reduce('TT_IDX', nTop())
                       )
                | epsilon . *Shift('TT_VAR', '&null') . *IncCounter() *$']' . *Reduce('TT_IDX', nTop())
                )
                . *PopCounter()
              | *FieldTail
              );
/* the blanks inside a case take the greedy form too: each clause sits in a FENCE, so a shortest-first  */
/* CaseGray that stopped before ` ;` could never be re-entered to take it                                */
CaseGray     = (*White | epsilon);
CaseClause   = ( *CaseGray *Expr *CaseGray *$':' *Expr *CaseGray . *IncCounter() . *IncCounter() );
CaseDefault  = ( *CaseGray *$'default' . *Reduce('TT_NUL', 0) . *IncCounter() *CaseGray *$':' *Expr *CaseGray . *IncCounter() );
CaseItem     = FENCE(*CaseDefault | *CaseClause);
CaseTail     = ( *$';' *CaseItem *CaseTail | epsilon );
Case         = ( *$'case' *Case_rest );
Case_rest        = ( epsilon . *PushCounter()
                  *$' ' *Expr  . *IncCounter()
                 *$'of' *CaseGray *$'{' *CaseGray
                 FENCE( *CaseItem *CaseTail | epsilon )
                 *CaseGray *$'}'
                 . *Reduce('TT_CASE', nTop())
                 . *PopCounter()
               );
/* return and suspend are expressions too (a | return b): DEFERRED, because they are defined below   */
/* and a by-value reference here would be the empty pattern, which matches everywhere (measured)     */
break_rest   = FENCE( *$' ' *Expr . *Reduce('TT_LOOP_BREAK', 1) | *$' ' . *Reduce('TT_LOOP_BREAK', 0) );
next_rest    = *$' '  . *Reduce('TT_LOOP_NEXT', 0);
fail_rest    = *$' '  . *Reduce('TT_PROC_FAIL', 0);
KwT          = TABLE(12);
kw_expr      = *$' ' *Id $ tx *DIFFER(KwT[tx]) *KwT[tx];
Expr11 = (   *kw_expr
         |   *ListCtor
         |   *Call  |  *Paren  |  *Compound
         |   *$' ' "'" (*csetchars) . thx . *Shift('TT_CSET', IcnUnesc(thx, 1)) "'"
         |   *$' ' '"' (*strchars) . thx . *Shift('TT_QLIT', IcnUnesc(thx)) '"'
         |   *$' ' (*real_pat) . thx . *Shift('TT_FLIT', thx)
         |   *$' ' (*int_pat) . thx . *IcnIntLit(thx)
         |   *$' ' ('&' *Id) . thx . *Shift('TT_VAR', thx)
         |   *$' ' (*id_pat) . thx . *Shift('TT_VAR', thx)
         );
Expr10 = (   *$' ' ('-' *$' '        *Expr10 . *Reduce('TT_MNS', 1)
         |   '+' *$' '        *Expr10 . *Reduce('TT_PLS', 1)
         |   '~' *$' '        *Expr10 . *Reduce('TT_CSET_COMPL', 1)
         ) |   *$'\\'       *Expr10 . *Reduce('TT_NONNULL', 1)
         |   *$' ' ('!' *$' '        *Expr10 . *Reduce('TT_ITERATE', 1)
         |   '*' *$' '        *Expr10 . *Reduce('TT_SIZE', 1)
         |   '?' *$' '        *Expr10 . *Reduce('TT_RANDOM', 1)
         |   '/' *$' '        *Expr10 . *Reduce('TT_NULL', 1)
         |   '=' . *Shift('TT_VAR', 'tab') . *Shift('TT_VAR', 'match') *$' ' *Expr10 . *Reduce('TT_FNC', 2) . *Reduce('TT_FNC', 2)
         ) |   *$'not' *$' ' *Expr10 . *Reduce('TT_NOT', 1)
         |   *$' ' ('|' *$' '        *Expr10 . *Reduce('TT_REPALT', 1)
         |   '@' *$' '        *Expr10 . *Reduce('TT_ACTIVATE', 1)
         |   '^' *$' ' . *PushCounter() . *Shift('TT_VAR', 'ICN$REFRESH') . *IncCounter() *Expr10 . *IncCounter() . *Reduce('TT_FNC', nTop()) . *PopCounter()
         ) |   *Expr11  *Expr11rest
         |   *$'.'        *Expr10 . *Reduce('TT_DEREF', 1)
         );
Expr11rest = FENCE(*Expr11tail *Expr11rest | epsilon);
Expr9tail = FENCE( *$'\\' *Expr10 . *Reduce('TT_LIMIT', 2)
                 | *$' ' ('!' *$' '  *Expr10 . *Reduce('TT_BANG_BINARY', 2)
                 | '@' *$' '  *Expr10 . *Reduce('TT_ACTIVATE', 2)
                 ));
Expr9     = ( *Expr10 *Expr9rest );
Expr9rest = FENCE(*Expr9tail *Expr9rest | epsilon);
Expr8     = ( *Expr9 FENCE(*$'^' *Expr8 . *Reduce('TT_POW', 2) | epsilon) );
Expr7tail = FENCE( *$' ' ('**' *$' ' *Expr8 . *Reduce('TT_CSET_INTER', 2)
                 | '*' *$' '  *Expr8 . *Reduce('TT_MUL', 2)
                 | '/' *$' '  *Expr8 . *Reduce('TT_DIV', 2)
                 | '%' *$' '  *Expr8 . *Reduce('TT_MOD', 2)
                 ));
Expr7     = ( *Expr8 *Expr7rest );
Expr7rest = FENCE(*Expr7tail *Expr7rest | epsilon);
Expr6tail = FENCE( *$' ' ('++' *$' ' *Expr7 . *Reduce('TT_CSET_UNION', 2)
                 | '--' *$' ' *Expr7 . *Reduce('TT_CSET_DIFF', 2)
                 | '+' *$' '  *Expr7 . *Reduce('TT_ADD', 2)
                 | '-' *$' '  *Expr7 . *Reduce('TT_SUB', 2)
                 ));
Expr6     = ( *Expr7 *Expr6rest );
Expr6rest = FENCE(*Expr6tail *Expr6rest | epsilon);
Expr5tail = FENCE( *$' ' ('|||' *$' ' *Expr6 . *Reduce('TT_LCONCAT', 2) | '||' *$' ' *Expr6 . *Reduce('TT_CAT', 2) ));
Expr5     = ( *Expr6 *Expr5rest );
Expr5rest = FENCE(*Expr5tail *Expr5rest | epsilon);
Expr4tail = FENCE( *$' ' ('<<=' *$' '  *Expr5 . *Reduce('TT_LLE', 2) | '<<' *$' '   *Expr5 . *Reduce('TT_LLT', 2)
                 | '>>=' *$' '  *Expr5 . *Reduce('TT_LGE', 2) | '>>' *$' '   *Expr5 . *Reduce('TT_LGT', 2)
                 | '~===' *$' ' *Expr5 . *Reduce('TT_NIDENTICAL', 2)
                 | '~==' *$' '  *Expr5 . *Reduce('TT_LNE', 2)
                 | '===' *$' '  *Expr5 . *Reduce('TT_IDENTICAL', 2)
                 | '==' *$' '   *Expr5 . *Reduce('TT_LEQ', 2)
                 | '<=' *$' '   *Expr5 . *Reduce('TT_LE', 2) | '>=' *$' '   *Expr5 . *Reduce('TT_GE', 2)
                 | '~=' *$' '   *Expr5 . *Reduce('TT_NE', 2) ) | *$'<'    *Expr5 . *Reduce('TT_LT', 2)
                 | *$'>'    *Expr5 . *Reduce('TT_GT', 2) | *$'='    *Expr5 . *Reduce('TT_EQ', 2)
                 );
Expr4     = ( *Expr5 *Expr4rest );
Expr4rest = FENCE(*Expr4tail *Expr4rest | epsilon);
X3        = ( epsilon . *IncCounter() *Expr4 FENCE(*$'|' *X3 | epsilon) );
Expr3     = ( epsilon . *PushCounter() *X3 . *Reduce('TT_ALTERNATE', *(GT(nTop(), 1) nTop())) . *PopCounter() );
ToStar    = FENCE(  *$'to' *$'  ' *Expr3
                    FENCE( *$'by' *$'  ' *Expr3 . *Reduce('TT_TO_BY', 3)
                         | epsilon . *Reduce('TT_TO', 2)
                         )
                    *ToStar
                 |  epsilon
                 );
Expr2     = ( *Expr3 *ToStar );
Expr1     = ( *Expr2
              FENCE(
                  *$' ' ('|||:=' *$' ' *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_LCONCAT')
              |   '~===:=' *$' ' *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_NIDENTICAL')
              |   '===:=' *$' ' *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_IDENTICAL')
              |   '<<=:=' *$' ' *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_LLE')
              |   '>>=:=' *$' ' *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_LGE')
              |   '~==:=' *$' ' *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_LNE')
              |   '<=:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_LE')
              |   '>=:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_GE')
              |   '~=:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_NE')
              |   '==:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_LEQ')
              |   '<<:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_LLT')
              |   '>>:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_LGT')
              |   '||:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_CAT')
              |   '++:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_CSET_UNION')
              |   '--:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_CSET_DIFF')
              |   '**:=' *$' '  *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_CSET_INTER')
              |   '+:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_ADD')
              |   '-:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_SUB')
              |   '*:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_MUL')
              |   '/:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_DIV')
              |   '%:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_MOD')
              |   '^:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_POW')
              |   '?:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   '=:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_EQ')
              |   '@:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_ACTIVATE')
              |   '&:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_CONJ')
              |   '<:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_LT')
              |   '>:=' *$' '   *Expr1 . *Reduce('TT_AUGOP', 2, 'TT_GT')
              |   ':=:' *$' '   *Expr1 . *Reduce('TT_SWAP', 2)
              |   '<->' *$' '   *Expr1 . *Reduce('TT_REVSWAP', 2)
              |   '<-' *$' '    *Expr1 . *Reduce('TT_REVASSIGN', 2)
              |   ':=' *$' '    *Expr1 . *Reduce('TT_ASSIGN', 2)
              ) |   epsilon
              )
            );
ReturnExpr  = ( *$'return' *ReturnExpr_rest );
ReturnExpr_rest  = ( epsilon . *PushCounter()
                 *$' ' *Expr . *IncCounter()  . *Reduce('TT_RETURN', 1) . *PopCounter()
              |  *$' '                   . *Reduce('TT_RETURN', 0)
              );
SuspendExpr = ( *$'suspend' *SuspendExpr_rest );
SuspendExpr_rest = ( epsilon . *PushCounter()
                (  *$' ' *Expr . *IncCounter()
                  FENCE( *$'do' *$'  ' *Expr . *IncCounter() | epsilon )
                |  *$' ' . *Shift('TT_VAR', '&null') . *IncCounter()
                )
                . *Reduce('TT_SUSPEND', nTop()) . *PopCounter()
              );
Expr1a    = ( *Expr1 FENCE(*$'?' *Expr1a . *Reduce('TT_SCAN', 2) | epsilon) );
ExprSeqRest = ( *$'&' ( *ReturnExpr | *SuspendExpr | *Expr1a ) . *IncCounter() );
ExprSeqStar = FENCE(*ExprSeqRest *ExprSeqStar | epsilon);
Expr        = ( epsilon . *PushCounter()
                ( *ReturnExpr | *SuspendExpr
                | *Expr1a
                )
                . *IncCounter() *ExprSeqStar . *Reduce('TT_CONJ', *(GT(nTop(), 1) nTop())) . *PopCounter()
              );
Blank     = ( *$' ' );
ReturnStmt = ( *$'return' *$' ' *Expr *$' ' *$';' *$' ' . *Reduce('TT_RETURN', 1)
             | *$'return' *$' '  *$';' *$' '             . *Reduce('TT_RETURN', 0)
             );
DeclFirst  = ( *$' ' (*id_pat) . thx . *Shift('TT_VAR', thx) . *IncCounter() );
DeclRest   = ( *$','  (*id_pat) . thx . *Shift('TT_VAR', thx) . *IncCounter() );
DeclStar   = FENCE(*DeclRest *DeclStar | epsilon);
DeclIds    = ( *DeclFirst *DeclStar );
/* LocalDecl: collect var names, reduce to TT_LOCAL node, push bare (no STMT wrap). */
LocalDecl  = ( epsilon . *PushCounter() *$'local'  *$'  ' *DeclIds *$' ' *$';' *$' ' . *Reduce('TT_LOCAL', nTop()) . *PopCounter() );
StaticDecl = ( epsilon . *PushCounter() *$'static' *$'  ' *DeclIds *$' ' *$';' *$' ' . *Reduce('TT_STATIC_DECL', nTop()) . *PopCounter() );
InitialStmt = ( epsilon . *PushCounter() *$'initial' *$' ' *Expr . *IncCounter() *$' ' *$';' *$' '
                . *Reduce('TT_INITIAL', nTop())
                . *PopCounter()
              );
SuspendStmt = ( epsilon . *PushCounter()
                ( *$'suspend' *$' ' *Expr . *IncCounter()
                  FENCE( *$'do' *$'  ' *Expr . *IncCounter() | epsilon )
                | *$'suspend' *$' ' . *Shift('TT_VAR', '&null') . *IncCounter()
                )
                *$' ' *$';' *$' '
                . *Reduce('TT_SUSPEND', nTop()) . *PopCounter()
              );
FailStmt    = ( *$'fail'    *$' '         *$';' *$' '      . *Reduce('TT_PROC_FAIL', 0) );
StmtBody  = ( *LocalDecl . *IncCounter()
            | *StaticDecl . *IncCounter()
            | *InitialStmt . *IncCounter()
            | *ReturnStmt . *IncCounter()
            | *SuspendStmt . *IncCounter()
            | *FailStmt . *IncCounter()
            | *$' ' *Expr *$' ' *$';' *$' ' . *IncCounter()
            );
ParamFirst = ( *$' ' (*id_pat) . thx . *Shift('TT_VAR', thx)  . *IncCounter() );
ParamRest  = ( *$',' (*id_pat) . thx . *Shift('TT_VAR', thx)  . *IncCounter() );
Params     = ( *ParamFirst ARBNO(*ParamRest) FENCE(*$'[' *$']' | epsilon) | epsilon );
Prochead   = ( *$'procedure' *$'  ' (*id_pat) . icnProcNm . *Shift('TT_VAR', icnProcNm)  . *IncCounter()
               *$'(' epsilon . *PushCounter() *Params *$')' . *Reduce('TT_VLIST', nTop()) . *PopCounter() . *IncCounter() *$' ' *semi_opt *$' '
             );
ProcbodyEnd = ( *$'end' *$' ' (*$' ' | RPOS(0)) );
/* a statement, once matched, is never re-matched when a later one refuses: the refusal is linear  */
Procbody    = ( *ProcbodyEnd | FENCE(*StmtBody) *Procbody );
/* Proc: the C frontend's shape -- TT_PROC_DECL <name> (TT_VAR name) (TT_VLIST params) (TT_PROGRAM stmts), in TT_ATTR :subj, in TT_STMT. */
Proc        = ( epsilon . *PushCounter()  *Prochead  epsilon . *PushCounter() *Procbody . *Reduce('TT_PROGRAM', nTop()) . *PopCounter() . *IncCounter()
                . *Reduce('TT_PROC_DECL', nTop(), icnProcNm) . *Reduce('TT_ATTR', 1, ':subj') . *Reduce('TT_STMT', 1)
                . *PopCounter() FLUSH
              );
/* GlobalDecl: collect var names; reduce to TT_GLOBAL; wrap in TT_ATTR :subj then TT_STMT. */
GlobalDecl = ( epsilon . *PushCounter() *$'global' *$'  ' *DeclIds *$' ' *semi_opt *$' '
               . *Reduce('TT_GLOBAL', nTop()) . *Reduce('TT_ATTR', 1, ':subj') . *Reduce('TT_STMT', 1)
               . *PopCounter()
             );
RecordField = ( *$',' (*id_pat) . thx . *Shift('TT_VAR', thx) . *IncCounter() );
/* Record: the C frontend's shape -- TT_RECORD <name> with the fields as children; wrap in TT_ATTR :subj then TT_STMT. */
Record      = ( epsilon . *PushCounter()
                *$'record' *$'  ' (*id_pat) . icnRecNm
                *$'(' ( *$' ' (*id_pat) . thx . *Shift('TT_VAR', thx) . *IncCounter() ARBNO(*RecordField) | epsilon ) *$')'
                *$' '
                . *Reduce('TT_RECORD', nTop(), icnRecNm) . *Reduce('TT_ATTR', 1, ':subj') . *Reduce('TT_STMT', 1)
                . *PopCounter()
              );
KwT['if'] = If_rest; KwT['until'] = Until_rest; KwT['while'] = While_rest; KwT['every'] = Every_rest; KwT['repeat'] = Repeat_rest;
KwT['case'] = Case_rest; KwT['create'] = Create_rest; KwT['return'] = ReturnExpr_rest; KwT['suspend'] = SuspendExpr_rest;
KwT['break'] = break_rest; KwT['next'] = next_rest; KwT['fail'] = fail_rest;
/* link a, "b" and invocable all, "+": names as TT_VAR leaves, a quoted name too (the C frontend's shape). */
LinkName    = ( ( *$' ' '"' (BREAK('"')) . thx . *Shift('TT_VAR', thx) '"' | *$' ' (*id_pat) . thx . *Shift('TT_VAR', thx) ) FENCE(*$':' SPAN('0123456789') | epsilon) . *IncCounter() );
LinkStar    = FENCE( *$',' *LinkName *LinkStar | epsilon );
LinkDecl    = ( epsilon . *PushCounter() *$'link' *$'  ' *LinkName *LinkStar *$' ' *semi_opt *$' '
                . *Reduce('TT_LINK', nTop()) . *Reduce('TT_ATTR', 1, ':subj') . *Reduce('TT_STMT', 1)
                . *PopCounter()
              );
InvocableDecl = ( epsilon . *PushCounter() *$'invocable' *$'  ' *LinkName *LinkStar *$' ' *semi_opt *$' '
                . *Reduce('TT_INVOCABLE', nTop()) . *Reduce('TT_ATTR', 1, ':subj') . *Reduce('TT_STMT', 1)
                . *PopCounter()
              );
TopStar   = FENCE( epsilon . *IncCounter() *$' ' (*GlobalDecl | *Record | *Proc | *LinkDecl | *InvocableDecl) FLUSH *$' ' *TopStar | epsilon );
Compiland = ( epsilon . *PushCounter()
              POS(0) *$' ' *TopStar RPOS(0)
              . *Reduce('Parse', nTop())
              . *PopCounter()
            );
/* ==================================================================================================================== */
/* Preprocess: text to text. The macro pre-pass of the language (Icon: $define $undef $ifdef $ifndef $else $endif $include $line $error, */
/* the table of ipp.icn, JCON's preprocessor) generates the compiland text; the Compiland pattern above parses ONLY that text (Lon 2026-10-03, CEO-1483). */
struct ppdef { pv }
/* ==================================================================================================================== */
/* the patterns are built the first time a source carries a directive: a program with none runs the Compiland alone, as before */
function PPPat() {
    pp_spc   = ' ' CHAR(9) CHAR(12) CHAR(13);
    pp_nl    = CHAR(10);
    pp_bs    = CHAR(92);
    pp_sp    = '"' "'" '#' CHAR(10);
    pp_lit   = ( '"' ARBNO(pp_bs LEN(1) | '_' pp_nl | NOTANY('"' pp_bs pp_nl)) '"'
               | "'" ARBNO(pp_bs LEN(1) | '_' pp_nl | NOTANY("'" pp_bs pp_nl)) "'" );
    pp_cmt   = '#' FENCE(BREAK(pp_nl) | REM);
    pp_tok   = ( pp_lit | pp_cmt | NOTANY(pp_sp) FENCE(BREAK(pp_sp) | REM) | ANY('"' "'") );
    pp_end   = ( FENCE(SPAN(pp_spc) | epsilon) ( RPOS(0) | '#' ) );
    pp_dir   = ( FENCE(SPAN(pp_spc) | epsilon) '$' ( ANY(&UCASE &LCASE '0123456789' pp_spc) FENCE(BREAK(pp_nl) | REM) ) . pp_dl ( pp_nl | RPOS(0) ) . pp_nc
                 . *PPDir(pp_dl, pp_nc) );
    pp_txt   = ( ( pp_tok ARBNO(pp_tok) ) . pp_tl ( pp_nl | RPOS(0) ) . pp_nc . *PPTxt(pp_tl, pp_nc)
               | pp_nl . pp_nc . *PPTxt('', pp_nc) );
    pp_need  = ( ( POS(0) | CHAR(10) ) FENCE(SPAN(pp_spc) | epsilon) '$' ANY(&UCASE &LCASE '0123456789' pp_spc)
               | '_V9' | '_UNIX' | '_ASCII' | '_CO_EXPRESSIONS' | '_DYNAMIC_LOADING' | '_EXTERNAL_VALUES' | '_KEYBOARD_FUNCTIONS' | '_LARGE_INTEGERS' | '_PIPES' | '_SYSTEM_FUNCTION' );
    Preprocess = ( POS(0) ARBNO(FENCE(pp_dir | pp_txt)) RPOS(0) );
    pp_ready = 1;
    return;
}
/* ==================================================================================================================== */
function PPInit(w, s) {
    if (IDENT(pp_ready)) { PPPat(); }
    pp_sym = TABLE(31);
    pp_gd = TABLE(31);
    pp_inc = TABLE(31);
    pp_stk = TABLE(31);
    pp_n = 0;
    pp_sn = 0;
    pp_base = 0;
    pp_skip = '';
    pp_err = '';
    pp_out = '';
    pp_buf = '';
    s = '_V9 _UNIX _ASCII _CO_EXPRESSIONS _DYNAMIC_LOADING _EXTERNAL_VALUES _KEYBOARD_FUNCTIONS _LARGE_INTEGERS _PIPES _SYSTEM_FUNCTION ';
    while (s ? (POS(0) BREAK(' ') . w ' ') = ) {
        pp_sym[w] = ppdef('1');
        pp_n = pp_n + 1;
    }
    return;
}
/* ==================================================================================================================== */
function PPErr(msg) {
    TERMINAL = 'Preprocess: $' pp_cmd ': ' msg;
    pp_err = 1;
    return;
}
/* ==================================================================================================================== */
function PPEmit(s) {
    pp_buf = pp_buf s;
    if (GT(SIZE(pp_buf), 2048)) { pp_out = pp_out pp_buf; pp_buf = ''; }
    return;
}
/* ==================================================================================================================== */
function PPTxt(tl, nc) {
    PPTxt = .dummy;
    if (IDENT(pp_skip)) {
        if (GT(pp_n, 0)) { PPEmit(PPSubst(tl) nc); } else { PPEmit(tl nc); }
    } else { PPEmit(nc); }
    nreturn;
}
/* ==================================================================================================================== */
function PPSubst(s, w, d, out, r) {
    out = '';
    while (DIFFER(s)) {
        if (s ? (POS(0) (pp_lit | pp_cmt) . w) = ) {
            out = out w;
        } else if (s ? (POS(0) Id . w) = ) {
            d = pp_sym[w];
            if (DIFFER(d)) {
                if (IDENT(pp_gd[w])) {
                    pp_gd[w] = 1;
                    r = PPSubst(pv(d));
                    pp_gd[w] = ;
                    out = out r;
                } else { out = out w; }
            } else { out = out w; }
        } else if (s ? (POS(0) (ANY('0123456789') FENCE(SPAN('0123456789' &UCASE &LCASE '_') | epsilon)) . w) = ) {
            out = out w;
        } else if (s ? (POS(0) (LEN(1) FENCE(BREAK('0123456789' &UCASE &LCASE '_' pp_sp) | REM)) . w) = ) {
            out = out w;
        }
    }
    PPSubst = out;
    return;
}
/* ==================================================================================================================== */
function PPRead(nm, t, lp, dir) {
    PPRead = ;
    if (INPUT(.PPIN, 10, nm '[-r16777215]')) {
        t = '';
        t = PPIN;
        ENDFILE(10);
        PPRead = t;
        return;
    }
    lp = HOST(4, 'LPATH');
    while (DIFFER(lp)) {
        if (lp ? (POS(0) BREAK(': ') . dir ANY(': ')) = ) { ; } else { dir = lp; lp = ''; }
        if (INPUT(.PPIN, 10, dir '/' nm '[-r16777215]')) {
            t = '';
            t = PPIN;
            ENDFILE(10);
            PPRead = t;
            return;
        }
    }
    freturn;
}
/* ==================================================================================================================== */
function PPDir(dl, nc, cmd, rest, sym, r2, dv, nm, t, sv, d, st) {
    PPDir = .dummy;
    dl ? (POS(0) FENCE(SPAN(pp_spc) | epsilon) FENCE(Id . cmd | epsilon) FENCE(SPAN(pp_spc) | epsilon) REM . rest);
    pp_cmd = cmd;
    PPEmit(nc);
    if (DIFFER(pp_skip)) { if (cmd ? (POS(0) 'if')) { cmd = '$if'; } }
    if (IDENT(cmd, 'define')) {
        if (IDENT(pp_skip)) {
            if (rest ? (POS(0) Id . sym REM . r2)) {
                if (r2 ? (POS(0) '(')) { PPErr('syntax error'); }
                else {
                    r2 ? (POS(0) SPAN(pp_spc)) = ;
                    if (r2 ? (POS(0) ARBNO(NOTANY('#"' "'") | pp_lit) . dv ( '#' | RPOS(0) ))) {
                        dv ? (SPAN(pp_spc) RPOS(0)) = ;
                        d = pp_sym[sym];
                        if (DIFFER(d)) {
                            if (DIFFER(pv(d), dv)) { PPErr('redefinition of ' sym); }
                        } else { pp_sym[sym] = ppdef(dv); pp_n = pp_n + 1; }
                    } else { PPErr('syntax error'); }
                }
            } else { PPErr('syntax error'); }
        }
    } else if (IDENT(cmd, 'undef')) {
        if (IDENT(pp_skip)) {
            if (rest ? (POS(0) Id . sym REM . r2)) {
                if (r2 ? (POS(0) pp_end)) {
                    if (DIFFER(pp_sym[sym])) { pp_sym[sym] = ; pp_n = pp_n - 1; }
                } else { PPErr('syntax error'); }
            } else { PPErr('syntax error'); }
        }
    } else if (cmd ? (POS(0) ('ifdef' | 'ifndef') RPOS(0))) {
        pp_sn = pp_sn + 1;
        pp_stk[pp_sn] = pp_skip;
        if (rest ? (POS(0) Id . sym REM . r2)) {
            if (r2 ? (POS(0) pp_end)) {
                if (IDENT(pp_skip)) {
                    st = 'false';
                    if (DIFFER(pp_sym[sym])) { st = ''; }
                    if (IDENT(cmd, 'ifndef')) { if (IDENT(st)) { st = 'false'; } else { st = ''; } }
                    pp_skip = st;
                } else { pp_skip = 'off'; }
            } else { pp_skip = 'off'; PPErr('syntax error'); }
        } else { pp_skip = 'off'; PPErr('syntax error'); }
    } else if (IDENT(cmd, '$if')) {
        pp_sn = pp_sn + 1;
        pp_stk[pp_sn] = pp_skip;
        pp_skip = 'off';
    } else if (IDENT(cmd, 'else')) {
        if (rest ? (POS(0) pp_end)) {
            if (LE(pp_sn, pp_base)) { PPErr('no corresponding $if...'); }
            else if (IDENT(pp_skip, 'false')) { pp_skip = ''; }
            else if (IDENT(pp_skip)) { pp_skip = 'false'; }
        } else { PPErr('syntax error'); }
    } else if (IDENT(cmd, 'endif')) {
        if (LE(pp_sn, pp_base)) { PPErr('no corresponding $if...'); }
        else { pp_skip = pp_stk[pp_sn]; pp_sn = pp_sn - 1; }
        if (rest ? (POS(0) pp_end)) { ; } else { PPErr('syntax error'); }
    } else if (IDENT(cmd, 'include')) {
        if (IDENT(pp_skip)) {
            if (rest ? (POS(0) ( '"' BREAK('"') . nm '"' | Id . nm ) REM . r2)) {
                if (r2 ? (POS(0) pp_end)) {
                    if (DIFFER(pp_inc[nm])) { PPErr('circular reference to ' nm); }
                    else if (t = PPRead(nm)) {
                        pp_inc[nm] = 1;
                        sv = pp_base;
                        pp_base = pp_sn;
                        t ? *Preprocess;
                        if (NE(pp_sn, pp_base)) { PPErr('$if(s) without $endif(s)'); pp_skip = pp_stk[pp_base + 1]; pp_sn = pp_base; }
                        pp_base = sv;
                        pp_inc[nm] = ;
                    } else { PPErr('cannot open ' nm); }
                } else { PPErr('syntax error'); }
            } else { PPErr('syntax error'); }
        }
    } else if (IDENT(cmd, 'line')) {
        ;
    } else if (IDENT(cmd, 'error')) {
        if (IDENT(pp_skip)) { PPErr(rest); }
    } else {
        if (IDENT(pp_skip)) { PPErr('unknown preprocessor directive'); }
    }
    nreturn;
}
function ParseOne(ptree, i, n_kids) {
    if (Src ? ANY('$_')) {
        PPInit();
        if (Src ? *pp_need) {
            if (Src ? *Preprocess) { ; } else { pp_err = 1; }
            if (NE(pp_sn, 0)) { PPErr('$if(s) without $endif(s)'); }
            if (DIFFER(pp_err)) { OUTPUT = 'Parse Error'; return; }
            Src = pp_out pp_buf;
        }
    }
    pf_a = TIME();
    InitCounter();
    InitStack();
    if (Src ? *Compiland) {
        ptree = Pop();
        pf_parse = pf_parse + (TIME() - pf_a);
        if (DIFFER(ptree)) {
            i = 1;
            n_kids = n(ptree);
            while (LE(i, n_kids)) {
                TreeDump(c(ptree)[i]);
                i = i + 1;
            }
            TreeDumpEnd();
        } else OUTPUT = 'Parse Error';
    } else { pf_parse = pf_parse + (TIME() - pf_a); OUTPUT = 'Parse Error'; }
    return;
}
pf_list = HOST(4, 'PARSER_FILES');
if (IDENT(pf_list)) {
    INPUT(.INPUT, 9, '[-f0 -r16777215]');
    Src = INPUT;
    ParseOne();
} else {
    pf_n = 0;
    pf_bytes = 0;
    pf_parse = 0;
    pf_parse1 = 0;
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
