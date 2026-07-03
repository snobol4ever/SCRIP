#include "core.h"
#include "builtins/gen_runtime.h"
#include <string.h>
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
    if (!name || is_global(name) || global_count >= GLOBAL_MAX) return;
    global_names[global_count++] = name;
}
typedef struct {
    int         entry_pc;
    const char *proc_name;
    const char *name;
    DESCR_t     val;
} static_ent_t;
#define STATIC_MAX 256
static static_ent_t static_tab[STATIC_MAX];
static int              static_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int static_proc_entry_pc(const char *proc_name) {
    if (!proc_name) return -1;
    for (int i = 0; i < g_stage2.proc_count; i++)
        if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, proc_name) == 0)
            return g_stage2.proc_table[i].entry_pc;
    return -1;
}
static int static_entry_matches(const static_ent_t *e, int epc,
                                const char *pname, const char *vname) {
    if (!e->name || !vname || strcmp(e->name, vname) != 0) return 0;
    if (epc >= 0 && e->entry_pc >= 0) return e->entry_pc == epc;
    return e->proc_name && pname && strcmp(e->proc_name, pname) == 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int static_get(tree_t *proc, const char *name, DESCR_t *out) {
    if (!proc || !name || !out) return 0;
    const char *pname = proc->v.sval;
    int epc = static_proc_entry_pc(pname);
    for (int i = 0; i < static_n; i++) {
        if (static_entry_matches(&static_tab[i], epc, pname, name)) {
            *out = static_tab[i].val;
            return 1;
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void static_set(tree_t *proc, const char *name, DESCR_t val) {
    if (!proc || !name) return;
    const char *pname = proc->v.sval;
    int epc = static_proc_entry_pc(pname);
    for (int i = 0; i < static_n; i++) {
        if (static_entry_matches(&static_tab[i], epc, pname, name)) {
            if (epc >= 0 && static_tab[i].entry_pc < 0)
                static_tab[i].entry_pc = epc;
            static_tab[i].val = val;
            return;
        }
    }
    if (static_n >= STATIC_MAX) return;
    static_tab[static_n].entry_pc  = epc;
    static_tab[static_n].proc_name = pname;
    static_tab[static_n].name      = name;
    static_tab[static_n].val       = val;
    static_n++;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void scope_patch(Scope *sc, tree_t *e) {
    if (!e) return;
    if (e->t == TT_GLOBAL) {
        for (int i=0;i<e->n;i++)
            if(e->c[i]&&e->c[i]->v.sval) scope_add(sc, e->c[i]->v.sval);
        return;
    }
    if (e->t == TT_VAR && e->v.sval) {
        if (e->v.sval[0] == '&') {
            e->v.ival = -1;
        } else if (is_global(e->v.sval)) {
            e->v.ival = -1;
        } else {
            int s = scope_add(sc, e->v.sval);
            e->v.ival = (s >= 0) ? s : -1;
        }
    }
    int child_start = (e->t == TT_FNC) ? 1 : 0;
    for (int i=child_start;i<e->n;i++) scope_patch(sc, e->c[i]);
}
