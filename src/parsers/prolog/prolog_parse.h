/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PL_PARSE_H
#define PL_PARSE_H
#include "term.h"
#include "prolog_atom.h"
#include "ast.h"
#include <stdio.h>
typedef struct PlClause PlClause;
struct PlClause {
    Term     *head;
    Term    **body;
    int       nbody;
    int       lineno;
    PlClause *next;
    tree_t   *tr;
    char    **var_names;
    Term    **var_terms;
    int       nvar;
    int       is_dcg;
};
typedef struct {
    PlClause *head;
    PlClause *tail;
    int       nclauses;
    int       nerrors;
} PlProgram;
PlProgram *prolog_parse(const char *src, const char *filename);
void prolog_program_free(PlProgram *prog);
int prolog_op_table_count(void);
int prolog_op_table_get(int idx, const char **name_out, int *prec_out, const char **type_out);
int prolog_op_table_add(const char *name, int prec, const char *type);
int prolog_op_user_count(void);
int prolog_op_user_get(int i, const char **name_out, int *prec_out, const char **type_out);
#endif
