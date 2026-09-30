/* parser_raku.sc -- the Raku parser as Snocone patterns: a mechanical conversion of SCRIP's own top-down C parser,
   src/parsers/raku/rk_syntax.c (the r_* recognizers become the patterns below, in its order and with its names) and
   src/parsers/raku/rk_tree.c (the rkb_* builders become the Rkb* functions at the end), rewritten on Lon's order of
   2026-09-29 ("Totally re-write the parser_raku.sc. That is a mess. Look at the top-down C parser and do a mechanical
   conversion to Snocone/SNOBOL4 parser patterns just like the other parser.sc programs.").
   TWO LAYERS, as in the C: the patterns (rk_syntax.c) recognize and record, through deferred Shift/Reduce actions,
   a raw tree of what they matched (R_* nodes: terms with their prefix and postfix operators, the operator levels of
   the C's x_* precedence climb, expression lists, statements, blocks); the builders (rk_tree.c) turn that raw tree
   into the TT_* tree the C parser prints, walking it in the order the C builds it.  The C parser's parse-time name
   tables (declared names, types, routines -- is_name_n, is_type_n, routine_visible) are kept in TABLEs and consulted
   by predicates during the match, exactly where the C consults them.
   Runtime chain (same as every other parser_*.sc): global, case, assign, match, counter, stack, tree, ShiftReduce,
   tdump, gen, qize, semantic, omega, trace. */
&FULLSCAN = 1;
/* ==================================================================================================================== */
/* character classes (rk_syntax.c is_*_cp): ASCII by the table, every byte >= 128 counts as a letter (UTF-8)        */
/* ==================================================================================================================== */
rk_alpha     =   ANY(&UCASE &LCASE '_' X1xxxxxxx);
rk_word      =   ANY(&UCASE &LCASE '_0123456789' X1xxxxxxx);
rk_words     =   SPAN(&UCASE &LCASE '_0123456789' X1xxxxxxx);
rk_digit     =   ANY('0123456789');
/* ==================================================================================================================== */
/* white space: ws_loop, r_comment, r_unsp, r_pod (the =begin / =pod / =finish forms)                                */
/* ==================================================================================================================== */
rk_comment   =   '#' FENCE(BREAK(CHAR(10) CHAR(13) CHAR(11) CHAR(12)) | REM);
rk_podline   =   FENCE(BREAK(CHAR(10)) CHAR(10) | REM);
rk_podblank  =   FENCE(SPAN(' ' CHAR(9)) | epsilon) (CHAR(10) | RPOS(0));
rk_podbody   =   FENCE(*rk_podblank | *rk_podline *rk_podbody | epsilon);
rk_podend    =   ARBNO(*rk_podline) '=end' *rk_podline;
rk_pod       =   @rk_q *RkAtBol(rk_q) FENCE(SPAN(' ' CHAR(9)) | epsilon) '='
                 (  'finish' . *RkFinish() REM
                 |  'begin' FENCE(SPAN(' ' CHAR(9)) | epsilon) 'finish' . *RkFinish() REM
                 |  'begin' *rk_podend
                 |  *rk_alpha *rk_podline *rk_podbody
                 );
rk_unsp      =   '\' @rk_q *RkIsSpaceOrHash(rk_q) *rk_ws;
rk_wsunit    =   SPAN(' ' CHAR(9) CHAR(10) CHAR(13) CHAR(11) CHAR(12)) | *rk_comment | *rk_pod | *rk_unsp;
rk_ws        =   FENCE(*rk_wsunit *rk_ws | epsilon);
rk_hs        =   FENCE(SPAN(' ' CHAR(9)) | epsilon);
/* r_ENDSTMT: after a closing brace, the rest of the line blank (or a comment) ends the statement                      */
rk_endstmt   =   FENCE(*rk_hs FENCE(*rk_comment | epsilon) @rk_q *RkAtEol(rk_q) *rk_ws @rk_q *RkMarkEnd(rk_q) | epsilon);
/* ==================================================================================================================== */
/* names: r_ident, r_identifier, r_morename, r_name, r_longname, r_deflongname                                       */
/* ==================================================================================================================== */
rk_ident     =   *rk_alpha FENCE(*rk_words | epsilon);
rk_idrest    =   FENCE(ANY("'-") *rk_ident *rk_idrest | epsilon);
rk_identifier =  *rk_ident *rk_idrest;
rk_morename  =   '::' FENCE('(' *rk_ws *rk_EXPR0 *rk_ws ')' | *rk_identifier | epsilon);
rk_mornames  =   FENCE(*rk_morename *rk_mornames | epsilon);
rk_name      =   FENCE(*rk_identifier *rk_mornames | *rk_morename *rk_mornames);
rk_longname  =   *rk_name *rk_cpairs;
rk_cpairs    =   FENCE(@rk_q *RkColonpairAhead(rk_q) *rk_colonpair_raw *rk_cpairs | epsilon);
rk_deflongname = *rk_name FENCE(':' ('<' BREAK('>') '>' | '[' BREAK(']') ']' | '(' BREAK(')') ')') | epsilon);
/* ==================================================================================================================== */
/* keywords: kok (keyword then required white space), kw_end (keyword at a word end), kw (keyword not before a word) */
/* ==================================================================================================================== */
rk_kwendok   =   @rk_q *RkEndKeywordOk(rk_q);
rk_kok_tail  =   *rk_kwendok @rk_q *RkIsSpaceOrHash(rk_q) *rk_ws;
rk_kwend_tail =  *rk_kwendok;
rk_kw_tail   =   @rk_q *RkNotWordAt(rk_q);
/* ==================================================================================================================== */
/* numbers: r_decint, r_integer, r_escale, r_dec_number, r_numish, r_version                                          */
/* ==================================================================================================================== */
rk_decrest   =   FENCE('_' SPAN('0123456789') *rk_decrest | epsilon);
rk_decint    =   SPAN('0123456789') *rk_decrest;
rk_hexrest   =   FENCE('_' SPAN('0123456789abcdefABCDEF') *rk_hexrest | epsilon);
rk_hexint    =   SPAN('0123456789abcdefABCDEF') *rk_hexrest;
rk_binrest   =   FENCE('_' SPAN('01') *rk_binrest | epsilon);
rk_octrest   =   FENCE('_' SPAN('01234567') *rk_octrest | epsilon);
rk_integer   =   FENCE( '0' ( 'x' FENCE('_' | epsilon) *rk_hexint
                            | 'b' FENCE('_' | epsilon) SPAN('01') *rk_binrest
                            | 'o' FENCE('_' | epsilon) SPAN('01234567') *rk_octrest
                            | 'd' FENCE('_' | epsilon) *rk_decint
                            )
                      | *rk_decint
                      );
rk_escale    =   ANY('eE') FENCE(ANY('+-') | epsilon) *rk_decint;
rk_decnumber =   FENCE( '.' *rk_decint FENCE(*rk_escale | epsilon)
                      | *rk_decint ( '.' @rk_q *RkDigitAt(rk_q) *rk_decint FENCE(*rk_escale | epsilon) | *rk_escale )
                      );
rk_numish    =   FENCE( 'NaN' *rk_kw_tail | 'Inf' *rk_kw_tail | *rk_decnumber | *rk_integer );
rk_verrest   =   FENCE('.' (SPAN(&UCASE &LCASE '_0123456789') | '*') *rk_verrest | epsilon);
rk_version   =   'v' *rk_digit FENCE(SPAN(&UCASE &LCASE '_0123456789') | epsilon) *rk_verrest FENCE('+' | epsilon) @rk_q *RkNotAt(rk_q, "-'");
/* ==================================================================================================================== */
/* the operator tables of rk_syntax.c (rk_infix, rk_prefix, rk_postfix: precedence, word flag) and rk_tree.c (lv_*),  */
/* generated from the C source by the conversion script; ASCII operators only                                           */
/* ==================================================================================================================== */
RkPrec = TABLE(211); RkWordOp = TABLE(61); RkAssoc = TABLE(211); RkFlags = TABLE(211); RkLv = TABLE(31);
RkPrec['**'] = 90; RkAssoc['**'] = 2; RkFlags['**'] = 0; RkPrec['*'] = 82; RkAssoc['*'] = 1; RkFlags['*'] = 0; RkPrec['/'] = 82; RkAssoc['/'] = 1; RkFlags['/'] = 0;
RkPrec['div'] = 82; RkAssoc['div'] = 1; RkFlags['div'] = 128; RkWordOp['div'] = 1; RkPrec['gcd'] = 82; RkAssoc['gcd'] = 1; RkFlags['gcd'] = 128;
RkWordOp['gcd'] = 1; RkPrec['lcm'] = 82; RkAssoc['lcm'] = 1; RkFlags['lcm'] = 128; RkWordOp['lcm'] = 1;
RkPrec['%'] = 82; RkAssoc['%'] = 1; RkFlags['%'] = 0; RkPrec['mod'] = 82; RkAssoc['mod'] = 1; RkFlags['mod'] = 128; RkWordOp['mod'] = 1;
RkPrec['%%'] = 82; RkAssoc['%%'] = 1; RkFlags['%%'] = 2; RkPrec['+&'] = 82; RkAssoc['+&'] = 1; RkFlags['+&'] = 0; RkPrec['~&'] = 82; RkAssoc['~&'] = 1; RkFlags['~&'] = 0;
RkPrec['?&'] = 82; RkAssoc['?&'] = 1; RkFlags['?&'] = 2; RkPrec['+<'] = 82; RkAssoc['+<'] = 1; RkFlags['+<'] = 0; RkPrec['+>'] = 82; RkAssoc['+>'] = 1; RkFlags['+>'] = 0;
RkPrec['~<'] = 82; RkAssoc['~<'] = 1; RkFlags['~<'] = 0; RkPrec['~>'] = 82; RkAssoc['~>'] = 1; RkFlags['~>'] = 0; RkPrec['+'] = 78; RkAssoc['+'] = 1; RkFlags['+'] = 0;
RkPrec['-'] = 78; RkAssoc['-'] = 1; RkFlags['-'] = 0; RkPrec['+|'] = 78; RkAssoc['+|'] = 1; RkFlags['+|'] = 0; RkPrec['+^'] = 78; RkAssoc['+^'] = 1; RkFlags['+^'] = 0;
RkPrec['~|'] = 78; RkAssoc['~|'] = 1; RkFlags['~|'] = 0; RkPrec['~^'] = 78; RkAssoc['~^'] = 1; RkFlags['~^'] = 0; RkPrec['?|'] = 78; RkAssoc['?|'] = 1; RkFlags['?|'] = 2;
RkPrec['?^'] = 78; RkAssoc['?^'] = 1; RkFlags['?^'] = 2; RkPrec['x'] = 74; RkAssoc['x'] = 1; RkFlags['x'] = 128; RkWordOp['x'] = 1;
RkPrec['xx'] = 74; RkAssoc['xx'] = 1; RkFlags['xx'] = 128; RkWordOp['xx'] = 1; RkPrec['~'] = 70; RkAssoc['~'] = 1; RkFlags['~'] = 0;
RkPrec['o'] = 70; RkAssoc['o'] = 1; RkFlags['o'] = 0; RkPrec['&'] = 66; RkAssoc['&'] = 4; RkFlags['&'] = 2; RkPrec['(&)'] = 66; RkAssoc['(&)'] = 4; RkFlags['(&)'] = 0;
RkPrec['(.)'] = 66; RkAssoc['(.)'] = 4; RkFlags['(.)'] = 0; RkPrec['|'] = 62; RkAssoc['|'] = 4; RkFlags['|'] = 2; RkPrec['^'] = 62; RkAssoc['^'] = 4; RkFlags['^'] = 2;
RkPrec['(|)'] = 62; RkAssoc['(|)'] = 4; RkFlags['(|)'] = 0; RkPrec['(^)'] = 62; RkAssoc['(^)'] = 4; RkFlags['(^)'] = 0; RkPrec['(+)'] = 62; RkAssoc['(+)'] = 4; RkFlags['(+)'] = 0;
RkPrec['(-)'] = 62; RkAssoc['(-)'] = 4; RkFlags['(-)'] = 0; RkPrec['..'] = 54; RkAssoc['..'] = 3; RkFlags['..'] = 4; RkPrec['^..'] = 54; RkAssoc['^..'] = 3; RkFlags['^..'] = 4;
RkPrec['..^'] = 54; RkAssoc['..^'] = 3; RkFlags['..^'] = 4; RkPrec['^..^'] = 54; RkAssoc['^..^'] = 3; RkFlags['^..^'] = 4; RkPrec['leg'] = 54; RkAssoc['leg'] = 3; RkFlags['leg'] = 132;
RkWordOp['leg'] = 1; RkPrec['cmp'] = 54; RkAssoc['cmp'] = 3; RkFlags['cmp'] = 132; RkWordOp['cmp'] = 1;
RkPrec['unicmp'] = 54; RkAssoc['unicmp'] = 3; RkFlags['unicmp'] = 132; RkWordOp['unicmp'] = 1; RkPrec['coll'] = 54; RkAssoc['coll'] = 3; RkFlags['coll'] = 132;
RkWordOp['coll'] = 1; RkPrec['<=>'] = 54; RkAssoc['<=>'] = 3; RkFlags['<=>'] = 4; RkPrec['but'] = 54; RkAssoc['but'] = 3; RkFlags['but'] = 132;
RkWordOp['but'] = 1; RkPrec['does'] = 54; RkAssoc['does'] = 3; RkFlags['does'] = 132; RkWordOp['does'] = 1;
RkPrec['=~='] = 50; RkAssoc['=~='] = 1; RkFlags['=~='] = 10; RkPrec['=='] = 50; RkAssoc['=='] = 1; RkFlags['=='] = 10; RkPrec['!='] = 50; RkAssoc['!='] = 1; RkFlags['!='] = 10;
RkPrec['<='] = 50; RkAssoc['<='] = 1; RkFlags['<='] = 10; RkPrec['>='] = 50; RkAssoc['>='] = 1; RkFlags['>='] = 10; RkPrec['<'] = 50; RkAssoc['<'] = 1; RkFlags['<'] = 10;
RkPrec['>'] = 50; RkAssoc['>'] = 1; RkFlags['>'] = 10; RkPrec['eq'] = 50; RkAssoc['eq'] = 1; RkFlags['eq'] = 138; RkWordOp['eq'] = 1;
RkPrec['ne'] = 50; RkAssoc['ne'] = 1; RkFlags['ne'] = 138; RkWordOp['ne'] = 1; RkPrec['le'] = 50; RkAssoc['le'] = 1; RkFlags['le'] = 138;
RkWordOp['le'] = 1; RkPrec['ge'] = 50; RkAssoc['ge'] = 1; RkFlags['ge'] = 138; RkWordOp['ge'] = 1;
RkPrec['lt'] = 50; RkAssoc['lt'] = 1; RkFlags['lt'] = 138; RkWordOp['lt'] = 1; RkPrec['gt'] = 50; RkAssoc['gt'] = 1; RkFlags['gt'] = 138;
RkWordOp['gt'] = 1; RkPrec['=:='] = 50; RkAssoc['=:='] = 1; RkFlags['=:='] = 10; RkPrec['==='] = 50; RkAssoc['==='] = 1; RkFlags['==='] = 10;
RkPrec['eqv'] = 50; RkAssoc['eqv'] = 1; RkFlags['eqv'] = 138; RkWordOp['eqv'] = 1; RkPrec['before'] = 50; RkAssoc['before'] = 1; RkFlags['before'] = 138;
RkWordOp['before'] = 1; RkPrec['after'] = 50; RkAssoc['after'] = 1; RkFlags['after'] = 138; RkWordOp['after'] = 1;
RkPrec['~~'] = 50; RkAssoc['~~'] = 1; RkFlags['~~'] = 10; RkPrec['!~~'] = 50; RkAssoc['!~~'] = 1; RkFlags['!~~'] = 10; RkPrec['(elem)'] = 50; RkAssoc['(elem)'] = 1; RkFlags['(elem)'] = 10;
RkPrec['(cont)'] = 50; RkAssoc['(cont)'] = 1; RkFlags['(cont)'] = 10; RkPrec['(<)'] = 50; RkAssoc['(<)'] = 1; RkFlags['(<)'] = 10; RkPrec['(>)'] = 50; RkAssoc['(>)'] = 1; RkFlags['(>)'] = 10;
RkPrec['(==)'] = 50; RkAssoc['(==)'] = 1; RkFlags['(==)'] = 10; RkPrec['(<=)'] = 50; RkAssoc['(<=)'] = 1; RkFlags['(<=)'] = 10; RkPrec['(>=)'] = 50; RkAssoc['(>=)'] = 1; RkFlags['(>=)'] = 10;
RkPrec['(<+)'] = 50; RkAssoc['(<+)'] = 1; RkFlags['(<+)'] = 10; RkPrec['(>+)'] = 50; RkAssoc['(>+)'] = 1; RkFlags['(>+)'] = 10; RkPrec['&&'] = 46; RkAssoc['&&'] = 1; RkFlags['&&'] = 2;
RkPrec['||'] = 42; RkAssoc['||'] = 1; RkFlags['||'] = 2; RkPrec['^^'] = 42; RkAssoc['^^'] = 4; RkFlags['^^'] = 2; RkPrec['//'] = 42; RkAssoc['//'] = 1; RkFlags['//'] = 0;
RkPrec['min'] = 42; RkAssoc['min'] = 4; RkFlags['min'] = 128; RkWordOp['min'] = 1; RkPrec['max'] = 42; RkAssoc['max'] = 4; RkFlags['max'] = 128;
RkWordOp['max'] = 1; RkPrec['ff'] = 38; RkAssoc['ff'] = 2; RkFlags['ff'] = 1; RkPrec['^ff'] = 38; RkAssoc['^ff'] = 2; RkFlags['^ff'] = 1;
RkPrec['ff^'] = 38; RkAssoc['ff^'] = 2; RkFlags['ff^'] = 1; RkPrec['^ff^'] = 38; RkAssoc['^ff^'] = 2; RkFlags['^ff^'] = 1; RkPrec['fff'] = 38; RkAssoc['fff'] = 2; RkFlags['fff'] = 1;
RkPrec['^fff'] = 38; RkAssoc['^fff'] = 2; RkFlags['^fff'] = 1; RkPrec['fff^'] = 38; RkAssoc['fff^'] = 2; RkFlags['fff^'] = 1; RkPrec['^fff^'] = 38; RkAssoc['^fff^'] = 2; RkFlags['^fff^'] = 1;
RkPrec[':='] = 34; RkAssoc[':='] = 2; RkFlags[':='] = 1; RkPrec['=>'] = 34; RkAssoc['=>'] = 2; RkFlags['=>'] = 0; RkPrec[','] = 26; RkAssoc[','] = 4; RkFlags[','] = 64;
RkPrec['Z'] = 22; RkAssoc['Z'] = 4; RkFlags['Z'] = 0; RkPrec['X'] = 22; RkAssoc['X'] = 4; RkFlags['X'] = 0; RkPrec['minmax'] = 22; RkAssoc['minmax'] = 4; RkFlags['minmax'] = 128;
RkWordOp['minmax'] = 1; RkPrec['...'] = 22; RkAssoc['...'] = 4; RkFlags['...'] = 0; RkPrec['...^'] = 22; RkAssoc['...^'] = 4; RkFlags['...^'] = 0;
RkPrec['^...'] = 22; RkAssoc['^...'] = 4; RkFlags['^...'] = 0; RkPrec['^...^'] = 22; RkAssoc['^...^'] = 4; RkFlags['^...^'] = 0; RkPrec['and'] = 14; RkAssoc['and'] = 1; RkFlags['and'] = 130;
RkWordOp['and'] = 1; RkPrec['andthen'] = 14; RkAssoc['andthen'] = 4; RkFlags['andthen'] = 128; RkWordOp['andthen'] = 1;
RkPrec['notandthen'] = 14; RkAssoc['notandthen'] = 4; RkFlags['notandthen'] = 128; RkWordOp['notandthen'] = 1; RkPrec['or'] = 10; RkAssoc['or'] = 1; RkFlags['or'] = 130;
RkWordOp['or'] = 1; RkPrec['xor'] = 10; RkAssoc['xor'] = 4; RkFlags['xor'] = 130; RkWordOp['xor'] = 1;
RkPrec['orelse'] = 10; RkAssoc['orelse'] = 4; RkFlags['orelse'] = 128; RkWordOp['orelse'] = 1; RkPrec['<=='] = 6; RkAssoc['<=='] = 4; RkFlags['<=='] = 0;
RkPrec['==>'] = 6; RkAssoc['==>'] = 4; RkFlags['==>'] = 0; RkPrec['<<=='] = 6; RkAssoc['<<=='] = 4; RkFlags['<<=='] = 0; RkPrec['==>>'] = 6; RkAssoc['==>>'] = 4; RkFlags['==>>'] = 0;
RkLv['MUL'] = TABLE(25); RkLv['MUL']['*'] = 0; RkLv['MUL']['~&'] = 3; RkLv['MUL']['mod'] = 4;
RkLv['MUL']['lcm'] = 5; RkLv['MUL']['/'] = 6; RkLv['MUL']['%'] = 7; RkLv['MUL']['div'] = 8;
RkLv['MUL']['gcd'] = 9; RkLv['MUL']['+&'] = 10; RkLv['MUL']['+<'] = 11; RkLv['ADDSUB'] = TABLE(15);
RkLv['ADDSUB']['+'] = 0; RkLv['ADDSUB']['?^'] = 2; RkLv['ADDSUB']['?|'] = 3; RkLv['ADDSUB']['~|'] = 4;
RkLv['ADDSUB']['+|'] = 5; RkLv['ADDSUB']['-'] = 6; RkLv['REPL'] = TABLE(5); RkLv['REPL']['x'] = 0;
RkLv['REPL']['xx'] = 1; RkLv['CAT'] = TABLE(7); RkLv['CAT']['~'] = 0; RkLv['CAT']['o'] = 2;
RkLv['RANGE1'] = TABLE(7); RkLv['RANGE1']['..'] = 0; RkLv['RANGE1']['...'] = 1; RkLv['RANGE1']['..^'] = 2;
RkLv['RANGE2'] = TABLE(9); RkLv['RANGE2']['unicmp'] = 0; RkLv['RANGE2']['coll'] = 1; RkLv['RANGE2']['^..^'] = 2;
RkLv['RANGE2']['^..'] = 3; RkLv['DOR'] = TABLE(3); RkLv['DOR']['//'] = 0; RkLv['JCT'] = TABLE(17);
RkLv['JCT']['|'] = 0; RkLv['JCT']['(^)'] = 1; RkLv['JCT']['(-)'] = 2; RkLv['JCT']['(+)'] = 3;
RkLv['JCT']['(|)'] = 4; RkLv['JCT']['(.)'] = 5; RkLv['JCT']['(&)'] = 6; RkLv['JCT']['&'] = 7;
RkLv['DIVIS'] = TABLE(3); RkLv['DIVIS']['%%'] = 0; RkLv['CMP'] = TABLE(55); RkLv['CMP']['=='] = 0;
RkLv['CMP']['!='] = 1; RkLv['CMP']['<'] = 2; RkLv['CMP']['>'] = 3; RkLv['CMP']['<='] = 4;
RkLv['CMP']['>='] = 5; RkLv['CMP']['!~~'] = 9; RkLv['CMP']['=~='] = 10; RkLv['CMP']['(elem)'] = 11;
RkLv['CMP']['(cont)'] = 12; RkLv['CMP']['after'] = 13; RkLv['CMP']['before'] = 14; RkLv['CMP']['eqv'] = 15;
RkLv['CMP']['==='] = 16; RkLv['CMP']['eq'] = 17; RkLv['CMP']['<=>'] = 18; RkLv['CMP']['cmp'] = 19;
RkLv['CMP']['leg'] = 20; RkLv['CMP']['ne'] = 21; RkLv['CMP']['lt'] = 22; RkLv['CMP']['le'] = 23;
RkLv['CMP']['gt'] = 24; RkLv['CMP']['ge'] = 25; RkLv['CMP']['~~'] = 26; RkLv['AND'] = TABLE(3);
RkLv['AND']['&&'] = 0; RkLv['OR'] = TABLE(9); RkLv['OR']['||'] = 0; RkLv['OR']['^^'] = 1;
RkLv['OR']['max'] = 2; RkLv['OR']['min'] = 3; RkLv['POW'] = TABLE(3); RkLv['POW']['**'] = 0;
RkLv['TERN'] = TABLE(3); RkLv['TERN']['??'] = 0; RkLv['PAIR'] = TABLE(3); RkLv['PAIR']['=>'] = 0;
RkLv['COMPOUND'] = TABLE(11); RkLv['COMPOUND']['+='] = 0; RkLv['COMPOUND']['-='] = 1; RkLv['COMPOUND']['*='] = 2;
RkLv['COMPOUND']['/='] = 3; RkLv['COMPOUND']['~='] = 4;
rk_iop_table =   (  'notandthen' *rk_kw_tail
                 |  'andthen' *rk_kw_tail
                 |  '(cont)'
                 |  '(elem)'
                 |  'before' *rk_kw_tail
                 |  'minmax' *rk_kw_tail
                 |  'orelse' *rk_kw_tail
                 |  'unicmp' *rk_kw_tail
                 |  '^...^'
                 |  '^fff^'
                 |  'after' *rk_kw_tail
                 |  '(<+)'
                 |  '(<=)'
                 |  '(==)'
                 |  '(>+)'
                 |  '(>=)'
                 |  '...^'
                 |  '<<=='
                 |  '==>>'
                 |  '^...'
                 |  '^..^'
                 |  '^ff^'
                 |  '^fff'
                 |  'coll' *rk_kw_tail
                 |  'does' *rk_kw_tail
                 |  'fff^'
                 |  '!~~'
                 |  '(&)'
                 |  '(+)'
                 |  '(-)'
                 |  '(.)'
                 |  '(<)'
                 |  '(>)'
                 |  '(^)'
                 |  '(|)'
                 |  '...'
                 |  '..^'
                 |  '<=='
                 |  '<=>'
                 |  '=:='
                 |  '==='
                 |  '==>'
                 |  '=~='
                 |  '^..'
                 |  '^ff'
                 |  'and' *rk_kw_tail
                 |  'but' *rk_kw_tail
                 |  'cmp' *rk_kw_tail
                 |  'div' *rk_kw_tail
                 |  'eqv' *rk_kw_tail
                 |  'ff^'
                 |  'fff'
                 |  'gcd' *rk_kw_tail
                 |  'lcm' *rk_kw_tail
                 |  'leg' *rk_kw_tail
                 |  'max' *rk_kw_tail
                 |  'min' *rk_kw_tail
                 |  'mod' *rk_kw_tail
                 |  'xor' *rk_kw_tail
                 |  '!=' @rk_q *RkNeOk(rk_q)
                 |  '%%'
                 |  '&&'
                 |  '**'
                 |  '+&'
                 |  '+<'
                 |  '+>'
                 |  '+^'
                 |  '+|'
                 |  '..'
                 |  '//'
                 |  ':='
                 |  '<='
                 |  '=='
                 |  '=>'
                 |  '>='
                 |  '?&'
                 |  '?^'
                 |  '?|'
                 |  '^^'
                 |  'eq' *rk_kw_tail
                 |  'ff'
                 |  'ge' *rk_kw_tail
                 |  'gt' *rk_kw_tail
                 |  'le' *rk_kw_tail
                 |  'lt' *rk_kw_tail
                 |  'ne' *rk_kw_tail
                 |  'or' *rk_kw_tail
                 |  'xx' *rk_kw_tail
                 |  '||'
                 |  '~&'
                 |  '~<'
                 |  '~>'
                 |  '~^'
                 |  '~|'
                 |  '~~'
                 |  '%'
                 |  '&'
                 |  '*'
                 |  '+'
                 |  ','
                 |  '-' @rk_q *RkMinusOk(rk_q)
                 |  '/'
                 |  '<'
                 |  '>'
                 |  'X'
                 |  'Z'
                 |  '^'
                 |  'o'
                 |  'x' *rk_kw_tail
                 |  '|'
                 |  '~'
                 );
RkPrePrec = TABLE(31);
RkPrePrec['++'] = 94; RkPrePrec['--'] = 94; RkPrePrec['+'] = 86; RkPrePrec['~'] = 86; RkPrePrec['-'] = 86; RkPrePrec['?'] = 86;
RkPrePrec['!'] = 86; RkPrePrec['|'] = 86; RkPrePrec['+^'] = 86; RkPrePrec['~^'] = 86; RkPrePrec['?^'] = 86; RkPrePrec['^'] = 86;
RkPrePrec['let'] = 94; RkPrePrec['temp'] = 94; RkPrePrec['so'] = 30; RkPrePrec['not'] = 30;
rk_pop_table =   (  'temp' *rk_kw_tail
                 |  'let' *rk_kw_tail
                 |  'not' *rk_kw_tail
                 |  '++'
                 |  '+^'
                 |  '--'
                 |  '?^'
                 |  'so' *rk_kw_tail
                 |  '~^'
                 |  '!'
                 |  '+'
                 |  '-' @rk_q *RkMinusOk(rk_q)
                 |  '?'
                 |  '^'
                 |  '|'
                 |  '~'
                 );
rk_postop_table = ( '++'
                 |  '--'
                 |  'i' *rk_kw_tail
                 );
/* rk_core_names.h rk_core_names[]: the setting's names, generated from the C header by the conversion script */
RkCoreN = TABLE(2111);
RkCoreN['AST'] = 1; RkCoreN['Allomorph'] = 1; RkCoreN['Any'] = 1; RkCoreN['Array'] = 1; RkCoreN['Array::Element'] = 1; RkCoreN['Array::Element::Access'] = 1;
RkCoreN['Array::Shaped'] = 1; RkCoreN['Array::Shaped1'] = 1; RkCoreN['Array::Shaped2'] = 1; RkCoreN['Array::Shaped3'] = 1; RkCoreN['Array::Slice'] = 1; RkCoreN['Array::Slice::Access'] = 1;
RkCoreN['Array::Slice::Assign'] = 1; RkCoreN['Array::Slice::Bind'] = 1; RkCoreN['Array::Typed'] = 1; RkCoreN['Associative'] = 1; RkCoreN['Attribute'] = 1; RkCoreN['Awaitable'] = 1;
RkCoreN['Awaitable::Handle'] = 1; RkCoreN['Awaiter'] = 1; RkCoreN['Awaiter::Blocking'] = 1; RkCoreN['Backtrace'] = 1; RkCoreN['Backtrace::Frame'] = 1; RkCoreN['Bag'] = 1;
RkCoreN['BagHash'] = 1; RkCoreN['Baggy'] = 1; RkCoreN['BigEndian'] = 1; RkCoreN['Blob'] = 1; RkCoreN['Block'] = 1; RkCoreN['Bool'] = 1;
RkCoreN['Bool::False'] = 1; RkCoreN['Bool::True'] = 1; RkCoreN['Broken'] = 1; RkCoreN['Buf'] = 1; RkCoreN['CORE-SETTING-REV'] = 1; RkCoreN['CX'] = 1;
RkCoreN['CX::Done'] = 1; RkCoreN['CX::Emit'] = 1; RkCoreN['CX::Last'] = 1; RkCoreN['CX::Next'] = 1; RkCoreN['CX::Proceed'] = 1; RkCoreN['CX::Redo'] = 1;
RkCoreN['CX::Return'] = 1; RkCoreN['CX::Succeed'] = 1; RkCoreN['CX::Take'] = 1; RkCoreN['CX::Warn'] = 1; RkCoreN['CallFrame'] = 1; RkCoreN['Callable'] = 1;
RkCoreN['Cancellation'] = 1; RkCoreN['Capture'] = 1; RkCoreN['Channel'] = 1; RkCoreN['Code'] = 1; RkCoreN['Collation'] = 1; RkCoreN['CompUnit'] = 1;
RkCoreN['CompUnit::DependencySpecification'] = 1; RkCoreN['CompUnit::Handle'] = 1; RkCoreN['CompUnit::Loader'] = 1; RkCoreN['CompUnit::PrecompilationDependency'] = 1; RkCoreN['CompUnit::PrecompilationDependency::File'] = 1; RkCoreN['CompUnit::PrecompilationId'] = 1;
RkCoreN['CompUnit::PrecompilationRepository'] = 1; RkCoreN['CompUnit::PrecompilationRepository::Default'] = 1; RkCoreN['CompUnit::PrecompilationRepository::None'] = 1; RkCoreN['CompUnit::PrecompilationStore'] = 1; RkCoreN['CompUnit::PrecompilationStore::File'] = 1; RkCoreN['CompUnit::PrecompilationStore::FileSystem'] = 1;
RkCoreN['CompUnit::PrecompilationUnit'] = 1; RkCoreN['CompUnit::PrecompilationUnit::File'] = 1; RkCoreN['CompUnit::Repository'] = 1; RkCoreN['CompUnit::Repository::AbsolutePath'] = 1; RkCoreN['CompUnit::Repository::Distribution'] = 1; RkCoreN['CompUnit::Repository::FileSystem'] = 1;
RkCoreN['CompUnit::Repository::Installable'] = 1; RkCoreN['CompUnit::Repository::Installation'] = 1; RkCoreN['CompUnit::Repository::Locally'] = 1; RkCoreN['CompUnit::Repository::NQP'] = 1; RkCoreN['CompUnit::Repository::Perl5'] = 1; RkCoreN['CompUnit::Repository::Spec'] = 1;
RkCoreN['CompUnit::Repository::Unknown'] = 1; RkCoreN['CompUnit::RepositoryRegistry'] = 1; RkCoreN['Compiler'] = 1; RkCoreN['Complex'] = 1; RkCoreN['ComplexStr'] = 1; RkCoreN['ContainerDescriptor'] = 1;
RkCoreN['Cool'] = 1; RkCoreN['CurrentThreadScheduler'] = 1; RkCoreN['Cursor'] = 1; RkCoreN['Date'] = 1; RkCoreN['DateTime'] = 1; RkCoreN['Dateish'] = 1;
RkCoreN['Deprecation'] = 1; RkCoreN['Distribution'] = 1; RkCoreN['Distribution::Hash'] = 1; RkCoreN['Distribution::Locally'] = 1; RkCoreN['Distribution::Path'] = 1; RkCoreN['Distribution::Resource'] = 1;
RkCoreN['Distribution::Resources'] = 1; RkCoreN['Distro'] = 1; RkCoreN['Duration'] = 1; RkCoreN['Empty'] = 1; RkCoreN['Encoding'] = 1; RkCoreN['Encoding::Builtin'] = 1;
RkCoreN['Encoding::Decoder'] = 1; RkCoreN['Encoding::Decoder::Builtin'] = 1; RkCoreN['Encoding::Encoder'] = 1; RkCoreN['Encoding::Encoder::Builtin'] = 1; RkCoreN['Encoding::Encoder::TranslateNewlineWrapper'] = 1; RkCoreN['Encoding::Registry'] = 1;
RkCoreN['Endian'] = 1; RkCoreN['Endian::BigEndian'] = 1; RkCoreN['Endian::LittleEndian'] = 1; RkCoreN['Endian::NativeEndian'] = 1; RkCoreN['Enumeration'] = 1; RkCoreN['Exception'] = 1;
RkCoreN['Exceptions'] = 1; RkCoreN['Exceptions::JSON'] = 1; RkCoreN['Failure'] = 1; RkCoreN['False'] = 1; RkCoreN['FatRat'] = 1; RkCoreN['FileChangeEvent'] = 1;
RkCoreN['FileChangeEvent::FileChanged'] = 1; RkCoreN['FileChangeEvent::FileRenamed'] = 1; RkCoreN['FileChanged'] = 1; RkCoreN['FileRenamed'] = 1; RkCoreN['ForeignCode'] = 1; RkCoreN['Grammar'] = 1;
RkCoreN['HardRoutine'] = 1; RkCoreN['Hash'] = 1; RkCoreN['Hash::Object'] = 1; RkCoreN['Hash::Typed'] = 1; RkCoreN['Hyper'] = 1; RkCoreN['HyperConfiguration'] = 1;
RkCoreN['HyperSeq'] = 1; RkCoreN['HyperWhatever'] = 1; RkCoreN['IO'] = 1; RkCoreN['IO::ArgFiles'] = 1; RkCoreN['IO::CatHandle'] = 1; RkCoreN['IO::Handle'] = 1;
RkCoreN['IO::Notification'] = 1; RkCoreN['IO::Notification::Change'] = 1; RkCoreN['IO::Path'] = 1; RkCoreN['IO::Path::Cygwin'] = 1; RkCoreN['IO::Path::Parts'] = 1; RkCoreN['IO::Path::QNX'] = 1;
RkCoreN['IO::Path::Spec'] = 1; RkCoreN['IO::Path::Unix'] = 1; RkCoreN['IO::Path::Win32'] = 1; RkCoreN['IO::Path::slurp-size'] = 1; RkCoreN['IO::Pipe'] = 1; RkCoreN['IO::Socket'] = 1;
RkCoreN['IO::Socket::Async'] = 1; RkCoreN['IO::Socket::INET'] = 1; RkCoreN['IO::Spec'] = 1; RkCoreN['IO::Spec::Cygwin'] = 1; RkCoreN['IO::Spec::QNX'] = 1; RkCoreN['IO::Spec::Unix'] = 1;
RkCoreN['IO::Spec::Win32'] = 1; RkCoreN['IO::Special'] = 1; RkCoreN['Inf'] = 1; RkCoreN['Instant'] = 1; RkCoreN['Int'] = 1; RkCoreN['IntAttrRef'] = 1;
RkCoreN['IntLexRef'] = 1; RkCoreN['IntPosRef'] = 1; RkCoreN['IntStr'] = 1; RkCoreN['Iterable'] = 1; RkCoreN['IterationBuffer'] = 1; RkCoreN['IterationEnd'] = 1;
RkCoreN['Iterator'] = 1; RkCoreN['JSONException'] = 1; RkCoreN['Junction'] = 1; RkCoreN['Kept'] = 1; RkCoreN['Kernel'] = 1; RkCoreN['Label'] = 1;
RkCoreN['Less'] = 1; RkCoreN['List'] = 1; RkCoreN['List::Reifier'] = 1; RkCoreN['LittleEndian'] = 1; RkCoreN['Lock'] = 1; RkCoreN['Lock::Async'] = 1;
RkCoreN['Lock::ConditionVariable'] = 1; RkCoreN['Lock::Soft'] = 1; RkCoreN['Macro'] = 1; RkCoreN['Map'] = 1; RkCoreN['Match'] = 1; RkCoreN['Metamodel'] = 1;
RkCoreN['Metamodel::Archetypes'] = 1; RkCoreN['Metamodel::ArrayType'] = 1; RkCoreN['Metamodel::AttributeContainer'] = 1; RkCoreN['Metamodel::BUILDPLAN'] = 1; RkCoreN['Metamodel::BaseType'] = 1; RkCoreN['Metamodel::BoolificationProtocol'] = 1;
RkCoreN['Metamodel::C3MRO'] = 1; RkCoreN['Metamodel::ClassHOW'] = 1; RkCoreN['Metamodel::CoercionHOW'] = 1; RkCoreN['Metamodel::ConcreteRoleHOW'] = 1; RkCoreN['Metamodel::Concretization'] = 1; RkCoreN['Metamodel::ConcretizationCache'] = 1;
RkCoreN['Metamodel::Configuration'] = 1; RkCoreN['Metamodel::ContainerSpecProtocol'] = 1; RkCoreN['Metamodel::CurriedRoleHOW'] = 1; RkCoreN['Metamodel::DefaultParent'] = 1; RkCoreN['Metamodel::DefiniteHOW'] = 1; RkCoreN['Metamodel::Documenting'] = 1;
RkCoreN['Metamodel::EnumHOW'] = 1; RkCoreN['Metamodel::Explaining'] = 1; RkCoreN['Metamodel::Finalization'] = 1; RkCoreN['Metamodel::GenericHOW'] = 1; RkCoreN['Metamodel::GrammarHOW'] = 1; RkCoreN['Metamodel::InvocationProtocol'] = 1;
RkCoreN['Metamodel::LanguageRevision'] = 1; RkCoreN['Metamodel::MROBasedMethodDispatch'] = 1; RkCoreN['Metamodel::MROBasedTypeChecking'] = 1; RkCoreN['Metamodel::MetaMethodContainer'] = 1; RkCoreN['Metamodel::MethodContainer'] = 1; RkCoreN['Metamodel::MethodDelegation'] = 1;
RkCoreN['Metamodel::Mixins'] = 1; RkCoreN['Metamodel::ModuleHOW'] = 1; RkCoreN['Metamodel::MultiMethodContainer'] = 1; RkCoreN['Metamodel::MultipleInheritance'] = 1; RkCoreN['Metamodel::Naming'] = 1; RkCoreN['Metamodel::NativeHOW'] = 1;
RkCoreN['Metamodel::NativeRefHOW'] = 1; RkCoreN['Metamodel::Nominalizable'] = 1; RkCoreN['Metamodel::PackageHOW'] = 1; RkCoreN['Metamodel::ParametricRoleGroupHOW'] = 1; RkCoreN['Metamodel::ParametricRoleHOW'] = 1; RkCoreN['Metamodel::Primitives'] = 1;
RkCoreN['Metamodel::PrivateMethodContainer'] = 1; RkCoreN['Metamodel::REPRComposeProtocol'] = 1; RkCoreN['Metamodel::RoleContainer'] = 1; RkCoreN['Metamodel::RolePunning'] = 1; RkCoreN['Metamodel::Stashing'] = 1; RkCoreN['Metamodel::SubsetHOW'] = 1;
RkCoreN['Metamodel::Trusting'] = 1; RkCoreN['Metamodel::TypePretense'] = 1; RkCoreN['Metamodel::Versioning'] = 1; RkCoreN['Method'] = 1; RkCoreN['MethodDispatcher'] = 1; RkCoreN['Mix'] = 1;
RkCoreN['MixHash'] = 1; RkCoreN['Mixy'] = 1; RkCoreN['More'] = 1; RkCoreN['Mu'] = 1; RkCoreN['MultiDispatcher'] = 1; RkCoreN['NFC'] = 1;
RkCoreN['NFD'] = 1; RkCoreN['NFKC'] = 1; RkCoreN['NFKD'] = 1; RkCoreN['NQPMatchRole'] = 1; RkCoreN['NQPdidMATCH'] = 1; RkCoreN['NaN'] = 1;
RkCoreN['NativeEndian'] = 1; RkCoreN['Nil'] = 1; RkCoreN['Num'] = 1; RkCoreN['NumAttrRef'] = 1; RkCoreN['NumLexRef'] = 1; RkCoreN['NumPosRef'] = 1;
RkCoreN['NumStr'] = 1; RkCoreN['Numeric'] = 1; RkCoreN['NumericEnumeration'] = 1; RkCoreN['NumericStringyEnumeration'] = 1; RkCoreN['ObjAt'] = 1; RkCoreN['Order'] = 1;
RkCoreN['Order::Less'] = 1; RkCoreN['Order::More'] = 1; RkCoreN['Order::Same'] = 1; RkCoreN['PF_INET'] = 1; RkCoreN['PF_INET6'] = 1; RkCoreN['PF_LOCAL'] = 1;
RkCoreN['PF_MAX'] = 1; RkCoreN['PF_UNIX'] = 1; RkCoreN['PF_UNSPEC'] = 1; RkCoreN['PROTO_TCP'] = 1; RkCoreN['PROTO_UDP'] = 1; RkCoreN['Pair'] = 1;
RkCoreN['ParallelSequence'] = 1; RkCoreN['Parameter'] = 1; RkCoreN['Perl'] = 1; RkCoreN['Planned'] = 1; RkCoreN['Pod'] = 1; RkCoreN['Pod::Block'] = 1;
RkCoreN['Pod::Block::Code'] = 1; RkCoreN['Pod::Block::Comment'] = 1; RkCoreN['Pod::Block::Declarator'] = 1; RkCoreN['Pod::Block::Named'] = 1; RkCoreN['Pod::Block::Para'] = 1; RkCoreN['Pod::Block::Table'] = 1;
RkCoreN['Pod::Config'] = 1; RkCoreN['Pod::Defn'] = 1; RkCoreN['Pod::FormattingCode'] = 1; RkCoreN['Pod::Heading'] = 1; RkCoreN['Pod::Item'] = 1; RkCoreN['Pod::Raw'] = 1;
RkCoreN['Positional'] = 1; RkCoreN['PositionalBindFailover'] = 1; RkCoreN['PredictiveIterator'] = 1; RkCoreN['Proc'] = 1; RkCoreN['Proc::Async'] = 1; RkCoreN['Proc::Async::Pipe'] = 1;
RkCoreN['Promise'] = 1; RkCoreN['PromiseStatus'] = 1; RkCoreN['PromiseStatus::Broken'] = 1; RkCoreN['PromiseStatus::Kept'] = 1; RkCoreN['PromiseStatus::Planned'] = 1; RkCoreN['ProtocolFamily'] = 1;
RkCoreN['ProtocolFamily::PF_INET'] = 1; RkCoreN['ProtocolFamily::PF_INET6'] = 1; RkCoreN['ProtocolFamily::PF_LOCAL'] = 1; RkCoreN['ProtocolFamily::PF_MAX'] = 1; RkCoreN['ProtocolFamily::PF_UNIX'] = 1; RkCoreN['ProtocolFamily::PF_UNSPEC'] = 1;
RkCoreN['ProtocolType'] = 1; RkCoreN['ProtocolType::PROTO_TCP'] = 1; RkCoreN['ProtocolType::PROTO_UDP'] = 1; RkCoreN['Proxy'] = 1; RkCoreN['PseudoStash'] = 1; RkCoreN['QuantHash'] = 1;
RkCoreN['REPL'] = 1; RkCoreN['RaceSeq'] = 1; RkCoreN['Raku'] = 1; RkCoreN['Rakudo'] = 1; RkCoreN['Rakudo::Deprecations'] = 1; RkCoreN['Rakudo::Internals'] = 1;
RkCoreN['Rakudo::Internals::CompilerServices'] = 1; RkCoreN['Rakudo::Internals::EvalIdSource'] = 1; RkCoreN['Rakudo::Internals::HyperBatcher'] = 1; RkCoreN['Rakudo::Internals::HyperIteratorBatcher'] = 1; RkCoreN['Rakudo::Internals::HyperJoiner'] = 1; RkCoreN['Rakudo::Internals::HyperPipeline'] = 1;
RkCoreN['Rakudo::Internals::HyperProcessor'] = 1; RkCoreN['Rakudo::Internals::HyperRaceSharedImpl'] = 1; RkCoreN['Rakudo::Internals::HyperRebatcher'] = 1; RkCoreN['Rakudo::Internals::HyperToIterator'] = 1; RkCoreN['Rakudo::Internals::HyperWorkBatch'] = 1; RkCoreN['Rakudo::Internals::HyperWorkStage'] = 1;
RkCoreN['Rakudo::Internals::ImplementationDetail'] = 1; RkCoreN['Rakudo::Internals::IterationSet'] = 1; RkCoreN['Rakudo::Internals::JSON'] = 1; RkCoreN['Rakudo::Internals::LoweredAwayLexical'] = 1; RkCoreN['Rakudo::Internals::RaceToIterator'] = 1; RkCoreN['Rakudo::Internals::ReactAwaitHandle'] = 1;
RkCoreN['Rakudo::Internals::ReactAwaitable'] = 1; RkCoreN['Rakudo::Internals::ReactOneWheneverAwaitHandle'] = 1; RkCoreN['Rakudo::Internals::RegexBoolification6cMarker'] = 1; RkCoreN['Rakudo::Internals::ShapedArrayCommon'] = 1; RkCoreN['Rakudo::Internals::SprintfHandler'] = 1; RkCoreN['Rakudo::Internals::SupplySequencer'] = 1;
RkCoreN['Rakudo::Iterator'] = 1; RkCoreN['Rakudo::Iterator::Blobby'] = 1; RkCoreN['Rakudo::Iterator::Dir'] = 1; RkCoreN['Rakudo::Iterator::DirTest'] = 1; RkCoreN['Rakudo::Iterator::Mappy'] = 1; RkCoreN['Rakudo::Iterator::Mappy-kv-from-pairs'] = 1;
RkCoreN['Rakudo::Iterator::ShapeBranch'] = 1; RkCoreN['Rakudo::Iterator::ShapeLeaf'] = 1; RkCoreN['Rakudo::Metaops'] = 1; RkCoreN['Rakudo::QuantHash'] = 1; RkCoreN['Rakudo::QuantHash::Pairs'] = 1; RkCoreN['Rakudo::QuantHash::Quanty-kv'] = 1;
RkCoreN['Rakudo::SlippyIterator'] = 1; RkCoreN['Rakudo::Sorting'] = 1; RkCoreN['Rakudo::Supply'] = 1; RkCoreN['Rakudo::Supply::BlockAddWheneverAwaiter'] = 1; RkCoreN['Rakudo::Supply::BlockState'] = 1; RkCoreN['Rakudo::Supply::BlockTappable'] = 1;
RkCoreN['Rakudo::Supply::CachedAwaitHandle'] = 1; RkCoreN['Rakudo::Supply::OneEmitTappable'] = 1; RkCoreN['Rakudo::Supply::OneWheneverState'] = 1; RkCoreN['Rakudo::Supply::OneWheneverTappable'] = 1; RkCoreN['Rakudo::Unicodey'] = 1; RkCoreN['Range'] = 1;
RkCoreN['Rat'] = 1; RkCoreN['RatStr'] = 1; RkCoreN['Rational'] = 1; RkCoreN['Real'] = 1; RkCoreN['Regex'] = 1; RkCoreN['Routine'] = 1;
RkCoreN['SIGABRT'] = 1; RkCoreN['SIGALRM'] = 1; RkCoreN['SIGBREAK'] = 1; RkCoreN['SIGBUS'] = 1; RkCoreN['SIGCHLD'] = 1; RkCoreN['SIGCONT'] = 1;
RkCoreN['SIGEMT'] = 1; RkCoreN['SIGFPE'] = 1; RkCoreN['SIGHUP'] = 1; RkCoreN['SIGILL'] = 1; RkCoreN['SIGINFO'] = 1; RkCoreN['SIGINT'] = 1;
RkCoreN['SIGIO'] = 1; RkCoreN['SIGKILL'] = 1; RkCoreN['SIGPIPE'] = 1; RkCoreN['SIGPROF'] = 1; RkCoreN['SIGPWR'] = 1; RkCoreN['SIGQUIT'] = 1;
RkCoreN['SIGSEGV'] = 1; RkCoreN['SIGSTKFLT'] = 1; RkCoreN['SIGSTOP'] = 1; RkCoreN['SIGSYS'] = 1; RkCoreN['SIGTERM'] = 1; RkCoreN['SIGTHR'] = 1;
RkCoreN['SIGTRAP'] = 1; RkCoreN['SIGTSTP'] = 1; RkCoreN['SIGTTIN'] = 1; RkCoreN['SIGTTOU'] = 1; RkCoreN['SIGURG'] = 1; RkCoreN['SIGUSR1'] = 1;
RkCoreN['SIGUSR2'] = 1; RkCoreN['SIGVTALRM'] = 1; RkCoreN['SIGWINCH'] = 1; RkCoreN['SIGXCPU'] = 1; RkCoreN['SIGXFSZ'] = 1; RkCoreN['SOCK_DGRAM'] = 1;
RkCoreN['SOCK_MAX'] = 1; RkCoreN['SOCK_PACKET'] = 1; RkCoreN['SOCK_RAW'] = 1; RkCoreN['SOCK_RDM'] = 1; RkCoreN['SOCK_SEQPACKET'] = 1; RkCoreN['SOCK_STREAM'] = 1;
RkCoreN['Same'] = 1; RkCoreN['Scalar'] = 1; RkCoreN['ScalarVAR'] = 1; RkCoreN['Scheduler'] = 1; RkCoreN['SeekFromBeginning'] = 1; RkCoreN['SeekFromCurrent'] = 1;
RkCoreN['SeekFromEnd'] = 1; RkCoreN['SeekType'] = 1; RkCoreN['SeekType::SeekFromBeginning'] = 1; RkCoreN['SeekType::SeekFromCurrent'] = 1; RkCoreN['SeekType::SeekFromEnd'] = 1; RkCoreN['Semaphore'] = 1;
RkCoreN['Seq'] = 1; RkCoreN['Sequence'] = 1; RkCoreN['Set'] = 1; RkCoreN['SetHash'] = 1; RkCoreN['Setty'] = 1; RkCoreN['Signal'] = 1;
RkCoreN['Signal::SIGABRT'] = 1; RkCoreN['Signal::SIGALRM'] = 1; RkCoreN['Signal::SIGBREAK'] = 1; RkCoreN['Signal::SIGBUS'] = 1; RkCoreN['Signal::SIGCHLD'] = 1; RkCoreN['Signal::SIGCONT'] = 1;
RkCoreN['Signal::SIGEMT'] = 1; RkCoreN['Signal::SIGFPE'] = 1; RkCoreN['Signal::SIGHUP'] = 1; RkCoreN['Signal::SIGILL'] = 1; RkCoreN['Signal::SIGINFO'] = 1; RkCoreN['Signal::SIGINT'] = 1;
RkCoreN['Signal::SIGIO'] = 1; RkCoreN['Signal::SIGKILL'] = 1; RkCoreN['Signal::SIGPIPE'] = 1; RkCoreN['Signal::SIGPROF'] = 1; RkCoreN['Signal::SIGPWR'] = 1; RkCoreN['Signal::SIGQUIT'] = 1;
RkCoreN['Signal::SIGSEGV'] = 1; RkCoreN['Signal::SIGSTKFLT'] = 1; RkCoreN['Signal::SIGSTOP'] = 1; RkCoreN['Signal::SIGSYS'] = 1; RkCoreN['Signal::SIGTERM'] = 1; RkCoreN['Signal::SIGTHR'] = 1;
RkCoreN['Signal::SIGTRAP'] = 1; RkCoreN['Signal::SIGTSTP'] = 1; RkCoreN['Signal::SIGTTIN'] = 1; RkCoreN['Signal::SIGTTOU'] = 1; RkCoreN['Signal::SIGURG'] = 1; RkCoreN['Signal::SIGUSR1'] = 1;
RkCoreN['Signal::SIGUSR2'] = 1; RkCoreN['Signal::SIGVTALRM'] = 1; RkCoreN['Signal::SIGWINCH'] = 1; RkCoreN['Signal::SIGXCPU'] = 1; RkCoreN['Signal::SIGXFSZ'] = 1; RkCoreN['Signature'] = 1;
RkCoreN['SignedBlob'] = 1; RkCoreN['Slang'] = 1; RkCoreN['Slip'] = 1; RkCoreN['SocketType'] = 1; RkCoreN['SocketType::SOCK_DGRAM'] = 1; RkCoreN['SocketType::SOCK_MAX'] = 1;
RkCoreN['SocketType::SOCK_PACKET'] = 1; RkCoreN['SocketType::SOCK_RAW'] = 1; RkCoreN['SocketType::SOCK_RDM'] = 1; RkCoreN['SocketType::SOCK_SEQPACKET'] = 1; RkCoreN['SocketType::SOCK_STREAM'] = 1; RkCoreN['SoftRoutine'] = 1;
RkCoreN['Stash'] = 1; RkCoreN['Str'] = 1; RkCoreN['StrAttrRef'] = 1; RkCoreN['StrDistance'] = 1; RkCoreN['StrLexRef'] = 1; RkCoreN['StrPosRef'] = 1;
RkCoreN['Stringy'] = 1; RkCoreN['StringyEnumeration'] = 1; RkCoreN['Sub'] = 1; RkCoreN['Submethod'] = 1; RkCoreN['Supplier'] = 1; RkCoreN['Supplier::Preserving'] = 1;
RkCoreN['Supply'] = 1; RkCoreN['Systemic'] = 1; RkCoreN['Tap'] = 1; RkCoreN['Tappable'] = 1; RkCoreN['Thread'] = 1; RkCoreN['Thread::THREAD_ERROR'] = 1;
RkCoreN['ThreadPoolScheduler'] = 1; RkCoreN['ThreadPoolScheduler::ThreadPoolAwaiter'] = 1; RkCoreN['True'] = 1; RkCoreN['UINT64_UPPER'] = 1; RkCoreN['UInt'] = 1; RkCoreN['UIntAttrRef'] = 1;
RkCoreN['UIntLexRef'] = 1; RkCoreN['UIntPosRef'] = 1; RkCoreN['Uni'] = 1; RkCoreN['UnsignedBlob'] = 1; RkCoreN['VM'] = 1; RkCoreN['ValueObjAt'] = 1;
RkCoreN['Variable'] = 1; RkCoreN['Version'] = 1; RkCoreN['WalkList'] = 1; RkCoreN['Whatever'] = 1; RkCoreN['WhateverCode'] = 1; RkCoreN['WrapDispatcher'] = 1;
RkCoreN['X'] = 1; RkCoreN['X::AdHoc'] = 1; RkCoreN['X::Adverb'] = 1; RkCoreN['X::Anon'] = 1; RkCoreN['X::Anon::Augment'] = 1; RkCoreN['X::Anon::Multi'] = 1;
RkCoreN['X::ArrayShapeMismatch'] = 1; RkCoreN['X::Assignment'] = 1; RkCoreN['X::Assignment::ArrayShapeMismatch'] = 1; RkCoreN['X::Assignment::RO'] = 1; RkCoreN['X::Assignment::RO::Comp'] = 1; RkCoreN['X::Assignment::ToShaped'] = 1;
RkCoreN['X::Attribute'] = 1; RkCoreN['X::Attribute::NoPackage'] = 1; RkCoreN['X::Attribute::Package'] = 1; RkCoreN['X::Attribute::Regex'] = 1; RkCoreN['X::Attribute::Required'] = 1; RkCoreN['X::Attribute::Scope'] = 1;
RkCoreN['X::Attribute::Scope::Package'] = 1; RkCoreN['X::Attribute::Undeclared'] = 1; RkCoreN['X::Augment'] = 1; RkCoreN['X::Augment::NoSuchType'] = 1; RkCoreN['X::Await'] = 1; RkCoreN['X::Await::Died'] = 1;
RkCoreN['X::Backslash'] = 1; RkCoreN['X::Backslash::NonVariableDollar'] = 1; RkCoreN['X::Backslash::UnrecognizedSequence'] = 1; RkCoreN['X::BadType'] = 1; RkCoreN['X::Bind'] = 1; RkCoreN['X::Bind::NativeType'] = 1;
RkCoreN['X::Bind::Rebind'] = 1; RkCoreN['X::Bind::Slice'] = 1; RkCoreN['X::Bind::ZenSlice'] = 1; RkCoreN['X::Buf'] = 1; RkCoreN['X::Buf::AsStr'] = 1; RkCoreN['X::Buf::Pack'] = 1;
RkCoreN['X::Buf::Pack::NonASCII'] = 1; RkCoreN['X::Caller'] = 1; RkCoreN['X::Caller::NotDynamic'] = 1; RkCoreN['X::Cannot'] = 1; RkCoreN['X::Cannot::Capture'] = 1; RkCoreN['X::Cannot::Empty'] = 1;
RkCoreN['X::Cannot::Lazy'] = 1; RkCoreN['X::Cannot::Map'] = 1; RkCoreN['X::Cannot::New'] = 1; RkCoreN['X::Channel'] = 1; RkCoreN['X::Channel::ReceiveOnClosed'] = 1; RkCoreN['X::Channel::SendOnClosed'] = 1;
RkCoreN['X::Coerce'] = 1; RkCoreN['X::Coerce::Impossible'] = 1; RkCoreN['X::Comp'] = 1; RkCoreN['X::Comp::AdHoc'] = 1; RkCoreN['X::Comp::BeginTime'] = 1; RkCoreN['X::Comp::FailGoal'] = 1;
RkCoreN['X::Comp::Group'] = 1; RkCoreN['X::Comp::NYI'] = 1; RkCoreN['X::Comp::Trait'] = 1; RkCoreN['X::Comp::Trait::Invalid'] = 1; RkCoreN['X::Comp::Trait::NotOnNative'] = 1; RkCoreN['X::Comp::Trait::Scope'] = 1;
RkCoreN['X::Comp::Trait::Unknown'] = 1; RkCoreN['X::Comp::WheneverOutOfScope'] = 1; RkCoreN['X::CompUnit'] = 1; RkCoreN['X::CompUnit::UnsatisfiedDependency'] = 1; RkCoreN['X::Composition'] = 1; RkCoreN['X::Composition::NotComposable'] = 1;
RkCoreN['X::Constructor'] = 1; RkCoreN['X::Constructor::BadType'] = 1; RkCoreN['X::Constructor::Positional'] = 1; RkCoreN['X::Control'] = 1; RkCoreN['X::ControlFlow'] = 1; RkCoreN['X::ControlFlow::Return'] = 1;
RkCoreN['X::DateTime'] = 1; RkCoreN['X::DateTime::InvalidDeltaUnit'] = 1; RkCoreN['X::DateTime::TimezoneClash'] = 1; RkCoreN['X::Declaration'] = 1; RkCoreN['X::Declaration::OurScopeInRole'] = 1; RkCoreN['X::Declaration::Scope'] = 1;
RkCoreN['X::Declaration::Scope::Multi'] = 1; RkCoreN['X::Delete'] = 1; RkCoreN['X::Does'] = 1; RkCoreN['X::Does::TypeObject'] = 1; RkCoreN['X::Dynamic'] = 1; RkCoreN['X::Dynamic::NotFound'] = 1;
RkCoreN['X::Dynamic::Package'] = 1; RkCoreN['X::Dynamic::Postdeclaration'] = 1; RkCoreN['X::Encoding'] = 1; RkCoreN['X::Encoding::AlreadyRegistered'] = 1; RkCoreN['X::Encoding::Unknown'] = 1; RkCoreN['X::Enum'] = 1;
RkCoreN['X::Enum::NoValue'] = 1; RkCoreN['X::Eval'] = 1; RkCoreN['X::Eval::NoSuchLang'] = 1; RkCoreN['X::Exhausted'] = 1; RkCoreN['X::Experimental'] = 1; RkCoreN['X::Export'] = 1;
RkCoreN['X::Export::NameClash'] = 1; RkCoreN['X::Hash'] = 1; RkCoreN['X::Hash::Store'] = 1; RkCoreN['X::Hash::Store::OddNumber'] = 1; RkCoreN['X::HyperOp'] = 1; RkCoreN['X::HyperOp::Infinite'] = 1;
RkCoreN['X::HyperOp::NonDWIM'] = 1; RkCoreN['X::HyperRace'] = 1; RkCoreN['X::HyperRace::Died'] = 1; RkCoreN['X::HyperWhatever'] = 1; RkCoreN['X::HyperWhatever::Multiple'] = 1; RkCoreN['X::IO'] = 1;
RkCoreN['X::IO::BinaryAndEncoding'] = 1; RkCoreN['X::IO::BinaryMode'] = 1; RkCoreN['X::IO::Chdir'] = 1; RkCoreN['X::IO::Chmod'] = 1; RkCoreN['X::IO::Chown'] = 1; RkCoreN['X::IO::Closed'] = 1;
RkCoreN['X::IO::Copy'] = 1; RkCoreN['X::IO::Cwd'] = 1; RkCoreN['X::IO::Dir'] = 1; RkCoreN['X::IO::Directory'] = 1; RkCoreN['X::IO::DoesNotExist'] = 1; RkCoreN['X::IO::Flush'] = 1;
RkCoreN['X::IO::Link'] = 1; RkCoreN['X::IO::Lock'] = 1; RkCoreN['X::IO::Mkdir'] = 1; RkCoreN['X::IO::Move'] = 1; RkCoreN['X::IO::NotAChild'] = 1; RkCoreN['X::IO::NotAFile'] = 1;
RkCoreN['X::IO::Null'] = 1; RkCoreN['X::IO::Rename'] = 1; RkCoreN['X::IO::Resolve'] = 1; RkCoreN['X::IO::Rmdir'] = 1; RkCoreN['X::IO::Symlink'] = 1; RkCoreN['X::IO::Unknown'] = 1;
RkCoreN['X::IO::Unlink'] = 1; RkCoreN['X::IllegalDimensionInShape'] = 1; RkCoreN['X::IllegalOnFixedDimensionArray'] = 1; RkCoreN['X::Immutable'] = 1; RkCoreN['X::Import'] = 1; RkCoreN['X::Import::MissingSymbols'] = 1;
RkCoreN['X::Import::NoSuchTag'] = 1; RkCoreN['X::Import::OnlystarProto'] = 1; RkCoreN['X::Import::Positional'] = 1; RkCoreN['X::Import::Redeclaration'] = 1; RkCoreN['X::Inheritance'] = 1; RkCoreN['X::Inheritance::NotComposed'] = 1;
RkCoreN['X::Inheritance::SelfInherit'] = 1; RkCoreN['X::Inheritance::UnknownParent'] = 1; RkCoreN['X::Inheritance::Unsupported'] = 1; RkCoreN['X::Invalid'] = 1; RkCoreN['X::Invalid::ComputedValue'] = 1; RkCoreN['X::Invalid::Value'] = 1;
RkCoreN['X::InvalidCodepoint'] = 1; RkCoreN['X::InvalidType'] = 1; RkCoreN['X::InvalidTypeSmiley'] = 1; RkCoreN['X::Item'] = 1; RkCoreN['X::Language'] = 1; RkCoreN['X::Language::IncompatRevisions'] = 1;
RkCoreN['X::Language::ModRequired'] = 1; RkCoreN['X::Language::TooLate'] = 1; RkCoreN['X::Language::Unsupported'] = 1; RkCoreN['X::LibEmpty'] = 1; RkCoreN['X::LibNone'] = 1; RkCoreN['X::Localizer'] = 1;
RkCoreN['X::Localizer::NoContainer'] = 1; RkCoreN['X::Lock'] = 1; RkCoreN['X::Lock::Async'] = 1; RkCoreN['X::Lock::Async::NotLocked'] = 1; RkCoreN['X::Lock::ConditionVariable'] = 1; RkCoreN['X::Lock::ConditionVariable::Duplicate'] = 1;
RkCoreN['X::Lock::ConditionVariable::New'] = 1; RkCoreN['X::Lock::ConditionVariable::NoMutex'] = 1; RkCoreN['X::Lock::ConditionVariable::WrongThread'] = 1; RkCoreN['X::Lock::Unlock'] = 1; RkCoreN['X::Lock::Unlock::NoMutex'] = 1; RkCoreN['X::Lock::Unlock::WrongThread'] = 1;
RkCoreN['X::MOP'] = 1; RkCoreN['X::Make'] = 1; RkCoreN['X::Make::MatchRequired'] = 1; RkCoreN['X::Match'] = 1; RkCoreN['X::Match::Bool'] = 1; RkCoreN['X::Method'] = 1;
RkCoreN['X::Method::Duplicate'] = 1; RkCoreN['X::Method::InvalidQualifier'] = 1; RkCoreN['X::Method::NotFound'] = 1; RkCoreN['X::Method::Private'] = 1; RkCoreN['X::Method::Private::Permission'] = 1; RkCoreN['X::Method::Private::Unqualified'] = 1;
RkCoreN['X::Mixin'] = 1; RkCoreN['X::Mixin::NotComposable'] = 1; RkCoreN['X::Multi'] = 1; RkCoreN['X::Multi::Ambiguous'] = 1; RkCoreN['X::Multi::NoMatch'] = 1; RkCoreN['X::MultipleTypeSmiley'] = 1;
RkCoreN['X::MustBeParametric'] = 1; RkCoreN['X::NQP'] = 1; RkCoreN['X::NQP::NotFound'] = 1; RkCoreN['X::NYI'] = 1; RkCoreN['X::NYI::Available'] = 1; RkCoreN['X::NYI::BigInt'] = 1;
RkCoreN['X::NoCoreRevision'] = 1; RkCoreN['X::NoDispatcher'] = 1; RkCoreN['X::NoSuchSymbol'] = 1; RkCoreN['X::Nominalizable'] = 1; RkCoreN['X::Nominalizable::NoKind'] = 1; RkCoreN['X::Nominalizable::NoWrappee'] = 1;
RkCoreN['X::NotEnoughDimensions'] = 1; RkCoreN['X::NotFoundInRepository'] = 1; RkCoreN['X::NotParametric'] = 1; RkCoreN['X::Numeric'] = 1; RkCoreN['X::Numeric::CannotConvert'] = 1; RkCoreN['X::Numeric::Confused'] = 1;
RkCoreN['X::Numeric::DivideByZero'] = 1; RkCoreN['X::Numeric::Overflow'] = 1; RkCoreN['X::Numeric::Real'] = 1; RkCoreN['X::Numeric::Underflow'] = 1; RkCoreN['X::Numeric::Uninitialized'] = 1; RkCoreN['X::OS'] = 1;
RkCoreN['X::Obsolete'] = 1; RkCoreN['X::OutOfRange'] = 1; RkCoreN['X::Package'] = 1; RkCoreN['X::Package::Stubbed'] = 1; RkCoreN['X::Package::UseLib'] = 1; RkCoreN['X::Pairup'] = 1;
RkCoreN['X::Pairup::OddNumber'] = 1; RkCoreN['X::Parameter'] = 1; RkCoreN['X::Parameter::AfterDefault'] = 1; RkCoreN['X::Parameter::BadType'] = 1; RkCoreN['X::Parameter::Default'] = 1; RkCoreN['X::Parameter::Default::TypeCheck'] = 1;
RkCoreN['X::Parameter::InvalidConcreteness'] = 1; RkCoreN['X::Parameter::InvalidType'] = 1; RkCoreN['X::Parameter::MultipleTypeConstraints'] = 1; RkCoreN['X::Parameter::Placeholder'] = 1; RkCoreN['X::Parameter::RW'] = 1; RkCoreN['X::Parameter::Twigil'] = 1;
RkCoreN['X::Parameter::TypedSlurpy'] = 1; RkCoreN['X::Parameter::WrongOrder'] = 1; RkCoreN['X::ParametricConstant'] = 1; RkCoreN['X::Phaser'] = 1; RkCoreN['X::Phaser::Multiple'] = 1; RkCoreN['X::Phaser::PrePost'] = 1;
RkCoreN['X::PhaserExceptions'] = 1; RkCoreN['X::Placeholder'] = 1; RkCoreN['X::Placeholder::Attribute'] = 1; RkCoreN['X::Placeholder::Block'] = 1; RkCoreN['X::Placeholder::Mainline'] = 1; RkCoreN['X::Placeholder::NonPlaceholder'] = 1;
RkCoreN['X::Pod'] = 1; RkCoreN['X::PoisonedAlias'] = 1; RkCoreN['X::Pragma'] = 1; RkCoreN['X::Pragma::CannotPrecomp'] = 1; RkCoreN['X::Pragma::CannotWhat'] = 1; RkCoreN['X::Pragma::MustOneOf'] = 1;
RkCoreN['X::Pragma::NoArgs'] = 1; RkCoreN['X::Pragma::OnlyOne'] = 1; RkCoreN['X::Pragma::UnknownArg'] = 1; RkCoreN['X::Proc'] = 1; RkCoreN['X::Proc::Async'] = 1; RkCoreN['X::Proc::Async::AlreadyStarted'] = 1;
RkCoreN['X::Proc::Async::BindOrUse'] = 1; RkCoreN['X::Proc::Async::CharsOrBytes'] = 1; RkCoreN['X::Proc::Async::MustBeStarted'] = 1; RkCoreN['X::Proc::Async::OpenForWriting'] = 1; RkCoreN['X::Proc::Async::SupplyOrStd'] = 1; RkCoreN['X::Proc::Async::TapBeforeSpawn'] = 1;
RkCoreN['X::Proc::Unsuccessful'] = 1; RkCoreN['X::Promise'] = 1; RkCoreN['X::Promise::Broken'] = 1; RkCoreN['X::Promise::CauseOnlyValidOnBroken'] = 1; RkCoreN['X::Promise::Combinator'] = 1; RkCoreN['X::Promise::Resolved'] = 1;
RkCoreN['X::Promise::Vowed'] = 1; RkCoreN['X::PseudoPackage'] = 1; RkCoreN['X::PseudoPackage::InDeclaration'] = 1; RkCoreN['X::Range'] = 1; RkCoreN['X::Range::Incomparable'] = 1; RkCoreN['X::Range::InvalidArg'] = 1;
RkCoreN['X::React'] = 1; RkCoreN['X::React::Died'] = 1; RkCoreN['X::Redeclaration'] = 1; RkCoreN['X::Redeclaration::Outer'] = 1; RkCoreN['X::Role'] = 1; RkCoreN['X::Role::Attribute'] = 1;
RkCoreN['X::Role::Attribute::Conflicts'] = 1; RkCoreN['X::Role::Attribute::Exists'] = 1; RkCoreN['X::Role::Group'] = 1; RkCoreN['X::Role::Group::Documenting'] = 1; RkCoreN['X::Role::Initialization'] = 1; RkCoreN['X::Role::Parametric'] = 1;
RkCoreN['X::Role::Parametric::NoSuchCandidate'] = 1; RkCoreN['X::Role::Unimplemented'] = 1; RkCoreN['X::Role::Unimplemented::Multi'] = 1; RkCoreN['X::Role::Unresolved'] = 1; RkCoreN['X::Role::Unresolved::Method'] = 1; RkCoreN['X::Role::Unresolved::Multi'] = 1;
RkCoreN['X::Role::Unresolved::Private'] = 1; RkCoreN['X::RoleApplier'] = 1; RkCoreN['X::RoleApplier::Method'] = 1; RkCoreN['X::Routine'] = 1; RkCoreN['X::Routine::Unwrap'] = 1; RkCoreN['X::Scheduler'] = 1;
RkCoreN['X::Scheduler::CueInNaNSeconds'] = 1; RkCoreN['X::SecurityPolicy'] = 1; RkCoreN['X::SecurityPolicy::Eval'] = 1; RkCoreN['X::Seq'] = 1; RkCoreN['X::Seq::Consumed'] = 1; RkCoreN['X::Seq::NotIndexable'] = 1;
RkCoreN['X::Sequence'] = 1; RkCoreN['X::Sequence::Deduction'] = 1; RkCoreN['X::Sequence::Endpoint'] = 1; RkCoreN['X::Set'] = 1; RkCoreN['X::Set::Coerce'] = 1; RkCoreN['X::Signature'] = 1;
RkCoreN['X::Signature::NameClash'] = 1; RkCoreN['X::Signature::Placeholder'] = 1; RkCoreN['X::Str'] = 1; RkCoreN['X::Str::InvalidCharName'] = 1; RkCoreN['X::Str::Match'] = 1; RkCoreN['X::Str::Match::x'] = 1;
RkCoreN['X::Str::Numeric'] = 1; RkCoreN['X::Str::Sprintf'] = 1; RkCoreN['X::Str::Sprintf::Directives'] = 1; RkCoreN['X::Str::Sprintf::Directives::BadType'] = 1; RkCoreN['X::Str::Sprintf::Directives::Count'] = 1; RkCoreN['X::Str::Sprintf::Directives::Unsupported'] = 1;
RkCoreN['X::Str::Subst'] = 1; RkCoreN['X::Str::Subst::Adverb'] = 1; RkCoreN['X::Str::Trans'] = 1; RkCoreN['X::Str::Trans::IllegalKey'] = 1; RkCoreN['X::Str::Trans::InvalidArg'] = 1; RkCoreN['X::StubCode'] = 1;
RkCoreN['X::Subscript'] = 1; RkCoreN['X::Subscript::Negative'] = 1; RkCoreN['X::Supply'] = 1; RkCoreN['X::Supply::Combinator'] = 1; RkCoreN['X::Supply::Migrate'] = 1; RkCoreN['X::Supply::Migrate::Needs'] = 1;
RkCoreN['X::Supply::New'] = 1; RkCoreN['X::Syntax'] = 1; RkCoreN['X::Syntax::AddCategorical'] = 1; RkCoreN['X::Syntax::AddCategorical::TooFewParts'] = 1; RkCoreN['X::Syntax::AddCategorical::TooManyParts'] = 1; RkCoreN['X::Syntax::Adverb'] = 1;
RkCoreN['X::Syntax::Argument'] = 1; RkCoreN['X::Syntax::Argument::MOPMacro'] = 1; RkCoreN['X::Syntax::Augment'] = 1; RkCoreN['X::Syntax::Augment::Adverb'] = 1; RkCoreN['X::Syntax::Augment::Illegal'] = 1; RkCoreN['X::Syntax::Augment::WithoutMonkeyTyping'] = 1;
RkCoreN['X::Syntax::BlockGobbled'] = 1; RkCoreN['X::Syntax::CannotMeta'] = 1; RkCoreN['X::Syntax::Coercer'] = 1; RkCoreN['X::Syntax::Coercer::TooComplex'] = 1; RkCoreN['X::Syntax::Comment'] = 1; RkCoreN['X::Syntax::Comment::Embedded'] = 1;
RkCoreN['X::Syntax::ConditionalOperator'] = 1; RkCoreN['X::Syntax::ConditionalOperator::PrecedenceTooLoose'] = 1; RkCoreN['X::Syntax::ConditionalOperator::SecondPartGobbled'] = 1; RkCoreN['X::Syntax::ConditionalOperator::SecondPartInvalid'] = 1; RkCoreN['X::Syntax::Confused'] = 1; RkCoreN['X::Syntax::DuplicatedPrefix'] = 1;
RkCoreN['X::Syntax::Extension'] = 1; RkCoreN['X::Syntax::Extension::Category'] = 1; RkCoreN['X::Syntax::Extension::Null'] = 1; RkCoreN['X::Syntax::Extension::SpecialForm'] = 1; RkCoreN['X::Syntax::Extension::TooComplex'] = 1; RkCoreN['X::Syntax::InfixInTermPosition'] = 1;
RkCoreN['X::Syntax::KeywordAsFunction'] = 1; RkCoreN['X::Syntax::Malformed'] = 1; RkCoreN['X::Syntax::Malformed::Elsif'] = 1; RkCoreN['X::Syntax::Missing'] = 1; RkCoreN['X::Syntax::Name'] = 1; RkCoreN['X::Syntax::Name::Null'] = 1;
RkCoreN['X::Syntax::NegatedPair'] = 1; RkCoreN['X::Syntax::NoSelf'] = 1; RkCoreN['X::Syntax::NonAssociative'] = 1; RkCoreN['X::Syntax::NonListAssociative'] = 1; RkCoreN['X::Syntax::Number'] = 1; RkCoreN['X::Syntax::Number::IllegalDecimal'] = 1;
RkCoreN['X::Syntax::Number::LiteralType'] = 1; RkCoreN['X::Syntax::Number::RadixOutOfRange'] = 1; RkCoreN['X::Syntax::P5'] = 1; RkCoreN['X::Syntax::ParentAsHash'] = 1; RkCoreN['X::Syntax::Perl5Var'] = 1; RkCoreN['X::Syntax::Pod'] = 1;
RkCoreN['X::Syntax::Pod::BeginWithoutEnd'] = 1; RkCoreN['X::Syntax::Pod::BeginWithoutIdentifier'] = 1; RkCoreN['X::Syntax::Pod::DeclaratorLeading'] = 1; RkCoreN['X::Syntax::Pod::DeclaratorTrailing'] = 1; RkCoreN['X::Syntax::Regex'] = 1; RkCoreN['X::Syntax::Regex::Adverb'] = 1;
RkCoreN['X::Syntax::Regex::Alias'] = 1; RkCoreN['X::Syntax::Regex::Alias::LongName'] = 1; RkCoreN['X::Syntax::Regex::MalformedRange'] = 1; RkCoreN['X::Syntax::Regex::NonQuantifiable'] = 1; RkCoreN['X::Syntax::Regex::NullRegex'] = 1; RkCoreN['X::Syntax::Regex::QuantifierValue'] = 1;
RkCoreN['X::Syntax::Regex::SolitaryBacktrackControl'] = 1; RkCoreN['X::Syntax::Regex::SolitaryQuantifier'] = 1; RkCoreN['X::Syntax::Regex::SpacesInBareRange'] = 1; RkCoreN['X::Syntax::Regex::UnrecognizedMetachar'] = 1; RkCoreN['X::Syntax::Regex::UnrecognizedModifier'] = 1; RkCoreN['X::Syntax::Regex::Unspace'] = 1;
RkCoreN['X::Syntax::Regex::Unterminated'] = 1; RkCoreN['X::Syntax::Reserved'] = 1; RkCoreN['X::Syntax::Self'] = 1; RkCoreN['X::Syntax::Self::WithoutObject'] = 1; RkCoreN['X::Syntax::Signature'] = 1; RkCoreN['X::Syntax::Signature::InvocantMarker'] = 1;
RkCoreN['X::Syntax::Signature::InvocantNotAllowed'] = 1; RkCoreN['X::Syntax::Type'] = 1; RkCoreN['X::Syntax::Type::Adverb'] = 1; RkCoreN['X::Syntax::UnlessElse'] = 1; RkCoreN['X::Syntax::Variable'] = 1; RkCoreN['X::Syntax::Variable::BadType'] = 1;
RkCoreN['X::Syntax::Variable::ConflictingTypes'] = 1; RkCoreN['X::Syntax::Variable::IndirectDeclaration'] = 1; RkCoreN['X::Syntax::Variable::Initializer'] = 1; RkCoreN['X::Syntax::Variable::Match'] = 1; RkCoreN['X::Syntax::Variable::MissingInitializer'] = 1; RkCoreN['X::Syntax::Variable::Numeric'] = 1;
RkCoreN['X::Syntax::Variable::SignatureAssignment'] = 1; RkCoreN['X::Syntax::Variable::SignatureWithoutInitializer'] = 1; RkCoreN['X::Syntax::Variable::Twigil'] = 1; RkCoreN['X::Syntax::VirtualCall'] = 1; RkCoreN['X::Syntax::WithoutElse'] = 1; RkCoreN['X::Temporal'] = 1;
RkCoreN['X::Temporal::InvalidFormat'] = 1; RkCoreN['X::TooLateForREPR'] = 1; RkCoreN['X::TooManyDimensions'] = 1; RkCoreN['X::Trait'] = 1; RkCoreN['X::Trait::Invalid'] = 1; RkCoreN['X::Trait::NotOnNative'] = 1;
RkCoreN['X::Trait::Scope'] = 1; RkCoreN['X::Trait::Unknown'] = 1; RkCoreN['X::TypeCheck'] = 1; RkCoreN['X::TypeCheck::Argument'] = 1; RkCoreN['X::TypeCheck::Assignment'] = 1; RkCoreN['X::TypeCheck::Attribute'] = 1;
RkCoreN['X::TypeCheck::Attribute::Default'] = 1; RkCoreN['X::TypeCheck::Binding'] = 1; RkCoreN['X::TypeCheck::Binding::Parameter'] = 1; RkCoreN['X::TypeCheck::Return'] = 1; RkCoreN['X::TypeCheck::Splice'] = 1; RkCoreN['X::Undeclared'] = 1;
RkCoreN['X::Undeclared::Symbols'] = 1; RkCoreN['X::UnitScope'] = 1; RkCoreN['X::UnitScope::Invalid'] = 1; RkCoreN['X::UnitScope::TooLate'] = 1; RkCoreN['X::Value'] = 1; RkCoreN['X::Value::Dynamic'] = 1;
RkCoreN['X::WheneverOutOfScope'] = 1; RkCoreN['X::Worry'] = 1; RkCoreN['X::Worry::P5'] = 1; RkCoreN['X::Worry::P5::BackReference'] = 1; RkCoreN['X::Worry::P5::LeadingZero'] = 1; RkCoreN['X::Worry::P5::Reference'] = 1;
RkCoreN['X::Worry::Precedence'] = 1; RkCoreN['X::Worry::Precedence::Range'] = 1; RkCoreN['array'] = 1; RkCoreN['array::intarray'] = 1; RkCoreN['array::numarray'] = 1; RkCoreN['array::shaped1intarray'] = 1;
RkCoreN['array::shaped1numarray'] = 1; RkCoreN['array::shaped1strarray'] = 1; RkCoreN['array::shaped1uintarray'] = 1; RkCoreN['array::shaped2intarray'] = 1; RkCoreN['array::shaped2numarray'] = 1; RkCoreN['array::shaped2strarray'] = 1;
RkCoreN['array::shaped2uintarray'] = 1; RkCoreN['array::shaped3intarray'] = 1; RkCoreN['array::shaped3numarray'] = 1; RkCoreN['array::shaped3strarray'] = 1; RkCoreN['array::shaped3uintarray'] = 1; RkCoreN['array::shapedarray'] = 1;
RkCoreN['array::shapedintarray'] = 1; RkCoreN['array::shapednumarray'] = 1; RkCoreN['array::shapedstrarray'] = 1; RkCoreN['array::shapeduintarray'] = 1; RkCoreN['array::strarray'] = 1; RkCoreN['array::typedim2role'] = 1;
RkCoreN['array::uintarray'] = 1; RkCoreN['atomicint'] = 1; RkCoreN['blob16'] = 1; RkCoreN['blob32'] = 1; RkCoreN['blob64'] = 1; RkCoreN['blob8'] = 1;
RkCoreN['buf16'] = 1; RkCoreN['buf32'] = 1; RkCoreN['buf64'] = 1; RkCoreN['buf8'] = 1; RkCoreN['byte'] = 1; RkCoreN['e'] = 1;
RkCoreN['i'] = 1; RkCoreN['int'] = 1; RkCoreN['int16'] = 1; RkCoreN['int32'] = 1; RkCoreN['int64'] = 1; RkCoreN['int8'] = 1;
RkCoreN['num'] = 1; RkCoreN['num32'] = 1; RkCoreN['num64'] = 1; RkCoreN['pi'] = 1; RkCoreN['str'] = 1; RkCoreN['tau'] = 1;
RkCoreN['uint'] = 1; RkCoreN['uint16'] = 1; RkCoreN['uint32'] = 1; RkCoreN['uint64'] = 1; RkCoreN['uint8'] = 1; RkCoreN['utf16'] = 1;
RkCoreN['utf32'] = 1; RkCoreN['utf8'] = 1;
/* ==================================================================================================================== */
/* the parser's state and predicates: the subject Src read at the cursor (ch, at_lit, wordch_at), the name tables    */
/* (add_name_n, scope_enter / scope_leave, is_name_n, is_type_n), the precedence limit of the running r_EXPR, the     */
/* statement-end marks of r_ENDSTMT                                                                                 */
/* ==================================================================================================================== */
RkNames = TABLE(211); RkNameVal = TABLE(211); RkNameStk = ARRAY('1:4096'); RkNameDep = ARRAY('1:4096'); RkNameN = 0; RkDepth = 0;
RkLoopQ = ARRAY('1:4096'); RkLoopQn = 0; RkLoopQi = 0; RkLimStk = ARRAY('1:256'); RkLimN = 0; RkEndT = TABLE(211); rk_finished = 0; rk_ilim = 18; rk_ntc = 0;
RkPseudo = TABLE(31);
RkPseudo['GLOBAL'] = 1; RkPseudo['OUR'] = 1; RkPseudo['MY'] = 1; RkPseudo['CORE'] = 1; RkPseudo['SETTING'] = 1; RkPseudo['OUTER'] = 1;
RkPseudo['CALLER'] = 1; RkPseudo['DYNAMIC'] = 1; RkPseudo['PROCESS'] = 1; RkPseudo['COMPILING'] = 1; RkPseudo['UNIT'] = 1;
RkPseudo['LEXICAL'] = 1; RkPseudo['CLIENT'] = 1; RkPseudo['OUTERS'] = 1; RkPseudo['CALLERS'] = 1; RkPseudo['EXPORT'] = 1;
RkNotTypeVal = TABLE(31);
RkNotTypeVal['True'] = 1; RkNotTypeVal['False'] = 1; RkNotTypeVal['Less'] = 1; RkNotTypeVal['Same'] = 1; RkNotTypeVal['More'] = 1;
RkNotTypeVal['Inf'] = 1; RkNotTypeVal['NaN'] = 1; RkNotTypeVal['pi'] = 1; RkNotTypeVal['e'] = 1; RkNotTypeVal['i'] = 1;
RkNotTypeVal['tau'] = 1; RkNotTypeVal['Empty'] = 1; RkNotTypeVal['Nil'] = 1;
RkStmtModKw = TABLE(31);
RkStmtModKw['if'] = 1; RkStmtModKw['unless'] = 1; RkStmtModKw['while'] = 1; RkStmtModKw['until'] = 1; RkStmtModKw['for'] = 1;
RkStmtModKw['given'] = 1; RkStmtModKw['when'] = 1; RkStmtModKw['with'] = 1; RkStmtModKw['without'] = 1;
/* ==================================================================================================================== */
function RkCh(q) {
    RkCh = SUBSTR(Src, q + 1, 1);
    return;
}
/* ==================================================================================================================== */
function RkIsWordCh(c) {
    if (c ? (POS(0) ANY(&UCASE &LCASE '_0123456789' X1xxxxxxx) RPOS(0))) { return; }
    freturn;
}
/* ==================================================================================================================== */
function RkAtLit(q, s) {
    if (IDENT(SUBSTR(Src, q + 1, SIZE(s)), s)) { RkAtLit = ; return; }
    freturn;
}
/* ==================================================================================================================== */
function RkChAt(q, c)            { if (IDENT(RkCh(q), c)) { RkChAt = ; return; } freturn; }
function RkNotAt(q, set, c)      { c = RkCh(q); if (IDENT(c)) { RkNotAt = ; return; } if (set ? c) { freturn; } RkNotAt = ; return; }
function RkWordAt(q)             { if (RkIsWordCh(RkCh(q))) { RkWordAt = ; return; } freturn; }
function RkNotWordAt(q)          { if (RkIsWordCh(RkCh(q))) { freturn; } RkNotWordAt = ; return; }
function RkDigitAt(q)            { if (RkCh(q) ? (POS(0) ANY('0123456789'))) { RkDigitAt = ; return; } freturn; }
function RkAlphaAt(q)            { if (RkCh(q) ? (POS(0) ANY(&UCASE &LCASE '_' X1xxxxxxx))) { RkAlphaAt = ; return; } freturn; }
function RkAlphaOrColonAt(q)     { if (RkCh(q) ? (POS(0) ANY(&UCASE &LCASE '_:' X1xxxxxxx))) { RkAlphaOrColonAt = ; return; } freturn; }
function RkIsSpaceAt(q)          { if (RkCh(q) ? (POS(0) ANY(' ' CHAR(9) CHAR(10) CHAR(13) CHAR(11) CHAR(12)))) { RkIsSpaceAt = ; return; } freturn; }
function RkIsSpaceOrHash(q)      { if (RkCh(q) ? (POS(0) ANY(' #' CHAR(9) CHAR(10) CHAR(13) CHAR(11) CHAR(12)))) { RkIsSpaceOrHash = ; return; } freturn; }
function RkSigilAt(q)            { if (RkCh(q) ? (POS(0) ANY('$@%&'))) { RkSigilAt = ; return; } freturn; }
function RkAtBol(q, c)           { if (EQ(q, 0)) { RkAtBol = ; return; } c = SUBSTR(Src, q, 1); if (c ? (POS(0) ANY(CHAR(10) CHAR(13)))) { RkAtBol = ; return; } freturn; }
function RkAtEol(q, c)           { c = RkCh(q); if (IDENT(c)) { RkAtEol = ; return; } if (c ? (POS(0) ANY(CHAR(10) CHAR(13)))) { RkAtEol = ; return; } freturn; }
function RkFinish()              { rk_finished = 1; RkFinish = .dummy; nreturn; }
function RkIsFinished()          { if (EQ(rk_finished, 1)) { RkIsFinished = ; return; } freturn; }
function RkLoopEnd(q, p, n) {
    p = q;
    while (GT(p, 0) (SUBSTR(Src, p, 1) ? ANY(' ' CHAR(9) CHAR(10) CHAR(13) CHAR(11) CHAR(12)))) { p = p - 1; }
    p = (GT(p, 0) p - 1, 0);
    n = 1;
    if (GT(p, 0)) { rk_lp = SUBSTR(Src, 1, p); while (rk_lp ? (POS(0) BREAK(CHAR(10)) CHAR(10)) =) { n = n + 1; } }
    RkLoopQn = RkLoopQn + 1; RkLoopQ[RkLoopQn] = n;
    RkLoopEnd = ;
    return;
}
function RkLoopNext()            { RkLoopQi = RkLoopQi + 1; RkLoopNext = RkLoopQ[RkLoopQi]; return; }
function RkMarkEnd(q)            { RkEndT[q] = 1; RkMarkEnd = ; return; }
function RkNotMarkedEnd(q)       { if (DIFFER(RkEndT[q])) { freturn; } RkNotMarkedEnd = ; return; }
function RkGoalPush(g)           { RkGoalPush = ; return; }
function RkGoalPop()             { RkGoalPop = ; return; }
function RkScopePush(k)          { RkScopePush = ; return; }
function RkScopePop()            { RkScopePop = ; return; }
function RkMultiPush(k)          { RkMultiPush = ; return; }
function RkMultiPop()            { RkMultiPop = ; return; }
function RkDeclPush(k)           { RkDeclPush = ; return; }
function RkDeclPop()             { RkDeclPop = ; return; }
function RkDeclName(v)           { RkDeclName = ; return; }
function RkParamName(v)          { RkParamName = ; return; }
function RkRegisterRoutine(n)    { RkRegisterRoutine = ; return; }
function RkNtc0()                { rk_ntc = 0; RkNtc0 = ; return; }
function RkNtcInc()              { rk_ntc = rk_ntc + 1; RkNtcInc = ; return; }
function RkHasTypes()            { if (GT(rk_ntc, 0)) { RkHasTypes = ; return; } freturn; }
function RkInitLim(v)            { rk_ilim = (IDENT(SUBSTR(v, 1, 1), '$') 33, 18); RkInitLim = ; return; }
/* ==================================================================================================================== */
/* end_keyword_ok: not before a word character, '(' '\' ''' '-', nor before (horizontal white) '=>'                    */
/* ==================================================================================================================== */
function RkEndKeywordOk(q, c) {
    c = RkCh(q);
    if (RkIsWordCh(c)) { freturn; }
    if (c ? (POS(0) ANY("(\'-"))) { freturn; }
    while (RkCh(q) ? (POS(0) ANY(' ' CHAR(9)))) { q = q + 1; }
    if (RkAtLit(q, '=>')) { freturn; }
    RkEndKeywordOk = ;
    return;
}
/* ==================================================================================================================== */
function RkScopeKwOk(q) {
    if (RkIsWordCh(RkCh(q))) { freturn; }
    if (RkEndKeywordOk(q)) { RkScopeKwOk = ; return; }
    if (RkCh(q) ? (POS(0) ANY('(\'))) { RkScopeKwOk = ; return; }
    freturn;
}
/* ==================================================================================================================== */
function RkColonpairAhead(q, c) {
    if (~IDENT(RkCh(q), ':')) { freturn; }
    c = RkCh(q + 1);
    if (c ? (POS(0) ANY(&UCASE &LCASE '_<[' X1xxxxxxx))) { RkColonpairAhead = ; return; }
    freturn;
}
/* ==================================================================================================================== */
/* the name tables: add_name_n, add_value_name, scope_enter, scope_leave, is_name_n, is_type_n                      */
/* ==================================================================================================================== */
function RkAddNameV(n, val) {
    if (IDENT(n)) { return; }
    RkNameN = RkNameN + 1;
    RkNameStk[RkNameN] = n;
    RkNameDep[RkNameN] = RkDepth;
    RkNames[n] = RkNames[n] + 1;
    RkNameVal[n] = val;
    return;
}
function RkAddName(n)            { RkAddNameV(n, 0); RkAddName = ; return; }
function RkAddValueName(n)       { RkAddNameV(n, 1); RkAddValueName = ; return; }
/* ==================================================================================================================== */
function RkAddPkgName(n, i) {
    RkAddNameV(n, 0);
    i = 0;
    while (n ? (POS(i) BREAK(':') . rk_pre '::' @i)) {
        RkAddNameV(rk_pre, 0);
        RkAddNameV(SUBSTR(n, i + 1), 0);
    }
    RkAddPkgName = ;
    return;
}
/* ==================================================================================================================== */
function RkEnumNames(ws, w) {
    while (ws ? (POS(0) FENCE(SPAN(' ' CHAR(9) CHAR(10)) | epsilon) BREAK(' >' CHAR(9) CHAR(10)) . w FENCE(SPAN(' ' CHAR(9) CHAR(10)) | epsilon)) =) {
        if (IDENT(w)) { break; }
        RkAddNameV(w, 1);
    }
    if (DIFFER(ws)) { RkAddNameV(ws, 1); }
    RkEnumNames = ;
    return;
}
/* ==================================================================================================================== */
function RkScopeEnter()          { RkDepth = RkDepth + 1; RkScopeEnter = ; return; }
function RkScopeLeave(n) {
    RkDepth = RkDepth - 1;
    while (GT(RkNameN, 0) GT(RkNameDep[RkNameN], RkDepth)) {
        n = RkNameStk[RkNameN];
        RkNames[n] = RkNames[n] - 1;
        RkNameN = RkNameN - 1;
    }
    RkScopeLeave = ;
    return;
}
/* ==================================================================================================================== */
function RkNamePart(s) {
    RkNamePart = s;
    s ? (POS(0) (ARB ANY(&UCASE &LCASE '_0123456789' X1xxxxxxx)) . RkNamePart ':' NOTANY(':'));
    return;
}
/* ==================================================================================================================== */
function RkIsName(s, i0, head) {
    if (IDENT(s)) { return; }
    if (s ? (POS(0) BREAK(':') . head '::')) {
        if (DIFFER(RkPseudo[head])) { return; }
    } else {
        if (DIFFER(RkPseudo[s])) { return; }
    }
    if (s ? (RTAB(2) '::')) { s = SUBSTR(s, 1, SIZE(s) - 2); }
    if (IDENT(s)) { return; }
    if (GT(RkNames[s], 0)) { return; }
    if (DIFFER(RkCoreN[s])) { return; }
    if (s ? (POS(0) BREAK(':') . head '::')) {
        if (DIFFER(head) GT(RkNames[head], 0)) { return; }
    }
    freturn;
}
function RkIsNameP(s)            { if (s ? (POS(0) '::')) { RkIsNameP = ; return; } if (RkIsName(RkNamePart(s))) { RkIsNameP = ; return; } freturn; }
function RkNotName(s)            { if (RkIsName(s)) { freturn; } RkNotName = ; return; }
function RkIsType(s) {
    if (DIFFER(RkNotTypeVal[s])) { freturn; }
    if (s ? (RTAB(2) '::')) { freturn; }
    if (GT(RkNames[s], 0) EQ(RkNameVal[s], 1)) { freturn; }
    if (RkIsName(s)) { return; }
    freturn;
}
function RkNotType(s)            { if (RkIsType(s)) { freturn; } RkNotType = ; return; }
/* ==================================================================================================================== */
/* stoppers: is_terminator_at, stdstopper, the statement-list and semilist ends, r_eat_terminator's accepted ends     */
/* ==================================================================================================================== */
function RkIsStmtModKw(q, w) {
    if (~(Src ? (POS(q) (SPAN(&LCASE)) . w))) { freturn; }
    if (IDENT(RkStmtModKw[w])) { freturn; }
    if (RkEndKeywordOk(q + SIZE(w))) { return; }
    freturn;
}
function RkIsTerminator(q, c) {
    c = RkCh(q);
    if (c ? (POS(0) ANY(';)]}'))) { return; }
    if (RkAtLit(q, '-->')) { return; }
    if (RkIsStmtModKw(q)) { return; }
    freturn;
}
function RkStdStopQ(q) {
    if (DIFFER(RkEndT[q])) { return; }
    if (GE(q, SIZE(Src))) { return; }
    if (RkIsTerminator(q)) { return; }
    freturn;
}
function RkStdStop(q)            { if (RkStdStopQ(q)) { RkStdStop = ; return; } freturn; }
function RkNotStdStop(q)         { if (RkStdStopQ(q)) { freturn; } RkNotStdStop = ; return; }
function RkNotStop(q)            { if (RkStdStopQ(q)) { freturn; } RkNotStop = ; return; }
function RkSemiStop(q)           { if (GE(q, SIZE(Src))) { RkSemiStop = ; return; } if (RkCh(q) ? (POS(0) ANY(')]}'))) { RkSemiStop = ; return; } freturn; }
function RkListStop(q)           { if (EQ(rk_finished, 1)) { RkListStop = ; return; } if (RkSemiStop(q)) { RkListStop = ; return; } freturn; }
function RkStmtStart(q)          { if (RkSemiStop(q)) { freturn; } RkStmtStart = ; return; }
function RkTermOk(q) {
    if (DIFFER(RkEndT[q])) { RkTermOk = ; return; }
    if (RkSemiStop(q)) { RkTermOk = ; return; }
    if (RkIsTerminator(q)) { if (~RkIsStmtModKw(q)) { RkTermOk = ; return; } }
    freturn;
}
function RkSigEnd(q, c) {
    c = RkCh(q);
    if (RkAtLit(q, '-->')) { RkSigEnd = ; return; }
    if (c ? (POS(0) ANY(')]{'))) { RkSigEnd = ; return; }
    if (IDENT(c, ':') RkIsSpaceAt(q + 1)) { RkSigEnd = ; return; }
    if (RkAtLit(q, ';;')) { RkSigEnd = ; return; }
    freturn;
}
/* ==================================================================================================================== */
/* operators: the precedence limit of the running r_EXPR, the level tables, '=' and op= assignment, prefix checks     */
/* ==================================================================================================================== */
function RkLimPush(n)            { RkLimN = RkLimN + 1; RkLimStk[RkLimN] = n; RkLimPush = ; return; }
function RkLimPop()              { RkLimN = RkLimN - 1; RkLimPop = ; return; }
function RkLimOk(n)              { if (GT(n, RkLimStk[RkLimN])) { RkLimOk = ; return; } freturn; }
function RkOpPrec(op) {
    if (IDENT(op, '=')) { RkOpPrec = 34; return; }
    if (IDENT(op, '??')) { RkOpPrec = 38; return; }
    if (DIFFER(RkPrec[op])) { RkOpPrec = RkPrec[op]; return; }
    if (op ? (POS(0) (ARB . rk_opb) '=' RPOS(0))) {
        if (DIFFER(RkPrec[rk_opb])) { RkOpPrec = 34; return; }
    }
    freturn;
}
function RkOpPrecOk(p)           { if (~(p = RkOpPrec(rk_opt))) { freturn; } if (GT(p, RkLimStk[RkLimN])) { RkOpPrecOk = ; return; } freturn; }
function RkLvIs(lv)              { if (DIFFER(RkLv[lv][rk_opt])) { RkLvIs = ; return; } freturn; }
function RkIsSmart()             { if (IDENT(rk_opt, '~~')) { RkIsSmart = ; return; } freturn; }
function RkIsListInfix()         { if (rk_opt ? (POS(0) ANY('XZ') RPOS(0))) { RkIsListInfix = ; return; } freturn; }
function RkIsComma()             { if (IDENT(rk_opt, ',')) { RkIsComma = ; return; } freturn; }
function RkLeftover()            { if (IDENT(rk_opt, ',')) { freturn; } if (rk_opt ? (POS(0) ANY('XZ') RPOS(0))) { freturn; } RkLeftover = ; return; }
function RkAssignOk(q, c) {
    c = RkCh(q);
    if (c ? (POS(0) ANY('=>~'))) { freturn; }
    if (RkAtLit(q, ':=')) { freturn; }
    RkAssignOk = ;
    return;
}
function RkCompoundOk(op, q, p) {
    if (IDENT(RkCh(q), '=')) { freturn; }
    if (~(p = RkPrec[op])) { freturn; }
    if (EQ(p, 34)) { freturn; }
    if (EQ(p, 50) (op ? ('=' RPOS(0)))) { freturn; }
    RkCompoundOk = ;
    return;
}
function RkMinusOk(q)            { if (IDENT(RkCh(q), '>')) { if (~RkAtLit(q, '>>')) { freturn; } } RkMinusOk = ; return; }
function RkNeOk(q, c)            { c = RkCh(q); if (IDENT(c, ']')) { RkNeOk = ; return; } if (RkIsSpaceAt(q)) { RkNeOk = ; return; } freturn; }
function RkPrefixStart(q) {
    if ((RkAtLit(q, '->'), RkAtLit(q, '<->'), RkAtLit(q, '???'), RkAtLit(q, '!!!'), RkAtLit(q, '...'))) { freturn; }
    RkPrefixStart = ;
    return;
}
function RkPrefixOk(op, q) {
    if (IDENT(op, '!') IDENT(RkCh(q), '!')) { freturn; }
    if (op ? (POS(0) ('let' | 'temp' | 'so' | 'not') RPOS(0))) {
        if (~RkEndKeywordOk(q)) { freturn; }
        if (op ? (POS(0) ('let' | 'temp') RPOS(0))) { if (~RkIsSpaceOrHash(q)) { freturn; } }
    }
    RkPrefixOk = ;
    return;
}
/* ==================================================================================================================== */
/* var_cls_of: the class of a variable by its sigil and twigil; the assignment a plain variable takes in x_expr      */
/* ==================================================================================================================== */
function RkVarCls(t, c1) {
    RkVarCls = ;
    if (t ? (POS(0) '$')) {
        if (EQ(SIZE(t), 1)) { return; }
        c1 = SUBSTR(t, 2, 1);
        if (t ? (POS(0) ('$*STDIN' | '$*STDOUT' | '$*STDERR') RPOS(0))) { RkVarCls = 'F'; return; }
        if (t ? (POS(0) '$' ANY('0123456789'))) { RkVarCls = 'P'; return; }
        if (IDENT(t, '$?LINE')) { RkVarCls = 'L'; return; }
        if (t ? (POS(0) '$' ANY('.!') ANY(&UCASE &LCASE '_'))) { RkVarCls = 'T'; return; }
        RkVarCls = 'S';
        return;
    }
    if (t ? (POS(0) '@')) { RkVarCls = 'A'; if (t ? (POS(0) '@' ANY('.!') LEN(1))) { RkVarCls = 'U'; } return; }
    if (t ? (POS(0) '%')) { RkVarCls = 'H'; if (t ? (POS(0) '%' ANY('.!') LEN(1))) { RkVarCls = 'W'; } return; }
    return;
}
function RkCompoundBase(op, i, lv, b) {
    if (~(op ? (POS(0) (ARB . b) '=' RPOS(0)))) { freturn; }
    if (LT(SIZE(op), 2)) { freturn; }
    i = 0;
    while (i = LT(i, 10) i + 1) {
        lv = RkCompLvs[i];
        if (DIFFER(RkLv[lv][b])) { RkCompoundBase = lv; rk_cbk = RkLv[lv][b]; return; }
    }
    freturn;
}
function RkAsgFor(v, cls, eq) {
    cls = RkVarCls(v);
    eq = IDENT(rk_opt, '=') 1;
    if (DIFFER(eq)) { if (cls ? ANY('SA')) { RkAsgFor = ; return; } freturn; }
    if (~IDENT(cls, 'S')) { freturn; }
    if (DIFFER(RkLv['COMPOUND'][rk_opt])) { RkAsgFor = ; return; }
    if (EQ(RkOpPrec(rk_opt), 34) RkCompoundBase(rk_opt)) { RkAsgFor = ; return; }
    freturn;
}
function RkNoPostfixAt(q, c) {
    c = RkCh(q);
    if (c ? (POS(0) ANY('[{(<.'))) { freturn; }
    if ((RkAtLit(q, '++'), RkAtLit(q, '--'), RkAtLit(q, '>>'))) { freturn; }
    if (IDENT(c, '!') (RkCh(q + 1) ? ANY(&UCASE &LCASE '_'))) { freturn; }
    RkNoPostfixAt = ;
    return;
}
/* ==================================================================================================================== */
/* the raw tree's deferred actions (performed in match order at the end of the successful match, as Shift/Reduce)    */
/* ==================================================================================================================== */
RkCompLvs = ARRAY('1:10');
RkCompLvs[1] = 'MUL'; RkCompLvs[2] = 'ADDSUB'; RkCompLvs[3] = 'REPL'; RkCompLvs[4] = 'CAT'; RkCompLvs[5] = 'DOR';
RkCompLvs[6] = 'JCT'; RkCompLvs[7] = 'DIVIS'; RkCompLvs[8] = 'AND'; RkCompLvs[9] = 'OR'; RkCompLvs[10] = 'POW';
/* ==================================================================================================================== */
function RkNode(t, v, n, c)      { RkNode = tree(t, v, n, c); return; }
function RkLeaf(t, v)            { RkLeaf = tree(t, v, 0); return; }
function RkRed(t, v, n, c, i) {
    c = ARRAY('1:' (GT(n, 0) n, 1));
    i = n + 1;
    while (i = GT(i, 1) i - 1) { c[i] = Pop(); }
    Push(tree(t, v, n, c));
    RkRed = .dummy;
    nreturn;
}
function RkSwap(a, b)            { a = Pop(); b = Pop(); Push(a); Push(b); RkSwap = .dummy; nreturn; }
function RkDropN(n)              { while (n = GT(n, 0) n - 1) { Pop(); } RkDropN = .dummy; nreturn; }
function RkBin(lv, r, op, l)     { RkRed('R_BIN', lv, 3); RkBin = .dummy; nreturn; }
/* R_CALL(name) [R_FORM, args]: the stack holds R_CALLNM, the arguments (R_LIST or R_NOARGS), R_FORM                  */
function RkCall(f, a, nm)        { f = Pop(); a = Pop(); nm = Pop(); Push(RkNode('R_CALL', v(nm), 2, RkArr2(f, a))); RkCall = .dummy; nreturn; }
function RkArr1(a, c)            { c = ARRAY('1:1'); c[1] = a; RkArr1 = c; return; }
function RkArr2(a, b, c)         { c = ARRAY('1:2'); c[1] = a; c[2] = b; RkArr2 = c; return; }
function RkArr3(a, b, d, c)      { c = ARRAY('1:3'); c[1] = a; c[2] = b; c[3] = d; RkArr3 = c; return; }
/* r_semilist: the first statement's expression list and the statement count (p->last_list, p->last_nstmts)          */
function RkSemiRed(n, i, s, first, lst) {
    n = nTop();
    i = n + 1;
    while (i = GT(i, 1) i - 1) { s = Pop(); if (EQ(i, 1)) { first = s; } }
    lst = RkStmtList(first);
    Push(RkNode('R_SEMI', n, (DIFFER(lst) 1, 0), (DIFFER(lst) RkArr1(lst), '')));
    RkSemiRed = .dummy;
    nreturn;
}
function RkStmtList(s, x) {
    RkStmtList = ;
    if (IDENT(s)) { return; }
    if (~IDENT(t(s), 'R_STMT')) { return; }
    x = c(s)[1];
    if (IDENT(t(x), 'R_EXPRSTMT')) { RkStmtList = c(x)[1]; }
    return;
}
function RkParen(s)              { s = Pop(); Push(RkNode('R_PAREN', v(s), n(s), c(s))); RkParen = .dummy; nreturn; }
function RkBracket(s)            { s = Pop(); Push(RkNode('R_BRACKET', v(s), n(s), c(s))); RkBracket = .dummy; nreturn; }
function RkPf(k, adj, s)         { s = Pop(); Push(RkNode(k, adj, 1, RkArr1(s))); RkPf = .dummy; nreturn; }
function RkStmts(bk)             { RkRed('R_STMTS', bk, nTop()); RkStmts = .dummy; nreturn; }
function RkBlockTerm(b, s)       { b = Pop(); s = Pop(); Push(RkNode('R_BLOCK', '', 2, RkArr2(s, b))); RkBlockTerm = .dummy; nreturn; }
/* R_PF_M(name) [R_FORM, args, R_MMOD]                                                                               */
function RkMethod(f, a, nm)      { f = Pop(); a = Pop(); nm = Pop(); Push(RkNode('R_PF_M', v(nm), 3, RkArr3(f, a, RkLeaf('R_MMOD', '')))); RkMethod = .dummy; nreturn; }
function RkMod(m, md)            { m = Pop(); md = Pop(); c(m)[3] = md; Push(m); RkMod = .dummy; nreturn; }
function RkHyper(m)              { m = Pop(); Push(RkNode('R_PF_HYPER', '', 1, RkArr1(m))); RkHyper = .dummy; nreturn; }
function RkYada(f, a)            { f = Pop(); a = Pop(); Push(RkLeaf('R_YADA', '')); RkYada = .dummy; nreturn; }
function RkReduceTerm(f, a, o)   { f = Pop(); a = Pop(); o = Pop(); Push(RkNode('R_REDUCE', v(o), 2, RkArr2(f, a))); RkReduceTerm = .dummy; nreturn; }
function RkbQuote(tx)            { RkRed('R_QUOTE', tx, nTop()); RkbQuote = .dummy; nreturn; }
/* R_CP(key) [ck, vkind, value]: :name, :!name, :name(...), :name<...>, :name[...], :$var                             */
function RkCp(ck, vk, val, key) {
    val = ;
    if (DIFFER(vk)) { if (~IDENT(vk, 0)) { val = Pop(); } }
    key = Pop();
    Push(RkNode('R_CP', v(key), 3, RkArr3(RkLeaf('R_A', ck), RkLeaf('R_A', vk), val)));
    RkCp = .dummy;
    nreturn;
}
function RkTraitCount()          { IncCounter(); RkTraitCount = .dummy; nreturn; }
function RkTraitsRed()           { RkRed('R_TRAITS', '', nTop()); RkTraitsRed = .dummy; nreturn; }
function RkFirstType(n, i, s, first) {
    n = nTop();
    first = '';
    i = n + 1;
    while (i = GT(i, 1) i - 1) { s = Pop(); if (EQ(i, 1)) { first = v(s); } }
    Push(RkLeaf('R_TYPE', first));
    RkFirstType = .dummy;
    nreturn;
}
function RkListOf1(d)            { d = Pop(); Push(RkNode('R_LIST', '', 1, RkArr1(RkNode('R_EL', '', 1, RkArr1(d))))); RkListOf1 = .dummy; nreturn; }
function RkCrossRed(n)           { n = nTop(); if (GT(n, 1)) { RkRed('R_CROSS', '', n); } RkCrossRed = .dummy; nreturn; }
function RkSemiSeen()            { RkSemiSeen = ; return; }
/* ==================================================================================================================== */
/* quotes: r_quote_raw / simple_quote / r_nibble_until / r_quote_escape / qq_backslash; the raw text and the parsed    */
/* {closures} are recorded, and rkb_quote (RkbQuote) takes them apart as b_dq / b_sq do                                 */
/* ==================================================================================================================== */
rk_sq_body   =   FENCE(BREAK("'\") FENCE('\' LEN(1) *rk_sq_body | epsilon));
rk_sq        =   "'" *rk_sq_body "'";
rk_qq_bs     =   '\' FENCE( ANY('xo') '[' BREAK(']') ']' | 'c' '[' BREAK(']') ']' | LEN(1) );
rk_closure   =   '{' . *PushCounter() *RkScopeEnter() FENCE(*rk_statementlist *rk_ws '}' *RkScopeLeave() | *RkScopeLeave() FAIL)
                 . *RkStmts('BLOCK') . *PopCounter() . *Reduce('R_CLOS', 1) . *IncCounter();
rk_dq_body   =   FENCE(BREAK('"\{') FENCE(*rk_qq_bs *rk_dq_body | *rk_closure *rk_dq_body | epsilon));
rk_dq        =   '"' *rk_dq_body '"';
rk_ang_body  =   FENCE(BREAK('<>\') FENCE('\' LEN(1) *rk_ang_body | '<' *rk_ang_body '>' *rk_ang_body | epsilon));
rk_rx_body   =   FENCE(BREAK("/\'" '"' '[') FENCE( '\' LEN(1) *rk_rx_body | "'" BREAK("'") "'" *rk_rx_body
                                               | '"' BREAK('"') '"' *rk_rx_body | '[' BREAK(']') ']' *rk_rx_body | epsilon));
rk_quote_raw =   FENCE( *rk_sq
                      | *rk_dq
                      | '/' *rk_rx_body '/'
                      | ('rx' | 'm') *rk_kw_tail '/' *rk_rx_body '/'
                      | ('qq' | 'q') *rk_kw_tail FENCE(SPAN(' ') | epsilon) ( '{' BREAK('}') '}' | '[' BREAK(']') ']' | '(' BREAK(')') ')' | '/' BREAK('/') '/' | '|' BREAK('|') '|' )
                      );
rk_quote     =   epsilon . *PushCounter() (*rk_quote_raw) . rk_qtx . *RkbQuote(rk_qtx) . *PopCounter();
/* ==================================================================================================================== */
/* variables: r_variable (sigil, twigil, desigilname) and the special forms $/ $_ $! $0 $<name>                        */
/* ==================================================================================================================== */
rk_twigil    =   ANY('.!^:*?=~') @rk_q *RkWordAt(rk_q);
rk_varname   =   FENCE( *rk_twigil *rk_longname
                      | *rk_longname
                      | SPAN('0123456789')
                      | '<' BREAK('>') '>'
                      | ANY('/_!')
                      );
rk_variable  =   ANY('$@%&') *rk_varname;
/* ==================================================================================================================== */
/* colon pairs: r_colonpair (:name, :!name, :name(value), :name<words>, :$var, :5name)                                 */
/* ==================================================================================================================== */
rk_colonpair_raw = ':' FENCE( '!' *rk_identifier
                            | SPAN('0123456789') *rk_identifier
                            | *rk_identifier FENCE('<>' | '(' BREAK(')') ')' | '<' *rk_ang_body '>' | '[' BREAK(']') ']' | epsilon)
                            | ANY('$@%&') FENCE(*rk_twigil | epsilon) *rk_longname
                            );
rk_colonpair =   ':' ( '!' (*rk_identifier) . thx . *Shift('R_CPKEY', thx) . *RkCp('!', 0)
                     | SPAN('0123456789') (*rk_identifier) . thx . *Shift('R_CPKEY', thx) . *RkCp('d', 0)
                     | (*rk_identifier) . thx . *Shift('R_CPKEY', thx)
                       ( '<>' epsilon . *Shift('R_WTXT', '') . *RkCp('v', 'W')
                       | '(' *rk_semilist *rk_ws ')' . *RkCp('v', 'P')
                       | '<' (*rk_ang_body) . thx '>' . *Shift('R_WTXT', thx) . *RkCp('v', 'W')
                       | '[' *rk_semilist *rk_ws ']' . *RkCp('v', 'B')
                       | epsilon . *RkCp('n', 0)
                       )
                     | (ANY('$@%&') FENCE(*rk_twigil | epsilon) *rk_longname) . thx . *Shift('R_CPKEY', thx) . *RkCp('$', 0)
                     );
/* ==================================================================================================================== */
/* operators: r_infixish / r_infix_plain (the longest operator, '=' and op= assignment), r_prefixish, r_postfix_op      */
/* ==================================================================================================================== */
rk_infix     =   FENCE( '??' @rk_q *RkNotAt(rk_q, '?')
                      | '=' @rk_q *RkAssignOk(rk_q)
                      | (*rk_iop_table) $ rk_opv FENCE('=' @rk_q *RkCompoundOk(rk_opv, rk_q) | epsilon)
                      );
rk_infix_at  =   *rk_ws @rk_q *RkNotStop(rk_q) (*rk_infix) $ rk_opt . rk_opc *RkOpPrecOk();
rk_opshift   =   epsilon . *Shift('R_OPTXT', rk_opc);
rk_prefixop  =   @rk_q *RkPrefixStart(rk_q) FENCE(*rk_pop_table) $ rk_pfx @rk_q *RkPrefixOk(rk_pfx, rk_q);
/* ==================================================================================================================== */
/* terms: r_term and its alternatives, in the C's order                                                              */
/* ==================================================================================================================== */
rk_semilist  =   epsilon . *PushCounter() *RkGoalPush(0) FENCE(*rk_ws *rk_semistmts *RkGoalPop() | *RkGoalPop() FAIL) . *RkSemiRed() . *PopCounter();
rk_semistmts =   FENCE(@rk_q *RkSemiStop(rk_q) | *rk_statement . *IncCounter() *rk_eat_terminator *rk_ws *rk_semistmts | epsilon);
rk_semiarg_r =   FENCE(*rk_ws ';' *rk_arglist_drop *rk_semiarg_r | epsilon);
rk_args_paren =  '(' *rk_arglist *rk_semiarg_r *rk_ws ')';
rk_args      =   FENCE( *rk_args_paren . *Shift('R_FORM', 1)
                      | *rk_unsp *rk_args_paren . *Shift('R_FORM', 1)
                      | @rk_q *RkIsSpaceAt(rk_q) LEN(1) *rk_arglist . *Shift('R_FORM', 2)
                      | epsilon . *Shift('R_NOARGS', '') . *Shift('R_FORM', 0)
                      );
rk_arglist   =   *rk_ws FENCE( @rk_q *RkStdStop(rk_q) . *Shift('R_NOARGS', '')
                             | *RkLimPush(18) FENCE(*rk_xlist *RkLimPop() | *RkLimPop() FAIL)
                             | epsilon . *Shift('R_NOARGS', '')
                             );
rk_arglist_drop = epsilon . *PushCounter() *rk_arglist . *IncCounter() . *RkDropN(nTop()) . *PopCounter();
rk_term_ident =  (*rk_identifier) $ rk_tid . thx *RkNotType(rk_tid) . *Shift('R_CALLNM', thx)
                 FENCE(@rk_q *RkChAt(rk_q, '(') | *rk_unsp @rk_q *RkChAt(rk_q, '(')) *rk_args_paren . *Shift('R_FORM', 1)
                 . *RkCall();
rk_term_name =   (*rk_longname) $ rk_tnm . thx
                 FENCE( *RkIsNameP(rk_tnm) . *Shift('R_NAME', thx) FENCE('(' *rk_ws *rk_typename *rk_ws ')' | '[' *rk_arglist_drop *rk_ws ']' | epsilon)
                      | epsilon . *Shift('R_CALLNM', thx) *rk_args . *RkCall()
                      );
rk_lambda    =   ('->' | '<->') *rk_ws *RkScopeEnter() FENCE(*rk_signature *rk_ws *rk_blockoid *RkScopeLeave() | *RkScopeLeave() FAIL) . *RkBlockTerm();
rk_fatarrow  =   (*rk_identifier) . thx *rk_hs '=>' . *Shift('R_FATKEY', thx) *rk_ws *RkLimPush(33) FENCE(*rk_xlist *RkLimPop() | *RkLimPop() FAIL)
                 . *Reduce('R_FAT', 2);
rk_self      =   'self' *rk_kwend_tail . *Shift('R_SELF', 'self');
rk_star      =   ('**' | '*') . thx . *Shift('R_STAR', thx);
rk_yada      =   ('...' | '???' | '!!!') *rk_args . *RkYada();
rk_reduce    =   '[' ('\' | epsilon) (*rk_iop_table) . thx ']' . *Shift('R_ROP', thx) *rk_args . *RkReduceTerm();
rk_term      =   FENCE( *rk_variable . thx . *Shift('R_VAR', thx)
                      | (*rk_numish) . thx . *Shift('R_NUM', thx)
                      | *rk_colonpair
                      | *rk_quote
                      | *rk_reduce
                      | '[' *rk_semilist *rk_ws ']' . *RkBracket()
                      | '(' *rk_semilist *rk_ws ')' . *RkParen()
                      | *rk_lambda
                      | @rk_q *RkChAt(rk_q, '{') . *Shift('R_NOSIG', '') *rk_sblockoid . *RkBlockTerm()
                      | '<' *rk_ang_body . thx '>' . *Shift('R_WORDS', thx)
                      | *rk_star
                      | *rk_yada
                      | '.' *rk_dottyop . *Reduce('R_DOTTY', 1)
                      | *rk_fatarrow
                      | *rk_keyword_term
                      | *rk_version . thx . *Shift('R_NAME', thx)
                      | *rk_term_ident
                      | *rk_term_name
                      );
/* ==================================================================================================================== */
/* postfixes: r_postfixish (no white space before), r_postcircumfix, r_dotty, r_methodop                              */
/* ==================================================================================================================== */
rk_methodargs =  FENCE( *rk_args_paren . *Shift('R_FORM', 1)
                      | ':' @rk_q *RkIsSpaceAt(rk_q) *rk_arglist . *Shift('R_FORM', 2)
                      | epsilon . *Shift('R_NOARGS', '') . *Shift('R_FORM', 0)
                      );
rk_methodop  =   ( (*rk_longname) . thx | (ANY('$@&') *rk_varname) . thx ) . *Shift('R_MNAME', thx) *rk_methodargs . *RkMethod();
rk_dottyop   =   FENCE( ANY('+*?^') . thx . *Shift('R_MMOD', thx) *rk_methodop . *RkMod() | *rk_methodop );
rk_postfix   =   FENCE( (*rk_postop_table) . thx . *Shift('R_PF_OP', thx)
                      | '[' *rk_semilist *rk_ws ']' . *RkPf('R_PF_IDX', 1)
                      | '{' *rk_semilist *rk_ws '}' . *RkPf('R_PF_HASH', 1)
                      | '<' (*rk_ang_body) . thx '>' . *Shift('R_PF_ANG', thx) . *RkPf('R_PF_ANGW', 1)
                      | *rk_args_paren . *RkPf('R_PF_CALL', 1)
                      | '.' ( '[' *rk_semilist *rk_ws ']' . *RkPf('R_PF_IDX', 0)
                            | '{' *rk_semilist *rk_ws '}' . *RkPf('R_PF_HASH', 0)
                            | '<' (*rk_ang_body) . thx '>' . *Shift('R_PF_ANG', thx) . *RkPf('R_PF_ANGW', 0)
                            | *rk_args_paren . *RkPf('R_PF_CALL', 0)
                            | *rk_dottyop
                            )
                      | '!' @rk_q *RkNotAt(rk_q, '!=~') . *Shift('R_MMOD', '!') *rk_methodop . *RkMod()
                      | '>>' '.' *rk_dottyop . *RkHyper()
                      );
rk_postfixes =   FENCE(*rk_postfix . *IncCounter() *rk_postfixes | epsilon);
rk_prefix1   =   (*rk_prefixop) . thx . *Shift('R_OPTXT', thx) . *IncCounter() *rk_ws;
rk_prefixes  =   FENCE(*rk_prefix1 *rk_prefixes | epsilon);
rk_termish   =   epsilon . *PushCounter() *rk_prefixes . *Reduce('R_PRE', nTop()) . *PopCounter()
                 epsilon . *PushCounter() *rk_term *rk_postfixes . *Reduce('R_TP', nTop() + 1) . *PopCounter()
                 . *Reduce('R_TERMISH', 2);
/* ==================================================================================================================== */
/* the expression: r_EXPR as the C's x_* climb -- x_list, x_elem, x_expr, x_tern, x_or ... x_mul, x_unary            */
/* ==================================================================================================================== */
rk_xunary    =   FENCE( ('so' | 'not') . thx *rk_kwend_tail . *Shift('R_OPTXT', thx) *rk_ws *rk_xtern . *Reduce('R_LOOSE', 2)
                      | *rk_termish FENCE(*rk_infix_at *RkLvIs('POW') *rk_opshift *rk_ws *rk_xunary . *RkBin('POW') | epsilon)
                      );
rk_xmul      =   *rk_xunary *rk_xmul_r;
rk_xmul_r    =   FENCE(*rk_infix_at *RkLvIs('MUL') *rk_opshift *rk_ws *rk_xunary . *RkBin('MUL') *rk_xmul_r | epsilon);
rk_xaddsub   =   *rk_xmul *rk_xaddsub_r;
rk_xaddsub_r =   FENCE(*rk_infix_at *RkLvIs('ADDSUB') *rk_opshift *rk_ws *rk_xmul . *RkBin('ADDSUB') *rk_xaddsub_r | epsilon);
rk_xrepl     =   *rk_xaddsub *rk_xrepl_r;
rk_xrepl_r   =   FENCE(*rk_infix_at *RkLvIs('REPL') *rk_opshift *rk_ws *rk_xaddsub . *RkBin('REPL') *rk_xrepl_r | epsilon);
rk_xcat      =   *rk_xrepl *rk_xcat_r;
rk_xcat_r    =   FENCE(*rk_infix_at *RkLvIs('CAT') *rk_opshift *rk_ws *rk_xrepl . *RkBin('CAT') *rk_xcat_r | epsilon);
rk_xrange    =   *rk_xcat FENCE(*rk_infix_at *RkLvIs('RANGE1') *rk_opshift *rk_ws *rk_xcat . *RkBin('RANGE1') | epsilon) *rk_xrange_r;
rk_xrange_r  =   FENCE(*rk_infix_at *RkLvIs('RANGE2') *rk_opshift *rk_ws *rk_xcat . *RkBin('RANGE2') *rk_xrange_r | epsilon);
rk_xdor      =   *rk_xrange *rk_xdor_r;
rk_xdor_r    =   FENCE(*rk_infix_at *RkLvIs('DOR') *rk_opshift *rk_ws *rk_xrange . *RkBin('DOR') *rk_xdor_r | epsilon);
rk_xjct      =   *rk_xdor *rk_xjct_r;
rk_xjct_r    =   FENCE(*rk_infix_at *RkLvIs('JCT') *rk_opshift *rk_ws *rk_xrange . *RkBin('JCT') *rk_xjct_r | epsilon);
rk_xdivis    =   *rk_xjct *rk_xdivis_r;
rk_xdivis_r  =   FENCE(*rk_infix_at *RkLvIs('DIVIS') *rk_opshift *rk_ws *rk_xjct . *RkBin('DIVIS') *rk_xdivis_r | epsilon);
rk_xcmp      =   *rk_xdivis *rk_xcmp_r;
rk_xcmp_r    =   FENCE(*rk_infix_at *RkLvIs('CMP') *rk_opshift *rk_ws
                       FENCE( *RkIsSmart() *rk_xdivis . *RkBin('SMART')
                            | *rk_xdivis . *RkBin('CMP')
                            ) *rk_xcmp_r
                     | epsilon);
rk_xand      =   *rk_xcmp *rk_xand_r;
rk_xand_r    =   FENCE(*rk_infix_at *RkLvIs('AND') *rk_opshift *rk_ws *rk_xcmp . *RkBin('AND') *rk_xand_r | epsilon);
rk_xor       =   *rk_xand *rk_xor_r;
rk_xor_r     =   FENCE(*rk_infix_at *RkLvIs('OR') *rk_opshift *rk_ws *rk_xand . *RkBin('OR') *rk_xor_r | epsilon);
rk_xtern     =   *rk_xor FENCE(*rk_ws '??' @rk_q *RkNotAt(rk_q, '?') *RkLimOk(38) *rk_ws *RkLimPush(34) FENCE(*rk_xlist *RkLimPop() | *RkLimPop() FAIL)
                               *rk_ws '!!' *rk_ws *rk_xtern . *Reduce('R_TERN', 3)
                             | epsilon);
rk_plainvar  =   epsilon . *Reduce('R_PRE', 0) (*rk_variable) $ rk_pvx . thx @rk_q *RkNoPostfixAt(rk_q) . *Shift('R_VAR', thx) . *Reduce('R_TP', 1) . *Reduce('R_TERMISH', 2);
rk_xexpr     =   FENCE( *rk_plainvar *rk_infix_at *RkAsgFor(rk_pvx) *rk_opshift *rk_ws *rk_xexpr . *RkBin('ASG')
                      | *rk_xtern FENCE(*rk_infix_at *RkLvIs('PAIR') *rk_opshift *rk_ws *rk_xexpr . *RkBin('PAIR') | epsilon)
                      );
rk_xelem_r   =   FENCE(*rk_infix_at *RkLeftover() *rk_opshift . *IncCounter() *rk_ws *rk_xexpr . *IncCounter() *rk_xelem_r | epsilon);
rk_xelem     =   epsilon . *PushCounter() *rk_xexpr . *IncCounter() *rk_xelem_r . *Reduce('R_EL', nTop()) . *PopCounter();
rk_xlist_r   =   FENCE(*rk_infix_at *RkIsComma() *rk_ws FENCE(*rk_xelem | epsilon . *Shift('R_EMPTY', '') . *Reduce('R_EL', 1)) . *IncCounter() *rk_xlist_r | epsilon);
rk_xlist_core =  epsilon . *PushCounter() *rk_xelem . *IncCounter() *rk_xlist_r . *Reduce('R_LIST', nTop()) . *PopCounter();
rk_xcross_r  =   FENCE(*rk_infix_at *RkIsListInfix() *rk_opshift . *IncCounter() *rk_ws *rk_xlist_core . *IncCounter() *rk_xcross_r | epsilon);
rk_xlist     =   epsilon . *PushCounter() *rk_xlist_core . *IncCounter() *rk_xcross_r . *RkCrossRed() . *PopCounter();
rk_EXPR0     =   *RkLimPush(0) FENCE(*rk_xlist *RkLimPop() | *RkLimPop() FAIL);
/* ==================================================================================================================== */
/* blocks: r_pblock, r_block, r_blockoid (the statement list between braces, then r_ENDSTMT), r_xblock               */
/* ==================================================================================================================== */
rk_blockbody =   *RkGoalPush(0) FENCE(*rk_statementlist *rk_ws '}' *RkGoalPop() | *RkGoalPop() FAIL);
rk_blockoid  =   '{' . *PushCounter() *rk_blockbody . *RkStmts('BLOCK') . *PopCounter() *rk_endstmt;
rk_blockoid_sub = '{' . *PushCounter() *rk_blockbody . *RkStmts('SUB') . *PopCounter() *rk_endstmt;
rk_blockoid_method = '{' . *PushCounter() *rk_blockbody . *RkStmts('METHOD') . *PopCounter() *rk_endstmt;
rk_blockoid_class = '{' . *PushCounter() *rk_blockbody . *RkStmts('CLASS') . *PopCounter() *rk_endstmt;
rk_blockoid_grammar = '{' . *PushCounter() *rk_blockbody . *RkStmts('GRAMMAR') . *PopCounter() *rk_endstmt;
rk_blockoid_module = '{' . *PushCounter() *rk_blockbody . *RkStmts('MODULE') . *PopCounter() *rk_endstmt;
rk_blockoid_given = '{' . *PushCounter() *rk_blockbody . *RkStmts('GIVEN') . *PopCounter() *rk_endstmt;
rk_blockoid_catch = '{' . *PushCounter() *rk_blockbody . *RkStmts('CATCH') . *PopCounter() *rk_endstmt;
rk_sblockoid =   *RkScopeEnter() FENCE(*rk_blockoid *RkScopeLeave() | *RkScopeLeave() FAIL);
rk_pblock    =   FENCE( ('->' | '<->') *rk_ws *RkScopeEnter() FENCE(*rk_signature *rk_ws *rk_blockoid *RkScopeLeave() | *RkScopeLeave() FAIL)
                      | epsilon . *Shift('R_NOSIG', '') *rk_sblockoid
                      );
rk_xblock    =   *RkLimPush(0) FENCE(*rk_xlist *RkLimPop() | *RkLimPop() FAIL) *rk_ws *rk_pblock;
/* ==================================================================================================================== */
/* the statement: r_statement, r_eat_terminator, r_statementlist, r_statement_mods, r_label                          */
/* ==================================================================================================================== */
rk_semi_flag =   FENCE(*rk_ws @rk_q *RkChAt(rk_q, ';') . *Shift('R_SEMI', 1) | epsilon . *Shift('R_SEMI', 0));
rk_stmt_mod1 =   (  ('if' | 'unless' | 'when' | 'without' | 'with') . thx *rk_kok_tail . *Shift('R_MODK', thx) . *IncCounter() *rk_EXPR0 . *IncCounter()
                    FENCE(*rk_ws ('while' | 'until' | 'for' | 'given') . thx *rk_kok_tail . *Shift('R_MODK', thx) . *IncCounter() *rk_EXPR0 . *IncCounter() | epsilon)
                 |  ('while' | 'until' | 'for' | 'given') . thx *rk_kok_tail . *Shift('R_MODK', thx) . *IncCounter() *rk_EXPR0 . *IncCounter()
                 );
rk_stmt_mods =   epsilon . *PushCounter() FENCE(@rk_q *RkNotMarkedEnd(rk_q) *rk_ws @rk_q *RkNotMarkedEnd(rk_q) *rk_stmt_mod1 | epsilon) . *Reduce('R_MODS', nTop()) . *PopCounter();
rk_label     =   (*rk_identifier) $ rk_lab ':' @rk_q *RkIsSpaceAt(rk_q) *RkAddName(rk_lab) *rk_ws;
rk_statement =   @rk_q *RkStmtStart(rk_q)
                 FENCE( ';' . *Shift('R_EMPTYSTMT', '') *RkSemiSeen()
                      | *rk_label *rk_statement
                      | *rk_statement_control . *Reduce('R_CTL', 1) *rk_semi_flag . *Reduce('R_STMT', 2)
                      | *rk_EXPR0 *rk_stmt_mods . *Reduce('R_EXPRSTMT', 2) *rk_semi_flag . *Reduce('R_STMT', 2)
                      );
rk_eat_terminator = *rk_ws FENCE(';' | @rk_q *RkTermOk(rk_q) | ABORT);
rk_stmts_r   =   FENCE(@rk_q *RkListStop(rk_q) | *rk_statement . *IncCounter() *rk_eat_terminator *rk_ws *rk_stmts_r | epsilon);
rk_statementlist = *rk_ws *rk_stmts_r;
/* ==================================================================================================================== */
/* statement control: r_statement_control (if/with, unless/without, while/until, repeat, for, loop, given, when,     */
/* default, CATCH/CONTROL/QUIT, use/no/need/import/require)                                                          */
/* ==================================================================================================================== */
rk_elsifs    =   FENCE(*rk_ws ('elsif' | 'orwith') *rk_kok_tail *rk_xblock . *IncCounter() . *IncCounter() . *IncCounter() *rk_elsifs | epsilon);
rk_ctl_if    =   ('if' | 'with') *rk_kok_tail . *PushCounter() *rk_xblock . *IncCounter() . *IncCounter() . *IncCounter() *rk_elsifs
                 FENCE(*rk_ws 'else' *rk_kwend_tail *rk_ws *rk_pblock . *IncCounter() . *IncCounter() | epsilon) . *Reduce('R_IF', nTop()) . *PopCounter();
rk_ctl_loop  =   'loop' *rk_kok_tail FENCE( '(' *rk_ws FENCE(*rk_EXPR0 | epsilon . *Shift('R_NOEXPR', '')) *rk_ws ';' *rk_ws FENCE(*rk_EXPR0 | epsilon . *Shift('R_NOEXPR', ''))
                                             *rk_ws ';' *rk_ws FENCE(*rk_EXPR0 | epsilon . *Shift('R_NOEXPR', '')) *rk_ws ')' *rk_ws *rk_sblockoid @rk_q *RkLoopEnd(rk_q) . *Reduce('R_CLOOP', 4)
                                          | epsilon *rk_sblockoid @rk_q *RkLoopEnd(rk_q) . *Reduce('R_LOOP', 1)
                                          );
rk_use_args  =   FENCE(@rk_q *RkIsSpaceOrHash(rk_q) *rk_ws @rk_q *RkNotStdStop(rk_q) *rk_arglist | epsilon . *Shift('R_NOARGS', ''));
rk_use_like  =   FENCE( (*rk_version) . thx . *Shift('R_USEV', thx)
                      | (*rk_longname) . thx . *Shift('R_USEM', thx) FENCE('[' BREAK(']') ']' | epsilon) *rk_use_args . *Reduce('R_USEM', 2)
                      );
rk_statement_control = @rk_q *RkAlphaAt(rk_q)
                 FENCE( *rk_ctl_if
                      | ('unless' | 'without') *rk_kok_tail *rk_xblock . *Reduce('R_UNLESS', 3)
                      | ('while' | 'until') . thx *rk_kok_tail . *Shift('R_WHK', thx) *rk_xblock @rk_q *RkLoopEnd(rk_q) . *Reduce('R_WHILE', 4)
                      | 'repeat' *rk_kok_tail FENCE( ('while' | 'until') . thx *rk_kok_tail . *Shift('R_WHK', thx) *rk_xblock @rk_q *RkLoopEnd(rk_q) . *Reduce('R_REPEATW', 4)
                                                  | *rk_pblock *rk_ws ('while' | 'until') . thx *rk_kok_tail . *Shift('R_WHK', thx) *rk_EXPR0 @rk_q *RkLoopEnd(rk_q) . *Reduce('R_REPEAT', 4)
                                                  )
                      | 'for' *rk_kok_tail *rk_xblock @rk_q *RkLoopEnd(rk_q) . *Reduce('R_FOR', 3)
                      | 'whenever' *rk_kok_tail *rk_xblock . *RkDropN(3) . *Shift('R_NOTREE', '')
                      | 'foreach' *rk_kwend_tail ABORT
                      | *rk_ctl_loop
                      | 'need' *rk_kok_tail *rk_longname FENCE(ARBNO(*rk_ws ',' *rk_ws *rk_longname)) . *Shift('R_NOTREE', '')
                      | 'import' *rk_kw_tail *rk_ws *rk_longname *rk_use_args . *RkDropN(1) . *Shift('R_NOTREE', '')
                      | 'no' *rk_kw_tail @rk_q *RkIsSpaceAt(rk_q) *rk_ws *rk_use_like . *RkDropN(1) . *Shift('R_NOTREE', '')
                      | 'use' *rk_kw_tail @rk_q *RkIsSpaceAt(rk_q) *rk_ws *rk_use_like
                      | 'require' *rk_kok_tail FENCE(*rk_longname | *rk_variable) FENCE(*rk_ws @rk_q *RkNotStdStop(rk_q) *rk_EXPR0 . *RkDropN(1) | epsilon) . *Shift('R_NOTREE', '')
                      | 'given' *rk_kok_tail *RkLimPush(0) FENCE(*rk_xlist *RkLimPop() | *RkLimPop() FAIL) *rk_ws . *Shift('R_NOSIG', '') *RkScopeEnter() FENCE(*rk_blockoid_given *RkScopeLeave() | *RkScopeLeave() FAIL) . *Reduce('R_GIVEN', 3)
                      | 'when' *rk_kok_tail *rk_xblock . *Reduce('R_WHEN', 3)
                      | 'default' *rk_kok_tail *rk_sblockoid . *Reduce('R_DEFAULT', 1)
                      | ('CATCH' | 'CONTROL' | 'QUIT') *rk_kw_tail *rk_ws @rk_q *RkChAt(rk_q, '{') *RkScopeEnter() FENCE(*rk_blockoid_catch *RkScopeLeave() | *RkScopeLeave() FAIL) . *Reduce('R_CATCH', 1)
                      );
/* ==================================================================================================================== */
/* declarations: r_scoped, r_declarator, r_variable_declarator, r_initializer, r_multi_declarator                     */
/* ==================================================================================================================== */
rk_trait1    =   FENCE( 'is' *rk_kw_tail *rk_ws (*rk_longname) . thx FENCE('(' BREAK(')') ')' | '<' BREAK('>') '>' | '[' BREAK(']') ']' | epsilon) . *Shift('R_TRAIT', 'is') . *Shift('R_TRAITN', thx) . *Reduce('R_TRAIT', 2)
                      | ('does' | 'hides' | 'of') . rk_tw *rk_kw_tail *rk_ws (*rk_typename) . thx . *Shift('R_TRAIT', rk_tw) . *Shift('R_TRAITN', thx) . *Reduce('R_TRAIT', 2)
                      | 'returns' *rk_kw_tail *rk_ws *rk_typename
                      | 'handles' *rk_kw_tail *rk_ws FENCE('<' BREAK('>') . thx '>' | (*rk_identifier) . thx) . *Shift('R_TRAIT', 'handles') . *Shift('R_TRAITN', thx) . *Reduce('R_TRAIT', 2)
                      );
rk_traits_r  =   FENCE(*rk_ws *rk_trait1 . *RkTraitCount() *rk_traits_r | epsilon);
rk_traits    =   epsilon . *PushCounter() *rk_traits_r . *RkTraitsRed() . *PopCounter();
rk_typename  =   *rk_longname $ rk_tyn *RkIsNameP(rk_tyn) FENCE('[' BREAK(']') ']' | epsilon) FENCE('(' BREAK(')') ')' | epsilon)
                 FENCE(*rk_ws 'of' *rk_kw_tail *rk_ws *rk_typename | epsilon);
rk_initializer = FENCE( '=' @rk_q *RkNotAt(rk_q, '=>') . *Shift('R_INITOP', '=') *rk_ws *RkLimPush(rk_ilim) FENCE(*rk_xlist *RkLimPop() | *RkLimPop() FAIL)
                      | ':=' . *Shift('R_INITOP', ':=') *rk_ws *RkLimPush(18) FENCE(*rk_xlist *RkLimPop() | *RkLimPop() FAIL)
                      | '.=' . *Shift('R_INITOP', '.=') *rk_ws *rk_dottyop . *Reduce('R_DOTTY', 1) . *RkListOf1()
                      );
rk_vardecl   =   (*rk_variable) $ rk_dvx . thx *RkDeclName(rk_dvx) . *Shift('R_DVAR', thx)
                 FENCE('[' *rk_semilist *rk_ws ']' . *RkDropN(1) | '{' *rk_semilist *rk_ws '}' . *RkDropN(1) | epsilon) *rk_traits
                 FENCE(*rk_ws 'where' *rk_kw_tail *rk_ws *RkLimPush(34) FENCE(*rk_xlist . *RkDropN(1) *RkLimPop() | *RkLimPop() FAIL) | epsilon);
rk_declarator =  FENCE( '\' (*rk_identifier) $ rk_dvx . thx *RkAddValueName(rk_dvx) . *Shift('R_DVAR', '$' thx) . *Shift('R_TRAITS', '') *rk_ws *RkInitLim('$') *rk_initializer . *Reduce('R_DECLV', 4)
                      | @rk_q *RkSigilAt(rk_q) *rk_vardecl *RkInitLim(rk_dvx) FENCE(*rk_ws *rk_initializer . *Reduce('R_DECLV', 4) | epsilon . *Reduce('R_DECLV', 2))
                      | '(' *rk_signature *rk_ws ')' *rk_traits FENCE(*rk_ws *RkInitLim('(') *rk_initializer . *Reduce('R_DECLP', 4) | epsilon . *Reduce('R_DECLP', 2))
                      | *rk_routine_declarator
                      | *rk_regex_declarator
                      | *rk_type_declarator
                      );
rk_typenames =   FENCE((*rk_typename) . thx . *Shift('R_TYPE', thx) . *IncCounter() *rk_ws *rk_typenames | epsilon);
rk_scoped    =   *rk_ws FENCE( *rk_declarator epsilon . *Shift('R_TYPE', '') . *RkSwap()
                             | *rk_regex_declarator epsilon . *Shift('R_TYPE', '') . *RkSwap()
                             | *rk_package_declarator epsilon . *Shift('R_TYPE', '') . *RkSwap()
                             | epsilon . *PushCounter() *rk_typenames . *RkFirstType() . *PopCounter() FENCE(*rk_multi_declarator | *rk_declarator)
                             | *rk_multi_declarator epsilon . *Shift('R_TYPE', '') . *RkSwap()
                             );
rk_scope_kw  =   ('my' | 'our' | 'has' | 'HAS' | 'augment' | 'anon' | 'state' | 'supersede' | 'unit') $ rk_sk . thx @rk_q *RkScopeKwOk(rk_q);
rk_scoped_term = *rk_scope_kw . *Shift('R_SCOPE', thx) *RkScopePush(rk_sk) FENCE(*rk_scoped *RkScopePop() | *RkScopePop() FAIL) . *Reduce('R_SCOPED', 3);
rk_multi_declarator = ('multi' | 'proto' | 'only') $ rk_mk . thx *rk_kok_tail . *Shift('R_MULTI', thx) *RkMultiPush(rk_mk)
                 FENCE(FENCE(*rk_declarator | *rk_routine_def_sub) *RkMultiPop() | *RkMultiPop() FAIL) . *Reduce('R_MULTI', 2);
/* ==================================================================================================================== */
/* routines: r_routine_declarator, r_routine_def (name, signature, traits, block), r_signature, r_parameter           */
/* ==================================================================================================================== */
rk_routine_declarator = FENCE( 'submethod' *rk_kwend_tail *rk_ws . *Shift('R_RKIND', 2) *rk_routine_def_m
                             | 'method' *rk_kwend_tail *rk_ws . *Shift('R_RKIND', 1) *rk_routine_def_m
                             | 'sub' *rk_kwend_tail *rk_ws . *Shift('R_RKIND', 0) *rk_routine_def_s
                             );
rk_routine_def_sub = epsilon . *Shift('R_RKIND', 0) *rk_routine_def_s;
rk_routine_def_s = FENCE( (*rk_deflongname) $ rk_rn . thx *RkRegisterRoutine(rk_rn) . *Shift('R_RNAME', thx) *rk_ws
                               *RkScopeEnter() FENCE(*rk_routine_head *rk_blockoid_sub *RkScopeLeave() | *RkScopeLeave() FAIL)
                         | epsilon . *Shift('R_RNAME', '') *RkScopeEnter() FENCE(*rk_routine_head *rk_blockoid *RkScopeLeave() | *RkScopeLeave() FAIL)
                         ) . *Reduce('R_ROUTINE', 4);
rk_routine_def_m = FENCE( (ANY('!^') | epsilon) (*rk_longname) . thx . *Shift('R_RNAME', thx) *rk_ws
                               *RkScopeEnter() FENCE(*rk_routine_head *rk_blockoid_method *RkScopeLeave() | *RkScopeLeave() FAIL)
                         | epsilon . *Shift('R_RNAME', '') *RkScopeEnter() FENCE(*rk_routine_head *rk_blockoid_method *RkScopeLeave() | *RkScopeLeave() FAIL)
                         ) . *Reduce('R_ROUTINE', 4);
rk_routine_head = FENCE('(' *rk_signature *rk_ws ')' *rk_ws | epsilon . *Shift('R_NOSIG', '')) *rk_traits . *RkDropN(1) *rk_ws;
rk_signature =   epsilon . *PushCounter() *RkDeclPush(1) FENCE(*rk_ws *rk_params *rk_ws *rk_retsig *RkDeclPop() | *RkDeclPop() FAIL) . *Reduce('R_SIG', nTop()) . *PopCounter();
rk_params    =   FENCE(@rk_q *RkSigEnd(rk_q) | *rk_parameter . *IncCounter() *rk_ws FENCE(ANY(',;:') *rk_ws *rk_params | epsilon) | epsilon);
rk_retsig    =   FENCE('-->' *rk_ws FENCE(*rk_typename | *rk_quote . *RkDropN(1) | *rk_numish | *rk_term . *RkDropN(1)) *rk_ws | epsilon);
rk_typecons  =   FENCE( 'where' *rk_kw_tail *rk_ws *RkLimPush(34) FENCE(*rk_xlist . *RkDropN(1) *RkLimPop() | *RkLimPop() FAIL)
                      | *rk_quote . *RkDropN(1)
                      | *rk_numish
                      | ANY('-+') *rk_numish
                      | @rk_q *RkAlphaOrColonAt(rk_q) *rk_typename
                      );
rk_typecons_r =  FENCE((*rk_typecons) . thx *RkNtcInc() . *Shift('R_PTYPE', thx) . *IncCounter() *rk_ws *rk_typecons_r | epsilon);
rk_param_var =   FENCE( ANY('$@%&') FENCE(ANY('.!^:*?=~') @rk_q *RkWordAt(rk_q) | epsilon) FENCE(*rk_identifier | ANY('/!') | epsilon)
                        FENCE('[' BREAK(']') ']' | epsilon)
                      | '[' *rk_signature *rk_ws ']' . *RkDropN(1)
                      | '(' *rk_signature *rk_ws ')' . *RkDropN(1)
                      );
rk_parameter =   *RkNtc0() epsilon . *PushCounter() *rk_typecons_r . *Reduce('R_PTYPES', nTop()) . *PopCounter()
                 FENCE( ('**' | '*' | '+') . thx . *Shift('R_PPRE', thx) FENCE((*rk_param_var) $ rk_pv . thx *RkParamName(rk_pv) . *Shift('R_PVAR', thx) | (*rk_identifier) . thx . *Shift('R_PVAR', thx) | epsilon . *Shift('R_PVAR', ''))
                      | ANY('\|') . *Shift('R_PPRE', '|') (*rk_identifier) . thx . *Shift('R_PVAR', thx)
                      | epsilon . *Shift('R_PPRE', '') FENCE( (*rk_param_var) $ rk_pv . thx *RkParamName(rk_pv) . *Shift('R_PVAR', thx)
                                                            | ':' FENCE((*rk_identifier) '(' *rk_ws (*rk_param_var) . thx *rk_ws ')' | (*rk_param_var) . thx) . *Shift('R_PVAR', thx)
                                                            | *RkHasTypes() . *Shift('R_PVAR', '')
                                                            )
                      )
                 FENCE(ANY('?!') | epsilon) *rk_ws *rk_traits . *RkDropN(1)
                 FENCE(*rk_ws 'where' *rk_kw_tail *rk_ws *RkLimPush(34) FENCE(*rk_xlist . *RkDropN(1) *RkLimPop() | *RkLimPop() FAIL) | epsilon) *rk_ws
                 FENCE('=' @rk_q *RkNotAt(rk_q, '=>') *rk_ws *RkLimPush(34) FENCE(*rk_xlist *RkLimPop() | *RkLimPop() FAIL) | epsilon . *Shift('R_NODEF', ''))
                 . *Reduce('R_PARAM', 4);
/* ==================================================================================================================== */
/* regexes, packages and types: r_regex_def, r_package_def, r_type_declarator (enum, subset, constant)               */
/* ==================================================================================================================== */
rk_regex_declarator = ('rule' | 'token' | 'regex') . thx *rk_kok_tail . *Shift('R_RXKIND', thx) *rk_regex_def;
rk_regex_def =   (*rk_deflongname) . thx . *Shift('R_RXNAME', thx) *rk_ws
                 FENCE(ARBNO(FENCE(':' | epsilon) '(' BREAK(')') ')' *rk_ws | *rk_trait1 . *RkDropN(1) *rk_ws))
                 '{' (*rk_rx_brace_body) . thx '}' . *Shift('R_RXBODY', thx) *rk_endstmt . *Reduce('R_REGEX', 3);
rk_rx_brace_body = FENCE(BREAK('{}\' "'" '"') FENCE( '\' LEN(1) *rk_rx_brace_body | '{' *rk_rx_brace_body '}' *rk_rx_brace_body
                                                 | "'" BREAK("'") "'" *rk_rx_brace_body | '"' BREAK('"') '"' *rk_rx_brace_body | epsilon));
rk_package_declarator = FENCE( ('class' | 'role') . thx *rk_kok_tail . *Shift('R_PKIND', thx) *rk_package_head *rk_blockoid_class *RkScopeLeave() . *Reduce('R_PACKAGE', 4)
                              | 'grammar' . thx *rk_kok_tail . *Shift('R_PKIND', thx) *rk_package_head *rk_blockoid_grammar *RkScopeLeave() . *Reduce('R_PACKAGE', 4)
                              | ('package' | 'module' | 'knowhow' | 'native' | 'slang') . thx *rk_kok_tail . *Shift('R_PKIND', thx) *rk_package_head
                                FENCE(*rk_blockoid_module | ';' . *PushCounter() *rk_statementlist . *RkStmts('MODULE') . *PopCounter()) *RkScopeLeave() . *Reduce('R_PACKAGE', 4)
                              );
rk_package_head = FENCE((*rk_longname) $ rk_pk . thx *RkAddPkgName(rk_pk) . *Shift('R_PNAME', thx) *rk_ws | epsilon . *Shift('R_PNAME', ''))
                  *RkScopeEnter() FENCE('[' *rk_signature *rk_ws ']' *rk_ws . *RkDropN(1) | epsilon) *rk_traits *rk_ws;
rk_type_declarator = FENCE( 'enum' *rk_kok_tail *RkDeclPush(1) FENCE((*rk_longname) $ rk_en *RkAddPkgName(rk_en) | epsilon) *RkDeclPop() *rk_ws *rk_traits . *RkDropN(1) *rk_ws
                            FENCE('<' (*rk_ang_body) $ rk_ew . thx '>' . *Shift('R_ENUM', thx) *RkEnumNames(rk_ew) | *rk_term . *RkDropN(1) . *Shift('R_ENUMX', ''))
                          | 'subset' *rk_kok_tail FENCE((*rk_longname) $ rk_en *RkAddPkgName(rk_en) *rk_ws | epsilon) *rk_traits . *RkDropN(1) *rk_ws
                            FENCE('where' *rk_kw_tail *rk_ws *RkLimPush(18) FENCE(*rk_xlist . *RkDropN(1) *RkLimPop() | *RkLimPop() FAIL) | epsilon) . *Shift('R_SUBSET', '')
                          | 'constant' *rk_kok_tail FENCE('\' | epsilon) FENCE((*rk_identifier) $ rk_cn . thx *RkAddName(rk_cn) | (*rk_variable) . thx) . *Shift('R_CNAME', thx)
                            *rk_ws *rk_traits . *RkDropN(1) *rk_ws *RkInitLim('$') *rk_initializer . *Reduce('R_CONST', 3)
                          );
/* ==================================================================================================================== */
/* keyword terms: r_keyword_term, r_statement_prefix, r_blorst                                                        */
/* ==================================================================================================================== */
rk_blorst    =   FENCE( @rk_q *RkChAt(rk_q, '{') *rk_sblockoid . *Reduce('R_BLORSTB', 1)
                      | epsilon . *PushCounter() *rk_statement . *IncCounter() . *RkStmts('BLORST') . *PopCounter() . *Reduce('R_BLORSTS', 1)
                      );
rk_statement_prefix = FENCE( ('BEGIN' | 'TEMP' | 'CHECK' | 'INIT' | 'ENTER' | 'FIRST' | 'END' | 'LEAVE' | 'KEEP' | 'UNDO' | 'NEXT' | 'LAST'
                             | 'PRE' | 'POST' | 'CLOSE' | 'eager' | 'sink' | 'try' | 'quietly' | 'gather' | 'once' | 'start' | 'supply' | 'react' | 'do')
                             . thx *rk_kok_tail . *Shift('R_SPWORD', thx) *rk_blorst . *Reduce('R_SPREFIX', 2)
                           | ('race' | 'hyper' | 'lazy') . thx *rk_kok_tail . *Shift('R_SPWORD', thx)
                             FENCE('for' *rk_kok_tail *rk_statement_control . *Reduce('R_BLORSTS1', 1) | *rk_blorst) . *Reduce('R_SPREFIX', 2)
                           );
rk_keyword_term = FENCE( *rk_statement_prefix
                       | *rk_scoped_term
                       | *rk_multi_declarator
                       | *rk_routine_declarator
                       | *rk_regex_declarator
                       | *rk_package_declarator
                       | *rk_type_declarator
                       | *rk_self
                       | ('now' | 'time') $ rk_kwx . thx *rk_kwend_tail *RkNotName(rk_kwx) . *Shift('R_NAME', thx)
                       | 'rand' *rk_kw_tail @rk_q *RkNotAt(rk_q, "-'") @rk_q *RkEndKeywordOk(rk_q) . *Shift('R_NAME', 'rand')
                       );
/* ==================================================================================================================== */
/* the compilation unit: r_comp_unit                                                                                 */
/* ==================================================================================================================== */
rk_comp_unit =   POS(0) FENCE('#!' BREAK(CHAR(10)) | epsilon) *RkScopeEnter() . *PushCounter() *rk_statementlist *rk_ws . *RkStmts('MAIN') . *PopCounter()
                 *rk_ws FENCE(RPOS(0) | @rk_q *RkIsFinished() | ABORT);
Compiland    =   *rk_comp_unit;
/* ==================================================================================================================== */
/* rk_tree.c: the builders.  RkTerm, RkList, RkEl and RkDecl are the C's structs; the X_* locals of RkBuildList are   */
/* the C's RkX (one per r_EXPR), visible to the x_* functions it calls by SNOBOL4's dynamic scoping                   */
/* ==================================================================================================================== */
struct rkterm { tk_t, tk_kind, tk_cls, tk_name, tk_npre, tk_pre, tk_npost, tk_ck, tk_cnt, tk_val, tk_lop, tk_in, tk_decl, tk_ctail }
struct rklist { ls_v, ls_n, ls_trailing, ls_nitem, ls_t1, ls_op1, ls_rglo, ls_rghi, ls_rgop, ls_rgnitem }
struct rkel { el_t, el_nitem, el_t0, el_op1, el_rest, el_pdph, el_pdd }
struct rkdecl { dc_scope, dc_sigil, dc_name, dc_type, dc_initop, dc_init, dc_sig, dc_tr }
RkArrN = TABLE(211); RkSlen = TABLE(211); RkCodeV = TABLE(31); RkAls = TABLE(31); RkPostUid = 0; RkTwPostUid = 0; RkDestrUid = 0; RkFmUid = 0; RkNonWhen = 0;
RkTailList = ; RkTailTree = ; RkAfterLine = 0;
/* ==================================================================================================================== */
function mk(t, v)                { mk = tree(t, v, 0); return; }
function mk1(t, v, a)            { mk1 = tree(t, v, 1, RkArr1(a)); return; }
function mk2(t, v, a, b)         { mk2 = tree(t, v, 2, RkArr2(a, b)); return; }
function mk3(t, v, a, b, d)      { mk3 = tree(t, v, 3, RkArr3(a, b, d)); return; }
function ad(x, y)                { if (IDENT(y)) { return; } Append(x, y); ad = x; return; }
function kid(x, i)               { kid = c(x)[i]; return; }
function ilit(n)                 { ilit = mk('TT_ILIT', n); return; }
function MakeCall(nm)            { MakeCall = mk1('TT_FNC', nm, mk('TT_VAR', nm)); return; }
function Call2(fn, l, r)         { Call2 = ad(ad(MakeCall(fn), l), r); return; }
function Call1(fn, x)            { Call1 = ad(MakeCall(fn), x); return; }
function StripSigil(s)           { StripSigil = s; s ? (POS(0) ANY('$@%') REM . StripSigil); return; }
function VarIdent(s)             { if (s ? (POS(0) ANY('@%'))) { VarIdent = s; return; } VarIdent = StripSigil(s); return; }
function TwBare(s)               { TwBare = s; s ? (POS(0) ANY('.!') REM . TwBare); return; }
function IsArr(nm)               { if (DIFFER(RkArrN[nm])) { return; } freturn; }
function VarNode(nm, id) {
    if ((nm ? (POS(0) '&')) DIFFER(RkCodeV[SUBSTR(nm, 2)])) { VarNode = mk('TT_VAR', SUBSTR(nm, 2) '__code'); return; }
    id = VarIdent(nm);
    if (nm ? (POS(0) '@')) { RkArrN[id] = 1; }
    VarNode = mk('TT_VAR', id);
    if (~(nm ? (POS(0) ANY('$@%')))) { RkSlen[VarNode] = 1; }
    return;
}
function NCtx(e) {
    NCtx = e;
    if (IDENT(e)) { return; }
    if (IDENT(t(e), 'TT_VAR') IsArr(v(e))) { NCtx = mk2('TT_METHCALL', '', e, mk('TT_QLIT', 'elems')); }
    return;
}
function Clone(e, x, i) {
    Clone = ;
    if (IDENT(e)) { return; }
    x = tree(t(e), v(e), 0);
    i = 0;
    while (i = LT(i, n(e)) i + 1) { ad(x, Clone(c(e)[i])); }
    Clone = x;
    return;
}
function Seq1(s, q)              { q = mk('TT_SEQ_EXPR', ''); ad(q, s); Seq1 = q; return; }
function QuietStore(tg, r, a)    { a = mk2('TT_ASSIGN', '', tg, r); QuietStore = a; return; }
/* ==================================================================================================================== */
/* terms: rkb_var, rkb_number, rkb_quote (b_dq, b_sq, lower_interp_str), rkb_words, rkb_name                        */
/* ==================================================================================================================== */
function NewTerm(kind)           { NewTerm = rkterm('', kind, '', '', 0, '', 0, '', '', '', '', '', '', ''); return; }
function RkbVar(txt, it, cls, nm) {
    it = NewTerm('VAR');
    if (txt ? (POS(0) ANY('$@') '<' BREAK('>') . nm '>' RPOS(0))) { tk_t(it) = mk1('TT_NAMED_CAPTURE', '', mk('TT_QLIT', RkTrim(nm))); tk_cls(it) = 'N'; RkbVar = it; return; }
    tk_name(it) = txt;
    cls = RkVarCls(txt);
    tk_cls(it) = cls;
    if (IDENT(cls, 'F')) { tk_t(it) = mk1('TT_FH_CAPTURE', '', ilit((IDENT(txt, '$*STDIN') 0, IDENT(txt, '$*STDOUT') 1, 2))); RkbVar = it; return; }
    if (IDENT(cls, 'P')) { tk_t(it) = mk1('TT_CAPTURE', '', ilit(+SUBSTR(txt, 2))); RkbVar = it; return; }
    if (cls ? ANY('TUW')) { tk_t(it) = mk('TT_TWIGIL_FIELD', TwBare(SUBSTR(txt, 2))); RkbVar = it; return; }
    tk_t(it) = VarNode(txt);
    RkbVar = it;
    return;
}
function RkTrim(s)               { RkTrim = s; s ? (POS(0) FENCE(SPAN(' ' CHAR(9) CHAR(10) CHAR(13)) | epsilon) (ARB NOTANY(' ' CHAR(9) CHAR(10) CHAR(13))) . RkTrim FENCE(SPAN(' ' CHAR(9) CHAR(10) CHAR(13)) | epsilon) RPOS(0)); return; }
function RkbNumber(txt, it, u, i, fl, len, d) {
    it = NewTerm('TREE');
    if (txt ? (POS(0) SPAN('0123456789') RPOS(0))) { tk_t(it) = ilit(+txt); RkbNumber = it; return; }
    if (txt ? (POS(0) SPAN('0123456789') FENCE('.' SPAN('0123456789') | epsilon) FENCE(ANY('eE') FENCE(ANY('+-') | epsilon) SPAN('0123456789') | epsilon) RPOS(0))) {
        if (txt ? ('.' | ANY('eE'))) { tk_t(it) = mk('TT_FLIT', RkReal(txt)); RkbNumber = it; return; }
    }
    u = txt;
    while (u ? ('_') =) { }
    if (u ? (POS(0) ('Inf' | 'NaN'))) { tk_t(it) = VarNode(u); RkbNumber = it; return; }
    if (u ? '.') { tk_t(it) = mk('TT_FLIT', RkReal(u)); RkbNumber = it; return; }
    if ((u ? ANY('eE')) ~(u ? (POS(0) '0x'))) { tk_t(it) = mk('TT_FLIT', RkReal(u)); RkbNumber = it; return; }
    tk_t(it) = ilit(RkStrtoll(u));
    RkbNumber = it;
    return;
}
function RkReal(s)               { RkReal = CONVERT(s, 'REAL'); if (IDENT(RkReal)) { RkReal = s; } return; }
function RkStrtoll(u, base, digs, i, d, val) {
    base = 10; digs = u;
    if (u ? (POS(0) '0x' REM . digs)) { base = 16; }
    else { if (u ? (POS(0) '0b' REM . digs)) { base = 2; } else { if (u ? (POS(0) '0o' REM . digs)) { base = 8; } else { if (u ? (POS(0) '0' REM . digs)) { if (DIFFER(digs)) { base = 8; } } } } }
    val = 0;
    i = 0;
    while (i = LT(i, SIZE(digs)) i + 1) {
        d = SUBSTR(digs, i, 1);
        if (~('0123456789abcdefABCDEF' ? (BREAK(d) @rk_dv))) { break; }
        if (GE(rk_dv, 16)) { rk_dv = rk_dv - 6; }
        if (GE(rk_dv, base)) { break; }
        val = val * base + rk_dv;
    }
    RkStrtoll = val;
    return;
}
/* ==================================================================================================================== */
function RkbQuoteTerm(r, it, raw, body) {
    it = NewTerm('QUOTE');
    raw = v(r);
    tk_ctail(it) = raw;
    if (raw ? (POS(0) '"')) { tk_t(it) = RkbDq(RkMid(raw), r); RkbQuoteTerm = it; return; }
    if (raw ? (POS(0) "'")) { tk_t(it) = RkbSq(RkMid(raw)); RkbQuoteTerm = it; return; }
    if (raw ? (POS(0) ('/' | 'rx/' | 'm/'))) {
        body = raw;
        body ? (POS(0) ('/' | 'rx/' | 'm/')) =;
        tk_t(it) = Call1('__rk_regex', mk('TT_QLIT', RkRegexToEngine(RkRegexBody(body, '/'))));
        RkbQuoteTerm = it;
        return;
    }
    tk_t(it) = mk('TT_QLIT', raw);
    RkbQuoteTerm = it;
    return;
}
function RkMid(s)                { RkMid = ''; if (GT(SIZE(s), 2)) { RkMid = SUBSTR(s, 2, SIZE(s) - 2); } return; }
function RkbSq(s, out, ch) {
    out = '';
    while (DIFFER(s)) {
        if (s ? (POS(0) '\' ANY("'\") . ch) =) { out = out ch; }
        else { s ? (POS(0) LEN(1) . ch) =; out = out ch; }
    }
    RkbSq = mk('TT_QLIT', out);
    return;
}
function RkRegexBody(s, stop, out, ch) {
    out = '';
    while (DIFFER(s)) {
        if (s ? (POS(0) '\/') =) { out = out '/'; }
        else {
            s ? (POS(0) LEN(1) . ch) =;
            if (IDENT(ch, stop)) { break; }
            out = out ch;
        }
    }
    RkRegexBody = out;
    return;
}
/* regex_to_engine: Raku regex syntax to the engine's -- white space and comments dropped, quoted text escaped, <[...]>  */
/* classes to [...], [ ] groups to (?: ), $<name>=( to <name>(, the named rules <alpha> <digit> ... to their classes   */
RkNamedRule = TABLE(17);
RkNamedRule['alpha'] = '[A-Za-z]'; RkNamedRule['digit'] = '[0-9]'; RkNamedRule['alnum'] = '[A-Za-z0-9]'; RkNamedRule['upper'] = '[A-Z]';
RkNamedRule['lower'] = '[a-z]'; RkNamedRule['space'] = '\s'; RkNamedRule['xdigit'] = '[0-9A-Fa-f]'; RkNamedRule['ws'] = '\s*';
RkNamedRule['punct'] = '[!-/:-@\[-`{-~]';
function RkReMeta(c)             { if ('\()[]{}<>|*+?.^$' ? c) { return; } freturn; }
function RkRegexToEngine(r, o, c, q, d, nm, neg, cls) {
    o = '';
    while (DIFFER(r)) {
        c = SUBSTR(r, 1, 1);
        if (c ? ANY(' ' CHAR(9) CHAR(10) CHAR(13))) { r = SUBSTR(r, 2); continue; }
        if (IDENT(c, '#')) { if (~(r ? (POS(0) BREAK(CHAR(10))) =)) { r = ''; } continue; }
        if (c ? ANY("'" '"')) {
            q = c; r = SUBSTR(r, 2);
            while (DIFFER(r) ~IDENT(SUBSTR(r, 1, 1), q)) {
                if (IDENT(SUBSTR(r, 1, 1), '\') GT(SIZE(r), 1)) { r = SUBSTR(r, 2); }
                d = SUBSTR(r, 1, 1); r = SUBSTR(r, 2);
                if (RkReMeta(d)) { o = o '\'; }
                o = o d;
            }
            if (DIFFER(r)) { r = SUBSTR(r, 2); }
            continue;
        }
        if (IDENT(c, '\') GT(SIZE(r), 1)) { o = o SUBSTR(r, 1, 2); r = SUBSTR(r, 3); continue; }
        if (r ? (POS(0) '||')) { o = o '|'; r = SUBSTR(r, 3); continue; }
        if (r ? (POS(0) '$<' (SPAN(&UCASE &LCASE '0123456789_-') . nm) '>=(')) { o = o '<' nm '>('; r ? (POS(0) '$<' nm '>=(') =; continue; }
        if (r ? (POS(0) '<' FENCE('-' | epsilon) . neg '[')) {
            r ? (POS(0) '<' neg '[') =;
            o = o '[' (DIFFER(neg) '^', '');
            while (1) {
                while (DIFFER(r) ~IDENT(SUBSTR(r, 1, 1), ']')) {
                    d = SUBSTR(r, 1, 1);
                    if (d ? ANY(' ' CHAR(9) CHAR(10))) { r = SUBSTR(r, 2); continue; }
                    if (IDENT(d, '\') GT(SIZE(r), 1)) { o = o SUBSTR(r, 1, 2); r = SUBSTR(r, 3); continue; }
                    if (r ? (POS(0) '..')) { o = o '-'; r = SUBSTR(r, 3); continue; }
                    if (d ? ANY('-^]')) { o = o '\'; }
                    o = o d; r = SUBSTR(r, 2);
                }
                if (DIFFER(r)) { r = SUBSTR(r, 2); }
                if (r ? (POS(0) '+[')) { r = SUBSTR(r, 3); continue; }
                break;
            }
            o = o ']';
            if (r ? (POS(0) '>')) { r = SUBSTR(r, 2); }
            continue;
        }
        if (r ? (POS(0) '<' (SPAN(&UCASE &LCASE '0123456789_-') . nm) '>')) {
            if (cls = RkNamedRule[nm]) { if (DIFFER(cls)) { o = o cls; r ? (POS(0) '<' nm '>') =; continue; } }
        }
        if (IDENT(c, '[')) { o = o '(?:'; r = SUBSTR(r, 2); continue; }
        if (IDENT(c, ']')) { o = o ')'; r = SUBSTR(r, 2); continue; }
        o = o c; r = SUBSTR(r, 2);
    }
    RkRegexToEngine = o;
    return;
}
/* b_dq: escapes, {closures} (the parse's R_CLOS children, in order), $var and @var interpolation per segment          */
function RkbDq(s, r, buf, segs, nseg, ch, d, acc, k, seg, st, ci, e) {
    buf = ''; segs = ARRAY('1:64'); nseg = 0; ci = 0;
    while (DIFFER(s)) {
        if (s ? (POS(0) '\' LEN(1) . d)) {
            if (d ? ANY('{}')) { buf = buf d; s = SUBSTR(s, 3); continue; }
            if (IDENT(d, 'n')) { buf = buf CHAR(10); s = SUBSTR(s, 3); continue; }
            if (IDENT(d, 't')) { buf = buf CHAR(9); s = SUBSTR(s, 3); continue; }
            if (IDENT(d, 'r')) { buf = buf CHAR(13); s = SUBSTR(s, 3); continue; }
            if (IDENT(d, 'e')) { buf = buf CHAR(27); s = SUBSTR(s, 3); continue; }
            if (IDENT(d, 'a')) { buf = buf CHAR(7); s = SUBSTR(s, 3); continue; }
            if (IDENT(d, 'b')) { buf = buf CHAR(8); s = SUBSTR(s, 3); continue; }
            if (IDENT(d, 'f')) { buf = buf CHAR(12); s = SUBSTR(s, 3); continue; }
            if (IDENT(d, '0')) { buf = buf CHAR(0); s = SUBSTR(s, 3); continue; }
            if (d ? ANY('\"')) { buf = buf d; s = SUBSTR(s, 3); continue; }
            if (IDENT(d, 'x')) {
                if (s ? (POS(0) '\x[' (SPAN('0123456789abcdefABCDEF, ') . e) ']') =) { buf = buf RkHexList(e); continue; }
                if (s ? (POS(0) '\x' (SPAN('0123456789abcdefABCDEF') . e)) =) { buf = buf RkUtf8(RkHexVal(e)); continue; }
            }
            if (IDENT(d, 'c')) {
                if (s ? (POS(0) '\c' (SPAN('0123456789') . e)) =) { buf = buf RkUtf8(+e); continue; }
            }
            buf = buf '\'; s = SUBSTR(s, 2);
            continue;
        }
        if (s ? (POS(0) '{')) {
            e = RkDqClosureEnd(s);
            if (GT(e, 0)) {
                nseg = nseg + 1; segs[nseg] = buf; buf = '';
                s = SUBSTR(s, e + 1);
                continue;
            }
        }
        s ? (POS(0) LEN(1) . ch) =;
        buf = buf ch;
    }
    if (EQ(nseg, 0)) { RkbDq = RkDqSegment(buf); return; }
    nseg = nseg + 1; segs[nseg] = buf;
    acc = ''; k = 0;
    while (k = LT(k, nseg) k + 1) {
        seg = segs[k];
        st = mk('TT_QLIT', seg);
        if (seg ? ANY('$@')) { st = RkInterpStr(seg); }
        acc = (DIFFER(acc) mk2('TT_CAT', '', acc, st), st);
        if (LT(k, nseg)) { ci = ci + 1; acc = mk2('TT_CAT', '', acc, RkClosureExpr(r, ci)); }
    }
    RkbDq = acc;
    return;
}
function RkDqClosureEnd(s, j, n, ch, k) {
    n = SIZE(s); j = 1;
    while (1) {
        if (GE(j, n)) { freturn; }
        ch = SUBSTR(s, j + 1, 1);
        if (IDENT(ch, '}')) { RkDqClosureEnd = j + 1; return; }
        if (ch ? ANY('"' CHAR(10))) { freturn; }
        if (IDENT(ch, '{')) {
            k = j + 1;
            while (LT(k, n) ~IDENT(SUBSTR(s, k + 1, 1), '}')) { if (SUBSTR(s, k + 1, 1) ? ANY('{"' CHAR(10))) { freturn; } k = k + 1; }
            if (GE(k, n)) { freturn; }
            j = k + 1;
            continue;
        }
        j = j + 1;
    }
}
function RkClosureExpr(r, ci, cl, stmts) {
    cl = c(r)[ci];
    if (IDENT(cl)) { RkClosureExpr = mk('TT_QLIT', ''); return; }
    stmts = c(cl)[1];
    RkClosureExpr = RkbParen(RkBlockLastList(stmts), RkBlockN);
    return;
}
function RkHexVal(h, i, d, val)  { val = 0; i = 0; while (i = LT(i, SIZE(h)) i + 1) { d = SUBSTR(h, i, 1); '0123456789abcdef0123456789ABCDEF' ? BREAK(d) @rk_dv; val = val * 16 + REMDR(rk_dv, 16); } RkHexVal = val; return; }
function RkHexList(e, out, h)    { out = ''; while (e ? (POS(0) FENCE(SPAN(', ') | epsilon) SPAN('0123456789abcdefABCDEF') . h) =) { out = out RkUtf8(RkHexVal(h)); } RkHexList = out; return; }
function RkUtf8(cp) {
    if (LT(cp, 128)) { RkUtf8 = CHAR(cp); return; }
    if (LT(cp, 2048)) { RkUtf8 = CHAR(192 + cp / 64) CHAR(128 + REMDR(cp, 64)); return; }
    if (LT(cp, 65536)) { RkUtf8 = CHAR(224 + cp / 4096) CHAR(128 + REMDR(cp / 64, 64)) CHAR(128 + REMDR(cp, 64)); return; }
    RkUtf8 = CHAR(240 + cp / 262144) CHAR(128 + REMDR(cp / 4096, 64)) CHAR(128 + REMDR(cp / 64, 64)) CHAR(128 + REMDR(cp, 64));
    return;
}
function RkDqSegment(seg)        { if (seg ? ANY('$@')) { RkDqSegment = RkInterpStr(seg); return; } RkDqSegment = mk('TT_QLIT', seg); return; }
/* lower_interp_str: $name and @name[...] inside a double-quoted segment, the rest literal, joined with TT_CAT          */
function RkInterpStr(s, result, lit, vn, var, idx, arrpart) {
    result = ''; lit = '';
    while (DIFFER(s)) {
        if (s ? (POS(0) '$' ANY(&UCASE &LCASE '_'))) {
            if (DIFFER(lit)) { result = RkCat(result, mk('TT_QLIT', lit)); lit = ''; }
            s ? (POS(0) '$' FENCE(SPAN(&UCASE &LCASE '_0123456789') | epsilon) . vn) =;
            result = RkCat(result, mk('TT_VAR', vn));
            continue;
        }
        if (s ? (POS(0) '@' ANY(&UCASE &LCASE '_'))) {
            if (DIFFER(lit)) { result = RkCat(result, mk('TT_QLIT', lit)); lit = ''; }
            s ? (POS(0) ('@' FENCE(SPAN(&UCASE &LCASE '_0123456789') | epsilon)) . vn) =;
            if (s ? (POS(0) '[')) {
                s = SUBSTR(s, 2);
                idx = RkInterpSub();
                s ? (POS(0) BREAK(']')) =;
                s ? (POS(0) ']') =;
                arrpart = (DIFFER(idx) RkArrIndex(vn, idx), mk('TT_VAR', vn));
            } else { arrpart = mk('TT_VAR', vn); }
            result = RkCat(result, arrpart);
            continue;
        }
        s ? (POS(0) LEN(1) . var) =;
        lit = lit var;
    }
    if (DIFFER(lit)) { result = RkCat(result, mk('TT_QLIT', lit)); }
    RkInterpStr = (DIFFER(result) result, mk('TT_QLIT', ''));
    return;
}
function RkCat(a, b)             { RkCat = (DIFFER(a) mk2('TT_CAT', '', a, b), b); return; }
function RkInterpPrim(vn, n) {
    s ? (POS(0) SPAN(' ')) =;
    if (s ? (POS(0) (ANY('$@') FENCE(SPAN(&UCASE &LCASE '_0123456789') | epsilon)) . vn) =) { RkInterpPrim = VarNode(vn); return; }
    if (s ? (POS(0) SPAN('0123456789') . n) =) { RkInterpPrim = ilit(+n); return; }
    freturn;
}
function RkInterpSub(left, right, op, k) {
    if (~(left = RkInterpPrim())) { freturn; }
    s ? (POS(0) SPAN(' ')) =;
    if (s ? (POS(0) ANY('+-*/%') . op) =) {
        if (~(right = RkInterpPrim())) { RkInterpSub = left; return; }
        k = (IDENT(op, '+') 'TT_ADD', IDENT(op, '-') 'TT_SUB', IDENT(op, '*') 'TT_MUL', IDENT(op, '/') 'TT_DIV', 'TT_MOD');
        RkInterpSub = mk2(k, '', NCtx(left), NCtx(right));
        return;
    }
    RkInterpSub = left;
    return;
}
function RkbWords(txt, it, call, w, wc, s) {
    it = NewTerm('WORDS');
    tk_name(it) = txt;
    call = MakeCall('__rk_arr'); wc = 0;
    s = txt;
    while (s ? (POS(0) FENCE(SPAN(' ' CHAR(9) CHAR(10)) | epsilon) BREAK(' ' CHAR(9) CHAR(10)) . w) =) { if (IDENT(w)) { break; } ad(call, mk('TT_QLIT', RkUnBs(w))); wc = wc + 1; }
    s ? (POS(0) FENCE(SPAN(' ' CHAR(9) CHAR(10)) | epsilon) REM . w);
    if (DIFFER(w)) { ad(call, mk('TT_QLIT', RkUnBs(w))); wc = wc + 1; }
    tk_t(it) = (EQ(wc, 1) c(call)[2], call);
    RkbWords = it;
    return;
}
function RkUnBs(w, o)            { o = ''; while (w ? (POS(0) (BREAK('\') . rk_ubp) '\\') =) { o = o rk_ubp '\'; } RkUnBs = o w; return; }
function MkBool(b)               { MkBool = ad(MakeCall('__rk_mkbool'), ilit(b)); return; }
function RkIsCoreType(s) {
    if (~(s ? (POS(0) ANY(&UCASE)))) { freturn; }
    if (s ? ':') { freturn; }
    if (s ? (POS(0) ('True' | 'False' | 'Less' | 'Same' | 'More' | 'Inf' | 'NaN' | 'Empty' | 'Nil') RPOS(0))) { freturn; }
    if (DIFFER(RkCoreN[s])) { return; }
    freturn;
}
function RkbName(nm, it) {
    it = NewTerm('NAME');
    tk_name(it) = nm;
    if (nm ? (POS(0) ('True' | 'False') RPOS(0))) { tk_t(it) = MkBool((IDENT(nm, 'True') 1, 0)); RkbName = it; return; }
    if (RkIsCoreType(nm)) { tk_t(it) = Call1('__rk_typeobj', mk('TT_QLIT', nm)); RkbName = it; return; }
    tk_t(it) = VarNode(nm);
    RkbName = it;
    return;
}
/* ==================================================================================================================== */
/* binary operators: rkb_binop (b_mul, b_addsub, b_cmp, the levels), rkb_prefix_apply, rkb_assign, rkb_assign_op      */
/* ==================================================================================================================== */
function Bin(k, l, r)            { Bin = mk2(k, '', l, r); return; }
function RkRangeEx(lo, hi, el) {
    if (IDENT(t(hi), 'TT_ILIT')) { RkRangeEx = Bin('TT_TO', lo, ilit(v(hi) - 1)); return; }
    if (IDENT(t(hi), 'TT_VAR') IsArr(v(hi))) { hi = mk2('TT_METHCALL', '', hi, mk('TT_QLIT', 'elems')); }
    RkRangeEx = Bin('TT_TO', lo, Bin('TT_SUB', hi, ilit(1)));
    return;
}
function RkLogicalAnd(a, b, s)   { s = Bin('TT_SEQ', a, b); RkLogAnd[s] = 1; RkLogicalAnd = s; return; }
RkLogAnd = TABLE(211);
function RkIsChainCmp(k)         { if (k ? (POS(0) ('TT_LT' | 'TT_GT' | 'TT_LE' | 'TT_GE' | 'TT_EQ' | 'TT_NE' | 'TT_LEQ' | 'TT_LNE') RPOS(0))) { return; } freturn; }
function RkChainLast(l) {
    if (IDENT(l)) { freturn; }
    if (RkIsChainCmp(t(l)) EQ(n(l), 2)) { RkChainLast = c(l)[2]; return; }
    if (IDENT(t(l), 'TT_SEQ') EQ(n(l), 2)) { RkChainLast = RkChainLast(c(l)[2]); return; }
    freturn;
}
function RkChainCmp(l, op, r, last) {
    if (last = RkChainLast(l)) { RkChainCmp = RkLogicalAnd(l, Bin(op, Clone(last), r)); return; }
    RkChainCmp = Bin(op, l, r);
    return;
}
function RkOrderCall(fn, l, r)   { RkOrderCall = ad(ad(mk1('TT_FNC', fn, mk('TT_VAR', fn)), l), r); return; }
function MkJunction(fl, l, r, e, i) {
    e = MakeCall(fl);
    if (IDENT(t(l), 'TT_FNC') IDENT(v(l), fl)) { i = 1; while (i = LT(i, n(l)) i + 1) { ad(e, c(l)[i]); } }
    else { ad(e, l); }
    RkMkJunction = ad(e, r);
    MkJunction = e;
    return;
}
function RkbBinop(lv, k, l, r, pc) {
    if (IDENT(lv, 'MUL')) {
        RkbBinop = (EQ(k, 0) Bin('TT_MUL', NCtx(l), NCtx(r)), EQ(k, 1) Call2('__rk_div', l, r), EQ(k, 2) Call2('__rk_mul', l, r), EQ(k, 3) Call2('__rk_sband', l, r),
                    EQ(k, 4) Call2('__rk_mod', l, r), EQ(k, 5) Call2('__rk_lcm', l, r), EQ(k, 6) Bin('TT_DIV', NCtx(l), NCtx(r)), EQ(k, 7) Bin('TT_MOD', NCtx(l), NCtx(r)),
                    EQ(k, 8) Call2('__rk_intdiv', NCtx(l), NCtx(r)), EQ(k, 9) Call2('__rk_gcd', l, r), EQ(k, 10) Call2('iand', l, r), Call2('ishift', l, r));
        return;
    }
    if (IDENT(lv, 'ADDSUB')) {
        RkbBinop = (EQ(k, 0) Bin('TT_ADD', NCtx(l), NCtx(r)), EQ(k, 1) Call2('__rk_sub', l, r), EQ(k, 2) Call2('__rk_lbxor', l, r), EQ(k, 3) Call2('__rk_lbor', l, r),
                    EQ(k, 4) Call2('__rk_sbor', l, r), EQ(k, 5) Call2('__rk_bor', l, r), Bin('TT_SUB', NCtx(l), NCtx(r)));
        return;
    }
    if (IDENT(lv, 'REPL'))   { RkbBinop = (EQ(k, 0) Bin('TT_XREP', l, r), Call2('__rk_arr_xx', l, r)); return; }
    if (IDENT(lv, 'CAT'))    { RkbBinop = (EQ(k, 0) Bin('TT_CAT', l, r), Call2('__rk_compose', l, r)); return; }
    if (IDENT(lv, 'RANGE1')) { RkbBinop = (LT(k, 2) Bin('TT_TO', l, r), RkRangeEx(l, r)); return; }
    if (IDENT(lv, 'RANGE2')) { RkbBinop = Call2((EQ(k, 0) '__rk_unicmp', EQ(k, 1) '__rk_coll', EQ(k, 2) '__rk_range_xb', '__rk_range_xl'), l, r); return; }
    if (IDENT(lv, 'DOR'))    { RkbBinop = Call2('__rk_dor', l, r); return; }
    if (IDENT(lv, 'JCT')) {
        if (EQ(k, 0)) { RkbBinop = MkJunction('any', l, r); return; }
        if (EQ(k, 7)) { RkbBinop = MkJunction('all', l, r); return; }
        RkbBinop = Call2((EQ(k, 1) '__rk_set_sym', EQ(k, 2) '__rk_set_dif', EQ(k, 3) '__rk_set_sum', EQ(k, 4) '__rk_set_uni', EQ(k, 5) '__rk_set_mul', '__rk_set_int'), l, r);
        return;
    }
    if (IDENT(lv, 'DIVIS'))  { RkbBinop = Bin('TT_DIVIS', l, r); return; }
    if (IDENT(lv, 'CMP')) {
        if (EQ(k, 0)) { RkbBinop = RkChainCmp(NCtx(l), 'TT_EQ', NCtx(r)); return; }
        if ((EQ(k, 1), EQ(k, 8))) { RkbBinop = RkChainCmp(NCtx(l), 'TT_NE', NCtx(r)); return; }
        if (EQ(k, 2)) { RkbBinop = RkChainCmp(NCtx(l), 'TT_LT', NCtx(r)); return; }
        if (EQ(k, 3)) { RkbBinop = RkChainCmp(NCtx(l), 'TT_GT', NCtx(r)); return; }
        if ((EQ(k, 4), EQ(k, 6))) { RkbBinop = RkChainCmp(NCtx(l), 'TT_LE', NCtx(r)); return; }
        if ((EQ(k, 5), EQ(k, 7))) { RkbBinop = RkChainCmp(NCtx(l), 'TT_GE', NCtx(r)); return; }
        if (EQ(k, 9))  { RkbBinop = Call2('__rk_not_smartmatch', l, r); return; }
        if (EQ(k, 10)) { RkbBinop = Call2('__rk_approx', l, r); return; }
        if (EQ(k, 11)) { RkbBinop = Call2('__rk_set_elem', l, r); return; }
        if (EQ(k, 12)) { RkbBinop = Call2('__rk_set_cont', l, r); return; }
        if (EQ(k, 13)) { RkbBinop = Call2('__rk_after', l, r); return; }
        if (EQ(k, 14)) { RkbBinop = Call2('__rk_before', l, r); return; }
        if (EQ(k, 15)) { RkbBinop = Call2('__rk_eqv', l, r); return; }
        if (EQ(k, 16)) { RkbBinop = Call2('__rk_ident', l, r); return; }
        if (EQ(k, 17)) { RkbBinop = Bin('TT_LEQ', l, r); return; }
        if (EQ(k, 18)) { RkbBinop = RkOrderCall('__rk_cmp3', l, r); return; }
        if (EQ(k, 19)) { RkbBinop = RkOrderCall('__rk_cmpg', l, r); return; }
        if (EQ(k, 20)) { RkbBinop = RkOrderCall('__rk_leg', l, r); return; }
        if (EQ(k, 21)) { RkbBinop = Bin('TT_LNE', l, r); return; }
        if (EQ(k, 22)) { RkbBinop = Bin('TT_LLT', l, r); return; }
        if (EQ(k, 23)) { RkbBinop = Bin('TT_LLE', l, r); return; }
        if (EQ(k, 24)) { RkbBinop = Bin('TT_LGT', l, r); return; }
        RkbBinop = Bin('TT_LGE', l, r);
        return;
    }
    if (IDENT(lv, 'AND'))    { RkbBinop = RkLogicalAnd(l, r); return; }
    if (IDENT(lv, 'OR'))     { RkbBinop = (EQ(k, 0) Bin('TT_ALT', l, r), Call2((EQ(k, 1) '__rk_xor', EQ(k, 2) '__rk_max', '__rk_min'), l, r)); return; }
    if (IDENT(lv, 'POW'))    { RkbBinop = Bin('TT_POW', NCtx(l), NCtx(r)); return; }
    if (IDENT(lv, 'PAIR')) {
        if (IDENT(t(l), 'TT_VAR') EQ(n(l), 0) DIFFER(RkSlen[l])) { l = mk('TT_QLIT', v(l)); }
        pc = MakeCall('__rk_pair'); ad(pc, l); ad(pc, r);
        RkbBinop = pc;
        return;
    }
    RkbBinop = l;
    return;
}
function RkbPrefixApply(op, x, n) {
    if (IDENT(op, '-')) { RkbPrefixApply = mk1('TT_MNS', '', NCtx(x)); return; }
    if (IDENT(op, '+')) {
        n = NCtx(x);
        if (IDENT(n, x) ~(t(x) ? (POS(0) ('TT_ILIT' | 'TT_FLIT') RPOS(0)))) { n = Bin('TT_ADD', x, ilit(0)); }
        RkbPrefixApply = n;
        return;
    }
    if (op ? (POS(0) ('?' | 'so') RPOS(0))) { RkbPrefixApply = Call1('__rk_mkbool', x); return; }
    if (op ? (POS(0) ('!' | 'not') RPOS(0))) { RkbPrefixApply = mk1('TT_NOT', '', x); return; }
    if (IDENT(op, '~')) { RkbPrefixApply = Call1('__rk_str', x); return; }
    if (IDENT(op, '^')) { RkbPrefixApply = RkRangeEx(ilit(0), x); return; }
    RkbPrefixApply = x;
    return;
}
function RkArrRhs(rhs) {
    RkArrRhs = rhs;
    if (IDENT(rhs)) { return; }
    if (IDENT(t(rhs), 'TT_TO') GE(n(rhs), 2)) { RkArrRhs = Call2('__rk_range_arr', c(rhs)[1], c(rhs)[2]); return; }
    if (~RkOneScalar(rhs)) { return; }
    RkArrRhs = Call1('__rk_arr', rhs);
    return;
}
function RkOneScalar(e) {
    if (t(e) ? (POS(0) ('TT_QLIT' | 'TT_ILIT' | 'TT_FLIT' | 'TT_CAT' | 'TT_XREP' | 'TT_ADD' | 'TT_SUB' | 'TT_MUL' | 'TT_DIV' | 'TT_MOD' | 'TT_POW' | 'TT_MNS') RPOS(0))) { return; }
    if (IDENT(t(e), 'TT_VAR')) { if (v(e) ? (POS(0) ANY('@%'))) { freturn; } if (DIFFER(RkSlen[e])) { freturn; } return; }
    freturn;
}
function RkScalarRhs(r)          { RkScalarRhs = r; if (IDENT(t(r), 'TT_XREP') GE(n(r), 2)) { RkScalarRhs = Call2('__rk_rep', c(r)[1], c(r)[2]); } return; }
function RkbAssign(cls, nm, k, r, vv) {
    if (LT(k, 0)) { RkbAssign = Bin('TT_ASSIGN', VarNode(nm), (IDENT(cls, 'A') RkArrRhs(r), r)); return; }
    vv = VarNode(nm);
    RkbAssign = Bin('TT_ASSIGN', VarNode(nm), Bin((EQ(k, 0) 'TT_ADD', EQ(k, 1) 'TT_SUB', EQ(k, 2) 'TT_MUL', EQ(k, 3) 'TT_DIV', 'TT_CAT'), vv, r));
    return;
}
function RkbAssignOp(nm, op, r, lv) {
    if (~(lv = RkCompoundBase(op))) { freturn; }
    RkbAssignOp = Bin('TT_ASSIGN', VarNode(nm), RkScalarRhs(RkbBinop(lv, rk_cbk, VarNode(nm), r)));
    return;
}
function RkbTernary(l, mid, r)   { RkbTernary = mk3('TT_TERNARY', '', l, (DIFFER(mid) mid, mk('TT_NUL', '')), r); return; }
function RkArrIndex(arr, idx) {
    if (IDENT(t(idx), 'TT_TO') GE(n(idx), 2)) { RkArrIndex = ad(ad(ad(MakeCall('__rk_arr_slice'), VarNode(arr)), c(idx)[1]), c(idx)[2]); return; }
    RkArrIndex = mk2('TT_ARR_GET', '', VarNode(arr), idx);
    return;
}
function RkIncDec(nm, add)       { RkIncDec = QuietStore(VarNode(nm), Bin((EQ(add, 1) 'TT_ADD', 'TT_SUB'), VarNode(nm), ilit(1))); return; }
function RkPostIncDec(nm, add, tmp, q) {
    tmp = '__post_' RkPostUid; RkPostUid = RkPostUid + 1;
    q = mk('TT_SEQ_EXPR', '');
    ad(q, QuietStore(mk('TT_VAR', tmp), VarNode(nm)));
    ad(q, QuietStore(VarNode(nm), Bin((EQ(add, 1) 'TT_ADD', 'TT_SUB'), VarNode(nm), ilit(1))));
    ad(q, mk('TT_VAR', tmp));
    RkPostIncDec = q;
    return;
}
function RkTwField(nm)           { RkTwField = mk('TT_TWIGIL_FIELD', TwBare(nm)); return; }
function RkTwPostIncDec(nm, add, tmp, q) {
    tmp = '__twpost_' RkTwPostUid; RkTwPostUid = RkTwPostUid + 1;
    q = mk('TT_SEQ_EXPR', '');
    ad(q, Bin('TT_ASSIGN', mk('TT_VAR', tmp), RkTwField(nm)));
    ad(q, Bin('TT_ASSIGN', RkTwField(nm), Bin((EQ(add, 1) 'TT_ADD', 'TT_SUB'), RkTwField(nm), ilit(1))));
    ad(q, mk('TT_VAR', tmp));
    RkTwPostIncDec = q;
    return;
}
function RkIsElem(g)             { if (IDENT(g)) { freturn; } if ((t(g) ? (POS(0) ('TT_ARR_GET' | 'TT_HASH_GET') RPOS(0))) GE(n(g), 2)) { return; } freturn; }
function RkElemStore(g, val)     { RkElemStore = mk3((IDENT(t(g), 'TT_ARR_GET') 'TT_ARR_SET', 'TT_HASH_SET'), '', Clone(c(g)[1]), Clone(c(g)[2]), val); return; }
function RkbElemIncDec(g, add, post, tmp, q) {
    if (EQ(post, 1)) {
        q = mk('TT_SEQ_EXPR', '');
        tmp = '__post_' RkPostUid; RkPostUid = RkPostUid + 1;
        ad(q, QuietStore(mk('TT_VAR', tmp), Clone(g)));
        ad(q, RkElemStore(g, Bin((EQ(add, 1) 'TT_ADD', 'TT_SUB'), mk('TT_VAR', tmp), ilit(1))));
        ad(q, mk('TT_VAR', tmp));
        RkbElemIncDec = q;
        return;
    }
    RkbElemIncDec = RkElemStore(g, Bin((EQ(add, 1) 'TT_ADD', 'TT_SUB'), Clone(g), ilit(1)));
    return;
}
/* ==================================================================================================================== */
/* the expression walk: r_EXPR's RkX as the X_* locals; x_term, x_take, x_after, x_unary, the levels, x_list          */
/* ==================================================================================================================== */
function RkNewList(n)            { RkNewList = rklist(ARRAY('1:' (GT(n, 0) n, 1)), 0, 0, 0, '', '', '', '', '', 0); return; }
function RkListOperand(L, a, i) {
    if (EQ(ls_n(L), 1) EQ(ls_trailing(L), 0)) { RkListOperand = RkArrRhs(ElTree(ls_v(L)[1])); return; }
    a = MakeCall('__rk_arr');
    i = 0; while (i = LT(i, ls_n(L)) i + 1) { ad(a, ElTree(ls_v(L)[i])); }
    RkListOperand = a;
    return;
}
function RkBuildCross(r, L, op, cc, j, R, e, tm) {
    L = RkBuildList(c(r)[1]);
    op = v(c(r)[2]);
    cc = MakeCall((IDENT(SUBSTR(op, 1, 1), 'Z') '__rk_zip', '__rk_cross'));
    ad(cc, RkListOperand(L));
    j = 3;
    while (LE(j, n(r))) { R = RkBuildList(c(r)[j]); ad(cc, RkListOperand(R)); j = j + 2; }
    tm = NewTerm('TREE'); tk_t(tm) = cc;
    e = rkel(cc, 1, tm, '', '', '', '');
    ls_v(L)[1] = e; ls_n(L) = 1; ls_nitem(L) = 1; ls_op1(L) = ''; ls_trailing(L) = 0;
    ls_rglo(L) = ''; ls_rghi(L) = ''; ls_rgop(L) = ''; ls_rgnitem(L) = 0; ls_t1(L) = '';
    RkBuildCross = L;
    return;
}
function RkBuildList(r, X_L, X_el, X_nterm, X_nops, X_elnops, X_eli, X_lastcls, X_lastname, X_pend, X_star, i, e, last) {
    if (IDENT(t(r), 'R_CROSS')) { RkBuildList = RkBuildCross(r); return; }
    X_L = RkNewList(n(r)); X_nops = 0; X_pend = ; X_star = RkStarNext; RkStarNext = ;
    i = 0;
    while (i = LT(i, n(r)) i + 1) {
        X_eli = i - 1;
        e = c(r)[i];
        X_el = rkel('', 0, '', '', '', '', '');
        ls_n(X_L) = ls_n(X_L) + 1;
        ls_v(X_L)[ls_n(X_L)] = X_el;
        X_nterm = 0; X_elnops = X_nops;
        RkBuildEl(e);
        el_nitem(X_el) = X_nterm;
    }
    last = ls_v(X_L)[ls_n(X_L)];
    if (GT(ls_n(X_L), 0) EQ(el_nitem(last), 1) IDENT(tk_kind(el_t0(last)), 'EMPTY')) {
        ls_n(X_L) = ls_n(X_L) - 1;
        ls_trailing(X_L) = (GT(ls_nitem(X_L), 1) 1, 0);
    }
    RkBuildList = X_L;
    return;
}
function RkBuildEl(e, j) {
    if (IDENT(t(c(e)[1]), 'R_EMPTY')) { XTerm(NewTerm('EMPTY')); el_t(X_el) = mk('TT_NUL', ''); return; }
    el_t(X_el) = BX(c(e)[1]);
    j = 2;
    while (LE(j, n(e))) { XTake(v(c(e)[j])); XAfter(c(e)[j + 1]); j = j + 2; }
    return;
}
function XTerm(tm) {
    if (EQ(X_nterm, 0)) { el_t0(X_el) = tm; }
    X_nterm = X_nterm + 1;
    ls_nitem(X_L) = ls_nitem(X_L) + 1;
    if (EQ(ls_nitem(X_L), 2)) { ls_t1(X_L) = tm; }
    return;
}
function XTake(op) {
    X_nops = X_nops + 1;
    if (EQ(X_nops, 1)) { ls_op1(X_L) = op; }
    if (EQ(X_nops - X_elnops, 1) ~IDENT(op, ',')) { el_op1(X_el) = op; }
    return;
}
function XAfter(nd, cap, r)      { cap = EQ(X_nops - X_elnops, 1) 1; r = BX(nd); if (DIFFER(cap)) { el_rest(X_el) = r; } XAfter = r; return; }
function BX(nd, tt) {
    tt = t(nd);
    if (IDENT(tt, 'R_TERMISH')) { BX = XUnary(nd, ''); return; }
    if (IDENT(tt, 'R_BIN')) { BX = XBin(nd); return; }
    if (IDENT(tt, 'R_TERN')) { BX = XTern(nd); return; }
    if (IDENT(tt, 'R_LOOSE')) { BX = RkbPrefixApply(v(c(nd)[1]), BX(c(nd)[2])); X_lastcls = ''; return; }
    BX = mk('TT_NUL', '');
    return;
}
/* x_unary: the term (x_term), the ++/-- prefixes on a variable, a pending declaration, ** (pow), then the prefixes    */
function XUnary(nd, pow, tm, base, np, pre, i, r, top) {
    if (DIFFER(X_pend)) { tm = X_pend; X_pend = ; } else { tm = RkBuildTermish(nd); XTerm(tm); }
    np = tk_npre(tm); pre = tk_pre(tm);
    X_lastcls = '';
    if (GT(np, 0) (pre[np] ? (POS(0) ('++' | '--') RPOS(0))) IDENT(tk_kind(tm), 'VAR') IDENT(tk_cls(tm), 'S') EQ(tk_npost(tm), 0)) {
        base = RkIncDec(tk_name(tm), (IDENT(pre[np], '++') 1, 0)); np = np - 1;
    } else {
        if (GT(np, 0) (pre[np] ? (POS(0) ('++' | '--') RPOS(0))) IDENT(tk_kind(tm), 'VAR') GT(tk_npost(tm), 0) RkIsElem(tk_t(tm))) {
            base = RkbElemIncDec(tk_t(tm), (IDENT(pre[np], '++') 1, 0), 0); np = np - 1;
        } else {
            if (IDENT(tk_kind(tm), 'DECL') IDENT(tk_t(tm))) { base = mk('TT_NUL', ''); el_pdph(X_el) = base; el_pdd(X_el) = tk_decl(tm); }
            else { base = (DIFFER(tk_t(tm)) tk_t(tm), mk('TT_NUL', '')); }
        }
    }
    if (IDENT(tk_kind(tm), 'VAR') EQ(tk_npre(tm), 0) EQ(tk_npost(tm), 0)) { X_lastcls = tk_cls(tm); X_lastname = tk_name(tm); }
    if (DIFFER(pow)) { XTake(v(c(pow)[2])); r = BX(c(pow)[3]); base = RkbBinop('POW', 0, base, r); X_lastcls = ''; }
    i = np + 1;
    while (i = GT(i, 1) i - 1) { base = RkbPrefixApply(pre[i], base); }
    XUnary = base;
    RkLastTm = tm;
    return;
}
function XBin(nd, lv, op, l, r, k, top, cls, nm, eq, first) {
    lv = v(nd); op = v(c(nd)[2]);
    if (IDENT(lv, 'POW')) { XBin = XUnary(c(nd)[1], nd); return; }
    if (IDENT(lv, 'ASG')) {
        l = XUnary(c(nd)[1], '');
        cls = X_lastcls; nm = X_lastname;
        XTake(op);
        r = XAfter(c(nd)[3]);
        eq = IDENT(op, '=') 1;
        if (DIFFER(eq)) { XBin = RkbAssign(cls, nm, -1, r); return; }
        if (DIFFER(RkLv['COMPOUND'][op])) { XBin = RkbAssign(cls, nm, RkLv['COMPOUND'][op], r); return; }
        XBin = RkbAssignOp(nm, op, r);
        return;
    }
    if (IDENT(lv, 'PAIR')) { l = BX(c(nd)[1]); XTake(op); r = XAfter(c(nd)[3]); XBin = RkbBinop('PAIR', 0, l, r); X_lastcls = ''; return; }
    if (IDENT(lv, 'SMART')) { XBin = XSmart(nd); return; }
    if (IDENT(lv, 'ADDSUB') DIFFER(X_star) EQ(X_nterm, 0) EQ(X_eli, 0) (op ? (POS(0) ANY('-+') RPOS(0))) RkIsBareStar(c(nd)[1])) {
        XTerm(RkBuildTermish(c(nd)[1])); XTake(op); XAfter(c(nd)[3]); X_lastcls = '';
        XBin = mk('TT_NUL', '');
        return;
    }
    top = (EQ(X_nterm, 0) EQ(X_eli, 0)) 1;
    l = BX(c(nd)[1]);
    XTake(op);
    r = BX(c(nd)[3]);
    k = RkLv[lv][op];
    if (IDENT(lv, 'RANGE1') DIFFER(top)) { ls_rglo(X_L) = l; ls_rghi(X_L) = r; ls_rgop(X_L) = op; ls_rgnitem(X_L) = ls_nitem(X_L); }
    XBin = RkbBinop(lv, k, l, r);
    X_lastcls = '';
    return;
}
function RkIsBareStar(nd)        { if (IDENT(t(nd), 'R_TERMISH') EQ(n(c(nd)[1]), 0) EQ(n(c(nd)[2]), 1) IDENT(t(c(c(nd)[2])[1]), 'R_STAR')) { return; } freturn; }
function XFirstTermish(nd)       { while (IDENT(t(nd), 'R_BIN')) { nd = c(nd)[1]; } XFirstTermish = nd; return; }
function XSmart(nd, l, rhs, first, tm, m) {
    l = BX(c(nd)[1]);
    XTake('~~');
    X_lastcls = '';
    rhs = c(nd)[3];
    first = XFirstTermish(rhs);
    if (~IDENT(t(first), 'R_TERMISH')) { XSmart = Call2('__rk_smartmatch', l, BX(rhs)); return; }
    tm = RkBuildTermish(first);
    XTerm(tm);
    if (m = RkbSmartmatchTerm(l, tm)) { XSmart = m; return; }
    X_pend = tm;
    XSmart = Call2('__rk_smartmatch', l, BX(rhs));
    return;
}
function RkbSmartmatchTerm(l, x, nm, mc, raw, kind, body, pat, rep, g, i) {
    if (IDENT(tk_kind(x), 'QUOTE') EQ(tk_npost(x), 0) EQ(tk_npre(x), 0)) {
        raw = tk_ctail(x); kind = '';
        if (raw ? (POS(0) '/')) { kind = 'match'; body = RkRegexToEngine(RkRegexBody(SUBSTR(raw, 2), '/')); }
        if (raw ? (POS(0) 'm:g/')) { kind = 'match_global'; body = RkRegexToEngine(RkRegexBody(SUBSTR(raw, 5), '/')); }
        if (raw ? (POS(0) 's/')) {
            kind = 'subst';
            pat = RkRegexToEngine(RkRegexBody(SUBSTR(raw, 3), '/'));
            raw ? (POS(0) 's/' (ARB NOTANY('\')) '/' REM . rep);
            g = (raw ? ('g' RPOS(0))) 'g';
            body = pat CHAR(1) RkRegexBody(rep, '/') CHAR(1) (DIFFER(g) 'g', '-');
        }
        if (DIFFER(kind)) { RkbSmartmatchTerm = mk3('TT_SMATCH', '', l, mk('TT_QLIT', body), mk('TT_QLIT', kind)); return; }
    }
    if ((IDENT(tk_kind(x), 'NAME'), (IDENT(tk_kind(x), 'CALL') IDENT(t(tk_t(x)), 'TT_VAR')))) {
        if (EQ(tk_npost(x), 0) EQ(tk_npre(x), 0)) { RkbSmartmatchTerm = mk3('TT_METHCALL', '', l, mk('TT_QLIT', 'does'), mk('TT_QLIT', tk_name(x))); return; }
    }
    freturn;
}
function XTern(nd, l, mid, r) {
    l = BX(c(nd)[1]);
    mid = RkbExpr(RkBuildList(c(nd)[2]));
    XTake('??');
    r = BX(c(nd)[3]);
    X_lastcls = '';
    XTern = RkbTernary(l, mid, r);
    return;
}
/* ==================================================================================================================== */
/* RkList consumers: rkb_expr, el_tree, el_rest, rkb_paren, rkb_bracket, arglist, named_form, pos_arg               */
/* ==================================================================================================================== */
function ElFill(e, d, t2) {
    if (IDENT(el_pdd(e))) { return; }
    d = el_pdd(e); el_pdd(e) = '';
    t2 = RkBuildDecl(d, '', 0);
    t(el_pdph(e)) = t(t2); v(el_pdph(e)) = v(t2); n(el_pdph(e)) = n(t2); c(el_pdph(e)) = c(t2);
    return;
}
function ElTree(e)               { ElFill(e); ElTree = (DIFFER(el_t(e)) el_t(e), mk('TT_NUL', '')); return; }
function ElRest(e)               { ElFill(e); ElRest = el_rest(e); return; }
function RkbExpr(L)              { if (IDENT(L)) { RkbExpr = mk('TT_NUL', ''); return; } if (EQ(ls_n(L), 0)) { RkbExpr = mk('TT_NUL', ''); return; } RkbExpr = ElTree(ls_v(L)[1]); return; }
function RkbParen(L, nst, call, i) {
    if (IDENT(L)) { RkbParen = MakeCall('__rk_arr'); return; }
    if (EQ(ls_nitem(L), 0)) { RkbParen = MakeCall('__rk_arr'); return; }
    if (EQ(ls_n(L), 1) EQ(ls_trailing(L), 0)) { RkbParen = ElTree(ls_v(L)[1]); return; }
    call = MakeCall('__rk_arr');
    i = 0;
    while (i = LT(i, ls_n(L)) i + 1) { ad(call, ElTree(ls_v(L)[i])); }
    RkbParen = call;
    return;
}
function RkbBracket(L, call, i) {
    call = MakeCall('__rk_arr_lit');
    if (DIFFER(L)) { i = 0; while (i = LT(i, ls_n(L)) i + 1) { ad(call, ElTree(ls_v(L)[i])); } }
    RkbBracket = call;
    return;
}
function NamedForm(tm, first) {
    if (IDENT(tm)) { freturn; }
    if ((GT(tk_npre(tm), 0), GT(tk_npost(tm), 0))) { freturn; }
    if (IDENT(tk_kind(tm), 'FAT')) { return; }
    if (IDENT(tk_kind(tm), 'CP') IDENT(tk_ck(tm), 'v') IDENT(tk_cnt(tm), 'P')) { return; }
    if (IDENT(tk_kind(tm), 'CP') IDENT(tk_ck(tm), 'n') EQ(first, 1)) { return; }
    freturn;
}
function AddNamed(tm, named) {
    Append(named, mk('TT_QLIT', tk_name(tm)));
    if (IDENT(tk_kind(tm), 'CP') IDENT(tk_ck(tm), 'n')) { Append(named, MkBool(1)); return; }
    Append(named, tk_val(tm));
    return;
}
function PosArg(e, tm) {
    if (EQ(el_nitem(e), 1)) {
        tm = el_t0(e);
        if (IDENT(tk_kind(tm), 'CP') EQ(tk_npre(tm), 0) EQ(tk_npost(tm), 0)) {
            if (IDENT(tk_ck(tm), 'n')) { PosArg = MkBool(1); return; }
            if (IDENT(tk_ck(tm), '!')) { PosArg = MkBool(0); return; }
            if (IDENT(tk_ck(tm), 'v') IDENT(tk_cnt(tm), 'P')) { PosArg = (DIFFER(tk_val(tm)) tk_val(tm), MakeCall('__rk_undef')); return; }
            if (IDENT(tk_ck(tm), 'v') IDENT(tk_cnt(tm), 'W')) { PosArg = tk_val(tm); return; }
        }
    }
    PosArg = ElTree(e);
    return;
}
/* arglist(ctx): positional arguments into pos, named (pair-shaped, first or after a named one) into named          */
function ArgList(L, ctx, pos, named, innamed, i, e, can) {
    innamed = 0;
    if (IDENT(L)) { ArgList = 0; return; }
    i = 0;
    while (i = LT(i, ls_n(L)) i + 1) {
        e = ls_v(L)[i];
        can = (EQ(ctx, 1) 1, (((EQ(ctx, 2), EQ(ctx, 3))) ((EQ(i, 1), EQ(innamed, 1)))) 1, 0);
        if (DIFFER(named) EQ(can, 1) EQ(el_nitem(e), 1) NamedForm(el_t0(e), (EQ(innamed, 0) 1, 0))) { innamed = 1; AddNamed(el_t0(e), named); continue; }
        Append(pos, PosArg(e));
    }
    ArgList = innamed;
    return;
}
/* ==================================================================================================================== */
/* r_termish's term and its postfixes: the term builders by kind, rkb_postfix, method_call, rkb_call, rkb_colonpair   */
/* ==================================================================================================================== */
function RkBuildTermish(nd, pre, tp, tm, i, np, arr) {
    pre = c(nd)[1]; tp = c(nd)[2];
    tm = RkBuildTerm(c(tp)[1]);
    np = n(pre);
    arr = ARRAY('1:' (GT(np, 0) np, 1));
    i = 0;
    while (i = LT(i, np) i + 1) { arr[i] = RkTrim(v(c(pre)[i])); }
    tk_npre(tm) = np; tk_pre(tm) = arr;
    i = 1;
    while (i = LT(i, n(tp)) i + 1) { RkbPostfix(tm, c(tp)[i]); }
    RkBuildTermish = tm;
    return;
}
function RkListOrNull(x)         { RkListOrNull = ; if (IDENT(x)) { return; } if ((IDENT(t(x), 'R_LIST'), IDENT(t(x), 'R_CROSS'))) { RkListOrNull = RkBuildList(x); } return; }
function RkSemiList(s)           { RkSemiList = ; if (GT(n(s), 0)) { RkSemiList = RkBuildList(c(s)[1]); } return; }
function RkBuildTerm(nd, tt, tm, L, nm, x) {
    tt = t(nd);
    if (IDENT(tt, 'R_VAR')) { RkBuildTerm = RkbVar(v(nd)); return; }
    if (IDENT(tt, 'R_SELF')) { tm = RkbVar('self'); tk_cls(tm) = 'S'; RkBuildTerm = tm; return; }
    if (IDENT(tt, 'R_NUM')) { RkBuildTerm = RkbNumber(v(nd)); return; }
    if (IDENT(tt, 'R_QUOTE')) { RkBuildTerm = RkbQuoteTerm(nd); return; }
    if (IDENT(tt, 'R_WORDS')) { RkBuildTerm = RkbWords(v(nd)); return; }
    if (IDENT(tt, 'R_NAME')) { RkBuildTerm = RkbName(RkNamePart(v(nd))); return; }
    if (IDENT(tt, 'R_CALL')) { RkBuildTerm = RkbCall(v(nd), RkListOrNull(c(nd)[2]), v(c(nd)[1])); return; }
    if (IDENT(tt, 'R_CP')) { RkBuildTerm = RkbColonpair(nd); return; }
    if (IDENT(tt, 'R_FAT')) { RkBuildTerm = RkbFatarrow(v(c(nd)[1]), RkBuildList(c(nd)[2])); return; }
    if (IDENT(tt, 'R_STAR')) { tm = RkbName(v(nd)); tk_kind(tm) = 'STAR'; RkBuildTerm = tm; return; }
    if (IDENT(tt, 'R_YADA')) { tm = NewTerm('TREE'); tk_t(tm) = mk('TT_YADA', ''); RkBuildTerm = tm; return; }
    if (IDENT(tt, 'R_PAREN')) { tm = NewTerm('PAREN'); tk_cnt(tm) = v(nd); tk_in(tm) = RkSemiList(nd); tk_t(tm) = RkbParen(tk_in(tm), v(nd)); RkBuildTerm = tm; return; }
    if (IDENT(tt, 'R_BRACKET')) { tm = NewTerm('TREE'); tk_t(tm) = RkbBracket(RkSemiList(nd)); RkBuildTerm = tm; return; }
    if (IDENT(tt, 'R_BLOCK')) { RkBuildTerm = RkbBlockTerm(c(nd)[1], c(nd)[2]); return; }
    if (IDENT(tt, 'R_DOTTY')) { RkBuildTerm = RkbMethodTerm(c(nd)[1]); return; }
    if (IDENT(tt, 'R_SCOPED')) { RkBuildTerm = RkBuildScoped(nd); return; }
    if (IDENT(tt, 'R_MULTI')) { RkBuildTerm = RkBuildMulti(nd); return; }
    if (IDENT(tt, 'R_ROUTINE')) { RkBuildTerm = RkBuildRoutine(nd, 0); return; }
    if (IDENT(tt, 'R_PACKAGE')) { RkBuildTerm = RkBuildPackage(nd); return; }
    if (IDENT(tt, 'R_REGEX')) { RkBuildTerm = RkBuildRegex(nd); return; }
    if (IDENT(tt, 'R_ENUM')) { tm = NewTerm('TREE'); tk_t(tm) = RkbEnum(v(nd)); RkBuildTerm = tm; return; }
    if (IDENT(tt, 'R_ENUMX')) { tm = NewTerm('TREE'); tk_t(tm) = mk('TT_SEQ_EXPR', ''); RkBuildTerm = tm; return; }
    if (IDENT(tt, 'R_SUBSET')) { tm = NewTerm('TREE'); tk_t(tm) = mk('TT_NUL', ''); RkBuildTerm = tm; return; }
    if (IDENT(tt, 'R_CONST')) { tm = NewTerm('TREE'); tk_t(tm) = Bin('TT_ASSIGN', VarNode(v(c(nd)[1])), RkbExpr(RkBuildList(c(nd)[3]))); RkBuildTerm = tm; return; }
    if (IDENT(tt, 'R_SPREFIX')) { RkBuildTerm = RkBuildSprefix(nd); return; }
    if (IDENT(tt, 'R_REDUCE')) { RkBuildTerm = RkbReduce(v(nd), RkListOrNull(c(nd)[2])); return; }
    tm = NewTerm('EMPTY');
    RkBuildTerm = tm;
    return;
}
/* ==================================================================================================================== */
function RkbColonpair(nd, it, ck, vk, val, key, vv) {
    it = NewTerm('CP');
    key = v(nd); ck = v(c(nd)[1]); vk = v(c(nd)[2]); val = c(nd)[3];
    tk_ck(it) = ck; tk_cnt(it) = vk; tk_name(it) = key;
    if (IDENT(ck, '$')) {
        vv = RkbVar(key);
        key ? (POS(0) ANY('$@%&') FENCE(ANY('.!^:*?=~') | epsilon) REM . key);
        tk_name(it) = key; tk_val(it) = tk_t(vv);
    }
    if (IDENT(vk, 'W')) { tk_val(it) = mk('TT_QLIT', RkTrim(v(val))); }
    if (IDENT(vk, 'P')) { if (GT(v(val), 0)) { tk_val(it) = RkbParen(RkSemiList(val), v(val)); } }
    if (IDENT(vk, 'B')) { tk_val(it) = RkbBracket(RkSemiList(val)); }
    tk_t(it) = MkBool((IDENT(ck, '!') 0, 1));
    RkbColonpair = it;
    return;
}
function RkbFatarrow(key, L, it) {
    it = NewTerm('FAT');
    tk_name(it) = key;
    tk_val(it) = RkbExpr(L);
    tk_t(it) = ad(ad(MakeCall('__rk_pair'), mk('TT_QLIT', key)), tk_val(it));
    RkbFatarrow = it;
    return;
}
function RkbReduce(op, L, it, l, rop) {
    it = NewTerm('TREE');
    rop = (IDENT(op, '+') '__rk_reduce_add', IDENT(op, '-') '__rk_reduce_sub', IDENT(op, '*') '__rk_reduce_mul', IDENT(op, '~') '__rk_reduce_cat',
           IDENT(op, 'min') '__rk_reduce_min', '__rk_reduce_max');
    if (IDENT(L)) { tk_t(it) = MakeCall(rop); RkbReduce = it; return; }
    if (EQ(ls_nitem(L), 0)) { tk_t(it) = MakeCall(rop); RkbReduce = it; return; }
    l = RkbParen(L, 0);
    if (IDENT(t(l), 'TT_TO') GE(n(l), 2)) { l = Call2('__rk_range_arr', c(l)[1], c(l)[2]); }
    tk_t(it) = Call1(rop, l);
    RkbReduce = it;
    return;
}
/* the postfix record (RkPf) from its raw node: k, txt, inner, args, form, mod, hyper, adjacency                     */
function RkPfOf(pf, tt) {
    tt = t(pf);
    rk_pfk = ''; rk_pftxt = ''; rk_pfin = ''; rk_pfargs = ''; rk_pfform = 0; rk_pfmod = ''; rk_pfhyper = 0; rk_pfadj = 0;
    if (IDENT(tt, 'R_PF_HYPER')) { RkPfOf(c(pf)[1]); rk_pfhyper = 1; return; }
    if (IDENT(tt, 'R_PF_OP')) { rk_pfk = 'P'; rk_pftxt = v(pf); return; }
    if (IDENT(tt, 'R_PF_IDX')) { rk_pfk = '['; rk_pfadj = v(pf); rk_pfin = RkSemiList(c(pf)[1]); return; }
    if (IDENT(tt, 'R_PF_HASH')) { rk_pfk = '{'; rk_pfadj = v(pf); rk_pfin = RkSemiList(c(pf)[1]); return; }
    if (IDENT(tt, 'R_PF_ANGW')) { rk_pfk = '<'; rk_pfadj = v(pf); rk_pftxt = v(c(pf)[1]); return; }
    if (IDENT(tt, 'R_PF_CALL')) { rk_pfk = '('; rk_pfadj = v(pf); rk_pfargs = RkListOrNull(c(pf)[1]); rk_pfform = 1; return; }
    if (IDENT(tt, 'R_PF_M')) { rk_pfk = 'M'; rk_pftxt = v(pf); rk_pfform = v(c(pf)[1]); rk_pfargs = RkListOrNull(c(pf)[2]); rk_pfmod = v(c(pf)[3]); return; }
    return;
}
function RkMethodCall(inv, nm, cc, pos, named, i) {
    nm = RkTrim(rk_pftxt);
    if (IDENT(rk_pfmod, '^')) { nm = '^' nm; }
    cc = mk2('TT_METHCALL', '', inv, mk('TT_QLIT', nm));
    if (DIFFER(rk_pfargs)) {
        pos = mk('TL', ''); named = mk('TL', '');
        ArgList(rk_pfargs, (EQ(rk_pfform, 2) 4, 2), pos, named);
        i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); }
        i = 0; while (i = LT(i, n(named)) i + 1) { ad(cc, c(named)[i]); }
    }
    RkMethodCall = cc;
    return;
}
function RkbMethodTerm(pf, it, rk_pfk, rk_pftxt, rk_pfin, rk_pfargs, rk_pfform, rk_pfmod, rk_pfhyper, rk_pfadj) {
    RkPfOf(pf);
    it = NewTerm('TREE');
    if (IDENT(rk_pfk, 'M')) { tk_kind(it) = 'DOTTY'; tk_name(it) = rk_pftxt; tk_t(it) = RkMethodCall(VarNode('$_')); RkbMethodTerm = it; return; }
    tk_t(it) = mk('TT_NUL', '');
    RkbMethodTerm = it;
    return;
}
function RkIsStar(x)             { if (IDENT(t(x), 'TT_VAR') IDENT(v(x), '*')) { return; } freturn; }
function RkPureBase(x, i) {
    if (IDENT(x)) { freturn; }
    if (t(x) ? (POS(0) ('TT_VAR' | 'TT_ILIT' | 'TT_QLIT') RPOS(0))) { return; }
    if (~(IDENT(t(x), 'TT_FNC') IDENT(v(x), '__rk_arr'))) { freturn; }
    i = 1; while (i = LT(i, n(x)) i + 1) { if (~RkPureBase(c(x)[i])) { freturn; } }
    return;
}
function RkStarElems(idx, base, i) {
    RkStarElems = idx;
    if (IDENT(idx)) { return; }
    if (~RkPureBase(base)) { return; }
    if (RkIsStar(idx)) { RkStarElems = mk2('TT_METHCALL', '', Clone(base), mk('TT_QLIT', 'elems')); return; }
    i = 0; while (i = LT(i, n(idx)) i + 1) { c(idx)[i] = RkStarElems(c(idx)[i], base); }
    return;
}
function RkArrAll(nm, el)        { el = mk2('TT_METHCALL', '', VarNode(nm), mk('TT_QLIT', 'elems')); RkArrAll = ad(ad(ad(MakeCall('__rk_arr_slice'), VarNode(nm)), ilit(0)), RkDec(el)); return; }
function RkArrEndIndex(nm, off, k, el) { el = mk2('TT_METHCALL', '', VarNode(nm), mk('TT_QLIT', 'elems')); RkArrEndIndex = mk2('TT_ARR_GET', '', VarNode(nm), Bin(k, el, off)); return; }
function RkbPostfix(it, pf, first, adj, e, nm, cls, xs, x0, call, i, key, cc, pos, named, hn, h, inv, rk_pfk, rk_pftxt, rk_pfin, rk_pfargs, rk_pfform, rk_pfmod, rk_pfhyper, rk_pfadj) {
    if (EQ(tk_npost(it), 0) IDENT(tk_kind(it), 'VAR') IDENT(tk_cls(it), 'A') IDENT(t(pf), 'R_PF_IDX')) { RkStarNext = 1; }
    RkPfOf(pf);
    RkStarNext = ;
    first = EQ(tk_npost(it), 0) 1;
    adj = (DIFFER(first) EQ(rk_pfadj, 1)) 1;
    e = tk_t(it);
    tk_npost(it) = tk_npost(it) + 1;
    if (IDENT(first) IDENT(rk_pfk, 'P') (rk_pftxt ? (POS(0) ('++' | '--') RPOS(0))) RkIsElem(e)) { tk_t(it) = RkbElemIncDec(e, (IDENT(rk_pftxt, '++') 1, 0), 1); return; }
    if (DIFFER(first) IDENT(tk_kind(it), 'VAR')) {
        nm = tk_name(it); cls = tk_cls(it);
        if (IDENT(cls, 'A') IDENT(rk_pfk, '[')) {
            xs = rk_pfin;
            if (DIFFER(xs)) { if (GT(ls_n(xs), 0)) {
                x0 = el_t0(ls_v(xs)[1]);
                if (EQ(ls_nitem(xs), 1) IDENT(tk_kind(x0), 'STAR') EQ(tk_npost(x0), 0)) { tk_t(it) = RkArrAll(nm); return; }
                if (EQ(ls_n(xs), 1) GE(ls_nitem(xs), 2) IDENT(tk_kind(x0), 'STAR') EQ(tk_npost(x0), 0) (ls_op1(xs) ? (POS(0) ANY('-+') RPOS(0)))) {
                    tk_t(it) = RkArrEndIndex(nm, ElRest(ls_v(xs)[1]), (IDENT(ls_op1(xs), '-') 'TT_SUB', 'TT_ADD')); return;
                }
            } }
            if (DIFFER(xs)) { if (GT(ls_n(xs), 1)) {
                call = ad(MakeCall('__rk_arr_pick'), VarNode(nm));
                i = 0; while (i = LT(i, ls_n(xs)) i + 1) { ad(call, ElTree(ls_v(xs)[i])); }
                tk_t(it) = call; return;
            } }
            tk_t(it) = RkArrIndex(nm, RkStarElems(RkbExpr(xs), VarNode(nm)));
            return;
        }
        if (((IDENT(cls, 'H'), (IDENT(cls, 'S') DIFFER(adj)))) (rk_pfk ? (POS(0) ANY('{<') RPOS(0)))) {
            key = (IDENT(rk_pfk, '<') mk('TT_QLIT', RkTrim(rk_pftxt)), RkbExpr(rk_pfin));
            tk_t(it) = mk2('TT_HASH_GET', '', VarNode(nm), key);
            return;
        }
        if (IDENT(cls, 'S') DIFFER(adj) IDENT(rk_pfk, '[')) { tk_t(it) = RkArrIndex(nm, RkStarElems(RkbExpr(rk_pfin), VarNode(nm))); return; }
        if (IDENT(cls, 'S') IDENT(rk_pfk, '(')) {
            cc = mk1('TT_INVOKE', '', VarNode(nm));
            pos = mk('TL', ''); ArgList(rk_pfargs, 0, pos, '');
            i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); }
            tk_t(it) = cc; return;
        }
        if ((cls ? (POS(0) ANY('ST') RPOS(0))) IDENT(rk_pfk, 'P') (rk_pftxt ? (POS(0) ('++' | '--') RPOS(0)))) {
            if (IDENT(cls, 'S')) { tk_t(it) = RkPostIncDec(nm, (IDENT(rk_pftxt, '++') 1, 0)); }
            else { tk_t(it) = RkTwPostIncDec(SUBSTR(nm, 2), (IDENT(rk_pftxt, '++') 1, 0)); }
            return;
        }
    }
    if (IDENT(rk_pfk, 'M')) {
        nm = RkTrim(rk_pftxt);
        if (DIFFER(first) IDENT(rk_pfmod) IDENT(nm, 'new') ((IDENT(tk_kind(it), 'NAME'), (IDENT(tk_kind(it), 'CALL') IDENT(t(e), 'TT_VAR'))))) {
            pos = mk('TL', ''); named = mk('TL', '');
            hn = (DIFFER(rk_pfargs) ArgList(rk_pfargs, 3, pos, named), 0);
            cc = mk1('TT_NEW', '', mk('TT_QLIT', tk_name(it)));
            i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); }
            i = 0; while (i = LT(i, n(named)) i + 1) { ad(cc, c(named)[i]); }
            tk_t(it) = cc; return;
        }
        tk_t(it) = RkMethodCall(e);
        if (EQ(rk_pfhyper, 1) IDENT(rk_pfmod)) {
            h = MakeCall('__rk_hyper_meth'); inv = c(tk_t(it))[1];
            if (IDENT(t(inv), 'TT_TO') GE(n(inv), 2)) { inv = Call2('__rk_range_arr', c(inv)[1], c(inv)[2]); }
            ad(h, inv);
            i = 1; while (i = LT(i, n(tk_t(it))) i + 1) { ad(h, c(tk_t(it))[i]); }
            tk_t(it) = h;
        }
        return;
    }
    if (IDENT(rk_pfk, '[')) {
        xs = rk_pfin;
        if (DIFFER(xs)) { if (GT(ls_n(xs), 1)) {
            call = ad(MakeCall('__rk_arr_pick'), e);
            i = 0; while (i = LT(i, ls_n(xs)) i + 1) { ad(call, ElTree(ls_v(xs)[i])); }
            tk_t(it) = call; return;
        } }
        key = RkStarElems(RkbExpr(xs), e);
        if (IDENT(t(key), 'TT_TO') GE(n(key), 2)) { tk_t(it) = ad(ad(ad(MakeCall('__rk_arr_slice'), e), c(key)[1]), c(key)[2]); return; }
        tk_t(it) = mk2('TT_ARR_GET', '', e, key);
        return;
    }
    if (rk_pfk ? (POS(0) ANY('{<') RPOS(0))) {
        key = (IDENT(rk_pfk, '<') mk('TT_QLIT', RkTrim(rk_pftxt)), RkbExpr(rk_pfin));
        tk_t(it) = mk2('TT_HASH_GET', '', e, key);
        return;
    }
    if (IDENT(rk_pfk, '(')) {
        cc = mk1('TT_INVOKE', '', e);
        pos = mk('TL', ''); ArgList(rk_pfargs, 0, pos, '');
        i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); }
        tk_t(it) = cc;
        return;
    }
    return;
}
/* ==================================================================================================================== */
/* rkb_call: the builtins with their own nodes (say print take return fail exit die flat await defined join map grep */
/* sort reverse exists delete), the test routines, then a plain or named call                                        */
/* ==================================================================================================================== */
RkTestRt = TABLE(47);
RkTestRt['plan'] = '__rk_test_plan'; RkTestRt['ok'] = '__rk_test_ok'; RkTestRt['nok'] = '__rk_test_nok'; RkTestRt['is'] = '__rk_test_is';
RkTestRt['isnt'] = '__rk_test_isnt'; RkTestRt['done-testing'] = '__rk_test_done'; RkTestRt['skip-rest'] = '__rk_test_skip_rest';
RkTestRt['skip'] = '__rk_test_skip'; RkTestRt['todo'] = '__rk_test_todo'; RkTestRt['diag'] = '__rk_test_diag'; RkTestRt['pass'] = '__rk_test_pass';
RkTestRt['flunk'] = '__rk_test_flunk'; RkTestRt['subtest'] = '__rk_test_subtest'; RkTestRt['is-deeply'] = '__rk_test_is_deeply';
RkTestRt['is-approx'] = '__rk_test_is_approx'; RkTestRt['isa-ok'] = '__rk_test_isa_ok'; RkTestRt['does-ok'] = '__rk_test_does_ok';
RkTestRt['cmp-ok'] = '__rk_test_cmp_ok'; RkTestRt['lives-ok'] = '__rk_test_lives_ok'; RkTestRt['dies-ok'] = '__rk_test_dies_ok';
RkTestRt['throws-like'] = '__rk_test_throws_like'; RkTestRt['eval-lives-ok'] = '__rk_test_eval_lives_ok'; RkTestRt['eval-dies-ok'] = '__rk_test_eval_dies_ok';
function RkNamedCall(fname, pos, named, cc, i) {
    cc = MakeCall('__rk_named_call');
    ad(cc, mk('TT_QLIT', fname));
    ad(cc, ilit((DIFFER(pos) n(pos), 0)));
    if (DIFFER(pos)) { i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); } }
    i = 0; while (i = LT(i, n(named)) i + 1) { ad(cc, c(named)[i]); }
    RkNamedCall = cc;
    return;
}
function RkFlattenParenArgs(L, pos, a, i) {
    if (~EQ(n(pos), 1)) { return; }
    if (IDENT(L)) { return; }
    if (~(EQ(ls_nitem(L), 1) EQ(ls_n(L), 1))) { return; }
    if (~(IDENT(tk_kind(el_t0(ls_v(L)[1])), 'PAREN') EQ(tk_npost(el_t0(ls_v(L)[1])), 0) EQ(tk_npre(el_t0(ls_v(L)[1])), 0))) { return; }
    a = c(pos)[1];
    if (~(IDENT(t(a), 'TT_FNC') IDENT(v(a), '__rk_arr'))) { return; }
    n(pos) = 0; c(pos) = '';
    i = 1; while (i = LT(i, n(a)) i + 1) { ad(pos, c(a)[i]); }
    return;
}
function RkbCall(nm, args, form, it, rt, pos, named, cc, i, k, lo, f0, lst, m, a, hn) {
    it = NewTerm('CALL');
    tk_name(it) = nm;
    if (DIFFER(args)) { if (EQ(ls_nitem(args), 0)) { args = ; } }
    rt = RkTestRt[nm];
    if (IDENT(rt) DIFFER(RkCodeV[nm])) {
        cc = mk1('TT_INVOKE', '', mk('TT_VAR', nm '__code'));
        if (DIFFER(args)) { pos = mk('TL', ''); ArgList(args, 0, pos, ''); i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); } }
        tk_t(it) = cc; RkbCall = it; return;
    }
    if ((EQ(form, 0), IDENT(args))) {
        if (nm ? (POS(0) ('True' | 'False') RPOS(0))) { tk_kind(it) = 'NAME'; tk_t(it) = MkBool((IDENT(nm, 'True') 1, 0)); RkbCall = it; return; }
        if (IDENT(nm, 'last')) { tk_t(it) = mk('TT_LOOP_BREAK', ''); RkbCall = it; return; }
        if (IDENT(nm, 'next')) { tk_t(it) = mk('TT_LOOP_NEXT', ''); RkbCall = it; return; }
        if (nm ? (POS(0) ('return' | 'fail') RPOS(0))) { tk_t(it) = mk('TT_RETURN', ''); RkbCall = it; return; }
        if (IDENT(nm, 'exit')) { tk_t(it) = Call1('__rk_exit', ilit(0)); RkbCall = it; return; }
        if (DIFFER(rt)) { tk_t(it) = MakeCall(rt); RkbCall = it; return; }
        if (EQ(form, 0)) { tk_t(it) = VarNode(nm); RkbCall = it; return; }
        tk_t(it) = MakeCall(nm); RkbCall = it; return;
    }
    pos = mk('TL', ''); named = mk('TL', '');
    if (DIFFER(rt)) { ArgList(args, 0, pos, ''); tk_t(it) = RkTestopCall(rt, pos); RkbCall = it; return; }
    if (nm ? (POS(0) ('say' | 'print') RPOS(0))) {
        ArgList(args, 0, pos, ''); RkFlattenParenArgs(args, pos);
        cc = mk((IDENT(nm, 'say') 'TT_SAY', 'TT_PRINT'), '');
        i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); }
        tk_t(it) = cc; RkbCall = it; return;
    }
    if (IDENT(nm, 'take')) {
        ArgList(args, 0, pos, '');
        if (EQ(n(pos), 1)) { tk_t(it) = mk1('TT_SUSPEND', '', c(pos)[1]); RkbCall = it; return; }
        cc = MakeCall('__rk_arr'); i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); }
        tk_t(it) = mk1('TT_SUSPEND', '', cc); RkbCall = it; return;
    }
    if (IDENT(nm, 'return')) { ArgList(args, 0, pos, ''); cc = mk('TT_RETURN', ''); if (GT(n(pos), 0)) { ad(cc, c(pos)[1]); } tk_t(it) = cc; RkbCall = it; return; }
    if (IDENT(nm, 'fail')) { tk_t(it) = mk('TT_RETURN', ''); RkbCall = it; return; }
    if (IDENT(nm, 'exit')) { ArgList(args, 0, pos, ''); cc = MakeCall('__rk_exit'); if (GT(n(pos), 0)) { ad(cc, c(pos)[1]); } tk_t(it) = cc; RkbCall = it; return; }
    if (IDENT(nm, 'die')) { ArgList(args, 0, pos, ''); cc = mk('TT_DIE', ''); if (GT(n(pos), 0)) { ad(cc, c(pos)[1]); } tk_t(it) = cc; RkbCall = it; return; }
    if (IDENT(nm, 'flat')) { ArgList(args, 0, pos, ''); tk_t(it) = RkFlatCall(pos); RkbCall = it; return; }
    if (IDENT(nm, 'await')) { ArgList(args, 0, pos, ''); tk_t(it) = (EQ(n(pos), 1) c(pos)[1], RkFlatArr(pos)); RkbCall = it; return; }
    if (IDENT(nm, 'defined')) { ArgList(args, 0, pos, ''); if (EQ(n(pos), 1)) { tk_t(it) = mk2('TT_METHCALL', '', c(pos)[1], mk('TT_QLIT', 'defined')); RkbCall = it; return; } }
    if (IDENT(nm, 'join') EQ(form, 2)) { pos = mk('TL', ''); ArgList(args, 0, pos, ''); cc = MakeCall('join'); i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); } tk_t(it) = cc; RkbCall = it; return; }
    if (nm ? (POS(0) ('map' | 'grep' | 'sort') RPOS(0))) {
        k = (IDENT(nm, 'map') 'TT_MAP', IDENT(nm, 'grep') 'TT_GREP', 'TT_SORT');
        cc = mk(k, '');
        lo = 0;
        if (GT(ls_n(args), 0)) { if (IDENT(tk_kind(el_t0(ls_v(args)[1])), 'BLOCK') EQ(el_nitem(ls_v(args)[1]), 1)) { ad(cc, tk_lop(el_t0(ls_v(args)[1]))); lo = 1; } }
        f0 = (GE(ls_n(args), 2) el_t0(ls_v(args)[1]), '');
        if (EQ(lo, 0) ~IDENT(k, 'TT_SORT') DIFFER(f0)) {
            if (EQ(el_nitem(ls_v(args)[1]), 1) IDENT(tk_kind(f0), 'VAR') EQ(tk_npost(f0), 0) EQ(tk_npre(f0), 0)
                (IDENT(tk_cls(f0), 'S') | ((tk_name(f0) ? (POS(0) '&')) DIFFER(RkCodeV[SUBSTR(tk_name(f0), 2)])))) {
                lst = (EQ(ls_n(args), 2) ElTree(ls_v(args)[2]), MakeCall('__rk_arr'));
                if (GT(ls_n(args), 2)) { i = 1; while (i = LT(i, ls_n(args)) i + 1) { ad(lst, ElTree(ls_v(args)[i])); } }
                m = mk3('TT_METHCALL', '', lst, mk('TT_QLIT', nm), ElTree(ls_v(args)[1]));
                tk_t(it) = m; RkbCall = it; return;
            }
        }
        i = lo; while (i = LT(i, ls_n(args)) i + 1) { ad(cc, ElTree(ls_v(args)[i])); }
        if (EQ(form, 1) EQ(lo, 0) GT(ls_n(args), 1)) {
            a = MakeCall('__rk_arr'); i = 0; while (i = LT(i, n(cc)) i + 1) { ad(a, c(cc)[i]); }
            n(cc) = 0; c(cc) = ''; ad(cc, a);
        }
        tk_t(it) = cc; RkbCall = it; return;
    }
    if (IDENT(nm, 'reverse')) { tk_t(it) = mk1('TT_REVERSE', '', RkbParen(args, 1)); RkbCall = it; return; }
    if (nm ? (POS(0) ('exists' | 'delete') RPOS(0))) {
        a = RkbExpr(args);
        if (IDENT(t(a), 'TT_HASH_GET')) { t(a) = (IDENT(nm, 'exists') 'TT_HASH_EXISTS', 'TT_HASH_DELETE'); tk_t(it) = a; RkbCall = it; return; }
        tk_t(it) = Call1(nm, a); RkbCall = it; return;
    }
    hn = ArgList(args, 1, pos, named);
    if (EQ(hn, 1)) { tk_t(it) = RkNamedCall(nm, (GT(n(pos), 0) pos, ''), named); RkbCall = it; return; }
    cc = MakeCall(nm); i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); }
    tk_t(it) = cc;
    RkbCall = it;
    return;
}
function RkFlatArr(pos, cc, i)   { cc = MakeCall('__rk_arr'); i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); } RkFlatArr = cc; return; }
function RkFlatCall(pos, cc, i, a) {
    cc = MakeCall('__rk_arr');
    i = 0; while (i = LT(i, n(pos)) i + 1) { a = c(pos)[i]; ad(cc, ((IDENT(t(a), 'TT_TO') GE(n(a), 2)) RkArrRhs(a), a)); }
    RkFlatCall = cc;
    return;
}
function RkTestopCall(nm, pos, cc, i, lst, pr) {
    cc = MakeCall(nm);
    if (EQ(n(pos), 1)) {
        lst = c(pos)[1];
        if (IDENT(t(lst), 'TT_FNC') IDENT(v(lst), '__rk_arr')) { i = 1; while (i = LT(i, n(lst)) i + 1) { ad(cc, c(lst)[i]); } RkTestopCall = cc; return; }
    }
    i = 0; while (i = LT(i, n(pos)) i + 1) { ad(cc, c(pos)[i]); }
    RkTestopCall = cc;
    return;
}
/* ==================================================================================================================== */
/* declarations: RkDecl from the raw R_SCOPED / R_DECLV / R_DECLP, build_decl, has_items                              */
/* ==================================================================================================================== */
RkScopeIx = TABLE(31);
RkScopeIx['my'] = 0; RkScopeIx['our'] = 1; RkScopeIx['has'] = 2; RkScopeIx['HAS'] = 3; RkScopeIx['augment'] = 4; RkScopeIx['anon'] = 5;
RkScopeIx['state'] = 6; RkScopeIx['supersede'] = 7; RkScopeIx['unit'] = 8;
function RkTraitsOf(tr)          { RkTraitsOf = tr; return; }
function RkBuildScoped(nd, sc, ty, dn, it, d, dt) {
    sc = RkScopeIx[v(c(nd)[1])]; ty = v(c(nd)[2]); dn = c(nd)[3];
    dt = t(dn);
    if ((IDENT(dt, 'R_DECLV'), IDENT(dt, 'R_DECLP'))) {
        d = RkBuildDeclRaw(dn);
        dc_scope(d) = sc;
        if (DIFFER(ty)) { dc_type(d) = ty; }
        it = NewTerm('DECL');
        tk_decl(it) = d;
        tk_t(it) = ;
        RkBuildScoped = it;
        return;
    }
    rk_scope_now = sc;
    RkBuildScoped = RkBuildTerm(dn);
    return;
}
function RkBuildDeclRaw(dn, d, vv, ntr) {
    d = rkdecl('', '', '', '', '', '', '', '');
    if (IDENT(t(dn), 'R_DECLP')) {
        dc_sigil(d) = '(';
        dc_sig(d) = RkBuildSig(c(dn)[1]);
        dc_tr(d) = RkTraitsOf(c(dn)[2]);
        if (EQ(n(dn), 4)) { dc_initop(d) = v(c(dn)[3]); dc_init(d) = RkBuildList(c(dn)[4]); }
        RkBuildDeclRaw = d;
        return;
    }
    vv = v(c(dn)[1]);
    dc_sigil(d) = SUBSTR(vv, 1, 1);
    dc_name(d) = vv;
    if (IDENT(dc_sigil(d), '&')) { RkCodeV[SUBSTR(vv, 2)] = 1; }
    dc_tr(d) = RkTraitsOf(c(dn)[2]);
    if (EQ(n(dn), 4)) { dc_initop(d) = v(c(dn)[3]); dc_init(d) = RkBuildList(c(dn)[4]); }
    RkBuildDeclRaw = d;
    return;
}
function RkDeclNode(type, var, val, e) { e = mk2('TT_DECL', '', mk('TT_VAR', type), var); ad(e, val); RkDeclNode = e; return; }
function RkIsAliasable(r)        { if (t(r) ? (POS(0) ('TT_VAR' | 'TT_ARR_GET' | 'TT_HASH_GET' | 'TT_FIELD' | 'TT_TWIGIL_FIELD' | 'TT_INDIRECT') RPOS(0))) { return; } freturn; }
function RkBind(tg, rhs)         { if (RkIsAliasable(rhs)) { freturn; } RkBind = Bin('TT_ASSIGN', tg, rhs); return; }
function RkRsList(rs, nrs, cc, i) { cc = MakeCall('__rk_arr'); i = 0; while (i = LT(i, nrs) i + 1) { ad(cc, ElTree(rs[i])); } RkRsList = cc; return; }
function RkPairSeg(e, tm) {
    tm = el_t0(e);
    if (EQ(el_nitem(e), 1) IDENT(tk_kind(tm), 'FAT') EQ(tk_npre(tm), 0) EQ(tk_npost(tm), 0)) { return; }
    if (GE(el_nitem(e), 2) IDENT(tk_kind(tm), 'QUOTE') EQ(tk_npre(tm), 0) EQ(tk_npost(tm), 0) IDENT(t(tk_t(tm)), 'TT_QLIT') IDENT(el_op1(e), '=>')) { return; }
    freturn;
}
function RkPairAdd(h, e, tm) {
    tm = el_t0(e);
    if (IDENT(tk_kind(tm), 'FAT')) { ad(h, mk('TT_QLIT', tk_name(tm))); ad(h, tk_val(tm)); return; }
    ad(h, mk('TT_QLIT', v(tk_t(tm)))); ad(h, ElRest(e));
    return;
}
function RkParenHash(L, h, i) {
    if (IDENT(L)) { freturn; }
    if (EQ(ls_n(L), 0)) { freturn; }
    i = 0; while (i = LT(i, ls_n(L)) i + 1) { if (~RkPairSeg(ls_v(L)[i])) { freturn; } }
    h = MakeCall('__rk_hash');
    i = 0; while (i = LT(i, ls_n(L)) i + 1) { RkPairAdd(h, ls_v(L)[i]); }
    RkParenHash = h;
    return;
}
function RkHashPairs(rs, nrs, h, i) {
    if (EQ(nrs, 1) EQ(el_nitem(rs[1]), 1) IDENT(tk_kind(el_t0(rs[1])), 'PAREN')) { if (RkHashPairs = RkParenHash(tk_in(el_t0(rs[1])))) { return; } freturn; }
    i = 0; while (i = LT(i, nrs) i + 1) { if (~RkPairSeg(rs[i])) { freturn; } }
    h = MakeCall('__rk_hash');
    i = 0; while (i = LT(i, nrs) i + 1) { RkPairAdd(h, rs[i]); }
    RkHashPairs = h;
    return;
}
function RkDestructure(targets, rhs, tmp, q, i, get) {
    tmp = '__destr_' RkDestrUid; RkDestrUid = RkDestrUid + 1;
    q = mk('TT_SEQ_EXPR', '');
    ad(q, QuietStore(mk('TT_VAR', tmp), rhs));
    i = 0;
    while (i = LT(i, n(targets)) i + 1) {
        get = ad(ad(MakeCall('__rk_arr_at'), mk('TT_VAR', tmp)), ilit(i - 1));
        ad(q, QuietStore(c(targets)[i], get));
    }
    RkDestructure = q;
    return;
}
function RkMarkArrLit(bare, rhs) { if (IDENT(t(rhs), 'TT_FNC') IDENT(v(rhs), '__rk_arr_lit')) { RkAls[bare] = 1; } return; }
function RkBuildDecl(d, outer, ofirst, rs, nrs, i, nm, type, sig, rhs, targets, once, flag, q, u, e, var, bd, val) {
    if (EQ(dc_scope(d), 6) ~IDENT(dc_sigil(d), '(') DIFFER(dc_name(d))) {
        dc_scope(d) = 0; once = RkBuildDecl(d, outer, ofirst); dc_scope(d) = 6;
        flag = VarNode('$' SUBSTR(dc_name(d), 2) '_init');
        q = mk('TT_SEQ_EXPR', ''); ad(q, Bin('TT_ASSIGN', flag, ilit(1))); ad(q, once);
        u = mk2('TT_UNLESS', '', Clone(flag), q);
        RkBuildDecl = u;
        return;
    }
    rs = ARRAY('1:64'); nrs = 0;
    if (DIFFER(dc_init(d))) { i = 0; while (i = LT(i, ls_n(dc_init(d))) i + 1) { nrs = nrs + 1; rs[nrs] = ls_v(dc_init(d))[i]; } }
    if (DIFFER(outer)) { i = ofirst; while (i = LT(i, ls_n(outer)) i + 1) { nrs = nrs + 1; rs[nrs] = ls_v(outer)[i]; } }
    nm = (DIFFER(dc_name(d)) dc_name(d), '$');
    type = dc_type(d); sig = dc_sigil(d);
    if (IDENT(sig, '(')) {
        rhs = (EQ(nrs, 1) ElTree(rs[1]), RkRsList(rs, nrs));
        targets = mk('TL', '');
        if (DIFFER(dc_sig(d))) { i = 0; while (i = LT(i, n(dc_sig(d))) i + 1) { ad(targets, c(dc_sig(d))[i]); } }
        RkBuildDecl = RkDestructure(targets, rhs);
        return;
    }
    if ((IDENT(dc_initop(d)), EQ(nrs, 0))) {
        var = VarNode(nm);
        if (DIFFER(type)) { RkBuildDecl = RkDeclNode(type, var, ''); return; }
        if (IDENT(sig, '$')) { RkBuildDecl = Bin('TT_ASSIGN', var, mk('TT_NUL', '')); return; }
        RkBuildDecl = Bin('TT_ASSIGN', var, MakeCall('__rk_undef'));
        return;
    }
    if (IDENT(dc_initop(d), ':=')) {
        e = ElTree(rs[1]);
        var = VarNode(nm);
        if (~IDENT(sig, '$')) { RkBuildDecl = Bin('TT_ASSIGN', var, e); return; }
        if (~(bd = RkBind(var, RkScalarRhs(e)))) { bd = Bin('TT_ASSIGN', var, e); }
        if (DIFFER(type)) { RkBuildDecl = RkDeclNode(type, VarNode(nm), c(bd)[2]); return; }
        RkBuildDecl = bd;
        return;
    }
    if (IDENT(sig, '@')) {
        if (EQ(nrs, 1)) { val = RkArrRhs(ElTree(rs[1])); } else { val = RkRsList(rs, nrs); }
        var = VarNode(nm);
        RkBuildDecl = (DIFFER(type) RkDeclNode(type, var, val), Bin('TT_ASSIGN', var, val));
        return;
    }
    if (IDENT(sig, '%')) {
        val = ;
        if (IDENT(type)) { val = RkHashPairs(rs, nrs); }
        if (IDENT(val)) { val = ElTree(rs[1]); }
        var = VarNode(nm);
        RkBuildDecl = (DIFFER(type) RkDeclNode(type, var, val), Bin('TT_ASSIGN', var, val));
        return;
    }
    e = ElTree(rs[1]);
    if (DIFFER(type)) { RkBuildDecl = RkDeclNode(type, VarNode(nm), e); return; }
    RkMarkArrLit(StripSigil(nm), e);
    RkBuildDecl = Bin('TT_ASSIGN', VarNode(nm), RkScalarRhs(e));
    return;
}
function RkHasItems(list, d, nm, twig, fn, tw, tn, handles, i, tr, fv, e, hasinit) {
    nm = (DIFFER(dc_name(d)) dc_name(d), '$');
    twig = (GT(SIZE(nm), 2) (SUBSTR(nm, 2, 1) ? ANY('.!'))) 1;
    fn = (DIFFER(twig) SUBSTR(nm, 2), StripSigil(nm));
    tw = ''; tn = ''; handles = '';
    i = 0;
    while (i = LT(i, n(dc_tr(d))) i + 1) {
        tr = c(dc_tr(d))[i];
        if (IDENT(v(c(tr)[1]), 'handles')) { handles = v(c(tr)[2]); continue; }
        if (IDENT(tw)) { tw = v(c(tr)[1]); tn = v(c(tr)[2]); }
    }
    if (DIFFER(handles)) { fv = mk1('TT_HANDLES_DECL', fn, mk('TT_QLIT', handles)); ad(list, fv); return; }
    hasinit = (IDENT(dc_initop(d), '=') DIFFER(dc_init(d))) 1;
    if (nm ? (POS(0) ANY('@%'))) { ad(list, mk((IDENT(SUBSTR(nm, 1, 1), '@') 'TT_ARR_DECL', 'TT_HASH_DECL'), fn)); return; }
    if (DIFFER(hasinit)) {
        e = RkbExpr(dc_init(d));
        if (IDENT(tw, 'is') IDENT(tn, 'rw')) { ad(list, mk('TT_RW_DECL', fn)); }
        ad(list, mk1('TT_HAS_DECL', fn, e));
        return;
    }
    if (IDENT(tw, 'is') IDENT(tn, 'required')) { fv = mk('TT_HAS_DECL', fn); }
    else { if (IDENT(tw, 'is') IDENT(tn, 'rw')) { fv = mk('TT_RW_DECL', fn); } else { fv = mk('TT_VAR', fn); } }
    ad(list, fv);
    return;
}
/* ==================================================================================================================== */
/* signatures and routines: rkb_param, rkb_routine (rk_defaults_prologue, rk_multi_mangle), rkb_block_term           */
/* ==================================================================================================================== */
function RkBuildSig(sg, s, i, p) {
    s = mk('TT_SEQ_EXPR', '');
    if (IDENT(t(sg), 'R_SIG')) { i = 0; while (i = LT(i, n(sg)) i + 1) { ad(s, RkBuildParam(c(sg)[i])); } }
    RkBuildSig = s;
    return;
}
function RkBuildParam(pn, types, pre, vv, dflt, dl, p, ty) {
    types = c(pn)[1]; pre = v(c(pn)[2]); vv = RkTrim(v(c(pn)[3])); dflt = c(pn)[4];
    dl = ;
    if ((IDENT(t(dflt), 'R_LIST'), IDENT(t(dflt), 'R_CROSS'))) { dl = RkBuildList(dflt); }
    if (pre ? (POS(0) ('**' | '*') RPOS(0))) {
        p = VarNode(vv);
        ad(p, mk('TT_QLIT', (IDENT(pre, '**') '**@', (vv ? (POS(0) '%')) '*%', '*@')));
    } else {
        if ((vv ? (POS(0) '@')) EQ(n(types), 0)) { p = VarNode(vv); ad(p, mk('TT_QLIT', '@')); }
        else {
            if (GT(n(types), 0)) { ty = RkTrimR(v(c(types)[1])); p = VarNode(vv); ad(p, mk('TT_QLIT', ty)); }
            else { p = VarNode(vv); }
        }
    }
    if (DIFFER(dl)) { p = Bin('TT_ASSIGN', p, RkbExpr(dl)); }
    RkBuildParam = p;
    return;
}
function RkTrimR(s)              { RkTrimR = s; s ? (POS(0) (ARB NOTANY(' ' CHAR(9) CHAR(10) CHAR(13))) . RkTrimR FENCE(SPAN(' ' CHAR(9) CHAR(10) CHAR(13)) | epsilon) RPOS(0)); return; }
function RkDefaultsPrologue(params, body, pro, i, p, pv, dv, mc, un) {
    pro = mk('TT_SEQ_EXPR', '');
    i = 0;
    while (i = LT(i, n(params)) i + 1) {
        p = c(params)[i];
        if (~(IDENT(t(p), 'TT_ASSIGN') GE(n(p), 2))) { continue; }
        pv = c(p)[1]; dv = c(p)[2];
        c(params)[i] = pv;
        mc = mk2('TT_METHCALL', '', Clone(pv), mk('TT_QLIT', 'defined'));
        un = mk2('TT_UNLESS', '', mc, Seq1(Bin('TT_ASSIGN', Clone(pv), dv)));
        ad(pro, un);
    }
    if (EQ(n(pro), 0)) { RkDefaultsPrologue = body; return; }
    i = 0; while (i = LT(i, n(body)) i + 1) { ad(pro, c(body)[i]); }
    RkDefaultsPrologue = pro;
    return;
}
function RkMultiMangle(base, params, s, i, p, ty) {
    s = base '$' n(params);
    i = 0;
    while (i = LT(i, n(params)) i + 1) {
        p = c(params)[i];
        ty = 'Any';
        if (GT(n(p), 0)) { if (DIFFER(v(c(p)[1]))) { ty = v(c(p)[1]); } }
        if (ty ? (POS(0) ('*@' | '**@') RPOS(0))) { ty = 'Slurpy'; }
        while (ty ? ':' = '_') { }
        s = s '$' ty;
    }
    RkMultiMangle = s;
    return;
}
function RkBuildMulti(nd, ms)    { ms = rk_multi; rk_multi = (IDENT(v(c(nd)[1]), 'multi') 1, IDENT(v(c(nd)[1]), 'proto') 2, 3); RkBuildMulti = RkBuildTerm(c(nd)[2]); rk_multi = ms; return; }
function RkBuildRoutine(nd, x, kind, nm, sig, body, it, params, rkbody, mn, e, i, lastl, bn) {
    kind = v(c(nd)[1]); nm = RkTrim(v(c(nd)[2])); sig = c(nd)[3];
    params = RkBuildSig(sig);
    body = RkBuildBlock(c(nd)[4]);
    lastl = RkBlockLast; bn = RkBlockN;
    if (IDENT(nm) EQ(kind, 0)) { RkBuildRoutine = RkbBlockTermOf(body, (IDENT(t(sig), 'R_SIG') params, ''), lastl, bn); return; }
    it = NewTerm('BSTMT');
    if (IDENT(body)) { body = mk('TT_SEQ_EXPR', ''); }
    rkbody = RkDefaultsPrologue(params, body);
    mn = (EQ(rk_multi, 1) RkMultiMangle(nm, params), nm);
    e = mk('TT_SUB_DECL', '');
    ad(e, mk('TT_VAR', mn));
    i = 0; while (i = LT(i, n(params)) i + 1) { ad(e, c(params)[i]); }
    i = 0; while (i = LT(i, n(rkbody)) i + 1) { ad(e, c(rkbody)[i]); }
    tk_t(it) = e;
    RkBuildRoutine = it;
    return;
}
function RkbBlockTerm(sg, stmts, sig, seq)  { sig = (IDENT(t(sg), 'R_SIG') RkBuildSig(sg), ''); seq = RkBuildBlock(stmts); RkbBlockTerm = RkbBlockTermOf(seq, sig, RkBlockLast, RkBlockN); return; }
function RkbBlockTermOf(seq, sig, last, nst, it, h, a, i, p) {
    it = NewTerm('BLOCK');
    tk_val(it) = seq;
    tk_lop(it) = RkbParen(last, nst);
    if (IDENT(sig) EQ(nst, 1) DIFFER(last)) { if (GE(ls_n(last), 1)) { if (h = RkParenHash(last)) { tk_kind(it) = 'TREE'; tk_t(it) = h; RkbBlockTermOf = it; return; } } }
    a = mk1('TT_ANON_BLOCK', '', seq);
    if (DIFFER(sig)) { i = 0; while (i = LT(i, n(sig)) i + 1) { p = c(sig)[i]; if (IDENT(t(p), 'TT_ASSIGN') GT(n(p), 0)) { p = c(p)[1]; } ad(a, p); } }
    tk_t(it) = a;
    RkbBlockTermOf = it;
    return;
}
/* ==================================================================================================================== */
/* packages, regexes, enums: rkb_package, rkb_regex_decl, rkb_enum                                                    */
/* ==================================================================================================================== */
function RkBuildPackage(nd, kind, nm, trs, body, k, cd, s, i, tr, tag, it) {
    kind = v(c(nd)[1]); nm = RkTrim(RkNamePart(v(c(nd)[2]))); trs = c(nd)[3];
    body = RkBuildBlock(c(nd)[4]);
    k = (IDENT(kind, 'class') 'TT_CLASS_DECL', IDENT(kind, 'role') 'TT_ROLE_DECL', IDENT(kind, 'grammar') 'TT_GRAMMAR_DECL', 'TT_MODULE_DECL');
    cd = mk(k, '');
    if (IDENT(k, 'TT_CLASS_DECL')) {
        s = '';
        i = 0;
        while (i = LT(i, n(trs)) i + 1) {
            tr = c(trs)[i];
            tag = (IDENT(v(c(tr)[1]), 'is') 'i', IDENT(v(c(tr)[1]), 'does') 'd', '');
            if (IDENT(tag)) { continue; }
            if (DIFFER(s)) { s = s CHAR(1); }
            s = s tag RkNamePart(v(c(tr)[2]));
        }
        if (DIFFER(s)) { v(cd) = s; }
    }
    ad(cd, mk('TT_VAR', nm));
    if (DIFFER(body)) { i = 0; while (i = LT(i, n(body)) i + 1) { ad(cd, c(body)[i]); } }
    it = NewTerm('BSTMT'); tk_t(it) = cd;
    RkBuildPackage = it;
    return;
}
function RkBuildRegex(nd, it, rd) {
    rd = mk('TT_REGEX_DECL', '');
    ad(rd, mk('TT_VAR', RkTrim(v(c(nd)[2]))));
    ad(rd, mk('TT_QLIT', v(c(nd)[3])));
    it = NewTerm('BSTMT'); tk_t(it) = rd;
    RkBuildRegex = it;
    return;
}
function RkbEnum(txt, l, idx, w) {
    l = mk('TT_SEQ_EXPR', ''); idx = 0;
    while (txt ? (POS(0) FENCE(SPAN(' ' CHAR(9) CHAR(10)) | epsilon) (BREAK(' ' CHAR(9) CHAR(10)) | (LEN(1) REM)) . w) =) {
        if (IDENT(w)) { break; }
        ad(l, Bin('TT_ASSIGN', VarNode(RkTrim(w)), ilit(idx))); idx = idx + 1;
    }
    RkbEnum = l;
    return;
}
/* ==================================================================================================================== */
/* statement prefixes: rkb_sprefix_term (phasers, try, gather, do)                                                   */
/* ==================================================================================================================== */
RkPhaserW = TABLE(31);
RkPhaserW['BEGIN'] = 1; RkPhaserW['CHECK'] = 1; RkPhaserW['INIT'] = 1; RkPhaserW['END'] = 1; RkPhaserW['ENTER'] = 1; RkPhaserW['LEAVE'] = 1;
RkPhaserW['KEEP'] = 1; RkPhaserW['UNDO'] = 1; RkPhaserW['PRE'] = 1; RkPhaserW['POST'] = 1; RkPhaserW['FIRST'] = 1; RkPhaserW['LAST'] = 1;
RkPhaserW['NEXT'] = 1; RkPhaserW['TEMP'] = 1;
function RkPhaserMark(w, body)   { RkPhaserMark = ad(MakeCall('__rk_phaser_' w), (DIFFER(body) body, mk('TT_SEQ_EXPR', ''))); return; }
function RkBuildSprefix(nd, w, b, blk, stmt, it, m) {
    w = v(c(nd)[1]); b = c(nd)[2];
    blk = ; stmt = ;
    if (IDENT(t(b), 'R_BLORSTB')) { blk = RkBuildBlock(c(b)[1]); }
    else { stmt = RkBuildBlorst(c(b)[1]); }
    it = NewTerm('TREE');
    if ((DIFFER(RkPhaserW[w]), (DIFFER(blk) (w ? (POS(0) ('once' | 'quietly' | 'react' | 'do') RPOS(0)))))) {
        tk_kind(it) = 'BSTMT'; tk_ck(it) = (DIFFER(blk) 2, 1);
        tk_t(it) = RkPhaserMark(w, (DIFFER(blk) blk, Seq1(stmt)));
        RkBuildSprefix = it; return;
    }
    if (IDENT(w, 'try')) { tk_kind(it) = (DIFFER(blk) 'BSTMT', 'TREE'); tk_t(it) = mk1('TT_TRY', '', (DIFFER(blk) blk, Seq1(stmt))); RkBuildSprefix = it; return; }
    if (IDENT(w, 'gather')) { tk_t(it) = mk1('TT_GATHER', '', (DIFFER(blk) blk, stmt)); RkBuildSprefix = it; return; }
    if (IDENT(w, 'do') IDENT(blk)) { if (m = RkDoForMap(stmt)) { tk_t(it) = m; RkBuildSprefix = it; return; } }
    tk_t(it) = (DIFFER(blk) blk, stmt);
    RkBuildSprefix = it;
    return;
}
function RkDoForMap(st, vv, e) {
    if (IDENT(st)) { freturn; }
    if (~(IDENT(t(st), 'TT_EVERY') EQ(n(st), 2))) { freturn; }
    if (~(IDENT(t(c(st)[1]), 'TT_ITERATE') EQ(n(c(st)[1]), 1))) { freturn; }
    vv = v(c(st)[1]);
    if (DIFFER(vv) ~IDENT(vv, '_')) { freturn; }
    e = c(st)[2];
    while (IDENT(t(e), 'TT_SEQ_EXPR') EQ(n(e), 1)) { e = c(e)[1]; }
    if (IDENT(t(e), 'TT_SEQ_EXPR')) { freturn; }
    RkDoForMap = mk2('TT_MAP', '', e, c(c(st)[1])[1]);
    return;
}
function RkBuildBlorst(stmts, bk, lst) {
    bk = (Cur_bk ? (POS(0) ('ITEMS' | 'MAIN' | 'CLASS' | 'GRAMMAR') RPOS(0))) 'MODULE';
    bk = (DIFFER(bk) bk, Cur_bk);
    lst = RkBuildStmtsBk(stmts, bk);
    RkBuildBlorst = (GT(n(lst), 0) c(lst)[1], '');
    return;
}
/* ==================================================================================================================== */
/* statements into a statement list: rkb_statement (stmt_plain, stmt_tail, assign_forms, wrap_mod), rkb_control,      */
/* rkb_empty, rkb_block_seq (tail value, phasers placed), rkb_program                                                */
/* ==================================================================================================================== */
function RkEmpty(list, bk)       { if (IDENT(list)) { return; } if (bk ? (POS(0) ('CLASS' | 'GRAMMAR' | 'ITEMS') RPOS(0))) { return; } ad(list, mk('TT_SEQ_EXPR', '')); return; }
function RkPlain(tm, cls)        { if (IDENT(tk_kind(tm), 'VAR') IDENT(tk_cls(tm), cls) EQ(tk_npost(tm), 0) EQ(tk_npre(tm), 0)) { return; } freturn; }
function RkBkBlockish(bk)        { if (bk ? (POS(0) ('BLOCK' | 'CATCH' | 'GIVEN') RPOS(0))) { return; } freturn; }
function RkAssignForms(L, tail, bk, e0, t0, op1, eq, e, bd, rhs, fe, g, cc, clv) {
    e0 = ls_v(L)[1]; t0 = el_t0(e0);
    op1 = (GT(ls_nitem(L), 1) ls_op1(L), '');
    if (IDENT(op1)) { freturn; }
    eq = IDENT(op1, '=') 1;
    if (IDENT(op1, ':=') RkPlain(t0, 'S') ((EQ(tail, 0), RkBkBlockish(bk)))) {
        e = ElRest(e0);
        if (~(bd = RkBind(VarNode(tk_name(t0)), RkScalarRhs(e)))) { bd = Bin('TT_ASSIGN', VarNode(tk_name(t0)), e); }
        RkAssignForms = bd; return;
    }
    if (DIFFER(eq) GT(tk_npost(t0), 0) EQ(tk_npre(t0), 0) DIFFER(tk_t(t0)) EQ(ls_n(L), 1) ((EQ(tail, 0), RkBkBlockish(bk)))) {
        if (IDENT(t(tk_t(t0)), 'TT_METHCALL') EQ(n(tk_t(t0)), 2)) {
            rhs = ElRest(e0);
            fe = mk1('TT_FIELD', v(c(tk_t(t0))[2]), c(tk_t(t0))[1]);
            RkAssignForms = Bin('TT_ASSIGN', fe, rhs); return;
        }
    }
    if (DIFFER(eq) RkPlain(t0, 'T') EQ(ls_n(L), 1) ((EQ(tail, 0), ~IDENT(bk, 'SUB')))) {
        rhs = ElRest(e0);
        fe = mk('TT_TWIGIL_FIELD', TwBare(SUBSTR(tk_name(t0), 2)));
        RkAssignForms = Bin('TT_ASSIGN', fe, rhs); return;
    }
    if (DIFFER(eq) IDENT(tk_kind(t0), 'VAR') EQ(tk_npost(t0), 1) EQ(tk_npre(t0), 0) DIFFER(tk_t(t0)) EQ(ls_n(L), 1) ((EQ(tail, 0), RkBkBlockish(bk)))) {
        g = tk_t(t0);
        if (IDENT(t(g), 'TT_ARR_GET') ((EQ(tail, 0), IDENT(tk_cls(t0), 'A')))) { RkAssignForms = mk3('TT_ARR_SET', '', c(g)[1], c(g)[2], ElRest(e0)); return; }
        if (IDENT(t(g), 'TT_FNC') IDENT(v(g), '__rk_arr_slice') EQ(n(g), 4) ((EQ(tail, 0), IDENT(tk_cls(t0), 'A')))) {
            RkAssignForms = mk3('TT_ARR_SET', '', c(g)[2], Bin('TT_TO', c(g)[3], c(g)[4]), ElRest(e0)); return;
        }
        if (IDENT(t(g), 'TT_HASH_GET') ((EQ(tail, 0), IDENT(tk_cls(t0), 'H')))) { RkAssignForms = mk3('TT_HASH_SET', '', c(g)[1], c(g)[2], ElRest(e0)); return; }
    }
    if (IDENT(eq) IDENT(tk_kind(t0), 'VAR') EQ(tk_npost(t0), 1) EQ(tk_npre(t0), 0) RkIsElem(tk_t(t0)) EQ(ls_n(L), 1) ((EQ(tail, 0), RkBkBlockish(bk)))) {
        if (clv = RkCompoundBase(op1)) {
            g = tk_t(t0); rhs = ElRest(e0);
            RkAssignForms = RkElemStore(g, RkScalarRhs(RkbBinop(clv, rk_cbk, Clone(g), rhs))); return;
        }
    }
    freturn;
}
function RkStmtPlain(L, e0, t0, op1, rhs, first, cc, i, g, var, from, len, inner, lhs, val, q, clv, eq, targets, mc, a) {
    if (EQ(ls_n(L), 0)) { RkStmtPlain = mk('TT_NUL', ''); return; }
    e0 = ls_v(L)[1]; t0 = el_t0(e0);
    op1 = (GT(ls_nitem(L), 1) ls_op1(L), '');
    if (IDENT(tk_kind(t0), 'DECL') EQ(tk_npost(t0), 0) EQ(tk_npre(t0), 0) EQ(el_nitem(e0), 1)) { RkStmtPlain = RkBuildDecl(tk_decl(t0), L, 1); return; }
    if (IDENT(op1, '=') RkPlain(t0, 'S') EQ(ls_n(L), 1)) {
        rhs = ElRest(e0);
        RkMarkArrLit(StripSigil(tk_name(t0)), rhs);
        RkStmtPlain = Bin('TT_ASSIGN', VarNode(tk_name(t0)), RkScalarRhs(rhs));
        return;
    }
    if (IDENT(op1, '=') RkPlain(t0, 'A')) {
        first = ElRest(e0);
        if (EQ(ls_n(L), 1)) { RkStmtPlain = Bin('TT_ASSIGN', VarNode(tk_name(t0)), RkArrRhs(first)); return; }
        cc = ad(MakeCall('__rk_arr'), first);
        i = 1; while (i = LT(i, ls_n(L)) i + 1) { ad(cc, ElTree(ls_v(L)[i])); }
        RkStmtPlain = Bin('TT_ASSIGN', VarNode(tk_name(t0)), cc);
        return;
    }
    if (IDENT(op1, '=') EQ(ls_n(L), 1) DIFFER(tk_t(t0))) {
        g = tk_t(t0); var = ;
        if (IDENT(t(g), 'TT_FNC') IDENT(v(g), 'substr-rw') GE(n(g), 3)) { var = c(g)[2]; from = c(g)[3]; len = (GE(n(g), 4) c(g)[4], ''); }
        if (IDENT(t(g), 'TT_METHCALL') GE(n(g), 3)) { if (IDENT(v(c(g)[2]), 'substr-rw')) { var = c(g)[1]; from = c(g)[3]; len = (GE(n(g), 4) c(g)[4], ''); } }
        if (DIFFER(var)) { if (IDENT(t(var), 'TT_VAR') ~(v(var) ? (POS(0) ANY('@%')))) {
            cc = ad(ad(MakeCall('__rk_substr_replace'), Clone(var)), from);
            ad(cc, len);
            ad(cc, ElRest(e0));
            RkStmtPlain = Bin('TT_ASSIGN', Clone(var), cc);
            return;
        } }
    }
    if (DIFFER(op1) IDENT(tk_kind(t0), 'PAREN') EQ(tk_npost(t0), 0) EQ(tk_npre(t0), 0) EQ(ls_n(L), 1) DIFFER(tk_t(t0))) {
        inner = tk_t(t0);
        if (((IDENT(t(inner), 'TT_ASSIGN') EQ(n(inner), 2) ((IDENT(t(c(inner)[1]), 'TT_VAR') ~(v(c(inner)[1]) ? (POS(0) ANY('@%')))), RkIsElem(c(inner)[1]))),
            ((t(inner) ? (POS(0) ('TT_ARR_SET' | 'TT_HASH_SET') RPOS(0))) EQ(n(inner), 3)))) {
            eq = IDENT(op1, '=') 1;
            if ((DIFFER(eq), (clv = RkCompoundBase(op1)))) {
                lhs = c(inner)[1]; rhs = ElRest(e0);
                if (t(inner) ? (POS(0) ('TT_ARR_SET' | 'TT_HASH_SET') RPOS(0))) { lhs = mk2((IDENT(t(inner), 'TT_ARR_SET') 'TT_ARR_GET', 'TT_HASH_GET'), '', Clone(c(inner)[1]), Clone(c(inner)[2])); }
                val = (DIFFER(eq) RkScalarRhs(rhs), RkScalarRhs(RkbBinop(clv, rk_cbk, Clone(lhs), rhs)));
                q = mk('TT_SEQ_EXPR', ''); ad(q, inner);
                ad(q, (RkIsElem(lhs) RkElemStore(lhs, val), Bin('TT_ASSIGN', Clone(lhs), val)));
                RkStmtPlain = q;
                return;
            }
        }
    }
    if (IDENT(op1, '=') IDENT(tk_kind(t0), 'PAREN') EQ(tk_npost(t0), 0) EQ(tk_npre(t0), 0)) {
        rhs = ElRest(e0);
        targets = mk('TL', '');
        a = tk_t(t0);
        if (IDENT(t(a), 'TT_FNC') IDENT(v(a), '__rk_arr')) { i = 1; while (i = LT(i, n(a)) i + 1) { ad(targets, c(a)[i]); } } else { ad(targets, a); }
        RkStmtPlain = RkDestructure(targets, rhs);
        return;
    }
    if (IDENT(op1, '.=') RkPlain(t0, 'S') EQ(ls_nitem(L), 2)) {
        if (IDENT(tk_kind(ls_t1(L)), 'DOTTY') IDENT(t(tk_t(ls_t1(L))), 'TT_METHCALL')) {
            mc = tk_t(ls_t1(L)); c(mc)[1] = VarNode(tk_name(t0));
            RkStmtPlain = QuietStore(VarNode(tk_name(t0)), mc);
            return;
        }
    }
    if (a = RkAssignForms(L, 0, 'MAIN')) { RkStmtPlain = a; return; }
    RkStmtPlain = ElTree(e0);
    return;
}
function RkStmtTail(L, bk, a, t0, e, r) {
    if (EQ(ls_n(L), 0)) { RkStmtTail = mk('TT_NUL', ''); return; }
    if (a = RkAssignForms(L, 1, bk)) { RkStmtTail = a; return; }
    t0 = el_t0(ls_v(L)[1]);
    e = ElTree(ls_v(L)[1]);
    if (bk ? (POS(0) ('SUB' | 'METHOD') RPOS(0))) {
        if (EQ(ls_nitem(L), 1) IDENT(tk_kind(t0), 'CALL') (tk_name(t0) ? (POS(0) ('say' | 'print' | 'return') RPOS(0)))) { RkStmtTail = e; return; }
        if (IDENT(t(e), 'TT_YADA') IDENT(bk, 'METHOD')) { RkStmtTail = e; return; }
        RkStmtTail = mk1('TT_RETURN', '', e);
        return;
    }
    RkStmtTail = e;
    return;
}
function RkWrapMod(k, cx, base, cond, e, gen) {
    if (IDENT(k, 'for')) { cond = RkbParen(cx, 1); gen = mk1('TT_ITERATE', '_', cond); RkWrapMod = Bin('TT_EVERY', gen, Seq1(base)); return; }
    cond = RkbExpr(cx);
    if (k ? (POS(0) ('if' | 'when') RPOS(0))) { RkWrapMod = mk2('TT_IF', '', cond, Seq1(base)); return; }
    if (IDENT(k, 'unless')) { RkWrapMod = mk2('TT_UNLESS', '', cond, Seq1(base)); return; }
    if (IDENT(k, 'while')) { RkWrapMod = Bin('TT_WHILE', cond, Seq1(base)); return; }
    if (IDENT(k, 'until')) { RkWrapMod = mk2('TT_UNTIL', '', cond, Seq1(base)); return; }
    if (k ? (POS(0) ('with' | 'without') RPOS(0))) { RkWrapMod = RkWithMod(base, cond, (IDENT(k, 'without') 1, 0)); return; }
    RkWrapMod = mk2('TT_SEQ_EXPR', '', Bin('TT_ASSIGN', mk('TT_VAR', '_'), cond), base);
    return;
}
function RkWithMod(stmt, cond, neg, topic, dcall, gate) {
    topic = Bin('TT_ASSIGN', mk('TT_VAR', '_'), cond);
    dcall = Call1('__rk_defined', mk('TT_VAR', '_'));
    gate = mk2((EQ(neg, 1) 'TT_UNLESS', 'TT_IF'), '', dcall, Seq1(stmt));
    RkWithMod = mk2('TT_SEQ_EXPR', '', topic, gate);
    return;
}
function RkbStatement(list, bk, L, nmods, mk_, mx, semi, last, e0, t0, single, ns, tail, tr, i) {
    if ((IDENT(bk, 'ITEMS'), IDENT(list), IDENT(L))) { return; }
    if ((EQ(ls_nitem(L), 0), EQ(ls_n(L), 0))) { return; }
    e0 = ls_v(L)[1]; t0 = el_t0(e0);
    single = EQ(ls_nitem(L), 1) 1;
    if (IDENT(bk, 'CLASS')) {
        if (DIFFER(single) IDENT(tk_kind(t0), 'DECL') DIFFER(tk_decl(t0))) { if (EQ(dc_scope(tk_decl(t0)), 2)) { RkHasItems(list, tk_decl(t0)); return; } }
        if (DIFFER(single) IDENT(tk_kind(t0), 'BSTMT')) { ad(list, tk_t(t0)); return; }
        ad(list, RkStmtPlain(L));
        return;
    }
    if (IDENT(bk, 'GRAMMAR')) { ad(list, (IDENT(tk_kind(t0), 'DECL') RkBuildDecl(tk_decl(t0), '', 0), DIFFER(tk_t(t0)) tk_t(t0), mk('TT_NUL', ''))); return; }
    ns = 1;
    if (DIFFER(single) EQ(nmods, 0) EQ(tk_npre(t0), 0) EQ(tk_npost(t0), 0)) {
        if (IDENT(tk_kind(t0), 'BSTMT')) { ns = (EQ(tk_ck(t0), 2) 2, EQ(tk_ck(t0), 1) 1, 0); }
        else { if (IDENT(tk_kind(t0), 'BLOCK') DIFFER(tk_val(t0))) { ns = 0; } }
    }
    if (IDENT(bk, 'CATCH')) { RkNonWhen = RkNonWhen + 1; }
    if (~EQ(ns, 1)) {
        ad(list, (IDENT(tk_kind(t0), 'BLOCK') tk_val(t0), tk_t(t0)));
        if (EQ(semi, 1) EQ(ns, 0)) { RkEmpty(list, bk); }
        return;
    }
    tail = (EQ(semi, 0) EQ(last, 1) (bk ? (POS(0) ('BLOCK' | 'SUB' | 'METHOD' | 'CATCH' | 'GIVEN') RPOS(0)))) 1;
    if (GT(nmods, 0)) {
        if (~(tr = RkAssignForms(L, 0, 'MAIN'))) { tr = ElTree(e0); }
        i = 0; while (i = LT(i, nmods) i + 1) { tr = RkWrapMod(mk_[i], mx[i], tr); }
    } else {
        tr = (DIFFER(tail) RkStmtTail(L, bk), RkStmtPlain(L));
    }
    ad(list, tr);
    if (DIFFER(tail)) { RkTailList = list; RkTailTree = tr; }
    return;
}
function RkbControl(list, bk, t2, ns, semi) {
    if ((IDENT(list), IDENT(bk, 'ITEMS'))) { return; }
    if (IDENT(bk, 'CATCH')) { RkNonWhen = RkNonWhen + 1; }
    ad(list, t2);
    if (EQ(semi, 1) EQ(ns, 0)) { RkEmpty(list, bk); }
    return;
}
/* the statement list of a block or the unit, from the raw R_STMTS: each statement built in order, as r_statementlist  */
function RkBuildStmtsBk(r, Cur_bk, Cur_list, i, s, nst, semi, last, lastl, lrs, ct, X, mods, nmods, mk_, mx, j, t2) {
    Cur_list = mk('TT_SEQ_EXPR', '');
    lastl = ; nst = n(r); lrs = 1;
    i = 0;
    while (i = LT(i, nst) i + 1) {
        s = c(r)[i];
        if (IDENT(t(s), 'R_EMPTYSTMT')) { RkEmpty(Cur_list, Cur_bk); lrs = 1; lastl = ; continue; }
        semi = v(c(s)[2]); lrs = semi;
        last = 0;
        if (EQ(i, nst) EQ(semi, 0)) { last = 1; }
        if (IDENT(Cur_bk, 'MAIN') EQ(last, 1)) { semi = 1; }
        ct = c(s)[1];
        if (IDENT(t(ct), 'R_CTL')) {
            RkCtlNs = 0;
            t2 = RkBuildControl(c(ct)[1]);
            if (DIFFER(t2)) { RkbControl(Cur_list, Cur_bk, t2, RkCtlNs, semi); }
            lastl = ;
            continue;
        }
        X = RkBuildList(c(ct)[1]);
        mods = c(ct)[2]; nmods = n(mods) / 2;
        mk_ = ARRAY('1:2'); mx = ARRAY('1:2');
        j = 0; while (j = LT(j, nmods) j + 1) { mk_[j] = v(c(mods)[2 * j - 1]); mx[j] = RkBuildList(c(mods)[2 * j]); }
        RkbStatement(Cur_list, Cur_bk, X, nmods, mk_, mx, semi, last);
        lastl = X;
    }
    RkBlockLast = lastl; RkBlockN = nst; RkLastRealSemi = lrs;
    RkBuildStmtsBk = Cur_list;
    return;
}
function RkBuildBlock(r, bk, lst)  { bk = v(r); lst = RkBuildStmtsBk(r, bk); RkBlkList = lst; if (bk ? (POS(0) ('GIVEN' | 'CATCH') RPOS(0))) { RkBuildBlock = ; return; } RkBuildBlock = RkbBlockSeq(lst, bk); return; }
function RkBlockLastList(r, lst) { lst = RkBuildStmtsBk(r, v(r)); RkBlockLastList = RkBlockLast; return; }
/* ==================================================================================================================== */
/* rkb_block_seq, the phasers (rk_phasers_place, rk_phaser_inline_tree, rk_loop_phasers), rk_tail_value, rkb_program */
/* ==================================================================================================================== */
function RkPhaserOf(x)           { if (IDENT(t(x), 'TT_FNC') (v(x) ? (POS(0) '__rk_phaser_' REM . RkPhaserOf))) { return; } freturn; }
function RkPhaserBody(x)         { RkPhaserBody = (GE(n(x), 2) c(x)[n(x)], mk('TT_SEQ_EXPR', '')); return; }
function RkPhaserIsLoop(p)       { if (p ? (POS(0) ('FIRST' | 'LAST' | 'NEXT' | 'once') RPOS(0))) { return; } freturn; }
function RkPhaserRank(p, ml) {
    if (IDENT(p, 'BEGIN')) { RkPhaserRank = 0; return; }
    if (IDENT(p, 'CHECK')) { RkPhaserRank = 1; return; }
    if (IDENT(p, 'INIT')) { RkPhaserRank = 2; return; }
    if (p ? (POS(0) ('ENTER' | 'PRE') RPOS(0))) { RkPhaserRank = 3; return; }
    if (p ? (POS(0) ('LEAVE' | 'POST') RPOS(0))) { RkPhaserRank = 5; return; }
    if (IDENT(p, 'KEEP')) { RkPhaserRank = (EQ(ml, 1) -1, 6); return; }
    if (IDENT(p, 'UNDO')) { RkPhaserRank = (EQ(ml, 1) 6, -1); return; }
    if (IDENT(p, 'END')) { RkPhaserRank = 7; return; }
    if (IDENT(p, 'TEMP')) { RkPhaserRank = -1; return; }
    if (RkPhaserIsLoop(p)) { RkPhaserRank = -2; return; }
    RkPhaserRank = 4;
    return;
}
function RkPhaserInline(x, i, p) {
    if (IDENT(x)) { return; }
    i = 0;
    while (i = LT(i, n(x)) i + 1) {
        if (p = RkPhaserOf(c(x)[i])) { c(x)[i] = RkPhaserBody(c(x)[i]); }
        RkPhaserInline(c(x)[i]);
    }
    return;
}
function RkPhasersPlace(l, ml, any, i, out, tailret, rank, it, p, r) {
    any = ;
    i = 0; while (i = LT(i, n(l)) i + 1) { if (RkPhaserOf(c(l)[i])) { any = 1; break; } }
    if (IDENT(any)) { RkPhasersPlace = l; return; }
    tailret = ;
    if (EQ(ml, 0) GT(n(l), 0)) { if (IDENT(t(c(l)[n(l)]), 'TT_RETURN')) { tailret = c(l)[n(l)]; n(l) = n(l) - 1; } }
    out = mk('TL', '');
    rank = -1;
    while (rank = LT(rank, 7) rank + 1) {
        i = 0;
        while (i = LT(i, n(l)) i + 1) {
            it = c(l)[i];
            r = 4;
            if (p = RkPhaserOf(it)) { r = RkPhaserRank(p, ml); } else { p = ; }
            if (EQ(r, -1)) { continue; }
            if (EQ(r, -2) EQ(ml, 0)) { if (EQ(rank, 4)) { ad(out, it); } continue; }
            if (EQ(r, -2)) { r = 4; }
            if (~EQ(r, rank)) { continue; }
            ad(out, (DIFFER(p) RkPhaserBody(it), it));
        }
    }
    ad(out, tailret);
    RkPhasersPlace = out;
    return;
}
RkTailStmt = TABLE(61);
RkTailStmt['TT_RETURN'] = 1; RkTailStmt['TT_NRETURN'] = 1; RkTailStmt['TT_PROC_FAIL'] = 1; RkTailStmt['TT_SAY'] = 1; RkTailStmt['TT_SAY_FH'] = 1;
RkTailStmt['TT_PRINT'] = 1; RkTailStmt['TT_PRINT_FH'] = 1; RkTailStmt['TT_IF'] = 1; RkTailStmt['TT_UNLESS'] = 1; RkTailStmt['TT_WHILE'] = 1;
RkTailStmt['TT_UNTIL'] = 1; RkTailStmt['TT_REPEAT'] = 1; RkTailStmt['TT_FOR'] = 1; RkTailStmt['TT_DO_WHILE'] = 1; RkTailStmt['TT_CLOOP'] = 1;
RkTailStmt['TT_EVERY'] = 1; RkTailStmt['TT_CASE'] = 1; RkTailStmt['TT_LOOP_BREAK'] = 1; RkTailStmt['TT_LOOP_NEXT'] = 1; RkTailStmt['TT_SUB_DECL'] = 1;
RkTailStmt['TT_PROC_DECL'] = 1; RkTailStmt['TT_CLASS_DECL'] = 1; RkTailStmt['TT_RECORD_DECL'] = 1; RkTailStmt['TT_DIE'] = 1; RkTailStmt['TT_TRY'] = 1;
RkTailStmt['TT_CATCH'] = 1; RkTailStmt['TT_YADA'] = 1; RkTailStmt['TT_SEQ'] = 1; RkTailStmt['TT_LABEL_DEF'] = 1; RkTailStmt['TT_STMT'] = 1;
RkTailStmt['TT_PROGRAM'] = 1; RkTailStmt['TT_END'] = 1; RkTailStmt['TT_GATHER'] = 1; RkTailStmt['TT_GOTO_S'] = 1; RkTailStmt['TT_GOTO_F'] = 1;
RkTailStmt['TT_GOTO_U'] = 1; RkTailStmt['TT_GOTO_DIRECT'] = 1; RkTailStmt['TT_GLOBAL'] = 1; RkTailStmt['TT_LOCAL'] = 1; RkTailStmt['TT_STATIC_DECL'] = 1;
RkTailStmt['TT_INITIAL'] = 1;
function RkTailValue(l, last) {
    if (EQ(n(l), 0)) { return; }
    last = c(l)[n(l)];
    if (DIFFER(RkTailStmt[t(last)])) { if (~(IDENT(t(last), 'TT_SEQ') EQ(n(last), 2) DIFFER(RkLogAnd[last]))) { return; } }
    c(l)[n(l)] = mk1('TT_RETURN', '', last);
    return;
}
function RkbBlockSeq(list, bk, st, i, tail, l, y) {
    st = mk('TL', '');
    i = 0; while (i = LT(i, n(list)) i + 1) { ad(st, c(list)[i]); }
    tail = ;
    if (IDENT(RkTailList, list) GT(n(st), 0)) { if (IDENT(c(st)[n(st)], RkTailTree)) { tail = c(st)[n(st)]; n(st) = n(st) - 1; RkTailList = ; } }
    if (bk ? (POS(0) ('CLASS' | 'GRAMMAR' | 'MODULE') RPOS(0))) { ad(st, tail); RkbBlockSeq = RkSeqOf(st); return; }
    if ((bk ? (POS(0) ('BLOCK' | 'METHOD' | 'CATCH' | 'GIVEN') RPOS(0))) DIFFER(tail) EQ(n(st), 0)) { if (IDENT(t(tail), 'TT_YADA')) { y = mk('TL', ''); ad(y, tail); RkbBlockSeq = RkSeqOf(y); return; } }
    if (bk ? (POS(0) ('SUB' | 'METHOD') RPOS(0))) {
        if (DIFFER(tail)) { l = RkPhasersPlace(st, 0); ad(l, tail); }
        else { RkTailValue(st); l = RkPhasersPlace(st, 0); }
    } else { l = RkPhasersPlace(st, 0); ad(l, tail); }
    RkbBlockSeq = RkSeqOf(l);
    return;
}
function RkSeqOf(l, q, i)        { q = mk('TT_SEQ_EXPR', ''); i = 0; while (i = LT(i, n(l)) i + 1) { ad(q, c(l)[i]); } RkSeqOf = q; return; }
function RkbProgram(list, all, prog, i, e, st) {
    all = RkPhasersPlace(list, 1);
    prog = mk('TT_PROGRAM', '');
    i = 0;
    while (i = LT(i, n(all)) i + 1) {
        e = c(all)[i];
        if (IDENT(e)) { continue; }
        RkPhaserInline(e);
        st = mk1('TT_STMT', '', mk1('TT_ATTR', ':subj', e));
        ad(prog, st);
    }
    RkbProgram = prog;
    return;
}
function RkPhaserJoin(a, b, l)   { if (IDENT(a)) { RkPhaserJoin = b; return; } l = mk('TL', ''); ad(l, a); ad(l, b); RkPhaserJoin = RkSeqOf(l); return; }
function RkLoopPhasers(loop, body, first, nxt, last, keep, i, p, g, inner, tq, iff, outer, u, b) {
    if (IDENT(body)) { RkLoopPhasers = loop; return; }
    if (~IDENT(t(body), 'TT_SEQ_EXPR')) { RkLoopPhasers = loop; return; }
    first = ; nxt = ; last = ; keep = 0;
    i = 0;
    while (i = LT(i, n(body)) i + 1) {
        b = c(body)[i];
        if (p = RkPhaserOf(b)) {
            if (p ? (POS(0) ('FIRST' | 'once') RPOS(0))) { first = RkPhaserJoin(first, RkPhaserBody(b)); continue; }
            if (IDENT(p, 'NEXT')) { nxt = RkPhaserJoin(nxt, RkPhaserBody(b)); continue; }
            if (IDENT(p, 'LAST')) { last = RkPhaserJoin(last, RkPhaserBody(b)); continue; }
        }
        keep = keep + 1; c(body)[keep] = b;
    }
    if (EQ(keep, n(body))) { RkLoopPhasers = loop; return; }
    g = '__rk_ph_g' RkAfterLine;
    inner = mk('TL', '');
    if (DIFFER(first)) {
        tq = mk('TT_SEQ_EXPR', ''); ad(tq, Bin('TT_ASSIGN', mk('TT_VAR', g), ilit(0))); ad(tq, first);
        iff = mk2('TT_IF', '', mk('TT_VAR', g), tq); ad(inner, iff);
    } else { if (DIFFER(last)) { ad(inner, Bin('TT_ASSIGN', mk('TT_VAR', g), ilit(0))); } }
    i = 0; while (i = LT(i, keep) i + 1) { ad(inner, c(body)[i]); }
    ad(inner, nxt);
    n(body) = 0; c(body) = '';
    i = 0; while (i = LT(i, n(inner)) i + 1) { ad(body, c(inner)[i]); }
    if (IDENT(first) IDENT(last)) { RkLoopPhasers = loop; return; }
    outer = mk('TL', '');
    ad(outer, Bin('TT_ASSIGN', mk('TT_VAR', g), ilit(1)));
    ad(outer, loop);
    if (DIFFER(last)) { u = mk2('TT_UNLESS', '', mk('TT_VAR', g), Seq1(last)); ad(outer, u); }
    RkLoopPhasers = RkSeqOf(outer);
    return;
}
/* ==================================================================================================================== */
/* statement control builders: rkb_if, rkb_unless, rkb_while, rkb_repeat, rkb_loop, rkb_for, rkb_given, rkb_when,    */
/* rkb_default, rkb_catch, rkb_use                                                                                  */
/* ==================================================================================================================== */
function RkBlockOf(sg, stmts)    { RkBlockOf = RkBuildBlock(stmts); rk_pbsig = sg; return; }
function RkBuildControl(nd, tt, i, cs, bs, nn, els, acc, e, cond, blk, sig, lst, until, e1, e2, e3, init, d, sw, nw, cc, u, nm, dot, part, mc, vv, L) {
    tt = t(nd);
    if (IDENT(tt, 'R_IF')) {
        nn = n(nd) / 3; cs = ARRAY('1:' (nn + 1)); bs = ARRAY('1:' (nn + 1)); els = ;
        i = 0;
        while (i = LT(i, nn) i + 1) { cs[i] = RkBuildList(c(nd)[3 * i - 2]); bs[i] = RkBlockOf(c(nd)[3 * i - 1], c(nd)[3 * i]); }
        if (EQ(REMDR(n(nd), 3), 2)) { els = RkBlockOf(c(nd)[n(nd) - 1], c(nd)[n(nd)]); }
        acc = els;
        i = nn + 1;
        while (i = GT(i, 1) i - 1) { e = mk2('TT_IF', '', RkbExpr(cs[i]), bs[i]); ad(e, acc); acc = e; }
        RkBuildControl = acc;
        return;
    }
    if (IDENT(tt, 'R_UNLESS')) { cond = RkBuildList(c(nd)[1]); blk = RkBlockOf(c(nd)[2], c(nd)[3]); RkBuildControl = mk2('TT_UNLESS', '', RkbExpr(cond), blk); return; }
    if (IDENT(tt, 'R_WHILE')) {
        cond = RkBuildList(c(nd)[2]); blk = RkBlockOf(c(nd)[3], c(nd)[4]); RkAfterLine = RkLoopNext();
        if (IDENT(v(c(nd)[1]), 'while')) { RkBuildControl = RkLoopPhasers(Bin('TT_WHILE', RkbExpr(cond), blk), blk); return; }
        RkBuildControl = RkLoopPhasers(mk2('TT_UNTIL', '', RkbExpr(cond), blk), blk);
        return;
    }
    if (IDENT(tt, 'R_REPEATW')) {
        cond = RkBuildList(c(nd)[2]); blk = RkBlockOf(c(nd)[3], c(nd)[4]); RkAfterLine = RkLoopNext();
        e = mk1('TT_REPEAT', '', blk); ad(e, RkbExpr(cond));
        RkBuildControl = RkLoopPhasers(e, blk);
        return;
    }
    if (IDENT(tt, 'R_REPEAT')) {
        blk = RkBlockOf(c(nd)[1], c(nd)[2]); cond = RkBuildList(c(nd)[4]); RkAfterLine = RkLoopNext();
        e = mk1('TT_REPEAT', '', blk); ad(e, RkbExpr(cond));
        RkCtlNs = 1;
        RkBuildControl = RkLoopPhasers(e, blk);
        return;
    }
    if (IDENT(tt, 'R_LOOP')) { blk = RkBuildBlock(c(nd)[1]); RkAfterLine = RkLoopNext(); RkBuildControl = RkLoopPhasers(Bin('TT_WHILE', ilit(1), blk), blk); return; }
    if (IDENT(tt, 'R_CLOOP')) {
        e1 = RkListOrNull(c(nd)[1]); e2 = RkListOrNull(c(nd)[2]); e3 = RkListOrNull(c(nd)[3]);
        blk = RkBuildBlock(c(nd)[4]); RkAfterLine = RkLoopNext();
        init = ;
        if (DIFFER(e1)) { if (EQ(ls_nitem(e1), 1) GT(ls_n(e1), 0) IDENT(tk_kind(el_t0(ls_v(e1)[1])), 'DECL')) {
            d = tk_decl(el_t0(ls_v(e1)[1]));
            init = Bin('TT_ASSIGN', VarNode(dc_name(d)), RkbExpr(dc_init(d)));
        } }
        if (IDENT(init)) { init = RkbExpr(e1); }
        cc = mk('TT_CLOOP', ''); ad(cc, init); ad(cc, RkbExpr(e2)); ad(cc, RkbExpr(e3)); ad(cc, blk);
        RkBuildControl = RkLoopPhasers(cc, blk);
        return;
    }
    if (IDENT(tt, 'R_FOR')) {
        lst = RkBuildList(c(nd)[1]);
        sig = (IDENT(t(c(nd)[2]), 'R_SIG') RkBuildSig(c(nd)[2]), '');
        blk = RkBuildBlock(c(nd)[3]); RkAfterLine = RkLoopNext();
        RkBuildControl = RkbFor(lst, sig, blk);
        return;
    }
    if (IDENT(tt, 'R_GIVEN')) {
        cond = RkBuildList(c(nd)[1]);
        RkBuildBlock(c(nd)[3]);
        cc = mk1('TT_CASE', '', RkbExpr(cond));
        i = 0; while (i = LT(i, n(RkBlkList)) i + 1) { ad(cc, c(RkBlkList)[i]); }
        RkBuildControl = cc;
        return;
    }
    if (IDENT(tt, 'R_WHEN')) {
        cond = RkBuildList(c(nd)[1]); blk = RkBlockOf(c(nd)[2], c(nd)[3]);
        e = RkbExpr(cond);
        if (IDENT(Cur_bk, 'CATCH')) { if (IDENT(t(e), 'TT_VAR') IDENT(v(e), 'X::AdHoc')) { e = mk('TT_NUL', ''); } }
        if (Cur_bk ? (POS(0) ('GIVEN' | 'CATCH') RPOS(0))) { ad(Cur_list, e); ad(Cur_list, blk); }
        else { ad(Cur_list, mk2('TT_IF', '', e, blk)); }
        RkBuildControl = ;
        return;
    }
    if (IDENT(tt, 'R_DEFAULT')) {
        blk = RkBuildBlock(c(nd)[1]);
        if (Cur_bk ? (POS(0) ('GIVEN' | 'CATCH') RPOS(0))) { ad(Cur_list, mk('TT_NUL', '')); ad(Cur_list, blk); } else { ad(Cur_list, blk); }
        RkBuildControl = ;
        return;
    }
    if (IDENT(tt, 'R_CATCH')) {
        sw = RkNonWhen; RkNonWhen = 0;
        RkBuildBlock(c(nd)[1]);
        nw = RkNonWhen; RkNonWhen = sw;
        e = mk('TT_CATCH', '');
        if (EQ(nw, 0) GT(n(RkBlkList), 0)) {
            cc = mk1('TT_CASE', '', mk('TT_VAR', '_'));
            i = 0; while (i = LT(i, n(RkBlkList)) i + 1) { ad(cc, c(RkBlkList)[i]); }
            ad(e, Seq1(cc));
            RkBuildControl = e;
            return;
        }
        ad(e, RkbBlockSeq(RkBlkList, 'BLOCK'));
        RkBuildControl = e;
        return;
    }
    if (IDENT(tt, 'R_USEV')) { RkCtlNs = 1; RkBuildControl = RkbUse(v(nd), ''); return; }
    if (IDENT(tt, 'R_USEM')) { RkCtlNs = 1; L = RkListOrNull(c(nd)[2]); RkBuildControl = RkbUse(v(c(nd)[1]), L); return; }
    RkBuildControl = ;
    return;
}
function RkbUse(nm, args, u, acc, part, mc, rest) {
    nm = RkTrim(RkNamePart(nm));
    u = mk('TT_USE_DECL', '');
    if (nm ? (POS(0) ('v' ANY('0123456789') BREAK('.')) . rk_ub '.' REM . rest)) {
        v(u) = rk_ub;
        acc = VarNode('$_');
        while (rest ? (POS(0) (BREAK('.') | REM) . part) =) {
            mc = mk2('TT_METHCALL', '', acc, mk('TT_QLIT', part)); acc = mc;
            if (~(rest ? (POS(0) '.') =)) { break; }
        }
        ad(u, acc);
        RkbUse = u;
        return;
    }
    v(u) = nm;
    if (DIFFER(args)) { if (GT(ls_nitem(args), 0)) { ad(u, RkbExpr(args)); } }
    RkbUse = u;
    return;
}
function RkbFor(lst, sig, blk, np, nn, vn, r, l, i, gen, vars) {
    np = (DIFFER(sig) n(sig), 0);
    nn = (DIFFER(lst) ls_n(lst), 0);
    vn = ;
    if (EQ(np, 1)) { if (IDENT(t(c(sig)[1]), 'TT_VAR') EQ(n(c(sig)[1]), 0) ~(v(c(sig)[1]) ? (POS(0) ANY('@%')))) { vn = v(c(sig)[1]); } }
    if (DIFFER(vn) EQ(nn, 1) GE(ls_nitem(lst), 2) (ls_rgop(lst) ? (POS(0) ('..' | '..^') RPOS(0))) EQ(ls_rgnitem(lst), ls_nitem(lst))) {
        ElFill(ls_v(lst)[1]);
        r = mk('TT_FOR_RANGE', '');
        ad(r, mk('TT_VAR', vn)); ad(r, ls_rglo(lst));
        ad(r, (IDENT(ls_rgop(lst), '..^') RkDec(ls_rghi(lst)), ls_rghi(lst)));
        ad(r, blk); ad(r, ilit(0));
        RkbFor = RkLoopPhasers(r, blk);
        return;
    }
    if (GT(nn, 1)) { l = MakeCall('__rk_arr'); i = 0; while (i = LT(i, nn) i + 1) { ad(l, ElTree(ls_v(lst)[i])); } }
    else { l = RkbExpr(lst); }
    if (GT(np, 1)) { RkbFor = RkLoopPhasers(RkForMulti(sig, l, blk), blk); return; }
    gen = mk1('TT_ITERATE', '', l);
    if (EQ(np, 1)) { v(gen) = v(c(sig)[1]); }
    RkbFor = RkLoopPhasers(Bin('TT_EVERY', gen, blk), blk);
    return;
}
function RkDec(hi)               { if (IDENT(t(hi), 'TT_ILIT')) { RkDec = ilit(v(hi) - 1); return; } RkDec = Bin('TT_SUB', hi, ilit(1)); return; }
function RkForMulti(vars, lst, body, av, iv, k, hoist, init, el, cond, incr, sq, i, idx, get, outer) {
    av = '__fm_a_' RkFmUid; iv = '__fm_i_' RkFmUid; RkFmUid = RkFmUid + 1;
    k = n(vars);
    if (IDENT(t(lst), 'TT_TO') GE(n(lst), 2)) { lst = RkArrRhs(lst); }
    hoist = Bin('TT_ASSIGN', mk('TT_VAR', av), lst);
    init = Bin('TT_ASSIGN', mk('TT_VAR', iv), ilit(0));
    el = Call1('elems', mk('TT_VAR', av));
    cond = Bin('TT_LT', mk('TT_VAR', iv), el);
    incr = Bin('TT_ASSIGN', mk('TT_VAR', iv), Bin('TT_ADD', mk('TT_VAR', iv), ilit(k)));
    sq = mk('TT_SEQ', '');
    i = 0;
    while (i = LT(i, k) i + 1) {
        idx = mk('TT_VAR', iv);
        if (GT(i, 1)) { idx = Bin('TT_ADD', idx, ilit(i - 1)); }
        get = ad(ad(MakeCall('__rk_arr_at'), mk('TT_VAR', av)), idx);
        ad(sq, Bin('TT_ASSIGN', c(vars)[i], get));
    }
    ad(sq, body);
    outer = mk('TT_SEQ', '');
    ad(outer, hoist);
    cc = mk('TT_CLOOP', ''); ad(cc, init); ad(cc, cond); ad(cc, incr); ad(cc, sq);
    ad(outer, cc);
    RkForMulti = outer;
    return;
}
/* ==================================================================================================================== */
/* Driver: one whole-file read; Init*(), the match, the builders and Pop() on the clock, as the other parsers         */
/* ==================================================================================================================== */
function RkReset() {
    RkNames = TABLE(211); RkNameVal = TABLE(211); RkNameN = 0; RkDepth = 0; RkLimN = 0; RkEndT = TABLE(211); rk_finished = 0;
    RkArrN = TABLE(211); RkSlen = TABLE(211); RkCodeV = TABLE(31); RkAls = TABLE(31); RkPostUid = 0; RkTwPostUid = 0; RkDestrUid = 0; RkFmUid = 0;
    RkNonWhen = 0; RkTailList = ; RkTailTree = ; RkAfterLine = 0; rk_multi = 0; RkLoopQn = 0; RkLoopQi = 0;
    RkLimPush(0);
    RkReset = .dummy;
    nreturn;
}
function RkProgramOf(r, list)    { list = RkBuildStmtsBk(r, 'MAIN'); if (EQ(RkLastRealSemi, 1)) { RkEmpty(list, 'MAIN'); } RkProgramOf = RkbProgram(list); return; }
function RkRawDump(x, lvl, i) {
    if (IDENT(x)) { OUTPUT = DUPL(' ', lvl) '<null>'; return; }
    if (~IDENT(DATATYPE(x), 'tree')) { OUTPUT = DUPL(' ', lvl) '<' DATATYPE(x) '>'; return; }
    OUTPUT = DUPL(' ', lvl) t(x) ' [' v(x) ']';
    i = 0;
    while (i = LT(i, n(x)) i + 1) { RkRawDump(c(x)[i], lvl + 2); }
    return;
}
function ParseOne(ptree, i, n_kids) {
    pf_a = TIME();
    RkReset();
    InitCounter();
    InitStack();
    if (Src ? *Compiland) {
        ptree = Pop();
        if (DIFFER(HOST(4, 'RKDEBUG'))) { RkRawDump(ptree, 0); }
        ptree = RkProgramOf(ptree);
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
