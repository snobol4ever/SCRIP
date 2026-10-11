#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "descr.h"
#include "core.h"
DESCR_t rt_sno_name_dl(DESCR_t *, int);
DESCR_t c_rt_sno_name_dl(DESCR_t *, int);
static int fails = 0, ncase = 0, handled = 0, declined = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t ds(const char *s, uint32_t len) { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_S; d.s = (char *)s; d.slen = len; return d; }
static int own(const char *nm) { size_t l = strlen(nm); if (strchr("\001&ABFRS", nm[0])) return 0; return !((nm[0] == 'I' && l == 5) || (nm[0] == 'O' && l == 6) || (nm[0] == 'T' && l == 8)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check(DESCR_t a, int nargs, const char *what, int expect) {
    DESCR_t args[1] = { a };
    uint64_t rdi = (uint64_t)(uintptr_t)args, rsi = (uint64_t)nargs, rax, rdx;
    DESCR_t (*fp)(DESCR_t *, int) = rt_sno_name_dl;
    __asm__ volatile ("call *%[fn]" : "+D"(rdi), "+S"(rsi), "=a"(rax), "=d"(rdx) : [fn] "r"(fp) : "rcx", "r8", "r9", "r10", "r11", "memory", "cc");
    ncase++;
    if ((uint8_t)rax == RTX_NOT_HANDLED) {
        declined++;
        if (expect == 1) { fails++; printf("  FAIL %s: declined a name its fast path owns\n", what); }
        if (rdi != (uint64_t)(uintptr_t)args || rsi != (uint64_t)nargs) { fails++; printf("  FAIL %s: declined with an argument register changed\n", what); }
        return;
    }
    handled++;
    if (expect == 0) { fails++; printf("  FAIL %s: handled a name it must hand to the box\n", what); }
    DESCR_t c = c_rt_sno_name_dl(args, nargs);
    uint64_t cw0, cw1;
    memcpy(&cw0, &c, 8);
    memcpy(&cw1, (char *)&c + 8, 8);
    if (rax != cw0 || rdx != cw1) {
        fails++;
        printf("  FAIL %s: leaf %016llx/%016llx twin %016llx/%016llx\n", what, (unsigned long long)rax, (unsigned long long)rdx, (unsigned long long)cw0, (unsigned long long)cw1);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    static const char * const names[] = { "a", "xy", "xyz", "abcd", "abcdefg", "abcdefgh", "abcdefghi", "abcdefghijklmnop", "abcdefghijklmnopq", "abcdefghijklmnopqrstuvwxyz", "holder", "target", "n1",
        "UPPER", "Bee", "Sam", "Fence", "Rtab", "Any", "INPUT", "OUTPUT", "TERMINAL", "Input", "output", "terminal", "lower_with_underscore", "x9" };
    int nn = (int)(sizeof names / sizeof names[0]);
    core_lib_init();
    for (int i = 0; i < nn; i++) NV_SET_fn(names[i], INTVAL(i + 1));
    for (int i = 0; i < nn; i++) { DESCR_t d = ds(names[i], (uint32_t)strlen(names[i])); check(d, 1, names[i], own(names[i])); check(d, 2, "two arguments", 0); }
    static const char * const absent[] = { "nothere", "abcdefgz", "abcdefghijklmnopz", "zzzzzzzzzzzzzzzzzzzzzzzzzzz", "q", "qq", "qqq", "Z" };
    for (int i = 0; i < (int)(sizeof absent / sizeof absent[0]); i++) check(ds(absent[i], (uint32_t)strlen(absent[i])), 1, absent[i], 0);
    check(ds("", 0), 1, "empty", 0);
    check(ds("&ANCHOR", 7), 1, "keyword", 0);
    check(ds("\001x", 2), 1, "nul key lead", 0);
    check(ds("ab\0cd", 5), 1, "embedded nul", 0);
    check(ds("abc", 0xFFFFFFFFu), 1, "unknown length", 0);
    check(ds(NULL, 3), 1, "null pointer", 0);
    { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_I; d.i = 5; check(d, 1, "integer", 0); }
    { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_N; d.slen = 1; d.ptr = &d; check(d, 1, "name", 0); }
    printf("RTX SNAME: %d cases, %d handled, %d declined, %d mismatches\n", ncase, handled, declined, fails);
    if (!handled || !declined) { printf("RTX SNAME UNIT: FAIL -- a battery that saw only one verdict proves nothing\n"); return 1; }
    printf("RTX SNAME UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
