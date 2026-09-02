%{
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "pascal.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
static char pascal_strbuf[65536];
static int  pascal_strpos;
static char *pascal_raw_dup(const char *s, int n) {
    char *b = (char *)malloc((size_t)(n + 1)); memcpy(b, s, (size_t)n); b[n] = '\0'; return b;
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
%x COMMENT BRACE
DIGIT    [0-9]
ALPHA    [A-Za-z_]
ALNUM    [A-Za-z0-9_]
%%
[ \t\r\n]+              { }
"(*"                    { BEGIN COMMENT; }
<COMMENT>"*)"           { BEGIN INITIAL; }
<COMMENT>.              { }
<COMMENT>\n             { }
"{"                     { BEGIN BRACE; }
<BRACE>"}"              { BEGIN INITIAL; }
<BRACE>.                { }
<BRACE>\n               { }
"program"               { return PROGRAMSY; }
"begin"                 { return BEGINSY; }
"end"                   { return ENDSY; }
"var"                   { return VARSY; }
"const"                 { return CONSTSY; }
"type"                  { return TYPESY; }
"procedure"             { return PROCEDURESY; }
"function"              { return FUNCTIONSY; }
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
"("                     { return LPARENT; }
")"                     { return RPARENT; }
"["                     { return LBRACK; }
"]"                     { return RBRACK; }
";"                     { return SEMICOLON; }
"."                     { return PERIOD; }
","                     { return COMMA; }
":"                     { return COLON; }
.                       { pascal_lex_error("invalid character"); }
%%
