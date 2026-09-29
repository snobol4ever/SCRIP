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
$'and'      =   *$' ' Id $ tx *IDENT(lwr(tx), 'and')       *$' ';
$'begin'    =   *$' ' Id $ tx *IDENT(lwr(tx), 'begin')     *$' ';
$'div'      =   *$' ' Id $ tx *IDENT(lwr(tx), 'div')       *$' ';
$'do'       =   *$' ' Id $ tx *IDENT(lwr(tx), 'do')        *$' ';
$'downto'   =   *$' ' Id $ tx *IDENT(lwr(tx), 'downto')    *$' ';
$'else'     =   *$' ' Id $ tx *IDENT(lwr(tx), 'else')      *$' ';
$'end'      =   *$' ' Id $ tx *IDENT(lwr(tx), 'end')       *$' ';
$'false'    =   *$' ' Id $ tx *IDENT(lwr(tx), 'false')     *$' ';
$'for'      =   *$' ' Id $ tx *IDENT(lwr(tx), 'for')       *$' ';
$'function' =   *$' ' Id $ tx *IDENT(lwr(tx), 'function')  *$' ';
$'if'       =   *$' ' Id $ tx *IDENT(lwr(tx), 'if')        *$' ';
$'mod'      =   *$' ' Id $ tx *IDENT(lwr(tx), 'mod')       *$' ';
$'not'      =   *$' ' Id $ tx *IDENT(lwr(tx), 'not')       *$' ';
$'or'       =   *$' ' Id $ tx *IDENT(lwr(tx), 'or')        *$' ';
$'procedure' =  *$' ' Id $ tx *IDENT(lwr(tx), 'procedure') *$' ';
$'program'  =   *$' ' Id $ tx *IDENT(lwr(tx), 'program')   *$' ';
$'repeat'   =   *$' ' Id $ tx *IDENT(lwr(tx), 'repeat')    *$' ';
$'then'     =   *$' ' Id $ tx *IDENT(lwr(tx), 'then')      *$' ';
$'to'       =   *$' ' Id $ tx *IDENT(lwr(tx), 'to')        *$' ';
$'true'     =   *$' ' Id $ tx *IDENT(lwr(tx), 'true')      *$' ';
$'until'    =   *$' ' Id $ tx *IDENT(lwr(tx), 'until')     *$' ';
$'var'      =   *$' ' Id $ tx *IDENT(lwr(tx), 'var')       *$' ';
$'while'    =   *$' ' Id $ tx *IDENT(lwr(tx), 'while')     *$' ';
$'with'     =   *$' ' Id $ tx *IDENT(lwr(tx), 'with')      *$' ';
$'array'    =   *$' ' Id $ tx *IDENT(lwr(tx), 'array')     *$' ';
$'case'     =   *$' ' Id $ tx *IDENT(lwr(tx), 'case')      *$' ';
$'const'    =   *$' ' Id $ tx *IDENT(lwr(tx), 'const')     *$' ';
$'file'     =   *$' ' Id $ tx *IDENT(lwr(tx), 'file')      *$' ';
$'forward'  =   *$' ' Id $ tx *IDENT(lwr(tx), 'forward')   *$' ';
$'goto'     =   *$' ' Id $ tx *IDENT(lwr(tx), 'goto')      *$' ';
$'in'       =   *$' ' Id $ tx *IDENT(lwr(tx), 'in')        *$' ';
$'label'    =   *$' ' Id $ tx *IDENT(lwr(tx), 'label')     *$' ';
$'nil'      =   *$' ' Id $ tx *IDENT(lwr(tx), 'nil')       *$' ';
$'of'       =   *$' ' Id $ tx *IDENT(lwr(tx), 'of')        *$' ';
$'packed'   =   *$' ' Id $ tx *IDENT(lwr(tx), 'packed')    *$' ';
$'record'   =   *$' ' Id $ tx *IDENT(lwr(tx), 'record')    *$' ';
$'set'      =   *$' ' Id $ tx *IDENT(lwr(tx), 'set')       *$' ';
$'type'     =   *$' ' Id $ tx *IDENT(lwr(tx), 'type')      *$' ';
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
String      =   "'" (*StrChars) . thx . *Shift('TT_QLIT', thx) "'";
Ident       =   Id $ tx $ *notmatch(lwr(tx), reserved) . token;
/* Punctuation.                                                                          */
$'('        =   *$' ' '(' *$' ';
$')'        =   *$' ' ')';
$'['        =   *$' ' '[' *$' ';
$']'        =   *$' ' ']';
$','        =   *$' ' ',' *$' ';
$';'        =   *$' ' ';' *$' ';
$':'        =   *$' ' ':' *$' ';
$'.'        =   *$' ' '.' *$' ';
$'..'       =   *$' ' '..' *$' ';
$'^'        =   *$' ' '^' *$' ';
$'@'        =   *$' ' '@' *$' ';
$':='       =   *$' ' ':=' *$' ';
$'='        =   *$' ' '=' *$' ';
$'<>'       =   *$' ' '<>' *$' ';
$'<='       =   *$' ' '<=' *$' ';
$'>='       =   *$' ' '>=' *$' ';
$'<'        =   *$' ' '<' *$' ';
$'>'        =   *$' ' '>' *$' ';
$'+'        =   *$' ' '+' *$' ';
$'-'        =   *$' ' '-' *$' ';
$'*'        =   *$' ' '*' *$' ';
$'/'        =   *$' ' '/' *$' ';
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
ArgFirst        =   *Expr0 . *IncCounter();
ArgRest         =   *$',' *Expr0 . *IncCounter();
CallArgs        =   *ArgFirst ARBNO(*ArgRest);
/* A write/writeln call lowers to TT_FNC(TT_VAR __pas_write[ln], args…, TT_ILIT -1) --   */
/* the trailing -1 is the oracle's own field-width sentinel, read off its dump.          */
WriteName       =   *$' ' Id $ tx *IDENT(lwr(tx), 'write')   *$' '
                    . *Shift('TT_VAR', '__pas_write') . *IncCounter();
WritelnName     =   *$' ' Id $ tx *IDENT(lwr(tx), 'writeln') *$' '
                    . *Shift('TT_VAR', '__pas_writeln') . *IncCounter();
WriteArg        =   *Expr0 . *IncCounter() FENCE(*$':' *Expr0 . *IncCounter() FENCE(*$':' *Expr0 . *IncCounter() | epsilon) | epsilon);
WriteArgs       =   *WriteArg ARBNO(*$',' *WriteArg);
WriteCall       =   epsilon . *PushCounter() (*WriteName | *WritelnName)
                    FENCE(*$'(' *WriteArgs *$')' | epsilon)
                    . *Shift('TT_ILIT', '-1') . *IncCounter()
                    . *Reduce('TT_FNC', nTop()) . *PopCounter();
/* An ordinary call: the callee is a TT_VAR leaf and is itself one of the children.      */
/* In an expression a call carries its parentheses; a bare identifier there is a TT_VAR.  */
ProcCall        =   epsilon . *PushCounter() (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter()
                    FENCE(*$'(' *CallArgs *$')' | epsilon)
                    . *Reduce('TT_FNC', nTop()) . *PopCounter();
FuncCall        =   epsilon . *PushCounter() (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter()
                    *$'(' *CallArgs *$')'
                    . *Reduce('TT_FNC', nTop()) . *PopCounter();
/* A set constructor [a, b..c] -> TT_FNC(TT_VAR __pas_set, a, TT_TO(b, c)), the callee first.  */
SetMember       =   *Expr0 FENCE(*$'..' *Expr0 . *Reduce('TT_TO', 2) | epsilon) . *IncCounter();
SetTail         =   FENCE(*$',' *SetMember *SetTail | epsilon);
SetCtor         =   epsilon . *PushCounter() *$'[' . *Shift('TT_VAR', '__pas_set') . *IncCounter()
                    FENCE(*SetMember *SetTail | epsilon) *$']'
                    . *Reduce('TT_FNC', nTop()) . *PopCounter();
/* A subscripted variable: a[i] -> TT_IDX(a, i…); the leading nInc counts a itself.        */
IdxTail         =   epsilon . *IncCounter() *$'[' *ArgFirst ARBNO(*ArgRest) *$']';
/* A variable access is a primary and any number of postfixes: a[i], r.f, p^.               */
Postfix         =   ( epsilon . *PushCounter() *IdxTail . *Reduce('TT_IDX', nTop()) . *PopCounter()
                    | *$'.' (*Ident) . thx . *Shift('TT_VAR', thx) . *Reduce('TT_FIELD', 2)
                    | *$'^' . *Reduce('TT_DEREF', 1)
                    );
PostStar        =   FENCE(*Postfix *PostStar | epsilon);
Primary         =   ( *$'(' *Expr0 *$')'
                    | *WriteCall
                    | *$'true'   . *Shift('TT_ILIT', '1') . *Shift('TT_ILIT', '1') . *Reduce('TT_EQ', 2)
                    | *$'false'  . *Shift('TT_ILIT', '0') . *Shift('TT_ILIT', '1') . *Reduce('TT_EQ', 2)
                    | *$'nil'    . *Shift('TT_NULL', 'nil')
                    | epsilon . *PushCounter() '#' . *Shift('TT_VAR', '__pas_chrlit') . *IncCounter() (*Integer) . thx . *Shift('TT_ILIT', thx) . *IncCounter()
                      . *Reduce('TT_FNC', nTop()) . *PopCounter()
                    | *SetCtor
                    | (*Real) . thx . *Shift('TT_FLIT', thx)
                    | (*Integer) . thx . *Shift('TT_ILIT', thx)
                    | *String
                    | *FuncCall
                    | (*Ident) . thx . *Shift('TT_VAR', thx)
                    );
Expr4           =   *Primary *PostStar;
Expr3           =   *$'-'   *Expr3 . *Reduce('TT_MNS', 1)
                |   *$'@'   . *PushCounter() . *Shift('TT_VAR', '__pas_addr') . *IncCounter() *Expr3 . *IncCounter() . *Reduce('TT_FNC', nTop()) . *PopCounter()
                |   *$'+'   *Expr3
                |   *$'not' *Expr3 . *Shift('TT_ILIT', '0') . *Reduce('TT_EQ', 2)
                |   *Expr4;
/* Multiplying operators.  `/` forces a real result the way the oracle does, by folding  */
/* the LEFT operand with a 1.0 before dividing.  `mod` is the named divergence above.    */
MulOp           =   ( *$'*'   *Expr3 . *Reduce('TT_MUL', 2)
                    | *$'/'   . *Shift('TT_FLIT', '1') . *Reduce('TT_MUL', 2)
                             *Expr3 . *Reduce('TT_DIV', 2)
                    | *$'div' *Expr3 . *Reduce('TT_DIV', 2)
                    | *$'mod' *Expr3 . *Reduce('TT_MOD', 2)
                    | *$'and' *Expr3 . *Reduce('TT_MUL', 2)
                    );
Expr2           =   *Expr3 *MulStar;
MulStar         =   FENCE(*MulOp *MulStar | epsilon);
AddOp           =   ( *$'+'  *Expr2 . *Reduce('TT_ADD', 2)
                    | *$'-'  *Expr2 . *Reduce('TT_SUB', 2)
                    | *$'or' *Expr2 . *Reduce('TT_ADD', 2)
                    );
Expr1           =   *Expr2 *AddStar;
AddStar         =   FENCE(*AddOp *AddStar | epsilon);
RelOp           =   ( *$'<>' *Expr1 . *Reduce('TT_NE', 2)
                    | *$'<=' *Expr1 . *Reduce('TT_LE', 2)
                    | *$'>=' *Expr1 . *Reduce('TT_GE', 2)
                    | *$'='  *Expr1 . *Reduce('TT_EQ', 2)
                    | *$'<'  *Expr1 . *Reduce('TT_LT', 2)
                    | *$'>'  *Expr1 . *Reduce('TT_GT', 2)
                    | *$'in' . *Shift('TT_VAR', '__pas_in') *swap *Expr1 . *Reduce('TT_FNC', 3)
                    );
Expr0           =   *Expr1 FENCE(*RelOp | epsilon);
/* ==================================================================================================================== */
/* Statement grammar.                                                                    */
/* ==================================================================================================================== */
/* assignment: v := e  ->  TT_ASSIGN(v, e).  The target may be subscripted.              */
AssignTarget    =   *Primary *PostStar;
assign_cmd      =   *AssignTarget *$':=' *Expr0 . *Reduce('TT_ASSIGN', 2);
/* compound: begin S1; S2; … end -> TT_SEQ_EXPR(S1, S2, …)                               */
StmtFirst       =   *Command . *IncCounter();
StmtRest        =   *$';' (*Command . *IncCounter() | epsilon);
/* the statements of a sequence repeat greedily: a refusal further on fails at once instead of    */
/* backtracking through every earlier statement (46 lines took over 60 s that way)                */
StmtStar        =   FENCE(*StmtRest *StmtStar | epsilon);
compound_cmd    =   epsilon . *PushCounter() *$'begin' (*StmtFirst *StmtStar | epsilon) *$'end'
                    . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter();
/* if C then S [else S] -> TT_IF(C, S[, S])                                              */
if_cmd          =   *$'if' *Expr0 *$'then' *Command
                    FENCE( *$'else' *Command . *Reduce('TT_IF', 3)
                         | epsilon . *Reduce('TT_IF', 2) );
/* while C do S -> TT_WHILE(C, S)                                                        */
while_cmd       =   *$'while' *Expr0 *$'do' *Command . *Reduce('TT_WHILE', 2);
/* repeat S… until C -> TT_REPEAT(S…, C).  The body is a statement sequence.             */
repeat_cmd      =   epsilon . *PushCounter() *$'repeat' (*StmtFirst *StmtStar | epsilon) *$'until'
                    *Expr0 . *IncCounter() . *Reduce('TT_REPEAT', nTop()) . *PopCounter();
/* for v := a to|downto b do S -> TT_FOR(v, a, b, S)                                     */
for_cmd         =   *$'for' (*Ident) . thx . *Shift('TT_VAR', thx) *$':=' *Expr0
                    (*$'to' | *$'downto') *Expr0 *$'do' *Command
                    . *Reduce('TT_FOR', 4);
/* case e of c1, c2: S; … end -> TT_CASE(e, TT_ALT(c1, c2), S, …): an arm's constants are one TT_ALT */
CaseConst       =   *Expr0 FENCE(*$'..' *Expr0 . *Reduce('TT_TO', 2) | epsilon) . *IncCounter();
/* the constants and the arms repeat greedily: an arm, once parsed, is never re-parsed when a later one refuses */
ConstStar       =   FENCE(*$',' *CaseConst *ConstStar | epsilon);
CaseArm         =   epsilon . *PushCounter() *CaseConst *ConstStar . *Reduce('TT_ALT', nTop()) . *PopCounter() . *IncCounter()
                    *$':' *Command . *IncCounter();
ArmStar         =   FENCE(*$';' *CaseArm *ArmStar | epsilon);
case_cmd        =   epsilon . *PushCounter() *$'case' *Expr0 . *IncCounter() *$'of'
                    *CaseArm *ArmStar FENCE(*$';' | epsilon)
                    FENCE(*$'else' *Command . *IncCounter() FENCE(*$';' | epsilon) | epsilon)
                    *$'end' . *Reduce('TT_CASE', nTop()) . *PopCounter();
/* with r1, r2 do S -> TT_FNC(TT_VAR __pas_with, r1, r2, S), the oracle's naming for a lowered form  */
with_cmd        =   epsilon . *PushCounter() *$'with' . *Shift('TT_VAR', '__pas_with') . *IncCounter()
                    *Expr0 . *IncCounter() ARBNO(*$',' *Expr0 . *IncCounter()) *$'do' *Command . *IncCounter()
                    . *Reduce('TT_FNC', nTop()) . *PopCounter();
/* goto 20 -> (TT_GOTO_U 20); 10: S -> TT_LABEL_DEF(TT_ILIT 10, S)                          */
goto_cmd        =   *$'goto' (*Integer) . thx . *Shift('TT_GOTO_U', thx);
label_cmd       =   (*Integer) . thx . *Shift('TT_ILIT', thx) *$':' *Command . *Reduce('TT_LABEL_DEF', 2);
/* an empty statement is legal Pascal wherever a statement may appear: the oracle's TT_SUCCEED */
empty_cmd       =   *$' ' *IDENT(epsilon, epsilon) . *Shift('TT_SUCCEED', '');
Command         =   *$' ' ( *compound_cmd
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
TypeName        =   *$' ' Id *$' ' FENCE(*$'[' BREAK(']') ']' *$' ' | epsilon);
/* A type denoter, ISO 7185 6.4: parsed and discarded like the rest of the declarations.  */
IdList          =   *Ident ARBNO(*$',' *Ident);
SConst          =   FENCE(*$'-' | *$'+' | epsilon) (*Real | *Integer | *Quoted | *Ident);
SimpleType      =   ( *$'(' *IdList *$')'
                    | *SConst *$'..' *SConst
                    | *TypeName
                    );
FixedField      =   *IdList *$':' *TypeSpec;
VariantArm      =   *SConst ARBNO(*$',' *SConst) *$':' *$'(' *FieldList *$')';
VariantTail     =   FENCE(*$';' *VariantArm *VariantTail | epsilon);
VariantPart     =   *$'case' FENCE(*Ident *$':' | epsilon) *Ident *$'of'
                    *VariantArm *VariantTail FENCE(*$';' | epsilon);
FieldList       =   FENCE(*FixedField FENCE(*$';' *FieldList | epsilon) | *VariantPart | epsilon);
TypeSpec        =   FENCE(*$'packed' | epsilon)
                    ( *$'array' *$'[' *SimpleType ARBNO(*$',' *SimpleType) *$']' *$'of' *TypeSpec
                    | *$'record' *FieldList *$'end'
                    | *$'set' *$'of' *SimpleType
                    | *$'file' FENCE(*$'of' *TypeSpec | epsilon)
                    | *$'^' *Ident
                    | *SimpleType
                    );
/* label, const and type parts declare no tree either; a constant is ISO 7185 6.3's.      */
label_part      =   *$'label' *Integer ARBNO(*$',' *Integer) *$';';
ConstDecl       =   *Ident *$'=' *SConst *$';';
ConstDecls      =   FENCE(*ConstDecl *ConstDecls | epsilon);
const_part      =   *$'const' *ConstDecls;
TypeDecl        =   *Ident *$'=' *TypeSpec *$';';
TypeDecls       =   FENCE(*TypeDecl *TypeDecls | epsilon);
type_part       =   *$'type' *TypeDecls;
VarGroup        =   *Ident ARBNO(*$',' *Ident) *$':' *TypeSpec *$';';
VarGroups       =   *VarGroup FENCE(*VarGroups | epsilon);
var_part        =   *$'var' *VarGroups;
/* A parameter list contributes TT_VAR leaves to the procedure's TT_VLIST.                */
ParamFirst      =   ( (*$'procedure' | *$'function') (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter()
                      FENCE(*$'(' BREAK(')') ')' *$' ' | epsilon) FENCE(*$':' *TypeName | epsilon)
                    | FENCE(*$'var' | epsilon) (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter()
                      ARBNO(*$',' (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter()) *$':' *TypeName );
ParamRest       =   *$';' *ParamFirst;
Params          =   epsilon . *PushCounter() FENCE(*$'(' *ParamFirst ARBNO(*ParamRest) *$')' | epsilon)
                    . *Reduce('TT_VLIST', nTop()) . *PopCounter();
/* procedure/function P(params); <decls> begin … end;  or  ...; forward;                  */
/*   -> TT_PROC_DECL(TT_VAR P, TT_VLIST(params), <nested TT_PROC_DECL…>, TT_PROGRAM(body), TT_VLIST()) */
SubBody         =   epsilon . *PushCounter() *$'begin' (*StmtFirst *StmtStar | epsilon) *$'end'
                    . *Reduce('TT_PROGRAM', nTop()) . *PopCounter();
proc_decl       =   epsilon . *PushCounter() (*$'procedure' | *$'function') (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter()
                    *Params . *IncCounter() FENCE(*$':' *TypeName | epsilon) *$';'
                    ( *$'forward' *$';' . *PushCounter() . *Reduce('TT_PROGRAM', nTop()) . *PopCounter() . *IncCounter()
                    | *Decls *SubBody . *IncCounter() *$';' )
                    . *PushCounter() . *Reduce('TT_VLIST', nTop()) . *PopCounter() . *IncCounter()
                    . *Reduce('TT_PROC_DECL', nTop()) . *PopCounter();
/* A block's declaration parts in ISO 7185 6.2.1's order: label, const, type, var, then the   */
/* procedures and functions; the C frontend refuses any other order, and so does this.          */
ProcDecls       =   FENCE(*proc_decl . *IncCounter() *ProcDecls | epsilon);
Decls           =   FENCE(*label_part | epsilon) FENCE(*const_part | epsilon) FENCE(*type_part | epsilon)
                    FENCE(*var_part | epsilon) *ProcDecls;
/* ==================================================================================================================== */
/* Compiland — program header, declarations, main block.  The main block is emitted as    */
/* a TT_PROC_DECL named `main`, which is the shape the C frontend's dump carries.         */
/* ==================================================================================================================== */
MainBody        =   epsilon . *PushCounter() *$'begin' (*StmtFirst *StmtStar | epsilon) *$'end'
                    . *Reduce('TT_PROGRAM', nTop()) . *PopCounter();
program_head    =   FENCE(*$'program' *Ident FENCE(*$'(' BREAK(')') ')' | epsilon) *$';' | epsilon);
main_decl       =   epsilon . *Shift('TT_VAR', 'main')
                    . *PushCounter() . *Reduce('TT_VLIST', nTop()) . *PopCounter()
                    *MainBody
                    . *PushCounter() . *Reduce('TT_VLIST', nTop()) . *PopCounter()
                    . *Reduce('TT_PROC_DECL', 4);
Compiland       =   epsilon . *PushCounter() POS(0) *$' ' *program_head
                    *Decls
                    *main_decl . *IncCounter()
                    *$' ' FENCE(*$'.' | epsilon) *$' ' RPOS(0)
                    . *Reduce('Parse', nTop()) . *PopCounter();
/* ==================================================================================================================== */
/* Driver — byte-identical in shape to the other six parsers.                             */
/* ==================================================================================================================== */
function ParseOne(ptree, i, n_kids) {
    pf_a = TIME();
    InitCounter();
    InitStack();
    if (Src ? Compiland) {
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
