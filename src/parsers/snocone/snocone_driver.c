#include "snocone_driver.h"
#include "../../parsers/snobol4/scrip_cc.h"
#include <stdio.h>
CODE_t *snocone_parse_program(const char *src, const char *filename);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *snocone_compile_parse(const char *source, const char *filename)
{
    return snocone_parse_program(source, filename ? filename : "<stdin>");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void snocone_compile_finish(void *parsed, const char *filename, tree_t **out_ast)
{
    (void)filename;
    if (out_ast) *out_ast = parsed ? code_to_ast((CODE_t *)parsed) : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void snocone_compile(const char *source, const char *filename, tree_t **out_ast)
{
    if (out_ast) *out_ast = NULL;
    snocone_compile_finish(snocone_compile_parse(source, filename), filename, out_ast);
}
