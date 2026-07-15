#include "rt.h"
#include <alloca.h>
#include "gc_heap.h"
#include "core.h"
#include "descr.h"
#include "sil_macros.h"
#include "bb_pool.h"
#include "bb_box.h"
#include "bb_pool.h"
#include "bb_build.h"
#include "ast.h"
#include "SM.h"
#include "../../parser/prolog/prolog_atom.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "zeta_alloc.h"
#include "zeta_heap.h"
#include "zeta_choices.h"
extern const char *Σ;
extern int Σlen;
#define STACKLESS_ABORT(fn) \
    do { fprintf(stderr, "libscrip_rt: %s called — Icon value stack removed (GROUND ZERO 3). " \
                         "This box must be rebuilt stackless (per-box slot, no value stack).\n", (fn)); \
         abort(); } while (0)
#include <string.h>
#include <math.h>
extern void    core_lib_init(void);
extern int     rt_zeta_cstack(void);
extern int     exec_stmt(const char *subj_name, DESCR_t *subj_var,
                         DESCR_t pat, DESCR_t *repl, int has_repl);
extern DESCR_t NV_GET_fn(const char *name);
extern DESCR_t NV_SET_fn(const char *name, DESCR_t val);
extern DESCR_t *NV_PTR_fn(const char *name);
extern int is_protected_pat_name(const char *name);
extern int g_call_fastpath_off;
extern DESCR_t NAME_fn(const char *varname);
extern char   *VARVAL_fn(DESCR_t v);
extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
extern int     junction_is(DESCR_t v);
extern int     junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric);
extern void    register_fn(const char *name,
                           DESCR_t (*fn)(DESCR_t *, int),
                           int min_args, int max_args);
extern DESCR_t pat_lit(const char *s);
extern DESCR_t pat_span(const char *chars);
extern DESCR_t pat_break_(const char *chars);
extern DESCR_t pat_any_cs(const char *chars);
extern DESCR_t pat_notany(const char *chars);
extern DESCR_t pat_len(int64_t n);
extern DESCR_t pat_pos(int64_t n);
extern DESCR_t pat_rpos(int64_t n);
extern DESCR_t pat_tab(int64_t n);
extern DESCR_t pat_rtab(int64_t n);
extern DESCR_t pat_arb(void);
extern DESCR_t pat_arbno(DESCR_t inner);
extern DESCR_t pat_rem(void);
extern DESCR_t pat_fence(void);
extern DESCR_t pat_fence_p(DESCR_t inner);
extern DESCR_t pat_fail(void);
extern DESCR_t pat_abort(void);
extern DESCR_t pat_succeed(void);
extern DESCR_t pat_bal(void);
extern DESCR_t pat_epsilon(void);
extern DESCR_t pat_cat(DESCR_t left, DESCR_t right);
extern DESCR_t pat_alt(DESCR_t left, DESCR_t right);
extern DESCR_t pat_ref(const char *name);
extern DESCR_t pat_assign_imm(DESCR_t child, DESCR_t var);
extern DESCR_t pat_assign_cond(DESCR_t child, DESCR_t var);
extern DESCR_t pat_at_cursor(const char *varname);
extern DESCR_t pat_user_call(const char *name, DESCR_t *args, int nargs);
extern DESCR_t (*g_user_call_hook)(const char *, DESCR_t *, int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_case_eq(const DESCR_t *sel, const DESCR_t *key)
{
    if (!sel || !key) return 0;
    if (sel->v == DT_I && key->v == DT_I) return sel->i == key->i;
    { const char *ss = VARVAL_fn(*sel); const char *ks = VARVAL_fn(*key); if (ss && ks) return strcmp(ss, ks) == 0; return ss == ks; }
}
#define EXPRESSION_REG_MAX 256
typedef struct { const char *name; void *fn; } ExpressionRegEntry;
static ExpressionRegEntry g_expression_reg[EXPRESSION_REG_MAX];
static int           g_expression_reg_count = 0;
#include "IR.h"
#include "../builtins/resolution.h"
extern cap_t *bb_cap_new(bb_box_fn child_fn, void *child_state, const char *varname, DESCR_t *var_ptr, int immediate);
extern cap_t *bb_cap_new_call(bb_box_fn child_fn, void *child_state, const char *fnc_name, DESCR_t *fnc_args, int fnc_nargs, char **fnc_arg_names, int fnc_n_arg_names, int immediate);
extern void *bb_arbno_new(void *fn, void *state);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_nv_cstr(const char *name)
{
    const char *s = VARVAL_fn(NV_GET_fn(name ? name : ""));
    return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_str(const char *name, const char *str)
{
    DESCR_t d;
    rt_gc_point((DESCR_t *)0, &str);
    d.v    = DT_S;
    d.s    = (char *)(str ? str : "");
    d.slen = (uint32_t)strlen(d.s);
    NV_SET_fn(name ? name : "", d);
    if (g_monitor_bin) mon_emit_value_bin(name ? name : "", d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_indirect_assign_str(const char *holder, const char *str)
{
    rt_gvar_assign_str(rt_nv_cstr(holder ? holder : ""), str);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_indirect_assign_var(const char *holder, const char *val_name)
{
    const char *target = rt_nv_cstr(holder ? holder : "");
    DESCR_t val = NV_GET_fn(val_name ? val_name : "");
    NV_SET_fn(target ? target : "", val);
    if (g_monitor_bin) mon_emit_value_bin(target ? target : "", val);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_pat(const char *name, void *head)
{
    extern void *dtp_wrap_fn(void *);
    DESCR_t d;
    d.v    = DT_P;
    d.slen = 0;
    d.p    = dtp_wrap_fn(head);
    NV_SET_fn(name ? name : "", d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_int(const char *name, int64_t val)
{
    DESCR_t d;
    rt_gc_point((DESCR_t *)0, (const char **)0);
    d.v    = DT_I;
    d.slen = 0;
    d.i    = val;
    NV_SET_fn(name ? name : "", d);
    if (g_monitor_bin) mon_emit_value_bin(name ? name : "", d);
}
extern DESCR_t binop_apply(int op, DESCR_t lv, DESCR_t rv, int *rel_fail);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int64_t rt_gvar_arith(const char *a, const char *b, int op)
{
    DESCR_t lv = NV_GET_fn(a ? a : "");
    DESCR_t rv = NV_GET_fn(b ? b : "");
    int rel_fail = 0;
    DESCR_t r = binop_apply(op, lv, rv, &rel_fail);
    if (r.v == DT_I) return r.i;
    if (r.v == DT_R) return (int64_t)r.r;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int64_t rt_relop_descr2(int64_t l_lo, int64_t l_hi, int64_t r_lo, int64_t r_hi, int op)
{
    DESCR_t lv; DESCR_t rv;
    union { int64_t q; struct { DTYPE_t v; uint32_t slen; } f; } ul; union { int64_t q; struct { DTYPE_t v; uint32_t slen; } f; } ur;
    ul.q = l_lo; lv.v = ul.f.v; lv.slen = ul.f.slen; lv.i = l_hi;
    ur.q = r_lo; rv.v = ur.f.v; rv.slen = ur.f.slen; rv.i = r_hi;
    int rel_fail = 0;
    binop_apply(op, lv, rv, &rel_fail);
    return rel_fail ? 0 : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int64_t rt_gvar_get_int(const char *name)
{
    DESCR_t v = NV_GET_fn(name ? name : "");
    if (v.v == DT_I) return v.i;
    if (v.v == DT_R) return (int64_t)v.r;
    if (v.v == DT_S && v.s) return (int64_t)strtoll(v.s, NULL, 10);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_gvar_get_descr(const char *name)
{
    return NV_GET_fn(name ? name : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_var(const char *dst, const char *src)
{
    DESCR_t d;
    rt_gc_point((DESCR_t *)0, (const char **)0);
    d = NV_GET_fn(src ? src : "");
    NV_SET_fn(dst ? dst : "", d);
    if (g_monitor_bin) mon_emit_value_bin(dst ? dst : "", d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_descr(const char *name, int64_t lo, int64_t hi)
{
    DESCR_t d;
    union { int64_t q; struct { DTYPE_t v; uint32_t slen; } f; } u;
    u.q    = lo;
    d.v    = u.f.v;
    d.slen = u.f.slen;
    d.i    = hi;
    rt_gc_point(&d, (const char **)0);
    NV_SET_fn(name ? name : "", d);
    if (g_monitor_bin) mon_emit_value_bin(name ? name : "", d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rt_coerce_errmsg(int code) {
    switch (code) {
    case  59: return "any argument is not a string or expression";
    case  69: return "break argument is not a string or expression";
    case  70: return "breakx argument is not a string or expression";
    case 151: return "notany argument is not a string or expression";
    case 188: return "span argument is not a string or expression";
    case 120: return "len argument is not integer or expression";
    case 162: return "pos argument is not integer or expression";
    case 181: return "rtab argument is not integer or expression";
    case 183: return "tab argument is not integer or expression";
    case 185: return "rpos argument is not integer or expression";
    case 121: return "len argument is negative or too large";
    case 163: return "pos argument is negative or too large";
    case 182: return "rtab argument is negative or too large";
    case 184: return "tab argument is negative or too large";
    case 186: return "rpos argument is negative or too large";
    case 101: return "eq first argument is not numeric";  case 102: return "eq second argument is not numeric";
    case 109: return "ge first argument is not numeric";  case 110: return "ge second argument is not numeric";
    case 111: return "gt first argument is not numeric";  case 112: return "gt second argument is not numeric";
    case 118: return "le first argument is not numeric";  case 119: return "le second argument is not numeric";
    case 147: return "lt first argument is not numeric";  case 148: return "lt second argument is not numeric";
    case 149: return "ne first argument is not numeric";  case 150: return "ne second argument is not numeric";
    case 122: return "leq first argument is not a string"; case 123: return "leq second argument is not a string";
    case 124: return "lge first argument is not a string"; case 125: return "lge second argument is not a string";
    case 126: return "lgt first argument is not a string"; case 127: return "lgt second argument is not a string";
    case 128: return "lle first argument is not a string"; case 129: return "lle second argument is not a string";
    case 130: return "llt first argument is not a string"; case 131: return "llt second argument is not a string";
    case 132: return "lne first argument is not a string"; case 133: return "lne second argument is not a string";
    default:  return "pattern primitive argument coercion failed"; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_coerce_str_d(const DESCR_t *in, DESCR_t *out, long codes) {
    extern void core_runtime_error(int code, const char *msg);
    int tc = (int)(codes & 0xffff);
    int nc = (int)((codes >> 16) & 0xffff);
    DESCR_t v = *in;
    if (v.v == DT_S && v.s && v.s[0]) { *out = v; out->slen = (uint32_t)strlen(v.s); return; }
    if (v.v == DT_S || v.v == DT_SNUL) { if (nc) core_runtime_error(nc, rt_coerce_errmsg(nc)); out->v = DT_S; out->s = (char *)""; out->slen = 0; return; }
    if (v.v == DT_I || v.v == DT_R) {
        char *s = VARVAL_fn(v);
        if ((!s || !s[0]) && nc) core_runtime_error(nc, rt_coerce_errmsg(nc));
        out->v = DT_S; out->s = s ? s : (char *)""; out->slen = (uint32_t)strlen(out->s); return; }
    if (tc) core_runtime_error(tc, rt_coerce_errmsg(tc));
    out->v = DT_S; out->s = (char *)""; out->slen = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_parse_num_d(const DESCR_t *v, int64_t *iv, double *rv, int *isreal) {
    if (v->v == DT_I) { *iv = v->i; *isreal = 0; return 1; }
    if (v->v == DT_R) { *rv = v->r; *isreal = 1; return 1; }
    if (v->v == DT_SNUL) { *iv = 0; *isreal = 0; return 1; }
    if ((v->v == DT_S || IS_CSET_fn(*v)) && v->s) {
        const char *p = v->s; while (*p == ' ') p++;
        if (!*p) { *iv = 0; *isreal = 0; return 1; }
        { char *ep = NULL; long long t = strtoll(p, &ep, 10);
          if (ep && ep != p) { const char *q = ep; while (*q == ' ') q++; if (!*q) { *iv = (int64_t)t; *isreal = 0; return 1; } } }
        { char *ep = NULL; double d = strtod(p, &ep);
          if (ep && ep != p) { const char *q = ep; while (*q == ' ') q++; if (!*q) { *rv = d; *isreal = 1; return 1; } } } }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_coerce_num2_d(const DESCR_t *self, const DESCR_t *other, DESCR_t *out, long codes) {
    extern void core_runtime_error(int code, const char *msg);
    int ec = (int)(codes & 0xffff);
    int64_t si = 0, oi = 0; double sr = 0, orr = 0; int sreal = 0, oreal = 0;
    if (!rt_parse_num_d(self, &si, &sr, &sreal)) { if (ec) core_runtime_error(ec, rt_coerce_errmsg(ec)); si = 0; sreal = 0; }
    int ook = rt_parse_num_d(other, &oi, &orr, &oreal);
    (void)ook; (void)oi; (void)orr;
    if (sreal || oreal) { out->v = DT_R; out->slen = 0; out->r = sreal ? sr : (double)si; }
    else { out->v = DT_I; out->slen = 0; out->i = si; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_cmp_d(const DESCR_t *a, const DESCR_t *b) {
    if ((a->v == DT_S || a->v == DT_SNUL) && (b->v == DT_S || b->v == DT_SNUL)) {
        int c = strcmp((a->v == DT_S && a->s) ? a->s : "", (b->v == DT_S && b->s) ? b->s : "");
        return (c < 0) ? -1 : (c > 0) ? 1 : 0; }
    if (a->v == DT_I && b->v == DT_I) return (a->i < b->i) ? -1 : (a->i > b->i) ? 1 : 0;
    { double x = (a->v == DT_R) ? a->r : (double)a->i; double y = (b->v == DT_R) ? b->r : (double)b->i;
      return (x < y) ? -1 : (x > y) ? 1 : 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_coerce_int_d(const DESCR_t *in, DESCR_t *out, long codes) {
    extern void core_runtime_error(int code, const char *msg);
    int ec = (int)(codes & 0xffff);
    int en = (int)((codes >> 16) & 0xffff);
    DESCR_t v = *in;
    int64_t r = 0; int ok = 0;
    if (v.v == DT_I) { r = v.i; ok = 1; }
    else if (v.v == DT_R) { double d = v.r; if (d == floor(d) && d >= -9.2e18 && d <= 9.2e18) { r = (int64_t)d; ok = 1; } }
    else if (v.v == DT_SNUL) { r = 0; ok = 1; }
    else if (v.v == DT_S && v.s) {
        if (!v.s[0]) { r = 0; ok = 1; }
        else { const char *p = v.s; while (*p == ' ') p++; char *ep = NULL; long long t = strtoll(p, &ep, 10);
               if (ep && ep != p) { while (*ep == ' ') ep++; if (*ep == 0) { r = (int64_t)t; ok = 1; } } } }
    if (!ok && ec) core_runtime_error(ec, rt_coerce_errmsg(ec));
    if (r < 0 && en) core_runtime_error(en, rt_coerce_errmsg(en));
    out->v = DT_I; out->slen = 0; out->i = r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_coerce_real_d(const DESCR_t *in, DESCR_t *out, long codes) {
    extern void core_runtime_error(int code, const char *msg);
    int ec = (int)(codes & 0xffff);
    DESCR_t v = *in;
    double r = 0.0; int ok = 0;
    if (v.v == DT_R) { r = v.r; ok = 1; }
    else if (v.v == DT_I) { r = (double)v.i; ok = 1; }
    else if (v.v == DT_SNUL) { r = 0.0; ok = 1; }
    else if (v.v == DT_S && v.s) {
        if (!v.s[0]) { r = 0.0; ok = 1; }
        else { const char *p = v.s; while (*p == ' ') p++; char *ep = NULL; double d = strtod(p, &ep);
               if (ep && ep != p) { while (*ep == ' ') ep++; if (*ep == 0) { r = d; ok = 1; } } } }
    if (!ok && ec) core_runtime_error(ec, rt_coerce_errmsg(ec));
    out->v = DT_R; out->slen = 0; out->r = r;
}
extern DESCR_t VARVAL_d_fn(DESCR_t d);
typedef struct { const char *base; long len; } rt_subj_t;
const char *g_subject_dbg_base = 0;
long        g_subject_dbg_len  = -1;
#define RT_FRAME_STACK_MAX 256
#define RT_FRAME_SLOT_MAX  64
typedef struct { DESCR_t slot[RT_FRAME_SLOT_MAX]; int nslots; } rt_frame_t;
static rt_frame_t g_rt_frames[RT_FRAME_STACK_MAX];
static int        g_rt_frame_depth = 0;
int rt_k_level = 1;
#define PROC_FRAME_QWORDS 512
#define CALL_ARGS_MAX     64
typedef struct {
    const char *name; bb_box_fn fn; const char **pnames; int nparams; int frame_nslots; int decl_level; uint64_t byref_mask;
    int frame_bytes; DESCR_t **pcells; DESCR_t *rcell; int cells_done; int is_generator; int dyn_scope; const char *result_name; int is_variadic;
} rt_proc_t;
static rt_proc_t    *g_rt_gen_procs = (rt_proc_t *)0;
static int           g_rt_gen_proc_count = 0;
static int           g_rt_gen_proc_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gen_proc_grow(void) {
    if (g_rt_gen_proc_count < g_rt_gen_proc_cap) return;
    int nc = g_rt_gen_proc_cap ? g_rt_gen_proc_cap * 2 : 64; rt_proc_t *np = (rt_proc_t *)realloc(g_rt_gen_procs, (size_t)nc * sizeof(rt_proc_t));
    if (!np) return; g_rt_gen_procs = np; g_rt_gen_proc_cap = nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_register(const char *name, const char **pnames, int nparams)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++) {
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) {
            if (pnames)  g_rt_gen_procs[i].pnames  = pnames;
            if (nparams) g_rt_gen_procs[i].nparams = nparams;
            g_rt_gen_procs[i].cells_done = 0;
            return;
        }
    }
    rt_gen_proc_grow();
    if (g_rt_gen_proc_count >= g_rt_gen_proc_cap) return;
    rt_proc_t *p = &g_rt_gen_procs[g_rt_gen_proc_count++];
    p->name = name; p->fn = NULL; p->pnames = pnames; p->nparams = nparams; p->frame_nslots = -1; p->decl_level = 0; p->byref_mask = 0;
    p->frame_bytes = 0; p->pcells = (DESCR_t **)0; p->rcell = (DESCR_t *)0; p->cells_done = 0; p->is_generator = 0; p->dyn_scope = 0; p->result_name = (const char *)0; p->is_variadic = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_result_name(const char *name, const char *rname)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_rt_gen_procs[i].result_name = rname; g_rt_gen_procs[i].cells_done = 0; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_dyn_scope(const char *name, int v)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_rt_gen_procs[i].dyn_scope = v ? 1 : 0; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_dyn_scope(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return g_rt_gen_procs[i].dyn_scope;
    return 0;
}
static rt_proc_t *rt_proc_find(const char *name);
void rt_proc_cache_clear(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_enum_count(void) { return g_rt_gen_proc_count; }
const char *rt_proc_enum_name(int i) { return (i >= 0 && i < g_rt_gen_proc_count) ? g_rt_gen_procs[i].name : (const char *)0; }
void rt_proc_reset(void) { g_rt_gen_proc_count = 0; rt_proc_cache_clear(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *g_gva_base = (DESCR_t *)0;
DESCR_t *gva_register(const char **names, DESCR_t *cells, int n) {
    if (!cells) return cells;
    g_gva_base = cells;
    for (int k = 0; k < n; k++) { const char *nm = names ? names[k] : (const char *)0; if (!nm) continue; (void)NV_bind_gva(nm, &cells[k]); }
    return cells;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_GVA_ISLAND_BYTES ((size_t)16u << 20)
DESCR_t *rt_gva_island(int n) {
    extern void *rt_slab_region(size_t);
    static DESCR_t *base = (DESCR_t *)0;
    if (!base) { base = (DESCR_t *)rt_slab_region(RT_GVA_ISLAND_BYTES); if (!base) { fprintf(stderr, "rt_gva_island: island reserve failed\n"); abort(); } }
    if ((size_t)n * sizeof(DESCR_t) > RT_GVA_ISLAND_BYTES) { fprintf(stderr, "rt_gva_island: %d slots exceed the island (raise RT_GVA_ISLAND_BYTES)\n", n); abort(); }
    if (n > 0) memset(base, 0, (size_t)n * sizeof(DESCR_t));
    return base;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_is_registered(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_nparams(const char *name, int nparams)
{
    rt_proc_t *p = rt_proc_find(name);
    if (p) p->nparams = nparams;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_nparams(const char *name)
{
    if (!name) return -1;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return g_rt_gen_procs[i].nparams;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_has_native_fn(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return g_rt_gen_procs[i].fn != (bb_box_fn)0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_get_fn(const char *name)
{
    if (!name) return (void *)0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return (void *)g_rt_gen_procs[i].fn;
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static struct { void *fn; int bytes; } g_blob_fb[512];
static int g_blob_fb_n = 0;
void rt_fn_frame_bytes_register(void *fn, int bytes)
{
    if (!fn || bytes <= 0) return;
    for (int i = 0; i < g_blob_fb_n; i++) if (g_blob_fb[i].fn == fn) { if (bytes > g_blob_fb[i].bytes) g_blob_fb[i].bytes = bytes; return; }
    if (g_blob_fb_n < (int)(sizeof g_blob_fb / sizeof *g_blob_fb)) { g_blob_fb[g_blob_fb_n].fn = fn; g_blob_fb[g_blob_fb_n].bytes = bytes; g_blob_fb_n++; }
}
long rt_fn_frame_bytes(void *fn)
{
    if (!fn) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++) if ((void *)g_rt_gen_procs[i].fn == fn && g_rt_gen_procs[i].frame_bytes > 0) return (long)g_rt_gen_procs[i].frame_bytes;
    for (int i = 0; i < g_blob_fb_n; i++) if (g_blob_fb[i].fn == fn) return (long)g_blob_fb[i].bytes;
    return (long)PROC_FRAME_QWORDS * 8;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_generator(const char *name, int is_gen)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_rt_gen_procs[i].is_generator = is_gen ? 1 : 0; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_variadic(const char *name, int is_var)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_rt_gen_procs[i].is_variadic = is_var ? 1 : 0; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_is_generator(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return g_rt_gen_procs[i].is_generator;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_fn(const char *name, bb_box_fn fn)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_rt_gen_procs[i].fn = fn; return; }
    rt_gen_proc_grow();
    if (g_rt_gen_proc_count >= g_rt_gen_proc_cap) return;
    rt_proc_t *p = &g_rt_gen_procs[g_rt_gen_proc_count++];
    p->name = name; p->fn = fn; p->pnames = NULL; p->nparams = 0; p->frame_nslots = -1; p->decl_level = 0; p->byref_mask = 0;
    p->frame_bytes = 0; p->pcells = (DESCR_t **)0; p->rcell = (DESCR_t *)0; p->cells_done = 0; p->is_generator = 0; p->dyn_scope = 0; p->result_name = (const char *)0; p->is_variadic = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_call_proc(const char *name, int nargs)
{
    (void)name;
    (void)nargs;
    STACKLESS_ABORT("rt_call_proc");
}
DESCR_t g_call_args[CALL_ARGS_MAX];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_arg_stage(int idx, DESCR_t v)
{
    rt_gc_point(&v, (const char **)0);
    if (idx >= 0 && idx < CALL_ARGS_MAX) g_call_args[idx] = v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_frame_bind_args(char *fb, rt_proc_t *p, int nargs)
{
    extern DESCR_t rt_make_list(DESCR_t *args, int nargs);
    int npc = p->nparams; if (npc > CALL_ARGS_MAX) npc = CALL_ARGS_MAX;
    if (p->is_variadic && npc > 0) {
        int fixed = npc - 1;
        for (int i = 0; i < fixed; i++) *(DESCR_t *)(fb + 16 * (i + 1)) = (i < nargs) ? g_call_args[i] : NULVCL;
        int rest = nargs - fixed; if (rest < 0) rest = 0;
        *(DESCR_t *)(fb + 16 * (fixed + 1)) = rt_make_list(rest > 0 ? &g_call_args[fixed] : (DESCR_t *)0, rest);
        return;
    }
    for (int i = 0; i < nargs; i++) *(DESCR_t *)(fb + 16 * (i + 1)) = g_call_args[i];
    for (int i = nargs; i < npc; i++) *(DESCR_t *)(fb + 16 * (i + 1)) = NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_g_ret_by_name = 0;
int rt_g_want_name = 0;
static DESCR_t rt_nret_fix(DESCR_t r, int wn) { if (rt_g_ret_by_name) { rt_g_ret_by_name = 0; if (!wn && r.v == DT_N) { extern DESCR_t rt_deref(DESCR_t); r = rt_deref(r); } } rt_g_want_name = wn; return r; }
/* NCB-1 leaves (defined below, beside the dyn trampolines they were split out of). */
long    rt_proc_call_open(const char *name, int nargs);
void   *rt_frame_prep(void *fb, long fbytes);
DESCR_t rt_proc_call_epilogue(DESCR_t fret);
DESCR_t rt_proc_call_epilogue_γ(void);
DESCR_t rt_proc_call_epilogue_ω(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_proc_descr(const char *name, int nargs)
{
    rt_proc_t *p = (rt_proc_t *)0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { p = &g_rt_gen_procs[i]; break; }
    if (!p || !p->fn) {
        extern void rt_pl_iso_throw_existence_key(const char *);
        fprintf(stderr, "[GZ-10] rt_call_proc_descr: procedure '%s' has no stackless slab\n", name ? name : "(null)");
        rt_pl_iso_throw_existence_key(name ? name : "?");
        return FAILDESCR;
    }
    /* NCB-1: this body is now EXACTLY the sequence the emitted call site will run — open leaf, frame, transfer,
     * epilogue leaf — with the frame still made by C (alloca / zls2) rather than by an rsp bump, and the
     * transfer still a C indirect call rather than an emitted one.  The open leaf selects the protocol, so the
     * dyn_scope delegation to rt_call_named_proc is gone: dyn and lexical now differ only INSIDE the leaves. */
    long fbytes = rt_proc_call_open(name, nargs);
    if (!fbytes) return FAILDESCR;
    char *fb; if (rt_zeta_cstack()) fb = (char *)alloca((size_t)fbytes); else fb = (char *)rt_zls2_push(fbytes);
    DESCR_t (*fn)(void *, int) = (DESCR_t (*)(void *, int))rt_frame_prep((void *)fb, fbytes);
    DESCR_t fret = fn((void *)fb, 0);
    DESCR_t result = rt_proc_call_epilogue(fret);
    if (!rt_zeta_cstack()) rt_zls2_release_to((void *)(fb + fbytes));
    return result;
}
#define RT_INITIAL_MAX 8192
static int64_t g_initial_fired[RT_INITIAL_MAX];
static int     g_initial_fired_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int64_t rt_initial_fire(int64_t site)
{
    for (int i = 0; i < g_initial_fired_n; i++) if (g_initial_fired[i] == site) return 0;
    if (g_initial_fired_n < RT_INITIAL_MAX) g_initial_fired[g_initial_fired_n++] = site;
    return 1;
}
DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **hout)
{
    rt_proc_t *p = (rt_proc_t *)0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { p = &g_rt_gen_procs[i]; break; }
    if (!p || !p->fn) { extern void rt_pl_iso_throw_existence_key(const char *); fprintf(stderr, "[SUSP] rt_proc_call_gen_h: generator '%s' has no stackless slab\n", name ? name : "(null)"); rt_pl_iso_throw_existence_key(name ? name : "?"); if (hout) *hout = (void *)0; return FAILDESCR; }
    int fbytes = (int)(PROC_FRAME_QWORDS * 8); if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    fbytes = (int)(((long)fbytes + 15L) & ~15L);
    long total = 16L + (long)fbytes;
    if (rt_zeta_mode() == ZC_ZETA_ZH) {
        void *ub = (void *)0; unsigned h = rt_zh_alloc(total, &ub);
        char *base = (char *)ub; char *fb = base + 16;
        ((void **)base)[0] = (void *)p->fn;
        ((long *)base)[1] = total;
        { DESCR_t *zf = (DESCR_t *)fb; for (int zi = 0; zi < fbytes / 16; zi++) zf[zi] = NULVCL; }
        if (nargs > CALL_ARGS_MAX) nargs = CALL_ARGS_MAX;
        rt_frame_bind_args(fb, p, nargs);
        if (hout) *hout = (void *)(uintptr_t)h;
        rt_k_level++; (void)p->fn((void *)fb, 0); rt_k_level--;
        fb = (char *)rt_zh_deref(h) + 16;
        DESCR_t result = *(DESCR_t *)(fb + 0);
        rt_zh_unpin(h);
        if (IS_FAIL(result)) { if (hout) *hout = (void *)0; rt_zh_mark_dead(h); }
        return result;
    }
    char *base = (char *)rt_zls_alloc(total);
    char *fb = base + 16;
    ((void **)base)[0] = (void *)p->fn;
    ((long *)base)[1] = total;
    { DESCR_t *zf = (DESCR_t *)fb; for (int zi = 0; zi < fbytes / 16; zi++) zf[zi] = NULVCL; }
    if (nargs > CALL_ARGS_MAX) nargs = CALL_ARGS_MAX;
    rt_frame_bind_args(fb, p, nargs);
    if (hout) *hout = (void *)fb;
    rt_k_level++; (void)p->fn((void *)fb, 0); rt_k_level--;
    DESCR_t result = *(DESCR_t *)(fb + 0);
    if (IS_FAIL(result)) { if (hout) *hout = (void *)0; ((void **)base)[0] = (void *)0; rt_zls_release((void *)base); }
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_resume_frame(void *frame)
{
    char *fb = (char *)frame;
    if (!fb) return FAILDESCR;
    char *base = fb - 16;
    bb_box_fn fn = (bb_box_fn)((void **)base)[0];
    long total = ((long *)base)[1];
    if (!fn) return FAILDESCR;
    rt_k_level++; (void)fn((void *)fb, 1); rt_k_level--;
    DESCR_t result = *(DESCR_t *)(fb + 0);
    if (IS_FAIL(result)) { (void)total; ((void **)base)[0] = (void *)0; rt_zls_release((void *)base); }
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_resume_frame_h(void **hslot)
{
    void *frame = hslot ? *hslot : (void *)0;
    if (!frame) return FAILDESCR;
    if (rt_zeta_mode() == ZC_ZETA_ZH) {
        unsigned h = (unsigned)(uintptr_t)frame;
        rt_zh_pin(h);
        char *fb = (char *)rt_zh_deref(h) + 16;
        bb_box_fn fn = (bb_box_fn)((void **)(fb - 16))[0];
        if (!fn) { rt_zh_unpin(h); return FAILDESCR; }
        rt_k_level++; (void)fn((void *)fb, 1); rt_k_level--;
        fb = (char *)rt_zh_deref(h) + 16;
        DESCR_t result = *(DESCR_t *)(fb + 0);
        rt_zh_unpin(h);
        if (IS_FAIL(result)) { rt_zh_mark_dead(h); *hslot = (void *)0; }
        return result;
    }
    DESCR_t result = rt_proc_resume_frame(frame);
    if (IS_FAIL(result) && hslot) *hslot = (void *)0;
    return result;
}
typedef struct { const char *name; DESCR_t *cell; DESCR_t old; } NameSaveEnt;
#define PROC_FRAME_NEST_QWORDS 512
#define DCR_CELL_CACHE_SIZE 2048
#define DCR_CELL_CACHE_MASK (DCR_CELL_CACHE_SIZE - 1)
static NameSaveEnt   *g_name_save = (NameSaveEnt *)0;
static int            g_name_save_top = 0;
static int            g_name_save_cap = 0;
static struct { const char *name; DESCR_t *cell; int valid; } g_cell_cache[DCR_CELL_CACHE_SIZE];
static int            g_proc_idx_slot[DCR_CELL_CACHE_SIZE];
static const char    *g_proc_idx_key[DCR_CELL_CACHE_SIZE];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_call_fastpath_ok(void) { return !g_call_fastpath_off; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_name_side_effecting(const char *nm)
{
    static const char *S[] = { "TERMINAL", "ALPHABET", "STCOUNT", "STNO", 0 };
    if (is_protected_pat_name(nm)) return 1;
    for (int i = 0; S[i]; i++) if (strcmp(nm, S[i]) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *rt_cell_for(const char *nm)
{
    unsigned h = (unsigned)(((uintptr_t)nm >> 4) & DCR_CELL_CACHE_MASK);
    if (g_cell_cache[h].valid && g_cell_cache[h].name == nm) return g_cell_cache[h].cell;
    DESCR_t *c = rt_name_side_effecting(nm) ? (DESCR_t *)0 : NV_PTR_fn(nm);
    g_cell_cache[h].name = nm; g_cell_cache[h].cell = c; g_cell_cache[h].valid = 1;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_proc_resolve_cells(rt_proc_t *p)
{
    if (p->cells_done) return;
    int np = p->nparams; const char **pn = p->pnames;
    if (np > 0 && pn) {
        p->pcells = (DESCR_t **)malloc((size_t)np * sizeof(DESCR_t *));
        if (p->pcells) for (int k = 0; k < np; k++) { const char *nm = pn[k]; p->pcells[k] = (nm && !rt_name_side_effecting(nm)) ? NV_PTR_fn(nm) : (DESCR_t *)0; }
    }
    { const char *rn = p->result_name ? p->result_name : p->name;
      p->rcell = (rn && !rt_name_side_effecting(rn)) ? NV_PTR_fn(rn) : (DESCR_t *)0; }
    p->cells_done = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_name_save_grow(void) {
    if (g_name_save_top < g_name_save_cap) return;
    int nc = g_name_save_cap ? g_name_save_cap * 2 : 4096; NameSaveEnt *np = (NameSaveEnt *)realloc(g_name_save, (size_t)nc * sizeof(NameSaveEnt));
    if (!np) return; g_name_save = np; g_name_save_cap = nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_name_save_push(const char **names, DESCR_t **cells, DESCR_t *args, int nargs, int n)
{
    int base = g_name_save_top;
    int fast = rt_call_fastpath_ok();
    for (int k = 0; k < n; k++) {
        const char *nm = names ? names[k] : (const char *)0; if (!nm) continue;
        rt_name_save_grow(); if (g_name_save_top >= g_name_save_cap) break;
        DESCR_t *cell = fast ? (cells ? cells[k] : rt_cell_for(nm)) : (DESCR_t *)0;
        DESCR_t arg = (k < nargs) ? args[k] : NULVCL;
        g_name_save[g_name_save_top].name = nm;
        g_name_save[g_name_save_top].cell = cell;
        if (cell) { g_name_save[g_name_save_top].old = *cell; *cell = arg; }
        else { g_name_save[g_name_save_top].old = NV_GET_fn(nm); NV_SET_fn(nm, arg); }
        g_name_save_top++;
    }
    return base;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_name_restore(int base)
{
    for (int k = g_name_save_top - 1; k >= base; k--) {
        if (g_name_save[k].cell) *g_name_save[k].cell = g_name_save[k].old;
        else NV_SET_fn(g_name_save[k].name, g_name_save[k].old);
    }
    g_name_save_top = base;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mon_emit_call_bin(const char *fname) {
    if (!g_monitor_bin || !fname) return;
    int64_t saved = kw_ftrace; kw_ftrace = 1; comm_call(fname); kw_ftrace = saved;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mon_emit_return_bin(const char *fname, DESCR_t retval) {
    if (!g_monitor_bin || !fname) return;
    char saved_rt[16]; memcpy(saved_rt, kw_rtntype, sizeof(saved_rt));
    const char *disc = IS_FAIL_fn(retval) ? "FRETURN" : "RETURN";
    size_t dl = strlen(disc); if (dl > 15) dl = 15; memcpy(kw_rtntype, disc, dl); kw_rtntype[dl] = '\0';
    int64_t saved = kw_ftrace; kw_ftrace = 1; comm_return(fname, retval); kw_ftrace = saved;
    memcpy(kw_rtntype, saved_rt, sizeof(saved_rt));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* NCB-1 LEAF SPLIT (Lon one-entry convention).  The dyn-scope call trampoline (V1 rt_call_named_proc, V2
 * rt_call_proc_direct — identical 16 steps, differing only in how p is found) is decomposed into two STRICT
 * LEAVES with the frame + the p->fn transfer between them, in exactly the shape the emitted call site will
 * take at the next step:
 *
 *     prologue leaf  →  frame (rsp bump)  →  call p->fn(fb, 0)  →  release  →  epilogue leaf
 *
 * The state that must survive across the transfer (save_base, Σ, want-name, rname, p) rides a LIFO context
 * stack rather than C locals, so the emitted site needs no ζ grant to hold it.  LIFO is sound BY THE LANGUAGE
 * DEFINITION, not by luck: SPITBOL's DEFINE saves formals AND locals on a pushdown stack and restores them on
 * RETURN/FRETURN/NRETURN (manual Ch.8, "Local variables"), so call nesting and save/restore nesting are the
 * same nesting — which is precisely why an assembly call/ret may carry it.  Depth-safe: grows like g_name_save.
 * Behaviour is preserved EXACTLY, including each caller's own pre-existing want-name clear ordering on the
 * not-found path (hence wn is passed IN rather than captured in the prologue). */
typedef struct {
    rt_proc_t  *p;
    const char *rname;
    const char *save_Σ;
    int         save_Σlen;
    int         save_base;
    int         wn;
    int         lex;        /* 0 = dyn-scope protocol (SNOBOL4 DEFINE), 1 = lexical (frame-bound args) */
    int         nargs;      /* lexical only: args to bind at frame_prep time */
    void       *fb;         /* lexical only: the frame, for the [fb+0] result read */
} rt_pcall_t;
static rt_pcall_t *g_pcall;
static int         g_pcall_top, g_pcall_cap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_pcall_grow(void)
{
    if (g_pcall_top < g_pcall_cap) return;
    int nc = g_pcall_cap ? g_pcall_cap * 2 : 1024;
    rt_pcall_t *np = (rt_pcall_t *)realloc(g_pcall, (size_t)nc * sizeof(rt_pcall_t));
    if (!np) return;
    g_pcall = np; g_pcall_cap = nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PROLOGUE LEAF — everything from resolve_cells through the monitor call event.  Returns the 16-aligned frame
 * byte count the caller must make available at fb; pushes the call context.  Strict leaf: calls no BB. */
int rt_proc_call_prologue(rt_proc_t *p, DESCR_t *args, int nargs, int wn)
{
    rt_proc_resolve_cells(p);
    int np = p->nparams;
    const char **pn = p->pnames;
    const char *rname = p->result_name ? p->result_name : p->name;
    int fbytes = (int)(PROC_FRAME_NEST_QWORDS * 8);
    if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    int save_base = rt_name_save_push(pn, p->pcells, args, nargs, np);
    { int rn_shadow = 0;
      for (int k = 0; k < np; k++) if (pn && pn[k] && !strcmp(pn[k], rname)) { rn_shadow = 1; break; }
      if (!rn_shadow) rt_name_save_push(&rname, &p->rcell, (DESCR_t *)0, 0, 1); }
    fbytes = (int)(((long)fbytes + 15L) & ~15L);
    rt_pcall_grow();
    if (g_pcall_top < g_pcall_cap) {
        rt_pcall_t *c = &g_pcall[g_pcall_top];
        c->p = p; c->rname = rname; c->save_Σ = Σ; c->save_Σlen = Σlen; c->save_base = save_base; c->wn = wn;
        c->lex = 0; c->nargs = 0; c->fb = (void *)0;
    }
    g_pcall_top++;
    if (g_monitor_bin) mon_emit_call_bin(p->name);
    rt_k_level++;
    return fbytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* EPILOGUE BODY — everything from the Σ restore through the monitor return event.  ONE body, TWO entries: the
 * port is a STATIC fact at each entry (γ passes 0, ω passes 1), never a flag re-derived from a sentinel.  The
 * IR already knows the port — lower_snobol4.c routes RETURN→exitnd(γ) and FRETURN→failnd(ω) — so the call
 * regime's IS_FAIL_fn round-trip is an impedance mismatch, not information.  PROC-CONV (R12-FREE rung 2) wires
 * γ/ω to separate landings; each landing knows its port BY BEING that landing, so `failed` arrives as a
 * constant and the shim below is deleted.  Strict leaf: calls no BB.  ⚠ The c.lex arm is PORT-AGNOSTIC today —
 * it reads [fb+0] whether the callee reached RETURN or FRETURN, ignoring the port entirely.  Preserved VERBATIM
 * here (this refactor is watermark-neutral by construction); it needs a ruling before the transfer converts. */
static DESCR_t rt_proc_epilogue_body(rt_pcall_t c, int failed)
{
    if (c.lex) return failed ? FAILDESCR : rt_nret_fix(*(DESCR_t *)c.fb, c.wn);
    Σ = c.save_Σ; Σlen = c.save_Σlen;
    DESCR_t *rcell = rt_call_fastpath_ok() ? c.p->rcell : (DESCR_t *)0;
    DESCR_t result = failed ? FAILDESCR : (rcell ? *rcell : NV_GET_fn(c.rname));
    result = rt_nret_fix(result, c.wn);
    rt_name_restore(c.save_base);
    if (g_monitor_bin) mon_emit_return_bin(c.p->name, result);
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* γ ENTRY — RETURN and NRETURN.  Manual Ch.8: RETURN yields a value for the caller.  NRETURN yields a NAME and
 * is a γ citizen too — lower_snobol4.c routes its SNO$NRET node to exitnd, the same γ as RETURN, the flag
 * riding in rt_g_ret_by_name.  There is no fifth port (RULES.md: FOUR PORTS = FOUR GREEK NAMES ALWAYS). */
DESCR_t rt_proc_call_epilogue_γ(void)
{
    rt_k_level--;
    if (g_pcall_top <= 0) return FAILDESCR;
    return rt_proc_epilogue_body(g_pcall[--g_pcall_top], 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ω ENTRY — FRETURN.  Manual Ch.8 verbatim: "Transferring to the special label FRETURN returns from a function
 * signaling failure to the caller.  No value is returned as the function result." */
DESCR_t rt_proc_call_epilogue_ω(void)
{
    rt_k_level--;
    if (g_pcall_top <= 0) return FAILDESCR;
    return rt_proc_epilogue_body(g_pcall[--g_pcall_top], 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* CALL-REGIME SHIM — the sentinel round-trip, retained VERBATIM in behavior until PROC-CONV converts the
 * transfer.  Every existing call site keeps reaching the epilogue through here, which is what makes this rung
 * watermark-neutral.  When the transfer becomes wire+jmp, the landings reach the two entries above directly
 * and this function — with IS_FAIL_fn — goes away. */
DESCR_t rt_proc_call_epilogue(DESCR_t fret)
{
    return IS_FAIL_fn(fret) ? rt_proc_call_epilogue_ω() : rt_proc_call_epilogue_γ();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* LEXICAL PROLOGUE LEAF — the rt_call_proc_descr protocol: no name saves, args bound INTO the frame, result
 * read back from [fb+0].  Kept behind the same ctx as the dyn protocol so the EMITTED call site is
 * protocol-agnostic: open → frame → call → epilogue, with the leaves knowing which discipline applies. */
static int rt_proc_call_prologue_lex(rt_proc_t *p, int nargs, int wn)
{
    int fbytes = (int)(PROC_FRAME_QWORDS * 8);
    if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    fbytes = (int)(((long)fbytes + 15L) & ~15L);
    if (nargs > CALL_ARGS_MAX) nargs = CALL_ARGS_MAX;
    rt_pcall_grow();
    if (g_pcall_top < g_pcall_cap) {
        rt_pcall_t *c = &g_pcall[g_pcall_top];
        c->p = p; c->rname = p->name; c->save_Σ = Σ; c->save_Σlen = Σlen; c->save_base = 0; c->wn = wn;
        c->lex = 1; c->nargs = nargs; c->fb = (void *)0;
    }
    g_pcall_top++;
    rt_k_level++;
    return fbytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* OPEN LEAF — the one entry the emitted call site uses.  Finds the proc, selects the protocol, runs the
 * matching prologue.  Returns the 16-aligned frame byte count the site must make available at fb, or 0 if the
 * proc has no body (the site's FAIL arm).  Args are read from the staged g_call_args, as the C trampolines do. */
long rt_proc_call_open(const char *name, int nargs)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (!p || !p->fn) return 0;
    int wn = rt_g_want_name; rt_g_want_name = 0;
    /* NB: cells are resolved ONLY on the dyn path (rt_proc_call_prologue does it).  The lexical path must not
     * touch them: NV_PTR_fn interns a global cell per name, so resolving there would mint spurious globals for
     * the lexically-scoped frontends' formals. */
    if (p->dyn_scope) return (long)rt_proc_call_prologue(p, g_call_args, nargs, wn);
    return (long)rt_proc_call_prologue_lex(p, nargs, wn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FRAME-PREP LEAF — the site has just made fbytes of frame available at fb (an rsp bump, once the call site is
 * emitted; an alloca while it is still C).  Fill it per the open protocol, record it, and hand back the entry
 * to transfer to.  Returning fn is what lets the site do a single medium-symmetric `call rax` — no proc-symbol
 * or table-index encoding is needed in either medium. */
void *rt_frame_prep(void *fb, long fbytes)
{
    if (g_pcall_top <= 0) return (void *)0;
    rt_pcall_t *c = &g_pcall[g_pcall_top - 1];
    c->fb = fb;
    if (c->lex) {
        DESCR_t *zf = (DESCR_t *)fb;
        for (long zi = 0; zi < fbytes / 16; zi++) zf[zi] = NULVCL;
        rt_frame_bind_args((char *)fb, c->p, c->nargs);
    } else {
        memset(fb, 0, (size_t)fbytes);
        if (g_monitor_bin) { /* dyn monitor call event already fired in the dyn prologue */ }
    }
    return (void *)c->p->fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FAIL LEAF (NCB-1b) — the emitted call site's no-body arm.  rt_proc_call_open returns 0 when the proc has no
 * body; the C trampolines answered that with `return FAILDESCR`, but an emitted site cannot materialise a
 * struct-by-value constant, so it calls this instead and lands the same rax:rdx pair.  Strict leaf. */
DESCR_t rt_faildescr(void) { return FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs)
{
    if (!name) return FAILDESCR;
    int _wn = rt_g_want_name; rt_g_want_name = 0;
    rt_proc_t *p = rt_proc_find(name);
    if (!p || !p->fn) return FAILDESCR;
    int fbytes = rt_proc_call_prologue(p, args, nargs, _wn);
    void *fb; if (rt_zeta_cstack()) fb = alloca((size_t)fbytes); else fb = rt_zls2_push((long)fbytes);
    memset(fb, 0, (size_t)fbytes);
    DESCR_t fret = p->fn(fb, 0);
    if (!rt_zeta_cstack()) rt_zls2_release_to((void *)((char *)fb + fbytes));
    return rt_proc_call_epilogue(fret);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_proc_direct(long idx, DESCR_t *args, int nargs)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return FAILDESCR;
    rt_proc_t *p = &g_rt_gen_procs[idx];
    if (!p->fn) return FAILDESCR;
    int _wn = rt_g_want_name; rt_g_want_name = 0;
    int fbytes = rt_proc_call_prologue(p, args, nargs, _wn);
    void *fb; if (rt_zeta_cstack()) fb = alloca((size_t)fbytes); else fb = rt_zls2_push((long)fbytes);
    memset(fb, 0, (size_t)fbytes);
    DESCR_t fret = p->fn(fb, 0);
    if (!rt_zeta_cstack()) rt_zls2_release_to((void *)((char *)fb + fbytes));
    return rt_proc_call_epilogue(fret);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_index_of(const char *name)
{
    if (!name) return -1;
    unsigned h = (unsigned)(((uintptr_t)name >> 4) & DCR_CELL_CACHE_MASK);
    if (g_proc_idx_key[h] == name) { int ci = g_proc_idx_slot[h]; if (ci < g_rt_gen_proc_count) return ci; }
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_proc_idx_key[h] = name; g_proc_idx_slot[h] = i; return i; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_table_fill(int64_t *tab, const char **names, int n)
{
    if (!tab || !names) return;
    for (int k = 0; k < n; k++) tab[k] = (int64_t)rt_proc_index_of(names[k]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_cache_clear(void) { for (int i = 0; i < DCR_CELL_CACHE_SIZE; i++) g_proc_idx_key[i] = (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_proc_t * rt_proc_find(const char *name)
{
    if (!name) return (rt_proc_t *)0;
    unsigned h = (unsigned)(((uintptr_t)name >> 4) & DCR_CELL_CACHE_MASK);
    if (g_proc_idx_key[h] == name) { int ci = g_proc_idx_slot[h]; if (ci < g_rt_gen_proc_count) return &g_rt_gen_procs[ci]; }
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_proc_idx_key[h] = name; g_proc_idx_slot[h] = i; return &g_rt_gen_procs[i]; }
    return (rt_proc_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_frame(const char *name, int nslots, int decl_level)
{
    rt_proc_t *p = rt_proc_find(name);
    if (p) { p->frame_nslots = nslots; p->decl_level = decl_level; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_frame_bytes(const char *name, int bytes)
{
    rt_proc_t *p = rt_proc_find(name);
    if (p && bytes > p->frame_bytes) p->frame_bytes = bytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_frame_nslots(const char *name)
{
    rt_proc_t *p = rt_proc_find(name);
    return p ? p->frame_nslots : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_decl_level(const char *name)
{
    rt_proc_t *p = rt_proc_find(name);
    return p ? p->decl_level : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_byref(const char *name, uint64_t mask)
{
    rt_proc_t *p = rt_proc_find(name);
    if (p) p->byref_mask = mask;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t rt_proc_byref_mask(const char *name)
{
    rt_proc_t *p = rt_proc_find(name);
    return p ? p->byref_mask : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *rt_gvar_cell(const char *name)
{
    return NV_PTR_fn(name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_named_proc_sl(const char *name, DESCR_t *args, int nargs, void *sl)
{
    rt_proc_t *p = rt_proc_find(name);
    if (!p || !p->fn) return FAILDESCR;
    rt_proc_resolve_cells(p);
    int np = p->nparams;
    int ns = (p->frame_nslots > np) ? p->frame_nslots : np;
    int fbytes = (int)(PROC_FRAME_NEST_QWORDS * 8);
    if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    int save_base = g_name_save_top;
    rt_name_save_push(&name, &p->rcell, (DESCR_t *)0, 0, 1);
    fbytes = (int)(((long)fbytes + 15L) & ~15L);
    void *fb; if (rt_zeta_cstack()) fb = alloca((size_t)fbytes); else fb = rt_zls2_push((long)fbytes);
    ((void **)fb)[0] = sl;
    DESCR_t *slots = (DESCR_t *)((char *)fb + 16);
    for (int k = 0; k < ns; k++) slots[k] = (k < np && k < nargs) ? args[k] : NULVCL;
    const char *save_Σ = Σ; int save_Σlen = Σlen;
    if (g_monitor_bin) mon_emit_call_bin(name);
    rt_k_level++; DESCR_t fret = p->fn(fb, 0); rt_k_level--;
    Σ = save_Σ; Σlen = save_Σlen;
    if (!rt_zeta_cstack()) rt_zls2_release_to((void *)((char *)fb + fbytes));
    DESCR_t *rcell = rt_call_fastpath_ok() ? p->rcell : (DESCR_t *)0; DESCR_t result = IS_FAIL_fn(fret) ? FAILDESCR : (rcell ? *rcell : NV_GET_fn(name));
    rt_name_restore(save_base);
    if (g_monitor_bin) mon_emit_return_bin(name, result);
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_define(const char *spec)
{
    (void)spec;
    return NULVCL;
}
static int g_last_ok = 0;
extern void rt_set_last_ok(int v);
extern DESCR_t binop_apply(int op, DESCR_t lv, DESCR_t rv, int *rel_fail);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_size_d(uint64_t lo, uint64_t hi)
{
    DESCR_t v;
    v.v    = (DTYPE_t)(uint32_t)(lo & 0xFFFFFFFFu);
    v.slen = (uint32_t)(lo >> 32);
    v.i    = (int64_t)hi;
    if (IS_FAIL_fn(v)) return FAILDESCR;
    if (v.v == DT_SNUL) { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = 0; return r; }
    if (v.v == DT_T) { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)(v.tbl ? v.tbl->size : 0); return r; }
    if (IS_CSET_fn(v)) { extern int kw_cset_len(const char *); int kn = v.s ? kw_cset_len(v.s) : -1; size_t n = (kn >= 0) ? (size_t)kn : (v.s ? strlen(v.s) : 0); DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)n; return r; }
    if (v.v == DT_S) {
        size_t n = v.slen ? (size_t)v.slen : (v.s ? strlen(v.s) : 0);
        DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)n; return r;
    }
    if (v.v == DT_DATA && v.u) {
        DESCR_t tag = FIELD_GET_fn(v, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)FIELD_GET_fn(v, "frame_size").i; return r; }
        if (v.u->type) { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)v.u->type->nfields; return r; }
        { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = 0; return r; }
    }
    { const char *s = VARVAL_fn(v); long n = s ? (long)strlen(s) : 0; DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)n; return r; }
}
extern int list_bang_at(DESCR_t obj, int64_t idx, DESCR_t *out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_list_bang_at(DESCR_t obj, int64_t idx)
{
    DESCR_t out;
    if (list_bang_at(obj, idx, &out)) return out;
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_list_bang_key_at(DESCR_t obj, int64_t idx)
{
    extern int list_bang_key_at(DESCR_t obj, int64_t idx, DESCR_t *out);
    DESCR_t out;
    if (list_bang_key_at(obj, idx, &out)) return out;
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_cut_set(void)
{
    extern int g_resolve_cut_flag;
    g_resolve_cut_flag = 1;
}
extern int     subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
extern int     subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val);
#include "SM.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((weak)) DESCR_t sm_eval_subexpr(int entry_pc)
{
    fprintf(stderr,
        "libscrip_rt: sm_eval_subexpr(%d) called — DT_E EVAL dispatch "
        "not yet wired in EM-6.  Add to EM-10 scope.\n", entry_pc);
    abort();
}
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct { const char *chars; int delta; } rt_cs_t;
extern void flush_pending_captures(void);
extern void reset_capture_registry(void);
extern void clear_pending_flags(void);
static void rt_register_cap(cap_t *c);
#define RT_MAX_CAPTURES 256
static cap_t *g_rt_cap_list[RT_MAX_CAPTURES];
static int    g_rt_cap_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_root_args(void)
{
    extern void rt_gc_visit_descr(DESCR_t *d);
    for (int i = 0; i < CALL_ARGS_MAX; i++) rt_gc_visit_descr(&g_call_args[i]);
}
