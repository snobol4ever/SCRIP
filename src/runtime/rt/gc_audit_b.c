/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifdef SCRIP_GC_AUDIT_B
#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "gc_audit_b.h"
#if RT_DIAG
typedef struct gc_audit_b_ctr_t { long words; long inheap; long marked; long fill; long excl; long found; long shown; long suppressed; long xframe; long xowner; } gc_audit_b_ctr_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_audit_b_log(const char *line) {
    extern const char *g_file;
    const char *lp = getenv("SCRIP_GC_AUDIT_B_LOG");
    const char *src = g_file ? g_file : "-";
    int fd, n;
    if (!lp || !*lp) return;
    n = snprintf((char *)0, 0, "pid=%ld exe=%s src=%s %s", (long)getpid(), program_invocation_short_name, src, line);
    if (n <= 0) return;
    {
        char buf[n + 1];
        snprintf(buf, (size_t)n + 1, "pid=%ld exe=%s src=%s %s", (long)getpid(), program_invocation_short_name, src, line);
        fd = open(lp, O_WRONLY | O_APPEND | O_CREAT | O_CLOEXEC, 0644);
        if (fd < 0) { fprintf(stderr, "[GC-AUDIT-B] SCRIP_GC_AUDIT_B_LOG=%s cannot be opened (errno %d) -- the log is NOT a receipt of this run\n", lp, errno); return; }
        if (write(fd, buf, (size_t)n) != (ssize_t)n) fprintf(stderr, "[GC-AUDIT-B] SCRIP_GC_AUDIT_B_LOG=%s short write -- the log is NOT a receipt of this run\n", lp);
        close(fd);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_audit_b_say(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
static void gc_audit_b_say(const char *fmt, ...) {
    va_list a;
    int n;
    va_start(a, fmt);
    n = vsnprintf((char *)0, 0, fmt, a);
    va_end(a);
    if (n <= 0) return;
    { char ln[n + 1]; va_start(a, fmt); vsnprintf(ln, (size_t)n + 1, fmt, a); va_end(a); fputs(ln, stderr); gc_audit_b_log(ln); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_audit_b_opaque(uint16_t t) { return (t == HB_WSB || t == HB_WSC || t == HB_ZBLK || t == HB_ZCOL || t == HB_AGGB) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *gc_audit_b_excluded(const gc_audit_b_t *v, const char *w) {
    long i;
    for (i = 0; i < v->nskip; i++) { const char *a = (const char *)v->skip[i].at; if (w >= a && w < a + v->skip[i].bytes) return v->skip[i].name; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *gc_audit_b_ct_owner(const gc_audit_b_t *v, const char *blk, char *also, long cap, long *need) {
    Dl_info me, di;
    const char *first = (const char *)0, *mine = (const char *)0;
    long i, used = 0;
    int have_me = dladdr((void *)gc_audit_b_collect, &me) && me.dli_fbase;
    if (also && cap > 0) also[0] = 0;
    for (i = 0; i < v->nrgn; i++) {
        const char *q;
        if (strcmp(v->rgn[i].pop, "static")) continue;
        for (q = (const char *)(((uintptr_t)v->rgn[i].lo + 7u) & ~(uintptr_t)7u); q + 8 <= v->rgn[i].hi; q += 8) {
            if (*(const char *const *)q != blk) continue;
            if (!first) first = q;
            if (!mine && have_me && dladdr((void *)q, &di) && di.dli_fbase == me.dli_fbase) mine = q;
            if (dladdr((void *)q, &di) && di.dli_sname) {
                char *at = (also && used < cap) ? also + used : (char *)0;
                used += snprintf(at, at ? (size_t)(cap - used) : 0, "%s%s", used ? "," : "", di.dli_sname);
            }
        }
    }
    *need = used;
    return mine ? mine : first;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_audit_b_words(const gc_audit_b_t *v, const char *lo, const char *hi, const char *pop, const char *nm, gc_audit_b_ctr_t *c, long cap, const gc_audit_b_xr_t *xr, long nxr) {
    const char *p = (const char *)(((uintptr_t)lo + 7u) & ~(uintptr_t)7u);
    long nw = 0, xi = 0;
    for (; p + 8 <= hi; p += 8) {
        const char *w = *(const char *const *)p;
        const rt_hblk_t *h;
        const char *ex;
        c->words++;
        nw++;
        if (w < v->alo || w >= v->ahi) continue;
        ex = gc_audit_b_excluded(v, p);
        if (ex) { c->excl++; continue; }
        while (xi < nxr && xr[xi].hi <= p) xi++;
        if (xi < nxr && xr[xi].lo <= p) { c->excl++; c->xframe++; continue; }
        if (v->owner_nonref && v->owner_nonref(p)) { c->excl++; c->xowner++; continue; }
        h = v->blk_of(w);
        if (!h) continue;
        c->inheap++;
        if (h->type == HB_FILL) { c->fill++; continue; }
        if (h->flags & HBF_MARK) { c->marked++; continue; }
        c->found++;
        if (c->shown >= cap) { c->suppressed++; continue; }
        c->shown++;
        {
            char bb[512];
            Dl_info di;
            const unsigned char *tb = (const unsigned char *)(h + 1);
            long tn = (long)h->size - (long)sizeof(rt_hblk_t), tm = tn < 24 ? (tn > 0 ? tn : 0) : 24, j;
            int dl, hl;
            const char *sn, *df;
            unsigned long dof;
            char tx[tm + 1];
            for (j = 0; j < tm; j++) tx[j] = (tb[j] >= 32 && tb[j] < 127) ? (char)tb[j] : '.';
            tx[j] = 0;
            bb[0] = 0;
            if (v->birth_of) v->birth_of((const char *)h, bb, (long)sizeof bb);
            {
                long an = 0;
                const char *ow = strcmp(pop, "ctarena") ? (const char *)0 : gc_audit_b_ct_owner(v, lo, (char *)0, 0, &an);
                char also[an + 1];
                also[0] = 0;
                if (ow) gc_audit_b_ct_owner(v, lo, also, an + 1, &an);
                const char *at = ow ? ow : p;
                long co = ow ? (long)(p - lo) : -1;
                dl = dladdr((void *)at, &di) && di.dli_fbase;
                sn = (dl && di.dli_sname) ? di.dli_sname : (nm ? nm : "-");
                df = (dl && di.dli_fname) ? di.dli_fname : "?";
                dof = dl ? (unsigned long)((const char *)at - (const char *)di.dli_fbase) : 0;
                hl = dl ? snprintf((char *)0, 0, co >= 0 ? "%s+0x%lx/%s[ct+0x%lx,owners=%s]" : "%s+0x%lx/%s", df, dof, sn, (unsigned long)co, also) : 0;
                {
                    char hn[hl + 1];
                    if (dl) snprintf(hn, (size_t)hl + 1, co >= 0 ? "%s+0x%lx/%s[ct+0x%lx,owners=%s]" : "%s+0x%lx/%s", df, dof, sn, (unsigned long)co, also);
                    gc_audit_b_say("[GC-AUDIT-B] run=%ld pop=%s CANDIDATE-LOST-ROOT at=%p in=%s word=%p blk=%p type=%u size=%u off=%ld text=%s%s%s\n", v->run, pop, (const void *)p, dl ? hn : sn,
                        (const void *)w, (const void *)h, (unsigned)h->type, (unsigned)h->size, (long)(w - (const char *)h), tx, bb[0] ? " " : "", bb);
                }
            }
        }
    }
    return nw;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct gc_audit_b_ctx_t { const gc_audit_b_t *v; gc_audit_b_ctr_t *c; long cap; long nblk; long nw; } gc_audit_b_ctx_t;
static void gc_audit_b_ct_one(void *x, const char *lo, const char *hi) {
    gc_audit_b_ctx_t *k = (gc_audit_b_ctx_t *)x;
    k->nblk++;
    k->nw += gc_audit_b_words(k->v, lo, hi, "ctarena", "ct", k->c, k->cap, (const gc_audit_b_xr_t *)0, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_audit_b_ct_vma(gc_audit_b_ctx_t *k, const char *lo, const char *hi) {
    extern long ct_arena_scan(const char *lo, const char *hi, void (*fn)(void *ctx, const char *plo, const char *phi), void *ctx);
    const char *hl = k->v->hlo, *hh = k->v->hhi;
    if (hl && hh && lo < hh && hi > hl) { if (lo < hl) ct_arena_scan(lo, hl, gc_audit_b_ct_one, k); if (hi > hh) ct_arena_scan(hh, hi, gc_audit_b_ct_one, k); return; }
    ct_arena_scan(lo, hi, gc_audit_b_ct_one, k);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_audit_b_ctarena(gc_audit_b_ctx_t *k) {
    char buf[16384];
    long have = 0, nvma = 0;
    ssize_t r;
    int fd = open("/proc/self/maps", O_RDONLY | O_CLOEXEC);
    if (fd < 0) { fprintf(stderr, "[GC-AUDIT-B] ctarena: /proc/self/maps cannot be opened (errno %d) -- the compile-time arena was NOT audited this run\n", errno); return -1; }
    while ((r = read(fd, buf + have, sizeof buf - 1 - (size_t)have)) > 0 || have > 0) {
        char *ln, *nl;
        have += r > 0 ? (long)r : 0;
        buf[have] = 0;
        if (r <= 0 && !strchr(buf, '\n')) { buf[have++] = '\n'; buf[have] = 0; }
        for (ln = buf; (nl = strchr(ln, '\n')) != (char *)0; ln = nl + 1) {
            unsigned long a, b, off, ino;
            char perm[8];
            int n = 0;
            *nl = 0;
            if (sscanf(ln, "%lx-%lx %7s %lx %*s %lu %n", &a, &b, perm, &off, &ino, &n) < 5) continue;
            if (strcmp(perm, "rw-p") || ino != 0 || (n > 0 && ln[n])) continue;
            nvma++;
            gc_audit_b_ct_vma(k, (const char *)a, (const char *)b);
        }
        { long j, k0 = (long)(ln - buf); have -= k0; for (j = 0; j < have; j++) buf[j] = buf[k0 + j]; }
        if (r <= 0) break;
        if (have >= (long)sizeof buf - 1) have = 0;
    }
    close(fd);
    return nvma;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long gc_audit_b_collect(const gc_audit_b_t *v) {
    gc_audit_b_ctr_t c;
    long i, cap, nop = 0, nhw = 0;
    const char *e = getenv("SCRIP_GC_AUDIT_B");
    const char *ps = getenv("SCRIP_GC_AUDIT_B_POPS");
    if (!e || !*e || *e == '0') return -1;
    if (!ps || !*ps) ps = "static,stack,heapint,ctarena";
    memset(&c, 0, sizeof c);
    cap = atol(e);
    if (cap <= 1) cap = 64;
    if (cap > 100000) cap = 100000;
    if (v->run <= 1)
        fprintf(stderr,
        "[GC-AUDIT-B] PASS B IS AN AUDITOR AND NEVER A COLLECTOR: pass A (the exact typed walk) has already decided this collection and pass B cannot mark, forward, free or influence it -- it is han"
        "ded a const block lookup and nothing else. IT NAMES CANDIDATES, NEVER DEFECTS: a dead word that looks like a heap pointer reads identically to a live root, so every line below is a CANDIDAT"
        "E to be cured or declared. WHAT IT CANNOT SEE, DECLARED IN THREE PARTS BECAUSE THE THIRD IS THE ONE THAT BITES: (1) a pointer that at the instant of collection lives only in a REGISTER; (2)"
        " the C stack above the emitted-stack ceiling SCRIP_GC_CEILING draws (98.4%% of mode 3's stack, dead compiler frames by measurement, CEO-1030) -- set SCRIP_GC_CEILING=0 and pass B scans that"
        " too; and (3) ⛔ ANY REGION PASS A DOES NOT ENUMERATE, because pass B's territory IS pass A's territory: the libc malloc heap, the compile-time arena and the collector's own mmap'd bookkee"
        "ping are NOT scanned, so THIS AUDITOR FINDS AN UNVISITED WORD INSIDE A VISITED REGION AND CANNOT FIND AN UNVISITED REGION. A findings=0 therefore bounds the probe and never clears the colle"
        "ctor. SUCCESS IS SILENCE, and the findings=0 line is PRINTED so its silence is a statement rather than an absence.\n");
    for (i = 0; i < v->nrgn; i++) {
        long nw;
        if (!strstr(ps, v->rgn[i].pop)) continue;
        nw = gc_audit_b_words(v, v->rgn[i].lo, v->rgn[i].hi, v->rgn[i].pop, v->rgn[i].name, &c, cap, v->rgn[i].xr, v->rgn[i].nxr);
        if (v->run <= 1)
            fprintf(stderr, "[GC-AUDIT-B] region %ld pop=%s name=%s lo=%p hi=%p bytes=%ld words=%ld\n", i, v->rgn[i].pop, v->rgn[i].name, (const void *)v->rgn[i].lo, (const void *)v->rgn[i].hi,
            (long)(v->rgn[i].hi - v->rgn[i].lo), nw);
    }
    for (i = 0; strstr(ps, "heapint") && i < v->nblk; i++) {
        const rt_hblk_t *h = v->blk_at(i);
        if (!h || !(h->flags & HBF_MARK) || !gc_audit_b_opaque(h->type)) continue;
        nop++;
        nhw += gc_audit_b_words(v, (const char *)(h + 1), (const char *)h + h->size, "heapint", "-", &c, cap, (const gc_audit_b_xr_t *)0, 0);
    }
    {
        gc_audit_b_ctx_t k;
        long nv = -2;
        k.v = v;
        k.c = &c;
        k.cap = cap;
        k.nblk = 0;
        k.nw = 0;
        if (strstr(ps, "ctarena")) nv = gc_audit_b_ctarena(&k);
        if (v->run <= 1)
            fprintf(stderr,
            "[GC-AUDIT-B] ctarena vmas=%ld blocks=%ld words=%ld -- the compile-time arena's live blocks, which neither the exact walk nor the static scan reads (-2 = not in pops, -1 = maps unreadabl"
            "e)\n", nv, k.nblk, k.nw);
    }
    gc_audit_b_say("[GC-AUDIT-B] run=%ld audited=1 findings=%ld shown=%ld suppressed=%ld words=%ld inheap=%ld marked=%ld fill=%ld excluded=%ld regions=%ld "
        "heapint_words=%ld opaque_blocks=%ld of %ld opaque_set=ZCOL,ZBLK,WSC,AGGB,WSB pops=%s excluded_frame_header_raw=%ld excluded_owner_declared=%ld\n", v->run, c.found, c.shown, c.suppressed,
        c.words, c.inheap, c.marked, c.fill, c.excl, v->nrgn, nhw, nop, v->nblk, ps, c.xframe, c.xowner);
    return c.found;
}
#endif
#endif
