#include "lower_ctx.h"
#include "../../runtime/interp/gen_runtime.h"
#include "../ast/ast.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <gc/gc.h>
char *kw_canonicalize(const char *raw)
{
    if (!raw) raw = "";
    size_t n = strlen(raw);
    char *buf = GC_MALLOC(n + 1);
    for (size_t i = 0; i < n; i++) buf[i] = (char)toupper((unsigned char)raw[i]);
    buf[n] = '\0';
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void expression_scope_walk(Scope *sc, tree_t *e)
{
    if (!e) return;
    if (e->t == TT_GLOBAL) {
        for (int i = 0; i < e->n; i++)
            if (e->c[i] && e->c[i]->v.sval)
                scope_add(sc, e->c[i]->v.sval);
        return;
    }
    if (e->t == TT_INITIAL) return;
    if (e->t == TT_VAR && e->v.sval && e->v.sval[0] != '&' && !is_global(e->v.sval))
        scope_add(sc, e->v.sval);
    for (int i = 0; i < e->n; i++)
        expression_scope_walk(sc, e->c[i]);
}
