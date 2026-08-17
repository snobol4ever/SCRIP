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
/* R12-EXTERN (Lon s173): mode-3's OUTSIDE sets the environment register — twin of the mode-4 wrapper's `mov r12, [RT_DCAP_TOP]` (scrip.c).  The blob no longer self-seeds (xa_flat REG-6 outer seed deleted); every graph assumes r12 = live pend/dcap top on entry.  push/pop r12 also closes the old in-blob seed's caller-r12 ABI clobber.  Literal address drift-locked below. */
/* R12-FREE-1 (Lon 2026-07-29, GOAL-ZETA-FOUR): the r12 environment seed is DELETED from the outer thunk -- the pend top is cell-resident at [RT_DCAP_TOP] (pattern_match.c g_dcap_top alias), nothing reads r12, and r12 is vacated for ZC_STORAGE_FRAME_R12.  The old push r12 was ALSO the 16-byte call-alignment adjuster, so its removal is paired with an explicit sub/add rsp,8 bracket. */
/* ONE-SHOT-BRIDGE-M3 (s22q): the adjuster is 16, NOT 8, and the 8 was correct only for the PRE-BRIDGE arrival parity.  MEASURED, not reasoned: mode-4's `main` jmps into α having moved 24 bytes (sub rsp,8 + push rdi + push rsi), so α arrives rsp≡0 (mod 16); mode-3 CALLs α through this thunk, and `call` pushes 8 more, so the old `sub $8` delivered α at rsp≡8 -- an 8-byte skew against the SAME α preamble (bb_glue_framed_enter's `push ___` + `sub rsp,8`), inherited by every C call the graph then makes.  Witness 002_output_integer_literal: graph-side NV_SET_fn entry measured rsp%16 = 8 in m3 vs 0 in m4, SIGSEGV in glibc dl_iterate_phdr's movaps during gc_static_segs_init.  `sub $16` puts α at rsp≡0 in BOTH modes: the bridge's parity contract is now one law, and mode-3 keeps `ret` (no PLT in the JIT slab) while agreeing with mode-4 on arrival. */
/* ZW-3 R12-FREE-1 REVERSAL (s23l): r12 is the LIVE CAS/dcap top register. rt_outer_call must seed r12
 * from [RT_DCAP_TOP] before entering the graph (the graph assumes r12 live on entry) and save/restore the
 * CALLER's r12 across the call (SysV callee-saved contract).  The sub/add $16 alignment is preserved.
 * 0x70000000 == RT_DCAP_TOP == RT_PIN_BASE+0 (rtx_init.c _Static_assert locks this). */
__asm__(".globl rt_outer_call\n.type rt_outer_call, @function\n"
        "rt_outer_call:\n"
        "  push %r12\n"
        "  sub $8, %rsp\n"
        "  mov %rdi, %rax\n"
        "  mov %rsi, %rdi\n"
        "  mov %rdx, %rsi\n"
        "  mov 0x70000000, %r12\n"
        /* RTCC RC-5-GVA INBOUND LOAD: same GOT-indirect pattern as rt_proc_enter/rt_chain_enter.   */
        /* rax=fn-ptr rdi/rsi=wire-args r12=dcap-top are load-bearing; r8/r9/r10/r11 are free.      */
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
/* ICN-FR-5 RESTORE (s238): zero the delta cursor register (r14) once at program entry, then tail-jump into the shared outer thunk unchanged.  &pos compiles to r14+1 in bb_keyword_icon.cpp UNCONDITIONALLY (the g_scan_regs_live gate was deleted deliberately: a ?-less scanning callee runs inline scan primitives on the caller's ambient subject, and the emit-time in_scan flag reads 0 there while the registers are live at runtime -- routing that read through the scan_pos global returned a stale cursor).  That template therefore rests on a stated invariant, "outside any scan r14 is 0 so &pos reads 1", which nothing established on this path: at main entry r14 holds libc residue, so &pos read garbage (measured 4304233 == 0x41B069) outside any scan.  The two original ICN-FR-5 sites (icn_zf_main_call below, and the mode-4 text preamble in scrip.c) are both gated on zframe_graph && !icn_cells_graph; Z-1 stamped icn_cells_graph=1 by DEFAULT for every Icon graph, which turned that gate permanently false and silently disabled the initialization on every arm.  Separate symbol rather than an edit to rt_outer_call because that thunk is SHARED with SNOBOL4/Prolog/Pascal/Raku and the SN4 match family also uses r14 -- the driver selects this variant only for Icon, so every other language stays byte-identical BY CONSTRUCTION.  r14 is callee-saved and rt_outer_call never touches it, so the zero survives into the graph.  No new global: this is a function symbol, not file-scope mutable state.                                                          */
/* ICN-FR-5 ABI HARDENING (s238-b): the first cut of this thunk was `xor %r14d,%r14d; jmp rt_outer_call`, which RETURNS to its C caller with r14 destroyed.  r14 is CALLEE-SAVED under SysV and rt_outer_call returns normally (unlike icn_zf_main_call below, whose jmp is correct precisely because fn never returns), so the tail-jump form silently broke the contract toward driver code.  MEASURED at the time of writing: `main` allocates r14 zero times, so it did not bite -- but this is exactly the latent class the ICN-FR-5 CLOBBER-FIX note below records being bitten by once already (GCC's clobber-driven r14 spill around the call site corrupted the stack), and `main` is a large function under concurrent edit.  push/pop costs two instructions and removes the hazard permanently.  ALIGNMENT: entry rsp%16==8, push makes it 0, so the inner call arrives at the ABI-standard 8 -- the same parity rt_outer_call's own sub/add $8 bracket is calibrated for.  */
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
/* D08 FIX (s22 CLIMB): match-time integer fetch for LEN(*var)/POS(*var)/etc. deferred-integer primitives.
 * Called from bb_match_len (and future siblings) alpha/beta when op_sval starts with '*'.
 * Reads the named variable at match time, coerces string→int per manual p.86 type-conversion rules,
 * returns the integer value (≥0) or -1 on failure (template treats negative as omega→fail). */
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
/* MODE34-5b FIX (this session): match-time STRING fetch for SPAN(var)/ANY(var)/NOTANY(var)/BREAK(var)/
 * BREAKX(var) dynamic charset primitives -- the string-argument twin of rt_pat_prim_int above.  Reads
 * the named variable at match time (by NAME, never via a pre-chain operand slot/FRQ offset), so it is
 * immune to the enclosing MATCH_BEGIN/MATCH_ASSIGN_SAVE/own-preamble stack growth that corrupts the
 * legacy op_sa/FRQ(off+8) read -- gdb-measured this session: 5b's SPAN(WS) reads landed 240 bytes off
 * the real DESCR pointer field, pulling stack garbage as the "charset", so the membership loop never
 * matched and SPAN always reported zero characters.  Writes the coerced string's pointer into *out_ptr
 * and its length into *out_len; returns 0 on success, -1 if the variable does not coerce to a string
 * (template treats negative as omega->fail, matching rt_pat_prim_int's convention). */
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
/* RTX-FUNC-1 — rt_k_level_p: the EXPORTED handle on rt_k_level, for EMITTED code only.  bb_define's α now does k_level++ inline, so the m4 executable must name this cell; a hidden symbol is
 * absent from libscrip_rt.so's .dynsym and every m4 DEFINE-bearing program would fail to LINK (the g_cap_gen class, pattern_match.c:737, 173/316).  ⛔ BUT rt_k_level CANNOT SIMPLY BE PROMOTED: the
 * hand-written rtx_call.S / rtx_plcall.S reach it with direct R_X86_64_PC32, which is legal ONLY while it is non-preemptible — promoting it fails the .so link outright ("relocation R_X86_64_PC32
 * against symbol rt_k_level can not be used when making a shared object", measured this session).  Its hidden visibility is LOAD-BEARING for the in-.so asm.  So the cell stays hidden and this
 * pointer carries it across the .so boundary: emitted code pays one extra load (GOT → pointer → cell) and both media use the identical shape.  ⇒ The RTX-FUNC-1 rung's "promote to hidden" reading is
 * backwards for emitted-code reach, AND a naive promotion in the other direction breaks the runtime's own asm — the two constraints are opposed and this alias is what satisfies both. */
int * const rt_k_level_p = &rt_k_level;
#define PROC_FRAME_QWORDS 512
#define CALL_ARGS_MAX     64
typedef struct {
    const char *name; bb_box_fn fn; const char **pnames; int nparams; int frame_nslots; int decl_level; uint64_t byref_mask;
    int frame_bytes; DESCR_t **pcells; DESCR_t *rcell; int cells_done; int is_generator; int dyn_scope; const char *result_name; int is_variadic; int rest_kind; int named_rest; int jmp_entry; int redefined; int zstatic; int pnames_owned; int nformals;   /* NPSPLIT (s22w): FORMALS-ONLY bound (arg-bind / excess-clamp / dc+slim admissions); 0 = unsplit registrant, consumers fall back to nparams.  nparams KEEPS the full-name-set meaning (save/restore span, pname bound, pad loop) — SPITBOL Ch.8: ALL prototype names save+null-init, only formals bind, excess args evaluated then IGNORED.  APPENDED LAST: rtx_call.S bakes PROC_FN/PROC_NAME/PROC_ISGEN offsets (the _Static_asserts below), so this field may never move earlier. */   /* rest_kind names WHAT differs about the variadic tail binding, never WHICH language asked: REST_LIST(0) = the DT_DATA list rt_make_list builds; REST_FLAT_AGG(1) = the SOH-joined flat aggregate (canonical from-slurpy-flat, List.rakumod:271) that .elems/subscripts/reductions already understand. NCB-1d: 1 = the emitted body is a jmp-entry blob (armed by the driver proc loops, = !is_generator for table procs); 0 = call-regime body (generators, blocks/rules registered outside the loops).  The C transfer fns select the window by THIS recorded fact, never by re-deriving the emit-side predicate.  PS-1b (s151): zstatic = 1 iff this proc's blob graph was DEFER/VALUE-free with a known region (emit-side emit_graph_zstatic); default 0 = conservative, so an unregistered proc reads as chain-path safe. */
} rt_proc_t;
_Static_assert(__builtin_offsetof(rt_proc_t, fn) == 8, "rtx_call.S bakes PROC_FN for the rt_proc_open_fn port (RTX-4 slice 3); confirmed from emitted -O0 code as mov 0x8(%rax),%rax");
_Static_assert(__builtin_offsetof(rt_proc_t, name) == 0 && __builtin_offsetof(rt_proc_t, is_generator) == 0x4c, "rtx_call.S bakes PROC_NAME and PROC_ISGEN");
_Static_assert(__builtin_offsetof(rt_proc_t, dyn_scope) == 80 && sizeof(rt_proc_t) == 128, "rtx_plcall.S bakes PROC_DYN_SCOPE and the shl 7 index stride (RTX-1-PL)");
_Static_assert(__builtin_offsetof(rt_proc_t, frame_bytes) == 48, "rtx_plcall.S records this in its offset table; the fbytes computation is elided, not baked");
__attribute__((visibility("hidden"))) rt_proc_t    *g_rt_gen_procs = (rt_proc_t *)0;
__attribute__((visibility("hidden"))) int           g_rt_gen_proc_count = 0;
static int           g_rt_gen_proc_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int          *g_proc_hsl = (int *)0;
static unsigned      g_proc_hcap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned rt_proc_fnv(const char *s) { unsigned h = 2166136261u; while (*s) { h ^= (unsigned char)*s++; h *= 16777619u; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* LADDER AB (2026-08-09): fn_cell initial stub — fires error 022 "Undefined function called" (manual ch.10 p.140, error table entry 22).  fn_cell$<FN> is initialized to this address at program      */
/* start; DEFINE's residual runtime action replaces it with &<FN>_act_α.  Reached by jmp [fn_cell] on the AB-3 call path when DEFINE has not yet executed (or never will).  __attribute__((noreturn))  */
/* because core_runtime_error does not return; absent that, the callee-save discipline of the AB frame's α would leave rsp/___ intact but the call chain is irrelevant — this function never returns.   */
__attribute__((noreturn)) void rt_ab_undef_fn_stub(void) { core_runtime_error(22, "Undefined function called"); __builtin_unreachable(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *gva_register(const char **names, DESCR_t *cells, int n) {
    if (!cells) return cells;
    for (int k = 0; k < n; k++) { const char *nm = names ? names[k] : (const char *)0; if (!nm) continue; (void)NV_bind_gva(nm, &cells[k]); }
    return cells;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_GVA_ISLAND_BYTES ((size_t)16u << 20)
__attribute__((constructor)) static void rt_pin_init(void) {
    void * p = mmap((void *)RT_PIN_BASE, RT_PIN_BYTES, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (p != (void *)RT_PIN_BASE) { fprintf(stderr, "rt_pin_init: RT_PIN_BASE 0x%lx unavailable (got %p) -- REG-0 tripwire, see RUNG REG-MAP\n", (unsigned long)RT_PIN_BASE, p); abort(); }
    void * g = mmap((void *)RT_GVA_VA, RT_GVA_ISLAND_BYTES, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (g != (void *)RT_GVA_VA) { fprintf(stderr, "rt_pin_init: RT_GVA_VA 0x%lx unavailable (got %p) -- REG-1 tripwire, see RUNG REG-MAP\n", (unsigned long)RT_GVA_VA, g); abort(); }
    *(volatile uint64_t *)RT_AB_NRET   = 0;   /* AB-2: NRET discriminator init — no pending NRETURN */
    { extern void rt_dcap_lazy_init(void); rt_dcap_lazy_init(); }   /* REG-6 PEND-PROMOTE: the outer-graph prologue SEEDS r12 from [RT_DCAP_TOP] before any match runs, so the island init can no longer ride rt_match_enter's lazy call alone -- chain it here, after the pin map it writes through, deterministic in BOTH modes (this constructor lives in scrip and libscrip_rt.so alike).  rt_match_enter's call stays as an idempotent no-op. */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* AB-2 STRICT LEAVES — C residue of the AB activation/return protocol (frame-setup and teardown owned by the emitted α/β; these carry only the C-world bookkeeping that must run near a C boundary). */
/* AB-2: forward declarations needed by the strict leaves (defined further down in this file) */
extern int rt_g_want_name;
extern int rt_g_ret_by_name;
DESCR_t rt_nret_fix(DESCR_t r, int wn);   /* s98: EXPORTED — the staged-call det landings consult it through the RTCC veneer (by-name deref at value sites, manual p.133); was hidden while only rt.c-internal */
int rt_value_trail_mark(void);
void rt_value_trail_tidy_dead_window(int mark, void *fb, void *top);
/* rt_ab_enter_env: called from α AFTER the frame is established and the save-set is spilled.                                                                                                          */
/*   Snapshots Σ/Σlen into the frame (mid-match protection); snapshots wn; marks the value trail;                                                                                                      */
/*   increments rt_k_level (= &FNCLEVEL; kw_fnclevel is the keyword readable cell, same value).                                                                                                        */
/*   Returns vtmark so α can store it in AB_OFF_VTMARK without a second C crossing.                                                                                                                     */
int rt_ab_enter_env(void *frame)
{
    char *fb = (char *)frame;
    *(uint64_t *)(fb + AB_OFF_SIGMA)    = (uint64_t)(uintptr_t)Σ;
    *(uint64_t *)(fb + AB_OFF_SIGMALEN) = (uint64_t)(int64_t)Σlen;
    *(uint64_t *)(fb + AB_OFF_WN)       = (uint64_t)(int64_t)rt_g_want_name; rt_g_want_name = 0;
    int vtm = rt_value_trail_mark();
    *(uint64_t *)(fb + AB_OFF_VTMARK)   = (uint64_t)(int64_t)vtm;
    rt_k_level++;
    kw_fnclevel = (int64_t)rt_k_level - 1;   /* &FNCLEVEL counts program-defined depth; rt_k_level starts at 1 (outermost) */
    return vtm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_ab_leave_env: called from β BEFORE the frame is torn down (LEAVE not yet executed).                                                                                                              */
/*   Performs the value-trail dead-window tidy (GC correctness: clears cells that lived only inside                                                                                                     */
/*   this activation); restores Σ/Σlen; decrements rt_k_level / kw_fnclevel.                                                                                                                           */
/*   result is the candidate DESCR (rax:rdx on arrival at β, pre-restore); is_fail = 1 for FRETURN.                                                                                                   */
/*   Returns rt_nret_fix(result, wn) — the caller's rax:rdx after restore.                                                                                                                             */
DESCR_t rt_ab_leave_env(void *frame, DESCR_t result, int is_fail)
{
    char *fb = (char *)frame;
    int vtm  = (int)(int64_t)*(uint64_t *)(fb + AB_OFF_VTMARK);
    int wn   = (int)(int64_t)*(uint64_t *)(fb + AB_OFF_WN);
    rt_value_trail_tidy_dead_window(vtm, (void *)fb, (char *)fb + 16);   /* dead window = [fb, fb+16) = the ___-push slot; mirrors slim epilogue's RSP-F-2 form */
    Σ    = (const char *)(uintptr_t)*(uint64_t *)(fb + AB_OFF_SIGMA);
    Σlen = (int)(int64_t)*(uint64_t *)(fb + AB_OFF_SIGMALEN);
    rt_k_level--;
    kw_fnclevel = (int64_t)rt_k_level - 1;
    if (is_fail) return FAILDESCR;
    return rt_nret_fix(result, wn);
}
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
    return p->nformals > 0 ? p->nformals : p->nparams;   /* NPSPLIT (s22w): 0 = unsplit registrant -> the historical conflated bound */
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
/* NCB-1d REGIME RECORD — the driver proc loops (the same four that bracket emit_jmp_entry_for_proc) record here whether the proc's EMITTED body is a jmp-entry blob, in-process for mode 3 and via the
 * printed startup registration for mode 4.  Blocks/rules/methods registered through rt_proc_set_fn outside those loops keep the default 0 and stay on the call-regime C window. */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_g_ret_by_name = 0;
int rt_g_want_name = 0;
DESCR_t rt_nret_fix(DESCR_t r, int wn);   /* s98: EXPORTED — the staged-call det landings consult it through the RTCC veneer (by-name deref at value sites, manual p.133); was hidden while only rt.c-internal */
DESCR_t rt_nret_fix(DESCR_t r, int wn) { if (rt_g_ret_by_name) { rt_g_ret_by_name = 0; if (!wn && r.v == DT_N) { extern DESCR_t rt_deref(DESCR_t); r = rt_deref(r); } } rt_g_want_name = wn; return r; }
DESCR_t rt_nret_fix_tiny(DESCR_t r, int unused_edx) { (void)unused_edx; int wn = rt_g_want_name; DESCR_t o = rt_nret_fix(r, wn); rt_g_want_name = 0; return o; }
/* NCB-1 leaves (defined below, beside the dyn trampolines they were split out of). */
long    rt_proc_call_open(const char *name, int nargs);
void   *rt_frame_prep(void *fb, long fbytes);
void   *rt_proc_open_fn(void);
DESCR_t rt_proc_enter(void *fn);
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
DESCR_t rt_proc_call_epilogue_ω(void);
DESCR_t rt_proc_call_epilogue_ret(DESCR_t fret);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* TINY RECORD ENTER (s104; ARG CONTRACT CORRECTED s118) — the C twin of the emitted TINY dyn call site, re-measured verbatim from the PASSING 2-arg `R('a','b')` asm (probe/opsyn/a_reg_only): the
 * record is FIVE quads {nargs, gamma, omega, argbytes, argbase} and the ARGS RIDE THE STACK ABOVE THE RESULT CELL IN REVERSE ORDER (last arg nearest, at [rsp+16]) — the callee computes formal i at
 * `caller_rsp + argbytes - 16*i` (`lea r8,[rsp+own]; mov rdi,[rcx+24]; add rdi,r8`) and NEVER adds argbase, so the arg block is PINNED at [rsp+16) and cannot be relocated.  s104-s117 wrote a THREE-quad
 * record AT [rsp+16] — i.e. on top of the arg block — so a by-name call published nargs=0 and every formal bound null (the `GOT-` vs `GOT-ab` class); widening that 0 alone makes the callee read the
 * record's own gamma/omega quads as args (SIGSEGV), which is why the count and the layout must move together.  Contract in: rdi = fn (sealed alpha$ cell content), rsi = nargs; args come from the staged
 * g_call_args channel (the REGAIN-1 arg medium) — no new state.  Contract at jmp: rsp = &result cell (16B), [rsp+16 .. 16+16n) = args reversed, rcx = &record (parked ABOVE the args, callee-read-only).
 * Contract back: callee restores rsp to its jmp-time value, stores the DESCR into the cell and jmps the recorded landing; the landing re-seats rax:rdx and runs the same NRETURN consult the emitted
 * landings run (rt_nret_fix_tiny).  Frame is now VARIABLE (56+16n), so the landings unwind off RBP rather than a constant: rbp is NOT in the VM register contract (rbx/r9/r10/r11/r12/r13/r14/r15 are) and
 * emitted bodies either never touch it or push/pop it symmetrically per the zeta-STANDING discipline.  Alignment: entry rsp≡8 (mod 16), +8 rbp ⇒ ≡0, +40 five pushes ⇒ ≡8, −(56+16n) ⇒ ≡0 at the jmp. */
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
DESCR_t rt_ret_faildescr(void) { rt_g_want_name = 0; rt_g_ret_by_name = 0; return FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_dyn_alpha_fn(const char *name, void *fallback);
DESCR_t rt_ret_faildescr(void);
DESCR_t rt_call_proc_descr(const char *name, int nargs)
{
    rt_proc_t *p = rt_proc_find(name);
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
    if (p->dyn_scope) { void *afn = rt_dyn_alpha_fn(name, (void *)0); if (afn) { extern DESCR_t rt_tiny_record_enter(void *fn, long nargs); int _n = nargs < CALL_ARGS_MAX ? nargs : CALL_ARGS_MAX; return rt_tiny_record_enter(afn, (long)(_n < 0 ? 0 : _n)); } }   /* s104 TINY RECORD ENTER: sealed alpha$ target speaks the record contract and owns its own bookkeeping — no C open.  s118: the arity now rides in AND the shim lays the args out where the callee reads them; the pre-s118 comment here claimed the count already rode in while the shim hardcoded 0 and the entry point took no count at all — code and comment disagreeing, the file's own spelled-twice hazard.  Args are already staged in g_call_args by this entry point's callers (the same channel rt_proc_call_open reads at :1701). */
    long fbytes = rt_proc_call_open(name, nargs);
    if (!fbytes) return FAILDESCR;
    if (!p->dyn_scope) {
        if (p->jmp_entry) return rt_proc_enter((void *)p->fn);   /* NCB-1d: the RECORDED regime — a jmp-entry blob entered by C `call` would run its sub/wire prologue with garbage rcx/rdx and jmp a dead wire at γ (the blk_calls_sub crash); the open above already pushed the lex record, so the two-landing shim completes the same open→transfer→epilogue sequence */
        void *fb = alloca((size_t)fbytes);
        void *fn2 = rt_frame_prep(fb, fbytes);
        DESCR_t fret = ((DESCR_t (*)(void *, long))fn2)(fb, 0);
        return rt_proc_call_epilogue_ret(fret);
    }
    return rt_proc_enter(rt_dyn_alpha_fn(name, (void *)p->fn));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DYN ALPHA TARGET (s104) — the by-name dyn transfer for m3 DEFINEs: p->fn from rt_define_site is the generic entry thunk (rt_goto_transfer into a label chain, wrong protocol for emitted bodies —
 * rip=5 crash class, APPLY('F') repro).  The WORKING staged sites jump via the alpha$<FN> cell m3_seal_entry_cells fills with <FN>_alpha; prefer that same target here.  WIP, DEFAULT OFF (opt-in SCRIP_DYN_ALPHA=1): the cell target still needs the staged rcx-record entry contract. */
void *rt_dyn_alpha_fn(const char *name, void *fallback)
{
    static int live = -1; if (live < 0) { const char *e = getenv("SCRIP_DYN_ALPHA"); live = e ? (e[0] != '0') : 1; }   /* s108 (Lon ruling in-chat: "if they need to be on, then turn them on"): DEFAULT ON -- the NRET_CAP/DYN_ALPHA pair takes beauty_suite/semantic_driver green (measured this seat, pair on = PASS, off = rc134 abort).  Killswitch inverted, not deleted: SCRIP_DYN_ALPHA=0 restores the pre-s108 default byte-identically. */
    if (!live || !name) return fallback;
    { extern void *bb_ab_fn_cell_ptr(const char *); char cn[264]; snprintf(cn, sizeof cn, "alpha$%s", name);
      void **cell = (void **)bb_ab_fn_cell_ptr(cn);
      return (cell && *cell) ? *cell : fallback; }
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
/* GENP slice-2 (Lon 2026-07-17 directive: "co-expressions and generator PROCEDURES get their OWN stack PER INSTANCE"): a generator-proc activation = a scrip_coctx_t on its own pthread stack (GC-heap
 * carved + registered per GC-U-6/7, rt_coexpr.c).  The jmp-entry body runs UNCHANGED on the instance stack (self-allocates, recursion/depth all natural); bb_suspend's flat_gen arm transmits the value
 * via rt_genp_yield (scrip_coret) and resumes by falling through when the caller re-activates; real return/fail unwind through rt_proc_enter's existing γ/ω landings into rt_genp_entry_c, which
 * transmits the final value (done=1) or failure (done=2) and parks — scrip_coexpr_destroy joins the parked thread.  done: 0=live(yield) 1=returned(final value, not resumable) 2=failed. */
typedef struct rt_genp_s {
    struct rt_genp_s *next;                                                                             /* offset 0 — live-instance list, the resume-window discriminator (rt_genp_lookup)   */
    uint64_t          regs[5];                                                                          /* offsets 8..47: caller's rbx r12 r13 r14 r15, restored by rt_genp_thread_entry     */
    scrip_coctx_t     co;
    DESCR_t           args[CALL_ARGS_MAX];
    int               nargs;
    void             *fn;
    const char       *name;
    int               done;
    int               first_done;                                                                       /* ONE-POP law on the instance thread: only the first γ/ω delivery runs an epilogue leaf (pops the pcall record); resumed deliveries pass through */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GENP C-window spine protocol (s94 repair): the s92 GENP-SPINE made generator bodies suspend via xa_flat's RETAINING γ epilogue — result preloaded rdi:rsi, a 16B resume record {res-landing, callee
 * ___} left AT the deep frontier, γ wire jumped, NO unwind.  rt_proc_enter's γ landing pops 5 saved regs — correct for det bodies (fully unwound before the jmp) but at the retained frontier it EATS
 * the resume record plus 24B of live frame into rbx/r12..r15 and the next wire jmps frame junk (rip=r12 class).  This shim mirrors bcps_spine_gen_arm's call-site contract on the instance thread:
 * enter the blob `jmp rax` with rcx/rdx = γ/ω wires; the landings POP NOTHING (callee-saved ride through scrip_coret's ABI preservation); first-vs-resumed is the instance once-flag (ONE-POP law —
 * rt_proc_call_open pushed ONE pcall record, only the first delivery runs an epilogue leaf); every γ delivery parks in scrip_coret and, on re-activation, resumes the recorded suspend via
 * `jmp qword [rsp]` (the record sits AT the frontier by LIFO balance on this instance's OWN stack); ω and the pushed SENTINEL record (post-`return` slot-poison resumption arrives fully unwound —
 * [rsp] is then the sentinel's landing) park the instance failed.  Three sentinel qwords keep rt_proc_enter's 16-byte entry alignment convention (call 8 + 24 = 32). */
static __thread rt_genp_s *g_genp_self = (rt_genp_s *)0;
extern void rt_genp_spine_enter(void *fn);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    for (;;) pause();                                                                                   /* unreachable: every delivery arm switches away and parks; destroy joins */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_genp_yield(uint64_t d0, uint64_t d1) { scrip_coret(d0, d1, (void *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    if (p->jmp_entry && p->is_generator) {   /* GENP slice-2: per-instance-stack generator.  Capture the caller's callee-saved regs FIRST (O0 keeps them untouched this early — the emitted caller's rbx/r12 GVA/ζ bases and any live scan triad ride into the instance thread via rt_genp_thread_entry); copy the staged args (caller blocks on the activate handshake, so the replay on the instance thread is race-free); first activation and every resume go through the ONE scrip_coexpr_activate window. */
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
    if (p->jmp_entry) {   /* NCB-1d: a DET jmp-entry callee reached through the value/generator window (proc values in `every (!plist)()`, computed calls) — one-shot: the recorded regime says the body self-allocates and fully unwinds, so entering it with `call fn(fb,0)` would jmp a garbage γ-wire (rung37_proc_lookup crash).  Route through the two-landing shim (args are already staged in g_call_args by the value marshaller; the open pushes the lex record); no resumable frame exists, so hout stays null and any β re-drive sees an exhausted generator. */
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
    { rt_genp_s *g = rt_genp_lookup(frame);                                                             /* GENP slice-2: live-instance list membership IS the flavor discriminator (a ZH handle or legacy fb pointer can never collide with a listed rt_genp_s address) */
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
/* rt_pcall_t + g_pcall + g_pcall_wires ERADICATED (Lon s55 in-chat: "Remove g_pcall* regardless of who uses them. We do not do that here.").  Linkage rides r10/r11 wires + the stack. */
extern int  rt_value_trail_mark(void);
extern void rt_value_trail_tidy_dead_below(int mark, void *upper);
extern void rt_value_trail_tidy_dead_window(int mark, void *lower, void *upper);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_lcl_proc_args_install(void *base_p, int nparams, int nlocals) {   /* ICN-PROC-FRAME (s211): install g_call_args into lexical-proc frame at ZLS vslot offsets. Param i lives at [___+(i+1)*16] (ZLS: slot 0 reserved for proc result, params at +16,+32,...). Locals at [___+(nparams+j+1)*16]. Both media call this C function. */
    char *base = (char *)base_p;
    int nargs = nparams;   /* GLOBALS-GONE s55: record eradicated; assume full formals until the stack-resident record lands (going-in only) */
    int na = (nargs < nparams) ? nargs : nparams;
    for (int i = 0; i < na; i++) *(DESCR_t *)(base + (i + 1) * 16) = g_call_args[i];
    for (int i = na; i < nparams; i++) { DESCR_t _n = NULVCL; *(DESCR_t *)(base + (i + 1) * 16) = _n; }
    for (int j = 0; j < nlocals; j++) { DESCR_t _n = NULVCL; *(DESCR_t *)(base + (nparams + j + 1) * 16) = _n; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_icn_zframe_args_install(void *base_p, int nparams, int nlocals) {   /* ICN-FR-2 / PL-FR-2: ζ-frame arg installer — reads g_call_args[0..nparams-1] DIRECTLY.  Slot layout: param i at [___+(i+1)*16] (positive, inside the frame [___+0..kt-1]); ZLS result slots start at 16+nparams*16 and above, so named params never collide.  Matches ir_drive_slot_assign vslot grants in scrip_ir.c. */
    char *base = (char *)base_p;
    for (int i = 0; i < nparams; i++) *(DESCR_t *)(base + (i + 1) * 16) = g_call_args[i];
    for (int j = 0; j < nlocals; j++) { DESCR_t _n = NULVCL; *(DESCR_t *)(base + (nparams + j + 1) * 16) = _n; }
}
/* rt_flat_wires_t / g_pcall_wires / g_flat_ret_snapbuf ERADICATED (Lon s55). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gc_ws_roots(void)
{
    extern void rt_gc_visit_descr(DESCR_t *); extern void rt_gc_visit_raw(const char **);
    for (int i = 0; i < g_name_save_top; i++) rt_gc_visit_descr(&g_name_save[i].old);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_pcall_grow ERADICATED (s55) */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RTX-4 test-battery accessors ERADICATED with the record (s55); rtx_call_test.c deleted. */
/* c_rt_flat_wire_adopt / rt_flat_wire_adopt_isle / c_rt_flat_ret_snap ERADICATED (s55): the wire pair rides r10/r11 set at the site; no bank, no snap. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PROLOGUE LEAF — everything from resolve_cells through the monitor call event.  Returns the 16-aligned frame
 * byte count the caller must make available at fb; pushes the call context.  Strict leaf: calls no BB. */
int rt_proc_call_prologue(rt_proc_t *p, DESCR_t *args, int nargs, int wn)
{
    rt_proc_resolve_cells(p);
    int np = p->nparams;
    { int nf = p->nformals > 0 ? p->nformals : np; if (nargs > nf) nargs = nf; }   /* NPSPLIT (s22w): excess actuals were already evaluated by the arg spine; binding stops at the formals per SPITBOL Ch.8 — the save_push below then null-inits every remaining name (locals included), which is the manual's locals discipline verbatim */
    const char **pn = p->pnames;
    const char *rname = p->result_name ? p->result_name : p->name;
    int fbytes = (int)(PROC_FRAME_NEST_QWORDS * 8);
    if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    int save_base = rt_name_save_push(pn, p->pcells, args, nargs, np);
    { int rn_shadow = 0;
      for (int k = 0; k < np; k++) if (pn && pn[k] && !strcmp(pn[k], rname)) { rn_shadow = 1; break; }
      if (!rn_shadow) rt_name_save_push(&rname, &p->rcell, (DESCR_t *)0, 0, 1); }
    fbytes = (int)(((long)fbytes + 15L) & ~15L);
    /* GLOBALS-GONE s55: record push ERADICATED — going-in keeps resolve/save/install/monitor/k_level */
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
/* rt_proc_epilogue_body ERADICATED with the record (s55). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* γ ENTRY — RETURN and NRETURN.  Manual Ch.8: RETURN yields a value for the caller.  NRETURN yields a NAME and
 * is a γ citizen too — lower_snobol4.c routes its SNO$NRET node to exitnd, the same γ as RETURN, the flag
 * riding in rt_g_ret_by_name.  There is no fifth port (RULES.md: FOUR PORTS = FOUR GREEK NAMES ALWAYS). */
DESCR_t c_rt_proc_call_epilogue_γ(DESCR_t frame0)
{
    rt_k_level--;   /* GLOBALS-GONE s55: record eradicated; restore side is future RBP-era work */
    return frame0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ω ENTRY — FRETURN.  Manual Ch.8 verbatim: "Transferring to the special label FRETURN returns from a function
 * signaling failure to the caller.  No value is returned as the function result."  Arriving here IS the failure
 * signal (s61 RULING 1); no frame value is read — s62 ruling (c): a failing lexical proc returns FAILDESCR. */
DESCR_t c_rt_proc_call_epilogue_ω(void)
{
    rt_k_level--;   /* GLOBALS-GONE s55: record eradicated; restore side is future RBP-era work */
    return FAILDESCR;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ NAMED γ/ω TWINS (s112) — THE NON-SLIM ARM'S MISSING RETURN CONTRACT.  MEASURED, not theorised: the classic `rt_proc_call_open` arm's landings called the bare γ/ω epilogues above, which do nothing but
 * `rt_k_level--` and hand back whatever junk rode rdi:rsi — so a DEFINE'd proc reached through that arm returned NULL and NEVER restored its save set.  Witness `probe/mon/mon_return_contract.sno`: oracle
 * `R1=5 A=OUTER-A L=OUTER-L`, non-slim arm `R1= A=4 L=104` — the result is lost AND the actual argument leaks out over the caller's variable.  BOTH halves are manual Ch.8 p.103-104 verbatim: "a function
 * may return a value by assigning it to a variable with the same name as the function.  If no assignment occurs, the result is the null string" and "when the function returns, the dummy arguments are
 * restored to their original values" (locals likewise, p.104).  REGIME-INDEPENDENT — `SCRIP_SCC_OFF=1` reproduces it with GVA fully ON, so this is NOT the GVA-off/monitor defect s111 named; MONITOR_BIN
 * merely forces sites onto this arm.  THE PASSING SIBLING IS THE SLIM ARM, whose γ landing does exactly three things inline (fetch the result cell into rdi:rsi, mirror-restore the save set, then call its
 * leaf) — these twins are that same contract for the arm whose save set lives on the runtime name-save stack instead of the caller's rsp block.  ONE AUTHORITY: the body is `rt_call_named_proc_sl`'s tail
 * (~:2018) verbatim in substance — `rcell ? *rcell : NV_GET_fn(rname)`, then `rt_name_restore`, then the monitor tap.  NO NEW GLOBAL: the restore base is DERIVED as `g_name_save_top - rt_proc_save_count(p)`
 * rather than banked, which is recursion-safe by the same LIFO discipline the RSP floater's depth-invariance law already assumes (each activation pops exactly what its own prologue pushed).  The dyn/lex
 * branch MIRRORS `rt_proc_call_open`'s own: only `rt_proc_call_prologue` pushes name saves, so only a dyn_scope proc may restore them — a lex proc would corrupt a stack it never wrote. */
static int rt_proc_save_count(rt_proc_t *p)
{
    if (!p || !p->dyn_scope) return 0;
    int np = p->nparams, n = 0; const char **pn = p->pnames; const char *rname = p->result_name ? p->result_name : p->name; int rn_shadow = 0;
    for (int k = 0; k < np; k++) if (pn && pn[k]) n++;
    for (int k = 0; k < np; k++) if (pn && pn[k] && rname && !strcmp(pn[k], rname)) { rn_shadow = 1; break; }
    if (!rn_shadow && rname) n++;
    return n;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_proc_call_epilogue_named_γ(const char *name) { return rt_proc_epilogue_named(name, 0); }
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_proc_call_epilogue_named_ω(const char *name) { return rt_proc_epilogue_named(name, 1); }
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BP-7 SCC SLIM LEAVES — the static-save-set call convention (GOAL-SNOBOL4-BB BP-7).  The emitted static arm performs the save-set old-value saves (GVA cell → caller rsp block) and the arg installs
 * (frame slot → GVA cell) INLINE; these leaves carry only the per-call residue the loops never were: Σ save/restore, the pcall context, NRETURN wn, the monitor events, k_level, and the vtmark tidy.
 * open_slim runs AFTER the inline saves and BEFORE the installs, so its null-padding of unpassed formals/locals and the result cell lands exactly where rt_name_save_push's install phase did; every
 * guard is checked BEFORE any side effect so a 0 return falls back to the classic arm with nothing to undo (the inline saves it leaves behind are pure reads, discarded by the fallback's rsp restore). */
void rt_c2b_arm_trap(void) { fprintf(stderr, "FATAL: CALL2BB 3b — slim open declined at RUNTIME on an fc-armed call site; the flat fallback does not exist as storage on an armed statement (registration excluded OPSYN/redefinition shapes at emit time, so this decline names a guard the planner does not mirror — widen fc_call_ok or the probe)\n"); fflush(stderr); abort(); }
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_proc_call_open_slim(const char *name, int np, int nargs)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (!p || !p->fn || !p->dyn_scope || p->is_generator || p->is_variadic || p->redefined || g_call_fastpath_off || p->nparams != np || nargs > (p->nformals > 0 ? p->nformals : np)) return 0;   /* NPSPLIT (s22w): the nargs admission is against FORMALS; the np equality stays the emit/runtime full-set consistency check */
    rt_proc_resolve_cells(p);
    const char *rname = p->result_name ? p->result_name : p->name;
    int wn = rt_g_want_name; rt_g_want_name = 0;
    for (int k = nargs; k < np; k++) { if (p->pcells && p->pcells[k]) *p->pcells[k] = NULVCL; else if (p->pnames && p->pnames[k]) NV_SET_fn(p->pnames[k], NULVCL); }
    { int sh = 0; for (int k = 0; k < np; k++) if (p->pnames && p->pnames[k] && !strcmp(p->pnames[k], rname)) { sh = 1; break; }
      if (!sh) { if (p->rcell) *p->rcell = NULVCL; else NV_SET_fn(rname, NULVCL); } }
    /* GLOBALS-GONE s55: record push ERADICATED — going-in keeps resolve/save/install/monitor/k_level */
    if (g_monitor_bin) mon_emit_call_bin(p->name);
    rt_k_level++;
    return (long)(uintptr_t)(void *)p->fn;   /* GLOBALS-GONE s55: record eradicated */ /* rax channel: nonzero == admitted AND the transfer target — c_rt_proc_open_fn crossing deleted from the site */
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_proc_call_epilogue_slim_γ(DESCR_t result)
{
    rt_k_level--;   /* GLOBALS-GONE s55: record eradicated; restore side is future RBP-era work */
    return result;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_proc_call_epilogue_slim_ω(void)
{
    rt_k_level--;   /* GLOBALS-GONE s55: record eradicated; restore side is future RBP-era work */
    return FAILDESCR;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-DC DIRECT-CALL FAMILY (REGAIN-1 SLICE C, 2026-07-20 s108) — the emit-time-resolved DIRECT det call: the site `call`s the callee's per-proc dc stub (m4: named `call proc_X_dcα`; m3: `call [r11]`
 * through the fixed dc-fn slot below), the stub self-builds the SAME frame layout the wire prologue makes (retaddr parked in the free kt-32 header pad, γ/ω wires pointed at two stub-local ret-shims),
 * and ONE slim prep crossing replaces {open_detN + prologue_lex + lexprep2}: it binds the arg CELL POINTERS the stub stashed in the param slots, NULVCL-pads to nparams, seeds slot0 + the suffix zone,
 * and pushes the RSP-F-2 vtmark onto the dc micro-stack.  The γ/ω ret-shims tail-jmp the leave leaves, which pop the mark, run the SAME death-tidy the wire epilogues run, and `ret` to the site with
 * the DESCR in rax:rdx (v==99 = FAIL, the site's existing join).  NO pcall record rides this path — the lex epilogue body is `failed ? FAILDESCR : frame0` (wn=0 identity), Σ untouched, so the record
 * bought nothing but the vtmark, which the micro-stack now carries.  Eligibility is emit-time-static (registered !dyn !gen nparams==nargs<=4, hatch SCRIP_NO_DC); every ineligible call keeps the wire
 * verbatim.  Fixed-address slot array (NOT a g_rt_gen_procs field): the registry reallocs on growth and the m3 site bakes the slot address at emit time, so the slot must never move.  Longjmp unwinds
 * need no bookkeeping: the FRAME-CELL vtmark dies with its frame. */
static int rt_proc_call_prologue_lex(rt_proc_t *p, int nargs, int wn);
#define RT_DC_FNS_MAX 8192
static void *g_rt_dc_fns_store[RT_DC_FNS_MAX];
void **rt_pl_dc_slot(long idx) { return (idx >= 0 && idx < RT_DC_FNS_MAX) ? &g_rt_dc_fns_store[idx] : (void **)0; }
void rt_proc_set_dcfn(const char *name, void *fp) { int i = name ? rt_proc_hash_lookup(name) : -1; if (i >= 0 && i < RT_DC_FNS_MAX) g_rt_dc_fns_store[i] = fp; }
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-DC ELIGIBILITY — the ONE predicate both the site arm and the driver's stub arming read (table facts only; p->fn deliberately untested — it is unset at m4 compile time, and dc-eligible ⊆
 * registered ⊆ has-graph guarantees the stub exists).  Hatch: SCRIP_NO_DC=1 restores the wire path byte-for-byte on both sides. */
int rt_pl_dc_ok(const char *name, int nargs)
{
    static int off = -1; if (off < 0) { const char *e = getenv("SCRIP_NO_DC"); off = (e && *e == '1') ? 1 : 0; }
    if (off) return 0;
    if (name && strncmp(name, "LBL__", 5) == 0) return 0;   /* PL-DC vs LBL__ (s119): main-program pseudo-procs arm flat_lex=0 (emit.cpp emit_jmp_entry_for_proc: is_lbl) — they are entered ONLY through rt_chain_enter (rt_goto_transfer arm 4), never a direct dc call, so admitting them here drifts the site/callee predicate and trips the FATAL PL-DC guard on CODE label-transfer.  Exclude structurally, mirroring the flat_lex shape. */
    { int i = name ? rt_proc_hash_lookup(name) : -1;
      if (i < 0 || i >= RT_DC_FNS_MAX) return 0;
      { rt_proc_t *p = &g_rt_gen_procs[i];
        return (!p->dyn_scope && !p->is_generator && p->jmp_entry && !p->is_variadic && !p->redefined && (p->nformals > 0 ? p->nformals : p->nparams) == nargs && nargs >= 0 && nargs <= 4) ? 1 : 0; } }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    *(long *)((char *)fb + region_bytes + 32) = (long)rt_value_trail_mark();   /* PL-DC FRAME-CELL vtmark (FACT RULE "no §10 global" — the s108 gate red): the stub carved 16 bytes below fb; the mark lives THERE, dies with the frame (longjmp-immune for free), and the γ/ω shims hand it back to the leaves in registers */
    rt_k_level++;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dc_leave_γ(DESCR_t r, long vtmark, void *fb)
{
    rt_k_level--;
    rt_value_trail_tidy_dead_window((int)vtmark, fb, (char *)__builtin_frame_address(0) + 16);
    return r;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dc_leave_ω(long vtmark, void *fb)
{
    rt_k_level--;
    rt_value_trail_tidy_dead_window((int)vtmark, fb, (char *)__builtin_frame_address(0) + 16);
    return FAILDESCR;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_proc_pname(const char *name, int k) { rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0; return (p && p->pnames && k >= 0 && k < p->nparams) ? p->pnames[k] : (const char *)0; }
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_proc_result_name_get(const char *name) { rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0; return p ? (p->result_name ? p->result_name : p->name) : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RET-REGIME EPILOGUE — the call/ret close.  Under jmp-entry the two wires made the IS_FAIL discriminator
 * redundant and it was deleted (s61 finding); a call-regime return has ONE edge, so the discriminator is
 * STRUCTURAL there, not redundant.  LEXICAL procs (dyn_scope=0: args bound into a caller-made frame — the
 * Icon/Prolog/Raku frontends) stay call-regime wholesale this rung; NCB-1d converts the static-link family. */
/* GENP-SPINE (s92) &level DANCE — a spine-resident generator's pcall record pops at its FIRST delivery (the ONE-POP LAW in bcps_spine_gen_arm: open pushed one record, only the first landing may run an
 * epilogue leaf, so abandonment stays free — no dangling record for an enclosing unwind to mis-pop).  But &level is the LIVE-CHAIN depth: a resumed generator runs one deeper than its resumer, so the
 * counter must be re-raised for the span of each resumed run.  The caller's β edge calls _resume_enter before jmp [rsp]; the resumed-delivery landings call the pass leaves (γ marshals the rdi:rsi
 * result through to rax:rdx, ω synthesizes FAILDESCR) which take the counter back down.  Nested resumed generators compose: each hop is ±1 around its own resume span.  Strict leaves in the s22 sense —
 * no frame, no transfer, one counter and a marshal. */
/* ICN-FR-4 NOTE: the resumed-delivery DESCR_t correctness is now ensured by xa_flat_zframe_epilogue_γ
 * loading rdi:rsi from [___+0]:[___+8] (= FRQ(0/8) = bb_suspend's stored yield value) rather than from
 * rax:rdx (where rax = last FRQ(op_sa+8) load = i-field only).  This pass_γ function is a strict
 * pass-through in BOTH C and RTX forms — the fix lives in the epilogue that feeds rdi:rsi. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_gen_get_fb(void) { return (void *)0; }   /* GLOBALS-GONE s55: pcall record eradicated; Icon FR-4 resume OWED a stack-resident carrier */   /* FR-4 ZFRAME GENERATOR RESUME: return generator frame base from top pcall record; template does jmp [rax+cont_off] to reach the stored continuation label in the generator's own frame. Strict leaf. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ICN-FR-5 ONE-SLOT FIX: persistent generator state stack, keyed by generator____ (= frame base pinned in α).
 * FR-4 used four process globals — correct for one pending generator but silently clobbered by a second.
 * The pcall record was the wrong anchor: it is pushed at call-open and POPPED at γ-exit, BEFORE the caller's
 * β-resume reads the continuation.  Any per-pcall-slot store is therefore already gone by read time.
 *
 * CORRECT ANCHOR: a separate stack of icn_gen_state_t entries, independent of g_pcall:
 *   - PUSH at save_wires (prologue, after lexprep2 populates fb): gen____ known, wires known.
 *   - UPDATE cont at save_cont (each suspend α): same gen____, new continuation.
 *   - UPDATE caller____ at save_caller____ (prologue, after lexprep2): same gen____.
 *   - READ at get_* (γ/ω epilogue and β-resume): scan from top for matching gen____.
 *   - POP at save_wires with gw=NULL sentinel (called from ω epilogue when generator is exhausted).
 *     OR: never explicitly pop — the stack is bounded by max nesting depth (~64); stale entries below
 *     live gen____ addresses are never found by the LIFO scan while those generators are exhausted.
 *     But we MUST pop at ω to prevent the stack growing unboundedly on repeated generator calls.
 *
 * POP PROTOCOL: xa_flat_zframe_epilogue_ω calls save_wires(gen____, NULL, NULL) as an explicit pop marker.
 * The save_wires body detects gw=NULL and pops/clears the matching entry instead of writing.
 *
 * Stack capacity: initial 64 entries (covers all practical nesting depths); grows by doubling. */
typedef struct { void *gen_fb; void *cont; void *caller_fb; void *gwire; void *owire; } icn_gen_state_t;
__attribute__((visibility("hidden"))) static icn_gen_state_t  g_icn_gen_stk_buf[64];
__attribute__((visibility("hidden"))) static icn_gen_state_t *g_icn_gen_stk     = g_icn_gen_stk_buf;
__attribute__((visibility("hidden"))) static int              g_icn_gen_stk_top = 0;
__attribute__((visibility("hidden"))) static int              g_icn_gen_stk_cap = 64;
/* Fast single-generator globals: still maintained so single-generator case hits no scan overhead. */
__attribute__((visibility("hidden"))) static void *g_gen_pending_cont        = (void *)0;
__attribute__((visibility("hidden"))) static void *g_gen_pending_caller____  = (void *)0;
__attribute__((visibility("hidden"))) static void *g_gen_pending_gamma_wire  = (void *)0;
__attribute__((visibility("hidden"))) static void *g_gen_pending_omega_wire  = (void *)0;
/* Find entry by gen____; LIFO scan; returns pointer or NULL. */
static icn_gen_state_t *icn_gen_find(void *gen_fb) {
    for (int i = g_icn_gen_stk_top - 1; i >= 0; i--)
        if (g_icn_gen_stk[i].gen_fb == gen_fb) return &g_icn_gen_stk[i];
    return (icn_gen_state_t *)0;
}
/* Grow the stack if needed. */
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
void rt_gen_save_wires(void *gen_fb, void *gw, void *ww) {
    if (!gw) {   /* ω-exit POP SENTINEL: remove this generator's entry. */
        for (int i = g_icn_gen_stk_top - 1; i >= 0; i--) {
            if (g_icn_gen_stk[i].gen_fb == gen_fb) {
                /* Shift entries above down by one. */
                for (int j = i; j < g_icn_gen_stk_top - 1; j++) g_icn_gen_stk[j] = g_icn_gen_stk[j+1];
                g_icn_gen_stk_top--;
                break;
            }
        }
        return;
    }
    g_gen_pending_gamma_wire = gw; g_gen_pending_omega_wire = ww;
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    if (e) { e->gwire = gw; e->owire = ww; return; }   /* update existing (re-activation via β) */
    icn_gen_stk_grow();
    if (g_icn_gen_stk_top >= g_icn_gen_stk_cap) return;   /* grow failed; global cache is fallback */
    g_icn_gen_stk[g_icn_gen_stk_top++] = (icn_gen_state_t){ gen_fb, (void*)0, (void*)0, gw, ww };
}
void rt_gen_save_cont(void *gen_fb, void *cont) {
    g_gen_pending_cont = cont;
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    if (e) e->cont = cont;
}
void *rt_gen_get_cont(void *gen_fb) {
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    void *v = e ? e->cont : g_gen_pending_cont;
    g_gen_pending_cont = v; return v;
}
void *rt_gen_get_gamma_wire(void *gen_fb) {
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    void *v = e ? e->gwire : g_gen_pending_gamma_wire;
    g_gen_pending_gamma_wire = v; return v;
}
void *rt_gen_get_omega_wire(void *gen_fb) {
    icn_gen_state_t *e = icn_gen_find(gen_fb);
    void *v = e ? e->owire : g_gen_pending_omega_wire;
    g_gen_pending_omega_wire = v; return v;
}/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_call_epilogue_ret(DESCR_t fret)
{
    if (IS_FAIL_fn(fret)) return rt_proc_call_epilogue_ω();
    /* The call-regime lexical protocol returns only a STATUS in rax:rdx; the RESULT lives at [fb+0]
     * (prologue_lex: "result read back from [fb+0]"; the s61 finding: the lex arm discards fret).
     * Lift it here so the γ entry's frame0 carries the same datum the dyn landing reads from slot 0. */
    DESCR_t frame0 = fret;
    /* GLOBALS-GONE s55: lex [fb+0] lift rode the record; passthrough until the stack-resident record lands */
    return rt_proc_call_epilogue_γ(frame0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PROC-CONV (R12-FREE rung 2) — THE TWO-LANDING TRANSFER SHIM.  rt_chain_enter's shape with the one change the
 * s61 finding mandates: γ and ω get SEPARATE landings, each statically knowing its port, so the FAILDESCR
 * sentinel round-trip (the proc-level LAST_OK) is deleted rather than re-derived.  ⚠ EXACTLY FIVE PUSHES —
 * SysV 16-byte alignment is load-bearing through the jmp (rsp 8 mod 16 at entry, +40B = 0 mod 16; the blob's
 * 16-mult `sub rsp,K_total` carries it in — the measured s60 correction).  γ arrives with the activation
 * RETAINED at the deep frontier ([rsp+0]=β-landing, [rsp+8]=region base — the one-shot resume record); the
 * landing lifts frame0 (the lex result slot, [base+0..15]) into rdi:rsi BEFORE the wholesale `mov rsp,r12`
 * reclaim, because the epilogue's own C frame would otherwise be carved straight through the retained region.
 * ω arrives with the activation already ABSOLUTELY unwound by the blob's ω-half (rsp = the r12 anchor exactly).
 * The tail-jmp hands rax:rdx (DESCR_t) straight back to the C caller. */
#if defined(ZC_FRAME) && defined(ZC_FRAME_RSP) && ZC_FRAME == ZC_FRAME_RSP
/* R12-ERAD s65: under rsp-frames the blob's LIFO exits fully unwind BEFORE the jmp (γ delivers frame0 in rdi:rsi pre-unwind), so both landings arrive at the pre-jmp rsp — the r12 anchor is deleted. */
/* ⭐ EXPR-THUNK RESULT (class B, s96, second half): the γ landing tail-calls c_rt_proc_call_epilogue_γ(DESCR_t frame0), which RETURNS frame0 = the DESCR it received in rdi:rsi -- but every wire-exiting body (bb_glue_wire_γ = `jmp r10`, the RETURN floater "rax:rdx riding untouched", the EXPR$ thunk's n_assign box) delivers its value in rax:rdx.  MEASURED: `*(X 'b')` / `*GT(N,3)` / `*IDENT(X,'a')` reached this landing with the right value in rax:rdx and the C caller received whatever rdi:rsi held (the last operand load before the body's final C call) -- benign-looking null garbage for `*DIFFER(X)` shapes (their MATCH was accidental), a rejected descriptor for the rest.  The landing now forwards rax:rdx into rdi:rsi; the non-RSP #else arm keeps its own frame-slot load (different regime). */
/* ⭐ EXPR-THUNK EXITS (GOAL-SNOBOL4-100 bb_probes class B, s96): the wires ride r10/r11 (Lon s55: "R10 and R11 for success and fail return address ... just like any BB BLOB interface"; role-3 WIRE-ADOPT is EMPTY since s55) and every emitted body exits bb_glue_wire_γ/ω = `jmp r10`/`jmp r11` -- yet this C-side opener still delivered its landings ONLY in rcx/rdx and the RTCC inbound load then seated the CALLER's written-back wires (or 0) in r10/r11 last.  MEASURED (`S BREAK(',') *DIFFER(X)`, every *EXPR pattern element via c_rt_defer_get_pat_fn -> rt_call_proc_descr -> here): the EXPR$ thunk unwound cleanly to this frame and jumped through wire 0.  Both arms now seat r10=2f/r11=3f AFTER the load; rcx/rdx kept (chain contract readers).  A fresh C-entered activation OWNS its ports; the caller's wires survive in rtccb and reload at its own crossing return. */
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
/* RTCC RC-5-GVA INBOUND LOAD (RC-0(d) edge class 2): load VM globals into caller-saved regs before jmp.    */
/* rax=fn-ptr rcx=γ-wire rdx=ω-wire are load-bearing; rsi/rdi/r8/r9/r10/r11 are free.                      */
/* Load order: r11=block ptr; rsi/rdi/r8/r9; r10 overwritten last (GOT scratch → r10 slot value).           */
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
"  leaq 2f(%rip), %r10\n"
"  leaq 3f(%rip), %r11\n"
"  jmp *%rax\n"
"2:\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  movq %rax, %rdi\n"
"  movq %rdx, %rsi\n"
"  jmp rt_proc_call_epilogue_γ\n"
"3:\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  jmp rt_proc_call_epilogue_ω\n"
);
#else
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
"  movq %rsp, %r12\n"
/* RTCC RC-5-GVA INBOUND LOAD (RC-0(d) edge class 2, non-RSP-frame variant): same GOT-indirect load as     */
/* rt_chain_enter; rax=fn-ptr rcx=γ-wire rdx=ω-wire r12=rsp-anchor are load-bearing; rsi/rdi/r8/r9/r11 free. */
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
"  leaq 2f(%rip), %r10\n"
"  leaq 3f(%rip), %r11\n"
"  jmp *%rax\n"
"2:\n"
"  movq 8(%rsp), %rax\n"
"  movq 0(%rax), %rdi\n"
"  movq 8(%rax), %rsi\n"
"  movq %r12, %rsp\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  jmp rt_proc_call_epilogue_γ\n"
"3:\n"
"  movq %r12, %rsp\n"
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
/* OPEN-FN LEAF — the emitted call site's fn fetch.  Under the call regime the entry rode back out of
 * rt_frame_prep; under jmp-entry there is no caller-made frame to prep, so the site asks for the entry alone.
 * Strict leaf: reads the pcall record the open just pushed. */
void *c_rt_proc_open_fn(void)
{
    return (void *)0;   /* GLOBALS-GONE s55: record eradicated */ /* fn now rides the OPEN return itself (rax channel) */
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
    /* ⭐ ICN-VARARG: THE FLAT INSTALLERS NEVER LEARNED THE VARIADIC TAIL RULE THEIR SIBLING ALREADY IMPLEMENTS.  A trailing `[]` parameter must receive a LIST of the surplus arguments (canonical: refs/icon-master/src/runtime/invoke.r:105-133 — nparam<0 marks variable arity, args beyond abs(nparam)-1 are collected by Ollist into the last argument, short calls null-pad first).  rt_frame_bind_args (:761) does exactly that, but the callee-carves-its-own-frame path never reaches it: the prologue calls rt_icn_zframe_args_install / rt_lcl_proc_args_install, both of which are flat `slot[i] = g_call_args[i]` copies with no variadic arm, so `procedure note(a[])` bound a RAW argument — measured `note("p0")` -> a = the STRING "p0" (so `!a` generated the characters 'p','0' instead of one element), `note("x","y")` -> a = "x" with "y" dropped, `note()` -> a = &null instead of an empty list.  THIS is the one site that holds BOTH the proc record and the true per-call-site nargs (every rt_proc_call_open_det* leaf funnels here), which is why the collapse belongs here and not in the installers — they receive neither, and the s55 pcall eradication left them assuming nargs==nparams.  Normalising g_call_args to exactly nparams entries HERE makes the downstream flat copy correct without touching the emitter, so BOTH MEDIA are fixed by construction and no signature, ABI or global changes. */
    { static int _va = -1; if (_va < 0) { const char *_e = getenv("SCRIP_VARARG_TAIL"); _va = (_e && *_e == '0') ? 0 : 1; }
      if (_va && p->is_variadic && p->jmp_entry) {   /* ⛔ jmp_entry IS THE DISCRIMINATOR AND IT IS LOAD-BEARING: rt_proc_call_gen_h calls rt_proc_call_open (which lands here) AND THEN rt_frame_bind_args on the same call, so an ungated collapse would run twice and nest the tail list inside itself.  jmp_entry=1 is precisely the recorded fact "the emitted body is a jmp-entry blob" (rt.c:431) = the flat-installer path; the call-regime bodies that reach rt_frame_bind_args are generators, which carry jmp_entry=0.  Behavioural fact, never a language name (NO-LANGUAGE-IDENTITY). */
        extern DESCR_t rt_make_list(DESCR_t *args, int nargs); extern DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs); extern DESCR_t rt_make_nested_agg(DESCR_t *args, int nargs);
        int npc = p->nparams; if (npc > CALL_ARGS_MAX) npc = CALL_ARGS_MAX;
        if (npc > 0) { int fixed = npc - 1; int rest = nargs - fixed; if (rest < 0) rest = 0;
            for (int i = nargs; i < fixed; i++) g_call_args[i] = NULVCL;   /* short call: null-pad the fixed prefix before the tail is built, mirroring invoke.r's D_Null fill */
            DESCR_t _tail = (p->rest_kind == 2) ? rt_make_nested_agg(rest > 0 ? &g_call_args[fixed] : (DESCR_t *)0, rest) : p->rest_kind ? rt_make_flat_agg(rest > 0 ? &g_call_args[fixed] : (DESCR_t *)0, rest) : rt_make_list(rest > 0 ? &g_call_args[fixed] : (DESCR_t *)0, rest);   /* SAME three constructors, selected by the SAME rest_kind, as rt_frame_bind_args — one spelling of the tail rule, so Raku slurpy/flat-agg semantics ride along unchanged */
            g_call_args[fixed] = _tail; } } }
    /* GLOBALS-GONE s55: record push ERADICATED — going-in keeps resolve/save/install/monitor/k_level */
    rt_k_level++;
    return fbytes;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* C-SIDE LEXICAL CALL WINDOW — the call-regime completion for lexical procs reached through the C transfer
 * fns (deferred callbacks, APPLY, FUNC_ENTRY hooks, first-class proc values).  Stages the caller's arg array
 * into g_call_args (rt_frame_bind_args reads there), runs the lex prologue, makes the frame in C, transfers
 * by call, closes through the ret-regime epilogue.  Mirrors the emitted lexical window in bcps_det_arm. */
static DESCR_t rt_proc_call_c_lex(rt_proc_t *p, DESCR_t *args, int nargs, int wn)
{
    if (nargs > CALL_ARGS_MAX) nargs = CALL_ARGS_MAX;
    for (int i = 0; i < nargs; i++) g_call_args[i] = args ? args[i] : NULVCL;
#if defined(ZC_FRAME) && defined(ZC_FRAME_RSP) && ZC_FRAME == ZC_FRAME_RSP
    if (p->jmp_entry) {   /* NCB-1d: the RECORDED regime — this body was emitted as a jmp-entry blob (driver-loop procs, !is_generator); the alloca+call window would run its sub/wire prologue with garbage rcx/rdx wires, so transfer through the two-landing shim (the blob lexpreps its own frame from the staged args).  Blocks/rules/methods registered outside the loops keep jmp_entry=0 and take the window below. */
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
/* FN-RET OPEN (s104) — the s55 rax-channel contract for the transfer consumers (rt_dcap_pump COND flush, rt_cap_open IMM): admitted == nonzero == THE TRANSFER TARGET.  c_rt_proc_open_fn was eradicated
 * at s55 (returns 0) but these two consumers were never migrated: every computed-name (*VAR/NRETURN) capture transfer since then loaded fbytes, nulled it through open_fn, and jumped 0. */
void *rt_proc_call_open_fnret(const char *name, int nargs)
{
    rt_proc_t *p = name ? rt_proc_find(name) : (rt_proc_t *)0;
    if (!p || !p->fn) return (void *)0;
    return rt_proc_call_open(name, nargs) ? (void *)p->fn : (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DET OPEN LEAF (PL-REGAIN-1 slice A, 2026-07-19 s100) — the emit-time-resolved det call site's fused open: the caller lowered a LITERAL !dyn callee, resolved its dense registry index at emission
 * (rt_proc_index_of; registration order is identical in-process and in the mode-4 startup bake, so the index is stable in both media), and calls here with the index — no name, no hash, and no separate
 * rt_proc_open_fn crossing.  Runs the same lex prologue as rt_proc_call_open's lexical arm and returns the callee fn pointer (0 = no body / guard mismatch, landing in the site's existing FAIL arm);
 * guards precede every side effect, so a decline is side-effect-free exactly like the SCC arm's discipline. */
void *c_rt_proc_call_open_det(long idx, int nargs)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return (void *)0;
    { rt_proc_t *p = &g_rt_gen_procs[idx];
      if (!p->fn || p->dyn_scope) return (void *)0;
      { int wn = rt_g_want_name; rt_g_want_name = 0;
        (void)rt_proc_call_prologue_lex(p, nargs, wn);
        return (void *)p->fn; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FUSED DET OPEN FAMILY (PL-REGAIN-4, 2026-07-19) — staging folded INTO the det open: the site passes CALLER-FRAME CELL POINTERS (lea [___+slot]) and the leaf copies them into g_call_args, then runs
 * the same guarded lex prologue as rt_proc_call_open_det.  ONE crossing replaces {rt_arg_stage × nargs + open_det}; g_call_args stays the arg MEDIUM (the REGAIN-1 slice-B residency decision — slab
 * rehome vs register ABI — is untouched: only the number of crossings that feed it changes).  Guards precede every copy so a decline (bad idx / no body / dyn) is side-effect-free per the SCC arm's
 * discipline; nargs > 4 keeps the classic stage chain at the site (no hot-path pred exceeds 4 today — tak/4 is the ceiling).
 * ICN-ARG-NULL (s239): each leaf ALSO seeds g_call_args[N .. p->nparams) with NULVCL before the prologue.  The classic path this family replaced performs that fill in rt_frame_bind_args (`for i in [nargs,npc): NULVCL`, :770); folding
 * the staging into the open dropped it, so an UNDER-SUPPLIED call left the missing parameter holding the PREVIOUS activation's argument — g_call_args is a process-lifetime medium and nothing else rewrites the tail.  The Icon
 * ζ-frame installer reads g_call_args[0..np-1] verbatim (rt_icn_zframe_args_install, "no pcall-nargs clamp"), so the stale word reached the callee's parameter slot: `try(p,a)` against `procedure try(p,a,b)` saw b = the prior
 * call's b, making `\b` succeed and `atan(x)` silently evaluate as atan(x,10.0).  Keyed on the callee record's arity, never on a language — the fill is the same contract rt_frame_bind_args already states for every frontend. */
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
/* LEXPREP LEAF (NCB-1d) — the DET-LEXICAL jmp-entry prologue's tail call: the blob has just self-allocated and
 * zero-filled its region; for a lexical proc that region IS the frame, so overwrite with the lexical init
 * (NULVCL slots) and bind the staged args at [fb + 16*(i+1)] — exactly rt_frame_prep's lex arm minus the fn
 * fetch.  Strict leaf, protocol-agnostic at the call site: dyn/PAT$/EVAL/LBL__ graphs never emit the call
 * (flat_lex gates it), and a non-lex top record no-ops defensively. */
void rt_jmp_frame_lexprep(void *fb, long region_bytes)
{
    (void)fb; (void)region_bytes;   /* GLOBALS-GONE s55: record eradicated */ /* Icon/PL lexical prep OWED a stack-resident record */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* LAZY LEXPREP LEAF (PL-REGAIN-4, 2026-07-19) — the one-call frame-init tail replacing {rep stosb zero-fill + rt_jmp_frame_lexprep's full NULVCL sweep} for DET-LEXICAL/GEN jmp-entry graphs.  zls_build's
 * layout is [0,16) slot0 | [16,16+np*16) params | box grants | [suffix_off, region) = resume slot (if any) + zeta-mark + named locals.  Only slot0 and the suffix need the NULVCL seed before the body:
 * params are covered by rt_frame_bind_args (staged args, NULVCL tail), and every box-grant slot is written by its producer before any consumer reads it (the four-port wiring).  Under SCRIP_ZLS_POISON=1
 * the box-grant span is filled 0xA5 instead of being left as stack garbage, so a planted use-before-init diverges loudly; params inside the poisoned span are overwritten by the bind that follows. */
/* PL-FR-4 ZFRAME PENDING RESUME — forward declarations; definitions below after g_pl_retry block. */
extern void   *g_pl_zf_pending_cursor;
extern long    g_pl_zf_pending_tm_lo;
extern long    g_pl_zf_pending_tm_hi;
extern int     g_pl_zf_pending_tm_off;
extern int     g_pl_zf_pending_cursor_off;
extern int     g_pl_zf_target_pcall_top;   /* PL-FR-4 BUG-FIX s14: pcall_top snapshot at resume_set time */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_jmp_frame_lexprep2(void *fb, long suffix_off, long region_bytes)
{
    (void)fb; (void)suffix_off; (void)region_bytes;   /* GLOBALS-GONE s55: record eradicated */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-FR-4 RETRY CONTINUATION STACK — the WAM B register for the ζ-frame regime.  THE DEFECT: MOVE_LABEL/DISJUNCTION rendezvous via [___+op_off+16]; the ζ epilogue restores ___ to the caller before
 * backtrack reads it — measured as rip=0 on bt_minimal.pl (the slot also aliases param-0).  CANONICAL SHAPE read this session from gprolog (ALTB on the B stack, EnginePl/wam_inst.h:92-107, CHOICE_STATIC_SIZE
 * 8, entirely separate from the E environment stack) and SWI-Prolog (struct choice { Choice parent; LocalFrame frame; union { Code pc; } value; }, pl-incl.h:1825-1838 — frame is a reference, never storage).
 * In both engines, choice-point lifetime is INDEPENDENT of activation-frame lifetime.  LIFO is sound: backtracking is stack-disciplined by the language definition; cut discards a contiguous top segment.
 * Killswitch: the emitter arms are gated on zframe_graph, so unflagged graphs never call these. */
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
/* PEEK-AND-POP: the retry address is consumed by the jump.  Returns 0 on empty — an exhausted choice point IS failure. */
void *rt_pl_retry_pop(void)
{
    if (g_pl_retry_top <= 0) return (void *)0;
    return g_pl_retry[--g_pl_retry_top];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-FR-4 ZFRAME RETRY CONTINUATION STACK — the β-resume address store for the ζ-frame regime.  Separate from g_pl_retry (cells track, PL-ZK-4) so the two embodiments are independent.  bb_move_label's zframe arm pushes the lbl_t0 (β) resume address here instead of writing FRQ(op_off+16); bb_indirect_goto's zframe arm pops and jumps here instead of jmp FRQ(op_off+16).  LIFO is sound: backtrack is stack-disciplined (WAM B-stack law); cut discards the contiguous top segment.  Killswitch: both emitter arms gate on g_emit.zframe_graph, so unflagged (SN4/Icon/non-zframe Prolog) graphs never call these — byte-identical by construction. */
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
/* PEEK-AND-POP: consumed by the jmp.  Returns 0 on empty — exhausted choice point = failure, caller jumps to omega. */
void *rt_pl_cp_pop(void)
{
    if (g_pl_cp_top <= 0) return (void *)0;
    return g_pl_cp_stack[--g_pl_cp_top];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-FR-4 ZFRAME TRIPLE PUSH — stores {trail_mark_lo, trail_mark_hi, cont_addr} as three consecutive slots.  Called by bb_suspend's zframe arm at each yield so the β-resume path in bb_call_proc_staged can restore the trail mark into a fresh callee frame and jump to the right suspend-β continuation without depending on the (dead) original frame. */
/* PL-FR-4 ZFRAME TRIPLE STACK — separate from g_pl_cp_stack (single-word, used by IR_INDIRECT_GOTO).  Mixing would allow IR_INDIRECT_GOTO's single-word rt_pl_cp_pop to corrupt triple entries mid-way. */
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
/* PL-FR-4 ZFRAME TRIPLE POP — reverses rt_pl_cp_push3 on its own separate g_pl_zf3_stack (never touched by rt_pl_cp_pop/push used by IR_INDIRECT_GOTO). */
void *rt_pl_cp_pop3(long *tm_lo, long *tm_hi)
{
    if (g_pl_zf3_top < 3) { if (tm_lo) *tm_lo = 0; if (tm_hi) *tm_hi = 0; return (void *)0; }
    void *cont     = g_pl_zf3_stack[--g_pl_zf3_top];
    if (tm_hi) *tm_hi = (long)(uintptr_t)g_pl_zf3_stack[--g_pl_zf3_top]; else --g_pl_zf3_top;
    if (tm_lo) *tm_lo = (long)(uintptr_t)g_pl_zf3_stack[--g_pl_zf3_top]; else --g_pl_zf3_top;
    return cont;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-FR-4 ZFRAME PENDING RESUME — globals that bridge the frame-allocation boundary.  When bcps_spine_gen_arm's β arm pops a triple and is about to re-enter the callee's α via rt_proc_call_open_det,
 * it calls rt_pl_zf_resume_set() to register the cursor+trail state for this activation.  rt_jmp_frame_lexprep2 then detects the pending state and writes cursor+trail into the freshly allocated frame
 * BEFORE the α_body cursor initialization runs, using a post-init override.  Cleared by rt_pl_zf_resume_clear after the override writes.  NOT concurrency-safe (single-threaded JIT assumption). */
__attribute__((visibility("default"))) void   *g_pl_zf_pending_cursor;   /* n15_suspend_β address to write into [fb+cursor_off] */
__attribute__((visibility("default"))) long    g_pl_zf_pending_tm_lo;    /* trail mark lo to write into [fb+tm_off] */
__attribute__((visibility("default"))) long    g_pl_zf_pending_tm_hi;    /* trail mark hi to write into [fb+tm_off+8] */
__attribute__((visibility("default"))) int     g_pl_zf_pending_tm_off;   /* frame slot offset for trail mark (pl_zf_trail_mark_off from IR_graph_t) */
__attribute__((visibility("default"))) int     g_pl_zf_pending_cursor_off;   /* frame slot offset for cursor (resume_slot from IR_graph_t; baked by bcps_spine_gen_arm from zls_g_resume_by_name) */
__attribute__((visibility("default"))) int     g_pl_zf_target_pcall_top;   /* PL-FR-4 BUG-FIX (s14): g_pcall_top snapshot at rt_pl_zf_resume_set time (BEFORE open_det increments it).  bb_suspend intercept fires only when g_pcall_top == this+1 — meaning THIS predicate is the direct β-resume target.  Inner predicates called from target's α_body have higher pcall_top and skip the intercept, maintaining triple-stack balance. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_zf_resume_set(void *cursor, long tm_lo, long tm_hi, int tm_off, int cursor_off)
{
    g_pl_zf_pending_cursor = cursor;
    g_pl_zf_pending_tm_lo = tm_lo;
    g_pl_zf_pending_tm_hi = tm_hi;
    g_pl_zf_pending_tm_off = tm_off;
    g_pl_zf_pending_cursor_off = cursor_off;
    g_pl_zf_target_pcall_top = 0;   /* GLOBALS-GONE s55: record eradicated */ /* PL-FR-4 intercept predicate OWED a stack-resident carrier */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_zf_resume_clear(void) { g_pl_zf_pending_cursor = (void *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FRAME-PREP LEAF — the site has just made fbytes of frame available at fb (an rsp bump, once the call site is
 * emitted; an alloca while it is still C).  Fill it per the open protocol, record it, and hand back the entry
 * to transfer to.  Returning fn is what lets the site do a single medium-symmetric `call rax` — no proc-symbol
 * or table-index encoding is needed in either medium. */
void *rt_frame_prep(void *fb, long fbytes)
{
    (void)fb; (void)fbytes;
    return (void *)0;   /* GLOBALS-GONE s55: record eradicated */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FAIL LEAF (NCB-1b) — the emitted call site's no-body arm.  rt_proc_call_open returns 0 when the proc has no
 * body; the C trampolines answered that with `return FAILDESCR`, but an emitted site cannot materialise a
 * struct-by-value constant, so it calls this instead and lands the same rax:rdx pair.  Strict leaf. */
DESCR_t c_rt_faildescr(void) { return FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs)
{
    if (!name) return FAILDESCR;
    int _wn = rt_g_want_name; rt_g_want_name = 0;
    rt_proc_t *p = rt_proc_find(name);
    if (!p || !p->fn) return FAILDESCR;
    if (!p->dyn_scope) return rt_proc_call_c_lex(p, args, nargs, _wn);
    { const char *_ba = getenv("SCRIP_BYNAME_ALPHA"); void *afn = ((!_ba || _ba[0] != '0') && !strchr(name, '$')) ? rt_dyn_alpha_fn(name, (void *)0) : (void *)0; if (afn) { extern DESCR_t rt_tiny_record_enter(void *fn, long nargs); int _n = nargs < CALL_ARGS_MAX ? nargs : CALL_ARGS_MAX; if (_n < 0) _n = 0; for (int i = 0; i < _n; i++) g_call_args[i] = args[i]; rt_g_want_name = _wn; return rt_tiny_record_enter(afn, (long)_n); } }   /* SPELLED-TWICE FIX (s117): p->fn from rt_define_site is the GENERIC ENTRY THUNK — the wrong protocol for an emitted body, so rt_proc_enter's wire jmp lands wild (rip=_rtld_global: the OPSYN `&` SIGSEGV, and every other by-name route to a DEFINE'd proc).  s104/s108 installed the sealed alpha$<FN> target in the sibling rt_call_proc_descr ONLY; this is that same arm, verbatim — the sealed target speaks the record contract and owns its own bookkeeping, so it is entered BEFORE (never after) any C open/prologue.  The C-array args this entry point carries are staged into the g_call_args channel the sealed target reads, which the emitted callers of the sibling fill themselves.  Falls through to the pre-s117 path untouched when no alpha$ cell exists or SCRIP_DYN_ALPHA=0. */
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
    return rt_proc_enter((void *)p->fn);   /* s117 SUSPECTED TWIN, NOT LANDED: this is the third by-name path and carries the same generic-entry-thunk hazard rt_call_named_proc does (see its SCRIP_BYNAME_ALPHA arm), but this session minted no witness that reaches it, so it is left verbatim rather than changed blind. */
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
    (void)nargs; return (p && p->dyn_scope && !p->is_generator && !p->is_variadic && !p->redefined) ? 1 : 0;   /* s58: arity clauses DROPPED — the real protocol fills missing formals with null and discards extras (manual Ch.8); the count rides the stack */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ICN-WIRE-PAIR (s244) — DOES THIS CALLEE CONSUME THE {gamma,omega} LANDING PAIR AT TOS?  Two return protocols share the flat rcx/rdx-wire call arms.  (1) The fnrbp2 RETURN/FRETURN floater protocol
 * (bb_define role-1/2) reads {gamma,omega} AT TOS and POPS 16 on the way out, which is why the s110/s111 sites push the pair at all.  (2) The plain jmp-entry protocol takes both wires in rcx/rdx, saves
 * them INSIDE its own frame at the prologue, and leaves by `add rsp,kt; jmp rcx` — it never reads TOS and never pops, so a pair pushed for it is never released and the CALLER resumes 16 bytes low.  The
 * caller then reads every rsp-relative slot at the wrong address; witness `every p(1|2|3)` against `procedure p(a,b)`, where the enclosing disjunction's arm counter aliased the argument descriptor and
 * the loop ran exactly once.  THE DISCRIMINATOR IS dyn_scope, NOT jmp_entry ALONE: a DEFINE-site callee is registered dyn_scope AND jmp_entry and still exits through the floater, so gating on jmp_entry
 * by itself suppresses a pair the floater goes on to pop and the callee returns into freed stack (measured: `1010_func_recursion` SEGVs, both media).  A callee that is jmp-entered and carries NO
 * dynamic-scope save/restore protocol is exactly the self-unwinding class.  Keyed on the callee record's own protocol bits, never on a language: both flags are set by whichever frontend uses them. */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_nofail_abort(void)
{
    extern void core_runtime_error(int code, const char *msg);
    core_runtime_error(35, "unexpected failure in -nofail mode");
    exit(1);   /* belt-and-braces: core_runtime_error is fatal for code 35, but rt_call_arr's setjmp may intercept the longjmp; direct exit ensures termination */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⛔⭐⭐⭐ GLOBALS-GONE s55 UNPREFIXED ALIASES — rtx_call.S / rtx_plcall.S deleted with the g_pcall record they ported (Lon in-chat: "Remove g_pcall* regardless of who uses them.  We do not do that
 * here.").  The unprefixed hot symbols they exported are still baked by templates and called by runtime C; each is now a thin jump to its record-free C body.  No RTX re-port is owed until the
 * stack-resident record exists (RBP-era coming-out work). */
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0) { return c_rt_proc_call_epilogue_γ(frame0); }
DESCR_t rt_proc_call_epilogue_ω(void) { return c_rt_proc_call_epilogue_ω(); }
DESCR_t rt_proc_call_epilogue_slim_γ(DESCR_t result) { return c_rt_proc_call_epilogue_slim_γ(result); }
DESCR_t rt_proc_call_epilogue_slim_ω(void) { return c_rt_proc_call_epilogue_slim_ω(); }
DESCR_t rt_proc_call_epilogue_named_γ(const char *name) { return c_rt_proc_call_epilogue_named_γ(name); }
DESCR_t rt_proc_call_epilogue_named_ω(const char *name) { return c_rt_proc_call_epilogue_named_ω(name); }
void *rt_proc_open_fn(void) { return c_rt_proc_open_fn(); }
void *rt_proc_call_open_det(long idx, int nargs) { return c_rt_proc_call_open_det(idx, nargs); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_register_rec(const rt_proc_reg_rec_t *r)
{   /* ONE-REG (Lon s119): replays EXACTLY the per-proc call sequence the pre-s119 m4 startup block emitted, in the same order, with the same skip conditions — behavior-identical by construction (every setter below is the same symbol the old block called via PLT).  _Static_asserts pin the record layout the scrip.c emitter spells in .quad/.long directives; a drift between the two is a compile error here, not a silent wrong-field read. */
    _Static_assert(sizeof(rt_proc_reg_rec_t) == 64, "ONE-REG record is 64 bytes");
    _Static_assert(__builtin_offsetof(rt_proc_reg_rec_t, pnames) == 32 && __builtin_offsetof(rt_proc_reg_rec_t, nparams) == 40 && __builtin_offsetof(rt_proc_reg_rec_t, flags) == 52, "ONE-REG field offsets are law");
    if (!r || !r->name) return;
    extern void rt_proc_set_dyn_scope(const char *, int); extern void rt_proc_set_result_name(const char *, const char *); extern void rt_proc_set_nparams(const char *, int); extern void rt_proc_set_nformals(const char *, int);
    extern void rt_proc_set_pname(const char *, int, const char *); extern void rt_proc_set_jmpentry(const char *, int); extern void rt_proc_set_dcfn(const char *, void *);
    if (r->flags & 1) { rt_proc_register(r->name, (const char **)r->pnames, r->nparams); rt_proc_set_dyn_scope(r->name, 1); if (r->result_name) rt_proc_set_result_name(r->name, r->result_name); }
    if (r->fn) rt_proc_set_fn(r->name, (bb_box_fn)r->fn);
    rt_proc_set_nparams(r->name, r->nparams);
    rt_proc_set_nformals(r->name, r->nformals);
    if (!(r->flags & 1) && r->pnames) for (int k = 0; k < r->nparams && r->pnames[k]; k++) rt_proc_set_pname(r->name, k, r->pnames[k]);   /* raku lex arm — array is exactly the ks the old block set, NULL-terminated */
    if (r->frame_bytes > 0) rt_proc_set_frame_bytes(r->name, r->frame_bytes);
    if (r->flags & 2) rt_proc_set_zstatic(r->name, 1);
    if (r->flags & 4) rt_proc_set_variadic(r->name, 1);
    if (r->rest_kind) rt_proc_set_rest_kind(r->name, r->rest_kind);
    if (r->named_rest) rt_proc_set_named_rest(r->name, r->named_rest);
    rt_proc_set_jmpentry(r->name, (r->flags >> 4) & 1);
    if (r->dcfn) rt_proc_set_dcfn(r->name, r->dcfn);
    if (r->flags & 8) rt_proc_set_generator(r->name, 1);
}
