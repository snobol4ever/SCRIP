E_Parse  = "'Parse'";
white   =   (  SPAN(' ' tab nl)
            |  '%'  BREAK(nl) nl
            |  '/*' BREAKX('*') '*/'
            );
White   =   white;
Gray    =   white | epsilon;
$' '    =   Gray;
$'  '   =   White;
Atom_first = ANY(&LCASE);
Atom_rest  = SPAN(digits &UCASE &LCASE '_');
Atom       = (Atom_first (Atom_rest | epsilon));
Qatom      = ("'" BREAK("'") . q_body "'");
Var_first  = ANY(&UCASE '_');
Var_rest   = SPAN(digits &UCASE &LCASE '_');
Var        = (Var_first (Var_rest | epsilon));
Float      = (SPAN(digits) '.' SPAN(digits) FENCE('e' FENCE(ANY('+-') | epsilon) SPAN(digits) | 'E' FENCE(ANY('+-') | epsilon) SPAN(digits) | epsilon));
Char_code  = ("0'" NOTANY(nl));
Int        = SPAN(digits);
Str        = ('"' BREAK('"') . s_body '"');
$'('   =       '('  $' ';  $')'  = $' ' ')';
$'['   =       '['  $' ';  $']'  = $' ' ']';
$','   = $' '  ','  $' ';  $';'  = $' ' ';' $' ';
$'|'   = $' '  '|'  $' ';
$'.'   = $' '  '.';
$':-'  = $' '  ':-' $' ';  $':'  = $' '  ':'  . _op_name $' ';  $'='  = $' ' '='  $' ';
$'+'   = $' '  '+'  $' ';  $'-'  = $' ' '-'  $' ';
$'*'   = $' '  '*'  $' ';  $'/'  = $' ' '/'  $' ';
$'is'  = $'  ' 'is' $'  ';
$'-->' = $' ' '-->' $' ';
$'{'   = $' '  '{'  $' ';  $'}'  = $' ' '}'  $' ';
Tk_cut = $' ' '!' $' ';
$'=:=' = $' ' '=:=' $' ';  $'=\=' = $' ' '=\=' $' ';
$'=='  = $' ' '=='  $' ';  $'\==' = $' ' '\==' $' ';
$'>='  = $' ' '>='  $' ';  $'=<'  = $' ' '=<'  $' ';
$'>'   = $' ' '>'   $' ';  $'<'   = $' ' '<'   $' ';
$'\='  = $' ' '\='  $' ';
$'=..' = $' ' '=..' $' ';
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
ascii_table = TABLE();
ascii_i = 0;
while (LE(ascii_i, 127)) {
    ascii_table[CHAR(ascii_i)] = ascii_i;
    ascii_i = ascii_i + 1;
}
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
function compute_hex(raw, val, s) {
    val = EVAL('0x' raw) '';
    val SPAN('0123456789') . s;
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
    n '' SPAN('0123456789') . s;
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
    n '' SPAN('0123456789') . s;
    compute_oct = s;
    return;
}
/* ==================================================================================================================== */
args      = ( nInc() *unify_expr FENCE(*args_tail | epsilon) );
args_tail = ( $',' nInc() *unify_expr FENCE(*args_tail | epsilon) );
list_body_tail = ( $',' nInc() *unify_expr FENCE( *list_body_tail | epsilon ) );
list_body      = ( nInc() *unify_expr FENCE( *list_body_tail | epsilon ) );
/* list: nil → TT_MAKELIST(0 children); [h|t] or [h,..] → TT_MAKELIST(n+1: elems then tail) */
list = (    $'['
            FENCE(
              $']'                    reduce("'TT_MAKELIST'", 0)
            | nPush()
                  list_body
                  FENCE( $'|' *unify_expr
                       | epsilon           reduce("'TT_MAKELIST'", 0)
                       )
                  $']'
                                       reduce("'TT_MAKELIST'", 'nTop()+1')
              nPop()
            )
       );
/* ==================================================================================================================== */
/* primary: leaf atoms, variables, numbers, compound terms, parenthesised expr, list */
primary = (   Atom . p_name $'('
                  nPush()
                  shift(p_name, 'TT_FNC') nInc()
                  args $')'
                  reduce("'TT_COMPOUND'", 'nTop()')
              nPop()
          |   $' ' Graphic_atom . g_name $'('
                  nPush()
                  shift(g_name, 'TT_FNC') nInc()
                  args $')'
                  reduce("'TT_COMPOUND'", 'nTop()')
              nPop()
          |   shift(Graphic_atom2, 'TT_FNC')
          |   Tk_cut                  reduce("'TT_CUT'", 0)
          |   "0'" NOTANY(nl) . p_cc
                  assign(.t_imm, ascii_table[p_cc])
                  shift(t_imm, 'TT_ILIT')
          |   shift(Float,'TT_FLIT')
          |   '0x' SPAN(hex_digits) . p_radix
                  assign(.t_imm, compute_hex(p_radix))
                  shift(t_imm, 'TT_ILIT')
          |   '0b' SPAN(bin_digits) . p_radix
                  assign(.t_imm, compute_bin(p_radix))
                  shift(t_imm, 'TT_ILIT')
          |   '0o' SPAN(oct_digits) . p_radix
                  assign(.t_imm, compute_oct(p_radix))
                  shift(t_imm, 'TT_ILIT')
          |   shift(Int,  'TT_ILIT')
          |   shift(Atom, 'TT_FNC')
          |   Qatom
                  assign(.t_imm, unescape_q(q_body))
                  shift(t_imm, 'TT_FNC')
          |   Str
                  shift(s_body, 'TT_FNC')
          |   Var . p_text
                  shift(p_text, 'TT_VAR')
          |   $'(' *unify_expr $')'
          |   *list
          |   $' ' '-' Float . p_negf
                  assign(.t_imm, '-' p_negf)
                  shift(t_imm, 'TT_FLIT')
          |   $' ' '-' Int . p_negi
                  assign(.t_imm, '-' p_negi)
                  shift(t_imm, 'TT_ILIT')
          |   $'\' $' ' *primary            reduce("'TT_BINOP'", 2)
          |   $' ' '-' *primary        reduce("'TT_UMINUS'", 1)
          );
pow_expr  = (   primary
                FENCE( $'^'  *pow_expr  reduce("'TT_BINOP'", 2)
                     | $'**' primary   reduce("'TT_BINOP'", 2)
                     | epsilon
                     )
            );
mul_expr  = (   pow_expr
                ARBNO(
                    FENCE( $'mod' pow_expr  reduce("'TT_BINOP'", 2)
                         | $'rem' pow_expr  reduce("'TT_BINOP'", 2)
                         | $'div' pow_expr  reduce("'TT_BINOP'", 2)
                         | $'rdiv' pow_expr reduce("'TT_BINOP'", 2)
                         | $'>>'  pow_expr  reduce("'TT_BINOP'", 2)
                         | $'<<'  pow_expr  reduce("'TT_BINOP'", 2)
                         | $'*'   pow_expr  reduce("'TT_MUL'",   2)
                         | $'//'  pow_expr  reduce("'TT_IDIV'",  2)
                         | $'/\'  pow_expr  reduce("'TT_BINOP'", 2)
                         | $'/'   pow_expr  reduce("'TT_DIV'",   2)
                         )
                )
            );
add_expr  = (   mul_expr
                ARBNO(
                    FENCE( $'+' mul_expr  reduce("'TT_ADD'",   2)
                         | $'-' mul_expr  reduce("'TT_SUB'",   2)
                         | $'\/' mul_expr reduce("'TT_BINOP'", 2)
                         | $'xor' mul_expr reduce("'TT_BINOP'", 2)
                         )
                )
            );
colon_expr = (  add_expr
                FENCE( $':' *colon_expr  reduce("'TT_BINOP'", 2)
                     | epsilon
                     )
             );
is_expr   = (   colon_expr
                FENCE( $'is' colon_expr  reduce("'TT_IS'", 2)
                     | epsilon
                     )
            );
cmp_expr  = (   is_expr
                FENCE( $'=:=' is_expr  reduce("'TT_EQQ'",  2)
                     | $'=\=' is_expr  reduce("'TT_NE2'",  2)
                     | $'\==' is_expr  reduce("'TT_NE3'",  2)
                     | $'@>=' is_expr  reduce("'TT_BINOP'",2)
                     | $'@=<' is_expr  reduce("'TT_BINOP'",2)
                     | $'@>'  is_expr  reduce("'TT_BINOP'",2)
                     | $'@<'  is_expr  reduce("'TT_BINOP'",2)
                     | $'>='  is_expr  reduce("'TT_GE'",   2)
                     | $'=<'  is_expr  reduce("'TT_LE'",   2)
                     | $'>'   is_expr  reduce("'TT_GT'",   2)
                     | $'<'   is_expr  reduce("'TT_LT'",   2)
                     | $'\='  is_expr  reduce("'TT_NE1'",  2)
                     | $'=='  is_expr  reduce("'TT_ID'",   2)
                     | epsilon
                     )
            );
unify_expr = (  cmp_expr
                FENCE( $'=..' cmp_expr  reduce("'TT_UNIV'",  2)
                     | $'='   cmp_expr  reduce("'TT_UNIFY'", 2)
                     | epsilon
                     )
             );
/* ==================================================================================================================== */
pfx_kw_name = (   "dynamic" | "discontiguous" | "meta_predicate" | "multifile"
              |   "module_transparent" | "thread_local" | "volatile"
              |   "initialization" | "thread_initialization" | "public" | "table"
              );
body_goal = (   $'(' *body $')'
            |   $' ' pfx_kw_name . pfx_kw $'  ' *unify_expr
                    reduce("'TT_PFX'", 1)
            |   $' ' '\+' $' ' *body_goal  reduce("'TT_NAF'", 1)
            |   unify_expr
            );
conj = (    nPush()
                nInc() body_goal
                ARBNO( $',' nInc() body_goal )
                                   reduce("'TT_CONJ'", 'nTop()')
            nPop()
        );
conj_arrow = ( conj FENCE( $'->' *conj_arrow  reduce("'TT_IFTHEN'", 2)  | epsilon ) );
disj_tail = ( $';' nInc() conj_arrow FENCE( *disj_tail | epsilon ) );
disj = (    nPush()
                nInc() conj_arrow
                FENCE( *disj_tail | epsilon )
                                   reduce("'TT_DISJ'", 'nTop()')
            nPop()
        );
body = disj;
/* head: reduces to a single TT_COMPOUND node (functor + args as children) */
head = (    nPush()
            (   Atom . h_text $'(' args $')'
                    shift(h_text, 'TT_FNC') nInc()
                    reduce("'TT_COMPOUND'", 'nTop()')
            |   Atom . h_text $'(' $')'
                    shift(h_text, 'TT_FNC') nInc()
                    reduce("'TT_COMPOUND'", 'nTop()')
            |   Atom . h_text
                    shift(h_text, 'TT_FNC') nInc()
                    reduce("'TT_COMPOUND'", 'nTop()')
            |   Str
                    shift(s_body, 'TT_FNC') nInc()
                    reduce("'TT_COMPOUND'", 'nTop()')
            )
            nPop()
        );
dcg_goal = (   *list
           |   $'{' body $'}'       reduce("'TT_DCG_IL'", 1)
           |   Tk_cut               reduce("'TT_CUT'", 0)
           |   $'(' *dcg_body $')'
           |   unify_expr
           );
dcg_conj = (   nPush()
                   nInc() dcg_goal
                   ARBNO( $',' nInc() dcg_goal )
                                      reduce("'TT_CONJ'", 'nTop()')
               nPop()
           );
dcg_disj = (   nPush()
                   nInc() dcg_conj
                   ARBNO( $';' nInc() dcg_conj )
                                      reduce("'TT_DISJ'", 'nTop()')
               nPop()
           );
dcg_body = dcg_disj;
dcg_rule  = (   head $'-->'
                dcg_body $'.'
                                      reduce("'TT_DCG_RULE'", 2)
            );
clause    = (   head
                ( $':-' body         reduce("'TT_CLAUSE'", 2)
                | epsilon            reduce("'TT_CLAUSE'", 1)
                )
                $'.'
            );
directive = (   $':-'
                body $'.'
                                     reduce("'TT_DIRECTIVE'", 1)
            );
top_form  = (directive | clause | dcg_rule);
/* ==================================================================================================================== */
skip_to_dot = ( BREAKX('.') $'.' shift('skip', 'TT_FNC') );
top_form_safe = ( top_form | skip_to_dot );
/* SCT-pivot (2026-05-17): nInc() must fire AFTER top_form_safe commits, not before. */
Compiland = nPush()
            POS(0) ARBNO( $' ' FENCE(top_form_safe nInc()) $' ' ) RPOS(0)
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
        while (LE(i, n_kids)) { TDump(c(ptree)[i]); i = i + 1; }
    }
}
