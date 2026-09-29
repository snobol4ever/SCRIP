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
/* PST-SN4-2 (2026-05-16): sn_upr is the one tokenizer helper -- it builds no tree nodes; all stmt-building helpers
   (pp_stmt, strip_parens, make_goto_slot, push_qlit) are deleted and the grammar builds TT_STMT directly.
   Keyword classes are TABLE lookups, each table sized exactly to its list (Lon 2026-09-29: "use simple TABLE() lookups
   with exactly sized TABLE parameters") -- the upper-cased token is looked up once; the old list scan
   re-evaluated *sn_upr(tx) at every start position of an unanchored match over the whole list. */
function sn_upr(s)                  { sn_upr   = REPLACE(s, &LCASE, &UCASE); return; }
FunctionsT   = TABLE(123);
UnprotKwdsT  = TABLE(21);
ProtKwdsT    = TABLE(28);
BuiltinVarsT = TABLE(7);
SpecialNmsT  = TABLE(8);
kw_s = Functions;    while (kw_s ? (POS(0) BREAK(' ') . kw_w ' ' REM . kw_s)) { FunctionsT[kw_w] = 1; }
kw_s = UnprotKwds;   while (kw_s ? (POS(0) BREAK(' ') . kw_w ' ' REM . kw_s)) { UnprotKwdsT[kw_w] = 1; }
kw_s = ProtKwds;     while (kw_s ? (POS(0) BREAK(' ') . kw_w ' ' REM . kw_s)) { ProtKwdsT[kw_w] = 1; }
kw_s = BuiltinVars;  while (kw_s ? (POS(0) BREAK(' ') . kw_w ' ' REM . kw_s)) { BuiltinVarsT[kw_w] = 1; }
kw_s = SpecialNms;   while (kw_s ? (POS(0) BREAK(' ') . kw_w ' ' REM . kw_s)) { SpecialNmsT[kw_w] = 1; }
Function    =  SPAN('.' '0123456789' &UCASE '_' &LCASE) $ tx *DIFFER(FunctionsT[sn_upr(tx)]);
BuiltinVar  =  SPAN('.' '0123456789' &UCASE '_' &LCASE) $ tx *DIFFER(BuiltinVarsT[sn_upr(tx)]);
SpecialNm   =  SPAN('.' '0123456789' &UCASE '_' &LCASE) $ tx *DIFFER(SpecialNmsT[sn_upr(tx)]);
ProtKwd     =  SPAN(&UCASE &LCASE)                $ tx *DIFFER(ProtKwdsT[sn_upr(tx)]);
UnprotKwd   =  SPAN(&UCASE &LCASE)                $ tx *DIFFER(UnprotKwdsT[sn_upr(tx)]);
Integer     =  SPAN('0123456789');
DQ          =  '"' (BREAK('"' CHAR(10))) . thx . *Shift('TT_QLIT', thx) '"';
SQ          =  "'" (BREAK("'" CHAR(10))) . thx . *Shift('TT_QLIT', thx) "'";
String      =  *SQ | *DQ;
Real        =  (  SPAN('0123456789')
                  ('.' FENCE(SPAN('0123456789') | epsilon) | epsilon)
                  ('E' | 'e')
                  ('+' | '-' | epsilon)
                  SPAN('0123456789')
               |  SPAN('0123456789') '.' FENCE(SPAN('0123456789') | epsilon)
               );
Id          =  ANY(&UCASE &LCASE)
               FENCE(SPAN('.' '0123456789' &UCASE '_' &LCASE) | epsilon);
White       =  (  SPAN(' ' CHAR(9))
                  FENCE(CHAR(10) ('+' | '.') FENCE(SPAN(' ' CHAR(9)) | epsilon) | epsilon)
               |  CHAR(10) ('+' | '.') FENCE(SPAN(' ' CHAR(9)) | epsilon)
               );
Gray        =  *White | epsilon;
$'  '       =  White;
$' '        =  Gray;
$'='        =  *$'  ' '='  *$'  ';
$'?'        =  *$'  ' '?'  *$'  ';
$'|'        =  *$'  ' '|'  *$'  ';
$'+'        =  *$'  ' '+'  *$'  ';
$'-'        =  *$'  ' '-'  *$'  ';
$'/'        =  *$'  ' '/'  *$'  ';
$'*'        =  *$'  ' '*'  *$'  ';
$'^'        =  *$'  ' '^'  *$'  ';
$'!'        =  *$'  ' '!'  *$'  ';
$'**'       =  *$'  ' '**' *$'  ';
$'$'        =  *$'  ' '$'  *$'  ';
$'.'        =  *$'  ' '.'  *$'  ';
$'&'        =  *$'  ' '&'  *$'  ';
$'@'        =  *$'  ' '@'  *$'  ';
$'#'        =  *$'  ' '#'  *$'  ';
$'%'        =  *$'  ' '%'  *$'  ';
$'~'        =  *$'  ' '~'  *$'  ';
$','        =  *$' ' ',' *$' ';
$'('        =  '(' *$' ';
$'['        =  '[' *$' ';
$'<'        =  '<' *$' ';
$')'        =  *$' ' ')';
$']'        =  *$' ' ']';
$'>'        =  *$' ' '>';
/* ==================================================================================================================== */
FnArgList   =  epsilon . *IncCounter() (*Expr | (epsilon) . thx . *Shift('TT_NUL', thx)) FENCE(*FnArgTail | epsilon);
FnArgTail   =  *$',' . *IncCounter() (*Expr | (epsilon) . thx . *Shift('TT_NUL', thx)) FENCE(*FnArgTail | epsilon);
ExprList    =  epsilon . *PushCounter()
               *XList
               . *Reduce('ExprList', *(GT(nTop(), 1) nTop()))
               . *PopCounter();
XList       =  epsilon . *IncCounter() (*Expr | (epsilon) . thx . *Shift('', thx)) FENCE(*$',' *XList | epsilon);
Expr        =  *Expr0;
Expr0       =  *Expr1 FENCE(*$'=' *Expr0 . *Reduce('TT_ASSIGN', 2) | *$'  ' '=' (epsilon) . thx . *Shift('TT_QLIT', thx) . *Reduce('TT_ASSIGN', 2) | epsilon);
Expr1       =  *Expr2 FENCE(*$'?' *Expr1 . *Reduce('TT_SCAN', 2) | epsilon);
Expr2       =  *Expr3 FENCE(*$'&' *Expr2 . *Reduce('TT_SEQ', 2) | epsilon);
/* PST-SN4-SC-4 (2026-05-19): replaced all foldop chains with pure shift/reduce.
   Expr3 (|/TT_ALT) and Expr4 (space/TT_SEQ): n-ary flat collect via nPush/nInc/X/nPop.
   Expr6-Expr10 binary arithmetic: right-recursive reduce(tag,2); lower flattens later.
   All *cont helper rules deleted. */
Expr3       =  epsilon . *PushCounter() *X3  . *Reduce('TT_ALT', *(GT(nTop(), 1) nTop())) . *PopCounter();
X3          =  epsilon . *IncCounter() *Expr4 FENCE(*$'|'  *X3 | epsilon);
Expr4       =  epsilon . *PushCounter() *X4  . *Reduce('TT_SEQ', *(GT(nTop(), 1) nTop())) . *PopCounter();
X4          =  epsilon . *IncCounter() *Expr5 FENCE(*$'  ' *X4 | epsilon);
Expr5       =  *Expr6 FENCE(*$'@' *Expr5 . *Reduce('TT_CAPT_CURSOR', 2) | epsilon);
Expr6       =  *Expr7
               FENCE(*$'  ' ('+' *$'  ' *Expr6 . *Reduce('TT_ADD', 2) | '-' *$'  ' *Expr6 . *Reduce('TT_SUB', 2) ) | epsilon);
Expr7       =  *Expr8 FENCE(*$'#' *Expr7 . *Reduce('TT_MUL', 2) | epsilon);
Expr8       =  *Expr9 FENCE(*$'/' *Expr8 . *Reduce('TT_DIV', 2) | epsilon);
Expr9       =  *Expr10 FENCE(*$'*' *Expr9 . *Reduce('TT_MUL', 2) | epsilon);
Expr10      =  *Expr11 FENCE(*$'%' *Expr10 . *Reduce('TT_DIV', 2) | epsilon);
/* SCT-9g-snobol4 n-ary rewrite (2026-05-17): exponentiation n-ary flat, lowerer right-folds.
   a^b^c => TT_POW(a,b,c); lower_sno.c / sm_lower.c right-fold to a^(b^c).
   Uses nPush/nInc/X11/nPop pattern (same as snocone X3/X4) to collect all base/exponent
   operands in left-to-right order, then reduce to flat n-ary node. */
Expr11      =  epsilon . *PushCounter() *X11 . *Reduce('TT_POW', *(GT(nTop(), 1) nTop())) . *PopCounter();
X11         =  epsilon . *IncCounter() *Expr12 FENCE((*$'  ' ('^' *$'  ' | '!' *$'  ' | '**' *$'  ')) *X11 | epsilon);
Expr12      =  *Expr13 *Expr12tail;
Expr12tail  =  FENCE(*$'  ' ('$' *$'  ' *Expr13 . *Reduce('TT_CAPT_IMMED_ASGN', 2) *Expr12tail | '.' *$'  ' *Expr13 . *Reduce('TT_CAPT_COND_ASGN', 2) *Expr12tail ) | epsilon);
Expr13      =  *Expr14 FENCE(*$'~' *Expr13 . *Reduce('TT_NOT', 2) | epsilon);
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
               (*$'[' *ExprList *$']' | *$'<' *ExprList *$'>')
               FENCE(*Expr16 | epsilon);
PrimLEN_rest    =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_LEN', nTop())    . *PopCounter() *$')';
PrimBREAK_rest  =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_BREAK', nTop())  . *PopCounter() *$')';
PrimSPAN_rest   =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_SPAN', nTop())   . *PopCounter() *$')';
PrimANY_rest    =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_ANY', nTop())    . *PopCounter() *$')';
PrimNOTANY_rest =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_NOTANY', nTop()) . *PopCounter() *$')';
PrimFENCE_rest  =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_FENCE', nTop())  . *PopCounter() *$')';
PrimARBNO_rest  =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_ARBNO', nTop())  . *PopCounter() *$')';
PrimPOS_rest    =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_POS', nTop())    . *PopCounter() *$')';
PrimRPOS_rest   =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_RPOS', nTop())   . *PopCounter() *$')';
PrimTAB_rest    =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_TAB', nTop())    . *PopCounter() *$')';
PrimRTAB_rest   =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_RTAB', nTop())   . *PopCounter() *$')';
PrimBREAKX_rest =  *$'(' . *PushCounter() FENCE(*FnArgList | epsilon) . *Reduce('TT_BREAKX', nTop()) . *PopCounter() *$')';
PrimT       =  TABLE(12);
prim_call   =  SPAN('.' '0123456789' &UCASE '_' &LCASE) $ tx *DIFFER(PrimT[sn_upr(tx)]) *PrimT[sn_upr(tx)];
Expr17      =  FENCE(
                  epsilon . *PushCounter() *$'(' *ExprList *$')' . *Reduce('()', 1) . *PopCounter()
               |  *prim_call
               |  (*Function) . thx . *Shift('TT_VAR', thx) FENCE(epsilon . *PushCounter() *$'(' FENCE(*FnArgList | epsilon) . *Reduce('TT_FNC', nTop() + 1) . *PopCounter() *$')' | epsilon)
               |  (*Id) . thx . *Shift('TT_VAR', thx) FENCE(epsilon . *PushCounter() *$'(' FENCE(*FnArgList | epsilon) . *Reduce('TT_FNC', nTop() + 1) . *PopCounter() *$')' | epsilon)
               |  *String
               |  (*Real) . thx . *Shift('TT_RLIT', thx)
               |  (*Integer) . thx . *Shift('TT_ILIT', thx)
               );
SGoto       =  ('S' | 's');
FGoto       =  ('F' | 'f');
Target      =  *$'(' . *assign(.Brackets, *'()') *Expr *$')'
            |  *$'<' . *assign(.Brackets, *'<>') *Expr *$'>';
Sgo         =  *SGoto *$' ' *Target . *Reduce('TT_GOTO_S', 1);
Fgo         =  *FGoto *$' ' *Target . *Reduce('TT_GOTO_F', 1);
Ugo         =  *Target . *Reduce('TT_GOTO_U', 1);
Goto        =  *$' ' ':'
               *$' '
               FENCE(
                  *Ugo (epsilon) . thx . *Shift('', thx)
               |  *Sgo FENCE(*$' ' (':' *$' ' | epsilon) *Fgo | (epsilon) . thx . *Shift('', thx))
               |  *Fgo FENCE(*$' ' (':' *$' ' | epsilon) *Sgo | (epsilon) . thx . *Shift('', thx))
               );
Control     =  '-' BREAK(CHAR(10) ';');
Comment     =  '*' BREAK(CHAR(10));
/* PST-SN4-2 (2026-05-16): Stmt redesigned to emit TT_STMT directly as a pure syntax tree.
   Children in source order: TT_LABEL? subject? TT_PAT? TT_EQ? replacement? goto*.
   No post-parse cooking.  Counter tracks child count for reduce('TT_STMT', nTop()). */
StmtLabel   =  (BREAK(' ' CHAR(9) CHAR(10) ';') | ARBNO(NOTANY(' ' CHAR(9) CHAR(10) ';')) RPOS(0)) . thx . *Shift('TT_LABEL', thx);
StmtRepl    =  *$'=' *$' ' *Expr . *Reduce('TT_EQ', 2)
            |  *$'  ' '=' *$' ' (epsilon) . thx . *Shift('TT_EQ', thx);
StmtGoto    =  FENCE(*Goto . *IncCounter() . *IncCounter() | epsilon);
Stmt        =  epsilon . *PushCounter()
               FENCE(epsilon . *IncCounter() *StmtLabel | epsilon)
               FENCE(
                  *$'  '
                  . *IncCounter() *Expr14
                  *$'?'
                  . *IncCounter() *Expr1 . *Reduce('TT_PAT', 1)
                  FENCE(*StmtRepl | epsilon)
               |  *$'  '
                  . *IncCounter() *Expr1
                  FENCE(*StmtRepl | epsilon)
               |  epsilon
               )
               *StmtGoto
               . *Reduce('TT_STMT', nTop())
               . *PopCounter()
               *$' ';
Commands    =  *Command FENCE(*Commands | epsilon);
Command     =  FENCE(
                  (*Comment) . thx . *Shift('TT_COMMENT', thx) . *IncCounter() . *Reduce('TT_COMMENT', 1) CHAR(10)
               |  (*Control) . thx . *Shift('TT_CONTROL', thx) . *IncCounter() . *Reduce('TT_CONTROL', 1) (CHAR(10) | ';')
               |  *Stmt . *IncCounter() (CHAR(10) | ';' | RPOS(0))
               );
Compiland   =  epsilon . *PushCounter()
               POS(0) ARBNO(*Command FLUSH) ('END' (ANY(' ' CHAR(9) CHAR(10)) | RPOS(0)) ARB | epsilon) RPOS(0)
               . *Reduce('Parse', nTop())
               . *PopCounter();
PrimT['LEN'] = PrimLEN_rest; PrimT['BREAK'] = PrimBREAK_rest; PrimT['SPAN'] = PrimSPAN_rest; PrimT['ANY'] = PrimANY_rest;
PrimT['NOTANY'] = PrimNOTANY_rest; PrimT['FENCE'] = PrimFENCE_rest; PrimT['ARBNO'] = PrimARBNO_rest; PrimT['POS'] = PrimPOS_rest;
PrimT['RPOS'] = PrimRPOS_rest; PrimT['TAB'] = PrimTAB_rest; PrimT['RTAB'] = PrimRTAB_rest; PrimT['BREAKX'] = PrimBREAKX_rest;
/* ==================================================================================================================== */
function ParseOne(ptree, i, nk, cmd) {
    pf_a = TIME();
    InitCounter();
    InitStack();
    if (Src ? *Compiland) {
        /* SCT-fix: $'[' and $']' are OPSYN binary operators (Expr16) that override
         * SPITBOL's built-in array-indexing brackets.  Use ITEM(array, index) which
         * is standard SNOBOL4 and not affected by OPSYN redefinition of '['. */
        ptree = Pop();
        pf_parse = pf_parse + (TIME() - pf_a);
        i = 1;
        nk = n(ptree);
        while (LE(i, nk)) {
            cmd = ITEM(c(ptree), i);
            if (IDENT(t(cmd), 'TT_STMT')) { TreeDump(cmd); }
            i = i + 1;
        }
    } else { pf_parse = pf_parse + (TIME() - pf_a); OUTPUT = 'Parse Error.'; }
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
