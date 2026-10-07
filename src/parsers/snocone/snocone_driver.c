#include "snocone_driver.h"
#include <stdio.h>
tree_t *snocone_parse_tree(const char *src, const char *filename);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *snocone_compile_parse(const char *source, const char *filename)
{
    return snocone_parse_tree(source, filename ? filename : "<stdin>");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void snocone_compile_finish(void *parsed, const char *filename, tree_t **out_ast)
{
    (void)filename;
    if (out_ast) *out_ast = (tree_t *)parsed;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void snocone_compile(const char *source, const char *filename, tree_t **out_ast)
{
    if (out_ast) *out_ast = NULL;
    snocone_compile_finish(snocone_compile_parse(source, filename), filename, out_ast);
}
