#include "rt.h"
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
#define STACKLESS_ABORT(fn) \
    do { fprintf(stderr, "libscrip_rt: %s called — Icon value stack removed (GROUND ZERO 3). " \
                         "This box must be rebuilt stackless (per-box slot, no value stack).\n", (fn)); \
         abort(); } while (0)
#include <string.h>
#include <math.h>
extern void    core_lib_init(void);
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
static int     g_native_chunk_depth = 0;
int rt_in_native_chunk(void) { return g_native_chunk_depth > 0; }
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_vstack_depth(void) { STACKLESS_ABORT("rt_vstack_depth"); return 0; }
DESCR_t rt_vstack_pop(void) { STACKLESS_ABORT("rt_vstack_pop"); return FAILDESCR; }
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _rt_IDENT(DESCR_t *a, int n)
{
    if (n == 1) return IS_NULL_fn(a[0]) ? NULVCL : FAILDESCR;
    const char *s1 = VARVAL_fn(a[0]); if (!s1) s1 = "";
    const char *s2 = VARVAL_fn(a[1]); if (!s2) s2 = "";
    return (strcmp(s1, s2) == 0) ? NULVCL : FAILDESCR;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _rt_DIFFER(DESCR_t *a, int n)
{
    if (n == 1) return IS_NULL_fn(a[0]) ? FAILDESCR : NULVCL;
    const char *s1 = VARVAL_fn(a[0]); if (!s1) s1 = "";
    const char *s2 = VARVAL_fn(a[1]); if (!s2) s2 = "";
    return (strcmp(s1, s2) != 0) ? NULVCL : FAILDESCR;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
#include "IR.h"
#include "../builtins/resolve_runtime.h"
extern cap_t *bb_cap_new(bb_box_fn child_fn, void *child_state, const char *varname, DESCR_t *var_ptr, int immediate);
extern cap_t *bb_cap_new_call(bb_box_fn child_fn, void *child_state, const char *fnc_name, DESCR_t *fnc_args, int fnc_nargs, char **fnc_arg_names, int fnc_n_arg_names, int immediate);
void rt_init_cap(void **slot_ptr, void *child_fn, const char *varname, int immediate)
{
    if (!slot_ptr || !child_fn) return;
    cap_t *c = bb_cap_new((bb_box_fn)child_fn, NULL, varname, NULL, immediate);
    *slot_ptr = c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_init_cap_call(void **slot_ptr, void *child_fn, const char *fnc_name)
{
    if (!slot_ptr || !child_fn) return;
    cap_t *c = bb_cap_new_call((bb_box_fn)child_fn, NULL, fnc_name, NULL, 0, NULL, 0, 0);
    *slot_ptr = c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern void *bb_arbno_new(void *fn, void *state);
void rt_init_arbno(void **slot_ptr, void *child_fn)
{
    if (!slot_ptr || !child_fn) return;
    *slot_ptr = bb_arbno_new(child_fn, NULL);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void *chunk_reg_lookup(const char *name)
{
    if (!name || !*name) return NULL;
    for (int i = 0; i < g_expression_reg_count; i++) {
        if (strcmp(g_expression_reg[i].name, name) == 0)
            return g_expression_reg[i].fn;
    }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
    typedef void (*chunk_fn_t)(void);
    chunk_fn_t cfn = (chunk_fn_t)fn;
    g_native_chunk_depth++;
    cfn();
    g_native_chunk_depth--;
    DESCR_t result = NV_GET_fn(retname);
    for (int k = nl - 1; k >= 0; k--)
        NV_SET_fn(lnames[k], saved_l[k]);
    for (int k = nbound - 1; k >= 0; k--)
        NV_SET_fn(pnames[k], saved_p[k]);
    NV_SET_fn(retname, saved_ret);
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_init(int argc, char **argv)
{
    (void)argc; (void)argv;
    setvbuf(stdout, NULL, _IOLBF, 0);
    bb_pool_init();
    core_lib_init();
    trail_init(&g_resolve_trail);
    prolog_atom_init();
    g_bb_mode = BB_MODE_BROKERED;
    register_fn("IDENT",  _rt_IDENT,  1, 2);
    register_fn("DIFFER", _rt_DIFFER, 1, 2);
    g_user_call_hook = _rt_usercall;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_int(int64_t v)
{
    (void)v;
    STACKLESS_ABORT("rt_push_int");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_halt_tos(void)
{
    STACKLESS_ABORT("rt_halt_tos");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_str(const char *s, uint32_t slen)
{
    (void)s;
    (void)slen;
    STACKLESS_ABORT("rt_push_str");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_str_nl(const char *s, uint32_t slen)
{
    if (s && slen) fwrite(s, 1, slen, stdout);
    fputc('\n', stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_int_nl(int64_t v)
{
    fprintf(stdout, "%lld\n", (long long)v);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_strz_nl(const char *s)
{
    if (s) fwrite(s, 1, strlen(s), stdout);
    fputc('\n', stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void *rt_frame(void)
{
    static int64_t g_frame_buf[4096];
    return (void *)g_frame_buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_write_int_nl(void)
{
    STACKLESS_ABORT("rt_pop_write_int_nl");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_write_any_nl(void)
{
    STACKLESS_ABORT("rt_pop_write_any_nl");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_nv_set(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_pop_nv_set");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_nv_get(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_nv_get");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_nv_set(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_nv_set");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_str(const char *name, const char *str)
{
    DESCR_t d;
    d.v    = DT_S;
    d.s    = (char *)(str ? str : "");
    d.slen = (uint32_t)strlen(d.s);
    NV_SET_fn(name ? name : "", d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_int(const char *name, int64_t val)
{
    DESCR_t d;
    d.v    = DT_I;
    d.slen = 0;
    d.i    = val;
    NV_SET_fn(name ? name : "", d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_var(const char *dst, const char *src)
{
    NV_SET_fn(dst ? dst : "", NV_GET_fn(src ? src : ""));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern DESCR_t VARVAL_d_fn(DESCR_t d);
typedef struct { const char *base; long len; } rt_subj_t;
const char *g_subject_dbg_base = 0;
long        g_subject_dbg_len  = -1;
rt_subj_t rt_subject_load(const char *name, const char *lit)
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
    g_subject_dbg_base = r.base;
    g_subject_dbg_len  = r.len;
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_match_lit(const char *subj, long subj_len, const char *lit, long lit_len, int anchored, long *m_start, long *m_end)
{
    if (!subj) subj = "";
    if (!lit)  lit  = "";
    if (lit_len < 0) lit_len = (long)strlen(lit);
    long max_start = subj_len - lit_len;
    if (max_start < 0) return 0;
    long last_start = anchored ? 0 : max_start;
    for (long start = 0; start <= last_start; start++) {
        if (lit_len == 0 || memcmp(subj + start, lit, (size_t)lit_len) == 0) {
            if (m_start) *m_start = start;
            if (m_end)   *m_end   = start + lit_len;
            return 1;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_void(void)
{
    STACKLESS_ABORT("rt_pop_void");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_store_i64(int64_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_pop_store_i64");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_stored_i64(const int64_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_push_stored_i64");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pop_store_descr(DESCR_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_pop_store_descr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_case_eq(const DESCR_t *slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_case_eq");
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define RT_FRAME_STACK_MAX 256
#define RT_FRAME_SLOT_MAX  64
typedef struct { DESCR_t slot[RT_FRAME_SLOT_MAX]; int nslots; } rt_frame_t;
static rt_frame_t g_rt_frames[RT_FRAME_STACK_MAX];
static int        g_rt_frame_depth = 0;
void rt_frame_enter(int nparams)
{
    (void)nparams;
    STACKLESS_ABORT("rt_frame_enter");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_frame_leave(void)
{
    if (g_rt_frame_depth > 0) g_rt_frame_depth--;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define RT_PROC_MAX 512
#define PROC_FRAME_QWORDS 512
#define PROC_FRAME_DEPTH  4096
#define CALL_ARGS_MAX     64
typedef struct { const char *name; bb_box_fn fn; void *entry; const char **pnames; int nparams; } rt_proc_t;
static rt_proc_t g_rt_gen_procs[RT_PROC_MAX];
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
    if (g_rt_gen_proc_count >= RT_PROC_MAX) return;
    rt_proc_t *p = &g_rt_gen_procs[g_rt_gen_proc_count++];
    p->name = name; p->fn = NULL; p->entry = entry; p->pnames = pnames; p->nparams = nparams;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_proc_reset(void) { g_rt_gen_proc_count = 0; }
int rt_proc_is_registered(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return 1;
    return 0;
}
void rt_proc_set_fn(const char *name, bb_box_fn fn)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_rt_gen_procs[i].fn = fn; return; }
    if (g_rt_gen_proc_count >= RT_PROC_MAX) return;
    rt_proc_t *p = &g_rt_gen_procs[g_rt_gen_proc_count++];
    p->name = name; p->fn = fn; p->entry = NULL; p->pnames = NULL; p->nparams = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_call_proc(const char *name, int nargs)
{
    (void)name;
    (void)nargs;
    STACKLESS_ABORT("rt_call_proc");
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t g_call_args[CALL_ARGS_MAX];
void rt_arg_stage(int idx, DESCR_t v)
{
    if (idx >= 0 && idx < CALL_ARGS_MAX) g_call_args[idx] = v;
}
static int64_t g_proc_arena[PROC_FRAME_DEPTH * PROC_FRAME_QWORDS];
static int     g_proc_depth = 0;
DESCR_t rt_call_proc_descr(const char *name, int nargs)
{
    rt_proc_t *p = (rt_proc_t *)0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { p = &g_rt_gen_procs[i]; break; }
    if (!p || !p->fn) {
        fprintf(stderr, "[GZ-10] rt_call_proc_descr: procedure '%s' has no stackless slab\n", name ? name : "(null)");
        abort();
    }
    if (g_proc_depth >= PROC_FRAME_DEPTH) {
        fprintf(stderr, "[GZ-10] rt_call_proc_descr: recursion depth exceeded (%d)\n", PROC_FRAME_DEPTH);
        abort();
    }
    char *fb = (char *)&g_proc_arena[g_proc_depth * PROC_FRAME_QWORDS];
    g_proc_depth++;
    *(DESCR_t *)(fb + 0) = NULVCL;
    if (nargs > CALL_ARGS_MAX) nargs = CALL_ARGS_MAX;
    for (int i = 0; i < nargs; i++) *(DESCR_t *)(fb + 16 * (i + 1)) = g_call_args[i];
    (void)p->fn((void *)fb, 0);
    DESCR_t result = *(DESCR_t *)(fb + 0);
    g_proc_depth--;
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int builtin_is_generator(const char *name)
{
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any")
        || !strcmp(name, "many") || !strcmp(name, "bal") || !strcmp(name, "key")
        || !strcmp(name, "seq");
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_call_builtin(const char *name, int nargs)
{
    (void)name;
    (void)nargs;
    STACKLESS_ABORT("rt_call_builtin");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_load_frame(int slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_load_frame");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_store_frame(int slot)
{
    (void)slot;
    STACKLESS_ABORT("rt_store_frame");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int g_last_ok = 0;
int rt_last_ok(void)
{
    return g_last_ok;
}
void rt_set_last_ok(int ok)
{
    g_last_ok = ok ? 1 : 0;
}
void rt_push_expression_descr(int64_t entry_pc, int64_t arity)
{
    (void)entry_pc;
    (void)arity;
    STACKLESS_ABORT("rt_push_expression_descr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern void rt_set_last_ok(int v);
void rt_exec_stmt_pat(void *blob_α, const char *subj_name, int has_repl)
{
    (void)blob_α;
    (void)subj_name;
    (void)has_repl;
    STACKLESS_ABORT("rt_exec_stmt_pat");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_match_blob(void *blob_α,
                         const char *subj_name,
                         int has_repl)
{
    (void)blob_α;
    (void)subj_name;
    (void)has_repl;
    STACKLESS_ABORT("rt_match_blob");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_lit(const char *s)
{
    (void)s;
    STACKLESS_ABORT("rt_pat_lit");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_refname(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_pat_refname");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_span(void)
{
    STACKLESS_ABORT("rt_pat_span");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_break(void)
{
    STACKLESS_ABORT("rt_pat_break");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_breakx(void)
{
    STACKLESS_ABORT("rt_pat_breakx");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_any(void)
{
    STACKLESS_ABORT("rt_pat_any");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_notany(void)
{
    STACKLESS_ABORT("rt_pat_notany");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_len(void)
{
    STACKLESS_ABORT("rt_pat_len");
}
void rt_pat_pos(void)
{
    STACKLESS_ABORT("rt_pat_pos");
}
void rt_pat_rpos(void)
{
    STACKLESS_ABORT("rt_pat_rpos");
}
void rt_pat_tab(void)
{
    STACKLESS_ABORT("rt_pat_tab");
}
void rt_pat_rtab(void)
{
    STACKLESS_ABORT("rt_pat_rtab");
}
void rt_pat_arb(void)
{
    STACKLESS_ABORT("rt_pat_arb");
}
void rt_pat_rem(void)
{
    STACKLESS_ABORT("rt_pat_rem");
}
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_at_cursor(const char *varname, int cur_delta)
{
    if (!varname || !*varname) return;
    DESCR_t pos = { .v = DT_I, .i = (int64_t)cur_delta };
    NV_SET_fn(varname, pos);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_fence(void)
{
    STACKLESS_ABORT("rt_pat_fence");
}
void rt_pat_fail(void)
{
    STACKLESS_ABORT("rt_pat_fail");
}
void rt_pat_abort(void)
{
    STACKLESS_ABORT("rt_pat_abort");
}
void rt_pat_succeed(void)
{
    STACKLESS_ABORT("rt_pat_succeed");
}
void rt_pat_bal(void)
{
    STACKLESS_ABORT("rt_pat_bal");
}
void rt_pat_eps(void)
{
    STACKLESS_ABORT("rt_pat_eps");
}
void rt_pat_arbno(void)
{
    STACKLESS_ABORT("rt_pat_arbno");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_fence1(void)
{
    STACKLESS_ABORT("rt_pat_fence1");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_cat(void)
{
    STACKLESS_ABORT("rt_pat_cat");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_alt(void)
{
    STACKLESS_ABORT("rt_pat_alt");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_deref(void)
{
    STACKLESS_ABORT("rt_pat_deref");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture(const char *varname, int kind)
{
    (void)varname;
    (void)kind;
    STACKLESS_ABORT("rt_pat_capture");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture_fn(const char *fname, int is_imm, const char *namelist)
{
    (void)fname;
    (void)is_imm;
    (void)namelist;
    STACKLESS_ABORT("rt_pat_capture_fn");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture_fn_args(const char *fname, int is_imm, int nargs)
{
    (void)fname;
    (void)is_imm;
    (void)nargs;
    STACKLESS_ABORT("rt_pat_capture_fn_args");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_usercall(const char *fname)
{
    (void)fname;
    STACKLESS_ABORT("rt_pat_usercall");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_usercall_args(const char *fname, int nargs)
{
    (void)fname;
    (void)nargs;
    STACKLESS_ABORT("rt_pat_usercall_args");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_match_variant(const char *subj_name, int has_repl)
{
    (void)subj_name;
    (void)has_repl;
    STACKLESS_ABORT("rt_match_variant");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_concat(void)
{
    STACKLESS_ABORT("rt_concat");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern DESCR_t binop_apply(int op, DESCR_t lv, DESCR_t rv, int *rel_fail);
void rt_gen_concat(void)
{
    STACKLESS_ABORT("rt_gen_concat");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_set_stno(int64_t stno)
{
    extern void comm_stno(int n);
    kw_stno = stno;
    comm_stno((int)stno);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_null(void)
{
    STACKLESS_ABORT("rt_push_null");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_coerce_num(void)
{
    STACKLESS_ABORT("rt_coerce_num");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_real_bits(uint64_t bits)
{
    (void)bits;
    STACKLESS_ABORT("rt_push_real_bits");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_null_noflip(void)
{
    STACKLESS_ABORT("rt_push_null_noflip");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_push_expr(void *ptr)
{
    (void)ptr;
    STACKLESS_ABORT("rt_push_expr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_exp(void)
{
    STACKLESS_ABORT("rt_exp");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_neg(void)
{
    STACKLESS_ABORT("rt_neg");
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_neg(void)
{
    STACKLESS_ABORT("rt_unop_neg");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_pos(void)
{
    STACKLESS_ABORT("rt_unop_pos");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_nonnull(void)
{
    STACKLESS_ABORT("rt_unop_nonnull");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_null_test(void)
{
    STACKLESS_ABORT("rt_unop_null_test");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_not(void)
{
    STACKLESS_ABORT("rt_unop_not");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_unop_size(void)
{
    STACKLESS_ABORT("rt_unop_size");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_size_d(uint64_t lo, uint64_t hi)
{
    DESCR_t v;
    v.v    = (DTYPE_t)(uint32_t)(lo & 0xFFFFFFFFu);
    v.slen = (uint32_t)(lo >> 32);
    v.i    = (int64_t)hi;
    if (IS_FAIL_fn(v)) return FAILDESCR;
    if (v.v == DT_SNUL) { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = 0; return r; }
    if (v.v == DT_S) {
        size_t n = v.slen ? (size_t)v.slen : (v.s ? strlen(v.s) : 0);
        DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)n; return r;
    }
    if (v.v == DT_DATA && v.u) {
        if (v.u->type) { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)v.u->type->nfields; return r; }
        { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = 0; return r; }
    }
    { const char *s = VARVAL_fn(v); long n = s ? (long)strlen(s) : 0; DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)n; return r; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_field_get(const char *fname)
{
    (void)fname;
    STACKLESS_ABORT("rt_field_get");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_field_set(const char *fname)
{
    (void)fname;
    STACKLESS_ABORT("rt_field_set");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_idx_get(void)
{
    STACKLESS_ABORT("rt_idx_get");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_idx_set(void)
{
    STACKLESS_ABORT("rt_idx_set");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_list_bang(DESCR_t *obj_slot, int64_t *idx_slot, int *state_slot, int reset)
{
    (void)obj_slot;
    (void)idx_slot;
    (void)state_slot;
    (void)reset;
    STACKLESS_ABORT("rt_list_bang");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_limit_begin(DESCR_t *max_slot, int64_t *count_slot)
{
    (void)max_slot;
    (void)count_slot;
    STACKLESS_ABORT("rt_limit_begin");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_limit_more(DESCR_t *max_slot, int64_t *count_slot)
{
    (void)max_slot;
    (void)count_slot;
    STACKLESS_ABORT("rt_limit_more");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_limit_inc(int64_t *count_slot)
{
    (void)count_slot;
    STACKLESS_ABORT("rt_limit_inc");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_incr(int64_t n)
{
    (void)n;
    STACKLESS_ABORT("rt_incr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_decr(int64_t n)
{
    (void)n;
    STACKLESS_ABORT("rt_decr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_acomp(int op)
{
    (void)op;
    STACKLESS_ABORT("rt_acomp");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_lcomp(int op)
{
    (void)op;
    STACKLESS_ABORT("rt_lcomp");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_define_entry(void)
{
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_define(void)
{
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_unhandled_sm(int op)
{
    fprintf(stderr, "libscrip_rt: unhandled SM opcode %d in emitted binary (M5 territory)\n", op);
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_atom(const char *s)
{
    if (s) fputs(s, stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void rt_format_float(char *buf, size_t bufsz, double d)
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_int(long v)
{
    printf("%ld", v);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_float(double v)
{
    char fb[64]; rt_format_float(fb, sizeof fb, v); fputs(fb, stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_cstr(const char *s)
{
    if (s) fputs(s, stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_cut_set(void)
{
    extern int g_resolve_cut_flag;
    g_resolve_cut_flag = 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_var(int slot)
{
    extern Term **g_resolve_env;
    if (!g_resolve_env || slot < 0) { fputs("_", stdout); return; }
    Term *t = g_resolve_env[slot] ? term_deref(g_resolve_env[slot]) : NULL;
    if (!t) { fputs("_", stdout); return; }
    if (t->tag == TERM_INT)   { printf("%ld", (long)t->ival); return; }
    if (t->tag == TERM_FLOAT) { char fb[64]; rt_format_float(fb, sizeof fb, t->fval); fputs(fb, stdout); return; }
    if (t->tag == TERM_ATOM)  { const char *nm = prolog_atom_name(t->atom_id);
                                 if (nm) fputs(nm, stdout); return; }
    if (t->tag == TERM_COMPOUND) { extern void pl_write(Term *); pl_write(t); return; }
    fputs("_", stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_term_ptr(void *t)
{
    extern void pl_write(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_write(d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_writeq_term_ptr(void *t)
{
    extern void pl_writeq(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_writeq(d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_write_canonical_term_ptr(void *t)
{
    extern void pl_write_canonical(Term *);
    Term *d = t ? term_deref((Term *)t) : NULL;
    if (!d) { fputs("_", stdout); return; }
    pl_write_canonical(d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
Term **resolve_bb_env_install(Term **env)
{
    extern Term **g_resolve_env;
    Term **prev = g_resolve_env;
    g_resolve_env = env;
    return prev;
}
/*--------------------------------------------------------------------------------------------------------------------*/
long rt_arith(int lk, long li, const char *ls,
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern int     subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
extern int     subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val);
static DESCR_t _rt_nv_fold_get(const char *raw)
{
    if (!raw || !*raw) return NULVCL;
    char *n = GC_strdup(raw);
    return NV_GET_fn(n);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void _rt_nv_fold_set(const char *raw, DESCR_t val)
{
    if (!raw || !*raw) return;
    char *n = GC_strdup(raw);
    NV_SET_fn(n, val);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_call(const char *name, int nargs)
{
    (void)name;
    (void)nargs;
    STACKLESS_ABORT("rt_call");
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_do_return(int kind, int cond)
{
    (void)kind;
    (void)cond;
    STACKLESS_ABORT("rt_do_return");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_do_nreturn(const char *fname, int cond)
{
    (void)fname;
    (void)cond;
    STACKLESS_ABORT("rt_do_nreturn");
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#include "SM.h"
__attribute__((weak)) DESCR_t sm_eval_subexpr(int entry_pc)
{
    fprintf(stderr,
        "libscrip_rt: sm_eval_subexpr(%d) called — DT_E EVAL dispatch "
        "not yet wired in EM-6.  Add to EM-10 scope.\n", entry_pc);
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
__attribute__((weak)) const char *sm_opcode_name(SM_op_t op)
{
    (void)op;
    return "?";
}
/*--------------------------------------------------------------------------------------------------------------------*/
int _is_pat_fnc_name(const char *s);
int _expr_is_pat(tree_t *e);
__attribute__((weak)) int _is_pat_fnc_name(const char *s)  { (void)s; return 0; }
__attribute__((weak)) int _expr_is_pat(tree_t *e)          { (void)e; return 0; }
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gc/gc.h>
typedef struct { const char *chars; int delta; } rt_cs_t;
void *rt_cs_new(const char *chars)
{
    rt_cs_t *z = GC_MALLOC(sizeof(rt_cs_t));
    z->chars = chars ? chars : "";
    z->delta = 0;
    return z;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
