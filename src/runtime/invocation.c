#include "core.h"
#include "builtins/gen_runtime.h"
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t proc_table_call(int pi, DESCR_t *args, int nargs)
{
    (void)args; (void)nargs;
    if (pi < 0 || pi >= g_stage2.proc_count) return FAILDESCR;
    return FAILDESCR;
}
