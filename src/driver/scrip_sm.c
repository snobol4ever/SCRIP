#include <stdio.h>
#include <stdlib.h>
#include "scrip_sm.h"
#include "lower.h"
#include "../parser/snobol4/scrip_cc.h"
#include "IR.h"
#include "bb_program.h"
#include "../runtime/builtins/gen_runtime.h"
#include "../runtime/builtins/resolution.h"
#include "driver_private.h"
#include "polyglot.h"
extern int g_core_err_active;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sm_resolve_proc_entry_pcs(stage2_t *s2)
{
    for (int i = 0; i < s2->proc_count; i++)
        s2->proc_table[i].entry_pc = -1;
    for (int b = 0; b < RESOLVE_PRED_TABLE_SIZE_FWD; b++) {
        for (Resolve_PredEntry *e = s2->resolve_pred_table.buckets[b]; e; e = e->next)
            e->entry_pc = -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *sm_preamble(const tree_t *ast_prog){
    g_core_err_active = 1;
    stage2_t *s2 = lower_stage2(ast_prog);
    if (!s2) {
        fprintf(stderr, "scrip: sm_lower failed\n");
        return NULL;
    }
    if (polyglot_lang_mask(ast_prog) & (1u << LANG_ICN)) {
        extern int g_lang;
        g_lang = LANG_ICN;
    }
    sm_resolve_proc_entry_pcs(s2);
    return s2;
}
