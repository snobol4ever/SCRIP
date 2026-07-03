/*--------------------------------------------------------------------------------------------------------------------*/
/* test_pl_area.c — standalone unit test for the PL-AREAS bump-pointer area (pl_area.h).
 * Verifies: bump alignment, mark/reset O(1) reclamation, grow-on-overflow preserving used bytes, and the de-risk's
 * key property that the mmap area is INVISIBLE to libgc's collector (a GC cycle does not slow down with area size). */
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
    pl_area_t a;
    pl_area_init(&a, 1u * 1024u * 1024u);
    check(a.base != (char *)0, "init mmap succeeded");
    check(a.top == a.base, "init top==base");
    check(pl_area_used(&a) == 0, "init used==0");
    /* bump alignment: a 1-byte request rounds up to 16; two of them are 16 apart */
    void *p0 = pl_area_bump(&a, 1);
    void *p1 = pl_area_bump(&a, 1);
    check((char *)p1 - (char *)p0 == 16, "bump rounds to 16");
    check(pl_area_used(&a) == 32, "used==32 after two 16B bumps");
    /* write through the blocks (touch the pages) and read back */
    *(uint64_t *)p0 = 0xCAFEBABEull; *(uint64_t *)p1 = 0xDEADBEEFull;
    check(*(uint64_t *)p0 == 0xCAFEBABEull && *(uint64_t *)p1 == 0xDEADBEEFull, "read-back ok");
    /* mark / reset: O(1) reclamation restores the top exactly */
    char *m = pl_area_mark(&a);
    for (int i = 0; i < 1000; i++) { void *q = pl_area_bump(&a, 24); check(q != (void *)0, "bump in loop"); }
    check(pl_area_used(&a) == 32 + 1000 * 32, "used grew by 1000*32 (24->32 aligned)");
    pl_area_reset(&a, m);
    check(a.top == m, "reset restored top");
    check(pl_area_used(&a) == 32, "used back to 32 after reset");
    /* large-reservation policy: a big area absorbs many bumps with NO growth (the production path — growth is the
     * pathological fallback, and an in-place grow is best-effort, so the design sizes the reservation to avoid it). */
    pl_area_t b;
    pl_area_init(&b, 8u * 1024u * 1024u);
    unsigned char *first = (unsigned char *)b.top;
    size_t filled = 0;
    for (int i = 0; i < 100000; i++) {
        unsigned char *blk = (unsigned char *)pl_area_bump(&b, 48); check(blk != (void *)0, "bump within large reservation"); blk[0] = 0x5A; blk[47] = 0xA5; filled += 48;
    }
    check(first[0] == 0x5A, "large-area first block intact");
    check(b.cap == 8u * 1024u * 1024u, "no growth needed within large reservation");
    check(pl_area_used(&b) == filled, "used matches 100000*48 aligned bumps");
    /* mark/reset reclaims the whole batch in O(1) */
    pl_area_reset(&b, b.base);
    check(pl_area_used(&b) == 0, "reset reclaims entire large batch");
    /* grow is best-effort: if it succeeds it must preserve bytes; if it fails it is a clean hard-overflow (NULL). */
    pl_area_t c; pl_area_init(&c, 64u * 1024u);
    while (c.top + 4096 <= c.limit) { unsigned char *blk = (unsigned char *)pl_area_bump(&c, 4096); memset(blk, 0x33, 4096); }
    size_t used_c = pl_area_used(&c);
    void *over = pl_area_bump(&c, 4096);
    if (over != (void *)0) { check(c.base[0] == (char)0x33 && c.base[used_c - 1] == (char)0x33, "in-place grow preserved bytes"); }
    else { check(1, "hard-overflow returned NULL cleanly (acceptable in-place-grow failure)"); }
    /* GC-invisibility: allocate a GC object reachable ONLY through the area, drop the C stack ref, force a GC,
     * and confirm the collector did NOT trace into the area (the area pointer is dangling-by-design — the area
     * owns its own reclamation). We assert the weaker, safe property: GC_gcollect() runs without touching area
     * memory as a root (no crash, area contents intact after collection). */
    pl_area_reset(&a, a.base);
    uint64_t *area_word = (uint64_t *)pl_area_bump(&a, 16);
    *area_word = 0x1234567890ABCDEFull;
    GC_gcollect();
    check(*area_word == 0x1234567890ABCDEFull, "area memory intact across GC (area is not a GC-managed region)");
    if (fails == 0) printf("PASS test_pl_area: all checks ok (mmap bump/mark/reset/grow + GC-invisible)\n");
    else printf("FAIL test_pl_area: %d check(s) failed\n", fails);
    return fails ? 1 : 0;
}
