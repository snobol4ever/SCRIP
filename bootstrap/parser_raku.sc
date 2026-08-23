/* PRF-14 ✅ — pure shift/reduce mirror of raku.y; tree shapes byte-equal to C frontend. */
&ANCHOR   = 0;
&FULLSCAN = 1;
bSlash = '\';
function strip_sigil(s) {
    if (s ? POS(0) ANY('$@%')) { strip_sigil = SUBSTR(s, 2); return; }
    strip_sigil = s; return;
}
white       =   (SPAN(' ' tab nl) | '#' BREAK(nl) nl);
White       =   white ARBNO(white);
Gray        =   ARBNO(white);
$'  '       =   White;
$' '        =   Gray;
Id          =   ANY(&UCASE &LCASE '_') FENCE(SPAN(digits &UCASE &LCASE '_') | epsilon);
$'CATCH'    =   $' ' Id $ tx (*IDENT(tx, 'CATCH') | *IDENT(tx, 'catch'));
$'class'    =   $' ' Id $ tx *IDENT(tx, 'class');
$'default'  =   $' ' Id $ tx *IDENT(tx, 'default');
$'delete'   =   $' ' Id $ tx *IDENT(tx, 'delete');
$'die'      =   $' ' Id $ tx *IDENT(tx, 'die');
$'div'      =   $' ' Id $ tx *IDENT(tx, 'div');
$'else'     =   $' ' Id $ tx *IDENT(tx, 'else');
$'elsif'    =   $' ' Id $ tx *IDENT(tx, 'elsif');
$'eq'       =   $' ' Id $ tx *IDENT(tx, 'eq');
$'exists'   =   $' ' Id $ tx *IDENT(tx, 'exists');
$'False'    =   $' ' Id $ tx *IDENT(tx, 'False');
$'for'      =   $' ' Id $ tx *IDENT(tx, 'for');
$'gather'   =   $' ' Id $ tx *IDENT(tx, 'gather');
$'given'    =   $' ' Id $ tx *IDENT(tx, 'given');
$'grep'     =   $' ' Id $ tx *IDENT(tx, 'grep');
$'has'      =   $' ' Id $ tx *IDENT(tx, 'has');
$'if'       =   $' ' Id $ tx *IDENT(tx, 'if');
$'map'      =   $' ' Id $ tx *IDENT(tx, 'map');
$'method'   =   $' ' Id $ tx *IDENT(tx, 'method');
$'my'       =   $' ' Id $ tx *IDENT(tx, 'my');
$'ne'       =   $' ' Id $ tx *IDENT(tx, 'ne');
$'new'      =   $' ' Id $ tx *IDENT(tx, 'new');
$'print'    =   $' ' Id $ tx *IDENT(tx, 'print');
$'repeat'   =   $' ' Id $ tx *IDENT(tx, 'repeat');
$'return'   =   $' ' Id $ tx *IDENT(tx, 'return');
$'say'      =   $' ' Id $ tx *IDENT(tx, 'say');
$'self'     =   $' ' Id $ tx *IDENT(tx, 'self');
$'sort'     =   $' ' Id $ tx *IDENT(tx, 'sort');
$'sub'      =   $' ' Id $ tx *IDENT(tx, 'sub');
$'take'     =   $' ' Id $ tx *IDENT(tx, 'take');
$'True'     =   $' ' Id $ tx *IDENT(tx, 'True');
$'try'      =   $' ' Id $ tx *IDENT(tx, 'try');
$'unless'   =   $' ' Id $ tx *IDENT(tx, 'unless');
$'until'    =   $' ' Id $ tx *IDENT(tx, 'until');
$'when'     =   $' ' Id $ tx *IDENT(tx, 'when');
$'while'    =   $' ' Id $ tx *IDENT(tx, 'while');
reserved    =   POS(0) ('CATCH' | 'catch' | 'class' | 'default' | 'delete' | 'die' | 'div'
                       | 'else' | 'elsif' | 'eq' | 'exists' | 'False' | 'for' | 'gather'
                       | 'given' | 'grep' | 'has' | 'if' | 'map' | 'method' | 'my' | 'ne'
                       | 'new' | 'print' | 'repeat' | 'return' | 'say' | 'self' | 'sort'
                       | 'sub' | 'take' | 'True' | 'try' | 'unless' | 'until' | 'when' | 'while')
                RPOS(0);
$'=>'  = $' ' '=>'  $' ';   $'..^' = $' ' '..^' $' ';
$'..'  = $' ' '..'  $' ';   $'->'  = $' ' '->'  $' ';
$'=='  = $' ' '=='  $' ';   $'!='  = $' ' '!='  $' ';
$'<='  = $' ' '<='  $' ';   $'>='  = $' ' '>='  $' ';
$'&&'  = $' ' '&&'  $' ';   $'||'  = $' ' '||'  $' ';
$':='  = $' ' ':='  $' ';   $'~~'  = $' ' '~~'  $' ';
$'='   = $' ' '='   $' ';   $'+'   = $' ' '+'   $' ';
$'-'   = $' ' '-'   $' ';   $'*'   = $' ' '*'   $' ';
$'/'   = $' ' '/'   $' ';   $'%'   = $' ' '%'   $' ';
$'~'   = $' ' '~'   $' ';   $'!'   = $' ' '!';
$'<'   = $' ' '<';          $'>'   = $' ' '>';
$';'   = $' ' ';';          $','   = $' ' ','   $' ';
$'('   = $' ' '('   $' ';   $')'   = $' ' ')';
$'{'   = $' ' '{'   $' ';   $'}'   = $' ' '}';
$'['   = $' ' '['   $' ';   $']'   = $' ' ']';
$'.'   = $' ' '.';
TT_PROGRAM      = 'TT_PROGRAM';
TT_STMT         = 'TT_STMT';
TT_VAR          = 'TT_VAR';
TT_QLIT         = 'TT_QLIT';
TT_ILIT         = 'TT_ILIT';
TT_FLIT         = 'TT_FLIT';
TT_NUL          = 'TT_NUL';
TT_SEQ_EXPR     = 'TT_SEQ_EXPR';
TT_DECL         = 'TT_DECL';
TT_ASSIGN       = 'TT_ASSIGN';
TT_FIELD        = 'TT_FIELD';
TT_ARR_SET      = 'TT_ARR_SET';
TT_ARR_GET      = 'TT_ARR_GET';
TT_HASH_SET     = 'TT_HASH_SET';
TT_HASH_GET     = 'TT_HASH_GET';
TT_HASH_DELETE  = 'TT_HASH_DELETE';
TT_HASH_EXISTS  = 'TT_HASH_EXISTS';
TT_SAY          = 'TT_SAY';
TT_SAY_FH       = 'TT_SAY_FH';
TT_PRINT        = 'TT_PRINT';
TT_PRINT_FH     = 'TT_PRINT_FH';
TT_SUSPEND      = 'TT_SUSPEND';
TT_RETURN       = 'TT_RETURN';
TT_IF           = 'TT_IF';
TT_WHILE        = 'TT_WHILE';
TT_UNLESS       = 'TT_UNLESS';
TT_UNTIL        = 'TT_UNTIL';
TT_REPEAT       = 'TT_REPEAT';
TT_FOR_RANGE    = 'TT_FOR_RANGE';
TT_ITERATE      = 'TT_ITERATE';
TT_EVERY        = 'TT_EVERY';
TT_CASE         = 'TT_CASE';
TT_TRY          = 'TT_TRY';
TT_SUB_DECL     = 'TT_SUB_DECL';
TT_CLASS_DECL   = 'TT_CLASS_DECL';
TT_GATHER       = 'TT_GATHER';
TT_FNC          = 'TT_FNC';
TT_NEW          = 'TT_NEW';
TT_METHCALL     = 'TT_METHCALL';
TT_DIE          = 'TT_DIE';
TT_MAP          = 'TT_MAP';
TT_GREP         = 'TT_GREP';
TT_SORT         = 'TT_SORT';
TT_ADD          = 'TT_ADD';
TT_SUB          = 'TT_SUB';
TT_CAT          = 'TT_CAT';
TT_MUL          = 'TT_MUL';
TT_DIV          = 'TT_DIV';
TT_MOD          = 'TT_MOD';
TT_MNS          = 'TT_MNS';
TT_NOT          = 'TT_NOT';
TT_SEQ          = 'TT_SEQ';
TT_ALT          = 'TT_ALT';
TT_EQ           = 'TT_EQ';
TT_NE           = 'TT_NE';
TT_LT           = 'TT_LT';
TT_GT           = 'TT_GT';
TT_LE           = 'TT_LE';
TT_GE           = 'TT_GE';
TT_LEQ          = 'TT_LEQ';
TT_LNE          = 'TT_LNE';
TT_SMATCH       = 'TT_SMATCH';
TT_TO           = 'TT_TO';
TT_CAPTURE      = 'TT_CAPTURE';
TT_NAMED_CAPTURE = 'TT_NAMED_CAPTURE';
TT_TWIGIL_FIELD = 'TT_TWIGIL_FIELD';
Parse           = 'Parse';
nTop_count      = 'nTop()';
IdentName       =   $' ' (Id $ tx_id) $ tx *notmatch(tx, reserved);
sigil_first     =   ANY(&UCASE &LCASE '_');
sigil_rest      =   SPAN(digits &UCASE &LCASE '_');
ArrayBare       =   $' ' '@'      sigil_first FENCE(sigil_rest | epsilon);
HashBare        =   $' ' '%'      sigil_first FENCE(sigil_rest | epsilon);
TwigilBare      =   $' ' ('$.' | '$!') sigil_first FENCE(sigil_rest | epsilon);
NamedCapBare    =   $' ' '$<' BREAK('>') . tx_ncap '>';
ScalarUnderscore =  $' ' '$_' ~ANY(digits &UCASE &LCASE '_');
StdInBare       =   $' ' '$*STDIN'  ~ANY(digits &UCASE &LCASE '_');
StdOutBare      =   $' ' '$*STDOUT' ~ANY(digits &UCASE &LCASE '_');
StdErrBare      =   $' ' '$*STDERR' ~ANY(digits &UCASE &LCASE '_');
CaptureBare     =   $' ' '$' SPAN(digits) . tx_capn ~ANY(&UCASE &LCASE '_');
VarScalar       =   ($' ' '$' (sigil_first FENCE(sigil_rest | epsilon) | '_')) . tx_var;
VarArray        =   ArrayBare  . tx_var;
VarHash         =   HashBare   . tx_var;
VarTwigil       =   TwigilBare . tx_var assign(.tx_twig, SUBSTR(tx_var, 3));
LitInt          =   $' ' SPAN(digits) . tx_int ~('.' SPAN(digits));
LitFloat        =   $' ' (SPAN(digits) '.' SPAN(digits) FENCE(ANY('eE') FENCE(ANY('+-') | epsilon) SPAN(digits) | epsilon)
                         | SPAN(digits) ANY('eE') FENCE(ANY('+-') | epsilon) SPAN(digits)) . tx_flt;
LitStrSQ        =   $' ' "'" BREAK("'") . tx_str "'";
LitStrDQ        =   $' ' '"' BREAK('"') . tx_str '"';
LitRegex        =   '/' BREAK('/') . tx_rx '/';
LitMatchGlobal  =   'm:g/' BREAK('/') . tx_rx '/';
LitSubst        =   's/' BREAK('/') . tx_subp '/' BREAK('/') . tx_subr '/'
                    FENCE('g' assign(.tx_subg, 'g') | assign(.tx_subg, '-'));
HashAngleKey    =   BREAK('>') . tx_hkey;
push_var_scalar     = epsilon . *assign(.tmp, strip_sigil(tx_var)) shift(tmp, TT_VAR);
push_var_array      = epsilon . *assign(.tmp, strip_sigil(tx_var)) shift(tmp, TT_VAR);
push_var_hash       = epsilon . *assign(.tmp, strip_sigil(tx_var)) shift(tmp, TT_VAR);
push_var_twigil     = epsilon . *assign(.tmp, tx_twig) shift(tmp, TT_VAR);
push_self           = assign(.tmp, 'self') shift(tmp, TT_VAR);
push_ident_as_var   = epsilon . *assign(.tmp, tx_id) shift(tmp, TT_VAR);
push_ident_as_qlit  = epsilon . *assign(.tmp, tx_id) shift(tmp, TT_QLIT);
push_hkey_qlit      = epsilon . *assign(.tmp, tx_hkey) shift(tmp, TT_QLIT);
push_capn_ilit      = epsilon . *assign(.tmp, tx_capn) shift(tmp, TT_ILIT);
push_stdin_ilit     = assign(.tmp, '0') shift(tmp, TT_ILIT);
push_stdout_ilit    = assign(.tmp, '1') shift(tmp, TT_ILIT);
push_stderr_ilit    = assign(.tmp, '2') shift(tmp, TT_ILIT);
push_ncap_qlit      = epsilon . *assign(.tmp, tx_ncap) shift(tmp, TT_QLIT);
push_int            = epsilon . *assign(.tmp, tx_int) shift(tmp, TT_ILIT);
push_true           = assign(.tmp, '1') shift(tmp, TT_ILIT);
push_false          = assign(.tmp, '0') shift(tmp, TT_ILIT);
push_flt            = epsilon . *assign(.tmp, tx_flt) shift(tmp, TT_FLIT);
push_str            = epsilon . *assign(.tmp, tx_str) shift(tmp, TT_QLIT);
push_regex          = epsilon . *assign(.tmp, tx_rx) shift(tmp, TT_QLIT);
push_subst          = epsilon . *assign(.tmp, tx_subp CHAR(1) tx_subr CHAR(1) tx_subg) shift(tmp, TT_QLIT);
push_kind_match     = assign(.tmp, 'match') shift(tmp, TT_QLIT);
push_kind_match_g   = assign(.tmp, 'match_global') shift(tmp, TT_QLIT);
push_kind_subst     = assign(.tmp, 'subst') shift(tmp, TT_QLIT);
atom            =   FENCE(
                       *LitFloat       *push_flt
                    |  *LitInt          *push_int
                    |  $'True'          *push_true
                    |  $'False'         *push_false
                    |  *LitStrSQ        *push_str
                    |  *LitStrDQ        *push_str
                    |  StdInBare        *push_stdin_ilit  reduce(TT_CAPTURE, 1)
                    |  StdOutBare       *push_stdout_ilit reduce(TT_CAPTURE, 1)
                    |  StdErrBare       *push_stderr_ilit reduce(TT_CAPTURE, 1)
                    |  CaptureBare      *push_capn_ilit   reduce(TT_CAPTURE, 1)
                    |  NamedCapBare     *push_ncap_qlit   reduce(TT_NAMED_CAPTURE, 1)
                    |  $'exists' VarHash *push_var_hash
                                       FENCE($'<' HashAngleKey *push_hkey_qlit $'>' reduce(TT_HASH_EXISTS, 2)
                                            | $'{' *expr $'}' reduce(TT_HASH_EXISTS, 2))
                    |  ArrayBare        *push_var_array
                                       $'[' *expr $']' reduce(TT_ARR_GET, 2)
                    |  HashBare         *push_var_hash
                                       FENCE($'<' HashAngleKey *push_hkey_qlit $'>' reduce(TT_HASH_GET, 2)
                                            | $'{' *expr $'}'                  reduce(TT_HASH_GET, 2))
                    |  ScalarUnderscore *push_var_scalar
                                          assign(.tx_var, '$_')
                                          assign(.tmp, '_') shift(tmp, TT_VAR)
                    |  $'self'          *push_self
                    |  VarScalar        *push_var_scalar
                    |  VarArray         *push_var_array
                    |  VarHash          *push_var_hash
                    |  VarTwigil        *push_var_twigil  reduce(TT_TWIGIL_FIELD, 0)
                    |  IdentName        *push_ident_as_var
                    |  $'(' *expr $')'
                    );
ArgFirst        =   *expr nInc();
ArgRest         =   $',' *expr nInc();
NArgFirst       =   IdentName *push_ident_as_qlit nInc() $'=>' *expr nInc();
NArgRest        =   $',' IdentName *push_ident_as_qlit nInc() $'=>' *expr nInc();
call_expr       =   FENCE(
                       IdentName *push_ident_as_qlit FENCE(
                              $'(' FENCE(  nPush() nInc() *ArgFirst ARBNO(*ArgRest) reduce(TT_FNC, nTop_count) nPop()
                                         | nInc() reduce(TT_FNC, 1)
                                        ) $')'
                            | $'.' $'new' $'(' FENCE(nPush() nInc() *NArgFirst ARBNO(*NArgRest) reduce(TT_NEW, nTop_count) nPop()
                                                    | nInc() reduce(TT_NEW, 1)) $')'
                            )
                    |  $'die'  *expr                              reduce(TT_DIE, 1)
                    |  $'map'  *closure *expr                     reduce(TT_MAP, 2)
                    |  $'grep' *closure *expr                     reduce(TT_GREP, 2)
                    |  $'sort' FENCE(*closure *expr reduce(TT_SORT, 2) | *expr reduce(TT_SORT, 1))
                    |  *atom
                       ARBNO(  $'.' IdentName *push_ident_as_qlit
                                FENCE(  $'(' FENCE(nPush() nInc() nInc() *ArgFirst ARBNO(*ArgRest) reduce(TT_METHCALL, nTop_count) nPop()
                                                  | nInc() reduce(TT_METHCALL, 2)) $')'
                                      | epsilon reduce(TT_FIELD, 1)
                                      )
                            )
                    );
postfix_expr    =   *call_expr;
unary_expr      =   FENCE(
                       $'-' *unary_expr reduce(TT_MNS, 1)
                    |  $'!' *unary_expr reduce(TT_NOT, 1)
                    |  *postfix_expr
                    );
mul_expr        =   *unary_expr
                    FENCE(
                       $'*'   *unary_expr reduce(TT_MUL, 2) ARBNO( $'*'   *unary_expr reduce(TT_MUL, 2)
                                                                 | $'/'   *unary_expr reduce(TT_DIV, 2)
                                                                 | $'%'   *unary_expr reduce(TT_MOD, 2)
                                                                 | $'div' *unary_expr reduce(TT_DIV, 2))
                    |  $'/'   *unary_expr reduce(TT_DIV, 2) ARBNO( $'*'   *unary_expr reduce(TT_MUL, 2)
                                                                 | $'/'   *unary_expr reduce(TT_DIV, 2)
                                                                 | $'%'   *unary_expr reduce(TT_MOD, 2)
                                                                 | $'div' *unary_expr reduce(TT_DIV, 2))
                    |  $'%'   *unary_expr reduce(TT_MOD, 2) ARBNO( $'*'   *unary_expr reduce(TT_MUL, 2)
                                                                 | $'/'   *unary_expr reduce(TT_DIV, 2)
                                                                 | $'%'   *unary_expr reduce(TT_MOD, 2)
                                                                 | $'div' *unary_expr reduce(TT_DIV, 2))
                    |  $'div' *unary_expr reduce(TT_DIV, 2) ARBNO( $'*'   *unary_expr reduce(TT_MUL, 2)
                                                                 | $'/'   *unary_expr reduce(TT_DIV, 2)
                                                                 | $'%'   *unary_expr reduce(TT_MOD, 2)
                                                                 | $'div' *unary_expr reduce(TT_DIV, 2))
                    |  epsilon
                    );
add_expr        =   *mul_expr
                    FENCE(
                       $'+' *mul_expr reduce(TT_ADD, 2) ARBNO( $'+' *mul_expr reduce(TT_ADD, 2)
                                                              | $'-' *mul_expr reduce(TT_SUB, 2)
                                                              | $'~' *mul_expr reduce(TT_CAT, 2))
                    |  $'-' *mul_expr reduce(TT_SUB, 2) ARBNO( $'+' *mul_expr reduce(TT_ADD, 2)
                                                              | $'-' *mul_expr reduce(TT_SUB, 2)
                                                              | $'~' *mul_expr reduce(TT_CAT, 2))
                    |  $'~' *mul_expr reduce(TT_CAT, 2) ARBNO( $'+' *mul_expr reduce(TT_ADD, 2)
                                                              | $'-' *mul_expr reduce(TT_SUB, 2)
                                                              | $'~' *mul_expr reduce(TT_CAT, 2))
                    |  epsilon
                    );
range_expr      =   *add_expr FENCE(  $'..^' *add_expr reduce(TT_TO, 2)
                                    | $'..'  *add_expr reduce(TT_TO, 2)
                                    | epsilon);
cmp_expr        =   *range_expr
                    FENCE(
                       $'&&' *range_expr reduce(TT_SEQ, 2) ARBNO($'&&' *range_expr reduce(TT_SEQ, 2) | $'||' *range_expr reduce(TT_ALT, 2))
                    |  $'||' *range_expr reduce(TT_ALT, 2) ARBNO($'&&' *range_expr reduce(TT_SEQ, 2) | $'||' *range_expr reduce(TT_ALT, 2))
                    |  $'==' *range_expr reduce(TT_EQ,  2)
                    |  $'!=' *range_expr reduce(TT_NE,  2)
                    |  $'<=' *range_expr reduce(TT_LE,  2)
                    |  $'>=' *range_expr reduce(TT_GE,  2)
                    |  $'<'  *range_expr reduce(TT_LT,  2)
                    |  $'>'  *range_expr reduce(TT_GT,  2)
                    |  $'eq' *range_expr reduce(TT_LEQ, 2)
                    |  $'ne' *range_expr reduce(TT_LNE, 2)
                    |  $'~~' FENCE(  *LitMatchGlobal *push_regex *push_kind_match_g reduce(TT_SMATCH, 3)
                                   | *LitSubst       *push_subst *push_kind_subst   reduce(TT_SMATCH, 3)
                                   | *LitRegex       *push_regex *push_kind_match   reduce(TT_SMATCH, 3))
                    |  epsilon
                    );
expr            =   FENCE(
                       VarScalar *push_var_scalar $'=' *expr reduce(TT_ASSIGN, 2)
                    |  $'gather' *block                       reduce(TT_GATHER, 1)
                    |  *cmp_expr
                    );
StmtList        =   nPush() ARBNO(nInc() *stmt) reduce(TT_SEQ_EXPR, nTop_count) nPop();
block           =   $'{' *StmtList $'}';
closure         =   $'{' *expr $'}';
ParamFirst      =   VarScalar *push_var_scalar nInc();
ParamRest       =   $',' VarScalar *push_var_scalar nInc();
WhenArm         =   $'when' *expr nInc() *block nInc();
when_list       =   ARBNO(*WhenArm);
sub_decl        =   $'sub' IdentName *push_ident_as_qlit
                    *push_ident_as_var nInc()
                    $'(' FENCE(*ParamFirst ARBNO(*ParamRest) | epsilon) $')'
                    *block nInc()
                    reduce(TT_SUB_DECL, nTop_count) ;
method_decl     =   $'method' IdentName *push_ident_as_qlit
                    *push_ident_as_var nInc() nInc()
                    $'(' FENCE(*ParamFirst ARBNO(*ParamRest) | epsilon) $')'
                    *block nInc()
                    reduce(TT_SUB_DECL, nTop_count) ;
class_decl      =   nPush()
                    $'class' IdentName *push_ident_as_var nInc()
                    $'{'
                    ARBNO(FENCE(  $'has' VarTwigil *push_var_twigil $';' nInc()
                                | $'has' VarScalar *push_var_scalar $';' nInc()
                                | nInc() *method_decl ))
                    $'}'
                    reduce(TT_CLASS_DECL, nTop_count) nPop();
if_stmt         =   $'if' $'(' *expr $')' *block
                    FENCE(  $'else' *if_stmt reduce(TT_IF, 3)
                          | $'else' *block   reduce(TT_IF, 3)
                          | epsilon          reduce(TT_IF, 2));
while_stmt      =   $'while' $'(' *expr $')' *block reduce(TT_WHILE, 2);
unless_stmt     =   $'unless' $'(' *expr $')' *block
                    FENCE(  $'else' *block reduce(TT_UNLESS, 3)
                          | epsilon        reduce(TT_UNLESS, 2));
until_stmt      =   $'until' $'(' *expr $')' *block reduce(TT_UNTIL, 2);
repeat_stmt     =   $'repeat' *block reduce(TT_REPEAT, 1);
forvar_strip    =   VarScalar *push_var_scalar;
for_stmt        =   $'for' FENCE(
                       *add_expr $'..^' *add_expr $'->' *forvar_strip *block
                         shift(epsilon, TT_ILIT) reduce(TT_FOR_RANGE, 5)
                    |  *add_expr $'..'  *add_expr $'->' *forvar_strip *block
                         shift(epsilon, TT_ILIT) reduce(TT_FOR_RANGE, 5)
                    |  *expr $'->' *forvar_strip *block
                         reduce(TT_ITERATE, 1) reduce(TT_EVERY, 2)
                    |  *expr *block
                         reduce(TT_ITERATE, 1) reduce(TT_EVERY, 2)
                    );
given_stmt      =   nPush()
                    $'given' *expr nInc()
                    $'{' *when_list
                    FENCE($'default' *block shift(epsilon, TT_NUL) nInc() nInc() | epsilon)
                    $'}'
                    reduce(TT_CASE, nTop_count) nPop();
try_stmt        =   $'try' *block FENCE($'CATCH' *block reduce(TT_TRY, 2) | reduce(TT_TRY, 1));
my_decl         =   $'my' FENCE(
                       IdentName *push_ident_as_var FENCE(
                            VarScalar *push_var_scalar FENCE($'=' *expr $';' reduce(TT_DECL, 3)
                                                            | $';'           reduce(TT_DECL, 2))
                          | VarArray  *push_var_array  FENCE($'=' *expr $';' reduce(TT_DECL, 3)
                                                            | $';'           reduce(TT_DECL, 2))
                          | VarHash   *push_var_hash   FENCE($'=' *expr $';' reduce(TT_DECL, 3)
                                                            | $';'           reduce(TT_DECL, 2)))
                    |  VarScalar *push_var_scalar $'=' *expr $';' reduce(TT_ASSIGN, 2)
                    |  VarArray  *push_var_array  $'=' *expr $';' reduce(TT_ASSIGN, 2)
                    |  VarHash   *push_var_hash   $'=' *expr $';' reduce(TT_ASSIGN, 2)
                    );
say_stmt        =   $'say' FENCE($'(' *expr $',' *expr $')' $';' reduce(TT_SAY_FH, 2)
                                | *expr $';' reduce(TT_SAY, 1));
print_stmt      =   $'print' FENCE($'(' *expr $',' *expr $')' $';' reduce(TT_PRINT_FH, 2)
                                  | *expr $';' reduce(TT_PRINT, 1));
take_stmt       =   $'take' *expr $';' reduce(TT_SUSPEND, 1);
return_stmt     =   $'return' FENCE(*expr $';' reduce(TT_RETURN, 1) | $';' reduce(TT_RETURN, 0));
delete_stmt     =   $'delete' VarHash *push_var_hash
                    FENCE($'<' HashAngleKey *push_hkey_qlit $'>' $';' reduce(TT_HASH_DELETE, 2)
                         | $'{' *expr $'}' $';' reduce(TT_HASH_DELETE, 2));
assign_stmt     =   FENCE(
                       VarScalar *push_var_scalar $'.' IdentName *push_ident_as_qlit
                            reduce(TT_FIELD, 1) $'=' *expr $';' reduce(TT_ASSIGN, 2)
                    |  VarArray *push_var_array $'[' *expr $']' $'=' *expr $';' reduce(TT_ARR_SET, 3)
                    |  VarHash  *push_var_hash  $'<' HashAngleKey *push_hkey_qlit $'>' $'=' *expr $';' reduce(TT_HASH_SET, 3)
                    |  VarHash  *push_var_hash  $'{' *expr $'}' $'=' *expr $';' reduce(TT_HASH_SET, 3)
                    |  VarScalar *push_var_scalar $'=' *expr $';' reduce(TT_ASSIGN, 2)
                    );
stmt            =   $' ' FENCE(
                       *my_decl
                    |  *say_stmt
                    |  *print_stmt
                    |  *take_stmt
                    |  *return_stmt
                    |  *delete_stmt
                    |  *assign_stmt
                    |  *if_stmt
                    |  *while_stmt
                    |  *for_stmt
                    |  *given_stmt
                    |  *try_stmt
                    |  *unless_stmt
                    |  *until_stmt
                    |  *repeat_stmt
                    |  *sub_decl
                    |  *class_decl
                    |  *expr $';'
                    );
Compiland       =   nPush() POS(0) ARBNO(*stmt) RPOS(0) reduce(Parse, nTop_count) nPop();
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
