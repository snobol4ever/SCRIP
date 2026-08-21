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
