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
white       =   (  SPAN(' ' CHAR(9) CHAR(10) CHAR(13))
                |  '{' BREAK('}') '}'
                |  '(*' FENCE(BREAKX('*') '*)')
                );
White       =   *white FENCE(*White | epsilon);
Gray        =   FENCE(*White | epsilon);
$'  '       =   White;
$' '        =   Gray;
Id          =   ANY(&UCASE &LCASE '_') FENCE(SPAN('0123456789' &UCASE '_' &LCASE) | epsilon);
$'and'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'and')       *$' ';
$'begin'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'begin')     *$' ';
$'div'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'div')       *$' ';
$'do'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'do')        *$' ';
$'downto'   =   *$' ' *Id $ tx *IDENT(lwr(tx), 'downto')    *$' ';
$'else'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'else')      *$' ';
$'end'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'end')       *$' ';
$'false'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'false')     *$' ';
$'for'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'for')       *$' ';
$'function' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'function')  *$' ';
$'if'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'if')        *$' ';
$'mod'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'mod')       *$' ';
$'not'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'not')       *$' ';
$'or'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'or')        *$' ';
$'procedure' =  *$' ' *Id $ tx *IDENT(lwr(tx), 'procedure') *$' ';
$'program'  =   *$' ' *Id $ tx *IDENT(lwr(tx), 'program')   *$' ';
$'repeat'   =   *$' ' *Id $ tx *IDENT(lwr(tx), 'repeat')    *$' ';
$'then'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'then')      *$' ';
$'to'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'to')        *$' ';
$'true'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'true')      *$' ';
$'until'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'until')     *$' ';
$'var'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'var')       *$' ';
$'while'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'while')     *$' ';
$'with'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'with')      *$' ';
$'array'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'array')     *$' ';
$'case'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'case')      *$' ';
$'const'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'const')     *$' ';
$'file'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'file')      *$' ';
$'forward'  =   *$' ' *Id $ tx *IDENT(lwr(tx), 'forward')   *$' ';
$'goto'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'goto')      *$' ';
$'in'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'in')        *$' ';
$'label'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'label')     *$' ';
$'nil'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'nil')       *$' ';
$'of'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'of')        *$' ';
$'packed'   =   *$' ' *Id $ tx *IDENT(lwr(tx), 'packed')    *$' ';
$'record'   =   *$' ' *Id $ tx *IDENT(lwr(tx), 'record')    *$' ';
$'set'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'set')       *$' ';
$'type'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'type')      *$' ';
/* Literals.  A Pascal string is single-quoted and doubles an embedded quote.            */
Integer     =   ('$' SPAN('0123456789' 'abcdefABCDEF') | SPAN('0123456789')) . token;
Real        =   ( SPAN('0123456789')
                  FENCE(
                    '.'
                    SPAN('0123456789')
                    FENCE(ANY('eE') FENCE(ANY('+-') | epsilon) SPAN('0123456789') | epsilon)
                  | ANY('eE')
                    FENCE(ANY('+-') | epsilon)
                    SPAN('0123456789')
                  )
                ) . token;
/* a string's characters, a doubled quote among them, are taken greedily: shortest-first, '''' read as the  */
/* empty string first, and inside a FENCE'd operand (x <> '''') that choice was never retried              */
StrChars    =   FENCE((NOTANY("'") | "''") *StrChars | epsilon);
Quoted      =   "'" *StrChars "'";
String      =   "'" (*StrChars) . thx . *Shift('TT_QLIT', thx) "'";
ResT        =   TABLE(35);
ResT['and'] = 1; ResT['array'] = 1; ResT['begin'] = 1; ResT['case'] = 1; ResT['const'] = 1; ResT['div'] = 1; ResT['do'] = 1; ResT['downto'] = 1;
ResT['else'] = 1; ResT['end'] = 1; ResT['file'] = 1; ResT['for'] = 1; ResT['function'] = 1; ResT['goto'] = 1; ResT['if'] = 1; ResT['in'] = 1;
ResT['label'] = 1; ResT['mod'] = 1; ResT['nil'] = 1; ResT['not'] = 1; ResT['of'] = 1; ResT['or'] = 1; ResT['packed'] = 1; ResT['procedure'] = 1;
ResT['program'] = 1; ResT['record'] = 1; ResT['repeat'] = 1; ResT['set'] = 1; ResT['then'] = 1; ResT['to'] = 1; ResT['type'] = 1; ResT['until'] = 1;
ResT['var'] = 1; ResT['while'] = 1; ResT['with'] = 1;
Ident       =   *Id $ tx *IDENT(ResT[lwr(tx)]) . token;
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
WriteName       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'write')   *$' '
                    . *Shift('TT_VAR', '__pas_write') . *IncCounter();
WritelnName     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'writeln') *$' '
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
                    | *$' ' ('.' *$' ' (*Ident) . thx . *Shift('TT_VAR', thx) . *Reduce('TT_FIELD', 2)
                    | '^' *$' ' . *Reduce('TT_DEREF', 1)
                    ));
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
MulOp           =   ( *$' ' ('*' *$' '   *Expr3 . *Reduce('TT_MUL', 2)
                    | '/' *$' '   . *Shift('TT_FLIT', '1') . *Reduce('TT_MUL', 2)
                             *Expr3 . *Reduce('TT_DIV', 2)
                    ) | *$'div' *Expr3 . *Reduce('TT_DIV', 2)
                    | *$'mod' *Expr3 . *Reduce('TT_MOD', 2)
                    | *$'and' *Expr3 . *Reduce('TT_MUL', 2)
                    );
Expr2           =   *Expr3 *MulStar;
MulStar         =   FENCE(*MulOp *MulStar | epsilon);
AddOp           =   ( *$' ' ('+' *$' '  *Expr2 . *Reduce('TT_ADD', 2)
                    | '-' *$' '  *Expr2 . *Reduce('TT_SUB', 2)
                    ) | *$'or' *Expr2 . *Reduce('TT_ADD', 2)
                    );
Expr1           =   *Expr2 *AddStar;
AddStar         =   FENCE(*AddOp *AddStar | epsilon);
RelOp           =   ( *$' ' ('<>' *$' ' *Expr1 . *Reduce('TT_NE', 2)
                    | '<=' *$' ' *Expr1 . *Reduce('TT_LE', 2)
                    | '>=' *$' ' *Expr1 . *Reduce('TT_GE', 2)
                    | '=' *$' '  *Expr1 . *Reduce('TT_EQ', 2)
                    | '<' *$' '  *Expr1 . *Reduce('TT_LT', 2)
                    | '>' *$' '  *Expr1 . *Reduce('TT_GT', 2)
                    ) | *$'in' . *Shift('TT_VAR', '__pas_in') *swap *Expr1 . *Reduce('TT_FNC', 3)
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
StmtRest        =   *$';' FENCE(*Command . *IncCounter() | epsilon);
/* the statements of a sequence repeat greedily: a refusal further on fails at once instead of    */
/* backtracking through every earlier statement (46 lines took over 60 s that way)                */
StmtStar        =   FENCE(*StmtRest *StmtStar | epsilon);
compound_cmd_rest =   epsilon . *PushCounter() *$' ' FENCE(*StmtFirst *StmtStar | epsilon) *$'end'
                    . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter();
/* if C then S [else S] -> TT_IF(C, S[, S])                                              */
if_cmd_rest     =   *$' ' *Expr0 *$'then' *Command
                    FENCE( *$'else' *Command . *Reduce('TT_IF', 3)
                         | epsilon . *Reduce('TT_IF', 2) );
/* while C do S -> TT_WHILE(C, S)                                                        */
while_cmd_rest  =   *$' ' *Expr0 *$'do' *Command . *Reduce('TT_WHILE', 2);
/* repeat S… until C -> TT_REPEAT(S…, C).  The body is a statement sequence.             */
repeat_cmd_rest =   epsilon . *PushCounter() *$' ' FENCE(*StmtFirst *StmtStar | epsilon) *$'until'
                    *Expr0 . *IncCounter() . *Reduce('TT_REPEAT', nTop()) . *PopCounter();
/* for v := a to|downto b do S -> TT_FOR(v, a, b, S)                                     */
for_cmd_rest    =   *$' ' (*Ident) . thx . *Shift('TT_VAR', thx) *$':=' *Expr0
                    (*$'to' | *$'downto') *Expr0 *$'do' *Command
                    . *Reduce('TT_FOR', 4);
/* case e of c1, c2: S; … end -> TT_CASE(e, TT_ALT(c1, c2), S, …): an arm's constants are one TT_ALT */
CaseConst       =   *Expr0 FENCE(*$'..' *Expr0 . *Reduce('TT_TO', 2) | epsilon) . *IncCounter();
/* the constants and the arms repeat greedily: an arm, once parsed, is never re-parsed when a later one refuses */
ConstStar       =   FENCE(*$',' *CaseConst *ConstStar | epsilon);
CaseArm         =   epsilon . *PushCounter() *CaseConst *ConstStar . *Reduce('TT_ALT', nTop()) . *PopCounter() . *IncCounter()
                    *$':' *Command . *IncCounter();
ArmStar         =   FENCE(*$';' *CaseArm *ArmStar | epsilon);
case_cmd_rest   =   epsilon . *PushCounter() *$' ' *Expr0 . *IncCounter() *$'of'
                    *CaseArm *ArmStar FENCE(*$';' | epsilon)
                    FENCE(*$'else' *Command . *IncCounter() FENCE(*$';' | epsilon) | epsilon)
                    *$'end' . *Reduce('TT_CASE', nTop()) . *PopCounter();
/* with r1, r2 do S -> TT_FNC(TT_VAR __pas_with, r1, r2, S), the oracle's naming for a lowered form  */
with_cmd_rest   =   epsilon . *PushCounter() *$' ' . *Shift('TT_VAR', '__pas_with') . *IncCounter()
                    *Expr0 . *IncCounter() ARBNO(*$',' *Expr0 . *IncCounter()) *$'do' *Command . *IncCounter()
                    . *Reduce('TT_FNC', nTop()) . *PopCounter();
/* goto 20 -> (TT_GOTO_U 20); 10: S -> TT_LABEL_DEF(TT_ILIT 10, S)                          */
goto_cmd_rest   =   *$' ' (*Integer) . thx . *Shift('TT_GOTO_U', thx);
label_cmd       =   (*Integer) . thx . *Shift('TT_ILIT', thx) *$':' *Command . *Reduce('TT_LABEL_DEF', 2);
/* an empty statement is legal Pascal wherever a statement may appear: the oracle's TT_SUCCEED */
empty_cmd       =   *$' ' *IDENT(epsilon, epsilon) . *Shift('TT_SUCCEED', '');
CmdT            =   TABLE(8);
kw_cmd          =   *$' ' *Id $ tx *DIFFER(CmdT[lwr(tx)]) *CmdT[lwr(tx)];
Command         =   *$' ' ( *kw_cmd
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
TypeName        =   *$' ' *Id *$' ' FENCE(*$'[' BREAK(']') ']' *$' ' | epsilon);
/* A type denoter, ISO 7185 6.4: parsed and discarded like the rest of the declarations.  */
IdList          =   *Ident ARBNO(*$',' *Ident);
SConst          =   FENCE(*$' ' ('-' *$' ' | '+' *$' ' ) | epsilon) (*Real | *Integer | *Quoted | *Ident);
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
SubBody         =   epsilon . *PushCounter() *$'begin' FENCE(*StmtFirst *StmtStar | epsilon) *$'end'
                    . *Reduce('TT_PROGRAM', nTop()) . *PopCounter();
proc_decl       =   epsilon . *PushCounter() (*$'procedure' | *$'function') (*Ident) . thx . *Shift('TT_VAR', thx) . *IncCounter()
                    *Params . *IncCounter() FENCE(*$':' *TypeName | epsilon) *$';'
                    ( *$'forward' *$';' . *PushCounter() . *Reduce('TT_PROGRAM', nTop()) . *PopCounter() . *IncCounter()
                    | *Decls *SubBody . *IncCounter() *$';' )
                    . *PushCounter() . *Reduce('TT_VLIST', nTop()) . *PopCounter() . *IncCounter()
                    . *Reduce('TT_PROC_DECL', nTop()) . *PopCounter();
/* A block's declaration parts in ISO 7185 6.2.1's order: label, const, type, var, then the   */
/* procedures and functions; the C frontend refuses any other order, and so does this.          */
ProcDecls       =   FENCE(*proc_decl . *IncCounter() FLUSH *ProcDecls | epsilon);
Decls           =   FENCE(*label_part | epsilon) FENCE(*const_part | epsilon) FENCE(*type_part | epsilon)
                    FENCE(*var_part | epsilon) *ProcDecls;
/* ==================================================================================================================== */
/* Compiland — program header, declarations, main block.  The main block is emitted as    */
/* a TT_PROC_DECL named `main`, which is the shape the C frontend's dump carries.         */
/* ==================================================================================================================== */
MainBody        =   epsilon . *PushCounter() *$'begin' FENCE(*StmtFirst *StmtStar | epsilon) *$'end'
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
CmdT['begin'] = compound_cmd_rest; CmdT['if'] = if_cmd_rest; CmdT['while'] = while_cmd_rest; CmdT['repeat'] = repeat_cmd_rest;
CmdT['for'] = for_cmd_rest; CmdT['case'] = case_cmd_rest; CmdT['with'] = with_cmd_rest; CmdT['goto'] = goto_cmd_rest;
/* ==================================================================================================================== */
/* Preprocess: text to text. The conditional-compilation pass of FPC (define undef setc, ifdef ifndef if ifc ifopt elseif else elsec endif ifend endc, */
/* include) generates the compiland text; the Compiland pattern above parses ONLY that text (Lon 2026-10-03, CEO-1483; semantics answered by hq_pascal). */
/* Every other directive passes through as the comment it is. A dropped or consumed span keeps its newlines, so line numbers hold.               */
struct ppdef { pv }
/* ==================================================================================================================== */
/* the patterns are built the first time a source carries a conditional directive: a program with none runs the Compiland alone, as before */
function PPPat() {
    pp_spc   = ' ' CHAR(9) CHAR(10) CHAR(13);
    pp_nl    = CHAR(10);
    pp_sp    = "'{(/";
    pp_num   = ( POS(0) FENCE('-' | epsilon) SPAN('0123456789') FENCE('.' SPAN('0123456789') | epsilon)
                 FENCE(ANY('eE') FENCE(ANY('+-') | epsilon) SPAN('0123456789') | epsilon) RPOS(0) );
    pp_dir   = ( ( '{$' BREAK('}') '}' | '(*$' BREAKX('*') '*)' ) . pp_t . *PPDir(pp_t) );
    pp_cmt   = ( '{' BREAK('}') '}' | '(*' FENCE(BREAKX('*') '*)') | '//' FENCE(BREAK(pp_nl) | REM) );
    pp_str   = ( "'" ARBNO(NOTANY("'" pp_nl) | "''") "'" );
    pp_oth   = ( NOTANY(pp_sp) FENCE(BREAK(pp_sp) | REM) | ANY(pp_sp) );
    pp_txt   = ( ( pp_cmt | pp_str | pp_oth ) . pp_t . *PPTxt(pp_t) );
    Preprocess = ( POS(0) ARBNO(FENCE(pp_dir | pp_txt)) RPOS(0) );
    pp_ready = 1;
    return;
}
/* ==================================================================================================================== */
function PPInit(w, s) {
    if (IDENT(pp_ready)) { PPPat(); }
    pp_sym = TABLE(31);
    pp_inc = TABLE(31);
    pp_stk = TABLE(31);
    pp_sn = 0;
    pp_base = 0;
    pp_skip = '';
    pp_err = '';
    pp_out = '';
    pp_buf = '';
    s = 'fpc unix linux cpu64 cpux86_64 endian_little ';
    while (s ? (POS(0) BREAK(' ') . w ' ') = ) { pp_sym[w] = ppdef('1'); }
    return;
}
/* ==================================================================================================================== */
function PPErr(msg) {
    TERMINAL = 'Preprocess: ' pp_t ': ' msg;
    pp_err = 1;
    return;
}
/* ==================================================================================================================== */
function PPEmit(s) {
    pp_buf = pp_buf s;
    if (GT(SIZE(pp_buf), 2048)) { pp_out = pp_out pp_buf; pp_buf = ''; }
    return;
}
/* ==================================================================================================================== */
function PPNls(s, out) {
    out = '';
    if (s ? pp_nl) { while (s ? (BREAK(pp_nl) pp_nl) = ) { out = out pp_nl; } }
    PPNls = out;
    return;
}
/* ==================================================================================================================== */
function PPTxt(tl) {
    PPTxt = .dummy;
    if (IDENT(pp_skip)) { PPEmit(tl); } else { PPEmit(PPNls(tl)); }
    nreturn;
}
/* ==================================================================================================================== */
function PPDirOf(f, d, p) {
    d = '';
    while (f ? (POS(0) BREAK('/') . p '/') = ) { d = d p '/'; }
    PPDirOf = d;
    return;
}
/* ==================================================================================================================== */
function PPOpen(path, t) {
    PPOpen = ;
    if (INPUT(.PPIN, 10, path '[-r16777215]')) {
        t = '';
        t = PPIN;
        ENDFILE(10);
        PPOpen = t;
        return;
    }
    freturn;
}
/* ==================================================================================================================== */
function PPFind(nm, t, lp, dir) {
    PPFind = ;
    if (t = PPOpen(nm)) { PPFind = t; return; }
    if (DIFFER(pp_cur)) { if (t = PPOpen(pp_cur nm)) { PPFind = t; return; } }
    lp = HOST(4, 'LPATH');
    while (DIFFER(lp)) {
        if (lp ? (POS(0) BREAK(': ') . dir ANY(': ')) = ) { ; } else { dir = lp; lp = ''; }
        if (t = PPOpen(dir '/' nm)) { PPFind = t; return; }
    }
    freturn;
}
/* ==================================================================================================================== */
function PPRead(nm, t) {
    PPRead = ;
    if (t = PPFind(nm)) { PPRead = t; return; }
    if (nm ? '.') { freturn; }
    if (t = PPFind(nm '.inc')) { PPRead = t; return; }
    freturn;
}
/* ==================================================================================================================== */
/* the arm's expression: defined/undefined, not, and, or, comparison, integer/real/boolean literals, symbols that carry a value (define X := v, setc);  */
/* declared() sizeof() option() `in` and an unknown identifier are not decided here: the arm is taken as false and one line names the directive         */
function PPWs() {
    pp_e ? (POS(0) SPAN(pp_spc)) = ;
    return;
}
/* ==================================================================================================================== */
function PPKw(k, w) {
    PPWs();
    w = '';
    pp_e ? (POS(0) Id . w);
    if (IDENT(w, k)) { pp_e ? (POS(0) Id) = ; return; }
    freturn;
}
/* ==================================================================================================================== */
function PPTruth(v) {
    PPTruth = 'false';
    if (IDENT(v, 'true')) { PPTruth = 'true'; }
    else if (v ? pp_num) { if (NE(v, 0)) { PPTruth = 'true'; } }
    return;
}
/* ==================================================================================================================== */
function PPAtom(w, v, q) {
    PPWs();
    if (pp_e ? (POS(0) '(') = ) {
        v = PPOr();
        PPWs();
        if (pp_e ? (POS(0) ')') = ) { ; } else { pp_bad = 'expression'; }
        PPAtom = v;
        return;
    }
    if (pp_e ? (POS(0) (SPAN('0123456789') FENCE('.' SPAN('0123456789') | epsilon) FENCE(ANY('eE') FENCE(ANY('+-') | epsilon) SPAN('0123456789') | epsilon)) . w) = ) {
        PPAtom = w;
        return;
    }
    if (pp_e ? (POS(0) Id . w) = ) {
        if (w ? (POS(0) ('defined' | 'undefined') RPOS(0))) {
            q = '';
            if (pp_e ? (POS(0) FENCE(SPAN(pp_spc) | epsilon) '(' FENCE(SPAN(pp_spc) | epsilon) Id . q FENCE(SPAN(pp_spc) | epsilon) ')') = ) { ; }
            else if (pp_e ? (POS(0) SPAN(pp_spc) Id . q) = ) { ; }
            else { pp_bad = w; }
            v = 'false';
            if (DIFFER(pp_sym[q])) { v = 'true'; }
            if (IDENT(w, 'undefined')) { if (IDENT(v, 'true')) { v = 'false'; } else { v = 'true'; } }
            PPAtom = v;
            return;
        }
        if (w ? (POS(0) ('declared' | 'sizeof' | 'option') RPOS(0))) {
            pp_e ? (POS(0) FENCE(SPAN(pp_spc) | epsilon) '(' BREAK(')') ')') = ;
            pp_bad = w;
            PPAtom = 'false';
            return;
        }
        if (w ? (POS(0) ('true' | 'false') RPOS(0))) { PPAtom = w; return; }
        if (DIFFER(pp_sym[w])) { PPAtom = pv(pp_sym[w]); return; }
        pp_bad = 'identifier ' w;
        PPAtom = 'false';
        return;
    }
    pp_bad = 'expression';
    pp_e = '';
    PPAtom = 'false';
    return;
}
/* ==================================================================================================================== */
function PPCmp(a, b, op, n, r) {
    a = PPAtom();
    PPWs();
    if (pp_e ? (POS(0) 'in' ANY(pp_spc '['))) { pp_bad = 'in'; pp_e = ''; PPCmp = 'false'; return; }
    if (pp_e ? (POS(0) ('<>' | '<=' | '>=' | '<' | '>' | '=') . op) = ) {
        b = PPAtom();
        n = '';
        if (a ? pp_num) { if (b ? pp_num) { n = 1; } }
        r = 'false';
        if (DIFFER(n)) {
            if (IDENT(op, '<>')) { if (NE(a, b)) { r = 'true'; } }
            else if (IDENT(op, '<=')) { if (LE(a, b)) { r = 'true'; } }
            else if (IDENT(op, '>=')) { if (GE(a, b)) { r = 'true'; } }
            else if (IDENT(op, '<')) { if (LT(a, b)) { r = 'true'; } }
            else if (IDENT(op, '>')) { if (GT(a, b)) { r = 'true'; } }
            else { if (EQ(a, b)) { r = 'true'; } }
        } else {
            if (IDENT(op, '<>')) { if (DIFFER(a, b)) { r = 'true'; } }
            else if (IDENT(op, '<=')) { if (LLE(a, b)) { r = 'true'; } }
            else if (IDENT(op, '>=')) { if (LGE(a, b)) { r = 'true'; } }
            else if (IDENT(op, '<')) { if (LLT(a, b)) { r = 'true'; } }
            else if (IDENT(op, '>')) { if (LGT(a, b)) { r = 'true'; } }
            else { if (IDENT(a, b)) { r = 'true'; } }
        }
        PPCmp = r;
        return;
    }
    PPCmp = a;
    return;
}
/* ==================================================================================================================== */
function PPNot(w, v) {
    PPWs();
    w = '';
    pp_e ? (POS(0) Id . w);
    if (IDENT(w, 'not')) {
        pp_e ? (POS(0) Id) = ;
        v = PPNot();
        if (IDENT(PPTruth(v), 'true')) { PPNot = 'false'; } else { PPNot = 'true'; }
        return;
    }
    PPNot = PPCmp();
    return;
}
/* ==================================================================================================================== */
function PPAnd(v, b) {
    v = PPNot();
    while (PPKw('and')) {
        b = PPNot();
        if (IDENT(PPTruth(v), 'true')) { v = PPTruth(b); } else { v = 'false'; }
    }
    PPAnd = v;
    return;
}
/* ==================================================================================================================== */
function PPOr(v, b) {
    v = PPAnd();
    while (PPKw('or')) {
        b = PPAnd();
        if (IDENT(PPTruth(v), 'true')) { v = 'true'; } else { v = PPTruth(b); }
    }
    PPOr = v;
    return;
}
/* ==================================================================================================================== */
function PPCond(e, v) {
    pp_e = lwr(e);
    pp_bad = '';
    v = PPOr();
    PPWs();
    if (DIFFER(pp_e)) { if (IDENT(pp_bad)) { pp_bad = 'expression'; } }
    if (DIFFER(pp_bad)) {
        TERMINAL = 'Preprocess: ' pp_t ': unsupported ' pp_bad ', the arm is taken as false';
        PPCond = 'wait';
        return;
    }
    if (IDENT(PPTruth(v), 'true')) { PPCond = ''; } else { PPCond = 'wait'; }
    return;
}
/* ==================================================================================================================== */
/* pp_skip: '' emitting; 'wait' skipping, no arm taken yet; 'done' skipping, an arm was taken; 'off' inside a skipped arm of an outer conditional */
function PPDir(tx, b, cmd, rest, sym, val, nm, t, sv, st, md, inc) {
    PPDir = .dummy;
    pp_t = tx;
    if (tx ? (POS(0) '{$' REM . b)) { b ? (ANY('}') RPOS(0)) = ; } else { tx ? (POS(0) '(*$' REM . b); b ? ('*)' RPOS(0)) = ; }
    b ? (POS(0) FENCE(SPAN(pp_spc) | epsilon) FENCE(Id . cmd | epsilon) FENCE(SPAN(pp_spc) | epsilon) REM . rest);
    cmd = lwr(cmd);
    rest ? (SPAN(pp_spc) RPOS(0)) = ;
    inc = '';
    if (cmd ? (POS(0) ('i' | 'include') RPOS(0))) { if (DIFFER(rest)) { if (~(rest ? (POS(0) ANY('+-,')))) { inc = 1; } } }
    if (cmd ? (POS(0) ('ifdef' | 'ifndef') RPOS(0))) {
        pp_sn = pp_sn + 1;
        pp_stk[pp_sn] = pp_skip;
        if (DIFFER(pp_skip)) { pp_skip = 'off'; }
        else {
            sym = '';
            rest ? (POS(0) Id . sym);
            st = 'wait';
            if (DIFFER(pp_sym[lwr(sym)])) { st = ''; }
            if (IDENT(cmd, 'ifndef')) { if (IDENT(st)) { st = 'wait'; } else { st = ''; } }
            pp_skip = st;
        }
        PPEmit(PPNls(tx));
    } else if (cmd ? (POS(0) ('if' | 'ifc' | 'ifopt') RPOS(0))) {
        pp_sn = pp_sn + 1;
        pp_stk[pp_sn] = pp_skip;
        if (DIFFER(pp_skip)) { pp_skip = 'off'; }
        else if (IDENT(cmd, 'ifopt')) { TERMINAL = 'Preprocess: ' tx ': unsupported ifopt, the arm is taken as false'; pp_skip = 'wait'; }
        else { pp_skip = PPCond(rest); }
        PPEmit(PPNls(tx));
    } else if (IDENT(cmd, 'elseif')) {
        if (LE(pp_sn, pp_base)) { PPErr('no corresponding $if...'); }
        else if (IDENT(pp_skip)) { pp_skip = 'done'; }
        else if (IDENT(pp_skip, 'wait')) { pp_skip = PPCond(rest); }
        PPEmit(PPNls(tx));
    } else if (cmd ? (POS(0) ('else' | 'elsec') RPOS(0))) {
        if (LE(pp_sn, pp_base)) { PPErr('no corresponding $if...'); }
        else if (IDENT(pp_skip)) { pp_skip = 'done'; }
        else if (IDENT(pp_skip, 'wait')) { pp_skip = ''; }
        PPEmit(PPNls(tx));
    } else if (cmd ? (POS(0) ('endif' | 'ifend' | 'endc') RPOS(0))) {
        if (LE(pp_sn, pp_base)) { PPErr('no corresponding $if...'); }
        else { pp_skip = pp_stk[pp_sn]; pp_sn = pp_sn - 1; }
        PPEmit(PPNls(tx));
    } else if (DIFFER(pp_skip)) {
        PPEmit(PPNls(tx));
    } else if (cmd ? (POS(0) ('define' | 'definec' | 'setc') RPOS(0))) {
        if (rest ? (POS(0) Id . sym FENCE(SPAN(pp_spc) | epsilon) FENCE((':=' | '=') FENCE(SPAN(pp_spc) | epsilon) REM . val | epsilon))) {
            if (IDENT(val)) { val = '1'; }
            pp_sym[lwr(sym)] = ppdef(lwr(val));
        } else { PPErr('syntax error'); }
        PPEmit(PPNls(tx));
    } else if (cmd ? (POS(0) ('undef' | 'undefc') RPOS(0))) {
        if (rest ? (POS(0) Id . sym)) { pp_sym[lwr(sym)] = ; } else { PPErr('syntax error'); }
        PPEmit(PPNls(tx));
    } else if (DIFFER(inc)) {
        PPEmit(PPNls(tx));
        if (rest ? (POS(0) "'" BREAK("'") . nm "'")) { ; }
        else if (rest ? (POS(0) (NOTANY(pp_spc "'") FENCE(BREAK(pp_spc) | REM)) . nm)) { ; }
        else { nm = ''; }
        if (IDENT(nm)) { PPErr('syntax error'); }
        else if (DIFFER(pp_inc[nm])) { PPErr('circular reference to ' nm); }
        else if (t = PPRead(nm)) {
            pp_inc[nm] = 1;
            sv = pp_base;
            pp_base = pp_sn;
            t ? *Preprocess;
            if (NE(pp_sn, pp_base)) { PPErr('$if(s) without $endif(s)'); pp_skip = pp_stk[pp_base + 1]; pp_sn = pp_base; }
            pp_base = sv;
            pp_inc[nm] = ;
        } else { TERMINAL = 'Preprocess: ' tx ': cannot open ' nm ', the include is dropped'; }
    } else if (IDENT(cmd, 'mode')) {
        md = '';
        rest ? (POS(0) Id . md);
        md = lwr(md);
        pp_sym['fpc_iso'] = ;
        pp_sym['fpc_objfpc'] = ;
        pp_sym['fpc_delphi'] = ;
        pp_sym['fpc_tp'] = ;
        pp_sym['fpc_macpas'] = ;
        if (md ? (POS(0) ('iso' | 'objfpc' | 'delphi' | 'tp' | 'macpas') RPOS(0))) { pp_sym['fpc_' md] = ppdef('1'); }
        PPEmit(tx);
    } else {
        PPEmit(tx);
    }
    nreturn;
}
/* ==================================================================================================================== */
function ParseOne(ptree, i, n_kids) {
    if (Src ? ('{$' | '(*$')) {
        if (lwr(Src) ? (('{$' | '(*$') ('if' | 'else' | 'endif' | 'endc' | 'define' | 'undef' | 'setc' | 'include' | 'i' ANY(' ' CHAR(9) CHAR(10) CHAR(13))))) {
            PPInit();
            pp_cur = '';
            if (DIFFER(pf_name)) { pp_cur = PPDirOf(pf_name); }
            if (Src ? *Preprocess) { ; } else { pp_err = 1; }
            if (NE(pp_sn, 0)) { PPErr('$if(s) without $endif(s)'); }
            if (DIFFER(pp_err)) { OUTPUT = 'Parse Error'; return; }
            Src = pp_out pp_buf;
        }
    }
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
