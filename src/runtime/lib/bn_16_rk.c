#include "runtime/lib/bn_head.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/core/nv.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include <ctype.h>
#include <fcntl.h>
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_09_rk.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_15_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_18_rk.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_23_rk.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/core/g_runtime_bn.h"
void rt_call_args_need(int n);
static DESCR_t rk_exc_from_msg(const char *msg);
extern FILE *fh_get(int);
extern unsigned rt_dtax_gen;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_join_gists(const char **tx, int n, const char *sep, const char *open, const char *close) {
    size_t sl = strlen(sep), ol = strlen(open), cl = strlen(close), tot = ol + cl + 1;
    for (int i = 0; i < n; i++) tot += strlen(tx[i]) + sl;
    char *buf = rt_wsb_alloc(tot);
    size_t p = 0;
    memcpy(buf, open, ol);
    p = ol;
    for (int i = 0; i < n; i++) { if (i) { memcpy(buf + p, sep, sl); p += sl; } size_t L = strlen(tx[i]); memcpy(buf + p, tx[i], L); p += L; }
    memcpy(buf + p, close, cl);
    p += cl;
    buf[p] = '\0';
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_raku_scalar(DESCR_t v) {
    if (v.v == DT_SNUL) return "Nil";
    if (rk_typeobj_name(v)) return rk_typeobj_name(v);
    if (v.v == DT_DATA) { extern int rk_rat_is(DESCR_t); extern const char *rk_rat_str(DESCR_t, int); if (rk_rat_is(v)) return rk_rat_str(v, 1); }
    if (IS_REAL_fn(v)) { const char *t = rk_cstr(v); if (strpbrk(t, "eEIN")) return t; char *r = (char *) rt_wsb_alloc(strlen(t) + 3); sprintf(r, "%se0", t); return r; }
    if (v.v == DT_BOOL) return v.i ? "Bool::True" : "Bool::False";
    if (IS_INT_fn(v) || IS_REAL_fn(v) || v.v == DT_BIG) return rk_cstr(v);
    const char *s = rk_cstr(v);
    size_t n = strlen(s);
    char *r = (char *) rt_wsb_alloc(6 * n + 3);
    size_t p = 0;
    r[p++] = '"';
    for (size_t i = 0; i < n; i++) {
        unsigned char u = (unsigned char) s[i];
        if (s[i] == '\n') { r[p++] = '\\'; r[p++] = 'n'; continue; }
        if (s[i] == '\t') { r[p++] = '\\'; r[p++] = 't'; continue; }
        if (s[i] == '\r') { r[p++] = '\\'; r[p++] = 'r'; continue; }
        if (s[i] == '\b') { r[p++] = '\\'; r[p++] = 'b'; continue; }
        if (s[i] == '\0') { r[p++] = '\\'; r[p++] = '0'; continue; }
        if (u < 0x20 || u == 0x7f || (u == 0xC2 && i + 1 < n && (unsigned char) s[i + 1] >= 0x80 && (unsigned char) s[i + 1] <= 0x9f)) {
            unsigned cp = u == 0xC2 ? (unsigned char) s[++i] : u;
            p += (size_t) sprintf(r + p, "\\x[%X]", cp);
            continue;
        }
        if (s[i] == '"' || s[i] == '\\' || s[i] == '$' || s[i] == '@' || s[i] == '%' || s[i] == '&' || s[i] == '{') r[p++] = '\\';
        r[p++] = s[i];
    }
    r[p++] = '"';
    r[p] = '\0';
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_ident_key(const char *k) {
    if (!*k) return 0;
    if (!(isalpha((unsigned char) *k) || *k == '_')) return 0;
    for (const char *p = k; *p; p++) if (!(isalnum((unsigned char) *p) || *p == '_' || *p == '-')) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_raku_str(DESCR_t v, int in_hash) {
    rk_reify_deep(v, 0);
    if (!in_hash && v.v == DT_A && v.arr && rk_is_slip(v) && rk_av(v).n == 0) return "Empty";
    if (rk_match_is(v)) {
        if (!(int) FIELD_GET_fn(v, "ok").i) return "Nil";
        DESCR_t pa = FIELD_GET_fn(v, "positional"), na = FIELD_GET_fn(v, "named");
        int np = (pa.v == DT_A && pa.arr) ? ((ARBLK_t *) pa.arr)->hi - ((ARBLK_t *) pa.arr)->lo + 1 : 0;
        const char *os = rk_raku_scalar(FIELD_GET_fn(v, "orig")), *ls = np ? rk_raku_str(pa, 0) : "";
        const char **tx = (na.v == DT_T && na.tbl && na.tbl->size) ? (const char **) rt_pvec_alloc((size_t) na.tbl->size) : NULL;
        int c = 0;
        if (tx) for (unsigned i = 0; i < na.tbl->ord_len; i++) {
            DESCR_t key = na.tbl->ord[i];
            if (key.v == DT_RAW) continue;
            TBPAIR_t *e = table_find_pair_d(na.tbl, key);
            if (!e) continue;
            const char *kt = rk_cstr(key), *vt = rk_raku_str(rt_deref(e->val), 0);
            char *ln = rt_wsb_alloc(strlen(kt) + strlen(vt) + 4);
            sprintf(ln, ":%s(%s)", kt, vt);
            tx[c++] = ln;
        }
        const char *hs = c ? rk_join_gists(tx, c, ",", "Map.new((", "))") : "";
        char *o = rt_wsb_alloc(strlen(os) + strlen(ls) + strlen(hs) + 96);
        sprintf(o, "Match.new(:orig(%s), :from(%d), :pos(%d)", os, (int) FIELD_GET_fn(v, "from").i, (int) FIELD_GET_fn(v, "to").i);
        if (np) { strcat(o, ", :list("); strcat(o, ls); strcat(o, ")"); }
        if (c) { strcat(o, ", :hash("); strcat(o, hs); strcat(o, ")"); }
        strcat(o, ")");
        return o;
    }
    if (v.v == DT_T && v.tbl) {
        TBBLK_t *t = v.tbl;
        const char **tx = t->size ? (const char **) rt_pvec_alloc((size_t) t->size) : NULL;
        int c = 0;
        for (unsigned i = 0; i < t->ord_len; i++) {
            DESCR_t key = t->ord[i];
            if (key.v == DT_RAW) continue;
            TBPAIR_t *e = table_find_pair_d(t, key);
            if (!e) continue;
            const char *kt = rk_cstr(key), *vt = rk_raku_str(rt_deref(e->val), 1);
            size_t kl = strlen(kt), vl = strlen(vt);
            char *ln;
            if (rk_ident_key(kt)) {
                ln = rt_wsb_alloc(kl + vl + 4);
                ln[0] = ':';
                memcpy(ln + 1, kt, kl);
                ln[1 + kl] = '(';
                memcpy(ln + 2 + kl, vt, vl);
                ln[2 + kl + vl] = ')';
                ln[3 + kl + vl] = '\0';
            } else {
                const char *kq = rk_raku_scalar(key);
                size_t ql = strlen(kq);
                ln = rt_wsb_alloc(ql + vl + 5);
                memcpy(ln, kq, ql);
                memcpy(ln + ql, " => ", 4);
                memcpy(ln + ql + 4, vt, vl);
                ln[ql + 4 + vl] = '\0';
            }
            tx[c++] = ln;
        }
        return rk_join_gists(tx, c, ", ", "{", "}");
    }
    if (rk_is_pair(v)) {
        rk_av_t a = rk_av(v);
        const char *kt = rk_av_text(a, 0), *vt = rk_raku_str(a.el[1], 0);
        size_t kl = strlen(kt), vl = strlen(vt);
        if (rk_ident_key(kt) && a.el[1].v == DT_BOOL) { char *ln = rt_wsb_alloc(kl + 3); sprintf(ln, a.el[1].i ? ":%s" : ":!%s", kt); return ln; }
        if (rk_ident_key(kt)) {
            char *ln = rt_wsb_alloc(kl + vl + 4);
            ln[0] = ':';
            memcpy(ln + 1, kt, kl);
            ln[1 + kl] = '(';
            memcpy(ln + 2 + kl, vt, vl);
            ln[2 + kl + vl] = ')';
            ln[3 + kl + vl] = '\0';
            return ln;
        }
        const char *kq = rk_is_pair(a.el[0]) ? rk_paren(rk_raku_str(a.el[0], 0)) : rk_raku_scalar(a.el[0]);
        size_t ql = strlen(kq);
        char *ln = rt_wsb_alloc(ql + vl + 5);
        memcpy(ln, kq, ql);
        memcpy(ln + ql, " => ", 4);
        memcpy(ln + ql + 4, vt, vl);
        ln[ql + 4 + vl] = '\0';
        return ln;
    }
    if (rk_is_jct(v)) {
        rk_av_t a = rk_av(v);
        const char *fl = ((ARBLK_t *) v.arr)->proto;
        const char **tx = a.n ? (const char **) rt_pvec_alloc((size_t) a.n) : NULL;
        for (int i = 0; i < a.n; i++) tx[i] = rk_raku_str(a.el[i], 0);
        size_t fln = strlen(fl);
        char *op = rt_wsb_alloc(fln + 2);
        memcpy(op, fl, fln);
        op[fln] = '(';
        op[fln + 1] = '\0';
        return rk_join_gists(tx, a.n, ", ", op, ")");
    }
    if (rk_is_range(v)) return rk_range_text(v);
    if (v.v == DT_A && v.arr) {
        rk_av_t a = rk_av(v);
        const char **tx = a.n ? (const char **) rt_pvec_alloc((size_t) a.n) : NULL;
        for (int i = 0; i < a.n; i++) tx[i] = rk_raku_str(a.el[i], 0);
        int sq = rk_is_seq(v), lst = sq || rk_is_list(v) || rk_is_slip(v), sl = !sq && rk_is_slip(v);
        if (lst && a.n == 1) { const char *one = tx[0]; size_t L = strlen(one); char *b = rt_wsb_alloc(L + 16); snprintf(b, L + 16, "%s(%s,)%s", sl ? "slip" : "", one, sq ? ".Seq" : ""); return b; }
        const char *body = rk_join_gists(tx, a.n, ", ", sl ? "slip(" : lst ? "(" : "[", lst ? ")" : "]");
        if (sq) { size_t L = strlen(body); char *b = rt_wsb_alloc(L + 5); snprintf(b, L + 5, "%s.Seq", body); return b; }
        if ((in_hash && !lst) || rk_is_item(v)) { size_t L = strlen(body); char *b = rt_wsb_alloc(L + 2); b[0] = '$'; memcpy(b + 1, body, L + 1); return b; }
        return body;
    }
    return rk_raku_scalar(v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_dat_tname(const char *nm) { return nm && nm[0] == 1 ? nm + 1 : nm; }
const char *rk_value_type(DESCR_t v) {
    const char *t = rk_typeobj_name(v);
    if (t) return t;
    if (v.v == DT_BOOL) return "Bool";
    if (v.v == DT_FH) return "IO::Handle";
    if (v.v == DT_ORDER) return "Order";
    if (IS_INT_fn(v) || v.v == DT_BIG) return "Int";
    if (IS_REAL_fn(v)) return "Num";
    if (v.v == DT_SNUL) return "Nil";
    if (v.v == DT_T) return "Hash";
    if (v.v == DT_A) return rk_is_pair(v) ? "Pair" : rk_is_jct(v) ? "Junction" : rk_is_seq(v) ? "Seq" : ((ARBLK_t *) v.arr)->proto == rk_proto_list ? "List" :
        ((ARBLK_t *) v.arr)->proto == rk_proto_range ? "Range" : ((ARBLK_t *) v.arr)->proto == rk_proto_slip ? "Slip" : "Array";
    if (IS_DATA_INST_fn(v) && v.u) { DATINST_t *di = (DATINST_t *)v.u; return (di && di->type) ? rk_dat_tname(di->type->name) : "Any"; }
    return "Str";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_combos_of(rk_av_t a, int k, DESCR_t outl) {
    if (k < 0 || k > a.n) return;
    int ix[k > 0 ? k : 1];
    for (int i = 0; i < k; i++) ix[i] = i;
    for (;;) {
        DESCR_t row[k > 0 ? k : 1];
        for (int i = 0; i < k; i++) row[i] = a.el[ix[i]];
        rk_arr_append((ARBLK_t *) outl.arr, rk_mk_arr(row, k));
        int i = k - 1;
        while (i >= 0 && ix[i] == a.n - k + i) i--;
        if (i < 0) break;
        ix[i]++;
        for (int j = i + 1; j < k; j++) ix[j] = ix[j - 1] + 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_seq_num2(DESCR_t a, DESCR_t b, int op) {
    double da, db;
    int ia, ib;
    rk_range_num(a, &da, &ia);
    rk_range_num(b, &db, &ib);
    if (ia && ib) {
        long long x = IS_INT_fn(a) ? a.i : (long long) da, y = IS_INT_fn(b) ? b.i : (long long) db, r;
        int ov = op == 0 ? __builtin_add_overflow(x, y, &r) : op == 1 ? __builtin_sub_overflow(x, y, &r) : __builtin_mul_overflow(x, y, &r);
        if (!ov) return INTVAL(r);
    }
    return REALVAL(op == 0 ? da + db : op == 1 ? da - db : da * db);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_seq_plan(DESCR_t seedv, int gen_code, DESCR_t ev) {
    rk_av_t sd = rk_av(seedv);
    int nseed = sd.n;
    int infinite = rk_typeobj_name(ev) != NULL || (IS_REAL_fn(ev) && isinf(ev.r)) || ev.v == DT_SNUL;
    double dE = 0;
    int iE = 0, endnum = !infinite && !rk_is_code(ev) && rk_range_num(ev, &dE, &iE);
    int numeric_seeds = nseed > 0;
    for (int i = 0; i < nseed; i++) { double d; int ii; if (!rk_range_num(sd.el[i], &d, &ii)) numeric_seeds = 0; }
    int dir = 1, kind = gen_code ? 4 : numeric_seeds ? 1 : 3;
    DESCR_t step = INTVAL(1);
    if (!gen_code && numeric_seeds) {
        double d0, d1, d2;
        int i0, i1, i2;
        if (nseed == 1) {
            rk_range_num(sd.el[0], &d0, &i0);
            dir = endnum && dE < d0 ? -1 : 1;
            step = INTVAL(dir);
        } else {
            rk_range_num(sd.el[nseed - 2], &d1, &i1);
            rk_range_num(sd.el[nseed - 1], &d2, &i2);
            step = rk_seq_num2(sd.el[nseed - 1], sd.el[nseed - 2], 1);
            if (nseed >= 3) {
                rk_range_num(sd.el[nseed - 3], &d0, &i0);
                if (d2 - d1 != d1 - d0) {
                    if (d0 != 0 && d1 != 0 && d1 / d0 == d2 / d1) {
                        kind = 2;
                        double r = d2 / d1;
                        step = (i0 && i1 && i2 && r == (double) (long long) r) ? INTVAL((long long) r) : REALVAL(r);
                    } else {
                        rk_exc_init();
                        rk_exc_throw(rk_exc_adhoc(STRVAL(rt_heap_strdup_c("Unable to deduce arithmetic or geometric sequence from the seeds"))));
                        return rk_mk_arr(NULL, 0);
                    }
                }
            }
            double ds = kind == 2 ? (d2 * (IS_REAL_fn(step) ? step.r : (double) step.i) - d2) : d2 - d1;
            dir = ds < 0 ? -1 : 1;
        }
    }
    DESCR_t pl = rk_mk_arr(NULL, 0);
    rk_arr_append((ARBLK_t *) pl.arr, INTVAL(kind));
    rk_arr_append((ARBLK_t *) pl.arr, step);
    rk_arr_append((ARBLK_t *) pl.arr, INTVAL(dir));
    rk_arr_append((ARBLK_t *) pl.arr, INTVAL(infinite ? 2 : endnum ? 1 : 0));
    return pl;
}
DESCR_t rk_seq_next(int kind, DESCR_t step, DESCR_t last) {
    if (kind == 1) return rk_seq_num2(last, step, 0);
    if (kind == 2) return rk_seq_num2(last, step, 2);
    { const char *ls = rk_cstr(last); return STRVAL(rk_str_succ_buf(ls, (char *)rt_heap_alloc_c(strlen(ls) + 2))); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_hash_from_list(const DESCR_t *el, int n) {
    DESCR_t h = TABLE_VAL(table_new());
    for (int i = 0; i < n; ) {
        if (rk_is_pair(el[i])) {
            rk_av_t p = rk_av(el[i]);
            h = rk_hash_store(h, p.el[0], p.n > 1 ? p.el[1] : NULVCL);
            i++;
        } else {
            if (i + 1 >= n) {
                const char * lr = rk_raku_str(el[i], 0);
                char *hm = rt_heap_fmt("Odd number of elements found where hash initializer expected:\nFound %d (implicit) elements:\nLast element seen: %s", n, lr);
                rk_exc_init();
                rk_exc_throw(DATCON_fn("X::Hash::Store::OddNumber", STRVAL(hm)));
                return FAILDESCR;
            }
            h = rk_hash_store(h, el[i], el[i + 1]);
            i += 2;
        }
    }
    return h;
}
int rk_io_fail(const char *cls, const char *msg) { rk_io_init(); rk_exc_throw(DATCON_fn(cls, STRVAL(rt_heap_strdup_c(msg)))); return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_qh_refuse(const char *fmt, const char *a, const char *b) { char mm[strlen(fmt) + strlen(a) + strlen(b) + 8]; snprintf(mm, sizeof mm, fmt, a, b); rk_exc_throw(rk_exc_from_msg(mm)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const struct {
    const char *needle, *cls, *parent;
} rk_exc_tab[] = { { "Attempt to divide by zero", "X::Numeric::DivideByZero", "Exception" }, { "Cannot resolve caller", "X::Multi::NoMatch", "Exception" }, { "Type check failed in binding",
    "X::TypeCheck::Binding", "X::TypeCheck" }, { "Type check failed in assignment", "X::TypeCheck::Assignment", "X::TypeCheck" }, { "Cannot modify an immutable", "X::Assignment::RO", "Exception" },
    { "No such method", "X::Method::NotFound", "Exception" }, { "out of range", "X::OutOfRange", "Exception" }, { "Cannot convert string to number", "X::Str::Numeric", "Exception" },
    { "imaginary part not zero", "X::Numeric::Real", "Exception" }, { "Unrecognized trailing characters", "X::Str::Numeric", "Exception" }, { "numeric expected", "X::Str::Numeric", "Exception" },
    { "must be resolved by class", "X::Role::Composition", "Exception" }, { "No compiler available for language", "X::Eval::NoSuchLang", "Exception" }, { "Could not find Inline::",
    "X::CompUnit::UnsatisfiedDependency", "Exception" }, { NULL, NULL, NULL } };
static const struct {
    const char *needle, *cls, *parent;
} rk_comp_tab[] = { { "Unsupported use of", "X::Obsolete", "X::Comp" }, { "Quantifier quantifies nothing", "X::Syntax::Regex::SolitaryQuantifier", "X::Syntax" }, { "Null regex not allowed",
    "X::Syntax::Regex::NullRegex", "X::Syntax" }, { "Missing block", "X::Syntax::Missing", "X::Syntax" }, { "Missing initializer", "X::Syntax::Missing", "X::Syntax" }, { "Malformed",
    "X::Syntax::Malformed", "X::Syntax" }, { "Bogus statement", "X::Syntax::Malformed", "X::Syntax" }, { "Redeclaration of", "X::Redeclaration", "X::Comp" }, { "already has a method",
    "X::Redeclaration", "X::Comp" }, { "Cannot declare a match variable", "X::Syntax::Variable::Match", "X::Syntax" }, { "Cannot declare a numeric variable", "X::Syntax::Variable::Numeric",
    "X::Syntax" }, { "Name component may not be null", "X::Syntax::Name::Null", "X::Syntax" }, { "Cannot add tokens of category", "X::Syntax::Reserved", "X::Syntax" },
    { "Opening bracket required for #`", "X::Syntax::Comment::Embedded", "X::Syntax" }, { "please rewrite using \"if\"", "X::Syntax::UnlessElse", "X::Syntax" }, { "Non-variable $ must be backslashed",
    "X::Backslash::NonVariableDollar", "X::Syntax" }, { "A unit-scoped sub definition is not allowed except on", "X::UnitScope::Invalid", "X::Syntax" }, { NULL, NULL, NULL } };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_exc_init(void) {
    static int rk_exc_reg = 0;
    if (rk_exc_reg) return;
    rk_exc_reg = 1;
    extern void record_register(const char *spec);
    extern void class_inherit(const char *child, const char *parent);
    record_register("Exception");
    record_register("X::AdHoc(payload)");
    class_inherit("X::AdHoc", "Exception");
    record_register("X::Hash::Store::OddNumber(message)");
    class_inherit("X::Hash::Store::OddNumber", "Exception");
    record_register("X::Parameter::RW(message)");
    class_inherit("X::Parameter::RW", "Exception");
    record_register("X::TypeCheck(message)");
    class_inherit("X::TypeCheck", "Exception");
    record_register("X::Comp(message)");
    class_inherit("X::Comp", "Exception");
    record_register("X::Comp::AdHoc(message)");
    class_inherit("X::Comp::AdHoc", "X::Comp");
    record_register("X::Syntax(message)");
    class_inherit("X::Syntax", "X::Comp");
    record_register("X::Syntax::Confused(message)");
    class_inherit("X::Syntax::Confused", "X::Syntax");
    record_register("X::Undeclared(message)");
    class_inherit("X::Undeclared", "X::Comp");
    record_register("X::Undeclared::Symbols(message)");
    class_inherit("X::Undeclared::Symbols", "X::Comp");
    record_register("X::TypeCheck::Argument(message)");
    class_inherit("X::TypeCheck::Argument", "X::TypeCheck");
    record_register("X::Cannot::Empty(message)");
    class_inherit("X::Cannot::Empty", "Exception");
    record_register("X::Dynamic::NotFound(message,name)");
    class_inherit("X::Dynamic::NotFound", "Exception");
    record_register("X::Numeric::CannotConvert(message)");
    class_inherit("X::Numeric::CannotConvert", "Exception");
    record_register("X::TypeCheck::Assignment(message,got,expected,symbol)");
    class_inherit("X::TypeCheck::Assignment", "X::TypeCheck");
    record_register("X::Method::NotFound(message,method,typename,invocant)");
    class_inherit("X::Method::NotFound", "Exception");
    record_register("X::OutOfRange(message,what,got,range)");
    class_inherit("X::OutOfRange", "Exception");
    for (int i = 0; rk_comp_tab[i].cls; i++) {
        char spec[strlen(rk_comp_tab[i].cls) + 32];
        snprintf(spec, sizeof spec, "%s(message%s)", rk_comp_tab[i].cls, strcmp(rk_comp_tab[i].cls, "X::Syntax::UnlessElse") ? "" : ",keyword");
        record_register(spec);
        class_inherit(rk_comp_tab[i].cls, rk_comp_tab[i].parent);
    }
    for (int i = 0; rk_exc_tab[i].cls; i++) {
        if (!strcmp(rk_exc_tab[i].cls, "X::Method::NotFound") || !strcmp(rk_exc_tab[i].cls, "X::TypeCheck::Assignment") || !strcmp(rk_exc_tab[i].cls, "X::OutOfRange")) continue;
        char spec[strlen(rk_exc_tab[i].cls) + 16];
        snprintf(spec, sizeof spec, "%s(message)", rk_exc_tab[i].cls);
        record_register(spec);
        class_inherit(rk_exc_tab[i].cls, rk_exc_tab[i].parent);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_exc_is(DESCR_t d) { return rk_exc_isa(d, "Exception"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_exc_adhoc(DESCR_t payload) { rk_exc_init(); return DATCON_fn("X::AdHoc", payload); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_exc_message(DESCR_t ex) {
    const char *cn = ex.u->type->name;
    extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
    const char *rmc = resolve_method_chain(cn, "message", NULL);
    char proc[fmt_len("%s__%s", rmc, "message")];
    snprintf(proc, sizeof proc, "%s__%s", rmc, "message");
    if (meth_is_user_proc(proc)) {
        DESCR_t a[2] = { ex, STRVAL((char *) "message") }, r;
        if (script_try_call_builtin_by_name("meth_call", a, 2, &r) && !IS_FAIL_fn(r)) { const char *m = rk_cstr(r); return m ? m : ""; }
    }
    DATBLK_t *t = ex.u->type;
    for (int i = 0; i < t->nfields; i++) if (!strcmp(t->fields[i], "message") || !strcmp(t->fields[i], "payload")) { const char *m = rk_cstr(FIELD_GET_fn(ex, t->fields[i])); return m ? m : ""; }
    return "Died";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_exc_umsg(DESCR_t ex) {
    if (!rk_exc_is(ex)) return (const char *) 0;
    const char *rmc = resolve_method_chain(ex.u->type->name, "message", NULL);
    char proc[fmt_len("%s__%s", rmc, "message")];
    snprintf(proc, sizeof proc, "%s__%s", rmc, "message");
    return meth_is_user_proc(proc) ? rmc : (const char *) 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_unless_else(const char *msg) {
    const char *kq = strstr(msg, "does not take \""), *ks = kq ? kq + 15 : NULL, *ke = ks ? strchr(ks, '"') : NULL;
    size_t kl = ke ? (size_t) (ke - ks) : 0;
    char *kw = rt_wsb_alloc(kl + 1);
    if (kl) memcpy(kw, ks, kl);
    kw[kl] = 0;
    return DATCON_fn("X::Syntax::UnlessElse", STRVAL(rt_heap_strdup_c(msg)), STRVAL(kw));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_exc_from_msg(const char *msg) {
    rk_exc_init();
    if (strstr(msg, "does not take \"") && strstr(msg, "please rewrite using \"if\"")) return rk_unless_else(msg);
    for (int i = 0; rk_exc_tab[i].cls; i++) if (strstr(msg, rk_exc_tab[i].needle)) return DATCON_fn(rk_exc_tab[i].cls, STRVAL(rt_heap_strdup_c(msg)), NULVCL, NULVCL, NULVCL);
    return rk_exc_adhoc(STRVAL(rt_heap_strdup_c(msg)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_exc_current(void) {
    extern char g_script_exception[512];
    DESCR_t cur = NV_GET_fn("!");
    if (rk_exc_is(cur) && (rk_exc_umsg(cur) || !strcmp(rk_exc_message(cur), g_script_exception))) return cur;
    if (!g_script_exception[0]) return rk_exc_is(cur) ? cur : NULVCL;
    cur = rk_exc_from_msg(g_script_exception);
    NV_SET_fn("!", cur);
    return cur;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_exc_throw(DESCR_t ex) { extern void rt_script_die_surface(const char *msg); NV_SET_fn("!", ex); rt_script_die_surface(rk_exc_message(ex)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_failure_is(DESCR_t d) { return IS_DATA_INST_fn(d) && d.u && d.u->type && d.u->type->name && !strcmp(d.u->type->name, "Failure"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_failure_handle(DESCR_t d) { FIELD_SET_fn(d, "handled", INTVAL(1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_failure_sink(DESCR_t f) { if (FIELD_GET_fn(f, "handled").i) return; rk_failure_handle(f); rk_exc_throw(FIELD_GET_fn(f, "exception")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_failure_str(DESCR_t f) { return STRVAL(rt_heap_fmt("(HANDLED) %s\n", rk_exc_message(FIELD_GET_fn(f, "exception")))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_failure_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out) {
    DESCR_t f = args[0];
    (void) nargs;
    if ((!strcmp(m, "Str") || !strcmp(m, "gist")) && FIELD_GET_fn(f, "handled").i) { *out = rk_failure_str(f); return 1; }
    if (!strcmp(m, "defined") || !strcmp(m, "Bool") || !strcmp(m, "so")) { rk_failure_handle(f); *out = (DESCR_t){ .v = DT_BOOL, .i = 0 }; return 1; }
    if (!strcmp(m, "not")) { rk_failure_handle(f); *out = (DESCR_t){ .v = DT_BOOL, .i = 1 }; return 1; }
    if (!strcmp(m, "handled")) { *out = (DESCR_t){ .v = DT_BOOL, .i = FIELD_GET_fn(f, "handled").i ? 1 : 0 }; return 1; }
    if (!strcmp(m, "exception")) { rk_failure_handle(f); *out = FIELD_GET_fn(f, "exception"); return 1; }
    if (!strcmp(m, "sink")) { rk_failure_sink(f); *out = NULVCL; return 1; }
    if (!strcmp(m, "Str") || !strcmp(m, "Numeric") || !strcmp(m, "Int") || !strcmp(m, "Num") || !strcmp(m, "Rat") || !strcmp(m, "elems") || !strcmp(m, "chars") || !strcmp(m, "uc") || !strcmp(m, "lc"))
        {
        rk_failure_sink(f);
        *out = FAILDESCR;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_raku_undeclared_call(const char *name) {
    extern int g_script_try_depth;
    if (g_script_try_depth <= 0 || !name || !(isalpha((unsigned char) name[0]) || name[0] == '_') || !strncmp(name, "__", 2) || strstr(name, "::")) return 0;
    rk_exc_init();
    const char *cls = "X::Undeclared::Symbols";
    const char *proto = !strcmp(name, "push") || !strcmp(name, "unshift") ? "($, |)" : !strcmp(name, "shift") ? "($, *%)" : (const char *) 0;
    char *msg;
    if (!strcmp(name, "e") || !strcmp(name, "pi") || !strcmp(name, "tau")) {
        cls = "X::Undeclared";
        msg = rt_heap_fmt("Variable '&%s' is not declared. Perhaps you forgot a 'sub' if this was\nintended to be part of a signature?", name);
    } else if (proto) {
        cls = "X::TypeCheck::Argument";
        msg = rt_heap_fmt("Calling %s() will never work with signature of the proto %s", name, proto);
    } else msg = rt_heap_fmt("Undeclared routine:\n    %s used", name);
    rk_exc_throw(DATCON_fn(cls, STRVAL(msg)));
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_method_missing(DESCR_t recv, const char *mname, DESCR_t *out) {
    rk_exc_init();
    if (!IS_DATA_INST_fn(recv) && !rk_typeobj_name(recv) && !strcmp(rk_value_type(recv), "Nil")) { *out = recv; return; }
    const char *tn = NULL;
    if (IS_DATA_INST_fn(recv) && recv.u && recv.u->type) tn = recv.u->type->name;
    else if (rk_typeobj_name(recv)) tn = rk_typeobj_name(recv);
    else if (IS_STR_fn(recv) && recv.s && dat_find_type(recv.s)) tn = recv.s;
    if (!tn) tn = rk_value_type(recv);
    char *msg = rt_heap_fmt("No such method '%s' for invocant of type '%s'", mname, tn);
    rk_exc_throw(DATCON_fn("X::Method::NotFound", STRVAL(msg), STRVAL(rt_heap_strdup_c(mname)), STRVAL(rt_heap_strdup_c(tn)), recv));
    *out = FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_exc_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    DESCR_t ex = args[0];
    (void) nargs;
    if (!strcmp(m, "message") || !strcmp(m, "Str") || !strcmp(m, "gist")) {
        const char *rmc = rk_exc_umsg(ex);
        if (rmc) { char proc[fmt_len("%s__%s", rmc, "message")]; snprintf(proc, sizeof proc, "%s__%s", rmc, "message"); *out = invoke_method_proc_rq(proc, &ex, 1, rq); return 1; }
        *out = STRVAL(rt_heap_strdup_c(rk_exc_message(ex)));
        return 1;
    }
    if (!strcmp(m, "throw") || !strcmp(m, "rethrow")) { rk_exc_throw(ex); *out = FAILDESCR; return 1; }
    if (!strcmp(m, "Bool") || !strcmp(m, "so") || !strcmp(m, "defined")) { *out = (DESCR_t){ .v = DT_BOOL, .i = 1 }; return 1; }
    if (!strcmp(m, "backtrace")) { *out = STRVAL(rt_heap_strdup_c("")); return 1; }
    return 0;
}
DESCR_t c_rt_rk_arr_at(DESCR_t *args, int nargs) { DESCR_t out = NULVCL; (void) nargs; rk_arr_at_impl(args, &out); return out; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_param_ok(DESCR_t *args) { const char *ptype = VARVAL_fn(args[0]); return rt_mc_accepts(ptype ? ptype : "Any", args[1]); }
DESCR_t rt_rk_param_check(DESCR_t *args, int nargs) { return nargs >= 2 && rk_param_ok(args) ? args[1] : rt_call_arr_bl("__param_check", args, nargs, -1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_assign_ok(DESCR_t *args) { return args[1].v == DT_SNUL || args[1].v == DT_FAIL || rk_type_isa(rk_value_type(args[1]), rk_cstr(args[0])); }
DESCR_t rt_rk_assign_check(DESCR_t *args, int nargs) { return nargs == 4 && rk_assign_ok(args) ? args[1] : rt_call_arr_bl("__rk_assign_check", args, nargs, -1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_bindattr_impl(DESCR_t *args) {
    extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
    const char *an = VARVAL_fn(args[2]);
    if (!an) an = "";
    if (an[0] && (an[1] == '!' || an[1] == '.')) an += 2;
    DESCR_t *cell = data_field_ptr(an, args[0]);
    if (!cell) return FAILDESCR;
    *cell = args[3];
    return args[3];
}
DESCR_t rt_rk_bindattr(DESCR_t *args, int nargs) { (void) nargs; return rk_bindattr_impl(args); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_box_impl(DESCR_t *args) {
    extern ARBLK_t *array_new(int lo, int hi);
    ARBLK_t *bx = array_new(0, 0);
    bx->data[0] = args[0];
    return (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *) &bx->data[0] };
}
DESCR_t rt_rk_box(DESCR_t *args, int nargs) { (void) nargs; return rk_box_impl(args); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_blk_ref_impl(DESCR_t *args) { const char *bn = VARVAL_fn(args[0]); DESCR_t b = {0}; b.v = DT_BLK; b.slen = 0; b.s = (char *)(bn ? bn : ""); return b; }
DESCR_t rt_rk_blk_ref(DESCR_t *args, int nargs) { (void) nargs; return rk_blk_ref_impl(args); }
DESCR_t rt_rk_undef(DESCR_t *args, int nargs) { (void) args; (void) nargs; return NULVCL; }
DESCR_t rt_rk_range_arr(DESCR_t *args, int nargs) { return rk_range_arr_make_fl(args[0], args[1], nargs == 3 ? (int) args[2].i : 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_iter_src_impl(DESCR_t *args, int nargs) {
    extern ARBLK_t *array_new(int lo, int hi);
    if (nargs && args[0].v == DT_A && args[0].arr) return args[0];
    if (nargs && args[0].v == DT_T && args[0].tbl) return rk_hash_list(args[0], 'p');
    rk_av_t a = { 0, NULL };
    if (nargs) a = rk_av(args[0]);
    ARBLK_t *b = array_new(0, a.n - 1);
    for (int i = 0; i < a.n; i++) b->data[i] = a.el[i];
    DESCR_t d = {0};
    d.v = DT_A;
    d.slen = 0;
    d.arr = b;
    return d;
}
DESCR_t rt_rk_iter_src(DESCR_t *args, int nargs) { return rk_iter_src_impl(args, nargs); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_append_ok(DESCR_t *args, int nargs) { return nargs == 2 && args[0].v == DT_A && args[0].arr; }
static DESCR_t rk_append_impl(DESCR_t *args, int is_map) {
    ARBLK_t *b = (ARBLK_t *) args[0].arr;
    if (is_map && rk_is_slip(args[1])) { rk_av_t v = rk_av(args[1]); for (int i = 0; i < v.n; i++) rk_arr_append(b, v.el[i]); } else rk_arr_append(b, args[1]);
    return args[0];
}
DESCR_t rt_rk_map_append(DESCR_t *args, int nargs) { return rk_append_ok(args, nargs) ? rk_append_impl(args, 1) : rt_call_arr_bl("__rk_map_append", args, nargs, -1); }
DESCR_t rt_rk_grep_append(DESCR_t *args, int nargs) { return rk_append_ok(args, nargs) ? rk_append_impl(args, 0) : rt_call_arr_bl("__rk_grep_append", args, nargs, -1); }
DESCR_t bt___rk_take_dyn(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        extern int rt_genp_active(void);
        extern void rt_genp_deliver_γ(DESCR_t);
        if (!rt_genp_active()) { rk_exc_init(); rk_exc_throw(rk_exc_adhoc(STRVAL(rt_heap_strdup_c("take without gather")))); return NULVCL; }
        rt_genp_deliver_γ(args[0]);
        return args[0];
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_rw_refuse(DESCR_t *args, int nargs) {
    if (nargs == 3) {
        extern const char * rt_proc_pname(const char *, int);
        char nb[256], vb[128];
        const char * pn = rt_proc_pname(to_cstring(args[0], nb, sizeof nb), (int) args[1].i);
        const char * vs = to_cstring(args[2], vb, sizeof vb);
        const char * tn = rk_value_type(args[2]);
        const char * mf = "Parameter '$%s' expects a writable container (variable) as an argument,\nbut got '%s' (%s) as a value without a container.";
        const char * pnn = pn ? pn : "", * vsn = vs ? vs : "", * tnn = tn ? tn : "Any";
        size_t mn = (size_t)snprintf((char *)0, 0, mf, pnn, vsn, tnn);
        char mm[mn + 1];
        snprintf(mm, mn + 1, mf, pnn, vsn, tnn);
        rk_exc_init();
        rk_exc_throw(DATCON_fn("X::Parameter::RW", STRVAL(rt_heap_strdup_c(mm))));
        return FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_sink(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 1) { *out = args[0]; if (rk_failure_is(args[0]) && !FIELD_GET_fn(args[0], "handled").i) { rk_failure_sink(args[0]); *out = rk_typeobj("Nil"); } return _bo; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_exc_umsg(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_exc_umsg(args[0]) ? INTVAL(1) : FAILDESCR; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_die_m(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        rk_exc_init();
        DESCR_t ex = rk_exc_is(args[0]) ? args[0] : rk_exc_adhoc(args[0]);
        if (rk_exc_umsg(ex)) { extern void rt_script_die_surface(const char *msg); const char *m = rk_cstr(args[1]); NV_SET_fn("!", ex); rt_script_die_surface(m ? m : ""); } else rk_exc_throw(ex);
        return FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_die(DESCR_t *args, int nargs) {
    if (nargs >= 1) { rk_exc_init(); rk_exc_throw(rk_exc_is(args[0]) ? args[0] : rk_exc_adhoc(args[0])); return FAILDESCR; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
int bt___rk_eval(DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (nargs >= 1) {
        const char *lg = nargs >= 2 ? rk_cstr(args[1]) : "Raku";
        if (strcmp(lg, "Raku") && strcmp(lg, "perl6")) {
            char msg[strlen(lg) + 48];
            if (!strcmp(lg, "Perl5")) snprintf(msg, sizeof msg, "Could not find Inline::Perl5");
            else snprintf(msg, sizeof msg, "No compiler available for language '%s'", lg);
            rk_exc_throw(rk_exc_from_msg(msg));
            *out = FAILDESCR;
            return 1;
        }
        extern const char *rt_raku_eval_compile(const char *src, const char **errmsg);
        const char *s0 = rk_cstr(args[0]);
        char src[strlen(s0) + 1];
        strcpy(src, s0);
        const char *err = NULL;
        const char *pn = rt_raku_eval_compile(src, &err);
        if (!pn) {
            rk_exc_init();
            const char *e0 = err ? err : "syntax error";
            const char *cls = strstr(e0, "Confused") ? "X::Syntax::Confused" : strstr(e0, "not declared") ? "X::Undeclared" : "X::Comp::AdHoc";
            for (int ci = 0; rk_comp_tab[ci].cls; ci++) if (strstr(e0, rk_comp_tab[ci].needle)) { cls = rk_comp_tab[ci].cls; break; }
            rk_exc_throw(!strcmp(cls, "X::Syntax::UnlessElse") ? rk_unless_else(e0) : DATCON_fn(cls, STRVAL(rt_heap_strdup_c(e0))));
            *out = FAILDESCR;
            return 1;
        }
        extern int rt_proc_has_native_fn(const char *name);
        extern DESCR_t ir_call_proc(int pi, DESCR_t *args, int nargs);
        int pi;
        for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, pn)) break;
        DESCR_t none[1] = { NULVCL };
        if (pi < g_stage2.proc_count && !rt_proc_has_native_fn(pn)) { *out = ir_call_proc(pi, none, 0); return 1; }
        rt_call_args_need(0);
        { extern int rt_call_open_tail_lex(const char *name, int nargs, long *rq); if (rq && rt_call_open_tail_lex(pn, 0, rq)) { *out = FAILDESCR; return 1; } }
        {
            extern DESCR_t rt_c2bb_bomb(const char *site, const char *name);
            *out = rt_c2bb_bomb("__rk_eval (the compiled EVAL body opens through the request channel; a by-name call without one)", pn);
        }
        return 1;
    }
    return 0;
}
DESCR_t bt___rk_rethrow(DESCR_t *args, int nargs) {
    if (nargs == 0) { DESCR_t ex = rk_exc_current(); if (rk_exc_is(ex)) rk_exc_throw(ex); return FAILDESCR; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_srand(DESCR_t *args, int nargs) {
    if (nargs == 1) { long seed = IS_INT_fn(args[0]) ? (long)args[0].i : (IS_REAL_fn(args[0]) ? (long)args[0].r : 0); srand((unsigned int)seed); return INTVAL(seed); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_exc_clear(DESCR_t *args, int nargs) {
    if (nargs == 0) { extern char g_script_exception[512]; (void) rk_exc_current(); g_script_exception[0] = '\0'; return STRVAL(rt_heap_strdup_c("")); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_try_enter(DESCR_t *args, int nargs) {
    if (nargs == 0) {
        extern char g_script_exception[512];
        extern int g_script_try_depth;
        g_script_try_depth++;
        g_script_exception[0] = '\0';
        NV_SET_fn("!", NULVCL);
        return STRVAL(rt_heap_strdup_c(""));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_try_exit(DESCR_t *args, int nargs) {
    if (nargs == 0) { extern int g_script_try_depth; if (g_script_try_depth > 0) g_script_try_depth--; return STRVAL(rt_heap_strdup_c("")); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_exc_check(DESCR_t *args, int nargs) {
    if (nargs == 0) { extern char g_script_exception[512]; if (g_script_exception[0] != '\0') { return INTVAL(1); } return FAILDESCR; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_exc_get(DESCR_t *args, int nargs) { if (nargs == 0) { DESCR_t ex = rk_exc_current(); return rk_exc_is(ex) ? ex : STRVAL(rt_heap_strdup_c("")); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_fh_capture(DESCR_t *args, int nargs) {
    if (nargs == 1) { extern void fh_ensure_init(void); fh_ensure_init(); int idx = (int)(IS_INT_fn(args[0]) ? args[0].i : 0); return FHVAL(idx); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_slurp(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        extern void fh_ensure_init(void);
        extern FILE *fh_get(int);
        FILE *fp = NULL;
        int need_close = 0;
        if (IS_INT_fn(args[0]) || IS_FH_fn(args[0])) {
            fh_ensure_init();
            fp = fh_get((int)args[0].i);
        } else {
            const char *path = VARVAL_fn(args[0]);
            if (!path || !*path) { return STRVAL(rt_heap_strdup_c("")); }
            fp = fopen(path, "r");
            need_close = 1;
        }
        if (!fp) { return STRVAL(rt_heap_strdup_c("")); }
        fseek(fp, 0, SEEK_END);
        long sz = ftell(fp);
        rewind(fp);
        char *buf = rt_wsb_alloc(sz + 1);
        size_t nr = fread(buf, 1, (size_t)sz, fp);
        buf[nr] = '\0';
        if (need_close) fclose(fp);
        return STRVAL(buf);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_box(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_box_impl(args); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___blk_ref(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_blk_ref_impl(args); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___param_check(DESCR_t *args, int nargs) {
    if (nargs >= 2) {
        const char *ptype = VARVAL_fn(args[0]);
        if (!ptype) ptype = "Any";
        const char *pname = (nargs >= 3) ? VARVAL_fn(args[2]) : "?";
        if (!pname) pname = "?";
        if (!rk_param_ok(args)) {
            extern void rt_script_die_surface(const char *msg);
            char m[fmt_len("Type check failed in binding to parameter '$%s'; expected %s", pname, ptype)];
            snprintf(m, sizeof m, "Type check failed in binding to parameter '$%s'; expected %s", pname, ptype);
            rt_script_die_surface(m);
            return FAILDESCR;
        }
        return args[1];
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_range_arr(DESCR_t *args, int nargs) {
    if ((nargs == 2 || nargs == 3)) { return rk_range_arr_make_fl(args[0], args[1], nargs == 3 ? (int) args[2].i : 0); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_assign_check(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 4) {
        const char *ty = rk_cstr(args[0]), *nm = rk_cstr(args[2]);
        DESCR_t v = args[1];
        *out = v;
        if (rk_assign_ok(args)) return _bo;
        const char *vt = rk_value_type(v);
        rk_exc_init();
        const char *got = rk_raku_str(v, 0);
        int el = IS_INT_fn(args[3]) && args[3].i;
        char *msg = el ? rt_heap_fmt("Type check failed for an element of %s; expected %s but got %s (%s)", nm, ty, vt, got) :
            rt_heap_fmt("Type check failed in assignment to $%s; expected %s but got %s (%s)", nm, ty, vt, got);
        rk_exc_throw(DATCON_fn("X::TypeCheck::Assignment", STRVAL(msg), STRVAL(rt_heap_strdup_c(vt)), STRVAL(rt_heap_strdup_c(ty)), STRVAL(rt_heap_strdup_c(nm))));
        return FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_elem_put(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 4) { rk_elem_put_impl(args, out); return _bo; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_iter_src(DESCR_t *args, int nargs) { if (nargs <= 1) { return rk_iter_src_impl(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_map_append(DESCR_t *args, int nargs, int _bop) { if (rk_append_ok(args, nargs)) { return rk_append_impl(args, (_bop == 0)); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_grep_append(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_map_append(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_map_append(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_map_append(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t rt_call_arr_bl(const char *fn, DESCR_t *args, int nargs, int bidlen) { return RT_GC_CALLBACK(rt_call_arr_bl_s(fn, args, nargs, bidlen, 0, 0, (long *)0)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int dtax_off(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_DTAX_OFF"); p = (e && *e) ? 1 : 0; } return p; }
int icn_dtax_plain(int bid) { return !dtax_off() && g_rt_bn.dtax_bid[bid].gen == rt_dtax_gen && (g_rt_bn.dtax_bid[bid].kind == 0 || g_rt_bn.dtax_bid[bid].kind == 3); }
