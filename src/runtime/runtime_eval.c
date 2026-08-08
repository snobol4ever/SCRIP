#include <stdio.h>
#include "rt/rt_arena.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "core.h"
#include "sil_macros.h"
#include "../parser/snobol4/scrip_cc.h"
#include "IR.h"
#include "stage2.h"
extern int exec_stmt(const char  *subj_name,
                          DESCR_t     *subj_var,
                          DESCR_t      pat,
                          DESCR_t     *repl,
                          int          has_repl);
extern const char *Σ;
extern int         Ω;
extern int         Δ;
typedef DESCR_t (*eval_chain_fn)(void *zeta, int entry);
extern void          *lower_snobol4(const tree_t *prog);
extern eval_chain_fn  emit_chain(void *entry, void *out, const char *prefix);
extern void           rt_chain_enter(eval_chain_fn fn);
extern int            emit_jmp_entry_for_chain(IR_graph_t *g);
extern void           emit_jmp_entry_clear(void);
extern void           ast_tree_free_dyn(tree_t *p);
extern void           IR_free_dyn(void *g);
extern size_t         bb_pool_mark(void);
extern void           bb_pool_release(size_t mark);
#define EVAL_TMP "ZZEVALZZ"
#define EVAL_RETAIN_BUDGET (2 * 1024 * 1024)
typedef struct { char *key; eval_chain_fn fn; } eval_cache_ent_t;
static eval_cache_ent_t *g_eval_cache = NULL;
static int               g_eval_cache_n = 0;
static int               g_eval_cache_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned long eval_cache_hash(const char *s) { unsigned long h = 1469598103934665603UL; while (*s) { h ^= (unsigned char)*s++; h *= 1099511628211UL; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn eval_cache_get(const char *s) {
    if (g_eval_cache_cap == 0) return NULL;
    unsigned long m = (unsigned long)g_eval_cache_cap - 1;
    for (unsigned long i = eval_cache_hash(s) & m, p = 0; p < (unsigned long)g_eval_cache_cap; p++, i = (i + 1) & m)
        if (!g_eval_cache[i].key) return NULL; else if (strcmp(g_eval_cache[i].key, s) == 0) return g_eval_cache[i].fn;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eval_cache_insert_raw(eval_cache_ent_t *tab, int cap, char *key, eval_chain_fn fn) {
    unsigned long m = (unsigned long)cap - 1;
    unsigned long i = eval_cache_hash(key) & m;
    while (tab[i].key) i = (i + 1) & m;
    tab[i].key = key; tab[i].fn = fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eval_cache_put(const char *s, eval_chain_fn fn) {
    if (g_eval_cache_cap == 0 || (g_eval_cache_n + 1) * 2 > g_eval_cache_cap) {
        int ncap = g_eval_cache_cap ? g_eval_cache_cap * 2 : 16;
        eval_cache_ent_t *ntab = (eval_cache_ent_t *)rt_ws_alloc((size_t)ncap * sizeof(eval_cache_ent_t));
        if (!ntab) return;
        for (int k = 0; k < g_eval_cache_cap; k++) if (g_eval_cache[k].key) eval_cache_insert_raw(ntab, ncap, g_eval_cache[k].key, g_eval_cache[k].fn);
        g_eval_cache = ntab; g_eval_cache_cap = ncap;
    }
    char *key = rt_ws_strdup(s);
    if (!key) return;
    eval_cache_insert_raw(g_eval_cache, g_eval_cache_cap, key, fn);
    g_eval_cache_n++;
}
/* rt_chain_enter — the s59 DEFER shape for EVAL/CODE (Lon: "no CALL/RET — a JUMP and a JUMP BACK").  Resolve
 * happened in C (cache/compile/registry lookup); this shim WIRES outside-γ→rcx and outside-ω→rdx to one shared
 * landing and JUMPS.  R12-ERAD s67: the r12 anchor + `mov r12,rsp` wholesale reclaim are DELETED — the s65
 * shared epilogue already converted chain exits to ABSOLUTE unwind on BOTH edges (measured in the live blob:
 * γ and ω each `mov [rsp+K-24/-16],rax; lea rsp,[rsp+K]; jmp *rax`, interior failures funnel through the
 * blob-local epilogue first), so the landing always arrives at exactly the pre-jmp rsp and r12 rides through
 * as an ordinary callee-save.  The chain is a NEW ACTIVATION: it self-allocates its own frame (K_total baked
 * at emit time from its region) and is one-shot.  The trailing ret is the C builtin boundary, not the
 * transfer.  ⚠ EXACTLY FIVE PUSHES — the SysV 16-byte stack alignment is load-bearing through the jmp: rsp is
 * 8 mod 16 at entry, +40 bytes of pushes makes it 0 mod 16, and the blob's `sub rsp,K_total` (K_total
 * 16-aligned) carries that alignment into the activation.  A sixth push (the first cut saved rbp) left every
 * C callee reached FROM the chain on a misaligned stack and SEGV'd in libc's SSE printf path — measured, gdb,
 * rsp=...be8.  rbp needs no save here: it is the align-save register the chains manage themselves
 * (x86_align_enter/leave).  PROC-CONV converted the last call-regime citizen (LBL__ pseudo-procs,
 * rt_goto_transfer arm 4) to this same transfer; the donated-frame shim rt_callregime_run is deleted. */
__asm__(
".text\n"
".globl rt_chain_enter\n"
"rt_chain_enter:\n"
"  pushq %rbx\n"
"  pushq %r12\n"
"  pushq %r13\n"
"  pushq %r14\n"
"  pushq %r15\n"
"  movq %rdi, %rax\n"
"  leaq 1f(%rip), %rcx\n"
"  movq %rcx, %rdx\n"
/* RTCC RC-2 INBOUND LOAD (RC-0(d) edge class 1): block→{r10,r11,r8,r9} before the jmp into generated code. */
/* BLOCK-CANONICAL LAW: a C→generated crossing LOADS the cache.  Gated on g_rtcc_on; GOT-indirect (PIC-safe). */
/* rax/rcx/rdx carry the live wires and are NOT touched; r10 is the GOT scratch and is overwritten LAST.      */
"  movq g_rtcc_on@GOTPCREL(%rip), %r10\n"
"  cmpb $0, (%r10)\n"
"  je 2f\n"
"  movq g_rtcc_block@GOTPCREL(%rip), %r10\n"
"  movq 64(%r10), %r11\n"
"  movq 40(%r10), %r8\n"
"  movq 48(%r10), %r9\n"
"  movq 56(%r10), %r10\n"
"2:\n"
"  jmp *%rax\n"
"1:\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  ret\n"
);
void rt_chain_enter(eval_chain_fn fn);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Deferred-expression thunks (`. *F(X)`) minted by a RUNTIME compile.  lower_snobol4 mints one EXPR$N proc per
 * deferred operand and files it in g_stage2; the driver does the register+emit walk for the main program, and
 * this is that same two-phase walk for the fragment path, over just the procs the fragment added [pc0, count).
 * Without it the dcap pump's rt_proc_call_open on EXPR$N finds no body and the conditional assignment is a
 * no-op.  dyn_scope=1 on every thunk, so the pump's open lands the dyn prologue and the NRETURN'd NAME rides
 * the rt_g_want_name the pump re-arms. */
static void eval_thunks_emit_from(int pc0)
{
    extern void rt_proc_register(const char *name, const char **pnames, int nparams);
    extern void rt_proc_set_fn(const char *name, eval_chain_fn fn);
    extern void rt_proc_set_generator(const char *name, int is_gen);
    extern void rt_proc_set_variadic(const char *name, int is_var);
    extern void rt_proc_set_dyn_scope(const char *name, int v);
    extern void rt_proc_set_result_name(const char *name, const char *rname);
    extern void ir_drive_slot_assign(IR_graph_t *g);
    extern int g_gen_proc_active;
    extern int g_frame_active;
    extern IR_graph_t *g_emit_cfg;
    for (int pi = pc0; pi < g_stage2.proc_count; pi++) {
        const char *pname = g_stage2.proc_table[pi].name;
        int idx = g_stage2.proc_table[pi].bb_idx;
        if (!pname || idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) continue;
        rt_proc_register(pname, (const char **)0, 0);
        rt_proc_set_generator(pname, g_stage2.proc_table[pi].is_generator);
        rt_proc_set_variadic(pname, g_stage2.proc_table[pi].is_variadic);
        rt_proc_set_rest_kind(pname, g_stage2.proc_table[pi].rest_kind);
        rt_proc_set_named_rest(pname, g_stage2.proc_table[pi].named_rest);
        rt_proc_set_dyn_scope(pname, g_stage2.proc_table[pi].dyn_scope);
        if (g_stage2.proc_table[pi].result_name) rt_proc_set_result_name(pname, g_stage2.proc_table[pi].result_name);
    }
    IR_graph_t *cfg_sv = g_emit_cfg;
    int fa = g_frame_active; g_frame_active = 1;
    int ga = g_gen_proc_active;
    for (int pi = pc0; pi < g_stage2.proc_count; pi++) {
        const char *pname = g_stage2.proc_table[pi].name;
        int idx = g_stage2.proc_table[pi].bb_idx;
        if (!pname || idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) continue;
        ir_drive_slot_assign(g_stage2.bbp.table[idx]);
        g_emit_cfg = g_stage2.bbp.table[idx];
        g_gen_proc_active = g_stage2.proc_table[pi].is_generator;
        { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*);
          if (!emit_jmp_entry_for_patproc(pname, g_stage2.bbp.table[idx])) emit_jmp_entry_for_proc(pname, g_stage2.proc_table[pi].dyn_scope, g_stage2.proc_table[pi].is_generator, g_stage2.bbp.table[idx]); }
        eval_chain_fn pfn = emit_chain(g_stage2.bbp.table[idx]->entry, NULL, "proc_flat");
        if (pfn) rt_proc_set_fn(pname, pfn);
        emit_jmp_entry_clear();
    }
    g_gen_proc_active = ga; g_frame_active = fa; g_emit_cfg = cfg_sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn eval_build_chain(const char *s)
{
    if (!s || !*s) return NULL;
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void fc_tables_reset(void); fc_tables_reset(); extern void zls_reset(void); zls_reset(); }
    size_t n = strlen(s);
    char *src = (char *)rt_ws_alloc(n + 4);
    if (!src) return NULL;
    snprintf(src, n + 4, "(%s)", s);
    extern void sno_error_quiet_begin(void); extern void sno_error_quiet_end(void); extern const char *sno_error_captured(void); extern const char *g_sno_errtext;
    sno_error_quiet_begin();
    tree_t *e = parse_expr_pat_from_str(src);
    sno_error_quiet_end();
    if (!e) { const char *cap = sno_error_captured(); if (cap) g_sno_errtext = rt_ws_strdup_c(cap); return NULL; }
    tree_t *var = ast_stmt_new(TT_VAR);
    var->v.sval = rt_ws_strdup(EVAL_TMP);
    tree_t *st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":line", 1));
    ast_push(st, ast_attr_int(":stno", 1));
    ast_push(st, ast_attr_expr(":subj", var));
    ast_push(st, ast_attr_leaf(":eq", ""));
    ast_push(st, ast_attr_expr(":repl", e));
    tree_t *prog = ast_stmt_new(TT_PROGRAM);
    ast_push(prog, st);
    extern void sno_expr_salt_next(void);
    extern int sno_expr_mark(void);
    extern void sno_expr_thunks_build(int x0);
    extern int sno_pat_count(void); extern void sno_pat_thunks_build(int p0);
    sno_expr_salt_next();
    int xm = sno_expr_mark();
    int pat0 = sno_pat_count();
    int pc0 = g_stage2.proc_count;
    void *g = lower_snobol4(prog);
    if (!g) { ast_tree_free_dyn(prog); return NULL; }
    sno_expr_thunks_build(xm);
    if (sno_pat_count() > pat0) sno_pat_thunks_build(pat0);   /* BLOCKER-C (s144): EVAL expr may mint PAT$N — build proc thunks so eval_thunks_emit_from below emits+registers them (else SNO$MKPAT miss) */
    extern int g_frame_active;
    extern IR_graph_t *g_emit_cfg;
    IR_graph_t *cfg_sv = g_emit_cfg; g_emit_cfg = (IR_graph_t *)g;
    int fa = g_frame_active; g_frame_active = 1;
    emit_jmp_entry_for_chain((IR_graph_t *)g);
    eval_chain_fn fn = emit_chain(((IR_graph_t *)g)->entry, NULL, "pat_flat");
    emit_jmp_entry_clear();
    g_frame_active = fa; g_emit_cfg = cfg_sv;
    eval_thunks_emit_from(pc0);
    IR_free_dyn(g);
    ast_tree_free_dyn(prog);
    return fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* EVAL-CHAIN-RET (this session): CLASS C chains (EVAL/CODE JIT fragments) exit via bb_glue_outer_γ/ω which
 * does mov rsp,rbp; pop rbp; ret.  The ret lands in eval_string_transient (the caller of THIS function)
 * after "call eval_chain_run_capture" — skipping this function's NV_GET_fn(EVAL_TMP) call entirely.
 * The chain's γ return value is eax=DT_S, rdx=garbage — unusable as a DESCR.
 * FIX: eval_string_transient reads ZZEVALZZ directly after calling eval_chain_run_capture.
 * THIS function now only exists to correctly set up rbp as the eval_chain_run_capture frame base
 * so the chain's "mov rsp,rbp; pop rbp; ret" correctly unwinds to eval_string_transient.
 * __attribute__((noinline)) ensures a real call frame with its own rbp. */
__attribute__((noinline))
static void eval_chain_enter_only(eval_chain_fn fn) {
    rt_chain_enter(fn);
    /* chain ret lands in eval_string_transient; this line is never reached */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_string_transient(const char *s) {
    if (!s || !*s) return NULVCL;
    eval_chain_fn cached = eval_cache_get(s);
    if (cached) {
        DESCR_t saved = NV_GET_fn(EVAL_TMP);
        eval_chain_enter_only(cached);
        DESCR_t result = NV_GET_fn(EVAL_TMP);
        NV_SET_fn(EVAL_TMP, saved);
        return result;
    }
    size_t mark = bb_pool_mark();
    eval_chain_fn fn = eval_build_chain(s);
    if (!fn) { bb_pool_release(mark); return FAILDESCR; }
    DESCR_t saved = NV_GET_fn(EVAL_TMP);
    eval_chain_enter_only(fn);
    DESCR_t result = NV_GET_fn(EVAL_TMP);
    NV_SET_fn(EVAL_TMP, saved);
    if (mark < EVAL_RETAIN_BUDGET) eval_cache_put(s, fn);
    else bb_pool_release(mark);
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_node(tree_t *e)
{
    (void)e;
    fprintf(stderr, "[B0b] BOMB eval_node: AST-walk evaluator deleted; nothing interprets tree_t at runtime\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_expr(const char *src)
{
    if (!src || !*src) return NULVCL;
    tree_t *tree = parse_expr_pat_from_str(src);
    if (!tree) return FAILDESCR;
    return eval_node(tree);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* EVAL/CODE (manual Ch.9, directive lifted 2026-07-08).  Runtime label registry: fragment labels registered by
 * code(); resolution order in rt_goto_transfer is (1) `$X` indirect deref, (2) END, (3) this registry — so a
 * fragment label OVERRIDES a same-named main label per the manual, (4) the main program's LBL__ pseudo-procs
 * (exported by lower_sno_stage2 when the program uses CODE), (5) a variable holding a CODE value (the lexer
 * folds direct-goto `:<C>` onto the plain-name form, so `C` here may be the variable), (6) fault.  A transfer
 * RUNS the target nested on a fresh 64KB frame (GC-visible so DESCR temporaries in it stay rooted); SNOBOL4
 * gotos never resume their source, so the target running to termination cascades clean returns back up every
 * crossing — the process exits through the driver as always.  Honest slice-1 caveats: one frame is allocated
 * per crossing and never freed, and each crossing nests one C-stack level, so a loop that ping-pongs across
 * the main/fragment boundary (label-to-label, not within one graph) grows both without bound — fine for the
 * manual's shapes (a handful of crossings), a real rung for a frame-recycling tail-transfer later. */
typedef struct { char *key; eval_chain_fn fn; } lbl_ent_t;
static lbl_ent_t *g_lbl_tab = NULL;
static int        g_lbl_n = 0;
static int        g_lbl_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_label_set_fn(const char *name, void *fn) {
    if (!name || !*name) return;
    for (int i = 0; i < g_lbl_n; i++) if (!strcmp(g_lbl_tab[i].key, name)) { g_lbl_tab[i].fn = (eval_chain_fn)fn; return; }
    if (g_lbl_n >= g_lbl_cap) {
        int ncap = g_lbl_cap ? g_lbl_cap * 2 : 16;
        lbl_ent_t *nt = (lbl_ent_t *)rt_ws_realloc(g_lbl_tab, (size_t)ncap * sizeof(lbl_ent_t));
        if (!nt) return;
        g_lbl_tab = nt; g_lbl_cap = ncap;
    }
    g_lbl_tab[g_lbl_n].key = rt_ws_strdup(name);
    g_lbl_tab[g_lbl_n].fn  = (eval_chain_fn)fn;
    g_lbl_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn rt_label_get_fn(const char *name) {
    if (!name || !*name) return NULL;
    for (int i = 0; i < g_lbl_n; i++) if (!strcmp(g_lbl_tab[i].key, name)) return g_lbl_tab[i].fn;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define GOTO_FRAME_BYTES (64 * 1024)
void rt_goto_transfer(const char *name)
{
    if (!name || !*name) return;
    if (name[0] == '$') {
        DESCR_t iv = NV_GET_fn(name + 1);
        const char *inm = VARVAL_fn(iv);
        if (!inm || !*inm) { fprintf(stderr, "[SNO] transfer to undefined label: $%s (indirect name is null)\n", name + 1); exit(1); }
        rt_goto_transfer(inm);
        return;
    }
    if (!strcmp(name, "END")) return;
    eval_chain_fn fn = rt_label_get_fn(name);
    if (fn) { rt_chain_enter(fn); return; }
    {
        extern void *rt_proc_get_fn(const char *);
        char lname[256]; snprintf(lname, sizeof lname, "LBL__%s", name);
        fn = (eval_chain_fn)rt_proc_get_fn(lname);
        if (fn) { rt_chain_enter(fn); return; }
    }
    {
        DESCR_t d = NV_GET_fn(name);
        if (d.v == DT_C && d.slen == 3) { rt_chain_enter((eval_chain_fn)d.ptr); return; }
    }
    fprintf(stderr, "[SNO] transfer to undefined label: %s\n", name); exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t code(const char *src)
{
    if (!src || !*src) return FAILDESCR;
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void fc_tables_reset(void); fc_tables_reset(); extern void zls_reset(void); zls_reset(); }
    extern tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out);
    extern IR_graph_t *sno_lower_fragment_at(const tree_t *prog, int entry_idx);
    extern const char *sno_stmt_label(const tree_t *s);
    extern int g_frame_active;
    extern void sno_error_quiet_begin(void); extern void sno_error_quiet_end(void); extern const char *sno_error_captured(void); extern const char *g_sno_errtext;
    sno_error_quiet_begin();
    tree_t *prog = sno_parse_string_ast(src, NULL);
    sno_error_quiet_end();
    if (!prog || prog->n == 0) { const char *cap = sno_error_captured(); if (cap) g_sno_errtext = rt_ws_strdup_c(cap); return FAILDESCR; }
    /* BLOCKER-C (s144): a runtime CODE fragment that contains a pattern match lowers through sno_lower_fragment_at,
     * whose sno_pat_collect walk MINTS new PAT$N entries (continuing g_sno_npat past the main-program set) but —
     * unlike lower_sno_stage2 — never turns them into emitted+registered proc_table blobs.  At match time the
     * fragment body calls SNO$MKPAT("PAT$N") → rt_proc_get_fn → NULL → "compiled pattern blob not registered".
     * Capture the pattern + proc watermarks BEFORE lowering; after every fragment body is lowered+emitted, build
     * proc_table thunks for the newly-collected patterns (sno_pat_thunks_build) and emit+register them through the
     * same eval_thunks_emit_from path the EVAL arm uses.  Mirrors main's sno_pat_thunks_build(0) + driver loop. */
    extern int sno_pat_count(void); extern void sno_pat_thunks_build(int p0);
    int pat0 = sno_pat_count();
    int proc0 = g_stage2.proc_count;
    eval_chain_fn first = NULL;
    int k = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t *c = prog->c[i];
        if (!c || c->t != TT_STMT) continue;
        const char *lbl = sno_stmt_label(c);
        if (k == 0 || (lbl && lbl[0])) {
            IR_graph_t *g = sno_lower_fragment_at(prog, k);
            if (!g) return FAILDESCR;
            extern IR_graph_t *g_emit_cfg;
            IR_graph_t *cfg_sv = g_emit_cfg; g_emit_cfg = g;
            int fa = g_frame_active; g_frame_active = 1;
            emit_jmp_entry_for_chain(g);
            eval_chain_fn fn = emit_chain(g->entry, NULL, "code_flat");
            emit_jmp_entry_clear();
            g_frame_active = fa; g_emit_cfg = cfg_sv;
            if (!fn) return FAILDESCR;
            if (k == 0) first = fn;
            if (lbl && lbl[0]) rt_label_set_fn(lbl, (void *)fn);
        }
        k++;
    }
    if (sno_pat_count() > pat0) { sno_pat_thunks_build(pat0); eval_thunks_emit_from(proc0); }
    if (!first) return FAILDESCR;
    DESCR_t d;
    d.v    = DT_C;
    d.slen = 3;
    d.ptr  = (void *)first;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t EXPVAL_fn(DESCR_t expr_d)
{
    if (expr_d.v == DT_E) {
        if (expr_d.slen == 3) {
            eval_chain_fn fn = (eval_chain_fn)expr_d.ptr;
            if (!fn) return FAILDESCR;
            DESCR_t saved = NV_GET_fn(EVAL_TMP);
            rt_chain_enter(fn);
            DESCR_t result = NV_GET_fn(EVAL_TMP);
            NV_SET_fn(EVAL_TMP, saved);
            return result;
        }
        if (expr_d.slen == 1) {
            extern DESCR_t sm_eval_subexpr(int entry_pc);
            int entry_pc = (int)expr_d.i;
            return sm_eval_subexpr(entry_pc);
        }
        if (expr_d.slen == 2) {
            fprintf(stderr, "[SMX] FATAL: eval_code DT_E thunk path used the global value stack, "
                            "which is removed. This SM-era code path is not on Byrd Boxes. "
                            "Aborting (by design).\n");
            abort();
        }
        if (!expr_d.ptr) return FAILDESCR;
        const char *save_Σ = Σ;
        int         save_Ω = Ω;
        int         save_Δ = Δ;
        NAME_ctx_t eval_ctx;
        NAME_ctx_enter(&eval_ctx);
        DESCR_t result = eval_node((tree_t *)expr_d.ptr);
        NAME_ctx_leave();
        Σ = save_Σ;
        Ω = save_Ω;
        Δ = save_Δ;
        return result;
    }
    if (expr_d.v == DT_C) {
        if (expr_d.slen == 3) {
            eval_chain_fn fn = (eval_chain_fn)expr_d.ptr;
            if (!fn) return FAILDESCR;
            rt_chain_enter(fn);
            return NULVCL;
        }
        return NULVCL;
    }
    const char *s = VARVAL_fn(expr_d);
    if (!s || !*s) return NULVCL;
    return eval_expr(s);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t CONVE_fn(DESCR_t str_d)
{
    const char *s = VARVAL_fn(str_d);
    if (!s || !*s) return FAILDESCR;
    eval_chain_fn fn = eval_build_chain(s);
    if (!fn) return FAILDESCR;
    DESCR_t d;
    d.v    = DT_E;
    d.slen = 3;
    d.ptr  = (void *)fn;
    return d;
}
