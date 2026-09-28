#include <stdio.h>
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
#define PARSER_COMPILE snocone_compile
#elif defined(PARSER_LANG_ICON)
#include "parsers/icon/icon_driver.h"
#define PARSER_NAME "icon"
#define PARSER_COMPILE icon_compile
#elif defined(PARSER_LANG_PROLOG)
#include "parsers/prolog/prolog_driver.h"
#define PARSER_NAME "prolog"
#define PARSER_COMPILE prolog_compile
#elif defined(PARSER_LANG_REBUS)
#include "parsers/rebus/rebus_lower.h"
#define PARSER_NAME "rebus"
#define PARSER_COMPILE rebus_compile
#elif defined(PARSER_LANG_RAKU)
#include "parsers/raku/raku_driver.h"
#define PARSER_NAME "raku"
#define PARSER_COMPILE raku_compile
#elif defined(PARSER_LANG_PASCAL)
#include "parsers/pascal/pascal_driver.h"
#define PARSER_NAME "pascal"
#define PARSER_COMPILE pascal_compile
#else
#error "parser_main.c is compiled once per frontend: -DPARSER_LANG_<SNOBOL4|SNOCONE|ICON|PROLOG|REBUS|RAKU|PASCAL>"
#endif
extern void ir_dump_tree(const tree_t * e, FILE * f);
extern void stmt_src_set_file(const char * path);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char * slurp(FILE * f, size_t * len) {
    size_t cap = 1 << 16, n = 0; char * b = ct_alloc(cap + 1);
    if (!b) return NULL;
    for (;;) { size_t r = fread(b + n, 1, cap - n, f); n += r; if (n < cap) break; cap *= 2; char * nb = ct_grow(b, cap + 1); if (!nb) { ct_drop(b); return NULL; } b = nb; }
    b[n] = 0; *len = n; return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void include_dirs_from_env(const char * path) {
#if defined(PARSER_LANG_SNOBOL4)
    const char * lib = getenv("SNO_LIB");
    if (lib && *lib) { char * copy = ct_strdup(lib); char * sp = copy; char * tk; while ((tk = strsep(&sp, ":")) != NULL) if (*tk) sno_add_include_dir(ct_strdup(tk)); }
    if (path) { char * d = ct_strdup(path); char * sl = strrchr(d, '/'); if (sl) { *sl = 0; sno_add_include_dir(d); } else sno_add_include_dir("."); }
    sno_add_include_dir(".");
#else
    (void)path;
#endif
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if defined(PARSER_LANG_SNOBOL4)
static int parse_snobol4_program(char * src, size_t len, const char * path) {
    FILE * mf = fmemopen(src, len, "r");
    if (!mf) { fprintf(stderr, "%s: fmemopen failed\n", PARSER_NAME); return 2; }
    sno_reset();
    tree_t * ast = sno_parse_ast(mf, path, NULL);
    fclose(mf);
    if (!ast) { puts("Parse Error"); return 1; }
    for (int i = 0; i < ast->n; i++) ir_dump_tree(ast->c[i], stdout);
    return 0;
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t mono_ns(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return (int64_t)ts.tv_sec * 1000000000LL + (int64_t)ts.tv_nsec; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int parse_and_dump(char * src, size_t len, const char * name) {
    stmt_src_set_file(name);
    include_dirs_from_env(strcmp(name, "(stdin)") ? name : NULL);
#if defined(PARSER_LANG_SNOBOL4)
    return parse_snobol4_program(src, len, name);
#else
    (void)len;
    tree_t * ast = NULL;
    PARSER_COMPILE(src, name, &ast);
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
        int rc = 0; long files = 0, bytes = 0; int64_t t0 = mono_ns(), t1 = t0;
        for (char * nm = strtok(names, "\n"); nm; nm = strtok(NULL, "\n")) {
            FILE * f = fopen(nm, "r");
            printf("== %s\n", nm);
            if (!f) { puts("Parse Error"); rc = 1; continue; }
            size_t len = 0; char * src = slurp(f, &len); fclose(f);
            if (!src) { fprintf(stderr, "parser_%s: out of memory\n", PARSER_NAME); return 2; }
            bytes += (long)len;
            fflush(stdout);
            if (parse_and_dump(src, len, nm)) rc = 1;
            fflush(stdout);
            if (++files == 1) t1 = mono_ns();
        }
        int64_t t2 = mono_ns();
        fprintf(stderr, "PARSER-METRICS files=%ld bytes=%ld first_us=%lld rest_us=%lld total_us=%lld\n", files, bytes, (long long)((t1 - t0) / 1000), (long long)((t2 - t1) / 1000), (long long)((t2 - t0) / 1000));
        return rc;
    }
    const char * path = (argc > 1 && strcmp(argv[1], "-") != 0) ? argv[1] : NULL;
    FILE * f = path ? fopen(path, "r") : stdin;
    if (!f) { fprintf(stderr, "parser_%s: cannot open '%s'\n", PARSER_NAME, path); return 2; }
    size_t len = 0; char * src = slurp(f, &len);
    if (path) fclose(f);
    if (!src) { fprintf(stderr, "parser_%s: out of memory\n", PARSER_NAME); return 2; }
    return parse_and_dump(src, len, path ? path : "(stdin)");
}
