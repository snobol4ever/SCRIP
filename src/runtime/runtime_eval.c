#include <stdio.h>
#include "rt/rt_arena.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "core.h"
#include "keywords.h"
#include "sil_macros.h"
#include "../parsers/snobol4/scrip_cc.h"
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
#define EVAL_TMP_MARKED "EVAL$"
#define EVAL_TMP_LEGACY "ZZEVALZZ"
#define EVAL_TMP eval_tmp_name()
typedef struct { char *key; eval_chain_fn fn; } eval_cache_ent_t;
static eval_cache_ent_t *g_eval_cache = NULL;
static int               g_eval_cache_n = 0;
static int               g_eval_cache_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *eval_tmp_name(void)
    { static int m = -1; if (m < 0) { const char *e = getenv("SCRIP_EVAL_TMP_MARK"); m = (e && e[0] == '0') ? 0 : 1; } return m ? EVAL_TMP_MARKED : EVAL_TMP_LEGACY; }
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
"  movq g_rtcc_on@GOTPCREL(%rip), %r10\n"
"  cmpb $0, (%r10)\n"
"  je 2f\n"
"  movq rtccb@GOTPCREL(%rip), %r10\n"
"  movq 24(%r10), %rsi\n"
"  movq 32(%r10), %rdi\n"
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
__asm__(
".text\n"
".globl rt_chain_enter_v\n"
"rt_chain_enter_v:\n"
"  pushq %rbx\n"
"  pushq %r12\n"
"  pushq %r13\n"
"  pushq %r14\n"
"  pushq %r15\n"
"  movq %rdi, %rax\n"
"  leaq 3f(%rip), %rcx\n"
"  movq %rcx, %rdx\n"
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
"  subq $8, %rsp\n"
"  pushq %rcx\n"
"  jmp *%rax\n"
"3:\n"
"  addq $8, %rsp\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  ret\n"
);
void rt_chain_enter_v(eval_chain_fn fn);
void rt_chain_enter(eval_chain_fn fn);
int g_rt_fragment_emit = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    g_rt_fragment_emit = 1;
    int b1c = 1; { const char *_be = getenv("SCRIP_B1C_PARITY"); if (_be && *_be == '0') b1c = 0; }
    int b1cland = 1; { const char *_le = getenv("SCRIP_B1C_LAND"); if (_le && *_le == '0') b1cland = 0; }
    for (int pi = pc0; pi < g_stage2.proc_count; pi++) {
        const char *pname = g_stage2.proc_table[pi].name;
        int idx = g_stage2.proc_table[pi].bb_idx;
        if (!pname || idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) continue;
        ir_drive_slot_assign(g_stage2.bbp.table[idx]);
        g_emit_cfg = g_stage2.bbp.table[idx];
        g_gen_proc_active = g_stage2.proc_table[pi].is_generator;
        if (b1c) { extern void rt_proc_set_jmpentry(const char *, int); rt_proc_set_jmpentry(pname, strncmp(pname, "gram__", 6) != 0); }
        if (b1c) { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = g_stage2.bbp.table[idx]; g_flat_frame_floor = 0;
            if (_pg && _pg->entry && ((_pg->entry->op == IR_DEFINE && IR_LIT(_pg->entry).ival == 3) || _pg->entry->op == IR_GOTO_DEFERRED)) {
                for (int _mi = 0; _mi < g_stage2.proc_count; _mi++) if (g_stage2.proc_table[_mi].name && !strcmp(g_stage2.proc_table[_mi].name, "main")) {
                    int _mx = g_stage2.proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < g_stage2.bbp.count && g_stage2.bbp.table[_mx]) g_flat_frame_floor = zls_g_region(g_stage2.bbp.table[_mx]); break; }
                if (g_flat_frame_floor <= 0 && b1cland) g_flat_frame_floor = zls_g_region(_pg); } }
        { extern int emit_jmp_entry_for_patproc(const char*, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int);
          int _isp = emit_jmp_entry_for_patproc(pname, g_stage2.bbp.table[idx]); if (!_isp) emit_jmp_entry_for_proc(pname, g_stage2.proc_table[pi].dyn_scope, g_stage2.proc_table[pi].is_generator, g_stage2.bbp.table[idx]);
          if (b1c) g_flat_dc_np = (!_isp && rt_pl_dc_ok(pname, g_stage2.proc_table[pi].nparams)) ? g_stage2.proc_table[pi].nparams : -1; }
        char _m3pfx[300]; snprintf(_m3pfx, sizeof _m3pfx, "proc_%s", pname);
        eval_chain_fn pfn = emit_chain(g_stage2.bbp.table[idx]->entry, NULL, _m3pfx);
        if (pfn) rt_proc_set_fn(pname, pfn);
        { extern void bb_ab_seal_entry_cells(const char *, void *, int); if (pfn) bb_ab_seal_entry_cells(pname, (void *)pfn, 1); }
        if (b1c && pfn) { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }
        if (b1c && pfn) { extern int g_last_flat_zstatic; extern void rt_proc_set_zstatic(const char *, int); rt_proc_set_zstatic(pname, g_last_flat_zstatic); }
        if (b1c && pfn) { extern int g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform; extern void emit_patzeta_register(const char *, int, int, int); emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform); }
        if (b1c && pfn) { extern long g_last_dc_off; extern void rt_proc_set_dcfn(const char *, void *); if (g_last_dc_off >= 0) rt_proc_set_dcfn(pname, (void *)((char *)pfn + g_last_dc_off)); }
        emit_jmp_entry_clear();
    }
    g_rt_fragment_emit = 0;
    g_gen_proc_active = ga; g_frame_active = fa; g_emit_cfg = cfg_sv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn eval_build_chain(const char *s)
{
    if (!s || !*s) return NULL;
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void fc_tables_reset(void); fc_tables_reset(); extern void zls_reset(void); zls_reset(); extern void bb_src_reset(void); bb_src_reset(); }
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
    { static int _cs = -1; if (_cs < 0) { const char * e = getenv("SCRIP_MON_CHAIN_STNO"); _cs = (e && e[0] == '1') ? 1 : 0; }
      ast_push(st, ast_attr_int(":stno", _cs ? 1 : 0)); }
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
    if (sno_pat_count() > pat0) sno_pat_thunks_build(pat0);
    extern int g_frame_active;
    extern IR_graph_t *g_emit_cfg;
    IR_graph_t *cfg_sv = g_emit_cfg; g_emit_cfg = (IR_graph_t *)g;
    int fa = g_frame_active; g_frame_active = 1;
    emit_jmp_entry_for_chain((IR_graph_t *)g);
    g_rt_fragment_emit = 1;
    eval_chain_fn fn = emit_chain(((IR_graph_t *)g)->entry, NULL, "pat_flat");
    g_rt_fragment_emit = 0;
    emit_jmp_entry_clear();
    g_frame_active = fa; g_emit_cfg = cfg_sv;
    eval_thunks_emit_from(pc0);
    IR_free_dyn(g);
    ast_tree_free_dyn(prog);
    return fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((noinline))
static void eval_chain_enter_only(eval_chain_fn fn) {
    static int _rv = -1; if (_rv < 0) { const char * e = getenv("SCRIP_EVAL_RET"); _rv = (e && *e == '0') ? 0 : 1; }
    if (_rv) { rt_chain_enter_v(fn); return; }
    rt_chain_enter(fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t eval_retain_budget(void) { static long v = -1; if (v < 0) { const char *e = getenv("SCRIP_EVAL_RETAIN"); v = (e && *e) ? atol(e) : -1; } return v < 0 ? ~(size_t)0 : (size_t)v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_chain_run_guarded(eval_chain_fn fn) {
    extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n;
    static int _ef = -1; if (_ef < 0) { const char *e = getenv("SCRIP_EVAL_FAILS"); _ef = (e && *e == '0') ? 0 : 1; }
    if (!_ef) { eval_chain_enter_only(fn); return 1; }
    int my = g_core_errjmp_n++; long esv = g_error; g_error = -1;
    if (setjmp(g_core_errjmp_stk[my])) { g_core_errjmp_n = my; g_error = esv; return 0; }
    eval_chain_enter_only(fn);
    g_core_errjmp_n = my; g_error = esv; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_string_transient(const char *s) {
    if (!s || !*s) return NULVCL;
    eval_chain_fn cached = eval_cache_get(s);
    if (cached) {
        DESCR_t saved = NV_GET_fn(EVAL_TMP);
        int ok = eval_chain_run_guarded(cached);
        DESCR_t result = ok ? NV_GET_fn(EVAL_TMP) : FAILDESCR;
        NV_SET_fn(EVAL_TMP, saved);
        return result;
    }
    size_t mark = bb_pool_mark();
    eval_chain_fn fn = eval_build_chain(s);
    if (!fn) { bb_pool_release(mark); return FAILDESCR; }
    DESCR_t saved = NV_GET_fn(EVAL_TMP);
    int ok = eval_chain_run_guarded(fn);
    DESCR_t result = ok ? NV_GET_fn(EVAL_TMP) : FAILDESCR;
    NV_SET_fn(EVAL_TMP, saved);
    if (mark < eval_retain_budget()) eval_cache_put(s, fn);
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
void *rt_goto_resolve(const char *name)
{
    if (!name || !*name) return NULL;
    if (name[0] == '$') {
        DESCR_t iv = NV_GET_fn(name + 1);
        const char *inm = rt_sno_indirect_name(iv);
        if (!inm || !*inm) { fprintf(stderr, "[SNO] transfer to undefined label: $%s (indirect name is null)\n", name + 1); exit(1); }
        return rt_goto_resolve(inm);
    }
    if (name[0] == '<') {
        DESCR_t cv = NV_GET_fn(name + 1);
        if (cv.v != DT_C || cv.slen != 3 || !cv.ptr) { fprintf(stderr, "[SNO] goto operand in direct goto is not code\n"); exit(1); }
        return cv.ptr;
    }
    if (!strcmp(name, "END")) return NULL;
    { eval_chain_fn fn = rt_label_get_fn(name); if (fn) return (void *)fn; }
    {
        extern void *rt_proc_get_fn(const char *);
        char lname[256]; snprintf(lname, sizeof lname, "LBL__%s", name);
        eval_chain_fn fn = (eval_chain_fn)rt_proc_get_fn(lname);
        if (fn) return (void *)fn;
    }
    { extern void core_runtime_error(int code, const char *msg); char eb[288]; snprintf(eb, sizeof eb, "transfer to undefined label: %s", name); core_runtime_error(38, eb); }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_goto_transfer(const char *name)
{
    void *fn = rt_goto_resolve(name);
    if (fn) rt_chain_enter((eval_chain_fn)fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t code(const char *src)
{
    if (!src || !*src) return FAILDESCR;
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void fc_tables_reset(void); fc_tables_reset(); extern void zls_reset(void); zls_reset(); extern void bb_src_reset(void); bb_src_reset(); }
    extern tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out);
    extern IR_graph_t *sno_lower_fragment_at(const tree_t *prog, int entry_idx);
    extern const char *sno_stmt_label(const tree_t *s);
    extern int g_frame_active;
    extern void sno_error_quiet_begin(void); extern void sno_error_quiet_end(void); extern const char *sno_error_captured(void); extern const char *g_sno_errtext;
    sno_error_quiet_begin();
    tree_t *prog = sno_parse_string_ast(src, NULL);
    sno_error_quiet_end();
    if (!prog || prog->n == 0) { const char *cap = sno_error_captured(); if (cap) g_sno_errtext = rt_ws_strdup_c(cap); return FAILDESCR; }
    extern int sno_pat_count(void); extern void sno_pat_thunks_build(int p0);
    extern int sno_expr_mark(void); extern void sno_expr_thunks_build(int x0);
    int pat0 = sno_pat_count();
    int expr0 = sno_expr_mark();
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
    { int patn = sno_pat_count(); const char *ks = getenv("SCRIP_CODE_THUNKS");
      if (!(ks && *ks == '0')) sno_expr_thunks_build(expr0);
      if (patn > pat0) sno_pat_thunks_build(pat0);
      if ((ks && *ks == '0') ? (patn > pat0) : 1) eval_thunks_emit_from(proc0); }
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
