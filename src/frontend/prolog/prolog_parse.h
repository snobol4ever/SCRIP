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
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    PlClause *head;
    PlClause *tail;
    int       nclauses;
    int       nerrors;
} PlProgram;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PlProgram *prolog_parse(const char *src, const char *filename);
void prolog_program_free(PlProgram *prog);
#endif
