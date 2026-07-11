#include "optimizer.h"
#include "branch_chain.h"
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void optimizer_run(IR_graph_t *g) {
    { extern void region_report(IR_graph_t *); if (getenv("SCRIP_REGION_REPORT")) region_report(g); }
    if (!g) return;
    const char *e = getenv("SCRIP_OPT");
    if (e && *e == '0') return;
    int bc = bc_run(g);
    if (getenv("SCRIP_OPT_STATS")) fprintf(stderr, "[optimizer] branch_chain=%d\n", bc);
}
