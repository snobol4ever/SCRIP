#include "runtime/lib/bn_head.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "runtime/rx.h"
#include "runtime/builtin_leaf.h"
#include "runtime/lib/bn_02_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_09_rk.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_13_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_36_sh.h"
extern gv_t g_call_args;
void rt_call_args_need(int n);
void rt_call_args_clear_from(int n);
extern void *rt_ws_alloc_descr(size_t);
extern DESCR_t rt_proc_resume_frame_h(void **);
const char *rk_rx_pat(DESCR_t v) { return (IS_STR_fn(v) && v.s && v.s[0] == RK_RX) ? v.s + 1 : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char rk_proto_closure[] = "Closure";
static int rk_is_closure(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_closure; }
int rk_is_code(DESCR_t v) { return (v.v == DT_BLK && v.s && *v.s) || rk_is_closure(v); }
const char *rk_code_name(DESCR_t v) {
    return v.v == DT_BLK ? v.s : (rk_is_closure(v) && ((ARBLK_t *) v.arr)->hi >= ((ARBLK_t *) v.arr)->lo) ? ((ARBLK_t *) v.arr)->data[0].s : (const char *) 0;
}
rk_cbh_t *g_rk_cbh_cur = (rk_cbh_t *)0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_blk_nref(DESCR_t blk) { return rk_is_closure(blk) ? ((ARBLK_t *) blk.arr)->hi - ((ARBLK_t *) blk.arr)->lo : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rk_cb_t rk_blk_snap(DESCR_t blk, DESCR_t *refs) { rk_cb_t c = { rk_code_name(blk), rk_blk_nref(blk) }; for (int i = 0; i < c.nref; i++) refs[i] = ((ARBLK_t *) blk.arr)->data[1 + i]; return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_proc_descr_rq(const char *name, int nargs, long *rq) {
    extern DESCR_t rt_c2bb_bomb(const char *site, const char *name);
    extern int rt_call_open_tail_lex(const char *name, int nargs, long *rq);
    (void) nargs;
    if (rq && rt_call_open_tail_lex(name, nargs, rq)) return FAILDESCR;
    return rt_c2bb_bomb("rk_proc_descr_rq (a Raku C site reached user code with no request channel: the box that called it opens the address, or the lowerer emits the loop)", name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_call_snap_rq(rk_cb_t c, const DESCR_t *refs, int na, long *rq) {
    if (c.nref) { rt_call_args_need(na + c.nref); for (int i = na - 1; i >= 0; i--) CALL_ARGS[i + c.nref] = CALL_ARGS[i]; for (int i = 0; i < c.nref; i++) CALL_ARGS[i] = refs[i]; }
    return rk_proc_descr_rq(c.name, na + c.nref, rq);
}
DESCR_t rk_call_snap(rk_cb_t c, const DESCR_t *refs, int na) { return rk_call_snap_rq(c, refs, na, (long *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_call_block_rq(DESCR_t blk, int na, long *rq) { int nr = rk_blk_nref(blk); DESCR_t refs[nr ? nr : 1]; rk_cb_t c = rk_blk_snap(blk, refs); return rk_call_snap_rq(c, refs, na, rq); }
DESCR_t rk_call_block(DESCR_t blk, int na) { return rk_call_block_rq(blk, na, (long *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_block_cmp_d(rk_cb_t cb, const DESCR_t *refs, DESCR_t x, DESCR_t y) {
    rt_call_args_need(2);
    CALL_ARGS[0] = x;
    CALL_ARGS[1] = y;
    double d = to_real(rk_call_snap(cb, refs, 2));
    return d < 0 ? -1 : (d > 0 ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_av_elem(rk_av_t a, int i) { return (i < 0 || i >= a.n) ? NULVCL : a.el[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_first_shape(DESCR_t src, int i, int mode) {
    DESCR_t ed = rk_av_elem(rk_av(src), i);
    if (mode == 1) return INTVAL(i);
    if (mode == 2) { DESCR_t kv[2] = { INTVAL(i), ed }; return rk_mk_arr(kv, 2); }
    if (mode == 3) return rk_mk_pair(INTVAL(i), ed);
    return ed;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_arr_append(ARBLK_t *b, DESCR_t v) {
    int n = b->hi - b->lo + 1;
    size_t cap = (HBLK_BYTES((rt_hblk_t *) b->data - 1) - sizeof(rt_hblk_t)) / sizeof(DESCR_t);
    if ((size_t) n + 1 > cap) { size_t nc = 2 * cap > (size_t) n + 1 ? 2 * cap : (size_t) n + 1; b->data = (DESCR_t *) rt_gcheap_grow_block(b->data, HB_DVEC, nc * sizeof(DESCR_t)); }
    b->data[n] = v;
    b->hi++;
}
int rk_seq_pull(DESCR_t *sp) {
    extern DESCR_t rt_proc_call_gen_h(const char *, int, void **, const uint64_t *);
    extern DESCR_t rt_proc_resume_frame_h(void **);
    ARBLK_t *s = (ARBLK_t *) sp->arr;
    if (s->data[2].i == 2) return 0;
    void *h = s->data[1].ptr;
    DESCR_t v;
    if (s->data[2].i == 0) {
        DESCR_t cl = s->data[0];
        const char *nm = rk_code_name(cl);
        int nr = rk_blk_nref(cl);
        s->data[2] = INTVAL(1);
        rt_call_args_need(nr);
        for (int k = 0; k < nr; k++) CALL_ARGS[k] = ((ARBLK_t *) cl.arr)->data[1 + k];
        rt_call_args_clear_from(nr);
#if RT_DIAG
        rt_c2bb_hit("gen_h.rk_seq", nm);
#endif
        v = rt_proc_call_gen_h(nm, nr, &h, (const uint64_t *) 0);
    } else {
#if RT_DIAG
        rt_c2bb_hit("gen_h.rk_seq_resume", "?");
#endif
        v = rt_proc_resume_frame_h(&h);
    }
    s = (ARBLK_t *) sp->arr;
    s->data[1] = (DESCR_t){ .v = DT_RAW, .ptr = h };
    if (IS_FAIL_fn(v)) { s->data[2] = INTVAL(2); return 0; }
    rk_arr_append((ARBLK_t *) s->data[3].arr, v);
    if (!h) { s = (ARBLK_t *) sp->arr; s->data[2] = INTVAL(2); }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_seq_done(DESCR_t list) {
    DESCR_t sq = rk_mk_arr(NULL, 0);
    rk_arr_append((ARBLK_t *) sq.arr, NULVCL);
    rk_arr_append((ARBLK_t *) sq.arr, (DESCR_t){ .v = DT_RAW, .ptr = (void *) 0 });
    rk_arr_append((ARBLK_t *) sq.arr, INTVAL(2));
    rk_arr_append((ARBLK_t *) sq.arr, rk_mark_list(list));
    ((ARBLK_t *) sq.arr)->proto = rk_proto_seq;
    return sq;
}
static int rk_rx_cp_to_byte(const char *s, long cp) { int i = 0; while (cp > 0 && s[i]) { i++; while (((unsigned char) s[i] & 0xC0) == 0x80) i++; cp--; } return i; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_rx_step(const char *s, int slen, int pos) { if (pos >= slen) return pos + 1; int l = 1; while (pos + l < slen && ((unsigned char) s[pos + l] & 0xC0) == 0x80) l++; return pos + l; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_rx_collect(const RxProg *prog, const char *subj, const RkRxOpt *o, DESCR_t *out) {
    int slen = (int) strlen(subj), start = 0, anchored = 0;
    if (o->mode & 8) start = rk_rx_cp_to_byte(subj, o->posv);
    if (o->mode & 16) { start = rk_rx_cp_to_byte(subj, o->posv); anchored = 1; }
    int stream = (o->mode & (1 | 2 | 4 | 32 | 64)) != 0;
    long maxnth = 0;
    for (int k = 0; k < o->nnth; k++) if (o->nth[k] > maxnth) maxnth = o->nth[k];
    DESCR_t gorig = STRVAL(rt_heap_strdup_c(subj));
    DESCR_t *mr = (DESCR_t *) rt_ws_alloc_descr((size_t) slen + 2);
    int count = 0, pos = start;
    long seen = 0;
    g_subject = subj;
    while (pos <= slen) {
        RxMatch rm;
        if (!rx_exec(prog, subj, slen, pos, anchored, &rm)) break;
        seen++;
        int take = 1;
        if (o->nnth) { take = 0; for (int k = 0; k < o->nnth; k++) if (o->nth[k] == seen) take = 1; }
        if (take) { rk_rx_to_match(&g_match, prog, &rm); mr[count++] = rk_match_obj_in(&g_match, subj, gorig); }
        if (!stream) break;
        if ((o->mode & 32) && o->xmax >= 0 && count >= o->xmax) break;
        if (maxnth && seen >= maxnth && !(o->mode & 1)) break;
        pos = (o->mode & 2) ? rk_rx_step(subj, slen, rm.from) : (rm.to > rm.from ? rm.to : rk_rx_step(subj, slen, rm.to));
        if (anchored) break;
    }
    if (count == 0 || ((o->mode & 32) && count < o->xmin)) { memset(&g_match, 0, sizeof g_match); *out = FAILDESCR; return 0; }
    if (!stream) { *out = mr[0]; return 1; }
    if (o->nnth == 1 && !(o->mode & (1 | 2 | 4 | 32))) { *out = mr[0]; return 1; }
    *out = rk_mk_arr(mr, count);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_rx_quoted(const char *s) {
    size_t n = strlen(s);
    char *o = (char *) rt_wsb_alloc(n * 2 + 3);
    size_t k = 0;
    o[k++] = '\'';
    for (size_t i = 0; i < n; i++) { if (s[i] == '\\' || s[i] == '\'') o[k++] = '\\'; o[k++] = s[i]; }
    o[k++] = '\'';
    o[k] = 0;
    return STRVAL(o);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_rx_piece(DESCR_t v, int as_source, char *scratch, size_t scap) {
    const char *rx = rk_rx_pat(v);
    if (rx) return rx;
    const char *t = to_cstring(v, scratch, scap);
    (void) as_source;
    return t ? t : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_rx_concat3(const char *a, const char *b, const char *c) {
    size_t la = strlen(a), lb = strlen(b), lc = strlen(c);
    char *o = (char *) rt_wsb_alloc(la + lb + lc + 1);
    memcpy(o, a, la);
    memcpy(o + la, b, lb);
    memcpy(o + la + lb, c, lc);
    o[la + lb + lc] = 0;
    return STRVAL(o);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_rx_interp_item(DESCR_t v, int as_source) {
    char sb[64];
    const char *rx = rk_rx_pat(v);
    if (rx) return rk_rx_concat3("[", rx, "]");
    const char *t = rk_rx_piece(v, as_source, sb, sizeof sb);
    if (as_source) return rk_rx_concat3("[", t, "]");
    return rk_rx_quoted(t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_rx_interp_list(DESCR_t arr, int as_source) {
    rk_av_t a = rk_av(arr);
    if (a.n == 0) return STRVAL(rt_heap_strdup_c("<!>"));
    size_t cap = 8;
    for (int i = 0; i < a.n; i++) { char sb[64]; const char *t = rk_rx_piece(a.el[i], as_source, sb, sizeof sb); cap += strlen(t) * 2 + 8; }
    char *o = (char *) rt_wsb_alloc(cap);
    size_t k = 0;
    o[k++] = '[';
    for (int i = 0; i < a.n; i++) {
        DESCR_t it = rk_rx_interp_item(a.el[i], as_source);
        const char *t = it.s ? it.s : "";
        if (i) { o[k++] = ' '; o[k++] = '|'; o[k++] = ' '; }
        size_t l = strlen(t);
        memcpy(o + k, t, l);
        k += l;
    }
    o[k++] = ']';
    o[k] = 0;
    return STRVAL(o);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_rx_piece_str(const char *s, int from, int to) {
    int n = to > from ? to - from : 0;
    char *t = (char *) rt_wsb_alloc((size_t) n + 1);
    if (n) memcpy(t, s + from, (size_t) n);
    t[n] = 0;
    return STRVAL(t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_subst_open(DESCR_t *args, int nargs) {
    DESCR_t sd = args[0], pd = args[1];
    char sbuf[64];
    if (!IS_STR_fn(sd)) { const char *t = to_cstring(sd, sbuf, sizeof sbuf); sd = STRVAL(rt_heap_strdup_c(t ? t : "")); }
    if (rk_rx_pat(pd)) pd = STRVAL((char *) rk_rx_pat(pd));
    long mode = args[2].i;
    int nn = nargs > 6 ? nargs - 6 : 0, pos = 0, anchored = 0, last_end = 0;
    DESCR_t nth[nn ? nn : 1];
    for (int k = 0; k < nn; k++) nth[k] = INTVAL(args[6 + k].i);
    const char *s0 = rk_cstr(sd);
    if (mode & 8) pos = rk_rx_cp_to_byte(s0, args[3].i);
    if (mode & 16) { pos = rk_rx_cp_to_byte(s0, args[3].i); anchored = 1; }
    if (pos > 0) last_end = pos <= (int) strlen(s0) ? pos : 0;
    DESCR_t st[16] = { sd, pd, rk_mk_arr(NULL, 0), INTVAL(pos), INTVAL(last_end), INTVAL(0), INTVAL(0), INTVAL(anchored), INTVAL(mode), INTVAL(args[3].i), INTVAL(args[4].i), INTVAL(args[5].i),
        rk_mk_arr(nth, nn), INTVAL(0), INTVAL(0), INTVAL(0) };
    return rk_mk_arr(st, 16);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_subst_after(DESCR_t *e, int from, int to) {
    long mode = e[8].i, seen = e[6].i, maxnth = 0;
    rk_av_t nv = rk_av(e[12]);
    for (int k = 0; k < nv.n; k++) if (nv.el[k].i > maxnth) maxnth = nv.el[k].i;
    int stream = (mode & (1 | 2 | 4 | 32 | 64)) != 0;
    if (!stream || (maxnth && seen >= maxnth && !(mode & 1)) || ((mode & 32) && e[11].i >= 0 && e[5].i >= e[11].i) || e[7].i) { e[13] = INTVAL(1); return; }
    const char *s1 = rk_cstr(e[0]);
    int sl1 = (int) strlen(s1);
    e[3] = INTVAL((mode & 2) ? rk_rx_step(s1, sl1, from) : (to > from ? to : rk_rx_step(s1, sl1, to)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_subst_next(DESCR_t stv) {
    rk_av_t a = rk_av(stv);
    if (a.n != 16 || a.el[13].i) return 0;
    DESCR_t *e = a.el;
    for (;;) {
        const char *subj = rk_cstr(e[0]), *pat = rk_cstr(e[1]);
        int slen = (int) strlen(subj), pos = (int) e[3].i;
        if (pos > slen) { e[13] = INTVAL(1); return 0; }
        RxEnv renv;
        RxProg *prog = rk_rx_compile(&renv, "", pat, 0);
        RxMatch rm;
        if (!prog || !rx_exec(prog, subj, slen, pos, (int) e[7].i, &rm)) { e[13] = INTVAL(1); return 0; }
        e[6] = INTVAL(e[6].i + 1);
        rk_av_t nv = rk_av(e[12]);
        int take = 1;
        if (nv.n) { take = 0; for (int k = 0; k < nv.n; k++) if (nv.el[k].i == e[6].i) take = 1; }
        if (take) {
            rk_rx_to_match(&g_match, prog, &rm);
            g_subject = subj;
            rk_arr_append((ARBLK_t *) e[2].arr, rk_rx_piece_str(subj, (int) e[4].i, rm.from));
            e[14] = INTVAL(rm.from);
            e[15] = INTVAL(rm.to);
            return 1;
        }
        rk_subst_after(e, rm.from, rm.to);
        if (e[13].i) return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_subst_put(DESCR_t stv, DESCR_t val) {
    rk_av_t a = rk_av(stv);
    if (a.n != 16) return;
    DESCR_t *e = a.el;
    char rb[64];
    const char *rt = to_cstring(val, rb, sizeof rb);
    rk_arr_append((ARBLK_t *) e[2].arr, STRVAL(rt_heap_strdup_c(rt ? rt : "")));
    e[4] = INTVAL(e[15].i);
    e[5] = INTVAL(e[5].i + 1);
    rk_subst_after(e, (int) e[14].i, (int) e[15].i);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_subst_close(DESCR_t stv) {
    rk_av_t a = rk_av(stv);
    if (a.n != 16) return FAILDESCR;
    DESCR_t *e = a.el;
    long mode = e[8].i, count = e[5].i;
    if (count == 0 || ((mode & 32) && count < e[10].i)) return (mode & 128) ? e[0] : FAILDESCR;
    const char *subj = rk_cstr(e[0]);
    rk_arr_append((ARBLK_t *) e[2].arr, rk_rx_piece_str(subj, (int) e[4].i, (int) strlen(subj)));
    rk_av_t pv = rk_av(e[2]);
    size_t total = 0;
    for (int i = 0; i < pv.n; i++) total += strlen(rk_cstr(pv.el[i]));
    char *res = (char *) rt_str_alloc((long) total + 1);
    size_t k = 0;
    for (int i = 0; i < pv.n; i++) { const char *t = rk_cstr(pv.el[i]); size_t l = strlen(t); memcpy(res + k, t, l); k += l; }
    res[k] = 0;
    return STRVAL(res);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_rx_subst_run(DESCR_t subjD, DESCR_t patD, DESCR_t replD, const RkRxOpt *o, DESCR_t *out) {
    DESCR_t items[3] = { subjD, patD, replD };
    rk_cbh_t H = { g_rk_cbh_cur, { rk_mk_arr(items, 3), rk_mk_arr(NULL, 0) } };
    g_rk_cbh_cur = &H;
    int iscode = rk_is_code(replD), nrf = iscode ? rk_blk_nref(replD) : 0;
    DESCR_t rfs[nrf ? nrf : 1];
    rk_cb_t cb = iscode ? rk_blk_snap(replD, rfs) : (rk_cb_t) { NULL, 0 };
    int anchored = 0, pos = 0, last_end = 0, count = 0, nondestructive = (o->mode & 128) != 0;
    int stream = (o->mode & (1 | 2 | 4 | 32 | 64)) != 0;
    long seen = 0, maxnth = 0;
    for (int k = 0; k < o->nnth; k++) if (o->nth[k] > maxnth) maxnth = o->nth[k];
    { const char *s0 = rk_cstr(rk_av_elem(rk_av(H.d[0]), 0)); if (o->mode & 8) pos = rk_rx_cp_to_byte(s0, o->posv); if (o->mode & 16) { pos = rk_rx_cp_to_byte(s0, o->posv); anchored = 1; } }
    if (pos > 0) { const char *s0 = rk_cstr(rk_av_elem(rk_av(H.d[0]), 0)); last_end = pos <= (int) strlen(s0) ? pos : 0; }
    for (;;) {
        rk_av_t hv = rk_av(H.d[0]);
        const char *subj = rk_cstr(hv.el[0]), *pat = rk_cstr(hv.el[1]);
        int slen = (int) strlen(subj);
        if (pos > slen) break;
        RxEnv renv;
        RxProg *prog = rk_rx_compile(&renv, "", pat, 0);
        RxMatch rm;
        if (!prog || !rx_exec(prog, subj, slen, pos, anchored, &rm)) break;
        seen++;
        int take = 1;
        if (o->nnth) { take = 0; for (int k = 0; k < o->nnth; k++) if (o->nth[k] == seen) take = 1; }
        if (take) {
            rk_rx_to_match(&g_match, prog, &rm);
            g_subject = subj;
            rk_arr_append((ARBLK_t *) H.d[1].arr, rk_rx_piece_str(subj, last_end, rm.from));
            DESCR_t rv;
            if (iscode) { rt_call_args_need(1); rv = rk_call_snap(cb, rfs, 0); } else rv = rk_av_elem(rk_av(H.d[0]), 2);
            char rb[64];
            const char *rt = to_cstring(rv, rb, sizeof rb);
            DESCR_t rs = STRVAL(rt_heap_strdup_c(rt ? rt : ""));
            rk_arr_append((ARBLK_t *) H.d[1].arr, rs);
            last_end = rm.to;
            count++;
        }
        if (!stream) break;
        if (maxnth && seen >= maxnth && !(o->mode & 1)) break;
        if ((o->mode & 32) && o->xmax >= 0 && count >= o->xmax) break;
        {
            const char *s1 = rk_cstr(rk_av_elem(rk_av(H.d[0]), 0));
            int sl1 = (int) strlen(s1);
            pos = (o->mode & 2) ? rk_rx_step(s1, sl1, rm.from) : (rm.to > rm.from ? rm.to : rk_rx_step(s1, sl1, rm.to));
        }
        if (anchored) break;
    }
    g_rk_cbh_cur = H.prev;
    if (count == 0 || ((o->mode & 32) && count < o->xmin)) { if (nondestructive) { *out = rk_av_elem(rk_av(H.d[0]), 0); return 1; } *out = FAILDESCR; return 0; }
    const char *subj = rk_cstr(rk_av_elem(rk_av(H.d[0]), 0));
    int slen = (int) strlen(subj);
    rk_arr_append((ARBLK_t *) H.d[1].arr, rk_rx_piece_str(subj, last_end, slen));
    rk_av_t pv = rk_av(H.d[1]);
    size_t total = 0;
    for (int i = 0; i < pv.n; i++) total += strlen(rk_cstr(pv.el[i]));
    char *res = (char *) rt_str_alloc((long) total + 1);
    size_t k = 0;
    for (int i = 0; i < pv.n; i++) { const char *t = rk_cstr(pv.el[i]); size_t l = strlen(t); memcpy(res + k, t, l); k += l; }
    res[k] = 0;
    *out = STRVAL(res);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_trans_has_rx(const DESCR_t *margs, int nmargs) {
    int rx = 0;
    for (int i = 0; i < nmargs; i++) {
        if (!rk_is_pair(margs[i])) continue;
        rk_av_t fp = rk_av(margs[i]);
        if (fp.n < 2) continue;
        if (rk_is_code(fp.el[1])) return 0;
        if (fp.el[1].v == DT_A && fp.el[1].arr && !rk_is_pair(fp.el[1])) { rk_av_t va = rk_av(fp.el[1]); for (int j = 0; j < va.n; j++) if (rk_is_code(va.el[j])) return 0; }
        if (rk_rx_pat(fp.el[0])) rx = 1;
        if (fp.el[0].v == DT_A && fp.el[0].arr && !rk_is_pair(fp.el[0]) && !rk_is_range(fp.el[0])) { rk_av_t ka = rk_av(fp.el[0]); for (int j = 0; j < ka.n; j++) if (rk_rx_pat(ka.el[j])) rx = 1; }
    }
    return rx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_trans_rx(DESCR_t subjD, const DESCR_t *margs, int nmargs, DESCR_t *out) {
    int total = 0;
    for (int i = 0; i < nmargs; i++) {
        if (!rk_is_pair(margs[i])) continue;
        rk_av_t fp = rk_av(margs[i]);
        if (fp.n < 2 || (!rk_rx_pat(fp.el[0]) && IS_STR_fn(fp.el[0]) && !strcmp(rk_cstr(fp.el[0]), "__trans_flags"))) continue;
        total += fp.el[0].v == DT_A && fp.el[0].arr && !rk_is_pair(fp.el[0]) && !rk_is_range(fp.el[0]) ? rk_av(fp.el[0]).n : 1;
    }
    DESCR_t items[2 * total + 1];
    int np = 0;
    items[0] = subjD;
    for (int i = 0; i < nmargs; i++) {
        if (!rk_is_pair(margs[i])) continue;
        rk_av_t fp = rk_av(margs[i]);
        if (fp.n < 2 || (!rk_rx_pat(fp.el[0]) && IS_STR_fn(fp.el[0]) && !strcmp(rk_cstr(fp.el[0]), "__trans_flags"))) continue;
        int ka = fp.el[0].v == DT_A && fp.el[0].arr && !rk_is_pair(fp.el[0]) && !rk_is_range(fp.el[0]);
        int nk = ka ? rk_av(fp.el[0]).n : 1;
        int va = fp.el[1].v == DT_A && fp.el[1].arr && !rk_is_pair(fp.el[1]) && !rk_is_range(fp.el[1]);
        int nv = va ? rk_av(fp.el[1]).n : 1;
        for (int j = 0; j < nk; j++) { items[1 + 2 * np] = ka ? rk_av(fp.el[0]).el[j] : fp.el[0]; items[2 + 2 * np] = va ? (nv ? rk_av(fp.el[1]).el[j < nv ? j : nv - 1] : NULVCL) : fp.el[1]; np++; }
    }
    rk_cbh_t H = { g_rk_cbh_cur, { rk_mk_arr(items, 2 * np + 1), rk_mk_arr(NULL, 0) } };
    g_rk_cbh_cur = &H;
    int pos = 0, last_end = 0, count = 0;
    for (;;) {
        rk_av_t hv = rk_av(H.d[0]);
        const char *subj = rk_cstr(hv.el[0]);
        int slen = (int) strlen(subj);
        if (pos > slen) break;
        int best = -1, bfrom = 0, bto = 0;
        for (int k = 0; k < np; k++) {
            DESCR_t kd = hv.el[1 + 2 * k];
            const char *pat = rk_rx_pat(kd);
            int mf = 0, mt = 0;
            if (pat) {
                RxEnv renv;
                RxProg *prog = rk_rx_compile(&renv, "", pat, 0);
                RxMatch rm;
                if (!prog || !rx_exec(prog, subj, slen, pos, 1, &rm)) continue;
                mf = rm.from;
                mt = rm.to;
            } else {
                const char *ks = rk_cstr(kd);
                int kl = (int) strlen(ks);
                if (kl == 0 || pos + kl > slen || memcmp(subj + pos, ks, (size_t) kl)) continue;
                mf = pos;
                mt = pos + kl;
            }
            if (mt > mf && (best < 0 || mt - mf > bto - bfrom)) { best = k; bfrom = mf; bto = mt; }
        }
        if (best < 0) { pos = rk_rx_step(subj, slen, pos); continue; }
        DESCR_t kd = hv.el[1 + 2 * best], rd = hv.el[2 + 2 * best];
        const char *pat = rk_rx_pat(kd);
        if (pat) {
            RxEnv renv;
            RxProg *prog = rk_rx_compile(&renv, "", pat, 0);
            RxMatch rm;
            if (prog && rx_exec(prog, subj, slen, pos, 1, &rm)) { rk_rx_to_match(&g_match, prog, &rm); g_subject = subj; }
        }
        rk_arr_append((ARBLK_t *) H.d[1].arr, rk_rx_piece_str(subj, last_end, bfrom));
        char rb[64];
        const char *rt = to_cstring(rd, rb, sizeof rb);
        rk_arr_append((ARBLK_t *) H.d[1].arr, STRVAL(rt_heap_strdup_c(rt ? rt : "")));
        last_end = bto;
        count++;
        pos = bto;
    }
    g_rk_cbh_cur = H.prev;
    const char *subj = rk_cstr(rk_av_elem(rk_av(H.d[0]), 0));
    rk_arr_append((ARBLK_t *) H.d[1].arr, rk_rx_piece_str(subj, last_end, (int) strlen(subj)));
    rk_av_t pv = rk_av(H.d[1]);
    size_t tot = 0;
    for (int i = 0; i < pv.n; i++) tot += strlen(rk_cstr(pv.el[i]));
    char *res = (char *) rt_str_alloc((long) tot + 1);
    size_t k = 0;
    for (int i = 0; i < pv.n; i++) { const char *t = rk_cstr(pv.el[i]); size_t l = strlen(t); memcpy(res + k, t, l); k += l; }
    res[k] = 0;
    *out = STRVAL(res);
    return 1;
}
static int rk_code_arity(DESCR_t blk) {
    extern const char *rt_proc_pname(const char *name, int k);
    const char *nm = rk_code_name(blk);
    int n = 0;
    while (nm && rt_proc_pname(nm, n)) n++;
    return n > 0 ? n : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_empty_failure(const char *action) {
    char *msg = rt_heap_fmt("Cannot %s from an empty Array", action);
    rk_exc_init();
    return rk_failure_new(DATCON_fn("X::Cannot::Empty", STRVAL(msg)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt___rk_dyn_notfound(DESCR_t *args, int nargs) {
    if (nargs != 1) return (DESCR_t){ .v = RTX_NOT_HANDLED };
    const char *nm = rk_cstr(args[0]);
    char *msg = rt_heap_fmt("Dynamic variable %s%s not found", nm[0] == '*' ? "$" : "", nm);
    rk_exc_init();
    return rk_failure_new(DATCON_fn("X::Dynamic::NotFound", STRVAL(msg), args[0]));
}
DESCR_t bt___rk_seq_arity(DESCR_t *args, int nargs) { if (nargs == 1) { return INTVAL(rk_is_code(args[0]) ? rk_code_arity(args[0]) : 0); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___blk_close(DESCR_t *args, int nargs) {
    if (nargs >= 1) { DESCR_t cl = rk_mk_arr(args, nargs); ((ARBLK_t *) cl.arr)->proto = rk_proto_closure; return cl; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
int bt___blk_invoke(DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (nargs == 2 && rk_typeobj_name(args[0])) {
        DESCR_t ma[2] = { args[1], STRVAL(rt_heap_strdup_c(rk_typeobj_name(args[0]))) };
        if (script_try_call_builtin_by_name_rq("meth_call", ma, 2, out, rq)) return 1;
    }
    if (nargs >= 1) {
        DESCR_t blk = args[0];
        if (!rk_is_code(blk)) { const char *vn = VARVAL_fn(blk); if (!vn || !*vn) { *out = FAILDESCR; return 1; } blk = (DESCR_t){ 0 }; blk.v = DT_BLK; blk.s = (char *) vn; }
        int na = nargs - 1;
        rt_call_args_need(na);
        for (int k = 0; k < na; k++) CALL_ARGS[k] = args[k + 1];
        *out = rk_call_block_rq(blk, na, rq);
        return 1;
    }
    return 0;
}
DESCR_t bt___rk_arr_sort(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs >= 2 && rk_is_code(args[1])) {
        int nrf = rk_blk_nref(args[1]);
        DESCR_t rfs[nrf ? nrf : 1];
        rk_cb_t cb = rk_blk_snap(args[1], rfs);
        rk_av_t a = rk_av(args[0]);
        int nel = a.n;
        if (nel <= 0) { return rk_mk_arr(NULL, 0); }
        rk_cbh_t H = { g_rk_cbh_cur, { rk_mk_arr(a.el, nel), NULVCL } };
        g_rk_cbh_cur = &H;
        for (int i = 1; i < nel; i++) {
            DESCR_t *w = ((ARBLK_t *) H.d[0].arr)->data;
            H.d[1] = w[i];
            int b = i - 1;
            while (b >= 0) { w = ((ARBLK_t *) H.d[0].arr)->data; if (rk_block_cmp_d(cb, rfs, w[b], H.d[1]) <= 0) break; w = ((ARBLK_t *) H.d[0].arr)->data; w[b + 1] = w[b]; b--; }
            w = ((ARBLK_t *) H.d[0].arr)->data;
            w[b + 1] = H.d[1];
        }
        *out = H.d[0];
        g_rk_cbh_cur = H.prev;
        return _bo;
    }
    if (nargs >= 1) {
        rk_av_t a = rk_av_args(args, 0, nargs);
        const char **tx = a.n ? (const char **) rt_pvec_alloc((size_t) a.n) : NULL;
        DESCR_t *r = a.n ? (DESCR_t *) rt_ws_alloc_descr((size_t) a.n) : NULL;
        for (int i = 0; i < a.n; i++) { tx[i] = rk_av_text(a, i); r[i] = a.el[i]; }
        for (int i = 1; i < a.n; i++) {
            const char *kt = tx[i];
            DESCR_t kd = r[i];
            int j = i - 1;
            while (j >= 0) {
                long long xi, yi;
                int both = rk_text_int(tx[j], &xi) && rk_text_int(kt, &yi);
                int c = (r[j].v == DT_DATA || kd.v == DT_DATA) && rk_order_both_numeric(r[j], kd) ? (int) rk_order_cmp_num(r[j], kd) : both ? (xi > yi ? 1 : xi < yi ? -1 : 0) : strcmp(tx[j], kt);
                if (c <= 0) break;
                tx[j + 1] = tx[j];
                r[j + 1] = r[j];
                j--;
            }
            tx[j + 1] = kt;
            r[j + 1] = kd;
        }
        return rk_mk_arr(r, a.n);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_reduce(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs >= 2 && rk_is_code(args[1])) {
        int nrf = rk_blk_nref(args[1]);
        DESCR_t rfs[nrf ? nrf : 1];
        rk_cb_t cb = rk_blk_snap(args[1], rfs);
        int an = rk_av(args[0]).n;
        if (an == 0) { return NULVCL; }
        rk_cbh_t H = { g_rk_cbh_cur, { args[0], rk_av_elem(rk_av(args[0]), 0) } };
        g_rk_cbh_cur = &H;
        for (int i = 1; i < an; i++) { DESCR_t ed = rk_av_elem(rk_av(H.d[0]), i); rt_call_args_need(2); CALL_ARGS[0] = H.d[1]; CALL_ARGS[1] = ed; H.d[1] = rk_call_snap(cb, rfs, 2); }
        *out = H.d[1];
        g_rk_cbh_cur = H.prev;
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_adjacent_pairs(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        rk_av_t a = rk_av(args[0]);
        int np = a.n > 1 ? a.n - 1 : 0;
        DESCR_t *r = np ? (DESCR_t *) rt_ws_alloc_descr((size_t) np) : NULL;
        for (int i = 0; i < np; i++) { DESCR_t pr[2] = { rk_av_elem(a, i), rk_av_elem(a, i + 1) }; r[i] = rk_mk_arr(pr, 2); }
        return rk_mark_list(rk_mk_arr(r, np));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_is_code(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_is_code(args[0]) ? INTVAL(1) : FAILDESCR; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_first_shape(DESCR_t *args, int nargs) {
    if (nargs == 3) { return rk_first_shape(args[0], IS_INT_fn(args[1]) ? (int)args[1].i : -1, IS_INT_fn(args[2]) ? (int)args[2].i : 0); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_first(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs >= 1) {
        int fend = 0, fmode = 0, fmi = -1;
        for (int i = 1; i < nargs; i++) {
            int adv = 0;
            if (rk_is_pair(args[i])) {
                rk_av_t pp = rk_av(args[i]);
                char kb[64];
                const char *k = pp.n > 1 && pp.el[1].v == DT_BOOL ? to_cstring(pp.el[0], kb, sizeof kb) : NULL;
                if (k && (!strcmp(k, "end") || !strcmp(k, "k") || !strcmp(k, "kv") || !strcmp(k, "p") || !strcmp(k, "v"))) {
                    adv = 1;
                    if (pp.el[1].i) { if (!strcmp(k, "end")) fend = 1; else fmode = !strcmp(k, "k") ? 1 : !strcmp(k, "kv") ? 2 : !strcmp(k, "p") ? 3 : 0; }
                }
            }
            if (!adv && fmi < 0) fmi = i;
        }
        rk_cbh_t H = { g_rk_cbh_cur, { args[0], fmi >= 0 ? args[fmi] : NULVCL } };
        g_rk_cbh_cur = &H;
        int code = fmi >= 0 && rk_is_code(args[fmi]);
        int nrf = code ? rk_blk_nref(args[fmi]) : 0;
        DESCR_t rfs[nrf ? nrf : 1];
        rk_cb_t cb = code ? rk_blk_snap(args[fmi], rfs) : (rk_cb_t) { NULL, 0 };
        int an = rk_av(args[0]).n;
        extern int rt_is_truthy(DESCR_t v);
        *out = NULVCL;
        for (int j = 0; j < an; j++) {
            int i = fend ? an - 1 - j : j;
            DESCR_t ed = rk_av_elem(rk_av(H.d[0]), i);
            int hit = 1;
            if (code) {
                rt_call_args_need(2);
                CALL_ARGS[0] = ed;
                hit = rt_is_truthy(rk_call_snap(cb, rfs, 1));
            } else if (fmi >= 0) {
                DESCR_t sm[2] = { ed, H.d[1] }, t;
                hit = script_try_call_builtin_by_name("__rk_smartmatch", sm, 2, &t) && !IS_FAIL_fn(t) && rk_is_truthy(t);
            }
            if (!hit) continue;
            *out = rk_first_shape(H.d[0], i, fmode);
            break;
        }
        g_rk_cbh_cur = H.prev;
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_produce_step(DESCR_t *args, int nargs) {
    if (nargs == 2) { rk_av_t s = rk_av(args[0]); if (s.n == 2) { rk_arr_append((ARBLK_t *) s.el[0].arr, args[1]); ((ARBLK_t *) args[0].arr)->data[1] = args[1]; } return args[0]; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_produce_done(DESCR_t *args, int nargs) { if (nargs == 1) { rk_av_t s = rk_av(args[0]); return s.n == 2 ? s.el[0] : rk_mk_arr(NULL, 0); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_keyed_done(DESCR_t *args, int nargs) { if (nargs == 1) { rk_av_t s = rk_av(args[0]); return s.n == 4 ? s.el[0] : rk_mk_arr(NULL, 0); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_subst_open(DESCR_t *args, int nargs) { if (nargs >= 6) { return rk_subst_open(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_subst_next(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_subst_next(args[0]) ? (DESCR_t){ .v = DT_BOOL, .i = 1 } : FAILDESCR; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_subst_put(DESCR_t *args, int nargs) { if (nargs == 2) { rk_subst_put(args[0], args[1]); return args[0]; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_subst_close(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_subst_close(args[0]); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_re_subst(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs >= 7) {
        long nth[nargs - 7 > 0 ? nargs - 7 : 1];
        for (int i = 7; i < nargs; i++) nth[i - 7] = args[i].i;
        RkRxOpt o = { args[3].i, args[4].i, args[5].i, args[6].i, nth, nargs - 7 };
        DESCR_t pd = args[1];
        if (rk_rx_pat(pd)) pd = STRVAL((char *) rk_rx_pat(pd));
        DESCR_t sd = args[0];
        char sbuf[64];
        if (!IS_STR_fn(sd)) { const char *t = to_cstring(sd, sbuf, sizeof sbuf); sd = STRVAL(rt_heap_strdup_c(t ? t : "")); }
        rk_rx_subst_run(sd, pd, args[2], &o, out);
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_rxq(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_rx_interp_item(args[0], 0); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_rxsrc(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_rx_interp_item(args[0], 1); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_rxalt(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_rx_interp_list(args[0], 0); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_rxsrcalt(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_rx_interp_list(args[0], 1); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_arr_last(DESCR_t *args, int nargs) {
    if (nargs == 1) { rk_av_t a = rk_av(args[0]); return a.n ? rk_av_elem(a, a.n - 1) : rk_empty_failure("pop"); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_shift_val(DESCR_t *args, int nargs) {
    if (nargs == 1) { rk_av_t a = rk_av(args[0]); return a.n ? rk_av_elem(a, 0) : rk_empty_failure("shift"); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta___rk_arr_map(DESCR_t *args, int nargs, int _bop) {
    static const char *const _bn[] = { "__rk_arr_map", "__rk_arr_grep" };
    const char *const fn = _bn[_bop];
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs >= 2) {
        int want_map = (fn[9] == 'm');
        if (!rk_is_code(args[1]) && want_map) { return FAILDESCR; }
        if (!rk_is_code(args[1])) {
            extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
            rk_cbh_t H = { g_rk_cbh_cur, { args[0], rk_mk_arr(NULL, 0) } }, M = { &H, { args[1], NULVCL } };
            g_rk_cbh_cur = &M;
            for (int si = 0, an = rk_av(H.d[0]).n; si < an; si++) {
                DESCR_t sm[2] = { rk_av_elem(rk_av(H.d[0]), si), M.d[0] }, t;
                if (script_try_call_builtin_by_name("__rk_smartmatch", sm, 2, &t) && !IS_FAIL_fn(t) && rk_is_truthy(t)) rk_arr_append((ARBLK_t *) H.d[1].arr, sm[0]);
            }
            *out = H.d[1];
            g_rk_cbh_cur = H.prev;
            return _bo;
        }
        int nrf = rk_blk_nref(args[1]);
        DESCR_t rfs[nrf ? nrf : 1];
        rk_cb_t cb = rk_blk_snap(args[1], rfs);
        rk_cbh_t H = { g_rk_cbh_cur, { args[0], rk_mk_arr(NULL, 0) } };
        g_rk_cbh_cur = &H;
        int an = rk_av(args[0]).n;
        extern int rt_is_truthy(DESCR_t v);
        for (int si = 0; si < an; si++) {
            DESCR_t ed = rk_av_elem(rk_av(H.d[0]), si);
            rt_call_args_need(2);
            CALL_ARGS[0] = ed;
            DESCR_t rd = rk_call_snap(cb, rfs, 1);
            if (want_map) {
                DESCR_t cur = H.d[1];
                rk_arr_append((ARBLK_t *) cur.arr, rd);
            } else if (rt_is_truthy(rd)) {
                DESCR_t cur = H.d[1];
                rk_arr_append((ARBLK_t *) cur.arr, rk_av_elem(rk_av(H.d[0]), si));
            }
        }
        *out = H.d[1];
        g_rk_cbh_cur = H.prev;
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_map(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_arr_map(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_arr_grep(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_arr_map(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
