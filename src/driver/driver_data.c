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
void class_inherit(const char *child, const char *parent) {
    if (!child || !parent) return;
    DatType *c = dat_find_type(child); DatType *p = dat_find_type(parent);
    if (!c || !p) return;
    if (c->parent[0]) return;
    char merged[64][64]; DESCR_t mdef[64]; char mhas[64]; char mreq[64]; int m = 0;
    for (int i = 0; i < p->nfields && m < 63; i++) { strncpy(merged[m], p->fields[i], 63); merged[m][63] = '\0'; mdef[m] = p->defaults[i]; mhas[m] = p->has_default[i]; mreq[m] = p->required[i]; m++; }
    for (int i = 0; i < c->nfields && m < 63; i++) {
        int dup = 0; for (int j = 0; j < m; j++) if (strcmp(merged[j], c->fields[i]) == 0) { dup = 1; break; }
        if (!dup) { strncpy(merged[m], c->fields[i], 63); merged[m][63] = '\0'; mdef[m] = c->defaults[i]; mhas[m] = c->has_default[i]; mreq[m] = c->required[i]; m++; }
    }
    for (int i = 0; i < m; i++) { strncpy(c->fields[i], merged[i], 63); c->defaults[i] = mdef[i]; c->has_default[i] = mhas[i]; c->required[i] = mreq[i]; }
    c->nfields = m;
    strncpy(c->parent, parent, 63); c->parent[63] = '\0';
}
/*--------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_default_i(const char *cls, const char *field, int64_t v) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->defaults[i] = INTVAL(v); t->has_default[i] = 1; return; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_default_s(const char *cls, const char *field, const char *v) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->defaults[i] = STRVAL(GC_strdup(v ? v : "")); t->has_default[i] = 1; return; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_default_r(const char *cls, const char *field, double v) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->defaults[i] = REALVAL(v); t->has_default[i] = 1; return; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_required(const char *cls, const char *field) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->required[i] = 1; return; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int dat_type_field_has_default(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].has_default[j] : 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
int dat_type_field_required(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].required[j] : 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t dat_type_field_default(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].defaults[j] : NULVCL; }
/*--------------------------------------------------------------------------------------------------------------------*/
const char *dat_parent(const char *name) {
    DatType *t = dat_find_type(name);
    return (t && t->parent[0]) ? t->parent : (const char *)0;
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
    for (int i = 0; i < t->nfields; i++) {
        inst->fields[i] = (i < nargs) ? args[i] : NULVCL;
        if (t->has_default[i] && inst->fields[i].v == DT_SNUL) inst->fields[i] = t->defaults[i];
    }
    for (int i = 0; i < t->nfields; i++) {
        if (t->required[i] && inst->fields[i].v == DT_SNUL) {
            extern void rt_script_die_surface(const char *msg);
            char _m[512]; snprintf(_m, sizeof _m, "The attribute '$!%s' is required, but you did not provide a value for it.", t->fields[i]);
            rt_script_die_surface(_m);
            break;
        }
    }
    DESCR_t r;
    r.v    = DT_DATA;
    r.slen = 0;
    r.u    = inst;
    extern void rt_fire_buildplan_tweak(const char *cname, DESCR_t self);
    rt_fire_buildplan_tweak(t->name, r);
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t dat_field_get(const char *fname, DESCR_t obj) {
    DESCR_t *cell = data_field_ptr(fname, obj);
    if (cell) return *cell;
    extern int rt_str_method(const char *meth, DESCR_t recv, const DESCR_t *margs, int nmargs, DESCR_t *out);
    DESCR_t r; if (obj.v != DT_DATA && rt_str_method(fname, obj, (DESCR_t *)0, 0, &r)) return r;
    return FAILDESCR;
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
