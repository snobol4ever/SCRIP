#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include "descr.h"
DESCR_t rt_add(DESCR_t, DESCR_t);
DESCR_t rt_sub(DESCR_t, DESCR_t);
DESCR_t rt_mul(DESCR_t, DESCR_t);
DESCR_t rt_add_sno(DESCR_t, DESCR_t);
DESCR_t rt_sub_sno(DESCR_t, DESCR_t);
DESCR_t rt_mul_sno(DESCR_t, DESCR_t);
DESCR_t c_rt_add(DESCR_t, DESCR_t);
DESCR_t c_rt_sub(DESCR_t, DESCR_t);
DESCR_t c_rt_mul(DESCR_t, DESCR_t);
DESCR_t c_rt_add_sno(DESCR_t, DESCR_t);
DESCR_t c_rt_sub_sno(DESCR_t, DESCR_t);
DESCR_t c_rt_mul_sno(DESCR_t, DESCR_t);
typedef DESCR_t (*binop_fn)(DESCR_t, DESCR_t);
static const struct {
    const char *name;
    binop_fn leaf;
    binop_fn twin;
    int op;
} leaves[] = { { "rt_add", rt_add, c_rt_add, 0 }, { "rt_sub", rt_sub, c_rt_sub, 1 }, { "rt_mul", rt_mul, c_rt_mul, 2 }, { "rt_add_sno", rt_add_sno, c_rt_add_sno, 0 }, { "rt_sub_sno", rt_sub_sno,
    c_rt_sub_sno, 1 }, { "rt_mul_sno", rt_mul_sno, c_rt_mul_sno, 2 }, };
static int fails = 0, n = 0, handled = 0, declined = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t lo(DESCR_t d) { uint64_t w; memcpy(&w, &d, 8); return w; }
static uint64_t hi(DESCR_t d) { uint64_t w; memcpy(&w, (char *)&d + 8, 8); return w; }
static DESCR_t di(int64_t i) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_I; d.i = i; return d; }
static DESCR_t dr(double r) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_R; d.r = r; return d; }
static DESCR_t ds(char *s) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_S; d.s = s; d.slen = (uint32_t)strlen(s); return d; }
static DESCR_t dtag(int t) { DESCR_t d; memset(&d, 0, sizeof d); d.v = (uint8_t)t; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int expect_handled(DESCR_t a, DESCR_t b, int op) {
    int64_t z;
    double r;
    if (a.v == DT_I && b.v == DT_I) return !(op == 0 ? __builtin_add_overflow(a.i, b.i, &z) : op == 1 ? __builtin_sub_overflow(a.i, b.i, &z) : __builtin_mul_overflow(a.i, b.i, &z));
    if (a.v != DT_R || b.v != DT_R) return 0;
    r = op == 0 ? a.r + b.r : op == 1 ? a.r - b.r : a.r * b.r;
    return isfinite(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check(int k, DESCR_t a, DESCR_t b, int ia, int ib) {
    uint64_t rdi = lo(a), rsi = hi(a), rdx = lo(b), rcx = hi(b), rax;
    binop_fn fp = leaves[k].leaf;
    int want = expect_handled(a, b, leaves[k].op);
    __asm__ volatile ("call *%[fn]" : "+D"(rdi), "+S"(rsi), "+d"(rdx), "+c"(rcx), "=a"(rax) : [fn] "r"(fp) : "r8", "r9", "r10", "r11", "memory", "cc", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5",
        "xmm6", "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15");
    n++;
    if ((uint8_t)rax == RTX_NOT_HANDLED) {
        declined++;
        if (want) { fails++; printf("  FAIL %-10s case %d,%d declined a pair its fast path owns\n", leaves[k].name, ia, ib); }
        if (rdi != lo(a) || rsi != hi(a) || rdx != lo(b) || rcx != hi(b)) { fails++; printf("  FAIL %-10s case %d,%d declined with an argument register changed\n", leaves[k].name, ia, ib); }
        return;
    }
    handled++;
    if (!want) { fails++; printf("  FAIL %-10s case %d,%d handled a pair it must hand to the box (tag 0x%02x)\n", leaves[k].name, ia, ib, (unsigned)(uint8_t)rax); return; }
    {
        DESCR_t c = leaves[k].twin(a, b);
        if ((uint8_t)rax != c.v || rdx != hi(c)) {
            fails++;
            printf("  FAIL %-10s case %d,%d asm 0x%02x/%016llx c 0x%02x/%016llx\n", leaves[k].name, ia, ib, (unsigned)(uint8_t)rax, (unsigned long long)rdx, (unsigned)c.v, (unsigned long long)hi(c));
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    static char s12[] = "12", sempty[] = "", sabc[] = "abc";
    DESCR_t cases[] = { di(0), di(1), di(-1), di(7), di(INT64_MAX), di(INT64_MIN), di((int64_t)1 << 62), di(-((int64_t)1 << 62)), di(3037000499LL), di(3037000500LL), dr(0.0), dr(-0.0), dr(1.5),
        dr(2.5), dr(1e300), dr(-1e300), dr(DBL_MAX), dr(INFINITY), dr(-INFINITY), dr(NAN), dr(5e-324), ds(s12), ds(sempty), ds(sabc), dtag(DT_SNUL), dtag(DT_FAIL), dtag(DT_N), };
    int nc = (int)(sizeof cases / sizeof cases[0]);
    printf("RTX ARITH verdict battery (the six arith leaves against their C twins, the verdict road of ARCH-RT-CALL-PROTOCOL.md section 15)\n");
    for (int k = 0; k < (int)(sizeof leaves / sizeof leaves[0]); k++) for (int i = 0; i < nc; i++) for (int j = 0; j < nc; j++) check(k, cases[i], cases[j], i, j);
    printf("RTX ARITH: %d cases, %d handled, %d declined, %d mismatches\n", n, handled, declined, fails);
    if (!handled || !declined) { printf("RTX ARITH UNIT: FAIL -- a battery that saw only one verdict proves nothing\n"); return 1; }
    printf("RTX ARITH UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
