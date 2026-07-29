#include "gen_runtime.h"
#include "gen_value.h"
#include "ast.h"
#include "../../parser/snobol4/scrip_cc.h"
#include "gen.h"
#include "../rt/gc_heap.h"
#include "coerce.h"
#include "../by_name_dispatch.h"
#include "../../lower/lower.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
extern DESCR_t NV_SET_fn(const char *name, DESCR_t val);
tree_t      *g_root     = NULL;
unsigned long bb_rnd_seed = 12345UL;
GenFrame frame_stack[FRAME_STACK_MAX];
int      frame_depth = 0;
tree_t  *drive_node = NULL;
DESCR_t  drive_val;
const char *scan_subj  = "";
int         scan_pos   = 1;
ScanEntry scan_stack[SCAN_STACK_MAX];
int         scan_depth = 0;
ScanEntry scan_saved[SCAN_STACK_MAX];
int         scan_saved_depth = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_scan_active(void)
{
    return (scan_depth > 0) || (scan_subj && scan_subj[0] != 0) || (scan_pos != 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_scan_state_capture(void *prev) {
    ScanState *s = (ScanState *)prev;
    if (!s) { s = (ScanState *)calloc(1, sizeof(ScanState)); if (!s) return NULL; }
    s->subj = scan_subj; s->pos = scan_pos; s->depth = scan_depth; s->saved_depth = scan_saved_depth;
    for (int i = 0; i < scan_depth && i < SCAN_STACK_MAX; i++) s->stack[i] = scan_stack[i];
    for (int i = 0; i < scan_saved_depth && i < SCAN_STACK_MAX; i++) s->saved[i] = scan_saved[i];
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_scan_state_apply(void *saved) {
    ScanState *s = (ScanState *)saved;
    if (!s) return;
    scan_subj = s->subj ? s->subj : ""; scan_pos = s->pos; scan_depth = s->depth; scan_saved_depth = s->saved_depth;
    for (int i = 0; i < scan_depth && i < SCAN_STACK_MAX; i++) scan_stack[i] = s->stack[i];
    for (int i = 0; i < scan_saved_depth && i < SCAN_STACK_MAX; i++) scan_saved[i] = s->saved[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_scan_state_reset(void) {
    scan_subj = ""; scan_pos = 1; scan_depth = 0; scan_saved_depth = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned long rt_scan_state_size(void) { return (unsigned long)sizeof(ScanState); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ScanSubjRegs rt_scan_enter(uint64_t lo, uint64_t hi, uint64_t sigma, uint64_t delta, uint64_t Delta) {
    uint64_t w[2]; w[0] = lo; w[1] = hi; DESCR_t sv; memcpy(&sv, w, sizeof sv);
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str(sv);
    if (scan_depth < SCAN_STACK_MAX) {
        scan_stack[scan_depth].subj  = scan_subj;
        scan_stack[scan_depth].pos   = scan_pos;
        scan_stack[scan_depth].sigma = sigma;
        scan_stack[scan_depth].delta = delta;
        scan_stack[scan_depth].Delta = Delta;
        scan_depth++;
    }
    rt_gc_point(&sv, (const char **)0);
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
    if (!s) s = "";
    scan_subj = s;
    scan_pos  = 1;
    uint64_t L = (sv.v == DT_S && sv.slen && s == sv.s) ? (uint64_t)sv.slen : (uint64_t)strlen(s);
    ScanSubjRegs r; r.ptr = (uint64_t)(uintptr_t)s; r.len = L;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ScanSubjRegs rt_scan_needle(uint64_t lo, uint64_t hi) {
    uint64_t w[2]; w[0] = lo; w[1] = hi; DESCR_t sv; memcpy(&sv, w, sizeof sv);
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str(sv);
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
    if (!s) s = "";
    uint64_t L = (sv.v == DT_S && sv.slen && s == sv.s) ? (uint64_t)sv.slen : (uint64_t)strlen(s);
    ScanSubjRegs r; r.ptr = (uint64_t)(uintptr_t)s; r.len = L;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_scan_leave(uint64_t *out3) {
    if (scan_depth > 0) {
        scan_depth--;
        if (scan_saved_depth < SCAN_STACK_MAX) {
            scan_saved[scan_saved_depth].subj  = scan_subj;
            scan_saved[scan_saved_depth].pos   = scan_pos;
            scan_saved[scan_saved_depth].sigma = scan_stack[scan_depth].sigma;
            scan_saved[scan_saved_depth].delta = scan_stack[scan_depth].delta;
            scan_saved[scan_saved_depth].Delta = scan_stack[scan_depth].Delta;
            scan_saved_depth++;
        }
        scan_subj = scan_stack[scan_depth].subj;
        scan_pos  = scan_stack[scan_depth].pos;
        if (out3) { out3[0] = scan_stack[scan_depth].sigma; out3[1] = scan_stack[scan_depth].delta; out3[2] = scan_stack[scan_depth].Delta; }
    } else if (out3) { out3[0] = 0; out3[1] = 0; out3[2] = 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ScanSubjRegs rt_scan_reenter(void) {
    ScanSubjRegs r; r.ptr = 0; r.len = 0;
    if (scan_saved_depth <= 0) return r;
    scan_saved_depth--;
    if (scan_depth < SCAN_STACK_MAX) {
        scan_stack[scan_depth].subj  = scan_subj;
        scan_stack[scan_depth].pos   = scan_pos;
        scan_stack[scan_depth].sigma = scan_saved[scan_saved_depth].sigma;
        scan_stack[scan_depth].delta = scan_saved[scan_saved_depth].delta;
        scan_stack[scan_depth].Delta = scan_saved[scan_saved_depth].Delta;
        scan_depth++;
    }
    scan_subj = scan_saved[scan_saved_depth].subj;
    scan_pos  = scan_saved[scan_saved_depth].pos;
    if (!scan_subj) scan_subj = "";
    r.ptr = (uint64_t)(uintptr_t)scan_subj;
    r.len = (uint64_t)strlen(scan_subj);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_scan_sync_out(uint64_t delta) { scan_pos = (int)delta + 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint64_t rt_scan_sync_in(void) { return (uint64_t)(int64_t)(scan_pos - 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ScanSubjRegs rt_match_enter(uint64_t lo, uint64_t hi) {
    extern const char *Σ; extern int Σlen;
    extern void rt_cap_match_begin(void);
    extern void rt_dcap_lazy_init(void);
    rt_cap_match_begin();
    rt_dcap_lazy_init();
    extern void rt_patstk_lazy_init(void); rt_patstk_lazy_init(); /* rbp-dcap: reserves the pend island on first match; the head box α then loads its
                          * cursor from g_dcap_top and saves its own MARK — the depth-mark array is gone. */
    uint64_t w[2]; w[0] = lo; w[1] = hi; DESCR_t sv; memcpy(&sv, w, sizeof sv);
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str(sv);
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
    if (!s) s = "";
    uint64_t L = (sv.v == DT_S && sv.slen && s == sv.s) ? (uint64_t)sv.slen : (uint64_t)strlen(s);
    Σ = s; Σlen = (int)L;
    ScanSubjRegs r; r.ptr = (uint64_t)(uintptr_t)s; r.len = L;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t capgen) {
    extern const char *Σ; extern int Σlen; extern uint32_t g_cap_gen;
    Σ = (const char *)(uintptr_t)sig; Σlen = (int)len;
    g_cap_gen = (uint32_t)capgen;   /* PATCTX-2: re-enter the OUTER match's capture generation — see pattern_match.c's well/current split for why the saved id, never a decrement */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_match_replace(const char *name, uint64_t sub_lo, uint64_t sub_hi, int64_t start, int64_t end, DESCR_t *replp) {
    extern char * rt_str_alloc(long n);
    uint64_t w[2]; w[0] = sub_lo; w[1] = sub_hi; DESCR_t sv; memcpy(&sv, w, sizeof sv);
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str(sv);
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv); if (!s) s = "";
    int64_t slen = (sv.v == DT_S && sv.slen && s == sv.s) ? (int64_t)sv.slen : (int64_t)strlen(s);
    DESCR_t rv = replp ? *replp : sv;
    if (IS_INT_fn(rv) || IS_REAL_fn(rv)) rv = descr_to_str(rv);
    const char *rs = (!replp || IS_NULL_fn(rv)) ? "" : VARVAL_fn(rv); if (!rs) rs = "";
    int64_t rlen = (int64_t)strlen(rs);
    if (start < 0) start = 0; if (start > slen) start = slen; if (end < start) end = start; if (end > slen) end = slen;
    { static int _rpt = -1; if (_rpt < 0) { const char *_e = getenv("SCRIP_REPL_TRACE"); _rpt = (_e && _e[0]) ? 1 : 0; } if (_rpt) fprintf(stderr, "[REPL] name=%s slen=%lld start=%lld end=%lld rs=\"%s\" rlen=%lld\n", name?name:"(null)", (long long)slen, (long long)start, (long long)end, rs, (long long)rlen); }   /* BP-2c: cached getenv — ran on EVERY replacement (gdb-sampled ~3% of string_pattern), the BP-2b environ-scan class */
    int64_t nlen = start + rlen + (slen - end);
    char *buf = rt_str_alloc((long)nlen);
    if (buf) { memcpy(buf, s, (size_t)start); memcpy(buf + start, rs, (size_t)rlen); memcpy(buf + start + rlen, s + end, (size_t)(slen - end)); buf[nlen] = '\0'; }
    DESCR_t d = { .v = DT_S, .slen = (uint32_t)nlen, .s = buf ? buf : "" };
    if (name && name[0]) NV_SET_fn(name, d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_match_capture(uint64_t sigma, int64_t start, int64_t end, const char *var) {
    DESCR_t sub = rt_substr((const char *)(uintptr_t)sigma, start, end);
    if (var && var[0]) NV_SET_fn(var, sub);
    return sub;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_substr(const char *sigma, int64_t a, int64_t b) {
    if (!sigma) sigma = "";
    int64_t lo = a < b ? a : b;
    int64_t hi = a < b ? b : a;
    int64_t len = hi - lo;
    char *buf = rt_str_alloc((long)len);
    if (len > 0) memcpy(buf, sigma + lo, (size_t)len);
    buf[len] = '\0';
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_subject(void) { return scan_subj ? STRVAL(scan_subj) : NULVCL; }
DESCR_t rt_keyword_pos(void) { return INTVAL((int64_t)scan_pos); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long cvpos_of(DESCR_t v, long len, int *ok) {
    long i; *ok = 1;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) { *ok = 0; return 0; } i = t; }
    else { *ok = 0; return 0; }
    if (i < -len || i > len + 1) { *ok = 0; return 0; }
    return (i > 0) ? i : (len + i + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int64_t rt_cvpos_pos(DESCR_t v, int64_t len) { int ok; long p = cvpos_of(v, (long)len, &ok); return ok ? (int64_t)p : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_pos_set(DESCR_t v) {
    long len = scan_subj ? (long)strlen(scan_subj) : 0; int ok; long p = cvpos_of(v, len, &ok);
    if (!ok) return FAILDESCR;
    scan_pos = (int)p; return INTVAL((int64_t)p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rsw_get(long kind, DESCR_t *vp, int64_t *spill) {
    if (kind == 0) return vp ? *vp : NULVCL;
    if (kind == 1) return INTVAL(spill ? spill[0] + 1 : (int64_t)scan_pos);
    fprintf(stderr, "[REVSWAP] FATAL: <-> read of unimplemented keyword kind %ld (only plain vars and &pos are wired; add the kind to rsw_get/rsw_set)\n", kind); abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rsw_set(long kind, DESCR_t *vp, int64_t *spill, DESCR_t v) {
    if (kind == 0) { if (vp) *vp = v; return 1; }
    if (kind == 1) { long len = spill ? (long)spill[1] : (scan_subj ? (long)strlen(scan_subj) : 0); int ok; long p = cvpos_of(v, len, &ok); if (!ok) return 0; if (spill) spill[0] = (int64_t)(p - 1); else scan_pos = (int)p; return 1; }
    fprintf(stderr, "[REVSWAP] FATAL: <-> write of unimplemented keyword kind %ld (only plain vars and &pos are wired; add the kind to rsw_get/rsw_set)\n", kind); abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_rev_swap_fwd(long lkind, DESCR_t *lp, long rkind, DESCR_t *rp, DESCR_t *save, int64_t *spill) {
    DESCR_t old_l = rsw_get(lkind, lp, spill); DESCR_t old_r = rsw_get(rkind, rp, spill);
    save[0] = old_l; save[1] = old_r;
    if (!rsw_set(lkind, lp, spill, old_r)) return FAILDESCR;
    if (!rsw_set(rkind, rp, spill, old_l)) return FAILDESCR;
    return old_r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_rev_swap_undo(long lkind, DESCR_t *lp, long rkind, DESCR_t *rp, DESCR_t *save, int64_t *spill) {
    if (!rsw_set(lkind, lp, spill, save[0])) return FAILDESCR;
    rsw_set(rkind, rp, spill, save[1]);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
ScanSubjRegs rt_keyword_subject_set(uint64_t lo, uint64_t hi) {
    uint64_t w[2]; w[0] = lo; w[1] = hi; DESCR_t sv; memcpy(&sv, w, sizeof sv);
    if (IS_INT_fn(sv) || IS_REAL_fn(sv)) sv = descr_to_str(sv);
    if (!(IS_STR_fn(sv) || IS_NULL_fn(sv))) { ScanSubjRegs r; r.ptr = 0; r.len = 0; return r; }
    const char *s = IS_NULL_fn(sv) ? "" : VARVAL_fn(sv);
    if (!s) s = "";
    scan_subj = s; scan_pos = 1;
    ScanSubjRegs r; r.ptr = (uint64_t)(uintptr_t)s; r.len = (uint64_t)strlen(s);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_random_set(DESCR_t v) {
    extern long g_random;
    long i;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) return FAILDESCR; i = t; }
    else return FAILDESCR;
    g_random = i; return INTVAL((int64_t)i);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_error_set(DESCR_t v) { extern long g_error;
    long i;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) return FAILDESCR; i = t; }
    else return FAILDESCR;
    g_error = i; return INTVAL((int64_t)i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_trace_set(DESCR_t v) { extern long g_trace;
    long i;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) return FAILDESCR; i = t; }
    else return FAILDESCR;
    g_trace = i; return INTVAL((int64_t)i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_keyword_dump_set(DESCR_t v)  { extern long g_dump;
    long i;
    if (v.v == DT_I) i = (long)v.i;
    else if (v.v == DT_R) i = (long)v.r;
    else if (v.v == DT_S && v.s) { char *end; long t = strtol(v.s, &end, 10); if (end == v.s) return FAILDESCR; i = t; }
    else return FAILDESCR;
    g_dump  = i; return INTVAL((int64_t)i); }
#include "../../driver/driver_private.h"
#include <time.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void gen_gc_roots(void)
{
    extern void rt_gc_visit_descr(DESCR_t *d); extern void rt_gc_visit_raw(const char **loc); extern void rt_gc_pin_ptr(const char *p);
    rt_gc_visit_descr(&drive_val);
    for (int f = 0; f < frame_depth; f++) { GenFrame *fr = &frame_stack[f]; for (int i = 0; i < fr->env_n; i++) rt_gc_visit_descr(&fr->env[i]); rt_gc_visit_descr(&fr->return_val); for (int g = 0; g < fr->gen_depth; g++) rt_gc_visit_raw(&fr->gen[g].sval); }
    rt_gc_pin_ptr(scan_subj);
    rt_gc_visit_raw(&scan_subj);
    for (int i = 0; i < scan_depth; i++) { rt_gc_visit_raw(&scan_stack[i].subj); rt_gc_visit_raw((const char **)&scan_stack[i].sigma); }
}
