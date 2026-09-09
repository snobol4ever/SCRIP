#include "core.h"
#include "builtins/gen_runtime.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
const char *global_names[GLOBAL_MAX];
int         global_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_global(const char *name) {
    for (int i = 0; i < global_count; i++)
        if (global_names[i] && strcmp(global_names[i], name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void global_register(const char *name) {
    if (!name || is_global(name)) return;
    if (global_count >= GLOBAL_MAX) { fprintf(stderr, "scrip: BOMB — global_register: GLOBAL_MAX (%d) exceeded at '%s' — silent drop would misclassify globals as locals\n", GLOBAL_MAX, name); abort(); }
    global_names[global_count++] = name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rbi_names[GLOBAL_MAX];
static int rbi_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_note_reassigned_builtin(const char *name) {
    if (!name) return;
    for (int i = 0; i < rbi_count; i++) if (rbi_names[i] && strcmp(rbi_names[i], name) == 0) return;
    if (rbi_count >= GLOBAL_MAX) return;
    rbi_names[rbi_count++] = name;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_is_reassigned_builtin(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < rbi_count; i++) if (rbi_names[i] && strcmp(rbi_names[i], name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scope_add(Scope *sc, const char *name) {
    if (!name) return -1;
    for (int i=0;i<sc->n;i++) if(strcmp(sc->e[i].name,name)==0) return sc->e[i].slot;
    if (sc->n >= FRAME_SLOT_MAX) return -1;
    int slot = sc->n;
    sc->e[sc->n].name=name; sc->e[sc->n].slot=slot; sc->n++;
    return slot;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int scope_get(Scope *sc, const char *name) {
    if (!name) return -1;
    for (int i=0;i<sc->n;i++) if(strcmp(sc->e[i].name,name)==0) return sc->e[i].slot;
    return -1;
}
