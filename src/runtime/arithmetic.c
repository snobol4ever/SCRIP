#include "core.h"
#include "sil_macros.h"
#include <stdlib.h>
#include <math.h>
/*====================================================================================================================*/
static DESCR_t coerce_numeric(DESCR_t v) {
    if (IS_STR(v)) {
        const char *s = v.s ? v.s : "";
        while (*s == ' ') s++;
        if (*s == '+' || *s == '-') s++;
        if (!*s) return INTVAL(0);
        const char *p = s;
        while (*p >= '0' && *p <= '9') p++;
        while (*p == ' ') p++;
        if (*p == '\0' && p > s)
            return INTVAL((int64_t)strtoll(v.s ? v.s : "", NULL, 10));
    }
    return v;
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t add(DESCR_t a, DESCR_t b) {
    if (IS_FAIL(a) || IS_FAIL(b)) return FAILDESCR;
    if (IS_NULL(a)) a = INTVAL(0);
    if (IS_NULL(b)) b = INTVAL(0);
    a = coerce_numeric(a); b = coerce_numeric(b);
    if (IS_INT(a) && IS_INT(b))
        return INTVAL(a.i + b.i);
    return REALVAL(to_real(a) + to_real(b));
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t sub(DESCR_t a, DESCR_t b) {
    if (IS_FAIL(a) || IS_FAIL(b)) return FAILDESCR;
    if (IS_NULL(a)) a = INTVAL(0);
    if (IS_NULL(b)) b = INTVAL(0);
    a = coerce_numeric(a); b = coerce_numeric(b);
    if (IS_INT(a) && IS_INT(b))
        return INTVAL(a.i - b.i);
    return REALVAL(to_real(a) - to_real(b));
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t mul(DESCR_t a, DESCR_t b) {
    if (IS_FAIL(a) || IS_FAIL(b)) return FAILDESCR;
    a = coerce_numeric(a); b = coerce_numeric(b);
    if (IS_INT(a) && IS_INT(b))
        return INTVAL(a.i * b.i);
    return REALVAL(to_real(a) * to_real(b));
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t DIVIDE_fn(DESCR_t a, DESCR_t b) {
    if (IS_FAIL(a) || IS_FAIL(b)) return FAILDESCR;
    if (IS_INT(a) && IS_INT(b)) {
        if (b.i == 0) { core_runtime_error(2, NULL); return FAILDESCR; }
        return INTVAL(a.i / b.i);
    }
    double denom = to_real(b);
    if (denom == 0.0) { core_runtime_error(2, NULL); return FAILDESCR; }
    return REALVAL(to_real(a) / denom);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t POWER_fn(DESCR_t a, DESCR_t b) {
    if (IS_FAIL(a) || IS_FAIL(b)) return FAILDESCR;
    if (IS_INT(a) && IS_INT(b)) {
        int64_t ix = a.i, iy = b.i;
        if (ix == 0 && iy < 0) { core_runtime_error(2, NULL); return FAILDESCR; }
        if (iy < 0) return INTVAL(0);
        int64_t p = 1;
        for (;;) {
            if (iy & 1) p *= ix;
            iy >>= 1;
            if (iy == 0) break;
            ix *= ix;
        }
        return INTVAL(p);
    }
    double r = pow(to_real(a), to_real(b));
    if (isinf(r) || isnan(r)) { core_runtime_error(2, NULL); return FAILDESCR; }
    return REALVAL(r);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t neg(DESCR_t a) {
    if (IS_FAIL(a)) return FAILDESCR;
    if (IS_INT(a))  return INTVAL(-a.i);
    if (IS_REAL(a)) return REALVAL(-a.r);
    return INTVAL(-to_int(a));
}
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t pos(DESCR_t a) {
    if (IS_FAIL(a))  return FAILDESCR;
    if (IS_INT(a))   return a;
    if (IS_REAL(a))  return a;
    return INTVAL(to_int(a));
}
/*--------------------------------------------------------------------------------------------------------------------*/
int eq(DESCR_t a, DESCR_t b) {
    if (IS_INT(a) && IS_INT(b)) return a.i == b.i;
    return to_real(a) == to_real(b);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int ne(DESCR_t a, DESCR_t b) { return !eq(a, b); }
int lt(DESCR_t a, DESCR_t b) {
    if (IS_INT(a) && IS_INT(b)) return a.i < b.i;
    return to_real(a) < to_real(b);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int le(DESCR_t a, DESCR_t b) {
    if (IS_INT(a) && IS_INT(b)) return a.i <= b.i;
    return to_real(a) <= to_real(b);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int gt(DESCR_t a, DESCR_t b) {
    if (IS_INT(a) && IS_INT(b)) return a.i > b.i;
    return to_real(a) > to_real(b);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int ge(DESCR_t a, DESCR_t b) {
    if (IS_INT(a) && IS_INT(b)) return a.i >= b.i;
    return to_real(a) >= to_real(b);
}
