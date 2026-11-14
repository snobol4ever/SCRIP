#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pas_view_raw.h"
#include "runtime/lib/bn_head_pl_cx_leaf_head.h"
#include "runtime/lib/bn_head_plf_float.h"
#include "runtime/lib/bn_head_pl_flag.h"
#include "runtime/lib/bn_head_pas_set_bytes.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/prolog_atom.h"
#include "runtime/rt/prolog_atom_prolog_max_arity.h"
#include <errno.h>
#include <ctype.h>
#include <float.h>
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_10_pas.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/core/g_runtime_pas.h"
#include "runtime/core/core_rt_asm_helpers_api.h"
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/rt/gc_heap_hb_aggb.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ct_arena.h"
extern void *rt_ws_alloc_descr(size_t);
extern DESCR_t rt_deref(DESCR_t d);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t elem_to_descr(const char *s, size_t slen) {
    char stk[64];
    char *buf = (slen + 1 <= sizeof stk) ? stk : (char *)ct_alloc(slen + 1);
    if (!buf) return INTVAL(0);
    memcpy(buf, s, slen);
    buf[slen] = '\0';
    char *ep;
    long iv = strtol(buf, &ep, 10);
    if (*ep == '\0' && ep > buf) { if (buf != stk) ct_drop(buf); return INTVAL(iv); }
    if (slen > 1 && strpbrk(buf, ".eE") && (buf[0] == '-' || buf[0] == '+' || buf[0] == '.' || (buf[0] >= '0' && buf[0] <= '9'))) {
        char *ep2;
        double dv = strtod(buf, &ep2);
        if (*ep2 == '\0' && ep2 > buf) { if (buf != stk) ct_drop(buf); return REALVAL(dv); }
    }
    { char *o = rt_str_alloc((long)slen); memcpy(o, s, slen); o[slen] = '\0'; if (buf != stk) ct_drop(buf); return STRVAL(o); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int jct_one_cmp_num(double a, double b, int op) {
    switch (op) {
        case TT_EQ:
        return a == b;
        case TT_NE:
        return a != b;
        case TT_LT:
        return a < b;
        case TT_LE:
        return a <= b;
        case TT_GT:
        return a > b;
        case TT_GE:
        return a >= b;
        default:
        return a == b;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int jct_one_cmp_str(const char *a, const char *b, int op) {
    int c = strcmp(a, b);
    switch (op) {
        case TT_EQ:
        return c == 0;
        case TT_NE:
        return c != 0;
        case TT_LT:
        return c < 0;
        case TT_LE:
        return c <= 0;
        case TT_GT:
        return c > 0;
        case TT_GE:
        return c >= 0;
        default:
        return c == 0;
    }
}
void pas_tf_set(int idx, DESCR_t buf, int has, int mode) {
    uint64_t need = (uint64_t)(2 * (idx + 1));
    gv_reserve(&g_rt_pas.pas_tf, (uint16_t)HB_DVEC, (uint32_t)sizeof(DESCR_t), need, "g_rt_pas.pas_tf");
    while (g_rt_pas.pas_tf.len < need) { ((DESCR_t *)g_rt_pas.pas_tf.p)[g_rt_pas.pas_tf.len] = INTVAL(0); g_rt_pas.pas_tf.len++; }
    PTF_BUF(idx) = buf;
    PTF_ST(idx) = INTVAL((long)(has & 1) | ((long)mode << 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *pas_heap_cell(long n, int grow) {
    if (n <= 0) return (DESCR_t *)0;
    if (n >= g_rt_pas.pas_heap_cap) {
        if (!grow) return (DESCR_t *)0;
        long nc = g_rt_pas.pas_heap_cap ? g_rt_pas.pas_heap_cap : 1024;
        while (nc <= n) nc *= 2;
        DESCR_t *nh = (DESCR_t *)ct_grow((void *)g_rt_pas.pas_heap, (size_t)nc * sizeof(DESCR_t));
        unsigned char *nd = (unsigned char *)ct_grow((void *)g_rt_pas.pas_heap_dead, (size_t)nc);
        if (!nh || !nd) { fprintf(stderr, "[PAS] heap table: out of memory growing to %ld entries\n", nc); abort(); }
        memset(nh + g_rt_pas.pas_heap_cap, 0, (size_t)(nc - g_rt_pas.pas_heap_cap) * sizeof(DESCR_t));
        memset(nd + g_rt_pas.pas_heap_cap, 0, (size_t)(nc - g_rt_pas.pas_heap_cap));
        g_rt_pas.pas_heap = nh;
        g_rt_pas.pas_heap_dead = nd;
        g_rt_pas.pas_heap_cap = nc;
    }
    return &g_rt_pas.pas_heap[n];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pas_numeral_put(char *b, size_t *n, size_t *cap, int c) {
    if (*n + 2 > *cap) { size_t nc = *cap ? *cap * 2 : 64; char *g = rt_str_alloc((long)nc); if (b) memcpy(g, b, *n); b = g; *cap = nc; }
    b[(*n)++] = (char)c;
    b[*n] = '\0';
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_read_number(FILE *f, int real, int fpc, const char *fn) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    char *b = NULL;
    size_t n = 0, cap = 0;
    int c = fgetc(f), nd = 0, bad = 0;
    while (c != EOF && isspace(c)) c = fgetc(f);
    if (c == EOF && fpc) return real ? REALVAL(0.0) : INTVAL(0);
    if (c == EOF) pas_file_err("6.9.1", "end-of-file is true when a number is to be read", fn);
    if (c == '+' || c == '-') { b = pas_numeral_put(b, &n, &cap, c); c = fgetc(f); }
    if (!real) {
        int base = 0;
        if (c == '$' || c == 'x' || c == 'X') base = 16;
        else if (c == '%') base = 2;
        else if (c == '&') base = 8;
        else if (c == '0') { int c2 = fgetc(f); if (c2 == 'x' || c2 == 'X') base = 16; else if (c2 != EOF) ungetc(c2, f); }
        if (base) {
            unsigned long long v = 0;
            int k = 0, ovf = 0;
            c = fgetc(f);
            for (;; c = fgetc(f)) {
                int d = (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'f') ? c - 'a' + 10 : (c >= 'A' && c <= 'F') ? c - 'A' + 10 : 99;
                if (d >= base) break;
                if (v > (~0ULL - (unsigned long long)d) / (unsigned long long)base) ovf = 1;
                else v = v * (unsigned long long)base + (unsigned long long)d;
                k++;
            }
            if (c != EOF) ungetc(c, f);
            if (!k) pas_file_err("6.9.1", "the characters read do not form a signed-integer", fn);
            if (ovf || v > 0x7FFFFFFFFFFFFFFFULL) pas_file_err("6.9.1", "the signed-integer read is outside the range of the integer type", fn);
            return INTVAL((b && b[0] == '-') ? -(long long)v : (long long)v);
        }
    }
    while (c != EOF && isdigit(c)) { b = pas_numeral_put(b, &n, &cap, c); nd++; c = fgetc(f); }
    bad = !nd;
    if (real && c == '.') {
        int k = 0;
        b = pas_numeral_put(b, &n, &cap, '.');
        c = fgetc(f);
        while (c != EOF && isdigit(c)) { b = pas_numeral_put(b, &n, &cap, c); k++; c = fgetc(f); }
        bad = !nd && !k;
    }
    if (real && !bad && (c == 'e' || c == 'E')) {
        int k = 0;
        b = pas_numeral_put(b, &n, &cap, 'e');
        c = fgetc(f);
        if (c == '+' || c == '-') { b = pas_numeral_put(b, &n, &cap, c); c = fgetc(f); }
        while (c != EOF && isdigit(c)) { b = pas_numeral_put(b, &n, &cap, c); k++; c = fgetc(f); }
        bad = !k;
    }
    if (c != EOF) ungetc(c, f);
    if (bad) pas_file_err("6.9.1", real ? "the characters read do not form a signed-number" : "the characters read do not form a signed-integer", fn);
    DESCR_t r = {0};
    errno = 0;
    if (real) { r.v = DT_R; r.r = strtod(b, NULL); } else r = INTVAL(strtoll(b, NULL, 10));
    if (!real && errno == ERANGE) pas_file_err("6.9.1", "the signed-integer read is outside the range of the integer type", fn);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_tag_ne(long long a, long long b) { if (a >= -1 && a <= 1 && b >= -1 && b <= 1) return (a != 0) != (b != 0); return a != b; }
long pas_heap_take(void) {
    if (g_rt_pas.pas_heap_nfree > 0) { long n = g_rt_pas.pas_heap_free[--g_rt_pas.pas_heap_nfree]; if (n < g_rt_pas.pas_heap_cap) g_rt_pas.pas_heap_dead[n] = 0; return n; }
    return ++g_rt_pas.pas_heap_ctr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pas_heap_give(long n) {
    if (n <= 0 || n > g_rt_pas.pas_heap_ctr || n >= g_rt_pas.pas_heap_cap || (g_rt_pas.pas_heap_dead[n] & 1)) return;
    g_rt_pas.pas_heap[n] = INTVAL(0);
    g_rt_pas.pas_heap_dead[n] = 1;
    if (g_rt_pas.pas_heap_nfree == g_rt_pas.pas_heap_freecap) {
        long nc = g_rt_pas.pas_heap_freecap ? g_rt_pas.pas_heap_freecap * 2 : 256;
        long *nf = (long *)ct_grow((void *)g_rt_pas.pas_heap_free, (size_t)nc * sizeof(long));
        if (!nf) return;
        g_rt_pas.pas_heap_free = nf;
        g_rt_pas.pas_heap_freecap = nc;
    }
    g_rt_pas.pas_heap_free[g_rt_pas.pas_heap_nfree++] = n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_list_to_arr(DESCR_t lst, DESCR_t *out, int max) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    int n = 0;
    DESCR_t cur = rt_pl_deref_val(lst);
    DESCR_t *slow = (DESCR_t *)0;
    while (cur.v == (DTYPE_t)DT_PLREF && plc_fid_arity(cur.slen) == 2) {
        DESCR_t *kids = (DESCR_t *)cur.p;
        if (n >= max || kids == slow) return -1;
        if (!(n & (n + 1))) slow = kids;
        if (out) out[n] = rt_pl_deref_val(kids[0]);
        n++;
        cur = rt_pl_deref_val(kids[1]);
    }
    return pl_is_nil(cur) ? n : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pl_list_to_cstr(DESCR_t arg, int codes) {
    int ln = pl_list_to_arr(arg, (DESCR_t *)0, INT_MAX);
    DESCR_t elems[ln > 0 ? ln : 1];
    int n = ln < 0 ? ln : pl_list_to_arr(arg, elems, ln);
    if (n < 0) return (char *)0;
    char *o = (char *)ct_alloc((size_t)n + 1);
    if (!o) return (char *)0;
    for (int i = 0; i < n; i++) {
        if (codes) {
            if (elems[i].v != DT_I) { ct_drop(o); return (char *)0; }
            o[i] = (char)elems[i].i;
        } else {
            const char *cs = pl_atom_str(elems[i]);
            if (!cs || !cs[0]) { ct_drop(o); return (char *)0; }
            o[i] = cs[0];
        }
    }
    o[n] = 0;
    return o;
}
static void pl_iso_uncaught(const char *fmt, const char *a, const char *b, const char *c, int n) {
    fprintf(stderr, "Warning: goal raised exception: error(");
    fprintf(stderr, fmt, a ? a : "?", b ? b : "?", c ? c : "?", n);
    fprintf(stderr, ")\n");
    fflush(stderr);
    exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_instantiation(void) { pl_iso_uncaught("instantiation_error%.0s%.0s%.0s%.0d", "", "", "", 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_pi(const char *errfn, const char *what, const char *nm, int ar) { pl_iso_uncaught("%s(%s, %s/%d)", errfn, what, nm, ar); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_existence_key(const char *key) {
    extern int rt_pl_unknown_suppress(const char *);
    char nm[(key ? strlen(key) : 1) + 1];
    int ar = 0;
    if (rt_pl_unknown_suppress(key)) return;
    const char *sl = key ? strrchr(key, '/') : (const char *)0;
    if (sl) { int kl = (int)(sl - key); memcpy(nm, key, (size_t)kl); nm[kl] = 0; ar = atoi(sl + 1); } else { snprintf(nm, sizeof nm, "%s", key ? key : "?"); ar = 0; }
    rt_pl_iso_throw_pi("existence_error", "procedure", nm, ar);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_permission(const char *op, const char *type, const char *nm, int ar) {
    pl_iso_uncaught("permission_error(%s, %s, %s/%d)", op ? op : "access", type ? type : "private_procedure", nm, ar);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_permission_t(const char *op, const char *type, DESCR_t culprit) {
    (void)culprit;
    pl_iso_uncaught("permission_error(%s, %s, %s%.0d)", op ? op : "access", type ? type : "stream", "_", 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_type(const char *type, DESCR_t culprit) { (void)culprit; pl_iso_uncaught("type_error(%s, %s%.0s%.0d)", type, "_", "", 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_domain(const char *domain, DESCR_t culprit) { (void)culprit; pl_iso_uncaught("domain_error(%s, %s%.0s%.0d)", domain, "_", "", 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_existence(const char *type, DESCR_t culprit) { (void)culprit; pl_iso_uncaught("existence_error(%s, %s%.0s%.0d)", type, "_", "", 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_unknown_suppress(const char *key) {
    pl_flag_t *fl = pl_flag_find("unknown");
    if (!fl || !strcmp(fl->val, "error")) return 0;
    if (!strcmp(fl->val, "warning")) { fprintf(stderr, "Warning: unknown procedure %s\n", key ? key : "?"); fflush(stderr); }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PL_CX_LEAF_HEAD(current_prolog_flag, 2)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        char fb[128];
        const char *fn;
        DESCR_t f = rt_pl_deref_val(args[0]);
        pl_flag_t *fl;
        const char *dv;
        ok = 0;
        if (pl_val_unbound(f)) {
            cx->ball = rt_pl_ball_instantiation();
        } else if ((f.v != (DTYPE_t)DT_S && f.v != (DTYPE_t)DT_PLATOM) || !pl_cell_text(args[0], fb, sizeof fb, &fn)) {
            cx->ball = rt_pl_ball_kind2("type_error", "atom", f);
        } else if ((dv = pl_dia_flag(fn))) {
            ok = plw_unify_vals(args[1], (dv[0] == 't' || dv[0] == 'f') ? pl_mk_atom(dv) : INTVAL((int64_t)strtoll(dv, (char **)0, 10)), cx);
        } else if (!(fl = pl_flag_find(fn))) {
            if (!pl_swi_dialect()) cx->ball = rt_pl_ball_kind2("domain_error", "prolog_flag", f);
            else if (pl_restraint_kind(fn) >= 2) ok = plw_unify_vals(args[1], pl_mk_atom("error"), cx);
        } else if (fl->type == PLF_GONE) {
            ok = 0;
        } else if (fl->is_term) {
            DESCR_t o = pl_flag_term_value(fl);
            ok = plw_unify_vals(args[1], o, cx);
        } else if (!strcmp(fl->nm, "argv") || !strcmp(fl->nm, "os_argv")) {
            extern int rt_main_args_count(void);
            extern const char *rt_main_arg_at(int);
            extern const char *rt_main_progname(void);
            DESCR_t l = pl_nil();
            for (int i = rt_main_args_count() - 1; i >= (fl->nm[0] == 'o' ? -1 : 0); i--) {
                const char *a = i < 0 ? rt_main_progname() : rt_main_arg_at(i);
                if (!a) a = "";
                l = pl_cons(pl_mk_atom_dup(a, strlen(a)), l);
            }
            ok = plw_unify_vals(args[1], l, cx);
        } else if (!strcmp(fl->nm, "pid")) {
            ok = plw_unify_vals(args[1], INTVAL((int64_t)getpid()), cx);
        } else if (!strcmp(fl->nm, "stack_limit")) {
            ok = plw_unify_vals(args[1], INTVAL((int64_t)strtoll(fl->val ? fl->val : "0", (char **)0, 10)), cx);
        } else if (!strcmp(fl->nm, "max_arity")) {
            ok = plw_unify_vals(args[1], pl_swi_dialect() ? pl_mk_atom("unbounded") : INTVAL(PROLOG_MAX_ARITY), cx);
        } else if (!strcmp(fl->nm, "max_procedure_arity")) {
            ok = plw_unify_vals(args[1], INTVAL(PROLOG_MAX_PROCEDURE_ARITY), cx);
        } else if (!strcmp(fl->nm, "max_char_code")) {
            ok = plw_unify_vals(args[1], INTVAL(0x10FFFF), cx);
        } else if (!strcmp(fl->nm, "max_tagged_integer") || !strcmp(fl->nm, "min_tagged_integer")) {
            ok = plw_unify_vals(args[1], INTVAL(fl->nm[1] == 'a' ? 72057594037927935LL : -72057594037927936LL), cx);
        } else if (!strcmp(fl->nm, "float_max") || !strcmp(fl->nm, "float_min") || !strcmp(fl->nm, "float_max_integer")) {
            ok = plw_unify_vals(args[1], REALVAL(!strcmp(fl->nm, "float_max") ? DBL_MAX : !strcmp(fl->nm, "float_min") ? DBL_MIN : 9007199254740992.0), cx);
        } else if (!strcmp(fl->nm, "version_data")) {
            extern int prolog_atom_intern(const char *);
            DESCR_t *kk = (DESCR_t *)rt_ws_alloc_descr(4), c;
            kk[0] = INTVAL(0);
            kk[1] = INTVAL(0);
            kk[2] = INTVAL(0);
            kk[3] = pl_nil();
            c.v = (DTYPE_t)DT_PLREF;
            if (pl_swi_dialect()) { kk[0] = INTVAL(9); kk[2] = INTVAL(4); }
            c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern(pl_swi_dialect() ? "swi" : "scrip"), 4);
            c.p = (void *)kk;
            ok = plw_unify_vals(args[1], c, cx);
        } else if (!strcmp(fl->nm, "dialect") && pl_swi_dialect()) {
            ok = plw_unify_vals(args[1], pl_mk_atom("swi"), cx);
        } else if (!strcmp(fl->nm, "debug") && pl_swi_dialect()) {
            ok = plw_unify_vals(args[1], pl_mk_atom(strcmp(fl->val, "on") ? "false" : "true"), cx);
        } else if (fl->type == PLF_INT || fl->type == PLF_FLOAT) {
            ok = plw_unify_vals(args[1], fl->type == PLF_INT ? INTVAL((int64_t)strtoll(fl->val, (char **)0, 10)) : REALVAL(strtod(fl->val, (char **)0)), cx);
        } else {
            ok = plw_unify_vals(args[1], pl_mk_atom_dup(fl->val, strlen(fl->val)), cx);
        }
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char pas_view_key[] = "__pas_view";
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_rdecode(DESCR_t reg, long off, long kind, long rsz) {
    int cls = (int)(kind & 0xFF), esz = (int)((kind >> 8) & 0xFF);
    size_t tsz = PAS_SET_BYTES;
    unsigned char tmp[tsz];
    memset(tmp, 0, tsz);
    if (esz > (int)tsz) esz = (int)tsz;
    if (IS_STR_fn(reg) && reg.s && off >= 0) for (int k = 0; k < esz && off + k < rsz && off + k < (long)reg.slen; k++) tmp[k] = (unsigned char)reg.s[off + k];
    if (cls == 1 || cls == 2) {
        uint64_t v = 0;
        for (int k = 0; k < esz && k < 8; k++) v |= (uint64_t)tmp[k] << (8 * k);
        if (cls == 1 && esz < 8 && esz > 0 && ((v >> (8 * esz - 1)) & 1u)) v |= ~(uint64_t)0 << (8 * esz);
        return INTVAL((int64_t)v);
    }
    if (cls == 3) { if (esz == 4) { float f; memcpy(&f, tmp, 4); return REALVAL((double)f); } double d; memcpy(&d, tmp, 8); return REALVAL(d); }
    if (cls == 4) return INTVAL((int64_t)tmp[0]);
    if (cls == 5) return INTVAL(tmp[0] ? 1 : 0);
    if (cls == 6) { unsigned char *b = (unsigned char *)rt_str_alloc(PAS_SET_BYTES); memcpy(b, tmp, PAS_SET_BYTES); return BSTRVAL((char *)b, PAS_SET_BYTES); }
    return INTVAL(0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_rpatch(DESCR_t reg, long off, long kind, long rsz, DESCR_t val) {
    int cls = (int)(kind & 0xFF), esz = (int)((kind >> 8) & 0xFF);
    if (rsz < 0) rsz = 0;
    unsigned char *b = (unsigned char *)rt_str_alloc(rsz > 0 ? rsz : 1);
    memset(b, 0, (size_t)(rsz > 0 ? rsz : 1));
    if (IS_STR_fn(reg) && reg.s) memcpy(b, reg.s, (size_t)(reg.slen < (uint32_t)rsz ? reg.slen : (uint32_t)rsz));
    if (off >= 0 && esz > 0 && off + esz <= rsz) {
        size_t tsz = PAS_SET_BYTES;
        unsigned char tmp[tsz];
        memset(tmp, 0, tsz);
        if (cls == 1 || cls == 2) {
            uint64_t v = (uint64_t)(IS_REAL_fn(val) ? (int64_t)val.r : (int64_t)pas_ord_of(val));
            for (int k = 0; k < esz && k < 8; k++) tmp[k] = (unsigned char)(v >> (8 * k));
        } else if (cls == 3) {
            double d = IS_REAL_fn(val) ? val.r : (double)pas_ord_of(val);
            if (esz == 4) { float f = (float)d; memcpy(tmp, &f, 4); } else memcpy(tmp, &d, 8);
        } else if (cls == 4) tmp[0] = (unsigned char)pas_ord_of(val);
        else if (cls == 5) tmp[0] = pas_ord_of(val) ? 1 : 0;
        else if (cls == 6) pas_set_bits(val, tmp);
        memcpy(b + off, tmp, (size_t)(esz < (int)sizeof tmp ? esz : (int)sizeof tmp));
    }
    return BSTRVAL((char *)b, rsz);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_view_make(DESCR_t *args, int nargs) {
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
    vc->cellp = 0;
    vc->tbl = 0;
    vc->key = pas_view_key;
    vc->key_d = INTVAL((long)pas_ord_of(args[2]));
    vc->sv = args[0];
    vc->pos = (long)pas_ord_of(args[1]);
    vc->len = (long)pas_ord_of(args[3]);
    return NAMETRAP(vc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_view_get(VCELL_t *vc) { return pas_rdecode(rt_deref(vc->sv), vc->pos, (long)vc->key_d.i, vc->len); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_view_set(VCELL_t *vc, DESCR_t val) { rtx_try_rt_assign_var(vc->sv, pas_rpatch(rt_deref(vc->sv), vc->pos, (long)vc->key_d.i, vc->len, val)); return val; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_alloc_rec(DESCR_t *args, int nargs) {
    long n = pas_heap_take();
    DESCR_t *c = pas_heap_cell(n, 1);
    (void) nargs;
    if (c) *c = (args[0].v == DT_A && args[0].arr) ? args[0] : STRVAL(rt_heap_strdup_c("0"));
    return INTVAL(n);
}
