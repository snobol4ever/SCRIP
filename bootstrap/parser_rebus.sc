/* PST-RB-SC ✅ 2026-05-19 — already shift/reduce-pure; verified zero violations. */
&FULLSCAN = 1;
white       =   (  SPAN(' ' CHAR(9))
                |  '#'  BREAK(CHAR(10))
                |  '//' BREAK(CHAR(10))
                |  '/*' BREAK('*') '*' ARBNO('*' | NOTANY('/*') BREAK('*') '*') '/'
                );
White       =   *white ARBNO(*white);
Gray        =   *White | epsilon;
$'  '       =   White;
$' '        =   Gray;
Id      = ANY(&UCASE &LCASE '_') FENCE(SPAN(&UCASE &LCASE '0123456789' '_' '.') | epsilon);
Integer = SPAN('0123456789');
Real    = SPAN('0123456789') '.' SPAN('0123456789');
KW_open = '&';
KW_body = ANY(&UCASE &LCASE '_') (SPAN(&UCASE &LCASE '0123456789' '_') | epsilon);
DQ_body = BREAK('"');
SQ_body = BREAK("'");
$'('        =       '('        *$' ';  $')'        = *$' ' ')';
$'['        =       '['        *$' ';  $']'        = *$' ' ']';
$'.'        = *$' '  '.'        *$' ';
$','        = *$' '  ','        *$' ';
$':='       = *$' '  ':='       *$' ';
$'?'        = *$' '  '?'        *$' ';
$'|'        = *$' '  '|'        *$' ';
$'+'        = *$' '  '+'        *$' ';  $'-'        = *$' ' '-'  *$' ';
$'*'        = *$' '  '*'        *$' ';  $'/'        = *$' ' '/'  *$' ';
$'^'        = *$' '  '^'        *$' ';  $'**'       = *$' ' '**' *$' ';
$'%'        = *$' '  '%'        *$' ';
$'~=='      = *$' '  '~=='      *$' ';  $'=='       = *$' ' '=='  *$' ';
$'<<='      = *$' '  '<<='      *$' ';  $'>>='      = *$' ' '>>=' *$' ';
$'<<'       = *$' '  '<<'       *$' ';  $'>>'       = *$' ' '>>'  *$' ';
$'<='       = *$' '  '<='       *$' ';  $'>='       = *$' ' '>='  *$' ';
$'<'        = *$' '  '<'        *$'  ';  $'>'       = *$' ' '>'   *$'  ';
$'<-arrow'  = *$' '  '<-'       *$' ';
$'~='       = *$' '  '~='       *$' ';  $'='        = *$' ' '='   *$' ';
$'||'       = *$' '  '||'       *$' ';  $'&'        = *$' ' '&'   *$' ';
$'function' = *$' ' *Id $ tx *IDENT(tx, 'function') *$'  '; $'end'      = *$' ' *Id $ tx *IDENT(tx, 'end');
$'record'   = *$' ' *Id $ tx *IDENT(tx, 'record') *$'  ';
$'if'       = *$' ' *Id $ tx *IDENT(tx, 'if') *$'  '; $'then'     = *$' ' *Id $ tx *IDENT(tx, 'then') *$' ';
$'else'     = *$' ' *Id $ tx *IDENT(tx, 'else') *$' ';
$'unless'   = *$' ' *Id $ tx *IDENT(tx, 'unless') *$'  ';
$'for'      = *$' ' *Id $ tx *IDENT(tx, 'for') *$'  ';
$'from'     = *$' ' *Id $ tx *IDENT(tx, 'from') *$'  ';
$'to'       = *$' ' *Id $ tx *IDENT(tx, 'to') *$'  ';
$'by'       = *$' ' *Id $ tx *IDENT(tx, 'by') *$'  ';
$'while'    = *$' ' *Id $ tx *IDENT(tx, 'while') *$'  '; $'do'       = *$' ' *Id $ tx *IDENT(tx, 'do') *$' ';
$'until'    = *$' ' *Id $ tx *IDENT(tx, 'until') *$'  ';
$'repeat'   = *$' ' *Id $ tx *IDENT(tx, 'repeat') *$'  ';
$'return'   = *$' ' *Id $ tx *IDENT(tx, 'return') *$' ';
$'exit'     = *$' ' *Id $ tx *IDENT(tx, 'exit') *$' ';
$'fail'     = *$' ' *Id $ tx *IDENT(tx, 'fail') *$' ';
$'stop'     = *$' ' *Id $ tx *IDENT(tx, 'stop') *$' ';
$'next'     = *$' ' *Id $ tx *IDENT(tx, 'next') *$' ';
$'local'    = *$' ' *Id $ tx *IDENT(tx, 'local') *$'  ';
$'initial'  = *$' ' *Id $ tx *IDENT(tx, 'initial') *$'  ';
rb_case_kw  = *$' ' *Id $ tx *IDENT(tx, 'case') *$'  ';
$'of'       = *$' ' *Id $ tx *IDENT(tx, 'of') *$' ';
$'<-'       = *$' '  '<-'       *$' ';
$'?-'       = *$' '  '?-'       *$' ';
$';'        = *$' '  ';'        *$' ';
$'{'        = *$' '  '{'        *$' ';
$'}'        = *$' '  '}'        *$' ';
$':'        = *$' '  ':'        *$' ';
$'||:='     = *$' '  '||:='     *$' ';
$'+:='      = *$' '  '+:='      *$' ';
$'-:='      = *$' '  '-:='      *$' ';
$':=:'      = *$' '  ':=:'      *$' ';
$'+:'       = *$' '  '+:'       *$' ';
dot_capt    = *$'  '  '.'        *$' ';
dollar_capt = *$'  '  '$'        *$' ';
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
X_sub = epsilon . *IncCounter() *expr FENCE(*$',' *X_sub | epsilon);
X_args   = epsilon . *IncCounter() *alt_expr FENCE(*$',' FENCE(*X_args | epsilon . *IncCounter() (epsilon) . thx . *Shift('TT_NUL', thx) FENCE(*$',' *X_args | epsilon)) | epsilon);
call_or_id = FENCE(  epsilon . *PushCounter() (*Id) . thx . *Shift('TT_VAR', thx) . *IncCounter() *$'(' FENCE(*X_args | epsilon) *$')' . *Reduce('TT_FNC', nTop()) . *PopCounter()
                   | (*Id) . thx . *Shift('TT_VAR', thx)
                  );
primary = FENCE(  '"' (*DQ_body) . thx . *Shift('TT_QLIT', thx) '"'
                | "'" (*SQ_body) . thx . *Shift('TT_QLIT', thx) "'"
                | *KW_open (*KW_body) . thx . *Shift('TT_KEYWORD', thx)
                | '@' (*Id) . thx . *Shift('TT_CAPT_CURSOR', thx)
                | (*Real) . thx . *Shift('TT_FLIT', thx)
                | (*Integer) . thx . *Shift('TT_ILIT', thx)
                | *call_or_id
                | '(' *expr ')'
               );
postfix_expr = *primary
               FENCE(  *$'[' *alt_expr *$'+:' *alt_expr *$']' . *Reduce('TT_IDX', 2) . *Reduce('TT_IDX', 2)
                         FENCE(*$'[' *alt_expr *$'+:' *alt_expr *$']' . *Reduce('TT_IDX', 2) . *Reduce('TT_IDX', 2) | epsilon)
                      | *$'[' . *PushCounter() . *IncCounter() *X_sub *$']' . *Reduce('TT_IDX', nTop()) . *PopCounter()
                         FENCE(*$'[' . *PushCounter() . *IncCounter() *X_sub *$']' . *Reduce('TT_IDX', nTop()) . *PopCounter() | epsilon)
                      | *dot_capt    *primary . *Reduce('TT_CAPT_COND_ASGN', 2)
                         FENCE(*dot_capt    *primary . *Reduce('TT_CAPT_COND_ASGN', 2) | epsilon)
                      | *dollar_capt *primary . *Reduce('TT_CAPT_IMMED_ASGN', 2)
                         FENCE(*dollar_capt *primary . *Reduce('TT_CAPT_IMMED_ASGN', 2) | epsilon)
                      | epsilon
                     );
unary_expr = FENCE(  *$'-'  *unary_expr . *Reduce('TT_MNS', 1)
                   | '+'   *unary_expr . *Reduce('TT_POS', 1)
                   | '~'   *unary_expr . *Reduce('TT_NOTPAT', 1)
                   | '!'   *unary_expr . *Reduce('TT_BANGPAT', 1)
                   | '/'   *unary_expr . *Reduce('TT_VALUEPAT', 1)
                   | '\'   *unary_expr . *Reduce('TT_NOTPAT', 1)
                   | '$'   *unary_expr . *Reduce('TT_INDIRECT', 1)
                   | '.'   *unary_expr . *Reduce('TT_CAPT_COND_ASGN', 1)
                   | *postfix_expr
                  );
pow_expr = *unary_expr FENCE(  *$'**' *pow_expr . *Reduce('TT_POW', 2)
                              | *$'^'  *pow_expr . *Reduce('TT_POW', 2)
                              | epsilon
                             );
mul_expr = *pow_expr *mul_tail;
mul_tail = ( *$'*' *pow_expr . *Reduce('TT_MUL', 2) *mul_tail
           | *$'/' *pow_expr . *Reduce('TT_DIV', 2) *mul_tail
           | *$'%' *pow_expr . *Reduce('REMDR', 2) *mul_tail
           | epsilon
           );
add_expr = *mul_expr *add_tail;
add_tail = ( *$'+' *mul_expr . *Reduce('TT_ADD', 2) *add_tail
           | *$'-' *mul_expr . *Reduce('TT_SUB', 2) *add_tail
           | epsilon
           );
cmp_expr = *add_expr FENCE(  *$'~==' *add_expr . *Reduce('CMP_SNE', 2)
                             | *$'==' *add_expr . *Reduce('CMP_SEQ', 2)
                             | *$'<<=' *add_expr . *Reduce('CMP_SLE', 2)
                             | *$'>>=' *add_expr . *Reduce('CMP_SGE', 2)
                             | *$'<<'  *add_expr . *Reduce('CMP_SLT', 2)
                             | *$'>>'  *add_expr . *Reduce('CMP_SGT', 2)
                             | *$'<='  *add_expr . *Reduce('CMP_LE', 2)
                             | *$'>='  *add_expr . *Reduce('CMP_GE', 2)
                             | *$'~='  *add_expr . *Reduce('CMP_NE', 2)
                             | *$'='   *add_expr . *Reduce('CMP_EQ', 2)
                             | *$'<'   *add_expr . *Reduce('CMP_LT', 2)
                             | *$'>'   *add_expr . *Reduce('CMP_GT', 2)
                             | epsilon
                            );
cat_expr = *cmp_expr *cat_tail;
cat_tail = ( *$'||' *cmp_expr . *Reduce('TT_CAT', 2) *cat_tail
           | *$'&'  *cmp_expr . *Reduce('TT_CAT', 2) *cat_tail
           | epsilon
           );
X_alt = epsilon . *IncCounter() *cat_expr FENCE(*$'|' *X_alt | epsilon);
alt_expr = epsilon . *PushCounter() *X_alt . *Reduce('ALT', nTop()) . *PopCounter();
expr = *alt_expr FENCE(  *$'||:=' *alt_expr . *Reduce('CATASSIGN', 2)
                       | *$'+:='  *alt_expr . *Reduce('ADDASSIGN', 2)
                       | *$'-:='  *alt_expr . *Reduce('SUBASSIGN', 2)
                       | *$':=:'  *alt_expr . *Reduce('EXCHG', 2)
                       | *$':='   *alt_expr . *Reduce('ASSIGN', 2)
                       | epsilon
                      );
$'?-match'  = *$' '  '?-'  *$' ';
match_or_expr = *expr FENCE(*$'?-match' *alt_expr . *Reduce('REPLN', 2)
                           | *$'?' *alt_expr *$'<-arrow' *alt_expr . *Reduce('REPLACE', 3)
                           | *$'?' *alt_expr . *Reduce('MATCH', 2)
                           | epsilon);
opt_nl = (CHAR(10) | epsilon);
StmtT     = TABLE(12);
kw_stmt   = *$' ' *Id $ tx *DIFFER(StmtT[tx]) *StmtT[tx];
stmt_body = *opt_nl *$' ' FENCE(*compound_stmt | *kw_stmt | *match_or_expr);
if_stmt    = *$'if' *if_stmt_rest;
if_stmt_rest = *$'  ' *match_or_expr *$'then' FENCE(*opt_nl *stmt_body *opt_nl *$'else' *opt_nl *stmt_body . *Reduce('IFELSE', 3) | *opt_nl *stmt_body . *Reduce('IF', 2));
while_stmt = *$'while' *while_stmt_rest;
while_stmt_rest = *$'  ' *match_or_expr *$'do'   *opt_nl *stmt_body . *Reduce('WHILE', 2);
unless_stmt = *$'unless' *unless_stmt_rest;
unless_stmt_rest = *$'  ' *match_or_expr *$'then' *opt_nl *stmt_body . *Reduce('UNLESS', 2);
until_stmt  = *$'until' *until_stmt_rest;
until_stmt_rest = *$'  ' *match_or_expr *$'do'   *opt_nl *stmt_body . *Reduce('UNTIL', 2);
repeat_stmt = *$'repeat' *repeat_stmt_rest;
repeat_stmt_rest = *$'  ' *opt_nl *stmt_body . *Reduce('REPEAT', 1);
for_body = *$'do' *opt_nl *stmt_body;
for_stmt = *$'for' *for_stmt_rest;
for_stmt_rest = *$'  ' (*Id) . thx . *Shift('TT_VAR', thx) *$'from' *match_or_expr *$'to' *match_or_expr
           FENCE(*$'by' *match_or_expr *for_body . *Reduce('RB_FOR', 5) | *for_body . *Reduce('RB_FOR', 4));
return_stmt = *$'return' *return_stmt_rest;
return_stmt_rest = *$' ' FENCE(*match_or_expr . *Reduce('RB_RETURN_VAL', 1) | epsilon . *Reduce('RB_RETURN', 0));
exit_stmt   = *$'exit' *exit_stmt_rest;
exit_stmt_rest = *$' ' . *Reduce('RB_EXIT', 0);
fail_stmt   = *$'fail' *fail_stmt_rest;
fail_stmt_rest = *$' ' . *Reduce('RB_FAIL', 0);
stop_stmt   = *$'stop' *stop_stmt_rest;
stop_stmt_rest = *$' ' . *Reduce('RB_STOP', 0);
next_stmt   = *$'next' *next_stmt_rest;
next_stmt_rest = *$' ' . *Reduce('RB_NEXT', 0);
compound_end       = *$' ' '}';
compound_item      = epsilon . *IncCounter() *stmt_inline FENCE(*$';' | epsilon) *$' ' CHAR(10);
compound_body_tail = FENCE(*compound_end | *blank_line *compound_body_tail | *compound_item *compound_body_tail);
compound_stmt = *$' ' '{' *$' ' CHAR(10) . *PushCounter() *compound_body_tail . *Reduce('COMPOUND', nTop()) . *PopCounter();
CASE_CLAUSE   = 'CASE_CLAUSE';
CASE_DEFAULT  = 'CASE_DEFAULT';
stmt_inline = *$' ' FENCE(*compound_stmt | *kw_stmt | *match_or_expr) *$' ';
caseclause_guard   = epsilon . *IncCounter() *match_or_expr *$':' *stmt_inline . *Reduce('CASE_CLAUSE', 2);
rb_default_kw  = *$' '  'default'   *$' ';
caseclause_default = epsilon . *IncCounter() *rb_default_kw *$':' *stmt_inline . *Reduce('CASE_DEFAULT', 1);
caseclause         = FENCE(*caseclause_default | *caseclause_guard);
caselist_tail = FENCE(FENCE(*$';' | epsilon) *$' ' CHAR(10) *$' ' FENCE(*caseclause *caselist_tail | *caselist_tail) | *$';' FENCE(*caseclause *caselist_tail | epsilon) | epsilon);
caselist      = *caseclause *caselist_tail;
case_stmt = *rb_case_kw *case_stmt_rest;
case_stmt_rest = *$'  ' . *PushCounter() . *IncCounter() *match_or_expr *$'of' *$'{' *opt_nl *$' ' *caselist *$'}' . *Reduce('RB_CASE', nTop()) . *PopCounter();
stmt = *$' ' FENCE(*compound_stmt | *kw_stmt | *match_or_expr) *$' ' FENCE(*$';' FENCE(CHAR(10) | epsilon) | CHAR(10));
func_end      = *$'end' *$' ' CHAR(10);
blank_line    = *$' ' CHAR(10);
func_body_stmt = FENCE(*blank_line *func_body_stmt | *func_end | epsilon . *IncCounter() *stmt *func_body_stmt);
func_body     = epsilon . *PushCounter() *func_body_stmt . *Reduce('BODY', nTop()) . *PopCounter();
X_params  = epsilon . *IncCounter() (*Id) . thx . *Shift('TT_VAR', thx) FENCE(*$',' *X_params | epsilon);
opt_params = epsilon . *PushCounter() FENCE(*X_params | epsilon) . *Reduce('PARAMS', nTop()) . *PopCounter();
X_fields  = epsilon . *IncCounter() (*Id) . thx . *Shift('TT_VAR', thx) FENCE(*$',' *X_fields | epsilon);
opt_fields = epsilon . *PushCounter() FENCE(*X_fields | epsilon) . *Reduce('FIELDS', nTop()) . *PopCounter();
X_locals   = epsilon . *IncCounter() (*Id) . thx . *Shift('TT_VAR', thx) FENCE(*$',' *X_locals | epsilon);
opt_locals = epsilon . *PushCounter() FENCE(*$'local' *X_locals FENCE(*$';' | epsilon) *$' ' CHAR(10) | epsilon) . *Reduce('LOCALS', nTop()) . *PopCounter();
init_expr   = *stmt_inline;
opt_initial = FENCE(epsilon . *PushCounter() *$'initial' *init_expr FENCE(*$';' | epsilon) *$' ' CHAR(10) . *Reduce('RB_INITIAL', 1) . *PopCounter() | epsilon . *Reduce('RB_INITIAL', 0));
function_decl =
    *$'function' (*Id) . thx . *Shift('TT_VAR', thx) *$'(' *opt_params *$')' *$' ' CHAR(10)
    *opt_locals
    *opt_initial
    *func_body
    . *Reduce('FUNC_DECL', 5);
record_decl =
    *$'record' (*Id) . thx . *Shift('TT_VAR', thx) *$'(' *opt_fields *$')' *$' ' CHAR(10)
    . *Reduce('REC_DECL', 2);
func_cmd = epsilon . *IncCounter() *function_decl;
rec_cmd  = epsilon . *IncCounter() *record_decl;
blank    = *$' ' CHAR(10);
Command  = *func_cmd | *rec_cmd | *blank;
Compiland = epsilon . *PushCounter() POS(0) ARBNO(*Command) RPOS(0) . *Reduce('Parse', nTop()) . *PopCounter();
StmtT['case'] = case_stmt_rest; StmtT['if'] = if_stmt_rest; StmtT['while'] = while_stmt_rest; StmtT['unless'] = unless_stmt_rest;
StmtT['until'] = until_stmt_rest; StmtT['repeat'] = repeat_stmt_rest; StmtT['for'] = for_stmt_rest; StmtT['return'] = return_stmt_rest;
StmtT['stop'] = stop_stmt_rest; StmtT['fail'] = fail_stmt_rest; StmtT['exit'] = exit_stmt_rest; StmtT['next'] = next_stmt_rest;
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
