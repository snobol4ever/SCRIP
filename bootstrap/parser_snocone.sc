&FULLSCAN = 1;
reserved          = POS(0) ('if' | 'else' | 'while' | 'do' | 'for') RPOS(0);
/* ==================================================================================================================== */
/* notmatch() is provided by corpus/SCRIP/match.sc (loaded first in the runtime chain by  */
/* run_parser_sync_monitor.sh / run_scrip_parser.sh).  Redefining it here triggered       */
/* SPITBOL ERROR 217 duplicate-label when the transpiled .sno was run under SPITBOL with  */
/* match.sc included in the prelude.  SCT-9 (Opus 4.7, 2026-05-18).                       */
white       =   (  SPAN(' ' tab nl)
                |  '//' BREAK(nl) nl
                |  '/*' BREAK('*') '*' ARBNO('*' | NOTANY('/*') BREAK('*') '*') '/'
                );
White       =   white ARBNO(white);
Gray        =   White | epsilon;
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
$'function' =   $' ' Id $ tx (*IDENT(tx, 'function') | *IDENT(tx, 'procedure')) $' ';
$'goto'     =   $' ' Id $ tx *IDENT(tx, 'goto')     $' ';
$'if'       =   $' ' Id $ tx *IDENT(tx, 'if')       $' ';
$'nreturn'  =   $' ' Id $ tx *IDENT(tx, 'nreturn')  $' ';
$'return'   =   $' ' Id $ tx *IDENT(tx, 'return')   $' ';
$'struct'   =   $' ' Id $ tx *IDENT(tx, 'struct')   $' ';
$'switch'   =   $' ' Id $ tx *IDENT(tx, 'switch')   $' ';
$'while'    =   $' ' Id $ tx *IDENT(tx, 'while')    $' ';
Keyword     =   '&' (SPAN(&UCASE '_' &LCASE)) . thx . *Shift('TT_KEYWORD', thx);
Integer     =   SPAN(digits) . token;
DQ_lit      =   '"' (BREAK('"')) . thx . *Shift('TT_QLIT', thx) '"';
SQ_lit      =   "'" (BREAK("'")) . thx . *Shift('TT_QLIT', thx) "'";
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
$'&&'       =   $'  ' '&&'  $'  ';
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
ArgFirst        =   *Expr0 . *IncCounter();
ArgRest         =   $',' *Expr0 . *IncCounter();
CallArgs        =   epsilon . *PushCounter() (*ArgFirst ARBNO(*ArgRest) | epsilon) . *Reduce('TT_ARGS', nTop()) . *PopCounter();
Call            =   (*Ident) . thx . *Shift('TT_QLIT', thx)
                    FENCE(
                      $'('
                      *CallArgs
                      $')'
                      . *Reduce('TT_FNC', 2)
                    );
ExprList        =   epsilon . *PushCounter() *XList . *Reduce('TT_VLIST', nTop()) . *PopCounter();
XList           =   epsilon . *IncCounter() (*Expr0 | (epsilon) . thx . *Shift('', thx)) FENCE($',' *XList | epsilon);
Expr17          =   FENCE(
                      *Call
                    | $'('
                      FENCE(
                        epsilon . *PushCounter() . *IncCounter() *Expr0 ARBNO($',' . *IncCounter() *Expr0)
                        FENCE( epsilon . *Reduce('TT_VLIST', nTop()) | epsilon )
                        . *PopCounter()
                        $')'
                      | $')' (epsilon) . thx . *Shift('TT_NUL', thx)
                      )
                    | *String
                    | (*Real) . thx . *Shift('TT_FLIT', thx)
                    | (*Integer) . thx . *Shift('TT_ILIT', thx)
                    | *Keyword
                    | (*Ident) . thx . *Shift('TT_VAR', thx)
                    );
Expr16          =   epsilon . *IncCounter() $'[' *ExprList $']' FENCE(*Expr16 | epsilon);
Expr15          =   *Expr17 FENCE(epsilon . *PushCounter() *Expr16 . *Reduce('TT_IDX', nTop() + 1) . *PopCounter() | epsilon);
Expr14          =   '@' *Expr14 . *Reduce('TT_CAPT_CURSOR', 1)
                |   '~' *Expr14 . *Reduce('TT_NOT', 1)
                |   '+' *Expr14 . *Reduce('TT_PLS', 1)
                |   '-' *Expr14 . *Reduce('TT_MNS', 1)
                |   '*' *Expr14 . *Reduce('TT_DEFER', 1)
                |   '$' *Expr14 . *Reduce('TT_INDIRECT', 1)
                |   '.' *Expr14 . *Reduce('TT_NAME', 1)
                |   '!' *Expr14 . *Reduce('TT_BANG', 1)
                |   '?' *Expr14 . *Reduce('TT_INTERROGATE', 1)
                |   '%' *Expr14 . *Reduce('TT_PCT', 1)
                |   '/' *Expr14 . *Reduce('TT_SLASH', 1)
                |   '#' *Expr14 . *Reduce('TT_POUND', 1)
                |   *Expr15;
Expr13          =   *Expr14 FENCE($'~' *Expr13 . *Reduce('TT_NOT', 2) | epsilon);
Expr12          =   *Expr13
                    FENCE(
                      $'$' *Expr13 . *Reduce('TT_CAPT_IMMED_ASGN', 2) FENCE($'$' *Expr13 . *Reduce('TT_CAPT_IMMED_ASGN', 2) | epsilon)
                    | $'.' *Expr13 . *Reduce('TT_CAPT_COND_ASGN', 2) FENCE($'.' *Expr13 . *Reduce('TT_CAPT_COND_ASGN', 2) | epsilon)
                    | epsilon
                    );
Expr11          =   *Expr12 FENCE(($'^' | $'!' | $'**') *Expr11 . *Reduce('TT_POW', 2) | epsilon);
Expr10          =   *Expr11 FENCE($'%' *Expr10 . *Reduce('TT_MUL', 2) | epsilon);
Expr9           =   *Expr10 FENCE($'*' *Expr9  . *Reduce('TT_MUL', 2) | epsilon);
Expr8           =   *Expr9  FENCE($'/' *Expr8  . *Reduce('TT_DIV', 2) | epsilon);
Expr7           =   *Expr8  FENCE($'#' *Expr7  . *Reduce('TT_SUB', 2) | epsilon);
Expr6           =   *Expr7  FENCE($'+' *Expr6 . *Reduce('TT_ADD', 2) | $'-' *Expr6 . *Reduce('TT_SUB', 2) | epsilon);
Expr5           =   *Expr6
                    FENCE(
                      $'@'  *Expr5 . *Reduce('TT_CAPT_CURSOR', 2)
                    | $'==' *Expr6 . *Reduce('TT_EQ', 2)
                    | $'!=' *Expr6 . *Reduce('TT_NE', 2)
                    | $'<=' *Expr6 . *Reduce('TT_LE', 2)
                    | $'>=' *Expr6 . *Reduce('TT_GE', 2)
                    | $'<'  *Expr6 . *Reduce('TT_LT', 2)
                    | $'>'  *Expr6 . *Reduce('TT_GT', 2)
                    | epsilon
                    );
Expr4           =   epsilon . *PushCounter() *X4 . *Reduce('TT_SEQ', nTop()) . *PopCounter();
X4              =   epsilon . *IncCounter() *Expr5 FENCE($'  ' *X4 | epsilon);
Expr3           =   epsilon . *PushCounter() *X3 . *Reduce('TT_ALT', nTop()) . *PopCounter();
X3              =   epsilon . *IncCounter() *Expr4 FENCE($'|' *X3 | epsilon);
Expr2           =   *Expr3 FENCE($'&&' *Expr2 . *Reduce('TT_SEQ', 2) | epsilon);
Expr1           =   *Expr2 FENCE($'?' *Expr1 . *Reduce('TT_SCAN', 2) | epsilon);
Expr0           =   *Expr1 FENCE(
                      $'='  FENCE(*Expr0 | (epsilon) . thx . *Shift('TT_QLIT', thx)) . *Reduce('TT_ASSIGN', 2)
                    | $'  ' '=' $';' (epsilon) . thx . *Shift('TT_QLIT', thx) . *Reduce('TT_ASSIGN', 2)
                    | $'+=' *Expr0 . *Reduce('TT_AUGOP', 2)
                    | $'-=' *Expr0 . *Reduce('TT_AUGOP', 2)
                    | $'*=' *Expr0 . *Reduce('TT_AUGOP', 2)
                    | $'/=' *Expr0 . *Reduce('TT_AUGOP', 2)
                    | $'^=' *Expr0 . *Reduce('TT_AUGOP', 2)
                    | epsilon);
/* ==================================================================================================================== */
/* Statement grammar — pure shift/reduce.  SC-SC-2.                                       */
/* ==================================================================================================================== */
/* Recurring body shape: brace-delimited list of Commands → TT_PROGRAM node. */
ThenBlock       =   epsilon . *PushCounter() $'{' ARBNO(*Command) $'}' . *Reduce('TT_PROGRAM', nTop()) . *PopCounter();
/* if_cmd → TT_IF(cond, then_block) or TT_IF(cond, then_block, else_block) */
ElseBranch      =   $'else' *ForBody;
if_cmd          =   $'if' $'(' *Expr0 $')' *ForBody
                    FENCE(*ElseBranch . *Reduce('TT_IF', 3) | epsilon . *Reduce('TT_IF', 2));
/* while_cmd → TT_WHILE(cond, body) */
while_cmd       =   $'while' $'(' *Expr0 $')' *ForBody . *Reduce('TT_WHILE', 2);
/* do_cmd → TT_DO_WHILE(body, cond) */
do_cmd          =   $'do' *ThenBlock $'while' $'(' *Expr0 $')' ($';' | epsilon)
                    . *Reduce('TT_DO_WHILE', 2);
/* for_cmd → TT_FOR(init, cond, step, body) */
ForBody         =   *ThenBlock
                |   epsilon . *PushCounter() *Command . *Reduce('TT_PROGRAM', nTop()) . *PopCounter();
for_cmd         =   $'for' $'(' *Expr0 $';' *Expr0 $';' *Expr0 $')'
                    *ForBody . *Reduce('TT_FOR', 4);
/* switch_cmd → TT_CASE(disc, val0, body0, val1, body1, …) */
CaseArm         =   $'case' *Expr0 $':' . *IncCounter() . *PushCounter() ARBNO(*Command)
                    . *Reduce('TT_PROGRAM', nTop()) . *PopCounter() . *IncCounter();
DefaultArm      =   $'default' $':' (epsilon) . thx . *Shift('TT_NUL', thx) . *IncCounter()
                    . *PushCounter() ARBNO(*Command) . *Reduce('TT_PROGRAM', nTop()) . *PopCounter() . *IncCounter();
switch_cmd      =   epsilon . *PushCounter()
                    $'switch' $'(' *Expr0 $')' . *IncCounter()
                    $'{' ARBNO(*CaseArm | *DefaultArm) $'}'
                    . *Reduce('TT_CASE', nTop())
                    . *PopCounter();
/* func_cmd → TT_DEFINE(QLIT(name), TT_PARAMS(params…), TT_PROGRAM(body)) */
ParamFirst      =   (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter();
ParamRest       =   $',' (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter();
Params          =   epsilon . *PushCounter() (*ParamFirst ARBNO(*ParamRest) | epsilon)
                    . *Reduce('TT_PARAMS', nTop()) . *PopCounter();
Locals          =   epsilon . *PushCounter() ($' ' *ParamFirst ARBNO(*ParamRest) | epsilon)
                    . *Reduce('TT_LOCALS', nTop()) . *PopCounter();
func_cmd        =   $'function' (*Ident) . thx . *Shift('TT_QLIT', thx)
                    $'(' *Params $')' *Locals *ThenBlock
                    . *Reduce('TT_DEFINE', 4);
/* return_cmd / freturn_cmd / nreturn_cmd */
return_cmd      =   $'return' ( *Expr0 $';' . *Reduce('TT_RETURN', 1)
                               |        $';' . *Reduce('TT_RETURN', 0) );
freturn_cmd     =   $'freturn' $';' . *Reduce('TT_PROC_FAIL', 0);
nreturn_cmd     =   $'nreturn' $';' . *Reduce('TT_NRETURN', 0);
/* goto_cmd / label_prefix */
goto_cmd        =   $'goto' (*Ident) . thx . *Shift('TT_QLIT', thx) $';' . *Reduce('TT_GOTO_U', 1);
label_prefix    =   (*Ident) . thx . *Shift('TT_QLIT', thx) $':'       . *Reduce('TT_LABEL', 1);
/* break_cmd / continue_cmd */
break_cmd       =   $'break'
                    ( (*Ident) . thx . *Shift('TT_QLIT', thx) $';' . *Reduce('TT_LOOP_BREAK', 1)
                    | $';'                            . *Reduce('TT_LOOP_BREAK', 0) );
continue_cmd    =   $'continue'
                    ( (*Ident) . thx . *Shift('TT_QLIT', thx) $';' . *Reduce('TT_LOOP_NEXT', 1)
                    | $';'                            . *Reduce('TT_LOOP_NEXT', 0) );
/* struct_cmd → TT_STRUCT(name, fields) */
StructFieldFirst =  (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter();
StructFieldRest  =  $',' (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter();
StructFields     =  epsilon . *PushCounter() (*StructFieldFirst ARBNO(*StructFieldRest) | epsilon)
                    . *Reduce('TT_FIELDS', nTop()) . *PopCounter();
struct_cmd       =  $'struct' (*Ident) . thx . *Shift('TT_QLIT', thx)
                    $'{' *StructFields $'}' . *Reduce('TT_STRUCT', 2);
/* stmt_cmd — subject/pattern decomposition removed (lower's job per PST-SC-4l) */
stmt_body       =   *Expr0 ($';' | epsilon);
stmt_cmd        =   *stmt_body;
/* empty_cmd */
empty_cmd       =   $';';
block_cmd       =   $'{' ARBNO(*Command) $'}';
/* Command dispatcher */
Command         =   $' ' FENCE( *empty_cmd
                    | *block_cmd
                    | $'case' *CaseArm
                    | *DefaultArm
                    | epsilon . *IncCounter() ( *if_cmd
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
                    | *label_prefix
                    | *stmt_cmd
                    ) );
/* Compiland — top-level program */
Compiland       =   epsilon . *PushCounter() POS(0) ARBNO(*Command) $' ' RPOS(0)
                    . *Reduce('Parse', nTop()) . *PopCounter();
/* ==================================================================================================================== */
/* Driver                                                                                  */
/* ==================================================================================================================== */
function ParseOne(ptree, i, n_kids) {
    InitCounter();
    InitStack();
    if (Src ? Compiland) {
        ptree = Pop();
        if (DIFFER(ptree)) {
            i = 1;
            n_kids = n(ptree);
            while (LE(i, n_kids)) {
                TreeDump(c(ptree)[i]);
                i = i + 1;
            }
        }
    } else OUTPUT = 'Parse Error';
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
    INPUT(.pf_names, 8, pf_list);
    pf_t0 = TIME();
    pf_t1 = pf_t0;
    while (pf_name = pf_names) {
        Src = '';
        INPUT(.pf_file, 9, pf_name '[-r16777215]');
        Src = pf_file;
        ENDFILE(9);
        pf_bytes = pf_bytes + SIZE(Src);
        OUTPUT = '== ' pf_name;
        ParseOne();
        pf_n = pf_n + 1;
        if (EQ(pf_n, 1)) pf_t1 = TIME();
    }
    pf_t2 = TIME();
    TERMINAL = 'PARSER-METRICS files=' pf_n ' bytes=' pf_bytes ' first_us=' (pf_t1 - pf_t0) / 1000 ' rest_us=' (pf_t2 - pf_t1) / 1000 ' total_us=' (pf_t2 - pf_t0) / 1000;
}
