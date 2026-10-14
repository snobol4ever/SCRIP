#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "descr.h"
#include "core.h"
#include "gc_heap.h"
DESCR_t rt_table_assign_fast(DESCR_t, DESCR_t, DESCR_t);
DESCR_t c_rt_table_assign_fast(DESCR_t, DESCR_t, DESCR_t);
extern int g_gc_pending;
static int fails = 0, nstep = 0, handled = 0, declined = 0, inserted = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void words(DESCR_t d, uint64_t *w) { memcpy(&w[0], &d, 8); memcpy(&w[1], (char *)&d + 8, 8); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int leaf(DESCR_t b, DESCR_t k, DESCR_t v, DESCR_t *out) {
    uint64_t wb[2], wk[2], wv[2];
    words(b, wb);
    words(k, wk);
    words(v, wv);
    register uint64_t r8_ __asm__("r8") = wv[0];
    register uint64_t r9_ __asm__("r9") = wv[1];
    uint64_t rdi = wb[0], rsi = wb[1], rdx = wk[0], rcx = wk[1], rax;
    DESCR_t (*fp)(DESCR_t, DESCR_t, DESCR_t) = rt_table_assign_fast;
    __asm__ volatile ("leaq -128(%%rsp), %%rsp; call *%[fn]; leaq 128(%%rsp), %%rsp" : "+D"(rdi), "+S"(rsi), "+d"(rdx), "+c"(rcx), "+r"(r8_), "+r"(r9_), "=a"(rax) : [fn] "r"(fp) : "r10", "r11",
        "memory", "cc");
    if ((uint8_t)rax == RTX_NOT_HANDLED) {
        if (rdi != wb[0] || rsi != wb[1] || rdx != wk[0] || rcx != wk[1] || r8_ != wv[0] || r9_ != wv[1]) { fails++; printf("  FAIL step %d: declined with an argument register changed\n", nstep); }
        return 0;
    }
    memcpy(out, &rax, 8);
    memcpy((char *)out + 8, &rdx, 8);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int same_tables(TBBLK_t *a, TBBLK_t *b) {
    if (a->size != b->size || a->ord_len != b->ord_len || a->ord_cap != b->ord_cap || a->nbuck != b->nbuck || a->icn_mask != b->icn_mask || a->ord_dead != b->ord_dead) return 0;
    if (a->ord_len && memcmp(a->ord, b->ord, (size_t)a->ord_len * sizeof(DESCR_t))) return 0;
    for (unsigned i = 0; i < a->nbuck; i++) {
        TBBUCK_t *x = a->buckets[i], *y = b->buckets[i];
        if (!x != !y) return 0;
        if (!x) continue;
        if (x->len != y->len || x->cap != y->cap) return 0;
        if (memcmp(x->ent, y->ent, (size_t)x->len * sizeof(TBPAIR_t))) return 0;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void step(TBBLK_t *a, TBBLK_t *b, DESCR_t k, DESCR_t v, const char *what) {
    DESCR_t ba = { .v = DT_T }, bb = { .v = DT_T }, ra, rb;
    ba.tbl = a;
    bb.tbl = b;
    nstep++;
    g_gc_pending = 0;
    unsigned before = (unsigned)a->size;
    if (leaf(ba, k, v, &ra)) { handled++; if (a->size == (int)before + 1) inserted++; } else { declined++; ra = c_rt_table_assign_fast(ba, k, v); }
    g_gc_pending = 0;
    rb = c_rt_table_assign_fast(bb, k, v);
    uint64_t wa[2], wb[2];
    words(ra, wa);
    words(rb, wb);
    if (wa[0] != wb[0] || wa[1] != wb[1]) {
        fails++;
        printf("  FAIL %s step %d: result %016llx/%016llx twin %016llx/%016llx\n", what, nstep, (unsigned long long)wa[0], (unsigned long long)wa[1], (unsigned long long)wb[0],
            (unsigned long long)wb[1]);
    }
    if (!same_tables(a, b)) { fails++; printf("  FAIL %s step %d: the tables differ (size %d/%d ord %u/%u nbuck %u/%u)\n", what, nstep, a->size, b->size, a->ord_len, b->ord_len, a->nbuck, b->nbuck); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t lcg(uint64_t *s) { *s = *s * 6364136223846793005ull + 1442695040888963407ull; return *s >> 33; }
static char kbuf[4096][24];
static char vbuf[64][8];
static DESCR_t ks(int i, int mode) { DESCR_t d; snprintf(kbuf[i & 4095], sizeof kbuf[0], "k%d_key", i); d = STRVAL(kbuf[i & 4095]); if (mode == 1) d.slen = 0xFFFFFFFFu; return d; }
static DESCR_t kv(int i) { if (i % 5 == 0) { snprintf(vbuf[i & 63], sizeof vbuf[0], "v%d", i); return STRVAL(vbuf[i & 63]); } return INTVAL(i * 3 - 7); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void run(int init, const char *what) {
    TBBLK_t *a = init ? table_new_args(init, 0) : table_new(), *b = init ? table_new_args(init, 0) : table_new();
    uint64_t seed = 12345u + (uint64_t)init;
    int n = 2200;
    for (int i = 1; i <= n; i++) step(a, b, INTVAL(i), kv(i), what);
    for (int i = n; i >= 1; i -= 3) step(a, b, INTVAL(-i), kv(i), what);
    for (int i = 0; i < 1500; i++) { int r = (int)(lcg(&seed) % 3000u) - 1500; step(a, b, INTVAL(r), kv(i), what); }
    for (int i = 0; i < 700; i++) step(a, b, ks(i, i % 2), kv(i), what);
    for (int i = 0; i < 300; i++) step(a, b, ks((int)(lcg(&seed) % 900u), (int)(lcg(&seed) % 2u)), kv(i), what);
    for (int i = 1; i < 120; i++) step(a, b, REALVAL(i / 2.0), kv(i), what);
    step(a, b, NULVCL, kv(1), what);
    step(a, b, STRVAL(""), kv(2), what);
    for (int i = 1; i <= n; i += 7) { table_delete_d(a, INTVAL(i)); table_delete_d(b, INTVAL(i)); }
    for (int i = n + 1; i <= n + 400; i++) step(a, b, INTVAL(i), kv(i), what);
    for (int i = 1; i <= n; i += 7) step(a, b, INTVAL(i), kv(i), what);
    for (int i = 0; i < 100; i++) {
        char *s = vbuf[i & 63];
        snprintf(s, sizeof vbuf[0], "w%d", i);
        rt_sxt_fr_p->owner = s;
        step(a, b, INTVAL(100000 + i), STRVAL(s), what);
        if (rt_sxt_fr_p->owner == s) { fails++; printf("  FAIL %s: the extension owner survived a store of its own string\n", what); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    core_lib_init();
    run(0, "default table");
    run(16, "TABLE(16)");
    run(64, "TABLE(64)");
    run(512, "TABLE(512)");
    run(5000, "TABLE(5000)");
    printf("RTX TINS: %d steps, %d handled by the leaf (%d of them new-key insertions), %d declined to the twin, %d mismatches\n", nstep, handled, inserted, declined, fails);
    if (inserted < 1000 || !declined) { printf("RTX TINS UNIT: FAIL -- a battery that saw only one verdict proves nothing\n"); return 1; }
    printf("RTX TINS UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
