/* PST-ICN-SC ✅ 2026-05-19 — Expr11 pure shift/reduce; zero violations. */
/* ==================================================================================================================== */
/* a $directive ($define, $include, $ifdef ...) is a whole line and only at a line's start: $( $) $< $> are brackets */
white        =   (  SPAN(' ' tab)
                 |  nl FENCE(SPAN(' ' tab) | epsilon) FENCE('$' FENCE(SPAN(' ' tab) | epsilon) ANY(&LCASE &UCASE) BREAK(nl) | epsilon)
                 |  POS(0) FENCE(SPAN(' ' tab) | epsilon) '$' FENCE(SPAN(' ' tab) | epsilon) ANY(&LCASE &UCASE) BREAK(nl)
                 |  '#' BREAK(nl)
                 );
White        =   white FENCE(*White | epsilon);
Gray         =   White | epsilon;
$' '         =   Gray;
$'  '        =   White;
Id           = ANY(&UCASE &LCASE '_') FENCE(SPAN(digits &UCASE &LCASE '_') | epsilon);
reserved     = POS(0) ('break' | 'by' | 'case' | 'create' | 'default' | 'do' | 'else' | 'end' | 'every' | 'fail'
                     | 'global' | 'if' | 'initial' | 'invocable' | 'link' | 'local' | 'next' | 'not' | 'of'
                     | 'procedure' | 'record' | 'repeat' | 'return' | 'static' | 'suspend' | 'then' | 'to'
                     | 'until' | 'while') RPOS(0);
id_pat       = Id $ tx $ *notmatch(tx, reserved);
int_pat      = SPAN(digits) FENCE(ANY('rR') SPAN(digits &UCASE &LCASE) | epsilon);
exp_part     = (('e' | 'E') ('+' | '-' | '') SPAN(digits));
real_pat     = (( SPAN(digits) '.' (SPAN(digits) | '') | '.' SPAN(digits) ) (exp_part | '')
               | SPAN(digits) exp_part
               );
str_pat      = ('"' BREAK('"') . strbody '"');
cset_pat     = ("'" BREAK("'") . csetbody "'");
strchars     = ARBNO(NOTANY('"\_') | '\^' LEN(1) | '\' LEN(1) | '_' nl FENCE(SPAN(' ' tab) | epsilon) | '_');
csetchars    = ARBNO(NOTANY("'\_") | '\^' LEN(1) | '\' LEN(1) | '_' nl FENCE(SPAN(' ' tab) | epsilon) | '_');
semi_opt     = (';' | epsilon);
$'if'        =  $' ' Id $ tx *IDENT(tx, 'if')       ;
$'then'      =  $' ' Id $ tx *IDENT(tx, 'then')     ;
$'else'      =  $' ' Id $ tx *IDENT(tx, 'else')     ;
$'while'     =  $' ' Id $ tx *IDENT(tx, 'while')    ;
$'do'        =  $' ' Id $ tx *IDENT(tx, 'do')       ;
$'every'     =  $' ' Id $ tx *IDENT(tx, 'every')    ;
$'return'    =  $' ' Id $ tx *IDENT(tx, 'return')   ;
$'end'       =  $' ' Id $ tx *IDENT(tx, 'end')      ;
$'procedure' =  $' ' Id $ tx *IDENT(tx, 'procedure');
$'until'     =  $' ' Id $ tx *IDENT(tx, 'until')    ;
$'repeat'    =  $' ' Id $ tx *IDENT(tx, 'repeat')   ;
$'break'     =  $' ' Id $ tx *IDENT(tx, 'break')    ;
$'next'      =  $' ' Id $ tx *IDENT(tx, 'next')     ;
$'case'      =  $' ' Id $ tx *IDENT(tx, 'case')     ;
$'of'        =  $' ' Id $ tx *IDENT(tx, 'of')       ;
$'default'   =  $' ' Id $ tx *IDENT(tx, 'default')  ;
$'to'        =  $' ' Id $ tx *IDENT(tx, 'to')       ;
$'by'        =  $' ' Id $ tx *IDENT(tx, 'by')       ;
$'global'    =  $' ' Id $ tx *IDENT(tx, 'global')   ;
$'local'     =  $' ' Id $ tx *IDENT(tx, 'local')    ;
$'static'    =  $' ' Id $ tx *IDENT(tx, 'static')   ;
$'record'    =  $' ' Id $ tx *IDENT(tx, 'record')   ;
$'initial'   =  $' ' Id $ tx *IDENT(tx, 'initial')  ;
$'suspend'   =  $' ' Id $ tx *IDENT(tx, 'suspend')  ;
$'fail'      =  $' ' Id $ tx *IDENT(tx, 'fail')     ;
$'not'       =  $' ' Id $ tx *IDENT(tx, 'not')      ;
$'create'    =  $' ' Id $ tx *IDENT(tx, 'create')   ;
$'link'      =  $' ' Id $ tx *IDENT(tx, 'link')     ;
$'invocable' =  $' ' Id $ tx *IDENT(tx, 'invocable');
$'('        =   $' ' '(' $' ';
$'['        =   $' ' ('[' | '$<') $' ';
$'{'        =   $' ' ('{' | '$(') $' ';
$')'        =   $' ' ')';
$']'        =   $' ' (']' | '$>');
$'}'        =   $' ' ('}' | '$)');
$','        =   $' ' ','   $' ';
$';'        =   $' ' ';'   $' ';
$':'        =   $' ' ':'   $' ';
$'.'        =   $' ' '.'   $' ';
$'|||'      =   $' ' '|||'   $' ';
$'||'       =   $' ' '||'    $' ';
$'|'        =   $' ' '|'     $' ';
$'++'       =   $' ' '++'    $' ';
$'--'       =   $' ' '--'    $' ';
$'**'       =   $' ' '**'    $' ';
$'+'        =   $' ' '+'     $' ';
$'-'        =   $' ' '-'     $' ';
$'*'        =   $' ' '*'     $' ';
$'/'        =   $' ' '/'     $' ';
$'%'        =   $' ' '%'     $' ';
$'^'        =   $' ' '^'     $' ';
$'?'        =   $' ' '?'     $' ';
$'~'        =   $' ' '~'     $' ';
$'!'        =   $' ' '!'     $' ';
$'@'        =   $' ' '@'     $' ';
$'&'        =   $' ' '&'     $' ';
$'\\'      =   $' ' '\'     $' ';
$'~==='     =   $' ' '~==='  $' ';
$'~=='      =   $' ' '~=='   $' ';
$'~='       =   $' ' '~='    $' ';
$'==='      =   $' ' '==='   $' ';
$'=='       =   $' ' '=='    $' ';
$'='        =   $' ' '='     $' ';
$'<='       =   $' ' '<='    $' ';
$'>='       =   $' ' '>='    $' ';
$'<<='      =   $' ' '<<='   $' ';
$'<<'       =   $' ' '<<'    $' ';
$'>>='      =   $' ' '>>='   $' ';
$'>>'       =   $' ' '>>'    $' ';
$'<'        =   $' ' '<' @lt_a (ANY('-=<') | epsilon) @lt_b *EQ(lt_a, lt_b) $' ';
$'>'        =   $' ' '>' @gt_a (ANY('=>')  | epsilon) @gt_b *EQ(gt_a, gt_b)  $' ';
$':=:'      =   $' ' ':=:'   $' ';
$':='       =   $' ' ':='    $' ';
$'+:'       =   $' ' '+:'    $' ';
$'-:'       =   $' ' '-:'    $' ';
$'<->'      =   $' ' '<->'   $' ';
$'<-'       =   $' ' '<-'    $' ';
$'~==:='    =   $' ' '~==:=' $' ';
$'~=:='     =   $' ' '~=:='  $' ';
$'<<=:='    =   $' ' '<<=:=' $' ';
$'<<:='     =   $' ' '<<:='  $' ';
$'>>=:='    =   $' ' '>>=:=' $' ';
$'>>:='     =   $' ' '>>:='  $' ';
$'==:='     =   $' ' '==:='  $' ';
$'<=:='     =   $' ' '<=:='  $' ';
$'>=:='     =   $' ' '>=:='  $' ';
$'<:='      =   $' ' '<:='   $' ';
$'>:='      =   $' ' '>:='   $' ';
$'+:='      =   $' ' '+:='   $' ';
$'-:='      =   $' ' '-:='   $' ';
$'*:='      =   $' ' '*:='   $' ';
$'/:='      =   $' ' '/:='   $' ';
$'%:='      =   $' ' '%:='   $' ';
$'^:='      =   $' ' '^:='   $' ';
$'||:='     =   $' ' '||:='  $' ';
$'++:='     =   $' ' '++:='  $' ';
$'--:='     =   $' ' '--:='  $' ';
$'**:='     =   $' ' '**:='  $' ';
$'?:='      =   $' ' '?:='   $' ';
$'=:='      =   $' ' '=:='   $' ';
$'@:='      =   $' ' '@:='   $' ';
$'&:='      =   $' ' '&:='   $' ';
$'|||:='    =   $' ' '|||:=' $' ';
$'~===:='   =   $' ' '~===:=' $' ';
$'===:='    =   $' ' '===:=' $' ';
/* ==================================================================================================================== */
/* Leaf-push helpers: allowed by PST rules — set v.sval/v.dval from token capture, no child inspection. */
/* ==================================================================================================================== */
If     = ( $'if'     $'  ' *Expr  $'then' $' ' *Expr
           (  $'else' $' ' *Expr  reduce('TT_IF', 3)
           |  reduce('TT_IF', 2)
           )
         );
While  = ( $'while'  $'  ' *Expr
           (  $'do' $' ' *Expr  reduce('TT_WHILE', 2)
           |  reduce('TT_WHILE', 1)
           )
         );
Until  = ( $'until'  $'  ' *Expr
           (  $'do' $' ' *Expr  reduce('TT_UNTIL', 2)
           |  reduce('TT_UNTIL', 1)
           )
         );
Every  = ( $'every'  $' ' *Expr
           (  $'do' $' ' *Expr  reduce('TT_EVERY', 2)
           |  reduce('TT_EVERY', 1)
           )
         );
Repeat = ( $'repeat' $' ' *Expr  reduce('TT_REPEAT', 1) );
Create = ( $'create' $' ' *Expr  reduce('TT_CREATE', 1) );
ArgFirst  = ( $' ' *Expr  nInc() );
/* an omitted argument f(a, , b) is &null, the C frontend's own leaf                                */
ArgRest   = ( $',' (*Expr | shift_value('&null', 'TT_VAR')) nInc() );
NullFirst = ( $' ' shift_value('&null', 'TT_VAR') nInc() ArgRest );
CallArgs  = ( ArgFirst ARBNO(ArgRest) | NullFirst ARBNO(ArgRest) | epsilon );
Call      = ( nPush()
              $' ' shift(id_pat, 'TT_VAR')  nInc()
              $'(' CallArgs $')'
              reduce('TT_FNC', 'nTop()')
              nPop()
            );
SeqRest   = ( $';' *Expr  nInc() );
ConjRest  = ( $',' (*Expr | shift_value('&null', 'TT_VAR')) nInc() );
/* (e1, e2) is mutual evaluation, TT_CONJ; (e1; e2) a sequence.                                */
Paren     = ( nPush()
              ( $' ' $'(' *Expr  nInc()
                ( ConjRest ARBNO(ConjRest) reduce('TT_CONJ', 'nTop()')
                | ARBNO(SeqRest) reduce('TT_SEQ_EXPR', "*(GT(nTop(), 1) nTop())")
                )
                $')'
              | $' ' $'(' $')' reduce('TT_SEQ_EXPR', 0)
              | $' ' $'(' shift_value('&null', 'TT_VAR') nInc() ConjRest ARBNO(ConjRest) reduce('TT_CONJ', 'nTop()') $')'
              )
              nPop()
            );
CompoundFirst = ( $' ' *Expr $' ' semi_opt $' ' nInc() );
CompoundRest  = ( $' ' *Expr $' ' semi_opt $' ' nInc() );
CompoundStar  = FENCE(CompoundRest *CompoundStar | epsilon);
Compound      = ( nPush()
                  $'{'
                  ( FENCE(CompoundFirst) *CompoundStar | epsilon )
                  $'}'
                  reduce('TT_SEQ_EXPR', "*(GT(nTop(), 1) nTop())")
                  nPop()
                );
ListFirst = ( $' ' *Expr  nInc() );
ListRest  = ( $',' (*Expr | shift_value('', 'TT_NUL')) nInc() );
NullListFirst = ( $' ' shift_value('', 'TT_NUL') nInc() ListRest );
ListCtor  = ( nPush()
              $' ' $'['
              ( ListFirst ARBNO(ListRest) | NullListFirst ARBNO(ListRest) | epsilon )
              $']'
              reduce('TT_MAKELIST', 'nTop()')
              nPop()
            );
/* FieldTail: shift field name as TT_VAR (source order: object already on stack below),
   then reduce('TT_FIELD', 2) gives children [object, TT_VAR(name)] in source order. */
FieldTail   = ( $'.' shift(id_pat, 'TT_VAR') reduce('TT_FIELD', 2) );
/* any primary may be invoked: (!p)(), 1(a, b), f(x) -- the callee is already on the stack.    */
IdxStar     = FENCE( $',' *Expr nInc() *IdxStar | epsilon );
/* f{e1, e2} invokes f with a list of co-expressions: TT_FNC(f, TT_MAKELIST(TT_CREATE e1, ...))      */
CoArg       = ( $' ' *Expr reduce('TT_CREATE', 1) nInc() );
Expr11tail  = ( nPush() $'(' CallArgs $')' reduce('TT_FNC', 'nTop() + 1') nPop()
              | nPush() $'{' ( CoArg ARBNO($',' CoArg) | epsilon ) $'}' reduce('TT_MAKELIST', 'nTop()') reduce('TT_FNC', 2) nPop()
              | nPush() nInc() $'['
                ( *Expr nInc()
                  FENCE( $'+:' *Expr $']' reduce('TT_SECTION_PLUS',  3)
                       | $'-:' *Expr $']' reduce('TT_SECTION_MINUS', 3)
                       | $':'  *Expr $']' reduce('TT_SECTION',       3)
                       | *IdxStar $']'   reduce('TT_IDX',            'nTop()')
                       )
                | shift_value('&null', 'TT_VAR') nInc() $']' reduce('TT_IDX', 'nTop()')
                )
                nPop()
              | FieldTail
              );
/* the blanks inside a case take the greedy form too: each clause sits in a FENCE, so a shortest-first  */
/* CaseGray that stopped before ` ;` could never be re-entered to take it                                */
CaseGray     = (White | epsilon);
CaseClause   = ( *CaseGray *Expr *CaseGray $':' *Expr *CaseGray semi_opt nInc() nInc() );
CaseDefault  = ( *CaseGray $'default' *CaseGray $':' *Expr *CaseGray semi_opt nInc() );
Case         = ( nPush()
                 $'case' $' ' *Expr  nInc()
                 $'of' *CaseGray $'{' *CaseGray
                 ARBNO( FENCE(CaseDefault | CaseClause) )
                 *CaseGray $'}'
                 reduce('TT_CASE', 'nTop()')
                 nPop()
               );
/* return and suspend are expressions too (a | return b): DEFERRED, because they are defined below   */
/* and a by-value reference here would be the empty pattern, which matches everywhere (measured)     */
Expr11 = (   If  |  Until  |  While  |  Every  |  Repeat  |  Case  |  Create  |  *ReturnExpr  |  *SuspendExpr
         |   $'break' FENCE( SPAN(' ' tab) *Expr reduce('TT_LOOP_BREAK', 1) | $' ' reduce('TT_LOOP_BREAK', 0) )
         |   $'next'  $' '  reduce('TT_LOOP_NEXT', 0)
         |   $'fail'  $' '  reduce('TT_PROC_FAIL', 0)
         |   ListCtor
         |   Call  |  Paren  |  Compound
         |   $' ' "'" shift(csetchars, 'TT_CSET') "'"
         |   $' ' '"' shift(strchars, 'TT_QLIT') '"'
         |   $' ' shift(real_pat, 'TT_FLIT')
         |   $' ' shift(int_pat, 'TT_ILIT')
         |   $' ' shift('&' Id, 'TT_VAR')
         |   $' ' shift(id_pat, 'TT_VAR')
         );
Expr10 = (   $'-'        *Expr10 reduce('TT_MNS', 1)
         |   $'+'        *Expr10 reduce('TT_PLS', 1)
         |   $'~'        *Expr10 reduce('TT_CSET_COMPL', 1)
         |   $'\\'       *Expr10 reduce('TT_NONNULL', 1)
         |   $'!'        *Expr10 reduce('TT_ITERATE', 1)
         |   $'*'        *Expr10 reduce('TT_SIZE', 1)
         |   $'?'        *Expr10 reduce('TT_RANDOM', 1)
         |   $'/'        *Expr10 reduce('TT_NULL', 1)
         |   $'='        *Expr10 reduce('TT_MATCH_UNARY', 1)
         |   $'not' $' ' *Expr10 reduce('TT_NOT', 1)
         |   $'|'        *Expr10 reduce('TT_REPALT', 1)
         |   $'@'        *Expr10 reduce('TT_ACTIVATE', 1)
         |   $'^' nPush() shift_value('ICN$REFRESH', 'TT_VAR') nInc() *Expr10 nInc() reduce('TT_FNC', 'nTop()') nPop()
         |   *Expr11  *Expr11rest
         |   $'.'        *Expr10 reduce('TT_DEREF', 1)
         );
Expr11rest = FENCE(Expr11tail *Expr11rest | epsilon);
Expr9tail = FENCE( $'\\' *Expr10 reduce('TT_LIMIT', 2)
                 | $'!'  *Expr10 reduce('TT_BANG_BINARY', 2)
                 | $'@'  *Expr10 reduce('TT_ACTIVATE', 2)
                 );
Expr9     = ( *Expr10 *Expr9rest );
Expr9rest = FENCE(Expr9tail *Expr9rest | epsilon);
Expr8     = ( *Expr9 FENCE($'^' *Expr8 reduce('TT_POW', 2) | epsilon) );
Expr7tail = FENCE( $'**' *Expr8 reduce('TT_CSET_INTER', 2)
                 | $'*'  *Expr8 reduce('TT_MUL', 2)
                 | $'/'  *Expr8 reduce('TT_DIV', 2)
                 | $'%'  *Expr8 reduce('TT_MOD', 2)
                 );
Expr7     = ( *Expr8 *Expr7rest );
Expr7rest = FENCE(Expr7tail *Expr7rest | epsilon);
Expr6tail = FENCE( $'++' *Expr7 reduce('TT_CSET_UNION', 2)
                 | $'--' *Expr7 reduce('TT_CSET_DIFF', 2)
                 | $'+'  *Expr7 reduce('TT_ADD', 2)
                 | $'-'  *Expr7 reduce('TT_SUB', 2)
                 );
Expr6     = ( *Expr7 *Expr6rest );
Expr6rest = FENCE(Expr6tail *Expr6rest | epsilon);
Expr5tail = FENCE( $'|||' *Expr6 reduce('TT_LCONCAT', 2) | $'||' *Expr6 reduce('TT_CAT', 2) );
Expr5     = ( *Expr6 *Expr5rest );
Expr5rest = FENCE(Expr5tail *Expr5rest | epsilon);
Expr4tail = FENCE( $'<<='  *Expr5 reduce('TT_LLE', 2) | $'<<'   *Expr5 reduce('TT_LLT', 2)
                 | $'>>='  *Expr5 reduce('TT_LGE', 2) | $'>>'   *Expr5 reduce('TT_LGT', 2)
                 | $'~===' *Expr5 reduce('TT_IDENTICAL', 2) reduce('TT_NOT', 1)
                 | $'~=='  *Expr5 reduce('TT_LNE', 2)
                 | $'==='  *Expr5 reduce('TT_IDENTICAL', 2)
                 | $'=='   *Expr5 reduce('TT_LEQ', 2)
                 | $'<='   *Expr5 reduce('TT_LE', 2) | $'>='   *Expr5 reduce('TT_GE', 2)
                 | $'~='   *Expr5 reduce('TT_NE', 2) | $'<'    *Expr5 reduce('TT_LT', 2)
                 | $'>'    *Expr5 reduce('TT_GT', 2) | $'='    *Expr5 reduce('TT_EQ', 2)
                 );
Expr4     = ( *Expr5 *Expr4rest );
Expr4rest = FENCE(Expr4tail *Expr4rest | epsilon);
X3        = ( nInc() *Expr4 FENCE($'|' *X3 | epsilon) );
Expr3     = ( nPush() X3 reduce('TT_ALTERNATE', "*(GT(nTop(), 1) nTop())") nPop() );
ToStar    = FENCE(  $'to' $'  ' *Expr3
                    FENCE( $'by' $'  ' *Expr3 reduce('TT_TO_BY', 3)
                         | reduce('TT_TO', 2)
                         )
                    *ToStar
                 |  epsilon
                 );
Expr2     = ( *Expr3 *ToStar );
Expr1     = ( *Expr2
              FENCE(
                  $'|||:=' *Expr1 reduce('TT_AUGOP', 2)
              |   $'~===:=' *Expr1 reduce('TT_AUGOP', 2)
              |   $'===:=' *Expr1 reduce('TT_AUGOP', 2)
              |   $'<<=:=' *Expr1 reduce('TT_AUGOP', 2)
              |   $'>>=:=' *Expr1 reduce('TT_AUGOP', 2)
              |   $'~==:=' *Expr1 reduce('TT_AUGOP', 2)
              |   $'<=:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'>=:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'~=:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'==:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'<<:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'>>:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'||:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'++:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'--:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'**:='  *Expr1 reduce('TT_AUGOP', 2)
              |   $'+:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'-:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'*:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'/:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'%:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'^:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'?:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'=:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'@:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'&:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'<:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $'>:='   *Expr1 reduce('TT_AUGOP', 2)
              |   $':=:'   *Expr1 reduce('TT_SWAP', 2)
              |   $'<->'   *Expr1 reduce('TT_REVSWAP', 2)
              |   $'<-'    *Expr1 reduce('TT_REVASSIGN', 2)
              |   $':='    *Expr1 reduce('TT_ASSIGN', 2)
              |   epsilon
              )
            );
ReturnExpr  = ( nPush()
                $'return' $' ' *Expr1a nInc()  reduce('TT_RETURN', 1) nPop()
              | $'return' $' '                   reduce('TT_RETURN', 0)
              );
SuspendExpr = ( nPush()
                ( $'suspend' $' ' *Expr1a nInc()
                  FENCE( $'do' $'  ' *Expr1a nInc() | epsilon )
                | $'suspend' $' '
                )
                reduce('TT_SUSPEND', 'nTop()') nPop()
              );
Expr1a    = ( *Expr1 FENCE($'?' *Expr reduce('TT_SCAN', 2) | epsilon) );
ExprSeqRest = ( $'&' ( ReturnExpr | SuspendExpr | *Expr1a ) nInc() );
ExprSeqStar = FENCE(ExprSeqRest *ExprSeqStar | epsilon);
Expr        = ( nPush()
                ( ReturnExpr | SuspendExpr
                | *Expr1a
                )
                nInc() *ExprSeqStar reduce('TT_SEQ', "*(GT(nTop(), 1) nTop())") nPop()
              );
Blank     = ( $' ' );
ReturnStmt = ( $'return' $' ' *Expr $' ' semi_opt $' ' reduce('TT_RETURN', 1)
             | $'return' $' '  semi_opt $' '             reduce('TT_RETURN', 0)
             );
DeclFirst  = ( $' ' shift(id_pat, 'TT_VAR') nInc() );
DeclRest   = ( $','  shift(id_pat, 'TT_VAR') nInc() );
DeclStar   = FENCE(DeclRest *DeclStar | epsilon);
DeclIds    = ( DeclFirst *DeclStar );
/* LocalDecl: collect var names, reduce to TT_LOCAL node, push bare (no STMT wrap). */
LocalDecl  = ( nPush() $'local'  $'  ' DeclIds $' ' semi_opt $' ' reduce('TT_LOCAL',      'nTop()') nPop() );
StaticDecl = ( nPush() $'static' $'  ' DeclIds $' ' semi_opt $' ' reduce('TT_STATIC_DECL', 'nTop()') nPop() );
InitialStmt = ( nPush() $'initial' $' ' *Expr nInc() $' ' semi_opt $' '
                reduce('TT_INITIAL', 'nTop()')
                nPop()
              );
SuspendStmt = ( nPush()
                ( $'suspend' $' ' *Expr nInc()
                  FENCE( $'do' $'  ' *Expr nInc() | epsilon )
                | $'suspend' $' '
                )
                $' ' semi_opt $' '
                reduce('TT_SUSPEND', 'nTop()') nPop()
              );
FailStmt    = ( $'fail'    $' '         semi_opt $' '      reduce('TT_PROC_FAIL', 0) );
StmtBody  = ( LocalDecl nInc()
            | StaticDecl nInc()
            | InitialStmt nInc()
            | ReturnStmt nInc()
            | SuspendStmt nInc()
            | FailStmt nInc()
            | $' ' *Expr $' ' semi_opt $' ' nInc()
            );
ParamFirst = ( $' ' shift(id_pat, 'TT_VAR')  nInc() );
ParamRest  = ( $',' shift(id_pat, 'TT_VAR')  nInc() );
Params     = ( ParamFirst ARBNO(ParamRest) ($'[' $']' | epsilon) | epsilon );
Prochead   = ( $'procedure' $'  ' shift(id_pat, 'TT_VAR')  nInc()
               $'(' Params $')' $' ' semi_opt $' '
             );
ProcbodyEnd = ( $'end' $' ' ($' ' | RPOS(0)) );
/* a statement, once matched, is never re-matched when a later one refuses: the refusal is linear  */
Procbody    = ( ProcbodyEnd | FENCE(StmtBody) *Procbody );
/* Proc: collect name + params + stmts; reduce to TT_FNC; wrap in :subj then STMT. */
Proc        = ( nPush()  Prochead  Procbody
                reduce('TT_FNC', 'nTop()') reduce(':subj', 1) reduce('STMT', 1)
                nPop() FLUSH
              );
/* GlobalDecl: collect var names; reduce to TT_GLOBAL; wrap in :subj then STMT. */
GlobalDecl = ( nPush() $'global' $'  ' DeclIds $' ' semi_opt $' '
               reduce('TT_GLOBAL', 'nTop()') reduce(':subj', 1) reduce('STMT', 1)
               nPop()
             );
RecordField = ( $',' shift(id_pat, 'TT_VAR') nInc() );
/* Record: collect name + fields; reduce to TT_RECORD; wrap in :subj then STMT. */
Record      = ( nPush()
                $'record' $'  ' shift(id_pat, 'TT_VAR') nInc()
                $'(' ( $' ' shift(id_pat, 'TT_VAR') nInc() ARBNO(RecordField) | epsilon ) $')'
                $' '
                reduce('TT_RECORD', 'nTop()') reduce(':subj', 1) reduce('STMT', 1)
                nPop()
              );
/* link a, "b" and invocable all, "+": names as TT_VAR leaves, a quoted name too (the C frontend's shape). */
LinkName    = ( ( $' ' '"' shift(BREAK('"'), 'TT_VAR') '"' | $' ' shift(id_pat, 'TT_VAR') ) FENCE($':' SPAN(digits) | epsilon) nInc() );
LinkStar    = FENCE( $',' LinkName *LinkStar | epsilon );
LinkDecl    = ( nPush() $'link' $'  ' LinkName *LinkStar $' ' semi_opt $' '
                reduce('TT_LINK', 'nTop()') reduce(':subj', 1) reduce('STMT', 1)
                nPop()
              );
InvocableDecl = ( nPush() $'invocable' $'  ' LinkName *LinkStar $' ' semi_opt $' '
                reduce('TT_INVOCABLE', 'nTop()') reduce(':subj', 1) reduce('STMT', 1)
                nPop()
              );
TopStar   = FENCE( nInc() $' ' (GlobalDecl | Record | Proc | LinkDecl | InvocableDecl) $' ' *TopStar | epsilon );
Compiland = ( nPush()
              POS(0) $' ' *TopStar RPOS(0)
              reduce('Parse', 'nTop()')
              nPop()
            );
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
        } else OUTPUT = 'Parse Error';
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
