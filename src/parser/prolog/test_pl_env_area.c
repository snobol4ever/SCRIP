/*--------------------------------------------------------------------------------------------------------------------*/
/* test_pl_env_area.c — standalone unit test for the PL-AREAS-3 ENVIRONMENT-area ABI (pl_env_bump/mark/reset in
 * pl_area.h). These are the EXACT functions the runtime wrappers rt_e_bump/rt_e_mark/rt_e_reset (unification.c)
 * delegate to over the global g_pl_env_area; testing them on a local pl_area_t exercises the real allocator logic.
 * Verifies: lazy mmap-on-first-bump, 16-aligned frame allocation, the int-OFFSET mark/reset round-trip (the
 * backtrack-reset + deterministic-pop primitive, mirroring the trail's int-index ABI), nested marks, and the
 * GC-invisibility property the area depends on (contents intact across a collection). */
#include "pl_area.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <gc.h>
/*--------------------------------------------------------------------------------------------------------------------*/
static int fails = 0;
static void check(int cond, const char *what) { if (!cond) { fprintf(stderr, "FAIL: %s\n", what); fails++; } }
/*--------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    GC_INIT();
    /* a zero-initialised area is the un-reserved state (matches the g_pl_env_area static initialiser). */
    pl_area_t e = { (char *)0, (char *)0, (char *)0, 0 };
    check(pl_env_mark(&e) == 0, "mark of un-reserved area is 0");
    pl_env_reset(&e, 0);  /* reset on an un-reserved area is a safe no-op */
    check(e.base == (char *)0, "reset did not reserve the area");
    /* lazy mmap-on-first-bump: the first frame allocation reserves the region. */
    void *f0 = pl_env_bump(&e, 56);   /* a typical callee frame: 8 header + 16*3 cells */
    check(e.base != (char *)0, "first bump lazily reserved the mmap region");
    check(f0 != (void *)0, "first frame allocated");
    check(pl_env_mark(&e) == 64, "56B frame rounds up to 64 (16-aligned), mark==offset 64");
    void *f1 = pl_env_bump(&e, 56);
    check((char *)f1 - (char *)f0 == 64, "second frame is 64 past the first");
    check(pl_env_mark(&e) == 128, "mark advanced to 128 after two frames");
    /* write through the frames (touch the demand-faulted pages) and read back. */
    *(uint64_t *)f0 = 0xF00DCAFEull; *(uint64_t *)f1 = 0xBEEFFACEull;
    check(*(uint64_t *)f0 == 0xF00DCAFEull && *(uint64_t *)f1 == 0xBEEFFACEull, "frame read-back ok");
    /* the reclamation primitive: mark at a choice α, bump a batch of failed-branch frames, reset on retry. */
    int m = pl_env_mark(&e);          /* α: save E top */
    for (int i = 0; i < 5000; i++) { void *fr = pl_env_bump(&e, 80); check(fr != (void *)0, "branch frame bumped"); ((char *)fr)[0] = (char)i; }
    check(pl_env_mark(&e) == m + 5000 * 80, "E top advanced by the whole failed branch");
    pl_env_reset(&e, m);              /* retry: O(1) reclaim of every branch frame */
    check(pl_env_mark(&e) == m, "reset reclaimed the entire branch in O(1)");
    check(pl_env_mark(&e) == 128, "E top is back to the pre-branch mark exactly");
    /* nested marks (a choice inside a choice): inner reset restores to inner mark, outer to outer. */
    int outer = pl_env_mark(&e);
    pl_env_bump(&e, 32); pl_env_bump(&e, 32);
    int inner = pl_env_mark(&e);
    pl_env_bump(&e, 32); pl_env_bump(&e, 32); pl_env_bump(&e, 32);
    check(pl_env_mark(&e) == inner + 96, "inner frames bumped");
    pl_env_reset(&e, inner);
    check(pl_env_mark(&e) == inner, "inner reset to inner mark");
    pl_env_reset(&e, outer);
    check(pl_env_mark(&e) == outer, "outer reset to outer mark");
    /* deterministic-pop shape: bump a frame, then pop it back to the caller's mark on a det return. */
    int caller = pl_env_mark(&e);
    void *callee = pl_env_bump(&e, 48);
    check(callee != (void *)0 && pl_env_mark(&e) == caller + 48, "callee frame bumped on entry");
    pl_env_reset(&e, caller);         /* bounded callee pops its frame at γ */
    check(pl_env_mark(&e) == caller, "deterministic-pop reclaimed the callee frame");
    /* GC-invisibility: the area is mmap'd, not a GC region; its contents survive a collection untouched. */
    uint64_t *w = (uint64_t *)pl_env_bump(&e, 16);
    *w = 0x0FEEDFACE0BADF00Dull;
    GC_gcollect();
    check(*w == 0x0FEEDFACE0BADF00Dull, "env-area memory intact across GC (mmap region, not GC-managed)");
    if (fails == 0) printf("PASS test_pl_env_area: all checks ok (lazy mmap + 16-align + int-offset mark/reset + GC-invisible)\n");
    else printf("FAIL test_pl_env_area: %d check(s) failed\n", fails);
    return fails ? 1 : 0;
}
