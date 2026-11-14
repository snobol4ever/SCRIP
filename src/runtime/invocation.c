#include "runtime/builtins/gen_runtime_invocation_api.h"
#include <string.h>
#include "ir/stage2_sm_prog_api.h"
#include "ir/descr.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t proc_table_call(int pi, DESCR_t *args, int nargs) { (void)args; (void)nargs; if (pi < 0 || pi >= g_stage2.proc_count) return FAILDESCR; return FAILDESCR; }
