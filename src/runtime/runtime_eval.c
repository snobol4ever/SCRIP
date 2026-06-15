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
#define EVAL_TMP "ZZEVALZZ"
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
    if (!g) return NULL;
    return gvar_flat_chain_build(g);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_node(tree_t *e)
{
    (void)e;
    fprintf(stderr, "[B0b] BOMB eval_node: AST-walk evaluator deleted (mode-1 era); nothing interprets tree_t at runtime\n");
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
DESCR_t code(const char *src)
{
    if (!src || !*src) return FAILDESCR;
    extern tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out);
    tree_t *ast = sno_parse_string_ast(src, NULL);
    if (!ast || ast->n == 0) return FAILDESCR;
    DESCR_t d;
    d.v   = DT_C;
    d.ptr = ast;
    d.slen = 0;
    return d;
}
/*--------------------------------------------------------------------------------------------------------------------*/
const char *exec_code(DESCR_t code_block)
{
    if (code_block.v != DT_C || !code_block.ptr) return NULL;
    const tree_t *prog = (const tree_t *)code_block.ptr;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t *s = prog->c[_ci];
        if (!s) continue;
        if (s->t == TT_END) return "";
        int has_eq = stmt_attr_find(s, ":eq") != NULL;
        tree_t *subject     = stmt_attr_expr(stmt_attr_find(s, ":subj"));
        tree_t *pattern     = stmt_attr_expr(stmt_attr_find(s, ":pat"));
        tree_t *replacement = stmt_attr_expr(stmt_attr_find(s, ":repl"));
        if (!pattern && subject && subject->t == TT_SCAN && subject->n == 2) {
            pattern = subject->c[1];
            subject = subject->c[0];
        }
        if (!pattern && subject && subject->t == TT_SEQ && subject->n >= 2) {
            tree_t *first = subject->c[0];
            if (first->t == TT_VAR || first->t == TT_KEYWORD || first->t == TT_QLIT || first->t == TT_INDIRECT) {
                int nc = subject->n - 1;
                tree_t *rest;
                if (nc == 1) { rest = subject->c[1]; }
                else { rest = ast_node_new(TT_SEQ); for (int i = 1; i < subject->n; i++) expr_add_child(rest, subject->c[i]); }
                pattern = rest;
                subject = first;
            }
        }
        const char *goto_u = goto_node_str(stmt_goto_find(s, TT_GOTO_U));
        const char *goto_s = goto_node_str(stmt_goto_find(s, TT_GOTO_S));
        const char *goto_f = goto_node_str(stmt_goto_find(s, TT_GOTO_F));
        DESCR_t subj_val = NULVCL;
        const char *subj_name = NULL;
        if (subject) {
            if (subject->t == TT_VAR && subject->v.sval) {
                subj_name = subject->v.sval;
                subj_val  = NV_GET_fn(subj_name);
            } else {
                subj_val = eval_node(subject);
            }
        }
        int succeeded = 1;
        if (pattern) {
            DESCR_t pat_d = eval_node(pattern);
            if (IS_FAIL_fn(pat_d)) {
                succeeded = 0;
            } else {
                DESCR_t repl_val;
                int has_repl = 0;
                if (has_eq && replacement) {
                    repl_val = eval_node(replacement);
                    has_repl = !IS_FAIL_fn(repl_val);
                }
                succeeded = exec_stmt(
                    subj_name,
                    subj_name ? NULL : &subj_val,
                    pat_d,
                    has_repl ? &repl_val : NULL,
                    has_repl);
            }
        } else if (has_eq && replacement && subj_name) {
            DESCR_t repl_val = eval_node(replacement);
            if (IS_FAIL_fn(repl_val)) {
                succeeded = 0;
            } else {
                NV_SET_fn(subj_name, repl_val);
                succeeded = 1;
            }
        } else if (subject && !pattern && !has_eq) {
            if (IS_FAIL_fn(subj_val)) succeeded = 0;
        }
        if (goto_u || goto_s || goto_f) {
            if (goto_u && *goto_u) return goto_u;
            if (succeeded && goto_s && *goto_s) return goto_s;
            if (!succeeded && goto_f && *goto_f) return goto_f;
        }
    }
    return "";
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
        exec_code(expr_d);
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
