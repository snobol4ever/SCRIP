#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
static int is_end_line(const char * p, const char * lim) {
    if (lim - p < 3 || strncmp(p, "END", 3) != 0) return 0;
    const char * q = p + 3;
    return q == lim || *q == '\n' || *q == ' ' || *q == '\t' || *q == '\r';
}
static int parse_snobol4_stream(char * src, size_t len, const char * path) {
    int programs = 0, refused = 0;
    char * p = src; char * lim = src + len;
    while (p < lim) {
        char * q = p; char * end = NULL;
        while (q < lim) { char * nl = memchr(q, '\n', (size_t)(lim - q)); char * next = nl ? nl + 1 : lim; if (is_end_line(q, lim)) { end = next; break; } q = next; }
        if (!end) end = lim;
        FILE * mf = fmemopen(p, (size_t)(end - p), "r");
        if (!mf) { fprintf(stderr, "%s: fmemopen failed\n", PARSER_NAME); return 2; }
        sno_reset();
        tree_t * ast = sno_parse_ast(mf, path, NULL);
        fclose(mf);
        programs++;
        if (ast) { for (int i = 0; i < ast->n; i++) ir_dump_tree(ast->c[i], stdout); } else { puts("Parse Error"); refused++; }
        p = end;
    }
    fprintf(stderr, "%s: %d program(s) at END boundaries, %d refused\n", PARSER_NAME, programs, refused);
    return refused ? 1 : 0;
}
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char ** argv) {
    const char * path = (argc > 1 && strcmp(argv[1], "-") != 0) ? argv[1] : NULL;
    FILE * f = path ? fopen(path, "r") : stdin;
    if (!f) { fprintf(stderr, "parser_%s: cannot open '%s'\n", PARSER_NAME, path); return 2; }
    size_t len = 0; char * src = slurp(f, &len);
    if (path) fclose(f);
    if (!src) { fprintf(stderr, "parser_%s: out of memory\n", PARSER_NAME); return 2; }
    const char * name = path ? path : "(stdin)";
    stmt_src_set_file(name);
    include_dirs_from_env(path);
#if defined(PARSER_LANG_SNOBOL4)
    return parse_snobol4_stream(src, len, name);
#else
    tree_t * ast = NULL;
    PARSER_COMPILE(src, name, &ast);
    if (!ast) { puts("Parse Error"); return 1; }
    for (int i = 0; i < ast->n; i++) ir_dump_tree(ast->c[i], stdout);
    return 0;
#endif
}
