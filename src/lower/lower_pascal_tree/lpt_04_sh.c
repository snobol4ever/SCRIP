#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include <stdlib.h>
#include "ir/ast.h"
#include "lower/lower_pascal_types.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_11_sh.h"
static void pas_proctype_add(const char *name, const char *sig);
static pval E_case_elem(const tree_t *n);
static pval E_case_open(const tree_t *n);
static pval E_packed_opt(const tree_t *n);
static pval E_record_body(const tree_t *n);
static pval E_record_case_arm(const tree_t *n);
static pval E_record_case_list(const tree_t *n, int from, int to);
static pval E_record_case_opt(const tree_t *n);
static pval E_record_field(const tree_t *n);
static pval E_record_field_list(const tree_t *n, int from, int to);
static pval E_type(const tree_t *n);
static pval E_type_decl(const tree_t *n);
static int pas_fcap(int n) { return n > 128 ? n : 128; }
static void pas_pend_ensure(int n) {
    int oc = g_lower_pas.sem.pas_pend_cap;
    if (n <= oc) return;
    int nc = oc ? oc : 128;
    while (nc < n) nc *= 2;
#define PAS_PEND_GROW(a) g_lower_pas.sem.a = ct_grow(g_lower_pas.sem.a, (size_t)nc * sizeof *g_lower_pas.sem.a); memset(g_lower_pas.sem.a + oc, 0, (size_t)(nc - oc) * sizeof *g_lower_pas.sem.a)
    PAS_PEND_GROW(pas_pend_fields);
    PAS_PEND_GROW(pas_pend_fldptrto);
    PAS_PEND_GROW(pas_pend_fldenum);
    PAS_PEND_GROW(pas_pend_fldrec);
    PAS_PEND_GROW(pas_pend_fldtypename);
    PAS_PEND_GROW(pas_pend_fldfile);
#undef PAS_PEND_GROW
    g_lower_pas.sem.pas_pend_cap = nc;
}
void pas_scope_define(const char *name) {
    if (name) { pas_scope_defined_after_use(name); pas_scope_push(&g_lower_pas.sem.pas_scope.defs, &g_lower_pas.sem.pas_scope.n, &g_lower_pas.sem.pas_scope.cap, name); }
}
int pas_scope_uid(const char *name) {
    for (int i = g_lower_pas.sem.pas_scope.n - 1; name && i >= 0; i--)
        if (g_lower_pas.sem.pas_scope.defs[i].name && !g_lower_pas.sem.pas_scope.defs[i].fld && !strcmp(g_lower_pas.sem.pas_scope.defs[i].name, name)) return g_lower_pas.sem.pas_scope.defs[i].uid;
    return 0;
}
void pas_scope_define_list(PNodeList *ids) { for (int i = 0; ids && i < ids->count; i++) if (ids->items[i] && ids->items[i]->v.sval) pas_scope_define(ids->items[i]->v.sval); }
static void pas_tsz_add(const char *name, long long bytes) {
    if (!name || bytes < 0) return;
    if (g_lower_pas.sem.pas_ntsz >= g_lower_pas.sem.pas_tszcap) {
        g_lower_pas.sem.pas_tszcap = g_lower_pas.sem.pas_tszcap ? g_lower_pas.sem.pas_tszcap * 2 : 64;
        g_lower_pas.sem.pas_tsz = (__typeof__(g_lower_pas.sem.pas_tsz))ct_grow(g_lower_pas.sem.pas_tsz, (size_t)g_lower_pas.sem.pas_tszcap * sizeof *g_lower_pas.sem.pas_tsz);
    }
    g_lower_pas.sem.pas_tsz[g_lower_pas.sem.pas_ntsz].name = ct_strdup(name);
    g_lower_pas.sem.pas_tsz[g_lower_pas.sem.pas_ntsz].bytes = bytes;
    g_lower_pas.sem.pas_ntsz++;
}
int pas_tsz_get(const char *name, long long *out) {
    if (!name) return 0;
    for (int i = g_lower_pas.sem.pas_ntsz - 1; i >= 0; i--) if (!strcmp(g_lower_pas.sem.pas_tsz[i].name, name)) { *out = g_lower_pas.sem.pas_tsz[i].bytes; return 1; }
    return 0;
}
int pas_array_is_zero_size(const char *name) { long long b; return pas_tsz_get(name, &b) && b == 0; }
void pas_setconst_add(const char *name, tree_t *val) {
    if (!name || !val) return;
    if (g_lower_pas.sem.pas_nsetconst >= g_lower_pas.sem.pas_setconst_cap) {
        g_lower_pas.sem.pas_setconst_cap = g_lower_pas.sem.pas_setconst_cap ? g_lower_pas.sem.pas_setconst_cap * 2 : 16;
        g_lower_pas.sem.pas_setconsts = ct_grow(g_lower_pas.sem.pas_setconsts, (size_t)g_lower_pas.sem.pas_setconst_cap * sizeof *g_lower_pas.sem.pas_setconsts);
    }
    g_lower_pas.sem.pas_setconsts[g_lower_pas.sem.pas_nsetconst].name = ct_strdup(name);
    g_lower_pas.sem.pas_setconsts[g_lower_pas.sem.pas_nsetconst].val = val;
    g_lower_pas.sem.pas_setconsts[g_lower_pas.sem.pas_nsetconst].uid = pas_scope_uid(name);
    g_lower_pas.sem.pas_nsetconst++;
}
static void pas_rows_add(const char *name, long long lo, long long hi, int istype, int kind) {
    if (!name || hi < lo) return;
    if (g_lower_pas.sem.pas_nrows >= g_lower_pas.sem.pas_rows_cap) g_lower_pas.sem.pas_rows =
        pas_pool_grow(g_lower_pas.sem.pas_rows, &g_lower_pas.sem.pas_rows_cap, sizeof *g_lower_pas.sem.pas_rows, 16);
    g_lower_pas.sem.pas_rows[g_lower_pas.sem.pas_nrows].name = ct_strdup(name);
    g_lower_pas.sem.pas_rows[g_lower_pas.sem.pas_nrows].lo = lo;
    g_lower_pas.sem.pas_rows[g_lower_pas.sem.pas_nrows].hi = hi;
    g_lower_pas.sem.pas_rows[g_lower_pas.sem.pas_nrows].istype = istype;
    g_lower_pas.sem.pas_rows[g_lower_pas.sem.pas_nrows].kind = kind;
    g_lower_pas.sem.pas_rows[g_lower_pas.sem.pas_nrows].uid = pas_scope_uid(name);
    g_lower_pas.sem.pas_nrows++;
}
int pas_rows_get(const char *name, int istype, long long *lo, long long *hi) {
    int u = pas_scope_uid(name);
    for (int i = g_lower_pas.sem.pas_nrows - 1; name && i >= 0; i--)
        if (g_lower_pas.sem.pas_rows[i].istype == istype && g_lower_pas.sem.pas_rows[i].uid == u && !strcmp(g_lower_pas.sem.pas_rows[i].name, name)) {
        if (lo) *lo = g_lower_pas.sem.pas_rows[i].lo;
        if (hi) *hi = g_lower_pas.sem.pas_rows[i].hi;
        return 1 + g_lower_pas.sem.pas_rows[i].kind;
    }
    return 0;
}
tree_t *pas_setconst_get(const char *name) {
    int u = pas_scope_uid(name);
    for (int i = g_lower_pas.sem.pas_nsetconst - 1; name && i >= 0; i--) if (g_lower_pas.sem.pas_setconsts[i].uid == u && !strcmp(g_lower_pas.sem.pas_setconsts[i].name, name))
        return g_lower_pas.sem.pas_setconsts[i].val;
    return NULL;
}
long long pas_array_low(const char *name) {
    const pty_t *t = name ? pty_name_type(name) : NULL;
    return t && t->k == PTK_ARRAY && !t->dyn && !t->open && !pas_array_is_param(name) && t->ix && t->ix[0] ? t->ix[0]->lo : 0;
}
static void pas_proctype_add(const char *name, const char *sig) {
    if (!name || !sig) return;
    if (g_lower_pas.sem.pas_nproctype >= g_lower_pas.sem.pas_proctype_cap) {
        g_lower_pas.sem.pas_proctype_cap = g_lower_pas.sem.pas_proctype_cap ? g_lower_pas.sem.pas_proctype_cap * 2 : 16;
        g_lower_pas.sem.pas_proctypes = ct_grow(g_lower_pas.sem.pas_proctypes, (size_t)g_lower_pas.sem.pas_proctype_cap * sizeof *g_lower_pas.sem.pas_proctypes);
    }
    g_lower_pas.sem.pas_proctypes[g_lower_pas.sem.pas_nproctype].name = ct_strdup(name);
    g_lower_pas.sem.pas_proctypes[g_lower_pas.sem.pas_nproctype].sig = ct_strdup(sig);
    g_lower_pas.sem.pas_proctypes[g_lower_pas.sem.pas_nproctype].uid = pas_scope_uid(name);
    g_lower_pas.sem.pas_nproctype++;
}
static void pas_pend_nest_save(void) {
    if (g_lower_pas.sem.pas_scope.npnest >= g_lower_pas.sem.pas_scope.cpnest) {
        g_lower_pas.sem.pas_scope.cpnest = g_lower_pas.sem.pas_scope.cpnest ? g_lower_pas.sem.pas_scope.cpnest * 2 : 4;
        g_lower_pas.sem.pas_scope.pnest = (struct pas_pend_frame *)ct_grow(g_lower_pas.sem.pas_scope.pnest, (size_t)g_lower_pas.sem.pas_scope.cpnest * sizeof *g_lower_pas.sem.pas_scope.pnest);
    }
    struct pas_pend_frame *fr = &g_lower_pas.sem.pas_scope.pnest[g_lower_pas.sem.pas_scope.npnest++];
    int n = g_lower_pas.sem.pas_pend_nf, fc = pas_fcap(n);
    fr->nf = n;
    fr->fields = ct_zalloc((size_t)fc, sizeof *fr->fields);
    fr->fldptrto = ct_zalloc((size_t)fc, sizeof *fr->fldptrto);
    fr->fldenum = ct_zalloc((size_t)fc, sizeof *fr->fldenum);
    fr->fldrec = ct_zalloc((size_t)fc, sizeof *fr->fldrec);
    fr->fldtypename = ct_zalloc((size_t)fc, sizeof *fr->fldtypename);
    fr->fldfile = ct_zalloc((size_t)fc, sizeof *fr->fldfile);
    for (int i = 0; i < n; i++) {
        fr->fields[i] = g_lower_pas.sem.pas_pend_fields[i];
        fr->fldptrto[i] = g_lower_pas.sem.pas_pend_fldptrto[i];
        fr->fldenum[i] = g_lower_pas.sem.pas_pend_fldenum[i];
        fr->fldrec[i] = g_lower_pas.sem.pas_pend_fldrec[i];
        fr->fldtypename[i] = g_lower_pas.sem.pas_pend_fldtypename[i];
        fr->fldfile[i] = g_lower_pas.sem.pas_pend_fldfile[i];
    }
    g_lower_pas.sem.pas_pend_nf = 0;
}
static void pas_pend_nest_restore(void) {
    if (g_lower_pas.sem.pas_scope.npnest <= 0) return;
    struct pas_pend_frame *fr = &g_lower_pas.sem.pas_scope.pnest[--g_lower_pas.sem.pas_scope.npnest];
    int n = fr->nf;
    pas_pend_ensure(n);
    for (int i = 0; i < n; i++) {
        g_lower_pas.sem.pas_pend_fields[i] = fr->fields[i];
        g_lower_pas.sem.pas_pend_fldptrto[i] = fr->fldptrto[i];
        g_lower_pas.sem.pas_pend_fldenum[i] = fr->fldenum[i];
        g_lower_pas.sem.pas_pend_fldrec[i] = fr->fldrec[i];
        g_lower_pas.sem.pas_pend_fldtypename[i] = fr->fldtypename[i];
        g_lower_pas.sem.pas_pend_fldfile[i] = fr->fldfile[i];
    }
    g_lower_pas.sem.pas_pend_nf = n;
}
const char *pas_ptrtype_target(const char *p) {
    if (!p) return NULL;
    for (int i = 0; i < g_lower_pas.sem.pas_nptrtype; i++) if (g_lower_pas.sem.pas_ptrtypes[i].pname && !strcmp(g_lower_pas.sem.pas_ptrtypes[i].pname, p)) return g_lower_pas.sem.pas_ptrtypes[i].rname;
    const char *const B[] = { "byte", "word", "integer", "longint", "longword", "cardinal", "shortint", "smallint", "int64", "uint64", "qword", "boolean", "double", "single", 0 };
    for (int i = 0; B[i]; i++) if (p[0] == 'p' && !strcmp(p + 1, B[i])) return B[i];
    return NULL;
}
static int pas_array_of_ptr(const pty_t *t) { return t && t->k == PTK_ARRAY && t->base && t->base->k == PTK_POINTER && !pty_is_pchar(t->base); }
const char *pas_arrptr_target(const char *v) {
    const pty_t *t = v ? pty_name_type(v) : NULL, *e = NULL, *b;
    if (v && !t) t = pty_type_by_name(v);
    while (t && t->k == PTK_ARRAY) { e = t; t = t->base; }
    b = pas_array_of_ptr(e) ? pty_pointee(e->base) : NULL;
    return b ? pty_name_of(b) : NULL;
}
static void pas_pend_reset(void) {
    g_lower_pas.sem.pas_pend_proctype = NULL;
    g_lower_pas.sem.pas_pend_dyn = 0;
    g_lower_pas.sem.pas_pend_row_lo = 0;
    g_lower_pas.sem.pas_pend_row_hi = -1;
    g_lower_pas.sem.pas_pend_row_kind = 0;
    g_lower_pas.sem.pas_pend_isarr = 0;
    g_lower_pas.sem.pas_pend_isbool = 0;
    g_lower_pas.sem.pas_pend_istfile = 0;
    g_lower_pas.sem.pas_pend_nf = 0;
    g_lower_pas.sem.pas_pend_ptrtarget = NULL;
    g_lower_pas.sem.pas_pend_typename = NULL;
    g_lower_pas.sem.pas_pend_ischar = 0;
    g_lower_pas.sem.pas_pend_arr_ischar = 0;
    g_lower_pas.sem.pas_pend_enum_max = -1;
    g_lower_pas.sem.pas_pend_sub_low = 0;
    g_lower_pas.sem.pas_pend_sub_high = -1;
    g_lower_pas.sem.pas_pend_arr_ncols = -1;
    g_lower_pas.sem.pas_pend_arr_wrap = 0;
    g_lower_pas.sem.pas_pend_set_hi = -1;
    g_lower_pas.sem.pas_pend_esz = -1;
}
long long pas_array_nonparam_low(const char *name) { return pas_array_low(name); }
long long pas_rk_get(long long ov, int w) { return w == 0 ? (ov >> 16) & 0xF : w == 1 ? (ov >> 20) & 0xFF : w == 2 ? (ov >> 28) & 0xF : w == 3 ? (ov >> 32) & 0xFFFF : (ov >> 48) & 0xFF; }
static void pas_pend_add(const char *f) {
    if (f) pas_pend_ensure(g_lower_pas.sem.pas_pend_nf + 1);
    if (f) {
        g_lower_pas.sem.pas_pend_fldptrto[g_lower_pas.sem.pas_pend_nf] = g_lower_pas.sem.pas_pend_ptrtarget;
        g_lower_pas.sem.pas_pend_fldenum[g_lower_pas.sem.pas_pend_nf] = (g_lower_pas.sem.pas_pend_typename && pas_enumnames_idx(g_lower_pas.sem.pas_pend_typename) >= 0) ?
            g_lower_pas.sem.pas_pend_typename : NULL;
        g_lower_pas.sem.pas_pend_fldrec[g_lower_pas.sem.pas_pend_nf] = (g_lower_pas.sem.pas_pend_typename && pas_rectype_nf(g_lower_pas.sem.pas_pend_typename) > 0) ?
            g_lower_pas.sem.pas_pend_typename : NULL;
        g_lower_pas.sem.pas_pend_fldtypename[g_lower_pas.sem.pas_pend_nf] = g_lower_pas.sem.pas_pend_typename;
        if (g_lower_pas.sem.pas_pend_dyn > 0 && g_lower_pas.sem.pas_pend_typename) {
            size_t _mn = strlen(g_lower_pas.sem.pas_pend_typename) + 16;
            char *_mk = (char *)ct_zalloc(_mn, 1);
            snprintf(_mk, _mn, "$dyn%d$%s", g_lower_pas.sem.pas_pend_dyn, g_lower_pas.sem.pas_pend_typename);
            g_lower_pas.sem.pas_pend_fldtypename[g_lower_pas.sem.pas_pend_nf] = _mk;
        }
        g_lower_pas.sem.pas_pend_fldfile[g_lower_pas.sem.pas_pend_nf] = g_lower_pas.sem.pas_pend_istfile ||
            (g_lower_pas.sem.pas_pend_typename && (!strcmp(g_lower_pas.sem.pas_pend_typename, "text") || pas_rectype_has_file(g_lower_pas.sem.pas_pend_typename)));
        g_lower_pas.sem.pas_pend_fields[g_lower_pas.sem.pas_pend_nf++] = ct_strdup(f);
        g_lower_pas.sem.pas_pend_ptrtarget = NULL;
        g_lower_pas.sem.pas_pend_arr_ischar = 0;
        g_lower_pas.sem.pas_pend_ischar = 0;
        g_lower_pas.sem.pas_pend_isarr = 0;
        g_lower_pas.sem.pas_pend_istfile = 0;
    }
}
int pas_rectype_to_pend(const char *tn) {
    const pty_t *t = tn ? pty_rec_by_name(tn) : NULL;
    if (!t || pty_rec_nf(t) <= 0) return 0;
    pas_pend_reset();
    pas_pend_ensure(pty_rec_nf(t));
    for (int j = 0; j < pty_rec_nf(t); j++) {
        struct pty_fld f;
        int k = g_lower_pas.sem.pas_pend_nf++;
        pty_rec_fld(t, j, &f);
        g_lower_pas.sem.pas_pend_fldptrto[k] = (char *)(f.ptr && f.ft ? pty_name_of(f.ft->base ? f.ft->base : f.ft) : NULL);
        g_lower_pas.sem.pas_pend_fldenum[k] = (char *)(f.en && f.ft ? pty_name_of(f.ft) : NULL);
        g_lower_pas.sem.pas_pend_fldrec[k] = (char *)pas_fld_rectype(&f);
        g_lower_pas.sem.pas_pend_fldtypename[k] = (char *)pty_fld_typename(t, j);
        g_lower_pas.sem.pas_pend_fldfile[k] = f.file;
        g_lower_pas.sem.pas_pend_fields[k] = (char *)f.name;
    }
    return 1;
}
int pas_is_declared_local_here(const char *name) {
    if (!name || g_lower_pas.sem.pas_ldepth <= 0) return 0;
    int d = g_lower_pas.sem.pas_ldepth - 1;
    for (int i = 0; i < g_lower_pas.sem.pas_lstk[d].n; i++) if (g_lower_pas.sem.pas_lstk[d].names[i] && !strcmp(g_lower_pas.sem.pas_lstk[d].names[i], name)) return 1;
    return 0;
}
static void pas_settype_add(const char *name, long long hi) {
    if (name) {
        if (g_lower_pas.sem.pas_nsettype >= g_lower_pas.sem.pas_settypes_cap) g_lower_pas.sem.pas_settypes =
            pas_pool_grow(g_lower_pas.sem.pas_settypes, &g_lower_pas.sem.pas_settypes_cap, sizeof *g_lower_pas.sem.pas_settypes, 16);
        g_lower_pas.sem.pas_settypes[g_lower_pas.sem.pas_nsettype].name = ct_strdup(name);
        g_lower_pas.sem.pas_settypes[g_lower_pas.sem.pas_nsettype].hi = hi;
        g_lower_pas.sem.pas_settypes[g_lower_pas.sem.pas_nsettype].pack = g_lower_pas.sem.pas_pack_set_size;
        g_lower_pas.sem.pas_nsettype++;
    }
}
static int pas_tree_same(tree_t *a, tree_t *b) {
    if (!a || !b) return a == b;
    if (a->t != b->t || a->n != b->n) return 0;
    if (a->t == TT_VAR || a->t == TT_QLIT) { if (!a->v.sval || !b->v.sval || strcmp(a->v.sval, b->v.sval)) return 0; } else if (a->t == TT_ILIT) { if (a->v.ival != b->v.ival) return 0; }
    for (int i = 0; i < a->n; i++) if (!pas_tree_same(a->c[i], b->c[i])) return 0;
    return 1;
}
int pas_with_holds_pointee_of(tree_t *ptr) {
    for (int i = 0; ptr && i < g_lower_pas.sem.with_depth; i++) {
        tree_t *w = g_lower_pas.sem.with_stk[i].orig;
        if (w && w->t == TT_FNC && w->n == 2 && w->c[0] && w->c[0]->v.sval && !strcmp(w->c[0]->v.sval, "__pas_deref") && pas_tree_same(w->c[1], ptr)) return 1;
    }
    return 0;
}
static void pas_type_not_self_applied(const char *name) {
    int self = name && g_lower_pas.sem.pas_pend_typename && !strcmp(g_lower_pas.sem.pas_pend_typename, name) && !g_lower_pas.sem.pas_pend_ptrtarget;
    for (int i = 0; name && !self && i < g_lower_pas.sem.pas_pend_nf; i++)
        if (g_lower_pas.sem.pas_pend_fldtypename[i] && !strcmp(g_lower_pas.sem.pas_pend_fldtypename[i], name) && !g_lower_pas.sem.pas_pend_fldptrto[i]) self = 1;
    if (!self) return;
    fprintf(stderr, "pascal: ISO 7185 6.4.1 violation: the type-denoter of the type-definition of '%s' contains an applied occurrence of '%s' outside the domain-type of a pointer-type\n", name, name);
    g_lower_pas.sem.pas_iso_errors++;
}
tree_t *pas_region_wrap(tree_t *e, long long ov, long long lo) {
    int cls = (int)pas_rk_get(ov, 0), es = (int)pas_rk_get(ov, 1), ecls = (int)pas_rk_get(ov, 2);
    long long off = pas_rk_get(ov, 3), rsz = (ov >> 48) & 0xFFFF;
    tree_t *f = ast_node_new(TT_FNC);
    ast_push(f, leaf_s(TT_VAR, cls == 7 ? "__pas_rarr" : "__pas_rdecode"));
    ast_push(f, e);
    ast_push(f, ilit(off));
    ast_push(f, ilit(cls == 7 ? (long long)(ecls | (es << 8)) : (long long)(cls | (es << 8))));
    if (cls == 7) ast_push(f, ilit(lo));
    ast_push(f, ilit(rsz));
    return f;
}
tree_t *pas_arrrec_region(tree_t *e, const char *an, int fi) {
    struct pty_fld f;
    if (!e || !an || fi < 0 || !pas_arf(an, fi, &f)) return e;
    long long ov = f.ov, lo = f.nalo;
    if (!(((ov >> 48) & 0xFFFF) != 0 && pas_rk_get(ov, 0) != 0)) return e;
    if (pas_rk_get(ov, 0) == 7) lo = f.ca ? f.calo : f.nalo;
    return pas_region_wrap(e, ov, lo);
}
static void pas_pf_variable(const char *name, const char *sig, PNodeList *decl) {
    for (int i = g_lower_pas.sem.pas_scope.n - 1; i >= 0; i--) {
        PasDef *d = &g_lower_pas.sem.pas_scope.defs[i];
        if (d->name && !d->fld && !strcmp(d->name, name)) { d->sig = sig; d->formal = 1; break; }
    }
    for (int k = 1; k <= 3; k++) { const char *en = pas_pf_env_name(name, k); pas_scope_define(en); pas_local_add(en); pnl_push(decl, leaf_s(TT_VAR, en)); }
}
void elab_bad(const char *nt, const tree_t *n) { fprintf(stderr, "pascal: internal error: no production of '%s' fits tree node kind %d\n", nt, n ? (int)n->t : -1); abort(); }
static pval E_case_elem(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_ARM)) { v[1] = E_constant_list(n->c[0], 0, n->c[0]->n); v[3] = E_body_stmt(n->c[1]); { out.node = bin(TT_IF, lpt_pas_cond(v[1].node), v[3].node); } return out; }
    elab_bad("case_elem", n);
    return out;
}
pval E_case_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_case_elem(ce);
            { PNodeList *l = pnl_new(); if (v[1].node) pnl_push(l, v[1].node); out.list = l; }
        } else {
            v[1] = out;
            v[3] = E_case_elem(ce);
            { if (v[3].node) pnl_push(v[1].list, v[3].node); out.list = v[1].list; }
        }
    }
    return out;
}
static pval E_case_open(const tree_t *n) { pval v[3]; pval out; memset(v, 0, sizeof v); memset(&out, 0, sizeof out); if (n == NULL) { return out; } elab_bad("case_open", n); return out; }
static pval E_packed_opt(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n == NULL) { { out.ival = 0; } return out; }
    if (n != NULL) { { out.ival = 1; } return out; }
    elab_bad("packed_opt", n);
    return out;
}
pval E_pv_mark(const tree_t *n) { pval v[3]; pval out; memset(v, 0, sizeof v); memset(&out, 0, sizeof out); if (n == NULL) return out; elab_bad("pv_mark", n); return out; }
static pval E_record_body(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int hasv = n->n > 0 && n->c[n->n - 1] && n->c[n->n - 1]->t == TT_CASE;
    if (IS(n, TT_FIELDS)) { v[1] = E_record_field_list(n, 0, hasv ? n->n - 1 : n->n); v[2] = E_record_case_opt(hasv ? n->c[n->n - 1] : NULL); return out; }
    elab_bad("record_body", n);
    return out;
}
static pval E_record_case_arm(const tree_t *n) {
    pval v[9];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_ARM)) { v[1] = E_constant_list(n->c[0], 0, n->c[0]->n); v[5] = E_record_body(n->c[1]); { out.node = v[1].node; } return out; }
    elab_bad("record_case_arm", n);
    return out;
}
static pval E_record_case_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_record_case_arm(ce);
            { PNodeList *l = pnl_new(); if (v[1].node) pnl_push(l, v[1].node); out.list = l; }
        } else {
            v[1] = out;
            v[3] = E_record_case_arm(ce);
            { if (v[3].node) pnl_push(v[1].list, v[3].node); out.list = v[1].list; }
        }
    }
    return out;
}
static pval E_record_case_opt(const tree_t *n) {
    pval v[10];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_CASE) && n->n >= 2 && n->c[1] && n->c[1]->t == TT_ENUM_TYPE) {
        v[2].str = tok_s(n->c[0]);
        v[4] = E_simple_type(n->c[1]);
        v[6] = E_case_open(NULL);
        v[7] = E_record_case_list(n, 2, n->n);
        { pas_scope_define(v[2].str); if (v[2].str) pas_pend_add(v[2].str); }
        return out;
    }
    if (IS(n, TT_CASE) && n->n >= 2 && n->c[1] && n->c[1]->t == TT_VAR) {
        v[2].str = tok_s(n->c[0]);
        v[4].str = tok_s(n->c[1]);
        v[6] = E_case_open(NULL);
        v[7] = E_record_case_list(n, 2, n->n);
        {
            pas_scope_define(v[2].str);
            pas_variant_constants_in_tag_type(v[4].str, v[7].list);
            if (v[2].str) {
                g_lower_pas.sem.pas_pend_typename = ct_strdup(v[4].str);
                { int _sb = g_lower_pas.sem.pas_pend_isbool; g_lower_pas.sem.pas_pend_isbool = pas_is_booltype(v[4].str); pas_pend_add(v[2].str); g_lower_pas.sem.pas_pend_isbool = _sb; }
            }
        }
        return out;
    }
    if (IS(n, TT_CASE)) {
        v[2].str = tok_s(n->c[0]);
        v[4] = E_case_open(NULL);
        v[5] = E_record_case_list(n, 1, n->n);
        { pas_variant_constants_in_tag_type(v[2].str, v[5].list); if (v[2].str) pas_pend_add(v[2].str); }
        return out;
    }
    if (n == NULL) { return out; }
    elab_bad("record_case_opt", n);
    return out;
}
static pval E_record_field(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "field")) {
        v[1] = E_id_list(n->c[0], 0, n->c[0]->n);
        { v[3].str = g_lower_pas.sem.pas_pend_typename; g_lower_pas.sem.pas_pend_typename = NULL; g_lower_pas.sem.pas_pend_isbool = 0; }
        v[4] = E_type(n->c[1]);
        {
            int _dd = 0;
            v[5].ival = g_lower_pas.sem.pas_pend_dyn;
            for (const tree_t *_t = n->c[1]; _t && IS(_t, TT_ARRAY_TYPE) && _t->n > 0 && _t->n - ((_t->c[0] && _t->c[0]->t == TT_KEYWORD) ? 1 : 0) == 1; _t = _t->c[_t->n - 1]) _dd++;
            g_lower_pas.sem.pas_pend_dyn = _dd;
        }
        {
            pas_scope_define_list(v[1].list);
            int _syn = 0;
            if (v[4].ival == -2 && !g_lower_pas.sem.pas_pend_typename) {
                char _snm[32];
                snprintf(_snm, sizeof _snm, "$anonset%d", ++g_lower_pas.sem.pas_scope.anonseq);
                pas_settype_add(_snm, g_lower_pas.sem.pas_pend_set_hi);
                g_lower_pas.sem.pas_pend_typename = ct_strdup(_snm);
                _syn = 1;
            }
            if (g_lower_pas.sem.pas_pend_proctype && !g_lower_pas.sem.pas_pend_typename) {
                char _snm[32];
                snprintf(_snm, sizeof _snm, "$anonproc%d", ++g_lower_pas.sem.pas_scope.anonseq);
                pas_proctype_add(_snm, g_lower_pas.sem.pas_pend_proctype);
                g_lower_pas.sem.pas_pend_typename = ct_strdup(_snm);
                g_lower_pas.sem.pas_pend_proctype = NULL;
                _syn = 1;
            }
            if (v[1].list) {
                char *_svp = g_lower_pas.sem.pas_pend_ptrtarget;
                int _svc = g_lower_pas.sem.pas_pend_ischar;
                int _sva = g_lower_pas.sem.pas_pend_arr_ischar;
                int _svr = g_lower_pas.sem.pas_pend_isarr;
                for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) {
                    g_lower_pas.sem.pas_pend_ptrtarget = _svp;
                    g_lower_pas.sem.pas_pend_ischar = _svc;
                    g_lower_pas.sem.pas_pend_arr_ischar = _sva;
                    g_lower_pas.sem.pas_pend_isarr = _svr;
                    pas_pend_add(v[1].list->items[i]->v.sval);
                }
            }
            g_lower_pas.sem.pas_pend_dyn = (int)v[5].ival;
            if (!g_lower_pas.sem.pas_pend_typename || _syn) g_lower_pas.sem.pas_pend_typename = v[3].str;
        }
        return out;
    }
    elab_bad("record_field", n);
    return out;
}
static pval E_record_field_list(const tree_t *n, int from, int to) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) { const tree_t *ce = n->c[i]; if (i == from) { v[1] = E_record_field(ce); } else { v[1] = out; v[3] = E_record_field(ce); } }
    return out;
}
static pval E_type(const tree_t *n) {
    pval v[12];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int off = (n->n > 0 && n->c[0] && n->c[0]->t == TT_KEYWORD) ? 1 : 0;
    if (IS(n, TT_PTR_TYPE)) { v[2].str = tok_s(n->c[0]); { g_lower_pas.sem.pas_pend_esz = -1; g_lower_pas.sem.pas_pend_ptrtarget = ct_strdup(v[2].str); out.ival = -3; } return out; }
    if (IS(n, TT_DECL) && n->v.sval && (!strcmp(n->v.sval, "proctype") || !strcmp(n->v.sval, "functype"))) {
        int fn = !strcmp(n->v.sval, "functype");
        const tree_t *pp = (n->n > 0 && IS(n->c[0], TT_PARAMS)) ? n->c[0] : NULL;
        v[2] = E_pf_params(pp);
        const char *rt = fn && n->n > 0 ? tok_s(n->c[n->n - 1]) : NULL;
        g_lower_pas.sem.pas_pend_proctype = pas_pf_sig(fn ? 'F' : 'P', v[2].str, rt);
        out.ival = -1;
        return out;
    }
    if (IS(n, TT_ARRAY_TYPE) && n->n - off == 1) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        g_lower_pas.sem.pas_pend_esz = -1;
        g_lower_pas.sem.pas_pend_dyn = 0;
        g_lower_pas.sem.pas_pend_row_lo = 0;
        g_lower_pas.sem.pas_pend_row_hi = -1;
        v[9] = E_type(n->c[off]);
        {
            int inner = g_lower_pas.sem.pas_pend_dyn;
            int _eic = g_lower_pas.sem.pas_pend_ischar ? (g_lower_pas.sem.pas_pend_ischar | PAS_CA_NOSTR) : 0;
            int _wr = g_lower_pas.sem.pas_pend_arr_ischar || (g_lower_pas.sem.pas_pend_typename && pas_arrtype_ischar(g_lower_pas.sem.pas_pend_typename));
            g_lower_pas.sem.pas_pend_arr_wrap = _wr;
            g_lower_pas.sem.pas_pend_ptrtarget = NULL;
            g_lower_pas.sem.pas_pend_arr_ncols = -1;
            g_lower_pas.sem.pas_pend_arr_ischar = _eic;
            g_lower_pas.sem.pas_pend_isarr = 1;
            g_lower_pas.sem.pas_pend_sub_low = 0;
            g_lower_pas.sem.pas_pend_sub_high = 0;
            g_lower_pas.sem.pas_pend_row_lo = 0;
            g_lower_pas.sem.pas_pend_row_hi = -1;
            g_lower_pas.sem.pas_pend_esz = -1;
            g_lower_pas.sem.pas_pend_dyn = inner + 1;
            out.ival = 0;
        }
        return out;
    }
    if (IS(n, TT_ARRAY_TYPE) && n->n - ((n->n > 0 && n->c[0] && n->c[0]->t == TT_KEYWORD) ? 1 : 0) == 2) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        v[4] = E_simple_type(n->c[off]);
        { v[7].ival = pas_index_bound(v[4].ival, 0); g_lower_pas.sem.pas_last_const_enum = pas_index_is_enum(v[4].ival); }
        { v[8].ival = pas_index_bound(v[4].ival, 1); g_lower_pas.sem.pas_pend_esz = -1; }
        long long _rlo = 0, _rhi = -1;
        int _rkind = 0;
        {
            const tree_t *el = n->c[off + 1];
            g_lower_pas.sem.pas_pend_row_lo = 0;
            g_lower_pas.sem.pas_pend_row_hi = -1;
            v[9] = E_type(el);
            int char_row = g_lower_pas.sem.pas_pend_ischar && (g_lower_pas.sem.pas_pend_arr_ischar & PAS_CA_NOSTR);
            int scalar_el = (char_row || (!g_lower_pas.sem.pas_pend_ischar && !g_lower_pas.sem.pas_pend_arr_ischar)) && g_lower_pas.sem.pas_pend_nf == 0 && !g_lower_pas.sem.pas_pend_ptrtarget &&
                !(el && pas_array_of_ptr(pty_of_src(el))) && g_lower_pas.sem.pas_pend_arr_ncols < 0 && !(g_lower_pas.sem.pas_pend_row_hi >= g_lower_pas.sem.pas_pend_row_lo);
            if (scalar_el && el && el->t == TT_ARRAY_TYPE && el->n - ((el->n > 0 && el->c[0] && el->c[0]->t == TT_KEYWORD) ? 1 : 0) == 2 &&
                g_lower_pas.sem.pas_pend_sub_high >= g_lower_pas.sem.pas_pend_sub_low) {
                _rlo = g_lower_pas.sem.pas_pend_sub_low;
                _rhi = g_lower_pas.sem.pas_pend_sub_high;
                _rkind = char_row ? 1 : (g_lower_pas.sem.pas_pend_isbool && !g_lower_pas.sem.pas_pend_ischar) ? 2 : 0;
            }
                else if (el && el->t == TT_VAR && el->v.sval && pas_arrtype_high(el->v.sval) >= 0 && (!pas_arrtype_ischar(el->v.sval) || pas_arrtype_nostr(el->v.sval)) &&
                pas_arrtype_ncols(el->v.sval) < 0 && pas_rectype_nf(el->v.sval) <= 0 && !pas_arrptr_target(el->v.sval) && g_lower_pas.sem.pas_pend_nf == 0 && !g_lower_pas.sem.pas_pend_ptrtarget &&
                !pas_rows_get(el->v.sval, 1, NULL, NULL)) {
                _rlo = pas_arrtype_lo(el->v.sval);
                _rhi = pas_arrtype_high(el->v.sval);
                _rkind = pas_arrtype_ischar(el->v.sval) ? 1 : pas_arrtype_isbool(el->v.sval) ? 2 : 0;
            }
            g_lower_pas.sem.pas_pend_row_lo = 0;
            g_lower_pas.sem.pas_pend_row_hi = -1;
        }
        {
            long long _cnt = (v[8].ival >= v[7].ival) ? v[8].ival - v[7].ival + 1 : 0, _tot;
            g_lower_pas.sem.pas_pend_esz = (g_lower_pas.sem.pas_pend_esz >= 0 && _cnt > 0) ? (__builtin_mul_overflow(g_lower_pas.sem.pas_pend_esz, _cnt, &_tot) ? (1LL << 62) : _tot) : -1;
            int _eic = g_lower_pas.sem.pas_pend_ischar;
            if (_eic &&
                (!v[1].ival || !g_lower_pas.sem.pas_pend_typename || strcmp(g_lower_pas.sem.pas_pend_typename, "char") || v[7].ival != 1 || v[8].ival < 2 || g_lower_pas.sem.pas_last_const_enum))
                _eic |= PAS_CA_NOSTR;
            int _wr = g_lower_pas.sem.pas_pend_arr_ischar || (g_lower_pas.sem.pas_pend_typename && pas_arrtype_ischar(g_lower_pas.sem.pas_pend_typename));
            g_lower_pas.sem.pas_pend_arr_wrap = _wr;
            g_lower_pas.sem.pas_pend_ptrtarget = NULL;
            g_lower_pas.sem.pas_pend_arr_ncols = -1;
            g_lower_pas.sem.pas_pend_arr_ischar = _eic;
            g_lower_pas.sem.pas_pend_isarr = 1;
            if (v[8].ival >= v[7].ival) { g_lower_pas.sem.pas_pend_sub_low = v[7].ival; g_lower_pas.sem.pas_pend_sub_high = v[8].ival; }
            out.ival = (v[4].ival >= 0 || v[8].ival < v[7].ival) ? v[4].ival : v[8].ival;
            g_lower_pas.sem.pas_pend_row_lo = _rlo;
            g_lower_pas.sem.pas_pend_row_hi = _rhi;
            g_lower_pas.sem.pas_pend_row_kind = _rkind;
            g_lower_pas.sem.pas_pend_dyn = 0;
            if (_rhi >= _rlo) { g_lower_pas.sem.pas_pend_ischar = 0; g_lower_pas.sem.pas_pend_arr_ischar = 0; g_lower_pas.sem.pas_pend_arr_wrap = 0; g_lower_pas.sem.pas_pend_isbool = 0; }
            const pty_t *_ft = pty_of_src(n);
            if (_ft && _ft->fold) {
                long long _r0 = _ft->ix[0]->lo, _r1 = _ft->ix[0]->hi, _c0 = _ft->ix[1]->lo, _c1 = _ft->ix[1]->hi;
                g_lower_pas.sem.pas_pend_arr_ncols = _c1 - _c0 + 1;
                g_lower_pas.sem.pas_pend_row_lo = 0;
                g_lower_pas.sem.pas_pend_row_hi = -1;
                g_lower_pas.sem.pas_pend_row_kind = 0;
                out.ival = (_r1 - _r0 + 1) * (_c1 - _c0 + 1) - 1;
            }
        }
        return out;
    }
    if (IS(n, TT_ARRAY_TYPE)) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        v[4] = E_simple_type(n->c[off]);
        long long _rlo = pas_index_low_of(n->c[off]);
        if (v[4].ival < 0) { _rlo = pas_index_bound(-1, 0); v[4].ival = pas_index_bound(-1, 1); }
        v[6] = E_simple_type(n->c[off + 1]);
        long long _clo = pas_index_low_of(n->c[off + 1]);
        if (v[6].ival < 0) { _clo = pas_index_bound(-1, 0); v[6].ival = pas_index_bound(-1, 1); }
        v[9] = E_type(n->c[off + 2]);
        {
            g_lower_pas.sem.pas_pend_esz = -1;
            int _eic = g_lower_pas.sem.pas_pend_ischar;
            g_lower_pas.sem.pas_pend_ptrtarget = NULL;
            long long r = v[4].ival;
            long long c = v[6].ival;
            g_lower_pas.sem.pas_pend_arr_ncols = c - _clo + 1;
            g_lower_pas.sem.pas_pend_arr_ischar = _eic;
            g_lower_pas.sem.pas_pend_isarr = 1;
            g_lower_pas.sem.pas_pend_dyn = 0;
            out.ival = (r - _rlo + 1) * (c - _clo + 1) - 1;
        }
        return out;
    }
    if (IS(n, TT_RECORD)) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        { g_lower_pas.sem.pas_recbody_depth++; if (g_lower_pas.sem.pas_recbody_depth > 1) pas_pend_nest_save(); }
        v[4] = E_record_body(n->c[off]);
        g_lower_pas.sem.pas_pend_recnode = n;
        if (g_lower_pas.dump_types)
            pty_lay_check(n, g_lower_pas.sem.pas_pend_nf, g_lower_pas.sem.pas_pend_fields, g_lower_pas.sem.pas_pend_fldfile, g_lower_pas.sem.pas_pend_fldptrto, g_lower_pas.sem.pas_pend_fldrec,
            g_lower_pas.sem.pas_pend_fldenum);
        {
            if (g_lower_pas.sem.pas_recbody_depth > 1) {
                char _anm[32];
                snprintf(_anm, sizeof _anm, "$anonrec%d", ++g_lower_pas.sem.pas_scope.anonseq);
                pty_note_recname(ct_strdup(_anm), n);
                pas_pend_nest_restore();
                g_lower_pas.sem.pas_pend_typename = ct_strdup(_anm);
            }
            g_lower_pas.sem.pas_recbody_depth--;
            g_lower_pas.sem.pas_pend_ptrtarget = NULL;
            g_lower_pas.sem.pas_pend_sub_low = 0;
            g_lower_pas.sem.pas_pend_sub_high = -1;
            g_lower_pas.sem.pas_pend_enum_max = -1;
            g_lower_pas.sem.pas_pend_arr_ncols = -1;
            g_lower_pas.sem.pas_pend_ischar = 0;
            g_lower_pas.sem.pas_pend_arr_ischar = 0;
            g_lower_pas.sem.pas_pend_esz = (g_lower_pas.sem.pas_recbody_depth == 0 && g_lower_pas.sem.pas_pend_nf == 0) ? 0 : -1;
            out.ival = v[1].ival ? -4 : -1;
        }
        return out;
    }
    if (IS(n, TT_SET_TYPE)) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        v[4] = E_simple_type(n->c[off]);
        { g_lower_pas.sem.pas_pend_esz = -1; g_lower_pas.sem.pas_pend_ptrtarget = NULL; g_lower_pas.sem.pas_pend_set_hi = v[4].ival; out.ival = -2; }
        return out;
    }
    if (IS(n, TT_FILE_TYPE) && n->n - ((n->n > 0 && n->c[0] && n->c[0]->t == TT_KEYWORD) ? 1 : 0) == 0) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        { g_lower_pas.sem.pas_pend_esz = -1; g_lower_pas.sem.pas_pend_ptrtarget = NULL; out.ival = -1; }
        return out;
    }
    if (IS(n, TT_FILE_TYPE)) {
        v[1] = E_packed_opt(off ? n->c[0] : NULL);
        v[4] = E_type(n->c[off]);
        {
            int _cf = g_lower_pas.sem.pas_pend_istfile ||
                (g_lower_pas.sem.pas_pend_typename && (!strcmp(g_lower_pas.sem.pas_pend_typename, "text") || pas_rectype_has_file(g_lower_pas.sem.pas_pend_typename)));
            for (int _i = 0; !_cf && _i < g_lower_pas.sem.pas_pend_nf; _i++) if (g_lower_pas.sem.pas_pend_fldfile[_i]) _cf = 1;
            if (_cf) {
                fprintf(stderr,
                    "pascal: ISO 7185 6.4.3.5 violation: the component-type of a file-type shall not be a file-type nor a structured-type having a file-type as a component -- 'file of %s'\n",
                    g_lower_pas.sem.pas_pend_typename ? g_lower_pas.sem.pas_pend_typename : "<file>");
                g_lower_pas.sem.pas_iso_errors++;
            }
            if (g_lower_pas.sem.pas_pend_sub_high >= 0) {
                g_lower_pas.sem.pas_pend_frng_lo = g_lower_pas.sem.pas_pend_sub_low;
                g_lower_pas.sem.pas_pend_frng_hi = g_lower_pas.sem.pas_pend_sub_high;
            } else if (g_lower_pas.sem.pas_pend_typename && pas_subtype_high(g_lower_pas.sem.pas_pend_typename) >= 0) {
                g_lower_pas.sem.pas_pend_frng_lo = pas_subtype_low(g_lower_pas.sem.pas_pend_typename);
                g_lower_pas.sem.pas_pend_frng_hi = pas_subtype_high(g_lower_pas.sem.pas_pend_typename);
            }
            g_lower_pas.sem.pas_pend_frng_ischar = g_lower_pas.sem.pas_pend_ischar;
            pas_pend_reset();
            g_lower_pas.sem.pas_pend_esz = -1;
            g_lower_pas.sem.pas_pend_istfile = 1;
            out.ival = -1;
        }
        return out;
    }
    if (n != NULL) {
        v[1] = E_simple_type(n);
        {
            if (g_lower_pas.sem.pas_pend_ptrtarget) {
                out.ival = -3;
            } else if (v[1].ival == -2) {
                out.ival = -2;
            } else if (v[1].ival >= 0 && g_lower_pas.sem.pas_pend_typename && pas_arrtype_high(g_lower_pas.sem.pas_pend_typename) >= 0) {
                long long _tnc = pas_arrtype_ncols(g_lower_pas.sem.pas_pend_typename);
                if (_tnc >= 0) g_lower_pas.sem.pas_pend_arr_ncols = _tnc;
                g_lower_pas.sem.pas_pend_dyn = pas_arrtype_isdyn(g_lower_pas.sem.pas_pend_typename);
                g_lower_pas.sem.pas_pend_arr_wrap = pas_arrtype_wrap(g_lower_pas.sem.pas_pend_typename);
                long long _rlo, _rhi;
                int _rk = pas_rows_get(g_lower_pas.sem.pas_pend_typename, 1, &_rlo, &_rhi);
                if (_rk) {
                    g_lower_pas.sem.pas_pend_row_lo = _rlo;
                    g_lower_pas.sem.pas_pend_row_hi = _rhi;
                    g_lower_pas.sem.pas_pend_row_kind = _rk - 1;
                    g_lower_pas.sem.pas_pend_ischar = 0;
                    g_lower_pas.sem.pas_pend_arr_ischar = 0;
                    g_lower_pas.sem.pas_pend_isbool = 0;
                }
                out.ival = v[1].ival;
            } else {
                out.ival = -1;
            }
        }
        return out;
    }
    elab_bad("type", n);
    return out;
}
static pval E_type_decl(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "type")) {
        v[1].str = tok_s(n->c[0]);
        v[3] = E_type(n->c[1]);
        {
            pas_scope_define(v[1].str);
            pas_type_not_self_applied(v[1].str);
            pas_tsz_add(v[1].str, g_lower_pas.sem.pas_pend_esz);
            if (g_lower_pas.sem.pas_pend_ischar && g_lower_pas.sem.pas_pend_sub_high >= 0 && !g_lower_pas.sem.pas_pend_isarr && g_lower_pas.sem.pas_pend_nf == 0) pas_typealias_add(v[1].str, "char");
            long long _ty3 = (v[3].ival == -4) ? -1 : v[3].ival;
            int _pk3 = (v[3].ival == -4) || (g_lower_pas.sem.pas_pend_typename && pas_rectype_is_packed(g_lower_pas.sem.pas_pend_typename));
            if (_ty3 < 0 && g_lower_pas.sem.pas_pend_istfile) pas_tfiletype_add(v[1].str);
            if (_ty3 < 0 && _ty3 != -2 && _ty3 != -3 && g_lower_pas.sem.pas_pend_isbool) pas_booltype_add(v[1].str);
            if (_ty3 == -2) pas_settype_add(v[1].str, g_lower_pas.sem.pas_pend_set_hi);
            if (g_lower_pas.sem.pas_pend_ptrtarget) pas_ptrtype_add(v[1].str, g_lower_pas.sem.pas_pend_ptrtarget);
            if (g_lower_pas.sem.pas_pend_enum_max >= 0) { pas_enumtype_add(v[1].str, g_lower_pas.sem.pas_pend_enum_max); pas_enumnames_add(v[1].str, g_lower_pas.sem.pas_pend_enum_names); }
            if (g_lower_pas.sem.pas_pend_sub_high >= 0 && _ty3 < 0 && g_lower_pas.sem.pas_pend_arr_ncols < 0)
                pas_subtype_add(v[1].str, g_lower_pas.sem.pas_pend_sub_low, g_lower_pas.sem.pas_pend_sub_high);
            if (g_lower_pas.sem.pas_pend_proctype) pas_proctype_add(v[1].str, g_lower_pas.sem.pas_pend_proctype);
            for (int _fi = 0; _fi < g_lower_pas.sem.pas_pend_nf; _fi++) {
                const char *_ft = g_lower_pas.sem.pas_pend_fldtypename[_fi];
                if (_ft && !strncmp(_ft, "$anonproc", 9)) pas_proctype_add(pas_pf_cat3(v[1].str, ".", g_lower_pas.sem.pas_pend_fields[_fi]), pas_proctype_get(_ft));
            }
            if (_ty3 >= 0 && !g_lower_pas.sem.pas_pend_ptrtarget && g_lower_pas.sem.pas_pend_row_hi >= g_lower_pas.sem.pas_pend_row_lo)
                pas_rows_add(v[1].str, g_lower_pas.sem.pas_pend_row_lo, g_lower_pas.sem.pas_pend_row_hi, 1, g_lower_pas.sem.pas_pend_row_kind);
            if (_ty3 == -1 && !g_lower_pas.sem.pas_pend_ptrtarget && g_lower_pas.sem.pas_pend_nf == 0 && g_lower_pas.sem.pas_pend_enum_max < 0 && g_lower_pas.sem.pas_pend_sub_high < 0 &&
                g_lower_pas.sem.pas_pend_typename) pas_typealias_add(v[1].str, g_lower_pas.sem.pas_pend_typename);
            pas_pend_reset();
        }
        return out;
    }
    elab_bad("type_decl", n);
    return out;
}
pval E_type_decl_list(const tree_t *n, int from, int to) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) { const tree_t *ce = n->c[i]; if (i == from) { v[1] = E_type_decl(ce); } else { v[1] = out; v[2] = E_type_decl(ce); } }
    return out;
}
pval E_var_decl(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "var")) {
        v[1] = E_id_list(n->c[0], 0, n->c[0]->n);
        v[3] = E_type(n->c[1]);
        {
            pas_scope_define_list(v[1].list);
            if (v[1].list && g_lower_pas.sem.pas_pend_esz > 2147483647LL) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) {
                int _vln = (i < n->c[0]->n && n->c[0]->c[i] && n->c[0]->c[i]->line > 0) ? n->c[0]->c[i]->line : n->line;
                fprintf(stderr, "pascal: line %d: the variable '%s' needs %lld bytes of storage, more than the 2 GiB a static variable can have (fpc cannot link one either)\n", _vln,
                    v[1].list->items[i]->v.sval, g_lower_pas.sem.pas_pend_esz);
                g_lower_pas.sem.pas_limit_errors++;
            }
            if (v[1].list) for (int i = 0; i < v[1].list->count; i++) if (v[1].list->items[i] && v[1].list->items[i]->v.sval) pas_tsz_add(v[1].list->items[i]->v.sval, g_lower_pas.sem.pas_pend_esz);
            long long _ty3 = (v[3].ival == -4) ? -1 : v[3].ival;
            int _pk3 = (v[3].ival == -4) || (g_lower_pas.sem.pas_pend_typename && pas_rectype_is_packed(g_lower_pas.sem.pas_pend_typename));
            if (_ty3 != -3 && g_lower_pas.sem.pas_pend_nf > 0 && !pas_pend_is_rectype(g_lower_pas.sem.pas_pend_typename) && !g_lower_pas.sem.pas_pend_ptrtarget) {
                char _anm[32];
                snprintf(_anm, sizeof _anm, "$anonrec%d", ++g_lower_pas.sem.pas_scope.anonseq);
                pty_note_recname(ct_strdup(_anm), g_lower_pas.sem.pas_pend_recnode);
                g_lower_pas.sem.pas_pend_typename = ct_strdup(_anm);
            }
            if (v[1].list) for (int i = 0; i < v[1].list->count; i++) {
                tree_t *id = v[1].list->items[i];
                if (id && id->v.sval) {
                    if (_ty3 == -3) {
                        pas_scalarvartype_add(id->v.sval, g_lower_pas.sem.pas_pend_typename && pas_ptrtype_target(g_lower_pas.sem.pas_pend_typename) ? g_lower_pas.sem.pas_pend_typename : "^");
                    } else {
                        if (_ty3 >= 0 && g_lower_pas.sem.pas_pend_nf <= 0) {
                            long long _varnc = (g_lower_pas.sem.pas_pend_arr_ncols >= 0) ? g_lower_pas.sem.pas_pend_arr_ncols : pas_arrtype_ncols(g_lower_pas.sem.pas_pend_typename);
                            if (!(g_lower_pas.sem.pas_pend_ischar && !g_lower_pas.sem.pas_pend_arr_ischar && _varnc < 0 && g_lower_pas.sem.pas_pend_nf == 0) && _varnc < 0) {
                                if (g_lower_pas.sem.pas_pend_row_hi >= g_lower_pas.sem.pas_pend_row_lo)
                                    pas_rows_add(id->v.sval, g_lower_pas.sem.pas_pend_row_lo, g_lower_pas.sem.pas_pend_row_hi, 0, g_lower_pas.sem.pas_pend_row_kind);
                                int _aic = !(g_lower_pas.sem.pas_pend_row_hi >= g_lower_pas.sem.pas_pend_row_lo) &&
                                    (g_lower_pas.sem.pas_pend_arr_ischar || (g_lower_pas.sem.pas_pend_typename && pas_arrtype_ischar(g_lower_pas.sem.pas_pend_typename)));
                                if (_aic && g_lower_pas.sem.pas_pend_arr_wrap)
                                    pas_strarr_add2(id->v.sval,
                                    (g_lower_pas.sem.pas_pend_typename && pas_arrtype_lo(g_lower_pas.sem.pas_pend_typename) > 0) ? pas_arrtype_lo(g_lower_pas.sem.pas_pend_typename) : 1);
                                else if (_aic) {
                                    pas_chararr_add2(id->v.sval,
                                        g_lower_pas.sem.pas_pend_arr_ischar ? (g_lower_pas.sem.pas_pend_sub_low > 0 ? g_lower_pas.sem.pas_pend_sub_low : 0) :
                                        pas_arrtype_lo(g_lower_pas.sem.pas_pend_typename));
                                    pas_chararr_shape(id->v.sval,
                                        (g_lower_pas.sem.pas_pend_arr_ischar & PAS_CA_NOSTR) || (g_lower_pas.sem.pas_pend_typename && pas_arrtype_nostr(g_lower_pas.sem.pas_pend_typename)),
                                        g_lower_pas.sem.pas_pend_arr_ischar ? g_lower_pas.sem.pas_pend_sub_high : pas_arrtype_high(g_lower_pas.sem.pas_pend_typename));
                                }
                            }
                        }
                        if (_ty3 < 0 && g_lower_pas.sem.pas_pend_istfile) {
                            pas_tfcomp_add(id->v.sval, g_lower_pas.sem.pas_pend_frng_lo, g_lower_pas.sem.pas_pend_frng_hi, g_lower_pas.sem.pas_pend_frng_ischar);
                        }
                        if (_ty3 < 0 && !g_lower_pas.sem.pas_pend_istfile && g_lower_pas.sem.pas_pend_nf == 0 && !g_lower_pas.sem.pas_pend_isarr && g_lower_pas.sem.pas_pend_sub_high >= 0) {
                            pas_subvar_add(id->v.sval, g_lower_pas.sem.pas_pend_sub_low, g_lower_pas.sem.pas_pend_sub_high);
                        }
                            else if (_ty3 < 0 && !g_lower_pas.sem.pas_pend_istfile && g_lower_pas.sem.pas_pend_nf == 0 && !g_lower_pas.sem.pas_pend_isarr && g_lower_pas.sem.pas_pend_typename &&
                            pas_subtype_high(g_lower_pas.sem.pas_pend_typename) >= 0) {
                            pas_subvar_add(id->v.sval, pas_subtype_low(g_lower_pas.sem.pas_pend_typename), pas_subtype_high(g_lower_pas.sem.pas_pend_typename));
                        }
                        pas_scalarvartype_add(id->v.sval, g_lower_pas.sem.pas_pend_typename ? g_lower_pas.sem.pas_pend_typename : "?");
                    }
                    pas_local_add(id->v.sval);
                }
            }
            {
                const char *_psig = g_lower_pas.sem.pas_pend_proctype ? g_lower_pas.sem.pas_pend_proctype :
                    (g_lower_pas.sem.pas_pend_typename ? pas_proctype_get(g_lower_pas.sem.pas_pend_typename) : NULL);
                int _np = v[1].list ? v[1].list->count : 0;
                for (int _pi = 0; _psig && _pi < _np; _pi++) if (v[1].list->items[_pi] && v[1].list->items[_pi]->v.sval) pas_pf_variable(v[1].list->items[_pi]->v.sval, _psig, v[1].list);
            }
            g_lower_pas.sem.pas_pend_frng_lo = 0;
            g_lower_pas.sem.pas_pend_frng_hi = -1;
            g_lower_pas.sem.pas_pend_frng_ischar = 0;
            pas_pend_reset();
            out.list = v[1].list;
        }
        return out;
    }
    elab_bad("var_decl", n);
    return out;
}
