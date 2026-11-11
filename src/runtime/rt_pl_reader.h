/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RT_PL_READER_H
#define RT_PL_READER_H
#include "runtime/rt/prolog_atom.h"
#include "ir/ast.h"
#include <stdio.h>
typedef struct PlClause PlClause;
struct PlClause { int nbody; int lineno; PlClause *next; tree_t *tr; char **var_names; int nvar; int is_dcg; int dq_forced; const char *file; int sstart; int send; };
typedef struct { PlClause *head; PlClause *tail; int nclauses; int nerrors; const char *src; const char *filename; int quiet; const char *synerr; int *cm; int ncm; int consults_yall; } PlProgram;
void prolog_fold_pieces(tree_t *t);
int prolog_token_extent(const char *src, int from, int skip_close, int *tstart, int *tend);
PlProgram *prolog_parse_ex(const char *src, const char *filename, int quiet);
int prolog_op_table_count(void);
int prolog_op_table_get(int idx, const char **name_out, int *prec_out, const char **type_out);
int prolog_op_table_add(const char *name, int prec, const char *type);
#endif
