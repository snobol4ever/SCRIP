/* parser_pascal.sc — the seventh self-hosted parser: ISO 7185 Pascal in Snocone.
   Written 2026-09-16 by hq_snocone on Lon's order (CEO-770 row: all seven parser_*.sc
   parse their language's corpus into a proper tree_t with proper TT_* types).
   Shapes are NOT invented: every node kind and arity below was read off the C
   frontend's own `scrip --dump-ast` for a Pascal program, which Lon named as the
   comparison oracle for this row.
   ⛔ ONE NAMED DIVERGENCE FROM THAT ORACLE, and it is a property of the ORACLE, not a
   gap in this file: for Pascal the C frontend's --dump-ast is a LOWERED tree, not a
   pure parse tree.  It rewrites `a mod b` into TT_MOD(TT_ADD(TT_MOD(a,b),b),b) to force
   the non-negative Pascal remainder.  That rewrite DUPLICATES its right operand three
   times, and this library has no subtree-copy primitive (tree.sc offers Append,
   Prepend, Insert, Remove, Tree, Equal, Equiv, Find, Visit — no Copy), so it is not
   expressible in the shift/reduce idiom every other parser here is written in.  This
   file therefore emits the FAITHFUL TT_MOD(a,b) and a tree-diff against the C frontend
   will show exactly that one shape.  The other rewrites ARE reproduced below because
   they are fixed-arity and need no duplication.
   The rewrites reproduced, each read off the oracle:
     a and b   -> TT_MUL(a, b)              a or b  -> TT_ADD(a, b)
     not a     -> TT_EQ(a, TT_ILIT 0)       true    -> TT_EQ(TT_ILIT 1, TT_ILIT 1)
     a / b     -> TT_DIV(TT_MUL(a, TT_FLIT 1), b)   false -> TT_EQ(TT_ILIT 0, TT_ILIT 1)
     +a        -> a  (unary plus is elided by the oracle, no node)
   Runtime chain (same as every other parser_*.sc): global, case, assign, match, counter,
   stack, tree, ShiftReduce, tdump, gen, qize, semantic, omega, trace.
   ⛔ Pascal identifiers and keywords are CASE-INSENSITIVE, unlike every other language
   in this directory, so every keyword matcher folds through lwr() from case.sc. */
&FULLSCAN = 1;
reserved          = POS(0) ( 'and' | 'array' | 'begin' | 'case' | 'const' | 'div'
                           | 'do' | 'downto' | 'else' | 'end' | 'file' | 'for'
                           | 'function' | 'goto' | 'if' | 'in' | 'label' | 'mod'
                           | 'nil' | 'not' | 'of' | 'or' | 'packed' | 'procedure'
                           | 'program' | 'record' | 'repeat' | 'set' | 'then'
                           | 'to' | 'type' | 'until' | 'var' | 'while' | 'with' ) RPOS(0);
/* ==================================================================================================================== */
/* Lexical layer.  Pascal has TWO comment forms and neither nests; a CR is blank (CRLF files). */
&ALPHABET ? (POS(13) LEN(1) . cr);
white       =   (  SPAN(' ' tab nl cr)
                |  '{' BREAK('}') '}'
                |  '(*' FENCE(BREAKX('*') '*)')
                );
White       =   white FENCE(*White | epsilon);
Gray        =   White | epsilon;
$'  '       =   White;
$' '        =   Gray;
Id          =   ANY(&UCASE &LCASE '_') FENCE(SPAN(digits &UCASE '_' &LCASE) | epsilon);
$'and'      =   $' ' Id $ tx *IDENT(lwr(tx), 'and')       $' ';
$'begin'    =   $' ' Id $ tx *IDENT(lwr(tx), 'begin')     $' ';
$'div'      =   $' ' Id $ tx *IDENT(lwr(tx), 'div')       $' ';
$'do'       =   $' ' Id $ tx *IDENT(lwr(tx), 'do')        $' ';
$'downto'   =   $' ' Id $ tx *IDENT(lwr(tx), 'downto')    $' ';
$'else'     =   $' ' Id $ tx *IDENT(lwr(tx), 'else')      $' ';
$'end'      =   $' ' Id $ tx *IDENT(lwr(tx), 'end')       $' ';
$'false'    =   $' ' Id $ tx *IDENT(lwr(tx), 'false')     $' ';
$'for'      =   $' ' Id $ tx *IDENT(lwr(tx), 'for')       $' ';
$'function' =   $' ' Id $ tx *IDENT(lwr(tx), 'function')  $' ';
$'if'       =   $' ' Id $ tx *IDENT(lwr(tx), 'if')        $' ';
$'mod'      =   $' ' Id $ tx *IDENT(lwr(tx), 'mod')       $' ';
$'not'      =   $' ' Id $ tx *IDENT(lwr(tx), 'not')       $' ';
$'or'       =   $' ' Id $ tx *IDENT(lwr(tx), 'or')        $' ';
$'procedure' =  $' ' Id $ tx *IDENT(lwr(tx), 'procedure') $' ';
$'program'  =   $' ' Id $ tx *IDENT(lwr(tx), 'program')   $' ';
$'repeat'   =   $' ' Id $ tx *IDENT(lwr(tx), 'repeat')    $' ';
$'then'     =   $' ' Id $ tx *IDENT(lwr(tx), 'then')      $' ';
$'to'       =   $' ' Id $ tx *IDENT(lwr(tx), 'to')        $' ';
$'true'     =   $' ' Id $ tx *IDENT(lwr(tx), 'true')      $' ';
$'until'    =   $' ' Id $ tx *IDENT(lwr(tx), 'until')     $' ';
$'var'      =   $' ' Id $ tx *IDENT(lwr(tx), 'var')       $' ';
$'while'    =   $' ' Id $ tx *IDENT(lwr(tx), 'while')     $' ';
$'with'     =   $' ' Id $ tx *IDENT(lwr(tx), 'with')      $' ';
$'array'    =   $' ' Id $ tx *IDENT(lwr(tx), 'array')     $' ';
$'case'     =   $' ' Id $ tx *IDENT(lwr(tx), 'case')      $' ';
$'const'    =   $' ' Id $ tx *IDENT(lwr(tx), 'const')     $' ';
$'file'     =   $' ' Id $ tx *IDENT(lwr(tx), 'file')      $' ';
$'forward'  =   $' ' Id $ tx *IDENT(lwr(tx), 'forward')   $' ';
$'goto'     =   $' ' Id $ tx *IDENT(lwr(tx), 'goto')      $' ';
$'in'       =   $' ' Id $ tx *IDENT(lwr(tx), 'in')        $' ';
$'label'    =   $' ' Id $ tx *IDENT(lwr(tx), 'label')     $' ';
$'nil'      =   $' ' Id $ tx *IDENT(lwr(tx), 'nil')       $' ';
$'of'       =   $' ' Id $ tx *IDENT(lwr(tx), 'of')        $' ';
$'packed'   =   $' ' Id $ tx *IDENT(lwr(tx), 'packed')    $' ';
$'record'   =   $' ' Id $ tx *IDENT(lwr(tx), 'record')    $' ';
$'set'      =   $' ' Id $ tx *IDENT(lwr(tx), 'set')       $' ';
$'type'     =   $' ' Id $ tx *IDENT(lwr(tx), 'type')      $' ';
/* Literals.  A Pascal string is single-quoted and doubles an embedded quote.            */
Integer     =   ('$' SPAN(digits 'abcdefABCDEF') | SPAN(digits)) . token;
Real        =   ( SPAN(digits)
                  FENCE(
                    '.'
                    SPAN(digits)
                    FENCE(ANY('eE') FENCE(ANY('+-') | epsilon) SPAN(digits) | epsilon)
                  | ANY('eE')
                    FENCE(ANY('+-') | epsilon)
                    SPAN(digits)
                  )
                ) . token;
/* a string's characters, a doubled quote among them, are taken greedily: shortest-first, '''' read as the  */
/* empty string first, and inside a FENCE'd operand (x <> '''') that choice was never retried              */
StrChars    =   FENCE((NOTANY("'") | "''") *StrChars | epsilon);
Quoted      =   "'" *StrChars "'";
String      =   "'" shift(*StrChars, "'TT_QLIT'") "'";
Ident       =   Id $ tx $ *notmatch(lwr(tx), reserved) . token;
/* Punctuation.                                                                          */
$'('        =   $' ' '(' $' ';
$')'        =   $' ' ')';
$'['        =   $' ' '[' $' ';
$']'        =   $' ' ']';
$','        =   $' ' ',' $' ';
$';'        =   $' ' ';' $' ';
$':'        =   $' ' ':' $' ';
$'.'        =   $' ' '.' $' ';
$'..'       =   $' ' '..' $' ';
$'^'        =   $' ' '^' $' ';
$'@'        =   $' ' '@' $' ';
$':='       =   $' ' ':=' $' ';
$'='        =   $' ' '=' $' ';
$'<>'       =   $' ' '<>' $' ';
$'<='       =   $' ' '<=' $' ';
$'>='       =   $' ' '>=' $' ';
$'<'        =   $' ' '<' $' ';
$'>'        =   $' ' '>' $' ';
$'+'        =   $' ' '+' $' ';
$'-'        =   $' ' '-' $' ';
$'*'        =   $' ' '*' $' ';
$'/'        =   $' ' '/' $' ';
/* ==================================================================================================================== */
/* Expression grammar.  Pascal precedence, lowest first:                                 */
/*   relational (= <> < <= > >=) · adding (+ - or) · multiplying (* / div mod and)        */
/*   · unary (+ - not) · primary                                                          */
/* ==================================================================================================================== */
/* Swap exchanges the two top stack nodes: `x in s` must come out as the oracle's                */
/* TT_FNC(TT_VAR __pas_in, x, s), callee first, and the callee is only known after x.           */
function Swap(a, b) {
    a = Pop();
    b = Pop();
    Push(a);
    Push(b);
    Swap = .dummy;
    nreturn;
}
swap            =   epsilon . *Swap();
ArgFirst        =   *Expr0 nInc();
ArgRest         =   $',' *Expr0 nInc();
CallArgs        =   *ArgFirst ARBNO(*ArgRest);
/* A write/writeln call lowers to TT_FNC(TT_VAR __pas_write[ln], args…, TT_ILIT -1) --   */
/* the trailing -1 is the oracle's own field-width sentinel, read off its dump.          */
WriteName       =   $' ' Id $ tx *IDENT(lwr(tx), 'write')   $' '
                    shift_value('__pas_write', "'TT_VAR'") nInc();
WritelnName     =   $' ' Id $ tx *IDENT(lwr(tx), 'writeln') $' '
                    shift_value('__pas_writeln', "'TT_VAR'") nInc();
WriteArg        =   *Expr0 nInc() FENCE($':' *Expr0 nInc() FENCE($':' *Expr0 nInc() | epsilon) | epsilon);
WriteArgs       =   *WriteArg ARBNO($',' *WriteArg);
WriteCall       =   nPush() (*WriteName | *WritelnName)
                    FENCE($'(' *WriteArgs $')' | epsilon)
                    shift_value('-1', "'TT_ILIT'") nInc()
                    reduce("'TT_FNC'", 'nTop()') nPop();
/* An ordinary call: the callee is a TT_VAR leaf and is itself one of the children.      */
/* In an expression a call carries its parentheses; a bare identifier there is a TT_VAR.  */
ProcCall        =   nPush() shift(*Ident, "'TT_VAR'") nInc()
                    FENCE($'(' *CallArgs $')' | epsilon)
                    reduce("'TT_FNC'", 'nTop()') nPop();
FuncCall        =   nPush() shift(*Ident, "'TT_VAR'") nInc()
                    $'(' *CallArgs $')'
                    reduce("'TT_FNC'", 'nTop()') nPop();
/* A set constructor [a, b..c] -> TT_FNC(TT_VAR __pas_set, a, TT_TO(b, c)), the callee first.  */
SetMember       =   *Expr0 FENCE($'..' *Expr0 reduce("'TT_TO'", 2) | epsilon) nInc();
SetTail         =   FENCE($',' *SetMember *SetTail | epsilon);
SetCtor         =   nPush() $'[' shift_value('__pas_set', "'TT_VAR'") nInc()
                    FENCE(*SetMember *SetTail | epsilon) $']'
                    reduce("'TT_FNC'", 'nTop()') nPop();
/* A subscripted variable: a[i] -> TT_IDX(a, i…); the leading nInc counts a itself.        */
IdxTail         =   nInc() $'[' *ArgFirst ARBNO(*ArgRest) $']';
/* A variable access is a primary and any number of postfixes: a[i], r.f, p^.               */
Postfix         =   ( nPush() *IdxTail reduce("'TT_IDX'", 'nTop()') nPop()
                    | $'.' shift(*Ident, "'TT_VAR'") reduce("'TT_FIELD'", 2)
                    | $'^' reduce("'TT_DEREF'", 1)
                    );
PostStar        =   FENCE(*Postfix *PostStar | epsilon);
Primary         =   ( $'(' *Expr0 $')'
                    | *WriteCall
                    | $'true'   shift_value('1', "'TT_ILIT'") shift_value('1', "'TT_ILIT'") reduce("'TT_EQ'", 2)
                    | $'false'  shift_value('0', "'TT_ILIT'") shift_value('1', "'TT_ILIT'") reduce("'TT_EQ'", 2)
                    | $'nil'    shift_value('nil', "'TT_NULL'")
                    | nPush() '#' shift_value('__pas_chrlit', "'TT_VAR'") nInc() shift(*Integer, "'TT_ILIT'") nInc()
                      reduce("'TT_FNC'", 'nTop()') nPop()
                    | *SetCtor
                    | shift(*Real, "'TT_FLIT'")
                    | shift(*Integer, "'TT_ILIT'")
                    | *String
                    | *FuncCall
                    | shift(*Ident, "'TT_VAR'")
                    );
Expr4           =   *Primary *PostStar;
Expr3           =   $'-'   *Expr3 reduce("'TT_MNS'", 1)
                |   $'@'   nPush() shift_value('__pas_addr', "'TT_VAR'") nInc() *Expr3 nInc() reduce("'TT_FNC'", 'nTop()') nPop()
                |   $'+'   *Expr3
                |   $'not' *Expr3 shift_value('0', "'TT_ILIT'") reduce("'TT_EQ'", 2)
                |   *Expr4;
/* Multiplying operators.  `/` forces a real result the way the oracle does, by folding  */
/* the LEFT operand with a 1.0 before dividing.  `mod` is the named divergence above.    */
MulOp           =   ( $'*'   *Expr3 reduce("'TT_MUL'", 2)
                    | $'/'   shift_value('1', "'TT_FLIT'") reduce("'TT_MUL'", 2)
                             *Expr3 reduce("'TT_DIV'", 2)
                    | $'div' *Expr3 reduce("'TT_DIV'", 2)
                    | $'mod' *Expr3 reduce("'TT_MOD'", 2)
                    | $'and' *Expr3 reduce("'TT_MUL'", 2)
                    );
Expr2           =   *Expr3 *MulStar;
MulStar         =   FENCE(*MulOp *MulStar | epsilon);
AddOp           =   ( $'+'  *Expr2 reduce("'TT_ADD'", 2)
                    | $'-'  *Expr2 reduce("'TT_SUB'", 2)
                    | $'or' *Expr2 reduce("'TT_ADD'", 2)
                    );
Expr1           =   *Expr2 *AddStar;
AddStar         =   FENCE(*AddOp *AddStar | epsilon);
RelOp           =   ( $'<>' *Expr1 reduce("'TT_NE'", 2)
                    | $'<=' *Expr1 reduce("'TT_LE'", 2)
                    | $'>=' *Expr1 reduce("'TT_GE'", 2)
                    | $'='  *Expr1 reduce("'TT_EQ'", 2)
                    | $'<'  *Expr1 reduce("'TT_LT'", 2)
                    | $'>'  *Expr1 reduce("'TT_GT'", 2)
                    | $'in' shift_value('__pas_in', "'TT_VAR'") *swap *Expr1 reduce("'TT_FNC'", 3)
                    );
Expr0           =   *Expr1 FENCE(*RelOp | epsilon);
/* ==================================================================================================================== */
/* Statement grammar.                                                                    */
/* ==================================================================================================================== */
/* assignment: v := e  ->  TT_ASSIGN(v, e).  The target may be subscripted.              */
AssignTarget    =   *Primary *PostStar;
assign_cmd      =   *AssignTarget $':=' *Expr0 reduce("'TT_ASSIGN'", 2);
/* compound: begin S1; S2; … end -> TT_SEQ_EXPR(S1, S2, …)                               */
StmtFirst       =   *Command nInc();
StmtRest        =   $';' (*Command nInc() | epsilon);
/* the statements of a sequence repeat greedily: a refusal further on fails at once instead of    */
/* backtracking through every earlier statement (46 lines took over 60 s that way)                */
StmtStar        =   FENCE(*StmtRest *StmtStar | epsilon);
compound_cmd    =   nPush() $'begin' (*StmtFirst *StmtStar | epsilon) $'end'
                    reduce("'TT_SEQ_EXPR'", 'nTop()') nPop();
/* if C then S [else S] -> TT_IF(C, S[, S])                                              */
if_cmd          =   $'if' *Expr0 $'then' *Command
                    FENCE( $'else' *Command reduce("'TT_IF'", 3)
                         | reduce("'TT_IF'", 2) );
/* while C do S -> TT_WHILE(C, S)                                                        */
while_cmd       =   $'while' *Expr0 $'do' *Command reduce("'TT_WHILE'", 2);
/* repeat S… until C -> TT_REPEAT(S…, C).  The body is a statement sequence.             */
repeat_cmd      =   nPush() $'repeat' (*StmtFirst *StmtStar | epsilon) $'until'
                    *Expr0 nInc() reduce("'TT_REPEAT'", 'nTop()') nPop();
/* for v := a to|downto b do S -> TT_FOR(v, a, b, S)                                     */
for_cmd         =   $'for' shift(*Ident, "'TT_VAR'") $':=' *Expr0
                    ($'to' | $'downto') *Expr0 $'do' *Command
                    reduce("'TT_FOR'", 4);
/* case e of c1, c2: S; … end -> TT_CASE(e, TT_ALT(c1, c2), S, …): an arm's constants are one TT_ALT */
CaseConst       =   *Expr0 FENCE($'..' *Expr0 reduce("'TT_TO'", 2) | epsilon) nInc();
/* the constants and the arms repeat greedily: an arm, once parsed, is never re-parsed when a later one refuses */
ConstStar       =   FENCE($',' *CaseConst *ConstStar | epsilon);
CaseArm         =   nPush() *CaseConst *ConstStar reduce("'TT_ALT'", 'nTop()') nPop() nInc()
                    $':' *Command nInc();
ArmStar         =   FENCE($';' *CaseArm *ArmStar | epsilon);
case_cmd        =   nPush() $'case' *Expr0 nInc() $'of'
                    *CaseArm *ArmStar FENCE($';' | epsilon)
                    FENCE($'else' *Command nInc() FENCE($';' | epsilon) | epsilon)
                    $'end' reduce("'TT_CASE'", 'nTop()') nPop();
/* with r1, r2 do S -> TT_FNC(TT_VAR __pas_with, r1, r2, S), the oracle's naming for a lowered form  */
with_cmd        =   nPush() $'with' shift_value('__pas_with', "'TT_VAR'") nInc()
                    *Expr0 nInc() ARBNO($',' *Expr0 nInc()) $'do' *Command nInc()
                    reduce("'TT_FNC'", 'nTop()') nPop();
/* goto 20 -> (TT_GOTO_U 20); 10: S -> TT_LABEL_DEF(TT_ILIT 10, S)                          */
goto_cmd        =   $'goto' shift(*Integer, "'TT_GOTO_U'");
label_cmd       =   shift(*Integer, "'TT_ILIT'") $':' *Command reduce("'TT_LABEL_DEF'", 2);
/* an empty statement is legal Pascal wherever a statement may appear: the oracle's TT_SUCCEED */
empty_cmd       =   $' ' *IDENT(epsilon, epsilon) shift_value('', "'TT_SUCCEED'");
Command         =   $' ' ( *compound_cmd
                    | *if_cmd
                    | *while_cmd
                    | *repeat_cmd
                    | *for_cmd
                    | *case_cmd
                    | *with_cmd
                    | *goto_cmd
                    | *label_cmd
                    | *assign_cmd
                    | *WriteCall
                    | *ProcCall
                    | *empty_cmd
                    );
/* ==================================================================================================================== */
/* Declarations.  var/const/type declare no tree of their own -- the oracle's dump shows  */
/* them absorbed, so they are PARSED AND DISCARDED here rather than silently skipped.     */
/* ==================================================================================================================== */
TypeName        =   $' ' Id $' ' FENCE($'[' BREAK(']') ']' $' ' | epsilon);
/* A type denoter, ISO 7185 6.4: parsed and discarded like the rest of the declarations.  */
IdList          =   *Ident ARBNO($',' *Ident);
SConst          =   FENCE($'-' | $'+' | epsilon) (*Real | *Integer | *Quoted | *Ident);
SimpleType      =   ( $'(' *IdList $')'
                    | *SConst $'..' *SConst
                    | *TypeName
                    );
FixedField      =   *IdList $':' *TypeSpec;
VariantArm      =   *SConst ARBNO($',' *SConst) $':' $'(' *FieldList $')';
VariantTail     =   FENCE($';' *VariantArm *VariantTail | epsilon);
VariantPart     =   $'case' FENCE(*Ident $':' | epsilon) *Ident $'of'
                    *VariantArm *VariantTail FENCE($';' | epsilon);
FieldList       =   FENCE(*FixedField FENCE($';' *FieldList | epsilon) | *VariantPart | epsilon);
TypeSpec        =   FENCE($'packed' | epsilon)
                    ( $'array' $'[' *SimpleType ARBNO($',' *SimpleType) $']' $'of' *TypeSpec
                    | $'record' *FieldList $'end'
                    | $'set' $'of' *SimpleType
                    | $'file' FENCE($'of' *TypeSpec | epsilon)
                    | $'^' *Ident
                    | *SimpleType
                    );
/* label, const and type parts declare no tree either; a constant is ISO 7185 6.3's.      */
label_part      =   $'label' *Integer ARBNO($',' *Integer) $';';
ConstDecl       =   *Ident $'=' *SConst $';';
ConstDecls      =   FENCE(*ConstDecl *ConstDecls | epsilon);
const_part      =   $'const' *ConstDecls;
TypeDecl        =   *Ident $'=' *TypeSpec $';';
TypeDecls       =   FENCE(*TypeDecl *TypeDecls | epsilon);
type_part       =   $'type' *TypeDecls;
VarGroup        =   *Ident ARBNO($',' *Ident) $':' *TypeSpec $';';
VarGroups       =   *VarGroup FENCE(*VarGroups | epsilon);
var_part        =   $'var' *VarGroups;
/* A parameter list contributes TT_VAR leaves to the procedure's TT_VLIST.                */
ParamFirst      =   ( ($'procedure' | $'function') shift(*Ident, "'TT_VAR'") nInc()
                      FENCE($'(' BREAK(')') ')' $' ' | epsilon) FENCE($':' *TypeName | epsilon)
                    | FENCE($'var' | epsilon) shift(*Ident, "'TT_VAR'") nInc()
                      ARBNO($',' shift(*Ident, "'TT_VAR'") nInc()) $':' *TypeName );
ParamRest       =   $';' *ParamFirst;
Params          =   nPush() FENCE($'(' *ParamFirst ARBNO(*ParamRest) $')' | epsilon)
                    reduce("'TT_VLIST'", 'nTop()') nPop();
/* procedure/function P(params); <decls> begin … end;  or  ...; forward;                  */
/*   -> TT_PROC_DECL(TT_VAR P, TT_VLIST(params), <nested TT_PROC_DECL…>, TT_PROGRAM(body), TT_VLIST()) */
SubBody         =   nPush() $'begin' (*StmtFirst *StmtStar | epsilon) $'end'
                    reduce("'TT_PROGRAM'", 'nTop()') nPop();
proc_decl       =   nPush() ($'procedure' | $'function') shift(*Ident, "'TT_VAR'") nInc()
                    *Params nInc() FENCE($':' *TypeName | epsilon) $';'
                    ( $'forward' $';' nPush() reduce("'TT_PROGRAM'", 'nTop()') nPop() nInc()
                    | *Decls *SubBody nInc() $';' )
                    nPush() reduce("'TT_VLIST'", 'nTop()') nPop() nInc()
                    reduce("'TT_PROC_DECL'", 'nTop()') nPop();
/* A block's declaration parts in ISO 7185 6.2.1's order: label, const, type, var, then the   */
/* procedures and functions; the C frontend refuses any other order, and so does this.          */
ProcDecls       =   FENCE(*proc_decl nInc() *ProcDecls | epsilon);
Decls           =   FENCE(*label_part | epsilon) FENCE(*const_part | epsilon) FENCE(*type_part | epsilon)
                    FENCE(*var_part | epsilon) *ProcDecls;
/* ==================================================================================================================== */
/* Compiland — program header, declarations, main block.  The main block is emitted as    */
/* a TT_PROC_DECL named `main`, which is the shape the C frontend's dump carries.         */
/* ==================================================================================================================== */
MainBody        =   nPush() $'begin' (*StmtFirst *StmtStar | epsilon) $'end'
                    reduce("'TT_PROGRAM'", 'nTop()') nPop();
program_head    =   FENCE($'program' *Ident FENCE($'(' BREAK(')') ')' | epsilon) $';' | epsilon);
main_decl       =   shift_value('main', "'TT_VAR'")
                    nPush() reduce("'TT_VLIST'", 'nTop()') nPop()
                    *MainBody
                    nPush() reduce("'TT_VLIST'", 'nTop()') nPop()
                    reduce("'TT_PROC_DECL'", 4);
Compiland       =   nPush() POS(0) $' ' *program_head
                    *Decls
                    *main_decl nInc()
                    $' ' FENCE($'.' | epsilon) $' ' RPOS(0)
                    reduce("'Parse'", 'nTop()') nPop();
/* ==================================================================================================================== */
/* Driver — byte-identical in shape to the other six parsers.                             */
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
            TreeDump(c(ptree)[i]);
            i = i + 1;
        }
    }
} else OUTPUT = 'Parse Error';
