#include "core.h"
#include <setjmp.h>
#include "sil_macros.h"
#include "rt/rt.h"
#include "rt/gc_heap.h"
#include "rk_opname.h"
#include "builtins/gen.h"
#include "builtins/gen_runtime.h"
#include "../parsers/prolog/prolog_atom.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define STACKLESS_ABORT(fn) \
    do { fprintf(stderr, "libscrip_rt: %s called — Icon value stack removed (GROUND ZERO 3). " \
                         "This box must be rebuilt stackless (per-box slot, no value stack).\n", (fn)); \
         abort(); } while (0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t coerce_numeric(DESCR_t v) {
    if (IS_STR(v)) {
        const char *s0 = rt_cstr_d(v), *s = s0;
        while (*s == ' ') s++;
        if (*s == '+' || *s == '-') s++;
        if (!*s) return INTVAL(0);
        const char *p = s;
        while (*p >= '0' && *p <= '9') p++;
        while (*p == ' ') p++;
        if (*p == '\0' && p > s)
            return INTVAL((int64_t)strtoll(s0, NULL, 10));
    }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_op_canon_base_c(const char *cat, const char *op, char *outbuf, int n) { rk_op_canon_base(cat, op, outbuf, (size_t)n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_binop_opstr(int op) {
    switch (op) { case BINOP_ADD: return "+"; case BINOP_SUB: return "-"; case BINOP_MUL: return "*"; case BINOP_DIV: return "/"; case BINOP_MOD: return "%"; case BINOP_POW: return "**";
                  case BINOP_POW_PROMOTE: return "**";
                  case BINOP_LT: return "<"; case BINOP_LE: return "<="; case BINOP_GT: return ">"; case BINOP_GE: return ">="; case BINOP_EQ: return "=="; case BINOP_NE: return "!=";
                  case BINOP_CONCAT: return "~"; case BINOP_SEQ: return "eq"; case BINOP_SNE: return "ne"; case BINOP_SLT: return "lt";
                  case BINOP_SLE: return "le"; case BINOP_SGT: return "gt"; case BINOP_SGE: return "ge"; default: return (const char *)0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_binop_overload(DESCR_t a, DESCR_t b, int op, DESCR_t *out) {
    if (a.v != DT_DATA && b.v != DT_DATA) return 0;
    const char *ops = rk_binop_opstr(op); if (!ops) return 0;
    char base[96]; rk_op_canon_base("infix", ops, base, sizeof base);
    extern int rt_proc_enum_count(void); extern const char *rt_proc_enum_name(int i);
    char prefix[112]; int pl = snprintf(prefix, sizeof prefix, "%s$", base);
    int found = 0, pcount = rt_proc_enum_count();
    for (int pi = 0; pi < pcount; pi++) { const char *pn = rt_proc_enum_name(pi); if (pn && !strncmp(pn, prefix, (size_t)pl)) { found = 1; break; } }
    if (!found) return 0;
    extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
    DESCR_t args[3]; args[0] = STRVAL(rt_str_dup(base)); args[1] = a; args[2] = b;
    DESCR_t r = FAILDESCR; if (script_try_call_builtin_by_name("__multi_call", args, 3, &r)) { *out = r; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_relop_overload(DESCR_t a, DESCR_t b, int op, DESCR_t *out) {
    if (a.v != DT_DATA && b.v != DT_DATA) return 0;
    DESCR_t r; if (!rt_binop_overload(a, b, op, &r)) return 0;
    *out = r; extern int rt_is_truthy(DESCR_t v); return rt_is_truthy(r) ? 2 : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_relop_val_coerce(DESCR_t a, DESCR_t b, DESCR_t *out) {
    *out = (IS_REAL(a) && !IS_REAL(b)) ? REALVAL(to_real(b)) : b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t add(DESCR_t a, DESCR_t b) {
    if (IS_FAIL(a) || IS_FAIL(b)) return FAILDESCR;
    if (IS_NULL(a)) a = INTVAL(0);
    if (IS_NULL(b)) b = INTVAL(0);
    a = coerce_numeric(a); b = coerce_numeric(b);
    if (IS_INT(a) && IS_INT(b))
        return INTVAL(a.i + b.i);
    return REALVAL(to_real(a) + to_real(b));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t sub(DESCR_t a, DESCR_t b) {
    if (IS_FAIL(a) || IS_FAIL(b)) return FAILDESCR;
    if (IS_NULL(a)) a = INTVAL(0);
    if (IS_NULL(b)) b = INTVAL(0);
    a = coerce_numeric(a); b = coerce_numeric(b);
    if (IS_INT(a) && IS_INT(b))
        return INTVAL(a.i - b.i);
    return REALVAL(to_real(a) - to_real(b));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t mul(DESCR_t a, DESCR_t b) {
    if (IS_FAIL(a) || IS_FAIL(b)) return FAILDESCR;
    a = coerce_numeric(a); b = coerce_numeric(b);
    if (IS_INT(a) && IS_INT(b))
        return INTVAL(a.i * b.i);
    return REALVAL(to_real(a) * to_real(b));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t neg(DESCR_t a) {
    if (IS_FAIL(a)) return FAILDESCR;
    if (IS_INT(a))  return INTVAL(-a.i);
    if (IS_REAL(a)) return REALVAL(-a.r);
    return INTVAL(-to_int(a));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pos(DESCR_t a) {
    if (IS_FAIL(a))  return FAILDESCR;
    if (IS_INT(a))   return a;
    if (IS_REAL(a))  return a;
    return INTVAL(to_int(a));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lt(DESCR_t a, DESCR_t b) {
    if (IS_INT(a) && IS_INT(b)) return a.i < b.i;
    return to_real(a) < to_real(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int le(DESCR_t a, DESCR_t b) {
    if (IS_INT(a) && IS_INT(b)) return a.i <= b.i;
    return to_real(a) <= to_real(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int gt(DESCR_t a, DESCR_t b) {
    if (IS_INT(a) && IS_INT(b)) return a.i > b.i;
    return to_real(a) > to_real(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ge(DESCR_t a, DESCR_t b) {
    if (IS_INT(a) && IS_INT(b)) return a.i >= b.i;
    return to_real(a) >= to_real(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_arith(int lk, long li, const char *ls,
                  int rk, long ri, const char *rs, const char *op)
{
    (void)ls; (void)rs; (void)lk; (void)rk;
    long lv = li;
    long rv = ri;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_ipow_descr(int64_t li, int64_t ri) {
    if (li == 0 && ri <= 0) { extern int core_icn_error(int code, DESCR_t val); core_icn_error(204, INTVAL(li)); return FAILDESCR; }
    if (ri >= 0) { int64_t acc = 1; for (int64_t k = 0; k < ri; k++) if (__builtin_mul_overflow(acc, li, &acc)) { extern DESCR_t rt_big_pow(DESCR_t, int64_t); return rt_big_pow(INTVAL(li), ri); } return INTVAL(acc); }
    if (li == 1) return INTVAL(1);
    if (li == -1) return INTVAL((ri & 1) ? -1 : 1);
    return INTVAL(0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_ipow_promote_descr(int64_t li, int64_t ri) {
    if (ri >= 0) { int64_t acc = 1; for (int64_t k = 0; k < ri; k++) acc *= li; return INTVAL(acc); }
    if (li == 0) return REALVAL(0.0);
    return REALVAL(pow((double)li, (double)ri));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int operand_is_real_str(DESCR_t v) {
    if (!IS_STR_fn(v) || !v.s) return 0;
    const char *s = v.s; while (*s == ' ') s++; if (!*s) return 0;
    if (rt_plain_int_str(s)) return 0;
    char *endi = 0, *endd = 0; strtoll(s, &endi, 10); strtod(s, &endd);
    if (endd <= endi) return 0; while (*endd == ' ') endd++; return *endd == '\0';
}
static DESCR_t rt_num_arith_impl(DESCR_t a, DESCR_t b, int op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op) {
    extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n;
    if (a.v == DT_I && b.v == DT_I) {
        int64_t _z;
        switch (op) {
            case BINOP_ADD: if (!__builtin_add_overflow(a.i, b.i, &_z)) return INTVAL(_z); break;
            case BINOP_SUB: if (!__builtin_sub_overflow(a.i, b.i, &_z)) return INTVAL(_z); break;
            case BINOP_MUL: if (!__builtin_mul_overflow(a.i, b.i, &_z)) return INTVAL(_z); break;
            case BINOP_DIV: if (b.i == 0) return FAILDESCR; if (b.i != -1) return INTVAL(a.i / b.i); break;
            case BINOP_MOD: if (b.i == 0) return FAILDESCR; if (b.i != -1) return INTVAL(a.i % b.i); break;
            default: break;
        }
    }
    if (g_core_errjmp_n >= 64) return rt_num_arith_impl(a, b, op);
    int my = g_core_errjmp_n;
    if (setjmp(g_core_errjmp_stk[my])) { g_core_errjmp_n = my; return FAILDESCR; }
    g_core_errjmp_n = my + 1;
    DESCR_t r = rt_num_arith_impl(a, b, op);
    g_core_errjmp_n = my;
    return r;
}
#define RT_BINOP_ENTRY(fn, code, fast) \
DESCR_t fn(DESCR_t a, DESCR_t b) { \
    extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n; \
    if (a.v == DT_I && b.v == DT_I) { fast } \
    if (a.v == DT_DATA || b.v == DT_DATA) { DESCR_t ov; if (rt_binop_overload(a, b, code, &ov)) return ov; } \
    if (g_core_errjmp_n >= 64) return rt_num_arith_impl(a, b, code); \
    int my = g_core_errjmp_n; \
    if (setjmp(g_core_errjmp_stk[my])) { g_core_errjmp_n = my; return FAILDESCR; } \
    g_core_errjmp_n = my + 1; \
    DESCR_t r = rt_num_arith_impl(a, b, code); \
    g_core_errjmp_n = my; \
    return r; \
}
RT_BINOP_ENTRY(c_rt_add,  BINOP_ADD,    { int64_t _z; if (!__builtin_add_overflow(a.i, b.i, &_z)) return INTVAL(_z); })
RT_BINOP_ENTRY(c_rt_sub,  BINOP_SUB,    { int64_t _z; if (!__builtin_sub_overflow(a.i, b.i, &_z)) return INTVAL(_z); })
RT_BINOP_ENTRY(c_rt_mul,  BINOP_MUL,    { int64_t _z; if (!__builtin_mul_overflow(a.i, b.i, &_z)) return INTVAL(_z); })
RT_BINOP_ENTRY(rt_div,    BINOP_DIV,    if (b.i == 0) { core_runtime_error(2, "division caused integer overflow"); return FAILDESCR; } if (b.i != -1) return INTVAL(a.i / b.i);)
RT_BINOP_ENTRY(rt_mod,    BINOP_MOD,    if (b.i == 0) { core_runtime_error(2, NULL); return FAILDESCR; } if (b.i != -1) return INTVAL(a.i % b.i);)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_add_big(DESCR_t a, DESCR_t b) { extern DESCR_t rt_big_add(DESCR_t, DESCR_t);
    if (a.v == DT_I && b.v == DT_I) { int64_t _z; if (!__builtin_add_overflow(a.i, b.i, &_z)) return INTVAL(_z); return rt_big_add(a, b); }
    return rt_num_arith_impl(a, b, BINOP_ADD); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_sub_big(DESCR_t a, DESCR_t b) { extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
    if (a.v == DT_I && b.v == DT_I) { int64_t _z; if (!__builtin_sub_overflow(a.i, b.i, &_z)) return INTVAL(_z); return rt_big_sub(a, b); }
    return rt_num_arith_impl(a, b, BINOP_SUB); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_mul_big(DESCR_t a, DESCR_t b) { extern DESCR_t rt_big_mul(DESCR_t, DESCR_t);
    if (a.v == DT_I && b.v == DT_I) { int64_t _z; if (!__builtin_mul_overflow(a.i, b.i, &_z)) return INTVAL(_z); return rt_big_mul(a, b); }
    return rt_num_arith_impl(a, b, BINOP_MUL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_BINOP_ENTRY(rt_pow,    BINOP_POW,    )
RT_BINOP_ENTRY(rt_powreal, BINOP_POW_PROMOTE, )
RT_BINOP_ENTRY(rt_cunion, BINOP_CUNION, )
RT_BINOP_ENTRY(rt_cdiff,  BINOP_CDIFF,  )
RT_BINOP_ENTRY(rt_cinter, BINOP_CINTER, )
static DESCR_t rt_big_arith_route(DESCR_t a, DESCR_t b, int op) {
    extern DESCR_t rt_big_add(DESCR_t, DESCR_t); extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_mul(DESCR_t, DESCR_t); extern DESCR_t rt_big_pow(DESCR_t, int64_t);
    switch (op) {
        case BINOP_ADD: return rt_big_add(a, b);
        case BINOP_SUB: return rt_big_sub(a, b);
        case BINOP_MUL: return rt_big_mul(a, b);
        case BINOP_POW: case BINOP_POW_PROMOTE: return (b.v == DT_I && b.i >= 0) ? rt_big_pow(a, b.i) : FAILDESCR;
        default: return FAILDESCR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_big_arith_wanted(DESCR_t a, DESCR_t b, int op) {
    if (a.v == DT_BIG || b.v == DT_BIG) return (op == BINOP_ADD || op == BINOP_SUB || op == BINOP_MUL || op == BINOP_POW || op == BINOP_POW_PROMOTE);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_num_arith_impl(DESCR_t a, DESCR_t b, int op) {
    if (rt_big_arith_wanted(a, b, op)) return rt_big_arith_route(a, b, op);
    int csop = (op == BINOP_CUNION || op == BINOP_CDIFF || op == BINOP_CINTER);
    if (!csop && (a.v == DT_S || a.v == DT_SNUL) && (!a.s || descr_slen(a) == 0)) a = INTVAL(0);
    if (!csop && (b.v == DT_S || b.v == DT_SNUL) && (!b.s || descr_slen(b) == 0)) b = INTVAL(0);
    if (!csop && (!is_numeric_like(a) || !is_numeric_like(b))) return FAILDESCR;
    int lf = IS_REAL_fn(a), rf = IS_REAL_fn(b);
    int anyf = lf || rf || operand_is_real_str(a) || operand_is_real_str(b);
    double ld = csop ? 0.0 : to_real(a), rd = csop ? 0.0 : to_real(b);
    int64_t li = csop ? 0 : to_int(a), ri = csop ? 0 : to_int(b);
    switch (op) {
        case BINOP_ADD: return anyf ? REALVAL(ld + rd) : INTVAL(li + ri);
        case BINOP_SUB: return anyf ? REALVAL(ld - rd) : INTVAL(li - ri);
        case BINOP_MUL: return anyf ? REALVAL(ld * rd) : INTVAL(li * ri);
        case BINOP_DIV: if (anyf) return (rd == 0.0) ? FAILDESCR : REALVAL(ld / rd); if (ri == 0) return FAILDESCR; return INTVAL(li / ri);
        case BINOP_MOD: if (anyf) return (rd == 0.0) ? FAILDESCR : REALVAL(fmod(ld, rd)); if (ri == 0) return FAILDESCR; return INTVAL(li % ri);
        case BINOP_POW: {
            extern int core_icn_error(int code, DESCR_t val);
            if (!anyf) return rt_ipow_descr(li, ri);
            if (ld == 0.0 && rd <= 0.0) { core_icn_error(204, REALVAL(ld)); return FAILDESCR; }
            if (ld < 0.0) { core_icn_error(206, REALVAL(ld)); return FAILDESCR; }
            { double _rp = pow(ld, rd); if (!isfinite(_rp)) { core_icn_error(204, REALVAL(ld)); return FAILDESCR; } return REALVAL(_rp); }
        }
        case BINOP_POW_PROMOTE: return anyf ? REALVAL(pow(ld, rd)) : rt_ipow_promote_descr(li, ri);
        case BINOP_CUNION: case BINOP_CDIFF: case BINOP_CINTER: {
            extern const char *icon_real_str(double r, char *buf, int bufsz);
            char _ab[64], _bb[64]; const char *as, *bs;
            if (a.v == DT_T && a.tbl && a.tbl->is_set && b.v == DT_T && b.tbl && b.tbl->is_set) {
                if (op == BINOP_CUNION) return TABLE_VAL(set_union(a.tbl, b.tbl));
                if (op == BINOP_CDIFF)  return TABLE_VAL(set_diff(a.tbl, b.tbl));
                return TABLE_VAL(set_inter(a.tbl, b.tbl));
            }
            if (IS_CSET_fn(a) || a.v == DT_S || a.v == DT_SNUL) as = a.s ? a.s : "";
            else if (IS_INT_fn(a))  { snprintf(_ab, sizeof _ab, "%lld", (long long)a.i); as = _ab; }
            else if (IS_REAL_fn(a)) { icon_real_str(a.r, _ab, sizeof _ab); as = _ab; }
            else { as = VARVAL_fn(a); if (!as) as = ""; }
            if (IS_CSET_fn(b) || b.v == DT_S || b.v == DT_SNUL) bs = b.s ? b.s : "";
            else if (IS_INT_fn(b))  { snprintf(_bb, sizeof _bb, "%lld", (long long)b.i); bs = _bb; }
            else if (IS_REAL_fn(b)) { icon_real_str(b.r, _bb, sizeof _bb); bs = _bb; }
            else { bs = VARVAL_fn(b); if (!bs) bs = ""; }
            extern int kw_cset_len(const char *);
            int aslen = IS_CSET_fn(a) ? kw_cset_len(as) : -1; if (aslen < 0) aslen = (int)strlen(as);
            int bslen = IS_CSET_fn(b) ? kw_cset_len(bs) : -1; if (bslen < 0) bslen = (int)strlen(bs);
            int outlen; const char *ur;
            if (op == BINOP_CUNION) { ur = cset_union(as, aslen, bs, bslen, &outlen); return CSETVAL(cset_canonical(ur, outlen)); }
            if (op == BINOP_CDIFF)  { ur = cset_diff(as, aslen, bs, bslen, &outlen); return CSETVAL(cset_canonical(ur, outlen)); }
            ur = cset_inter(as, aslen, bs, bslen, &outlen); return CSETVAL(cset_canonical(ur, outlen));
        }
        default: return anyf ? REALVAL(ld + rd) : INTVAL(li + ri);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_cset_compl(DESCR_t a) {
    extern const char *icon_real_str(double r, char *buf, int bufsz);
    char _cbuf[64]; const char *raw;
    if (IS_FAIL_fn(a)) return FAILDESCR;
    if (IS_CSET_fn(a)) raw = a.s ? a.s : "";
    else if (IS_INT_fn(a))  { snprintf(_cbuf, sizeof _cbuf, "%lld", (long long)a.i); raw = _cbuf; }
    else if (IS_REAL_fn(a)) { icon_real_str(a.r, _cbuf, sizeof _cbuf); raw = _cbuf; }
    else { raw = VARVAL_fn(a); if (!raw) raw = ""; }
    extern int kw_cset_len(const char *);
    int rawlen = IS_CSET_fn(a) ? kw_cset_len(raw) : -1; if (rawlen < 0) rawlen = (int)strlen(raw);
    unsigned char in[256] = {0}; for (int _i = 0; _i < rawlen; _i++) in[(unsigned char)raw[_i]] = 1;
    char *outs = rt_str_alloc(255); int n = 0; for (int c = 1; c < 256; c++) if (!in[c]) outs[n++] = (char)c; outs[n] = 0;
    return CSETVAL(cset_canonical(outs, n));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_num_neg(DESCR_t a) {
    if (!is_numeric_like(a)) { core_runtime_error(1, "negation operand is not numeric"); return FAILDESCR; }
    if (IS_REAL_fn(a) || operand_is_real_str(a)) return REALVAL(-to_real(a));
    return INTVAL(-to_int(a));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_num_pos(DESCR_t a) {
    if (!is_numeric_like(a)) { core_runtime_error(1, "affirmation operand is not numeric"); return FAILDESCR; }
    if (IS_REAL_fn(a) || operand_is_real_str(a)) return REALVAL(to_real(a));
    return INTVAL(to_int(a));
}
