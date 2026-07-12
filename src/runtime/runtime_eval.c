#include <stdio.h>
#include "rt/rt_arena.h"
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include <math.h>
#include "core.h"
#include "sil_macros.h"
#include "../parser/snobol4/scrip_cc.h"
#include "IR.h"
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
extern void           rt_eval_run(eval_chain_fn fn, void *zeta);
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
        eval_cache_ent_t *ntab = (eval_cache_ent_t *)calloc((size_t)ncap, sizeof(eval_cache_ent_t));
        if (!ntab) return;
        for (int k = 0; k < g_eval_cache_cap; k++) if (g_eval_cache[k].key) eval_cache_insert_raw(ntab, ncap, g_eval_cache[k].key, g_eval_cache[k].fn);
        free(g_eval_cache); g_eval_cache = ntab; g_eval_cache_cap = ncap;
    }
    char *key = strdup(s);
    if (!key) return;
    eval_cache_insert_raw(g_eval_cache, g_eval_cache_cap, key, fn);
    g_eval_cache_n++;
}
__asm__(
".text\n"
".globl rt_eval_run\n"
"rt_eval_run:\n"
"  pushq %rbx\n"
"  pushq %r12\n"
"  pushq %r13\n"
"  pushq %r14\n"
"  pushq %r15\n"
"  movq %rdi, %rax\n"
"  movq %rsi, %rdi\n"
"  xorl %esi, %esi\n"
"  call *%rax\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  ret\n"
);
void rt_eval_run(eval_chain_fn fn, void *zeta);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn eval_build_chain(const char *s)
{
    if (!s || !*s) return NULL;
    { extern void bb_pool_init(void); bb_pool_init(); }
    size_t n = strlen(s);
    char *src = (char *)malloc(n + 4);
    if (!src) return NULL;
    snprintf(src, n + 4, "(%s)", s);
    tree_t *e = parse_expr_pat_from_str(src);
    free(src);
    if (!e) return NULL;
    tree_t *var = ast_stmt_new(TT_VAR);
    var->v.sval = strdup(EVAL_TMP);
    tree_t *st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":line", 1));
    ast_push(st, ast_attr_int(":stno", 1));
    ast_push(st, ast_attr_expr(":subj", var));
    ast_push(st, ast_attr_leaf(":eq", ""));
    ast_push(st, ast_attr_expr(":repl", e));
    tree_t *prog = ast_stmt_new(TT_PROGRAM);
    ast_push(prog, st);
    void *g = lower_snobol4(prog);
    if (!g) { ast_tree_free_dyn(prog); return NULL; }
    extern int g_frame_active;
    extern IR_graph_t *g_emit_cfg;
    IR_graph_t *cfg_sv = g_emit_cfg; g_emit_cfg = (IR_graph_t *)g;
    int fa = g_frame_active; g_frame_active = 1;
    eval_chain_fn fn = emit_chain(((IR_graph_t *)g)->entry, NULL, "pat_flat");
    g_frame_active = fa; g_emit_cfg = cfg_sv;
    IR_free_dyn(g);
    ast_tree_free_dyn(prog);
    return fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t eval_chain_run_capture(eval_chain_fn fn) {
    int64_t eval_frame[512];
    memset(eval_frame, 0, sizeof eval_frame);
    DESCR_t saved = NV_GET_fn(EVAL_TMP);
    rt_eval_run(fn, (void *)eval_frame);
    DESCR_t result = NV_GET_fn(EVAL_TMP);
    NV_SET_fn(EVAL_TMP, saved);
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_string_transient(const char *s) {
    if (!s || !*s) return NULVCL;
    eval_chain_fn cached = eval_cache_get(s);
    if (cached) return eval_chain_run_capture(cached);
    size_t mark = bb_pool_mark();
    eval_chain_fn fn = eval_build_chain(s);
    if (!fn) { bb_pool_release(mark); return FAILDESCR; }
    DESCR_t result = eval_chain_run_capture(fn);
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
        lbl_ent_t *nt = (lbl_ent_t *)realloc(g_lbl_tab, (size_t)ncap * sizeof(lbl_ent_t));
        if (!nt) return;
        g_lbl_tab = nt; g_lbl_cap = ncap;
    }
    g_lbl_tab[g_lbl_n].key = strdup(name);
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
static void run_code_chain(eval_chain_fn fn)
{
    if (!fn) return;
    extern int rt_zeta_cstack(void);
    if (rt_zeta_cstack()) { void *frame = alloca((size_t)GOTO_FRAME_BYTES); memset(frame, 0, (size_t)GOTO_FRAME_BYTES); rt_eval_run(fn, frame); return; }
    void *frame = rt_ws_alloc(GOTO_FRAME_BYTES);
    if (!frame) { fprintf(stderr, "[SNO] rt_goto_transfer: transfer frame allocation failed\n"); exit(1); }
    rt_eval_run(fn, frame);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    if (!fn) {
        extern void *rt_proc_get_fn(const char *);
        char lname[256]; snprintf(lname, sizeof lname, "LBL__%s", name);
        fn = (eval_chain_fn)rt_proc_get_fn(lname);
    }
    if (!fn) {
        DESCR_t d = NV_GET_fn(name);
        if (d.v == DT_C && d.slen == 3) fn = (eval_chain_fn)d.ptr;
    }
    if (!fn) { fprintf(stderr, "[SNO] transfer to undefined label: %s\n", name); exit(1); }
    run_code_chain(fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t code(const char *src)
{
    if (!src || !*src) return FAILDESCR;
    { extern void bb_pool_init(void); bb_pool_init(); }
    extern tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out);
    extern IR_graph_t *sno_lower_fragment_at(const tree_t *prog, int entry_idx);
    extern const char *sno_stmt_label(const tree_t *s);
    extern int g_frame_active;
    tree_t *prog = sno_parse_string_ast(src, NULL);
    if (!prog || prog->n == 0) return FAILDESCR;
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
            eval_chain_fn fn = emit_chain(g->entry, NULL, "code_flat");
            g_frame_active = fa; g_emit_cfg = cfg_sv;
            if (!fn) return FAILDESCR;
            if (k == 0) first = fn;
            if (lbl && lbl[0]) rt_label_set_fn(lbl, (void *)fn);
        }
        k++;
    }
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
            int64_t eval_frame[512];
            memset(eval_frame, 0, sizeof eval_frame);
            DESCR_t saved = NV_GET_fn(EVAL_TMP);
            rt_eval_run(fn, (void *)eval_frame);
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
            run_code_chain(fn);
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
