/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifdef SCRIP_GC_AUDIT_B
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dlfcn.h>
#include "gc_audit_b.h"
typedef struct gc_audit_b_ctr_t { long words; long inheap; long marked; long fill; long excl; long found; long shown; long suppressed; } gc_audit_b_ctr_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gc_audit_b_opaque(uint16_t t) { return (t == HB_WSB || t == HB_WSC || t == HB_ZBLK || t == HB_ZCOL || t == HB_AGGB) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *gc_audit_b_excluded(const gc_audit_b_t *v, const char *w)
{
    long i;
    for (i = 0; i < v->nskip; i++) { const char *a = (const char *)v->skip[i].at; if (w >= a && w < a + v->skip[i].bytes) return v->skip[i].name; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gc_audit_b_text(const rt_hblk_t *h, char *tx, long cap)
{
    const unsigned char *b = (const unsigned char *)(h + 1); long n = (long)h->size - (long)sizeof(rt_hblk_t), j = 0;
    for (; j < cap - 1 && j < n; j++) tx[j] = (b[j] >= 32 && b[j] < 127) ? (char)b[j] : '.';
    tx[j] = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long gc_audit_b_words(const gc_audit_b_t *v, const char *lo, const char *hi, const char *pop, const char *nm, gc_audit_b_ctr_t *c, long cap)
{
    const char *p = (const char *)(((uintptr_t)lo + 7u) & ~(uintptr_t)7u); long nw = 0;
    for (; p + 8 <= hi; p += 8) {
        const char *w = *(const char *const *)p; const rt_hblk_t *h; const char *ex;
        c->words++; nw++;
        if (w < v->alo || w >= v->ahi) continue;
        ex = gc_audit_b_excluded(v, p);
        if (ex) { c->excl++; continue; }
        h = v->blk_of(w);
        if (!h) continue;
        c->inheap++;
        if (h->type == HB_FILL) { c->fill++; continue; }
        if (h->flags & HBF_MARK) { c->marked++; continue; }
        c->found++;
        if (c->shown >= cap) { c->suppressed++; continue; }
        c->shown++;
        { char tx[25]; char bb[512]; char hn[512]; Dl_info di; bb[0] = 0; gc_audit_b_text(h, tx, (long)sizeof tx);
          if (v->birth_of) v->birth_of((const char *)h, bb, (long)sizeof bb);
          if (dladdr((void *)p, &di) && di.dli_fbase) snprintf(hn, sizeof hn, "%s+0x%lx/%s", di.dli_fname ? di.dli_fname : "?", (unsigned long)((const char *)p - (const char *)di.dli_fbase), di.dli_sname ? di.dli_sname : (nm ? nm : "-"));
          else snprintf(hn, sizeof hn, "%s", nm ? nm : "-");
          fprintf(stderr, "[GC-AUDIT-B] run=%ld pop=%s CANDIDATE-LOST-ROOT at=%p in=%s word=%p blk=%p type=%u size=%u off=%ld text=%s%s%s\n",
              v->run, pop, (const void *)p, hn, (const void *)w, (const void *)h, (unsigned)h->type, (unsigned)h->size, (long)(w - (const char *)h), tx, bb[0] ? " " : "", bb); } }
    return nw;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long gc_audit_b_collect(const gc_audit_b_t *v)
{
    gc_audit_b_ctr_t c; long i, cap, nop = 0, nhw = 0; const char *e = getenv("SCRIP_GC_AUDIT_B"); const char *ps = getenv("SCRIP_GC_AUDIT_B_POPS");
    if (!e || !*e || *e == '0') return -1;
    if (!ps || !*ps) ps = "static,stack,heapint";
    memset(&c, 0, sizeof c);
    cap = atol(e); if (cap <= 1) cap = 64; if (cap > 100000) cap = 100000;
    if (v->run <= 1) fprintf(stderr, "[GC-AUDIT-B] PASS B IS AN AUDITOR AND NEVER A COLLECTOR: pass A (the exact typed walk) has already decided this collection and pass B cannot mark, forward, free or influence it -- it is handed a const block lookup and nothing else. IT NAMES CANDIDATES, NEVER DEFECTS: a dead word that looks like a heap pointer reads identically to a live root, so every line below is a CANDIDATE to be cured or declared. WHAT IT CANNOT SEE, DECLARED IN THREE PARTS BECAUSE THE THIRD IS THE ONE THAT BITES: (1) a pointer that at the instant of collection lives only in a REGISTER; (2) the C stack above the emitted-stack ceiling SCRIP_GC_CEILING draws (98.4%% of mode 3's stack, dead compiler frames by measurement, CEO-1030) -- set SCRIP_GC_CEILING=0 and pass B scans that too; and (3) ⛔ ANY REGION PASS A DOES NOT ENUMERATE, because pass B's territory IS pass A's territory: the libc malloc heap, the compile-time arena and the collector's own mmap'd bookkeeping are NOT scanned, so THIS AUDITOR FINDS AN UNVISITED WORD INSIDE A VISITED REGION AND CANNOT FIND AN UNVISITED REGION. A findings=0 therefore bounds the probe and never clears the collector. SUCCESS IS SILENCE, and the findings=0 line is PRINTED so its silence is a statement rather than an absence.\n");
    for (i = 0; i < v->nrgn; i++) { long nw; if (!strstr(ps, v->rgn[i].pop)) continue; nw = gc_audit_b_words(v, v->rgn[i].lo, v->rgn[i].hi, v->rgn[i].pop, v->rgn[i].name, &c, cap);
        if (v->run <= 1) fprintf(stderr, "[GC-AUDIT-B] region %ld pop=%s name=%s lo=%p hi=%p bytes=%ld words=%ld\n", i, v->rgn[i].pop, v->rgn[i].name, (const void *)v->rgn[i].lo, (const void *)v->rgn[i].hi, (long)(v->rgn[i].hi - v->rgn[i].lo), nw); }
    for (i = 0; strstr(ps, "heapint") && i < v->nblk; i++) { const rt_hblk_t *h = v->blk_at(i);
        if (!h || !(h->flags & HBF_MARK) || !gc_audit_b_opaque(h->type)) continue;
        nop++; nhw += gc_audit_b_words(v, (const char *)(h + 1), (const char *)h + h->size, "heapint", "-", &c, cap); }
    fprintf(stderr, "[GC-AUDIT-B] run=%ld audited=1 findings=%ld shown=%ld suppressed=%ld words=%ld inheap=%ld marked=%ld fill=%ld excluded=%ld regions=%ld heapint_words=%ld opaque_blocks=%ld of %ld opaque_set=ZCOL,ZBLK,WSC,AGGB,WSB pops=%s\n",
        v->run, c.found, c.shown, c.suppressed, c.words, c.inheap, c.marked, c.fill, c.excl, v->nrgn, nhw, nop, v->nblk, ps);
    return c.found;
}
#endif
