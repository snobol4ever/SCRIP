#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_04_pl.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_06_pl.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_28_sh.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_32_pl.h"
extern int rt_proc_is_registered(const char *name);
static int pl_ax_eval_r(DESCR_t t, DESCR_t *out, void **ball, int depth, pl_ax_cx_t *cx);
int rt_pl_acyclic_cell(void *);
static const char *pl_dq_swap(const char *v);
extern int rt_pl_db_abolish(void *, void *);
extern int rt_pl_nb_set(void *, int64_t, void *);
extern int rt_pl_nb_get_cell(void *, int64_t, void *, pl_tr_ctx_t *);
extern int rt_pl_b_set(void *, int64_t, void *, pl_tr_ctx_t *);
extern int rt_pl_db_count(void *);
extern int rt_pl_db_seed(void *, void *, void *);
extern int rt_pl_db_term_key(void *, char *, size_t, int *);
extern void *rt_pl_db_get_by_key(void *, const char *, int);
extern int rt_pl_db_term_key(void *, char *, size_t, int *);
extern int rt_pl_db_key_is_dynamic(void *, const char *);
extern void *rt_pl_ball_permission_pi(const char *, const char *, const char *, int);
extern void rt_pl_db_unqual_term(void *);
extern void rt_pl_db_unqual_pi(void *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *pl_procedure_arity_ball(const char *opname, int request) {
    extern void *rt_pl_ball_kind1_system_ctx(const char *, const char *, const char *, int, const char *);
    char msg[64];
    snprintf(msg, sizeof msg, "limit is %d, request = %d", PROLOG_MAX_PROCEDURE_ARITY, request);
    return rt_pl_ball_kind1_system_ctx("representation_error", "max_procedure_arity", opname, 1, msg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_real_iso_str(double fv, char * fb, int bufsz) {
    extern int rt_pl_dialect_swi(void);
    extern int rt_pl_dialect_gnu(void);
    extern int rt_pl_dialect_xsb(void);
    extern const char *rt_pl_flag_text(const char *);
    const char *xfp = rt_pl_dialect_xsb() ? rt_pl_flag_text("$xsb_float_precision") : (const char *)0;
    int prec = rt_pl_dialect_xsb() ? (xfp ? atoi(xfp) : 15) : rt_pl_dialect_gnu() ? 17 : 0;
    if (isnan(fv)) { snprintf(fb, (size_t)bufsz, "1.5NaN"); return fb; }
    if (isinf(fv) && fv < 0) { snprintf(fb, (size_t)bufsz, "-1.0Inf"); return fb; }
    if (isinf(fv)) { snprintf(fb, (size_t)bufsz, "1.0Inf"); return fb; }
    if (prec) snprintf(fb, (size_t)bufsz, "%.*g", prec, fv);
    else for (prec = 15; prec <= 17; prec++) { snprintf(fb, (size_t)bufsz, "%.*g", prec, fv); if (strtod(fb, (char **)0) == fv) break; }
    if (!strchr(fb, '.') && strlen(fb) + 2 < (size_t)bufsz) {
        if (strpbrk(fb, "eE")) snprintf(fb, (size_t)bufsz, "%.1e", fv);
        else { size_t n = strlen(fb); fb[n] = '.'; fb[n + 1] = '0'; fb[n + 2] = '\0'; }
    }
    { char * x = strpbrk(fb, "eE"); if (x && (x[1] == '+' || x[1] == '-') && x[2] == '0' && x[3] && rt_pl_dialect_swi()) { x[2] = x[3]; x[3] = '\0'; } }
    return fb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *pl_ax_int_ball(DESCR_t a, DESCR_t b, int ai) { extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t); return rt_pl_ball_kind2("type_error", "integer", ai ? b : a); }
int pl_swi_dialect(void) { return pl_float_flag("$dialect", "swi"); }
int pl_gnu_dialect(void) { return pl_float_flag("$dialect", "gnu"); }
int pl_xsb_dialect(void) { return pl_float_flag("$dialect", "xsb"); }
int rt_pl_flag_true(const char *nm) { return pl_float_flag(nm, "true"); }
int rt_pl_flag_is(const char *nm, const char *val) { return pl_float_flag(nm, val); }
int rt_pl_fp_max_code(FILE *fp) {
    for (int i = 0; fp && i < FH_N; i++) {
        const char *e = g_fh[i].fp == fp ? g_fh[i].enc : (const char *)0;
        if (g_fh[i].fp != fp) continue;
        if (e && !strcmp(e, "US-ASCII")) return 0x7F;
        if (e && (!strcmp(e, "ISO-8859-1") || !strcmp(e, "octet"))) return 0xFF;
        break;
    }
    return 0x10FFFF;
}
int pl_ax_undef(const char *fn, int arity, DESCR_t *out, void **ball) {
    extern void *rt_pl_ball_eval_error(const char *, const char *, int);
    if (pl_float_flag("float_undefined", "nan")) { *out = REALVAL(NAN); return 1; }
    *out = FAILDESCR;
    if (ball && !*ball) *ball = rt_pl_ball_eval_error("undefined", fn, arity);
    return 1;
}
int pl_ax_zdiv(double num, double den, int fdiv, const char *fn, int arity, DESCR_t *out, void **ball) {
    extern void *rt_pl_ball_eval_error(const char *, const char *, int);
    if (pl_float_flag("float_zero_div", "infinity")) { if (num == 0.0 || isnan(num)) return pl_ax_undef(fn, arity, out, ball); *out = REALVAL(num / den); return 1; }
    if (fdiv && (num == 0.0 || isnan(num))) return pl_ax_undef(fn, arity, out, ball);
    *out = FAILDESCR;
    if (ball && !*ball) *ball = rt_pl_ball_eval_error("zero_divisor", fn, arity);
    return 1;
}
int pl_ax_float_result(double r, double x, double y, const char *fn, int arity, DESCR_t *out, void **ball) {
    extern void *rt_pl_ball_eval_error(const char *, const char *, int);
    (void)x;
    (void)y;
    if (isnan(r)) return pl_ax_undef(fn, arity, out, ball);
    if (isinf(r) && !pl_float_flag("float_overflow", "infinity")) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_eval_error("float_overflow", fn, arity); return 1; }
    *out = REALVAL(r);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_big_sign(DESCR_t d) { extern int rt_big_cmp(DESCR_t, DESCR_t); return rt_big_cmp(d, INTVAL(0)); }
double pl_big_as_real(DESCR_t d) {
    extern char *rt_big_str(DESCR_t);
    extern long rt_big_bits(DESCR_t);
    char *t;
    if (rt_big_bits(d) >= 1025) return pl_big_sign(d) < 0 ? -HUGE_VAL : HUGE_VAL;
    t = rt_big_str(d);
    return t ? strtod(t, (char **)0) : 0.0;
}
static DESCR_t rt_big_neg_l(DESCR_t d) { extern DESCR_t rt_big_neg(DESCR_t); return rt_big_neg(d); }
int pl_big_unop(const char *op, DESCR_t a, DESCR_t *out) {
    extern DESCR_t rt_big_neg(DESCR_t);
    if (!strcmp(op, "neg")) { *out = rt_big_neg(a); return 1; }
    if (!strcmp(op, "pos")) { *out = a; return 1; }
    if (!strcmp(op, "abs")) { *out = pl_big_sign(a) < 0 ? rt_big_neg(a) : a; return 1; }
    if (!strcmp(op, "sign")) { int s = pl_big_sign(a); *out = INTVAL(s < 0 ? -1 : (s > 0 ? 1 : 0)); return 1; }
    if ((!strcmp(op, "msb") || !strcmp(op, "lsb") || !strcmp(op, "popc")) && pl_big_sign(a) > 0) {
        extern DESCR_t rt_big_div(DESCR_t, DESCR_t);
        extern DESCR_t rt_big_mod(DESCR_t, DESCR_t);
        extern int rt_big_cmp(DESCR_t, DESCR_t);
        long long bits = 0, low = -1, pop = 0;
        DESCR_t q = a;
        while (rt_big_cmp(q, INTVAL(0)) > 0) { if (rt_big_cmp(rt_big_mod(q, INTVAL(2)), INTVAL(1)) == 0) { pop++; if (low < 0) low = bits; } q = rt_big_div(q, INTVAL(2)); bits++; }
        *out = INTVAL(!strcmp(op, "msb") ? bits - 1 : !strcmp(op, "lsb") ? low : pop);
        return 1;
    }
    if (!strcmp(op, "intg") || !strcmp(op, "trunc") || !strcmp(op, "floor") || !strcmp(op, "ceil") || !strcmp(op, "round")) { *out = a; return 1; }
    return 0;
}
int pl_big_binop(const char *op, DESCR_t a, DESCR_t b, DESCR_t *out, void **ball) {
    extern DESCR_t rt_big_add(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_mul(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_div(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_mod(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_pow(DESCR_t, int64_t);
    extern int rt_big_cmp(DESCR_t, DESCR_t);
    extern void *rt_pl_ball_eval_error(const char *, const char *, int);
    if (!strcmp(op, "add")) { *out = rt_big_add(a, b); return 1; }
    if (!strcmp(op, "sub")) { *out = rt_big_sub(a, b); return 1; }
    if (!strcmp(op, "mul")) { *out = rt_big_mul(a, b); return 1; }
    if (!strcmp(op, "min")) { *out = rt_big_cmp(a, b) <= 0 ? a : b; return 1; }
    if (!strcmp(op, "max")) { *out = rt_big_cmp(a, b) >= 0 ? a : b; return 1; }
    if (!strcmp(op, "idiv") || !strcmp(op, "divf") || !strcmp(op, "mod") || !strcmp(op, "rem")) {
        if (pl_big_sign(b) == 0) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_eval_error("zero_divisor", !strcmp(op, "idiv") ? "//" : !strcmp(op, "divf") ? "div" : op, 2); return 1; }
        {
            DESCR_t q = rt_big_div(a, b), m = rt_big_mod(a, b);
            if (!strcmp(op, "idiv")) { *out = q; return 1; }
            if (!strcmp(op, "rem")) { *out = m; return 1; }
            if (pl_big_sign(m) != 0 && (pl_big_sign(m) < 0) != (pl_big_sign(b) < 0)) { q = rt_big_sub(q, INTVAL(1)); m = rt_big_add(m, b); }
            *out = !strcmp(op, "mod") ? m : q;
            return 1;
        }
    }
    if ((!strcmp(op, "pow") || !strcmp(op, "fpow")) && b.v == DT_I && b.i >= 0) { *out = rt_big_pow(a, b.i); return 1; }
    if ((!strcmp(op, "pow") || !strcmp(op, "fpow")) && b.v == DT_BIG && a.v == DT_I && (a.i == 1 || a.i == -1 || (a.i == 0 && pl_big_sign(b) > 0))) {
        long long r = a.i == 0 ? 0 : (a.i == 1 || rt_big_cmp(rt_big_mod(b, INTVAL(2)), INTVAL(0)) == 0) ? 1 : -1;
        *out = (!strcmp(op, "fpow") && !pl_swi_dialect()) ? REALVAL((double)r) : INTVAL(r);
        return 1;
    }
    {
        extern DESCR_t rt_big_and(DESCR_t, DESCR_t);
        extern DESCR_t rt_big_or(DESCR_t, DESCR_t);
        extern DESCR_t rt_big_xor(DESCR_t, DESCR_t);
        if (!strcmp(op, "band")) { *out = rt_big_and(a, b); return 1; }
        if (!strcmp(op, "bor")) { *out = rt_big_or(a, b); return 1; }
        if (!strcmp(op, "xor")) { *out = rt_big_xor(a, b); return 1; }
    }
    if (!strcmp(op, "shl") || !strcmp(op, "shr")) return pl_ax_shift(op, a, b, out, ball);
    if (!strcmp(op, "getbit") && b.v == DT_I && b.i >= 0 && pl_big_sign(a) >= 0) {
        extern DESCR_t rt_big_and(DESCR_t, DESCR_t);
        DESCR_t t;
        if (!pl_ax_shift("shr", a, b, &t, ball)) return 0;
        *out = rt_big_and(t, INTVAL(1));
        return 1;
    }
    if (!strcmp(op, "gcd")) {
        DESCR_t x = pl_big_sign(a) < 0 ? rt_big_neg_l(a) : a, y = pl_big_sign(b) < 0 ? rt_big_neg_l(b) : b;
        while (pl_big_sign(y) != 0) { DESCR_t t = rt_big_mod(x, y); if (pl_big_sign(t) < 0) t = rt_big_add(t, y); x = y; y = t; }
        *out = x;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_ax_shift(const char *op, DESCR_t a, DESCR_t b, DESCR_t *out, void **ball) {
    extern DESCR_t rt_big_mul(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_pow(DESCR_t, int64_t);
    enum { SHIFT_BITS_MAX = 1 << 20 };
    if (b.v == DT_BIG && (pl_big_sign(b) > 0) != !strcmp(op, "shl")) { *out = INTVAL(0); return 1; }
    if (b.v != DT_I) return 0;
    long long n = b.i;
    int left = !strcmp(op, "shl");
    if (n < 0) { if (n == LLONG_MIN) { *out = INTVAL(pl_big_sign(a) < 0 ? -1 : 0); return 1; } n = -n; left = !left; }
    if (!left) {
        if (a.v == DT_I) { *out = INTVAL(n >= 63 ? (a.i < 0 ? -1 : 0) : (a.i >> n)); return 1; }
        if (n > SHIFT_BITS_MAX) return 0;
        { DESCR_t t = rt_big_pow(INTVAL(2), n); if (t.v == DT_FAIL) return 0; return pl_big_binop("divf", a, t, out, ball); }
    }
    if (a.v == DT_I) {
        long long x = a.i;
        if (x == 0) { *out = INTVAL(0); return 1; }
        if (n < 63) { long long r = (long long)((unsigned long long)x << n); if ((r >> n) == x) { *out = INTVAL(r); return 1; } }
    }
    if (n > SHIFT_BITS_MAX) return 0;
    { DESCR_t t = rt_big_pow(INTVAL(2), n); if (t.v == DT_FAIL) return 0; *out = rt_big_mul(a, t); return 1; }
}
static const char *pl_rat_fn(const char *op) { return !strcmp(op, "fpow") ? "**" : !strcmp(op, "pow") ? "^" : !strcmp(op, "div") ? "/" : op; }
int pl_rat_done(int qe, const char *op, int arity, DESCR_t culprit, DESCR_t *out, void **ball) {
    extern void *rt_pl_ball_eval_error(const char *, const char *, int);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    if (!qe) return 1;
    if (qe == PLQ_UNDEF) return pl_ax_undef(pl_rat_fn(op), arity, out, ball);
    *out = FAILDESCR;
    if (ball && !*ball) *ball = qe == PLQ_ZDIV ? rt_pl_ball_eval_error("zero_divisor", pl_rat_fn(op), arity) : rt_pl_ball_kind2("type_error", qe == PLQ_TYPE_INT ? "integer" : "rational", culprit);
    return 1;
}
int pl_prefer_rationals(void) { return pl_float_flag("prefer_rationals", "true"); }
int pl_q_odd(DESCR_t q) {
    extern char *rt_big_str(DESCR_t);
    char *t;
    size_t n;
    if (q.v == DT_I) return (int)(q.i & 1);
    t = q.v == DT_BIG ? rt_big_str(q) : (char *)0;
    n = t ? strlen(t) : 0;
    return n && ((t[n - 1] - '0') & 1);
}
static const char *pl_xsb_arith_name(const char *sfx, int ar) {
    static const char *const un[][2] = { { "trunc", "truncate" }, { "round", "round" }, { "ceil", "ceiling" }, { "floor", "floor" }, { "exp", "exp" }, { "log", "log" }, { "log10", "log10" }, { "sign",
        "sign" }, { "acos", "acos" }, { "asin", "asin" }, { "tan", "tan" }, { "cos", "cos" }, { "sin", "sin" }, { "atan", "atan" }, { "abs", "abs" }, { "flt", "float" }, { "sqrt", "sqrt" }, { "bnot",
        "'\\'" }, { "neg", "-" }, { (const char *)0, (const char *)0 } };
    static const char *const bn[][2] = { { "add", "+" }, { "sub", "-" }, { "mul", "*" }, { "div", "/" }, { "idiv", "//" }, { "divf", "div" }, { "mod", "/" }, { "rem", "//" }, { "fpow", "**" },
        { "pow", "**" }, { "min", "min" }, { "max", "max" }, { "xor", "'><'" }, { "shr", "'>>'" }, { "shl", "'<<'" }, { "band", "'/\\'" }, { "bor", "'\\/'" }, { (const char *)0, (const char *)0 } };
    if (!sfx) return (const char *)0;
    if (ar == 1) { for (int i = 0; un[i][0]; i++) if (!strcmp(sfx, un[i][0])) return un[i][1]; }
    if (ar == 2) { for (int i = 0; bn[i][0]; i++) if (!strcmp(sfx, bn[i][0])) return bn[i][1]; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_xsb_arith_ctx(void **ball, const char *sfx, int ar, DESCR_t *kids, DESCR_t *ev, int k, const char *nm, const DESCR_t *whole) {
    extern int rt_pl_ball_xsb_arith_kind(void *);
    extern void *rt_pl_ball_set_xsb_arith_msg(void *, const char *, const char *);
    extern const char *rt_pl_writeq_text(DESCR_t, int, size_t *);
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    const char *op, *s0 = "", *s1 = "", *f;
    size_t n0 = 0, n1 = 0;
    DESCR_t o0, o1;
    int kind, neg1 = ar == 1 && sfx && !strcmp(sfx, "neg");
    if (!ball || !*ball || !kids || !pl_xsb_dialect() || !(kind = rt_pl_ball_xsb_arith_kind(*ball))) return;
    if (whole) {
        if (!nm || (ar != 1 && ar != 2) || !pl_xsb_arith_name(sfx, ar)) return;
        int sym = !(nm[0] >= 'a' && nm[0] <= 'z');
        s0 = rt_pl_writeq_text(rt_pl_deref_val(kids[0]), 1, &n0);
        if (ar == 2) s1 = rt_pl_writeq_text(rt_pl_deref_val(kids[1]), 1, &n1);
        f = ar == 2 ? (sym ? "'%s'(%.*s,%.*s)" : "%s(%.*s,%.*s)") : (sym ? "'%s'(%.*s)" : "%s(%.*s)");
        char a0[snprintf((char *)0, 0, f, nm, (int)n0, s0, (int)n1, s1) + 1];
        snprintf(a0, sizeof a0, f, nm, (int)n0, s0, (int)n1, s1);
        f = ar == 1 ? " In evaluable function (Goal: %s)" : kind == 1 ? " Uninstantiated argument of evaluable function %s/2 (Goal: %s)" : " Wrong type in evaluable function %s/2 (Goal: %s)";
        char msg[snprintf((char *)0, 0, f, nm, a0) + 1];
        snprintf(msg, sizeof msg, f, nm, a0);
        *ball = rt_pl_ball_set_xsb_arith_msg(*ball, msg, a0);
        return;
    }
    op = pl_xsb_arith_name(sfx, ar);
    if (!op) return;
    o0 = 0 < k ? ev[0] : rt_pl_deref_val(kids[0]);
    o1 = ar == 2 ? (1 < k ? ev[1] : rt_pl_deref_val(kids[1])) : o0;
    if (ar == 1 && !neg1) {
        s0 = rt_pl_writeq_text(o0, 0, &n0);
        f = " In evaluable function (Goal: %s(%.*s))";
        char msg[snprintf((char *)0, 0, f, op, (int)n0, s0) + 1];
        snprintf(msg, sizeof msg, f, op, (int)n0, s0);
        *ball = rt_pl_ball_set_xsb_arith_msg(*ball, msg, (const char *)0);
        return;
    }
    if (neg1) { s0 = "0"; n0 = 1; s1 = rt_pl_writeq_text(o0, 0, &n1); } else { s0 = rt_pl_writeq_text(o0, 0, &n0); s1 = rt_pl_writeq_text(o1, 0, &n1); }
    if (kind == 1) f = " Uninstantiated argument of evaluable function %s/2 (Goal: %s(%.*s,%.*s))";
    else if (kind == 2) f = " Wrong type in evaluable function %s/2: (Goal: %s(%.*s,%.*s))";
    else if (o0.v != DT_I && o0.v != DT_BIG) f = " Wrong type in evaluable function %s/2  (Goal: %s(%.*s,%.*s))";
    else f = " Wrong type in evaluable function %s/2 (Goal: %s(%.*s,%.*s)";
    char msg[snprintf((char *)0, 0, f, op, op, (int)n0, s0, (int)n1, s1) + 1];
    snprintf(msg, sizeof msg, f, op, op, (int)n0, s0, (int)n1, s1);
    *ball = rt_pl_ball_set_xsb_arith_msg(*ball, msg, (const char *)0);
}
int dop_ax(const char *op, DESCR_t *args, int nargs, DESCR_t *out, void **ball) {
    int r = dop_ax_core(op, args, nargs, out, ball);
    if (out->v == DT_FAIL && (nargs == 1 || nargs == 2)) pl_xsb_arith_ctx(ball, op, nargs, args, args, 0, (const char *)0, (const DESCR_t *)0);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ax_isign(DESCR_t d) { return d.v == DT_I ? (d.i > 0) - (d.i < 0) : pl_big_sign(d); }
static int pl_ax_powm_arg(DESCR_t d, const char *dom, int floor1, DESCR_t *out, void **ball) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    if (d.v != DT_I && d.v != DT_BIG) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_kind2("type_error", "integer", d); return 0; }
    if (dom && pl_ax_isign(d) < floor1) { *out = FAILDESCR; if (ball && !*ball) *ball = rt_pl_ball_kind2("domain_error", dom, d); return 0; }
    return 1;
}
int pl_ax_powm(DESCR_t *args, DESCR_t *out, void **ball) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    DESCR_t vb = rt_pl_deref_val(args[0]), ve = rt_pl_deref_val(args[1]), vm = rt_pl_deref_val(args[2]);
    if (!pl_ax_powm_arg(vb, (const char *)0, 0, out, ball) || !pl_ax_powm_arg(ve, (const char *)0, 0, out, ball) || !pl_ax_powm_arg(vm, (const char *)0, 0, out, ball)) return 1;
    if (!pl_ax_powm_arg(vb, "not_less_than_zero", 0, out, ball) || !pl_ax_powm_arg(ve, "not_less_than_zero", 0, out, ball) || !pl_ax_powm_arg(vm, "not_less_than_one", 1, out, ball)) return 1;
    if (vb.v == DT_I && ve.v == DT_I && vm.v == DT_I) {
        unsigned __int128 m = (unsigned __int128)vm.i, b = (unsigned __int128)vb.i % m, r = 1 % m;
        unsigned long long e = (unsigned long long)ve.i;
        while (e) { if (e & 1) r = (r * b) % m; b = (b * b) % m; e >>= 1; }
        *out = INTVAL((long long)r);
        return 1;
    }
    {
        DESCR_t r, b, e = ve, two = INTVAL(2), pr[2];
        pr[0] = INTVAL(1);
        pr[1] = vm;
        if (!dop_ax("mod", pr, 2, &r, ball) || r.v == DT_FAIL) { *out = FAILDESCR; return 1; }
        pr[0] = vb;
        if (!dop_ax("mod", pr, 2, &b, ball) || b.v == DT_FAIL) { *out = FAILDESCR; return 1; }
        while (pl_ax_isign(e) > 0) {
            DESCR_t bit, t;
            pr[0] = e;
            pr[1] = two;
            if (!dop_ax("mod", pr, 2, &bit, ball) || bit.v == DT_FAIL) { *out = FAILDESCR; return 1; }
            if (bit.v == DT_I && bit.i == 1) {
                pr[0] = r;
                pr[1] = b;
                dop_ax("mul", pr, 2, &t, ball);
                pr[0] = t;
                pr[1] = vm;
                dop_ax("mod", pr, 2, &r, ball);
                if (r.v == DT_FAIL) { *out = FAILDESCR; return 1; }
            }
            pr[0] = b;
            pr[1] = b;
            dop_ax("mul", pr, 2, &t, ball);
            pr[0] = t;
            pr[1] = vm;
            dop_ax("mod", pr, 2, &b, ball);
            if (b.v == DT_FAIL) { *out = FAILDESCR; return 1; }
            pr[0] = e;
            pr[1] = two;
            dop_ax("idiv", pr, 2, &e, ball);
            if (e.v == DT_FAIL) { *out = FAILDESCR; return 1; }
        }
        *out = r;
        return 1;
    }
}
static const char *pl_ax_sfx_rt(const char *nm, int ar) { return (ar == 2 && nm && !strcmp(nm, "><") && pl_xsb_dialect()) ? "xor" : pl_ax_suffix_any(nm, ar); }
int pl_ax_eval(DESCR_t t, DESCR_t *out, void **ball) { return pl_ax_eval_d(t, out, ball, 0); }
int pl_ax_eval_d(DESCR_t t, DESCR_t *out, void **ball, int depth) {
    extern int rt_pl_acyclic_cell(void *);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    pl_ax_cx_t cx = { t, 0 };
    if (pl_ax_eval_r(t, out, ball, depth, &cx)) return 1;
    if (!cx.proven && ball && *ball) { DESCR_t r = rt_pl_deref_val(t); if ((int)r.v == DT_PLREF && !rt_pl_acyclic_cell(&r)) *ball = rt_pl_ball_kind2("type_error", "expression", r); }
    return 0;
}
static int pl_ax_eval_r(DESCR_t t, DESCR_t *out, void **ball, int depth, pl_ax_cx_t *cx) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    extern const char *prolog_atom_name(int);
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_type_pi(const char *, const char *, const char *, int);
    DESCR_t d = rt_pl_deref_val(t);
    if (d.v == DT_I || d.v == DT_R || d.v == DT_BIG || pl_rat_is(d)) { *out = d; return 1; }
    *out = d;
    if (d.v == (DTYPE_t)DT_PLVAR || d.v == DT_SNUL || d.v == DT_FAIL) { *ball = rt_pl_ball_instantiation(); return 0; }
    if (d.v == DT_S) { const char *one = pl_atom_str(d); int adv = 0; if (one && one[0]) { long long cp = (long long)rt_pl_u8_get(one, &adv); if (one[adv] == '\0') { *out = INTVAL(cp); return 1; } } }
    if ((int)d.v == DT_PLATOM || d.v == DT_S) {
        const char *nm = pl_atom_str(d);
        const char *sfx = pl_ax_sfx_rt(nm, 0);
        if (sfx) { DESCR_t none = FAILDESCR; dop_ax(sfx, &none, 0, out, ball); return out->v != DT_FAIL; }
        if (pl_xsb_dialect()) { extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t); *ball = rt_pl_ball_kind2("type_error", "evaluable", d); return 0; }
        *ball = rt_pl_ball_type_pi("type_error", "evaluable", nm ? nm : "?", 0);
        return 0;
    }
    if ((int)d.v == DT_PLREF) {
        int ar = plc_fid_arity(d.slen);
        const char *nm = prolog_atom_name(plc_fid_name(d.slen));
        const char *sfx = pl_ax_sfx_rt(nm, ar);
        DESCR_t *kids = (DESCR_t *)d.p;
        DESCR_t ev[3];
        if (depth >= 256 && !cx->proven) {
            extern int rt_pl_acyclic_cell(void *);
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            DESCR_t r = rt_pl_deref_val(cx->root);
            if (!rt_pl_acyclic_cell(&r)) { *ball = rt_pl_ball_kind2("type_error", "expression", r); return 0; }
            cx->proven = 1;
        }
        if (ar == 2 && nm && !strcmp(nm, ".") && kids && !pl_xsb_dialect()) {
            DESCR_t tl = rt_pl_deref_val(kids[1]), hd = rt_pl_deref_val(kids[0]);
            if ((int)tl.v == DT_PLATOM && pl_atom_str(tl) && !strcmp(pl_atom_str(tl), "[]")) {
                if (hd.v == (DTYPE_t)DT_PLVAR) { *ball = rt_pl_ball_instantiation(); return 0; }
                if (hd.v == DT_I) { *out = hd; return 1; }
                if ((int)hd.v == DT_PLATOM) {
                    const char *a = pl_atom_str(hd);
                    int adv = 0;
                    if (a && a[0]) { long long cp = (long long)rt_pl_u8_get(a, &adv); if (a[adv] == '\0') { *out = INTVAL(cp); return 1; } }
                }
            }
        }
        if (ar == 2 && nm && !strcmp(nm, "roundtoward") && kids) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            DESCR_t md = rt_pl_deref_val(kids[1]);
            int mode = pl_round_mode(pl_atom_str(md)), saved = fegetround(), r;
            if (md.v == (DTYPE_t)DT_PLVAR) { *ball = rt_pl_ball_instantiation(); return 0; }
            if (mode < 0) { *ball = rt_pl_ball_kind2(pl_atom_str(md) ? "domain_error" : "type_error", pl_atom_str(md) ? "round_toward" : "atom", md); return 0; }
            fesetround(mode);
            r = pl_ax_eval_r(kids[0], out, ball, depth + 1, cx);
            fesetround(saved);
            return r;
        }
        if (!sfx || ar < 1 || ar > 3 || !kids) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            *ball = pl_xsb_dialect() ? rt_pl_ball_kind2("type_error", "evaluable", (depth == 0 && ar == 1 && kids) ? rt_pl_deref_val(kids[0]) : d) :
                rt_pl_ball_type_pi("type_error", "evaluable", nm ? nm : "?", ar);
            return 0;
        }
        for (int k = 0; k < ar; k++) if (!pl_ax_eval_r(kids[k], &ev[k], ball, depth + 1, cx)) { *out = ev[k]; pl_xsb_arith_ctx(ball, sfx, ar, kids, ev, k, nm, &d); return 0; }
        dop_ax_core(sfx, ev, ar, out, ball);
        if (out->v == DT_FAIL) pl_xsb_arith_ctx(ball, sfx, ar, kids, ev, ar, nm, &d);
        return out->v != DT_FAIL;
    }
    *ball = rt_pl_ball_type_pi("type_error", "evaluable", "?", 0);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_ax_eval_val(DESCR_t t, DESCR_t *out, void **ball) { return pl_ax_eval(t, out, ball); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_unify_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    if (nargs != 2) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    {
        char *tr0 = cx->tr;
        DESCR_t out;
        if (plw_unify_vals(args[0], args[1], cx)) out = rt_pl_deref_val(args[0]);
        else { cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0); out = FAILDESCR; }
        rt_pl_tr_gc_sync(cx->tr);
        return out;
    }
}
const char *pl_sp_mode_name(int i) {
    char m = (i == 0) ? 'r' : (i == 1 || i == 2) ? ((pl_swi_dialect() || pl_gnu_dialect()) ? 'w' : 'a') : g_fh[i].mode;
    return (m == 'r') ? "read" : (m == 'a') ? "append" : (m == '+') ? "update" : "write";
}
int pl_swi_string_zero_quote(const char *t) { return pl_swi_dialect() && t && t[0] == '0' && t[1] == '\'' && !t[2]; }
int pl_tok_is_graphic(int c) { return c && strchr("+-*/\\^<>=~:.?@#&$", c) != (char *)0; }
static const char *pl_read_dq_option(DESCR_t opts) {
    static const char *const dq[] = { "atom", "chars", "codes", "string", 0 };
    for (DESCR_t cur = rt_pl_deref_val(opts); pl_is_cons(cur); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        const char *fn = (e.v == (DTYPE_t)DT_PLREF && plc_fid_arity(e.slen) == 1) ? prolog_atom_name(plc_fid_name(e.slen)) : (const char *)0;
        if (!fn || strcmp(fn, "double_quotes")) continue;
        const char *as = pl_atom_str(rt_pl_deref_val(((DESCR_t *)e.p)[0]));
        for (int i = 0; as && dq[i]; i++) if (!strcmp(as, dq[i])) return dq[i];
    }
    return (const char *)0;
}
int pl_read_term_from_text(DESCR_t *args, pl_tr_ctx_t *cx, int atom_text) {
    extern void *rt_pl_ball_kind1(const char *, const char *);
    const char *b = atom_text && pl_is_nil(rt_pl_deref_val(args[0])) ? "[]" : pl_list_text_heap(args[0]);
    DESCR_t t;
    pl_vtab_t vt;
    int gnu = pl_gnu_dialect(), sm = 0;
    if (!b) return 0;
    if (gnu) { void *ob = pl_read_opts_ball(args[2]); if (ob) { cx->ball = ob; return 0; } sm = pl_read_syntax_mode(args[2]); }
    const char *dq = pl_read_dq_option(args[2]), *sv = dq ? pl_dq_swap(dq) : (const char *)0;
    int parsed = pl_parse_term_text(b, &t, &vt, (PlProgram **)0);
    if (dq) pl_dq_swap(sv);
    if (!parsed) {
        if (sm == 2) printf("warning: syntax error: %s\n", PL_SYNERR(vt));
        if (sm == 3) fprintf(stderr, "warning: syntax error: %s\n", PL_SYNERR(vt));
        if (!sm) cx->ball = rt_pl_ball_kind1("syntax_error", PL_SYNERR(vt));
        return 0;
    }
    return plw_unify_vals(args[1], t, cx) && pl_read_term_options_cell(args[2], &vt, cx);
}
int pl_read_syntax_mode(DESCR_t opts) {
    extern const char *prolog_atom_name(int);
    DESCR_t cur, e;
    const char *fn, *as;
    int m = 0;
    for (cur = rt_pl_deref_val(opts); pl_is_cons(cur); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
        e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        if (e.v != (DTYPE_t)DT_PLREF || plc_fid_arity(e.slen) != 1) continue;
        fn = prolog_atom_name(plc_fid_name(e.slen));
        as = pl_atom_str(rt_pl_deref_val(((DESCR_t *)e.p)[0]));
        if (!fn || !as) continue;
        if (!strcmp(fn, "syntax_error")) m = !strcmp(as, "fail") ? 1 : !strcmp(as, "warning") ? 2 : 0;
        else if (!strcmp(fn, "syntax_errors")) m = (!strcmp(as, "fail") || !strcmp(as, "quiet")) ? 1 : !strcmp(as, "dec10") ? 3 : 0;
    }
    return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_flag_t * pl_flag_find(const char *nm) { pl_flags_ready(); for (int i = 0; pl_flags[i].nm; i++) if (!strcmp(nm, pl_flags[i].nm)) return &pl_flags[i]; return (pl_flag_t *)0; }
const char *pl_flag_encoding(void) { pl_flag_t *fl = pl_flag_find("encoding"); return fl && fl->val ? fl->val : "UTF-8"; }
const char *rt_pl_flag_text(const char *nm) { pl_flag_t *fl = pl_flag_find(nm); return fl && fl->val ? fl->val : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_dialect_swi(void) { return pl_swi_dialect(); }
int rt_pl_dialect_gnu(void) { return pl_gnu_dialect(); }
int rt_pl_dialect_xsb(void) { return pl_xsb_dialect(); }
int rt_pl_iso_mode(void) { pl_flag_t *fl = pl_flag_find("iso"); return fl && !strcmp(fl->val, "true"); }
int pl_float_flag(const char *nm, const char *val) { pl_flag_t *fl = pl_flag_find(nm); return fl && fl->val && !strcmp(fl->val, val); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_protect_static_code(void) { pl_flag_t *fl = pl_flag_find("protect_static_code"); return fl && !strcmp(fl->val, "true"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_dq_swap(const char *v) { pl_flag_t *fl = pl_flag_find("double_quotes"); if (!fl) return (const char *)0; const char *old = fl->val; if (v) fl->val = v; return old; }
int rt_pl_double_quotes_mode(void) {
    pl_flag_t *fl = pl_flag_find("double_quotes");
    const char *v = fl ? fl->val : NULL;
    if (!v) return 2;
    return !strcmp(v, "atom") ? 0 : !strcmp(v, "chars") ? 1 : !strcmp(v, "string") ? 3 : 2;
}
PL_CX_LEAF_HEAD(gnu_last_read_start, 2)
    { extern int fh_last_read_start(long *, long *); long l = 0, c = 0; (void)fh_last_read_start(&l, &c); ok = plw_unify_vals(args[0], INTVAL(l), cx) && plw_unify_vals(args[1], INTVAL(c), cx); }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pl_gnu_cat3(const char *x, const char *y, const char *z) {
    size_t lx = strlen(x), ly = strlen(y), lz = strlen(z);
    char *o = (char *)rt_wsb_alloc(lx + ly + lz + 1);
    memcpy(o, x, lx);
    memcpy(o + lx, y, ly);
    memcpy(o + lx + ly, z, lz);
    o[lx + ly + lz] = 0;
    return o;
}
char *pl_gnu_cwd(void) {
    for (size_t n = 256; n < ((size_t)1 << 30); n *= 2) { char *b = (char *)rt_wsb_alloc(n); if (getcwd(b, n)) return b; if (errno != ERANGE) return (char *)0; }
    return (char *)0;
}
char *pl_gnu_abs_path(const char *src) {
    extern int rt_pl_dialect_swi(void);
    extern const char *rt_pl_flag_text(const char *);
    const char *fv = rt_pl_flag_text("file_name_variables");
    int ex = !rt_pl_dialect_swi() || (fv && !strcmp(fv, "true"));
    size_t need = 1, k = 0;
    char *a = (char *)0, *out;
    size_t o = 0, n;
    for (int pass = 0; pass < 2; pass++) {
        if (pass) { a = (char *)rt_wsb_alloc(need); k = 0; }
        for (const char *p = src; *p; ) {
            if (ex && *p == '$') {
                const char *q = p + 1;
                while (isalnum((unsigned char)*q) || *q == '_') q++;
                if (q > p + 1) {
                    char *nm = (char *)rt_wsb_alloc((size_t)(q - p));
                    const char *v;
                    memcpy(nm, p + 1, (size_t)(q - p - 1));
                    nm[q - p - 1] = 0;
                    if ((v = getenv(nm))) { size_t L = strlen(v); if (pass) memcpy(a + k, v, L); k += L; if (!pass) need += L; p = q; continue; }
                }
            }
            if (pass) a[k] = *p;
            k++;
            if (!pass) need++;
            p++;
        }
        if (pass) a[k] = 0;
    }
    if (ex && a[0] == '~' && (a[1] == '/' || a[1] == 0)) { const char *h = getenv("HOME"); if (!h) return (char *)0; a = pl_gnu_cat3(h, "/", a + 1); } else if (ex && a[0] == '~') return (char *)0;
    if (!strcmp(a, "user")) return a;
    if (!a[0]) return (char *)0;
    if (a[0] != '/') { char *cwd = pl_gnu_cwd(); if (!cwd) return (char *)0; a = pl_gnu_cat3(cwd, "/", a); }
    n = strlen(a);
    out = (char *)rt_wsb_alloc(n + 2);
    for (size_t i = 0; i < n; ) {
        size_t j = i;
        while (j < n && a[j] == '/') j++;
        size_t e = j;
        while (e < n && a[e] != '/') e++;
        if (e == j) break;
        if (e - j == 1 && a[j] == '.') { i = e; continue; }
        if (e - j == 2 && a[j] == '.' && a[j + 1] == '.') { if (o == 0) return (char *)0; while (o > 0 && out[o - 1] != '/') o--; if (o > 0) o--; i = e; continue; }
        out[o++] = '/';
        memcpy(out + o, a + j, e - j);
        o += e - j;
        i = e;
    }
    if (o == 0) out[o++] = '/';
    out[o] = 0;
    return out;
}
PL_CX_LEAF_HEAD(gnu_absolute_file_name, 2)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        extern int rt_pl_dialect_swi(void);
        DESCR_t f = rt_pl_deref_val(args[0]);
        const char *nm = pl_atom_str(f);
        char *ab;
        ok = 0;
        if (pl_val_unbound(f)) cx->ball = rt_pl_ball_instantiation();
        else if (!nm) cx->ball = rt_pl_ball_kind2("type_error", "atom", f);
        else if (!(ab = pl_gnu_abs_path(nm))) cx->ball = rt_pl_ball_kind2("domain_error", "os_path", f);
        else {
            if (rt_pl_dialect_swi() && nm[0] && nm[strlen(nm) - 1] == '/' && strcmp(ab, "/") && ab[strlen(ab) - 1] != '/') ab = pl_gnu_cat3(ab, "/", "");
            ok = plw_unify_vals(args[1], pl_mk_atom_dup(ab, strlen(ab)), cx);
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_prolog_file_name, 2)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        const char *const sfx[] = { ".pl", ".pro", ".prolog", 0 };
        DESCR_t f = rt_pl_deref_val(args[0]);
        const char *nm = pl_atom_str(f), *base, *use = ".pl";
        char *ab, *cand;
        ok = 0;
        if (pl_val_unbound(f)) cx->ball = rt_pl_ball_instantiation();
        else if (!nm) cx->ball = rt_pl_ball_kind2("type_error", "atom", f);
        else if (!(ab = pl_gnu_abs_path(nm))) cx->ball = rt_pl_ball_kind2("domain_error", "os_path", f);
        else if (!strcmp(ab, "user") || strchr((base = strrchr(ab, '/')) ? base : ab, '.')) ok = plw_unify_vals(args[1], f, cx);
        else {
            for (int i = 0; sfx[i]; i++) if (!access(pl_gnu_cat3(ab, sfx[i], ""), F_OK)) { use = sfx[i]; break; }
            cand = pl_gnu_cat3(nm, use, "");
            ok = plw_unify_vals(args[1], pl_mk_atom_dup(cand, strlen(cand)), cx);
        }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(gnu_builtin, 2)
    {
        extern int pl_pi_is_builtin(const char *, int);
        DESCR_t a = rt_pl_deref_val(args[1]);
        const char *nm = pl_atom_str(rt_pl_deref_val(args[0]));
        ok = a.v == (DTYPE_t)DT_I && nm && pl_pi_is_builtin(nm, (int)a.i);
    }
PL_CX_LEAF_TAIL
static int64_t pl_nb_cell_k(DESCR_t k, void *root) {
    extern int rt_pl_db_cell_for(void *, const char *, int);
    extern const char *prolog_atom_name(int);
    if (k.v == DT_I) return k.i;
    if (k.v == (DTYPE_t)DT_PLATOM) { const char *nm = prolog_atom_name((int)k.i); return nm ? (int64_t)rt_pl_db_cell_for(root, nm, -1) : -1; }
    if (k.v == DT_S && k.s) return (int64_t)rt_pl_db_cell_for(root, k.s, -1);
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_nb_setval_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    { DESCR_t k = rt_pl_deref_val(args[0]); int64_t kk = pl_nb_cell_k(k, root); if (kk < 0) return FAILDESCR; return rt_pl_nb_set(root, kk, (void *)&args[1]) ? pl_ok() : FAILDESCR; }
}
DESCR_t rt_pl_dop_nb_delete_c(DESCR_t *args, int nargs, void *root) {
    extern void rt_pl_nb_unset(void *, int64_t);
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    { DESCR_t k = rt_pl_deref_val(args[0]); int64_t kk = pl_nb_cell_k(k, root); if (kk >= 0) rt_pl_nb_unset(root, kk); return pl_ok(); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_nb_getval_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    {
        DESCR_t k = rt_pl_deref_val(args[0]);
        int ok;
        int64_t kk = pl_nb_cell_k(k, root);
        if (kk < 0) { rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        ok = rt_pl_nb_get_cell(root, kk, (void *)&args[1], cx);
        rt_pl_tr_gc_sync(cx->tr);
        return ok ? pl_ok() : FAILDESCR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_nb_getval_guard_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_nb_is_set(void *, int64_t);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    if (nargs != 2) return (void *)0;
    pl_atoms_ready();
    {
        DESCR_t k = rt_pl_deref_val(args[0]);
        int64_t kk = pl_nb_cell_k(k, root);
        if (kk < 0) return (void *)0;
        if (rt_pl_nb_is_set(root, kk)) return (void *)0;
        return rt_pl_ball_kind2("existence_error", "variable", args[1]);
    }
}
DESCR_t rt_pl_dop_b_setval_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    {
        DESCR_t k = rt_pl_deref_val(args[0]);
        int64_t kk = pl_nb_cell_k(k, root);
        if (kk < 0) return FAILDESCR;
        int ok = rt_pl_b_set(root, kk, (void *)&args[1], cx);
        rt_pl_tr_gc_sync(cx->tr);
        return ok ? pl_ok() : FAILDESCR;
    }
}
int pl_seed_src(void *db, DESCR_t *fl, int ok) {
    extern int rt_pl_db_src_set_last(void *, int, int);
    DESCR_t fv = rt_pl_deref_val(fl[0]), lv = rt_pl_deref_val(fl[1]);
    if (ok) rt_pl_db_src_set_last(db, (int)fv.v == DT_PLATOM ? (int)fv.i : -1, lv.v == DT_I ? (int)lv.i : 0);
    return ok;
}
DESCR_t rt_pl_dop_db_seed_once_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 5) return FAILDESCR;
    pl_atoms_ready();
    {
        extern void *rt_pl_db_get_by_key(void *, const char *, int);
        DESCR_t kv = rt_pl_deref_val(args[0]);
        const char *ks = kv.v == DT_I ? (const char *)0 : pl_atom_str(kv);
        void *db = ks ? rt_pl_db_get_by_key(root, ks, 1) : pl_db_cell_of(args, root);
        DESCR_t iv = rt_pl_deref_val(args[1]);
        if (!db || iv.v != DT_I) return FAILDESCR;
        if (rt_pl_db_count(db) > (int)iv.i) return pl_ok();
        {
            extern int rt_pl_db_consult(void *, void *, void *, int, int);
            extern int rt_pl_db_key_is_declared(void *, const char *);
            char key[bnd_db_key_need((void *)&args[2])];
            int ar = 0;
            if (rt_pl_db_term_key((void *)&args[2], key, sizeof key, &ar)) return pl_seed_src(db, &args[3], rt_pl_db_consult(root, db, (void *)&args[2], rt_pl_db_key_is_declared(root, key), 3)) ?
                pl_ok() : FAILDESCR;
        }
        return pl_seed_src(db, &args[3], rt_pl_db_seed(root, db, (void *)&args[2])) ? pl_ok() : FAILDESCR;
    }
}
static void *pl_db_static_ball(void *root, const char *op, const char *nm, int ar) {
    extern int rt_pl_db_key_is_static(void *, const char *);
    extern int rt_pl_iso_mode(void);
    extern int rt_pl_protect_static_code(void);
    extern int pl_pi_is_builtin(const char *, int);
    extern int rt_pl_db_key_lib_take(void *, const char *);
    char key[fmt_len("%s/%d", nm, ar)];
    int isclause = !strcmp(op, "clause");
    int bi;
    snprintf(key, sizeof key, "%s/%d", nm, ar);
    if (!strcmp(op, "assert") && rt_pl_db_key_lib_take(root, key)) return (void *)0;
    bi = pl_pi_is_control(nm, ar) || (isclause && pl_pi_is_builtin(nm, ar) && !rt_pl_db_key_is_static(root, key) && !rt_pl_db_key_is_dynamic(root, key));
    if (!bi && (rt_pl_db_key_is_dynamic(root, key) || !(rt_proc_is_registered(key) || (isclause && rt_pl_db_key_is_static(root, key))))) return (void *)0;
    if (isclause) { if (!bi && !rt_pl_iso_mode() && !rt_pl_protect_static_code() && !pl_gnu_dialect()) return (void *)0; return rt_pl_ball_permission_pi("access", "private_procedure", nm, ar); }
    return rt_pl_ball_permission_pi("modify", "static_procedure", nm, ar);
}
void * rt_pl_dop_db_t_guard_c(DESCR_t *args, int nargs, void *root) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    extern int rt_proc_is_registered(const char *);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_kind1(const char *, const char *);
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_culprit1(const char *, DESCR_t);
    extern void *rt_pl_ball_kind1_system_ctx(const char *, const char *, const char *, int, const char *);
    char ob[32];
    const char *op;
    const char *opname;
    int ar = 0;
    if (nargs != 2) return (void *)0;
    pl_atoms_ready();
    if (!pl_cell_text(args[1], ob, sizeof ob, &op) || !op) op = "assert";
    opname = op;
    if (!strcmp(op, "asserta") || !strcmp(op, "assertz")) op = "assert";
    DESCR_t ua[2] = { args[0], args[1] };
    args = ua;
    if (!strcmp(op, "abolish")) {
        if (!pl_xsb_dialect()) rt_pl_db_unqual_pi((void *)&args[0]);
    } else if (!strcmp(op, "assert") || !strcmp(op, "assert_cyc") || !strcmp(op, "retract") || !strcmp(op, "retractall")) rt_pl_db_unqual_term((void *)&args[0]);
    {
        DESCR_t t = rt_pl_deref_val(args[0]);
        if (!strcmp(op, "clref_out")) return pl_iso_unbound(t) ? (void *)0 : rt_pl_ball_culprit1("uninstantiation_error", t);
        if (!strcmp(op, "clref_in") || !strcmp(op, "clref_opt")) {
            if (pl_iso_unbound(t)) return strcmp(op, "clref_in") ? (void *)0 : rt_pl_ball_instantiation();
            if (!pl_cell_is_clref(t)) return rt_pl_ball_kind2("type_error", "db_reference", t);
            return (void *)0;
        }
        if ((!strcmp(op, "assert") || !strcmp(op, "assert_cyc")) && !rt_pl_acyclic_cell((void *)&args[0])) return rt_pl_ball_kind1("representation_error", "cyclic_term");
        if (!strcmp(op, "assert_cyc")) {
            extern int rt_pl_db_space_limited(void *);
            extern void *rt_pl_db_space_ball(void *, const char *, void *);
            if (!rt_pl_db_space_limited(root)) return (void *)0;
            { char key[bnd_db_key_need((void *)&args[0])]; if (!rt_pl_db_term_key((void *)&args[0], key, sizeof key, &ar)) return (void *)0; return rt_pl_db_space_ball(root, key, (void *)&args[0]); }
        }
        if (pl_iso_unbound(t)) return rt_pl_ball_instantiation();
        if (!strcmp(op, "abolish")) {
            DESCR_t n, a;
            const char *ns;
            if (pl_xsb_dialect() && ((int)t.v == DT_PLATOM || t.v == DT_S || ((int)t.v == DT_PLREF && !(plc_fid_arity(t.slen) == 2 && plc_fid_name(t.slen) == prolog_atom_intern(":")))))
                return (void *)0;
            if (!(t.v == (DTYPE_t)DT_PLREF && plc_fid_arity(t.slen) == 2 && plc_fid_name(t.slen) == prolog_atom_intern("/"))) return rt_pl_ball_kind2("type_error", "predicate_indicator", t);
            n = rt_pl_deref_val(((DESCR_t *)t.p)[0]);
            a = rt_pl_deref_val(((DESCR_t *)t.p)[1]);
            if (pl_iso_unbound(n) || pl_iso_unbound(a)) return rt_pl_ball_instantiation();
            if (!pl_anum_is_text(n)) return rt_pl_ball_kind2("type_error", "atom", n);
            if (a.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", a);
            if ((long long)a.i < 0) return rt_pl_ball_kind2("domain_error", "not_less_than_zero", a);
            if (pl_swi_dialect() && (long long)a.i > PROLOG_MAX_PROCEDURE_ARITY) return pl_procedure_arity_ball(opname, (int)a.i);
            if ((long long)a.i > PROLOG_MAX_ARITY) return rt_pl_ball_kind1("representation_error", "max_arity");
            ns = pl_atom_str(n);
            return pl_db_static_ball(root, op, ns ? ns : "?", (int)a.i);
        }
        {
            DESCR_t h = t;
            DESCR_t b;
            int hasbody = 0;
            if (t.v == (DTYPE_t)DT_PLREF && plc_fid_arity(t.slen) == 2 && plc_fid_name(t.slen) == prolog_atom_intern(":-")) {
                h = rt_pl_deref_val(((DESCR_t *)t.p)[0]);
                b = ((DESCR_t *)t.p)[1];
                hasbody = 1;
            }
            if (pl_iso_unbound(h)) return rt_pl_ball_instantiation();
            if (!pl_anum_is_compound(h) && !pl_anum_is_text(h)) return rt_pl_ball_kind2("type_error", "callable", h);
            if (pl_swi_dialect() && !strcmp(op, "assert") && h.v == (DTYPE_t)DT_PLREF && plc_fid_arity(h.slen) > PROLOG_MAX_PROCEDURE_ARITY)
                return pl_procedure_arity_ball(opname, plc_fid_arity(h.slen));
            if (hasbody && !strcmp(op, "assert") && pl_db_body_ill_typed(b)) return rt_pl_ball_kind2("type_error", "callable", b);
            if (hasbody && (!strcmp(op, "clause") || (!strcmp(op, "retract") && pl_gnu_dialect()))) {
                DESCR_t bd = rt_pl_deref_val(b);
                if (bd.v == DT_I || bd.v == DT_R) return rt_pl_ball_kind2("type_error", "callable", bd);
            }
            char key[bnd_db_key_need((void *)&args[0])];
            if (!rt_pl_db_term_key((void *)&args[0], key, sizeof key, &ar)) return (void *)0;
            if (!strcmp(op, "assert")) { extern void *rt_pl_db_space_ball(void *, const char *, void *); void *sb = rt_pl_db_space_ball(root, key, (void *)&args[0]); if (sb) return sb; }
            { char *sl = strrchr(key, '/'); if (sl) *sl = 0; return pl_db_static_ball(root, op, key, ar); }
        }
    }
}
static DESCR_t pl_db_add_r(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root, int prepend) {
    extern void *rt_pl_compound_cell(const char *, int, void *);
    extern int rt_pl_db_assert_ref(void *, void *, void *, int);
    extern int rt_pl_db_term_key(void *, char *, size_t, int *);
    extern void *rt_pl_db_get_by_key(void *, const char *, int);
    size_t kn;
    char *key;
    int kid;
    int ar = 0;
    int ref;
    int ok;
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    kn = (size_t)bnd_db_key_need((void *)&args[0]);
    key = pl_atom_carve_cx(kn - 1, cx);
    if (!key) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    if (!rt_pl_db_term_key((void *)&args[0], key, kn, &ar)) { prolog_atom_uncarve(key, kn - 1); rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
    kid = prolog_atom_carved(key, kn - 1, strlen(key));
    key = (char *)prolog_atom_name(kid);
    {
        void *db = rt_pl_db_get_by_key(root, key, 1);
        if (!db) { rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        ref = rt_pl_db_assert_ref(root, db, (void *)&args[0], prepend);
        if (ref < 1) { rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
    }
    {
        DESCR_t kids[2];
        DESCR_t *rc;
        kids[0] = pl_mk_atom(key);
        kids[1] = INTVAL((long long)ref);
        rc = (DESCR_t *)rt_pl_compound_cell("$clref", 2, (void *)kids);
        if (!rc) { rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
        ok = plw_unify_vals(args[1], *rc, cx);
    }
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_db_assertz_r_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) { return pl_db_add_r(args, nargs, cx, root, 0); }
DESCR_t rt_pl_dop_db_asserta_r_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) { return pl_db_add_r(args, nargs, cx, root, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_consult_t_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_db_consult(void *, void *, void *, int, int);
    extern int rt_pl_db_key_is_declared(void *, const char *);
    char key[bnd_db_key_need((void *)&args[0])];
    int ar = 0;
    void *db;
    if (nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    if (!rt_pl_db_term_key((void *)&args[0], key, sizeof key, &ar)) return FAILDESCR;
    db = rt_pl_db_get_by_key(root, key, 1);
    if (!db) return FAILDESCR;
    return pl_seed_src(db, &args[1], rt_pl_db_consult(root, db, (void *)&args[0], rt_pl_db_key_is_declared(root, key), 1)) ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_db_abolish_t_c(DESCR_t *args, int nargs, void *root) {
    extern int prolog_atom_intern(const char *);
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    if (!pl_xsb_dialect()) rt_pl_db_unqual_pi((void *)&args[0]);
    {
        DESCR_t t = rt_pl_deref_val(args[0]);
        if (pl_xsb_dialect() && !(t.v == (DTYPE_t)DT_PLREF && plc_fid_arity(t.slen) == 2 && (plc_fid_name(t.slen) == prolog_atom_intern("/") || plc_fid_name(t.slen) == prolog_atom_intern(":"))) &&
            ((int)t.v == DT_PLATOM || t.v == DT_S || (int)t.v == DT_PLREF)) {
            const char *hn = (int)t.v == DT_PLREF ? prolog_atom_name(plc_fid_name(t.slen)) : pl_atom_str(t);
            int ha = (int)t.v == DT_PLREF ? plc_fid_arity(t.slen) : 0;
            char hk[fmt_len("%s/%d", hn ? hn : "?", ha)];
            void *hdb;
            snprintf(hk, sizeof hk, "%s/%d", hn ? hn : "?", ha);
            hdb = rt_pl_db_get_by_key(root, hk, 0);
            if (!hdb) return pl_ok();
            return rt_pl_db_abolish(root, hdb) ? pl_ok() : FAILDESCR;
        }
        if (!(t.v == (DTYPE_t)DT_PLREF && plc_fid_arity(t.slen) == 2 && plc_fid_name(t.slen) == prolog_atom_intern("/"))) return FAILDESCR;
        {
            DESCR_t n = rt_pl_deref_val(((DESCR_t *)t.p)[0]);
            DESCR_t a = rt_pl_deref_val(((DESCR_t *)t.p)[1]);
            const char *ns = pl_atom_str(n);
            if (!ns || a.v != DT_I) return FAILDESCR;
            char key[fmt_len("%s/%d", ns, (int)a.i)];
            snprintf(key, sizeof key, "%s/%d", ns, (int)a.i);
            { void *db = rt_pl_db_get_by_key(root, key, 0); if (!db) return pl_ok(); return rt_pl_db_abolish(root, db) ? pl_ok() : FAILDESCR; }
        }
    }
}
