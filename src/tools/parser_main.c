#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "ast.h"
#if defined(PARSER_LANG_SNOBOL4)
#include "parsers/snobol4/scrip_cc.h"
#define PARSER_NAME "snobol4"
#elif defined(PARSER_LANG_SNOCONE)
#include "parsers/snocone/snocone_driver.h"
#define PARSER_NAME "snocone"
#define PARSER_PARSE snocone_compile_parse
#define PARSER_FINISH snocone_compile_finish
#elif defined(PARSER_LANG_ICON)
#include "parsers/icon/icon_driver.h"
#define PARSER_NAME "icon"
#define PARSER_PARSE icon_compile_parse
#define PARSER_FINISH icon_compile_finish
#elif defined(PARSER_LANG_PROLOG)
#include "parsers/prolog/prolog_driver.h"
#define PARSER_NAME "prolog"
#define PARSER_PARSE prolog_compile_parse
#define PARSER_FINISH prolog_compile_finish
#elif defined(PARSER_LANG_REBUS)
#include "parsers/rebus/rebus_lower.h"
#define PARSER_NAME "rebus"
#define PARSER_PARSE rebus_compile_parse
#define PARSER_FINISH rebus_compile_finish
#elif defined(PARSER_LANG_RAKU)
#include "parsers/raku/raku_driver.h"
#define PARSER_NAME "raku"
static void * raku_compile_whole(const char * src, const char * name) { tree_t * ast = NULL; raku_compile(src, name, &ast); return ast; }
static void raku_compile_done(void * parsed, const char * name, tree_t ** out_ast) { (void)name; *out_ast = (tree_t *)parsed; }
#define PARSER_PARSE raku_compile_whole
#define PARSER_FINISH raku_compile_done
#elif defined(PARSER_LANG_PASCAL)
#include "parsers/pascal/pascal_driver.h"
#define PARSER_NAME "pascal"
#define PARSER_PARSE pascal_compile_parse
#define PARSER_FINISH pascal_compile_finish
#else
#error "parser_main.c is compiled once per frontend: -DPARSER_LANG_<SNOBOL4|SNOCONE|ICON|PROLOG|REBUS|RAKU|PASCAL>"
#endif
extern void ir_dump_tree(const tree_t * e, FILE * f);
extern void stmt_src_set_file(const char * path);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * slurp(FILE * f, size_t * len) {
    size_t cap = 1 << 16, n = 0; char * b = ct_alloc(cap + 2);
    if (!b) return NULL;
    for (;;) { size_t r = fread(b + n, 1, cap - n, f); n += r; if (n < cap) break; cap *= 2; char * nb = ct_grow(b, cap + 2); if (!nb) { ct_drop(b); return NULL; } b = nb; }
    b[n] = 0; b[n + 1] = 0; *len = n; return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t mono_ns(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return (int64_t)ts.tv_sec * 1000000000LL + (int64_t)ts.tv_nsec; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t g_lex_ns, g_lex_calls;
static int lex_clock_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("PARSER_LEX_CLOCK"); v = (e && *e == '1') ? 1 : 0; } return v; }
static void * lex_real(const char * nm) { void * f = dlsym(RTLD_NEXT, nm); if (!f) { fprintf(stderr, "parser_%s: no %s behind the lexer clock\n", PARSER_NAME, nm); exit(2); } return f; }
#define LEX_CLOCKED(call) do { if (!lex_clock_on()) return call; int64_t a_ = mono_ns(); __typeof__(call) r_ = call; g_lex_ns += mono_ns() - a_; g_lex_calls++; return r_; } while (0)
#if defined(PARSER_LANG_SNOBOL4)
int snobol4_lex(void * lv) { static int (*real)(void *); if (!real) real = (int (*)(void *)) lex_real("snobol4_lex"); LEX_CLOCKED(real(lv)); }
#elif defined(PARSER_LANG_SNOCONE)
int sc_lex(void * lv, void * st) { static int (*real)(void *, void *); if (!real) real = (int (*)(void *, void *)) lex_real("sc_lex"); LEX_CLOCKED(real(lv, st)); }
#elif defined(PARSER_LANG_ICON)
#include "parsers/icon/icon_lex.h"
IcnToken icn_lex_next(IcnLexer * lx) { static IcnToken (*real)(IcnLexer *); if (!real) real = (IcnToken (*)(IcnLexer *)) lex_real("icn_lex_next"); LEX_CLOCKED(real(lx)); }
#elif defined(PARSER_LANG_PROLOG)
#include "parsers/prolog/prolog_lex.h"
Token lexer_next(Lexer * lx) { static Token (*real)(Lexer *); if (!real) real = (Token (*)(Lexer *)) lex_real("lexer_next"); LEX_CLOCKED(real(lx)); }
Token lexer_peek(Lexer * lx) { static Token (*real)(Lexer *); if (!real) real = (Token (*)(Lexer *)) lex_real("lexer_peek"); LEX_CLOCKED(real(lx)); }
#elif defined(PARSER_LANG_REBUS)
int rebus_yylex(void) { static int (*real)(void); if (!real) real = (int (*)(void)) lex_real("rebus_yylex"); LEX_CLOCKED(real()); }
#elif defined(PARSER_LANG_PASCAL)
int pascal_lex_wrapped(void) { static int (*real)(void); if (!real) real = (int (*)(void)) lex_real("pascal_lex_wrapped"); LEX_CLOCKED(real()); }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lex_clock_noop(int i) { return i; }
static void lex_clock_calibrate(double * in_ns, double * full_ns) {
    enum { N = 4000000 }; int (* volatile fn)(int) = lex_clock_noop; int64_t acc = 0, s = 0;
    for (int i = 0; i < N; i++) { int64_t a = mono_ns(); acc += mono_ns() - a; }
    *in_ns = (double) acc / N;
    int64_t t0 = mono_ns(); for (int i = 0; i < N; i++) s += fn(i); int64_t t1 = mono_ns();
    for (int i = 0; i < N; i++) { int64_t a = mono_ns(); s += fn(i); acc += mono_ns() - a; } int64_t t2 = mono_ns();
    *full_ns = (double) ((t2 - t1) - (t1 - t0)) / N + (double) (s & 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if defined(PARSER_LANG_SNOBOL4)
static int parse_snobol4_program(char * src, size_t len, const char * path, int64_t * pns) {
    int64_t a = mono_ns();
    sno_reset();
    tree_t * ast = sno_parse_ast_buf(src, len, path, NULL, 0);
    *pns += mono_ns() - a;
    if (!ast) { puts("Parse Error"); return 1; }
    for (int i = 0; i < ast->n; i++) ir_dump_tree(ast->c[i], stdout);
    return 0;
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int parse_and_dump(char * src, size_t len, const char * name, int64_t * pns) {
    stmt_src_set_file(name);
#if defined(PARSER_LANG_SNOBOL4)
    return parse_snobol4_program(src, len, name, pns);
#else
    (void)len;
    tree_t * ast = NULL;
    int64_t a = mono_ns();
    void * parsed = PARSER_PARSE(src, name);
    *pns += mono_ns() - a;
    { int64_t ln = g_lex_ns, lc = g_lex_calls; PARSER_FINISH(parsed, name, &ast); g_lex_ns = ln; g_lex_calls = lc; }
    if (!ast) { puts("Parse Error"); return 1; }
    for (int i = 0; i < ast->n; i++) ir_dump_tree(ast->c[i], stdout);
    return 0;
#endif
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char ** argv) {
    const char * list = getenv("PARSER_FILES");
    if (list && *list) {
        FILE * lf = fopen(list, "r");
        if (!lf) { fprintf(stderr, "parser_%s: cannot open list '%s'\n", PARSER_NAME, list); return 2; }
        size_t llen = 0; char * names = slurp(lf, &llen); fclose(lf);
        if (!names) { fprintf(stderr, "parser_%s: out of memory\n", PARSER_NAME); return 2; }
        int rc = 0; long files = 0, bytes = 0; int64_t pns = 0, pns1 = 0;
        for (char * nm = strtok(names, "\n"); nm; nm = strtok(NULL, "\n")) {
            FILE * f = fopen(nm, "r");
            printf("== %s\n", nm);
            if (!f) { puts("Parse Error"); rc = 1; continue; }
            size_t len = 0; char * src = slurp(f, &len); fclose(f);
            if (!src) { fprintf(stderr, "parser_%s: out of memory\n", PARSER_NAME); return 2; }
            bytes += (long)len;
            fflush(stdout);
            if (parse_and_dump(src, len, nm, &pns)) rc = 1;
            fflush(stdout);
            if (++files == 1) pns1 = pns;
        }
        char lexm[256] = "";
        if (lex_clock_on()) { double in_ns = 0, full_ns = 0; lex_clock_calibrate(&in_ns, &full_ns); double lex_ns = (double) g_lex_ns - (double) g_lex_calls * in_ns, par_ns = (double) pns - lex_ns - (double) g_lex_calls * full_ns;
            snprintf(lexm, sizeof lexm, " lex_calls=%lld lex_raw_us=%lld clock_in_ns=%.1f clock_full_ns=%.1f lex_us=%.0f parser_us=%.0f", (long long) g_lex_calls, (long long) (g_lex_ns / 1000), in_ns, full_ns, lex_ns / 1000, par_ns / 1000); }
        fprintf(stderr, "PARSER-METRICS files=%ld bytes=%ld parse_first_us=%lld parse_us=%lld%s\n", files, bytes, (long long)(pns1 / 1000), (long long)(pns / 1000), lexm);
        return rc;
    }
    const char * path = (argc > 1 && strcmp(argv[1], "-") != 0) ? argv[1] : NULL;
    FILE * f = path ? fopen(path, "r") : stdin;
    if (!f) { fprintf(stderr, "parser_%s: cannot open '%s'\n", PARSER_NAME, path); return 2; }
    size_t len = 0; char * src = slurp(f, &len);
    if (path) fclose(f);
    if (!src) { fprintf(stderr, "parser_%s: out of memory\n", PARSER_NAME); return 2; }
    int64_t pns = 0;
    return parse_and_dump(src, len, path ? path : "(stdin)", &pns);
}
