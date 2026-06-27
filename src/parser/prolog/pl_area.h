#ifndef PL_AREA_H
#define PL_AREA_H
/*--------------------------------------------------------------------------------------------------------------------*/
/* PL-AREAS — a contiguous, mmap-backed, GC-INVISIBLE bump-pointer area.
 *
 * Three of these back the Prolog Heap / Trail / Environment areas (PL-AREAS-2/3/4); only each area's hot TOP moves
 * into a register (R13=H, R14=TR, R15=E), the cold base/limit stay in this struct, touched only on overflow/grow.
 *
 * WHY mmap, NOT a static slab (PL-AREAS-0 de-risk, 2026-06-27 #5): libgc conservatively scans static DATA roots on
 * every collection, so a large static array makes every GC pay for its whole size (a 256MB static slab took fib from
 * 13ms to 71ms; shrinking it to 4MB restored 14ms — the GC was scanning the slab). mmap'd memory is NOT a static root
 * and is NOT registered with libgc, so an mmap area is NEVER scanned. The area manages its OWN memory via mark/reset
 * (pop on deterministic return, restore on backtrack) — O(1), zero GC.
 *
 * WHY GC-invisibility is SAFE: area-resident terms point only to other area cells or to interned atoms (permanent);
 * nothing in the GC heap is solely area-reachable, so the GC never needs to trace into the area. A value that must
 * outlive its choice point (the final answer, an assertz clause, a findall result) is COPIED OUT to the GC heap on
 * the escape path (PL-AREAS-4) — that copy, not the area cell, is what the GC then owns.
 *
 * RECLAMATION is O(1): the choice box saves the area top at α (pl_area_mark) and restores it at β/retry
 * (pl_area_reset); a deterministic return pops the Environment top the same way. No per-object free, no GC sweep.
 *
 * GROWTH / POINTER STABILITY: the area is reserved LARGE up front (default below) — mmap reserves virtual address
 * space lazily (pages fault in on first touch), so a big reservation costs ~nothing until used, and the Heap area's
 * inter-term ABSOLUTE pointers never move in practice. pl_area_grow is the pathological fallback; it prefers an
 * IN-PLACE mremap (no move). A moving grow would invalidate every raw area pointer held across it — so the Heap area
 * must size its reservation to never need a moving grow (PL-AREAS-4 owns that sizing decision).
 */
#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>
#if defined(__linux__)
/* explicit prototype: mremap is a GNU extension; declaring it here makes pl_area_grow type-check regardless of
 * whether the including TU set _GNU_SOURCE before <sys/mman.h>. flags=0 means grow IN PLACE (never move) or fail. */
extern void *mremap(void *, size_t, size_t, int, ...);
#endif
/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct { char *base; char *top; char *limit; size_t cap; } pl_area_t;
/*--------------------------------------------------------------------------------------------------------------------*/
/* default reservation: 256 MiB of lazily-faulted virtual space per area (demand-paged, GC-invisible). */
#ifndef PL_AREA_DEFAULT_BYTES
#define PL_AREA_DEFAULT_BYTES ((size_t)256u * 1024u * 1024u)
#endif
/*--------------------------------------------------------------------------------------------------------------------*/
/* reserve `bytes` via anonymous mmap (rw, private); base==top, limit==base+bytes. mmap memory is GC-invisible. */
static inline void pl_area_init(pl_area_t *a, size_t bytes) {
    if (bytes < 4096u) bytes = 4096u;
    void *p = mmap((void *)0, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED) { a->base = a->top = a->limit = (char *)0; a->cap = 0; return; }
    a->base = (char *)p; a->top = (char *)p; a->limit = (char *)p + bytes; a->cap = bytes;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* grow to fit `need` more bytes IN PLACE (mremap flags=0 — never moves, so raw area pointers stay valid); returns 1
 * on success, 0 if an in-place grow is impossible. A 0 is a HARD area-overflow: the caller must enlarge the initial
 * reservation, never silently relocate (a moving Heap/Env area would corrupt every inter-frame/inter-term pointer). */
static inline int pl_area_grow(pl_area_t *a, size_t need) {
    if (!a->base) return 0;
    size_t used = (size_t)(a->top - a->base);
    size_t nc = a->cap ? a->cap : 4096u;
    while (nc - used < need) nc *= 2;
#if defined(__linux__)
    void *p = mremap(a->base, a->cap, nc, 0);
    if (p == MAP_FAILED) return 0;
    a->base = (char *)p; a->top = (char *)p + used; a->limit = (char *)p + nc; a->cap = nc; return 1;
#else
    (void)nc; return 0;
#endif
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* bump `bytes` (rounded up to 16) off the top; grow in place on overflow. Returns the block, or NULL on hard OOM. */
static inline void *pl_area_bump(pl_area_t *a, size_t bytes) {
    bytes = (bytes + 15u) & ~((size_t)15u);
    if (a->top + bytes > a->limit) { if (!pl_area_grow(a, bytes)) return (void *)0; }
    void *r = a->top; a->top += bytes; return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* mark / reset: the O(1) reclamation primitives (pop-on-return, restore-on-backtrack). */
static inline char *pl_area_mark(const pl_area_t *a) { return a->top; }
static inline void  pl_area_reset(pl_area_t *a, char *mark) { a->top = mark; }
/*--------------------------------------------------------------------------------------------------------------------*/
/* bytes currently in use (for instrumentation / overflow diagnostics). */
static inline size_t pl_area_used(const pl_area_t *a) { return (size_t)(a->top - a->base); }
/*--------------------------------------------------------------------------------------------------------------------*/
/* PL-AREAS-3 — the ENVIRONMENT-area ABI (the E region: R15=E in the ratified H/TR/E assignment). rt_enter bumps each
 * callee frame off this region; a choice box saves the top at α and restores it on retry (backtrack-reset), and a
 * bounded callee pops it at γ (deterministic-pop) — both O(1), zero GC. These helpers are the in-memory form (the
 * register-residency of E in R15 is a later sub-step, exactly as PL-AREAS-2 landed the trail area before R14). The
 * MARK is an int OFFSET (top-base), DELIBERATELY parallel to the trail's int-index mark, so the emitted code stores
 * it in a 32-bit frame cell and reset takes an int — byte-for-byte the same ABI shape as rt_trail_mark/unwind. Lazy
 * mmap on first bump (a non-Prolog program never reserves the region). GC-invisible for the same reason as the trail
 * area: a frame cell holds inline scalars / refs into the env+heap areas, never a GC object solely env-reachable;
 * survivors that must outlive their choice point are copied to the GC heap on the escape path (PL-AREAS-4). */
static inline void *pl_env_bump(pl_area_t *a, int nbytes) {
    if (!a->base) pl_area_init(a, PL_AREA_DEFAULT_BYTES);
    return pl_area_bump(a, (size_t)(nbytes > 0 ? nbytes : 16));
}
static inline int  pl_env_mark(const pl_area_t *a) { return a->base ? (int)(a->top - a->base) : 0; }
static inline void pl_env_reset(pl_area_t *a, int off) { if (a->base && off >= 0) a->top = a->base + off; }
#endif
