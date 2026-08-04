/* rtx_varval_test.c — differential battery: asm VARVAL_fn vs c_VARVAL_fn.
 *
 * VARVAL_fn is comparable arm-for-arm in a way the allocator is not, and the two arms
 * demand DIFFERENT comparisons — which is the whole point of this battery:
 *
 *   THE PORTED ARM (DT_S, .s non-NULL) MUST RETURN THE CALLER'S OWN POINTER. The C reads
 *   `return v.s ? v.s : ...`, so it hands back the identical address and allocates nothing.
 *   Comparing BYTES there would pass even if the asm had quietly strdup'd, and a silent
 *   strdup on a path taken 20,000,006 times in one benchmark run is exactly the regression
 *   this file exists to catch. So the fast arm is checked with POINTER IDENTITY, not strcmp.
 *
 *   THE DELEGATED ARMS mint fresh storage on every call, so asm and C legitimately return
 *   DIFFERENT addresses and only the BYTES are comparable. Those are checked with strcmp.
 *
 * The integer and real cases are the SPITBOL Ch.3 conversion rules, and they are the reason
 * those arms stay in C rather than becoming asm: integer->string suppresses leading zeros and
 * carries a sign with zero rendering as "0", and real->string is the standard representation
 * with a mandatory decimal point. Both PRODUCE a string, so both must allocate.
 *
 * DT_N / DT_K / populated DT_A / DT_T / DT_DATA are deliberately ABSENT. They dereference
 * their payload (and DT_N/DT_K recurse through rt_deref / NV_GET_fn), so fabricating one here
 * would test the fixture rather than the port. Their NULL-payload guards ARE covered, because
 * those the C answers without dereferencing anything.
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "descr.h"
char *VARVAL_fn(DESCR_t v);
char *c_VARVAL_fn(DESCR_t v);
static int fails = 0, n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bytes_agree(const char *what, DESCR_t v) {
    char *a = VARVAL_fn(v), *c = c_VARVAL_fn(v); n++;
    if (!a || !c) { if (a != c) { fails++; printf("  FAIL %-22s NULL mismatch asm=%p c=%p\n", what, (void *)a, (void *)c); } return; }
    if (strcmp(a, c) != 0) { fails++; printf("  FAIL %-22s asm=\"%s\" c=\"%s\"\n", what, a, c); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pointer_is_identity(const char *what, DESCR_t v) {
    char *a = VARVAL_fn(v), *c = c_VARVAL_fn(v); n++;
    if (a != v.s) { fails++; printf("  FAIL %-22s asm did not return the caller's pointer (%p vs %p)\n", what, (void *)a, (void *)v.s); return; }
    if (c != v.s) { fails++; printf("  FAIL %-22s C did not return the caller's pointer (%p vs %p)\n", what, (void *)c, (void *)v.s); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t ds(char *s)    { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_S;  d.s = s; d.slen = s ? (unsigned)strlen(s) : 0u; return d; }
static DESCR_t di(int64_t i)  { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_I;  d.i = i; return d; }
static DESCR_t dr(double r)   { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_R;  d.r = r; return d; }
static DESCR_t dtag(int t)    { DESCR_t d; memset(&d, 0, sizeof d); d.v = (DTYPE_t)t;     return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    static char abc[] = "abc", empty[] = "", longs[] = "the quick brown fox jumps over the lazy dog 0123456789";
    printf("RTX STR differential battery (asm VARVAL_fn vs c_VARVAL_fn)\n");
    pointer_is_identity("DT_S abc",        ds(abc));
    pointer_is_identity("DT_S empty",      ds(empty));
    pointer_is_identity("DT_S long",       ds(longs));
    bytes_agree("DT_S NULL .s",            ds(NULL));
    bytes_agree("DT_SNUL",                 dtag(DT_SNUL));
    bytes_agree("DT_I zero",               di(0));
    bytes_agree("DT_I positive",           di(4294967296LL));
    bytes_agree("DT_I negative",           di(-23));
    bytes_agree("DT_I INT64_MIN",          di(INT64_MIN));
    bytes_agree("DT_R simple",             dr(12.1));
    bytes_agree("DT_R exponent",           dr(123456789.012));
    bytes_agree("DT_R negative",           dr(-0.5));
    bytes_agree("DT_R zero",               dr(0.0));
    bytes_agree("DT_P",                    dtag(DT_P));
    bytes_agree("DT_E",                    dtag(DT_E));
    bytes_agree("DT_C",                    dtag(DT_C));
    bytes_agree("DT_A null payload",       dtag(DT_A));
    bytes_agree("DT_T null payload",       dtag(DT_T));
    bytes_agree("DT_DATA null payload",    dtag(DT_DATA));
    bytes_agree("DT_FAIL (default arm)",   dtag(DT_FAIL));
    printf("RTX STR VARVAL: %d cases, %d mismatches\n", n, fails);
    printf("RTX STR VARVAL UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
