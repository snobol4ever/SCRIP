#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "core.h"
#include "sil_macros.h"
#include "../parser/snobol4/scrip_cc.h"
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
extern eval_chain_fn  gvar_flat_chain_build(void *g);
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
/*--------------------------------------------------------------------------------------------------------------------*/
static unsigned long eval_cache_hash(const char *s) { unsigned long h = 1469598103934665603UL; while (*s) { h ^= (unsigned char)*s++; h *= 1099511628211UL; } return h; }
/*--------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn eval_cache_get(const char *s) {
    if (g_eval_cache_cap == 0) return NULL;
    unsigned long m = (unsigned long)g_eval_cache_cap - 1;
    for (unsigned long i = eval_cache_hash(s) & m, p = 0; p < (unsigned long)g_eval_cache_cap; p++, i = (i + 1) & m)
        if (!g_eval_cache[i].key) return NULL; else if (strcmp(g_eval_cache[i].key, s) == 0) return g_eval_cache[i].fn;
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void eval_cache_insert_raw(eval_cache_ent_t *tab, int cap, char *key, eval_chain_fn fn) {
    unsigned long m = (unsigned long)cap - 1;
    unsigned long i = eval_cache_hash(key) & m;
    while (tab[i].key) i = (i + 1) & m;
    tab[i].key = key; tab[i].fn = fn;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
    eval_chain_fn fn = gvar_flat_chain_build(g);
    IR_free_dyn(g);
    ast_tree_free_dyn(prog);
    return fn;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t eval_chain_run_capture(eval_chain_fn fn) {
    int64_t eval_frame[512];
    memset(eval_frame, 0, sizeof eval_frame);
    DESCR_t saved = NV_GET_fn(EVAL_TMP);
    rt_eval_run(fn, (void *)eval_frame);
    DESCR_t result = NV_GET_fn(EVAL_TMP);
    NV_SET_fn(EVAL_TMP, saved);
    return result;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_node(tree_t *e)
{
    (void)e;
    fprintf(stderr, "[B0b] BOMB eval_node: AST-walk evaluator deleted; nothing interprets tree_t at runtime\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_expr(const char *src)
{
    if (!src || !*src) return NULVCL;
    tree_t *tree = parse_expr_pat_from_str(src);
    if (!tree) return FAILDESCR;
    return eval_node(tree);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn code_build_chain(const char *s)
{
    if (!s || !*s) return NULL;
    { extern void bb_pool_init(void); bb_pool_init(); }
    extern tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out);
    tree_t *prog = sno_parse_string_ast(s, NULL);
    if (!prog || prog->n == 0) return NULL;
    void *g = lower_snobol4(prog);
    if (!g) return NULL;
    return gvar_flat_chain_build(g);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t code(const char *src)
{
    if (!src || !*src) return FAILDESCR;
    eval_chain_fn fn = code_build_chain(src);
    if (!fn) return FAILDESCR;
    DESCR_t d;
    d.v    = DT_C;
    d.slen = 3;
    d.ptr  = (void *)fn;
    return d;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void run_code_chain(eval_chain_fn fn)
{
    if (!fn) return;
    int64_t code_frame[512];
    memset(code_frame, 0, sizeof code_frame);
    rt_eval_run(fn, (void *)code_frame);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_goto_dyn(const char *name)
{
    if (!name || !*name) return;
    DESCR_t d = NV_GET_fn(name);
    if (d.v == DT_C && d.slen == 3) run_code_chain((eval_chain_fn)d.ptr);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
