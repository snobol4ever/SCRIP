#include "rebus_driver.h"
#include "rebus.h"
#include <stdio.h>
#include <string.h>
#include "ir/ast.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rebus_compile_parse(const char *src, const char *filename) { return rebus_parse_buf(src, strlen(src), filename ? filename : "<stdin>"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rebus_compile_finish(void *parsed, const char *filename, tree_t **out_ast) {
    if (!filename) filename = "<stdin>";
    if (out_ast) *out_ast = NULL;
    if (rebus_nerrors > 0) { fprintf(stderr, "rebus_compile: %d parse error(s) in %s -- no code generated\n", rebus_nerrors, filename); return; }
    if (!parsed) { fprintf(stderr, "rebus_compile: parse error in %s\n", filename); return; }
    if (out_ast) *out_ast = (tree_t *)parsed;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rebus_compile(const char *src, const char *filename, tree_t **out_ast) { if (out_ast) *out_ast = NULL; rebus_compile_finish(rebus_compile_parse(src, filename), filename, out_ast); }
