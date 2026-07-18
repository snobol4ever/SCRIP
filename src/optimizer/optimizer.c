#include "optimizer.h"
#include "branch_chain.h"
#include "const_fold.h"
#include "copy_prop.h"
#include "pat_fold.h"
#include "dead_pure.h"
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void optimizer_run(IR_graph_t *g) {
    { extern void region_report(IR_graph_t *); if (getenv("SCRIP_REGION_REPORT")) region_report(g); }
    if (!g) return;
    const char *e = getenv("SCRIP_OPT");
    if (e && *e == '0') return;
    int t_cf = 0, t_cp = 0, t_pf = 0, t_dp = 0, t_bc = 0;
    for (int round = 0; round < 8; round++) {
        int n_cf = cf_run(g), n_cp = cp_run(g), n_pf = pf_run(g), n_dp = dp_run(g);
        int b = bc_run(g);
        t_cf += n_cf; t_cp += n_cp; t_pf += n_pf; t_dp += n_dp; t_bc += b;
        if (!(n_cf + n_cp + n_pf + n_dp)) break;
    }
    if (getenv("SCRIP_OPT_STATS")) fprintf(stderr, "[optimizer] fold=%d copy=%d pat=%d dead=%d branch_chain=%d\n", t_cf, t_cp, t_pf, t_dp, t_bc);
}
