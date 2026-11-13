#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include <math.h>
#include "ir/ast.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_08_pas.h"
#include "lower/lower_pascal_tree/lpt_11_sh.h"
#include "lower/lower_pascal_tree/lpt_12_pas.h"
static void pas_openparm_add(const char *name, unsigned long long m);
static void pas_caparm_add(const char *name, unsigned long long m, const long long *lo);
void *pas_pool_grow(void *p, int *cap, size_t esz, int init) {
    int oc = *cap, nc = oc ? oc * 2 : init;
    p = ct_grow(p, (size_t)nc * esz);
    memset((char *)p + (size_t)oc * esz, 0, (size_t)(nc - oc) * esz);
    *cap = nc;
    return p;
}
static void pas_scope_fwd_save(const char *name, PNodeList *params, const char *sig) {
    if (!name) return;
    if (g_lower_pas.sem.pas_scope.nfwd >= g_lower_pas.sem.pas_scope.fcap) {
        g_lower_pas.sem.pas_scope.fcap = g_lower_pas.sem.pas_scope.fcap ? g_lower_pas.sem.pas_scope.fcap * 2 : 32;
        g_lower_pas.sem.pas_scope.fwd = (PasFwd *)ct_grow(g_lower_pas.sem.pas_scope.fwd, (size_t)g_lower_pas.sem.pas_scope.fcap * sizeof(PasFwd));
    }
    g_lower_pas.sem.pas_scope.fwd[g_lower_pas.sem.pas_scope.nfwd].name = name;
    g_lower_pas.sem.pas_scope.fwd[g_lower_pas.sem.pas_scope.nfwd].sig = sig;
    g_lower_pas.sem.pas_scope.fwd[g_lower_pas.sem.pas_scope.nfwd++].params = params;
}
const PasFwd *pas_scope_fwd_of(const char *name, PNodeList *params) {
    if ((!params || params->count == 0) && name) for (int i = g_lower_pas.sem.pas_scope.nfwd - 1; i >= 0; i--)
        if (g_lower_pas.sem.pas_scope.fwd[i].name && !strcmp(g_lower_pas.sem.pas_scope.fwd[i].name, name)) return &g_lower_pas.sem.pas_scope.fwd[i];
    return NULL;
}
static PNodeList *pas_scope_fwd_params(const char *name, PNodeList *params) { const PasFwd *f = pas_scope_fwd_of(name, params); return f ? f->params : params; }
static void pas_scope_enter(const char *name, PNodeList *params) {
    if (g_lower_pas.sem.pas_scope.depth >= g_lower_pas.sem.pas_scope.mcap) {
        g_lower_pas.sem.pas_scope.mcap = g_lower_pas.sem.pas_scope.mcap ? g_lower_pas.sem.pas_scope.mcap * 2 : 16;
        g_lower_pas.sem.pas_scope.marks = (int *)ct_grow(g_lower_pas.sem.pas_scope.marks, (size_t)g_lower_pas.sem.pas_scope.mcap * sizeof(int));
    }
    g_lower_pas.sem.pas_scope.marks[g_lower_pas.sem.pas_scope.depth++] = g_lower_pas.sem.pas_scope.n;
    const PasFwd *fwd = pas_scope_fwd_of(name, params);
    if (fwd) params = fwd->params;
    int first = g_lower_pas.sem.pas_scope.n;
    pas_scope_define_list(params);
    for (int i = first; name && i < g_lower_pas.sem.pas_scope.n; i++) for (int j = g_lower_pas.sem.pas_scope.nfsig - 1; j >= 0; j--)
        if (!strcmp(g_lower_pas.sem.pas_scope.fsig[j].owner, name) && !strcmp(g_lower_pas.sem.pas_scope.fsig[j].name, g_lower_pas.sem.pas_scope.defs[i].name)) {
        g_lower_pas.sem.pas_scope.defs[i].sig = g_lower_pas.sem.pas_scope.fsig[j].sig;
        g_lower_pas.sem.pas_scope.defs[i].formal = 1;
        break;
    }
    if (first > 0 && g_lower_pas.sem.pas_scope.defs[first - 1].rid && name && !strcmp(g_lower_pas.sem.pas_scope.defs[first - 1].name, name)) {
        const char *sig = (fwd && fwd->sig) ? fwd->sig : g_lower_pas.sem.pas_scope.defs[first - 1].sig;
        pas_formal_subranges(sig, params);
        pas_formal_chararrs(sig, params);
        pas_formal_strtypes(sig, params);
        pas_formal_tfiles(sig, params);
    }
}
static const char *pas_pf_rtype(const tree_t *e) {
    if (!e || e->t != TT_FNC || e->n < 4 || !e->c[0] || !e->c[0]->v.sval || strcmp(e->c[0]->v.sval, "__pas_pcall") || !e->c[2]->v.sval || e->c[2]->v.sval[0] != 'F') return NULL;
    const char *r = strrchr(e->c[2]->v.sval, ')');
    return (r && r[1] == ':') ? r + 2 : NULL;
}
static void pas_scope_exit(void) {
    if (g_lower_pas.sem.pas_scope.depth > 0) g_lower_pas.sem.pas_scope.n = g_lower_pas.sem.pas_scope.marks[--g_lower_pas.sem.pas_scope.depth];
    while (g_lower_pas.sem.pas_scope.napl > 0 && g_lower_pas.sem.pas_scope.apl[g_lower_pas.sem.pas_scope.napl - 1].depth > g_lower_pas.sem.pas_scope.depth) g_lower_pas.sem.pas_scope.napl--;
}
tree_t *mk_proc(const char *name, PNodeList *params, tree_t *body_stmt, int is_function, int decl_level, const char **lnames, int lcount) {
    tree_t *body_prog = ast_node_new(TT_PROGRAM);
    for (int _li = 0; lnames && _li < lcount; _li++) {
        long long _hi;
        if (lnames[_li] && pas_array_high_get(lnames[_li], &_hi)) ast_push(body_prog, bin(TT_ASSIGN, leaf_s(TT_VAR, lnames[_li]), mk_array_init(lnames[_li], _hi)));
        else if (lnames[_li] && pas_array_isdyn(lnames[_li])) ast_push(body_prog, bin(TT_ASSIGN, leaf_s(TT_VAR, lnames[_li]), mk_array_init(lnames[_li], -1)));
    }
    if (body_stmt && body_stmt->t == TT_PROGRAM) { for (int i = 0; i < body_stmt->n; i++) ast_push(body_prog, body_stmt->c[i]); } else if (body_stmt) { ast_push(body_prog, body_stmt); }
    tree_t *proc = ast_node_new(TT_PROC_DECL);
    proc->v.sval = (char *)name;
    ast_push(proc, leaf_s(TT_VAR, name));
    tree_t *vlist = ast_node_new(TT_VLIST);
    long long byref = 0;
    unsigned long long camask = 0;
    unsigned long long opmask = 0;
    long long calo[64];
    for (int _z = 0; _z < 64; _z++) calo[_z] = 0;
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *pv = params->items[i];
        if (pv && pv->n > 0) { int _isvar = 0; for (int _c = 0; _c < pv->n; _c++) if (pv->c[_c] && pv->c[_c]->t == TT_SUCCEED) _isvar = 1; if (_isvar && i < 64) byref |= (1LL << i); pv->n = 0; }
        if (pv && pv->v.sval && i < 64 && pas_is_chararr(pv->v.sval)) { camask |= (1ULL << i); calo[i] = pas_chararr_lo(pv->v.sval); }
        if (pv && pv->v.sval && i < 64 && pas_array_isopen(pv->v.sval)) opmask |= (1ULL << i);
        ast_push(vlist, pv);
    }
    if (camask) pas_caparm_add(name, camask, calo);
    if (opmask) pas_openparm_add(name, opmask);
    vlist->v.ival = byref;
    ast_push(proc, vlist);
    ast_push(proc, body_prog);
    if (is_function) ast_push(proc, leaf_s(TT_VAR, name));
    tree_t *locals = ast_node_new(TT_VLIST);
    locals->v.ival = decl_level;
    for (int i = 0; i < lcount; i++) if (lnames[i]) ast_push(locals, leaf_s(TT_VAR, lnames[i]));
    ast_push(proc, locals);
    return proc;
}
static void pas_func_add(const char *name) {
    if (name) {
        if (g_lower_pas.sem.pas_nfunc >= g_lower_pas.sem.pas_funcs_cap) g_lower_pas.sem.pas_funcs =
            pas_pool_grow(g_lower_pas.sem.pas_funcs, &g_lower_pas.sem.pas_funcs_cap, sizeof *g_lower_pas.sem.pas_funcs, 16);
        g_lower_pas.sem.pas_funcs[g_lower_pas.sem.pas_nfunc].name = ct_strdup(name);
        g_lower_pas.sem.pas_funcs[g_lower_pas.sem.pas_nfunc].nvp = 0;
        g_lower_pas.sem.pas_funcs[g_lower_pas.sem.pas_nfunc].nrp = 0;
        g_lower_pas.sem.pas_funcs[g_lower_pas.sem.pas_nfunc].nsp = 0;
        g_lower_pas.sem.pas_nfunc++;
    }
}
static void pas_caparm_add(const char *name, unsigned long long m, const long long *lo) {
    if (name && m) {
        if (g_lower_pas.sem.pas_ncaparm >= g_lower_pas.sem.pas_caparm_cap) g_lower_pas.sem.pas_caparm =
            pas_pool_grow(g_lower_pas.sem.pas_caparm, &g_lower_pas.sem.pas_caparm_cap, sizeof *g_lower_pas.sem.pas_caparm, 32);
        g_lower_pas.sem.pas_caparm[g_lower_pas.sem.pas_ncaparm].name = ct_strdup(name);
        g_lower_pas.sem.pas_caparm[g_lower_pas.sem.pas_ncaparm].camask = m;
        for (int i = 0; i < 64; i++) g_lower_pas.sem.pas_caparm[g_lower_pas.sem.pas_ncaparm].lo[i] = lo ? lo[i] : 0;
        g_lower_pas.sem.pas_ncaparm++;
    }
}
unsigned long long pas_caparm_mask(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_ncaparm; i++) if (g_lower_pas.sem.pas_caparm[i].name && !strcmp(g_lower_pas.sem.pas_caparm[i].name, name)) return g_lower_pas.sem.pas_caparm[i].camask;
    return 0;
}
long long pas_caparm_lo(const char *name, int pos) {
    if (!name || pos < 0 || pos >= 64) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_ncaparm; i++) if (g_lower_pas.sem.pas_caparm[i].name && !strcmp(g_lower_pas.sem.pas_caparm[i].name, name)) return g_lower_pas.sem.pas_caparm[i].lo[pos];
    return 0;
}
void pas_const_add(const char *name, long long v) {
    if (name) {
        if (g_lower_pas.sem.pas_nconst >= g_lower_pas.sem.pas_consts_cap) g_lower_pas.sem.pas_consts =
            pas_pool_grow(g_lower_pas.sem.pas_consts, &g_lower_pas.sem.pas_consts_cap, sizeof *g_lower_pas.sem.pas_consts, 16);
        g_lower_pas.sem.pas_consts[g_lower_pas.sem.pas_nconst].name = ct_strdup(name);
        g_lower_pas.sem.pas_consts[g_lower_pas.sem.pas_nconst].val = v;
        g_lower_pas.sem.pas_consts[g_lower_pas.sem.pas_nconst].isenum = 0;
        g_lower_pas.sem.pas_consts[g_lower_pas.sem.pas_nconst].uid = pas_scope_uid(name);
        g_lower_pas.sem.pas_nconst++;
    }
}
static void pas_rconst_add(const char *name, double v) {
    if (name) {
        if (g_lower_pas.sem.pas_nrconst >= g_lower_pas.sem.pas_rconsts_cap) g_lower_pas.sem.pas_rconsts =
            pas_pool_grow(g_lower_pas.sem.pas_rconsts, &g_lower_pas.sem.pas_rconsts_cap, sizeof *g_lower_pas.sem.pas_rconsts, 16);
        g_lower_pas.sem.pas_rconsts[g_lower_pas.sem.pas_nrconst].name = ct_strdup(name);
        g_lower_pas.sem.pas_rconsts[g_lower_pas.sem.pas_nrconst].val = v;
        g_lower_pas.sem.pas_nrconst++;
    }
}
int pas_rconst_get(const char *name, double *out) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nrconst; i++) if (g_lower_pas.sem.pas_rconsts[i].name && !strcmp(g_lower_pas.sem.pas_rconsts[i].name, name)) {
        *out = g_lower_pas.sem.pas_rconsts[i].val;
        return 1;
    }
    return 0;
}
static void pas_sconst_add(const char *name, const char *v) {
    if (name && v) {
        if (g_lower_pas.sem.pas_nsconst >= g_lower_pas.sem.pas_sconsts_cap) g_lower_pas.sem.pas_sconsts =
            pas_pool_grow(g_lower_pas.sem.pas_sconsts, &g_lower_pas.sem.pas_sconsts_cap, sizeof *g_lower_pas.sem.pas_sconsts, 16);
        g_lower_pas.sem.pas_sconsts[g_lower_pas.sem.pas_nsconst].name = ct_strdup(name);
        g_lower_pas.sem.pas_sconsts[g_lower_pas.sem.pas_nsconst].val = ct_strdup(v);
        g_lower_pas.sem.pas_nsconst++;
    }
}
const char *pas_sconst_get(const char *name) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nsconst; i++) if (g_lower_pas.sem.pas_sconsts[i].name && !strcmp(g_lower_pas.sem.pas_sconsts[i].name, name)) return g_lower_pas.sem.pas_sconsts[i].val;
    return 0;
}
void pas_assigned_add(const char *n) {
    if (!n) return;
    for (int i = 0; i < g_lower_pas.sem.pas_nassigned; i++) if (g_lower_pas.sem.pas_assigned[i].name && !strcmp(g_lower_pas.sem.pas_assigned[i].name, n)) return;
    if (g_lower_pas.sem.pas_nassigned >= g_lower_pas.sem.pas_assigned_cap) g_lower_pas.sem.pas_assigned =
        pas_pool_grow(g_lower_pas.sem.pas_assigned, &g_lower_pas.sem.pas_assigned_cap, sizeof *g_lower_pas.sem.pas_assigned, 64);
    g_lower_pas.sem.pas_assigned[g_lower_pas.sem.pas_nassigned++].name = ct_strdup(n);
}
int pas_assigned_get(const char *n) {
    if (!n) return 0;
    for (int i = 0; i < g_lower_pas.sem.pas_nassigned; i++) if (g_lower_pas.sem.pas_assigned[i].name && !strcmp(g_lower_pas.sem.pas_assigned[i].name, n)) return 1;
    return 0;
}
const char *pas_selector_base_name(tree_t *e) {
    while (e) {
        if (e->t == TT_VAR) return e->v.sval;
        if ((e->t == TT_IDX || e->t == TT_FIELD) && e->n >= 1 && e->c[0]) { e = e->c[0]; continue; }
        if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref") && e->c[1]) { e = e->c[1]; continue; }
        break;
    }
    return NULL;
}
static void pas_openparm_add(const char *name, unsigned long long m) {
    if (!name || !m) return;
    if (g_lower_pas.sem.pas_nopenparm >= g_lower_pas.sem.pas_openparm_cap) {
        g_lower_pas.sem.pas_openparm_cap = g_lower_pas.sem.pas_openparm_cap ? g_lower_pas.sem.pas_openparm_cap * 2 : 16;
        g_lower_pas.sem.pas_openparms = ct_grow(g_lower_pas.sem.pas_openparms, (size_t)g_lower_pas.sem.pas_openparm_cap * sizeof *g_lower_pas.sem.pas_openparms);
    }
    g_lower_pas.sem.pas_openparms[g_lower_pas.sem.pas_nopenparm].name = ct_strdup(name);
    g_lower_pas.sem.pas_openparms[g_lower_pas.sem.pas_nopenparm].mask = m;
    g_lower_pas.sem.pas_nopenparm++;
}
unsigned long long pas_openparm_mask(const char *name) {
    for (int i = g_lower_pas.sem.pas_nopenparm - 1; name && i >= 0; i--) if (!strcmp(g_lower_pas.sem.pas_openparms[i].name, name)) return g_lower_pas.sem.pas_openparms[i].mask;
    return 0;
}
int pas_arrtype_wrap(const char *name) { return pty_tn_wrap(name); }
void pas_enumtype_add(const char *n, long long h) {
    if (n) {
        if (g_lower_pas.sem.pas_nenum >= g_lower_pas.sem.pas_enumtypes_cap) g_lower_pas.sem.pas_enumtypes =
            pas_pool_grow(g_lower_pas.sem.pas_enumtypes, &g_lower_pas.sem.pas_enumtypes_cap, sizeof *g_lower_pas.sem.pas_enumtypes, 16);
        g_lower_pas.sem.pas_enumtypes[g_lower_pas.sem.pas_nenum].name = ct_strdup(n);
        g_lower_pas.sem.pas_enumtypes[g_lower_pas.sem.pas_nenum].high = h;
        g_lower_pas.sem.pas_enumtypes[g_lower_pas.sem.pas_nenum].min_size = g_lower_pas.sem.pas_min_enum_size;
        g_lower_pas.sem.pas_nenum++;
    }
}
void pas_enumnames_add(const char *tn, const char *names) {
    if (tn && names && names[0]) {
        if (g_lower_pas.sem.pas_nenumname >= g_lower_pas.sem.pas_enumnames_cap) g_lower_pas.sem.pas_enumnames =
            pas_pool_grow(g_lower_pas.sem.pas_enumnames, &g_lower_pas.sem.pas_enumnames_cap, sizeof *g_lower_pas.sem.pas_enumnames, 16);
        g_lower_pas.sem.pas_enumnames[g_lower_pas.sem.pas_nenumname].tname = ct_strdup(tn);
        g_lower_pas.sem.pas_enumnames[g_lower_pas.sem.pas_nenumname].names = ct_strdup(names);
        g_lower_pas.sem.pas_nenumname++;
    }
}
void pas_typealias_add(const char *n, const char *t) {
    if (n && t && strcmp(n, t)) {
        if (g_lower_pas.sem.pas_ntypealias >= g_lower_pas.sem.pas_typealias_cap) g_lower_pas.sem.pas_typealias =
            pas_pool_grow(g_lower_pas.sem.pas_typealias, &g_lower_pas.sem.pas_typealias_cap, sizeof *g_lower_pas.sem.pas_typealias, 16);
        g_lower_pas.sem.pas_typealias[g_lower_pas.sem.pas_ntypealias].name = ct_strdup(n);
        g_lower_pas.sem.pas_typealias[g_lower_pas.sem.pas_ntypealias].target = ct_strdup(t);
        g_lower_pas.sem.pas_ntypealias++;
    }
}
void pas_ptrtype_add(const char *p, const char *r) {
    if (p && r) {
        if (g_lower_pas.sem.pas_nptrtype >= g_lower_pas.sem.pas_ptrtypes_cap) g_lower_pas.sem.pas_ptrtypes =
            pas_pool_grow(g_lower_pas.sem.pas_ptrtypes, &g_lower_pas.sem.pas_ptrtypes_cap, sizeof *g_lower_pas.sem.pas_ptrtypes, 16);
        int k = g_lower_pas.sem.pas_nptrtype++;
        g_lower_pas.sem.pas_ptrtypes[k].pname = ct_strdup(p);
        g_lower_pas.sem.pas_ptrtypes[k].rname = ct_strdup(r);
    }
}
const char *pas_ptrvar_target(const char *v) { const pty_t *t = v ? pty_name_type(v) : NULL, *b = pty_is_pchar(t) ? NULL : pty_pointee(t); return b ? pty_name_of(b) : NULL; }
static void pas_proc_enter(void) {
    if (g_lower_pas.sem.pas_ldepth >= g_lower_pas.sem.pas_lstk_cap) {
        int oc = g_lower_pas.sem.pas_lstk_cap;
        g_lower_pas.sem.pas_lstk_cap = oc ? oc * 2 : 16;
        g_lower_pas.sem.pas_lstk = ct_grow(g_lower_pas.sem.pas_lstk, (size_t)g_lower_pas.sem.pas_lstk_cap * sizeof *g_lower_pas.sem.pas_lstk);
        memset(g_lower_pas.sem.pas_lstk + oc, 0, (size_t)(g_lower_pas.sem.pas_lstk_cap - oc) * sizeof *g_lower_pas.sem.pas_lstk);
    }
    g_lower_pas.sem.pas_lstk[g_lower_pas.sem.pas_ldepth].n = 0;
    g_lower_pas.sem.pas_lstk[g_lower_pas.sem.pas_ldepth].decl_level = g_lower_pas.sem.pas_level;
    g_lower_pas.sem.pas_lstk[g_lower_pas.sem.pas_ldepth].func_name = NULL;
    g_lower_pas.sem.pas_ldepth++;
    g_lower_pas.sem.pas_level++;
}
static void pas_proc_exit(void) { if (g_lower_pas.sem.pas_ldepth > 0) g_lower_pas.sem.pas_ldepth--; if (g_lower_pas.sem.pas_level > 1) g_lower_pas.sem.pas_level--; }
void pas_local_add(const char *name) {
    if (g_lower_pas.sem.pas_level < 2 || g_lower_pas.sem.pas_ldepth == 0 || !name) return;
    int d = g_lower_pas.sem.pas_ldepth - 1;
    if (g_lower_pas.sem.pas_lstk[d].n >= g_lower_pas.sem.pas_lstk[d].cap) {
        g_lower_pas.sem.pas_lstk[d].cap = g_lower_pas.sem.pas_lstk[d].cap ? g_lower_pas.sem.pas_lstk[d].cap * 2 : 64;
        g_lower_pas.sem.pas_lstk[d].names = ct_grow(g_lower_pas.sem.pas_lstk[d].names, (size_t)g_lower_pas.sem.pas_lstk[d].cap * sizeof *g_lower_pas.sem.pas_lstk[d].names);
    }
    g_lower_pas.sem.pas_lstk[d].names[g_lower_pas.sem.pas_lstk[d].n++] = ct_strdup(name);
}
static void pas_curfunc_push(const char *fname) { if (g_lower_pas.sem.pas_ldepth > 0) g_lower_pas.sem.pas_lstk[g_lower_pas.sem.pas_ldepth - 1].func_name = fname; }
const char *pas_curfunc_name(void) { return (g_lower_pas.sem.pas_ldepth > 0) ? g_lower_pas.sem.pas_lstk[g_lower_pas.sem.pas_ldepth - 1].func_name : NULL; }
void pas_tfiletype_add(const char *name) {
    if (!name) return;
    if (g_lower_pas.sem.pas_ntfiletype >= g_lower_pas.sem.pas_tfiletypes_cap) g_lower_pas.sem.pas_tfiletypes =
        pas_pool_grow(g_lower_pas.sem.pas_tfiletypes, &g_lower_pas.sem.pas_tfiletypes_cap, sizeof *g_lower_pas.sem.pas_tfiletypes, 16);
    g_lower_pas.sem.pas_tfiletypes[g_lower_pas.sem.pas_ntfiletype++].name = ct_strdup(name);
}
void pas_booltype_add(const char *name) {
    if (!name) return;
    if (g_lower_pas.sem.pas_nbooltype >= g_lower_pas.sem.pas_booltypes_cap) g_lower_pas.sem.pas_booltypes =
        pas_pool_grow(g_lower_pas.sem.pas_booltypes, &g_lower_pas.sem.pas_booltypes_cap, sizeof *g_lower_pas.sem.pas_booltypes, 16);
    g_lower_pas.sem.pas_booltypes[g_lower_pas.sem.pas_nbooltype++].name = ct_strdup(name);
}
void pas_strarr_add2(const char *name, long long lo) {
    if (name) {
        if (g_lower_pas.sem.pas_nstrarr >= g_lower_pas.sem.pas_strarrs_cap) g_lower_pas.sem.pas_strarrs =
            pas_pool_grow(g_lower_pas.sem.pas_strarrs, &g_lower_pas.sem.pas_strarrs_cap, sizeof *g_lower_pas.sem.pas_strarrs, 16);
        g_lower_pas.sem.pas_strarrs[g_lower_pas.sem.pas_nstrarr].name = ct_strdup(name);
        g_lower_pas.sem.pas_strarrs[g_lower_pas.sem.pas_nstrarr].lo = lo;
        g_lower_pas.sem.pas_nstrarr++;
    }
}
static void pas_strarr_add(const char *name) { pas_strarr_add2(name, 1); }
static void pas_case_push(void) {
    if (g_lower_pas.sem.pas_case_depth >= g_lower_pas.sem.pas_case_cap) {
        g_lower_pas.sem.pas_case_cap = g_lower_pas.sem.pas_case_cap ? g_lower_pas.sem.pas_case_cap * 2 : 8;
        g_lower_pas.sem.pas_case_tmp = ct_grow(g_lower_pas.sem.pas_case_tmp, (size_t)g_lower_pas.sem.pas_case_cap * sizeof *g_lower_pas.sem.pas_case_tmp);
    }
    char nm[24];
    snprintf(nm, sizeof nm, "__pct%d", g_lower_pas.sem.pas_case_ctr++);
    g_lower_pas.sem.pas_case_tmp[g_lower_pas.sem.pas_case_depth++] = ct_strdup(nm);
}
static const char *pas_case_cur(void) { int d = g_lower_pas.sem.pas_case_depth - 1; if (d < 0) d = 0; return ct_strdup(g_lower_pas.sem.pas_case_cap > 0 ? g_lower_pas.sem.pas_case_tmp[d] : ""); }
static void pas_case_pop(void) { if (g_lower_pas.sem.pas_case_depth > 0) g_lower_pas.sem.pas_case_depth--; }
pval E_case_statement(const tree_t *n) {
    pval v[9];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_CASE)) {
        const tree_t *els = n->n > 1 && IS(n->c[n->n - 1], TT_PART) && n->c[n->n - 1]->n > 0 ? n->c[n->n - 1] : NULL;
        v[2] = E_expression(n->c[0]);
        { pas_case_push(); }
        v[5] = E_case_list(n, 1, els ? n->n - 1 : n->n);
        {
            tree_t *seq = ast_node_new(TT_SEQ_EXPR);
            ast_push(seq, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_case_cur()), v[2].node));
            tree_t *chain = els ? E_compound_statement(els->c[0]).node : ast_node_new(g_lower_pas.sem.pas_dialect_fpc ? TT_SUCCEED : TT_FNC);
            if (!els && !g_lower_pas.sem.pas_dialect_fpc) {
                ast_push(chain, leaf_s(TT_VAR, "__pas_rterr"));
                ast_push(chain, leaf_s(TT_QLIT, "6.8.3.5"));
                ast_push(chain, leaf_s(TT_QLIT, "no case-constant of the case-statement is equal to the value of its case-index"));
            }
            if (v[5].list) for (int i = v[5].list->count - 1; i >= 0; i--) { tree_t *e = v[5].list->items[i]; if (!e) continue; ast_push(e, chain); chain = e; }
            ast_push(seq, chain);
            pas_case_pop();
            out.node = seq;
        }
        return out;
    }
    elab_bad("case_statement", n);
    return out;
}
pval E_compound_statement(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_SEQ_EXPR)) { v[2] = E_statement_list(n, 0, n->n); { out.node = seq_of(v[2].list); } return out; }
    elab_bad("compound_statement", n);
    return out;
}
static int pas_const_fold_fn(const char *s) {
    static const char *const N[] = { "high", "low", "trunc", "round", "ln", "sqrt", "exp" };
    for (size_t i = 0; s && i < sizeof N / sizeof N[0]; i++) if (!strcmp(s, N[i])) return 1;
    return 0;
}
static int pas_intcast_bits(const char *f, int *w, int *sg) {
    static const char *const T[] = { "byte", "shortint", "smallint", "word", "longint", "longword", "cardinal", "int64", "qword", "uint64", "integer" };
    static const int W[] = { 8, 8, 16, 16, 32, 32, 32, 64, 64, 64, 32 };
    static const int S[] = { 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1 };
    for (size_t j = 0; j < sizeof T / sizeof T[0]; j++) if (!strcmp(f, T[j])) { *w = W[j]; *sg = S[j]; if (!strcmp(f, "integer")) *w = g_lower_pas.sem.pas_int16 ? 16 : 32; return 1; }
    return 0;
}
static long long pas_intcast_wrap(long long v, int w, int sg) {
    if (w >= 64) return v;
    unsigned long long u = (unsigned long long)v & ((1ULL << w) - 1);
    if (sg && ((u >> (w - 1)) & 1ULL)) return (long long)u - (1LL << w);
    return (long long)u;
}
static int pas_fold_const(const tree_t *e, int *k, long long *iv, double *dv, const char **sv) {
    if (!e) return 0;
    if (e->t == TT_ILIT) { *k = 1; *iv = e->v.ival; return 1; }
    if (e->t == TT_FLIT) { *k = 2; *dv = e->v.dval; return 1; }
    if (e->t == TT_CHRLIT) { *k = 4; *iv = e->v.ival; return 1; }
    if (e->t == TT_QLIT) { if (e->v.sval && strlen(e->v.sval) == 1) { *k = 4; *iv = (unsigned char)e->v.sval[0]; } else { *k = 3; *sv = e->v.sval ? e->v.sval : ""; } return 1; }
    if (e->t == TT_VAR && e->v.sval) {
        const char *s = pas_sconst_get(e->v.sval);
        if (s) { *k = 3; *sv = s; return 1; }
        if (pas_rconst_get(e->v.sval, dv)) { *k = 2; return 1; }
        if (!pas_const_isenum(e->v.sval) && pas_const_get(e->v.sval, iv)) { *k = pas_is_charvar(e->v.sval) ? 4 : 1; return 1; }
        return 0;
    }
    if ((e->t == TT_PLS || e->t == TT_MNS) && e->n == 1) {
        if (!pas_fold_const(e->c[0], k, iv, dv, sv) || *k > 2) return 0;
        if (e->t == TT_MNS) { if (*k == 1) *iv = -*iv; else *dv = -*dv; }
        return 1;
    }
    if (e->n == 2 && (e->t == TT_ADD || e->t == TT_SUB || e->t == TT_MUL || e->t == TT_DIV || e->t == TT_IDIV || e->t == TT_MOD)) {
        int ka = 0, kb = 0;
        long long ia = 0, ib = 0;
        double da = 0, db = 0;
        const char *sa = NULL, *sb = NULL;
        if (!pas_fold_const(e->c[0], &ka, &ia, &da, &sa) || !pas_fold_const(e->c[1], &kb, &ib, &db, &sb)) return 0;
        if (ka >= 3 || kb >= 3) {
            if (e->t != TT_ADD || ka < 3 || kb < 3) return 0;
            size_t la = ka == 4 ? 1 : strlen(sa), lb = kb == 4 ? 1 : strlen(sb);
            char *o = (char *)ct_zalloc(la + lb + 1, 1);
            if (ka == 4) o[0] = (char)ia;
            else memcpy(o, sa, la);
            if (kb == 4) o[la] = (char)ib;
            else memcpy(o + la, sb, lb);
            *k = 3;
            *sv = o;
            return 1;
        }
        if (ka == 1 && kb == 1 && e->t != TT_DIV) {
            if ((e->t == TT_IDIV || e->t == TT_MOD) && ib == 0) return 0;
            *k = 1;
            *iv = e->t == TT_ADD ? ia + ib : e->t == TT_SUB ? ia - ib : e->t == TT_MUL ? ia * ib : e->t == TT_IDIV ? ia / ib : ia % ib;
            return 1;
        }
        if (e->t == TT_IDIV || e->t == TT_MOD) return 0;
        double xa = ka == 1 ? (double)ia : da, xb = kb == 1 ? (double)ib : db;
        if (e->t == TT_DIV && xb == 0) return 0;
        *k = 2;
        *dv = e->t == TT_ADD ? xa + xb : e->t == TT_SUB ? xa - xb : e->t == TT_MUL ? xa * xb : xa / xb;
        return 1;
    }
    if (e->t == TT_FNC && e->n == 3 && e->c[0] && e->c[0]->v.sval && (!strcmp(e->c[0]->v.sval, "shl") || !strcmp(e->c[0]->v.sval, "shr") || !strcmp(e->c[0]->v.sval, "xor"))) {
        int ka = 0, kb = 0;
        long long ia = 0, ib = 0;
        double da = 0, db = 0;
        const char *sa = NULL, *sb = NULL;
        if (!pas_fold_const(e->c[1], &ka, &ia, &da, &sa) || !pas_fold_const(e->c[2], &kb, &ib, &db, &sb) || ka != 1 || kb != 1) return 0;
        *k = 1;
        *iv = !strcmp(e->c[0]->v.sval, "xor") ? (ia ^ ib) : !strcmp(e->c[0]->v.sval, "shl") ? (long long)((unsigned long long)ia << (ib & 63)) : (long long)((unsigned long long)ia >> (ib & 63));
        return 1;
    }
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->v.sval && e->c[1] && !pas_scope_uid(e->c[0]->v.sval)) {
        int w = 0, sg = 0;
        if (pas_intcast_bits(e->c[0]->v.sval, &w, &sg) && pas_fold_const(e->c[1], k, iv, dv, sv) && (*k == 1 || *k == 4)) { *k = 1; *iv = pas_intcast_wrap(*iv, w, sg); return 1; }
    }
    if (e->t == TT_FNC && e->n == 2 && e->c[0] && e->c[0]->v.sval && e->c[1] && !pas_is_func(e->c[0]->v.sval) && !pas_is_proc(e->c[0]->v.sval)) {
        const char *f = e->c[0]->v.sval, *a = e->c[1]->t == TT_VAR ? e->c[1]->v.sval : NULL;
        if (!strcmp(f, "sizeof") && a) return pas_sizeof_lookup(a, iv) && *iv >= 0 ? (*k = 1, 1) : 0;
        if ((!strcmp(f, "high") || !strcmp(f, "low")) && a) {
            long long h = pas_subtype_high(a), eh = pas_enumtype_high(a);
            if (h >= 0) { *k = 1; *iv = f[0] == 'h' ? h : pas_subtype_low(a); return 1; }
            if (eh >= 0) { *k = 1; *iv = f[0] == 'h' ? eh : 0; g_lower_pas.sem.pas_last_const_enum = 1; return 1; }
            static const char *const P[] = { "byte", "shortint", "smallint", "word", "longint", "longword", "cardinal", "int64", "char", "boolean" };
            static const long long PL[] = { 0, -128, -32768, 0, -2147483647LL - 1, 0, 0, INT64_MIN, 0, 0 };
            static const long long PH[] = { 255, 127, 32767, 65535, 2147483647LL, 4294967295LL, 4294967295LL, INT64_MAX, 255, 1 };
            for (int j = 0; j < 10; j++) if (!strcmp(a, P[j]) && !pas_scope_uid(a)) { *k = j == 8 ? 4 : 1; *iv = f[0] == 'h' ? PH[j] : PL[j]; return 1; }
            long long ah;
            if (pas_array_high_get(a, &ah) && pas_array_ncols(a) < 0) { *k = 1; *iv = f[0] == 'h' ? ah : pas_array_nonparam_low(a); return 1; }
            if (!pas_arrtype_isdyn(a) && pas_arrtype_high(a) >= 0 && pas_arrtype_ncols(a) < 0) { *k = 1; *iv = f[0] == 'h' ? pas_arrtype_high(a) : pas_arrtype_lo(a); return 1; }
            return 0;
        }
        if (!strcmp(f, "trunc") || !strcmp(f, "round") || !strcmp(f, "ln") || !strcmp(f, "sqrt") || !strcmp(f, "exp")) {
            int ka = 0;
            long long ia = 0;
            double da = 0;
            const char *sa = NULL;
            if (!pas_fold_const(e->c[1], &ka, &ia, &da, &sa) || (ka != 1 && ka != 2)) return 0;
            double x = ka == 1 ? (double)ia : da;
            if (f[0] == 't' || f[0] == 'r') {
                double y = f[0] == 't' ? trunc(x) : g_lower_pas.sem.pas_dialect_fpc ? rint(x) : round(x);
                if (!(y > -9223372036854775809.0 && y < 9223372036854775808.0)) return 0;
                *k = 1;
                *iv = (long long)y;
                return 1;
            }
            double y = f[0] == 'l' ? (x > 0 ? log(x) : NAN) : f[0] == 's' ? (x >= 0 ? sqrt(x) : NAN) : exp(x);
            if (!isfinite(y)) return 0;
            *k = 2;
            *dv = y;
            return 1;
        }
        static const char *const F[] = { "ord", "chr", "succ", "pred", "abs", "odd" };
        int fi = -1;
        for (int j = 0; j < 6; j++) if (!strcmp(f, F[j])) fi = j;
        int en = a && pas_const_isenum(a) && pas_const_get(a, iv);
        if (fi < 0 || (en ? (*k = 1, 0) : (!pas_fold_const(e->c[1], k, iv, dv, sv) || (*k != 1 && *k != 4)))) return 0;
        g_lower_pas.sem.pas_last_const_enum = en && (fi == 2 || fi == 3);
        if (fi == 0) *k = 1;
        else if (fi == 1) *k = 4;
        else if (fi == 2) *iv += 1;
        else if (fi == 3) *iv -= 1;
        else if (fi == 5) { *k = 1; *iv = (*iv % 2) != 0; } else if (*iv < 0) *iv = -*iv;
        return 1;
    }
    return 0;
}
pval E_const_decl(const tree_t *n) {
    pval v[8];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_FLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[3].dval = tok_d(n->c[1]);
        { pas_scope_define(v[1].str); pas_rconst_add(v[1].str, v[3].dval); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_PLS) && IS(n->c[1]->c[0], TT_FLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[4].dval = tok_d(n->c[1]->c[0]);
        { pas_scope_define(v[1].str); pas_rconst_add(v[1].str, v[4].dval); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_MNS) && IS(n->c[1]->c[0], TT_FLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[4].dval = tok_d(n->c[1]->c[0]);
        { pas_scope_define(v[1].str); pas_rconst_add(v[1].str, -v[4].dval); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_QLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[3].str = tok_s(n->c[1]);
        { pas_scope_define(v[1].str); if (v[3].str && strlen(v[3].str)==1) { pas_const_add(v[1].str,(long long)(unsigned char)v[3].str[0]); } else pas_sconst_add(v[1].str,v[3].str); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_CHRLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[3].ival = tok_i(n->c[1]);
        { pas_scope_define(v[1].str); pas_const_add(v[1].str, v[3].ival); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && IS(n->c[1], TT_FNC) && n->c[1]->n == 2 && n->c[1]->c[0] && n->c[1]->c[0]->v.sval && !strcmp(n->c[1]->c[0]->v.sval, "chr") &&
        IS(n->c[1]->c[1], TT_ILIT)) {
        v[1].str = tok_s(n->c[0]);
        v[3].ival = n->c[1]->c[1]->v.ival;
        { pas_scope_define(v[1].str); pas_const_add(v[1].str, v[3].ival); }
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && n->c[1] &&
        (n->c[1]->t == TT_ADD || n->c[1]->t == TT_SUB || n->c[1]->t == TT_MUL || n->c[1]->t == TT_DIV || n->c[1]->t == TT_IDIV || n->c[1]->t == TT_MOD ||
        (n->c[1]->t == TT_FNC && n->c[1]->n == 2 && n->c[1]->c[0] && pas_const_fold_fn(n->c[1]->c[0]->v.sval)) ||
        (n->c[1]->t == TT_VAR && n->c[1]->v.sval && (pas_sconst_get(n->c[1]->v.sval) || pas_rconst_get(n->c[1]->v.sval, &v[5].dval))))) {
        int k = 0;
        long long iv = 0;
        double dv = 0;
        const char *sv = NULL;
        if (pas_fold_const(n->c[1], &k, &iv, &dv, &sv)) {
            v[1].str = tok_s(n->c[0]);
            pas_scope_define(v[1].str);
            if (k == 1) pas_const_add(v[1].str, iv);
            else if (k == 2) pas_rconst_add(v[1].str, dv);
            else if (k == 3) pas_sconst_add(v[1].str, sv);
            else { pas_const_add(v[1].str, iv); }
            return out;
        }
    }
    if (IS(n, TT_DECL) && n->v.sval && !strcmp(n->v.sval, "tconst") && n->n >= 2) {
        if (n->n > 2) {
            fprintf(stderr, "pascal: the typed constant '%s' has an initial value this compiler cannot yet place (an aggregate whose type it cannot resolve)\n",
                n->c[0] && n->c[0]->n > 0 && n->c[0]->c[0] && n->c[0]->c[0]->v.sval ? n->c[0]->c[0]->v.sval : "?");
            g_lower_pas.sem.pas_iso_errors++;
        }
        tree_t *vd = ast_node_new(TT_DECL);
        vd->v.sval = "var";
        ast_push(vd, n->c[0]);
        ast_push(vd, n->c[1]);
        return E_var_decl(vd);
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const") && n->c[1] &&
        (n->c[1]->t == TT_SET || n->c[1]->t == TT_FNC || n->c[1]->t == TT_ADD || n->c[1]->t == TT_SUB || n->c[1]->t == TT_MUL || n->c[1]->t == TT_DIV || n->c[1]->t == TT_IDIV ||
        n->c[1]->t == TT_MOD || n->c[1]->t == TT_CONJ || n->c[1]->t == TT_ALT || n->c[1]->t == TT_NOT ||
        ((n->c[1]->t == TT_PLS || n->c[1]->t == TT_MNS) && n->c[1]->n == 1 && n->c[1]->c[0] && n->c[1]->c[0]->t != TT_ILIT && n->c[1]->c[0]->t != TT_FLIT && n->c[1]->c[0]->t != TT_VAR))) {
        v[1].str = tok_s(n->c[0]);
        v[3] = E_expression(n->c[1]);
        pas_scope_define(v[1].str);
        pas_setconst_add(v[1].str, v[3].node);
        return out;
    }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "const")) { v[1].str = tok_s(n->c[0]); v[3] = E_constant(n->c[1]); { pas_scope_define(v[1].str); pas_const_add(v[1].str, v[3].ival); } return out; }
    elab_bad("const_decl", n);
    return out;
}
pval E_constant(const tree_t *n) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n && n->n > 0 && (IS(n, TT_ADD) || IS(n, TT_SUB) || IS(n, TT_MUL) || IS(n, TT_IDIV) || IS(n, TT_MOD) || IS(n, TT_FNC) || ((IS(n, TT_PLS) || IS(n, TT_MNS)) && n->c[0] && n->c[0]->n > 0))) {
        int k = 0;
        long long iv = 0;
        double dv = 0;
        const char *sv = NULL;
        if (pas_fold_const(n, &k, &iv, &dv, &sv) && (k == 1 || k == 4)) { out.ival = iv; return out; }
    }
    if (IS(n, TT_PLS)) { v[2] = E_scalar_constant(n->c[0]); { out.ival = v[2].ival; } return out; }
    if (IS(n, TT_MNS)) { v[2] = E_scalar_constant(n->c[0]); { out.ival = -v[2].ival; } return out; }
    if (n != NULL) { v[1] = E_scalar_constant(n); { out.ival = v[1].ival; } return out; }
    elab_bad("constant", n);
    return out;
}
static tree_t *pas_case_label_test(const tree_t *ce) {
    if (IS(ce, TT_SUBRANGE) && ce->n == 2) {
        pval lo = E_constant(ce->c[0]), hi = E_constant(ce->c[1]);
        return bin(TT_MUL, bin(TT_GE, leaf_s(TT_VAR, pas_case_cur()), ilit(lo.ival)), bin(TT_LE, leaf_s(TT_VAR, pas_case_cur()), ilit(hi.ival)));
    }
    pval c = E_constant(ce);
    return bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit(c.ival));
}
pval E_constant_list(const tree_t *n, int from, int to) {
    pval out;
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) { tree_t *t = pas_case_label_test(n->c[i]); out.node = i == from ? t : bin(TT_ADD, out.node, t); }
    return out;
}
pval E_procedure_decl_in(const tree_t *n) {
    pval v[13];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int hasp = n->n > 1 && n->c[1] && n->c[1]->t == TT_PARAMS;
    int hasr = n->t == TT_FUNCTION && n->n > (hasp ? 2 : 1) && n->c[hasp ? 2 : 1] && n->c[hasp ? 2 : 1]->t == TT_VAR;
    tree_t *tail = n->c[n->n - 1];
    const char *ovs = pas_ov_psig(n);
    if (IS(n, TT_PROCEDURE) && IS(tail, TT_KEYWORD)) {
        v[2].str = pas_overload_name(tok_s(n->c[0]), ovs);
        v[3] = E_pv_mark(NULL);
        v[4] = E_parameter_list_opt(hasp ? n->c[1] : NULL);
        {
            const char *_sg = pas_pf_heading(v[2].str, v[4].list, 'P', NULL);
            pas_pf_define_routine(v[2].str, _sg);
            pas_ov_tag(ovs);
            pas_scope_fwd_save(v[2].str, v[4].list, _sg);
            pas_proc_add(v[2].str);
            pas_proc_vparams(v[2].str, v[4].list);
        }
        return out;
    }
    if (IS(n, TT_FUNCTION) && hasr && IS(tail, TT_KEYWORD)) {
        v[2].str = pas_overload_name(tok_s(n->c[0]), ovs);
        v[3] = E_pv_mark(NULL);
        v[4] = E_parameter_list_opt(hasp ? n->c[1] : NULL);
        v[6].str = tok_s(n->c[hasp ? 2 : 1]);
        {
            const char *_sg = pas_pf_heading(v[2].str, v[4].list, 'F', v[6].str);
            pas_pf_define_routine(v[2].str, _sg);
            pas_ov_tag(ovs);
            pas_scope_fwd_save(v[2].str, v[4].list, _sg);
            pas_func_add(v[2].str);
            pas_proc_vparams(v[2].str, v[4].list);
            if (v[6].str) pas_scalarvartype_add(v[2].str, v[6].str);
        }
        return out;
    }
    if (IS(n, TT_PROCEDURE)) {
        v[2].str = pas_overload_name(tok_s(n->c[0]), ovs);
        v[3] = E_pv_mark(NULL);
        v[4] = E_parameter_list_opt(hasp ? n->c[1] : NULL);
        {
            pas_pf_define_routine(v[2].str, pas_pf_heading(v[2].str, v[4].list, 'P', NULL));
            pas_ov_tag(ovs);
            pas_scope_enter(v[2].str, v[4].list);
            pas_proc_add(v[2].str);
            pas_proc_vparams(v[2].str, pas_scope_fwd_params(v[2].str, v[4].list));
            pas_proc_enter();
        }
        v[7] = E_block(tail);
        {
            int d = g_lower_pas.sem.pas_ldepth - 1;
            int d_ok = (d >= 0 && d < g_lower_pas.sem.pas_ldepth);
            int dl = d_ok ? g_lower_pas.sem.pas_lstk[d].decl_level : 1;
            const char **ln = d_ok ? g_lower_pas.sem.pas_lstk[d].names : NULL;
            int lc = d_ok ? g_lower_pas.sem.pas_lstk[d].n : 0;
            tree_t *p = mk_proc(v[2].str, pas_scope_fwd_params(v[2].str, v[4].list), pas_trace_wrap_proc(v[2].str, v[4].list, v[7].node, 0), 0, dl, ln, lc);
            pas_proc_exit();
            pas_scope_exit();
            emit_proc(&g_lower_pas.sem.pascal_procs, p);
        }
        return out;
    }
    if (IS(n, TT_FUNCTION) && hasr) {
        v[2].str = pas_overload_name(tok_s(n->c[0]), ovs);
        v[3] = E_pv_mark(NULL);
        v[4] = E_parameter_list_opt(hasp ? n->c[1] : NULL);
        v[6].str = tok_s(n->c[hasp ? 2 : 1]);
        {
            pas_pf_define_routine(v[2].str, pas_pf_heading(v[2].str, v[4].list, 'F', v[6].str));
            pas_ov_tag(ovs);
            pas_scope_enter(v[2].str, v[4].list);
            pas_func_add(v[2].str);
            pas_proc_vparams(v[2].str, pas_scope_fwd_params(v[2].str, v[4].list));
            if (v[6].str) pas_scalarvartype_add(v[2].str, v[6].str);
            pas_proc_enter();
            pas_curfunc_push(v[2].str);
            pas_local_add(v[2].str);
        }
        int resrec = v[6].str && pty_rec_by_name(v[6].str) != NULL;
        v[9] = E_block(tail);
        if (resrec && v[9].node) { tree_t *rb = ast_node_new(TT_PROGRAM); ast_push(rb, bin(TT_ASSIGN, leaf_s(TT_VAR, v[2].str), mk_rec_init_type(v[6].str))); ast_push(rb, v[9].node); v[9].node = rb; }
        {
            int d = g_lower_pas.sem.pas_ldepth - 1;
            int d_ok = (d >= 0 && d < g_lower_pas.sem.pas_ldepth);
            int dl = d_ok ? g_lower_pas.sem.pas_lstk[d].decl_level : 1;
            const char **ln = d_ok ? g_lower_pas.sem.pas_lstk[d].names : NULL;
            int lc = d_ok ? g_lower_pas.sem.pas_lstk[d].n : 0;
            tree_t *p = mk_proc(v[2].str, pas_scope_fwd_params(v[2].str, v[4].list), pas_trace_wrap_proc(v[2].str, v[4].list, pas_result_check(v[2].str, v[9].node), 1), 1, dl, ln, lc);
            pas_proc_exit();
            pas_scope_exit();
            emit_proc(&g_lower_pas.sem.pascal_procs, p);
        }
        return out;
    }
    if (IS(n, TT_FUNCTION)) {
        v[2].str = pas_overload_name(tok_s(n->c[0]), ovs);
        v[3] = E_pv_mark(NULL);
        {
            pas_pf_define_routine(v[2].str, pas_pf_heading(v[2].str, NULL, 'F', NULL));
            pas_ov_tag(ovs);
            pas_scope_enter(v[2].str, NULL);
            pas_func_add(v[2].str);
            pas_proc_vparams(v[2].str, pas_scope_fwd_params(v[2].str, NULL));
            pas_proc_enter();
            pas_curfunc_push(v[2].str);
            pas_local_add(v[2].str);
        }
        v[6] = E_block(tail);
        {
            int d = g_lower_pas.sem.pas_ldepth - 1;
            int d_ok = (d >= 0 && d < g_lower_pas.sem.pas_ldepth);
            int dl = d_ok ? g_lower_pas.sem.pas_lstk[d].decl_level : 1;
            const char **ln = d_ok ? g_lower_pas.sem.pas_lstk[d].names : NULL;
            int lc = d_ok ? g_lower_pas.sem.pas_lstk[d].n : 0;
            tree_t *p = mk_proc(v[2].str, pas_scope_fwd_params(v[2].str, pnl_new()), pas_trace_wrap_proc(v[2].str, NULL, pas_result_check(v[2].str, v[6].node), 1), 1, dl, ln, lc);
            pas_proc_exit();
            pas_scope_exit();
            emit_proc(&g_lower_pas.sem.pascal_procs, p);
        }
        return out;
    }
    elab_bad("procedure_decl", n);
    return out;
}
