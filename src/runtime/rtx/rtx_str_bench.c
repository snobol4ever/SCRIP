#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "descr.h"
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
extern int g_gc_pending;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t S(const char *s, uint32_t n) { DESCR_t d; d.v = DT_S; d.slen = n; d.s = (char *)s; return d; }
static double now_ns(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return (double)t.tv_sec * 1e9 + (double)t.tv_nsec; }
static volatile uint64_t sink = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static double run(DESCR_t a, DESCR_t b, long n) {
    double t0 = now_ns();
    for (long i = 0; i < n; i++) { DESCR_t r = str_concat_d(a, b); sink += (uint64_t)(uintptr_t)r.s + r.slen; }
    return now_ns() - t0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char **argv) {
    static const char A64[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ab";
    static const char B64[] = "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba9876543210ZY";
    long n = (argc > 1) ? atol(argv[1]) : 300000;
    int reps = (argc > 2) ? atoi(argv[2]) : 7;
    int only = (argc > 3) ? atoi(argv[3]) : -1;
    const char *arm = getenv("SCRIP_RTX_STR");
    struct { const char *name; uint32_t al, bl; } cases[] = {
        { "tiny   3+3",   3,  3 }, { "small  8+8",   8,  8 }, { "token 12+7",  12,  7 },
        { "mid   24+24", 24, 24 }, { "big   64+64", 64, 64 },
    };
    printf("# RTX-3 str_concat_d A/B  arm=SCRIP_RTX_STR=%s  n=%ld reps=%d  (min ns/concat)\n", arm ? arm : "default(1)", n, reps);
    for (unsigned c = 0; c < sizeof cases / sizeof cases[0]; c++) {
        if (only >= 0 && (int)c != only) continue;
        DESCR_t a = S(A64, cases[c].al), b = S(B64, cases[c].bl);
        double best = 1e30; int gc0 = g_gc_pending, gcs = 0;
        for (int r = 0; r < reps; r++) { double d = run(a, b, n); if (d < best) best = d; if (g_gc_pending != gc0) gcs++; }
        printf("%-12s  %8.2f ns/concat%s\n", cases[c].name, best / (double)n, gcs ? "   [!! COLLECTION OBSERVED — NOT COMPARABLE]" : "");
    }
    return 0;
}
