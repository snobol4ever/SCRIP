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
int pascal_cenv_int(const char *name, long long *out);
int pascal_unit_mode(void);
void pascal_unit_mode_set(int m);
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
static const char *const PAS_FPC_MACROS[] = { "CONSOLE", "CPU64", "CPUAMD64", "CPUATHLON64", "CPUINT64", "CPUX64", "CPUX86_64", "CPUX86_HAS_CMOV", "CPUX86_HAS_SSE2", "CPUX86_HAS_SSEUNIT", "ENDIAN_LITTLE", "FPC", "FPC_ABI_DEFAULT", "FPC_DYNARRAYCOPY_FIXED", "FPC_HAS_CEXTENDED", "FPC_HAS_CONSTREF", "FPC_HAS_CPSTRING", "FPC_HAS_FEATURE_ANSISTRINGS", "FPC_HAS_FEATURE_CLASSES", "FPC_HAS_FEATURE_COMMANDARGS", "FPC_HAS_FEATURE_CONSOLEIO", "FPC_HAS_FEATURE_DYNARRAYS", "FPC_HAS_FEATURE_DYNLIBS", "FPC_HAS_FEATURE_EXCEPTIONS", "FPC_HAS_FEATURE_EXITCODE", "FPC_HAS_FEATURE_FILEIO", "FPC_HAS_FEATURE_HEAP", "FPC_HAS_FEATURE_INITFINAL", "FPC_HAS_FEATURE_OBJECTIVEC1", "FPC_HAS_FEATURE_OBJECTS", "FPC_HAS_FEATURE_PROCESSES", "FPC_HAS_FEATURE_RANDOM", "FPC_HAS_FEATURE_RESOURCES", "FPC_HAS_FEATURE_RTTI", "FPC_HAS_FEATURE_SOFTFPU", "FPC_HAS_FEATURE_STACKCHECK", "FPC_HAS_FEATURE_SUPPORT", "FPC_HAS_FEATURE_TEXTIO", "FPC_HAS_FEATURE_THREADING", "FPC_HAS_FEATURE_UNICODESTRINGS", "FPC_HAS_FEATURE_VARIANTS", "FPC_HAS_FEATURE_WIDESTRINGS", "FPC_HAS_INDIRECT_ENTRY_INFORMATION", "FPC_HAS_INTERNAL_ABS_INT64", "FPC_HAS_INTERNAL_ABS_LONG", "FPC_HAS_INTERNAL_BSF", "FPC_HAS_INTERNAL_BSR", "FPC_HAS_INTERNAL_ROX", "FPC_HAS_INTERNAL_SAR", "FPC_HAS_MEMBAR", "FPC_HAS_OPERATOR_ENUMERATOR", "FPC_HAS_RESSTRINITS", "FPC_HAS_RIP_RELATIVE", "FPC_HAS_TYPE_DOUBLE", "FPC_HAS_TYPE_EXTENDED", "FPC_HAS_TYPE_SINGLE", "FPC_HAS_UNICODESTRING", "FPC_HAS_WINLIKERESOURCES", "FPC_LINK_STATIC", "FPC_LITTLE_ENDIAN", "FPC_RTTI_PACKSET1", "FPC_SETBASE_USED", "FPC_STATICRIPFIXED", "FPC_VARIANTCOPY_FIXED", "FPC_WIDESTRING_EQUAL_UNICODESTRING", "FPUSSE64", "HASUNIX", "INTERNAL_BACKTRACE", "LINUX", "REGCALL", "STR_CONCAT_PROCS", "UNIX", "VER3", "VER3_2", "VER3_2_2", 0 };
static int pascal_macro_value(const char *n, long long *v) {
    static const char *const N[] = { "FPC_FULLVERSION", "FPC_VERSION", "FPC_RELEASE", "FPC_PATCH" };
    static const long long V[] = { 30202, 3, 2, 2 };
    for (int i = 0; i < 4; i++) if (!strcasecmp(n, N[i])) { *v = V[i]; return 1; }
    return 0;
}
static int pascal_defined(const char *n) {
    long long v;
    int u = pascal_def_state(n);
    if (u >= 0) return u;
    for (int i = 0; PAS_FPC_MACROS[i]; i++) if (!strcasecmp(PAS_FPC_MACROS[i], n)) return 1;
    if (pascal_macro_value(n, &v)) return 1;
    const char *mm = pascal_mode_macro();
    if (mm && !strcasecmp(mm, n)) return 1;
    int d = pascal_dialect();
    return (d == PAS_DIALECT_ISO_DEFAULT || d == PAS_DIALECT_ISO) && !strcasecmp(n, "FPC_ISO");
}
typedef struct { const char *p; int bad; } PasCondX;
static void pcx_ws(PasCondX *x) { while (*x->p == ' ' || *x->p == '\t' || *x->p == '\n' || *x->p == '\r') x->p++; }
static int pcx_word(PasCondX *x, const char *w) {
    size_t n = strlen(w);
    pcx_ws(x);
    if (strncasecmp(x->p, w, n) || isalnum((unsigned char)x->p[n]) || x->p[n] == '_') return 0;
    x->p += n;
    return 1;
}
static int pcx_ident(PasCondX *x, char *buf, size_t cap) {
    size_t k = 0;
    pcx_ws(x);
    if (!isalpha((unsigned char)*x->p) && *x->p != '_') return 0;
    while (isalnum((unsigned char)*x->p) || *x->p == '_') { if (k + 1 < cap) buf[k++] = *x->p; x->p++; }
    buf[k] = '\0';
    return 1;
}
static double pcx_expr(PasCondX *x);
static double pcx_prim(PasCondX *x) {
    size_t cap = strlen(x->p) + 1;
    char id[cap];
    long long lv;
    double rv;
    pcx_ws(x);
    if (*x->p == '(') {
        x->p++;
        double v = pcx_expr(x);
        pcx_ws(x);
        if (*x->p == ')') x->p++; else x->bad = 1;
        return v;
    }
    int und = pcx_word(x, "undefined");
    if (und || pcx_word(x, "defined")) {
        pcx_ws(x);
        int par = *x->p == '(';
        if (par) x->p++;
        if (!pcx_ident(x, id, cap)) { x->bad = 1; return 0; }
        pcx_ws(x);
        if (par) { if (*x->p == ')') x->p++; else x->bad = 1; }
        return und ? !pascal_defined(id) : pascal_defined(id);
    }
    if (pcx_word(x, "declared")) {
        pcx_ws(x);
        if (*x->p != '(') { x->bad = 1; return 0; }
        x->p++;
        if (!pcx_ident(x, id, cap)) { x->bad = 1; return 0; }
        pcx_ws(x);
        if (*x->p == ')') x->p++; else x->bad = 1;
        return pascal_if_declared(id);
    }
    if (pcx_word(x, "option")) {
        pcx_ws(x);
        if (*x->p != '(') { x->bad = 1; return 0; }
        x->p++;
        pcx_ws(x);
        int c = tolower((unsigned char)*x->p) - 'a';
        if (c < 0 || c > 25) { x->bad = 1; return 0; }
        x->p++;
        pcx_ws(x);
        if (*x->p == ')') x->p++; else x->bad = 1;
        if (!(*pascal_sw_set_p() & (1u << c))) { x->bad = 1; return 0; }
        return (*pascal_sw_on_p() >> c) & 1u;
    }
    if (pcx_word(x, "true")) return 1;
    if (pcx_word(x, "false")) return 0;
    if (isdigit((unsigned char)*x->p)) { char *e; double v = strtod(x->p, &e); x->p = e; return v; }
    if (!pcx_ident(x, id, cap)) { x->bad = 1; return 0; }
    for (char *q = id; *q; q++) *q = (char)tolower((unsigned char)*q);
    if (pascal_def_value(id, &rv)) return rv;
    if (pascal_macro_value(id, &lv) || pascal_cenv_int(id, &lv)) return (double)lv;
    if (pascal_cenv_real(id, &rv)) return rv;
    x->bad = 1;
    return 0;
}
static double pcx_rel(PasCondX *x) {
    static const char *const OP[] = { "<>", "<=", ">=", "=", "<", ">" };
    double a = pcx_prim(x);
    pcx_ws(x);
    for (int i = 0; i < 6; i++) {
        size_t n = strlen(OP[i]);
        if (strncmp(x->p, OP[i], n)) continue;
        x->p += n;
        double b = pcx_prim(x);
        return i == 0 ? a != b : i == 1 ? a <= b : i == 2 ? a >= b : i == 3 ? a == b : i == 4 ? a < b : a > b;
    }
    return a;
}
static double pcx_unary(PasCondX *x) { return pcx_word(x, "not") ? !pcx_unary(x) : pcx_rel(x); }
static double pcx_and(PasCondX *x) {
    double v = pcx_unary(x);
    while (pcx_word(x, "and")) { double w = pcx_unary(x); v = v != 0 && w != 0; }
    return v;
}
static double pcx_expr(PasCondX *x) {
    double v = pcx_and(x);
    while (pcx_word(x, "or")) { double w = pcx_and(x); v = v != 0 || w != 0; }
    return v;
}
extern int pascal_yylineno;
static int pascal_if_eval(const char *expr) {
    PasCondX x = { expr, 0 };
    double v = pcx_expr(&x);
    pcx_ws(&x);
    if (!x.bad && !*x.p) return v != 0;
    return -1;
}
static double pascal_value_eval(const char *expr, int *ok) {
    PasCondX x = { expr, 0 };
    double v = pcx_expr(&x);
    pcx_ws(&x);
    *ok = !x.bad && !*x.p;
    return v;
}
static const char *pascal_unknown_expr;
static void pascal_cond_code_in_unknown(void) {
    if (!*pascal_cond_unknown_p()) return;
    fprintf(stderr, "pascal: line %d: the conditional expression '%s' is beyond SCRIP's {$if} evaluator (defined, undefined, declared, option, not, and, or, and comparisons of numbers, constants and macro values), and a branch it guards holds program text\n", *pascal_cond_unknown_p(), pascal_unknown_expr ? pascal_unknown_expr : "?");
    *pascal_cond_unknown_p() = 0;
    pascal_event("iso_error");
}
static void pascal_cond(const char *text) {
    const char *p = text + 2;
    size_t cap = strlen(text) + 1;
    char kw[cap], arg[cap];
    size_t k = 0;
    while (isalpha((unsigned char)*p)) { kw[k++] = (char)tolower((unsigned char)*p); p++; }
    kw[k] = '\0';
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
    size_t al = strlen(p);
    while (al > 0 && (p[al - 1] == '}' || p[al - 1] == ' ' || p[al - 1] == '\t' || p[al - 1] == '\n' || p[al - 1] == '\r')) al--;
    memcpy(arg, p, al);
    arg[al] = '\0';
    size_t w = 0;
    while (arg[w] && (isalnum((unsigned char)arg[w]) || arg[w] == '_')) w++;
    char word[w + 1];
    memcpy(word, arg, w);
    word[w] = '\0';
    int *skip = pascal_cond_skip_p(), *depth = pascal_cond_depth_p(), *taken = pascal_cond_taken_p();
    if (!strcmp(kw, "ifdef") || !strcmp(kw, "ifndef") || !strcmp(kw, "if") || !strcmp(kw, "ifc")) {
        if (*skip) { (*skip)++; return; }
        int c = (!strcmp(kw, "if") || !strcmp(kw, "ifc")) ? pascal_if_eval(arg) : (pascal_defined(word) != (kw[2] == 'n'));
        if (c < 0) {
            *skip = 1;
            *taken = 1;
            *pascal_cond_unknown_p() = pascal_yylineno;
            pascal_unknown_expr = ct_strdup(arg);
        } else if (c) (*depth)++;
        else { *skip = 1; *taken = 0; }
    } else if (!strcmp(kw, "elseif") || !strcmp(kw, "else") || !strcmp(kw, "elsec")) {
        if (*skip > 1) return;
        if (*skip == 1) { if (!*taken && (kw[4] != 'i' || pascal_if_eval(arg) > 0)) { *skip = 0; (*depth)++; } return; }
        if (*depth > 0) { (*depth)--; *skip = 1; *taken = 1; }
    } else if (!strcmp(kw, "endif") || !strcmp(kw, "ifend") || !strcmp(kw, "endc")) {
        if (*skip) { (*skip)--; if (!*skip) *pascal_cond_unknown_p() = 0; }
        else if (*depth) (*depth)--;
    } else if (!strcmp(kw, "define") || !strcmp(kw, "setc")) {
        const char *as = strstr(arg, ":=");
        int ok = 0;
        double v = as ? pascal_value_eval(as + 2, &ok) : 0;
        if (*skip) return;
        if (as && ok && (*pascal_macro_on_p() || kw[0] == 's')) pascal_def_setval(word, v);
        else pascal_def_set(word, 1);
    } else if (!strcmp(kw, "undef")) {
        if (!*skip) pascal_def_set(word, 0);
    }
}
int pascal_seen_decl_start = 0;
static int pascal_context_keyword(const char *w) {
    static const char *K[] = { "unit", "uses", "interface", "implementation", "initialization", "finalization" };
    static const int T[] = { UNITSY, USESSY, INTERFACESY, IMPLEMENTATIONSY, INITIALIZATIONSY, FINALIZATIONSY };
    int k = -1, m = pascal_unit_mode(), d = pascal_dialect();
    for (int i = 0; i < 6; i++) if (!strcasecmp(w, K[i])) k = i;
    if (k < 0 || d == PAS_DIALECT_ISO) return 0;
    if (d == PAS_DIALECT_ISO_DEFAULT) {
        if (k == 0 && (m != 0 || pascal_seen_decl_start)) return 0;
        if (k == 1 && pascal_seen_decl_start) return 0;
        if (k >= 2 && m != 1) return 0;
        if (k == 2 && pascal_seen_decl_start) return 0;
        if (k <= 1) { pascal_dialect_set(PAS_DIALECT_FPC); pascal_event("seen_mode"); }
    }
    if (k == 0 && m == 0) pascal_unit_mode_set(1);
    if (k == 3) pascal_seen_decl_start = 0;
    return T[k];
}
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
static void pascal_mode_directive(const char *m) {
    static const char *M[] = { "objfpc", "delphi", "tp", "fpc", "macpas" };
    static const int D[] = { PAS_DIALECT_OBJFPC, PAS_DIALECT_DELPHI, PAS_DIALECT_FPC, PAS_DIALECT_FPC, PAS_DIALECT_FPC };
    if (!strcasecmp(m, "iso")) {
        pascal_event("mode_iso");
        if (!pascal_seen_decl_start) pascal_dialect_set(PAS_DIALECT_ISO);
        return;
    }
    static const char *const MAC[] = { "FPC_OBJFPC", "FPC_DELPHI", "FPC_TP", NULL, "FPC_MACPAS" };
    for (int i = 0; i < 5; i++) if (!strcasecmp(m, M[i]) && !pascal_seen_decl_start) {
        pascal_dialect_set(D[i]);
        pascal_mode_macro_set(MAC[i]);
        pascal_event("seen_mode");
    }
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
%x COMMENT BRACE SKIPC MODE_HDR MINENUM_HDR PACKSET_HDR ALIGN_HDR PACKREC_HDR CODEPAGE_HDR
DIGIT    [0-9]
ALPHA    [A-Za-z_]
ALNUM    [A-Za-z0-9_]
%%
[ \t\r\n]+              { }
"(*"                    { BEGIN COMMENT; }
<COMMENT>"*)"           { BEGIN INITIAL; }
<COMMENT>.              { }
<COMMENT>\n             { }
"//"[^\n]*              { }
"{$"(ifdef|ifndef|if|elseif|else|endif|ifend|define|undef|ifc|elsec|endc|setc)([ \t\r\n][^}]*)?"}" { pascal_cond(pascal_yytext); if (*pascal_cond_skip_p()) BEGIN SKIPC; }
<SKIPC>"{$"(ifdef|ifndef|if|elseif|else|endif|ifend|define|undef|ifc|elsec|endc|setc)([ \t\r\n][^}]*)?"}" { pascal_cond(pascal_yytext); if (!*pascal_cond_skip_p()) BEGIN INITIAL; }
<SKIPC>"{"[^}]*"}"        { }
<SKIPC>"(*"([^*]|"*"+[^*)])*"*"+")" { }
<SKIPC>"'"[^'\n]*"'"     { pascal_cond_code_in_unknown(); }
<SKIPC>"//"[^\n]*         { }
<SKIPC>[ \t\r\n]           { }
<SKIPC>.                 { pascal_cond_code_in_unknown(); }
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
"{$macro"[ \t]+"on"[ \t]*"}"  { *pascal_macro_on_p() = 1; }
"{$macro"[ \t]+"off"[ \t]*"}" { *pascal_macro_on_p() = 0; }
"{$push}"               { pascal_event("push"); pascal_sw_push(); }
"{$pop}"                { pascal_event("pop"); pascal_sw_pop(); }
"{$"[Rr]"+}"             { pascal_event("range 1"); *pascal_sw_set_p() |= 1u << 17; *pascal_sw_on_p() |= 1u << 17; }
"{$"[Rr]"-}"             { pascal_event("range 0"); *pascal_sw_set_p() |= 1u << 17; *pascal_sw_on_p() &= ~(1u << 17); }
"{$"[A-PS-Za-ps-z][+-]"}" { int c = tolower((unsigned char)pascal_yytext[2]) - 'a'; *pascal_sw_set_p() |= 1u << c; if (pascal_yytext[3] == '+') *pascal_sw_on_p() |= 1u << c; else *pascal_sw_on_p() &= ~(1u << c); }
"{$mode"                { pascal_modebuf[0] = '\0'; pascal_modepos = 0; BEGIN MODE_HDR; }
<MODE_HDR>"}"           { pascal_mode_directive(pascal_modebuf); BEGIN INITIAL; }
<MODE_HDR>[ \t]         { }
<MODE_HDR>.             { if (pascal_modepos < (int)sizeof(pascal_modebuf) - 1) { pascal_modebuf[pascal_modepos++] = pascal_yytext[0]; pascal_modebuf[pascal_modepos] = '\0'; } }
<MODE_HDR>\n            { }
"{$zerobasedstrings"[ \t]+"on"[ \t]*"}"   { pascal_event("zerobased 1"); }
"{$zerobasedstrings"[ \t]+"off"[ \t]*"}"  { pascal_event("zerobased 0"); }
"{$zerobasedstrings+}"  { pascal_event("zerobased 1"); }
"{$Q+}"  { pascal_event("overflow 1"); *pascal_sw_set_p() |= 1u << 16; *pascal_sw_on_p() |= 1u << 16; }
"{$Q-}"  { pascal_event("overflow 0"); *pascal_sw_set_p() |= 1u << 16; *pascal_sw_on_p() &= ~(1u << 16); }
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
"program"               { pascal_unit_mode_set(-1); return PROGRAMSY; }
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
{ALPHA}{ALNUM}*         { int kw = pascal_context_keyword(pascal_yytext);
                          if (kw) return kw;
                          pascal_yylval.node = pascal_leaf(TT_VAR); pascal_yylval.node->v.sval = pascal_lower_dup(pascal_yytext, (int)pascal_yyleng); return IDENT; }
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
void pascal_lex_reset(void) { pascal_nev = 0; pascal_seen_decl_start = 0; pascal_yylineno = 1; BEGIN INITIAL; }
