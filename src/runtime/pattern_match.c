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
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t (*g_eval_str_hook)(const char *s) = NULL;
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_lit(const char *s) {
    fprintf(stderr, "[B0] BOMB pat_lit: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_span(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_span: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_break_(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_break_: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_breakx(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_breakx: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_any_cs(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_any_cs: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_notany(const char *chars) {
    fprintf(stderr, "[B0] BOMB pat_notany: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_len(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_len: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_pos(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_pos: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rpos(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_rpos: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_tab(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_tab: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rtab(int64_t n) {
    fprintf(stderr, "[B0] BOMB pat_rtab: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_arb(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_arbno(DESCR_t inner) {
    fprintf(stderr, "[B0] BOMB pat_arbno: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rem(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fence_p(DESCR_t inner) {
    fprintf(stderr, "[B0] BOMB pat_fence_p: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fence(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fail(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_abort(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_succeed(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_bal(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_epsilon(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = NULL;
    return v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_cat(DESCR_t left, DESCR_t right) {
    fprintf(stderr, "[B0] BOMB pat_cat: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_alt(DESCR_t left, DESCR_t right) {
    fprintf(stderr, "[B0] BOMB pat_alt: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_ref(const char *name) {
    fprintf(stderr, "[B0] BOMB pat_ref: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_imm(DESCR_t child, DESCR_t var) {
    fprintf(stderr, "[B0] BOMB pat_assign_imm: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_cond(DESCR_t child, DESCR_t var) {
    fprintf(stderr, "[B0] BOMB pat_assign_cond: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_callcap(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs) {
    fprintf(stderr, "[B0] BOMB pat_assign_callcap: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_callcap_named(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs, char **arg_names, int n_arg_names) {
    fprintf(stderr, "[B0] BOMB pat_assign_callcap_named: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_assign_callcap_named_imm(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs, char **arg_names, int n_arg_names) {
    fprintf(stderr, "[B0] BOMB pat_assign_callcap_named_imm: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t var_as_pattern(DESCR_t v) {
    fprintf(stderr, "[B0] BOMB var_as_pattern: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_user_call(const char *name, DESCR_t *args, int nargs) {
    fprintf(stderr, "[B0] BOMB pat_user_call: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_at_cursor(const char *varname) {
    fprintf(stderr, "[B0] BOMB pat_at_cursor: pattern construction needs DT_P builders (B-ladder, GOAL-SNOBOL4-BB)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx) {
    if (arr.v == DT_A) {
        return array_get(arr.arr, (int)to_int(idx));
    }
    if (arr.v == DT_T) {
        char kb[64]; const char *ks;
        if (IS_INT_fn(idx))       { snprintf(kb,sizeof kb,"%lld",(long long)idx.i); ks=kb; }
        else if (IS_REAL_fn(idx)) { snprintf(kb,sizeof kb,"%g",idx.r); ks=kb; }
        else                      { ks = VARVAL_fn(idx); if (!ks) ks=""; }
        if (!table_has(arr.tbl, ks)) {
            if (arr.tbl->dflt.v != DT_FAIL && arr.tbl->dflt.v != 0)
                return arr.tbl->dflt;
            return NULVCL;
        }
        return table_get(arr.tbl, ks);
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
        if (ii > jj) { char *e=GC_malloc(1); e[0]='\0'; return STRVAL(e); }
        int len = jj - ii;
        char *buf = GC_malloc(len+1); memcpy(buf, s+ii-1, len); buf[len]='\0';
        return STRVAL(buf);
    }
    return FAILDESCR;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void register_fn(const char *name, DESCR_t (*fn)(DESCR_t*, int), int min_args, int max_args) {
    (void)min_args; (void)max_args;
    DEFINE_fn(name, fn);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
    DESCR_t compiled = CONVE_fn(expr);
    if (IS_FAIL_fn(compiled)) { fprintf(stderr, "DBG IS_FAIL true!\n"); return FAILDESCR; }
    DESCR_t _ev2 = EXPVAL_fn(compiled);
    return _ev2;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t interp_eval_pat(tree_t *e) {
    (void)e;
    fprintf(stderr, "[B0b] BOMB interp_eval_pat: AST-walk evaluator deleted; runtime pattern eval needs DT_P builders (B-ladder)\n");
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_exec_stmt_pat(void *blob_α, const char *subj_name, int has_repl)
{
    (void)blob_α;
    (void)subj_name;
    (void)has_repl;
    STACKLESS_ABORT("rt_exec_stmt_pat");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_match_blob(void *blob_α,
                         const char *subj_name,
                         int has_repl)
{
    (void)blob_α;
    (void)subj_name;
    (void)has_repl;
    STACKLESS_ABORT("rt_match_blob");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_lit(const char *s)
{
    (void)s;
    STACKLESS_ABORT("rt_pat_lit");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_refname(const char *name)
{
    (void)name;
    STACKLESS_ABORT("rt_pat_refname");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_span(void)
{
    STACKLESS_ABORT("rt_pat_span");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_break(void)
{
    STACKLESS_ABORT("rt_pat_break");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_breakx(void)
{
    STACKLESS_ABORT("rt_pat_breakx");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_any(void)
{
    STACKLESS_ABORT("rt_pat_any");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_notany(void)
{
    STACKLESS_ABORT("rt_pat_notany");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_len(void)
{
    STACKLESS_ABORT("rt_pat_len");
}
void rt_pat_pos(void)
{
    STACKLESS_ABORT("rt_pat_pos");
}
void rt_pat_rpos(void)
{
    STACKLESS_ABORT("rt_pat_rpos");
}
void rt_pat_tab(void)
{
    STACKLESS_ABORT("rt_pat_tab");
}
void rt_pat_rtab(void)
{
    STACKLESS_ABORT("rt_pat_rtab");
}
void rt_pat_arb(void)
{
    STACKLESS_ABORT("rt_pat_arb");
}
void rt_pat_rem(void)
{
    STACKLESS_ABORT("rt_pat_rem");
}
#define RT_DCAP_MAX 32
typedef struct { const char *varname; const char *base; int len; } rt_dcap_t;
static rt_dcap_t g_rt_dcap[RT_DCAP_MAX];
static int       g_rt_dcap_n = 0;
int              g_rt_dcap_active = 0;
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_clear(void) { g_rt_dcap_n = 0; }
void rt_cap_assign(const char *varname, const char *base, int len)
{
    if (!varname || !*varname) return;
    if (len < 0) len = 0;
    if (g_rt_dcap_active) { rt_dcap_record(varname, base, len); return; }
    char *copy = (char *)GC_MALLOC((size_t)len + 1);
    if (copy) { if (len > 0 && base) memcpy(copy, base, (size_t)len); copy[len] = '\0'; }
    DESCR_t matched = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
    NV_SET_fn(varname, matched);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_concat_parts(const char *dst, void *parts, int n)
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
        }
        vals[i] = s; lens[i] = len; total += (size_t)len;
    }
    char *buf = (char *)GC_MALLOC(total + 1); size_t off = 0;
    for (int i = 0; i < n; i++) { if (lens[i] > 0) memcpy(buf + off, vals[i], (size_t)lens[i]); off += (size_t)lens[i]; }
    buf[off] = '\0';
    DESCR_t d = { .v = DT_S, .slen = (uint32_t)total, .s = buf };
    NV_SET_fn(dst ? dst : "", d);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_at_cursor(const char *varname, int cur_delta)
{
    if (!varname || !*varname) return;
    DESCR_t pos = { .v = DT_I, .i = (int64_t)cur_delta };
    NV_SET_fn(varname, pos);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int exec_stmt(const char *sname, DESCR_t *sv, DESCR_t pat, DESCR_t *repl, int has_repl);
extern const char *Σ;
extern int Σlen;
__asm__(
".text\n"
".globl rt_dtp_run\n"
"rt_dtp_run:\n"
"  pushq %rbx\n"
"  pushq %r12\n"
"  pushq %r13\n"
"  pushq %r14\n"
"  pushq %r15\n"
"  leaq 1f(%rip), %rax\n"
"  movq %rax, 8(%rdi)\n"
"  leaq 2f(%rip), %rax\n"
"  movq %rax, 16(%rdi)\n"
"  movq %rsi, %r13\n"
"  movq %rdx, %r14\n"
"  movq %rcx, %r15\n"
"  movq 0(%rdi), %rax\n"
"  jmp *%rax\n"
"1:\n"
"  movq %r14, %rax\n"
"  jmp 3f\n"
"2:\n"
"  movq $-1, %rax\n"
"3:\n"
"  popq %r15\n"
"  popq %r14\n"
"  popq %r13\n"
"  popq %r12\n"
"  popq %rbx\n"
"  ret\n"
);
long rt_dtp_run(struct _DTP_t *h, const char *s, long delta, long Delta);
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pattern_build(DTP_FRAG_t *out, const void *proto, uint32_t len, const DTP_PROTO_DESC *desc, long op_i, const char *op_s)
{
    uint8_t *blob = g_pat_pool_cur; memcpy(blob, proto, (size_t)len); g_pat_pool_cur += len;
    if (desc->op1_off >= 0) { if (op_s) *(const char **)(blob + desc->op1_off) = op_s; else *(long *)(blob + desc->op1_off) = op_i; }
    if (desc->op2_off >= 0) *(long *)(blob + desc->op2_off) = op_i;
    out->entry = blob + desc->entry_off; out->γ_site = (void **)(blob + desc->γ_off); out->ω_site = (void **)(blob + desc->ω_off);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pattern_stitch_cat(DTP_FRAG_t *out, const DTP_FRAG_t *l, const DTP_FRAG_t *r)
{
    *l->γ_site = r->entry; *r->ω_site = (void *)((uint8_t *)l->ω_site + 8);
    out->entry = l->entry; out->γ_site = r->γ_site; out->ω_site = l->ω_site;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pattern_stitch_alt(DTP_FRAG_t *out, const DTP_FRAG_t *l, const DTP_FRAG_t *r)
{
    static const uint8_t join[14] = { 0,0,0,0,0,0,0,0, 0xFF,0x25,0xF2,0xFF,0xFF,0xFF };
    *l->ω_site = r->entry;
    uint8_t *jn = g_pat_pool_cur; memcpy(jn, join, 14); g_pat_pool_cur += 14;
    *l->γ_site = jn + 8; *r->γ_site = jn + 8;
    out->entry = l->entry; out->γ_site = (void **)jn; out->ω_site = r->ω_site;
}
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_lit_proto[125] = {
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0x8B,0x0D,0xE2,0xFF,0xFF,0xFF, 0x41,0x29,0xCE, 0xFF,0x25,0xE9,0xFF,0xFF,0xFF,
    0x8B,0x0D,0xD3,0xFF,0xFF,0xFF, 0x44,0x89,0xF0, 0x01,0xC8, 0x44,0x39,0xF8,
    0x7F,0x38, 0x48,0x8B,0x35,0xBA,0xFF,0xFF,0xFF,
    0x49,0x63,0xCE, 0x49,0x8D,0x7C,0x0D,0x00, 0x8B,0x0D,0xB4,0xFF,0xFF,0xFF,
    0x85,0xC9, 0x74,0x10, 0x8A,0x06, 0x3A,0x07, 0x75,0x19,
    0x48,0xFF,0xC6, 0x48,0xFF,0xC7, 0xFF,0xC9, 0xEB,0xEC,
    0x8B,0x0D,0x9A,0xFF,0xFF,0xFF, 0x41,0x01,0xCE,
    0xFF,0x25,0x99,0xFF,0xFF,0xFF, 0xFF,0x25,0x9B,0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_lit_proto_desc = {47, 32, 16, 24, -1, 0, 8};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_len_proto[78] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x8B,0x0D,0xDA,0xFF,0xFF,0xFF,0x41,0x29,0xCE,0xFF,0x25,0xE9,0xFF,0xFF,0xFF,0x8B,
    0x0D,0xCB,0xFF,0xFF,0xFF,0x44,0x89,0xF0,0x01,0xC8,0x44,0x39,0xF8,0x7F,0x09,0x41,
    0x89,0xC6,0xFF,0x25,0xC8,0xFF,0xFF,0xFF,0xFF,0x25,0xCA,0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_len_proto_desc = {47, 32, 16, 24, -1, 0, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_pos_proto[61] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0x25,0xF2,0xFF,0xFF,0xFF,0x8B,0x0D,0xD4,0xFF,0xFF,0xFF,0x41,0x39,0xCE,0x75,
    0x06,0xFF,0x25,0xD9,0xFF,0xFF,0xFF,0xFF,0x25,0xDB,0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_pos_proto_desc = {38, 32, 16, 24, -1, 0, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_rpos_proto[66] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0x25,0xF2,0xFF,0xFF,0xFF,0x8B,0x0D,0xD4,0xFF,0xFF,0xFF,0x44,0x89,0xF8,0x29,
    0xC8,0x41,0x39,0xC6,0x75,0x06,0xFF,0x25,0xD4,0xFF,0xFF,0xFF,0xFF,0x25,0xD6,0xFF,
    0xFF,0xFF,
};
const DTP_PROTO_DESC bb_rpos_proto_desc = {38, 32, 16, 24, -1, 0, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_tab_proto[83] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF,0xFF,0x25,0xEB,0xFF,0xFF,0xFF,0x8B,0x0D,0xCD,
    0xFF,0xFF,0xFF,0x44,0x39,0xF1,0x7C,0x15,0x44,0x39,0xF9,0x7F,0x10,0x44,0x89,0x35,
    0xC4,0xFF,0xFF,0xFF,0x41,0x89,0xCE,0xFF,0x25,0xC3,0xFF,0xFF,0xFF,0xFF,0x25,0xC5,
    0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_tab_proto_desc = {45, 32, 16, 24, -1, 0, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_rtab_proto[83] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF,0xFF,0x25,0xEB,0xFF,0xFF,0xFF,0x8B,0x0D,0xCD,
    0xFF,0xFF,0xFF,0x44,0x89,0xF8,0x29,0xC8,0x44,0x39,0xF0,0x7C,0x10,0x44,0x89,0x35,
    0xC4,0xFF,0xFF,0xFF,0x41,0x89,0xC6,0xFF,0x25,0xC3,0xFF,0xFF,0xFF,0xFF,0x25,0xC5,
    0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_rtab_proto_desc = {45, 32, 16, 24, -1, 0, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_fail_proto[44] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0x25,0xF2,0xFF,0xFF,0xFF,0xFF,0x25,0xEC,0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_fail_proto_desc = {38, 32, 16, 24, -1, -1, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_rem_proto[61] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF,0xFF,0x25,0xEB,0xFF,0xFF,0xFF,0x44,0x89,0x35,
    0xD4,0xFF,0xFF,0xFF,0x45,0x89,0xFE,0xFF,0x25,0xD3,0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_rem_proto_desc = {45, 32, 16, 24, -1, -1, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_succeed_proto[44] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0x25,0xEA,0xFF,0xFF,0xFF,0xFF,0x25,0xE4,0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_succeed_proto_desc = {38, 32, 16, 24, -1, -1, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_fence_proto[44] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0x25,0xF2,0xFF,0xFF,0xFF,0xFF,0x25,0xE4,0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_fence_proto_desc = {38, 32, 16, 24, -1, -1, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
const uint8_t bb_abort_proto[44] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFF,0x25,0xF2,0xFF,0xFF,0xFF,0xFF,0x25,0xEC,0xFF,0xFF,0xFF,
};
const DTP_PROTO_DESC bb_abort_proto_desc = {38, 32, 16, 24, -1, -1, -1};
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_pat(const char *name, void *head);
void rt_dtp_head_build(DTP_FRAG_t *frag, const char *varname)
{
    static const uint8_t head[36] = {
        0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
        0xFF,0x25,0xEA,0xFF,0xFF,0xFF, 0xFF,0x25,0xEC,0xFF,0xFF,0xFF,
    };
    uint8_t *blob = g_pat_pool_cur; memcpy(blob, head, 36); g_pat_pool_cur += 36;
    *(void **)(blob + 0) = frag->entry;
    *frag->γ_site = blob + 24; *frag->ω_site = blob + 30;
    rt_gvar_assign_pat(varname ? varname : "", blob);
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
    if (val.v == DT_P && val.p) {
        long nd2 = rt_dtp_run(val.p, Σ, (long)cur_delta, (long)Σlen);
        return (nd2 >= 0) ? (int)nd2 : -1;
    }
    return -1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_fence(void)
{
    STACKLESS_ABORT("rt_pat_fence");
}
void rt_pat_fail(void)
{
    STACKLESS_ABORT("rt_pat_fail");
}
void rt_pat_abort(void)
{
    STACKLESS_ABORT("rt_pat_abort");
}
void rt_pat_succeed(void)
{
    STACKLESS_ABORT("rt_pat_succeed");
}
void rt_pat_bal(void)
{
    STACKLESS_ABORT("rt_pat_bal");
}
void rt_pat_eps(void)
{
    STACKLESS_ABORT("rt_pat_eps");
}
void rt_pat_arbno(void)
{
    STACKLESS_ABORT("rt_pat_arbno");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_fence1(void)
{
    STACKLESS_ABORT("rt_pat_fence1");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_cat(void)
{
    STACKLESS_ABORT("rt_pat_cat");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_alt(void)
{
    STACKLESS_ABORT("rt_pat_alt");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_deref(void)
{
    STACKLESS_ABORT("rt_pat_deref");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture(const char *varname, int kind)
{
    (void)varname;
    (void)kind;
    STACKLESS_ABORT("rt_pat_capture");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture_fn(const char *fname, int is_imm, const char *namelist)
{
    (void)fname;
    (void)is_imm;
    (void)namelist;
    STACKLESS_ABORT("rt_pat_capture_fn");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_capture_fn_args(const char *fname, int is_imm, int nargs)
{
    (void)fname;
    (void)is_imm;
    (void)nargs;
    STACKLESS_ABORT("rt_pat_capture_fn_args");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_usercall(const char *fname)
{
    (void)fname;
    STACKLESS_ABORT("rt_pat_usercall");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_pat_usercall_args(const char *fname, int nargs)
{
    (void)fname;
    (void)nargs;
    STACKLESS_ABORT("rt_pat_usercall_args");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_match_variant(const char *subj_name, int has_repl)
{
    (void)subj_name;
    (void)has_repl;
    STACKLESS_ABORT("rt_match_variant");
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
int cset_has(const char *cv, int clen, unsigned char ch) {
    return cv && clen > 0 && memchr(cv, ch, (size_t)clen) != NULL;
}
