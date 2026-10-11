/* parser_pascal.sc — the seventh self-hosted parser: ISO 7185 Pascal in Snocone.
   Written 2026-09-16 by hq_snocone on Lon's order (CEO-770 row: all seven parser_*.sc
   parse their language's corpus into a proper tree_t with proper TT_* types).
   THE TREE IS THE PRUNED PARSE TREE (RULES.md § FACT RULE, Lon 2026-10-03): source order, one node per rule that
   fires, built with Shift/Reduce and the Counter stack only, no desugaring.  The shapes are the C frontend's
   own (src/parsers/pascal/pascal.y since hq_pascal's pruned-tree landing, SCRIP 1889d257e): TT_VAR <program>,
   [TT_VLIST of the program parameters], TT_BLOCK of TT_PART <label|const|type|var> rows of TT_DECL, TT_PROCEDURE
   and TT_FUNCTION (name, TT_PARAMS, result type, TT_BLOCK or TT_KEYWORD forward), a body as TT_SEQ_EXPR; types as
   TT_ENUM_TYPE, TT_SUBRANGE, TT_ARRAY_TYPE, TT_RECORD(TT_FIELDS), TT_SET_TYPE, TT_FILE_TYPE, TT_PTR_TYPE; a for
   loop carries to|downto as its value, a case arm is TT_ARM(TT_VLIST, S), and / is TT_DIV, div TT_IDIV, and TT_CONJ,
   or TT_ALT, not TT_NOT, in TT_IN, a sign is TT_MNS or TT_PLS over the first term, write and writeln are calls, a
   write argument e:w:p is TT_FMT.  Identifiers are lower-cased as the C lexer does.
   Runtime chain (same as every other parser_*.sc): global, case, assign, match, counter,
   stack, tree, ShiftReduce, tdump, gen, qize, semantic, omega, trace.
   ⛔ Pascal identifiers and keywords are CASE-INSENSITIVE, unlike every other language
   in this directory, so every keyword matcher folds through lwr() from case.sc. */
&FULLSCAN = 1;
reserved          = POS(0) ( 'and' | 'array' | 'begin' | 'case' | 'const' | 'div'
                           | 'do' | 'downto' | 'else' | 'end' | 'file' | 'for'
                           | 'function' | 'goto' | 'if' | 'in' | 'label' | 'mod'
                           | 'not' | 'of' | 'or' | 'packed' | 'procedure'
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
function Unq(s, r, p) {
    r = '';
    while (s ? (POS(0) BREAK("'") . p "''") = ) { r = r p "'"; }
    Unq = r s;
    return;
}
String      =   "'" (*StrChars) . thx . *Shift('TT_QLIT', Unq(thx)) "'";
ResT        =   TABLE(35);
ResT['and'] = 1; ResT['array'] = 1; ResT['begin'] = 1; ResT['case'] = 1; ResT['const'] = 1; ResT['div'] = 1; ResT['do'] = 1; ResT['downto'] = 1;
ResT['else'] = 1; ResT['end'] = 1; ResT['file'] = 1; ResT['for'] = 1; ResT['function'] = 1; ResT['goto'] = 1; ResT['if'] = 1; ResT['in'] = 1;
ResT['label'] = 1; ResT['mod'] = 1; ResT['not'] = 1; ResT['of'] = 1; ResT['or'] = 1; ResT['packed'] = 1; ResT['procedure'] = 1;
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
/* An argument is an expression, or a write argument with its field width and precision: e:w[:p] -> TT_FMT(e, w[, p]).  */
Arg             =   *Expr0 FENCE(*$':' *Expr0 FENCE(*$':' *Expr0 . *Reduce('TT_FMT', 3) | epsilon . *Reduce('TT_FMT', 2)) | epsilon);
ArgFirst        =   *Arg . *IncCounter();
ArgRest         =   *$',' *Arg . *IncCounter();
CallArgs        =   *ArgFirst ARBNO(*ArgRest);
/* A call: the callee is a TT_VAR leaf and is itself one of the children; write and writeln are calls like any other.  */
ProcCall        =   epsilon . *PushCounter() (*Ident) . thx . *Shift('TT_VAR', lwr(thx)) . *IncCounter()
                    FENCE(*$'(' (*CallArgs | epsilon) *$')' | epsilon)
                    . *Reduce('TT_FNC', nTop()) . *PopCounter();
FuncCall        =   epsilon . *PushCounter() (*Ident) . thx . *Shift('TT_VAR', lwr(thx)) . *IncCounter()
                    *$'(' (*CallArgs | epsilon) *$')'
                    . *Reduce('TT_FNC', nTop()) . *PopCounter();
/* A set constructor [a, b..c] -> TT_SET(a, TT_SUBRANGE(b, c)).  */
SetMember       =   *Expr0 FENCE(*$'..' *Expr0 . *Reduce('TT_SUBRANGE', 2) | epsilon) . *IncCounter();
SetTail         =   FENCE(*$',' *SetMember *SetTail | epsilon);
SetCtor         =   epsilon . *PushCounter() *$'[' FENCE(*SetMember *SetTail | epsilon) *$']'
                    . *Reduce('TT_SET', nTop()) . *PopCounter();
/* A subscripted variable: a[i] -> TT_IDX(a, i…); the leading nInc counts a itself.        */
IdxTail         =   epsilon . *IncCounter() *$'[' *ArgFirst ARBNO(*ArgRest) *$']';
/* A variable access is a primary and any number of postfixes: a[i], r.f, p^.               */
Postfix         =   ( epsilon . *PushCounter() *IdxTail . *Reduce('TT_IDX', nTop()) . *PopCounter()
                    | *$' ' ('.' *$' ' (*Ident) . thx . *Shift('TT_VAR', lwr(thx)) . *Reduce('TT_FIELD', 2)
                    | '^' *$' ' . *Reduce('TT_DEREF', 1)
                    ));
PostStar        =   FENCE(*Postfix *PostStar | epsilon);
/* true, false and nil are identifiers; #n is a character literal with no value of its own in the dump.  */
Primary         =   ( *$'(' *Expr0 *$')'
                    | '#' (*Integer) . *Shift('TT_CHRLIT', '')
                    | *SetCtor
                    | (*Real) . thx . *Shift('TT_FLIT', thx)
                    | (*Integer) . thx . *Shift('TT_ILIT', thx)
                    | *String
                    | *FuncCall
                    | (*Ident) . thx . *Shift('TT_VAR', lwr(thx))
                    );
Expr4           =   *Primary *PostStar;
Expr3           =   *$'@'   *Expr3 . *Reduce('TT_ADDR', 1)
                |   *$'not' *Expr3 . *Reduce('TT_NOT', 1)
                |   *Expr4;
/* Multiplying operators: * / div mod and.                                                */
MulOp           =   ( *$' ' ('*' *$' '   *Expr3 . *Reduce('TT_MUL', 2)
                    | '/' *$' '   *Expr3 . *Reduce('TT_DIV', 2)
                    ) | *$'div' *Expr3 . *Reduce('TT_IDIV', 2)
                    | *$'mod' *Expr3 . *Reduce('TT_MOD', 2)
                    | *$'and' *Expr3 . *Reduce('TT_CONJ', 2)
                    );
Expr2           =   *Expr3 *MulStar;
MulStar         =   FENCE(*MulOp *MulStar | epsilon);
AddOp           =   ( *$' ' ('+' *$' '  *Expr2 . *Reduce('TT_ADD', 2)
                    | '-' *$' '  *Expr2 . *Reduce('TT_SUB', 2)
                    ) | *$'or' *Expr2 . *Reduce('TT_ALT', 2)
                    );
/* A sign belongs to the first term of a simple expression: -a*b is TT_MNS(TT_MUL(a, b)).  */
Expr1           =   ( *$'-' *Expr2 . *Reduce('TT_MNS', 1)
                    | *$'+' *Expr2 . *Reduce('TT_PLS', 1)
                    | *Expr2 ) *AddStar;
AddStar         =   FENCE(*AddOp *AddStar | epsilon);
RelOp           =   ( *$' ' ('<>' *$' ' *Expr1 . *Reduce('TT_NE', 2)
                    | '<=' *$' ' *Expr1 . *Reduce('TT_LE', 2)
                    | '>=' *$' ' *Expr1 . *Reduce('TT_GE', 2)
                    | '=' *$' '  *Expr1 . *Reduce('TT_EQ', 2)
                    | '<' *$' '  *Expr1 . *Reduce('TT_LT', 2)
                    | '>' *$' '  *Expr1 . *Reduce('TT_GT', 2)
                    ) | *$'in' *Expr1 . *Reduce('TT_IN', 2)
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
repeat_cmd_rest =   epsilon . *PushCounter() *$' ' FENCE(*StmtFirst *StmtStar | epsilon) . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter() *$'until'
                    *Expr0 . *Reduce('TT_REPEAT', 2);
/* for v := a to|downto b do S -> TT_FOR(v, a, b, S)                                     */
for_cmd_rest    =   *$' ' (*Ident) . thx . *Shift('TT_VAR', lwr(thx)) *$':=' *Expr0
                    (*$'to' . *PushVal('to') | *$'downto' . *PushVal('downto')) *Expr0 *$'do' *Command
                    . *Reduce('TT_FOR', 4, PopVal());
/* case e of c1, c2: S; … end -> TT_CASE(e, TT_ALT(c1, c2), S, …): an arm's constants are one TT_ALT */
CaseConst       =   *Expr0 FENCE(*$'..' *Expr0 . *Reduce('TT_SUBRANGE', 2) | epsilon) . *IncCounter();
/* the constants and the arms repeat greedily: an arm, once parsed, is never re-parsed when a later one refuses */
ConstStar       =   FENCE(*$',' *CaseConst *ConstStar | epsilon);
CaseArm         =   *CaseLabels *$':' *Command . *Reduce('TT_ARM', 2) . *IncCounter();
ArmStar         =   FENCE(*$';' *CaseArm *ArmStar | epsilon);
case_cmd_rest   =   epsilon . *PushCounter() *$' ' *Expr0 . *IncCounter() *$'of'
                    *CaseArm *ArmStar FENCE(*$';' | epsilon)
                    FENCE(*$'else' epsilon . *PushCounter() FENCE(*StmtFirst *StmtStar | epsilon) . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter()
                         . *Reduce('TT_PART', 1, 'else') . *IncCounter() | epsilon)
                    *$'end' . *Reduce('TT_CASE', nTop()) . *PopCounter();
/* with r1, r2 do S -> TT_FNC(TT_VAR __pas_with, r1, r2, S), the oracle's naming for a lowered form  */
with_cmd_rest   =   epsilon . *PushCounter() *$' '
                    *Expr0 . *IncCounter() ARBNO(*$',' *Expr0 . *IncCounter()) *$'do' *Command . *IncCounter()
                    . *Reduce('TT_WITH', nTop()) . *PopCounter();
/* goto 20 -> (TT_GOTO_U 20); 10: S -> TT_LABEL_DEF(TT_ILIT 10, S)                          */
goto_cmd_rest   =   *$' ' (*Integer) . thx . *Shift('TT_ILIT', thx) . *Reduce('TT_GOTO_U', 1);
label_cmd       =   (*Integer) . thx . *Shift('TT_ILIT', thx) *$':' *Command . *Reduce('TT_LABEL_DEF', 2);
/* an empty statement is legal Pascal wherever a statement may appear: the oracle's TT_SUCCEED */
empty_cmd       =   *$' ' *IDENT(epsilon, epsilon) . *Shift('TT_SUCCEED', '');
CmdT            =   TABLE(8);
kw_cmd          =   *$' ' *Id $ tx *DIFFER(CmdT[lwr(tx)]) *CmdT[lwr(tx)];
Command         =   *$' ' ( *kw_cmd
                    | *label_cmd
                    | *assign_cmd
                    | *ProcCall
                    | *empty_cmd
                    );
/* ==================================================================================================================== */
/* Declarations.  A block's parts keep their rows: TT_PART <kind> of TT_DECL <kind> (pascal.y), types as the grammar builds them. */
/* ==================================================================================================================== */
Name            =   (*Ident) . thx . *Shift('TT_VAR', lwr(thx));
IdList          =   epsilon . *PushCounter() *Name . *IncCounter() ARBNO(*$',' *Name . *IncCounter()) . *Reduce('TT_VLIST', nTop()) . *PopCounter();
/* A type name, or string[n] -> TT_IDX(string, n).  */
TypeName        =   *Name FENCE(*$'[' *Expr0 *$']' . *Reduce('TT_IDX', 2) | epsilon);
/* A type denoter, ISO 7185 6.4.  */
EnumType        =   epsilon . *PushCounter() *$'(' *Name . *IncCounter() ARBNO(*$',' *Name . *IncCounter()) *$')' . *Reduce('TT_ENUM_TYPE', nTop()) . *PopCounter();
SimpleType      =   ( *EnumType
                    | *Expr1 *$'..' *Expr1 . *Reduce('TT_SUBRANGE', 2)
                    | *TypeName
                    );
CaseLabels      =   epsilon . *PushCounter() *CaseConst *ConstStar . *Reduce('TT_VLIST', nTop()) . *PopCounter();
FieldDecl       =   *IdList *$':' *TypeSpec . *Reduce('TT_DECL', 2, 'field') . *IncCounter();
FieldDecls      =   FENCE(*FieldDecl | epsilon) FENCE(*$';' *FieldDecls | epsilon);
VariantArm      =   *CaseLabels *$':' *$'(' *RecordBody *$')' . *Reduce('TT_ARM', 2) . *IncCounter();
VariantArms     =   *VariantArm FENCE(*$';' FENCE(*VariantArms | epsilon) | epsilon);
VariantPart     =   epsilon . *PushCounter() *$'case' FENCE(*Name . *IncCounter() *$':' *Name . *IncCounter() | *Name . *IncCounter()) *$'of'
                    *VariantArms . *Reduce('TT_CASE', nTop()) . *PopCounter();
RecordBody      =   epsilon . *PushCounter() *FieldDecls FENCE(*VariantPart . *IncCounter() | epsilon) . *Reduce('TT_FIELDS', nTop()) . *PopCounter();
PackedOpt       =   FENCE(*$'packed' . *Shift('TT_KEYWORD', 'packed') . *IncCounter() | epsilon);
Structured      =   epsilon . *PushCounter() *PackedOpt
                    ( *$'array' FENCE(*$'[' *SimpleType . *IncCounter() ARBNO(*$',' *SimpleType . *IncCounter()) *$']' | epsilon) *$'of' *TypeSpec . *IncCounter()
                      . *Reduce('TT_ARRAY_TYPE', nTop())
                    | *$'record' *RecordBody . *IncCounter() *$'end' . *Reduce('TT_RECORD', nTop())
                    | *$'set' *$'of' *SimpleType . *IncCounter() . *Reduce('TT_SET_TYPE', nTop())
                    | *$'file' FENCE(*$'of' *TypeSpec . *IncCounter() | epsilon) . *Reduce('TT_FILE_TYPE', nTop())
                    ) . *PopCounter();
TypeSpec        =   ( *Structured
                    | *$'^' *Name . *Reduce('TT_PTR_TYPE', 1)
                    | *SimpleType
                    );
/* label, const, type and var parts.  */
label_part      =   epsilon . *PushCounter() *$'label' (*Integer) . thx . *Shift('TT_ILIT', thx) . *IncCounter()
                    ARBNO(*$',' (*Integer) . thx . *Shift('TT_ILIT', thx) . *IncCounter()) *$';'
                    . *Reduce('TT_PART', nTop(), 'label') . *PopCounter();
ConstDecl       =   *Name *$'=' *Expr0 *$';' . *Reduce('TT_DECL', 2, 'const') . *IncCounter();
ConstDecls      =   FENCE(*ConstDecl *ConstDecls | epsilon);
const_part      =   epsilon . *PushCounter() *$'const' *ConstDecl *ConstDecls . *Reduce('TT_PART', nTop(), 'const') . *PopCounter();
TypeDecl        =   *Name *$'=' *TypeSpec *$';' . *Reduce('TT_DECL', 2, 'type') . *IncCounter();
TypeDecls       =   FENCE(*TypeDecl *TypeDecls | epsilon);
type_part       =   epsilon . *PushCounter() *$'type' *TypeDecl *TypeDecls . *Reduce('TT_PART', nTop(), 'type') . *PopCounter();
VarGroup        =   *IdList *$':' *TypeSpec *$';' . *Reduce('TT_DECL', 2, 'var') . *IncCounter();
VarGroups       =   FENCE(*VarGroup *VarGroups | epsilon);
var_part        =   epsilon . *PushCounter() *$'var' *VarGroup *VarGroups . *Reduce('TT_PART', nTop(), 'var') . *PopCounter();
/* Parameters: TT_PARAMS of TT_DECL value|var|const|procedure|function rows; an empty list builds nothing.  */
ParamType       =   ( *$'array' *$'of' *Name . *Reduce('TT_ARRAY_TYPE', 1)
                    | *Name
                    );
ParamDecl       =   ( epsilon . *PushCounter() *$'procedure' *Name . *IncCounter() *ParamsOpt . *Reduce('TT_DECL', nTop(), 'procedure') . *PopCounter()
                    | epsilon . *PushCounter() *$'function' *Name . *IncCounter() *ParamsOpt FENCE(*$':' *Name . *IncCounter() | epsilon)
                      . *Reduce('TT_DECL', nTop(), 'function') . *PopCounter()
                    | *$'var' *IdList *$':' *ParamType . *Reduce('TT_DECL', 2, 'var')
                    | *$'const' *IdList *$':' *ParamType . *Reduce('TT_DECL', 2, 'const')
                    | *IdList *$':' *ParamType . *Reduce('TT_DECL', 2, 'value')
                    );
Params          =   epsilon . *PushCounter() *$'(' *ParamDecl . *IncCounter() ARBNO(*$';' *ParamDecl . *IncCounter()) *$')' . *Reduce('TT_PARAMS', nTop()) . *PopCounter();
ParamsOpt       =   FENCE(*Params . *IncCounter() | *$'(' *$')' | epsilon);
/* procedure P(params); <block>; -> TT_PROCEDURE(TT_VAR P, TT_PARAMS, TT_BLOCK); function F(params): T; adds the result type before the block; forward; is TT_KEYWORD forward. */
Body            =   epsilon . *PushCounter() *$'begin' FENCE(*StmtFirst *StmtStar | epsilon) *$'end' . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter();
Block           =   epsilon . *PushCounter() *Decls *Body . *IncCounter() . *Reduce('TT_BLOCK', nTop()) . *PopCounter();
ProcTail        =   ( *$'forward' *$';' . *Shift('TT_KEYWORD', 'forward') . *IncCounter()
                    | *Block . *IncCounter() *$';'
                    );
proc_decl       =   epsilon . *PushCounter() *$'procedure' *Name . *IncCounter() *ParamsOpt *$';' *ProcTail . *Reduce('TT_PROCEDURE', nTop()) . *PopCounter();
func_decl       =   epsilon . *PushCounter() *$'function' *Name . *IncCounter() *ParamsOpt FENCE(*$':' *Name . *IncCounter() | epsilon) *$';' *ProcTail
                    . *Reduce('TT_FUNCTION', nTop()) . *PopCounter();
/* A block's declaration parts in ISO 7185 6.2.1's order: label, const, type, var, then the   */
/* procedures and functions; the C frontend refuses any other order, and so does this.          */
ProcDecls       =   FENCE((*proc_decl | *func_decl) . *IncCounter() FLUSH *ProcDecls | epsilon);
Decls           =   FENCE(*label_part . *IncCounter() | epsilon) FENCE(*const_part . *IncCounter() | epsilon) FENCE(*type_part . *IncCounter() | epsilon)
                    FENCE(*var_part . *IncCounter() | epsilon) *ProcDecls;
/* ==================================================================================================================== */
/* Compiland — program header, then the block: TT_VAR <name>, the program parameters as a TT_VLIST, TT_BLOCK.  */
/* ==================================================================================================================== */
program_head    =   FENCE(*$'program' *Name . *IncCounter() FENCE(*$'(' *IdList *$')' . *IncCounter() | epsilon) *$';' | epsilon);
Compiland       =   epsilon . *PushCounter() POS(0) *$' ' *program_head
                    *Block . *IncCounter()
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
