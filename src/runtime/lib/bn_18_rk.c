#include "runtime/lib/bn_head.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include "runtime/builtin_leaf.h"
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_15_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_18_rk.h"
#include "runtime/lib/bn_20_pl.h"
#include <time.h>
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern DESCR_t rt_deref(DESCR_t d);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_type_isa(const char *have, const char *want) {
    static const char *const up[][8] = { { "Bool", "Int", "Cool", "Any", "Numeric", "Real", 0 }, { "Order", "Int", "Cool", "Any", "Numeric", "Real", 0 }, { "Int", "Cool", "Any", "Numeric", "Real",
        0 }, { "Num", "Cool", "Any", "Numeric", "Real", 0 }, { "Rat", "Cool", "Any", "Numeric", "Real", "Rational", 0 }, { "FatRat", "Cool", "Any", "Numeric", "Real", "Rational", 0 }, { "Str", "Cool",
        "Any", "Stringy", 0 }, { "List", "Cool", "Any", "Positional", "Iterable", 0 }, { "Array", "List", "Cool", "Any", "Positional", "Iterable", 0 }, { "Range", "Cool", "Any", "Positional",
        "Iterable", 0 }, { "Hash", "Map", "Cool", "Any", "Associative", "Iterable", 0 }, { "Map", "Cool", "Any", "Associative", "Iterable", 0 }, { "Sub", "Routine", "Code", "Callable", "Any", 0 },
        { "Block", "Code", "Callable", "Any", 0 }, { "Code", "Callable", "Any", 0 }, { "Nil", "Cool", "Any", 0 }, { 0 } };
    if (!have || !want) return 0;
    if (!strcmp(have, want) || !strcmp(want, "Mu")) return 1;
    for (int i = 0; up[i][0]; i++) if (!strcmp(up[i][0], have)) { for (int j = 1; up[i][j]; j++) if (!strcmp(up[i][j], want)) return 1; return 0; }
    DatType *t = dat_find_type(have);
    if (!t) return 0;
    if (!strcmp(t->name, want) || !strcmp(want, "Any")) return 1;
    for (int i = 1; i < t->mro_len; i++) if (t->mro[i] && !strcmp(t->mro[i], want)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rk_str_succ_buf(const char *s, char *b) {
    size_t n = strlen(s);
    memcpy(b, s, n + 1);
    if (!n) return b;
    int p = (int) n - 1;
    while (p >= 0 && !isalnum((unsigned char) b[p])) p--;
    if (p < 0) { b[n - 1]++; return b; }
    for (int i = p; ; i--) {
        char c = b[i], kind;
        if (c == 'z') { b[i] = 'a'; kind = 'a'; } else if (c == 'Z') { b[i] = 'A'; kind = 'A'; } else if (c == '9') { b[i] = '0'; kind = '1'; } else { b[i]++; break; }
        if (i == 0 || !isalnum((unsigned char) b[i - 1])) { memmove(b + i + 1, b + i, strlen(b + i) + 1); b[i] = kind; break; }
    }
    return b;
}
static DESCR_t bta_succ(DESCR_t *args, int nargs, int _bop) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if ((IS_INT_fn(args[0]) || IS_REAL_fn(args[0]))) { int d = (_bop == 0) ? 1 : -1; if (IS_INT_fn(args[0])) *out = INTVAL((long)args[0].i + d); else *out = REALVAL(args[0].r + d); return _bo; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_succ(DESCR_t *args, int nargs) {
    if (!IS_INT_fn(args[0]) && !IS_REAL_fn(args[0]) && IS_STR_fn(args[0]) && !rk_typeobj_name(args[0])) {
        const char *sv = rk_cstr(args[0]);
        return STRVAL(rk_str_succ_buf(sv, (char *)rt_heap_alloc_c(strlen(sv) + 2)));
    }
    { const DESCR_t _r = bta_succ(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_pred(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_succ(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_mc_type_name(DESCR_t d) {
    if (rk_typeobj_name(d)) return rk_typeobj_name(d);
    if (d.v == DT_A && d.arr) return rk_is_range(d) ? "Range" : rk_is_pair(d) ? "Pair" : rk_is_seq(d) ? "Seq" : (rk_is_list(d) || rk_is_slip(d)) ? "List" : "Array";
    if (d.v == DT_T && d.tbl) return "Hash";
    if (rk_is_code(d)) return "Block";
    switch (d.v) {
        case DT_I:
        return "Int";
        case DT_BOOL:
        return "Bool";
        case DT_ORDER:
        return "Order";
        case DT_R:
        return "Num";
        case DT_S:
        return "Str";
        case DT_DATA:
        { if (d.slen == DATA_INST_SLEN && d.u && d.u->type && d.u->type->name) return rk_dat_tname(d.u->type->name); return "Any"; }
        default:
        return "Any";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_mc_is_subtype(const char *a, const char *b) {
    if (!a || !b) return 0;
    if (!strcmp(b, "Any") || !strcmp(b, "Mu") || !strcmp(b, "Cool")) return 1;
    if (!strcmp(a, b)) return 1;
    int a_numleaf = (!strcmp(a, "Int") || !strcmp(a, "Num") || !strcmp(a, "Rat") || !strcmp(a, "FatRat"));
    int a_pos = (!strcmp(a, "Array") || !strcmp(a, "List") || !strcmp(a, "Range") || !strcmp(a, "Seq")), a_ass = (!strcmp(a, "Hash") || !strcmp(a, "Map")),
        a_code = (!strcmp(a, "Block") || !strcmp(a, "Sub") || !strcmp(a, "Code"));
    if (!strcmp(b, "Positional") && a_pos) return 1;
    if (!strcmp(b, "Associative") && a_ass) return 1;
    if (!strcmp(b, "Iterable") && (a_pos || a_ass)) return 1;
    if (!strcmp(b, "List") && !strcmp(a, "Array")) return 1;
    if (!strcmp(b, "Map") && !strcmp(a, "Hash")) return 1;
    if ((!strcmp(b, "Callable") || !strcmp(b, "Code")) && a_code) return 1;
    if ((!strcmp(b, "Numeric") || !strcmp(b, "Real")) && a_numleaf) return 1;
    {
        extern int dat_mro(const char *name, const char **out, int max);
        const char *mro[bnd_dat_need(a, 0)];
        int n = dat_mro(a, mro, (int)(sizeof mro / sizeof mro[0]));
        for (int i = 0; i < n; i++) if (mro[i] && !strcmp(mro[i], b)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_mc_accepts(const char *ptype, DESCR_t arg) {
    if (!ptype || !strcmp(ptype, "Any") || !strcmp(ptype, "Mu")) return 1;
    int require_def = 0, require_undef = 0;
    const char *colon = strrchr(ptype, ':');
    const char *under = strrchr(ptype, '_');
    char base[strlen(ptype) + 1];
    base[0] = 0;
    if (colon && (!strcmp(colon, ":D") || !strcmp(colon, ":U"))) {
        require_def = !strcmp(colon, ":D");
        require_undef = !strcmp(colon, ":U");
        int bl = (int)(colon - ptype);
        memcpy(base, ptype, (size_t)bl);
        base[bl] = 0;
    } else if (under && (!strcmp(under, "_D") || !strcmp(under, "_U"))) {
        require_def = !strcmp(under, "_D");
        require_undef = !strcmp(under, "_U");
        int bl = (int)(under - ptype);
        memcpy(base, ptype, (size_t)bl);
        base[bl] = 0;
    } else {
        snprintf(base, sizeof base, "%s", ptype);
    }
    int is_undef = (arg.v == DT_SNUL) || rk_typeobj_name(arg);
    if (require_def && is_undef) return 0;
    if (require_undef && !is_undef) return 0;
    if (!strcmp(base, "Any") || !strcmp(base, "Mu")) return 1;
    if (require_undef && arg.v == DT_SNUL) return 0;
    if (!strcmp(base, "Cool")) return 1;
    return rt_mc_is_subtype(rt_mc_type_name(arg), base);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_mc_narrower(char (*ta)[32], char (*tb)[32], int na) {
    int narrower = 0, tied = 0;
    for (int i = 0; i < na; i++) {
        const char *A = ta[i], *B = tb[i];
        if (!strcmp(A, B)) { tied++; continue; }
        const char *cA = strrchr(A, ':'), *cB = strrchr(B, ':');
        if (!cA || (!strcmp(cA,":D") && !strcmp(cA,":U"))) cA = strrchr(A, '_');
        if (!cB || (!strcmp(cB,":D") && !strcmp(cB,":U"))) cB = strrchr(B, '_');
        int aD = cA && (!strcmp(cA, ":D")||!strcmp(cA, "_D")), aU = cA && (!strcmp(cA, ":U")||!strcmp(cA, "_U"));
        int bD = cB && (!strcmp(cB, ":D")||!strcmp(cB, "_D")), bU = cB && (!strcmp(cB, ":U")||!strcmp(cB, "_U"));
        int lA = cA && (aD||aU) ? (int)(cA-A) : (int)strlen(A), lB = cB && (bD||bU) ? (int)(cB-B) : (int)strlen(B);
        char baseA[lA + 1], baseB[lB + 1];
        { memcpy(baseA,A,lA); baseA[lA]=0; }
        { memcpy(baseB,B,lB); baseB[lB]=0; }
        int same_base = !strcmp(baseA, baseB);
        if (same_base && (aD||aU) && !(bD||bU)) { narrower++; continue; }
        if (same_base && !(aD||aU) && (bD||bU)) { continue; }
        if (rt_mc_is_subtype(baseA, baseB) && !rt_mc_is_subtype(baseB, baseA)) narrower++;
        else if (!rt_mc_is_subtype(baseA, baseB) && !rt_mc_is_subtype(baseB, baseA)) tied++;
    }
    return narrower > 0 && (narrower + tied == na);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_multi_meth_dispatch(const char *cname, const char *mname, DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (!cname || !mname) return 0;
    int nm = nargs - 2;
    if (nm < 0) return 0;
    DESCR_t *ma = &args[2];
    extern int dat_mro(const char *name, const char **out, int max);
    extern int rt_proc_enum_count(void);
    extern const char *rt_proc_enum_name(int i);
    const char *mro[bnd_dat_need(cname, 0)];
    int mn = dat_mro(cname, mro, (int)(sizeof mro / sizeof mro[0]));
    if (mn == 0) { mro[0] = cname; mn = 1; }
    int pcount = rt_proc_enum_count();
    int cidx[pcount * mn + 1], cpl[pcount * mn + 1], nc = 0;
    for (int ci = 0; ci < mn; ci++) {
        if (!mro[ci]) continue;
        char prefix[fmt_len("%s__%s$", mro[ci], mname)];
        int pl = snprintf(prefix, sizeof prefix, "%s__%s$", mro[ci], mname);
        for (int pi = 0; pi < pcount; pi++) { const char *pn = rt_proc_enum_name(pi); if (pn && !strncmp(pn, prefix, (size_t)pl)) { cidx[nc] = pi; cpl[nc] = pl; nc++; } }
    }
    char acc_types[nc + 1][8][32];
    const char *acc_pn[nc + 1];
    int acc_var[nc + 1], acc_fixed[nc + 1];
    int nacc = 0;
    for (int cj = 0; cj < nc; cj++) {
        const char *pn = rt_proc_enum_name(cidx[cj]);
        const char *p = pn + cpl[cj];
        const char *e = strchr(p, '$');
        int arity = atoi(p);
        int nt = 0;
        const char *q = e ? e + 1 : (const char *)0;
        while (q && nt < 8) {
            const char *nx = strchr(q, '$');
            int len = nx ? (int)(nx - q) : (int)strlen(q);
            if (len > 31) len = 31;
            memcpy(acc_types[nacc][nt], q, (size_t)len);
            acc_types[nacc][nt][len] = 0;
            nt++;
            if (!nx) break;
            q = nx + 1;
        }
        int isvar = (arity > 0 && arity <= nt && !strcmp(acc_types[nacc][arity - 1], "Slurpy"));
        int fixed = isvar ? arity - 1 : arity;
        if (isvar ? nm < fixed : arity != nm) continue;
        int ok = 1;
        for (int i = 0; i < fixed && i < nt; i++) if (!rt_mc_accepts(acc_types[nacc][i], ma[i])) { ok = 0; break; }
        if (!ok) continue;
        int dup = 0;
        for (int k = 0; k < nacc; k++) if (!strcmp(acc_pn[k], pn)) { dup = 1; break; }
        if (dup) continue;
        acc_pn[nacc] = pn;
        acc_var[nacc] = isvar;
        acc_fixed[nacc] = fixed;
        nacc++;
    }
    if (nacc == 0) return 0;
    int win = -1;
    for (int pass = 0; pass < 2 && win < 0; pass++) for (int i = 0; i < nacc; i++) {
        int beaten = 0;
        if (acc_var[i] != pass) continue;
        for (int j = 0; j < nacc; j++) { if (i == j || acc_var[j] != pass) continue; if (rt_mc_narrower(acc_types[j], acc_types[i], acc_fixed[i])) { beaten = 1; break; } }
        if (!beaten) { win = i; break; }
    }
    if (win < 0) win = 0;
    int total = 1 + nm;
    DESCR_t ca[total];
    ca[0] = args[0];
    for (int k = 0; k < nm; k++) ca[1 + k] = ma[k];
    *out = invoke_method_proc_rq(acc_pn[win], ca, total, rq);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_spec_need(const char *p) {
    int n = 2;
    while (*p == '-' || *p == '+' || *p == ' ' || *p == '0' || *p == '#') { n++; p++; }
    if (*p == '*') { n += 21; p++; } else while (*p >= '0' && *p <= '9') { n++; p++; }
    if (*p == '.') { n++; p++; if (*p == '*') n += 21; else while (*p >= '0' && *p <= '9') { n++; p++; } }
    return n;
}
void rk_sprintf_core(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen) {
    size_t cap = strlen(fmt) + 64, len = 0;
    char *buf = (char *)rt_wsb_alloc(cap);
    int ai = from;
    for (const char *p = fmt; *p; ) {
        if (*p != '%') { if (len + 2 > cap) { cap = cap * 2 + 8; char *nb = (char *)rt_wsb_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = *p++; continue; }
        const char *start = p;
        p++;
        if (*p == '%') { if (len + 2 > cap) { cap = cap * 2 + 8; char *nb = (char *)rt_wsb_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = '%'; p++; continue; }
        char spec[rk_spec_need(p)];
        int sp = 0;
        spec[sp++] = '%';
        while (*p == '-' || *p == '+' || *p == ' ' || *p == '0' || *p == '#') { spec[sp++] = *p; p++; }
        long width = -1;
        if (*p == '*') {
            width = (ai < nargs) ? (IS_INT_fn(args[ai]) ? (long)args[ai].i : (IS_REAL_fn(args[ai]) ? (long)args[ai].r : 0)) : 0;
            ai++;
            sp += snprintf(spec + sp, sizeof spec - sp, "%ld", width);
            p++;
        } else while (*p >= '0' && *p <= '9') {
            spec[sp++] = *p;
            p++;
        }
        if (*p == '.') {
            spec[sp++] = '.';
            p++;
            if (*p == '*') {
                long prec = (ai < nargs) ? (IS_INT_fn(args[ai]) ? (long)args[ai].i : (IS_REAL_fn(args[ai]) ? (long)args[ai].r : 0)) : 0;
                ai++;
                sp += snprintf(spec + sp, sizeof spec - sp, "%ld", prec);
                p++;
            } else while (*p >= '0' && *p <= '9') {
                spec[sp++] = *p;
                p++;
            }
        }
        char conv = *p;
        if (!conv) { if (len + 1 < cap) buf[len++] = '%'; break; }
        p++;
        DESCR_t a = (ai < nargs) ? args[ai] : NULVCL;
        ai++;
        char sb[256];
        if (conv != 's' && a.v == DT_DATA) {
            extern int rk_rat_to_real(DESCR_t, double *);
            extern int rk_rat_to_int(DESCR_t, long long *);
            double rv;
            long long iv;
            if (conv == 'd' || conv == 'i' || conv == 'u' || conv == 'x' || conv == 'X' || conv == 'o') { if (rk_rat_to_int(a, &iv)) a = INTVAL(iv); } else if (rk_rat_to_real(a, &rv)) a = REALVAL(rv);
        }
        if (conv == 'd' || conv == 'i' || conv == 'u' || conv == 'x' || conv == 'X' || conv == 'o') {
            long lv = IS_INT_fn(a) ? (long)a.i : (IS_REAL_fn(a) ? (long)a.r : atol(to_cstring(a, sb, sizeof sb)));
            char cspec[sp + 3];
            int cl = 0;
            for (int k = 0; k < sp; k++) cspec[cl++] = spec[k];
            cspec[cl++] = 'l';
            cspec[cl++] = conv;
            cspec[cl] = 0;
            size_t need = (size_t)fmt_len(cspec, lv);
            if (len + need > cap) { cap = len + need + 8; char *nb = (char *)rt_wsb_alloc(cap); memcpy(nb, buf, len); buf = nb; }
            snprintf(buf + len, cap - len, cspec, lv);
            len += strlen(buf + len);
        } else if (conv == 'e' || conv == 'E' || conv == 'f' || conv == 'F' || conv == 'g' || conv == 'G') {
            double dv = IS_REAL_fn(a) ? a.r : (IS_INT_fn(a) ? (double)a.i : atof(to_cstring(a, sb, sizeof sb)));
            char cspec[sp + 3];
            int cl = 0;
            for (int k = 0; k < sp; k++) cspec[cl++] = spec[k];
            cspec[cl++] = conv;
            cspec[cl] = 0;
            size_t need = (size_t)fmt_len(cspec, dv);
            if (len + need > cap) { cap = len + need + 8; char *nb = (char *)rt_wsb_alloc(cap); memcpy(nb, buf, len); buf = nb; }
            snprintf(buf + len, cap - len, cspec, dv);
            len += strlen(buf + len);
        } else if (conv == 'c') {
            long lv = IS_INT_fn(a) ? (long)a.i : (IS_REAL_fn(a) ? (long)a.r : atol(to_cstring(a, sb, sizeof sb)));
            char cspec[sp + 3];
            int cl = 0;
            for (int k = 0; k < sp; k++) cspec[cl++] = spec[k];
            cspec[cl++] = 'c';
            cspec[cl] = 0;
            size_t need = (size_t)fmt_len(cspec, (int)lv);
            if (len + need > cap) { cap = len + need + 8; char *nb = (char *)rt_wsb_alloc(cap); memcpy(nb, buf, len); buf = nb; }
            snprintf(buf + len, cap - len, cspec, (int)lv);
            len += strlen(buf + len);
        } else if (conv == 's') {
            const char *sv = to_cstring(a, sb, sizeof sb);
            if (!sv) sv = "";
            char cspec[sp + 3];
            int cl = 0;
            for (int k = 0; k < sp; k++) cspec[cl++] = spec[k];
            cspec[cl++] = 's';
            cspec[cl] = 0;
            size_t need = (size_t)fmt_len(cspec, sv);
            if (len + need > cap) { cap = len + need + 8; char *nb = (char *)rt_wsb_alloc(cap); memcpy(nb, buf, len); buf = nb; }
            snprintf(buf + len, cap - len, cspec, sv);
            len += strlen(buf + len);
        } else if (conv == 'b') {
            unsigned long uv = IS_INT_fn(a) ? (unsigned long)a.i : (IS_REAL_fn(a) ? (unsigned long)a.r : (unsigned long)atol(to_cstring(a, sb, sizeof sb)));
            char bits[72];
            int bn = 0;
            if (uv == 0) bits[bn++] = '0';
            else { char tmp[72]; int tn = 0; while (uv) { tmp[tn++] = (char)('0' + (uv & 1)); uv >>= 1; } while (tn) bits[bn++] = tmp[--tn]; }
            bits[bn] = 0;
            long w = 0;
            int zero = 0;
            for (int k = 1; k < sp; k++) { if (spec[k] == '0') zero = 1; else if (spec[k] >= '1' && spec[k] <= '9') { w = w * 10 + (spec[k] - '0'); } }
            int pad = (int)w - bn;
            if (pad > 0) {
                char pc = zero ? '0' : ' ';
                for (int k = 0; k < pad; k++) { if (len + 1 >= cap) { cap = cap * 2 + 8; char *nb = (char *)rt_wsb_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = pc; }
            }
            for (int k = 0; k < bn; k++) { if (len + 1 >= cap) { cap = cap * 2 + 8; char *nb = (char *)rt_wsb_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = bits[k]; }
            continue;
        } else {
            for (const char *q = start; q <= start + (p - start) - 1; q++) {
                if (len + 1 >= cap) { cap = cap * 2 + 8; char *nb = (char *)rt_wsb_alloc(cap); memcpy(nb, buf, len); buf = nb; }
                buf[len++] = *q;
            }
            continue;
        }
    }
    buf[len] = 0;
    *outp = buf;
    if (outlen) *outlen = len;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_scrip_exe(void) {
    char *self = rt_wsb_alloc(4096), *path = rt_wsb_alloc(4200);
    ssize_t k = readlink("/proc/self/exe", self, 4095);
    self[k > 0 ? k : 0] = '\0';
    const char *bn = strrchr(self, '/');
    if (bn && !strcmp(bn + 1, "scrip")) return rt_heap_strdup_c(self);
    struct { const char *dli_fname; void *dli_fbase; const char *dli_sname; void *dli_saddr; } di;
    extern int dladdr(const void *, void *);
    if (dladdr((void *) rk_scrip_exe, &di) && di.dli_fname) {
        char *lib = rt_wsb_alloc(strlen(di.dli_fname) + 1);
        strcpy(lib, di.dli_fname);
        char *sl = strrchr(lib, '/');
        if (sl) { *sl = '\0'; snprintf(path, 4200, "%s/../scrip", lib); if (access(path, X_OK) == 0) return rt_heap_strdup_c(path); }
    }
    return "scrip";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *rk_slurp_file(const char *fn) {
    FILE *f = fopen(fn, "rb");
    if (!f) return rt_wsb_alloc(1);
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n < 0) n = 0;
    char *b = rt_wsb_alloc((size_t) n + 1);
    size_t r = n ? fread(b, 1, (size_t) n, f) : 0;
    b[r] = '\0';
    fclose(f);
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_run_child(const char *code, const char *input, DESCR_t argl, DESCR_t *got_out, DESCR_t *got_err) {
    char dir[] = "/tmp/rk_is_run_XXXXXX";
    if (!mkdtemp(dir)) return 255;
    size_t dl = strlen(dir);
    char pf[dl + 16], inf[dl + 16], of[dl + 16], ef[dl + 16];
    snprintf(pf, sizeof pf, "%s/prog.raku", dir);
    snprintf(inf, sizeof inf, "%s/in.txt", dir);
    snprintf(of, sizeof of, "%s/out.txt", dir);
    snprintf(ef, sizeof ef, "%s/err.txt", dir);
    FILE *f = fopen(pf, "wb");
    if (f) { fputs(code, f); fclose(f); }
    f = fopen(inf, "wb");
    if (f) { fputs(input, f); fclose(f); }
    rk_av_t al = rk_av(argl);
    const char *exe = rk_scrip_exe();
    const char *argv[al.n + 5];
    int k = 0;
    argv[k++] = exe;
    argv[k++] = pf;
    if (al.n) { argv[k++] = "--"; for (int i = 0; i < al.n; i++) argv[k++] = rk_av_text(al, i); }
    argv[k] = NULL;
    fflush(NULL);
    pid_t pid = fork();
    int status = 255;
    if (pid == 0) {
        int fi = open(inf, O_RDONLY), fo = open(of, O_WRONLY | O_CREAT | O_TRUNC, 0600), fe = open(ef, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        if (fi >= 0) dup2(fi, 0);
        if (fo >= 0) dup2(fo, 1);
        if (fe >= 0) dup2(fe, 2);
        execv(exe, (char *const *) argv);
        execvp(exe, (char *const *) argv);
        _exit(127);
    }
    if (pid > 0) { int ws = 0; if (waitpid(pid, &ws, 0) == pid) status = WIFEXITED(ws) ? WEXITSTATUS(ws) : 128 + WTERMSIG(ws); }
    *got_out = STRVAL(rt_heap_strdup_c(rk_slurp_file(of)));
    *got_err = STRVAL(rt_heap_strdup_c(rk_slurp_file(ef)));
    unlink(pf);
    unlink(inf);
    unlink(of);
    unlink(ef);
    rmdir(dir);
    return status;
}
int rk_fast_rat_builtin(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    if (fn[0] != '_' || fn[1] != '_' || fn[2] != 'r' || fn[3] != 'k' || fn[4] != '_') return 0;
    if (!strcmp(fn, "__rk_exend") && nargs == 1) {
        extern int rk_rat_is(DESCR_t);
        extern int rk_rat_method(const char *, DESCR_t, DESCR_t *, DESCR_t *);
        extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
        DESCR_t h = args[0];
        if (IS_INT_fn(h)) { *out = h.i == INT64_MIN ? rt_num_arith(h, INTVAL(1), BINOP_SUB) : INTVAL(h.i - 1); return 1; }
        if (IS_REAL_fn(h)) { *out = (h.r == h.r && fabs(h.r) < 9.0e18) ? INTVAL((long long) ceil(h.r) - 1) : h; return 1; }
        if (rk_rat_is(h)) { DESCR_t c; if (rk_rat_method("ceiling", h, NULL, &c) && IS_INT_fn(c)) { *out = INTVAL(c.i - 1); return 1; } }
        *out = rt_num_arith(h, INTVAL(1), BINOP_SUB);
        return 1;
    }
    if (!strcmp(fn, "__rk_ratnew") && nargs == 3) {
        extern DESCR_t rk_rat_new(DESCR_t, DESCR_t, int);
        const char *kn = VARVAL_fn(args[0]);
        *out = rk_rat_new(args[1], args[2], kn && kn[0] == 'F');
        return 1;
    }
    if (!strcmp(fn, "__rk_ratlit") && nargs == 1) {
        extern DESCR_t rk_rat_lit(const char *);
        char sb[128];
        const char *t = to_cstring(args[0], sb, sizeof sb);
        *out = rk_rat_lit(t ? t : "0");
        return 1;
    }
    if (!strcmp(fn, "__rk_div") && nargs == 2) {
        extern void rt_script_die_surface(const char *msg);
        extern int rk_rat_binop(DESCR_t, DESCR_t, int, DESCR_t *);
        extern DESCR_t rk_rat_div(DESCR_t, DESCR_t);
        DESCR_t a = args[0], b = args[1];
        if ((a.v == DT_DATA || b.v == DT_DATA) && rk_rat_binop(a, b, BINOP_DIV, out)) return 1;
        if ((a.v == DT_BIG || b.v == DT_BIG) && (a.v == DT_BIG || IS_INT_fn(a)) && (b.v == DT_BIG || IS_INT_fn(b))) { *out = rk_rat_div(a, b); return 1; }
        int ai = IS_INT_fn(a), arl = IS_REAL_fn(a), bi = IS_INT_fn(b), brl = IS_REAL_fn(b);
        double ad = arl ? a.r : (ai ? (double)a.i : 0.0), bd = brl ? b.r : (bi ? (double)b.i : 0.0);
        if (!ai && !arl) { char sa[64]; const char *cs = to_cstring(a, sa, sizeof sa); ad = cs ? strtod(cs, (char **)0) : 0.0; }
        if (!bi && !brl) { char sb[64]; const char *cs = to_cstring(b, sb, sizeof sb); bd = cs ? strtod(cs, (char **)0) : 0.0; }
        if (ai && bi) { *out = rk_rat_div(a, b); return 1; }
        if (bd == 0.0) { rt_script_die_surface("Attempt to divide by zero"); *out = FAILDESCR; return 1; }
        *out = REALVAL(ad / bd);
        return 1;
    }
    if (!strcmp(fn, "__rk_pow") && nargs == 2) {
        extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
        extern DESCR_t rk_rat_pow(DESCR_t, DESCR_t);
        extern int rk_rat_binop(DESCR_t, DESCR_t, int, DESCR_t *);
        DESCR_t a = args[0], b = args[1];
        int ai = a.v == DT_I || a.v == DT_BIG || a.v == DT_BOOL, bi = b.v == DT_I || b.v == DT_BIG || b.v == DT_BOOL;
        if (ai && bi) { *out = rk_rat_pow(a, b); return 1; }
        if ((a.v == DT_DATA || b.v == DT_DATA) && rk_rat_binop(a, b, BINOP_POW, out)) return 1;
        if ((IS_REAL_fn(a) || IS_INT_fn(a) || a.v == DT_BIG) && (IS_REAL_fn(b) || IS_INT_fn(b) || b.v == DT_BIG)) { *out = REALVAL(pow(to_real(a), to_real(b))); return 1; }
        *out = rt_num_arith(a, b, BINOP_POW);
        return 1;
    }
    return 0;
}
static int rk_mc_named_is(const rk_mc_named_t *d, const char *key) { return !strncmp(d->name, key, (size_t)d->nlen) && !key[d->nlen]; }
static int rk_mc_named_of(const char *types, int arity, rk_mc_named_t *out, int max) {
    int seg = 0, n = 0;
    for (const char *q = types; q; seg++) {
        const char *nx = strchr(q, '$');
        int len = nx ? (int)(nx - q) : (int)strlen(q);
        if (seg >= arity && len > 4 && !strncmp(q, "N_", 2) && n < max) { out[n].name = q + 2; out[n].nlen = len - 4; out[n].req = q[len - 1] == 'R'; n++; }
        q = nx ? nx + 1 : (const char *)0;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_multi_named_winner(const char *base, const DESCR_t *pos, int npos, const DESCR_t *kv, int nkv, int *none) {
    char prefix[fmt_len("%s$", base)];
    int pl = snprintf(prefix, sizeof prefix, "%s$", base);
    extern int rt_proc_enum_count(void);
    extern const char *rt_proc_enum_name(int i);
    int pcount = rt_proc_enum_count();
    int cidx[pcount + 1], nc = 0;
    for (int pi = 0; pi < pcount; pi++) { const char *pn = rt_proc_enum_name(pi); if (pn && !strncmp(pn, prefix, (size_t)pl)) cidx[nc++] = pi; }
    char acc_types[nc + 1][8][32];
    int acc_var[nc + 1], acc_fixed[nc + 1], acc_nn[nc + 1], acc_idx[nc + 1];
    int nacc = 0, ncand = nc;
    for (int cj = 0; cj < nc; cj++) {
        int pi = cidx[cj];
        const char *pn = rt_proc_enum_name(pi);
        const char *p = pn + pl;
        const char *e = strchr(p, '$');
        int arity = atoi(p), nt = 0;
        const char *q = e ? e + 1 : (const char *)0;
        while (q && nt < 8) {
            const char *nx = strchr(q, '$');
            int len = nx ? (int)(nx - q) : (int)strlen(q);
            if (len > 31) len = 31;
            memcpy(acc_types[nacc][nt], q, (size_t)len);
            acc_types[nacc][nt][len] = 0;
            nt++;
            if (!nx) break;
            q = nx + 1;
        }
        int isvar = (arity > 0 && arity <= nt && !strcmp(acc_types[nacc][arity - 1], "Slurpy"));
        int fixed = isvar ? arity - 1 : arity;
        if (isvar ? (npos < fixed) : (arity != npos)) continue;
        int ok = 1;
        for (int i = 0; i < fixed && i < nt; i++) if (!rt_mc_accepts(acc_types[nacc][i], pos[i])) { ok = 0; break; }
        if (!ok) continue;
        rk_mc_named_t nmd[16];
        int nnm = rk_mc_named_of(e ? e + 1 : (const char *)0, arity, nmd, 16);
        for (int k = 0; k < nkv && ok; k++) {
            char kb[128];
            const char *key = to_cstring(kv[2 * k], kb, sizeof kb);
            int hit = 0;
            for (int i = 0; i < nnm; i++) if (key && rk_mc_named_is(&nmd[i], key)) hit = 1;
            if (!hit) ok = 0;
        }
        for (int i = 0; i < nnm && ok; i++) {
            if (!nmd[i].req) continue;
            int hit = 0;
            for (int k = 0; k < nkv; k++) { char kb[128]; const char *key = to_cstring(kv[2 * k], kb, sizeof kb); if (key && rk_mc_named_is(&nmd[i], key)) hit = 1; }
            if (!hit) ok = 0;
        }
        if (!ok) continue;
        acc_var[nacc] = isvar;
        acc_fixed[nacc] = fixed;
        acc_nn[nacc] = nnm;
        acc_idx[nacc] = pi;
        nacc++;
    }
    *none = ncand == 0;
    if (nacc == 0) return (const char *)0;
    int win = -1;
    for (int pass = 0; pass < 2 && win < 0; pass++) for (int i = 0; i < nacc; i++) {
        int beaten = 0;
        if (acc_var[i] != pass) continue;
        for (int j = 0; j < nacc; j++) { if (i == j || acc_var[j] != pass) continue; if (rt_mc_narrower(acc_types[j], acc_types[i], acc_fixed[i])) { beaten = 1; break; } }
        if (!beaten && (win < 0 || acc_nn[i] > acc_nn[win])) win = i;
    }
    if (win < 0) win = 0;
    return rt_proc_enum_name(acc_idx[win]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_multi_winner(const char *base, const DESCR_t *aa, int na) {
    char prefix[fmt_len("%s$", base)];
    int pl = snprintf(prefix, sizeof prefix, "%s$", base);
    extern int rt_proc_enum_count(void);
    extern const char *rt_proc_enum_name(int i);
    int pcount = rt_proc_enum_count();
    int cidx[pcount + 1], nc = 0;
    for (int pi = 0; pi < pcount; pi++) { const char *pn = rt_proc_enum_name(pi); if (pn && !strncmp(pn, prefix, (size_t)pl)) cidx[nc++] = pi; }
    char acc_types[nc + 1][8][32];
    int acc_idx[nc + 1], acc_var[nc + 1], acc_fixed[nc + 1], acc_nn[nc + 1];
    int nacc = 0;
    for (int cj = 0; cj < nc; cj++) {
        int pi = cidx[cj];
        const char *pn = rt_proc_enum_name(pi);
        const char *p = pn + pl;
        const char *e = strchr(p, '$');
        int arity = atoi(p);
        int nt = 0;
        const char *q = e ? e + 1 : (const char *)0;
        while (q && nt < 8) {
            const char *nx = strchr(q, '$');
            int len = nx ? (int)(nx - q) : (int)strlen(q);
            if (len > 31) len = 31;
            memcpy(acc_types[nacc][nt], q, (size_t)len);
            acc_types[nacc][nt][len] = 0;
            nt++;
            if (!nx) break;
            q = nx + 1;
        }
        int isvar = (arity > 0 && arity <= nt && !strcmp(acc_types[nacc][arity - 1], "Slurpy"));
        int fixed = isvar ? arity - 1 : arity;
        if (isvar ? (na < fixed) : (arity != na)) continue;
        rk_mc_named_t nmd[16];
        int nnm = rk_mc_named_of(e ? e + 1 : (const char *)0, arity, nmd, 16), reqn = 0;
        for (int i = 0; i < nnm; i++) if (nmd[i].req) reqn = 1;
        if (reqn) continue;
        int ok = 1;
        for (int i = 0; i < fixed && i < nt; i++) if (!rt_mc_accepts(acc_types[nacc][i], aa[i])) { ok = 0; break; }
        if (!ok) continue;
        acc_var[nacc] = isvar;
        acc_fixed[nacc] = fixed;
        acc_nn[nacc] = nnm;
        acc_idx[nacc] = pi;
        nacc++;
    }
    if (nacc == 0) return (const char *)0;
    int win = -1;
    for (int pass = 0; pass < 2 && win < 0; pass++) for (int i = 0; i < nacc; i++) {
        int beaten = 0;
        if (acc_var[i] != pass) continue;
        for (int j = 0; j < nacc; j++) { if (i == j || acc_var[j] != pass) continue; if (rt_mc_narrower(acc_types[j], acc_types[i], acc_fixed[i])) { beaten = 1; break; } }
        if (!beaten && (win < 0 || acc_nn[i] > acc_nn[win])) win = i;
    }
    if (win < 0) win = 0;
    return rt_proc_enum_name(acc_idx[win]);
}
DESCR_t bt___rk_mkcplx_i(DESCR_t *args, int nargs) {
    { (void) args; (void) nargs; extern DESCR_t rt_cplx_make(double, double); return rt_cplx_make(0.0, 1.0); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_exit(DESCR_t *args, int nargs) { if (nargs == 1) { long code = IS_INT_fn(args[0]) ? args[0].i : 0; exit((int) code); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta_wall_us(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 0) {
        struct timespec _ts;
        clock_gettime(CLOCK_MONOTONIC, &_ts);
        int64_t us = (int64_t)_ts.tv_sec * 1000000 + (int64_t)_ts.tv_nsec / 1000;
        return INTVAL((_bop == 0) ? us : us / 1000);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_wall_us(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_wall_us(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_wall_ms(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_wall_us(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
int bt___multi_call(DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (nargs >= 1) {
        const char *base = VARVAL_fn(args[0]);
        if (!base) { *out = FAILDESCR; return 1; }
        int na = nargs - 1;
        DESCR_t *aa = &args[1];
        const char *wname = rt_multi_winner(base, aa, na);
        if (!wname) {
            extern void rt_script_die_surface(const char *msg);
            char m[fmt_len("Cannot resolve caller %s(...); no candidate matches the argument types", base)];
            snprintf(m, sizeof m, "Cannot resolve caller %s(...); no candidate matches the argument types", base);
            rt_script_die_surface(m);
            *out = FAILDESCR;
            return 1;
        }
        rt_call_args_need(na);
        for (int k = 0; k < na; k++) CALL_ARGS[k] = aa[k];
        *out = rk_proc_descr_rq(wname, na, rq);
        return 1;
    }
    return 0;
}
DESCR_t bt___rk_run_child(DESCR_t *args, int nargs) {
    if (nargs >= 2) {
        DESCR_t gout, gerr;
        const char *cs = rk_cstr(args[0]), *is = rk_cstr(args[1]);
        char cb[strlen(cs) + 1], ib[strlen(is) + 1];
        strcpy(cb, cs);
        strcpy(ib, is);
        int st = rk_run_child(cb, ib, nargs > 2 ? args[2] : rk_mk_arr(NULL, 0), &gout, &gerr);
        DESCR_t r3[3] = { INTVAL(st), gout, gerr };
        return rk_mk_arr(r3, 3);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_deref(DESCR_t *args, int nargs) { if (nargs == 1) { extern DESCR_t rt_deref(DESCR_t d); return rt_deref(args[0]); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_attr_assign(DESCR_t *args, int nargs) {
    if (nargs != 3) return (DESCR_t){ .v = RTX_NOT_HANDLED };
    extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
    const char *fname = VARVAL_fn(args[1]);
    DESCR_t *cell = data_field_ptr(fname ? fname : "", args[0]);
    if (!cell) return args[2];
    if (cell->v == DT_N) { DESCR_t rv = rtx_try_rt_assign_var(*cell, args[2]); return IS_FAIL_fn(rv) ? args[2] : rv; }
    *cell = args[2];
    return args[2];
}
DESCR_t bt___rk_elem_ref(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        extern ARBLK_t *array_new(int lo, int hi);
        DESCR_t c = args[0];
        long idx = IS_INT_fn(args[1]) ? (long) args[1].i : IS_REAL_fn(args[1]) ? (long) args[1].r : -1L;
        ARBLK_t *b = (c.v == DT_A && c.arr && !rk_is_seq(c)) ? (ARBLK_t *) c.arr : NULL;
        int own = b && !b->proto && b->ndim == 1 && b->lo == 0 && idx >= 0 && idx <= b->hi;
        if (own && rk_is_cell(b->data[idx])) { return b->data[idx]; }
        ARBLK_t *bx = array_new(0, 0);
        bx->data[0] = own ? b->data[idx] : rk_typeobj("Any");
        DESCR_t cell = (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *) &bx->data[0] };
        if (own) { b->data[idx] = cell; b->proto_bare |= RK_ARR_BOUND; }
        return cell;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
