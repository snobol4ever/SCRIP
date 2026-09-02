/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PL_BUILTIN_H
#define PL_BUILTIN_H
#include "term.h"
#include "prolog_runtime.h"
void pl_write(Term *t);
void pl_writeq(Term *t);
void pl_write_canonical(Term *t);
#endif
#ifndef EXPR_T_DEFINED
struct tree_t;
typedef struct tree_t tree_t;
#define EXPR_T_DEFINED 1
#endif
