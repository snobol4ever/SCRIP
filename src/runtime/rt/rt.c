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
#include <gc.h>
/* Subject capture base (pattern_match.c). A nested user-proc call whose body
   runs its own SUBJ?PAT clobbers these globals; save/restore across the callee
   so the caller's pending capture/scan keeps its own subject base. Mirrors the
   save_Σ discipline in runtime_eval.c. */
extern const char *Σ;
extern int Σlen;
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
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_case_eq(const DESCR_t *sel, const DESCR_t *key)
{
    if (!sel || !key) return 0;
    if (sel->v == DT_I && key->v == DT_I) return sel->i == key->i;
    { const char *ss = VARVAL_fn(*sel); const char *ks = VARVAL_fn(*key); if (ss && ks) return strcmp(ss, ks) == 0; return ss == ks; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define EXPRESSION_REG_MAX 256
typedef struct { const char *name; void *fn; } ExpressionRegEntry;
static ExpressionRegEntry g_expression_reg[EXPRESSION_REG_MAX];
static int           g_expression_reg_count = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
#include "IR.h"
#include "../builtins/resolution.h"
extern cap_t *bb_cap_new(bb_box_fn child_fn, void *child_state, const char *varname, DESCR_t *var_ptr, int immediate);
extern cap_t *bb_cap_new_call(bb_box_fn child_fn, void *child_state, const char *fnc_name, DESCR_t *fnc_args, int fnc_nargs, char **fnc_arg_names, int fnc_n_arg_names, int immediate);
/*--------------------------------------------------------------------------------------------------------------------*/
extern void *bb_arbno_new(void *fn, void *state);
/*--------------------------------------------------------------------------------------------------------------------*/
void *rt_frame(void)
{
    static int64_t g_frame_buf[8192];
    return (void *)g_frame_buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
const char *rt_nv_cstr(const char *name)
{
    const char *s = VARVAL_fn(NV_GET_fn(name ? name : ""));
    return s ? s : "";
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_str(const char *name, const char *str)
{
    DESCR_t d;
    d.v    = DT_S;
    d.s    = (char *)(str ? str : "");
    d.slen = (uint32_t)strlen(d.s);
    NV_SET_fn(name ? name : "", d);
    if (g_monitor_bin) mon_emit_value_bin(name ? name : "", d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_indirect_assign_str(const char *holder, const char *str)
{
    rt_gvar_assign_str(rt_nv_cstr(holder ? holder : ""), str);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_indirect_assign_var(const char *holder, const char *val_name)
{
    const char *target = rt_nv_cstr(holder ? holder : "");
    DESCR_t val = NV_GET_fn(val_name ? val_name : "");
    NV_SET_fn(target ? target : "", val);
    if (g_monitor_bin) mon_emit_value_bin(target ? target : "", val);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_pat(const char *name, void *head)
{
    DESCR_t d;
    d.v    = DT_P;
    d.slen = 0;
    d.p    = head;
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
    if (g_monitor_bin) mon_emit_value_bin(name ? name : "", d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern DESCR_t binop_apply(int op, DESCR_t lv, DESCR_t rv, int *rel_fail);
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
int64_t rt_gvar_get_int(const char *name)
{
    DESCR_t v = NV_GET_fn(name ? name : "");
    if (v.v == DT_I) return v.i;
    if (v.v == DT_R) return (int64_t)v.r;
    if (v.v == DT_S && v.s) return (int64_t)strtoll(v.s, NULL, 10);
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_gvar_get_descr(const char *name)
{
    return NV_GET_fn(name ? name : "");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_var(const char *dst, const char *src)
{
    DESCR_t d = NV_GET_fn(src ? src : "");
    NV_SET_fn(dst ? dst : "", d);
    if (g_monitor_bin) mon_emit_value_bin(dst ? dst : "", d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_descr(const char *name, int64_t lo, int64_t hi)
{
    DESCR_t d;
    union { int64_t q; struct { DTYPE_t v; uint32_t slen; } f; } u;
    u.q    = lo;
    d.v    = u.f.v;
    d.slen = u.f.slen;
    d.i    = hi;
    NV_SET_fn(name ? name : "", d);
    if (g_monitor_bin) mon_emit_value_bin(name ? name : "", d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern DESCR_t VARVAL_d_fn(DESCR_t d);
typedef struct { const char *base; long len; } rt_subj_t;
const char *g_subject_dbg_base = 0;
long        g_subject_dbg_len  = -1;
/*--------------------------------------------------------------------------------------------------------------------*/
#define RT_FRAME_STACK_MAX 256
#define RT_FRAME_SLOT_MAX  64
typedef struct { DESCR_t slot[RT_FRAME_SLOT_MAX]; int nslots; } rt_frame_t;
static rt_frame_t g_rt_frames[RT_FRAME_STACK_MAX];
static int        g_rt_frame_depth = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
#define PROC_FRAME_QWORDS 512
#define PROC_FRAME_DEPTH  4096
#define CALL_ARGS_MAX     64
typedef struct { const char *name; bb_box_fn fn; const char **pnames; int nparams; int frame_nslots; int decl_level; uint64_t byref_mask; int frame_bytes; DESCR_t **pcells; DESCR_t *rcell; int cells_done; int is_generator; } rt_proc_t;
static rt_proc_t    *g_rt_gen_procs = (rt_proc_t *)0;
static int           g_rt_gen_proc_count = 0;
static int           g_rt_gen_proc_cap = 0;
static void rt_gen_proc_grow(void) { if (g_rt_gen_proc_count < g_rt_gen_proc_cap) return; int nc = g_rt_gen_proc_cap ? g_rt_gen_proc_cap * 2 : 64; rt_proc_t *np = (rt_proc_t *)realloc(g_rt_gen_procs, (size_t)nc * sizeof(rt_proc_t)); if (!np) return; g_rt_gen_procs = np; g_rt_gen_proc_cap = nc; }
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
    p->name = name; p->fn = NULL; p->pnames = pnames; p->nparams = nparams; p->frame_nslots = -1; p->decl_level = 0; p->byref_mask = 0; p->frame_bytes = 0; p->pcells = (DESCR_t **)0; p->rcell = (DESCR_t *)0; p->cells_done = 0; p->is_generator = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static rt_proc_t *rt_proc_find(const char *name);
void rt_proc_cache_clear(void);
int rt_proc_enum_count(void) { return g_rt_gen_proc_count; }
const char *rt_proc_enum_name(int i) { return (i >= 0 && i < g_rt_gen_proc_count) ? g_rt_gen_procs[i].name : (const char *)0; }
void rt_proc_reset(void) { g_rt_gen_proc_count = 0; rt_proc_cache_clear(); }
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t *gva_register(const char **names, DESCR_t *cells, int n) {
    if (!cells) return cells;
    for (int k = 0; k < n; k++) { const char *nm = names ? names[k] : (const char *)0; if (!nm) continue; (void)NV_bind_gva(nm, &cells[k]); }
    return cells;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_proc_is_registered(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_proc_has_native_fn(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return g_rt_gen_procs[i].fn != (bb_box_fn)0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_generator(const char *name, int is_gen)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_rt_gen_procs[i].is_generator = is_gen ? 1 : 0; return; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_proc_is_generator(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) return g_rt_gen_procs[i].is_generator;
    return 0;
}
void rt_proc_set_fn(const char *name, bb_box_fn fn)
{
    if (!name) return;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_rt_gen_procs[i].fn = fn; return; }
    rt_gen_proc_grow();
    if (g_rt_gen_proc_count >= g_rt_gen_proc_cap) return;
    rt_proc_t *p = &g_rt_gen_procs[g_rt_gen_proc_count++];
    p->name = name; p->fn = fn; p->pnames = NULL; p->nparams = 0; p->frame_nslots = -1; p->decl_level = 0; p->byref_mask = 0; p->frame_bytes = 0; p->pcells = (DESCR_t **)0; p->rcell = (DESCR_t *)0; p->cells_done = 0; p->is_generator = 0;
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
/* Lazily heap-allocated (GC-scanned) so the 16 MB never sits in static BSS:
 * programs that make no procedure calls (e.g. all Prolog GZ programs) never
 * allocate it, sparing Boehm a ~MB conservative root scan at startup. */
static int64_t *g_proc_arena = (int64_t *)0;
static int64_t *proc_arena(void) {
    if (!g_proc_arena)
        g_proc_arena = (int64_t *)GC_MALLOC((size_t)(PROC_FRAME_DEPTH * PROC_FRAME_QWORDS) * sizeof(int64_t));
    return g_proc_arena;
}
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
    char *fb = (char *)&proc_arena()[g_proc_depth * PROC_FRAME_QWORDS];
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
#define RT_INITIAL_MAX 8192
static int64_t g_initial_fired[RT_INITIAL_MAX];
static int     g_initial_fired_n = 0;
int64_t rt_initial_fire(int64_t site)
{
    for (int i = 0; i < g_initial_fired_n; i++) if (g_initial_fired[i] == site) return 0;
    if (g_initial_fired_n < RT_INITIAL_MAX) g_initial_fired[g_initial_fired_n++] = site;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define RT_GEN_ACT_MAX 256
typedef struct { char *frame; bb_box_fn fn; } rt_gen_act_t;
static int64_t     g_gen_arena[RT_GEN_ACT_MAX * PROC_FRAME_QWORDS];
static rt_gen_act_t g_gen_act[RT_GEN_ACT_MAX];
static int          g_gen_act_top = 0;
DESCR_t rt_proc_call_gen(const char *name, int nargs)
{
    rt_proc_t *p = (rt_proc_t *)0;
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { p = &g_rt_gen_procs[i]; break; }
    if (!p || !p->fn) { fprintf(stderr, "[SUSP] rt_proc_call_gen: generator '%s' has no stackless slab\n", name ? name : "(null)"); abort(); }
    if (g_gen_act_top >= RT_GEN_ACT_MAX) { fprintf(stderr, "[SUSP] rt_proc_call_gen: generator activation depth exceeded (%d)\n", RT_GEN_ACT_MAX); abort(); }
    char *fb = (char *)&g_gen_arena[g_gen_act_top * PROC_FRAME_QWORDS];
    *(DESCR_t *)(fb + 0) = NULVCL;
    if (nargs > CALL_ARGS_MAX) nargs = CALL_ARGS_MAX;
    for (int i = 0; i < nargs; i++) *(DESCR_t *)(fb + 16 * (i + 1)) = g_call_args[i];
    g_gen_act[g_gen_act_top].frame = fb;
    g_gen_act[g_gen_act_top].fn    = p->fn;
    g_gen_act_top++;
    (void)p->fn((void *)fb, 0);
    DESCR_t result = *(DESCR_t *)(fb + 0);
    if (IS_FAIL(result)) g_gen_act_top--;
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_resume_gen(void)
{
    if (g_gen_act_top <= 0) return FAILDESCR;
    rt_gen_act_t *a = &g_gen_act[g_gen_act_top - 1];
    (void)a->fn((void *)a->frame, 1);
    DESCR_t result = *(DESCR_t *)(a->frame + 0);
    if (IS_FAIL(result)) g_gen_act_top--;
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *name; DESCR_t *cell; DESCR_t old; } NameSaveEnt;
#define PROC_FRAME_NEST_QWORDS 512
#define PROC_FRAME_ARENA_QWORDS (8 * 1024 * 1024)
#define PROC_FRAME_NEST_MAX (PROC_FRAME_ARENA_QWORDS / PROC_FRAME_NEST_QWORDS)
#define DCR_CELL_CACHE_SIZE 2048
#define DCR_CELL_CACHE_MASK (DCR_CELL_CACHE_SIZE - 1)
static NameSaveEnt   *g_name_save = (NameSaveEnt *)0;
static int            g_name_save_top = 0;
static int            g_name_save_cap = 0;
static struct { const char *name; DESCR_t *cell; int valid; } g_cell_cache[DCR_CELL_CACHE_SIZE];
static int            g_proc_idx_slot[DCR_CELL_CACHE_SIZE];
static const char    *g_proc_idx_key[DCR_CELL_CACHE_SIZE];
static int rt_call_fastpath_ok(void) { return !g_call_fastpath_off; }
static int rt_name_side_effecting(const char *nm)
{
    static const char *S[] = { "TERMINAL", "ALPHABET", "STCOUNT", "STNO", 0 };
    if (is_protected_pat_name(nm)) return 1;
    for (int i = 0; S[i]; i++) if (strcmp(nm, S[i]) == 0) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *rt_cell_for(const char *nm)
{
    unsigned h = (unsigned)(((uintptr_t)nm >> 4) & DCR_CELL_CACHE_MASK);
    if (g_cell_cache[h].valid && g_cell_cache[h].name == nm) return g_cell_cache[h].cell;
    DESCR_t *c = rt_name_side_effecting(nm) ? (DESCR_t *)0 : NV_PTR_fn(nm);
    g_cell_cache[h].name = nm; g_cell_cache[h].cell = c; g_cell_cache[h].valid = 1;
    return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void rt_proc_resolve_cells(rt_proc_t *p)
{
    if (p->cells_done) return;
    int np = p->nparams; const char **pn = p->pnames;
    if (np > 0 && pn) { p->pcells = (DESCR_t **)malloc((size_t)np * sizeof(DESCR_t *)); if (p->pcells) for (int k = 0; k < np; k++) { const char *nm = pn[k]; p->pcells[k] = (nm && !rt_name_side_effecting(nm)) ? NV_PTR_fn(nm) : (DESCR_t *)0; } }
    p->rcell = (p->name && !rt_name_side_effecting(p->name)) ? NV_PTR_fn(p->name) : (DESCR_t *)0;
    p->cells_done = 1;
}
static void rt_name_save_grow(void) { if (g_name_save_top < g_name_save_cap) return; int nc = g_name_save_cap ? g_name_save_cap * 2 : 4096; NameSaveEnt *np = (NameSaveEnt *)realloc(g_name_save, (size_t)nc * sizeof(NameSaveEnt)); if (!np) return; g_name_save = np; g_name_save_cap = nc; }
/* Lazily heap-allocated (GC-scanned), see proc_arena() above — keeps 64 MB out
 * of static BSS so unused-procedure-call programs skip the startup root scan. */
static int64_t *g_proc_frame_nest_arena = (int64_t *)0;
static int64_t *proc_nest_arena(void) {
    if (!g_proc_frame_nest_arena)
        g_proc_frame_nest_arena = (int64_t *)GC_MALLOC((size_t)PROC_FRAME_ARENA_QWORDS * sizeof(int64_t));
    return g_proc_frame_nest_arena;
}
static int            g_proc_frame_nest_depth = 0;
static long           g_proc_frame_cursor_qw = 0;
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_name_restore(int base)
{
    for (int k = g_name_save_top - 1; k >= base; k--) {
        if (g_name_save[k].cell) *g_name_save[k].cell = g_name_save[k].old;
        else NV_SET_fn(g_name_save[k].name, g_name_save[k].old);
    }
    g_name_save_top = base;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void mon_emit_call_bin(const char *fname) {
    if (!g_monitor_bin || !fname) return;
    int64_t saved = kw_ftrace; kw_ftrace = 1; comm_call(fname); kw_ftrace = saved;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void mon_emit_return_bin(const char *fname, DESCR_t retval) {
    if (!g_monitor_bin || !fname) return;
    char saved_rt[16]; memcpy(saved_rt, kw_rtntype, sizeof(saved_rt));
    const char *disc = IS_FAIL_fn(retval) ? "FRETURN" : "RETURN";
    size_t dl = strlen(disc); if (dl > 15) dl = 15; memcpy(kw_rtntype, disc, dl); kw_rtntype[dl] = '\0';
    int64_t saved = kw_ftrace; kw_ftrace = 1; comm_return(fname, retval); kw_ftrace = saved;
    memcpy(kw_rtntype, saved_rt, sizeof(saved_rt));
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs)
{
    if (!name) return FAILDESCR;
    rt_proc_t *p = rt_proc_find(name);
    if (!p || !p->fn) return FAILDESCR;
    rt_proc_resolve_cells(p);
    int np = p->nparams;
    const char **pn = p->pnames;
    if (g_proc_frame_nest_depth >= PROC_FRAME_NEST_MAX) return FAILDESCR;
    int fbytes = (int)(PROC_FRAME_NEST_QWORDS * 8);
    if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    long fqw = (long)(((fbytes + 15) & ~15) / 8);
    if (g_proc_frame_cursor_qw + fqw > PROC_FRAME_ARENA_QWORDS) return FAILDESCR;
    int save_base = rt_name_save_push(pn, p->pcells, args, nargs, np);
    rt_name_save_push(&name, &p->rcell, (DESCR_t *)0, 0, 1);
    void *fb = (void *)&proc_nest_arena()[g_proc_frame_cursor_qw];
    long save_cursor = g_proc_frame_cursor_qw;
    g_proc_frame_cursor_qw += fqw;
    g_proc_frame_nest_depth++;
    const char *save_Σ = Σ; int save_Σlen = Σlen;
    if (g_monitor_bin) mon_emit_call_bin(name);
    DESCR_t fret = p->fn(fb, 0);
    Σ = save_Σ; Σlen = save_Σlen;
    g_proc_frame_nest_depth--;
    g_proc_frame_cursor_qw = save_cursor;
    DESCR_t *rcell = rt_call_fastpath_ok() ? p->rcell : (DESCR_t *)0; DESCR_t result = IS_FAIL_fn(fret) ? FAILDESCR : (rcell ? *rcell : NV_GET_fn(name));
    rt_name_restore(save_base);
    if (g_monitor_bin) mon_emit_return_bin(name, result);
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_proc_direct(long idx, DESCR_t *args, int nargs)
{
    if (idx < 0 || idx >= g_rt_gen_proc_count) return FAILDESCR;
    rt_proc_t *p = &g_rt_gen_procs[idx];
    if (!p->fn) return FAILDESCR;
    rt_proc_resolve_cells(p);
    int np = p->nparams;
    const char **pn = p->pnames;
    const char *name = p->name;
    if (g_proc_frame_nest_depth >= PROC_FRAME_NEST_MAX) return FAILDESCR;
    int fbytes = (int)(PROC_FRAME_NEST_QWORDS * 8);
    if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    long fqw = (long)(((fbytes + 15) & ~15) / 8);
    if (g_proc_frame_cursor_qw + fqw > PROC_FRAME_ARENA_QWORDS) return FAILDESCR;
    int save_base = rt_name_save_push(pn, p->pcells, args, nargs, np);
    rt_name_save_push(&name, &p->rcell, (DESCR_t *)0, 0, 1);
    void *fb = (void *)&proc_nest_arena()[g_proc_frame_cursor_qw];
    long save_cursor = g_proc_frame_cursor_qw;
    g_proc_frame_cursor_qw += fqw;
    g_proc_frame_nest_depth++;
    const char *save_Σ = Σ; int save_Σlen = Σlen;
    if (g_monitor_bin) mon_emit_call_bin(name);
    DESCR_t fret = p->fn(fb, 0);
    Σ = save_Σ; Σlen = save_Σlen;
    g_proc_frame_nest_depth--;
    g_proc_frame_cursor_qw = save_cursor;
    DESCR_t *rcell = rt_call_fastpath_ok() ? p->rcell : (DESCR_t *)0; DESCR_t result = IS_FAIL_fn(fret) ? FAILDESCR : (rcell ? *rcell : NV_GET_fn(name));
    rt_name_restore(save_base);
    if (g_monitor_bin) mon_emit_return_bin(name, result);
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_proc_index_of(const char *name)
{
    if (!name) return -1;
    unsigned h = (unsigned)(((uintptr_t)name >> 4) & DCR_CELL_CACHE_MASK);
    if (g_proc_idx_key[h] == name) { int ci = g_proc_idx_slot[h]; if (ci < g_rt_gen_proc_count) return ci; }
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_proc_idx_key[h] = name; g_proc_idx_slot[h] = i; return i; }
    return -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_proc_table_fill(int64_t *tab, const char **names, int n)
{
    if (!tab || !names) return;
    for (int k = 0; k < n; k++) tab[k] = (int64_t)rt_proc_index_of(names[k]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_proc_cache_clear(void) { for (int i = 0; i < DCR_CELL_CACHE_SIZE; i++) g_proc_idx_key[i] = (const char *)0; }
/*--------------------------------------------------------------------------------------------------------------------*/
static rt_proc_t * rt_proc_find(const char *name)
{
    if (!name) return (rt_proc_t *)0;
    unsigned h = (unsigned)(((uintptr_t)name >> 4) & DCR_CELL_CACHE_MASK);
    if (g_proc_idx_key[h] == name) { int ci = g_proc_idx_slot[h]; if (ci < g_rt_gen_proc_count) return &g_rt_gen_procs[ci]; }
    for (int i = 0; i < g_rt_gen_proc_count; i++)
        if (g_rt_gen_procs[i].name && strcmp(g_rt_gen_procs[i].name, name) == 0) { g_proc_idx_key[h] = name; g_proc_idx_slot[h] = i; return &g_rt_gen_procs[i]; }
    return (rt_proc_t *)0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_frame(const char *name, int nslots, int decl_level)
{
    rt_proc_t *p = rt_proc_find(name);
    if (p) { p->frame_nslots = nslots; p->decl_level = decl_level; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_frame_bytes(const char *name, int bytes)
{
    rt_proc_t *p = rt_proc_find(name);
    if (p && bytes > p->frame_bytes) p->frame_bytes = bytes;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_proc_frame_nslots(const char *name)
{
    rt_proc_t *p = rt_proc_find(name);
    return p ? p->frame_nslots : -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_proc_decl_level(const char *name)
{
    rt_proc_t *p = rt_proc_find(name);
    return p ? p->decl_level : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_proc_set_byref(const char *name, uint64_t mask)
{
    rt_proc_t *p = rt_proc_find(name);
    if (p) p->byref_mask = mask;
}
/*--------------------------------------------------------------------------------------------------------------------*/
uint64_t rt_proc_byref_mask(const char *name)
{
    rt_proc_t *p = rt_proc_find(name);
    return p ? p->byref_mask : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t *rt_gvar_cell(const char *name)
{
    return NV_PTR_fn(name);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_named_proc_sl(const char *name, DESCR_t *args, int nargs, void *sl)
{
    rt_proc_t *p = rt_proc_find(name);
    if (!p || !p->fn) return FAILDESCR;
    rt_proc_resolve_cells(p);
    int np = p->nparams;
    int ns = (p->frame_nslots > np) ? p->frame_nslots : np;
    if (g_proc_frame_nest_depth >= PROC_FRAME_NEST_MAX) return FAILDESCR;
    int fbytes = (int)(PROC_FRAME_NEST_QWORDS * 8);
    if (p->frame_bytes > fbytes) fbytes = p->frame_bytes;
    long fqw = (long)(((fbytes + 15) & ~15) / 8);
    if (g_proc_frame_cursor_qw + fqw > PROC_FRAME_ARENA_QWORDS) return FAILDESCR;
    int save_base = g_name_save_top;
    rt_name_save_push(&name, &p->rcell, (DESCR_t *)0, 0, 1);
    void *fb = (void *)&proc_nest_arena()[g_proc_frame_cursor_qw];
    long save_cursor = g_proc_frame_cursor_qw;
    g_proc_frame_cursor_qw += fqw;
    g_proc_frame_nest_depth++;
    ((void **)fb)[0] = sl;
    DESCR_t *slots = (DESCR_t *)((char *)fb + 16);
    for (int k = 0; k < ns; k++) slots[k] = (k < np && k < nargs) ? args[k] : NULVCL;
    const char *save_Σ = Σ; int save_Σlen = Σlen;
    if (g_monitor_bin) mon_emit_call_bin(name);
    DESCR_t fret = p->fn(fb, 0);
    Σ = save_Σ; Σlen = save_Σlen;
    g_proc_frame_nest_depth--;
    g_proc_frame_cursor_qw = save_cursor;
    DESCR_t *rcell = rt_call_fastpath_ok() ? p->rcell : (DESCR_t *)0; DESCR_t result = IS_FAIL_fn(fret) ? FAILDESCR : (rcell ? *rcell : NV_GET_fn(name));
    rt_name_restore(save_base);
    if (g_monitor_bin) mon_emit_return_bin(name, result);
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_proc_define(const char *spec)
{
    (void)spec;
    return NULVCL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int g_last_ok = 0;
/*--------------------------------------------------------------------------------------------------------------------*/
extern void rt_set_last_ok(int v);
/*--------------------------------------------------------------------------------------------------------------------*/
extern DESCR_t binop_apply(int op, DESCR_t lv, DESCR_t rv, int *rel_fail);
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_size_d(uint64_t lo, uint64_t hi)
{
    DESCR_t v;
    v.v    = (DTYPE_t)(uint32_t)(lo & 0xFFFFFFFFu);
    v.slen = (uint32_t)(lo >> 32);
    v.i    = (int64_t)hi;
    if (IS_FAIL_fn(v)) return FAILDESCR;
    if (v.v == DT_SNUL) { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = 0; return r; }
    if (v.v == DT_T) { DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)(v.tbl ? v.tbl->size : 0); return r; }
    if (IS_CSET_fn(v)) { size_t n = v.s ? strlen(v.s) : 0; DESCR_t r; r.v = DT_I; r.slen = 0; r.i = (int64_t)n; return r; }
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern int list_bang_at(DESCR_t obj, int64_t idx, DESCR_t *out);
DESCR_t rt_list_bang_at(DESCR_t obj, int64_t idx)
{
    DESCR_t out;
    if (list_bang_at(obj, idx, &out)) return out;
    return FAILDESCR;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_cut_set(void)
{
    extern int g_resolve_cut_flag;
    g_resolve_cut_flag = 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int     subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
extern int     subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val);
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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gc/gc.h>
typedef struct { const char *chars; int delta; } rt_cs_t;
/*--------------------------------------------------------------------------------------------------------------------*/
extern void flush_pending_captures(void);
extern void reset_capture_registry(void);
extern void clear_pending_flags(void);
static void rt_register_cap(cap_t *c);
#define RT_MAX_CAPTURES 256
static cap_t *g_rt_cap_list[RT_MAX_CAPTURES];
static int    g_rt_cap_count = 0;
