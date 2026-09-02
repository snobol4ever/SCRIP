#include <stdio.h>
#include <stdlib.h>
#include "scrip_sm.h"
#include "lower.h"
#include "../parsers/snobol4/scrip_cc.h"
#include "IR.h"
#include "bb_program.h"
#include "../runtime/builtins/gen_runtime.h"
#include "driver_private.h"
#include "polyglot.h"
extern int g_core_err_active;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sm_resolve_proc_entry_pcs(stage2_t *s2)
{
    for (int i = 0; i < s2->proc_count; i++)
        s2->proc_table[i].entry_pc = -1;
    for (int b = 0; b < STAGE2_PL_PRED_TABLE_SIZE; b++) {
        for (Resolve_PredEntry *e = s2->resolve_pred_table.buckets[b]; e; e = e->next)
            e->entry_pc = -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *sm_preamble(const tree_t *ast_prog, const lower_seg_t *segs, int nsegs){
    g_core_err_active = 1;
    stage2_reset();
    polyglot_init(&g_stage2, ast_prog);
    stage2_t *s2 = (nsegs > 0) ? &g_stage2 : NULL;
    for (int i = 0; i < nsegs; i++) { if (!segs[i].fn || !segs[i].prog || !segs[i].fn(segs[i].prog)) s2 = NULL; }
    if (!s2) {
        fprintf(stderr, "scrip: sm_lower failed\n");
        return NULL;
    }
    sm_resolve_proc_entry_pcs(s2);
    return s2;
}
