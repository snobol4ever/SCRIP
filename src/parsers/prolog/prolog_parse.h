/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PL_PARSE_H
#define PL_PARSE_H
#include "rt/prolog_atom.h"
#include "ast.h"
#include <stdio.h>
typedef struct PlClause PlClause;
struct PlClause {
    int       nbody;
    int       lineno;
    PlClause *next;
    tree_t   *tr;
    char    **var_names;
    int       nvar;
    int       is_dcg;
};
typedef struct {
    PlClause *head;
    PlClause *tail;
    int       nclauses;
    int       nerrors;
    const char *src;
    const char *filename;
    int       quiet;
} PlProgram;
void prolog_preprocess(PlProgram *prog);
PlProgram *prolog_parse(const char *src, const char *filename);
void prolog_inject_prelude(PlProgram *prog, const char *user_src);
void prolog_dcg_expand(PlClause *cl);
PlProgram *prolog_parse_ex(const char *src, const char *filename, int quiet);
int pl_prelude_defines(const char *nm, int ar);
void prolog_program_free(PlProgram *prog);
int prolog_op_table_count(void);
int prolog_op_table_get(int idx, const char **name_out, int *prec_out, const char **type_out);
int prolog_op_table_add(const char *name, int prec, const char *type);
int prolog_op_user_count(void);
int prolog_op_user_get(int i, const char **name_out, int *prec_out, const char **type_out);
#endif
