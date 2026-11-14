/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PROLOG_LOWER_H
#define PROLOG_LOWER_H
#include "prolog_parse.h"
#include <stdio.h>
#include "ir/ast.h"
tree_t *prolog_lower(PlProgram *pl_prog);
#endif
