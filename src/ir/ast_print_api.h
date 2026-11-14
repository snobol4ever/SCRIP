#ifndef AST_PRINT_API_H
#define AST_PRINT_API_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ir/ast.h"
typedef struct tree_t tree_t;
void ir_print_node (const tree_t *e, FILE *f);
#endif
