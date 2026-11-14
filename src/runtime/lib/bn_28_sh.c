#include "runtime/by_name_dispatch.h"
#include <math.h>
#include "runtime/pl_rat.h"
#include <fenv.h>
#include <float.h>
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_28_sh.h"
#include <time.h>
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/builtins/gen.h"
#include "ir/descr_tags_dt_platom.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_op_code(const char *s) {
    if (!strcmp(s, "add")) return BINOP_ADD;
    if (!strcmp(s, "sub")) return BINOP_SUB;
    if (!strcmp(s, "mul")) return BINOP_MUL;
    if (!strcmp(s, "div")) return BINOP_DIV;
    if (!strcmp(s, "idiv")) return BINOP_DIV;
    if (!strcmp(s, "mod")) return BINOP_MOD;
    if (!strcmp(s, "pow")) return BINOP_POW;
    return BINOP_ADD;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_arith2(const char *op, DESCR_t a, DESCR_t b) {
    extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
    int ai = (a.v == DT_I), bi = (b.v == DT_I);
    if (ai && bi && b.i == -1) return rt_num_arith(a, b, pl_is_op_code(op));
    if (ai && bi && !strcmp(op, "idiv")) { if (b.i == 0) return FAILDESCR; return INTVAL(a.i / b.i); }
    if (ai && bi && !strcmp(op, "div")) { if (b.i == 0) return FAILDESCR; if (a.i % b.i == 0) return INTVAL(a.i / b.i); return REALVAL((double)a.i / (double)b.i); }
    if (ai && bi && !strcmp(op, "mod")) { if (b.i == 0) return FAILDESCR; long long m = a.i % b.i; if (m && ((m < 0) != (b.i < 0))) m += b.i; return INTVAL(m); }
    return rt_num_arith(a, b, pl_is_op_code(op));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ax_float_to_int(double r, DESCR_t *out) {
    extern DESCR_t rt_big_from_str(const char *);
    if (r >= -9223372036854775808.0 && r < 9223372036854775808.0) { *out = INTVAL((long long)r); return 1; }
    if (r != r || r - r != 0.0) return 0;
    char nb[snprintf((char *)0, 0, "%.0f", r) + 1];
    snprintf(nb, sizeof nb, "%.0f", r);
    *out = rt_big_from_str(nb);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ax_swi_round(double d, DESCR_t *out) { if (d == 9223372036854775808.0) { *out = INTVAL(LLONG_MAX); return 1; } return pl_ax_float_to_int(round(d), out); }
int dop_ax_core(const char *op, DESCR_t *args, int nargs, DESCR_t *out, void **ball) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
    extern void *rt_pl_ball_eval_error(const char *, const char *, int);
    if (nargs == 0) {
        if (!strcmp(op, "pi")) { *out = REALVAL(M_PI); return 1; }
        if (!strcmp(op, "e")) { *out = REALVAL(M_E); return 1; }
        if (!strcmp(op, "eps")) { *out = REALVAL(DBL_EPSILON); return 1; }
        if (!strcmp(op, "inf")) { *out = REALVAL(HUGE_VAL); return 1; }
        if (!strcmp(op, "nan")) { *out = REALVAL(NAN); return 1; }
        if (!strcmp(op, "rflt")) { *out = REALVAL(drand48()); return 1; }
        if (!strcmp(op, "rstate")) {
            unsigned short z[3] = { 0, 0, 0 }, *o = seed48(z), s[3];
            s[0] = o[0];
            s[1] = o[1];
            s[2] = o[2];
            (void)seed48(s);
            *out = INTVAL((long long)s[0] | (long long)s[1] << 16 | (long long)s[2] << 32);
            return 1;
        }
        *out = FAILDESCR;
        return 1;
    }
    if (nargs == 3) return pl_ax_powm(args, out, ball);
    if (nargs == 1 && (!strcmp(op, "inc") || !strcmp(op, "dec"))) { DESCR_t one[2] = { args[0], INTVAL(1) }; return dop_ax_core(op[0] == 'i' ? "add" : "sub", one, 2, out, ball); }
    DESCR_t a = rt_pl_deref_val(args[0]);
    if (a.v != DT_I && a.v != DT_R && a.v != DT_BIG) {
        void *bl = (void *)0;
        DESCR_t ev;
        if (pl_ax_eval_d(a, &ev, &bl, 1)) a = ev;
        else { if (bl && ball && !*ball) *ball = bl; *out = FAILDESCR; return 1; }
    }
    if (nargs == 1 && (pl_rat_is(a) || !strcmp(op, "rat") || !strcmp(op, "ratz") || !strcmp(op, "numer") || !strcmp(op, "denom"))) {
        int qe = 0;
        if (pl_rat_unop(op, a, out, &qe)) return pl_rat_done(qe, op, 1, a, out, ball);
        if (pl_rat_is(a)) { DESCR_t fa = REALVAL(pl_rat_real(a)); return dop_ax(op, &fa, 1, out, ball); }
    }
    int ai = (a.v == DT_I), arl = (a.v == DT_R);
    double ad = arl ? a.r : (a.v == DT_BIG ? pl_big_as_real(a) : (double)a.i);
    if (a.v == DT_BIG && nargs == 1 && pl_big_unop(op, a, out)) return 1;
    if (a.v == DT_BIG && nargs == 1 && isinf(ad) && !pl_float_flag("float_overflow", "infinity")) {
        extern void *rt_pl_ball_eval_error(const char *, const char *, int);
        *out = FAILDESCR;
        if (ball && !*ball) *ball = rt_pl_ball_eval_error("float_overflow", op, 1);
        return 1;
    }
    if (nargs == 1) {
        if (!strcmp(op, "neg")) { if (ai && a.i == LLONG_MIN) return pl_big_unop("neg", a, out); *out = ai ? INTVAL(-a.i) : REALVAL(-ad); return 1; }
        if (!strcmp(op, "pos")) { *out = a; return 1; }
        if (!strcmp(op, "abs")) { if (ai && a.i == LLONG_MIN) return pl_big_unop("abs", a, out); *out = ai ? INTVAL(a.i < 0 ? -a.i : a.i) : REALVAL(fabs(ad)); return 1; }
        if (!strcmp(op, "sign")) { *out = ai ? INTVAL((a.i > 0) - (a.i < 0)) : isnan(ad) ? a : REALVAL((double)((ad > 0) - (ad < 0))); return 1; }
        if (!strcmp(op, "intg") && arl && isfinite(ad) && pl_swi_dialect()) return pl_ax_swi_round(ad, out);
        if (!strcmp(op, "trunc") || !strcmp(op, "floor") || !strcmp(op, "ceil") || !strcmp(op, "round")) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            int swi = pl_swi_dialect();
            if (!arl && (swi || pl_xsb_dialect())) { *out = a; return 1; }
            if (arl && !isfinite(ad) && swi) { *out = a; return 1; }
            if (!arl) { if (ball && !*ball) *ball = rt_pl_ball_kind2("type_error", "float", a); *out = FAILDESCR; return 1; }
            if (swi && !strcmp(op, "round")) return pl_ax_swi_round(ad, out);
            {
                double r = !strcmp(op, "trunc") ? trunc(ad) : !strcmp(op, "floor") ? floor(ad) : !strcmp(op, "ceil") ? ceil(ad) : floor(ad) + (ad - floor(ad) >= 0.5);
                if (pl_ax_float_to_int(r, out)) return 1;
                *out = FAILDESCR;
                if (ball && !*ball) *ball = rt_pl_ball_eval_error("undefined", op, 1);
            }
            return 1;
        }
        if (!strcmp(op, "intg")) { *out = ai ? a : INTVAL((long long)llround(ad)); return 1; }
        if (!strcmp(op, "flt")) { *out = REALVAL(ad); return 1; }
        if (!strcmp(op, "sqrt")) { if (ad < 0.0) return pl_ax_undef("sqrt", 1, out, ball); return pl_ax_float_result(sqrt(ad), ad, 0.0, "sqrt", 1, out, ball); }
        if (!strcmp(op, "sin")) { *out = REALVAL(sin(ad)); return 1; }
        if (!strcmp(op, "cos")) { *out = REALVAL(cos(ad)); return 1; }
        if (!strcmp(op, "atan")) { *out = REALVAL(atan(ad)); return 1; }
        if (!strcmp(op, "log") && pl_swi_dialect()) return pl_ax_float_result(log(ad), ad, 0.0, "log", 1, out, ball);
        if (!strcmp(op, "erf")) return pl_ax_float_result(erf(ad), ad, 0.0, "erf", 1, out, ball);
        if (!strcmp(op, "erfc")) return pl_ax_float_result(erfc(ad), ad, 0.0, "erfc", 1, out, ball);
        if (!strcmp(op, "log")) {
            if (ad < 0.0) return pl_ax_undef("log", 1, out, ball);
            if (ad == 0.0) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_eval_error("undefined", "log", 1); return 1; }
            *out = REALVAL(log(ad));
            return 1;
        }
        if (!strcmp(op, "exp")) { return pl_ax_float_result(exp(ad), ad, 0.0, "exp", 1, out, ball); }
        if (!strcmp(op, "lgamma")) { return pl_ax_float_result(lgamma(ad), ad, 0.0, "lgamma", 1, out, ball); }
        if ((!strcmp(op, "fip") || !strcmp(op, "ffp")) && (a.v == DT_I || a.v == DT_BIG) && pl_swi_dialect()) { *out = !strcmp(op, "fip") ? a : INTVAL(0); return 1; }
        if ((!strcmp(op, "fip") || !strcmp(op, "ffp")) && (int)a.v != DT_R) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            if (ball && !*ball) *ball = rt_pl_ball_kind2("type_error", "float", a);
            *out = FAILDESCR;
            return 1;
        }
        if (!strcmp(op, "fip")) { *out = REALVAL(trunc(ad)); return 1; }
        if (!strcmp(op, "ffp") && pl_swi_dialect()) { double ip; if (isnan(ad)) return pl_ax_undef("float_fractional_part", 1, out, ball); *out = REALVAL(modf(ad, &ip)); return 1; }
        if (!strcmp(op, "ffp")) { *out = REALVAL(ad - trunc(ad)); return 1; }
        if (!strcmp(op, "msb")) {
            if (!ai) { if (ball && !*ball) *ball = pl_ax_int_ball(a, a, 0); *out = FAILDESCR; return 1; }
            if (a.i <= 0) {
                extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
                if (pl_swi_dialect() && ball && !*ball) *ball = rt_pl_ball_kind2("domain_error", "not_less_than_one", a);
                *out = FAILDESCR;
                return 1;
            }
            *out = INTVAL(63 - __builtin_clzll((unsigned long long)a.i));
            return 1;
        }
        if (!strcmp(op, "bnot")) {
            if (!ai) { extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t); if (ball && !*ball) *ball = rt_pl_ball_kind2("type_error", "integer", a); *out = FAILDESCR; return 1; }
            *out = INTVAL(~a.i);
            return 1;
        }
        if (!strcmp(op, "rand")) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            if (a.v == DT_BIG) {
                extern int rt_big_sign(DESCR_t);
                extern long rt_big_bits(DESCR_t);
                extern DESCR_t rt_big_add(DESCR_t, DESCR_t), rt_big_mul(DESCR_t, DESCR_t), rt_big_mod(DESCR_t, DESCR_t);
                DESCR_t r = INTVAL(0);
                if (rt_big_sign(a) < 1) { if (ball && !*ball) *ball = rt_pl_ball_kind2("domain_error", "not_less_than_one", a); *out = FAILDESCR; return 1; }
                for (long k = rt_big_bits(a) + 64; k > 0; k -= 31) r = rt_big_add(rt_big_mul(r, INTVAL(1L << 31)), INTVAL(lrand48()));
                *out = rt_big_mod(r, a);
                return 1;
            }
            if (!ai) { if (ball && !*ball) *ball = rt_pl_ball_kind2("type_error", "integer", a); *out = FAILDESCR; return 1; }
            if (a.i < 1) { if (ball && !*ball) *ball = rt_pl_ball_kind2("domain_error", "not_less_than_one", a); *out = FAILDESCR; return 1; }
            *out = INTVAL((long long)(drand48() * (double)a.i));
            return 1;
        }
        if (!strcmp(op, "rset") || !strcmp(op, "rseed")) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            if (!ai) { if (ball && !*ball) *ball = rt_pl_ball_kind2("type_error", "integer", a); *out = FAILDESCR; return 1; }
            if (!strcmp(op, "rseed")) srand48(a.i < 0 ? (long)time((time_t *)0) ^ (long)getpid() : (long)a.i);
            else { unsigned short s[3] = { (unsigned short)(a.i & 0xFFFF), (unsigned short)(a.i >> 16 & 0xFFFF), (unsigned short)(a.i >> 32 & 0xFFFF) }; (void)seed48(s); }
            *out = INTVAL(0);
            return 1;
        }
        if (!strcmp(op, "tan")) { *out = REALVAL(tan(ad)); return 1; }
        if (!strcmp(op, "sinh")) { return pl_ax_float_result(sinh(ad), ad, 0.0, "sinh", 1, out, ball); }
        if (!strcmp(op, "cosh")) { return pl_ax_float_result(cosh(ad), ad, 0.0, "cosh", 1, out, ball); }
        if (!strcmp(op, "tanh")) { *out = REALVAL(tanh(ad)); return 1; }
        if (!strcmp(op, "asinh")) { return pl_ax_float_result(asinh(ad), ad, 0.0, "asinh", 1, out, ball); }
        if (!strcmp(op, "asin") || !strcmp(op, "acos") || !strcmp(op, "acosh") || !strcmp(op, "atanh") || !strcmp(op, "log2") || !strcmp(op, "log10")) {
            if (pl_swi_dialect()) {
                double r = !strcmp(op, "asin") ? asin(ad) : !strcmp(op, "acos") ? acos(ad) : !strcmp(op, "acosh") ? acosh(ad) : !strcmp(op, "atanh") ? atanh(ad) : !strcmp(op, "log2") ? log2(ad) :
                    log10(ad);
                return pl_ax_float_result(r, ad, 0.0, op, 1, out, ball);
            }
        }
        if (!strcmp(op, "asin") || !strcmp(op, "acos") || !strcmp(op, "acosh") || !strcmp(op, "atanh") || !strcmp(op, "log2") || !strcmp(op, "log10")) {
            int ok = !strcmp(op, "asin") || !strcmp(op, "acos") ? (ad >= -1.0 && ad <= 1.0) : !strcmp(op, "acosh") ? (ad >= 1.0) : !strcmp(op, "atanh") ? (ad > -1.0 && ad < 1.0) : (ad > 0.0);
            if (!ok) return pl_ax_undef(op, 1, out, ball);
            *out =
                REALVAL(!strcmp(op, "asin") ? asin(ad) : !strcmp(op, "acos") ? acos(ad) : !strcmp(op, "acosh") ? acosh(ad) : !strcmp(op, "atanh") ? atanh(ad) : !strcmp(op, "log2") ? log2(ad) :
                log10(ad));
            return 1;
        }
        if (!strcmp(op, "lsb") || !strcmp(op, "popc")) {
            if (!ai) { if (ball && !*ball) *ball = pl_ax_int_ball(a, a, 0); *out = FAILDESCR; return 1; }
            if (a.i < 0 || (a.i == 0 && !strcmp(op, "lsb") && pl_swi_dialect())) {
                extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
                if (ball && !*ball) *ball = rt_pl_ball_kind2("domain_error", !strcmp(op, "lsb") && pl_swi_dialect() ? "not_less_than_one" : "not_less_than_zero", a);
                *out = FAILDESCR;
                return 1;
            }
            if (!strcmp(op, "popc")) { *out = INTVAL(__builtin_popcountll((unsigned long long)a.i)); return 1; }
            if (a.i == 0) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_eval_error("undefined", "lsb", 1); return 1; }
            *out = INTVAL(__builtin_ctzll((unsigned long long)a.i));
            return 1;
        }
        *out = FAILDESCR;
        return 1;
    }
    DESCR_t b = rt_pl_deref_val(args[1]);
    if (b.v != DT_I && b.v != DT_R && b.v != DT_BIG) {
        void *bl = (void *)0;
        DESCR_t ev;
        if (pl_ax_eval_d(b, &ev, &bl, 1)) b = ev;
        else { if (bl && ball && !*ball) *ball = bl; *out = FAILDESCR; return 1; }
    }
    if (!strcmp(op, "div") && (pl_rat_is(a) || pl_rat_is(b)) && pl_rat_exact(b) && pl_rat_cmp(b, INTVAL(0)) == 0 && pl_float_flag("float_zero_div", "infinity")) {
        int sg = pl_rat_cmp(a, INTVAL(0));
        if (!sg) return pl_ax_undef("/", 2, out, ball);
        *out = REALVAL(sg < 0 ? -INFINITY : INFINITY);
        return 1;
    }
    if (pl_rat_is(a) || pl_rat_is(b) || !strcmp(op, "rdiv") ||
        (pl_rat_exact(a) && pl_rat_exact(b) && pl_prefer_rationals() && (!strcmp(op, "div") || ((!strcmp(op, "fpow") || !strcmp(op, "pow")) && rt_big_sign(b) < 0)))) {
        extern int rt_big_sign(DESCR_t);
        int qe = 0;
        DESCR_t cul = a;
        if (pl_rat_binop(op, a, b, out, &qe, &cul)) return pl_rat_done(qe, op, 2, cul, out, ball);
        if ((!strcmp(op, "fpow") || !strcmp(op, "pow")) && a.v == DT_R && a.r == 0.0 && pl_rat_is(b) && pl_rat_cmp(b, INTVAL(0)) < 0) return pl_rat_done(PLQ_ZDIV, op, 2, a, out, ball);
        if (!strcmp(op, "fpow") && !pl_rat_is(a) && pl_rat_is(b) && pl_swi_dialect()) {
            double x = a.v == DT_R ? a.r : a.v == DT_I ? (double)a.i : pl_big_as_real(a);
            if (x != 0.0 && !isnan(x)) {
                int m = fegetround(), neg = x < 0.0 && pl_q_odd(pl_rat_num(b));
                double e, r;
                if (x < 0.0 && !pl_q_odd(pl_rat_den(b))) return pl_ax_undef("**", 2, out, ball);
                fesetround(FE_TONEAREST);
                e = pl_rat_real(b);
                fesetround(neg && m == FE_UPWARD ? FE_DOWNWARD : neg && m == FE_DOWNWARD ? FE_UPWARD : m);
                r = pow(fabs(x), e);
                fesetround(m);
                return pl_ax_float_result(neg ? -r : r, x, 0.0, "**", 2, out, ball);
            }
        }
        if (pl_rat_is(a) || pl_rat_is(b)) { DESCR_t fa[2] = { pl_rat_is(a) ? REALVAL(pl_rat_real(a)) : a, pl_rat_is(b) ? REALVAL(pl_rat_real(b)) : b }; return dop_ax(op, fa, 2, out, ball); }
    }
    int bi = (b.v == DT_I), brl = (b.v == DT_R);
    double bd = brl ? b.r : (b.v == DT_BIG ? pl_big_as_real(b) : (double)b.i);
    if ((a.v == DT_BIG || b.v == DT_BIG) && !arl && !brl && pl_big_binop(op, a, b, out, ball)) return 1;
    if ((!strcmp(op, "fpow") || !strcmp(op, "pow")) && pl_swi_dialect() && ((bi && b.i == 0) || (brl && bd == 0.0))) { *out = INTVAL(1); return 1; }
    if (!strcmp(op, "fpow") || !strcmp(op, "pow")) {
        if (ai && bi && b.i >= 0) {
            long long r = 1, bs = a.i, e = b.i;
            int ovf = 0;
            while (e) { if (e & 1) ovf |= __builtin_mul_overflow(r, bs, &r); e >>= 1; if (e) ovf |= __builtin_mul_overflow(bs, bs, &bs); }
            if (!ovf) { *out = INTVAL(r); return 1; }
            if (!strcmp(op, "pow") || pl_swi_dialect()) { extern DESCR_t rt_big_pow(DESCR_t, int64_t); *out = rt_big_pow(a, b.i); return 1; }
        }
        if (!strcmp(op, "fpow") && ai && bi && b.i < 0 && (a.i == 1 || a.i == -1 || a.i == 0) && pl_swi_dialect()) {
            extern void *rt_pl_ball_eval_error(const char *, const char *, int);
            if (a.i == 0) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_eval_error("zero_divisor", "**", 2); return 1; }
            *out = INTVAL(a.i == 1 || !(b.i & 1) ? 1 : -1);
            return 1;
        }
        {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            extern void *rt_pl_ball_eval_error(const char *, const char *, int);
            const char *pn = !strcmp(op, "pow") ? "^" : "**";
            if (!strcmp(op, "pow") && ai && bi) {
                if (a.i == 1) { *out = INTVAL(1); return 1; }
                if (a.i == -1) { *out = INTVAL((b.i & 1) ? -1 : 1); return 1; }
                *out = FAILDESCR;
                if (ball && !*ball) *ball = a.i == 0 ? rt_pl_ball_eval_error("undefined", pn, 2) : rt_pl_ball_kind2("type_error", "float", a);
                return 1;
            }
            if (ad == 0.0 && bd < 0.0) {
                if (pl_float_flag("float_zero_div", "infinity")) { *out = REALVAL(pow(ad, bd)); return 1; }
                *out = FAILDESCR;
                if (ball && !*ball) *ball = rt_pl_ball_eval_error(pl_swi_dialect() ? "zero_divisor" : "undefined", pn, 2);
                return 1;
            }
            if (ad < 0.0 && bd != floor(bd)) return pl_ax_undef(pn, 2, out, ball);
            return pl_ax_float_result(pow(ad, bd), ad, bd, pn, 2, out, ball);
        }
    }
    if (!strcmp(op, "atan2") && pl_swi_dialect()) return pl_ax_float_result(atan2(ad, bd), ad, bd, "atan2", 2, out, ball);
    if (!strcmp(op, "atan2")) {
        if (ad == 0.0 && bd == 0.0) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_eval_error("undefined", "atan2", 2); return 1; }
        *out = REALVAL(atan2(ad, bd));
        return 1;
    }
    if (!strcmp(op, "logb")) {
        if (ad <= 0.0 || bd <= 0.0 || ad == 1.0) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_eval_error("undefined", "log", 2); return 1; }
        *out = REALVAL(log(bd) / log(ad));
        return 1;
    }
    if ((!strcmp(op, "min") || !strcmp(op, "max")) && (arl && isnan(ad)) != (brl && isnan(bd))) { *out = (arl && isnan(ad)) ? b : a; return 1; }
    if ((!strcmp(op, "min") || !strcmp(op, "max")) && !(ai && bi) && ad == bd && pl_swi_dialect()) {
        int c = arl != brl ? (arl ? -1 : 1) : (arl && signbit(ad) != signbit(bd)) ? (signbit(ad) ? -1 : 1) : 0;
        *out = !strcmp(op, "min") ? (c <= 0 ? a : b) : (c >= 0 ? a : b);
        return 1;
    }
    if (!strcmp(op, "min")) { *out = (ai && bi) ? INTVAL(a.i < b.i ? a.i : b.i) : (ad < bd ? a : ad > bd ? b : arl ? a : b); return 1; }
    if (!strcmp(op, "max")) { *out = (ai && bi) ? INTVAL(a.i > b.i ? a.i : b.i) : (ad > bd ? a : ad < bd ? b : arl ? a : b); return 1; }
    if (!strcmp(op, "copysign")) { if (ai) { long long m = a.i < 0 ? -a.i : a.i; *out = INTVAL(signbit(bd) ? -m : m); } else *out = REALVAL(copysign(ad, bd)); return 1; }
    if (!strcmp(op, "nexttoward")) { *out = REALVAL(nexttoward(ad, (long double)bd)); return 1; }
    if (!strcmp(op, "getbit")) {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; }
        if (a.i < 0 || b.i < 0) { if (ball && !*ball) *ball = rt_pl_ball_kind2("domain_error", "not_less_than_zero", a.i < 0 ? a : b); *out = FAILDESCR; return 1; }
        *out = INTVAL(b.i >= 64 ? 0 : (long long)(((unsigned long long)a.i >> b.i) & 1ULL));
        return 1;
    }
    if (!strcmp(op, "gcd")) {
        if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; }
        unsigned long long x = a.i < 0 ? 0ULL - (unsigned long long)a.i : (unsigned long long)a.i, y = b.i < 0 ? 0ULL - (unsigned long long)b.i : (unsigned long long)b.i;
        while (y) { unsigned long long t2 = x % y; x = y; y = t2; }
        if (x > (unsigned long long)LLONG_MAX) return pl_big_unop("neg", INTVAL(LLONG_MIN), out);
        *out = INTVAL((long long)x);
        return 1;
    }
    if (!strcmp(op, "rem")) {
        if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; }
        if (b.i == 0) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_eval_error("zero_divisor", "rem", 2); return 1; }
        *out = INTVAL(b.i == -1 ? 0 : a.i % b.i);
        return 1;
    }
    if (!strcmp(op, "idiv") || !strcmp(op, "divf") || !strcmp(op, "mod")) {
        if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; }
        if (b.i == 0) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_eval_error("zero_divisor", !strcmp(op, "idiv") ? "//" : !strcmp(op, "divf") ? "div" : "mod", 2); return 1; }
        if (b.i == -1) { if (a.i == LLONG_MIN && strcmp(op, "mod")) return pl_big_binop(op, a, b, out, ball); *out = INTVAL(!strcmp(op, "mod") ? 0 : -a.i); return 1; }
        {
            long long q = a.i / b.i, m = a.i % b.i;
            if (!strcmp(op, "idiv")) { *out = INTVAL(q); return 1; }
            if (m != 0 && ((m < 0) != (b.i < 0))) { q--; m += b.i; }
            *out = INTVAL(!strcmp(op, "mod") ? m : q);
            return 1;
        }
    }
    if (ai && bi && (!strcmp(op, "add") || !strcmp(op, "sub") || !strcmp(op, "mul"))) {
        long long r;
        int ovf = !strcmp(op, "add") ? __builtin_add_overflow(a.i, b.i, &r) : !strcmp(op, "sub") ? __builtin_sub_overflow(a.i, b.i, &r) : __builtin_mul_overflow(a.i, b.i, &r);
        if (ovf) return pl_big_binop(op, a, b, out, ball);
        *out = INTVAL(r);
        return 1;
    }
    if (!strcmp(op, "xor")) { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } *out = INTVAL(a.i ^ b.i); return 1; }
    if (!strcmp(op, "shl") || !strcmp(op, "shr")) {
        if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; }
        if (pl_ax_shift(op, a, b, out, ball)) return 1;
        *out = INTVAL(!strcmp(op, "shl") ? a.i << b.i : a.i >> b.i);
        return 1;
    }
    if (!strcmp(op, "band")) { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } *out = INTVAL(a.i & b.i); return 1; }
    if (!strcmp(op, "bor")) { if (!ai || !bi) { if (ball && !*ball) *ball = pl_ax_int_ball(a, b, ai); *out = FAILDESCR; return 1; } *out = INTVAL(a.i | b.i); return 1; }
    if (arl || brl) {
        if (!strcmp(op, "add")) return pl_ax_float_result(ad + bd, ad, bd, "+", 2, out, ball);
        if (!strcmp(op, "sub")) return pl_ax_float_result(ad - bd, ad, bd, "-", 2, out, ball);
        if (!strcmp(op, "mul")) return pl_ax_float_result(ad * bd, ad, bd, "*", 2, out, ball);
        if (!strcmp(op, "div")) {
            extern void *rt_pl_ball_eval_error(const char *, const char *, int);
            if (bd == 0.0) return pl_ax_zdiv(ad, bd, 1, "/", 2, out, ball);
            return pl_ax_float_result(ad / bd, ad, bd, "/", 2, out, ball);
        }
    }
    if (!strcmp(op, "div") && ai && bi && b.i == 0) return pl_ax_zdiv((double)a.i, 0.0, 0, "/", 2, out, ball);
    if (!strcmp(op, "div") && ai && bi) {
        extern int rt_pl_iso_mode(void);
        if (rt_pl_iso_mode() || pl_xsb_dialect() || pl_gnu_dialect()) return pl_ax_float_result((double)a.i / (double)b.i, (double)a.i, (double)b.i, "/", 2, out, ball);
    }
    { DESCR_t r = pl_arith2(op, a, b); if (r.v == DT_FAIL) { *out = FAILDESCR; return 1; } *out = r; return 1; }
}
