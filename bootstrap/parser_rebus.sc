/* parser_rebus.sc -- Rebus (Griswold, TR 84-9) as ONE pattern, built only from shift / reduce / nPush / nInc / nTop / nPop.
   ⛔ THE STATEMENT GRAMMAR IS src/parsers/rebus/rebus.y AND THE STATEMENT SEPARATOR IS rebus.l's SEMICOLON RULE, mirrored
   here rather than re-invented (hq_snocone 2026-09-23, on Lon's order to get every parser_*.sc parsing the whole corpus):
     - a newline ENDS a statement exactly where rebus.l inserts ';' -- after an identifier, a literal, a keyword
       reference, ')' ']' '}' and the words end/exit/next/fail/stop/return -- so an operator, an opener, ',' or one of
       the words then/do/else/of/from/to/by leaves the newline as plain blank space (GrayNL below);
     - a line that STARTS with then, do or else continues the statement before it (rebus.l next_is_continuation);
     - ';' is always a separator, and blank lines and '#' comment lines between statements are skipped.
   THE TREE uses the C parser's own node kinds (ast.h), so a program parsed here and by rebus.y has the same shape:
     TT_FUNCTION(name, TT_VLIST params, TT_VLIST locals, initial|TT_NUL, TT_PROGRAM body) · TT_RECORD_DECL(name, fields...)
     TT_IF(c,t[,e]) TT_UNLESS TT_WHILE TT_UNTIL TT_REPEAT · TT_FOR(var, from, to, by|TT_NUL, body) · TT_CASE(e, g1, b1, ...,
     TT_NUL, default-body) · TT_RETURN([e]) TT_PROC_FAIL TT_LOOP_BREAK(exit) TT_LOOP_NEXT(next) TT_END(stop) ·
     TT_SCAN(s, p[, r|TT_NUL]) · TT_ASSIGN TT_SWAP TT_AUGOP(lhs, op-kind leaf, rhs) · TT_ALT TT_CAT TT_EQ.. TT_LGE TT_ADD
     TT_SUB TT_MUL TT_DIV TT_MOD TT_POW · TT_MNS TT_NOT TT_NONNULL TT_ITERATE TT_INDIRECT TT_CAPT_CURSOR TT_CAPT_COND_ASGN
     TT_CAPT_IMMED_ASGN · TT_FNC(callee, args...) TT_IDX(base, args...) -- where rebus.y keeps a value in a node's ival or
     sval (the for-variable, the augmented operator) it is the node's FIRST child here (the operator: its binary kind as a
     leaf, between the operands), because Reduce builds no valued interior node.
   ⭐ Identifiers keep the case they were written in; rebus.l upcases them, which is a lowering decision, not a parse. */
&FULLSCAN = 1;
/* ---- blanks: GREEDY and COMMITTED. A lazy ARBNO(white) matched zero blanks first, so an alternative that succeeds on
   nothing (an empty argument, a bare `return`) won before the blank was consumed and a FENCE forbade the backtrack. */
white       =   SPAN(' ' tab) | '#' BREAK(nl);
White       =   white FENCE(*White | epsilon);
Gray        =   FENCE(*White | epsilon);
whitenl     =   white | nl;
WhiteNL     =   whitenl FENCE(*WhiteNL | epsilon);
GrayNL      =   FENCE(*WhiteNL | epsilon);
$' '        =   Gray;
$'  '       =   White;
/* ---- statement separators: ';' or a newline, with any number of blank or comment lines */
Sep         =   Gray (';' | nl);
Seps        =   FENCE(*Sep *Seps | epsilon);
/* ---- words: an identifier is the WHOLE run of identifier characters (rebus.l IDENT), so a keyword never matches the
   front of a longer name (`returned`, `format`); keywords are reserved and case-insensitive, as in rebus.l */
IdStart     =   &UCASE &LCASE '_' X1xxxxxxx;
IdChars     =   &UCASE &LCASE digits '_' '.' X1xxxxxxx;
Word        =   (ANY(IdStart) FENCE(SPAN(IdChars) | epsilon)) $ tx;
RbKw        =   TABLE();
RbKw['by'] = 1;       RbKw['case'] = 1;    RbKw['default'] = 1; RbKw['do'] = 1;      RbKw['else'] = 1;
RbKw['end'] = 1;      RbKw['exit'] = 1;    RbKw['fail'] = 1;    RbKw['for'] = 1;     RbKw['from'] = 1;
RbKw['function'] = 1; RbKw['if'] = 1;      RbKw['initial'] = 1; RbKw['local'] = 1;   RbKw['next'] = 1;
RbKw['of'] = 1;       RbKw['record'] = 1;  RbKw['repeat'] = 1;  RbKw['return'] = 1;  RbKw['stop'] = 1;
RbKw['then'] = 1;     RbKw['to'] = 1;      RbKw['unless'] = 1;  RbKw['until'] = 1;   RbKw['while'] = 1;
Ident       =   *Word *IDENT(RbKw[REPLACE(tx, &UCASE, &LCASE)]);
kw_by       =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'by')       GrayNL;
kw_case     =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'case')     GrayNL;
kw_default  =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'default')  Gray;
kw_do       =   GrayNL *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'do')     GrayNL;
kw_else     =   GrayNL *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'else')   GrayNL;
kw_end      =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'end')      Gray;
kw_exit     =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'exit')     Gray;
kw_fail     =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'fail')     Gray;
kw_for      =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'for')      GrayNL;
kw_from     =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'from')     GrayNL;
kw_function =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'function') GrayNL;
kw_if       =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'if')       GrayNL;
kw_initial  =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'initial')  GrayNL;
kw_local    =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'local')    GrayNL;
kw_next     =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'next')     Gray;
kw_of       =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'of')       GrayNL;
kw_record   =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'record')   GrayNL;
kw_repeat   =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'repeat')   GrayNL;
kw_return   =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'return')   Gray;
kw_stop     =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'stop')     Gray;
kw_then     =   GrayNL *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'then')   GrayNL;
kw_to       =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'to')       GrayNL;
kw_unless   =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'unless')   GrayNL;
kw_until    =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'until')    GrayNL;
kw_while    =   Gray *Word *IDENT(REPLACE(tx, &UCASE, &LCASE), 'while')    GrayNL;
/* ---- operators: a newline after an operator or an opener never ends the statement (rebus.l needs_semi) */
$'('        =   Gray '('    GrayNL;     $')'   = Gray ')';
$'['        =   Gray '['    GrayNL;     $']'   = Gray ']';
$'{'        =   Gray '{'    GrayNL;     $'}'   = GrayNL '}';
$','        =   Gray ','    GrayNL;     $':'   = Gray ':'   GrayNL;
$':=:'      =   Gray ':=:'  GrayNL;     $':='  = Gray ':='  GrayNL;
$'||:='     =   Gray '||:=' GrayNL;     $'+:=' = Gray '+:=' GrayNL;    $'-:=' = Gray '-:=' GrayNL;
$'?-'       =   Gray '?-'   GrayNL;     $'?'   = Gray '?'   GrayNL;    $'<-'  = Gray '<-'  GrayNL;
$'|'        =   Gray '|'    GrayNL;     $'||'  = Gray '||'  GrayNL;    $'&'   = Gray '&'   GrayNL;
$'~=='      =   Gray '~=='  GrayNL;     $'=='  = Gray '=='  GrayNL;    $'~='  = Gray '~='  GrayNL;
$'<<='      =   Gray '<<='  GrayNL;     $'>>=' = Gray '>>=' GrayNL;    $'<<'  = Gray '<<'  GrayNL;
$'>>'       =   Gray '>>'   GrayNL;     $'<='  = Gray '<='  GrayNL;    $'>='  = Gray '>='  GrayNL;
$'='        =   Gray '='    GrayNL;     $'>'   = Gray '>'   GrayNL;
/* '<' is never the front of '<-': `s ? p <- r` must not read as `s ? p < -r` (rebus.l munches '<-' whole) */
$'<'        =   Gray '<' @ltcur (RPOS(0) | *LNE(SUBSTR(Src, ltcur + 1, 1), '-')) GrayNL;
$'+'        =   Gray '+'    GrayNL;     $'-'   = Gray '-'   GrayNL;
$'*'        =   Gray '*'    GrayNL;     $'/'   = Gray '/'   GrayNL;    $'%'   = Gray '%'   GrayNL;
$'^'        =   Gray '^'    GrayNL;     $'**'  = Gray '**'  GrayNL;
$'.'        =   Gray '.'    GrayNL;     $'$'   = Gray '$'   GrayNL;    $'+:'  = Gray '+:'  GrayNL;
/* ---- literals (rebus.l): strings have no escapes; a real has a '.' or an exponent */
Integer     =   SPAN(digits);
Exponent    =   ANY('eE') FENCE(ANY('+-') | epsilon) SPAN(digits);
Real        =   SPAN(digits) ('.' FENCE(SPAN(digits) | epsilon) FENCE(Exponent | epsilon) | Exponent);
/* ---- expressions (rebus.y: assign < alt < cat < cmp < add < mul < pow < unary < postfix < primary) */
args_tail   =   $',' nInc() FENCE(*expr | shift(epsilon, 'TT_NUL')) FENCE(*args_tail | epsilon);
arglist     =   FENCE(nInc() *expr FENCE(*args_tail | epsilon) | epsilon);
primary     =   FENCE(  '"' shift(BREAK('"'), 'TT_QLIT') '"'
                     |  "'" shift(BREAK("'"), 'TT_QLIT') "'"
                     |  '&' shift(*Word, 'TT_KEYWORD')
                     |  shift(*Real, 'TT_FLIT')
                     |  shift(*Integer, 'TT_ILIT')
                     |  shift(*Ident, 'TT_VAR')
                     |  '(' GrayNL *expr $')'
                     );
postfix_tail =  FENCE(  nPush() $'(' *arglist $')' reduce('TT_FNC', 'nTop() + 1') nPop()
                     |  $'[' *expr $'+:' *expr $']' reduce('TT_IDX', 3)
                     |  nPush() $'[' *arglist $']' reduce('TT_IDX', 'nTop() + 1') nPop()
                     |  $'.' *primary reduce('TT_CAPT_COND_ASGN', 2)
                     |  $'$' *primary reduce('TT_CAPT_IMMED_ASGN', 2)
                     )
                FENCE(*postfix_tail | epsilon);
postfix_expr =  *primary FENCE(*postfix_tail | epsilon);
unary_expr  =   FENCE(  '-'  GrayNL *unary_expr reduce('TT_MNS', 1)
                     |  '+'  GrayNL *unary_expr
                     |  '~'  GrayNL *unary_expr reduce('TT_NOT', 1)
                     |  '\'  GrayNL *unary_expr reduce('TT_NOT', 1)
                     |  '/'  GrayNL *unary_expr reduce('TT_NONNULL', 1)
                     |  '!'  GrayNL *unary_expr reduce('TT_ITERATE', 1)
                     |  '@'  Gray   shift(*Ident, 'TT_CAPT_CURSOR')
                     |  '$'  GrayNL *unary_expr reduce('TT_INDIRECT', 1)
                     |  '.'  GrayNL shift(epsilon, 'TT_NUL') *unary_expr reduce('TT_CAPT_COND_ASGN', 2)
                     |  *postfix_expr
                     );
pow_expr    =   *unary_expr FENCE(($'**' | $'^') *pow_expr reduce('TT_POW', 2) | epsilon);
mul_tail    =   FENCE(  $'*' *pow_expr reduce('TT_MUL', 2)
                     |  $'/' *pow_expr reduce('TT_DIV', 2)
                     |  $'%' *pow_expr reduce('TT_MOD', 2)
                     )
                FENCE(*mul_tail | epsilon);
mul_expr    =   *pow_expr FENCE(*mul_tail | epsilon);
add_tail    =   FENCE($'+' *mul_expr reduce('TT_ADD', 2) | $'-' *mul_expr reduce('TT_SUB', 2)) FENCE(*add_tail | epsilon);
add_expr    =   *mul_expr FENCE(*add_tail | epsilon);
cmp_tail    =   FENCE(  $'~==' *add_expr reduce('TT_LNE', 2)
                     |  $'=='  *add_expr reduce('TT_LEQ', 2)
                     |  $'~='  *add_expr reduce('TT_NE',  2)
                     |  $'<<=' *add_expr reduce('TT_LLE', 2)
                     |  $'>>=' *add_expr reduce('TT_LGE', 2)
                     |  $'<<'  *add_expr reduce('TT_LLT', 2)
                     |  $'>>'  *add_expr reduce('TT_LGT', 2)
                     |  $'<='  *add_expr reduce('TT_LE',  2)
                     |  $'>='  *add_expr reduce('TT_GE',  2)
                     |  $'='   *add_expr reduce('TT_EQ',  2)
                     |  $'<'   *add_expr reduce('TT_LT',  2)
                     |  $'>'   *add_expr reduce('TT_GT',  2)
                     )
                FENCE(*cmp_tail | epsilon);
cmp_expr    =   *add_expr FENCE(*cmp_tail | epsilon);
cat_tail    =   FENCE($'||' *cmp_expr reduce('TT_CAT', 2) | $'&' *cmp_expr reduce('TT_CAT', 2)) FENCE(*cat_tail | epsilon);
cat_expr    =   *cmp_expr FENCE(*cat_tail | epsilon);
alt_tail    =   $'|' *cat_expr reduce('TT_ALT', 2) FENCE(*alt_tail | epsilon);
alt_expr    =   *cat_expr FENCE(*alt_tail | epsilon);
expr        =   *alt_expr
                FENCE(  $':=:'  *expr reduce('TT_SWAP', 2)
                     |  $':='   *expr reduce('TT_ASSIGN', 2)
                     |  $'||:=' reduce('TT_CAT', 0) *expr reduce('TT_AUGOP', 3)
                     |  $'+:='  reduce('TT_ADD', 0) *expr reduce('TT_AUGOP', 3)
                     |  $'-:='  reduce('TT_SUB', 0) *expr reduce('TT_AUGOP', 3)
                     |  epsilon
                     );
/* ---- statements (rebus.y stmt) */
expr_as_stmt =  *expr
                FENCE(  $'?-' *expr shift(epsilon, 'TT_NUL') reduce('TT_SCAN', 3)
                     |  $'?'  *expr FENCE($'<-' *expr reduce('TT_SCAN', 3) | reduce('TT_SCAN', 2))
                     |  epsilon
                     );
opt_semi    =   FENCE(';' GrayNL | epsilon);
stmt_seq    =   nInc() *stmt FENCE(*Sep *Seps FENCE(*stmt_seq | epsilon) | epsilon);
stmt_list   =   nPush() *Seps FENCE(*stmt_seq | epsilon) reduce('TT_PROGRAM', 'nTop()') nPop();
compound_stmt = $'{' *stmt_list $'}';
if_stmt     =   *kw_if *stmt *kw_then *opt_semi *stmt
                FENCE(*kw_else *opt_semi *stmt reduce('TT_IF', 3) | reduce('TT_IF', 2));
unless_stmt =   *kw_unless *stmt *kw_then *opt_semi *stmt reduce('TT_UNLESS', 2);
while_stmt  =   *kw_while  *stmt *kw_do   *opt_semi *stmt reduce('TT_WHILE', 2);
until_stmt  =   *kw_until  *stmt *kw_do   *opt_semi *stmt reduce('TT_UNTIL', 2);
repeat_stmt =   *kw_repeat *opt_semi *stmt reduce('TT_REPEAT', 1);
for_stmt    =   *kw_for shift(*Ident, 'TT_VAR') *kw_from *expr *kw_to *expr
                FENCE(*kw_by *expr | shift(epsilon, 'TT_NUL'))
                *kw_do *opt_semi *stmt reduce('TT_FOR', 5);
case_clause =   Gray nInc() FENCE(*kw_default shift(epsilon, 'TT_NUL') | *expr) nInc() $':' *stmt;
case_list   =   *Seps FENCE(*case_clause FENCE(*Sep *case_list | epsilon) | epsilon);
case_stmt   =   *kw_case nPush() nInc() *expr *kw_of $'{' *case_list $'}' reduce('TT_CASE', 'nTop()') nPop();
return_stmt =   *kw_return FENCE(*expr reduce('TT_RETURN', 1) | reduce('TT_RETURN', 0));
stmt        =   Gray
                FENCE(  *compound_stmt
                     |  *if_stmt
                     |  *unless_stmt
                     |  *while_stmt
                     |  *until_stmt
                     |  *repeat_stmt
                     |  *for_stmt
                     |  *case_stmt
                     |  *return_stmt
                     |  *kw_exit reduce('TT_LOOP_BREAK', 0)
                     |  *kw_next reduce('TT_LOOP_NEXT', 0)
                     |  *kw_fail reduce('TT_PROC_FAIL', 0)
                     |  *kw_stop reduce('TT_END', 0)
                     |  *expr_as_stmt
                     );
/* ---- declarations (rebus.y decl) */
id_tail     =   $',' nInc() shift(*Ident, 'TT_VAR') FENCE(*id_tail | epsilon);
idlist      =   FENCE(nInc() shift(*Ident, 'TT_VAR') FENCE(*id_tail | epsilon) | epsilon);
params      =   nPush() *idlist reduce('TT_VLIST', 'nTop()') nPop();
locals      =   nPush() FENCE(*kw_local *idlist *Sep | epsilon) reduce('TT_VLIST', 'nTop()') nPop();
initial     =   FENCE(*kw_initial FENCE(*compound_stmt | *stmt) | shift(epsilon, 'TT_NUL'));
function_decl = *kw_function shift(*Ident, 'TT_VAR') $'(' *params $')' *Seps
                *locals *Seps *initial *stmt_list *kw_end reduce('TT_FUNCTION', 5);
record_decl =   *kw_record nPush() nInc() shift(*Ident, 'TT_VAR') $'(' *idlist $')' reduce('TT_RECORD_DECL', 'nTop()') nPop();
decls       =   *Seps FENCE(nInc() FENCE(*function_decl | *record_decl) *decls | epsilon);
Compiland   =   nPush() POS(0) *decls Gray RPOS(0) reduce('TT_PROGRAM', 'nTop()') nPop();
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
