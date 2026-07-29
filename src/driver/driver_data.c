#include "rt/rt_arena.h"
#include "driver_private.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _builtin_print(DESCR_t *args, int nargs) {
    if (nargs == 0) { output_str(""); return NULVCL; }
    for (int i = 0; i < nargs; i++) output_val(args[i]);
    return NULVCL;
}
#define SC_DAT_MAX_FIELDS 64
#define SC_DAT_MAX_TYPES  1024   /* REC-REG-CAP: was 128 — silently dropped record types past the cap (jtran 17-module merge declares 415; u_pnull unresolved -> Error 5); 1024 + loud guard per the pair-array precedent */
static DatType dat_types[SC_DAT_MAX_TYPES];
static int       dat_ntypes = 0;
unsigned rt_dtax_gen = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_nfields_byref(void *t) { return t ? ((DatType *)t)->nfields : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t dat_construct_byref(void *t, DESCR_t *args, int nargs) { extern DESCR_t dat_construct(DatType *, DESCR_t *, int); return dat_construct((DatType *)t, args, nargs); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DatType *dat_register(const char *spec) {
    if (dat_ntypes >= SC_DAT_MAX_TYPES) { fprintf(stderr, "[REC-REG-CAP] FATAL: record/type registry saturated (%d >= %d) registering '%s' -- constructors past the cap resolve as Error 5; raise SC_DAT_MAX_TYPES\n", dat_ntypes, SC_DAT_MAX_TYPES, spec ? spec : "?"); return NULL; }
    DatType *t = &dat_types[dat_ntypes];
    memset(t, 0, sizeof *t);
    const char *p = spec;
    int ni = 0;
    while (*p && *p != '(' && ni < 63) t->name[ni++] = *p++;
    t->name[ni] = '\0';
    strncpy(t->mro[0], t->name, 63); t->mro[0][63] = '\0'; t->mro_len = 1;
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
    dat_ntypes++; rt_dtax_gen++;
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DatType *dat_find_type(const char *name) {
    for (int i = 0; i < dat_ntypes; i++)
        if (strcmp(dat_types[i].name, name) == 0) return &dat_types[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void class_inherit(const char *child, const char *parent) {
    if (!child || !parent) return;
    const char *one[1]; one[0] = parent;
    class_inherit_multi(child, one, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int c3_in_tail(char list[][64], int len, const char *name) {
    for (int k = 1; k < len; k++) if (strcmp(list[k], name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int c3_merge(char lists[][64][64], int *lens, int nlists, char out[][64], int outmax) {
    int outn = 0;
    for (;;) {
        int picked = 0;
        for (int i = 0; i < nlists; i++) {
            if (lens[i] == 0) continue;
            const char *cand = lists[i][0];
            int rejected = 0;
            for (int j = 0; j < nlists; j++) if (c3_in_tail(lists[j], lens[j], cand)) { rejected = 1; break; }
            if (rejected) continue;
            char cb[64]; strncpy(cb, cand, 63); cb[63] = '\0';
            if (outn < outmax) { strncpy(out[outn], cb, 63); out[outn][63] = '\0'; outn++; }
            for (int j = 0; j < nlists; j++) {
                int w = 0;
                for (int k = 0; k < lens[j]; k++) if (strcmp(lists[j][k], cb) != 0) { if (w != k) strncpy(lists[j][w], lists[j][k], 63); lists[j][w][63] = '\0'; w++; }
                lens[j] = w;
            }
            picked = 1; break;
        }
        if (!picked) break;
    }
    return outn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void compute_mro_multi(DatType *c) {
    static char lists[12][64][64]; static int lens[12]; static char merged[64][64];
    int nlists = 0;
    for (int pi = 0; pi < c->nparents && nlists < 11; pi++) {
        DatType *p = dat_find_type(c->parents[pi]); int L = 0;
        if (p) { for (int k = 0; k < p->mro_len && L < 64; k++) { strncpy(lists[nlists][L], p->mro[k], 63); lists[nlists][L][63] = '\0'; L++; } }
        else  { strncpy(lists[nlists][L], c->parents[pi], 63); lists[nlists][L][63] = '\0'; L++; }
        lens[nlists] = L; nlists++;
    }
    int L = 0; for (int pi = 0; pi < c->nparents && L < 64; pi++) { strncpy(lists[nlists][L], c->parents[pi], 63); lists[nlists][L][63] = '\0'; L++; } lens[nlists] = L; nlists++;
    int mn = c3_merge(lists, lens, nlists, merged, 64);
    c->mro_len = 0;
    strncpy(c->mro[c->mro_len], c->name, 63); c->mro[c->mro_len][63] = '\0'; c->mro_len++;
    for (int k = 0; k < mn && c->mro_len < 64; k++) { strncpy(c->mro[c->mro_len], merged[k], 63); c->mro[c->mro_len][63] = '\0'; c->mro_len++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void class_inherit_multi(const char *child, const char **parents, int nparents) {
    if (!child || nparents <= 0) return;
    DatType *c = dat_find_type(child); if (!c) return;
    if (c->nparents > 0) return;
    char merged[64][64]; DESCR_t mdef[64]; char mhas[64]; char mreq[64]; char mrw[64]; char msig[64]; char mprv[64]; int m = 0;
    for (int pi = 0; pi < nparents; pi++) {
        DatType *p = dat_find_type(parents[pi]); if (!p) continue;
        for (int i = 0; i < p->nfields && m < 63; i++) {
            int dup = 0; for (int j = 0; j < m; j++) if (!strcmp(merged[j], p->fields[i])) { dup = 1; break; }
            if (!dup) {
                strncpy(merged[m], p->fields[i], 63); merged[m][63] = '\0'; mdef[m] = p->defaults[i]; mhas[m] = p->has_default[i]; mreq[m] = p->required[i]; mrw[m] = p->rw[i];
                msig[m] = p->sigil[i]; mprv[m] = p->priv[i]; m++;
            }
        }
    }
    for (int i = 0; i < c->nfields && m < 63; i++) {
        int dup = 0; for (int j = 0; j < m; j++) if (!strcmp(merged[j], c->fields[i])) { dup = 1; break; }
        if (!dup) {
            strncpy(merged[m], c->fields[i], 63); merged[m][63] = '\0'; mdef[m] = c->defaults[i]; mhas[m] = c->has_default[i]; mreq[m] = c->required[i]; mrw[m] = c->rw[i]; msig[m] = c->sigil[i];
            mprv[m] = c->priv[i]; m++;
        }
    }
    for (int i = 0; i < m; i++) {
        strncpy(c->fields[i], merged[i], 63); c->fields[i][63] = '\0'; c->defaults[i] = mdef[i]; c->has_default[i] = mhas[i]; c->required[i] = mreq[i]; c->rw[i] = mrw[i]; c->sigil[i] = msig[i];
        c->priv[i] = mprv[i];
    }
    c->nfields = m;
    c->nparents = 0;
    for (int pi = 0; pi < nparents && c->nparents < 8; pi++) { strncpy(c->parents[c->nparents], parents[pi], 63); c->parents[c->nparents][63] = '\0'; c->nparents++; }
    strncpy(c->parent, parents[0], 63); c->parent[63] = '\0';
    compute_mro_multi(c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void class_compose_role(const char *child, const char *role) {
    if (!child || !role) return;
    DatType *c = dat_find_type(child); if (!c) return;
    DatType *r = dat_find_type(role);  if (!r) return;
    for (int mi = 0; mi < r->nmethods; mi++) {
        const char *m = r->methods[mi];
        int own = 0; for (int k = 0; k < c->nmethods; k++) if (!strcmp(c->methods[k], m)) { own = 1; break; }
        if (own) continue;
        for (int pri = 0; pri < c->nroles; pri++) {
            DatType *rp = dat_find_type(c->roles[pri]); if (!rp) continue;
            int clash = 0; for (int k = 0; k < rp->nmethods; k++) if (!strcmp(rp->methods[k], m)) { clash = 1; break; }
            if (clash) {
                extern void rt_script_die_surface(const char *msg); char _m[256];
                snprintf(_m, sizeof _m, "Method '%s' must be resolved by class %s because it exists in multiple roles (%s, %s)", m, child, c->roles[pri], role); rt_script_die_surface(_m); return;
            }
        }
    }
    for (int i = 0; i < r->nfields && c->nfields < 63; i++) {
        int dup = 0; for (int j = 0; j < c->nfields; j++) if (!strcmp(c->fields[j], r->fields[i])) { dup = 1; break; }
        if (!dup) {
            int k = c->nfields; strncpy(c->fields[k], r->fields[i], 63); c->fields[k][63] = '\0'; c->defaults[k] = r->defaults[i]; c->has_default[k] = r->has_default[i];
            c->required[k] = r->required[i]; c->rw[k] = r->rw[i]; c->sigil[k] = r->sigil[i]; c->priv[k] = r->priv[i]; c->nfields++;
        }
    }
    int dupr = 0; for (int i = 0; i < c->nroles; i++) if (!strcmp(c->roles[i], role)) { dupr = 1; break; }
    if (!dupr && c->nroles < 8) { strncpy(c->roles[c->nroles], role, 63); c->roles[c->nroles][63] = '\0'; c->nroles++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_add_method(const char *type, const char *mname) {
    if (!type || !mname || !*mname) return;
    DatType *t = dat_find_type(type); if (!t) return;
    for (int i = 0; i < t->nmethods; i++) if (!strcmp(t->methods[i], mname)) return;
    if (t->nmethods < 32) { strncpy(t->methods[t->nmethods], mname, 63); t->methods[t->nmethods][63] = '\0'; t->nmethods++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_set_build_key(const char *cls, const char *key) {
    DatType *t = dat_find_type(cls); if (!t) return; t->has_build = 1; if (!key || !*key) return;
    if (t->nbuild_keys < 16) { strncpy(t->build_keys[t->nbuild_keys], key, 63); t->build_keys[t->nbuild_keys][63] = '\0'; t->nbuild_keys++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_has_build_mro(const char *cls) {
    if (!cls) return 0; extern int dat_mro(const char *name, const char **out, int max);
    const char *mro[64]; int mn = dat_mro(cls, mro, 64); if (mn == 0) { mro[0] = cls; mn = 1; }
    for (int mi = 0; mi < mn; mi++) { DatType *t = mro[mi] ? dat_find_type(mro[mi]) : NULL; if (t && t->has_build) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_class_has_build(const char *cls) { DatType *t = cls ? dat_find_type(cls) : NULL; return (t && t->has_build) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_build_keys(const char *cls, const char **out, int max) {
    DatType *t = cls ? dat_find_type(cls) : NULL; if (!t) return 0; int n = 0;
    for (int i = 0; i < t->nbuild_keys && n < max; i++) out[n++] = t->build_keys[i];
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_add_handles(const char *cls, const char *meth, const char *field) {
    DatType *t = dat_find_type(cls); if (!t || !meth || !*meth || !field || !*field) return;
    for (int i = 0; i < t->nhandles; i++) if (!strcmp(t->handles_meth[i], meth)) return;
    if (t->nhandles < 32) {
        strncpy(t->handles_meth[t->nhandles], meth, 63); t->handles_meth[t->nhandles][63] = '\0'; strncpy(t->handles_fld[t->nhandles], field, 63); t->handles_fld[t->nhandles][63] = '\0';
        t->nhandles++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_handles_field(const char *cls, const char *meth, char *out, int outsz) {
    if (!cls || !meth) return 0; extern int dat_mro(const char *name, const char **out, int max);
    const char *mro[64]; int mn = dat_mro(cls, mro, 64); if (mn == 0) { mro[0] = cls; mn = 1; }
    for (int mi = 0; mi < mn; mi++) { DatType *t = mro[mi] ? dat_find_type(mro[mi]) : NULL; if (!t) continue;
        for (int i = 0; i < t->nhandles; i++) if (!strcmp(t->handles_meth[i], meth)) { snprintf(out, outsz, "%s", t->handles_fld[i]); return 1; } }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_type_nhandles(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].nhandles : 0; }
const char *dat_type_handles_meth_at(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nhandles) ? dat_types[i].handles_meth[j] : (const char *)0; }
const char *dat_type_handles_fld_at(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nhandles) ? dat_types[i].handles_fld[j] : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_default_i(const char *cls, const char *field, int64_t v) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->defaults[i] = INTVAL(v); t->has_default[i] = 1; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_default_s(const char *cls, const char *field, const char *v) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->defaults[i] = STRVAL(rt_ws_strdup(v ? v : "")); t->has_default[i] = 1; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_default_r(const char *cls, const char *field, double v) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->defaults[i] = REALVAL(v); t->has_default[i] = 1; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_required(const char *cls, const char *field) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->required[i] = 1; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_rw(const char *cls, const char *field) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->rw[i] = 1; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_sigil(const char *cls, const char *field, int sig) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->sigil[i] = (char)sig; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void dat_set_field_priv(const char *cls, const char *field) {
    DatType *t = dat_find_type(cls); if (!t) return;
    for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) { t->priv[i] = 1; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_field_is_private(const char *cls, const char *field) {
    if (!cls || !field) return 0;
    extern int dat_mro(const char *name, const char **out, int max);
    const char *mro[64]; int mn = dat_mro(cls, mro, 64); if (mn == 0) { mro[0] = cls; mn = 1; }
    for (int mi = 0; mi < mn; mi++) { DatType *t = mro[mi] ? dat_find_type(mro[mi]) : NULL; if (!t) continue;
        for (int i = 0; i < t->nfields; i++) if (strcmp(t->fields[i], field) == 0) return t->priv[i] ? 1 : 0; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_type_field_priv(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].priv[j] : 0; }
int dat_type_field_has_default(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].has_default[j] : 0; }
int dat_type_field_required(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].required[j] : 0; }
int dat_type_field_rw(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].rw[j] : 0; }
int dat_type_field_sigil(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].sigil[j] : 0; }
DESCR_t dat_type_field_default(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].defaults[j] : NULVCL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *dat_parent(const char *name) {
    DatType *t = dat_find_type(name);
    return (t && t->parent[0]) ? t->parent : (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_mro(const char *name, const char **out, int max) {
    DatType *t = dat_find_type(name);
    if (!t) return 0;
    int n = (t->mro_len < max) ? t->mro_len : max;
    for (int i = 0; i < n; i++) out[i] = t->mro[i];
    if (n == 0 && max > 0) { out[0] = t->name; n = 1; }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_roles(const char *name, const char **out, int max) {
    DatType *t = dat_find_type(name);
    if (!t) return 0;
    int n = (t->nroles < max) ? t->nroles : max;
    for (int i = 0; i < n; i++) out[i] = t->roles[i];
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_type_count(void) { return dat_ntypes; }
const char *dat_type_name(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].name : (const char *)0; }
int dat_type_nfields(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].nfields : 0; }
int dat_type_nparents(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].nparents : 0; }
const char *dat_type_parent_at(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nparents) ? dat_types[i].parents[j] : (const char *)0; }
int dat_type_nroles(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].nroles : 0; }
const char *dat_type_role_at(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nroles) ? dat_types[i].roles[j] : (const char *)0; }
int dat_type_nmethods(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].nmethods : 0; }
const char *dat_type_method_at(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nmethods) ? dat_types[i].methods[j] : (const char *)0; }
int dat_type_has_build(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].has_build : 0; }
int dat_type_nbuild_keys(int i) { return (i >= 0 && i < dat_ntypes) ? dat_types[i].nbuild_keys : 0; }
const char *dat_type_build_key_at(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nbuild_keys) ? dat_types[i].build_keys[j] : (const char *)0; }
const char *dat_type_field(int i, int j) { return (i >= 0 && i < dat_ntypes && j >= 0 && j < dat_types[i].nfields) ? dat_types[i].fields[j] : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_methods(const char *name, const char **out, int max) {
    DatType *t = dat_find_type(name); if (!t) return 0; int n = 0; const char *mro[64]; int mn = dat_mro(name, mro, 64);
    if (mn == 0) { mro[0] = name; mn = 1; }
    for (int mi = 0; mi < mn; mi++) { DatType *c = dat_find_type(mro[mi]); if (!c) continue;
        for (int j = 0; j < c->nmethods && n < max; j++) { int dup = 0; for (int k = 0; k < n; k++) if (!strcmp(out[k], c->methods[j])) { dup = 1; break; } if (!dup) out[n++] = c->methods[j]; }
        for (int ri = 0; ri < c->nroles; ri++) { DatType *r = dat_find_type(c->roles[ri]); if (!r) continue;
            for (int j = 0; j < r->nmethods && n < max; j++) {
                int dup = 0; for (int k = 0; k < n; k++) if (!strcmp(out[k], r->methods[j])) { dup = 1; break; } if (!dup) out[n++] = r->methods[j];
            } } }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dat_attributes(const char *name, const char **out, int max) {
    DatType *t = dat_find_type(name); if (!t) return 0; int n = 0; const char *mro[64]; int mn = dat_mro(name, mro, 64);
    if (mn == 0) { mro[0] = name; mn = 1; }
    for (int mi = 0; mi < mn; mi++) { DatType *c = dat_find_type(mro[mi]); if (!c) continue;
        for (int j = 0; j < c->nfields && n < max; j++) { int dup = 0; for (int k = 0; k < n; k++) if (!strcmp(out[k], c->fields[j])) { dup = 1; break; } if (!dup) out[n++] = c->fields[j]; } }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DatType *dat_find_field(const char *name, int *fidx) {
    for (int i = 0; i < dat_ntypes; i++)
        for (int j = 0; j < dat_types[i].nfields; j++)
            if (strcmp(dat_types[i].fields[j], name) == 0) {
                if (fidx) *fidx = j;
                return &dat_types[i];
            }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t dat_alloc_fill(DatType *t, DESCR_t *args, int nargs) {
    DATINST_t *inst = rt_ws_alloc(sizeof(DATINST_t));
    DATBLK_t *blk = rt_ws_alloc(sizeof(DATBLK_t));
    blk->name    = rt_ws_strdup(t->name);
    blk->nfields = t->nfields;
    blk->fields  = rt_ws_alloc(t->nfields * sizeof(char *));
    for (int i = 0; i < t->nfields; i++) blk->fields[i] = rt_ws_strdup(t->fields[i]);
    blk->next    = NULL;
    inst->type   = blk;
    if (t->serial_next < 1) t->serial_next = 1;
    inst->id     = t->serial_next++;
    inst->fields = rt_ws_alloc(t->nfields * sizeof(DESCR_t));
    for (int i = 0; i < t->nfields; i++) {
        inst->fields[i] = (i < nargs) ? args[i] : NULVCL;
        if (t->has_default[i] && inst->fields[i].v == DT_SNUL) inst->fields[i] = t->defaults[i];
    }
    for (int i = 0; i < t->nfields; i++) {
        if ((t->sigil[i] == '@' || t->sigil[i] == '%') && inst->fields[i].v == DT_SNUL && !t->required[i]) inst->fields[i] = STRVAL(rt_ws_strdup(""));
    }
    DESCR_t r; r.v = DT_DATA; r.slen = 0; r.u = inst; return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dat_check_required(DatType *t, DESCR_t self) {
    DATINST_t *inst = (DATINST_t *)self.u; if (!inst) return;
    for (int i = 0; i < t->nfields; i++) {
        if (t->required[i] && inst->fields[i].v == DT_SNUL) {
            extern void rt_script_die_surface(const char *msg);
            char _m[512]; snprintf(_m, sizeof _m, "The attribute '$!%s' is required, but you did not provide a value for it.", t->fields[i]);
            rt_script_die_surface(_m); break;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t dat_construct(DatType *t, DESCR_t *args, int nargs) {
    DESCR_t r = dat_alloc_fill(t, args, nargs);
    dat_check_required(t, r);
    extern void rt_fire_buildplan_tweak(const char *cname, DESCR_t self);
    rt_fire_buildplan_tweak(t->name, r);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_construct_build(DatType *t, DESCR_t *named, int nnamed) {
    DESCR_t self = dat_alloc_fill(t, (DESCR_t *)0, 0);
    extern void rt_fire_build(const char *cname, DESCR_t self, DESCR_t *named, int nnamed);
    rt_fire_build(t->name, self, named, nnamed);
    dat_check_required(t, self);
    extern void rt_fire_buildplan_tweak(const char *cname, DESCR_t self);
    rt_fire_buildplan_tweak(t->name, self);
    return self;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_dat_field_get(const char *fname, DESCR_t obj) {
    DESCR_t *cell = data_field_ptr(fname, obj);
    if (cell) return *cell;
    if (fname && !strcmp(fname, "WHAT")) {
        const char *cn = (const char *)0;
        if (obj.v >= DT_DATA && obj.u) { DATBLK_t *b = obj.u->type; cn = b ? b->name : (const char *)0; }
        else { const char *s = VARVAL_fn(obj); if (s && dat_find_type(s)) cn = s; }
        if (cn) return STRVAL(rt_ws_strdup(cn));
    }
    extern int rt_str_method(const char *meth, DESCR_t recv, const DESCR_t *margs, int nmargs, DESCR_t *out);
    DESCR_t r; if (obj.v != DT_DATA && rt_str_method(fname, obj, (DESCR_t *)0, 0, &r)) return r;
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t dat_field_set(const char *fname, DESCR_t obj, DESCR_t val) {
    DESCR_t *cell = data_field_ptr(fname, obj);
    if (cell) { *cell = val; return val; }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t _builtin_DATA(DESCR_t *args, int nargs) {
    if (nargs < 1) return FAILDESCR;
    const char *raw_spec = VARVAL_fn(args[0]);
    if (!raw_spec || !*raw_spec) return FAILDESCR;
    char *spec = rt_ws_strdup(raw_spec);
    DEFDAT_fn(spec);
    dat_register(spec);
    extern DESCR_t core_DATA_register(DESCR_t *a, int n);
    core_DATA_register(args, nargs);
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *data_field_ptr(const char *fname, DESCR_t inst) {
    if (inst.v < DT_DATA || !inst.u) return NULL;
    DATBLK_t *blk = inst.u->type;
    if (!blk) return NULL;
    for (int i = 0; i < blk->nfields; i++)
        if (blk->fields[i] && strcmp(blk->fields[i], fname) == 0)
            return &inst.u->fields[i];
    return NULL;
}
#include "../runtime/builtins/gen_runtime.h"
