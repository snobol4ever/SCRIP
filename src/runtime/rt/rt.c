#include "rt.h"
#include "core.h"
#include "descr.h"
#include "sil_macros.h"
#include "bb_pool.h"
#include "bb_box.h"
#include "../../processor/bb_pool.h"
#include "bb_build.h"
#include "../../ast/ast.h"
#include "../../include/SM.h"
#include "../../frontend/prolog/prolog_atom.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define STACKLESS_ABORT(fn) \
    do { fprintf(stderr, "libscrip_rt: %s called — Icon value stack removed (GROUND ZERO 3). " \
                         "This box must be rebuilt stackless (per-box slot, no value stack).\n", (fn)); \
         abort(); } while (0)
#include <string.h>
#include <math.h>
extern void    SNO_INIT_fn(void);
extern int     exec_stmt(const char *subj_name, DESCR_t *subj_var,
                         DESCR_t pat, DESCR_t *repl, int has_repl);
extern DESCR_t NV_GET_fn(const char *name);
extern DESCR_t NV_SET_fn(const char *name, DESCR_t val);
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
static int     g_vtop    = 0;
static int     g_vframe_base = 0;
static int     g_last_ok  = 1;
static void    _default_push     (const DESCR_t *d);
static void    _default_pop      (DESCR_t *out);
static void    _default_peek     (DESCR_t *out);
static int     _default_depth    (void)       { return g_vtop; }
static void    _default_set_depth(int n)      { g_vtop = n; }
static int     _default_get_last_ok(void)     { return g_last_ok; }
static void    _default_set_last_ok(int ok)   { g_last_ok = ok ? 1 : 0; }
static const rt_vstack_ops_t g_default_ops = {
    .push        = _default_push,
    .pop         = _default_pop,
    .peek        = _default_peek,
    .depth       = _default_depth,
    .set_depth   = _default_set_depth,
    .get_last_ok = _default_get_last_ok,
    .set_last_ok = _default_set_last_ok,
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const rt_vstack_ops_t *g_ops = &g_default_ops;
int rt_vstack_depth(void) { return g_ops->depth(); }
DESCR_t rt_vstack_pop(void) { DESCR_t out; g_ops->pop(&out); return out; }
static int     g_halt_rc  = 0;
static int     g_halt_set = 0;
static int     g_native_chunk_depth = 0;
int rt_in_native_chunk(void) { return g_native_chunk_depth > 0; }
static void _default_push(const DESCR_t *d)
{
    (void)d;
    fprintf(stderr, "[SMX] FATAL: SM value stack push after excision. There is no value "
                    "stack. This code path belongs to a language not yet on Byrd Boxes. "
                    "Aborting (by design).\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _default_pop(DESCR_t *out)
{
    (void)out;
    fprintf(stderr, "[SMX] FATAL: SM value stack pop after excision. There is no value "
                    "stack. Aborting (by design).\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _default_peek(DESCR_t *out)
{
    (void)out;
    fprintf(stderr, "[SMX] FATAL: SM value stack peek after excision. There is no value "
                    "stack. Aborting (by design).\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void vstack_push(DESCR_t d) { g_ops->push(&d); }
static DESCR_t vstack_pop(void)    { DESCR_t out; g_ops->pop(&out); return out; }
static DESCR_t vstack_peek(void)   { DESCR_t out; g_ops->peek(&out); return out; }
#define LAST_OK_GET()   (g_ops->get_last_ok())
#define LAST_OK_SET(x)  (g_ops->set_last_ok(x))
static const char *vstack_pop_str(void)
{
    DESCR_t d = vstack_pop();
    if (d.v == DT_S) return d.s ? d.s : "";
    char *s = VARVAL_fn(d);
    return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t vstack_pop_int64(void)
{
    DESCR_t d = vstack_pop();
    if (d.v == DT_I) return d.i;
    return to_int(d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _rt_IDENT(DESCR_t *a, int n)
{
    if (n == 1) return IS_NULL_fn(a[0]) ? NULVCL : FAILDESCR;
    const char *s1 = VARVAL_fn(a[0]); if (!s1) s1 = "";
    const char *s2 = VARVAL_fn(a[1]); if (!s2) s2 = "";
    return (strcmp(s1, s2) == 0) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _rt_DIFFER(DESCR_t *a, int n)
{
    if (n == 1) return IS_NULL_fn(a[0]) ? FAILDESCR : NULVCL;
    const char *s1 = VARVAL_fn(a[0]); if (!s1) s1 = "";
    const char *s2 = VARVAL_fn(a[1]); if (!s2) s2 = "";
    return (strcmp(s1, s2) != 0) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define EXPRESSION_REG_MAX 256
typedef struct { const char *name; void *fn; } ExpressionRegEntry;
static ExpressionRegEntry g_expression_reg[EXPRESSION_REG_MAX];
static int           g_expression_reg_count = 0;
void rt_register_expressions(const rt_expression_entry *tbl)
{
    if (!tbl) return;
    for (; tbl->name && g_expression_reg_count < EXPRESSION_REG_MAX; tbl++) {
        g_expression_reg[g_expression_reg_count].name = tbl->name;
        g_expression_reg[g_expression_reg_count].fn   = tbl->fn;
        g_expression_reg_count++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "../../include/IR.h"
#include "../interp/resolve_runtime.h"
extern cap_t *bb_cap_new(bb_box_fn child_fn, void *child_state, const char *varname, DESCR_t *var_ptr, int immediate);
extern cap_t *bb_cap_new_call(bb_box_fn child_fn, void *child_state, const char *fnc_name, DESCR_t *fnc_args, int fnc_nargs, char **fnc_arg_names, int fnc_n_arg_names, int immediate);
void rt_init_cap(void **slot_ptr, void *child_fn, const char *varname, int immediate)
{
    if (!slot_ptr || !child_fn) return;
    cap_t *c = bb_cap_new((bb_box_fn)child_fn, NULL, varname, NULL, immediate);
    *slot_ptr = c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_init_cap_call(void **slot_ptr, void *child_fn, const char *fnc_name)
{
    if (!slot_ptr || !child_fn) return;
    cap_t *c = bb_cap_new_call((bb_box_fn)child_fn, NULL, fnc_name, NULL, 0, NULL, 0, 0);
    *slot_ptr = c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void *bb_arbno_new(void *fn, void *state);
void rt_init_arbno(void **slot_ptr, void *child_fn)
{
    if (!slot_ptr || !child_fn) return;
    *slot_ptr = bb_arbno_new(child_fn, NULL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *chunk_reg_lookup(const char *name)
{
    if (!name || !*name) return NULL;
    for (int i = 0; i < g_expression_reg_count; i++) {
        if (strcmp(g_expression_reg[i].name, name) == 0)
            return g_expression_reg[i].fn;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t call_native_chunk(const char *fname, void *fn,
                                  DESCR_t *args, int nargs)
{
    static const DESCR_t SNUL_D = { DT_SNUL, {0}, 0, NULL };
    const char *entry = fname ? FUNC_ENTRY_fn(fname) : NULL;
    const char *retname = (entry && fname && strcmp(entry, fname) != 0 && FNCEX_fn(entry)) ? entry : fname;
    if (!retname) retname = fname ? fname : "";
    DESCR_t saved_ret = NV_GET_fn(retname);
    NV_SET_fn(retname, SNUL_D);
    DESCR_t saved_p[32];
    const char *pnames[32];
    int nbound = 0;
    for (int k = 0; k < nargs && k < 32; k++) {
        const char *pname = FUNC_PARAM_fn(fname, k);
        if (!pname || !*pname) break;
        pnames[nbound] = pname;
        saved_p[nbound] = NV_GET_fn(pname);
        NV_SET_fn(pname, args[k]);
        nbound++;
    }
    int nl = fname ? FUNC_NLOCALS_fn(fname) : 0;
    if (nl > 32) nl = 32;
    DESCR_t saved_l[32];
    const char *lnames[32];
    for (int k = 0; k < nl; k++) {
        const char *lname = FUNC_LOCAL_fn(fname, k);
        lnames[k] = lname ? lname : "";
        saved_l[k] = NV_GET_fn(lnames[k]);
        NV_SET_fn(lnames[k], SNUL_D);
    }
    int saved_vtop = g_ops->depth();
    int saved_vframe_base = g_vframe_base;
    g_vframe_base = saved_vtop;
    typedef void (*chunk_fn_t)(void);
    chunk_fn_t cfn = (chunk_fn_t)fn;
    g_native_chunk_depth++;
    cfn();
    g_native_chunk_depth--;
    g_vframe_base = saved_vframe_base;
    DESCR_t result = NV_GET_fn(retname);
    g_ops->set_depth(saved_vtop);
    for (int k = nl - 1; k >= 0; k--)
        NV_SET_fn(lnames[k], saved_l[k]);
    for (int k = nbound - 1; k >= 0; k--)
        NV_SET_fn(pnames[k], saved_p[k]);
    NV_SET_fn(retname, saved_ret);
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _rt_usercall(const char *name, DESCR_t *args, int nargs)
{
    if (!name || !*name) return FAILDESCR;
    void *fn = chunk_reg_lookup(name);
    if (fn) return call_native_chunk(name, fn, args, nargs);
    DESCR_t nv = NV_GET_fn(name);
    if (!IS_FAIL_fn(nv) && nv.v == DT_E && nv.ptr) {
        typedef DESCR_t (*cfn_t)(DESCR_t *, int);
        cfn_t cfn = (cfn_t)nv.ptr;
        return cfn(args, nargs);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_init(int argc, char **argv)
{
    (void)argc; (void)argv;
    setvbuf(stdout, NULL, _IOLBF, 0);
    bb_pool_init();
    SNO_INIT_fn();
    trail_init(&g_resolve_trail);
    prolog_atom_init();
    g_bb_mode = BB_MODE_BROKERED;
    register_fn("IDENT",  _rt_IDENT,  1, 2);
    register_fn("DIFFER", _rt_DIFFER, 1, 2);
    g_user_call_hook = _rt_usercall;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_finalize(void)
{
    return g_halt_set ? g_halt_rc : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_push_int(int64_t v)
{
    vstack_push(INTVAL(v));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_halt_tos(void)
{
    int rc = 0;
    int depth = g_ops->depth();
    if (depth > 0) {
        DESCR_t d = vstack_peek();
        if (d.v == DT_I) { rc = (int)d.i; g_ops->set_depth(depth - 1); }
    }
    g_halt_rc  = rc;
    g_halt_set = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_unhandled_op(int op)
{
    fprintf(stderr,
        "libscrip_rt: unhandled SM opcode %d reached in emitted code.\n"
        "  (scrip --dump-sm to identify; subsequent EM-N rungs shrink the set)\n",
        op);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_bomb(const char *msg)
{
    fprintf(stderr, "libscrip_rt: BOMB — %s\n", msg ? msg : "(no message)");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_push_str(const char *s, uint32_t slen)
{
    DESCR_t d;
    d.v    = DT_S;
    d.slen = slen ? slen : (uint32_t)(s ? strlen(s) : 0);
    d.s    = (char *)s;
    vstack_push(d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_write_str_nl(const char *s, uint32_t slen)
{
    if (s && slen) fwrite(s, 1, slen, stdout);
    fputc('\n', stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_write_int_nl(int64_t v)
{
    fprintf(stdout, "%lld\n", (long long)v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_write_any_nl(DESCR_t d)
{
    if (d.v == DT_I)       fprintf(stdout, "%lld\n", (long long)d.i);
    else if (d.v == DT_R)  fprintf(stdout, "%g\n", d.r);
    else if (d.v == DT_FAIL) fputc('\n', stdout);
    else {
        char *s = VARVAL_fn(d);
        if (s) fwrite(s, 1, strlen(s), stdout);
        fputc('\n', stdout);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_write_strz_nl(const char *s)
{
    if (s) fwrite(s, 1, strlen(s), stdout);
    fputc('\n', stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_frame(void)
{
    static int64_t g_frame_buf[4096];
    return (void *)g_frame_buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pop_write_int_nl(void)
{
    STACKLESS_ABORT("rt_pop_write_int_nl");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pop_write_any_nl(void)
{
    STACKLESS_ABORT("rt_pop_write_any_nl");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pop_nv_set(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_pop_nv_set");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_arith(int op)
{
    DESCR_t r = vstack_pop();
    DESCR_t l = vstack_pop();
    if (l.v == DT_FAIL || r.v == DT_FAIL) { vstack_push(FAILDESCR); LAST_OK_SET(0); return; }
    if (l.v == DT_S) l = INTVAL(to_int(l));
    if (r.v == DT_S) r = INTVAL(to_int(r));
    if (l.v == DT_SNUL) l = INTVAL(0);
    if (r.v == DT_SNUL) r = INTVAL(0);
    extern DESCR_t shared_arith(DESCR_t l, DESCR_t r, int op);
    DESCR_t result = shared_arith(l, r, op);
    vstack_push(result);
    LAST_OK_SET(result.v != DT_FAIL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_nv_get(const char *name)
{
    vstack_push(NV_GET_fn(name ? name : ""));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_nv_set(const char *name)
{
    DESCR_t val = vstack_pop();
    if (val.v == DT_FAIL) {
        vstack_push(val);
        LAST_OK_SET(0);
        return;
    }
    NV_SET_fn(name ? name : "", val);
    vstack_push(val);
    LAST_OK_SET(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_sno_assign_lit_s(const char *name, const char *str)
{
    DESCR_t d;
    d.v    = DT_S;
    d.s    = (char *)(str ? str : "");
    d.slen = (uint32_t)strlen(d.s);
    NV_SET_fn(name ? name : "", d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SBL-M3-ARITH (2026-05-31): integer analog of rt_sno_assign_lit_s. The stackless RO-int binop box (GZ-3 bb_binop.cpp) computed its raw int64 result into a ζ-frame slot; the SNOBOL4 assign box reads that slot by value     */
/* and hands it here with the target name. Build a DT_I integer DESCR and store via NV_SET_fn — the same associated-variable hook (output_val) that handles OUTPUT assignment in rt_sno_assign_lit_s, so `OUTPUT = 2 + 3` prints. */
void rt_sno_assign_int(const char *name, int64_t val)
{
    DESCR_t d;
    d.v    = DT_I;
    d.slen = 0;
    d.i    = val;
    NV_SET_fn(name ? name : "", d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SBL-M3-VAR (2026-05-31): SNOBOL4 `dst = src` where src is a bare variable reference (`OUTPUT = S`). Fully stackless — the dst and src names are RO immediates baked by the bb_sno_assign IR_VAR arm; this fetches src's value */
/* via NV_GET_fn and stores it under dst via NV_SET_fn (the same associated-variable hook, so OUTPUT prints, S round-trips). VARVAL is resolved by NV_GET_fn (handles NAME indirection). NO value stack (Lon directive).        */
void rt_sno_assign_var(const char *dst, const char *src)
{
    NV_SET_fn(dst ? dst : "", NV_GET_fn(src ? src : ""));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PB-0 SUBJECT phase (GOAL-SNOBOL4-BB SBL-PAT-BB, 2026-05-31). The SUBJECT byrd box (bb_sno_subject.cpp) evaluates a SNOBOL4 match statement's subject value-expr and establishes the SCANNED WHOLE: Σ (base ptr) + Δ (length). */
/* SPITBOL Manual ch.18 (pattern-match algorithm): Σ/Ω are the fixed subject + bound; the cursor is set to ZERO when the match begins, so the cursor δ is owned by the matcher (PB-2 BB_MATCH), NOT this box. This helper returns */
/* {base,len} (SysV: base in rax, len in rdx — a 16-byte two-INTEGER struct) so the box can store Σ→[r12+off] and Δ→[r12+off+8] into its ζ-frame slot. A variable subject is resolved via VARVAL_d_fn (NAME indirection +     */
/* string slen, honoring embedded NULs); a literal subject rides on `lit` directly. g_sno_subject_dbg_* expose the loaded base+len for the PB-0 mode-3 execution probe (verify the box ran and computed the right length).      */
extern DESCR_t VARVAL_d_fn(DESCR_t d);
typedef struct { const char *base; long len; } rt_subj_t;
const char *g_sno_subject_dbg_base = 0;
long        g_sno_subject_dbg_len  = -1;
rt_subj_t rt_sno_subject_load(const char *name, const char *lit)
{
    rt_subj_t r;
    if (lit) {
        r.base = lit;
        r.len  = (long)strlen(lit);
    } else {
        DESCR_t d = VARVAL_d_fn(NV_GET_fn(name ? name : ""));
        if (d.v == DT_S || d.v == DT_SNUL) {
            r.base = d.s ? d.s : "";
            r.len  = d.slen ? (long)d.slen : (long)strlen(r.base);
        } else {
            const char *s = VARVAL_fn(NV_GET_fn(name ? name : ""));
            r.base = s ? s : "";
            r.len  = (long)strlen(r.base);
        }
    }
    g_sno_subject_dbg_base = r.base;
    g_sno_subject_dbg_len  = r.len;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pop_void(void)
{
    (void)vstack_pop();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pop_store_i64(int64_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_pop_store_i64");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_push_stored_i64(const int64_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_push_stored_i64");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pop_store_descr(DESCR_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_pop_store_descr");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_case_eq(const DESCR_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_case_eq");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_FRAME_STACK_MAX 256
#define RT_FRAME_SLOT_MAX  64
typedef struct { DESCR_t slot[RT_FRAME_SLOT_MAX]; int nslots; } rt_frame_t;
static rt_frame_t g_rt_frames[RT_FRAME_STACK_MAX];
static int        g_rt_frame_depth = 0;
void rt_frame_enter(int nparams)
{
    if (nparams < 0) nparams = 0;
    if (nparams > RT_FRAME_SLOT_MAX) nparams = RT_FRAME_SLOT_MAX;
    if (g_rt_frame_depth >= RT_FRAME_STACK_MAX) {
        for (int k = 0; k < nparams; k++) (void)vstack_pop();
        return;
    }
    rt_frame_t *f = &g_rt_frames[g_rt_frame_depth++];
    for (int k = 0; k < RT_FRAME_SLOT_MAX; k++) f->slot[k] = NULVCL;
    f->nslots = nparams;
    for (int i = 0; i < nparams; i++) {
        DESCR_t v = vstack_pop();
        int s = nparams - 1 - i;
        if (s >= 0 && s < RT_FRAME_SLOT_MAX) f->slot[s] = v;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_frame_leave(void)
{
    if (g_rt_frame_depth > 0) g_rt_frame_depth--;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_ICN_PROC_MAX 512
typedef struct { const char *name; bb_box_fn fn; void *entry; const char **pnames; int nparams; } rt_proc_t;
static rt_proc_t g_rt_gen_procs[RT_ICN_PROC_MAX];
static int           g_rt_gen_proc_count = 0;
static bb_box_fn (*g_rt_gen_proc_builder)(void *entry) = NULL;
void rt_proc_set_builder(bb_box_fn (*builder)(void *entry)) { g_rt_gen_proc_builder = builder; }
void rt_proc_register(const char *name, void *entry, const char **pnames, int nparams)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++) {
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) {
            if (entry)   g_rt_gen_procs[i].entry   = entry;
            if (pnames)  g_rt_gen_procs[i].pnames  = pnames;
            if (nparams) g_rt_gen_procs[i].nparams = nparams;
            return;
        }
    }
    if (g_rt_gen_proc_count >= RT_ICN_PROC_MAX) return;
    rt_proc_t *p = &g_rt_gen_procs[g_rt_gen_proc_count++];
    p->name = name; p->fn = NULL; p->entry = entry; p->pnames = pnames; p->nparams = nparams;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_proc_reset(void) { g_rt_gen_proc_count = 0; }
int rt_proc_is_registered(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_call_proc(const char *name, int nargs)
{
    (void)name;
    (void)nargs;
    STACKLESS_ABORT("rt_call_proc");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int builtin_is_generator(const char *name)
{
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any")
        || !strcmp(name, "many") || !strcmp(name, "bal") || !strcmp(name, "key")
        || !strcmp(name, "seq");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_builtin_is_known(const char *name)
{
    if (!name) return 0;
    if (rt_proc_is_registered(name)) return 0;
    if (builtin_is_generator(name))  return 0;
    static const char *known[] = {
        "write", "writes", "stop",
        "integer", "real", "string", "numeric", "char", "ord", "cset",
        "type", "image", "proc", "args", "copy",
        "abs", "sqrt", "sin", "cos", "tan", "exp", "log",
        "max", "min",
        "trim", "reverse", "repl", "map", "left", "center", "right",
        "table", "list", "set", "sort", "sortf", "get", "pop", "pull",
        "member", "insert", "delete", "key",
        "MAKELIST",
        NULL
    };
    for (int i = 0; known[i]; i++) if (!strcmp(known[i], name)) return 1;
    {
        extern void *dat_find_type(const char *nm);
        if (dat_find_type(name)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_call_builtin(const char *name, int nargs)
{
    (void)name;
    (void)nargs;
    STACKLESS_ABORT("rt_call_builtin");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_load_frame(int slot)
{
    if (g_rt_frame_depth <= 0 || slot < 0 || slot >= RT_FRAME_SLOT_MAX) {
        vstack_push(FAILDESCR);
        LAST_OK_SET(0);
        return;
    }
    rt_frame_t *f = &g_rt_frames[g_rt_frame_depth - 1];
    vstack_push(f->slot[slot]);
    LAST_OK_SET((f->slot[slot].v != DT_FAIL));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_store_frame(int slot)
{
    DESCR_t v = vstack_pop();
    if (g_rt_frame_depth <= 0 || slot < 0 || slot >= RT_FRAME_SLOT_MAX) {
        vstack_push(v);
        LAST_OK_SET(0);
        return;
    }
    rt_frame_t *f = &g_rt_frames[g_rt_frame_depth - 1];
    f->slot[slot] = v;
    if (slot >= f->nslots) f->nslots = slot + 1;
    vstack_push(v);
    LAST_OK_SET((v.v != DT_FAIL));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_last_ok(void)  { return LAST_OK_GET(); }
void rt_set_last_ok(int ok) { LAST_OK_SET(ok ? 1 : 0); }
void rt_push_expression_descr(int64_t entry_pc, int64_t arity)
{
    DESCR_t d;
    d.v    = DT_E;
    d.slen = (uint32_t)arity;
    d.i    = entry_pc;
    vstack_push(d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void rt_set_last_ok(int v);
void rt_exec_stmt_pat(void *blob_α, const char *subj_name, int has_repl)
{
    DESCR_t repl   = vstack_pop();
    DESCR_t subj_d = vstack_pop();
    (void)vstack_pop();
    bb_box_fn root_fn = (bb_box_fn)blob_α;
    int ok = exec_stmt_blob(subj_name, &subj_d, root_fn, has_repl ? &repl : NULL, has_repl);
    rt_set_last_ok(ok);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_match_blob(void *blob_α,
                         const char *subj_name,
                         int has_repl)
{
    DESCR_t repl = vstack_pop();
    DESCR_t subj = vstack_pop();
    bb_box_fn root_fn = (bb_box_fn)blob_α;
    int ok = exec_stmt_blob(subj_name,
                            &subj,
                            root_fn,
                            has_repl ? &repl : NULL,
                            has_repl);
    rt_set_last_ok(ok);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_lit(const char *s)
{
    vstack_push(pat_lit(s ? s : ""));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_refname(const char *name)
{
    vstack_push(pat_ref(name ? name : ""));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_span(void)
{
    const char *cs = vstack_pop_str();
    vstack_push(pat_span(cs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_break(void)
{
    const char *cs = vstack_pop_str();
    vstack_push(pat_break_(cs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_breakx(void)
{
    const char *cs = vstack_pop_str();
    vstack_push(pat_breakx(cs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_any(void)
{
    const char *cs = vstack_pop_str();
    vstack_push(pat_any_cs(cs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_notany(void)
{
    const char *cs = vstack_pop_str();
    vstack_push(pat_notany(cs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_len(void)   { vstack_push(pat_len (vstack_pop_int64())); }
void rt_pat_pos(void)   { vstack_push(pat_pos (vstack_pop_int64())); }
void rt_pat_rpos(void)  { vstack_push(pat_rpos(vstack_pop_int64())); }
void rt_pat_tab(void)   { vstack_push(pat_tab (vstack_pop_int64())); }
void rt_pat_rtab(void)  { vstack_push(pat_rtab(vstack_pop_int64())); }
void rt_pat_arb(void)     { vstack_push(pat_arb());     }
void rt_pat_rem(void)     { vstack_push(pat_rem());     }
#define RT_DCAP_MAX 32
typedef struct { const char *varname; const char *base; int len; } rt_dcap_t;
static rt_dcap_t g_rt_dcap[RT_DCAP_MAX];
static int       g_rt_dcap_n = 0;
int              g_rt_dcap_active = 0;
static void rt_dcap_record(const char *vname, const char *base, int len) {
    if (!vname || !*vname) return;
    for (int i = 0; i < g_rt_dcap_n; i++) {
        if (g_rt_dcap[i].varname && strcmp(g_rt_dcap[i].varname, vname) == 0) {
            g_rt_dcap[i].base = base; g_rt_dcap[i].len = len; return;
        }
    }
    if (g_rt_dcap_n < RT_DCAP_MAX) {
        g_rt_dcap[g_rt_dcap_n].varname = vname;
        g_rt_dcap[g_rt_dcap_n].base    = base;
        g_rt_dcap[g_rt_dcap_n].len     = len;
        g_rt_dcap_n++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_flush(void) {
    for (int i = 0; i < g_rt_dcap_n; i++) {
        int len = g_rt_dcap[i].len < 0 ? 0 : g_rt_dcap[i].len;
        char *copy = (char *)GC_MALLOC((size_t)len + 1);
        if (copy) { if (len > 0 && g_rt_dcap[i].base) memcpy(copy, g_rt_dcap[i].base, (size_t)len); copy[len] = '\0'; }
        DESCR_t d = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
        NV_SET_fn(g_rt_dcap[i].varname, d);
    }
    g_rt_dcap_n = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_clear(void) { g_rt_dcap_n = 0; }
void rt_cap_assign(const char *varname, const char *base, int len)
{
    if (!varname || !*varname) return;
    if (len < 0) len = 0;
    if (g_rt_dcap_active) { rt_dcap_record(varname, base, len); return; }
    char *copy = (char *)GC_MALLOC((size_t)len + 1);
    if (copy) { if (len > 0 && base) memcpy(copy, base, (size_t)len); copy[len] = '\0'; }
    DESCR_t matched = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
    NV_SET_fn(varname, matched);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_cap_assign_cursor(const char *varname, int saved_delta, int cur_delta, int is_imm)
{
    (void)is_imm;
    if (!varname || !*varname) return;
    int len = cur_delta - saved_delta;
    if (len < 0) len = 0;
    const char *base = Σ ? Σ + saved_delta : NULL;
    if (g_rt_dcap_active) { rt_dcap_record(varname, base, len); return; }
    char *copy = (char *)GC_MALLOC((size_t)len + 1);
    if (copy) { if (len > 0 && base) memcpy(copy, base, (size_t)len); copy[len] = '\0'; }
    DESCR_t matched = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
    NV_SET_fn(varname, matched);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_at_cursor(const char *varname, int cur_delta)
{
    if (!varname || !*varname) return;
    DESCR_t pos = { .v = DT_I, .i = (int64_t)cur_delta };
    NV_SET_fn(varname, pos);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int exec_stmt(const char *sname, DESCR_t *sv, DESCR_t pat, DESCR_t *repl, int has_repl);
extern const char *Σ;
extern int Σlen;
int rt_defer_match(const char *varname, int ival_flag, int cur_delta)
{
    DESCR_t val = NV_GET_fn(varname ? varname : "");
    if (ival_flag) {
        if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s);
        else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val);
    }
    if (val.v == DT_S || val.v == DT_SNUL) {
        const char *lit = val.s ? val.s : "";
        int llen = val.slen ? (int)val.slen : (int)strlen(lit);
        if (cur_delta + llen > Σlen) return -1;
        if (llen > 0 && strncmp(Σ + cur_delta, lit, (size_t)llen) != 0) return -1;
        return cur_delta + llen;
    }
    if (val.v == DT_P && val.p) {
        const char *save_Σ = Σ; int save_Σlen = Σlen;
        extern int Ω; int save_Ω = Ω;
        extern int Δ; int save_Δ = Δ;
        const char *sub = Σ + cur_delta; int sublen = Σlen - cur_delta;
        Σ = sub; Σlen = sublen; Ω = sublen; Δ = 0;
        DESCR_t sub_d = { .v = DT_S, .slen = (uint32_t)sublen, .s = (char *)sub };
        int ok = exec_stmt(NULL, &sub_d, val, NULL, 0);
        int matched = ok ? Δ : 0;
        Σ = save_Σ; Σlen = save_Σlen; Ω = save_Ω; Δ = save_Δ;
        if (!ok) return -1;
        return cur_delta + matched;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_fence(void)   { vstack_push(pat_fence());   }
void rt_pat_fail(void)    { vstack_push(pat_fail());    }
void rt_pat_abort(void)   { vstack_push(pat_abort());   }
void rt_pat_succeed(void) { vstack_push(pat_succeed()); }
void rt_pat_bal(void)     { vstack_push(pat_bal());     }
void rt_pat_eps(void)     { vstack_push(pat_epsilon()); }
void rt_pat_arbno(void)
{
    DESCR_t inner = vstack_pop();
    vstack_push(pat_arbno(inner));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_fence1(void)
{
    DESCR_t child = vstack_pop();
    vstack_push(pat_fence_p(child));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_cat(void)
{
    DESCR_t right = vstack_pop();
    DESCR_t left  = vstack_pop();
    vstack_push(pat_cat(left, right));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_alt(void)
{
    DESCR_t right = vstack_pop();
    DESCR_t left  = vstack_pop();
    vstack_push(pat_alt(left, right));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_deref(void)
{
    DESCR_t v = vstack_pop();
    if (v.v == DT_P) {
        vstack_push(v);
    } else if (v.v == DT_S && v.s) {
        vstack_push(pat_lit(v.s));
    } else {
        char *name = VARVAL_fn(v);
        vstack_push(pat_ref(name ? name : ""));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture(const char *varname, int kind)
{
    DESCR_t child = vstack_pop();
    DESCR_t var   = NAME_fn(varname ? varname : "");
    DESCR_t result;
    if (kind == 1)
        result = pat_assign_imm(child, var);
    else if (kind == 2) {
        vstack_push(pat_cat(child, pat_at_cursor(varname ? varname : "")));
        return;
    } else
        result = pat_assign_cond(child, var);
    if (result.v == DT_P && result.p && varname && varname[0]) {
        PATND_t *pp = (PATND_t *)result.p;
        if (!pp->STRVAL_fn || !pp->STRVAL_fn[0])
            pp->STRVAL_fn = GC_strdup(varname);
    }
    vstack_push(result);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture_fn(const char *fname, int is_imm, const char *namelist)
{
    DESCR_t child = vstack_pop();
    if (!fname) fname = "";
    if (namelist && namelist[0]) {
        int nnames = 1;
        for (const char *q = namelist; *q; q++) if (*q == '\t') nnames++;
        char **names = (char **)GC_MALLOC((size_t)nnames * sizeof(char *));
        int ni = 0;
        const char *start = namelist;
        for (const char *q = namelist; ; q++) {
            if (*q == '\t' || *q == '\0') {
                size_t len = (size_t)(q - start);
                char *nm = (char *)GC_MALLOC(len + 1);
                memcpy(nm, start, len);  nm[len] = '\0';
                names[ni++] = nm;
                if (*q == '\0') break;
                start = q + 1;
            }
        }
        vstack_push(is_imm
            ? pat_assign_callcap_named_imm(child, fname, NULL, 0, names, nnames)
            : pat_assign_callcap_named    (child, fname, NULL, 0, names, nnames));
    } else {
        vstack_push(is_imm
            ? pat_assign_callcap_named_imm(child, fname, NULL, 0, NULL, 0)
            : pat_assign_callcap          (child, fname, NULL, 0));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture_fn_args(const char *fname, int is_imm, int nargs)
{
    if (!fname) fname = "";
    DESCR_t *argv = nargs > 0
        ? (DESCR_t *)GC_MALLOC((size_t)nargs * sizeof(DESCR_t))
        : NULL;
    for (int i = nargs - 1; i >= 0; i--) argv[i] = vstack_pop();
    DESCR_t child = vstack_pop();
    vstack_push(is_imm
        ? pat_assign_callcap_named_imm(child, fname, argv, nargs, NULL, 0)
        : pat_assign_callcap          (child, fname, argv, nargs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_usercall(const char *fname)
{
    if (!fname) fname = "";
    vstack_push(pat_user_call(fname, NULL, 0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pat_usercall_args(const char *fname, int nargs)
{
    if (!fname) fname = "";
    DESCR_t *argv = nargs > 0
        ? (DESCR_t *)GC_MALLOC((size_t)nargs * sizeof(DESCR_t))
        : NULL;
    for (int i = nargs - 1; i >= 0; i--) argv[i] = vstack_pop();
    vstack_push(pat_user_call(fname, argv, nargs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_match_variant(const char *subj_name, int has_repl)
{
    DESCR_t repl   = vstack_pop();
    DESCR_t subj_d = vstack_pop();
    DESCR_t pat_d  = vstack_pop();
    int err = setjmp(g_core_err_jmp);
    g_core_err_active = 1;
    if (err != 0) {
        exec_stmt_pool_reset();
        LAST_OK_SET(0);
        return;
    }
    int ok = exec_stmt(subj_name, &subj_d, pat_d,
                       has_repl ? &repl : NULL, has_repl);
    g_core_err_active = 0;
    LAST_OK_SET(ok ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_concat(void)
{
    DESCR_t r = vstack_pop();
    DESCR_t l = vstack_pop();
    DESCR_t result = CONCAT_fn(l, r);
    vstack_push(result);
    LAST_OK_SET((result.v != DT_FAIL));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern DESCR_t binop_apply(int op, DESCR_t lv, DESCR_t rv, int *rel_fail);
void rt_gen_concat(void)
{
    STACKLESS_ABORT("rt_gen_concat");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_set_stno(int64_t stno)
{
    extern void comm_stno(int n);
    kw_stno = stno;
    comm_stno((int)stno);
    if (g_ops->depth() > g_vframe_base) g_ops->set_depth(g_vframe_base);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_push_null(void)
{
    vstack_push(NULVCL);
    LAST_OK_SET(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_coerce_num(void)
{
    DESCR_t v = vstack_pop();
    if (v.v == DT_FAIL) { vstack_push(FAILDESCR); LAST_OK_SET(0); return; }
    if (v.v == DT_S || v.v == DT_SNUL) {
        const char *s = v.s ? v.s : "";
        int is_real = 0;
        for (const char *p = s; *p; p++) {
            if (*p == '.' || *p == 'e' || *p == 'E' || *p == 'd' || *p == 'D') { is_real = 1; break; }
        }
        if (is_real) vstack_push(REALVAL(to_real(v)));
        else         vstack_push(INTVAL(to_int(v)));
    } else {
        vstack_push(v);
    }
    LAST_OK_SET(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_push_real_bits(uint64_t bits)
{
    double v;
    __builtin_memcpy(&v, &bits, 8);
    vstack_push(REALVAL(v));
    LAST_OK_SET(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_push_null_noflip(void)
{
    vstack_push(NULVCL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_push_expr(void *ptr)
{
    DESCR_t d;
    d.v    = DT_E;
    d.slen = 0;
    d.ptr  = ptr;
    vstack_push(d);
    LAST_OK_SET(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_exp(void)
{
    DESCR_t r = vstack_pop();
    DESCR_t l = vstack_pop();
    if (l.v == DT_FAIL || r.v == DT_FAIL) {
        vstack_push(FAILDESCR); LAST_OK_SET(0); return;
    }
    if (l.v == DT_S) l = INTVAL(to_int(l));
    if (r.v == DT_S) r = INTVAL(to_int(r));
    if (l.v == DT_SNUL) l = INTVAL(0);
    if (r.v == DT_SNUL) r = INTVAL(0);
    extern DESCR_t shared_arith(DESCR_t l, DESCR_t r, int op);
    DESCR_t result = shared_arith(l, r, SM_EXP);
    vstack_push(result);
    LAST_OK_SET(result.v != DT_FAIL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_neg(void)
{
    DESCR_t v = vstack_pop();
    if (v.v == DT_I) vstack_push(INTVAL(-v.i));
    else              vstack_push(REALVAL(-to_real(v)));
    LAST_OK_SET(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unop_neg(void)
{
    STACKLESS_ABORT("rt_unop_neg");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unop_pos(void)
{
    STACKLESS_ABORT("rt_unop_pos");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unop_nonnull(void)
{
    STACKLESS_ABORT("rt_unop_nonnull");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unop_null_test(void)
{
    STACKLESS_ABORT("rt_unop_null_test");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unop_not(void)
{
    STACKLESS_ABORT("rt_unop_not");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unop_size(void)
{
    STACKLESS_ABORT("rt_unop_size");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_field_get(const char *fname)
{
    (void)fname;
    STACKLESS_ABORT("rt_field_get");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_field_set(const char *fname)
{
    (void)fname;
    STACKLESS_ABORT("rt_field_set");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_idx_get(void)
{
    STACKLESS_ABORT("rt_idx_get");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_idx_set(void)
{
    STACKLESS_ABORT("rt_idx_set");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_list_bang(DESCR_t *obj_slot, int64_t *idx_slot, int *state_slot, int reset)
{
    (void)obj_slot;
    (void)idx_slot;
    (void)state_slot;
    (void)reset;
    STACKLESS_ABORT("rt_list_bang");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_limit_begin(DESCR_t *max_slot, int64_t *count_slot)
{
    (void)max_slot;
    (void)count_slot;
    STACKLESS_ABORT("rt_limit_begin");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_limit_more(DESCR_t *max_slot, int64_t *count_slot)
{
    if (*count_slot < max_slot->i) { LAST_OK_SET(1); return 1; }
    LAST_OK_SET(0);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_limit_inc(int64_t *count_slot)
{
    (*count_slot)++;
    LAST_OK_SET(1);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_toby_real(DESCR_t *cur_slot, int64_t lo_bits, int64_t hi_bits, int64_t step_bits, int reset)
{
    (void)cur_slot;
    (void)lo_bits;
    (void)hi_bits;
    (void)step_bits;
    (void)reset;
    STACKLESS_ABORT("rt_toby_real");
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_incr(int64_t n)
{
    DESCR_t v = vstack_pop();
    vstack_push(INTVAL(v.i + n));
    LAST_OK_SET(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_decr(int64_t n)
{
    DESCR_t v = vstack_pop();
    vstack_push(INTVAL(v.i - n));
    LAST_OK_SET(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_acomp(int op)
{
    DESCR_t r = vstack_pop();
    DESCR_t l = vstack_pop();
    if (l.v == DT_FAIL || r.v == DT_FAIL) {
        vstack_push(FAILDESCR); LAST_OK_SET(0); return;
    }
    if (junction_is(r) || junction_is(l)) {
        int jr = junction_is(r);
        DESCR_t jct = jr ? r : l;
        DESCR_t scl = jr ? l : r;
        int jok = junction_collapse(scl, jct, op, 1);
        vstack_push(jok ? scl : FAILDESCR); LAST_OK_SET(jok ? 1 : 0); return;
    }
    if (l.v == DT_SNUL) l = INTVAL(0);
    if (r.v == DT_SNUL) r = INTVAL(0);
    double lv = (l.v == DT_R) ? l.r : (l.v == DT_I) ? (double)l.i : (l.v == DT_S && l.s) ? strtod(l.s, NULL) : 0.0;
    double rv = (r.v == DT_R) ? r.r : (r.v == DT_I) ? (double)r.i : (r.v == DT_S && r.s) ? strtod(r.s, NULL) : 0.0;
    int ok;
    switch (op) {
        case TT_EQ: ok = (lv == rv); break;
        case TT_NE: ok = (lv != rv); break;
        case TT_LT: ok = (lv <  rv); break;
        case TT_LE: ok = (lv <= rv); break;
        case TT_GT: ok = (lv >  rv); break;
        case TT_GE: ok = (lv >= rv); break;
        default:    ok = (lv == rv); break;
    }
    vstack_push(ok ? r : FAILDESCR);
    LAST_OK_SET(ok ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_lcomp(int op)
{
    DESCR_t r = vstack_pop();
    DESCR_t l = vstack_pop();
    if (l.v == DT_FAIL || r.v == DT_FAIL) {
        vstack_push(FAILDESCR); LAST_OK_SET(0); return;
    }
    if (junction_is(r) || junction_is(l)) {
        int jr = junction_is(r);
        DESCR_t jct = jr ? r : l;
        DESCR_t scl = jr ? l : r;
        int jok = junction_collapse(scl, jct, op, 0);
        vstack_push(jok ? scl : FAILDESCR); LAST_OK_SET(jok ? 1 : 0); return;
    }
    const char *ls = VARVAL_fn(l); if (!ls) ls = "";
    const char *rs = VARVAL_fn(r); if (!rs) rs = "";
    int cmp = strcmp(ls, rs);
    int ok;
    switch (op) {
        case TT_LLT: ok = (cmp <  0); break;
        case TT_LLE: ok = (cmp <= 0); break;
        case TT_LGT: ok = (cmp >  0); break;
        case TT_LGE: ok = (cmp >= 0); break;
        case TT_LEQ: ok = (cmp == 0); break;
        case TT_LNE: ok = (cmp != 0); break;
        default:     ok = (cmp == 0); break;
    }
    vstack_push(ok ? r : FAILDESCR);
    LAST_OK_SET(ok ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_define_entry(void)
{
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_define(void)
{
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_unhandled_sm(int op)
{
    fprintf(stderr, "libscrip_rt: unhandled SM opcode %d in emitted binary (M5 territory)\n", op);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_atom(const char *s)
{
    if (s) fputs(s, stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_pl_format_float(char *buf, size_t bufsz, double d)
{
    for (int prec = 15; prec <= 17; prec++) {
        snprintf(buf, bufsz, "%.*g", prec, d);
        if (strtod(buf, NULL) == d) break;
    }
    if (!strpbrk(buf, ".eEnN")) {
        size_t n = strlen(buf);
        if (n + 2 < bufsz) { buf[n] = '.'; buf[n+1] = '0'; buf[n+2] = '\0'; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_int(long v)
{
    printf("%ld", v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_float(double v)
{
    char fb[64]; rt_pl_format_float(fb, sizeof fb, v); fputs(fb, stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_cstr(const char *s)
{
    if (s) fputs(s, stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cut_set(void)
{
    extern int g_resolve_cut_flag;
    g_resolve_cut_flag = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_var(int slot)
{
    extern Term **g_resolve_env;
    if (!g_resolve_env || slot < 0) { fputs("_", stdout); return; }
    Term *t = g_resolve_env[slot] ? term_deref(g_resolve_env[slot]) : NULL;
    if (!t) { fputs("_", stdout); return; }
    if (t->tag == TERM_INT)   { printf("%ld", (long)t->ival); return; }
    if (t->tag == TERM_FLOAT) { char fb[64]; rt_pl_format_float(fb, sizeof fb, t->fval); fputs(fb, stdout); return; }
    if (t->tag == TERM_ATOM)  { const char *nm = prolog_atom_name(t->atom_id);
                                 if (nm) fputs(nm, stdout); return; }
    if (t->tag == TERM_COMPOUND) { extern void pl_write(Term *); pl_write(t); return; }
    fputs("_", stdout);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_term_ptr(void *t)
{
    extern void pl_write(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_write(d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_writeq_term_ptr(void *t)
{
    extern void pl_writeq(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_writeq(d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_write_canonical_term_ptr(void *t)
{
    extern void pl_write_canonical(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_write_canonical(d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_node_to_term(int kind, long ival, const char *sval, double dval)
{
    extern Term **g_resolve_env;
    switch (kind) {
    case IR_LOGICVAR: {
        int slot = (int)ival;
        Term *t = (g_resolve_env && slot >= 0 && g_resolve_env[slot]) ? term_deref(g_resolve_env[slot]) : NULL;
        if (!t) { t = term_new_var(slot); if (g_resolve_env && slot >= 0) g_resolve_env[slot] = t; }
        return t;
    }
    case IR_ATOM:  return term_new_atom(prolog_atom_intern(sval ? sval : "[]"));
    case IR_LIT_F: return term_new_float(dval);
    case IR_LIT_I: return term_new_int(ival);
    default:       return term_new_int(ival);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_unify_terms(void *l, void *r)
{
    extern Trail g_resolve_trail;
    Term *lt = (Term *)l, *rt_ = (Term *)r;
    if (!lt || !rt_) return 0;
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(lt, rt_, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_trail_mark(void)
{
    extern Trail g_resolve_trail;
    return trail_mark(&g_resolve_trail);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_trail_unwind(int mark)
{
    extern Trail g_resolve_trail;
    trail_unwind(&g_resolve_trail, mark);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_PL_MARK_STACK_MAX 32
static int g_resolve_mark_stack[RT_PL_MARK_STACK_MAX];
static int g_resolve_mark_top = 0;
void rt_pl_trail_mark_push(void)
{
    extern Trail g_resolve_trail;
    int m = trail_mark(&g_resolve_trail);
    if (g_resolve_mark_top < RT_PL_MARK_STACK_MAX) g_resolve_mark_stack[g_resolve_mark_top++] = m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_trail_unwind_top(void)
{
    extern Trail g_resolve_trail;
    if (g_resolve_mark_top <= 0) return;
    int m = g_resolve_mark_stack[g_resolve_mark_top - 1];
    trail_unwind(&g_resolve_trail, m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_trail_mark_pop(void)
{
    if (g_resolve_mark_top > 0) g_resolve_mark_top--;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Term **rt_pl_env_current(void) { extern Term **g_resolve_env; return g_resolve_env; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PLG-9b (2026-05-31): allocate the per-activation logic-variable environment for the native flat path.    */
/* In mode-3 the driver GC_MALLOCs g_resolve_env before bb_build_flat; in mode-4 the emitted main: wrapper  */
/* has no driver to do that, so it calls this once before invoking the body (mirrors the driver's           */
/* GC_MALLOC(nslots+8) at scrip.c). g_resolve_env holds Term* slots that rt_pl_node_to_term / rt_pl_unify_  */
/* terms / rt_pl_write_var read and write; the slots ARE the per-box RW storage for logic variables.        */
void rt_pl_env_alloc(int nslots)
{
    extern Term **g_resolve_env;
    extern Trail  g_resolve_trail;
    int n = (nslots > 0 ? nslots : 1) + 8;
    g_resolve_env = (Term **)GC_MALLOC((size_t)n * sizeof(Term *));
    /* The mode-4 standalone binary's main: never calls rt_init (that is the interpreter driver's */
    /* entry), so the binding trail rt_pl_unify_terms uses is still zero-initialized {NULL,0,0}.   */
    /* The PLG-9a hello tier never touched it (write of a constant atom needs no trail); the first */
    /* unify in PLG-9b limps on a 0-capacity trail by luck, but the SECOND trail_push dereferences */
    /* past the zero-size block and segfaults. Initialize it here, alongside the env, since this   */
    /* is the native-flat Prolog runtime-state setup hook. Idempotent re-init across activations is */
    /* harmless for the single-activation flat tier (no live bindings span the call).               */
    trail_init(&g_resolve_trail);
}
void rt_pl_cp_save_caller_env(void *caller_env) { if (g_resolve_bfr) g_resolve_bfr->saved_args = (Term **)caller_env; }
void rt_pl_choice_cut_enter(void *cp_void)
{
    resolve_choice *cp = (resolve_choice *)cp_void;
    if (!cp) return;
    cp->saved_cut_flag    = g_resolve_cut_flag;
    cp->saved_cut_barrier = g_resolve_cut_barrier;
    g_resolve_cut_flag         = 0;
    g_resolve_cut_barrier      = cp->parent;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_choice_cut_exit(void *cp_void)
{
    resolve_choice *cp = (resolve_choice *)cp_void;
    if (!cp) return;
    g_resolve_cut_flag    = cp->saved_cut_flag;
    g_resolve_cut_barrier = cp->saved_cut_barrier;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_choice_cut_unwind(void *cp_void)
{
    resolve_choice *cp = (resolve_choice *)cp_void;
    if (!cp) return;
    resolve_choice *parent = cp->parent;
    g_resolve_cut_flag    = cp->saved_cut_flag;
    g_resolve_cut_barrier = cp->saved_cut_barrier;
    resolve_cp_truncate(parent);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_get_cut_flag(void)
{
    return g_resolve_cut_flag;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Term **resolve_bb_env_install(Term **env)
{
    extern Term **g_resolve_env;
    Term **prev = g_resolve_env;
    g_resolve_env = env;
    return prev;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_pl_arith(int lk, long li, const char *ls,
                  int rk, long ri, const char *rs, const char *op)
{
    extern Term **g_resolve_env;
    (void)ls; (void)rs;
    long lv = li;
    if (lk == IR_LOGICVAR && g_resolve_env && li >= 0) {
        Term *t = g_resolve_env[li] ? term_deref(g_resolve_env[li]) : NULL;
        if (t && t->tag == TERM_INT) lv = t->ival;
    }
    long rv = ri;
    if (rk == IR_LOGICVAR && g_resolve_env && ri >= 0) {
        Term *t = g_resolve_env[ri] ? term_deref(g_resolve_env[ri]) : NULL;
        if (t && t->tag == TERM_INT) rv = t->ival;
    }
    if (!op) return lv + rv;
    if (strcmp(op, "sign")==0)     return (lv > 0) ? 1 : (lv < 0) ? -1 : 0;
    if (strcmp(op, "abs")==0)      return (lv < 0) ? -lv : lv;
    if (strcmp(op, "truncate")==0) return lv;
    if (strcmp(op, "integer")==0)  return lv;
    if (strcmp(op, "round")==0)    return lv;
    if (strcmp(op, "ceiling")==0)  return lv;
    if (strcmp(op, "floor")==0)    return lv;
    if (strcmp(op, "\\")==0)       return ~lv;
    if (strcmp(op, "msb")==0)      { long v=lv, m=-1; while(v){v>>=1;m++;} return m; }
    if (strcmp(op, "**") == 0)   { long r=1; for(long i=0;i<rv;i++) r*=lv; return r; }
    if (strcmp(op, "^") == 0)    { long r=1; for(long i=0;i<rv;i++) r*=lv; return r; }
    if (strcmp(op, "/\\") == 0)  return lv & rv;
    if (strcmp(op, "\\/") == 0)  return lv | rv;
    if (strcmp(op, "xor") == 0)  return lv ^ rv;
    if (strcmp(op, ">>") == 0)   return lv >> rv;
    if (strcmp(op, "<<") == 0)   return lv << rv;
    if (strcmp(op, "mod") == 0)  return rv ? lv % rv : 0;
    if (strcmp(op, "rem") == 0)  return rv ? lv % rv : 0;
    /* PLG-9c (2026-05-31): gcd + div were present in bb_exec.c's resolve_arith_eval (the mode-2/3 */
    /* evaluator the IR_ARITH/rt_pl_is_eval path uses) but MISSING here in rt_pl_arith (the integer */
    /* evaluator the mode-4 MEDIUM_TEXT `is` arm reaches via rt_pl_is). With them absent, gcd(12,8)  */
    /* fell through to the `return lv + rv` default → 20 (wrong, addition) in mode-4 only. Added to  */
    /* match bb_exec.c byte-for-byte (gcd: Euclid on absolute values; div: floored integer division). */
    if (strcmp(op, "gcd") == 0)  { long a = lv<0?-lv:lv, b = rv<0?-rv:rv; while (b) { long r = a % b; a = b; b = r; } return a; }
    if (strcmp(op, "div") == 0)  { if (!rv) return 0; long q = lv / rv; if ((lv % rv != 0) && ((lv < 0) != (rv < 0))) q--; return q; }
    if (strcmp(op, "max") == 0)  return lv > rv ? lv : rv;
    if (strcmp(op, "min") == 0)  return lv < rv ? lv : rv;
    if (strcmp(op, "//") == 0)   return rv ? lv / rv : 0;
    if (op[0] == '+' && op[1] == '\0') return lv + rv;
    if (op[0] == '-' && op[1] == '\0') return lv - rv;
    if (op[0] == '*' && op[1] == '\0') return lv * rv;
    if (op[0] == '/' && op[1] == '\0') return rv ? lv / rv : 0;
    if (op[0] == '%' && op[1] == '\0') return rv ? lv % rv : 0;
    return lv + rv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int     subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
extern int     subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val);
static DESCR_t _rt_nv_fold_get(const char *raw)
{
    if (!raw || !*raw) return NULVCL;
    char *n = GC_strdup(raw);
    return NV_GET_fn(n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _rt_nv_fold_set(const char *raw, DESCR_t val)
{
    if (!raw || !*raw) return;
    char *n = GC_strdup(raw);
    NV_SET_fn(n, val);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_call(const char *name, int nargs)
{
    DESCR_t args[32];
    if (nargs > 32) nargs = 32;
    for (int k = nargs - 1; k >= 0; k--) args[k] = vstack_pop();
    if (name && strcmp(name, "INDIR_GET") == 0) {
        DESCR_t name_d = args[0];
        DESCR_t val;
        if (IS_NAMEPTR(name_d))      val = NAME_DEREF_PTR(name_d);
        else if (IS_NAMEVAL(name_d)) val = _rt_nv_fold_get(name_d.s);
        else                         val = _rt_nv_fold_get(VARVAL_fn(name_d));
        vstack_push(val);
        LAST_OK_SET(1);
        return;
    }
    if (name && strcmp(name, "NAME_PUSH") == 0) {
        DESCR_t name_d = args[0];
        const char *vname0 = VARVAL_fn(name_d);
        char *vname = GC_strdup(vname0 ? vname0 : "");
        vstack_push(NAMEVAL(vname));
        LAST_OK_SET(1);
        return;
    }
    if (name && strncmp(name, "NRETURN_ASGN_", 13) == 0) {
        const char *fname = name + 13;
        DESCR_t rhs = args[0];
        void *cfn = chunk_reg_lookup(fname);
        DESCR_t fres = cfn ? call_native_chunk(fname, cfn, NULL, 0) : FAILDESCR;
        int ok = 0;
        if (!IS_FAIL_fn(fres)) {
            if (IS_NAMEPTR(fres))      { NAME_DEREF_PTR(fres) = rhs; ok = 1; }
            else if (IS_NAMEVAL(fres)) {
                char *fn = GC_strdup(fres.s);
                NV_SET_fn(fn, rhs); ok = 1;
            }
            else {
                char setname[256];
                snprintf(setname, sizeof(setname), "%s_SET", fname ? fname : "");
                vstack_push(rhs); vstack_push(fres);
                rt_call(setname, 2);
                ok = LAST_OK_GET();
            }
        }
        vstack_push(rhs);
        LAST_OK_SET(ok);
        return;
    }
    if (name && strcmp(name, "ASGN_INDIR") == 0) {
        DESCR_t name_d = args[1];
        DESCR_t val    = args[0];
        int ok = 0;
        if (IS_NAMEPTR(name_d)) { *(DESCR_t*)name_d.ptr = val; ok = 1; }
        else if (IS_NAMEVAL(name_d)) { _rt_nv_fold_set(name_d.s, val); ok = 1; }
        else {
            const char *vname0 = VARVAL_fn(name_d);
            if (vname0 && *vname0) { _rt_nv_fold_set(vname0, val); ok = 1; }
        }
        vstack_push(val);
        LAST_OK_SET(ok);
        return;
    }
    if (name && strcmp(name, "IDX") == 0) {
        if (nargs == 2) {
            DESCR_t r = subscript_get(args[0], args[1]);
            vstack_push(r);
            LAST_OK_SET((r.v != DT_FAIL));
        } else if (nargs == 3) {
            DESCR_t r = subscript_get2(args[0], args[1], args[2]);
            vstack_push(r);
            LAST_OK_SET((r.v != DT_FAIL));
        } else {
            DESCR_t r = INVOKE_fn("ITEM", args, nargs);
            vstack_push(r);
            LAST_OK_SET((r.v != DT_FAIL));
        }
        return;
    }
    if (name && strcmp(name, "IDX_SET") == 0) {
        if (nargs == 3) {
            DESCR_t val = args[0]; DESCR_t base = args[1]; DESCR_t idx = args[2];
            LAST_OK_SET(subscript_set(base, idx, val));
            vstack_push(val);
        } else if (nargs >= 4) {
            DESCR_t val = args[0]; DESCR_t base = args[1];
            DESCR_t i = args[2]; DESCR_t j = args[3];
            LAST_OK_SET(subscript_set2(base, i, j, val));
            vstack_push(val);
        } else {
            DESCR_t r = INVOKE_fn("ITEM_SET", args, nargs);
            LAST_OK_SET((r.v != DT_FAIL));
            vstack_push(args[0]);
        }
        return;
    }
    if (name && strcmp(name, "ITEM_SET") == 0) {
        if (nargs == 3) {
            DESCR_t val = args[0]; DESCR_t base = args[1]; DESCR_t idx = args[2];
            LAST_OK_SET(subscript_set(base, idx, val));
            vstack_push(val);
        } else if (nargs >= 4) {
            DESCR_t val = args[0]; DESCR_t base = args[1];
            DESCR_t i = args[2]; DESCR_t j = args[3];
            LAST_OK_SET(subscript_set2(base, i, j, val));
            vstack_push(val);
        } else {
            DESCR_t r = INVOKE_fn("ITEM_SET", args, nargs);
            LAST_OK_SET((r.v != DT_FAIL));
            vstack_push(args[0]);
        }
        return;
    }
    for (int k = 0; k < nargs; k++) {
        if (args[k].v == DT_FAIL) {
            vstack_push(FAILDESCR);
            LAST_OK_SET(0);
            return;
        }
    }
    void *cfn = chunk_reg_lookup(name ? name : "");
    if (!cfn && name) {
        const char *entry = FUNC_ENTRY_fn(name);
        if (entry && strcmp(entry, name) != 0)
            cfn = chunk_reg_lookup(entry);
    }
    if (cfn) {
        strncpy(kw_rtntype, "RETURN", sizeof(kw_rtntype)-1);
        DESCR_t result = call_native_chunk(name, cfn, args, nargs);
        if (strcmp(kw_rtntype, "FRETURN") == 0) {
            vstack_push(FAILDESCR);
            LAST_OK_SET(0);
        } else if (strcmp(kw_rtntype, "NRETURN") == 0) {
            DESCR_t deref = result;
            if (IS_NAMEPTR(deref))      deref = NAME_DEREF_PTR(deref);
            else if (IS_NAMEVAL(deref)) deref = NV_GET_fn(deref.s);
            vstack_push(deref);
            LAST_OK_SET(1);
        } else {
            vstack_push(result);
            LAST_OK_SET((result.v != DT_FAIL));
        }
        return;
    }
    if (name && strcmp(name, "SIZE") == 0 && nargs == 1 && args[0].v == DT_A) {
        DESCR_t r = INTVAL(args[0].arr ? (args[0].arr->hi - args[0].arr->lo + 1) : 0);
        vstack_push(r); LAST_OK_SET(1); return;
    }
    if (name) {
        extern int g_lang;
        if (g_lang == LANG_RAKU) {
            if (nargs >= 1 && args[0].v == DT_S && args[0].s
                    && (strcmp(name, "push") == 0
                     || strcmp(name, "pop")  == 0
                     || strcmp(name, "arr_set") == 0
                     || strcmp(name, "hash_set") == 0
                     || strcmp(name, "hash_delete") == 0)) {
                extern int script_try_mutating_builtin_by_name(
                    const char *fn, const char *vname,
                    DESCR_t *args, int nargs, DESCR_t *out);
                extern int script_try_hash_mutating_builtin(
                    const char *fn, const char *vname,
                    DESCR_t *args, int nargs, DESCR_t *out);
                DESCR_t mu_out;
                if (script_try_hash_mutating_builtin(name, args[0].s,
                        &args[1], nargs - 1, &mu_out)
                 || script_try_mutating_builtin_by_name(name, args[0].s,
                        &args[1], nargs - 1, &mu_out)) {
                    if (IS_NAMEPTR(mu_out))      mu_out = NAME_DEREF_PTR(mu_out);
                    else if (IS_NAMEVAL(mu_out)) mu_out = NV_GET_fn(mu_out.s);
                    vstack_push(mu_out);
                    LAST_OK_SET((mu_out.v != DT_FAIL));
                    return;
                }
            }
            extern int try_call_builtin_by_name(
                const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
            DESCR_t rk_out;
            if (try_call_builtin_by_name(name, args, nargs, &rk_out)) {
                if (IS_NAMEPTR(rk_out))      rk_out = NAME_DEREF_PTR(rk_out);
                else if (IS_NAMEVAL(rk_out)) rk_out = NV_GET_fn(rk_out.s);
                vstack_push(rk_out);
                LAST_OK_SET((rk_out.v != DT_FAIL));
                return;
            }
        }
        extern int core_fn_registered(const char *);
        if (!core_fn_registered(name)) {
        extern int try_call_builtin_by_name(
            const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
        DESCR_t gen_out;
        if (try_call_builtin_by_name(name, args, nargs, &gen_out)) {
            if (IS_NAMEPTR(gen_out))      gen_out = NAME_DEREF_PTR(gen_out);
            else if (IS_NAMEVAL(gen_out)) gen_out = NV_GET_fn(gen_out.s);
            vstack_push(gen_out);
            LAST_OK_SET((gen_out.v != DT_FAIL));
            return;
        }
        }
    }
    DESCR_t result = INVOKE_fn(name ? name : "", args, nargs);
    if (strcmp(kw_rtntype, "NRETURN") == 0) {
        if (IS_NAMEPTR(result))      result = NAME_DEREF_PTR(result);
        else if (IS_NAMEVAL(result)) result = NV_GET_fn(result.s);
    }
    vstack_push(result);
    LAST_OK_SET((result.v != DT_FAIL));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_do_return(int kind, int cond)
{
    if (cond == 1 && !LAST_OK_GET()) return 0;
    if (cond == 2 &&  LAST_OK_GET()) return 0;
    if (kind == 1) {
        if (g_ops->depth() > 0) (void)vstack_pop();
        vstack_push(FAILDESCR);
        LAST_OK_SET(0);
        strncpy(kw_rtntype, "FRETURN", sizeof(kw_rtntype)-1);
    } else if (kind == 2) {
        DESCR_t v = (g_ops->depth() > 0) ? vstack_pop() : FAILDESCR;
        if (v.v == DT_N) {
            vstack_push(v);
        } else if (v.v == DT_S && v.s) {
            char *n = GC_strdup(v.s);
            vstack_push(NAMEVAL(n));
        } else {
            vstack_push(FAILDESCR);
        }
        LAST_OK_SET(1);
        strncpy(kw_rtntype, "NRETURN", sizeof(kw_rtntype)-1);
    } else {
        int ok = 0;
        if (g_ops->depth() > 0) ok = (vstack_peek().v != DT_FAIL);
        LAST_OK_SET(ok);
        strncpy(kw_rtntype, "RETURN",  sizeof(kw_rtntype)-1);
    }
    return g_native_chunk_depth > 0 ? 2 : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_do_nreturn(const char *fname, int cond)
{
    if (cond == 1 && !LAST_OK_GET()) return 0;
    if (cond == 2 &&  LAST_OK_GET()) return 0;
    LAST_OK_SET(fname ? 1 : 0);
    strncpy(kw_rtntype, "NRETURN", sizeof(kw_rtntype)-1);
    return g_native_chunk_depth > 0 ? 2 : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "SM.h"
__attribute__((weak)) DESCR_t sm_eval_subexpr(int entry_pc)
{
    fprintf(stderr,
        "libscrip_rt: sm_eval_subexpr(%d) called — DT_E EVAL dispatch "
        "not yet wired in EM-6.  Add to EM-10 scope.\n", entry_pc);
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((weak)) const char *sm_opcode_name(SM_op_t op)
{
    (void)op;
    return "?";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int _is_pat_fnc_name(const char *s);
int _expr_is_pat(tree_t *e);
__attribute__((weak)) int _is_pat_fnc_name(const char *s)  { (void)s; return 0; }
__attribute__((weak)) int _expr_is_pat(tree_t *e)          { (void)e; return 0; }
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gc/gc.h>
void rt_gc_init(void)
{
    GC_INIT();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_set_lang(int lang)
{
    extern int g_lang;
    g_lang = lang;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *chars; int delta; } rt_cs_t;
void *rt_cs_new(const char *chars)
{
    rt_cs_t *z = GC_MALLOC(sizeof(rt_cs_t));
    z->chars = chars ? chars : "";
    z->delta = 0;
    return z;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void flush_pending_captures(void);
extern void reset_capture_registry(void);
extern void clear_pending_flags(void);
static void rt_register_cap(cap_t *c);
#define RT_MAX_CAPTURES 256
static cap_t *g_rt_cap_list[RT_MAX_CAPTURES];
static int    g_rt_cap_count = 0;
static void rt_register_cap(cap_t *c)
{
    for (int i = 0; i < g_rt_cap_count; i++)
        if (g_rt_cap_list[i] == c) return;
    if (g_rt_cap_count < RT_MAX_CAPTURES)
        g_rt_cap_list[g_rt_cap_count++] = c;
}
