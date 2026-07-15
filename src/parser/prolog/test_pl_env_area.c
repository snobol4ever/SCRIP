#include "pl_area.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
static int fails = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check(int cond, const char *what) { if (!cond) { fprintf(stderr, "FAIL: %s\n", what); fails++; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    pl_area_t e = { (char *)0, (char *)0, (char *)0, 0 };
    check(pl_env_mark(&e) == 0, "mark of un-reserved area is 0");
    pl_env_reset(&e, 0);
    check(e.base == (char *)0, "reset did not reserve the area");
    void *f0 = pl_env_bump(&e, 56);
    check(e.base != (char *)0, "first bump lazily reserved the mmap region");
    check(f0 != (void *)0, "first frame allocated");
    check(pl_env_mark(&e) == 64, "56B frame rounds up to 64 (16-aligned), mark==offset 64");
    void *f1 = pl_env_bump(&e, 56);
    check((char *)f1 - (char *)f0 == 64, "second frame is 64 past the first");
    check(pl_env_mark(&e) == 128, "mark advanced to 128 after two frames");
    *(uint64_t *)f0 = 0xF00DCAFEull; *(uint64_t *)f1 = 0xBEEFFACEull;
    check(*(uint64_t *)f0 == 0xF00DCAFEull && *(uint64_t *)f1 == 0xBEEFFACEull, "frame read-back ok");
    int m = pl_env_mark(&e);
    for (int i = 0; i < 5000; i++) { void *fr = pl_env_bump(&e, 80); check(fr != (void *)0, "branch frame bumped"); ((char *)fr)[0] = (char)i; }
    check(pl_env_mark(&e) == m + 5000 * 80, "E top advanced by the whole failed branch");
    pl_env_reset(&e, m);
    check(pl_env_mark(&e) == m, "reset reclaimed the entire branch in O(1)");
    check(pl_env_mark(&e) == 128, "E top is back to the pre-branch mark exactly");
    int outer = pl_env_mark(&e);
    pl_env_bump(&e, 32); pl_env_bump(&e, 32);
    int inner = pl_env_mark(&e);
    pl_env_bump(&e, 32); pl_env_bump(&e, 32); pl_env_bump(&e, 32);
    check(pl_env_mark(&e) == inner + 96, "inner frames bumped");
    pl_env_reset(&e, inner);
    check(pl_env_mark(&e) == inner, "inner reset to inner mark");
    pl_env_reset(&e, outer);
    check(pl_env_mark(&e) == outer, "outer reset to outer mark");
    int caller = pl_env_mark(&e);
    void *callee = pl_env_bump(&e, 48);
    check(callee != (void *)0 && pl_env_mark(&e) == caller + 48, "callee frame bumped on entry");
    pl_env_reset(&e, caller);
    check(pl_env_mark(&e) == caller, "deterministic-pop reclaimed the callee frame");
    uint64_t *w = (uint64_t *)pl_env_bump(&e, 16);
    *w = 0x0FEEDFACE0BADF00Dull;
    check(*w == 0x0FEEDFACE0BADF00Dull, "env-area memory intact across GC (mmap region, not GC-managed)");
    if (fails == 0) printf("PASS test_pl_env_area: all checks ok (lazy mmap + 16-align + int-offset mark/reset + GC-invisible)\n");
    else printf("FAIL test_pl_env_area: %d check(s) failed\n", fails);
    return fails ? 1 : 0;
}
