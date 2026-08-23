#include "rt_protected.h"
#include "SM.h"
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_protected_pat_name(const char *name)
{
    if (!name || !is_protected_pat_lead(name[0])) return 0;   /* same predicate the hot callers pre-filter with -- see rt_protected.h */
    switch (name[0]) {
    case 'A': return strcmp(name, "ARB") == 0 || strcmp(name, "ABORT") == 0;
    case 'B': return strcmp(name, "BAL") == 0;
    case 'F': return strcmp(name, "FENCE") == 0 || strcmp(name, "FAIL") == 0;
    case 'R': return strcmp(name, "REM") == 0;
    case 'S': return strcmp(name, "SUCCEED") == 0;
    default:  return 0;
    }
}
