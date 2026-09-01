/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PROLOG_LOWER_H
#define PROLOG_LOWER_H
#include "scrip_cc.h"
#include "prolog_parse.h"
#include <stdio.h>
CODE_t *prolog_lower(PlProgram *pl_prog);
void prolog_lower_pretty(CODE_t *prog, FILE *out);
#endif
