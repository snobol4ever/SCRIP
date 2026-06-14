#include "IR_interp_state.h"
#include "rt/rt.h"
#include "lower.h"
#include "../../emitter/sil_macros.h"
#include "../../parser/prolog/term.h"
#include "../../parser/prolog/prolog_runtime.h"
#include "../../parser/prolog/prolog_atom.h"
#include "../../runtime/builtins/resolution.h"
#include "../../parser/raku/re.h"
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
#define SAVE_MAX 4096
typedef struct { const char * name; DESCR_t old; } SaveEnt;
static SaveEnt   g_save_stack[SAVE_MAX];
static int          g_save_stack_top = 0;
static const char * g_cur_func = NULL;
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
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { IR_t * node; DESCR_t * items; int count; int cap; } seq_cache_t;
#define SEQ_CACHE_MAX 64
static seq_cache_t g_seq_cache[SEQ_CACHE_MAX];
static int g_seq_cache_n = 0;
static seq_cache_t * seq_cache_find(IR_t * node) {
    for (int i = 0; i < g_seq_cache_n; i++) if (g_seq_cache[i].node == node) return &g_seq_cache[i];
    return NULL;
}
static seq_cache_t * seq_cache_get(IR_t * node) {
    seq_cache_t * e = seq_cache_find(node);
    if (e) return e;
    if (g_seq_cache_n >= SEQ_CACHE_MAX) { g_seq_cache_n = 0; }
    e = &g_seq_cache[g_seq_cache_n++];
    e->node = node; e->items = NULL; e->count = 0; e->cap = 0;
    return e;
}
static void seq_cache_push(seq_cache_t * e, DESCR_t v) {
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
    case IR_CALL_DEFINE:
    case IR_SCAN_POS: case IR_SCAN_ANY: case IR_SCAN_MATCH: case IR_SCAN_MANY: case IR_SCAN_TAB: case IR_SCAN_MOVE: case IR_SCAN_UPTO: case IR_SCAN_FIND: case IR_SCAN_BAL:
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
static Term *resolve_node_to_term(IR_t *bb) { (void)bb; fprintf(stderr, "[NO-IR-INTERP] resolve_node_to_term: IR interpretation deleted; native Prolog operand resolution pending\n"); return (Term *)0; }
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
static DESCR_t resolve_arith_eval(IR_t *bb) { (void)bb; fprintf(stderr, "[NO-IR-INTERP] resolve_arith_eval: IR interpretation deleted; native Prolog arithmetic pending\n"); return FAILDESCR; }
/*--------------------------------------------------------------------------------------------------------------------*/
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
static int descr_is_truthy(DESCR_t cv) {
    if (IS_FAIL_fn(cv)) return 0;
    if (IS_INT_fn(cv))  return cv.i != 0;
    if (IS_REAL_fn(cv)) return cv.r != 0.0;
    if (cv.v == DT_SNUL) return 0;
    const char *s = cv.s ? cv.s : "";
    return s[0] != '\0' && !(s[0] == '0' && s[1] == '\0');
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t ir_call_proc(int upi, DESCR_t *args, int nargs) { (void)upi; (void)args; (void)nargs; fprintf(stderr, "[NO-IR-INTERP] ir_call_proc: IR interpreter deleted (walked IR via IR_interp_pump); native BB proc-call pending\n"); return FAILDESCR; }
