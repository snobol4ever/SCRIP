#include "rt/rt_arena.h"
#include "emit.h"
#include "rt/rt.h"
#include "lower.h"
#include "../../emitter/sil_macros.h"
#include "../../parsers/prolog/prolog_atom.h"
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
#include "bb_box.h"
DESCR_t binop_apply(BinopKind op, DESCR_t lv, DESCR_t rv, int *rel_fail);
static DESCR_t g_ir_return_val;
typedef struct { DESCR_t * items; int count; int cap; int active; } SuspendBuf;
static SuspendBuf g_suspend_buf;
typedef struct { DESCR_t * items; int count; } SuspendList;
#define SAVE_MAX 4096
typedef struct { const char * name; DESCR_t old; } SaveEnt;
static SaveEnt   g_save_stack[SAVE_MAX];
static int          g_save_stack_top = 0;
static const char * g_cur_func = NULL;
IR_graph_t * g_current_cfg = NULL;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bb_body_has_live_choice(IR_graph_t *bbg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_int(void *lhs_cell, long val) {
    pl_cell_t *lhs = (pl_cell_t *)lhs_cell;
    if (!lhs) return 0;
    pl_cell_t w = pl_make_int((int64_t)val);
    if (!pl_unify(lhs, &w)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_float(void *lhs_cell, double val) {
    pl_cell_t *lhs = (pl_cell_t *)lhs_cell;
    if (!lhs) return 0;
    pl_cell_t w = pl_make_float(val);
    if (!pl_unify(lhs, &w)) return 0;
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
    if (!pl_unify(lhs, &w)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_is_cell_bivar(void *lhs_cell, void *cell1, void *cell2, const char *op) {
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
    if (!pl_unify(lhs, &w)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
