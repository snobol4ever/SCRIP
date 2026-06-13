#include "IR_interp.h"
#include "IR_interp_state.h"
#include "rt/rt.h"
#include "lower.h"
#include "../../emitter/sil_macros.h"
#include "../../parser/prolog/term.h"
#include "../../parser/prolog/prolog_runtime.h"
#include "../../parser/prolog/prolog_atom.h"
#include "../../runtime/builtins/resolution.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <gc/gc.h>
#include <setjmp.h>
#include "../include/dtp.h"
extern const char *Σ;
extern int         Δ;
extern int         Ω;
extern int         Σlen;
#define BB_DCAP_MAX 32
typedef struct { const char *varname; int start; int len; } bb_dcap_t;
static bb_dcap_t g_dcap[BB_DCAP_MAX];
static int       g_dcap_n = 0;
static int       g_dcap_active = 0;
static void bb_dcap_record(const char *vname, int start, int len) {
    if (!vname || !*vname) return;
    for (int i = 0; i < g_dcap_n; i++) {
        if (g_dcap[i].varname && strcmp(g_dcap[i].varname, vname) == 0) {
            g_dcap[i].start = start; g_dcap[i].len = len; return;
        }
    }
    if (g_dcap_n < BB_DCAP_MAX) { g_dcap[g_dcap_n].varname = vname; g_dcap[g_dcap_n].start = start; g_dcap[g_dcap_n].len = len; g_dcap_n++; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void bb_dcap_flush(void) {
    for (int i = 0; i < g_dcap_n; i++) {
        int len = g_dcap[i].len;
        char *copy = (char *)GC_MALLOC((size_t)len + 1);
        if (copy) { memcpy(copy, Σ + g_dcap[i].start, (size_t)len); copy[len] = '\0'; }
        DESCR_t d = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
        NV_SET_fn(g_dcap[i].varname, d);
    }
    g_dcap_n = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void bb_dcap_clear(void) { g_dcap_n = 0; }
#include "core.h"
#include "lower.h"
#include "../runtime/builtins/gen_runtime.h"
#include "../runtime/builtins/gen_value.h"
#include "coerce.h"
extern int try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
extern DESCR_t *data_field_ptr(const char *field, DESCR_t obj);
typedef struct { char name[64]; int nfields; char fields[64][64]; } DatType;
extern DatType *dat_register(const char *spec);
extern DatType *dat_find_type(const char *name);
extern DESCR_t    dat_construct(DatType *t, DESCR_t *args, int nargs);
extern int exec_stmt(const char *subj_name, DESCR_t *subj_var, DESCR_t pat, DESCR_t *repl, int has_repl);
#include "bb_box.h"
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail);
static DESCR_t g_ir_return_val;
#define SUSPEND_COLLECT_MAX 65536
typedef struct { DESCR_t * items; int count; int cap; int active; } SuspendBuf;
static SuspendBuf g_suspend_buf;
static void suspend_buf_push(DESCR_t v) {
    if (!g_suspend_buf.active) return;
    if (g_suspend_buf.count >= g_suspend_buf.cap) {
        int ncap = g_suspend_buf.cap ? g_suspend_buf.cap * 2 : 16;
        if (ncap > SUSPEND_COLLECT_MAX) ncap = SUSPEND_COLLECT_MAX;
        if (g_suspend_buf.count >= ncap) return;
        DESCR_t * ni = (DESCR_t *) GC_MALLOC((size_t) ncap * sizeof(DESCR_t));
        if (!ni) return;
        if (g_suspend_buf.items) memcpy(ni, g_suspend_buf.items, (size_t) g_suspend_buf.count * sizeof(DESCR_t));
        g_suspend_buf.items = ni; g_suspend_buf.cap = ncap;
    }
    g_suspend_buf.items[g_suspend_buf.count++] = v;
}
typedef struct { DESCR_t * items; int count; } SuspendList;
#define SNO_SAVE_MAX 4096
typedef struct { const char * name; DESCR_t old; } SnoSaveEnt;
static SnoSaveEnt   g_sno_save[SNO_SAVE_MAX];
static int          g_sno_save_top = 0;
static const char * g_sno_cur_func = NULL;
#define RESOLVE_NB_SIZE 64
typedef struct { int atom_id; Term *val; } PlNbSlot;
static PlNbSlot g_resolve_nb[RESOLVE_NB_SIZE];
static void resolve_nb_set(int aid, Term *v) {
    unsigned h=(unsigned)aid%(unsigned)RESOLVE_NB_SIZE;
    for (int i=0;i<RESOLVE_NB_SIZE;i++) { int s=(h+i)%RESOLVE_NB_SIZE; if (!g_resolve_nb[s].val||g_resolve_nb[s].atom_id==aid) { g_resolve_nb[s].atom_id=aid; g_resolve_nb[s].val=v; return; } }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static Term *resolve_nb_get(int aid) {
    unsigned h=(unsigned)aid%(unsigned)RESOLVE_NB_SIZE;
    for (int i=0;i<RESOLVE_NB_SIZE;i++) { int s=(h+i)%RESOLVE_NB_SIZE; if (!g_resolve_nb[s].val) return NULL; if (g_resolve_nb[s].atom_id==aid) return g_resolve_nb[s].val; }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * g_current_cfg = NULL;
static IR_graph_t * g_resolve_tail_redirect_cfg   = NULL;
static IR_t       * g_resolve_tail_redirect_entry = NULL;
int g_resolve_b3_call_mark = -1;
DESCR_t IR_interp_once(IR_graph_t * bbg);
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { IR_t * node; DESCR_t * items; int count; int cap; } rk_seq_cache_t;
#define RK_SEQ_CACHE_MAX 64
static rk_seq_cache_t g_rk_seq_cache[RK_SEQ_CACHE_MAX];
static int g_rk_seq_cache_n = 0;
static rk_seq_cache_t * rk_seq_cache_find(IR_t * node) {
    for (int i = 0; i < g_rk_seq_cache_n; i++) if (g_rk_seq_cache[i].node == node) return &g_rk_seq_cache[i];
    return NULL;
}
static rk_seq_cache_t * rk_seq_cache_get(IR_t * node) {
    rk_seq_cache_t * e = rk_seq_cache_find(node);
    if (e) return e;
    if (g_rk_seq_cache_n >= RK_SEQ_CACHE_MAX) { g_rk_seq_cache_n = 0; }
    e = &g_rk_seq_cache[g_rk_seq_cache_n++];
    e->node = node; e->items = NULL; e->count = 0; e->cap = 0;
    return e;
}
static void rk_seq_cache_push(rk_seq_cache_t * e, DESCR_t v) {
    if (e->count >= e->cap) {
        int ncap = e->cap ? e->cap * 2 : 8;
        DESCR_t * ni = (DESCR_t *) GC_malloc((size_t) ncap * sizeof(DESCR_t));
        if (e->items && e->count > 0) memcpy(ni, e->items, (size_t) e->count * sizeof(DESCR_t));
        e->items = ni; e->cap = ncap;
    }
    e->items[e->count++] = v;
}
typedef struct { IR_t * node; DESCR_t * items; int count; } susp_gen_cache_t;
#define SUSP_GEN_CACHE_MAX 64
static susp_gen_cache_t g_susp_gen_cache[SUSP_GEN_CACHE_MAX];
static int g_susp_gen_cache_n = 0;
static susp_gen_cache_t * susp_gen_cache_get(IR_t * node) {
    for (int i = 0; i < g_susp_gen_cache_n; i++) if (g_susp_gen_cache[i].node == node) return &g_susp_gen_cache[i];
    if (g_susp_gen_cache_n >= SUSP_GEN_CACHE_MAX) g_susp_gen_cache_n = 0;
    susp_gen_cache_t * e = &g_susp_gen_cache[g_susp_gen_cache_n++];
    e->node = node; e->items = NULL; e->count = 0;
    return e;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int rt_scan_exec(const char *subj_name, const char *subj_lit, int has_repl, const char *repl_str, void *pat_graph);
int rt_scan_exec(const char *subj_name, const char *subj_lit, int has_repl, const char *repl_str, void *pat_graph) {
    IR_graph_t *pat = (IR_graph_t *)pat_graph;
    if (!pat || !pat->entry) return 0;
    const char *subj_str = ""; int subj_len = 0;
    if (subj_name) {
        DESCR_t sv = VARVAL_d_fn(NV_GET_fn(subj_name));
        if (sv.v == DT_S || sv.v == DT_SNUL) { subj_str = sv.s ? sv.s : ""; subj_len = sv.slen ? (int)sv.slen : (int)strlen(subj_str); }
        else if (IS_INT_fn(sv) || IS_REAL_fn(sv)) { DESCR_t ss = descr_to_str(sv); subj_str = ss.s ? ss.s : ""; subj_len = (int)strlen(subj_str); }
    } else if (subj_lit) {
        subj_str = subj_lit; subj_len = (int)strlen(subj_lit);
    }
    const char *save_Σ = Σ; int save_Σlen = Σlen; int save_Ω = Ω; int save_Δ = Δ; int save_dca = g_dcap_active; int save_dcn = g_dcap_n;
    extern int64_t kw_anchor;
    Σ = subj_str; Σlen = subj_len; Ω = subj_len;
    int max_start = kw_anchor ? 0 : subj_len; int matched = 0; int m_start = -1; int m_end = -1;
    g_dcap_active = 1;
    for (int start = 0; start <= max_start; start++) {
        Δ = start; g_dcap_n = 0;
        DESCR_t r = IR_interp_once(pat);
        if (!IS_FAIL_fn(r)) { matched = 1; m_start = start; m_end = Δ; break; }
    }
    if (matched) bb_dcap_flush(); else g_dcap_n = 0;
    if (matched && has_repl && subj_name) {
        const char *repl = repl_str ? repl_str : "";
        int repl_len = (int)strlen(repl);
        int new_len = m_start + repl_len + (subj_len - m_end);
        char *new_s = (char *)GC_MALLOC((size_t)new_len + 1);
        memcpy(new_s, subj_str, (size_t)m_start);
        memcpy(new_s + m_start, repl, (size_t)repl_len);
        memcpy(new_s + m_start + repl_len, subj_str + m_end, (size_t)(subj_len - m_end));
        new_s[new_len] = '\0';
        DESCR_t nv = { .v = DT_S, .slen = (uint32_t)new_len, .s = new_s };
        NV_SET_fn(subj_name, nv);
    }
    Σ = save_Σ; Σlen = save_Σlen; Ω = save_Ω; Δ = save_Δ; g_dcap_active = save_dca; g_dcap_n = save_dcn;
    return matched;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int rt_gvar_assign_concat(const char *name, void *left_graph, void *right_graph);
int rt_gvar_assign_concat(const char *name, void *left_graph, void *right_graph) {
    IR_graph_t *lblk = (IR_graph_t *)left_graph;
    IR_graph_t *rblk = (IR_graph_t *)right_graph;
    if (!lblk || !rblk) return 0;
    bb_reset(lblk); DESCR_t lv = IR_interp_once(lblk);
    if (IS_FAIL_fn(lv)) return 0;
    bb_reset(rblk); DESCR_t rv = IR_interp_once(rblk);
    if (IS_FAIL_fn(rv)) return 0;
    int rel_fail = 0;
    DESCR_t result = binop_apply(BINOP_CONCAT, lv, rv, &rel_fail);
    if (IS_FAIL_fn(result)) return 0;
    NV_SET_fn(name ? name : "", result);
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int rt_scan(void * pat_graph, void * subj_graph, int is_repl, const char * subj_name, void * repl_graph);
int rt_scan(void * pat_graph, void * subj_graph, int is_repl, const char * subj_name, void * repl_graph) {
    IR_graph_t * pat = (IR_graph_t *)pat_graph;
    if (!pat || !pat->entry) return 0;
    const char * save_Σ = Σ; int save_Σlen = Σlen; int save_Ω = Ω; int save_Δ = Δ; int save_dca = g_dcap_active; int save_dcn = g_dcap_n;
    IR_graph_t * sjg = (IR_graph_t *)subj_graph;
    DESCR_t subj_d = sjg ? IR_interp_once(sjg) : (is_repl ? NV_GET_fn(subj_name ? subj_name : "") : FAILDESCR);
    DESCR_t sv = VARVAL_d_fn(subj_d);
    const char * subj_str = ""; int subj_len = 0;
    if (sv.v == DT_S || sv.v == DT_SNUL) { subj_str = sv.s ? sv.s : ""; subj_len = sv.slen ? (int)sv.slen : (int)strlen(subj_str); }
    else if (IS_INT_fn(sv) || IS_REAL_fn(sv)) { DESCR_t t = descr_to_str(sv); subj_str = t.s ? t.s : ""; subj_len = t.slen ? (int)t.slen : (int)strlen(subj_str); }
    extern int64_t kw_anchor;
    Σ = subj_str; Σlen = subj_len; Ω = subj_len;
    int max_start = kw_anchor ? 0 : subj_len; int matched = 0; int m_start = -1; int m_end = -1;
    g_dcap_active = 1;
    for (int start = 0; start <= max_start; start++) {
        Δ = start; g_dcap_n = 0;
        DESCR_t r = IR_interp_once(pat);
        if (!IS_FAIL_fn(r)) { matched = 1; m_start = start; m_end = Δ; break; }
    }
    if (matched) bb_dcap_flush(); else g_dcap_n = 0;
    if (matched && is_repl) {
        IR_graph_t * rpg = (IR_graph_t *)repl_graph;
        DESCR_t repl_d = rpg ? VARVAL_d_fn(IR_interp_once(rpg)) : NULVCL;
        if (IS_INT_fn(repl_d) || IS_REAL_fn(repl_d)) repl_d = descr_to_str(repl_d);
        const char * repl_str = (repl_d.v == DT_S || repl_d.v == DT_SNUL) ? (repl_d.s ? repl_d.s : "") : "";
        int repl_len = (repl_d.v == DT_S || repl_d.v == DT_SNUL) ? (repl_d.slen ? (int)repl_d.slen : (int)strlen(repl_str)) : 0;
        int new_len = m_start + repl_len + (subj_len - m_end);
        char * new_s = (char *)GC_MALLOC((size_t)new_len + 1);
        memcpy(new_s, subj_str, (size_t)m_start);
        memcpy(new_s + m_start, repl_str, (size_t)repl_len);
        memcpy(new_s + m_start + repl_len, subj_str + m_end, (size_t)(subj_len - m_end));
        new_s[new_len] = '\0';
        DESCR_t nv = { .v = DT_S, .slen = (uint32_t)new_len, .s = new_s };
        NV_SET_fn(subj_name ? subj_name : "", nv);
    }
    Σ = save_Σ; Σlen = save_Σlen; Ω = save_Ω; Δ = save_Δ; g_dcap_active = save_dca; g_dcap_n = save_dcn;
    return matched ? 1 : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int rt_scan_lit(const char * subj_name, const char * subj_lit, const char * pat_lit, int is_repl, const char * repl_lit);
int rt_scan_lit(const char * subj_name, const char * subj_lit, const char * pat_lit, int is_repl, const char * repl_lit) {
    const char * subj_str = ""; int subj_len = 0;
    if (subj_name && subj_name[0]) {
        DESCR_t sv = VARVAL_d_fn(NV_GET_fn(subj_name));
        if (sv.v == DT_S || sv.v == DT_SNUL) { subj_str = sv.s ? sv.s : ""; subj_len = sv.slen ? (int)sv.slen : (int)strlen(subj_str); }
        else if (IS_INT_fn(sv) || IS_REAL_fn(sv)) { DESCR_t t = descr_to_str(sv); subj_str = t.s ? t.s : ""; subj_len = t.slen ? (int)t.slen : (int)strlen(subj_str); }
    } else if (subj_lit) {
        subj_str = subj_lit; subj_len = (int)strlen(subj_lit);
    }
    const char * pat = pat_lit ? pat_lit : ""; int pat_len = (int)strlen(pat);
    extern int64_t kw_anchor;
    int max_start = kw_anchor ? 0 : subj_len; int matched = 0; int m_start = -1; int m_end = -1;
    for (int start = 0; start <= max_start; start++) {
        if (start + pat_len <= subj_len && (pat_len == 0 || memcmp(subj_str + start, pat, (size_t)pat_len) == 0)) { matched = 1; m_start = start; m_end = start + pat_len; break; }
    }
    if (matched && is_repl && subj_name && subj_name[0]) {
        const char * repl = repl_lit ? repl_lit : ""; int repl_len = (int)strlen(repl);
        int new_len = m_start + repl_len + (subj_len - m_end);
        char * new_s = (char *)GC_MALLOC((size_t)new_len + 1);
        memcpy(new_s, subj_str, (size_t)m_start);
        memcpy(new_s + m_start, repl, (size_t)repl_len);
        memcpy(new_s + m_start + repl_len, subj_str + m_end, (size_t)(subj_len - m_end));
        new_s[new_len] = '\0';
        DESCR_t nv = { .v = DT_S, .slen = (uint32_t)new_len, .s = new_s };
        NV_SET_fn(subj_name, nv);
    }
    return matched ? 1 : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int ir_is_single_shot(IR_t * e) {
    if (!e) return 1;
    switch (e->op) {
    case IR_TO: case IR_TO_BY: case IR_UPTO: case IR_ITERATE:
    case IR_GEN_ALT: case IR_LIMIT: case IR_GEN_BINOP: case IR_TO_NESTED:
    case IR_PROC_GEN: case IR_BINOP_GEN: case IR_ALT:
    case IR_SUSPEND: case IR_REPEAT: case IR_GEN_SCAN:
    case IR_LIST_BANG: case IR_KEY_GEN: case IR_FIND_GEN: case IR_SEQ_GEN:
    case IR_GATHER:
    case IR_MAP: case IR_GREP:
        return 0;
    case IR_CALL: {
        if (!IR_LIT(e).sval) return 1;
        for (int _pi = 0; _pi < g_stage2.proc_count; _pi++) {
            if (!g_stage2.proc_table[_pi].name || strcmp(g_stage2.proc_table[_pi].name, IR_LIT(e).sval) != 0) continue;
            if (!bb_graph_of_proc(&g_stage2.proc_table[_pi])) return 0;
            if (g_stage2.proc_table[_pi].is_generator) return 0;
            for (int _ci = 0; ; _ci++) { IR_t * a = ir_call_arg(e, _ci); if (!a) break; if (!ir_is_single_shot(a)) return 0; }
            return 1;
        }
        if (!strcmp(IR_LIT(e).sval, "find") || !strcmp(IR_LIT(e).sval, "upto") || !strcmp(IR_LIT(e).sval, "any")
            || !strcmp(IR_LIT(e).sval, "many") || !strcmp(IR_LIT(e).sval, "bal") || !strcmp(IR_LIT(e).sval, "key")
            || !strcmp(IR_LIT(e).sval, "seq")) return 0;
        for (int _ci = 0; ; _ci++) { IR_t * a = ir_call_arg(e, _ci); if (!a) break; if (!ir_is_single_shot(a)) return 0; }
        return 1;
    }
    case IR_SCAN: return 1;
    default: {
        if (((IR_t*)0) && !ir_is_single_shot(((IR_t*)0))) return 0;
        if (((IR_t*)0) && !ir_is_single_shot(((IR_t*)0))) return 0;
        for (int _oi = 0; _oi < e->n_operands; _oi++) if (e->operands[_oi] && !ir_is_single_shot(e->operands[_oi])) return 0;
        return 1;
    }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int bb_is_gen_node(IR_t * e);
static int bb_is_gen_kind_raw(IR_e k) {
    return k == IR_TO || k == IR_TO_BY || k == IR_UPTO || k == IR_ALT ||
           k == IR_BINOP_GEN || k == IR_ITERATE || k == IR_LIMIT || k == IR_PROC_GEN ||
           k == IR_LIST_BANG || k == IR_KEY_GEN || k == IR_FIND_GEN || k == IR_SEQ_GEN || k == IR_GATHER || k == IR_MAP || k == IR_GREP;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int bb_is_gen_node(IR_t * e) {
    if (!e) return 0;
    if (e->op == IR_ASSIGN || e->op == IR_ASSIGN_LIT_S || e->op == IR_ASSIGN_LIT_I || e->op == IR_ASSIGN_VAR || e->op == IR_ASSIGN_CONCAT || e->op == IR_ASSIGN_CALL) return bb_is_gen_node(((IR_t*)0));
    return bb_is_gen_kind_raw(e->op);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * gen_resume_target(IR_t * e) {
    if (!e) return NULL;
    if (e->op == IR_ASSIGN || e->op == IR_ASSIGN_LIT_S || e->op == IR_ASSIGN_LIT_I || e->op == IR_ASSIGN_VAR || e->op == IR_ASSIGN_CONCAT || e->op == IR_ASSIGN_CALL) return gen_resume_target(((IR_t*)0));
    if (bb_is_gen_kind_raw(e->op)) return e;
    if (e->op == IR_BINOP) {
        int n2 = 0;
        IR_t * const * ax = bb_operand_aux_get(g_current_cfg, e, &n2);
        if (ax && n2 == 2) {
            IR_t * r = gen_resume_target(ax[1]);
            if (r) return r;
            return gen_resume_target(ax[0]);
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static Term *resolve_node_to_term(IR_t *bb) {
    extern Term **g_resolve_env;
    if (!bb) return NULL;
    switch (bb->op) {
    case IR_LOGICVAR: {
        int slot = (int)IR_LIT(bb).ival;
        Term *t = (g_resolve_env && slot >= 0 && g_resolve_env[slot]) ? term_deref(g_resolve_env[slot]) : NULL;
        if (!t) { t = term_new_var(slot); if (g_resolve_env && slot >= 0) g_resolve_env[slot] = t; }
        return t;
    }
    case IR_ATOM:  return term_new_atom(prolog_atom_intern(IR_LIT(bb).sval ? IR_LIT(bb).sval : "[]"));
    case IR_LIT_I: return term_new_int((long)IR_LIT(bb).ival);
    case IR_LIT_F: return term_new_float(IR_LIT(bb).dval);
    case IR_STRUCT: {
        int arity = (int)IR_LIT(bb).ival;
        if (arity <= 0) return term_new_atom(prolog_atom_intern(IR_LIT(bb).sval ? IR_LIT(bb).sval : "[]"));
        Term **args = (Term **)GC_MALLOC((size_t)arity * sizeof(Term *));
        for (int i = 0; i < arity; i++) { IR_t *a = ir_call_arg(bb, i); if (!a) break; args[i] = resolve_node_to_term(a); }
        return term_new_compound(prolog_atom_intern(IR_LIT(bb).sval ? IR_LIT(bb).sval : "[]"), arity, args);
    }
    case IR_ARITH: {
        int arity = (int)IR_LIT(bb).ival;
        const char *f = IR_LIT(bb).sval ? IR_LIT(bb).sval : "+";
        if (arity == 0) return term_new_atom(prolog_atom_intern(f));
        if (arity == 1) { Term **a=(Term**)GC_MALLOC(sizeof(Term*)); a[0]=resolve_node_to_term(bb->n_operands>0?bb->operands[0]:NULL); return term_new_compound(prolog_atom_intern(f),1,a); }
        Term **a=(Term**)GC_MALLOC(2*sizeof(Term*)); a[0]=resolve_node_to_term(bb->n_operands>0?bb->operands[0]:NULL); a[1]=resolve_node_to_term(bb->n_operands>1?bb->operands[1]:NULL);
        return term_new_compound(prolog_atom_intern(f),2,a);
    }
    default: {
        IR_interp_node(bb); DESCR_t v = IR_EXEC(bb).value;
        if (v.v == DT_I) return term_new_int((long)v.i);
        if (v.v == DT_R) return term_new_float(v.r);
        if ((v.v == DT_S || v.v == DT_SNUL) && v.s) return term_new_atom(prolog_atom_intern(v.s));
        return term_new_atom(prolog_atom_intern("[]"));
    }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void *rt_node_to_term_ptr(void *ir_node) { return (void *)resolve_node_to_term((IR_t *)ir_node); }
/*--------------------------------------------------------------------------------------------------------------------*/
static void resolve_format_float(char *buf, size_t bufsz, double d);
static const char *resolve_atomic_text(Term *t, char *buf, size_t bufsz) {
    t = t ? term_deref(t) : NULL;
    if (!t) return NULL;
    if (t->tag == TERM_ATOM)  return prolog_atom_name(t->atom_id);
    if (t->tag == TERM_INT)   { snprintf(buf, bufsz, "%ld", t->ival); return buf; }
    if (t->tag == TERM_FLOAT) { resolve_format_float(buf, bufsz, t->fval); return buf; }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { Term *orig; Term *copy; } BBCopyMap;
static Term *bb_copy_term_rec(Term *t, BBCopyMap *map, int *nmap) {
    t = t ? term_deref(t) : NULL;
    if (!t) return term_new_atom(prolog_atom_intern("[]"));
    switch (t->tag) {
    case TERM_VAR:
        for (int i=0;i<*nmap;i++) if (map[i].orig==t) return map[i].copy;
        { Term *nv=term_new_var(-1); if (*nmap<256){ map[*nmap].orig=t; map[*nmap].copy=nv; (*nmap)++; } return nv; }
    case TERM_ATOM:  return term_new_atom(t->atom_id);
    case TERM_INT:   return term_new_int(t->ival);
    case TERM_FLOAT: return term_new_float(t->fval);
    case TERM_COMPOUND: {
        int ar=t->compound.arity;
        Term **args=(Term**)GC_MALLOC((size_t)ar*sizeof(Term*));
        for (int i=0;i<ar;i++) args[i]=bb_copy_term_rec(t->compound.args[i],map,nmap);
        return term_new_compound(t->compound.functor,ar,args);
    }
    default: return term_new_atom(prolog_atom_intern("[]"));
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static Term *bb_copy_term(Term *t) { BBCopyMap map[256]; int n=0; return bb_copy_term_rec(t,map,&n); }
static int resolve_term_class(Term *t) {
    switch (t->tag) {
    case TERM_VAR: return 0;
    case TERM_FLOAT: case TERM_INT: return 1;
    case TERM_ATOM: return 2;
    case TERM_COMPOUND: return 3;
    default: return 4;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int resolve_term_compare(Term *a, Term *b) {
    a = a ? term_deref(a) : NULL; b = b ? term_deref(b) : NULL;
    if (!a && !b) return 0; if (!a) return -1; if (!b) return 1;
    int ca = resolve_term_class(a), cb = resolve_term_class(b);
    if (ca != cb) return ca < cb ? -1 : 1;
    switch (a->tag) {
    case TERM_VAR: return (a==b)?0:(a<b?-1:1);
    case TERM_INT: {
        double x=(double)a->ival, y=(b->tag==TERM_INT)?(double)b->ival:b->fval;
        return x<y?-1:(x>y?1:0);
    }
    case TERM_FLOAT: {
        double x=a->fval, y=(b->tag==TERM_INT)?(double)b->ival:b->fval;
        return x<y?-1:(x>y?1:0);
    }
    case TERM_ATOM: {
        const char *na=prolog_atom_name(a->atom_id), *nb=prolog_atom_name(b->atom_id);
        int c=strcmp(na?na:"",nb?nb:""); return c<0?-1:(c>0?1:0);
    }
    case TERM_COMPOUND: {
        if (a->compound.arity != b->compound.arity) return a->compound.arity < b->compound.arity ? -1 : 1;
        const char *na=prolog_atom_name(a->compound.functor), *nb=prolog_atom_name(b->compound.functor);
        int c=strcmp(na?na:"",nb?nb:""); if (c) return c<0?-1:1;
        for (int i=0;i<a->compound.arity;i++) { int r=resolve_term_compare(a->compound.args[i],b->compound.args[i]); if (r) return r; }
        return 0;
    }
    default: return 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
 static int resolve_term_is_ground(Term *t) {
    t = t ? term_deref(t) : NULL;
    if (!t) return 0;
    if (t->tag == TERM_VAR) return 0;
    if (t->tag == TERM_COMPOUND) {
        for (int i = 0; i < t->compound.arity; i++)
            if (!resolve_term_is_ground(t->compound.args[i])) return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int resolve_term_is_proper_list(Term *t) {
    extern int ATOM_DOT, ATOM_NIL;
    t = t ? term_deref(t) : NULL;
    while (t) {
        if (t->tag == TERM_ATOM && t->atom_id == ATOM_NIL) return 1;
        if (t->tag == TERM_COMPOUND && t->compound.functor == ATOM_DOT && t->compound.arity == 2) {
            t = term_deref(t->compound.args[1]);
            continue;
        }
        return 0;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t resolve_arith_eval(IR_t *bb) {
    extern Term **g_resolve_env;
    if (!bb) return FAILDESCR;
    switch (bb->op) {
    case IR_LIT_I: return INTVAL(IR_LIT(bb).ival);
    case IR_LIT_F: return REALVAL(IR_LIT(bb).dval);
    case IR_ATOM:
        if (IR_LIT(bb).sval && strcmp(IR_LIT(bb).sval,"pi")==0) return REALVAL(M_PI);
        if (IR_LIT(bb).sval && strcmp(IR_LIT(bb).sval,"e")==0)  return REALVAL(M_E);
        return FAILDESCR;
    case IR_LOGICVAR: {
        int slot = (int)IR_LIT(bb).ival;
        Term *t = (g_resolve_env && slot >= 0 && g_resolve_env[slot]) ? term_deref(g_resolve_env[slot]) : NULL;
        if (!t) return FAILDESCR;
        if (t->tag == TERM_INT)   return INTVAL((int64_t)t->ival);
        if (t->tag == TERM_FLOAT) return REALVAL(t->fval);
        return FAILDESCR;
    }
    case IR_ARITH: {
        const char *fn = IR_LIT(bb).sval ? IR_LIT(bb).sval : "+";
        int arity = (int)IR_LIT(bb).ival;
        if (arity == 0 || bb->n_operands == 0) {
            if (strcmp(fn,"pi")==0) return REALVAL(M_PI);
            if (strcmp(fn,"e")==0)  return REALVAL(M_E);
            return FAILDESCR;
        }
        DESCR_t lv = resolve_arith_eval(bb->operands[0]);
        if (IS_FAIL_fn(lv)) return FAILDESCR;
        int lf = (lv.v == DT_R);
        double ld = lf ? lv.r : (double)lv.i;
        int64_t li = lf ? (int64_t)lv.r : lv.i;
        if (arity == 1 || bb->n_operands < 2 || !bb->operands[1]) {
            if (strcmp(fn,"-")==0)  return lf ? REALVAL(-ld) : INTVAL(-li);
            if (strcmp(fn,"+")==0)  return lv;
            if (strcmp(fn,"abs")==0)  return lf ? REALVAL(fabs(ld)) : INTVAL(li<0?-li:li);
            if (strcmp(fn,"sign")==0) return lf ? REALVAL(ld>0?1.0:ld<0?-1.0:0.0) : INTVAL(li>0?1:li<0?-1:0);
            if (strcmp(fn,"sqrt")==0) return REALVAL(sqrt(ld));
            if (strcmp(fn,"sin")==0)  return REALVAL(sin(ld));
            if (strcmp(fn,"cos")==0)  return REALVAL(cos(ld));
            if (strcmp(fn,"tan")==0)  return REALVAL(tan(ld));
            if (strcmp(fn,"asin")==0) return REALVAL(asin(ld));
            if (strcmp(fn,"acos")==0) return REALVAL(acos(ld));
            if (strcmp(fn,"atan")==0) return REALVAL(atan(ld));
            if (strcmp(fn,"exp")==0)  return REALVAL(exp(ld));
            if (strcmp(fn,"log")==0)  return REALVAL(log(ld));
            if (strcmp(fn,"float")==0)              return REALVAL(ld);
            if (strcmp(fn,"integer")==0)            return INTVAL((int64_t)llround(ld));
            if (strcmp(fn,"float_integer_part")==0) return REALVAL(trunc(ld));
            if (strcmp(fn,"float_fractional_part")==0) return REALVAL(ld - trunc(ld));
            if (strcmp(fn,"truncate")==0) return INTVAL((int64_t)trunc(ld));
            if (strcmp(fn,"round")==0)    return INTVAL((int64_t)llround(ld));
            if (strcmp(fn,"ceiling")==0)  return INTVAL((int64_t)ceil(ld));
            if (strcmp(fn,"floor")==0)    return INTVAL((int64_t)floor(ld));
            if (strcmp(fn,"\\")==0)       return INTVAL(~li);
            if (strcmp(fn,"msb")==0)      { int64_t v=li; int m=-1; while(v){v>>=1;m++;} return INTVAL(m); }
            return FAILDESCR;
        }
        DESCR_t rv = resolve_arith_eval(bb->operands[1]);
        if (IS_FAIL_fn(rv)) return FAILDESCR;
        int rf = (rv.v == DT_R);
        double rd = rf ? rv.r : (double)rv.i;
        int64_t ri = rf ? (int64_t)rv.r : rv.i;
        int anyf = lf || rf;
        if (strcmp(fn,"+")==0) return anyf ? REALVAL(ld+rd) : INTVAL(li+ri);
        if (strcmp(fn,"-")==0) return anyf ? REALVAL(ld-rd) : INTVAL(li-ri);
        if (strcmp(fn,"*")==0) return anyf ? REALVAL(ld*rd) : INTVAL(li*ri);
        if (strcmp(fn,"/")==0) {
            if (anyf) return (rd==0.0) ? FAILDESCR : REALVAL(ld/rd);
            if (ri==0) return FAILDESCR;
            return (li % ri == 0) ? INTVAL(li/ri) : REALVAL((double)li/(double)ri);
        }
        if (strcmp(fn,"//")==0)  { if (ri==0) return FAILDESCR; return INTVAL((int64_t)(li/ri)); }
        if (strcmp(fn,"div")==0) { if (ri==0) return FAILDESCR; int64_t q=li/ri; if ((li%ri!=0)&&((li<0)!=(ri<0))) q--; return INTVAL(q); }
        if (strcmp(fn,"mod")==0) { if (ri==0) return FAILDESCR; int64_t m=li%ri; if (m!=0 && ((m<0)!=(ri<0))) m+=ri; return INTVAL(m); }
        if (strcmp(fn,"rem")==0) { if (ri==0) return FAILDESCR; return INTVAL(li%ri); }
        if (strcmp(fn,"**")==0) {
            if (!lf && !rf && ri>=0) { int64_t b=li,ex=ri,acc=1; while(ex-->0) acc*=b; return INTVAL(acc); }
            return REALVAL(pow(ld,rd));
        }
        if (strcmp(fn,"^")==0) {
            return REALVAL(pow(ld,rd));
        }
        if (strcmp(fn,"min")==0) return anyf ? (ld<=rd?lv:rv) : (li<=ri?lv:rv);
        if (strcmp(fn,"max")==0) return anyf ? (ld>=rd?lv:rv) : (li>=ri?lv:rv);
        if (strcmp(fn,"gcd")==0) { int64_t a=li<0?-li:li,b=ri<0?-ri:ri; while(b){int64_t r=a%b;a=b;b=r;} return INTVAL(a); }
        if (strcmp(fn,"/\\")==0) return INTVAL(li & ri);
        if (strcmp(fn,"\\/")==0) return INTVAL(li | ri);
        if (strcmp(fn,"xor")==0) return INTVAL(li ^ ri);
        if (strcmp(fn,"<<")==0)  return INTVAL(li << ri);
        if (strcmp(fn,">>")==0)  return INTVAL(li >> ri);
        return FAILDESCR;
    }
    default: {
        IR_interp_node(bb); DESCR_t v = IR_EXEC(bb).value;
        if (v.v == DT_I || v.v == DT_R) return v;
        return FAILDESCR;
    }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern long rt_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
int rt_is(int dst_slot, const char *op, int lk, long li, int rk, long ri) {
    extern Term **g_resolve_env; extern Trail g_resolve_trail;
    long r = rt_arith(lk, li, NULL, rk, ri, NULL, op);
    if (!g_resolve_env || dst_slot < 0) return 0;
    Term *vt = term_new_int(r);
    Term *lhs = g_resolve_env[dst_slot];
    if (!lhs) { g_resolve_env[dst_slot] = vt; return 1; }
    if (!unify(lhs, vt, &g_resolve_trail)) return 0;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_is_lint(long lval, const char *op, int lk, long li, int rk, long ri) {
    extern Trail g_resolve_trail;
    long r = rt_arith(lk, li, NULL, rk, ri, NULL, op);
    Term *vt = term_new_int(r);
    Term *lt = term_new_int(lval);
    if (!unify(lt, vt, &g_resolve_trail)) return 0;
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_is_eval(void *lhs_bb, void *rhs_bb) {
    extern Term **g_resolve_env; extern Trail g_resolve_trail;
    IR_t *lhs = (IR_t *)lhs_bb;
    IR_t *rhs = (IR_t *)rhs_bb;
    if (!lhs || !rhs || !g_resolve_env) return 0;
    int dst_slot = (int)IR_LIT(lhs).ival;
    if (dst_slot < 0) return 0;
    DESCR_t v = resolve_arith_eval(rhs);
    if (IS_FAIL_fn(v)) return 0;
    Term *vt = (v.v == DT_R) ? term_new_float(v.r) : term_new_int(v.i);
    Term *lhst = g_resolve_env[dst_slot];
    int mark = trail_mark(&g_resolve_trail);
    if (!lhst) { g_resolve_env[dst_slot] = vt; return 1; }
    if (!unify(lhst, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_is_cell(void *dst_cell, const char *op,
               int lk, void *larg, double ld,
               int rk, void *rarg, double rd) {
    extern Trail g_resolve_trail;
    if (!dst_cell) return 0;
    if (!op) op = "+";
    int    lf = 0; double ldv = 0.0; long liv = 0;
    switch (lk) {
    case IR_LIT_F:    lf = 1; ldv = ld; liv = (long)ld; break;
    case IR_LIT_I:    lf = 0; liv = (long)larg; ldv = (double)(long)larg; break;
    case IR_LOGICVAR: {
        Term *t = larg ? term_deref(*(Term **)larg) : NULL;
        if (t && t->tag == TERM_FLOAT)    { lf = 1; ldv = t->fval; liv = (long)t->fval; }
        else if (t && t->tag == TERM_INT) { lf = 0; liv = t->ival; ldv = (double)t->ival; }
        else return 0;
        break;
    }
    default: break;
    }
    int    have_r = (rk != -1);
    int    rf = 0; double rdv = 0.0; long riv = 0;
    if (have_r) {
        switch (rk) {
        case IR_LIT_F:    rf = 1; rdv = rd; riv = (long)rd; break;
        case IR_LIT_I:    rf = 0; riv = (long)rarg; rdv = (double)(long)rarg; break;
        case IR_LOGICVAR: {
            Term *t = rarg ? term_deref(*(Term **)rarg) : NULL;
            if (t && t->tag == TERM_FLOAT)    { rf = 1; rdv = t->fval; riv = (long)t->fval; }
            else if (t && t->tag == TERM_INT) { rf = 0; riv = t->ival; rdv = (double)t->ival; }
            else return 0;
            break;
        }
        default: break;
        }
    }
    Term *result = NULL;
    if (!strcmp(op, "pi"))      result = term_new_float(M_PI);
    else if (!strcmp(op, "e")) result = term_new_float(M_E);
    else if (!have_r) {
        if      (!strcmp(op, "-"))                     result = lf ? term_new_float(-ldv) : term_new_int(-liv);
        else if (!strcmp(op, "+"))                     result = lf ? term_new_float(ldv) : term_new_int(liv);
        else if (!strcmp(op, "abs"))                   result = lf ? term_new_float(fabs(ldv)) : term_new_int(liv < 0 ? -liv : liv);
        else if (!strcmp(op, "sign"))                  result = lf ? term_new_float(ldv > 0 ? 1.0 : ldv < 0 ? -1.0 : 0.0) : term_new_int(liv > 0 ? 1 : liv < 0 ? -1 : 0);
        else if (!strcmp(op, "sqrt"))                  result = term_new_float(sqrt(ldv));
        else if (!strcmp(op, "sin"))                   result = term_new_float(sin(ldv));
        else if (!strcmp(op, "cos"))                   result = term_new_float(cos(ldv));
        else if (!strcmp(op, "tan"))                   result = term_new_float(tan(ldv));
        else if (!strcmp(op, "asin"))                  result = term_new_float(asin(ldv));
        else if (!strcmp(op, "acos"))                  result = term_new_float(acos(ldv));
        else if (!strcmp(op, "atan"))                  result = term_new_float(atan(ldv));
        else if (!strcmp(op, "exp"))                   result = term_new_float(exp(ldv));
        else if (!strcmp(op, "log"))                   result = term_new_float(log(ldv));
        else if (!strcmp(op, "float"))                 result = term_new_float(ldv);
        else if (!strcmp(op, "integer"))               result = term_new_int((long)llround(ldv));
        else if (!strcmp(op, "float_integer_part"))    result = term_new_float(trunc(ldv));
        else if (!strcmp(op, "float_fractional_part")) result = term_new_float(ldv - trunc(ldv));
        else if (!strcmp(op, "truncate"))              result = term_new_int((long)trunc(ldv));
        else if (!strcmp(op, "round"))                 result = term_new_int((long)llround(ldv));
        else if (!strcmp(op, "ceiling"))               result = term_new_int((long)ceil(ldv));
        else if (!strcmp(op, "floor"))                 result = term_new_int((long)floor(ldv));
        else if (!strcmp(op, "\\"))                    { if (lf) return 0; result = term_new_int(~liv); }
        else if (!strcmp(op, "msb"))                   { if (lf || liv <= 0) return 0; long v=liv,m=-1; while(v){v>>=1;m++;} result = term_new_int(m); }
        else return 0;
    } else {
        int anyf = lf || rf;
        if      (!strcmp(op, "+")) result = anyf ? term_new_float(ldv + rdv) : term_new_int(liv + riv);
        else if (!strcmp(op, "-")) result = anyf ? term_new_float(ldv - rdv) : term_new_int(liv - riv);
        else if (!strcmp(op, "*")) result = anyf ? term_new_float(ldv * rdv) : term_new_int(liv * riv);
        else if (!strcmp(op, "/")) {
            if (anyf) { if (rdv == 0.0) return 0; result = term_new_float(ldv / rdv); }
            else { if (riv == 0) return 0; result = (liv % riv == 0) ? term_new_int(liv / riv) : term_new_float((double)liv / (double)riv); }
        }
        else if (!strcmp(op, "**") || !strcmp(op, "^")) {
            if (!lf && !rf && riv >= 0) { long b = liv, ex = riv, acc = 1; while (ex-- > 0) acc *= b; result = term_new_int(acc); }
            else result = term_new_float(pow(ldv, rdv));
        }
        else if (!strcmp(op, "min")) {
            if (!anyf) result = term_new_int(liv <= riv ? liv : riv);
            else if (ldv <= rdv) result = lf ? term_new_float(ldv) : term_new_int(liv);
            else result = rf ? term_new_float(rdv) : term_new_int(riv);
        }
        else if (!strcmp(op, "max")) {
            if (!anyf) result = term_new_int(liv >= riv ? liv : riv);
            else if (ldv >= rdv) result = lf ? term_new_float(ldv) : term_new_int(liv);
            else result = rf ? term_new_float(rdv) : term_new_int(riv);
        }
        else if (!strcmp(op, "//"))  { if (!riv) return 0; result = term_new_int(liv / riv); }
        else if (!strcmp(op, "mod")) { if (!riv) return 0; long r = liv % riv; if (r && (r<0) != (riv<0)) r += riv; result = term_new_int(r); }
        else if (!strcmp(op, "rem")) { if (!riv) return 0; result = term_new_int(liv % riv); }
        else if (!strcmp(op, "div")) { if (!riv) return 0; long q = liv/riv; if ((liv%riv!=0) && ((liv<0)!=(riv<0))) q--; result = term_new_int(q); }
        else if (!strcmp(op, "gcd")) { long a = liv<0?-liv:liv, b = riv<0?-riv:riv; while (b) { long r = a%b; a = b; b = r; } result = term_new_int(a); }
        else if (!strcmp(op, "/\\")) { if (anyf) return 0; result = term_new_int(liv & riv); }
        else if (!strcmp(op, "\\/")) { if (anyf) return 0; result = term_new_int(liv | riv); }
        else if (!strcmp(op, "xor")) { if (anyf) return 0; result = term_new_int(liv ^ riv); }
        else if (!strcmp(op, ">>"))  { if (anyf) return 0; result = term_new_int(liv >> riv); }
        else if (!strcmp(op, "<<"))  { if (anyf) return 0; result = term_new_int(liv << riv); }
        else return 0;
    }
    if (!result) return 0;
    Term *lhs = *(Term **)dst_cell;
    if (!lhs) { *(Term **)dst_cell = result; return 1; }
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(lhs, result, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* rt_is_cell_lit — like rt_is_cell but the destination is a literal integer value (IR_LIT_I LHS of is/2).
 * Computes the arithmetic result, then checks equality with lval.
 * No cell write, no trail — purely a deterministic equality test.
 * Signature mirrors rt_is_cell so the TEXT arm can share icm_arg_load_lit helpers. */
int rt_is_cell_lit(long lval, const char *op,
                   int lk, void *larg, double ld,
                   int rk, void *rarg, double rd) {
    if (!op) op = "+";
    int    lf = 0; double ldv = 0.0; long liv = 0;
    switch (lk) {
    case IR_LIT_F:    lf = 1; ldv = ld; liv = (long)ld; break;
    case IR_LIT_I:    lf = 0; liv = (long)larg; ldv = (double)(long)larg; break;
    case IR_LOGICVAR: {
        Term *t = larg ? term_deref(*(Term **)larg) : NULL;
        if (t && t->tag == TERM_FLOAT)    { lf = 1; ldv = t->fval; liv = (long)t->fval; }
        else if (t && t->tag == TERM_INT) { lf = 0; liv = t->ival; ldv = (double)t->ival; }
        else return 0;
        break;
    }
    default: break;
    }
    int    have_r = (rk != -1);
    int    rf = 0; double rdv = 0.0; long riv = 0;
    if (have_r) {
        switch (rk) {
        case IR_LIT_F:    rf = 1; rdv = rd; riv = (long)rd; break;
        case IR_LIT_I:    rf = 0; riv = (long)rarg; rdv = (double)(long)rarg; break;
        case IR_LOGICVAR: {
            Term *t = rarg ? term_deref(*(Term **)rarg) : NULL;
            if (t && t->tag == TERM_FLOAT)    { rf = 1; rdv = t->fval; riv = (long)t->fval; }
            else if (t && t->tag == TERM_INT) { rf = 0; riv = t->ival; rdv = (double)t->ival; }
            else return 0;
            break;
        }
        default: break;
        }
    }
    /* Evaluate: same op table as rt_is_cell, integer-only path (float result can never equal long lval exactly) */
    long result = 0; int got = 0; int anyf = lf || rf;
    if (!strcmp(op, "pi") || !strcmp(op, "e")) return 0; /* always float */
    if (!have_r) {
        if      (!strcmp(op, "-"))                     { if (anyf) return 0; result = -liv; got = 1; }
        else if (!strcmp(op, "+"))                     { if (anyf) return 0; result = liv;  got = 1; }
        else if (!strcmp(op, "abs"))                   { if (anyf) return 0; result = liv < 0 ? -liv : liv; got = 1; }
        else if (!strcmp(op, "sign"))                  { if (anyf) return 0; result = liv > 0 ? 1 : liv < 0 ? -1 : 0; got = 1; }
        else if (!strcmp(op, "truncate") || !strcmp(op, "integer") || !strcmp(op, "round") || !strcmp(op, "ceiling") || !strcmp(op, "floor"))
                                                       { if (anyf) result = (long)ldv; else result = liv; got = 1; }
        else if (!strcmp(op, "\\"))                    { if (anyf) return 0; result = ~liv; got = 1; }
        else if (!strcmp(op, "msb"))                   { if (anyf) return 0; long v=liv,m=-1; while(v){v>>=1;m++;} result=m; got=1; }
    } else {
        if (!strcmp(op, "+"))  { if (anyf) return 0; result = liv + riv; got = 1; }
        else if (!strcmp(op, "-"))  { if (anyf) return 0; result = liv - riv; got = 1; }
        else if (!strcmp(op, "*"))  { if (anyf) return 0; result = liv * riv; got = 1; }
        else if (!strcmp(op, "//")) { if (!riv) return 0; result = liv / riv; got = 1; }
        else if (!strcmp(op, "mod")){ if (!riv) return 0; long r = liv % riv; if (r && (r<0)!=(riv<0)) r+=riv; result = r; got = 1; }
        else if (!strcmp(op, "rem")){ if (!riv) return 0; result = liv % riv; got = 1; }
        else if (!strcmp(op, "**") || !strcmp(op, "^")) {
            if (!lf && !rf && riv >= 0) { long b=liv,ex=riv,acc=1; while(ex-->0) acc*=b; result=acc; got=1; }
        }
        else if (!strcmp(op, "max")) { if (anyf) return 0; result = liv > riv ? liv : riv; got = 1; }
        else if (!strcmp(op, "min")) { if (anyf) return 0; result = liv < riv ? liv : riv; got = 1; }
        else if (!strcmp(op, "/\\")) { if (anyf) return 0; result = liv & riv; got = 1; }
        else if (!strcmp(op, "\\/")) { if (anyf) return 0; result = liv | riv; got = 1; }
        else if (!strcmp(op, "xor")) { if (anyf) return 0; result = liv ^ riv; got = 1; }
        else if (!strcmp(op, ">>"))  { if (anyf) return 0; result = liv >> riv; got = 1; }
        else if (!strcmp(op, "<<"))  { if (anyf) return 0; result = liv << riv; got = 1; }
        else if (!strcmp(op, "gcd")) { if (anyf) return 0; long a=liv<0?-liv:liv,b=riv<0?-riv:riv; while(b){long r=a%b;a=b;b=r;} result=a; got=1; }
        else if (!strcmp(op, "div")) { if (!riv) return 0; long q=liv/riv; if((liv%riv!=0)&&((liv<0)!=(riv<0))) q--; result=q; got=1; }
        else if (!strcmp(op, "/"))   { if (anyf) return 0; if (!riv) return 0; result = (liv%riv==0) ? liv/riv : 0; if (liv%riv!=0) return 0; got=1; }
    }
    if (!got) return 0;
    return (result == lval) ? 1 : 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern void *rt_node_to_term(int kind, long ival, const char *sval, double dval);
int rt_succ(int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *xt = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *yt = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    Term *xd = xt ? term_deref(xt) : NULL;
    Term *yd = yt ? term_deref(yt) : NULL;
    if (xd && xd->tag == TERM_INT) {
        if (xd->ival < 0) { trail_unwind(&g_resolve_trail, mark); return 0; }
        Term *vt = term_new_int(xd->ival + 1);
        if (!unify(yt, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    if (yd && yd->tag == TERM_INT) {
        if (yd->ival <= 0) { trail_unwind(&g_resolve_trail, mark); return 0; }
        Term *vt = term_new_int(yd->ival - 1);
        if (!unify(xt, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    trail_unwind(&g_resolve_trail, mark);
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_plus(int k0, long i0, const char *s0,
               int k1, long i1, const char *s1,
               int k2, long i2, const char *s2) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *at = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *bt = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    Term *ct = (Term *)rt_node_to_term(k2, i2, s2, 0.0);
    Term *ad = at ? term_deref(at) : NULL;
    Term *bd = bt ? term_deref(bt) : NULL;
    Term *cd = ct ? term_deref(ct) : NULL;
    int ai = (ad && ad->tag == TERM_INT);
    int bi = (bd && bd->tag == TERM_INT);
    int ci = (cd && cd->tag == TERM_INT);
    Term *vt = NULL; Term *tgt = NULL;
    if (ai && bi)      { vt = term_new_int(ad->ival + bd->ival); tgt = ct; }
    else if (ai && ci) { vt = term_new_int(cd->ival - ad->ival); tgt = bt; }
    else if (bi && ci) { vt = term_new_int(cd->ival - bd->ival); tgt = at; }
    else { trail_unwind(&g_resolve_trail, mark); return 0; }
    if (!unify(tgt, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern void pl_write(Term *);
static int rt_format_walk(const char *fmt, Term *args_list) {
    Term *arg_cur = args_list;
    for (const char *p = fmt; *p; p++) {
        if (*p == '~' && *(p+1)) {
            p++;
            if (*p == 'n' || *p == 'N') { putchar('\n'); }
            else if (*p == 'i') {
                if (arg_cur && arg_cur->tag == TERM_COMPOUND
                    && arg_cur->compound.functor == ATOM_DOT && arg_cur->compound.arity == 2)
                    arg_cur = term_deref(arg_cur->compound.args[1]);
            }
            else if (*p == 'a' || *p == 'w' || *p == 'd' || *p == 'p') {
                Term *arg = NULL;
                if (arg_cur && arg_cur->tag == TERM_COMPOUND
                    && arg_cur->compound.functor == ATOM_DOT && arg_cur->compound.arity == 2) {
                    arg = term_deref(arg_cur->compound.args[0]);
                    arg_cur = term_deref(arg_cur->compound.args[1]);
                }
                if (arg) {
                    if (arg->tag == TERM_INT)        printf("%ld", (long)arg->ival);
                    else if (arg->tag == TERM_FLOAT) { char fb[64]; resolve_format_float(fb, sizeof fb, arg->fval); fputs(fb, stdout); }
                    else if (arg->tag == TERM_ATOM) { const char *n = prolog_atom_name(arg->atom_id); if (n) fputs(n, stdout); }
                    else pl_write(arg);
                }
            }
            else if (*p == '~') putchar('~');
            else if (*p == 't' || *p == 'r') {
                if (*p == 't') putchar('\t');
            }
        } else {
            putchar(*p);
        }
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static const char *rt_format_resolve(Term *fmt_t, char *fmtbuf, size_t bufsz) {
    fmt_t = fmt_t ? term_deref(fmt_t) : NULL;
    if (!fmt_t) return NULL;
    if (fmt_t->tag == TERM_ATOM) return prolog_atom_name(fmt_t->atom_id);
    if (fmt_t->tag == TERM_COMPOUND && fmt_t->compound.functor == ATOM_DOT) {
        size_t fi = 0;
        Term *c = fmt_t;
        while (c && c->tag == TERM_COMPOUND && c->compound.functor == ATOM_DOT
               && c->compound.arity == 2 && fi + 1 < bufsz) {
            Term *ch = term_deref(c->compound.args[0]);
            if (ch && ch->tag == TERM_INT) fmtbuf[fi++] = (char)ch->ival;
            else if (ch && ch->tag == TERM_ATOM) {
                const char *n = prolog_atom_name(ch->atom_id);
                if (n && n[0]) fmtbuf[fi++] = n[0];
            }
            c = term_deref(c->compound.args[1]);
        }
        fmtbuf[fi] = 0;
        return fmtbuf;
    }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_format(int arity, int k0, long i0, const char *s0,
                              int k1, long i1, const char *s1) {
    Term *fmt_t = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    char fmtbuf[1024];
    const char *fmt = rt_format_resolve(fmt_t, fmtbuf, sizeof fmtbuf);
    if (!fmt) return 0;
    Term *args_list = NULL;
    if (arity == 2) {
        args_list = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
        args_list = args_list ? term_deref(args_list) : NULL;
    }
    return rt_format_walk(fmt, args_list);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_format_term(int arity, int k0, long i0, const char *s0, void *args_term_ptr) {
    Term *fmt_t = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    char fmtbuf[1024];
    const char *fmt = rt_format_resolve(fmt_t, fmtbuf, sizeof fmtbuf);
    if (!fmt) return 0;
    Term *args_list = (arity == 2) ? (Term *)args_term_ptr : NULL;
    args_list = args_list ? term_deref(args_list) : NULL;
    return rt_format_walk(fmt, args_list);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern void *rt_node_to_term(int kind, long ival, const char *sval, double dval);
static const char *rt_atomic_text_helper(Term *t, char *buf, size_t bufsz) {
    t = t ? term_deref(t) : NULL;
    if (!t) return NULL;
    if (t->tag == TERM_ATOM) return prolog_atom_name(t->atom_id);
    if (t->tag == TERM_INT)  { snprintf(buf, bufsz, "%ld", t->ival); return buf; }
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_atom_length(int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    char buf[256]; const char *txt = rt_atomic_text_helper(t0, buf, sizeof buf);
    if (!txt) { trail_unwind(&g_resolve_trail, mark); return 0; }
    Term *vt = term_new_int((long)strlen(txt));
    if (!unify(t1, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int rt_case_atom_common(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int up) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    char buf[256]; const char *txt = rt_atomic_text_helper(t0, buf, sizeof buf);
    if (!txt) { trail_unwind(&g_resolve_trail, mark); return 0; }
    size_t n = strlen(txt); char *out = (char *)GC_MALLOC(n + 1);
    for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)txt[i]) : (char)tolower((unsigned char)txt[i]);
    out[n] = '\0';
    Term *vt = term_new_atom(prolog_atom_intern(out));
    if (!unify(t1, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_upcase_atom(int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    return rt_case_atom_common(k0, i0, s0, k1, i1, s1, 1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_downcase_atom(int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    return rt_case_atom_common(k0, i0, s0, k1, i1, s1, 0);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_char_type(int k0, long i0, const char *s0, const char *ty, int is_compound, int ki, long ii, const char *si) {
    extern Trail g_resolve_trail;
    if (!ty) return 0;
    int mark = trail_mark(&g_resolve_trail);
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    char b0[256]; const char *cs = rt_atomic_text_helper(t0, b0, sizeof b0);
    if (!cs || !cs[0]) { trail_unwind(&g_resolve_trail, mark); return 0; }
    unsigned char ch = (unsigned char)cs[0];
    if (is_compound) {
        Term *out = NULL;
        if (strcmp(ty, "digit") == 0)         { if (!isdigit(ch)) { trail_unwind(&g_resolve_trail, mark); return 0; } out = term_new_int((long)(ch - '0')); }
        else if (strcmp(ty, "to_lower") == 0) { char c2[2] = { (char)tolower(ch), 0 }; out = term_new_atom(prolog_atom_intern(c2)); }
        else if (strcmp(ty, "to_upper") == 0) { char c2[2] = { (char)toupper(ch), 0 }; out = term_new_atom(prolog_atom_intern(c2)); }
        else if (strcmp(ty, "upper") == 0)    { if (!isupper(ch)) { trail_unwind(&g_resolve_trail, mark); return 0; } char c2[2] = { (char)tolower(ch), 0 }; out = term_new_atom(prolog_atom_intern(c2)); }
        else if (strcmp(ty, "lower") == 0)    { if (!islower(ch)) { trail_unwind(&g_resolve_trail, mark); return 0; } char c2[2] = { (char)toupper(ch), 0 }; out = term_new_atom(prolog_atom_intern(c2)); }
        else if (strcmp(ty, "code") == 0)     { out = term_new_int((long)ch); }
        else { trail_unwind(&g_resolve_trail, mark); return 0; }
        Term *inner = (Term *)rt_node_to_term(ki, ii, si, 0.0);
        if (!unify(inner, out, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    int ok = 0;
    if      (strcmp(ty, "alpha") == 0)       ok = isalpha(ch);
    else if (strcmp(ty, "alnum") == 0)       ok = isalnum(ch);
    else if (strcmp(ty, "digit") == 0)       ok = isdigit(ch);
    else if (strcmp(ty, "space") == 0 || strcmp(ty, "white") == 0) ok = isspace(ch);
    else if (strcmp(ty, "upper") == 0)       ok = isupper(ch);
    else if (strcmp(ty, "lower") == 0)       ok = islower(ch);
    else if (strcmp(ty, "punct") == 0)       ok = ispunct(ch);
    else if (strcmp(ty, "graph") == 0)       ok = isgraph(ch);
    else if (strcmp(ty, "csym") == 0)        ok = (isalnum(ch) || ch == '_');
    else if (strcmp(ty, "csymf") == 0)       ok = (isalpha(ch) || ch == '_');
    else if (strcmp(ty, "end_of_line") == 0) ok = (ch == '\n' || ch == '\r');
    else if (strcmp(ty, "newline") == 0)     ok = (ch == '\n');
    else { trail_unwind(&g_resolve_trail, mark); return 0; }
    if (!ok) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_numbervars_term(void *t0, long start, int k2, long i2, const char *s2) {
    extern Trail g_resolve_trail;
    Term *term_arg = (Term *)t0;
    long counter = start;
    int atom_var_id = prolog_atom_intern("$VAR");
    Term *stack[2048]; int top = 0;
    if (term_arg) { stack[top] = term_arg; top++; }
    while (top > 0) {
        Term *t = term_deref(stack[--top]);
        if (!t) continue;
        if (t->tag == TERM_VAR) {
            Term **a = (Term **)GC_MALLOC(sizeof(Term *)); a[0] = term_new_int(counter++);
            Term *vt = term_new_compound(atom_var_id, 1, a);
            unify(t, vt, &g_resolve_trail);
        } else if (t->tag == TERM_COMPOUND) {
            for (int i = t->compound.arity - 1; i >= 0; i--) if (top < 2048) { stack[top] = t->compound.args[i]; top++; }
        }
    }
    Term *end_var = (Term *)rt_node_to_term(k2, i2, s2, 0.0);
    int mark = trail_mark(&g_resolve_trail);
    if (end_var && !unify(end_var, term_new_int(counter), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_atom_concat(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int k2, long i2, const char *s2) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    Term *t2 = (Term *)rt_node_to_term(k2, i2, s2, 0.0);
    char b0[256], b1[256];
    const char *x0 = rt_atomic_text_helper(t0, b0, sizeof b0);
    const char *x1 = rt_atomic_text_helper(t1, b1, sizeof b1);
    if (!x0 || !x1) { trail_unwind(&g_resolve_trail, mark); return 0; }
    size_t l0 = strlen(x0), l1 = strlen(x1);
    char *cat = (char *)GC_MALLOC(l0 + l1 + 1);
    memcpy(cat, x0, l0); memcpy(cat + l0, x1, l1); cat[l0 + l1] = '\0';
    Term *vt = term_new_atom(prolog_atom_intern(cat));
    if (!unify(t2, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_atom_string_pair(int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    Term *d0 = t0 ? term_deref(t0) : NULL;
    Term *d1 = t1 ? term_deref(t1) : NULL;
    char buf[256]; const char *txt = NULL; Term *dst = NULL;
    if (d0 && d0->tag != TERM_VAR) { txt = rt_atomic_text_helper(d0, buf, sizeof buf); dst = t1; }
    else if (d1 && d1->tag != TERM_VAR) { txt = rt_atomic_text_helper(d1, buf, sizeof buf); dst = t0; }
    if (!txt || !dst) { trail_unwind(&g_resolve_trail, mark); return 0; }
    Term *vt = term_new_atom(prolog_atom_intern(txt));
    if (!unify(dst, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_number_string_pair(int num_first, int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    Term *d0 = t0 ? term_deref(t0) : NULL;
    Term *d1 = t1 ? term_deref(t1) : NULL;
    Term *numNode = num_first ? d0 : d1; Term *txtNode = num_first ? d1 : d0;
    Term *numDst  = num_first ? t0 : t1; Term *txtDst  = num_first ? t1 : t0;
    if (numNode && numNode->tag != TERM_VAR) {
        char b[256]; const char *s = rt_atomic_text_helper(numNode, b, sizeof b);
        if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
        if (!unify(txtDst, term_new_atom(prolog_atom_intern(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    char b[256]; const char *s = txtNode ? rt_atomic_text_helper(txtNode, b, sizeof b) : NULL;
    if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
    char *endp = NULL; long iv = strtol(s, &endp, 10); Term *nt;
    if (endp && *endp == '\0') nt = term_new_int(iv);
    else { char *fe = NULL; double dv = strtod(s, &fe); if (!fe || *fe != '\0') { trail_unwind(&g_resolve_trail, mark); return 0; } nt = term_new_float(dv); }
    if (!unify(numDst, nt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_term_to_atom_term(void *t0, int k1, long i1, const char *s1) {
    extern Trail g_resolve_trail;
    extern char *pl_term_to_string(Term *);
    int mark = trail_mark(&g_resolve_trail);
    Term *d0 = t0 ? term_deref((Term *)t0) : NULL;
    if (!d0 || d0->tag == TERM_VAR) { trail_unwind(&g_resolve_trail, mark); return 0; }
    char *s = pl_term_to_string(d0);
    if (!s) { trail_unwind(&g_resolve_trail, mark); return 0; }
    Term *at = term_new_atom(prolog_atom_intern(s)); free(s);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    if (!unify(t1, at, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_atomic_list_concat_term(void *list, int arity,
                                  int ksep, long isep, const char *ssep,
                                  int kres, long ires, const char *sres) {
    extern Trail g_resolve_trail;
    extern int ATOM_DOT;
    int mark = trail_mark(&g_resolve_trail);
    int sep3 = (arity == 3);
    char sepbuf[256]; const char *sep = "";
    if (sep3) {
        Term *st = (Term *)rt_node_to_term(ksep, isep, ssep, 0.0);
        sep = resolve_atomic_text(st, sepbuf, sizeof sepbuf);
        if (!sep) sep = "";
    }
    Term *cur = list ? term_deref((Term *)list) : NULL;
    char out[1024]; size_t oi = 0; int first = 1;
    while (cur && cur->tag == TERM_COMPOUND && cur->compound.functor == ATOM_DOT && cur->compound.arity == 2) {
        Term *el = term_deref(cur->compound.args[0]); char eb[256];
        const char *es = resolve_atomic_text(el, eb, sizeof eb);
        if (!es) { trail_unwind(&g_resolve_trail, mark); return 0; }
        if (sep3 && !first) { size_t sl = strlen(sep); if (oi + sl < sizeof(out)) { memcpy(out + oi, sep, sl); oi += sl; } }
        first = 0;
        size_t el_n = strlen(es); if (oi + el_n >= sizeof(out)) break;
        memcpy(out + oi, es, el_n); oi += el_n;
        cur = term_deref(cur->compound.args[1]);
    }
    out[oi] = '\0';
    Term *rt = (Term *)rt_node_to_term(kres, ires, sres, 0.0);
    if (!unify(rt, term_new_atom(prolog_atom_intern(out)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int bb_body_has_live_choice(IR_graph_t *bbg);
int rt_findall(void *fs_ptr) {
    extern Trail g_resolve_trail;
    extern Term **g_resolve_env;
    extern int ATOM_DOT, ATOM_NIL;
    extern resolve_choice *g_resolve_bfr;
    extern resolve_choice *g_resolve_cut_barrier;
    extern int g_resolve_cut_flag;
    extern int g_resolve_b3_call_mark;
    bb_findall_state_t *fs = (bb_findall_state_t *)fs_ptr;
    if (!fs || !fs->gcfg) return 0;
    Term       **outer_env          = g_resolve_env;
    resolve_choice   *outer_bfr          = g_resolve_bfr;
    resolve_choice   *outer_barrier      = g_resolve_cut_barrier;
    int          outer_cut_flag     = g_resolve_cut_flag;
    int          outer_b3_mark      = g_resolve_b3_call_mark;
    IR_graph_t  *outer_redirect_cfg = g_resolve_tail_redirect_cfg;
    IR_t        *outer_redirect_ent = g_resolve_tail_redirect_entry;
    int mark = trail_mark(&g_resolve_trail);
    g_resolve_bfr              = NULL;
    g_resolve_cut_barrier      = NULL;
    g_resolve_cut_flag         = 0;
    g_resolve_b3_call_mark     = -1;
    g_resolve_tail_redirect_cfg   = NULL;
    g_resolve_tail_redirect_entry = NULL;
    Term **acc = (Term **)calloc(4096, sizeof(Term *)); int nacc = 0;
    bb_reset(fs->gcfg);
    DESCR_t res = IR_interp_once(fs->gcfg);
    int fa_safety = fs->gcfg->n * 256 + 4096;
    while (!IS_FAIL_fn(res) && fa_safety-- > 0) {
        if (nacc >= 4096) break;
        Term **goal_env = g_resolve_env;
        g_resolve_env = outer_env;
        acc[nacc++] = bb_copy_term(resolve_node_to_term(fs->tmpl));
        g_resolve_env = goal_env;
        if (!bb_body_has_live_choice(fs->gcfg)) break;
        g_resolve_tail_redirect_cfg   = NULL;
        g_resolve_tail_redirect_entry = NULL;
        res = IR_interp_resume(fs->gcfg);
    }
    g_resolve_env                 = outer_env;
    g_resolve_bfr                 = outer_bfr;
    g_resolve_cut_barrier         = outer_barrier;
    g_resolve_cut_flag            = outer_cut_flag;
    g_resolve_b3_call_mark        = outer_b3_mark;
    g_resolve_tail_redirect_cfg   = outer_redirect_cfg;
    g_resolve_tail_redirect_entry = outer_redirect_ent;
    trail_unwind(&g_resolve_trail, mark);
    Term *lst = term_new_atom(prolog_atom_intern("[]"));
    for (int i = nacc - 1; i >= 0; i--) {
        Term **c = (Term **)GC_MALLOC(2 * sizeof(Term *)); c[0] = acc[i]; c[1] = lst;
        lst = term_new_compound(ATOM_DOT, 2, c);
    }
    int mark2 = trail_mark(&g_resolve_trail);
    if (!unify(resolve_node_to_term(fs->result), lst, &g_resolve_trail)) {
        trail_unwind(&g_resolve_trail, mark2); return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int rt_meta_solve(void *goal_v, void **root_out);
extern int rt_meta_redo(void *root_v);
int rt_aggregate(void *agg_ptr) {
    extern Trail g_resolve_trail;
    extern Term **g_resolve_env;
    extern resolve_choice *g_resolve_bfr;
    extern resolve_choice *g_resolve_cut_barrier;
    extern int g_resolve_cut_flag;
    extern int g_resolve_b3_call_mark;
    bb_findall_state_t *as = (bb_findall_state_t *)agg_ptr;
    if (!as || !as->gcfg) return 0;
    Term       **outer_env          = g_resolve_env;
    resolve_choice   *outer_bfr          = g_resolve_bfr;
    resolve_choice   *outer_barrier      = g_resolve_cut_barrier;
    int          outer_cut_flag     = g_resolve_cut_flag;
    int          outer_b3_mark      = g_resolve_b3_call_mark;
    IR_graph_t  *outer_redirect_cfg = g_resolve_tail_redirect_cfg;
    IR_t        *outer_redirect_ent = g_resolve_tail_redirect_entry;
    Term *spec0 = as->tmpl ? term_deref(resolve_node_to_term(as->tmpl)) : (Term *)0;
    int mode_count = 0, mode_sum = 0, mode_max = 0, mode_min = 0;
    if (spec0 && spec0->tag == TERM_ATOM) { const char *fn = prolog_atom_name(spec0->atom_id); if (fn && strcmp(fn,"count") == 0) mode_count = 1; }
    else if (spec0 && spec0->tag == TERM_COMPOUND && spec0->compound.arity == 1) {
        const char *fn = prolog_atom_name(spec0->compound.functor);
        if      (fn && strcmp(fn,"sum") == 0) mode_sum = 1;
        else if (fn && strcmp(fn,"max") == 0) mode_max = 1;
        else if (fn && strcmp(fn,"min") == 0) mode_min = 1;
    }
    if (!mode_count && !mode_sum && !mode_max && !mode_min) return 0;
    int mark = trail_mark(&g_resolve_trail);
    g_resolve_bfr              = NULL;
    g_resolve_cut_barrier      = NULL;
    g_resolve_cut_flag         = 0;
    g_resolve_b3_call_mark     = -1;
    g_resolve_tail_redirect_cfg   = NULL;
    g_resolve_tail_redirect_entry = NULL;
    int64_t acc_count = 0; double acc_sum = 0, acc_max = 0, acc_min = 0; int acc_first = 1;
    bb_reset(as->gcfg);
    DESCR_t res = IR_interp_once(as->gcfg);
    int fa_safety = as->gcfg->n * 256 + 4096;
    while (!IS_FAIL_fn(res) && fa_safety-- > 0) {
        Term **goal_env = g_resolve_env;
        g_resolve_env = outer_env;
        if (mode_count) { acc_count++; }
        else {
            Term *sp = as->tmpl ? term_deref(resolve_node_to_term(as->tmpl)) : (Term *)0;
            if (sp && sp->tag == TERM_COMPOUND && sp->compound.arity == 1 && sp->compound.args[0]) {
                Term *vt = term_deref(sp->compound.args[0]);
                double v = (vt && vt->tag == TERM_INT) ? (double)vt->ival : (vt && vt->tag == TERM_FLOAT) ? vt->fval : 0.0;
                if (mode_sum) acc_sum += v;
                if (mode_max && (acc_first || v > acc_max)) acc_max = v;
                if (mode_min && (acc_first || v < acc_min)) acc_min = v;
                acc_first = 0;
            }
        }
        g_resolve_env = goal_env;
        if (!bb_body_has_live_choice(as->gcfg)) break;
        g_resolve_tail_redirect_cfg   = NULL;
        g_resolve_tail_redirect_entry = NULL;
        res = IR_interp_resume(as->gcfg);
    }
    g_resolve_env                 = outer_env;
    g_resolve_bfr                 = outer_bfr;
    g_resolve_cut_barrier         = outer_barrier;
    g_resolve_cut_flag            = outer_cut_flag;
    g_resolve_b3_call_mark        = outer_b3_mark;
    g_resolve_tail_redirect_cfg   = outer_redirect_cfg;
    g_resolve_tail_redirect_entry = outer_redirect_ent;
    trail_unwind(&g_resolve_trail, mark);
    Term *result_term = (Term *)0;
    if      (mode_count) result_term = term_new_int(acc_count);
    else if (mode_sum)   result_term = (acc_sum == (double)(int64_t)acc_sum) ? term_new_int((int64_t)acc_sum) : term_new_float(acc_sum);
    else if (mode_max)   result_term = (acc_max == (double)(int64_t)acc_max) ? term_new_int((int64_t)acc_max) : term_new_float(acc_max);
    else if (mode_min)   result_term = (acc_min == (double)(int64_t)acc_min) ? term_new_int((int64_t)acc_min) : term_new_float(acc_min);
    if (!result_term) return 0;
    int mark2 = trail_mark(&g_resolve_trail);
    if (!unify(resolve_node_to_term(as->result), result_term, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark2); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_findall_term(void *goal_v, void *tmpl_v, void *result_v) {
    extern Trail g_resolve_trail;
    extern int ATOM_DOT;
    extern Term **g_resolve_env;
    extern int g_resolve_cut_flag;
    extern resolve_choice *g_resolve_cut_barrier;
    Term **outer_env                 = g_resolve_env;
    int outer_cut_flag               = g_resolve_cut_flag;
    resolve_choice *outer_barrier    = g_resolve_cut_barrier;
    int mark = trail_mark(&g_resolve_trail);
    resolve_choice *entry_cp = resolve_cp_current();
    Term **acc = (Term **)calloc(4096, sizeof(Term *)); int nacc = 0;
    if (!acc) return 0;
    void *mroot = (void *)0;
    int ok = rt_meta_solve(goal_v, &mroot);
    int fa_safety = 1 << 20;
    while (ok && nacc < 4096 && fa_safety-- > 0) {
        acc[nacc++] = bb_copy_term(term_deref((Term *)tmpl_v));
        ok = rt_meta_redo(mroot);
    }
    resolve_cp_truncate(entry_cp);
    trail_unwind(&g_resolve_trail, mark);
    g_resolve_env         = outer_env;
    g_resolve_cut_flag    = outer_cut_flag;
    g_resolve_cut_barrier = outer_barrier;
    Term *lst = term_new_atom(prolog_atom_intern("[]"));
    for (int i = nacc - 1; i >= 0; i--) {
        Term **c = (Term **)GC_MALLOC(2 * sizeof(Term *)); c[0] = acc[i]; c[1] = lst;
        lst = term_new_compound(ATOM_DOT, 2, c);
    }
    free(acc);
    int mark2 = trail_mark(&g_resolve_trail);
    if (!unify(term_deref((Term *)result_v), lst, &g_resolve_trail)) {
        trail_unwind(&g_resolve_trail, mark2); return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_aggregate_all_meta(void *tmpl_v, void *goal_v, void *result_v) {
    extern Trail g_resolve_trail;
    extern Term **g_resolve_env;
    extern int g_resolve_cut_flag;
    extern resolve_choice *g_resolve_cut_barrier;
    Term *tmpl_d = tmpl_v ? term_deref((Term *)tmpl_v) : NULL;
    if (!tmpl_d) return 0;
    int mode_count = 0, mode_sum = 0, mode_max = 0, mode_min = 0;
    if (tmpl_d->tag == TERM_ATOM) {
        const char *fn2 = prolog_atom_name(tmpl_d->atom_id);
        if (fn2 && strcmp(fn2, "count") == 0) mode_count = 1;
    } else if (tmpl_d->tag == TERM_COMPOUND && tmpl_d->compound.arity == 1) {
        const char *fn2 = prolog_atom_name(tmpl_d->compound.functor);
        if      (fn2 && strcmp(fn2, "sum") == 0) mode_sum = 1;
        else if (fn2 && strcmp(fn2, "max") == 0) mode_max = 1;
        else if (fn2 && strcmp(fn2, "min") == 0) mode_min = 1;
    }
    if (!mode_count && !mode_sum && !mode_max && !mode_min) return 0;
    Term **outer_env                 = g_resolve_env;
    int outer_cut_flag               = g_resolve_cut_flag;
    resolve_choice *outer_barrier    = g_resolve_cut_barrier;
    int mark = trail_mark(&g_resolve_trail);
    resolve_choice *entry_cp = resolve_cp_current();
    int64_t acc_count = 0; double acc_sum = 0, acc_max = 0, acc_min = 0; int acc_first = 1;
    void *mroot = (void *)0;
    int ok = rt_meta_solve(goal_v, &mroot);
    int fa_safety = 1 << 20;
    while (ok && fa_safety-- > 0) {
        acc_count++;
        if (mode_sum || mode_max || mode_min) {
            if (tmpl_d->tag == TERM_COMPOUND && tmpl_d->compound.arity == 1 && tmpl_d->compound.args[0]) {
                Term *vt2 = term_deref(tmpl_d->compound.args[0]);
                double v2 = (vt2 && vt2->tag == TERM_INT) ? (double)vt2->ival : (vt2 && vt2->tag == TERM_FLOAT) ? vt2->fval : 0.0;
                if (mode_sum) acc_sum += v2;
                if (mode_max && (acc_first || v2 > acc_max)) acc_max = v2;
                if (mode_min && (acc_first || v2 < acc_min)) acc_min = v2;
                acc_first = 0;
            }
        }
        ok = rt_meta_redo(mroot);
    }
    resolve_cp_truncate(entry_cp);
    trail_unwind(&g_resolve_trail, mark);
    g_resolve_env         = outer_env;
    g_resolve_cut_flag    = outer_cut_flag;
    g_resolve_cut_barrier = outer_barrier;
    Term *result_term = NULL;
    if (mode_count)      result_term = term_new_int(acc_count);
    else if (mode_sum)   result_term = (acc_sum == (int64_t)acc_sum) ? term_new_int((int64_t)acc_sum) : term_new_float(acc_sum);
    else if (mode_max)   result_term = (acc_max == (int64_t)acc_max) ? term_new_int((int64_t)acc_max) : term_new_float(acc_max);
    else if (mode_min)   result_term = (acc_min == (int64_t)acc_min) ? term_new_int((int64_t)acc_min) : term_new_float(acc_min);
    if (!result_term) return 0;
    int mark2 = trail_mark(&g_resolve_trail);
    if (!unify(term_deref((Term *)result_v), result_term, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark2); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_catch(void *zc_ptr) {
    extern Trail g_resolve_trail; extern Term **g_resolve_env;
    bb_catch_state_t *zc = (bb_catch_state_t *)zc_ptr;
    if (!zc || !zc->goal_g) return 0;
    Term *catcher = resolve_node_to_term(zc->catcher);
    jmp_buf *jb_p = (jmp_buf *)resolve_catch_push(catcher, g_resolve_env);
    if (!jb_p) return 0;
    if (setjmp(*jb_p) == 0) {
        bb_reset(zc->goal_g);
        DESCR_t res = IR_interp_once(zc->goal_g);
        resolve_catch_pop_top();
        if (IS_FAIL_fn(res)) return 0;
        return 1;
    } else {
        int tm = resolve_catch_top_trail_mark();
        Term **saved_env = resolve_catch_top_env();
        trail_unwind(&g_resolve_trail, tm);
        if (saved_env) g_resolve_env = saved_env;
        resolve_catch_pop_top();
        Term *exc = resolve_catch_take_exception();
        if (exc) {
            int mark2 = trail_mark(&g_resolve_trail);
            if (!unify(catcher, exc, &g_resolve_trail)) {
                trail_unwind(&g_resolve_trail, mark2);
                resolve_throw_term(exc);
                return 0;
            }
        }
        if (!zc->rec_g) return 1;
        bb_reset(zc->rec_g);
        DESCR_t res = IR_interp_once(zc->rec_g);
        if (IS_FAIL_fn(res)) return 0;
        return 1;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_throw(void *alpha_ptr) {
    IR_t *alpha = (IR_t *)alpha_ptr;
    Term *ball = alpha ? resolve_node_to_term(alpha) : term_new_atom(prolog_atom_intern("error"));
    resolve_throw_term(ball);
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_throw_term(void *ball_v) {
    Term *b0 = ball_v ? term_deref((Term *)ball_v) : term_new_atom(prolog_atom_intern("error"));
    Term *ball = bb_copy_term(b0 ? b0 : (Term *)ball_v);
    resolve_throw_term(ball ? ball : b0);
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_catch_native(void *goal_fn_v, void *rec_fn_v, void *catcher_v) {
    extern Trail g_resolve_trail; extern Term **g_resolve_env;
    extern int rt_last_ok(void);
    Term *catcher = (Term *)catcher_v;
    if (!goal_fn_v) return 0;
    jmp_buf *jb_p = (jmp_buf *)resolve_catch_push(catcher, g_resolve_env);
    if (!jb_p) return 0;
    if (setjmp(*jb_p) == 0) {
        ((void (*)(void))goal_fn_v)();
        resolve_catch_pop_top();
        return rt_last_ok();
    } else {
        int tm = resolve_catch_top_trail_mark();
        void *cpm = resolve_catch_top_cp_mark();
        Term **saved_env = resolve_catch_top_env();
        trail_unwind(&g_resolve_trail, tm);
        resolve_cp_truncate((resolve_choice *)cpm);
        if (saved_env) g_resolve_env = saved_env;
        resolve_catch_pop_top();
        Term *exc = resolve_catch_take_exception();
        if (exc) {
            int mark2 = trail_mark(&g_resolve_trail);
            if (!unify(catcher, exc, &g_resolve_trail)) {
                trail_unwind(&g_resolve_trail, mark2);
                resolve_throw_term(exc);
                return 0;
            }
        }
        if (!rec_fn_v) return 1;
        ((void (*)(void))rec_fn_v)();
        return rt_last_ok();
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_copy_term(int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    Term *d0 = t0 ? term_deref(t0) : NULL;
    Term *cp = bb_copy_term(d0 ? d0 : t0);
    if (!unify(t1, cp, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_copy_term_term(void *t0, int k1, long i1, const char *s1) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *d0 = t0 ? term_deref((Term *)t0) : NULL;
    Term *cp = bb_copy_term(d0 ? d0 : (Term *)t0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    if (!unify(t1, cp, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_copy_term_terms(void *t0, void *t1) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *d0 = t0 ? term_deref((Term *)t0) : NULL;
    Term *cp = bb_copy_term(d0 ? d0 : (Term *)t0);
    if (!unify((Term *)t1, cp, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_nb_setval_term(void *key, void *val) {
    Term *kd = key ? term_deref((Term *)key) : NULL;
    if (!kd || kd->tag != TERM_ATOM) return 0;
    Term *vd = val ? term_deref((Term *)val) : NULL;
    resolve_nb_set(kd->atom_id, vd ? vd : (Term *)val);
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_nb_getval_term(void *key, int kres, long ires, const char *sres) {
    extern Trail g_resolve_trail;
    Term *kd = key ? term_deref((Term *)key) : NULL;
    if (!kd || kd->tag != TERM_ATOM) return 0;
    Term *stored = resolve_nb_get(kd->atom_id);
    if (!stored) return 0;
    int mark = trail_mark(&g_resolve_trail);
    Term *rt = (Term *)rt_node_to_term(kres, ires, sres, 0.0);
    if (!unify(rt, stored, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_aggregate_all_term(void *tmpl, void *goal, int kres, long ires, const char *sres) {
    extern Term **g_resolve_env; extern Trail g_resolve_trail;
    Term *tmpl_d = tmpl ? term_deref((Term *)tmpl) : NULL;
    Term *goal_d = goal ? term_deref((Term *)goal) : NULL;
    if (!tmpl_d || !goal_d) return 0;
    int mode_count = 0, mode_sum = 0, mode_max = 0, mode_min = 0;
    if (tmpl_d->tag == TERM_ATOM) {
        const char *fn2 = prolog_atom_name(tmpl_d->atom_id);
        if (fn2 && strcmp(fn2, "count") == 0) mode_count = 1;
    } else if (tmpl_d->tag == TERM_COMPOUND && tmpl_d->compound.arity == 1) {
        const char *fn2 = prolog_atom_name(tmpl_d->compound.functor);
        if      (fn2 && strcmp(fn2, "sum") == 0) mode_sum = 1;
        else if (fn2 && strcmp(fn2, "max") == 0) mode_max = 1;
        else if (fn2 && strcmp(fn2, "min") == 0) mode_min = 1;
    }
    if (!mode_count && !mode_sum && !mode_max && !mode_min) return 0;
    if (goal_d->tag != TERM_ATOM && goal_d->tag != TERM_COMPOUND) return 0;
    const char *gfn = (goal_d->tag == TERM_ATOM) ? prolog_atom_name(goal_d->atom_id) : prolog_atom_name(goal_d->compound.functor);
    int garity = (goal_d->tag == TERM_COMPOUND) ? goal_d->compound.arity : 0;
    char gkey[128]; snprintf(gkey, sizeof gkey, "%s/%d", gfn, garity);
    Resolve_PredEntry_BB *gpe = resolve_bb_lookup(gkey, garity);
    IR_graph_t *gcfg = bb_graph_of_pred(gpe);
    if (!gcfg) return 0;
    int mark0 = trail_mark(&g_resolve_trail);
    int gnslots = garity + 16;
    Term **genv = (Term **)calloc((size_t)gnslots, sizeof(Term *));
    for (int ai = 0; ai < garity; ai++) {
        genv[ai] = term_new_var(ai);
        if (goal_d->compound.args[ai]) unify(genv[ai], goal_d->compound.args[ai], &g_resolve_trail);
    }
    Term **saved_env = g_resolve_env; g_resolve_env = genv;
    bb_node_state_t *snap = bb_snapshot_state(gcfg);
    bb_reset(gcfg);
    int64_t acc_count = 0; double acc_sum = 0, acc_max = 0, acc_min = 0; int acc_first = 1;
    int safety2 = gcfg->n * 256 + 4096;
    DESCR_t gres = IR_interp_once(gcfg);
    while (!IS_FAIL_fn(gres) && safety2-- > 0) {
        acc_count++;
        if (mode_sum || mode_max || mode_min) {
            if (tmpl_d->tag == TERM_COMPOUND && tmpl_d->compound.arity == 1 && tmpl_d->compound.args[0]) {
                Term *vt2 = term_deref(tmpl_d->compound.args[0]);
                double v2 = (vt2 && vt2->tag == TERM_INT) ? (double)vt2->ival : (vt2 && vt2->tag == TERM_FLOAT) ? vt2->fval : 0.0;
                if (mode_sum) acc_sum += v2;
                if (mode_max && (acc_first || v2 > acc_max)) acc_max = v2;
                if (mode_min && (acc_first || v2 < acc_min)) acc_min = v2;
                acc_first = 0;
            }
        }
        gres = IR_interp_resume(gcfg);
    }
    bb_restore_state(gcfg, snap);
    g_resolve_env = saved_env; free(genv);
    trail_unwind(&g_resolve_trail, mark0);
    Term *result_term = NULL;
    if (mode_count)      result_term = term_new_int(acc_count);
    else if (mode_sum)   result_term = (acc_sum == (int64_t)acc_sum) ? term_new_int((int64_t)acc_sum) : term_new_float(acc_sum);
    else if (mode_max)   result_term = (acc_max == (int64_t)acc_max) ? term_new_int((int64_t)acc_max) : term_new_float(acc_max);
    else if (mode_min)   result_term = (acc_min == (int64_t)acc_min) ? term_new_int((int64_t)acc_min) : term_new_float(acc_min);
    if (!result_term) return 0;
    int mark2 = trail_mark(&g_resolve_trail);
    Term *res_t = (Term *)rt_node_to_term(kres, ires, sres, 0.0);
    if (!unify(res_t, result_term, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark2); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int atom_chars_codes_common(int as_codes, Term *t0, Term *t1) {
    extern int ATOM_DOT, ATOM_NIL;
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *d0 = t0 ? term_deref(t0) : NULL;
    if (d0 && d0->tag != TERM_VAR) {
        char buf[256]; const char *txt = rt_atomic_text_helper(d0, buf, sizeof buf);
        if (!txt) { trail_unwind(&g_resolve_trail, mark); return 0; }
        size_t n = strlen(txt);
        Term *lst = term_new_atom(prolog_atom_intern("[]"));
        for (size_t i = n; i > 0; i--) {
            unsigned char ch = (unsigned char)txt[i - 1];
            Term *el;
            if (as_codes) { el = term_new_int((long)ch); }
            else { char cs[2] = {(char)ch, '\0'}; el = term_new_atom(prolog_atom_intern(cs)); }
            Term **c = (Term **)GC_MALLOC(2 * sizeof(Term *)); c[0] = el; c[1] = lst;
            lst = term_new_compound(ATOM_DOT, 2, c);
        }
        if (!unify(t1, lst, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    Term *lt = t1 ? term_deref(t1) : NULL;
    Term *cur = lt;
    char out[1024]; size_t oi = 0;
    while (cur && cur->tag == TERM_COMPOUND && cur->compound.functor == ATOM_DOT && cur->compound.arity == 2) {
        Term *el = term_deref(cur->compound.args[0]);
        if (oi >= sizeof(out) - 1) { trail_unwind(&g_resolve_trail, mark); return 0; }
        if (as_codes) {
            if (!el || el->tag != TERM_INT) { trail_unwind(&g_resolve_trail, mark); return 0; }
            out[oi++] = (char)el->ival;
        } else {
            if (!el || el->tag != TERM_ATOM) { trail_unwind(&g_resolve_trail, mark); return 0; }
            const char *cn = prolog_atom_name(el->atom_id);
            out[oi++] = cn ? cn[0] : '?';
        }
        cur = term_deref(cur->compound.args[1]);
    }
    out[oi] = '\0';
    if (!unify(t0, term_new_atom(prolog_atom_intern(out)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_atom_chars_codes(int as_codes, int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    return atom_chars_codes_common(as_codes, t0, t1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_atom_chars_codes_term(int as_codes, int k0, long i0, const char *s0, void *t1) {
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    return atom_chars_codes_common(as_codes, t0, (Term *)t1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int type_test_common(const char *fn, Term *t) {
    Term *d = t ? term_deref(t) : NULL;
    int isvar = (!d || d->tag == TERM_VAR);
    if (!fn) return 0;
    if (strcmp(fn, "var")      == 0) return  isvar ? 1 : 0;
    if (strcmp(fn, "nonvar")   == 0) return !isvar ? 1 : 0;
    if (strcmp(fn, "atom")     == 0) return (d && d->tag == TERM_ATOM) ? 1 : 0;
    if (strcmp(fn, "integer")  == 0) return (d && d->tag == TERM_INT)  ? 1 : 0;
    if (strcmp(fn, "float")    == 0) return (d && d->tag == TERM_FLOAT) ? 1 : 0;
    if (strcmp(fn, "number")   == 0) return (d && (d->tag == TERM_INT || d->tag == TERM_FLOAT)) ? 1 : 0;
    if (strcmp(fn, "atomic")   == 0) return (d && (d->tag == TERM_ATOM || d->tag == TERM_INT || d->tag == TERM_FLOAT)) ? 1 : 0;
    if (strcmp(fn, "compound") == 0) return (d && d->tag == TERM_COMPOUND) ? 1 : 0;
    if (strcmp(fn, "callable") == 0) return (d && (d->tag == TERM_ATOM || d->tag == TERM_COMPOUND)) ? 1 : 0;
    if (strcmp(fn, "ground")   == 0) return resolve_term_is_ground(d) ? 1 : 0;
    if (strcmp(fn, "is_list")  == 0) return resolve_term_is_proper_list(d) ? 1 : 0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_type_test(const char *fn, int k0, long i0, const char *s0) {
    Term *t = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    return type_test_common(fn, t);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_type_test_term(const char *fn, void *t0) {
    return type_test_common(fn, (Term *)t0);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int sort_msort_common(int do_msort, Term *t0, Term *t1) {
    extern int ATOM_DOT, ATOM_NIL;
    extern Trail g_resolve_trail;
    if (!t0 || !t1) return 0;
    int mark = trail_mark(&g_resolve_trail);
    int dot_id = prolog_atom_intern(".");
    int nil_id = prolog_atom_intern("[]");
    Term *lst = term_deref(t0);
    Term *elems[4096]; int n = 0;
    Term *cur = lst;
    while (cur && cur->tag == TERM_COMPOUND && cur->compound.functor == dot_id && cur->compound.arity == 2 && n < 4096) {
        elems[n++] = term_deref(cur->compound.args[0]);
        cur = term_deref(cur->compound.args[1]);
    }
    for (int i = 1; i < n; i++) {
        Term *key = elems[i]; int j = i - 1;
        while (j >= 0 && resolve_term_compare(elems[j], key) > 0) { elems[j + 1] = elems[j]; j--; }
        elems[j + 1] = key;
    }
    int m = 0; int out_idx[4096];
    for (int i = 0; i < n; i++) {
        if (!do_msort && m > 0 && resolve_term_compare(elems[out_idx[m - 1]], elems[i]) == 0) continue;
        out_idx[m++] = i;
    }
    Term *result = term_new_atom(nil_id);
    for (int i = m - 1; i >= 0; i--) {
        Term *pair[2]; pair[0] = elems[out_idx[i]]; pair[1] = result;
        result = term_new_compound(dot_id, 2, pair);
    }
    if (!unify(t1, result, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_sort_msort(int do_msort, int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    return sort_msort_common(do_msort, t0, t1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_sort_msort_term(int do_msort, void *t0, int k1, long i1, const char *s1) {
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    return sort_msort_common(do_msort, (Term *)t0, t1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_term_cmp(const char *op, int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    if (!op) return 0;
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    int c = resolve_term_compare(t0, t1);
    if (strcmp(op, "==")   == 0) return (c == 0) ? 1 : 0;
    if (strcmp(op, "\\==") == 0) return (c != 0) ? 1 : 0;
    if (strcmp(op, "@<")   == 0) return (c <  0) ? 1 : 0;
    if (strcmp(op, "@>")   == 0) return (c >  0) ? 1 : 0;
    if (strcmp(op, "@=<")  == 0) return (c <= 0) ? 1 : 0;
    if (strcmp(op, "@>=")  == 0) return (c >= 0) ? 1 : 0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int rt_arith_cmp_extract(int k, long i, const char *s, double *out_d) {
    if (k == IR_LIT_I) { *out_d = (double)i; return 1; }
    Term *t = (Term *)rt_node_to_term(k, i, s, 0.0);
    Term *d = t ? term_deref(t) : NULL;
    if (!d) return 0;
    if (d->tag == TERM_INT)   { *out_d = (double)d->ival; return 1; }
    if (d->tag == TERM_FLOAT) { *out_d = d->fval;         return 1; }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_arith_cmp(const char *op, int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    if (!op) return 0;
    double l = 0.0, r = 0.0;
    if (!rt_arith_cmp_extract(k0, i0, s0, &l)) return 0;
    if (!rt_arith_cmp_extract(k1, i1, s1, &r)) return 0;
    if (strcmp(op, "=:=") == 0) return (l == r) ? 1 : 0;
    if (strcmp(op, "=\\=")== 0) return (l != r) ? 1 : 0;
    if (strcmp(op, "<")   == 0) return (l <  r) ? 1 : 0;
    if (strcmp(op, ">")   == 0) return (l >  r) ? 1 : 0;
    if (strcmp(op, "=<")  == 0) return (l <= r) ? 1 : 0;
    if (strcmp(op, "<=")  == 0) return (l <= r) ? 1 : 0;
    if (strcmp(op, ">=")  == 0) return (l >= r) ? 1 : 0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_int(void *lhs_cell, long val) {
    extern Trail g_resolve_trail;
    Term *lhs = (Term *)lhs_cell;
    if (!lhs) return 0;
    Term *vt = term_new_int(val);
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(term_deref(lhs), vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_float(void *lhs_cell, double val) {
    extern Trail g_resolve_trail;
    Term *lhs = (Term *)lhs_cell;
    if (!lhs) return 0;
    Term *vt = term_new_float(val);
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(term_deref(lhs), vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell(void *lhs_cell, void *rhs_node) {
    extern Trail g_resolve_trail;
    IR_t *rhs = (IR_t *)rhs_node;
    Term *lhs = (Term *)lhs_cell;
    if (!lhs || !rhs) return 0;
    DESCR_t v = resolve_arith_eval(rhs);
    if (IS_FAIL_fn(v)) return 0;
    Term *vt = (v.v == DT_R) ? term_new_float(v.r) : term_new_int(v.i);
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(term_deref(lhs), vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static double gz_eval_cell(void *cell, const IR_t *nd, int *ok) {
    *ok = 1;
    if (!nd) { *ok = 0; return 0.0; }
    if (nd->op == IR_LIT_I) return (double)IR_LIT(nd).ival;
    if (nd->op == IR_LIT_F) return IR_LIT(nd).dval;
    if (nd->op == IR_LOGICVAR) {
        Term *t = cell ? term_deref((Term *)cell) : (Term *)0;
        if (!t) { *ok = 0; return 0.0; }
        if (t->tag == TERM_INT)   return (double)t->ival;
        if (t->tag == TERM_FLOAT) return t->fval;
        *ok = 0; return 0.0;
    }
    DESCR_t v = resolve_arith_eval((IR_t *)nd);
    if (IS_FAIL_fn(v)) { *ok = 0; return 0.0; }
    return (v.v == DT_R) ? v.r : (double)v.i;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_arith_cmp_cells(const char *op, void *lhs_cell, void *lhs_nd, void *rhs_cell, void *rhs_nd) {
    if (!op) return 0;
    int lo = 0, ro = 0;
    double l = gz_eval_cell(lhs_cell, (IR_t *)lhs_nd, &lo);
    double r = gz_eval_cell(rhs_cell, (IR_t *)rhs_nd, &ro);
    if (!lo || !ro) return 0;
    if (strcmp(op,"=:=")==0) return (l==r)?1:0;
    if (strcmp(op,"=\=")==0) return (l!=r)?1:0;
    if (strcmp(op,"<"  )==0) return (l< r)?1:0;
    if (strcmp(op,">"  )==0) return (l> r)?1:0;
    if (strcmp(op,"=<" )==0) return (l<=r)?1:0;
    if (strcmp(op,"<=" )==0) return (l<=r)?1:0;
    if (strcmp(op,">=" )==0) return (l>=r)?1:0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_arith_cmp_cell_val(const char *op, void *lhs_cell, long lhs_ival, void *rhs_cell, long rhs_ival) {
    if (!op) return 0;
    double l = 0.0, r = 0.0;
    if (lhs_cell) {
        Term *t = term_deref((Term *)lhs_cell);
        if (!t) return 0;
        if (t->tag == TERM_INT)   l = (double)t->ival;
        else if (t->tag == TERM_FLOAT) l = t->fval;
        else return 0;
    } else { l = (double)lhs_ival; }
    if (rhs_cell) {
        Term *t = term_deref((Term *)rhs_cell);
        if (!t) return 0;
        if (t->tag == TERM_INT)   r = (double)t->ival;
        else if (t->tag == TERM_FLOAT) r = t->fval;
        else return 0;
    } else { r = (double)rhs_ival; }
    if (strcmp(op,"=:=")==0) return (l==r)?1:0;
    if (strcmp(op,"=\\=")==0) return (l!=r)?1:0;
    if (strcmp(op,"<"  )==0) return (l< r)?1:0;
    if (strcmp(op,">"  )==0) return (l> r)?1:0;
    if (strcmp(op,"=<" )==0) return (l<=r)?1:0;
    if (strcmp(op,"<=" )==0) return (l<=r)?1:0;
    if (strcmp(op,">=" )==0) return (l>=r)?1:0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_arith(void *lhs_cell, void *rhs_cell, const char *op, long rhs_ival) {
    extern Trail g_resolve_trail;
    Term *lhs = (Term *)lhs_cell;
    if (!lhs) return 0;
    double rv = 0.0;
    if (rhs_cell) {
        Term *t = term_deref((Term *)rhs_cell);
        if (!t) return 0;
        if (t->tag == TERM_INT)   rv = (double)t->ival;
        else if (t->tag == TERM_FLOAT) rv = t->fval;
        else return 0;
        if (!op) { }
        else if (strcmp(op,"+")==0) rv = rv + (double)rhs_ival;
        else if (strcmp(op,"-")==0) rv = rv - (double)rhs_ival;
        else if (strcmp(op,"*")==0) rv = rv * (double)rhs_ival;
        else if (strcmp(op,"mod")==0||strcmp(op,"rem")==0) { long li=(long)rv; if (!rhs_ival) return 0; rv=(double)(li%rhs_ival); }
    } else { rv = (double)rhs_ival; }
    long ival = (long)rv;
    Term *vt = ((double)ival == rv) ? term_new_int(ival) : term_new_float(rv);
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(term_deref(lhs), vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_bivar(void *lhs_cell, void *cell1, void *cell2, const char *op) {
    extern Trail g_resolve_trail;
    Term *lhs = (Term *)lhs_cell;
    if (!lhs || !cell1 || !cell2) return 0;
    Term *t1 = term_deref((Term *)cell1), *t2 = term_deref((Term *)cell2);
    if (!t1 || !t2) return 0;
    double a = (t1->tag == TERM_INT) ? (double)t1->ival : (t1->tag == TERM_FLOAT) ? t1->fval : -1e300;
    double b = (t2->tag == TERM_INT) ? (double)t2->ival : (t2->tag == TERM_FLOAT) ? t2->fval : -1e300;
    if (a == -1e300 || b == -1e300) return 0;
    double rv;
    if (!op || strcmp(op,"+")==0) rv = a + b;
    else if (strcmp(op,"-")==0) rv = a - b;
    else if (strcmp(op,"*")==0) rv = a * b;
    else if (strcmp(op,"/")==0) { if (!b) return 0; rv = a / b; }
    else if (strcmp(op,"mod")==0||strcmp(op,"rem")==0) { long la=(long)a,lb=(long)b; if (!lb) return 0; rv=(double)(la%lb); }
    else return 0;
    long ival = (long)rv;
    Term *vt = ((double)ival == rv) ? term_new_int(ival) : term_new_float(rv);
    int mark = trail_mark(&g_resolve_trail);
    if (!unify(term_deref(lhs), vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_arith_cmp_nodes(const char *op, void *lhs_node, void *rhs_node) {
    if (!op || !lhs_node || !rhs_node) return 0;
    DESCR_t lv = resolve_arith_eval((IR_t *)lhs_node);
    DESCR_t rv = resolve_arith_eval((IR_t *)rhs_node);
    if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) return 0;
    double l = (lv.v == DT_R) ? lv.r : (double)lv.i;
    double r = (rv.v == DT_R) ? rv.r : (double)rv.i;
    if (strcmp(op, "=:=") == 0) return (l == r) ? 1 : 0;
    if (strcmp(op, "=\\=") == 0) return (l != r) ? 1 : 0;
    if (strcmp(op, "<")   == 0) return (l <  r) ? 1 : 0;
    if (strcmp(op, ">")   == 0) return (l >  r) ? 1 : 0;
    if (strcmp(op, "=<")  == 0) return (l <= r) ? 1 : 0;
    if (strcmp(op, "<=")  == 0) return (l <= r) ? 1 : 0;
    if (strcmp(op, ">=")  == 0) return (l >= r) ? 1 : 0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_term_cmp_nodes(const char *op, void *lhs_node, void *rhs_node) {
    if (!op || !lhs_node || !rhs_node) return 0;
    Term *lt = (Term *)resolve_node_to_term((IR_t *)lhs_node);
    Term *rt = (Term *)resolve_node_to_term((IR_t *)rhs_node);
    int c = resolve_term_compare(lt, rt);
    if (strcmp(op, "==")   == 0) return (c == 0) ? 1 : 0;
    if (strcmp(op, "\\==")  == 0) return (c != 0) ? 1 : 0;
    if (strcmp(op, "@<")   == 0) return (c <  0) ? 1 : 0;
    if (strcmp(op, "@>")   == 0) return (c >  0) ? 1 : 0;
    if (strcmp(op, "@=<")  == 0) return (c <= 0) ? 1 : 0;
    if (strcmp(op, "@>=")  == 0) return (c >= 0) ? 1 : 0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void *rt_compound_build_n(const char *functor_name, int arity, void *args_ptr) {
    Term **args_in = (Term **)args_ptr;
    Term **args = (Term **)GC_MALLOC(arity * sizeof(Term *));
    for (int i = 0; i < arity; i++) args[i] = args_in[i];
    int fid = prolog_atom_intern(functor_name ? functor_name : "");
    return term_new_compound(fid, arity, args);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_term_cmp_terms(const char *op, void *t0, void *t1) {
    if (!op) return 0;
    int c = resolve_term_compare((Term *)t0, (Term *)t1);
    if (strcmp(op, "==")   == 0) return (c == 0) ? 1 : 0;
    if (strcmp(op, "\\==") == 0) return (c != 0) ? 1 : 0;
    if (strcmp(op, "@<")   == 0) return (c <  0) ? 1 : 0;
    if (strcmp(op, "@>")   == 0) return (c >  0) ? 1 : 0;
    if (strcmp(op, "@=<")  == 0) return (c <= 0) ? 1 : 0;
    if (strcmp(op, "@>=")  == 0) return (c >= 0) ? 1 : 0;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int functor_common(Term *t0, Term *t1, Term *t2) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *d0 = t0 ? term_deref(t0) : NULL;
    if (d0 && d0->tag != TERM_VAR) {
        Term *nameT, *arityT;
        if      (d0->tag == TERM_COMPOUND) { nameT = term_new_atom(d0->compound.functor); arityT = term_new_int(d0->compound.arity); }
        else if (d0->tag == TERM_ATOM)     { nameT = term_new_atom(d0->atom_id);          arityT = term_new_int(0); }
        else if (d0->tag == TERM_INT)      { nameT = term_new_int(d0->ival);              arityT = term_new_int(0); }
        else if (d0->tag == TERM_FLOAT)    { nameT = term_new_float(d0->fval);            arityT = term_new_int(0); }
        else { trail_unwind(&g_resolve_trail, mark); return 0; }
        if (!unify(t1, nameT, &g_resolve_trail) || !unify(t2, arityT, &g_resolve_trail)) {
            trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    Term *nT = t1 ? term_deref(t1) : NULL;
    Term *aT = t2 ? term_deref(t2) : NULL;
    if (!aT || aT->tag != TERM_INT) { trail_unwind(&g_resolve_trail, mark); return 0; }
    long ar = aT->ival;
    Term *built;
    if (ar == 0) {
        built = nT ? nT : term_new_atom(prolog_atom_intern("[]"));
    } else {
        if (!nT || nT->tag != TERM_ATOM) { trail_unwind(&g_resolve_trail, mark); return 0; }
        Term **args = (Term **)GC_MALLOC((size_t)ar * sizeof(Term *));
        for (long i = 0; i < ar; i++) args[i] = term_new_var(-1);
        built = term_new_compound(nT->atom_id, (int)ar, args);
    }
    if (!unify(t0, built, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_functor(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int k2, long i2, const char *s2) {
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    Term *t2 = (Term *)rt_node_to_term(k2, i2, s2, 0.0);
    return functor_common(t0, t1, t2);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_functor_term(void *t0, int k1, long i1, const char *s1, int k2, long i2, const char *s2) {
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    Term *t2 = (Term *)rt_node_to_term(k2, i2, s2, 0.0);
    return functor_common((Term *)t0, t1, t2);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int arg_common(Term *tN, Term *tT, Term *tA) {
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *dN = tN ? term_deref(tN) : NULL;
    Term *dT = tT ? term_deref(tT) : NULL;
    if (!dN || dN->tag != TERM_INT || !dT || dT->tag != TERM_COMPOUND) {
        trail_unwind(&g_resolve_trail, mark); return 0; }
    long n = dN->ival;
    if (n < 1 || n > dT->compound.arity) { trail_unwind(&g_resolve_trail, mark); return 0; }
    if (!unify(tA, dT->compound.args[n - 1], &g_resolve_trail)) {
        trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_arg(int k0, long i0, const char *s0, int k1, long i1, const char *s1, int k2, long i2, const char *s2) {
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    Term *t2 = (Term *)rt_node_to_term(k2, i2, s2, 0.0);
    return arg_common(t0, t1, t2);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_arg_term(int k0, long i0, const char *s0, void *t1, int k2, long i2, const char *s2) {
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t2 = (Term *)rt_node_to_term(k2, i2, s2, 0.0);
    return arg_common(t0, (Term *)t1, t2);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int univ_common(Term *t0, Term *t1) {
    extern int ATOM_DOT;
    extern Trail g_resolve_trail;
    int mark = trail_mark(&g_resolve_trail);
    Term *d0 = t0 ? term_deref(t0) : NULL;
    if (d0 && d0->tag != TERM_VAR) {
        Term *lst;
        if (d0->tag == TERM_COMPOUND) {
            lst = term_new_atom(prolog_atom_intern("[]"));
            for (int i = d0->compound.arity - 1; i >= 0; i--) {
                Term **c = (Term **)GC_MALLOC(2 * sizeof(Term *));
                c[0] = d0->compound.args[i]; c[1] = lst;
                lst = term_new_compound(ATOM_DOT, 2, c);
            }
            Term **c = (Term **)GC_MALLOC(2 * sizeof(Term *));
            c[0] = term_new_atom(d0->compound.functor); c[1] = lst;
            lst = term_new_compound(ATOM_DOT, 2, c);
        } else {
            Term **c = (Term **)GC_MALLOC(2 * sizeof(Term *));
            c[0] = d0; c[1] = term_new_atom(prolog_atom_intern("[]"));
            lst = term_new_compound(ATOM_DOT, 2, c);
        }
        if (!unify(t1, lst, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
        return 1;
    }
    Term *ld = t1 ? term_deref(t1) : NULL;
    Term *elems[64]; int ne = 0;
    Term *cur = ld;
    while (cur && cur->tag == TERM_COMPOUND && cur->compound.functor == ATOM_DOT && cur->compound.arity == 2) {
        if (ne >= 64) break;
        elems[ne++] = term_deref(cur->compound.args[0]);
        cur = term_deref(cur->compound.args[1]);
    }
    if (ne == 0) { trail_unwind(&g_resolve_trail, mark); return 0; }
    Term *built;
    if (ne == 1) { built = elems[0]; }
    else {
        Term *h = elems[0];
        if (!h || h->tag != TERM_ATOM) { trail_unwind(&g_resolve_trail, mark); return 0; }
        Term **args = (Term **)GC_MALLOC((size_t)(ne - 1) * sizeof(Term *));
        for (int i = 1; i < ne; i++) args[i - 1] = elems[i];
        built = term_new_compound(h->atom_id, ne - 1, args);
    }
    if (!unify(t0, built, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_univ(int k0, long i0, const char *s0, int k1, long i1, const char *s1) {
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    return univ_common(t0, t1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_univ_term(void *t0, int k1, long i1, const char *s1) {
    Term *t1 = (Term *)rt_node_to_term(k1, i1, s1, 0.0);
    return univ_common((Term *)t0, t1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_univ_term_list(int k0, long i0, const char *s0, void *t1) {
    Term *t0 = (Term *)rt_node_to_term(k0, i0, s0, 0.0);
    return univ_common(t0, (Term *)t1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_univ_term_term(void *t0, void *t1) {
    return univ_common((Term *)t0, (Term *)t1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void resolve_format_float(char *buf, size_t bufsz, double d) {
    for (int prec = 15; prec <= 17; prec++) {
        snprintf(buf, bufsz, "%.*g", prec, d);
        double rt = strtod(buf, NULL);
        if (rt == d) break;
    }
    if (!strpbrk(buf, ".eEnN")) {
        size_t n = strlen(buf);
        if (n + 2 < bufsz) { buf[n] = '.'; buf[n+1] = '0'; buf[n+2] = '\0'; }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static long g_pl_yield_seq = 1;
typedef struct { Term **callee_env; Term **saved_env; int trail_mark; int nslots;
                 bb_node_state_t *act; void *cp_floor; int disj_hint; } PlCallSt;
static int bb_body_has_live_choice(IR_graph_t *bbg) {
    if (!bbg) return 0;
    for (int i = 0; i < bbg->n; i++) {
        IR_t *bb = bbg->all[i];
        if (!bb) continue;
        if (bb->op == IR_CHOICE && IR_EXEC(bb).state > 0) {
            bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)IR_LIT(bb).ival;
            if (zc && zc->cp) {
                int in_ledger = 0;
                for (resolve_choice *c = resolve_cp_current(); c; c = c->parent) if (c == (resolve_choice *)zc->cp && c->resume == (void *)bb) { in_ledger = 1; break; }
                if (!in_ledger) continue;
            }
            return 1;
        }
        if (bb->op == IR_GOAL && IR_EXEC(bb).state > 0) {
            bb_goal_state_t *zg = (bb_goal_state_t *)(intptr_t)IR_LIT(bb).ival;
            PlCallSt *cs = zg ? (PlCallSt *)zg->cs : NULL;
            if (!cs) return 1;
            if (cs->disj_hint) return 1;
            resolve_choice *floor_ = (resolve_choice *)cs->cp_floor;
            if (!floor_) { if (resolve_cp_current()) return 1; continue; }
            if (resolve_cp_current() == floor_) continue;
            for (resolve_choice *c = resolve_cp_current(); c; c = c->parent) if (c == floor_) return 1;
            continue;
        }
        if (bb->op == IR_DISJ && IR_EXEC(bb).state > 0)
            return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int pl_callee_disj_hint(IR_graph_t *bbg) {
    if (!bbg) return 0;
    for (int i = 0; i < bbg->n; i++) {
        IR_t *bb = bbg->all[i];
        if (!bb) continue;
        if (bb->op == IR_DISJ && IR_EXEC(bb).state > 0) return 1;
        if (bb->op == IR_GOAL && IR_EXEC(bb).state > 0) {
            bb_goal_state_t *zg = (bb_goal_state_t *)(intptr_t)IR_LIT(bb).ival;
            PlCallSt *cs = zg ? (PlCallSt *)zg->cs : NULL;
            if (cs && cs->disj_hint) return 1;
        }
    }
    return 0;
}
static int bb_body_live_choice_cut_aware(IR_graph_t *bbg) {
    if (!bbg) return 0;
    bb_conj_state_t *zs = (bbg->entry && bbg->entry->op == IR_GCONJ) ? (bb_conj_state_t *)(intptr_t)IR_LIT(bbg->entry).ival : NULL;
    if (zs && zs->goals) {
        int cuti = -1;
        for (int i = 0; i < zs->ngoals; i++) if (zs->goals[i] && zs->goals[i]->op == IR_CUT && IR_EXEC(zs->goals[i]).state > 0) cuti = i;
        if (cuti >= 0) {
            for (int i = cuti + 1; i < zs->ngoals; i++) { IR_t *p = zs->goals[i]; if (p && (p->op == IR_GOAL || p->op == IR_CHOICE || p->op == IR_DISJ) && IR_EXEC(p).state > 0) return 1; }
            return 0;
        }
    }
    return bb_body_has_live_choice(bbg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int bb_body_cp_free_except_tail(IR_graph_t *bbg) {
    if (!bbg) return 0;
    for (int i = 0; i < bbg->n; i++) {
        IR_t *bb = bbg->all[i];
        if (!bb) continue;
        if (bb->op == IR_CHOICE || bb->op == IR_DISJ) return 0;
        if (bb->op == IR_GOAL && bb->γ.node != NULL)     return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static long resolve_term_first_arg_key(Term *t) {
    if (!t) return RESOLVE_IDX_NOKEY;
    t = term_deref(t);
    if (!t) return RESOLVE_IDX_NOKEY;
    switch (t->tag) {
    case TERM_VAR:      return RESOLVE_IDX_NOKEY;
    case TERM_REF:      return RESOLVE_IDX_NOKEY;
    case TERM_ATOM:     return RESOLVE_IDX_ATOM(t->atom_id);
    case TERM_INT:      return RESOLVE_IDX_INT(t->ival);
    case TERM_FLOAT:    return RESOLVE_IDX_FLT;
    case TERM_COMPOUND: return RESOLVE_IDX_CMP(t->compound.functor, t->compound.arity);
    default:            return RESOLVE_IDX_NOKEY;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int bb_body_single_solution(IR_graph_t *bbg) {
    if (!bbg) return 0;
    for (int i = 0; i < bbg->n; i++) {
        IR_t *bb = bbg->all[i];
        if (!bb) continue;
        if (bb->op == IR_CHOICE || bb->op == IR_DISJ || bb->op == IR_GOAL) return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * resolve_choice_unique_indexed_body(IR_graph_t *callee, Term *first_arg) {
    if (!callee || !callee->entry || callee->entry->op != IR_CHOICE) return NULL;
    bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)IR_LIT(callee->entry).ival;
    if (!zc || !zc->idx_ok || !zc->idx_key || zc->nbodies == 0) return NULL;
    long ckey = resolve_term_first_arg_key(first_arg);
    if (ckey == RESOLVE_IDX_NOKEY) return NULL;
    int cand = -1, ncand = 0;
    for (int ci = 0; ci < zc->nbodies; ci++) {
        long k = zc->idx_key[ci];
        if (k == RESOLVE_IDX_VAR || k == ckey) { ncand++; if (ncand == 1) cand = ci; else break; }
    }
    if (ncand != 1) return NULL;
    IR_graph_t *body = zc->bodies[cand];
    if (!bb_body_cp_free_except_tail(body)) return NULL;
    return body;
}
/*--------------------------------------------------------------------------------------------------------------------*/
long size_value(DESCR_t v, int * failed) {
    *failed = 0;
    if (IS_FAIL_fn(v)) { *failed = 1; return 0; }
    if (IS_INT_fn(v) || IS_REAL_fn(v)) return 0;
    if (v.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(v, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) return (long)(int)FIELD_GET_fn(v, "frame_size").i;
        if (v.u && v.u->type) return (long)v.u->type->nfields;
        return 0;
    }
    if (v.v == DT_T && v.tbl) {
        long cnt = 0;
        for (int b = 0; b < TABLE_BUCKETS; b++) for (TBPAIR_t * ep = v.tbl->buckets[b]; ep; ep = ep->next) cnt++;
        return cnt;
    }
    if (IS_CSET_fn(v)) {
        int klen = kw_cset_len(v.s);
        return klen >= 0 ? (long)klen : (v.s ? (long)strlen(v.s) : 0);
    }
    {
        const char * s = VARVAL_fn(v);
        return s ? (long)strlen(s) : 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int list_bang_at(DESCR_t obj, int64_t idx, DESCR_t * out) {
    if (obj.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(obj, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
            int n          = (int)FIELD_GET_fn(obj, "frame_size").i;
            DESCR_t ea     = FIELD_GET_fn(obj, "frame_elems");
            DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
            if (!elems || idx >= n) return 0;
            *out = elems[idx];
            return 1;
        }
        if (obj.u && obj.u->type && obj.u->type->nfields > 0) {
            int nf = obj.u->type->nfields;
            if (idx >= nf) return 0;
            *out = obj.u->fields[idx];
            return 1;
        }
    }
    if (obj.v == DT_T && obj.tbl) {
        TBBLK_t *tbl   = obj.tbl;
        int64_t  seen  = 0;
        for (int b = 0; b < TABLE_BUCKETS; b++) {
            for (TBPAIR_t *ep = tbl->buckets[b]; ep; ep = ep->next) {
                if (seen == idx) { *out = ep->val; return 1; }
                seen++;
            }
        }
        return 0;
    }
    {
        const char *s   = (obj.v == DT_S) ? obj.s : NULL;
        int64_t     slen = s ? (int64_t)(obj.slen > 0 ? obj.slen : strlen(s)) : 0;
        if (!s || idx >= slen) return 0;
        char *ch = GC_malloc(2);
        ch[0] = s[idx];
        ch[1] = '\0';
        *out = (DESCR_t){ .v = DT_S, .slen = 1, .s = ch };
        return 1;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_disj_arm_enter(IR_t * a) {
    if (a && a->op == IR_GCONJ) {
        bb_conj_state_t * zs = (bb_conj_state_t *)(intptr_t)IR_LIT(a).ival;
        if (zs && zs->goals && zs->ngoals > 0) return zs->goals[0];
    }
    return a;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pas_slot_read(GenFrame *f, int slot) {
    if (!f || slot < 0 || slot >= FRAME_SLOT_MAX) return NULVCL;
    if (f->slotref[slot].is_ref) {
        if (f->slotref[slot].frame) return pas_slot_read(f->slotref[slot].frame, f->slotref[slot].slot);
        return f->slotref[slot].name ? NV_GET_fn(f->slotref[slot].name) : NULVCL;
    }
    return f->env[slot];
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void pas_slot_write(GenFrame *f, int slot, DESCR_t v) {
    if (!f || slot < 0 || slot >= FRAME_SLOT_MAX) return;
    if (f->slotref[slot].is_ref) {
        if (f->slotref[slot].frame) { pas_slot_write(f->slotref[slot].frame, f->slotref[slot].slot, v); return; }
        if (f->slotref[slot].name) NV_SET_fn(f->slotref[slot].name, v);
        return;
    }
    f->env[slot] = v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static GenFrame *pas_base(GenFrame *f, int ld) {
    while (ld > 0 && f) { f = f->static_link; ld--; }
    return f;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int pas_uplevel_find(GenFrame *cur, const char *name, GenFrame **of, int *os) {
    if (!name) return 0;
    for (GenFrame *f = cur ? cur->static_link : NULL; f; f = f->static_link) {
        int slot = scope_get(&f->sc, name);
        if (slot >= 0 && slot < f->env_n) { *of = f; *os = slot; return 1; }
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void pas_loc_of_name(GenFrame *caller, const char *name, GenFrame **of, int *os, const char **on) {
    *of = NULL; *os = -1; *on = name;
    if (!caller || !name) return;
    int slot = scope_get(&caller->sc, name);
    if (slot < 0 || slot >= FRAME_SLOT_MAX) {
        GenFrame *uf; int us;
        if (pas_uplevel_find(caller, name, &uf, &us)) {
            if (uf->slotref[us].is_ref) { *of = uf->slotref[us].frame; *os = uf->slotref[us].slot; *on = uf->slotref[us].name; }
            else { *of = uf; *os = us; *on = NULL; }
        }
        return;
    }
    if (caller->slotref[slot].is_ref) { *of = caller->slotref[slot].frame; *os = caller->slotref[slot].slot; *on = caller->slotref[slot].name; }
    else { *of = caller; *os = slot; *on = NULL; }
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int rk_is_truthy(DESCR_t cv) {
    if (IS_FAIL_fn(cv)) return 0;
    if (IS_INT_fn(cv))  return cv.i != 0;
    if (IS_REAL_fn(cv)) return cv.r != 0.0;
    if (cv.v == DT_SNUL) return 0;
    const char *s = cv.s ? cv.s : "";
    return s[0] != '\0' && !(s[0] == '0' && s[1] == '\0');
}
/*--------------------------------------------------------------------------------------------------------------------*/
IR_t * IR_interp_node(IR_t * bb) {
    switch (bb->op) {
    case IR_LIT_I:
        IR_EXEC(bb).value = INTVAL(IR_LIT(bb).ival);
        return bb->γ.node;
    case IR_VAR: {
        if (frame_depth > 0 && IR_LIT(bb).sval) {
            int slot = scope_get(&FRAME.sc, IR_LIT(bb).sval);
            if (slot >= 0 && slot < FRAME.env_n) {
                if (FRAME.slotref[slot].is_ref) { IR_EXEC(bb).value = pas_slot_read(&FRAME, slot); return bb->γ.node; }
                DESCR_t sv = FRAME.env[slot];
                if (sv.v != 0) { IR_EXEC(bb).value = sv; return bb->γ.node; }
            } else if (g_current_cfg && g_current_cfg->lang == IR_LANG_PAS) {
                GenFrame *uf; int us;
                if (pas_uplevel_find(&FRAME, IR_LIT(bb).sval, &uf, &us)) { IR_EXEC(bb).value = pas_slot_read(uf, us); return bb->γ.node; }
            }
        }
        if (IR_LIT(bb).sval) {
            DESCR_t gv = NV_GET_fn(IR_LIT(bb).sval);
            IR_EXEC(bb).value = gv;
            return IS_FAIL_fn(gv) ? bb->ω.node : bb->γ.node;
        }
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_ASSIGN_LIT_S: case IR_ASSIGN_LIT_I:
    case IR_ASSIGN_VAR: case IR_ASSIGN_CONCAT: case IR_ASSIGN_CALL:
    case IR_ASSIGN: {
        DESCR_t val = ag_ring_peek(g_current_cfg, 0);
        if (IS_FAIL_fn(val)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        const char *name = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        int stored = 0;
        if (frame_depth > 0) {
            int slot = scope_get(&FRAME.sc, name);
            if (slot >= 0 && slot < FRAME.env_n) {
                if (FRAME.slotref[slot].is_ref) pas_slot_write(&FRAME, slot, val); else FRAME.env[slot] = val;
                stored = 1;
            } else if (g_current_cfg && g_current_cfg->lang == IR_LANG_PAS) {
                GenFrame *uf; int us;
                if (pas_uplevel_find(&FRAME, name, &uf, &us)) { pas_slot_write(uf, us, val); stored = 1; }
            }
        }
        if (!stored) NV_SET_fn(name, val);
        IR_EXEC(bb).value = val;
        return bb->γ.node;
    }
    case IR_VAR_FRAME: case IR_VAR_FRAME_REF: {
        GenFrame *f = (frame_depth > 0) ? &FRAME : NULL;
        for (int h = (int) IR_LIT(bb).dval; h > 0 && f; h--) f = f->static_link;
        IR_EXEC(bb).value = pas_slot_read(f, (int) IR_LIT(bb).ival);
        return bb->γ.node;
    }
    case IR_ASSIGN_FRAME: case IR_ASSIGN_FRAME_REF: {
        DESCR_t val = ag_ring_peek(g_current_cfg, 0);
        if (IS_FAIL_fn(val)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        GenFrame *f = (frame_depth > 0) ? &FRAME : NULL;
        for (int h = (int) IR_LIT(bb).dval; h > 0 && f; h--) f = f->static_link;
        pas_slot_write(f, (int) IR_LIT(bb).ival, val);
        IR_EXEC(bb).value = val;
        return bb->γ.node;
    }
    case IR_SWAP: {
        IR_t *l_var = bb->n_operands > 0 ? bb->operands[0] : NULL;
        IR_t *r_var = bb->n_operands > 1 ? bb->operands[1] : NULL;
        if (!l_var || !r_var) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (l_var->op != IR_VAR || r_var->op != IR_VAR || !IR_LIT(l_var).sval || !IR_LIT(r_var).sval) {
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        IR_interp_node(l_var);
        DESCR_t lv = IR_EXEC(l_var).value;
        if (IS_FAIL_fn(lv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(r_var);
        DESCR_t rv = IR_EXEC(r_var).value;
        if (IS_FAIL_fn(rv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int wrote_l = 0;
        if (frame_depth > 0) {
            int slot = scope_get(&FRAME.sc, IR_LIT(l_var).sval);
            if (slot >= 0 && slot < FRAME.env_n) { FRAME.env[slot] = rv; wrote_l = 1; }
        }
        if (!wrote_l) NV_SET_fn(IR_LIT(l_var).sval, rv);
        int wrote_r = 0;
        if (frame_depth > 0) {
            int slot = scope_get(&FRAME.sc, IR_LIT(r_var).sval);
            if (slot >= 0 && slot < FRAME.env_n) { FRAME.env[slot] = lv; wrote_r = 1; }
        }
        if (!wrote_r) NV_SET_fn(IR_LIT(r_var).sval, lv);
        IR_EXEC(bb).value = rv;
        return bb->γ.node;
    }
    case IR_CALL: {
        if (!IR_LIT(bb).sval) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if ((IR_LIT(bb).dval == 2.0 || IR_LIT(bb).dval == 5.0) && !strcmp(IR_LIT(bb).sval, "__rk_try")) {
            extern char g_script_exception[512];
            int nargs = (int) IR_LIT(bb).ival;
            IR_graph_t ** blks = (nargs > 0) ? (IR_graph_t **)(intptr_t) IR_EXEC(bb).counter : NULL;
            IR_graph_t * body = (blks && nargs >= 1) ? blks[0] : NULL;
            IR_graph_t * catcher = (blks && nargs >= 2) ? blks[1] : NULL;
            g_script_exception[0] = '\0';
            if (body) { bb_reset(body); IR_interp_once(body); }
            if (g_script_exception[0] != '\0') {
                if (catcher) { g_script_exception[0] = '\0'; bb_reset(catcher); IR_interp_once(catcher); }
                else { g_script_exception[0] = '\0'; }
            }
            IR_EXEC(bb).value = NULVCL; return bb->γ.node;
        }
        if (IR_LIT(bb).dval == 2.0 || IR_LIT(bb).dval == 5.0) {
            int nargs = (int) IR_LIT(bb).ival;
            if (nargs == 2 && IR_LIT(bb).sval && !strcmp(IR_LIT(bb).sval, "[]")) {
                IR_graph_t ** blks = nargs > 0 ? (IR_graph_t **)(intptr_t) IR_EXEC(bb).counter : NULL;
                IR_graph_t * ab0 = blks ? blks[0] : NULL; IR_graph_t * ab1 = blks ? blks[1] : NULL;
                if (!ab0 || !ab1) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                susp_gen_cache_t * gc = susp_gen_cache_get(bb);
                DESCR_t obj; DESCR_t idx;
                if (IR_EXEC(bb).state == 2) {
                    obj = (gc->count > 0) ? gc->items[0] : FAILDESCR;
                    idx = IR_interp_once(ab1);
                } else {
                    bb_reset(ab0); obj = IR_interp_once(ab0);
                    if (IS_FAIL_fn(obj)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    if (gc->count == 0) { gc->count = 1; gc->items = (DESCR_t *) GC_malloc(sizeof(DESCR_t)); gc->items[0] = obj; }
                    else gc->items[0] = obj;
                    bb_reset(ab1); idx = IR_interp_once(ab1);
                    IR_EXEC(bb).state = 2;
                }
                if (IS_FAIL_fn(obj) || IS_FAIL_fn(idx)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                DESCR_t sargs[2]; sargs[0] = obj; sargs[1] = idx; DESCR_t out = FAILDESCR;
                if (try_call_builtin_by_name("[]", sargs, 2, &out)) { IR_EXEC(bb).value = out; return IS_FAIL_fn(out) ? bb->ω.node : bb->γ.node; }
                IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
            DESCR_t * args = NULL;
            if (nargs > 0) {
                IR_graph_t ** blks = (IR_graph_t **)(intptr_t) IR_EXEC(bb).counter;
                args = (DESCR_t *) GC_malloc((size_t) nargs * sizeof(DESCR_t));
                for (int j = 0; j < nargs; j++) {
                    IR_graph_t * ab = blks ? blks[j] : NULL;
                    if (!ab) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    bb_reset(ab);
                    DESCR_t av = IR_interp_once(ab);
                    if (IS_FAIL_fn(av)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    args[j] = av;
                }
            }
            int upi = -1;
            for (int _pi = 0; _pi < g_stage2.proc_count; _pi++)
                if (g_stage2.proc_table[_pi].name && strcmp(g_stage2.proc_table[_pi].name, IR_LIT(bb).sval) == 0
                    && g_stage2.proc_table[_pi].bb_idx >= 0) { upi = _pi; break; }
            if (upi >= 0) {
                IR_graph_t * fg = bb_graph_of_proc(&g_stage2.proc_table[upi]);
                Scope * sc = &g_stage2.proc_table[upi].lower_sc;
                int np = g_stage2.proc_table[upi].nparams;
                if (!fg || frame_depth >= FRAME_STACK_MAX || g_sno_save_top + sc->n > SNO_SAVE_MAX) {
                    IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
                }
                int save_base = g_sno_save_top;
                for (int k = 0; k < sc->n; k++) {
                    const char * nm = sc->e[k].name; if (!nm) continue;
                    g_sno_save[g_sno_save_top].name = nm;
                    g_sno_save[g_sno_save_top].old  = NV_GET_fn(nm);
                    g_sno_save_top++;
                    NV_SET_fn(nm, (k < np && k < nargs) ? args[k] : NULVCL);
                }
                const char * saved_func = g_sno_cur_func;
                g_sno_cur_func = IR_LIT(bb).sval;
                GenFrame * _f = &frame_stack[frame_depth++];
                memset(_f, 0, sizeof *_f);
                DESCR_t _ring_save[AG_RING];
                int _ring_head = fg->ring_head, _ring_depth = fg->ring_depth;
                memcpy(_ring_save, fg->ring, sizeof _ring_save);
                bb_node_state_t * _snap = bb_snapshot_state(fg);
                bb_reset(fg);
                DESCR_t out = IR_interp_once(fg);
                if (frame_depth > 0 && FRAME.returning) { out = g_ir_return_val; FRAME.returning = 0; }
                frame_depth--;
                bb_restore_state(fg, _snap);
                memcpy(fg->ring, _ring_save, sizeof _ring_save);
                fg->ring_head = _ring_head; fg->ring_depth = _ring_depth;
                g_sno_cur_func = saved_func;
                for (int k = g_sno_save_top - 1; k >= save_base; k--)
                    NV_SET_fn(g_sno_save[k].name, g_sno_save[k].old);
                g_sno_save_top = save_base;
                IR_EXEC(bb).value = out;
                return IS_FAIL_fn(out) ? bb->ω.node : bb->γ.node;
            }
            DESCR_t out = FAILDESCR;
            if (try_call_builtin_by_name(IR_LIT(bb).sval, args, nargs, &out)) {
                IR_EXEC(bb).value = out;
                return IS_FAIL_fn(out) ? bb->ω.node : bb->γ.node;
            }
            if (FNCEX_fn(IR_LIT(bb).sval)) {
                out = APPLY_fn(IR_LIT(bb).sval, args, nargs);
                IR_EXEC(bb).value = out;
                return IS_FAIL_fn(out) ? bb->ω.node : bb->γ.node;
            }
            IR_EXEC(bb).value = FAILDESCR;
            return bb->ω.node;
        }
        if (IR_LIT(bb).dval == 3.0) {
            if (IR_EXEC(bb).state == 1) {
                susp_gen_cache_t * gc = susp_gen_cache_get(bb);
                if (IR_EXEC(bb).counter < (int64_t) gc->count) {
                    IR_EXEC(bb).value = gc->items[IR_EXEC(bb).counter];
                    IR_EXEC(bb).counter++;
                    return bb->γ.node;
                }
                IR_EXEC(bb).state = 0; IR_EXEC(bb).counter = 0; IR_EXEC(bb).value = FAILDESCR;
                return bb->ω.node;
            }
            int nargs = (int) IR_LIT(bb).ival;
            DESCR_t * args = NULL;
            if (nargs > 0) {
                IR_graph_t ** blks = (IR_graph_t **)(intptr_t) IR_EXEC(bb).counter;
                args = (DESCR_t *) GC_malloc((size_t) nargs * sizeof(DESCR_t));
                for (int j = 0; j < nargs; j++) {
                    IR_graph_t * ab = blks ? blks[j] : NULL;
                    if (!ab) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    bb_reset(ab);
                    DESCR_t av = IR_interp_once(ab);
                    if (IS_FAIL_fn(av)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    args[j] = av;
                }
            }
            int upi = -1;
            for (int _pi = 0; _pi < g_stage2.proc_count; _pi++)
                if (g_stage2.proc_table[_pi].name && strcmp(g_stage2.proc_table[_pi].name, IR_LIT(bb).sval) == 0
                    && g_stage2.proc_table[_pi].bb_idx >= 0) { upi = _pi; break; }
            if (upi >= 0) {
                IR_graph_t * fg = bb_graph_of_proc(&g_stage2.proc_table[upi]);
                if (!fg || frame_depth >= FRAME_STACK_MAX) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                int is_gen = g_stage2.proc_table[upi].is_generator;
                GenFrame * caller = (frame_depth > 0) ? &FRAME : NULL;
                uint64_t bmask = g_stage2.proc_table[upi].byref_mask;
                IR_graph_t ** call_blks = (IR_graph_t **)(intptr_t) IR_EXEC(bb).counter;
                GenFrame * _f = &frame_stack[frame_depth++];
                memset(_f, 0, sizeof *_f);
                if (g_current_cfg && g_current_cfg->lang == IR_LANG_PAS) {
                    int callee_dl = g_stage2.proc_table[upi].decl_level;
                    int caller_lvl = caller ? caller->level : 1;
                    _f->static_link = pas_base(caller, caller_lvl - callee_dl);
                    _f->level = callee_dl + 1;
                }
                Scope * lsc = &g_stage2.proc_table[upi].lower_sc;
                int np = g_stage2.proc_table[upi].nparams;
                for (int k = 0; k < lsc->n && k < FRAME_SLOT_MAX; k++) {
                    if (!lsc->e[k].name) continue;
                    int slot = scope_add(&_f->sc, lsc->e[k].name);
                    if (slot < 0 || slot >= FRAME_SLOT_MAX) continue;
                    if (k >= np) { _f->env[slot] = NULVCL; continue; }
                    _f->env[slot] = (k < nargs) ? args[k] : NULVCL;
                    if ((bmask & (1ull << k)) && k < nargs && call_blks && call_blks[k] && call_blks[k]->entry
                        && (call_blks[k]->entry->op == IR_VAR || call_blks[k]->entry->op == IR_VAR_FRAME || call_blks[k]->entry->op == IR_VAR_FRAME_REF) && IR_LIT(call_blks[k]->entry).sval) {
                        GenFrame * hf; int hs; const char * hn;
                        pas_loc_of_name(caller, IR_LIT(call_blks[k]->entry).sval, &hf, &hs, &hn);
                        _f->slotref[slot].is_ref = 1; _f->slotref[slot].frame = hf; _f->slotref[slot].slot = hs; _f->slotref[slot].name = hn;
                    }
                }
                _f->env_n = _f->sc.n > 0 ? _f->sc.n : 1;
                DESCR_t _ring_save[AG_RING];
                int _ring_head = fg->ring_head, _ring_depth = fg->ring_depth;
                memcpy(_ring_save, fg->ring, sizeof _ring_save);
                bb_node_state_t * _snap = bb_snapshot_state(fg);
                SuspendBuf _sb_save = g_suspend_buf;
                if (is_gen) { g_suspend_buf.items = NULL; g_suspend_buf.count = 0; g_suspend_buf.cap = 0; g_suspend_buf.active = 1; }
                bb_reset(fg);
                DESCR_t out = IR_interp_once(fg);
                if (frame_depth > 0 && FRAME.returning) { out = g_ir_return_val; FRAME.returning = 0; }
                frame_depth--;
                bb_restore_state(fg, _snap);
                memcpy(fg->ring, _ring_save, sizeof _ring_save);
                fg->ring_head = _ring_head; fg->ring_depth = _ring_depth;
                if (is_gen) {
                    int collected = g_suspend_buf.count;
                    susp_gen_cache_t * gc = susp_gen_cache_get(bb);
                    gc->count = collected;
                    gc->items = (collected > 0) ? (DESCR_t *) GC_malloc((size_t) collected * sizeof(DESCR_t)) : NULL;
                    if (gc->items && collected > 0) memcpy(gc->items, g_suspend_buf.items, (size_t) collected * sizeof(DESCR_t));
                    g_suspend_buf = _sb_save;
                    if (collected > 0) { IR_EXEC(bb).state = 1; IR_EXEC(bb).counter = 1; IR_EXEC(bb).value = gc->items[0]; return bb->γ.node; }
                    IR_EXEC(bb).state = 0; IR_EXEC(bb).counter = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
                }
                IR_EXEC(bb).value = out;
                return IS_FAIL_fn(out) ? bb->ω.node : bb->γ.node;
            }
            DESCR_t out = FAILDESCR;
            if (try_call_builtin_by_name(IR_LIT(bb).sval, args, nargs, &out)) {
                int _is_strgen = IR_LIT(bb).sval && (!strcmp(IR_LIT(bb).sval,"find")||!strcmp(IR_LIT(bb).sval,"upto"));
                if (_is_strgen && !IS_FAIL_fn(out) && nargs >= 2) {
                    const char *_needle = VARVAL_fn(args[0]);
                    const char *_hay    = VARVAL_fn(args[1]);
                    if (_needle && _hay) {
                        int _nlen = (int)strlen(_needle), _hlen = (int)strlen(_hay);
                        int _cnt = 0;
                        for (int _p = 0; _p <= _hlen - _nlen; _p++) if (memcmp(_hay+_p,_needle,(size_t)_nlen)==0) _cnt++;
                        susp_gen_cache_t *_gc = susp_gen_cache_get(bb);
                        _gc->count = _cnt;
                        _gc->items = _cnt > 0 ? (DESCR_t *)GC_malloc((size_t)_cnt*sizeof(DESCR_t)) : NULL;
                        if (_gc->items) { int _k=0; for (int _p=0;_p<=_hlen-_nlen;_p++) if (memcmp(_hay+_p,_needle,(size_t)_nlen)==0) _gc->items[_k++]=INTVAL(_p+1); }
                        if (_cnt > 0) { IR_EXEC(bb).state=1; IR_EXEC(bb).counter=1; IR_EXEC(bb).value=_gc->items[0]; return bb->γ.node; }
                    }
                }
                IR_EXEC(bb).value = out;
                return IS_FAIL_fn(out) ? bb->ω.node : bb->γ.node;
            }
            IR_EXEC(bb).value = FAILDESCR;
            return bb->ω.node;
        }
        if (IR_EXEC(bb).state == 1 && IR_EXEC(bb).counter) {
            GeneratorState *gs = (GeneratorState *)(intptr_t)IR_EXEC(bb).counter;
            DESCR_t v;
            int ok = bb_broker_drive_sm_one(gs, &v);
            if (!ok) { IR_EXEC(bb).state = 0; IR_EXEC(bb).counter = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = v;
            return bb->γ.node;
        }
        int is_deep = (IR_LIT(bb).dval == 1.0) ? 1 : 0;
        int nargs   = (int)IR_LIT(bb).ival;
        if (is_deep && nargs == 2 && IR_LIT(bb).sval && !strcmp(IR_LIT(bb).sval, "[]") && bb->n_operands == 2) {
            IR_t *a0 = bb->operands[0]; IR_t *a1 = bb->operands[1];
            if (!a0 || !a1) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            DESCR_t obj = IR_EXEC(a0).value;
            DESCR_t idx = IR_EXEC(a1).value;
            if (IS_FAIL_fn(obj) || IS_FAIL_fn(idx)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            DESCR_t sargs2[2]; sargs2[0] = obj; sargs2[1] = idx; DESCR_t out2 = FAILDESCR;
            if (try_call_builtin_by_name("[]", sargs2, 2, &out2)) { IR_EXEC(bb).value = out2; return IS_FAIL_fn(out2) ? bb->ω.node : bb->γ.node; }
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        int has_gen_arg = 0;
        if (nargs > 0 && !is_deep) {
            for (int j = 0; j < nargs; j++) { IR_t *gx = ir_call_arg(bb, j); if (!gx) break; if (!ir_is_single_shot(gx)) { has_gen_arg = 1; break; } }
        }
        int is_plain_builtin = 0;
        if (has_gen_arg) {
            int is_user_proc = 0;
            for (int _pc = 0; _pc < g_stage2.proc_count; _pc++)
                if (g_stage2.proc_table[_pc].name && strcmp(g_stage2.proc_table[_pc].name, IR_LIT(bb).sval) == 0) { is_user_proc = 1; break; }
            is_plain_builtin = !is_user_proc;
        }
        if (has_gen_arg && is_plain_builtin) {
            DESCR_t *oargs = (DESCR_t *)GC_malloc((size_t)nargs * sizeof(DESCR_t));
            if (IR_EXEC(bb).state == 2) {
                IR_t **argv = (IR_t **)GC_malloc((size_t)nargs * sizeof(IR_t *));
                for (int j = 0; j < nargs; j++) argv[j] = ir_call_arg(bb, j);
                int advanced = 0;
                for (int j = nargs - 1; j >= 0; j--) {
                    if (ir_is_single_shot(argv[j])) continue;
                    IR_interp_node(argv[j]);
                    if (!IS_FAIL_fn(IR_EXEC(argv[j]).value)) { advanced = 1; break; }
                    IR_EXEC(argv[j]).state = 0; IR_interp_node(argv[j]);
                    if (IS_FAIL_fn(IR_EXEC(argv[j]).value)) { advanced = 0; break; }
                }
                if (!advanced) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                for (int j = 0; j < nargs; j++) {
                    oargs[j] = IR_EXEC(argv[j]).value;
                    if (IS_FAIL_fn(oargs[j])) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                }
            } else {
                for (int j = 0; j < nargs; j++) {
                    IR_t *ax = ir_call_arg(bb, j);
                    if (!ax) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_EXEC(ax).state = 0; IR_interp_node(ax);
                    oargs[j] = IR_EXEC(ax).value;
                    if (IS_FAIL_fn(oargs[j])) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                }
                IR_EXEC(bb).state = 2;
            }
            DESCR_t oout = FAILDESCR;
            if (try_call_builtin_by_name(IR_LIT(bb).sval, oargs, nargs, &oout)) {
                IR_EXEC(bb).value = oout;
                return IS_FAIL_fn(oout) ? bb->ω.node : bb->γ.node;
            }
            IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        DESCR_t *args = NULL;
        if (nargs > 0) {
            args = (DESCR_t *)GC_malloc((size_t)nargs * sizeof(DESCR_t));
            if (is_deep) {
                for (int j = 0; j < nargs; j++) {
                    args[j] = ag_ring_peek(g_current_cfg, nargs - 1 - j);
                    if (IS_FAIL_fn(args[j])) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                }
            } else {
                for (int j = 0; j < nargs; j++) {
                    IR_t *ax = ir_call_arg(bb, j);
                    if (!ax) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_interp_node(ax);
                    args[j] = IR_EXEC(ax).value;
                    if (IS_FAIL_fn(args[j])) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                }
            }
        }
        for (int _pi0 = 0; _pi0 < g_stage2.proc_count; _pi0++) {
            if (!g_stage2.proc_table[_pi0].name || strcmp(g_stage2.proc_table[_pi0].name, IR_LIT(bb).sval) != 0) continue;
            if (!g_stage2.proc_table[_pi0].is_generator) break;
            if (bb_graph_of_proc(&g_stage2.proc_table[_pi0])) break;
            if (g_stage2.proc_table[_pi0].entry_pc < 0) break;
            GeneratorState *pgs = generator_state_new_proc(_pi0, args, nargs);
            if (!pgs) break;
            DESCR_t v;
            int ok = bb_broker_drive_sm_one(pgs, &v);
            if (!ok) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = (int64_t)(intptr_t)pgs;
            IR_EXEC(bb).state  = 1;
            IR_EXEC(bb).value  = v;
            return bb->γ.node;
        }
        DESCR_t out = FAILDESCR;
        if (try_call_builtin_by_name(IR_LIT(bb).sval, args, nargs, &out)) {
            IR_EXEC(bb).value = out;
            return IS_FAIL_fn(out) ? bb->ω.node : bb->γ.node;
        }
        for (int _pi = 0; _pi < g_stage2.proc_count; _pi++) {
            if (!g_stage2.proc_table[_pi].name || strcmp(g_stage2.proc_table[_pi].name, IR_LIT(bb).sval) != 0) continue;
            IR_graph_t *_cfg = bb_graph_of_proc(&g_stage2.proc_table[_pi]);
            if (!_cfg) break;
            if (frame_depth >= FRAME_STACK_MAX) break;
            GenFrame *_f = &frame_stack[frame_depth++];
            memset(_f, 0, sizeof *_f);
            _f->sc   = g_stage2.proc_table[_pi].lower_sc;
            int _nsl = _f->sc.n > 0 ? _f->sc.n : 1;
            if (_nsl > FRAME_SLOT_MAX) _nsl = FRAME_SLOT_MAX;
            _f->env_n = _nsl;
            for (int _k = 0; _k < g_stage2.proc_table[_pi].nparams && _k < nargs && _k < FRAME_SLOT_MAX; _k++)
                _f->env[_k] = args[_k];
            bb_node_state_t * _snap = bb_snapshot_state(_cfg);
            bb_reset(_cfg);
            out = IR_interp_once(_cfg);
            if (frame_depth > 0 && FRAME.returning) { out = g_ir_return_val; FRAME.returning = 0; }
            frame_depth--;
            bb_restore_state(_cfg, _snap);
            IR_EXEC(bb).value = out;
            return IS_FAIL_fn(out) ? bb->ω.node : bb->γ.node;
        }
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_SEQ: {
        if (IR_LIT(bb).dval == 1.0) {
            IR_graph_t * lblk = (IR_graph_t *)(intptr_t) IR_EXEC(bb).counter;
            IR_graph_t * rblk = (IR_graph_t *)(intptr_t) IR_LIT(bb).ival;
            if (!lblk || !rblk) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            bb_reset(lblk); DESCR_t lv = IR_interp_once(lblk);
            if (IS_FAIL_fn(lv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            bb_reset(rblk); DESCR_t rv = IR_interp_once(rblk);
            if (IS_FAIL_fn(rv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            int rel_fail = 0;
            DESCR_t result = binop_apply(BINOP_CONCAT, lv, rv, &rel_fail);
            if (IS_FAIL_fn(result)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = result;
            return bb->γ.node;
        }
        IR_EXEC(bb).value = NULVCL;
        return NULL;
    }
    case IR_SEQ_EXPR: {
        IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_BINOP: {
        if (!((IR_t*)0) && !((IR_t*)0)) {
            int n_aux = 0;
            IR_t * const * aux = bb_operand_aux_get(g_current_cfg, bb, &n_aux);
            DESCR_t lv, rv;
            if (aux && n_aux == 2 && aux[0] && aux[1]) {
                if (g_current_cfg && g_current_cfg->lang == IR_LANG_ICN) {
                    if (aux[0]->op == IR_VAR || aux[0]->op == IR_KEYWORD) IR_interp_node(aux[0]);
                    if (aux[1]->op == IR_VAR || aux[1]->op == IR_KEYWORD) IR_interp_node(aux[1]);
                }
                lv = IR_EXEC(aux[0]).value;
                rv = IR_EXEC(aux[1]).value;
            } else {
                rv = ag_ring_peek(g_current_cfg, 0);
                lv = ag_ring_peek(g_current_cfg, 1);
            }
            if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            int rel_fail = 0;
            DESCR_t result = binop_apply((BinopKind)IR_LIT(bb).ival, lv, rv, &rel_fail);
            if (IS_FAIL_fn(result)) {
                if (rel_fail && aux && n_aux == 2) {
                    IR_t * rt_tgt = gen_resume_target(aux[1]);
                    if (rt_tgt) { IR_EXEC(bb).value = FAILDESCR; return rt_tgt; }
                    rt_tgt = gen_resume_target(aux[0]);
                    if (rt_tgt) { IR_EXEC(bb).value = FAILDESCR; return rt_tgt; }
                }
                IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
            IR_EXEC(bb).value = result;
            return bb->γ.node;
        }
        if (!((IR_t*)0) || !((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t lv = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(lv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t rv = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(rv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int rel_fail = 0;
        DESCR_t result = binop_apply((BinopKind)IR_LIT(bb).ival, lv, rv, &rel_fail);
        if (IS_FAIL_fn(result)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = result;
        return bb->γ.node;
    }
    case IR_BINOP_GEN: {
        IR_t * Lc = ir_pair_arg(bb, 0);
        IR_t * Rc = ir_pair_arg(bb, 1);
        if (!Lc || !Rc) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int l_gen = bb_is_gen_node(Lc);
        int r_gen = bb_is_gen_node(Rc);
        if (IR_EXEC(bb).state == 0) {
            IR_EXEC(Lc).state = 0;
            IR_EXEC(Rc).state = 0;
            IR_interp_node(Lc);
            if (IS_FAIL_fn(IR_EXEC(Lc).value)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_interp_node(Rc);
            if (IS_FAIL_fn(IR_EXEC(Rc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).state = 1;
        } else {
            if (r_gen) {
                IR_interp_node(Rc);
                if (IS_FAIL_fn(IR_EXEC(Rc).value)) {
                    if (!l_gen) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_interp_node(Lc);
                    if (IS_FAIL_fn(IR_EXEC(Lc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_EXEC(Rc).state = 0;
                    IR_interp_node(Rc);
                    if (IS_FAIL_fn(IR_EXEC(Rc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                }
                if (!l_gen && (Lc->op == IR_VAR || Lc->op == IR_KEYWORD)) {
                    IR_interp_node(Lc);
                    if (IS_FAIL_fn(IR_EXEC(Lc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                }
            } else if (l_gen) {
                IR_interp_node(Lc);
                if (IS_FAIL_fn(IR_EXEC(Lc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                if (Rc->op == IR_VAR || Rc->op == IR_KEYWORD) {
                    IR_interp_node(Rc);
                    if (IS_FAIL_fn(IR_EXEC(Rc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                }
            } else {
                IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
        }
        for (;;) {
            int rel_fail = 0;
            DESCR_t result = binop_apply((BinopKind)IR_LIT(bb).ival, IR_EXEC(Lc).value, IR_EXEC(Rc).value, &rel_fail);
            if (!IS_FAIL_fn(result)) { IR_EXEC(bb).value = result; return bb->γ.node; }
            if (!rel_fail) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            if (r_gen) {
                IR_interp_node(Rc);
                if (IS_FAIL_fn(IR_EXEC(Rc).value)) {
                    if (!l_gen) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_interp_node(Lc);
                    if (IS_FAIL_fn(IR_EXEC(Lc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_EXEC(Rc).state = 0;
                    IR_interp_node(Rc);
                    if (IS_FAIL_fn(IR_EXEC(Rc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                }
            } else if (l_gen) {
                IR_interp_node(Lc);
                if (IS_FAIL_fn(IR_EXEC(Lc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            } else {
                IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
        }
    }
    case IR_LIT_F:
        IR_EXEC(bb).value = REALVAL(IR_LIT(bb).dval);
        return bb->γ.node;
    case IR_LIT_S:
        IR_EXEC(bb).value = STRVAL(IR_LIT(bb).sval ? IR_LIT(bb).sval : "");
        return bb->γ.node;
    case IR_LIT_NUL:
    case IR_SUCCEED:
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    case IR_INITIAL: {
        char _init_key[32];
        snprintf(_init_key, sizeof(_init_key), "__init_%p", (void*)bb);
        DESCR_t _init_flag = NV_GET_fn(_init_key);
        if (!IS_FAIL_fn(_init_flag) && IS_INT_fn(_init_flag) && _init_flag.i != 0) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        NV_SET_fn(_init_key, INTVAL(1));
        if (bb->n_operands > 0 && bb->operands[0]) {
            int ini_safe = (g_current_cfg ? g_current_cfg->n : 64) * 64 + 256;
            IR_t *ic = bb->operands[0];
            while (ic && ini_safe-- > 0) {
                IR_t *in = IR_interp_node(ic);
                if (IS_FAIL_fn(IR_EXEC(ic).value)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                if (!in || in == bb->γ.node || in == bb->ω.node || in == ic) break;
                ag_ring_push(g_current_cfg, IR_EXEC(ic).value);
                ic = in;
            }
        }
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_LCONCAT: {
        int n_lc = 0;
        IR_t * const * lc_ax = bb_operand_aux_get(g_current_cfg, bb, &n_lc);
        DESCR_t a, b;
        if (lc_ax && n_lc == 2 && lc_ax[0] && lc_ax[1]) {
            IR_interp_node(lc_ax[0]); a = IR_EXEC(lc_ax[0]).value;
            IR_interp_node(lc_ax[1]); b = IR_EXEC(lc_ax[1]).value;
        } else {
            b = ag_ring_peek(g_current_cfg, 0);
            a = ag_ring_peek(g_current_cfg, 1);
        }
        if (IS_FAIL_fn(a) || IS_FAIL_fn(b)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DESCR_t r = lconcat_d(a, b);
        if (IS_FAIL_fn(r)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = r;
        return bb->γ.node;
    }
    case IR_RETURN: {
        DESCR_t rv;
        if (IR_LIT(bb).dval == 2.0) {
            rv = FAILDESCR;
            g_ir_return_val = FAILDESCR;
        } else if (IR_LIT(bb).dval == 1.0) {
            rv = g_sno_cur_func ? NV_GET_fn(g_sno_cur_func) : NULVCL;
            g_ir_return_val = IS_FAIL_fn(rv) ? NULVCL : rv;
        } else {
            rv = NULVCL;
            if (bb->n_operands > 0 && bb->operands[0]) { IR_interp_node(bb->operands[0]); rv = IR_EXEC(bb->operands[0]).value; }
            else { DESCR_t pv = ag_ring_peek(g_current_cfg, 0); if (!IS_FAIL_fn(pv)) rv = pv; }
            g_ir_return_val = IS_FAIL_fn(rv) ? NULVCL : rv;
        }
        if (frame_depth > 0) FRAME.returning = 1;
        IR_EXEC(bb).value = g_ir_return_val;
        return bb->ω.node;
    }
    case IR_SUSPEND: {
        if (IR_LIT(bb).dval == 1.0) {
            IR_graph_t * eblk = (IR_graph_t *)(intptr_t) IR_EXEC(bb).counter;
            IR_graph_t * bblk = (IR_graph_t *)(intptr_t) IR_LIT(bb).ival;
            DESCR_t last = NULVCL;
            if (eblk && eblk->entry) {
                int e_is_gen = !ir_is_single_shot(eblk->entry);
                IR_graph_t * save_cfg = g_current_cfg;
                bb_reset(eblk);
                DESCR_t v = IR_interp_once(eblk);
                int safety = eblk->n * 256 + 4096;
                while (!IS_FAIL_fn(v) && safety-- > 0) {
                    suspend_buf_push(v);
                    last = v;
                    if (bblk && bblk->entry) { bb_reset(bblk); (void) IR_interp_once(bblk); }
                    if (!e_is_gen) break;
                    v = IR_interp_resume(eblk);
                }
                g_current_cfg = save_cfg;
            }
            IR_EXEC(bb).value = last;
            return bb->γ.node;
        }
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_FAIL:
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    case IR_GOTO: {
        extern IR_t * bb_label_landing(const char * name);
        IR_graph_t * sub = (IR_graph_t *)(intptr_t) IR_EXEC(bb).counter;
        if (!sub || !sub->entry) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        bb_reset(sub);
        DESCR_t lv = IR_interp_once(sub);
        if (IS_FAIL_fn(lv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        char * lname = VARVAL_fn(lv);
        IR_t * land = lname ? bb_label_landing(lname) : NULL;
        if (!land) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = NULVCL;
        return land;
    }
    case IR_IF: {
        IR_t * cnd = bb->n_operands > 0 ? bb->operands[0] : ((IR_t*)0);
        if (!cnd && !((IR_t*)0)) {
            DESCR_t cv = ag_ring_peek(g_current_cfg, 0);
            IR_EXEC(bb).value = IS_FAIL_fn(cv) ? FAILDESCR : cv;
            return IS_FAIL_fn(cv) ? bb->ω.node : bb->γ.node;
        }
        if (!cnd) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        IR_interp_node(cnd);
        DESCR_t cv = IR_EXEC(cnd).value;
        if (!IS_FAIL_fn(cv)) {
            if (((IR_t*)0)) { IR_EXEC(((IR_t*)0)).state = 0; IR_interp_node(((IR_t*)0)); IR_EXEC(bb).value = IR_EXEC(((IR_t*)0)).value; }
            else IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        if (bb->ω.node) {
            IR_t *else_box = bb->ω.node;
            IR_EXEC(else_box).state = 0;
            IR_interp_node(else_box);
            IR_EXEC(bb).value = IR_EXEC(else_box).value;
            return bb->γ.node;
        }
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_CONJ: {
        if (!((IR_t*)0) && !((IR_t*)0)) {
            DESCR_t rv = ag_ring_peek(g_current_cfg, 0);
            if (IS_FAIL_fn(rv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = rv;
            return bb->γ.node;
        }
        if (!((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) { IR_EXEC(((IR_t*)0)).state = 0; IR_EXEC(bb).state = 1; }
        IR_interp_node(((IR_t*)0));
        DESCR_t cv = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(cv)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (((IR_t*)0)) { IR_EXEC(((IR_t*)0)).state = 0; IR_interp_node(((IR_t*)0)); IR_EXEC(bb).value = IR_EXEC(((IR_t*)0)).value; }
        else IR_EXEC(bb).value = cv;
        return bb->γ.node;
    }
    case IR_EVERY: {
        IR_t * ev0 = bb->n_operands > 0 ? bb->operands[0] : ((IR_t*)0);
        if (!ev0) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        if (IR_LIT(bb).ival == 0) {
            if (frame_depth > 0 && FRAME.loop_break) FRAME.loop_break = 0;
            IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        if (IR_LIT(bb).ival == 1 || IR_LIT(bb).ival == 2) {
            if (IR_EXEC(bb).state == 0) {
                IR_EXEC(bb).state = 1;
                IR_EXEC(bb).value = NULVCL;
                return ev0;
            }
            if (frame_depth > 0) FRAME.loop_break = 0;
            IR_EXEC(bb).state = 0;
            IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        if (IR_LIT(bb).ival == 3) {
            if (IR_EXEC(bb).state == 0) { IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return ev0; }
            if (IR_EXEC(bb).state == 1) {
                DESCR_t gv = IR_EXEC(ev0).value;
                if (IS_FAIL_fn(gv)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
                if (frame_depth > 0 && FRAME.loop_break) { FRAME.loop_break = 0; IR_EXEC(bb).state = 0; IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
                if (!((IR_t*)0)) { IR_EXEC(bb).value = NULVCL; return ev0; }
                IR_EXEC(((IR_t*)0)).state = 0;
                IR_EXEC(bb).state = 2;
                IR_EXEC(bb).value = NULVCL;
                return ((IR_t*)0);
            }
            if (frame_depth > 0 && (FRAME.loop_break || FRAME.returning)) {
                if (FRAME.returning) return bb->γ.node;
                FRAME.loop_break = 0; IR_EXEC(bb).state = 0; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
            }
            if (frame_depth > 0) FRAME.loop_next = 0;
            IR_EXEC(bb).state = 1;
            IR_EXEC(bb).value = NULVCL;
            return ev0;
        }
        int single_shot_call = ir_is_single_shot(((IR_t*)0));
        int saved_brk = frame_depth > 0 ? FRAME.loop_break : 0;
        int saved_nxt = frame_depth > 0 ? FRAME.loop_next  : 0;
        if (frame_depth > 0) { FRAME.loop_break = 0; FRAME.loop_next = 0; }
        int safety = 1000000;
        while (safety-- > 0) {
            IR_interp_node(((IR_t*)0));
            DESCR_t v = IR_EXEC(((IR_t*)0)).value;
            if (IS_FAIL_fn(v)) break;
            if (frame_depth > 0 && FRAME.loop_break) break;
            if (((IR_t*)0)) { IR_EXEC(((IR_t*)0)).state = 0; IR_interp_node(((IR_t*)0)); }
            if (frame_depth > 0 && (FRAME.loop_break || FRAME.returning)) break;
            if (frame_depth > 0) FRAME.loop_next = 0;
            if (single_shot_call) break;
        }
        if (frame_depth > 0) { FRAME.loop_break = saved_brk; FRAME.loop_next = saved_nxt; }
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_WHILE: {
        IR_t * cnd = bb->n_operands > 0 ? bb->operands[0] : ((IR_t*)0);
        if (!cnd) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        int saved_brk_w = frame_depth > 0 ? FRAME.loop_break : 0;
        int saved_nxt_w = frame_depth > 0 ? FRAME.loop_next  : 0;
        if (frame_depth > 0) { FRAME.loop_break = 0; FRAME.loop_next = 0; }
        int safety = (g_current_cfg ? g_current_cfg->n : 64) * 65536 + 1048576;
        int exited = 0;
        while (!exited && safety-- > 0) {
            IR_t * cur = cnd;
            int inner = (g_current_cfg ? g_current_cfg->n : 64) * 1024 + 4096;
            while (cur && cur != bb && inner-- > 0) {
                IR_t * nxt = IR_interp_node(cur);
                if (IS_FAIL_fn(IR_EXEC(cur).value)) { exited = 1; break; }
                if (frame_depth > 0 && (FRAME.loop_break || FRAME.returning)) { exited = 1; break; }
                if (!nxt || nxt == bb) { exited = 1; break; }
                ag_ring_push(g_current_cfg, IR_EXEC(cur).value);
                cur = nxt;
            }
            if (inner <= 0) exited = 1;
            if (frame_depth > 0) FRAME.loop_next = 0;
        }
        if (frame_depth > 0) { FRAME.loop_break = saved_brk_w; FRAME.loop_next = saved_nxt_w; }
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_UNTIL: {
        IR_t * ucnd = bb->n_operands > 0 ? bb->operands[0] : ((IR_t*)0);
        if (!ucnd) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        int saved_brk_u = frame_depth > 0 ? FRAME.loop_break : 0;
        int saved_nxt_u = frame_depth > 0 ? FRAME.loop_next  : 0;
        if (frame_depth > 0) { FRAME.loop_break = 0; FRAME.loop_next = 0; }
        int safety_u = 1000000;
        while (safety_u-- > 0) {
            IR_EXEC(ucnd).state = 0;
            IR_interp_node(ucnd);
            DESCR_t cv = IR_EXEC(ucnd).value;
            if (!IS_FAIL_fn(cv)) break;
            if (frame_depth > 0 && FRAME.loop_break) break;
            if (((IR_t*)0)) { IR_EXEC(((IR_t*)0)).state = 0; IR_interp_node(((IR_t*)0)); }
            if (frame_depth > 0 && (FRAME.loop_break || FRAME.returning)) break;
            if (frame_depth > 0) FRAME.loop_next = 0;
        }
        if (frame_depth > 0) { FRAME.loop_break = saved_brk_u; FRAME.loop_next = saved_nxt_u; }
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_REPEAT: {
        IR_t * r0 = bb->n_operands > 0 ? bb->operands[0] : ((IR_t*)0);
        if (!r0) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        IR_EXEC(bb).value = NULVCL;
        return r0;
    }
    case IR_LIMIT: {
        IR_t * expr_r = bb->n_operands > 0 ? bb->operands[0] : NULL;
        IR_t * lim_nd = bb->n_operands > 1 ? bb->operands[1] : NULL;
        if (!lim_nd || !expr_r) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_LIT(bb).ival == 1) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) {
            IR_interp_node(lim_nd);
            DESCR_t mv = IR_EXEC(lim_nd).value;
            if (IS_FAIL_fn(mv)) { IR_LIT(bb).ival = 1; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            int64_t mx = IS_INT_fn(mv) ? mv.i : (mv.v == DT_R ? (int64_t)mv.r : 0);
            if (mx <= 0) { IR_LIT(bb).ival = 1; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = mx;
            IR_EXEC(bb).state = 1;
        }
        DESCR_t val = IR_EXEC(expr_r).value;
        if (IS_FAIL_fn(val)) { IR_LIT(bb).ival = 1; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).counter--;
        IR_EXEC(bb).value = val;
        if (IR_EXEC(bb).counter <= 0) { IR_LIT(bb).ival = 1; IR_EXEC(bb).state = 0; }
        return bb->γ.node;
    }
    case IR_ALT: {
        if (!((IR_t*)0)) {
            int n_arm = 0;
            IR_t * const * arms = bb_operand_aux_get(g_current_cfg, bb, &n_arm);
            if (!arms || n_arm <= 0) {
                DESCR_t v = ag_ring_peek(g_current_cfg, 0);
                if (IS_FAIL_fn(v)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value = v;
                return bb->γ.node;
            }
            if (IR_EXEC(bb).state == 0) {
                int safety0 = (g_current_cfg ? g_current_cfg->n : 64) * 64 + 256;
                IR_t * cur0 = (n_arm > 0 && arms[0]) ? arms[0] : NULL;
                while (cur0 && safety0-- > 0) {
                    if (cur0 == bb->ω.node) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_t * nxt0 = IR_interp_node(cur0);
                    if (nxt0 == bb) {
                        IR_EXEC(bb).value = IR_EXEC(cur0).value;
                        for (int j = 0; j < n_arm; j++) if (arms[j] == cur0) { IR_EXEC(bb).counter = j; break; }
                        IR_EXEC(bb).state = 1;
                        return bb->γ.node;
                    }
                    if (!nxt0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    ag_ring_push(g_current_cfg, IR_EXEC(cur0).value);
                    cur0 = nxt0;
                }
                IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
            int ci = (int)IR_EXEC(bb).counter;
            IR_t * cur;
            if (ci >= 0 && ci < n_arm && arms[ci] && bb_is_gen_node(arms[ci])) {
                IR_interp_node(arms[ci]);
                if (!IS_FAIL_fn(IR_EXEC(arms[ci]).value)) { IR_EXEC(bb).value = IR_EXEC(arms[ci]).value; return bb->γ.node; }
                cur = arms[ci]->ω.node;
            } else {
                cur = (ci >= 0 && ci < n_arm && arms[ci]) ? arms[ci]->ω.node : NULL;
            }
            int safety2 = (g_current_cfg ? g_current_cfg->n : 64) * 64 + 256;
            while (cur && safety2-- > 0) {
                if (cur == bb->ω.node) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                IR_t * nxt = IR_interp_node(cur);
                if (nxt == bb) {
                    IR_EXEC(bb).value = IR_EXEC(cur).value;
                    for (int j = 0; j < n_arm; j++) if (arms[j] == cur) { IR_EXEC(bb).counter = j; break; }
                    return bb->γ.node;
                }
                if (!nxt) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                ag_ring_push(g_current_cfg, IR_EXEC(cur).value);
                cur = nxt;
            }
            IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        #define ALT_IS_GEN(k) ( \
            (k) == IR_TO || (k) == IR_TO_BY || (k) == IR_UPTO || \
            (k) == IR_ALT    || (k) == IR_BINOP_GEN || \
            (k) == IR_ITERATE || (k) == IR_LIMIT || (k) == IR_PROC_GEN || \
            (k) == IR_LIST_BANG || (k) == IR_KEY_GEN || (k) == IR_FIND_GEN || (k) == IR_SEQ_GEN || (k) == IR_TO_BY  || (k) == IR_GEN_ALT || (k) == IR_GATHER || (k) == IR_MAP || (k) == IR_GREP)
        if (!((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) {
            int i = 0;
            for (IR_t * arm = ((IR_t*)0); arm; arm = arm->ω.node, i++) {
                IR_EXEC(arm).state = 0;
                IR_interp_node(arm);
                if (!IS_FAIL_fn(IR_EXEC(arm).value)) {
                    IR_EXEC(bb).value   = IR_EXEC(arm).value;
                    IR_EXEC(bb).counter = i;
                    IR_EXEC(bb).state   = 1;
                    return bb->γ.node;
                }
            }
            IR_EXEC(bb).value = FAILDESCR;
            return bb->ω.node;
        }
        int ci = (int)IR_EXEC(bb).counter;
        IR_t * cur = ((IR_t*)0);
        for (int j = 0; j < ci && cur; j++) cur = cur->ω.node;
        if (cur && ALT_IS_GEN(cur->op)) {
            IR_interp_node(cur);
            if (!IS_FAIL_fn(IR_EXEC(cur).value)) { IR_EXEC(bb).value = IR_EXEC(cur).value; return bb->γ.node; }
        }
        int i = ci + 1;
        for (IR_t * arm = (cur ? cur->ω.node : NULL); arm; arm = arm->ω.node, i++) {
            IR_EXEC(arm).state = 0;
            IR_interp_node(arm);
            if (!IS_FAIL_fn(IR_EXEC(arm).value)) {
                IR_EXEC(bb).value   = IR_EXEC(arm).value;
                IR_EXEC(bb).counter = i;
                return bb->γ.node;
            }
        }
        IR_EXEC(bb).state = 0;
        IR_EXEC(bb).value = FAILDESCR;
        #undef ALT_IS_GEN
        return bb->ω.node;
    }
    case IR_TO_BY: {
        IR_t * Lc = ir_pair_arg(bb, 0);
        IR_t * Hc = ir_pair_arg(bb, 1);
        if (!Lc && !Hc && IR_LIT(bb).sval && IR_LIT(bb).sval[0] == 'a') {
            int is_real_ag = (IR_LIT(bb).sval[1] == 'r');
            if (IR_EXEC(bb).state == 0) {
                DESCR_t lv = ag_ring_peek(g_current_cfg, 1);
                DESCR_t hv = ag_ring_peek(g_current_cfg, 0);
                if (IS_FAIL_fn(lv) || IS_FAIL_fn(hv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                if (is_real_ag) {
                    double lo_r = (lv.v == DT_R) ? lv.r : (double)(IS_INT_fn(lv) ? lv.i : 0);
                    double hi_r = (hv.v == DT_R) ? hv.r : (double)(IS_INT_fn(hv) ? hv.i : 0);
                    IR_LIT(bb).dval = lo_r;
                    memcpy(&IR_EXEC(bb).counter, &hi_r, 8);
                } else {
                    int64_t lo_i = IS_INT_fn(lv) ? lv.i : (lv.v == DT_R ? (int64_t)lv.r : 0);
                    int64_t hi_i = IS_INT_fn(hv) ? hv.i : (hv.v == DT_R ? (int64_t)hv.r : 0);
                    IR_EXEC(bb).counter = lo_i;
                    memcpy(&IR_LIT(bb).dval, &hi_i, 8);
                }
                IR_EXEC(bb).state = 1;
            }
            if (is_real_ag) {
                double by_r; memcpy(&by_r, &IR_LIT(bb).ival, sizeof(double));
                if (by_r == 0.0) by_r = 1.0;
                double hi_r; memcpy(&hi_r, &IR_EXEC(bb).counter, 8);
                if (by_r >= 0.0 ? IR_LIT(bb).dval > hi_r + 1e-12 : IR_LIT(bb).dval < hi_r - 1e-12) {
                    IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
                }
                DESCR_t rv; rv.v = DT_R; rv.r = IR_LIT(bb).dval; IR_EXEC(bb).value = rv;
                IR_LIT(bb).dval += by_r;
                return bb->γ.node;
            }
            int64_t by_i = IR_LIT(bb).ival ? IR_LIT(bb).ival : 1;
            int64_t hi_i; memcpy(&hi_i, &IR_LIT(bb).dval, 8);
            if (by_i >= 0 ? IR_EXEC(bb).counter > hi_i : IR_EXEC(bb).counter < hi_i) {
                IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
            IR_EXEC(bb).value    = INTVAL(IR_EXEC(bb).counter);
            IR_EXEC(bb).counter += by_i;
            return bb->γ.node;
        }
        int is_real = (IR_LIT(bb).sval && (IR_LIT(bb).sval[0] == 'r' || (IR_LIT(bb).sval[0] == 'a' && IR_LIT(bb).sval[1] == 'r')));
        if (IR_EXEC(bb).state == 0) {
            if (Lc) IR_interp_node(Lc);
            if (Hc) IR_interp_node(Hc);
            if (is_real) {
                double lo_r = Lc ? ((IR_EXEC(Lc).value.v == DT_R) ? IR_EXEC(Lc).value.r : (double)(IS_INT_fn(IR_EXEC(Lc).value) ? IR_EXEC(Lc).value.i : 0)) : 0.0;
                IR_LIT(bb).dval = lo_r;
            } else {
                IR_EXEC(bb).counter = Lc ? (IS_INT_fn(IR_EXEC(Lc).value) ? IR_EXEC(Lc).value.i : 0) : 0;
            }
            IR_EXEC(bb).state = 1;
        }
        if (is_real) {
            double by_r; memcpy(&by_r, &IR_LIT(bb).ival, sizeof(double));
            if (by_r == 0.0) by_r = 1.0;
            double to_r = Hc ? ((IR_EXEC(Hc).value.v == DT_R) ? IR_EXEC(Hc).value.r : (double)(IS_INT_fn(IR_EXEC(Hc).value) ? IR_EXEC(Hc).value.i : 0)) : 0.0;
            if (by_r >= 0.0 ? IR_LIT(bb).dval > to_r + 1e-12 : IR_LIT(bb).dval < to_r - 1e-12) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            DESCR_t rv; rv.v = DT_R; rv.r = IR_LIT(bb).dval; IR_EXEC(bb).value = rv;
            IR_LIT(bb).dval += by_r;
            return bb->γ.node;
        }
        int64_t by = IR_LIT(bb).ival ? IR_LIT(bb).ival : 1;
        if (bb->n_operands > 2 && bb->operands[2]) { DESCR_t bv = IR_EXEC(bb->operands[2]).value; if (IS_INT_fn(bv) && bv.i != 0) by = bv.i; else if (bv.v == DT_R && (int64_t)bv.r != 0) by = (int64_t)bv.r; }
        int64_t to_val = Hc ? (IS_INT_fn(IR_EXEC(Hc).value) ? IR_EXEC(Hc).value.i : 0) : 0;
        if (by >= 0 ? IR_EXEC(bb).counter > to_val : IR_EXEC(bb).counter < to_val) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value    = INTVAL(IR_EXEC(bb).counter);
        IR_EXEC(bb).counter += by;
        return bb->γ.node;
    }
    case IR_PATTERN_LIT: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        const char *lit = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        long litlen = (long)strlen(lit);
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_lit_proto, 125, &bb_lit_proto_desc, litlen, lit);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1;
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_PATTERN_LEN: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_len_proto, 78, &bb_len_proto_desc, (long)IR_LIT(bb).ival, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_POS: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_pos_proto, 61, &bb_pos_proto_desc, (long)IR_LIT(bb).ival, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_RPOS: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_rpos_proto, 66, &bb_rpos_proto_desc, (long)IR_LIT(bb).ival, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_TAB: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_tab_proto, 83, &bb_tab_proto_desc, (long)IR_LIT(bb).ival, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_RTAB: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_rtab_proto, 83, &bb_rtab_proto_desc, (long)IR_LIT(bb).ival, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_ANY: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_any_proto, 89, &bb_any_proto_desc, 0, IR_LIT(bb).sval ? IR_LIT(bb).sval : "");
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_NOTANY: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_notany_proto, 89, &bb_notany_proto_desc, 0, IR_LIT(bb).sval ? IR_LIT(bb).sval : "");
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_SPAN: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_span_proto, 101, &bb_span_proto_desc, 0, IR_LIT(bb).sval ? IR_LIT(bb).sval : "");
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_BREAK: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_break_proto, 96, &bb_break_proto_desc, 0, IR_LIT(bb).sval ? IR_LIT(bb).sval : "");
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_BREAKX: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_breakx_proto, 165, &bb_breakx_proto_desc, 0, IR_LIT(bb).sval ? IR_LIT(bb).sval : "");
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_ALT: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        IR_t *la = bb->n_operands > 0 ? bb->operands[0] : NULL;
        IR_t *rb = bb->n_operands > 1 ? bb->operands[1] : NULL;
        if (!la || !rb) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DTP_FRAG_t *fl = (DTP_FRAG_t *)(intptr_t)IR_EXEC(la).counter;
        DTP_FRAG_t *fr = (DTP_FRAG_t *)(intptr_t)IR_EXEC(rb).counter;
        if (!fl || !fr) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DTP_FRAG_t *out = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!out) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_stitch_alt(out, fl, fr);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)out;
        IR_EXEC(bb).state = 1;
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_PATTERN_FAIL: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_fail_proto, 44, &bb_fail_proto_desc, 0, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_REM: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_rem_proto, 61, &bb_rem_proto_desc, 0, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_SUCCEED: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_succeed_proto, 44, &bb_succeed_proto_desc, 0, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_FENCE: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_fence_proto, 44, &bb_fence_proto_desc, 0, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_ABORT: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_abort_proto, 44, &bb_abort_proto_desc, 0, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_ARB: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        DTP_FRAG_t *frag = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!frag) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_build(frag, bb_arb_proto, 104, &bb_arb_proto_desc, 0, NULL);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)frag;
        IR_EXEC(bb).state = 1; IR_EXEC(bb).value = NULVCL; return bb->γ.node;
    }
    case IR_PATTERN_CAT: {
        if (IR_EXEC(bb).state) return bb->γ.node;
        IR_t *la = bb->n_operands > 0 ? bb->operands[0] : NULL;
        IR_t *rb = bb->n_operands > 1 ? bb->operands[1] : NULL;
        if (!la || !rb) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DTP_FRAG_t *fl = (DTP_FRAG_t *)(intptr_t)IR_EXEC(la).counter;
        DTP_FRAG_t *fr = (DTP_FRAG_t *)(intptr_t)IR_EXEC(rb).counter;
        if (!fl || !fr) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DTP_FRAG_t *out = (DTP_FRAG_t *)GC_MALLOC(sizeof(DTP_FRAG_t));
        if (!out) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rt_pattern_stitch_cat(out, fl, fr);
        IR_EXEC(bb).counter = (int64_t)(intptr_t)out;
        IR_EXEC(bb).state = 1;
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_DTP_ASSIGN: {
        IR_t *op0 = bb->n_operands > 0 ? bb->operands[0] : NULL;
        if (!op0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DTP_FRAG_t *frag = (DTP_FRAG_t *)(intptr_t)IR_EXEC(op0).counter;
        if (!frag || !frag->entry) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        const char *varname = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        rt_dtp_head_build(frag, varname);
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_SCAN: {
        IR_graph_t *pat = (IR_graph_t *)(intptr_t)IR_EXEC(bb).counter;
        if (!pat || !pat->entry) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int is_repl = (IR_LIT(bb).ival != 0);
        DESCR_t subj_d = is_repl ? NV_GET_fn(IR_LIT(bb).sval ? IR_LIT(bb).sval : "") : ag_ring_peek(g_current_cfg, 0);
        DESCR_t sv = VARVAL_d_fn(subj_d);
        const char *subj_str = ""; int subj_len = 0;
        if (sv.v == DT_S || sv.v == DT_SNUL) { subj_str = sv.s ? sv.s : ""; subj_len = sv.slen ? (int)sv.slen : (int)strlen(subj_str); }
        const char *save_Σ = Σ; int save_Σlen = Σlen; int save_Ω = Ω; int save_Δ = Δ; int save_dca = g_dcap_active; int save_dcn = g_dcap_n;
        extern int64_t kw_anchor;
        Σ = subj_str; Σlen = subj_len; Ω = subj_len;
        int max_start = kw_anchor ? 0 : subj_len; int matched = 0; int m_start = -1; int m_end = -1;
        g_dcap_active = 1;
        for (int start = 0; start <= max_start; start++) {
            Δ = start; g_dcap_n = 0;
            DESCR_t r = IR_interp_once(pat);
            if (!IS_FAIL_fn(r)) { matched = 1; m_start = start; m_end = Δ; break; }
        }
        if (matched) bb_dcap_flush(); else g_dcap_n = 0;
        DESCR_t mspan = matched ? descr_match_span(subj_str + m_start, m_end - m_start) : FAILDESCR;
        if (matched && is_repl) {
            DESCR_t repl_d = VARVAL_d_fn(ag_ring_peek(g_current_cfg, 0));
            if (IS_INT_fn(repl_d) || IS_REAL_fn(repl_d)) repl_d = descr_to_str(repl_d);
            const char *repl_str = (repl_d.v == DT_S || repl_d.v == DT_SNUL) ? (repl_d.s ? repl_d.s : "") : "";
            int repl_len = (repl_d.v == DT_S || repl_d.v == DT_SNUL) ? (repl_d.slen ? (int)repl_d.slen : (int)strlen(repl_str)) : 0;
            int new_len = m_start + repl_len + (subj_len - m_end);
            char *new_s = (char *)GC_MALLOC((size_t)new_len + 1);
            memcpy(new_s, subj_str, (size_t)m_start);
            memcpy(new_s + m_start, repl_str, (size_t)repl_len);
            memcpy(new_s + m_start + repl_len, subj_str + m_end, (size_t)(subj_len - m_end));
            new_s[new_len] = '\0';
            DESCR_t nv = { .v = DT_S, .slen = (uint32_t)new_len, .s = new_s };
            NV_SET_fn(IR_LIT(bb).sval, nv);
        }
        Σ = save_Σ; Σlen = save_Σlen; Ω = save_Ω; Δ = save_Δ; g_dcap_active = save_dca; g_dcap_n = save_dcn;
        if (!matched) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = mspan;
        return bb->γ.node;
    }
    case IR_SUBJECT: {
        if (IR_EXEC(bb).state == 0) {
            DESCR_t sv = VARVAL_d_fn(ag_ring_peek(g_current_cfg, 0));
            const char *subj_str = ""; int subj_len = 0;
            if (sv.v == DT_S || sv.v == DT_SNUL) { subj_str = sv.s ? sv.s : ""; subj_len = sv.slen ? (int)sv.slen : (int)strlen(subj_str); }
            else if (IS_INT_fn(sv) || IS_REAL_fn(sv)) { DESCR_t t = descr_to_str(sv); subj_str = t.s ? t.s : ""; subj_len = t.slen ? (int)t.slen : (int)strlen(subj_str); }
            Σ = subj_str; Σlen = subj_len; Ω = subj_len; Δ = 0;
            IR_EXEC(bb).state = 1;
            DESCR_t vd = { .v = DT_S, .slen = (uint32_t)subj_len, .s = (char *)subj_str };
            IR_EXEC(bb).value = vd;
            return bb->γ.node;
        }
        IR_EXEC(bb).state = 0;
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_REF_INVARIANT: {
        if (IR_EXEC(bb).state == 0) {
            const char *lit = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
            IR_t * in0 = bb->n_operands > 0 ? bb->operands[0] : NULL;
            if (in0 && IR_LIT(in0).sval) lit = IR_LIT(in0).sval;
            IR_EXEC(bb).state = 1;
            DESCR_t vd = { .v = DT_S, .slen = (uint32_t)strlen(lit), .s = (char *)lit };
            IR_EXEC(bb).value = vd;
            return bb->γ.node;
        }
        IR_EXEC(bb).state = 0;
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_PAT_LIT: {
        const char *lit = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        int         len = (int)strlen(lit);
        if (IR_EXEC(bb).state == 0) {
            if (Δ + len > Σlen || (len > 0 && memcmp(Σ + Δ, lit, (size_t)len) != 0)) {
                IR_EXEC(bb).value = FAILDESCR;
                return bb->ω.node;
            }
            IR_EXEC(bb).counter = len;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = descr_match_span(Σ + Δ, len);
            Δ += len;
            return bb->γ.node;
        }
        Δ -= (int)IR_EXEC(bb).counter;
        IR_EXEC(bb).state = 0;
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_PAT_ANY: {
        const char *chars = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        if (IR_LIT(bb).dval == 1.0 && IR_LIT(bb).sval) { const char *cv = VARVAL_fn(NV_GET_fn(IR_LIT(bb).sval)); chars = cv ? cv : ""; }
        if (IR_EXEC(bb).state == 0) {
            if (Δ >= Σlen || !strchr(chars, Σ[Δ])) {
                IR_EXEC(bb).value = FAILDESCR;
                return bb->ω.node;
            }
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = descr_match_span(Σ + Δ, 1);
            Δ++;
            return bb->γ.node;
        }
        Δ--;
        IR_EXEC(bb).state = 0;
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_NOT: {
        IR_t * c0 = (bb->n_operands > 0) ? bb->operands[0] : ((IR_t*)0);
        if (!c0) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        IR_EXEC(c0).state = 0;
        IR_interp_node(c0);
        if (IS_FAIL_fn(IR_EXEC(c0).value)) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_BREAK: {
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node ? bb->γ.node : bb->ω.node;
    }
    case IR_NEXT: {
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node ? bb->γ.node : bb->ω.node;
    }
    case IR_NONNULL: {
        IR_t * c0 = (bb->n_operands > 0) ? bb->operands[0] : ((IR_t*)0);
        if (!c0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(c0);
        DESCR_t v = IR_EXEC(c0).value;
        if (IS_FAIL_fn(v)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (v.v == DT_SNUL) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = v;
        return bb->γ.node;
    }
    case IR_IDENTICAL: {
        if (!((IR_t*)0) || !((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t lv = IR_EXEC(((IR_t*)0)).value;
        IR_interp_node(((IR_t*)0));
        DESCR_t rv = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int ident = 0;
        if (lv.v == rv.v) {
            if (lv.v == DT_SNUL) ident = 1;
            else if (lv.v == DT_I) ident = (lv.i == rv.i);
            else if (lv.v == DT_S || lv.v == DT_K) ident = (lv.s == rv.s) || (lv.s && rv.s && strcmp(lv.s, rv.s) == 0);
            else if (lv.v == DT_DATA) ident = (lv.ptr == rv.ptr);
            else if (lv.v == DT_T) ident = (lv.tbl == rv.tbl);
            else ident = (lv.i == rv.i);
        }
        if (!ident) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = rv;
        return bb->γ.node;
    }
    case IR_NULL_TEST: {
        if (!((IR_t*)0)) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t v = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(v)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (v.v == DT_SNUL) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_RANDOM: {
        if (!((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t v = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(v)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        extern uint64_t bb_rnd_seed;
        bb_rnd_seed = bb_rnd_seed * 6364136223846793005UL + 1442695040888963407UL;
        unsigned long rnd = (unsigned long)(bb_rnd_seed >> 33);
        if (IS_INT_fn(v)) {
            int64_t n = v.i;
            if (n <= 0) { IR_EXEC(bb).value = INTVAL(0); return bb->γ.node; }
            IR_EXEC(bb).value = INTVAL((int64_t)(rnd % (unsigned long)n) + 1);
            return bb->γ.node;
        }
        if (v.v == DT_T) {
            if (!v.tbl || v.tbl->size <= 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            int target = (int)(rnd % (unsigned long)v.tbl->size);
            int seen = 0;
            for (int b = 0; b < TABLE_BUCKETS; b++) {
                for (TBPAIR_t *p = v.tbl->buckets[b]; p; p = p->next) {
                    if (seen == target) { IR_EXEC(bb).value = p->val; return bb->γ.node; }
                    seen++;
                }
            }
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        const char *s = VARVAL_fn(v);
        if (s) {
            long slen = v.slen > 0 ? v.slen : (long)strlen(s);
            if (slen <= 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            int idx = (int)(rnd % (unsigned long)slen);
            char buf[2] = { s[idx], '\0' };
            IR_EXEC(bb).value = STRVAL(GC_strdup(buf));
            return bb->γ.node;
        }
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_UNOP: {
        DESCR_t v;
        IR_t * c0 = (bb->n_operands > 0) ? bb->operands[0] : ((IR_t*)0);
        if (c0) { IR_interp_node(c0); v = IR_EXEC(c0).value; }
        else    { v = ag_ring_peek(g_current_cfg, 0); }
        if (IS_FAIL_fn(v)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int rel_fail = 0;
        switch ((tree_e) IR_LIT(bb).ival) {
        case TT_MNS: { DESCR_t r = binop_apply(BINOP_SUB, INTVAL(0), v, &rel_fail); if (IS_FAIL_fn(r)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; } IR_EXEC(bb).value = r; return bb->γ.node; }
        case TT_PLS: { DESCR_t r = binop_apply(BINOP_ADD, INTVAL(0), v, &rel_fail); if (IS_FAIL_fn(r)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; } IR_EXEC(bb).value = r; return bb->γ.node; }
        case TT_SIZE: { int failed = 0; long len = size_value(v, &failed); if (failed) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; } IR_EXEC(bb).value = INTVAL(len); return bb->γ.node; }
        case TT_NONNULL: { if (v.v == DT_SNUL) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; } IR_EXEC(bb).value = v; return bb->γ.node; }
        case TT_NULL: { if (v.v != DT_SNUL) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; } IR_EXEC(bb).value = v; return bb->γ.node; }
        case TT_CSET_COMPL: { if (IS_INT_fn(v) || IS_REAL_fn(v)) v = descr_to_str(v); const char *cs = IS_NULL_fn(v) ? "" : VARVAL_fn(v); IR_EXEC(bb).value = CSETVAL(cset_complement(cs ? cs : "")); return bb->γ.node; }
        default: IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
    }
    case IR_NEG: {
        IR_t * c0 = (bb->n_operands > 0) ? bb->operands[0] : ((IR_t*)0);
        if (!c0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(c0);
        DESCR_t v = IR_EXEC(c0).value;
        if (IS_FAIL_fn(v)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int rel_fail = 0;
        DESCR_t result = binop_apply(BINOP_SUB, INTVAL(0), v, &rel_fail);
        if (IS_FAIL_fn(result)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = result;
        return bb->γ.node;
    }
    case IR_POS: {
        IR_t * c0 = (bb->n_operands > 0) ? bb->operands[0] : ((IR_t*)0);
        if (!c0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(c0);
        DESCR_t v = IR_EXEC(c0).value;
        if (IS_FAIL_fn(v)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int rel_fail = 0;
        DESCR_t result = binop_apply(BINOP_ADD, INTVAL(0), v, &rel_fail);
        if (IS_FAIL_fn(result)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = result;
        return bb->γ.node;
    }
    case IR_CSET_COMPL: {
        if (!((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t v = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(v)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IS_INT_fn(v) || IS_REAL_fn(v)) v = descr_to_str(v);
        const char *cs = IS_NULL_fn(v) ? "" : VARVAL_fn(v);
        IR_EXEC(bb).value = CSETVAL(cset_complement(cs ? cs : ""));
        return bb->γ.node;
    }
    case IR_CSET_UNION:
    case IR_CSET_DIFF:
    case IR_CSET_INTER: {
        if (!((IR_t*)0) || !((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t lv = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(lv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t rv = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(rv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IS_INT_fn(lv) || IS_REAL_fn(lv)) lv = descr_to_str(lv);
        if (IS_INT_fn(rv) || IS_REAL_fn(rv)) rv = descr_to_str(rv);
        const char *a = IS_NULL_fn(lv) ? "" : VARVAL_fn(lv); if (!a) a = "";
        const char *b = IS_NULL_fn(rv) ? "" : VARVAL_fn(rv); if (!b) b = "";
        const char *raw = (bb->op == IR_CSET_UNION) ? cset_union(a, b)
                        : (bb->op == IR_CSET_DIFF)  ? cset_diff (a, b)
                                                   : cset_inter(a, b);
        IR_EXEC(bb).value = CSETVAL(cset_canonical(raw));
        return bb->γ.node;
    }
    case IR_GEN_SCAN: {
        if (IR_LIT(bb).dval == 1.0) {
            IR_graph_t * subj_sg = (IR_graph_t *)(intptr_t) IR_EXEC(bb).counter;
            IR_graph_t * body_sg = (IR_graph_t *)(intptr_t) IR_LIT(bb).ival;
            if (!subj_sg || !subj_sg->entry) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_graph_t * save_cfg = g_current_cfg;
            bb_reset(subj_sg);
            DESCR_t sv = IR_interp_once(subj_sg);
            g_current_cfg = save_cfg;
            if (IS_FAIL_fn(sv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str(sv);
            const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
            if (!s) s = "";
            if (scan_depth < SCAN_STACK_MAX) {
                scan_stack[scan_depth].subj = scan_subj;
                scan_stack[scan_depth].pos  = scan_pos;
                scan_depth++;
            }
            scan_subj = s;
            scan_pos  = 1;
            DESCR_t body_val = NULVCL;
            int body_ok = 1;
            if (body_sg && body_sg->entry) {
                bb_reset(body_sg);
                body_val = IR_interp_once(body_sg);
                g_current_cfg = save_cfg;
                if (IS_FAIL_fn(body_val)) body_ok = 0;
            }
            if (scan_depth > 0) {
                scan_depth--;
                scan_subj = scan_stack[scan_depth].subj;
                scan_pos  = scan_stack[scan_depth].pos;
            }
            if (!body_ok) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = body_val;
            return bb->γ.node;
        }
        if (!((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t sv = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(sv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        const char *s = VARVAL_fn(sv);
        if (!s) s = "";
        if (scan_depth < SCAN_STACK_MAX) {
            scan_stack[scan_depth].subj = scan_subj;
            scan_stack[scan_depth].pos  = scan_pos;
            scan_depth++;
        }
        scan_subj = s;
        scan_pos  = 1;
        DESCR_t body_val = NULVCL;
        int body_ok = 1;
        if (((IR_t*)0)) {
            IR_interp_node(((IR_t*)0));
            body_val = IR_EXEC(((IR_t*)0)).value;
            if (IS_FAIL_fn(body_val)) body_ok = 0;
        }
        if (scan_depth > 0) {
            scan_depth--;
            scan_subj = scan_stack[scan_depth].subj;
            scan_pos  = scan_stack[scan_depth].pos;
        }
        if (!body_ok) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = body_val;
        return bb->γ.node;
    }
    case IR_KEYWORD: {
        if (!IR_LIT(bb).sval) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        const char *kw = IR_LIT(bb).sval[0] == '&' ? IR_LIT(bb).sval + 1 : IR_LIT(bb).sval;
        if (!strcmp(kw, "subject")) {
            IR_EXEC(bb).value = scan_subj ? STRVAL(scan_subj) : NULVCL;
            return bb->γ.node;
        }
        if (!strcmp(kw, "pos")) {
            IR_EXEC(bb).value = INTVAL((int64_t)scan_pos);
            return bb->γ.node;
        }
        if (!strcmp(kw, "null")) {
            IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        if (!strcmp(kw, "fail")) {
            IR_EXEC(bb).value = FAILDESCR;
            return bb->ω.node;
        }
        DESCR_t kv = kw_read(kw);
        if (!IS_FAIL_fn(kv)) {
            IR_EXEC(bb).value = kv;
            return bb->γ.node;
        }
        DESCR_t gv = NV_GET_fn(IR_LIT(bb).sval);
        IR_EXEC(bb).value = gv;
        return IS_FAIL_fn(gv) ? bb->ω.node : bb->γ.node;
    }
    case IR_SIZE: {
        IR_t * c0 = (bb->n_operands > 0) ? bb->operands[0] : ((IR_t*)0);
        if (!c0) { IR_EXEC(bb).value = INTVAL(0); return bb->γ.node; }
        IR_interp_node(c0);
        int  failed = 0;
        long len    = size_value(IR_EXEC(c0).value, &failed);
        if (failed) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = INTVAL(len);
        return bb->γ.node;
    }
    case IR_IDX: {
        if (!((IR_t*)0) && !((IR_t*)0)) {
            DESCR_t idx  = ag_ring_peek(g_current_cfg, 0);
            DESCR_t base = ag_ring_peek(g_current_cfg, 1);
            if (IS_FAIL_fn(base) || IS_FAIL_fn(idx)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            DESCR_t r = subscript_get(base, idx);
            if (IS_FAIL_fn(r)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = r;
            return bb->γ.node;
        }
        if (!((IR_t*)0) || !((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t base = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(base)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(((IR_t*)0));
        DESCR_t idx = IR_EXEC(((IR_t*)0)).value;
        if (IS_FAIL_fn(idx)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DESCR_t r = subscript_get(base, idx);
        if (IS_FAIL_fn(r)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = r;
        return bb->γ.node;
    }
    case IR_SECTION: {
        if (bb->n_operands == 0) {
            DESCR_t i2   = ag_ring_peek(g_current_cfg, 0);
            DESCR_t i1   = ag_ring_peek(g_current_cfg, 1);
            DESCR_t base = ag_ring_peek(g_current_cfg, 2);
            if (IS_FAIL_fn(base) || IS_FAIL_fn(i1) || IS_FAIL_fn(i2)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            if (IR_LIT(bb).ival == 1 && IS_INT_fn(i1) && IS_INT_fn(i2)) { i2 = INTVAL(i1.i + i2.i); }
            else if (IR_LIT(bb).ival == 2 && IS_INT_fn(i1) && IS_INT_fn(i2)) { int64_t lo = i1.i - i2.i; i2 = i1; i1 = INTVAL(lo); }
            DESCR_t r = subscript_get2(base, i1, i2);
            if (IS_FAIL_fn(r)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = r;
            return bb->γ.node;
        }
        IR_t *base_box = bb->n_operands > 0 ? bb->operands[0] : NULL;
        IR_t *i1_box   = bb->n_operands > 1 ? bb->operands[1] : NULL;
        IR_t *i2_box   = bb->n_operands > 2 ? bb->operands[2] : NULL;
        if (!base_box || !i1_box || !i2_box) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(base_box);
        DESCR_t base = IR_EXEC(base_box).value;
        if (IS_FAIL_fn(base)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(i1_box);
        DESCR_t i1 = IR_EXEC(i1_box).value;
        if (IS_FAIL_fn(i1)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(i2_box);
        DESCR_t i2 = IR_EXEC(i2_box).value;
        if (IS_FAIL_fn(i2)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_LIT(bb).ival == 1 && IS_INT_fn(i1) && IS_INT_fn(i2)) {
            i2 = INTVAL(i1.i + i2.i);
        } else if (IR_LIT(bb).ival == 2 && IS_INT_fn(i1) && IS_INT_fn(i2)) {
            int64_t lo = i1.i - i2.i;
            i2 = i1;
            i1 = INTVAL(lo);
        }
        DESCR_t r = subscript_get2(base, i1, i2);
        if (IS_FAIL_fn(r)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = r;
        return bb->γ.node;
    }
    case IR_LIST_BANG: {
        IR_t * lb0 = bb->n_operands > 0 ? bb->operands[0] : ((IR_t*)0);
        if (!lb0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (lb0->op == IR_MAP || lb0->op == IR_GREP || lb0->op == IR_GATHER || lb0->op == IR_TO) {
            if (IR_EXEC(bb).state == 0) bb_reset(lb0);
            IR_interp_node(lb0);
            DESCR_t gv = IR_EXEC(lb0).value;
            if (IS_FAIL_fn(gv)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).state = 1;
            IR_EXEC(bb).value = gv;
            return bb->γ.node;
        }
        if (IR_EXEC(bb).state == 0) {
            IR_interp_node(lb0);
            DESCR_t obj0 = IR_EXEC(lb0).value;
            if (IS_FAIL_fn(obj0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = 0;
            IR_EXEC(bb).state   = 1;
        } else {
            IR_EXEC(bb).counter++;
        }
        {
            DESCR_t obj = IR_EXEC(lb0).value;
            DESCR_t out;
            if (list_bang_at(obj, IR_EXEC(bb).counter, &out)) { IR_EXEC(bb).value = out; return bb->γ.node; }
            IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
    }
    case IR_RECORD_DEF: {
        if (IR_EXEC(bb).state == 0 && IR_LIT(bb).sval) {
            DEFDAT_fn(IR_LIT(bb).sval);
            dat_register(IR_LIT(bb).sval);
            IR_EXEC(bb).state = 1;
        }
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_FIELD_GET: {
        IR_t *obj_box = bb->n_operands > 0 ? bb->operands[0] : NULL;
        if (!obj_box || !IR_LIT(bb).sval) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(obj_box);
        DESCR_t obj = IR_EXEC(obj_box).value;
        if (IS_FAIL_fn(obj)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DESCR_t *cell = data_field_ptr(IR_LIT(bb).sval, obj);
        if (!cell) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = *cell;
        return bb->γ.node;
    }
    case IR_FIELD_SET: {
        IR_t *obj_box = bb->n_operands > 0 ? bb->operands[0] : NULL;
        IR_t *rhs_box = bb->n_operands > 1 ? bb->operands[1] : NULL;
        if (!obj_box || !rhs_box || !IR_LIT(bb).sval) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(rhs_box);
        DESCR_t rhs = IR_EXEC(rhs_box).value;
        if (IS_FAIL_fn(rhs)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(obj_box);
        DESCR_t obj = IR_EXEC(obj_box).value;
        if (IS_FAIL_fn(obj)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DESCR_t *cell = data_field_ptr(IR_LIT(bb).sval, obj);
        if (!cell) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        *cell = rhs;
        IR_EXEC(bb).value = rhs;
        return bb->γ.node;
    }
    case IR_IDX_SET: {
        if (bb->n_operands == 0) {
            DESCR_t rhs  = ag_ring_peek(g_current_cfg, 0);
            DESCR_t idx  = ag_ring_peek(g_current_cfg, 1);
            DESCR_t base = ag_ring_peek(g_current_cfg, 2);
            if (IS_FAIL_fn(base) || IS_FAIL_fn(idx) || IS_FAIL_fn(rhs)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            if (!subscript_set(base, idx, rhs)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = rhs;
            return bb->γ.node;
        }
        IR_t *base_box = bb->n_operands > 0 ? bb->operands[0] : NULL;
        IR_t *idx_box  = bb->n_operands > 1 ? bb->operands[1] : NULL;
        IR_t *rhs_box  = bb->n_operands > 2 ? bb->operands[2] : NULL;
        if (!base_box || !idx_box || !rhs_box) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(base_box);
        DESCR_t base = IR_EXEC(base_box).value;
        if (IS_FAIL_fn(base)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(idx_box);
        DESCR_t idx = IR_EXEC(idx_box).value;
        if (IS_FAIL_fn(idx)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_interp_node(rhs_box);
        DESCR_t rhs = IR_EXEC(rhs_box).value;
        if (IS_FAIL_fn(rhs)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (!subscript_set(base, idx, rhs)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = rhs;
        return bb->γ.node;
    }
    case IR_KEY_GEN: {
        if (!((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) {
            IR_interp_node(((IR_t*)0));
            DESCR_t tv = IR_EXEC(((IR_t*)0)).value;
            if (IS_FAIL_fn(tv) || tv.v != DT_T || !tv.tbl) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = 0;
            IR_EXEC(bb).state   = 1;
        } else {
            IR_EXEC(bb).counter++;
        }
        DESCR_t tvc = IR_EXEC(((IR_t*)0)).value;
        TBBLK_t *tbl = (tvc.v == DT_T) ? tvc.tbl : NULL;
        if (!tbl) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int64_t target = IR_EXEC(bb).counter, seen = 0;
        for (int b = 0; b < TABLE_BUCKETS; b++) {
            for (TBPAIR_t *ep = tbl->buckets[b]; ep; ep = ep->next) {
                if (seen == target) {
                    IR_EXEC(bb).value = ep->key_descr;
                    return bb->γ.node;
                }
                seen++;
            }
        }
        IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_FIND_GEN: {
        typedef struct { const char *needle; const char *hay; int nlen; int hlen; int stop; int pos; } find_gen_state_t;
        if (!((IR_t*)0) || !((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        find_gen_state_t *st = (find_gen_state_t *)(intptr_t)IR_EXEC(bb).counter;
        if (IR_EXEC(bb).state == 0) {
            IR_interp_node(((IR_t*)0));
            DESCR_t nv = IR_EXEC(((IR_t*)0)).value;
            if (IS_FAIL_fn(nv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_interp_node(((IR_t*)0));
            DESCR_t hv = IR_EXEC(((IR_t*)0)).value;
            if (IS_FAIL_fn(hv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            const char *ns = VARVAL_fn(nv); if (!ns) ns = "";
            const char *hs = VARVAL_fn(hv); if (!hs) hs = "";
            int start1 = 1;
            if (IR_LIT(bb).ival >= 3 && bb->γ.node) {
                IR_interp_node(bb->γ.node);
                DESCR_t sv = IR_EXEC(bb->γ.node).value;
                if (!IS_FAIL_fn(sv) && IS_INT_fn(sv)) start1 = (int)sv.i;
            }
            int hlen = (int)strlen(hs);
            int stop1 = hlen + 1;
            if (IR_LIT(bb).ival >= 4 && bb->ω.node) {
                IR_interp_node(bb->ω.node);
                DESCR_t sv = IR_EXEC(bb->ω.node).value;
                if (!IS_FAIL_fn(sv) && IS_INT_fn(sv)) stop1 = (int)sv.i;
            }
            if (start1 == 0) start1 = hlen + 1;
            if (start1 < 0)  start1 = hlen + 1 + start1;
            if (stop1  == 0) stop1  = hlen + 1;
            if (stop1  < 0)  stop1  = hlen + 1 + stop1;
            if (start1 < 1) start1 = 1;
            if (stop1  > hlen + 1) stop1 = hlen + 1;
            if (!st) { st = (find_gen_state_t *)GC_malloc(sizeof *st); IR_EXEC(bb).counter = (int64_t)(intptr_t)st; }
            st->needle = ns;
            st->hay    = hs;
            st->nlen   = (int)strlen(ns);
            st->hlen   = hlen;
            st->stop   = stop1;
            st->pos    = start1 - 1;
            IR_EXEC(bb).state  = 1;
        }
        if (!st) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (st->nlen == 0) {
            int pos1 = st->pos + 1;
            if (pos1 > st->stop) { IR_EXEC(bb).state = 0; IR_EXEC(bb).counter = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            st->pos     = pos1;
            IR_EXEC(bb).value   = INTVAL(pos1);
            return bb->γ.node;
        }
        int search_from = st->pos;
        if (search_from < 0) search_from = 0;
        if (search_from + st->nlen > st->hlen) { IR_EXEC(bb).state = 0; IR_EXEC(bb).counter = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        const char *hit = strstr(st->hay + search_from, st->needle);
        if (!hit) { IR_EXEC(bb).state = 0; IR_EXEC(bb).counter = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int pos1 = (int)(hit - st->hay) + 1;
        if (pos1 + st->nlen - 1 >= st->stop) { IR_EXEC(bb).state = 0; IR_EXEC(bb).counter = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        st->pos     = pos1;
        IR_EXEC(bb).value   = INTVAL(pos1);
        return bb->γ.node;
    }
    case IR_SEQ_GEN: {
        if (IR_EXEC(bb).state == 0) {
            int64_t start = 1, step = 1;
            if (IR_LIT(bb).ival >= 1 && ((IR_t*)0)) {
                IR_interp_node(((IR_t*)0));
                DESCR_t sv = IR_EXEC(((IR_t*)0)).value;
                if (IS_FAIL_fn(sv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                if (IS_INT_fn(sv)) start = sv.i;
                else if (IS_REAL_fn(sv)) start = (int64_t)sv.r;
            }
            if (IR_LIT(bb).ival >= 2 && ((IR_t*)0)) {
                IR_interp_node(((IR_t*)0));
                DESCR_t sv = IR_EXEC(((IR_t*)0)).value;
                if (IS_FAIL_fn(sv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                if (IS_INT_fn(sv)) step = sv.i;
                else if (IS_REAL_fn(sv)) step = (int64_t)sv.r;
            }
            (void)step;
            IR_EXEC(bb).counter = start;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = INTVAL(start);
            return bb->γ.node;
        }
        {
            int64_t step = 1;
            if (IR_LIT(bb).ival >= 2 && ((IR_t*)0)) {
                DESCR_t sv = IR_EXEC(((IR_t*)0)).value;
                if (IS_INT_fn(sv)) step = sv.i;
                else if (IS_REAL_fn(sv)) step = (int64_t)sv.r;
            }
            if (step == 0) step = 1;
            IR_EXEC(bb).counter += step;
        }
        IR_EXEC(bb).value    = INTVAL(IR_EXEC(bb).counter);
        return bb->γ.node;
    }
    case IR_GATHER: {
        int n = (int) IR_LIT(bb).ival;
        IR_graph_t ** subs = (IR_graph_t **)(intptr_t) IR_EXEC(bb).counter;
        int idx = IR_EXEC(bb).state;
        if (idx >= n || !subs) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_graph_t * sg = subs[idx];
        DESCR_t tv = NULVCL;
        if (sg) { bb_reset(sg); tv = IR_interp_once(sg); }
        if (IS_FAIL_fn(tv)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).state = idx + 1;
        IR_EXEC(bb).value = tv;
        return bb->γ.node;
    }
    case IR_MAP:
    case IR_GREP: {
        IR_graph_t * src_sg  = (IR_graph_t *)(intptr_t) IR_EXEC(bb).counter;
        IR_graph_t * body_sg = (IR_graph_t *)(intptr_t) IR_LIT(bb).ival;
        if (!src_sg || !body_sg) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        rk_seq_cache_t * sc = rk_seq_cache_find(bb);
        if (IR_EXEC(bb).state == 0 || !sc) {
            sc = rk_seq_cache_get(bb);
            sc->count = 0;
            IR_graph_t * save_cfg = g_current_cfg;
            bb_reset(src_sg);
            DESCR_t sv = IR_interp_once(src_sg);
            int safety = src_sg->n * 256 + 4096;
            while (!IS_FAIL_fn(sv) && safety-- > 0) {
                rk_seq_cache_push(sc, sv);
                sv = IR_interp_resume(src_sg);
            }
            g_current_cfg = save_cfg;
            IR_EXEC(bb).state = 1;
        }
        int cur = IR_EXEC(bb).state - 1;
        for (; cur < sc->count; cur++) {
            NV_SET_fn("_", sc->items[cur]);
            IR_graph_t * save_cfg = g_current_cfg;
            bb_reset(body_sg);
            DESCR_t bv = IR_interp_once(body_sg);
            g_current_cfg = save_cfg;
            if (bb->op == IR_GREP) {
                if (IS_FAIL_fn(bv)) continue;
                IR_EXEC(bb).state = cur + 2;
                IR_EXEC(bb).value = sc->items[cur];
                return bb->γ.node;
            } else {
                IR_EXEC(bb).state = cur + 2;
                IR_EXEC(bb).value = IS_FAIL_fn(bv) ? NULVCL : bv;
                return bb->γ.node;
            }
        }
        IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_CASE: {
        if (bb->n_operands < 1 || !bb->operands[0]) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int case_safe = (g_current_cfg ? g_current_cfg->n : 64) * 64 + 256;
        IR_t *scur = bb->operands[0];
        DESCR_t sel = FAILDESCR;
        while (scur && case_safe-- > 0) {
            IR_t *snxt = IR_interp_node(scur);
            if (IS_FAIL_fn(IR_EXEC(scur).value)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            sel = IR_EXEC(scur).value;
            if (snxt == bb || !snxt || snxt == bb->γ.node || snxt == bb->ω.node) break;
            ag_ring_push(g_current_cfg, IR_EXEC(scur).value); scur = snxt;
        }
        if (IS_FAIL_fn(sel)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        for (int ai = 1; ai < bb->n_operands; ai++) {
            IR_t *arm = bb->operands[ai];
            if (!arm) continue;
            IR_t *key_sub = (arm->n_operands > 0) ? arm->operands[0] : NULL;
            IR_t *val_sub = (arm->n_operands > 1) ? arm->operands[1] : NULL;
            if (!key_sub) continue;
            if (!val_sub) {
                int ds = (g_current_cfg ? g_current_cfg->n : 64) * 64 + 256;
                IR_t *dc = key_sub; DESCR_t dv = NULVCL;
                while (dc && ds-- > 0) {
                    IR_t *dn = IR_interp_node(dc);
                    if (IS_FAIL_fn(IR_EXEC(dc).value)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    dv = IR_EXEC(dc).value;
                    if (!dn || dn == bb->γ.node || dn == bb->ω.node || dn == dc) break;
                    ag_ring_push(g_current_cfg, IR_EXEC(dc).value); dc = dn;
                }
                IR_EXEC(bb).value = dv;
                return IS_FAIL_fn(IR_EXEC(bb).value) ? bb->ω.node : bb->γ.node;
            }
            int ks = (g_current_cfg ? g_current_cfg->n : 64) * 64 + 256;
            IR_t *kc = key_sub; DESCR_t kv = NULVCL;
            while (kc && ks-- > 0) {
                IR_t *kn = IR_interp_node(kc);
                if (!IS_FAIL_fn(IR_EXEC(kc).value)) kv = IR_EXEC(kc).value;
                if (!kn || kn == bb->γ.node || kn == bb->ω.node || kn == kc) break;
                ag_ring_push(g_current_cfg, IR_EXEC(kc).value); kc = kn;
            }
            int match = 0;
            if (IS_INT_fn(sel) && IS_INT_fn(kv)) match = (sel.i == kv.i);
            else { const char *ss = VARVAL_fn(sel); const char *ks2 = VARVAL_fn(kv); match = (ss && ks2) ? !strcmp(ss,ks2) : (ss==ks2); }
            if (match) {
                int vs = (g_current_cfg ? g_current_cfg->n : 64) * 64 + 256;
                IR_t *vc = val_sub; DESCR_t vv = NULVCL;
                while (vc && vs-- > 0) {
                    IR_t *vn = IR_interp_node(vc);
                    if (IS_FAIL_fn(IR_EXEC(vc).value)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    vv = IR_EXEC(vc).value;
                    if (!vn || vn == bb->γ.node || vn == bb->ω.node || vn == vc) break;
                    ag_ring_push(g_current_cfg, IR_EXEC(vc).value); vc = vn;
                }
                IR_EXEC(bb).value = vv;
                return IS_FAIL_fn(IR_EXEC(bb).value) ? bb->ω.node : bb->γ.node;
            }
        }
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_PAT_BREAK: case IR_PAT_BREAKX: {
        const char *chars = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        if (IR_LIT(bb).dval == 1.0 && IR_LIT(bb).sval) { const char *cv = VARVAL_fn(NV_GET_fn(IR_LIT(bb).sval)); chars = cv ? cv : ""; }
        if (IR_EXEC(bb).state == 0) {
            int i = 0;
            while (Δ + i < Σlen && !strchr(chars, Σ[Δ + i])) i++;
            if (Δ + i >= Σlen) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = i;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = descr_match_span(Σ + Δ, i);
            Δ += i;
            return bb->γ.node;
        }
        if (IR_LIT(bb).ival == 1) {
            int origin = Δ - (int)IR_EXEC(bb).counter;
            int i = (int)IR_EXEC(bb).counter + 1;
            while (origin + i < Σlen && !strchr(chars, Σ[origin + i])) i++;
            if (origin + i >= Σlen || i <= (int)IR_EXEC(bb).counter) { Δ = origin; IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = i;
            IR_EXEC(bb).value   = descr_match_span(Σ + origin, i);
            Δ = origin + i;
            return bb->γ.node;
        }
        Δ -= (int)IR_EXEC(bb).counter;
        IR_EXEC(bb).state = 0;
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_PAT_SPAN: case IR_PAT_SPAN_VAR: {
        const char *chars = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        if (IR_LIT(bb).ival == 1 && IR_LIT(bb).sval) { const char *cv = VARVAL_fn(NV_GET_fn(IR_LIT(bb).sval)); chars = cv ? cv : ""; }
        if (IR_EXEC(bb).state == 0) {
            int i = 0;
            while (Δ + i < Σlen && strchr(chars, Σ[Δ + i])) i++;
            if (i == 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = i;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = descr_match_span(Σ + Δ, i);
            Δ += i;
            return bb->γ.node;
        }
        if (IR_EXEC(bb).state == 1) {
            Δ -= (int)IR_EXEC(bb).counter;
            IR_EXEC(bb).counter--;
            if (IR_EXEC(bb).counter < 1) { IR_EXEC(bb).state = 2; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = descr_match_span(Σ + Δ, (int)IR_EXEC(bb).counter);
            Δ += (int)IR_EXEC(bb).counter;
            return bb->γ.node;
        }
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_PAT_ARB: {
        if (IR_EXEC(bb).state == 0) {
            IR_EXEC(bb).counter = 0;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = descr_match_span(Σ + Δ, 0);
            return bb->γ.node;
        }
        if (IR_EXEC(bb).state == 1) {
            Δ -= (int)IR_EXEC(bb).counter;
            IR_EXEC(bb).counter++;
            if (Δ + (int)IR_EXEC(bb).counter > Σlen) {
                IR_EXEC(bb).state = 2;
                IR_EXEC(bb).value = FAILDESCR;
                return bb->ω.node;
            }
            IR_EXEC(bb).value = descr_match_span(Σ + Δ, (int)IR_EXEC(bb).counter);
            Δ += (int)IR_EXEC(bb).counter;
            return bb->γ.node;
        }
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_PAT_REM: {
        if (IR_EXEC(bb).state == 0) {
            int rem = Σlen - Δ;
            IR_EXEC(bb).counter = rem;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = descr_match_span(Σ + Δ, rem);
            Δ = Σlen;
            return bb->γ.node;
        }
        Δ -= (int)IR_EXEC(bb).counter;
        IR_EXEC(bb).state = 0;
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_PAT_FENCE: {
        if (IR_EXEC(bb).state == 0) {
            IR_EXEC(bb).counter = Δ;
            IR_EXEC(bb).state = 1;
            IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        Δ = (int)IR_EXEC(bb).counter;
        IR_EXEC(bb).state = 0;
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_PAT_ABORT: {
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_PAT_LEN: {
        int64_t n = (IR_LIT(bb).dval == 1.0 && IR_LIT(bb).sval) ? to_int(NV_GET_fn(IR_LIT(bb).sval)) : IR_LIT(bb).ival;
        if (IR_EXEC(bb).state == 0) {
            if (n < 0 || Δ + (int)n > Σlen) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = n;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = descr_match_span(Σ + Δ, (int)n);
            Δ += (int)n;
            return bb->γ.node;
        }
        Δ -= (int)IR_EXEC(bb).counter;
        IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_PAT_NOTANY: {
        const char *chars = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        if (IR_LIT(bb).dval == 1.0 && IR_LIT(bb).sval) { const char *cv = VARVAL_fn(NV_GET_fn(IR_LIT(bb).sval)); chars = cv ? cv : ""; }
        if (IR_EXEC(bb).state == 0) {
            if (Δ >= Σlen || strchr(chars, Σ[Δ])) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).state = 1;
            IR_EXEC(bb).value = descr_match_span(Σ + Δ, 1);
            Δ++;
            return bb->γ.node;
        }
        Δ--; IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_PAT_POS: {
        if (IR_EXEC(bb).state == 0) {
            int64_t arg      = (IR_LIT(bb).dval != 0.0 && IR_LIT(bb).sval) ? to_int(NV_GET_fn(IR_LIT(bb).sval)) : IR_LIT(bb).ival;
            int     from_end = (IR_LIT(bb).dval != 0.0) ? (IR_LIT(bb).dval == 1.0) : (IR_LIT(bb).sval && IR_LIT(bb).sval[0] == 'r');
            int     pos = from_end ? (Σlen - (int)arg) : (int)arg;
            if (pos < 0 || pos > Σlen || Δ != pos) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).state = 1;
            IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_PAT_ATP: {
        if (IR_EXEC(bb).state == 0) {
            if (IR_LIT(bb).sval && *IR_LIT(bb).sval) {
                DESCR_t pos = { .v = DT_I, .i = (int64_t)Δ };
                NV_SET_fn(IR_LIT(bb).sval, pos);
            }
            IR_EXEC(bb).state = 1;
            IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_PAT_TAB: case IR_PAT_RTAB: {
        if (IR_EXEC(bb).state == 0) {
            int64_t arg      = (IR_LIT(bb).dval != 0.0 && IR_LIT(bb).sval) ? to_int(NV_GET_fn(IR_LIT(bb).sval)) : IR_LIT(bb).ival;
            int     from_end = (IR_LIT(bb).dval != 0.0) ? (IR_LIT(bb).dval == 1.0) : (IR_LIT(bb).sval && IR_LIT(bb).sval[0] == 'r');
            int     target = from_end ? (Σlen - (int)arg) : (int)arg;
            if (target < 0 || target > Σlen || Δ > target) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = Δ;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = descr_match_span(Σ + Δ, target - Δ);
            Δ = target;
            return bb->γ.node;
        }
        Δ = (int)IR_EXEC(bb).counter;
        IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_PAT_CAT:
    case IR_PAT_ALT:
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    case IR_PAT_ASSIGN_COND: {
        IR_t * in0 = bb->n_operands > 0 ? bb->operands[0] : NULL;
        int fresh = (IR_EXEC(bb).state == 0) && (!in0 || IR_EXEC(in0).state == 0);
        if (fresh) {
            IR_EXEC(bb).counter = Δ;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = NULVCL;
            return in0;
        }
        if (IR_LIT(bb).sval && *IR_LIT(bb).sval) {
            int matched_len = Δ - (int)IR_EXEC(bb).counter;
            if (matched_len < 0) matched_len = 0;
            if (g_dcap_active) {
                bb_dcap_record(IR_LIT(bb).sval, (int)IR_EXEC(bb).counter, matched_len);
            } else {
                char *copy = (char *)GC_MALLOC((size_t)matched_len + 1);
                if (copy) { memcpy(copy, Σ + (int)IR_EXEC(bb).counter, (size_t)matched_len); copy[matched_len] = '\0'; }
                DESCR_t matched = { .v = DT_S, .slen = (uint32_t)matched_len, .s = copy ? copy : "" };
                NV_SET_fn(IR_LIT(bb).sval, matched);
            }
        }
        IR_EXEC(bb).state = 1;
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_PAT_ASSIGN_IMM: {
        IR_t * in0 = bb->n_operands > 0 ? bb->operands[0] : NULL;
        int fresh = (IR_EXEC(bb).state == 0) && (!in0 || IR_EXEC(in0).state == 0);
        if (fresh) {
            IR_EXEC(bb).counter = Δ;
            IR_EXEC(bb).state   = 1;
            IR_EXEC(bb).value   = NULVCL;
            return in0;
        }
        if (IR_LIT(bb).sval && *IR_LIT(bb).sval) {
            int matched_len = Δ - (int)IR_EXEC(bb).counter;
            if (matched_len < 0) matched_len = 0;
            if (g_dcap_active) {
                bb_dcap_record(IR_LIT(bb).sval, (int)IR_EXEC(bb).counter, matched_len);
            } else {
                char *copy = (char *)GC_MALLOC((size_t)matched_len + 1);
                if (copy) { memcpy(copy, Σ + (int)IR_EXEC(bb).counter, (size_t)matched_len); copy[matched_len] = '\0'; }
                DESCR_t matched = { .v = DT_S, .slen = (uint32_t)matched_len, .s = copy ? copy : "" };
                NV_SET_fn(IR_LIT(bb).sval, matched);
            }
        }
        IR_EXEC(bb).state = 1;
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_PAT_ARBNO: {
        bb_arbno_state_t * az = (bb_arbno_state_t *)(intptr_t)IR_EXEC(bb).counter;
        IR_graph_t * inner_blk = az ? az->inner : NULL;
        int       * pos_stack = az ? az->pos_stack : NULL;
        if (!inner_blk || !pos_stack) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) {
            int depth = 0;
            int cap   = az->cap;
            az->saved_delta = Δ;
            while (depth < cap) {
                int pre = Δ;
                DESCR_t r = IR_interp_once(inner_blk);
                if (IS_FAIL_fn(r) || Δ == pre) break;
                pos_stack[depth++] = Δ;
            }
            IR_EXEC(bb).state = depth;
            IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        IR_EXEC(bb).state--;
        if (IR_EXEC(bb).state < 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        Δ = (IR_EXEC(bb).state > 0) ? pos_stack[IR_EXEC(bb).state - 1] : az->saved_delta;
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_PAT_DEFER: {
        if (IR_EXEC(bb).state == 1) {
            IR_graph_t *sub_bb = NULL;
            memcpy(&sub_bb, &IR_LIT(bb).dval, sizeof sub_bb);
            int origin = (int)IR_EXEC(bb).counter;
            if (!sub_bb || !sub_bb->entry) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            const char *save_Σ = Σ; int save_Σlen = Σlen; int save_Ω = Ω; int save_Δ = Δ;
            int prev_match = save_Δ - origin;
            Σ = save_Σ + origin; Σlen = save_Σlen - origin; Ω = Σlen; Δ = prev_match;
            DESCR_t result = IR_interp_resume(sub_bb);
            int ok = !IS_FAIL_fn(result);
            int new_match = ok ? Δ : 0;
            Σ = save_Σ; Σlen = save_Σlen; Ω = save_Ω; Δ = save_Δ;
            if (!ok) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            Δ = origin + new_match;
            IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        if (IR_EXEC(bb).state >= 2) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        const char *vname = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        DESCR_t val = NV_GET_fn(vname);
        if (IR_LIT(bb).ival) {
            if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s);
            else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val);
        }
        if (val.v == DT_S || val.v == DT_SNUL) {
            const char *lit = val.s ? val.s : "";
            int llen = val.slen ? (int)val.slen : (int)strlen(lit);
            if (Δ + llen > Σlen) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            if (llen > 0 && strncmp(Σ + Δ, lit, (size_t)llen) != 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            Δ += llen;
            IR_EXEC(bb).state = 2; IR_EXEC(bb).value = NULVCL;
            return bb->γ.node;
        }
        if (val.v == DT_P && val.p) {
            extern long rt_dtp_run(DTP_t *h, const char *s, long delta, long Delta);
            long nd = rt_dtp_run((DTP_t *)val.p, Σ, (long)Δ, (long)Σlen);
            if (nd >= 0) { Δ = (int)nd; IR_EXEC(bb).state = 2; IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
    case IR_UPTO: {
        if (IR_EXEC(bb).state == 0) IR_EXEC(bb).counter = 0;
        IR_EXEC(bb).state = 1;
        const char *cset = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        const char *hay  = scan_subj ? scan_subj : "";
        int slen = (int)strlen(hay);
        while (IR_EXEC(bb).counter < slen) {
            char c = hay[IR_EXEC(bb).counter];
            IR_EXEC(bb).counter++;
            if (strchr(cset, c)) {
                IR_EXEC(bb).value = INTVAL((int64_t)IR_EXEC(bb).counter);
                return bb->γ.node;
            }
        }
        IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_TO: {
        IR_t * Lc = ir_pair_arg(bb, 0);
        IR_t * Hc = ir_pair_arg(bb, 1);
        if (IR_LIT(bb).sval && IR_LIT(bb).sval[0] == 'a') {
            int have_ops = (Lc && Hc);
            if (IR_EXEC(bb).state == 0) {
                DESCR_t lv = have_ops ? IR_EXEC(Lc).value : ag_ring_peek(g_current_cfg, 1);
                DESCR_t hv = have_ops ? IR_EXEC(Hc).value : ag_ring_peek(g_current_cfg, 0);
                if (IS_FAIL_fn(lv) || IS_FAIL_fn(hv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                int64_t lo = IS_INT_fn(lv) ? lv.i : (lv.v == DT_R ? (int64_t)lv.r : 0);
                int64_t hi = IS_INT_fn(hv) ? hv.i : (hv.v == DT_R ? (int64_t)hv.r : 0);
                IR_EXEC(bb).counter = lo;
                memcpy(&IR_LIT(bb).dval, &hi, 8);
                IR_EXEC(bb).state   = 1;
            } else {
                if (have_ops && (Hc->op == IR_BINOP || Hc->op == IR_VAR || Hc->op == IR_LIT_I || Hc->op == IR_LIT_F)) {
                    DESCR_t hv_now = IR_EXEC(Hc).value;
                    int64_t hi_now = IS_INT_fn(hv_now) ? hv_now.i : (hv_now.v == DT_R ? (int64_t)hv_now.r : -1);
                    int64_t hi_was; memcpy(&hi_was, &IR_LIT(bb).dval, 8);
                    if (hi_now != hi_was) {
                        DESCR_t lv_now = IR_EXEC(Lc).value;
                        int64_t lo_now = IS_INT_fn(lv_now) ? lv_now.i : (lv_now.v == DT_R ? (int64_t)lv_now.r : 0);
                        IR_EXEC(bb).counter = lo_now;
                        memcpy(&IR_LIT(bb).dval, &hi_now, 8);
                    } else { IR_EXEC(bb).counter++; }
                } else { IR_EXEC(bb).counter++; }
            }
            int64_t hi_cached;
            memcpy(&hi_cached, &IR_LIT(bb).dval, 8);
            if (IR_EXEC(bb).counter > hi_cached) {
                if (have_ops) {
                    IR_t * rt_tgt = gen_resume_target(Hc);
                    if (rt_tgt) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return rt_tgt; }
                    rt_tgt = gen_resume_target(Lc);
                    if (rt_tgt) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return rt_tgt; }
                }
                IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
            IR_EXEC(bb).value = INTVAL(IR_EXEC(bb).counter);
            return bb->γ.node;
        }
        #define IR_IS_GEN_KIND_TO(k) ( \
            (k) == IR_TO || (k) == IR_TO_BY || (k) == IR_UPTO || \
            (k) == IR_ALT || (k) == IR_BINOP_GEN || \
            (k) == IR_ITERATE || (k) == IR_LIMIT || (k) == IR_PROC_GEN || \
            (k) == IR_LIST_BANG || (k) == IR_KEY_GEN || (k) == IR_TO_BY)
        int has_dyn = (Lc && Hc);
        int lo_gen  = has_dyn && IR_IS_GEN_KIND_TO(Lc->op);
        int hi_gen  = has_dyn && IR_IS_GEN_KIND_TO(Hc->op);
        int64_t hi;
        if (IR_EXEC(bb).state == 0) {
            if (has_dyn) {
                IR_interp_node(Lc);
                if (IS_FAIL_fn(IR_EXEC(Lc).value)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                IR_interp_node(Hc);
                if (IS_FAIL_fn(IR_EXEC(Hc).value)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).counter = IR_EXEC(Lc).value.i;
            } else {
                int64_t lo;
                memcpy(&hi, &IR_LIT(bb).dval, 8);
                lo = IR_LIT(bb).ival;
                IR_EXEC(bb).counter = lo;
            }
        }
        else IR_EXEC(bb).counter++;
        IR_EXEC(bb).state = 1;
        if (has_dyn) hi = IR_EXEC(Hc).value.i; else memcpy(&hi, &IR_LIT(bb).dval, 8);
        if (IR_EXEC(bb).counter > hi) {
            if (hi_gen) {
                IR_interp_node(Hc);
                if (IS_FAIL_fn(IR_EXEC(Hc).value)) {
                    if (!lo_gen) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_interp_node(Lc);
                    if (IS_FAIL_fn(IR_EXEC(Lc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_EXEC(Hc).state = 0;
                    IR_interp_node(Hc);
                    if (IS_FAIL_fn(IR_EXEC(Hc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                    IR_EXEC(bb).counter = IR_EXEC(Lc).value.i;
                } else {
                    IR_EXEC(bb).counter = Lc ? IR_EXEC(Lc).value.i : IR_LIT(bb).ival;
                }
                hi = IR_EXEC(Hc).value.i;
                if (IR_EXEC(bb).counter > hi) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            } else if (lo_gen) {
                IR_interp_node(Lc);
                if (IS_FAIL_fn(IR_EXEC(Lc).value)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).counter = IR_EXEC(Lc).value.i;
                if (IR_EXEC(bb).counter > hi) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            } else {
                IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
        }
        IR_EXEC(bb).value = INTVAL(IR_EXEC(bb).counter);
        return bb->γ.node;
        #undef IR_IS_GEN_KIND_TO
    }
    case IR_ITERATE: {
        if (IR_LIT(bb).sval) {
            DESCR_t src = NV_GET_fn(IR_LIT(bb).sval);
            if (src.v != DT_S || !src.s) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            if (IR_EXEC(bb).state == 0) { IR_EXEC(bb).counter = 0; IR_EXEC(bb).state = 1; }
            int64_t total = (int64_t)(src.slen > 0 ? src.slen : (int64_t)strlen(src.s));
            if (IR_EXEC(bb).counter >= total) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            int64_t end = IR_EXEC(bb).counter;
            while (end < total && src.s[end] != '\x01') end++;
            int64_t seg_len = end - IR_EXEC(bb).counter;
            char *seg = GC_malloc(seg_len + 1);
            memcpy(seg, src.s + IR_EXEC(bb).counter, seg_len);
            seg[seg_len] = '\0';
            DESCR_t out = (DESCR_t){ .v = DT_S, .slen = (uint32_t)seg_len, .s = seg };
            IR_EXEC(bb).counter = end + 1;
            IR_EXEC(bb).value = out;
            return bb->γ.node;
        }
        if (!((IR_t*)0)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) {
            IR_interp_node(((IR_t*)0));
            if (IS_FAIL_fn(IR_EXEC(((IR_t*)0)).value)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).counter = 0;
        } else {
            IR_EXEC(bb).counter++;
        }
        IR_EXEC(bb).state = 1;
        DESCR_t sv = IR_EXEC(((IR_t*)0)).value;
        const char *str = (sv.v == DT_S && sv.s) ? sv.s : "";
        int64_t len = (sv.v == DT_S) ? (int64_t)(sv.slen > 0 ? sv.slen : (int64_t)strlen(str)) : 0;
        if (IR_EXEC(bb).counter >= len) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        char *ch = GC_malloc(2);
        ch[0] = str[IR_EXEC(bb).counter];
        ch[1] = '\0';
        IR_EXEC(bb).value = (DESCR_t){ .v = DT_S, .slen = 1, .s = ch };
        return bb->γ.node;
    }
    case IR_GEN_ALT: {
        alt_dcg_t *z = (alt_dcg_t *)(intptr_t)IR_EXEC(bb).counter;
        if (!z) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) {
            z->which = 0;
            DESCR_t v = z->gen[0].fn(z->gen[0].ζ, α);
            if (!IS_FAIL_fn(v)) { IR_EXEC(bb).value = v; IR_EXEC(bb).state = 1; return bb->γ.node; }
            z->which = 1;
            DESCR_t v2 = z->gen[1].fn(z->gen[1].ζ, α);
            if (!IS_FAIL_fn(v2)) { IR_EXEC(bb).value = v2; IR_EXEC(bb).state = 1; return bb->γ.node; }
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        DESCR_t v = z->gen[z->which].fn(z->gen[z->which].ζ, β);
        if (!IS_FAIL_fn(v)) { IR_EXEC(bb).value = v; return bb->γ.node; }
        if (z->which == 0) {
            z->which = 1;
            DESCR_t v2 = z->gen[1].fn(z->gen[1].ζ, α);
            if (!IS_FAIL_fn(v2)) { IR_EXEC(bb).value = v2; return bb->γ.node; }
        }
        IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_GEN_BINOP: {
        binop_dcg_t *z = (binop_dcg_t *)(intptr_t)IR_EXEC(bb).counter;
        if (!z) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) {
            z->left_val = z->left.fn(z->left.ζ, α);
            if (IS_FAIL_fn(z->left_val)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            z->right_val = z->right.fn(z->right.ζ, α);
            if (IS_FAIL_fn(z->right_val)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).state = 1;
        } else {
            for (;;) {
                DESCR_t rv = z->right.fn(z->right.ζ, β);
                if (!IS_FAIL_fn(rv)) { z->right_val = rv; break; }
                DESCR_t lv = z->left.fn(z->left.ζ, β);
                if (IS_FAIL_fn(lv)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                z->left_val = lv;
                z->right_val = z->right.fn(z->right.ζ, α);
                if (!IS_FAIL_fn(z->right_val)) break;
                IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
        }
        for (;;) {
            int rel_fail = 0;
            DESCR_t result = binop_apply(z->op, z->left_val, z->right_val, &rel_fail);
            if (!IS_FAIL_fn(result)) { IR_EXEC(bb).value = result; return bb->γ.node; }
            if (!rel_fail) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            DESCR_t rv = z->right.fn(z->right.ζ, β);
            if (!IS_FAIL_fn(rv)) { z->right_val = rv; continue; }
            DESCR_t lv = z->left.fn(z->left.ζ, β);
            if (IS_FAIL_fn(lv)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            z->left_val = lv;
            z->right_val = z->right.fn(z->right.ζ, α);
            if (IS_FAIL_fn(z->right_val)) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        }
    }
    case IR_TO_NESTED: {
        to_nested_state_t *z = (to_nested_state_t *)(intptr_t)IR_EXEC(bb).counter;
        if (!z || z->nlo == 0 || z->nhi == 0) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) { z->li = 0; z->hi2 = 0; z->cur = z->lo_vals[0]; IR_EXEC(bb).state = 1; }
        else z->cur++;
        while (z->cur > z->hi_vals[z->hi2]) {
            z->hi2++;
            if (z->hi2 >= z->nhi) { z->hi2 = 0; z->li++; }
            if (z->li >= z->nlo) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            z->cur = z->lo_vals[z->li];
        }
        IR_EXEC(bb).value = INTVAL(z->cur);
        return bb->γ.node;
    }
    case IR_PROC_GEN: {
        GeneratorState *gs = (GeneratorState *)(intptr_t)IR_EXEC(bb).counter;
        if (!gs) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        DESCR_t v;
        int ok = bb_broker_drive_sm_one(gs, &v);
        if (!ok) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).state = 1;
        IR_EXEC(bb).value = v;
        return bb->γ.node;
    }
    case IR_GCONJ: {
        IR_EXEC(bb).value = INTVAL(1);
        return bb->γ.node;
    }
    case IR_ITE: {
        bb_ite_state_t * zi = (bb_ite_state_t *)(intptr_t)IR_LIT(bb).ival;
        if (IR_EXEC(bb).state > 0 && zi && zi->seen_seq == g_pl_yield_seq) { IR_EXEC(bb).value = INTVAL(1); return zi->committed ? zi->then_ : zi->else_; }
        if (zi) { zi->cp_mark = (void *)resolve_cp_current(); zi->committed = 0; zi->seen_seq = g_pl_yield_seq; }
        IR_EXEC(bb).state = 1;
        IR_EXEC(bb).value = INTVAL(1);
        return ir_call_arg(bb, 0);
    }
    case IR_ITE_COMMIT: {
        bb_ite_state_t * zi = (bb_ite_state_t *)(intptr_t)IR_LIT(bb).ival;
        if (zi) { resolve_cp_truncate((resolve_choice *)zi->cp_mark); zi->committed = 1; }
        IR_EXEC(bb).value = INTVAL(1);
        return bb->γ.node;
    }
    case IR_ITE_GATE: {
        bb_ite_state_t * zi = (bb_ite_state_t *)(intptr_t)IR_LIT(bb).ival;
        if (zi && zi->committed) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = INTVAL(1);
        return bb->γ.node;
    }
    case IR_CATCH: {
        extern Trail g_resolve_trail; extern Term **g_resolve_env;
        bb_catch_state_t *zc = (bb_catch_state_t *)(intptr_t)IR_LIT(bb).ival;
        if (!zc || !zc->goal_g) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        Term *catcher = resolve_node_to_term(zc->catcher);
        jmp_buf *jb_p = (jmp_buf *)resolve_catch_push(catcher, g_resolve_env);
        if (!jb_p) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (setjmp(*jb_p) == 0) {
            bb_reset(zc->goal_g);
            DESCR_t res = IR_interp_once(zc->goal_g);
            resolve_catch_pop_top();
            if (IS_FAIL_fn(res)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = res; return bb->γ.node;
        } else {
            int tm = resolve_catch_top_trail_mark();
            Term **saved_env = resolve_catch_top_env();
            trail_unwind(&g_resolve_trail, tm);
            if (saved_env) g_resolve_env = saved_env;
            resolve_catch_pop_top();
            Term *exc = resolve_catch_take_exception();
            if (exc) {
                int mark2 = trail_mark(&g_resolve_trail);
                if (!unify(catcher, exc, &g_resolve_trail)) {
                    trail_unwind(&g_resolve_trail, mark2);
                    resolve_throw_term(exc);
                    IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
                }
            }
            if (!zc->rec_g) { IR_EXEC(bb).value = INTVAL(1); return bb->γ.node; }
            bb_reset(zc->rec_g);
            DESCR_t res = IR_interp_once(zc->rec_g);
            if (IS_FAIL_fn(res)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value = res; return bb->γ.node;
        }
    }
    case IR_DISJ: {
        extern Trail g_resolve_trail; extern Term **g_resolve_env;
        int n_arm = 0;
        IR_t * const * arms = bb_operand_aux_get(g_current_cfg, bb, &n_arm);
        if (!arms || n_arm <= 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) {
            IR_EXEC(bb).state = 1;
            IR_EXEC(bb).counter = 0;
            IR_LIT(bb).ival = (int64_t)trail_mark(&g_resolve_trail);
            IR_EXEC(bb).value = INTVAL(1);
            return pl_disj_arm_enter(arms[0]);
        }
        trail_unwind(&g_resolve_trail, (int)IR_LIT(bb).ival);
        IR_EXEC(bb).counter += 1;
        if (IR_EXEC(bb).counter >= n_arm) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_LIT(bb).ival = (int64_t)trail_mark(&g_resolve_trail);
        g_pl_yield_seq += 1;
        IR_EXEC(bb).value = INTVAL(1);
        return pl_disj_arm_enter(arms[(int)IR_EXEC(bb).counter]);
    }
    case IR_CHOICE: {
        extern Trail g_resolve_trail; extern Term **g_resolve_env; extern int g_resolve_cut_flag;
        bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)IR_LIT(bb).ival;
        if (!zc || zc->nbodies == 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0 && zc->idx_ok && zc->idx_key) {
            long ckey = resolve_term_first_arg_key((g_resolve_env && g_resolve_env[0]) ? g_resolve_env[0] : NULL);
            if (ckey != RESOLVE_IDX_NOKEY) {
                int cand = -1, ncand = 0;
                for (int ci = 0; ci < zc->nbodies; ci++) {
                    long k = zc->idx_key[ci];
                    if (k == RESOLVE_IDX_VAR || k == ckey) { ncand++; if (ncand == 1) cand = ci; else break; }
                }
                if (ncand == 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                if (ncand == 1 && (bb_body_single_solution(zc->bodies[cand])
                                   || bb_body_cp_free_except_tail(zc->bodies[cand]))) {
                    extern int g_resolve_cut_flag;
                    static int idx_trace_on = 0, idx_trace_inited = 0;
                    if (!idx_trace_inited) { const char *e = getenv("SCRIP_IDX_TRACE"); idx_trace_on = (e && e[0]=='1'); idx_trace_inited = 1; }
                    if (idx_trace_on) fprintf(stderr, "[IDX] CP-ELIDED clause=%d/%d key=0x%lx (single matching clause, no resolve_cp_push)\n", cand, zc->nbodies, ckey);
                    resolve_choice *idx_saved_barrier = g_resolve_cut_barrier;
                    int idx_saved_cut = g_resolve_cut_flag;
                    g_resolve_cut_flag = 0;
                    g_resolve_cut_barrier = g_resolve_bfr;
                    int mark = trail_mark(&g_resolve_trail);
                    Term **saved_env_idx = g_resolve_env;
                    IR_graph_t *body = zc->bodies[cand];
                    DESCR_t res = body ? IR_interp_once(body) : FAILDESCR;
                    if (!IS_FAIL_fn(res)) {
                        g_resolve_cut_flag = idx_saved_cut; g_resolve_cut_barrier = idx_saved_barrier;
                        IR_EXEC(bb).value = res; return bb->γ.node;
                    }
                    g_resolve_cut_flag = idx_saved_cut; g_resolve_cut_barrier = idx_saved_barrier;
                    trail_unwind(&g_resolve_trail, mark);
                    g_resolve_env = saved_env_idx;
                    IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
                }
            }
        }
        resolve_choice *saved_barrier = g_resolve_cut_barrier;
        int saved_cut = g_resolve_cut_flag;
        g_resolve_cut_flag = 0;
        if (IR_EXEC(bb).state == 0) {
            zc->cut_barrier = (void *)g_resolve_bfr;
            g_resolve_cut_barrier = (resolve_choice *)zc->cut_barrier;
            zc->cp = (void *)resolve_cp_push(RESOLVE_CP_CLAUSE, trail_mark(&g_resolve_trail), g_resolve_env, (void *)bb, 0);
        } else {
            g_resolve_cut_barrier = (resolve_choice *)zc->cut_barrier;
        }
        int cp_cut_away = 0;
        if (IR_EXEC(bb).state > 0 && zc->cp) {
            cp_cut_away = 1;
            for (resolve_choice *c = resolve_cp_current(); c; c = c->parent) if (c == (resolve_choice *)zc->cp && c->resume == (void *)bb) { cp_cut_away = 0; break; }
        }
        int spine_says_live = (IR_EXEC(bb).state > 0 && zc->last_body && zc->cp != NULL && !cp_cut_away
                               && resolve_cp_current() != (resolve_choice *)zc->cp);
        int inner_live = spine_says_live
                         || (IR_EXEC(bb).state > 0 && zc->last_body && bb_body_live_choice_cut_aware(zc->last_body));
        if (inner_live) {
            IR_graph_t *lb = zc->last_body;
            DESCR_t res = IR_interp_resume(lb);
            if (!IS_FAIL_fn(res)) {
                g_resolve_cut_flag = saved_cut; g_resolve_cut_barrier = saved_barrier;
                IR_EXEC(bb).value = res; return bb->γ.node;
            }
            if (g_resolve_cut_flag || cp_cut_away) {
                g_resolve_cut_flag = saved_cut; g_resolve_cut_barrier = saved_barrier;
                IR_EXEC(bb).state = 0; zc->last_body = NULL; zc->cp = NULL;
                IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
            trail_unwind(&g_resolve_trail, zc->mark);
            g_resolve_env = (Term **)zc->saved_env;
            zc->last_body = NULL;
        } else if (IR_EXEC(bb).state > 0) {
            if (cp_cut_away) {
                g_resolve_cut_flag = saved_cut; g_resolve_cut_barrier = saved_barrier;
                IR_EXEC(bb).state = 0; zc->last_body = NULL; zc->cp = NULL;
                IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
            trail_unwind(&g_resolve_trail, zc->mark);
            g_resolve_env = (Term **)zc->saved_env;
            zc->last_body = NULL;
        }
        int start = IR_EXEC(bb).state;
        for (int ci = start; ci < zc->nbodies; ci++) {
            int mark = trail_mark(&g_resolve_trail);
            IR_graph_t *body = zc->bodies[ci];
            Term **saved_for_retry = g_resolve_env;
            DESCR_t res = body ? IR_interp_once(body) : FAILDESCR;
            if (!IS_FAIL_fn(res)) {
                zc->last_body = body;
                if (start > 0) g_pl_yield_seq += 1;
                g_resolve_cut_flag = saved_cut; g_resolve_cut_barrier = saved_barrier;
                IR_EXEC(bb).state = ci + 1;
                zc->mark = mark;
                zc->saved_env = (void *)saved_for_retry;
                if (zc->cp && resolve_cp_current() == (resolve_choice *)zc->cp) { ((resolve_choice *)zc->cp)->cursor = ci + 1; ((resolve_choice *)zc->cp)->trail_mark = mark; }
                IR_EXEC(bb).value = res; return bb->γ.node;
            }
            if (g_resolve_cut_flag) {
                g_resolve_cut_flag = saved_cut; g_resolve_cut_barrier = saved_barrier;
                IR_EXEC(bb).state = 0; zc->last_body = NULL; zc->cp = NULL;
                IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
            trail_unwind(&g_resolve_trail, mark);
            g_resolve_env = saved_for_retry;
        }
        g_resolve_cut_flag = saved_cut; g_resolve_cut_barrier = saved_barrier;
        IR_EXEC(bb).state = 0; zc->last_body = NULL;
        if (zc->cp && resolve_cp_current() == (resolve_choice *)zc->cp) resolve_cp_pop();
        zc->cp = NULL;
        IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    case IR_GOAL: {
        extern Term **g_resolve_env; extern Trail g_resolve_trail;
        bb_goal_state_t *zc = (bb_goal_state_t *)(intptr_t)IR_LIT(bb).ival;
        if (!zc) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        const char *callee = zc->callee; int carity = zc->arity;
        if (!callee) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        char key[128]; snprintf(key, sizeof key, "%s/%d", callee, carity);
        if ((carity >= 1 && strcmp(callee, "call") == 0) ||
            (carity == 1 && strcmp(callee, "once") == 0)) {
            if (IR_EXEC(bb).state != 0) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            Term *gt = (zc->nargs >= 1 && zc->args && zc->args[0]) ? resolve_node_to_term(zc->args[0]) : NULL;
            gt = term_deref(gt);
            if (!gt) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            int ok;
            if (carity == 1) {
                ok = resolve_call_term(gt);
            } else {
                int n_extra = carity - 1;
                Term **extras = (Term **)malloc((size_t)n_extra * sizeof(Term *));
                for (int i = 0; i < n_extra; i++) {
                    IR_t *ab = (zc->nargs > i + 1) ? zc->args[i + 1] : NULL;
                    extras[i] = ab ? term_deref(resolve_node_to_term(ab)) : NULL;
                }
                ok = resolve_call_term_n(gt, n_extra, extras);
                free(extras);
            }
            if (!ok) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).state = 1; IR_EXEC(bb).value = INTVAL(1); return bb->γ.node;
        }
        Resolve_PredEntry_BB *pe = resolve_bb_lookup(key, carity);
        IR_graph_t *_bcfg = bb_graph_of_pred(pe);
        if (!_bcfg) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        if (IR_EXEC(bb).state == 0) {
            resolve_choice *lco_entry_bfr = g_resolve_bfr;
            int lco_tail_pos = (bb->γ.node == NULL);
            if (lco_tail_pos && g_resolve_bfr == NULL) {
                int redirect_ready = 0;
                IR_graph_t *redirect_body = NULL;
                int nslots_lco = carity + 16;
                Term **callee_env_lco = NULL;
                int b3_base = g_resolve_trail.top;
                if (_bcfg->entry && _bcfg->entry->op != IR_CHOICE && bb_body_cp_free_except_tail(_bcfg)) {
                    callee_env_lco = calloc((size_t)nslots_lco, sizeof(Term *));
                    for (int ai = 0; ai < zc->nargs && ai < carity; ai++) {
                        if (!zc->args[ai]) continue;
                        Term *at = term_new_var(ai);
                        callee_env_lco[ai] = at;
                        Term *caller_term = resolve_node_to_term(zc->args[ai]);
                        if (caller_term) unify(at, caller_term, &g_resolve_trail);
                    }
                    redirect_body = _bcfg; redirect_ready = 1;
                } else if (_bcfg->entry && _bcfg->entry->op == IR_CHOICE) {
                    callee_env_lco = calloc((size_t)nslots_lco, sizeof(Term *));
                    for (int ai = 0; ai < zc->nargs && ai < carity; ai++) {
                        if (!zc->args[ai]) continue;
                        Term *at = term_new_var(ai);
                        callee_env_lco[ai] = at;
                        Term *caller_term = resolve_node_to_term(zc->args[ai]);
                        if (caller_term) unify(at, caller_term, &g_resolve_trail);
                    }
                    IR_graph_t *ub = resolve_choice_unique_indexed_body(_bcfg, callee_env_lco[0]);
                    if (ub) { redirect_body = ub; redirect_ready = 1; }
                    else    { free(callee_env_lco); callee_env_lco = NULL; }
                }
                if (redirect_ready) {
                    extern int g_resolve_b3_call_mark;
                    if (g_resolve_b3_call_mark < 0 || g_resolve_b3_call_mark > b3_base) g_resolve_b3_call_mark = b3_base;
                    int dst   = g_resolve_b3_call_mark;
                    int n_fwd = g_resolve_trail.top - b3_base;
                    if (dst < b3_base && n_fwd >= 0) {
                        for (int k = 0; k < n_fwd; k++) g_resolve_trail.stack[dst + k] = g_resolve_trail.stack[b3_base + k];
                        g_resolve_trail.top = dst + n_fwd;
                    }
                    g_resolve_env = callee_env_lco;
                    bb_reset(redirect_body);
                    g_resolve_tail_redirect_cfg   = redirect_body;
                    g_resolve_tail_redirect_entry = redirect_body->entry;
                    IR_EXEC(bb).state = 0;
                    {
                        static int lco_acted_inited = 0;
                        static int lco_acted_on = 0;
                        if (!lco_acted_inited) { const char *e = getenv("SCRIP_LCO_TRACE"); lco_acted_on = (e && e[0]=='2'); lco_acted_inited = 1; }
                        if (lco_acted_on) fprintf(stderr, "[LCO] ACTED %s/%d frame-reuse redirect%s\n", callee, carity,
                                                 (_bcfg->entry && _bcfg->entry->op == IR_CHOICE) ? " (B2 indexed)" : "");
                    }
                    return NULL;
                }
            }
            int nslots = carity + 16;
            g_resolve_b3_call_mark = -1;
            Term **callee_env = calloc((size_t)nslots, sizeof(Term *));
            int mark = trail_mark(&g_resolve_trail);
            for (int ai = 0; ai < zc->nargs && ai < carity; ai++) {
                if (!zc->args[ai]) continue;
                Term *at = term_new_var(ai);
                callee_env[ai] = at;
                Term *caller_term = resolve_node_to_term(zc->args[ai]);
                if (caller_term) unify(at, caller_term, &g_resolve_trail);
            }
            Term **saved_env = g_resolve_env;
            g_resolve_env = callee_env;
            bb_node_state_t *caller_snap = bb_snapshot_state(_bcfg);
            bb_reset(_bcfg);
            DESCR_t res = IR_interp_once(_bcfg);
            if (IS_FAIL_fn(res)) {
                bb_restore_state(_bcfg, caller_snap);
                trail_unwind(&g_resolve_trail, mark); g_resolve_env = saved_env; free(callee_env);
                IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
            }
            PlCallSt *cs = malloc(sizeof(PlCallSt));
            cs->callee_env = callee_env; cs->saved_env = saved_env; cs->trail_mark = mark;
            cs->nslots = nslots;
            cs->act = bb_snapshot_state(_bcfg);
            cs->cp_floor = (void *)lco_entry_bfr; cs->disj_hint = pl_callee_disj_hint(_bcfg);
            bb_restore_state(_bcfg, caller_snap);
            zc->cs = cs;
            IR_EXEC(bb).state = 1;
            g_resolve_env = saved_env;
            {
                static int lco_trace_inited = 0;
                static int lco_trace_on = 0;
                if (!lco_trace_inited) { const char *e = getenv("SCRIP_LCO_TRACE"); lco_trace_on = (e && e[0]=='1'); lco_trace_inited = 1; }
                if (lco_trace_on) {
                    int lco_det = (g_resolve_bfr == lco_entry_bfr) && !bb_body_has_live_choice(_bcfg);
                    int lco_eligible = lco_tail_pos && lco_det;
                    fprintf(stderr, "[LCO] %s/%d tail=%d det=%d eligible=%d bfr_before=%p bfr_after=%p\n",
                            callee, carity, lco_tail_pos, lco_det, lco_eligible,
                            (void*)lco_entry_bfr, (void*)g_resolve_bfr);
                }
            }
            IR_EXEC(bb).value = INTVAL(1); return bb->γ.node;
        }
        PlCallSt *cs = (PlCallSt *)zc->cs;
        if (!cs) { IR_EXEC(bb).state = 0; IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        g_resolve_env = cs->callee_env;
        bb_node_state_t *caller_snap2 = bb_snapshot_state(_bcfg);
        bb_restore_state(_bcfg, cs->act); cs->act = NULL;
        if (!bb_body_live_choice_cut_aware(_bcfg)) {
            bb_restore_state(_bcfg, caller_snap2);
            trail_unwind(&g_resolve_trail, cs->trail_mark);
            g_resolve_env = cs->saved_env; free(cs); zc->cs = NULL; IR_EXEC(bb).state = 0;
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        DESCR_t res2 = IR_interp_resume(_bcfg);
        if (IS_FAIL_fn(res2)) {
            bb_restore_state(_bcfg, caller_snap2);
            trail_unwind(&g_resolve_trail, cs->trail_mark);
            g_resolve_env = cs->saved_env; free(cs); zc->cs = NULL; IR_EXEC(bb).state = 0;
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        cs->act = bb_snapshot_state(_bcfg);
        cs->disj_hint = pl_callee_disj_hint(_bcfg);
        bb_restore_state(_bcfg, caller_snap2);
        g_resolve_env = cs->saved_env;
        IR_EXEC(bb).value = INTVAL(1); return bb->γ.node;
    }
    case IR_CUT: {
        extern int g_resolve_cut_flag;
        if (IR_EXEC(bb).state == 0) { resolve_cp_truncate(g_resolve_cut_barrier); g_resolve_cut_flag = 1; IR_EXEC(bb).state = 1; }
        IR_EXEC(bb).value = INTVAL(1); return bb->γ.node;
    }
    case IR_ATOM: {
        IR_EXEC(bb).value = IR_LIT(bb).sval ? STRVAL(IR_LIT(bb).sval) : NULVCL;
        return bb->γ.node;
    }
    case IR_STRUCT: {
        Term *t = resolve_node_to_term(bb);
        IR_EXEC(bb).value = (DESCR_t){ .v = DT_DATA, .ptr = t };
        return bb->γ.node;
    }
    case IR_LOGICVAR: {
        extern Term **g_resolve_env;
        int slot = (int)IR_LIT(bb).ival;
        if (!g_resolve_env || slot < 0) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        Term *t = g_resolve_env[slot] ? term_deref(g_resolve_env[slot]) : NULL;
        if (!t) { IR_EXEC(bb).value = NULVCL; return bb->γ.node; }
        if (t->tag == TERM_INT)   { IR_EXEC(bb).value = INTVAL(t->ival);  return bb->γ.node; }
        if (t->tag == TERM_FLOAT) { IR_EXEC(bb).value = REALVAL(t->fval); return bb->γ.node; }
        if (t->tag == TERM_ATOM)  { const char *nm = prolog_atom_name(t->atom_id); IR_EXEC(bb).value = nm ? STRVAL(nm) : NULVCL; return bb->γ.node; }
        if (t->tag == TERM_COMPOUND) { IR_EXEC(bb).value = (DESCR_t){ .v = DT_DATA, .ptr = t }; return bb->γ.node; }
        IR_EXEC(bb).value = NULVCL;
        return bb->γ.node;
    }
    case IR_ARITH: {
        DESCR_t v = resolve_arith_eval(bb);
        if (IS_FAIL_fn(v)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = v;
        return bb->γ.node;
    }
    case IR_UNIFY: {
        extern Term **g_resolve_env; extern Trail g_resolve_trail;
        if (bb->n_operands < 2 || !bb->operands[0] || !bb->operands[1]) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        Term *lt = resolve_node_to_term(bb->operands[0]);
        Term *rt = resolve_node_to_term(bb->operands[1]);
        if (!lt || !rt) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        int mark = trail_mark(&g_resolve_trail);
        if (!unify(lt, rt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
        IR_EXEC(bb).value = INTVAL(1);
        return bb->γ.node;
    }
    case IR_BUILTIN: {
        const char *fn = IR_LIT(bb).sval ? IR_LIT(bb).sval : "";
        if (strcmp(fn,"throw")==0) {
            IR_t *thr0 = ir_call_arg(bb,0); Term *ball = thr0 ? resolve_node_to_term(thr0) : term_new_atom(prolog_atom_intern("error"));
            resolve_throw_term(ball);
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        if (strcmp(fn,"findall")==0) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            bb_findall_state_t *fs = (bb_findall_state_t *)(intptr_t)IR_LIT(bb).ival;
            if (!fs || !fs->gcfg) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            int mark = trail_mark(&g_resolve_trail);
            Term **acc = (Term **)GC_MALLOC(4096 * sizeof(Term *)); int nacc=0;
            bb_reset(fs->gcfg);
            DESCR_t res = IR_interp_once(fs->gcfg);
            int fa_safety = fs->gcfg->n * 256 + 4096;
            while (!IS_FAIL_fn(res) && fa_safety-- > 0) {
                if (nacc >= 4096) break;
                acc[nacc++] = bb_copy_term(resolve_node_to_term(fs->tmpl));
                if (!bb_body_has_live_choice(fs->gcfg)) break;
                res = IR_interp_resume(fs->gcfg);
            }
            trail_unwind(&g_resolve_trail, mark);
            Term *lst = term_new_atom(prolog_atom_intern("[]"));
            for (int i=nacc-1; i>=0; i--) {
                Term **c=(Term**)GC_MALLOC(2*sizeof(Term*)); c[0]=acc[i]; c[1]=lst;
                lst=term_new_compound(ATOM_DOT,2,c);
            }
            int mark2 = trail_mark(&g_resolve_trail);
            if (!unify(resolve_node_to_term(fs->result), lst, &g_resolve_trail)) {
                trail_unwind(&g_resolve_trail,mark2); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
        }
        if (strcmp(fn, "nl") == 0) { putchar('\n'); IR_EXEC(bb).value = INTVAL(1); return bb->γ.node; }
        if (ir_pair_arg(bb,0) && ir_pair_arg(bb,1) &&
            (strcmp(fn,">")==0||strcmp(fn,"<")==0||strcmp(fn,">=")==0||strcmp(fn,"=<")==0||strcmp(fn,"<=")==0||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0)) {
            DESCR_t lv = resolve_arith_eval(ir_pair_arg(bb,0));
            DESCR_t rv = resolve_arith_eval(ir_pair_arg(bb,1));
            if (IS_FAIL_fn(lv) || IS_FAIL_fn(rv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
            double l = (lv.v == DT_I) ? (double)lv.i : lv.r;
            double r = (rv.v == DT_I) ? (double)rv.i : rv.r;
            int ok = (strcmp(fn,">")==0)?(l>r):(strcmp(fn,"<")==0)?(l<r):(strcmp(fn,">=")==0)?(l>=r):(strcmp(fn,"=<")==0||strcmp(fn,"<=")==0)?(l<=r):(strcmp(fn,"=:=")==0)?(l==r):(l!=r);
            if (ok) { IR_EXEC(bb).value = INTVAL(1); return bb->γ.node; }
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        if (ir_pair_arg(bb,0) && ir_pair_arg(bb,1) &&
            (strcmp(fn,"==")==0||strcmp(fn,"\\==")==0||strcmp(fn,"@<")==0||strcmp(fn,"@>")==0
             ||strcmp(fn,"@=<")==0||strcmp(fn,"@>=")==0)) {
            int c = resolve_term_compare(resolve_node_to_term(ir_pair_arg(bb,0)), resolve_node_to_term(ir_pair_arg(bb,1)));
            int ok = (strcmp(fn,"==")==0)?(c==0):(strcmp(fn,"\\==")==0)?(c!=0)
                   :(strcmp(fn,"@<")==0)?(c<0):(strcmp(fn,"@>")==0)?(c>0)
                   :(strcmp(fn,"@=<")==0)?(c<=0):(c>=0);
            if (ok) { IR_EXEC(bb).value=INTVAL(1); return bb->γ.node; }
            IR_EXEC(bb).value=FAILDESCR; return bb->ω.node;
        }
        if (ir_pair_arg(bb,0) && ir_pair_arg(bb,1) && strcmp(fn,"succ")==0) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            Term *xt = resolve_node_to_term(ir_pair_arg(bb,0)); Term *yt = resolve_node_to_term(ir_pair_arg(bb,1));
            Term *xd = xt ? term_deref(xt) : NULL; Term *yd = yt ? term_deref(yt) : NULL;
            int mark = trail_mark(&g_resolve_trail);
            if (xd && xd->tag == TERM_INT) {
                if (xd->ival < 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                Term *vt = term_new_int(xd->ival + 1);
                if (!unify(yt, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value = INTVAL(1); return bb->γ.node;
            }
            if (yd && yd->tag == TERM_INT) {
                if (yd->ival <= 0) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                Term *vt = term_new_int(yd->ival - 1);
                if (!unify(xt, vt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value = INTVAL(1); return bb->γ.node;
            }
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        if (strcmp(fn,"plus")==0 && IR_LIT(bb).ival==3) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            IR_t *a0=ir_call_arg(bb,0), *a1=ir_call_arg(bb,1), *a2=ir_call_arg(bb,2);
            if (!a0||!a1||!a2) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            Term *t0=resolve_node_to_term(a0); Term *t1=resolve_node_to_term(a1); Term *t2=resolve_node_to_term(a2);
            Term *d0=t0?term_deref(t0):NULL; Term *d1=t1?term_deref(t1):NULL; Term *d2=t2?term_deref(t2):NULL;
            int v0=(d0&&d0->tag==TERM_INT), v1=(d1&&d1->tag==TERM_INT), v2=(d2&&d2->tag==TERM_INT);
            int mark=trail_mark(&g_resolve_trail);
            int ok=0;
            if (v0&&v1) { Term *r=term_new_int(d0->ival+d1->ival); ok=unify(t2,r,&g_resolve_trail); }
            else if (v0&&v2) { Term *r=term_new_int(d2->ival-d0->ival); ok=unify(t1,r,&g_resolve_trail); }
            else if (v1&&v2) { Term *r=term_new_int(d2->ival-d1->ival); ok=unify(t0,r,&g_resolve_trail); }
            if (!ok) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
        }
        if ((strcmp(fn,"nb_setval")==0||strcmp(fn,"nb_getval")==0) && IR_LIT(bb).ival==2) {
            extern Trail g_resolve_trail;
            IR_t *a0=ir_call_arg(bb,0), *a1=ir_call_arg(bb,1);
            if (!a0||!a1) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            Term *kt=resolve_node_to_term(a0); Term *kd=kt?term_deref(kt):NULL;
            if (!kd||kd->tag!=TERM_ATOM) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            int aid=kd->atom_id;
            if (strcmp(fn,"nb_setval")==0) {
                Term *vt=resolve_node_to_term(a1); Term *vd=vt?term_deref(vt):NULL;
                resolve_nb_set(aid, vd ? vd : vt);
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            } else {
                Term *stored=resolve_nb_get(aid);
                if (!stored) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                int mark=trail_mark(&g_resolve_trail);
                Term *vt=resolve_node_to_term(a1);
                if (!unify(vt,stored,&g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
        }
        if (strcmp(fn,"aggregate_all")==0) {
            bb_findall_state_t *fs_agg = (bb_findall_state_t *)(intptr_t)IR_LIT(bb).ival;
            extern int rt_aggregate(void *agg_ptr);
            if (!fs_agg || !fs_agg->gcfg) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            if (rt_aggregate((void *)fs_agg)) { IR_EXEC(bb).value=INTVAL(1); return bb->γ.node; }
            IR_EXEC(bb).value=FAILDESCR; return bb->ω.node;
        }
        if (ir_call_arg(bb,0) && IR_LIT(bb).ival==1 &&
            (strcmp(fn,"var")==0||strcmp(fn,"nonvar")==0||strcmp(fn,"atom")==0||strcmp(fn,"atomic")==0
             ||strcmp(fn,"number")==0||strcmp(fn,"integer")==0||strcmp(fn,"float")==0||strcmp(fn,"compound")==0
             ||strcmp(fn,"callable")==0||strcmp(fn,"is_list")==0||strcmp(fn,"ground")==0)) {
            Term *t = resolve_node_to_term(ir_call_arg(bb,0)); Term *d = t ? term_deref(t) : NULL;
            int ok = 0;
            int isvar = (!d || d->tag==TERM_VAR);
            if (strcmp(fn,"var")==0)            ok =  isvar;
            else if (strcmp(fn,"nonvar")==0)    ok = !isvar;
            else if (strcmp(fn,"atom")==0)      ok = (d && d->tag==TERM_ATOM);
            else if (strcmp(fn,"integer")==0)   ok = (d && d->tag==TERM_INT);
            else if (strcmp(fn,"float")==0)     ok = (d && d->tag==TERM_FLOAT);
            else if (strcmp(fn,"number")==0)    ok = (d && (d->tag==TERM_INT||d->tag==TERM_FLOAT));
            else if (strcmp(fn,"atomic")==0)    ok = (d && (d->tag==TERM_ATOM||d->tag==TERM_INT||d->tag==TERM_FLOAT));
            else if (strcmp(fn,"compound")==0)  ok = (d && d->tag==TERM_COMPOUND);
            else if (strcmp(fn,"callable")==0)  ok = (d && (d->tag==TERM_ATOM||d->tag==TERM_COMPOUND));
            else if (strcmp(fn,"ground")==0)    ok = resolve_term_is_ground(d);
            else if (strcmp(fn,"is_list")==0)   ok = resolve_term_is_proper_list(d);
            if (ok) { IR_EXEC(bb).value = INTVAL(1); return bb->γ.node; }
            IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
        }
        if (strcmp(fn,"functor")==0 && IR_LIT(bb).ival==3) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            IR_t *a0=ir_call_arg(bb,0), *a1=ir_call_arg(bb,1), *a2=ir_call_arg(bb,2);
            Term *t0 = resolve_node_to_term(a0); Term *d0 = t0?term_deref(t0):NULL;
            int mark = trail_mark(&g_resolve_trail);
            if (d0 && d0->tag!=TERM_VAR) {
                Term *nameT, *arityT;
                if (d0->tag==TERM_COMPOUND) { nameT=term_new_atom(d0->compound.functor); arityT=term_new_int(d0->compound.arity); }
                else if (d0->tag==TERM_ATOM){ nameT=term_new_atom(d0->atom_id);          arityT=term_new_int(0); }
                else if (d0->tag==TERM_INT) { nameT=term_new_int(d0->ival);              arityT=term_new_int(0); }
                else if (d0->tag==TERM_FLOAT){nameT=term_new_float(d0->fval);            arityT=term_new_int(0); }
                else { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                if (!unify(resolve_node_to_term(a1),nameT,&g_resolve_trail) || !unify(resolve_node_to_term(a2),arityT,&g_resolve_trail)) {
                    trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            } else {
                Term *nT=resolve_node_to_term(a1), *aT=resolve_node_to_term(a2);
                Term *nd1=nT?term_deref(nT):NULL, *ad=aT?term_deref(aT):NULL;
                if (!ad || ad->tag!=TERM_INT) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                long ar = ad->ival;
                Term *built;
                if (ar==0) { built = nd1 ? nd1 : term_new_atom(prolog_atom_intern("[]")); }
                else {
                    if (!nd1 || nd1->tag!=TERM_ATOM) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    Term **args=(Term**)GC_MALLOC((size_t)ar*sizeof(Term*));
                    for (long i=0;i<ar;i++) args[i]=term_new_var(-1);
                    built=term_new_compound(nd1->atom_id,(int)ar,args);
                }
                if (!unify(t0,built,&g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
        }
        if (strcmp(fn,"arg")==0 && IR_LIT(bb).ival==3) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            IR_t *a0=ir_call_arg(bb,0), *a1=ir_call_arg(bb,1), *a2=ir_call_arg(bb,2);
            Term *nT=resolve_node_to_term(a0); Term *nd0=nT?term_deref(nT):NULL;
            Term *tT=resolve_node_to_term(a1); Term *td=tT?term_deref(tT):NULL;
            if (!nd0||nd0->tag!=TERM_INT||!td||td->tag!=TERM_COMPOUND) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            long n = nd0->ival;
            if (n<1 || n>td->compound.arity) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            int mark = trail_mark(&g_resolve_trail);
            if (!unify(resolve_node_to_term(a2), td->compound.args[n-1], &g_resolve_trail)) {
                trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
        }
        if (strcmp(fn,"=..")==0 && IR_LIT(bb).ival==2) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            IR_t *a0=ir_call_arg(bb,0), *a1=ir_call_arg(bb,1);
            Term *t0=resolve_node_to_term(a0); Term *d0=t0?term_deref(t0):NULL;
            int mark = trail_mark(&g_resolve_trail);
            if (d0 && d0->tag!=TERM_VAR) {
                Term *lst;
                if (d0->tag==TERM_COMPOUND) {
                    lst = term_new_atom(prolog_atom_intern("[]"));
                    for (int i=d0->compound.arity-1; i>=0; i--) {
                        Term **c=(Term**)GC_MALLOC(2*sizeof(Term*)); c[0]=d0->compound.args[i]; c[1]=lst;
                        lst=term_new_compound(ATOM_DOT,2,c);
                    }
                    Term **c=(Term**)GC_MALLOC(2*sizeof(Term*)); c[0]=term_new_atom(d0->compound.functor); c[1]=lst;
                    lst=term_new_compound(ATOM_DOT,2,c);
                } else {
                    Term **c=(Term**)GC_MALLOC(2*sizeof(Term*)); c[0]=d0; c[1]=term_new_atom(prolog_atom_intern("[]"));
                    lst=term_new_compound(ATOM_DOT,2,c);
                }
                if (!unify(resolve_node_to_term(a1),lst,&g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            } else {
                Term *lt=resolve_node_to_term(a1); Term *ld=lt?term_deref(lt):NULL;
                Term *elems[64]; int ne=0;
                Term *cur=ld;
                while (cur && cur->tag==TERM_COMPOUND && cur->compound.functor==ATOM_DOT && cur->compound.arity==2) {
                    if (ne>=64) break;
                    elems[ne++]=term_deref(cur->compound.args[0]);
                    cur=term_deref(cur->compound.args[1]);
                }
                if (ne==0) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                Term *built;
                if (ne==1) { built=elems[0]; }
                else {
                    Term *h=elems[0];
                    if (!h||h->tag!=TERM_ATOM) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    Term **args=(Term**)GC_MALLOC((size_t)(ne-1)*sizeof(Term*));
                    for (int i=1;i<ne;i++) args[i-1]=elems[i];
                    built=term_new_compound(h->atom_id,ne-1,args);
                }
                if (!unify(t0,built,&g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
        }
        if (strcmp(fn,"char_type")==0 && IR_LIT(bb).ival==2 && ir_call_arg(bb,0)) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            IR_t *a0=ir_call_arg(bb,0), *a1=ir_call_arg(bb,1);
            char b0[256]; const char *cs = resolve_atomic_text(resolve_node_to_term(a0), b0, sizeof b0);
            if (!cs || !cs[0]) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            unsigned char ch = (unsigned char)cs[0];
            int mark = trail_mark(&g_resolve_trail);
            if (a1 && (a1->op==IR_STRUCT || a1->op==IR_ARITH) && IR_LIT(a1).sval) {
                const char *ty = IR_LIT(a1).sval; IR_t *inner = ir_call_arg(a1,0);
                Term *out = NULL;
                if (strcmp(ty,"digit")==0)         { if (!isdigit(ch)) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; } out=term_new_int((long)(ch-'0')); }
                else if (strcmp(ty,"to_lower")==0) { char c2[2]={(char)tolower(ch),0}; out=term_new_atom(prolog_atom_intern(c2)); }
                else if (strcmp(ty,"to_upper")==0) { char c2[2]={(char)toupper(ch),0}; out=term_new_atom(prolog_atom_intern(c2)); }
                else if (strcmp(ty,"upper")==0)    { if (!isupper(ch)) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; } char c2[2]={(char)tolower(ch),0}; out=term_new_atom(prolog_atom_intern(c2)); }
                else if (strcmp(ty,"lower")==0)    { if (!islower(ch)) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; } char c2[2]={(char)toupper(ch),0}; out=term_new_atom(prolog_atom_intern(c2)); }
                else if (strcmp(ty,"code")==0)     { out=term_new_int((long)ch); }
                else { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                if (!unify(resolve_node_to_term(inner), out, &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            char b1[256]; const char *ty = resolve_atomic_text(resolve_node_to_term(a1), b1, sizeof b1);
            if (!ty) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            int ok = 0;
            if      (strcmp(ty,"alpha")==0)       ok=isalpha(ch);
            else if (strcmp(ty,"alnum")==0)       ok=isalnum(ch);
            else if (strcmp(ty,"digit")==0)       ok=isdigit(ch);
            else if (strcmp(ty,"space")==0||strcmp(ty,"white")==0) ok=isspace(ch);
            else if (strcmp(ty,"upper")==0)       ok=isupper(ch);
            else if (strcmp(ty,"lower")==0)       ok=islower(ch);
            else if (strcmp(ty,"punct")==0)       ok=ispunct(ch);
            else if (strcmp(ty,"graph")==0)       ok=isgraph(ch);
            else if (strcmp(ty,"csym")==0)        ok=(isalnum(ch)||ch=='_');
            else if (strcmp(ty,"csymf")==0)       ok=(isalpha(ch)||ch=='_');
            else if (strcmp(ty,"end_of_line")==0) ok=(ch=='\n'||ch=='\r');
            else if (strcmp(ty,"newline")==0)     ok=(ch=='\n');
            else { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            if (!ok) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
        }
        if ((strcmp(fn,"atom_string")==0||strcmp(fn,"number_string")==0||strcmp(fn,"string_upper")==0
             ||strcmp(fn,"string_lower")==0||strcmp(fn,"string_concat")==0||strcmp(fn,"string_length")==0
             ||strcmp(fn,"string_chars")==0||strcmp(fn,"string_codes")==0||strcmp(fn,"term_to_atom")==0
             ||strcmp(fn,"term_string")==0
             ||strcmp(fn,"atom_number")==0||strcmp(fn,"copy_term")==0||strcmp(fn,"atomic_list_concat")==0
             ||strcmp(fn,"concat_atom")==0||strcmp(fn,"string_to_atom")==0) && ir_call_arg(bb,0)) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            IR_t *a0=ir_call_arg(bb,0), *a1=ir_call_arg(bb,1), *a2=ir_call_arg(bb,2);
            int mark = trail_mark(&g_resolve_trail);
            if (strcmp(fn,"string_length")==0) {
                char b[256]; const char *s=resolve_atomic_text(resolve_node_to_term(a0),b,sizeof b);
                if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                if (!unify(resolve_node_to_term(a1), term_new_int((long)strlen(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"string_upper")==0 || strcmp(fn,"string_lower")==0) {
                char b[256]; const char *s=resolve_atomic_text(resolve_node_to_term(a0),b,sizeof b);
                if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                size_t n=strlen(s); char *o=(char*)GC_MALLOC(n+1); int up=(strcmp(fn,"string_upper")==0);
                for (size_t i=0;i<n;i++) o[i]= up?(char)toupper((unsigned char)s[i]):(char)tolower((unsigned char)s[i]);
                o[n]='\0';
                if (!unify(resolve_node_to_term(a1), term_new_atom(prolog_atom_intern(o)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"string_concat")==0) {
                char b0[256],b1[256]; const char *s0=resolve_atomic_text(resolve_node_to_term(a0),b0,sizeof b0);
                const char *s1=resolve_atomic_text(resolve_node_to_term(a1),b1,sizeof b1);
                if (!s0||!s1) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                size_t l0=strlen(s0),l1=strlen(s1); char *c=(char*)GC_MALLOC(l0+l1+1);
                memcpy(c,s0,l0); memcpy(c+l0,s1,l1); c[l0+l1]='\0';
                if (!unify(resolve_node_to_term(a2), term_new_atom(prolog_atom_intern(c)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            Term *t0=resolve_node_to_term(a0); Term *d0=t0?term_deref(t0):NULL;
            Term *t1=resolve_node_to_term(a1); Term *d1=t1?term_deref(t1):NULL;
            if (strcmp(fn,"copy_term")==0) {
                Term *cp = bb_copy_term(d0 ? d0 : t0);
                if (!unify(t1, cp, &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"atomic_list_concat")==0 || strcmp(fn,"concat_atom")==0) {
                int sep3 = (IR_LIT(bb).ival==3);
                char sepbuf[256]; const char *sep = "";
                if (sep3) { sep = resolve_atomic_text(resolve_node_to_term(a1), sepbuf, sizeof sepbuf); if (!sep) sep=""; }
                Term *cur = d0; char out[1024]; size_t oi=0; int first=1;
                while (cur && cur->tag==TERM_COMPOUND && cur->compound.functor==ATOM_DOT && cur->compound.arity==2) {
                    Term *el=term_deref(cur->compound.args[0]); char eb[256];
                    const char *es=resolve_atomic_text(el,eb,sizeof eb);
                    if (!es) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    if (sep3 && !first) { size_t sl=strlen(sep); if (oi+sl<sizeof(out)) { memcpy(out+oi,sep,sl); oi+=sl; } }
                    first=0;
                    size_t el_n=strlen(es); if (oi+el_n>=sizeof(out)) break;
                    memcpy(out+oi,es,el_n); oi+=el_n;
                    cur=term_deref(cur->compound.args[1]);
                }
                out[oi]='\0';
                IR_t *rb = sep3 ? a2 : a1;
                if (!unify(resolve_node_to_term(rb), term_new_atom(prolog_atom_intern(out)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"string_to_atom")==0) {
                if (d0 && d0->tag!=TERM_VAR) {
                    char b[256]; const char *s=resolve_atomic_text(d0,b,sizeof b);
                    if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    if (!unify(t1, term_new_atom(prolog_atom_intern(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                } else {
                    char b[256]; const char *s=resolve_atomic_text(d1,b,sizeof b);
                    if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    if (!unify(t0, term_new_atom(prolog_atom_intern(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"term_to_atom")==0) {
                if (d0 && d0->tag!=TERM_VAR) {
                    extern char *pl_term_to_string(Term *);
                    char *s = pl_term_to_string(d0);
                    if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    Term *at = term_new_atom(prolog_atom_intern(s)); free(s);
                    if (!unify(t1, at, &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                } else {
                    IR_EXEC(bb).value=FAILDESCR; return bb->ω.node;
                }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"term_string")==0) {
                if (d0 && d0->tag!=TERM_VAR) {
                    extern char *pl_term_to_string(Term *);
                    char *s = pl_term_to_string(d0);
                    if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    Term *at = term_new_atom(prolog_atom_intern(s)); free(s);
                    if (!unify(t1, at, &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                } else {
                    IR_EXEC(bb).value=FAILDESCR; return bb->ω.node;
                }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"atom_string")==0) {
                if (d0 && d0->tag!=TERM_VAR) {
                    char b[256]; const char *s=resolve_atomic_text(d0,b,sizeof b);
                    if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    if (!unify(t1, term_new_atom(prolog_atom_intern(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                } else {
                    char b[256]; const char *s=resolve_atomic_text(d1,b,sizeof b);
                    if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    if (!unify(t0, term_new_atom(prolog_atom_intern(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"number_string")==0 || strcmp(fn,"atom_number")==0) {
                int num_first = (strcmp(fn,"number_string")==0);
                Term *numNode = num_first ? d0 : d1; Term *txtNode = num_first ? d1 : d0;
                IR_t *numBB = num_first ? a0 : a1;   IR_t *txtBB = num_first ? a1 : a0;
                if (numNode && numNode->tag!=TERM_VAR) {
                    char b[256]; const char *s=resolve_atomic_text(numNode,b,sizeof b);
                    if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    if (!unify(resolve_node_to_term(txtBB), term_new_atom(prolog_atom_intern(s)), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                } else {
                    char b[256]; const char *s=resolve_atomic_text(txtNode,b,sizeof b);
                    if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    char *endp=NULL; long iv=strtol(s,&endp,10);
                    Term *nt;
                    if (endp && *endp=='\0') nt=term_new_int(iv);
                    else { char *fe=NULL; double dv=strtod(s,&fe); if (!fe||*fe!='\0') { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; } nt=term_new_float(dv); }
                    if (!unify(resolve_node_to_term(numBB), nt, &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            {
                int as_codes=(strcmp(fn,"string_codes")==0);
                if (d0 && d0->tag!=TERM_VAR) {
                    char b[256]; const char *s=resolve_atomic_text(d0,b,sizeof b);
                    if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    size_t n=strlen(s); Term *lst=term_new_atom(prolog_atom_intern("[]"));
                    for (size_t i=n;i>0;i--) { unsigned char c=(unsigned char)s[i-1]; Term *el;
                        if (as_codes) el=term_new_int((long)c); else { char cs[2]={(char)c,0}; el=term_new_atom(prolog_atom_intern(cs)); }
                        Term **cc=(Term**)GC_MALLOC(2*sizeof(Term*)); cc[0]=el; cc[1]=lst; lst=term_new_compound(ATOM_DOT,2,cc); }
                    if (!unify(t1,lst,&g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                    IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
                }
                IR_EXEC(bb).value=FAILDESCR; return bb->ω.node;
            }
        }
        if ((strcmp(fn,"atom_length")==0||strcmp(fn,"atom_concat")==0||strcmp(fn,"atom_chars")==0
             ||strcmp(fn,"atom_codes")==0||strcmp(fn,"upcase_atom")==0||strcmp(fn,"downcase_atom")==0)
            && ir_call_arg(bb,0)) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            IR_t *a0=ir_call_arg(bb,0), *a1=ir_call_arg(bb,1), *a2=ir_call_arg(bb,2);
            int mark = trail_mark(&g_resolve_trail);
            if (strcmp(fn,"atom_length")==0) {
                char buf[256]; const char *s = resolve_atomic_text(resolve_node_to_term(a0), buf, sizeof buf);
                if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                if (!unify(resolve_node_to_term(a1), term_new_int((long)strlen(s)), &g_resolve_trail)) {
                    trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"atom_concat")==0) {
                char b0[256], b1[256];
                const char *s0=resolve_atomic_text(resolve_node_to_term(a0),b0,sizeof b0);
                const char *s1=resolve_atomic_text(resolve_node_to_term(a1),b1,sizeof b1);
                if (!s0||!s1) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                size_t l0=strlen(s0), l1=strlen(s1);
                char *cat=(char*)GC_MALLOC(l0+l1+1); memcpy(cat,s0,l0); memcpy(cat+l0,s1,l1); cat[l0+l1]='\0';
                if (!unify(resolve_node_to_term(a2), term_new_atom(prolog_atom_intern(cat)), &g_resolve_trail)) {
                    trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            if (strcmp(fn,"upcase_atom")==0 || strcmp(fn,"downcase_atom")==0) {
                char buf[256]; const char *s = resolve_atomic_text(resolve_node_to_term(a0), buf, sizeof buf);
                if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                size_t n=strlen(s); char *out=(char*)GC_MALLOC(n+1);
                int up = (strcmp(fn,"upcase_atom")==0);
                for (size_t i=0;i<n;i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
                out[n]='\0';
                if (!unify(resolve_node_to_term(a1), term_new_atom(prolog_atom_intern(out)), &g_resolve_trail)) {
                    trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
            int as_codes = (strcmp(fn,"atom_codes")==0);
            Term *t0=resolve_node_to_term(a0); Term *d0=t0?term_deref(t0):NULL;
            if (d0 && d0->tag!=TERM_VAR) {
                char buf[256]; const char *s = resolve_atomic_text(d0, buf, sizeof buf);
                if (!s) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                size_t n=strlen(s);
                Term *lst=term_new_atom(prolog_atom_intern("[]"));
                for (size_t i=n; i>0; i--) {
                    unsigned char ch=(unsigned char)s[i-1];
                    Term *el;
                    if (as_codes) el=term_new_int((long)ch);
                    else { char cs[2]={(char)ch,'\0'}; el=term_new_atom(prolog_atom_intern(cs)); }
                    Term **c=(Term**)GC_MALLOC(2*sizeof(Term*)); c[0]=el; c[1]=lst;
                    lst=term_new_compound(ATOM_DOT,2,c);
                }
                if (!unify(resolve_node_to_term(a1), lst, &g_resolve_trail)) {
                    trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            } else {
                Term *lt=resolve_node_to_term(a1); Term *cur=lt?term_deref(lt):NULL;
                char out[256]; size_t oi=0;
                while (cur && cur->tag==TERM_COMPOUND && cur->compound.functor==ATOM_DOT && cur->compound.arity==2) {
                    Term *el=term_deref(cur->compound.args[0]);
                    if (oi>=sizeof(out)-1) break;
                    if (as_codes) { if (!el||el->tag!=TERM_INT){IR_EXEC(bb).value=FAILDESCR;return bb->ω.node;} out[oi++]=(char)el->ival; }
                    else { if (!el||el->tag!=TERM_ATOM){IR_EXEC(bb).value=FAILDESCR;return bb->ω.node;} const char *cn=prolog_atom_name(el->atom_id); out[oi++]=cn?cn[0]:'?'; }
                    cur=term_deref(cur->compound.args[1]);
                }
                out[oi]='\0';
                if (!unify(t0, term_new_atom(prolog_atom_intern(out)), &g_resolve_trail)) {
                    trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
                IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
            }
        }
        if (ir_call_arg(bb,0)) {
            IR_t *b0 = ir_call_arg(bb,0);
            if (strcmp(fn, "write") == 0 || strcmp(fn, "writeln") == 0) {
                extern void pl_write(Term *);
                if (b0->op == IR_ARITH && IR_LIT(b0).ival > 0) {
                    Term *wt = resolve_node_to_term(b0);
                    if (wt) pl_write(term_deref(wt));
                } else {
                    IR_interp_node(b0); DESCR_t av = IR_EXEC(b0).value;
                    if (av.v == DT_I) printf("%ld", (long)av.i);
                    else if (av.v == DT_R) { char fb[64]; resolve_format_float(fb, sizeof fb, av.r); fputs(fb, stdout); }
                    else if ((av.v == DT_S || av.v == DT_SNUL) && av.s) fputs(av.s, stdout);
                    else if (av.v == DT_DATA && av.ptr) pl_write((Term *)av.ptr);
                }
                if (strcmp(fn, "writeln") == 0) putchar('\n');
                IR_EXEC(bb).value = INTVAL(1); return bb->γ.node;
            }
            IR_interp_node(b0); DESCR_t av = IR_EXEC(b0).value;
            if (strcmp(fn, "is") == 0 && ir_pair_arg(bb,1)) {
                extern Term **g_resolve_env; extern Trail g_resolve_trail;
                DESCR_t rv = resolve_arith_eval(ir_pair_arg(bb,1));
                if (IS_FAIL_fn(rv)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; }
                Term *vt = (rv.v == DT_I) ? term_new_int((long)rv.i) : term_new_float(rv.r);
                Term *lhs = resolve_node_to_term(b0);
                if (lhs) { if (!unify(lhs, vt, &g_resolve_trail)) { IR_EXEC(bb).value = FAILDESCR; return bb->ω.node; } }
                IR_EXEC(bb).value = INTVAL(1); return bb->γ.node;
            }
        }
        if ((strcmp(fn,"sort")==0||strcmp(fn,"msort")==0) && ir_call_arg(bb,0)) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            int do_msort = (strcmp(fn,"msort")==0);
            Term *lst = resolve_node_to_term(ir_call_arg(bb,0));
            if (!lst) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            lst = term_deref(lst);
            Term **elems = (Term **)GC_MALLOC(4096 * sizeof(Term *)); int n=0;
            Term *cur = lst;
            while (cur && cur->tag==TERM_COMPOUND && cur->compound.functor==ATOM_DOT && cur->compound.arity==2 && n<4096) {
                elems[n++] = term_deref(cur->compound.args[0]);
                cur = term_deref(cur->compound.args[1]);
            }
            for (int i=1;i<n;i++) { Term *key=elems[i]; int j=i-1; while(j>=0 && resolve_term_compare(elems[j],key)>0){elems[j+1]=elems[j];j--;} elems[j+1]=key; }
            int m=0; int *out_idx = (int *)GC_MALLOC(4096 * sizeof(int));
            for (int i=0;i<n;i++) {
                if (!do_msort && m>0 && resolve_term_compare(elems[out_idx[m-1]],elems[i])==0) continue;
                out_idx[m++]=i;
            }
            Term *result = term_new_atom(ATOM_NIL);
            for (int i=m-1;i>=0;i--) { Term **args=(Term**)GC_MALLOC(2*sizeof(Term*)); args[0]=elems[out_idx[i]]; args[1]=result; result=term_new_compound(ATOM_DOT,2,args); }
            IR_t *a1 = ir_call_arg(bb,1);
            Term *out_var = a1 ? resolve_node_to_term(a1) : NULL;
            if (!out_var) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            int mark = trail_mark(&g_resolve_trail);
            if (!unify(out_var, result, &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
        }
        if (strcmp(fn,"format")==0) {
            IR_t *f0 = ir_call_arg(bb,0); Term *fmt_t = f0 ? resolve_node_to_term(f0) : NULL;
            if (!fmt_t) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            fmt_t = term_deref(fmt_t);
            const char *fmt = NULL;
            if (fmt_t && fmt_t->tag==TERM_ATOM) fmt = prolog_atom_name(fmt_t->atom_id);
            else if (fmt_t && fmt_t->tag==TERM_COMPOUND && fmt_t->compound.functor==ATOM_DOT) {
                static char fmtbuf[1024]; int fi=0;
                Term *c = fmt_t;
                while (c && c->tag==TERM_COMPOUND && c->compound.functor==ATOM_DOT && c->compound.arity==2 && fi<1023) {
                    Term *ch = term_deref(c->compound.args[0]);
                    if (ch && ch->tag==TERM_INT) fmtbuf[fi++]=(char)ch->ival;
                    else if (ch && ch->tag==TERM_ATOM) { const char *n=prolog_atom_name(ch->atom_id); if(n&&n[0]) fmtbuf[fi++]=n[0]; }
                    c = term_deref(c->compound.args[1]);
                }
                fmtbuf[fi]=0; fmt=fmtbuf;
            }
            if (!fmt) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            Term *args_list = NULL;
            if (IR_LIT(bb).ival==2 && ir_call_arg(bb,1)) args_list = term_deref(resolve_node_to_term(ir_call_arg(bb,1)));
            Term *arg_cur = args_list;
            for (const char *p = fmt; *p; p++) {
                if (*p=='~' && *(p+1)) {
                    p++;
                    if (*p=='n'||*p=='N') { putchar('\n'); }
                    else if (*p=='i') {
                        if (arg_cur && arg_cur->tag==TERM_COMPOUND && arg_cur->compound.functor==ATOM_DOT && arg_cur->compound.arity==2)
                            arg_cur = term_deref(arg_cur->compound.args[1]);
                    }
                    else if (*p=='a'||*p=='w'||*p=='d'||*p=='p') {
                        extern void pl_write(Term *);
                        Term *arg = NULL;
                        if (arg_cur && arg_cur->tag==TERM_COMPOUND && arg_cur->compound.functor==ATOM_DOT && arg_cur->compound.arity==2) {
                            arg = term_deref(arg_cur->compound.args[0]);
                            arg_cur = term_deref(arg_cur->compound.args[1]);
                        }
                        if (arg) {
                            if (arg->tag==TERM_INT) printf("%ld",(long)arg->ival);
                            else if (arg->tag==TERM_FLOAT) { char fb[64]; resolve_format_float(fb,sizeof fb,arg->fval); fputs(fb,stdout); }
                            else if (arg->tag==TERM_ATOM) { const char *n=prolog_atom_name(arg->atom_id); if(n) fputs(n,stdout); }
                            else pl_write(arg);
                        }
                    }
                    else if (*p=='~') putchar('~');
                } else putchar(*p);
            }
            IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
        }
        if (strcmp(fn,"numbervars")==0 && ir_call_arg(bb,0)) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            Term *term_arg = resolve_node_to_term(ir_call_arg(bb,0));
            IR_t *a1_nd = ir_call_arg(bb,1);
            IR_t *a2_nd = ir_call_arg(bb,2);
            if (!a1_nd || !a2_nd) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            Term *start_t = term_deref(resolve_node_to_term(a1_nd));
            if (!start_t || start_t->tag!=TERM_INT) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            long counter = start_t->ival;
            int atom_var_id = prolog_atom_intern("$VAR");
            typedef struct { Term *t; } NVWork;
            NVWork stack[2048]; int top=0;
            if (term_arg) { stack[top].t=term_arg; top++; }
            while (top>0) {
                Term *t = term_deref(stack[--top].t);
                if (!t) continue;
                if (t->tag==TERM_VAR) {
                    Term **a=(Term**)GC_MALLOC(sizeof(Term*)); a[0]=term_new_int(counter++);
                    Term *vt=term_new_compound(atom_var_id,1,a);
                    unify(t, vt, &g_resolve_trail);
                } else if (t->tag==TERM_COMPOUND) {
                    for (int i=t->compound.arity-1;i>=0;i--) if(top<2048){stack[top].t=t->compound.args[i];top++;}
                }
            }
            Term *end_var = resolve_node_to_term(a2_nd);
            int mark = trail_mark(&g_resolve_trail);
            if (end_var && !unify(end_var, term_new_int(counter), &g_resolve_trail)) { trail_unwind(&g_resolve_trail,mark); IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
        }
        if ((strcmp(fn,"writeq")==0||strcmp(fn,"write_canonical")==0||strcmp(fn,"print")==0) && ir_call_arg(bb,0)) {
            extern void pl_writeq(Term *); extern void pl_write_canonical(Term *); extern void pl_write(Term *);
            Term *t = term_deref(resolve_node_to_term(ir_call_arg(bb,0)));
            if (strcmp(fn,"writeq")==0) pl_writeq(t);
            else if (strcmp(fn,"write_canonical")==0) pl_write_canonical(t);
            else pl_write(t);
            IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
        }
        if ((strcmp(fn,"retract")==0||strcmp(fn,"retractall")==0) && ir_call_arg(bb,0)) {
            extern Term **g_resolve_env; extern Trail g_resolve_trail;
            int do_all = (strcmp(fn,"retractall")==0);
            Term *head_t = term_deref(resolve_node_to_term(ir_call_arg(bb,0)));
            if (!head_t) { IR_EXEC(bb).value=(do_all?INTVAL(1):FAILDESCR); return (do_all?bb->γ.node:bb->ω.node); }
            const char *pred_name = NULL; int pred_arity = 0;
            if (head_t->tag==TERM_ATOM) { pred_name=prolog_atom_name(head_t->atom_id); pred_arity=0; }
            else if (head_t->tag==TERM_COMPOUND) { pred_name=prolog_atom_name(head_t->compound.functor); pred_arity=head_t->compound.arity; }
            if (!pred_name) { IR_EXEC(bb).value=(do_all?INTVAL(1):FAILDESCR); return (do_all?bb->γ.node:bb->ω.node); }
            char key[128]; snprintf(key,sizeof key,"%s/%d",pred_name,pred_arity);
            Resolve_PredEntry_BB *entry = resolve_bb_lookup(key, pred_arity);
            IR_graph_t *pred_cfg = entry ? bb_graph_of_pred(entry) : NULL;
            if (!pred_cfg || !pred_cfg->entry) { IR_EXEC(bb).value=(do_all?INTVAL(1):FAILDESCR); return (do_all?bb->γ.node:bb->ω.node); }
            IR_t *choice_nd = pred_cfg->entry;
            if (choice_nd->op != IR_CHOICE) { IR_EXEC(bb).value=FAILDESCR; return bb->ω.node; }
            bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)IR_LIT(choice_nd).ival;
            if (!zc) { IR_EXEC(bb).value=(do_all?INTVAL(1):FAILDESCR); return (do_all?bb->γ.node:bb->ω.node); }
            int removed=0;
            for (int ci=0; ci<zc->nbodies; ) {
                IR_graph_t *body = zc->bodies[ci];
                int nslots = pred_arity + 8;
                Term **clause_env = (Term**)calloc((size_t)nslots, sizeof(Term*));
                for (int ai=0; ai<pred_arity; ai++) {
                    clause_env[ai] = term_new_var(ai);
                    Term *pattern_arg = (head_t->tag==TERM_COMPOUND && ai<head_t->compound.arity) ? head_t->compound.args[ai] : NULL;
                    if (pattern_arg) unify(clause_env[ai], pattern_arg, &g_resolve_trail);
                }
                Term **saved_env = g_resolve_env;
                g_resolve_env = clause_env;
                int mark = trail_mark(&g_resolve_trail);
                bb_node_state_t *snap = bb_snapshot_state(body);
                bb_reset(body);
                DESCR_t res = IR_interp_once(body);
                bb_restore_state(body, snap);
                g_resolve_env = saved_env;
                int matched = !IS_FAIL_fn(res);
                if (matched && !do_all) {
                    free(clause_env);
                    for (int j=ci; j<zc->nbodies-1; j++) zc->bodies[j]=zc->bodies[j+1];
                    zc->nbodies--;
                    removed=1;
                    break;
                }
                trail_unwind(&g_resolve_trail, mark);
                free(clause_env);
                if (matched && do_all) {
                    for (int j=ci; j<zc->nbodies-1; j++) zc->bodies[j]=zc->bodies[j+1];
                    zc->nbodies--;
                    removed++;
                } else ci++;
            }
            if (do_all) { IR_EXEC(bb).value=INTVAL(1); return bb->γ.node; }
            if (removed>0) { IR_EXEC(bb).value=INTVAL(1); return bb->γ.node; }
            IR_EXEC(bb).value=FAILDESCR; return bb->ω.node;
        }
        if ((strcmp(fn,"assertz")==0||strcmp(fn,"asserta")==0||strcmp(fn,"assert")==0) && ir_call_arg(bb,0)) {
            extern int pl_rt_assertz(Term *clause_term, int prepend);
            Term *clause_t = resolve_node_to_term(ir_call_arg(bb,0));
            int prepend = (strcmp(fn,"asserta")==0);
            int ok = clause_t ? pl_rt_assertz(clause_t, prepend) : 0;
            if (ok) { IR_EXEC(bb).value=INTVAL(1); return bb->γ.node; }
            IR_EXEC(bb).value=FAILDESCR; return bb->ω.node;
        }
        if (strcmp(fn,"abolish")==0 && ir_call_arg(bb,0)) {
            Term *spec = term_deref(resolve_node_to_term(ir_call_arg(bb,0)));
            const char *pred_name = NULL; int pred_arity = 0;
            if (spec && spec->tag==TERM_COMPOUND && spec->compound.arity==2
                    && strcmp(prolog_atom_name(spec->compound.functor),"/")==0) {
                Term *nm = term_deref(spec->compound.args[0]);
                Term *ar = term_deref(spec->compound.args[1]);
                if (nm && nm->tag==TERM_ATOM) pred_name=prolog_atom_name(nm->atom_id);
                if (ar && ar->tag==TERM_INT)  pred_arity=(int)ar->ival;
            }
            if (pred_name) {
                char key[128]; snprintf(key,sizeof key,"%s/%d",pred_name,pred_arity);
                Resolve_PredEntry_BB *entry = resolve_bb_lookup(key, pred_arity);
                IR_graph_t *pred_cfg = entry ? bb_graph_of_pred(entry) : NULL;
                if (pred_cfg && pred_cfg->entry && pred_cfg->entry->op==IR_CHOICE) {
                    bb_choice_state_t *zc = (bb_choice_state_t *)(intptr_t)IR_LIT(pred_cfg->entry).ival;
                    if (zc) zc->nbodies = 0;
                }
            }
            IR_EXEC(bb).value=INTVAL(1); return bb->γ.node;
        }
        IR_EXEC(bb).value = FAILDESCR; return bb->ω.node;
    }
    default:
        IR_EXEC(bb).value = FAILDESCR;
        return bb->ω.node;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t IR_interp_once(IR_graph_t * bbg) {
    if (!bbg || !bbg->entry) return FAILDESCR;
    bb_reset(bbg);
    IR_graph_t * saved_cfg = g_current_cfg;
    g_current_cfg = bbg;
    IR_t * cur = bbg->entry;
    int safety = bbg->n * 65536 + 1048576;
    DESCR_t result = FAILDESCR;
    while (cur && safety-- > 0) {
        IR_t * next = IR_interp_node(cur);
        if (g_resolve_tail_redirect_cfg) {
            IR_graph_t * tgt = g_resolve_tail_redirect_cfg;
            IR_t       * te  = g_resolve_tail_redirect_entry;
            g_resolve_tail_redirect_cfg   = NULL;
            g_resolve_tail_redirect_entry = NULL;
            g_current_cfg = tgt;
            bbg = tgt;
            safety = tgt->n * 65536 + 1048576;
            cur = te;
            continue;
        }
        if (frame_depth > 0 && FRAME.returning) {
            result = g_ir_return_val;
            g_current_cfg = saved_cfg;
            return result;
        }
        if (!next) {
            result = IS_FAIL_fn(IR_EXEC(cur).value) ? FAILDESCR : IR_EXEC(cur).value;
            g_current_cfg = saved_cfg;
            return result;
        }
        if (cur->op == IR_ALT && IR_EXEC(cur).counter == 0) { } else ag_ring_push(bbg, IR_EXEC(cur).value);
        cur = next;
    }
    g_current_cfg = saved_cfg;
    return FAILDESCR;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t IR_interp_resume(IR_graph_t * bbg) {
    if (!bbg || !bbg->entry) return FAILDESCR;
    IR_graph_t * saved_cfg = g_current_cfg;
    g_current_cfg = bbg;
    IR_t * cur = bbg->entry;
    int safety = bbg->n * 65536 + 1048576;
    DESCR_t result = FAILDESCR;
    while (cur && safety-- > 0) {
        IR_t * next = IR_interp_node(cur);
        if (g_resolve_tail_redirect_cfg) {
            IR_graph_t * tgt = g_resolve_tail_redirect_cfg;
            IR_t       * te  = g_resolve_tail_redirect_entry;
            g_resolve_tail_redirect_cfg   = NULL;
            g_resolve_tail_redirect_entry = NULL;
            g_current_cfg = tgt;
            bbg = tgt;
            safety = tgt->n * 65536 + 1048576;
            cur = te;
            continue;
        }
        if (frame_depth > 0 && FRAME.returning) {
            result = g_ir_return_val;
            g_current_cfg = saved_cfg;
            return result;
        }
        if (!next) {
            result = IS_FAIL_fn(IR_EXEC(cur).value) ? FAILDESCR : IR_EXEC(cur).value;
            g_current_cfg = saved_cfg;
            return result;
        }
        if (cur->op == IR_ALT && IR_EXEC(cur).counter == 0) { } else ag_ring_push(bbg, IR_EXEC(cur).value);
        cur = next;
    }
    g_current_cfg = saved_cfg;
    return FAILDESCR;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int IR_interp_pump(IR_graph_t * bbg, bb_body_fn body_fn, void * ctx) {
    if (!bbg || !bbg->entry) return 0;
    bb_reset(bbg);
    IR_graph_t * saved_cfg = g_current_cfg;
    g_current_cfg = bbg;
    int ticks  = 0;
    int safety = bbg->n * 256 + 1024;
    IR_t * cur = bbg->entry;
    while (cur && safety-- > 0) {
        IR_t * next = IR_interp_node(cur);
        if (!next) {
            if (!IS_FAIL_fn(IR_EXEC(cur).value)) {
                ag_ring_push(bbg, IR_EXEC(cur).value);
                ticks++;
                if (body_fn && body_fn(IR_EXEC(cur).value, ctx)) break;
                next = ((IR_t*)0);
                if (!next) break;
            } else {
                break;
            }
        } else if (next == cur) {
            continue;
        } else {
            ag_ring_push(bbg, IR_EXEC(cur).value);
        }
        cur = next;
    }
    g_current_cfg = saved_cfg;
    return ticks;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int IR_interp_pat(IR_graph_t *bbg,
                const char *subj_name,
                DESCR_t    *subj_var,
                DESCR_t    *repl,
                int         has_repl)
{
    if (!bbg || !bbg->entry) return 0;
    const char *subj_str = "";
    int         subj_len = 0;
    DESCR_t subj_fetched;
    if (subj_name && *subj_name) {
        subj_fetched = NV_GET_fn(subj_name);
        subj_var     = &subj_fetched;
    }
    if (subj_var) {
        DESCR_t sv = VARVAL_d_fn(*subj_var);
        if (sv.v == DT_S || sv.v == DT_SNUL) {
            subj_str = sv.s ? sv.s : "";
            subj_len = sv.slen ? (int)sv.slen : (int)strlen(subj_str);
        }
    }
    Σ    = subj_str;
    Σlen = subj_len;
    Ω    = subj_len;
    int match_start = -1;
    int match_end   = -1;
    extern int64_t kw_anchor;
    int max_start = kw_anchor ? 0 : Ω;
    g_dcap_active = 1;
    g_dcap_n = 0;
    for (int start = 0; start <= max_start; start++) {
        Δ = start;
        bb_reset(bbg);
        g_dcap_n = 0;
        DESCR_t result = IR_interp_once(bbg);
        if (!IS_FAIL_fn(result)) {
            match_start = start;
            match_end   = Δ;
            break;
        }
    }
    g_dcap_active = 0;
    if (match_start < 0) { bb_dcap_clear(); return 0; }
    bb_dcap_flush();
    if (!has_repl || !repl) return 1;
    if (!subj_name && !subj_var)        return 0;
    const char *repl_str = "";
    int         repl_len = 0;
    if (repl->v == DT_S && repl->s) {
        repl_str = repl->s;
        repl_len = repl->slen ? (int)repl->slen : (int)strlen(repl->s);
    } else if (repl->v == DT_I) {
        char ibuf[32];
        snprintf(ibuf, sizeof(ibuf), "%lld", (long long)repl->i);
        char *gs = (char *)GC_MALLOC(strlen(ibuf) + 1);
        strcpy(gs, ibuf);
        repl_str = gs;
        repl_len = (int)strlen(gs);
    }
    int   new_len = match_start + repl_len + (subj_len - match_end);
    char *new_s   = (char *)GC_MALLOC((size_t)new_len + 1);
    memcpy(new_s,                          subj_str,                (size_t)match_start);
    memcpy(new_s + match_start,            repl_str,                (size_t)repl_len);
    memcpy(new_s + match_start + repl_len, subj_str + match_end,    (size_t)(subj_len - match_end));
    new_s[new_len] = '\0';
    DESCR_t new_val = { .v = DT_S, .slen = (uint32_t)new_len, .s = new_s };
    if (subj_name && *subj_name) {
        NV_SET_fn(subj_name, new_val);
    } else if (subj_var) {
        *subj_var = new_val;
    }
    return 1;
}
/*====================================================================================================================================================================================================*/
DESCR_t rk_ir_call_proc(int upi, DESCR_t *args, int nargs) {
    if (upi < 0 || upi >= g_stage2.proc_count) return FAILDESCR;
    IR_graph_t * fg = bb_graph_of_proc(&g_stage2.proc_table[upi]);
    Scope * sc = &g_stage2.proc_table[upi].lower_sc;
    int np = g_stage2.proc_table[upi].nparams;
    if (!fg || frame_depth >= FRAME_STACK_MAX || g_sno_save_top + sc->n > SNO_SAVE_MAX) return FAILDESCR;
    int save_base = g_sno_save_top;
    for (int k = 0; k < sc->n; k++) {
        const char * nm = sc->e[k].name; if (!nm) continue;
        g_sno_save[g_sno_save_top].name = nm;
        g_sno_save[g_sno_save_top].old  = NV_GET_fn(nm);
        g_sno_save_top++;
        NV_SET_fn(nm, (k < np && k < nargs) ? args[k] : NULVCL);
    }
    const char * saved_func = g_sno_cur_func;
    g_sno_cur_func = g_stage2.proc_table[upi].name;
    GenFrame * _f = &frame_stack[frame_depth++];
    memset(_f, 0, sizeof *_f);
    DESCR_t _ring_save[AG_RING];
    int _ring_head = fg->ring_head, _ring_depth = fg->ring_depth;
    memcpy(_ring_save, fg->ring, sizeof _ring_save);
    bb_node_state_t * _snap = bb_snapshot_state(fg);
    bb_reset(fg);
    DESCR_t out = IR_interp_once(fg);
    if (frame_depth > 0 && FRAME.returning) { out = g_ir_return_val; FRAME.returning = 0; }
    frame_depth--;
    bb_restore_state(fg, _snap);
    memcpy(fg->ring, _ring_save, sizeof _ring_save);
    fg->ring_head = _ring_head; fg->ring_depth = _ring_depth;
    g_sno_cur_func = saved_func;
    for (int k = g_sno_save_top - 1; k >= save_base; k--)
        NV_SET_fn(g_sno_save[k].name, g_sno_save[k].old);
    g_sno_save_top = save_base;
    return out;
}
