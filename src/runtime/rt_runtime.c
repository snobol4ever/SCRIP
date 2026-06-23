#include "box_state.h"
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
typedef struct { DESCR_t * items; int count; } SuspendList;
#define SAVE_MAX 4096
typedef struct { const char * name; DESCR_t old; } SaveEnt;
static SaveEnt   g_save_stack[SAVE_MAX];
static int          g_save_stack_top = 0;
static const char * g_cur_func = NULL;
#define RESOLVE_NB_SIZE 64
typedef struct { int atom_id; Term *val; } PlNbSlot;
static PlNbSlot g_resolve_nb[RESOLVE_NB_SIZE];
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
typedef struct { IR_t * node; DESCR_t * items; int count; } susp_gen_cache_t;
#define SUSP_GEN_CACHE_MAX 64
static susp_gen_cache_t g_susp_gen_cache[SUSP_GEN_CACHE_MAX];
static int g_susp_gen_cache_n = 0;
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
static int bb_is_gen_node(IR_t * e);
static void resolve_format_float(char *buf, size_t bufsz, double d);
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
extern void *rt_node_to_term(int kind, long ival, const char *sval, double dval);
/*--------------------------------------------------------------------------------------------------------------------*/
extern void pl_write(Term *);
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
static int bb_body_has_live_choice(IR_graph_t *bbg);
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
int rt_type_test_term(const char *fn, void *t0) {
    return type_test_common(fn, (Term *)t0);
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
int rt_univ_term_term(void *t0, void *t1) {
    return univ_common((Term *)t0, (Term *)t1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static long g_pl_yield_seq = 1;
typedef struct { Term **callee_env; Term **saved_env; int trail_mark; int nslots;
                 bb_node_state_t *act; void *cp_floor; int disj_hint; } PlCallSt;
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
DESCR_t ir_call_proc(int upi, DESCR_t *args, int nargs) { (void)upi; (void)args; (void)nargs; fprintf(stderr, "[NO-IR-INTERP] ir_call_proc: IR interpreter deleted (walked IR via IR_interp_pump); native BB proc-call pending\n"); return FAILDESCR; }
/*--------------------------------------------------------------------------------------------------------------------*/
extern const char *Σ;
extern int Σlen;
void rt_scan_splice_empty(const char *subj_name, int m_start, int m_end)
{
    if (!subj_name || !subj_name[0]) return;
    DESCR_t sv = VARVAL_d_fn(NV_GET_fn(subj_name));
    const char *s = ""; int slen = 0;
    if (sv.v == DT_S || sv.v == DT_SNUL) { s = sv.s ? sv.s : ""; slen = sv.slen ? (int)sv.slen : (int)strlen(s); }
    if (m_start < 0 || m_end < m_start || m_end > slen) return;
    int new_len = m_start + (slen - m_end);
    char *ns = (char *)GC_MALLOC((size_t)new_len + 1);
    if (!ns) return;
    if (m_start > 0) memcpy(ns, s, (size_t)m_start);
    if (slen - m_end > 0) memcpy(ns + m_start, s + m_end, (size_t)(slen - m_end));
    ns[new_len] = '\0';
    DESCR_t nv = { .v = DT_S, .slen = (uint32_t)new_len, .s = ns };
    NV_SET_fn(subj_name, nv);
}
