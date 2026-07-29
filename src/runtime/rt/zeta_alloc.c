#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "zeta_choices.h"
#include "rt_slab.h"
#include "rt_arena.h"
#include "zeta_alloc.h"
#define ZLS_HDR 16L
#define ZLS_ZB_BYTES ((size_t)ZC_ZBLOCK_KB << 10)
#define ZBF_WS 1L
static rt_arena_t g_zls_zb;
static int g_zls_zb_up = 0;
static void *g_zls_cur = (void *)0;
static long  g_zls_allocs = 0;
static long  g_zls_releases = 0;
static long  g_zls_nonhead = 0;
static long  g_zls_bytes = 0;
static int   g_zls_report_reg = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_zls_report(void)
{
    long depth = 0; void *it = g_zls_cur; while (it) { depth += 1; it = ((void **)((char *)it - ZLS_HDR))[0]; }
    if (!getenv("SCRIP_ZETA_TELEM") && !getenv("SCRIP_ZLS_LIFO_PROBE")) return;
    fprintf(stderr, "[ZLS] chain_depth=%ld live=%ld %s\n", depth, g_zls_allocs - g_zls_releases, depth == (g_zls_allocs - g_zls_releases) ? "COHERENT" : "ORPHANED");
    fprintf(stderr, "[ZLS] ZC_INIT=%d ZC_POISON=%d zblock=%ldKB allocs=%ld releases=%ld live=%ld nonhead=%ld bytes=%ld\n", (int)ZC_INIT, (int)ZC_POISON,
            (long)ZC_ZBLOCK_KB, g_zls_allocs, g_zls_releases, g_zls_allocs - g_zls_releases, g_zls_nonhead, g_zls_bytes);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZA-FLIP s67 (GC-U-2, Lon's s36 destination): the long-lived-ζ provider is rt_arena_zblock_get/put — FIXED ZC_ZBLOCK_KB blocks recycled through the A_COEXPR free list — replacing per-block
 * GC_MALLOC/GC_FREE.  The zblock list is UNIFORM-SIZE by contract (rt_arena_zblock_get aborts on mixed sizes — this is the s39 fork made concrete), so requests larger than one ζ block route to the
 * grow-only WORKSPACE (rt_ws_alloc) with ZBF_WS flagged in the header's size word (sizes are 16-aligned, low bits free); release leaves WS blocks in place (immortal until GC-W-2 collects the
 * workspace — the TR-4-sanctioned transitional state).  The prev/size chain, zero/poison semantics, counters and the g_gc_pending collector trigger are VERBATIM; rooting compensation is inherited,
 * not rebuilt: both providers are rt_slab-backed and RT_SLAB_GC_ROOTS (s37) roots every slab until TR-4 deletes libgc.  This file is now libgc-FREE — the rung's stated TR-4 unblock. */
void *rt_zls_alloc(long bytes)
{
    long sz = (bytes + 15L) & ~15L;
    long flag = 0L;
    char *base;
    extern int g_gc_pending; extern long rt_gc_collect(void);
    if (g_gc_pending) { g_gc_pending = 0; rt_gc_collect(); }
    if (!g_zls_report_reg) { g_zls_report_reg = 1; atexit(rt_zls_report); }
    if ((size_t)(ZLS_HDR + sz) <= ZLS_ZB_BYTES) {
        if (!g_zls_zb_up) { rt_arena_init(&g_zls_zb, A_COEXPR); g_zls_zb_up = 1; }
        base = (char *)rt_arena_zblock_get(&g_zls_zb, ZLS_ZB_BYTES);
    } else {
        base = (char *)rt_ws_alloc((size_t)(ZLS_HDR + sz));
        flag = ZBF_WS;
    }
    if (!base) { fprintf(stderr, "[ZLS] activation alloc failed (%ld bytes)\n", ZLS_HDR + sz); abort(); }
#if ZC_INIT == ZC_INIT_ZERO
    memset(base, 0, (size_t)(ZLS_HDR + sz));
#elif ZC_POISON == ZC_POISON_FILL
    memset(base, 0xAA, (size_t)(ZLS_HDR + sz));
#endif
    ((void **)base)[0] = g_zls_cur;
    ((long *)base)[1] = sz | flag;
    g_zls_cur = (void *)(base + ZLS_HDR);
    g_zls_allocs += 1;
    g_zls_bytes += (long)(ZLS_HDR + sz);
    return (void *)(base + ZLS_HDR);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls_frames_head(void) { return g_zls_cur; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_zls_frame_prev(void *fb) { return fb ? ((void **)((char *)fb - ZLS_HDR))[0] : (void *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_zls_frame_size(void *fb) { return fb ? (((long *)((char *)fb - ZLS_HDR))[1] & ~15L) : 0L; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_zls_poison(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_ZLS_POISON"); p = e ? (atoi(e) != 0) : (ZC_POISON == ZC_POISON_FILL); } return p; }
static int rt_zls_reltrace(void) { static int p = -1; if (p < 0) p = getenv("SCRIP_ZLS_RELEASE_TRACE") != NULL; return p; }
static int rt_zls_arbtrace(void) { static int p = -1; if (p < 0) p = getenv("SCRIP_ARBNO_STEP1_TRACE") != NULL; return p; }
static int rt_zls2_tron(void) { static int p = -1; if (p < 0) p = getenv("SCRIP_ZLS2_TRACE") != NULL; return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* CHAIN INTEGRITY (2026-07-12, MEASURED not reasoned). This function assumed fb is ALWAYS the chain head and spliced with an unconditional g_zls_cur = prev(fb). That assumption is FALSE: SCRIP_ZLS_LIFO_PROBE over the
 * whole corpus reports SNOBOL4 crosscheck 517/517 and Icon 20486/20488 perfectly LIFO (nonhead=0), but Prolog meta_qsort.pl issues 122 NON-HEAD releases. A non-head splice ORPHANS every block between the old head and fb:
 * they leave the chain while still live. Benign TODAY only because libgc scans conservatively and never consults this chain -- but GC-W-1 makes this chain THE ROOT SET, at which point an orphaned-but-live frame is invisible
 * to the mark phase and gets collected out from under a running Prolog generator (a silent, stress-only corruption in the very mechanism the collector stands on). Fixed by unlinking properly: the head release keeps its O(1)
 * fast path (the 100% case for SNOBOL4/Icon); a non-head release walks to fb's successor and repoints that successor's prev-link. A block not found in the chain (double release / foreign pointer) leaves the chain untouched
 * and does NOT free, which leaks one block rather than corrupting the root set or double-freeing. The BUMP_LIFO cursor pop is tightened the same way: it now fires only when the released block is genuinely the arena top --
 * the old `base < g_zls_top` test would have truncated the arena UNDER LIVE BLOCKS on exactly meta_qsort's 122 non-head releases; a non-top release now leaves a hole instead (grow-only), which is safe. */
void rt_zls_release(void *fb)
{
    char *base;
    if (!fb) return;
    base = (char *)fb - ZLS_HDR;
    if (rt_zls_reltrace()) fprintf(stderr, "[ZLS-RELEASE] fb=%p sz=%ld fn=%p (call #%ld)\n", fb, ((long *)base)[1], ((void **)fb)[0], g_zls_releases + 1);
    g_zls_releases += 1;
    if (fb == g_zls_cur) { g_zls_cur = ((void **)base)[0]; }
    else { void *it = g_zls_cur; g_zls_nonhead += 1; while (it && rt_zls_frame_prev(it) != fb) it = rt_zls_frame_prev(it); if (!it) return; ((void **)((char *)it - ZLS_HDR))[0] = ((void **)base)[0]; }
    /* ZA-FLIP s67: unlink verbatim above; reclaim below.  A ζ block goes back on the A_COEXPR reuse list (poison-then-put is safe: rt_arena_zblock_get re-zeroes reuse under RT_ARENA_ZERO and the
     * alloc's ZC_INIT memset covers HDR+sz, so quarantine-by-leak — the old MALLOC arm's poison behavior — is retired).  A ZBF_WS oversize block stays where it is (workspace is grow-only until
     * GC-W-2); poison still fires on it so a stale read is loud. */
    { long rawsz = ((long *)base)[1]; long psz = rawsz & ~15L;
      if (rt_zls_poison()) memset(base, 0xDD, (size_t)(ZLS_HDR + psz));
      if (!(rawsz & ZBF_WS)) rt_arena_zblock_put(&g_zls_zb, base); }
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
void *rt_zls_mark(void) { if (rt_zls_reltrace()) fprintf(stderr, "[ZLS-MARK] returning g_zls_cur=%p\n", g_zls_cur); return g_zls_cur; }
void rt_zls_release_to(void *mark)
{
    if (rt_zls_reltrace()) fprintf(stderr, "[ZLS-RELEASE-TO] called with mark=%p, g_zls_cur=%p\n", mark, g_zls_cur);
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
void rt_zls_arbno_step1_store(void *p) { if (rt_zls_arbtrace()) fprintf(stderr, "[ARBNO-S1] STORE %p (prev carrier was %p)\n", p, g_zls_arbno_step1_carrier); g_zls_arbno_step1_carrier = p; }
void *rt_zls_arbno_step1_load(void) { if (rt_zls_arbtrace()) fprintf(stderr, "[ARBNO-S1] LOAD  %p\n", g_zls_arbno_step1_carrier); return g_zls_arbno_step1_carrier; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZLS2 ARENA (Lon directive 2026-07-08) — the DOWN-GROWING bare-bump ζ arena.  No headers, no per-alloc
 * runtime call, no per-alloc GC-root chunking: emitted code does `sub r12, K` at α and `add r12, K` at the
 * box's single true-exit ω, K an emit-time constant of the box's OWN frame (down-growth is what makes both
 * constants the box's own — up-growth would need the runtime predecessor's size).  MAP_NORESERVE reserves
 * ZC_ZLS2_MB of contiguous virtual space that commits page-by-page on touch — the mmap stand-in for the C
 * stack's kernel-grown virtue; the discipline itself is backing-agnostic and moves to rsp-on-the-C-stack
 * when the direct-jmp call convention retires the proc trampoline (no C frame may then interleave above a
 * live BB frame — Lon's caveat, made true by construction).  GC visibility (ZA-FLIP s67): the slab backing
 * is rooted by RT_SLAB_GC_ROOTS (s37) — the per-arena GC_add_roots this file used to do is deleted. */
static char *g_zls2_lo = (char *)0;
static char *g_zls2_hi = (char *)0;
void *rt_zls2_init(void)
{
    if (!g_zls2_hi) {
        long mb = (long)ZC_ZLS2_MB;
        /* TR-2: slab-pool backing (was private mmap). ZLS2 grows DOWN from hi, so the
         * region must be contiguous — rt_slab_region guarantees exactly that. */
        g_zls2_lo = (char *)rt_slab_region((size_t)mb << 20);
        if (!g_zls2_lo) { fprintf(stderr, "[ZLS2] arena slab failed (%ld MB) — lower ZC_ZLS2_MB\n", mb); abort(); }
        g_zls2_hi = g_zls2_lo + ((size_t)mb << 20);
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
    if (rt_zls2_tron()) fprintf(stderr, "[ZLS2] PUSH %ld -> cur=%p (used=%ld)\n", k, (void *)g_zls2_cur, (long)(g_zls2_hi - g_zls2_cur));
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
    if (rt_zls2_tron()) fprintf(stderr, "[ZLS2] POP  %ld -> cur=%p (used=%ld)\n", k, (void *)g_zls2_cur, (long)(g_zls2_hi - g_zls2_cur));
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
    if (rt_zls2_tron()) fprintf(stderr, "[ZLS2] MARK cur=%p\n", (void *)g_zls2_cur);
    return (void *)g_zls2_cur;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_zls2_release_to(void *mark)
{
    char *m = (char *)mark;
    if (!m) return;
    if (m < g_zls2_cur) { fprintf(stderr, "[ZLS2] release_to(%p) BELOW cursor %p — LIFO discipline violated\n", mark, (void *)g_zls2_cur); abort(); }
    if (m > g_zls2_hi)  { fprintf(stderr, "[ZLS2] release_to(%p) past arena top %p\n", mark, (void *)g_zls2_hi); abort(); }
    if (rt_zls2_tron() && m != g_zls2_cur) fprintf(stderr, "[ZLS2] RELEASE_TO %p (reclaimed %ld)\n", mark, (long)(m - g_zls2_cur));
    if (m != g_zls2_cur && rt_zls2_poison()) memset(g_zls2_cur, 0xDD, (size_t)(m - g_zls2_cur));
    g_zls2_cur = m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZETA SUBSYSTEM SELECTOR (Lon directive 2026-07-09) — the mode the --zeta CLI flag selects.  ONE variable,
 * both worlds: scrip's driver sets it after argv parse (covers mode 3 and all emit-time x86_zeta_mode()
 * reads); mode 4 binaries re-set it at entry via the call xa_file_header BAKES when the flag overrode the
 * ZC_ZETA default (no flag = no bake = byte-identical output).  RUNG-0: nothing allocates by it yet. */
/* PL-SINK-8 (2026-07-25): linkage widened static -> extern (NOT a new global — the same cell, one storage class) so the emitted $trail_mark fast path can RIPSEAL-read the mode LIVE rather than trusting a
 * snapshot.  Live is the honest read: rt_zeta_set_mode is driver-only (--zeta= parse in m3, a baked preamble call in m4) and so always precedes the first goal, but reading the cell each time means the
 * inline cannot go stale even if that ever changes.  ZH(2) is the only mode with a live zh shadow stack, so ZH -> SLOW and the default ZLS2(1) inlines.  zeta_alloc.c is outside the policed PL_FILES set. */
int g_zeta_mode = (int)ZC_ZETA;
void rt_zeta_set_mode(int m) { g_zeta_mode = (m == ZC_ZETA_ZLS) ? ZC_ZETA_ZLS : (m == ZC_ZETA_ZH) ? ZC_ZETA_ZH : ZC_ZETA_ZLS2; if (getenv("SCRIP_ZETA_TELEM")) fprintf(stderr, "[ZETA] mode=%s\n", g_zeta_mode == ZC_ZETA_ZLS ? "zls" : g_zeta_mode == ZC_ZETA_ZH ? "zh" : "zls2"); }
int  rt_zeta_mode(void) { return g_zeta_mode; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rt_zeta_cstack (ZC_PORT_CSTACK rung, Lon 2026-07-09) — the ONE runtime-side reader of the C-stack zeta
 * choice, the twin of x86_asm.h's emit-time x86_port_mode(): when the port flavor is CSTACK, the C-side
 * exact-bracket frames (rt_call_proc_descr / rt_call_named_proc / rt_call_proc_direct / rt_call_named_proc_sl
 * in rt.c, run_code_chain's fragment frame in runtime_eval.c) allocate with alloca on the caller's own C
 * activation instead of the ZLS2 arena — identical lifetime by construction (the bracket IS the C call),
 * conservatively GC-scanned for free, and the fragment frame's recorded 64KB-per-crossing leak closes.
 * Emit-side and C-side disciplines are independent, so an env/default mismatch is safe, just unmixed.
 * Survive-return frames (rt_proc_call_gen* suspended activations — the s6 long-lived class) never consult
 * this and stay on their heap provider. */
/* ZETA PORT SELECTOR (Lon directive 2026-07-10: switch back and forth between the C-stack mode and the old
 * arena mode at runtime for testing) — the ZC_PORT axis twin of rt_zeta_set_mode/rt_zeta_mode above, the mode
 * the --zeta-port CLI flag selects.  ONE variable, both worlds: scrip's driver sets it after argv parse
 * (covers mode 3 and every emit-time x86_port_mode() read); mode 4 binaries re-set it at entry via the call
 * the main wrapper BAKES when the resolved mode differs from the ZC_PORT build default (no override = no bake
 * = byte-identical output).  UNSET (-1) resolves lazily: SCRIP_ZETA_STORAGE FIRST (Z4-5 — the selector is the
 * single authority; its setter derives this axis, and a frame-r12 selection that derives nothing falls
 * through), then the pre-existing SCRIP_ZETA_PORT env, else ZC_PORT —
 * precedence flag > env > default.  The bake also closes the M4 coherence hazard the env alone carried: a .s
 * emitted under one port mode is mode-COMMITTED (CSTACK emits rsp arithmetic, INLINE emits arena-cursor
 * arithmetic), so its runtime side (rt_zeta_cstack's alloca-vs-arena proc frames) must self-select the SAME
 * mode regardless of the executing shell's env. */
static int g_zeta_port = -1;
void rt_zeta_port_set_mode(int m) { g_zeta_port = (m >= ZC_PORT_PLAIN && m <= ZC_PORT_HEAP) ? m : (int)ZC_PORT; if (getenv("SCRIP_ZETA_TELEM")) fprintf(stderr, "[ZETA] port=%d\n", g_zeta_port); }
int  rt_zeta_port_mode(void) { if (g_zeta_port < 0) { if (getenv("SCRIP_ZETA_STORAGE")) rt_zeta_storage_get(); if (g_zeta_port < 0) { const char *e = getenv("SCRIP_ZETA_PORT"); g_zeta_port = e ? atoi(e) : (int)ZC_PORT; } } return g_zeta_port; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZC_STORAGE SELECTOR (GOAL-ZETA-FOUR Z4-4 slice 2, Lon directive 2026-07-28) — the FOUR-config axis that subsumes the ZC_PORT ladder + ZC_FRAME + ZC_ZETA + the never-added SCRIP_ZMODE:
 * 0=frame-r12 1=frame-rsp 2=cell-stack 3=cell-heap.  Same one-variable/both-worlds shape as the port selector above: the driver sets it post-argv (m3 + every emit-time read), m4 binaries re-set it
 * via the bake scrip.c places beside the port bake (no override = no bake = byte-identical).  UNSET (-1) resolves lazily from SCRIP_ZETA_STORAGE (name or single digit), else ZC_STORAGE — precedence
 * flag > env > build.  CLAMPED OVER THE FULL 0..3 RANGE — the s206 lesson verbatim: rt_zeta_port_set_mode's stale <=FORTH clamp silently reset port 7 to the default, so a mode-4 heap binary ran
 * FORTH and every m3-vs-m4 A/B on the port was invalid; a clamp that lags its enum is a silent config lie.  THE SETTER DERIVES THE LEGACY TUPLE so zero downstream seams flip this rung:
 * cell-stack->PORT_FORTH, cell-heap->PORT_HEAP, frame-rsp->PORT_CSTACK (the R-B embodiment — ROTTED at HEAD until Z4-6; selecting it is honest, not yet safe); frame-r12 derives NOTHING (no legacy
 * twin survives at HEAD — Z4-7 gives the value its meaning).  SEAM CLOSED AT Z4-5: rt_zeta_port_mode's lazy branch now consults SCRIP_ZETA_STORAGE FIRST, so env-only storage selection reaches m3
 * through the port axis every emit-time read already takes; --zeta-port=forth/cstack/heap are ALIASES routed through THIS setter (scrip.c), making the selector the single authority for the three
 * mapped ports (the unmapped four stay direct until the Z4-9 cut).  THE LAZY DEFAULT SETS THE CELL BARE (no derive, no telem): measured s2, a deriving default STOMPED an explicit --zeta-port=heap back to forth through the bake-site getter — an absent axis must never overwrite a present one.  Legacy axes stay behaviorally authoritative until slices 3+; Z4-9 deletes them. */
static const char *g_zeta_storage_names[4] = { "frame-r12", "frame-rsp", "cell-stack", "cell-heap" };
static int g_zeta_storage = -1;
void rt_zeta_storage_set(int s) { g_zeta_storage = (s >= ZC_STORAGE_FRAME_R12 && s <= ZC_STORAGE_CELL_HEAP) ? s : (int)ZC_STORAGE; if (g_zeta_storage == ZC_STORAGE_CELL_STACK) rt_zeta_port_set_mode(ZC_PORT_FORTH); else if (g_zeta_storage == ZC_STORAGE_CELL_HEAP) rt_zeta_port_set_mode(ZC_PORT_HEAP); else if (g_zeta_storage == ZC_STORAGE_FRAME_RSP) rt_zeta_port_set_mode(ZC_PORT_CSTACK); if (getenv("SCRIP_ZETA_TELEM")) fprintf(stderr, "[ZETA] storage=%s\n", g_zeta_storage_names[g_zeta_storage]); }
int  rt_zeta_storage_get(void) { if (g_zeta_storage < 0) { const char *e = getenv("SCRIP_ZETA_STORAGE"); int s = (int)ZC_STORAGE; int i; if (e) { if (e[0] >= '0' && e[0] <= '3' && !e[1]) s = e[0] - '0'; else for (i = 0; i < 4; i++) if (strcmp(e, g_zeta_storage_names[i]) == 0) s = i; } if (e) rt_zeta_storage_set(s); else g_zeta_storage = (int)ZC_STORAGE; } return g_zeta_storage; }
int rt_zeta_cstack(void)
{
    return (rt_zeta_port_mode() == ZC_PORT_CSTACK || rt_zeta_port_mode() == ZC_PORT_FORTH) ? 1 : 0;
}
