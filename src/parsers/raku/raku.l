%{
#include "ct_arena.h"
#include "raku.tab.h"
#include "rk_opname.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char raku_strbuf[65536];
static int  raku_strpos;
static int raku_after_smatch  = 0;
static int raku_match_global   = 0;
static char *raku_subst_pat    = NULL;
static int raku_expect_rebody  = 0;
static int rebody_depth   = 0;
void raku_lex_error(const char *msg) {
    fprintf(stderr, "raku lex error line %d: %s\n", raku_yylineno, msg);
}
int raku_get_lineno(void) { return raku_yylineno; }
static int raku_str_line = 0;
static void raku_stamp_yylloc(const char *text, int len) {
    int nl = 0;
    for (int k = 0; k < len; k++) if (text[k] == '\n') nl++;
    raku_yylloc.first_line = raku_yylineno - nl; raku_yylloc.last_line = raku_yylineno;
    raku_yylloc.first_column = raku_yylloc.last_column = 0;
}
#define YY_USER_ACTION raku_stamp_yylloc(yytext, yyleng);
%}
%option noyywrap noinput nounput
%option prefix="raku_yy"
%option yylineno
%x STR_DQ STR_SQ STR_RE STR_SUBST_PAT STR_SUBST_REPL STR_REBODY
%x STR_CORNER
%x POD
DIGIT    [0-9]
ALPHA    [A-Za-z_]
ALNUM    [A-Za-z0-9_]
PHB         [ \t\r\n]*"{"
EXP      [eE][+-]?{DIGIT}+
WS       [ \t\r]
%%
{WS}+           { }
\n              { }
"#"[^\n]*       { }
^"=begin"[^\n]*\n   { BEGIN(POD); }
^"=for"[^\n]*\n     { BEGIN(POD); }
^"="[A-Za-z][^\n]*\n { BEGIN(POD); }
<POD>^"=end"[^\n]*\n { BEGIN(INITIAL); }
<POD>^"=cut"[^\n]*\n { BEGIN(INITIAL); }
<POD>[^\n]*\n        { }
<POD>[^\n]+          { }
<POD><<EOF>>         { BEGIN(INITIAL); yyterminate(); }
"my"            { return KW_MY; }
"our"           { return KW_MY; }
"BEGIN"/{PHB}   { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"CHECK"/{PHB}   { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"INIT"/{PHB}    { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"END"/{PHB}     { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"ENTER"/{PHB}   { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"LEAVE"/{PHB}   { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"KEEP"/{PHB}    { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"UNDO"/{PHB}    { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"PRE"/{PHB}     { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"POST"/{PHB}    { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"FIRST"/{PHB}   { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"LAST"/{PHB}    { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"NEXT"/{PHB}    { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"TEMP"/{PHB}    { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"once"/{PHB}    { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"quietly"/{PHB} { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"react"/{PHB}   { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"do"/{PHB}      { raku_yylval.sval = ct_strdup(yytext); return PHASER; }
"use"           { return KW_USE; }
"plan"          { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"ok"            { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"nok"           { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"is"            { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"isnt"          { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"done-testing"  { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"skip-rest"     { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"skip"          { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"todo"          { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"diag"          { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"pass"          { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"flunk"         { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"subtest"       { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"is-deeply"     { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"isa-ok"        { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"does-ok"       { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"cmp-ok"        { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"lives-ok"      { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"dies-ok"       { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"throws-like"   { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"eval-lives-ok" { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"eval-dies-ok"  { raku_yylval.sval = ct_strdup(yytext); return TESTOP; }
"say"           { return KW_SAY; }
"join"/"("      { raku_yylval.sval = ct_strdup(yytext); return IDENT; }
"join"          { return KW_JOIN; }
"print"         { return KW_PRINT; }
"if"            { return KW_IF; }
"else"          { return KW_ELSE; }
"elsif"         { return KW_ELSIF; }
"while"         { return KW_WHILE; }
"for"           { return KW_FOR; }
"given"         { return KW_GIVEN; }
"without"       { return KW_WITHOUT; }
"with"          { return KW_WITH; }
"when"          { return KW_WHEN; }
"default"       { return KW_DEFAULT; }
"sub"           { return KW_SUB; }
"constant"      { return KW_CONSTANT; }
"enum"          { return KW_ENUM; }
"gather"        { return KW_GATHER; }
"take"          { return KW_TAKE; }
"return"        { return KW_RETURN; }
"exit"          { return KW_EXIT; }
"exists"        { return KW_EXISTS; }
"delete"        { return KW_DELETE; }
"unless"        { return KW_UNLESS; }
"until"         { return KW_UNTIL; }
"repeat"        { return KW_REPEAT; }
"loop"          { return KW_LOOP; }
"last"          { return KW_LAST; }
"next"          { return KW_NEXT; }
"map"           { return KW_MAP; }
"grep"          { return KW_GREP; }
"sort"          { return KW_SORT; }
"reverse"       { return KW_REVERSE; }
"try"           { return KW_TRY; }
"catch"         { return KW_CATCH; }
"CATCH"         { return KW_CATCH; }
"die"           { return KW_DIE; }
"fail"          { return KW_FAIL; }
"class"         { return KW_CLASS; }
"role"          { return KW_ROLE; }
"grammar"       { return KW_GRAMMAR; }
"module"        { return KW_MODULE; }
"token"         { raku_expect_rebody = 1; return KW_TOKEN; }
"rule"          { raku_expect_rebody = 1; return KW_RULE; }
"regex"         { raku_expect_rebody = 1; return KW_REGEX; }
"method"        { return KW_METHOD; }
"submethod"     { return KW_METHOD; }
"multi"         { return KW_MULTI; }
"proto"         { return KW_PROTO; }
"infix:<"[^>]+">"   { char raw[64]; int rl=(int)yyleng-8; if(rl>63)rl=63; memcpy(raw,yytext+7,rl); raw[rl]=0; char cb[96]; rk_op_canon_base("infix",raw,cb,sizeof cb); raku_yylval.sval=ct_strdup(cb); return OP_NAME; }
"prefix:<"[^>]+">"  { char raw[64]; int rl=(int)yyleng-9; if(rl>63)rl=63; memcpy(raw,yytext+8,rl); raw[rl]=0; char cb[96]; rk_op_canon_base("prefix",raw,cb,sizeof cb); raku_yylval.sval=ct_strdup(cb); return OP_NAME; }
"postfix:<"[^>]+">" { char raw[64]; int rl=(int)yyleng-10; if(rl>63)rl=63; memcpy(raw,yytext+9,rl); raw[rl]=0; char cb[96]; rk_op_canon_base("postfix",raw,cb,sizeof cb); raku_yylval.sval=ct_strdup(cb); return OP_NAME; }
":exists"       { return ADV_EXISTS; }
":delete"       { return ADV_DELETE; }
":D"            { return OP_COLON_D; }
":U"            { return OP_COLON_U; }
":$"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return VAR_SCALAR; }
"has"           { return KW_HAS; }
"handles"[ \t]*"<"[^>]*">"  { const char *lt = strchr(yytext,'<'); const char *gt = strrchr(yytext,'>'); int n=(int)(gt-lt-1); if(n<0)n=0; char *w=(char*)ct_alloc(n+1); memcpy(w,lt+1,n); w[n]=0; raku_yylval.sval=w; return KW_HANDLES; }
"new"           { return KW_NEW; }
"self"          { raku_yylval.sval = ct_strdup("self"); return VAR_SCALAR; }
"True"          { raku_yylval.ival = 1; return LIT_BOOL; }
"False"         { raku_yylval.ival = 0; return LIT_BOOL; }
"eq"            { return OP_SEQ; }
"ne"            { return OP_SNE; }
"lt"            { return OP_SLT; }
"le"            { return OP_SLE; }
"gt"            { return OP_SGT; }
"ge"            { return OP_SGE; }
"cmp"           { return OP_CMPG; }
"leg"           { return OP_LEG; }
[ \t]+("xx"|"x")/[ \t]*"=>"  { const char *_p = yytext; while (*_p == ' ' || *_p == '\t') _p++; raku_yylval.sval = ct_strdup(_p); return IDENT; }
[ \t]+"xx"/[^A-Za-z0-9_-]  { return OP_REP_XX; }
[ \t]+"x"/[^A-Za-z0-9_-]   { if (raku_expect_rebody) { raku_yylval.sval = ct_strdup("x"); return IDENT; } return OP_REP_X; }
"div"           { return OP_DIV; }
[ 	]+"gcd"/[ 	]    { return OP_GCD; }
[ 	]+"lcm"/[ 	]    { return OP_LCM; }
[ 	]+"mod"/[ 	]    { return OP_MODW; }
"~&"            { return OP_NBAND; }
"\xc3\x97"      { return OP_UMUL; }
"\xc3\xb7"      { return OP_UDIV; }
"+|"            { return OP_BORT; }
"~|"            { return OP_NBOR; }
"?|"            { return OP_QBOR; }
"?^"            { return OP_QBXOR; }
"\xe2\x88\x92"  { return OP_UMINUS_I; }
[ 	]+"o"/[ 	]    { return OP_COMPOSE; }
"\xe2\x88\x98"  { return OP_COMPOSEU; }
"(&)"           { return OP_SETINT; }
"(.)"           { return OP_SETMUL; }
"(|)"           { return OP_SETUNI; }
"(+)"           { return OP_SETSUM; }
"(-)"           { return OP_SETDIF; }
"(^)"           { return OP_SETSYM; }
"^.."           { return OP_RANGE_XL; }
"^..^"          { return OP_RANGE_XB; }
[ 	]+"coll"/[ 	]    { return OP_COLL; }
[ 	]+"unicmp"/[ 	]    { return OP_UNICMP; }
"==="           { return OP_IDENT3; }
[ 	]+"eqv"/[ 	]    { return OP_EQV; }
[ 	]+"before"/[ 	]    { return OP_BEFORE; }
[ 	]+"after"/[ 	]    { return OP_AFTER; }
"(cont)"        { return OP_SETCONT; }
"(elem)"        { return OP_SETELEM; }
"=~="           { return OP_APPROX; }
"!~~"           { return OP_NSMARTM; }
[ 	]+"min"/[ 	]    { return OP_MINOP; }
[ 	]+"max"/[ 	]    { return OP_MAXOP; }
"^^"            { return OP_XOROP; }
"..^"           { return OP_RANGE_EX; }
"..."           { return YADA; }
"^"             { return CARET; }
".."            { return OP_RANGE; }
"->"            { return OP_ARROW; }
"=="            { return OP_EQ; }
"!="            { return OP_NE; }
"<=>"           { return OP_CMP3; }
"≤"          { return OP_LE; }
"≥"          { return OP_GE; }
"≠"          { return OP_NE; }
"<="            { return OP_LE; }
">="            { return OP_GE; }
"&&"            { return OP_AND; }
"||"            { return OP_OR; }
"??"            { return OP_TERNARY1; }
"!!"            { return OP_TERNARY2; }
"|"             { return '|'; }
"&"             { return '&'; }
":="            { return OP_BIND; }
"~~"            { raku_after_smatch = 1; return OP_SMATCH; }
"++"            { return OP_INC; }
"--"            { return OP_DEC; }
"+="            { return OP_ADD_EQ; }
"-="            { return OP_SUB_EQ; }
"*="            { return OP_MUL_EQ; }
"/="            { return OP_DIV_EQ; }
"~="            { return OP_CAT_EQ; }
"//"            { return OP_DOR; }
"+&"            { return OP_BAND; }
"+<"            { return OP_SHL; }
"+"             { return '+'; }
"-"             { return '-'; }
"**"            { return OP_POW; }
"*"             { return '*'; }
"%%"            { return OP_DIVIS; }
"%"             { return '%'; }
"~"             { return '~'; }
"<"[ \t]*[a-zA-Z0-9_.:/*+?=!$,;#@|\\-]+([ \t]+[a-zA-Z0-9_.:/*+?=!$,;#@|\\-]+)+[ \t]*">"  {
    int n=(int)yyleng-2; char *inner=(char*)ct_alloc(n+1); memcpy(inner,yytext+1,n); inner[n]='\0';
    raku_yylval.sval=inner; return WORDLIST; }
"<"             { return '<'; }
">"             { return '>'; }
"!"             { return '!'; }
"="             { return '='; }
"{"             { if (raku_expect_rebody) { raku_expect_rebody = 0; raku_strpos = 0; rebody_depth = 1; BEGIN(STR_REBODY); } else { return '{'; } }
"}"             { return '}'; }
"("             { return '('; }
")"             { return ')'; }
"[+]"           { raku_yylval.sval = ct_strdup("+");   return OP_REDUCE; }
"[-]"           { raku_yylval.sval = ct_strdup("-");   return OP_REDUCE; }
"[*]"           { raku_yylval.sval = ct_strdup("*");   return OP_REDUCE; }
"[~]"           { raku_yylval.sval = ct_strdup("~");   return OP_REDUCE; }
"[min]"         { raku_yylval.sval = ct_strdup("min"); return OP_REDUCE; }
"[max]"         { raku_yylval.sval = ct_strdup("max"); return OP_REDUCE; }
"["             { return '['; }
"]"             { return ']'; }
";"             { return ';'; }
","             { return ','; }
":"             { return ':'; }
"\.="           { return OP_DOTEQ; }
"."             { return '.'; }
"=>"            { return OP_FATARROW; }
{DIGIT}+"."{DIGIT}+{EXP}?  { raku_yylval.dval = atof(yytext); return LIT_FLOAT; }
{DIGIT}+{EXP}               { raku_yylval.dval = atof(yytext); return LIT_FLOAT; }
{DIGIT}+                    { raku_yylval.ival = atol(yytext); return LIT_INT; }
"$_"            { raku_yylval.sval = ct_strdup("$_"); return VAR_SCALAR; }
"$*STDIN"       { raku_yylval.ival = 0; return VAR_FH; }
"$*STDOUT"      { raku_yylval.ival = 1; return VAR_FH; }
"$*STDERR"      { raku_yylval.ival = 2; return VAR_FH; }
"$"[0-9]+       { raku_yylval.ival = atoi(yytext+1); return VAR_CAPTURE; }
"$["            { return DOLLAR_LBRACKET; }
"$<"[a-zA-Z][a-zA-Z0-9_]*">" {
    int len = strlen(yytext)-3;
    char *nm = ct_alloc(len+1); memcpy(nm,yytext+2,len); nm[len]='\0';
    raku_yylval.sval = nm; return VAR_NAMED_CAPTURE; }
"$."{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return VAR_TWIGIL; }
"$!"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return VAR_TWIGIL; }
"@."{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return VAR_ARRAY_TWIGIL; }
"@!"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return VAR_ARRAY_TWIGIL; }
"%."{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return VAR_HASH_TWIGIL; }
"%!"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return VAR_HASH_TWIGIL; }
"$?LINE"        { raku_yylval.ival = raku_yylineno; return LIT_INT; }
"$*"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext); return VAR_SCALAR; }
"$?"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext); return VAR_SCALAR; }
"$^"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext); return VAR_SCALAR; }
"$="{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext); return VAR_SCALAR; }
"$~"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext); return VAR_SCALAR; }
"$!"            { raku_yylval.sval = ct_strdup("$!"); return VAR_SCALAR; }
"@*"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext); return VAR_ARRAY; }
"%*"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext); return VAR_HASH; }
"$"{ALPHA}{ALNUM}*("-"{ALPHA}{ALNUM}*)*  { raku_yylval.sval = ct_strdup(yytext); return VAR_SCALAR; }
"**@"{ALPHA}{ALNUM}* { raku_yylval.sval = ct_strdup(yytext+2); return SLURPY_LOL; }
"*@"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return SLURPY_POS; }
"*%"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return SLURPY_NAMED; }
"@"{ALPHA}{ALNUM}*"[*]"  { char *s = ct_strdup(yytext); s[yyleng - 3] = '\0'; raku_yylval.sval = s; return ARR_ALL_SLICE; }
"@"{ALPHA}{ALNUM}*("-"{ALPHA}{ALNUM}*)*  { raku_yylval.sval = ct_strdup(yytext); return VAR_ARRAY; }
"%"{ALPHA}{ALNUM}*("-"{ALPHA}{ALNUM}*)*  { raku_yylval.sval = ct_strdup(yytext); return VAR_HASH; }
"\\"{ALPHA}{ALNUM}*  { raku_yylval.sval = ct_strdup(yytext+1); return VAR_SCALAR; }
{ALPHA}{ALNUM}*("::"{ALPHA}{ALNUM}*)+ { raku_yylval.sval = ct_strdup(yytext); return IDENT; }
{ALPHA}{ALNUM}*("-"{ALPHA}{ALNUM}*)* { raku_yylval.sval = ct_strdup(yytext); return IDENT; }
"\""            { raku_strpos = 0; raku_str_line = raku_yylineno; BEGIN(STR_DQ); }
<STR_DQ>"\"" {
    raku_strbuf[raku_strpos] = '\0';
    raku_yylloc.first_line = raku_str_line;
    raku_yylval.sval = ct_strdup(raku_strbuf);
    BEGIN(INITIAL);
    if (strchr(raku_strbuf, '$') != NULL || strchr(raku_strbuf, '@') != NULL)
        return LIT_INTERP_STR;
    return LIT_STR;
}
<STR_DQ>"\\n"  { raku_strbuf[raku_strpos++] = '\n'; }
<STR_DQ>"\\t"  { raku_strbuf[raku_strpos++] = '\t'; }
<STR_DQ>"\\\\" { raku_strbuf[raku_strpos++] = '\\'; }
<STR_DQ>"\\\"" { raku_strbuf[raku_strpos++] = '"'; }
<STR_DQ>\n     { raku_strbuf[raku_strpos++] = '\n'; }
<STR_DQ>.      { raku_strbuf[raku_strpos++] = yytext[0]; }
"'"             { raku_strpos = 0; raku_str_line = raku_yylineno; BEGIN(STR_SQ); }
"｢"         { raku_strpos = 0; BEGIN(STR_CORNER); }
<STR_CORNER>"｣"  {
    raku_strbuf[raku_strpos] = '\0';
    raku_yylloc.first_line = raku_str_line;
    raku_yylval.sval = ct_strdup(raku_strbuf);
    BEGIN(INITIAL);
    return LIT_STR;
}
<STR_CORNER>\n    { if (raku_strpos < 65534) raku_strbuf[raku_strpos++] = '\n'; }
<STR_CORNER>.      { if (raku_strpos < 65534) raku_strbuf[raku_strpos++] = yytext[0]; }
<STR_SQ>"'"    {
    raku_strbuf[raku_strpos] = '\0';
    raku_yylloc.first_line = raku_str_line;
    raku_yylval.sval = ct_strdup(raku_strbuf);
    BEGIN(INITIAL);
    return LIT_STR;
}
<STR_SQ>"\\'"  { raku_strbuf[raku_strpos++] = '\''; }
<STR_SQ>"\\\\" { raku_strbuf[raku_strpos++] = '\\'; }
<STR_SQ>\n     { raku_strbuf[raku_strpos++] = '\n'; }
<STR_SQ>.      { raku_strbuf[raku_strpos++] = yytext[0]; }
"s/"            { if (raku_after_smatch) { raku_after_smatch=0; raku_strpos=0; BEGIN(STR_SUBST_PAT); } else { yyless(1); return 's'; } }
<STR_SUBST_PAT>"/"  {
    raku_strbuf[raku_strpos]='\0';
    raku_subst_pat = ct_strdup(raku_strbuf);
    raku_strpos=0; BEGIN(STR_SUBST_REPL);
}
<STR_SUBST_PAT>"\\/"  { raku_strbuf[raku_strpos++]='/'; }
<STR_SUBST_PAT>.       { raku_strbuf[raku_strpos++]=yytext[0]; }
<STR_SUBST_REPL>"/"[g]? {
    raku_strbuf[raku_strpos]='\0';
    int global = (yytext[1]=='g');
    size_t plen=strlen(raku_subst_pat), rlen=raku_strpos;
    char *tok = ct_alloc(plen+rlen+4);
    memcpy(tok, raku_subst_pat, plen); tok[plen]='\x01';
    memcpy(tok+plen+1, raku_strbuf, rlen); tok[plen+1+rlen]='\x01';
    tok[plen+2+rlen] = global ? 'g' : '-'; tok[plen+3+rlen]='\0';
    ct_drop(raku_subst_pat); raku_subst_pat=NULL;
    raku_yylval.sval = tok;
    BEGIN(INITIAL); return LIT_SUBST;
}
<STR_SUBST_REPL>"\\/"  { raku_strbuf[raku_strpos++]='/'; }
<STR_SUBST_REPL>.       { raku_strbuf[raku_strpos++]=yytext[0]; }
"m:g/"          { if (raku_after_smatch) { raku_after_smatch=0; raku_strpos=0; BEGIN(STR_RE); raku_match_global=1; } else { yyless(3); return IDENT; } }
"/"             { if (raku_after_smatch) { raku_after_smatch=0; raku_strpos=0; BEGIN(STR_RE); } else { return '/'; } }
<STR_RE>"/"    {
    raku_strbuf[raku_strpos] = '\0';
    raku_yylloc.first_line = raku_str_line;
    raku_yylval.sval = ct_strdup(raku_strbuf);
    BEGIN(INITIAL);
    if (raku_match_global) { raku_match_global=0; return LIT_MATCH_GLOBAL; }
    return LIT_REGEX;
}
<STR_RE>"\\/"  { raku_strbuf[raku_strpos++] = '/'; }
<STR_RE>\n     { raku_strbuf[raku_strpos++] = '\n'; }
<STR_RE>.      { raku_strbuf[raku_strpos++] = yytext[0]; }
<STR_REBODY>"{"  { if (raku_strpos < 65534) raku_strbuf[raku_strpos++] = '{'; rebody_depth++; }
<STR_REBODY>"}"  {
    rebody_depth--;
    if (rebody_depth == 0) {
        raku_strbuf[raku_strpos] = '\0';
        raku_yylval.sval = ct_strdup(raku_strbuf);
        BEGIN(INITIAL);
        return LIT_REGEX;
    }
    if (raku_strpos < 65534) raku_strbuf[raku_strpos++] = '}';
}
<STR_REBODY>"\\".  { if (raku_strpos < 65533) { raku_strbuf[raku_strpos++] = yytext[0]; raku_strbuf[raku_strpos++] = yytext[1]; } }
<STR_REBODY>\n     { if (raku_strpos < 65534) raku_strbuf[raku_strpos++] = '\n'; }
<STR_REBODY>.      { if (raku_strpos < 65534) raku_strbuf[raku_strpos++] = yytext[0]; }
.               {
    char buf[64];
    snprintf(buf, sizeof buf, "unexpected char '%c'", yytext[0]);
    raku_lex_error(buf);
}
%%
