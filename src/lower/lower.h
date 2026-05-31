#ifndef LOWER_H
#define LOWER_H
#include "SM.h"
#include "stage2.h"
#include "../../ast/ast.h"
stage2_t *lower(const tree_t *prog);
#endif
