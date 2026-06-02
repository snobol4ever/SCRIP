#include "rt_protected.h"
#include "SM.h"
#include <string.h>
static const struct {
    const char *name;
    SM_op_t     op;
} g_protected_pat_op[] = {
/*--------------------------------------------------------------------------------------------------------------------*/
    { "REM",     SM_PAT_REM     },
    { "ARB",     SM_PAT_ARB     },
    { "FENCE",   SM_PAT_FENCE0  },
    { "FAIL",    SM_PAT_FAIL    },
    { "SUCCEED", SM_PAT_SUCCEED },
    { "ABORT",   SM_PAT_ABORT   },
    { "BAL",     SM_PAT_BAL     },
    { NULL,      (SM_op_t)0     }
};
/*--------------------------------------------------------------------------------------------------------------------*/
int is_protected_pat_name(const char *name)
{
    if (!name) return 0;
    for (int i = 0; g_protected_pat_op[i].name; i++) {
        if (strcmp(name, g_protected_pat_op[i].name) == 0) return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int protected_pat_name_to_sm_op(const char *name)
{
    if (!name) return -1;
    for (int i = 0; g_protected_pat_op[i].name; i++) {
        if (strcmp(name, g_protected_pat_op[i].name) == 0)
            return (int)g_protected_pat_op[i].op;
    }
    return -1;
}
