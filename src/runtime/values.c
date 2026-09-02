#include "core.h"
#include "builtins/gen_runtime.h"
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int descr_identical(DESCR_t a, DESCR_t b) {
    if (IS_FAIL_fn(a) || IS_FAIL_fn(b)) return 0;
    int an = (a.v == DT_SNUL) || (a.v == DT_S && (!a.s || (a.slen == 0xFFFFFFFFu ? !*a.s : a.slen == 0)));
    int bn = (b.v == DT_SNUL) || (b.v == DT_S && (!b.s || (b.slen == 0xFFFFFFFFu ? !*b.s : b.slen == 0)));
    if (an && bn) return 1;
    if (an != bn) return 0;
    int as_str = (a.v == DT_S || a.v == DT_SNUL);
    int bs_str = (b.v == DT_S || b.v == DT_SNUL);
    if (as_str && bs_str) {
        const char *s1 = a.s ? a.s : ""; size_t l1 = (a.slen != 0xFFFFFFFFu) ? (size_t)a.slen : __builtin_strlen(s1);
        const char *s2 = b.s ? b.s : ""; size_t l2 = (b.slen != 0xFFFFFFFFu) ? (size_t)b.slen : __builtin_strlen(s2);
        return (l1 == l2 && memcmp(s1, s2, l1) == 0);
    }
    int a_cset = (a.v == DT_S && a.slen == 0xFFFFFFFFu);
    int b_cset = (b.v == DT_S && b.slen == 0xFFFFFFFFu);
    if (a_cset != b_cset) return 0;
    if (a.v != b.v) return 0;
    if (a.v == DT_N) {
        if (a.slen == 0 && b.slen == 0) return (a.s && b.s) ? (strcmp(a.s, b.s) == 0) : (a.s == b.s);
        if (a.slen != b.slen) return 0;
        return a.p == b.p;
    }
    if (a.v == DT_I) return a.i == b.i;
    if (a.v == DT_R) return a.r == b.r;
    if (a.v == DT_T) return a.tbl == b.tbl;
    if (a.v == DT_DATA) return a.ptr == b.ptr;
    return memcmp(&a, &b, sizeof(DESCR_t)) == 0;
}
