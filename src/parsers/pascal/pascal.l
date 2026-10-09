%{
#include "ct_arena.h"
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "pascal.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
static char pascal_strbuf[65536];
static int  pascal_strpos;
static char pascal_ifdefbuf[256];
static int  pascal_ifdefpos;
int pascal_cenv_int(const char *name, long long *out);
int pascal_cenv_real(const char *name, double *out);
const char *pascal_cenv_str(const char *name);
static char **pascal_ev; static int pascal_nev, pascal_cev;
static void pascal_event(const char *s) {
    if (pascal_nev >= pascal_cev) { pascal_cev = pascal_cev ? pascal_cev * 2 : 8; pascal_ev = (char **)ct_grow(pascal_ev, (size_t)pascal_cev * sizeof(char *)); }
    pascal_ev[pascal_nev++] = ct_strdup(s);
}
static void pascal_event_num(const char *name, long long v) { size_t n = strlen(name) + 24; char b[n]; snprintf(b, n, "%s %lld", name, v); pascal_event(b); }
static tree_t *pascal_attr_of(const char *ev) { tree_t *a = ast_node_new(TT_ATTR); tree_t *q = ast_node_new(TT_QLIT); a->v.sval = ct_strdup(":pragma"); q->v.sval = ct_strdup(ev); ast_push(a, q); return a; }
static tree_t *pascal_leaf(tree_e kind) {
    tree_t *l = ast_node_new(kind); l->line = pascal_yylineno;
    for (int i = 0; i < pascal_nev; i++) ast_push(l, pascal_attr_of(pascal_ev[i]));
    pascal_nev = 0;
    return l;
}
void pascal_pragma_flush(tree_t *root) { for (int i = 0; root && i < pascal_nev; i++) ast_push(root, pascal_attr_of(pascal_ev[i])); pascal_nev = 0; }
static long long pascal_int_lit(const char *t) {
    errno = 0; unsigned long long v = strtoull(t, NULL, 10);
    if (errno == ERANGE) { fprintf(stderr, "pascal: ISO 7185 6.1.5 violation line %d: the integer literal '%.24s%s' is beyond the largest integer SCRIP represents\n",
                                   pascal_yylineno, t, strlen(t) > 24 ? "..." : ""); pascal_event("iso_error"); }
    return (long long)v;
}
static double pascal_real_lit(const char *t) {
    errno = 0; double d = strtod(t, NULL);
    if (errno == ERANGE && isinf(d)) { fprintf(stderr, "pascal: ISO 7185 6.1.5 violation line %d: the real literal '%.24s%s' is beyond the largest real SCRIP represents\n",
                                               pascal_yylineno, t, strlen(t) > 24 ? "..." : ""); pascal_event("iso_error"); }
    return d;
}
static int pascal_if_declared(const char *name) {
    long long iv; double rv;
    return pascal_cenv_int(name, &iv) || pascal_cenv_real(name, &rv) || pascal_cenv_str(name) != NULL;
}
static int pascal_if_eval(const char *expr) {
    char buf[256]; int bn = 0;
    for (const char *s = expr; *s && bn < (int)sizeof(buf) - 1; s++) buf[bn++] = (char)tolower((unsigned char)*s);
    buf[bn] = '\0';
    char *p = buf; while (*p == ' ' || *p == '\t') p++;
    int neg = 0;
    if (!strncmp(p, "not ", 4)) { neg = 1; p += 4; while (*p == ' ') p++; }
    int result = 0;
    char ident[128];
    if (sscanf(p, "declared(%127[^)])", ident) == 1) {
        result = pascal_if_declared(ident);
    } else {
        char op[3] = { 0, 0, 0 }; long long rhs;
        if (sscanf(p, "%127[a-z_0-9] %2[<>=] %lld", ident, op, &rhs) == 3) {
            long long lv;
            if (pascal_cenv_int(ident, &lv)) {
                if      (!strcmp(op, ">=")) result = lv >= rhs;
                else if (!strcmp(op, "<=")) result = lv <= rhs;
                else if (!strcmp(op, ">"))  result = lv > rhs;
                else if (!strcmp(op, "<"))  result = lv < rhs;
                else if (!strcmp(op, "="))  result = lv == rhs;
                else if (!strcmp(op, "<>")) result = lv != rhs;
            }
        }
    }
    return neg ? !result : result;
}
int pascal_seen_decl_start = 0;
static char pascal_modebuf[64];
static int  pascal_modepos;
static char pascal_packrecbuf[16];
static int  pascal_packrecpos;
static char pascal_alignbuf[32];
static int  pascal_alignpos;
static char pascal_codepagebuf[32];
static int  pascal_codepagepos;
static int pas_codepage_parse(const char *s) {
    if (!s) return 0;
    while (*s == ' ' || *s == '\t') s++;
    if ((s[0] == 'c' || s[0] == 'C') && (s[1] == 'p' || s[1] == 'P') && isdigit((unsigned char)s[2])) return atoi(s + 2);
    int alldig = *s != '\0';
    for (const char *p = s; *p; p++) if (!isdigit((unsigned char)*p)) { alldig = 0; break; }
    return alldig ? atoi(s) : 0;
}
static char pascal_minenumbuf[16];
static int  pascal_minenumpos;
static char pascal_packsetbuf[16];
static int  pascal_packsetpos;
static int pascal_mode_is_known(const char *m) {
    static const char *K[] = { "objfpc", "delphi", "tp", "fpc", "macpas" };
    for (size_t i = 0; i < sizeof(K) / sizeof(K[0]); i++) if (!strcasecmp(m, K[i])) return 1;
    return 0;
}
static char *pascal_raw_dup(const char *s, int n) {
    char *b = (char *)ct_alloc((size_t)(n + 1)); memcpy(b, s, (size_t)n); b[n] = '\0'; return b;
}
static char *pascal_lower_dup(const char *s, int n) {
    char *b = pascal_raw_dup(s, n);
    for (int i = 0; i < n; i++) b[i] = (char)tolower((unsigned char)b[i]);
    return b;
}
void pascal_lex_error(const char *msg) { fprintf(stderr, "pascal lex error line %d: %s\n", pascal_yylineno, msg); }
int  pascal_get_lineno(void) { return pascal_yylineno; }
%}
%option noyywrap noinput nounput
%option prefix="pascal_yy"
%option yylineno
%option caseless
%x COMMENT BRACE IFDEF_HDR IFDEF_BODY IFDEF_END IFDEF_ELSE1 IFDEF_ELSE2 MODE_HDR IF_HDR MINENUM_HDR PACKSET_HDR ALIGN_HDR PACKREC_HDR CODEPAGE_HDR
%s IFDEF_TRUE
DIGIT    [0-9]
ALPHA    [A-Za-z_]
ALNUM    [A-Za-z0-9_]
%%
[ \t\r\n]+              { }
"(*"                    { BEGIN COMMENT; }
<COMMENT>"*)"           { BEGIN INITIAL; }
<COMMENT>.              { }
<COMMENT>\n             { }
"{$if"[ \t]             { pascal_ifdefbuf[0] = '\0'; pascal_ifdefpos = 0; BEGIN IF_HDR; }
<IF_HDR>"}"             { BEGIN (pascal_if_eval(pascal_ifdefbuf) ? IFDEF_TRUE : IFDEF_BODY); }
<IF_HDR>.               { if (pascal_ifdefpos < (int)sizeof(pascal_ifdefbuf) - 1) { pascal_ifdefbuf[pascal_ifdefpos++] = pascal_yytext[0]; pascal_ifdefbuf[pascal_ifdefpos] = '\0'; } }
<IF_HDR>\n              { }
"{$ifdef"               { pascal_ifdefbuf[0] = '\0'; pascal_ifdefpos = 0; BEGIN IFDEF_HDR; }
<IFDEF_HDR>"}"          { BEGIN (strcasecmp(pascal_ifdefbuf, "fpc") == 0 ? IFDEF_TRUE : IFDEF_BODY); }
<IFDEF_HDR>[ \t]        { }
<IFDEF_HDR>.            { if (pascal_ifdefpos < (int)sizeof(pascal_ifdefbuf) - 1) { pascal_ifdefbuf[pascal_ifdefpos++] = pascal_yytext[0]; pascal_ifdefbuf[pascal_ifdefpos] = '\0'; } }
<IFDEF_HDR>\n           { }
<IFDEF_TRUE>"{$else"    { BEGIN IFDEF_ELSE1; }
<IFDEF_ELSE1>"}"        { BEGIN IFDEF_BODY; }
<IFDEF_ELSE1>.          { }
<IFDEF_ELSE1>\n         { }
<IFDEF_BODY>"{$else"    { BEGIN IFDEF_ELSE2; }
<IFDEF_ELSE2>"}"        { BEGIN IFDEF_TRUE; }
<IFDEF_ELSE2>.          { }
<IFDEF_ELSE2>\n         { }
<IFDEF_BODY>"{$endif"   { BEGIN IFDEF_END; }
<IFDEF_BODY>"{$ifend"   { BEGIN IFDEF_END; }
<IFDEF_BODY>.           { }
<IFDEF_BODY>\n          { }
<IFDEF_END>"}"          { BEGIN INITIAL; }
<IFDEF_END>.            { }
<IFDEF_END>\n           { }
"{$MinEnumSize"         { pascal_minenumbuf[0] = '\0'; pascal_minenumpos = 0; BEGIN MINENUM_HDR; }
<MINENUM_HDR>"}"        { if (atoi(pascal_minenumbuf) > 0) pascal_event_num("minenum", atoi(pascal_minenumbuf)); BEGIN INITIAL; }
<MINENUM_HDR>[ \t]      { }
<MINENUM_HDR>.          { if (pascal_minenumpos < (int)sizeof(pascal_minenumbuf) - 1) { pascal_minenumbuf[pascal_minenumpos++] = pascal_yytext[0]; pascal_minenumbuf[pascal_minenumpos] = '\0'; } }
<MINENUM_HDR>\n         { }
"{$PackSet"             { pascal_packsetbuf[0] = '\0'; pascal_packsetpos = 0; BEGIN PACKSET_HDR; }
<PACKSET_HDR>"}"        { if (atoi(pascal_packsetbuf) > 0) pascal_event_num("packset", atoi(pascal_packsetbuf)); BEGIN INITIAL; }
<PACKSET_HDR>[ \t]      { }
<PACKSET_HDR>.          { if (pascal_packsetpos < (int)sizeof(pascal_packsetbuf) - 1) { pascal_packsetbuf[pascal_packsetpos++] = pascal_yytext[0]; pascal_packsetbuf[pascal_packsetpos] = '\0'; } }
<PACKSET_HDR>\n         { }
"{$push}"               { pascal_event("push"); }
"{$pop}"                { pascal_event("pop"); }
"{$"[Rr]"+}"             { pascal_event("range 1"); }
"{$"[Rr]"-}"             { pascal_event("range 0"); }
"{$mode"                { pascal_modebuf[0] = '\0'; pascal_modepos = 0; BEGIN MODE_HDR; }
<MODE_HDR>"}"           { if (!pascal_seen_decl_start && pascal_mode_is_known(pascal_modebuf) && strcasecmp(pascal_modebuf, "iso") != 0) pascal_event("seen_mode"); if (strcasecmp(pascal_modebuf, "iso") == 0) pascal_event("mode_iso"); BEGIN INITIAL; }
<MODE_HDR>[ \t]         { }
<MODE_HDR>.             { if (pascal_modepos < (int)sizeof(pascal_modebuf) - 1) { pascal_modebuf[pascal_modepos++] = pascal_yytext[0]; pascal_modebuf[pascal_modepos] = '\0'; } }
<MODE_HDR>\n            { }
"{$zerobasedstrings"[ \t]+"on"[ \t]*"}"   { pascal_event("zerobased 1"); }
"{$zerobasedstrings"[ \t]+"off"[ \t]*"}"  { pascal_event("zerobased 0"); }
"{$zerobasedstrings+}"  { pascal_event("zerobased 1"); }
"{$Q+}"  { pascal_event("overflow 1"); }
"{$Q-}"  { pascal_event("overflow 0"); }
"{$overflowchecks"[ \t]+"on"[ \t]*"}"  { pascal_event("overflow 1"); }
"{$overflowchecks"[ \t]+"off"[ \t]*"}"  { pascal_event("overflow 0"); }
"{$zerobasedstrings-}"  { pascal_event("zerobased 0"); }
"{$PackRecords"         { pascal_packrecbuf[0] = '\0'; pascal_packrecpos = 0; BEGIN PACKREC_HDR; }
<PACKREC_HDR>"}"        { pascal_event_num("packrec", atoi(pascal_packrecbuf)); BEGIN INITIAL; }
<PACKREC_HDR>[ \t]      { }
<PACKREC_HDR>.          { if (pascal_packrecpos < (int)sizeof(pascal_packrecbuf) - 1) { pascal_packrecbuf[pascal_packrecpos++] = pascal_yytext[0]; pascal_packrecbuf[pascal_packrecpos] = '\0'; } }
<PACKREC_HDR>\n         { }
"{$align"               { pascal_alignbuf[0] = '\0'; pascal_alignpos = 0; BEGIN ALIGN_HDR; }
<ALIGN_HDR>"}"          { pascal_event_num("align", strcasecmp(pascal_alignbuf, "mac68k") == 0); BEGIN INITIAL; }
<ALIGN_HDR>[ \t]        { }
<ALIGN_HDR>.            { if (pascal_alignpos < (int)sizeof(pascal_alignbuf) - 1) { pascal_alignbuf[pascal_alignpos++] = pascal_yytext[0]; pascal_alignbuf[pascal_alignpos] = '\0'; } }
<ALIGN_HDR>\n           { }
"{$codepage"            { pascal_codepagebuf[0] = '\0'; pascal_codepagepos = 0; BEGIN CODEPAGE_HDR; }
<CODEPAGE_HDR>"}"       { pascal_event_num("codepage", pas_codepage_parse(pascal_codepagebuf)); BEGIN INITIAL; }
<CODEPAGE_HDR>[ \t]     { }
<CODEPAGE_HDR>.         { if (pascal_codepagepos < (int)sizeof(pascal_codepagebuf) - 1) { pascal_codepagebuf[pascal_codepagepos++] = pascal_yytext[0]; pascal_codepagebuf[pascal_codepagepos] = '\0'; } }
<CODEPAGE_HDR>\n        { }
"{"                     { BEGIN BRACE; }
<BRACE>"}"              { BEGIN INITIAL; }
<BRACE>.                { }
<BRACE>\n               { }
"program"               { return PROGRAMSY; }
"begin"                 { pascal_seen_decl_start = 1; return BEGINSY; }
"end"                   { return ENDSY; }
"var"                   { pascal_seen_decl_start = 1; return VARSY; }
"const"                 { pascal_seen_decl_start = 1; return CONSTSY; }
"type"                  { pascal_seen_decl_start = 1; return TYPESY; }
"procedure"             { pascal_seen_decl_start = 1; return PROCEDURESY; }
"function"              { pascal_seen_decl_start = 1; return FUNCTIONSY; }
"if"                    { return IFSY; }
"then"                  { return THENSY; }
"else"                  { return ELSESY; }
"while"                 { return WHILESY; }
"do"                    { return DOSY; }
"repeat"                { return REPEATSY; }
"until"                 { return UNTILSY; }
"for"                   { return FORSY; }
"to"                    { return TOSY; }
"downto"                { return DOWNTOSY; }
"array"                 { return ARRAYSY; }
"of"                    { return OFSY; }
"record"                { return RECORDSY; }
"packed"                { return PACKEDSY; }
"set"                   { return SETSY; }
"file"                  { return FILESY; }
"label"                 { return LABELSY; }
"goto"                  { return GOTOSY; }
"forward"               { return FORWARDSY; }
"with"                  { return WITHSY; }
"case"                  { return CASESY; }
"not"                   { return NOTSY; }
"div"                   { return IDIV; }
"mod"                   { return IMOD; }
"and"                   { return ANDOP; }
"or"                    { return OROP; }
"in"                    { return INOP; }
{DIGIT}+"."{DIGIT}+([eE][+-]?{DIGIT}+)?   { double _d = pascal_real_lit(pascal_yytext); pascal_yylval.node = pascal_leaf(TT_FLIT); pascal_yylval.node->v.dval = _d; return REALCONST; }
{DIGIT}+[eE][+-]?{DIGIT}+                 { double _d = pascal_real_lit(pascal_yytext); pascal_yylval.node = pascal_leaf(TT_FLIT); pascal_yylval.node->v.dval = _d; return REALCONST; }
{DIGIT}+                { long long _i = pascal_int_lit(pascal_yytext); pascal_yylval.node = pascal_leaf(TT_ILIT); pascal_yylval.node->v.ival = _i; return INTCONST; }
{DIGIT}+{ALPHA}{ALNUM}* { int nd = 0; while (isdigit((unsigned char)pascal_yytext[nd])) nd++;
                          fprintf(stderr, "pascal: ISO 7185 6.1.8 violation line %d: the unsigned-number '%.*s' is directly followed by '%s' -- a separator shall occur between them\n",
                                  pascal_yylineno, nd, pascal_yytext, pascal_yytext + nd);
                          pascal_event("iso_error"); yyless(nd); pascal_yylval.node = pascal_leaf(TT_ILIT); pascal_yylval.node->v.ival = (long long)(unsigned long long)strtoull(pascal_yytext, NULL, 10); return INTCONST; }
"$"[0-9A-Fa-f]+          { pascal_yylval.node = pascal_leaf(TT_ILIT); pascal_yylval.node->v.ival = (long long)(unsigned long long)strtoull(pascal_yytext + 1, NULL, 16); return INTCONST; }
"#"[0-9]+               { pascal_yylval.node = pascal_leaf(TT_CHRLIT); pascal_yylval.node->v.ival = strtoll(pascal_yytext + 1, NULL, 10) & 0xFF; return CHARCODE; }
"#$"[0-9A-Fa-f]+         { pascal_yylval.node = pascal_leaf(TT_CHRLIT); pascal_yylval.node->v.ival = strtoll(pascal_yytext + 2, NULL, 16) & 0xFF; return CHARCODE; }
'([^']|'')*'            { int n = (int)pascal_yyleng; pascal_strpos = 0;
                          if (n == 2) { pascal_event("iso_error");
                              fprintf(stderr, "pascal: ISO 7185 6.1.7 violation line %d: a character-string shall contain at least one string-element -- '' is not one\n", pascal_yylineno); }
                          for (int i = 1; i < n - 1; i++) {
                              if (pascal_yytext[i] == '\'' && i + 1 < n - 1 && pascal_yytext[i + 1] == '\'') { if (pascal_strpos < (int)sizeof pascal_strbuf - 1) pascal_strbuf[pascal_strpos++] = '\''; i++; }
                              else { if (pascal_strpos < (int)sizeof pascal_strbuf - 1) pascal_strbuf[pascal_strpos++] = pascal_yytext[i]; }
                          }
                          pascal_strbuf[pascal_strpos] = '\0';
                          pascal_yylval.node = pascal_leaf(TT_QLIT); pascal_yylval.node->v.sval = pascal_raw_dup(pascal_strbuf, pascal_strpos);
                          return STRINGCONST; }
{ALPHA}{ALNUM}*         { pascal_yylval.node = pascal_leaf(TT_VAR); pascal_yylval.node->v.sval = pascal_lower_dup(pascal_yytext, (int)pascal_yyleng); return IDENT; }
":="                    { return BECOMES; }
".."                    { return DOTDOT; }
"<="                    { return LEOP; }
">="                    { return GEOP; }
"<>"                    { return NEOP; }
"<"                     { return LTOP; }
">"                     { return GTOP; }
"="                     { return EQOP; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MUL; }
"/"                     { return RDIV; }
"^"                     { return ARROW; }
"@"                     { return ATSIGN; }
"("                     { return LPARENT; }
")"                     { return RPARENT; }
"["                     { return LBRACK; }
"]"                     { return RBRACK; }
";"                     { return SEMICOLON; }
"."                     { return PERIOD; }
","                     { return COMMA; }
":"                     { return COLON; }
.                       { pascal_lex_error("invalid character"); return PASCAL_YYUNDEF; }
%%
void pascal_lex_reset(void) { pascal_nev = 0; pascal_seen_decl_start = 0; BEGIN INITIAL; }
