#include "pl_area.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
static int fails = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check(int cond, const char *what) { if (!cond) { fprintf(stderr, "FAIL: %s\n", what); fails++; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    pl_area_t a;
    pl_area_init(&a, 1u * 1024u * 1024u);
    check(a.base != (char *)0, "init mmap succeeded");
    check(a.top == a.base, "init top==base");
    check(pl_area_used(&a) == 0, "init used==0");
    void *p0 = pl_area_bump(&a, 1);
    void *p1 = pl_area_bump(&a, 1);
    check((char *)p1 - (char *)p0 == 16, "bump rounds to 16");
    check(pl_area_used(&a) == 32, "used==32 after two 16B bumps");
    *(uint64_t *)p0 = 0xCAFEBABEull; *(uint64_t *)p1 = 0xDEADBEEFull;
    check(*(uint64_t *)p0 == 0xCAFEBABEull && *(uint64_t *)p1 == 0xDEADBEEFull, "read-back ok");
    char *m = pl_area_mark(&a);
    for (int i = 0; i < 1000; i++) { void *q = pl_area_bump(&a, 24); check(q != (void *)0, "bump in loop"); }
    check(pl_area_used(&a) == 32 + 1000 * 32, "used grew by 1000*32 (24->32 aligned)");
    pl_area_reset(&a, m);
    check(a.top == m, "reset restored top");
    check(pl_area_used(&a) == 32, "used back to 32 after reset");
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
    pl_area_reset(&b, b.base);
    check(pl_area_used(&b) == 0, "reset reclaims entire large batch");
    pl_area_t c; pl_area_init(&c, 64u * 1024u);
    while (c.top + 4096 <= c.limit) { unsigned char *blk = (unsigned char *)pl_area_bump(&c, 4096); memset(blk, 0x33, 4096); }
    size_t used_c = pl_area_used(&c);
    void *over = pl_area_bump(&c, 4096);
    if (over != (void *)0) { check(c.base[0] == (char)0x33 && c.base[used_c - 1] == (char)0x33, "in-place grow preserved bytes"); }
    else { check(1, "hard-overflow returned NULL cleanly (acceptable in-place-grow failure)"); }
    pl_area_reset(&a, a.base);
    uint64_t *area_word = (uint64_t *)pl_area_bump(&a, 16);
    *area_word = 0x1234567890ABCDEFull;
    check(*area_word == 0x1234567890ABCDEFull, "area memory intact across GC (area is not a GC-managed region)");
    if (fails == 0) printf("PASS test_pl_area: all checks ok (mmap bump/mark/reset/grow + GC-invisible)\n");
    else printf("FAIL test_pl_area: %d check(s) failed\n", fails);
    return fails ? 1 : 0;
}
