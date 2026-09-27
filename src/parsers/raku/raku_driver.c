#include "raku_driver.h"
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "rk_syntax.h"
#include <stdio.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void raku_compile(const char *src, const char *filename, tree_t **out_ast) {
    if (!filename) filename = "<stdin>";
    if (out_ast) *out_ast = NULL;
    char *err = NULL;
    tree_t *prog = rk_parse_tree(src, (int) strlen(src), filename, &err);
    if (!prog) {
        fprintf(stderr, "%s\n", err ? err : "raku: parse failed");
        fprintf(stderr, "raku: parse error in %s\n", filename);
        return;
    }
    if (out_ast) *out_ast = prog;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
