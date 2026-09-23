/* parser_snocone.sc -- Snocone as ONE pattern, built only from shift / reduce / nPush / nInc / nTop / nPop.
   Lon 2026-09-23 (CEO-1212): "read the SPITBOL manual for expression syntax, and the Snocone parser for statement syntax."
   STATEMENTS mirror src/parsers/snocone/snocone_parse.y and TOKENS mirror snocone_lex.c:
     - blanks are blanks, tabs, CR, FF, newlines, // and block comments; a '+' or '.' in column 1 after a newline is a
       continuation mark the lexer drops (S_CONT);
     - an identifier is [A-Za-z_][A-Za-z0-9_]*; the keywords are reserved and case-sensitive; name( with NO blank is a
       call, name ( is the name concatenated with a parenthesised expression;
     - a binary operator needs a blank on BOTH sides; after a value, an operator char with a blank before and none after
       is a unary operator in an implicit concatenation; '=' after a value is always assignment; the relations
       == != < > <= >= and :==: :!=: :<: :>: :<=: :>=: and the assignments = += -= *= /= ^= need no blanks;
       '&&' is explicit concatenation; ' ^ ', ' ** ' and ' ! ' are exponentiation.
   EXPRESSIONS follow the SPITBOL manual's operator table (Chapter 15), lowest first, with Snocone's relations at
   priority 5 where snocone_parse.y puts them (between concatenation and + -):
       = 0 right · ? 1 left · & 2 left · | 3 right · concatenation 4 right · relations, @ 5 · + - 6 left · # 7 left ·
       / 8 left · * 9 left · % 10 left · ^ ! ** 11 right · $ . 12 left · ~ 13 right · unary operators highest.
     ⛔ snocone_parse.y puts * and / on ONE level and makes ? right-associative, so SCRIP reads 8 / 2 * 2 as 8 where
     SPITBOL reads 2; this parser follows the manual.
   THE TREE uses ast.h's kinds: TT_PROGRAM of statements · TT_DEFINE(name, TT_VLIST params, TT_VLIST locals, TT_PROGRAM)
     · TT_RECORD_DECL(name, fields...) for struct · TT_IF(c, TT_PROGRAM[, TT_PROGRAM]) TT_WHILE(c, body)
     TT_DO_WHILE(body, c) TT_FOR(init, cond, step, body) · TT_CASE(e, v1, b1, ..., TT_NUL, default-body) ·
     TT_RETURN([e]) TT_PROC_FAIL TT_NRETURN · (TT_GOTO_U L) (TT_LABEL_DEF L) · TT_LOOP_BREAK/TT_LOOP_NEXT([TT_QLIT L]) ·
     TT_ASSIGN TT_AUGOP(lhs, op-kind leaf, rhs) TT_SCAN TT_ALT TT_SEQ TT_EQ..TT_LGE TT_ADD TT_SUB TT_MUL TT_DIV TT_POW
     TT_CAPT_IMMED_ASGN TT_CAPT_COND_ASGN TT_CAPT_CURSOR · TT_OPSYN(TT_QLIT op, operands...) for an operator the manual
     leaves to OPSYN · TT_PLS TT_MNS TT_DEFER TT_NAME TT_INDIRECT TT_NOT TT_INTERROGATE · TT_FNC(name, args...) and the
     eighteen pattern primitives as their own kinds (TT_ANY(args...) ...) · TT_IDX(base, subscripts...) · TT_VLIST for
     (a, b) · TT_NUL for () · TT_VAR TT_KEYWORD TT_ILIT TT_FLIT TT_QLIT (a string's text is kept as written, a doubled
     quote stays doubled). A block { } as a statement adds its statements to the list it stands in, as snocone_parse.y
     does; the relations are their own kinds (rebus.y's), where snocone_parse.y spells them as calls to EQ, NE, ... */
&FULLSCAN = 1;
/* ---- blanks: greedy and committed */
wsp         =   SPAN(' ' tab cr ff) | nl FENCE(ANY('+.') | epsilon) | '//' BREAK(nl) | '/*' BREAKX('*') '*/';
White       =   wsp FENCE(*White | epsilon);
Gray        =   FENCE(*White | epsilon);
$' '        =   Gray;
$'  '       =   White;
/* ---- words */
IdStart     =   &UCASE &LCASE '_';
IdChars     =   &UCASE &LCASE digits '_';
Word        =   (ANY(IdStart) FENCE(SPAN(IdChars) | epsilon)) $ tx;
ScKw        =   TABLE();
ScKw['if'] = 1;        ScKw['else'] = 1;      ScKw['while'] = 1;    ScKw['do'] = 1;        ScKw['for'] = 1;
ScKw['switch'] = 1;    ScKw['case'] = 1;      ScKw['default'] = 1;  ScKw['break'] = 1;     ScKw['continue'] = 1;
ScKw['goto'] = 1;      ScKw['function'] = 1;  ScKw['procedure'] = 1; ScKw['return'] = 1;  ScKw['freturn'] = 1;
ScKw['nreturn'] = 1;   ScKw['struct'] = 1;
Ident       =   *Word *IDENT(ScKw[tx]);
kw_if       =   Gray *Word *IDENT(tx, 'if')       Gray;
kw_else     =   Gray *Word *IDENT(tx, 'else')     Gray;
kw_while    =   Gray *Word *IDENT(tx, 'while')    Gray;
kw_do       =   Gray *Word *IDENT(tx, 'do')       Gray;
kw_for      =   Gray *Word *IDENT(tx, 'for')      Gray;
kw_switch   =   Gray *Word *IDENT(tx, 'switch')   Gray;
kw_case     =   Gray *Word *IDENT(tx, 'case')     Gray;
kw_default  =   Gray *Word *IDENT(tx, 'default')  Gray;
kw_break    =   Gray *Word *IDENT(tx, 'break')    Gray;
kw_continue =   Gray *Word *IDENT(tx, 'continue') Gray;
kw_goto     =   Gray *Word *IDENT(tx, 'goto')     Gray;
kw_function =   Gray *Word (*IDENT(tx, 'function') | *IDENT(tx, 'procedure')) Gray;
kw_return   =   Gray *Word *IDENT(tx, 'return')   Gray;
kw_freturn  =   Gray *Word *IDENT(tx, 'freturn')  Gray;
kw_nreturn  =   Gray *Word *IDENT(tx, 'nreturn')  Gray;
kw_struct   =   Gray *Word *IDENT(tx, 'struct')   Gray;
/* ---- operators: the spaced binary ones need a blank on both sides (snocone_lex.c is_rws_at) */
$'?'        =   White '?'  White;      $'|'   = White '|'  White;
$'+'        =   White '+'  White;      $'-'   = White '-'  White;
$'/'        =   White '/'  White;      $'*'   = White '*'  White;
$'$'        =   White '$'  White;      $'.'   = White '.'  White;
$'^'        =   White ('^' | '**' | '!') White;
$'&&'       =   Gray '&&' Gray;
$'='        =   Gray '='   Gray;       $'+='  = Gray '+='  Gray;       $'-='  = Gray '-='  Gray;
$'*='       =   Gray '*='  Gray;       $'/='  = Gray '/='  Gray;       $'^='  = Gray '^='  Gray;
$'('        =   '(' Gray;              $')'   = Gray ')';
$'['        =   Gray '[' Gray;         $']'   = Gray ']';
$'{'        =   Gray '{' Gray;         $'}'   = Gray '}';
$','        =   Gray ',' Gray;         $';'   = Gray ';';             $':'   = Gray ':';
/* an implicit concatenation is a blank NOT followed by an operator the lexer would read as binary or as an assignment:
   '=' after a value is always assignment, and an operator char followed by a blank or '=' is binary or augmented */
CatOps      =   '+-*.$@~?&%/#|!';
CatWs       =   ' ' tab nl cr ff;
CatFrom     =   '=' CatOps CatWs;
CatTo       =   CHAR(3) DUPL(CHAR(1), SIZE(CatOps)) DUPL(CHAR(2), SIZE(CatWs));
CatOK       =   @ccur *DIFFER(REPLACE(SUBSTR(Src, ccur + 1, 1), CatFrom, CatTo), CHAR(3))
                      *DIFFER(REPLACE(SUBSTR(Src, ccur + 1, 2), CatFrom, CatTo), CHAR(1) CHAR(2))
                      *DIFFER(REPLACE(SUBSTR(Src, ccur + 1, 2), CatFrom, CatTo), CHAR(1) CHAR(3));
/* ---- literals (snocone_lex.c S_INT S_FRAC S_EXP S_STR1 S_STR2) */
Integer     =   SPAN(digits);
Exponent    =   ANY('eEdD') FENCE(ANY('+-') | epsilon) FENCE(SPAN(digits) | epsilon);
Real        =   SPAN(digits) ('.' FENCE(SPAN(digits) FENCE(*Exponent | epsilon) | epsilon) | *Exponent)
            |   '.' SPAN(digits) FENCE(*Exponent | epsilon);
SQbody      =   BREAK("'") FENCE("''" *SQbody | epsilon);
DQbody      =   BREAK('"') FENCE('""' *DQbody | epsilon);
/* ---- expressions */
ListTail    =   $',' nInc() *Expr0 FENCE(*ListTail | epsilon);
ArgList     =   FENCE(nInc() *Expr0 FENCE(*ListTail | epsilon) | epsilon);
Args        =   '(' Gray *ArgList $')';
PrimCall    =   nPush()
                FENCE(  *Word *IDENT(tx, 'ANY')     *Args reduce('TT_ANY',     'nTop()')
                     |  *Word *IDENT(tx, 'NOTANY')  *Args reduce('TT_NOTANY',  'nTop()')
                     |  *Word *IDENT(tx, 'SPAN')    *Args reduce('TT_SPAN',    'nTop()')
                     |  *Word *IDENT(tx, 'BREAK')   *Args reduce('TT_BREAK',   'nTop()')
                     |  *Word *IDENT(tx, 'BREAKX')  *Args reduce('TT_BREAKX',  'nTop()')
                     |  *Word *IDENT(tx, 'LEN')     *Args reduce('TT_LEN',     'nTop()')
                     |  *Word *IDENT(tx, 'POS')     *Args reduce('TT_POS',     'nTop()')
                     |  *Word *IDENT(tx, 'RPOS')    *Args reduce('TT_RPOS',    'nTop()')
                     |  *Word *IDENT(tx, 'TAB')     *Args reduce('TT_TAB',     'nTop()')
                     |  *Word *IDENT(tx, 'RTAB')    *Args reduce('TT_RTAB',    'nTop()')
                     |  *Word *IDENT(tx, 'ARB')     *Args reduce('TT_ARB',     'nTop()')
                     |  *Word *IDENT(tx, 'ARBNO')   *Args reduce('TT_ARBNO',   'nTop()')
                     |  *Word *IDENT(tx, 'REM')     *Args reduce('TT_REM',     'nTop()')
                     |  *Word *IDENT(tx, 'FAIL')    *Args reduce('TT_FAIL',    'nTop()')
                     |  *Word *IDENT(tx, 'SUCCEED') *Args reduce('TT_SUCCEED', 'nTop()')
                     |  *Word *IDENT(tx, 'FENCE')   *Args reduce('TT_FENCE',   'nTop()')
                     |  *Word *IDENT(tx, 'ABORT')   *Args reduce('TT_ABORT',   'nTop()')
                     |  *Word *IDENT(tx, 'BAL')     *Args reduce('TT_BAL',     'nTop()')
                     )
                nPop();
Call        =   nPush() nInc() shift(*Ident, 'TT_VAR') *Args reduce('TT_FNC', 'nTop()') nPop();
Paren       =   '(' Gray
                FENCE(  ')' shift(epsilon, 'TT_NUL')
                     |  nPush() nInc() *Expr0 FENCE(*ListTail reduce('TT_VLIST', 'nTop()') | epsilon) nPop() $')'
                     );
Expr17      =   FENCE(  *PrimCall
                     |  *Call
                     |  shift(*Ident, 'TT_VAR')
                     |  '&' shift(*Word, 'TT_KEYWORD')
                     |  shift(*Real, 'TT_FLIT')
                     |  shift(*Integer, 'TT_ILIT')
                     |  "'" shift(*SQbody, 'TT_QLIT') "'"
                     |  '"' shift(*DQbody, 'TT_QLIT') '"'
                     |  *Paren
                     );
IdxTail     =   nPush() $'[' *ArgList $']' reduce('TT_IDX', 'nTop() + 1') nPop() FENCE(*IdxTail | epsilon);
Expr15      =   *Expr17 FENCE(*IdxTail | epsilon);
Expr14      =   FENCE(  *Expr15
                     |  '+' Gray *Expr14 reduce('TT_PLS', 1)
                     |  '-' Gray *Expr14 reduce('TT_MNS', 1)
                     |  '*' Gray *Expr14 reduce('TT_DEFER', 1)
                     |  '.' Gray *Expr14 reduce('TT_NAME', 1)
                     |  '$' Gray *Expr14 reduce('TT_INDIRECT', 1)
                     |  '@' Gray *Expr14 reduce('TT_CAPT_CURSOR', 1)
                     |  '~' Gray *Expr14 reduce('TT_NOT', 1)
                     |  '?' Gray *Expr14 reduce('TT_INTERROGATE', 1)
                     |  shift(ANY('&%/#|=!'), 'TT_QLIT') Gray *Expr14 reduce('TT_OPSYN', 2)
                     );
Expr13      =   *Expr14 FENCE(White shift('~', 'TT_QLIT') White *Expr13 reduce('TT_OPSYN', 3) | epsilon);
CaptTail    =   FENCE($'$' *Expr13 reduce('TT_CAPT_IMMED_ASGN', 2) | $'.' *Expr13 reduce('TT_CAPT_COND_ASGN', 2))
                FENCE(*CaptTail | epsilon);
Expr12      =   *Expr13 FENCE(*CaptTail | epsilon);
Expr11      =   *Expr12 FENCE($'^' *Expr11 reduce('TT_POW', 2) | epsilon);
PctTail     =   White shift('%', 'TT_QLIT') White *Expr11 reduce('TT_OPSYN', 3) FENCE(*PctTail | epsilon);
Expr10      =   *Expr11 FENCE(*PctTail | epsilon);
MulTail     =   $'*' *Expr10 reduce('TT_MUL', 2) FENCE(*MulTail | epsilon);
Expr9       =   *Expr10 FENCE(*MulTail | epsilon);
DivTail     =   $'/' *Expr9 reduce('TT_DIV', 2) FENCE(*DivTail | epsilon);
Expr8       =   *Expr9 FENCE(*DivTail | epsilon);
PndTail     =   White shift('#', 'TT_QLIT') White *Expr8 reduce('TT_OPSYN', 3) FENCE(*PndTail | epsilon);
Expr7       =   *Expr8 FENCE(*PndTail | epsilon);
AddTail     =   FENCE($'+' *Expr7 reduce('TT_ADD', 2) | $'-' *Expr7 reduce('TT_SUB', 2)) FENCE(*AddTail | epsilon);
Expr6       =   *Expr7 FENCE(*AddTail | epsilon);
RelTail     =   FENCE(  Gray ':==:' Gray *Expr6 reduce('TT_LEQ', 2)
                     |  Gray ':!=:' Gray *Expr6 reduce('TT_LNE', 2)
                     |  Gray ':<=:' Gray *Expr6 reduce('TT_LLE', 2)
                     |  Gray ':>=:' Gray *Expr6 reduce('TT_LGE', 2)
                     |  Gray ':<:'  Gray *Expr6 reduce('TT_LLT', 2)
                     |  Gray ':>:'  Gray *Expr6 reduce('TT_LGT', 2)
                     |  Gray '=='   Gray *Expr6 reduce('TT_EQ',  2)
                     |  Gray '!='   Gray *Expr6 reduce('TT_NE',  2)
                     |  Gray '<='   Gray *Expr6 reduce('TT_LE',  2)
                     |  Gray '>='   Gray *Expr6 reduce('TT_GE',  2)
                     |  Gray '<'    Gray *Expr6 reduce('TT_LT',  2)
                     |  Gray '>'    Gray *Expr6 reduce('TT_GT',  2)
                     )
                FENCE(*RelTail | epsilon);
Expr5       =   *Expr6 FENCE(*RelTail | White shift('@', 'TT_QLIT') White *Expr5 reduce('TT_OPSYN', 3) | epsilon);
Expr4       =   *Expr5 FENCE(($'&&' | White *CatOK) *Expr4 reduce('TT_SEQ', 2) | epsilon);
Expr3       =   *Expr4 FENCE($'|' *Expr3 reduce('TT_ALT', 2) | epsilon);
AmpTail     =   White shift('&', 'TT_QLIT') White *Expr3 reduce('TT_OPSYN', 3) FENCE(*AmpTail | epsilon);
Expr2       =   *Expr3 FENCE(*AmpTail | epsilon);
ScanTail    =   $'?' *Expr2 reduce('TT_SCAN', 2) FENCE(*ScanTail | epsilon);
Expr1       =   *Expr2 FENCE(*ScanTail | epsilon);
Expr0       =   *Expr1
                FENCE(  $'+=' reduce('TT_ADD', 0) *Expr0 reduce('TT_AUGOP', 3)
                     |  $'-=' reduce('TT_SUB', 0) *Expr0 reduce('TT_AUGOP', 3)
                     |  $'*=' reduce('TT_MUL', 0) *Expr0 reduce('TT_AUGOP', 3)
                     |  $'/=' reduce('TT_DIV', 0) *Expr0 reduce('TT_AUGOP', 3)
                     |  $'^=' reduce('TT_POW', 0) *Expr0 reduce('TT_AUGOP', 3)
                     |  $'=' FENCE(*Expr0 | shift(epsilon, 'TT_QLIT')) reduce('TT_ASSIGN', 2)
                     |  epsilon
                     );
/* ---- statements (snocone_parse.y stmt) */
stmts       =   ARBNO(*stmt);
block       =   $'{' *stmts $'}';
body        =   nPush() *stmt reduce('TT_PROGRAM', 'nTop()') nPop();
if_stmt     =   nInc() *kw_if $'(' *Expr0 $')' *body FENCE(*kw_else *body reduce('TT_IF', 3) | reduce('TT_IF', 2));
while_stmt  =   nInc() *kw_while $'(' *Expr0 $')' *body reduce('TT_WHILE', 2);
do_stmt     =   nInc() *kw_do nPush() *block reduce('TT_PROGRAM', 'nTop()') nPop()
                *kw_while $'(' *Expr0 $')' $';' reduce('TT_DO_WHILE', 2);
for_stmt    =   nInc() *kw_for $'(' *Expr0 $';' Gray *Expr0 $';' Gray *Expr0 $')' *body reduce('TT_FOR', 4);
case_clause =   FENCE(*kw_case nInc() *Expr0 $':' | *kw_default $':' nInc() shift(epsilon, 'TT_NUL'))
                nInc() nPush() *stmts reduce('TT_PROGRAM', 'nTop()') nPop();
switch_stmt =   nInc() *kw_switch $'(' nPush() nInc() *Expr0 $')' $'{' ARBNO(*case_clause) $'}'
                reduce('TT_CASE', 'nTop()') nPop();
id_tail     =   $',' nInc() shift(*Ident, 'TT_VAR') FENCE(*id_tail | epsilon);
idlist      =   nPush() FENCE(nInc() shift(*Ident, 'TT_VAR') FENCE(*id_tail | epsilon) | epsilon)
                reduce('TT_VLIST', 'nTop()') nPop();
func_stmt   =   nInc() *kw_function shift(*Ident, 'TT_VAR') Gray '(' Gray *idlist $')' Gray *idlist
                $'{' nPush() *stmts reduce('TT_PROGRAM', 'nTop()') nPop() $'}' reduce('TT_DEFINE', 4);
struct_stmt =   nInc() *kw_struct nPush() nInc() shift(*Ident, 'TT_VAR') $'{'
                FENCE(nInc() Gray shift(*Ident, 'TT_VAR') FENCE(*id_tail | epsilon) | epsilon) $'}'
                reduce('TT_RECORD_DECL', 'nTop()') nPop();
jump_label  =   shift(*Ident, 'TT_QLIT');
stmt        =   Gray
                FENCE(  *block
                     |  *if_stmt
                     |  *while_stmt
                     |  *do_stmt
                     |  *for_stmt
                     |  *switch_stmt
                     |  *func_stmt
                     |  *struct_stmt
                     |  nInc() *kw_return FENCE(*Expr0 $';' reduce('TT_RETURN', 1) | $';' reduce('TT_RETURN', 0))
                     |  nInc() *kw_freturn $';' reduce('TT_PROC_FAIL', 0)
                     |  nInc() *kw_nreturn $';' reduce('TT_NRETURN', 0)
                     |  nInc() *kw_goto shift(*Ident, 'TT_GOTO_U') $';'
                     |  nInc() *kw_break    FENCE(*jump_label reduce('TT_LOOP_BREAK', 1) | reduce('TT_LOOP_BREAK', 0)) $';'
                     |  nInc() *kw_continue FENCE(*jump_label reduce('TT_LOOP_NEXT', 1)  | reduce('TT_LOOP_NEXT', 0))  $';'
                     |  nInc() *Expr0 $';'
                     |  nInc() shift(*Ident, 'TT_LABEL_DEF') $':'
                     |  ';'
                     );
Compiland   =   nPush() POS(0) *stmts Gray RPOS(0) reduce('TT_PROGRAM', 'nTop()') nPop();
/* ==================================================================================================================== */
InitCounter();
InitStack();
Src = '';
while (Line = INPUT) Src = Src Line nl;
if (Src ? Compiland) {
    ptree = Pop();
    i = 1;
    n_kids = n(ptree);
    while (LE(i, n_kids)) {
        TDump(ITEM(c(ptree), i));
        i = i + 1;
    }
    if (EQ(n_kids, 0)) TDump(ptree);
} else OUTPUT = 'Parse Error';
