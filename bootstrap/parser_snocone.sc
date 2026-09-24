&FULLSCAN = 1;
reserved          = POS(0) ('if' | 'else' | 'while' | 'do' | 'for') RPOS(0);
/* ==================================================================================================================== */
/* notmatch() is provided by corpus/SCRIP/match.sc (loaded first in the runtime chain by  */
/* run_parser_sync_monitor.sh / run_scrip_parser.sh).  Redefining it here triggered       */
/* SPITBOL ERROR 217 duplicate-label when the transpiled .sno was run under SPITBOL with  */
/* match.sc included in the prelude.  SCT-9 (Opus 4.7, 2026-05-18).                       */
white       =   (  SPAN(' ' tab nl)
                |  '//' BREAK(nl) nl
                |  '/*' BREAKX('*') '*/'
                );
White       =   white ARBNO(white);
Gray        =   ARBNO(white);
$'  '       =   White;
$' '        =   Gray;
Id          =   ANY(&UCASE &LCASE '_') FENCE(SPAN('.' digits &UCASE '_' &LCASE) | epsilon);
$'break'    =   $' ' Id $ tx *IDENT(tx, 'break')    $' ';
$'case'     =   $' ' Id $ tx *IDENT(tx, 'case')     $' ';
$'continue' =   $' ' Id $ tx *IDENT(tx, 'continue') $' ';
$'default'  =   $' ' Id $ tx *IDENT(tx, 'default')  $' ';
$'do'       =   $' ' Id $ tx *IDENT(tx, 'do')       $' ';
$'else'     =   $' ' Id $ tx *IDENT(tx, 'else')     $' ';
$'for'      =   $' ' Id $ tx *IDENT(tx, 'for')      $' ';
$'freturn'  =   $' ' Id $ tx *IDENT(tx, 'freturn')  $' ';
$'function' =   $' ' Id $ tx *IDENT(tx, 'function') $' ';
$'goto'     =   $' ' Id $ tx *IDENT(tx, 'goto')     $' ';
$'if'       =   $' ' Id $ tx *IDENT(tx, 'if')       $' ';
$'nreturn'  =   $' ' Id $ tx *IDENT(tx, 'nreturn')  $' ';
$'return'   =   $' ' Id $ tx *IDENT(tx, 'return')   $' ';
$'struct'   =   $' ' Id $ tx *IDENT(tx, 'struct')   $' ';
$'switch'   =   $' ' Id $ tx *IDENT(tx, 'switch')   $' ';
$'while'    =   $' ' Id $ tx *IDENT(tx, 'while')    $' ';
Keyword     =   '&' SPAN(&UCASE '_' &LCASE) . token;
Integer     =   SPAN(digits) . token;
DQ_lit      =   '"' BREAK('"') . token '"';
SQ_lit      =   "'" BREAK("'") . token "'";
String      =   (*SQ_lit | *DQ_lit);
Ident       =   Id $ tx $ *notmatch(tx, reserved) . token;
Real        =   ( SPAN(digits)
                  FENCE(
                    '.'
                    SPAN(digits)
                    FENCE(ANY('eEdD') FENCE(ANY('+-') | epsilon) SPAN(digits) | epsilon)
                  | ANY('eEdD')
                    FENCE(ANY('+-') | epsilon)
                    SPAN(digits)
                  )
                ) . token;
$'('        =   '(' $' ';
$'['        =   '[' $' ';
$'{'        =   $' ' '{' $' ';
$')'        =   $' ' ')';
$'}'        =   $' ' '}';
$']'        =   $' ' ']';
$','        =   $' ' ',' $' ';
$':'        =   $' ' ':' $' ';
$';'        =   $' ' ';' $' ';
$'='        =   $'  ' '='   $'  ';
$'?'        =   $'  ' '?'   $'  ';
$'|'        =   $'  ' '|'   $'  ';
$'+'        =   $'  ' '+'   $'  ';
$'-'        =   $'  ' '-'   $'  ';
$'*'        =   $'  ' '*'   $'  ';
$'/'        =   $'  ' '/'   $'  ';
$'^'        =   $'  ' '^'   $'  ';
$'**'       =   $'  ' '**'  $'  ';
$'!'        =   $'  ' '!'   $'  ';
$'$'        =   $'  ' '$'   $'  ';
$'.'        =   $'  ' '.'   $'  ';
$'&'        =   $'  ' '&'   $'  ';
$'@'        =   $'  ' '@'   $'  ';
$'#'        =   $'  ' '#'   $'  ';
$'%'        =   $'  ' '%'   $'  ';
$'~'        =   $'  ' '~'   $'  ';
$'=='       =   $'  ' '=='  $'  ';
$'!='       =   $'  ' '!='  $'  ';
$'<'        =   $'  ' '<'   $'  ';
$'>'        =   $'  ' '>'   $'  ';
$'<='       =   $'  ' '<='  $'  ';
$'>='       =   $'  ' '>='  $'  ';
$'::'       =   $'  ' '::'  $'  ';
$':!:'      =   $'  ' ':!:' $'  ';
$'+='       =   $'  ' '+='  $'  ';
$'-='       =   $'  ' '-='  $'  ';
$'*='       =   $'  ' '*='  $'  ';
$'/='       =   $'  ' '/='  $'  ';
$'^='       =   $'  ' '^='  $'  ';
/* ==================================================================================================================== */
/* Expression grammar — shift/reduce only.  SC-SC-3: augop, cmp, paren, idx rewritten.   */
/* ==================================================================================================================== */
ArgFirst        =   *Expr0 nInc();
ArgRest         =   $',' *Expr0 nInc();
CallArgs        =   nPush() (*ArgFirst ARBNO(*ArgRest) | epsilon) reduce("'TT_ARGS'", 'nTop()') nPop();
Call            =   shift(*Ident, "'TT_QLIT'")
                    FENCE(
                      $'('
                      *CallArgs
                      reduce("'TT_FNC'", 2)
                    );
ExprList        =   nPush() *XList reduce("'TT_VLIST'", 'nTop()') nPop();
XList           =   nInc() (*Expr0 | shift(epsilon, "''")) FENCE($',' *XList | epsilon);
Expr17          =   FENCE(
                      *Call
                    | $'('
                      FENCE(
                        nPush() nInc() *Expr0 ARBNO($',' nInc() *Expr0)
                        FENCE( reduce("'TT_VLIST'", 'nTop()') | epsilon )
                        nPop()
                        $')'
                      | $')' shift(epsilon, "'TT_NUL'")
                      )
                    | *String   shift(epsilon, "'TT_QLIT'")
                    | *Real     shift(epsilon, "'TT_FLIT'")
                    | *Integer  shift(epsilon, "'TT_ILIT'")
                    | *Keyword  shift(epsilon, "'TT_KEYWORD'")
                    | *Ident    shift(epsilon, "'TT_VAR'")
                    );
Expr16          =   nInc() $'[' *ExprList $']' FENCE(*Expr16 | epsilon);
Expr15          =   *Expr17 FENCE(nPush() *Expr16 reduce("'TT_IDX'", 'nTop() + 1') nPop() | epsilon);
Expr14          =   '@' *Expr14 reduce("'TT_CAPT_CURSOR'", 1)
                |   '~' *Expr14 reduce("'TT_NOT'", 1)
                |   '+' *Expr14 reduce("'TT_PLS'", 1)
                |   '-' *Expr14 reduce("'TT_MNS'", 1)
                |   '*' *Expr14 reduce("'TT_DEFER'", 1)
                |   '$' *Expr14 reduce("'TT_INDIRECT'", 1)
                |   '.' *Expr14 reduce("'TT_NAME'", 1)
                |   '!' *Expr14 reduce("'TT_BANG'", 1)
                |   '%' *Expr14 reduce("'TT_PCT'", 1)
                |   '/' *Expr14 reduce("'TT_SLASH'", 1)
                |   '#' *Expr14 reduce("'TT_POUND'", 1)
                |   *Expr15;
Expr13          =   *Expr14 FENCE($'~' *Expr13 reduce("'TT_NOT'", 2) | epsilon);
Expr12          =   *Expr13
                    FENCE(
                      $'$' *Expr13 reduce("'TT_CAPT_IMMED_ASGN'", 2) FENCE($'$' *Expr13 reduce("'TT_CAPT_IMMED_ASGN'", 2) | epsilon)
                    | $'.' *Expr13 reduce("'TT_CAPT_COND_ASGN'", 2) FENCE($'.' *Expr13 reduce("'TT_CAPT_COND_ASGN'", 2) | epsilon)
                    | epsilon
                    );
Expr11          =   *Expr12 FENCE(($'^' | $'!' | $'**') *Expr11 reduce("'TT_POW'", 2) | epsilon);
Expr10          =   *Expr11 FENCE($'%' *Expr10 reduce("'TT_MUL'", 2) | epsilon);
Expr9           =   *Expr10 FENCE($'*' *Expr9  reduce("'TT_MUL'", 2) | epsilon);
Expr8           =   *Expr9  FENCE($'/' *Expr8  reduce("'TT_DIV'", 2) | epsilon);
Expr7           =   *Expr8  FENCE($'#' *Expr7  reduce("'TT_SUB'", 2) | epsilon);
Expr6           =   *Expr7  FENCE($'+' *Expr6 reduce("'TT_ADD'", 2) | $'-' *Expr6 reduce("'TT_SUB'", 2) | epsilon);
Expr5           =   *Expr6
                    FENCE(
                      $'@'  *Expr5 reduce("'TT_CAPT_CURSOR'", 2)
                    | $'==' *Expr6 reduce("'TT_EQ'", 2)
                    | $'!=' *Expr6 reduce("'TT_NE'", 2)
                    | $'<=' *Expr6 reduce("'TT_LE'", 2)
                    | $'>=' *Expr6 reduce("'TT_GE'", 2)
                    | $'<'  *Expr6 reduce("'TT_LT'", 2)
                    | $'>'  *Expr6 reduce("'TT_GT'", 2)
                    | epsilon
                    );
Expr4           =   nPush() *X4 reduce("'TT_SEQ'", 'nTop()') nPop();
X4              =   nInc() *Expr5 FENCE($'  ' *X4 | epsilon);
Expr3           =   nPush() *X3 reduce("'TT_ALT'", 'nTop()') nPop();
X3              =   nInc() *Expr4 FENCE($'|' *X3 | epsilon);
Expr2           =   *Expr3 FENCE($'&' *Expr2 reduce("'TT_SEQ'", 2) | epsilon);
Expr1           =   *Expr2 FENCE($'?' *Expr1 reduce("'TT_SCAN'", 2) | epsilon);
Expr0           =   *Expr1 FENCE(
                      $'='  FENCE(*Expr0 | shift(epsilon, "'TT_QLIT'")) reduce("'TT_ASSIGN'", 2)
                    | $'+=' *Expr0 reduce("'TT_AUGOP'", 2)
                    | $'-=' *Expr0 reduce("'TT_AUGOP'", 2)
                    | $'*=' *Expr0 reduce("'TT_AUGOP'", 2)
                    | $'/=' *Expr0 reduce("'TT_AUGOP'", 2)
                    | $'^=' *Expr0 reduce("'TT_AUGOP'", 2)
                    | epsilon);
/* ==================================================================================================================== */
/* Statement grammar — pure shift/reduce.  SC-SC-2.                                       */
/* ==================================================================================================================== */
/* Recurring body shape: brace-delimited list of Commands → TT_PROGRAM node. */
ThenBlock       =   nPush() $'{' ARBNO(*Command) $'}' reduce("'TT_PROGRAM'", 'nTop()') nPop();
/* if_cmd → TT_IF(cond, then_block) or TT_IF(cond, then_block, else_block) */
ElseBranch      =   $'else' ( *ThenBlock
                             | *if_cmd
                             );
if_cmd          =   $'if' $'(' *Expr0 $')' *ThenBlock
                    FENCE(*ElseBranch reduce("'TT_IF'", 3) | reduce("'TT_IF'", 2));
/* while_cmd → TT_WHILE(cond, body) */
while_cmd       =   $'while' $'(' *Expr0 $')' *ThenBlock reduce("'TT_WHILE'", 2);
/* do_cmd → TT_DO_WHILE(body, cond) */
do_cmd          =   $'do' *ThenBlock $'while' $'(' *Expr0 $')' ($';' | epsilon)
                    reduce("'TT_DO_WHILE'", 2);
/* for_cmd → TT_FOR(init, cond, step, body) */
ForBody         =   *ThenBlock
                |   nPush() *Command reduce("'TT_PROGRAM'", 'nTop()') nPop();
for_cmd         =   $'for' $'(' *Expr0 $';' *Expr0 $';' *Expr0 $')'
                    *ForBody reduce("'TT_FOR'", 4);
/* switch_cmd → TT_CASE(disc, val0, body0, val1, body1, …) */
CaseArm         =   $'case' *Expr0 $':' nInc() nPush() ARBNO(*Command)
                    reduce("'TT_PROGRAM'", 'nTop()') nPop() nInc();
DefaultArm      =   $'default' $':' shift(epsilon, "'TT_NUL'") nInc()
                    nPush() ARBNO(*Command) reduce("'TT_PROGRAM'", 'nTop()') nPop() nInc();
switch_cmd      =   nPush()
                    $'switch' $'(' *Expr0 $')' nInc()
                    $'{' ARBNO(*CaseArm | *DefaultArm) $'}'
                    reduce("'TT_CASE'", 'nTop()')
                    nPop();
/* func_cmd → TT_DEFINE(QLIT(name), TT_PARAMS(params…), TT_PROGRAM(body)) */
ParamFirst      =   shift(*Ident, "'TT_VAR'") nInc();
ParamRest       =   $',' shift(*Ident, "'TT_VAR'") nInc();
Params          =   nPush() (*ParamFirst ARBNO(*ParamRest) | epsilon)
                    reduce("'TT_PARAMS'", 'nTop()') nPop();
func_cmd        =   $'function' shift(*Ident, "'TT_QLIT'")
                    $'(' *Params $')' *ThenBlock
                    reduce("'TT_DEFINE'", 3);
/* return_cmd / freturn_cmd / nreturn_cmd */
return_cmd      =   $'return' ( *Expr0 $';' reduce("'TT_RETURN'", 1)
                               |        $';' reduce("'TT_RETURN'", 0) );
freturn_cmd     =   $'freturn' $';' reduce("'TT_PROC_FAIL'", 0);
nreturn_cmd     =   $'nreturn' $';' reduce("'TT_NRETURN'", 0);
/* goto_cmd / label_prefix */
goto_cmd        =   $'goto' shift(*Ident, "'TT_QLIT'") $';' reduce("'TT_GOTO_U'", 1);
label_prefix    =   shift(*Ident, "'TT_QLIT'") $':'       reduce("'TT_LABEL'", 1);
/* break_cmd / continue_cmd */
break_cmd       =   $'break'
                    ( shift(*Ident, "'TT_QLIT'") $';' reduce("'TT_LOOP_BREAK'", 1)
                    | $';'                            reduce("'TT_LOOP_BREAK'", 0) );
continue_cmd    =   $'continue'
                    ( shift(*Ident, "'TT_QLIT'") $';' reduce("'TT_LOOP_NEXT'", 1)
                    | $';'                            reduce("'TT_LOOP_NEXT'", 0) );
/* struct_cmd → TT_STRUCT(name, fields) */
StructFieldFirst =  shift(*Ident, "'TT_VAR'") nInc();
StructFieldRest  =  $',' shift(*Ident, "'TT_VAR'") nInc();
StructFields     =  nPush() (*StructFieldFirst ARBNO(*StructFieldRest) | epsilon)
                    reduce("'TT_FIELDS'", 'nTop()') nPop();
struct_cmd       =  $'struct' shift(*Ident, "'TT_QLIT'")
                    $'{' *StructFields $'}' reduce("'TT_STRUCT'", 2);
/* stmt_cmd — subject/pattern decomposition removed (lower's job per PST-SC-4l) */
stmt_body       =   *Expr0 ($';' | epsilon);
stmt_cmd        =   *stmt_body;
/* empty_cmd */
empty_cmd       =   $';';
/* Command dispatcher */
Command         =   $' ' ( *if_cmd
                    | *while_cmd
                    | *do_cmd
                    | *for_cmd
                    | *func_cmd
                    | *return_cmd
                    | *freturn_cmd
                    | *nreturn_cmd
                    | *goto_cmd
                    | *break_cmd
                    | *continue_cmd
                    | *struct_cmd
                    | *switch_cmd
                    | $'case' *CaseArm
                    | *DefaultArm
                    | *label_prefix
                    | *empty_cmd
                    | nInc() *stmt_cmd
                    );
/* Compiland — top-level program */
Compiland       =   nPush() POS(0) ARBNO(*Command) RPOS(0)
                    reduce("'Parse'", 'nTop()') nPop();
/* ==================================================================================================================== */
/* Driver                                                                                  */
/* ==================================================================================================================== */
InitCounter();
InitStack();
Src = '';
while (Line = INPUT) Src = Src Line nl ;
if (Src ? Compiland) {
    ptree = Pop();
    if (DIFFER(ptree)) {
        i = 1;
        n_kids = n(ptree);
        while (LE(i, n_kids)) {
            TDump(c(ptree)[i]);
            i = i + 1;
        }
    }
} else OUTPUT = 'Parse Error';
