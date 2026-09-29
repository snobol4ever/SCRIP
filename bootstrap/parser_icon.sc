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
$'if'        =  *$' ' Id $ tx *IDENT(tx, 'if')       ;
$'then'      =  *$' ' Id $ tx *IDENT(tx, 'then')     ;
$'else'      =  *$' ' Id $ tx *IDENT(tx, 'else')     ;
$'while'     =  *$' ' Id $ tx *IDENT(tx, 'while')    ;
$'do'        =  *$' ' Id $ tx *IDENT(tx, 'do')       ;
$'every'     =  *$' ' Id $ tx *IDENT(tx, 'every')    ;
$'return'    =  *$' ' Id $ tx *IDENT(tx, 'return')   ;
$'end'       =  *$' ' Id $ tx *IDENT(tx, 'end')      ;
$'procedure' =  *$' ' Id $ tx *IDENT(tx, 'procedure');
$'until'     =  *$' ' Id $ tx *IDENT(tx, 'until')    ;
$'repeat'    =  *$' ' Id $ tx *IDENT(tx, 'repeat')   ;
$'break'     =  *$' ' Id $ tx *IDENT(tx, 'break')    ;
$'next'      =  *$' ' Id $ tx *IDENT(tx, 'next')     ;
$'case'      =  *$' ' Id $ tx *IDENT(tx, 'case')     ;
$'of'        =  *$' ' Id $ tx *IDENT(tx, 'of')       ;
$'default'   =  *$' ' Id $ tx *IDENT(tx, 'default')  ;
$'to'        =  *$' ' Id $ tx *IDENT(tx, 'to')       ;
$'by'        =  *$' ' Id $ tx *IDENT(tx, 'by')       ;
$'global'    =  *$' ' Id $ tx *IDENT(tx, 'global')   ;
$'local'     =  *$' ' Id $ tx *IDENT(tx, 'local')    ;
$'static'    =  *$' ' Id $ tx *IDENT(tx, 'static')   ;
$'record'    =  *$' ' Id $ tx *IDENT(tx, 'record')   ;
$'initial'   =  *$' ' Id $ tx *IDENT(tx, 'initial')  ;
$'suspend'   =  *$' ' Id $ tx *IDENT(tx, 'suspend')  ;
$'fail'      =  *$' ' Id $ tx *IDENT(tx, 'fail')     ;
$'not'       =  *$' ' Id $ tx *IDENT(tx, 'not')      ;
$'create'    =  *$' ' Id $ tx *IDENT(tx, 'create')   ;
$'link'      =  *$' ' Id $ tx *IDENT(tx, 'link')     ;
$'invocable' =  *$' ' Id $ tx *IDENT(tx, 'invocable');
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
$'<'        =   *$' ' '<' @lt_a (ANY('-=<') | epsilon) @lt_b *EQ(lt_a, lt_b) *$' ';
$'>'        =   *$' ' '>' @gt_a (ANY('=>')  | epsilon) @gt_b *EQ(gt_a, gt_b)  *$' ';
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
If     = ( *$'if'     *$'  ' *Expr  *$'then' *$' ' *Expr
           (  *$'else' *$' ' *Expr  . *Reduce('TT_IF', 3)
           |  epsilon . *Reduce('TT_IF', 2)
           )
         );
While  = ( *$'while'  *$'  ' *Expr
           (  *$'do' *$' ' *Expr  . *Reduce('TT_WHILE', 2)
           |  epsilon . *Reduce('TT_WHILE', 1)
           )
         );
Until  = ( *$'until'  *$'  ' *Expr
           (  *$'do' *$' ' *Expr  . *Reduce('TT_UNTIL', 2)
           |  epsilon . *Reduce('TT_UNTIL', 1)
           )
         );
Every  = ( *$'every'  *$' ' *Expr
           (  *$'do' *$' ' *Expr  . *Reduce('TT_EVERY', 2)
           |  epsilon . *Reduce('TT_EVERY', 1)
           )
         );
Repeat = ( *$'repeat' *$' ' *Expr  . *Reduce('TT_REPEAT', 1) );
Create = ( *$'create' *$' ' *Expr  . *Reduce('TT_CREATE', 1) );
ArgFirst  = ( *$' ' *Expr  . *IncCounter() );
/* an omitted argument f(a, , b) is &null, the C frontend's own leaf                                */
ArgRest   = ( *$',' (*Expr | epsilon . *Shift('TT_VAR', '&null')) . *IncCounter() );
NullFirst = ( *$' ' . *Shift('TT_VAR', '&null') . *IncCounter() ArgRest );
CallArgs  = ( ArgFirst ARBNO(ArgRest) | NullFirst ARBNO(ArgRest) | epsilon );
Call      = ( epsilon . *PushCounter()
              *$' ' (id_pat) . thx . *Shift('TT_VAR', thx)  . *IncCounter()
              *$'(' CallArgs *$')'
              . *Reduce('TT_FNC', nTop())
              . *PopCounter()
            );
SeqRest   = ( *$';' *Expr  . *IncCounter() );
ConjRest  = ( *$',' (*Expr | epsilon . *Shift('TT_VAR', '&null')) . *IncCounter() );
/* (e1, e2) is mutual evaluation, TT_CONJ; (e1; e2) a sequence.                                */
Paren     = ( epsilon . *PushCounter()
              ( *$' ' *$'(' *Expr  . *IncCounter()
                ( ConjRest ARBNO(ConjRest) . *Reduce('TT_CONJ', nTop())
                | ARBNO(SeqRest) . *Reduce('TT_SEQ_EXPR', *(GT(nTop(), 1) nTop()))
                )
                *$')'
              | *$' ' *$'(' *$')' . *Reduce('TT_SEQ_EXPR', 0)
              | *$' ' *$'(' . *Shift('TT_VAR', '&null') . *IncCounter() ConjRest ARBNO(ConjRest) . *Reduce('TT_CONJ', nTop()) *$')'
              )
              . *PopCounter()
            );
CompoundFirst = ( *$' ' *Expr *$' ' semi_opt *$' ' . *IncCounter() );
CompoundRest  = ( *$' ' *Expr *$' ' semi_opt *$' ' . *IncCounter() );
CompoundStar  = FENCE(CompoundRest *CompoundStar | epsilon);
Compound      = ( epsilon . *PushCounter()
                  *$'{'
                  ( FENCE(CompoundFirst) *CompoundStar | epsilon )
                  *$'}'
                  . *Reduce('TT_SEQ_EXPR', *(GT(nTop(), 1) nTop()))
                  . *PopCounter()
                );
ListFirst = ( *$' ' *Expr  . *IncCounter() );
ListRest  = ( *$',' (*Expr | epsilon . *Shift('TT_NUL', '')) . *IncCounter() );
NullListFirst = ( *$' ' . *Shift('TT_NUL', '') . *IncCounter() ListRest );
ListCtor  = ( epsilon . *PushCounter()
              *$' ' *$'['
              ( ListFirst ARBNO(ListRest) | NullListFirst ARBNO(ListRest) | epsilon )
              *$']'
              . *Reduce('TT_MAKELIST', nTop())
              . *PopCounter()
            );
/* FieldTail: shift field name as TT_VAR (source order: object already on stack below),
   then reduce('TT_FIELD', 2) gives children [object, TT_VAR(name)] in source order. */
FieldTail   = ( *$'.' (id_pat) . thx . *Shift('TT_VAR', thx) . *Reduce('TT_FIELD', 2) );
/* any primary may be invoked: (!p)(), 1(a, b), f(x) -- the callee is already on the stack.    */
IdxStar     = FENCE( *$',' *Expr . *IncCounter() *IdxStar | epsilon );
/* f{e1, e2} invokes f with a list of co-expressions: TT_FNC(f, TT_MAKELIST(TT_CREATE e1, ...))      */
CoArg       = ( *$' ' *Expr . *Reduce('TT_CREATE', 1) . *IncCounter() );
Expr11tail  = ( epsilon . *PushCounter() *$'(' CallArgs *$')' . *Reduce('TT_FNC', nTop() + 1) . *PopCounter()
              | epsilon . *PushCounter() *$'{' ( CoArg ARBNO(*$',' CoArg) | epsilon ) *$'}' . *Reduce('TT_MAKELIST', nTop()) . *Reduce('TT_FNC', 2) . *PopCounter()
              | epsilon . *PushCounter() . *IncCounter() *$'['
                ( *Expr . *IncCounter()
                  FENCE( *$'+:' *Expr *$']' . *Reduce('TT_SECTION_PLUS', 3)
                       | *$'-:' *Expr *$']' . *Reduce('TT_SECTION_MINUS', 3)
                       | *$':'  *Expr *$']' . *Reduce('TT_SECTION', 3)
                       | *IdxStar *$']'   . *Reduce('TT_IDX', nTop())
                       )
                | epsilon . *Shift('TT_VAR', '&null') . *IncCounter() *$']' . *Reduce('TT_IDX', nTop())
                )
                . *PopCounter()
              | FieldTail
              );
/* the blanks inside a case take the greedy form too: each clause sits in a FENCE, so a shortest-first  */
/* CaseGray that stopped before ` ;` could never be re-entered to take it                                */
CaseGray     = (White | epsilon);
CaseClause   = ( *CaseGray *Expr *CaseGray *$':' *Expr *CaseGray semi_opt . *IncCounter() . *IncCounter() );
CaseDefault  = ( *CaseGray *$'default' *CaseGray *$':' *Expr *CaseGray semi_opt . *IncCounter() );
Case         = ( epsilon . *PushCounter()
                 *$'case' *$' ' *Expr  . *IncCounter()
                 *$'of' *CaseGray *$'{' *CaseGray
                 ARBNO( FENCE(CaseDefault | CaseClause) )
                 *CaseGray *$'}'
                 . *Reduce('TT_CASE', nTop())
                 . *PopCounter()
               );
/* return and suspend are expressions too (a | return b): DEFERRED, because they are defined below   */
/* and a by-value reference here would be the empty pattern, which matches everywhere (measured)     */
Expr11 = (   If  |  Until  |  While  |  Every  |  Repeat  |  Case  |  Create  |  *ReturnExpr  |  *SuspendExpr
         |   *$'break' FENCE( SPAN(' ' tab) *Expr . *Reduce('TT_LOOP_BREAK', 1) | *$' ' . *Reduce('TT_LOOP_BREAK', 0) )
         |   *$'next'  *$' '  . *Reduce('TT_LOOP_NEXT', 0)
         |   *$'fail'  *$' '  . *Reduce('TT_PROC_FAIL', 0)
         |   ListCtor
         |   Call  |  Paren  |  Compound
         |   *$' ' "'" (csetchars) . thx . *Shift('TT_CSET', thx) "'"
         |   *$' ' '"' (strchars) . thx . *Shift('TT_QLIT', thx) '"'
         |   *$' ' (real_pat) . thx . *Shift('TT_FLIT', thx)
         |   *$' ' (int_pat) . thx . *Shift('TT_ILIT', thx)
         |   *$' ' ('&' Id) . thx . *Shift('TT_VAR', thx)
         |   *$' ' (id_pat) . thx . *Shift('TT_VAR', thx)
         );
Expr10 = (   *$'-'        *Expr10 . *Reduce('TT_MNS', 1)
         |   *$'+'        *Expr10 . *Reduce('TT_PLS', 1)
         |   *$'~'        *Expr10 . *Reduce('TT_CSET_COMPL', 1)
         |   *$'\\'       *Expr10 . *Reduce('TT_NONNULL', 1)
         |   *$'!'        *Expr10 . *Reduce('TT_ITERATE', 1)
         |   *$'*'        *Expr10 . *Reduce('TT_SIZE', 1)
         |   *$'?'        *Expr10 . *Reduce('TT_RANDOM', 1)
         |   *$'/'        *Expr10 . *Reduce('TT_NULL', 1)
         |   *$'='        *Expr10 . *Reduce('TT_MATCH_UNARY', 1)
         |   *$'not' *$' ' *Expr10 . *Reduce('TT_NOT', 1)
         |   *$'|'        *Expr10 . *Reduce('TT_REPALT', 1)
         |   *$'@'        *Expr10 . *Reduce('TT_ACTIVATE', 1)
         |   *$'^' . *PushCounter() . *Shift('TT_VAR', 'ICN$REFRESH') . *IncCounter() *Expr10 . *IncCounter() . *Reduce('TT_FNC', nTop()) . *PopCounter()
         |   *Expr11  *Expr11rest
         |   *$'.'        *Expr10 . *Reduce('TT_DEREF', 1)
         );
Expr11rest = FENCE(Expr11tail *Expr11rest | epsilon);
Expr9tail = FENCE( *$'\\' *Expr10 . *Reduce('TT_LIMIT', 2)
                 | *$'!'  *Expr10 . *Reduce('TT_BANG_BINARY', 2)
                 | *$'@'  *Expr10 . *Reduce('TT_ACTIVATE', 2)
                 );
Expr9     = ( *Expr10 *Expr9rest );
Expr9rest = FENCE(Expr9tail *Expr9rest | epsilon);
Expr8     = ( *Expr9 FENCE(*$'^' *Expr8 . *Reduce('TT_POW', 2) | epsilon) );
Expr7tail = FENCE( *$'**' *Expr8 . *Reduce('TT_CSET_INTER', 2)
                 | *$'*'  *Expr8 . *Reduce('TT_MUL', 2)
                 | *$'/'  *Expr8 . *Reduce('TT_DIV', 2)
                 | *$'%'  *Expr8 . *Reduce('TT_MOD', 2)
                 );
Expr7     = ( *Expr8 *Expr7rest );
Expr7rest = FENCE(Expr7tail *Expr7rest | epsilon);
Expr6tail = FENCE( *$'++' *Expr7 . *Reduce('TT_CSET_UNION', 2)
                 | *$'--' *Expr7 . *Reduce('TT_CSET_DIFF', 2)
                 | *$'+'  *Expr7 . *Reduce('TT_ADD', 2)
                 | *$'-'  *Expr7 . *Reduce('TT_SUB', 2)
                 );
Expr6     = ( *Expr7 *Expr6rest );
Expr6rest = FENCE(Expr6tail *Expr6rest | epsilon);
Expr5tail = FENCE( *$'|||' *Expr6 . *Reduce('TT_LCONCAT', 2) | *$'||' *Expr6 . *Reduce('TT_CAT', 2) );
Expr5     = ( *Expr6 *Expr5rest );
Expr5rest = FENCE(Expr5tail *Expr5rest | epsilon);
Expr4tail = FENCE( *$'<<='  *Expr5 . *Reduce('TT_LLE', 2) | *$'<<'   *Expr5 . *Reduce('TT_LLT', 2)
                 | *$'>>='  *Expr5 . *Reduce('TT_LGE', 2) | *$'>>'   *Expr5 . *Reduce('TT_LGT', 2)
                 | *$'~===' *Expr5 . *Reduce('TT_IDENTICAL', 2) . *Reduce('TT_NOT', 1)
                 | *$'~=='  *Expr5 . *Reduce('TT_LNE', 2)
                 | *$'==='  *Expr5 . *Reduce('TT_IDENTICAL', 2)
                 | *$'=='   *Expr5 . *Reduce('TT_LEQ', 2)
                 | *$'<='   *Expr5 . *Reduce('TT_LE', 2) | *$'>='   *Expr5 . *Reduce('TT_GE', 2)
                 | *$'~='   *Expr5 . *Reduce('TT_NE', 2) | *$'<'    *Expr5 . *Reduce('TT_LT', 2)
                 | *$'>'    *Expr5 . *Reduce('TT_GT', 2) | *$'='    *Expr5 . *Reduce('TT_EQ', 2)
                 );
Expr4     = ( *Expr5 *Expr4rest );
Expr4rest = FENCE(Expr4tail *Expr4rest | epsilon);
X3        = ( epsilon . *IncCounter() *Expr4 FENCE(*$'|' *X3 | epsilon) );
Expr3     = ( epsilon . *PushCounter() X3 . *Reduce('TT_ALTERNATE', *(GT(nTop(), 1) nTop())) . *PopCounter() );
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
                  *$'|||:=' *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'~===:=' *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'===:=' *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'<<=:=' *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'>>=:=' *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'~==:=' *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'<=:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'>=:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'~=:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'==:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'<<:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'>>:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'||:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'++:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'--:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'**:='  *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'+:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'-:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'*:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'/:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'%:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'^:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'?:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'=:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'@:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'&:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'<:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$'>:='   *Expr1 . *Reduce('TT_AUGOP', 2)
              |   *$':=:'   *Expr1 . *Reduce('TT_SWAP', 2)
              |   *$'<->'   *Expr1 . *Reduce('TT_REVSWAP', 2)
              |   *$'<-'    *Expr1 . *Reduce('TT_REVASSIGN', 2)
              |   *$':='    *Expr1 . *Reduce('TT_ASSIGN', 2)
              |   epsilon
              )
            );
ReturnExpr  = ( epsilon . *PushCounter()
                *$'return' *$' ' *Expr1a . *IncCounter()  . *Reduce('TT_RETURN', 1) . *PopCounter()
              | *$'return' *$' '                   . *Reduce('TT_RETURN', 0)
              );
SuspendExpr = ( epsilon . *PushCounter()
                ( *$'suspend' *$' ' *Expr1a . *IncCounter()
                  FENCE( *$'do' *$'  ' *Expr1a . *IncCounter() | epsilon )
                | *$'suspend' *$' '
                )
                . *Reduce('TT_SUSPEND', nTop()) . *PopCounter()
              );
Expr1a    = ( *Expr1 FENCE(*$'?' *Expr . *Reduce('TT_SCAN', 2) | epsilon) );
ExprSeqRest = ( *$'&' ( ReturnExpr | SuspendExpr | *Expr1a ) . *IncCounter() );
ExprSeqStar = FENCE(ExprSeqRest *ExprSeqStar | epsilon);
Expr        = ( epsilon . *PushCounter()
                ( ReturnExpr | SuspendExpr
                | *Expr1a
                )
                . *IncCounter() *ExprSeqStar . *Reduce('TT_SEQ', *(GT(nTop(), 1) nTop())) . *PopCounter()
              );
Blank     = ( *$' ' );
ReturnStmt = ( *$'return' *$' ' *Expr *$' ' semi_opt *$' ' . *Reduce('TT_RETURN', 1)
             | *$'return' *$' '  semi_opt *$' '             . *Reduce('TT_RETURN', 0)
             );
DeclFirst  = ( *$' ' (id_pat) . thx . *Shift('TT_VAR', thx) . *IncCounter() );
DeclRest   = ( *$','  (id_pat) . thx . *Shift('TT_VAR', thx) . *IncCounter() );
DeclStar   = FENCE(DeclRest *DeclStar | epsilon);
DeclIds    = ( DeclFirst *DeclStar );
/* LocalDecl: collect var names, reduce to TT_LOCAL node, push bare (no STMT wrap). */
LocalDecl  = ( epsilon . *PushCounter() *$'local'  *$'  ' DeclIds *$' ' semi_opt *$' ' . *Reduce('TT_LOCAL', nTop()) . *PopCounter() );
StaticDecl = ( epsilon . *PushCounter() *$'static' *$'  ' DeclIds *$' ' semi_opt *$' ' . *Reduce('TT_STATIC_DECL', nTop()) . *PopCounter() );
InitialStmt = ( epsilon . *PushCounter() *$'initial' *$' ' *Expr . *IncCounter() *$' ' semi_opt *$' '
                . *Reduce('TT_INITIAL', nTop())
                . *PopCounter()
              );
SuspendStmt = ( epsilon . *PushCounter()
                ( *$'suspend' *$' ' *Expr . *IncCounter()
                  FENCE( *$'do' *$'  ' *Expr . *IncCounter() | epsilon )
                | *$'suspend' *$' '
                )
                *$' ' semi_opt *$' '
                . *Reduce('TT_SUSPEND', nTop()) . *PopCounter()
              );
FailStmt    = ( *$'fail'    *$' '         semi_opt *$' '      . *Reduce('TT_PROC_FAIL', 0) );
StmtBody  = ( LocalDecl . *IncCounter()
            | StaticDecl . *IncCounter()
            | InitialStmt . *IncCounter()
            | ReturnStmt . *IncCounter()
            | SuspendStmt . *IncCounter()
            | FailStmt . *IncCounter()
            | *$' ' *Expr *$' ' semi_opt *$' ' . *IncCounter()
            );
ParamFirst = ( *$' ' (id_pat) . thx . *Shift('TT_VAR', thx)  . *IncCounter() );
ParamRest  = ( *$',' (id_pat) . thx . *Shift('TT_VAR', thx)  . *IncCounter() );
Params     = ( ParamFirst ARBNO(ParamRest) (*$'[' *$']' | epsilon) | epsilon );
Prochead   = ( *$'procedure' *$'  ' (id_pat) . thx . *Shift('TT_VAR', thx)  . *IncCounter()
               *$'(' Params *$')' *$' ' semi_opt *$' '
             );
ProcbodyEnd = ( *$'end' *$' ' (*$' ' | RPOS(0)) );
/* a statement, once matched, is never re-matched when a later one refuses: the refusal is linear  */
Procbody    = ( ProcbodyEnd | FENCE(StmtBody) *Procbody );
/* Proc: collect name + params + stmts; reduce to TT_FNC; wrap in :subj then STMT. */
Proc        = ( epsilon . *PushCounter()  Prochead  Procbody
                . *Reduce('TT_FNC', nTop()) . *Reduce(':subj', 1) . *Reduce('STMT', 1)
                . *PopCounter() FLUSH
              );
/* GlobalDecl: collect var names; reduce to TT_GLOBAL; wrap in :subj then STMT. */
GlobalDecl = ( epsilon . *PushCounter() *$'global' *$'  ' DeclIds *$' ' semi_opt *$' '
               . *Reduce('TT_GLOBAL', nTop()) . *Reduce(':subj', 1) . *Reduce('STMT', 1)
               . *PopCounter()
             );
RecordField = ( *$',' (id_pat) . thx . *Shift('TT_VAR', thx) . *IncCounter() );
/* Record: collect name + fields; reduce to TT_RECORD; wrap in :subj then STMT. */
Record      = ( epsilon . *PushCounter()
                *$'record' *$'  ' (id_pat) . thx . *Shift('TT_VAR', thx) . *IncCounter()
                *$'(' ( *$' ' (id_pat) . thx . *Shift('TT_VAR', thx) . *IncCounter() ARBNO(RecordField) | epsilon ) *$')'
                *$' '
                . *Reduce('TT_RECORD', nTop()) . *Reduce(':subj', 1) . *Reduce('STMT', 1)
                . *PopCounter()
              );
/* link a, "b" and invocable all, "+": names as TT_VAR leaves, a quoted name too (the C frontend's shape). */
LinkName    = ( ( *$' ' '"' (BREAK('"')) . thx . *Shift('TT_VAR', thx) '"' | *$' ' (id_pat) . thx . *Shift('TT_VAR', thx) ) FENCE(*$':' SPAN(digits) | epsilon) . *IncCounter() );
LinkStar    = FENCE( *$',' LinkName *LinkStar | epsilon );
LinkDecl    = ( epsilon . *PushCounter() *$'link' *$'  ' LinkName *LinkStar *$' ' semi_opt *$' '
                . *Reduce('TT_LINK', nTop()) . *Reduce(':subj', 1) . *Reduce('STMT', 1)
                . *PopCounter()
              );
InvocableDecl = ( epsilon . *PushCounter() *$'invocable' *$'  ' LinkName *LinkStar *$' ' semi_opt *$' '
                . *Reduce('TT_INVOCABLE', nTop()) . *Reduce(':subj', 1) . *Reduce('STMT', 1)
                . *PopCounter()
              );
TopStar   = FENCE( epsilon . *IncCounter() *$' ' (GlobalDecl | Record | Proc | LinkDecl | InvocableDecl) *$' ' *TopStar | epsilon );
Compiland = ( epsilon . *PushCounter()
              POS(0) *$' ' *TopStar RPOS(0)
              . *Reduce('Parse', nTop())
              . *PopCounter()
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
