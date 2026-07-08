#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <gc.h>
#include "zeta_choices.h"
#include "zeta_alloc.h"
#define ZLS_HDR 16L
#define ZLS_ROOT_CHUNK (8L * 1024 * 1024)
static char *g_zls_arena = (char *)0;
static char *g_zls_top = (char *)0;
static char *g_zls_end = (char *)0;
static char *g_zls_hiwater = (char *)0;
static char *g_zls_rooted = (char *)0;
static void *g_zls_cur = (void *)0;
static long  g_zls_allocs = 0;
static long  g_zls_releases = 0;
static int   g_zls_report_reg = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_zls_report(void)
{
    if (!getenv("SCRIP_ZETA_TELEM")) return;
    fprintf(stderr, "[ZLS] ZC_ALLOC=%d ZC_INIT=%d ZC_POISON=%d arena=%dMB hiwater=%ldB allocs=%ld releases=%ld live=%ld\n", (int)ZC_ALLOC, (int)ZC_INIT, (int)ZC_POISON, (int)ZC_ARENA_MB,
            g_zls_arena ? (long)(g_zls_hiwater - g_zls_arena) : 0L, g_zls_allocs, g_zls_releases, g_zls_allocs - g_zls_releases);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_zls_arena_init(void)
{
    long mb = (long)ZC_ARENA_MB;
    g_zls_arena = (char *)mmap((void *)0, (size_t)mb << 20, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if (g_zls_arena == MAP_FAILED) { fprintf(stderr, "[ZLS] zeta arena mmap failed (%ld MB) — lower ZC_ARENA_MB\n", mb); abort(); }
    g_zls_top = g_zls_arena; g_zls_end = g_zls_arena + ((size_t)mb << 20); g_zls_hiwater = g_zls_arena; g_zls_rooted = g_zls_arena;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls_alloc(long bytes)
{
    long sz = (bytes + 15L) & ~15L;
    char *base;
    extern int g_gc_pending; extern long rt_gc_collect(void);
    if (g_gc_pending) { g_gc_pending = 0; rt_gc_collect(); }
    if (!g_zls_report_reg) { g_zls_report_reg = 1; atexit(rt_zls_report); }
#if ZC_ALLOC == ZC_ALLOC_MALLOC
    base = (char *)malloc((size_t)(ZLS_HDR + sz));
    if (!base) { fprintf(stderr, "[ZLS] malloc-mode activation alloc failed (%ld bytes)\n", ZLS_HDR + sz); abort(); }
    GC_add_roots(base, base + ZLS_HDR + sz);
#else
    if (!g_zls_arena) rt_zls_arena_init();
    base = g_zls_top;
    if (base + ZLS_HDR + sz > g_zls_end) { fprintf(stderr, "[ZLS] zeta arena exhausted (%d MB reserve, hiwater %ldB) — raise ZC_ARENA_MB\n", (int)ZC_ARENA_MB, (long)(g_zls_hiwater - g_zls_arena)); abort(); }
    g_zls_top = base + ZLS_HDR + sz;
    if (g_zls_top > g_zls_hiwater) {
        g_zls_hiwater = g_zls_top;
        while (g_zls_rooted < g_zls_hiwater) { char *nx = g_zls_rooted + ZLS_ROOT_CHUNK; if (nx > g_zls_end) nx = g_zls_end; GC_add_roots(g_zls_rooted, nx); g_zls_rooted = nx; }
    }
#endif
#if ZC_INIT == ZC_INIT_ZERO
    memset(base, 0, (size_t)(ZLS_HDR + sz));
#elif ZC_POISON == ZC_POISON_FILL
    memset(base, 0xAA, (size_t)(ZLS_HDR + sz));
#endif
    ((void **)base)[0] = g_zls_cur;
    ((long *)base)[1] = sz;
    g_zls_cur = (void *)(base + ZLS_HDR);
    g_zls_allocs += 1;
    return (void *)(base + ZLS_HDR);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls_frames_head(void) { return g_zls_cur; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls_frame_prev(void *fb) { return fb ? ((void **)((char *)fb - ZLS_HDR))[0] : (void *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_zls_frame_size(void *fb) { return fb ? ((long *)((char *)fb - ZLS_HDR))[1] : 0L; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls_release(void *fb)
{
    char *base;
    if (!fb) return;
    if (getenv("SCRIP_ZLS_RELEASE_TRACE")) fprintf(stderr, "[ZLS-RELEASE] fb=%p (call #%ld)\n", fb, g_zls_releases + 1);
    base = (char *)fb - ZLS_HDR;
    g_zls_releases += 1;
    g_zls_cur = ((void **)base)[0];
#if ZC_ALLOC == ZC_ALLOC_MALLOC
    { long sz = ((long *)base)[1]; GC_remove_roots(base, base + ZLS_HDR + sz); }
    free(base);
#elif ZC_ALLOC == ZC_ALLOC_BUMP_LIFO
    if (base >= g_zls_arena && base < g_zls_top) {
#if ZC_POISON == ZC_POISON_FILL
        memset(base, 0xDD, (size_t)(g_zls_top - base));
#endif
        g_zls_top = base;
    }
#endif
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB-OWNED-ζ GRAPH-EXIT STEP (Lon directive, this session).  rt_zls_mark/rt_zls_release_to — the graph-scale
 * counterpart of the construct-scale alloc/free pair ARBNO already proves (role 0's α allocs, role 2's ω
 * frees, for ONE construct).  Here: rt_zls_mark() is called once at a compiled graph's own prologue (before
 * any BB inside that graph has run) and simply snapshots g_zls_cur — the LIFO chain's current top — as "the
 * mark below which nothing belongs to this graph."  rt_zls_release_to(mark) is called once at that SAME
 * graph's true success exit (the γ-landing in xa_flat_epilogue_str, immediately before the function returns)
 * and walks the LIFO chain from whatever g_zls_cur is NOW back down to mark, releasing each block on the way
 * via the exact same per-block logic rt_zls_release already uses — this is not a new allocator, it is a bulk
 * caller of the existing one-block-at-a-time release, stopping at the boundary instead of going all the way
 * to NULL.  This walks EVERY block any BB in the graph allocated between the mark and now, regardless of
 * which construct made it or whether that construct's OWN local free already ran — a block already freed by
 * its own construct is no longer reachable from g_zls_cur (rt_zls_release already spliced it out), so it is
 * never visited twice; a block whose construct never reached its own local free (e.g. an ARBNO activation
 * still mid-iteration when a LATER part of the pattern succeeds and the whole graph returns) is still on the
 * chain and DOES get caught here — this is precisely the backstop case the per-construct rule alone misses.
 * The mark itself must be held in the COMPILED CODE's own per-activation storage (a stack slot in the
 * graph's frame), never a global: graphs call graphs (procedure calls nest), so a single global "current
 * mark" would be clobbered by an inner call before the outer call's epilogue reads it back — the same
 * single-carrier trap rt_zls_arbno_step1_store/load below is honestly scoped to avoid, at the LARGER scale
 * where it would corrupt the outer caller's own graph, not just lose one activation's pointer. */
void *rt_zls_mark(void) { if (getenv("SCRIP_ZLS_RELEASE_TRACE")) fprintf(stderr, "[ZLS-MARK] returning g_zls_cur=%p\n", g_zls_cur); return g_zls_cur; }
void rt_zls_release_to(void *mark)
{
    if (getenv("SCRIP_ZLS_RELEASE_TRACE")) fprintf(stderr, "[ZLS-RELEASE-TO] called with mark=%p, g_zls_cur=%p\n", mark, g_zls_cur);
    while (g_zls_cur != mark) {
        void *cur = g_zls_cur;
        if (!cur) break;
        rt_zls_release(cur);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB-OWNED-ζ STEP 1 (Lon pivot, this session).  rt_zls_arbno_step1_store/load — a SINGLE static carrier for
 * the pointer role 0's fresh alloc returns, read back by role 2 right before its own true-exit free.
 * DELIBERATELY not slot-packed (role 0's own zls_field grant is exactly 16B, "3x4B + pad" — 4 bytes of pad,
 * not 8, so an 8-byte pointer does not fit there; widening the grant would require touching the whole-
 * program slot-layout return-value-times-16 arithmetic in zeta_storage.c, disproportionate risk for a first
 * proof-of-wiring slice).  HONEST SCOPE: this is correct ONLY for SEQUENTIAL re-entry (one activation live at
 * a time — exactly arbno_reentrant.sno's shape, and the ALREADY-PASSING case per this session's earlier
 * testing). It does NOT handle nested or concurrent activations of the same ARBNO node (a second alloc before
 * the first's matching free would silently overwrite the carrier, losing the first pointer — the general
 * fix needs a real per-activation carrier, e.g. a correctly-widened slot or an explicit stack, and is
 * explicitly future work, not solved here). */
static void *g_zls_arbno_step1_carrier = (void *)0;
void rt_zls_arbno_step1_store(void *p) { if (getenv("SCRIP_ARBNO_STEP1_TRACE")) fprintf(stderr, "[ARBNO-S1] STORE %p (prev carrier was %p)\n", p, g_zls_arbno_step1_carrier); g_zls_arbno_step1_carrier = p; }
void *rt_zls_arbno_step1_load(void) { if (getenv("SCRIP_ARBNO_STEP1_TRACE")) fprintf(stderr, "[ARBNO-S1] LOAD  %p\n", g_zls_arbno_step1_carrier); return g_zls_arbno_step1_carrier; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZLS2 ARENA (Lon directive 2026-07-08) — the DOWN-GROWING bare-bump ζ arena.  No headers, no per-alloc
 * runtime call, no per-alloc GC-root chunking: emitted code does `sub r12, K` at α and `add r12, K` at the
 * box's single true-exit ω, K an emit-time constant of the box's OWN frame (down-growth is what makes both
 * constants the box's own — up-growth would need the runtime predecessor's size).  MAP_NORESERVE reserves
 * ZC_ZLS2_MB of contiguous virtual space that commits page-by-page on touch — the mmap stand-in for the C
 * stack's kernel-grown virtue; the discipline itself is backing-agnostic and moves to rsp-on-the-C-stack
 * when the direct-jmp call convention retires the proc trampoline (no C frame may then interleave above a
 * live BB frame — Lon's caveat, made true by construction).  GC visibility: the WHOLE reserve is rooted
 * once at init; libgc scans only committed pages' contents lazily via its dirty logic, and untouched
 * NORESERVE pages read as zero — acceptable for bring-up, revisit if root-scan cost shows in telemetry. */
static char *g_zls2_lo = (char *)0;
static char *g_zls2_hi = (char *)0;
void *rt_zls2_init(void)
{
    if (!g_zls2_hi) {
        long mb = (long)ZC_ZLS2_MB;
        g_zls2_lo = (char *)mmap((void *)0, (size_t)mb << 20, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
        if (g_zls2_lo == MAP_FAILED) { fprintf(stderr, "[ZLS2] arena mmap failed (%ld MB) — lower ZC_ZLS2_MB\n", mb); abort(); }
        g_zls2_hi = g_zls2_lo + ((size_t)mb << 20);
        GC_add_roots(g_zls2_lo, g_zls2_hi);
    }
    return (void *)g_zls2_hi;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls2_lo(void) { return (void *)g_zls2_lo; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_zls2_push/rt_zls2_pop (Claude, 2026-07-08 continuation session) — the ZLS2 arena's FIRST-CONSUMER bump
 * pair, the C-helper form of the design's own-constant discipline: push = cursor -= K (down-growth), pop =
 * cursor += K, K always the CALLER's own emit-time frame constant, so a pop needs no size lookup, no header,
 * no walk — exactly the `sub r12,K` / `add r12,K` end state, held in a memory cursor instead of r12 because
 * r12 is still the graph frame base today (repointing it breaks every sibling [r12+off] reference — the
 * documented wall in GOAL-SNOBOL4-BB.md).  The pair migrates INTO emitted code (register cursor) when the
 * proc trampoline retires and rsp becomes the frame pointer; until then this is the bare bump with a bounds
 * check.  NO zeroing (the ZLS2 contract: a consumer writes every field it reads before reading it), NO
 * per-block header, NO GC-root chunking (whole reserve rooted once at init, see rt_zls2_init above).
 * LIFO enforcement: pop past hi aborts loudly; push past lo aborts loudly (exhaustion).  SCRIP_ZLS2_TRACE=1
 * prints every push/pop with the cursor so alloc/free balance is auditable event-by-event. */
char *g_zls2_cur = (char *)0;
void *rt_zls2_push(long k)
{
    if (!g_zls2_cur) { rt_zls2_init(); g_zls2_cur = g_zls2_hi; }
    if (g_zls2_cur - k < g_zls2_lo) { fprintf(stderr, "[ZLS2] arena exhausted on push(%ld) — raise ZC_ZLS2_MB\n", k); abort(); }
    g_zls2_cur -= k;
    if (getenv("SCRIP_ZLS2_TRACE")) fprintf(stderr, "[ZLS2] PUSH %ld -> cur=%p (used=%ld)\n", k, (void *)g_zls2_cur, (long)(g_zls2_hi - g_zls2_cur));
    return (void *)g_zls2_cur;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_zls2_pop/rt_zls2_release_to poison (Claude Sonnet 5, 2026-07-08 session 2) — the ZLS2 proving lever, the
 * arena's ASan-equivalent-lite: under ZC_POISON_FILL (the compiled default) or SCRIP_ZLS2_POISON=1, every
 * reclaimed byte is filled 0xDD, so a consumer reading a popped/released block gets loud garbage instead of
 * silently-still-valid stale data.  Rationale from this session's own regression: the word2 bug survived TWO
 * full crosschecks precisely because reclaimed-then-reallocated arena memory happened to hold plausible
 * values; poison converts that silence into immediate, visible divergence.  Gated OFF by env
 * SCRIP_ZLS2_POISON=0 for perf runs; the pop-side bounds/LIFO aborts are unconditional either way. */
static int rt_zls2_poison(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_ZLS2_POISON"); p = e ? (atoi(e) != 0) : (ZC_POISON == ZC_POISON_FILL); } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls2_pop(long k)
{
    if (!g_zls2_cur || g_zls2_cur + k > g_zls2_hi) { fprintf(stderr, "[ZLS2] pop(%ld) past arena top — unbalanced push/pop (LIFO discipline violated)\n", k); abort(); }
    if (rt_zls2_poison()) memset(g_zls2_cur, 0xDD, (size_t)k);
    g_zls2_cur += k;
    if (getenv("SCRIP_ZLS2_TRACE")) fprintf(stderr, "[ZLS2] POP  %ld -> cur=%p (used=%ld)\n", k, (void *)g_zls2_cur, (long)(g_zls2_hi - g_zls2_cur));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_zls2_mark/rt_zls2_release_to — the ZLS2 twins of rt_zls_mark/rt_zls_release_to above, same statement-
 * scope backstop role: IR_MATCH_HEAD marks the cursor once at match entry; the ONE exit the statement
 * actually takes (head's own ω-choke on failure, IR_MATCH_RELEASE's α on success) releases back to the mark.
 * This is what reclaims a v1 ARBNO activation whose match SUCCEEDED and therefore left via the pattern's
 * success join without ever reaching its own role-2 pop — the success-path leak, closed the same way the
 * prior session closed it for the up-growing arena.  Release-to is idempotent with role 2's own eager pop
 * (an already-popped cursor equals the mark; setting it to the mark is then a no-op), and releasing to a
 * mark BELOW the cursor is impossible under LIFO — enforced loudly. */
void *rt_zls2_mark(void)
{
    if (!g_zls2_cur) { rt_zls2_init(); g_zls2_cur = g_zls2_hi; }
    if (getenv("SCRIP_ZLS2_TRACE")) fprintf(stderr, "[ZLS2] MARK cur=%p\n", (void *)g_zls2_cur);
    return (void *)g_zls2_cur;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls2_release_to(void *mark)
{
    char *m = (char *)mark;
    if (!m) return;
    if (m < g_zls2_cur) { fprintf(stderr, "[ZLS2] release_to(%p) BELOW cursor %p — LIFO discipline violated\n", mark, (void *)g_zls2_cur); abort(); }
    if (m > g_zls2_hi)  { fprintf(stderr, "[ZLS2] release_to(%p) past arena top %p\n", mark, (void *)g_zls2_hi); abort(); }
    if (getenv("SCRIP_ZLS2_TRACE") && m != g_zls2_cur) fprintf(stderr, "[ZLS2] RELEASE_TO %p (reclaimed %ld)\n", mark, (long)(m - g_zls2_cur));
    if (m != g_zls2_cur && rt_zls2_poison()) memset(g_zls2_cur, 0xDD, (size_t)(m - g_zls2_cur));
    g_zls2_cur = m;
}
