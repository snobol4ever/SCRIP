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
static char pascal_strbuf[65536];
static int  pascal_strpos;
static char pascal_ifdefbuf[256];
static int  pascal_ifdefpos;
int g_pas_seen_mode_directive = 0;
int pascal_seen_decl_start = 0;
static char pascal_modebuf[64];
static int  pascal_modepos;
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
%x COMMENT BRACE IFDEF_HDR IFDEF_BODY IFDEF_END IFDEF_ELSE1 IFDEF_ELSE2 MODE_HDR
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
<IFDEF_BODY>.           { }
<IFDEF_BODY>\n          { }
<IFDEF_END>"}"          { BEGIN INITIAL; }
<IFDEF_END>.            { }
<IFDEF_END>\n           { }
"{$mode"                { pascal_modebuf[0] = '\0'; pascal_modepos = 0; BEGIN MODE_HDR; }
<MODE_HDR>"}"           { if (!pascal_seen_decl_start && pascal_mode_is_known(pascal_modebuf) && strcasecmp(pascal_modebuf, "iso") != 0) g_pas_seen_mode_directive = 1; BEGIN INITIAL; }
<MODE_HDR>[ \t]         { }
<MODE_HDR>.             { if (pascal_modepos < (int)sizeof(pascal_modebuf) - 1) { pascal_modebuf[pascal_modepos++] = pascal_yytext[0]; pascal_modebuf[pascal_modepos] = '\0'; } }
<MODE_HDR>\n            { }
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
{DIGIT}+"."{DIGIT}+([eE][+-]?{DIGIT}+)?   { pascal_yylval.dval = strtod(pascal_yytext, NULL); return REALCONST; }
{DIGIT}+[eE][+-]?{DIGIT}+                 { pascal_yylval.dval = strtod(pascal_yytext, NULL); return REALCONST; }
{DIGIT}+                { pascal_yylval.ival = strtoll(pascal_yytext, NULL, 10); return INTCONST; }
"$"[0-9A-Fa-f]+          { pascal_yylval.ival = (long long)(unsigned long long)strtoull(pascal_yytext + 1, NULL, 16); return INTCONST; }
"#"[0-9]+               { long long _cc = strtoll(pascal_yytext + 1, NULL, 10); pascal_strbuf[0] = (char)_cc; pascal_strbuf[1] = '\0'; pascal_yylval.str = pascal_raw_dup(pascal_strbuf, 1); return STRINGCONST; }
"#$"[0-9A-Fa-f]+         { long long _cc = strtoll(pascal_yytext + 2, NULL, 16); pascal_strbuf[0] = (char)_cc; pascal_strbuf[1] = '\0'; pascal_yylval.str = pascal_raw_dup(pascal_strbuf, 1); return STRINGCONST; }
'([^']|'')*'            { int n = (int)pascal_yyleng; pascal_strpos = 0;
                          for (int i = 1; i < n - 1; i++) {
                              if (pascal_yytext[i] == '\'' && i + 1 < n - 1 && pascal_yytext[i + 1] == '\'') { if (pascal_strpos < (int)sizeof pascal_strbuf - 1) pascal_strbuf[pascal_strpos++] = '\''; i++; }
                              else { if (pascal_strpos < (int)sizeof pascal_strbuf - 1) pascal_strbuf[pascal_strpos++] = pascal_yytext[i]; }
                          }
                          pascal_strbuf[pascal_strpos] = '\0';
                          pascal_yylval.str = pascal_raw_dup(pascal_strbuf, pascal_strpos);
                          return STRINGCONST; }
{ALPHA}{ALNUM}*         { pascal_yylval.str = pascal_lower_dup(pascal_yytext, (int)pascal_yyleng); return IDENT; }
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
