#include "core.h"
#include "parser/snobol4/scrip_cc.h"
#include <stdio.h>
const tree_t *g_exec_prog = NULL;
DESCR_t interp_eval(tree_t *e) { (void)e; fprintf(stderr, "[NO-AST] interp_eval stub\n"); return FAILDESCR; }
