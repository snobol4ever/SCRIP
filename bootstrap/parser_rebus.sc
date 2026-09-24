/* PST-RB-SC ✅ 2026-05-19 — already shift/reduce-pure; verified zero violations. */
&FULLSCAN = 1;
white       =   (  SPAN(' ' tab)
                |  '#'  BREAK(nl)
                |  '//' BREAK(nl)
                |  '/*' BREAKX('*') '*/'
                );
White       =   white ARBNO(white);
Gray        =   ARBNO(white);
$'  '       =   White;
$' '        =   Gray;
Id      = ANY(&UCASE &LCASE '_') (SPAN(&UCASE &LCASE digits '_' '.') | epsilon);
Integer = SPAN(digits);
Real    = SPAN(digits) '.' SPAN(digits);
KW_open = '&';
KW_body = ANY(&UCASE &LCASE '_') (SPAN(&UCASE &LCASE digits '_') | epsilon);
DQ_body = BREAK('"');
SQ_body = BREAK("'");
$'('        =       '('        $' ';  $')'        = $' ' ')';
$'['        =       '['        $' ';  $']'        = $' ' ']';
$'.'        = $' '  '.'        $' ';
$','        = $' '  ','        $' ';
$':='       = $' '  ':='       $' ';
$'?'        = $' '  '?'        $' ';
$'|'        = $' '  '|'        $' ';
$'+'        = $' '  '+'        $' ';  $'-'        = $' ' '-'  $' ';
$'*'        = $' '  '*'        $' ';  $'/'        = $' ' '/'  $' ';
$'^'        = $' '  '^'        $' ';  $'**'       = $' ' '**' $' ';
$'%'        = $' '  '%'        $' ';
$'~=='      = $' '  '~=='      $' ';  $'=='       = $' ' '=='  $' ';
$'<<='      = $' '  '<<='      $' ';  $'>>='      = $' ' '>>=' $' ';
$'<<'       = $' '  '<<'       $' ';  $'>>'       = $' ' '>>'  $' ';
$'<='       = $' '  '<='       $' ';  $'>='       = $' ' '>='  $' ';
$'<'        = $' '  '<'        $'  ';  $'>'       = $' ' '>'   $'  ';
$'<-arrow'  = $' '  '<-'       $' ';
$'~='       = $' '  '~='       $' ';  $'='        = $' ' '='   $' ';
$'||'       = $' '  '||'       $' ';  $'&'        = $' ' '&'   $' ';
$'function' = $' '  'function' $'  '; $'end'      = $' ' 'end';
$'record'   = $' '  'record'   $'  ';
$'if'       = $' '  'if'       $'  '; $'then'     = $' ' 'then' $' ';
$'else'     = $' '  'else'     $' ';
$'unless'   = $' '  'unless'   $'  ';
$'for'      = $' '  'for'      $'  ';
$'from'     = $' '  'from'     $'  ';
$'to'       = $' '  'to'       $'  ';
$'by'       = $' '  'by'       $'  ';
$'while'    = $' '  'while'    $'  '; $'do'       = $' ' 'do'   $' ';
$'until'    = $' '  'until'    $'  ';
$'repeat'   = $' '  'repeat'   $'  ';
$'return'   = $' '  'return'   $' ';
$'exit'     = $' '  'exit'     $' ';
$'fail'     = $' '  'fail'     $' ';
$'stop'     = $' '  'stop'     $' ';
$'next'     = $' '  'next'     $' ';
$'local'    = $' '  'local'    $'  ';
$'initial'  = $' '  'initial'  $'  ';
rb_case_kw  = $' '  'case'     $'  ';
$'of'       = $' '  'of'       $' ';
$'<-'       = $' '  '<-'       $' ';
$'?-'       = $' '  '?-'       $' ';
$';'        = $' '  ';'        $' ';
$'{'        = $' '  '{'        $' ';
$'}'        = $' '  '}'        $' ';
$':'        = $' '  ':'        $' ';
$'||:='     = $' '  '||:='     $' ';
$'+:='      = $' '  '+:='      $' ';
$'-:='      = $' '  '-:='      $' ';
$':=:'      = $' '  ':=:'      $' ';
$'+:'       = $' '  '+:'       $' ';
dot_capt    = $'  '  '.'        $' ';
dollar_capt = $'  '  '$'        $' ';
CMP_EQ       = 'CMP_EQ'; CMP_NE = 'CMP_NE';
CMP_LT       = 'CMP_LT'; CMP_LE = 'CMP_LE';
CMP_GT       = 'CMP_GT'; CMP_GE = 'CMP_GE';
CMP_SEQ      = 'CMP_SEQ'; CMP_SNE = 'CMP_SNE';
CMP_SLT      = 'CMP_SLT'; CMP_SLE = 'CMP_SLE';
CMP_SGT      = 'CMP_SGT'; CMP_SGE = 'CMP_SGE';
REMDR        = 'REMDR';
Parse        = 'Parse';
FUNC_DECL = 'FUNC_DECL';
REC_DECL  = 'REC_DECL';
PARAMS    = 'PARAMS';
FIELDS    = 'FIELDS';
LOCALS    = 'LOCALS';
BODY      = 'BODY';
ASSIGN    = 'ASSIGN';
ALT       = 'ALT';
MATCH     = 'MATCH';
IF        = 'IF';
IFELSE    = 'IFELSE';
WHILE     = 'WHILE';
UNLESS    = 'UNLESS';
UNTIL     = 'UNTIL';
REPEAT    = 'REPEAT';
RB_FOR    = 'RB_FOR';
CALL      = 'CALL';
RB_RETURN = 'RB_RETURN';
RB_RETURN_VAL = 'RB_RETURN_VAL';
RB_FAIL   = 'RB_FAIL';
RB_STOP   = 'RB_STOP';
RB_EXIT   = 'RB_EXIT';
RB_NEXT   = 'RB_NEXT';
RB_INITIAL = 'RB_INITIAL';
REPLACE   = 'REPLACE';
REPLN     = 'REPLN';
RB_CASE   = 'RB_CASE';
EXCHG       = 'EXCHG';
ADDASSIGN   = 'ADDASSIGN';
SUBASSIGN   = 'SUBASSIGN';
CATASSIGN   = 'CATASSIGN';
COMPOUND    = 'COMPOUND';
nTop_count   = 'nTop()';
X_sub = nInc() *expr FENCE($',' *X_sub | epsilon);
X_args   = nInc() *alt_expr FENCE($',' FENCE(*X_args | nInc() shift(epsilon, 'TT_NUL') FENCE($',' *X_args | epsilon)) | epsilon);
call_or_id = FENCE(  nPush() shift(*Id, 'TT_VAR') nInc() $'(' FENCE(*X_args | epsilon) $')' reduce('TT_FNC', nTop_count) nPop()
                   | shift(*Id, 'TT_VAR')
                  );
primary = FENCE(  '"' shift(*DQ_body, 'TT_QLIT') '"'
                | "'" shift(*SQ_body, 'TT_QLIT') "'"
                | KW_open shift(*KW_body, 'TT_KEYWORD')
                | '@' shift(*Id, 'TT_CAPT_CURSOR')
                | shift(*Real, 'TT_FLIT')
                | shift(*Integer, 'TT_ILIT')
                | *call_or_id
                | '(' *expr ')'
               );
postfix_expr = *primary
               FENCE(  $'[' *alt_expr $'+:' *alt_expr $']' reduce('TT_IDX', 2) reduce('TT_IDX', 2)
                         FENCE($'[' *alt_expr $'+:' *alt_expr $']' reduce('TT_IDX', 2) reduce('TT_IDX', 2) | epsilon)
                      | $'[' nPush() nInc() *X_sub $']' reduce('TT_IDX', nTop_count) nPop()
                         FENCE($'[' nPush() nInc() *X_sub $']' reduce('TT_IDX', nTop_count) nPop() | epsilon)
                      | *dot_capt    *primary reduce('TT_CAPT_COND_ASGN', 2)
                         FENCE(*dot_capt    *primary reduce('TT_CAPT_COND_ASGN', 2) | epsilon)
                      | *dollar_capt *primary reduce('TT_CAPT_IMMED_ASGN',  2)
                         FENCE(*dollar_capt *primary reduce('TT_CAPT_IMMED_ASGN',  2) | epsilon)
                      | epsilon
                     );
unary_expr = FENCE(  $'-'  *unary_expr reduce('TT_MNS', 1)
                   | '+'   *unary_expr reduce('TT_POS', 1)
                   | '~'   *unary_expr reduce('TT_NOTPAT', 1)
                   | '!'   *unary_expr reduce('TT_BANGPAT', 1)
                   | '/'   *unary_expr reduce('TT_VALUEPAT', 1)
                   | '\'   *unary_expr reduce('TT_NOTPAT', 1)
                   | '$'   *unary_expr reduce('TT_INDIRECT', 1)
                   | '.'   *unary_expr reduce('TT_CAPT_COND_ASGN', 1)
                   | *postfix_expr
                  );
pow_expr = *unary_expr FENCE(  $'**' *pow_expr reduce('TT_POW', 2)
                              | $'^'  *pow_expr reduce('TT_POW', 2)
                              | epsilon
                             );
mul_expr = *pow_expr
           ( $'*' *pow_expr reduce('TT_MUL', 2) ($'*' *pow_expr reduce('TT_MUL', 2) | epsilon)
           | $'/' *pow_expr reduce('TT_DIV', 2) ($'/' *pow_expr reduce('TT_DIV', 2) | epsilon)
           | $'%' *pow_expr reduce(REMDR,  2) ($'%' *pow_expr reduce(REMDR,  2) | epsilon)
           | epsilon
           );
add_expr = *mul_expr
           ( $'+' *mul_expr reduce('TT_ADD', 2) ($'+' *mul_expr reduce('TT_ADD', 2) | epsilon)
           | $'-' *mul_expr reduce('TT_SUB', 2) ($'-' *mul_expr reduce('TT_SUB', 2) | epsilon)
           | epsilon
           );
cmp_expr = *add_expr FENCE(  $'~==' *add_expr reduce(CMP_SNE, 2)
                             | $'==' *add_expr reduce(CMP_SEQ, 2)
                             | $'<<=' *add_expr reduce(CMP_SLE, 2)
                             | $'>>=' *add_expr reduce(CMP_SGE, 2)
                             | $'<<'  *add_expr reduce(CMP_SLT, 2)
                             | $'>>'  *add_expr reduce(CMP_SGT, 2)
                             | $'<='  *add_expr reduce(CMP_LE,  2)
                             | $'>='  *add_expr reduce(CMP_GE,  2)
                             | $'~='  *add_expr reduce(CMP_NE,  2)
                             | $'='   *add_expr reduce(CMP_EQ,  2)
                             | $'<'   *add_expr reduce(CMP_LT,  2)
                             | $'>'   *add_expr reduce(CMP_GT,  2)
                             | epsilon
                            );
cat_expr = *cmp_expr
           ( $'||' *cmp_expr reduce('TT_CAT', 2) ($'||' *cmp_expr reduce('TT_CAT', 2) | epsilon)
           | $'&'  *cmp_expr reduce('TT_CAT', 2) ($'&'  *cmp_expr reduce('TT_CAT', 2) | epsilon)
           | epsilon
           );
X_alt = nInc() *cat_expr FENCE($'|' *X_alt | epsilon);
alt_expr = nPush() *X_alt reduce(ALT, nTop_count) nPop();
expr = *alt_expr FENCE(  $'||:=' *alt_expr reduce(CATASSIGN, 2)
                       | $'+:='  *alt_expr reduce(ADDASSIGN, 2)
                       | $'-:='  *alt_expr reduce(SUBASSIGN, 2)
                       | $':=:'  *alt_expr reduce(EXCHG, 2)
                       | $':='   *alt_expr reduce(ASSIGN, 2)
                       | epsilon
                      );
$'?-match'  = $' '  '?-'  $' ';
match_or_expr = *expr FENCE($'?-match' *alt_expr reduce(REPLN, 2)
                           | $'?' *alt_expr $'<-arrow' *alt_expr reduce(REPLACE, 3)
                           | $'?' *alt_expr reduce(MATCH, 2)
                           | epsilon);
opt_nl = (nl | epsilon);
stmt_body = *opt_nl FENCE(*compound_stmt | *case_stmt | *if_stmt | *while_stmt | *unless_stmt | *until_stmt | *repeat_stmt | *for_stmt | *return_stmt | *stop_stmt | *fail_stmt | *exit_stmt | *next_stmt | *match_or_expr);
if_stmt    = $'if'     *match_or_expr $'then' FENCE(*opt_nl *stmt_body $'else' *opt_nl *stmt_body reduce(IFELSE, 3) | *opt_nl *stmt_body reduce(IF, 2));
while_stmt = $'while'  *match_or_expr $'do'   *opt_nl *stmt_body reduce(WHILE,  2);
unless_stmt = $'unless' *match_or_expr $'then' *opt_nl *stmt_body reduce(UNLESS, 2);
until_stmt  = $'until'  *match_or_expr $'do'   *opt_nl *stmt_body reduce(UNTIL,  2);
repeat_stmt = $'repeat' *opt_nl *stmt_body reduce(REPEAT, 1);
for_body = $'do' BREAK(nl);
for_stmt = $'for' shift(*Id, 'TT_VAR') $'from' *match_or_expr $'to' *match_or_expr
           FENCE($'by' *match_or_expr reduce(RB_FOR, 4) | reduce(RB_FOR, 3))
           *for_body;
return_stmt = $'return' FENCE(*match_or_expr reduce(RB_RETURN_VAL, 1) | reduce(RB_RETURN, 0));
exit_stmt   = $'exit'   reduce(RB_EXIT, 0);
fail_stmt   = $'fail'   reduce(RB_FAIL, 0);
stop_stmt   = $'stop'   reduce(RB_STOP, 0);
next_stmt   = $'next'   reduce(RB_NEXT, 0);
compound_end       = $' ' '}';
compound_item      = nInc() *stmt_inline $';' $' ' nl;
compound_body_tail = FENCE(*compound_end | *compound_item *compound_body_tail);
compound_stmt = $' ' '{' $' ' nl nPush() *compound_body_tail reduce(COMPOUND, nTop_count) nPop();
CASE_CLAUSE   = 'CASE_CLAUSE';
CASE_DEFAULT  = 'CASE_DEFAULT';
stmt_inline = $' ' FENCE(*compound_stmt | *case_stmt | *if_stmt | *while_stmt | *unless_stmt | *until_stmt | *repeat_stmt | *for_stmt | *return_stmt | *stop_stmt | *fail_stmt | *exit_stmt | *next_stmt | *match_or_expr) $' ';
caseclause_guard   = nInc() *match_or_expr $':' *stmt_inline reduce(CASE_CLAUSE, 2);
rb_default_kw  = $' '  'default'   $' ';
caseclause_default = nInc() *rb_default_kw $':' *stmt_inline reduce(CASE_DEFAULT, 1);
caseclause         = FENCE(*caseclause_default | *caseclause_guard);
caselist_tail = FENCE($';' FENCE(*caseclause *caselist_tail | epsilon) | epsilon);
caselist      = *caseclause *caselist_tail;
case_stmt = *rb_case_kw nPush() nInc() *match_or_expr $'of' $'{' *caselist $'}' reduce(RB_CASE, nTop_count) nPop();
stmt = $' ' FENCE(*compound_stmt | *case_stmt | *if_stmt | *while_stmt | *unless_stmt | *until_stmt | *repeat_stmt | *for_stmt | *return_stmt | *stop_stmt | *fail_stmt | *exit_stmt | *next_stmt | *match_or_expr) $' ' FENCE(nl | epsilon);
func_end      = $'end' $' ' nl;
blank_line    = $' ' nl;
func_body_stmt = FENCE(*blank_line *func_body_stmt | *func_end | nInc() *stmt *func_body_stmt);
func_body     = nPush() *func_body_stmt reduce(BODY, nTop_count) nPop();
X_params  = nInc() shift(*Id, 'TT_VAR') FENCE($',' *X_params | epsilon);
opt_params = nPush() FENCE(*X_params | epsilon) reduce(PARAMS, nTop_count) nPop();
X_fields  = nInc() shift(*Id, 'TT_VAR') FENCE($',' *X_fields | epsilon);
opt_fields = nPush() FENCE(*X_fields | epsilon) reduce(FIELDS, nTop_count) nPop();
X_locals   = nInc() shift(*Id, 'TT_VAR') FENCE($',' *X_locals | epsilon);
opt_locals = nPush() FENCE($'local' *X_locals FENCE($';' | epsilon) $' ' nl | epsilon) reduce(LOCALS, nTop_count) nPop();
init_expr   = $' ' *match_or_expr $' ';
opt_initial = FENCE(nPush() $'initial' *init_expr $';' $' ' nl reduce(RB_INITIAL, 1) nPop() | reduce(RB_INITIAL, 0));
function_decl =
    $'function' shift(*Id, 'TT_VAR') $'(' *opt_params $')' $' ' nl
    *opt_locals
    *opt_initial
    *func_body
    reduce(FUNC_DECL, 5);
record_decl =
    $'record' shift(*Id, 'TT_VAR') $'(' *opt_fields $')' $' ' nl
    reduce(REC_DECL, 2);
func_cmd = nInc() *function_decl;
rec_cmd  = nInc() *record_decl;
blank    = $' ' nl;
Command  = *func_cmd | *rec_cmd | *blank;
Compiland = nPush() POS(0) ARBNO(Command) RPOS(0) reduce(Parse, nTop_count) nPop();
InitCounter();
InitStack();
Src = '';
while (Line = INPUT) Src = Src Line nl;
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
