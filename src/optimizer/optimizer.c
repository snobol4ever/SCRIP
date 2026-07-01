/*--------------------------------------------------------------------------------------------------------------------*/
#include "optimizer.h"
#include "branch_chain.h"
#include "copy_prop.h"
#include <stdio.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------------------------------------------------*/
void optimizer_run(IR_graph_t *g) {
    if (!g || !getenv("SCRIP_OPT")) return;
    int bc = bc_run(g);
    int cp = cp_run(g);
    if (getenv("SCRIP_OPT_STATS")) fprintf(stderr, "[optimizer] branch_chain=%d copy_prop=%d\n", bc, cp);
}
