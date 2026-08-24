#include "core.h"
#include "frontend/snobol4/scrip_cc.h"
#include <stdio.h>
const tree_t *g_exec_prog = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_ast(tree_t *e) { (void)e; fprintf(stderr, "[NO-AST] eval_ast stub\n"); return FAILDESCR; }
