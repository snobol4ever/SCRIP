#include <stdio.h>
#include "rt/rt_arena.h"
#include "rt/gc_heap.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "core.h"
#include "dtp.h"
#include "keywords.h"
#include "sil_macros.h"
#include "../parsers/snobol4/scrip_cc.h"
#include "IR.h"
#include "pin_va.h"
_Static_assert(RT_DCAP_TOP == 0x70000000UL, "rt_chain_enter/rt_chain_enter_v seed r12 from the absolute slot 0x70000000 written literally in their asm, exactly as rt_outer_call and main's prologue do; if pin_va.h moves the pin the trampolines read a DEAD page and hand generated code a zero pend top");
_Static_assert(RT_DCAP_ISLAND_BYTES == 67108864UL, "rt_chain_enter/rt_chain_enter_v bound-test r12 against base+67108864 written literally in their asm to tell a LIVE pend top from C's own callee-saved value; if the island resizes the test admits or rejects the wrong halves of it");
#include "stage2.h"
#include "core/core_errjmp.h"
extern void *rt_wsb_alloc(size_t n);
extern const char *Σ;
extern int         Ω;
extern int         Δ;
void rt_proc_set_rest_kind(const char *name, int kind);
void rt_proc_set_named_rest(const char *name, int slot);
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
typedef struct { char *key; eval_chain_fn fn; long gen; } eval_cache_ent_t;
static eval_cache_ent_t *g_eval_cache = NULL;
static int               g_eval_cache_n = 0;
static int               g_eval_cache_cap = 0;
typedef struct { DESCR_t saved; DESCR_t res; char *key; int depth; int made; int keep; int thunks; int opened; long esv; size_t mark; size_t built; eval_chain_fn fn; } eval_frame_t;
static eval_frame_t     *g_eval_frames = NULL;
static int               g_eval_frames_n = 0;
static int               g_eval_frames_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *eval_tmp_name(void)
    { static int m = -1; if (m < 0) { const char *e = getenv("SCRIP_EVAL_TMP_MARK"); m = (e && e[0] == '0') ? 0 : 1; } return m ? EVAL_TMP_MARKED : EVAL_TMP_LEGACY; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern size_t bb_pool_used(void);
extern size_t bb_pool_free(void);
extern void   rt_code_pool_overflow(unsigned long long used_kb, unsigned long long cap_kb);
void rt_code_pool_check(void) {
    size_t fr = bb_pool_free(), cap = bb_pool_used() + fr;
    if (cap && fr < (4UL << 20)) rt_code_pool_overflow((unsigned long long)(bb_pool_used() >> 10), (unsigned long long)(cap >> 10));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_result_is_plain(DESCR_t d) {
    if (IS_FAIL(d)) return 1;
    switch ((int) d.v) { case DT_SNUL: case DT_S: case DT_I: case DT_R: case DT_BIG: case DT_A: case DT_T: case DT_N: case DT_K: case DT_DATA: case DT_FAIL: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned long eval_cache_hash(const char *s) { unsigned long h = 1469598103934665603UL; while (*s) { h ^= (unsigned char)*s++; h *= 1099511628211UL; } return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_cache_ent_t *eval_cache_slot(const char *s) {
    if (g_eval_cache_cap == 0) return NULL;
    unsigned long m = (unsigned long)g_eval_cache_cap - 1;
    for (unsigned long i = eval_cache_hash(s) & m, p = 0; p < (unsigned long)g_eval_cache_cap; p++, i = (i + 1) & m)
        if (!g_eval_cache[i].key) return NULL; else if (strcmp(g_eval_cache[i].key, s) == 0) return &g_eval_cache[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn eval_cache_get(const char *s) { eval_cache_ent_t *e = eval_cache_slot(s); return e ? e->fn : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eval_cache_insert_raw(eval_cache_ent_t *tab, int cap, char *key, eval_chain_fn fn, long gen) {
    unsigned long m = (unsigned long)cap - 1;
    unsigned long i = eval_cache_hash(key) & m;
    while (tab[i].key) i = (i + 1) & m;
    tab[i].key = key; tab[i].fn = fn; tab[i].gen = gen;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eval_cache_put(char *key, eval_chain_fn fn) {
    eval_cache_ent_t *have = eval_cache_slot(key);
    if (have) { if (fn) have->fn = fn; return; }
    extern long rt_gc_runs_count(void);
    long now = rt_gc_runs_count();
    if (g_eval_cache_cap == 0 || (g_eval_cache_n + 1) * 2 > g_eval_cache_cap) {
        int keep = 0, ncap = 16;
        for (int k = 0; k < g_eval_cache_cap; k++) if (g_eval_cache[k].key && (g_eval_cache[k].fn || g_eval_cache[k].gen == now)) keep++;
        while ((keep + 1) * 4 > ncap) ncap *= 2;
        eval_cache_ent_t *ntab = (eval_cache_ent_t *)rt_wsb_alloc((size_t)ncap * sizeof(eval_cache_ent_t));
        if (!ntab) return;
        memset(ntab, 0, (size_t)ncap * sizeof(eval_cache_ent_t));
        for (int k = 0; k < g_eval_cache_cap; k++) if (g_eval_cache[k].key && (g_eval_cache[k].fn || g_eval_cache[k].gen == now)) eval_cache_insert_raw(ntab, ncap, g_eval_cache[k].key, g_eval_cache[k].fn, g_eval_cache[k].gen);
        g_eval_cache = ntab; g_eval_cache_cap = ncap; g_eval_cache_n = keep;
    }
    eval_cache_insert_raw(g_eval_cache, g_eval_cache_cap, key, fn, now);
    g_eval_cache_n++;
}
__asm__(
".text\n"
".globl rt_chain_enter\n"
"rt_chain_enter:\n"
"  pushq %rbx\n"
"  pushq %r12\n"
"  pushq %r14\n"
"  subq $16, %rsp\n"
"  movl $2, (%rsp)\n"
"  movl %r15d, 4(%rsp)\n"
"  movq %r13, 8(%rsp)\n"
"  movq %rdi, %rax\n"
"  leaq 1f(%rip), %rcx\n"
"  movq %rcx, %rdx\n"
"  leaq 9f(%rip), %r10\n"
"  pushq %r10\n"
"  pushq %r10\n"
"  movq g_dcap_base@GOTPCREL(%rip), %r10\n"
"  movq (%r10), %r10\n"
"  testq %r10, %r10\n"
"  jz  5f\n"
"  cmpq %r10, %r12\n"
"  jb  5f\n"
"  addq $67108864, %r10\n"
"  cmpq %r10, %r12\n"
"  jb  6f\n"
"5:\n"
"  movq 0x70000000, %r12\n"
"6:\n"
"  movq Σ@GOTPCREL(%rip), %r10\n"
"  movq (%r10), %r13\n"
"  movq Σlen@GOTPCREL(%rip), %r10\n"
"  movl (%r10), %r15d\n"
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
"9:\n"
"  call rt_kw_return_level_zero@PLT\n"
"  ud2\n"
".globl rt_chain_enter_ret\n"
"rt_chain_enter_ret:\n"
"1:\n"
"  addq $16, %rsp\n"
"  movl 4(%rsp), %r15d\n"
"  movq 8(%rsp), %r13\n"
"  addq $16, %rsp\n"
"  popq %r14\n"
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
"  pushq %r14\n"
"  subq $16, %rsp\n"
"  movl $2, (%rsp)\n"
"  movl %r15d, 4(%rsp)\n"
"  movq %r13, 8(%rsp)\n"
"  movq %rdi, %rax\n"
"  leaq 3f(%rip), %rcx\n"
"  movq %rcx, %rdx\n"
"  movq g_dcap_base@GOTPCREL(%rip), %r10\n"
"  movq (%r10), %r10\n"
"  testq %r10, %r10\n"
"  jz  7f\n"
"  cmpq %r10, %r12\n"
"  jb  7f\n"
"  addq $67108864, %r10\n"
"  cmpq %r10, %r12\n"
"  jb  8f\n"
"7:\n"
"  movq 0x70000000, %r12\n"
"8:\n"
"  movq Σ@GOTPCREL(%rip), %r10\n"
"  movq (%r10), %r13\n"
"  movq Σlen@GOTPCREL(%rip), %r10\n"
"  movl (%r10), %r15d\n"
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
".globl rt_chain_enter_v_ret\n"
"rt_chain_enter_v_ret:\n"
"3:\n"
"  addq $8, %rsp\n"
"  movl 4(%rsp), %r15d\n"
"  movq 8(%rsp), %r13\n"
"  addq $16, %rsp\n"
"  popq %r14\n"
"  popq %r12\n"
"  popq %rbx\n"
"  ret\n"
);
void rt_chain_enter_v(eval_chain_fn fn);
void rt_chain_enter(eval_chain_fn fn);
int g_rt_fragment_emit = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_thunks_emit_from(int pc0)
{
    int emit_failed = 0;
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
        { extern void zls_forget_graph_nodes(const IR_graph_t *); zls_forget_graph_nodes(g_stage2.bbp.table[idx]); }
        ir_drive_slot_assign(g_stage2.bbp.table[idx]);
        g_emit_cfg = g_stage2.bbp.table[idx];
        g_gen_proc_active = g_stage2.proc_table[pi].is_generator;
        if (b1c) { extern void rt_proc_set_jmpentry(const char *, int); rt_proc_set_jmpentry(pname, strncmp(pname, "gram__", 6) != 0); }
        if (b1c) { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); IR_graph_t *_pg = g_stage2.bbp.table[idx]; g_flat_frame_floor = 0;
            if (_pg && _pg->entry && ((_pg->entry->op == IR_DEFINE && IR_LIT(_pg->entry).ival == 3) || _pg->entry->op == IR_GOTO_DEFERRED)) {
                for (int _mi = 0; _mi < g_stage2.proc_count; _mi++) if (g_stage2.proc_table[_mi].name && !strcmp(g_stage2.proc_table[_mi].name, "main")) {
                    int _mx = g_stage2.proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < g_stage2.bbp.count && g_stage2.bbp.table[_mx]) g_flat_frame_floor = zls_g_region(g_stage2.bbp.table[_mx]); break; }
                if (g_flat_frame_floor <= 0 && b1cland) g_flat_frame_floor = zls_g_region(_pg); } }
        int _ispe = 0;
        { extern int emit_jmp_entry_for_patproc(int, IR_graph_t*); extern int emit_jmp_entry_for_proc(const char*, int, int, IR_graph_t*); extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int);
          int _isp = emit_jmp_entry_for_patproc(g_stage2.proc_table[pi].thunk_kind, g_stage2.bbp.table[idx]); if (!_isp) emit_jmp_entry_for_proc(pname, g_stage2.proc_table[pi].dyn_scope, g_stage2.proc_table[pi].is_generator, g_stage2.bbp.table[idx]);
          if (b1c) g_flat_dc_np = (!_isp && rt_pl_dc_ok(pname, g_stage2.proc_table[pi].nparams)) ? g_stage2.proc_table[pi].nparams : -1; _ispe = _isp; }
        char _m3pfx[strlen(pname) + 6]; snprintf(_m3pfx, sizeof _m3pfx, "proc_%s", pname);
        eval_chain_fn pfn = emit_chain(g_stage2.bbp.table[idx]->entry, NULL, _m3pfx);
        if (!pfn) emit_failed = 1;
        { extern void emit_gc_tables_register(const void *); emit_gc_tables_register((const void *) pfn); }
        if (pfn) rt_proc_set_fn(pname, pfn);
        { extern int g_last_flat_frame_bytes, g_last_flat_zstatic; extern void bb_thunk_rec_fill(const char *, void *, int32_t, int32_t); if (pfn && _ispe) bb_thunk_rec_fill(pname, (void *)pfn, b1c ? g_last_flat_frame_bytes : 0, b1c ? g_last_flat_zstatic : 0); }
        { extern void bb_ab_seal_entry_cells(const char *, void *, int); if (pfn) bb_ab_seal_entry_cells(pname, (void *)pfn, 1); }
        if (b1c && pfn) { extern int g_last_flat_frame_bytes; extern void rt_proc_set_frame_bytes(const char *, int); rt_proc_set_frame_bytes(pname, g_last_flat_frame_bytes); }
        if (b1c && pfn) { extern int g_last_flat_zstatic; extern void rt_proc_set_zstatic(const char *, int); rt_proc_set_zstatic(pname, g_last_flat_zstatic); }
        if (b1c && pfn) { extern int g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform; extern void emit_patzeta_register(const char *, int, int, int); emit_patzeta_register(pname, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform); }
        if (b1c && pfn) { extern long g_last_dc_off; extern void rt_proc_set_dcfn(const char *, void *); if (g_last_dc_off >= 0) rt_proc_set_dcfn(pname, (void *)((char *)pfn + g_last_dc_off)); }
        emit_jmp_entry_clear();
    }
    g_rt_fragment_emit = 0;
    g_gen_proc_active = ga; g_frame_active = fa; g_emit_cfg = cfg_sv;
    return emit_failed;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long sno_text_illegal_at(const char *s)
{
    char q = 0;
    for (long i = 0; s && s[i]; i++) {
        char c = s[i];
        if (q) { if (c == q) q = 0; continue; }
        if (c == '\'' || c == '"') { q = c; continue; }
        if (c == '\n' || c == '\v' || c == '\f' || c == '\r') return i;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_top_comma(const char *s)
{
    int d = 0; char q = 0;
    for (; *s; s++) {
        if (q) { if (*s == q) q = 0; continue; }
        if (*s == '\'' || *s == '"') q = *s;
        else if (*s == '(' || *s == '<' || *s == '[') d++;
        else if ((*s == ')' || *s == '>' || *s == ']') && d > 0) d--;
        else if (*s == ',' && d == 0) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eval_stmt_strings_drop(tree_t *st) {
    extern void ast_attr_strings_drop(tree_t *a, int leaf);
    for (int i = 0; st && i < st->n; i++) { tree_t *a = st->c[i]; if (a && a->t == TT_ATTR && a->v.sval) ast_attr_strings_drop(a, strcmp(a->v.sval, ":subj") && strcmp(a->v.sval, ":repl")); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static eval_chain_fn eval_build_chain(const char *s, int *pe, const char **pm, int *thunks)
{
    *thunks = 0;
    if (!s || !*s) return NULL;
    if (eval_top_comma(s)) { extern const char *g_sno_errtext; g_sno_errtext = "syntax error: invalid use of comma"; return NULL; }
    if (sno_text_illegal_at(s) >= 0) { extern const char *g_sno_errtext; g_sno_errtext = "syntax error: illegal character"; return NULL; }
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void fc_tables_reset(void); fc_tables_reset(); extern void zls_reset(void); zls_reset(); extern void bb_src_reset(void); bb_src_reset(); }
    size_t n = strlen(s);
    char *src = (char *)rt_wsb_alloc(n + 4);
    if (!src) return NULL;
    snprintf(src, n + 4, "(%s)", s);
    extern void sno_error_quiet_begin(void); extern void sno_error_quiet_end(void); extern const char *sno_error_captured(void); extern const char *g_sno_errtext;
    sno_error_quiet_begin();
    tree_t *e = parse_expr_pat_from_str(src);
    sno_error_quiet_end();
    if (!e || sno_error_captured()) { const char *cap = sno_error_captured(); if (cap) g_sno_errtext = rt_heap_strdup_c(cap); return NULL; }
    { extern int sno_preeval_expr(const tree_t *, const char **); *pe = sno_preeval_expr(e, pm); if (*pe) return NULL; }
    tree_t *var = ast_stmt_new(TT_VAR);
    var->v.sval = (char *)EVAL_TMP;
    tree_t *st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":line", 1));
    ast_push(st, ast_attr_int(":evalstmt", 1));
#if RT_DIAG
    { static int _cs = -1; if (_cs < 0) { const char * e = getenv("SCRIP_MON_CHAIN_STNO"); _cs = (e && e[0] == '1') ? 1 : 0; }
      ast_push(st, ast_attr_int(":stno", _cs ? 1 : 0)); }
#else
    ast_push(st, ast_attr_int(":stno", 0));
#endif
    ast_push(st, ast_attr_int(":nocount", 1));
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
    if (!g) { eval_stmt_strings_drop(st); ast_tree_free_dyn(prog); return NULL; }
    sno_expr_thunks_build(xm);
    if (sno_pat_count() > pat0) sno_pat_thunks_build(pat0);
    extern int g_frame_active;
    extern IR_graph_t *g_emit_cfg;
    IR_graph_t *cfg_sv = g_emit_cfg; g_emit_cfg = (IR_graph_t *)g;
    int fa = g_frame_active; g_frame_active = 1;
    emit_jmp_entry_for_chain((IR_graph_t *)g);
    g_rt_fragment_emit = 1;
    eval_chain_fn fn = emit_chain(((IR_graph_t *)g)->entry, NULL, "pat_flat");
    { extern void emit_gc_tables_register(const void *); emit_gc_tables_register((const void *) fn); }
    g_rt_fragment_emit = 0;
    emit_jmp_entry_clear();
    g_frame_active = fa; g_emit_cfg = cfg_sv;
    *thunks = sno_pat_count() > pat0 || sno_expr_mark() > xm || g_stage2.proc_count > pc0;
    if (eval_thunks_emit_from(pc0)) fn = NULL;
    IR_free_dyn(g);
    eval_stmt_strings_drop(st);
    ast_tree_free_dyn(prog);
    return fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_c2bb_hit(const char *site, const char *name);
int g_eval_ret_v = 1;
__attribute__((constructor)) static void eval_ret_init(void) { const char *e = getenv("SCRIP_EVAL_RET"); g_eval_ret_v = (e && *e == '0') ? 0 : 1; }
extern void eval_chain_enter_only(eval_chain_fn fn);
__asm__(
".text\n"
".globl eval_chain_enter_only\n"
"eval_chain_enter_only:\n"
"  movq g_eval_ret_v@GOTPCREL(%rip), %rax\n"
"  cmpl $0, (%rax)\n"
"  je 1f\n"
#if RT_DIAG
"  pushq %rdi\n"
"  leaq 2f(%rip), %rdi\n"
"  leaq 4f(%rip), %rsi\n"
"  call rt_c2bb_hit\n"
"  popq %rdi\n"
#endif
"  jmp rt_chain_enter_v\n"
"1:\n"
#if RT_DIAG
"  pushq %rdi\n"
"  leaq 3f(%rip), %rdi\n"
"  leaq 4f(%rip), %rsi\n"
"  call rt_c2bb_hit\n"
"  popq %rdi\n"
#endif
"  jmp rt_chain_enter\n"
#if RT_DIAG
".section .rodata\n"
"2:  .asciz \"chain.eval.v\"\n"
"3:  .asciz \"chain.eval\"\n"
"4:  .asciz \"?\"\n"
".text\n"
#endif
);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t eval_retain_budget(void) { static long v = -1; if (v < 0) { const char *e = getenv("SCRIP_EVAL_RETAIN"); v = (e && *e) ? atol(e) : -1; } return v < 0 ? ~(size_t)0 : (size_t)v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_guard_on(void) { static int _ef = -1; if (_ef < 0) { const char *e = getenv("SCRIP_EVAL_FAILS"); _ef = (e && *e == '0') ? 0 : 1; } return _ef; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_open_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_EVAL_OPEN"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_chain_run_guarded(eval_chain_fn fn) {
    extern int g_core_errjmp_n;
#if RT_DIAG
    if (!eval_guard_on()) { rt_c2bb_hit("chain.eval.unguarded", "?"); eval_chain_enter_only(fn); return 1; }
#else
    if (!eval_guard_on()) { eval_chain_enter_only(fn); return 1; }
#endif
    core_errjmp_t ej; int my = g_core_errjmp_n; core_errjmp_push(&ej); g_core_errjmp_n = my + 1; long esv = g_error == G_ERROR_EVAL_STAGE ? 0 : g_error; g_error = G_ERROR_EVAL_STAGE;
    if (setjmp(ej.jb)) { core_errjmp_pop(&ej, my); g_error = esv; return 0; }
#if RT_DIAG
    rt_c2bb_hit("chain.eval.guarded", "?");
#endif
    eval_chain_enter_only(fn);
    core_errjmp_pop(&ej, my); g_error = esv; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_frame_push(DESCR_t saved, const char *key_src) {
    extern int g_core_errjmp_n;
    while (g_eval_frames_n > 0 && g_eval_frames[g_eval_frames_n - 1].depth > g_core_errjmp_n) g_eval_frames_n--;
    if (g_eval_frames_n >= g_eval_frames_cap) {
        int nc = g_eval_frames_cap ? g_eval_frames_cap * 2 : 8;
        eval_frame_t *nf = (eval_frame_t *)rt_wsb_realloc(g_eval_frames, (size_t)nc * sizeof(eval_frame_t));
        if (!nf) return -1;
        g_eval_frames = nf; g_eval_frames_cap = nc;
    }
    char *key = key_src ? rt_heap_strdup_c(key_src) : NULL;
    if (key_src && !key) return -1;
    eval_frame_t *f = &g_eval_frames[g_eval_frames_n];
    f->saved = saved; f->res = FAILDESCR; f->key = key; f->depth = g_core_errjmp_n;
    f->made = 0; f->keep = 0; f->thunks = 0; f->opened = 0; f->esv = 0; f->mark = 0; f->built = 0; f->fn = NULL;
    return g_eval_frames_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_sb_numok(const char *t, long len) {
    long i = 0, nd = 0, ne = 0; int64_t iv = 0; int ov = 0; char ex = 0;
    while (i < len && t[i] >= '0' && t[i] <= '9') { if (__builtin_mul_overflow(iv, 10, &iv) || __builtin_add_overflow(iv, (int64_t)(t[i] - '0'), &iv)) ov = 1; i++; nd++; }
    if (!nd) return 0;
    if (i == len) return !ov;
    if (t[i] == '.') { i++; while (i < len && t[i] >= '0' && t[i] <= '9') i++; }
    if (i < len && (t[i] == 'e' || t[i] == 'E' || t[i] == 'd' || t[i] == 'D')) { ex = t[i]; i++; if (i < len && (t[i] == '+' || t[i] == '-')) i++; while (i < len && t[i] >= '0' && t[i] <= '9') { i++; ne++; } if (!ne) return 0; }
    if (i != len) return 0;
    if (ex != 'd' && ex != 'D') return isfinite(strtod(t, (char **)0)) ? 1 : 0;
    { const char *e = t; while (*e != 'd' && *e != 'D') e++; return isfinite(strtod(t, (char **)0) * pow(10.0, strtod(e + 1, (char **)0))) ? 1 : 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_sb_fail(int c, int *code, const char **msg) {
    *code = c;
    switch (c) {
    case 220: *msg = "syntax error: missing operator"; break;
    case 221: *msg = "syntax error: missing operand"; break;
    case 222: *msg = "syntax error: invalid use of left bracket"; break;
    case 223: *msg = "syntax error: invalid use of comma"; break;
    case 224: *msg = "syntax error: unbalanced right parenthesis"; break;
    case 225: *msg = "syntax error: unbalanced right bracket"; break;
    case 226: *msg = "syntax error: missing right paren"; break;
    case 229: *msg = "syntax error: missing right array bracket"; break;
    case 230: *msg = "syntax error: illegal character"; break;
    case 231: *msg = "syntax error: invalid numeric item"; break;
    case 232: *msg = "syntax error: unmatched string quote"; break;
    case 212: *msg = "syntax error: value used where name is required"; break;
    case 214: *msg = "bad label or misplaced continuation line"; break;
    case 218: *msg = "syntax error: duplicated goto field"; break;
    case 219: *msg = "syntax error: empty goto field"; break;
    case 227: *msg = "syntax error: right paren missing from goto"; break;
    case 228: *msg = "syntax error: right bracket missing from goto"; break;
    case 234: *msg = "syntax error: goto field incorrect"; break;
    default: *msg = "syntax error: invalid use of operator"; break;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_sb_syntax(const char *s, int *code, const char **msg) {
    long n = (long)strlen(s), p = 0, nopen = 0, depth = 0, k;
    for (k = 0; k < n; k++) if (s[k] == '(' || s[k] == '[' || s[k] == '<') nopen++;
    char lev[nopen + 1];
    int prev = 0, st = 0;
    for (;;) {
        int blank = 0, el; char c, nx;
        while (p < n && (s[p] == ' ' || s[p] == '\t')) { p++; blank = 1; }
        c = p < n ? s[p] : 0;
        if (!c) el = 36;
        else if (c >= '0' && c <= '9') {
            long q = p; while (q < n && (isalnum((unsigned char)s[q]) || s[q] == '.' || s[q] == '_' || s[q] == '+' || s[q] == '-')) q++;
            if (!eval_sb_numok(s + p, q - p)) return eval_sb_fail(231, code, msg);
            el = 18; p = q; }
        else if (isalpha((unsigned char)c)) {
            long q = p + 1; while (q < n && (isalnum((unsigned char)s[q]) || s[q] == '.' || s[q] == '_')) q++;
            if (q < n && (s[q] == '+' || s[q] == '-')) return eval_sb_fail(233, code, msg);
            if (q < n && s[q] == '(') { el = 12; p = q + 1; } else { el = 15; p = q; } }
        else if (c == '\'' || c == '"') {
            long q = p + 1; while (q < n && s[q] != c) q++;
            if (q >= n) return eval_sb_fail(232, code, msg);
            el = 18; p = q + 1; }
        else if (c == '(') { el = 3; p++; }
        else if (c == ')') { el = 24; p++; }
        else if (c == '[' || c == '<') { el = 6; p++; }
        else if (c == ']' || c == '>') { el = 27; p++; }
        else if (c == ',') { el = 9; p++; }
        else if (c == ':' || c == ';') { el = 33; p++; }
        else if (strchr("+-.$!%*/#@|&?=~^", c)) {
            long len = (c == '*' && p + 1 < n && s[p + 1] == '*' && (p + 2 >= n || s[p + 2] == ' ' || s[p + 2] == '\t')) ? 2 : 1;
            nx = p + len < n ? s[p + len] : 0;
            if (!nx || nx == ' ' || nx == '\t' || nx == ';' || nx == ':' || nx == ')' || nx == ']' || nx == '>') {
                if (!blank) return eval_sb_fail(233, code, msg);
                el = c == '=' ? 22 : 21; }
            else if (prev <= 12 || blank) el = 0;
            else return eval_sb_fail(233, code, msg);
            p += len; }
        else return eval_sb_fail(230, code, msg);
        prev = el;
        switch (el) {
        case 15: case 18: if (st == 2 && !blank) return eval_sb_fail(220, code, msg); st = 2; break;
        case 3: case 12: if (st == 2 && !blank) return eval_sb_fail(220, code, msg); lev[depth++] = el == 3 ? 4 : 5; st = 0; break;
        case 0: if (st == 2 && !blank) return eval_sb_fail(220, code, msg); st = 1; break;
        case 21: if (st != 2) return eval_sb_fail(221, code, msg); st = 1; break;
        case 22: if (st != 2) return eval_sb_fail(221, code, msg); st = 0; break;
        case 6: if (st != 2) return eval_sb_fail(222, code, msg); lev[depth++] = 3; st = 0; break;
        case 24: if (st == 1) return eval_sb_fail(221, code, msg); if (depth && lev[depth - 1] > 3) { depth--; st = 2; break; } return eval_sb_fail(224, code, msg);
        case 27: if (st == 1) return eval_sb_fail(221, code, msg); if (depth && lev[depth - 1] == 3) { depth--; st = 2; break; } return eval_sb_fail(225, code, msg);
        case 9: if (st == 1) return eval_sb_fail(221, code, msg); if (depth) { st = 0; break; } return eval_sb_fail(223, code, msg);
        default: if (el == 33 && (st != 2 || !depth)) return 0; if (st == 1) return eval_sb_fail(221, code, msg); if (depth) return eval_sb_fail(lev[depth - 1] == 3 ? 229 : 226, code, msg); return 0;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int code_sb_field(const char *s, long a, long b, char *buf, int *code, const char **msg) {
    memcpy(buf, s + a, (size_t)(b - a)); buf[b - a] = '\0';
    return eval_sb_syntax(buf, code, msg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int code_sb_goto(const char *s, long i, long q, char *buf, int *code, const char **msg) {
    int sg = 0, fg = 0, any = 0;
    for (;;) {
        while (i < q && (s[i] == ' ' || s[i] == '\t')) i++;
        if (i >= q) return any ? 0 : eval_sb_fail(219, code, msg);
        char kind = 0; long j = i, k, d = 0; char qt = 0;
        if (s[i] == 'S' || s[i] == 'F') { j = i + 1; while (j < q && (s[j] == ' ' || s[j] == '\t')) j++; if (j < q && (s[j] == '(' || s[j] == '<')) kind = s[i]; else j = i; }
        if (s[j] != '(' && s[j] != '<') return eval_sb_fail(234, code, msg);
        char close = s[j] == '(' ? ')' : '>';
        for (k = j + 1; k < q; k++) { char c = s[k]; if (qt) { if (c == qt) qt = 0; continue; }
            if (c == '\'' || c == '"') qt = c; else if (c == '(' || c == '[' || c == '<') d++; else if (c == ')' || c == ']' || c == '>') { if (!d && c == close) break; if (d) d--; } }
        if (code_sb_field(s, j + 1, k, buf, code, msg)) return 1;
        if (k >= q) return eval_sb_fail(close == ')' ? 227 : 228, code, msg);
        { long e = j + 1; while (e < k && (s[e] == ' ' || s[e] == '\t')) e++; if (e == k) return eval_sb_fail(212, code, msg); }
        if (kind == 'S' || !kind) { if (sg) return eval_sb_fail(218, code, msg); }
        if (kind == 'F' || !kind) { if (fg) return eval_sb_fail(218, code, msg); }
        if (kind == 'S') sg = 1; else if (kind == 'F') fg = 1; else sg = fg = 1;
        any = 1; i = k + 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int code_sb_syntax(const char *s, int *code, const char **msg) {
    long n = (long)strlen(s), p = 0;
    char buf[n + 1];
    while (p < n) {
        long b = p, q, d = 0, eq = -1, col = -1, k; char qt = 0;
        if (s[b] != ' ' && s[b] != '\t' && s[b] != ';') {
            if (s[b] == '*' || s[b] == '-') return 0;
            if (!isalnum((unsigned char)s[b])) return eval_sb_fail(214, code, msg);
            while (b < n && s[b] != ' ' && s[b] != '\t' && s[b] != ';') b++; }
        for (q = b; q < n; q++) { char c = s[q]; if (qt) { if (c == qt) qt = 0; continue; }
            if (c == '\'' || c == '"') qt = c; else if (c == ';') break; else if (c == '(' || c == '[' || c == '<') d++; else if ((c == ')' || c == ']' || c == '>') && d) d--;
            else if (!d && c == ':' && col < 0) col = q; else if (!d && c == '=' && eq < 0 && col < 0 && q > b && (s[q - 1] == ' ' || s[q - 1] == '\t')) eq = q; }
        long be = col >= 0 ? col : q;
        if (eq >= 0) { for (k = b; k < eq && (s[k] == ' ' || s[k] == '\t'); k++) ; if (k == eq) return eval_sb_fail(221, code, msg); }
        if (code_sb_field(s, b, eq >= 0 ? eq : be, buf, code, msg)) return 1;
        if (eq >= 0 && code_sb_field(s, eq + 1, be, buf, code, msg)) return 1;
        if (col >= 0 && code_sb_goto(s, col + 1, q, buf, code, msg)) return 1;
        p = q + 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_eval_raise(int code, const char *msg) {
    long esv = g_error == G_ERROR_EVAL_STAGE ? 0 : g_error; if (!esv) g_error = G_ERROR_EVAL_STAGE; core_runtime_error(code, msg); g_error = esv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void code_compile_raise(const char *src, int pe, const char *pm) {
    int code = pe; const char *msg = pm;
    if (!code && (!src || !code_sb_syntax(src, &code, &msg))) return;
    rt_eval_raise(code, msg ? msg : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_eval_syntax_raise(const char *s) {
    int code = 0; const char *msg = (const char *)0;
    if (!s || !eval_sb_syntax(s, &code, &msg)) return;
    rt_eval_raise(code, msg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void eval_chain_settle(char *key, eval_chain_fn fn, DESCR_t res, size_t mark, size_t built, int thunks, int keep) {
    if (!keep) { bb_pool_release(mark); return; }
    if (!thunks && bb_pool_mark() == built && eval_result_is_plain(res) && !eval_cache_slot(key)) { eval_cache_put(key, NULL); bb_pool_release(mark); }
    else eval_cache_put(key, fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_frame_open(const char *s, int raise) {
    eval_chain_fn fn = eval_cache_get(s);
    size_t mark = 0, built = 0; int made = 0, keep = 0, thunks = 0;
    if (!fn) {
        mark = bb_pool_mark();
        int pe = 0; const char *pm = (const char *)0;
        fn = eval_build_chain(s, &pe, &pm, &thunks);
        if (!fn) { if (raise && !pe) rt_code_pool_check(); bb_pool_release(mark); if (raise) { if (pe) rt_eval_raise(pe, pm); else rt_eval_syntax_raise(s); } return -1; }
        built = bb_pool_mark(); made = 1; keep = mark < eval_retain_budget();
    }
    int my = eval_frame_push(NV_GET_fn(EVAL_TMP), made ? s : NULL);
    if (my < 0) { if (made) bb_pool_release(mark); return -1; }
    eval_frame_t *f = &g_eval_frames[my];
    f->fn = fn; f->mark = mark; f->built = built; f->made = made; f->keep = keep; f->thunks = thunks;
    NV_SET_fn(EVAL_TMP, FAILDESCR);
    return my;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t eval_frame_land(int my, int ok) {
    eval_frame_t *f = &g_eval_frames[my];
    DESCR_t got = NV_GET_fn(EVAL_TMP);
    f->res = (ok && !IS_FAIL(got)) ? got : FAILDESCR;
    NV_SET_fn(EVAL_TMP, f->saved);
    if (f->made) eval_chain_settle(f->key, f->fn, f->res, f->mark, f->built, f->thunks, f->keep);
    DESCR_t result = f->res;
    g_eval_frames_n = my;
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_string_transient(const char *s) {
    if (!s || !*s) return NULVCL;
    int my = eval_frame_open(s, 1);
    if (my < 0) return FAILDESCR;
    int ok = eval_chain_run_guarded(g_eval_frames[my].fn);
    return eval_frame_land(my, ok);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long fn; long how; } rt_eval_next_t;
rt_eval_next_t rt_eval_open(DESCR_t *args, int nargs) {
    extern int eval_text_takes_chain(const char *); extern void rt_eval_stage_leave(const char *);
    rt_eval_next_t none = { 0, 0 };
    if (args && nargs == 1 && args[0].v == DT_X) { extern int rt_dtx_open_tail(sno_dstar_rec_t *, long *); long rq[2] = { 0, 0 };
      if (rt_dtx_open_tail(SNO_DTX_REC(args[0]), rq) && rq[0]) return (rt_eval_next_t){ rq[0], rq[1] | (1L << 62) };
      return none; }
    if (!args || nargs != 1 || args[0].v != DT_S || !eval_guard_on() || !eval_open_on()) return none;
    const char *s = VARVAL_fn(args[0]);
    if (!s || !*s || !eval_text_takes_chain(s)) return none;
    rt_eval_stage_leave((const char *)0);
    int my = eval_frame_open(s, 0);
    if (my < 0) return none;
    eval_frame_t *f = &g_eval_frames[my];
    f->opened = 1;
    f->esv = g_error == G_ERROR_EVAL_STAGE ? 0 : g_error; g_error = G_ERROR_EVAL_STAGE;
    return (rt_eval_next_t){ (long)(uintptr_t)f->fn, (long)my };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_eval_land(long word) {
    int my = (int)word;
    if (my < 0 || my >= g_eval_frames_n || !g_eval_frames[my].opened) { fprintf(stderr, "rt_eval_land: frame %d is not an opened EVAL frame (%d live)\n", my, g_eval_frames_n); abort(); }
    g_error = g_eval_frames[my].esv;
    return eval_frame_land(my, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t eval_node(tree_t *e)
{
    (void)e;
    fprintf(stderr, "[B0b] BOMB eval_node: AST-walk evaluator deleted; nothing interprets tree_t at runtime\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t eval_expr(const char *src)
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void eval_gc_roots(void)
{
    extern void rt_gc_visit_raw(const char **loc);
    if (g_eval_cache) { rt_gc_visit_raw((const char **)&g_eval_cache);
        for (int i = 0; i < g_eval_cache_cap; i++) if (g_eval_cache[i].key) rt_gc_visit_raw((const char **)&g_eval_cache[i].key); }
    if (g_eval_frames) { extern void rt_gc_visit_descr(DESCR_t *); rt_gc_visit_raw((const char **)&g_eval_frames);
        for (int i = 0; i < g_eval_frames_n; i++) { rt_gc_visit_descr(&g_eval_frames[i].saved); rt_gc_visit_descr(&g_eval_frames[i].res); if (g_eval_frames[i].key) rt_gc_visit_raw((const char **)&g_eval_frames[i].key); } }
    if (g_lbl_tab) { rt_gc_visit_raw((const char **)&g_lbl_tab);
        for (int i = 0; i < g_lbl_n; i++) if (g_lbl_tab[i].key) rt_gc_visit_raw((const char **)&g_lbl_tab[i].key); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_label_set_fn(const char *name, void *fn) {
    if (!name || !*name) return;
    for (int i = 0; i < g_lbl_n; i++) if (!strcmp(g_lbl_tab[i].key, name)) { g_lbl_tab[i].fn = (eval_chain_fn)fn; return; }
    if (g_lbl_n >= g_lbl_cap) {
        int ncap = g_lbl_cap ? g_lbl_cap * 2 : 16;
        lbl_ent_t *nt = (lbl_ent_t *)rt_wsb_realloc(g_lbl_tab, (size_t)ncap * sizeof(lbl_ent_t));
        if (!nt) return;
        g_lbl_tab = nt; g_lbl_cap = ncap;
    }
    g_lbl_tab[g_lbl_n].key = rt_heap_strdup_c(name);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rt_goto_peek_name(const char *name) {
    if (!name || !*name) return NULL;
    if (name[0] != '$' && name[0] != '@') return NULL;
    DESCR_t iv = NV_GET_fn(name + 1);
    if (name[0] == '@' && iv.v != DT_N) return NULL;
    if (iv.v == DT_FAIL) return NULL;
    if (iv.v != DT_N) { DESCR_t s = VARVAL_d_fn(iv); if (s.v == DT_S && s.s && descr_slen(s) > 0) return rt_cstr_d(s); return NULL; }
    return VARVAL_fn(iv);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_sno_goto_special_is(const char *enc) {
    if (!enc || enc[0] != '^' || !enc[1]) return 0;
    const char *want = (enc[1] == 'R') ? "RETURN" : (enc[1] == 'F') ? "FRETURN" : (enc[1] == 'N') ? "NRETURN" : NULL;
    if (!want) return 0;
    const char *got = rt_goto_peek_name(enc + 2);
    return (got && !strcmp(got, want)) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *rt_goto_resolve_x(const char *name, int *undef)
{
    if (!name || !*name) return NULL;
    if (name[0] == '$') {
        DESCR_t iv = NV_GET_fn(name + 1);
        const char *inm = rt_sno_indirect_name(iv);
        if (!inm || !*inm) { fprintf(stderr, "[SNO] transfer to undefined label: $%s (indirect name is null)\n", name + 1); exit(1); }
        return rt_goto_resolve_x(inm, undef);
    }
    if (name[0] == '@') {
        DESCR_t iv = NV_GET_fn(name + 1);
        extern int kwb_error(int code, const char *msg);
        if (iv.v != DT_N) { kwb_error(21, "function called by name returned a value"); return NULL; }
        const char *inm = rt_sno_indirect_name(iv);
        if (!inm || !*inm) { kwb_error(21, "function called by name returned a value"); return NULL; }
        return rt_goto_resolve_x(inm, undef);
    }
    if (name[0] == '<') {
        DESCR_t cv = NV_GET_fn(name + 1);
        if (cv.v != DT_C || cv.slen != 3 || !cv.ptr) { fprintf(stderr, "[SNO] goto operand in direct goto is not code\n"); exit(1); }
        return cv.ptr;
    }
    if (!strcmp(name, "END")) return NULL;
    if (!strcmp(name, "CONTINUE") || !strcmp(name, "SCONTINUE") || !strcmp(name, "ABORT")) {
        extern uint64_t rtccb[32];
        if (rtccb[26]) { if (name[0] == 'A') { extern void rt_setexit_abort(void); rt_setexit_abort(); } extern void rt_setexit_continue_tramp(void); return (void *)rt_setexit_continue_tramp; }
        { extern void sno_setexit_resume(const char *which); sno_setexit_resume(name); return NULL; } }
    { eval_chain_fn fn = rt_label_get_fn(name); if (fn) return (void *)fn; }
    {
        extern void *rt_proc_get_fn(const char *);
        char lname[strlen(name) + 6]; snprintf(lname, sizeof lname, "LBL__%s", name);
        eval_chain_fn fn = (eval_chain_fn)rt_proc_get_fn(lname);
        if (fn) return (void *)fn;
    }
    if (undef) { *undef = 1; return NULL; }
    { extern void core_runtime_error(int code, const char *msg); core_runtime_error(38, "goto undefined label"); }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_goto_resolve(const char *name) { return rt_goto_resolve_x(name, NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_entry_resolve(const char *name, int *is_frag)
{
    if (is_frag) *is_frag = 0;
    if (!name || !*name) return NULL;
    { eval_chain_fn fn = rt_label_get_fn(name); if (fn) { if (is_frag) *is_frag = 1; return (void *)fn; } }
    { extern void *rt_proc_get_fn(const char *); char lname[strlen(name) + 6]; snprintf(lname, sizeof lname, "LBL__%s", name); return rt_proc_get_fn(lname); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_goto_transfer(const char *name)
{
    extern void rt_c2bb_hit(const char *site, const char *name);
    void *fn = rt_goto_resolve(name);
#if RT_DIAG
    if (fn) { rt_c2bb_hit("chain.goto", name); RT_GC_CALLBACK_V(rt_chain_enter((eval_chain_fn)fn)); return 1; }
#else
    if (fn) { RT_GC_CALLBACK_V(rt_chain_enter((eval_chain_fn)fn)); return 1; }
#endif
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_goto_resolve_ck(const char *name, int *undef) { return rt_goto_resolve_x(name, undef); }
__asm__(
".text\n"
".globl rt_setexit_continue_tramp\n"
"rt_setexit_continue_tramp:\n"
"  movq rtccb@GOTPCREL(%rip), %rax\n"
"  movq 208(%rax), %rcx\n"
"  movq $0, 208(%rax)\n"
"  movq 216(%rax), %rdx\n"
"  movq 224(%rax), %rbp\n"
"  movq 232(%rax), %r12\n"
"  movq %rdx, %rsp\n"
"  jmp *%rcx\n"
);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t code_at(const char *src, long base);
DESCR_t code(const char *src) { return code_at(src, 0); }
long g_sno_stmt_compiled = 0;
DESCR_t code_at(const char *src, long base)
{
    { extern void rt_eval_stage_leave(const char *); rt_eval_stage_leave((const char *)0); }
    if (!src) return FAILDESCR;
    const char *illegal = NULL, *orig = src;
    { long bad = sno_text_illegal_at(src);
      if (bad >= 0) { long cut = -1; char q = 0; for (long i = 0; i < bad; i++) { char c = src[i]; if (q) { if (c == q) q = 0; continue; } if (c == '\'' || c == '"') q = c; else if (c == ';') cut = i; }
        illegal = "syntax error: illegal character";
        if (cut < 0) { extern const char *g_sno_errtext; g_sno_errtext = illegal; code_compile_raise(orig, 0, NULL); return FAILDESCR; }
        char *pre = (char *)rt_wsb_alloc((size_t)cut + 1); if (!pre) return FAILDESCR; memcpy(pre, src, (size_t)cut); pre[cut] = '\0'; src = pre; } }
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void fc_tables_reset(void); fc_tables_reset(); extern void zls_reset(void); zls_reset(); extern void bb_src_reset(void); bb_src_reset(); }
    extern tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out);
    extern IR_graph_t *sno_lower_fragment_at(const tree_t *prog, int entry_idx, long stno_base);
    extern const char *sno_stmt_label(const tree_t *s);
    extern int g_frame_active;
    extern void sno_error_quiet_begin(void); extern void sno_error_quiet_end(void); extern const char *sno_error_captured(void); extern const char *g_sno_errtext;
    sno_error_quiet_begin();
    tree_t *prog = sno_parse_string_ast(src, NULL);
    sno_error_quiet_end();
    int empty = 0;
    if (!prog || prog->n == 0) { const char *cap = sno_error_captured(); if (cap) { g_sno_errtext = rt_heap_strdup_c(cap); code_compile_raise(orig, 0, NULL); return FAILDESCR; }
      if (!prog) prog = ast_stmt_new(TT_PROGRAM);
      tree_t *st = ast_stmt_new(TT_STMT); ast_push(st, ast_attr_int(":line", 1)); ast_push(prog, st); empty = 1; }
    else { int nst = 0, nmt = 0;
      for (int i = 0; i < prog->n; i++) { const tree_t *c = prog->c[i]; if (!c || c->t != TT_STMT) continue; nst++;
          nmt += !stmt_attr_find(c, ":lbl") && !stmt_attr_find(c, ":subj") && !stmt_attr_find(c, ":pat") && !stmt_attr_find(c, ":eq")
               && !stmt_goto_find(c, TT_GOTO_S) && !stmt_goto_find(c, TT_GOTO_F) && !stmt_goto_find(c, TT_GOTO_U); }
      if (nst && nmt == nst) empty = nst; }
    const char *parse_err = sno_error_captured(); if (parse_err) parse_err = rt_heap_strdup_c(parse_err);
    int pe = 0; const char *pm = (const char *)0;
    { extern int sno_preeval_stmt(const tree_t *, const char **);
      for (int i = 0; i < prog->n && !parse_err; i++) if ((pe = sno_preeval_stmt(prog->c[i], &pm))) parse_err = pm ? pm : ""; }
    if (g_sno_stmt_compiled < base) g_sno_stmt_compiled = base;
    long stno_base = g_sno_stmt_compiled;
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
            int rfl_sv = g_rt_fragment_emit; g_rt_fragment_emit = 1;
            IR_graph_t *g = sno_lower_fragment_at(prog, k, stno_base);
            g_rt_fragment_emit = rfl_sv;
            if (!g) return FAILDESCR;
            g->runtime_fragment_graph = 1;
            extern IR_graph_t *g_emit_cfg;
            IR_graph_t *cfg_sv = g_emit_cfg; g_emit_cfg = g;
            int fa = g_frame_active; g_frame_active = 1;
            int rfe_sv = g_rt_fragment_emit; g_rt_fragment_emit = 1;
            emit_jmp_entry_for_chain(g);
            eval_chain_fn fn = emit_chain(g->entry, NULL, "code_flat");
            { extern void emit_gc_tables_register(const void *); emit_gc_tables_register((const void *) fn); }
            emit_jmp_entry_clear();
            g_rt_fragment_emit = rfe_sv;
            g_frame_active = fa; g_emit_cfg = cfg_sv;
            if (!fn) { rt_code_pool_check(); return FAILDESCR; }
            if (k == 0) first = fn;
            if (lbl && lbl[0]) rt_label_set_fn(lbl, (void *)fn);
        }
        k++;
    }
    { int patn = sno_pat_count(); const char *ks = getenv("SCRIP_CODE_THUNKS");
      if (!(ks && *ks == '0')) sno_expr_thunks_build(expr0);
      if (patn > pat0) sno_pat_thunks_build(pat0);
      if (((ks && *ks == '0') ? (patn > pat0) : 1) && eval_thunks_emit_from(proc0)) { rt_code_pool_check(); return FAILDESCR; } }
    g_sno_stmt_compiled += (long)(k - empty) + 1;
    if (parse_err || illegal) { g_sno_errtext = parse_err ? parse_err : illegal; code_compile_raise(orig, pe, pm); return FAILDESCR; }
    if (!first) return FAILDESCR;
    DESCR_t d = {0};
    d.v    = DT_C;
    d.slen = 3;
    d.ptr  = (void *)first;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t EXPVAL_fn(DESCR_t expr_d)
{
    if (expr_d.v == DT_E) {
        if (expr_d.slen == RT_CONVE_CHAIN_MARK) {
            eval_chain_fn fn = (eval_chain_fn)expr_d.ptr;
            if (!fn) return FAILDESCR;
            DESCR_t saved = NV_GET_fn(EVAL_TMP);
            NV_SET_fn(EVAL_TMP, FAILDESCR);
            rt_c2bb_hit("chain.eval.conve", "?");
            { extern void rt_eval_stage_enter(const char *); rt_eval_stage_enter((const char *)0); }
            eval_chain_enter_only(fn);
            { extern void rt_eval_stage_leave(const char *); rt_eval_stage_leave((const char *)0); }
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
        { extern void rt_eval_stage_enter(const char *); rt_eval_stage_enter((const char *)0); }
        DESCR_t result = eval_node((tree_t *)expr_d.ptr);
        { extern void rt_eval_stage_leave(const char *); rt_eval_stage_leave((const char *)0); }
        NAME_ctx_leave();
        Σ = save_Σ;
        Ω = save_Ω;
        Δ = save_Δ;
        return result;
    }
    if (expr_d.v == DT_C) {
        if (expr_d.slen == 3) {
            core_runtime_error(103, "eval argument is not expression: EXPVAL of a CODE value is DELETED (ceo CEO-1094, Lon 2026-09-21: eradicate C->BB->C->BB, and a C function creates residue on the hardware stack). It entered the chain from a C frame through rt_chain_enter and returned NULVCL afterwards, so the C frame survived the transition. THE ORACLE REFUSES THIS ROAD TOO: sbl -bf raises ERROR 103 on EVAL of a CODE value, while this arm returned quietly. The sanctioned way to run a CODE value is the DIRECT GOTO :<C>, which is EMITTED and never enters a box from C.");
            return FAILDESCR;
        }
        return NULVCL;
    }
    const char *s = VARVAL_fn(expr_d);
    if (!s || !*s) return NULVCL;
    return eval_expr(s);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int conve_is_bare_name(const char *s) {
    if (!s || !((*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z'))) return 0;
    for (const char *p = s; *p; p++) if (!((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9') || *p == '.' || *p == '_')) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t CONVE_fn(DESCR_t str_d)
{
    { extern void rt_eval_stage_leave(const char *); rt_eval_stage_leave((const char *)0); }
    const char *s = VARVAL_fn(str_d);
    if (!s || !*s) return FAILDESCR;
    if (conve_is_bare_name(s)) {
        extern void *bb_dstar_rec_intern(const char *, uint32_t);
        size_t nl = strlen(s); char key[nl + 2]; key[0] = '='; memcpy(key + 1, s, nl + 1);
        DESCR_t xd = {0}; xd.v = DT_X; xd.slen = 0; xd.p = bb_dstar_rec_intern(key, SNO_DSTAR_VARREF);
        return xd;
    }
    int pe = 0, thunks = 0; const char *pm = (const char *)0;
    eval_chain_fn fn = eval_build_chain(s, &pe, &pm, &thunks);
    if (!fn) { rt_code_pool_check(); return FAILDESCR; }
    DESCR_t d = {0};
    d.v    = DT_E;
    d.slen = RT_CONVE_CHAIN_MARK;
    d.ptr  = (void *)fn;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_emit_new_procs(int pc0)
{
    extern void optimizer_run(IR_graph_t *g); extern void fl_derive_tier(IR_graph_t *g); extern void ir_drive_slot_assign(IR_graph_t *g); extern void rt_proc_set_frame(const char *, int, int);
    extern int g_gen_proc_active; extern int g_frame_active; extern IR_graph_t *g_emit_cfg;
    int emit_failed = 0;
    for (int pi = pc0; pi < g_stage2.proc_count; pi++) {
        const char *pname = g_stage2.proc_table[pi].name; int idx = g_stage2.proc_table[pi].bb_idx;
        if (!pname || !strcmp(pname, "main") || idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) continue;
        emit_register_proc(&g_stage2, pi); emit_proc_props(&g_stage2, pi);
        IR_graph_t *g = g_stage2.bbp.table[idx];
        optimizer_run(g); { extern void zls_forget_graph_nodes(const IR_graph_t *); zls_forget_graph_nodes(g); } fl_derive_tier(g); ir_drive_slot_assign(g);
        if (g->caller_frame && g->nslots > 0) rt_proc_set_frame(pname, g->nslots - 1, g_stage2.proc_table[pi].decl_level);
    }
    IR_graph_t *cfg_sv = g_emit_cfg; int fa = g_frame_active; g_frame_active = 1; int ga = g_gen_proc_active; g_rt_fragment_emit = 1;
    for (int pi = pc0; pi < g_stage2.proc_count; pi++) {
        const char *pname = g_stage2.proc_table[pi].name; int idx = g_stage2.proc_table[pi].bb_idx;
        if (!pname || !strcmp(pname, "main") || idx < 0 || idx >= g_stage2.bbp.count || !g_stage2.bbp.table[idx] || !g_stage2.bbp.table[idx]->entry) continue;
        if (!emit_install_proc(&g_stage2, pi, NULL)) emit_failed = 1;
    }
    g_rt_fragment_emit = 0; g_gen_proc_active = ga; g_frame_active = fa; g_emit_cfg = cfg_sv;
    return emit_failed;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_raku_eval_compile(const char *src, const char **errmsg)
{
    extern tree_t *rk_parse_tree(const char *src, int len, const char *path, char **errmsg);
    extern const char *lower_raku_eval_stage2(const tree_t *prog);
    char *perr = NULL; *errmsg = NULL;
    if (!src) return NULL;
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void fc_tables_reset(void); fc_tables_reset(); extern void zls_reset(void); zls_reset(); extern void bb_src_reset(void); bb_src_reset(); }
    tree_t *prog = rk_parse_tree(src, (int) strlen(src), "EVAL", &perr);
    if (!prog) { *errmsg = perr ? perr : "syntax error"; return NULL; }
    int pc0 = g_stage2.proc_count;
    const char *nm = lower_raku_eval_stage2(prog);
    if (rk_emit_new_procs(pc0)) { *errmsg = "EVAL: the code could not be emitted"; return NULL; }
    return nm;
}
