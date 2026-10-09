E_Parse  = "'Parse'";
white   =   (  SPAN(' ' CHAR(9) CHAR(10))
            |  '%'  ARBNO(NOTANY(CHAR(10))) (CHAR(10) | RPOS(0))
            |  '/*' BREAK('*') '*' ARBNO('*' | NOTANY('/*') BREAK('*') '*') '/'
            );
White   =   *white FENCE(*White | epsilon);
Gray    =   FENCE(*White | epsilon);
$' '    =   Gray;
$'  '   =   White;
Atom_first = ANY(&LCASE SUBSTR(&ALPHABET, 129, 128));
Atom_rest  = SPAN('0123456789' &UCASE &LCASE '_' SUBSTR(&ALPHABET, 129, 128));
Atom       = (*Atom_first FENCE(*Atom_rest | epsilon));
Var_first  = ANY(&UCASE '_');
Var_rest   = SPAN('0123456789' &UCASE &LCASE '_');
Var        = (*Var_first FENCE(*Var_rest | epsilon));
Float      = (SPAN('0123456789') ('.' SPAN('0123456789') FENCE(ANY('eE') FENCE(ANY('+-') | epsilon) SPAN('0123456789') | epsilon) FENCE('NaN' | 'Inf' | epsilon) | ANY('eE') FENCE(ANY('+-') | epsilon) SPAN('0123456789')));
Char_code  = ("0'" NOTANY(CHAR(10)));
Int        = SPAN('0123456789') FENCE(('_' FENCE(SPAN(' ' CHAR(9) CHAR(10)) | epsilon) | ' ') *Int | epsilon);
$'('   =       '('  *$' ';  $')'  = *$' ' ')';
$'['   =       '['  *$' ';  $']'  = *$' ' ']';
$','   = *$' '  ','  *$' ';  $';'  = *$' ' ';' *$' ';
$'|'   = *$' '  '|'  *$' ';
$'.'   = *$' '  '.' ( @eq *IDENT(SIZE(Src), eq) | @eq *DIFFER(' ' CHAR(9) CHAR(10) CHAR(13) '%' ? SUBSTR(Src, eq + 1, 1)) );
$':-'  = *$' '  ':-' *$' ';  $':'  = *$' '  ':'  . op_name_ @la_c *DIFFER(SUBSTR(Src, la_c + 1, 1), '-') *$' ';  $'='  = *$' ' '='  *$' ';
$'+'   = *$' '  '+'  *$' ';  $'-'   = *$' ' '-' @la_m *DIFFER(SUBSTR(Src, la_m + 1, 1), '>') *DIFFER(SUBSTR(Src, la_m + 1, 2), '->') *$' ';
$'*'   = *$' '  '*' @la_s *DIFFER(SUBSTR(Src, la_s + 1, 2), '->') *$' ';  $'/'  = *$' ' '/' @la_d *DIFFER(SUBSTR(Src, la_d + 1, 1), '\') *$' ';
$'is'  = *$' ' 'is' @wq *(~(Src ? (TAB(wq) ANY(&LCASE &UCASE '0123456789_')))) *$' ';
$'*->' = *$' ' '*->' . op_name_ *$' ';
$'as'  = *$' ' 'as' @wq *(~(Src ? (TAB(wq) ANY(&LCASE &UCASE '0123456789_')))) . op_name_ *$' ';
$'-->' = *$' ' '-->' *$' ';
$'{'   = *$' '  '{'  *$' ';  $'}'  = *$' ' '}'  *$' ';
Tk_cut = *$' ' '!' *$' ';
$'=:=' = *$' ' '=:=' *$' ';  $'=\=' = *$' ' '=\=' *$' ';
$'=='  = *$' ' '=='  *$' ';  $'\==' = *$' ' '\==' *$' ';
$'>='  = *$' ' '>='  *$' ';  $'=<'  = *$' ' '=<'  *$' ';
$'>'   = *$' ' '>'   *$' ';  $'<'   = *$' ' '<'   *$' ';
$'\='  = *$' ' '\='  *$' ';
$'=..' = *$' ' '=..' *$' ';
$'=@=' = *$' ' '=@=' . op_name_ *$' ';  $'\=@=' = *$' ' '\=@=' . op_name_ *$' ';
$'@>=' = *$' ' '@>=' . op_name_ *$' ';  $'@=<' = *$' ' '@=<' . op_name_ *$' ';
$'@>'  = *$' ' '@>'  . op_name_ *$' ';  $'@<'  = *$' ' '@<'  . op_name_ *$' ';
$'**'  = *$' ' '**'  . op_name_ *$' ';  $'^'   = *$' '  '^'  . op_name_ *$' ';
$'//'  = *$' ' '//'  *$' ';
$'/\' = *$' ' '/\' . op_name_ *$' ';  $'\/' = *$' ' '\/' . op_name_ *$' ';
$'>>'  = *$' ' '>>'  . op_name_ *$' ';  $'<<'  = *$' ' '<<'  . op_name_ *$' ';
$'mod' = *$' ' 'mod' @wq *(~(Src ? (TAB(wq) ANY(&LCASE &UCASE '0123456789_')))) . op_name_ *$' ';
$'rem' = *$' ' 'rem' @wq *(~(Src ? (TAB(wq) ANY(&LCASE &UCASE '0123456789_')))) . op_name_ *$' ';
$'xor' = *$' ' 'xor' @wq *(~(Src ? (TAB(wq) ANY(&LCASE &UCASE '0123456789_')))) . op_name_ *$' ';
$'div' = *$' ' 'div' @wq *(~(Src ? (TAB(wq) ANY(&LCASE &UCASE '0123456789_')))) . op_name_ *$' ';
$'rdiv' = *$' ' 'rdiv' @wq *(~(Src ? (TAB(wq) ANY(&LCASE &UCASE '0123456789_')))) . op_name_ *$' ';
$'\'   = *$' ' '\' . op_name_;
$'->'  = *$' ' '->' *$' ';
Graphic_first = ANY('\\@#^~?=<>+\-*/:.$&`');
Graphic_rest  = SPAN('\\+\-*/^<>=~?@#&:.$`');
Graphic_atom  = (*Graphic_first FENCE(*Graphic_rest | epsilon));
Graphic_atom2 = (*Graphic_first *Graphic_first FENCE(*Graphic_rest | epsilon));
hex_value = TABLE();
hex_i = 0;
while (LE(hex_i, 35)) {
    hex_value[SUBSTR('0123456789abcdefghijklmnopqrstuvwxyz', hex_i + 1, 1)] = hex_i;
    hex_value[SUBSTR('0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', hex_i + 1, 1)] = hex_i;
    hex_i = hex_i + 1;
}
ascii_table = TABLE(300);
ascii_i = 0;
while (LE(ascii_i, 255)) {
    ascii_table[CHAR(ascii_i)] = ascii_i;
    ascii_i = ascii_i + 1;
}
ascii_table["''"] = 39;  ascii_table['\\'] = 92;  ascii_table["\'"] = 39;  ascii_table['\"'] = 34;  ascii_table['\`'] = 96;
ascii_table['\n'] = 10;  ascii_table['\r'] = 13;   ascii_table['\t'] = 9;    ascii_table['\a'] = 7;    ascii_table['\b'] = 8;
ascii_table['\f'] = 12;  ascii_table['\v'] = 11;   ascii_table['\0'] = 0;    ascii_table['\e'] = 27;   ascii_table['\s'] = 32;
/* ==================================================================================================================== */
/* THE TREE IS THE ONE RAW Shift AND Reduce BUILD, LEFT TO RIGHT, PROPERLY NESTED (Lon 2026-09-30 16:0x, CEO-1378), and it is */
/* the C parser's own term tree: every compound and every operator term is (TT_FNC name args...), an atom (TT_QLIT "a"), a    */
/* variable (TT_VAR X), numbers TT_ILIT / TT_FLIT, a list (TT_MAKELIST elems... [tail]), the cut (TT_CUT), a conjunction the  */
/* right-nested binary ','; a clause is (TT_CLAUSE head [body]), a directive (TT_CLAUSE (TT_NUL) body), a DCG rule a clause    */
/* whose one term is (TT_FNC --> head body) -- the C parser translates it in the lowerer's pre-pass since this landing.       */
/* No function is defined here: a value computed while scanning (an escape, a radix literal) is built by assignments that run  */
/* as null matches, *DIFFER((v = e) 'x'), a loop over a literal is a nested anchored match, and the value is keyed by the      */
/* literal's own text in a table the deferred Shift reads at replay. No selection (a, b) sits inside a pattern: the compiled   */
/* grammar dies on one (the deferred-selection crash, CTO-208).                                                               */
esc_code = TABLE(20);
esc_code['a'] = 7; esc_code['b'] = 8; esc_code['f'] = 12; esc_code['n'] = 10; esc_code['r'] = 13; esc_code['t'] = 9; esc_code['v'] = 11;
esc_code['e'] = 27; esc_code['s'] = 32; esc_code['d'] = 127; esc_code['\'] = 92; esc_code["'"] = 39; esc_code['"'] = 34; esc_code['`'] = 96;
HexDigits = '0123456789abcdefABCDEF';
qesc = TABLE(1024);
rval = TABLE(1024);
eval_ = TABLE(256);
/* the value of the digit run rd in radix rb, accumulated into rv by a loop over the run: the group marks are skipped */
RadixLoop = *DIFFER((rv = 0) 'x') *DIFFER((rd ? (POS(0) ARBNO((ANY(HexDigits) $ rc) *DIFFER((rv = rv * rb + hex_value[rc]) 'x') | ANY('_ ' CHAR(9) CHAR(10))) RPOS(0))) 'x');
/* one escape after the backslash: its code is left in rv (and the text after the backslash in ec); \c and a backslash-newline stand for nothing */
Escape  = ( (ANY('abfnrtvesd\' "'" '"`') $ qc) *DIFFER((rv = esc_code[qc]) 'x')
          | 'x' (SPAN(HexDigits) $ rd) FENCE('\' | epsilon) *DIFFER((rb = 16) 'x') *RadixLoop
          | 'u' (LEN(4) $ rd) *DIFFER((rb = 16) 'x') *RadixLoop
          | 'U' (LEN(8) $ rd) *DIFFER((rb = 16) 'x') *RadixLoop
          | (SPAN('01234567') $ rd) FENCE('\' | epsilon) *DIFFER((rb = 8) 'x') *RadixLoop
          );
/* the code point rv appended to qacc as UTF-8, as the C lexer encodes an escape */
Utf8App = ( *LT(rv, 128)   *DIFFER((qacc = qacc CHAR(rv)) 'x')
          | *LT(rv, 2048)  *DIFFER((qacc = qacc CHAR(192 + rv / 64) CHAR(128 + REMDR(rv, 64))) 'x')
          | *LT(rv, 65536) *DIFFER((qacc = qacc CHAR(224 + rv / 4096) CHAR(128 + REMDR(rv / 64, 64)) CHAR(128 + REMDR(rv, 64))) 'x')
          | *DIFFER((qacc = qacc CHAR(240 + rv / 262144) CHAR(128 + REMDR(rv / 4096, 64)) CHAR(128 + REMDR(rv / 64, 64)) CHAR(128 + REMDR(rv, 64))) 'x')
          );
/* the same code point as one to four byte codes for a code list, held under the escape's text for the replay */
Utf8Bytes = ( *LT(rv, 128)   *DIFFER((eb1[ec] = rv) 'x') *DIFFER((ebn[ec] = 1) 'x')
            | *LT(rv, 2048)  *DIFFER((eb1[ec] = 192 + rv / 64) 'x') *DIFFER((eb2[ec] = 128 + REMDR(rv, 64)) 'x') *DIFFER((ebn[ec] = 2) 'x')
            | *LT(rv, 65536) *DIFFER((eb1[ec] = 224 + rv / 4096) 'x') *DIFFER((eb2[ec] = 128 + REMDR(rv / 64, 64)) 'x') *DIFFER((eb3[ec] = 128 + REMDR(rv, 64)) 'x') *DIFFER((ebn[ec] = 3) 'x')
            | *DIFFER((eb1[ec] = 240 + rv / 262144) 'x') *DIFFER((eb2[ec] = 128 + REMDR(rv / 4096, 64)) 'x') *DIFFER((eb3[ec] = 128 + REMDR(rv / 64, 64)) 'x') *DIFFER((eb4[ec] = 128 + REMDR(rv, 64)) 'x') *DIFFER((ebn[ec] = 4) 'x')
            );
eb1 = TABLE(64); eb2 = TABLE(64); eb3 = TABLE(64); eb4 = TABLE(64); ebn = TABLE(64);
EscNone = ( 'c' FENCE(SPAN(' ' CHAR(9) CHAR(10)) | epsilon) | CHAR(10) FENCE(SPAN(' ' CHAR(9)) | epsilon) );
/* a quoted atom: its text is built in qacc while scanning and held in qesc under the raw text for the replay */
/* the text is taken greedily on the first try (a fenced tail recursion): a repeat found shortest-first would close at the */
/* first quote of a doubled one and the expression fences above never let it extend                                       */
QatomIn = FENCE( ( "''" *DIFFER((qacc = qacc "'") 'x')
                 | '\' ( *Escape *Utf8App | *EscNone )
                 | ((NOTANY("'\" CHAR(10) CHAR(9)) FENCE(BREAK("'\" CHAR(10) CHAR(9)) | epsilon)) $ qc) *DIFFER((qacc = qacc qc) 'x')
                 ) *QatomIn | epsilon );
Qatom   = ( "'" *DIFFER((qacc = '') 'x') ((*QatomIn) $ q_raw . q_txt *DIFFER((qesc[q_raw] = qacc) 'x')) "'" );
/* a quoted atom as a term, and a double-quoted text as one TT_DQLIT node, are what the pattern reads (the ceo 2026-09-30 */
/* 18:3x on Lon's law, CEO-1378; parser_raku.sc's shape): each literal run and each single-character escape (decoded by a     */
/* table) is Shifted as TT_QLIT, a doubled quote is one such piece, every piece after the first is joined by Reduce TT_CAT 2  */
/* as it is read, a numeric escape is Shifted raw as TT_ESC, \c and a backslash-newline are no piece; the lowerer folds the   */
/* pieces into one literal. A quoted name before ( stays the decoded name the compound carries as its value (Qatom above).   */
esc_chr = TABLE(20);
esc_chr['a'] = CHAR(7); esc_chr['b'] = CHAR(8); esc_chr['f'] = CHAR(12); esc_chr['n'] = CHAR(10); esc_chr['r'] = CHAR(13); esc_chr['t'] = CHAR(9); esc_chr['v'] = CHAR(11);
esc_chr['e'] = CHAR(27); esc_chr['s'] = ' '; esc_chr['d'] = CHAR(127); esc_chr['\'] = '\'; esc_chr["'"] = "'"; esc_chr['"'] = '"'; esc_chr['`'] = '`';
qa_skip  = FENCE( '\' *EscNone *qa_skip | epsilon );
qa_piece = FENCE( "''" . *Shift('TT_QLIT', "'")
                | '\' (ANY('abfnrtvesd\' "'" '"`')) . thx . *Shift('TT_QLIT', esc_chr[thx])
                | ('\' *Escape) . thx . *Shift('TT_ESC', thx)
                | (NOTANY("'\" CHAR(10) CHAR(9)) FENCE(BREAK("'\" CHAR(10) CHAR(9)) | epsilon)) . thx . *Shift('TT_QLIT', thx)
                );
qa_more  = FENCE( *qa_skip *qa_piece . *Reduce('TT_CAT', 2) *qa_more | *qa_skip );
QatomP   = "'" *qa_skip FENCE( *qa_piece *qa_more | epsilon . *Shift('TT_QLIT', '') ) "'";
sa_piece = FENCE( '""' . *Shift('TT_QLIT', '"')
                | '\' (ANY('abfnrtvesd\' "'" '"`')) . thx . *Shift('TT_QLIT', esc_chr[thx])
                | ('\' *Escape) . thx . *Shift('TT_ESC', thx)
                | (NOTANY('"\' CHAR(10) CHAR(9)) FENCE(BREAK('"\' CHAR(10) CHAR(9)) | epsilon)) . thx . *Shift('TT_QLIT', thx)
                );
sa_more  = FENCE( *qa_skip *sa_piece . *Reduce('TT_CAT', 2) *sa_more | *qa_skip );
StrEsc  = ( ('\' *Escape) $ ec . ech *Utf8Bytes );
CodeEsc = ( *StrEsc *IDENT(ebn[ec], 1) . *Shift('TT_ILIT', eb1[ech]) . *IncCounter()
          | *StrEsc *IDENT(ebn[ec], 2) . *Shift('TT_ILIT', eb1[ech]) . *IncCounter() . *Shift('TT_ILIT', eb2[ech]) . *IncCounter()
          | *StrEsc *IDENT(ebn[ec], 3) . *Shift('TT_ILIT', eb1[ech]) . *IncCounter() . *Shift('TT_ILIT', eb2[ech]) . *IncCounter() . *Shift('TT_ILIT', eb3[ech]) . *IncCounter()
          | *StrEsc . *Shift('TT_ILIT', eb1[ech]) . *IncCounter() . *Shift('TT_ILIT', eb2[ech]) . *IncCounter() . *Shift('TT_ILIT', eb3[ech]) . *IncCounter() . *Shift('TT_ILIT', eb4[ech]) . *IncCounter()
          );
/* a Shift with a null value takes the matched text as the value (ShiftReduce.sc), so an empty atom's Shift sits on an empty match */
Str     = ( '"' *qa_skip FENCE( *sa_piece *sa_more | epsilon . *Shift('TT_QLIT', '') ) '"' . *Reduce('TT_DQLIT', 1) );
BqIn    = FENCE( ( (NOTANY('`\' CHAR(10) CHAR(9)) . sch) . *Shift('TT_ILIT', ascii_table[sch]) . *IncCounter()
                 | '``' . *Shift('TT_ILIT', 96) . *IncCounter()
                 | *CodeEsc
                 | '\' *EscNone
                 ) *BqIn | epsilon );
BqStr   = ( '`' . *PushCounter() *BqIn '`' . *Reduce('TT_MAKELIST', nTop()) . *PopCounter() );
/* integer literals: the value is accumulated while scanning and held in rval under the literal's text; a char code is a table */
/* lookup on its text; a minus and blanks before a number make a negative literal, as the C reads them                        */
HexGroups = SPAN(HexDigits) FENCE(ANY('_ ') *HexGroups | epsilon);
OctGroups = SPAN('01234567') FENCE(ANY('_ ') *OctGroups | epsilon);
BinGroups = SPAN('01') FENCE(ANY('_ ') *BinGroups | epsilon);
RadixVal = ( ( '0x' (*HexGroups $ rd) *DIFFER((rb = 16) 'x') *RadixLoop
             | '0o' (*OctGroups $ rd) *DIFFER((rb = 8) 'x') *RadixLoop
             | '0b' (*BinGroups $ rd) *DIFFER((rb = 2) 'x') *RadixLoop
             ) $ i_raw . i_txt *DIFFER((rval[i_raw] = rv) 'x') );
IntVal  = ( ( (SPAN('0123456789') $ rbs) "'" (SPAN('0123456789' &LCASE &UCASE) $ rd) *DIFFER((rb = rbs + 0) 'x') *RadixLoop
            | (*Int $ rd) *DIFFER((rb = 10) 'x') *RadixLoop
            ) $ i_raw . i_txt *DIFFER((rval[i_raw] = rv) 'x') );
CharCode = ( "0'" ( (('\' *Escape) $ ec . ech *DIFFER((eval_[ec] = rv) 'x')) . *Shift('TT_ILIT', eval_[ech])
                  | ( "''" | NOTANY(CHAR(10)) ) . p_cc . *Shift('TT_ILIT', ascii_table[p_cc]) ) );
/* an integer beyond 64 bits (the accumulation overflows and the alternative fails) is the C's (TT_FNC $pl_big (TT_QLIT digits)), */
/* the digit groups' marks stripped while scanning and the clean digits held under the literal's text; a radix literal beyond */
/* 64 bits carries its text (0x, 0o, 0b and the digits, the group marks stripped) and the runtime's $pl_big reads it in its radix */
bigclean = TABLE(64);
BigStrip = *DIFFER((bigc = '') 'x') *DIFFER((bigraw ? (POS(0) ARBNO((SPAN(HexDigits 'xo') $ dch) *DIFFER((bigc = bigc dch) 'x') | ANY('_ ' CHAR(9) CHAR(10))) RPOS(0))) 'x') *DIFFER((bigclean[bigraw] = bigc) 'x');
BigRadix = ( '0x' *HexGroups | '0o' *OctGroups | '0b' *BinGroups );
RadixBig = ( (*BigRadix) $ bigraw . bigtx *BigStrip . *Shift('TT_QLIT', bigclean[bigtx]) . *Reduce('TT_FNC', 1, '$pl_big') );
NegRadixBig = ( (*BigRadix) $ bigraw . bigtx *BigStrip . *Shift('TT_QLIT', '-' bigclean[bigtx]) . *Reduce('TT_FNC', 1, '$pl_big') );
BigInt  = ( (*Int) $ bigraw . bigtx *BigStrip . *Shift('TT_QLIT', bigclean[bigtx]) . *Reduce('TT_FNC', 1, '$pl_big') );
NegBigInt = ( (*Int) $ bigraw . bigtx *BigStrip . *Shift('TT_QLIT', '-' bigclean[bigtx]) . *Reduce('TT_FNC', 1, '$pl_big') );
Number  = ( *CharCode
          | (*Float) . thx . *Shift('TT_FLIT', thx)
          | *RadixVal . *Shift('TT_ILIT', rval[i_txt])
          | *RadixBig
          | *IntVal . *Shift('TT_ILIT', rval[i_txt])
          | *BigInt
          );
NegNumber = ( '-' *$' ' ( (*Float) . thx . *Shift('TT_FLIT', '-' thx)
                        | *RadixVal . *Shift('TT_ILIT', -rval[i_txt])
                        | *NegRadixBig
                        | *IntVal . *Shift('TT_ILIT', -rval[i_txt])
                        | *NegBigInt ) );
/* ==================================================================================================================== */
/* op/3: the user operator table -- uop_band[kind name] is the band key ('in700', 'pre500', 'post'); uop_on is FAIL until the first */
/* op/3 goal declares, then the one token pattern; each site checks its own key at scan time and the one Reduce pops the name */
uop_band = TABLE(64);
uop_tok  = *$' ' ((*Atom | *Graphic_atom) $ uop_tx . uop_nm . *PushVal(uop_nm)) *$' ';
uop_on   = FAIL;
op_kind = TABLE(7);
op_kind['xfx'] = 'in'; op_kind['xfy'] = 'in'; op_kind['yfx'] = 'in'; op_kind['fy'] = 'pre'; op_kind['fx'] = 'pre'; op_kind['xf'] = 'post'; op_kind['yf'] = 'post';
band_key = TABLE(24);
band_key['in200'] = 'in200'; band_key['in400'] = 'in400'; band_key['in500'] = 'in500'; band_key['in600'] = 'in600'; band_key['in700'] = 'in700'; band_key['in900'] = 'in900';
band_key['pre200'] = 'pre200'; band_key['pre400'] = 'pre400'; band_key['pre500'] = 'pre500'; band_key['pre600'] = 'pre600'; band_key['pre700'] = 'pre700'; band_key['pre900'] = 'pre900';
band_key['post200'] = 'post'; band_key['post400'] = 'post'; band_key['post500'] = 'post'; band_key['post600'] = 'post'; band_key['post700'] = 'post'; band_key['post900'] = 'post';
OpBand  = ( *LE(op_p, 0)   *DIFFER((uop_band[op_kind[op_t] op_n] = '') 'x')
          | *LE(op_p, 200) *DIFFER((uop_band[op_kind[op_t] op_n] = band_key[op_kind[op_t] '200']) 'x')
          | *LE(op_p, 400) *DIFFER((uop_band[op_kind[op_t] op_n] = band_key[op_kind[op_t] '400']) 'x')
          | *LE(op_p, 500) *DIFFER((uop_band[op_kind[op_t] op_n] = band_key[op_kind[op_t] '500']) 'x')
          | *LE(op_p, 600) *DIFFER((uop_band[op_kind[op_t] op_n] = band_key[op_kind[op_t] '600']) 'x')
          | *LE(op_p, 700) *DIFFER((uop_band[op_kind[op_t] op_n] = band_key[op_kind[op_t] '700']) 'x')
          | *DIFFER((uop_band[op_kind[op_t] op_n] = band_key[op_kind[op_t] '900']) 'x')
          ) *DIFFER((uop_on = uop_tok) 'x');
op_type = ( 'xfx' | 'xfy' | 'yfx' | 'fy' | 'fx' | 'xf' | 'yf' );
op_goal = (   *$' ' 'op' *$'(' . *PushVal('op') . *PushCounter()
              (*Int $ op_p) . thx . *Shift('TT_ILIT', thx) . *IncCounter() *$','
              (*op_type $ op_t) . thx . *Shift('TT_QLIT', thx) . *IncCounter() *$','
              *$' ' ( "'" (BREAK("'") $ op_n . thx) "'" | (*Atom | *Graphic_atom) $ op_n . thx | '(' *$' ' (*Atom | *Graphic_atom) $ op_n . thx *$')' ) . *Shift('TT_QLIT', thx) . *IncCounter()
              *$')' *OpBand
              . *Reduce('TT_FNC', nTop(), PopVal()) . *PopCounter()
          );
/* ==================================================================================================================== */
arg       = ( *arg_top | (*Graphic_atom | ';') . b_name . *Shift('TT_QLIT', b_name) );
arg_ite   = ( *unify_expr FENCE( *$'->' *arg_ite  . *Reduce('TT_FNC', 2, '->') | epsilon ) );
arg_disj  = ( *arg_ite    FENCE( *$';'  *arg_disj . *Reduce('TT_FNC', 2, ';') | epsilon ) );
arg_top   = ( *arg_disj   FENCE( *$':-' *arg_disj . *Reduce('TT_FNC', 2, ':-') | epsilon ) );
args      = ( epsilon . *IncCounter() *arg FENCE(*args_tail | epsilon) );
args_tail = ( *$',' . *IncCounter() *arg FENCE(*args_tail | epsilon) );
list_body_tail = ( *$',' . *IncCounter() *arg FENCE( *list_body_tail | epsilon ) );
list_body      = ( epsilon . *IncCounter() *arg FENCE( *list_body_tail | epsilon ) );
/* list: nil is (TT_MAKELIST); [h|t] or [h,..] (TT_MAKELIST elems... [tail]) */
list = (    *$'['
            FENCE(
              *$']'                    . *Reduce('TT_MAKELIST', 0)
            | epsilon . *PushCounter()
                  *list_body
                  FENCE( *$'|' *arg . *IncCounter() | epsilon )
                  *$']'
                                       . *Reduce('TT_MAKELIST', nTop())
              . *PopCounter()
            )
       );
/* a compound: the name is held from its token to the one Reduce that builds (TT_FNC name args...) */
compound_args = ( epsilon . *PushCounter() FENCE(*args | epsilon) *$')' . *Reduce('TT_FNC', nTop(), PopVal()) . *PopCounter() );
/* primary: atoms, variables, numbers, compound terms, parenthesised terms, lists, curly terms */
/* a fenced level above never retries a primary once one has matched, so a quoted form stands before the graphic-atom forms */
primary = (   *BqStr
          |   *Str
          |   (*Atom) . p_name *$'(' . *PushVal(p_name) *compound_args
          |   *$' ' (*Graphic_atom | ';') . g_name *$'(' . *PushVal(g_name) *compound_args
          |   *$' ' *NegNumber
          |   *uop_on *IDENT(uop_band['pre' uop_tx], 'pre200') *primary     . *Reduce('TT_FNC', 1, PopVal())
          |   *uop_on *IDENT(uop_band['pre' uop_tx], 'pre400') *pow_expr    . *Reduce('TT_FNC', 1, PopVal())
          |   *uop_on *IDENT(uop_band['pre' uop_tx], 'pre500') *mul_expr    . *Reduce('TT_FNC', 1, PopVal())
          |   *uop_on *IDENT(uop_band['pre' uop_tx], 'pre600') *add_expr    . *Reduce('TT_FNC', 1, PopVal())
          |   *uop_on *IDENT(uop_band['pre' uop_tx], 'pre700') *colon_expr  . *Reduce('TT_FNC', 1, PopVal())
          |   *uop_on *IDENT(uop_band['pre' uop_tx], 'pre900') *unify_expr  . *Reduce('TT_FNC', 1, PopVal())
          |   *$' ' '\+' *$' ' *unify_expr    . *Reduce('TT_FNC', 1, '\+')
          |   (*Graphic_atom2) . thx . *Shift('TT_QLIT', thx)
          |   *Tk_cut                  . *Reduce('TT_CUT', 0)
          |   *Number
          |   (*Atom) . thx . *Shift('TT_QLIT', thx)
          |   *Qatom *$'(' . *PushVal(qesc[q_txt]) *compound_args
          |   *Qatom *IDENT(qacc, '[]') . *Reduce('TT_MAKELIST', 0)
          |   *QatomP
          |   *Var . p_text . *Shift('TT_VAR', p_text)
          |   *$'(' *term_top *$')'
          |   *$'(' (*Graphic_atom | ';') . b_name *$')' . *Shift('TT_QLIT', b_name)
          |   *$'{' *$'}'             . *Reduce('TT_FNC', 0, '{}')
          |   *$'{' *body *$'}'       . *Reduce('TT_FNC', 1, '{}')
          |   *$'{' (*Graphic_atom | ';') . b_name *$'}' . *Shift('TT_QLIT', b_name) . *Reduce('TT_FNC', 1, '{}')
          |   *list
          |   *$'\' *$' ' *primary            . *Reduce('TT_FNC', 1, '\')
          |   *$' ' '-' *$' ' *primary   . *Reduce('TT_FNC', 1, '-')
          |   *$' ' '+' *$' ' *primary   . *Reduce('TT_FNC', 1, '+')
          );
pow_expr  = (   *primary
                FENCE( *$'^'  *pow_expr  . *Reduce('TT_FNC', 2, '^')
                     | *$'**' *primary   . *Reduce('TT_FNC', 2, '**')
                     | *uop_on *IDENT(uop_band['in' uop_tx], 'in200') *pow_expr . *Reduce('TT_FNC', 2, PopVal())
                     | *uop_on *IDENT(uop_band['post' uop_tx], 'post') . *Reduce('TT_FNC', 1, PopVal())
                     | epsilon
                     )
            );
mul_expr  = (   *pow_expr *mul_tail );
mul_tail  = FENCE( FENCE( *$'mod' *pow_expr  . *Reduce('TT_FNC', 2, 'mod')
                         | *$'rem' *pow_expr  . *Reduce('TT_FNC', 2, 'rem')
                         | *$'div' *pow_expr  . *Reduce('TT_FNC', 2, 'div')
                         | *$'rdiv' *pow_expr . *Reduce('TT_FNC', 2, 'rdiv')
                         | *$'>>'  *pow_expr  . *Reduce('TT_FNC', 2, '>>')
                         | *$'<<'  *pow_expr  . *Reduce('TT_FNC', 2, '<<')
                         | *$'*'   *pow_expr  . *Reduce('TT_FNC', 2, '*')
                         | *$'//'  *pow_expr  . *Reduce('TT_FNC', 2, '//')
                         | *$'/'   *pow_expr  . *Reduce('TT_FNC', 2, '/')
                         | *$'xor' *pow_expr  . *Reduce('TT_FNC', 2, 'xor')
                         | *uop_on *IDENT(uop_band['in' uop_tx], 'in400') *pow_expr . *Reduce('TT_FNC', 2, PopVal())
                         ) *mul_tail | epsilon );
add_expr  = (   *mul_expr *add_tail );
add_tail  = FENCE( FENCE( *$'+' *mul_expr  . *Reduce('TT_FNC', 2, '+')
                         | *$'-' *mul_expr  . *Reduce('TT_FNC', 2, '-')
                         | *$'/\' *mul_expr . *Reduce('TT_FNC', 2, '/\')
                         | *$'\/' *mul_expr . *Reduce('TT_FNC', 2, '\/')
                         | *uop_on *IDENT(uop_band['in' uop_tx], 'in500') *mul_expr . *Reduce('TT_FNC', 2, PopVal())
                         ) *add_tail | epsilon );
colon_expr = (  *add_expr
                FENCE( *$':' *colon_expr  . *Reduce('TT_FNC', 2, ':')
                     | *uop_on *IDENT(uop_band['in' uop_tx], 'in600') *colon_expr . *Reduce('TT_FNC', 2, PopVal())
                     | epsilon
                     )
             );
cmp_expr  = (   *colon_expr
                FENCE( *$'is'  *colon_expr  . *Reduce('TT_FNC', 2, 'is')
                     | *$'as'  *colon_expr  . *Reduce('TT_FNC', 2, 'as')
                     | *$'=@=' *colon_expr  . *Reduce('TT_FNC', 2, '=@=')
                     | *$'\=@=' *colon_expr . *Reduce('TT_FNC', 2, '\=@=')
                     | *$'=:=' *colon_expr  . *Reduce('TT_FNC', 2, '=:=')
                     | *$'=\=' *colon_expr  . *Reduce('TT_FNC', 2, '=\=')
                     | *$'\==' *colon_expr  . *Reduce('TT_FNC', 2, '\==')
                     | *$'@>=' *colon_expr  . *Reduce('TT_FNC', 2, '@>=')
                     | *$'@=<' *colon_expr  . *Reduce('TT_FNC', 2, '@=<')
                     | *$'@>'  *colon_expr  . *Reduce('TT_FNC', 2, '@>')
                     | *$'@<'  *colon_expr  . *Reduce('TT_FNC', 2, '@<')
                     | *$'>='  *colon_expr  . *Reduce('TT_FNC', 2, '>=')
                     | *$'=<'  *colon_expr  . *Reduce('TT_FNC', 2, '=<')
                     | *$'>'   *colon_expr  . *Reduce('TT_FNC', 2, '>')
                     | *$'<'   *colon_expr  . *Reduce('TT_FNC', 2, '<')
                     | *$'\='  *colon_expr  . *Reduce('TT_FNC', 2, '\=')
                     | *$'=..' *colon_expr  . *Reduce('TT_FNC', 2, '=..')
                     | *$'=='  *colon_expr  . *Reduce('TT_FNC', 2, '==')
                     | *$'='   *colon_expr  . *Reduce('TT_FNC', 2, '=')
                     | *uop_on *IDENT(uop_band['in' uop_tx], 'in700') *colon_expr . *Reduce('TT_FNC', 2, PopVal())
                     | epsilon
                     )
            );
unify_expr = ( *cmp_expr *op_tail );
op_tail    = FENCE( *uop_on *IDENT(uop_band['in' uop_tx], 'in900') *cmp_expr . *Reduce('TT_FNC', 2, PopVal()) *op_tail | epsilon );
/* ==================================================================================================================== */
pfx_kw_name = (   "dynamic" | "discontiguous" | "meta_predicate" | "multifile"
              |   "module_transparent" | "thread_local" | "volatile"
              |   "initialization" | "thread_initialization" | "public" | "table" | "record"
              );
/* a prefix declaration keyword (fx 1150) takes a term up to 1149: a comma list or a disjunction */
body_goal = (   *$' ' (*pfx_kw_name) . pfx_kw *$'  ' . *PushVal(pfx_kw) *disj . *Reduce('TT_FNC', 1, PopVal())
            |   *$' ' '\+' *$' ' *body_goal  . *Reduce('TT_FNC', 1, '\+')
            |   *op_goal
            |   *unify_expr
            |   *$'(' *body *$')'
            );
/* the control terms as the C reads them: ',' 1000 xfy, '->' and '*->' 1050 xfy, ';' 1100 xfy, right-nested binary nodes */
conj       = ( *body_goal FENCE( *$',' *conj . *Reduce('TT_FNC', 2, ',') | epsilon ) );
conj_arrow = ( *conj FENCE( *$'->' *conj_arrow  . *Reduce('TT_FNC', 2, '->') | *$'*->' *conj_arrow  . *Reduce('TT_FNC', 2, '*->') | epsilon ) );
disj       = ( *conj_arrow FENCE( *$';' *disj . *Reduce('TT_FNC', 2, ';') | epsilon ) );
body = *disj;
term_top = ( *$':-' *body . *Reduce('TT_FNC', 1, ':-')
           | *body FENCE( *$':-' *body . *Reduce('TT_FNC', 2, ':-') | *$'-->' *body . *Reduce('TT_FNC', 2, '-->') | epsilon ) );
head = *unify_expr;
/* a DCG rule is the clause of the one term -->(head, body); a pushback after the head is ','(head, terms) */
dcg_push  = (   *unify_expr FENCE(*$',' *dcg_push . *Reduce('TT_FNC', 2, ',') | epsilon) );
dcg_rule  = (   *head FENCE(*$',' *dcg_push . *Reduce('TT_FNC', 2, ',') | epsilon) *$'-->'
                *body *$'.'
                                      . *Reduce('TT_FNC', 2, '-->') . *Reduce('TT_CLAUSE', 1)
            );
clause    = (   *head
                ( *$':-' *body         . *Reduce('TT_CLAUSE', 2)
                | epsilon            . *Reduce('TT_CLAUSE', 1)
                )
                *$'.'
            );
directive = (   *$':-' . *Reduce('TT_NUL', 0)
                *body *$'.'
                                     . *Reduce('TT_CLAUSE', 2)
            );
top_form  = (*directive | *clause | *dcg_rule);
/* ==================================================================================================================== */
Compiland = epsilon . *PushCounter()
            POS(0) ARBNO( FENCE(*$' ' *top_form . *IncCounter()) FLUSH ) *$' ' RPOS(0)
            . *Reduce('Parse', nTop())
            . *PopCounter();
/* the driver: one file from stdin, or each file of PARSER_FILES in turn with a == header; the trailing newline the read adds  */
/* is stripped; each clause is dumped, or the parse refused with Parse Error                                                 */
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
    if (GT(SIZE(Src), 0)) Src = SUBSTR(Src, 1, SIZE(Src) - 1);
    pf_a = TIME();
    InitCounter();
    InitStack();
    uop_on = FAIL;
    uop_band = TABLE(64);
    if (Src ? *Compiland) {
        ptree = Pop();
        pf_parse = pf_parse + (TIME() - pf_a);
        if (DIFFER(ptree)) {
            pf_i = 1;
            pf_nk = n(ptree);
            while (LE(pf_i, pf_nk)) { TreeDump(c(ptree)[pf_i]); pf_i = pf_i + 1; }
        }
        TreeDumpEnd();
    } else {
        pf_parse = pf_parse + (TIME() - pf_a);
        OUTPUT = 'Parse Error';
    }
    pf_n = pf_n + 1;
    if (EQ(pf_n, 1)) { pf_parse1 = pf_parse; }
    if (IDENT(pf_list)) break;
}
if (DIFFER(pf_list)) TERMINAL = 'PARSER-METRICS files=' pf_n ' bytes=' pf_bytes ' parse_first_us=' pf_parse1 / 1000 ' parse_us=' pf_parse / 1000;
