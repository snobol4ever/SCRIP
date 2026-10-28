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
/* -CASE control lines as snobol4.l reads them: honoured when folding is already on or the line is spelled -CASE; the digits */
/* after the blanks turn folding on when nonzero, anything else turns it off. The digits are captured twice: SnCaseNow at    */
/* scan time (the $ assignment, read by the next control line's guard) and SnCase at replay time (the . assignment, read by  */
/* every Shift that folds -- the deferred assignments replay in source order, so each token sees the state written before   */
/* it). Identifiers, labels, keywords, function names and goto targets fold as sno_fold does; strings never (case1.sno).     */
SnCaseNow = ;
SnCase    = ;
SnLblT    = TABLE(256);
SnLT        =  TABLE(10);
SnUT        =  TABLE(10);
SnLT[''] = 'a'; SnUT[''] = 'a';
SnLT['1'] = &LCASE; SnLT['2'] = &LCASE; SnLT['3'] = &LCASE; SnLT['4'] = &LCASE; SnLT['5'] = &LCASE; SnLT['6'] = &LCASE; SnLT['7'] = &LCASE; SnLT['8'] = &LCASE; SnLT['9'] = &LCASE;
SnUT['1'] = &UCASE; SnUT['2'] = &UCASE; SnUT['3'] = &UCASE; SnUT['4'] = &UCASE; SnUT['5'] = &UCASE; SnUT['6'] = &UCASE; SnUT['7'] = &UCASE; SnUT['8'] = &UCASE; SnUT['9'] = &UCASE;
/* the captured key is the first significant digit (atoi nonzero: '10' and '007' fold, '0' and '' do not); the two tables map */
/* it to the REPLACE character sets, the null key to an identity pair, so no selection sits inside a deferred expression      */
CaseCtl     =  ( '-CASE' | *NE(SnCaseNow, 0) '-' ANY('Cc') ANY('Aa') ANY('Ss') ANY('Ee') )
               ( ANY(' ' CHAR(9)) (SPAN(' ' CHAR(9)) | epsilon) (SPAN('0') | epsilon) ((ANY('123456789') | epsilon) $ SnCaseNow . SnCase) BREAK(CHAR(10) ';')
               | @cq (*IDENT(SUBSTR(Src, cq + 1, 1), CHAR(10)) | *IDENT(SUBSTR(Src, cq + 1, 1), ';') | RPOS(0)) (epsilon $ SnCaseNow . SnCase)
               );
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
Function    =  SPAN('.' '0123456789' &UCASE '_' &LCASE) $ tx *DIFFER(FunctionsT[REPLACE(tx, &LCASE, &UCASE)]);
BuiltinVar  =  SPAN('.' '0123456789' &UCASE '_' &LCASE) $ tx *DIFFER(BuiltinVarsT[REPLACE(tx, &LCASE, &UCASE)]);
SpecialNm   =  SPAN('.' '0123456789' &UCASE '_' &LCASE) $ tx *DIFFER(SpecialNmsT[REPLACE(tx, &LCASE, &UCASE)]);
UnprotKwd   =  SPAN(&UCASE &LCASE)                $ tx *DIFFER(UnprotKwdsT[REPLACE(tx, &LCASE, &UCASE)]);
Integer     =  SPAN('0123456789');
DQ          =  '"' (BREAK('"' CHAR(10))) . thx . *Shift('TT_QLIT', thx) '"';
SQ          =  "'" (BREAK("'" CHAR(10))) . thx . *Shift('TT_QLIT', thx) "'";
String      =  *SQ | *DQ;
Real        =  (  SPAN('0123456789')
                  FENCE('.' FENCE(SPAN('0123456789') | epsilon) | epsilon)
                  ('E' | 'e')
                  FENCE('+' | '-' | epsilon)
                  SPAN('0123456789')
               |  SPAN('0123456789') '.' FENCE(SPAN('0123456789') | epsilon)
               );
/* snobol4.l ALPHA [A-Za-z\x80-\xFF] and IDCONT: the bytes above 127 are letters to the C lexer (8bit2.sno) */
HiBytes     =  SUBSTR(&ALPHABET, 129, 128);
Id          =  ANY(&UCASE &LCASE HiBytes)
               FENCE(SPAN('.' '0123456789' &UCASE '_' &LCASE HiBytes) | epsilon);
/* snobol4.l W ({WS}|{CONT})+ with CONT \n[+.][ \t]*: continuation lines repeat, a bare '+' line among them (rc.sno) */
Cont        =  CHAR(10) ANY('+.') FENCE(SPAN(' ' CHAR(9)) | epsilon) FENCE(*Cont | epsilon);
White       =  (  SPAN(' ' CHAR(9)) FENCE(*Cont | epsilon)
               |  *Cont
               );
Gray        =  FENCE(*White | epsilon);
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
/* THE EXPRESSION, as src/parsers/snobol4/snobol4.y builds it (Lon 2026-09-30: the C tree is canonical, and "the tree is */
/* built from tokens in the same order as they are recognized by the PATTERN ... directly and once only"): every left-     */
/* associative level is a tail loop that reduces as each right operand is recognised; =, ^, @ and ~ are right-recursive.  */
ArgTail     =  *$',' FENCE(*Expr | epsilon . *Reduce('TT_NUL', 0)) . *IncCounter() FENCE(*ArgTail | epsilon);
ArgList     =  FENCE(*Expr . *IncCounter() FENCE(*ArgTail | epsilon) | epsilon . *Reduce('TT_NUL', 0) . *IncCounter() *ArgTail);
Expr        =  *Expr0;
Expr0       =  *Expr1 FENCE(*$'=' *Expr0 . *ReduceV('TT_ASSIGN', 2) | *$'  ' '=' (epsilon) . thx . *Shift('TT_QLIT', thx) . *ReduceV('TT_ASSIGN', 2) | epsilon);
Expr1       =  *Expr2 *Expr1t;
Expr1t      =  FENCE(*$'?' *Expr2 . *Reduce('TT_SCAN', 2) *Expr1t | epsilon);
Expr2       =  *Expr3 *Expr2t;
Expr2t      =  FENCE(*$'&' *Expr3 . *Reduce('TT_OPSYN', 2, '&') *Expr2t | epsilon);
Expr3       =  *Expr4 *Expr3t;
Expr3t      =  FENCE(*$'|' *Expr4 . *Reduce('TT_ALT', 2) *Expr3t | epsilon);
Expr4       =  *Expr5 *Expr4t;
Expr4t      =  FENCE(*$'  ' *Expr5 . *Reduce('TT_SEQ', 2) *Expr4t | epsilon);
Expr5       =  *Expr6 FENCE(*$'@' *Expr5 . *Reduce('TT_OPSYN', 2, '@') | epsilon);
Expr6       =  *Expr7 *Expr6t;
Expr6t      =  FENCE(*$'  ' ('+' *$'  ' *Expr7 . *ReduceV('TT_ADD', 2) | '-' *$'  ' *Expr7 . *ReduceV('TT_SUB', 2)) *Expr6t | epsilon);
Expr7       =  *Expr8 *Expr7t;
Expr7t      =  FENCE(*$'#' *Expr8 . *Reduce('TT_OPSYN', 2, '#') *Expr7t | epsilon);
Expr8       =  *Expr9 *Expr8t;
Expr8t      =  FENCE(*$'/' *Expr9 . *ReduceV('TT_DIV', 2) *Expr8t | epsilon);
Expr9       =  *Expr10 *Expr9t;
Expr9t      =  FENCE(*$'*' *Expr10 . *ReduceV('TT_MUL', 2) *Expr9t | epsilon);
Expr10      =  *Expr11 *Expr10t;
Expr10t     =  FENCE(*$'%' *Expr11 . *Reduce('TT_OPSYN', 2, '%') *Expr10t | epsilon);
Expr11      =  *Expr12 FENCE(*$'  ' ('**' | '^' | '!') *$'  ' *Expr11 . *ReduceV('TT_POW', 2) | epsilon);
Expr12      =  *Expr13 *Expr12tail;
Expr12tail  =  FENCE(*$'  ' ('$' *$'  ' *Expr13 . *ReduceV('TT_CAPT_IMMED_ASGN', 2) *Expr12tail | '.' *$'  ' *Expr13 . *ReduceV('TT_CAPT_COND_ASGN', 2) *Expr12tail ) | epsilon);
Expr13      =  *Expr14 FENCE(*$'~' *Expr13 . *Reduce('TT_OPSYN', 2, '~') | epsilon);
Expr14      =  '@' *Expr14 . *ReduceV('TT_CAPT_CURSOR', 1)
            |  '~' *Expr14 . *Reduce('TT_NOT', 1)
            |  '?' *Expr14 . *Reduce('TT_INTERROGATE', 1)
            |  '&' (*Id) . thx . *Shift('TT_KEYWORD', REPLACE(thx, SnLT[SnCase], SnUT[SnCase]))
            |  '&' *Expr14 . *ReduceV('TT_OPSYN', 1, '&')
            |  '+' *Expr14 . *ReduceV('TT_PLS', 1)
            |  '-' *Expr14 . *ReduceV('TT_MNS', 1)
            |  '*' *Expr14 . *Reduce('TT_DEFER', 1)
            |  '$' *Expr14 . *Reduce('TT_INDIRECT', 1)
            |  '.' *Expr14 . *Reduce('TT_NAME', 1)
            |  '!' *Expr14 . *Reduce('TT_OPSYN', 1, '!')
            |  '^' *Expr14 . *Reduce('TT_OPSYN', 1, '^')
            |  '%' *Expr14 . *Reduce('TT_OPSYN', 1, '%')
            |  '/' *Expr14 . *Reduce('TT_OPSYN', 1, '/')
            |  '#' *Expr14 . *Reduce('TT_OPSYN', 1, '#')
            |  '=' *Expr14 . *Reduce('TT_OPSYN', 1, '=')
            |  '|' *Expr14 . *Reduce('TT_OPSYN', 1, '|')
            |  *Expr15;
Expr15      =  *Expr17 *Expr15t;
Expr15t     =  FENCE(  epsilon . *PushCounter() . *IncCounter() *$'[' FENCE(*ArgList | epsilon) *$']' . *Reduce('TT_IDX', nTop()) . *PopCounter() *Expr15t
                     | epsilon . *PushCounter() . *IncCounter() *$'<' FENCE(*ArgList | epsilon) *$'>' . *Reduce('TT_IDX', nTop()) . *PopCounter() *Expr15t
                     | epsilon);
/* a call: the name is held (PushVal) when its '(' is recognised and SnoCall builds the ONE node -- TT_FNC <name>, or the  */
/* pattern primitive's own kind for snobol4.y pat_prim_kind's names (exact case), ARB BAL REM FAIL SUCCEED ABORT keeping it */
SnoPrimT    =  TABLE(31);
SnoPrimT['ANY'] = 'TT_ANY'; SnoPrimT['NOTANY'] = 'TT_NOTANY'; SnoPrimT['SPAN'] = 'TT_SPAN'; SnoPrimT['BREAK'] = 'TT_BREAK';
SnoPrimT['BREAKX'] = 'TT_BREAKX'; SnoPrimT['LEN'] = 'TT_LEN'; SnoPrimT['POS'] = 'TT_POS'; SnoPrimT['RPOS'] = 'TT_RPOS';
SnoPrimT['TAB'] = 'TT_TAB'; SnoPrimT['RTAB'] = 'TT_RTAB'; SnoPrimT['ARBNO'] = 'TT_ARBNO'; SnoPrimT['FENCE'] = 'TT_FENCE';
SnoPrimT['FLUSH'] = 'TT_FLUSH'; SnoPrimT['ARB'] = 'TT_ARB'; SnoPrimT['BAL'] = 'TT_BAL'; SnoPrimT['REM'] = 'TT_REM';
FoldPatNmT  =  TABLE(7); FoldPatNmT['ARB'] = 1; FoldPatNmT['BAL'] = 1; FoldPatNmT['REM'] = 1; FoldPatNmT['FAIL'] = 1; FoldPatNmT['SUCCEED'] = 1; FoldPatNmT['ABORT'] = 1; FoldPatNmT['FENCE'] = 1;
FoldPatNdT  =  TABLE(6); FoldPatNdT['TT_ARB'] = 1; FoldPatNdT['TT_BAL'] = 1; FoldPatNdT['TT_REM'] = 1; FoldPatNdT['TT_FAIL'] = 1; FoldPatNdT['TT_SUCCEED'] = 1; FoldPatNdT['TT_ABORT'] = 1;
FoldBinT    =  TABLE(5); FoldBinT['TT_ADD'] = 1; FoldBinT['TT_SUB'] = 1; FoldBinT['TT_MUL'] = 1; FoldBinT['TT_DIV'] = 1; FoldBinT['TT_POW'] = 1;
FoldCsetT   =  TABLE(5); FoldCsetT['TT_ANY'] = 'ANY'; FoldCsetT['TT_NOTANY'] = 'NOTANY'; FoldCsetT['TT_SPAN'] = 'SPAN'; FoldCsetT['TT_BREAK'] = 'BREAK'; FoldCsetT['TT_BREAKX'] = 'BREAKX';
FoldPatFnT  =  TABLE(12); FoldPatFnT['TT_ANY'] = 1; FoldPatFnT['TT_NOTANY'] = 1; FoldPatFnT['TT_SPAN'] = 1; FoldPatFnT['TT_BREAK'] = 1; FoldPatFnT['TT_BREAKX'] = 1; FoldPatFnT['TT_LEN'] = 1;
FoldPatFnT['TT_POS'] = 1; FoldPatFnT['TT_RPOS'] = 1; FoldPatFnT['TT_TAB'] = 1; FoldPatFnT['TT_RTAB'] = 1; FoldPatFnT['TT_ARBNO'] = 1; FoldPatFnT['TT_FENCE'] = 1;
SnoPrimT['FAIL'] = 'TT_FAIL'; SnoPrimT['SUCCEED'] = 'TT_SUCCEED'; SnoPrimT['ABORT'] = 'TT_ABORT';
SnoValT     =  TABLE(7);
SnoValT['ARB'] = 'ARB'; SnoValT['BAL'] = 'BAL'; SnoValT['REM'] = 'REM'; SnoValT['FAIL'] = 'FAIL'; SnoValT['SUCCEED'] = 'SUCCEED'; SnoValT['ABORT'] = 'ABORT';
/* the node's value (the name for TT_FNC and the six named primitives, none for the other primitives) is held under its    */
/* kind, chosen at scan time by the table test on the folded name, and the one Reduce pops the kind first and the value     */
/* second (arguments evaluate left to right)                                                                                */
Call        =  (*Id) $ tx . thx '('
               ( *DIFFER(SnoPrimT[REPLACE(tx, SnLT[SnCaseNow], SnUT[SnCaseNow])])
                 . *PushVal(SnoValT[REPLACE(thx, SnLT[SnCase], SnUT[SnCase])]) . *PushVal(SnoPrimT[REPLACE(thx, SnLT[SnCase], SnUT[SnCase])])
               | epsilon . *PushVal(REPLACE(thx, SnLT[SnCase], SnUT[SnCase])) . *PushVal('TT_FNC')
               )
               . *PushCounter() *$' ' FENCE(*ArgList | epsilon) *$')' . *Reduce(PopVal(), nTop(), PopVal()) . *PopCounter();
Expr17      =  FENCE(
                  *$'(' FENCE(  *$')' . *Reduce('TT_NUL', 0)
                             |  epsilon . *PushCounter() *Expr . *IncCounter() FENCE(*ArgTail *$')' . *Reduce('TT_VLIST', nTop()) | *$')') . *PopCounter()
                             |  epsilon . *PushCounter() . *Reduce('TT_NUL', 0) . *IncCounter() *ArgTail *$')' . *Reduce('TT_VLIST', nTop()) . *PopCounter()
                             )
               |  *Call
               |  (*Id) . thx . *Shift('TT_VAR', REPLACE(thx, SnLT[SnCase], SnUT[SnCase]))
               |  *String
               |  ((*Real) $ rtx *DIFFER(CONVERT(rtx, 'REAL'))) . thx . *Shift('TT_FLIT', thx)
               |  (*Integer) . thx . *Shift('TT_ILIT', '' (thx + 0))
               );
SGoto       =  ('S' | 's');
FGoto       =  ('F' | 'f');
/* a goto target as snobol4.y goto_label_expr: (L) and ($L) are the label's text, ($'s') the string, ($(e)) the           */
/* expression, ($A<i>) and ($A[i]) the subscript expression A<i> (goto_sub), (F(args)) a call, <e> TT_GOTO_DIRECT e         */
GoSub1      =  FENCE(  epsilon . *PushCounter() . *IncCounter() '[' FENCE(*ArgList | epsilon) ']' . *Reduce('TT_IDX', nTop()) . *PopCounter()
                    |  epsilon . *PushCounter() . *IncCounter() '<' FENCE(*ArgList | epsilon) '>' . *Reduce('TT_IDX', nTop()) . *PopCounter()
                    );
GoInner     =  FENCE(  '$' '(' *$' ' *Expr *$' ' ')'
                    |  '$' (*Id) . thx . *Shift('TT_VAR', REPLACE(thx, SnLT[SnCase], SnUT[SnCase])) *GoSub1 *Expr15t
                    |  '$' "'" (BREAK("'")) . thx . *Shift('TT_QLIT', thx) "'"
                    |  '$' '"' (BREAK('"')) . thx . *Shift('TT_QLIT', thx) '"'
                    |  '$' (*Id) . thx . *Shift('TT_QLIT', '$' REPLACE(thx, SnLT[SnCase], SnUT[SnCase]))
                    |  *Call
                    |  (*Id) . thx . *Shift('TT_QLIT', REPLACE(thx, SnLT[SnCase], SnUT[SnCase]))
                    );
/* a goto operand that is not one of those shapes is an expression (snobol4.y goto_label_expr XLPAREN expr0 XRPAREN, sno_goto_operand): the lowest-precedence operator present decides, a value form is wrapped in */
/* TT_NAME (error 212 in the lowerer), a variable is the label, $ loses one TT_INDIRECT, any other name form (A[1], *X, &K, X @ Y) stands as the expression; each GoV level is the first step of that level's tail */
GoV0        =  *Expr1 FENCE(*$'=' *Expr0 . *ReduceV('TT_ASSIGN', 2) | *$'  ' '=' (epsilon) . thx . *Shift('TT_QLIT', thx) . *ReduceV('TT_ASSIGN', 2)) . *Reduce('TT_NAME', 1);
GoV1        =  *Expr2 *$'?' *Expr2 . *Reduce('TT_SCAN', 2) *Expr1t . *Reduce('TT_NAME', 1);
GoV3        =  *Expr4 *$'|' *Expr4 . *Reduce('TT_ALT', 2) *Expr3t . *Reduce('TT_NAME', 1);
GoV4        =  *Expr5 *$'  ' *Expr5 . *Reduce('TT_SEQ', 2) *Expr4t . *Reduce('TT_NAME', 1);
GoV6        =  *Expr7 *$'  ' ('+' *$'  ' *Expr7 . *ReduceV('TT_ADD', 2) | '-' *$'  ' *Expr7 . *ReduceV('TT_SUB', 2)) *Expr6t . *Reduce('TT_NAME', 1);
GoV8        =  *Expr9 *$'/' *Expr9 . *ReduceV('TT_DIV', 2) *Expr8t . *Reduce('TT_NAME', 1);
GoV9        =  *Expr10 *$'*' *Expr10 . *ReduceV('TT_MUL', 2) *Expr9t . *Reduce('TT_NAME', 1);
GoV11       =  *Expr12 *$'  ' ('**' | '^' | '!') *$'  ' *Expr11 . *ReduceV('TT_POW', 2) . *Reduce('TT_NAME', 1);
GoV12       =  *Expr13 *$'  ' ('$' *$'  ' *Expr13 . *ReduceV('TT_CAPT_IMMED_ASGN', 2) | '.' *$'  ' *Expr13 . *ReduceV('TT_CAPT_COND_ASGN', 2)) *Expr12tail . *Reduce('TT_NAME', 1);
GoVU        =  @gu *DIFFER(Src ? (POS(gu) ANY('@~?+-.'))) *Expr14 . *Reduce('TT_NAME', 1);
GoVL        =  @gl *DIFFER(Src ? (POS(gl) ANY("'" '"' '0123456789'))) *Expr17 . *Reduce('TT_NAME', 1);
GoX         =  *GoInner | *GoV0 | *GoV1 | *GoV3 | *GoV4 | *GoV6 | *GoV8 | *GoV9 | *GoV11 | *GoV12 | *GoVU | *GoVL
            |  '(' *$' ' *GoX *$' ' ')'
            |  '$' *Expr14
            |  *Expr;
Target      =  '(' *$' ' *GoX *$' ' ')'
            |  '<' *$' ' *Expr *$' ' '>' . *Reduce('TT_GOTO_DIRECT', 1);
/* the gotos stand in the statement in written order, :F(x)S(y) as F then S (stmt_ast.c keeps the written order since this landing) */
Sgo         =  *SGoto *$' ' *Target . *Reduce('TT_GOTO_S', 1) . *IncCounter();
Fgo         =  *FGoto *$' ' *Target . *Reduce('TT_GOTO_F', 1) . *IncCounter();
Ugo         =  *Target . *Reduce('TT_GOTO_U', 1) . *IncCounter();
Goto        =  *$' ' ':'
               *$' '
               FENCE(
                  *Ugo
               |  *Sgo FENCE(*$' ' FENCE(':' *$' ' | epsilon) *Fgo | epsilon)
               |  *Fgo FENCE(*$' ' FENCE(':' *$' ' | epsilon) *Sgo | epsilon)
               );
Control     =  '-' BREAK(CHAR(10) ';');
SemiOk      =  @sq *(~(Src ? (TAB(sq) (SPAN(' ' CHAR(9)) | epsilon) CHAR(10) ANY('+.'))));
Comment     =  '*' BREAK(CHAR(10));
/* THE STATEMENT, as stmt_ast.c stmt_to_ast lays it out, built in recognition order: :lbl, then :subj -- a blank-separated */
/* or ?-separated pattern match is TT_SCAN subject pattern inside it (snobol4.y opt_subject) -- then :eq and :repl (an     */
/* empty replacement is the null string), then the gotos present.                                                         */
StmtLabel   =  (NOTANY(' ' CHAR(9) CHAR(10) ';') FENCE(BREAK(' ' CHAR(9) CHAR(10) ';') | REM)) $ ltx *IDENT(SnLblT[ltx]) *DIFFER((SnLblT[ltx] = 1));
StmtRepl    =  *$'  ' '=' . *ReduceEq() . *IncCounter() *$' '
               FENCE(*Expr | (epsilon) . thx . *Shift('TT_QLIT', thx)) . *Reduce('TT_ATTR', 1, ':repl') . *IncCounter();
/* a statement-level ? chain, left to right: each ? takes its pattern (or none) and wraps what stands before it in a TT_SCAN (snobol4.y qsubject) */
StmtQuery   =  *$'?' FENCE(*Expr3 . *Reduce('TT_SCAN', 2) | epsilon . *Reduce('TT_SCAN', 1)) FENCE(*StmtQuery | epsilon);
/* a replacement needs a subject: a label followed by = is SPITBOL's "missing operand" and the C's syntax error (smoke_hello.sno) */
/* a bare subject may be an & chain whose first operand is an expr5 (snobol4.y subj_amp): A & B with & OPSYN'd, no pattern  */
Stmt        =  @StmtAt *NoteStmtPos() epsilon . *PushCounter()
               FENCE((*StmtLabel) . thx . *Shift('TT_QLIT', REPLACE(thx, SnLT[SnCase], SnUT[SnCase])) . *Reduce('TT_ATTR', 1, ':lbl') . *IncCounter() | epsilon)
               FENCE(
                  FENCE(
                     *$'  ' *Expr14 *$'  ' *Expr4 . *Reduce('TT_SEQ', 2) *StmtQuery . *Reduce('TT_ATTR', 1, ':subj') . *IncCounter()
                  |  *$'  ' *Expr14 *$'  ' *Expr2 . *Reduce('TT_SCAN', 2) . *Reduce('TT_ATTR', 1, ':subj') . *IncCounter()
                  |  *$'  ' *Expr2 *StmtQuery . *Reduce('TT_ATTR', 1, ':subj') . *IncCounter()
                  |  *$'  ' *Expr5 *$'&' *Expr3 . *Reduce('TT_OPSYN', 2, '&') *Expr2t . *Reduce('TT_ATTR', 1, ':subj') . *IncCounter()
                  |  *$'  ' *Expr5 . *Reduce('TT_ATTR', 1, ':subj') . *IncCounter()
                  )
                  FENCE(*StmtRepl | epsilon)
               |  epsilon
               )
               FENCE(*Goto | epsilon)
               . *ReduceStmt(nTop())
               . *PopCounter()
               *$' ';
/* END after the case map in force, as sbl -bf reads it: upper-case END ends the program and end or End is an ordinary label */
/* unless -CASE folding is on (the C lexer's strcmp after sno_fold; the coo's bisect of 2026-10-01 against ae0a91289)      */
EndStmt     =  ( ((ANY('Ee') ANY('Nn') ANY('Dd')) $ etx *IDENT(REPLACE(etx, SnLT[SnCaseNow], SnUT[SnCaseNow]), 'END')) . thx . *PushCounter() . *Shift('TT_QLIT', REPLACE(thx, SnLT[SnCase], SnUT[SnCase])) . *Reduce('TT_ATTR', 1, ':lbl') . *IncCounter()
                 FENCE(*$'  ' (*Id) . thx . *Shift('TT_QLIT', thx) . *Reduce('TT_ATTR', 1, ':entry') . *IncCounter() | epsilon)
               ) . *Reduce('TT_END', nTop()) . *PopCounter();
Commands    =  *Command FENCE(*Commands | epsilon);
Command     =  FENCE(
                  (*Comment) . thx . *Shift('TT_COMMENT', thx) . *IncCounter() . *Reduce('TT_COMMENT', 1) CHAR(10)
               |  (*CaseCtl | *Control) . thx . *Shift('TT_CONTROL', thx) . *IncCounter() . *Reduce('TT_CONTROL', 1) (CHAR(10) | ';')
               |  *Stmt . *IncCounter() (CHAR(10) | ';' *SemiOk | RPOS(0))
               );
/* a continuation line after a ;-ended statement is error 214 in SPITBOL and the C lexer (diag2.sno): the ; is refused when   */
/* the line's remainder is blank and the next line starts with + or . -- a lookahead from the cursor, the blanks left for the  */
/* next statement (a blank first keeps its first word from being a label); a program without END is refused by both (a bare   */
/* label END ends it, the text after it is never parsed)                                                                     */
Compiland   =  epsilon . *PushCounter()
               POS(0) ARBNO(*Command FLUSH) *EndStmt . *IncCounter() (ANY(' ' CHAR(9) CHAR(10) ';') | RPOS(0)) ARB RPOS(0)
               . *Reduce('Parse', nTop())
               . *PopCounter();
/* ==================================================================================================================== */
/* Preprocess: text to text. The macro pre-pass of the language (SNOBOL4: -INCLUDE and -COPY, as snobol4.l reads them) generates the compiland text; the   */
/* Compiland pattern above parses ONLY that text (Lon 2026-10-03, CEO-1483). -INCLUDE names a file once, -COPY every time; a name is tried as written, then   */
/* in each directory of SNO_LIB, then in the directory of each file already included.                                                                      */
/* ==================================================================================================================== */
/* the patterns are built the first time a source carries an include line: a program with none runs the Compiland alone, as before */
function PPPat() {
    pp_spc   = ' ' CHAR(9);
    pp_nl    = CHAR(10);
    pp_kw    = ( '-' ANY('Ii') ANY('Nn') ANY('Cc') ANY('Ll') ANY('Uu') ANY('Dd') ANY('Ee') | '-' ANY('Cc') ANY('Oo') ANY('Pp') ANY('Yy') );
    pp_q     = ( "'" NOTANY("'" pp_nl) FENCE(BREAK("'" pp_nl) | epsilon) "'" | '"' NOTANY('"' pp_nl) FENCE(BREAK('"' pp_nl) | epsilon) '"' );
    pp_inl   = ( ( pp_kw SPAN(pp_spc) pp_q FENCE(BREAK(pp_nl) | epsilon) ) . pp_ln pp_nl . *PPInc(pp_ln) );
    pp_oth   = ( ( BREAK(pp_nl) pp_nl | LEN(1) REM ) . pp_ln . *PPTxt(pp_ln) );
    Preprocess = ( POS(0) ARBNO(FENCE(pp_inl | pp_oth)) RPOS(0) );
    pp_ready = 1;
    return;
}
/* ==================================================================================================================== */
function PPInit(w, s) {
    if (IDENT(pp_ready)) { PPPat(); }
    pp_seen = TABLE(31);
    pp_dirs = TABLE(31);
    pp_nd = 0;
    pp_err = '';
    pp_out = '';
    pp_buf = '';
    s = HOST(4, 'SNO_LIB');
    s = DIFFER(ModLinked) ModIncl;
    while (DIFFER(s)) {
        if (s ? (POS(0) BREAK(':') . w ':') = ) { ; } else { w = s; s = ''; }
        if (DIFFER(w)) { pp_nd = pp_nd + 1; pp_dirs[pp_nd] = w; }
    }
    return;
}
/* ==================================================================================================================== */
function PPEmit(s) {
    pp_buf = pp_buf s;
    if (GT(SIZE(pp_buf), 2048)) { pp_out = pp_out pp_buf; pp_buf = ''; }
    return;
}
/* ==================================================================================================================== */
function PPTxt(ln) {
    PPTxt = .dummy;
    PPEmit(ln);
    nreturn;
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
function PPFind(nm, i, t) {
    PPFind = ;
    pp_path = nm;
    if (t = PPOpen(nm)) { PPFind = t; return; }
    i = 0;
    while (i = LT(i, pp_nd) i + 1) {
        pp_path = pp_dirs[i] '/' nm;
        if (t = PPOpen(pp_path)) { PPFind = t; return; }
    }
    freturn;
}
/* ==================================================================================================================== */
/* THE REFUSALS AT REDUCE TIME (the ceo, CEO-1649/CEO-1650): a conditional assignment runs when the whole match has succeeded, in       */
/* recognition order, so a predicate inside the pattern sees an empty stack; the checks the C parser makes in its reduce actions are made */
/* here by ReduceV, the Reduce that inspects the node it has just built, and a refusal prints the C parser's voice and parks the parse.   */
/* A value (a literal or a dot-made name) is refused as the target of =, $, . and @ and as the operand of &; a constant expression is      */
/* evaluated as SPITBOL evaluates it at compile time, the runtime doing the arithmetic under an error limit of one so the error number    */
/* and text are the runtime's own; the kinds and lists mirror sno_pe_walk in lower_snobol4.c, which the C parser asks through            */
/* sno_preeval_expr. A function call is never folded by a parser (the runtime is not up at parse time): the lowerer folds it.             */
/* the positions go into an ARRAY sized once before the match: a TABLE growing during the match allocates, and a collection  */
/* in the middle of a match with pending conditional assignments leaves a tree with a child that is not a node (CEO-1650)   */
function NoteStmtPos() {
    StmtPosN = StmtPosN + 1;
    StmtPosA[StmtPosN] = StmtAt;
    return;
}
/* ==================================================================================================================== */
function ReduceStmt(n) {
    Reduce('TT_STMT', n);
    StmtNo = StmtNo + 1;
    ReduceStmt = .dummy;
    nreturn;
}
/* ==================================================================================================================== */
function StmtLine(p, s, n, i) {
    p = StmtPosA[StmtNo + 1];
    if (IDENT(p)) { StmtLine = 0; return; }
    if (EQ(p, 0)) { StmtLine = 1; return; }
    s = SUBSTR(Src, 1, p);
    n = 1;
    while (s ? (BREAK(CHAR(10)) CHAR(10) @i)) { n = n + 1; if (~(s = SUBSTR(s, i + 1))) { break; } }
    StmtLine = n;
    return;
}
/* ==================================================================================================================== */
function Refuse(code, text) {
    if (EQ(RefusedStmt, StmtNo + 1)) { return; }
    RefusedStmt = StmtNo + 1;
    ParseRefused = 1;
    if (IDENT(code, 0)) { TERMINAL = 'snobol4:' StmtLine() ': error: ' text; return; }
    TERMINAL = 'scrip: error ' code ': ' text;
    TERMINAL = '  at ' (IDENT(pf_name) '-', pf_name) ':' StmtLine();
    return;
}
/* ==================================================================================================================== */
function IsValueNode(x, t) {
    t = t(x);
    if (IDENT(t, 'TT_QLIT')) { return; }
    if (IDENT(t, 'TT_ILIT')) { return; }
    if (IDENT(t, 'TT_FLIT')) { return; }
    if (IDENT(t, 'TT_NAME')) { return; }
    freturn;
}
/* ==================================================================================================================== */
function SpineDown(s) {
    if (IDENT(t(s), 'TT_SCAN')) { SpineDown = c(s)[1]; return; }
    if (IDENT(t(s), 'TT_SEQ')) { SpineDown = c(s)[1]; return; }
    freturn;
}
/* ==================================================================================================================== */
function ReduceV(t, n, v, x) {
    Reduce(t, n, v);
    ReduceV = .dummy;
    x = Top();
    if (IDENT(t, 'TT_ASSIGN')) { if (IsValueNode(c(x)[1])) { Refuse(0, 'value used where name is required'); } nreturn; }
    if (IDENT(t, 'TT_CAPT_IMMED_ASGN')) { if (IsValueNode(c(x)[2])) { Refuse(0, 'value used where name is required'); } nreturn; }
    if (IDENT(t, 'TT_CAPT_COND_ASGN')) { if (IsValueNode(c(x)[2])) { Refuse(0, 'value used where name is required'); } nreturn; }
    if (IDENT(t, 'TT_CAPT_CURSOR')) { if (IsValueNode(c(x)[1])) { Refuse(0, 'value used where name is required'); } nreturn; }
    if (IDENT(t, 'TT_OPSYN')) { if (IsValueNode(c(x)[1])) { Refuse(0, 'value used where name is required'); } nreturn; }
    ConstVal(x);
    nreturn;
}
/* ==================================================================================================================== */
function ReduceEq(x, s) {
    ReduceEq = .dummy;
    x = Top();
    s = c(x)[1];
    while (s = SpineDown(s)) { ; }
    if (IsValueNode(s)) { Refuse(0, 'value used where name is required'); }
    Reduce('TT_ATTR', 0, ':eq');
    nreturn;
}
/* ==================================================================================================================== */
function FoldRefuse() {
    if (EQ(&ERRLIMIT, 0)) { Refuse(&ERRTYPE, &ERRTEXT); }
    &ERRLIMIT = 0;
    return;
}
/* ==================================================================================================================== */
function FoldPatLike(x, d) {
    d = DATATYPE(x);
    if (IDENT(d, 'PATTERN')) { return; }
    if (IDENT(d, 'EXPRESSION')) { return; }
    freturn;
}
/* ==================================================================================================================== */
function FoldOp(t, l, r, v) {
    &ERRLIMIT = 1;
    if (IDENT(t, 'TT_ADD')) { if (v = l + r) { &ERRLIMIT = 0; FoldOp = v; return; } }
    else if (IDENT(t, 'TT_SUB')) { if (v = l - r) { &ERRLIMIT = 0; FoldOp = v; return; } }
    else if (IDENT(t, 'TT_MUL')) { if (v = l * r) { &ERRLIMIT = 0; FoldOp = v; return; } }
    else if (IDENT(t, 'TT_DIV')) { if (v = l / r) { &ERRLIMIT = 0; FoldOp = v; return; } }
    else if (IDENT(t, 'TT_POW')) { if (v = l ^ r) { &ERRLIMIT = 0; FoldOp = v; return; } }
    else if (IDENT(t, 'TT_MNS')) { if (v = -l) { &ERRLIMIT = 0; FoldOp = v; return; } }
    else if (IDENT(t, 'TT_PLS')) { if (v = +l) { &ERRLIMIT = 0; FoldOp = v; return; } }
    else if (IDENT(t, 'TT_SEQ')) { if (v = l r) { &ERRLIMIT = 0; FoldOp = v; return; } }
    FoldRefuse();
    freturn;
}
/* ==================================================================================================================== */
function FoldCset(nm, a) {
    &ERRLIMIT = 1;
    if (a = APPLY(nm, '')) { &ERRLIMIT = 0; return; }
    FoldRefuse();
    return;
}
/* ==================================================================================================================== */
function ConstVal(y, t, l, r, i, k, a, b) {
    t = t(y);
    if (IDENT(t, 'TT_QLIT')) { ConstVal = v(y); return; }
    if (IDENT(t, 'TT_ILIT')) { ConstVal = v(y) + 0; return; }
    if (IDENT(t, 'TT_FLIT')) { ConstVal = v(y) + 0.0; return; }
    if (IDENT(t, 'TT_VAR')) { if (DIFFER(FoldPatNmT[v(y)])) { ConstVal = ARB; return; } freturn; }
    if (DIFFER(FoldPatNdT[t])) { ConstVal = ARB; return; }
    if (IDENT(t, 'TT_KEYWORD')) {
        if (DIFFER(FoldPatNmT[v(y)])) { ConstVal = ARB; return; }
        if (IDENT(v(y), 'ALPHABET')) { ConstVal = &ALPHABET; return; }
        if (IDENT(v(y), 'LCASE')) { ConstVal = &LCASE; return; }
        if (IDENT(v(y), 'UCASE')) { ConstVal = &UCASE; return; }
        freturn;
    }
    if (IDENT(t, 'TT_NAME')) { if (EQ(n(y), 1) IDENT(t(c(y)[1]), 'TT_VAR')) { ConstVal = .FoldDummy; return; } freturn; }
    if (IDENT(t, 'TT_CAPT_CURSOR')) { if (EQ(n(y), 1) IDENT(t(c(y)[1]), 'TT_VAR')) { ConstVal = @FoldDummy; return; } freturn; }
    if (IDENT(t, 'TT_DEFER')) { ConstVal = *FoldDummy; return; }
    if (IDENT(t, 'TT_MNS')) { if (l = ConstVal(c(y)[1])) { if (a = FoldOp(t, l, l)) { ConstVal = a; return; } } freturn; }
    if (IDENT(t, 'TT_PLS')) { if (l = ConstVal(c(y)[1])) { if (a = FoldOp(t, l, l)) { ConstVal = a; return; } } freturn; }
    if (DIFFER(FoldBinT[t])) { if (l = ConstVal(c(y)[1])) { if (r = ConstVal(c(y)[2])) { if (a = FoldOp(t, l, r)) { ConstVal = a; return; } } } freturn; }
    if (IDENT(t, 'TT_SEQ')) {
        k = n(y);
        if (EQ(k, 0)) { freturn; }
        i = 1;
        while (LE(i, k)) {
            if (~(b = ConstVal(c(y)[i]))) { freturn; }
            if (EQ(i, 1)) { a = b; }
            else if (FoldPatLike(a)) { a = ARB; }
            else if (FoldPatLike(b)) { a = ARB; }
            else if (~(a = FoldOp('TT_SEQ', a, b))) { freturn; }
            i = i + 1;
        }
        ConstVal = a;
        return;
    }
    if (IDENT(t, 'TT_CAPT_COND_ASGN')) { if (l = ConstVal(c(y)[1])) { if (IDENT(t(c(y)[2]), 'TT_VAR')) { ConstVal = ARB; return; } } freturn; }
    if (IDENT(t, 'TT_CAPT_IMMED_ASGN')) { if (l = ConstVal(c(y)[1])) { if (IDENT(t(c(y)[2]), 'TT_VAR')) { ConstVal = ARB; return; } } freturn; }
    if (IDENT(t, 'TT_FNC')) { freturn; }
    if (IDENT(t, 'TT_ALT')) { if (l = ConstVal(c(y)[1])) { if (r = ConstVal(c(y)[2])) { ConstVal = ARB; return; } } freturn; }
    if (DIFFER(FoldPatFnT[t])) {
        k = n(y);
        i = 1;
        while (LE(i, k)) { if (~(b = ConstVal(c(y)[i]))) { freturn; } i = i + 1; }
        if (DIFFER(FoldCsetT[t]) EQ(k, 1)) {
            if (IDENT(b)) { FoldCset(FoldCsetT[t]); freturn; }
            if (IDENT(DATATYPE(b), 'PATTERN')) { FoldCset(FoldCsetT[t]); freturn; }
        }
        ConstVal = ARB;
        return;
    }
    freturn;
}
/* ==================================================================================================================== */
function PPErr(msg) {
    TERMINAL = 'Preprocess: ' msg;
    pp_err = 1;
    return;
}
/* ==================================================================================================================== */
function PPInc(ln, qc, r, nm, key, once, t, dir) {
    PPInc = .dummy;
    once = '';
    if (ln ? (POS(0) '-' ANY('Ii'))) { once = 1; }
    ln ? (POS(0) BREAK(pp_spc) SPAN(pp_spc) LEN(1) . qc REM . r);
    r ? (POS(0) ARB . nm qc ARBNO(NOTANY(qc)) RPOS(0));
    key = nm;
    nm ? (SPAN(pp_spc) RPOS(0)) = ;
    if (~(t = PPFind(nm))) { PPErr('cannot open include ' nm); nreturn; }
    if (DIFFER(once)) {
        if (DIFFER(pp_seen[key])) { nreturn; }
        pp_seen[key] = 1;
    }
    if (pp_path ? (POS(0) ARB . dir '/' ARBNO(NOTANY('/')) RPOS(0))) { pp_nd = pp_nd + 1; pp_dirs[pp_nd] = dir; }
    t ? *Preprocess;
    nreturn;
}
/* ==================================================================================================================== */
/* ParseTree(src, path): the one parse the linked module and the driver share -- the include pre-pass, the per-parse setup and   */
/* the Compiland match; the tree, or failure with ModErrPos the refused statement                                               */
function ParseTree(src, path, ptree) {
    Src = src;
    pf_name = path;
    pp_err = '';
    if (Src ? ((POS(0) | CHAR(10)) '-' ANY('IiCc') ANY('NnOo'))) {
        PPInit();
        if (Src ? *Preprocess) { ; } else { pp_err = 1; }
        if (IDENT(pp_err)) { Src = pp_out pp_buf; }
    }
    InitCounter();
    InitStack();
    ParseRefused = ;
    StmtNo = 0;
    RefusedStmt = 0;
    StmtPosA = ARRAY(SIZE(Src) + 2);
    StmtPosN = 0;
    SnCaseNow = ;
    SnCase = ;
    SnLblT = TABLE(256);
    if (DIFFER(pp_err)) freturn;
    if (Src ? *Compiland) {
        ptree = Pop();
        if (DIFFER(ParseRefused)) { ModErrPos = RefusedStmt; freturn; }
        ParseTree = ptree;
        return;
    }
    freturn;
}
/* the driver: one file from stdin, or each file of PARSER_FILES in turn with a == header; the tree is dumped statement by  */
/* statement, or the parse refused with Parse Error; a linked module (mod_head.sc) skips it                                 */
if (DIFFER(ModLinked)) goto ModEnd;
pf_list = HOST(4, 'PARSER_FILES');
pf_n = 0;
pf_bytes = 0;
pf_parse = 0;
pf_parse1 = 0;
if (IDENT(pf_list)) { INPUT(.INPUT, 9, '[-f0 -r16777215]'); } else { INPUT(.pf_names, 8, pf_list); }
while (LE(0, 1)) {
    if (IDENT(pf_list)) {
        Src = INPUT;
    } else {
        if (~(pf_name = pf_names)) break;
        INPUT(.INPUT, 9, pf_name '[-r16777215]');
        Src = INPUT;
        ENDFILE(9);
        pf_bytes = pf_bytes + SIZE(Src);
        OUTPUT = '== ' pf_name;
    }
    pp_err = '';
    if (Src ? ((POS(0) | CHAR(10)) '-' ANY('IiCc') ANY('NnOo'))) {
        PPInit();
        if (Src ? *Preprocess) { ; } else { pp_err = 1; }
        if (IDENT(pp_err)) { Src = pp_out pp_buf; }
    }
    pf_a = TIME();
    InitCounter();
    InitStack();
    ParseRefused = ;
    StmtNo = 0;
    RefusedStmt = 0;
    StmtPosA = ARRAY(SIZE(Src) + 2);
    StmtPosN = 0;
    SnCaseNow = ;
    SnCase = ;
    SnLblT = TABLE(256);
    if (DIFFER(pp_err)) {
        pf_parse = pf_parse + (TIME() - pf_a);
        OUTPUT = 'Parse Error.';
    } else if (Src ? *Compiland) {
        ptree = Pop();
        pf_parse = pf_parse + (TIME() - pf_a);
        if (DIFFER(ParseRefused)) { OUTPUT = 'Parse Error.'; }
        else {
            pf_i = 1;
            pf_nk = n(ptree);
            while (LE(pf_i, pf_nk)) {
                pf_cmd = ITEM(c(ptree), pf_i);
                if (t(pf_cmd) ? (POS(0) ('TT_STMT' | 'TT_END') RPOS(0))) { TreeDump(pf_cmd); }
                pf_i = pf_i + 1;
            }
            TreeDumpEnd();
        }
    } else {
        pf_parse = pf_parse + (TIME() - pf_a);
        OUTPUT = 'Parse Error.';
    }
    pf_n = pf_n + 1;
    if (EQ(pf_n, 1)) { pf_parse1 = pf_parse; }
    if (IDENT(pf_list)) break;
}
ModEnd: if (DIFFER(pf_list)) TERMINAL = 'PARSER-METRICS files=' pf_n ' bytes=' pf_bytes ' parse_first_us=' pf_parse1 / 1000 ' parse_us=' pf_parse / 1000;
