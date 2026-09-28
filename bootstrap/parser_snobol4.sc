E_Parse            = "'Parse'";
/* PST-SN4-1c (2026-05-16): goto node kinds renamed from TT_ATTR-style tags
   (':goS'/':goF'/':go') to dedicated TT_GOTO_* kinds, mirroring C stmt_ast.c. */
E_goU              = "'TT_GOTO_U'";
E_goS              = "'TT_GOTO_S'";
E_goF              = "'TT_GOTO_F'";
Functions   = 'ABS AND ANY APPEND APPLY ARBNO ARG ARRAY ATAN BACKSPACE '
              'BCHAR BREAK BREAKX BSIZE BUFFER CC CHAR CHOP CLEAR CODE '
              'COLLECT COMPL CONVERT COPY COS DATA DATATYPE DATE DEF DEFINE '
              'DEPTH DETACH DIFFER DUMP DUP DUPL EJECT ENDFILE EQ EVAL EXIT '
              'EXP FENCE FIELD FIX FREEZE FRONT FUNCTION GE GT HEIGHT HOR '
              'HOR_REG HOST IDENT INPUT INSERT INTEGER IT ITEM LABEL LE LEN '
              'LEQ LGE LGT LLE LLT LN LNE LOAD LOC LOCAL LPAD LRECL LT '
              'MERGE NE NODE NORM_REG NOTANY OPSYN OR OUTPUT OVY PAR POS '
              'PRINT PROTOTYPE REMDR REP REPLACE REVERSE REWIND RPAD RPOS '
              'RSORT RTAB SER SET SETEXIT SIN SIZE SLAB SORT SPAN SQRT '
              'STOPTR SUBSTR TAB TABLE TAN THAW TIME TRACE TRIM UNLOAD '
              'VALUE VDIFFER VER VER_REG WIDTH XOR ';
UnprotKwds  = 'ABEND ANCHOR CASE CODE COMPARE DUMP ERRLIMIT ERRTEXT ERRTYPE '
              'FATALLIMIT FILL FTRACE FULLSCAN GTRACE INPUT MAXLNGTH OUTPUT '
              'PROFILE STLIMIT TRACE TRIM ';
ProtKwds    = 'ABORT ALPHABET ARB BAL COMPNO DIGITS FAIL FATAL FENCE FILE '
              'FNCLEVEL GCTIME LASTFILE LASTLINE LASTNO LCASE LINE MAXINT '
              'PARM PI REM RTNTYPE STCOUNT STEXEC STFCOUNT STNO SUCCEED '
              'UCASE ';
BuiltinVars = 'ABORT ARB BAL FAIL REM SUCCEED TERMINAL ';
SpecialNms  = 'ABORT CONTINUE END FRETURN NRETURN RETURN SCONTINUE START ';
/* ==================================================================================================================== */
/* PST-SN4-2 (2026-05-16): sn_match and sn_upr are pure tokenizer helpers — they perform
   keyword classification during lexing and build no tree nodes.  They are the only functions
   permitted in a pure-syntax-tree parser.  All stmt-building helpers (pp_stmt, strip_parens,
   make_goto_slot, push_qlit) are deleted; the grammar builds TT_STMT directly. */
function sn_match(subject, pattern) { sn_match = .dummy; if (subject ? pattern) nreturn; else freturn; }
/* ==================================================================================================================== */
function sn_upr(s)                  { sn_upr   = REPLACE(s, &LCASE, &UCASE); return; }
TxInList    =  (POS(0) | ' ') *sn_upr(tx) (' ' | RPOS(0));
Function    =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match(Functions,   TxInList);
BuiltinVar  =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match(BuiltinVars, TxInList);
SpecialNm   =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match(SpecialNms,  TxInList);
ProtKwd     =  SPAN(&UCASE &LCASE)                $ tx $ *sn_match(ProtKwds,    TxInList);
UnprotKwd   =  SPAN(&UCASE &LCASE)                $ tx $ *sn_match(UnprotKwds,  TxInList);
Integer     =  SPAN(digits);
DQ          =  '"' (BREAK('"' nl)) . thx . *Shift('TT_QLIT', thx) '"';
SQ          =  "'" (BREAK("'" nl)) . thx . *Shift('TT_QLIT', thx) "'";
String      =  *SQ | *DQ;
Real        =  (  SPAN(digits)
                  ('.' FENCE(SPAN(digits) | epsilon) | epsilon)
                  ('E' | 'e')
                  ('+' | '-' | epsilon)
                  SPAN(digits)
               |  SPAN(digits) '.' FENCE(SPAN(digits) | epsilon)
               );
Id          =  ANY(&UCASE &LCASE)
               FENCE(SPAN('.' digits &UCASE '_' &LCASE) | epsilon);
White       =  (  SPAN(' ' tab)
                  FENCE(nl ('+' | '.') FENCE(SPAN(' ' tab) | epsilon) | epsilon)
               |  nl ('+' | '.') FENCE(SPAN(' ' tab) | epsilon)
               );
Gray        =  White | epsilon;
$'  '       =  White;
$' '        =  Gray;
$'='        =  $'  ' '='  $'  ';
$'?'        =  $'  ' '?'  $'  ';
$'|'        =  $'  ' '|'  $'  ';
$'+'        =  $'  ' '+'  $'  ';
$'-'        =  $'  ' '-'  $'  ';
$'/'        =  $'  ' '/'  $'  ';
$'*'        =  $'  ' '*'  $'  ';
$'^'        =  $'  ' '^'  $'  ';
$'!'        =  $'  ' '!'  $'  ';
$'**'       =  $'  ' '**' $'  ';
$'$'        =  $'  ' '$'  $'  ';
$'.'        =  $'  ' '.'  $'  ';
$'&'        =  $'  ' '&'  $'  ';
$'@'        =  $'  ' '@'  $'  ';
$'#'        =  $'  ' '#'  $'  ';
$'%'        =  $'  ' '%'  $'  ';
$'~'        =  $'  ' '~'  $'  ';
$','        =  $' ' ',' $' ';
$'('        =  '(' $' ';
$'['        =  '[' $' ';
$'<'        =  '<' $' ';
$')'        =  $' ' ')';
$']'        =  $' ' ']';
$'>'        =  $' ' '>';
/* ==================================================================================================================== */
FnArgList   =  epsilon . *IncCounter() (*Expr | (epsilon) . thx . *Shift('TT_NUL', thx)) FENCE(*FnArgTail | epsilon);
FnArgTail   =  $',' . *IncCounter() (*Expr | (epsilon) . thx . *Shift('TT_NUL', thx)) FENCE(*FnArgTail | epsilon);
ExprList    =  epsilon . *PushCounter()
               *XList
               . *Reduce('ExprList', *(GT(nTop(), 1) nTop()))
               . *PopCounter();
XList       =  epsilon . *IncCounter() (*Expr | (epsilon) . thx . *Shift('', thx)) FENCE($',' *XList | epsilon);
Expr        =  *Expr0;
Expr0       =  *Expr1 FENCE($'=' *Expr0 . *Reduce('TT_ASSIGN', 2) | $'  ' '=' (epsilon) . thx . *Shift('TT_QLIT', thx) . *Reduce('TT_ASSIGN', 2) | epsilon);
Expr1       =  *Expr2 FENCE($'?' *Expr1 . *Reduce('TT_SCAN', 2) | epsilon);
Expr2       =  *Expr3 FENCE($'&' *Expr2 . *Reduce('TT_SEQ', 2) | epsilon);
/* PST-SN4-SC-4 (2026-05-19): replaced all foldop chains with pure shift/reduce.
   Expr3 (|/TT_ALT) and Expr4 (space/TT_SEQ): n-ary flat collect via nPush/nInc/X/nPop.
   Expr6-Expr10 binary arithmetic: right-recursive reduce(tag,2); lower flattens later.
   All *cont helper rules deleted. */
Expr3       =  epsilon . *PushCounter() *X3  . *Reduce('TT_ALT', *(GT(nTop(), 1) nTop())) . *PopCounter();
X3          =  epsilon . *IncCounter() *Expr4 FENCE($'|'  *X3 | epsilon);
Expr4       =  epsilon . *PushCounter() *X4  . *Reduce('TT_SEQ', *(GT(nTop(), 1) nTop())) . *PopCounter();
X4          =  epsilon . *IncCounter() *Expr5 FENCE($'  ' *X4 | epsilon);
Expr5       =  *Expr6 FENCE($'@' *Expr5 . *Reduce('TT_CAPT_CURSOR', 2) | epsilon);
Expr6       =  *Expr7
               FENCE($'+' *Expr6 . *Reduce('TT_ADD', 2) | $'-' *Expr6 . *Reduce('TT_SUB', 2) | epsilon);
Expr7       =  *Expr8 FENCE($'#' *Expr7 . *Reduce('TT_MUL', 2) | epsilon);
Expr8       =  *Expr9 FENCE($'/' *Expr8 . *Reduce('TT_DIV', 2) | epsilon);
Expr9       =  *Expr10 FENCE($'*' *Expr9 . *Reduce('TT_MUL', 2) | epsilon);
Expr10      =  *Expr11 FENCE($'%' *Expr10 . *Reduce('TT_DIV', 2) | epsilon);
Expr10      =  *Expr11 FENCE($'%' *Expr10 . *Reduce('TT_DIV', 2) | epsilon);
/* SCT-9g-snobol4 n-ary rewrite (2026-05-17): exponentiation n-ary flat, lowerer right-folds.
   a^b^c => TT_POW(a,b,c); lower_sno.c / sm_lower.c right-fold to a^(b^c).
   Uses nPush/nInc/X11/nPop pattern (same as snocone X3/X4) to collect all base/exponent
   operands in left-to-right order, then reduce to flat n-ary node. */
Expr11      =  epsilon . *PushCounter() *X11 . *Reduce('TT_POW', *(GT(nTop(), 1) nTop())) . *PopCounter();
X11         =  epsilon . *IncCounter() *Expr12 FENCE(($'^' | $'!' | $'**') *X11 | epsilon);
Expr12      =  *Expr13 *Expr12tail;
Expr12tail  =  FENCE($'$' *Expr13 . *Reduce('TT_CAPT_IMMED_ASGN', 2) *Expr12tail | $'.' *Expr13 . *Reduce('TT_CAPT_COND_ASGN', 2) *Expr12tail | epsilon);
Expr13      =  *Expr14 FENCE($'~' *Expr13 . *Reduce('TT_NOT', 2) | epsilon);
Expr14      =  '@' *Expr14 . *Reduce('TT_CAPT_CURSOR', 1)
            |  '~' *Expr14 . *Reduce('TT_NOT', 1)
            |  '?' *Expr14 . *Reduce('TT_INTERROGATE', 1)
            |  '&' (*ProtKwd) . thx . *Shift('TT_KEYWORD', thx)
            |  '&' (*Id) . thx . *Shift('TT_KEYWORD', thx)
            |  '+' *Expr14 . *Reduce('TT_PLS', 1)
            |  '-' *Expr14 . *Reduce('TT_MNS', 1)
            |  '*' *Expr14 . *Reduce('TT_DEFER', 1)
            |  '$' *Expr14 . *Reduce('TT_INDIRECT', 1)
            |  '.' *Expr14 . *Reduce('TT_NAME', 1)
            |  ('!' | '^') *Expr14 . *Reduce('TT_POW', 1)
            |  '%' *Expr14 . *Reduce('TT_DIV', 1)
            |  '/' *Expr14 . *Reduce('TT_DIV', 1)
            |  '#' *Expr14 . *Reduce('TT_MUL', 1)
            |  '=' *Expr14 . *Reduce('TT_ASSIGN', 1)
            |  '|' *Expr14 . *Reduce('TT_OPSYN', 1)
            |  *Expr15;
Expr15      =  *Expr17
               FENCE(epsilon . *PushCounter() *Expr16 . *Reduce('TT_IDX', nTop() + 1) . *PopCounter() | epsilon);
Expr16      =  epsilon . *IncCounter()
               ($'[' *ExprList $']' | $'<' *ExprList $'>')
               FENCE(*Expr16 | epsilon);
Expr17      =  FENCE(
                  epsilon . *PushCounter() $'(' *ExprList $')' . *Reduce('()', 1) . *PopCounter()
               |  *PrimLEN    $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_LEN', nTop())    . *PopCounter() $')'
               |  *PrimBREAK  $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_BREAK', nTop())  . *PopCounter() $')'
               |  *PrimSPAN   $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_SPAN', nTop())   . *PopCounter() $')'
               |  *PrimANY    $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_ANY', nTop())    . *PopCounter() $')'
               |  *PrimNOTANY $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_NOTANY', nTop()) . *PopCounter() $')'
               |  *PrimFENCE  $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_FENCE', nTop())  . *PopCounter() $')'
               |  *PrimARBNO  $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_ARBNO', nTop())  . *PopCounter() $')'
               |  *PrimPOS    $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_POS', nTop())    . *PopCounter() $')'
               |  *PrimRPOS   $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_RPOS', nTop())   . *PopCounter() $')'
               |  *PrimTAB    $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_TAB', nTop())    . *PopCounter() $')'
               |  *PrimRTAB   $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_RTAB', nTop())   . *PopCounter() $')'
               |  *PrimBREAKX $'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_BREAKX', nTop()) . *PopCounter() $')'
               |  (*Function) . thx . *Shift('TT_VAR', thx) FENCE(epsilon . *PushCounter() $'(' FENCE(*FnArgList | epsilon) . *Reduce('TT_FNC', nTop() + 1) . *PopCounter() $')' | epsilon)
               |  (*Id) . thx . *Shift('TT_VAR', thx) FENCE(epsilon . *PushCounter() $'(' FENCE(*FnArgList | epsilon) . *Reduce('TT_FNC', nTop() + 1) . *PopCounter() $')' | epsilon)
               |  *String
               |  (*Real) . thx . *Shift('TT_RLIT', thx)
               |  (*Integer) . thx . *Shift('TT_ILIT', thx)
               );
PrimLEN     =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('LEN ',    TxInList);
PrimBREAK   =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('BREAK ',  TxInList);
PrimSPAN    =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('SPAN ',   TxInList);
PrimANY     =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('ANY ',    TxInList);
PrimNOTANY  =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('NOTANY ', TxInList);
PrimFENCE   =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('FENCE ',  TxInList);
PrimARBNO   =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('ARBNO ',  TxInList);
PrimPOS     =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('POS ',    TxInList);
PrimRPOS    =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('RPOS ',   TxInList);
PrimTAB     =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('TAB ',    TxInList);
PrimRTAB    =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('RTAB ',   TxInList);
PrimBREAKX  =  SPAN('.' digits &UCASE '_' &LCASE) $ tx $ *sn_match('BREAKX ', TxInList);
SGoto       =  ('S' | 's');
FGoto       =  ('F' | 'f');
Target      =  $'(' . *assign(.Brackets, *'()') *Expr $')'
            |  $'<' . *assign(.Brackets, *'<>') *Expr $'>';
Sgo         =  *SGoto $' ' *Target . *Reduce('TT_GOTO_S', 1);
Fgo         =  *FGoto $' ' *Target . *Reduce('TT_GOTO_F', 1);
Ugo         =  *Target . *Reduce('TT_GOTO_U', 1);
Goto        =  $' ' ':'
               $' '
               FENCE(
                  *Ugo (epsilon) . thx . *Shift('', thx)
               |  *Sgo FENCE($' ' (':' $' ' | epsilon) *Fgo | (epsilon) . thx . *Shift('', thx))
               |  *Fgo FENCE($' ' (':' $' ' | epsilon) *Sgo | (epsilon) . thx . *Shift('', thx))
               );
Control     =  '-' BREAK(nl ';');
Comment     =  '*' BREAK(nl);
/* PST-SN4-2 (2026-05-16): Stmt redesigned to emit TT_STMT directly as a pure syntax tree.
   Children in source order: TT_LABEL? subject? TT_PAT? TT_EQ? replacement? goto*.
   No post-parse cooking.  Counter tracks child count for reduce('TT_STMT', nTop()). */
StmtLabel   =  (BREAK(' ' tab nl ';') | ARBNO(NOTANY(' ' tab nl ';')) RPOS(0)) . thx . *Shift('TT_LABEL', thx);
StmtRepl    =  $'=' $' ' *Expr . *Reduce('TT_EQ', 2)
            |  $'  ' '=' $' ' (epsilon) . thx . *Shift('TT_EQ', thx);
StmtGoto    =  FENCE(*Goto . *IncCounter() . *IncCounter() | epsilon);
Stmt        =  epsilon . *PushCounter()
               FENCE(epsilon . *IncCounter() *StmtLabel | epsilon)
               FENCE(
                  $'  '
                  . *IncCounter() *Expr14
                  $'?'
                  . *IncCounter() *Expr1 . *Reduce('TT_PAT', 1)
                  FENCE(*StmtRepl | epsilon)
               |  $'  '
                  . *IncCounter() *Expr1
                  FENCE(*StmtRepl | epsilon)
               |  epsilon
               )
               *StmtGoto
               . *Reduce('TT_STMT', nTop())
               . *PopCounter()
               $' ';
Commands    =  *Command FENCE(*Commands | epsilon);
Command     =  FENCE(
                  (*Comment) . thx . *Shift('TT_COMMENT', thx) . *IncCounter() . *Reduce('TT_COMMENT', 1) nl
               |  (*Control) . thx . *Shift('TT_CONTROL', thx) . *IncCounter() . *Reduce('TT_CONTROL', 1) (nl | ';')
               |  *Stmt . *IncCounter() (nl | ';' | RPOS(0))
               );
Compiland   =  epsilon . *PushCounter()
               POS(0) ARBNO(*Command FLUSH) ('END' (ANY(' ' tab nl) | RPOS(0)) ARB | epsilon) RPOS(0)
               . *Reduce('Parse', nTop())
               . *PopCounter();
/* ==================================================================================================================== */
function ParseOne(ptree, i, nk, cmd) {
    InitCounter();
    InitStack();
    if (Src ? Compiland) {
        /* SCT-fix: $'[' and $']' are OPSYN binary operators (Expr16) that override
         * SPITBOL's built-in array-indexing brackets.  Use ITEM(array, index) which
         * is standard SNOBOL4 and not affected by OPSYN redefinition of '['. */
        ptree = Pop();
        i = 1;
        nk = n(ptree);
        while (LE(i, nk)) {
            cmd = ITEM(c(ptree), i);
            if (IDENT(t(cmd), 'TT_STMT')) { TreeDump(cmd); }
            i = i + 1;
        }
    } else OUTPUT = 'Parse Error.';
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
