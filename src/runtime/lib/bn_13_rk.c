#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_gram_ent.h"
#include "runtime/lib/bn_head_gram_n.h"
#include "runtime/lib/bn_head_rk_ev.h"
#include "runtime/lib/bn_head_rkgrp.h"
#include "runtime/lib/bn_head_rkrxopt.h"
#include "runtime/by_name_dispatch.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "driver/driver_globals.h"
#include "runtime/rx.h"
#include <ctype.h>
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_13_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/rk_gencat_table.inc"
#include "runtime/core/core_rtx_str_shim_api.h"
#include "runtime/core/core.h"
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/descr_tags.h"
#include "ir/descr_tags_dt_platom.h"
extern void *rt_ws_alloc_descr(size_t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_rx_sym_of(const char *name, int nlen, int *len) {
    const char *q = NULL;
    for (int i = 0; i + 5 < nlen; i++) if (!strncmp(name + i, ":sym", 4) && (name[i + 4] == '<' || (unsigned char) name[i + 4] == 0xC2)) { q = name + i + 4; break; }
    if (!q) return NULL;
    const char *st = q[0] == '<' ? q + 1 : q + 2;
    const char *e = q[0] == '<' ? name + nlen - 1 : name + nlen - 2;
    if (e <= st) return NULL;
    *len = (int) (e - st);
    return st;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_rx_rule(void *ud, const char *name, int nlen, int *flags) {
    const char *gname = ud ? (const char *) ud : "";
    char qn[strlen(gname) + (size_t) nlen + 3];
    snprintf(qn, sizeof qn, "%s::%.*s", gname, nlen, name);
    const char *body = gram_get(qn);
    if (!body && gname[0]) { snprintf(qn, sizeof qn, "::%.*s", nlen, name); body = gram_get(qn); }
    if (!body) return NULL;
    int fl = gram_get_flavor(qn);
    *flags = fl == 0 ? RXF_R : fl == 1 ? (RXF_R | RXF_S) : 0;
    const char *tb = body;
    while (*tb == ' ' || *tb == '\t' || *tb == '\n') tb++;
    if (tb[0] == '*') {
        size_t cap = 16;
        for (int i = 0; i < gram_n; i++) cap += strlen(GRAM(i).qname) + 8;
        char *o = (char *) rt_wsb_alloc(cap);
        size_t k = 0;
        char pre[strlen(gname) + (size_t) nlen + 4];
        snprintf(pre, sizeof pre, "%s::%.*s:", gname, nlen, name);
        o[k++] = '[';
        int any = 0;
        for (int i = 0; i < gram_n; i++) {
            const char *qi = GRAM(i).qname;
            if (strncmp(qi, pre, strlen(pre))) continue;
            const char *cand = qi + strlen(gname) + 2;
            if (any) { o[k++] = ' '; o[k++] = '|'; o[k++] = ' '; }
            o[k++] = '<';
            o[k++] = '.';
            size_t l = strlen(cand);
            memcpy(o + k, cand, l);
            k += l;
            o[k++] = '>';
            any = 1;
        }
        if (!any) { o[k++] = '<'; o[k++] = '!'; o[k++] = '>'; }
        o[k++] = ']';
        o[k] = 0;
        return o;
    }
    int sl = 0;
    const char *sym = rk_rx_sym_of(name, nlen, &sl);
    if (sym && strstr(body, "<sym>")) {
        size_t cap = strlen(body) + (size_t) sl * 4 + 32;
        char *o = (char *) rt_wsb_alloc(cap);
        size_t k = 0;
        for (const char *b = body; *b;) {
            if (!strncmp(b, "<sym>", 5)) {
                memcpy(o + k, "$<sym>='", 8);
                k += 8;
                for (int j = 0; j < sl; j++) { if (sym[j] == '\\' || sym[j] == '\'') o[k++] = '\\'; o[k++] = sym[j]; }
                o[k++] = '\'';
                b += 5;
            } else o[k++] = *b++;
        }
        o[k] = 0;
        return o;
    }
    return body;
}
static int rk_gc_of(unsigned c) {
    int lo = 0, hi = rk_gc_nrng - 1;
    while (lo <= hi) { int mid = (lo + hi) / 2; if (c < rk_gc_rng[mid][0]) hi = mid - 1; else if (c > rk_gc_rng[mid][1]) lo = mid + 1; else return (int) rk_gc_rng[mid][2]; }
    return 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gc_loose(const char *a, int al, const char *b) {
    int i = 0, j = 0;
    for (;;) {
        while (i < al && (a[i] == '_' || a[i] == '-' || a[i] == ' ')) i++;
        while (b[j] == '_' || b[j] == '-' || b[j] == ' ') j++;
        if (i >= al || !b[j]) return i >= al && !b[j];
        if (tolower((unsigned char) a[i]) != tolower((unsigned char) b[j])) return 0;
        i++;
        j++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gc_named(const char *nm, int nl, unsigned c) {
    const struct {
        const char *n;
        char c;
    } gl[] = { { "Letter", 'L' }, { "Mark", 'M' }, { "Combining_Mark", 'M' }, { "Number", 'N' }, { "Punctuation", 'P' }, { "Symbol", 'S' }, { "Separator", 'Z' }, { "Other", 'C' }, { "L", 'L' }, { "M",
        'M' }, { "N", 'N' }, { "P", 'P' }, { "S", 'S' }, { "Z", 'Z' }, { "C", 'C' }, { 0, 0 } };
    int g = rk_gc_of(c);
    for (int k = 0; k < 30; k++) if (rk_gc_loose(nm, nl, rk_gc_short[k]) || rk_gc_loose(nm, nl, rk_gc_long[k])) return g == k;
    if (rk_gc_loose(nm, nl, "LC") || rk_gc_loose(nm, nl, "Cased_Letter")) return g == 5 || g == 8 || g == 9;
    for (int k = 0; gl[k].n; k++) if (rk_gc_loose(nm, nl, gl[k].n)) return rk_gc_short[g][0] == gl[k].c;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_rx_prop(void *ud, const char *name, int nlen, unsigned cp) { (void) ud; return nlen > 0 ? rk_gc_named(name, nlen, cp) : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RxProg *rk_rx_compile(RxEnv *env, const char *gname, const char *pat, int flags) {
    env->ud = (void *) gname;
    env->rule = rk_rx_rule;
    env->fold = NULL;
    env->prop = rk_rx_prop;
    const char *err = NULL;
    RxProg *p = rx_compile(pat, flags, env, &err);
    if (!p) fprintf(stderr, "re: compile error: %s\n", err ? err : "?");
    return p;
}
static int rk_grp_find(const RkGrp *g, int ng, char kind, int idx, const char *nm, int nl) {
    for (int k = 0; k < ng; k++) if (g[k].kind == kind && (kind == 'p' ? g[k].idx == idx : (g[k].nl == nl && !strncmp(g[k].nm, nm, (size_t) nl)))) return k;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cap_is(const RxCap *c, char kind, int idx, const char *nm, int nl) {
    if (kind == 'p') return c->kind == 'p' && c->idx == idx;
    if (c->kind != 'n' || !nm) return 0;
    return (c->nlen == nl && !strncmp(c->name, nm, (size_t) nl)) || (c->name2 && c->nlen2 == nl && !strncmp(c->name2, nm, (size_t) nl));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_rx_to_match(Match *gm, const RxProg *p, const RxMatch *rm) {
    memset(gm, 0, sizeof *gm);
    if (!rm->ok) return;
    int ntops, nev = rm->nev;
    const RxCap *const *tops = rx_tops(p, &ntops);
    int topidx[nev + 1], nt = 0;
    { int tmp[nev + 1], n2 = 0; for (int i = nev - 1; i >= 0; i = rm->ev[i].sub - 1) tmp[n2++] = i; for (int a = n2 - 1; a >= 0; a--) topidx[nt++] = tmp[a]; }
    int maxp = -1;
    for (int t = 0; t < ntops; t++) if (tops[t]->kind == 'p') {
        int has = tops[t]->rep;
        for (int a = 0; a < nt; a++) if (rm->ev[topidx[a]].cap->kind == 'p' && rm->ev[topidx[a]].cap->idx == tops[t]->idx) has = 1;
        if (has && tops[t]->idx > maxp) maxp = tops[t]->idx;
    }
    for (int a = 0; a < nt; a++) if (rm->ev[topidx[a]].cap->kind == 'p' && rm->ev[topidx[a]].cap->idx > maxp) maxp = rm->ev[topidx[a]].cap->idx;
    RkGrp grp[maxp + 1 + 2 * (ntops + nt) + 1];
    int ng = 0;
    for (int i = 0; i <= maxp; i++) {
        grp[ng] = (RkGrp) { 'p', i, NULL, 0, 0, -1, -1 };
        for (int t = 0; t < ntops; t++) if (tops[t]->kind == 'p' && tops[t]->idx == i) grp[ng].rep |= tops[t]->rep;
        ng++;
    }
    for (int t = 0; t < ntops + nt; t++) {
        const RxCap *c = t < ntops ? tops[t] : rm->ev[topidx[t - ntops]].cap;
        if (c->kind != 'n') continue;
        for (int w = 0; w < 2; w++) {
            const char *nm = w ? c->name2 : c->name;
            int nl = w ? c->nlen2 : c->nlen;
            if (!nm || !nl) continue;
            if (rk_grp_find(grp, ng, 'n', 0, nm, nl) < 0) grp[ng++] = (RkGrp) { 'n', 0, nm, nl, 0, -1, -1 };
        }
    }
    int nbefore = ng;
    ng = 0;
    RkGrp fin[nbefore + 1];
    for (int k = 0; k < nbefore; k++) {
        int has = 0;
        for (int t = 0; t < ntops; t++) if (rk_cap_is(tops[t], grp[k].kind, grp[k].idx, grp[k].nm, grp[k].nl)) grp[k].rep |= tops[t]->rep;
        for (int a = 0; a < nt; a++) if (rk_cap_is(rm->ev[topidx[a]].cap, grp[k].kind, grp[k].idx, grp[k].nm, grp[k].nl)) has = 1;
        if (has || grp[k].rep || grp[k].kind == 'p') fin[ng++] = grp[k];
    }
    int ncl = 0, evg[nev + 1][2];
    for (int i = 0; i < nev; i++) evg[i][0] = evg[i][1] = -1;
    for (int a = 0; a < nt; a++) {
        int i = topidx[a];
        const RxCap *c = rm->ev[i].cap;
        if (c->kind == 'p') evg[i][0] = rk_grp_find(fin, ng, 'p', c->idx, NULL, 0);
        else { evg[i][0] = rk_grp_find(fin, ng, 'n', 0, c->name, c->nlen); if (c->name2 && c->nlen2) evg[i][1] = rk_grp_find(fin, ng, 'n', 0, c->name2, c->nlen2); }
        for (int w = 0; w < 2; w++) if (evg[i][w] >= 0) { RkGrp *g = &fin[evg[i][w]]; g->start = rm->ev[i].from; g->end = rm->ev[i].to; ncl++; }
    }
    size_t nb = 1;
    for (int k = 0; k < ng; k++) nb += (size_t) fin[k].nl + 1;
    for (int i = 0; i < nev; i++) { const RxCap *c = rm->ev[i].cap; if (c->kind == 'n') nb += (size_t) c->nlen + 1 + (size_t) c->nlen2 + 1; }
    size_t ints = (size_t) (4 * ng + 3 * ncl + 10 * nev);
    gm->matched = 1;
    gm->full_start = rm->from;
    gm->full_end = rm->to;
    gm->ngroups = ng;
    gm->ncaplog = ncl;
    gm->nev = nev;
    gm->blk = rt_wsb_alloc(ints * sizeof(int) + nb);
    char *names = gm->blk + ints * sizeof(int), *np = names;
    *np++ = '\0';
    for (int k = 0; k < ng; k++) {
        int *gr = MATCH_GRP(gm, k);
        gr[0] = fin[k].start;
        gr[1] = fin[k].end;
        gr[2] = fin[k].rep;
        if (fin[k].nl) { gr[3] = (int) (np - gm->blk); memcpy(np, fin[k].nm, (size_t) fin[k].nl); np += fin[k].nl; *np++ = '\0'; } else gr[3] = (int) (names - gm->blk);
    }
    int lk = 0;
    for (int a = 0; a < nt; a++) {
        int i = topidx[a];
        for (int w = 0; w < 2; w++) if (evg[i][w] >= 0) { int *lg = MATCH_LOG(gm, lk++); lg[0] = evg[i][w]; lg[1] = rm->ev[i].from; lg[2] = rm->ev[i].to; }
    }
    int cnt[ng + 1];
    for (int k = 0; k < ng; k++) cnt[k] = 0;
    for (int a = 0; a < nt; a++) for (int w = 0; w < 2; w++) if (evg[topidx[a]][w] >= 0) cnt[evg[topidx[a]][w]]++;
    for (int k = 0; k < ng; k++) if (cnt[k] > 1) MATCH_GRP(gm, k)[2] = 1;
    for (int i = 0; i < nev; i++) {
        const RxCap *c = rm->ev[i].cap;
        int *e = RK_EV(gm, i);
        e[0] = c->kind == 'p' ? 0 : 1;
        e[1] = c->idx;
        e[2] = -1;
        e[3] = -1;
        e[4] = c->rep;
        e[5] = rm->ev[i].from;
        e[6] = rm->ev[i].to;
        e[7] = rm->ev[i].sub;
        e[8] = evg[i][0];
        e[9] = evg[i][1];
        if (c->kind == 'n') {
            e[2] = (int) (np - gm->blk);
            memcpy(np, c->name, (size_t) c->nlen);
            np += c->nlen;
            *np++ = '\0';
            if (c->name2 && c->nlen2) { e[3] = (int) (np - gm->blk); memcpy(np, c->name2, (size_t) c->nlen2); np += c->nlen2; *np++ = '\0'; }
        }
    }
}
static int rk_int_exact(DESCR_t v, long long *out) {
    extern int rk_rat_method(const char *, DESCR_t, DESCR_t *, DESCR_t *);
    extern int rk_rat_to_int(DESCR_t, long long *);
    DESCR_t dn;
    if (IS_INT_fn(v)) { *out = (long long) v.i; return 1; }
    return v.v == DT_DATA && rk_rat_method("denominator", v, NULL, &dn) && dn.v == DT_I && dn.i == 1 && rk_rat_to_int(v, out);
}
double rk_rat_num(DESCR_t v) { extern int rk_rat_to_real(DESCR_t, double *); double r; return v.v == DT_DATA && rk_rat_to_real(v, &r) ? r : 0.0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_rat_dispatch(const char *meth, DESCR_t *recv, const DESCR_t *margs, int nmargs, DESCR_t *out) {
    extern int rk_rat_is(DESCR_t);
    extern int rk_rat_method(const char *, DESCR_t, DESCR_t *, DESCR_t *);
    extern int rk_rat_int_method(const char *, DESCR_t, DESCR_t *);
    extern int rk_rat_to_real(DESCR_t, double *);
    extern int rk_rat_to_int(DESCR_t, long long *);
    extern const char *rk_rat_str(DESCR_t, int);
    static const char *const strmeth[] = { "chars", "codes", "lc", "uc", "tc", "fc", "tclc", "flip", "chop", "chomp", "trim", "trim-leading", "trim-trailing", "wordcase", "ord", "ords", "lines",
        "words", "comb", "contains", "starts-with", "ends-with", "index", "rindex", "indices", "split", "substr", "substr-eq", "substr-rw", "samecase", "samemark", "samespace", "subst", "trans",
        "match", "parse-base", "encode", "indent", "wrap", NULL };
    static const char *const keepmeth[] = { "WHAT", "WHICH", "HOW", "WHO", "VAR", "DEFINITE", "isa", "does", "can", "ACCEPTS", "clone", "Numeric", "Real", "Rational", "Bridge", NULL };
    if (rk_rat_is(*recv)) {
        if (!strcmp(meth, "base") && (nmargs == 1 || nmargs == 2)) {
            extern int rk_rat_base(DESCR_t self, DESCR_t radix, const DESCR_t *digd, DESCR_t *out);
            long long iv;
            if (!rk_int_exact(*recv, &iv) && rk_rat_base(*recv, margs[0], nmargs == 2 ? &margs[1] : NULL, out)) return 1;
        }
        if (nmargs <= 1) {
            if (!strcmp(meth, "nude") && nmargs == 0) {
                DESCR_t pr[2];
                rk_rat_method("numerator", *recv, NULL, &pr[0]);
                rk_rat_method("denominator", *recv, NULL, &pr[1]);
                *out = rk_mark_list(rk_mk_arr(pr, 2));
                return 1;
            }
            DESCR_t arg0 = nmargs ? margs[0] : NULVCL;
            if (!strcmp(meth, "fmt") && nmargs <= 1) {
                DESCR_t fa[2] = { nmargs ? margs[0] : STRVAL("%s"), *recv };
                extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
                return script_try_call_builtin_by_name("__rk_sprintf_d", fa, 2, out);
            }
            if (rk_rat_method(meth, *recv, nmargs ? &arg0 : NULL, out)) return 1;
            long long iv;
            if (!strcmp(meth, "chr") && nmargs == 0) { if (rk_rat_to_int(*recv, &iv)) *recv = INTVAL(iv); return 0; }
            if (!strcmp(meth, "is-prime") && nmargs == 0) { *recv = INTVAL(rk_int_exact(*recv, &iv) ? iv : 0); return 0; }
            if (!strcmp(meth, "base") && nmargs == 1 && rk_int_exact(*recv, &iv)) { *recv = INTVAL(iv); return 0; }
        }
        for (int nm = 0; keepmeth[nm]; nm++) if (!strcmp(meth, keepmeth[nm])) return 0;
        for (int nm = 0; strmeth[nm]; nm++) if (!strcmp(meth, strmeth[nm])) { *recv = STRVAL((char *) rk_rat_str(*recv, 0)); return 0; }
        double rv;
        if (rk_rat_to_real(*recv, &rv)) *recv = REALVAL(rv);
        return 0;
    }
    if (nmargs > 1) return 0;
    if (recv->v == DT_R) {
        extern int rk_rat_real_method(const char *, DESCR_t, DESCR_t *, DESCR_t *);
        DESCR_t arg0 = nmargs ? margs[0] : NULVCL;
        return rk_rat_real_method(meth, *recv, nmargs ? &arg0 : NULL, out);
    }
    if ((recv->v == DT_S || recv->v == DT_SNUL) && nmargs == 0 && (meth[0] == 'R' || meth[0] == 'F')) {
        extern int rk_rat_str_method(const char *, DESCR_t, DESCR_t *);
        return rk_rat_str_method(meth, *recv, out);
    }
    if ((recv->v == DT_I || recv->v == DT_BIG) && nmargs == 0) {
        if (!strcmp(meth, "nude")) { DESCR_t pr[2] = { *recv, INTVAL(1) }; *out = rk_mark_list(rk_mk_arr(pr, 2)); return 1; }
        return rk_rat_int_method(meth, *recv, out);
    }
    return 0;
}
DESCR_t bt_truncate(DESCR_t *args, int nargs) {
    if (IS_REAL_fn(args[0]) && !isfinite(args[0].r)) { return args[0]; }
    { return INTVAL((long)to_real(args[0])); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_floor(DESCR_t *args, int nargs) {
    if (IS_REAL_fn(args[0]) && !isfinite(args[0].r)) { return args[0]; }
    { return INTVAL((long)floor(to_real(args[0]))); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_round(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (IS_REAL_fn(args[0]) && !isfinite(args[0].r)) { return args[0]; }
    {
        if ((nargs - 1) >= 1 && to_real(args[1 + (0)]) != 0.0) {
            double sc = to_real(args[1 + (0)]), rv = floor(to_real(args[0]) / sc + 0.5) * sc;
            if (IS_INT_fn(args[1 + (0)]) && IS_INT_fn(args[0])) *out = INTVAL((long) rv);
            else *out = REALVAL(rv);
            return _bo;
        }
        return INTVAL((long)floor(to_real(args[0]) + 0.5));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_ceiling(DESCR_t *args, int nargs) {
    if (IS_REAL_fn(args[0]) && !isfinite(args[0].r)) { return args[0]; }
    { return INTVAL((long)ceil(to_real(args[0]))); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_sin(DESCR_t *args, int nargs) { { return REALVAL(sin(to_real(args[0]))); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_m_cos(DESCR_t *args, int nargs) { { return REALVAL(cos(to_real(args[0]))); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_m_tan(DESCR_t *args, int nargs) { { return REALVAL(tan(to_real(args[0]))); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_sec(DESCR_t *args, int nargs) { if ((nargs - 1) == 0) { return REALVAL(1.0 / cos(to_real(args[0]))); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_cosec(DESCR_t *args, int nargs) { if ((nargs - 1) == 0) { return REALVAL(1.0 / sin(to_real(args[0]))); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_cotan(DESCR_t *args, int nargs) { if ((nargs - 1) == 0) { return REALVAL(1.0 / tan(to_real(args[0]))); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_atan2(DESCR_t *args, int nargs) { if ((nargs - 1) <= 1) { return REALVAL(atan2(to_real(args[0]), (nargs - 1) ? to_real(args[1 + (0)]) : 1.0)); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_sign(DESCR_t *args, int nargs) {
    if ((nargs - 1) == 0 && (IS_INT_fn(args[0]) || IS_REAL_fn(args[0]))) { double d = to_real(args[0]); return isnan(d) ? args[0] : INTVAL(d < 0 ? -1 : d > 0 ? 1 : 0); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_isNaN(DESCR_t *args, int nargs) { if ((nargs - 1) == 0 && IS_REAL_fn(args[0])) { return (DESCR_t){ .v = DT_BOOL, .i = isnan(args[0].r) }; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static unsigned long long rk_mulmod64(unsigned long long a, unsigned long long b, unsigned long long m) { return (unsigned long long) ((unsigned __int128) a * b % m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned long long rk_powmod64(unsigned long long a, unsigned long long e, unsigned long long m) {
    unsigned long long r = 1;
    for (a %= m; e; e >>= 1) { if (e & 1) r = rk_mulmod64(r, a, m); a = rk_mulmod64(a, a, m); }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_isprime64(unsigned long long n) {
    const unsigned char *sp = (const unsigned char *) "\2\3\5\7\13\15\21\23\27\35\37\45";
    if (n < 2) return 0;
    for (int i = 0; i < 12; i++) { if (n == sp[i]) return 1; if (n % sp[i] == 0) return 0; }
    unsigned long long d = n - 1;
    int s = 0;
    while (!(d & 1)) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        unsigned long long x = rk_powmod64(sp[i], d, n);
        int comp = x != 1 && x != n - 1;
        for (int r = 1; r < s && comp; r++) { x = rk_mulmod64(x, x, n); if (x == n - 1) comp = 0; }
        if (comp) return 0;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_bpowmod(DESCR_t a, DESCR_t e, DESCR_t m) {
    extern DESCR_t rt_big_mul(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_div(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_mod(DESCR_t, DESCR_t);
    extern int rt_big_cmp(DESCR_t, DESCR_t);
    extern int rt_big_is_odd(DESCR_t);
    DESCR_t r = INTVAL(1);
    a = rt_big_mod(a, m);
    while (rt_big_cmp(e, INTVAL(0)) > 0) {
        if (rt_big_is_odd(e)) r = rt_big_mod(rt_big_mul(r, a), m);
        e = rt_big_div(e, INTVAL(2));
        if (rt_big_cmp(e, INTVAL(0)) > 0) a = rt_big_mod(rt_big_mul(a, a), m);
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_isprime_big(DESCR_t n) {
    extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_div(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_mod(DESCR_t, DESCR_t);
    extern int rt_big_cmp(DESCR_t, DESCR_t);
    extern int rt_big_is_odd(DESCR_t);
    const unsigned char *sp = (const unsigned char *) "\2\3\5\7\13\15\21\23\27\35\37\45";
    if (rt_big_cmp(n, INTVAL(2)) < 0) return 0;
    for (int i = 0; i < 12; i++) if (rt_big_cmp(rt_big_mod(n, INTVAL(sp[i])), INTVAL(0)) == 0) return 0;
    DESCR_t nm1 = rt_big_sub(n, INTVAL(1)), d = nm1;
    int s = 0;
    while (!rt_big_is_odd(d)) { d = rt_big_div(d, INTVAL(2)); s++; }
    for (int i = 0; i < 12; i++) {
        DESCR_t x = rk_bpowmod(INTVAL(sp[i]), d, n);
        int comp = rt_big_cmp(x, INTVAL(1)) != 0 && rt_big_cmp(x, nm1) != 0;
        extern DESCR_t rt_big_mul(DESCR_t, DESCR_t);
        for (int r = 1; r < s && comp; r++) { x = rt_big_mod(rt_big_mul(x, x), n); if (rt_big_cmp(x, nm1) == 0) comp = 0; }
        if (comp) return 0;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_isprime_num(DESCR_t v, int *ok) {
    extern int rt_big_is(DESCR_t);
    extern DESCR_t rt_big_shl(DESCR_t, int64_t);
    *ok = 1;
    if (IS_INT_fn(v)) return v.i >= 2 && rk_isprime64((unsigned long long) v.i);
    if (rt_big_is(v)) return rk_isprime_big(v);
    if (IS_REAL_fn(v)) {
        if (!isfinite(v.r) || v.r != floor(v.r) || v.r < 2.0) return 0;
        if (v.r < 9.2e18) return rk_isprime64((unsigned long long) v.r);
        int ex;
        double fr = frexp(v.r, &ex);
        return rk_isprime_big(rt_big_shl(INTVAL((long long) ldexp(fr, 53)), ex - 53));
    }
    long long iv;
    if (v.v == DT_DATA && rk_int_exact(v, &iv)) return iv >= 2 && rk_isprime64((unsigned long long) iv);
    if (v.v == DT_DATA && rk_rat_is(v)) return 0;
    *ok = 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_isprime_str(DESCR_t s, int *ok) {
    extern int rk_rat_str_method(const char *, DESCR_t, DESCR_t *);
    const char *t = rk_cstr(s);
    while (*t == ' ' || *t == '\t' || *t == '\n') t++;
    char b[strlen(t) + 2];
    if (!strncmp(t, "\xe2\x88\x92", 3)) { b[0] = '-'; strcpy(b + 1, t + 3); } else strcpy(b, t);
    DESCR_t r;
    *ok = 1;
    if (!*b) return 0;
    size_t bl = strlen(b);
    if (bl > 1 && b[bl - 1] == 'i') {
        size_t k = bl - 2;
        while (k > 0 && !((b[k] == '+' || b[k] == '-') && b[k - 1] != 'e' && b[k - 1] != 'E')) k--;
        if (strtod(b + k, NULL) != 0.0) {
            extern void rt_script_die_surface(const char *msg);
            char msg[bl + 64];
            snprintf(msg, sizeof msg, "Cannot convert %s to Real: imaginary part not zero", b);
            rt_script_die_surface(msg);
            *ok = 2;
            return 0;
        }
        b[k] = 0;
        if (!*b) return 0;
    }
    if (!rk_rat_str_method("Rat", STRVAL(b), &r)) { *ok = 0; return 0; }
    return rk_isprime_num(r, ok);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_isx2Dprime(DESCR_t *args, int nargs) {
    int ok = 0, pr = 0;
    if ((nargs - 1) == 0) pr = IS_STR_fn(args[0]) ? rk_isprime_str(args[0], &ok) : rk_isprime_num(args[0], &ok);
    if (ok == 2) return FAILDESCR;
    if (ok) return (DESCR_t){ .v = DT_BOOL, .i = pr };
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_read_src_from_fp(FILE *f, char *rb, int cap) {
    int rn = 0;
    int rc;
    int rql = 0;
    char rq = 0;
    for (;;) {
        rc = fgetc(f);
        if (rc == EOF) break;
        if (rql) { if (rc == '\n') rql = 0; continue; }
        if (rc == ' ' || rc == '\t' || rc == '\n' || rc == '\r') continue;
        if (rc == '%') { rql = 1; continue; }
        break;
    }
    if (rc == EOF) return -1;
    for (;;) {
        if (rq) { if (rn < cap - 1) rb[rn++] = (char)rc; if (rc == rq) rq = 0; rc = fgetc(f); if (rc == EOF) break; continue; }
        if (rc == '\'' || rc == '"' || rc == '`') { rq = (char)rc; if (rn < cap - 1) rb[rn++] = (char)rc; rc = fgetc(f); if (rc == EOF) break; continue; }
        if (rc == '.') { int nx = fgetc(f); if (nx == EOF || nx == ' ' || nx == '\t' || nx == '\n' || nx == '\r' || nx == '%') break; if (rn < cap - 1) rb[rn++] = '.'; rc = nx; continue; }
        if (rn < cap - 1) rb[rn++] = (char)rc;
        rc = fgetc(f);
        if (rc == EOF) break;
    }
    rb[rn] = 0;
    return rn;
}
DESCR_t bt_re_match(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        const char *subj = VARVAL_fn(args[0]);
        if (!subj) subj = "";
        const char *pat = VARVAL_fn(args[1]);
        if (!pat) pat = "";
        RxEnv renv;
        RxProg *prog = rk_rx_compile(&renv, "", pat, 0);
        if (!prog) { return FAILDESCR; }
        RxMatch rm;
        rx_exec(prog, subj, (int) strlen(subj), 0, 0, &rm);
        rk_rx_to_match(&g_match, prog, &rm);
        g_subject = subj;
        return g_match.matched ? rk_match_obj(&g_match, subj) : FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_make(DESCR_t *args, int nargs) { if (nargs == 2) { if (IS_DATA_INST_fn(args[0])) FIELD_SET_fn(args[0], "made", args[1]); return args[1]; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_cap_named(DESCR_t *args, int nargs, int _bop) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 2) {
        *out = NULVCL;
        if (!IS_DATA_INST_fn(args[0])) return _bo;
        if ((_bop == 0)) {
            char kb[128];
            const char *key = to_cstring(args[1], kb, sizeof kb);
            DESCR_t hn = rk_hash_list(FIELD_GET_fn(args[0], "named"), 'w');
            ARBLK_t *hb = (hn.v == DT_A && hn.arr) ? (ARBLK_t *) hn.arr : NULL;
            int nh = hb ? (hb->hi - hb->lo + 1) / 2 : 0;
            for (int q = 0; q < nh && key; q++) { const char *kk = rk_cstr(hb->data[2 * q]); if (kk && !strcmp(kk, key)) { *out = hb->data[2 * q + 1]; break; } }
        } else {
            DESCR_t pa = FIELD_GET_fn(args[0], "positional");
            long i = rk_ival(args[1]);
            ARBLK_t *pb = (pa.v == DT_A && pa.arr) ? (ARBLK_t *) pa.arr : NULL;
            if (pb && i >= 0 && i < pb->hi - pb->lo + 1) *out = pb->data[i];
        }
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_cap_named(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_cap_named(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_cap_pos(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_cap_named(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta_re_matchx(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 6) {
        const char *subj = VARVAL_fn(args[0]);
        if (!subj) subj = "";
        const char *pat = VARVAL_fn(args[1]);
        if (!pat) pat = "";
        RxEnv renv;
        RxProg *prog = rk_rx_compile(&renv, "", pat, 0);
        if (!prog) { return FAILDESCR; }
        long nth[nargs - 6 > 0 ? nargs - 6 : 1];
        for (int i = 6; i < nargs; i++) nth[i - 6] = args[i].i;
        RkRxOpt o = { args[2].i, args[3].i, args[4].i, args[5].i, nth, nargs - 6 };
        DESCR_t r;
        int ok = rk_rx_collect(prog, subj, &o, &r);
        return ok ? ((_bop == 1) ? INTVAL(1) : r) : FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_re_testx(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_re_matchx(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_re_matchx(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_re_matchx(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_re_test(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        const char *subj = VARVAL_fn(args[0]);
        if (!subj) subj = "";
        const char *pat = VARVAL_fn(args[1]);
        if (!pat) pat = "";
        RxEnv renv;
        RxProg *prog = rk_rx_compile(&renv, "", pat, 0);
        if (!prog) { return FAILDESCR; }
        RxMatch rm;
        rx_exec(prog, subj, (int) strlen(subj), 0, 0, &rm);
        rk_rx_to_match(&g_match, prog, &rm);
        g_subject = subj;
        return g_match.matched ? INTVAL(1) : FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_nfa_accepts(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        DESCR_t topic = args[0], matcher = args[1];
        if (junction_is(matcher)) { int numeric = (IS_INT_fn(topic) || IS_REAL_fn(topic)); int ok = junction_collapse(topic, matcher, TT_EQ, numeric); return ok ? INTVAL(1) : FAILDESCR; }
        int both_num = (IS_INT_fn(topic) || IS_REAL_fn(topic)) && (IS_INT_fn(matcher) || IS_REAL_fn(matcher));
        int ok;
        if (both_num) {
            ok = (to_real(topic) == to_real(matcher));
        } else {
            const char *ts = VARVAL_fn(topic);
            if (!ts) ts = "";
            const char *ms = VARVAL_fn(matcher);
            if (!ms) ms = "";
            ok = (strcmp(ts, ms) == 0);
        }
        return ok ? INTVAL(1) : FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_re_match_global(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        const char *subj = VARVAL_fn(args[0]);
        if (!subj) subj = "";
        const char *pat = VARVAL_fn(args[1]);
        if (!pat) pat = "";
        RxEnv renv;
        RxProg *prog = rk_rx_compile(&renv, "", pat, 0);
        if (!prog) { return STRVAL(rt_heap_strdup_c("")); }
        int slen = (int)strlen(subj);
        DESCR_t *mr = (DESCR_t *) rt_ws_alloc_descr((size_t) slen + 1);
        int pos = 0, count = 0;
        DESCR_t gorig = STRVAL(rt_heap_strdup_c(subj));
        while (pos <= slen) {
            RxMatch rm;
            if (!rx_exec(prog, subj, slen, pos, 0, &rm)) break;
            rk_rx_to_match(&g_match, prog, &rm);
            g_subject = subj;
            mr[count] = rk_match_obj_in(&g_match, subj, gorig);
            if (rm.to > rm.from) pos = rm.to;
            else { int l = 1; while (rm.to + l < slen && ((unsigned char) subj[rm.to + l] & 0xC0) == 0x80) l++; pos = rm.to + l; }
            count++;
        }
        return count > 0 ? rk_mk_arr(mr, count) : FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned rk_box_cp(const char *s, long i, long end, int *len) {
    unsigned c = (unsigned char) s[i];
    int k = c < 0x80 ? 1 : c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
    if (i + k > end) k = 1;
    *len = k;
    if (k == 1) return c;
    { unsigned v = c & (0x7Fu >> k); for (int j = 1; j < k; j++) v = (v << 6) | ((unsigned char) s[i + j] & 0x3Fu); return v; }
}
static int rk_box_class_named(const char *nm, int nl, unsigned c) {
    if (nl == 5 && !strncmp(nm, "digit", 5)) return c >= '0' && c <= '9';
    if (nl == 4 && !strncmp(nm, "word", 4)) return c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c >= 0xAA;
    if (nl == 5 && !strncmp(nm, "space", 5)) return c == ' ' || (c >= 9 && c <= 13) || c == 0x85 || c == 0xA0 || c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x2028 || c == 0x2029 ||
        c == 0x202F || c == 0x205F || c == 0x3000;
    if (nl == 6 && !strncmp(nm, "hspace", 6)) return c == 9 || c == ' ' || c == 0xA0 || c == 0x1680 || (c >= 0x2000 && c <= 0x200A) || c == 0x202F || c == 0x205F || c == 0x3000;
    if (nl == 6 && !strncmp(nm, "vspace", 6)) return (c >= 10 && c <= 13) || c == 0x85 || c == 0x2028 || c == 0x2029;
    if (nl == 2 && !strncmp(nm, "nl", 2)) return c == 10 || c == 11 || c == 12 || c == 13 || c == 0x85 || c == 0x2028 || c == 0x2029;
    if (nl == 5 && !strncmp(nm, "alpha", 5)) return c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c >= 0xAA;
    if (nl == 5 && !strncmp(nm, "alnum", 5)) return (c >= '0' && c <= '9') || c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c >= 0xAA;
    if (nl == 5 && !strncmp(nm, "upper", 5)) return c >= 'A' && c <= 'Z';
    if (nl == 5 && !strncmp(nm, "lower", 5)) return c >= 'a' && c <= 'z';
    if (nl == 5 && !strncmp(nm, "punct", 5)) return c < 0x80 && ispunct((int) c);
    if (nl == 6 && !strncmp(nm, "xdigit", 6)) return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    if (nl == 5 && !strncmp(nm, "cntrl", 5)) return c < 32 || c == 127;
    if (nl == 5 && !strncmp(nm, "print", 5)) return c >= 32 && c != 127;
    if (nl == 5 && !strncmp(nm, "graph", 5)) return c > 32 && c != 127;
    if (nl == 5 && !strncmp(nm, "blank", 5)) return c == ' ' || c == 9;
    if (nl == 5 && !strncmp(nm, "ascii", 5)) return c < 0x80;
    return rk_gc_named(nm, nl, c) > 0;
}
static int rk_box_class_member(const char *p, unsigned c) {
    int in = (*p == '-') ? 1 : 0;
    while (*p) {
        int sub = 0, hit = 0;
        if (*p == '-') { sub = 1; p++; }
        if (*p == 'r') {
            char *e;
            unsigned lo = (unsigned) strtoul(p + 1, &e, 10), hi = (unsigned) strtoul(e + 1, &e, 10);
            hit = c >= lo && c <= hi;
            p = e;
        } else if (*p == 'n' || *p == 'N') {
            const char *q = strchr(p + 1, ';');
            if (!q) break;
            hit = rk_box_class_named(p + 1, (int) (q - p - 1), c) ^ (*p == 'N');
            p = q;
        } else break;
        if (*p == ';') p++;
        if (sub) { if (hit) in = 0; } else if (hit) in = 1;
    }
    return in;
}
static int rk_box_mid(const char *subj, long delta) {
    if (delta <= 0 || ((unsigned char) subj[delta] & 0xC0) != 0x80) return 0;
    for (int j = 1; j <= 3 && delta - j >= 0; j++) { unsigned char b = (unsigned char) subj[delta - j]; if ((b & 0xC0) == 0x80) continue; return b >= 0xC0 && j < (b >= 0xF0 ? 4 : b >= 0xE0 ? 3 : 2); }
    return 0;
}
long rk_box_step(const char *subj, long delta, long end, const char *blob) {
    const char *spec = blob ? strchr(blob, ';') : (const char *) 0;
    if (!spec) return -1;
    spec++;
    if (blob[0] != 'L' && delta < end && rk_box_mid(subj, delta)) return -1;
    if (blob[0] == 'L') { long k = (long) strlen(spec); return (delta + k <= end && !memcmp(subj + delta, spec, (size_t) k)) ? delta + k : -1; }
    if (blob[0] == 'A') { int l; if (delta >= end) return -1; rk_box_cp(subj, delta, end, &l); return delta + l; }
    if (blob[0] == 'C') { int l; unsigned c; if (delta >= end) return -1; c = rk_box_cp(subj, delta, end, &l); return rk_box_class_member(spec, c) ? delta + l : -1; }
    return -1;
}
long rk_box_back(const char *subj, long delta, const char *blob, long floor_delta) {
    const char *spec = blob ? strchr(blob, ';') : (const char *) 0;
    if (spec && blob[0] == 'L') { long k = (long) strlen(spec + 1); return delta - k >= floor_delta ? delta - k : floor_delta; }
    if (delta <= floor_delta) return floor_delta;
    delta--;
    while (delta > floor_delta && ((unsigned char) subj[delta] & 0xC0) == 0x80) delta--;
    return delta;
}
void rk_box_match_build(const char *subj, long span, const long *cells, const char *names) {
    int start = (int) (span >> 32), end = (int) (span & 0xFFFFFFFFL), n = (int) (cells[0] & 0xFFFFFFFFL), slen = (int) (cells[0] >> 32);
    const int *pairs = (const int *) (cells + 1);
    const char *nm[n > 0 ? n : 1];
    int nl[n > 0 ? n : 1], ncl = 0;
    {
        const char *q = names ? strchr(names, '\n') : (const char *) 0;
        q = q ? q + 1 : "";
        for (int g = 0; g < n; g++) { const char *e = strchr(q, '\n'); nm[g] = q; nl[g] = e ? (int) (e - q) : (int) strlen(q); q = e ? e + 1 : q + strlen(q); if (pairs[2 * g + 1] >= 0) ncl++; }
    }
    {
        size_t nb = 1, ints = (size_t) (4 * n + 3 * ncl + 10 * ncl);
        for (int g = 0; g < n; g++) nb += 2 * ((size_t) nl[g] + 1);
        char *copy = rt_wsb_alloc((size_t) slen + 1);
        if (slen > 0) memcpy(copy, subj, (size_t) slen);
        copy[slen] = '\0';
        memset(&g_match, 0, sizeof g_match);
        g_match.matched = 1;
        g_match.full_start = start;
        g_match.full_end = end;
        g_match.ngroups = n;
        g_match.ncaplog = ncl;
        g_match.nev = ncl;
        g_match.blk = rt_wsb_alloc(ints * sizeof(int) + nb);
        {
            char *base = g_match.blk + ints * sizeof(int), *np = base;
            int lk = 0, ev = 0, pidx = 0;
            *np++ = '\0';
            for (int g = 0; g < n; g++) {
                int *gr = MATCH_GRP(&g_match, g);
                gr[0] = pairs[2 * g];
                gr[1] = pairs[2 * g + 1];
                gr[2] = 0;
                if (nl[g]) { gr[3] = (int) (np - g_match.blk); memcpy(np, nm[g], (size_t) nl[g]); np += nl[g]; *np++ = '\0'; } else gr[3] = (int) (base - g_match.blk);
            }
            for (int g = 0; g < n; g++) {
                int my = nl[g] ? 0 : pidx++;
                if (pairs[2 * g + 1] < 0) continue;
                { int *lg = MATCH_LOG(&g_match, lk++); lg[0] = g; lg[1] = pairs[2 * g]; lg[2] = pairs[2 * g + 1]; }
                {
                    int *e = RK_EV(&g_match, ev);
                    e[0] = nl[g] ? 1 : 0;
                    e[1] = my;
                    e[2] = -1;
                    e[3] = -1;
                    e[4] = 0;
                    e[5] = pairs[2 * g];
                    e[6] = pairs[2 * g + 1];
                    e[7] = ev;
                    e[8] = g;
                    e[9] = -1;
                    if (nl[g]) { e[2] = (int) (np - g_match.blk); memcpy(np, nm[g], (size_t) nl[g]); np += nl[g]; *np++ = '\0'; }
                    ev++;
                }
            }
        }
        g_subject = copy;
    }
}
