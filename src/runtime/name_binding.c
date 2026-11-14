#include "runtime/builtins/gen_runtime_name_binding_api.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ir/stage2_scope.h"
#include "ir/stage2_scope_reserve.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scope_add(Scope *sc, const char *name) {
    if (!name) return -1;
    for (int i=0;i<sc->n;i++) if(strcmp(sc->e[i].name,name)==0) return sc->e[i].slot;
    stage2_scope_reserve(sc, sc->n + 1);
    int slot = sc->n;
    sc->e[sc->n].name=name;
    sc->e[sc->n].slot=slot;
    sc->n++;
    return slot;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scope_get(Scope *sc, const char *name) { if (!name) return -1; for (int i=0;i<sc->n;i++) if(strcmp(sc->e[i].name,name)==0) return sc->e[i].slot; return -1; }
