/* PST-ICN-SC ✅ 2026-05-19 — Expr11 pure shift/reduce; zero violations. */
/* ==================================================================================================================== */
white        =   (  SPAN(' ' tab nl)
                 |  '#' BREAK(nl) nl
                 );
White        =   white ARBNO(white);
Gray         =   White | epsilon;
$' '         =   Gray;
$'  '        =   White;
Id           = ANY(&UCASE &LCASE '_') FENCE(SPAN(digits &UCASE &LCASE '_') | epsilon);
id_pat       = Id $ tx $ *notmatch(tx, POS(0) ('end' | 'procedure' | 'record' | 'global') RPOS(0));
int_pat      = SPAN(digits);
exp_part     = (('e' | 'E') ('+' | '-' | '') SPAN(digits));
real_pat     = (( SPAN(digits) '.' (SPAN(digits) | '') | '.' SPAN(digits) ) (exp_part | '')
               | SPAN(digits) exp_part
               );
str_pat      = ('"' BREAK('"') . strbody '"');
cset_pat     = ("'" BREAK("'") . csetbody "'");
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
$'('        =   '(' $' ';
$'['        =   '[' $' ';
$'{'        =   $' ' '{' $' ';
$')'        =   $' ' ')';
$']'        =   $' ' ']';
$'}'        =   $' ' '}';
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
/* ==================================================================================================================== */
/* Leaf-push helpers: allowed by PST rules — set v.sval/v.dval from token capture, no child inspection. */
/* ==================================================================================================================== */
If     = ( $'if'     $'  ' *Expr  $'then' $' ' *Expr
           (  $'else' $' ' *Expr  reduce('TT_IF', 3)
           |  reduce('TT_IF', 2)
           )
         );
While  = ( $'while'  $'  ' *Expr  $'do' $' ' *Expr  reduce('TT_WHILE', 2) );
Until  = ( $'until'  $'  ' *Expr  $'do' $' ' *Expr  reduce('TT_UNTIL', 2) );
Every  = ( $'every'  $'  ' *Expr
           (  $'do' $' ' *Expr  reduce('TT_EVERY', 2)
           |  reduce('TT_EVERY', 1)
           )
         );
Repeat = ( $'repeat' $' ' *Expr  reduce('TT_REPEAT', 1) );
ArgFirst  = ( $' ' *Expr  nInc() );
ArgRest   = ( $','  *Expr  nInc() );
CallArgs  = ( ArgFirst ARBNO(ArgRest) | epsilon );
Call      = ( nPush()
              $' ' shift(id_pat, 'TT_VAR')  nInc()
              $'(' CallArgs $')'
              reduce('TT_FNC', 'nTop()')
              nPop()
            );
SeqRest   = ( $';' *Expr  nInc() );
Paren     = ( nPush()
              $' ' $'(' *Expr  nInc()  ARBNO(SeqRest)  $')'
              reduce('TT_SEQ_EXPR', "*(GT(nTop(), 1) nTop())")
              nPop()
            );
CompoundFirst = ( $' ' *Expr $' ' semi_opt $' ' nInc() );
CompoundRest  = ( $' ' *Expr $' ' semi_opt $' ' nInc() );
Compound      = ( nPush()
                  $'{'
                  ( CompoundFirst ARBNO(CompoundRest) | epsilon )
                  $'}'
                  reduce('TT_SEQ_EXPR', "*(GT(nTop(), 1) nTop())")
                  nPop()
                );
ListFirst = ( $' ' *Expr  nInc() );
ListRest  = ( $','  *Expr  nInc() );
ListCtor  = ( nPush()
              $' ' $'['
              ( ListFirst ARBNO(ListRest) | epsilon )
              $']'
              reduce('TT_MAKELIST', 'nTop()')
              nPop()
            );
/* FieldTail: shift field name as TT_VAR (source order: object already on stack below),
   then reduce('TT_FIELD', 2) gives children [object, TT_VAR(name)] in source order. */
FieldTail   = ( $'.' shift(id_pat, 'TT_VAR') reduce('TT_FIELD', 2) );
Expr11tail  = ( $'[' *Expr
                FENCE( $':+' *Expr $']' reduce('TT_SECTION_PLUS',  3)
                     | $':-' *Expr $']' reduce('TT_SECTION_MINUS', 3)
                     | $':'  *Expr $']' reduce('TT_SECTION',       3)
                     | $']'            reduce('TT_IDX',            2)
                     )
              | FieldTail
              );
CaseGray     = ARBNO(white);
CaseClause   = ( *CaseGray *Expr *CaseGray $':' *Expr *CaseGray semi_opt nInc() nInc() );
CaseDefault  = ( *CaseGray $'default' *CaseGray $':' *Expr *CaseGray semi_opt nInc() );
Case         = ( nPush()
                 $'case' $'  ' *Expr  nInc()
                 $'of' *CaseGray $'{' *CaseGray
                 ARBNO( FENCE(CaseDefault | CaseClause) )
                 *CaseGray $'}'
                 reduce('TT_CASE', 'nTop()')
                 nPop()
               );
Expr11 = (   If  |  Until  |  While  |  Every  |  Repeat  |  Case
         |   $'break' $' '  reduce('TT_LOOP_BREAK', 0)
         |   $'next'  $' '  reduce('TT_LOOP_NEXT', 0)
         |   $'fail'  $' '  reduce('TT_PROC_FAIL', 0)
         |   ListCtor
         |   Call  |  Paren  |  Compound
         |   $' ' cset_pat assign(.t_imm, csetbody) shift(t_imm, 'TT_CSET')
         |   $' ' str_pat  assign(.t_imm, strbody)  shift(t_imm, 'TT_QLIT')
         |   $' ' real_pat . rval assign(.t_imm, REAL(rval)) shift(t_imm, 'TT_FLIT')
         |   $' ' shift(int_pat, 'TT_ILIT')
         |   $' ' '&' id_pat . kwname assign(.t_imm, '&' kwname) shift(t_imm, 'TT_VAR')
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
         |   $'not' $'  ' *Expr10 reduce('TT_NOT', 1)
         |   *Expr11  ARBNO(Expr11tail)
         );
Expr9tail = FENCE( $'\\' *Expr10 reduce('TT_LIMIT', 2)
                 | $'!'  *Expr10 reduce('TT_BANG_BINARY', 2)
                 );
Expr9     = ( *Expr10 ARBNO(Expr9tail) );
Expr8     = ( *Expr9 FENCE($'^' *Expr8 reduce('TT_POW', 2) | epsilon) );
Expr7tail = FENCE( $'**' *Expr8 reduce('TT_CSET_INTER', 2)
                 | $'*'  *Expr8 reduce('TT_MUL', 2)
                 | $'/'  *Expr8 reduce('TT_DIV', 2)
                 | $'%'  *Expr8 reduce('TT_MOD', 2)
                 );
Expr7     = ( *Expr8 ARBNO(Expr7tail) );
Expr6tail = FENCE( $'++' *Expr7 reduce('TT_CSET_UNION', 2)
                 | $'--' *Expr7 reduce('TT_CSET_DIFF', 2)
                 | $'+'  *Expr7 reduce('TT_ADD', 2)
                 | $'-'  *Expr7 reduce('TT_SUB', 2)
                 );
Expr6     = ( *Expr7 ARBNO(Expr6tail) );
Expr5tail = FENCE( $'|||' *Expr6 reduce('TT_LCONCAT', 2) | $'||' *Expr6 reduce('TT_CAT', 2) );
Expr5     = ( *Expr6 ARBNO(Expr5tail) );
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
Expr4     = ( *Expr5 ARBNO(Expr4tail) );
X3        = ( nInc() *Expr4 FENCE($'|' *X3 | epsilon) );
Expr3     = ( nPush() X3 reduce('TT_ALTERNATE', "*(GT(nTop(), 1) nTop())") nPop() );
Expr2     = ( *Expr3
              FENCE(  $'to' $'  ' *Expr3
                      FENCE( $'by' $'  ' *Expr3 reduce('TT_TO_BY', 3)
                           | reduce('TT_TO', 2)
                           )
                   |  epsilon
                   )
            );
Expr1     = ( *Expr2
              FENCE(
                  $'<<=:=' *Expr1 reduce('TT_AUGOP', 2)
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
                $'return' $'  ' *Expr1a nInc()  reduce('TT_RETURN', 1) nPop()
              | $'return' $' '                   reduce('TT_RETURN', 0)
              );
SuspendExpr = ( nPush()
                $'suspend' $'  ' *Expr1a nInc()
                FENCE( $'do' $'  ' *Expr1a nInc() | epsilon )
                reduce('TT_SUSPEND', 'nTop()') nPop()
              );
Expr1a    = ( *Expr1 FENCE($'?' *Expr reduce('TT_SCAN', 2) | epsilon) );
ExprSeqRest = ( $'&' *Expr1a nInc() );
Expr        = ( nPush()
                ( ReturnExpr | SuspendExpr
                | *Expr1a
                )
                nInc() ARBNO(ExprSeqRest) reduce('TT_SEQ', "*(GT(nTop(), 1) nTop())") nPop()
              );
Blank     = ( $' ' );
ReturnStmt = ( $'return' $'  ' *Expr $' ' semi_opt $' ' reduce('TT_RETURN', 1)
             | $'return' $' '  semi_opt $' '             reduce('TT_RETURN', 0)
             );
DeclFirst  = ( $' ' shift(id_pat, 'TT_VAR') nInc() );
DeclRest   = ( $','  shift(id_pat, 'TT_VAR') nInc() );
DeclIds    = ( DeclFirst ARBNO(DeclRest) );
/* LocalDecl: collect var names, reduce to TT_LOCAL node, push bare (no STMT wrap). */
LocalDecl  = ( nPush() $'local'  $'  ' DeclIds $' ' semi_opt $' ' reduce('TT_LOCAL',      'nTop()') nPop() );
StaticDecl = ( nPush() $'static' $'  ' DeclIds $' ' semi_opt $' ' reduce('TT_STATIC_DECL', 'nTop()') nPop() );
InitialStmt = ( nPush() $'initial' $' '
                $'{' *Expr nInc() $' ' semi_opt $'}'
                reduce('TT_INITIAL', 'nTop()')
                nPop()
              );
SuspendStmt = ( nPush() $'suspend' $'  ' *Expr nInc()
                FENCE( $'do' $'  ' *Expr nInc() | epsilon )
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
               $'(' Params $')' $' '
             );
ProcbodyEnd = ( $'end' $' ' ($' ' | RPOS(0)) );
Procbody    = ( ProcbodyEnd | StmtBody *Procbody );
/* Proc: collect name + params + stmts; reduce to TT_FNC; wrap in :subj then STMT. */
Proc        = ( nPush()  Prochead  Procbody
                reduce('TT_FNC', 'nTop()') reduce(':subj', 1) reduce('STMT', 1)
                nPop()
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
Compiland = ( nPush()
              POS(0) ARBNO( nInc() $' ' (GlobalDecl | Record | Proc) $' ' ) RPOS(0)
              reduce('Parse', 'nTop()')
              nPop()
            );
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
    } else OUTPUT = 'Parse Error';
} else OUTPUT = 'Parse Error';
