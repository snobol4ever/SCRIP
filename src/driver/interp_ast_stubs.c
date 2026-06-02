#include "core.h"
#include "parser/snobol4/scrip_cc.h"
#include <stdio.h>
const tree_t *g_exec_prog = NULL;
void execute_program_steps(const tree_t *prog, int n) { (void)prog; (void)n; fprintf(stderr, "[NO-AST] execute_program_steps stub\n"); }
DESCR_t interp_eval(tree_t *e) { (void)e; fprintf(stderr, "[NO-AST] interp_eval stub\n"); return FAILDESCR; }
