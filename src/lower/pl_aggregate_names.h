#ifndef PL_AGGREGATE_NAMES_H
#define PL_AGGREGATE_NAMES_H
#include <string.h>
#include "ast.h"
static inline const char * pl_aggregate_all_native_kind(const tree_t * spec) {
    static const char * const kinds[] = { "count", "max", "min", "sum", "bag", "set", NULL };
    if (spec && spec->t == TT_FNC && spec->n == 1 && spec->v.sval) for (int i = 0; kinds[i]; i++) if (!strcmp(spec->v.sval, kinds[i])) return kinds[i];
    if (spec && (spec->t == TT_NAME || spec->t == TT_QLIT) && spec->v.sval && !strcmp(spec->v.sval, "count")) return "count";
    return NULL;
}
#endif
