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
int pas_const_get(const char *name, long long *out);
int pas_rconst_get(const char *name, double *out);
const char *pas_sconst_get(const char *name);
extern int g_pas_iso_errors;
static long long pascal_int_lit(const char *t) {
    errno = 0; unsigned long long v = strtoull(t, NULL, 10);
    if (errno == ERANGE) { fprintf(stderr, "pascal: ISO 7185 6.1.5 violation line %d: the integer literal '%.24s%s' is beyond the largest integer SCRIP represents\n",
                                   pascal_yylineno, t, strlen(t) > 24 ? "..." : ""); g_pas_iso_errors++; }
    return (long long)v;
}
static double pascal_real_lit(const char *t) {
    errno = 0; double d = strtod(t, NULL);
    if (errno == ERANGE && isinf(d)) { fprintf(stderr, "pascal: ISO 7185 6.1.5 violation line %d: the real literal '%.24s%s' is beyond the largest real SCRIP represents\n",
                                               pascal_yylineno, t, strlen(t) > 24 ? "..." : ""); g_pas_iso_errors++; }
    return d;
}
static int pascal_if_declared(const char *name) {
    long long iv; double rv;
    return pas_const_get(name, &iv) || pas_rconst_get(name, &rv) || pas_sconst_get(name) != NULL;
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
            if (pas_const_get(ident, &lv)) {
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
int g_pas_seen_mode_directive = 0;
int pascal_seen_decl_start = 0;
static char pascal_modebuf[64];
static int  pascal_modepos;
int g_pas_min_enum_size = 4;
int g_pas_pack_set_size = 0;
int g_pas_range_check_on = 0;
int g_pas_align_mac68k = 0;
static char pascal_alignbuf[32];
static int  pascal_alignpos;
int g_pas_codepage = 0;
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
static int g_pas_min_enum_stack[64];
static int g_pas_pack_set_stack[64];
static int g_pas_min_enum_sp;
static char pascal_minenumbuf[16];
static int  pascal_minenumpos;
static char pascal_packsetbuf[16];
static int  pascal_packsetpos;
static void pas_min_enum_size_set(int n) { if (n > 0) g_pas_min_enum_size = n; }
static void pas_pack_set_size_set(int n) { if (n > 0) g_pas_pack_set_size = n; }
static void pas_min_enum_size_push(void) { if (g_pas_min_enum_sp < (int)(sizeof(g_pas_min_enum_stack) / sizeof(g_pas_min_enum_stack[0]))) { g_pas_min_enum_stack[g_pas_min_enum_sp] = g_pas_min_enum_size; g_pas_pack_set_stack[g_pas_min_enum_sp] = g_pas_pack_set_size; g_pas_min_enum_sp++; } }
static void pas_min_enum_size_pop(void) { if (g_pas_min_enum_sp > 0) { g_pas_min_enum_sp--; g_pas_min_enum_size = g_pas_min_enum_stack[g_pas_min_enum_sp]; g_pas_pack_set_size = g_pas_pack_set_stack[g_pas_min_enum_sp]; } }
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
%x COMMENT BRACE IFDEF_HDR IFDEF_BODY IFDEF_END IFDEF_ELSE1 IFDEF_ELSE2 MODE_HDR IF_HDR MINENUM_HDR PACKSET_HDR ALIGN_HDR CODEPAGE_HDR
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
<MINENUM_HDR>"}"        { pas_min_enum_size_set(atoi(pascal_minenumbuf)); BEGIN INITIAL; }
<MINENUM_HDR>[ \t]      { }
<MINENUM_HDR>.          { if (pascal_minenumpos < (int)sizeof(pascal_minenumbuf) - 1) { pascal_minenumbuf[pascal_minenumpos++] = pascal_yytext[0]; pascal_minenumbuf[pascal_minenumpos] = '\0'; } }
<MINENUM_HDR>\n         { }
"{$PackSet"             { pascal_packsetbuf[0] = '\0'; pascal_packsetpos = 0; BEGIN PACKSET_HDR; }
<PACKSET_HDR>"}"        { pas_pack_set_size_set(atoi(pascal_packsetbuf)); BEGIN INITIAL; }
<PACKSET_HDR>[ \t]      { }
<PACKSET_HDR>.          { if (pascal_packsetpos < (int)sizeof(pascal_packsetbuf) - 1) { pascal_packsetbuf[pascal_packsetpos++] = pascal_yytext[0]; pascal_packsetbuf[pascal_packsetpos] = '\0'; } }
<PACKSET_HDR>\n         { }
"{$push}"               { pas_min_enum_size_push(); }
"{$pop}"                { pas_min_enum_size_pop(); }
"{$R+}"                 { g_pas_range_check_on = 1; }
"{$R-}"                 { g_pas_range_check_on = 0; }
"{$mode"                { pascal_modebuf[0] = '\0'; pascal_modepos = 0; BEGIN MODE_HDR; }
<MODE_HDR>"}"           { if (!pascal_seen_decl_start && pascal_mode_is_known(pascal_modebuf) && strcasecmp(pascal_modebuf, "iso") != 0) g_pas_seen_mode_directive = 1; BEGIN INITIAL; }
<MODE_HDR>[ \t]         { }
<MODE_HDR>.             { if (pascal_modepos < (int)sizeof(pascal_modebuf) - 1) { pascal_modebuf[pascal_modepos++] = pascal_yytext[0]; pascal_modebuf[pascal_modepos] = '\0'; } }
<MODE_HDR>\n            { }
"{$align"               { pascal_alignbuf[0] = '\0'; pascal_alignpos = 0; BEGIN ALIGN_HDR; }
<ALIGN_HDR>"}"          { g_pas_align_mac68k = (strcasecmp(pascal_alignbuf, "mac68k") == 0); BEGIN INITIAL; }
<ALIGN_HDR>[ \t]        { }
<ALIGN_HDR>.            { if (pascal_alignpos < (int)sizeof(pascal_alignbuf) - 1) { pascal_alignbuf[pascal_alignpos++] = pascal_yytext[0]; pascal_alignbuf[pascal_alignpos] = '\0'; } }
<ALIGN_HDR>\n           { }
"{$codepage"            { pascal_codepagebuf[0] = '\0'; pascal_codepagepos = 0; BEGIN CODEPAGE_HDR; }
<CODEPAGE_HDR>"}"       { g_pas_codepage = pas_codepage_parse(pascal_codepagebuf); BEGIN INITIAL; }
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
{DIGIT}+"."{DIGIT}+([eE][+-]?{DIGIT}+)?   { pascal_yylval.dval = pascal_real_lit(pascal_yytext); return REALCONST; }
{DIGIT}+[eE][+-]?{DIGIT}+                 { pascal_yylval.dval = pascal_real_lit(pascal_yytext); return REALCONST; }
{DIGIT}+                { pascal_yylval.ival = pascal_int_lit(pascal_yytext); return INTCONST; }
{DIGIT}+{ALPHA}{ALNUM}* { int nd = 0; while (isdigit((unsigned char)pascal_yytext[nd])) nd++;
                          fprintf(stderr, "pascal: ISO 7185 6.1.8 violation line %d: the unsigned-number '%.*s' is directly followed by '%s' -- a separator shall occur between them\n",
                                  pascal_yylineno, nd, pascal_yytext, pascal_yytext + nd);
                          g_pas_iso_errors++; yyless(nd); pascal_yylval.ival = (long long)(unsigned long long)strtoull(pascal_yytext, NULL, 10); return INTCONST; }
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
