#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dtp.h"
#include "core.h"
#include "ast.h"
#include "../parser/snobol4/scrip_cc.h"
#include "sil_macros.h"
#include "builtins/gen_runtime.h"
#define STACKLESS_ABORT(fn) \
    do { fprintf(stderr, "libscrip_rt: %s called — Icon value stack removed (GROUND ZERO 3). " \
                         "This box must be rebuilt stackless (per-box slot, no value stack).\n", (fn)); \
         abort(); } while (0)
DESCR_t (*g_eval_str_hook)(const char *s) = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_lit(const char *s) {
    fprintf(stderr, "[B0] BOMB pat_lit: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_span(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_span: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_break_(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_break_: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_breakx(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_breakx: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_any_cs(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_any_cs: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_notany(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_notany: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_len(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_len: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_pos(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_pos: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rpos(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_rpos: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_tab(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_tab: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rtab(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_rtab: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_arb(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_arbno(DESCR_t inner) {
    fprintf(stderr, "[B0] BOMB pat_arbno: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rem(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fence_p(DESCR_t inner) {
    fprintf(stderr, "[B0] BOMB pat_fence_p: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fence(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fail(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_abort(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_succeed(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_bal(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_epsilon(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_cat(DESCR_t left, DESCR_t right) {
    fprintf(stderr, "[B0] BOMB pat_cat: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_alt(DESCR_t left, DESCR_t right) {
    fprintf(stderr, "[B0] BOMB pat_alt: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx) {
    if (arr.v == DT_A) {
        return array_get(arr.arr, (int)to_int(idx));
    }
    if (arr.v == DT_T) {
        char kb[64]; const char *ks;
        if (IS_INT_fn(idx))       { snprintf(kb,sizeof kb,"%lld",(long long)idx.i); ks=kb; }
        else if (IS_REAL_fn(idx)) { snprintf(kb,sizeof kb,"%g",idx.r); ks=kb; }
        else                      { ks = VARVAL_fn(idx); if (!ks) ks=""; }
        int found; DESCR_t hit = table_get_found(arr.tbl, ks, &found);
        if (found) return hit;
        if (arr.tbl->dflt.v != DT_FAIL && arr.tbl->dflt.v != 0)
            return arr.tbl->dflt;
        return NULVCL;
    }
    if (arr.v == DT_I) {
        char ibuf[32]; snprintf(ibuf, sizeof ibuf, "%lld", (long long)arr.i);
        arr = STRVAL(GC_strdup(ibuf));
    }
    if (arr.v == DT_S || arr.v == DT_SNUL) {
        const char *s = arr.s ? arr.s : "";
        int slen = (int)strlen(s);
        int i = (int)to_int(idx);
        if (i < 0) i = slen + i + 1;
        if (i < 1 || i > slen) return FAILDESCR;
        char *buf = GC_malloc(2); buf[0] = s[i-1]; buf[1] = '\0';
        return STRVAL(buf);
    }
    if (arr.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(arr, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s,"list")==0) {
            int n = (int)FIELD_GET_fn(arr,"frame_size").i;
            DESCR_t ea = FIELD_GET_fn(arr,"frame_elems");
            DESCR_t *elems = (ea.v==DT_DATA) ? (DESCR_t*)ea.ptr : NULL;
            int i = (int)to_int(idx);
            if (i < 0) i = n + i + 1;
            if (!elems || i < 1 || i > n) return FAILDESCR;
            return elems[i-1];
        }
        if (arr.u && arr.u->type && arr.u->type->nfields > 0 && arr.u->fields) {
            DATBLK_t *blk = arr.u->type;
            if (IS_INT_fn(idx)) {
                int i = (int)idx.i;
                if (i < 1 || i > blk->nfields) return FAILDESCR;
                return arr.u->fields[i-1];
            }
            if (idx.v == DT_S || idx.v == DT_SNUL) {
                const char *k = idx.s ? idx.s : "";
                for (int i = 0; i < blk->nfields; i++)
                    if (blk->fields[i] && strcmp(blk->fields[i], k) == 0)
                        return arr.u->fields[i];
                return FAILDESCR;
            }
        }
        int i = (int)to_int(idx);
        DESCR_t children = FIELD_GET_fn(arr, "c");
        if (children.v == DT_A && children.arr)
            return array_get(children.arr, i);
        return FAILDESCR;
    }
    core_runtime_error(3, NULL);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val) {
    if (arr.v == DT_A) {
        int i = (int)to_int(idx);
        if (i < arr.arr->lo || i > arr.arr->hi) return 0;
        array_set(arr.arr, i, val);
        return 1;
    }
    if (arr.v == DT_T) {
        const char *k = VARVAL_fn(idx);
        table_set_descr(arr.tbl, k ? k : "", idx, val);
        return 1;
    }
    if (arr.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(arr, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
            int n = (int)FIELD_GET_fn(arr, "frame_size").i;
            DESCR_t ea = FIELD_GET_fn(arr, "frame_elems");
            DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
            int i = (int)to_int(idx);
            if (i < 0) i = n + i + 1;
            if (!elems || i < 1 || i > n) return 0;
            elems[i - 1] = val;
            return 1;
        }
        if (arr.u && arr.u->type && arr.u->fields) {
            DATBLK_t *blk = arr.u->type;
            if (IS_INT_fn(idx)) {
                int i = (int)idx.i;
                if (i < 1 || i > blk->nfields) return 0;
                arr.u->fields[i - 1] = val;
                return 1;
            }
            if (idx.v == DT_S || idx.v == DT_SNUL) {
                const char *k = idx.s ? idx.s : "";
                for (int i = 0; i < blk->nfields; i++)
                    if (blk->fields[i] && strcmp(blk->fields[i], k) == 0) {
                        arr.u->fields[i] = val;
                        return 1;
                    }
                return 0;
            }
        }
        return 0;
    }
    if (arr.v == DT_S && arr.s) {
        int slen = (int)strlen(arr.s);
        int i = (int)to_int(idx);
        if (i < 0) i = slen + 1 + i;
        if (i < 1 || i > slen) { core_runtime_error(3, NULL); return 0; }
        const char *vs = VARVAL_fn(val);
        if (!vs) vs = "";
        int vlen = (int)strlen(vs);
        int newlen = slen - 1 + vlen;
        char *ns = GC_malloc(newlen + 1);
        memcpy(ns, arr.s, i - 1);
        memcpy(ns + i - 1, vs, vlen);
        memcpy(ns + i - 1 + vlen, arr.s + i, slen - i + 1);
        char *live = (char *)arr.s;
        if (vlen == 1) {
            live[i - 1] = vs[0];
        } else {
            memmove(live + i - 1 + vlen, live + i, slen - i + 1);
            memcpy(live + i - 1, vs, vlen);
        }
        return 1;
    }
    core_runtime_error(3, NULL);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j) {
    if (arr.v == DT_A)
        return array_get2(arr.arr, (int)to_int(i), (int)to_int(j));
    if (arr.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(arr, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s,"list")==0) {
            int n = (int)FIELD_GET_fn(arr,"frame_size").i;
            DESCR_t ea = FIELD_GET_fn(arr,"frame_elems");
            DESCR_t *elems = (ea.v==DT_DATA) ? (DESCR_t*)ea.ptr : NULL;
            int ii = (int)to_int(i), jj = (int)to_int(j);
            if (ii == 0) ii = n + 1; else if (ii < 0) ii = n + ii + 1;
            if (jj == 0) jj = n + 1; else if (jj < 0) jj = n + jj + 1;
            if (ii < 1) ii = 1; if (jj > n+1) jj = n+1;
            if (ii > jj) { int t = ii; ii = jj; jj = t; }
            int rlen = jj - ii;
            if (rlen <= 0) {
                static int list_empty_reg = 0;
                if (!list_empty_reg) { DEFDAT_fn("list(frame_elems,frame_size,gen_type)"); list_empty_reg=1; }
                DESCR_t empty_ptr; empty_ptr.v=DT_DATA; empty_ptr.slen=0; empty_ptr.ptr=NULL;
                return DATCON_fn("list", empty_ptr, INTVAL(0), STRVAL("list"));
            }
            DESCR_t *rbuf = GC_malloc(rlen * sizeof(DESCR_t));
            for (int k = 0; k < rlen; k++) rbuf[k] = (elems && ii+k-1 >= 0 && ii+k-1 < n) ? elems[ii+k-1] : NULVCL;
            DESCR_t rptr; rptr.v=DT_DATA; rptr.slen=0; rptr.ptr=(void*)rbuf;
            static int list_slice_reg = 0;
            if (!list_slice_reg) { DEFDAT_fn("list(frame_elems,frame_size,gen_type)"); list_slice_reg=1; }
            return DATCON_fn("list", rptr, INTVAL(rlen), STRVAL("list"));
        }
    }
    if (arr.v == DT_S || arr.v == DT_SNUL) {
        const char *s = arr.s ? arr.s : "";
        int slen = (int)strlen(s);
        int ii = (int)to_int(i), jj = (int)to_int(j);
        if (ii < 0) ii = slen + 1 + ii + 1;
        if (jj < 0) jj = slen + 1 + jj + 1;
        if (ii < 1) ii = 1; if (jj > slen+1) jj = slen+1;
        if (ii > jj) { int t = ii; ii = jj; jj = t; }
        int len = jj - ii;
        char *buf = GC_malloc(len+1); memcpy(buf, s+ii-1, len); buf[len]='\0';
        return STRVAL(buf);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val) {
    if (arr.v == DT_A) {
        int ii = (int)to_int(i), jj = (int)to_int(j);
        if (ii < arr.arr->lo || ii > arr.arr->hi) return 0;
        if (arr.arr->ndim >= 2 && (jj < arr.arr->lo2 || jj > arr.arr->hi2)) return 0;
        array_set2(arr.arr, ii, jj, val);
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void register_fn(const char *name, DESCR_t (*fn)(DESCR_t*, int), int min_args, int max_args) {
    (void)min_args; (void)max_args;
    DEFINE_fn(name, fn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t EVAL_fn(DESCR_t expr) {
    if (expr.v == DT_E) {
        return EXPVAL_fn(expr);
    }
    if (expr.v == DT_I) return expr;
    if (expr.v == DT_R) return expr;
    if (expr.v == DT_P) {
        if (g_eval_pat_hook) return g_eval_pat_hook(expr);
        return expr;
    }
    const char *s = VARVAL_fn(expr);
    if (!s || !*s) return NULVCL;
    {
        char *endp = NULL;
        int64_t iv = (int64_t)strtoll(s, &endp, 10);
        if (endp && *endp == '\0') return INTVAL(iv);
    }
    {
        char *endp = NULL;
        double rv = strtod(s, &endp);
        if (endp && *endp == '\0') return REALVAL(rv);
    }
    if (g_eval_str_hook) return g_eval_str_hook(s);
    extern DESCR_t eval_string_transient(const char *s);
    return eval_string_transient(s);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t opsyn(DESCR_t newname, DESCR_t oldname, DESCR_t type) {
    (void)type;
    const char *nm  = VARVAL_fn(newname);
    const char *old = NULL;
    if (oldname.v == DT_N) {
        if (oldname.slen == 0 && oldname.s && *oldname.s)
            old = oldname.s;
        else if (oldname.slen == 1 && oldname.ptr)
            old = NV_name_from_ptr((const DESCR_t *)oldname.ptr);
    }
    if (!old) old = VARVAL_fn(oldname);
    if (!nm || !old || !*old) return FAILDESCR;
    register_fn_alias(nm, old);
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int _sort_type_rank(DESCR_t d) {
    switch (d.v) {
        case DT_A: return 0;
        case DT_C: return 1;
        case DT_E: return 2;
        case DT_I: return 3;
        case DT_P: return 6;
        case DT_R: return 7;
        case DT_S: return 8;
        case DT_SNUL: return 8;
        case DT_T: return 9;
        default:   return 5;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int _sort_cmp_descr(DESCR_t a, DESCR_t b, const char *sa, const char *sb) {
    if (a.v == DT_I && b.v == DT_I) {
        if (a.i < b.i) return -1;
        if (a.i > b.i) return  1;
        return 0;
    }
    if ((a.v == DT_S || a.v == DT_SNUL) && (b.v == DT_S || b.v == DT_SNUL)) {
        return strcmp(sa ? sa : "", sb ? sb : "");
    }
    int ra = _sort_type_rank(a), rb = _sort_type_rank(b);
    if (ra != rb) return ra - rb;
    return strcmp(sa ? sa : "", sb ? sb : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t sort_fn(DESCR_t arr) {
    if (arr.v != DT_T) return arr;
    TBBLK_t *tbl = arr.tbl;
    if (!tbl) return FAILDESCR;
    int n = 0;
    for (int h = 0; h < TABLE_BUCKETS; h++)
        for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next) n++;
    if (n == 0) return FAILDESCR;
    const char **keys = GC_malloc(n * sizeof(char *));
    DESCR_t *key_descrs = GC_malloc(n * sizeof(DESCR_t));
    DESCR_t *vals = GC_malloc(n * sizeof(DESCR_t));
    int idx = 0;
    for (int h = 0; h < TABLE_BUCKETS; h++)
        for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next) {
            keys[idx] = e->key;
            key_descrs[idx] = e->key_descr;
            vals[idx] = e->val;
            idx++;
        }
    int *order = GC_malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) order[i] = i;
    for (int i = 1; i < n; i++) {
        int tmp = order[i];
        int j = i - 1;
        while (j >= 0 &&
               _sort_cmp_descr(key_descrs[order[j]], key_descrs[tmp],
                               keys[order[j]],      keys[tmp]) > 0) {
            order[j+1] = order[j]; j--;
        }
        order[j+1] = tmp;
    }
    ARBLK_t *a = GC_malloc(sizeof(ARBLK_t));
    a->lo         = 1;
    a->hi         = n;
    a->ndim       = 2;
    a->lo2        = 1;
    a->hi2        = 2;
    a->proto_bare = 1;
    a->data = GC_malloc(n * 2 * sizeof(DESCR_t));
    for (int i = 0; i < n; i++) {
        a->data[i * 2 + 0] = key_descrs[order[i]];
        a->data[i * 2 + 1] = vals[order[i]];
    }
    DESCR_t result = {0};
    result.v = DT_A;
    result.arr    = a;
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t compile_to_expression(const char *src) {
    if (!src || !*src) return FAILDESCR;
    tree_t *tree = parse_expr_pat_from_str(src);
    if (!tree) return FAILDESCR;
    DESCR_t d;
    d.v    = DT_E;
    d.slen = 0;
    d.s    = NULL;
    d.ptr  = tree;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rsort_fn(DESCR_t arr) {
    DESCR_t sorted = sort_fn(arr);
    if (sorted.v != DT_A || !sorted.arr) return sorted;
    ARBLK_t *a = sorted.arr;
    int n = a->hi - a->lo + 1;
    for (int lo = 0, hi = n - 1; lo < hi; lo++, hi--) {
        DESCR_t tmp0 = a->data[lo*2+0], tmp1 = a->data[lo*2+1];
        a->data[lo*2+0] = a->data[hi*2+0];
        a->data[lo*2+1] = a->data[hi*2+1];
        a->data[hi*2+0] = tmp0;
        a->data[hi*2+1] = tmp1;
    }
    return sorted;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_ast_pat(tree_t *e) {
    (void)e;
    fprintf(stderr, "[B0b] BOMB eval_ast_pat: AST-walk evaluator deleted; runtime pattern eval needs DT_P builders (B-ladder)\n");
    abort();
}
#define RT_DCAP_MAX 32
typedef struct { const char *varname; const char *base; int len; } rt_dcap_t;
static rt_dcap_t g_rt_dcap[RT_DCAP_MAX];
static int       g_rt_dcap_n = 0;
int              g_rt_dcap_active = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_dcap_record(const char *vname, const char *base, int len) {
    if (!vname || !*vname) return;
    for (int i = 0; i < g_rt_dcap_n; i++) {
        if (g_rt_dcap[i].varname && strcmp(g_rt_dcap[i].varname, vname) == 0) {
            g_rt_dcap[i].base = base; g_rt_dcap[i].len = len; return;
        }
    }
    if (g_rt_dcap_n < RT_DCAP_MAX) {
        g_rt_dcap[g_rt_dcap_n].varname = vname;
        g_rt_dcap[g_rt_dcap_n].base    = base;
        g_rt_dcap[g_rt_dcap_n].len     = len;
        g_rt_dcap_n++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_flush(void) {
    for (int i = 0; i < g_rt_dcap_n; i++) {
        int len = g_rt_dcap[i].len < 0 ? 0 : g_rt_dcap[i].len;
        char *copy = (char *)GC_MALLOC((size_t)len + 1);
        if (copy) { if (len > 0 && g_rt_dcap[i].base) memcpy(copy, g_rt_dcap[i].base, (size_t)len); copy[len] = '\0'; }
        DESCR_t d = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
        NV_SET_fn(g_rt_dcap[i].varname, d);
    }
    g_rt_dcap_n = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_begin(void) { g_rt_dcap_active = 1; g_rt_dcap_n = 0; }
void rt_dcap_end_fail(void) { g_rt_dcap_n = 0; g_rt_dcap_active = 0; }
void rt_dcap_end_ok(void) { rt_dcap_flush(); g_rt_dcap_active = 0; }
void rt_cap_assign_cursor(const char *varname, int saved_delta, int cur_delta, int is_imm)
{
    (void)is_imm;
    if (!varname || !*varname) return;
    int len = cur_delta - saved_delta;
    if (len < 0) len = 0;
    const char *base = Σ ? Σ + saved_delta : NULL;
    if (g_rt_dcap_active) { rt_dcap_record(varname, base, len); return; }
    char *copy = (char *)GC_MALLOC((size_t)len + 1);
    if (copy) { if (len > 0 && base) memcpy(copy, base, (size_t)len); copy[len] = '\0'; }
    DESCR_t matched = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
    NV_SET_fn(varname, matched);
}
extern const char *Σ;
extern int Σlen;
void rt_subject_load_nv(const char *name, void *slot)
{
    DESCR_t v = NV_GET_fn(name ? name : "");
    if (IS_NAMEVAL(v)) v = NV_GET_fn(v.s);
    const char *s = ""; int len = 0;
    if (v.v == DT_S || v.v == DT_SNUL) { s = v.s ? v.s : ""; len = v.slen ? (int)v.slen : (int)strlen(s); }
    else if (IS_INT_fn(v)) { char *b = (char *)GC_MALLOC_ATOMIC(32); snprintf(b, 32, "%lld", (long long)v.i); s = b; len = (int)strlen(b); }
    ((const char **)slot)[0] = s;
    *(int *)((char *)slot + 8) = len;
    Σ = s; Σlen = len;
}
void rt_subject_load_lit(const char *s, void *slot)
{
    if (!s) s = "";
    int len = (int)strlen(s);
    ((const char **)slot)[0] = s;
    *(int *)((char *)slot + 8) = len;
    Σ = s; Σlen = len;
}
DESCR_t rt_concat_parts_d(void *parts, int n)
{
    struct part_t { int tag; int pad; const char *s; } *p = (struct part_t *)parts;
    const char *vals[16]; int lens[16]; size_t total = 0;
    if (n > 16) n = 16;
    for (int i = 0; i < n; i++) {
        const char *s = ""; int len = 0;
        if (p[i].tag == 0) { s = p[i].s ? p[i].s : ""; len = (int)strlen(s); }
        else {
            DESCR_t v = NV_GET_fn(p[i].s ? p[i].s : "");
            if (IS_NAMEVAL(v)) v = NV_GET_fn(v.s);
            if (v.v == DT_S || v.v == DT_SNUL) { s = v.s ? v.s : ""; len = v.slen ? (int)v.slen : (int)strlen(s); }
            else if (IS_INT_fn(v)) { char *b = (char *)GC_MALLOC_ATOMIC(32); snprintf(b, 32, "%lld", (long long)v.i); s = b; len = (int)strlen(b); }
            else if (IS_REAL_fn(v)) { char *b = (char *)GC_MALLOC_ATOMIC(40); gcvt(v.r, 14, b); s = b; len = (int)strlen(b); }
        }
        vals[i] = s; lens[i] = len; total += (size_t)len;
    }
    char *buf = (char *)GC_MALLOC(total + 1); size_t off = 0;
    for (int i = 0; i < n; i++) { if (lens[i] > 0) memcpy(buf + off, vals[i], (size_t)lens[i]); off += (size_t)lens[i]; }
    buf[off] = '\0';
    DESCR_t d = { .v = DT_S, .slen = (uint32_t)total, .s = buf };
    return d;
}
void rt_gvar_assign_concat_parts(const char *dst, void *parts, int n)
{
    DESCR_t d = rt_concat_parts_d(parts, n);
    NV_SET_fn(dst ? dst : "", d);
    if (g_monitor_bin) mon_emit_value_bin(dst ? dst : "", d);
}
void rt_at_cursor(const char *varname, int cur_delta)
{
    if (!varname || !*varname) return;
    DESCR_t pos = { .v = DT_I, .i = (int64_t)cur_delta };
    NV_SET_fn(varname, pos);
}
extern int exec_stmt(const char *sname, DESCR_t *sv, DESCR_t pat, DESCR_t *repl, int has_repl);
extern const char *Σ;
extern int Σlen;
int rt_defer_match(const char *varname, int ival_flag, int cur_delta)
{
    DESCR_t val = NV_GET_fn(varname ? varname : "");
    if (ival_flag) {
        if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s);
        else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val);
    }
    if (val.v == DT_S || val.v == DT_SNUL) {
        const char *lit = val.s ? val.s : "";
        int llen = val.slen ? (int)val.slen : (int)strlen(lit);
        if (cur_delta + llen > Σlen) return -1;
        if (llen > 0 && strncmp(Σ + cur_delta, lit, (size_t)llen) != 0) return -1;
        return cur_delta + llen;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cset_resolve(DESCR_t arg, const char **out_ptr, int *out_len) {
    const char *cv;
    int clen;
    if (IS_CSET_fn(arg)) {
        cv = arg.s;
        if (!cv) return 0;
        int klen = kw_cset_len(cv);
        clen = (klen >= 0) ? klen : (int)strlen(cv);
    } else {
        cv = VARVAL_fn(arg);
        if (!cv) return 0;
        clen = (int)strlen(cv);
    }
    *out_ptr = cv;
    *out_len = clen;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cset_has(const char *cv, int clen, unsigned char ch) {
    return cv && clen > 0 && memchr(cv, ch, (size_t)clen) != NULL;
}
void *rt_defer_get_pat_fn(const char *varname, int ival_flag)
{
    DESCR_t val = NV_GET_fn(varname ? varname : "");
    if (ival_flag) {
        if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s);
        else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val);
    }
    if (val.v == DT_P && val.p) return val.p;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx) {
    DESCR_t bvar = base;
    if (IS_NAMETRAP_fn(base)) base = rt_deref(base);
    if (base.v == DT_A) {
        ARBLK_t *a = base.arr; if (!a) return FAILDESCR;
        int i = (int)to_int(idx); int off = i - a->lo;
        if (off < 0 || off >= (a->hi - a->lo + 1)) return FAILDESCR;
        VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = &a->data[off]; vc->tbl = 0; vc->key = 0; vc->key_d = idx; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
        return NAMETRAP(vc);
    }
    if (base.v == DT_T) {
        TBBLK_t *tb = base.tbl; if (!tb) return FAILDESCR;
        char kb[64]; const char *ks;
        if (IS_INT_fn(idx))       { snprintf(kb, sizeof kb, "%lld", (long long)idx.i); ks = kb; }
        else if (IS_REAL_fn(idx)) { snprintf(kb, sizeof kb, "%g", idx.r); ks = kb; }
        else                      { ks = VARVAL_fn(idx); if (!ks) ks = ""; }
        VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = tb; vc->key = GC_strdup(ks); vc->key_d = idx; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
        return NAMETRAP(vc);
    }
    if (base.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(base, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
            int n = (int)FIELD_GET_fn(base, "frame_size").i;
            DESCR_t ea = FIELD_GET_fn(base, "frame_elems");
            DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
            int i = (int)to_int(idx);
            if (i < 0) i = n + i + 1;
            if (!elems || i < 1 || i > n) return FAILDESCR;
            VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = &elems[i - 1]; vc->tbl = 0; vc->key = 0; vc->key_d = idx; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            return NAMETRAP(vc);
        }
        return subscript_get(base, idx);
    }
    if ((base.v == DT_S || base.v == DT_SNUL) && IS_NAMETRAP_fn(bvar)) {
        const char *sp = base.s ? base.s : ""; long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        long i = (long)to_int(idx);
        if (i <= 0) i = slen + 1 + i;
        if (i < 1 || i > slen) return FAILDESCR;
        VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = 0; vc->key = 0; vc->key_d = idx; vc->sv = bvar; vc->pos = i; vc->len = 1;
        return NAMETRAP(vc);
    }
    return subscript_get(base, idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_field_var(const char *fname, DESCR_t obj) {
    extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
    if (IS_NAMETRAP_fn(obj)) obj = rt_deref(obj);
    if (obj.v != DT_DATA || !obj.u) return FAILDESCR;
    DESCR_t *cell = data_field_ptr(fname ? fname : "", obj);
    if (!cell) return FAILDESCR;
    VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = cell; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
    return NAMETRAP(vc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_list_bang_var_at(DESCR_t obj, int64_t idx) {
    DESCR_t bvar = obj;
    if (IS_NAMETRAP_fn(obj)) obj = rt_deref(obj);
    if (obj.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(obj, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
            int n = (int)FIELD_GET_fn(obj, "frame_size").i;
            DESCR_t ea = FIELD_GET_fn(obj, "frame_elems");
            DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
            if (!elems || idx < 0 || idx >= n) return FAILDESCR;
            VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = &elems[idx]; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            return NAMETRAP(vc);
        }
        if (obj.u && obj.u->type && obj.u->type->nfields > 0) {
            int nf = obj.u->type->nfields;
            if (idx < 0 || idx >= nf) return FAILDESCR;
            VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = &obj.u->fields[idx]; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            return NAMETRAP(vc);
        }
        return FAILDESCR;
    }
    if (obj.v == DT_T && obj.tbl) {
        TBBLK_t *tbl = obj.tbl; int64_t seen = 0;
        for (int b = 0; b < TABLE_BUCKETS; b++)
            for (TBPAIR_t *ep = tbl->buckets[b]; ep; ep = ep->next) {
                if (seen == idx) {
                    VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = &ep->val; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
                    return NAMETRAP(vc);
                }
                seen++;
            }
        return FAILDESCR;
    }
    if ((obj.v == DT_S || obj.v == DT_SNUL) && IS_NAMETRAP_fn(bvar)) {
        const char *sp = obj.s ? obj.s : ""; long slen = obj.slen ? (long)obj.slen : (long)strlen(sp);
        if (idx < 0 || idx >= slen) return FAILDESCR;
        VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = bvar; vc->pos = idx + 1; vc->len = 1;
        return NAMETRAP(vc);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_random_var(DESCR_t base) {
    extern long g_random;
    DESCR_t bvar = base;
    if (IS_NAMETRAP_fn(base)) base = rt_deref(base);
    g_random = (1103515245L * g_random + 453816694L) & 0x7FFFFFFFL; double rval = 4.65661286e-10 * (double)g_random;
    if (base.v == DT_S && base.slen == 0xFFFFFFFFu) {
        const char *cp; int clen; if (!cset_resolve(base, &cp, &clen) || clen <= 0) return FAILDESCR;
        long i = (long)(rval * (double)clen); char *one = GC_malloc(2); one[0] = cp[i]; one[1] = 0;
        return (DESCR_t){ .v = DT_S, .slen = 1, .s = one };
    }
    if ((base.v == DT_S || base.v == DT_SNUL) && IS_NAMETRAP_fn(bvar)) {
        const char *sp = base.s ? base.s : ""; long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        if (slen <= 0) return FAILDESCR;
        VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = bvar; vc->pos = (long)(rval * (double)slen) + 1; vc->len = 1;
        return NAMETRAP(vc);
    }
    if (base.v == DT_S || base.v == DT_SNUL) {
        const char *sp = base.s ? base.s : ""; long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        if (slen <= 0) return FAILDESCR;
        long i = (long)(rval * (double)slen); char *one = GC_malloc(2); one[0] = sp[i]; one[1] = 0;
        return (DESCR_t){ .v = DT_S, .slen = 1, .s = one };
    }
    if (base.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(base, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
            int n = (int)FIELD_GET_fn(base, "frame_size").i;
            DESCR_t ea = FIELD_GET_fn(base, "frame_elems");
            DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
            if (!elems || n <= 0) return FAILDESCR;
            long i = (long)(rval * (double)n);
            VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = &elems[i]; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            return NAMETRAP(vc);
        }
        if (base.u && base.u->type && base.u->type->nfields > 0) {
            int nf = base.u->type->nfields;
            long i = (long)(rval * (double)nf);
            VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = &base.u->fields[i]; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            return NAMETRAP(vc);
        }
        return FAILDESCR;
    }
    if (base.v == DT_T && base.tbl) {
        TBBLK_t *tbl = base.tbl; if (tbl->size <= 0) return FAILDESCR;
        long n = (long)(rval * (double)tbl->size) + 1; long seen = 0;
        for (int b = 0; b < TABLE_BUCKETS; b++)
            for (TBPAIR_t *ep = tbl->buckets[b]; ep; ep = ep->next)
                if (++seen == n) {
                    VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = tbl; vc->key = GC_strdup(ep->key); vc->key_d = ep->key_descr; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
                    return NAMETRAP(vc);
                }
        return FAILDESCR;
    }
    if (base.v == DT_I) {
        int64_t v = base.i; if (v < 0) return FAILDESCR;
        if (v == 0) return REALVAL(rval);
        return INTVAL((int64_t)(rval * (double)v) + 1);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_section_var(DESCR_t base, DESCR_t i1d, DESCR_t i2d) {
    DESCR_t bvar = base;
    if (IS_NAMETRAP_fn(base)) base = rt_deref(base);
    if ((base.v == DT_S || base.v == DT_SNUL) && IS_NAMETRAP_fn(bvar)) {
        const char *sp = base.s ? base.s : ""; long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        long ii = (long)to_int(i1d), jj = (long)to_int(i2d);
        if (ii == 0) ii = slen + 1; else if (ii < 0) ii = slen + ii + 1;
        if (jj == 0) jj = slen + 1; else if (jj < 0) jj = slen + jj + 1;
        if (ii < 1) ii = 1; if (jj > slen + 1) jj = slen + 1;
        if (ii > jj) { long t = ii; ii = jj; jj = t; }
        VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = 0; vc->key = 0; vc->key_d = i1d; vc->sv = bvar; vc->pos = ii; vc->len = jj - ii;
        return NAMETRAP(vc);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_var_ref_cell(DESCR_t *cellp) {
    VCELL_t *vc = GC_malloc(sizeof(VCELL_t)); vc->cellp = cellp; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
    return NAMETRAP(vc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_deref(DESCR_t d) {
    if (d.v == DT_N && d.slen == 0 && d.s && *d.s) { extern DESCR_t NV_GET_fn(const char *); return NV_GET_fn(d.s); }
    if (d.v == DT_N && d.slen == 1 && d.ptr) return *(DESCR_t *)d.ptr;
    if (!IS_NAMETRAP_fn(d)) return d;
    VCELL_t *vc = (VCELL_t *)d.p; if (!vc) return FAILDESCR;
    if (vc->tbl) {
        int found; DESCR_t hit = table_get_found(vc->tbl, vc->key, &found);
        if (found) return hit;
        if (vc->tbl->dflt.v != DT_FAIL && vc->tbl->dflt.v != 0) return vc->tbl->dflt;
        return NULVCL;
    }
    if (vc->cellp) return *vc->cellp;
    if (IS_NAMETRAP_fn(vc->sv)) {
        DESCR_t sd = rt_deref(vc->sv);
        if (sd.v != DT_S && sd.v != DT_SNUL) return FAILDESCR;
        const char *sp = sd.s ? sd.s : ""; long slen = sd.slen ? (long)sd.slen : (long)strlen(sp);
        if (vc->pos + vc->len - 1 > slen) return FAILDESCR;
        char *out = GC_malloc((size_t)vc->len + 1); memcpy(out, sp + vc->pos - 1, (size_t)vc->len); out[vc->len] = 0;
        return (DESCR_t){ .v = DT_S, .slen = (uint32_t)vc->len, .s = out };
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_assign_var(DESCR_t var, DESCR_t val) {
    if (var.v == DT_N && var.slen == 0 && var.s && *var.s) { extern DESCR_t NV_SET_fn(const char *, DESCR_t); NV_SET_fn(var.s, val); return val; }
    if (var.v == DT_N && var.slen == 1 && var.ptr) { *(DESCR_t *)var.ptr = val; return val; }
    if (!IS_NAMETRAP_fn(var)) {
        fprintf(stderr, "[IDX] BOMB rt_assign_var: lvalue is not a variable (dtype=%d) — string/record subscript assignment is the tvsubs rung (GOAL-IR-IMMUTABLE-EMIT IDX-UNIFY)\n", (int)var.v);
        abort();
    }
    VCELL_t *vc = (VCELL_t *)var.p; if (!vc) return FAILDESCR;
    if (vc->tbl) { table_set_descr(vc->tbl, vc->key, vc->key_d, val); return val; }
    if (vc->cellp) { *vc->cellp = val; return val; }
    if (IS_NAMETRAP_fn(vc->sv)) {
        char nb[64]; const char *src; long srclen;
        if (val.v == DT_S || val.v == DT_SNUL) { src = val.s ? val.s : ""; srclen = val.slen ? (long)val.slen : (long)strlen(src); }
        else if (val.v == DT_I) { snprintf(nb, sizeof nb, "%lld", (long long)val.i); src = nb; srclen = (long)strlen(nb); }
        else if (val.v == DT_R) { snprintf(nb, sizeof nb, "%g", val.r); src = nb; srclen = (long)strlen(nb); }
        else { fprintf(stderr, "[IDX] tvsubs assign: value not string-convertible (dtype=%d)\n", (int)val.v); return FAILDESCR; }
        DESCR_t sd = rt_deref(vc->sv);
        if (sd.v != DT_S && sd.v != DT_SNUL) return FAILDESCR;
        const char *sp = sd.s ? sd.s : ""; long slen = sd.slen ? (long)sd.slen : (long)strlen(sp);
        long prelen = vc->pos - 1, poststrt = prelen + vc->len;
        if (poststrt > slen) return FAILDESCR;
        long nlen = prelen + srclen + (slen - poststrt);
        char *ns = GC_malloc((size_t)nlen + 1);
        memcpy(ns, sp, (size_t)prelen); memcpy(ns + prelen, src, (size_t)srclen); memcpy(ns + prelen + srclen, sp + poststrt, (size_t)(slen - poststrt)); ns[nlen] = 0;
        DESCR_t nsd = (DESCR_t){ .v = DT_S, .slen = (uint32_t)nlen, .s = ns };
        DESCR_t wr = rt_assign_var(vc->sv, nsd);
        if (wr.v == DT_FAIL) return FAILDESCR;
        vc->len = srclen;
        char *rs = GC_malloc((size_t)srclen + 1); memcpy(rs, src, (size_t)srclen); rs[srclen] = 0;
        return (DESCR_t){ .v = DT_S, .slen = (uint32_t)srclen, .s = rs };
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static VCELL_t * vcell_ultimate(DESCR_t d) {
    while (IS_NAMETRAP_fn(d)) { VCELL_t *vc = (VCELL_t *)d.p; if (!vc) return 0; if (IS_NAMETRAP_fn(vc->sv)) { d = vc->sv; continue; } return vc; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_swap_var(DESCR_t va, DESCR_t vb) {
    if (!IS_NAMETRAP_fn(va) || !IS_NAMETRAP_fn(vb)) return FAILDESCR;
    VCELL_t *xc = (VCELL_t *)va.p, *yc = (VCELL_t *)vb.p; if (!xc || !yc) return FAILDESCR;
    DESCR_t dx = rt_deref(va), dy = rt_deref(vb);
    if (dx.v == DT_FAIL || dy.v == DT_FAIL) return FAILDESCR;
    long adj1 = 0, adj2 = 0;
    if (IS_NAMETRAP_fn(xc->sv) && IS_NAMETRAP_fn(yc->sv)) {
        VCELL_t *ux = vcell_ultimate(xc->sv), *uy = vcell_ultimate(yc->sv);
        if (ux && uy && ((ux->cellp && ux->cellp == uy->cellp) || (ux->tbl && ux->tbl == uy->tbl && ux->key && uy->key && !strcmp(ux->key, uy->key)))) {
            if (xc->pos > yc->pos) adj1 = xc->len - yc->len;
            else if (yc->pos > xc->pos) adj2 = yc->len - xc->len;
        }
    }
    if (rt_assign_var(va, dy).v == DT_FAIL) return FAILDESCR;
    if (adj2 != 0) yc->pos += adj2;
    if (rt_assign_var(vb, dx).v == DT_FAIL) return FAILDESCR;
    if (adj1 != 0) xc->pos += adj1;
    return rt_deref(va);
}
