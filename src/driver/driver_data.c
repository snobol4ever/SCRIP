#include "driver_private.h"
DESCR_t _builtin_print(DESCR_t *args, int nargs) {
    if (nargs == 0) { output_str(""); return NULVCL; }
    for (int i = 0; i < nargs; i++) output_val(args[i]);
    return NULVCL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define SC_DAT_MAX_FIELDS 64
#define SC_DAT_MAX_TYPES  128
static DatType dat_types[SC_DAT_MAX_TYPES];
static int       dat_ntypes = 0;
DatType *dat_register(const char *spec) {
    if (dat_ntypes >= SC_DAT_MAX_TYPES) return NULL;
    DatType *t = &dat_types[dat_ntypes];
    memset(t, 0, sizeof *t);
    const char *p = spec;
    int ni = 0;
    while (*p && *p != '(' && ni < 63) t->name[ni++] = *p++;
    t->name[ni] = '\0';
    if (*p == '(') p++;
    while (*p && *p != ')') {
        while (*p == ' ' || *p == '\t') p++;
        if (!*p || *p == ')') break;
        int fi = 0;
        while (*p && *p != ',' && *p != ')' && fi < 63) t->fields[t->nfields][fi++] = *p++;
        t->fields[t->nfields][fi] = '\0';
        if (t->nfields < SC_DAT_MAX_FIELDS - 1) t->nfields++;
        if (*p == ',') p++;
    }
    dat_ntypes++;
    return t;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void record_register(const char *spec) {
    if (!spec || !*spec) return;
    const char *p = spec;
    char name[64]; int ni = 0;
    while (*p && *p != '(' && ni < 63) name[ni++] = *p++;
    name[ni] = '\0';
    if (dat_find_type(name)) return;
    DEFDAT_fn(spec);
    dat_register(spec);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DatType *dat_find_type(const char *name) {
    for (int i = 0; i < dat_ntypes; i++)
        if (strcmp(dat_types[i].name, name) == 0) return &dat_types[i];
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int dat_type_count(void) { return dat_ntypes; }
/*--------------------------------------------------------------------------------------------------------------------*/
const char *dat_type_name(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].name : (const char *)0; }
/*--------------------------------------------------------------------------------------------------------------------*/
int dat_type_nfields(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].nfields : 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
const char *dat_type_field(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].fields[j] : (const char *)0; }
/*--------------------------------------------------------------------------------------------------------------------*/
DatType *dat_find_field(const char *name, int *fidx) {
    for (int i = 0; i < dat_ntypes; i++)
        for (int j = 0; j < dat_types[i].nfields; j++)
            if (strcmp(dat_types[i].fields[j], name) == 0) {
                if (fidx) *fidx = j;
                return &dat_types[i];
            }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t dat_construct(DatType *t, DESCR_t *args, int nargs) {
    DATINST_t *inst = GC_malloc(sizeof(DATINST_t));
    DATBLK_t *blk = GC_malloc(sizeof(DATBLK_t));
    blk->name    = GC_strdup(t->name);
    blk->nfields = t->nfields;
    blk->fields  = GC_malloc(t->nfields * sizeof(char *));
    for (int i = 0; i < t->nfields; i++) blk->fields[i] = GC_strdup(t->fields[i]);
    blk->next    = NULL;
    inst->type   = blk;
    inst->fields = GC_malloc(t->nfields * sizeof(DESCR_t));
    for (int i = 0; i < t->nfields; i++)
        inst->fields[i] = (i < nargs) ? args[i] : NULVCL;
    DESCR_t r;
    r.v    = DT_DATA;
    r.slen = 0;
    r.u    = inst;
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t dat_field_get(const char *fname, DESCR_t obj) {
    DESCR_t *cell = data_field_ptr(fname, obj);
    if (!cell) return FAILDESCR;
    return *cell;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t _builtin_DATA(DESCR_t *args, int nargs) {
    if (nargs < 1) return FAILDESCR;
    const char *raw_spec = VARVAL_fn(args[0]);
    if (!raw_spec || !*raw_spec) return FAILDESCR;
    char *spec = GC_strdup(raw_spec);
    DEFDAT_fn(spec);
    dat_register(spec);
    extern DESCR_t core_DATA_register(DESCR_t *a, int n);
    core_DATA_register(args, nargs);
    return NULVCL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t *data_field_ptr(const char *fname, DESCR_t inst) {
    if (inst.v < DT_DATA || !inst.u) return NULL;
    DATBLK_t *blk = inst.u->type;
    if (!blk) return NULL;
    for (int i = 0; i < blk->nfields; i++)
        if (blk->fields[i] && strcmp(blk->fields[i], fname) == 0)
            return &inst.u->fields[i];
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#include "../runtime/builtins/gen_runtime.h"
