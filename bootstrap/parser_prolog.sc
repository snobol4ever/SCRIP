E_Parse  = "'Parse'";
white   =   (  SPAN(' ' tab nl)
            |  '%'  ARBNO(NOTANY(nl)) (nl | RPOS(0))
            |  '/*' BREAK('*') '*' ARBNO('*' | NOTANY('/*') BREAK('*') '*') '/'
            );
White   =   white FENCE(*White | epsilon);
Gray    =   White | epsilon;
$' '    =   Gray;
$'  '   =   White;
Atom_first = ANY(&LCASE X1xxxxxxx);
Atom_rest  = SPAN(digits &UCASE &LCASE '_' X1xxxxxxx);
Atom       = (Atom_first (Atom_rest | epsilon));
Qchars     = FENCE((NOTANY("'\") | "''" | '\' ('x' SPAN(hex_digits) '\' | SPAN(oct_digits) '\' | LEN(1))) *Qchars | epsilon);
Qatom      = ("'" *Qchars . q_body "'");
Qatom_h    = ("'" BREAK("'") . h_body "'");
Var_first  = ANY(&UCASE '_');
Var_rest   = SPAN(digits &UCASE &LCASE '_');
Var        = (Var_first (Var_rest | epsilon));
Float      = (SPAN(digits) '.' SPAN(digits) FENCE('e' FENCE(ANY('+-') | epsilon) SPAN(digits) | 'E' FENCE(ANY('+-') | epsilon) SPAN(digits) | epsilon));
Char_code  = ("0'" NOTANY(nl));
Int        = SPAN(digits) FENCE(('_' FENCE(SPAN(' ' tab nl) | epsilon) | ' ') *Int | epsilon);
Str        = ('"' BREAK('"') . s_body '"');
$'('   =       '('  $' ';  $')'  = $' ' ')';
$'['   =       '['  $' ';  $']'  = $' ' ']';
$','   = $' '  ','  $' ';  $';'  = $' ' ';' $' ';
$'|'   = $' '  '|'  $' ';
$'.'   = $' '  '.';
$':-'  = $' '  ':-' $' ';  $':'  = $' '  ':'  . _op_name @la_c *DIFFER(SUBSTR(Src, la_c + 1, 1), '-') $' ';  $'='  = $' ' '='  $' ';
$'+'   = $' '  '+'  $' ';  $'-'   = $' ' '-' @la_m *DIFFER(SUBSTR(Src, la_m + 1, 1), '>') *DIFFER(SUBSTR(Src, la_m + 1, 2), '->') $' ';
$'*'   = $' '  '*' @la_s *DIFFER(SUBSTR(Src, la_s + 1, 2), '->') $' ';  $'/'  = $' ' '/'  $' ';
$'is'  = $'  ' 'is' $'  ';
$'*->' = $' ' '*->' . _op_name $' ';
$'as'  = $'  ' 'as' . _op_name $'  ';
$'-->' = $' ' '-->' $' ';
$'{'   = $' '  '{'  $' ';  $'}'  = $' ' '}'  $' ';
Tk_cut = $' ' '!' $' ';
$'=:=' = $' ' '=:=' $' ';  $'=\=' = $' ' '=\=' $' ';
$'=='  = $' ' '=='  $' ';  $'\==' = $' ' '\==' $' ';
$'>='  = $' ' '>='  $' ';  $'=<'  = $' ' '=<'  $' ';
$'>'   = $' ' '>'   $' ';  $'<'   = $' ' '<'   $' ';
$'\='  = $' ' '\='  $' ';
$'=..' = $' ' '=..' $' ';
$'=@=' = $' ' '=@=' . _op_name $' ';  $'\=@=' = $' ' '\=@=' . _op_name $' ';
$'@>=' = $' ' '@>=' . _op_name $' ';  $'@=<' = $' ' '@=<' . _op_name $' ';
$'@>'  = $' ' '@>'  . _op_name $' ';  $'@<'  = $' ' '@<'  . _op_name $' ';
$'**'  = $' ' '**'  . _op_name $' ';  $'^'   = $' '  '^'  . _op_name $' ';
$'//'  = $' ' '//'  $' ';
$'/\' = $' ' '/\' . _op_name $' ';  $'\/' = $' ' '\/' . _op_name $' ';
$'>>'  = $' ' '>>'  . _op_name $' ';  $'<<'  = $' ' '<<'  . _op_name $' ';
$'mod' = $'  ' 'mod' . _op_name $'  ';
$'rem' = $'  ' 'rem' . _op_name $'  ';
$'xor' = $'  ' 'xor' . _op_name $'  ';
$'div' = $'  ' 'div' . _op_name $'  ';
$'rdiv' = $'  ' 'rdiv' . _op_name $'  ';
$'\'   = $' ' '\' . _op_name;
$'->'  = $' ' '->' $' ';
Graphic_first = ANY('\\@#^~?=<>+\-*/:.$&`');
Graphic_rest  = SPAN('\\+\-*/^<>=~?@#&:.$`');
Graphic_atom  = (Graphic_first (Graphic_rest | epsilon));
Graphic_atom2 = (Graphic_first Graphic_first (Graphic_rest | epsilon));
hex_value = TABLE();
hex_i = 0;
while (LE(hex_i, 35)) {
    hex_value[SUBSTR('0123456789abcdefghijklmnopqrstuvwxyz', hex_i + 1, 1)] = hex_i;
    hex_value[SUBSTR('0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', hex_i + 1, 1)] = hex_i;
    hex_i = hex_i + 1;
}
ascii_table = TABLE();
ascii_i = 0;
while (LE(ascii_i, 127)) {
    ascii_table[CHAR(ascii_i)] = ascii_i;
    ascii_i = ascii_i + 1;
}
ascii_table["''"] = 39;  ascii_table['\\'] = 92;  ascii_table["\'"] = 39;  ascii_table['\"'] = 34;  ascii_table['\`'] = 96;
ascii_table['\n'] = 10;  ascii_table['\r'] = 13;   ascii_table['\t'] = 9;    ascii_table['\a'] = 7;    ascii_table['\b'] = 8;
ascii_table['\f'] = 12;  ascii_table['\v'] = 11;   ascii_table['\0'] = 0;    ascii_table['\e'] = 27;   ascii_table['\s'] = 32;
/* ==================================================================================================================== */
function unescape_q(raw, out, i, n, c, prev_was_quote) {
    out = '';  n = SIZE(raw);  i = 1;  prev_was_quote = 0;
    while (LE(i, n)) {
        c = SUBSTR(raw, i, 1);
        if (IDENT(c, "'")) {
            if (EQ(prev_was_quote, 1)) { out = out "'";  prev_was_quote = 0; }
            else                       { prev_was_quote = 1; }
        } else {
            if (EQ(prev_was_quote, 1)) { out = out "'";  prev_was_quote = 0; }
            out = out c;
        }
        i = i + 1;
    }
    unescape_q = out;
    return;
}
/* ==================================================================================================================== */
/* radix value helpers — pure computation, no stack ops */
function compute_hex(raw, n, i, len, s) {
    n = 0;  i = 1;  len = SIZE(raw);
    while (LE(i, len)) {
        n = n * 16 + hex_value[SUBSTR(raw, i, 1)];
        i = i + 1;
    }
    (n '') ? SPAN('0123456789') . s;
    compute_hex = s;
    return;
}
/* ==================================================================================================================== */
function compute_bin(raw, n, i, len, s) {
    n = 0;  i = 1;  len = SIZE(raw);
    while (LE(i, len)) {
        n = n * 2 + SUBSTR(raw, i, 1) + 0;
        i = i + 1;
    }
    (n '') ? SPAN('0123456789') . s;
    compute_bin = s;
    return;
}
/* ==================================================================================================================== */
function compute_oct(raw, n, i, len, s) {
    n = 0;  i = 1;  len = SIZE(raw);
    while (LE(i, len)) {
        n = n * 8 + SUBSTR(raw, i, 1) + 0;
        i = i + 1;
    }
    (n '') ? SPAN('0123456789') . s;
    compute_oct = s;
    return;
}
/* ==================================================================================================================== */
/* an integer in radix r (10 for a plain or digit-grouped one): a digit group's _ and blanks have no hex_value and are skipped */
function compute_radix(r, raw, n, i, len, d, s) {
    n = 0;  i = 1;  len = SIZE(raw);
    while (LE(i, len)) {
        d = hex_value[SUBSTR(raw, i, 1)];
        if (DIFFER(d)) n = n * r + d;
        i = i + 1;
    }
    (n '') ? SPAN('0123456789') . s;
    compute_radix = s;
    return;
}
/* ==================================================================================================================== */
/* op/3: the user operator table -- uop_band[name] is the band key ('in700', 'pre500', 'post'); uop_on is FAIL until the first */
/* op/3 goal declares, then the one token pattern, and each site checks its own key by *IDENT at match time; no pattern is built */
uop_band = TABLE();
uop_tok  = $' ' ((((Atom | Graphic_atom) $ uop_tx) . thx) . *Shift('TT_FNC', thx)) $' ';
uop_on   = FAIL;
op_infix   = epsilon . *OpSwap(3);
op_postfix = epsilon . *OpSwap(2);
/* ==================================================================================================================== */
function OpSwap(n, x, k, f) {
    Reduce('TT_COMPOUND', n);
    x = Pop();  k = c(x);  f = k[1];  k[1] = k[2];  k[2] = f;  Push(x);
    OpSwap = .dummy;
    nreturn;
}
/* ==================================================================================================================== */
function DeclareOp(p, t, n, b) {
    DeclareOp = .dummy;
    if (LE(p, 0)) { uop_band[n] = ;  nreturn; }
    b = 900;  b = LE(p, 700) 700;  b = LE(p, 600) 600;  b = LE(p, 500) 500;  b = LE(p, 400) 400;  b = LE(p, 200) 200;
    if (EQ(SIZE(t), 3))                    uop_band[n] = 'in' b;
    else if (IDENT(SUBSTR(t, 1, 1), 'f'))  uop_band[n] = 'pre' b;
    else                                   uop_band[n] = 'post';
    uop_on = uop_tok;
    nreturn;
}
/* ==================================================================================================================== */
arg       = ( *arg_top | (Graphic_atom | ';') . b_name epsilon . *Shift('TT_FNC', b_name) );
arg_ite   = ( *unify_expr FENCE( $'->' *arg_ite  reduce("'TT_IFTHEN'", 2) | epsilon ) );
arg_disj  = ( *arg_ite    FENCE( $';'  *arg_disj reduce("'TT_DISJ'",   2) | epsilon ) );
arg_top   = ( *arg_disj   FENCE( $':-' *arg_disj reduce("'TT_CLAUSE'", 2) | epsilon ) );
args      = ( nInc() *arg FENCE(*args_tail | epsilon) );
args_tail = ( $',' nInc() *arg FENCE(*args_tail | epsilon) );
list_body_tail = ( $',' nInc() *arg FENCE( *list_body_tail | epsilon ) );
list_body      = ( nInc() *arg FENCE( *list_body_tail | epsilon ) );
/* list: nil → TT_MAKELIST(0 children); [h|t] or [h,..] → TT_MAKELIST(n+1: elems then tail) */
list = (    $'['
            FENCE(
              $']'                    reduce("'TT_MAKELIST'", 0)
            | nPush()
                  *list_body
                  FENCE( $'|' *arg
                       | epsilon           reduce("'TT_MAKELIST'", 0)
                       )
                  $']'
                                       reduce("'TT_MAKELIST'", 'nTop() + 1')
              nPop()
            )
       );
/* ==================================================================================================================== */
/* primary: leaf atoms, variables, numbers, compound terms, parenthesised expr, list */
primary = (   Atom . p_name $'('
                  nPush()
                  epsilon . *Shift('TT_FNC', p_name) nInc()
                  (*args | epsilon) $')'
                  reduce("'TT_COMPOUND'", 'nTop()')
              nPop()
          |   $' ' (Graphic_atom | ';') . g_name $'('
                  nPush()
                  epsilon . *Shift('TT_FNC', g_name) nInc()
                  *args $')'
                  reduce("'TT_COMPOUND'", 'nTop()')
              nPop()
          |   $' ' '-' Float . p_negf
                  epsilon . *Shift('TT_FLIT', '-' p_negf)
          |   $' ' '-' Int . p_negi
                  epsilon . *Shift('TT_ILIT', '-' p_negi)
          |   *uop_on *IDENT(uop_band[uop_tx], 'pre200') *primary     reduce("'TT_COMPOUND'", 2)
          |   *uop_on *IDENT(uop_band[uop_tx], 'pre400') *pow_expr    reduce("'TT_COMPOUND'", 2)
          |   *uop_on *IDENT(uop_band[uop_tx], 'pre500') *mul_expr    reduce("'TT_COMPOUND'", 2)
          |   *uop_on *IDENT(uop_band[uop_tx], 'pre600') *add_expr    reduce("'TT_COMPOUND'", 2)
          |   *uop_on *IDENT(uop_band[uop_tx], 'pre700') *colon_expr  reduce("'TT_COMPOUND'", 2)
          |   *uop_on *IDENT(uop_band[uop_tx], 'pre900') *unify_expr  reduce("'TT_COMPOUND'", 2)
          |   $' ' '\+' $' ' *unify_expr    reduce("'TT_NAF'", 1)
          |   shift(Graphic_atom2, 'TT_FNC')
          |   Tk_cut                  reduce("'TT_CUT'", 0)
          |   "0'\x" SPAN(hex_digits) . p_radix FENCE('\' | epsilon)
                  epsilon . *Shift('TT_ILIT', compute_hex(p_radix))
          |   "0'" ("''" | '\' LEN(1) | NOTANY(nl)) . p_cc
                  epsilon . *Shift('TT_ILIT', ascii_table[p_cc])
          |   SPAN(digits) . p_rad "'" SPAN(digits &LCASE &UCASE) . p_rdig
                  epsilon . *Shift('TT_ILIT', compute_radix(p_rad, p_rdig))
          |   shift(Float,'TT_FLIT')
          |   '0x' SPAN(hex_digits) . p_radix
                  epsilon . *Shift('TT_ILIT', compute_hex(p_radix))
          |   '0b' SPAN(bin_digits) . p_radix
                  epsilon . *Shift('TT_ILIT', compute_bin(p_radix))
          |   '0o' SPAN(oct_digits) . p_radix
                  epsilon . *Shift('TT_ILIT', compute_oct(p_radix))
          |   Int . p_int
                  epsilon . *Shift('TT_ILIT', compute_radix(10, p_int))
          |   shift(Atom, 'TT_FNC')
          |   Qatom $'('
                  nPush()
                  epsilon . *Shift('TT_FNC', unescape_q(q_body)) nInc()
                  *args $')'
                  reduce("'TT_COMPOUND'", 'nTop()')
              nPop()
          |   Qatom
                  epsilon . *Shift('TT_FNC', unescape_q(q_body))
          |   Str
                  epsilon . *Shift('TT_FNC', s_body)
          |   Var . p_text
                  epsilon . *Shift('TT_VAR', p_text)
          |   $'(' *unify_expr $')'
          |   $'(' *unify_expr $':-' *body $')'  reduce("'TT_CLAUSE'", 2)
          |   $'(' *unify_expr FENCE($',' *unify_expr reduce("'TT_CONJ'", 2) | epsilon) $'-->' *dcg_body $')'  reduce("'TT_DCG_RULE'", 2)
          |   $'(' $':-' *body $')'              reduce("'TT_DIRECTIVE'", 1)
          |   $'(' *body $')'
          |   $'(' (Graphic_atom | ';') . b_name $')'
                  epsilon . *Shift('TT_FNC', b_name)
          |   $'{' $'}'             reduce("'TT_DCG_IL'", 0)
          |   $'{' *body $'}'       reduce("'TT_DCG_IL'", 1)
          |   *list
          |   $'\' $' ' *primary            reduce("'TT_BINOP'", 2)
          |   $' ' '-' $' ' *primary   reduce("'TT_UMINUS'", 1)
          |   $' ' '+' $' ' *primary   reduce("'TT_UPLUS'", 1)
          );
pow_expr  = (   *primary
                FENCE( $'^'  *pow_expr  reduce("'TT_BINOP'", 2)
                     | $'**' *primary   reduce("'TT_BINOP'", 2)
                     | *uop_on *IDENT(uop_band[uop_tx], 'in200') *pow_expr op_infix
                     | *uop_on *IDENT(uop_band[uop_tx], 'post') op_postfix
                     | epsilon
                     )
            );
mul_expr  = (   *pow_expr *mul_tail );
mul_tail  = FENCE( FENCE( $'mod' *pow_expr  reduce("'TT_BINOP'", 2)
                         | $'rem' *pow_expr  reduce("'TT_BINOP'", 2)
                         | $'div' *pow_expr  reduce("'TT_BINOP'", 2)
                         | $'rdiv' *pow_expr reduce("'TT_BINOP'", 2)
                         | $'>>'  *pow_expr  reduce("'TT_BINOP'", 2)
                         | $'<<'  *pow_expr  reduce("'TT_BINOP'", 2)
                         | $'*'   *pow_expr  reduce("'TT_MUL'",   2)
                         | $'//'  *pow_expr  reduce("'TT_IDIV'",  2)
                         | $'/\'  *pow_expr  reduce("'TT_BINOP'", 2)
                         | $'/'   *pow_expr  reduce("'TT_DIV'",   2)
                         | *uop_on *IDENT(uop_band[uop_tx], 'in400') *pow_expr op_infix
                         ) *mul_tail | epsilon );
add_expr  = (   *mul_expr *add_tail );
add_tail  = FENCE( FENCE( $'+' *mul_expr  reduce("'TT_ADD'",   2)
                         | $'-' *mul_expr  reduce("'TT_SUB'",   2)
                         | $'\/' *mul_expr reduce("'TT_BINOP'", 2)
                         | $'xor' *mul_expr reduce("'TT_BINOP'", 2)
                         | *uop_on *IDENT(uop_band[uop_tx], 'in500') *mul_expr op_infix
                         ) *add_tail | epsilon );
colon_expr = (  *add_expr
                FENCE( $':' *colon_expr  reduce("'TT_BINOP'", 2)
                     | *uop_on *IDENT(uop_band[uop_tx], 'in600') *colon_expr op_infix
                     | epsilon
                     )
             );
is_expr   = (   *colon_expr
                FENCE( $'is' *colon_expr  reduce("'TT_IS'", 2)
                     | epsilon
                     )
            );
cmp_expr  = (   *is_expr
                FENCE( $'as'  *is_expr  reduce("'TT_BINOP'",2)
                     | $'=@=' *is_expr  reduce("'TT_BINOP'",2)
                     | $'\=@=' *is_expr reduce("'TT_BINOP'",2)
                     | $'=:=' *is_expr  reduce("'TT_EQQ'",  2)
                     | $'=\=' *is_expr  reduce("'TT_NE2'",  2)
                     | $'\==' *is_expr  reduce("'TT_NE3'",  2)
                     | $'@>=' *is_expr  reduce("'TT_BINOP'",2)
                     | $'@=<' *is_expr  reduce("'TT_BINOP'",2)
                     | $'@>'  *is_expr  reduce("'TT_BINOP'",2)
                     | $'@<'  *is_expr  reduce("'TT_BINOP'",2)
                     | $'>='  *is_expr  reduce("'TT_GE'",   2)
                     | $'=<'  *is_expr  reduce("'TT_LE'",   2)
                     | $'>'   *is_expr  reduce("'TT_GT'",   2)
                     | $'<'   *is_expr  reduce("'TT_LT'",   2)
                     | $'\='  *is_expr  reduce("'TT_NE1'",  2)
                     | $'=='  *is_expr  reduce("'TT_ID'",   2)
                     | *uop_on *IDENT(uop_band[uop_tx], 'in700') *is_expr  op_infix
                     | epsilon
                     )
            );
eq_expr    = (  *cmp_expr
                FENCE( $'=..' *cmp_expr  reduce("'TT_UNIV'",  2)
                     | $'='   *cmp_expr  reduce("'TT_UNIFY'", 2)
                     | epsilon
                     )
             );
unify_expr = ( *eq_expr *op_tail );
op_tail    = FENCE( *uop_on *IDENT(uop_band[uop_tx], 'in900') *eq_expr op_infix *op_tail | epsilon );
/* ==================================================================================================================== */
pfx_kw_name = (   "dynamic" | "discontiguous" | "meta_predicate" | "multifile"
              |   "module_transparent" | "thread_local" | "volatile"
              |   "initialization" | "thread_initialization" | "public" | "table" | "record"
              );
op_type = ( 'xfx' | 'xfy' | 'yfx' | 'fy' | 'fx' | 'xf' | 'yf' );
op_goal = (   $' ' 'op' $'(' nPush() epsilon . *Shift('TT_FNC', 'op') nInc()
              shift(Int $ op_p, 'TT_ILIT') nInc() $','
              shift(op_type $ op_t, 'TT_FNC') nInc() $','
              $' ' ( "'" (BREAK("'") $ op_n . thx) "'" | (Atom | Graphic_atom) $ op_n . thx ) . *Shift('TT_FNC', thx) nInc()
              $')' epsilon $ *DeclareOp(op_p, op_t, op_n)
              reduce("'TT_COMPOUND'", 'nTop()') nPop()
          );
body_goal = (   $' ' pfx_kw_name . pfx_kw $'  ' *unify_expr
                    reduce("'TT_PFX'", 1)
            |   $' ' '\+' $' ' *body_goal  reduce("'TT_NAF'", 1)
            |   *op_goal
            |   *unify_expr
            |   $'(' *body $')'
            );
conj = (    nPush()
                nInc() *body_goal
                *conj_tail
                                   reduce("'TT_CONJ'", 'nTop()')
            nPop()
        );
conj_tail = FENCE( $',' nInc() *body_goal *conj_tail | epsilon );
conj_arrow = ( *conj FENCE( $'->' *conj_arrow  reduce("'TT_IFTHEN'", 2)  | $'*->' *conj_arrow  reduce("'TT_BINOP'", 2)  | epsilon ) );
disj_tail = ( $';' nInc() *conj_arrow FENCE( *disj_tail | epsilon ) );
disj = (    nPush()
                nInc() *conj_arrow
                FENCE( *disj_tail | epsilon )
                                   reduce("'TT_DISJ'", 'nTop()')
            nPop()
        );
body = *disj;
/* head: reduces to a single TT_COMPOUND node (functor + args as children) */
head = *unify_expr;
dcg_goal = (   *list
           |   $'{' *body $'}'       reduce("'TT_DCG_IL'", 1)
           |   Tk_cut               reduce("'TT_CUT'", 0)
           |   $'(' *dcg_body $')'
           |   *unify_expr
           );
dcg_conj = (   nPush()
                   nInc() *dcg_goal
                   *dcg_conj_tail
                                      reduce("'TT_CONJ'", 'nTop()')
               nPop()
           );
dcg_disj = (   nPush()
                   nInc() *dcg_conj
                   *dcg_disj_tail
                                      reduce("'TT_DISJ'", 'nTop()')
               nPop()
           );
dcg_conj_tail = FENCE( $',' nInc() *dcg_goal *dcg_conj_tail | epsilon );
dcg_disj_tail = FENCE( $';' nInc() *dcg_conj *dcg_disj_tail | epsilon );
dcg_body = *dcg_disj;
/* a pushback after the head is a comma sequence of terms, right-nested as Prolog reads ','(A, ','(B, C)) */
dcg_push  = (   *unify_expr FENCE($',' *dcg_push reduce("'TT_CONJ'", 2) | epsilon) );
dcg_rule  = (   *head FENCE($',' *dcg_push reduce("'TT_CONJ'", 2) | epsilon) $'-->'
                *dcg_body $'.'
                                      reduce("'TT_DCG_RULE'", 2)
            );
clause    = (   head
                ( $':-' *body         reduce("'TT_CLAUSE'", 2)
                | epsilon            reduce("'TT_CLAUSE'", 1)
                )
                $'.'
            );
directive = (   $':-'
                *body $'.'
                                     reduce("'TT_DIRECTIVE'", 1)
            );
top_form  = (*directive | *clause | *dcg_rule);
/* ==================================================================================================================== */
/* SCT-pivot (2026-05-17): nInc() must fire AFTER top_form commits, not before. */
Compiland = nPush()
            POS(0) ARBNO( FENCE($' ' *top_form nInc()) ) $' ' RPOS(0)
            reduce(E_Parse, 'nTop()')
            nPop();
InitCounter();
InitStack();
Src = '';
while ((Line = INPUT)) Src = Src Line nl ;
/* SCT-pivot (2026-05-17): strip the trailing nl added by the loop. */
if (GT(SIZE(Src), 0)) Src = SUBSTR(Src, 1, SIZE(Src) - 1);
if (Src ? Compiland) {
    ptree = Pop();
    if (DIFFER(ptree)) {
        i = 1; n_kids = n(ptree);
        while (LE(i, n_kids)) { TreeDump(c(ptree)[i]); i = i + 1; }
    }
} else OUTPUT = 'Parse Error';
