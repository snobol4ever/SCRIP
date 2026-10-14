#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "descr.h"
#include "core.h"
#include "gc_heap.h"
DESCR_t rt_subscript_val(DESCR_t, DESCR_t);
DESCR_t c_rt_subscript_val(DESCR_t, DESCR_t);
extern int g_gc_pending;
static int fails = 0, ncase = 0, handled = 0, declined = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void words(DESCR_t d, uint64_t *w) { memcpy(&w[0], &d, 8); memcpy(&w[1], (char *)&d + 8, 8); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int leaf(DESCR_t b, DESCR_t k, DESCR_t *out) {
    uint64_t wb[2], wk[2];
    words(b, wb);
    words(k, wk);
    uint64_t rdi = wb[0], rsi = wb[1], rdx = wk[0], rcx = wk[1], rax;
    DESCR_t (*fp)(DESCR_t, DESCR_t) = rt_subscript_val;
    __asm__ volatile ("leaq -128(%%rsp), %%rsp; call *%[fn]; leaq 128(%%rsp), %%rsp" : "+D"(rdi), "+S"(rsi), "+d"(rdx), "+c"(rcx), "=a"(rax) : [fn] "r"(fp) : "r8", "r9", "r10", "r11", "memory", "cc");
    if ((uint8_t)rax == RTX_NOT_HANDLED) {
        if (rdi != wb[0] || rsi != wb[1] || rdx != wk[0] || rcx != wk[1]) { fails++; printf("  FAIL case %d: declined with an argument register changed\n", ncase); }
        return 0;
    }
    memcpy(out, &rax, 8);
    memcpy((char *)out + 8, &rdx, 8);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int same(DESCR_t a, DESCR_t b) {
    if (a.v == DT_SNUL && b.v == DT_SNUL) return a.slen == b.slen && a.s && b.s && a.s[0] == 0 && b.s[0] == 0;
    uint64_t wa[2], wb[2];
    words(a, wa);
    words(b, wb);
    return wa[0] == wb[0] && wa[1] == wb[1];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void check(const char *what, DESCR_t base, DESCR_t idx, int expect) {
    DESCR_t r, c;
    ncase++;
    g_gc_pending = 0;
    int h = leaf(base, idx, &r);
    if (h) {
        handled++;
        c = c_rt_subscript_val(base, idx);
        if (!same(r, c)) { fails++; printf("  FAIL %s: leaf %02x/%08x twin %02x/%08x\n", what, r.v & 0xff, r.slen, c.v & 0xff, c.slen); }
    } else declined++;
    if (expect == 1 && !h) { fails++; printf("  FAIL %s: declined a read the leaf owns\n", what); }
    if (expect == 0 && h) { fails++; printf("  FAIL %s: handled a read it must hand to the twin\n", what); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t dtab(TBBLK_t *t) { DESCR_t d = { .v = DT_T }; d.tbl = t; return d; }
static DESCR_t darr(ARBLK_t *a) { DESCR_t d = { .v = DT_A }; d.arr = a; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    static char keys[40][16];
    core_lib_init();
    TBBLK_t *t = table_new_args(64, 0), *td = table_new(), *tf = table_new(), *tn = table_new(), *te = table_new();
    for (int i = 0; i < 40; i++) { snprintf(keys[i], sizeof keys[0], "key%d", i); table_set_descr_d(t, STRVAL(keys[i]), INTVAL(i * 11)); table_set_descr_d(t, INTVAL(i - 20), STRVAL(keys[i])); }
    table_set_descr_d(t, REALVAL(2.5), INTVAL(25));
    table_set_descr_d(t, NULVCL, INTVAL(99));
    td->dflt = INTVAL(-1);
    tf->dflt = (DESCR_t){ .v = DT_FAIL };
    tn->dflt = NULVCL;
    table_set_descr_d(td, INTVAL(1), INTVAL(100));
    table_set_descr_d(te, INTVAL(7), NULVCL);
    DESCR_t varref = { .v = DT_N, .slen = 1 };
    DESCR_t cell = INTVAL(5);
    varref.ptr = &cell;
    DESCR_t pv = { .v = DT_N, .slen = 1 };
    pv.ptr = &cell;
    table_set_descr_d(t, INTVAL(500), pv);
    for (int i = 0; i < 40; i++) { check("string key hit", dtab(t), STRVAL(keys[i]), 1); check("integer key hit", dtab(t), INTVAL(i - 20), 1); }
    check("real key", dtab(t), REALVAL(2.5), 1);
    check("null key", dtab(t), NULVCL, 1);
    check("missing string key, no default", dtab(t), STRVAL("absent"), 1);
    check("missing integer key", dtab(t), INTVAL(99999), 1);
    check("value that is a varref (derefs in C)", dtab(t), INTVAL(500), 0);
    check("missing key, default", dtab(td), INTVAL(2), 1);
    check("present key, default table", dtab(td), INTVAL(1), 1);
    check("missing key, FAIL default", dtab(tf), INTVAL(2), 1);
    check("missing key, null default", dtab(tn), INTVAL(2), 1);
    check("stored null value", dtab(te), INTVAL(7), 1);
    check("empty-string key", dtab(t), STRVAL(""), 1);
    { DESCR_t nt = { .v = DT_T }; nt.tbl = NULL; check("null table pointer", nt, INTVAL(1), 0); }
    for (int lo = -3; lo <= 3; lo += 3) {
        ARBLK_t *a = array_new(lo, lo + 9);
        for (int i = lo; i <= lo + 9; i++) a->data[i - lo] = INTVAL(i * 5);
        a->data[3] = pv;
        for (int i = lo - 3; i <= lo + 12; i++) check("array index", darr(a), INTVAL(i), (i >= lo && i <= lo + 9) ? 1 : 0);
        check("array, string index", darr(a), STRVAL("3"), 0);
        check("array, real index", darr(a), REALVAL(3.0), 0);
    }
    { ARBLK_t *e = array_new(1, 0); check("empty array", darr(e), INTVAL(1), 0); check("empty array, zero", darr(e), INTVAL(0), 0); }
    { ARBLK_t *m = array_new2d(1, 3, 1, 3); check("two-dimensional array", darr(m), INTVAL(1), 0); }
    { ARBLK_t *a = array_new(1, 4); DESCR_t *sv = a->data; a->data = NULL; check("array without data", darr(a), INTVAL(1), 0); a->data = sv; }
    check("varref base", varref, INTVAL(1), 0);
    check("string base", STRVAL("hello"), INTVAL(2), 0);
    check("integer base", INTVAL(5), INTVAL(1), 0);
    check("null base", NULVCL, INTVAL(1), 0);
    printf("RTX SVAL: %d cases, %d handled, %d declined, %d mismatches\n", ncase, handled, declined, fails);
    if (handled < 100 || !declined) { printf("RTX SVAL UNIT: FAIL -- a battery that saw only one verdict proves nothing\n"); return 1; }
    printf("RTX SVAL UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
