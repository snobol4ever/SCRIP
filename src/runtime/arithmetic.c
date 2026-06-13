#include "core.h"
#include "sil_macros.h"
#include "rt/rt.h"
#include "builtins/resolution.h"
#include "../parser/prolog/prolog_atom.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define STACKLESS_ABORT(fn) \
    do { fprintf(stderr, "libscrip_rt: %s called — Icon value stack removed (GROUND ZERO 3). " \
                         "This box must be rebuilt stackless (per-box slot, no value stack).\n", (fn)); \
         abort(); } while (0)
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
    if (IS_NULL(a)) a = INTVAL(0);
    if (IS_NULL(b)) b = INTVAL(0);
    a = coerce_numeric(a); b = coerce_numeric(b);
    if (IS_INT(a) && IS_INT(b) && b.i >= 0) {
        if (a.i == 0 && b.i == 0) { core_runtime_error(2, NULL); return FAILDESCR; }
        int64_t acc = 1;
        for (int64_t k = 0; k < b.i; k++) acc *= a.i;
        return INTVAL(acc);
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
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_coerce_num(void)
{
    STACKLESS_ABORT("rt_coerce_num");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_exp(void)
{
    STACKLESS_ABORT("rt_exp");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_neg(void)
{
    STACKLESS_ABORT("rt_neg");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_incr(int64_t n)
{
    (void)n;
    STACKLESS_ABORT("rt_incr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_decr(int64_t n)
{
    (void)n;
    STACKLESS_ABORT("rt_decr");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_acomp(int op)
{
    (void)op;
    STACKLESS_ABORT("rt_acomp");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void rt_lcomp(int op)
{
    (void)op;
    STACKLESS_ABORT("rt_lcomp");
}
/*--------------------------------------------------------------------------------------------------------------------*/
long rt_arith(int lk, long li, const char *ls,
                  int rk, long ri, const char *rs, const char *op)
{
    extern Term **g_resolve_env;
    (void)ls; (void)rs;
    long lv = li;
    if (lk == IR_LOGICVAR && g_resolve_env && li >= 0) {
        Term *t = g_resolve_env[li] ? term_deref(g_resolve_env[li]) : NULL;
        if (t && t->tag == TERM_INT) lv = t->ival;
    }
    long rv = ri;
    if (rk == IR_LOGICVAR && g_resolve_env && ri >= 0) {
        Term *t = g_resolve_env[ri] ? term_deref(g_resolve_env[ri]) : NULL;
        if (t && t->tag == TERM_INT) rv = t->ival;
    }
    if (!op) return lv + rv;
    if (strcmp(op, "sign")==0)     return (lv > 0) ? 1 : (lv < 0) ? -1 : 0;
    if (strcmp(op, "abs")==0)      return (lv < 0) ? -lv : lv;
    if (strcmp(op, "truncate")==0) return lv;
    if (strcmp(op, "integer")==0)  return lv;
    if (strcmp(op, "round")==0)    return lv;
    if (strcmp(op, "ceiling")==0)  return lv;
    if (strcmp(op, "floor")==0)    return lv;
    if (strcmp(op, "\\")==0)       return ~lv;
    if (strcmp(op, "msb")==0)      { long v=lv, m=-1; while(v){v>>=1;m++;} return m; }
    if (strcmp(op, "**") == 0)   { long r=1; for(long i=0;i<rv;i++) r*=lv; return r; }
    if (strcmp(op, "^") == 0)    { long r=1; for(long i=0;i<rv;i++) r*=lv; return r; }
    if (strcmp(op, "/\\") == 0)  return lv & rv;
    if (strcmp(op, "\\/") == 0)  return lv | rv;
    if (strcmp(op, "xor") == 0)  return lv ^ rv;
    if (strcmp(op, ">>") == 0)   return lv >> rv;
    if (strcmp(op, "<<") == 0)   return lv << rv;
    if (strcmp(op, "mod") == 0)  return rv ? lv % rv : 0;
    if (strcmp(op, "rem") == 0)  return rv ? lv % rv : 0;
    if (strcmp(op, "gcd") == 0)  { long a = lv<0?-lv:lv, b = rv<0?-rv:rv; while (b) { long r = a % b; a = b; b = r; } return a; }
    if (strcmp(op, "div") == 0)  { if (!rv) return 0; long q = lv / rv; if ((lv % rv != 0) && ((lv < 0) != (rv < 0))) q--; return q; }
    if (strcmp(op, "max") == 0)  return lv > rv ? lv : rv;
    if (strcmp(op, "min") == 0)  return lv < rv ? lv : rv;
    if (strcmp(op, "//") == 0)   return rv ? lv / rv : 0;
    if (op[0] == '+' && op[1] == '\0') return lv + rv;
    if (op[0] == '-' && op[1] == '\0') return lv - rv;
    if (op[0] == '*' && op[1] == '\0') return lv * rv;
    if (op[0] == '/' && op[1] == '\0') return rv ? lv / rv : 0;
    if (op[0] == '%' && op[1] == '\0') return rv ? lv % rv : 0;
    return lv + rv;
}
