#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "dtp.h"
#include "core.h"
#include "ast.h"
#include "../frontend/snobol4/scrip_cc.h"
#include "sil_macros.h"
#include "builtins/gen_runtime.h"
#include "rt/gc_heap.h"
#include "rt/rt_arena.h"
#include "zeta_choices.h"
#include "snobol4_system_fns.h"
#define STACKLESS_ABORT(fn) \
    do { fprintf(stderr, "libscrip_rt: %s called — Icon value stack removed (GROUND ZERO 3). " \
                         "This box must be rebuilt stackless (per-box slot, no value stack).\n", (fn)); \
         abort(); } while (0)
DESCR_t (*g_eval_str_hook)(const char *s) = NULL;
static DATBLK_t *g_lf_type;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int rt_list_view(DESCR_t o, DESCR_t **elems, int *n) {
    if (o.v != DT_DATA || !o.u) return 0;
    DATBLK_t *t = o.u->type;
    if (t != g_lf_type) { if (!t || t->nfields < 3 || !t->fields[0] || strcmp(t->fields[0], "frame_elems") != 0) return 0; g_lf_type = t; }
    DESCR_t gt = o.u->fields[2]; if (gt.v != DT_S || !gt.s || strcmp(gt.s, "list") != 0) return 0;
    DESCR_t ea = o.u->fields[0]; *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL; *n = (int)o.u->fields[1].i;
    return 1;
}
typedef struct dtp_rcp { int tt; const char *s; uint32_t slen; int64_t ival; struct dtp_rcp *l; struct dtp_rcp *r; } dtp_rcp_t;
typedef struct DTP { void *fn; dtp_rcp_t *rcp; int64_t zsz; int32_t zstatic; int32_t zpad; DESCR_t *snap; int64_t nsnap; } DTP_t;
_Static_assert(__builtin_offsetof(DTP_t, fn) == 0, "bb_match_defer inline cache reads DTP_t.fn at offset 0");
_Static_assert(__builtin_offsetof(DTP_t, zsz) == 16, "PS-3 ARBNO stride latch reads DTP_t.zsz at offset 16");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pstamp_trace(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_PSTAMP_TRACE"); v = e ? (atoi(e) != 0) : 0; } return v; }
static DTP_t *dtp_new(void *fn, dtp_rcp_t *rcp) { DTP_t *h = (DTP_t *)rt_ws_alloc(sizeof(DTP_t)); h->fn = fn; h->rcp = rcp; h->zsz = 0; h->zstatic = 0; h->zpad = 0; h->snap = 0; h->nsnap = 0; return h; }
void *dtp_wrap_fn(void *fn) { return (void *)dtp_new(fn, (dtp_rcp_t *)0); }
void *dtp_wrap_fn_sz(void *fn, int64_t zsz, int32_t zstatic) { DTP_t *h = dtp_new(fn, (dtp_rcp_t *)0); h->zsz = zsz; h->zstatic = zstatic; if (pstamp_trace()) fprintf(stderr, "PSTAMP wrap fn=%p zsz=%lld zstatic=%d\n", fn, (long long)zsz, (int)zstatic); return (void *)h; }
int64_t dtp_zsz_of(void *headv) { DTP_t *h = (DTP_t *)headv; return h ? h->zsz : 0; }
int dtp_zstatic_of(void *headv) { DTP_t *h = (DTP_t *)headv; return h ? (int)h->zstatic : 0; }
static dtp_rcp_t *rcp_node(int tt, const char *s, uint32_t n, int64_t iv, dtp_rcp_t *l, dtp_rcp_t *rr) { dtp_rcp_t *r = (dtp_rcp_t *)rt_ws_alloc(sizeof *r); r->tt = tt; r->s = s; r->slen = n; r->ival = iv; r->l = l; r->r = rr; return r; }
static dtp_rcp_t *rcp_lit(const char *s, uint32_t n) { return rcp_node(TT_QLIT, s ? s : "", n, 0, 0, 0); }
static dtp_rcp_t *rcp_bin(int tt, dtp_rcp_t *l, dtp_rcp_t *rr) { return rcp_node(tt, 0, 0, 0, l, rr); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static dtp_rcp_t *rcp_of(DESCR_t d) {
    if (d.v == DT_P && d.p) {
        DTP_t *h = (DTP_t *)d.p;
        if (h->rcp) return h->rcp;
        static int opq_uid = 0; char nb[24]; snprintf(nb, sizeof nb, "OPQ$%d", opq_uid++);
        NV_SET_fn(nb, d);
        { const char *pn = rt_ws_strdup_c(nb); return rcp_node(TT_DEFER, pn, (uint32_t)strlen(pn), 0, 0, 0); }
    }
    if (d.v == DT_X) { const char *nm = d.s ? d.s : ""; uint32_t nl = d.slen ? d.slen : (uint32_t)strlen(nm); char *pb = rt_str_alloc((int)nl + 1); pb[0] = '*'; memcpy(pb + 1, nm, nl); pb[nl + 1] = 0; return rcp_node(TT_DEFER, pb, nl + 1, 0, 0, 0); }
    if (d.v == DT_S || d.v == DT_SNUL) { const char *s = d.s ? d.s : ""; return rcp_lit(s, d.slen ? d.slen : (uint32_t)strlen(s)); }
    if (IS_INT_fn(d)) { char *b = rt_str_alloc(31); snprintf(b, 32, "%lld", (long long)d.i); return rcp_lit(b, (uint32_t)strlen(b)); }
    if (IS_REAL_fn(d)) { char *b = rt_str_alloc(39); gcvt(d.r, 14, b); return rcp_lit(b, (uint32_t)strlen(b)); }
    { const char *s = VARVAL_fn(d); return rcp_lit(s ? s : "", s ? (uint32_t)strlen(s) : 0); }
}
extern tree_t *ast_stmt_new(tree_e kind);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dtp_rcp_tree(dtp_rcp_t *r, DESCR_t self) {
    if (!r) { tree_t *t = ast_stmt_new(TT_QLIT); t->v.sval = (char *)""; return t; }
    tree_t *t = ast_stmt_new((tree_e)r->tt);
    switch (r->tt) {
    case TT_QLIT: t->v.sval = (char *)(r->s ? r->s : ""); break;
    case TT_SEQ: case TT_ALT: ast_push(t, dtp_rcp_tree(r->l, self)); ast_push(t, dtp_rcp_tree(r->r, self)); break;
    case TT_ANY: case TT_NOTANY: case TT_SPAN: case TT_BREAK: case TT_BREAKX: { tree_t *c = ast_stmt_new(TT_QLIT); c->v.sval = (char *)(r->s ? r->s : ""); ast_push(t, c); break; }
    case TT_LEN: case TT_TAB: case TT_RTAB: case TT_POS: case TT_RPOS: { tree_t *c = ast_stmt_new(TT_ILIT); c->v.ival = r->ival; ast_push(t, c); break; }
    case TT_ARBNO: {
        static int arb_uid = 0; char nb[24]; snprintf(nb, sizeof nb, "ARB$%d", arb_uid++);
        DESCR_t sub;
        if (self.p && ((DTP_t *)self.p)->rcp == r) sub = self;
        else { sub.v = DT_P; sub.slen = 0; sub.p = (void *)dtp_new((void *)0, r); }
        NV_SET_fn(nb, sub);
        if (sub.p != self.p) { extern void *dtp_fn_of(void *); dtp_fn_of(sub.p); }
        t->t = TT_ALT;
        { tree_t *nul = ast_stmt_new(TT_QLIT); nul->v.sval = (char *)""; ast_push(t, nul); }
        { tree_t *sq = ast_stmt_new(TT_SEQ); ast_push(sq, dtp_rcp_tree(r->l, self));
          tree_t *df = ast_stmt_new(TT_DEFER); tree_t *v = ast_stmt_new(TT_VAR); v->v.sval = rt_ws_strdup_c(nb); ast_push(df, v); ast_push(sq, df); ast_push(t, sq); }
        break; }
    case TT_DEFER: { tree_t *v = ast_stmt_new(TT_VAR); v->v.sval = (char *)(r->s ? r->s : ""); ast_push(t, v); break; }
    case TT_FENCE: if (r->ival) ast_push(t, dtp_rcp_tree(r->l, self)); break;
    case TT_CAPT_COND_ASGN: case TT_CAPT_IMMED_ASGN: { ast_push(t, dtp_rcp_tree(r->l, self)); tree_t *v = ast_stmt_new(TT_VAR); v->v.sval = (char *)(r->s ? r->s : ""); ast_push(t, v); break; }
    case TT_CAPT_CURSOR: { tree_t *v = ast_stmt_new(TT_VAR); v->v.sval = (char *)(r->s ? r->s : ""); ast_push(t, v); break; }
    default: break;
    }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *dtp_fn_of(void *headv) {
    DTP_t *h = (DTP_t *)headv;
    if (!h) return (void *)0;
    if (!h->fn && h->rcp) { extern void *bb_compile_pat_tree_sz(const void *tv, int64_t *zsz, int32_t *zstatic); DESCR_t sd; sd.v = DT_P; sd.slen = 0; sd.p = (void *)h; h->fn = bb_compile_pat_tree_sz((const void *)dtp_rcp_tree(h->rcp, sd), &h->zsz, &h->zstatic);
        if (pstamp_trace()) fprintf(stderr, "PSTAMP fn=%p zsz=%lld zstatic=%d\n", h->fn, (long long)h->zsz, (int)h->zstatic); }
    return h->fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_lit(const char *s) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_lit(s ? s : "", s ? (uint32_t)strlen(s) : 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_span(const char *chars) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_SPAN, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_break_(const char *chars) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_BREAK, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_breakx(const char *chars) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_BREAKX, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_any_cs(const char *chars) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_ANY, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_notany(const char *chars) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_NOTANY, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_len(int64_t n) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_LEN, 0, 0, n, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_pos(int64_t n) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_POS, 0, 0, n, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rpos(int64_t n) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_RPOS, 0, 0, n, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_tab(int64_t n) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_TAB, 0, 0, n, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rtab(int64_t n) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_RTAB, 0, 0, n, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_arb(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_ARB, 0, 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_arbno(DESCR_t inner) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_ARBNO, 0, 0, 0, rcp_of(inner), 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rem(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_REM, 0, 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fence_p(DESCR_t inner) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_FENCE, 0, 0, 1, rcp_of(inner), 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fence(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_FENCE, 0, 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fail(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_FAIL, 0, 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_abort(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_ABORT, 0, 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_succeed(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_SUCCEED, 0, 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_bal(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_BAL, 0, 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_epsilon(void) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_lit("", 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_mk_cset(int tt, const char *cs) { DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(tt, cs ? cs : "", cs ? (uint32_t)strlen(cs) : 0, 0, 0, 0)); return v; }
DESCR_t pat_mk_num(int tt, int64_t n) { DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(tt, 0, 0, n, 0, 0)); return v; }
DESCR_t pat_mk_nil(int tt) { DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(tt, 0, 0, 0, 0, 0)); return v; }
DESCR_t pat_mk_capt(int tt, const char *name, DESCR_t sub) { DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(tt, name ? name : "", name ? (uint32_t)strlen(name) : 0, 0, rcp_of(sub), 0)); return v; }
DESCR_t pat_mk_cursor(const char *name) { DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_CAPT_CURSOR, name ? name : "", name ? (uint32_t)strlen(name) : 0, 0, 0, 0)); return v; }
DESCR_t pat_defer(const char *name) { DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_DEFER, name ? name : "", name ? (uint32_t)strlen(name) : 0, 0, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_cat(DESCR_t left, DESCR_t right) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_bin(TT_SEQ, rcp_of(left), rcp_of(right)));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_alt(DESCR_t left, DESCR_t right) {
    DESCR_t v; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_bin(TT_ALT, rcp_of(left), rcp_of(right)));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx) {
    if (arr.v == DT_A) {
        return array_get(arr.arr, (int)to_int(idx));
    }
    if (arr.v == DT_T) {
        int found; DESCR_t hit = table_get_found_d(arr.tbl, idx, &found);
        if (found) return hit;
        if (arr.tbl->dflt.v != DT_FAIL && arr.tbl->dflt.v != 0)
            return arr.tbl->dflt;
        return NULVCL;
    }
    if (arr.v == DT_I) {
        char ibuf[32]; snprintf(ibuf, sizeof ibuf, "%lld", (long long)arr.i);
        arr = STRVAL(rt_str_dup(ibuf));
    }
    if (arr.v == DT_S || arr.v == DT_SNUL) {
        const char *s = arr.s ? arr.s : "";
        int slen = (int)strlen(s);
        int i = (int)to_int(idx);
        if (i < 0) i = slen + i + 1;
        if (i < 1 || i > slen) return FAILDESCR;
        char *buf = rt_str_alloc(1); buf[0] = s[i-1]; buf[1] = '\0';
        return STRVAL(buf);
    }
    if (arr.v == DT_DATA) {
        DESCR_t *elems; int n;
        if (rt_list_view(arr, &elems, &n)) {
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
static int subscript_set_body(DESCR_t arr, DESCR_t idx, DESCR_t val) {
    if (arr.v == DT_A) {
        int i = (int)to_int(idx);
        if (i < arr.arr->lo || i > arr.arr->hi) return 0;
        array_set(arr.arr, i, val);
        return 1;
    }
    if (arr.v == DT_T) {
        table_set_descr_d(arr.tbl, idx, val);
        return 1;
    }
    if (arr.v == DT_DATA) {
        DESCR_t *elems; int n;
        if (rt_list_view(arr, &elems, &n)) {
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
        char *ns = rt_str_alloc(newlen);
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
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val) { int ok = subscript_set_body(arr, idx, val); if (ok && g_monitor_bin) mon_emit_value_bin("<lval>", val); return ok; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t subscript_get2_ext(DESCR_t arr, DESCR_t i, DESCR_t end) {
    long ii = (long)to_int(i), ee = (long)to_int(end);
    if (((-ii) ^ (-ee)) < 0) return FAILDESCR;
    return subscript_get2(arr, i, end);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j) {
    if (arr.v == DT_A)
        return array_get2(arr.arr, (int)to_int(i), (int)to_int(j));
    if (arr.v == DT_DATA) {
        DESCR_t *elems; int n;
        if (rt_list_view(arr, &elems, &n)) {
            int ii = (int)to_int(i), jj = (int)to_int(j);
            if (ii < -n || ii > n + 1) return FAILDESCR;
            if (jj < -n || jj > n + 1) return FAILDESCR;
            if (ii <= 0) ii = n + ii + 1;
            if (jj <= 0) jj = n + jj + 1;
            if (ii > jj) { int t = ii; ii = jj; jj = t; }
            int rlen = jj - ii;
            if (rlen <= 0) {
                static int list_empty_reg = 0;
                if (!list_empty_reg) { DEFDAT_fn("list(frame_elems,frame_size,gen_type,frame_cap)"); list_empty_reg=1; }
                DESCR_t empty_ptr; empty_ptr.v=DT_DATA; empty_ptr.slen=0; empty_ptr.ptr=NULL;
                return DATCON_fn("list", empty_ptr, INTVAL(0), STRVAL("list"), INTVAL(0));
            }
            DESCR_t *rbuf = rt_ws_alloc(rlen * sizeof(DESCR_t));
            for (int k = 0; k < rlen; k++) rbuf[k] = (elems && ii+k-1 >= 0 && ii+k-1 < n) ? elems[ii+k-1] : NULVCL;
            DESCR_t rptr; rptr.v=DT_DATA; rptr.slen=0; rptr.ptr=(void*)rbuf;
            static int list_slice_reg = 0;
            if (!list_slice_reg) { DEFDAT_fn("list(frame_elems,frame_size,gen_type,frame_cap)"); list_slice_reg=1; }
            return DATCON_fn("list", rptr, INTVAL(rlen), STRVAL("list"), INTVAL(rlen));
        }
    }
    if (arr.v == DT_S || arr.v == DT_SNUL) {
        const char *s = arr.s ? arr.s : "";
        int slen = (arr.slen && arr.slen != 0xFFFFFFFFu) ? (int)arr.slen : (int)strlen(s);
        int ii = (int)to_int(i), jj = (int)to_int(j);
        if (ii < -slen || ii > slen + 1) return FAILDESCR;
        if (jj < -slen || jj > slen + 1) return FAILDESCR;
        if (ii <= 0) ii = slen + ii + 1;
        if (jj <= 0) jj = slen + jj + 1;
        if (ii > jj) { int t = ii; ii = jj; jj = t; }
        int len = jj - ii;
        char *buf = rt_str_alloc(len); memcpy(buf, s+ii-1, len); buf[len]='\0';
        return STRVAL(buf);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int subscript_set2_body(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val) {
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
int subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val) { int ok = subscript_set2_body(arr, i, j, val); if (ok && g_monitor_bin) mon_emit_value_bin("<lval>", val); return ok; }
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
    const char *nm  = VARVAL_fn(newname);
    const char *old = NULL;
    const char *ks = getenv("SCRIP_OPSYN_KIND");
    if (!(ks && *ks == '0')) {
        extern int kwb_error(int code, const char *msg);
        if (!is_numeric_like(type)) { kwb_error(152, "opsyn third argument is not integer"); return FAILDESCR; }
        { int64_t kind = to_int(type);
          if (kind < 0 || kind > 16777216) { kwb_error(153, "opsyn third argument is negative or too large"); return FAILDESCR; }
          if (kind != 0 && !(nm && nm[0] && !nm[1] && strchr(kind == 1 ? "!#%/=|" : "#%&@~", nm[0]))) { kwb_error(156, "opsyn first arg is not correct operator name"); return FAILDESCR; } }
    }
    if (to_int(type) == 0 && sn4_is_system_fn(nm)) { extern int kwb_error(int code, const char *msg); kwb_error(248, "attempted redefinition of system function"); return FAILDESCR; }
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
    /*⭐ DIRECT-ARRAY OVERLOAD (conform-rsort-sort-array-noop): a table argument falls through to the
      key/value path below; an array argument has no keys, so it is sorted on a COPY of its values using
      the same comparator and same string-encoding (tbl_key_str) the table path already uses for its keys
      -- one comparator, two shapes in, matching SPITBOL: SORT ascending, RSORT (via the reverse below) descending. */
    if (arr.v == DT_A) {
        ARBLK_t *src = arr.arr;
        if (!src) return arr;
        int n = src->hi - src->lo + 1;
        if (n <= 0) return arr;
        DESCR_t *vals = rt_ws_alloc(n * sizeof(DESCR_t));
        const char **strs = rt_ws_alloc(n * sizeof(char *));
        char *bufblk = rt_ws_alloc((size_t)n * 64);
        for (int i = 0; i < n; i++) { vals[i] = src->data[i]; strs[i] = tbl_key_str(vals[i], bufblk + (size_t)i * 64, 64); }
        for (int i = 1; i < n; i++) {
            DESCR_t tv = vals[i]; const char *ts = strs[i];
            int j = i - 1;
            while (j >= 0 && _sort_cmp_descr(vals[j], tv, strs[j], ts) > 0) {
                vals[j+1] = vals[j]; strs[j+1] = strs[j]; j--;
            }
            vals[j+1] = tv; strs[j+1] = ts;
        }
        ARBLK_t *a = array_new(src->lo, src->hi);
        for (int i = 0; i < n; i++) a->data[i] = vals[i];
        DESCR_t result = {0}; result.v = DT_A; result.arr = a;
        return result;
    }
    if (arr.v != DT_T) return arr;
    TBBLK_t *tbl = arr.tbl;
    if (!tbl) return FAILDESCR;
    int n = 0; TBPAIR_t *e;
    TBL_FOREACH(tbl, e) n++;
    if (n == 0) return FAILDESCR;
    const char **keys = rt_ws_alloc(n * sizeof(char *));
    DESCR_t *key_descrs = rt_ws_alloc(n * sizeof(DESCR_t));
    DESCR_t *vals = rt_ws_alloc(n * sizeof(DESCR_t));
    int idx = 0;
    TBL_FOREACH(tbl, e) {
            keys[idx] = tbl_pair_key(e);
            key_descrs[idx] = e->key_descr;
            vals[idx] = e->val;
            idx++;
        }
    int *order = rt_ws_alloc(n * sizeof(int));
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
    ARBLK_t *a = rt_ws_alloc(sizeof(ARBLK_t));
    a->lo         = 1;
    a->hi         = n;
    a->ndim       = 1;
    a->lo2        = 0;
    a->hi2        = 0;
    a->proto_bare = 1;
    a->id         = rt_agg_serial_list();
    { char pb[48]; snprintf(pb, sizeof pb, "%d,2", n); a->proto = rt_ws_strdup(pb); }
    a->data = rt_ws_alloc(n * sizeof(DESCR_t));
    for (int i = 0; i < n; i++) {
        ARBLK_t *row = rt_ws_alloc(sizeof(ARBLK_t));
        row->lo = 1; row->hi = 2; row->ndim = 1; row->lo2 = 0; row->hi2 = 0; row->proto_bare = 1; row->proto = 0; row->id = rt_agg_serial_list();
        row->data = rt_ws_alloc(2 * sizeof(DESCR_t));
        row->data[0] = key_descrs[order[i]];
        row->data[1] = vals[order[i]];
        DESCR_t rd = {0}; rd.v = DT_A; rd.arr = row;
        a->data[i] = rd;
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
        DESCR_t tmp = a->data[lo];
        a->data[lo] = a->data[hi];
        a->data[hi] = tmp;
    }
    return sorted;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t eval_ast_pat(tree_t *e) {
    (void)e;
    fprintf(stderr, "[B0b] BOMB eval_ast_pat: AST-walk evaluator deleted; runtime pattern eval needs DT_P builders (B-ladder)\n");
    abort();
}
#define RT_CAS_ISLAND_BYTES ((size_t)8u << 20)
#define RT_CAS_CAPX_MAX     (1 << 16)
#define RT_CAS_DFX_MAX      (1 << 14)
#define RT_CAS_DCF_MAX      (1 << 14)
#define RT_CAS_SPK_MAX      256
static char  *g_cas_base = 0;
static size_t g_cas_used = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *rt_cas_carve(size_t bytes)
{
    extern void *rt_slab_region(size_t);
    if (!g_cas_base) { g_cas_base = (char *)rt_slab_region(RT_CAS_ISLAND_BYTES); if (!g_cas_base) { fprintf(stderr, "rt_cas: island reserve failed\n"); abort(); } }
    bytes = (bytes + 15u) & ~(size_t)15u;
    if (g_cas_used + bytes > RT_CAS_ISLAND_BYTES) { fprintf(stderr, "rt_cas: carve of %zu exceeds the island (raise RT_CAS_ISLAND_BYTES)\n", bytes); abort(); }
    void *p = g_cas_base + g_cas_used; g_cas_used += bytes; memset(p, 0, bytes);
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_cas_roots(void **base, size_t *bytes) { if (base) *base = (void *)g_cas_base; if (bytes) *bytes = g_cas_used; }
uint64_t g_scan_hit_start = 0;
uint64_t g_sno_defer_cells[4096];
uint64_t g_pat_main_rsp = 0;
uint64_t g_rspd_save = 0, g_rspd_g4 = 0, g_rspd_g5 = 0, g_rspd_s2 = 0, g_rspd_g6 = 0, g_rspd_beta = 0;
static int g_rspd_active = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rt_rspd_init(void) { g_rspd_active = (getenv("SCRIP_RSPDIFF") != NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((destructor)) static void rt_rspd_report(void) {
    if (!g_rspd_active) return;
    fprintf(stderr, "RSPDIFF raw: save=%#lx g4=%#lx g5=%#lx s2=%#lx g6=%#lx beta=%#lx\n", (unsigned long)g_rspd_save, (unsigned long)g_rspd_g4, (unsigned long)g_rspd_g5, (unsigned long)g_rspd_s2, (unsigned long)g_rspd_g6, (unsigned long)g_rspd_beta);
    if (g_rspd_save && g_rspd_g4)  fprintf(stderr, "RSPDIFF gamma-retained (save-g4)   = %ld\n", (long)(g_rspd_save - g_rspd_g4));
    if (g_rspd_save && g_rspd_g5)  fprintf(stderr, "RSPDIFF omega-restored (save-g5)   = %ld\n", (long)(g_rspd_save - g_rspd_g5));
    if (g_rspd_save && g_rspd_beta) fprintf(stderr, "RSPDIFF beta-children (save-beta)  = %ld\n", (long)(g_rspd_save - g_rspd_beta));
    if (g_rspd_s2 && g_rspd_g6)    fprintf(stderr, "RSPDIFF exhaust-delta (s2-g6)      = %ld\n", (long)(g_rspd_s2 - g_rspd_g6));
}
#define RT_DCAP_ISLAND_BYTES ((size_t)64u << 20)
typedef struct { const char *varname; uint64_t saved_delta; uint64_t len; } rt_dcap_e;
const char *g_dcap_base = 0;
#include "pin_va.h"
#define g_dcap_top (*(const char **)RT_DCAP_TOP)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_lazy_init(void) {
    extern void *rt_slab_region(size_t);
    if (!g_dcap_top) {
        g_dcap_base = (const char *)rt_slab_region(RT_DCAP_ISLAND_BYTES);
        if (!g_dcap_base) { fprintf(stderr, "rt_dcap: island reserve failed\n"); abort(); }
        g_dcap_top = g_dcap_base;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_cap_fail_retreat(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_CAP_FAIL_RETREAT"); v = (e && *e == '0') ? 0 : 1; } return v; }
int rt_cap_name_strict(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_CAP_NAME_STRICT"); v = (e && *e == '0') ? 0 : 1; } return v; }
typedef struct { const char *cur; const char *top; const char *subj; DESCR_t pending; } rt_dcf_t;
__attribute__((visibility("hidden"))) rt_dcf_t *g_dcf; __attribute__((visibility("hidden"))) int g_dcf_top; __attribute__((visibility("hidden"))) int g_dcf_cap;
__attribute__((visibility("hidden"))) int g_dcap_trace = -1;
_Static_assert(sizeof(rt_dcf_t) == 40, "rtx_match.S RTX-8 slice 8 hardcodes stride 40 for rt_dcf_t");
_Static_assert(offsetof(rt_dcf_t, cur) == 0, "rtx_match.S RTX-8 slice 8 hardcodes cur at +0");
_Static_assert(offsetof(rt_dcf_t, top) == 8, "rtx_match.S RTX-8 slice 8 hardcodes top at +8");
_Static_assert(offsetof(rt_dcf_t, subj) == 16, "rtx_match.S RTX-8 slice 8 hardcodes subj at +16");
_Static_assert(offsetof(rt_dcf_t, pending) == 24, "rtx_match.S RTX-8 slice 8 hardcodes pending at +24");
_Static_assert(sizeof(DESCR_t) == 16, "rtx_match.S RTX-8 slice 8 stores pending as v/slen qword + s qword");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((visibility("hidden"))) long rt_dcap_pump(void)
{
    extern long rt_proc_call_open(const char *name, int nargs);
    extern int rt_g_want_name;
    if (g_dcf_top <= 0) return 0;
    rt_dcf_t *c = &g_dcf[g_dcf_top - 1];
    long rc = 0;
    while (c->cur < c->top) {
        const rt_dcap_e *e = (const rt_dcap_e *)(const void *)c->cur;
        int len = (int)e->len; if (len < 0) len = 0;
        /* ⛔ THE CAPTURE MUST LIE INSIDE THE SUBJECT. Guarding len<0 was never enough: a 4-byte input of
           "\t" reached here with len=480251808 after a deferred *F() re-entered and pushed a second dcf
           frame, and the outer frame's entry was then read stale. That allocated ~458 MB (heap exhausted
           at 512 MB, abort) -- and the ABORT WAS MASKING THE REAL HAZARD, because had the allocation
           succeeded the memcpy below would have read 458 MB PAST THE SUBJECT. The same bound is already
           enforced elsewhere in this file for literals; it simply was not applied on this path.
           ⭐ This is a GUARD, not the root fix: the root defect is that a reentrant push invalidates the
           outer frame's entries. Refusing here turns silent memory corruption into a named, bounded
           failure that says exactly what it saw. */
        { extern int Σlen;
          long long _end = (long long)e->saved_delta + (long long)len;
          if (len > Σlen || _end > (long long)Σlen) {
              fprintf(stderr, "rt_dcap_pump: CORRUPT CAPTURE ENTRY refused — len=%d saved_delta=%llu end=%lld exceeds subject length %d (target '%s', frame depth %d). Deferred re-entry invalidated the outer frame; capture skipped rather than reading out of bounds.\n",
                      len, (unsigned long long)e->saved_delta, _end, Σlen, e->varname ? e->varname : "(null)", g_dcf_top);
              c->cur += sizeof(rt_dcap_e);
              rc = 1;
              continue; } }
        char *copy = rt_str_alloc(len);
        if (copy) { if (len > 0 && c->subj) memcpy(copy, c->subj + e->saved_delta, (size_t)len); copy[len] = '\0'; }
        DESCR_t d = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
        c->cur += sizeof(rt_dcap_e);
        if (e->varname && e->varname[0] == '*') {
            const int strict = rt_cap_name_strict();
            extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
            extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
            extern int rt_g_ret_by_name;
            /* ⭐ A DEFERRED TARGET THAT IS AN INDIRECT REFERENCE YIELDS A NAME BY CONSTRUCTION, AND THE LOWERER
               KNOWS IT STATICALLY. `p = "HELLO" . *$("dummy")` must land in `dummy`, but $() lowers to an
               unconditional IR_DEREF, so the thunk hands back the VALUE of dummy (null) and the by_name gate
               below refused the whole match. The discrimination cannot be re-derived here -- at run time a
               name string and an ordinary string are the same bytes, which is exactly why the gate must stay
               as strict as it is for every other shape (it is what correctly fails `. *("dum" "my")`). So the
               lowerer carries the fact in the callee name, the same way this varname already carries `*`:
               an EXPRNM$ thunk lowers the INNER expression and returns the NAME STRING. */
            const int nmyield = !strncmp(e->varname + 1, "EXPRNM$", 7);
            { int _wsv = rt_g_want_name; rt_g_want_name = 1;
            DESCR_t nm = rt_call_proc_descr(e->varname + 1, 0);
            rt_g_want_name = _wsv;
            const int by_name = rt_g_ret_by_name || nmyield; rt_g_ret_by_name = 0;
            if (IS_FAIL_fn(nm)) { if (strict) { if (g_dcap_trace < 0) { const char *_e = getenv("SCRIP_DCAP_TRACE"); g_dcap_trace = (_e && _e[0]) ? 1 : 0; } if (g_dcap_trace) fprintf(stderr, "[DCAP] STRICT-REFUSE target=%s: call FAILED -> rc=1 (match will fail at END)\n", e->varname); rc = 1; break; } fprintf(stderr, "[DCAP] WARN deferred assignment target '%s' failed or is not invocable; conditional assignment skipped\n", e->varname); continue; }
            if (strict && !by_name) { if (g_dcap_trace < 0) { const char *_e = getenv("SCRIP_DCAP_TRACE"); g_dcap_trace = (_e && _e[0]) ? 1 : 0; } if (g_dcap_trace) fprintf(stderr, "[DCAP] STRICT-REFUSE target=%s: returned a VALUE not a NAME (by_name=0, nm.v=%d, nm.slen=%u, nm.s=%.24s) -> rc=1 (match will fail at END)\n", e->varname, (int)nm.v, nm.slen, (nm.v == DT_S && nm.s) ? nm.s : "?"); rc = 1; break; }
            if (IS_STR_fn(nm)) {
                                 const char *ns = VARVAL_fn(nm); if (ns && *ns) NV_SET_fn(ns, d); }
            else rt_assign_var(nm, d);
            continue;
        }
            }
        if (e->varname && e->varname[0]) NV_SET_fn(e->varname, d);
    }
    return rc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long c_rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj)
{
    { if (g_dcap_trace < 0) { const char *_e = getenv("SCRIP_DCAP_TRACE"); g_dcap_trace = (_e && _e[0]) ? 1 : 0; } if (g_dcap_trace) fprintf(stderr, "[DCAP] end_ok n=%ld\n", (long)((top - mark) / (long)sizeof(rt_dcap_e))); }
    if (!g_dcf) { g_dcf = (rt_dcf_t *)rt_cas_carve((size_t)RT_CAS_DCF_MAX * sizeof(rt_dcf_t)); g_dcf_cap = RT_CAS_DCF_MAX; }
    if (g_dcf_top >= g_dcf_cap) { fprintf(stderr, "rt_cas: dcf overflow (%d) — raise RT_CAS_DCF_MAX\n", g_dcf_cap); abort(); }
    rt_dcf_t *c = &g_dcf[g_dcf_top++];
    c->cur = mark; c->top = top; c->subj = subj; c->pending = NULVCL;
    return rt_dcap_pump();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long c_rt_dcap_step(DESCR_t nm)
{
    extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
    extern int rt_g_want_name;
    if (g_dcf_top <= 0) return 0;
    rt_dcf_t *c = &g_dcf[g_dcf_top - 1];
    rt_g_want_name = 0;
    if (!IS_FAIL_fn(nm)) { if (IS_STR_fn(nm)) { const char *ns = VARVAL_fn(nm); if (ns && *ns) NV_SET_fn(ns, c->pending); } else rt_assign_var(nm, c->pending); }
    return rt_dcap_pump();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long c_rt_match_end_all(const char *mark, const char *top, const char *subj, const uint64_t *outer)
{
    extern void rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t capgen);
    extern long rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj);
    long rc = rt_dcap_end_ok_open(mark, top, subj);
    { if (g_dcap_trace < 0) { const char *_e = getenv("SCRIP_DCAP_TRACE"); g_dcap_trace = (_e && _e[0]) ? 1 : 0; } if (g_dcap_trace && rc) fprintf(stderr, "[DCAP] MATCH_END rc=%ld -> OMEGA (strict stub)\n", rc); }
    if (g_dcf_top > 0) g_dcf_top--;
    rt_match_ctx_restore(outer[0], outer[1], 0);
    return rc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_flush(void) { fprintf(stderr, "[DCAP] FATAL rt_dcap_flush: dead C-side flush called — the commit flush is box-driven since NCB-1c M3 (rt_dcap_end_ok_open/step/close)\n"); abort(); }
void rt_dcap_end_ok(void) { fprintf(stderr, "[DCAP] FATAL rt_dcap_end_ok: superseded by the box-driven pump (NCB-1c M3: rt_dcap_end_ok_open/step/close)\n"); abort(); }
typedef struct { uint32_t *buf; uint32_t gen; uint32_t sp; } rt_cap_stk_t;
uint32_t g_cap_gen = 1;
__attribute__((visibility("hidden"))) uint32_t g_cap_gen_next = 1;
_Static_assert(__builtin_offsetof(rt_cap_stk_t, buf) == 0, "rtx_match.S hardcodes rt_cap_stk_t.buf at +0; the struct drifted -- rt_cap_top would read the span array through the wrong member, which links fine and returns garbage capture cursors silently");
_Static_assert(__builtin_offsetof(rt_cap_stk_t, gen) == 8, "rtx_match.S hardcodes rt_cap_stk_t.gen at +8; the struct drifted -- the generation compare would test the wrong word and stale frames would resurrect across statements");
_Static_assert(__builtin_offsetof(rt_cap_stk_t, sp) == 12, "rtx_match.S hardcodes rt_cap_stk_t.sp at +12; the struct drifted -- rt_cap_pop/rt_cap_top would index the wrong word");
_Static_assert(sizeof(uint32_t) == 4, "rtx_match.S scales the sp index by 4 in [rdx+rcx*4]; uint32_t drifted");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_cap_push(void *slot, int delta)
{
    rt_cap_stk_t *s = (rt_cap_stk_t *)slot;
    if (s->gen != g_cap_gen) { s->sp = 0; s->gen = g_cap_gen; }
    if (!s->buf) { s->buf = (uint32_t *)rt_ws_alloc(17 * sizeof(uint32_t)); s->buf[0] = 16; }
    if (s->sp == s->buf[0]) {
        uint32_t nc = s->buf[0] * 2;
        uint32_t *nb = (uint32_t *)rt_ws_alloc(((size_t)nc + 1) * sizeof(uint32_t));
        memcpy(nb + 1, s->buf + 1, (size_t)s->sp * sizeof(uint32_t));
        nb[0] = nc; s->buf = nb;
    }
    s->buf[1 + s->sp++] = (uint32_t)delta;
}
static DESCR_t *g_capx; static int g_capx_top, g_capx_cap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long c_rt_cap_open(const char *varname, int saved_delta, int cur_delta, int is_imm)
{
    (void)is_imm;
    if (!varname || !*varname) return 0;
    int len = cur_delta - saved_delta;
    if (len < 0) len = 0;
    const char *base = Σ ? Σ + saved_delta : NULL;
    char *copy = rt_str_alloc(len);
    if (copy) { if (len > 0 && base) memcpy(copy, base, (size_t)len); copy[len] = '\0'; }
    DESCR_t matched = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
    if (varname[0] != '*') { rt_bomb("c_rt_cap_open: plain-name arm DELETED (s196 Lon one-to-maintain) — rt_cap_open in rtx_match.S is the sole spelling; this entry serves computed-name '*' targets only"); return 0; }
    extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
    extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
    extern int rt_g_want_name;
    DESCR_t nm_capo;
    { int _wsv2 = rt_g_want_name; rt_g_want_name = 1;
      nm_capo = rt_call_proc_descr(varname + 1, 0);
      rt_g_want_name = _wsv2; }
    if (IS_FAIL_fn(nm_capo)) return rt_cap_fail_retreat() ? -1 : 0;
    if (IS_STR_fn(nm_capo)) { const char *ns = VARVAL_fn(nm_capo); if (ns && *ns) NV_SET_fn(ns, matched); } else rt_assign_var(nm_capo, matched);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_cap_finish(DESCR_t nm)
{
    extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
    extern int rt_g_want_name;
    rt_g_want_name = 0;
    DESCR_t matched = g_capx_top > 0 ? g_capx[--g_capx_top] : (DESCR_t){ .v = DT_S, .slen = 0, .s = "" };
    if (!IS_FAIL_fn(nm)) rt_assign_var(nm, matched);
}
extern const char *Σ;
extern int Σlen;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_subject_load_nv(const char *name, void *slot)
{
    DESCR_t v = NV_GET_fn(name ? name : "");
    if (IS_NAMEVAL(v)) v = NV_GET_fn(v.s);
    const char *s = ""; int len = 0;
    if (v.v == DT_S || v.v == DT_SNUL) { s = v.s ? v.s : ""; len = v.slen ? (int)v.slen : (int)strlen(s); }
    else if (IS_INT_fn(v)) { char *b = rt_str_alloc(31); snprintf(b, 32, "%lld", (long long)v.i); s = b; len = (int)strlen(b); }
    ((const char **)slot)[0] = s;
    *(int *)((char *)slot + 8) = len;
    Σ = s; Σlen = len;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_subject_load_lit(const char *s, void *slot)
{
    if (!s) s = "";
    int len = (int)strlen(s);
    ((const char **)slot)[0] = s;
    *(int *)((char *)slot + 8) = len;
    Σ = s; Σlen = len;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
            else if (IS_INT_fn(v)) { char *b = rt_str_alloc(31); snprintf(b, 32, "%lld", (long long)v.i); s = b; len = (int)strlen(b); }
            else if (IS_REAL_fn(v)) { char *b = rt_str_alloc(39); gcvt(v.r, 14, b); s = b; len = (int)strlen(b); }
        }
        vals[i] = s; lens[i] = len; total += (size_t)len;
    }
    char *buf = rt_str_alloc((long)total); size_t off = 0;
    for (int i = 0; i < n; i++) { if (lens[i] > 0) memcpy(buf + off, vals[i], (size_t)lens[i]); off += (size_t)lens[i]; }
    buf[off] = '\0';
    DESCR_t d = { .v = DT_S, .slen = (uint32_t)total, .s = buf };
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_concat_parts(const char *dst, void *parts, int n)
{
    DESCR_t d = rt_concat_parts_d(parts, n);
    NV_SET_fn(dst ? dst : "", d);
    if (g_monitor_bin) mon_emit_value_bin(dst ? dst : "", d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_at_cursor(const char *varname, int cur_delta)
{
    if (!varname || !*varname) return;
    DESCR_t pos = { .v = DT_I, .i = (int64_t)cur_delta };
    NV_SET_fn(varname, pos);
}
extern int exec_stmt(const char *sname, DESCR_t *sv, DESCR_t pat, DESCR_t *repl, int has_repl);
extern const char *Σ;
extern int Σlen;
typedef struct { DESCR_t val; int failed; int dtx_used; } rt_dfx_t;
typedef struct { void *fn; long aux; } rt_defer_pr_t;
__attribute__((visibility("hidden"))) rt_dfx_t *g_dfx;
__attribute__((visibility("hidden"))) int g_dfx_top, g_dfx_cap;
_Static_assert(sizeof(rt_dfx_t) == 24, "rtx_match.S strides g_dfx by 24");
_Static_assert(__builtin_offsetof(rt_dfx_t, val) == 0, "rtx_match.S reads val at +0");
_Static_assert(__builtin_offsetof(rt_dfx_t, failed) == 16, "rtx_match.S reads failed at +16");
_Static_assert(__builtin_offsetof(rt_dfx_t, dtx_used) == 20, "rtx_match.S reads dtx_used at +20");
_Static_assert(sizeof(DESCR_t) == 16, "rtx_match.S assumes the 16-byte DESCR pair");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_dfx_t *rt_dfx_push(void) {
    if (!g_dfx) { g_dfx = (rt_dfx_t *)rt_cas_carve((size_t)RT_CAS_DFX_MAX * sizeof(rt_dfx_t)); g_dfx_cap = RT_CAS_DFX_MAX; }
    if (g_dfx_top >= g_dfx_cap) { fprintf(stderr, "rt_cas: dfx overflow (%d) — raise RT_CAS_DFX_MAX\n", g_dfx_cap); abort(); }
    rt_dfx_t *s = &g_dfx[g_dfx_top++]; s->val = NULVCL; s->failed = 0; s->dtx_used = 0; return s;
}
typedef struct { const char *nm; DESCR_t val; } rt_spk_t;
static int rt_defer_xpat_on(void);
static int rt_spk_take(const char *nm, DESCR_t *out);
static rt_spk_t *g_spk;
static int g_spk_n, g_spk_cap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_cas_live_span(int i, void **base, size_t *bytes)
{
    void *b = 0; size_t n = 0;
    switch (i) {
        case 0: if (g_capx && g_capx_top > 0) { b = (void *)g_capx; n = (size_t)g_capx_top * sizeof(DESCR_t); } break;
        case 1: if (g_dfx  && g_dfx_top  > 0) { b = (void *)g_dfx;  n = (size_t)g_dfx_top  * sizeof(rt_dfx_t); } break;
        case 2: if (g_dcf  && g_dcf_top  > 0) { b = (void *)g_dcf;  n = (size_t)g_dcf_top  * sizeof(rt_dcf_t); } break;
        case 3: if (g_spk  && g_spk_n    > 0) { b = (void *)g_spk;  n = (size_t)g_spk_n    * sizeof(rt_spk_t); } break;
        default: return 0;
    }
    if (base) *base = b; if (bytes) *bytes = n;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_defer_nv_read(const char *name)
{
    extern int rt_udc_on(void);
    if (name && name[0] == '&' && rt_udc_on() && NV_CONST_ASSIGNED_fn(name)) return NV_KW_GET_fn(name);
    return NV_GET_fn(name ? name : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long c_rt_defer_open(const char *varname, int ival_flag)
{
    extern long rt_proc_call_open(const char *name, int nargs);
    rt_dfx_t *s = rt_dfx_push(); if (!s) return 0;
    if (varname && varname[0] == 'F' && !strcmp(varname, "FAIL")) { s->failed = 1; return 0; }
    if (varname && varname[0] == '*') {
        for (int _i = 0; _i < g_spk_n; _i++) { if (g_spk[_i].nm && !strcmp(g_spk[_i].nm, varname)) { DESCR_t r = g_spk[_i].val; if (_i < g_spk_n - 1) memmove(&g_spk[_i], &g_spk[_i+1], (size_t)(g_spk_n-1-_i)*sizeof(rt_spk_t)); g_spk_n--; if (IS_FAIL_fn(r)) { s->failed = 1; return 0; } if (r.v == DT_X && !s->dtx_used) { s->dtx_used = 1; long fb2 = rt_proc_call_open(r.s ? r.s : "", 0); if (!fb2) s->failed = 1; return fb2; } s->val = r; return 0; } }
        long fb = rt_proc_call_open(varname + 1, 0); if (!fb) s->failed = 1; return fb;
    }
    DESCR_t val = rt_defer_nv_read(varname);
    if (ival_flag) {
        if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s);
        else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val);
    }
    if (val.v == DT_X) { s->dtx_used = 1; long fb = rt_proc_call_open(val.s ? val.s : "", 0); if (!fb) s->failed = 1; return fb; }
    s->val = val;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_defer_step(DESCR_t val)
{
    extern long rt_proc_call_open(const char *name, int nargs);
    if (g_dfx_top <= 0) return 0;
    rt_dfx_t *s = &g_dfx[g_dfx_top - 1];
    if (IS_FAIL_fn(val)) { s->failed = 1; return 0; }
    if (val.v == DT_X && !s->dtx_used) { s->dtx_used = 1; long fb = rt_proc_call_open(val.s ? val.s : "", 0); if (!fb) s->failed = 1; return fb; }
    s->val = val;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int c_rt_defer_close(int cur_delta)
{
    if (g_dfx_top <= 0) return -1;
    rt_dfx_t s = g_dfx[--g_dfx_top];
    if (s.failed) return -1;
    DESCR_t val = s.val;
    if (IS_FAIL_fn(val)) return -1;
    char nb[40];
    if (val.v == DT_I) { snprintf(nb, sizeof nb, "%lld", (long long)val.i); val.v = DT_S; val.slen = (uint32_t)strlen(nb); val.s = nb; }
    else if (val.v == DT_R) { snprintf(nb, sizeof nb, "%g", val.r); val.v = DT_S; val.slen = (uint32_t)strlen(nb); val.s = nb; }
    if (val.v == DT_S || val.v == DT_SNUL) {
        const char *lit = val.s ? val.s : "";
        int llen = val.slen ? (int)val.slen : (int)strlen(lit);
        if (cur_delta + llen > Σlen) return -1;
        if (llen > 0 && strncmp(Σ + cur_delta, lit, (size_t)llen) != 0) return -1;
        return cur_delta + llen;
    }
    return -1;
}
#define RT_XPAT_CHAIN_MAX 256
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_defer_xstar_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_DEFER_XSTAR"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_dtx_drain(DESCR_t r)
{
    extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
    for (int _g = 0; r.v == DT_X && r.s && _g < RT_XPAT_CHAIN_MAX; _g++) r = rt_call_proc_descr(r.s, 0);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_defer_take(rt_dfx_t *s, DESCR_t r)
{
    extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
    if (IS_FAIL_fn(r)) { s->failed = 1; return; }
    if (r.v == DT_X && r.s) { s->dtx_used = 1; r = rt_dtx_drain(r); if (IS_FAIL_fn(r)) { s->failed = 1; return; } }
    s->val = r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_defer_run_all(const char *varname, int cur_delta)
{
    extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
    rt_dfx_t *s = rt_dfx_push(); if (!s) return -1;
    if (varname && varname[0] == 'F' && !strcmp(varname, "FAIL")) { s->failed = 1; return c_rt_defer_close(cur_delta); }
    if (varname && varname[0] == '*') {
        for (int _i = 0; _i < g_spk_n; _i++) { if (g_spk[_i].nm && !strcmp(g_spk[_i].nm, varname)) { DESCR_t r = g_spk[_i].val; if (_i < g_spk_n - 1) memmove(&g_spk[_i], &g_spk[_i+1], (size_t)(g_spk_n-1-_i)*sizeof(rt_spk_t)); g_spk_n--; if (IS_FAIL_fn(r)) { s->failed = 1; } else if (r.v == DT_X) { s->dtx_used = 1; rt_defer_take(s, rt_call_proc_descr(r.s ? r.s : "", 0)); } else s->val = r; return c_rt_defer_close(cur_delta); } }
        s->dtx_used = 1; rt_defer_take(s, rt_call_proc_descr(varname + 1, 0)); return c_rt_defer_close(cur_delta);
    }
    DESCR_t val = rt_defer_nv_read(varname);
    if (val.v == DT_X) { s->dtx_used = 1; DESCR_t _pk; if (rt_defer_xpat_on() && rt_spk_take(val.s, &_pk)) rt_defer_take(s, _pk); else rt_defer_take(s, rt_call_proc_descr(val.s ? val.s : "", 0)); return c_rt_defer_close(cur_delta); }
    s->val = val;
    return c_rt_defer_close(cur_delta);
}
static DESCR_t patv_slot(void *hv, long i, const char *fb, int ival_flag);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_patv_defer_run_all(void *hv, long i, const char *fb, int cur_delta)
{
    extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
    rt_dfx_t *s = rt_dfx_push(); if (!s) return -1;
    DESCR_t val; { DTP_t *h = (DTP_t *)hv;   /* patv_slot's snapshot arm, hoisted -- see the block on rt_patv_defer_get_pat_dtp */
      if (h && h->snap && i >= 0 && i < h->nsnap) val = h->snap[i]; else val = patv_slot(hv, i, fb, 0); }
    if (val.v == DT_X) { s->dtx_used = 1; DESCR_t _pk; if (rt_defer_xpat_on() && rt_spk_take(val.s, &_pk)) rt_defer_take(s, _pk); else rt_defer_take(s, rt_call_proc_descr(val.s ? val.s : "", 0)); return c_rt_defer_close(cur_delta); }
    s->val = val;
    return c_rt_defer_close(cur_delta);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⛔ SNOBOL4 STRINGS ARE COUNTED, NOT NUL-TERMINATED -- CHAR(0) IS A LEGAL CHARACTER (Lon, s264: "Using any C function to manipulate strings is INVALID since the NUL character problem").  This is the
   ONE funnel every character-class builtin resolves its argument through, so it is the one place the class is fixed: BREAK, SPAN, ANY, NOTANY and their kin all arrive here.  The else arm used a bare
   strlen(), which stops at the first NUL: ANY(CHAR(0)) resolved to a cset of length ZERO, and every caller rejects a zero-length cset -- so a legal program died with "argument is not a string or
   expression" (59/69/151) -- an error the oracle never raises -- while SPAN silently returned no-match.  Length now comes from the DESCRIPTOR, which SIZE() already proves is stamped correctly.
   ⛔ THE GUARDS ARE LOAD-BEARING, ALL THREE: cv == arg.s because VARVAL_fn may hand back a CONVERTED buffer (an integer rendered as text) whose length has nothing to do with arg.slen; arg.slen != 0
   preserves the strlen fallback for any descriptor that never got its length stamped, so this cannot regress a working path; and 0xFFFFFFFFu is the IS_CSET_fn sentinel, not a length -- a real cset
   takes the branch above and gets kw_cset_len, which is what lets &ALPHABET carry its own NUL. */
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
        clen = (cv == arg.s && arg.slen && arg.slen != 0xFFFFFFFFu) ? (int)arg.slen : (int)strlen(cv);
    }
    *out_ptr = cv;
    *out_len = clen;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cset_has(const char *cv, int clen, unsigned char ch) {
    return cv && clen > 0 && memchr(cv, ch, (size_t)clen) != NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *c_rt_defer_get_pat_fn(const char *varname, int ival_flag)
{
    if (varname && varname[0] == '*') {
        extern DESCR_t rt_call_proc_descr(const char *, int);
        DESCR_t r = rt_call_proc_descr(varname + 1, 0);
        if (r.v == DT_P && r.p) { extern void *dtp_fn_of(void *); return dtp_fn_of(r.p); }
        if (!g_spk) { g_spk = (rt_spk_t *)rt_cas_carve((size_t)RT_CAS_SPK_MAX * sizeof(rt_spk_t)); g_spk_cap = RT_CAS_SPK_MAX; }
        if (g_spk_n >= g_spk_cap) { fprintf(stderr, "rt_cas: spk overflow (%d) — raise RT_CAS_SPK_MAX\n", g_spk_cap); abort(); }
        g_spk[g_spk_n].nm = varname; g_spk[g_spk_n].val = r; g_spk_n++;
        return NULL;
    }
    rt_bomb("c_rt_defer_get_pat_fn: plain-name arm DELETED (s196 Lon one-to-maintain) — rt_defer_get_pat_fn in rtx_match.S is the sole plain-name spelling (the asm bails here for star-vars ONLY)");
    DESCR_t val = NV_GET_fn(varname ? varname : "");
    if (ival_flag) {
        if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s);
        else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val);
    }
    if (val.v == DT_P && val.p) { extern void *dtp_fn_of(void *); return dtp_fn_of(val.p); }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_patv_freeze(void *hv, const char *bn, long n)
{
    DTP_t *h = (DTP_t *)hv;
    if (!h || !bn || n <= 0) return;
    DESCR_t *v = (DESCR_t *)rt_ws_alloc((size_t)n * sizeof(DESCR_t));
    for (long i = 0; i < n; i++) { char nb[64]; snprintf(nb, sizeof nb, "%s$V%ld", bn, i); v[i] = NV_GET_fn(nb); }
    h->snap = v; h->nsnap = n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t patv_slot(void *hv, long i, const char *fb, int ival_flag)
{
    DTP_t *h = (DTP_t *)hv;
    if (h && h->snap && i >= 0 && i < h->nsnap) return h->snap[i];
    { DESCR_t val = rt_defer_nv_read(fb); if (ival_flag) { if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s); else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val); } return val; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_defer_xpat_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_DEFER_XPAT"); v = (e && *e == '0') ? 0 : 1; } return v; }
static void rt_spk_park(const char *nm, DESCR_t r) { if (!g_spk) { g_spk = (rt_spk_t *)rt_cas_carve((size_t)RT_CAS_SPK_MAX * sizeof(rt_spk_t)); g_spk_cap = RT_CAS_SPK_MAX; } if (g_spk_n >= g_spk_cap) { fprintf(stderr, "rt_cas: spk overflow (%d)\n", g_spk_cap); abort(); } g_spk[g_spk_n].nm = nm; g_spk[g_spk_n].val = r; g_spk_n++; }
static int rt_spk_take(const char *nm, DESCR_t *out) { if (!nm) return 0; for (int _i = 0; _i < g_spk_n; _i++) { if (g_spk[_i].nm && !strcmp(g_spk[_i].nm, nm)) { *out = g_spk[_i].val; if (_i < g_spk_n - 1) memmove(&g_spk[_i], &g_spk[_i+1], (size_t)(g_spk_n-1-_i)*sizeof(rt_spk_t)); g_spk_n--; return 1; } } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_defer_xpat_dtp(const char *nm)
{
    extern DESCR_t rt_call_proc_descr(const char *, int);
    if (!rt_defer_xpat_on()) return NULL;
    DESCR_t r = rt_call_proc_descr(nm ? nm : "", 0);
    r = rt_dtx_drain(r);
    if (r.v == DT_P && r.p) { extern void *dtp_fn_of(void *); dtp_fn_of(r.p); return r.p; }
    rt_spk_park(nm, r);
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐⭐ THE SNAPSHOT SLOT IS READ IN PLACE, NOT THROUGH A CALL (hq_P s266).  patv_slot below stays the C OF RECORD and every arm it owns is still reached through it; what is hoisted here is
  only its FIRST LINE -- the three-load snapshot hit -- because the json deserializer takes it 316,517 times per parse (17 per value node: every `*jvalue` / `*jelement` reference in the
  grammar re-resolves the pattern variable).  MEASURED: patv_slot 10.1M Ir/parse + dtp_fn_of 3.5M + this function's own frame = ~11% of the whole program, to hand back a pointer that was
  already sitting in a slot.  ⛔ Written out rather than made always_inline on purpose: s264 measured that always_inline on the descr.h/core.h tag predicates BROKE three deferred-capture
  tests by moving descriptors out of memory where the GC's stack scan could no longer see them, and this file is the deferred-capture engine itself. */
void *rt_patv_defer_get_pat_dtp(void *hv, long i, const char *fb)
{
    /*⭐ The head is returned; dtp_fn_of is called for its SIDE EFFECT (compile the blob on first use) and its result is discarded.  So when the blob is already compiled the call cannot
       change the answer -- fn non-null is exactly dtp_fn_of's own "nothing to do" condition -- and skipping it here is an identity, not an approximation. */
    { DTP_t *h = (DTP_t *)hv;
      if (h && h->snap && i >= 0 && i < h->nsnap) { DESCR_t sv = h->snap[i]; if (sv.v == DT_P && sv.p && ((DTP_t *)sv.p)->fn) return sv.p; } }
    { DESCR_t v = patv_slot(hv, i, fb, 0);
      if (v.v == DT_P && v.p) { extern void *dtp_fn_of(void *); dtp_fn_of(v.p); return v.p; }
      if (v.v == DT_X && rt_defer_xpat_on()) { extern void *rt_defer_xpat_dtp(const char *); return rt_defer_xpat_dtp(v.s); } }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_patv_defer_open(void *hv, long i, const char *fb, int ival_flag)
{
    extern long rt_proc_call_open(const char *name, int nargs);
    rt_dfx_t *s = rt_dfx_push(); if (!s) return 0;
    DESCR_t val; { DTP_t *h = (DTP_t *)hv;   /* patv_slot's snapshot arm, hoisted -- see the block on rt_patv_defer_get_pat_dtp.  ival_flag is applied only on the NON-snapshot arm, so a
                                                snapshot hit is flag-independent and this is the same value patv_slot would have returned. */
      if (h && h->snap && i >= 0 && i < h->nsnap) val = h->snap[i]; else val = patv_slot(hv, i, fb, ival_flag); }
    if (val.v == DT_X) { s->dtx_used = 1; long fb2 = rt_proc_call_open(val.s ? val.s : "", 0); if (!fb2) s->failed = 1; return fb2; }
    s->val = val;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_defer_get_pat_dtp(const char *varname, int ival_flag)
{
    if (varname && varname[0] == '*') {
        extern DESCR_t rt_call_proc_descr(const char *, int);
        DESCR_t r = rt_call_proc_descr(varname + 1, 0);
        if (rt_defer_xstar_on()) r = rt_dtx_drain(r);
        if (r.v == DT_P && r.p) { extern void *dtp_fn_of(void *); dtp_fn_of(r.p); return r.p; }
        if (!g_spk) { g_spk = (rt_spk_t *)rt_cas_carve((size_t)RT_CAS_SPK_MAX * sizeof(rt_spk_t)); g_spk_cap = RT_CAS_SPK_MAX; }
        if (g_spk_n >= g_spk_cap) { fprintf(stderr, "rt_cas: spk overflow (%d) — raise RT_CAS_SPK_MAX\n", g_spk_cap); abort(); }
        g_spk[g_spk_n].nm = varname; g_spk[g_spk_n].val = r; g_spk_n++;
        return NULL;
    }
    DESCR_t val = rt_defer_nv_read(varname);
    if (ival_flag) {
        if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s);
        else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val);
    }
    if (val.v == DT_P && val.p) { extern void *dtp_fn_of(void *); dtp_fn_of(val.p); return val.p; }
    if (val.v == DT_X && rt_defer_xpat_on()) { extern void *rt_defer_xpat_dtp(const char *); return rt_defer_xpat_dtp(val.s); }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐ ONE RESOLUTION PER DEFERRED NODE, NOT TWO (hq_P s260, roman row).  The emitted defer box called rt_defer_get_pat_dtp and then, when the value was not a pattern, rt_defer_run_all -- and BOTH opened with
   rt_defer_nv_read(varname) on the SAME baked literal, back to back, with nothing between them but a test and a jz.  callgrind on roman.sno (N=2000, -O2) named the pair exactly: rt_defer_nv_read'rt_defer_get_pat_dtp
   594,060 Ir and rt_defer_nv_read'rt_defer_run_all 594,060 Ir -- identical counts -- driving NV_GET_fn'rt_defer_nv_read to 19.35% of ALL of roman's instructions plus 5.91% of __strcmp_avx2 underneath it.  The
   lookup was already minimal (hashed, memoised, chain length ~1); the defect was doing it TWICE.  This entry resolves ONCE and returns BOTH answers in registers -- rax=fn, rdx=dtp when the value is a pattern,
   rax=0 and rdx=the new cursor when it is a string -- so no state persists across the call and RULES.md's NO-NEW-GLOBALS rule is satisfied by construction rather than by grant.
   ⛔ THREE CASES DELIBERATELY FALL BACK TO THE ORIGINAL TWO-CALL PAIR, because for them the second read is NOT redundant: (1) a '*'-prefixed name, where get_pat_dtp PUSHES to g_spk and run_all POPS it -- a
   producer/consumer pair, not a repeat; (2) DT_X, where resolving the pattern CALLS A PROCEDURE that may itself assign the variable, so the second read can legitimately differ; (3) a DT_P whose fn has not
   materialised.  Only the plain-value case -- which is what a deferred ordinary variable is -- reuses the resolution.  ⛔ ival_flag is 0 on this arm by construction: the box emits `xor esi, esi` at the only
   site that reaches here, and esi now carries cur_delta instead. */
/* ⛔ always_inline, and READ ONCE PER CALL -- the fourth instance of this class in one session.  At -O0 (the s262
   NO-`-O2` fact rule makes -O0 the number of record) gcc emits a real call for a `static inline`, so a killswitch
   memo consulted on a hot path costs a call every time it is consulted.  MEASURED: rt_defer_merge_on was 3.66% of
   pattern_bt's marginal profile, and rt_defer_probe_run below consulted it TWICE per call.  Sibling instances cured
   the same day: repl_pl_off (three reads per bn_replace), is_protected_pat_lead (0.80% of roman), _var_find_cached,
   sv_len.  ⭐ THE RULE: a control arm must be read ONCE into a local and carried -- never re-consulted inside the
   code it guards. */
static inline __attribute__((always_inline)) int rt_defer_merge_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_DEFER_MERGE"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ THE dfx FRAME IS UNOBSERVABLE ON THIS PATH, SO IT IS NOT BUILT (hq_P s260).  rt_defer_run_all pushed a dfx frame, stored the resolved value into it, and had c_rt_defer_close pop it straight back off -- and
   between the push and the pop NOTHING RUNS: no call out, no emitted code, no assembly.  The value never leaves the C frame, so the g_dfx round trip is pure ceremony.  Measured on roman.sno it was not cheap
   ceremony: rt_dfx_push'rt_defer_probe_run 2,376,245 Ir (3.78% of the whole program) plus the pop, the 24-byte struct copy and the top-of-stack check inside c_rt_defer_close.  ⛔ ONLY this path may skip it --
   every other caller of c_rt_defer_close can have emitted code or a procedure call between its push and its close, where the frame IS observable (rt_defer_step reads the top frame; rtx_match.S strides g_dfx by
   24), so c_rt_defer_close itself is untouched and the push/close balance elsewhere is exactly as it was.  ⭐ The llen==1 arm is the same argument one level down: a one-byte compare was calling __strncmp_avx2,
   1,069,308 Ir (1.70%), and a deferred single character is the common shape -- roman's deferred node is one digit. */
static int rt_defer_close_v(int cur_delta, DESCR_t val)
{
    if (IS_FAIL_fn(val)) return -1;
    char nb[40];
    if (val.v == DT_I) { snprintf(nb, sizeof nb, "%lld", (long long)val.i); val.v = DT_S; val.slen = (uint32_t)strlen(nb); val.s = nb; }
    else if (val.v == DT_R) { snprintf(nb, sizeof nb, "%g", val.r); val.v = DT_S; val.slen = (uint32_t)strlen(nb); val.s = nb; }
    if (val.v == DT_S || val.v == DT_SNUL) {
        const char *lit = val.s ? val.s : "";
        int llen = val.slen ? (int)val.slen : (int)strlen(lit);
        if (cur_delta + llen > Σlen) return -1;
        if (llen == 1) { if (Σ[cur_delta] != lit[0]) return -1; }
        else if (llen > 0 && strncmp(Σ + cur_delta, lit, (size_t)llen) != 0) return -1;
        return cur_delta + llen;
    }
    return -1;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_defer_run_all_v(const char *varname, int cur_delta, DESCR_t val)
{
    if (varname && varname[0] == 'F' && !strcmp(varname, "FAIL")) return -1;
    return rt_defer_close_v(cur_delta, val);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐⭐ SPITBOL'S vrblk DISCIPLINE FOR THE DEFERRED NAME (hq_P s261).  After the two-lookups-became-one cure, the ONE surviving resolution was still the largest runtime item in roman: NV_GET_fn'rt_defer_nv_read
   14.85% + __strcmp_avx2'NV_GET_fn 4.37% = 19.2%, 140,381 calls at ~74 Ir each.  callgrind's line annotation says the LOOKUP is not what costs -- the CEREMONY is: NV_GET_fn's prologue 1,214,530 Ir, a NON-INLINED
   _var_init() call 462,184 Ir to test one flag, the memo index 770,115, the memo key/generation check 1,271,736, and the strcmp that validates a memo hit 2,376,240.  You cannot make a lookup cheap enough; you
   have to stop doing it.  NV_PTR_fn already hands back the STABLE cell -- core.c's own memo comment carries the proof: an NV_t comes from rt_ws_alloc, a bump allocator whose cursor only advances, the GC marks
   HB_WS blocks but never moves or frees them, and NV_SET_fn REUSES an existing entry rather than shadowing it, so a resolved cell is valid for the life of the program.  That is exactly SPITBOL's static-area
   guarantee, and it is why sbl spends 3.97% in b_vra where we spent 19.2%.
   ⭐ THE SLOT IS SELF-VALIDATING, WHICH IS WHY A COLLISION CANNOT BECOME A WRONG ANSWER.  Each site gets a PAIR in the EXISTING g_sno_defer_cells array -- [0] the baked varname pointer that resolved, [1] the
   cell -- and the cached cell is used only when [0] still equals the varname this call was handed.  Two sites landing on one slot therefore MISS and re-resolve; they never hand each other a cell.  No new global:
   the array already exists for the DTP cache, and the DTP arm only allocates when ci >= 0 (a GVA-eligible name), which never fires for the PATV$ sites this arm serves.
   ⛔ NOT CACHED, and each for its own reason: an '&' name (rt_defer_nv_read has a keyword path NV_PTR_fn does not model), a NULL return (NV_PTR_fn refuses the reserved I/O and control names -- INPUT, OUTPUT,
   STLIMIT, ANCHOR, ...  whose value is COMPUTED, not stored), and site < 0 (the emitter had no slot to give). */
static DESCR_t rt_defer_cell_read(const char *varname, long site, int *ok)
{
    extern DESCR_t *NV_PTR_fn(const char *name);
    extern uint64_t g_sno_defer_cells[4096];
    *ok = 0;
    if (site < 0 || site >= 1024 || !varname || varname[0] == '&' || varname[0] == '*') return NULVCL;
    uint64_t *slot = &g_sno_defer_cells[2048 + site * 2];
    DESCR_t *cell;
    if (slot[0] == (uint64_t)(uintptr_t)varname) cell = (DESCR_t *)(uintptr_t)slot[1];
    else { cell = NV_PTR_fn(varname); if (!cell) return NULVCL; slot[0] = (uint64_t)(uintptr_t)varname; slot[1] = (uint64_t)(uintptr_t)cell; }
    *ok = 1;
    return *cell;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_defer_pr_t rt_defer_probe_run(const char *varname, int cur_delta, long site)
{
    extern void *dtp_fn_of(void *);
    rt_defer_pr_t r; r.fn = (void *)0; r.aux = 0;
    const int _merge = rt_defer_merge_on();
    if (_merge && varname && varname[0] != '*') {
        int ok = 0; DESCR_t cv = rt_defer_cell_read(varname, site, &ok);
        if (ok && cv.v != DT_P && cv.v != DT_X) { r.aux = (long)rt_defer_run_all_v(varname, cur_delta, cv); return r; }
    }
    if (!_merge || !varname || varname[0] == '*') {
        void *dtp = rt_defer_get_pat_dtp(varname, 0);
        if (dtp) { void *fn = *(void **)dtp; if (fn) { r.fn = fn; r.aux = (long)(uintptr_t)dtp; return r; } }
        r.aux = (long)rt_defer_run_all(varname, cur_delta); return r;
    }
    DESCR_t val = rt_defer_nv_read(varname);
    if (val.v == DT_P && val.p) { dtp_fn_of(val.p); void *fn = *(void **)val.p; if (fn) { r.fn = fn; r.aux = (long)(uintptr_t)val.p; return r; } r.aux = (long)rt_defer_run_all(varname, cur_delta); return r; }
    if (val.v == DT_X) { void *dtp = rt_defer_xpat_on() ? rt_defer_xpat_dtp(val.s) : (void *)0; if (dtp) { void *fn = *(void **)dtp; if (fn) { r.fn = fn; r.aux = (long)(uintptr_t)dtp; return r; } } r.aux = (long)rt_defer_run_all(varname, cur_delta); return r; }
    r.aux = (long)rt_defer_run_all_v(varname, cur_delta, val);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_match_value_get_pat_fn(DESCR_t *pval)
{
    if (pval && pval->v == DT_P && pval->p) { extern void *dtp_fn_of(void *); return dtp_fn_of(pval->p); }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_match_value_get_pat_dtp(DESCR_t *pval)
{
    if (pval && pval->v == DT_P && pval->p) { extern void *dtp_fn_of(void *); dtp_fn_of(pval->p); return pval->p; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_match_value_open(DESCR_t *pval)
{
    rt_dfx_t *s = rt_dfx_push(); if (!s) return 0;
    DESCR_t val = pval ? *pval : NULVCL;
    if (IS_FAIL_fn(val)) { s->failed = 1; return 0; }
    s->val = val;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_subscript_var(DESCR_t base, DESCR_t idx) {
    DESCR_t bvar = base;
    if (IS_VARREF_fn(base)) base = rt_deref(base);
    if (base.v == DT_A) {
        ARBLK_t *a = base.arr; if (!a) return FAILDESCR;
        int i = (int)to_int(idx); int off = i - a->lo;
        if (off < 0 || off >= (a->hi - a->lo + 1)) return FAILDESCR;
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = &a->data[off]; vc->tbl = 0; vc->key = 0; vc->key_d = idx; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
        return NAMETRAP(vc);
    }
    if (base.v == DT_T) {
        TBBLK_t *tb = base.tbl; if (!tb) return FAILDESCR;
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
        vc->cellp = 0; vc->tbl = tb; vc->key = 0;   /*⛔⭐ s262 Lon: NO CODE MAY HOLD AN ADDRESS INTO A TABLE.  cellp used to be &e->val, which forced entries to be pinned against a compacting collector; the cell names (tbl, key_descr) and re-resolves instead -- and that also applies the table's DEFAULT on a miss, which the raw pointer never did.  ⭐ IT ALSO DELETED A WHOLE LOOKUP: this arm used to call table_find_pair_d purely to compute that address, so minting a subscript and then reading it hashed TWICE.  Measured at fixed work, 100,000 reads: table_find_pair_d 51.0M Ir with the probe, 38.7M without. */
        vc->key_d = idx; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
        return NAMETRAP(vc);
    }
    if (base.v == DT_DATA) {
        DESCR_t *elems; int n;
        if (rt_list_view(base, &elems, &n)) {
            int i = (int)to_int(idx);
            if (i < 0) i = n + i + 1;
            if (!elems || i < 1 || i > n) return FAILDESCR;
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = &elems[i - 1]; vc->tbl = 0; vc->key = 0; vc->key_d = idx; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            return NAMETRAP(vc);
        }
        return subscript_get(base, idx);
    }
    if ((base.v == DT_S || base.v == DT_SNUL) && IS_VARREF_fn(bvar)) {
        const char *sp = base.s ? base.s : ""; long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        long i = (long)to_int(idx);
        if (i <= 0) i = slen + 1 + i;
        if (i < 1 || i > slen) return FAILDESCR;
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = 0; vc->key = 0; vc->key_d = idx; vc->sv = bvar; vc->pos = i; vc->len = 1;
        return NAMETRAP(vc);
    }
    return subscript_get(base, idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐⭐ THE MISS ARM, KEPT IN C ON PURPOSE (hq_P s266).  rtx_table.S owns the exported rt_subscript_var_container_only and tail-jumps here when the hashed lookup misses.  It stays C
  because the two things it needs -- TBBLK_t.dflt's offset and the exact bit pattern of NULVCL (which is DT_SNUL plus a pointer to a "" literal, NOT a zero word) -- are the kind of
  constant that an .S file can only COPY, and a copied constant is a drift hazard with no build-time witness.  One call on the miss path buys both from the C of record. */
DESCR_t c_rt_svco_miss_d(TBBLK_t *tb) {
    if (!tb) return FAILDESCR;
    if (tb->dflt.v != DT_FAIL && tb->dflt.v != 0) return tb->dflt;
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_subscript_var_container_only(DESCR_t base, DESCR_t idx) {
    extern int kwb_error(int code, const char *msg);
    DESCR_t b = base;
    if (IS_VARREF_fn(b)) b = rt_deref(b);
    if (b.v != DT_A && b.v != DT_T) { kwb_error(235, "subscripted operand is not table or array"); return FAILDESCR; }
    if (b.v == DT_T) {
        TBBLK_t *tb = b.tbl; if (!tb) return FAILDESCR;
        /*⭐⭐ RETURNS THE VALUE, AND ALLOCATES NOTHING (hq_P s264).  This arm used to look the key up, and then -- unless the hit was a nested TABLE/ARRAY -- allocate a
           VCELL_t on the GC heap, fill seven fields, and hand back a NAMETRAP.  The caller's very next emitted instruction is rt_deref, which walked straight into
           rt_deref_slow's `if (vc->tbl)` arm and LOOKED THE SAME KEY UP A SECOND TIME to get the value out.  One read of one table cost two hashed lookups, one heap
           allocation and a GC root.  MEASURED on claws5: ~2.9 of the 14 container-only subscripts per token took that path, ~740 Ir each in lookup + alloc + slow deref.
           ⛔ WHY RETURNING A BARE VALUE IS SAFE HERE, AND THE PROOF IS IN THE LOWERER, NOT IN THIS FILE.  `container-only` is set at exactly two sites in
           lower_snobol4.c: line 392, every link of an RVALUE chain, and line 860, `if (k < nidx - 1)` -- the NON-FINAL links of an LVALUE chain.  The final link of an
           lvalue chain is lowered to rt_subscript_var, which still builds the assignable nametrap.  So NOTHING IS EVER ASSIGNED THROUGH THIS FUNCTION'S RESULT, and the
           nametrap it used to build could only ever be deref'd. ⛔ If a third `container-only` site is ever added, it must obey that same rule or this is wrong.
           ⭐ The three outcomes reproduce rt_deref_slow's table arm EXACTLY -- hit, then the table's own default, then the null string -- so a chain that reads a missing
           intermediate still yields NULVCL and still lands on the same kwb_error(235) at the next link. */
        { TBPAIR_t *e = table_find_pair_d(tb, idx);
          if (e) return e->val;
          if (tb->dflt.v != DT_FAIL && tb->dflt.v != 0) return tb->dflt;
          return NULVCL; }
    }
    return rt_subscript_var(base, idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐ row perf-table-subscript-fastpath lever 2 (seat12, 2026-08-27): T[I]=v (SNOBOL4 shape, base already deref'd) fuses the mint+store
   round trip. RTX-31/RTX-NEW-ICNVAR (2026-08-24, this row's own lever 1) already proved that for a DT_T base the mint
   (rt_subscript_var) does NO lookup -- it is seven VCELL field stores, cellp=0/tbl=tb/key_d=idx -- and rt_assign_var's own
   .Lav_table_store arm immediately unpacks exactly those two fields back out and calls table_set_descr_d(tbl,key_d,val)
   UNCHANGED. So for this one shape the heap-allocated VCELL and both call/ret boundaries around it are pure overhead: this
   function calls table_set_descr_d directly with no allocation and no intermediate descriptor.
   PRECONDITION, ENFORCED BY THE CALLER, NOT RE-CHECKED HERE (bb_assign_var_sub.cpp): base.v==DT_T and base.tbl!=0 -- the
   template tests both INLINE (cmp dil,DT_T / test rsi,rsi) before ever calling this function, so every other base shape
   (array, DATA, string substring, a VARREF base, a null table) never reaches here at all -- it calls rt_subscript_var then
   rt_assign_var directly from the template instead (see that file's own header for why: an earlier version of this
   function did that fallback itself, which added a third wrapper call frame around the same two calls for every non-table
   write and regressed array_sum.sno's Ir count, caught by test_gate_instr_budget.sh). The one thing this function still
   checks at runtime is g_gc_pending -- the same provably-safe window rt_assign_var's own asm entry gates its fast arms on
   (rtx_icnvar.S:72-75): a pending collection could relocate base/idx/val before table_set_descr_d runs, and that window is
   cheap to fall back out of (base is already known to be a table, so the two-call chain below is doing real, needed work,
   not wrapper overhead). rt_sxt_break is replicated for a DT_S val because every other assignment path (aggregates.c:425,
   pattern_match.c c_rt_assign_var_body:1518, core.c twice) calls it unconditionally before the store -- it is a general
   pre-assignment hook, not something specific to the nametrap/cellp road this function bypasses. Killswitch
   SCRIP_SUBASSIGN_FUSE (default on) lives in the lowerer (lower_snobol4.c), which is what decides whether IR_ASSIGN_VAR
   ever carries 3 operands at all -- when it is off, this function is simply dead code, never called. */
DESCR_t c_rt_table_assign_fast(DESCR_t base, DESCR_t idx, DESCR_t val) {
    extern int g_gc_pending;
    if (g_gc_pending) {
        DESCR_t ref = rt_subscript_var(base, idx);
        if (ref.v == DT_FAIL) return ref;
        return rt_assign_var(ref, val);
    }
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    table_set_descr_d(base.tbl, idx, val); return val;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐ row `table-int-keys-and-nd-subscript` defect (2): a[i,j] on a plain 2-D array lowered to TWO chained single-index
   dispatches (aggregates.c:59-73's array_get2/array_set2 row-major formula existed but nothing in codegen ever called
   it). GUARD DELIBERATELY NARROW, the standard RTX shape: only a deref'd DT_A base with ndim==2 and both indices DT_I,
   in-bounds, takes this path; anything else (tables, nested containers, wrong index type, out-of-range) falls through
   to the exact pre-existing two-hop chain below, byte-identical to before this row. */
static int rt_nd2_fast(DESCR_t base, DESCR_t idx1, DESCR_t idx2, DESCR_t *out) {
    DESCR_t b = base; if (IS_VARREF_fn(b)) b = rt_deref(b);
    if (b.v != DT_A || !b.arr || b.arr->ndim != 2 || idx1.v != DT_I || idx2.v != DT_I) return 0;
    ARBLK_t *a = b.arr; int i = (int)idx1.i, j = (int)idx2.i, cols = a->hi2 - a->lo2 + 1, row = i - a->lo, col = j - a->lo2;
    if (row < 0 || row >= (a->hi - a->lo + 1) || col < 0 || col >= cols) return 0;
    int off = row * cols + col, total = (a->hi - a->lo + 1) * cols; if (off < 0 || off >= total) return 0;
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = &a->data[off]; vc->tbl = 0; vc->key = 0; vc->key_d = idx2; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
    *out = NAMETRAP(vc); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_subscript_var2(DESCR_t base, DESCR_t idx1, DESCR_t idx2) {
    DESCR_t out; if (rt_nd2_fast(base, idx1, idx2, &out)) return out;
    DESCR_t hop1 = rt_subscript_var_container_only(base, idx1);
    if (hop1.v == DT_FAIL) return FAILDESCR;
    return rt_subscript_var_container_only(hop1, idx2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_subscript_var2_lv(DESCR_t base, DESCR_t idx1, DESCR_t idx2) {
    DESCR_t out; if (rt_nd2_fast(base, idx1, idx2, &out)) return out;
    DESCR_t hop1 = rt_subscript_var_container_only(base, idx1);
    if (hop1.v == DT_FAIL) return FAILDESCR;
    return rt_subscript_var(hop1, idx2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_field_var(const char *fname, DESCR_t obj) {
    extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
    if (IS_VARREF_fn(obj)) obj = rt_deref(obj);
    if (obj.v != DT_DATA || !obj.u) return FAILDESCR;
    DESCR_t *cell = data_field_ptr(fname ? fname : "", obj);
    if (!cell) return FAILDESCR;
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = cell; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
    { const char *rn = (obj.u && obj.u->type && obj.u->type->name) ? obj.u->type->name : "record"; const char *fn = fname ? fname : ""; int rl = (int)strlen(rn); int fl = (int)strlen(fn); char *nb = rt_str_alloc(rl + fl + 1); memcpy(nb, rn, rl); nb[rl] = '.'; memcpy(nb + rl + 1, fn, fl); nb[rl + 1 + fl] = 0; vc->key = nb; }
    return NAMETRAP(vc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_list_bang_var_at(DESCR_t obj, int64_t idx) {
    DESCR_t bvar = obj;
    if (IS_VARREF_fn(obj)) obj = rt_deref(obj);
    if (obj.v == DT_DATA) {
        DESCR_t *elems; int n;
        if (rt_list_view(obj, &elems, &n)) {
            if (!elems || idx < 0 || idx >= n) return FAILDESCR;
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = &elems[idx]; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            return NAMETRAP(vc);
        }
        if (obj.u && obj.u->type && obj.u->type->nfields > 0) {
            int nf = obj.u->type->nfields;
            if (idx < 0 || idx >= nf) return FAILDESCR;
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = &obj.u->fields[idx]; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            { const char *rn = obj.u->type->name ? obj.u->type->name : "record"; const char *fn = (obj.u->type->fields && obj.u->type->fields[idx]) ? obj.u->type->fields[idx] : ""; int rl = (int)strlen(rn); int fl = (int)strlen(fn); char *nb = rt_str_alloc(rl + fl + 1); memcpy(nb, rn, rl); nb[rl] = '.'; memcpy(nb + rl + 1, fn, fl); nb[rl + 1 + fl] = 0; vc->key = nb; }
            return NAMETRAP(vc);
        }
        return FAILDESCR;
    }
    if (obj.v == DT_T && obj.tbl) {
        TBBLK_t *tbl = obj.tbl; int64_t seen = 0; TBPAIR_t *ep;
        TBL_FOREACH(tbl, ep) {
                if (seen == idx) {
                    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = tbl; vc->key = 0; vc->key_d = ep->key_descr; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;   /*⛔ s262 Lon: NO ADDRESS INTO A TABLE */
                    return NAMETRAP(vc);
                }
                seen++;
            }
        return FAILDESCR;
    }
    if ((obj.v == DT_S || obj.v == DT_SNUL) && IS_VARREF_fn(bvar)) {
        const char *sp = obj.s ? obj.s : ""; long slen = obj.slen ? (long)obj.slen : (long)strlen(sp);
        if (idx < 0 || idx >= slen) return FAILDESCR;
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = bvar; vc->pos = idx + 1; vc->len = 1;
        return NAMETRAP(vc);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_random_var(DESCR_t base) {
    extern long g_random;
    DESCR_t bvar = base;
    if (IS_VARREF_fn(base)) base = rt_deref(base);
    g_random = (1103515245L * g_random + 453816694L) & 0x7FFFFFFFL; double rval = 4.65661286e-10 * (double)g_random;
    if (base.v == DT_S && base.slen == 0xFFFFFFFFu) {
        const char *cp; int clen; if (!cset_resolve(base, &cp, &clen) || clen <= 0) return FAILDESCR;
        long i = (long)(rval * (double)clen); char *one = rt_str_alloc(1); one[0] = cp[i]; one[1] = 0;
        return (DESCR_t){ .v = DT_S, .slen = 1, .s = one };
    }
    if ((base.v == DT_S || base.v == DT_SNUL) && IS_VARREF_fn(bvar)) {
        const char *sp = base.s ? base.s : ""; long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        if (slen <= 0) return FAILDESCR;
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = bvar; vc->pos = (long)(rval * (double)slen) + 1; vc->len = 1;
        return NAMETRAP(vc);
    }
    if (base.v == DT_S || base.v == DT_SNUL) {
        const char *sp = base.s ? base.s : ""; long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        if (slen <= 0) return FAILDESCR;
        long i = (long)(rval * (double)slen); char *one = rt_str_alloc(1); one[0] = sp[i]; one[1] = 0;
        return (DESCR_t){ .v = DT_S, .slen = 1, .s = one };
    }
    if (base.v == DT_DATA) {
        DESCR_t *elems; int n;
        if (rt_list_view(base, &elems, &n)) {
            if (!elems || n <= 0) return FAILDESCR;
            long i = (long)(rval * (double)n);
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = &elems[i]; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            return NAMETRAP(vc);
        }
        if (base.u && base.u->type && base.u->type->nfields > 0) {
            int nf = base.u->type->nfields;
            long i = (long)(rval * (double)nf);
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = &base.u->fields[i]; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
            return NAMETRAP(vc);
        }
        return FAILDESCR;
    }
    if (base.v == DT_T && base.tbl) {
        TBBLK_t *tbl = base.tbl; if (tbl->size <= 0) return FAILDESCR;
        long n = (long)(rval * (double)tbl->size) + 1; long seen = 0; TBPAIR_t *ep;
        TBL_FOREACH(tbl, ep)
                if (++seen == n) {
                    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = tbl; vc->key = rt_ws_strdup_c(ep->key); vc->key_d = ep->key_descr; vc->sv = FAILDESCR; vc->pos = 0;
                        vc->len = 0;
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
    if (IS_VARREF_fn(base)) base = rt_deref(base);
    if ((base.v == DT_S || base.v == DT_SNUL) && IS_VARREF_fn(bvar)) {
        const char *sp = base.s ? base.s : ""; long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        long ii = (long)to_int(i1d), jj = (long)to_int(i2d);
        if (ii < -slen || ii > slen + 1) return FAILDESCR;
        if (jj < -slen || jj > slen + 1) return FAILDESCR;
        if (ii <= 0) ii = slen + ii + 1;
        if (jj <= 0) jj = slen + jj + 1;
        if (ii > jj) { long t = ii; ii = jj; jj = t; }
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = 0; vc->tbl = 0; vc->key = 0; vc->key_d = i1d; vc->sv = bvar; vc->pos = ii; vc->len = jj - ii;
        return NAMETRAP(vc);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_var_ref_cell(DESCR_t *cellp) {
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = cellp; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
    return NAMETRAP(vc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_deref_slow(DESCR_t d) {
    if (d.v == DT_N && d.slen == 0 && d.s && *d.s) { extern DESCR_t NV_GET_fn(const char *); return NV_GET_fn(d.s); }
    if (d.v == DT_N && d.slen == 1 && d.ptr) return *(DESCR_t *)d.ptr;
    if (!IS_NAMETRAP_fn(d)) return d;
    VCELL_t *vc = (VCELL_t *)d.p; if (!vc) return FAILDESCR;
    if (vc->cellp) return *vc->cellp;
    if (vc->tbl) {
        int found; DESCR_t hit = table_get_found_d(vc->tbl, vc->key_d, &found);
        if (found) return hit;
        if (vc->tbl->dflt.v != DT_FAIL && vc->tbl->dflt.v != 0) return vc->tbl->dflt;
        return NULVCL;
    }
    if (IS_VARREF_fn(vc->sv)) {
        DESCR_t sd = rt_deref(vc->sv);
        if (sd.v != DT_S && sd.v != DT_SNUL) return FAILDESCR;
        const char *sp = sd.s ? sd.s : ""; long slen = sd.slen ? (long)sd.slen : (long)strlen(sp);
        if (vc->pos + vc->len - 1 > slen) return FAILDESCR;
        char *out = rt_str_alloc(vc->len); memcpy(out, sp + vc->pos - 1, (size_t)vc->len); out[vc->len] = 0;
        return (DESCR_t){ .v = DT_S, .slen = (uint32_t)vc->len, .s = out };
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t c_rt_assign_var_body(DESCR_t var, DESCR_t val) {
    { DESCR_t sh[2]; sh[0] = var; sh[1] = val; rt_gc_point_arr(sh, 2, (const char **)0); var = sh[0]; val = sh[1]; }
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    if (var.v == DT_N && var.slen == 0 && var.s && *var.s) { extern DESCR_t NV_SET_fn(const char *, DESCR_t); NV_SET_fn(var.s, val); return val; }
    if (var.v == DT_N && var.slen == 1 && var.ptr) { extern void mon_tap_cell_store(void *, DESCR_t); *(DESCR_t *)var.ptr = val; if (monitor_fd >= 0) mon_tap_cell_store(var.ptr, val); return val; }
    if (!IS_NAMETRAP_fn(var)) {
        fprintf(stderr, "[IDX] BOMB rt_assign_var: lvalue is not a variable (dtype=%d) — string/record subscript assignment is the tvsubs rung (GOAL-IR-IMMUTABLE-EMIT IDX-UNIFY)\n", (int)var.v);
        abort();
    }
    VCELL_t *vc = (VCELL_t *)var.p; if (!vc) return FAILDESCR;
    if (vc->cellp) { extern void mon_tap_cell_store(void *, DESCR_t); *vc->cellp = val; if (monitor_fd >= 0) mon_tap_cell_store((void *)vc->cellp, val); return val; }
    if (vc->tbl) { table_set_descr_d(vc->tbl, vc->key_d, val); return val; }
    if (IS_VARREF_fn(vc->sv)) {
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
        char *ns = rt_str_alloc(nlen);
        memcpy(ns, sp, (size_t)prelen); memcpy(ns + prelen, src, (size_t)srclen); memcpy(ns + prelen + srclen, sp + poststrt, (size_t)(slen - poststrt)); ns[nlen] = 0;
        DESCR_t nsd = (DESCR_t){ .v = DT_S, .slen = (uint32_t)nlen, .s = ns };
        DESCR_t wr = rt_assign_var(vc->sv, nsd);
        if (wr.v == DT_FAIL) return FAILDESCR;
        vc->len = srclen;
        char *rs = rt_str_alloc(srclen); memcpy(rs, src, (size_t)srclen); rs[srclen] = 0;
        return (DESCR_t){ .v = DT_S, .slen = (uint32_t)srclen, .s = rs };
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_assign_var(DESCR_t var, DESCR_t val)
{
    int simple = (var.v == DT_N && var.slen == 0 && var.s && *var.s);
    DESCR_t r = c_rt_assign_var_body(var, val);
    if (!simple && g_monitor_bin && !IS_FAIL_fn(r)) mon_emit_value_bin("<lval>", val);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static VCELL_t * vcell_ultimate(DESCR_t d) {
    while (IS_NAMETRAP_fn(d)) { VCELL_t *vc = (VCELL_t *)d.p; if (!vc) return 0; if (IS_NAMETRAP_fn(vc->sv)) { d = vc->sv; continue; } return vc; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_swap_var(DESCR_t va, DESCR_t vb) {
    if (!IS_VARREF_fn(va) || !IS_VARREF_fn(vb)) return FAILDESCR;
    if (!IS_NAMETRAP_fn(va) || !IS_NAMETRAP_fn(vb)) {
        DESCR_t dx = rt_deref(va), dy = rt_deref(vb);
        { extern void rt_sxt_break(const char *); if (dx.v == DT_S) rt_sxt_break(dx.s); if (dy.v == DT_S) rt_sxt_break(dy.s); }
        if (dx.v == DT_FAIL || dy.v == DT_FAIL) return FAILDESCR;
        if (rt_assign_var(va, dy).v == DT_FAIL) return FAILDESCR;
        if (rt_assign_var(vb, dx).v == DT_FAIL) return FAILDESCR;
        return rt_deref(va);
    }
    VCELL_t *xc = (VCELL_t *)va.p, *yc = (VCELL_t *)vb.p; if (!xc || !yc) return FAILDESCR;
    DESCR_t dx = rt_deref(va), dy = rt_deref(vb);
    { extern void rt_sxt_break(const char *); if (dx.v == DT_S) rt_sxt_break(dx.s); if (dy.v == DT_S) rt_sxt_break(dy.s); }
    if (dx.v == DT_FAIL || dy.v == DT_FAIL) return FAILDESCR;
    long adj1 = 0, adj2 = 0;
    if (IS_NAMETRAP_fn(xc->sv) && IS_NAMETRAP_fn(yc->sv)) {
        VCELL_t *ux = vcell_ultimate(xc->sv), *uy = vcell_ultimate(yc->sv);
        int same_slot = 0;
        if (ux && uy) {
            if (ux->cellp && ux->cellp == uy->cellp) same_slot = 1;
            else if (ux->tbl && ux->tbl == uy->tbl) {
                char k1[64], k2[64];
                const char *s1 = ux->key ? ux->key : tbl_key_str(ux->key_d, k1, sizeof k1);
                const char *s2 = uy->key ? uy->key : tbl_key_str(uy->key_d, k2, sizeof k2);
                if (s1 && s2 && !strcmp(s1, s2)) same_slot = 1;
            }
        }
        if (same_slot) {
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_zcol_push(void ** ptr_cell, int * cap_cell, int i, long elem_sz)
{
    extern void rt_bomb(const char *);
    if (i + 1 > *cap_cell) {
        int nc = *cap_cell > 0 ? *cap_cell : 4;
        while (nc < i + 1) nc *= 2;
        char * op = (char *)*ptr_cell;
        char * np = (char *)realloc(op, (size_t)nc * (size_t)elem_sz);
        if (!np) rt_bomb("rt_zcol_push: collection realloc failed");
        memset(np + (size_t)*cap_cell * (size_t)elem_sz, 0, (size_t)(nc - *cap_cell) * (size_t)elem_sz);
        *ptr_cell = np; *cap_cell = nc;
    }
    char * e = (char *)*ptr_cell + (size_t)i * (size_t)elem_sz;
    memset(e, 0, (size_t)elem_sz);
    return e;
}
