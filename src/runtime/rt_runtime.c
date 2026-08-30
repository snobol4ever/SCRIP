#include "rt/rt_arena.h"
#include "emit.h"
#include "rt/rt.h"
#include "lower.h"
#include "../../emitter/sil_macros.h"
#include "../../parsers/prolog/term.h"
#include "../../parsers/prolog/prolog_runtime.h"
#include "../../parsers/prolog/prolog_atom.h"
#include "../../runtime/builtins/resolution.h"
#include "../../parsers/raku/re.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <setjmp.h>
#include "../../parsers/prolog/pl_cell.h"
#define PL_CELL_ALLOC(n) rt_ws_alloc(n)
#include "../../parsers/prolog/pl_cell_conv.h"
#include "../ir/dtp.h"
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
IR_graph_t * g_current_cfg = NULL;
static IR_graph_t * g_resolve_tail_redirect_cfg   = NULL;
static IR_t       * g_resolve_tail_redirect_entry = NULL;
int g_resolve_b3_call_mark = -1;
typedef struct { IR_t * node; DESCR_t * items; int count; int cap; } seq_cache_t;
#define SEQ_CACHE_MAX 64
static seq_cache_t g_seq_cache[SEQ_CACHE_MAX];
static int g_seq_cache_n = 0;
typedef struct { IR_t * node; DESCR_t * items; int count; } susp_gen_cache_t;
#define SUSP_GEN_CACHE_MAX 64
static susp_gen_cache_t g_susp_gen_cache[SUSP_GEN_CACHE_MAX];
static int g_susp_gen_cache_n = 0;
extern int rt_scan_exec(const char *subj_name, const char *subj_lit, int has_repl, const char *repl_str, void *pat_graph);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
        char * new_s = (char *)rt_ws_alloc((size_t)new_len + 1);
        memcpy(new_s, subj_str, (size_t)m_start);
        memcpy(new_s + m_start, repl, (size_t)repl_len);
        memcpy(new_s + m_start + repl_len, subj_str + m_end, (size_t)(subj_len - m_end));
        new_s[new_len] = '\0';
        DESCR_t nv = { .v = DT_S, .slen = (uint32_t)new_len, .s = new_s };
        NV_SET_fn(subj_name, nv);
    }
    return matched ? 1 : 0;
}
static int bb_is_gen_node(IR_t * e);
static void resolve_format_float(char *buf, size_t bufsz, double d);
typedef struct { Term *orig; Term *copy; } BBCopyMap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
        Term **args=(Term**)rt_ws_alloc((size_t)ar*sizeof(Term*));
        for (int i=0;i<ar;i++) args[i]=bb_copy_term_rec(t->compound.args[i],map,nmap);
        return term_new_compound(t->compound.functor,ar,args);
    }
    default: return term_new_atom(prolog_atom_intern("[]"));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *bb_copy_term(Term *t) { BBCopyMap map[256]; int n=0; return bb_copy_term_rec(t,map,&n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resolve_term_class(Term *t) {
    switch (t->tag) {
    case TERM_VAR: return 0;
    case TERM_FLOAT: case TERM_INT: return 1;
    case TERM_ATOM: return 2;
    case TERM_COMPOUND: return 3;
    default: return 4;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
static int bb_body_has_live_choice(IR_graph_t *bbg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_type_test_term(const char *fn, void *t0) {
    return type_test_common(fn, (Term *)t0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_int(void *lhs_cell, long val) {
    extern pl_trail_t g_pl_trail;
    pl_cell_t *lhs = (pl_cell_t *)lhs_cell;
    if (!lhs) return 0;
    pl_cell_t w = pl_make_int((int64_t)val);
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify(lhs, &w, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_float(void *lhs_cell, double val) {
    extern pl_trail_t g_pl_trail;
    pl_cell_t *lhs = (pl_cell_t *)lhs_cell;
    if (!lhs) return 0;
    pl_cell_t w = pl_make_float(val);
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify(lhs, &w, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_arith_cmp_cell_val(const char *op, void *lhs_cell, long lhs_ival, void *rhs_cell, long rhs_ival) {
    if (!op) return 0;
    int li_int = 1, ri_int = 1; long la = lhs_ival, ra = rhs_ival; double l = 0.0, r = 0.0;
    if (lhs_cell) {
        pl_cell_t *t = pl_deref((pl_cell_t *)lhs_cell);
        if ((int)t->v == DT_I) { la = (long)t->i; } else if ((int)t->v == DT_R) { l = t->r; li_int = 0; } else return 0;
    }
    if (rhs_cell) {
        pl_cell_t *t = pl_deref((pl_cell_t *)rhs_cell);
        if ((int)t->v == DT_I) { ra = (long)t->i; } else if ((int)t->v == DT_R) { r = t->r; ri_int = 0; } else return 0;
    }
    char c0 = op[0], c1 = op[1];
    int cmp;
    if (c0 == '<') cmp = (c1 == '=') ? -1 : -2;
    else if (c0 == '>') cmp = (c1 == '=') ? 1 : 2;
    else if (c0 == '=') { if (c1 == '<') cmp = -1; else if (c1 == ':') cmp = 0; else if (c1 == '\\') cmp = 3; else return 0; }
    else return 0;
    if (li_int && ri_int) {
        switch (cmp) { case -2: return la< ra; case -1: return la<=ra; case 0: return la==ra; case 1: return la>=ra; case 2: return la> ra; case 3: return la!=ra; }
    }
    if (li_int) l = (double)la; if (ri_int) r = (double)ra;
    switch (cmp) { case -2: return l< r; case -1: return l<=r; case 0: return l==r; case 1: return l>=r; case 2: return l> r; case 3: return l!=r; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_arith(void *lhs_cell, void *rhs_cell, const char *op, long rhs_ival) {
    extern pl_trail_t g_pl_trail;
    pl_cell_t *lhs = (pl_cell_t *)lhs_cell;
    if (!lhs) return 0;
    pl_cell_t w;
    if (rhs_cell) {
        pl_cell_t *t = pl_deref((pl_cell_t *)rhs_cell);
        char oc = op ? op[0] : 0; char oc2 = op ? op[1] : 0;
        if ((int)t->v == DT_I) {
            long lv = (long)t->i, res;
            switch (oc) {
            case 0:   res = lv; break;
            case '+': res = lv + rhs_ival; break;
            case '-': res = lv - rhs_ival; break;
            case '*': res = lv * rhs_ival; break;
            case '/':
                if (oc2 == '/') { if (!rhs_ival) return 0; res = lv / rhs_ival; }
                else { if (!rhs_ival) return 0; double q = (double)lv / (double)rhs_ival; w = ((double)(long)q == q) ? pl_make_int((int64_t)(long)q) : pl_make_float(q); goto bind; }
                break;
            case 'm': if (!rhs_ival) return 0; res = lv % rhs_ival; break;
            case 'r': if (!rhs_ival) return 0; res = lv % rhs_ival; break;
            case 'd': if (!rhs_ival) return 0; res = lv / rhs_ival; break;
            default:  return 0;
            }
            w = pl_make_int((int64_t)res);
        } else if ((int)t->v == DT_R) {
            double rv = t->r;
            switch (oc) {
            case 0:   break;
            case '+': rv = rv + (double)rhs_ival; break;
            case '-': rv = rv - (double)rhs_ival; break;
            case '*': rv = rv * (double)rhs_ival; break;
            case '/': if (!rhs_ival) return 0; rv = rv / (double)rhs_ival; break;
            case 'm': case 'r': { long li=(long)rv; if (!rhs_ival) return 0; rv=(double)(li%rhs_ival); } break;
            case 'd': { long li=(long)rv; if (!rhs_ival) return 0; rv=(double)(li/rhs_ival); } break;
            default:  return 0;
            }
            long iv = (long)rv;
            w = ((double)iv == rv) ? pl_make_int((int64_t)iv) : pl_make_float(rv);
        } else return 0;
    } else { w = pl_make_int((int64_t)rhs_ival); }
bind:;
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify(lhs, &w, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_bivar(void *lhs_cell, void *cell1, void *cell2, const char *op) {
    extern pl_trail_t g_pl_trail;
    pl_cell_t *lhs = (pl_cell_t *)lhs_cell;
    if (!lhs || !cell1 || !cell2) return 0;
    pl_cell_t *t1 = pl_deref((pl_cell_t *)cell1), *t2 = pl_deref((pl_cell_t *)cell2);
    char oc = op ? op[0] : 0; char oc2 = op ? op[1] : 0;
    pl_cell_t w;
    if ((int)t1->v == DT_I && (int)t2->v == DT_I) {
        long a = (long)t1->i, b = (long)t2->i, res;
        switch (oc) {
        case 0:   res = a + b; break;
        case '+': res = a + b; break;
        case '-': res = a - b; break;
        case '*': res = a * b; break;
        case '/':
            if (oc2 == '/') { if (!b) return 0; res = a / b; }
            else { if (!b) return 0; double q=(double)a/(double)b; w = ((double)(long)q==q)?pl_make_int((int64_t)(long)q):pl_make_float(q); goto bind; }
            break;
        case 'm': case 'r': if (!b) return 0; res = a % b; break;
        case 'd': if (!b) return 0; res = a / b; break;
        default:  return 0;
        }
        w = pl_make_int((int64_t)res);
    } else {
        double a = ((int)t1->v == DT_I) ? (double)t1->i : ((int)t1->v == DT_R) ? t1->r : -1e300;
        double b = ((int)t2->v == DT_I) ? (double)t2->i : ((int)t2->v == DT_R) ? t2->r : -1e300;
        if (a == -1e300 || b == -1e300) return 0;
        double rv;
        switch (oc) {
        case 0: case '+': rv = a + b; break;
        case '-': rv = a - b; break;
        case '*': rv = a * b; break;
        case '/': if (!b) return 0; rv = a / b; break;
        case 'm': case 'r': { long la=(long)a,lb=(long)b; if (!lb) return 0; rv=(double)(la%lb); } break;
        case 'd': { long la=(long)a,lb=(long)b; if (!lb) return 0; rv=(double)(la/lb); } break;
        default:  return 0;
        }
        long iv = (long)rv;
        w = ((double)iv == rv) ? pl_make_int((int64_t)iv) : pl_make_float(rv);
    }
bind:;
    int mark = pl_trail_mark(&g_pl_trail);
    if (!pl_unify(lhs, &w, &g_pl_trail)) { pl_trail_unwind(&g_pl_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_compound_build_n(const char *functor_name, int arity, void *args_ptr) {
    Term **args_in = (Term **)args_ptr;
    Term **args = (Term **)rt_ws_alloc(arity * sizeof(Term *));
    for (int i = 0; i < arity; i++) args[i] = args_in[i];
    int fid = prolog_atom_intern(functor_name ? functor_name : "");
    return term_new_compound(fid, arity, args);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *rt_cmp_cell_to_term_shared(pl_cell_t *c, pl_cell_t **vaddr, Term **vterm, int *vn, int cap) {
    while (c && (int)c->v == DT_N && c->slen == 2 && c->p && ((VCELL_t *)c->p)->cellp) c = (pl_cell_t *)((VCELL_t *)c->p)->cellp;
    pl_cell_t *d = pl_deref(c);
    int t = (int)d->v;
    if (t == DT_PLVAR) {
        for (int i = 0; i < *vn; i++) if (vaddr[i] == d) return vterm[i];
        Term *fresh = term_new_var((int)d->slen);
        if (*vn < cap) { vaddr[*vn] = d; vterm[*vn] = fresh; (*vn)++; }
        return fresh;
    }
    if (t == DT_I) return term_new_int((long)d->i);
    if (t == DT_A) return term_new_atom((int)d->i);
    if (t == DT_R) return term_new_float(d->r);
    if (t == DT_S || t == DT_SNUL) return term_new_atom(prolog_atom_intern(d->s ? d->s : ""));
    if (t == DT_PLREF) {
        int fn = (int)(d->slen >> 16), ar = (int)(d->slen & 0xFFFFu);
        pl_cell_t *aa = (pl_cell_t *)d->p;
        Term **args = (Term **)rt_ws_alloc((size_t)(ar > 0 ? ar : 1) * sizeof(Term *));
        for (int i = 0; i < ar; i++) args[i] = rt_cmp_cell_to_term_shared(&aa[i], vaddr, vterm, vn, cap);
        return term_new_compound(fn, ar, args);
    }
    return term_new_var(-1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Term *rt_pl_cell_to_term(void *cell) {
    { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); }
    pl_cell_t *vaddr[256]; Term *vterm[256]; int vn = 0;
    return rt_cmp_cell_to_term_shared((pl_cell_t *)cell, vaddr, vterm, &vn, 256);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Term *rt_pl_cell_to_term_named(void *cell) {
    { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); }
    return pl_cell_to_term_named((pl_cell_t *)cell);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_term_cmp_terms(const char *op, void *t0, void *t1) {
    if (!op) return 0;
    pl_cell_t *vaddr[256]; Term *vterm[256]; int vn = 0;
    arena_mark_t cm = rt_pl_cterm_mark();
    Term *T0 = rt_cmp_cell_to_term_shared((pl_cell_t *)t0, vaddr, vterm, &vn, 256);
    Term *T1 = rt_cmp_cell_to_term_shared((pl_cell_t *)t1, vaddr, vterm, &vn, 256);
    int c = resolve_term_compare(T0, T1);
    if (rt_pl_ctr_on()) rt_pl_cterm_release(cm);
    if (strcmp(op, "==")   == 0) return (c == 0) ? 1 : 0;
    if (strcmp(op, "\\==") == 0) return (c != 0) ? 1 : 0;
    if (strcmp(op, "@<")   == 0) return (c <  0) ? 1 : 0;
    if (strcmp(op, "@>")   == 0) return (c >  0) ? 1 : 0;
    if (strcmp(op, "@=<")  == 0) return (c <= 0) ? 1 : 0;
    if (strcmp(op, "@>=")  == 0) return (c >= 0) ? 1 : 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
                Term **c = (Term **)rt_ws_alloc(2 * sizeof(Term *));
                c[0] = d0->compound.args[i]; c[1] = lst;
                lst = term_new_compound(ATOM_DOT, 2, c);
            }
            Term **c = (Term **)rt_ws_alloc(2 * sizeof(Term *));
            c[0] = term_new_atom(d0->compound.functor); c[1] = lst;
            lst = term_new_compound(ATOM_DOT, 2, c);
        } else {
            Term **c = (Term **)rt_ws_alloc(2 * sizeof(Term *));
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
        Term **args = (Term **)rt_ws_alloc((size_t)(ne - 1) * sizeof(Term *));
        for (int i = 1; i < ne; i++) args[i - 1] = elems[i];
        built = term_new_compound(h->atom_id, ne - 1, args);
    }
    if (!unify(t0, built, &g_resolve_trail)) { trail_unwind(&g_resolve_trail, mark); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_univ_term_term(void *t0, void *t1) {
    return univ_common((Term *)t0, (Term *)t1);
}
static long g_pl_yield_seq = 1;
typedef struct { Term **callee_env; Term **saved_env; int trail_mark; int nslots;
                 bb_node_state_t *act; void *cp_floor; int disj_hint; } PlCallSt;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    if (obj.v == DT_FH) {
        extern FILE *fh_get(int);
        FILE *fp = fh_get((int)obj.i);
        if (!fp) return 0;
        char buf[4096];
        if (!fgets(buf, sizeof buf, fp)) return 0;
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0';
        char *cp = rt_ws_alloc(len + 1); memcpy(cp, buf, len + 1);
        *out = (DESCR_t){ .v = DT_S, .slen = (uint32_t)len, .s = cp };
        return 1;
    }
    if (obj.v == DT_T && obj.tbl) {
        TBBLK_t *tbl   = obj.tbl;
        int64_t  seen  = 0; TBPAIR_t *ep;
        TBL_FOREACH(tbl, ep) { if (seen == idx) { *out = ep->val; return 1; } seen++; }
        return 0;
    }
    {
        DESCR_t     sobj = (obj.v == DT_S) ? obj : descr_to_str_fracdigit(obj);
        const char *s    = (sobj.v == DT_S) ? sobj.s : NULL;
        int64_t     slen = !s ? 0 : (IS_CSET_fn(sobj) ? (int64_t)strlen(s) : (int64_t)(sobj.slen > 0 ? sobj.slen : strlen(s)));
        if (!s || idx >= slen) return 0;
        char *ch = rt_ws_alloc(2);
        ch[0] = s[idx];
        ch[1] = '\0';
        *out = (DESCR_t){ .v = DT_S, .slen = 1, .s = ch };
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int list_bang_key_at(DESCR_t obj, int64_t idx, DESCR_t * out) {
    if (obj.v == DT_T && obj.tbl) {
        TBBLK_t *tbl  = obj.tbl;
        int64_t  seen = 0; TBPAIR_t *ep;
        TBL_FOREACH(tbl, ep) {
            {
                if (seen == idx) { *out = ep->key_descr; return 1; }
                seen++;
            }
        }
        return 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t ir_call_proc(int upi, DESCR_t *args, int nargs) {
    (void)upi; (void)args; (void)nargs;
    fprintf(stderr, "[NO-IR-INTERP] ir_call_proc: IR interpreter deleted (walked IR via IR_interp_pump); native BB proc-call pending\n"); return FAILDESCR;
}
extern const char *Σ;
extern int Σlen;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_scan_splice_empty(const char *subj_name, int m_start, int m_end)
{
    if (!subj_name || !subj_name[0]) return;
    DESCR_t sv = VARVAL_d_fn(NV_GET_fn(subj_name));
    const char *s = ""; int slen = 0;
    if (sv.v == DT_S || sv.v == DT_SNUL) { s = sv.s ? sv.s : ""; slen = sv.slen ? (int)sv.slen : (int)strlen(s); }
    if (m_start < 0 || m_end < m_start || m_end > slen) return;
    int new_len = m_start + (slen - m_end);
    char *ns = (char *)rt_ws_alloc((size_t)new_len + 1);
    if (!ns) return;
    if (m_start > 0) memcpy(ns, s, (size_t)m_start);
    if (slen - m_end > 0) memcpy(ns + m_start, s + m_end, (size_t)(slen - m_end));
    ns[new_len] = '\0';
    DESCR_t nv = { .v = DT_S, .slen = (uint32_t)new_len, .s = ns };
    NV_SET_fn(subj_name, nv);
}
