#include "rt.h"
#include "rt_arena.h"
#include "rt_coexpr.h"
#include <unistd.h>
#include <stddef.h>
#include "../contracts/pin_va.h"
#include "../contracts/ab_abi.h"
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
__asm__(".globl rt_outer_call\n.type rt_outer_call, @function\n"
        "rt_outer_call:\n"
        "  push %r12\n"
        "  sub $8, %rsp\n"
        "  mov %rdi, %rax\n"
        "  mov %rsi, %rdi\n"
        "  mov %rdx, %rsi\n"
        "  mov 0x70000000, %r12\n"
        "  movq g_rtcc_on@GOTPCREL(%rip), %r10\n"
        "  cmpb $0, (%r10)\n"
        "  je 1f\n"
        "  movq rtccb@GOTPCREL(%rip), %r10\n"
        "  movq 64(%r10), %r11\n"
        "  movq 40(%r10), %r8\n"
        "  movq 48(%r10), %r9\n"
        "  movq 56(%r10), %r10\n"
        "1:\n"
        "  call *%rax\n"
        "  add $8, %rsp\n"
        "  pop %r12\n"
        "  ret\n"
        ".size rt_outer_call, .-rt_outer_call\n");
__asm__(".globl rt_outer_call_delta0\n.type rt_outer_call_delta0, @function\n"
        "rt_outer_call_delta0:\n"
        "  push %r14\n"
        "  xor %r14d, %r14d\n"
        "  call rt_outer_call\n"
        "  pop %r14\n"
        "  ret\n"
        ".size rt_outer_call_delta0, .-rt_outer_call_delta0\n");
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
const char *rt_nv_cstr(const char *name)
{
    const char *s = VARVAL_fn(NV_GET_fn(name ? name : ""));
    return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_nv_slen(const char *name)
{
    return (long)descr_slen(NV_GET_fn(name ? name : ""));
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
void rt_gvar_assign_pat_sz(const char *name, void *fn, int64_t zsz, int32_t zstatic)
{
    extern void *dtp_wrap_fn_sz(void *, int64_t, int32_t);
    DESCR_t d;
    d.v    = DT_P;
    d.slen = 0;
    d.p    = dtp_wrap_fn_sz(fn, zsz, zstatic);
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
    if (v.v == DT_N && v.slen == 0 && v.s) { out->v = DT_S; out->s = v.s; out->slen = (uint32_t)strlen(v.s); return; }
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
void c_rt_coerce_num2_d(const DESCR_t *self, const DESCR_t *other, DESCR_t *out, long codes) {
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
long rt_pat_prim_int(const char *varname) {
    extern DESCR_t NV_GET_fn(const char *);
    DESCR_t v = NV_GET_fn(varname ? varname : "");
    int64_t r = 0;
    if (v.v == DT_I) { r = v.i; }
    else if (v.v == DT_R) { double d = v.r; r = (int64_t)d; }
    else if (v.v == DT_SNUL) { r = 0; }
    else if (v.v == DT_S && v.s) {
        if (!v.s[0]) { r = 0; }
        else { const char *p = v.s; while (*p == ' ') p++; char *ep = NULL; long long t = strtoll(p, &ep, 10);
               if (ep && ep != p) { while (*ep == ' ') ep++; if (*ep == 0) r = (int64_t)t; else return -1; } else return -1; }
    } else { return -1; }
    if (r < 0) return -1;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_pat_prim_str(const char *varname, const char **out_ptr, long *out_len) {
    extern DESCR_t NV_GET_fn(const char *);
    DESCR_t v = NV_GET_fn(varname ? varname : "");
    DESCR_t s;
    rt_coerce_str_d(&v, &s, 0);
    if (s.v != DT_S) return -1;
    *out_ptr = s.s ? s.s : "";
    *out_len = (long)s.slen;
    return 0;
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
__attribute__((visibility("hidden"))) int rt_k_level = 1;
int * const rt_k_level_p = &rt_k_level;
#define PROC_FRAME_QWORDS 512
#define CALL_ARGS_MAX     64
typedef struct {
    const char *name; bb_box_fn fn; const char **pnames; int nparams; int frame_nslots; int decl_level; uint64_t byref_mask;
    int frame_bytes; DESCR_t **pcells; DESCR_t *rcell; int cells_done; int is_generator; int dyn_scope; const char *result_name; int is_variadic; int rest_kind; int named_rest; int jmp_entry; int redefined; int zstatic; int pnames_owned; int nformals;
} rt_proc_t;
_Static_assert(__builtin_offsetof(rt_proc_t, fn) == 8, "rtx_call.S bakes PROC_FN for the rt_proc_open_fn port (RTX-4 slice 3); confirmed from emitted -O0 code as mov 0x8(%rax),%rax");
_Static_assert(__builtin_offsetof(rt_proc_t, name) == 0 && __builtin_offsetof(rt_proc_t, is_generator) == 0x4c, "rtx_call.S bakes PROC_NAME and PROC_ISGEN");
_Static_assert(__builtin_offsetof(rt_proc_t, dyn_scope) == 80 && sizeof(rt_proc_t) == 128, "rtx_plcall.S bakes PROC_DYN_SCOPE and the shl 7 index stride (RTX-1-PL)");
_Static_assert(__builtin_offsetof(rt_proc_t, frame_bytes) == 48, "rtx_plcall.S records this in its offset table; the fbytes computation is elided, not baked");
__attribute__((visibility("hidden"))) rt_proc_t    *g_rt_gen_procs = (rt_proc_t *)0;
__attribute__((visibility("hidden"))) int           g_rt_gen_proc_count = 0;
static int           g_rt_gen_proc_cap = 0;
static int          *g_proc_hsl = (int *)0;
static unsigned      g_proc_hcap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned rt_proc_fnv(const char *s) { unsigned h = 2166136261u; while (*s) { h ^= (unsigned char)*s++; h *= 16777619u; } return h; }
static void rt_proc_hash_seed(int idx) { unsigned m = g_proc_hcap - 1, h = rt_proc_fnv(g_rt_gen_procs[idx].name) & m; while (g_proc_hsl[h]) h = (h + 1) & m; g_proc_hsl[h] = idx + 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_proc_hash_insert(int idx) {
    if ((unsigned)(g_rt_gen_proc_count + 1) * 4 >= g_proc_hcap * 3) {
        unsigned nc = g_proc_hcap ? g_proc_hcap * 2 : 1024; int *np = (int *)rt_ws_realloc(g_proc_hsl, (size_t)nc * sizeof(int)); if (!np) return;
        g_proc_hsl = np; g_proc_hcap = nc; memset(g_proc_hsl, 0, (size_t)nc * sizeof(int));
        for (int i = 0; i < g_rt_gen_proc_count; i++) if (g_rt_gen_procs[i].name) rt_proc_hash_seed(i);
        return;
    }
    rt_proc_hash_seed(idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_proc_hash_lookup(const char *name) {
    if (!name || !g_proc_hcap) return -1;
    unsigned m = g_proc_hcap - 1, h = rt_proc_fnv(name) & m;
    while (g_proc_hsl[h]) { int ix = g_proc_hsl[h] - 1; if (ix < g_rt_gen_proc_count && g_rt_gen_procs[ix].name && strcmp(g_rt_gen_procs[ix].name, name) == 0) return ix; h = (h + 1) & m; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_gen_proc_grow(void) {
    if (g_rt_gen_proc_count < g_rt_gen_proc_cap) return;
    int nc = g_rt_gen_proc_cap ? g_rt_gen_proc_cap * 2 : 64; rt_proc_t *np = (rt_proc_t *)rt_ws_realloc(g_rt_gen_procs, (size_t)nc * sizeof(rt_proc_t));
    if (!np) return; g_rt_gen_procs = np; g_rt_gen_proc_cap = nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_register(const char *name, const char **pnames, int nparams)
{
    if (!name) return;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { if (pnames) g_rt_gen_procs[i].pnames = pnames; if (nparams) g_rt_gen_procs[i].nparams = nparams; g_rt_gen_procs[i].cells_done = 0; g_rt_gen_procs[i].nformals = 0; g_rt_gen_procs[i].redefined = 1; return; } }
    rt_gen_proc_grow();
    if (g_rt_gen_proc_count >= g_rt_gen_proc_cap) return;
    rt_proc_t *p = &g_rt_gen_procs[g_rt_gen_proc_count++];
    p->name = name; p->fn = NULL; p->pnames = pnames; p->nparams = nparams; p->frame_nslots = -1; p->decl_level = 0; p->byref_mask = 0;
    p->frame_bytes = 0; p->pcells = (DESCR_t **)0; p->rcell = (DESCR_t *)0; p->cells_done = 0; p->is_generator = 0; p->dyn_scope = 0; p->result_name = (const char *)0; p->is_variadic = 0; p->rest_kind = 0; p->named_rest = 0; p->jmp_entry = 0; p->zstatic = 0; p->pnames_owned = 0; p->nformals = 0; rt_proc_hash_insert(g_rt_gen_proc_count - 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_result_name(const char *name, const char *rname)
{
    if (!name) return;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_rt_gen_procs[i].result_name = rname; g_rt_gen_procs[i].cells_done = 0; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_dyn_scope(const char *name, int v)
{
    if (!name) return;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_rt_gen_procs[i].dyn_scope = v ? 1 : 0; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_dyn_scope(const char *name)
{
    if (!name) return 0;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) return g_rt_gen_procs[i].dyn_scope; }
    return 0;
}
static rt_proc_t *rt_proc_find(const char *name);
void rt_proc_cache_clear(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_enum_count(void) { return g_rt_gen_proc_count; }
const char *rt_proc_enum_name(int i) { return (i >= 0 && i < g_rt_gen_proc_count) ? g_rt_gen_procs[i].name : (const char *)0; }
void rt_proc_reset(void) { g_rt_gen_proc_count = 0; rt_proc_cache_clear(); if (g_proc_hsl) memset(g_proc_hsl, 0, (size_t)g_proc_hcap * sizeof(int)); }
__attribute__((noreturn)) void rt_ab_undef_fn_stub(void) { core_runtime_error(22, "Undefined function called"); __builtin_unreachable(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *gva_register(const char **names, DESCR_t *cells, int n) {
    if (!cells) return cells;
    for (int k = 0; k < n; k++) { const char *nm = names ? names[k] : (const char *)0; if (!nm) continue; (void)NV_bind_gva(nm, &cells[k]); }
    { static int _b1cg = -1; if (_b1cg < 0) { const char *_e = getenv("SCRIP_B1C_PARITY"); _b1cg = (_e && *_e == '0') ? 0 : 1; }
      extern int gva_count(void); extern int gva_collect_var(const char *); extern int g_gva_active;
      if (_b1cg && n > 0 && gva_count() == 0) { for (int k = 0; k < n; k++) if (names && names[k]) (void)gva_collect_var(names[k]); g_gva_active = (gva_count() > 0) ? 1 : 0; } }
    return cells;
}
#define RT_GVA_ISLAND_BYTES ((size_t)16u << 20)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rt_pin_init(void) {
    void * p = mmap((void *)RT_PIN_BASE, RT_PIN_BYTES, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (p != (void *)RT_PIN_BASE) { fprintf(stderr, "rt_pin_init: RT_PIN_BASE 0x%lx unavailable (got %p) -- REG-0 tripwire, see RUNG REG-MAP\n", (unsigned long)RT_PIN_BASE, p); abort(); }
    void * g = mmap((void *)RT_GVA_VA, RT_GVA_ISLAND_BYTES, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (g != (void *)RT_GVA_VA) { fprintf(stderr, "rt_pin_init: RT_GVA_VA 0x%lx unavailable (got %p) -- REG-1 tripwire, see RUNG REG-MAP\n", (unsigned long)RT_GVA_VA, g); abort(); }
    *(volatile uint64_t *)RT_AB_NRET   = 0;
    { extern void rt_dcap_lazy_init(void); rt_dcap_lazy_init(); }
}
extern int rt_g_want_name;
extern int rt_g_ret_by_name;
DESCR_t rt_nret_fix(DESCR_t r, int wn);
int rt_value_trail_mark(void);
void rt_value_trail_tidy_dead_window(int mark, void *fb, void *top);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_ab_enter_env(void *frame)
{
    char *fb = (char *)frame;
    *(uint64_t *)(fb + AB_OFF_SIGMA)    = (uint64_t)(uintptr_t)Σ;
    *(uint64_t *)(fb + AB_OFF_SIGMALEN) = (uint64_t)(int64_t)Σlen;
    *(uint64_t *)(fb + AB_OFF_WN)       = (uint64_t)(int64_t)rt_g_want_name; rt_g_want_name = 0;
    int vtm = rt_value_trail_mark();
    *(uint64_t *)(fb + AB_OFF_VTMARK)   = (uint64_t)(int64_t)vtm;
    rt_k_level++;
    kw_fnclevel = (int64_t)rt_k_level - 1;
    return vtm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_ab_leave_env(void *frame, DESCR_t result, int is_fail)
{
    char *fb = (char *)frame;
    int vtm  = (int)(int64_t)*(uint64_t *)(fb + AB_OFF_VTMARK);
    int wn   = (int)(int64_t)*(uint64_t *)(fb + AB_OFF_WN);
    rt_value_trail_tidy_dead_window(vtm, (void *)fb, (char *)fb + 16);
    Σ    = (const char *)(uintptr_t)*(uint64_t *)(fb + AB_OFF_SIGMA);
    Σlen = (int)(int64_t)*(uint64_t *)(fb + AB_OFF_SIGMALEN);
    rt_k_level--;
    kw_fnclevel = (int64_t)rt_k_level - 1;
    if (is_fail) { rt_g_want_name = wn; return FAILDESCR; }
    return rt_nret_fix(result, wn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *rt_gva_island(int n) {
    if ((size_t)n * sizeof(DESCR_t) > RT_GVA_ISLAND_BYTES) { fprintf(stderr, "rt_gva_island: %d slots exceed the island (raise RT_GVA_ISLAND_BYTES)\n", n); abort(); }
    { extern void rt_sxt_gva_count(int); rt_sxt_gva_count(n); }
    if (n > 0) memset((void *)RT_GVA_VA, 0, (size_t)n * sizeof(DESCR_t));
    return (DESCR_t *)RT_GVA_VA;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_is_registered(const char *name)
{
    if (!name) return 0;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_jmp_entry(const char *name)
{
    if (!name) return 0;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) return g_rt_gen_procs[i].jmp_entry; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_fn(const char *name)
{
    if (!name) return (void *)0;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) return (void *)g_rt_gen_procs[i].fn; }
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_nparams(const char *name, int nparams)
{
    rt_proc_t *p = rt_proc_find(name);
    if (p) p->nparams = nparams;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_nformals(const char *name, int nformals)
{
    rt_proc_t *p = rt_proc_find(name);
    if (p) p->nformals = nformals;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_nformals(const char *name)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (!p) return -1;
    return p->nformals > 0 ? p->nformals : p->nparams;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_pname(const char *name, int k, const char *pname)
{
    rt_proc_t *p = rt_proc_find(name);
    if (!p || k < 0 || k >= CALL_ARGS_MAX) return;
    if (!p->pnames) { const char **v = (const char **)calloc((size_t)CALL_ARGS_MAX, sizeof(const char *)); if (!v) return; p->pnames = v; p->pnames_owned = 1; }
    if (!p->pnames_owned) return;
    ((const char **)p->pnames)[k] = pname;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_nparams(const char *name)
{
    if (!name) return -1;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) return g_rt_gen_procs[i].nparams; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_has_native_fn(const char *name)
{
    if (!name) return 0;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) return g_rt_gen_procs[i].fn != (bb_box_fn)0; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_get_fn(const char *name)
{
    if (!name) return (void *)0;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) return (void *)g_rt_gen_procs[i].fn; }
    return (void *)0;
}
static struct { void *fn; int bytes; } g_blob_fb[512];
static int g_blob_fb_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_fn_frame_bytes_register(void *fn, int bytes)
{
    if (!fn || bytes <= 0) return;
    for (int i = 0; i < g_blob_fb_n; i++) if (g_blob_fb[i].fn == fn) { if (bytes > g_blob_fb[i].bytes) g_blob_fb[i].bytes = bytes; return; }
    if (g_blob_fb_n < (int)(sizeof g_blob_fb / sizeof *g_blob_fb)) { g_blob_fb[g_blob_fb_n].fn = fn; g_blob_fb[g_blob_fb_n].bytes = bytes; g_blob_fb_n++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_fn_frame_bytes(void *fn)
{
    if (!fn) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++) if ((void *)g_rt_gen_procs[i].fn == fn && g_rt_gen_procs[i].frame_bytes > 0) return (long)g_rt_gen_procs[i].frame_bytes;
    for (int i = 0; i < g_blob_fb_n; i++) if (g_blob_fb[i].fn == fn) return (long)g_blob_fb[i].bytes;
    return (long)PROC_FRAME_QWORDS * 8;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_fn_frame_bytes_known(void *fn)
{
    if (!fn) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++) if ((void *)g_rt_gen_procs[i].fn == fn && g_rt_gen_procs[i].frame_bytes > 0) return (long)g_rt_gen_procs[i].frame_bytes;
    for (int i = 0; i < g_blob_fb_n; i++) if (g_blob_fb[i].fn == fn) return (long)g_blob_fb[i].bytes;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_zstatic(const char *name, int bit)
{
    if (!name) return;
    int i = rt_proc_hash_lookup(name); if (i >= 0) g_rt_gen_procs[i].zstatic = bit ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_fn_zstatic_known(void *fn)
{
    if (!fn) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++) if ((void *)g_rt_gen_procs[i].fn == fn) return (long)g_rt_gen_procs[i].zstatic;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_generator(const char *name, int is_gen)
{
    if (!name) return;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_rt_gen_procs[i].is_generator = is_gen ? 1 : 0; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_variadic(const char *name, int is_var)
{
    if (!name) return;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_rt_gen_procs[i].is_variadic = is_var ? 1 : 0; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_rest_kind(const char *name, int kind)
{
    if (!name) return;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_rt_gen_procs[i].rest_kind = kind; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_named_rest(const char *name, int slot)
{
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_rt_gen_procs[i].named_rest = slot; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_named_rest(const char *name)
{
    { int i = rt_proc_hash_lookup(name); if (i >= 0) return g_rt_gen_procs[i].named_rest; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_jmpentry(const char *name, int on)
{
    if (!name) return;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_rt_gen_procs[i].jmp_entry = on ? 1 : 0; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_is_generator(const char *name)
{
    if (!name) return 0;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) return g_rt_gen_procs[i].is_generator; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_fn(const char *name, bb_box_fn fn)
{
    if (!name) return;
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_rt_gen_procs[i].fn = fn; return; } }
    rt_gen_proc_grow();
    if (g_rt_gen_proc_count >= g_rt_gen_proc_cap) return;
    rt_proc_t *p = &g_rt_gen_procs[g_rt_gen_proc_count++];
    p->name = name; p->fn = fn; p->pnames = NULL; p->nparams = 0; p->frame_nslots = -1; p->decl_level = 0; p->byref_mask = 0;
    p->frame_bytes = 0; p->pcells = (DESCR_t **)0; p->rcell = (DESCR_t *)0; p->cells_done = 0; p->is_generator = 0; p->dyn_scope = 0; p->result_name = (const char *)0; p->is_variadic = 0; p->rest_kind = 0; p->named_rest = 0; p->jmp_entry = 0; p->zstatic = 0; p->pnames_owned = 0; rt_proc_hash_insert(g_rt_gen_proc_count - 1);
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
DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_frame_bind_args(char *fb, rt_proc_t *p, int nargs)
{
    extern DESCR_t rt_make_list(DESCR_t *args, int nargs);
    extern DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs);
    extern DESCR_t rt_make_nested_agg(DESCR_t *args, int nargs);
    int npc = p->nparams; if (npc > CALL_ARGS_MAX) npc = CALL_ARGS_MAX;
    if (p->is_variadic && npc > 0) {
        int fixed = npc - 1;
        for (int i = 0; i < fixed; i++) *(DESCR_t *)(fb + 16 * (i + 1)) = (i < nargs) ? g_call_args[i] : NULVCL;
        int rest = nargs - fixed; if (rest < 0) rest = 0;
        DESCR_t *tail = rest > 0 ? &g_call_args[fixed] : (DESCR_t *)0;
        *(DESCR_t *)(fb + 16 * (fixed + 1)) = (p->rest_kind == 2) ? rt_make_nested_agg(tail, rest) : p->rest_kind ? rt_make_flat_agg(tail, rest) : rt_make_list(tail, rest);
        return;
    }
    for (int i = 0; i < nargs; i++) *(DESCR_t *)(fb + 16 * (i + 1)) = g_call_args[i];
    for (int i = nargs; i < npc; i++) *(DESCR_t *)(fb + 16 * (i + 1)) = NULVCL;
    if (p->named_rest > 0 && p->named_rest <= npc) { DESCR_t *slot = (DESCR_t *)(fb + 16 * p->named_rest); if (slot->v == NULVCL.v && slot->i == NULVCL.i) { char *e = (char *)rt_ws_alloc(1); e[0] = '\0'; *slot = STRVAL(e); } }
}
int rt_g_ret_by_name = 0;
int rt_g_want_name = 0;
DESCR_t rt_nret_fix(DESCR_t r, int wn);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_nret_fix(DESCR_t r, int wn) { extern int rt_cap_name_strict(void); if (rt_g_ret_by_name) { if (!wn || !rt_cap_name_strict()) rt_g_ret_by_name = 0; if (!wn && r.v == DT_N) { extern DESCR_t rt_deref(DESCR_t); r = rt_deref(r); } } rt_g_want_name = wn; return r; }
DESCR_t rt_nret_fix_tiny(DESCR_t r, int unused_edx) { (void)unused_edx; int wn = rt_g_want_name; DESCR_t o = rt_nret_fix(r, wn); rt_g_want_name = wn; return o; }
long    rt_proc_call_open(const char *name, int nargs);
void   *rt_frame_prep(void *fb, long fbytes);
void   *rt_proc_open_fn(void);
DESCR_t rt_proc_enter(void *fn);
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
DESCR_t rt_proc_call_epilogue_ω(void);
DESCR_t rt_proc_call_epilogue_ret(DESCR_t fret);
__asm__(
".text\n"
".globl rt_tiny_record_enter\n"
"rt_tiny_record_enter:\n"
"  pushq %rbp\n"
"  movq %rsp, %rbp\n"
"  pushq %rbx\n"
"  pushq %r12\n"
"  pushq %r13\n"
"  pushq %r14\n"
"  pushq %r15\n"
"  movq %rdi, %rax\n"
"  movq %rsi, %rdx\n"
"  shlq $4, %rdx\n"
"  leaq 56(%rdx), %rcx\n"
"  subq %rcx, %rsp\n"
"  movq g_call_args@GOTPCREL(%rip), %r10\n"
"  xorq %rdi, %rdi\n"
"  cmpq $0, %rsi\n"
"  jle 6f\n"
"5:\n"
"  movq %rsi, %r8\n"
"  subq $1, %r8\n"
"  subq %rdi, %r8\n"
"  shlq $4, %r8\n"
"  addq %r10, %r8\n"
"  movq 0(%r8), %rcx\n"
"  movq 8(%r8), %rdx\n"
"  movq %rdi, %r9\n"
"  shlq $4, %r9\n"
"  leaq 16(%rsp,%r9,1), %r9\n"
"  movq %rcx, 0(%r9)\n"
"  movq %rdx, 8(%r9)\n"
"  addq $1, %rdi\n"
"  cmpq %rsi, %rdi\n"
"  jl 5b\n"
"6:\n"
"  movq %rsi, %rdx\n"
"  shlq $4, %rdx\n"
"  leaq 16(%rsp,%rdx,1), %rcx\n"
"  movq %rsi, 0(%rcx)\n"
"  leaq 2f(%rip), %r10\n"
"  movq %r10, 8(%rcx)\n"
"  leaq 3f(%rip), %r10\n"
"  movq %r10, 16(%rcx)\n"
"  movq %rdx, 24(%rcx)\n"
"  movq $16, 32(%rcx)\n"
"  movq g_rtcc_on@GOTPCREL(%rip), %r10\n"
"  cmpb $0, (%r10)\n"
"  je 4f\n"
"  movq rtccb@GOTPCREL(%rip), %r10\n"
"  movq 24(%r10), %rsi\n"
"  movq 32(%r10), %rdi\n"
"  movq 64(%r10), %r11\n"
"  movq 40(%r10), %r8\n"
"  movq 48(%r10), %r9\n"
"  movq 56(%r10), %r10\n"
"4:\n"
"  jmp *%rax\n"
"2:\n"
"  movq %rax, %rdi\n"
"  movq %rdx, %rsi\n"
"  xorl %edx, %edx\n"
"  leaq -40(%rbp), %rsp\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  popq %rbp\n"
"  jmp rt_nret_fix_tiny\n"
"3:\n"
"  leaq -40(%rbp), %rsp\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  popq %rbp\n"
"  jmp rt_ret_faildescr\n"
);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_ret_faildescr(void) { rt_g_ret_by_name = 0; return FAILDESCR; }
void *rt_dyn_alpha_fn(const char *name, void *fallback);
DESCR_t rt_ret_faildescr(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_proc_descr(const char *name, int nargs)
{
    rt_proc_t *p = rt_proc_find(name);
    if (!p || !p->fn) {
        extern void rt_pl_iso_throw_existence_key(const char *);
        fprintf(stderr, "[GZ-10] rt_call_proc_descr: procedure '%s' has no stackless slab\n", name ? name : "(null)");
        rt_pl_iso_throw_existence_key(name ? name : "?");
        return FAILDESCR;
    }
    if (p->dyn_scope) { void *afn = rt_dyn_alpha_fn(name, (void *)0); if (afn) { extern DESCR_t rt_tiny_record_enter(void *fn, long nargs); int _n = nargs < CALL_ARGS_MAX ? nargs : CALL_ARGS_MAX; return rt_tiny_record_enter(afn, (long)(_n < 0 ? 0 : _n)); } }
    int _wn_gen = rt_g_want_name;
    long fbytes = rt_proc_call_open(name, nargs);
    if (!fbytes) return FAILDESCR;
    if (!p->dyn_scope) {
        if (p->jmp_entry) return rt_proc_enter((void *)p->fn);
        void *fb = alloca((size_t)fbytes);
        void *fn2 = rt_frame_prep(fb, fbytes);
        DESCR_t fret = ((DESCR_t (*)(void *, long))fn2)(fb, 0);
        return rt_proc_call_epilogue_ret(fret);
    }
    rt_g_want_name = _wn_gen;
    return rt_proc_enter(rt_dyn_alpha_fn(name, (void *)p->fn));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_dyn_alpha_fn(const char *name, void *fallback)
{
    static int live = -1; if (live < 0) { const char *e = getenv("SCRIP_DYN_ALPHA"); live = e ? (e[0] != '0') : 1; }
    if (!live || !name) return fallback;
    { extern void *bb_ab_fn_cell_ptr(const char *); char cn[264]; snprintf(cn, sizeof cn, "alpha$%s", name);
      void **cell = (void **)bb_ab_fn_cell_ptr(cn);
      return (cell && *cell && *cell != (void *)(uintptr_t)rt_ab_undef_fn_stub) ? *cell : fallback; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_seal_alpha(const char * name, void * fn) {
    static int live = -1; if (live < 0) { const char * e = getenv("SCRIP_M4_ALPHA_SEAL"); live = e ? (e[0] != '0') : 1; }
    if (!live || !name || !fn) return;
    { extern void * bb_ab_fn_cell_ptr(const char *); char cn[264]; snprintf(cn, sizeof cn, "alpha$%s", name);
      void ** cell = (void **) bb_ab_fn_cell_ptr(cn); if (cell) *cell = fn; }
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
typedef struct rt_genp_s {
    struct rt_genp_s *next;
    uint64_t          regs[5];
    scrip_coctx_t     co;
    DESCR_t           args[CALL_ARGS_MAX];
    int               nargs;
    void             *fn;
    const char       *name;
    int               done;
    int               first_done;
} rt_genp_s;
_Static_assert(offsetof(rt_genp_s, next) == 0 && offsetof(rt_genp_s, regs) == 8, "rt_genp_s layout drift vs rt_genp_thread_entry asm offsets");
static rt_genp_s *g_genp_head = (rt_genp_s *)0;
extern void rt_genp_entry_c(rt_genp_s *g);
extern void rt_genp_thread_entry(void *arg);
__asm__(
".text\n"
".globl rt_genp_thread_entry\n"
"rt_genp_thread_entry:\n"
"  movq  8(%rdi), %rbx\n"
"  movq 16(%rdi), %r12\n"
"  movq 24(%rdi), %r13\n"
"  movq 32(%rdi), %r14\n"
"  movq 40(%rdi), %r15\n"
"  jmp rt_genp_entry_c\n"
);
static __thread rt_genp_s *g_genp_self = (rt_genp_s *)0;
extern void rt_genp_spine_enter(void *fn);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_genp_deliver_γ(DESCR_t v)
{
    rt_genp_s *g = g_genp_self;
    if (!g->first_done) { g->first_done = 1; v = rt_proc_call_epilogue_γ(v); }
    { uint64_t d[2]; memcpy(d, &v, 16); scrip_coret(d[0], d[1], (void *)0); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_genp_deliver_ω(void)
{
    rt_genp_s *g = g_genp_self;
    if (!g->first_done) { g->first_done = 1; (void)rt_proc_call_epilogue_ω(); }
    g->done = 2;
    scrip_cofail();
    for (;;) pause();
}
__asm__(
".text\n"
".globl rt_genp_spine_enter\n"
"rt_genp_spine_enter:\n"
"  pushq $0\n"
"  leaq 4f(%rip), %rax\n"
"  pushq $0\n"
"  pushq %rax\n"
"  movq %rdi, %rax\n"
"  leaq 2f(%rip), %rcx\n"
"  leaq 3f(%rip), %rdx\n"
"  jmp *%rax\n"
"2:\n"
"  call rt_genp_deliver_γ\n"
"  jmp *(%rsp)\n"
"3:\n"
"  call rt_genp_deliver_ω\n"
"4:\n"
"  call rt_genp_deliver_ω\n"
);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_genp_entry_c(rt_genp_s *g)
{
    g_genp_self = g;
    for (int i = 0; i < g->nargs; i++) rt_arg_stage(i, g->args[i]);
    long fb = rt_proc_call_open(g->name, g->nargs);
    if (!fb) { g->done = 2; scrip_cofail(); }
    rt_genp_spine_enter(g->fn);
    g->done = 2; scrip_cofail();
    for (;;) pause();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_genp_yield(uint64_t d0, uint64_t d1) { scrip_coret(d0, d1, (void *)0); }
static rt_genp_s *rt_genp_lookup(void *h) { for (rt_genp_s *g = g_genp_head; g; g = g->next) if ((void *)g == h) return g; return (rt_genp_s *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_genp_destroy(rt_genp_s *g)
{
    scrip_coexpr_destroy(&g->co);
    rt_genp_s **pp = &g_genp_head; while (*pp && *pp != g) pp = &(*pp)->next; if (*pp) *pp = g->next;
    free(g);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_genp_triage(rt_genp_s *g, int ok, uint64_t out2[2], void **hout)
{
    DESCR_t r;
    if (!ok || g->done == 2) { if (hout) *hout = (void *)0; rt_genp_destroy(g); return FAILDESCR; }
    memcpy(&r, out2, 16);
    if (g->done == 1) { if (hout) *hout = (void *)0; rt_genp_destroy(g); return r; }
    if (hout) *hout = (void *)g;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **hout)
{
    rt_proc_t *p = rt_proc_find(name);
    if (!p || !p->fn) { extern void rt_pl_iso_throw_existence_key(const char *); fprintf(stderr, "[SUSP] rt_proc_call_gen_h: generator '%s' has no stackless slab\n", name ? name : "(null)"); rt_pl_iso_throw_existence_key(name ? name : "?"); if (hout) *hout = (void *)0; return FAILDESCR; }
    if (p->jmp_entry && p->is_generator) {
        uint64_t cregs[5];
        __asm__ volatile("movq %%rbx,%0\n\tmovq %%r12,%1\n\tmovq %%r13,%2\n\tmovq %%r14,%3\n\tmovq %%r15,%4" : "=m"(cregs[0]), "=m"(cregs[1]), "=m"(cregs[2]), "=m"(cregs[3]), "=m"(cregs[4]));
        rt_genp_s *g = (rt_genp_s *)calloc(1, sizeof *g);
        if (!g) { if (hout) *hout = (void *)0; return FAILDESCR; }
        memcpy(g->regs, cregs, sizeof cregs);
        g->nargs = nargs; if (g->nargs > CALL_ARGS_MAX) g->nargs = CALL_ARGS_MAX; if (g->nargs < 0) g->nargs = 0;
        for (int i = 0; i < g->nargs; i++) g->args[i] = g_call_args[i];
        g->fn = (void *)p->fn; g->name = p->name; g->done = 0;
        scrip_co_ctx_init(&g->co, rt_genp_thread_entry, (void *)g);
        scrip_co_gc_link(&g->co);
        g->next = g_genp_head; g_genp_head = g;
        uint64_t out2[2] = { 0, 0 };
        rt_k_level++;
        int ok = scrip_coexpr_activate(&g->co, 0, 0, out2);
        rt_k_level--;
        return rt_genp_triage(g, ok, out2, hout);
    }
    if (p->jmp_entry) {
        long fb2 = rt_proc_call_open(name, nargs);
        if (!fb2) { if (hout) *hout = (void *)0; return FAILDESCR; }
        if (hout) *hout = (void *)0;
        return rt_proc_enter((void *)p->fn);
    }
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
    { rt_genp_s *g = rt_genp_lookup(frame);
      if (g) {
          uint64_t out2[2] = { 0, 0 };
          rt_k_level++;
          int ok = scrip_coexpr_activate(&g->co, 0, 0, out2);
          rt_k_level--;
          return rt_genp_triage(g, ok, out2, hslot);
      } }
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
        p->pcells = (DESCR_t **)rt_ws_alloc((size_t)np * sizeof(DESCR_t *));
        if (p->pcells) for (int k = 0; k < np; k++) { const char *nm = pn[k]; p->pcells[k] = (nm && !rt_name_side_effecting(nm)) ? NV_PTR_fn(nm) : (DESCR_t *)0; }
    }
    { const char *rn = p->result_name ? p->result_name : p->name;
      p->rcell = (rn && !rt_name_side_effecting(rn)) ? NV_PTR_fn(rn) : (DESCR_t *)0; }
    p->cells_done = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_name_save_grow(void) {
    if (g_name_save_top < g_name_save_cap) return;
    int nc = g_name_save_cap ? g_name_save_cap * 2 : 4096; NameSaveEnt *np = (NameSaveEnt *)rt_ws_realloc(g_name_save, (size_t)nc * sizeof(NameSaveEnt));
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
extern int  rt_value_trail_mark(void);
extern void rt_value_trail_tidy_dead_below(int mark, void *upper);
extern void rt_value_trail_tidy_dead_window(int mark, void *lower, void *upper);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_lcl_proc_args_install(void *base_p, int nparams, int nlocals) {
    char *base = (char *)base_p;
    int nargs = nparams;
    int na = (nargs < nparams) ? nargs : nparams;
    for (int i = 0; i < na; i++) *(DESCR_t *)(base + (i + 1) * 16) = g_call_args[i];
    for (int i = na; i < nparams; i++) { DESCR_t _n = NULVCL; *(DESCR_t *)(base + (i + 1) * 16) = _n; }
    for (int j = 0; j < nlocals; j++) { DESCR_t _n = NULVCL; *(DESCR_t *)(base + (nparams + j + 1) * 16) = _n; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_icn_zframe_args_install(void *base_p, int nparams, int nlocals) {
    char *base = (char *)base_p;
    for (int i = 0; i < nparams; i++) *(DESCR_t *)(base + (i + 1) * 16) = g_call_args[i];
    for (int j = 0; j < nlocals; j++) { DESCR_t _n = NULVCL; *(DESCR_t *)(base + (nparams + j + 1) * 16) = _n; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_ws_roots(void)
{
    extern void rt_gc_visit_descr(DESCR_t *); extern void rt_gc_visit_raw(const char **);
    for (int i = 0; i < g_name_save_top; i++) rt_gc_visit_descr(&g_name_save[i].old);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_call_prologue(rt_proc_t *p, DESCR_t *args, int nargs, int wn)
{
    rt_proc_resolve_cells(p);
    int np = p->nparams;
    { int nf = p->nformals > 0 ? p->nformals : np; if (nargs > nf) nargs = nf; }
    const char **pn = p->pnames;
    const char *rname = p->result_name ? p->result_name : p->name;
    int fbytes = (int)(PROC_FRAME_NEST_QWORDS * 8);
    if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    int save_base = rt_name_save_push(pn, p->pcells, args, nargs, np);
    { int rn_shadow = 0;
      for (int k = 0; k < np; k++) if (pn && pn[k] && !strcmp(pn[k], rname)) { rn_shadow = 1; break; }
      if (!rn_shadow) rt_name_save_push(&rname, &p->rcell, (DESCR_t *)0, 0, 1); }
    fbytes = (int)(((long)fbytes + 15L) & ~15L);
    if (g_monitor_bin) mon_emit_call_bin(p->name);
    rt_k_level++;
    rt_g_want_name = wn;
    return fbytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0)
{
    rt_k_level--;
    return frame0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_call_epilogue_ω(void)
{
    rt_k_level--;
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_proc_save_count(rt_proc_t *p)
{
    if (!p || !p->dyn_scope) return 0;
    int np = p->nparams, n = 0; const char **pn = p->pnames; const char *rname = p->result_name ? p->result_name : p->name; int rn_shadow = 0;
    for (int k = 0; k < np; k++) if (pn && pn[k]) n++;
    for (int k = 0; k < np; k++) if (pn && pn[k] && rname && !strcmp(pn[k], rname)) { rn_shadow = 1; break; }
    if (!rn_shadow && rname) n++;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_proc_epilogue_named(const char *name, int failed)
{
    rt_k_level--;
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (!p) return failed ? FAILDESCR : NULVCL;
    const char *rname = p->result_name ? p->result_name : p->name;
    DESCR_t *rcell = rt_call_fastpath_ok() ? p->rcell : (DESCR_t *)0;
    DESCR_t result = failed ? FAILDESCR : (rcell ? *rcell : NV_GET_fn(rname));
    { int base = g_name_save_top - rt_proc_save_count(p); if (base < 0) base = 0; rt_name_restore(base); }
    if (g_monitor_bin) mon_emit_return_bin(p->name, result);
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_call_epilogue_named_γ(const char *name) { return rt_proc_epilogue_named(name, 0); }
DESCR_t rt_proc_call_epilogue_named_ω(const char *name) { return rt_proc_epilogue_named(name, 1); }
void rt_c2b_arm_trap(void) { fprintf(stderr, "FATAL: CALL2BB 3b — slim open refused at RUNTIME on an fc-armed call site; the flat fallback does not exist as storage on an armed statement (registration excluded OPSYN/redefinition shapes at emit time, so this refuse names a guard the planner does not mirror — widen fc_call_ok or the probe)\n"); fflush(stderr); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_proc_call_open_slim(const char *name, int np, int nargs)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (!p || !p->fn || !p->dyn_scope || p->is_generator || p->is_variadic || p->redefined || g_call_fastpath_off || p->nparams != np || nargs > (p->nformals > 0 ? p->nformals : np)) return 0;
    rt_proc_resolve_cells(p);
    const char *rname = p->result_name ? p->result_name : p->name;
    int wn = rt_g_want_name; rt_g_want_name = 0;
    for (int k = nargs; k < np; k++) { if (p->pcells && p->pcells[k]) *p->pcells[k] = NULVCL; else if (p->pnames && p->pnames[k]) NV_SET_fn(p->pnames[k], NULVCL); }
    { int sh = 0; for (int k = 0; k < np; k++) if (p->pnames && p->pnames[k] && !strcmp(p->pnames[k], rname)) { sh = 1; break; }
      if (!sh) { if (p->rcell) *p->rcell = NULVCL; else NV_SET_fn(rname, NULVCL); } }
    if (g_monitor_bin) mon_emit_call_bin(p->name);
    rt_k_level++;
    return (long)(uintptr_t)(void *)p->fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_call_epilogue_slim_γ(DESCR_t result)
{
    rt_k_level--;
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_call_epilogue_slim_ω(void)
{
    rt_k_level--;
    return FAILDESCR;
}
static int rt_proc_call_prologue_lex(rt_proc_t *p, int nargs, int wn);
#define RT_DC_FNS_MAX 8192
static void *g_rt_dc_fns_store[RT_DC_FNS_MAX];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void **rt_pl_dc_slot(long idx) { return (idx >= 0 && idx < RT_DC_FNS_MAX) ? &g_rt_dc_fns_store[idx] : (void **)0; }
void rt_proc_set_dcfn(const char *name, void *fp) { int i = name ? rt_proc_hash_lookup(name) : -1; if (i >= 0 && i < RT_DC_FNS_MAX) g_rt_dc_fns_store[i] = fp; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dc_ok(const char *name, int nargs)
{
    static int off = -1; if (off < 0) { const char *e = getenv("SCRIP_NO_DC"); off = (e && *e == '1') ? 1 : 0; }
    if (off) return 0;
    if (name && strncmp(name, "LBL__", 5) == 0) return 0;
    { int i = name ? rt_proc_hash_lookup(name) : -1;
      if (i < 0 || i >= RT_DC_FNS_MAX) return 0;
      { rt_proc_t *p = &g_rt_gen_procs[i];
        return (!p->dyn_scope && !p->is_generator && p->jmp_entry && !p->is_variadic && !p->redefined && (p->nformals > 0 ? p->nformals : p->nparams) == nargs && nargs >= 0 && nargs <= 4) ? 1 : 0; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_dc_prep(void *fb, long suffix_off, long region_bytes, long np, long nargs, long idx)
{
    (void)idx;
    DESCR_t *zf = (DESCR_t *)fb;
    DESCR_t **pp = (DESCR_t **)((char *)fb + 16);
    DESCR_t *s0 = (nargs > 0) ? pp[0] : (DESCR_t *)0, *s1 = (nargs > 1) ? pp[1] : (DESCR_t *)0, *s2 = (nargs > 2) ? pp[2] : (DESCR_t *)0, *s3 = (nargs > 3) ? pp[3] : (DESCR_t *)0;
    if (s0) zf[1] = *s0; if (s1) zf[2] = *s1; if (s2) zf[3] = *s2; if (s3) zf[4] = *s3;
    for (long k = nargs; k < np; k++) zf[1 + k] = NULVCL;
    zf[0] = NULVCL;
    { DESCR_t *sz = (DESCR_t *)((char *)fb + suffix_off); for (long zi = 0; zi < (region_bytes - suffix_off) / 16; zi++) sz[zi] = NULVCL; }
    *(long *)((char *)fb + region_bytes + 32) = (long)rt_value_trail_mark();
    rt_k_level++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dc_leave_γ(DESCR_t r, long vtmark, void *fb)
{
    rt_k_level--;
    rt_value_trail_tidy_dead_window((int)vtmark, fb, (char *)__builtin_frame_address(0) + 16);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dc_leave_ω(long vtmark, void *fb)
{
    rt_k_level--;
    rt_value_trail_tidy_dead_window((int)vtmark, fb, (char *)__builtin_frame_address(0) + 16);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_proc_pname(const char *name, int k) { rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0; return (p && p->pnames && k >= 0 && k < p->nparams) ? p->pnames[k] : (const char *)0; }
const char *rt_proc_result_name_get(const char *name) { rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0; return p ? (p->result_name ? p->result_name : p->name) : (const char *)0; }
void *rt_gen_get_fb(void) { return (void *)0; }
typedef struct { void *gen_fb; void *cont; void *caller_fb; void *gwire; void *owire; } icn_gen_state_t;
__attribute__((visibility("hidden"))) static icn_gen_state_t  g_icn_gen_stk_buf[64];
__attribute__((visibility("hidden"))) static icn_gen_state_t *g_icn_gen_stk     = g_icn_gen_stk_buf;
__attribute__((visibility("hidden"))) static int              g_icn_gen_stk_top = 0;
__attribute__((visibility("hidden"))) static int              g_icn_gen_stk_cap = 64;
__attribute__((visibility("hidden"))) static void *g_gen_pending_cont        = (void *)0;
__attribute__((visibility("hidden"))) static void *g_gen_pending_caller____  = (void *)0;
__attribute__((visibility("hidden"))) static void *g_gen_pending_gamma_wire  = (void *)0;
__attribute__((visibility("hidden"))) static void *g_gen_pending_omega_wire  = (void *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static icn_gen_state_t *icn_gen_find(void *gen_fb) {
    for (int i = g_icn_gen_stk_top - 1; i >= 0; i--)
        if (g_icn_gen_stk[i].gen_fb == gen_fb) return &g_icn_gen_stk[i];
    return (icn_gen_state_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_gen_stk_grow(void) {
    if (g_icn_gen_stk_top < g_icn_gen_stk_cap) return;
    int nc = g_icn_gen_stk_cap * 2;
    icn_gen_state_t *nb = (icn_gen_state_t *)rt_ws_realloc(
        g_icn_gen_stk == g_icn_gen_stk_buf ? (void *)0 : (void *)g_icn_gen_stk,
        (size_t)nc * sizeof(icn_gen_state_t));
    if (!nb) return;
    if (g_icn_gen_stk == g_icn_gen_stk_buf) memcpy(nb, g_icn_gen_stk_buf, (size_t)g_icn_gen_stk_top * sizeof(icn_gen_state_t));
    g_icn_gen_stk = nb; g_icn_gen_stk_cap = nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gen_save_wires(void *gen_fb, void *gw, void *ww) {
    if (!gw) {
        for (int i = g_icn_gen_stk_top - 1; i >= 0; i--) {
            if (g_icn_gen_stk[i].gen_fb == gen_fb) {
                for (int j = i; j < g_icn_gen_stk_top - 1; j++) g_icn_gen_stk[j] = g_icn_gen_stk[j+1];
                g_icn_gen_stk_top--;
                break;
            }
        }
        return;
    }
    g_gen_pending_gamma_wire = gw; g_gen_pending_omega_wire = ww;
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    if (e) { e->gwire = gw; e->owire = ww; return; }
    icn_gen_stk_grow();
    if (g_icn_gen_stk_top >= g_icn_gen_stk_cap) return;
    g_icn_gen_stk[g_icn_gen_stk_top++] = (icn_gen_state_t){ gen_fb, (void*)0, (void*)0, gw, ww };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gen_save_cont(void *gen_fb, void *cont) {
    g_gen_pending_cont = cont;
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    if (e) e->cont = cont;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_gen_get_cont(void *gen_fb) {
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    void *v = e ? e->cont : g_gen_pending_cont;
    g_gen_pending_cont = v; return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_gen_get_gamma_wire(void *gen_fb) {
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    void *v = e ? e->gwire : g_gen_pending_gamma_wire;
    g_gen_pending_gamma_wire = v; return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_gen_get_omega_wire(void *gen_fb) {
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    void *v = e ? e->owire : g_gen_pending_omega_wire;
    g_gen_pending_omega_wire = v; return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_call_epilogue_ret(DESCR_t fret)
{
    if (IS_FAIL_fn(fret)) return rt_proc_call_epilogue_ω();
    DESCR_t frame0 = fret;
    return rt_proc_call_epilogue_γ(frame0);
}
#if defined(ZC_FRAME) && defined(ZC_FRAME_RSP) && ZC_FRAME == ZC_FRAME_RSP
__asm__(
".text\n"
".globl rt_proc_enter\n"
"rt_proc_enter:\n"
"  pushq %rbx\n"
"  pushq %r12\n"
"  pushq %r13\n"
"  pushq %r14\n"
"  pushq %r15\n"
"  movq %rdi, %rax\n"
"  leaq 2f(%rip), %rcx\n"
"  leaq 3f(%rip), %rdx\n"
"  movq g_rtcc_on@GOTPCREL(%rip), %r10\n"
"  cmpb $0, (%r10)\n"
"  je 4f\n"
"  movq rtccb@GOTPCREL(%rip), %r10\n"
"  movq 24(%r10), %rsi\n"
"  movq 32(%r10), %rdi\n"
"  movq 64(%r10), %r11\n"
"  movq 40(%r10), %r8\n"
"  movq 48(%r10), %r9\n"
"  movq 56(%r10), %r10\n"
"4:\n"
"  pushq %rdx\n"
"  pushq %rcx\n"
"  jmp *%rax\n"
"2:\n"
"  addq $16, %rsp\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  movq %rax, %rdi\n"
"  movq %rdx, %rsi\n"
"  jmp rt_proc_call_epilogue_γ\n"
"3:\n"
"  addq $16, %rsp\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  jmp rt_proc_call_epilogue_ω\n"
);
#endif
DESCR_t rt_proc_enter(void *fn);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_open_fn(void)
{
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_proc_call_prologue_lex(rt_proc_t *p, int nargs, int wn)
{
    int fbytes = (int)(PROC_FRAME_QWORDS * 8);
    if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    fbytes = (int)(((long)fbytes + 15L) & ~15L);
    if (nargs > CALL_ARGS_MAX) nargs = CALL_ARGS_MAX;
    { static int _va = -1; if (_va < 0) { const char *_e = getenv("SCRIP_VARARG_TAIL"); _va = (_e && *_e == '0') ? 0 : 1; }
      if (_va && p->is_variadic && p->jmp_entry) {
        extern DESCR_t rt_make_list(DESCR_t *args, int nargs); extern DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs); extern DESCR_t rt_make_nested_agg(DESCR_t *args, int nargs);
        int npc = p->nparams; if (npc > CALL_ARGS_MAX) npc = CALL_ARGS_MAX;
        if (npc > 0) { int fixed = npc - 1; int rest = nargs - fixed; if (rest < 0) rest = 0;
            for (int i = nargs; i < fixed; i++) g_call_args[i] = NULVCL;
            DESCR_t _tail = (p->rest_kind == 2) ? rt_make_nested_agg(rest > 0 ? &g_call_args[fixed] : (DESCR_t *)0, rest) : p->rest_kind ? rt_make_flat_agg(rest > 0 ? &g_call_args[fixed] : (DESCR_t *)0, rest) : rt_make_list(rest > 0 ? &g_call_args[fixed] : (DESCR_t *)0, rest);
            g_call_args[fixed] = _tail; } } }
    rt_k_level++;
    return fbytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_proc_call_c_lex(rt_proc_t *p, DESCR_t *args, int nargs, int wn)
{
    if (nargs > CALL_ARGS_MAX) nargs = CALL_ARGS_MAX;
    for (int i = 0; i < nargs; i++) g_call_args[i] = args ? args[i] : NULVCL;
#if defined(ZC_FRAME) && defined(ZC_FRAME_RSP) && ZC_FRAME == ZC_FRAME_RSP
    if (p->jmp_entry) {
        (void)rt_proc_call_prologue_lex(p, nargs, wn);
        return rt_proc_enter((void *)p->fn);
    }
#endif
    long fbytes = (long)rt_proc_call_prologue_lex(p, nargs, wn);
    void *fb = alloca((size_t)fbytes);
    void *fn2 = rt_frame_prep(fb, fbytes);
    DESCR_t fret = ((DESCR_t (*)(void *, long))fn2)(fb, 0);
    return rt_proc_call_epilogue_ret(fret);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_proc_call_open(const char *name, int nargs)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (!p || !p->fn) return 0;
    int wn = rt_g_want_name; rt_g_want_name = 0;
    if (p->dyn_scope) return (long)rt_proc_call_prologue(p, g_call_args, nargs, wn);
    return (long)rt_proc_call_prologue_lex(p, nargs, wn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_call_open_fnret(const char *name, int nargs)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (!p || !p->fn) return (void *)0;
    return rt_proc_call_open(name, nargs) ? (void *)p->fn : (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_call_open_det(long idx, int nargs)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return (void *)0;
    { rt_proc_t *p = &g_rt_gen_procs[idx];
      if (!p->fn || p->dyn_scope) return (void *)0;
      { int wn = rt_g_want_name; rt_g_want_name = 0;
        (void)rt_proc_call_prologue_lex(p, nargs, wn);
        return (void *)p->fn; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_call_open_det0(long idx)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return (void *)0;
    { rt_proc_t *p = &g_rt_gen_procs[idx];
      if (!p->fn || p->dyn_scope) return (void *)0;
      { int _np = p->nparams; if (_np > CALL_ARGS_MAX) _np = CALL_ARGS_MAX; for (int i = 0; i < _np; i++) g_call_args[i] = NULVCL; }
      { int wn = rt_g_want_name; rt_g_want_name = 0;
        (void)rt_proc_call_prologue_lex(p, 0, wn);
        return (void *)p->fn; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_call_open_det1(long idx, DESCR_t *a0)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return (void *)0;
    { rt_proc_t *p = &g_rt_gen_procs[idx];
      if (!p->fn || p->dyn_scope) return (void *)0;
      g_call_args[0] = *a0;
      { int _np = p->nparams; if (_np > CALL_ARGS_MAX) _np = CALL_ARGS_MAX; for (int i = 1; i < _np; i++) g_call_args[i] = NULVCL; }
      { int wn = rt_g_want_name; rt_g_want_name = 0;
        (void)rt_proc_call_prologue_lex(p, 1, wn);
        return (void *)p->fn; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_call_open_det2(long idx, DESCR_t *a0, DESCR_t *a1)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return (void *)0;
    { rt_proc_t *p = &g_rt_gen_procs[idx];
      if (!p->fn || p->dyn_scope) return (void *)0;
      g_call_args[0] = *a0; g_call_args[1] = *a1;
      { int _np = p->nparams; if (_np > CALL_ARGS_MAX) _np = CALL_ARGS_MAX; for (int i = 2; i < _np; i++) g_call_args[i] = NULVCL; }
      { int wn = rt_g_want_name; rt_g_want_name = 0;
        (void)rt_proc_call_prologue_lex(p, 2, wn);
        return (void *)p->fn; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_call_open_det3(long idx, DESCR_t *a0, DESCR_t *a1, DESCR_t *a2)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return (void *)0;
    { rt_proc_t *p = &g_rt_gen_procs[idx];
      if (!p->fn || p->dyn_scope) return (void *)0;
      g_call_args[0] = *a0; g_call_args[1] = *a1; g_call_args[2] = *a2;
      { int _np = p->nparams; if (_np > CALL_ARGS_MAX) _np = CALL_ARGS_MAX; for (int i = 3; i < _np; i++) g_call_args[i] = NULVCL; }
      { int wn = rt_g_want_name; rt_g_want_name = 0;
        (void)rt_proc_call_prologue_lex(p, 3, wn);
        return (void *)p->fn; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_proc_call_open_det4(long idx, DESCR_t *a0, DESCR_t *a1, DESCR_t *a2, DESCR_t *a3)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return (void *)0;
    { rt_proc_t *p = &g_rt_gen_procs[idx];
      if (!p->fn || p->dyn_scope) return (void *)0;
      g_call_args[0] = *a0; g_call_args[1] = *a1; g_call_args[2] = *a2; g_call_args[3] = *a3;
      { int _np = p->nparams; if (_np > CALL_ARGS_MAX) _np = CALL_ARGS_MAX; for (int i = 4; i < _np; i++) g_call_args[i] = NULVCL; }
      { int wn = rt_g_want_name; rt_g_want_name = 0;
        (void)rt_proc_call_prologue_lex(p, 4, wn);
        return (void *)p->fn; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_jmp_frame_lexprep(void *fb, long region_bytes)
{
    (void)fb; (void)region_bytes;
}
extern void   *g_pl_zf_pending_cursor;
extern long    g_pl_zf_pending_tm_lo;
extern long    g_pl_zf_pending_tm_hi;
extern int     g_pl_zf_pending_tm_off;
extern int     g_pl_zf_pending_cursor_off;
extern int     g_pl_zf_target_pcall_top;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_jmp_frame_lexprep2(void *fb, long suffix_off, long region_bytes)
{
    (void)fb; (void)suffix_off; (void)region_bytes;
}
__attribute__((visibility("default"))) void **g_pl_retry;
__attribute__((visibility("default"))) int    g_pl_retry_top;
__attribute__((visibility("default"))) int    g_pl_retry_cap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_retry_push(void *addr)
{
    if (g_pl_retry_top >= g_pl_retry_cap) {
        int nc = g_pl_retry_cap ? g_pl_retry_cap * 2 : 1024;
        void **np = (void **)rt_ws_realloc(g_pl_retry, (size_t)nc * sizeof(void *));
        if (!np) return;
        g_pl_retry = np; g_pl_retry_cap = nc;
    }
    g_pl_retry[g_pl_retry_top++] = addr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_retry_pop(void)
{
    if (g_pl_retry_top <= 0) return (void *)0;
    return g_pl_retry[--g_pl_retry_top];
}
__attribute__((visibility("default"))) void **g_pl_cp_stack;
__attribute__((visibility("default"))) int    g_pl_cp_top;
__attribute__((visibility("default"))) int    g_pl_cp_cap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cp_push(void *addr)
{
    if (g_pl_cp_top >= g_pl_cp_cap) {
        int nc = g_pl_cp_cap ? g_pl_cp_cap * 2 : 1024;
        void **np = (void **)rt_ws_realloc(g_pl_cp_stack, (size_t)nc * sizeof(void *));
        if (!np) return;
        g_pl_cp_stack = np; g_pl_cp_cap = nc;
    }
    g_pl_cp_stack[g_pl_cp_top++] = addr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_cp_pop(void)
{
    if (g_pl_cp_top <= 0) return (void *)0;
    return g_pl_cp_stack[--g_pl_cp_top];
}
__attribute__((visibility("default"))) void **g_pl_zf3_stack;
__attribute__((visibility("default"))) int    g_pl_zf3_top;
__attribute__((visibility("default"))) int    g_pl_zf3_cap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cp_push3(long tm_lo, long tm_hi, void *cont)
{
    if (g_pl_zf3_top + 3 > g_pl_zf3_cap) {
        int nc = g_pl_zf3_cap ? g_pl_zf3_cap * 2 : 1024;
        if (nc < g_pl_zf3_top + 3) nc = g_pl_zf3_top + 3;
        void **np = (void **)rt_ws_realloc(g_pl_zf3_stack, (size_t)nc * sizeof(void *));
        if (!np) return;
        g_pl_zf3_stack = np; g_pl_zf3_cap = nc;
    }
    g_pl_zf3_stack[g_pl_zf3_top++] = (void *)(uintptr_t)(uint64_t)tm_lo;
    g_pl_zf3_stack[g_pl_zf3_top++] = (void *)(uintptr_t)(uint64_t)tm_hi;
    g_pl_zf3_stack[g_pl_zf3_top++] = cont;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_cp_pop3(long *tm_lo, long *tm_hi)
{
    if (g_pl_zf3_top < 3) { if (tm_lo) *tm_lo = 0; if (tm_hi) *tm_hi = 0; return (void *)0; }
    void *cont     = g_pl_zf3_stack[--g_pl_zf3_top];
    if (tm_hi) *tm_hi = (long)(uintptr_t)g_pl_zf3_stack[--g_pl_zf3_top]; else --g_pl_zf3_top;
    if (tm_lo) *tm_lo = (long)(uintptr_t)g_pl_zf3_stack[--g_pl_zf3_top]; else --g_pl_zf3_top;
    return cont;
}
__attribute__((visibility("default"))) void   *g_pl_zf_pending_cursor;
__attribute__((visibility("default"))) long    g_pl_zf_pending_tm_lo;
__attribute__((visibility("default"))) long    g_pl_zf_pending_tm_hi;
__attribute__((visibility("default"))) int     g_pl_zf_pending_tm_off;
__attribute__((visibility("default"))) int     g_pl_zf_pending_cursor_off;
__attribute__((visibility("default"))) int     g_pl_zf_target_pcall_top;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_zf_resume_set(void *cursor, long tm_lo, long tm_hi, int tm_off, int cursor_off)
{
    g_pl_zf_pending_cursor = cursor;
    g_pl_zf_pending_tm_lo = tm_lo;
    g_pl_zf_pending_tm_hi = tm_hi;
    g_pl_zf_pending_tm_off = tm_off;
    g_pl_zf_pending_cursor_off = cursor_off;
    g_pl_zf_target_pcall_top = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_zf_resume_clear(void) { g_pl_zf_pending_cursor = (void *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_frame_prep(void *fb, long fbytes)
{
    (void)fb; (void)fbytes;
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_byname_alpha_on(void) { static int live = -1; if (live < 0) { const char *e = getenv("SCRIP_BYNAME_ALPHA"); live = (e && e[0] == '0') ? 0 : 1; } return live; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs)
{
    if (!name) return FAILDESCR;
    int _wn = rt_g_want_name; rt_g_want_name = 0;
    rt_proc_t *p = rt_proc_find(name);
    if (!p || !p->fn) return FAILDESCR;
    if (!p->dyn_scope) return rt_proc_call_c_lex(p, args, nargs, _wn);
    { void *afn = (rt_byname_alpha_on() && !strchr(name, '$')) ? rt_dyn_alpha_fn(name, (void *)0) : (void *)0;
      if (afn) { extern DESCR_t rt_tiny_record_enter(void *fn, long nargs); int _n = nargs < CALL_ARGS_MAX ? nargs : CALL_ARGS_MAX; if (_n < 0) _n = 0;
                 for (int i = 0; i < _n; i++) g_call_args[i] = args[i]; rt_g_want_name = _wn; return rt_tiny_record_enter(afn, (long)_n); } }
    (void)rt_proc_call_prologue(p, args, nargs, _wn);
    return rt_proc_enter((void *)p->fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_proc_direct(long idx, DESCR_t *args, int nargs)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return FAILDESCR;
    rt_proc_t *p = &g_rt_gen_procs[idx];
    if (!p->fn) return FAILDESCR;
    int _wn = rt_g_want_name; rt_g_want_name = 0;
    if (!p->dyn_scope) return rt_proc_call_c_lex(p, args, nargs, _wn);
    (void)rt_proc_call_prologue(p, args, nargs, _wn);
    return rt_proc_enter((void *)p->fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_proc_index_of(const char *name)
{
    if (!name) return -1;
    unsigned h = (unsigned)(((uintptr_t)name >> 4) & DCR_CELL_CACHE_MASK);
    if (g_proc_idx_key[h] == name) { int ci = g_proc_idx_slot[h]; if (ci < g_rt_gen_proc_count && g_rt_gen_procs[ci].name == name) return ci; }
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_proc_idx_key[h] = name; g_proc_idx_slot[h] = i; return i; } }
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
    if (g_proc_idx_key[h] == name) { int ci = g_proc_idx_slot[h]; if (ci < g_rt_gen_proc_count && g_rt_gen_procs[ci].name == name) return &g_rt_gen_procs[ci]; }
    { int i = rt_proc_hash_lookup(name); if (i >= 0) { g_proc_idx_key[h] = name; g_proc_idx_slot[h] = i; return &g_rt_gen_procs[i]; } }
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
void rt_define_site(const char *name, const char *params_csv, int nparams, int nformals, int frame_bytes, void *fn)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (p) { if (fn && p->fn && (void *)p->fn != fn) p->redefined = 1; if (fn) p->fn = (bb_box_fn)fn; if (nparams) p->nparams = nparams; p->nformals = nformals; if (frame_bytes) p->frame_bytes = frame_bytes; p->dyn_scope = 1; p->jmp_entry = 1; return; }
    { int np = nparams > 0 ? nparams : 0; const char **pn = (const char **)calloc((size_t)(np + 1), sizeof(const char *)); char *dup = params_csv ? strdup(params_csv) : (char *)0; int k = 0;
      if (pn && dup) { char *sv = (char *)0; for (char *t = strtok_r(dup, ",", &sv); t && k < np; t = strtok_r((char *)0, ",", &sv)) pn[k++] = t; }
      rt_proc_register(name, pn, np); p = rt_proc_find(name);
      if (p) { p->pnames_owned = 1; p->fn = (bb_box_fn)fn; p->dyn_scope = 1; p->jmp_entry = 1; p->nformals = nformals; p->frame_bytes = frame_bytes; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_define_tiny_ok(const char *name, int nargs)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    (void)nargs; return (p && p->dyn_scope && !p->is_generator && !p->is_variadic && !p->redefined) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_define_returns_by_frame(const char *name)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    return (p && p->jmp_entry && !p->dyn_scope) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_define_query(const char *name, int *np_out, int *nf_out, int *fb_out, void **fn_out)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (np_out) *np_out = p ? p->nparams : 0; if (nf_out) *nf_out = p ? p->nformals : 0; if (fb_out) *fb_out = p ? p->frame_bytes : 0; if (fn_out) *fn_out = p ? (void *)p->fn : (void *)0;
    if (!p || !p->pnames || p->nparams <= 0) return "";
    { size_t need = 1; for (int k = 0; k < p->nparams; k++) need += (p->pnames[k] ? strlen(p->pnames[k]) : 0) + 1; char *csv = (char *)malloc(need); if (!csv) return ""; csv[0] = 0;
      for (int k = 0; k < p->nparams; k++) { if (k) strcat(csv, ","); if (p->pnames[k]) strcat(csv, p->pnames[k]); } return csv; }
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
DESCR_t c_rt_size_d(uint64_t lo, uint64_t hi)
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
DESCR_t c_rt_list_bang_at(DESCR_t obj, int64_t idx)
{
    (void)obj; (void)idx;
    rt_bomb("c_rt_list_bang_at: DELETED (s196 Lon one-to-maintain) — rt_list_bang_at in rtx_icnagg.S is the sole spelling (zero bails, gate removed)");
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_nofail_abort(void)
{
    extern void core_runtime_error(int code, const char *msg);
    core_runtime_error(35, "unexpected failure in -nofail mode");
    exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_register_rec(const rt_proc_reg_rec_t *r)
{
    _Static_assert(sizeof(rt_proc_reg_rec_t) == 64, "ONE-REG record is 64 bytes");
    _Static_assert(__builtin_offsetof(rt_proc_reg_rec_t, pnames) == 32 && __builtin_offsetof(rt_proc_reg_rec_t, nparams) == 40 && __builtin_offsetof(rt_proc_reg_rec_t, flags) == 52, "ONE-REG field offsets are law");
    if (!r || !r->name) return;
    extern void rt_proc_set_dyn_scope(const char *, int); extern void rt_proc_set_result_name(const char *, const char *); extern void rt_proc_set_nparams(const char *, int); extern void rt_proc_set_nformals(const char *, int);
    extern void rt_proc_set_pname(const char *, int, const char *); extern void rt_proc_set_jmpentry(const char *, int); extern void rt_proc_set_dcfn(const char *, void *);
    if (r->flags & 1) { rt_proc_register(r->name, (const char **)r->pnames, r->nparams); rt_proc_set_dyn_scope(r->name, 1); if (r->result_name) rt_proc_set_result_name(r->name, r->result_name); }
    if (r->fn) rt_proc_set_fn(r->name, (bb_box_fn)r->fn);
    rt_proc_set_nparams(r->name, r->nparams);
    rt_proc_set_nformals(r->name, r->nformals);
    if (!(r->flags & 1) && r->pnames) for (int k = 0; k < r->nparams && r->pnames[k]; k++) rt_proc_set_pname(r->name, k, r->pnames[k]);
    if (r->frame_bytes > 0) rt_proc_set_frame_bytes(r->name, r->frame_bytes);
    if (r->flags & 2) rt_proc_set_zstatic(r->name, 1);
    if (r->flags & 4) rt_proc_set_variadic(r->name, 1);
    if (r->rest_kind) rt_proc_set_rest_kind(r->name, r->rest_kind);
    if (r->named_rest) rt_proc_set_named_rest(r->name, r->named_rest);
    rt_proc_set_jmpentry(r->name, (r->flags >> 4) & 1);
    if (r->dcfn) rt_proc_set_dcfn(r->name, r->dcfn);
    if (r->flags & 8) rt_proc_set_generator(r->name, 1);
}
