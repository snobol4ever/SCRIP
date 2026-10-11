#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include "descr.h"
int descr_identical(DESCR_t, DESCR_t);
int c_descr_identical(DESCR_t, DESCR_t);
static int fails = 0, n = 0, handled = 0, declined = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t lo(DESCR_t d) { uint64_t w; memcpy(&w, &d, 8); return w; }
static uint64_t hi(DESCR_t d) { uint64_t w; memcpy(&w, (char *)&d + 8, 8); return w; }
static DESCR_t di(int64_t i) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_I; d.i = i; return d; }
static DESCR_t dr(double r) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_R; d.r = r; return d; }
static DESCR_t ds(char *s, uint32_t len) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_S; d.s = s; d.slen = len; return d; }
static DESCR_t dtag(int t) { DESCR_t d; memset(&d, 0, sizeof d); d.v = (uint8_t)t; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_null(DESCR_t d) { if (d.v == DT_SNUL) return 1; if (d.v != DT_S) return 0; if (!d.s) return 1; return d.slen == 0xFFFFFFFFu ? d.s[0] == 0 : d.slen == 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int model(DESCR_t a, DESCR_t b, int *answer) {
    if (a.v == DT_FAIL || b.v == DT_FAIL) { *answer = 0; return 1; }
    int na = is_null(a), nb = is_null(b);
    if (na && nb) { *answer = 1; return 1; }
    if (na != nb) { *answer = 0; return 1; }
    if (a.v == DT_I) { if (b.v != DT_I) return 0; *answer = a.i == b.i; return 1; }
    if (a.v != DT_S || b.v != DT_S) return 0;
    if (a.slen == 0xFFFFFFFFu || b.slen == 0xFFFFFFFFu) return 0;
    *answer = a.slen == b.slen && memcmp(a.s, b.s, a.slen) == 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check(DESCR_t a, DESCR_t b, int ia, int ib) {
    uint64_t rdi = lo(a), rsi = hi(a), rdx = lo(b), rcx = hi(b), rax;
    int answer = 0, want = model(a, b, &answer);
    int (*fp)(DESCR_t, DESCR_t) = descr_identical;
    __asm__ volatile ("call *%[fn]" : "+D"(rdi), "+S"(rsi), "+d"(rdx), "+c"(rcx), "=a"(rax) : [fn] "r"(fp) : "r8", "r9", "r10", "r11", "memory", "cc");
    n++;
    if ((uint8_t)rax == RTX_NOT_HANDLED) {
        declined++;
        if (want) { fails++; printf("  FAIL descr_identical case %d,%d declined a pair its fast path owns\n", ia, ib); }
        if (rdi != lo(a) || rsi != hi(a) || rdx != lo(b) || rcx != hi(b)) { fails++; printf("  FAIL descr_identical case %d,%d declined with an argument register changed\n", ia, ib); }
        return;
    }
    handled++;
    if (!want) { fails++; printf("  FAIL descr_identical case %d,%d handled a pair it must hand to the box (eax 0x%08x)\n", ia, ib, (unsigned)rax); return; }
    if ((uint32_t)rax != (uint32_t)answer || (uint32_t)rax != (uint32_t)c_descr_identical(a, b)) {
        fails++;
        printf("  FAIL descr_identical case %d,%d asm %u model %d c %d\n", ia, ib, (unsigned)(uint32_t)rax, answer, c_descr_identical(a, b));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    static char s12[] = "12", s12b[] = "12", sempty[] = "", sabc[] = "abc", sabd[] = "abd", sab[] = "ab", snul[] = "\0x", sbig[] = "a longer string of bytes";
    DESCR_t cases[] = { ds(s12, 2), ds(s12b, 2), ds(sempty, 0), ds(sempty, 0xFFFFFFFFu), ds(sabc, 3), ds(sabc, 0xFFFFFFFFu), ds(sabd, 3), ds(sab, 2), ds(sabc, 2), ds(snul, 0xFFFFFFFFu), ds(snul, 1),
        ds(sbig, 24), ds(sbig, 23), ds(NULL, 0), ds(NULL, 5), ds(NULL, 0xFFFFFFFFu), ds(s12, 0), di(0), di(1), di(-1), di(12), di(INT64_MAX), di(INT64_MIN), dr(0.0), dr(1.5), dr(-0.0), dr(NAN),
        dtag(DT_SNUL), dtag(DT_FAIL), dtag(DT_N), dtag(DT_DATA), dtag(DT_P) };
    int nc = (int)(sizeof cases / sizeof cases[0]);
    printf("RTX MISC descr_identical verdict battery (the leaf against its C twin, the verdict road of ARCH-RT-CALL-PROTOCOL.md section 15)\n");
    for (int i = 0; i < nc; i++) for (int j = 0; j < nc; j++) check(cases[i], cases[j], i, j);
    printf("RTX MISC: %d cases, %d handled, %d declined, %d mismatches\n", n, handled, declined, fails);
    if (!handled || !declined) { printf("RTX MISC UNIT: FAIL -- a battery that saw only one verdict proves nothing\n"); return 1; }
    printf("RTX MISC UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
