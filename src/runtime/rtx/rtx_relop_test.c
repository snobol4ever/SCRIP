#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "descr.h"
#include "core.h"
int rt_jct_relop(DESCR_t, DESCR_t, int);
int c_rt_jct_relop(DESCR_t, DESCR_t, int);
static int fails = 0, ncase = 0, handled = 0, declined = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void words(DESCR_t d, uint64_t *w) { memcpy(&w[0], &d, 8); memcpy(&w[1], (char *)&d + 8, 8); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check(DESCR_t a, DESCR_t b, int op) {
    uint64_t wa[2], wb[2];
    words(a, wa);
    words(b, wb);
    uint64_t rdi = wa[0], rsi = wa[1], rdx = wb[0], rcx = wb[1], rax;
    register uint64_t r8_ __asm__("r8") = (uint64_t)(unsigned)op;
    int (*fp)(DESCR_t, DESCR_t, int) = rt_jct_relop;
    __asm__ volatile ("leaq -128(%%rsp), %%rsp; call *%[fn]; leaq 128(%%rsp), %%rsp" : "+D"(rdi), "+S"(rsi), "+d"(rdx), "+c"(rcx), "+r"(r8_), "=a"(rax) : [fn] "r"(fp) : "r9", "r10", "r11", "memory",
        "cc", "xmm0", "xmm1", "xmm2", "xmm3");
    int want = (op >= 5 && op <= 10) && (a.v == DT_I || a.v == DT_R) && (b.v == DT_I || b.v == DT_R);
    ncase++;
    if ((uint8_t)rax == RTX_NOT_HANDLED) {
        declined++;
        if (want) { fails++; printf("  FAIL op %d case %d: declined a numeric pair the leaf owns\n", op, ncase); }
        if (rdi != wa[0] || rsi != wa[1] || rdx != wb[0] || rcx != wb[1] || r8_ != (uint64_t)(unsigned)op) {
            fails++;
            printf("  FAIL op %d case %d: declined with an argument register changed\n", op, ncase);
        }
        return;
    }
    handled++;
    if (!want && op >= 5 && op <= 10) { fails++; printf("  FAIL op %d case %d: handled a pair outside the numeric arm\n", op, ncase); }
    if (want) {
        int c = c_rt_jct_relop(a, b, op);
        if ((int)(rax & 0xFFFFFFFFu) != (c ? 1 : 0) || (c != 0 && c != 1)) { fails++; printf("  FAIL op %d case %d: leaf %d twin %d\n", op, ncase, (int)(rax & 0xFFFFFFFFu), c); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    core_lib_init();
    DESCR_t vals[] = { INTVAL(0), INTVAL(1), INTVAL(-1), INTVAL(5), INTVAL(-5), INTVAL(INT64_MAX), INTVAL(INT64_MIN), INTVAL((int64_t)1 << 53), INTVAL(((int64_t)1 << 53) + 1),
        INTVAL((int64_t)1 << 62), REALVAL(0.0), REALVAL(-0.0), REALVAL(1.5), REALVAL(-1.5), REALVAL(5.0), REALVAL(1e300), REALVAL(-1e300), REALVAL(INFINITY), REALVAL(-INFINITY), REALVAL(NAN),
        REALVAL(5e-324), REALVAL(9007199254740992.0), REALVAL(9007199254740994.0), STRVAL("5"), STRVAL("abc"), STRVAL(""), NULVCL };
    int nv = (int)(sizeof vals / sizeof vals[0]);
    static const int ops[] = { 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 22, 23, 5 | 0x100, 9 | 0x100 };
    for (unsigned k = 0; k < sizeof ops / sizeof ops[0]; k++) for (int i = 0; i < nv; i++) for (int j = 0; j < nv; j++) check(vals[i], vals[j], ops[k]);
    printf("RTX RELOP: %d cases, %d handled, %d declined, %d mismatches\n", ncase, handled, declined, fails);
    if (!handled || !declined) { printf("RTX RELOP UNIT: FAIL -- a battery that saw only one verdict proves nothing\n"); return 1; }
    printf("RTX RELOP UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
