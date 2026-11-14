#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "ir/descr.h"
#include "ir/cset_block.h"
#include "runtime/core/core.h"
#include "ir/descr_tags_dt_platom.h"
int rt_jct_relop(DESCR_t, DESCR_t, int);
int c_rt_jct_relop(DESCR_t, DESCR_t, int);
void rt_relop_val_coerce(DESCR_t, DESCR_t, DESCR_t *);
void c_rt_relop_val_coerce(DESCR_t, DESCR_t, DESCR_t *);
static int fails = 0, ncase = 0, handled = 0, vc_case = 0, vc_handled = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int own(DESCR_t d) {
    if (d.v == DT_I || d.v == DT_R) return 1;
    if (d.v != DT_S || d.slen == 0 || d.slen == 0xFFFFFFFFu || !d.s) return 0;
    const char *p = d.s, *e = d.s + d.slen;
    while (p < e && *p == ' ') p++;
    if (p < e && (*p == '+' || *p == '-')) p++;
    int nd = 0;
    while (p < e && *p >= '0' && *p <= '9') { p++; nd++; }
    if (nd < 1 || nd > 18) return 0;
    while (p < e && *p == ' ') p++;
    return p == e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void words(DESCR_t d, uint64_t *w) { memcpy(&w[0], &d, 8); memcpy(&w[1], (char *)&d + 8, 8); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check(DESCR_t a, DESCR_t b, int op) {
    uint64_t wa[2], wb[2];
    int want = (op >= 5 && op <= 10) && own(a) && own(b);
    if (!want && op != 22 && op != 23) return;
    words(a, wa);
    words(b, wb);
    uint64_t rdi = wa[0], rsi = wa[1], rdx = wb[0], rcx = wb[1], rax;
    register uint64_t r8_ __asm__("r8") = (uint64_t)(unsigned)op;
    int (*fp)(DESCR_t, DESCR_t, int) = rt_jct_relop;
    __asm__ volatile ("leaq -128(%%rsp), %%rsp; call *%[fn]; leaq 128(%%rsp), %%rsp" : "+D"(rdi), "+S"(rsi), "+d"(rdx), "+c"(rcx), "+r"(r8_), "=a"(rax) : [fn] "r"(fp) : "r9", "r10", "r11", "memory",
        "cc", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6");
    ncase++;
    handled++;
    {
        int c = c_rt_jct_relop(a, b, op);
        if ((int)(rax & 0xFFFFFFFFu) != (c ? 1 : 0) || (c != 0 && c != 1)) { fails++; printf("  FAIL op %d case %d: leaf %d twin %d\n", op, ncase, (int)(rax & 0xFFFFFFFFu), c); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check_vc(DESCR_t a, DESCR_t b) {
    uint64_t wa[2], wb[2], wo[2] = { 0x5a5a5a5a5a5a5a5aull, 0x5a5a5a5a5a5a5a5aull };
    int want = (a.v == DT_I || (a.v == DT_S && own(a))) && (b.v == DT_I || (b.v == DT_S && own(b)));
    if (!want) return;
    words(a, wa);
    words(b, wb);
    uint64_t rdi = wa[0], rsi = wa[1], rdx = wb[0], rcx = wb[1], rax;
    register uint64_t r8_ __asm__("r8") = (uint64_t)(uintptr_t)wo;
    void (*fp)(DESCR_t, DESCR_t, DESCR_t *) = rt_relop_val_coerce;
    __asm__ volatile ("leaq -128(%%rsp), %%rsp; call *%[fn]; leaq 128(%%rsp), %%rsp" : "+D"(rdi), "+S"(rsi), "+d"(rdx), "+c"(rcx), "+r"(r8_), "=a"(rax) : [fn] "r"(fp) : "r9", "r10", "r11", "memory",
        "cc", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6");
    vc_case++;
    vc_handled++;
    DESCR_t c;
    memset(&c, 0x5a, sizeof c);
    c_rt_relop_val_coerce(a, b, &c);
    if (memcmp(&c, wo, sizeof c) != 0) { fails++; printf("  FAIL coerce case %d: leaf %016llx %016llx twin differs\n", vc_case, (unsigned long long)wo[0], (unsigned long long)wo[1]); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    core_lib_init();
    char cblk3[CSET_BLK_HDR + 4], cblk1[CSET_BLK_HDR + 2];
    DESCR_t cabc = CSETVAL(cset_block_init(cblk3, "abc", 3)), cfive = CSETVAL(cset_block_init(cblk1, "5", 1));
    DESCR_t vals[] = { INTVAL(0), INTVAL(1), INTVAL(-1), INTVAL(5), INTVAL(-5), INTVAL(INT64_MAX), INTVAL(INT64_MIN), INTVAL((int64_t)1 << 53), INTVAL(((int64_t)1 << 53) + 1),
        INTVAL((int64_t)1 << 62), REALVAL(0.0), REALVAL(-0.0), REALVAL(1.5), REALVAL(-1.5), REALVAL(5.0), REALVAL(1e300), REALVAL(-1e300), REALVAL(INFINITY), REALVAL(-INFINITY), REALVAL(NAN),
        REALVAL(5e-324), REALVAL(9007199254740992.0), REALVAL(9007199254740994.0), STRVAL("5"), STRVAL("abc"), STRVAL(""), NULVCL };
    int nv = (int)(sizeof vals / sizeof vals[0]);
    static const int ops[] = { 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 22, 23, 5 | 0x100, 9 | 0x100 };
    for (unsigned k = 0; k < sizeof ops / sizeof ops[0]; k++) for (int i = 0; i < nv; i++) for (int j = 0; j < nv; j++) check(vals[i], vals[j], ops[k]);
    static char sa[] = "abc", sb[] = "abc", sc[] = "abd", sd[] = "ab\0c", se[] = "ab\0d", sf[] = "abcd", sg[] = "\003bc", sh[] = "\003bc", si[] = "";
    DESCR_t svals[] = { BSTRVAL(sa, 3), BSTRVAL(sb, 3), BSTRVAL(sc, 3), BSTRVAL(sa, 2), BSTRVAL(sf, 3), BSTRVAL(sf, 4), BSTRVAL(sd, 4), BSTRVAL(se, 4), BSTRVAL(sd, 2), BSTRVAL(sg, 3), BSTRVAL(sh, 3),
        BSTRVAL(si, 0), BSTRVAL(sa, 0), cabc, cabc, NULVCL, INTVAL(3), REALVAL(3.0) };
    int ns = (int)(sizeof svals / sizeof svals[0]);
    for (int k = 22; k <= 23; k++) for (int i = 0; i < ns; i++) for (int j = 0; j < ns; j++) check(svals[i], svals[j], k);
    static char n1[] = "123abc", n2[] = "1\0002", n3[] = " 42 x";
    DESCR_t nvals[] = { STRVAL("5"), STRVAL(" 5 "), STRVAL("-5"), STRVAL("+5"), STRVAL("007"), STRVAL("-0"), STRVAL("123456789012345678"), STRVAL("-123456789012345678"), STRVAL("999999999999999999"),
        STRVAL("1234567890123456789"), STRVAL("9223372036854775807"), STRVAL("5\t"), STRVAL("\t5"), STRVAL("5.0"), STRVAL("1e3"), STRVAL("   "), STRVAL("+"), STRVAL("-"), STRVAL("5 5"), STRVAL("- 5"),
        STRVAL("+-5"), STRVAL("16r1F"), BSTRVAL(n1, 3), BSTRVAL(n1, 4), BSTRVAL(n1, 1), BSTRVAL(n2, 1), BSTRVAL(n2, 3), BSTRVAL(n3, 4), BSTRVAL(n3, 5), cfive, NULVCL, INTVAL(5), INTVAL(-5),
        INTVAL(42), INTVAL(123456789012345678), INTVAL(INT64_MIN), REALVAL(5.0), REALVAL(-0.5), REALVAL(42.0000001), REALVAL(NAN), REALVAL(INFINITY) };
    int nn = (int)(sizeof nvals / sizeof nvals[0]);
    for (int k = 5; k <= 10; k++) for (int i = 0; i < nn; i++) for (int j = 0; j < nn; j++) check(nvals[i], nvals[j], k);
    for (int i = 0; i < nn; i++) for (int j = 0; j < nn; j++) check_vc(nvals[i], nvals[j]);
    for (int i = 0; i < nv; i++) for (int j = 0; j < nv; j++) check_vc(vals[i], vals[j]);
    printf("RTX RELOP COERCE: %d cases, the leaf answers every one and its slow path is the twin; only pairs its fast path owns are driven (the twin raises the language voice on the rest)\n", vc_case)
        ;
    if (!vc_handled) { printf("RTX RELOP UNIT: FAIL -- no coerce case ran\n"); return 1; }
    printf("RTX RELOP: %d cases, %d mismatches against the twin\n", ncase, fails);
    if (!handled) { printf("RTX RELOP UNIT: FAIL -- no case ran\n"); return 1; }
    printf("RTX RELOP UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
