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
ProtKwd     =  SPAN(&UCASE &LCASE)                $ tx *DIFFER(ProtKwdsT[REPLACE(tx, &LCASE, &UCASE)]);
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
/* associative level is a tail loop that reduces as each right operand is recognised; =, ^ and ~ are right-recursive.     */
ArgTail     =  *$',' FENCE(*Expr | epsilon . *Reduce('TT_NUL', 0)) . *IncCounter() FENCE(*ArgTail | epsilon);
ArgList     =  FENCE(*Expr . *IncCounter() FENCE(*ArgTail | epsilon) | epsilon . *Reduce('TT_NUL', 0) . *IncCounter() *ArgTail);
Expr        =  *Expr0;
Expr0       =  *Expr1 FENCE(*$'=' *Expr0 . *Reduce('TT_ASSIGN', 2) | *$'  ' '=' (epsilon) . thx . *Shift('TT_QLIT', thx) . *Reduce('TT_ASSIGN', 2) | epsilon);
Expr1       =  *Expr2 *Expr1t;
Expr1t      =  FENCE(*$'?' *Expr2 . *Reduce('TT_SCAN', 2) *Expr1t | epsilon);
Expr2       =  *Expr3 *Expr2t;
Expr2t      =  FENCE(*$'&' *Expr3 . *Reduce('TT_OPSYN', 2, '&') *Expr2t | epsilon);
Expr3       =  *Expr4 *Expr3t;
Expr3t      =  FENCE(*$'|' *Expr4 . *Reduce('TT_ALT', 2) *Expr3t | epsilon);
Expr4       =  *Expr5 *Expr4t;
Expr4t      =  FENCE(*$'  ' *Expr5 . *Reduce('TT_SEQ', 2) *Expr4t | epsilon);
Expr5       =  *Expr6 *Expr5t;
Expr5t      =  FENCE(*$'@' *Expr6 . *Reduce('TT_OPSYN', 2, '@') *Expr5t | epsilon);
Expr6       =  *Expr7 *Expr6t;
Expr6t      =  FENCE(*$'  ' ('+' *$'  ' *Expr7 . *Reduce('TT_ADD', 2) | '-' *$'  ' *Expr7 . *Reduce('TT_SUB', 2)) *Expr6t | epsilon);
Expr7       =  *Expr8 *Expr7t;
Expr7t      =  FENCE(*$'#' *Expr8 . *Reduce('TT_OPSYN', 2, '#') *Expr7t | epsilon);
Expr8       =  *Expr9 *Expr8t;
Expr8t      =  FENCE(*$'/' *Expr9 . *Reduce('TT_DIV', 2) *Expr8t | epsilon);
Expr9       =  *Expr10 *Expr9t;
Expr9t      =  FENCE(*$'*' *Expr10 . *Reduce('TT_MUL', 2) *Expr9t | epsilon);
Expr10      =  *Expr11 *Expr10t;
Expr10t     =  FENCE(*$'%' *Expr11 . *Reduce('TT_OPSYN', 2, '%') *Expr10t | epsilon);
Expr11      =  *Expr12 FENCE(*$'  ' ('**' | '^' | '!') *$'  ' *Expr11 . *Reduce('TT_POW', 2) | epsilon);
Expr12      =  *Expr13 *Expr12tail;
Expr12tail  =  FENCE(*$'  ' ('$' *$'  ' *Expr13 . *Reduce('TT_CAPT_IMMED_ASGN', 2) *Expr12tail | '.' *$'  ' *Expr13 . *Reduce('TT_CAPT_COND_ASGN', 2) *Expr12tail ) | epsilon);
Expr13      =  *Expr14 FENCE(*$'~' *Expr13 . *Reduce('TT_OPSYN', 2, '~') | epsilon);
Expr14      =  '@' *Expr14 . *Reduce('TT_CAPT_CURSOR', 1)
            |  '~' *Expr14 . *Reduce('TT_NOT', 1)
            |  '?' *Expr14 . *Reduce('TT_INTERROGATE', 1)
            |  '&' (*ProtKwd) . thx . *Shift('TT_KEYWORD', REPLACE(thx, SnLT[SnCase], SnUT[SnCase]))
            |  '&' (*Id) . thx . *Shift('TT_KEYWORD', REPLACE(thx, SnLT[SnCase], SnUT[SnCase]))
            |  '&' *Expr14 . *Reduce('TT_OPSYN', 1, '&')
            |  '+' *Expr14 . *Reduce('TT_PLS', 1)
            |  '-' *Expr14 . *Reduce('TT_MNS', 1)
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
/* expression, (F(args)) a call, <e> TT_GOTO_DIRECT e                                                                      */
GoInner     =  FENCE(  '$' '(' *$' ' *Expr *$' ' ')'
                    |  '$' "'" (BREAK("'")) . thx . *Shift('TT_QLIT', thx) "'"
                    |  '$' '"' (BREAK('"')) . thx . *Shift('TT_QLIT', thx) '"'
                    |  '$' (*Id) . thx . *Shift('TT_QLIT', '$' REPLACE(thx, SnLT[SnCase], SnUT[SnCase]))
                    |  *Call
                    |  (*Id) . thx . *Shift('TT_QLIT', REPLACE(thx, SnLT[SnCase], SnUT[SnCase]))
                    );
Target      =  '(' *$' ' *GoInner *$' ' ')'
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
StmtRepl    =  *$'  ' '=' . *Reduce('TT_ATTR', 0, ':eq') . *IncCounter() *$' '
               FENCE(*Expr | (epsilon) . thx . *Shift('TT_QLIT', thx)) . *Reduce('TT_ATTR', 1, ':repl') . *IncCounter();
/* a replacement needs a subject: a label followed by = is SPITBOL's "missing operand" and the C's syntax error (smoke_hello.sno) */
Stmt        =  epsilon . *PushCounter()
               FENCE((*StmtLabel) . thx . *Shift('TT_QLIT', REPLACE(thx, SnLT[SnCase], SnUT[SnCase])) . *Reduce('TT_ATTR', 1, ':lbl') . *IncCounter() | epsilon)
               FENCE(
                  FENCE(
                     *$'  ' *Expr14 *$'  ' *Expr2 . *Reduce('TT_SCAN', 2) . *Reduce('TT_ATTR', 1, ':subj') . *IncCounter()
                  |  *$'  ' *Expr2 *$'?' FENCE(*Expr3 . *Reduce('TT_SCAN', 2) | epsilon . *Reduce('TT_SCAN', 1)) . *Reduce('TT_ATTR', 1, ':subj') . *IncCounter()
                  |  *$'  ' *Expr5 . *Reduce('TT_ATTR', 1, ':subj') . *IncCounter()
                  )
                  FENCE(*StmtRepl | epsilon)
               |  epsilon
               )
               FENCE(*Goto | epsilon)
               . *Reduce('TT_STMT', nTop())
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
               POS(0) ARBNO(*Command FLUSH) *EndStmt . *IncCounter() (ANY(' ' CHAR(9) CHAR(10)) | RPOS(0)) ARB RPOS(0)
               . *Reduce('Parse', nTop())
               . *PopCounter();
/* ==================================================================================================================== */
/* the driver: one file from stdin, or each file of PARSER_FILES in turn with a == header; the tree is dumped statement by  */
/* statement, or the parse refused with Parse Error                                                                        */
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
    pf_a = TIME();
    InitCounter();
    InitStack();
    SnCaseNow = ;
    SnCase = ;
    SnLblT = TABLE(256);
    if (Src ? *Compiland) {
        ptree = Pop();
        pf_parse = pf_parse + (TIME() - pf_a);
        pf_i = 1;
        pf_nk = n(ptree);
        while (LE(pf_i, pf_nk)) {
            pf_cmd = ITEM(c(ptree), pf_i);
            if (t(pf_cmd) ? (POS(0) ('TT_STMT' | 'TT_END') RPOS(0))) { TreeDump(pf_cmd); }
            pf_i = pf_i + 1;
        }
        TreeDumpEnd();
    } else {
        pf_parse = pf_parse + (TIME() - pf_a);
        OUTPUT = 'Parse Error.';
    }
    pf_n = pf_n + 1;
    if (EQ(pf_n, 1)) { pf_parse1 = pf_parse; }
    if (IDENT(pf_list)) break;
}
if (DIFFER(pf_list)) TERMINAL = 'PARSER-METRICS files=' pf_n ' bytes=' pf_bytes ' parse_first_us=' pf_parse1 / 1000 ' parse_us=' pf_parse / 1000;
