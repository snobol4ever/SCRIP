#include "pascal_driver.h"
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include <stdio.h>
#include <stdlib.h>
extern tree_t *pascal_prog_result;
extern tree_t *pascal_parse_string(const char *src);
extern int pascal_iso_error_count(void);
extern void pascal_iso_error_reset(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *pascal_compile_parse(const char *src, const char *filename) {
    (void)filename;
    pascal_prog_result = NULL;
    pascal_iso_error_reset();
    return pascal_parse_string(src);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pascal_compile(const char *src, const char *filename, tree_t **out_ast) {
    if (out_ast) *out_ast = NULL;
    pascal_compile_finish(pascal_compile_parse(src, filename), filename, out_ast);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pascal_compile_finish(void *parsed, const char *filename, tree_t **out_ast) {
    if (!filename) filename = "<stdin>";
    if (out_ast) *out_ast = NULL;
    tree_t *prog = (tree_t *)parsed;
    if (!prog) {
        fprintf(stderr, "pascal: parse error in %s\n", filename);
        return;
    }
    int nsem = pascal_sem_check(prog, filename) + pascal_iso_error_count();
    if (nsem > 0) { fprintf(stderr, "pascal: %d ISO 7185 violation(s) in %s -- no code generated\n", nsem, filename); return; }
    if (out_ast) *out_ast = prog;
}
