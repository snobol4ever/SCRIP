#include "core.h"
#include "builtins/gen_runtime.h"
#include <string.h>
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* sm_call_proc DELETED 2026-08-27 (row sm-eval-subexpr-weak-abort-landmine, arm (a) RESIDUE, decided by measurement not inspection).
   It built a GenFrame, walked g_stage2.proc_table[].proc as a tree_t AST at RUNTIME (against the standing no-AST-walking rule), then called
   sm_eval_subexpr() -- a weak stub with NO strong definition anywhere, which abort()s unconditionally. So the function could never return: every
   call with entry_pc >= 0 killed the process. It was unreachable only because all 17 live stage2_proc_grow() sites set entry_pc = -1 by hand,
   over a memset-to-0 default in which 0 means ">= 0" means abort. That is an incidental guard, not a structural one -- one forgotten assignment
   at a new call site would have armed it. test_gate_no_weak_abort_stub.sh now pins both halves so a refactor cannot silently re-open it. */
DESCR_t proc_table_call(int pi, DESCR_t *args, int nargs)
{
    (void)args; (void)nargs;
    /* Behaviour-preserving: entry_pc is -1 at every live producer, so the old `entry_pc >= 0` branch was already dead and this
       already returned FAILDESCR for every input. The branch it guarded is gone; the answer for every input is unchanged. */
    if (pi < 0 || pi >= g_stage2.proc_count) return FAILDESCR;
    return FAILDESCR;
}
