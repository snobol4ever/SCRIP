&FULLSCAN = 1;
reserved          = POS(0) ('if' | 'else' | 'while' | 'do' | 'for') RPOS(0);
/* ==================================================================================================================== */
/* notmatch() is provided by corpus/SCRIP/match.sc (loaded first in the runtime chain by  */
/* run_parser_sync_monitor.sh / run_scrip_parser.sh).  Redefining it here triggered       */
/* SPITBOL ERROR 217 duplicate-label when the transpiled .sno was run under SPITBOL with  */
/* match.sc included in the prelude.  SCT-9 (Opus 4.7, 2026-05-18).                       */
white       =   (  SPAN(' ' CHAR(9) CHAR(10))
                |  '//' BREAK(CHAR(10)) CHAR(10)
                |  '/*' BREAK('*') '*' ARBNO('*' | NOTANY('/*') BREAK('*') '*') '/'
                );
White       =   *white ARBNO(*white);
Gray        =   *White | epsilon;
$'  '       =   White;
$' '        =   Gray;
Id          =   ANY(&UCASE &LCASE '_') FENCE(SPAN('.' '0123456789' &UCASE '_' &LCASE) | epsilon);
$'break'    =   *$' ' *Id $ tx *IDENT(tx, 'break')    *$' ';
$'case'     =   *$' ' *Id $ tx *IDENT(tx, 'case')     *$' ';
$'continue' =   *$' ' *Id $ tx *IDENT(tx, 'continue') *$' ';
$'default'  =   *$' ' *Id $ tx *IDENT(tx, 'default')  *$' ';
$'do'       =   *$' ' *Id $ tx *IDENT(tx, 'do')       *$' ';
$'else'     =   *$' ' *Id $ tx *IDENT(tx, 'else')     *$' ';
$'for'      =   *$' ' *Id $ tx *IDENT(tx, 'for')      *$' ';
$'freturn'  =   *$' ' *Id $ tx *IDENT(tx, 'freturn')  *$' ';
$'function' =   *$' ' *Id $ tx (*IDENT(tx, 'function') | *IDENT(tx, 'procedure')) *$' ';
$'goto'     =   *$' ' *Id $ tx *IDENT(tx, 'goto')     *$' ';
$'if'       =   *$' ' *Id $ tx *IDENT(tx, 'if')       *$' ';
$'nreturn'  =   *$' ' *Id $ tx *IDENT(tx, 'nreturn')  *$' ';
$'return'   =   *$' ' *Id $ tx *IDENT(tx, 'return')   *$' ';
$'struct'   =   *$' ' *Id $ tx *IDENT(tx, 'struct')   *$' ';
$'switch'   =   *$' ' *Id $ tx *IDENT(tx, 'switch')   *$' ';
$'while'    =   *$' ' *Id $ tx *IDENT(tx, 'while')    *$' ';
Keyword     =   '&' (SPAN(&UCASE '_' &LCASE)) . thx . *Shift('TT_KEYWORD', thx);
Integer     =   SPAN('0123456789') . token;
DQ_lit      =   '"' (BREAK('"')) . thx . *Shift('TT_QLIT', thx) '"';
SQ_lit      =   "'" (BREAK("'")) . thx . *Shift('TT_QLIT', thx) "'";
String      =   (*SQ_lit | *DQ_lit);
ResT        =   TABLE(5);
ResT['if'] = 1; ResT['else'] = 1; ResT['while'] = 1; ResT['do'] = 1; ResT['for'] = 1;
Ident       =   *Id $ tx *IDENT(ResT[tx]) . token;
Real        =   ( SPAN('0123456789')
                  FENCE(
                    '.'
                    SPAN('0123456789')
                    FENCE(ANY('eEdD') FENCE(ANY('+-') | epsilon) SPAN('0123456789') | epsilon)
                  | ANY('eEdD')
                    FENCE(ANY('+-') | epsilon)
                    SPAN('0123456789')
                  )
                ) . token;
$'('        =   '(' *$' ';
$'['        =   '[' *$' ';
$'{'        =   *$' ' '{' *$' ';
$')'        =   *$' ' ')';
$'}'        =   *$' ' '}';
$']'        =   *$' ' ']';
$','        =   *$' ' ',' *$' ';
$':'        =   *$' ' ':' *$' ';
$';'        =   *$' ' ';' *$' ';
$'='        =   *$'  ' '='   *$'  ';
$'?'        =   *$'  ' '?'   *$'  ';
$'|'        =   *$'  ' '|'   *$'  ';
$'+'        =   *$'  ' '+'   *$'  ';
$'-'        =   *$'  ' '-'   *$'  ';
$'*'        =   *$'  ' '*'   *$'  ';
$'/'        =   *$'  ' '/'   *$'  ';
$'^'        =   *$'  ' '^'   *$'  ';
$'**'       =   *$'  ' '**'  *$'  ';
$'!'        =   *$'  ' '!'   *$'  ';
$'$'        =   *$'  ' '$'   *$'  ';
$'.'        =   *$'  ' '.'   *$'  ';
$'@'        =   *$'  ' '@'   *$'  ';
$'#'        =   *$'  ' '#'   *$'  ';
$'%'        =   *$'  ' '%'   *$'  ';
$'~'        =   *$'  ' '~'   *$'  ';
$'+='       =   *$'  ' '+='  *$'  ';
$'-='       =   *$'  ' '-='  *$'  ';
$'*='       =   *$'  ' '*='  *$'  ';
$'/='       =   *$'  ' '/='  *$'  ';
$'^='       =   *$'  ' '^='  *$'  ';
/* ==================================================================================================================== */
/* Expression grammar — shift/reduce only.  SC-SC-3: augop, cmp, paren, idx rewritten.   */
/* ==================================================================================================================== */
ArgFirst        =   *Expr0 . *IncCounter();
ArgRest         =   *$',' *Expr0 . *IncCounter();
CallArgs        =   epsilon . *PushCounter() (*ArgFirst ARBNO(*ArgRest) | epsilon) . *Reduce('TT_ARGS', nTop()) . *PopCounter();
Call            =   (*Ident) . thx . *Shift('TT_QLIT', thx)
                    FENCE(
                      *$'('
                      *CallArgs
                      *$')'
                      . *Reduce('TT_FNC', 2)
                    );
ExprList        =   epsilon . *PushCounter() *XList . *Reduce('TT_VLIST', nTop()) . *PopCounter();
XList           =   epsilon . *IncCounter() (*Expr0 | (epsilon) . thx . *Shift('', thx)) FENCE(*$',' *XList | epsilon);
Expr17          =   FENCE(
                      *Call
                    | *$'('
                      FENCE(
                        epsilon . *PushCounter() . *IncCounter() *Expr0 ARBNO(*$',' . *IncCounter() *Expr0)
                        FENCE( epsilon . *Reduce('TT_VLIST', *(GT(nTop(), 1) nTop())) | epsilon )
                        . *PopCounter()
                        *$')'
                      | *$')' (epsilon) . thx . *Shift('TT_NUL', thx)
                      )
                    | *String
                    | (*Real) . thx . *Shift('TT_FLIT', thx)
                    | (*Integer) . thx . *Shift('TT_ILIT', thx)
                    | *Keyword
                    | (*Ident) . thx . *Shift('TT_VAR', thx)
                    );
Expr15          =   *Expr17 *Expr15t;
Expr15t         =   FENCE(*$'[' *ExprList *$']' . *Reduce('TT_IDX', 2) *Expr15t | epsilon);
Expr14          =   '@' *Expr14 . *Reduce('TT_CAPT_CURSOR', 1)
                |   '~' *Expr14 . *Reduce('TT_NOT', 1)
                |   '+' *Expr14 . *Reduce('TT_PLS', 1)
                |   '-' *Expr14 . *Reduce('TT_MNS', 1)
                |   '*' *Expr14 . *Reduce('TT_DEFER', 1)
                |   '$' *Expr14 . *Reduce('TT_INDIRECT', 1)
                |   '.' *Expr14 . *Reduce('TT_NAME', 1)
                |   '!' *Expr14 . *Reduce('TT_OPSYN', 1, '!')
                |   '?' *Expr14 . *Reduce('TT_INTERROGATE', 1)
                |   '%' *Expr14 . *Reduce('TT_OPSYN', 1, '%')
                |   '/' *Expr14 . *Reduce('TT_OPSYN', 1, '/')
                |   '#' *Expr14 . *Reduce('TT_OPSYN', 1, '#')
                |   '|' *Expr14 . *Reduce('TT_OPSYN', 1, '|')
                |   '=' *Expr14 . *Reduce('TT_OPSYN', 1, '=')
                |   *Expr15
                |   '&' *Expr14 . *Reduce('TT_OPSYN', 1, '&');
Expr13          =   *Expr14 FENCE(*$'  ' '~' *$'  ' *Expr13 . *Reduce('TT_OPSYN', 2, '~') | epsilon);
Expr12          =   *Expr13 *Expr12t;
Expr12t         =   FENCE(
                      *$'  ' ('$' *$'  ' *Expr13 . *Reduce('TT_CAPT_IMMED_ASGN', 2) *Expr12t
                    | '.' *$'  ' *Expr13 . *Reduce('TT_CAPT_COND_ASGN', 2) *Expr12t
                    ) | epsilon
                    );
Expr11          =   *Expr12 FENCE((*$'  ' ('^' *$'  ' | '!' *$'  ' | '**' *$'  ')) *Expr11 . *Reduce('TT_POW', 2) | epsilon);
Expr10          =   *Expr11 *Expr10t;
Expr10t         =   FENCE(*$'  ' '%' *$'  ' *Expr11 . *Reduce('TT_OPSYN', 2, '%') *Expr10t | epsilon);
Expr9           =   *Expr10 *Expr9t;
Expr9t          =   FENCE(*$'*' *Expr10 . *Reduce('TT_MUL', 2) *Expr9t | epsilon);
Expr8           =   *Expr9 *Expr8t;
Expr8t          =   FENCE(*$'/' *Expr9 . *Reduce('TT_DIV', 2) *Expr8t | epsilon);
Expr7           =   *Expr8 *Expr7t;
Expr7t          =   FENCE(*$'  ' '#' *$'  ' *Expr8 . *Reduce('TT_OPSYN', 2, '#') *Expr7t | epsilon);
Expr6           =   *Expr7 *Expr6t;
Expr6t          =   FENCE(*$'  ' ('+' *$'  ' *Expr7 . *Reduce('TT_ADD', 2) *Expr6t | '-' *$'  ' *Expr7 . *Reduce('TT_SUB', 2) *Expr6t) | epsilon);
Expr5           =   *Expr6 FENCE(*$'  ' '@' *$'  ' *Expr5 . *Reduce('TT_OPSYN', 2, '@') | epsilon);
Expr4           =   *Expr5 *Expr4t;
Expr4t          =   FENCE(*$'  ' *Expr5 . *Reduce('TT_SEQ', 2) *Expr4t | epsilon);
Expr3           =   *Expr4 *Expr3t;
Expr3t          =   FENCE(*$'|' *Expr4 . *Reduce('TT_ALT', 2) *Expr3t | epsilon);
Expr2           =   *Expr3 *Expr2t;
Expr2t          =   FENCE(*$'  ' '&' *$'  ' *Expr3 . *Reduce('TT_OPSYN', 2, '&') *Expr2t | epsilon);
Expr1           =   *Expr2 FENCE(*$'?' *Expr1 . *Reduce('TT_SCAN', 2) | epsilon);
Expr0           =   *Expr1 FENCE(
                      *$'='  FENCE(*Expr0 | (epsilon) . thx . *Shift('TT_QLIT', thx)) . *Reduce('TT_ASSIGN', 2)
                    | *$'  ' '=' *$';' (epsilon) . thx . *Shift('TT_QLIT', thx) . *Reduce('TT_ASSIGN', 2)
                    | *$'  ' ('+=' *$'  ' *Expr0 . *Reduce('TT_AUGOP', 2)
                    | '-=' *$'  ' *Expr0 . *Reduce('TT_AUGOP', 2)
                    | '*=' *$'  ' *Expr0 . *Reduce('TT_AUGOP', 2)
                    | '/=' *$'  ' *Expr0 . *Reduce('TT_AUGOP', 2)
                    | '^=' *$'  ' *Expr0 . *Reduce('TT_AUGOP', 2)
                    ) | epsilon);
/* ==================================================================================================================== */
/* Statement grammar — pure shift/reduce.  SC-SC-2.                                       */
/* ==================================================================================================================== */
/* Recurring body shape: brace-delimited list of Commands → TT_PROGRAM node. */
ThenBlock       =   epsilon . *PushCounter() *$'{' ARBNO(*Command) *$'}' . *Reduce('TT_PROGRAM', nTop()) . *PopCounter();
/* if_cmd → TT_IF(cond, then_block) or TT_IF(cond, then_block, else_block) */
ElseBranch      =   *$'else' *ForBody;
if_cmd_rest     =   *$' ' *$'(' *Expr0 *$')' *ForBody
                    FENCE(*ElseBranch . *Reduce('TT_IF', 3) | epsilon . *Reduce('TT_IF', 2));
/* while_cmd → TT_WHILE(cond, body) */
while_cmd_rest  =   *$' ' *$'(' *Expr0 *$')' *ForBody . *Reduce('TT_WHILE', 2);
/* do_cmd → TT_DO_WHILE(body, cond) */
do_cmd_rest     =   *$' ' *ThenBlock *$'while' *$'(' *Expr0 *$')' FENCE(*$';' | epsilon)
                    . *Reduce('TT_DO_WHILE', 2);
/* for_cmd → TT_FOR(init, cond, step, body) */
ForBody         =   *ThenBlock
                |   epsilon . *PushCounter() *Command . *Reduce('TT_PROGRAM', nTop()) . *PopCounter();
for_cmd_rest    =   *$' ' *$'(' *Expr0 *$';' *Expr0 *$';' *Expr0 *$')'
                    *ForBody . *Reduce('TT_FOR', 4);
/* switch_cmd → TT_CASE(disc, val0, body0, val1, body1, …) */
CaseArm         =   *$'case' *Expr0 *$':' . *IncCounter() . *PushCounter() ARBNO(*Command)
                    . *Reduce('TT_PROGRAM', nTop()) . *PopCounter() . *IncCounter();
DefaultArm      =   *$'default' *$':' (epsilon) . thx . *Shift('TT_NUL', thx) . *IncCounter()
                    . *PushCounter() ARBNO(*Command) . *Reduce('TT_PROGRAM', nTop()) . *PopCounter() . *IncCounter();
switch_cmd_rest =   epsilon . *PushCounter() *$' ' *$'(' *Expr0 *$')' . *IncCounter()
                    *$'{' ARBNO(*CaseArm | *DefaultArm) *$'}'
                    . *Reduce('TT_CASE', nTop())
                    . *PopCounter();
/* func_cmd → TT_DEFINE(QLIT(name), TT_PARAMS(params…), TT_PROGRAM(body)) */
ParamFirst      =   (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter();
ParamRest       =   *$',' (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter();
Params          =   epsilon . *PushCounter() (*ParamFirst ARBNO(*ParamRest) | epsilon)
                    . *Reduce('TT_PARAMS', nTop()) . *PopCounter();
Locals          =   epsilon . *PushCounter() (*$' ' *ParamFirst ARBNO(*ParamRest) | epsilon)
                    . *Reduce('TT_LOCALS', nTop()) . *PopCounter();
func_cmd_rest   =   *$' ' (*Ident) . thx . *Shift('TT_QLIT', thx)
                    *$'(' *Params *$')' *Locals *ThenBlock
                    . *Reduce('TT_DEFINE', 4);
/* return_cmd / freturn_cmd / nreturn_cmd */
return_cmd_rest =   *$' ' ( *Expr0 *$';' . *Reduce('TT_RETURN', 1)
                               |        *$';' . *Reduce('TT_RETURN', 0) );
freturn_cmd_rest =   *$' ' *$';' . *Reduce('TT_PROC_FAIL', 0);
nreturn_cmd_rest =   *$' ' *$';' . *Reduce('TT_NRETURN', 0);
/* goto_cmd / label_prefix */
goto_cmd_rest   =   *$' ' (*Ident) . thx . *Shift('TT_QLIT', thx) *$';' . *Reduce('TT_GOTO_U', 1);
label_prefix    =   (*Ident) . thx . *Shift('TT_QLIT', thx) *$':'       . *Reduce('TT_LABEL', 1);
/* break_cmd / continue_cmd */
break_cmd_rest  =   *$' '
                    ( (*Ident) . thx . *Shift('TT_QLIT', thx) *$';' . *Reduce('TT_LOOP_BREAK', 1)
                    | *$';'                            . *Reduce('TT_LOOP_BREAK', 0) );
continue_cmd_rest =   *$' '
                    ( (*Ident) . thx . *Shift('TT_QLIT', thx) *$';' . *Reduce('TT_LOOP_NEXT', 1)
                    | *$';'                            . *Reduce('TT_LOOP_NEXT', 0) );
/* struct_cmd → TT_STRUCT(name, fields) */
StructFieldFirst =  (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter();
StructFieldRest  =  *$',' (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter();
StructFields     =  epsilon . *PushCounter() (*StructFieldFirst ARBNO(*StructFieldRest) | epsilon)
                    . *Reduce('TT_FIELDS', nTop()) . *PopCounter();
struct_cmd_rest =   *$' ' (*Ident) . thx . *Shift('TT_QLIT', thx)
                    *$'{' *StructFields *$'}' . *Reduce('TT_STRUCT', 2);
/* stmt_cmd — subject/pattern decomposition removed (lower's job per PST-SC-4l) */
stmt_body       =   *Expr0 FENCE(*$';' | epsilon);
stmt_cmd        =   *stmt_body;
/* empty_cmd */
empty_cmd       =   *$';';
block_cmd       =   *$'{' ARBNO(*Command) *$'}';
CmdT            =   TABLE(14);
CmdT['if']          = if_cmd_rest;
CmdT['while']       = while_cmd_rest;
CmdT['do']          = do_cmd_rest;
CmdT['for']         = for_cmd_rest;
CmdT['function']    = func_cmd_rest;
CmdT['return']      = return_cmd_rest;
CmdT['freturn']     = freturn_cmd_rest;
CmdT['nreturn']     = nreturn_cmd_rest;
CmdT['goto']        = goto_cmd_rest;
CmdT['break']       = break_cmd_rest;
CmdT['continue']    = continue_cmd_rest;
CmdT['struct']      = struct_cmd_rest;
CmdT['switch']      = switch_cmd_rest;
CmdT['procedure']   = func_cmd_rest;
kw_cmd          =   *$' ' *Id $ tx *DIFFER(CmdT[tx]) *CmdT[tx];
/* Command dispatcher */
Command         =   *$' ' FENCE( *empty_cmd
                    | *block_cmd
                    | *$'case' *CaseArm
                    | *DefaultArm
                    | epsilon . *IncCounter() ( *kw_cmd
                    | *label_prefix
                    | *stmt_cmd
                    ) );
/* Compiland — top-level program */
Compiland       =   epsilon . *PushCounter() POS(0) ARBNO(*Command FLUSH) *$' ' RPOS(0)
                    . *Reduce('Parse', nTop()) . *PopCounter();
/* ==================================================================================================================== */
/* Driver                                                                                  */
/* ==================================================================================================================== */
function ParseOne(ptree, i, n_kids) {
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
        }
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
