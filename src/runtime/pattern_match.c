#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "dtp.h"
#include "core.h"
#include "ast.h"
#include "../parser/snobol4/scrip_cc.h"
#include "sil_macros.h"
#include "builtins/gen_runtime.h"
#include "rt/gc_heap.h"
#include "rt/rt_arena.h"
#include "zeta_choices.h"
#define STACKLESS_ABORT(fn) \
    do { fprintf(stderr, "libscrip_rt: %s called — Icon value stack removed (GROUND ZERO 3). " \
                         "This box must be rebuilt stackless (per-box slot, no value stack).\n", (fn)); \
         abort(); } while (0)
DESCR_t (*g_eval_str_hook)(const char *s) = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DATBLK_t *g_lf_type;
static inline int rt_list_view(DESCR_t o, DESCR_t **elems, int *n) {
    if (o.v != DT_DATA || !o.u) return 0;
    DATBLK_t *t = o.u->type;
    if (t != g_lf_type) { if (!t || t->nfields < 3 || !t->fields[0] || strcmp(t->fields[0], "frame_elems") != 0) return 0; g_lf_type = t; }
    DESCR_t gt = o.u->fields[2]; if (gt.v != DT_S || !gt.s || strcmp(gt.s, "list") != 0) return 0;
    DESCR_t ea = o.u->fields[0]; *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL; *n = (int)o.u->fields[1].i;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct dtp_rcp { int tt; const char *s; uint32_t slen; int64_t ival; struct dtp_rcp *l; struct dtp_rcp *r; } dtp_rcp_t;
typedef struct DTP { void *fn; dtp_rcp_t *rcp; int64_t zsz; int32_t zstatic; int32_t zpad; DESCR_t *snap; int64_t nsnap; } DTP_t;   /* PS-1 (s150): zsz = per-activation frame bytes stamped at compile (0=unknown); zstatic = 1 iff blob graph has no DEFER/VALUE nodes (extent sound for ARBNO frame arithmetic); fields APPENDED — fn stays offset 0, future asm consumers read zsz at [p+16] */   /* PB-1s (s108): snap/nsnap = PER-CONSTRUCTION value snapshot (manual p.85-86: each construction freezes ITS OWN values) — filled by rt_patv_freeze at SNO$MKPAT from the per-site PAT$n$V<i> globals, read by the blob's $V slot arm via [rbp-24]->snap[i] at [p+32]/[p+40]; rt_ws_alloc island block = immortal + conservatively root-scanned every collect, so the held DESCRs (which can be patterns holding patterns) are GC roots by construction */
_Static_assert(__builtin_offsetof(DTP_t, fn) == 0, "bb_match_defer inline cache reads DTP_t.fn at offset 0");
_Static_assert(__builtin_offsetof(DTP_t, zsz) == 16, "PS-3 ARBNO stride latch reads DTP_t.zsz at offset 16");
static int pstamp_trace(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_PSTAMP_TRACE"); v = e ? (atoi(e) != 0) : 0; } return v; }
static DTP_t *dtp_new(void *fn, dtp_rcp_t *rcp) { DTP_t *h = (DTP_t *)rt_ws_alloc(sizeof(DTP_t)); h->fn = fn; h->rcp = rcp; h->zsz = 0; h->zstatic = 0; h->zpad = 0; h->snap = 0; h->nsnap = 0; return h; }
void *dtp_wrap_fn(void *fn) { return (void *)dtp_new(fn, (dtp_rcp_t *)0); }
void *dtp_wrap_fn_sz(void *fn, int64_t zsz, int32_t zstatic) { DTP_t *h = dtp_new(fn, (dtp_rcp_t *)0); h->zsz = zsz; h->zstatic = zstatic; if (pstamp_trace()) fprintf(stderr, "PSTAMP wrap fn=%p zsz=%lld zstatic=%d\n", fn, (long long)zsz, (int)zstatic); return (void *)h; }
int64_t dtp_zsz_of(void *headv) { DTP_t *h = (DTP_t *)headv; return h ? h->zsz : 0; }
int dtp_zstatic_of(void *headv) { DTP_t *h = (DTP_t *)headv; return h ? (int)h->zstatic : 0; }
static dtp_rcp_t *rcp_node(int tt, const char *s, uint32_t n, int64_t iv, dtp_rcp_t *l, dtp_rcp_t *rr) { dtp_rcp_t *r = (dtp_rcp_t *)rt_ws_alloc(sizeof *r); r->tt = tt; r->s = s; r->slen = n; r->ival = iv; r->l = l; r->r = rr; return r; }
static dtp_rcp_t *rcp_lit(const char *s, uint32_t n) { return rcp_node(TT_QLIT, s ? s : "", n, 0, 0, 0); }
static dtp_rcp_t *rcp_bin(int tt, dtp_rcp_t *l, dtp_rcp_t *rr) { return rcp_node(tt, 0, 0, 0, l, rr); }
static dtp_rcp_t *rcp_of(DESCR_t d) {
    if (d.v == DT_P && d.p) {
        DTP_t *h = (DTP_t *)d.p;
        if (h->rcp) return h->rcp;
        /* S-C (s104): a recipe-less compiled DT_P (SNO$MKPAT patproc product) composes via a minted-global
         * DEFER — the ARB$ self-reference trick one function down, generalized.  The defer resolves through
         * rt_defer_get_pat_fn -> dtp_fn_of -> h->fn at match time and rides the S-A suspend-gamma protocol;
         * the nested-blob-defer class this arm previously refused (124/143/147) landed with that fix. */
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
        char kb[64]; const char *ks = tbl_key_str(idx, kb, sizeof kb);
        int found; DESCR_t hit = table_get_found(arr.tbl, ks, &found);
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
        char kb[64];
        table_set_descr(arr.tbl, tbl_key_str(idx, kb, sizeof kb), idx, val);
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
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ s112 MON-CAP — THE SUBSCRIPTED-LVALUE VALUE TAP.  MEASURED on beauty.sno: the sync-step monitor's FIRST divergence (step 49, global.inc:29 `UTF[CHAR(194) CHAR(160)] = 'NO_BREAK_SPACE'`) was the oracle
 * emitting `VALUE <lval> = STRING(14)='NO_BREAK_SPACE'` where SCRIP emitted NOTHING and ran straight on to the next LABEL.  IT IS NOT A PROGRAM DEFECT — the discriminating probe stores and reads back
 * table AND array subscripts oracle-identically; SCRIP was simply DARK for this assignment class, which RULES.md names explicitly ("blind to the divergence CLASS ... reinstating/extending it comes
 * FIRST").  The simple-name taps live on the NV_SET family in rt.c; a subscripted store never passes through NV_SET, so it had no tap anywhere.  SPELLING IS THE ORACLE'S, NOT INVENTED: `<lval>` is
 * verbatim what the x64 `sbl` fire-point prints for a non-simple target (measured on a 4-form probe covering table, array, simple and indirect targets).  Wrapped rather than patched at each `return`
 * so every success path taps exactly once and the arms stay ONE AUTHORITY — `_body` keeps the whole original decision tree byte-for-byte.  Fires only under g_monitor_bin: zero cost when dark. */
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val) { int ok = subscript_set_body(arr, idx, val); if (ok && g_monitor_bin) mon_emit_value_bin("<lval>", val); return ok; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t subscript_get2_ext(DESCR_t arr, DESCR_t i, DESCR_t end) {
    long ii = (long)to_int(i), ee = (long)to_int(end);
    if (((-ii) ^ (-ee)) < 0) return FAILDESCR;
    return subscript_get2(arr, i, end);
}
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
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    const char **keys = rt_ws_alloc(n * sizeof(char *));
    DESCR_t *key_descrs = rt_ws_alloc(n * sizeof(DESCR_t));
    DESCR_t *vals = rt_ws_alloc(n * sizeof(DESCR_t));
    int idx = 0;
    for (int h = 0; h < TABLE_BUCKETS; h++)
        for (TBPAIR_t *e = tbl->buckets[h]; e; e = e->next) {
            keys[idx] = e->key;
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
/* ___-dcap (2026-07-13, Lon directive: "the conditional assignment capture should not call a C function when
 * all it must do is increment ___ and decrement ___ in the capture BB").  The bounded ring (RT_DCAP_MAX 32) +
 * mark/depth arrays + active flag are DELETED; the pend stack is an UNBOUNDED register-anchored island (the
 * rt_gva_island precedent, ARCH-ZETA §12) whose live cursor is REGISTER ___ in emitted code.  Entry = 24B,
 * pointer-free into Region 2 (varname is sealed RO strtab; saved_delta resolved against the subject at flush
 * via the pump ctx, never stored as a pointer — no GC root, no ADJUST entry).  Layout: varname@+0,
 * saved_delta@+8 (zero-extended u32), len@+16 (zero-extended u32).  Future entry species (SZ-3's *FN() commit
 * chains — the Python engine's one-cstack uniformity) discriminate on the varname qword, as '*' names do
 * today.  Push = box-inline stores + add ___,24 (capture γ).  Pop = sub ___,24 (capture β) — balanced by
 * generator LIFO scoping (the s45 Python insight; snobol4python Δ.γ append/yield/pop).  MARK = per-match-head
 * frame qword (head α saves ___; nesting = ζ frames nest; the depth array dies).  Head-fail/release restore
 * ___=mark AND g_dcap_top=mark inline.  g_dcap_top is the MIRROR: match-head α loads its cursor from it, and
 * every mid-match transfer window in a MATCH-FAMILY box mirrors ___ out first so a nested graph's heads see
 * the live top (a stale-low mirror would let nested pushes overwrite live pends).  Non-SNOBOL graphs never
 * touch ___ or the mirror (mirror-out sites are IR-kind-conditioned, per the no-language-sentinel FACT RULE).
 * KNOWN INHERITED LIMIT (status quo ante, the old shared ring had it too): a generator that SUSPENDS mid-match
 * with pends live shares the one stack non-LIFO — the suspended-ζ residue class, NCB-2/TR-6 territory. */
/* CAS-1 (Lon directive s60: "SNOBOL4 will have a CONDITIONAL ASSIGN stack in separate mmap"; GOAL-SNOBOL4-BB.md
 * RUNG GC-U).  The conditional-assign machinery's DESCR-BEARING side stacks — the *FN() commit-value stack
 * (g_capx), the deferred-function frame stack (g_dfx) and the pump's re-entrant cursor stack (g_dcf) — move off
 * libc realloc onto ONE base-pinned island in the rt_gva_island / g_dcap class (ARCH-ZETA §12): reserved once,
 * carved once, NEVER moved, NEVER freed, NEVER slid.  WHY THIS IS A GC RUNG AND NOT HYGIENE: these three hold
 * live DESCR_t — pointers into the collected workspace — and malloc'd memory was never GC-scanned (the TR-2
 * lesson, which cost a GC_add_roots compensation there).  ⛔ THE NEXT SENTENCE WAS FALSE AND IS CORRECTED (RC-8a / HOME-RBX X-1, s33): this banner used to read "on the island they are covered by
 * RT_SLAB_GC_ROOTS today". They were NOT. RT_SLAB_GC_ROOTS is #defined 0 (rt_slab.h:14) and gates ZERO `#if` bodies tree-wide — it was a TR-3 compensation and died at TR-4,
 * taking
 * the coverage with it and leaving only the claim; rt_cas_roots, the "named root area" export, had zero consumers from the day it was written. For the whole interval these stacks held live DESCRs
 * that
 * no root phase walked. THEY ARE NOW SCANNED FOR REAL: rt_cas_live_span (below) exports the live prefix of each sub-stack and gc_root_cas (gc_heap.c) walks it with gc_zeta_frame every collection.
 * Fixed caps + a loud bomb replace doubling:
 * an island cannot realloc-move under a collector that has recorded its base.  ⚠ NOT this island (named, so the
 * next session does not re-derive): g_dcap itself is ALREADY an island and its 24B entry is deliberately
 * POINTER-FREE (no root, no adjust — see the block below); rt_zcol_push's per-iteration COLLECTIONS ride the
 * ZC_COLLECTION flavor switch and belong to ZB-ITER, not here. */
#define RT_CAS_ISLAND_BYTES ((size_t)8u << 20)
#define RT_CAS_CAPX_MAX     (1 << 16)
#define RT_CAS_DFX_MAX      (1 << 14)
#define RT_CAS_DCF_MAX      (1 << 14)
#define RT_CAS_SPK_MAX      256
static char  *g_cas_base = 0;
static size_t g_cas_used = 0;
static void *rt_cas_carve(size_t bytes)
{
    extern void *rt_slab_region(size_t);
    if (!g_cas_base) { g_cas_base = (char *)rt_slab_region(RT_CAS_ISLAND_BYTES); if (!g_cas_base) { fprintf(stderr, "rt_cas: island reserve failed\n"); abort(); } }
    bytes = (bytes + 15u) & ~(size_t)15u;
    if (g_cas_used + bytes > RT_CAS_ISLAND_BYTES) { fprintf(stderr, "rt_cas: carve of %zu exceeds the island (raise RT_CAS_ISLAND_BYTES)\n", bytes); abort(); }
    void *p = g_cas_base + g_cas_used; g_cas_used += bytes; memset(p, 0, bytes);
    return p;
}
void rt_cas_roots(void **base, size_t *bytes) { if (base) *base = (void *)g_cas_base; if (bytes) *bytes = g_cas_used; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* CAS-SENTINEL-CLEAN (this session): g_patstk_sp / g_patstk_base / c_rt_patstk_lazy_init DELETED.
 * The pattern stack island was carry-state in the 24-byte CAS sentinel ([+0]=tag, [+8]=rsp_mark,
 * [+16]=patstk_snapshot) so the failure path could restore g_patstk_sp without a frame-addressed
 * reload.  The model is pure R12/DCAP-island — no separate pattern stack exists.  The sentinel
 * shrinks to 16 bytes ([+0]=tag, [+8]=rsp_mark); scan loops use `sub r10,16` not `sub r10,24`.
 * Call sites in rtx_match.S (rt_match_enter, rt_patstk_lazy_init stub) and gen_runtime.c
 * (c_rt_match_enter) are pruned in the same commit. */
uint64_t g_scan_hit_start = 0;   /* SPD-2 RETRY-INTERNAL: flat_pat blob publishes the WINNING attempt-start on scan-mode gamma; the statement defer gamma-cont copies it into the head counter slot (replace-span source) */
uint64_t g_sno_defer_cells[4096];   /* s142 DEFER-SITE DIET: per-site fn cache for WRITE-ONCE deferred names (IR_t.seal==2) — emit assigns indices via g_emit.sn4_defer_cell_n; the site's cold path runs the full GVA/DT_P/dtp_fn_of dance once and stores the fn here; steady state = lea+load+test.  Write-once (single program-wide assignment, fz-safe — the g_sno_seal eligibility) is what makes the cache sound: once DT_P appears the fn can never change.  Zero-init .bss; a 0 store on the not-yet-DT_P path is a harmless no-op. */
uint64_t g_pat_main_rsp = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t g_rspd_save = 0, g_rspd_g4 = 0, g_rspd_g5 = 0, g_rspd_s2 = 0, g_rspd_g6 = 0, g_rspd_beta = 0;
static int g_rspd_active = 0;   /* M4-DESTR-FIX: cached at startup — destructor must not call getenv() at exit time (libc may have cleaned up); constructor caches the flag instead */
__attribute__((constructor)) static void rt_rspd_init(void) { g_rspd_active = (getenv("SCRIP_RSPDIFF") != NULL); }
__attribute__((destructor)) static void rt_rspd_report(void) {
    if (!g_rspd_active) return;
    fprintf(stderr, "RSPDIFF raw: save=%#lx g4=%#lx g5=%#lx s2=%#lx g6=%#lx beta=%#lx\n", (unsigned long)g_rspd_save, (unsigned long)g_rspd_g4, (unsigned long)g_rspd_g5, (unsigned long)g_rspd_s2, (unsigned long)g_rspd_g6, (unsigned long)g_rspd_beta);
    if (g_rspd_save && g_rspd_g4)  fprintf(stderr, "RSPDIFF gamma-retained (save-g4)   = %ld\n", (long)(g_rspd_save - g_rspd_g4));
    if (g_rspd_save && g_rspd_g5)  fprintf(stderr, "RSPDIFF omega-restored (save-g5)   = %ld\n", (long)(g_rspd_save - g_rspd_g5));
    if (g_rspd_save && g_rspd_beta) fprintf(stderr, "RSPDIFF beta-children (save-beta)  = %ld\n", (long)(g_rspd_save - g_rspd_beta));
    if (g_rspd_s2 && g_rspd_g6)    fprintf(stderr, "RSPDIFF exhaust-delta (s2-g6)      = %ld\n", (long)(g_rspd_s2 - g_rspd_g6));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_DCAP_ISLAND_BYTES (4u << 20)
typedef struct { const char *varname; uint64_t saved_delta; uint64_t len; } rt_dcap_e;
const char *g_dcap_base = 0;
#include "pin_va.h"
#define g_dcap_top (*(const char **)RT_DCAP_TOP)
void rt_dcap_lazy_init(void) {
    extern void *rt_slab_region(size_t);
    if (!g_dcap_top) {
        g_dcap_base = (const char *)rt_slab_region(RT_DCAP_ISLAND_BYTES);
        if (!g_dcap_base) { fprintf(stderr, "rt_dcap: island reserve failed\n"); abort(); }
        g_dcap_top = g_dcap_base;
    }
}
int rt_cap_name_strict(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_CAP_NAME_STRICT"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* ⭐ SN4-CAP-NAME-STRICT (s170, row b1c-retreat) runtime half — the emitter-side twin lives in lower_snobol4.c and both read the SAME env name, so a flip moves them together (the s121 both-halves-land-together law).  DEFAULT ON (s178 flip, Lon greenlight; =0 reverts): a deferred capture target that resolves to a VALUE rather than a NAME reports failure to the terminus instead of silently assigning through it. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* NCB-1c M3 (2026-07-11): the commit-time flush is BOX-DRIVEN.  rt_dcap_flush_from's 0..N computed-name
 * (*VAR) transfers move OUT of C into bb_match_end, which pumps: end_ok_open → [transfer → step]* → close.
 * Manual Ch.6: conditional assignments are performed ONLY when the whole match succeeds — hence the deferred
 * batch, hence 0..N calls in one commit.  The cursor rides a LIFO because the OLD loop was re-entrant through
 * its C locals (a *VAR proc body may run its own match, which commits its own pends); a static cursor would
 * have silently corrupted that.  g_rt_dcap_n is re-read every iteration, exactly as the old for-loop did, so
 * pends recorded by a nested match are still swept by the outer pump. */
typedef struct { const char *cur; const char *top; const char *subj; DESCR_t pending; } rt_dcf_t;
__attribute__((visibility("hidden"))) rt_dcf_t *g_dcf; __attribute__((visibility("hidden"))) int g_dcf_top; __attribute__((visibility("hidden"))) int g_dcf_cap;
__attribute__((visibility("hidden"))) int g_dcap_trace = -1;
_Static_assert(sizeof(rt_dcf_t) == 40, "rtx_match.S RTX-8 slice 8 hardcodes stride 40 for rt_dcf_t");
_Static_assert(offsetof(rt_dcf_t, cur) == 0, "rtx_match.S RTX-8 slice 8 hardcodes cur at +0");
_Static_assert(offsetof(rt_dcf_t, top) == 8, "rtx_match.S RTX-8 slice 8 hardcodes top at +8");
_Static_assert(offsetof(rt_dcf_t, subj) == 16, "rtx_match.S RTX-8 slice 8 hardcodes subj at +16");
_Static_assert(offsetof(rt_dcf_t, pending) == 24, "rtx_match.S RTX-8 slice 8 hardcodes pending at +24");
_Static_assert(sizeof(DESCR_t) == 16, "rtx_match.S RTX-8 slice 8 stores pending as v/slen qword + s qword");
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
        char *copy = rt_str_alloc(len);
        if (copy) { if (len > 0 && c->subj) memcpy(copy, c->subj + e->saved_delta, (size_t)len); copy[len] = '\0'; }
        DESCR_t d = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
        c->cur += sizeof(rt_dcap_e);
        if (e->varname && e->varname[0] == '*') {
            const int strict = rt_cap_name_strict();
            extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
            extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
            extern int rt_g_ret_by_name;
            rt_g_want_name = 1;
            DESCR_t nm = rt_call_proc_descr(e->varname + 1, 0);
            rt_g_want_name = 0;
            const int by_name = rt_g_ret_by_name; rt_g_ret_by_name = 0;   /* SN4-CAP-NAME-STRICT: the RETURN-vs-NRETURN discriminator.  rt_nret_fix leaves this standing for a name-context caller (wn==1) instead of clearing it, because the DESCR ALONE CANNOT TELL THE TWO APART -- `F = \'ZZ\' :(NRETURN)` and `F = \'ZZ\' :(RETURN)` both hand back the same STRING, and sbl matches the first while retreating on the second.  Read once, cleared here. */
            if (IS_FAIL_fn(nm)) { if (strict) { rc = 1; continue; } fprintf(stderr, "[DCAP] WARN deferred assignment target '%s' failed or is not invocable; conditional assignment skipped\n", e->varname); continue; }   /* strict: an FRETURNing target is not a name either -- sbl retreats (witness p6), so report it rather than warn-and-succeed */
            if (strict && !by_name) { rc = 1; continue; }   /* a plain RETURN yields a VALUE; a value is not a name, so the node retreats (oracle p3/p5/p8/p13 and all three b1 witnesses).  NRETURN falls through to the two assigning arms below, which stay pre-s170-identical. */
            if (IS_STR_fn(nm)) {   /* ⭐ THE R2 WRONG ANSWER: a plain RETURN hands back a VALUE, which is not a name.  Pre-s170 this was spent as an INDIRECT name (NV_SET on the returned string) and the match went on to answer `match`; sbl 4.0f evaluates the target in NAME context, finds no name, and RETREATS.  NRETURN targets take the rt_assign_var arm below and stay green either way (p16/p17, oracle-identical at HEAD -- the s82 "NRETURN does not compile" blocker is FALSIFIED). */
                                 const char *ns = VARVAL_fn(nm); if (ns && *ns) NV_SET_fn(ns, d); }
            else rt_assign_var(nm, d);
            continue;
        }
        if (e->varname && e->varname[0]) NV_SET_fn(e->varname, d);
    }
    return rc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Box contract (bb_match_end): rdi = MARK (head's saved ___, FRQ(head+32)), rsi = TOP (live ___), rdx =
 * SUBJECT base (r13, by value).  The subject rides the ctx BY VALUE so a mid-pump *VAR transfer that runs a
 * nested match (clobbering Σ and r13 under xfer save) cannot skew the resolution of the REMAINING entries —
 * the old ring was immune by snapshotting base pointers at record time; the pointer-free entry moves that
 * immunity here.  Walk range is FIXED [mark, top): nested matches during a transfer push above top, flush
 * their own range through their own open/close, and restore ___/mirror to their mark == our top — disjoint by
 * construction (the old g_rt_dcap_n re-read compensated for a SHARED counter; ranges need no compensation). */
long c_rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj)
{
    { if (g_dcap_trace < 0) { const char *_e = getenv("SCRIP_DCAP_TRACE"); g_dcap_trace = (_e && _e[0]) ? 1 : 0; } if (g_dcap_trace) fprintf(stderr, "[DCAP] end_ok n=%ld\n", (long)((top - mark) / (long)sizeof(rt_dcap_e))); }   /* BP-2c: cached getenv — this ran per match-with-captures (gdb-sampled ~3% of string_pattern), the BP-2b environ-scan class.  RTX-8 slice 8: the cache moved from a function-local static to the hidden file-scope g_dcap_trace so the asm entry can TEST it; -1 (unresolved) is nonzero, so the asm's first call delegates here and resolution still happens exactly once. */
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
long rt_match_end_all(const char *mark, const char *top, const char *subj, const uint64_t *outer)
{   /* ONE-END (Lon s119 in-chat: "reduce blocks like n64_match_end_alpha down to ONE RT call"): open + close + ctx_restore in ONE call.  The box's transfer loop (glue_pass_wires(3,4) + epilogue_gamma/omega + rt_dcap_step) was MEASURED STATICALLY DEAD this seat: the pump's *VAR arm went C-complete (rt_call_proc_descr, the s117-fixed by-name entry) in a prior session, so c_rt_dcap_end_ok_open and c_rt_dcap_step both return rt_dcap_pump() == 0 unconditionally -- the L(3)/L(4) arms were unreachable plumbing.  A nested match during a *VAR transfer flushes its own disjoint [mark,top) range exactly as before (same pump, same LIFO ctx).  close = the ctx pop (rtx_match.S slice 7: "one test, one decrement").  ctx_restore takes the OUTER sig/len the box used to reload r13/r15 from its saved slots -- passed by value so this call is home-agnostic (mrbp [rbp-16]/[rbp-32] or legacy HKQ/FRQ). */
    extern void rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t capgen);
    long rc = c_rt_dcap_end_ok_open(mark, top, subj);   /* SN4-CAP-NAME-STRICT: nonzero = a deferred capture target was not a NAME; the box turns it into ω (strict arm only -- 0 on every pre-s170 path, so the discarded-return behaviour is unchanged at default) */
    if (g_dcf_top > 0) g_dcf_top--;
    rt_match_ctx_restore(outer[0], outer[1], 0);   /* outer pair rides ONE stack-built pointer (rcx): SysV arg5 is r8 and the x86("call") encoder OWNS r8 (rtccb spill) -- an r8-staged arg saves the argument over the VM global and reloads it back (caught by .s inspection this seat, never executed) */   /* arg3 discarded since CAPGEN-ERAD (gen_runtime.c (void)capgen); the emitted 2-reg call passed garbage rdx into the same discard */
    return rc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* DEAD EXPORT, PARKED NOT DELETED (PARK-NEVER-DELETE): zero callers at NCB-1c.  Its body was the C-side flush
 * loop whose *VAR arm was a C→BB pathway; that pathway is now the box's (rt_dcap_end_ok_open/step/close).  A
 * silent plain-only flush here would be a semantic trap, so it bombs loudly instead.  If a caller ever appears,
 * it must drive the pump from an emitted box. */
void rt_dcap_flush(void) { fprintf(stderr, "[DCAP] FATAL rt_dcap_flush: dead C-side flush called — the commit flush is box-driven since NCB-1c M3 (rt_dcap_end_ok_open/step/close)\n"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ___-dcap: rt_dcap_begin (depth-mark push) → head α's inline `mov FRQ(+32), ___` after loading the mirror;
 * rt_dcap_end_fail (truncate-to-mark) → head ω's inline mirror-store + `mov ___, FRQ(+40)` incoming restore;
 * rt_cap_unpend (dead-trial discard by name) had ZERO callers — the balanced capture-β pop is the discard.
 * All three DELETED with their ring.  end_ok's bomb stays parked (PARK-NEVER-DELETE, NCB-1c). */
void rt_dcap_end_ok(void) { fprintf(stderr, "[DCAP] FATAL rt_dcap_end_ok: superseded by the box-driven pump (NCB-1c M3: rt_dcap_end_ok_open/step/close)\n"); abort(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { uint32_t *buf; uint32_t gen; uint32_t sp; } rt_cap_stk_t;
uint32_t g_cap_gen = 1;   /* ⛔ VISIBILITY IS LOAD-BEARING AND MUST STAY DEFAULT — DO NOT RE-ADD visibility("hidden") (2026-07-29): the α template reads this symbol BY NAME in the EMITTED program text, so in mode 4 the reference lives in a SEPARATE object linked against libscrip_rt.so and a hidden symbol is not in the dynamic table ⇒ 173/316 programs failed to LINK while mode 3 stayed green (mode 3 bakes the address in-process and cannot see the defect).  hidden is reachable from a .S INSIDE the .so and unreachable from emitted code OUTSIDE it — those are two different axes and ARCH §7 step 0(c) only documents the first.  PATCTX-2 (2026-07-29): un-static'd — IR_MATCH_BEGIN's α reads it via [rip+g_cap_gen] (both media) into head.capgen_save (+72) BEFORE rt_match_enter issues a fresh id.  nest1 autopsy: with nesting live (PATCTX), the inner match's stamp invalidated the OUTER match's open brackets — pop no-op'd on stale gen, top returned 0, R captured [0,end).  The id is pattern context. */
__attribute__((visibility("hidden"))) uint32_t g_cap_gen_next = 1;   /* PATCTX-2: the monotonic WELL.  Exits restore g_cap_gen to the SAVED id (an old draw) — never the counter itself, because a restored-then-re-bumped counter would re-issue the inner match's retired stamp and zombie its success-exited frames.  Retired ids never re-issue (modulo the same 2^32 wrap exposure the old counter had), so the lazy-kill invariant — stale gen ⟹ dead frames — survives nesting. */
_Static_assert(__builtin_offsetof(rt_cap_stk_t, buf) == 0, "rtx_match.S hardcodes rt_cap_stk_t.buf at +0; the struct drifted -- rt_cap_top would read the span array through the wrong member, which links fine and returns garbage capture cursors silently");
_Static_assert(__builtin_offsetof(rt_cap_stk_t, gen) == 8, "rtx_match.S hardcodes rt_cap_stk_t.gen at +8; the struct drifted -- the generation compare would test the wrong word and stale frames would resurrect across statements");
_Static_assert(__builtin_offsetof(rt_cap_stk_t, sp) == 12, "rtx_match.S hardcodes rt_cap_stk_t.sp at +12; the struct drifted -- rt_cap_pop/rt_cap_top would index the wrong word");
_Static_assert(sizeof(uint32_t) == 4, "rtx_match.S scales the sp index by 4 in [rdx+rcx*4]; uint32_t drifted");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_cap_push(void *slot, int delta)
{
    /* SN4-PAT-CAPTURE-STACK (Lon directive 2026-07-05): capture frames on a per-box stack — the SAVE box's
     * α does ++ (push), its β does -- (pop); the COND at each yield reads the top-of-stack span, so the
     * β-resume chain survives a generator re-entry between capture-open and capture-close.  The slot is the
     * box's 16B zls grant: +0 buf (rt_ws_alloc u32[]: [0]=cap, frames from [1] — cursor ints only, ZERO
     * pointers out, so no GC root compensation is owed; TR-3(c) workspace = grow-only, the abandoned buffer
     * on a grow is reclaimed by GC-W-2, not sooner — the doubling keeps total waste under 2x), +8 gen, +12 sp.  gen is a
     * per-match generation stamped by rt_match_enter: a stale-gen slot lazily resets sp=0, so success-exited
     * frames (never β-popped — the γ-exit-live case, ZB-ALLOC §3) die at the next match instead of leaking
     * across statement executions; it also validates ZC_INIT_ZERO-fresh ζ frames (gen 0 ≠ any live gen). */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* NCB-1c M2 (2026-07-11): rt_cap_assign_cursor split into strict leaves — the computed-name (*VAR) transfer
 * moves OUT of C into the emitted capture box (the NCB-1b arm).  rt_cap_open does everything up to the
 * transfer and returns fbytes (a proc call is owed: box does sub rsp / rt_frame_prep / call rax / finish) or
 * 0 (fully handled: plain name assigned, dcap recorded, empty name, or *proc unresolvable — no assign, the
 * FAILDESCR behavior of the old path).  The matched DESCR rides a LIFO beside the pcall ctx (the callee may
 * itself capture); rt_g_want_name is set here and captured/cleared by rt_proc_call_open into the ctx. */
static DESCR_t *g_capx; static int g_capx_top, g_capx_cap;
long c_rt_cap_open(const char *varname, int saved_delta, int cur_delta, int is_imm)
{
    (void)is_imm; /* ___-dcap: the COND (deferred) arm no longer calls here — bb_match_capture phase 1 records
                   * its entry inline on the ___ stack.  Every remaining caller is the immediate ($) path. */
    if (!varname || !*varname) return 0;
    int len = cur_delta - saved_delta;
    if (len < 0) len = 0;
    const char *base = Σ ? Σ + saved_delta : NULL;
    char *copy = rt_str_alloc(len);
    if (copy) { if (len > 0 && base) memcpy(copy, base, (size_t)len); copy[len] = '\0'; }
    DESCR_t matched = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
    if (varname[0] != '*') { NV_SET_fn(varname, matched); return 0; }
    extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
    extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
    extern int rt_g_want_name;
    rt_g_want_name = 1;
    DESCR_t nm = rt_call_proc_descr(varname + 1, 0);
    rt_g_want_name = 0;
    if (!IS_FAIL_fn(nm)) { if (IS_STR_fn(nm)) { const char *ns = VARVAL_fn(nm); if (ns && *ns) NV_SET_fn(ns, matched); } else rt_assign_var(nm, matched); }
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* NCB-1c M1 (2026-07-11): rt_defer_match split into strict leaves — the *X / DT_X transfers move OUT of C into
 * bb_match_defer (the NCB-1b arm, looped).  Manual p.85-86: *X re-fetches at EVERY match-time reference; the
 * Expression datatype (DT_X) is evaluated only when referenced in a match, and its evaluation may itself run a
 * match — hence the LIFO, and hence the box (not C) must own the transfer.  Round discipline mirrors the old
 * body EXACTLY: one '*'-triggered call and at most one DT_X-triggered call (dtx_used); a second DT_X result is
 * stored, not re-called, and falls out of close as -1 (it is neither S nor I nor R).
 *   rt_defer_open  -> fbytes (a call is owed) | 0 (resolved or failed)
 *   rt_defer_step  -> fbytes (another call owed) | 0 (resolved or failed)   [after each transfer]
 *   rt_defer_close -> new cursor | -1 */
typedef struct { DESCR_t val; int failed; int dtx_used; } rt_dfx_t;
/* RTX-8 SLICE 2 (s215): these three were `static`, i.e. LOCAL in pattern_match.o and per ARCH section 7 step
 * 0(c) NOT REFERENCEABLE FROM A .S AT ALL. Promoted to non-static hidden so rtx_match.S can reach them with a
 * direct [rip+sym] -- NOT through the GOT. `hidden` and not default is the correct tier, VERIFIED not assumed
 * (the s214 g_cap_gen defect): none of the three is named by any template and none appears in any emitted .s,
 * so they are .so-internal only. scripts/test_gate_no_hidden_global_in_emitted.sh keeps that true.
 * The offsets below are the asm's contract. Anchored so a struct change breaks the BUILD, not the runtime --
 * the RTX-4 slice-3 stride lesson: a probe can miss a layout drift, a _Static_assert cannot. */
__attribute__((visibility("hidden"))) rt_dfx_t *g_dfx;
__attribute__((visibility("hidden"))) int g_dfx_top, g_dfx_cap;
_Static_assert(sizeof(rt_dfx_t) == 24, "rtx_match.S strides g_dfx by 24");
_Static_assert(__builtin_offsetof(rt_dfx_t, val) == 0, "rtx_match.S reads val at +0");
_Static_assert(__builtin_offsetof(rt_dfx_t, failed) == 16, "rtx_match.S reads failed at +16");
_Static_assert(__builtin_offsetof(rt_dfx_t, dtx_used) == 20, "rtx_match.S reads dtx_used at +20");
_Static_assert(sizeof(DESCR_t) == 16, "rtx_match.S assumes the 16-byte DESCR pair");
static rt_dfx_t *rt_dfx_push(void) {
    if (!g_dfx) { g_dfx = (rt_dfx_t *)rt_cas_carve((size_t)RT_CAS_DFX_MAX * sizeof(rt_dfx_t)); g_dfx_cap = RT_CAS_DFX_MAX; }
    if (g_dfx_top >= g_dfx_cap) { fprintf(stderr, "rt_cas: dfx overflow (%d) — raise RT_CAS_DFX_MAX\n", g_dfx_cap); abort(); }
    rt_dfx_t *s = &g_dfx[g_dfx_top++]; s->val = NULVCL; s->failed = 0; s->dtx_used = 0; return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RTX-8 SLICE 2 (s215): promoted off `static` for the same step-0(c) reason as the g_dfx trio above. This is
 * the ONE-ENTRY LATCH the rung is required to FIX rather than transliterate (the s161 EVAL/deferred slot
 * collision). It is NOT touched by the slice-2 asm: measured 0(d) on json.sno + twitter.json is star=0 of
 * 402,121 opens, so the star path this latch serves carries ZERO traffic on the graded workload and cannot be
 * graded by it. The fix is therefore a SEPARATE correctness deliverable with its own canary (140/141), not a
 * clause of a speed rung -- keeping them apart is what stops a correctness fix borrowing a speed number.
 * DEFER-LATCH FIX (this session): replaced the one-entry g_star_peek with a per-site FIFO stack g_spk[].
 * Root cause: pattern concatenation `outer('c1') outer('c2')` calls rt_defer_get_pat_fn twice at build time,
 * pushing c1 then c2; the single latch kept only the last (c2), so c1's defer_open fell through to
 * rt_proc_call_open("inner(c1)",0) -- a proc name that does not exist -- and segfaulted.  The FIFO pop (oldest
 * matching name first) restores left-to-right ordering: build pushes c1→c2, match traverses c1 then c2. */
typedef struct { const char *nm; DESCR_t val; } rt_spk_t;
static int rt_defer_xpat_on(void);
static int rt_spk_take(const char *nm, DESCR_t *out);
static rt_spk_t *g_spk;
static int g_spk_n, g_spk_cap;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RC-8a / HOME-RBX X-1 (s33): THE ISLAND WAS NEVER SCANNED.  The CAS-1 banner above says these stacks "are covered by RT_SLAB_GC_ROOTS today" — FALSE AT HEAD, and stale in two independent ways:
 * RT_SLAB_GC_ROOTS is #defined 0 (rt_slab.h:14) AND there is not one `#if RT_SLAB_GC_ROOTS` body left in the tree — it was a TR-3 root-registration compensation and died at TR-4,
 * taking the
 * coverage with it and leaving only the sentence. rt_cas_roots has had ZERO consumers since it was written ("for GC-W-1's MARK tomorrow"), so nothing walked the island either. Net: g_capx (a
 * DESCR_t
 * stack), g_dfx (DESCR val), g_dcf (three char* INTO THE SUBJECT plus a pending DESCR) and g_spk (name ptr + DESCR) held live references into the collected workspace that no root phase could see.
 * rt_cas_live_span is the fix's read side: it enumerates the LIVE PREFIX of each sub-stack — the used cursor, not the carved reserve, exactly as the CAS-1 banner specifies — so the collector walks
 * a few
 * KB of live entries instead of the multi-MB zero-filled carve. Index-driven so gc_root_cas can loop it the way gc_root_zeta loops the zeta frames; returns 0 to end the walk. A not-yet-carved or
 * empty
 * stack yields bytes==0 and is skipped by the caller, which is why lazy carve needs no special case here. */
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
long c_rt_defer_open(const char *varname, int ival_flag)
{
    extern long rt_proc_call_open(const char *name, int nargs);
    rt_dfx_t *s = rt_dfx_push(); if (!s) return 0;
    if (varname && !strcmp(varname, "FAIL")) { s->failed = 1; return 0; }
    if (varname && varname[0] == '*') {
        for (int _i = 0; _i < g_spk_n; _i++) { if (g_spk[_i].nm && !strcmp(g_spk[_i].nm, varname)) { DESCR_t r = g_spk[_i].val; if (_i < g_spk_n - 1) memmove(&g_spk[_i], &g_spk[_i+1], (size_t)(g_spk_n-1-_i)*sizeof(rt_spk_t)); g_spk_n--; if (IS_FAIL_fn(r)) { s->failed = 1; return 0; } if (r.v == DT_X && !s->dtx_used) { s->dtx_used = 1; long fb2 = rt_proc_call_open(r.s ? r.s : "", 0); if (!fb2) s->failed = 1; return fb2; } s->val = r; return 0; } }
        long fb = rt_proc_call_open(varname + 1, 0); if (!fb) s->failed = 1; return fb;
    }
    DESCR_t val = NV_GET_fn(varname ? varname : "");
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_XPAT_CHAIN_MAX 256
static void rt_defer_take(rt_dfx_t *s, DESCR_t r)
{   /* ONE-DEFER shared tail: rt_defer_step's val handling for a DESCR-call result -- FAIL sets failed; else val=r.  ⭐ DEFER-DEPTH FLOOR (s187): the old arm resolved a DT_X only when !dtx_used, so a chain of deferred EXPRESSIONs (`G0 = *G1` then `P = *G0`, matched as `*P`) got exactly ONE evaluation and the SECOND DT_X was stored unresolved -- close then -1'd it, and a plain string two links down came back NOMATCH in both modes.  Manual p.85-86 (quoted at rt_defer_match above) says a DT_X evaluation may itself run a match, so the chain is walked to exhaustion; the oracle agrees and loops forever on a self-referential `V = *V`, which is why the walk is BOUNDED rather than a while.  RT_XPAT_CHAIN_MAX terminates a cycle back into the pre-s187 answer (store the DT_X, close -1s it) instead of hanging.  Twinned with rt_defer_step above; a drift between them is the spelled-twice disease. */
    extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
    if (IS_FAIL_fn(r)) { s->failed = 1; return; }
    for (int _g = 0; r.v == DT_X && r.s && _g < RT_XPAT_CHAIN_MAX; _g++) { s->dtx_used = 1; r = rt_call_proc_descr(r.s, 0); if (IS_FAIL_fn(r)) { s->failed = 1; return; } }
    s->val = r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_defer_run_all(const char *varname, int cur_delta)
{   /* ONE-DEFER (Lon s119 in-chat: "reduce blocks like n63_match_defer_alpha down to ONE RT call"): c_rt_defer_open's resolution, with every rt_proc_call_open(nm,0)+wire-enter+epilogue+rt_defer_step round replaced by ONE rt_call_proc_descr(nm,0) -- the s117-fixed C-complete by-name entry into emitted EXPR$ thunks (sealed alpha$ + tiny-record; rt.c:875's named hazard, cured s104/s108/s117).  This is the NCB-1c arc closing: the pump went box-driven BECAUSE the C-to-BB pathway was broken; that pathway is now the runtime's own front door, so the loop folds home.  Resolution arms mirror c_rt_defer_open line-for-line (FAIL literal, g_spk FIFO, ival NAMEVAL/NAMEPTR deref, DT_X thunk); tail = c_rt_defer_close(cur_delta).  Returns the new cursor or -1, exactly close's contract. */
    extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
    rt_dfx_t *s = rt_dfx_push(); if (!s) return -1;
    if (varname && !strcmp(varname, "FAIL")) { s->failed = 1; return c_rt_defer_close(cur_delta); }
    if (varname && varname[0] == '*') {
        for (int _i = 0; _i < g_spk_n; _i++) { if (g_spk[_i].nm && !strcmp(g_spk[_i].nm, varname)) { DESCR_t r = g_spk[_i].val; if (_i < g_spk_n - 1) memmove(&g_spk[_i], &g_spk[_i+1], (size_t)(g_spk_n-1-_i)*sizeof(rt_spk_t)); g_spk_n--; if (IS_FAIL_fn(r)) { s->failed = 1; } else if (r.v == DT_X) { s->dtx_used = 1; rt_defer_take(s, rt_call_proc_descr(r.s ? r.s : "", 0)); } else s->val = r; return c_rt_defer_close(cur_delta); } }
        s->dtx_used = 1; rt_defer_take(s, rt_call_proc_descr(varname + 1, 0)); return c_rt_defer_close(cur_delta);
    }
    DESCR_t val = NV_GET_fn(varname ? varname : "");
    if (val.v == DT_X) { s->dtx_used = 1; DESCR_t _pk; if (rt_defer_xpat_on() && rt_spk_take(val.s, &_pk)) rt_defer_take(s, _pk); else rt_defer_take(s, rt_call_proc_descr(val.s ? val.s : "", 0)); return c_rt_defer_close(cur_delta); }
    s->val = val;
    return c_rt_defer_close(cur_delta);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t patv_slot(void *hv, long i, const char *fb, int ival_flag);
int rt_patv_defer_run_all(void *hv, long i, const char *fb, int cur_delta)
{   /* ONE-DEFER patv twin: rt_patv_defer_open's slot read + the same descr-call substitution + close.  PB-1s: the $V slot is frozen-per-construction; a DT_X slot still owes its call. */
    extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
    rt_dfx_t *s = rt_dfx_push(); if (!s) return -1;
    DESCR_t val = patv_slot(hv, i, fb, 0);   /* ival measured 0 at every emitted site (all xor esi/ecx in bb_match_defer.cpp) -- the deref arm is c_rt_defer_open business, not this fold's */
    if (val.v == DT_X) { s->dtx_used = 1; DESCR_t _pk; if (rt_defer_xpat_on() && rt_spk_take(val.s, &_pk)) rt_defer_take(s, _pk); else rt_defer_take(s, rt_call_proc_descr(val.s ? val.s : "", 0)); return c_rt_defer_close(cur_delta); }
    s->val = val;
    return c_rt_defer_close(cur_delta);
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
    DESCR_t val = NV_GET_fn(varname ? varname : "");
    if (ival_flag) {
        if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s);
        else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val);
    }
    if (val.v == DT_P && val.p) { extern void *dtp_fn_of(void *); return dtp_fn_of(val.p); }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PB-1s (s108) PER-CONSTRUCTION SNAPSHOT: manual p.85-86 -- "NPAT captures the value of variable N at the time of pattern construction"; only unary * defers.  The lowered per-SITE PAT$n$V<i> globals
 * are stage-2 marshalling only: SNO$MKPAT freezes them into the fresh DTP_t's snap vector (one per construction), and the blob's $V DEFER reads slot i of THE DTP IT IS RUNNING UNDER ([rbp-24], stored
 * by the blob preamble from entry rdx) instead of the global by name.  Cures both failure modes of the per-site cell: the self-reference STRUCTURAL CYCLE (SIG11, case_driver / pb_selfref_alt_cycle --
 * iteration 2's cell held a pattern deferring to the cell itself) and the SILENT STALE VALUE (pb_stale_snapshot_value -- a saved construction re-read a later construction's overwrite). */
void rt_patv_freeze(void *hv, const char *bn, long n)
{
    DTP_t *h = (DTP_t *)hv;
    if (!h || !bn || n <= 0) return;
    DESCR_t *v = (DESCR_t *)rt_ws_alloc((size_t)n * sizeof(DESCR_t));
    for (long i = 0; i < n; i++) { char nb[64]; snprintf(nb, sizeof nb, "%s$V%ld", bn, i); v[i] = NV_GET_fn(nb); }   /* indices are the SPARSE api order shared with $A leaves (lower_snobol4.c 2222/2504's identical-traversal invariant): a non-snapg index probes an unregistered name and freezes the harmless null value nothing reads */
    h->snap = v; h->nsnap = n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t patv_slot(void *hv, long i, const char *fb, int ival_flag)
{
    DTP_t *h = (DTP_t *)hv;
    if (h && h->snap && i >= 0 && i < h->nsnap) return h->snap[i];
    { DESCR_t val = NV_GET_fn(fb ? fb : ""); if (ival_flag) { if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s); else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val); } return val; }   /* fallback = the pre-s108 by-name read, taken only when the entry site could not supply a DTP (defensive; every MKPAT product carries snap) */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_DEFER_XPAT_MAX_HOPS 64   /* cycle guard only (`A = *B` with `B = *A`); a legitimate SNOBOL4 deferral chain is a handful of hops -- beauty's deepest is Expr -> Expr0 */
static int rt_defer_xpat_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_DEFER_XPAT"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int rt_defer_xpat_chain_on(void) { const char *e = getenv("SCRIP_DEFER_XPAT_CHAIN"); return !(e && *e == '0'); }   /* s186 KILLSWITCH: =0 restores the pre-fix SINGLE resolve, i.e. the class-B behaviour, byte-for-byte on this path */
static void rt_spk_park(const char *nm, DESCR_t r) { if (!g_spk) { g_spk = (rt_spk_t *)rt_cas_carve((size_t)RT_CAS_SPK_MAX * sizeof(rt_spk_t)); g_spk_cap = RT_CAS_SPK_MAX; } if (g_spk_n >= g_spk_cap) { fprintf(stderr, "rt_cas: spk overflow (%d)\n", g_spk_cap); abort(); } g_spk[g_spk_n].nm = nm; g_spk[g_spk_n].val = r; g_spk_n++; }
static int rt_spk_take(const char *nm, DESCR_t *out) { if (!nm) return 0; for (int _i = 0; _i < g_spk_n; _i++) { if (g_spk[_i].nm && !strcmp(g_spk[_i].nm, nm)) { *out = g_spk[_i].val; if (_i < g_spk_n - 1) memmove(&g_spk[_i], &g_spk[_i+1], (size_t)(g_spk_n-1-_i)*sizeof(rt_spk_t)); g_spk_n--; return 1; } } return 0; }
void *rt_defer_xpat_dtp(const char *nm)
{
    extern DESCR_t rt_call_proc_descr(const char *, int);
    if (!rt_defer_xpat_on()) return NULL;
    DESCR_t r = rt_call_proc_descr(nm ? nm : "", 0);
    for (int _g = 0; r.v == DT_X && r.s && _g < RT_XPAT_CHAIN_MAX; _g++) r = rt_call_proc_descr(r.s, 0);
    if (r.v == DT_P && r.p) { extern void *dtp_fn_of(void *); dtp_fn_of(r.p); return r.p; }
    if (r.v == DT_X && rt_defer_xpat_chain_on()) { for (int hop = 0; r.v == DT_X && r.s && r.s[0] && hop < RT_DEFER_XPAT_MAX_HOPS; hop++) { r = rt_call_proc_descr(r.s, 0); if (r.v == DT_P && r.p) { extern void *dtp_fn_of(void *); dtp_fn_of(r.p); return r.p; } } }   /* s186 CHAINED DEFERRAL: `V = *W` makes V's own value a DT_X, so one resolve lands on ANOTHER unevaluated expression, not on a pattern -- SPITBOL keeps dereferencing until a pattern or a scalar falls out (manual v3.7 p.86/p.196), and stopping at hop 1 is what failed beauty's `Expr = *Expr0`.  The final r is parked under the ORIGINAL nm so the close path still literal-matches a scalar tail and still runs each expression exactly once. */
    rt_spk_park(nm, r);
    return NULL;
}
void *rt_patv_defer_get_pat_dtp(void *hv, long i, const char *fb)
{
    DESCR_t v = patv_slot(hv, i, fb, 0);
    if (v.v == DT_P && v.p) { extern void *dtp_fn_of(void *); dtp_fn_of(v.p); return v.p; }
    if (v.v == DT_X && rt_defer_xpat_on()) { extern void *rt_defer_xpat_dtp(const char *); return rt_defer_xpat_dtp(v.s); }   /* DTP_t out -- dtp_fn_of MATERIALIZES fn for recipe composites first (lazy-compile, its !fn&&rcp arm), then the template loads fn=[dtp+0] and rides dtp into the blob in rdx */
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_patv_defer_open(void *hv, long i, const char *fb, int ival_flag)
{
    extern long rt_proc_call_open(const char *name, int nargs);
    rt_dfx_t *s = rt_dfx_push(); if (!s) return 0;
    DESCR_t val = patv_slot(hv, i, fb, ival_flag);
    if (val.v == DT_X) { s->dtx_used = 1; long fb2 = rt_proc_call_open(val.s ? val.s : "", 0); if (!fb2) s->failed = 1; return fb2; }   /* same owed-call arm as c_rt_defer_open: a frozen DT_X still opens its call */
    s->val = val;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_defer_get_pat_dtp(const char *varname, int ival_flag)
{   /* s108: DTP-returning twin of c_rt_defer_get_pat_fn for the non-GVA defer arm -- caller does fn=[dtp+0] and carries dtp into the blob in rdx so $V-bearing interiors can slot-read; scalar/absent returns NULL exactly as the fn twin does */
    if (varname && varname[0] == '*') {
        extern DESCR_t rt_call_proc_descr(const char *, int);
        DESCR_t r = rt_call_proc_descr(varname + 1, 0);
        if (r.v == DT_P && r.p) { extern void *dtp_fn_of(void *); dtp_fn_of(r.p); return r.p; }   /* materialize-then-return (lazy recipe compile) */
        if (!g_spk) { g_spk = (rt_spk_t *)rt_cas_carve((size_t)RT_CAS_SPK_MAX * sizeof(rt_spk_t)); g_spk_cap = RT_CAS_SPK_MAX; }
        if (g_spk_n >= g_spk_cap) { fprintf(stderr, "rt_cas: spk overflow (%d) — raise RT_CAS_SPK_MAX\n", g_spk_cap); abort(); }
        g_spk[g_spk_n].nm = varname; g_spk[g_spk_n].val = r; g_spk_n++;
        return NULL;
    }
    DESCR_t val = NV_GET_fn(varname ? varname : "");
    if (ival_flag) {
        if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s);
        else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val);
    }
    if (val.v == DT_P && val.p) { extern void *dtp_fn_of(void *); dtp_fn_of(val.p); return val.p; }   /* materialize-then-return (lazy recipe compile) */
    if (val.v == DT_X && rt_defer_xpat_on()) { extern void *rt_defer_xpat_dtp(const char *); return rt_defer_xpat_dtp(val.s); }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SN4 kill-manufactured-names (2026-07-22): the VALUE-operand siblings of rt_defer_get_pat_fn / rt_defer_open.
 * IR_MATCH_VALUE hands the already-computed pattern value in by POINTER (operand[0]'s frame slot) instead of a
 * global name, so there is no NV_GET, no *X star-transfer, and no DT_X owed call (the eager TT_FNC result is a
 * concrete value).  DT_P -> run the compiled pattern fn (dtp_fn_of, the box's first arm); a scalar -> store it
 * and let rt_defer_close do the literal match (the box's second arm).  A DT_P or DT_X reaching close is not a
 * scalar and close returns -1 (clean fail), so the box's DT_P-first order is what keeps semantics correct. */
void *rt_match_value_get_pat_fn(DESCR_t *pval)
{
    if (pval && pval->v == DT_P && pval->p) { extern void *dtp_fn_of(void *); return dtp_fn_of(pval->p); }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_match_value_get_pat_dtp(DESCR_t *pval)
{   /* s108: DTP-returning twin -- bb_match_value loads fn=[dtp+0] and carries dtp into the blob in rdx (same per-construction $V contract as the defer twins) */
    if (pval && pval->v == DT_P && pval->p) { extern void *dtp_fn_of(void *); dtp_fn_of(pval->p); return pval->p; }   /* materialize-then-return (lazy recipe compile) */
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
        char kb[64]; const char *ks = tbl_key_str(idx, kb, sizeof kb);
        TBPAIR_t *e = table_find_pair(tb, ks);
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
        if (e) { vc->cellp = &e->val; vc->tbl = tb; vc->key = 0; }
        else   { vc->cellp = 0; vc->tbl = tb; vc->key = rt_ws_strdup_c(ks); }
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
        TBBLK_t *tbl = obj.tbl; int64_t seen = 0;
        for (int b = 0; b < TABLE_BUCKETS; b++)
            for (TBPAIR_t *ep = tbl->buckets[b]; ep; ep = ep->next) {
                if (seen == idx) {
                    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t)); vc->cellp = &ep->val; vc->tbl = 0; vc->key = 0; vc->key_d = FAILDESCR; vc->sv = FAILDESCR; vc->pos = 0; vc->len = 0;
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
        long n = (long)(rval * (double)tbl->size) + 1; long seen = 0;
        for (int b = 0; b < TABLE_BUCKETS; b++)
            for (TBPAIR_t *ep = tbl->buckets[b]; ep; ep = ep->next)
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
        int found; DESCR_t hit = table_get_found(vc->tbl, vc->key, &found);
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
static DESCR_t c_rt_assign_var_body(DESCR_t var, DESCR_t val) {   /* RTX ICNVAR: asm entry is rt_assign_var (rtx_icnvar.S); this is the gate-off body */
    { DESCR_t sh[2]; sh[0] = var; sh[1] = val; rt_gc_point_arr(sh, 2, (const char **)0); var = sh[0]; val = sh[1]; }
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    if (var.v == DT_N && var.slen == 0 && var.s && *var.s) { extern DESCR_t NV_SET_fn(const char *, DESCR_t); NV_SET_fn(var.s, val); return val; }
    if (var.v == DT_N && var.slen == 1 && var.ptr) { *(DESCR_t *)var.ptr = val; return val; }
    if (!IS_NAMETRAP_fn(var)) {
        fprintf(stderr, "[IDX] BOMB rt_assign_var: lvalue is not a variable (dtype=%d) — string/record subscript assignment is the tvsubs rung (GOAL-IR-IMMUTABLE-EMIT IDX-UNIFY)\n", (int)var.v);
        abort();
    }
    VCELL_t *vc = (VCELL_t *)var.p; if (!vc) return FAILDESCR;
    if (vc->cellp) { *vc->cellp = val; return val; }
    if (vc->tbl) { table_set_descr_keyown(vc->tbl, vc->key, vc->key_d, val); return val; }
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
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ s112 MON-CAP — THE SUBSCRIPTED-LVALUE VALUE TAP (fire point 2 of 2).  SNOBOL4 does NOT reach subscript_set for `T[k] = v`: the front-end lowers a subscripted target to rt_subscript_var, which
 * returns a NAMETRAP lvalue, and the store lands HERE — measured from the emitted asm of a 4-form probe (rt_subscript_var/rt_assign_var, never subscript_set).  This is where beauty.sno's step-49
 * blindness actually lived.  ONLY NON-SIMPLE TARGETS TAP: the `DT_N slen==0` arm is a plain NAME (`$'Y' = ...`), whose event is already emitted by rt_indirect_assign_* in rt.c under the REAL variable
 * name, so tapping it here too would double-emit and desync the very instrument this rung exists to fix.  Cell/table/tvsubs targets have no other tap anywhere and get the oracle's `<lval>` spelling.
 * FAIL results do not tap — the oracle emits no VALUE for a store that did not happen.  Wrapped, not patched per-return, so the original decision tree stays byte-for-byte ONE AUTHORITY. */
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
    /* ZB-5 ARBNO v2 COLLECTION (ARCH-ZETA-LOCAL-STORAGE.md section 5f): grow the owner's per-iteration
     * element store to hold index i, ZERO element i (the fresh-iteration rule — body boxes may read-before-
     * write via rt_cap_push and a reused index must not leak a popped iteration's state; POP never zeroes,
     * resume needs the state), return its address.  ZC_COLLECTION = MALLOC (D7): realloc house style, with
     * the zeta arena (TR-4 s67: the external rooting this once leaned on is gone; the unified collector's root story is GC-W-1's);
     * roots move with the block.  Known v1 lifetime residual (watermarked): the block is reused across
     * anchor retries and statement re-executions within a frame, but leaks at frame death — the per-
     * activation grown-collection release list (5f) or the GC backing (GC-4) retires this. */
    extern void rt_bomb(const char *);
#if ZC_COLLECTION == ZC_COL_MALLOC
    if (i + 1 > *cap_cell) {
        int nc = *cap_cell > 0 ? *cap_cell : 4;
        while (nc < i + 1) nc *= 2;
        char * op = (char *)*ptr_cell;
        char * np = (char *)realloc(op, (size_t)nc * (size_t)elem_sz);
        if (!np) rt_bomb("rt_zcol_push: collection realloc failed");
        memset(np + (size_t)*cap_cell * (size_t)elem_sz, 0, (size_t)(nc - *cap_cell) * (size_t)elem_sz);
        *ptr_cell = np; *cap_cell = nc;
    }
#elif ZC_COLLECTION == ZC_COL_ARENA
    /* BB-OWNED-ζ pivot (statement-scope mark/release_to, this session): grow onto the SAME LIFO arena
     * rt_zls_alloc/rt_zls_release already use, instead of realloc.  No per-table root churn here (TR-4) —
     * rt_zls_alloc already widened the arena's root range to cover every byte up to the new hiwater
     * (zeta_alloc.c rt_zls_alloc), so a block living INSIDE the arena is already GC-visible by construction;
     * per-block rooting was only ever needed for the malloc arm, where each block was its own separate
     * allocation outside any pre-rooted range.  Deliberately NO free of the old block: it chained onto
     * g_zls_cur when it was allocated (rt_zls_alloc's own header write) and stays reachable there even
     * after *ptr_cell moves past it — the enclosing statement's rt_zls_release_to(mark) walks the WHOLE
     * chain back to the mark at statement end and reclaims it then, same backstop property already
     * documented for a mid-iteration ARBNO activation in zeta_alloc.c's rt_zls_release_to comment.  This is
     * what retires the "leaks at frame death" residual noted above: nothing here can outlive the mark. */
    extern void * rt_zls_alloc(long bytes);
    if (i + 1 > *cap_cell) {
        int nc = *cap_cell > 0 ? *cap_cell : 4;
        while (nc < i + 1) nc *= 2;
        char * op = (char *)*ptr_cell;
        char * np = (char *)rt_zls_alloc((long)nc * elem_sz);
        if (op) memcpy(np, op, (size_t)*cap_cell * (size_t)elem_sz);
        *ptr_cell = np; *cap_cell = nc;
    }
#else
#error "rt_zcol_push: ZC_COLLECTION must be ZC_COL_MALLOC or ZC_COL_ARENA (ZC_COL_GC is the GC-4 rung)"
#endif
    char * e = (char *)*ptr_cell + (size_t)i * (size_t)elem_sz;
    memset(e, 0, (size_t)elem_sz);
    return e;
}
