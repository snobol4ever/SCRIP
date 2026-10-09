#include <stdio.h>
#include <errno.h>
#include "ct_arena.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "dtp.h"
#include "core.h"
#include "ast.h"
#include "../parsers/snobol4/scrip_cc.h"
#include "sil_macros.h"
#include "builtins/gen_runtime.h"
#include "rt/gc_heap.h"
#include "rt_diag.h"
#include "rt/rt_arena.h"
#include "rt/rt_protected.h"
#include "snobol4_system_fns.h"
int core_icn_error(int code, DESCR_t val);
void rt_bomb(const char *msg);
#define STACKLESS_ABORT(fn) do { fprintf(stderr, "libscrip_rt: %s called — Icon value stack removed (GROUND ZERO 3). " "This box must be rebuilt stackless (per-box slot, no value stack).\n", (fn)) \
    ; abort(); } while (0)
DESCR_t (*g_eval_str_hook)(const char *s) = NULL;
static DATBLK_t *g_lf_type;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pm_lf_type_invalidate(void) { g_lf_type = (DATBLK_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pm_lf_type_is(DATBLK_t *t) { if (!t) return 0; if (t == g_lf_type) return 1; if (t->nfields < 3 || !t->fields[0] || strcmp(t->fields[0], "frame_elems") != 0) return 0; g_lf_type = t; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int rt_list_view(DESCR_t o, DESCR_t **elems, int *n) {
    if (!IS_DATA_INST_fn(o) || !o.u) return 0;
    DATBLK_t *t = o.u->type;
    if (!pm_lf_type_is(t)) return 0;
    DESCR_t gt = o.u->fields[2];
    if (gt.v != DT_S || !gt.s) return 0;
    if (!(gt.s[0] == 'l' && gt.s[1] == 'i' && gt.s[2] == 's' && gt.s[3] == 't' && gt.s[4] == '\0')) return 0;
    DESCR_t ea = o.u->fields[0];
    *elems = IS_DATA_ELEMS_fn(ea) ? (DESCR_t *)ea.ptr : NULL;
    *n = (int)o.u->fields[1].i;
    return 1;
}
typedef struct dtp_rcp { int tt; const char *s; uint32_t slen; int64_t ival; struct dtp_rcp *l; struct dtp_rcp *r; } dtp_rcp_t;
typedef struct DTP { void *fn; dtp_rcp_t *rcp; int64_t zsz; int32_t zstatic; int32_t zpad; DESCR_t *snap; int64_t nsnap; } DTP_t;
_Static_assert(__builtin_offsetof(DTP_t, fn) == 0, "bb_match_defer inline cache reads DTP_t.fn at offset 0");
_Static_assert(__builtin_offsetof(DTP_t, zsz) == 16, "PS-3 ARBNO stride latch reads DTP_t.zsz at offset 16");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pm_struct_gc_visit(uint16_t type, void *p, size_t bytes) {
    extern void rt_gc_visit_raw(const char **);
    (void)bytes;
    if (type == HB_DTP) { DTP_t *h = (DTP_t *)p; if (h->rcp) rt_gc_visit_raw((const char **)&h->rcp); if (h->snap) rt_gc_visit_raw((const char **)&h->snap); return; }
    if (type == HB_DTPRCP) {
        dtp_rcp_t *r = (dtp_rcp_t *)p;
        if (r->s) rt_gc_visit_raw((const char **)&r->s);
        if (r->l) rt_gc_visit_raw((const char **)&r->l);
        if (r->r) rt_gc_visit_raw((const char **)&r->r);
        return;
    }
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
static int pstamp_trace(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_PSTAMP_TRACE"); v = e ? (atoi(e) != 0) : 0; } return v; }
#else
#define pstamp_trace() 0
#endif
static DTP_t *dtp_new(void *fn, dtp_rcp_t *rcp) {
    DTP_t *h = (DTP_t *)rt_pm_struct_alloc(HB_DTP, sizeof(DTP_t));
    h->fn = fn;
    h->rcp = rcp;
    h->zsz = 0;
    h->zstatic = 0;
    h->zpad = 0;
    h->snap = 0;
    h->nsnap = 0;
    return h;
}
void *dtp_wrap_fn(void *fn) { return (void *)dtp_new(fn, (dtp_rcp_t *)0); }
void *dtp_wrap_fn_sz(void *fn, int64_t zsz, int32_t zstatic) {
    DTP_t *h = dtp_new(fn, (dtp_rcp_t *)0);
    h->zsz = zsz;
    h->zstatic = zstatic;
    if (pstamp_trace()) fprintf(stderr, "PSTAMP wrap fn=%p zsz=%lld zstatic=%d\n", fn, (long long)zsz, (int)zstatic);
    return (void *)h;
}
int64_t dtp_zsz_of(void *headv) { DTP_t *h = (DTP_t *)headv; return h ? h->zsz : 0; }
int dtp_zstatic_of(void *headv) { DTP_t *h = (DTP_t *)headv; return h ? (int)h->zstatic : 0; }
static dtp_rcp_t *rcp_node(int tt, const char *s, uint32_t n, int64_t iv, dtp_rcp_t *l, dtp_rcp_t *rr) {
    dtp_rcp_t *r = (dtp_rcp_t *)rt_pm_struct_alloc(HB_DTPRCP, sizeof *r);
    r->tt = tt;
    r->s = s;
    r->slen = n;
    r->ival = iv;
    r->l = l;
    r->r = rr;
    return r;
}
static dtp_rcp_t *rcp_lit(const char *s, uint32_t n) { return rcp_node(TT_QLIT, s ? s : "", n, 0, 0, 0); }
static dtp_rcp_t *rcp_bin(int tt, dtp_rcp_t *l, dtp_rcp_t *rr) { return rcp_node(tt, 0, 0, 0, l, rr); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static dtp_rcp_t *rcp_of(DESCR_t d) {
    if (d.v == DT_P && d.p) {
        DTP_t *h = (DTP_t *)d.p;
        if (h->rcp) return h->rcp;
        static int opq_uid = 0;
        char nb[24];
        snprintf(nb, sizeof nb, "OPQ$%d", opq_uid++);
        NV_SET_fn(nb, d);
        { const char *pn = rt_heap_strdup_c(nb); return rcp_node(TT_DEFER, pn, (uint32_t)strlen(pn), 0, 0, 0); }
    }
    if (d.v == DT_X) {
        sno_dstar_rec_t *rec = SNO_DTX_REC(d);
        const char *nm = !rec ? "*" : (rec->flags & SNO_DSTAR_VARREF) ? rec->star + 1 : rec->star;
        return rcp_node(TT_DEFER, nm, (uint32_t)strlen(nm), 0, 0, 0);
    }
    if (d.v == DT_S || d.v == DT_SNUL) { const char *s = d.s ? d.s : ""; return rcp_lit(s, d.slen ? d.slen : (uint32_t)strlen(s)); }
    if (IS_INT_fn(d)) { char *b = rt_str_alloc(31); snprintf(b, 32, "%lld", (long long)d.i); return rcp_lit(b, (uint32_t)strlen(b)); }
    if (IS_REAL_fn(d)) { char *b = rt_str_alloc(39); real_str(d.r, b, 40); return rcp_lit(b, (uint32_t)strlen(b)); }
    { const char *s = VARVAL_fn(d); return rcp_lit(s ? s : "", s ? (uint32_t)strlen(s) : 0); }
}
extern tree_t *ast_stmt_new(tree_e kind);
#if RT_DIAG
static int rtpat_plant_heapimm(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_RTPAT_PLANT_HEAPIMM"); v = (e && *e && *e != '0') ? 1 : 0; } return v; }
#else
#define rtpat_plant_heapimm() 0
#endif
_Static_assert(sizeof(tree_t *) == 8,
    "EVERY STRING A RUNTIME-COMPILED PATTERN BAKES INTO ITS CODE LIVES IN THE COMPILE-TIME ARENA, NEVER IN THE COLLECTED HEAP (cto 2026-09-23, CTO-152; site bb_match_defer.cpp:115): dtp_rcp_tree han"
    "ds the runtime compiler a tree whose sval words become imm64 operands of code that outlives every collection, so a heap string there (the ARB$n deferred name was one, rt_heap_strdup_c; the lite"
    "ral copies were rt_str_alloc) is an address the collector never sees, reclaimed at the first collection after the compile -- arbno_fence_span_branch_1 read FAIL for MATCH at stress 1/3/5 in eve"
    "ry arena and relocation configuration once the entry was polled. ct_strdup/ct_strndup own them for the code's lifetime, like the parser's own literals; SCRIP_RTPAT_PLANT_HEAPIMM=1 restores the "
    "heap name as the plant the gate fires on");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dtp_rcp_tree(dtp_rcp_t *r, DESCR_t self) {
    if (!r) { tree_t *t = ast_stmt_new(TT_QLIT); t->v.sval = (char *)""; return t; }
    tree_t *t = ast_stmt_new((tree_e)r->tt);
    switch (r->tt) {
        case TT_QLIT:
        { uint32_t L = r->slen; const char *sp = r->s ? r->s : ""; t->v.sval = ct_strndup(sp, (size_t)L); break; }
        case TT_SEQ:
        case TT_ALT:
        ast_push(t, dtp_rcp_tree(r->l, self));
        ast_push(t, dtp_rcp_tree(r->r, self));
        break;
        case TT_ANY:
        case TT_NOTANY:
        case TT_SPAN:
        case TT_BREAK:
        case TT_BREAKX:
        { tree_t *c = ast_stmt_new(TT_QLIT); uint32_t L = r->slen; const char *sp = r->s ? r->s : ""; c->v.sval = ct_strndup(sp, (size_t)L); ast_push(t, c); break; }
        case TT_LEN:
        case TT_TAB:
        case TT_RTAB:
        case TT_POS:
        case TT_RPOS:
        { tree_t *c = ast_stmt_new(TT_ILIT); c->v.ival = r->ival; ast_push(t, c); break; }
        case TT_ARBNO:
        {
            static int arb_uid = 0;
            char nb[24];
            snprintf(nb, sizeof nb, "ARB$%d", arb_uid++);
            DESCR_t sub;
            if (self.p && ((DTP_t *)self.p)->rcp == r) sub = self;
            else { sub.v = DT_P; sub.slen = 0; sub.p = (void *)dtp_new((void *)0, r); }
            NV_SET_fn(nb, sub);
            if (sub.p != self.p) { extern void *dtp_fn_of(void *); dtp_fn_of(sub.p); }
            t->t = TT_ALT;
            { tree_t *nul = ast_stmt_new(TT_QLIT); nul->v.sval = (char *)""; ast_push(t, nul); }
            {
                tree_t *sq = ast_stmt_new(TT_SEQ);
                ast_push(sq, dtp_rcp_tree(r->l, self));
                tree_t *df = ast_stmt_new(TT_DEFER);
                tree_t *v = ast_stmt_new(TT_VAR);
                v->v.sval = rtpat_plant_heapimm() ? rt_heap_strdup_c(nb) : ct_strdup(nb);
                ast_push(df, v);
                ast_push(sq, df);
                ast_push(t, sq);
            }
            break;
        }
        case TT_DEFER:
        { tree_t *v = ast_stmt_new(TT_VAR); v->v.sval = ct_strdup(r->s ? r->s : ""); ast_push(t, v); break; }
        case TT_FENCE:
        if (r->ival) ast_push(t, dtp_rcp_tree(r->l, self));
        break;
        case TT_CAPT_COND_ASGN:
        case TT_CAPT_IMMED_ASGN:
        { ast_push(t, dtp_rcp_tree(r->l, self)); tree_t *v = ast_stmt_new(TT_VAR); v->v.sval = (char *)(r->s ? r->s : ""); ast_push(t, v); break; }
        case TT_CAPT_CURSOR:
        { tree_t *v = ast_stmt_new(TT_VAR); v->v.sval = (char *)(r->s ? r->s : ""); ast_push(t, v); break; }
        default:
        break;
    }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *dtp_fn_of(void *headv) {
    DTP_t *h = (DTP_t *)headv;
    if (!h) return (void *)0;
    if (!h->fn && h->rcp) {
        extern void *bb_compile_pat_tree_sz(const void *tv, int64_t *zsz, int32_t *zstatic);
        DESCR_t sd = {0};
        sd.v = DT_P;
        sd.slen = 0;
        sd.p = (void *)h;
        h->fn = bb_compile_pat_tree_sz((const void *)dtp_rcp_tree(h->rcp, sd), &h->zsz, &h->zstatic);
        if (pstamp_trace()) fprintf(stderr, "PSTAMP fn=%p zsz=%lld zstatic=%d\n", h->fn, (long long)h->zsz, (int)h->zstatic);
    }
    return h->fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_lit(const char *s) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_lit(s ? s : "", s ? (uint32_t)strlen(s) : 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_span(const char *chars) {
    DESCR_t v = {0};
    v.v = DT_P;
    v.slen = 0;
    v.p = (void *)dtp_new((void *)0, rcp_node(TT_SPAN, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_break_(const char *chars) {
    DESCR_t v = {0};
    v.v = DT_P;
    v.slen = 0;
    v.p = (void *)dtp_new((void *)0, rcp_node(TT_BREAK, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_breakx(const char *chars) {
    DESCR_t v = {0};
    v.v = DT_P;
    v.slen = 0;
    v.p = (void *)dtp_new((void *)0, rcp_node(TT_BREAKX, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_any_cs(const char *chars) {
    DESCR_t v = {0};
    v.v = DT_P;
    v.slen = 0;
    v.p = (void *)dtp_new((void *)0, rcp_node(TT_ANY, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_notany(const char *chars) {
    DESCR_t v = {0};
    v.v = DT_P;
    v.slen = 0;
    v.p = (void *)dtp_new((void *)0, rcp_node(TT_NOTANY, chars ? chars : "", chars ? (uint32_t)strlen(chars) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_len(int64_t n) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_LEN, 0, 0, n, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_pos(int64_t n) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_POS, 0, 0, n, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rpos(int64_t n) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_RPOS, 0, 0, n, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_tab(int64_t n) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_TAB, 0, 0, n, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rtab(int64_t n) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_RTAB, 0, 0, n, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_arb(void) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_ARB, 0, 0, 0, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_arbno(DESCR_t inner) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_ARBNO, 0, 0, 0, rcp_of(inner), 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_rem(void) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_REM, 0, 0, 0, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fence_p(DESCR_t inner) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_FENCE, 0, 0, 1, rcp_of(inner), 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fence(void) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_FENCE, 0, 0, 0, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_flush(void) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_FLUSH, 0, 0, 0, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pat_is_bare_flush(DESCR_t d) { const DTP_t *h = (d.v == DT_P) ? (const DTP_t *)d.p : (const DTP_t *)0; return h && !h->fn && h->rcp && h->rcp->tt == TT_FLUSH && !h->rcp->l && !h->rcp->r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_fail(void) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_FAIL, 0, 0, 0, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_abort(void) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_ABORT, 0, 0, 0, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_succeed(void) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_SUCCEED, 0, 0, 0, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_bal(void) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(TT_BAL, 0, 0, 0, 0, 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_epsilon(void) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_lit("", 0)); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pat_cset_code(int tt) { return tt == TT_ANY ? 59 : tt == TT_NOTANY ? 151 : tt == TT_SPAN ? 188 : tt == TT_BREAK ? 69 : tt == TT_BREAKX ? 70 : 0; }
int pat_cset_arg_ok(int code, DESCR_t a) {
    extern int rt_coerce_str_d(const DESCR_t *, DESCR_t *, long);
    DESCR_t o;
    if (!code || a.v == DT_E) return 1;
    return !rt_coerce_str_d(&a, &o, (long)code | ((long)code << 16));
}
DESCR_t pat_mk_cset(int tt, const char *cs) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(tt, cs ? cs : "", cs ? (uint32_t)strlen(cs) : 0, 0, 0, 0)); return v; }
DESCR_t pat_mk_num(int tt, int64_t n) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(tt, 0, 0, n, 0, 0)); return v; }
DESCR_t pat_mk_nil(int tt) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_node(tt, 0, 0, 0, 0, 0)); return v; }
DESCR_t pat_mk_capt(int tt, const char *name, DESCR_t sub) {
    DESCR_t v = {0};
    v.v = DT_P;
    v.slen = 0;
    v.p = (void *)dtp_new((void *)0, rcp_node(tt, name ? name : "", name ? (uint32_t)strlen(name) : 0, 0, rcp_of(sub), 0));
    return v;
}
DESCR_t pat_mk_cursor(const char *name) {
    DESCR_t v = {0};
    v.v = DT_P;
    v.slen = 0;
    v.p = (void *)dtp_new((void *)0, rcp_node(TT_CAPT_CURSOR, name ? name : "", name ? (uint32_t)strlen(name) : 0, 0, 0, 0));
    return v;
}
DESCR_t pat_defer(const char *name) {
    DESCR_t v = {0};
    v.v = DT_P;
    v.slen = 0;
    v.p = (void *)dtp_new((void *)0, rcp_node(TT_DEFER, name ? name : "", name ? (uint32_t)strlen(name) : 0, 0, 0, 0));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pat_operand_is_null(DESCR_t d) { return (d.v == DT_SNUL) || (d.v == DT_S && (!d.s || (d.slen == 0xFFFFFFFFu ? !*d.s : d.slen == 0))); }
DESCR_t pat_cat(DESCR_t left, DESCR_t right) {
    if (pat_operand_is_null(right)) return left;
    if (pat_operand_is_null(left)) return right;
    DESCR_t v = {0};
    v.v = DT_P;
    v.slen = 0;
    v.p = (void *)dtp_new((void *)0, rcp_bin(TT_SEQ, rcp_of(left), rcp_of(right)));
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pat_alt(DESCR_t left, DESCR_t right) { DESCR_t v = {0}; v.v = DT_P; v.slen = 0; v.p = (void *)dtp_new((void *)0, rcp_bin(TT_ALT, rcp_of(left), rcp_of(right))); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_index_operand_ok(DESCR_t base, DESCR_t idx, int strict) {
    if (!strict) return 1;
    core_icn_op_ctx("[]", 2, base, idx);
    int ok = core_icn_int_operand_ok_d(idx);
    core_icn_op_ctx_clear();
    return ok;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_section_operands_ok_code(DESCR_t base, DESCR_t i, DESCR_t j, int code, int strict) {
    if (!strict) return 1;
    core_icn_op_ctx("[]", 2, base, i);
    if (!core_icn_num_operand_ok_d(i, code)) { core_icn_op_ctx_clear(); return 0; }
    core_icn_op_ctx("[]", 2, base, j);
    int ok = core_icn_num_operand_ok_d(j, code);
    core_icn_op_ctx_clear();
    return ok;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_section_operands_ok(DESCR_t base, DESCR_t i, DESCR_t j, int strict) { return icn_section_operands_ok_code(base, i, j, 101, strict); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t subscript_get_s(DESCR_t arr, DESCR_t idx, int strict) {
    if (IS_REAL_fn(arr)) { extern DESCR_t descr_to_str_fracdigit(DESCR_t); arr = descr_to_str_fracdigit(arr); }
    if (arr.v == DT_BIG) { extern char *rt_big_str(DESCR_t); arr = STRVAL(rt_big_str(arr)); }
    if (arr.v == DT_A) { if (!icn_index_operand_ok(arr, idx, strict)) return FAILDESCR; return array_get(arr.arr, (int)to_int(idx)); }
    if (arr.v == DT_T) {
        int found;
        DESCR_t hit = table_get_found_d(arr.tbl, idx, &found);
        if (found) return hit;
        if (arr.tbl->dflt.v != DT_FAIL && arr.tbl->dflt.v != 0) return arr.tbl->dflt;
        return NULVCL;
    }
    if (arr.v == DT_I) { char ibuf[32]; snprintf(ibuf, sizeof ibuf, "%lld", (long long)arr.i); arr = STRVAL(rt_str_dup(ibuf)); }
    if (arr.v == DT_S || arr.v == DT_SNUL) {
        const char *s = arr.s ? arr.s : "";
        int slen = IS_CSET_fn(arr) ? kw_cset_len(s) : -1;
        if (slen < 0 && arr.v == DT_S && arr.slen != 0xFFFFFFFFu) slen = (int)arr.slen;
        if (slen < 0) slen = (int)strlen(s);
        if (!icn_index_operand_ok(arr, idx, strict)) return FAILDESCR;
        int i = (int)to_int(idx);
        if (i < 0) i = slen + i + 1;
        if (i < 1 || i > slen) return FAILDESCR;
        char *buf = rt_str_alloc(1);
        buf[0] = s[i-1];
        buf[1] = '\0';
        return BSTRVAL(buf, 1);
    }
    if (arr.v == DT_DATA) {
        DESCR_t *elems;
        int n;
        if (rt_list_view(arr, &elems, &n)) {
            if (!icn_index_operand_ok(arr, idx, strict)) return FAILDESCR;
            int i = (int)to_int(idx);
            if (i < 0) i = n + i + 1;
            if (!elems || i < 1 || i > n) return FAILDESCR;
            return elems[i-1];
        }
        if (arr.u && arr.u->type && arr.u->type->nfields > 0 && arr.u->fields) {
            DATBLK_t *blk = arr.u->type;
            if (idx.v == DT_SNUL && !icn_index_operand_ok(arr, idx, strict)) return FAILDESCR;
            if (IS_INT_fn(idx)) { int i = (int)idx.i; if (i <= 0) i = blk->nfields + 1 + i; if (i < 1 || i > blk->nfields) return FAILDESCR; return arr.u->fields[i-1]; }
            if (idx.v == DT_S || idx.v == DT_SNUL) {
                const char *k = idx.s ? idx.s : "";
                for (int i = 0; i < blk->nfields; i++) if (blk->fields[i] && strcmp(blk->fields[i], k) == 0) return arr.u->fields[i];
                return FAILDESCR;
            }
        }
        int i = (int)to_int(idx);
        DESCR_t children = FIELD_GET_fn(arr, "c");
        if (children.v == DT_A && children.arr) return array_get(children.arr, i);
        return FAILDESCR;
    }
    core_runtime_error(3, NULL);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int subscript_set_body(DESCR_t arr, DESCR_t idx, DESCR_t val) {
    if (arr.v == DT_A) { int i = (int)to_int(idx); if (i < arr.arr->lo || i > arr.arr->hi) return 0; array_set(arr.arr, i, val); return 1; }
    if (arr.v == DT_T) { table_set_descr_d(arr.tbl, idx, val); return 1; }
    if (arr.v == DT_DATA) {
        DESCR_t *elems;
        int n;
        if (rt_list_view(arr, &elems, &n)) { int i = (int)to_int(idx); if (i < 0) i = n + i + 1; if (!elems || i < 1 || i > n) return 0; elems[i - 1] = val; return 1; }
        if (arr.u && arr.u->type && arr.u->fields) {
            DATBLK_t *blk = arr.u->type;
            if (IS_INT_fn(idx)) { int i = (int)idx.i; if (i < 1 || i > blk->nfields) return 0; arr.u->fields[i - 1] = val; return 1; }
            if (idx.v == DT_S || idx.v == DT_SNUL) {
                const char *k = idx.s ? idx.s : "";
                for (int i = 0; i < blk->nfields; i++) if (blk->fields[i] && strcmp(blk->fields[i], k) == 0) { arr.u->fields[i] = val; return 1; }
                return 0;
            }
        }
        return 0;
    }
    if (arr.v == DT_S && arr.s) {
        int slen = (int)descr_slen(arr);
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
        if (vlen == 1) { live[i - 1] = vs[0]; } else { memmove(live + i - 1 + vlen, live + i, slen - i + 1); memcpy(live + i - 1, vs, vlen); }
        return 1;
    }
    core_runtime_error(3, NULL);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val) { int ok = subscript_set_body(arr, idx, val); if (ok && g_trace_budget != 0) sno_trace_value("<lval>", val); return ok; }
#else
int subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val) { return subscript_set_body(arr, idx, val); }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t subscript_get2_s(DESCR_t arr, DESCR_t i, DESCR_t j, int strict);
static DESCR_t subscript_get2_ext_s(DESCR_t arr, DESCR_t i, DESCR_t end, int strict) {
    if (!icn_section_operands_ok_code(arr, i, end, 102, strict)) return FAILDESCR;
    return subscript_get2_s(arr, i, end, strict);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t subscript_get2_s(DESCR_t arr, DESCR_t i, DESCR_t j, int strict) {
    if (IS_INT_fn(arr) || IS_REAL_fn(arr)) { extern DESCR_t descr_to_str_fracdigit(DESCR_t); arr = descr_to_str_fracdigit(arr); }
    if (arr.v == DT_BIG) { extern char *rt_big_str(DESCR_t); arr = STRVAL(rt_big_str(arr)); }
    if (arr.v == DT_A) { if (!icn_section_operands_ok(arr, i, j, strict)) return FAILDESCR; return array_get2(arr.arr, (int)to_int(i), (int)to_int(j)); }
    if (arr.v == DT_DATA) {
        DESCR_t *elems;
        int n;
        if (rt_list_view(arr, &elems, &n)) {
            if (!icn_section_operands_ok(arr, i, j, strict)) return FAILDESCR;
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
                DESCR_t empty_ptr = {0};
                empty_ptr.v=DT_DATA;
                empty_ptr.slen=DATA_ELEMS_SLEN;
                empty_ptr.ptr=NULL;
                return DATCON_fn("list", empty_ptr, INTVAL(0), STRVAL("list"), INTVAL(0));
            }
            DESCR_t *rbuf = rt_ws_alloc_descr((size_t)rlen);
            for (int k = 0; k < rlen; k++) rbuf[k] = (elems && ii+k-1 >= 0 && ii+k-1 < n) ? elems[ii+k-1] : NULVCL;
            DESCR_t rptr = {0};
            rptr.v=DT_DATA;
            rptr.slen=DATA_ELEMS_SLEN;
            rptr.ptr=(void*)rbuf;
            static int list_slice_reg = 0;
            if (!list_slice_reg) { DEFDAT_fn("list(frame_elems,frame_size,gen_type,frame_cap)"); list_slice_reg=1; }
            return DATCON_fn("list", rptr, INTVAL(rlen), STRVAL("list"), INTVAL(rlen));
        }
    }
    if (arr.v == DT_S || (arr.v == DT_SNUL && arr.s)) {
        const char *s = arr.s ? arr.s : "";
        int slen = IS_CSET_fn(arr) ? kw_cset_len(s) : -1;
        if (slen < 0) slen = (arr.slen && arr.slen != 0xFFFFFFFFu) ? (int)arr.slen : (int)strlen(s);
        if (!icn_section_operands_ok(arr, i, j, strict)) return FAILDESCR;
        int ii = (int)to_int(i), jj = (int)to_int(j);
        if (ii < -slen || ii > slen + 1) return FAILDESCR;
        if (jj < -slen || jj > slen + 1) return FAILDESCR;
        if (ii <= 0) ii = slen + ii + 1;
        if (jj <= 0) jj = slen + jj + 1;
        if (ii > jj) { int t = ii; ii = jj; jj = t; }
        int len = jj - ii;
        char *buf = rt_str_alloc(len);
        memcpy(buf, s+ii-1, len);
        buf[len]='\0';
        return BSTRVAL(buf, len);
    }
    { extern void core_icn_op_ctx(const char *, int, DESCR_t, DESCR_t); if ((arr.v == DT_SNUL || IS_PROCVAL_fn(arr)) && strict) { core_icn_op_ctx("[:]", 3, arr, i); core_icn_error(110, arr); } }
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
#if RT_DIAG
int subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val) { int ok = subscript_set2_body(arr, i, j, val); if (ok && g_trace_budget != 0) sno_trace_value("<lval>", val); return ok; }
#else
int subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val) { return subscript_set2_body(arr, i, j, val); }
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void register_fn(const char *name, DESCR_t (*fn)(DESCR_t*, int), int min_args, int max_args) {
    DEFINE_fn(name, fn);
    { extern void core_fn_set_arity(const char *name, int min_args, int max_args); core_fn_set_arity(name, min_args, max_args); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int eval_text_takes_chain(const char *s) {
    if (!s || !*s) return 0;
    { const char *c = s; while (*c == ' ' || *c == '\t') c++; if ((*c >= 'A' && *c <= 'Z') || (*c >= 'a' && *c <= 'z')) return 1; }
    if (strpbrk(s, " \t\n\v\f\r")) return 1;
    { char *endp = NULL; errno = 0; (void)strtoll(s, &endp, 10); if (endp && *endp == '\0') return 0; }
    { extern int rt_str_to_real(const char *, double *); double rv; if (rt_str_to_real(s, &rv)) return 0; }
    { char *endp = NULL; (void)strtod(s, &endp); if (endp && *endp == '\0') return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t EVAL_fn(DESCR_t expr) {
    if (expr.v == DT_E) { return EXPVAL_fn(expr); }
    if (expr.v == DT_X) { extern DESCR_t rt_sno_dtx_value_rec(sno_dstar_rec_t *); return rt_sno_dtx_value_rec(SNO_DTX_REC(expr)); }
    if (expr.v != DT_P) { extern void rt_eval_stage_leave(const char *); rt_eval_stage_leave((const char *)0); }
    if (expr.v == DT_I) return expr;
    if (expr.v == DT_R) return expr;
    if (expr.v == DT_P) { core_runtime_error(103, "eval argument is not expression"); return FAILDESCR; }
    const char *s = VARVAL_fn(expr);
    if (!s || !*s) return NULVCL;
    if (eval_text_takes_chain(s)) goto eval_str;
    {
        char *endp = NULL;
        errno = 0;
        int64_t iv = (int64_t)strtoll(s, &endp, 10);
        if (endp && *endp == '\0') { if (errno == ERANGE || iv == INT64_MIN) { extern void rt_eval_syntax_raise(const char *); rt_eval_syntax_raise(s); return FAILDESCR; } return INTVAL(iv); }
    }
    {
        extern int rt_str_to_real(const char *, double *);
        double rv;
        if (rt_str_to_real(s, &rv)) return REALVAL(rv);
        { char *endp = NULL; (void)strtod(s, &endp); if (endp && *endp == '\0') { extern void rt_eval_syntax_raise(const char *); rt_eval_syntax_raise(s); return FAILDESCR; } }
    }
    eval_str:
    if (g_eval_str_hook) return g_eval_str_hook(s);
    extern DESCR_t eval_string_transient(const char *s);
    return eval_string_transient(s);
}
_Static_assert(sizeof(char) == 1,
    "EVAL OF A STRING THAT CANNOT BE A NUMBER SKIPS THE NUMBER PARSES (ceo CEO-1263): a string whose first non-blank character is a letter is an expression, never a number literal, so it goes straig"
    "ht to the compiled-expression cache: three parses of 'X + 1' were 8% of the eval_fixed kernel, and strtod's inf/nan spellings made EVAL('INF') a real where SPITBOL evaluates the variable INF");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *sn4_unary_op_key(const char *op) {
    static const char set[] = "!#%/=|^";
    static const char *const key[] = { "unary!", "unary#", "unary%", "unary/", "unary=", "unary|", "unary^" };
    const char *p = (op && op[0] && !op[1]) ? strchr(set, op[0]) : (const char *)0;
    return p ? key[p - set] : op;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t opsyn(DESCR_t newname, DESCR_t oldname, DESCR_t type) {
    const char *nm = VARVAL_fn(newname);
    const char *old = NULL;
    const char *ks = getenv("SCRIP_OPSYN_KIND");
    if (!(ks && *ks == '0')) {
        extern int kwb_error(int code, const char *msg);
        if (!is_numeric_like(type)) { kwb_error(152, "opsyn third argument is not integer"); return FAILDESCR; }
        {
            int64_t kind = to_int(type);
            if (kind < 0 || kind > 16777216) { kwb_error(153, "opsyn third argument is negative or too large"); return FAILDESCR; }
            if (kind != 0 && !(nm && nm[0] && !nm[1] && strchr(kind == 1 ? "!#%/=|" : "#%&@~", nm[0]))) { kwb_error(156, "opsyn first arg is not correct operator name"); return FAILDESCR; }
        }
    }
    if (to_int(type) == 0 && sn4_sysfn_protected(nm)) { extern int kwb_error(int code, const char *msg); kwb_error(248, "attempted redefinition of system function"); return FAILDESCR; }
    if (oldname.v == DT_N) { if (oldname.slen == 0 && oldname.s && *oldname.s) old = oldname.s; else if (oldname.slen == 1 && oldname.ptr) old = NV_name_from_ptr((const DESCR_t *)oldname.ptr); }
    if (!old) old = VARVAL_fn(oldname);
    if (!nm || !old || !*old) return FAILDESCR;
    if (to_int(type) == 1) nm = sn4_unary_op_key(nm);
    register_fn_alias(nm, old);
    { extern void rt_proc_opsyn_bind(const char *, const char *); rt_proc_opsyn_bind(nm, old); }
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int _sort_type_rank(DESCR_t d) {
    switch (d.v) {
        case DT_A:
        return 0;
        case DT_C:
        return 1;
        case DT_E:
        return 2;
        case DT_I:
        return 3;
        case DT_BOOL:
        return 3;
        case DT_P:
        return 6;
        case DT_R:
        return 7;
        case DT_S:
        return 8;
        case DT_SNUL:
        return 8;
        case DT_T:
        return 9;
        default:
        return 5;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int _sort_cmp_descr(DESCR_t a, DESCR_t b, const char *sa, const char *sb) {
    if (a.v == DT_I && b.v == DT_I) { if (a.i < b.i) return -1; if (a.i > b.i) return 1; return 0; }
    if ((a.v == DT_I || a.v == DT_R) && (b.v == DT_I || b.v == DT_R)) { double x = a.v == DT_I ? (double)a.i : a.r, y = b.v == DT_I ? (double)b.i : b.r; return x < y ? -1 : (x > y ? 1 : 0); }
    if ((a.v == DT_S || a.v == DT_SNUL) && (b.v == DT_S || b.v == DT_SNUL)) { return strcmp(sa ? sa : "", sb ? sb : ""); }
    int ra = _sort_type_rank(a), rb = _sort_type_rank(b);
    if (ra != rb) return ra - rb;
    if (a.v == DT_BIG && b.v == DT_BIG) { extern int rt_big_cmp(DESCR_t, DESCR_t); return rt_big_cmp(a, b); }
    { long ia = tbl_key_serial(a), ib = tbl_key_serial(b); if (ia || ib) return ia < ib ? -1 : (ia > ib ? 1 : 0); }
    if (a.v == DT_N && b.v == DT_N && a.slen == 0 && b.slen == 0 && a.s && b.s) return strcmp(a.s, b.s);
    return a.ptr < b.ptr ? -1 : (a.ptr > b.ptr ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_is_rowarr(const ARBLK_t *a) { return a && a->ndim == 1 && a->proto && strchr(a->proto, ','); }
static int sort_proto_dims(const ARBLK_t *a) { int d = 1; if (a && a->proto) for (const char *p = a->proto; *p; p++) if (*p == ',') d++; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t sort_row_cell(DESCR_t row, int c) { return (row.v == DT_A && row.arr && c >= row.arr->lo && c <= row.arr->hi) ? row.arr->data[c - row.arr->lo] : NULVCL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t sort_field_key(DESCR_t v, const char *fld) {
    if (!fld || !IS_DATA(v) || !v.u || !v.u->type) return v;
    for (int i = 0; i < v.u->type->nfields; i++) if (strcasecmp(v.u->type->fields[i], fld) == 0) return v.u->fields[i];
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_ord_cmp(const DESCR_t *keys, const char **strs, int x, int y, int rev) {
    int c = _sort_cmp_descr(keys[x], keys[y], strs[x], strs[y]);
    if (c) return rev ? -c : c;
    return x < y ? -1 : (x > y ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sort_ord_merge(int *ord, int *tmp, int n, const DESCR_t *keys, const char **strs, int rev) {
    if (n < 2) return;
    int m = n / 2, i = 0, j = m, k = 0;
    sort_ord_merge(ord, tmp, m, keys, strs, rev);
    sort_ord_merge(ord + m, tmp + m, n - m, keys, strs, rev);
    while (i < m && j < n) tmp[k++] = (sort_ord_cmp(keys, strs, ord[j], ord[i], rev) < 0) ? ord[j++] : ord[i++];
    while (i < m) tmp[k++] = ord[i++];
    while (j < n) tmp[k++] = ord[j++];
    for (int t = 0; t < n; t++) ord[t] = tmp[t];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int *sort_order(const DESCR_t *keys, int n, int rev) {
    const char **strs = rt_pvec_alloc((size_t)n);
    for (int i = 0; i < n; i++) strs[i] = (keys[i].v == DT_S && keys[i].s) ? keys[i].s : "";
    int *ord = rt_wsb_alloc((size_t)n * sizeof(int)), *tmp = rt_wsb_alloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) ord[i] = i;
    sort_ord_merge(ord, tmp, n, keys, strs, rev);
    return ord;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t sort_row_copy(DESCR_t row) {
    if (row.v != DT_A || !row.arr) return row;
    ARBLK_t *s = row.arr, *r = rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
    int w = s->hi - s->lo + 1;
    if (w < 1) w = 1;
    r->lo = s->lo;
    r->hi = s->hi;
    r->ndim = 1;
    r->lo2 = 0;
    r->hi2 = 0;
    r->proto_bare = s->proto_bare;
    r->proto = s->proto;
    r->id = rt_agg_serial_list();
    r->dumpno = 0;
    r->data = rt_ws_alloc_descr((size_t)w);
    for (int i = 0; i < w; i++) r->data[i] = s->data[i];
    DESCR_t d = {0};
    d.v = DT_A;
    d.arr = r;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_column(DESCR_t col, int lb2, int dim2, int *out) {
    extern DESCR_t rt_sno_cnv_num(DESCR_t, int);
    if (IS_NULL_fn(col)) { *out = lb2; return 1; }
    DESCR_t ci = IS_INT_fn(col) ? col : rt_sno_cnv_num(col, 'I');
    if (ci.v != DT_I || ci.i < lb2 || ci.i >= (int64_t)lb2 + dim2) return 0;
    *out = (int)ci.i;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t sort_rows(DESCR_t *rows, int n, int lo, const char *proto, int proto_bare, DESCR_t col, int rev) {
    extern int kwb_error(int code, const char *msg);
    int c, lb2 = 1, w = 2;
    if (n > 0 && rows[0].v == DT_A && rows[0].arr) { lb2 = rows[0].arr->lo; w = rows[0].arr->hi - rows[0].arr->lo + 1; }
    if (!sort_column(col, lb2, w, &c)) { kwb_error(258, "sort/rsort 2nd arg out of range or non-integer"); return FAILDESCR; }
    DESCR_t *keys = rt_ws_alloc_descr((size_t)n);
    for (int i = 0; i < n; i++) keys[i] = sort_row_cell(rows[i], c);
    int *ord = sort_order(keys, n, rev);
    ARBLK_t *a = rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
    a->dumpno = rt_sno_dumpno_next();
    a->lo = lo;
    a->hi = lo + n - 1;
    a->ndim = 1;
    a->lo2 = 0;
    a->hi2 = 0;
    a->proto_bare = proto_bare;
    a->id = rt_agg_serial_list();
    a->proto = proto;
    a->data = rt_ws_alloc_descr((size_t)n);
    for (int i = 0; i < n; i++) a->data[i] = sort_row_copy(rows[ord[i]]);
    DESCR_t result = {0};
    result.v = DT_A;
    result.arr = a;
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t sort_impl(DESCR_t arr, DESCR_t col, int rev) {
    extern int kwb_error(int code, const char *msg);
    if (arr.v == DT_T) {
        TBBLK_t *tbl = arr.tbl;
        if (!tbl) return FAILDESCR;
        int n = 0;
        TBPAIR_t *e;
        TBL_FOREACH(tbl, e) if (!IS_NULL_fn(e->val)) n++;
        if (n == 0) return FAILDESCR;
        DESCR_t *rows = rt_ws_alloc_descr((size_t)n);
        int k = 0;
        TBL_FOREACH(tbl, e) {
            if (IS_NULL_fn(e->val)) continue;
            ARBLK_t *row = rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
            row->lo = 1;
            row->hi = 2;
            row->ndim = 1;
            row->lo2 = 0;
            row->hi2 = 0;
            row->proto_bare = 1;
            row->proto = 0;
            row->id = rt_agg_serial_list();
            row->dumpno = 0;
            row->data = rt_ws_alloc_descr(2);
            row->data[0] = e->key_descr;
            row->data[1] = e->val;
            DESCR_t rd = {0};
            rd.v = DT_A;
            rd.arr = row;
            rows[k++] = rd;
        }
        char pb[48];
        snprintf(pb, sizeof pb, "%d,2", n);
        return sort_rows(rows, n, 1, rt_heap_strdup_c(pb), 1, col, rev);
    }
    if (arr.v != DT_A || !arr.arr || (arr.arr->ndim != 1 && arr.arr->ndim != 2) || sort_proto_dims(arr.arr) > 2) { kwb_error(256, "sort/rsort 1st arg not suitable array or table"); return FAILDESCR; }
    ARBLK_t *src = arr.arr;
    int n = src->hi - src->lo + 1;
    if (sort_is_rowarr(src)) return sort_rows(src->data, n < 0 ? 0 : n, src->lo, src->proto, src->proto_bare, col, rev);
    if (src->ndim == 2) {
        int w = src->hi2 - src->lo2 + 1, c;
        if (!sort_column(col, src->lo2, w, &c)) { kwb_error(258, "sort/rsort 2nd arg out of range or non-integer"); return FAILDESCR; }
        ARBLK_t *a = array_new2d(src->lo, src->hi, src->lo2, src->hi2);
        a->proto = src->proto;
        a->proto_bare = src->proto_bare;
        if (n <= 0 || w <= 0) { DESCR_t r0 = {0}; r0.v = DT_A; r0.arr = a; return r0; }
        DESCR_t *keys = rt_ws_alloc_descr((size_t)n);
        for (int i = 0; i < n; i++) keys[i] = src->data[(size_t)i * w + (c - src->lo2)];
        int *ord = sort_order(keys, n, rev);
        for (int i = 0; i < n; i++) for (int j = 0; j < w; j++) a->data[(size_t)i * w + j] = src->data[(size_t)ord[i] * w + j];
        DESCR_t result = {0};
        result.v = DT_A;
        result.arr = a;
        return result;
    }
    const char *fld = NULL;
    if (!IS_NULL_fn(col)) { if (!IS_STR_fn(col) && !IS_INT_fn(col) && !IS_REAL_fn(col)) { kwb_error(257, "erroneous 2nd arg in sort/rsort of vector"); return FAILDESCR; } fld = VARVAL_fn(col); }
    ARBLK_t *a = array_new(src->lo, src->hi);
    if (n <= 0) { DESCR_t r0 = {0}; r0.v = DT_A; r0.arr = a; return r0; }
    DESCR_t *keys = rt_ws_alloc_descr((size_t)n);
    for (int i = 0; i < n; i++) keys[i] = sort_field_key(src->data[i], fld);
    int *ord = sort_order(keys, n, rev);
    for (int i = 0; i < n; i++) a->data[i] = src->data[ord[i]];
    DESCR_t result = {0};
    result.v = DT_A;
    result.arr = a;
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t sort_fn(DESCR_t arr, DESCR_t col) { return sort_impl(arr, col, 0); }
DESCR_t rsort_fn(DESCR_t arr, DESCR_t col) { return sort_impl(arr, col, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t g_scan_hit_start = 0;
uint64_t g_sno_defer_cells[4096];
static int g_sno_defer_pair_hwm = 0;
void rt_defer_pairs_forget(void) { for (int i = 0; i < g_sno_defer_pair_hwm; i++) { g_sno_defer_cells[2048 + i * 2] = 0; g_sno_defer_cells[2048 + i * 2 + 1] = 0; } g_sno_defer_pair_hwm = 0; }
uint64_t g_pat_main_rsp = 0;
uint64_t g_rspd_save = 0, g_rspd_g4 = 0, g_rspd_g5 = 0, g_rspd_s2 = 0, g_rspd_g6 = 0, g_rspd_beta = 0;
#if RT_DIAG
static int g_rspd_active = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rt_rspd_init(void) { g_rspd_active = (getenv("SCRIP_RSPDIFF") != NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((destructor)) static void rt_rspd_report(void) {
    if (!g_rspd_active) return;
    fprintf(stderr, "RSPDIFF raw: save=%#lx g4=%#lx g5=%#lx s2=%#lx g6=%#lx beta=%#lx\n", (unsigned long)g_rspd_save, (unsigned long)g_rspd_g4, (unsigned long)g_rspd_g5, (unsigned long)g_rspd_s2,
        (unsigned long)g_rspd_g6, (unsigned long)g_rspd_beta);
    if (g_rspd_save && g_rspd_g4) fprintf(stderr, "RSPDIFF gamma-retained (save-g4)   = %ld\n", (long)(g_rspd_save - g_rspd_g4));
    if (g_rspd_save && g_rspd_g5) fprintf(stderr, "RSPDIFF omega-restored (save-g5)   = %ld\n", (long)(g_rspd_save - g_rspd_g5));
    if (g_rspd_save && g_rspd_beta) fprintf(stderr, "RSPDIFF beta-children (save-beta)  = %ld\n", (long)(g_rspd_save - g_rspd_beta));
    if (g_rspd_s2 && g_rspd_g6) fprintf(stderr, "RSPDIFF exhaust-delta (s2-g6)      = %ld\n", (long)(g_rspd_s2 - g_rspd_g6));
}
#endif
#include "pin_va.h"
typedef struct { const char *varname; uint64_t saved_delta; uint64_t len; } rt_dcap_e;
const char *g_dcap_base = 0;
#define g_dcap_top (*(const char **)RT_DCAP_TOP)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_lazy_init(void) {
    extern void *rt_slab_region(size_t);
    if (!g_dcap_top) { g_dcap_base = (const char *)rt_slab_region(RT_DCAP_ISLAND_BYTES); if (!g_dcap_base) { fprintf(stderr, "rt_dcap: island reserve failed\n"); abort(); } g_dcap_top = g_dcap_base; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_cap_fail_retreat(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_CAP_FAIL_RETREAT"); v = (e && *e == '0') ? 0 : 1; } return v; }
int rt_cap_name_strict(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_CAP_NAME_STRICT"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#if RT_DIAG
int rt_cap_poison(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_CAP_POISON"); v = (e && *e && *e != '0') ? (int)(unsigned char)'Z' : 0; } return v; }
#else
#define rt_cap_poison() 0
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_cap_slice_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_CAP_SLICE"); v = (e && *e == '0') ? 0 : 1; } return v; }
typedef struct {
    DESCR_t pending;
    uint32_t subj_v, subj_len;
    const char *subj;
    uint32_t star_v, star_len;
    const char *star;
    uint32_t cur_v, cur_pad;
    const char *cur;
    uint32_t top_v, top_pad;
    const char *top;
    uint32_t nhr_v, nhr_pad;
    int nsb;
    short how;
    short rc;
    uint32_t wa_v, wa_pad;
    int wsv;
    uint32_t asv;
} rt_dcf_t;
typedef struct { long fn; long how; } rt_dcap_next_t;
_Static_assert(sizeof(rt_dcap_next_t) == 16, "bb_match_end reads the pump result as rax = entry (0 done, 1 refuse) and rdx = protocol (2 = the alpha tiny record, else wires)");
extern uint32_t g_cap_gen;
__attribute__((visibility("hidden"))) uint32_t g_cap_abort_gen;
#if RT_DIAG
__attribute__((visibility("hidden"))) int g_dcap_trace = -1;
#else
__attribute__((visibility("hidden"))) int g_dcap_trace = 0;
#endif
_Static_assert(sizeof(rt_dcf_t) == 112, "bb_match_end.cpp release_pump carves the commit-pump record as 112 bytes of spine: seven cells");
_Static_assert(offsetof(rt_dcf_t, pending) == 0 && offsetof(rt_dcf_t, subj_v) == 16 && offsetof(rt_dcf_t, subj) == 24 && offsetof(rt_dcf_t, star_v) == 32 && offsetof(rt_dcf_t, star) == 40,
    "rtx_match.s rt_dcap_end_ok_open writes pending at +0 and the subject and star DT_S cells at +16 and +32");
_Static_assert(offsetof(rt_dcf_t, cur_v) == 48 && offsetof(rt_dcf_t, cur) == 56 && offsetof(rt_dcf_t, top_v) == 64 && offsetof(rt_dcf_t, top) == 72,
    "rtx_match.s rt_dcap_end_ok_open writes the cursor and top DT_I cells at +48 and +64");
_Static_assert(offsetof(rt_dcf_t, nhr_v) == 80 && offsetof(rt_dcf_t, nsb) == 88 && offsetof(rt_dcf_t, how) == 92 && offsetof(rt_dcf_t, rc) == 94 && offsetof(rt_dcf_t, wa_v) == 96 &&
    offsetof(rt_dcf_t, wsv) == 104 && offsetof(rt_dcf_t, asv) == 108, "rtx_match.s rt_dcap_end_ok_open writes the two DT_I flag cells at +80 and +96 with their payloads zero");
_Static_assert(sizeof(DESCR_t) == 16 && DT_S == 2 && DT_I == 3, "rtx_match.s rt_dcap_end_ok_open stores the cell tags as imm32 qwords");
#define RT_DCAP_NVCACHE_N 16
static const char *g_dcap_nv_key[RT_DCAP_NVCACHE_N];
static DESCR_t *g_dcap_nv_cell[RT_DCAP_NVCACHE_N];
static unsigned long g_dcap_nv_seen[RT_DCAP_NVCACHE_N];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) DESCR_t *rt_dcap_nv_cell(const char *name) {
    extern unsigned long g_nv_memo_gen;
    unsigned i = (unsigned)((((uintptr_t)name * 0x9E3779B97F4A7C15ull) >> 32) & (RT_DCAP_NVCACHE_N - 1));
    if (g_dcap_nv_key[i] == name && g_dcap_nv_seen[i] == g_nv_memo_gen) return g_dcap_nv_cell[i];
    DESCR_t *cell = NV_CELL_IF_FASTSET_fn(name);
    if (cell) { g_dcap_nv_key[i] = name; g_dcap_nv_cell[i] = cell; g_dcap_nv_seen[i] = g_nv_memo_gen; }
    return cell;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const sno_dstar_rec_t *dcf_rec(const char *s) { return (s && s[0] == '*' && (unsigned char)s[1] == 1) ? (const sno_dstar_rec_t *)(const void *)s : (const sno_dstar_rec_t *)0; }
static inline const char *dcf_name(const char *s) { const sno_dstar_rec_t *r = dcf_rec(s); return r ? r->star : s; }
static void dcf_stage_leave(const char *s) {
    extern void rt_eval_stage_leave(const char *);
    extern void rt_eval_stage_leave_var(int);
    const sno_dstar_rec_t *r = dcf_rec(s);
    if (r && (r->flags & SNO_DSTAR_THUNK)) rt_eval_stage_leave_var((r->flags & SNO_DSTAR_STAGEVAR) ? 1 : 0);
    else rt_eval_stage_leave(s ? dcf_name(s) + 1 : (const char *)0);
}
static long rt_dcap_star_finish(rt_dcf_t *c, DESCR_t nm) {
    extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
    extern int rt_g_ret_by_name;
    extern int rt_g_want_name;
    const int strict = rt_cap_name_strict();
    const sno_dstar_rec_t *srec = dcf_rec(c->star);
    const int nmyield = srec ? ((srec->flags & SNO_DSTAR_EXPRNM) ? 1 : 0) : (c->star[1] == 'E' && !strncmp(c->star + 1, "EXPRNM$", 7));
    g_cap_abort_gen = c->asv;
    rt_g_want_name = c->wsv;
    const int by_name = rt_g_ret_by_name || nmyield;
    rt_g_ret_by_name = 0;
    DESCR_t d = c->pending;
#if RT_DIAG
    if (IS_FAIL_fn(nm)) {
        if (strict) {
            if (g_dcap_trace < 0) { const char *_e = getenv("SCRIP_DCAP_TRACE"); g_dcap_trace = (_e && _e[0]) ? 1 : 0; }
            if (g_dcap_trace) fprintf(stderr, "[DCAP] STRICT-REFUSE target=%s: call FAILED -> rc=1 (match will fail at END)\n", dcf_name(c->star));
            c->rc = 1;
            return 1;
        }
        fprintf(stderr, "[DCAP] WARN deferred assignment target '%s' failed or is not invocable; conditional assignment skipped\n", dcf_name(c->star));
        return 0;
    }
    if (strict && !by_name) {
        if (g_dcap_trace < 0) { const char *_e = getenv("SCRIP_DCAP_TRACE"); g_dcap_trace = (_e && _e[0]) ? 1 : 0; }
        if (g_dcap_trace)
            fprintf(stderr, "[DCAP] STRICT-REFUSE target=%s: returned a VALUE not a NAME (by_name=0, nm.v=%d, nm.slen=%u, nm.s=%.24s) -> rc=1 (match will fail at END)\n", dcf_name(c->star), (int)nm.v,
            nm.slen, (nm.v == DT_S && nm.s) ? nm.s : "?");
        c->rc = 1;
        return 1;
    }
#else
    if (IS_FAIL_fn(nm)) {
        if (strict) { c->rc = 1; return 1; }
        fprintf(stderr, "[DCAP] WARN deferred assignment target '%s' failed or is not invocable; conditional assignment skipped\n", dcf_name(c->star));
        return 0;
    }
    if (strict && !by_name) { c->rc = 1; return 1; }
#endif
    if (IS_STR_fn(nm)) {
        const char *ns = VARVAL_fn(nm);
        if (!ns || !*ns) { extern int kwb_error(int, const char *); kwb_error(239, "indirection operand is not name"); c->rc = 1; return 1; }
        NV_SET_fn(ns, d);
    } else rt_assign_var(nm, d);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((visibility("hidden"))) rt_dcap_next_t rt_dcap_pump(rt_dcf_t *c) {
    extern long rt_proc_call_open(const char *name, int nargs);
    extern int rt_g_want_name;
    extern int g_protected_pat_vars_armed;
    if (g_cap_abort_gen && g_cap_abort_gen == g_cap_gen) { g_cap_abort_gen = 0; return (rt_dcap_next_t){ 1, 0 }; }
    if (!c) { rt_bomb("rt_dcap_pump: no spine record -- release_pump carves one before rt_dcap_end_ok_open"); return (rt_dcap_next_t){ 1, 0 }; }
#if RT_DIAG
    int _cva = comm_var_active();
    int _prev_star = 0;
#endif
#if RT_DIAG
    static long _slice_budget = -2;
    static int _slice_trace = -1;
    static long _slice_idx = 0;
    if (_slice_budget == -2) { const char *_e = getenv("SCRIP_CAP_SLICE_MAX"); _slice_budget = (_e && *_e) ? atol(_e) : -1; }
    if (_slice_trace < 0) { const char *_e = getenv("SCRIP_CAP_SLICE_TRACE"); _slice_trace = (_e && *_e) ? 1 : 0; }
    const int _fast_ok = rt_cap_slice_on() && _slice_budget < 0 && !_slice_trace;
#else
    const int _fast_ok = rt_cap_slice_on();
#endif
    while (c->cur < c->top) {
#if RT_DIAG
        if (_prev_star) { _cva = comm_var_active(); _prev_star = 0; }
#endif
        const rt_dcap_e *e = (const rt_dcap_e *)(const void *)c->cur;
        if (!e->varname) { c->cur += sizeof(rt_dcap_e); continue; }
        if (_fast_ok && e->len > 0 && c->subj && e->varname[0] != '*') {
            extern int Σlen;
            extern unsigned long g_nv_memo_gen;
            unsigned fi = (unsigned)((((uintptr_t)e->varname * 0x9E3779B97F4A7C15ull) >> 32) & (RT_DCAP_NVCACHE_N - 1));
#if RT_DIAG
            if (_cva) goto dcap_slow;
#endif
            if (g_dcap_nv_key[fi] == e->varname && g_dcap_nv_seen[fi] == g_nv_memo_gen && (int)e->len <= Σlen && (long long)e->saved_delta + (long long)e->len <= (long long)Σlen) {
                rt_sxt_break_fast(c->subj);
                DESCR_t fd = (DESCR_t){ .v = DT_S, .slen = (uint32_t)e->len, .s = (char *)c->subj + e->saved_delta };
                rt_sxt_break_fast(fd.s);
                *g_dcap_nv_cell[fi] = fd;
                c->cur += sizeof(rt_dcap_e);
                continue;
            }
        }
#if RT_DIAG
        dcap_slow:
        ;
#endif
        DESCR_t *ecell;
        { extern DESCR_t *rt_gva_cell_of(const void *); ecell = rt_gva_cell_of(e->varname); }
        int len = (int)e->len;
        if (len < 0) len = 0;
        {
            extern int Σlen;
            long long _end = (long long)e->saved_delta + (long long)len;
            if (len > Σlen || _end > (long long)Σlen) {
                fprintf(stderr,
                    "rt_dcap_pump: CORRUPT CAPTURE ENTRY refused — len=%d saved_delta=%llu end=%lld exceeds subject length %d (target '%s'). Deferred re-entry invalidated the outer frame; capture "
                    "skipped rather than reading out of bounds.\n", len, (unsigned long long)e->saved_delta, _end, Σlen,
                    ecell ? "(a variable's cell)" : !e->varname ? "(null)" : (e->varname[0] == '*' && (unsigned char)e->varname[1] == 1) ? ((const sno_dstar_rec_t *)(const void *)e->varname)->star :
                    e->varname);
                c->cur += sizeof(rt_dcap_e);
                c->rc = 1;
                continue;
            }
        }
        DESCR_t d;
        int _star_arm = (!ecell && e->varname && e->varname[0] == '*');
#if RT_DIAG
        int _budget_ok = (_slice_budget < 0) || (_slice_idx < _slice_budget);
#else
        enum { _budget_ok = 1 };
#endif
        if (len > 0 && c->subj && _budget_ok && rt_cap_slice_on()) {
#if RT_DIAG
            if (_slice_trace)
                fprintf(stderr, "[SLICE] #%ld var=%s len=%d delta=%llu subj=%p\n", _slice_idx, ecell ? "(cell)" : e->varname ? e->varname : "?", len, (unsigned long long)e->saved_delta,
                (const void *)c->subj);
            _slice_idx++;
#endif
            rt_sxt_break_fast(c->subj);
            d = (DESCR_t){ .v = DT_S, .slen = (uint32_t)len, .s = (char *)c->subj + e->saved_delta };
        } else {
            char *copy = rt_str_alloc(len);
            if (copy) { if (len > 0 && c->subj) memcpy(copy, c->subj + e->saved_delta, (size_t)len); copy[len] = (char)(len > 0 ? rt_cap_poison() : 0); }
            d = (DESCR_t){ .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
        }
        c->cur += sizeof(rt_dcap_e);
        if (ecell) { if (d.v == DT_S) rt_sxt_break_fast(d.s); *ecell = d; continue; }
        if (e->varname && e->varname[0] == '*') {
            extern int rt_proc_is_registered(const char *);
            extern long rt_dcap_call_prepare(const char *, short *, int *, int *);
            extern long rt_dcap_call_prepare_rec(sno_dstar_rec_t *, short *, int *, int *);
            sno_dstar_rec_t *srec = ((unsigned char)e->varname[1] == 1) ? (sno_dstar_rec_t *)(void *)e->varname : (sno_dstar_rec_t *)0;
            const char *star = srec ? srec->star : e->varname;
            const char *pn = star + 1;
#if RT_DIAG
            _prev_star = 1;
#endif
            c->pending = d;
            c->star = srec ? (const char *)(const void *)srec : star;
            c->wsv = rt_g_want_name;
            c->asv = g_cap_abort_gen;
            c->how = 0;
            c->nsb = 0;
            rt_g_want_name = 1;
            {
                int reg = 0;
                long fn = srec ? rt_dcap_call_prepare_rec(srec, &c->how, &c->nsb, &reg) : rt_dcap_call_prepare(pn, &c->how, &c->nsb, &reg);
                if (!reg) { if (rt_dcap_star_finish(c, NV_GET_fn(pn))) return (rt_dcap_next_t){ 1, 0 }; continue; }
                if (!fn) { if (rt_dcap_star_finish(c, FAILDESCR)) return (rt_dcap_next_t){ 1, 0 }; continue; }
                return (rt_dcap_next_t){ fn, (long)c->how };
            }
        }
        if (e->varname && e->varname[0]) {
            DESCR_t *cell0;
            {
                extern unsigned long g_nv_memo_gen;
                unsigned _i = (unsigned)((((uintptr_t)e->varname * 0x9E3779B97F4A7C15ull) >> 32) & (RT_DCAP_NVCACHE_N - 1));
                cell0 = (g_dcap_nv_key[_i] == e->varname && g_dcap_nv_seen[_i] == g_nv_memo_gen) ? g_dcap_nv_cell[_i] : (DESCR_t *)0;
            }
            if (!cell0 && g_protected_pat_vars_armed && is_protected_pat_lead(e->varname[0]) && is_protected_pat_name(e->varname)) {
                NV_SET_fn(e->varname, d);
            } else {
                DESCR_t *cell = cell0 ? cell0 : rt_dcap_nv_cell(e->varname);
                if (cell) {
                    if (d.v == DT_S) rt_sxt_break_fast(d.s);
                    *cell = d;
#if RT_DIAG
                    if (_cva) comm_var(e->varname, d, stmt_src_get_file(), 0, 0);
#endif
                } else {
                    NV_SET_fn(e->varname, d);
                }
            }
        }
    }
    return (rt_dcap_next_t){ (long)c->rc, 0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t c_rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj, rt_dcf_t *c) {
#if RT_DIAG
    {
        if (g_dcap_trace < 0) { const char *_e = getenv("SCRIP_DCAP_TRACE"); g_dcap_trace = (_e && _e[0]) ? 1 : 0; }
        if (g_dcap_trace) fprintf(stderr, "[DCAP] end_ok n=%ld\n", (long)((top - mark) / (long)sizeof(rt_dcap_e)));
    }
#endif
    if (!c) { rt_bomb("c_rt_dcap_end_ok_open: no spine record -- release_pump carves one before the call"); return (rt_dcap_next_t){ 1, 0 }; }
    memset(c, 0, sizeof *c);
    c->pending = NULVCL;
    c->subj_v = DT_S;
    c->subj = subj;
    c->star_v = DT_S;
    c->star = (const char *)0;
    c->cur_v = DT_I;
    c->cur = mark;
    c->top_v = DT_I;
    c->top = top;
    c->nhr_v = DT_I;
    c->wa_v = DT_I;
    return rt_dcap_pump(c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t rt_dcap_land_γ(DESCR_t frame0, rt_dcf_t *c) {
    extern DESCR_t rt_proc_call_epilogue_γ(DESCR_t, long);
    extern DESCR_t rt_proc_call_epilogue_named_γ(const char *, long);
    extern DESCR_t rt_nret_fix_tiny(DESCR_t, int);
    if (!c) { rt_bomb("rt_dcap_land_γ: no spine record -- release_pump passes the one it carved"); return (rt_dcap_next_t){ 1, 0 }; }
    int how = c->how & 0x3f;
    DESCR_t nm = (how == 2) ? rt_nret_fix_tiny(frame0, 0) : (how == 1) ? rt_proc_call_epilogue_named_γ(dcf_name(c->star) + 1, (long)((c->how >> 7) & 1)) :
        rt_proc_call_epilogue_γ(frame0, (long)((c->how >> 6) & 1));
    dcf_stage_leave(c->star);
    if (rt_dcap_star_finish(c, nm)) return (rt_dcap_next_t){ 1, 0 };
    return rt_dcap_pump(c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t rt_dcap_land_ω(rt_dcf_t *c) {
    extern DESCR_t rt_proc_call_epilogue_ω(long);
    extern DESCR_t rt_proc_call_epilogue_named_ω(const char *, long);
    extern DESCR_t rt_ret_faildescr(void);
    if (!c) { rt_bomb("rt_dcap_land_ω: no spine record -- release_pump passes the one it carved"); return (rt_dcap_next_t){ 1, 0 }; }
    int how = c->how & 0x3f;
    DESCR_t nm = (how == 2) ? rt_ret_faildescr() : (how == 1) ? rt_proc_call_epilogue_named_ω(dcf_name(c->star) + 1, (long)((c->how >> 7) & 1)) : rt_proc_call_epilogue_ω((long)((c->how >> 6) & 1));
    dcf_stage_leave(c->star);
    if (rt_dcap_star_finish(c, nm)) return (rt_dcap_next_t){ 1, 0 };
    return rt_dcap_pump(c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_dcap_flush(void) { fprintf(stderr, "[DCAP] FATAL rt_dcap_flush: dead C-side flush called — the commit flush is box-driven since NCB-1c M3 (rt_dcap_end_ok_open/step/close)\n"); abort(); }
void rt_dcap_end_ok(void) { fprintf(stderr, "[DCAP] FATAL rt_dcap_end_ok: superseded by the box-driven pump (NCB-1c M3: rt_dcap_end_ok_open/step/close)\n"); abort(); }
uint32_t g_cap_gen = 1;
__attribute__((visibility("hidden"))) uint32_t g_cap_gen_next = 1;
typedef struct { DESCR_t matched; DESCR_t wsv; DESCR_t asv; DESCR_t nmy; } rt_capo_t;
_Static_assert(sizeof(rt_capo_t) == 64, "bb_match_capture.cpp carves the computed-name capture record as 64 bytes of spine cells");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long rt_cap_target_finish(DESCR_t nm, DESCR_t matched, int by_name) {
    extern DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
    if (IS_FAIL_fn(nm)) return rt_cap_fail_retreat() ? -1 : 0;
    if (rt_cap_name_strict() && !by_name) { g_cap_abort_gen = g_cap_gen; return 0; }
    if (IS_STR_fn(nm)) { const char *ns = VARVAL_fn(nm); if (ns && *ns) NV_SET_fn(ns, matched); } else rt_assign_var(nm, matched);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t c_rt_cap_open(const char *varname, int saved_delta, int cur_delta, int is_imm, rt_capo_t *rec) {
    extern rt_dcap_next_t rt_call_open_by_name(const char *, int);
    extern int rt_proc_is_registered(const char *);
    extern int rt_g_want_name;
    extern int rt_g_ret_by_name;
    (void)is_imm;
    if (!rec) { rt_bomb("c_rt_cap_open: no spine record -- the computed-name capture box carves one before the call"); return (rt_dcap_next_t){ 0, 0 }; }
    rec->matched = INTVAL(0);
    rec->wsv = INTVAL(0);
    rec->asv = INTVAL(0);
    rec->nmy = INTVAL(0);
    if (!varname || !*varname) return (rt_dcap_next_t){ 0, 0 };
    {
        int len = cur_delta - saved_delta;
        if (len < 0) len = 0;
        const char *base = Σ ? Σ + saved_delta : NULL;
        char *copy = rt_str_alloc(len);
        if (copy) { if (len > 0 && base) memcpy(copy, base, (size_t)len); copy[len] = '\0'; }
        {
            DESCR_t matched = { .v = DT_S, .slen = (uint32_t)len, .s = copy ? copy : "" };
            if (g_cap_abort_gen && g_cap_abort_gen == g_cap_gen) return (rt_dcap_next_t){ 0, 0 };
            if (varname[0] != '*') {
                rt_bomb("c_rt_cap_open: plain-name arm DELETED (s196 Lon one-to-maintain) — rt_cap_open in rtx_match.s is the sole spelling; this entry serves computed-name '*' targets only");
                return (rt_dcap_next_t){ 0, 0 };
            }
            {
                const char *tn = varname + 1;
                const int nmyield = tn[0] == 'E' && !strncmp(tn, "EXPRNM$", 7);
                int wsv = rt_g_want_name;
                if (!rt_proc_is_registered(tn)) {
                    rt_g_want_name = 1;
                    DESCR_t nm = NV_GET_fn(tn);
                    rt_g_want_name = wsv;
                    { int by_name = rt_g_ret_by_name || nmyield; rt_g_ret_by_name = 0; return (rt_dcap_next_t){ rt_cap_target_finish(nm, matched, by_name), 0 }; }
                }
                rec->matched = matched;
                rec->wsv = INTVAL((long long)wsv);
                rec->asv = INTVAL((long long)g_cap_abort_gen);
                rec->nmy = INTVAL((long long)nmyield);
                rt_g_want_name = 1;
                {
                    rt_dcap_next_t n = rt_call_open_by_name(tn, 0);
                    if (!n.fn) { rt_g_want_name = wsv; return (rt_dcap_next_t){ 0, 0 }; }
                    { extern void rt_eval_stage_enter(const char *); rt_eval_stage_enter(tn); }
                    return n;
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_cap_land_γ(DESCR_t frame0, long word, rt_capo_t *rec) {
    extern DESCR_t rt_call_land_γ(DESCR_t, long);
    extern int rt_g_want_name;
    extern int rt_g_ret_by_name;
    DESCR_t nm = rt_call_land_γ(frame0, word);
    { extern void rt_eval_stage_leave_word(long); rt_eval_stage_leave_word(word); }
    if (!rec) { rt_bomb("rt_cap_land_γ: no spine record -- the computed-name capture box passes the one it carved"); return 0; }
    {
        DESCR_t matched = rec->matched;
        int by_name;
        g_cap_abort_gen = (uint32_t)rec->asv.i;
        rt_g_want_name = (int)rec->wsv.i;
        by_name = rt_g_ret_by_name || (int)rec->nmy.i;
        rt_g_ret_by_name = 0;
        return rt_cap_target_finish(nm, matched, by_name);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_cap_land_ω(long word, rt_capo_t *rec) {
    extern DESCR_t rt_call_land_ω(long);
    extern int rt_g_want_name;
    extern int rt_g_ret_by_name;
    DESCR_t nm = rt_call_land_ω(word);
    { extern void rt_eval_stage_leave_word(long); rt_eval_stage_leave_word(word); }
    if (!rec) { rt_bomb("rt_cap_land_ω: no spine record -- the computed-name capture box passes the one it carved"); return 0; }
    g_cap_abort_gen = (uint32_t)rec->asv.i;
    rt_g_want_name = (int)rec->wsv.i;
    rt_g_ret_by_name = 0;
    return rt_cap_target_finish(nm, rec->matched, 0);
}
extern const char *Σ;
extern int Σlen;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_subject_load_nv(const char *name, void *slot) {
    DESCR_t v = NV_GET_fn(name ? name : "");
    if (IS_NAMEVAL(v)) v = NV_GET_fn(v.s);
    const char *s = "";
    int len = 0;
    if (v.v == DT_S || v.v == DT_SNUL) {
        s = v.s ? v.s : "";
        len = v.slen ? (int)v.slen : (int)strlen(s);
    } else if (IS_INT_fn(v)) {
        char *b = rt_str_alloc(31);
        snprintf(b, 32, "%lld", (long long)v.i);
        s = b;
        len = (int)strlen(b);
    }
    ((const char **)slot)[0] = s;
    *(int *)((char *)slot + 8) = len;
    Σ = s;
    Σlen = len;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_subject_load_lit(const char *s, void *slot) { if (!s) s = ""; int len = (int)strlen(s); ((const char **)slot)[0] = s; *(int *)((char *)slot + 8) = len; Σ = s; Σlen = len; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_concat_parts_d(void *parts, int n) {
    struct part_t { int tag; int pad; const char *s; } *p = (struct part_t *)parts;
    if (n < 0) n = 0;
    const char *vals[n + 1];
    int lens[n + 1];
    size_t total = 0;
    for (int i = 0; i < n; i++) {
        const char *s = "";
        int len = 0;
        if (p[i].tag == 0) {
            s = p[i].s ? p[i].s : "";
            len = (int)strlen(s);
        } else {
            DESCR_t v = NV_GET_fn(p[i].s ? p[i].s : "");
            if (IS_NAMEVAL(v)) v = NV_GET_fn(v.s);
            if (v.v == DT_BOOL) {
                s = v.i ? "True" : "False";
                len = (int)strlen(s);
            } else if (v.v == DT_S || v.v == DT_SNUL) {
                s = v.s ? v.s : "";
                len = v.slen ? (int)v.slen : (int)strlen(s);
            } else if (IS_INT_fn(v)) {
                char *b = rt_str_alloc(31);
                snprintf(b, 32, "%lld", (long long)v.i);
                s = b;
                len = (int)strlen(b);
            } else if (IS_REAL_fn(v)) {
                char *b = rt_str_alloc(39);
                gcvt(v.r, 14, b);
                s = b;
                len = (int)strlen(b);
            }
        }
        vals[i] = s;
        lens[i] = len;
        total += (size_t)len;
    }
    char *buf = rt_str_alloc((long)total);
    size_t off = 0;
    for (int i = 0; i < n; i++) { if (lens[i] > 0) memcpy(buf + off, vals[i], (size_t)lens[i]); off += (size_t)lens[i]; }
    buf[off] = '\0';
    DESCR_t d = { .v = DT_S, .slen = (uint32_t)total, .s = buf };
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_gvar_assign_concat_parts(const char *dst, void *parts, int n) {
    DESCR_t d = rt_concat_parts_d(parts, n);
    NV_SET_fn(dst ? dst : "", d);
#if RT_DIAG
    if (g_trace_budget != 0) sno_trace_value(dst ? dst : "", d);
#endif
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_at_cursor(const char *varname, int cur_delta) {
    if (!varname || !*varname) return;
    DESCR_t pos = { .v = DT_I, .i = (int64_t)cur_delta };
    if (varname[0] == '*') {
        extern int rt_proc_is_registered(const char *);
        extern DESCR_t rt_call_proc_descr(const char *, int);
        extern int rt_g_want_name;
        extern DESCR_t rt_assign_var(DESCR_t, DESCR_t);
        const char * pn = varname + 1;
        if (!rt_proc_is_registered(pn)) return;
        int wsv = rt_g_want_name;
        rt_g_want_name = 1;
        DESCR_t nm = RT_GC_CALLBACK(rt_call_proc_descr(pn, 0));
        rt_g_want_name = wsv;
        if (IS_FAIL_fn(nm)) return;
        if (IS_VARREF_fn(nm)) rt_assign_var(nm, pos);
        else if (nm.v == DT_S && nm.s && *nm.s) NV_SET_fn(nm.s, pos);
        return;
    }
    NV_SET_fn(varname, pos);
}
extern const char *Σ;
extern int Σlen;
typedef struct { DESCR_t val; int failed; int dtx_used; int64_t zero; } rt_dfx_t;
typedef struct { void *fn; long aux; } rt_defer_pr_t;
_Static_assert(sizeof(rt_dfx_t) == 32, "bb_match_defer.cpp carves the deferred-expression record as 32 bytes of spine: the value cell and a flags unit whose pointer word stays 0");
_Static_assert(__builtin_offsetof(rt_dfx_t, val) == 0, "rtx_match.s reads val at +0");
_Static_assert(__builtin_offsetof(rt_dfx_t, failed) == 16, "rtx_match.s reads failed at +16");
_Static_assert(__builtin_offsetof(rt_dfx_t, dtx_used) == 20, "rtx_match.s reads dtx_used at +20");
_Static_assert(sizeof(DESCR_t) == 16, "rtx_match.s assumes the 16-byte DESCR pair");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) rt_dfx_t *rt_dfx_init(rt_dfx_t *s) {
    if (!s) { rt_bomb("rt_dfx_init: no spine record -- the deferred-expression box carves one before the open"); return s; }
    s->val = NULVCL;
    s->failed = 0;
    s->dtx_used = 0;
    s->zero = 0;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_cas_gc_roots(void) {
    extern void rt_gc_visit_raw(const char **);
    long b = 0;
    for (int c = 0; c < 2048; c += 8) {
        const uint64_t *w = &g_sno_defer_cells[c];
        if ((w[0] | w[1] | w[2] | w[3] | w[4] | w[5] | w[6] | w[7]) == 0) continue;
        for (int i = 0; i < 8; i++) if (w[i]) rt_gc_visit_raw((const char **)&w[i]);
    }
    for (int i = 0; i < g_sno_defer_pair_hwm; i++) { uint64_t *slot = &g_sno_defer_cells[2048 + i * 2]; if (slot[0] && slot[1]) rt_gc_visit_raw((const char **)&slot[1]); }
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_defer_nv_read(const char *name) {
    extern int rt_udc_on(void);
    if (name && name[0] == '&' && rt_udc_on() && NV_CONST_ASSIGNED_fn(name)) return NV_KW_GET_fn(name);
    return NV_GET_fn(name ? name : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_defer_expr_value(DESCR_t val) { if (val.v != DT_E) return val; val = EXPVAL_fn(val); return val; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int c_rt_defer_close(int cur_delta, rt_dfx_t *sp) {
    if (!sp) return -1;
    rt_dfx_t s = *sp;
    if (s.failed) return -1;
    DESCR_t val = s.val;
    if (IS_FAIL_fn(val)) return -1;
    val = rt_defer_expr_value(val);
    char nb[40];
    if (val.v == DT_I) {
        snprintf(nb, sizeof nb, "%lld", (long long)val.i);
        val.v = DT_S;
        val.slen = (uint32_t)strlen(nb);
        val.s = nb;
    } else if (val.v == DT_R) {
        real_str(val.r, nb, (int)sizeof nb);
        val.v = DT_S;
        val.slen = (uint32_t)strlen(nb);
        val.s = nb;
    }
    if (val.v == DT_S || val.v == DT_SNUL) {
        const char *lit = val.s ? val.s : "";
        int llen = (val.v == DT_SNUL || !val.s) ? 0 : (val.slen == 0xFFFFFFFFu ? (int)strlen(lit) : (int)val.slen);
        if (cur_delta + llen > Σlen) return -1;
        if (llen > 0 && memcmp(Σ + cur_delta, lit, (size_t)llen) != 0) return -1;
        return cur_delta + llen;
    }
    return -1;
}
#define RT_XPAT_CHAIN_MAX 256
static DESCR_t patv_slot(void *hv, long i, const char *fb, int ival_flag);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rt_dcap_next_t rt_defer_resolve(rt_dfx_t *s, DESCR_t r) {
    extern rt_dcap_next_t rt_call_open_by_name(const char *, int);
    extern int rt_proc_is_registered(const char *);
    extern DESCR_t EXPVAL_fn(DESCR_t);
    extern void *dtp_fn_of(void *);
    for (int _g = 0; _g < RT_XPAT_CHAIN_MAX; _g++) {
        if (IS_FAIL_fn(r)) { s->failed = 1; return (rt_dcap_next_t){ 0, 0 }; }
        if (r.v == DT_E) { r = EXPVAL_fn(r); continue; }
        if (r.v == DT_X) {
            sno_dstar_rec_t *rec = SNO_DTX_REC(r);
            s->dtx_used = 1;
            if (!rec) { s->failed = 1; return (rt_dcap_next_t){ 0, 0 }; }
            if (rec->flags & SNO_DSTAR_VARREF) { r = NV_GET_fn(rec->star + 1); continue; }
            {
                extern rt_dcap_next_t rt_call_open_staged_rec(sno_dstar_rec_t *, int *);
                int reg = 0;
                rt_dcap_next_t n = rt_call_open_staged_rec(rec, &reg);
                if (!reg) { r = NV_GET_fn(rec->star + 1); continue; }
                if (!n.fn) { s->failed = 1; return (rt_dcap_next_t){ 0, 0 }; }
                return n;
            }
        }
        if (r.v == DT_P && r.p) { dtp_fn_of(r.p); if (*(void **)r.p) return (rt_dcap_next_t){ (long)(uintptr_t)r.p, 4 }; s->failed = 1; return (rt_dcap_next_t){ 0, 0 }; }
        s->val = r;
        return (rt_dcap_next_t){ 0, 0 };
    }
    s->failed = 1;
    return (rt_dcap_next_t){ 0, 0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t rt_defer_open_cell(DESCR_t *cell, int ival_flag, rt_dfx_t *rec) {
    rt_dfx_t *s = rt_dfx_init(rec);
    DESCR_t val = cell ? *cell : NULVCL;
    if (ival_flag) { if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s); else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val); }
    return rt_defer_resolve(s, val);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t rt_defer_open_entry(const char *varname, int ival_flag, rt_dfx_t *rec) {
    rt_dfx_t *s = rt_dfx_init(rec);
    if (varname && varname[0] == 'F' && !strcmp(varname, "FAIL")) { s->failed = 1; return (rt_dcap_next_t){ 0, 0 }; }
    if (varname && varname[0] == '*') {
        extern void *bb_dstar_rec_intern(const char *, uint32_t);
        DESCR_t x = NULVCL;
        x.v = DT_X;
        x.slen = 0;
        x.p = bb_dstar_rec_intern(varname, 0u);
        return rt_defer_resolve(s, x);
    }
    {
        DESCR_t val = rt_defer_nv_read(varname ? varname : "");
        if (ival_flag) { if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s); else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val); }
        return rt_defer_resolve(s, val);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t rt_defer_open_rec(sno_dstar_rec_t *r, rt_dfx_t *rec) { rt_dfx_t *s = rt_dfx_init(rec); DESCR_t x = NULVCL; x.v = DT_X; x.slen = 0; x.p = r; return rt_defer_resolve(s, x); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t rt_patv_defer_open_entry(void *hv, long i, const char *fb, int ival_flag, rt_dfx_t *rec) {
    rt_dfx_t *s = rt_dfx_init(rec);
    DESCR_t val;
    { DTP_t *h = (DTP_t *)hv; if (h && h->snap && i >= 0 && i < h->nsnap) val = h->snap[i]; else val = patv_slot(hv, i, fb, ival_flag); }
    return rt_defer_resolve(s, val);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t rt_defer_land_γ(DESCR_t frame0, long word, rt_dfx_t *s) {
    extern DESCR_t rt_call_land_γ(DESCR_t, long);
    if (!s) { rt_bomb("rt_defer_land_γ: no spine record -- the deferred-expression box passes the one it carved"); return (rt_dcap_next_t){ 0, 0 }; }
    { DESCR_t r = rt_call_land_γ(frame0, word); { extern void rt_eval_stage_leave_word(long); rt_eval_stage_leave_word(word); } return rt_defer_resolve(s, r); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_dcap_next_t rt_defer_land_ω(long word, rt_dfx_t *s) {
    extern DESCR_t rt_call_land_ω(long);
    if (!s) { rt_bomb("rt_defer_land_ω: no spine record -- the deferred-expression box passes the one it carved"); return (rt_dcap_next_t){ 0, 0 }; }
    { DESCR_t r = rt_call_land_ω(word); { extern void rt_eval_stage_leave_word(long); rt_eval_stage_leave_word(word); } return rt_defer_resolve(s, r); }
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
        clen = (arg.v == DT_S && arg.slen && arg.slen != 0xFFFFFFFFu) ? (int)arg.slen : (int)strlen(cv);
    }
    *out_ptr = cv;
    *out_len = clen;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cset_has(const char *cv, int clen, unsigned char ch) { return cv && clen > 0 && memchr(cv, ch, (size_t)clen) != NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_patv_freeze(void *hv, const char *bn, long n) {
    DTP_t *h = (DTP_t *)hv;
    if (!h || !bn || n <= 0) return;
    DESCR_t *v = (DESCR_t *)rt_ws_alloc_descr((size_t)n);
    for (long i = 0; i < n; i++) { char nb[fmt_len("%s$V%ld", bn, i)]; snprintf(nb, sizeof nb, "%s$V%ld", bn, i); v[i] = NV_GET_fn(nb); }
    h->snap = v;
    h->nsnap = n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_sno_mkpat_d(DESCR_t *args, int nargs) {
    const sno_thunk_rec_t *r = (nargs >= 1 && args[0].v == DT_I) ? (const sno_thunk_rec_t *)(uintptr_t)args[0].i : (const sno_thunk_rec_t *)0;
    if (!r || !r->fn) return FAILDESCR;
    DESCR_t pd = {0};
    pd.v = DT_P;
    pd.slen = 0;
    pd.p = dtp_wrap_fn_sz(r->fn, (int64_t)r->frame_bytes, r->zstatic);
    if (nargs > 1) {
        long n = (long)nargs - 1;
        DESCR_t *v = (DESCR_t *)rt_ws_alloc_descr((size_t)n);
        for (long i = 0; i < n; i++) v[i] = args[1 + i];
        ((DTP_t *)pd.p)->snap = v;
        ((DTP_t *)pd.p)->nsnap = n;
    }
    return pd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t patv_slot(void *hv, long i, const char *fb, int ival_flag) {
    DTP_t *h = (DTP_t *)hv;
    if (h && h->snap && i >= 0 && i < h->nsnap) return h->snap[i];
    if (!fb) return NULVCL;
    { DESCR_t val = rt_defer_nv_read(fb); if (ival_flag) { if (IS_NAMEVAL(val)) val = NV_GET_fn(val.s); else if (IS_NAMEPTR(val)) val = NAME_DEREF_PTR(val); } return val; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_patv_defer_get_pat_dtp(void *hv, long i, const char *fb) {
    { DTP_t *h = (DTP_t *)hv; if (h && h->snap && i >= 0 && i < h->nsnap) { DESCR_t sv = h->snap[i]; if (sv.v == DT_P && sv.p && ((DTP_t *)sv.p)->fn) return sv.p; } }
    { DESCR_t v = patv_slot(hv, i, fb, 0); if (v.v == DT_P && v.p) { extern void *dtp_fn_of(void *); dtp_fn_of(v.p); return v.p; } }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int rt_defer_merge_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_DEFER_MERGE"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_defer_close_v(int cur_delta, DESCR_t val) {
    val = rt_defer_expr_value(val);
    if (IS_FAIL_fn(val)) return -1;
    char nb[40];
    if (val.v == DT_I) {
        snprintf(nb, sizeof nb, "%lld", (long long)val.i);
        val.v = DT_S;
        val.slen = (uint32_t)strlen(nb);
        val.s = nb;
    } else if (val.v == DT_R) {
        real_str(val.r, nb, (int)sizeof nb);
        val.v = DT_S;
        val.slen = (uint32_t)strlen(nb);
        val.s = nb;
    }
    if (val.v == DT_S || val.v == DT_SNUL) {
        const char *lit = val.s ? val.s : "";
        int llen = val.slen ? (int)val.slen : (int)strlen(lit);
        if (cur_delta + llen > Σlen) return -1;
        if (llen == 1) { if (Σ[cur_delta] != lit[0]) return -1; } else if (llen > 0 && strncmp(Σ + cur_delta, lit, (size_t)llen) != 0) return -1;
        return cur_delta + llen;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_defer_run_all_v(const char *varname, int cur_delta, DESCR_t val) { if (varname && varname[0] == 'F' && !strcmp(varname, "FAIL")) return -1; return rt_defer_close_v(cur_delta, val); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) DESCR_t *rt_defer_cell_ptr(const char *varname, long site) {
    extern DESCR_t *NV_PTR_fn(const char *name);
    extern DESCR_t *NV_CELL_IF_FASTSET_fn(const char *);
    extern uint64_t g_sno_defer_cells[4096];
    if (site < 0 || site >= 1024 || !varname || varname[0] == '&' || varname[0] == '*') return (DESCR_t *)0;
    uint64_t *slot = &g_sno_defer_cells[2048 + site * 2];
    if (slot[0] == (uint64_t)(uintptr_t)varname) return (DESCR_t *)(uintptr_t)slot[1];
    DESCR_t *cell = NV_PTR_fn(varname);
    if (!cell || !NV_CELL_IF_FASTSET_fn(varname)) return (DESCR_t *)0;
    slot[0] = (uint64_t)(uintptr_t)varname;
    slot[1] = (uint64_t)(uintptr_t)cell;
    if ((int)site >= g_sno_defer_pair_hwm) g_sno_defer_pair_hwm = (int)site + 1;
    return cell;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rt_defer_pr_t rt_defer_probe_run(const char *varname, int cur_delta, long site) {
    extern void *dtp_fn_of(void *);
    rt_defer_pr_t r;
    r.fn = (void *)0;
    r.aux = 0;
    const int _merge = rt_defer_merge_on();
    if (_merge && varname && varname[0] != '*') {
        DESCR_t *cp = rt_defer_cell_ptr(varname, site);
        const int ok = cp != (DESCR_t *)0;
        DESCR_t cv = ok ? *cp : NULVCL;
        if (ok && cv.v != DT_P && cv.v != DT_X && cv.v != DT_E) { r.aux = (long)rt_defer_run_all_v(varname, cur_delta, cv); return r; }
        if (ok && cv.v == DT_P && cv.p) {
            void *fn = *(void **)cv.p;
            if (!fn) { dtp_fn_of(cv.p); fn = *(void **)cv.p; }
            if (fn) { r.fn = fn; r.aux = (long)(uintptr_t)cv.p; return r; }
            r.aux = -2;
            return r;
        }
    }
    if (!_merge || !varname || varname[0] == '*') { r.aux = -2; return r; }
    DESCR_t val = rt_defer_nv_read(varname);
    if (val.v == DT_P && val.p) { dtp_fn_of(val.p); void *fn = *(void **)val.p; if (fn) { r.fn = fn; r.aux = (long)(uintptr_t)val.p; return r; } r.aux = -2; return r; }
    if (val.v == DT_X || val.v == DT_E) { r.aux = -2; return r; }
    r.aux = (long)rt_defer_run_all_v(varname, cur_delta, val);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_match_value_get_pat_fn(DESCR_t *pval) { if (pval && pval->v == DT_P && pval->p) { extern void *dtp_fn_of(void *); return dtp_fn_of(pval->p); } return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_match_value_get_pat_dtp(DESCR_t *pval) { if (pval && pval->v == DT_P && pval->p) { extern void *dtp_fn_of(void *); dtp_fn_of(pval->p); return pval->p; } return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_match_value_open(DESCR_t *pval, rt_dfx_t *rec) {
    rt_dfx_t *s = rt_dfx_init(rec);
    if (!s) return 0;
    DESCR_t val = pval ? *pval : NULVCL;
    if (IS_FAIL_fn(val)) { s->failed = 1; return 0; }
    s->val = val;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t c_rt_subscript_var_s(DESCR_t base, DESCR_t idx, int strict) {
    DESCR_t bvar = base;
    if (IS_VARREF_fn(base)) base = rt_deref(base);
    if ((base.v == DT_SNUL || IS_PROCVAL_fn(base)) && strict) { core_icn_op_ctx("[]", 2, base, idx); core_icn_error(114, base); core_icn_op_ctx_clear(); return FAILDESCR; }
    if (base.v == DT_A) {
        ARBLK_t *a = base.arr;
        if (!a) return FAILDESCR;
        if (!icn_index_operand_ok(base, idx, strict)) return FAILDESCR;
        int i = (int)to_int(idx);
        int off = i - a->lo;
        if (off < 0 || off >= (a->hi - a->lo + 1)) return FAILDESCR;
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
        vc->cellp = &a->data[off];
        vc->tbl = 0;
        vc->key = 0;
        vc->key_d = idx;
        vc->sv = FAILDESCR;
        vc->pos = 0;
        vc->len = 0;
        return NAMETRAP(vc);
    }
    if (base.v == DT_T) {
        TBBLK_t *tb = base.tbl;
        if (!tb) return FAILDESCR;
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
        vc->cellp = 0;
        vc->tbl = tb;
        vc->key = 0;
        vc->key_d = idx;
        vc->sv = FAILDESCR;
        vc->pos = 0;
        vc->len = 0;
        return NAMETRAP(vc);
    }
    if (base.v == DT_DATA) {
        DESCR_t *elems;
        int n;
        if (rt_list_view(base, &elems, &n)) {
            if (!icn_index_operand_ok(base, idx, strict)) return FAILDESCR;
            int i = (int)to_int(idx);
            if (i < 0) i = n + i + 1;
            if (!elems || i < 1 || i > n) return FAILDESCR;
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
            vc->cellp = &elems[i - 1];
            vc->tbl = 0;
            vc->key = 0;
            vc->key_d = idx;
            vc->sv = FAILDESCR;
            vc->pos = i;
            vc->len = 0;
            return NAMETRAP(vc);
        }
        if (base.u && base.u->type && base.u->type->nfields > 0 && base.u->fields) {
            DATBLK_t *blk = base.u->type;
            int f = -1;
            if (idx.v == DT_SNUL && !icn_index_operand_ok(base, idx, strict)) return FAILDESCR;
            if (IS_INT_fn(idx)) {
                int i = (int)idx.i;
                if (i <= 0) i = blk->nfields + 1 + i;
                if (i < 1 || i > blk->nfields) return FAILDESCR;
                f = i - 1;
            } else if (idx.v == DT_S || idx.v == DT_SNUL) {
                const char *k = idx.s ? idx.s : "";
                for (int i = 0; i < blk->nfields; i++) if (blk->fields[i] && strcmp(blk->fields[i], k) == 0) { f = i; break; }
                if (f < 0) return FAILDESCR;
            } else {
                if (!icn_index_operand_ok(base, idx, strict)) return FAILDESCR;
                return subscript_get_s(base, idx, strict);
            }
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
            vc->cellp = &base.u->fields[f];
            vc->tbl = 0;
            vc->key = 0;
            vc->key_d = idx;
            vc->sv = base;
            vc->pos = -(f + 1);
            vc->len = 0;
            return NAMETRAP(vc);
        }
        return subscript_get_s(base, idx, strict);
    }
    if ((base.v == DT_S || base.v == DT_SNUL) && !IS_CSET_fn(base) && IS_VARREF_fn(bvar)) {
        const char *sp = base.s ? base.s : "";
        long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        if (!icn_index_operand_ok(base, idx, strict)) return FAILDESCR;
        long i = (long)to_int(idx);
        if (i <= 0) i = slen + 1 + i;
        if (i < 1 || i > slen) return FAILDESCR;
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
        vc->cellp = 0;
        vc->tbl = 0;
        vc->key = 0;
        vc->key_d = idx;
        vc->sv = bvar;
        vc->pos = i;
        vc->len = 1;
        return NAMETRAP(vc);
    }
    return subscript_get_s(base, idx, strict);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_svco_miss_d(TBBLK_t *tb) { if (!tb) return FAILDESCR; if (tb->dflt.v != DT_FAIL && tb->dflt.v != 0) return tb->dflt; return NULVCL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t c_rt_subscript_var_container_only_s(DESCR_t base, DESCR_t idx, int strict) {
    extern int kwb_error(int code, const char *msg);
    DESCR_t b = base;
    if (IS_VARREF_fn(b)) b = rt_deref(b);
    if ((b.v == DT_SNUL || IS_PROCVAL_fn(b)) && strict) { core_icn_op_ctx("[]", 2, b, idx); core_icn_error(114, b); core_icn_op_ctx_clear(); return FAILDESCR; }
    if (b.v != DT_A && b.v != DT_T) { kwb_error(235, "subscripted operand is not table or array"); return FAILDESCR; }
    if (b.v == DT_T) {
        TBBLK_t *tb = b.tbl;
        if (!tb) return FAILDESCR;
        { TBPAIR_t *e = table_find_pair_d(tb, idx); if (e) return e->val; if (tb->dflt.v != DT_FAIL && tb->dflt.v != 0) return tb->dflt; return NULVCL; }
    }
    return rt_subscript_var(base, idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_table_assign_fast(DESCR_t base, DESCR_t idx, DESCR_t val) {
#if RT_DIAG
    extern int g_gc_pending;
    extern int g_sno_etrace_n;
#else
    extern int g_gc_pending;
    enum { g_sno_etrace_n = 0, g_trace_budget = 0 };
#endif
    if (g_gc_pending || g_sno_etrace_n != 0) { DESCR_t ref = rt_subscript_var(base, idx); if (ref.v == DT_FAIL) return ref; return rt_assign_var(ref, val); }
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    if (base.v == DT_A) {
        ARBLK_t *a = base.arr;
        if (a && a->ndim == 1 && a->data && idx.v == DT_I) {
            long off = (long)idx.i - (long)a->lo;
            if (off >= 0 && off <= (long)a->hi - (long)a->lo) { a->data[off] = val; if (g_trace_budget != 0) sno_trace_value("<lval>", val); return val; }
        }
        { DESCR_t ref = rt_subscript_var(base, idx); if (ref.v == DT_FAIL) return ref; return rt_assign_var(ref, val); }
    }
    table_set_descr_d(base.tbl, idx, val);
    if (g_trace_budget != 0) sno_trace_value("<lval>", val);
    return val;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_nd2_fast(DESCR_t base, DESCR_t idx1, DESCR_t idx2, DESCR_t *out) {
    DESCR_t b = base;
    if (IS_VARREF_fn(b)) b = rt_deref(b);
    if (b.v != DT_A || !b.arr || b.arr->ndim != 2 || idx1.v != DT_I || idx2.v != DT_I) return 0;
    ARBLK_t *a = b.arr;
    int i = (int)idx1.i, j = (int)idx2.i, cols = a->hi2 - a->lo2 + 1, row = i - a->lo, col = j - a->lo2;
    if (row < 0 || row >= (a->hi - a->lo + 1) || col < 0 || col >= cols) return 0;
    int off = row * cols + col, total = (a->hi - a->lo + 1) * cols;
    if (off < 0 || off >= total) return 0;
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
    vc->cellp = &a->data[off];
    vc->tbl = 0;
    vc->key = 0;
    vc->key_d = idx2;
    vc->sv = FAILDESCR;
    vc->pos = 0;
    vc->len = 0;
    *out = NAMETRAP(vc);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_subscript_var2(DESCR_t base, DESCR_t idx1, DESCR_t idx2) {
    DESCR_t out;
    if (rt_nd2_fast(base, idx1, idx2, &out)) return out;
    DESCR_t hop1 = rt_subscript_var_container_only(base, idx1);
    if (hop1.v == DT_FAIL) return FAILDESCR;
    return rt_subscript_var_container_only(hop1, idx2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_subscript_var2_lv(DESCR_t base, DESCR_t idx1, DESCR_t idx2) {
    DESCR_t out;
    if (rt_nd2_fast(base, idx1, idx2, &out)) return out;
    DESCR_t hop1 = rt_subscript_var_container_only(base, idx1);
    if (hop1.v == DT_FAIL) return FAILDESCR;
    return rt_subscript_var(hop1, idx2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_data_is_record_inst(DESCR_t obj) { DESCR_t *e = 0; int n = 0; return IS_DATA_INST_fn(obj) && obj.u && obj.u->type && obj.u->type->name && !rt_list_view(obj, &e, &n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t icn_field_get(const char *fname, DESCR_t obj) {
    extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
    if (IS_VARREF_fn(obj)) obj = rt_deref(obj);
    if (!rt_data_is_record_inst(obj)) { core_icn_op_ctx(".", 2, obj, FAILDESCR); core_icn_error(107, obj); return FAILDESCR; }
    { DESCR_t *cell = data_field_ptr(fname ? fname : "", obj); if (!cell) { core_icn_op_ctx(".", 2, obj, FAILDESCR); core_icn_error(207, obj); return FAILDESCR; } return *cell; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t icn_field_get_at(const char *fname, DESCR_t obj, long at1) {
    long at = at1 - 1;
    if (IS_VARREF_fn(obj)) obj = rt_deref(obj);
    if (IS_DATA_INST_fn(obj) && obj.u && obj.u->fields) { DATBLK_t *t = obj.u->type; if (t && t->name && at < t->nfields && t->fields[at] && !strcmp(t->fields[at], fname)) return obj.u->fields[at]; }
    DESCR_t declined = { .v = RTX_NOT_HANDLED };
    return declined;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_field_var_cell(const char *fname, DESCR_t obj, DESCR_t *cell) {
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
    vc->cellp = cell;
    vc->tbl = 0;
    vc->key = 0;
    vc->key_d = FAILDESCR;
    vc->sv = FAILDESCR;
    vc->pos = 0;
    vc->len = 0;
    {
        const char *rn = (obj.u && obj.u->type && obj.u->type->name) ? obj.u->type->name : "record";
        const char *fn = fname ? fname : "";
        int rl = (int)strlen(rn);
        int fl = (int)strlen(fn);
        char *nb = rt_str_alloc(rl + fl + 1);
        memcpy(nb, rn, rl);
        nb[rl] = '.';
        memcpy(nb + rl + 1, fn, fl);
        nb[rl + 1 + fl] = 0;
        vc->key = nb;
    }
    return NAMETRAP(vc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_field_var(const char *fname, DESCR_t obj) {
    extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
    if (IS_VARREF_fn(obj)) obj = rt_deref(obj);
    {
        extern int rt_field_index_cached(const char *, const DATBLK_t *);
        DESCR_t *cell = (DESCR_t *)0;
        if (fname && IS_DATA_INST_fn(obj) && obj.u && obj.u->type && obj.u->fields) {
            int fi = rt_field_index_cached(fname, obj.u->type);
            if (fi >= 0 && rt_data_is_record_inst(obj)) cell = &obj.u->fields[fi];
        }
        if (!cell) cell = rt_data_is_record_inst(obj) ? data_field_ptr(fname ? fname : "", obj) : (DESCR_t *)0;
        if (!cell) { core_runtime_error(41, "field function argument is wrong datatype"); return FAILDESCR; }
#if RT_DIAG
        if (!comm_var_active()) return (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *)cell };
#else
        return (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *)cell };
#endif
        return rt_field_var_cell(fname, obj, cell);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_field_var_strict(const char *fname, DESCR_t obj) {
    extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
    if (IS_VARREF_fn(obj)) obj = rt_deref(obj);
    if (!rt_data_is_record_inst(obj)) { core_icn_op_ctx(".", 2, obj, FAILDESCR); core_icn_error(107, obj); return FAILDESCR; }
    {
        DESCR_t *cell = data_field_ptr(fname ? fname : "", obj);
        if (!cell) { core_icn_op_ctx(".", 2, obj, FAILDESCR); core_icn_error(207, obj); return FAILDESCR; }
        return rt_field_var_cell(fname, obj, cell);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_field_var_strict_at(const char *fname, DESCR_t obj, long at1) {
    long at = at1 - 1;
    if (IS_VARREF_fn(obj)) obj = rt_deref(obj);
    if (IS_DATA_INST_fn(obj) && obj.u && obj.u->fields) {
        DATBLK_t *t = obj.u->type;
        if (t && t->name && at < t->nfields && t->fields[at] && !strcmp(t->fields[at], fname)) return rt_field_var_cell(fname, obj, &obj.u->fields[at]);
    }
    DESCR_t declined = { .v = RTX_NOT_HANDLED };
    return declined;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_list_bang_var_body(DESCR_t obj, int64_t idx, int elems_only);
DESCR_t rt_list_bang_var_at(DESCR_t obj, int64_t idx) { return rt_list_bang_var_body(obj, idx, 0); }
static DESCR_t rt_list_bang_var_body(DESCR_t obj, int64_t idx, int elems_only) {
    DESCR_t bvar = obj;
    if (IS_VARREF_fn(obj)) obj = rt_deref(obj);
    if (obj.v == DT_DATA) {
        DESCR_t *elems;
        int n;
        if (rt_list_view(obj, &elems, &n)) {
            if (!elems || idx < 0 || idx >= n) return FAILDESCR;
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
            vc->cellp = &elems[idx];
            vc->tbl = 0;
            vc->key = 0;
            vc->key_d = FAILDESCR;
            vc->sv = FAILDESCR;
            vc->pos = 0;
            vc->len = 0;
            return NAMETRAP(vc);
        }
        if (obj.u && obj.u->type && obj.u->type->nfields > 0) {
            int nf = obj.u->type->nfields;
            if (idx < 0 || idx >= nf) return FAILDESCR;
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
            vc->cellp = &obj.u->fields[idx];
            vc->tbl = 0;
            vc->key = 0;
            vc->key_d = FAILDESCR;
            vc->sv = FAILDESCR;
            vc->pos = 0;
            vc->len = 0;
            {
                const char *rn = obj.u->type->name ? obj.u->type->name : "record";
                const char *fn = (obj.u->type->fields && obj.u->type->fields[idx]) ? obj.u->type->fields[idx] : "";
                int rl = (int)strlen(rn);
                int fl = (int)strlen(fn);
                char *nb = rt_str_alloc(rl + fl + 1);
                memcpy(nb, rn, rl);
                nb[rl] = '.';
                memcpy(nb + rl + 1, fn, fl);
                nb[rl + 1 + fl] = 0;
                vc->key = nb;
            }
            return NAMETRAP(vc);
        }
        { extern DESCR_t rt_list_bang_at(DESCR_t, int64_t); return rt_list_bang_at(bvar, idx); }
    }
    if (obj.v == DT_T && obj.tbl) {
        TBBLK_t *tbl = obj.tbl;
        int64_t seen = 0;
        TBPAIR_t *ep;
        TBL_FOREACH(tbl, ep) {
            if (seen == idx) {
                VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
                vc->cellp = 0;
                vc->tbl = tbl;
                vc->key = 0;
                vc->key_d = ep->key_descr;
                vc->sv = FAILDESCR;
                vc->pos = 0;
                vc->len = 0;
                return NAMETRAP(vc);
            }
            seen++;
        }
        return FAILDESCR;
    }
    if ((obj.v == DT_S || obj.v == DT_SNUL) && !IS_CSET_fn(obj)) {
        const char *sp = obj.s ? obj.s : "";
        long slen = obj.slen ? (long)obj.slen : (long)strlen(sp);
        if (idx < 0 || idx >= slen) return FAILDESCR;
        if (!elems_only && IS_VARREF_fn(bvar)) {
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
            vc->cellp = 0;
            vc->tbl = 0;
            vc->key = 0;
            vc->key_d = FAILDESCR;
            vc->sv = bvar;
            vc->pos = idx + 1;
            vc->len = 1;
            return NAMETRAP(vc);
        }
        char *out = rt_str_alloc(1);
        out[0] = sp[idx];
        out[1] = 0;
        return (DESCR_t){ .v = DT_S, .slen = 1, .s = out };
    }
    { extern DESCR_t rt_list_bang_at(DESCR_t, int64_t); return rt_list_bang_at(obj, idx); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_list_bang_elem_at(DESCR_t obj, int64_t idx) { return rt_list_bang_var_body(obj, idx, 1); }
DESCR_t rt_var_table_elem(DESCR_t d, DESCR_t *tbl_out, DESCR_t *key_out) {
    if (!IS_NAMETRAP_fn(d)) { if (tbl_out) tbl_out->v = DT_FAIL; return FAILDESCR; }
    VCELL_t *vc = (VCELL_t *)d.p;
    if (!vc || !vc->tbl) { if (tbl_out) tbl_out->v = DT_FAIL; return FAILDESCR; }
    if (tbl_out) { DESCR_t t; memset(&t, 0, sizeof t); t.v = DT_T; t.slen = 0; t.tbl = vc->tbl; *tbl_out = t; }
    if (key_out) *key_out = vc->key_d;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_random_var_body(DESCR_t base, int strict);
static DESCR_t rt_random_var_s(DESCR_t base, int strict) { extern long g_random; long saved = g_random; DESCR_t r = rt_random_var_body(base, strict); if (r.v == DT_FAIL) g_random = saved; return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_random_var_body(DESCR_t base, int strict) {
    extern long g_random;
    DESCR_t bvar = base;
    if (IS_VARREF_fn(base)) base = rt_deref(base);
    if ((base.v == DT_SNUL || IS_PROCVAL_fn(base)) && strict) { core_icn_op_ctx("?", 1, base, base); core_icn_error(113, base); core_icn_op_ctx_clear(); return FAILDESCR; }
    if (base.v == DT_R) { if (base.r >= 9223372036854775808.0 || base.r <= -9223372036854775808.0) return FAILDESCR; base = INTVAL((int64_t)base.r); bvar = base; }
    g_random = (1103515245L * g_random + 453816694L) & 0x7FFFFFFFL;
    double rval = 4.65661286e-10 * (double)g_random;
    if (base.v == DT_S && base.slen == 0xFFFFFFFFu) {
        const char *cp;
        int clen;
        if (!cset_resolve(base, &cp, &clen) || clen <= 0) return FAILDESCR;
        long i = (long)(rval * (double)clen);
        char *one = rt_str_alloc(1);
        one[0] = cp[i];
        one[1] = 0;
        return (DESCR_t){ .v = DT_S, .slen = 1, .s = one };
    }
    if ((base.v == DT_S || base.v == DT_SNUL) && IS_VARREF_fn(bvar)) {
        const char *sp = base.s ? base.s : "";
        long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        if (slen <= 0) return FAILDESCR;
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
        vc->cellp = 0;
        vc->tbl = 0;
        vc->key = 0;
        vc->key_d = FAILDESCR;
        vc->sv = bvar;
        vc->pos = (long)(rval * (double)slen) + 1;
        vc->len = 1;
        return NAMETRAP(vc);
    }
    if (base.v == DT_S || base.v == DT_SNUL) {
        const char *sp = base.s ? base.s : "";
        long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        if (slen <= 0) return FAILDESCR;
        long i = (long)(rval * (double)slen);
        return (DESCR_t){ .v = DT_S, .slen = 1, .s = (char *)sp + i };
    }
    if (base.v == DT_DATA) {
        DESCR_t *elems;
        int n;
        if (rt_list_view(base, &elems, &n)) {
            if (!elems || n <= 0) return FAILDESCR;
            long i = (long)(rval * (double)n);
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
            vc->cellp = &elems[i];
            vc->tbl = 0;
            vc->key = 0;
            vc->key_d = FAILDESCR;
            vc->sv = FAILDESCR;
            vc->pos = 0;
            vc->len = 0;
            return NAMETRAP(vc);
        }
        if (base.u && base.u->type && base.u->type->nfields > 0) {
            int nf = base.u->type->nfields;
            long i = (long)(rval * (double)nf);
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
            vc->cellp = &base.u->fields[i];
            vc->tbl = 0;
            vc->key = 0;
            vc->key_d = FAILDESCR;
            vc->sv = FAILDESCR;
            vc->pos = 0;
            vc->len = 0;
            return NAMETRAP(vc);
        }
        return FAILDESCR;
    }
    if (base.v == DT_T && base.tbl) {
        TBBLK_t *tbl = base.tbl;
        if (tbl->size <= 0) return FAILDESCR;
        long n = (long)(rval * (double)tbl->size) + 1;
        long seen = 0;
        TBPAIR_t *ep;
        TBL_FOREACH(tbl, ep) if (++seen == n) {
            VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
            vc->cellp = 0;
            vc->tbl = tbl;
            vc->key = 0;
            vc->key_d = ep->key_descr;
            vc->sv = FAILDESCR;
            vc->pos = 0;
            vc->len = 0;
            return NAMETRAP(vc);
        }
        return FAILDESCR;
    }
    if (base.v == DT_I) { int64_t v = base.i; if (v < 0) return FAILDESCR; if (v == 0) return REALVAL(rval); return INTVAL((int64_t)(rval * (double)v) + 1); }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_section_var_s(DESCR_t base, DESCR_t i1d, DESCR_t i2d, int strict) {
    DESCR_t bvar = base;
    if (IS_VARREF_fn(base)) base = rt_deref(base);
    if ((base.v == DT_S || (base.v == DT_SNUL && base.s)) && !IS_CSET_fn(base) && IS_VARREF_fn(bvar)) {
        const char *sp = base.s ? base.s : "";
        long slen = base.slen ? (long)base.slen : (long)strlen(sp);
        if (!icn_section_operands_ok(base, i1d, i2d, strict)) return FAILDESCR;
        long ii = (long)to_int(i1d), jj = (long)to_int(i2d);
        if (ii < -slen || ii > slen + 1) return FAILDESCR;
        if (jj < -slen || jj > slen + 1) return FAILDESCR;
        if (ii <= 0) ii = slen + ii + 1;
        if (jj <= 0) jj = slen + jj + 1;
        if (ii > jj) { long t = ii; ii = jj; jj = t; }
        VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
        vc->cellp = 0;
        vc->tbl = 0;
        vc->key = 0;
        vc->key_d = i1d;
        vc->sv = bvar;
        vc->pos = ii;
        vc->len = jj - ii;
        return NAMETRAP(vc);
    }
    return subscript_get2_s(base, i1d, i2d, strict);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_var_ref_cell_named(DESCR_t *cellp, const char *name) {
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
    vc->cellp = cellp;
    vc->tbl = 0;
    vc->key = name;
    vc->key_d = FAILDESCR;
    vc->sv = FAILDESCR;
    vc->pos = 0;
    vc->len = 0;
    return NAMETRAP(vc);
}
DESCR_t rt_var_ref_cell(DESCR_t *cellp) {
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
    vc->cellp = cellp;
    vc->tbl = 0;
    vc->key = 0;
    vc->key_d = FAILDESCR;
    vc->sv = FAILDESCR;
    vc->pos = 0;
    vc->len = 0;
    return NAMETRAP(vc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_var(const char *name) {
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
    vc->cellp = 0;
    vc->tbl = 0;
    vc->key = name;
    vc->key_d = FAILDESCR;
    vc->sv = FAILDESCR;
    vc->pos = -1;
    vc->len = 0;
    return NAMETRAP(vc);
}
int rt_var_is_keyword(DESCR_t d, const char **name_out) {
    if (!IS_NAMETRAP_fn(d)) return 0;
    VCELL_t *vc = (VCELL_t *)d.p;
    if (!vc || vc->cellp || vc->tbl || !vc->key || vc->pos != -1) return 0;
    if (name_out) *name_out = vc->key;
    return 1;
}
int rt_var_substring(DESCR_t d, DESCR_t *base_out, long *pos_out, long *len_out) {
    if (!IS_NAMETRAP_fn(d)) return 0;
    VCELL_t *vc = (VCELL_t *)d.p;
    if (!vc || vc->cellp || vc->pos <= 0 || !IS_VARREF_fn(vc->sv)) return 0;
    DESCR_t base = vc->sv;
    long pos = vc->pos, len = vc->len;
    while (IS_NAMETRAP_fn(base)) { VCELL_t *bv = (VCELL_t *)base.p; if (!bv || bv->cellp || bv->pos <= 0 || !IS_VARREF_fn(bv->sv)) break; pos += bv->pos - 1; base = bv->sv; }
    if (base_out) *base_out = base;
    if (pos_out) *pos_out = pos;
    if (len_out) *len_out = len;
    return 1;
}
static const char k_one_char_str[513] =
    "\000\000" "\001\000" "\002\000" "\003\000" "\004\000" "\005\000" "\006\000" "\007\000" "\010\000" "\011\000" "\012\000" "\013\000" "\014\000" "\015\000" "\016\000" "\017\000" "\020\000"
    "\021\000" "\022\000" "\023\000" "\024\000" "\025\000" "\026\000" "\027\000" "\030\000" "\031\000" "\032\000" "\033\000" "\034\000" "\035\000" "\036\000" "\037\000" "\040\000" "\041\000"
    "\042\000" "\043\000" "\044\000" "\045\000" "\046\000" "\047\000" "\050\000" "\051\000" "\052\000" "\053\000" "\054\000" "\055\000" "\056\000" "\057\000" "\060\000" "\061\000" "\062\000"
    "\063\000" "\064\000" "\065\000" "\066\000" "\067\000" "\070\000" "\071\000" "\072\000" "\073\000" "\074\000" "\075\000" "\076\000" "\077\000" "\100\000" "\101\000" "\102\000" "\103\000"
    "\104\000" "\105\000" "\106\000" "\107\000" "\110\000" "\111\000" "\112\000" "\113\000" "\114\000" "\115\000" "\116\000" "\117\000" "\120\000" "\121\000" "\122\000" "\123\000" "\124\000"
    "\125\000" "\126\000" "\127\000" "\130\000" "\131\000" "\132\000" "\133\000" "\134\000" "\135\000" "\136\000" "\137\000" "\140\000" "\141\000" "\142\000" "\143\000" "\144\000" "\145\000"
    "\146\000" "\147\000" "\150\000" "\151\000" "\152\000" "\153\000" "\154\000" "\155\000" "\156\000" "\157\000" "\160\000" "\161\000" "\162\000" "\163\000" "\164\000" "\165\000" "\166\000"
    "\167\000" "\170\000" "\171\000" "\172\000" "\173\000" "\174\000" "\175\000" "\176\000" "\177\000" "\200\000" "\201\000" "\202\000" "\203\000" "\204\000" "\205\000" "\206\000" "\207\000"
    "\210\000" "\211\000" "\212\000" "\213\000" "\214\000" "\215\000" "\216\000" "\217\000" "\220\000" "\221\000" "\222\000" "\223\000" "\224\000" "\225\000" "\226\000" "\227\000" "\230\000"
    "\231\000" "\232\000" "\233\000" "\234\000" "\235\000" "\236\000" "\237\000" "\240\000" "\241\000" "\242\000" "\243\000" "\244\000" "\245\000" "\246\000" "\247\000" "\250\000" "\251\000"
    "\252\000" "\253\000" "\254\000" "\255\000" "\256\000" "\257\000" "\260\000" "\261\000" "\262\000" "\263\000" "\264\000" "\265\000" "\266\000" "\267\000" "\270\000" "\271\000" "\272\000"
    "\273\000" "\274\000" "\275\000" "\276\000" "\277\000" "\300\000" "\301\000" "\302\000" "\303\000" "\304\000" "\305\000" "\306\000" "\307\000" "\310\000" "\311\000" "\312\000" "\313\000"
    "\314\000" "\315\000" "\316\000" "\317\000" "\320\000" "\321\000" "\322\000" "\323\000" "\324\000" "\325\000" "\326\000" "\327\000" "\330\000" "\331\000" "\332\000" "\333\000" "\334\000"
    "\335\000" "\336\000" "\337\000" "\340\000" "\341\000" "\342\000" "\343\000" "\344\000" "\345\000" "\346\000" "\347\000" "\350\000" "\351\000" "\352\000" "\353\000" "\354\000" "\355\000"
    "\356\000" "\357\000" "\360\000" "\361\000" "\362\000" "\363\000" "\364\000" "\365\000" "\366\000" "\367\000" "\370\000" "\371\000" "\372\000" "\373\000" "\374\000" "\375\000" "\376\000"
    "\377\000";
void rt_trace_deref_slot(DESCR_t *p) { if (p) *p = rt_deref(*p); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_deref_slow(DESCR_t d) {
    if (d.v == DT_N && d.slen == 0 && d.s && *d.s) { extern DESCR_t NV_GET_fn(const char *); return NV_GET_fn(d.s); }
    if (d.v == DT_N && d.slen == 1 && d.ptr) return *(DESCR_t *)d.ptr;
    if (!IS_NAMETRAP_fn(d)) return d;
    VCELL_t *vc = (VCELL_t *)d.p;
    if (!vc) return FAILDESCR;
    if (!vc->cellp && !vc->tbl && vc->key && vc->pos == -1) { extern DESCR_t rt_keyword_read(const char *); return rt_keyword_read(vc->key); }
    if (vc->cellp) return *vc->cellp;
    if (vc->tbl) {
        int found;
        DESCR_t hit = table_get_found_d(vc->tbl, vc->key_d, &found);
        if (found) return hit;
        if (vc->tbl->dflt.v != DT_FAIL && vc->tbl->dflt.v != 0) return vc->tbl->dflt;
        return NULVCL;
    }
    if (IS_VARREF_fn(vc->sv)) {
        DESCR_t sd = rt_deref(vc->sv);
        if (sd.v != DT_S && sd.v != DT_SNUL) return FAILDESCR;
        const char *sp = sd.s ? sd.s : "";
        long slen = sd.slen ? (long)sd.slen : (long)strlen(sp);
        if (vc->pos + vc->len - 1 > slen) return FAILDESCR;
        if (vc->len == 1) return (DESCR_t){ .v = DT_S, .slen = 1, .s = (char *)&k_one_char_str[2 * (unsigned char)sp[vc->pos - 1]] };
        char *out = rt_str_alloc(vc->len);
        memcpy(out, sp + vc->pos - 1, (size_t)vc->len);
        out[vc->len] = 0;
        return (DESCR_t){ .v = DT_S, .slen = (uint32_t)vc->len, .s = out };
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t c_rt_assign_var_body(DESCR_t var, DESCR_t val, int strict) {
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    if (var.v == DT_N && var.slen == 0 && var.s && *var.s) { extern DESCR_t NV_SET_fn(const char *, DESCR_t); NV_SET_fn(var.s, val); return val; }
#if RT_DIAG
    if (var.v == DT_N && var.slen == 1 && var.ptr) { extern void mon_tap_cell_store(void *, DESCR_t); *(DESCR_t *)var.ptr = val; if (monitor_fd >= 0) mon_tap_cell_store(var.ptr, val); return val; }
#else
    if (var.v == DT_N && var.slen == 1 && var.ptr) { *(DESCR_t *)var.ptr = val; return val; }
#endif
    if (!IS_NAMETRAP_fn(var)) {
        { extern int core_icn_error(int code, DESCR_t val); if (strict) { core_icn_error(111, var); return FAILDESCR; } }
        fprintf(stderr, "[IDX] BOMB rt_assign_var: lvalue is not a variable (dtype=%d) — string/record subscript assignment is the tvsubs rung (GOAL-IR-IMMUTABLE-EMIT IDX-UNIFY)\n", (int)var.v);
        abort();
    }
    VCELL_t *vc = (VCELL_t *)var.p;
    if (!vc) return FAILDESCR;
    if (!vc->cellp && !vc->tbl && vc->key && vc->pos == -1) {
        fprintf(stderr, "[IDX] BOMB rt_assign_var: assignment to keyword variable %s is not implemented (the keyword write also resets the scanning environment, so it is not a cell store)\n", vc->key)
            ;
        abort();
    }
#if RT_DIAG
    if (vc->cellp) { extern void mon_tap_cell_store(void *, DESCR_t); *vc->cellp = val; if (monitor_fd >= 0) mon_tap_cell_store((void *)vc->cellp, val); return val; }
#else
    if (vc->cellp) { *vc->cellp = val; return val; }
#endif
    if (vc->tbl) { table_set_descr_d(vc->tbl, vc->key_d, val); return val; }
    if (IS_VARREF_fn(vc->sv)) {
        char nb[64];
        const char *src;
        long srclen;
        int owned = 1;
        if (IS_CSET_fn(val)) {
            src = val.s ? val.s : "";
            srclen = (long)descr_slen(val);
            owned = 0;
        } else if (val.v == DT_S || val.v == DT_SNUL) {
            src = val.s ? val.s : "";
            srclen = val.slen ? (long)val.slen : (long)strlen(src);
        } else if (val.v == DT_I) {
            snprintf(nb, sizeof nb, "%lld", (long long)val.i);
            src = nb;
            srclen = (long)strlen(nb);
            owned = 0;
        } else if (val.v == DT_R) {
            snprintf(nb, sizeof nb, "%g", val.r);
            src = nb;
            srclen = (long)strlen(nb);
            owned = 0;
        } else if (val.v == DT_BIG) {
            src = VARVAL_fn(val);
            if (!src) src = "";
            srclen = (long)strlen(src);
            owned = 0;
        } else {
            fprintf(stderr, "[IDX] tvsubs assign: value not string-convertible (dtype=%d)\n", (int)val.v);
            return FAILDESCR;
        }
        DESCR_t sd = rt_deref(vc->sv);
        if (sd.v != DT_S && sd.v != DT_SNUL) return FAILDESCR;
        const char *sp = sd.s ? sd.s : "";
        long slen = sd.slen ? (long)sd.slen : (long)strlen(sp);
        long prelen = vc->pos - 1, poststrt = prelen + vc->len;
        if (poststrt > slen) { if (strict) { extern int core_icn_error(int code, DESCR_t val); core_icn_error(205, FAILDESCR); } return FAILDESCR; }
        long nlen = prelen + srclen + (slen - poststrt);
        char *ns = rt_str_alloc(nlen);
        memcpy(ns, sp, (size_t)prelen);
        memcpy(ns + prelen, src, (size_t)srclen);
        memcpy(ns + prelen + srclen, sp + poststrt, (size_t)(slen - poststrt));
        ns[nlen] = 0;
        DESCR_t nsd = (DESCR_t){ .v = DT_S, .slen = (uint32_t)nlen, .s = ns };
#if RT_DIAG
        long tb = g_trace_budget;
        g_trace_budget = 0;
#endif
        DESCR_t wr = rt_assign_var(vc->sv, nsd);
#if RT_DIAG
        g_trace_budget = tb;
#endif
        if (wr.v == DT_FAIL) return FAILDESCR;
        vc->len = srclen;
        if (owned) return (DESCR_t){ .v = DT_S, .slen = (uint32_t)srclen, .s = (char *)src };
        char *rs = rt_str_alloc(srclen);
        memcpy(rs, src, (size_t)srclen);
        rs[srclen] = 0;
        return (DESCR_t){ .v = DT_S, .slen = (uint32_t)srclen, .s = rs };
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t c_rt_assign_var_s(DESCR_t var, DESCR_t val, int strict) {
#if RT_DIAG
    int simple = (var.v == DT_N && var.slen == 0 && var.s && *var.s);
    DESCR_t r = c_rt_assign_var_body(var, val, strict);
    if (!simple && g_trace_budget != 0 && !IS_FAIL_fn(r)) {
        extern int monitor_fd;
        extern int mon_cell_is_named(void *);
        void *cp = (var.v == DT_N && var.slen == 1) ? var.ptr : (IS_NAMETRAP_fn(var) && var.p) ? (void *)((VCELL_t *)var.p)->cellp : (void *)0;
        if (!(monitor_fd >= 0 && cp && mon_cell_is_named(cp))) sno_trace_value("<lval>", val);
    }
    { extern int g_sno_etrace_n; extern void rt_sno_elem_store_trace(DESCR_t, DESCR_t); if (g_sno_etrace_n != 0 && !IS_FAIL_fn(r) && IS_NAMETRAP_fn(var)) rt_sno_elem_store_trace(var, val); }
    return r;
#else
    return c_rt_assign_var_body(var, val, strict);
#endif
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static VCELL_t * vcell_ultimate(DESCR_t d) { while (IS_NAMETRAP_fn(d)) { VCELL_t *vc = (VCELL_t *)d.p; if (!vc) return 0; if (IS_NAMETRAP_fn(vc->sv)) { d = vc->sv; continue; } return vc; } return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *swap_base_cell(DESCR_t sv) {
    if (sv.v == DT_N && sv.slen == 1) return (DESCR_t *)sv.ptr;
    if (IS_NAMETRAP_fn(sv)) { VCELL_t *u = (VCELL_t *)sv.p; if (u && u->cellp && !u->tbl) return u->cellp; }
    return (DESCR_t *)0;
}
static int swap_tvsubs_fits(const VCELL_t *c) {
    if (c->cellp || c->tbl || !IS_VARREF_fn(c->sv)) return 1;
    DESCR_t sd = rt_deref(c->sv);
    if (sd.v != DT_S && sd.v != DT_SNUL) return 1;
    long slen = sd.slen ? (long)sd.slen : (sd.s ? (long)strlen(sd.s) : 0);
    return c->pos + c->len - 1 <= slen;
}
DESCR_t rt_swap_var(DESCR_t va, DESCR_t vb) {
    if (!IS_VARREF_fn(va) || !IS_VARREF_fn(vb)) return FAILDESCR;
    if (!IS_NAMETRAP_fn(va) || !IS_NAMETRAP_fn(vb)) {
        if ((IS_NAMETRAP_fn(va) && va.p && !swap_tvsubs_fits((VCELL_t *)va.p)) || (IS_NAMETRAP_fn(vb) && vb.p && !swap_tvsubs_fits((VCELL_t *)vb.p))) {
            extern void core_icn_fatal(int code, DESCR_t val);
            core_icn_fatal(205, FAILDESCR);
            return FAILDESCR;
        }
        DESCR_t dx = rt_deref(va), dy = rt_deref(vb);
        { extern void rt_sxt_break(const char *); if (dx.v == DT_S) rt_sxt_break(dx.s); if (dy.v == DT_S) rt_sxt_break(dy.s); }
        if (dx.v == DT_FAIL || dy.v == DT_FAIL) return FAILDESCR;
        if (rt_assign_var(va, dy).v == DT_FAIL) return FAILDESCR;
        if (rt_assign_var(vb, dx).v == DT_FAIL) return FAILDESCR;
        return rt_deref(va);
    }
    VCELL_t *xc = (VCELL_t *)va.p, *yc = (VCELL_t *)vb.p;
    if (!xc || !yc) return FAILDESCR;
    {
        extern int g_sno_etrace_n;
#if !RT_DIAG
#define g_trace_budget 0
#define g_sno_etrace_n 0
#define monitor_fd (-1)
#endif
        if (xc->len == 1 && yc->len == 1 && xc->pos > 0 && yc->pos > 0 && !xc->cellp && !yc->cellp && !xc->tbl && !yc->tbl && g_trace_budget == 0 && g_sno_etrace_n == 0 && monitor_fd < 0) {
#if !RT_DIAG
#undef g_trace_budget
#undef g_sno_etrace_n
#undef monitor_fd
#endif
            DESCR_t *cx_cell = swap_base_cell(xc->sv), *cy_cell = swap_base_cell(yc->sv);
            if (cx_cell && cx_cell == cy_cell) {
                DESCR_t sd = *cx_cell;
                if (sd.v == DT_S && !IS_CSET_fn(sd) && sd.s) {
                    long slen = sd.slen ? (long)sd.slen : (long)strlen(sd.s);
                    if (xc->pos <= slen && yc->pos <= slen) {
                        char *ns = rt_str_alloc(slen);
                        memcpy(ns, sd.s, (size_t)slen);
                        ns[slen] = 0;
                        char cx = sd.s[xc->pos - 1], cy = sd.s[yc->pos - 1];
                        ns[xc->pos - 1] = cy;
                        ns[yc->pos - 1] = cx;
                        rt_sxt_break_fast(ns);
                        *cx_cell = (DESCR_t){ .v = DT_S, .slen = (uint32_t)slen, .s = ns };
                        return (DESCR_t){ .v = DT_S, .slen = 1, .s = (char *)&k_one_char_str[2 * (unsigned char)cy] };
                    }
                }
            }
        }
    }
    if (!swap_tvsubs_fits(xc) || !swap_tvsubs_fits(yc)) { extern void core_icn_fatal(int code, DESCR_t val); core_icn_fatal(205, FAILDESCR); return FAILDESCR; }
    DESCR_t dx = rt_deref(va), dy = rt_deref(vb);
    { extern void rt_sxt_break(const char *); if (dx.v == DT_S) rt_sxt_break(dx.s); if (dy.v == DT_S) rt_sxt_break(dy.s); }
    if (dx.v == DT_FAIL || dy.v == DT_FAIL) return FAILDESCR;
    long adj1 = 0, adj2 = 0;
    {
        DESCR_t *bx = swap_base_cell(xc->sv);
        if (bx && bx == swap_base_cell(yc->sv) && xc->pos > 0 && yc->pos > 0 && !xc->cellp && !yc->cellp) {
            if (xc->pos > yc->pos) adj1 = xc->len - yc->len;
            else if (yc->pos > xc->pos) adj2 = yc->len - xc->len;
        }
    }
    if (!adj1 && !adj2 && IS_NAMETRAP_fn(xc->sv) && IS_NAMETRAP_fn(yc->sv)) {
        VCELL_t *ux = vcell_ultimate(xc->sv), *uy = vcell_ultimate(yc->sv);
        int same_slot = 0;
        if (ux && uy) { if (ux->cellp && ux->cellp == uy->cellp) same_slot = 1; else if (ux->tbl && ux->tbl == uy->tbl) { if (tbl_key_equal(ux->key_d, uy->key_d)) same_slot = 1; } }
        if (same_slot) { if (xc->pos > yc->pos) adj1 = xc->len - yc->len; else if (yc->pos > xc->pos) adj2 = yc->len - xc->len; }
    }
    if (rt_assign_var(va, dy).v == DT_FAIL) return FAILDESCR;
    if (adj2 != 0) yc->pos += adj2;
    if (rt_assign_var(vb, dx).v == DT_FAIL) return FAILDESCR;
    if (adj1 != 0) xc->pos += adj1;
    return rt_deref(va);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_zcol_push(void ** ptr_cell, int * cap_cell, int i, long elem_sz) {
    extern void rt_bomb(const char *);
    if (i + 1 > *cap_cell) {
        int nc = *cap_cell > 0 ? *cap_cell : 4;
        while (nc < i + 1) nc *= 2;
        char * op = (char *)*ptr_cell;
        char * np = (char *)ct_grow(op, (size_t)nc * (size_t)elem_sz);
        if (!np) rt_bomb("rt_zcol_push: collection realloc failed");
        memset(np + (size_t)*cap_cell * (size_t)elem_sz, 0, (size_t)(nc - *cap_cell) * (size_t)elem_sz);
        *ptr_cell = np;
        *cap_cell = nc;
    }
    char * e = (char *)*ptr_cell + (size_t)i * (size_t)elem_sz;
    memset(e, 0, (size_t)elem_sz);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx) { return subscript_get_s(arr, idx, 0); }
DESCR_t subscript_get_strict(DESCR_t arr, DESCR_t idx) { return subscript_get_s(arr, idx, 1); }
DESCR_t subscript_get2_ext(DESCR_t arr, DESCR_t i, DESCR_t end) { return subscript_get2_ext_s(arr, i, end, 0); }
DESCR_t subscript_get2_ext_strict(DESCR_t arr, DESCR_t i, DESCR_t end) { return subscript_get2_ext_s(arr, i, end, 1); }
DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j) { return subscript_get2_s(arr, i, j, 0); }
DESCR_t subscript_get2_strict(DESCR_t arr, DESCR_t i, DESCR_t j) { return subscript_get2_s(arr, i, j, 1); }
DESCR_t rt_section_var(DESCR_t base, DESCR_t i1d, DESCR_t i2d) { return rt_section_var_s(base, i1d, i2d, 0); }
DESCR_t rt_section_var_strict(DESCR_t base, DESCR_t i1d, DESCR_t i2d) { return rt_section_var_s(base, i1d, i2d, 1); }
DESCR_t rt_subscript_val(DESCR_t base, DESCR_t idx) {
    extern DESCR_t rt_deref(DESCR_t);
    extern DESCR_t rt_subscript_var_container_only(DESCR_t, DESCR_t);
    if (IS_VARREF_fn(base)) base = rt_deref(base);
    if (base.v == DT_A && idx.v == DT_I) {
        ARBLK_t *a = base.arr;
        if (a && a->ndim == 1 && a->data) { long off = (long)idx.i - (long)a->lo; if (off >= 0 && off <= (long)a->hi - (long)a->lo) return a->data[off]; }
    }
    { DESCR_t r = rt_subscript_var_container_only(base, idx); return (r.v == DT_N && r.slen != 0) ? rt_deref(r) : r; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_subscript_val_strict(DESCR_t base, DESCR_t idx) {
    DESCR_t declined = { .v = RTX_NOT_HANDLED };
    if (base.v == DT_N && base.slen == 1 && base.ptr) base = *(DESCR_t *)base.ptr;
    if (IS_VARREF_fn(base)) return declined;
    if (base.v == DT_T && base.tbl) { TBPAIR_t *e = table_find_pair_d(base.tbl, idx); if (e) return e->val; return (base.tbl->dflt.v != DT_FAIL && base.tbl->dflt.v != 0) ? base.tbl->dflt : NULVCL; }
    if (idx.v != DT_I) return declined;
    if (base.v == DT_A && base.arr && base.arr->ndim == 1 && base.arr->data) {
        long off = (long)idx.i - (long)base.arr->lo;
        return (off >= 0 && off <= (long)base.arr->hi - (long)base.arr->lo) ? base.arr->data[off] : declined;
    }
    DESCR_t *elems = 0;
    int n = 0;
    if (base.v == DT_DATA && rt_list_view(base, &elems, &n)) { long i = (long)idx.i; if (i < 0) i = n + i + 1; return (elems && i >= 1 && i <= n) ? elems[i - 1] : declined; }
    return declined;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_subscript_var(DESCR_t base, DESCR_t idx) { return c_rt_subscript_var_s(base, idx, 0); }
DESCR_t rt_subscript_var_strict(DESCR_t base, DESCR_t idx) { return c_rt_subscript_var_s(base, idx, 1); }
DESCR_t c_rt_subscript_var_container_only(DESCR_t base, DESCR_t idx) { return c_rt_subscript_var_container_only_s(base, idx, 0); }
DESCR_t rt_subscript_var_container_only_strict(DESCR_t base, DESCR_t idx) { return c_rt_subscript_var_container_only_s(base, idx, 1); }
DESCR_t rt_random_var(DESCR_t base) { return rt_random_var_s(base, 0); }
DESCR_t rt_random_var_strict(DESCR_t base) { return rt_random_var_s(base, 1); }
DESCR_t c_rt_assign_var(DESCR_t var, DESCR_t val) { return c_rt_assign_var_s(var, val, 0); }
DESCR_t rt_assign_var_strict(DESCR_t var, DESCR_t val) { return c_rt_assign_var_s(var, val, 1); }
