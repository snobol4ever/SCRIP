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
                |  '{$' BREAK('}') . pd '}' . *PasDir(pd)
                |  '{' FENCE(BREAK('}') '}' | REM)
                |  '(*' FENCE(BREAKX('*') '*)' | REM)
                |  '//' FENCE(BREAK(CHAR(10)) | REM)
                );
White      =   *white FENCE(*White | epsilon);
Gray        =   FENCE(*White | epsilon);
$'  '       =   White;
$' '        =   Gray;
Id          =   ANY(&UCASE &LCASE '_') FENCE(SPAN('0123456789' &UCASE '_' &LCASE) | epsilon);
$'and'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'and')       *$' ';
$'begin'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'begin') epsilon . *PasDS()     *$' ';
$'div'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'div')       *$' ';
$'do'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'do')        *$' ';
$'downto'   =   *$' ' *Id $ tx *IDENT(lwr(tx), 'downto')    *$' ';
$'else'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'else')      *$' ';
$'end'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'end')       *$' ';
$'for'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'for')       *$' ';
$'function' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'function') epsilon . *PasDS()  *$' ';
$'if'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'if')        *$' ';
$'mod'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'mod')       *$' ';
$'not'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'not')       *$' ';
$'or'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'or')        *$' ';
$'procedure' =  *$' ' *Id $ tx *IDENT(lwr(tx), 'procedure') epsilon . *PasDS() *$' ';
$'program'  =   *$' ' *Id $ tx *IDENT(lwr(tx), 'program')   *$' ';
$'repeat'   =   *$' ' *Id $ tx *IDENT(lwr(tx), 'repeat')    *$' ';
$'then'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'then')      *$' ';
$'to'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'to')        *$' ';
$'until'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'until')     *$' ';
$'var'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'var') epsilon . *PasDS()       *$' ';
$'while'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'while')     *$' ';
$'with'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'with')      *$' ';
$'uses'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'uses') *DIFFER(dialect, 'iso')      *$' ';
$'out'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'out')       *$' ';
$'on'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'on')        *$' ';
$'except'   =   *$' ' *Id $ tx *IDENT(lwr(tx), 'except')    *$' ';
$'finally'  =   *$' ' *Id $ tx *IDENT(lwr(tx), 'finally')   *$' ';
$'unit'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'unit') *DIFFER(dialect, 'iso')        *$' ';
$'interface' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'interface') *DIFFER(dialect, 'iso')   *$' ';
$'implementation' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'implementation') *DIFFER(dialect, 'iso') *$' ';
$'initialization' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'initialization') *DIFFER(dialect, 'iso') *$' ';
$'finalization' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'finalization') *DIFFER(dialect, 'iso') *$' ';
$'constructor' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'constructor') *DIFFER(dialect, 'iso') *$' ';
$'destructor' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'destructor') *DIFFER(dialect, 'iso')  *$' ';
$'class'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'class') *DIFFER(dialect, 'iso')       *$' ';
$'property'  =   *$' ' *Id $ tx *IDENT(lwr(tx), 'property') *DIFFER(dialect, 'iso')    *$' ';
$'inherited' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'inherited') *DIFFER(dialect, 'iso')   *$' ';
$'is'        =   *$' ' *Id $ tx *IDENT(lwr(tx), 'is') *DIFFER(dialect, 'iso')          *$' ';
$'as'        =   *$' ' *Id $ tx *IDENT(lwr(tx), 'as') *DIFFER(dialect, 'iso')          *$' ';
$'otherwise' =   *$' ' *Id $ tx *IDENT(lwr(tx), 'otherwise')   *$' ';
$'array'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'array')     *$' ';
$'case'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'case')      *$' ';
$'const'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'const') epsilon . *PasDS()     *$' ';
$'file'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'file')      *$' ';
$'forward'  =   *$' ' *Id $ tx *IDENT(lwr(tx), 'forward')   *$' ';
$'goto'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'goto')      *$' ';
$'in'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'in')        *$' ';
$'label'    =   *$' ' *Id $ tx *IDENT(lwr(tx), 'label')     *$' ';
$'of'       =   *$' ' *Id $ tx *IDENT(lwr(tx), 'of')        *$' ';
$'packed'   =   *$' ' *Id $ tx *IDENT(lwr(tx), 'packed')    *$' ';
$'record'   =   *$' ' *Id $ tx *IDENT(lwr(tx), 'record')    *$' ';
$'set'      =   *$' ' *Id $ tx *IDENT(lwr(tx), 'set')       *$' ';
$'type'     =   *$' ' *Id $ tx *IDENT(lwr(tx), 'type') epsilon . *PasDS()      *$' ';
/* The lexer's pragma events (pascal.l pascal_event, pascal_leaf, pascal_flip_dialect): an event is queued where its source token is, and the next leaf made carries it as (TT_ATTR :pragma (TT_QLIT "event")). */
/* Every action below runs in the order the tokens were matched, so the dialect and the pending list are the lexer's own. dialect is pascal.tab.h's enum as ONE global (CEO-1614, ruled by the cto): the null string is ISO_DEFAULT, then 'iso', 'fpc', 'objfpc', 'delphi'; an explicit 'iso' is sticky by being non-null.   */
function PasInit() {
    dialect = '';
    pas_ds = '';
    pas_nev = 0;
    pas_ev = TABLE(15);
    pas_hasprog = '';
    pas_strdecl = '';
    pas_unit = '';
    pas_ek = 0;
    pas_eln = TABLE(31);
    return;
}
/* ==================================================================================================================== */
function PasEv(s) {
    pas_nev = pas_nev + 1;
    pas_ev[pas_nev] = s;
    PasEv = .dummy;
    nreturn;
}
/* ==================================================================================================================== */
function PasFlip(d) {
    PasFlip = .dummy;
    if (IDENT(dialect)) { dialect = d; PasEv('seen_mode'); }
    nreturn;
}
/* ==================================================================================================================== */
function PasDS() {
    pas_ds = 1;
    PasDS = .dummy;
    nreturn;
}
/* ==================================================================================================================== */
function PasStrip(s) {
    while (s ? (ANY(' ' CHAR(9) CHAR(10) CHAR(13))) = ) { ; }
    PasStrip = s;
    return;
}
/* ==================================================================================================================== */
function PasNum(s, n) {
    n = 0;
    if (s ? (POS(0) SPAN('0123456789') . n)) { ; }
    PasNum = n + 0;
    return;
}
/* ==================================================================================================================== */
/* One {$...} directive, the forms pascal.l gives a rule to and no other; a directive that is not one of them is the comment it is.   */
function PasDir(d, l, r, m, fnc) {
    PasDir = .dummy;
    l = lwr(d);
    if (l ? (POS(0) 'mode')) {
        r = PasStrip(SUBSTR(l, 5, SIZE(l) - 4));
        if (IDENT(r, 'iso')) { PasEv('mode_iso'); if (IDENT(pas_ds)) { dialect = 'iso'; } nreturn; }
        m = '';
        fnc = 1;
        if (IDENT(r, 'objfpc')) { m = 'objfpc'; fnc = 0; } else if (IDENT(r, 'delphi')) { m = 'delphi'; } else if (IDENT(r, 'delphiunicode')) { m = 'delphi'; }
        else if (IDENT(r, 'tp')) { m = 'fpc'; } else if (IDENT(r, 'fpc')) { m = 'fpc'; fnc = 0; } else if (IDENT(r, 'macpas')) { m = 'fpc'; }
        if (DIFFER(m) IDENT(pas_ds)) { dialect = m; PasEv('seen_mode'); PasEv('fnamecall ' fnc); }
        nreturn;
    }
    if (l ? (POS(0) 'minenumsize')) { r = PasNum(PasStrip(SUBSTR(l, 12, SIZE(l) - 11))); if (GT(r, 0)) { PasEv('minenum ' r); } nreturn; }
    if (l ? (POS(0) 'packset')) { r = PasNum(PasStrip(SUBSTR(l, 8, SIZE(l) - 7))); if (GT(r, 0)) { PasEv('packset ' r); } nreturn; }
    if (l ? (POS(0) 'packrecords')) { r = PasNum(PasStrip(SUBSTR(l, 12, SIZE(l) - 11))); PasEv('packrec ' r); nreturn; }
    if (l ? (POS(0) 'align')) { r = PasStrip(SUBSTR(l, 6, SIZE(l) - 5)); if (IDENT(r, 'mac68k')) { PasEv('align 1'); } else { PasEv('align 0'); } nreturn; }
    if (l ? (POS(0) 'codepage')) {
        r = PasStrip(SUBSTR(l, 9, SIZE(l) - 8));
        if (r ? (POS(0) 'cp' SPAN('0123456789') . m)) { PasEv('codepage ' (m + 0)); }
        else if (r ? (POS(0) SPAN('0123456789') RPOS(0))) { PasEv('codepage ' (r + 0)); }
        else { PasEv('codepage 0'); }
        nreturn;
    }
    if (IDENT(l, 'push')) { PasEv('push'); nreturn; }
    if (IDENT(l, 'pop')) { PasEv('pop'); nreturn; }
    if (IDENT(l, 'r+')) { PasEv('range 1'); nreturn; }
    if (IDENT(l, 'r-')) { PasEv('range 0'); nreturn; }
    if (IDENT(l, 'q+')) { PasEv('overflow 1'); nreturn; }
    if (IDENT(l, 'q-')) { PasEv('overflow 0'); nreturn; }
    if (l ? (POS(0) 'overflowchecks' SPAN(' ' CHAR(9)) ('on' | 'off') . r FENCE(SPAN(' ' CHAR(9)) | epsilon) RPOS(0))) { PasEv('overflow ' (IDENT(r, 'on') 1, 0)); nreturn; }
    if (l ? (POS(0) 'zerobasedstrings' FENCE(SPAN(' ' CHAR(9)) ('on' | 'off') . r FENCE(SPAN(' ' CHAR(9)) | epsilon) | ('+' | '-') . r) RPOS(0))) {
        PasEv('zerobased ' (IDENT(r, 'on') 1, IDENT(r, '+') 1, 0));
        nreturn;
    }
    nreturn;
}
/* ==================================================================================================================== */
/* A leaf (pascal_leaf): the pending events become its (TT_ATTR :pragma) children, in the order they were raised.  */
function Lf(k, v, i, n) {
    Lf = .dummy;
    n = pas_nev;
    i = 0;
    while (i = LT(i, n) i + 1) {
        Shift('TT_QLIT', pas_ev[i]);
        Reduce('TT_ATTR', 1, ':pragma');
    }
    if (GT(n, 0)) { Reduce(k, n, v); } else { Shift(k, v); }
    pas_nev = 0;
    nreturn;
}
/* ==================================================================================================================== */
/* pascal_note_type_ident / pascal_note_type_decl: the type name string, used before the program declares one, is FPC's.  */
function NoteType(x) {
    NoteType = .dummy;
    if (IDENT(lwr(x), 'string') IDENT(pas_strdecl)) { PasFlip('fpc'); }
    nreturn;
}
/* ==================================================================================================================== */
function NoteDecl(x) {
    NoteDecl = .dummy;
    if (IDENT(lwr(x), 'string')) { pas_strdecl = 1; }
    nreturn;
}
/* ==================================================================================================================== */
function Swap2(a, b) {
    Swap2 = .dummy;
    b = Pop();
    a = Pop();
    Push(b);
    Push(a);
    nreturn;
}
/* ==================================================================================================================== */
/* The empty string '' carries the iso_6_1_7 event with the line it is on (pascal.l: yylineno). The tree is built at the end of the match, so the line numbers are read in a scan of their own */
/* over the same comments and strings the lexer skips: the k-th '' of the text is the k-th '' leaf the build makes.                                                                                 */
function PasNote(ch) {
    PasNote = .dummy;
    if (IDENT(ch, "''")) { pas_en = pas_en + 1; pas_eln[pas_en] = pas_line + 1; }
    while (ch ? (BREAK(CHAR(10)) CHAR(10)) = ) { pas_line = pas_line + 1; }
    nreturn;
}
/* ==================================================================================================================== */
function PasLines() {
    pas_line = 0;
    pas_en = 0;
    Src ? *PasScan;
    return;
}
/* ==================================================================================================================== */
function CatDecode(t, r, p, h, n, i, c) {
    r = '';
    while (DIFFER(t)) {
        if (t ? (POS(0) "'")) {
            t ? (POS(0) "'") = ;
            n = 1;
            while (DIFFER(n)) {
                t ? (POS(0) BREAK("'") . p "'") = ;
                r = r p;
                if (t ? (POS(0) "'")) { t ? (POS(0) "'") = ; r = r "'"; } else { n = ''; }
            }
        } else if (t ? (POS(0) '#$' SPAN('0123456789abcdefABCDEF') . h)) {
            t ? (POS(0) '#$' SPAN('0123456789abcdefABCDEF')) = ;
            h = REPLACE(h, 'abcdef', 'ABCDEF');
            n = 0;
            i = SIZE(h);
            if (GT(i, 1)) { h = SUBSTR(h, i - 1, 2); }
            while (h ? (LEN(1) . c) = ) { '0123456789ABCDEF' ? (BREAK(c) @p); n = n * 16 + p; }
            r = r CHAR(n);
        } else {
            t ? (POS(0) '#' SPAN('0123456789') . h);
            t ? (POS(0) '#' SPAN('0123456789')) = ;
            n = 255;
            if (LT(SIZE(h), 19)) { n = REMDR(h + 0, 256); }
            r = r CHAR(n);
        }
    }
    CatDecode = r;
    return;
}
/* ==================================================================================================================== */
function CatLeaf(t) {
    CatLeaf = .dummy;
    Lf('TT_QLIT', CatDecode(t));
    nreturn;
}
/* ==================================================================================================================== */
function RealLeaf(t, v) {
    RealLeaf = .dummy;
    if (v = CONVERT(t, 'REAL')) { Lf('TT_FLIT', t); } else { PasEv('iso_error'); Lf('TT_FLIT', 'Inf'); }
    nreturn;
}
/* ==================================================================================================================== */
function StrLeaf(t) {
    StrLeaf = .dummy;
    if (IDENT(t)) { pas_ek = pas_ek + 1; PasEv('iso_6_1_7 ' pas_eln[pas_ek]); }
    Lf('TT_QLIT', Unq(t));
    nreturn;
}
/* ==================================================================================================================== */
function PasOO() {
    PasOO = .dummy;
    if (IDENT(dialect)) { PasEv('seen_mode'); dialect = 'objfpc'; } else if (IDENT(dialect, 'fpc')) { dialect = 'objfpc'; }
    nreturn;
}
/* ==================================================================================================================== */
function Drop() {
    Drop = .dummy;
    Pop();
    nreturn;
}
/* ==================================================================================================================== */
function PasUnit() {
    pas_unit = 1;
    PasUnit = .dummy;
    nreturn;
}
/* ==================================================================================================================== */
function PasProg() {
    pas_hasprog = 1;
    PasProg = .dummy;
    nreturn;
}
/* ==================================================================================================================== */
function PasAttr(ev) {
    Shift('TT_QLIT', ev);
    Reduce('TT_ATTR', 1, ':pragma');
    return;
}
/* ==================================================================================================================== */
/* program: pascal_stamp_dialect on the program name (a program with no header: on the root), then pt_flush_pragmas; stamp = seen_mode if the dialect ended at FPC or above.  */
function PasStamp(k, i, a, nm) {
    PasStamp = .dummy;
    if (DIFFER(dialect) DIFFER(dialect, 'iso') IDENT(pas_unit)) {
        if (DIFFER(pas_hasprog)) {
            k = $'@S';
            i = TopCounter() - 1;
            while (i = GT(i, 0) i - 1) { k = next(k); }
            nm = value(k);
            PasAttr('seen_mode');
            a = Pop();
            Append(nm, a);
        } else { PasAttr('seen_mode'); IncCounter(); }
    }
    i = 0;
    while (i = LT(i, pas_nev) i + 1) { PasAttr(pas_ev[i]); IncCounter(); }
    pas_nev = 0;
    nreturn;
}
/* ==================================================================================================================== */
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
AlphaT      =   TABLE(63);
AlphaT['_'] = 1;
alpha_s     =   'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
while (alpha_s ? (LEN(1) . alpha_c) = ) { AlphaT[alpha_c] = 1; }
NextAlpha   =   @nlc *DIFFER(AlphaT[SUBSTR(Src, nlc + 1, 1)]);
IntLeaf     =   ( (SPAN('0123456789') . thx) *NextAlpha epsilon . *LitIntS(thx) | (*Integer) . thx . *LitInt(thx) );
String      =   "'" (*StrChars) . thx . *StrLeaf(thx) "'";
Piece       =   ( *Quoted | '#$' SPAN('0123456789abcdefABCDEF') | '#' SPAN('0123456789') );
PieceRest   =   *Piece FENCE(*PieceRest | epsilon);
StrCatTok   =   (*Piece *PieceRest) . thc . *CatLeaf(thc);
PasChunk    =   ( '{' FENCE(BREAK('}') '}' | REM) | '(*' FENCE(BREAKX('*') '*)' | REM) | '//' FENCE(BREAK(CHAR(10)) | REM) | *Quoted | NOTANY("'{(/") FENCE(BREAK("'{(/") | REM) | ANY("'{(/") );
PasScan     =   POS(0) ARBNO(FENCE(*PasChunk $ pch $ *PasNote(pch))) RPOS(0);
/* An integer literal's value is strtoull's, read as a signed 64-bit: $hex in base 16, leading zeros gone, 2^63 .. 2^64-1 negative; beyond 2^64-1 it is -1 and carries the iso_error pragma (pascal.l). */
function IntVal(t, d, n, i, p, hi, lo, h, l) {
    iv_err = '';
    if (t ? (POS(0) '$' REM . d)) {
        d ? (POS(0) SPAN('0')) = ;
        d = REPLACE(d, 'abcdef', 'ABCDEF');
        n = SIZE(d);
        if (GT(n, 16)) { IntVal = '-1'; return; }
        hi = 0;
        lo = 0;
        i = 0;
        while (i = LT(i, n) i + 1) {
            '0123456789ABCDEF' ? (BREAK(SUBSTR(d, i, 1)) @p);
            if (LT(n - i, 8)) { lo = lo * 16 + p; } else { hi = hi * 16 + p; }
        }
        if (GT(hi, 2147483647)) { hi = hi - 4294967296; }
        IntVal = '' (hi * 4294967296 + lo);
        return;
    }
    d = t;
    d ? (POS(0) SPAN('0')) = ;
    if (IDENT(d)) { d = '0'; }
    n = SIZE(d);
    if (LT(n, 19)) { IntVal = d; return; }
    if (GT(n, 20)) { iv_err = 1; IntVal = '-1'; return; }
    if (EQ(n, 20) LGT(d, '18446744073709551615')) { iv_err = 1; IntVal = '-1'; return; }
    if (EQ(n, 19) LLT(d, '9223372036854775808')) { IntVal = d; return; }
    d ? (LEN(n - 10) . h REM . l);
    IntVal = '' ((h - 1844674406) * 10000000000 + (l - 13709551616));
    return;
}
/* ==================================================================================================================== */
function LitIntS(t, v) {
    LitIntS = .dummy;
    PasEv('iso_error');
    v = IntVal(t);
    Lf('TT_ILIT', v);
    nreturn;
}
/* ==================================================================================================================== */
function LitInt(t, v) {
    LitInt = .dummy;
    v = IntVal(t);
    if (DIFFER(iv_err)) { PasEv('iso_error'); }
    Lf('TT_ILIT', v);
    nreturn;
}
ResT        =   TABLE(35);
ResT['and'] = 1; ResT['array'] = 1; ResT['begin'] = 1; ResT['case'] = 1; ResT['const'] = 1; ResT['div'] = 1; ResT['do'] = 1; ResT['downto'] = 1;
ResT['else'] = 1; ResT['end'] = 1; ResT['file'] = 1; ResT['for'] = 1; ResT['function'] = 1; ResT['goto'] = 1; ResT['if'] = 1; ResT['in'] = 1;
ResT['label'] = 1; ResT['mod'] = 1; ResT['not'] = 1; ResT['of'] = 1; ResT['or'] = 1; ResT['packed'] = 1; ResT['procedure'] = 1;
ResT['program'] = 1; ResT['record'] = 1; ResT['repeat'] = 1; ResT['set'] = 1; ResT['then'] = 1; ResT['to'] = 1; ResT['type'] = 1; ResT['until'] = 1;
ResT['except'] = 1; ResT['finally'] = 1; ResT['otherwise'] = 1; ResT['initialization'] = 1; ResT['finalization'] = 1; ResT['implementation'] = 1; ResT['var'] = 1; ResT['while'] = 1; ResT['with'] = 1;
OoT = TABLE(15);
OoT['class'] = 1; OoT['try'] = 1; OoT['raise'] = 1; OoT['inherited'] = 1; OoT['constructor'] = 1; OoT['destructor'] = 1; OoT['property'] = 1; OoT['is'] = 1; OoT['as'] = 1;
Ident       =   *Id $ tx *IDENT(ResT[lwr(tx)]) ( *IDENT(OoT[lwr(tx)]) | *IDENT(dialect) | *IDENT(dialect, 'iso') | *IDENT(dialect, 'fpc') ) . token;
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
$'+='       =   *$' ' '+=' *$' ';
$'-='       =   *$' ' '-=' *$' ';
$'*='       =   *$' ' '*=' *$' ';
$'**'       =   *$' ' '**' *$' ';
$'/='       =   *$' ' '/=' *$' ';
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
ProcCall        =   epsilon . *PushCounter() (*Ident) . thx . *Lf('TT_VAR', lwr(thx)) . *IncCounter()
                    FENCE(*$'(' (*CallArgs | epsilon) *$')' | epsilon)
                    . *Reduce('TT_FNC', nTop()) . *PopCounter();
FuncCall        =   epsilon . *PushCounter() (*Ident) . thx . *Lf('TT_VAR', lwr(thx)) . *IncCounter()
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
MethodArgs      =   epsilon . *PushCounter() epsilon . *IncCounter() *$'(' *CallArgs *$')' . *Reduce('TT_FNC', nTop()) . *PopCounter();
Postfix         =   ( epsilon . *PushCounter() *IdxTail . *Reduce('TT_IDX', nTop()) . *PopCounter()
                    | *$' ' ('.' *$' ' (*Ident) . thx . *Lf('TT_VAR', lwr(thx)) . *Reduce('TT_FIELD', 2) FENCE(*MethodArgs | epsilon)
                    | '^' *$' ' . *Reduce('TT_DEREF', 1)
                    ));
PostStar        =   FENCE(*Postfix *PostStar | epsilon);
/* true, false and nil are identifiers; #n is a character literal with no value of its own in the dump.  */
Primary         =   ( *$'(' *Expr0 *$')'
                    | *$'inherited' *InhCall
                    | *StrCatTok
                    | '#' *Integer epsilon . *Lf('TT_CHRLIT', '')
                    | *SetCtor
                    | (*Real) . thx . *RealLeaf(thx)
                    | *IntLeaf
                    | *String
                    | *FuncCall
                    | (*Ident) . thx . *Lf('TT_VAR', lwr(thx))
                    );
Expr4           =   *Primary *PostStar;
Expr3           =   *$'@'   *Expr3 . *Reduce('TT_ADDR', 1)
                |   *$'not' *Expr3 . *Reduce('TT_NOT', 1)
                |   *Expr4;
/* Multiplying operators: * / div mod and.                                                */
PowStar         =   FENCE(*$'**' epsilon . *PasFlip('fpc') *Expr3 . *Reduce('TT_POW', 2) *PowStar | epsilon);
Expr3P          =   *Expr3 *PowStar;
MulOp           =   ( *$' ' ('*' *$' '   *Expr3P . *Reduce('TT_MUL', 2)
                    | '/' *$' '   *Expr3P . *Reduce('TT_DIV', 2)
                    ) | *$'div' *Expr3P . *Reduce('TT_IDIV', 2)
                    | *$'mod' *Expr3P . *Reduce('TT_MOD', 2)
                    | *$'and' *Expr3P . *Reduce('TT_CONJ', 2)
                    | *$' ' (*Id $ tx . thy) ( *IDENT(lwr(tx), 'shl') | *IDENT(lwr(tx), 'shr') ) *$' ' epsilon . *Lf('TT_VAR', lwr(thy)) epsilon . *Swap2() epsilon . *PasFlip('fpc') *Expr3P . *Reduce('TT_FNC', 3)
                    | *$' ' (*Id $ tx . thy) *IDENT(lwr(tx), 'as') *DIFFER(dialect, 'iso') *$' ' epsilon . *Lf('TT_VAR', lwr(thy)) epsilon . *Swap2() *Expr3P . *Reduce('TT_FNC', 3)
                    );
Expr2           =   *Expr3P *MulStar;
MulStar         =   FENCE(*MulOp *MulStar | epsilon);
AddOp           =   ( *$' ' ('+' *$' '  *Expr2 . *Reduce('TT_ADD', 2)
                    | '-' *$' '  *Expr2 . *Reduce('TT_SUB', 2)
                    ) | *$'or' *Expr2 . *Reduce('TT_ALT', 2)
                    | *$' ' (*Id $ tx . thy) *IDENT(lwr(tx), 'xor') *$' ' epsilon . *Lf('TT_VAR', lwr(thy)) epsilon . *Swap2() *Expr2 . *Reduce('TT_FNC', 3)
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
                    | *$' ' (*Id $ tx . thy) *IDENT(lwr(tx), 'is') *DIFFER(dialect, 'iso') *$' ' epsilon . *Lf('TT_VAR', lwr(thy)) epsilon . *Swap2() *Expr1 . *Reduce('TT_FNC', 3)
                    );
RelStar         =   FENCE(*RelOp *RelStar | epsilon);
Expr0           =   *Expr1 *RelStar;
/* ==================================================================================================================== */
/* Statement grammar.                                                                    */
/* ==================================================================================================================== */
/* assignment: v := e  ->  TT_ASSIGN(v, e).  The target may be subscripted.              */
AssignTarget    =   *Primary *PostStar;
assign_cmd      =   *AssignTarget ( *$':=' *Expr0 . *Reduce('TT_ASSIGN', 2)
                                  | *$'+=' *Expr0 epsilon . *Shift('TT_KEYWORD', '+=') . *Reduce('TT_ASSIGN', 3)
                                  | *$'-=' *Expr0 epsilon . *Shift('TT_KEYWORD', '-=') . *Reduce('TT_ASSIGN', 3)
                                  | *$'*=' *Expr0 epsilon . *Shift('TT_KEYWORD', '*=') . *Reduce('TT_ASSIGN', 3)
                                  | *$'/=' *Expr0 epsilon . *Shift('TT_KEYWORD', '/=') . *Reduce('TT_ASSIGN', 3) );
/* a call statement through a selector: sel.m -> TT_FNC(TT_FIELD(sel, m)), sel.m(args) -> TT_FNC(TT_FIELD(sel, m), args) (pascal.y call)  */
MidStep         =   ( epsilon . *PushCounter() *IdxTail . *Reduce('TT_IDX', nTop()) . *PopCounter()
                    | *$' ' ('.' *$' ' (*Ident) . thx . *Lf('TT_VAR', lwr(thx)) . *Reduce('TT_FIELD', 2)
                    | '^' *$' ' . *Reduce('TT_DEREF', 1)
                    ));
MethodStmt      =   (*Ident) . thx . *Lf('TT_VAR', lwr(thx)) ARBNO(*MidStep) *$' ' '.' *$' ' (*Ident) . thx . *Lf('TT_VAR', lwr(thx)) . *Reduce('TT_FIELD', 2)
                    epsilon . *PushCounter() epsilon . *IncCounter() FENCE(*$'(' *CallArgs *$')' | epsilon) . *Reduce('TT_FNC', nTop()) . *PopCounter();
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
for_cmd_rest    =   *$' ' (*Ident) . thx . *Lf('TT_VAR', lwr(thx))
                    ( *$':=' *Expr0 (*$'to' . *PushVal('to') | *$'downto' . *PushVal('downto')) *Expr0 *$'do' *Command . *Reduce('TT_FOR', 4, PopVal())
                    | *$'in' *Expr0 *$'do' *Command . *Reduce('TT_FOR', 3, 'in') );
/* case e of c1, c2: S; … end -> TT_CASE(e, TT_ALT(c1, c2), S, …): an arm's constants are one TT_ALT */
CaseConst       =   *Expr0 FENCE(*$'..' *Expr0 . *Reduce('TT_SUBRANGE', 2) | epsilon) . *IncCounter();
/* the constants and the arms repeat greedily: an arm, once parsed, is never re-parsed when a later one refuses */
ConstStar       =   FENCE(*$',' *CaseConst *ConstStar | epsilon);
CaseArm         =   *CaseLabels *$':' *Command . *Reduce('TT_ARM', 2) . *IncCounter();
ArmStar         =   FENCE(*$';' *CaseArm *ArmStar | epsilon);
case_cmd_rest   =   epsilon . *PushCounter() *$' ' *Expr0 . *IncCounter() *$'of'
                    *CaseArm *ArmStar FENCE(*$';' | epsilon)
                    FENCE(*$'else' epsilon . *PushCounter() FENCE(*StmtFirst *StmtStar | epsilon) . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter()
                         . *Reduce('TT_PART', 1, 'else') . *IncCounter()
                         | *$'otherwise' epsilon . *PasFlip('fpc') *StmtList . *Reduce('TT_PART', 1, 'otherwise') . *IncCounter() | epsilon)
                    *$'end' . *Reduce('TT_CASE', nTop()) . *PopCounter();
/* with r1, r2 do S -> TT_FNC(TT_VAR __pas_with, r1, r2, S), the oracle's naming for a lowered form  */
with_cmd_rest   =   epsilon . *PushCounter() *$' '
                    *Expr0 . *IncCounter() ARBNO(*$',' *Expr0 . *IncCounter()) *$'do' *Command . *IncCounter()
                    . *Reduce('TT_WITH', nTop()) . *PopCounter();
/* goto 20 -> (TT_GOTO_U 20); 10: S -> TT_LABEL_DEF(TT_ILIT 10, S)                          */
LabelRef        =   ( *IntLeaf | *Name );
goto_cmd_rest   =   *$' ' *LabelRef . *Reduce('TT_GOTO_U', 1);
label_cmd       =   *LabelRef *$' ' ':' @lc *DIFFER(SUBSTR(Src, lc + 1, 1), '=') *$' ' *Command . *Reduce('TT_LABEL_DEF', 2);
/* an empty statement is legal Pascal wherever a statement may appear: the oracle's TT_SUCCEED */
empty_cmd       =   *$' ' *IDENT(epsilon, epsilon) . *Shift('TT_SUCCEED', '');
CmdT            =   TABLE(8);
kw_cmd          =   *$' ' *Id $ tx *DIFFER(CmdT[lwr(tx)]) ( *IDENT(OoT[lwr(tx)]) | *DIFFER(dialect, 'iso') ) *CmdT[lwr(tx)];
Command         =   *$' ' ( *kw_cmd
                    | *label_cmd
                    | *assign_cmd
                    | *MethodStmt
                    | *ProcCall
                    | *empty_cmd
                    );
/* ==================================================================================================================== */
/* Declarations.  A block's parts keep their rows: TT_PART <kind> of TT_DECL <kind> (pascal.y), types as the grammar builds them. */
/* ==================================================================================================================== */
Name            =   (*Ident) . thx . *Lf('TT_VAR', lwr(thx));
TypeId          =   (*Ident) . thx . *Lf('TT_VAR', lwr(thx)) . *NoteType(thx);
IdList          =   epsilon . *PushCounter() *Name . *IncCounter() ARBNO(*$',' *Name . *IncCounter()) . *Reduce('TT_VLIST', nTop()) . *PopCounter();
/* A type name, or string[n] -> TT_IDX(string, n).  */
TypeName        =   (*Ident) . tnm . *Lf('TT_VAR', lwr(tnm)) FENCE(*$'[' *Expr0 *$']' . *Reduce('TT_IDX', 2) | epsilon) epsilon . *NoteType(tnm);
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
ClassArgs       =   epsilon . *PushCounter() *Name . *IncCounter() ARBNO(*$',' *Name . *IncCounter()) . *Reduce('TT_ARGS', nTop()) . *PopCounter();
PropIndex       =   epsilon . *PushCounter() *$'[' *ParamDecl . *IncCounter() ARBNO(*$';' *ParamDecl . *IncCounter()) *$']' . *Reduce('TT_PARAMS', nTop()) . *PopCounter();
PropSpecs       =   epsilon . *PushCounter() ARBNO(*Name . *IncCounter() *$' ' (*IntLeaf | *Name) . *IncCounter() *$' ') . *Reduce('TT_VLIST', nTop()) . *PopCounter();
ClassMember     =   ( *IdList *$':' *TypeSpec *$';' . *Reduce('TT_DECL', 2, 'field')
                    | epsilon . *PushCounter() *$'procedure' *Name . *IncCounter() *ParamsOpt *$';' . *Reduce('TT_PROCEDURE', nTop()) . *PopCounter()
                    | epsilon . *PushCounter() *$'function' *Name . *IncCounter() *ParamsOpt *$':' *Name . *IncCounter() *$';' . *Reduce('TT_FUNCTION', nTop()) . *PopCounter()
                    | epsilon . *PushCounter() *$'constructor' *Name . *IncCounter() *ParamsOpt *$';' . *Reduce('TT_PROCEDURE', nTop(), 'constructor') . *PopCounter()
                    | epsilon . *PushCounter() *$'destructor' *Name . *IncCounter() *ParamsOpt *$';' . *Reduce('TT_PROCEDURE', nTop(), 'destructor') . *PopCounter()
                    | epsilon . *PushCounter() *$'class' *$'procedure' *Name . *IncCounter() *ParamsOpt *$';' . *Reduce('TT_PROCEDURE', nTop(), 'class') . *PopCounter()
                    | epsilon . *PushCounter() *$'class' *$'function' *Name . *IncCounter() *ParamsOpt *$':' *Name . *IncCounter() *$';' . *Reduce('TT_FUNCTION', nTop(), 'class') . *PopCounter()
                    | epsilon . *PushCounter() *$'property' *Name . *IncCounter()
                      ( *PropIndex *$':' *Name epsilon . *Swap2() | *$':' *Name ) . *IncCounter() *PropSpecs . *IncCounter() *$';' . *Reduce('TT_DECL', nTop(), 'property') . *PopCounter()
                    | (*Ident) . thx . *Lf('TT_KEYWORD', lwr(thx)) *$' ' FENCE(*$';' | epsilon)
                    );
ClassMembers    =   FENCE(*ClassMember . *IncCounter() *ClassMembers | epsilon);
SemiNext        =   @snc *IDENT(SUBSTR(Src, snc + 1, 1), ';');
ClassType       =   epsilon . *PushCounter() *$'class' epsilon . *PasOO() epsilon . *Shift('TT_QLIT', tdn) . *IncCounter()
                    ( *SemiNext . *Reduce('TT_CLASS_DECL', 1, 'forward')
                    | *$'(' *ClassArgs . *IncCounter() (*$')' *$' ') ( *SemiNext epsilon . *Reduce('TT_VLIST', 0) . *IncCounter() . *Reduce('TT_CLASS_DECL', nTop())
                                                          | epsilon . *PushCounter() *ClassMembers . *Reduce('TT_VLIST', nTop()) . *PopCounter() . *IncCounter() *$'end' . *Reduce('TT_CLASS_DECL', nTop()) )
                    | epsilon . *PushCounter() *ClassMembers . *Reduce('TT_VLIST', nTop()) . *PopCounter() . *IncCounter() *$'end' . *Reduce('TT_CLASS_DECL', nTop())
                    ) . *PopCounter();
ProcType        =   ( epsilon . *PushCounter() *$'procedure' *ParamsOpt . *Reduce('TT_DECL', nTop(), 'proctype') . *PopCounter()
                    | epsilon . *PushCounter() *$'function' *ParamsOpt *$':' *TypeId . *IncCounter() . *Reduce('TT_DECL', nTop(), 'functype') . *PopCounter()
                    );
TypeSpec        =   ( *Structured
                    | *ClassType
                    | *ProcType
                    | *$'^' *Name . *Reduce('TT_PTR_TYPE', 1)
                    | *SimpleType
                    );
/* label, const, type and var parts.  */
label_part      =   epsilon . *PushCounter() *$'label' *LabelRef . *IncCounter()
                    ARBNO(*$',' *LabelRef . *IncCounter()) *$';'
                    . *Reduce('TT_PART', nTop(), 'label') . *PopCounter();
FieldValue      =   *Name *$':' *TypedValue . *Reduce('TT_FIELD', 2) . *IncCounter();
RecordAggr      =   epsilon . *PushCounter() *$'(' *FieldValue ARBNO(*$';' *FieldValue) FENCE(*$';' | epsilon) *$')' . *Reduce('TT_VLIST', nTop(), 'record aggregate') . *PopCounter();
Aggregate       =   epsilon . *PushCounter() *$'(' *TypedValue . *IncCounter() *$',' *TypedValue . *IncCounter() ARBNO(*$',' *TypedValue . *IncCounter()) *$')'
                    . *Reduce('TT_VLIST', nTop(), 'aggregate') . *PopCounter();
TypedValue      =   ( *Aggregate | *RecordAggr | *Expr0 );
TConst          =   epsilon . *PushCounter() *Name . *Reduce('TT_VLIST', 1) . *IncCounter() *$':' *TypeSpec . *IncCounter() *$'=' *TypedValue . *IncCounter() *$';'
                    . *Reduce('TT_DECL', nTop(), 'tconst') . *PopCounter() . *IncCounter();
ConstDecl       =   ( *TConst | *Name *$'=' *Expr0 *$';' . *Reduce('TT_DECL', 2, 'const') . *IncCounter() );
ConstDecls      =   FENCE(*ConstDecl *ConstDecls | epsilon);
const_part      =   epsilon . *PushCounter() *$'const' *ConstDecl *ConstDecls . *Reduce('TT_PART', nTop(), 'const') . *PopCounter();
TypeDecl        =   (*Ident) . tdn . *Lf('TT_VAR', lwr(tdn)) *$'=' *TypeSpec *$';' epsilon . *NoteDecl(tdn) . *Reduce('TT_DECL', 2, 'type') . *IncCounter();
TypeDecls       =   FENCE(*TypeDecl *TypeDecls | epsilon);
type_part       =   epsilon . *PushCounter() *$'type' *TypeDecl *TypeDecls . *Reduce('TT_PART', nTop(), 'type') . *PopCounter();
VarGroup        =   *IdList *$':' *TypeSpec FENCE(*$'=' *TypedValue . *Reduce('TT_DECL', 3, 'var') | epsilon . *Reduce('TT_DECL', 2, 'var')) *$';' . *IncCounter();
VarGroups       =   FENCE(*VarGroup *VarGroups | epsilon);
var_part        =   epsilon . *PushCounter() *$'var' *VarGroup *VarGroups . *Reduce('TT_PART', nTop(), 'var') . *PopCounter();
/* Parameters: TT_PARAMS of TT_DECL value|var|const|procedure|function rows; an empty list builds nothing.  */
ParamType       =   ( *$'array' *$'of' *TypeId . *Reduce('TT_ARRAY_TYPE', 1)
                    | *TypeId
                    );
ParamDecl       =   ( epsilon . *PushCounter() *$'procedure' *Name . *IncCounter() *ParamsOpt . *Reduce('TT_DECL', nTop(), 'procedure') . *PopCounter()
                    | epsilon . *PushCounter() *$'function' *Name . *IncCounter() *ParamsOpt FENCE(*$':' *TypeId . *IncCounter() | epsilon)
                      . *Reduce('TT_DECL', nTop(), 'function') . *PopCounter()
                    | *$'var' *IdList *$':' *ParamType . *Reduce('TT_DECL', 2, 'var')
                    | *$'const' *IdList *$':' *ParamType FENCE(*$'=' *Expr0 . *Reduce('TT_DECL', 3, 'const') epsilon . *PasFlip('objfpc') | epsilon . *Reduce('TT_DECL', 2, 'const'))
                    | *$'out' *IdList *$':' *ParamType . *Reduce('TT_DECL', 2, 'var') epsilon . *PasFlip('objfpc')
                    | *IdList *$':' *ParamType FENCE(*$'=' *Expr0 . *Reduce('TT_DECL', 3, 'value') epsilon . *PasFlip('objfpc') | epsilon . *Reduce('TT_DECL', 2, 'value'))
                    );
Params          =   epsilon . *PushCounter() *$'(' *ParamDecl . *IncCounter() ARBNO(*$';' *ParamDecl . *IncCounter()) *$')' . *Reduce('TT_PARAMS', nTop()) . *PopCounter();
ParamsOpt       =   FENCE(*Params . *IncCounter() | *$'(' *$')' | epsilon);
/* procedure P(params); <block>; -> TT_PROCEDURE(TT_VAR P, TT_PARAMS, TT_BLOCK); function F(params): T; adds the result type before the block; forward; is TT_KEYWORD forward. */
Body            =   epsilon . *PushCounter() *$'begin' FENCE(*StmtFirst *StmtStar | epsilon) *$'end' . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter();
Block           =   epsilon . *PushCounter() *Decls *Body . *IncCounter() . *Reduce('TT_BLOCK', nTop()) . *PopCounter();
RdT             =   TABLE(15);
RdT['inline'] = 1; RdT['cdecl'] = 1; RdT['register'] = 1; RdT['stdcall'] = 1; RdT['pascal'] = 1; RdT['overload'] = 1; RdT['safecall'] = 1;
RdT['far'] = 1; RdT['near'] = 1; RdT['noinline'] = 1; RdT['nostackframe'] = 1;
RoutineDir      =   *$' ' (*Id $ tx . thd) *DIFFER(RdT[lwr(tx)]) epsilon . *Lf('TT_VAR', lwr(thd)) epsilon . *Drop() *$';' epsilon . *PasFlip('fpc');
RoutineDirs     =   FENCE(*RoutineDir *RoutineDirs | epsilon);
ProcTail        =   ( *$'forward' *$';' . *Shift('TT_KEYWORD', 'forward') . *IncCounter()
                    | *RoutineDirs *Block . *IncCounter() *$';'
                    );
QName           =   *Name FENCE(*$'.' *Name . *Reduce('TT_FIELD', 2) | epsilon);
proc_decl       =   ( epsilon . *PushCounter() *$'procedure' *QName . *IncCounter() *ParamsOpt *$';' *ProcTail . *Reduce('TT_PROCEDURE', nTop()) . *PopCounter()
                    | epsilon . *PushCounter() *$'constructor' *QName . *IncCounter() *ParamsOpt *$';' *ProcTail . *Reduce('TT_PROCEDURE', nTop(), 'constructor') . *PopCounter()
                    | epsilon . *PushCounter() *$'destructor' *QName . *IncCounter() *ParamsOpt *$';' *ProcTail . *Reduce('TT_PROCEDURE', nTop(), 'destructor') . *PopCounter()
                    | epsilon . *PushCounter() *$'class' epsilon . *PasOO() *$'procedure' *QName . *IncCounter() *ParamsOpt *$';' *ProcTail . *Reduce('TT_PROCEDURE', nTop(), 'class') . *PopCounter()
                    );
func_decl       =   ( epsilon . *PushCounter() *$'function' *QName . *IncCounter() *ParamsOpt FENCE(*$':' *TypeId . *IncCounter() | epsilon) *$';' *ProcTail
                      . *Reduce('TT_FUNCTION', nTop()) . *PopCounter()
                    | epsilon . *PushCounter() *$'class' epsilon . *PasOO() *$'function' *QName . *IncCounter() *ParamsOpt FENCE(*$':' *TypeId . *IncCounter() | epsilon) *$';' *ProcTail
                      . *Reduce('TT_FUNCTION', nTop(), 'class') . *PopCounter()
                    );
/* A block's declaration parts in any order, as pascal.y's decl_part_list takes them: each part, a procedure or a function is one child in source order.  */
uses_part       =   epsilon . *PushCounter() *$'uses' epsilon . *PasFlip('fpc') *IdList . *IncCounter() *$';' . *Reduce('TT_PART', nTop(), 'uses') . *PopCounter();
DeclPart        =   ( *uses_part | *label_part | *const_part | *type_part | *var_part | *proc_decl | *func_decl ) . *IncCounter() FLUSH;
Decls           =   FENCE(*DeclPart *Decls | epsilon);
/* ==================================================================================================================== */
/* Compiland — program header, then the block: TT_VAR <name>, the program parameters as a TT_VLIST, TT_BLOCK.  */
/* ==================================================================================================================== */
program_head    =   FENCE(*$'program' *Name . *IncCounter() epsilon . *PasProg() ARBNO(*$'.' (*Ident) . thx . *Lf('TT_VAR', lwr(thx)) . *Drop()) FENCE(*$'(' *IdList *$')' . *IncCounter() | epsilon) *$';' | epsilon);
IntfProc        =   epsilon . *PushCounter() *$'procedure' *Name . *IncCounter() *ParamsOpt *$';' . *Reduce('TT_PROCEDURE', nTop()) . *PopCounter();
IntfFunc        =   epsilon . *PushCounter() *$'function' *Name . *IncCounter() *ParamsOpt *$':' *TypeId . *IncCounter() *$';' . *Reduce('TT_FUNCTION', nTop()) . *PopCounter();
IntfPart        =   ( *uses_part | *const_part | *type_part | *var_part | *IntfProc | *IntfFunc ) . *IncCounter() FLUSH;
IntfParts       =   FENCE(*IntfPart *IntfParts | epsilon);
InitPart        =   FENCE( ( *$'initialization' | *$'begin' ) *StmtList . *Reduce('TT_PART', 1, 'initialization') . *IncCounter() | epsilon );
FiniPart        =   FENCE( *$'finalization' *StmtList . *Reduce('TT_PART', 1, 'finalization') . *IncCounter() | epsilon );
UnitBody        =   *$'unit' epsilon . *PasFlip('fpc') epsilon . *PasUnit() *Name . *IncCounter() *$';' *$'interface'
                    epsilon . *PushCounter() *IntfParts . *Reduce('TT_PART', nTop(), 'interface') . *PopCounter() . *IncCounter()
                    *$'implementation' epsilon . *PushCounter() *Decls . *Reduce('TT_PART', nTop(), 'implementation') . *PopCounter() . *IncCounter()
                    *InitPart *FiniPart *$'end';
Compiland       =   epsilon . *PushCounter() POS(0) *$' ' ( *UnitBody | *program_head *Block . *IncCounter() )
                    *$' ' FENCE(*$'.' REM | epsilon) *$' ' RPOS(0)
                    epsilon . *PasStamp() . *Reduce('Parse', nTop()) . *PopCounter();
/* ==================================================================================================================== */
/* Driver — byte-identical in shape to the other six parsers.                             */
CmdT['begin'] = compound_cmd_rest; CmdT['if'] = if_cmd_rest; CmdT['while'] = while_cmd_rest; CmdT['repeat'] = repeat_cmd_rest;
StmtList        =   epsilon . *PushCounter() FENCE(*StmtFirst *StmtStar | epsilon) . *Reduce('TT_SEQ_EXPR', nTop()) . *PopCounter();
Handler         =   *$'on' *Name ( *$':' *Name *$'do' *Command . *Reduce('TT_CATCH', 3) | *$'do' *Command . *Reduce('TT_CATCH', 2) );
HandlerList     =   epsilon . *PushCounter() *Handler . *IncCounter() ARBNO(*$';' *Handler . *IncCounter()) FENCE(*$';' | epsilon) . *Reduce('TT_VLIST', nTop()) . *PopCounter();
try_cmd_rest    =   epsilon . *PushCounter() epsilon . *PasOO() *StmtList . *IncCounter()
                    ( *$'except' ( *HandlerList . *IncCounter() FENCE(*$'else' *StmtList . *Reduce('TT_PART', 1, 'else') . *IncCounter() | epsilon) | *StmtList . *IncCounter() )
                      *$'end' . *Reduce('TT_TRY', nTop(), 'except')
                    | *$'finally' *StmtList . *IncCounter() *$'end' . *Reduce('TT_TRY', nTop(), 'finally')
                    ) . *PopCounter();
raise_cmd_rest  =   FENCE(*$' ' *Expr0 . *Reduce('TT_DIE', 1) | epsilon . *Reduce('TT_DIE', 0));
InhCall         =   epsilon . *PushCounter() (*Ident) . thx . *Lf('TT_VAR', lwr(thx)) . *IncCounter() FENCE(*$'(' (*CallArgs | epsilon) *$')' | epsilon)
                    . *Reduce('TT_FNC', nTop()) . *PopCounter() . *Reduce('TT_KEYWORD', 1, 'inherited');
inherited_cmd_rest = FENCE(*$' ' *InhCall | epsilon . *Shift('TT_KEYWORD', 'inherited'));
CmdT['inherited'] = inherited_cmd_rest;
CmdT['try'] = try_cmd_rest; CmdT['raise'] = raise_cmd_rest;
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
function PPDefs(s, w) {
    while (s ? (POS(0) BREAK(' ') . w ' ') = ) { pp_sym[w] = ppdef('1'); }
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
    PPDefs('console cpu64 cpuamd64 cpuathlon64 cpuint64 cpux64 cpux86_64 cpux86_has_cmov cpux86_has_sse2 cpux86_has_sseunit endian_little fpc fpc_abi_default ');
    PPDefs('fpc_dynarraycopy_fixed fpc_has_cextended fpc_has_constref fpc_has_cpstring fpc_has_feature_ansistrings fpc_has_feature_classes ');
    PPDefs('fpc_has_feature_commandargs fpc_has_feature_consoleio fpc_has_feature_dynarrays fpc_has_feature_dynlibs fpc_has_feature_exceptions ');
    PPDefs('fpc_has_feature_exitcode fpc_has_feature_fileio fpc_has_feature_heap fpc_has_feature_initfinal fpc_has_feature_objectivec1 fpc_has_feature_objects ');
    PPDefs('fpc_has_feature_processes fpc_has_feature_random fpc_has_feature_resources fpc_has_feature_rtti fpc_has_feature_softfpu fpc_has_feature_stackcheck ');
    PPDefs('fpc_has_feature_support fpc_has_feature_textio fpc_has_feature_threading fpc_has_feature_unicodestrings fpc_has_feature_variants ');
    PPDefs('fpc_has_feature_widestrings fpc_has_indirect_entry_information fpc_has_internal_abs_int64 fpc_has_internal_abs_long fpc_has_internal_bsf ');
    PPDefs('fpc_has_internal_bsr fpc_has_internal_rox fpc_has_internal_sar fpc_has_membar fpc_has_operator_enumerator fpc_has_resstrinits fpc_has_rip_relative ');
    PPDefs('fpc_has_type_double fpc_has_type_extended fpc_has_type_single fpc_has_unicodestring fpc_has_winlikeresources fpc_link_static fpc_little_endian ');
    PPDefs('fpc_rtti_packset1 fpc_setbase_used fpc_staticripfixed fpc_variantcopy_fixed fpc_widestring_equal_unicodestring fpusse64 hasunix internal_backtrace ');
    PPDefs('linux regcall str_concat_procs unix ver3 ver3_2 ver3_2_2 ');
    pp_sym['fpc_fullversion'] = ppdef('30202');
    pp_sym['fpc_version'] = ppdef('3');
    pp_sym['fpc_release'] = ppdef('2');
    pp_sym['fpc_patch'] = ppdef('2');
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
    } else if (cmd ? (POS(0) ('if' | 'ifc') RPOS(0))) {
        pp_sn = pp_sn + 1;
        pp_stk[pp_sn] = pp_skip;
        if (DIFFER(pp_skip)) { pp_skip = 'off'; }
        else { pp_skip = PPCond(rest); }
        PPEmit(PPNls(tx));
    } else if (IDENT(cmd, 'elseif')) {
        if (LE(pp_sn, pp_base)) { ; }
        else if (IDENT(pp_skip)) { pp_skip = 'done'; }
        else if (IDENT(pp_skip, 'wait')) { pp_skip = PPCond(rest); }
        PPEmit(PPNls(tx));
    } else if (cmd ? (POS(0) ('else' | 'elsec') RPOS(0))) {
        if (LE(pp_sn, pp_base)) { ; }
        else if (IDENT(pp_skip)) { pp_skip = 'done'; }
        else if (IDENT(pp_skip, 'wait')) { pp_skip = ''; }
        PPEmit(PPNls(tx));
    } else if (cmd ? (POS(0) ('endif' | 'ifend' | 'endc') RPOS(0))) {
        if (LE(pp_sn, pp_base)) { ; }
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
/* The dialect of a mode directive is a fact of the text, set BEFORE the match (the cto's ruling, CEO-1614): the first {$mode X} ahead of the first of begin var const type procedure function (the window pascal.l's pascal_seen_decl_start gives pascal_mode_directive) and ahead of uses and unit, the only flips that can come first: past one the in-order deferred PasDir decides, so its seen_mode events keep the C order. PasDir's queued pragma events and every flip stay deferred. */
PmKw = TABLE(8);
PmKw['begin'] = 1; PmKw['var'] = 1; PmKw['const'] = 1; PmKw['type'] = 1; PmKw['procedure'] = 1; PmKw['function'] = 1; PmKw['uses'] = 1; PmKw['unit'] = 1;
PmDia = TABLE(8);
PmDia['iso'] = 'iso'; PmDia['objfpc'] = 'objfpc'; PmDia['delphi'] = 'delphi'; PmDia['delphiunicode'] = 'delphi'; PmDia['tp'] = 'fpc'; PmDia['fpc'] = 'fpc'; PmDia['macpas'] = 'fpc';
pre_ws   = SPAN(' ' CHAR(9) CHAR(10) CHAR(13));
pre_mode = '{$mode' FENCE(pre_ws | epsilon) ('iso' | 'objfpc' | 'delphiunicode' | 'delphi' | 'tp' | 'fpc' | 'macpas') . pre_n FENCE(pre_ws | epsilon) '}';
pre_item = ( pre_ws | '{' FENCE(BREAK('}') '}' | REM) | '(*' FENCE(BREAKX('*') '*)' | REM) | '//' FENCE(BREAK(CHAR(10)) | REM) | "'" FENCE(BREAK("'") "'" | REM) | Id $ pre_w *IDENT(PmKw[pre_w]) | NOTANY(&UCASE &LCASE '_') );
PasPre   = ( *pre_mode | *pre_item *PasPre | epsilon );
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
    PasInit();
    pre_n = '';
    if (lwr(Src) ? *PasPre) { if (DIFFER(pre_n)) { dialect = PmDia[pre_n]; } }
    if (Src ? "''") { PasLines(); }
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
