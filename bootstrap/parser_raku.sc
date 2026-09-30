/* parser_raku.sc -- Raku as Snocone patterns (Rakudo 2026.05 src/Raku/Grammar.nqp: comp_unit, statementlist, statement, the
   scope/routine/package declarators, termish and the operator precedence table as a climb, tightest level first).
   THE TREE IS THE ONE THE PATTERN BUILDS IN ORDER (Lon 2026-09-30, RULES.md FACT RULE): every node is made by a raw Shift or
   Reduce at the token that completes it, children left to right, arity through PushCounter/IncCounter/PopCounter and nTop();
   this file defines no function but ParseOne, and no action but Shift, Reduce and the counters. A construct is recorded as
   it is spelled -- `x op= y` is TT_AUGOP, `x++` TT_POSTINC, `for l -> $a { }` TT_FOR, `.foo` a method call on $_ -- and the
   lowerer places and desugars (CEO-1377/1378). The conditional actions are replayed at FLUSH, after each top-level statement,
   in recognition order (Lon 2026-09-30 16:2x): a Shift's value is always a `.`-capture on that same tape, never an immediate.
   Lookahead is a pattern, not a function: @q @r (X @r FAIL | epsilon) *EQ(r, q) succeeds when X does NOT match at the cursor
   and consumes nothing; *NE(r, q) when it does.
   Runtime chain, as every parser_*.sc: global case assign match counter stack tree ShiftReduce tdump gen qize semantic omega trace. */
&FULLSCAN = 1;
E_Parse   = "'Parse'";
wordchars = &UCASE &LCASE '_' digits X1xxxxxxx;
alpha     = ANY(&UCASE &LCASE '_' X1xxxxxxx);
/* ==================================================================================================================== */
/* lookahead patterns: nb_* = the cursor is NOT followed by ..., la_* = it IS followed by ...; nothing is consumed         */
/* ==================================================================================================================== */
nb_word   = @lk_q @lk_r ((ANY(wordchars) | ANY("-'") *alpha) @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_eq     = @lk_q @lk_r ('=' @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_pluseq = @lk_q @lk_r (ANY('+=&|^<>') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_minuseq = @lk_q @lk_r (ANY('-=>') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_stareq = @lk_q @lk_r (ANY('*=') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_slasheq = @lk_q @lk_r (ANY('/=') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_pcteq  = @lk_q @lk_r (ANY('%=') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_tildeq = @lk_q @lk_r (ANY('~=&|^<>') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_ampeq  = @lk_q @lk_r (ANY('&=') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_bareq  = @lk_q @lk_r (ANY('|=') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_careteq = @lk_q @lk_r (ANY('^=.') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_lt     = @lk_q @lk_r (ANY('=<') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_gt     = @lk_q @lk_r (ANY('=>') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_assign = @lk_q @lk_r (ANY('=>:~') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_dot    = @lk_q @lk_r (ANY('.^') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_bang   = @lk_q @lk_r (ANY('=~') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_quest  = @lk_q @lk_r ('?' @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_qw     = @lk_q @lk_r (ANY('=<-') @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
nb_unspace = @lk_q @lk_r (ANY(' ' tab nl cr) @lk_r FAIL | epsilon) *EQ(lk_r, lk_q);
la_lparen = @lk_q @lk_r ('(' @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_rbrace = @lk_q @lk_r ('}' @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_sigil  = @lk_q @lk_r (ANY('$@%&\') @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_type   = @lk_q @lk_r (SPAN(' ' tab) FENCE(ANY('*+:') | epsilon) ANY('$@%&\') @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_fatarrow = @lk_q @lk_r (FENCE(SPAN(' ' tab) | epsilon) '=>' @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_hash   = @lk_q @lk_r (FENCE(SPAN(' ' tab nl) | epsilon) ('}' | FENCE(*ident | *var_tok | "'" *sq_body "'" | '"' *dq_body '"') FENCE(SPAN(' ' tab) | epsilon) '=>') @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_args   = @lk_q @lk_r (SPAN(' ' tab) ( (*name) $ lk_w *IDENT(opword_tbl[lk_w]) | ANY(digits '$@%&"' "'" '([:\') | '{' *EQ(in_cond, 0) | '.' ANY(&UCASE &LCASE '_[<{^' X1xxxxxxx) | ANY('-+!?~^*/') NOTANY(' ' tab '=>.' '-') | '<' NOTANY(' ' tab '=<-') ) @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_args_kw = @lk_q @lk_r (SPAN(' ' tab) ( (*name) $ lk_w *IDENT(opword_tbl[lk_w]) | ANY(digits '$@%&"' "'" '([:\') | '{' | '.' ANY(&UCASE &LCASE '_[<{^' X1xxxxxxx) | ANY('-+!?~^*/') NOTANY(' ' tab '=>.' '-') | '<' NOTANY(' ' tab '=<-') ) @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_end    = @lk_q @lk_r (FENCE(SPAN(' ' tab) | epsilon) (ANY(';}),' nl cr) | '#' | RPOS(0)) @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_stmt_end = @lk_q @lk_r (FENCE(SPAN(' ' tab) | epsilon) (ANY(';}' nl cr) | '#') @lk_r FAIL | RPOS(0) @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
la_bol    = @lk_q *EQ(lk_q, 0) | @lk_q *IDENT(SUBSTR(Src, lk_q, 1), nl);
/* ==================================================================================================================== */
/* token ws: blanks, comments (# and #`( )), pod (=begin/=end, =finish, =word ... blank line), unspace                    */
/* ==================================================================================================================== */
pod_end  = BREAK(nl) nl ARBNO(NOTANY(nl) BREAK(nl) nl | nl) '=end' (BREAK(nl) nl | REM);
pod      = la_bol FENCE(SPAN(' ' tab) | epsilon) '=' ( 'finish' REM
                                                     | 'begin' *pod_end
                                                     | *alpha (BREAK(nl) | REM) FENCE(ARBNO(nl NOTANY(nl) BREAK(nl)) | epsilon) );
white    = ( SPAN(' ' tab nl cr) | '#`(' BREAK(')') ')' | '#`[' BREAK(']') ']' | '#' (BREAK(nl) | REM) | *pod | bSlash SPAN(' ' tab nl cr) );
White    = *white FENCE(*White | epsilon);
Gray     = FENCE(*White | epsilon);
$' '     = Gray;
$'  '    = White;
/* ==================================================================================================================== */
/* names and variables (token identifier, name, variable): a variable is one TT_VAR carrying its whole spelling -- sigil,  */
/* twigil and name ($x @a %h &f $.x $!x $*OUT $_ $/ $! $0 $<name>); a sigilless declaration's \name is TT_VAR name          */
/* ==================================================================================================================== */
ident    = *alpha FENCE(SPAN(wordchars) | epsilon) FENCE(*ident_more | epsilon);
ident_more = ('-' | "'") *alpha FENCE(SPAN(wordchars) | epsilon) FENCE(*ident_more | epsilon);
name     = *ident FENCE(*name_more | epsilon);
name_more = '::' *ident FENCE(*name_more | epsilon);
sigil    = ANY('$@%&');
twigil   = ANY('.!*?^:');
var_body = *sigil FENCE(*twigil *name | *name | ANY('_/!') | SPAN(digits) | '<' BREAK('>') '>' | '[' BREAK(']') ']');
var_tok  = ( ((*var_body) $ dv_tx) . thx . *Shift('TT_VAR', thx)
           | bSlash ((*name) $ dv_tx *DIFFER(const_tbl[dv_tx] = 1)) . thx . *Shift('TT_VAR', thx)
           );
/* ==================================================================================================================== */
/* values (token value:sym<number>, quote:sym<apos>, quote:sym<dblq>, quote:sym<< < > >>): a number is TT_ILIT or TT_FLIT   */
/* with its spelling (underscore groups joined; a radix literal keeps its 0x/0b/0o spelling for the lowerer); a string is   */
/* its literal runs, decoded single-character escapes and interpolations, each Shifted as it is read and every piece after */
/* the first joined by TT_CAT, so "a$x" is (TT_CAT (TT_QLIT "a") (TT_VAR $x)); a bracketed escape is TT_ESC raw           */
/* ==================================================================================================================== */
number   = ( ('0' ANY('xX') SPAN(hex_digits '_') | '0' ANY('bB') SPAN('01_') | '0' ANY('oO') SPAN('01234567_')) . thx . *Shift('TT_ILIT', thx)
           | (SPAN(digits '_') '.' SPAN(digits) FENCE(ANY('eE') FENCE(ANY('+-') | epsilon) SPAN(digits) | epsilon)
             | SPAN(digits '_') ANY('eE') FENCE(ANY('+-') | epsilon) SPAN(digits)) . thx . *Shift('TT_FLIT', thx)
           | SPAN(digits) . n1x FENCE('_' SPAN(digits) . n2x FENCE('_' SPAN(digits) . n3x FENCE('_' SPAN(digits) . n4x | epsilon . n4x) | epsilon . n3x . n4x) | epsilon . n2x . n3x . n4x) . *Shift('TT_ILIT', n1x n2x n3x n4x)
           );
esc_tbl = TABLE(19);
esc_tbl['n'] = nl; esc_tbl['t'] = tab; esc_tbl['r'] = cr; esc_tbl['a'] = CHAR(7); esc_tbl['b'] = bs; esc_tbl['e'] = CHAR(27); esc_tbl['f'] = ff; esc_tbl['0'] = nul;
esc_tbl[bSlash] = bSlash; esc_tbl['"'] = '"'; esc_tbl['{'] = '{'; esc_tbl['}'] = '}'; esc_tbl["'"] = "'"; esc_tbl['$'] = '$'; esc_tbl['@'] = '@'; esc_tbl['%'] = '%'; esc_tbl['&'] = '&'; esc_tbl['<'] = '<'; esc_tbl['>'] = '>';
sq_body  = ARBNO( NOTANY("'" bSlash) | bSlash LEN(1) );
dq_body  = ARBNO( NOTANY('"' bSlash) | bSlash LEN(1) );
sq_piece = FENCE( (NOTANY(bSlash "'") FENCE(BREAK(bSlash "'") | REM)) . thx . *Shift('TT_QLIT', thx)
                | bSlash (ANY(bSlash "'")) . thx . *Shift('TT_QLIT', thx)
                | (bSlash) . thx . *Shift('TT_QLIT', thx) );
sq_more  = FENCE( *sq_piece . *Reduce('TT_CAT', 2) *sq_more | epsilon );
sq_str   = "'" FENCE(*sq_piece *sq_more | epsilon . *Shift('TT_QLIT', '')) "'";
dq_sub   = FENCE( '[' *$' ' *item *$' ' ']' . *Reduce('TT_ARR_GET', 2) *dq_sub
                | '<' (BREAK('>')) . thx '>' . *Shift('TT_QLIT', thx) . *Reduce('TT_HASH_GET', 2) *dq_sub
                | '{' *$' ' *item *$' ' '}' . *Reduce('TT_HASH_GET', 2) *dq_sub
                | '.' (*ident) . thx la_lparen . *Shift('TT_QLIT', thx) . *PushCounter() *paren_args . *Reduce('TT_METHCALL', nTop() + 2) . *PopCounter() *dq_sub
                | epsilon );
dq_piece = FENCE( (NOTANY('"' bSlash '$@{') FENCE(BREAK('"' bSlash '$@{') | REM)) . thx . *Shift('TT_QLIT', thx)
                | bSlash (ANY('xc') '[' BREAK(']') ']' | 'x' SPAN(hex_digits)) . thx . *Shift('TT_ESC', thx)
                | bSlash (ANY('ntrabef0' bSlash '"{}' "'" '$@%&<>')) . thx . *Shift('TT_QLIT', esc_tbl[thx])
                | (bSlash LEN(1)) . thx . *Shift('TT_QLIT', thx)
                | la_dq_var *var_tok *dq_sub
                | *block
                | (ANY('$@{')) . thx . *Shift('TT_QLIT', thx) );
la_dq_var = @lk_q @lk_r ('$' ANY(wordchars '.!*?^:/<[') @lk_r FAIL | '@' *ident ANY('[.') @lk_r FAIL | epsilon) *NE(lk_r, lk_q);
dq_more  = FENCE( *dq_piece . *Reduce('TT_CAT', 2) *dq_more | epsilon );
dq_str   = '"' FENCE(*dq_piece *dq_more | epsilon . *Shift('TT_QLIT', '')) '"';
qq_piece = FENCE( (NOTANY(']' bSlash '$@{') FENCE(BREAK(']' bSlash '$@{') | REM)) . thx . *Shift('TT_QLIT', thx)
                | bSlash (ANY('ntrabef0' bSlash '"{}' "'" '$@%&<>')) . thx . *Shift('TT_QLIT', esc_tbl[thx])
                | (bSlash LEN(1)) . thx . *Shift('TT_QLIT', thx)
                | la_dq_var *var_tok *dq_sub
                | *block
                | (ANY('$@{')) . thx . *Shift('TT_QLIT', thx) );
qq_more  = FENCE( *qq_piece . *Reduce('TT_CAT', 2) *qq_more | epsilon );
string   = ( *sq_str
           | *dq_str
           | 'qq' '[' FENCE(*qq_piece *qq_more | epsilon . *Shift('TT_QLIT', '')) ']'
           | 'q' FENCE(':w' | epsilon) '[' (BREAK(']')) . thx ']' . *Shift('TT_QLIT', thx)
           | 'Q' '[' (BREAK(']')) . thx ']' . *Shift('TT_QLIT', thx)
           );
word     = (NOTANY(' ' tab nl '>') FENCE(BREAK(' ' tab nl '>') | REM)) . thx . *Shift('TT_QLIT', thx) . *IncCounter();
words    = FENCE(SPAN(' ' tab nl) | epsilon) FENCE(*word FENCE(SPAN(' ' tab nl) | epsilon) *words | epsilon);
qwords   = '<' nb_qw . *PushCounter() *words '>' . *Reduce('TT_QWORDS', nTop()) . *PopCounter();
/* a regex is its text, raw (the lowerer's translation), its adverbs as the node's value; s/// is TT_SUBST pattern replacement */
rx_body  = ARBNO(NOTANY('/' bSlash) | bSlash LEN(1));
rx_adv   = (ARBNO(':' *ident)) . adv;
regex    = ( '/' (*rx_body) . thx '/' . *Shift('TT_QLIT', thx) . *Reduce('TT_REGEX', 1)
           | ('rx' | 'm') *rx_adv '/' (*rx_body) . thx '/' . *Shift('TT_QLIT', thx) . *Reduce('TT_REGEX', 1, adv)
           | 's' *rx_adv '/' (*rx_body) . thx '/' . *Shift('TT_QLIT', thx) (*rx_body) . thx '/' . *Shift('TT_QLIT', thx) . *Reduce('TT_SUBST', 2, adv)
           );
/* ==================================================================================================================== */
/* operator tokens, longest spelling first, each ruling out the spellings it prefixes; word operators need word ends      */
/* ==================================================================================================================== */
op_pow   = *$' ' '**' nb_eq *$' ';
op_mul   = *$' ' '*' nb_stareq *$' ';                        op_div = *$' ' '/' nb_slasheq *$' ';
op_mod   = *$' ' '%' nb_pcteq *$' ';                         op_divis = *$' ' '%%' nb_eq *$' ';
op_add   = *$' ' '+' nb_pluseq *$' ';                        op_sub = *$' ' '-' nb_minuseq *$' ';
op_cat   = *$' ' '~' nb_tildeq *$' ';
op_jand  = *$' ' '&' nb_ampeq *$' ';                         op_setdiff = *$' ' '∖' *$' ';                         op_jor = *$' ' '|' nb_bareq *$' ';    op_jxor = *$' ' '^' nb_careteq *$' ';
op_eq    = *$' ' '==' nb_eq *$' ';                           op_ne  = *$' ' '!=' nb_eq *$' ';
op_lt    = *$' ' '<' nb_lt *$' ';                            op_gt  = *$' ' '>' nb_gt *$' ';
op_le    = *$' ' '<=' nb_gt *$' ';                           op_ge  = *$' ' '>=' *$' ';
op_id    = *$' ' '===' *$' ';                                op_cmp3 = *$' ' '<=>' *$' ';
op_smatch = *$' ' '~~' *$' ';                                op_nsmatch = *$' ' '!~~' *$' ';
op_tand  = *$' ' '&&' *$' ';                                 op_tor = *$' ' '||' *$' ';    op_dor = *$' ' '//' nb_eq *$' ';   op_txor = *$' ' '^^' *$' ';
op_assign = *$' ' '=' nb_assign *$' ';
op_bind  = *$' ' ':=' *$' ';                                 op_pair = *$' ' '=>' *$' ';
op_range = *$' ' '..' nb_dot *$' ';                          op_rangex = *$' ' '..^' *$' ';
op_xrange = *$' ' '^..' nb_dot *$' ';                        op_xrangex = *$' ' '^..^' *$' ';
op_seq   = *$' ' '...' *$' ';
op_tern1 = *$' ' '??' *$' ';                                 op_tern2 = *$' ' '!!' *$' ';
op_xrep  = *$'  ' 'x' nb_word *$'  ';                        op_xx  = *$'  ' 'xx' nb_word *$'  ';
op_leq   = *$'  ' 'eq' nb_word *$'  ';                       op_lne = *$'  ' 'ne' nb_word *$'  ';
op_llt   = *$'  ' 'lt' nb_word *$'  ';                       op_lgt = *$'  ' 'gt' nb_word *$'  ';
op_lle   = *$'  ' 'le' nb_word *$'  ';                       op_lge = *$'  ' 'ge' nb_word *$'  ';
op_eqv   = *$'  ' 'eqv' nb_word *$'  ';                      op_cmp = *$'  ' 'cmp' nb_word *$'  ';   op_leg = *$'  ' 'leg' nb_word *$'  ';
op_min   = *$'  ' 'min' nb_word *$'  ';                      op_max = *$'  ' 'max' nb_word *$'  ';
op_land  = *$'  ' 'and' nb_word *$'  ';                      op_lor = *$'  ' 'or' nb_word *$'  ';    op_lxor = *$'  ' 'xor' nb_word *$'  ';
op_andthen = *$'  ' 'andthen' nb_word *$'  ';                op_orelse = *$'  ' 'orelse' nb_word *$'  ';
op_div_i = *$'  ' 'div' nb_word *$'  ';                      op_mod_w = *$'  ' 'mod' nb_word *$'  ';
op_gcd   = *$'  ' 'gcd' nb_word *$'  ';                      op_lcm = *$'  ' 'lcm' nb_word *$'  ';
op_band  = *$' ' '+&' nb_eq *$' ';                           op_shl = *$' ' '+<' nb_eq *$' ';   op_shr = *$' ' '+>' nb_eq *$' ';   op_sband = *$' ' '~&' nb_eq *$' ';
op_bor   = *$' ' '+|' nb_eq *$' ';                           op_bxor = *$' ' '+^' nb_eq *$' ';  op_sbor = *$' ' '~|' nb_eq *$' ';
kw_X     = *$'  ' 'X' nb_word *$'  ';                        kw_Z = *$'  ' 'Z' nb_word *$'  ';
/* keywords: the word and a word end; the same shape as the other grammars' *Id $ tx *IDENT(tx, 'if') */
kw_if = 'if' nb_word;  kw_unless = 'unless' nb_word;  kw_while = 'while' nb_word;  kw_until = 'until' nb_word;  kw_for = 'for' nb_word;  kw_loop = 'loop' nb_word;  kw_repeat = 'repeat' nb_word;
kw_given = 'given' nb_word;  kw_when = 'when' nb_word;  kw_default = 'default' nb_word;  kw_else = 'else' nb_word;  kw_elsif = 'elsif' nb_word;  kw_CATCH = 'CATCH' nb_word;  kw_use = 'use' nb_word;
kw_my = 'my' nb_word;  kw_our = 'our' nb_word;  kw_state = 'state' nb_word;  kw_has = 'has' nb_word;  kw_constant = 'constant' nb_word;  kw_multi = 'multi' nb_word;  kw_proto = 'proto' nb_word;  kw_only = 'only' nb_word;
kw_sub = 'sub' nb_word;  kw_method = 'method' nb_word;  kw_submethod = 'submethod' nb_word;  kw_class = 'class' nb_word;  kw_role = 'role' nb_word;  kw_grammar = 'grammar' nb_word;  kw_module = 'module' nb_word;
kw_token = 'token' nb_word;  kw_rule = 'rule' nb_word;  kw_regex = 'regex' nb_word;  kw_is = 'is' nb_word;  kw_does = 'does' nb_word;  kw_handles = 'handles' nb_word;  kw_returns = 'returns' nb_word;  kw_of = 'of' nb_word;
kw_gather = 'gather' nb_word;  kw_try = 'try' nb_word;  kw_do = 'do' nb_word;  kw_not = 'not' nb_word;  kw_so = 'so' nb_word;  kw_with = 'with' nb_word;  kw_without = 'without' nb_word;  kw_enum = 'enum' nb_word;
kw_no = 'no' nb_word;  kw_whenever = 'whenever' nb_word;  kw_need = 'need' nb_word;  kw_require = 'require' nb_word;  kw_import = 'import' nb_word;  kw_unit = 'unit' nb_word;
/* words that are operators or statement heads, never the head of a listop call; declared constants and enum values join them */
opword_tbl = TABLE(64);
opword_tbl['x'] = 1; opword_tbl['xx'] = 1; opword_tbl['eq'] = 1; opword_tbl['ne'] = 1; opword_tbl['lt'] = 1; opword_tbl['gt'] = 1; opword_tbl['le'] = 1; opword_tbl['ge'] = 1; opword_tbl['div'] = 1;
opword_tbl['mod'] = 1; opword_tbl['and'] = 1; opword_tbl['or'] = 1; opword_tbl['cmp'] = 1; opword_tbl['leg'] = 1; opword_tbl['eqv'] = 1; opword_tbl['gcd'] = 1;
opword_tbl['lcm'] = 1; opword_tbl['min'] = 1; opword_tbl['max'] = 1; opword_tbl['is'] = 1; opword_tbl['does'] = 1; opword_tbl['xor'] = 1; opword_tbl['andthen'] = 1; opword_tbl['orelse'] = 1; opword_tbl['but'] = 1;
opword_tbl['X'] = 1; opword_tbl['Z'] = 1; opword_tbl['with'] = 1; opword_tbl['without'] = 1; opword_tbl['if'] = 1; opword_tbl['unless'] = 1; opword_tbl['while'] = 1; opword_tbl['until'] = 1; opword_tbl['for'] = 1;
opword_tbl['given'] = 1; opword_tbl['when'] = 1; opword_tbl['else'] = 1; opword_tbl['elsif'] = 1; opword_tbl['default'] = 1; opword_tbl['returns'] = 1; opword_tbl['of'] = 1; opword_tbl['handles'] = 1;
const_tbl = TABLE(256);
const_tbl['pi'] = 1; const_tbl['e'] = 1; const_tbl['tau'] = 1; const_tbl['i'] = 1; const_tbl['Inf'] = 1; const_tbl['NaN'] = 1; const_tbl['Nil'] = 1; const_tbl['Empty'] = 1; const_tbl['True'] = 1; const_tbl['False'] = 1;
const_tbl['self'] = 1; const_tbl['now'] = 1; const_tbl['time'] = 1; const_tbl['rand'] = 1; const_tbl['Any'] = 1; const_tbl['Mu'] = 1; const_tbl['Int'] = 1; const_tbl['Str'] = 1; const_tbl['Num'] = 1; const_tbl['Bool'] = 1;
const_tbl['Rat'] = 1; const_tbl['Array'] = 1; const_tbl['Hash'] = 1; const_tbl['List'] = 1; const_tbl['Order'] = 1; const_tbl['Less'] = 1; const_tbl['Same'] = 1; const_tbl['More'] = 1; const_tbl['Whatever'] = 1;
/* the listops whose calls are their own node kinds, flat arguments (say print die take return next last sort map grep reverse) */
listop_tbl = TABLE(16);
listop_tbl['say'] = 'TT_SAY'; listop_tbl['print'] = 'TT_PRINT'; listop_tbl['die'] = 'TT_DIE'; listop_tbl['return'] = 'TT_RETURN'; listop_tbl['take'] = 'TT_SUSPEND'; listop_tbl['next'] = 'TT_LOOP_NEXT';
listop_tbl['last'] = 'TT_LOOP_BREAK'; listop_tbl['sort'] = 'TT_SORT'; listop_tbl['map'] = 'TT_MAP'; listop_tbl['grep'] = 'TT_GREP'; listop_tbl['reverse'] = 'TT_REVERSE';
/* ==================================================================================================================== */
/* lists and arguments: a comma list is TT_LIST of its items (a lone item stays itself); a call's arguments are flat        */
/* ==================================================================================================================== */
list_tail = FENCE( *$' ' ',' *$' ' FENCE(*item . *IncCounter() *list_tail | epsilon) | epsilon );
list_expr = epsilon . *PushCounter() *item . *IncCounter() FENCE( *$' ' ',' *$' ' FENCE(*item . *IncCounter() | epsilon) *list_tail . *Reduce('TT_LIST', nTop()) | epsilon ) . *PopCounter();
list_items = *item . *IncCounter() *list_tail;
paren_args = '(' *$' ' FENCE(*list_items *args_cross | epsilon) *$' ' ')';
args_cross = FENCE( *kw_X . *Reduce('TT_LIST', nTop()) . *PopCounter() . *PushCounter() . *IncCounter() *list_expr . *Reduce('TT_CROSS', 2) *args_cross
                  | *kw_Z . *Reduce('TT_LIST', nTop()) . *PopCounter() . *PushCounter() . *IncCounter() *list_expr . *Reduce('TT_ZIP', 2) *args_cross
                  | *op_seq . *Reduce('TT_LIST', nTop()) . *PopCounter() . *PushCounter() . *IncCounter() *list_expr . *Reduce('TT_SEQOP', 2) *args_cross
                  | epsilon );
/* the arguments of a word call: (args), or a blank then a term (never an infix -- `f - 1` subtracts, `f -1` calls), or none */
word_args = FENCE( *paren_args | la_args_kw *$'  ' *list_items *args_cross | epsilon );
/* ==================================================================================================================== */
/* blocks: { statements } is TT_SEQ_EXPR of its statements; as a term, { } or -> sig { } is TT_ANON_BLOCK (sig-or-NUL, body)  */
/* ==================================================================================================================== */
block    = '{' *DIFFER(in_cond = 0) *$' ' . *PushCounter() *stmts *$' ' '}' . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter();
stmts    = ARBNO( FENCE(*$' ' *statement . *IncCounter()) );
signature = '(' *$' ' . *PushCounter() FENCE(*params | epsilon) *$' ' FENCE('-->' *$' ' (*name) . thx *$' ' . *Shift('TT_TRAIT', 'returns ' thx) . *IncCounter() | epsilon) ')' . *Reduce('TT_SIGNATURE', nTop()) . *PopCounter();
sig_opt  = FENCE( *signature *$' ' | epsilon . *Reduce('TT_NUL', 0) );
params   = *param . *IncCounter() FENCE( *$' ' ',' *$' ' FENCE(*params | epsilon) | epsilon );
type_nm  = ( (*name FENCE(':' ANY('DU_') | epsilon) FENCE('(' *$' ' ')' | epsilon)) . thx la_type *$'  ' . *Shift('TT_TYPE', thx) );
type_opt = FENCE( *type_nm | epsilon . *Reduce('TT_NUL', 0) );
param_var = ( ('|' *ident) . thx . *Shift('TT_VAR', thx)
            | (FENCE('**' | '*' | '+' | ':' | epsilon) (*var_body | bSlash *name | bSlash | *sigil) FENCE(ANY('?!') | epsilon)) . thx . *Shift('TT_VAR', thx) );
param    = *type_opt FENCE(*param_var | *signature) *$' ' FENCE( *op_assign *cond_expr | epsilon . *Reduce('TT_NUL', 0) ) *traits . *Reduce('TT_PARAM', 4);
trait    = ( *kw_is *$'  ' (*ident FENCE('(' BREAK(')') ')' | epsilon)) . thx . *Shift('TT_TRAIT', 'is ' thx)
           | *kw_does *$'  ' (*name) . thx . *Shift('TT_TRAIT', 'does ' thx)
           | *kw_returns *$'  ' (*name) . thx . *Shift('TT_TRAIT', 'returns ' thx)
           | *kw_of *$'  ' (*name) . thx . *Shift('TT_TRAIT', 'of ' thx)
           | *kw_handles *$'  ' ('<' BREAK('>') '>' | *ident) . thx . *Shift('TT_TRAIT', 'handles ' thx)
           | 'where' nb_word *$'  ' *cond_expr . *Reduce('TT_TRAIT', 1, 'where') );
trait_more = FENCE( *$' ' *trait . *IncCounter() *trait_more | epsilon );
traits   = epsilon . *PushCounter() *trait_more . *Reduce('TT_TRAITS', nTop()) . *PopCounter();
pointy   = '->' *$' ' . *PushCounter() FENCE(*params | epsilon) *$' ' . *Reduce('TT_SIGNATURE', nTop()) . *PopCounter() *block . *Reduce('TT_ANON_BLOCK', 2);
anon_block = epsilon . *Reduce('TT_NUL', 0) *block . *Reduce('TT_ANON_BLOCK', 2);
hash_lit = '{' la_hash *$' ' . *PushCounter() FENCE(*list_items | epsilon) *$' ' FENCE(',' *$' ' | epsilon) '}' . *Reduce('TT_HASH', nTop()) . *PopCounter();
block_term = ( *pointy | *hash_lit | *anon_block );
/* ==================================================================================================================== */
/* terms (token termish, term:sym<...>): variables, values, calls, names, circumfixes, blocks, regexes, reductions -- a     */
/* listop whose call is its own kind (say print die return take next last sort map grep reverse) reduces flat; any other  */
/* word with arguments is TT_FNC (TT_VAR f) args...; a bare word is TT_VAR; .foo is a method call on $_                   */
/* ==================================================================================================================== */
colonpair = ( ':' (SPAN(digits)) . thx . *Shift('TT_ILIT', thx) FENCE( '[' *$' ' *expr *$' ' ']' . *Reduce('TT_RADIX', 2) | '(' *$' ' *expr *$' ' ')' . *Reduce('TT_RADIX', 2) | '<' (BREAK('>')) . thx '>' . *Shift('TT_QLIT', thx) . *Reduce('TT_RADIX', 2) )
            | ':!' (*ident) . thx . *Shift('TT_QLIT', thx) . *Reduce('TT_COLONPAIR', 1, 'not')
            | ':' (*ident) . thx . *Shift('TT_QLIT', thx) FENCE( '(' *$' ' *expr *$' ' ')' . *Reduce('TT_COLONPAIR', 2)
                                                             | '<' (BREAK('>')) . thx '>' . *Shift('TT_QLIT', thx) . *Reduce('TT_COLONPAIR', 2)
                                                             | epsilon . *Reduce('TT_COLONPAIR', 1) )
            | ':' *var_tok . *Reduce('TT_COLONPAIR', 1, 'var') );
topic_call = ( '.' . *Shift('TT_VAR', '$_') FENCE( (FENCE('^' | epsilon) *ident) . thx . *Shift('TT_QLIT', thx) . *PushCounter() *meth_args . *Reduce('TT_METHCALL', nTop() + 2) . *PopCounter()
                                                 | '[' *$' ' *expr *$' ' ']' . *Reduce('TT_ARR_GET', 2)
                                                 | '<' (BREAK('>')) . thx '>' . *Shift('TT_QLIT', thx) . *Reduce('TT_HASH_GET', 2)
                                                 | '{' *$' ' *expr *$' ' '}' . *Reduce('TT_HASH_GET', 2) ) );
meth_args = FENCE( *paren_args | ':' la_args *$'  ' *list_items *args_cross | epsilon );
reduce_op = '[' (ANY('+-*~') | 'min' | 'max' | '<' | '>' | 'X' | 'Z' | '&&' | '||' | '&' | '|' | 'lcm' | 'gcd' | 'eq' | '+&' | '+|') . thx ']' *$' ' . *Shift('TT_QLIT', thx) *rng_expr . *Reduce('TT_REDUCE', 2);
paren    = '(' *$' ' FENCE( ')' . *Reduce('TT_LIST', 0) | *expr *$' ' ')' ) *DIFFER(dv_tx = '@');
bracket  = '[' *$' ' . *PushCounter() FENCE(*list_items | epsilon) *$' ' FENCE(',' *$' ' | epsilon) ']' . *Reduce('TT_ARRAY', nTop()) . *PopCounter();
listop_kind = ( 'say' nb_word . *PushCounter() *word_args . *Reduce('TT_SAY', nTop()) . *PopCounter()
              | 'print' nb_word . *PushCounter() *word_args . *Reduce('TT_PRINT', nTop()) . *PopCounter()
              | 'die' nb_word . *PushCounter() *word_args . *Reduce('TT_DIE', nTop()) . *PopCounter()
              | 'return' nb_word . *PushCounter() *word_args . *Reduce('TT_RETURN', nTop()) . *PopCounter()
              | 'take' nb_word . *PushCounter() *word_args . *Reduce('TT_SUSPEND', nTop()) . *PopCounter()
              | 'next' nb_word . *PushCounter() *word_args . *Reduce('TT_LOOP_NEXT', nTop()) . *PopCounter()
              | 'last' nb_word . *PushCounter() *word_args . *Reduce('TT_LOOP_BREAK', nTop()) . *PopCounter()
              | 'sort' nb_word . *PushCounter() *word_args . *Reduce('TT_SORT', nTop()) . *PopCounter()
              | 'map' nb_word . *PushCounter() *word_args . *Reduce('TT_MAP', nTop()) . *PopCounter()
              | 'grep' nb_word . *PushCounter() *word_args . *Reduce('TT_GREP', nTop()) . *PopCounter()
              | 'reverse' nb_word . *PushCounter() *word_args . *Reduce('TT_REVERSE', nTop()) . *PopCounter() );
word_call = ( (*ident) . thx la_fatarrow . *Shift('TT_QLIT', thx)
            | *listop_kind
            | (*name) . thx la_lparen . *Shift('TT_VAR', thx) . *PushCounter() *paren_args . *Reduce('TT_FNC', nTop() + 1) . *PopCounter()
            | ((*ident) $ tx *IDENT(opword_tbl[tx]) *IDENT(const_tbl[tx])) . thx la_args . *Shift('TT_VAR', thx) . *PushCounter() *$'  ' *list_items *args_cross . *Reduce('TT_FNC', nTop() + 1) . *PopCounter()
            | (*name) . thx . *Shift('TT_VAR', thx) );
key_term = ( *kw_do *$'  ' . *Shift('TT_QLIT', 'do') FENCE( *block | *control_core ) . *Reduce('TT_STMT_PREFIX', 2)
           | (('once' | 'quietly' | 'react' | 'supply' | 'start' | 'lazy' | 'eager' | 'sink' | 'hyper' | 'race') nb_word) . thx *$' ' . *Shift('TT_QLIT', thx) FENCE( *block | *item ) . *Reduce('TT_STMT_PREFIX', 2)
           | 'await' nb_word *$' ' *item . *Reduce('TT_AWAIT', 1)
           | *kw_sub *$' ' . *Reduce('TT_QUAL', 0) . *Reduce('TT_NUL', 0) *sig_opt *traits *$' ' *block . *Reduce('TT_SUB_DECL', 5)
           | *kw_gather *$' ' FENCE( *block | *control_core ) . *Reduce('TT_GATHER', 1)
           | *kw_try *$' ' *block . *Reduce('TT_TRY', 1)
           | *kw_not *$' ' *item . *Reduce('TT_NOT', 1)
           | *kw_so *$' ' *item . *Reduce('TT_BOOL', 1)
           | *scope_decl
           | '^' *$' ' *pow_expr . *Reduce('TT_UPTO', 1)
           | '|' *postfix_expr . *Reduce('TT_FLATTEN', 1)
           | '$' '(' *$' ' *expr *$' ' ')' . *Reduce('TT_CONTEXT', 1, '$')
           | '@' '(' *$' ' *expr *$' ' ')' . *Reduce('TT_CONTEXT', 1, '@')
           | '%' '(' *$' ' *expr *$' ' ')' . *Reduce('TT_CONTEXT', 1, '%')
           | '$' la_sigil *postfix_expr . *Reduce('TT_CONTEXT', 1, '$')
           | '@' la_sigil *postfix_expr . *Reduce('TT_CONTEXT', 1, '@')
           | '%' la_sigil *postfix_expr . *Reduce('TT_CONTEXT', 1, '%')
           | '*' . *Shift('TT_VAR', '*')
           );
term     = ( *colonpair
           | *topic_call
           | *var_tok
           | *number
           | *string
           | *qwords
           | *reduce_op
           | *regex
           | *paren
           | *bracket
           | *block_term
           | *key_term
           | *word_call
           );
/* postfixes: method call, subscripts, call parens, ++ --, :exists :delete -- adjacent to the term (Grammar.nqp: no ws before them) */
unspace  = FENCE( bSlash SPAN(' ' tab nl cr) | epsilon );
postfix  = *unspace FENCE( ('>>.' | '».') (FENCE('^' | epsilon) *ident) . thx . *Shift('TT_QLIT', thx) . *PushCounter() *meth_args . *Reduce('TT_HYPERMETH', nTop() + 2) . *PopCounter() *postfix
                | '.' (FENCE('^' | epsilon) *ident) . thx . *Shift('TT_QLIT', thx) . *PushCounter() *meth_args . *Reduce('TT_METHCALL', nTop() + 2) . *PopCounter() *postfix
                | ('!' *ident) . thx . *Shift('TT_QLIT', thx) . *PushCounter() *meth_args . *Reduce('TT_METHCALL', nTop() + 2) . *PopCounter() *postfix
                | '.' '[' *$' ' *expr *$' ' ']' . *Reduce('TT_ARR_GET', 2) *postfix
                | '.' '<' (BREAK('>')) . thx '>' . *Shift('TT_QLIT', thx) . *Reduce('TT_HASH_GET', 2) *postfix
                | '.' '{' *$' ' *expr *$' ' '}' . *Reduce('TT_HASH_GET', 2) *postfix
                | '[' *$' ' ']' . *Reduce('TT_NUL', 0) . *Reduce('TT_ARR_GET', 2) *postfix
                | '[' *$' ' *expr *$' ' ']' . *Reduce('TT_ARR_GET', 2) *postfix
                | '<' (BREAK('>')) . thx '>' . *Shift('TT_QLIT', thx) . *Reduce('TT_HASH_GET', 2) *postfix
                | '{' *$' ' *expr *$' ' '}' . *Reduce('TT_HASH_GET', 2) *postfix
                | '(' *$' ' . *PushCounter() FENCE(*list_items *args_cross | epsilon) *$' ' ')' . *Reduce('TT_INVOKE', nTop() + 1) . *PopCounter() *postfix
                | '++' . *Reduce('TT_POSTINC', 1) *postfix
                | '--' . *Reduce('TT_POSTDEC', 1) *postfix
                | ':exists' . *Reduce('TT_HASH_EXISTS', 1) *postfix
                | ':delete' . *Reduce('TT_HASH_DELETE', 1) *postfix
                | (':' ('kv' | 'k' | 'v' | 'p')) . thx nb_word . *Shift('TT_QLIT', thx) . *Reduce('TT_ADVERB', 2) *postfix
                | epsilon );
postfix_expr = *term *postfix;
/* ==================================================================================================================== */
/* the precedence climb, tightest first: ** | symbolic unary | * / % | + - | x xx | ~ | & | | ^ | .. | cmp | chaining | && | || // | ?? !! | = op= | , | X Z ... | and | or */
/* ==================================================================================================================== */
pow_expr   = *postfix_expr FENCE( *op_pow *unary_expr . *Reduce('TT_POW', 2) | epsilon );
unary_expr = ( *$' ' '+' ('«' | '<<') *$' ' *unary_expr . *Reduce('TT_HYPERPFX', 1, '+')
             | *$' ' '-' ('«' | '<<') *$' ' *unary_expr . *Reduce('TT_HYPERPFX', 1, '-')
             | *$' ' '~' ('«' | '<<') *$' ' *unary_expr . *Reduce('TT_HYPERPFX', 1, '~')
             | *$' ' '-' nb_minuseq *$' ' *unary_expr . *Reduce('TT_MNS', 1)
             | *$' ' '+' nb_pluseq *$' ' *unary_expr . *Reduce('TT_PLS', 1)
             | *$' ' '~' nb_tildeq *$' ' *unary_expr . *Reduce('TT_STR', 1)
             | *$' ' '!' nb_bang *$' ' *unary_expr . *Reduce('TT_NOT', 1)
             | *$' ' '?' nb_quest *$' ' *unary_expr . *Reduce('TT_BOOL', 1)
             | *$' ' '++' *unary_expr . *Reduce('TT_PREINC', 1)
             | *$' ' '--' *unary_expr . *Reduce('TT_PREDEC', 1)
             | *pow_expr );
mul_expr = *unary_expr *mul_tail;
mul_tail = FENCE( ( *op_divis *unary_expr . *Reduce('TT_DIVIS', 2) | *op_mul *unary_expr . *Reduce('TT_MUL', 2) | *op_div *unary_expr . *Reduce('TT_DIV', 2) | *op_mod *unary_expr . *Reduce('TT_MOD', 2)
                  | *op_div_i *unary_expr . *Reduce('TT_INTDIV', 2) | *op_mod_w *unary_expr . *Reduce('TT_INTMOD', 2) | *op_gcd *unary_expr . *Reduce('TT_GCD', 2) | *op_lcm *unary_expr . *Reduce('TT_LCM', 2)
                  | *op_band *unary_expr . *Reduce('TT_BAND', 2) | *op_shl *unary_expr . *Reduce('TT_SHL', 2) | *op_shr *unary_expr . *Reduce('TT_SHR', 2) | *op_sband *unary_expr . *Reduce('TT_SBAND', 2) ) *mul_tail | epsilon );
add_expr = *mul_expr *add_tail;
add_tail = FENCE( ( *op_add *mul_expr . *Reduce('TT_ADD', 2) | *op_sub *mul_expr . *Reduce('TT_SUB', 2) | *op_bor *mul_expr . *Reduce('TT_BOR', 2) | *op_bxor *mul_expr . *Reduce('TT_BXOR', 2) | *op_sbor *mul_expr . *Reduce('TT_SBOR', 2) ) *add_tail | epsilon );
rep_expr = *add_expr *rep_tail;
rep_tail = FENCE( ( *op_xx *add_expr . *Reduce('TT_XX', 2) | *op_xrep *add_expr . *Reduce('TT_XREP', 2) ) *rep_tail | epsilon );
cat_expr = *rep_expr *cat_tail;
cat_tail = FENCE( *op_cat *rep_expr . *Reduce('TT_CAT', 2) *cat_tail | epsilon );
jand_expr = *cat_expr *jand_tail;
jand_tail = FENCE( ( *op_jand *cat_expr . *Reduce('TT_JALL', 2) | *op_setdiff *cat_expr . *Reduce('TT_SETDIFF', 2) ) *jand_tail | epsilon );
jor_expr = *jand_expr *jor_tail;
jor_tail = FENCE( ( *op_jor *jand_expr . *Reduce('TT_JANY', 2) | *op_jxor *jand_expr . *Reduce('TT_JONE', 2) ) *jor_tail | epsilon );
rng_expr = *jor_expr FENCE( *op_rangex *jor_expr . *Reduce('TT_TO_EXCL', 2) | *op_xrangex *jor_expr . *Reduce('TT_TO_BEXCL', 2) | *op_xrange *jor_expr . *Reduce('TT_TO_LEXCL', 2) | *op_range *jor_expr . *Reduce('TT_TO', 2) | epsilon );
str_expr = *rng_expr FENCE( *op_cmp3 *rng_expr . *Reduce('TT_CMP3', 2) | *op_cmp *rng_expr . *Reduce('TT_CMP', 2) | *op_leg *rng_expr . *Reduce('TT_LEG', 2) | epsilon );
chn_expr = *str_expr *chn_tail;
chn_tail = FENCE( ( *op_id *str_expr . *Reduce('TT_IDENTICAL', 2) | *op_eqv *str_expr . *Reduce('TT_EQV', 2)
                  | *op_nsmatch *str_expr . *Reduce('TT_NSMATCH', 2) | *op_smatch *str_expr . *Reduce('TT_SMATCH', 2)
                  | *op_eq *str_expr . *Reduce('TT_EQ', 2) | *op_ne *str_expr . *Reduce('TT_NE', 2)
                  | *op_le *str_expr . *Reduce('TT_LE', 2) | *op_ge *str_expr . *Reduce('TT_GE', 2)
                  | *op_lt *str_expr . *Reduce('TT_LT', 2) | *op_gt *str_expr . *Reduce('TT_GT', 2)
                  | *op_leq *str_expr . *Reduce('TT_LEQ', 2) | *op_lne *str_expr . *Reduce('TT_LNE', 2)
                  | *op_llt *str_expr . *Reduce('TT_LLT', 2) | *op_lgt *str_expr . *Reduce('TT_LGT', 2)
                  | *op_lle *str_expr . *Reduce('TT_LLE', 2) | *op_lge *str_expr . *Reduce('TT_LGE', 2) ) *chn_tail | epsilon );
tand_expr = *chn_expr *tand_tail;
tand_tail = FENCE( *op_tand *chn_expr . *Reduce('TT_SEQ', 2) *tand_tail | epsilon );
tor_expr = *tand_expr *tor_tail;
tor_tail = FENCE( ( *op_tor *tand_expr . *Reduce('TT_ALT', 2) | *op_dor *tand_expr . *Reduce('TT_DOR', 2) | *op_txor *tand_expr . *Reduce('TT_XOR', 2)
                  | *op_min *tand_expr . *Reduce('TT_MIN', 2) | *op_max *tand_expr . *Reduce('TT_MAX', 2) ) *tor_tail | epsilon );
cond_expr = *tor_expr FENCE( *op_tern1 *cond_expr *op_tern2 *cond_expr . *Reduce('TT_TERNARY', 3) | epsilon );
/* assignment: `=` takes a comma list when its target is a list (an @ or % variable, a paren) and one item otherwise */
list_sigil = TABLE(2); list_sigil['@'] = 1; list_sigil['%'] = 1;
assign_rhs = FENCE( *DIFFER(list_sigil[SUBSTR(dv_tx, 1, 1)]) *list_expr | *item );
item     = *cond_expr FENCE( *op_pair *item . *Reduce('TT_PAIR', 2)
                           | *op_assign *assign_rhs . *Reduce('TT_ASSIGN', 2)
                           | *op_bind *item . *Reduce('TT_BIND', 2)
                           | *$' ' '.=' *$' ' (*ident) . thx . *Shift('TT_QLIT', thx) . *PushCounter() FENCE(*paren_args | epsilon) . *Reduce('TT_METHASSIGN', nTop() + 2) . *PopCounter()
                           | *$' ' '+=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_ADD') | *$' ' '-=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_SUB')
                           | *$' ' '*=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_MUL') | *$' ' '/=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_DIV')
                           | *$' ' '~=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_CAT') | *$' ' '%=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_MOD')
                           | *$' ' '//=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_DOR')
                           | *$' ' '||=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_ALT') | *$' ' '&&=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_SEQ')
                           | *$' ' '**=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_POW') | *$' ' '%%=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_DIVIS')
                           | *$' ' '+|=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_BOR') | *$' ' '+&=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_BAND')
                           | *$' ' '+^=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_BXOR') | *$' ' '+<=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_SHL') | *$' ' '+>=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_SHR')
                           | *$'  ' 'x=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_XREP') | *$'  ' 'xx=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_XX')
                           | *$'  ' 'div=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_INTDIV') | *$'  ' 'mod=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_INTMOD')
                           | *$'  ' 'min=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_MIN') | *$'  ' 'max=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_MAX')
                           | *$'  ' 'gcd=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_GCD') | *$'  ' 'lcm=' *$' ' *item . *Reduce('TT_AUGOP', 2, 'TT_LCM')
                           | epsilon );
cross_expr = *list_expr *cross_tail;
cross_tail = FENCE( ( *kw_X *list_expr . *Reduce('TT_CROSS', 2) | *kw_Z *list_expr . *Reduce('TT_ZIP', 2) | *op_seq *list_expr . *Reduce('TT_SEQOP', 2) ) *cross_tail | epsilon );
land_expr = *cross_expr *land_tail;
land_tail = FENCE( ( *op_land *cross_expr . *Reduce('TT_SEQ', 2) | *op_andthen *cross_expr . *Reduce('TT_ANDTHEN', 2) ) *land_tail | epsilon );
lor_expr = *land_expr *lor_tail;
lor_tail = FENCE( ( *op_lor *land_expr . *Reduce('TT_ALT', 2) | *op_lxor *land_expr . *Reduce('TT_XOR', 2) | *op_orelse *land_expr . *Reduce('TT_ORELSE', 2) ) *lor_tail | epsilon );
expr     = *lor_expr;
/* ==================================================================================================================== */
/* statements (rule statementlist, token statement, statement_mod_cond, statement_mod_loop): a statement ends at ; or     */
/* before } or at the end, or after a } that closes its line; a modifier is TT_STMT_MOD (statement, condition) valued if  */
/* unless while until for given with without                                                                              */
/* ==================================================================================================================== */
stmt_end = FENCE( *$' ' ';' | *$' ' la_rbrace | *$' ' RPOS(0) | @lk_q *IDENT(SUBSTR(Src, lk_q, 1), '}') FENCE(SPAN(' ' tab) | epsilon) FENCE('#' (BREAK(nl) | REM) | epsilon) (nl | RPOS(0)) );
blk_end  = *$' ' FENCE( ';' | epsilon );
stmt_mod = ( *kw_if *$'  ' *expr . *Reduce('TT_STMT_MOD', 2, 'if')
           | *kw_unless *$'  ' *expr . *Reduce('TT_STMT_MOD', 2, 'unless')
           | *kw_while *$'  ' *expr . *Reduce('TT_STMT_MOD', 2, 'while')
           | *kw_until *$'  ' *expr . *Reduce('TT_STMT_MOD', 2, 'until')
           | *kw_for *$'  ' *expr . *Reduce('TT_STMT_MOD', 2, 'for')
           | *kw_given *$'  ' *expr . *Reduce('TT_STMT_MOD', 2, 'given')
           | *kw_with *$'  ' *expr . *Reduce('TT_STMT_MOD', 2, 'with')
           | *kw_without *$'  ' *expr . *Reduce('TT_STMT_MOD', 2, 'without') );
stmt_mods = FENCE( *$' ' *stmt_mod *stmt_mods | epsilon );
statement = ( *control
            | *declaration
            | *phaser_stmt
            | *prefix_stmt
            | *block *blk_end
            | ('...' | '!!!' | '???') . *Reduce('TT_YADA', 0) *stmt_end
            | *expr *stmt_mods *stmt_end
            | ';' . *Reduce('TT_NUL', 0) );
/* statement_control: if elsif else (one flat TT_IF: cond block cond block ... else-block), unless, with, without, while,   */
/* until, repeat, loop, for (list, signature-or-NUL, body), given (TT_CASE topic body) with when and default as statements */
cond      = *$' ' *DIFFER(in_cond = 1) *expr *DIFFER(in_cond = 0) *$' ';
else_part = FENCE( *$' ' *kw_elsif *cond . *IncCounter() *block . *IncCounter() *else_part
                 | *$' ' *kw_else *$' ' *block . *IncCounter()
                 | epsilon );
loop_part = FENCE( *expr *$' ' | epsilon . *Reduce('TT_NUL', 0) );
control  = *control_core *blk_end;
control_core = ( epsilon . *PushCounter() *kw_if *cond . *IncCounter() *block . *IncCounter() *else_part . *Reduce('TT_IF', nTop()) . *PopCounter()
           | epsilon . *PushCounter() *kw_unless *cond . *IncCounter() *block . *IncCounter() *else_part . *Reduce('TT_UNLESS', nTop()) . *PopCounter()
           | epsilon . *PushCounter() *kw_with *cond . *IncCounter() *block . *IncCounter() *else_part . *Reduce('TT_WITH', nTop()) . *PopCounter()
           | epsilon . *PushCounter() *kw_without *cond . *IncCounter() *block . *IncCounter() *else_part . *Reduce('TT_WITHOUT', nTop()) . *PopCounter()
           | *kw_while *cond *block . *Reduce('TT_WHILE', 2)
           | *kw_until *cond *block . *Reduce('TT_UNTIL', 2)
           | *kw_repeat *$' ' FENCE( *block *$' ' FENCE( *kw_while *cond . *Reduce('TT_REPEAT_WHILE', 2) | *kw_until *cond . *Reduce('TT_REPEAT_UNTIL', 2) )
                                   | *kw_while *cond *block . *Reduce('TT_REPEAT_WHILE', 2)
                                   | *kw_until *cond *block . *Reduce('TT_REPEAT_UNTIL', 2) )
           | *kw_loop *$' ' FENCE( '(' *$' ' *loop_part ';' *$' ' *loop_part ';' *$' ' *loop_part ')' *$' ' *block . *Reduce('TT_CLOOP', 4) | *block . *Reduce('TT_LOOP', 1) )
           | *kw_for *$'  ' *DIFFER(in_cond = 1) *expr *DIFFER(in_cond = 0) *$' ' FENCE( '->' *$' ' . *PushCounter() FENCE(*params | epsilon) *$' ' . *Reduce('TT_SIGNATURE', nTop()) . *PopCounter() | epsilon . *Reduce('TT_NUL', 0) ) *block . *Reduce('TT_FOR', 3)
           | *kw_given *cond *block . *Reduce('TT_CASE', 2)
           | *kw_when *cond *block . *Reduce('TT_WHEN', 2)
           | *kw_whenever *cond *block . *Reduce('TT_WHENEVER', 2)
           | *kw_default *$' ' *block . *Reduce('TT_DEFAULT', 1)
           | *kw_CATCH *$' ' *block . *Reduce('TT_CATCH', 1)
           | *kw_try *$' ' *block . *Reduce('TT_TRY', 1)
           | *kw_gather *$' ' *block . *Reduce('TT_GATHER', 1)
           | *kw_use *$'  ' *use_rest . *Reduce('TT_USE_DECL', nTop(), 'use') . *PopCounter() *use_end
           | *kw_no *$'  ' *use_rest . *Reduce('TT_USE_DECL', nTop(), 'no') . *PopCounter() *use_end
           | *kw_need *$'  ' *use_rest . *Reduce('TT_USE_DECL', nTop(), 'need') . *PopCounter() *use_end
           | *kw_require *$'  ' *use_rest . *Reduce('TT_USE_DECL', nTop(), 'require') . *PopCounter() *use_end
           | *kw_import *$'  ' *use_rest . *Reduce('TT_USE_DECL', nTop(), 'import') . *PopCounter() *use_end
           );
use_end  = FENCE( *$' ' ';' | *$' ' la_rbrace | *$' ' RPOS(0) );
use_rest = epsilon . *PushCounter() ('v' SPAN(digits '.' &LCASE) | *name FENCE(':' SPAN(wordchars '<>') | epsilon)) . thx . *Shift('TT_QLIT', thx) . *IncCounter() FENCE( la_args *$'  ' *list_items | epsilon );
/* phasers keep their place in the statement list (the placement is the lowerer's): TT_PHASER (word, block); a statement */
/* prefix is TT_STMT_PREFIX (word, block-or-statement)                                                                    */
phaser_stmt = (('BEGIN' | 'END' | 'INIT' | 'CHECK' | 'FIRST' | 'LAST' | 'NEXT' | 'ENTER' | 'LEAVE' | 'KEEP' | 'UNDO' | 'PRE' | 'POST' | 'TEMP' | 'CLOSE' | 'CONTROL' | 'QUIT' | 'DOC') nb_word) . thx *$' ' . *Shift('TT_QLIT', thx) *block . *Reduce('TT_PHASER', 2) *blk_end;
prefix_stmt = (('do' | 'once' | 'quietly' | 'react' | 'supply' | 'start' | 'lazy' | 'eager' | 'sink' | 'hyper' | 'race') nb_word) . thx *$'  ' . *Shift('TT_QLIT', thx) FENCE( *block *blk_end | *statement ) . *Reduce('TT_STMT_PREFIX', 2);
/* ==================================================================================================================== */
/* declarations: scope_declarator my our state (TT_DECL type-or-NUL target init-or-NUL, valued by the declarator),        */
/* constant, has (TT_HAS_DECL type-or-NUL var traits init-or-NUL), routine_declarator (TT_SUB_DECL / TT_METHOD_DECL /     */
/* TT_SUBMETHOD_DECL: qualifiers name-or-NUL signature-or-NUL traits body), package_declarator (name traits body-or-NUL), */
/* enum, regex_declarator (name body-text, valued token rule regex)                                                        */
/* ==================================================================================================================== */
decl_target = ( *var_tok
              | '(' *$' ' . *PushCounter() *decl_target . *IncCounter() *decl_more *$' ' ')' . *Reduce('TT_LIST', nTop()) . *PopCounter() *DIFFER(dv_tx = '@') );
decl_more = FENCE( *$' ' ',' *$' ' *decl_target . *IncCounter() *decl_more | epsilon );
decl_init = FENCE( *op_assign *assign_rhs | *op_bind *item . *Reduce('TT_BIND', 1) | epsilon . *Reduce('TT_NUL', 0) );
decl_body = *type_opt *decl_target *$' ' *decl_init;
scope_decl = ( *kw_my *$'  ' *decl_body . *Reduce('TT_DECL', 3, 'my')
             | *kw_our *$'  ' *decl_body . *Reduce('TT_DECL', 3, 'our')
             | *kw_state *$'  ' *decl_body . *Reduce('TT_DECL', 3, 'state') );
const_decl = FENCE((*kw_my | *kw_our) *$'  ' | epsilon) *kw_constant *$'  ' *type_opt ( ((*name) $ tx *DIFFER(const_tbl[tx] = 1)) . thx . *Shift('TT_VAR', thx) | *var_tok ) *$' ' *op_assign *item . *Reduce('TT_DECL', 3, 'constant');
has_decl = *kw_has *$'  ' *type_opt (*var_body) . thx . *Shift('TT_VAR', thx) *traits *$' ' FENCE( *op_assign *item | epsilon . *Reduce('TT_NUL', 0) ) . *Reduce('TT_HAS_DECL', 4);
qual     = epsilon . *PushCounter() FENCE((*kw_my | *kw_our) . thx . *Shift('TT_QLIT', thx) . *IncCounter() *$'  ' | epsilon)
           FENCE((*kw_multi | *kw_proto | *kw_only) . thx . *Shift('TT_QLIT', thx) . *IncCounter() *$'  ' | epsilon) . *Reduce('TT_QUAL', nTop()) . *PopCounter();
routine_name = FENCE( (('infix' | 'prefix' | 'postfix' | 'circumfix' | 'postcircumfix') ':' ('<' BREAK('>') '>' | '«' BREAK('»') '»')) . thx . *Shift('TT_VAR', thx)
                    | (FENCE('!' | '^' | epsilon) *name FENCE(':' *ident FENCE('<' BREAK('>') '>' | epsilon) | epsilon)) . thx . *Shift('TT_VAR', thx)
                    | epsilon . *Reduce('TT_NUL', 0) );
routine_rest = *$' ' *routine_name *$' ' *sig_opt *traits *$' ' *block;
routine_decl = ( *qual FENCE( *kw_sub *routine_rest . *Reduce('TT_SUB_DECL', 5) | *kw_method *routine_rest . *Reduce('TT_METHOD_DECL', 5) | *kw_submethod *routine_rest . *Reduce('TT_SUBMETHOD_DECL', 5) )
               | epsilon . *PushCounter() (*kw_multi | *kw_proto) . thx . *Shift('TT_QLIT', thx) . *IncCounter() . *Reduce('TT_QUAL', nTop()) . *PopCounter() *routine_rest . *Reduce('TT_SUB_DECL', 5) );
pkg_rest = *$'  ' (*name FENCE('[' BREAK(']') ']' | epsilon)) . thx . *Shift('TT_VAR', thx) *traits *$' ' FENCE( *block | ';' . *Reduce('TT_NUL', 0) );
package_decl = ( *kw_class *pkg_rest . *Reduce('TT_CLASS_DECL', 3) | *kw_role *pkg_rest . *Reduce('TT_ROLE_DECL', 3) | *kw_grammar *pkg_rest . *Reduce('TT_GRAMMAR_DECL', 3) | *kw_module *pkg_rest . *Reduce('TT_MODULE_DECL', 3) );
enum_word = (((NOTANY(' ' tab nl '>') FENCE(BREAK(' ' tab nl '>') | REM)) $ tx *DIFFER(const_tbl[tx] = 1))) . thx . *Shift('TT_QLIT', thx) . *IncCounter();
enum_words = FENCE(SPAN(' ' tab nl) | epsilon) FENCE(*enum_word FENCE(SPAN(' ' tab nl) | epsilon) *enum_words | epsilon);
enum_decl = *kw_enum *$'  ' (*name) . thx . *Shift('TT_VAR', thx) *$' ' FENCE( '<' nb_qw . *PushCounter() *enum_words '>' . *Reduce('TT_QWORDS', nTop()) . *PopCounter() | *paren ) . *Reduce('TT_ENUM', 2);
rg_body  = ARBNO( NOTANY('{}') | '{' *rg_body '}' );
rg_rest  = *$'  ' (*name FENCE(':sym<' BREAK('>') '>' | epsilon)) . thx . *Shift('TT_VAR', thx) *$' ' FENCE('(' BREAK(')') ')' *$' ' | epsilon) '{' (*rg_body) . thx '}' . *Shift('TT_QLIT', thx);
regex_decl = FENCE(*kw_proto *$'  ' | epsilon) ( *kw_token *rg_rest . *Reduce('TT_REGEX_DECL', 2, 'token') | *kw_rule *rg_rest . *Reduce('TT_REGEX_DECL', 2, 'rule') | *kw_regex *rg_rest . *Reduce('TT_REGEX_DECL', 2, 'regex') );
declaration = ( *const_decl *stmt_end
              | *scope_decl *stmt_mods *stmt_end
              | *has_decl *stmt_end
              | *routine_decl *blk_end
              | *package_decl *blk_end
              | *kw_unit *$'  ' *package_decl *blk_end
              | *enum_decl *stmt_end
              | *regex_decl *blk_end
              );
/* ==================================================================================================================== */
/* comp_unit: every top-level statement is TT_STMT(TT_ATTR :subj X), the actions replayed at the FLUSH that closes it      */
/* ==================================================================================================================== */
top_stmt  = *statement . *Reduce('TT_ATTR', 1, ':subj') . *Reduce('TT_STMT', 1) . *IncCounter();
Compiland = epsilon . *PushCounter()
            POS(0) ARBNO( *$' ' *top_stmt FLUSH ) *$' ' RPOS(0)
            . *Reduce('Parse', nTop())
            . *PopCounter();
function ParseOne(ptree, i, n_kids) {
    pf_a = TIME();
    InitCounter();
    InitStack();
    dv_tx = ''; in_cond = 0;
    if (Src ? *Compiland) {
        ptree = Pop();
        pf_parse = pf_parse + (TIME() - pf_a);
        if (DIFFER(ptree)) { i = 1; n_kids = n(ptree); while (LE(i, n_kids)) { TreeDump(c(ptree)[i]); i = i + 1; } }
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
    pf_n = 0; pf_bytes = 0; pf_parse = 0; pf_parse1 = 0;
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
